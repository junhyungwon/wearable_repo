/*==============================================================================
 * @file:       ti_vsys.c
 *
 * @brief:      System mcfw function definition.
 *
 * @vers:       0.5.0.0 2011-06
 *
 *==============================================================================
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include "ti_vsys_priv.h"
#include <cmem.h>
#include <sys/time.h>

/* =============================================================================
 * Globals
 * =============================================================================
 */

VSYS_MODULE_CONTEXT_S gVsysModuleContext;

VSYS_TIMER_OBJ gTimerThObj;

#define DT_WAIT_TIME	500			//# 500 msec
#define DT_CAPT_SYNC	(60 * 120)	//# 60 min (DT_WAIT_TIME*value)

/* =============================================================================
 * Vsys module APIs
 * =============================================================================
 */
/**
 * \brief:
 *      Initialize parameters to be passed to init
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vsys_params_init(VSYS_PARAMS_S * pContext)
{
    memset(pContext, 0, sizeof(VSYS_PARAMS_S));
    pContext->enableEncode  = FALSE;
    pContext->enableDecode  = FALSE;
    pContext->enableMjpeg  = FALSE;
    pContext->enableLcd  = FALSE;
    pContext->systemUseCase = VSYS_USECASE_MAX;

    return 0;
}

static Void *Vsys_dateTimeTaskMain(Void * prm)
{
    time_t now;
    struct tm ts;
    SwosdLink_dateTime dateTime;

    struct timeval tv;
	UInt_64 cur_time;
	int scnt=(DT_CAPT_SYNC+1);

	tzset();

    while (gTimerThObj.exitTimerTh == FALSE)
    {
    	gettimeofday(&tv, NULL);

		if(scnt > DT_CAPT_SYNC)
		{
			cur_time = (((UInt_64)tv.tv_sec*1000000 + tv.tv_usec)/1000);

			System_linkControl(SYSTEM_LINK_ID_M3VPSS,
						SYSTEM_M3VPSS_CMD_CMD_SYNC_DATETIME,
						&cur_time, sizeof(cur_time),
						TRUE);
			scnt = 0;
		}
		else {
			scnt++;
		}

    	if (gVsysModuleContext.swOsdId != SYSTEM_LINK_ID_INVALID)
    	{
			now = tv.tv_sec;
			localtime_r(&now, &ts);

			dateTime.year = 1900 + ts.tm_year;
			dateTime.month = 1 + ts.tm_mon;
			dateTime.day = ts.tm_mday;
			dateTime.hour = ts.tm_hour;
			dateTime.min = ts.tm_min;
			dateTime.sec = ts.tm_sec;

			System_linkControl(gVsysModuleContext.swOsdId, // dest link id
							   SWOSDLINK_CMD_DATETIME,     // cmd
							   &dateTime,                  // prm
							   sizeof(SwosdLink_dateTime), // prm size
							   TRUE);                      // wait for ack
		}

        /* sleep for 0.5 sec */
		OSA_waitMsecs(DT_WAIT_TIME);
    }

    gTimerThObj.exitTimerDone = TRUE;

    return NULL;
}

Int32 Vsys_datetimeCreate(void)
{
    gTimerThObj.startCnt = 0;
    gTimerThObj.exitTimerTh = FALSE;
    gTimerThObj.exitTimerDone = FALSE;

    OSA_thrCreate(&gTimerThObj.hndl,                       // hndl
                  Vsys_dateTimeTaskMain,                   // entryFunc
                  VSYS_TIMER_TSK_PRI,                      // pri
                  VSYS_TIMER_TSK_STACK_SIZE,               // stackSize
                  NULL,									   // prm	
				  NULL);                                   // thread name

    return 0;
}

Int32 Vsys_datetimeDelete()
{
	OSA_printf("%s start\n", __func__);

    gTimerThObj.exitTimerTh = TRUE;

    while (gTimerThObj.exitTimerDone == FALSE)
    {
        OSA_waitMsecs(DT_WAIT_TIME);
    }

    OSA_thrDelete(&gTimerThObj.hndl);

	OSA_printf("Date time task deleted\n");

    return 0;
}

Int32 Vsys_datetime_init(void)
{
	UInt_64 cur_time;
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz); // maybe localtime 

	printf("tz.tz_minuteswest = %d\n", tz.tz_minuteswest);/* minutes west of Greenwich */
	printf("tz.tz_dsttime     = %d\n", tz.tz_dsttime);    /* type of DST correction */

    struct tm *pgm = gmtime(&tv.tv_sec);
    time_t gm_sec  = mktime(pgm);

    printf("%s, timezone = %ld, is_dst:%d, tm_zone:%s, gmtoff:%ld\n", __func__, (tv.tv_sec-gm_sec)/3600, pgm->tm_isdst, pgm->tm_zone, pgm->tm_gmtoff);
    unsigned int cur_sec = tv.tv_sec + (tv.tv_sec-gm_sec);

	cur_time = (((UInt_64)cur_sec*1000000 + tv.tv_usec)/1000);

	System_linkControl(SYSTEM_LINK_ID_M3VPSS,
				SYSTEM_M3VPSS_CMD_CMD_SYNC_DATETIME,
				&cur_time, sizeof(cur_time),
				TRUE);

	return 0;
}

