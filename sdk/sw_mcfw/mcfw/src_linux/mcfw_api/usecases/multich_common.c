#include "multich_common.h"

UInt32 gIsGlbceInitDone = 0;
UInt32 gIsDiplayToBeDisabled = 0;
//GlbcePreset gGLBCE_AlgoPreset = GLBCE_HIGHSPEED;
#if defined (TI816X_DVR) || defined (TI8107_DVR)
#    define     SYSTEM_PLATFORM_BOARD                 SYSTEM_PLATFORM_BOARD_DVR
#else
#    ifdef TI816X_EVM
#        define SYSTEM_PLATFORM_BOARD                 SYSTEM_PLATFORM_BOARD_VS
#    else
#        ifdef TI814X_EVM
#            define SYSTEM_PLATFORM_BOARD             SYSTEM_PLATFORM_BOARD_VS
#        else
#           ifdef TI814X_DVR
#                define SYSTEM_PLATFORM_BOARD             SYSTEM_PLATFORM_BOARD_VS
#            else
#               ifdef TI8107_EVM
#                   define SYSTEM_PLATFORM_BOARD          SYSTEM_PLATFORM_BOARD_VS
#               else
#                   if defined(TI810X_UBX)
#                       define SYSTEM_PLATFORM_BOARD  SYSTEM_PLATFORM_BOARD_VS
#                   else
#                       error "Unknown Board Type"
#                   endif
#               endif
#            endif
#        endif
#    endif
#endif

UInt32 gMultiCh_enabledProcs[] = {
    SYSTEM_LINK_ID_M3VPSS,
    SYSTEM_LINK_ID_M3VIDEO,
#if defined(TI_8107_BUILD) || !defined(IPNC_DSP_ON)
    SYSTEM_LINK_ID_INVALID,
#else
    SYSTEM_LINK_ID_DSP,
#endif
};

char *gMultiCh_cpuName[SYSTEM_PLATFORM_CPU_REV_MAX] = {
    "ES1.0",
    "ES1.1",
    "ES2.0",
    "ES2.1",
    "UNKNOWN",
};

char *gMultiCh_boardName[SYSTEM_PLATFORM_BOARD_MAX] = {
    "UNKNOWN",
    "4x TVP5158 VS",
    "2x SII9135, 1x TVP7002 VC",
    "2x SIL1161A, 2x TVP7002 Catalog",
    "UBX"
};

char *gMultiCh_boardRev[SYSTEM_PLATFORM_BOARD_REV_MAX] = {
    "UNKNOWN",
    "REV A",
    "REV B",
    "REV C",
    "DVR"
};

Int32 MultiCh_detectBoard()
{
    Int32 status;
    UInt32 boardRev;
    UInt32 cpuRev;
    UInt32 boardId;


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
    cpuRev = platformInfo.cpuRev;
    if (cpuRev >= SYSTEM_PLATFORM_CPU_REV_MAX)
    {
        cpuRev = SYSTEM_PLATFORM_CPU_REV_UNKNOWN;
    }
    OSA_printf(" %u: MCFW  : CPU Revision [%s] !!! \r\n",
        OSA_getCurTimeInMsec(), gMultiCh_cpuName[cpuRev]);

    /* Detect board */
    boardId = platformInfo.boardId;
    if (boardId >= SYSTEM_PLATFORM_BOARD_MAX)
    {
        boardId = SYSTEM_PLATFORM_BOARD_UNKNOWN;
    }
    OSA_printf(" %u: MCFW  : Detected [%s] Board !!! \r\n",
        OSA_getCurTimeInMsec(), gMultiCh_boardName[boardId]);

    /* Get base board revision */
    boardRev = platformInfo.baseBoardRev;
    if (boardRev >= SYSTEM_PLATFORM_BOARD_REV_MAX)
    {
        boardRev = SYSTEM_PLATFORM_BOARD_REV_UNKNOWN;
    }
    OSA_printf(" %u: MCFW  : Base Board Revision [%s] !!! \r\n",
        OSA_getCurTimeInMsec(), gMultiCh_boardRev[boardRev]);

    if (boardId != SYSTEM_PLATFORM_BOARD_UNKNOWN)
    {
        /* Get daughter card revision */
        boardRev = platformInfo.dcBoardRev;
        if (boardRev >= SYSTEM_PLATFORM_BOARD_REV_MAX)
        {
            boardRev = SYSTEM_PLATFORM_BOARD_REV_UNKNOWN;
        }
        OSA_printf(" %u: MCFW  : Daughter Card Revision [%s] !!! \r\n",
            OSA_getCurTimeInMsec(), gMultiCh_boardRev[boardRev]);
    }

    return 0;
}

Int32 MultiCh_displayCtrlInit(VDIS_PARAMS_S * prm)
{
    Int32 status;

    status = System_linkControl(
        SYSTEM_LINK_ID_M3VPSS,
        SYSTEM_M3VPSS_CMD_GET_DISPLAYCTRL_INIT,
        &(*prm),
        sizeof(*prm),
        TRUE
        );
    UTILS_assert(status==OSA_SOK);
    return status;
}

