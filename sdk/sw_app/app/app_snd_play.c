/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_snd.c
 * @brief	application sound thread
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <alsa/asoundlib.h>

#include "app_comm.h"
#include "app_main.h"
#include "app_gmem.h"
#include "app_cap.h"
#include "app_snd_play.h"
#include "app_set.h"
#include "app_rtsptx.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#if SYS_CONFIG_SND_OUT

//#define DBG_SND_DUMP					1

#ifdef DBG_SND_DUMP
#define SND_DUMP_MAX_FRAMES				100
#endif

#define APP_SND_PCM_BITS				16

/* PLAYBACK (BACKCHANNEL) SOUND PARAM */
#define APP_SND_BC_SRATE				8000  //# for baresip
#define APP_SND_BC_PTIME				80    //# 고정해야 함.
#define APP_SND_BC_CH					1     /* sound channel */
#define FRAME_PER_BYTES					(APP_SND_BC_CH * APP_SND_PCM_BITS / 8)   /* sound channel */

#define	SIGN_BIT						(0x80)		/* Sign bit for a u-law byte. */
#define	QUANT_MASK						(0xf)		/* Quantization field mask. */
#define	NSEGS							(8)		/* Number of u-law segments. */
#define	SEG_SHIFT						(4)		/* Left shift for segment number. */
#define	SEG_MASK						(0x70)		/* Segment field mask. */
#define	BIAS							(0x84)		/* Bias for linear code. */

#define BCPLAY_MSG_KEY					0x185EA
#define BCPLAY_MSG_TYPE_TO_MAIN         1
#define BCPLAY_CMD_READY				(0x624)
#define BCPLAY_CMD_CONFIG				(0x625)
#define BCPLAY_CMD_REQ_DATA			    (0x626) /* request audio data */
#define BCPLAY_CMD_AUD_DATA			    (0x627) /* delivery audio data */
#define BCPLAY_RCV_SIZE 				2048

//########################### SOUND INFO PARAM ############################################
#define APP_SND_INFO_PTIME				80    //# 고정해야 함.

#define MKTAG(a,b,c,d)			((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
#define TAG_RIFF				MKTAG('R','I','F','F')
#define TAG_WAVE				MKTAG('W','A','V','E')
#define TAG_DATA				MKTAG('d','a','t','a')

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
#ifdef DBG_SND_DUMP
struct dwav_header {
	char chunk_id[4];
	unsigned int chunk_size;
	char format[4];
	char subchunk1_id[4];
	unsigned int subchunk1_size;
	unsigned short int audio_format;
	unsigned short int channels;
	unsigned int sample_rate;
	unsigned int byte_range;
	unsigned short int block_align;
	unsigned short int bits_per_sample;
	char subchunk2_id[4];
	unsigned int subchunk2_size;
};
#endif

typedef struct {
	app_thr_obj mObj;	 //# message receive thread
	
	int qid;
	int shmid;
	int len;
	
	Uint8 sbuf[BCPLAY_RCV_SIZE];
	
	OSA_MutexHndl mutex_bcplay;
	
} app_snd_bcplay_t;

typedef struct {
	long mtext;
	int cmd;
	int len;
	
	Uint8 sbuf[BCPLAY_RCV_SIZE];
} bcplay_to_main_msg_t;

//################# WAVE DATA TAG #################################################################
typedef struct {
	int sz;	//# size
	int ch;	//# channel
	int sr;	//# sample rate
	int bs;	//# bits_per_sample

} wave_info_t;

typedef struct {
	unsigned int magic;	 /* 'RIFF' */
	unsigned int length; /* file_len */
	unsigned int type;	 /* 'WAVE' */
} wave_header;

typedef struct {
	char chunk_id[4];
	unsigned int chunk_size;
	char format[4];
	char subchunk1_id[4];
	unsigned int subchunk1_size;
	unsigned short int audio_format;
	unsigned short int channels;
	unsigned int sample_rate;
	unsigned int byte_range;
	unsigned short int block_align;
	unsigned short int bits_per_sample;
	//char subchunk2_id[4];
	//unsigned int subchunk2_size;
} wav_header;

typedef struct {
	app_thr_obj iObj;	 //#
	
	char filename[256];      //32
	char *buf;
	
	wave_info_t winfo;
	int so_init;
	
} app_snd_iplay_t;

static app_snd_bcplay_t snd_bc_obj;
static app_snd_bcplay_t *isnd_bc=&snd_bc_obj;

