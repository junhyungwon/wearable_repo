/** ==================================================================
 *  @file   ti_motiondetect.c
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
/* DM81XX Motion Detection Algorithm */

#include <xdc/std.h>
#include "motiondetect_priv.h"
#include <mcfw/src_bios6/links_common/system/system_priv_common.h>
#include <mcfw/src_bios6/links_m3video/iva_enc/encLink_priv.h>

#include <mcfw/interfaces/link_api/swosdLink.h>

/* Motion Detection Task stack */
#pragma DATA_ALIGN(gMD_tskStack, 32)
#pragma DATA_SECTION(gMD_tskStack, ".bss:taskStackSection")
UInt8 gMD_tskStack[MD_TSK_STACK_SIZE];

/* Motion Detection Algorithm handle */
MD_Hndl_t gMDHndl;

/* ===================================================================
 *  @func     DM81XX_MD_SetPrm
 *
 *  @desc     GUI params set functions
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
int DM81XX_MD_SetPrm(int streamId, int *pPrm)
{
	gMDHndl.motioncenable = pPrm[1];
	gMDHndl.motioncvalue = pPrm[3];

	gMDHndl.chPrm[streamId].motionenable = pPrm[0];
	gMDHndl.chPrm[streamId].motionlevel = pPrm[2];
	gMDHndl.chPrm[streamId].blockNum = pPrm[4];

	gMDHndl.chPrm[streamId].detectCnt = 0;
	gMDHndl.chPrm[streamId].update = 1;

	return 0;
}

/* ===================================================================
 *  @func     DM81XX_MD_Calc
 *
 *  @desc     Motion Detection Calculate
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
int DM81XX_MD_Calc(MD_Hndl_t *hndl, int MvDataOffset)
{
	MD_ChPrm_t *chp = (MD_ChPrm_t *)&hndl->chPrm[hndl->chid];
    unsigned int mbMV_data;
    int i, j, status;
    unsigned short mbSAD;
    int warning_count = 0;

    mbMV_data = hndl->runPrm.mbMvInfo + MvDataOffset;

    for (i = 0; i < chp->win_height; i++)
    {
        for (j = 0; j < chp->win_width; j++)
        {
			mbSAD = *((unsigned short *) mbMV_data);
            if (mbSAD > chp->SAD_THRESHOLD) {
                warning_count++;
                if(warning_count >= chp->threshold)
                	break;
            }
            mbMV_data += (hndl->runPrm.mvJump);
        }
        mbMV_data += ((chp->frame_width - chp->win_width) * hndl->runPrm.mvJump);
    }

    /* If the warning_count is bigger than hndl->threshold, the
     * function will return a alarm signal */
    status = (warning_count >= chp->threshold)
				? ALG_MOTION_S_DETECT : ALG_MOTION_S_NO_DETECT;

    return status;
}

/* ===================================================================
 *  @func     DM81XX_MD_Start
 *
 *  @desc     DM81XX Motion Detection start
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
int DM81XX_MD_Start(MD_Hndl_t * hndl)
{
	MD_ChPrm_t *chp = (MD_ChPrm_t *)&hndl->chPrm[hndl->chid];
    int detect_cnt = 0;
    int i = 0, j = 0;
    int MvDataOffset = 0;

    for (i = 0; ((chp->win_height * i) < chp->frame_height); i++)
    {
        if (i >= ALG_MOTION_DETECT_VERT_REGIONS)
            continue;

        for (j = 0; ((chp->win_width * j) < chp->frame_width); j++)
        {
            if (j >= ALG_MOTION_DETECT_HORZ_REGIONS)
                continue;

            if (chp->enMap.windowEnableMap[j][i] == 1)   // 1- True , 0 - False
            {
                MvDataOffset = ((chp->win_width * j) + (chp->frame_width * chp->win_height * i));
                MvDataOffset *= (hndl->runPrm.mvJump);
                if (DM81XX_MD_Calc(hndl, MvDataOffset) == ALG_MOTION_S_DETECT) {
                    hndl->runStatus.windowMotionDetected[j][i] = 1; // Motion Detected
                    detect_cnt++;
                    if(detect_cnt >= ALG_MOTION_DETECT_MAX_AREA)
                    	break;
                }
                else {
                    hndl->runStatus.windowMotionDetected[j][i] = 0; // Motion Not Detected
                }
            }
        }
    }

    return ((detect_cnt >= ALG_MOTION_DETECT_MAX_AREA) ? ALG_MOTION_S_DETECT : ALG_MOTION_S_NO_DETECT);
}

/* ===================================================================
 *  @func     DM81XX_MD_GetThres
 *
 *  @desc     DM81XX Get Threshold
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
static int DM81XX_MD_GetThres(MD_Hndl_t *hndl)
{
	MD_ChPrm_t *chp = (MD_ChPrm_t *)&hndl->chPrm[hndl->chid];

	chp->frame_width = (hndl->runPrm.ImageWidth >> 4);    // Number of macroblocks in frame width
	chp->frame_height = (hndl->runPrm.ImageHeight >> 4);  // Number of macroblocks in frame height

	/* Set the motion block size base on image size */
	chp->win_width = (hndl->runPrm.windowWidth >> 4);     // Window width in macroblocks
	chp->win_height = (hndl->runPrm.windowHeight >> 4);   // Window height in macroblocks

	if (chp->frame_width >= 120)								//# for 1080P
		chp->SAD_THRESHOLD = 6000;
	else if (chp->frame_width < 120 && chp->frame_width >= 80)	//# for 720P
		chp->SAD_THRESHOLD = 5500;
	else if (chp->frame_width < 80 && chp->frame_width >= 40)	//# for D1
		chp->SAD_THRESHOLD = 5000;
	else														//# less than D1
		chp->SAD_THRESHOLD = 4500;

	return ALG_MOTION_S_OK;
}

