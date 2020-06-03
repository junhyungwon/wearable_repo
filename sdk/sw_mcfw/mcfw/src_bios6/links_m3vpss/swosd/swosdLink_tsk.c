/** ==================================================================
 *  @file   swosdLink_tsk.c
 *
 *  @path    ipnc_mcfw/mcfw/src_bios6/links_m3vpss/swosd/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

#include "swosdLink_priv.h"

#pragma DATA_ALIGN(gSwosdLink_tskStack, 32)
#pragma DATA_SECTION(gSwosdLink_tskStack, ".bss:taskStackSection")
UInt8 gSwosdLink_tskStack[SWOSD_LINK_OBJ_MAX][SWOSD_LINK_TSK_STACK_SIZE];

#define MAX_OSD_CH	1//2

/* Swosd link get date & time */
//#define YEAR0		1900
#define EPOCH_YR	1970
#define SECS_DAY	(24*60*60)
#define LEAPYEAR(year)   	(!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)   	(LEAPYEAR(year) ? 366 : 365)

const int _ytab[2][12] = {
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

SwosdLink_Obj gSwosdLink_obj[SWOSD_LINK_OBJ_MAX];

extern Int32 isTiled[];
extern Int32 lineOffsets[];

static int swosd_max_ch = MAX_OSD_CH;

/*----------------------------------------------------------------------------
 Swosd date/time set
-----------------------------------------------------------------------------*/
static void SwosdLink_gmtime(UInt32 curtime, DATE_TIME_INFO *tm)
{
	UInt32 dayclock, dayno;
	int year, mon;

	dayclock = curtime % SECS_DAY;
	dayno = curtime / SECS_DAY;

	tm->sec = dayclock%60;
	tm->min = (dayclock%3600)/60;
	tm->hours = dayclock / 3600;

	year = EPOCH_YR;
	while (dayno >= YEARSIZE(year)) {
		dayno -= YEARSIZE(year);
		year++;
	}
	tm->year = year;// - YEAR0;
	mon = 0;
	while (dayno >= _ytab[LEAPYEAR(year)][mon]) {
		dayno -= _ytab[LEAPYEAR(year)][mon];
		mon++;
	}
	tm->month = mon+1;
	tm->mday = dayno + 1;
}

/* Swosd link post Frame Sync sem */
Int32 SwosdLink_postFrameSyncSem(UInt32 linkId)
{
    UInt32 instId = (linkId - SYSTEM_LINK_ID_SWOSD_0);

    SwosdLink_Obj *pObj;

    pObj = &gSwosdLink_obj[instId];
    Semaphore_post(pObj->frameSync);

    return FVID2_SOK;
}

/*----------------------------------------------------------------------------
 Swosd link create
-----------------------------------------------------------------------------*/
Int32 SwosdLink_create(SwosdLink_Obj * pObj, SwosdLink_CreateParams * pPrm)
{
    Int32 status;
    UInt32 strId;
    UInt32 chId, winId;
    SWOSD_config_t swOsdPrm;
    System_LinkChInfo *pChInfo;
    System_LinkQueInfo *pOutQueInfo;
    System_LinkChInfo *pInChInfo, *pOutChInfo;
    Semaphore_Params semParams;

    memcpy(&pObj->createArgs, pPrm, sizeof(pObj->createArgs));

    /* Get the link info from prev link */
    status = System_linkGetInfo(pPrm->inQueParams.prevLinkId, &pObj->inTskInfo);

    UTILS_assert(status == FVID2_SOK);
    UTILS_assert(pPrm->inQueParams.prevLinkQueId < pObj->inTskInfo.numQue);

    /* copy the previous link info */
    memcpy(&pObj->inQueInfo,
           &pObj->inTskInfo.queInfo[pPrm->inQueParams.prevLinkQueId],
           sizeof(pObj->inQueInfo));

    /* Create the output buffers */
    status = Utils_bufCreate(&pObj->outFrameQue,           // pHndl
                             FALSE,                        // blockOnGet
                             FALSE);                       // blockOnPut

	UTILS_assert(status == FVID2_SOK);

    /* SWOSD alg create */
    memset(&swOsdPrm, 0x00, sizeof(SWOSD_config_t));

    /* NOTE:Assuming numCh is same as no of streams */
    swosd_max_ch = pObj->inQueInfo.numCh;
	if(swosd_max_ch > MAX_OSD_CH) {
		swosd_max_ch = MAX_OSD_CH;
	}
	swOsdPrm.numCaptureStream = swosd_max_ch;
    swOsdPrm.autoFocusVal = 0;

    if ((pObj->inQueInfo.chInfo[0].dataFormat == FVID2_DF_YUV420SP_VU) ||
        (pObj->inQueInfo.chInfo[0].dataFormat == FVID2_DF_YUV420SP_UV))
    {
        swOsdPrm.captureYuvFormat = SWOSD_FORMATYUV420p;
    }
    else
    {
        swOsdPrm.captureYuvFormat = SWOSD_FORMATYUV422i;
    }

    for (strId = 0; strId < swOsdPrm.numCaptureStream; strId++)
    {
		pChInfo = &pObj->inQueInfo.chInfo[strId];

		swOsdPrm.swosdConfig[strId].swosdEnable = 1;
		swOsdPrm.swosdConfig[strId].capLineOffset = pChInfo->pitch[0];
		swOsdPrm.swosdConfig[strId].capWidth = pChInfo->width;
		swOsdPrm.swosdConfig[strId].capHeight = pChInfo->height;
		swOsdPrm.swosdConfig[strId].cropWidth = pChInfo->width;
		swOsdPrm.swosdConfig[strId].cropHeight = pChInfo->height;

		swOsdPrm.swosdConfig[strId].swosdType = SWOSD_BASIC;
		swOsdPrm.swosdConfig[strId].swosdTimeFormat = SWOSD_FMT_24HR;
		swOsdPrm.swosdConfig[strId].swosdDateFormat = SWOSD_FMT_YMD;

		for(winId=0; winId<MAX_OSDWIN; winId++)
			swOsdPrm.swosdConfig[strId].en_win[winId] = OSD_NO;

		swOsdPrm.swosdConfig[strId].update_datetime = OSD_NO;
		swOsdPrm.swosdConfig[strId].update_volt = OSD_NO;
		swOsdPrm.swosdConfig[strId].update_temp = OSD_NO;
		swOsdPrm.swosdConfig[strId].update_speed = OSD_NO;
		swOsdPrm.swosdConfig[strId].update_gsens = OSD_NO;
		swOsdPrm.swosdConfig[strId].update_userstr = OSD_NO;

		swOsdPrm.swosdConfig[strId].evt_cnt = 0;
		swOsdPrm.swosdConfig[strId].mot_cnt = 0;

		isTiled[strId] = FALSE;
		if(pChInfo->memType==SYSTEM_MT_TILEDMEM)
		    isTiled[strId] = TRUE;

		swOsdPrm.swosdConfig[strId].memType = isTiled[strId];
		lineOffsets[strId] = pChInfo->pitch[0];

		status = SWOSD_imxCreate(strId, isTiled[strId], lineOffsets[strId]);
		UTILS_assert(status == FVID2_SOK);
    }

	//# defaut osd win enable
	for (strId=0; strId<swosd_max_ch; strId++) {
		swOsdPrm.swosdConfig[strId].en_win[0] = OSD_YES;		//# datetime enable
	}

    status = DM81XX_SWOSD_create(&swOsdPrm);
    UTILS_assert(status == SWOSD_SOK);

    /* Populate the link's info structure */
    pObj->info.numQue = 1;

    pOutQueInfo = &pObj->info.queInfo[0];
    pObj->info.queInfo[0].numCh = pObj->inQueInfo.numCh;

    for (chId = 0; chId < pObj->info.queInfo[0].numCh; chId++)
    {
        pInChInfo = &pObj->inQueInfo.chInfo[chId];
        pOutChInfo = &pOutQueInfo->chInfo[chId];

        memcpy(pOutChInfo, pInChInfo, sizeof(System_LinkChInfo));
    }
    pObj->ts32 = 0;

    /* create semaphore for frame sync */
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    pObj->frameSync = Semaphore_create(0u, &semParams, NULL);

    UTILS_assert(pObj->frameSync != NULL);

#ifdef SYSTEM_DEBUG_SWOSD
    Vps_printf(" %d: SWOSD   : Create Done !!!\n", Clock_getTicks());
#endif

    return FVID2_SOK;
}

/*----------------------------------------------------------------------------
 Swosd process frames
-----------------------------------------------------------------------------*/
Int32 SwosdLink_processFrames(SwosdLink_Obj * pObj)
{
    Int32 status = FVID2_SOK;
    UInt32 frameId;
	Int32 Width = 0,Height = 0,Pitch =0;
    FVID2_FrameList frameList;
    FVID2_Frame *pFullFrame;
    System_LinkInQueParams *pInQueParams;
    System_LinkQueInfo *pInQueInfo;
    System_LinkChInfo *pChInfo;
	System_FrameInfo *pInFrameInfo;
    Bool rtParamUpdatePerFrame = FALSE;
	DATE_TIME_INFO iDateTime;
	UInt_64 walltime;
	UInt32 curtime, i;

    pInQueParams = &pObj->createArgs.inQueParams;
    pInQueInfo = &pObj->inTskInfo.queInfo[pInQueParams->prevLinkQueId];

    System_getLinksFullFrames(pInQueParams->prevLinkId,
                              pInQueParams->prevLinkQueId, &frameList);

    if (frameList.numFrames)
    {
        /* Apply SWOSD on the full frames */
        for (frameId = 0; frameId < frameList.numFrames; frameId++)
        {
            pFullFrame = frameList.frames[frameId];

            if ((pObj->createArgs.frameSync == 1) &&
                (pFullFrame->channelNum == pObj->createArgs.frameSycChId))
            {
                Semaphore_pend(pObj->frameSync, BIOS_WAIT_FOREVER);
            }

            pChInfo 	 = &pInQueInfo->chInfo[pFullFrame->channelNum];
			Width        = pChInfo->width;
			Height       = pChInfo->height;
			Pitch        = pChInfo->pitch[0];
			pInFrameInfo = (System_FrameInfo *) pFullFrame->appData;

			if ((pInFrameInfo != NULL) && (pInFrameInfo->rtChInfoUpdate == TRUE))
            {
				if (pInFrameInfo->rtChInfo.height != Height)
                {
                    Height = pInFrameInfo->rtChInfo.height;
					rtParamUpdatePerFrame = TRUE;
                }

                if (pInFrameInfo->rtChInfo.width != Width)
                {
                    Width = pInFrameInfo->rtChInfo.width;
					rtParamUpdatePerFrame = TRUE;
                }

                if (pInFrameInfo->rtChInfo.pitch[0] != Pitch)
                {
                    Pitch = pInFrameInfo->rtChInfo.pitch[0];
					rtParamUpdatePerFrame = TRUE;
                }
				if(rtParamUpdatePerFrame == TRUE){
					DM81XX_SWOSD_Reset(pFullFrame->channelNum,Pitch,Width,Height);
				}
            }

			//# wait iss dma copy done
			//Task_sleep(1);

			Utils_get_walltime(&walltime);
			curtime = (UInt32)(walltime/1000);
			if(pObj->ts32 != curtime)
			{
				//# update date & time
				pObj->ts32 = curtime;
				SwosdLink_gmtime(curtime, &iDateTime);
				for (i=0; i<swosd_max_ch/*pObj->inQueInfo.numCh*/; i++)
				{
                	DM81XX_SWOSD_setDateTime(i, iDateTime);
				}
			}

            DM81XX_SWOSD_apply(pFullFrame->channelNum,     // stream Id
                               (Int32) pFullFrame->addr[0][0],  // Y plane address
                               (Int32) pFullFrame->addr[0][1]); // UV plane address

            /* Put the full buffer into full queue */
            status = Utils_bufPutFullFrame(&pObj->outFrameQue, pFullFrame);

            UTILS_assert(status == FVID2_SOK);
        }

        /* send SYSTEM_CMD_NEW_DATA to next link */
        System_sendLinkCmd(pObj->createArgs.outQueParams.nextLink,
                           SYSTEM_CMD_NEW_DATA);
    }

    return FVID2_SOK;
}

/* Swosd link drv delete */
Int32 SwosdLink_delete(SwosdLink_Obj * pObj)
{
    Int32 status, i;

    /* Delete the buffers */
    status = Utils_bufDelete(&pObj->outFrameQue);

    UTILS_assert(status == FVID2_SOK);

    /* SW OSD Alg Delete */
    DM81XX_SWOSD_delete();

	for(i = 0; i < swosd_max_ch/*pObj->inQueInfo.numCh*/; i++)
	{
		SWOSD_imxDelete(i);
	}

    /* Delete Frame Sync semaphore */
    Semaphore_delete(&pObj->frameSync);

#ifdef SYSTEM_DEBUG_SWOSD
    Vps_printf(" %d: SWOSD   : Delete Done !!!\n", Clock_getTicks());
#endif

    return FVID2_SOK;
}

/*----------------------------------------------------------------------------
 Swosd link task main function
-----------------------------------------------------------------------------*/
void SwosdLink_tskMain(struct Utils_TskHndl * pTsk, Utils_MsgHndl * pMsg)
{
    UInt32 cmd = Utils_msgGetCmd(pMsg);
    Bool ackMsg, done;
    Int32 status;
    SwosdLink_Obj *pObj = (SwosdLink_Obj *) pTsk->appData;
    SwosdLink_GuiParams *pSwOsdGuiPrm;

    if (cmd != SYSTEM_CMD_CREATE)
    {
        Utils_tskAckOrFreeMsg(pMsg, FVID2_EFAIL);
        return;
    }

    /* create the swosd driver */
    status = SwosdLink_create(pObj, Utils_msgGetPrm(pMsg));

    Utils_tskAckOrFreeMsg(pMsg, status);

    if (status != FVID2_SOK)
        return;

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
            case SYSTEM_CMD_DELETE:
                done = TRUE;
                ackMsg = TRUE;
                break;

            case SYSTEM_CMD_NEW_DATA:
                Utils_tskAckOrFreeMsg(pMsg, status);
                SwosdLink_processFrames(pObj);
                break;

			case SWOSDLINK_CMD_ENABLE:
				pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setSwOsdEnable(pSwOsdGuiPrm->streamId, pSwOsdGuiPrm->enable);
				Utils_tskAckOrFreeMsg(pMsg, status);
				break;

			case SWOSDLINK_CMD_DATETIME:
				//# do nothing
				break;

			case SWOSDLINK_CMD_EN_DATETIME:
				pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setWindowEnable(pSwOsdGuiPrm->streamId, OSDWIN_0, pSwOsdGuiPrm->enable);
				Utils_tskAckOrFreeMsg(pMsg, status);
				break;

			case SWOSDLINK_CMD_GPS:
				pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setWindowEnable(pSwOsdGuiPrm->streamId, OSDWIN_1, pSwOsdGuiPrm->enable);
				Utils_tskAckOrFreeMsg(pMsg, status);
				break;

			case SWOSDLINK_CMD_PARK:
				pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setWindowEnable(pSwOsdGuiPrm->streamId, OSDWIN_2, pSwOsdGuiPrm->enable);
				Utils_tskAckOrFreeMsg(pMsg, status);
				break;

			case SWOSDLINK_CMD_EVENT:
				pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setWindowEnable(pSwOsdGuiPrm->streamId, OSDWIN_3, pSwOsdGuiPrm->enable);
				Utils_tskAckOrFreeMsg(pMsg, status);
				break;

			case SWOSDLINK_CMD_MOTION:
				pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setWindowEnable(pSwOsdGuiPrm->streamId, OSDWIN_4, pSwOsdGuiPrm->enable);
				Utils_tskAckOrFreeMsg(pMsg, status);
				break;

			case SWOSDLINK_CMD_VOLTAGE:
            	pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setWindowEnable(pSwOsdGuiPrm->streamId, OSDWIN_5, pSwOsdGuiPrm->enable);
				if(pSwOsdGuiPrm->enable)
            		DM81XX_SWOSD_setVolt(pSwOsdGuiPrm->streamId, pSwOsdGuiPrm->volt);
            	Utils_tskAckOrFreeMsg(pMsg, status);
				break;

			case SWOSDLINK_CMD_TEMPERATURE:
            	pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setWindowEnable(pSwOsdGuiPrm->streamId, OSDWIN_6, pSwOsdGuiPrm->enable);
				if(pSwOsdGuiPrm->enable)
            		DM81XX_SWOSD_setTemp(pSwOsdGuiPrm->streamId, pSwOsdGuiPrm->temp);
            	Utils_tskAckOrFreeMsg(pMsg, status);
				break;

            case SWOSDLINK_CMD_SPEED:
            	pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setWindowEnable(pSwOsdGuiPrm->streamId, OSDWIN_7, pSwOsdGuiPrm->enable);
				if(pSwOsdGuiPrm->enable)
					DM81XX_SWOSD_setSpeed(pSwOsdGuiPrm->streamId, pSwOsdGuiPrm->speed);
            	Utils_tskAckOrFreeMsg(pMsg, status);
				break;

			case SWOSDLINK_CMD_GSENS:
            	pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setWindowEnable(pSwOsdGuiPrm->streamId, OSDWIN_8, pSwOsdGuiPrm->enable);
				if(pSwOsdGuiPrm->enable)
					DM81XX_SWOSD_setGsens(pSwOsdGuiPrm->streamId,
							pSwOsdGuiPrm->x, pSwOsdGuiPrm->y, pSwOsdGuiPrm->z);
            	Utils_tskAckOrFreeMsg(pMsg, status);
				break;

			case SWOSDLINK_CMD_USERSTR:
                pSwOsdGuiPrm = (SwosdLink_GuiParams *) Utils_msgGetPrm(pMsg);

				DM81XX_SWOSD_setWindowEnable(pSwOsdGuiPrm->streamId, OSDWIN_9, pSwOsdGuiPrm->enable);
				if(pSwOsdGuiPrm->enable)
					DM81XX_SWOSD_setUserString(pSwOsdGuiPrm->streamId, (char *)pSwOsdGuiPrm->usrString);
				Utils_tskAckOrFreeMsg(pMsg, status);
                break;

            default:
                Utils_tskAckOrFreeMsg(pMsg, status);
                break;
        }
    }

    /* Delete the swosd driver */
    SwosdLink_delete(pObj);

    if (ackMsg && pMsg != NULL)
    {
        Utils_tskAckOrFreeMsg(pMsg, status);
    }

    return;
}

