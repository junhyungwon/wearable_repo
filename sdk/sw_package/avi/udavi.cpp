// AVIEncoder.cpp: implementation of the CAVIEncoder class.
//
//////////////////////////////////////////////////////////////////////
#include <inttypes.h>
#include <iostream>
#include <limits.h>
#include <assert.h>

#include <stdio.h>
#include "syscommon.h"
#include "mmsystem.h"
#include "aviwriter.hh"
#include "avirecover.hh"
#include "udavi.h"

void* LIBAVI_createAvi(char* filePullPath, AVI_SYSTEM_PARAM* pAviPrm)
{
	CAVIWriter* pWriter;
	int i;

	pWriter = CAVIWriter::CreateNew();

	if(pAviPrm->nVidCh < 0) {
		printf("[ERR] avi param fail! (number of chann %d\n", pAviPrm->nVidCh);
	}
	for(i=0; i<pAviPrm->nVidCh; i++) {
		if(pAviPrm->nVidWi[i] == 0 || pAviPrm->nVidHe[i] == 0) {
			printf("[ERR] avi ch%d param fail! (width or height)\n", i);
		}
	}

	if(pWriter != NULL)
	{
		SYSTEM_PARAM sysParm;

		sysParm.nVidChannel = pAviPrm->nVidCh;		//# number of channel
		sysParm.fFrameRate 	= pAviPrm->fFrameRate;
		for(i=0; i<MAX_VID_CH; i++) {
			sysParm.nVidWidth[i] 	= pAviPrm->nVidWi[i];
			sysParm.nVidHeight[i] 	= pAviPrm->nVidHe[i];
		}

		sysParm.bEnableAudio		= pAviPrm->bEnAudio;
		sysParm.nAudioBitRate 		= pAviPrm->nAudioBitRate;
		sysParm.nAudioChannel 		= pAviPrm->nAudioChannel;
		sysParm.nAudioBitPerSample 	= pAviPrm->nAudioBitPerSample;
		sysParm.nAudioFrameSize 	= pAviPrm->nAudioFrameSize;
		sysParm.nSamplesPerSec 		= pAviPrm->nSamplesPerSec;

		sysParm.bEnableMeta			= pAviPrm->bEnMeta;
		sysParm.bEncrypt			= pAviPrm->bEncrypt;

		printf("LIBAVI_createAvi pAviPrm->bEncrypt = %d\n",pAviPrm->bEncrypt) ;

		switch(pAviPrm->uVideoType)
		{
			case ENCODING_MPEG4:
				sysParm.uVideoType = IME6400_SD_MPEG4;
			break;
			case ENCODING_H264:
				sysParm.uVideoType = IME6400_SD_H264;
			break;
			default:
				sysParm.uVideoType = IME6400_SD_H264;
			break;
		}

		switch(pAviPrm->nAudioType)
		{
			case ENCODING_ADPCM:
				sysParm.nAudioType = IME6400_AM_ADPCM;
			break;
			case ENCODING_ULAW:
				sysParm.nAudioType = IME6400_AM_MULAW;
			break;
			default:
				sysParm.nAudioType = IME6400_AM_MULAW;
			break;
		}

		if(pWriter->Open(filePullPath, &sysParm) != LIBAVI_SUCCESS) {
			delete pWriter;
			pWriter = NULL;
		}
	}

	return pWriter;
}

int LIBAVI_write_frame(void* handlAvi, AVI_FRAME_PARAM* pframePrm)
{
	CAVIWriter* pWriter = (CAVIWriter*)handlAvi;
	int ret;

	if(pWriter != NULL)
	{
		if(pframePrm->data_type == DATA_TYPE_VIDEO) {
			ret = pWriter->WriteVideoData((BYTE*)pframePrm->buf, pframePrm->size, pframePrm->iskey_frame, 1, pframePrm->channel);
		}
		if(pframePrm->data_type == DATA_TYPE_AUDIO) {
			ret = pWriter->WriteAudioData((BYTE*)pframePrm->buf, pframePrm->size, 1);
		}
		if(pframePrm->data_type == DATA_TYPE_META) {
			ret = pWriter->WriteMetaData((BYTE*)pframePrm->buf, pframePrm->size, 1);
		}
	}

	if(ret < 0) {
		return LIBAVI_ERROR;
	}

	return LIBAVI_SUCCESS;
}

int LIBAVI_closeAvi(void* handlAvi)
{
	CAVIWriter* pWriter = (CAVIWriter*)handlAvi;

	if(pWriter != NULL) {
		pWriter->Close();
		delete pWriter;
		pWriter = NULL;
	}

	return LIBAVI_SUCCESS;
}


int LIBAVI_recoverFile(char* filePullPath)
{
	return AVIrcStart(filePullPath);
}

char* LIBAVI_recoverGetErrMsg()
{
	return AVIrcGetErrMsg();
}