/* ===================================================================
 *  @func     DM81XX_MD_GetSensitivity
 *
 *  @desc     DM81XX Get Sensitivity
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
static int DM81XX_MD_GetSensitivity(MD_Hndl_t * hndl)
{
	MD_ChPrm_t *chp = (MD_ChPrm_t *)&hndl->chPrm[hndl->chid];

	if (chp->motionlevel == ALG_MOTION_DETECT_SENSITIVITY_LOW)
		chp->threshold = ((chp->win_width*chp->win_height)*12) / 100;
	else if (chp->motionlevel == ALG_MOTION_DETECT_SENSITIVITY_MEDIUM)
		chp->threshold = ((chp->win_width*chp->win_height)*8) / 100;
	else if (chp->motionlevel == ALG_MOTION_DETECT_SENSITIVITY_HIGH)
		chp->threshold = ((chp->win_width*chp->win_height)*4) / 100;

    return ALG_MOTION_S_OK;
}

/* ===================================================================
 *  @func     DM81XX_MD_SetEnableMap
 *
 *  @desc     DM81XX Set EnableMap
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
static int DM81XX_MD_SetEnableMap(MD_Hndl_t *hndl)
{
	MD_ChPrm_t *chp = (MD_ChPrm_t *)&hndl->chPrm[hndl->chid];

	#if 0
    chp->enMap.windowEnableMap[0][0] = (chp->blockNum & 0x100) ? 1 : 0;
    chp->enMap.windowEnableMap[1][0] = (chp->blockNum & 0x200) ? 1 : 0;
    chp->enMap.windowEnableMap[2][0] = (chp->blockNum & 0x400) ? 1 : 0;
    chp->enMap.windowEnableMap[3][0] = (chp->blockNum & 0x800) ? 1 : 0;
    chp->enMap.windowEnableMap[0][1] = (chp->blockNum & 0x010) ? 1 : 0;
    chp->enMap.windowEnableMap[1][1] = (chp->blockNum & 0x020) ? 1 : 0;
    chp->enMap.windowEnableMap[2][1] = (chp->blockNum & 0x040) ? 1 : 0;
    chp->enMap.windowEnableMap[3][1] = (chp->blockNum & 0x080) ? 1 : 0;
    chp->enMap.windowEnableMap[0][2] = (chp->blockNum & 0x001) ? 1 : 0;
    chp->enMap.windowEnableMap[1][2] = (chp->blockNum & 0x002) ? 1 : 0;
    chp->enMap.windowEnableMap[2][2] = (chp->blockNum & 0x004) ? 1 : 0;
    chp->enMap.windowEnableMap[3][2] = (chp->blockNum & 0x008) ? 1 : 0;
    #endif

    int i, j;
    for(i=0; i<ALG_MOTION_DETECT_VERT_REGIONS; i++)
    	for(j=0; j<ALG_MOTION_DETECT_HORZ_REGIONS; j++)
    		chp->enMap.windowEnableMap[j][i] = 1;

    //Vps_printf("--- %s: chid %d, blockNum 0x%x\n", __func__, chid, chp->blockNum);

    return ALG_MOTION_S_OK;
}

/* ===================================================================
 *  @func     DM81XX_MD_Run
 *
 *  @desc     DM81XX Motion Detection Run
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
int DM81XX_MD_Run(MD_Hndl_t *hndl, MD_RunPrm_t *prm, MD_RunStatus_t *status)
{
    if (hndl == NULL) {
        return ALG_MOTION_S_FAIL;
    }

    /* Parm tranfer */
    hndl->runPrm = *prm;
    hndl->runStatus = *status;
    hndl->frameCnt++;
    //# skip first
    if ((hndl->runPrm.isKeyFrame == TRUE) || (hndl->frameCnt < hndl->startCnt)) {
        return ALG_MOTION_S_NO_DETECT;
    }

    if(hndl->chPrm[hndl->chid].update)
    {
    	DM81XX_MD_GetThres(hndl);
    	DM81XX_MD_GetSensitivity(hndl);
    	DM81XX_MD_SetEnableMap(hndl);

    	hndl->chPrm[hndl->chid].update = 0;
    }

    return DM81XX_MD_Start(hndl);
}

