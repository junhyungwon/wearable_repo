/** ==================================================================
 *  @file   motiondetect_priv.h
 *
 *  @path    ipnc_mcfw/mcfw/src_bios6/links_m3video/alg/motion_detection/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/
/*
 * DM81XX Motion Detection Algotithm
 */

#ifndef _DM81XX_MD_ALG_H_
#define _DM81XX_MD_ALG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <mcfw/src_bios6/utils/utils_tsk.h>

#define MD_align(value, align)   ((( (value) + ( (align) - 1 ) ) / (align) ) * (align) )

#define ALG_MOTION_DETECT_HORZ_REGIONS  			(8)//(4)
#define ALG_MOTION_DETECT_VERT_REGIONS  			(4)//(3)

// Following values are Sensitivity levels that can come from UI
#define ALG_MOTION_DETECT_SENSITIVITY_LOW           (0)
#define ALG_MOTION_DETECT_SENSITIVITY_MEDIUM        (1)
#define ALG_MOTION_DETECT_SENSITIVITY_HIGH          (2)

#define ALG_MOTION_DETECT_MAX_DETECT_CNT            (3)
#define ALG_MOTION_DETECT_MAX_CH					(4)

#define ALG_MOTION_DETECT_MAX_AREA					(1)

enum {
    ALG_MOTION_S_DETECT = 100,
    ALG_MOTION_S_NO_DETECT,

    ALG_MOTION_S_FAIL = -1,
    ALG_MOTION_S_OK = 0
};

/* MV and SAD structures */

typedef struct {
    /* Starting position of data from the buffer base address */
    unsigned int StartPos;
    /* No. of bytes to jump from the current position to get the next
     * data of this element group */
    unsigned short Jump;
    /* Number of data elements in this group */
    unsigned int Count;

} ElementInfo;

typedef struct {
    unsigned int NumElements;
    ElementInfo elementInfoField0SAD;
    ElementInfo elementInfoField1SAD;
    ElementInfo elementInfoField0MVL0;
    ElementInfo elementInfoField0MVL1;
    ElementInfo elementInfoField1MVL0;
    ElementInfo elementInfoField1MVL1;

} AnalyticHeaderInfo;

typedef struct {
    unsigned int mbMvInfo;
    unsigned int mvJump;

    int ImageWidth;
    int ImageHeight;
    int windowWidth;
    int windowHeight;
    int isKeyFrame;
    int codecType;
    int isDateTimeDraw;

} MD_RunPrm_t;

/* The following structure will store the information for motion detected
 * for a particular selected window */

typedef struct {
    char windowMotionDetected[ALG_MOTION_DETECT_HORZ_REGIONS]
        [ALG_MOTION_DETECT_VERT_REGIONS];

} MD_RunStatus_t;

typedef struct {
    char windowEnableMap[ALG_MOTION_DETECT_HORZ_REGIONS]
        [ALG_MOTION_DETECT_VERT_REGIONS];

} MD_EnableMap_t;

typedef struct {
	int motionenable;
	int motionlevel;
	int blockNum;
	MD_EnableMap_t enMap;
	int update;

	int frame_width;
	int frame_height;
	int win_width;
	int win_height;
	int SAD_THRESHOLD;
	int threshold;
	int detectCnt;

} MD_ChPrm_t;

typedef struct {
	int chid;
	int frameCnt;
	int startCnt;

	int motioncenable;
	int motioncvalue;

	MD_ChPrm_t chPrm[ALG_MOTION_DETECT_MAX_CH];
	MD_RunPrm_t runPrm;
	MD_RunStatus_t runStatus;

	Utils_TskHndl tskHndl;

	UInt32 sadBuf[2];
	MD_RunPrm_t mdRunPrm[2];
	Int32 curSadBuf;
	Int32 prevSadBuf;

} MD_Hndl_t;

int DM81XX_MD_Create();
int DM81XX_MD_Apply(Uint32 mvBuf, Uint32 frameWidth, Uint32 frameHeight,
                    Uint32 isKeyFrame, Uint32 chid);
int DM81XX_MD_Delete();

#ifdef __cplusplus
}
#endif
#endif	/* _DM81XX_MD_ALG_H_ */
