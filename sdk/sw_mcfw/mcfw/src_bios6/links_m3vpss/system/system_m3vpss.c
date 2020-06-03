/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

#include "system_priv_m3vpss.h"
#include <mcfw/src_bios6/utils/utils_tiler_allocator.h>
#include <mcfw/src_bios6/utils/utils_dmtimer.h>
#include <mcfw/src_bios6/utils/utils_dma.h>
#include <ti/psp/devices/vps_device.h>
#include <ti/psp/platforms/iss_platform.h>
#include <ti/psp/iss/drivers/iss_init.h>
#include <ti/psp/devices/tvp5158/vpsdrv_tvp5158.h>
#include <ti/psp/devices/tvp5158/vpsdrv_tvp5158.h>
#include <mcfw/interfaces/link_api/swosdLink.h>
#include <mcfw/interfaces/link_api/fdLink.h>
#include <mcfw/interfaces/link_api/ispLink.h>

System_VpssObj gSystem_objVpss;

#ifdef SYSTEM_DEBUG
#define	dprintf(x...) Vps_printf(x);
#else
#define	dprintf(x...)
#endif

Int32 System_init()
{
	Int32 status;

	Semaphore_Params semParams;

	Vps_PlatformDeviceInitParams initPrms;
	Vps_PlatformInitParams platformInitPrms;
	Iss_PlatformInitParams issPlatInitPrms;
	#ifdef ISS_DEV_ON
	Iss_PlatformDeviceInitParams issInitPrms;
	#endif

	dprintf(" %d: SYSTEM  : System M3VPSS Init in progress !!!\n", Utils_getCurTimeInMsec());
	System_dispCheckStopList();

	Utils_tilerAllocatorInit();

	#ifdef SYSTEM_DEBUG_TILER_ALLOC
	Utils_tilerDebugLogEnable(TRUE);
	#endif

	#if defined(TI_814X_BUILD) || defined(TI_8107_BUILD)
	/* Need to set this bit only for ti814x to support tied vencs, pin mux settings */
	/* Need to call this before accessing HD VENC registers in FVID2_init()
	    Other wise it may hang in access of HDVENC registers
	*/
	//(* (UInt32 *)0x481C52C8) = 0x01000000;
	#endif

	platformInitPrms.isPinMuxSettingReq = TRUE;
	status = Vps_platformInit(&platformInitPrms);
	UTILS_assert(status == 0);

	issPlatInitPrms.isPinMuxSettingReq = TRUE;
	status = Iss_platformInit(&issPlatInitPrms);
	UTILS_assert(status == 0);

	Utils_dmTimerInit();
	IpcOutM3Link_init();
	IpcInM3Link_init();
	IpcFramesInLink_init();
	IpcFramesOutLink_init();
	IpcBitsInLink_init();
	IpcBitsOutLink_init();

	dprintf(" %d: SYSTEM : HDVPSS Drivers Version: %s\n",
				Utils_getCurTimeInMsec(), FVID2_getVersionString());

	/* FVID2 system init */
	status = FVID2_init(NULL);
	UTILS_assert(status == 0);
	dprintf(" %d: SYSTEM  : FVID2 Init in progress DONE !!!\n", Utils_getCurTimeInMsec());

	status = Iss_init(NULL);
	UTILS_assert(status == 0);
	dprintf(" %d: SYSTEM  : ISS Init in progress DONE !!!\n", Utils_getCurTimeInMsec());

	initPrms.isI2cInitReq = TRUE;
	initPrms.isI2cProbingReq = FALSE;
    #if 0
	dprintf("\ninitPrms.isI2cInitReq = %d\n", initPrms.isI2cInitReq);
	dprintf("\ninitPrms.isI2cProbingReq = %d\n", initPrms.isI2cProbingReq);
    #endif

	gSystem_objVpss.ste_dssdev = Vps_platformDeviceInit(&initPrms);

	dprintf(" %d: SYSTEM  : Device Init in progress DONE !!!\n", Utils_getCurTimeInMsec());

	#ifdef ISS_DEV_ON
	//issInitPrms.isI2cInitReq = TRUE;
	gSystem_objVpss.ste_issdev = Iss_platformDeviceInit(&issInitPrms);
	#endif

	#ifdef SYSTEM_DEBUG_VIP_RES_ALLOC
	/* enable logs from VIP resource allocator */
	Vcore_vipResDebugLogEnable(TRUE);
	#endif

	Semaphore_Params_init(&semParams);
	semParams.mode = Semaphore_Mode_BINARY;

	gSystem_objVpss.vipLock[SYSTEM_VIP_0] = Semaphore_create(1u, &semParams, NULL);
	UTILS_assert(gSystem_objVpss.vipLock[SYSTEM_VIP_0] != NULL);

	Semaphore_Params_init(&semParams);
	semParams.mode = Semaphore_Mode_BINARY;

	gSystem_objVpss.vipLock[SYSTEM_VIP_1] = Semaphore_create(1u, &semParams, NULL);
	UTILS_assert(gSystem_objVpss.vipLock[SYSTEM_VIP_1] != NULL);

	System_clearVipResetFlag(SYSTEM_VIP_0);
	System_clearVipResetFlag(SYSTEM_VIP_1);

	System_allocBlankFrame();

	Vps_printf(" %d: SYSTEM  : System M3VPSS Init Done !!!\n", Utils_getCurTimeInMsec());

	Utils_dmaInit();

	System_initLinks();

	return status;
}

