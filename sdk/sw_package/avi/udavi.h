/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	udavi.h
 * @brief	interface for the common define.
 */
/*****************************************************************************/
#ifndef _UDAVI_H_
#define _UDAVI_H_

#ifdef __cplusplus
extern "C"{
#endif

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define LIBAVI_ERROR	(-1)
#define LIBAVI_SUCCESS	(0)

typedef enum {
    DATA_TYPE_VIDEO,
    DATA_TYPE_AUDIO,
    DATA_TYPE_META,
    DATA_TYPE_MAX
} enumDVR_DATA_TYPE;

typedef enum {
	NSDVR_PICTURE_TYPE_I,
	NSDVR_PICTURE_TYPE_P,
	NSDVR_PICTURE_TYPE_B,
} enumNSDVR_PICTURE_TYPE;

typedef enum {
    ENCODING_UNKNOWN=0,

    /* Video */
    ENCODING_MPEG4=1,
    ENCODING_JPEG,
    ENCODING_H264,

    /* Audio */
    ENCODING_PCM=32,
    ENCODING_ULAW,
    ENCODING_ALAW,
    ENCODING_ADPCM,
    ENCODING_G721,
    ENCODING_G723,
    ENCODING_MP3,

    MAX_ENCODING_TYPE
} enumDVR_ENCODING_TYPE;

typedef struct tag_AVI_SYSTEM_PARAM {
	//# System information
	int nVidCh;
	int bEnAudio;
	int bEnMeta;

	// Video information
	int uVideoType;
	int nVidWi[MAX_VID_CH];
	int nVidHe[MAX_VID_CH];
	double fFrameRate;

	// Audio information
	int nAudioType;			//Audio encoding type
	int nAudioChannel;		//1: momo, 2: stereo
	int nSamplesPerSec;		//8KHz, 16KHz, ...
	int nAudioBitRate;		//96Kbps, 128Kbps,...
	int nAudioFrameSize;
	int nAudioBitPerSample;	//8bit, 16bit,...
} AVI_SYSTEM_PARAM;

typedef struct tag_AVI_FRAME_PARAM {
	char *buf;
	int size;

	int channel;
	int data_type;		//0: video_main, 1: video_sub, 2: audio
	int iskey_frame;
	int time_sec;
	int time_usec;
} AVI_FRAME_PARAM;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
void* LIBAVI_createAvi(char* filePullPath, AVI_SYSTEM_PARAM* pAviPrm);
int LIBAVI_write_frame(void* handlAvi, AVI_FRAME_PARAM* pframePrm);
int LIBAVI_closeAvi(void* handlAvi);

int LIBAVI_recoverFile(char* filePullPath);
char* LIBAVI_recoverGetErrMsg();

#ifdef __cplusplus
}
#endif

#endif // _UDAVI_H_