static app_snd_iplay_t snd_info_obj;
static app_snd_iplay_t *isnd_info=&snd_info_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    ALSA Initialize
*****************************************************************************/
#define SND_PLAY_DEVNAME		"pasymed" //"plughw:0,0"
static snd_pcm_t * __snd_out_init(int ch, int rate, int period_frames)
{
	snd_pcm_t *handle = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;

	snd_pcm_uframes_t buffer_size, period_size;
	
	unsigned int freq, nchannels;
	
	if (snd_pcm_open(&handle, SND_PLAY_DEVNAME, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
		TRACE_INFO("could not open aix3x play device!\n");
		goto out;
	}
	
	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_hw_params_any(handle, hw_params);
	snd_pcm_hw_params_set_access(handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);

	/* Set sample format */
	snd_pcm_hw_params_set_format(handle, hw_params, SND_PCM_FORMAT_S16_LE);

	/* Set sample rate. If the exact rate is not supported */
	freq = (unsigned int)rate;
	if (snd_pcm_hw_params_set_rate_near(handle, hw_params, &freq, 0) < 0)
		goto out;
	
	nchannels = (unsigned int)ch;
	snd_pcm_hw_params_set_channels(handle, hw_params, nchannels);
	
	period_size = period_frames;
	buffer_size = (period_size * 4); //# alsa 표준
    if (snd_pcm_hw_params_set_period_size_near(handle, hw_params, &period_size, 0) < 0) {
        TRACE_INFO("Failed to set period size to %ld\n", period_size);
        goto out;
    }

    if (snd_pcm_hw_params_set_buffer_size_near(handle, hw_params, &buffer_size) < 0){
        TRACE_INFO("Failed to set buffer size to %ld\n", buffer_size);
        goto out;
    }

    /* PCM device and prepare device  */
    if (snd_pcm_hw_params(handle, hw_params) < 0)
		goto out;

#if 0	
	/* returned approximate maximum buffer size in frames  */
	snd_pcm_hw_params_get_period_size(hw_params, &period_size, 0);
	snd_pcm_hw_params_get_buffer_size(hw_params, &buffer_size);
	TRACE_INFO(" ALSA Period Size %ld\n", period_size);
	TRACE_INFO(" ALSA Buffer Size %ld\n", buffer_size);
#endif

	/* prepare for audio device */
	if (snd_pcm_prepare(handle) < 0) {
        TRACE_ERR("Could not prepare handle!\n");
        goto out;
    }
	return (snd_pcm_t *)handle;
	
out:
	snd_pcm_hw_params_free(hw_params);
	TRACE_INFO("alsa: init failed!!\n");
	return NULL;
}

static int __snd_out_write(snd_pcm_t *handle, void *buf, int data_size)
{
	char *data = (char *)buf;
	size_t count;
	
	count = data_size / FRAME_PER_BYTES;
	
	while (data_size > 0)
	{
		snd_pcm_sframes_t w;
		
		w = snd_pcm_writei(handle, data, count);
		if (w < 0) {
			if (w == -EAGAIN) {
				//TRACE_INFO("bcsnd pcm write wait(required 100ms)!!\n");
				snd_pcm_wait(handle, 100);
			} else if (w == -EPIPE) {
				//TRACE_INFO("bcsnd pcm write underrun!!\n");
				snd_pcm_prepare(handle);
			} else if (w == -ESTRPIPE) {
				//TRACE_INFO("bcsnd pcm write resume!!\n");
				snd_pcm_resume(handle);
			} else if (w < 0) {
				TRACE_INFO("backchannel sound device write error!!\n");
			}
		} else if ((w >= 0) && ((size_t)w < count)) {
			/* blocking 장치라 이 경우가 발생하는 지 잘 모름? */
			//snd_pcm_wait(handle, 100);
		}
		
		if (w > 0) {
			data += (w * FRAME_PER_BYTES);
			data_size -= (w * FRAME_PER_BYTES);
		}
	}

	return 0;
}

static void __snd_set_swparam(snd_pcm_t *handle, size_t frames)
{
	snd_pcm_sw_params_t *sw_params = NULL;
	
	if (handle != NULL) {
		/* set ALSA software params */
		snd_pcm_sw_params_alloca(&sw_params);
		snd_pcm_sw_params_current(handle, sw_params);
		snd_pcm_sw_params_set_avail_min(handle, sw_params, frames);
		snd_pcm_sw_params_set_start_threshold(handle, sw_params, frames*4);
		snd_pcm_sw_params_set_stop_threshold(handle, sw_params, frames*4);
		snd_pcm_sw_params(handle, sw_params);
	}
}

static void __snd_out_exit(snd_pcm_t *handle)
{
	if (handle != NULL) {
		/* Stop PCM device after pending frames have been played */
		snd_pcm_nonblock(handle, 0);
		/* Stop PCM device and drop pending frames */
    	//snd_pcm_drop(pcm_handle);
		snd_pcm_drain(handle);
		//snd_pcm_nonblock(handle, 1);
		snd_pcm_hw_free(handle);
		snd_pcm_close(handle);
	}
}

//############################################# BACK-CHANNEL SOUND PLAY HELPER ########################################################################
/*
 * ulaw2linear() - Convert a u-law value to 16-bit linear PCM
 *
 * First, a biased linear code is derived from the code word. An unbiased
 * output can then be obtained by subtracting 33 from the biased code.
 *
 * Note that this function expects to be passed the complement of the
 * original code word. This is in keeping with ISDN conventions.
 */
static int ulaw2linear(unsigned char u_val)
{
	int t;

	/* Complement to obtain normal u-law value. */
	u_val = ~u_val;

	/*
	 * Extract and bias the quantization bits. Then
	 * shift up by the segment number and subtract out the bias.
	 */
	t = ((u_val & QUANT_MASK) << 3) + BIAS;
	t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

	return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
}

static int bcplay_recv_msg(void)
{
	bcplay_to_main_msg_t msg;
	
	if (msgrcv(isnd_bc->qid, &msg, sizeof(bcplay_to_main_msg_t)-sizeof(long), 0, 0) == -1) {
		perror("msgrcv:");
		return -1;
	}
	
	isnd_bc->len = msg.len;
	memcpy(isnd_bc->sbuf, msg.sbuf, msg.len);

	return msg.cmd;
}

#define BC_SAMPLE_BUFFER_SZ		4096 
static void *THR_snd_bcplay_main(void *prm)
{
	app_thr_obj *tObj = &isnd_bc->mObj;
	snd_pcm_t *h_pcm;
	
	int cmd, exit=0;
	size_t pframes = 0;
	unsigned short *sampv=NULL; /* playback sound buffer */
	
#ifdef DBG_SND_DUMP
	struct dwav_header header;
	FILE *file = NULL;
	int dump_cnt=0;
#endif		

	tObj->active = 1;
	TRACE_INFO("enter...\n");
	
	/* 
	 * sound period size (프레임 단위로 설정함)
	 * 1초:sample rate = PTIME(ms):x
	 * 1000(ms):8000(8KHz sampling rate) = 80(ms):x
	 * x(frame) = samplerate * 80 / 1000
	 */
	pframes = APP_SND_BC_SRATE * APP_SND_BC_PTIME / 1000; /* secound -> ms */
	/* Initialize ALSA Speaker */
	h_pcm = __snd_out_init(APP_SND_BC_CH, APP_SND_BC_SRATE, pframes);
	if (h_pcm == NULL) {
		TRACE_ERR("Failed to init plughw:0,0 device!\n");
		return NULL;
	}
	
	LOGD("[main] Initializing Backchannel Audio Device success!\n");
	__snd_set_swparam(h_pcm, pframes);
	sampv = (unsigned short *)malloc(BC_SAMPLE_BUFFER_SZ); //# 최대 크기로 가정함.
	if (sampv == NULL) {
		__snd_out_exit(h_pcm);
		return NULL;
	}

#ifdef DBG_SND_DUMP
	file = fopen("/mmc/record.wav", "w+");
	if (file == NULL) {
		TRACE_INFO("Failed to open file!\n");
	}
	
	/* init wave header */
	memcpy(header.chunk_id,"RIFF",4);
	header.chunk_size = 0;
	memcpy(header.format, "WAVE",4);
	memcpy(header.subchunk1_id, "fmt ", 4);

	header.subchunk1_size = 16;
	header.audio_format = 1;
	header.channels = APP_SND_BC_CH;
	header.sample_rate = APP_SND_BC_SRATE;
	header.bits_per_sample = 16;
	header.block_align = header.channels * header.bits_per_sample / 8;
	header.byte_range = header.sample_rate * header.block_align;
	memcpy(header.subchunk2_id, "data", 4);
	header.subchunk2_size = 0;

	fwrite(&header, sizeof(struct dwav_header), 1, file);
#endif	
	
	//# message queue
	isnd_bc->qid = Msg_Init(BCPLAY_MSG_KEY);
	TRACE_INFO("isnd_bc->qid:0x%X\n", isnd_bc->qid);
	
	while (!exit) 
	{
		int bytes=0;
	
		//# wait cmd
		cmd = bcplay_recv_msg();
		if (cmd >= 0)
		{
			switch (cmd)
			{
				case BCPLAY_CMD_READY:
					LOGD("[main] Backchannel Audio Process Ready!\n");
					break;
					
				case BCPLAY_CMD_AUD_DATA:
				{
					int i;
					
					bytes = isnd_bc->len;
					//TRACE_INFO("received audio data, len=%d\n", bytes);
					if (bytes == 0) {
						OSA_waitMsecs(10);
						continue;
					}
					
					/* g.711 -> linear pcm으로 변환.  */
					memset(sampv, 0, BC_SAMPLE_BUFFER_SZ);

					for(i=0;i<bytes;i++){
						sampv[i] = (unsigned short)ulaw2linear(isnd_bc->sbuf[i]);
					}
					
					#ifdef DBG_SND_DUMP
					if (file != NULL) {
						header.subchunk2_size += bytes*FRAME_PER_BYTES;
						fwrite(sampv, bytes*FRAME_PER_BYTES, 1, file);
						dump_cnt++;
					}
					
					if (dump_cnt > SND_DUMP_MAX_FRAMES) {
						if (file != NULL) {
							/* save rest of the data */
							header.chunk_size = ftell(file) - 8;
							fseek(file, 0, SEEK_SET);
							fwrite(&header, sizeof(struct dwav_header), 1, file);
							fclose(file);
							file = NULL;
							sync();
							TRACE_INFO("wave dump done!!\n");
						}
					}
					#endif
					
					/* sound out */
					__snd_out_write(h_pcm, (void *)sampv, bytes*FRAME_PER_BYTES);
				}
				break;
			} /* end of switch (cmd) */
		} else {
			LOGE("[main] Failed to initialize Backchannel audio process!(for ipc)\n");
			OSA_waitMsecs(1000);
			continue;
		}
	} /* while(!exit) */
	
	Msg_Kill(isnd_bc->qid);
	__snd_out_exit(h_pcm);
	
	/* buffer free */
	if (sampv != NULL)
		free(sampv);
	
	tObj->active = 0;
	TRACE_INFO("...exit\n");
	return NULL;
}

/*****************************************************************************
* @brief    sound bchannel play init/exit function
* @section  [desc]
*****************************************************************************/
int app_snd_bcplay_init(void)
{
	memset(isnd_bc, 0, sizeof(app_snd_bcplay_t));

    /* VOIP를 사용할 수 있는 board에서만 동작 */
	if (!app_cfg->voip_set_ON_OFF) 
	{
		/* set playback volume */
		system("/usr/bin/amixer cset numid=1 60% > /dev/null 2>&1");
		
		//#--- create backchannel play thread bkkim
		if (thread_create(&isnd_bc->mObj, THR_snd_bcplay_main, APP_THREAD_PRI, 
						NULL, __FILENAME__) < 0) {
			TRACE_ERR("create bc play thread\n");
			return -1;
		}
	}

	TRACE_INFO("... done!\n");
	return 0;
}

void app_snd_bcplay_exit(void)
{
	app_thr_obj *tObj = NULL;
	
	/* set playback volume */
//	system("/usr/bin/amixer cset numid=1 0% > /dev/null 2>&1");
	
	//#--- stop backchannel audio
	tObj = &isnd_bc->mObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active) {
		app_msleep(20);
	}
	thread_delete(tObj);
}
//############################################# BACK-CHANNEL SOUND PLAY HELPER ########################################################################