/* Swosd link get info */
Int32 SwosdLink_getInfo(Utils_TskHndl * pTsk, System_LinkInfo * info)
{
    SwosdLink_Obj *pObj = (SwosdLink_Obj *) pTsk->appData;

    memcpy(info, &pObj->info, sizeof(*info));

    return FVID2_SOK;
}

/* Swosd get full frames */
Int32 SwosdLink_getFullFrames(Utils_TskHndl * pTsk, UInt16 queId,
                              FVID2_FrameList * pFrameList)
{
    Int32 status;

    SwosdLink_Obj *pObj = (SwosdLink_Obj *) pTsk->appData;

    status = Utils_bufGetFull(&pObj->outFrameQue, pFrameList, BIOS_NO_WAIT);

    UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

/* Swosd put empty frames */
Int32 SwosdLink_putEmptyFrames(Utils_TskHndl * pTsk, UInt16 queId,
                               FVID2_FrameList * pFrameList)
{
    System_LinkInQueParams *pInQueParams;

    SwosdLink_Obj *pObj = (SwosdLink_Obj *) pTsk->appData;

    pInQueParams = &pObj->createArgs.inQueParams;

    /* Put the empty buffers back to previous link */
    System_putLinksEmptyFrames(pInQueParams->prevLinkId,
                               pInQueParams->prevLinkQueId, pFrameList);

    return FVID2_SOK;
}

/* Swosd Link init */
Int32 SwosdLink_init()
{
    Int32 status;

    System_LinkObj linkObj;

    UInt32 swosdId;

    SwosdLink_Obj *pObj;

    char tskName[32];

    for (swosdId = 0; swosdId < SWOSD_LINK_OBJ_MAX; swosdId++)
    {
        pObj = &gSwosdLink_obj[swosdId];

        memset(pObj, 0, sizeof(*pObj));

        linkObj.pTsk = &pObj->tsk;
        linkObj.linkGetFullFrames = SwosdLink_getFullFrames;
        linkObj.linkPutEmptyFrames = SwosdLink_putEmptyFrames;
        linkObj.getLinkInfo = SwosdLink_getInfo;

        System_registerLink(SYSTEM_LINK_ID_SWOSD_0 + swosdId, &linkObj);

        sprintf(tskName, "SWOSD%d", swosdId);

        status = Utils_tskCreate(&pObj->tsk,
                                 SwosdLink_tskMain,
                                 SWOSD_LINK_TSK_PRI,
                                 gSwosdLink_tskStack[swosdId],
                                 SWOSD_LINK_TSK_STACK_SIZE, pObj, tskName);

        UTILS_assert(status == FVID2_SOK);
    }

    return status;
}

/* Swosd link de-init */
Int32 SwosdLink_deInit()
{
    UInt32 swosdId;

    for (swosdId = 0; swosdId < SWOSD_LINK_OBJ_MAX; swosdId++)
    {
        Utils_tskDelete(&gSwosdLink_obj[swosdId].tsk);
    }

    return FVID2_SOK;
}
