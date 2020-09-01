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

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 avi file open/close function
-----------------------------------------------------------------------------*/
FILE *avi_file_open(char *filename, stream_info_t *ifr, int snd_on)
{
    char msg[128] = {0, };
	AVI_SYSTEM_PARAM aviInfo;
	FILE *favi;
	
	memset(&aviInfo, 0, sizeof(AVI_SYSTEM_PARAM));

	aviInfo.nVidCh	= 1;
	aviInfo.bEnMeta	= TRUE; //# FALSE
	aviInfo.uVideoType	= ENCODING_H264;

	aviInfo.nVidWi[0] = ifr->frm_wi;
	aviInfo.nVidHe[0] = ifr->frm_he;
	aviInfo.fFrameRate	= (double)(ifr->frm_rate*1000./1001.);

	if (snd_on) {
		aviInfo.bEnAudio 			= TRUE;
		aviInfo.nAudioType			= ENCODING_ULAW;
		aviInfo.nAudioChannel		= 1;
		aviInfo.nSamplesPerSec		= 8000; //SND_PCM_SRATE;
		aviInfo.nAudioBitRate		= 8000; //SND_PCM_SRATE;
		aviInfo.nAudioBitPerSample	= 16;
		aviInfo.nAudioFrameSize		= 2000; //# fixed
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

	if (favi)
	{
		frame.buf	= (char *)(gmem_addr+ifr->offset); //(ifr->addr);
		frame.size	= ifr->b_size;

		if (ifr->d_type == DATA_TYPE_VIDEO) {
			frame.data_type		= ifr->d_type;
			frame.channel 		= ifr->ch;
			frame.iskey_frame 	= ifr->is_key;
		}
		else if(ifr->d_type == DATA_TYPE_AUDIO) {
			frame.data_type		= ifr->d_type;
		}
		else if(ifr->d_type == DATA_TYPE_META) {
			frame.data_type		= ifr->d_type;
 		}
		else {
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