Int32 System_deInit()
{
	Int32 status = FVID2_SOK;

	dprintf(" %d: SYSTEM  : System M3VPSS De-Init in progress !!!\n", Utils_getCurTimeInMsec());

	IpcInM3Link_deInit();
	IpcOutM3Link_deInit();
	IpcFramesInLink_deInit();
	IpcFramesOutLink_deInit();
	IpcBitsInLink_deInit();
	IpcBitsOutLink_deInit();

	Utils_dmaDeInit();

	Utils_dmTimerDeInit();
	System_deInitLinks();

	System_freeBlankFrame();

	Semaphore_delete(&gSystem_objVpss.vipLock[SYSTEM_VIP_0]);
	Semaphore_delete(&gSystem_objVpss.vipLock[SYSTEM_VIP_1]);

	dprintf(" %d: SYSTEM  : Platform Device De-init in progress !!!\n", Utils_getCurTimeInMsec());

	#ifdef ISS_DEV_ON
	Iss_platformDeviceDeInit();
	#endif
	Vps_platformDeviceDeInit();
	dprintf(" %d: SYSTEM  : Platform Device De-init progress Done !!!\n", Utils_getCurTimeInMsec());

	Iss_deInit(NULL);	/* ISS system de-init */
	FVID2_deInit(NULL);	/* FVID2 system de-init */

	Iss_platformDeInit();
	Vps_platformDeInit();
	dprintf(" %d: SYSTEM  : Platform De-init progress Done !!!\n", Utils_getCurTimeInMsec());

	#ifdef SYSTEM_USE_TILER
	Utils_tilerAllocatorDeInit();
	#endif

	Vps_printf(" %d: SYSTEM  : System M3VPSS De-Init Done !!!\n", Utils_getCurTimeInMsec());

	return status;
}

Void System_initLinks()
{
	Vps_printf(" %d: SYSTEM  : Initializing Links !!! \r\n", Utils_getCurTimeInMsec());
	System_memPrintHeapStatus();

	CaptureLink_init();
	#ifdef ISS_DEV_ON
	CameraLink_init();
	IspLink_init();
	#endif
	SwosdLink_init();

	NsfLink_init();
	DeiLink_init();
	DisplayLink_init();
	NullLink_init();
	NullSrcLink_init();

	DupLink_init();
	SclrLink_init();
	SwMsLink_init();
	MergeLink_init();
	SelectLink_init();

	#if AVSYNC_COMP_ENABLE
	Vps_printf(" %d: SYSTEM  : Initializing AVsync ********************** !!! \r\n", Utils_getCurTimeInMsec());
	AVSYNC_M3_Init();
	#endif

	Vps_printf(" %d: SYSTEM  : Initializing Links ... DONE !!! \r\n", Utils_getCurTimeInMsec());
}

Void System_deInitLinks()
{
	Vps_printf(" %d: SYSTEM  : De-Initializing Links !!! \r\n", Utils_getCurTimeInMsec());

	SelectLink_deInit();
	MergeLink_deInit();

	NullLink_deInit();
	DisplayLink_deInit();
	NullSrcLink_deInit();
	DeiLink_deInit();
	NsfLink_deInit();

	CaptureLink_deInit();
	#ifdef ISS_DEV_ON
	CameraLink_deInit();
	IspLink_deInit();
	#endif
	SwosdLink_deInit();
	DupLink_deInit();
	SclrLink_deInit();
	SwMsLink_deInit();

	System_memPrintHeapStatus();

	#if AVSYNC_COMP_ENABLE
	Vps_printf(" %d: SYSTEM  : De-Initializing Links ...  avsync !!! \r\n", Utils_getCurTimeInMsec());
	AVSYNC_M3_DeInit();
	#endif

	Vps_printf(" %d: SYSTEM  : De-Initializing Links ... DONE !!! \r\n", Utils_getCurTimeInMsec());
}

