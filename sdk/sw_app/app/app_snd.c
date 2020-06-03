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
#include "dev_sound.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_gmem.h"
#include "app_cap.h"
#include "app_snd.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef struct {
	snd_pcm_t *pcm_t;
	char *buf;
} snd_data_t;

typedef struct {
	app_thr_obj cObj;		//# sound in thread
	snd_data_t snd_in;
        int evt_rec ;
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
	int exit=0;

	snd_data_t *pSnd = &isnd->snd_in;
	int nch, si_size;
	int buf_sz, period_sz;
	int ret;

	char *enc_buf;
	int enc_size;

	int idx;
	char *addr;
	stream_info_t *ifr;

	aprintf("enter...\n");
	tObj->active = 1;

	buf_sz    = 16000; 				//# frames
	period_sz = (buf_sz / 4);
	nch		  = SND_PCM_CH;			//# number of pcm channels
	si_size	  = (period_sz * nch * 2);

	pSnd->pcm_t = (snd_pcm_t *)dev_snd_aic3x_init(SND_PCM_REC, SND_PCM_SRATE, nch, buf_sz, period_sz);
	if(pSnd->pcm_t == NULL) {
		eprintf("snd capture device init\n");
		return NULL;
	}

	pSnd->buf = malloc(si_size);
	if(pSnd->buf==NULL) {
		eprintf("snd buffer malloc\n");
		return NULL;
	}
	enc_buf = malloc(si_size/2);
	if(enc_buf == NULL) {
		eprintf("ulaw buffer malloc\n");
		return NULL;
	}

    isnd->evt_rec = 1 ;
	while(!exit)
	{
		if(tObj->cmd == APP_CMD_EXIT) {
			break;
		}

		ret = dev_snd_aic3x_read((void *)pSnd->pcm_t, pSnd->buf, nch, period_sz);
		if(ret < 0) {
			continue;
		}

		//# audio codec : g.711
		enc_size = alg_ulaw_encode((unsigned short *)enc_buf, (unsigned short *)pSnd->buf, si_size);

		addr = g_mem_get_addr(enc_size, &idx);
		if(addr == NULL) {
			continue;
		}

		ifr = &isnd->imem->ifr[idx];
		ifr->d_type = CAP_TYPE_AUDIO;
		ifr->ch = 0;
		ifr->addr = addr;
		ifr->b_size = enc_size;

		app_memcpy(addr, enc_buf, enc_size);
	}

	free(enc_buf);
	free(pSnd->buf);

	dev_snd_aic3x_close((void *)pSnd->pcm_t, SND_PCM_REC);

	tObj->active = 0;
	aprintf("...exit\n");
    isnd->evt_rec = 0 ;

	return NULL;
}

/*****************************************************************************
* @brief    sound init/exit function
* @section  [desc]
*****************************************************************************/
int app_snd_start(g_mem_info_t *imem)
{
    if(isnd->evt_rec)
       return EFAIL ;      

	memset(isnd, 0, sizeof(app_snd_t));

	isnd->imem = imem;

	//# set default device
	dev_snd_set_aic3x_input_path(SND_MIC_IN);
	dev_snd_set_aic3x_volume(SND_VOLUME_C, 80);

	//#--- create thread
	if(thread_create(&isnd->cObj, THR_snd_cap, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}

	aprintf("... done!\n");

	return 0;
}

void app_snd_stop(void)
{
	app_thr_obj *tObj;

    if(!isnd->evt_rec)
        return ;

	//#--- stop thread
	tObj = &isnd->cObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		app_msleep(20);
	}
	thread_delete(tObj);

	dprintf("... done!\n");
}