/* ===================================================================
 *  @func     DM81XX_MD_Apply
 *
 *  @desc     DM81XX Motion Detection Apply
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
int DM81XX_MD_Apply(Uint32 mvBuf, Uint32 frameWidth, Uint32 frameHeight,
                    Uint32 isKeyFrame, Uint32 chid)
{
    int copySize;
    AnalyticHeaderInfo *pAnalyticHeaderInfo = (AnalyticHeaderInfo *) mvBuf;
    int numElements = pAnalyticHeaderInfo->NumElements;

	if(chid > ALG_MOTION_DETECT_MAX_CH-1) {
		return 0;
	}

	if((gMDHndl.chPrm[chid].motionenable==0) || (numElements==0) || (isKeyFrame==1) ) {
    	return 0;
    }

    if(gMDHndl.curSadBuf == gMDHndl.prevSadBuf) {
    	//Vps_printf("--- %s: reject frame\n", __func__);
    	return 0;
    }

    copySize =
        (MD_align(frameWidth, 16) / 16) * (MD_align(frameHeight, 16) / 16) *
        pAnalyticHeaderInfo->elementInfoField0SAD.Jump;

    /* copy SAD data to sad buf */
    memcpy((void *) gMDHndl.sadBuf[gMDHndl.curSadBuf],
           (void *) (mvBuf + (pAnalyticHeaderInfo->elementInfoField0SAD.StartPos)), copySize);

    gMDHndl.mdRunPrm[gMDHndl.curSadBuf].mbMvInfo =
        gMDHndl.sadBuf[gMDHndl.curSadBuf];
    gMDHndl.mdRunPrm[gMDHndl.curSadBuf].mvJump =
        pAnalyticHeaderInfo->elementInfoField0SAD.Jump;
    gMDHndl.mdRunPrm[gMDHndl.curSadBuf].ImageWidth = frameWidth;
    gMDHndl.mdRunPrm[gMDHndl.curSadBuf].ImageHeight = frameHeight;
    gMDHndl.mdRunPrm[gMDHndl.curSadBuf].isKeyFrame = isKeyFrame;
    gMDHndl.mdRunPrm[gMDHndl.curSadBuf].codecType = 0;
    gMDHndl.mdRunPrm[gMDHndl.curSadBuf].isDateTimeDraw = 0;
    gMDHndl.mdRunPrm[gMDHndl.curSadBuf].windowWidth =
        (gMDHndl.mdRunPrm[gMDHndl.curSadBuf].ImageWidth / ALG_MOTION_DETECT_HORZ_REGIONS);
    gMDHndl.mdRunPrm[gMDHndl.curSadBuf].windowHeight =
        (gMDHndl.mdRunPrm[gMDHndl.curSadBuf].ImageHeight / ALG_MOTION_DETECT_VERT_REGIONS);
    gMDHndl.prevSadBuf = gMDHndl.curSadBuf;
    gMDHndl.chid = chid;

    /* Send cmd to the Motion Detection task */
    Utils_tskSendCmd(&gMDHndl.tskHndl, SYSTEM_CMD_NEW_DATA);

    return 0;
}

