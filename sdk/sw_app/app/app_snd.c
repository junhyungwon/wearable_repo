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
	
	char *enc_buf = NULL;
	char *addr = NULL;

	struct timeval tv ;
    unsigned int timestamp ;
	aprintf("enter...\n");
	tObj->active = 1;
	
	//init_wav_file(WAV_FILE_NAME, 1, ALSA_SAMPLE_RATE);
	
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
	/* G.711로 Encoding 시 16bit -> 8bit로 변경됨 */
	enc_buf = (char *)malloc(si_size/2);
	if (enc_buf == NULL) {
		eprintf("ulaw buffer malloc\n");
		return NULL;
	}
	
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
#else		
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
/*
		gettimeofday(&tv, NULL) ;
		timestamp = tv.tv_sec + tv.tv_usec*1000 ;
		app_rtsptx_write((void *)ifr->addr, ifr->offset, ifr->b_size,
		0,  2, timestamp);
*/
#endif

	}

#if SYS_CONFIG_VOIP
	dev_snd_stop(&isnd->snd_dup, SND_PCM_PLAY);
	dev_snd_stop(&isnd->snd_in, SND_PCM_CAP);
#endif	
	/* free memory alloc */
	free(enc_buf);
	dev_snd_param_free(&isnd->snd_in);
#if SYS_CONFIG_VOIP	
	dev_snd_param_free(&isnd->snd_dup);
#endif	
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
	
	/* set audio record enable */
	isnd->snd_rec_enable = app_set->rec_info.audio_rec;
	/* set capture volume */
	//# "/usr/bin/amixer cset numid=31 60% > /dev/null"
	dev_snd_set_volume(SND_VOLUME_C, 60);
	
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
