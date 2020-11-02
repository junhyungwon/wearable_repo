/** ===========================================================================
* @file stream.h
*
* @path $(IPNCPATH)\multimedia\encode_stream\stream
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#ifndef _STREAM_H_
#define _STREAM_H_

#include <rendezvous.h>
#include <mem_mng.h>
#include <Msg_Def.h>
#include <cache_mng.h>
#include <gop_lock.h>
#include <semaphore_util.h>

#define USE_GMEM
#define SYS_DEBUG

enum{
	STREAM_FAILURE   = -1,
	STREAM_SUCCESS   = 0
};

enum{
	STREAM_H264_1 = 0,
	STREAM_AUDIO_1,
};

enum{
	STREAM_SEM_VIDEO_CH1 = 0,
    STREAM_SEM_VIDEO_CH2,
	STREAM_SEM_AUDIO_1,
    STREAM_SEM_AUDIO_2,

	STREAM_SEM_MEMCPY,
	STREAM_SEM_CACHECPY,
	STREAM_SEM_GOP,
	STREAM_SEM_NUM
};

enum{
	STREAM_FEATURE_BIT_RATE1 = 0,
	STREAM_FEATURE_BIT_RATE2,
	STREAM_FEATURE_JPG_QUALITY,
	STREAM_FEATURE_M41_FRAMERATE,
	STREAM_FEATURE_M42_FRAMERATE,
	STREAM_FEATURE_JPG_FRAMERATE,
	STREAM_FEATURE_BLC,
	STREAM_FEATURE_SATURATION,
	STREAM_FEATURE_AWB_MODE,
	STREAM_FEATURE_AE_MODE,
	STREAM_FEATURE_BRIGHTNESS,
	STREAM_FEATURE_CONTRAST,
	STREAM_FEATURE_SHARPNESS,
	STREAM_FEATURE_AEW_TYPE,
	STREAM_FEATURE_ENV_50_60HZ,
	STREAM_FEATURE_BINNING_SKIP,
	STREAM_FEATURE_TSTAMPENABLE,
	STREAM_FEATURE_PTZ,
	STREAM_FEATURE_MOTION,
	STREAM_FEATURE_M41_ADV_FEATURE,
	STREAM_FEATURE_M42_ADV_FEATURE,
	STREAM_FEATURE_JPG_ADV_FEATURE,
	STREAM_FEATURE_ROICFG,
	STREAM_FEATURE_OSDTEXT_EN,
	STREAM_FEATURE_HIST_EN,
	STREAM_FEATURE_GBCE_EN,
	STREAM_FEATURE_EVALMSG_EN,
	STREAM_FEATURE_OSDLOGO_EN,
	STREAM_FEATURE_OSDTEXT,
	STREAM_FEATURE_FACE_SETUP,
	STREAM_FEATURE_NUM
};

enum{
	STREAM_VIDEO_1 = 0,
    STREAM_VIDEO_2,

	STREAM_VIDEO_NUM
};

enum{
	STREAM_EXT_MP4_CIF=0,
	STREAM_EXT_JPG,
	STREAM_EXT_NUM
};

typedef struct _ApproMotionPrm{
	int bMotionEnable;
	int bMotionCEnale;
	int MotionLevel;
	int MotionCValue;
	int MotionBlock;
} ApproMotionPrm;

typedef struct _STREAM_SET{
	int stream_wi[STREAM_VIDEO_NUM];
	int stream_he[STREAM_VIDEO_NUM];
    char stream_name[6] ;
} STREAM_SET;

typedef struct _STREAM_PARM{
	MEM_MNG_INFO 	MemInfo;
	int 			MemMngSemId[STREAM_SEM_NUM];
	pthread_t 		threadControl;

	Rendezvous_Obj	objRv[GOP_INDEX_NUM];

	int 			checkNewFrame[GOP_INDEX_NUM];
	int 			lockNewFrame[GOP_INDEX_NUM];
	int				IsQuit;

	int				qid;

	int				ImageWidth[STREAM_VIDEO_NUM];
	int				ImageHeight[STREAM_VIDEO_NUM];
	int				ImageWidth_Ext[STREAM_EXT_NUM];
	int				ImageHeight_Ext[STREAM_EXT_NUM];
	int				JpgQuality;
	int				Mpeg4Quality[STREAM_VIDEO_NUM];
}	STREAM_PARM;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
#define eprintf_rt(x...) do { printf(" [rtsp ERR] %s: ", __func__); printf(x); } while(0)
#ifdef SYS_DEBUG
#define dprintf(x...) do { printf(" [rtsp ] %s: ", __func__); printf(x); } while(0)
#else
#define dprintf(x...)
#endif

STREAM_PARM *stream_get_handle(void);
int stream_init(STREAM_SET *pSet);
void msg_resol(int resol) ;

#ifdef USE_GMEM
int stream_write(int pAddr, int size, int frame_type, int stream_type, unsigned int timestamp);
#else
int stream_write(void *pAddr, int size, int frame_type, int stream_type, unsigned int timestamp);
#endif
int stream_end(void);

void Motion_setparm(ApproMotionPrm* pMotionPrm);
#endif
