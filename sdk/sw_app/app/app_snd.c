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
	
} app_wave_t;
#endif /* #if __WAVE_DUMP__ */

#define SND_IN_DEV				"plughw:0,0"
#define SND_DUP_DEV				"plughw:1,0"  //# --> plughw:1,1

#define SND_PCM_CAP				0
#define SND_PCM_PLAY			1

typedef struct {
	char path[256];			//# pcm virtual name
	snd_pcm_t *pcm_t;
	
	int channel;
	int sample_rate;
	int mode;
	int num_frames;
	
	char *sampv;

} snd_prm_t;

typedef struct {
	app_thr_obj cObj;	 //# sound in thread
	
	snd_prm_t snd_in;    //# real alsa sound.
	snd_prm_t snd_dup;   //# duplicate real sound
	
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
}
#endif /* #if __WAVE_DUMP__ */ 

/*----------------------------------------------------------------------------
 audio codec function
-----------------------------------------------------------------------------*/
static inline void *advance(void *p, int incr)
{
    unsigned char *d = p;
    return (d + incr);
}

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

static int __snd_prm_init(const char *name, snd_prm_t *prm, 
						int mode, int ch, int rate, int ptime)
{
	int sampc = 0;
	int ret = 0;
	
	memset(prm->path, 0, sizeof(prm->path));
	strcpy(prm->path, name);
	
	/* capture device init */
	prm->channel = ch;
	prm->mode = mode; //# capture
	prm->sample_rate = rate;
	/* 1초에 sample에 해당하는 프레임 수신. 
	 * period size = 1s:8000 = 80ms : ? --> sample rate * ptime / 1000
	 */
	prm->num_frames = ptime; //# period
	sampc = rate * ch * ptime / 1000;
	prm->sampv = (char *)malloc(sampc * SND_PCM_BITS / 8); //# 1sample 16bit..
	if (prm->sampv == NULL)
		ret = -1;
	
	return ret;
}

static void __snd_prm_free(snd_prm_t *prm)
{
	memset(prm->path, 0, sizeof(prm->path));
	if (prm->sampv != NULL) {
		free(prm->sampv);
		prm->sampv = NULL;
	}
}

static int __snd_dev_init(const char *pcm_name, snd_prm_t *prm)
{
	unsigned int freq, nchannels;
	int err;
	
	snd_pcm_t *handle = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;

	snd_pcm_uframes_t buffer_size, period_size;
	snd_pcm_stream_t mode;
	
	period_size = prm->num_frames;
	buffer_size = (period_size * 4); //# alsa 표준
	
	if (prm->mode == SND_PCM_PLAY) {
		mode = SND_PCM_STREAM_PLAYBACK;
	} else {
		mode = SND_PCM_STREAM_CAPTURE;	
	}
	
	err = snd_pcm_open(&handle, pcm_name, mode, 0);
	if (err < 0) {
		dprintf("alsa: could not open device '%s' (%s)\n", pcm_name, snd_strerror(err));
		goto out;
	}
	
	snd_pcm_hw_params_alloca(&hw_params);
	err = snd_pcm_hw_params_any(handle, hw_params);
	if (err < 0) {
		dprintf("Failed to initialize hardware parameters\n");
		goto out;
	}

	/* Set access type.*/
	err = snd_pcm_hw_params_set_access(handle, hw_params,
							SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0) {
		dprintf("Failed to set access type\n");
		goto out;
	}

	/* Set sample format */
	err = snd_pcm_hw_params_set_format(handle, hw_params, SND_PCM_FORMAT_S16_LE);
	if (err < 0) {
		dprintf("cannot set sample format!\n");
		goto out;
	}

	/* Set sample rate. If the exact rate is not supported */
	/* by the hardware, use nearest possible rate.         */
	freq = prm->sample_rate;
	err = snd_pcm_hw_params_set_rate_near(handle, hw_params, &freq, 0);
	if (err < 0) {
		dprintf("Failed to set frequency %d\n", freq);
		goto out;
	}
	
	nchannels = prm->channel;
	err = snd_pcm_hw_params_set_channels(handle, hw_params, nchannels);
	if (err < 0) {
		dprintf("Failed to set number of channels %d\n", nchannels);
		goto out;
	}

    err = snd_pcm_hw_params_set_period_size_near(handle, hw_params, &period_size, 0);
    if (err < 0) {
        dprintf("Failed to set period size to %ld\n", period_size);
        goto out;
    }

    err = snd_pcm_hw_params_set_buffer_size_near(handle, hw_params, &buffer_size);
    if (err < 0) {
        dprintf("Failed to set buffer size to %ld\n", buffer_size);
        goto out;
    }

	/* Apply HW parameter settings to */
    /* PCM device and prepare device  */
    err = snd_pcm_hw_params(handle, hw_params);
	if (err < 0) {
		dprintf("Unable to install hw params\n");
		goto out;
	}

	/* returned approximate maximum buffer size in frames  */
	snd_pcm_hw_params_get_period_size(hw_params, &period_size, 0);
	snd_pcm_hw_params_get_buffer_size(hw_params, &buffer_size);

	dprintf(" ALSA Period Size %ld\n", period_size);
	dprintf(" ALSA Buffer Size %ld\n", buffer_size);

	/* prepare for audio device */
	err = snd_pcm_prepare(handle);
    if (err < 0) {
        printf("Could not prepare handle %p\n", handle);
        goto out;
    }

	prm->pcm_t = handle;
	err = 0;
	
out:
	if (err) {
		eprintf("alsa: init failed: err=%d\n", err);
	}
	
	return err;
}