//############################################# SOUND INFO PLAY HELPER ################################################################################
static int __wav_decode(FILE *f, wave_info_t *info)
{
	wav_header wh;
	size_t size, type;
	ssize_t next_tag;
	
	/* find wav data tag */
	fread(&wh, sizeof(wav_header), 1, f);
	if ((MKTAG(wh.chunk_id[0], wh.chunk_id[1], wh.chunk_id[2], wh.chunk_id[3]) != TAG_RIFF)
		|| (MKTAG(wh.format[0], wh.format[1], wh.format[2], wh.format[3])!= TAG_WAVE)) {
		TRACE_ERR("Invaild file format!\n");
		return -1;
	}
	
	/* find data tag */
	for (;;) 
	{
		if (feof(f)) {
			TRACE_ERR("Not founded wave data tag!\n");
			return -1;
		}

		/* read next tag and tag size */
		fread(&type, 4, 1, f);
		fread(&size, 4, 1, f);

		if (type == TAG_DATA)
            break;

        next_tag = ftell(f) + size;
		fseek(f, next_tag, SEEK_SET);
	}
	//*ofs = ftell(file);
	TRACE_INFO("size %d, channels %d, sample_rate %d, bits_per_sample %d\n", size, 
				wh.channels, wh.sample_rate, wh.bits_per_sample);

	info->sz = size;
	info->ch = wh.channels;
	info->sr = wh.sample_rate;
	info->bs = wh.bits_per_sample;

	return 0;
}

