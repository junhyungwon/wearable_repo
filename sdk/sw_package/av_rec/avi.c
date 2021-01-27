/******************************************************************************
 * NEXTT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_main.c
 * @brief
 * @section MODIFY history
 *     - 2020.07.08 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmem.h>

#include "main.h"
#include "avi.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static unsigned int gmem_addr;
extern char *enc_buf;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 audio codec function
-----------------------------------------------------------------------------*/
static int alg_ulaw_encode(unsigned short *dst, unsigned short *src, int bufsize)
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

/*----------------------------------------------------------------------------
 avi file open/close function
-----------------------------------------------------------------------------*/
FILE *avi_file_open(char *filename, stream_info_t *ifr, int snd_on, int ch, int rate, int btime)
{
    char msg[128] = {0, };
	AVI_SYSTEM_PARAM aviInfo;
	FILE *favi;
	int i;
	
	memset(&aviInfo, 0, sizeof(AVI_SYSTEM_PARAM));
	
	aviInfo.nVidCh	= MODEL_CH_NUM;
	aviInfo.bEnMeta	= TRUE; //# FALSE
	aviInfo.uVideoType	= ENCODING_H264;
	
	for (i = 0; i < aviInfo.nVidCh; i++) {
		aviInfo.nVidWi[i] = ifr->frm_wi;
		aviInfo.nVidHe[i] = ifr->frm_he;
	}
	aviInfo.fFrameRate	= (double)(ifr->frm_rate*1000./1001.);

	if (snd_on) {
		aviInfo.bEnAudio 			= TRUE;
		aviInfo.nAudioType			= ENCODING_ULAW;
		aviInfo.nAudioChannel		= ch;
		aviInfo.nSamplesPerSec		= rate; //SND_PCM_SRATE;
		aviInfo.nAudioBitRate		= rate; //SND_PCM_SRATE;
		aviInfo.nAudioBitPerSample	= 16;   //# fixed 16bits
		aviInfo.nAudioFrameSize		= btime; //# fixed
	}
	
	favi = LIBAVI_createAvi(filename, &aviInfo);
	if (favi == NULL) {
        log_write(" !!! file open failed !!!");
		eprintf("avi save handle is NULL!\n");
	}

	return favi;
}

void avi_file_close(FILE *favi, char *fname)
{
	if (favi) {
		LIBAVI_closeAvi(favi);
	}
	favi = NULL;
    
//	app_file_add(fname) ;
}

int avi_file_write(FILE *favi, stream_info_t *ifr)
{
	AVI_FRAME_PARAM frame;
	int enc_size=0, sz;
	char *tmp=NULL;
	
	if (favi)
	{
		if (ifr->d_type == DATA_TYPE_VIDEO) 
		{
			frame.buf			= (char *)(gmem_addr+ifr->offset); //(ifr->addr);
			frame.size			= ifr->b_size;
			frame.data_type		= ifr->d_type;
			frame.channel 		= ifr->ch;
			frame.iskey_frame 	= ifr->is_key;
		}
		else if(ifr->d_type == DATA_TYPE_AUDIO) 
		{
			/* muraw encoding required */
			tmp = (char *)(gmem_addr+ifr->offset);
			sz  = ifr->b_size;
			enc_size=alg_ulaw_encode((unsigned short *)enc_buf, (unsigned short *)tmp, sz);
			
			frame.buf       = enc_buf;
			frame.size      = enc_size;
			frame.data_type	= ifr->d_type;
		}
		else if(ifr->d_type == DATA_TYPE_META) 
		{
			frame.buf		= (char *)(gmem_addr+ifr->offset); //(ifr->addr);
			frame.size		= ifr->b_size;
			frame.data_type	= ifr->d_type;
 		}
		else 
		{
			dprintf("unknown data type\n");
			return 0;
		}
		
		if (LIBAVI_write_frame(favi, &frame) < 0) {
			return -1;
		}
	}

	return 0;
}

int avi_file_init(unsigned int addr)
{
	/* ifr->offset을 사용할 경우 이 값을 더해야 함 */
	gmem_addr = addr;
	
	return 0;
}