Int32 Vsys_datetime_init2(int isdst)
{
	UInt_64 cur_time;
	struct timeval tv;
	struct timezone tz;
	struct tm *pgm;
	time_t gm_sec;
	unsigned int cur_sec;

	gettimeofday(&tv, &tz); // maybe localtime 

    pgm     = gmtime(&tv.tv_sec);
    gm_sec  = mktime(pgm);
    cur_sec = tv.tv_sec;

	if(isdst)
		cur_sec += pgm->tm_gmtoff;
	else
		cur_sec += tv.tv_sec-gm_sec;

	cur_time = (((UInt_64)cur_sec*1000000 + tv.tv_usec)/1000);

	System_linkControl(SYSTEM_LINK_ID_M3VPSS,
				SYSTEM_M3VPSS_CMD_CMD_SYNC_DATETIME,
				&cur_time, sizeof(cur_time),
				TRUE);

	return 0;
}

/**
 * \brief:
 *      Initialize Vsys instance
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vsys_init(VSYS_PARAMS_S * pContext)
{
	gVsysModuleContext.swOsdId 	= SYSTEM_LINK_ID_INVALID;

    if(pContext==NULL)
    {
        Vsys_params_init(&gVsysModuleContext.vsysConfig);
    }
    else
    {
        memcpy(&gVsysModuleContext.vsysConfig, pContext, sizeof(VSYS_PARAMS_S));
    }

    #if 0
    Vcap_init(NULL);
    Vcam_init(NULL);
    Vdis_init(NULL);

    Vdec_init(NULL);
    Venc_init(NULL);
    #endif

    return 0;
}

/**
 * \brief:
 *      Finalize Vsys instance
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vsys_exit()
{
    return 0;
}

/**
 * \brief:
 *      Creates instances of links and prepares chains
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
#if defined(LF_SYS_NEXXONE_VOIP) || defined(LF_SYS_NEXXB_ONE)
Int32 Vsys_create(void)
#else 
Int32 Vsys_create(int mode)
#endif
{
    switch (gVsysModuleContext.vsysConfig.systemUseCase)
    {
		case VSYS_USECASE_CAPTURE:
			#if defined(LF_SYS_NEXXONE_VOIP) || defined(LF_SYS_NEXXB_ONE)
			mcfw_capture_display_init();
			#else
			mcfw_capture_display_init(mode);
			#endif
			break;
		case VSYS_USECASE_DECODE:
			break;
		case VSYS_USECASE_MAX:
		default:
			return -1;
    }
    return 0;
}

/**
 * \brief:
 *      Delete instances of links
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vsys_delete()
{
    switch (gVsysModuleContext.vsysConfig.systemUseCase)
    {
		case VSYS_USECASE_CAPTURE:
			mcfw_capture_display_exit();
			break;
		case VSYS_USECASE_DECODE:
			break;
		case VSYS_USECASE_MAX:
		default:
			break;
    }
    return 0;
}

#if 0	//# ubx not used function
Int32 Vsys_allocBuf(UInt32 srRegId, UInt32 bufSize, UInt32 bufAlign, Vsys_AllocBufInfo *bufInfo)
{
#if 0
    IHeap_Handle heapHndl;

    heapHndl = SharedRegion_getHeap(srRegId);
    OSA_assert(heapHndl != NULL);

    bufInfo->virtAddr = NULL;
    bufInfo->physAddr = NULL;
    bufInfo->srPtr    = 0;

    bufInfo->virtAddr = Memory_alloc(heapHndl, bufSize, bufAlign, NULL);

    if(bufInfo->virtAddr==NULL)
        return -1;

    bufInfo->physAddr = Memory_translate (bufInfo->virtAddr, Memory_XltFlags_Virt2Phys);

    if(bufInfo->physAddr==NULL)
        return -1;

    bufInfo->srPtr = SharedRegion_getSRPtr(bufInfo->virtAddr,srRegId);
#else
    CMEM_AllocParams  prm;

    prm.type      = CMEM_HEAP;
    prm.flags     = CMEM_NONCACHED;
    prm.alignment = bufAlign;

    bufInfo->virtAddr = (Uint8*)CMEM_alloc(bufSize,&prm);
    bufInfo->physAddr = (Uint8*)CMEM_getPhys(bufInfo->virtAddr);

    OSA_printf("%s - addr = 0x%x,size = %d\n",__func__,(unsigned int)bufInfo->virtAddr,bufSize);
#endif

    return 0;
}

Int32 Vsys_freeBuf(UInt32 srRegId, UInt8 *virtAddr, UInt32 bufSize)
{
#if 0
    IHeap_Handle heapHndl;

    heapHndl = SharedRegion_getHeap(srRegId);
    OSA_assert(heapHndl != NULL);

    OSA_assert(virtAddr != NULL);

    Memory_free(heapHndl, virtAddr, bufSize);
#else
    CMEM_AllocParams  prm;

    prm.type      = CMEM_HEAP;
    prm.flags     = 0;
    prm.alignment = 0;

    CMEM_free(virtAddr,&prm);

    OSA_printf("%s - addr = 0x%x\n",__func__,(unsigned int)virtAddr);
#endif

    return 0;
}
#endif

VSYS_USECASES_E Vsys_getSystemUseCase()
{
    return gVsysModuleContext.vsysConfig.systemUseCase;
}

Int32 Vsys_setSwOsdPrm(VSYS_SWOSD_SETPARAM swOsdSetPrm, Vsys_swOsdPrm *pSwOsdPrm)
{
	SwosdLink_GuiParams *swOsdGuiPrm = (SwosdLink_GuiParams *)pSwOsdPrm;

	if (gVsysModuleContext.swOsdId == SYSTEM_LINK_ID_INVALID)
		return -1;

	switch (swOsdSetPrm)
	{
		case VSYS_SWOSD_ENABLE:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_ENABLE,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;
		case VSYS_SWOSD_EN_DATETIME:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_EN_DATETIME,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;
		case VSYS_SWOSD_GPS:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_GPS,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;
		case VSYS_SWOSD_PARK:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_PARK,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;
		case VSYS_SWOSD_EVENT:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_EVENT,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;
		case VSYS_SWOSD_MOTION:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_MOTION,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;
		case VSYS_SWOSD_CALLSTAT:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_CALLSTAT,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;	
		case VSYS_SWOSD_VOLTAGE:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_VOLTAGE,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;
		case VSYS_SWOSD_SPEED:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_SPEED,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;
		case VSYS_SWOSD_GSENS:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_GSENS,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;
		case VSYS_SWOSD_USERSTR:
			System_linkControl(gVsysModuleContext.swOsdId,
						SWOSDLINK_CMD_USERSTR,
						swOsdGuiPrm, sizeof(SwosdLink_GuiParams), TRUE);
			break;
		default:
			break;
	}

	return 0;
}

Int32 Vsys_printDetailedStatistics()
{
    UInt32 devId;

    if(gVcamModuleContext.cameraId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVcamModuleContext.cameraId,
                            CAMERA_LINK_CMD_PRINT_ADV_STATISTICS,
                            NULL, 0, TRUE);
    }
    if(gVsysModuleContext.swOsdId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVsysModuleContext.swOsdId,
                            SWOSD_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
    }

    OSA_waitMsecs(2000); // allow for print to complete

    for(devId=0; devId<VDIS_DEV_MAX; devId++)
    {
        if(gVdisModuleContext.swMsId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkControl(gVdisModuleContext.swMsId[devId],
                            SYSTEM_SW_MS_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
        }
    }

    for(devId=0; devId<VDIS_DEV_MAX; devId++)
    {
        if(gVdisModuleContext.displayId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkControl(gVdisModuleContext.displayId[devId],
                            DISPLAY_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
        }
    }

    OSA_waitMsecs(2000); // allow for print to complete

    if(gVencModuleContext.encId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVencModuleContext.encId,
                            ENC_LINK_CMD_PRINT_IVAHD_STATISTICS,
                            NULL, 0, TRUE);
    }
    else if (gVdecModuleContext.decId !=SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVdecModuleContext.decId,
                            DEC_LINK_CMD_PRINT_IVAHD_STATISTICS,
                            NULL, 0, TRUE);
	}

    if(gVencModuleContext.encId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVencModuleContext.encId,
                            ENC_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
    }
    if(gVdecModuleContext.decId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVdecModuleContext.decId,
                            DEC_LINK_CMD_PRINT_STATISTICS,
                            NULL, 0, TRUE);
    }

    OSA_waitMsecs(2000); // allow for print to complete

    MultiCh_prfLoadPrint(TRUE,TRUE);

    return ERROR_NONE;
}

Int32 Vsys_printBufferStatistics()
{
    UInt32 devId;

    if(gVcapModuleContext.captureId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVcapModuleContext.captureId,
                            CAPTURE_LINK_CMD_PRINT_BUFFER_STATISTICS,
                            NULL, 0, TRUE);
    }

    for(devId=0; devId<VDIS_DEV_MAX; devId++)
    {
        if(gVdisModuleContext.swMsId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkControl(gVdisModuleContext.swMsId[devId],
                            SYSTEM_SW_MS_LINK_CMD_PRINT_BUFFER_STATISTICS,
                            NULL, 0, TRUE);
        }
    }

    for(devId=0; devId<VDIS_DEV_MAX; devId++)
    {
        if(gVdisModuleContext.displayId[devId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkControl(gVdisModuleContext.displayId[devId],
                            DISPLAY_LINK_CMD_PRINT_BUFFER_STATISTICS,
                            NULL, 0, TRUE);
        }
    }

    OSA_waitMsecs(500);

    if(gVencModuleContext.encId !=SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVencModuleContext.encId,
                        ENC_LINK_CMD_PRINT_BUFFER_STATISTICS,
                        NULL, 0, TRUE);
    }

    if(gVdecModuleContext.decId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkControl(gVdecModuleContext.decId,
                            DEC_LINK_CMD_PRINT_BUFFER_STATISTICS,
                            NULL, 0, TRUE);
    }

    OSA_waitMsecs(500);

    return ERROR_NONE;
}

/**
 * \brief:
 *      This function gives CPU ID, board type and revision.
 * \input:
 *      VSYS_PLATFORM_INFO_S sturcture
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vsys_getPlatformInfo(VSYS_PLATFORM_INFO_S *pInfo)
{
    Int32 status;

    SystemVpss_PlatformInfo  platformInfo;

    status = System_linkControl(
                SYSTEM_LINK_ID_M3VPSS,
                SYSTEM_M3VPSS_CMD_GET_PLATFORM_INFO,
                &platformInfo,
                sizeof(platformInfo),
                TRUE
			);

    UTILS_assert(status==OSA_SOK);

    /* Get CPU version */
    pInfo->cpuRev = platformInfo.cpuRev;

    if (pInfo->cpuRev >= SYSTEM_PLATFORM_CPU_REV_MAX)
    {
        pInfo->cpuRev = SYSTEM_PLATFORM_CPU_REV_UNKNOWN;
    }
    /* Detect board */
    pInfo->boardId = platformInfo.boardId;

    if (pInfo->boardId >= SYSTEM_PLATFORM_BOARD_MAX)
    {
        pInfo->boardId = SYSTEM_PLATFORM_BOARD_UNKNOWN;
    }

    /* Get base board revision */
    pInfo->boardRev = platformInfo.baseBoardRev;

    if (pInfo->boardRev >= SYSTEM_PLATFORM_BOARD_REV_MAX)
    {
        pInfo->boardRev = SYSTEM_PLATFORM_BOARD_REV_UNKNOWN;
    }

    if (pInfo->boardId != SYSTEM_PLATFORM_BOARD_UNKNOWN)
    {
        /* Get daughter card revision */
        pInfo->boardRev = platformInfo.dcBoardRev;
        if (pInfo->boardRev >= SYSTEM_PLATFORM_BOARD_REV_MAX)
        {
            pInfo->boardRev = SYSTEM_PLATFORM_BOARD_REV_UNKNOWN;
        }
    }

    return ERROR_NONE;
}