static void *THR_snd_iplay_main(void *prm)
{
	app_thr_obj *tObj = &isnd_info->iObj;
	snd_pcm_t *h_pcm;
	int cmd, exit=0;
	int repeat;
	FILE *fin;
	
	tObj->active = 1;
	TRACE_INFO("enter...\n");
	
	while (!exit) 
	{
		size_t pframes = 0, size;
		int ch, sr;
		char *sampv=NULL; /* playback sound buffer */
		
		cmd = event_wait(tObj);
		if(cmd == APP_CMD_EXIT) {
			break;
		}
		repeat = tObj->param0;
		
		isnd_info->so_init = 1;
		/* decode wav file */
		fin = fopen(&isnd_info->filename[0], "rb");
		if (fin == NULL) {
			TRACE_ERR("Unable to open %s file \n", &isnd_info->filename[0]);
			continue;
		}
	
		if (__wav_decode(fin, &isnd_info->winfo) < 0) {
			TRACE_ERR("Failed to decode wav file!\n");
			fclose(fin);
			continue;
		}
		
		/* Initialize ALSA Device */
		ch = isnd_info->winfo.ch; 
		sr = isnd_info->winfo.sr; 
		size = isnd_info->winfo.sz;
		
		sampv = (char *)malloc(size);
		if (sampv == NULL) {
			TRACE_ERR("Failed to alloc memory!\n");
			fclose(fin);
			continue;
		}
		
		fread(sampv, size, 1, fin);
		if (fin != NULL)
			fclose(fin);
		
		pframes = sr * APP_SND_BC_PTIME / 1000; /* secound -> ms */
		h_pcm = __snd_out_init(ch, sr, pframes);
		if (h_pcm != NULL) 
		{
			/* repeat play */
			while (repeat > 0)
			{
				cmd = tObj->cmd;
				if (cmd == APP_CMD_EXIT || cmd == APP_CMD_STOP) {
					break;
				}
				
				/* sound out */
				__snd_out_write(h_pcm, (void *)sampv, size);
				repeat--;
			}
		}
		__snd_out_exit(h_pcm);
		if (sampv != NULL)
			free(sampv);
		
		isnd_info->so_init = 0;	
	} /* while(!exit) */
	
	tObj->active = 0;
	TRACE_INFO("...exit\n");
	
	return NULL;
}