Int32 MultiCh_displayCtrlDeInit(VDIS_PARAMS_S * prm)
{
    Int32 status;

    status = System_linkControl(
        SYSTEM_LINK_ID_M3VPSS,
        SYSTEM_M3VPSS_CMD_GET_DISPLAYCTRL_DEINIT,
        &(*prm),
        sizeof(*prm),
        TRUE
        );
    UTILS_assert(status==OSA_SOK);

    return status;
}

Int32 MultiCh_prfLoadCalcEnable(Bool enable, Bool printStatus, Bool printTskLoad)
{
    UInt32 numProcs, procId;

    numProcs = sizeof(gMultiCh_enabledProcs)/sizeof(gMultiCh_enabledProcs[0]);

    for(procId=0; procId<numProcs; procId++)
    {
	    if(gMultiCh_enabledProcs[procId] == SYSTEM_LINK_ID_INVALID)
		{
		    continue;
		}

        if(enable)
        {
            System_linkControl(
                gMultiCh_enabledProcs[procId],
                SYSTEM_COMMON_CMD_CPU_LOAD_CALC_START,
                NULL,
                0,
                TRUE
            );
        }
        else
        {
            System_linkControl(
                gMultiCh_enabledProcs[procId],
                SYSTEM_COMMON_CMD_CPU_LOAD_CALC_STOP,
                NULL,
                0,
                TRUE
            );
            if(printStatus)
            {
                SystemCommon_PrintStatus printStatus;

                memset(&printStatus, 0, sizeof(printStatus));

                printStatus.printCpuLoad = TRUE;
                printStatus.printTskLoad = printTskLoad;
                System_linkControl(
                    gMultiCh_enabledProcs[procId],
                    SYSTEM_COMMON_CMD_PRINT_STATUS,
                    &printStatus,
                    sizeof(printStatus),
                    TRUE
                );
            }
            System_linkControl(
                gMultiCh_enabledProcs[procId],
                SYSTEM_COMMON_CMD_CPU_LOAD_CALC_RESET,
                NULL,
                0,
                TRUE
            );
        }
    }

    return 0;
}

Int32 MultiCh_getSensorId(Int32 sensorId)
{
    return (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + sensorId);
}

Int32 MultiCh_prfLoadPrint(Bool printTskLoad, Bool resetTskLoad)
{
    UInt32 numProcs, procId;

    numProcs = sizeof(gMultiCh_enabledProcs)/sizeof(gMultiCh_enabledProcs[0]);

    for(procId=0; procId<numProcs; procId++)
    {
	    if(gMultiCh_enabledProcs[procId] == SYSTEM_LINK_ID_INVALID)
		{
		    continue;
		}

        SystemCommon_PrintStatus printStatus;

        memset(&printStatus, 0, sizeof(printStatus));

        printStatus.printCpuLoad = TRUE;
        printStatus.printTskLoad = printTskLoad;
        System_linkControl(
                   gMultiCh_enabledProcs[procId],
                   SYSTEM_COMMON_CMD_PRINT_STATUS,
                   &printStatus,
                   sizeof(printStatus),
                   TRUE);
        if (resetTskLoad)
        {
            System_linkControl(
               gMultiCh_enabledProcs[procId],
               SYSTEM_COMMON_CMD_CPU_LOAD_CALC_RESET,
               NULL,
               0,
               TRUE);
        }
        OSA_waitMsecs(500); // allow for print to complete
    }
    return 0;
}

Int32 MultiCh_memPrintHeapStatus(void)
{
    UInt32 numProcs, procId;
    SystemCommon_PrintStatus printStatus;

    memset(&printStatus, 0, sizeof(printStatus));
    numProcs = sizeof(gMultiCh_enabledProcs)/sizeof(gMultiCh_enabledProcs[0]);

    printStatus.printHeapStatus = TRUE;

    for(procId=0; procId<numProcs; procId++)
    {
	    if(gMultiCh_enabledProcs[procId] == SYSTEM_LINK_ID_INVALID)
		{
		    continue;
		}

        System_linkControl(
                gMultiCh_enabledProcs[procId],
                SYSTEM_COMMON_CMD_CPU_LOAD_CALC_START,
                &printStatus,
                sizeof(printStatus),
                TRUE
            );
    }

    return 0;
}

Int32 MultiCh_swMsGetOutSize(UInt32 outRes, UInt32 * width, UInt32 * height)
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

        default:
        case VSYS_STD_1080I_60:
        case VSYS_STD_1080P_60:
        case VSYS_STD_1080P_30:
            *width = 1920;
            *height = 1080;
            break;

        case VSYS_STD_NTSC:
            *width = 720;
            *height = 480;
            break;

        case VSYS_STD_PAL:
            *width = 720;
            *height = 576;
            break;

        case VSYS_STD_XGA_60:
            *width = 1024;
            *height = 768;
            break;

        case VSYS_STD_SXGA_60:
            *width = 1280;
            *height = 1024;
            break;

        case VSYS_STD_QVGA_60:
            *width = 240;
            *height = 320;
            break;
    }

    return 0;
}

