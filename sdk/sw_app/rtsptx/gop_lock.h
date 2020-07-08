/** ===========================================================================
* @file gop_lock.h
*
* @path $(IPNCPATH)\multimedia\encode_stream\stream
*
* @desc 
* .
*
* =========================================================================== */

#ifndef __GOP_LOCK_H__
#define __GOP_LOCK_H__

#include <mem_mng.h>

typedef enum{ 
	GOP_FAIL = -104,
	GOP_INCOMPLETE,
	GOP_INVALID_PRM,
	GOP_ERROR_OP,
	GOP_NOMEM,
	GOP_OVERWRITTEN,
	GOP_NOT_READY,
	GOP_COMPLETE = 0
}GopRet_t;


// rtsp EXT
enum{
	GOP_INDEX_VIDEO_1 = 0,
	GOP_INDEX_VIDEO_2 = 0,
 //RTSP_EXT_CHANNEL	
	GOP_INDEX_AUDIO_1,
	GOP_INDEX_AUDIO_2,
	GOP_INDEX_NUM 	
};


void 	GopInit(void);


GopRet_t LockGopBySerial(int serial, VIDEO_BLK_INFO *pVidInfo,int index);
GopRet_t UnlockGopBySerial(int serial, VIDEO_BLK_INFO *pVidInfo,int index);
void GopCleanup(VIDEO_BLK_INFO *pVidInfo,int index );
GopRet_t LockCurrentGopP(VIDEO_BLK_INFO *pVidInfo, int index);
#endif