/* ===================================================================
 *  @func     MD_tskMain
 *
 *  @desc     Motion Detection task
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
Void MD_tskMain(struct Utils_TskHndl *pTsk, Utils_MsgHndl * pMsg)
{
    UInt32 cmd = Utils_msgGetCmd(pMsg);
    Bool ackMsg, done;
    Int32 status, mdStatus;
    MD_RunStatus_t mdRunStatus;
    //SwosdLink_GuiParams swOsdGuiPrm;

    if (cmd != SYSTEM_CMD_CREATE) {
        Utils_tskAckOrFreeMsg(pMsg, FVID2_EFAIL);
        return;
    }

    /* Allocate buffers to hold the SAD data */
    gMDHndl.curSadBuf = 0;
    gMDHndl.prevSadBuf = -1;
    gMDHndl.sadBuf[0] = (UInt32) Utils_memAlloc(ENC_LINK_MV_DATA_SIZE, 32);
    gMDHndl.sadBuf[1] = (UInt32) Utils_memAlloc(ENC_LINK_MV_DATA_SIZE, 32);
    Utils_tskAckOrFreeMsg(pMsg, FVID2_SOK);
    done = FALSE;
    ackMsg = FALSE;

    while (!done)
    {
        status = Utils_tskRecvMsg(pTsk, &pMsg, BIOS_WAIT_FOREVER);
        if (status != FVID2_SOK)
            break;

        cmd = Utils_msgGetCmd(pMsg);
        switch (cmd)
        {
            case SYSTEM_CMD_NEW_DATA:
                /* ack or free msg */
                Utils_tskAckOrFreeMsg(pMsg, status);

                if(!gMDHndl.chPrm[gMDHndl.chid].detectCnt)
                {
	                /* apply Motion Detection */
	                mdStatus = DM81XX_MD_Run(&gMDHndl, &gMDHndl.mdRunPrm[gMDHndl.curSadBuf], &mdRunStatus);

	                if (mdStatus == ALG_MOTION_S_DETECT)
	                {
	                    /* Notify A8 about the Motion Detected */
	                    System_linkControl(SYSTEM_LINK_ID_HOST, VSYS_EVENT_MOTION_DETECT, &gMDHndl.chid, sizeof(gMDHndl.chid), TRUE);

						#if 0	//# display osd motion
						swOsdGuiPrm.streamId = gMDHndl.chid;
						System_linkControl(SYSTEM_LINK_ID_SWOSD_0, SWOSDLINK_CMD_MOTION, &swOsdGuiPrm, sizeof(swOsdGuiPrm), TRUE);
						#endif

						gMDHndl.chPrm[gMDHndl.chid].detectCnt=15;  //# skip 1sec on 15fps
	                }
            	}
            	else
            	{
            		gMDHndl.chPrm[gMDHndl.chid].detectCnt--;
            	}

                /* Toggle the cur sad buffer */
                if (gMDHndl.curSadBuf == 0)
                    gMDHndl.curSadBuf = 1;
                else
                    gMDHndl.curSadBuf = 0;
                break;

            case SYSTEM_CMD_DELETE:
                done = TRUE;
                ackMsg = TRUE;
                break;

            default:
                Utils_tskAckOrFreeMsg(pMsg, status);
                break;
        }
    }

    if (ackMsg && pMsg != NULL) {
        Utils_tskAckOrFreeMsg(pMsg, status);
    }

    /* Free the SAD buffers */
    Utils_memFree((Ptr)gMDHndl.sadBuf[0], ENC_LINK_MV_DATA_SIZE);
    Utils_memFree((Ptr)gMDHndl.sadBuf[1], ENC_LINK_MV_DATA_SIZE);

	#ifdef SYS_DEBUG
    Vps_printf(" Motion Detect Task Exit Done\n");
	#endif
}

/* ===================================================================
 *  @func     DM81XX_MD_Create
 *
 *  @desc     DM81XX Motion Detection Algorithm create
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
int DM81XX_MD_Create(void)
{
    Int32 status, i;

	gMDHndl.frameCnt = 0;
	gMDHndl.startCnt = ALG_MOTION_DETECT_MAX_CH*50;
	gMDHndl.motioncenable = 0;
	gMDHndl.motioncvalue = 0;

	for(i=0; i<ALG_MOTION_DETECT_MAX_CH; i++)
	{
		gMDHndl.chPrm[i].motionenable = 0;
		gMDHndl.chPrm[i].motionlevel = ALG_MOTION_DETECT_SENSITIVITY_LOW;
		gMDHndl.chPrm[i].detectCnt = 0;
		gMDHndl.chPrm[i].blockNum = 0;
		gMDHndl.chPrm[i].update = 1;
	}

    /* Create Motion Detection Task */
    status = Utils_tskCreate(&gMDHndl.tskHndl, MD_tskMain, MD_TSK_PRI,
                        gMD_tskStack, MD_TSK_STACK_SIZE, NULL, "MD_TASK");
    UTILS_assert(status == FVID2_SOK);

    /* Send SYSTEM_CMD_CREATE message to the MD task */
    Utils_tskSendCmd(&gMDHndl.tskHndl, SYSTEM_CMD_CREATE);

    return 0;
}

/* ===================================================================
 *  @func     DM81XX_MD_Delete
 *
 *  @desc     DM81XX Motion Detection Algorithm delete
 *
 *  @modif    This function modifies the following structures
 *
 *  @inputs   This function takes the following inputs
 *            <argument name>
 *            Description of usage
 *            <argument name>
 *            Description of usage
 *
 *  @outputs  <argument name>
 *            Description of usage
 *
 *  @return   Return value of this function if any
 *  ==================================================================
 */
int DM81XX_MD_Delete()
{
    /* Send SYSTEM_CMD_DELETE message to the MD task */
    Utils_tskSendCmd(&gMDHndl.tskHndl, SYSTEM_CMD_DELETE);

    /* Delete Motion Detection task */
    Utils_tskDelete(&gMDHndl.tskHndl);

    return 0;
}