static void __snd_set_swparam(snd_prm_t *prm, int mode)
{
	snd_pcm_sw_params_t *sw_params = NULL;
	snd_pcm_t *handle = prm->pcm_t;
	snd_pcm_uframes_t buffer_size, period_size;
	int err;
	
	period_size = prm->num_frames;
	buffer_size = (period_size * 4); //# alsa 표준
	
	/* set software params */
	snd_pcm_sw_params_alloca(&sw_params);
    err = snd_pcm_sw_params_current(handle, sw_params);
    if (err < 0) {
        eprintf("Failed to get current software parameters\n");
    }
	snd_pcm_sw_params_set_avail_min(handle, sw_params, period_size);

	if (mode == SND_PCM_PLAY) {
		snd_pcm_sw_params_set_start_threshold(handle, sw_params, buffer_size);
	    snd_pcm_sw_params_set_stop_threshold(handle, sw_params, buffer_size);
	} else {
		snd_pcm_sw_params_set_start_threshold(handle, sw_params, 1);
	    snd_pcm_sw_params_set_stop_threshold(handle, sw_params, buffer_size);	
	}

    err = snd_pcm_sw_params(handle, sw_params);
    if (err < 0) {
        eprintf("Failed to set software parameters\n");
    }
}

static ssize_t __snd_dev_read(snd_prm_t *prm)
{
	snd_pcm_t *handle = prm->pcm_t;
	char *rbuf = (char *)prm->sampv;

	ssize_t result = 0;
	size_t count = (size_t)prm->num_frames;
	int hwshift;

	hwshift = prm->channel * (SND_PCM_BITS / 8);

	while (count > 0)
	{
		snd_pcm_sframes_t r;

		r = snd_pcm_readi(handle, rbuf, count);
		if (r <= 0)
		{
			switch (r) {
			case 0:
				dprintf(" Failed to read frames(zero)\n");
				continue;

			case -EAGAIN:
				dprintf(" pcm wait (count = %d)!!\n", count);
				snd_pcm_wait(handle, 100);
				break;

			case -EPIPE:
				snd_pcm_prepare(handle);
				dprintf(" pcm overrun(count = %d)!!\n", count);
				break;

			default:
				dprintf(" read error!!\n");
				return -1;
			}
		}

		/* read size is frame size. For byte conversion..*/
		/* Frame = 1sample * Total Channel, 1 sample = 16bit (2Byte) */
		if (r > 0) {
			rbuf = advance(rbuf, (r * hwshift));
			result += (r * hwshift);
			count -= r;
		}
	}

	return result;
}

static ssize_t __snd_dev_write(snd_prm_t *prm, size_t w_samples)
{
	snd_pcm_t *handle = prm->pcm_t;
	char *rbuf = (char *)prm->sampv;
	
	ssize_t result = 0;
	size_t count = (size_t)w_samples;
	size_t period = (size_t)prm->num_frames;
	int hwshift;

	hwshift = prm->channel * (SND_PCM_BITS / 8); //# 16bit * channel / 8bit

	if (count < period) {
    	snd_pcm_format_set_silence(SND_PCM_FORMAT_S16_LE, rbuf + (count * hwshift),
						(period - count) * prm->channel);
        count = period;
	}

	while (count > 0)
	{
		snd_pcm_sframes_t r;
		int ret;

		r = snd_pcm_writei(handle, rbuf, count);
		if (r == -EAGAIN || (r >= 0 && (size_t)r < count)) {
			dprintf("pcm write wait(100ms)!!\n");
			snd_pcm_wait(handle, 100);
		} else if (r == -EPIPE) {
			dprintf("pcm write underrun!!\n");
			ret = snd_pcm_prepare(handle);
			if (ret < 0) {
				dprintf("Failed to prepare handle %p\n", handle);
			}
			continue;
		} else if (r == -ESTRPIPE) {
			ret = snd_pcm_resume(handle);
			if (ret < 0) {
				dprintf("Failed. Restarting stream.\n");
			}
			continue;
		} else if (r < 0) {
			dprintf("write error\n");
			return -1;
		}

		if (r > 0) {
			rbuf = advance(rbuf, r * hwshift);
			result += (r * hwshift);
			count -= r;
		}
	}

	return result;
}

