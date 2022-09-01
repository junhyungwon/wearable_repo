/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_snd_capt.c
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
#include "app_snd_capt.h"
#include "app_set.h"
#include "app_rtsptx.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/* CAPTURE (MIC) SOUND PARAM */
#define APP_SND_PCM_BITS		16

#define FRAME_PER_BYTES			(APP_SND_CAPT_CH * APP_SND_PCM_BITS / 8)   /* sound channel */


typedef struct {
	app_thr_obj cObj;	 	  //# sound in thread
		
	g_mem_info_t *imem;
} app_snd_capt_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_snd_capt_t snd_capt_obj;
static app_snd_capt_t *isnd_capt=&snd_capt_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    ALSA Initialize
*****************************************************************************/
static snd_pcm_t * __snd_capt_dev_init(int ch, int rate, int period_frames)
{
	snd_pcm_t *handle = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;
	snd_pcm_uframes_t buffer_size, period_size;
	
	unsigned int freq, nchannels;
	
	if (snd_pcm_open(&handle, "plughw:0,0", SND_PCM_STREAM_CAPTURE, 0) < 0) {
		TRACE_INFO("could not open device!\n");
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

#if 1	
	/* returned approximate maximum buffer size in frames  */
	snd_pcm_hw_params_get_period_size(hw_params, &period_size, 0);
	snd_pcm_hw_params_get_buffer_size(hw_params, &buffer_size);
	printf(" ALSA Period Size %ld\n", period_size);
	printf(" ALSA Buffer Size %ld\n", buffer_size);
#endif

	/* prepare for audio device */
	if (snd_pcm_prepare(handle) < 0) {
        TRACE_INFO("Could not prepare handle!\n");
        goto out;
    }
	return (snd_pcm_t *)handle;
	
out:
	snd_pcm_hw_params_free(hw_params);
	TRACE_INFO("alsa: init failed!!\n");
	return NULL;
}

static void __snd_capt_dev_exit(snd_pcm_t *handle)
{
	if (handle != NULL) {
		snd_pcm_hw_free(handle);
		snd_pcm_close(handle);
		handle = NULL;
	}
}

static snd_pcm_t * __snd_dummy_dev_init(int ch, int rate, int period_frames)
{
	snd_pcm_t *handle = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;
	snd_pcm_uframes_t buffer_size, period_size;
	
	unsigned int freq, nchannels;
	
	if (snd_pcm_open(&handle, "plughw:1,0", SND_PCM_STREAM_PLAYBACK, 0) < 0) {
		TRACE_INFO("could not open device!\n");
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

#if 1	
	/* returned approximate maximum buffer size in frames  */
	snd_pcm_hw_params_get_period_size(hw_params, &period_size, 0);
	snd_pcm_hw_params_get_buffer_size(hw_params, &buffer_size);
	printf(" ALSA Period Size %ld\n", period_size);
	printf(" ALSA Buffer Size %ld\n", buffer_size);
#endif

	/* prepare for audio device */
	if (snd_pcm_prepare(handle) < 0) {
        TRACE_INFO("Could not prepare handle!\n");
        goto out;
    }
	return (snd_pcm_t *)handle;
	
out:
	snd_pcm_hw_params_free(hw_params);
	TRACE_INFO("alsa: init failed!!\n");
	return NULL;
}

static void __snd_dummy_dev_exit(snd_pcm_t *handle)
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
						
/*****************************************************************************
* @brief    sound capture thread function
* @section  [desc]
*****************************************************************************/
static void *THR_snd_capt_main(void *prm)
{
	app_thr_obj *tObj = &isnd_capt->cObj;
	stream_info_t *ifr;
	snd_pcm_t *h_pcm_capt=NULL;
	snd_pcm_t *h_pcm_dummy=NULL;
	
	int exit=0, idx;
	size_t pframes = 0; /* alsa read size(frames) */
	
	char *addr=NULL;
	char *csampv=NULL; /* capture sound buffer */
	char *psampv=NULL; /* playback sound buffer */
	
	tObj->active = 1;
	TRACE_INFO("enter...\n");
	
	/* 
	 * set alsa period size (프레임 단위로 설정함)
	 * 1초:sample rate = PTIME(ms):x
	 * 1000(ms):8000(8KHz sampling rate) = 80(ms):x
	 * x(frame) = samplerate * 80 / 1000
	 */
	pframes = APP_SND_CAPT_SRATE * APP_SND_CAPT_PTIME / 1000;
	/* Initialize ALSA MIC */
	h_pcm_capt = __snd_capt_dev_init(APP_SND_CAPT_CH, APP_SND_CAPT_SRATE, pframes);
	if (h_pcm_capt == NULL) {
		TRACE_ERR("Failed to init sound record device!\n");
		return NULL;
	}
	LOGD("Initializing audio mic device success!\n");
	
	/* 
	 * alloc buffer (byte 단위로 변경해야 한다)
	 * 1 frame = 1 sample(16bit/8) * channel = 2 * 1 = 2 (만일 2ch일 경우== 4)
	 */
	csampv = (char *)malloc(pframes * FRAME_PER_BYTES);
	if (csampv == NULL) {
		__snd_capt_dev_exit(h_pcm_capt);
		return NULL;
	}
	
	/* mic data를 copy해서 baresip 에 전달하기 위한 dummpy sound 장치 
	 * playback 장치로 설정해야 함. (마지막 인자를 1로 설정)
	 */

	if (app_cfg->voip_set_ON_OFF) {
		h_pcm_dummy = __snd_dummy_dev_init(APP_SND_CAPT_CH, APP_SND_CAPT_SRATE, pframes);
		if (h_pcm_dummy == NULL) {
			TRACE_ERR("Failed to init sound dummy device!\n");
			__snd_capt_dev_exit(h_pcm_capt);
			free(csampv);
			return NULL;
		}
		
		psampv = (char *)malloc(pframes * FRAME_PER_BYTES);
		if (psampv == NULL) {
			__snd_capt_dev_exit(h_pcm_capt);
			__snd_dummy_dev_exit(h_pcm_dummy);
			free(csampv);
			return NULL;
		}
	}

    if (app_cfg->voip_set_ON_OFF) {
		snd_pcm_sw_params_t *sw_params = NULL;
		
		snd_pcm_sw_params_alloca(&sw_params);
		snd_pcm_sw_params_current(h_pcm_dummy, sw_params);
		snd_pcm_sw_params_set_avail_min(h_pcm_dummy, sw_params, pframes);
		snd_pcm_sw_params_set_start_threshold(h_pcm_dummy, sw_params, pframes*4);
	    snd_pcm_sw_params_set_stop_threshold(h_pcm_dummy, sw_params, pframes*4);
		snd_pcm_sw_params(h_pcm_dummy, sw_params);
	}
	
	/* recording device start */
	snd_pcm_start(h_pcm_capt);
	while (!exit)
	{
		snd_pcm_sframes_t r, w;
		size_t period=0;
		
		if (tObj->cmd == APP_CMD_EXIT)
			break;
		
		period = pframes;
		r = snd_pcm_readi(h_pcm_capt, csampv, period);
		if (r > 0) {
			ssize_t bytes=0;
			/* success. conversion to byte unit */
			bytes = (r * FRAME_PER_BYTES);
			if (app_cfg->voip_set_ON_OFF) {
				//# copy to dup device
				app_memcpy(psampv, csampv, bytes);
				/* mic sound copy */
				/* 묵음 처리 */
				if (r < period) {
					snd_pcm_format_set_silence(SND_PCM_FORMAT_S16_LE, (char *)psampv + bytes,
							(period - r) * APP_SND_CAPT_CH);
					r = period;
				}
				
				w = snd_pcm_writei(h_pcm_dummy, psampv, r);
				if (w < 0) {
					if (w == -EAGAIN) {
						//TRACE_ERR("dummy pcm write wait(required 100ms)!!\n");
						snd_pcm_wait(h_pcm_dummy, 100);
					} else if (w == -EPIPE) {
						//TRACE_ERR("dummy pcm write underrun!!\n");
						snd_pcm_prepare(h_pcm_dummy);
					} else if (w == -ESTRPIPE) {
						//TRACE_ERR("dummy pcm write resume!!\n");
						snd_pcm_resume(h_pcm_dummy);
					} else {
						/* debugging : 이 경우가 발생하면 안됨 */
						TRACE_ERR("sound device write error(%d)!!\n", (int)r);
					}
				} else if ((w >= 0) && ((size_t)w < r)) {
					/* blocking 장치라 이 경우가 발생하는 지 잘 모름? */
					//TRACE_ERR("dummy pcm not enough sound data!!\n");
					snd_pcm_wait(h_pcm_dummy, 100);
				}
			} /* if (app_cfg->voip_set_ON_OFF) */	
		
			//# audio codec : g.711
			addr = g_mem_get_addr(bytes, &idx);
			if(addr == NULL) {
				TRACE_INFO("audio gmem is null!!\n");
				continue;
			}
			
			ifr = &isnd_capt->imem->ifr[idx];
			ifr->d_type = CAP_TYPE_AUDIO;
			ifr->ch = 0;
			ifr->addr = addr;
			ifr->offset = (int)addr - g_mem_get_virtaddr();
			ifr->b_size = bytes;
			app_memcpy(addr, csampv, bytes);
			
			if(!app_cfg->voip_set_ON_OFF) { // BACKCHANNEL AUDIO
				struct timeval tv;
				gettimeofday(&tv, NULL) ;
				long timestamp = tv.tv_sec + tv.tv_usec*1000 ;
//				printf("app_cfg->ste.b.rtsptx = %d app_cfg->stream_enable_audio = %d\n",app_cfg->ste.b.rtsptx, app_cfg->stream_enable_audio) ;
				if(app_cfg->ste.b.rtsptx && app_cfg->stream_enable_audio)
					app_rtsptx_write((void *)ifr->addr, ifr->offset, ifr->b_size, 0,  2, timestamp);
			}
		} else {
			/* error returned */
			if (r == 0) {
				//TRACE_ERR("Failed to read sound data!!\n");
			} else if (r == -EAGAIN) {
				//TRACE_ERR("requied pcm wait!\n", count);
				snd_pcm_wait(h_pcm_capt, 100);
			} else if (r == -EPIPE) {
				//TRACE_ERR("pcm overrun!\n");
				snd_pcm_prepare(h_pcm_capt);
			} else {
				/* debugging : 이 경우가 발생하면 안됨 */
				TRACE_ERR("sound device read error(%d)!!\n", (int)r);
			}
			continue;
		} /* end of if (r > 0) */
	} /* while (!exit) */

	if (app_cfg->voip_set_ON_OFF) {
		__snd_dummy_dev_exit(h_pcm_dummy);
	}
	/* stop mic pcm */
	__snd_capt_dev_exit(h_pcm_capt);
	if (csampv != NULL){
		free(csampv);
	}
	if (app_set->voip.ON_OFF) {
		if (psampv != NULL)
			free(psampv);
	}
	
	tObj->active = 0;
	TRACE_INFO("...exit\n");
	return NULL;
}

/*****************************************************************************
* @brief    sound init/exit function
* @section  [desc]
*****************************************************************************/
int app_snd_capt_init(void)
{
	g_mem_info_t *imem=NULL;
	
	memset(isnd_capt, 0, sizeof(app_snd_capt_t));
	/* set capture volume */
	//# "/usr/bin/amixer cset numid=31 60% > /dev/null"
	system("/usr/bin/amixer cset numid=31 60% > /dev/null 2>&1");
	/* GMEM Address 가져온다 */
	imem = (g_mem_info_t *)app_cap_get_gmem();
	isnd_capt->imem = imem;

	//#--- create thread
	if (thread_create(&isnd_capt->cObj, THR_snd_capt_main, APP_THREAD_PRI, 
					NULL, __FILENAME__) < 0) {
		TRACE_ERR("create thread\n");
		return -1;
	}
	
	TRACE_INFO("... done!\n");
	return 0;
}

void app_snd_capt_exit(void)
{
	app_thr_obj *tObj;

	tObj = &isnd_capt->cObj;
	//#--- stop mic capture
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		app_msleep(20);
	}
	thread_delete(tObj);

	TRACE_INFO("... done!\n");
}