/*****************************************************************************
* @brief    sound out start function
* @section  DESC Description
*   - desc
*****************************************************************************/
void app_snd_iplay_start(const char *fname, int count)
{
	if (count <= 0)
		return;
		
	/* copy filename */
	memset(&isnd_info->filename[0], 0, 256);
	strcpy(&isnd_info->filename[0], fname);
	
	event_send(&isnd_info->iObj, APP_CMD_NOTY, count, 0);
//	OSA_waitMsecs(1);
/*
	if (count == 1) {		//# wait
		while (isnd_info->so_init)	//# wait close sound
			OSA_waitMsecs(10);
	}
*/

	return ;
}

/*****************************************************************************
* @brief    sound out stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
void app_snd_iplay_stop(void)
{
	if (isnd_info->so_init) 
	{
		event_send(&isnd_info->iObj, APP_CMD_STOP, 0, 0);
//		OSA_waitMsecs(1);
/*
		//# wait close sound
		while (isnd_info->so_init)
		{
			OSA_waitMsecs(10);
		}
*/
	}

	return ;
}

/*****************************************************************************
* @brief    sound info play init/exit function
* @section  [desc]
*****************************************************************************/
int app_snd_iplay_init(void)
{
	memset(isnd_info, 0, sizeof(app_snd_iplay_t));

	/* set playback volume */
	system("/usr/bin/amixer cset numid=1 60% > /dev/null 2>&1");
		
	//#--- create sound info play thread
	if (thread_create(&isnd_info->iObj, THR_snd_iplay_main, APP_THREAD_PRI, 
					NULL, __FILENAME__) < 0) {
		TRACE_ERR("create sound info play thread\n");
		return -1;
	}

	TRACE_INFO("... done!\n");
	return 0;
}

void app_snd_iplay_exit(void)
{
	app_thr_obj *tObj = NULL;
	
	/* set playback volume */
//	system("/usr/bin/amixer cset numid=1 0% > /dev/null 2>&1");
	
	tObj = &isnd_info->iObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active) {
		app_msleep(20);
	}
	thread_delete(tObj);
}
//############################################# INFO SOUND PLAY HELPER ########################################################################

#endif /* #if SYS_CONFIG_SND_OUT */
