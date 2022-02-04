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
#include "app_snd.h"
#include "app_set.h"
#include "app_rtsptx.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define SND_AIC3X			"plughw:0,0"
#define SND_DUMMY			"plughw:1,0"

#define FRAME_PER_BYTES		(APP_SND_CH * APP_SND_PCM_BITS / 8)   /* sound channel */

typedef struct {
	snd_pcm_t *pcm_t;
	
	int channel;
	int sample_rate;
	int num_frames;
	
} snd_prm_t;

typedef struct {
	app_thr_obj cObj;	 //# sound in thread
	
	snd_pcm_t *aic3x;    //# read sound card.
	snd_pcm_t *dummy;    //# dummy sound card (virtual)
	
	g_mem_info_t *imem;
} app_snd_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_snd_t snd_obj;
static app_snd_t *isnd=&snd_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    ALSA Initialize
*****************************************************************************/
static snd_pcm_t * __snd_init(const char *pcm_name, int ch, int rate, 
						int period_ms, int playback)
{
	snd_pcm_t *handle = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;

	snd_pcm_uframes_t buffer_size, period_size;
	snd_pcm_stream_t pcm_mode;
	
	unsigned int freq, nchannels;
	
	if (playback) 	pcm_mode = SND_PCM_STREAM_PLAYBACK;
	else 			pcm_mode = SND_PCM_STREAM_CAPTURE;
	
	if (snd_pcm_open(&handle, pcm_name, pcm_mode, 0) < 0) {
		dprintf("%s: could not open device!\n", pcm_name);
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
	
	period_size = period_ms;
	buffer_size = (period_size * 4); //# alsa 표준
    if (snd_pcm_hw_params_set_period_size_near(handle, hw_params, &period_size, 0) < 0) {
        dprintf("%s: Failed to set period size to %ld\n", pcm_name, period_size);
        goto out;
    }

    if (snd_pcm_hw_params_set_buffer_size_near(handle, hw_params, &buffer_size) < 0){
        dprintf("%s: Failed to set buffer size to %ld\n", pcm_name, buffer_size);
        goto out;
    }

    /* PCM device and prepare device  */
    if (snd_pcm_hw_params(handle, hw_params) < 0)
		goto out;

#if 1	
	/* returned approximate maximum buffer size in frames  */
	snd_pcm_hw_params_get_period_size(hw_params, &period_size, 0);
	snd_pcm_hw_params_get_buffer_size(hw_params, &buffer_size);
	printf(" ALSA Period Size %ld\n", period_size);
	printf(" ALSA Buffer Size %ld\n", buffer_size);
#endif

	/* prepare for audio device */
	if (snd_pcm_prepare(handle) < 0) {
        dprintf("%s: Could not prepare handle!\n", pcm_name);
        goto out;
    }
	return (snd_pcm_t *)handle;
	
out:
	snd_pcm_hw_params_free(hw_params);
	eprintf("alsa: init failed!!\n");
	return NULL;
}

static void __snd_exit(snd_pcm_t *handle)
{
	if (handle != NULL) {
		snd_pcm_hw_free(handle);
		snd_pcm_close(handle);
		handle = NULL;
	}
}
						
/*****************************************************************************
* @brief    sound capture thread function
* @section  [desc]
*****************************************************************************/
static void *THR_snd_cap(void *prm)
{
	app_thr_obj *tObj = &isnd->cObj;
	stream_info_t *ifr;
	
	int exit=0, idx;
	size_t btime_ms = 0;
	
	char *addr=NULL;
	char *csampv=NULL; /* capture sound buffer */
	char *psampv=NULL; /* playback sound buffer */
	
	tObj->active = 1;
	aprintf("enter...\n");
	
	/* 
	 * set alsa buffer time in ms (프레임 단위로 설정함)
	 * 1s:8000sample(8KHz sampling rate) = PTIME:x
	 * x(ms) = samplerate * ptime / 1000 (ms)
	 */
	btime_ms = APP_SND_SRATE * APP_SND_PTIME / 1000;
	/* Initialize ALSA MIC */
	isnd->aic3x = __snd_init(SND_AIC3X, APP_SND_CH, 
						APP_SND_SRATE, btime_ms, APP_SND_CAPT_DEV);
	if (isnd->aic3x == NULL) {
		eprintf("Failed to init %s device!\n", SND_AIC3X);
		return NULL;
	}
	
	/* 
	 * alloc buffer (byte 단위로 변경해야 한다)
	 * 1 sample = pcm bits / 8 = 16 / 8 = 2
	 * 1 frame = 1 sample * channel = 2 * 1 = 2 (만일 2ch일 경우== 4)
	 */
	csampv = (char *)malloc(btime_ms * FRAME_PER_BYTES);
	if (csampv == NULL) {
		__snd_exit(isnd->aic3x);
		return NULL;
	}
	
	/* mic data를 copy해서 baresip 에 전달하기 위한 dummpy sound 장치 
	 * playback 장치로 설정해야 함. (마지막 인자를 1로 설정)
	 */
	if (app_set->voip.ON_OFF) {
		isnd->dummy = __snd_init(SND_DUMMY, APP_SND_CH, APP_SND_SRATE, 
								btime_ms, APP_SND_PLAY_DEV);
		if (isnd->dummy == NULL) {
			eprintf("Failed to init %s device!\n", SND_DUMMY);
			__snd_exit(isnd->aic3x);
			free(csampv);
			return NULL;
		}
		
		/* 
		 * alloc buffer (byte 단위로 변경해야 한다)
		 * 1 sample = pcm bits / 8 = 16 / 8 = 2
		 * 1 frame = 1 sample * channel = 2 * 1 = 2 (만일 2ch일 경우== 4)
		 */
		psampv = (char *)malloc(btime_ms * FRAME_PER_BYTES);
		if (psampv == NULL) {
			__snd_exit(isnd->aic3x);
			__snd_exit(isnd->dummy);
			free(csampv);
			return NULL;
		}
	}

    if (app_set->voip.ON_OFF) {
		snd_pcm_sw_params_t *sw_params = NULL;
		snd_pcm_uframes_t buffer_size=0;
		
		/* set software params */
		snd_pcm_sw_params_alloca(&sw_params);
		snd_pcm_sw_params_current(isnd->dummy, sw_params);
		snd_pcm_sw_params_set_avail_min(isnd->dummy, sw_params, btime_ms);
		/* ALSA 표준 버퍼 --> x4 */
		buffer_size = btime_ms*4;
		snd_pcm_sw_params_set_start_threshold(isnd->dummy, sw_params, buffer_size);
	    snd_pcm_sw_params_set_stop_threshold(isnd->dummy, sw_params, buffer_size);
		snd_pcm_sw_params(isnd->dummy, sw_params);
	}
	
	/* recording device start */
	snd_pcm_start(isnd->aic3x);
	while (!exit)
	{
		snd_pcm_sframes_t r, w;
		size_t period=0;
		
		if (tObj->cmd == APP_CMD_EXIT)
			break;
		
		period = btime_ms;
		r = snd_pcm_readi(isnd->aic3x, csampv, period);
		if (r > 0) {
			ssize_t bytes=0;
			/* success. conversion to byte unit */
			bytes = (r * FRAME_PER_BYTES);
			if (app_set->voip.ON_OFF) {
				//# copy to dup device
				app_memcpy(psampv, csampv, bytes);
				/* mic sound copy */
				/* 묵음 처리 */
				if (r < period) {
					snd_pcm_format_set_silence(SND_PCM_FORMAT_S16_LE, (char *)psampv + bytes,
							(period - r) * APP_SND_CH);
					r = period;
				}
				
				w = snd_pcm_writei(isnd->dummy, psampv, r);
				if (w < 0) {
					if (w == -EAGAIN) {
						//dprintf("pcm write wait(required 100ms)!!\n");
						snd_pcm_wait(isnd->dummy, 100);
					} else if (w == -EPIPE) {
						//dprintf("pcm write underrun!!\n");
						snd_pcm_prepare(isnd->dummy);
					} else if (w == -ESTRPIPE) {
						snd_pcm_resume(isnd->dummy);
					} else {
						/* debugging : 이 경우가 발생하면 안됨 */
						eprintf("sound device write error(%d)!!\n", (int)r);
					}
				} else if ((w >= 0) && ((size_t)w < r)) {
					/* blocking 장치라 이 경우가 발생하는 지 잘 모름? */
					//snd_pcm_wait(isnd->dummy, 100);
				}
			} /* if (app_set->voip.ON_OFF) */	
		
			//# audio codec : g.711
			addr = g_mem_get_addr(bytes, &idx);
			if(addr == NULL) {
				eprintf("audio gmem is null!!\n");
				continue;
			}
			
			ifr = &isnd->imem->ifr[idx];
			ifr->d_type = CAP_TYPE_AUDIO;
			ifr->ch = 0;
			ifr->addr = addr;
			ifr->offset = (int)addr - g_mem_get_virtaddr();
			ifr->b_size = bytes;
			app_memcpy(addr, csampv, bytes);
			
			if(!app_set->voip.ON_OFF) { // BACKCHANNEL AUDIO
				struct timeval tv;
				gettimeofday(&tv, NULL) ;
				long timestamp = tv.tv_sec + tv.tv_usec*1000 ;
				app_rtsptx_write((void *)ifr->addr, ifr->offset, ifr->b_size, 0,  2, timestamp);
			}
		} else {
			/* error returned */
			if (r == 0) {
				//dprintf("Failed to read sound data!!\n");
			} else if (r == -EAGAIN) {
				//dprintf("requied pcm wait!\n", count);
				snd_pcm_wait(isnd->aic3x, 100);
			} else if (r == -EPIPE) {
				//dprintf("pcm overrun!\n");
				snd_pcm_prepare(isnd->aic3x);
			} else {
				/* debugging : 이 경우가 발생하면 안됨 */
				eprintf("sound device read error(%d)!!\n", (int)r);
			}
			continue;
		} /* end of if (r > 0) */
	} /* while (!exit) */

	if (app_set->voip.ON_OFF) {
		snd_pcm_nonblock(isnd->dummy, 0);
		/* Stop PCM device after pending frames have been played */
		snd_pcm_drain(isnd->dummy);
		//snd_pcm_nonblock(isnd->dummy, 1);
		__snd_exit(isnd->dummy);
	}
	/* stop mic pcm */
	__snd_exit(isnd->aic3x);
	if (csampv != NULL)
		free(csampv);
	if (app_set->voip.ON_OFF) {
		if (psampv != NULL)
			free(psampv);
	}
	
	tObj->active = 0;
	aprintf("...exit\n");
	return NULL;
}

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
#define BCPLAY_MSG_KEY					0x185EA
#define BCPLAY_MSG_TYPE_TO_MAIN         1
#define BCPLAY_CMD_READY				(0x624)
#define BCPLAY_CMD_CONFIG				(0x625)
#define BCPLAY_CMD_REQ_DATA			    (0x626) /* request audio data */
#define BCPLAY_CMD_AUD_DATA			    (0x627) /* delivery audio data */
#define BCPLAY_RCV_SIZE 				2048
typedef struct {
	long mtext;
	int cmd;
	int len;
	unsigned char sbuf[BCPLAY_RCV_SIZE];
} bcplay_to_main_msg_t;
typedef struct {
	app_thr_obj rObj;		//# message receive thread
	
	int qid;
	int shmid;
	
	int len;
	unsigned char sbuf[BCPLAY_RCV_SIZE];
	
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
	snd_pcm_sw_params_t *sw_params = NULL;
	
	int cmd, exit=0;
	int bytes = 0;
	short lbuf[2048];
	size_t btime_ms = 0;
	
	char *sampv=NULL; /* playback sound buffer */
	/* 
	 * set alsa buffer time in ms (프레임 단위로 설정함)
	 * 1s:8000sample(8KHz sampling rate) = PTIME:x
	 * x(ms) = samplerate * ptime / 1000 (ms)
	 */
	btime_ms = APP_SND_BC_SRATE * APP_SND_BC_PTIME / 1000;
	/* Initialize ALSA Speaker */
	isnd->aic3x = __snd_init(SND_AIC3X, APP_SND_BC_CH, APP_SND_BC_SRATE, 
						btime_ms, APP_SND_PLAY_DEV);
	if (isnd->aic3x == NULL) {
		eprintf("Failed to init %s device!\n", SND_AIC3X);
		return NULL;
	}
	
	/* 
	 * alloc buffer (byte 단위로 변경해야 한다)
	 * 1 sample = pcm bits / 8 = 16 / 8 = 2
	 * 1 frame = 1 sample * channel = 2 * 1 = 2 (만일 2ch일 경우== 4)
	 */
	sampv = (char *)malloc(btime_ms * FRAME_PER_BYTES);
	if (sampv == NULL) {
		__snd_exit(isnd->aic3x);
		return NULL;
	}
	
	/* set ALSA software params */
	snd_pcm_sw_params_alloca(&sw_params);
	snd_pcm_sw_params_current(isnd->aic3x, sw_params);
	snd_pcm_sw_params_set_avail_min(isnd->aic3x, sw_params, btime_ms);
	/* ALSA 표준 버퍼 --> x4 */
	snd_pcm_sw_params_set_start_threshold(isnd->aic3x, sw_params, btime_ms*4);
	snd_pcm_sw_params_set_stop_threshold(isnd->aic3x, sw_params, btime_ms*4);
	snd_pcm_sw_params(isnd->aic3x, sw_params);
	
	//# message queue
	ibcplay->qid = Msg_Init(BCPLAY_MSG_KEY);
	aprintf("ibcplay->qid:0x%X\n", ibcplay->qid);
	
	while(!exit){
		//# wait cmd
		cmd = bcplay_recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive gps process msg!\n");
			sleep(1);
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
				snd_pcm_sframes_t w;
				int i;
				
				bytes = ibcplay->len;
				//dprintf("received audio data, len=%d\n", bytes);

				if (bytes == 0)
				{
					eprintf("bc sound size error!!\n");
					continue;
				}
				memset(lbuf, 0, sizeof lbuf);
				for(i=0;i<bytes;i++){
					lbuf[i] = ulaw2linear(ibcplay->sbuf[i]);
				}

				/*
				 * copy to alsa buffer
				 * g.711 -> linear pcm으로 변환. 
				 * 8bit에서 16bit로 변환되므로 size가 2배가 됨.
				 */
				app_memcpy(sampv, lbuf, bytes*2);
				
				/* 묵음 처리 */
				//if (bytes < btime_ms) {
				//	snd_pcm_format_set_silence(SND_PCM_FORMAT_S16_LE, (char *)sampv + bytes,
				//			(btime_ms - r) * APP_SND_CH);
				//	bytes = btime_ms;
				//}
				w = snd_pcm_writei(isnd->aic3x, sampv, bytes);
				if (w < 0) {
					if (w == -EAGAIN) {
						//dprintf("pcm write wait(required 100ms)!!\n");
						snd_pcm_wait(isnd->aic3x, 100);
					} else if (w == -EPIPE) {
						//dprintf("pcm write underrun!!\n");
						snd_pcm_prepare(isnd->aic3x);
					} else if (w == -ESTRPIPE) {
						snd_pcm_resume(isnd->aic3x);
					} else if (w < 0) {
						eprintf("backchannel sound device write error!!\n");
					}
				} else if ((w >= 0) && ((size_t)w < bytes)) {
					/* blocking 장치라 이 경우가 발생하는 지 잘 모름? */
					//snd_pcm_wait(isnd->aic3x, 100);
				}
			}
			break;
		}
	} /* while(!exit) */
	
	/* Stop PCM device after pending frames have been played */
	snd_pcm_nonblock(isnd->aic3x, 0);
	snd_pcm_drain(isnd->aic3x);
	//snd_pcm_nonblock(isnd->aic3x, 1);
	__snd_exit(isnd->aic3x);
	/* buffer free */
	if (sampv != NULL)
		free(sampv);
		
	aprintf("...exit\n");
	return NULL;
}

/*****************************************************************************
* @brief    sound init/exit function
* @section  [desc]
*****************************************************************************/
int app_snd_start(void)
{
	g_mem_info_t *imem;
	FILE *f;
	
	memset(isnd, 0, sizeof(app_snd_t));
	/* set capture volume */
	//# "/usr/bin/amixer cset numid=31 60% > /dev/null"
	system("/usr/bin/amixer cset numid=31 60% > /dev/null 2>&1");
	/* GMEM Address 가져온다 */
	imem = (g_mem_info_t *)app_cap_get_gmem();
	isnd->imem = imem;

	//#--- create thread
	if (thread_create(&isnd->cObj, THR_snd_cap, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
		eprintf("create thread\n");
		return -1;
	}

    if(!app_set->voip.ON_OFF) {
		//#--- create backchannel play thread bkkim
		if (thread_create(&isnd->cObj, THR_bc_play, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
			eprintf("create bc play thread\n");
			return -1;
		}
	}

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