Int32 System_lockVip(UInt32 vipInst)
{
   if (vipInst < SYSTEM_VIP_MAX)
   {
       Semaphore_pend(gSystem_objVpss.vipLock[vipInst], BIOS_WAIT_FOREVER);
   }

   return FVID2_SOK;
}

Int32 System_unlockVip(UInt32 vipInst)
{
   if (vipInst < SYSTEM_VIP_MAX)
   {
       Semaphore_post(gSystem_objVpss.vipLock[vipInst]);
   }

   return FVID2_SOK;
}

Int32 System_setVipResetFlag(UInt32 vipInst)
{
	if (vipInst < SYSTEM_VIP_MAX)
	{
		gSystem_objVpss.vipResetFlag[vipInst] = TRUE;
	}

	return FVID2_SOK;
}

Bool System_clearVipResetFlag(UInt32 vipInst)
{
	Bool isReset = FALSE;

	if (vipInst < SYSTEM_VIP_MAX)
	{
	   isReset = gSystem_objVpss.vipResetFlag[vipInst];
	   gSystem_objVpss.vipResetFlag[vipInst] = FALSE;
	}

	return isReset;
}

Int32 System_videoResetVideoDevices()
{
	Int32 status = FVID2_SOK;

	status = Vps_platformVideoResetVideoDevices();

	return status;
}

UInt8 System_getVidDecI2cAddr(UInt32 vidDecId, UInt32 vipInstId)
{
	UInt8 devAddr = NULL;

	devAddr = Vps_platformGetVidDecI2cAddr(vidDecId, vipInstId);

	return (devAddr);
}

System_PlatformBoardRev System_getBaseBoardRev()
{
	return (System_PlatformBoardRev)(Vps_platformGetBaseBoardRev());
}

System_PlatformBoardRev System_getDcBoardRev()
{
	return (System_PlatformBoardRev)(Vps_platformGetDcBoardRev());
}

System_PlatformBoardId System_getBoardId()
{
	return (System_PlatformBoardId)(Vps_platformGetBoardId());
}

Int32 System_allocBlankFrame()
{
    UInt32 memSize;

    memSize = SYSTEM_BLANK_FRAME_WIDTH*SYSTEM_BLANK_FRAME_HEIGHT*SYSTEM_BLANK_FRAME_BYTES_PER_PIXEL;

    gSystem_objVpss.nonTiledBlankFrameAddr = Utils_memAlloc(memSize, VPS_BUFFER_ALIGNMENT*2);

    UTILS_assert(gSystem_objVpss.nonTiledBlankFrameAddr!=NULL);

    return 0;
}

Int32 System_getBlankFrame(FVID2_Frame *pFrame)
{
    memset(pFrame, 0, sizeof(*pFrame));

    pFrame->addr[0][0] = gSystem_objVpss.nonTiledBlankFrameAddr;
    pFrame->addr[0][1] = gSystem_objVpss.nonTiledBlankFrameAddr;

    return 0;
}

Int32 System_freeBlankFrame()
{
    UInt32 memSize;

    memSize = SYSTEM_BLANK_FRAME_WIDTH*SYSTEM_BLANK_FRAME_HEIGHT*SYSTEM_BLANK_FRAME_BYTES_PER_PIXEL;
    Utils_memFree(gSystem_objVpss.nonTiledBlankFrameAddr, memSize);

    return 0;
}

Int32 System_getOutSize(UInt32 outRes, UInt32 * width, UInt32 * height)
{
    switch (outRes)
    {
        case VSYS_STD_MAX:
            *width = 1920;
            *height = 1200;
            break;
        case VSYS_STD_720P_60:
            *width = 1280;
            *height = 720;
            break;
        case VSYS_STD_XGA_60:
            *width = 1024;
            *height = 768;
            break;
        case VSYS_STD_SXGA_60:
            *width = 1280;
            *height = 1024;
            break;
        case VSYS_STD_WVGA_60:
            *width = 800;
            *height = 480;
            break;
        case VSYS_STD_NTSC:
            *width = 720;
            *height = 480;
            break;
        case VSYS_STD_PAL:
            *width = 720;
            *height = 576;
            break;

        default:
        case VSYS_STD_1080I_60:
        case VSYS_STD_1080P_60:
        case VSYS_STD_1080P_30:
            *width = 1920;
            *height = 1080;
            break;
    }

    return 0;
}

Int32 system_getDeviceStatus(VSYS_M3VPSSDEV_S *dev_status)
{
	dev_status->dss_device = gSystem_objVpss.ste_dssdev;
	dev_status->iss_device = gSystem_objVpss.ste_issdev;

	return 0;
}
