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
#include <alsa/asoundlib.h>

#include "dev_snd.h"
#include "app_comm.h"
#include "app_main.h"
#include "app_gmem.h"
#include "app_cap.h"
#include "app_snd.h"
#include "app_set.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define __WAVE_DUMP__		0

#if __WAVE_DUMP__
#define WAV_FILE_NAME		"/mmc/dump.wav"
#define DUMP_FRAME_CNT		100	
#define MKTAG(a,b,c,d)		((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
#define TAG_RIFF			MKTAG('R','I','F','F')
#define TAG_WAVE			MKTAG('W','A','V','E')
#define TAG_DATA			MKTAG('d','a','t','a')

typedef struct {
	unsigned int magic;	 /* 'RIFF' */
	unsigned int length; /* file_len */
	unsigned int type;	 /* 'WAVE' */
} wave_header;

struct wav_header {
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

typedef struct {
	FILE *f;
	struct wav_header header;   
	
	int wave_frame_cnt;
	int dump_done;
} app_wave_t;

#endif /* #if __WAVE_DUMP__ */

#define SND_IN_DEV				"plughw:0,0"
#define SND_DUP_DEV				"plughw:1,0"  //# --> plughw:1,1

typedef struct {
	app_thr_obj cObj;	 //# sound in thread
	
	snd_prm_t snd_in;    //# real alsa sound.
	snd_prm_t snd_dup;   //# duplicate real sound
	
	g_mem_info_t *imem;
	int snd_rec_enable;
	
} app_snd_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_snd_t snd_obj;
static app_snd_t *isnd=&snd_obj;

#if __WAVE_DUMP__
static app_wave_t wave_obj;
static app_wave_t *iwave=&wave_obj;
#endif

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

//################## For wave file debugging #####################################
#if __WAVE_DUMP__
static int init_wav_file(const char *file_name, int ch, int rate)
{
	struct wav_header *pHead = &iwave->header;
	
	iwave->f = fopen(file_name, "w+");
	if (iwave->f == NULL) {
		printf("Failed to open file!\n");
		return -1;
	}
	
	/* init wave header */
	memcpy(pHead->chunk_id,"RIFF",4);
	pHead->chunk_size = 0;
	memcpy(pHead->format, "WAVE",4);
	memcpy(pHead->subchunk1_id, "fmt ", 4);

	pHead->subchunk1_size = 16;
	pHead->audio_format = 1;
	pHead->channels = ch;
	pHead->sample_rate = rate;
	pHead->bits_per_sample = 16;
	
	pHead->block_align = ch * pHead->bits_per_sample / 8;
	pHead->byte_range = pHead->sample_rate * pHead->block_align;
	memcpy(pHead->subchunk2_id, "data", 4);
	pHead->subchunk2_size = 0;

	fwrite(pHead, sizeof(struct wav_header), 1, iwave->f);
	
	return 0;
}

static int write_wav_file(const char *buf, size_t sz)
{
	struct wav_header *pHead = &iwave->header;
	int res;
	
	pHead->subchunk2_size += sz;
	res = fwrite(buf, sz, 1, iwave->f);
	
	return res;
}

static void close_wav_file(void)
{
	struct wav_header *pHead = &iwave->header;
	
	/* save rest of the data */
	pHead->chunk_size = ftell(iwave->f) - 8;
	fseek(iwave->f, 0, SEEK_SET);
	fwrite(pHead, sizeof(struct wav_header), 1, iwave->f);
	fclose(iwave->f);
	fprintf(stderr, "wave dump done!!\n");
}
#endif /* #if __WAVE_DUMP__ */ 

/*----------------------------------------------------------------------------
 audio codec function
-----------------------------------------------------------------------------*/
static int alg_ulaw_encode(unsigned short *dst, unsigned short *src, Int32 bufsize)
{
    int i, isNegative;
    short data;
    short nOut;
    short lowByte = 1;
    int outputSize = bufsize / 2;

    for (i=0; i<outputSize; i++)
    {
        data = *(src + i);
        data >>= 2;
        isNegative = (data < 0 ? 1 : 0);

        if (isNegative)
            data = -data;

        if (data <= 1) 			nOut = (char) data;
        else if (data <= 31) 	nOut = ((data - 1) >> 1) + 1;
        else if (data <= 95)	nOut = ((data - 31) >> 2) + 16;
        else if (data <= 223)	nOut = ((data - 95) >> 3) + 32;
        else if (data <= 479)	nOut = ((data - 223) >> 4) + 48;
        else if (data <= 991)	nOut = ((data - 479) >> 5) + 64;
        else if (data <= 2015)	nOut = ((data - 991) >> 6) + 80;
        else if (data <= 4063)	nOut = ((data - 2015) >> 7) + 96;
        else if (data <= 7903)	nOut = ((data - 4063) >> 8) + 112;
        else 					nOut = 127;

        if (isNegative) {
            nOut = 127 - nOut;
        } else {
            nOut = 255 - nOut;
        }

        // Pack the bytes in a word
        if (lowByte)
            *(dst + (i >> 1)) = (nOut & 0x00FF);
        else
            *(dst + (i >> 1)) |= ((nOut << 8) & 0xFF00);

        lowByte ^= 0x1;
    }

	return (outputSize);
}

/*****************************************************************************
* @brief    sound capture thread function
* @section  [desc]
*****************************************************************************/
static void *THR_snd_cap(void *prm)
{
	app_thr_obj *tObj = &isnd->cObj;
	stream_info_t *ifr;
	
	int exit=0;
	int ret, idx;
	int read_sz = 0;
	int si_size, enc_size;
	
	char *addr = NULL;

	struct timeval tv ;
    unsigned int timestamp ;
	aprintf("enter...\n");
	tObj->active = 1;

#if __WAVE_DUMP__	
	init_wav_file(WAV_FILE_NAME, 1, APP_SND_SRATE);
#endif	
	/* get alsa period size (in sec) */
	read_sz = APP_SND_SRATE * APP_SND_PTIME / 1000; //# 
	ret = dev_snd_set_param("aic3x", &isnd->snd_in, SND_PCM_CAP, 
				APP_SND_CH, APP_SND_SRATE, read_sz);
#if SYS_CONFIG_VOIP	
	ret |= dev_snd_set_param("dummy", &isnd->snd_dup, SND_PCM_PLAY, 
				APP_SND_CH, APP_SND_SRATE, read_sz);
#endif
	ret |= dev_snd_open(SND_IN_DEV, &isnd->snd_in);

#if SYS_CONFIG_VOIP
	ret |= dev_snd_open(SND_DUP_DEV, &isnd->snd_dup);
#endif
	if (ret) {
		eprintf("Failed to init sound device!\n");
		return NULL;
	}
	
	si_size  = (read_sz * APP_SND_CH * (SND_PCM_BITS / 8));
	dev_snd_start(&isnd->snd_in);
#if SYS_CONFIG_VOIP	
	dev_snd_set_swparam(&isnd->snd_dup, SND_PCM_PLAY);
#endif
	while (!exit)
	{
		int bytes = 0;
		
		if (tObj->cmd == APP_CMD_EXIT) {
			break;
		}

		bytes = dev_snd_read(&isnd->snd_in);
		if(bytes < 0) {
			eprintf("sound device error!!\n");
			continue;
		}

#if SYS_CONFIG_VOIP		
		//# copy to dup device
		app_memcpy(isnd->snd_dup.sampv, isnd->snd_in.sampv, bytes);
		/* VOIP를 사용할 경우에만 copy ?? */
		dev_snd_write(&isnd->snd_dup, bytes/2); 
#endif	

#if defined(NEXXONE)	
		if (isnd->snd_rec_enable)
		{
			//# audio codec : g.711
			addr = g_mem_get_addr(bytes, &idx);
			if(addr == NULL) {
				eprintf("audio gmem is null\n");
				continue;
			}

			ifr = &isnd->imem->ifr[idx];
			ifr->d_type = CAP_TYPE_AUDIO;
			ifr->ch = 0;
			ifr->addr = addr;
			ifr->offset = (int)addr - g_mem_get_virtaddr();
			ifr->b_size = bytes;
			//ifr->t_sec = (Uint32)(captime/1000);
			//ifr->t_msec = (Uint32)(captime%1000);
			app_memcpy(addr, isnd->snd_in.sampv, bytes);
			
			#if __WAVE_DUMP__
			if (iwave->wave_frame_cnt >= DUMP_FRAME_CNT) 
			{
				if (iwave->dump_done == 0) {
					iwave->dump_done = 1;
					close_wav_file();
				}
			} else {
				write_wav_file(addr, bytes);
				iwave->wave_frame_cnt++;
			}
			#endif
		}
#else		
			//# audio codec : g.711
		addr = g_mem_get_addr(bytes, &idx);
		if(addr == NULL) {
			eprintf("audio gmem is null\n");
			continue;
		}

		ifr = &isnd->imem->ifr[idx];
		ifr->d_type = CAP_TYPE_AUDIO;
		ifr->ch = 0;
		ifr->addr = addr;
		ifr->offset = (int)addr - g_mem_get_virtaddr();
		ifr->b_size = bytes;
		//ifr->t_sec = (Uint32)(captime/1000);
		//ifr->t_msec = (Uint32)(captime%1000);
		app_memcpy(addr, isnd->snd_in.sampv, bytes);

#if 0 // Enable Audio Streaming for NEXX360
		gettimeofday(&tv, NULL) ;
		timestamp = tv.tv_sec + tv.tv_usec*1000 ;
		app_rtsptx_write((void *)ifr->addr, ifr->offset, ifr->b_size, 0,  2, timestamp);
#endif// Enable Audio Streaming for NEXX360

#endif

	}

#if SYS_CONFIG_VOIP
	dev_snd_stop(&isnd->snd_dup, SND_PCM_PLAY);
	dev_snd_stop(&isnd->snd_in, SND_PCM_CAP);
#endif	
	dev_snd_param_free(&isnd->snd_in);
#if SYS_CONFIG_VOIP	
	dev_snd_param_free(&isnd->snd_dup);
#endif	
	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

#if SYS_CONFIG_BACKCHANNEL
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
#define BCPLAY_MSG_KEY					0x185EA
#define BCPLAY_MSG_TYPE_TO_MAIN         1
#define BCPLAY_CMD_READY				(0x624)
#define BCPLAY_CMD_CONFIG				(0x625)
#define BCPLAY_CMD_REQ_DATA			    (0x626) /* request audio data */
#define BCPLAY_CMD_AUD_DATA			    (0x627) /* delivery audio data */
typedef struct {
	long mtext;
	int cmd;
	int len;
	unsigned char sbuf[1044];
} bcplay_to_main_msg_t;
typedef struct {
	app_thr_obj rObj;		//# message receive thread
	
	int qid;
	int shmid;
	
	int len;
	unsigned char sbuf[1044];
	
	OSA_MutexHndl mutex_bcplay;
	
} app_bcplay_obj_t;
static app_bcplay_obj_t t_bcplay_obj;
static app_bcplay_obj_t *ibcplay=&t_bcplay_obj;

static int bcplay_recv_msg(void)
{
	bcplay_to_main_msg_t msg;
	
	if ( msgrcv( ibcplay->qid, &msg, sizeof(bcplay_to_main_msg_t)-sizeof(long), 0, 0) == -1){
		perror("msgrcv:");
		return -1;
	}
	ibcplay->len = msg.len;
	memcpy(ibcplay->sbuf, msg.sbuf, msg.len);

	return msg.cmd;
}


#define	SIGN_BIT	(0x80)		/* Sign bit for a u-law byte. */
#define	QUANT_MASK	(0xf)		/* Quantization field mask. */
#define	NSEGS		(8)		/* Number of u-law segments. */
#define	SEG_SHIFT	(4)		/* Left shift for segment number. */
#define	SEG_MASK	(0x70)		/* Segment field mask. */
#define	BIAS		(0x84)		/* Bias for linear code. */
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

static void *THR_bc_play(void *prm)
{
	int ret, cmd, exit=0;
	int bytes = 0;
	short lbuf[1040];
	
	/* get alsa period size (in sec) */
	int read_sz = APP_SND_SRATE * APP_SND_PTIME / 1000; //# 
	ret = dev_snd_set_param("aic3x", &isnd->snd_dup, SND_PCM_PLAY, 
				APP_SND_CH, APP_SND_SRATE, read_sz);
	aprintf("ret:%d\n", ret);
	
	ret = dev_snd_open(SND_IN_DEV, &isnd->snd_dup);
	aprintf("ret:%d\n", ret);

	if (ret) {
		eprintf("Failed to init sound device for bc play!\n");
		return NULL;
	}

	dev_snd_set_swparam(&isnd->snd_dup, SND_PCM_PLAY);

	//# message queue
	ibcplay->qid = Msg_Init(BCPLAY_MSG_KEY);

	aprintf("ibcplay->qid:0x%X\n", ibcplay->qid);
	

	while(!exit){
		//# wait cmd
		cmd = bcplay_recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive gps process msg!\n");
			continue;
		}
		bytes = 0;

		switch (cmd)
		{
		case BCPLAY_CMD_READY:
			dprintf("received ready!\n");
			break;
		case BCPLAY_CMD_AUD_DATA:
			{
				bytes = ibcplay->len;
				//dprintf("received audio data, len=%d\n", bytes);

				if (bytes == 0)
				{
					eprintf("bc sound size error!!\n");
					continue;
				}
				memset(lbuf, 0, sizeof lbuf);

				int i;
				for(i=0;i<bytes;i++){

					lbuf[i] = ulaw2linear(ibcplay->sbuf[i]);

				}

				//# copy to dup device
				app_memcpy(isnd->snd_dup.sampv, lbuf, bytes*2);
				/* VOIP를 사용할 경우에만 copy ?? */
				ret = dev_snd_write(&isnd->snd_dup, bytes);
				//aprintf("dev_snd_write ret:%d\n", ret);
			}
			break;
		}
	}

	dev_snd_stop(&isnd->snd_dup, SND_PCM_PLAY);
	dev_snd_param_free(&isnd->snd_dup);

	aprintf("...exit\n");

	return NULL;
}
#endif // SYS_CONFIG_BACKCHANNEL

/*****************************************************************************
* @brief    sound init/exit function
* @section  [desc]
*****************************************************************************/
int app_snd_start(void)
{
	g_mem_info_t *imem;
	
	memset(isnd, 0, sizeof(app_snd_t));
	
	/* set audio record enable */
	isnd->snd_rec_enable = app_set->rec_info.audio_rec;
	/* set capture volume */
	//# "/usr/bin/amixer cset numid=31 60% > /dev/null"
	dev_snd_set_volume(SND_VOLUME_C, 60);
	
	/* GMEM Address 가져온다 */
	imem = (g_mem_info_t *)app_cap_get_gmem();
	isnd->imem = imem;

	//#--- create thread
	if (thread_create(&isnd->cObj, THR_snd_cap, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
		eprintf("create thread\n");
		return -1;
	}

#if SYS_CONFIG_BACKCHANNEL
	//#--- create backchannel play thread bkkim
	if (thread_create(&isnd->cObj, THR_bc_play, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
		eprintf("create bc play thread\n");
		return -1;
	}
#endif//SYS_CONFIG_BACKCHANNEL

	aprintf("... done!\n");

	return 0;
}

void app_snd_stop(void)
{
	app_thr_obj *tObj;

	//#--- stop thread
	tObj = &isnd->cObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		app_msleep(20);
	}
	thread_delete(tObj);

	dprintf("... done!\n");
}