Void MultiCh_swMsGetUserLayoutPrm(UInt32 dev, SwMsLink_CreateParams *swMsCreateArgs, int win_num)
{
	SwMsLink_LayoutPrm *layoutInfo;
    SwMsLink_LayoutWinInfo *winInfo;
    UInt32 outWidth, outHeight, row, col, winWidth, winHeight, widthAlign;
    UInt32 winId, offset=0;
    VDIS_MOSAIC_S *mosaicParam;
    UInt32 wi, he;

    MultiCh_swMsGetOutSize(swMsCreateArgs->maxOutRes, &wi, &he);

    widthAlign = 8;
	if(dev == VDIS_DEV_SD) {
//		offset = 16;
	}
	

	outWidth = wi - (offset*2);
	outHeight = he - (offset*2);

	layoutInfo = &swMsCreateArgs->layoutPrm;

    /* init to known default */
    memset(layoutInfo, 0, sizeof(*layoutInfo));

    mosaicParam = &(gVdisModuleContext.vdisConfig.mosaicParams[dev]);
	mosaicParam->userSetDefaultSWMLayout = FALSE;
	mosaicParam->outputFPS = 30;

	layoutInfo->onlyCh2WinMapChanged = FALSE;
	layoutInfo->numWin = win_num;
	winId = 0;

	if(win_num == 1)
	{
		winInfo = &layoutInfo->winInfo[winId];
		winInfo->channelNum = winId;
		winInfo->bypass = FALSE;
		winInfo->startX = offset;
		winInfo->startY = offset;
		winInfo->width  = SystemUtils_floor(outWidth, widthAlign);
		winInfo->height = outHeight;
	}
	else if(win_num < 5)	//# 2x2
    {
		winWidth  = SystemUtils_floor((outWidth/2), widthAlign);
		winHeight = outHeight/2;

		for(row=0; row<2; row++)
		{
			for(col=0; col<2; col++)
			{
				winInfo = &layoutInfo->winInfo[winId];
				winInfo->channelNum = winId++;
				winInfo->bypass = FALSE;
				winInfo->startX = offset + (winWidth * col);
				winInfo->startY = offset + (winHeight * row);
/*              
                if(row == 1 && col == 0)
				    winInfo->width  = winWidth + 1; 
                else
*/
				    winInfo->width  = winWidth;

				winInfo->height = winHeight;
				if(winId == win_num) {
					return;
				}
			}
		}
    }
    else	//# max display (3x3)
    {
    	winWidth  = SystemUtils_floor((outWidth/3), widthAlign);
		winHeight = outHeight/3;

		for(row=0; row<3; row++)
		{
			for(col=0; col<3; col++)
			{
				winInfo = &layoutInfo->winInfo[winId];
				winInfo->channelNum = winId++;
				winInfo->bypass = FALSE;
				winInfo->startX = offset + (winWidth * col);
				winInfo->startY = offset + (winHeight * row);
				winInfo->width  = winWidth;
				winInfo->height = winHeight;
				if(winId == win_num) {
					return;
				}
			}
		}
	}
}

Void MultiCh_createBuf(Ptr * phyAddr, Ptr * bufPtr, Ptr * srBufPtr,
                       UInt32 bufSize, UInt32 srIndex)
{
    IHeap_Handle srBitBufHeapHandle;

    UInt32 cacheLineSize = 128;

    srBitBufHeapHandle = SharedRegion_getHeap(srIndex);
    OSA_assert(srBitBufHeapHandle != NULL);
    /* if(srIndex == SYSTEM_IPC_SR_CACHED) { cacheLineSize =
     * SharedRegion_getCacheLineSize(srIndex); } */

    bufSize = OSA_align(bufSize, cacheLineSize);

    *bufPtr = (Ptr)
        Memory_alloc(srBitBufHeapHandle, bufSize, cacheLineSize, NULL);
    OSA_assert(*bufPtr != NULL);

    *phyAddr = Memory_translate(*bufPtr, Memory_XltFlags_Virt2Phys);

    *srBufPtr = (Ptr) SharedRegion_getSRPtr(*bufPtr, srIndex);
    OSA_printf("CHAINS_IPCALLOCBUF:Translated Addr Virt:%p To Phy:%p Shared:%p",
               *bufPtr, *phyAddr, *srBufPtr);

    OSA_assert(*srBufPtr != 0);
}

Void MultiCh_deleteBuf(Ptr bufPtr, UInt32 bufSize, UInt32 srIndex)
{
    IHeap_Handle srBitBufHeapHandle;

    srBitBufHeapHandle = SharedRegion_getHeap(srIndex);
    OSA_assert(srBitBufHeapHandle != NULL);

    OSA_assert(bufPtr != NULL);

    Memory_free(srBitBufHeapHandle, bufPtr, bufSize);
}

Void MultiCh_fillBuf(UInt8 * bufPtr, Int8 * fileName, UInt32 fSize)
{
    FILE *fp = NULL;

    fp = fopen(fileName, "rb");

    if(fp == NULL)
	return;
	OSA_assert(fp != NULL);
    OSA_assert(fread(bufPtr, 1, fSize, fp) == fSize);

    fclose(fp);
}