static void __snd_dev_stop(snd_prm_t *prm, int mode)
{
	snd_pcm_t *handle = prm->pcm_t;
	
	if (handle != NULL) {
		if (mode == SND_PCM_PLAY) {
			snd_pcm_nonblock(handle, 0);
			/* Stop PCM device after pending frames have been played */
			snd_pcm_drain(handle);
			//snd_pcm_nonblock(handle, 1);
		} 
		snd_pcm_hw_free(handle);
		snd_pcm_close(handle);
	}
}

static void __snd_dev_start(snd_prm_t *prm)
{
	snd_pcm_t *handle = prm->pcm_t;
	int err;
	
	/* Start */
	err = snd_pcm_start(handle);
	if (err) {
		dprintf("alsa: could not start ausrc device %s, (%s)\n", 
				prm->path, snd_strerror(err));
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
	
	int exit=0;
	int ret, idx;
	int read_sz = 0;
	int si_size, enc_size;
	
	char *enc_buf = NULL;
	char *addr = NULL;

	aprintf("enter...\n");
	tObj->active = 1;
	
	//init_wav_file(WAV_FILE_NAME, 1, ALSA_SAMPLE_RATE);
	
	/* get alsa period size (in sec) */
	read_sz = SND_PCM_SRATE * SND_PCM_PTIME / 1000; //# 
	ret = __snd_prm_init("aic3x", &isnd->snd_in, SND_PCM_CAP, 
				SND_PCM_CH, SND_PCM_SRATE, read_sz);
	ret |= __snd_prm_init("dummy", &isnd->snd_dup, SND_PCM_PLAY, 
				SND_PCM_CH, SND_PCM_SRATE, read_sz);
	
	ret |= __snd_dev_init(SND_IN_DEV, &isnd->snd_in);
	ret |= __snd_dev_init(SND_DUP_DEV, &isnd->snd_dup);
	if (ret) {
		eprintf("Failed to init sound device!\n");
		return NULL;
	}
	
	si_size  = (read_sz * SND_PCM_CH * (SND_PCM_BITS / 8));
	/* G.711로 Encoding 시 16bit -> 8bit로 변경됨 */
	enc_buf = (char *)malloc(si_size/2);
	if (enc_buf == NULL) {
		eprintf("ulaw buffer malloc\n");
		return NULL;
	}
	
	__snd_dev_start(&isnd->snd_in);
	__snd_set_swparam(&isnd->snd_dup, SND_PCM_PLAY);

	while (!exit)
	{
		int bytes = 0;
		
		if (tObj->cmd == APP_CMD_EXIT) {
			break;
		}

		bytes = __snd_dev_read(&isnd->snd_in);
		if(bytes < 0) {
			eprintf("sound device error!!\n");
			continue;
		}
		
		//# copy to dup device
		app_memcpy(isnd->snd_dup.sampv, isnd->snd_in.sampv, bytes);
		/* VOIP를 사용할 경우에만 copy ?? */
		__snd_dev_write(&isnd->snd_dup, bytes/2); 
		
		if (app_set->rec_info.audio_rec)
		{
			//# audio codec : g.711
			enc_size = alg_ulaw_encode((unsigned short *)enc_buf, (unsigned short *)isnd->snd_in.sampv, si_size);
			addr = g_mem_get_addr(enc_size, &idx);
			if(addr == NULL) {
				eprintf("audio gmem is null\n");
				continue;
			}

			ifr = &isnd->imem->ifr[idx];
			ifr->d_type = CAP_TYPE_AUDIO;
			ifr->ch = 0;
			ifr->addr = addr;
			ifr->offset = (int)addr - g_mem_get_virtaddr();
			ifr->b_size = enc_size;
			//ifr->t_sec = (Uint32)(captime/1000);
			//ifr->t_msec = (Uint32)(captime%1000);
			app_memcpy(addr, enc_buf, enc_size);
		}
	}

	__snd_dev_stop(&isnd->snd_dup, SND_PCM_PLAY);
	__snd_dev_stop(&isnd->snd_dup, SND_PCM_CAP);
	
	/* free memory alloc */
	free(enc_buf);
	__snd_prm_free(&isnd->snd_in);
	__snd_prm_free(&isnd->snd_dup);
	
	tObj->active = 0;
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
	
	memset(isnd, 0, sizeof(app_snd_t));
	
	/* GMEM Address 가져온다 */
	imem = (g_mem_info_t *)app_cap_get_gmem();
	isnd->imem = imem;

	//#--- create thread
	if (thread_create(&isnd->cObj, THR_snd_cap, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
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