Void Vsys_getContext(VSYS_PARAMS_S * contextInfo)
{
    if (contextInfo != NULL)
    {
        memcpy(contextInfo,
               &gVsysModuleContext.vsysConfig,
               sizeof(VSYS_PARAMS_S));
    }
}

Void Vsys_switchFormatSD(Int32 standard)
{
	VCAM_CHN_DYNAMIC_PARAM_S psCamChnDynaParam;

	if ((standard == SYSTEM_STD_NTSC) || (standard == SYSTEM_STD_PAL))
	{
		psCamChnDynaParam.sdFormat = standard;
		Vcam_setDynamicParamChn(1, &psCamChnDynaParam, VCAM_SD_FORMAT);

		Vdis_stopDrv(VDIS_DEV_SD);
		Vdis_setResolution(VDIS_DEV_SD, standard);
		Vdis_startDrv(VDIS_DEV_SD);
	}
}

void Vsys_get_device_status(Int32 *dev_iss, Int32 *dev_dss)
{
	VSYS_M3VPSSDEV_S st;

	System_linkControl(SYSTEM_LINK_ID_M3VPSS, SYSTEM_M3VPSS_CMD_GET_DEVICE_STATUS, &st, sizeof(st), TRUE);

	*dev_iss = st.iss_device;
	*dev_dss = st.dss_device;
}

void mcfw_linux_init(int pal, int resol)
{
	int display = 1 ; /* HD display */

	if(resol == 2)
		display = 0 ; /* FHD display */

	System_init();
	Vdis_set_standard(pal);
	Vdis_disp_ctrl_init(display);
}

void mcfw_linux_exit(void)
{
	Vdis_disp_ctrl_exit();
	System_deInit();
}
