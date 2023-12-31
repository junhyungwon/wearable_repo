/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

#include <xdc/std.h>
#include <mcfw/interfaces/link_api/system_tiler.h>
#include "captureLink_priv.h"
#include <mcfw/src_bios6/links_m3vpss/avsync/avsync_m3vpss.h>
#include "mcfw/interfaces/common_def/ti_vsys_common_def.h"


char *gCaptureLink_portName[] = {
    "VIP0 PortA", "VIP0 PortB", "VIP1 PortA", "VIP1 PortB",
};

char *gCaptureLink_ifName[] = {
    " 8-bit", "16-bit", "24-bit",
};

char *gCaptureLink_modeName[] = {
    "Non-mux Embedded Sync",
    "Line-mux Embedded Sync",
    "Pixel-mux Embedded Sync",
    "Non-mux Discrete Sync - HSYNC_VBLK",
    "Non-mux Discrete Sync - HSYNC_VSYNC",
    "Non-mux Discrete Sync - ACTVID_VBLK",
    "Non-mux Discrete Sync - ACTVID_VSYNC",
    "Split Line Embedded Sync"
};


/* driver callback */
Int32 CaptureLink_drvCallback(FVID2_Handle handle, Ptr appData, Ptr reserved)
{
    CaptureLink_Obj *pObj = (CaptureLink_Obj *) appData;

    if (pObj->cbCount && (pObj->cbCount % CAPTURE_LINK_TSK_TRIGGER_COUNT) == 0)
    {
        Utils_tskSendCmd(&pObj->tsk, SYSTEM_CMD_NEW_DATA);
    }
    pObj->cbCount++;

    return FVID2_SOK;
}

/* select input source in simulator environment */
Int32 CaptureLink_drvSimVideoSourceSelect(UInt32 instId,
                                          UInt32 captureMode,
                                          UInt32 videoIfMode)
{
    UInt32 fileId = 0, pixelClk;

    /** select input source file,

       Assumes that the simulator VIP super file contents are like below

        1 <user path>\output_bt656_QCIF.bin             # 8 -bit YUV422 single CH input
        2 <user path>\output_bt1120_QCIF.bin            # 16-bit YUV422 single CH input
        3 <user path>\output_bt1120_QCIF_RGB.bin        # 24-bit RGB888 single CH input
        4 <user path>\output_tvp5158_8CH_bt656_QCIF.bin # 8 -bit YUV422 multi  CH input
    */

    pixelClk = 80 * 1000000 / 2;

    switch (captureMode)
    {
        case VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC:
            switch (videoIfMode)
            {
                case VPS_CAPT_VIDEO_IF_MODE_8BIT:
                    fileId = 5;
                    break;
                case VPS_CAPT_VIDEO_IF_MODE_16BIT:
                    fileId = 6;
                    break;
                case VPS_CAPT_VIDEO_IF_MODE_24BIT:
                    fileId = 7;
                    break;

            }
            break;

        case VPS_CAPT_VIDEO_CAPTURE_MODE_MULTI_CH_LINE_MUX_EMBEDDED_SYNC:
            fileId = 8;
            pixelClk = 40 * 1000000;
            break;

    }

    Vps_platformSimVideoInputSelect(instId, fileId, pixelClk);

    return FVID2_SOK;
}

/* Create video decoder, TVP5158 or HDMI RX */
Int32 CaptureLink_drvCreateVideoDecoder(CaptureLink_Obj * pObj, UInt16 instId)
{
    CaptureLink_VipInstParams *pInstPrm;
    CaptureLink_InstObj *pInst;
    Vps_CaptCreateParams *pVipCreateArgs;

    Vps_VideoDecoderChipIdParams vidDecChipIdArgs;
    Vps_VideoDecoderChipIdStatus vidDecChipIdStatus;
    Int32 status = FVID2_SOK;

    pInstPrm = &pObj->createArgs.vipInst[instId];
    pInst = &pObj->instObj[instId];

    pVipCreateArgs = &pInst->createArgs;

    pInst->vidDecCreateArgs.deviceI2cInstId = Vps_platformGetI2cInstId();
    pInst->vidDecCreateArgs.numDevicesAtPort = 1;
    pInst->vidDecCreateArgs.deviceI2cAddr[0]
        = Vps_platformGetVidDecI2cAddr(pInstPrm->videoDecoderId, pInstPrm->vipInstId);
    pInst->vidDecCreateArgs.deviceResetGpio[0] = VPS_VIDEO_DECODER_GPIO_NONE;
	pInst->vidDecCreateArgs.serdesEQ = pInstPrm->serdesEQ;

    if ((pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_TVP7002_DRV ||
         pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_SII9135_DRV)
        &&
        (pInstPrm->vipInstId == VPS_CAPT_INST_VIP0_PORTA ||
         pInstPrm->vipInstId == VPS_CAPT_INST_VIP1_PORTA))
    {
        Vps_platformSelectVideoDecoder(pInstPrm->videoDecoderId,
                                       pInstPrm->vipInstId);
    }

    pInst->videoDecoderHandle = FVID2_create(pInstPrm->videoDecoderId,
                                             0,
                                             &pInst->vidDecCreateArgs,
                                             &pInst->vidDecCreateStatus, NULL);
    UTILS_assert(pInst->videoDecoderHandle != NULL);

	#if 0
    vidDecChipIdArgs.deviceNum = 0;
    status = FVID2_control(pInst->videoDecoderHandle,
                           IOCTL_VPS_VIDEO_DECODER_GET_CHIP_ID,
                           &vidDecChipIdArgs, &vidDecChipIdStatus);
	//UTILS_assert(status == FVID2_SOK);
	#endif

    pInst->vidDecVideoModeArgs.videoIfMode = pVipCreateArgs->videoIfMode;
    pInst->vidDecVideoModeArgs.videoDataFormat = pVipCreateArgs->inDataFormat;
    pInst->vidDecVideoModeArgs.standard = pInstPrm->standard;
    pInst->vidDecVideoModeArgs.videoCaptureMode = pVipCreateArgs->videoCaptureMode;
    pInst->vidDecVideoModeArgs.videoSystem = VPS_VIDEO_DECODER_VIDEO_SYSTEM_AUTO_DETECT;
    pInst->vidDecVideoModeArgs.videoCropEnable = pObj->createArgs.enableSdCrop;
    if (TRUE == pObj->createArgs.doCropInCapture)
    {
        pInst->vidDecVideoModeArgs.videoCropEnable = FALSE;
    }
    pInst->vidDecVideoModeArgs.videoAutoDetectTimeout = BIOS_WAIT_FOREVER;

    if (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_TVP5158_DRV
    		|| pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP6124B_DRV)
    {
        /* need to be in NTSC or PAL mode specifically in order for cable
         * disconnect/connect to work reliably with TVP5158 */
        if (pObj->isPalMode) {
            pInst->vidDecVideoModeArgs.videoSystem = VPS_VIDEO_DECODER_VIDEO_SYSTEM_PAL;
        }
        else {
            pInst->vidDecVideoModeArgs.videoSystem = VPS_VIDEO_DECODER_VIDEO_SYSTEM_NTSC;
        }
	}

    #ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: VIP %d: VID DEC %d (0x%02x): %04x:%04x:%04x\n",
               Utils_getCurTimeInMsec(),
               pInstPrm->vipInstId,
               pInstPrm->videoDecoderId,
               pInst->vidDecCreateArgs.deviceI2cAddr[0],
               vidDecChipIdStatus.chipId,
               vidDecChipIdStatus.chipRevision,
               vidDecChipIdStatus.firmwareVersion);
    #endif

    return status;
}

Int32 CaptureLink_drvInstSetFrameSkip(CaptureLink_Obj * pObj, UInt16 instId,
                                      UInt32 frameSkipMask)
{
    Vps_CaptFrameSkip frameSkip;
    UInt16 outId, chId;
    Int32 status = FVID2_SOK;
    CaptureLink_InstObj *pInst;
    Vps_CaptCreateParams *pVipCreateArgs;

    pInst = &pObj->instObj[instId];
    pVipCreateArgs = &pInst->createArgs;

    /*
     * set frame skip using a IOCTL if enabled
     */
    for (outId = 0; outId < pVipCreateArgs->numStream; outId++)
    {
        for (chId = 0; chId < pVipCreateArgs->numCh; chId++)
        {

            frameSkip.channelNum = pVipCreateArgs->channelNumMap[outId][chId];

            frameSkip.frameSkipMask = frameSkipMask;

            status = FVID2_control(pInst->captureVipHandle,
                                   IOCTL_VPS_CAPT_SET_FRAME_SKIP,
                                   &frameSkip, NULL);
            UTILS_assert(status == FVID2_SOK);
        }
    }

    return status;
}

/* Create capture driver */
Int32 CaptureLink_drvCreateInst(CaptureLink_Obj * pObj, UInt16 instId)
{
    CaptureLink_VipInstParams *pInstPrm;
    CaptureLink_InstObj *pInst;
    Vps_CaptCreateParams *pVipCreateArgs;
    Vps_CaptOutInfo *pVipOutPrm;
    CaptureLink_OutParams *pOutPrm;
    System_LinkChInfo *pQueChInfo;
    UInt16 queId, queChId, outId, chId, inWidth, inHeight;
    FVID2_ScanFormat inScanFormat;
#ifdef ENABLE_BLIND_AREA
    UInt16 winId;
#endif

    pInstPrm = &pObj->createArgs.vipInst[instId];
    pInst = &pObj->instObj[instId];
    pVipCreateArgs = &pInst->createArgs;

    pInst->instId = pInstPrm->vipInstId;

    CaptureLink_drvInitCreateArgs(pVipCreateArgs);

    pVipCreateArgs->inDataFormat = pInstPrm->inDataFormat;

    memset(&pInst->vidDecCurStatus, 0, sizeof(pInst->vidDecCurStatus));

    for(chId=0; chId<VPS_CAPT_CH_PER_PORT_MAX; chId++)
    {
        pInst->vidDecCurStatus[chId].chId = chId;
        pInst->vidDecCurStatus[chId].vipInstId = pInst->instId;
    }

    pObj->en_videodet = TRUE;

    if (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_TVP5158_DRV)
    {
        inScanFormat = FVID2_SF_INTERLACED;

        if(pObj->createArgs.enableSdCrop) {
            pObj->maxWidth = 704;
            pObj->inWidth[instId] = 704;
        } else {
            pObj->maxWidth = 720;
            pObj->inWidth[instId] = 720;
        }

        if (pObj->isPalMode) {
            pObj->maxHeight = 288;
            pObj->inHeight[instId] = 288;
        } else {
            pObj->maxHeight = 240;
            pObj->inHeight[instId] = 240;
        }

        inWidth = pObj->maxWidth;
        inHeight = pObj->maxHeight;

        pVipCreateArgs->videoCaptureMode =
            VPS_CAPT_VIDEO_CAPTURE_MODE_MULTI_CH_PIXEL_MUX_EMBEDDED_SYNC;
        pVipCreateArgs->videoIfMode = VPS_CAPT_VIDEO_IF_MODE_8BIT;

        switch (pInstPrm->standard)
        {
            case FVID2_STD_CIF:
                pObj->maxHeight = pObj->maxHeight/2 ;
                inHeight = inHeight/2;
            case FVID2_STD_HALF_D1:
                pVipCreateArgs->numCh = 1;
                pVipCreateArgs->videoCaptureMode =
                    VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;
                pObj->maxWidth = pObj->maxWidth/2;
                inWidth = inWidth/2;
                break;
            case FVID2_STD_D1:
                pVipCreateArgs->numCh = 1;
                pVipCreateArgs->videoCaptureMode =
                    VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;

                // psuedo HD mode
                if(pObj->createArgs.fakeHdMode)
                {
					//if(instId == 0)
                    {
                        pObj->maxWidth = 1920;
                        pObj->maxHeight = 1080;
                    }
                }
                break;

            case FVID2_STD_MUX_2CH_D1:
                pVipCreateArgs->numCh = 2;
                break;

            case FVID2_STD_MUX_4CH_CIF:
                pObj->maxHeight /= 2 ;
                inHeight = inHeight/2;
            case FVID2_STD_MUX_4CH_HALF_D1:
                pVipCreateArgs->numCh = 4;
                pObj->maxWidth = pObj->maxWidth/2;
                inWidth = inWidth/2;
                break;
            case FVID2_STD_MUX_4CH_D1:
            default:
                pVipCreateArgs->numCh = 4;
                break;
        }
    }
    else if (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP6124B_DRV)
    {
    	pVipCreateArgs->numCh = 1;	//# default

		switch (pInstPrm->standard)
        {
        	case FVID2_STD_MUX_4CH_D1:
        	case FVID2_STD_MUX_2CH_D1:
        	case FVID2_STD_D1:
        		inScanFormat = FVID2_SF_INTERLACED;
				pObj->maxWidth = pObj->createArgs.enableSdCrop?704:720;
	            pObj->maxHeight = pObj->isPalMode?288:240;

        		if(pInstPrm->standard == FVID2_STD_MUX_2CH_D1) {
        			pVipCreateArgs->numCh = 2;
        		} else if(pInstPrm->standard == FVID2_STD_MUX_4CH_D1) {
        			pVipCreateArgs->numCh = 4;
        		}
        		break;

        	case FVID2_STD_1080P_60:
        		inScanFormat = FVID2_SF_PROGRESSIVE;
        		pObj->maxWidth = 1920;
			    pObj->maxHeight = 1080;
        		break;

			case (FVID2_STD_CUSTOM+1):	//# FVID2_STD_MUX_2CH_720P
				pVipCreateArgs->numCh = 2;
			case FVID2_STD_720P_60:
        	default:
        		inScanFormat = FVID2_SF_PROGRESSIVE;
        		pObj->maxWidth = 1280;
			    pObj->maxHeight = 720;
        		break;
        }

        pObj->inWidth[instId] = pObj->maxWidth;
	    pObj->inHeight[instId] = pObj->maxHeight;

		inWidth = pObj->maxWidth;
		inHeight = pObj->maxHeight;

        //pVipCreateArgs->videoCaptureMode = pInstPrm->captureMode;
        if(pVipCreateArgs->numCh > 1) {
        	pVipCreateArgs->videoCaptureMode = VPS_CAPT_VIDEO_CAPTURE_MODE_MULTI_CH_PIXEL_MUX_EMBEDDED_SYNC;
        } else {
        	pVipCreateArgs->videoCaptureMode = VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;
        }
		pVipCreateArgs->videoIfMode = VPS_CAPT_VIDEO_IF_MODE_8BIT;
    }
    else if (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_PH1100K_DRV ||
    		pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_PH3100K_DRV ||
    		pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP2440H_DRV)
    {
		inScanFormat = FVID2_SF_PROGRESSIVE;
		switch (pInstPrm->standard)
        {
        	case FVID2_STD_D1:
			    pObj->maxWidth = 852;
			    pObj->maxHeight = 480;
			    pObj->inWidth[instId] = pObj->maxWidth;
			    pObj->inHeight[instId] = pObj->maxHeight;

        		inWidth = pObj->maxWidth;
        		inHeight = pObj->maxHeight;
        		break;
			case FVID2_STD_720P_60:
        	default:
        		pObj->maxWidth = 1280;
			    pObj->maxHeight = 720;
			    pObj->inWidth[instId] = pObj->maxWidth;
			    pObj->inHeight[instId] = pObj->maxHeight;

        		inWidth = pObj->maxWidth;
        		inHeight = pObj->maxHeight;
        		break;
        }

        pVipCreateArgs->videoCaptureMode = pInstPrm->captureMode;
		pVipCreateArgs->videoIfMode = VPS_CAPT_VIDEO_IF_MODE_8BIT;
    }
    else
    {
        switch (pInstPrm->standard)
        {
            case FVID2_STD_1080I_60:
                inScanFormat = FVID2_SF_INTERLACED;
                if (pVipCreateArgs->inDataFormat == FVID2_DF_RGB24_888) {
                	// since in 24-bit discrete sync mode FID signal is not connected in board
                    inScanFormat = FVID2_SF_PROGRESSIVE;
                }

                if (instId == 0)
                {
                    pObj->maxWidth = 1920;
                    pObj->maxHeight = 540;
                }
                inWidth = 1920;
                inHeight = 540;

                break;
            case FVID2_STD_720P_60:
                inScanFormat = FVID2_SF_PROGRESSIVE;
                if (instId == 0)
                {
                    pObj->maxWidth = 1280;
                    pObj->maxHeight = 748;
                }
                inWidth = 1280;
                inHeight = 720;

                break;
            case FVID2_STD_480I:
                inScanFormat = FVID2_SF_INTERLACED;
                if (instId == 0)
                {
                    pObj->maxWidth = 720;
                    if (pObj->isPalMode)
                        pObj->maxHeight = 288;
                    else
                        pObj->maxHeight = 240;
                }
                inWidth = 720;
                if (pObj->isPalMode)
                    inHeight = 288;
                else
                    inHeight = 240;
                break;

            default:
                inScanFormat = FVID2_SF_PROGRESSIVE;
                if (instId == 0)
                {
                    pObj->maxWidth = 1920;
                    pObj->maxHeight = 1080;
                }
                inWidth = 1920;
                inHeight = 1080;

                break;
        }

        if (pVipCreateArgs->inDataFormat == FVID2_DF_RGB24_888)
        {
            pVipCreateArgs->videoCaptureMode =
                VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_ACTVID_VBLK;
            pVipCreateArgs->videoIfMode = VPS_CAPT_VIDEO_IF_MODE_24BIT;
        }
        else
        {
            pVipCreateArgs->videoCaptureMode = VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;
            pVipCreateArgs->videoIfMode = VPS_CAPT_VIDEO_IF_MODE_16BIT;
        }
        pVipCreateArgs->numCh = 1;
    }

    Vps_printf(" %d: CAPTURE: %s [%s, %s] !!! \n",
               Utils_getCurTimeInMsec(),
               gCaptureLink_portName[pInstPrm->vipInstId],
               gCaptureLink_ifName[pVipCreateArgs->videoIfMode],
               gCaptureLink_modeName[pVipCreateArgs->videoCaptureMode]);

    pVipCreateArgs->inScanFormat = inScanFormat;
    pVipCreateArgs->periodicCallbackEnable = TRUE;

    pVipCreateArgs->numStream = pInstPrm->numOutput;

    for (outId = 0; outId < pVipCreateArgs->numStream; outId++)
    {
        pVipOutPrm = &pVipCreateArgs->outStreamInfo[outId];
        pOutPrm = &pInstPrm->outParams[outId];

        pVipOutPrm->dataFormat = pOutPrm->dataFormat;

        pVipOutPrm->memType = VPS_VPDMA_MT_NONTILEDMEM;

        if (pObj->createArgs.tilerEnable &&
            (pOutPrm->dataFormat == FVID2_DF_YUV420SP_UV ||
             pOutPrm->dataFormat == FVID2_DF_YUV422SP_UV))
        {
            pVipOutPrm->memType = VPS_VPDMA_MT_TILEDMEM;
        }

        pVipOutPrm->pitch[0] = VpsUtils_align(pObj->maxWidth, VPS_BUFFER_ALIGNMENT*2);
        if ((pObj->maxWidth == 704)         &&
            (pObj->createArgs.enableSdCrop) &&
            (pObj->createArgs.doCropInCapture))
        {
            pVipOutPrm->pitch[0] =
                VpsUtils_align(720, VPS_BUFFER_ALIGNMENT * 2);
        }

        if (pVipOutPrm->dataFormat == FVID2_DF_YUV422I_YUYV)
            pVipOutPrm->pitch[0] *= 2;
        if (pVipOutPrm->dataFormat == FVID2_DF_RGB24_888)
            pVipOutPrm->pitch[0] *= 3;

        pVipOutPrm->pitch[1] = pVipOutPrm->pitch[0];

        if (CaptureLink_drvIsDataFormatTiled(pVipCreateArgs, outId))
        {
            pVipOutPrm->pitch[0] = VPSUTILS_TILER_CNT_8BIT_PITCH;
            pVipOutPrm->pitch[1] = VPSUTILS_TILER_CNT_16BIT_PITCH;
        }

        pVipOutPrm->pitch[2] = 0;

        pVipOutPrm->scEnable = pOutPrm->scEnable;

        if (pObj->maxHeight <= 288)
        {
            pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_288_LINES;
        }
        else if (pObj->maxHeight <= 576)
        {
            pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_576_LINES;
        }
        else if (pObj->maxHeight <= 720)
        {
            pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_720_LINES;
        }
        else if (pObj->maxHeight <= 1080)
        {
            pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_1080_LINES;
        }
        else
        {
            pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_UNLIMITED;
        }

        /* KC:
             No need to keep unlimited height
             This was kept mainly for debug and statistics collection purpose.
             In real system we should limit the height to max expected
        */
        // pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_UNLIMITED;

        if (pVipOutPrm->scEnable)
        {
            Vps_CaptScParams *pScParams;

            pScParams = &pVipCreateArgs->scParams;

            pScParams->inScanFormat = FVID2_SF_PROGRESSIVE; // NOT USED
            pScParams->inWidth = inWidth;
            pScParams->inHeight = inHeight;
            pScParams->inCropCfg.cropStartX = 0;
            pScParams->inCropCfg.cropStartY = 0;
            pScParams->inCropCfg.cropWidth = (pScParams->inWidth & 0xFFFE);
            pScParams->inCropCfg.cropHeight = (pScParams->inHeight & 0xFFFE);
            pScParams->outWidth = pOutPrm->scOutWidth;
            if (pObj->isPalMode)
            {
                /* input source is PAL mode scOutHeight is setup assuming
                 * NTSC so convert it to 288 lines for PAL mode */
                if (pOutPrm->scOutHeight == 240)
                    pOutPrm->scOutHeight = 288;
            }
            pScParams->outHeight = pOutPrm->scOutHeight;
            pScParams->scConfig = NULL;
            pScParams->scCoeffConfig = NULL;
        }

        for (chId = 0; chId < pVipCreateArgs->numCh; chId++)
        {
            queId = pOutPrm->outQueId;
            queChId = pObj->info.queInfo[queId].numCh;

            pQueChInfo = &pObj->info.queInfo[queId].chInfo[queChId];

            pQueChInfo->dataFormat = (FVID2_DataFormat) pVipOutPrm->dataFormat;
            pQueChInfo->memType = (Vps_VpdmaMemoryType) pVipOutPrm->memType;

            if (pVipOutPrm->scEnable && !pObj->createArgs.fakeHdMode)
            {
                pQueChInfo->width = pOutPrm->scOutWidth;
                pQueChInfo->height = pOutPrm->scOutHeight;
            }
            else
            {
                pQueChInfo->width = pObj->maxWidth;
                pQueChInfo->height = pObj->maxHeight;
            }

            pQueChInfo->startX = 0;
            pQueChInfo->startY = 0;
            pQueChInfo->pitch[0] = pVipOutPrm->pitch[0];
            pQueChInfo->pitch[1] = pVipOutPrm->pitch[1];
            pQueChInfo->pitch[2] = pVipOutPrm->pitch[2];

            if(pObj->createArgs.fakeHdMode)
            {
                pQueChInfo->scanFormat = FVID2_SF_PROGRESSIVE;
            }
            else
            {
                pQueChInfo->scanFormat = inScanFormat;
            }

            pObj->info.queInfo[queId].numCh++;

            pVipCreateArgs->channelNumMap[outId][chId] =
                CaptureLink_makeChannelNum(queId, queChId);

#ifdef ENABLE_BLIND_AREA
            //configure EDMA stuff for blind area
            for(winId = 0;winId<CAPTURE_LINK_NUM_BLIND_AREA_PER_CHNL;winId++)
            {
                UInt32  channel = pVipCreateArgs->channelNumMap[outId][chId];
                pObj->blindArea[outId][channel][winId].dataFormat = pVipOutPrm->dataFormat;
                pObj->blindArea[outId][channel][winId].destPitch[0] = pVipOutPrm->pitch[0];
                pObj->blindArea[outId][channel][winId].destPitch[1] = pVipOutPrm->pitch[1];

                if(((pObj->blindArea[outId][channel][winId].dataFormat != FVID2_DF_YUV422I_YUYV)
                    &&(pObj->blindArea[outId][channel][winId].dataFormat != FVID2_DF_YUV420SP_UV))
                    || pVipOutPrm->memType == VPS_VPDMA_MT_TILEDMEM)
                {
                    // now we only support 2 format without tiler memory
                    pObj->blindArea[outId][channel][winId].dataFormat = FVID2_DF_INVALID;
                }
            }
#endif //ENABLE_BLIND_AREA
        }
    }

    memset(&pInst->cbPrm, 0, sizeof(pInst->cbPrm));

    pInst->cbPrm.appData = pObj;

    //if (instId == 0)	//# phoong
        pInst->cbPrm.cbFxn = CaptureLink_drvCallback;

    if (Vps_platformIsSim())
    {
        CaptureLink_drvSimVideoSourceSelect(pInst->instId,
                                            pVipCreateArgs->videoCaptureMode,
                                            pVipCreateArgs->videoIfMode);
    }

    pObj->enableCheckOverflowDetect = TRUE;//FALSE;
    if (pVipCreateArgs->videoCaptureMode ==
        VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_ACTVID_VBLK)
    {
        pObj->enableCheckOverflowDetect = FALSE;
    }

    pInst->captureVipHandle = FVID2_create(FVID2_VPS_CAPT_VIP_DRV,
                                           pInst->instId,
                                           pVipCreateArgs,
                                           &pInst->createStatus, &pInst->cbPrm);
    UTILS_assert(pInst->captureVipHandle != NULL);

    CaptureLink_drvAllocAndQueueFrames(pObj, pInst);

	/*
	 * 0x00000000, //30
	 * 0x00000001,0x20004000,0x20080200,0x20404040,0x20820820,  //29-25
	 * 0x21084210,0x01111111,0x11111111,0x24892488,0x24924924, //24-20
	 * 0x24924949,0x2949494A,0x2949A94A,0x29A9A94A,0x2AAAAAAA, //19-15
	 * 0x2AAAEAAA,0x2EAAEAAA,0x2B5AD6B5,0x35AD6B5B,0x1B6DB6DB, //14-10
	 * 0x1BDDB6DB,0x1BDDBDDB,0x3BDDBDDB,0x1EF7BDEF,0x1F7DF7DF,
	 * 0x1F7FF7DF,0x1F7FF7FF,0x1FFFF7FF,0x1FFFFFFF,0x3FFFFFFF
	 */
#if defined(LF_SYS_NEXXONE_VOIP) || defined(LF_SYS_NEXXB_ONE) || defined(LF_SYS_NEXXONE_CCTV_SA)	
	CaptureLink_drvInstSetFrameSkip(pObj, instId, 0);				//# nexxone 30fps
#else	
//	CaptureLink_drvInstSetFrameSkip(pObj, instId, 0x24924924);		//# 30fps->20fps
	CaptureLink_drvInstSetFrameSkip(pObj, instId, 0x2AAAAAAA);		//# 19fps->15fps
//	CaptureLink_drvInstSetFrameSkip(pObj, instId, 0);				//# nexxone 30fps
#endif
    pInst->videoDecoderHandle = NULL;

	#ifdef SYSTEM_USE_VIDEO_DECODER
	CaptureLink_drvCreateVideoDecoder(pObj, instId);
	#endif

    return FVID2_SOK;
}

/** Create capture link

    This creates
    - capture driver
    - video decoder driver
    - allocate and queues frames to the capture driver
    - DOES NOT start the capture ONLY make it ready for operation
*/
Int32 CaptureLink_drvCreate(CaptureLink_Obj * pObj,
                            CaptureLink_CreateParams * pPrm)
{
    Int32 status;
    UInt32 queId, instId;
#ifdef ENABLE_BLIND_AREA
    UInt32 streamId, channelId;
#endif //ENABLE_BLIND_AREA

#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Create in progress !!!\n", Utils_getCurTimeInMsec());
#endif

    UTILS_MEMLOG_USED_START();
    memcpy(&pObj->createArgs, pPrm, sizeof(*pPrm));

    #ifndef SYSTEM_USE_TILER
    if (pObj->createArgs.tilerEnable)
    {
        Vps_printf("CAPTURELINK:!!WARNING.FORCIBLY DISABLING TILER since tiler is disabled at build time");
        pObj->createArgs.tilerEnable = FALSE;
    }
    #endif

    pObj->captureDequeuedFrameCount = 0;
    pObj->captureQueuedFrameCount = 0;
    pObj->cbCount = 0;
    pObj->cbCountServicedCount = 0;
    pObj->prevFrameCount = 0;
    pObj->totalCpuLoad = 0;
    pObj->resetCount = 0;
    pObj->resetTotalTime = 0;
    pObj->prevResetTime = 0;
    pObj->isPalMode = FALSE;
    pObj->en_videodet = 0;
    pObj->wallTimeInTicks = 0;
    pObj->isTimesync = FALSE;

    memset(pObj->captureFrameCount, 0, sizeof(pObj->captureFrameCount));

    pObj->info.numQue = CAPTURE_LINK_MAX_OUT_QUE;

    pObj->isPalMode = pPrm->isPalMode;

	#ifdef SYSTEM_USE_VIDEO_DECODER
	//CaptureLink_drvDetectVideoStandard(pObj);
	#endif

    for (queId = 0; queId < CAPTURE_LINK_MAX_OUT_QUE; queId++)
    {
        status = Utils_bufCreate(&pObj->bufQue[queId], FALSE, FALSE);
        UTILS_assert(status == FVID2_SOK);

        pObj->info.queInfo[queId].numCh = 0;
    }

    /*
     * Create global VIP capture handle, used for dequeue,
     * queue from all active captures
     */
    pObj->fvidHandleVipAll = FVID2_create(FVID2_VPS_CAPT_VIP_DRV,
                                          VPS_CAPT_INST_VIP_ALL,
                                          NULL, NULL, NULL);
    UTILS_assert(pObj->fvidHandleVipAll != NULL);


    if (CAPTURE_LINK_TMP_BUF_SIZE)
    {
        pObj->tmpBufAddr = Utils_memAlloc(CAPTURE_LINK_TMP_BUF_SIZE, 32);
    }
#if 0
    status = FVID2_control(pObj->fvidHandleVipAll,
                           IOCTL_VPS_CAPT_DROP_DATA_BUFFER,
                           pObj->tmpBufAddr, NULL);
    UTILS_assert(status == FVID2_SOK);
#endif

    status = FVID2_control(pObj->fvidHandleVipAll,
                           IOCTL_VPS_CAPT_RESET_VIP0, NULL, NULL);
    UTILS_assert(status == FVID2_SOK);

    status = FVID2_control(pObj->fvidHandleVipAll,
                           IOCTL_VPS_CAPT_RESET_VIP1, NULL, NULL);
    UTILS_assert(status == FVID2_SOK);

    System_clearVipResetFlag(SYSTEM_VIP_0);
    System_clearVipResetFlag(SYSTEM_VIP_1);

    for (instId = 0; instId < pPrm->numVipInst; instId++)
    {
        CaptureLink_drvCreateInst(pObj, instId);
    }

	//CaptureLink_drvDetectVideo(pObj, FALSE, FALSE, FALSE);		//# for init detect flag

    /* this is effective only if CaptureLink_CreateParams.numExtraBufs > 0 */
    CaptureLink_drvAllocAndQueueExtraFrames(pObj, 0, 0, 0);

#ifdef ENABLE_BLIND_AREA
    //create DMA channel for blind area
    status = Utils_dmaCreateCh(&pObj->dmaObj,
                                   UTILS_DMA_DEFAULT_EVENT_Q,
                                   CAPTURE_LINK_MAX_OUTPUT_PER_INST *
                                   CAPTURE_LINK_MAX_CH_PER_OUT_QUE*
                                   CAPTURE_LINK_NUM_BLIND_AREA_PER_CHNL);
    UTILS_assert(status==FVID2_SOK);
    for(streamId = 0;streamId<CAPTURE_LINK_MAX_OUTPUT_PER_INST;streamId++)
    {
        for(channelId = 0;channelId<CAPTURE_LINK_MAX_CH_PER_OUT_QUE;channelId++)
        {
            pObj->numBlindArea[streamId][channelId] = 0;
        }
    }

    for (instId = 0;instId <
            CAPTURE_LINK_MAX_CH_PER_OUT_QUE*CAPTURE_LINK_MAX_OUTPUT_PER_INST;
                instId ++)
    {
        if(pPrm->blindInfo[instId].numBlindArea != 0)
            CapterLink_drvBlindAreaConfigure(pObj,pPrm->blindInfo[instId].streamId,
                                        pPrm->blindInfo[instId].channelId, &(pPrm->blindInfo[instId]));
    }
#endif

    UTILS_MEMLOG_USED_END(pObj->memUsed);
    UTILS_MEMLOG_PRINT(" CAPTURE", pObj->memUsed, UTILS_ARRAYSIZE(pObj->memUsed));

#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Create Done !!!\n", Utils_getCurTimeInMsec());
#endif

    return status;
}

Int32 CaptureLink_drvGetVideoStatus(CaptureLink_Obj * pObj, VCAP_VIDEO_SOURCE_STATUS_S *pStatus)
{
    UInt32 chId, instId;
    CaptureLink_InstObj *pInst;

    pStatus->numChannels = 0;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];

        for (chId = 0; chId < pInst->createArgs.numCh; chId++)
        {
            pStatus->chStatus[pStatus->numChannels] = pInst->vidDecCurStatus[chId];

            pStatus->numChannels++;
        }
    }

    return 0;
}

Int32 CaptureLink_drvDetectVideo(CaptureLink_Obj * pObj, Bool notifyHostOnChange, Bool printInfo, Bool forceReset)
{
    Vps_VideoDecoderVideoStatusParams videoStatusArgs;
    Vps_VideoDecoderVideoStatus       videoStatus;
    CaptureLink_InstObj *pInst;
    Int32 status = FVID2_SOK;
    UInt32 chId, instId;

    Bool isVideoStatusChanged;
    VCAP_VIDEO_SOURCE_CH_STATUS_S *pVidStatus;
    CaptureLink_VipInstParams *pInstPrm;

    isVideoStatusChanged = FALSE;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];

        for (chId = 0; chId < pInst->createArgs.numCh; chId++)
        {
            pVidStatus = &pInst->vidDecCurStatus[chId];

            videoStatusArgs.channelNum = chId;

            if(pInst->videoDecoderHandle==NULL)
                continue;

            status = FVID2_control(pInst->videoDecoderHandle,
                                   IOCTL_VPS_VIDEO_DECODER_GET_VIDEO_STATUS,
                                   &videoStatusArgs, &videoStatus);
            UTILS_assert(status == FVID2_SOK);

            if(videoStatus.isVideoDetect)
            {
            	if(!pVidStatus->isVideoDetect)
            	{
			    	pInstPrm = &pObj->createArgs.vipInst[instId];
					if (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_PH1100K_DRV ||
						pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_PH3100K_DRV ||
						pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP2440H_DRV)
					{
						if(forceReset) {
							//# sensor size change
							CaptureLink_drvConfigureSensor(&pObj->instObj[instId], (UInt32) BIOS_WAIT_FOREVER);
						}
					}

					isVideoStatusChanged = TRUE;
                	pVidStatus->isVideoDetect = videoStatus.isVideoDetect;

					if(pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_TVP5158_DRV) {
	                    if (videoStatus.frameHeight == 288) {
				            pVidStatus->isPalMode = TRUE;
				        } else {
				        	pVidStatus->isPalMode = FALSE;
				    	}
				    }

					#if 0
					if(pVidStatus->isVideoDetect) {
	                    Vps_printf
	                        (" %d: CAPTURE: Detected video at CH [%d,%d] (%dx%d@%dHz, %s%s)!!!\n",
	                         Utils_getCurTimeInMsec(), instId, chId,
	                         videoStatus.frameWidth, videoStatus.frameHeight,
	                         1000000 / videoStatus.frameInterval, videoStatus.isInterlaced?"I":"P",
	                         pVidStatus->isPalMode?", PAL":"");
	                }
                    #endif
                }
            }
            else
            {
                if(pVidStatus->isVideoDetect)
                {
                	isVideoStatusChanged = TRUE;
					pVidStatus->isVideoDetect = videoStatus.isVideoDetect;

					#if 0
                    Vps_printf(" %d: CAPTURE: No video detected at CH [%d,%d] !!!\n",
                     		Utils_getCurTimeInMsec(), instId, chId);
                    #endif
                }
            }
		}
    }

    if(isVideoStatusChanged && notifyHostOnChange) {
        System_linkControl(SYSTEM_LINK_ID_HOST, VSYS_EVENT_VIDEO_DETECT, NULL, 0, FALSE);
        //CaptureLink_drvOverflowDetectAndReset(pObj, TRUE);
    }

	return status;
}

Int32 CaptureLink_drvDetectSensor(CaptureLink_Obj *pObj)
{
	Int32 status = FVID2_SOK;
    UInt32 instId;
    CaptureLink_InstObj *pInst;
    CaptureLink_VipInstParams *pInstPrm;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
    	if(pObj->detectStop) {
    		return FVID2_SOK;
    	}
		pInstPrm = &pObj->createArgs.vipInst[instId];
		if (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_PH1100K_DRV ||
			pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_PH3100K_DRV ||
			pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP2440H_DRV)
		{
			pInst = &pObj->instObj[instId];
	        if (pInst->videoDecoderHandle == NULL)
	            return FVID2_SOK;

	        status = FVID2_control(pInst->videoDecoderHandle,
	                               IOCTL_VPS_VIDEO_DECODER_SENSOR_DETECT,
	                               NULL, NULL);
    	}
    }

	return status;
}

Int32 CaptureLink_drvConfigureVideoDecoder(CaptureLink_Obj * pObj, UInt32 timeout)
{
    Int32 status;
    UInt32 instId;

    CaptureLink_InstObj *pInst;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];

        if (pInst->videoDecoderHandle == NULL)
            return FVID2_SOK;

        pInst->vidDecVideoModeArgs.videoAutoDetectTimeout = timeout;
        status = FVID2_control(pInst->videoDecoderHandle,
                               IOCTL_VPS_VIDEO_DECODER_SET_VIDEO_MODE,
                               &pInst->vidDecVideoModeArgs, NULL);

        UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

Int32 CaptureLink_drvConfigureSensor(CaptureLink_InstObj *pInst, UInt32 timeout)
{
	Int32 status;

    if (pInst->videoDecoderHandle == NULL)
        return FVID2_SOK;

    pInst->vidDecVideoModeArgs.videoAutoDetectTimeout = timeout;
    status = FVID2_control(pInst->videoDecoderHandle,
                           IOCTL_VPS_VIDEO_DECODER_SET_VIDEO_MODE,
                           &pInst->vidDecVideoModeArgs, NULL);

    return status;
}

Int32 CaptureLink_drvOverflowDetectAndReset(CaptureLink_Obj * pObj,
                                            Bool doForceReset)
{
    Vps_CaptOverFlowStatus overFlowStatus;
    UInt32 curTime;

    FVID2_control(pObj->fvidHandleVipAll,
                  IOCTL_VPS_CAPT_CHECK_OVERFLOW, NULL, &overFlowStatus);

    if (doForceReset)
    {
    	//# skip first 3sec
	    curTime = Utils_getCurTimeInMsec() - pObj->startTime;
	    if (curTime < 3 * 1000) {
	        return FVID2_SOK;
	    }

		overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTA] = TRUE;
		overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTB] = TRUE;
		overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTA] = TRUE;
		overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTB] = TRUE;
    }

    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTB]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTB])
    {
        pObj->resetCount++;
        curTime = Utils_getCurTimeInMsec();
    }

    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTB])
    {
        System_lockVip(SYSTEM_VIP_0);

        Vps_rprintf(" %d: CAPTURE: Overflow detected on VIP0, Total Resets = %d\n",
			Utils_getCurTimeInMsec(), pObj->resetCount);
    }
    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTB])
    {
        System_lockVip(SYSTEM_VIP_1);

        Vps_rprintf(" %d: CAPTURE: Overflow detected on VIP1, Total Resets = %d\n",
			Utils_getCurTimeInMsec(), pObj->resetCount);
    }

    FVID2_control(pObj->fvidHandleVipAll,
                  IOCTL_VPS_CAPT_RESET_AND_RESTART, &overFlowStatus, NULL);

    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTB])
    {
        System_setVipResetFlag(SYSTEM_VIP_0);
        System_unlockVip(SYSTEM_VIP_0);
    }
    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTB])
    {
        System_setVipResetFlag(SYSTEM_VIP_1);
        System_unlockVip(SYSTEM_VIP_1);
    }
    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTB]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTB])
    {
        curTime = Utils_getCurTimeInMsec() - curTime;
        pObj->resetTotalTime += curTime;
    }

    return FVID2_SOK;
}

Int32 CaptureLink_drvPrintRtStatus(CaptureLink_Obj * pObj, UInt32 frameCount,
                                   UInt32 elaspedTime)
{
    UInt32 fps = (frameCount * 100) / (elaspedTime / 10);

    Vps_rprintf
		(" [i] CAPTURE: Fields = %d (fps = %d), Resets = %d (CPU Load = %d)\r\n",
			/*Utils_getCurTimeInMsec(), */frameCount, fps, pObj->resetCount,
			pObj->totalCpuLoad / pObj->cpuLoadCount);

	//System_memPrintHeapStatus();

    return 0;
}

#ifdef ENABLE_BLIND_AREA
Int32 CapterLink_drvBlindAreaConfigure(CaptureLink_Obj * pObj,UInt32 streamId, UInt32 channelId,
                                          CaptureLink_BlindInfo *blindAreaInfo)
{
    UInt32 i;

    pObj->numBlindArea[streamId][channelId] = blindAreaInfo->numBlindArea;

    UTILS_assert(blindAreaInfo->numBlindArea <= CAPTURE_LINK_NUM_BLIND_AREA_PER_CHNL);
    UTILS_assert(channelId <CAPTURE_LINK_MAX_CH_PER_OUT_QUE);

    for(i = 0;i<blindAreaInfo->numBlindArea;i++)
    {
        pObj->blindArea[streamId][channelId][i].startX = blindAreaInfo->win[i].startX;
        pObj->blindArea[streamId][channelId][i].startY = blindAreaInfo->win[i].startY;
        pObj->blindArea[streamId][channelId][i].width = blindAreaInfo->win[i].width;
        pObj->blindArea[streamId][channelId][i].height = blindAreaInfo->win[i].height;
        pObj->blindArea[streamId][channelId][i].fillColorYUYV = blindAreaInfo->fillColorYUYV;
    }

    // if it is the dataFormat we're not support, we should disable the BlindArea here
    if(pObj->blindArea[streamId][channelId][0].dataFormat == FVID2_DF_INVALID)
    {
        pObj->numBlindArea[streamId][channelId] = 0;
    }

    return 0;
}
#endif //ENABLE_BLIND_AREA

Int32 CaptureLink_drvProcessData(CaptureLink_Obj * pObj)
{
    UInt32 frameId, queId, streamId, queChId, elaspedTime;
    UInt_64 cur_time;
    FVID2_FrameList frameList;
    FVID2_Frame *pFrame;
    volatile UInt32 sendMsgToTsk = 0, tmpValue;
    Int32 status=FVID2_SOK;
    System_FrameInfo *pFrameInfo;
	#ifdef ENABLE_BLIND_AREA
    UInt32 blindWin;
	#endif

    //pObj->cbCountServicedCount++;
	System_displayUnderflowCheck(FALSE);

    for (streamId = 0; streamId < CAPTURE_LINK_MAX_OUTPUT_PER_INST; streamId++)
    {
        /* Deque frames for all active handles */
		FVID2_dequeue(pObj->fvidHandleVipAll, &frameList, streamId, BIOS_NO_WAIT);
        if (frameList.numFrames)
        {
        	Utils_get_walltime(&cur_time);
            for (frameId = 0; frameId < frameList.numFrames; frameId++)
            {
                pFrame = frameList.frames[frameId];

                //pFrame->timeStamp = Utils_getCurTimeInMsec();

                queId = CaptureLink_getQueId(pFrame->channelNum);
                queChId = CaptureLink_getQueChId(pFrame->channelNum);

                //UTILS_assert(queId < CAPTURE_LINK_MAX_OUT_QUE);
                //UTILS_assert(queChId < CAPTURE_LINK_MAX_CH_PER_OUT_QUE);

                pObj->captureDequeuedFrameCount++;
                pObj->captureFrameCount[queId][queChId]++;

                tmpValue = (UInt32) pFrame->reserved;
                #if 0
                if (tmpValue > 0) {
                    Vps_printf (" %d: CAPTURE: Dequeued frame more than once (%d,%d, %08x) \n",
                         Utils_getCurTimeInMsec(), queId, queChId, pFrame->addr[0][0]);
                }
                #endif
                tmpValue++;
                pFrame->reserved = (Ptr) tmpValue;
                pFrame->perFrameCfg = NULL;
                pFrame->channelNum = queChId;

                pFrameInfo = (System_FrameInfo *) pFrame->appData;
                pFrameInfo->ts64 = cur_time;

                sendMsgToTsk |= (1 << queId);

				#ifdef ENABLE_BLIND_AREA
                //add blind area mask process here.
                if(pObj->numBlindArea[queId][queChId] != 0)
                {
                    for(blindWin = 0;blindWin < pObj->numBlindArea[queId][queChId];blindWin ++)
                    {
                        pObj->blindArea[queId][queChId][blindWin].destAddr[0] =
                                    pFrame->addr[0][0];
                        pObj->blindArea[queId][queChId][blindWin].destAddr[1] =
                                    pFrame->addr[0][1];
                    }
                    status = Utils_dmaFill2D(&pObj->dmaObj, pObj->blindArea[queId][queChId], pObj->numBlindArea[queId][queChId]);
                }
				#endif

                status = Utils_bufPutFullFrame(&pObj->bufQue[queId], pFrame);
                //UTILS_assert(status == FVID2_SOK);
            }

			#ifdef SYSTEM_DEBUG_CAPTURE_RT
            Vps_printf(" %d: CAPTURE: Dequeued %d frames !!!\n",
                       Utils_getCurTimeInMsec(), frameList.numFrames);
			#endif
        }
    }

	elaspedTime = Utils_getCurTimeInMsec() - pObj->startTime;
	#ifdef SYS_DEBUG
    if ((elaspedTime - pObj->prevTime) > 60 * 1000)
    {
        CaptureLink_getCpuLoad();
        CaptureLink_drvPrintRtStatus(pObj,
						pObj->captureDequeuedFrameCount - pObj->prevFrameCount,
						elaspedTime - pObj->prevTime);

        pObj->prevFrameCount = pObj->captureDequeuedFrameCount;
        pObj->prevTime = elaspedTime;
    }
    #endif

    for (queId = 0; queId < CAPTURE_LINK_MAX_OUT_QUE; queId++)
    {
        if (sendMsgToTsk & 0x1) {
            /* send data available message to next tsk */
			System_sendLinkCmd(pObj->createArgs.outQueParams[queId].nextLink, SYSTEM_CMD_NEW_DATA);
        }

        sendMsgToTsk >>= 1;
        if (sendMsgToTsk == 0)
            break;
    }

    if (pObj->enableCheckOverflowDetect) {
        CaptureLink_drvOverflowDetectAndReset(pObj, FALSE);
    }

    if (pObj->en_videodet)		//# for video detect
    {
	    if ((elaspedTime - pObj->prevVideoDetectCheckTime) > CAPTURE_VIDEO_DETECT_CHECK_INTERVAL)
	    {
	        CaptureLink_drvDetectVideo(pObj, TRUE, FALSE, TRUE);
	        pObj->prevVideoDetectCheckTime = elaspedTime;
	    }
	}

    pObj->exeTime = Utils_getCurTimeInMsec() - pObj->startTime;

    return status;//FVID2_SOK;
}

Int32 CaptureLink_drvPutEmptyFrames(CaptureLink_Obj * pObj, FVID2_FrameList * pFrameList)
{
    UInt32 frameId;
    FVID2_Frame *pFrame;
    System_FrameInfo *pFrameInfo;
    volatile UInt32 tmpValue;

    if (pFrameList->numFrames)
    {
        for (frameId = 0; frameId < pFrameList->numFrames; frameId++)
        {
            pFrame = pFrameList->frames[frameId];

            tmpValue = (UInt32) pFrame->reserved;
            tmpValue--;
            pFrame->reserved = (Ptr) tmpValue;

            pFrameInfo = (System_FrameInfo *) pFrame->appData;
            UTILS_assert(pFrameInfo != NULL);

            pFrame->perFrameCfg = &pFrameInfo->captureRtParams;

            if(pFrameInfo->captureChannelNum == CAPTURE_LINK_EXTRA_FRAME_CH_ID)
            {
                UInt32 cookie;

                /* this frame is from extra buffer pool,
                    make it go to the current CH ID which need extra buffers at this moment

                    since pObj->extraFrameObj.captureChannelNum could get
                    updated at run-time in a different thread
                    protect this via interrupt lock
                */

                cookie = Hwi_disable();

                pFrame->channelNum = pObj->extraFrameObj.captureChannelNum;

                Hwi_restore(cookie);
            }
            else
            {
                pFrame->channelNum = pFrameInfo->captureChannelNum;
            }
        }

		#ifdef SYSTEM_DEBUG_CAPTURE_RT
        Vps_printf(" %d: CAPTURE: Queued back %d frames !!!\n",
                   Utils_getCurTimeInMsec(), pFrameList->numFrames);
		#endif

        pObj->captureQueuedFrameCount += pFrameList->numFrames;

        FVID2_queue(pObj->fvidHandleVipAll, pFrameList, VPS_CAPT_STREAM_ID_ANY);
    }

    return FVID2_SOK;
}

Void CaptureLink_drvSetFrameCropBufPtr(CaptureLink_Obj * pObj,
                                       FVID2_FrameList * pFrameList)
{
    if (pObj->createArgs.enableSdCrop
        &&
        pObj->createArgs.doCropInCapture
        &&
        (pObj->maxWidth == 704))
    {
        Int i;
        FVID2_Frame *pFrame;

        for (i = 0; i < pFrameList->numFrames; i++)
        {
            pFrame = pFrameList->frames[i];
            pFrame->addr[0][0] = (Ptr)((UInt32)pFrame->addr[0][0] +
                                       (CAPTURE_LINK_SD_CROP_LEN_PIXELS_LEFTMARGIN * 2));
        }
    }
}

Void CaptureLink_drvReSetFrameCropBufPtr(CaptureLink_Obj * pObj,
                                         FVID2_FrameList * pFrameList)
{
    if (pObj->createArgs.enableSdCrop
        &&
        pObj->createArgs.doCropInCapture
        &&
        (pObj->maxWidth == 704))
    {
        Int i;
        FVID2_Frame *pFrame;

        for (i = 0; i < pFrameList->numFrames; i++)
        {
            pFrame = pFrameList->frames[i];
            pFrame->addr[0][0] = (Ptr)((UInt32)pFrame->addr[0][0] -
                                       (CAPTURE_LINK_SD_CROP_LEN_PIXELS_LEFTMARGIN * 2));
        }
    }
}

Int32 CaptureLink_drvStart(CaptureLink_Obj * pObj)
{
    UInt32 instId;
    CaptureLink_InstObj *pInstObj;

	#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Start in progress !!!\n", Utils_getCurTimeInMsec());
	#endif

	CaptureLink_drvOverflowDetectAndReset(pObj, TRUE);	//# phoong - for complement

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* video decoder */
        if (pInstObj->videoDecoderHandle)
            FVID2_start(pInstObj->videoDecoderHandle, NULL);

    }

	#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Start Done !!!\n", Utils_getCurTimeInMsec());
	#endif

    //Task_sleep(100);		//# why?

	pObj->prevVideoDetectCheckTime = pObj->prevTime =
            pObj->startTime = Utils_getCurTimeInMsec();

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* VIP capture */
        FVID2_start(pInstObj->captureVipHandle, NULL);
    }

	//# start sensor detect task
	Utils_tskSendCmd(&pObj->tsk_det, SYSTEM_CMD_CREATE);

    return FVID2_SOK;
}

Int32 CaptureLink_drvFlush(FVID2_Handle captureVipHandle, char *portName)
{
    Int32 status;
    FVID2_FrameList frameList;

    do
    {
        status = FVID2_control(captureVipHandle,
                               IOCTL_VPS_CAPT_FLUSH, NULL, &frameList);

		#ifdef SYSTEM_DEBUG_CAPTURE_RT
        Vps_rprintf(" %d: CAPTURE: %s: Flushed %d frames.\n",
                    Utils_getCurTimeInMsec(), portName, frameList.numFrames);
		#endif

    } while (frameList.numFrames != 0 && status == FVID2_SOK);

    if (status != FVID2_SOK)
    {
		#ifdef SYSTEM_DEBUG_CAPTURE_RT
        Vps_rprintf(" %d: CAPTURE: %s: Flushing ... ERROR !!!\n",
                    Utils_getCurTimeInMsec(), portName);
		#endif
    }

    return FVID2_SOK;
}

Int32 CaptureLink_drvStop(CaptureLink_Obj * pObj)
{
    UInt32 instId;
    CaptureLink_InstObj *pInstObj;

	//# stop sensor detect task
	pObj->detectStop = TRUE;
	Task_sleep(100);		//# wait stop detect

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* VIP capture */
        FVID2_stop(pInstObj->captureVipHandle, NULL);

        CaptureLink_drvFlush(pInstObj->captureVipHandle,
                             gCaptureLink_portName[pInstObj->instId]);
    }

    pObj->exeTime = Utils_getCurTimeInMsec() - pObj->startTime;

	#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Stop in progress !!!\n", Utils_getCurTimeInMsec());
	#endif

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* video decoder */
        if (pInstObj->videoDecoderHandle)
            FVID2_stop(pInstObj->videoDecoderHandle, NULL);
    }

	#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Stop Done !!!\n", Utils_getCurTimeInMsec());
	#endif

    return FVID2_SOK;
}

Int32 CaptureLink_drvDelete(CaptureLink_Obj * pObj)
{
    UInt32 instId;
    UInt32 queId;
    CaptureLink_InstObj *pInstObj;
    Int32 status = FVID2_SOK;

	#ifdef SYSTEM_DEBUG_CAPTURE
    CaptureLink_drvPrintStatus(pObj);
	#endif

	#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Delete in progress !!!\n", Utils_getCurTimeInMsec());
	#endif

    if (CAPTURE_LINK_TMP_BUF_SIZE)
    {
        Utils_memFree(pObj->tmpBufAddr, CAPTURE_LINK_TMP_BUF_SIZE);
    }

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* VIP capture */
        FVID2_delete(pInstObj->captureVipHandle, NULL);

        if (pInstObj->videoDecoderHandle)
        {
            /* video decoder */
            FVID2_delete(pInstObj->videoDecoderHandle, NULL);
        }

        CaptureLink_drvFreeFrames(pObj, pInstObj, instId);
    }

    /* this is effective only if CaptureLink_CreateParams.numExtraBufs > 0 */
    CaptureLink_drvFreeExtraFrames(pObj);

    status = FVID2_delete(pObj->fvidHandleVipAll, NULL);

    for (queId = 0; queId < CAPTURE_LINK_MAX_OUT_QUE; queId++)
    {
        Utils_bufDelete(&pObj->bufQue[queId]);
    }

	#ifdef ENABLE_BLIND_AREA
    status = Utils_dmaDeleteCh(&pObj->dmaObj);
    UTILS_assert(status==FVID2_SOK);
	#endif

	#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Delete Done !!!\n", Utils_getCurTimeInMsec());
	#endif

    return status;
}

Int32 CaptureLink_drvSetExtraFramesChId(CaptureLink_Obj * pObj, CaptureLink_ExtraFramesChId *pPrm)
{
    UInt32 cookie;

    /* check limits of queId and chId */
    if(pPrm->queId > CAPTURE_LINK_MAX_OUT_QUE)
        return FVID2_EFAIL;

    if(pPrm->chId > CAPTURE_LINK_MAX_CH_PER_OUT_QUE)
        return FVID2_EFAIL;

    /* since pObj->extraFrameObj.captureChannelNum could get
        updated at run-time in a different thread
        protect this via interrupt lock
    */

    cookie = Hwi_disable();

    pObj->extraFrameObj.captureChannelNum =
        CaptureLink_makeChannelNum(pPrm->queId, pPrm->chId);
        ;

    Hwi_restore(cookie);

    return FVID2_SOK;
}

Int32 CaptureLink_drvAllocAndQueueExtraFrames(CaptureLink_Obj * pObj, UInt32 instId, UInt32 streamId, UInt32 chId)
{
    Int32 status;
    UInt16 frameId;
    Vps_CaptOutInfo *pOutInfo;
    FVID2_Frame *frames;
    System_FrameInfo *pFrameInfo;
    FVID2_FrameList frameList;
    FVID2_Format *pFormat;
    Int i,j;
    CaptureLink_ExtraFrameObj *pExtraFrameObj;
    Vps_CaptCreateParams *pVipCreateArgs;

    if(pObj->createArgs.numExtraBufs == 0)
        return 0; /* no need to alocate extra buffers */

    /* limit numExtraBufs to CAPTURE_LINK_MAX_FRAMES_PER_CH */
    if(pObj->createArgs.numExtraBufs > CAPTURE_LINK_MAX_FRAMES_PER_CH)
        pObj->createArgs.numExtraBufs = CAPTURE_LINK_MAX_FRAMES_PER_CH;

    pExtraFrameObj = &pObj->extraFrameObj;

    pFormat = &pExtraFrameObj->format;

    /* extra frames allocated using instId, streamId , chId info */

    pVipCreateArgs = &pObj->instObj[instId].createArgs;

    pExtraFrameObj->captureChannelNum = pVipCreateArgs->channelNumMap[streamId][chId];

    pOutInfo = &pVipCreateArgs->outStreamInfo[streamId];

    pExtraFrameObj->tilerUsed =
        CaptureLink_drvIsDataFormatTiled(pVipCreateArgs, streamId);

    pFrameInfo = &pExtraFrameObj->frameInfo[0];
    frames = &pExtraFrameObj->frames[0];


    /* fill format with channel specific values */

    pFormat->channelNum =
        pExtraFrameObj->captureChannelNum;

    pFormat->width = pObj->maxWidth;
    pFormat->height = pObj->maxHeight;

    if(pFormat->height < CAPTURE_LINK_HEIGHT_MIN_LINES)
        pFormat->height = CAPTURE_LINK_HEIGHT_MIN_LINES;

    pFormat->pitch[0] = pOutInfo->pitch[0];
    pFormat->pitch[1] = pOutInfo->pitch[1];
    pFormat->pitch[2] = pOutInfo->pitch[2];
    pFormat->fieldMerged[0] = FALSE;
    pFormat->fieldMerged[1] = FALSE;
    pFormat->fieldMerged[2] = FALSE;
    pFormat->dataFormat = pOutInfo->dataFormat;
    pFormat->scanFormat = FVID2_SF_PROGRESSIVE;
    pFormat->bpp = FVID2_BPP_BITS8;                  /* ignored */

    if (pFormat->dataFormat == FVID2_DF_RAW_VBI)
    {
        pFormat->height = CAPTURE_LINK_RAW_VBI_LINES;
    }

    /*
     * alloc memory based on 'format'
     * Allocated frame info is put in frames[]
     * numBufsPerCh is the number of buffers per channel to
     * allocate
     */
    if (pExtraFrameObj->tilerUsed)
    {
        Utils_tilerFrameAlloc(pFormat, frames,
                              pObj->createArgs.numExtraBufs);
    }
    else
    {
        /**
        Vps_printf("CAPTURE:ALLOCINFO:FMT[%d]/PITCH[%d]/HEIGHT[%d]/NUMBUFS[%d]",
                   pFormat->dataFormat,
                   pFormat->pitch[0],
                   pFormat->height,
                   pObj->createArgs.numExtraBufs);
        */
        Utils_memFrameAlloc(pFormat, frames,
                            pObj->createArgs.numExtraBufs);
    }

    /*
     * Set rtParams for every frame in perFrameCfg
     */
    for (frameId = 0; frameId < pObj->createArgs.numExtraBufs; frameId++)
    {
        frames[frameId].perFrameCfg =
            &pFrameInfo[frameId].captureRtParams;
        frames[frameId].subFrameInfo = NULL;
        frames[frameId].appData = &pFrameInfo[frameId];
        frames[frameId].reserved = NULL;
        for (i = 0; i < FVID2_MAX_FIELDS; i++)
        {
            for (j = 0; j < FVID2_MAX_PLANES ; j++)
            {
                pExtraFrameObj->origAddr[frameId][i][j] =
                    frames[frameId].addr[i][j];
            }
        }
        pFrameInfo[frameId].captureChannelNum = CAPTURE_LINK_EXTRA_FRAME_CH_ID;
        pFrameInfo[frameId].rtChInfoUpdate = FALSE;

        frameList.frames[frameId] = &frames[frameId];

		#ifdef SYSTEM_VERBOSE_PRINTS
		Vps_rprintf(" %d: CAPTURE: %d: 0x%08x, %d x %d, %08x B --> Extra Frames \n",
                        Utils_getCurTimeInMsec(),
                        frameId, frames[frameId].addr[0][0],
                        pFormat->pitch[0] / 2, pFormat->height,
                        pFormat->height * pFormat->pitch[0]);
		#endif
    }

    /*
     * Set number of frame in frame list
     */
    frameList.numFrames = pObj->createArgs.numExtraBufs;

    /*
     * queue the frames in frameList
     * All allocate frames are queued here as an example.
     * In general atleast 2 frames per channel need to queued
     * before starting capture,
     * else frame will get dropped until frames are queued
     */
    status =
        FVID2_queue(pObj->fvidHandleVipAll, &frameList, VPS_CAPT_STREAM_ID_ANY);

    UTILS_assert(status == FVID2_SOK);
    pObj->captureQueuedFrameCount += frameList.numFrames;

    return status;
}

Int32 CaptureLink_drvFreeExtraFrames(CaptureLink_Obj * pObj)
{
    FVID2_Frame *pFrames;
    Int i,j;
    CaptureLink_ExtraFrameObj *pExtraFrameObj;

    if(pObj->createArgs.numExtraBufs == 0)
        return 0; /* no need to free extra buffers */

    pExtraFrameObj = &pObj->extraFrameObj;

    pFrames = &pExtraFrameObj->frames[0];

    for (i = 0; i < FVID2_MAX_FIELDS; i++)
    {
        for (j = 0; j < FVID2_MAX_PLANES ; j++)
        {
            if (pExtraFrameObj->frames[0].addr[i][j] !=
                pExtraFrameObj->origAddr[0][i][j])
            {
                Vps_printf("CAPTURELINK: !!WARN. Mismatch FrameAddr:%p,OrigAddr:%p\n",
                           pExtraFrameObj->frames[0].addr[i][j], pExtraFrameObj->origAddr[0][i][j]);
                pExtraFrameObj->frames[0].addr[i][j] = pExtraFrameObj->origAddr[0][i][j];
            }
        }
    }


    if(pExtraFrameObj->tilerUsed)
    {
        SystemTiler_freeAll();
    }
    else
    {
        /*
         * free frames for this channel, based on pFormat
         */
        Utils_memFrameFree(&pExtraFrameObj->format, pFrames,
                           pObj->createArgs.numExtraBufs);
    }

    return FVID2_SOK;
}


/*
 * Allocate and queue frames to driver
 *
 * pDrvObj - capture driver information */
Int32 CaptureLink_drvAllocAndQueueFrames(CaptureLink_Obj * pObj,
                                         CaptureLink_InstObj * pDrvObj)
{
    Int32 status;
    UInt16 streamId, chId, frameId, idx;
    Vps_CaptOutInfo *pOutInfo;
    FVID2_Frame *frames;
    System_FrameInfo *pFrameInfo;
    FVID2_FrameList frameList;
    FVID2_Format format;
    Int i,j;
    UInt32 numBufsPerCh;

    /*
     * init frameList for list of frames that are queued per CH to driver
     */
    frameList.perListCfg = NULL;
    frameList.reserved = NULL;

    if (CAPTURE_LINK_NUM_BUFS_PER_CH_DEFAULT ==
        pObj->createArgs.numBufsPerCh)
    {
        pObj->createArgs.numBufsPerCh = CAPTURE_LINK_FRAMES_PER_CH;
    }
    else
    {
        /* Bound the numBufsPerCh between min and max supported */
        if (pObj->createArgs.numBufsPerCh < CAPTURE_LINK_MIN_FRAMES_PER_CH)
        {
            Vps_printf("CAPTURE:Create args numBufsPerCh[%d] < min[%d]"
                       "Overriding create args",
                       pObj->createArgs.numBufsPerCh,
                       CAPTURE_LINK_MIN_FRAMES_PER_CH);
            pObj->createArgs.numBufsPerCh = CAPTURE_LINK_MIN_FRAMES_PER_CH;

        }
        else
        {
            if (pObj->createArgs.numBufsPerCh > CAPTURE_LINK_MAX_FRAMES_PER_CH)
            {
                Vps_printf("CAPTURE:Create args numBufsPerCh[%d] > max[%d]"
                           "Overriding create args",
                           pObj->createArgs.numBufsPerCh,
                           CAPTURE_LINK_MAX_FRAMES_PER_CH);
                pObj->createArgs.numBufsPerCh = CAPTURE_LINK_MAX_FRAMES_PER_CH;
            }
        }
    }

    numBufsPerCh = pObj->createArgs.numBufsPerCh;

    /*
     * for every stream and channel in a capture handle
     */
    for (streamId = 0; streamId < pDrvObj->createArgs.numStream; streamId++)
    {
        for (chId = 0; chId < pDrvObj->createArgs.numCh; chId++)
        {

            pOutInfo = &pDrvObj->createArgs.outStreamInfo[streamId];

            /*
             * base index for pDrvObj->frames[] and pDrvObj->frameInfo[]
             */
            idx =
                VPS_CAPT_CH_PER_PORT_MAX * numBufsPerCh *
                streamId + numBufsPerCh * chId;
            if (idx >= CAPTURE_LINK_MAX_FRAMES_PER_HANDLE)
            {
                idx = 0u;
            }

            pFrameInfo = &pDrvObj->frameInfo[idx];
            frames = &pDrvObj->frames[idx];

            /* fill format with channel specific values */
            format.channelNum =
                pDrvObj->createArgs.channelNumMap[streamId][chId];
            format.width = pObj->maxWidth;
            format.height = pObj->maxHeight;

            if(format.height < CAPTURE_LINK_HEIGHT_MIN_LINES)
                format.height = CAPTURE_LINK_HEIGHT_MIN_LINES;

            format.pitch[0] = pOutInfo->pitch[0];
            format.pitch[1] = pOutInfo->pitch[1];
            format.pitch[2] = pOutInfo->pitch[2];
            format.fieldMerged[0] = FALSE;
            format.fieldMerged[1] = FALSE;
            format.fieldMerged[2] = FALSE;
            format.dataFormat = pOutInfo->dataFormat;
            format.scanFormat = FVID2_SF_PROGRESSIVE;
            format.bpp = FVID2_BPP_BITS8;                  /* ignored */

            if (format.dataFormat == FVID2_DF_RAW_VBI)
            {
                format.height = CAPTURE_LINK_RAW_VBI_LINES;
            }

            /*
             * alloc memory based on 'format'
             * Allocated frame info is put in frames[]
             * CAPTURE_LINK_APP_FRAMES_PER_CH is the number of buffers per channel to
             * allocate
             */
            if (CaptureLink_drvIsDataFormatTiled
                (&pDrvObj->createArgs, streamId))
            {
                status = Utils_tilerFrameAlloc(&format, frames,
                                      numBufsPerCh);
            }
            else
            {
                /**
                Vps_printf("CAPTURE:ALLOCINFO:FMT[%d]/PITCH[%d]/HEIGHT[%d]/NUMBUFS[%d]",
                           format.dataFormat,
                           format.pitch[0],
                           format.height,
                           numBufsPerCh);
                */
                status = Utils_memFrameAlloc(&format, frames,
                                    numBufsPerCh);
            }
            UTILS_assert(status == FVID2_SOK);

            /*
             * Set rtParams for every frame in perFrameCfg
             */
            for (frameId = 0; frameId < numBufsPerCh; frameId++)
            {
                frames[frameId].perFrameCfg =
                    &pFrameInfo[frameId].captureRtParams;
                frames[frameId].subFrameInfo = NULL;
                frames[frameId].appData = &pFrameInfo[frameId];
                frames[frameId].reserved = NULL;
                for (i = 0; i < FVID2_MAX_FIELDS; i++)
                {
                    for (j = 0; j < FVID2_MAX_PLANES ; j++)
                    {
                        pDrvObj->origAddr[idx + frameId][i][j] =
                            frames[frameId].addr[i][j];
                    }
                }
                pFrameInfo[frameId].captureChannelNum =
                    frames[frameId].channelNum;
                pFrameInfo[frameId].rtChInfoUpdate = FALSE;

                frameList.frames[frameId] = &frames[frameId];

				#ifdef SYSTEM_VERBOSE_PRINTS
                if (/*pDrvObj->instId == 0 && streamId == 0 &&*/ chId == 0)
                {
                    Vps_rprintf(" %d: CAPTURE: %d: 0x%08x, %d x %d, %08x B\n",
                                Utils_getCurTimeInMsec(),
                                frameId, frames[frameId].addr[0][0],
                                format.pitch[0] / 2, format.height,
                                format.height * format.pitch[0]);
                }
				#endif
            }

            #ifdef SYSTEM_VERBOSE_PRINTS
            if (pDrvObj->instId == 0 && streamId == 0 && chId == 0)
            {
                Vps_rprintf(" %d: CAPTURE: 0x%08x %08x B\n",
                            Utils_getCurTimeInMsec(),
                            frames[0].addr[0][0],
                            format.height * format.pitch[0] * frameId);
            }
			#endif

            /*
             * Set number of frame in frame list
             */
            frameList.numFrames = numBufsPerCh;

            /*
             * queue the frames in frameList
             * All allocate frames are queued here as an example.
             * In general atleast 2 frames per channel need to queued
             * before starting capture,
             * else frame will get dropped until frames are queued
             */
            status =
                FVID2_queue(pDrvObj->captureVipHandle, &frameList, streamId);
            UTILS_assert(status == FVID2_SOK);
            pObj->captureQueuedFrameCount += frameList.numFrames;
        }
    }

    return FVID2_SOK;
}

/*
 * Free allocated frames
 *
 * pDrvObj - capture driver information */
Int32 CaptureLink_drvFreeFrames(CaptureLink_Obj * pObj,
                                CaptureLink_InstObj * pDrvObj, int instId)
{
    UInt32 idx;
    UInt16 streamId, chId;
    FVID2_Format format;
    FVID2_Frame *pFrames;
    Vps_CaptOutInfo *pOutInfo;
    UInt32 tilerUsed = FALSE;
    Int i,j;
    UInt32 numBufsPerCh;

    numBufsPerCh = pObj->createArgs.numBufsPerCh;

    for (streamId = 0; streamId < pDrvObj->createArgs.numStream; streamId++)
    {
        for (chId = 0; chId < pDrvObj->createArgs.numCh; chId++)
        {
            pOutInfo = &pDrvObj->createArgs.outStreamInfo[streamId];

            idx = VPS_CAPT_CH_PER_PORT_MAX *
                numBufsPerCh * streamId +
                numBufsPerCh * chId;

            if (idx >= CAPTURE_LINK_MAX_FRAMES_PER_HANDLE)
            {
                idx = 0u;
            }

            pFrames = &pDrvObj->frames[idx];

			if (!pObj->createArgs.tilerEnable)	//# phoong
			{
				for (i = 0; i < FVID2_MAX_FIELDS; i++)
	            {
	                for (j = 0; j < FVID2_MAX_PLANES ; j++)
	                {
	                    if (pDrvObj->frames[idx].addr[i][j] !=
	                        pDrvObj->origAddr[idx][i][j])
	                    {
	                        Vps_printf("CAPTURELINK: !!WARN. Mismatch FrameAddr:%p,OrigAddr:%p\n",
	                                   pDrvObj->frames[idx].addr[i][j], pDrvObj->origAddr[idx][i][j]);
	                        pDrvObj->frames[idx].addr[i][j] = pDrvObj->origAddr[idx][i][j];
	                    }
	                }
	            }
	        }

            /* fill format with channel specific values */
            format.channelNum =
                pDrvObj->createArgs.channelNumMap[streamId][chId];
            //format.width = pObj->maxWidth;
            //format.height = pObj->maxHeight;
            format.width = pObj->inWidth[instId];
            format.height = pObj->inHeight[instId];

            if(format.height < CAPTURE_LINK_HEIGHT_MIN_LINES)
                format.height = CAPTURE_LINK_HEIGHT_MIN_LINES;

            format.pitch[0] = pOutInfo->pitch[0];
            format.pitch[1] = pOutInfo->pitch[1];
            format.pitch[2] = pOutInfo->pitch[2];
            format.fieldMerged[0] = FALSE;
            format.fieldMerged[1] = FALSE;
            format.fieldMerged[2] = FALSE;
            format.dataFormat = pOutInfo->dataFormat;
            format.scanFormat = FVID2_SF_PROGRESSIVE;
            format.bpp = FVID2_BPP_BITS8;                  /* ignored */

            if (format.dataFormat == FVID2_DF_RAW_VBI)
            {
                format.height = CAPTURE_LINK_RAW_VBI_LINES;
            }

            if (CaptureLink_drvIsDataFormatTiled
                (&pDrvObj->createArgs, streamId))
            {
                /* cannot free tiled frames */
                tilerUsed = TRUE;
            }
            else
            {
                /*
                 * free frames for this channel, based on pFormat
                 */
                Utils_memFrameFree(&format, pFrames,
                                   numBufsPerCh);
            }
        }
    }

    if (tilerUsed)
    {
        SystemTiler_freeAll();
    }

    return FVID2_SOK;
}

UInt32 CaptureLink_drvIsDataFormatTiled(Vps_CaptCreateParams * createArgs,
                                        UInt16 streamId)
{
    Vps_CaptOutInfo *pOutInfo;

    pOutInfo = &createArgs->outStreamInfo[streamId];

    if ((pOutInfo->dataFormat == FVID2_DF_YUV420SP_UV ||
         pOutInfo->dataFormat == FVID2_DF_YUV422SP_UV)
        && pOutInfo->memType == VPS_VPDMA_MT_TILEDMEM)
    {
        return TRUE;
    }

    return FALSE;
}

/*
 * Init create arguments to default values
 *
 * createArgs - create arguments */
Int32 CaptureLink_drvInitCreateArgs(Vps_CaptCreateParams * createArgs)
{
    UInt16 chId, streamId;
    Vps_CaptOutInfo *pOutInfo;
    Vps_CaptScParams *pScParams;

    memset(createArgs, 0, sizeof(*createArgs));

    createArgs->videoCaptureMode =
        VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;

    createArgs->videoIfMode = VPS_CAPT_VIDEO_IF_MODE_8BIT;

    createArgs->inDataFormat = FVID2_DF_YUV422P;
    createArgs->periodicCallbackEnable = FALSE;
    createArgs->numCh = 1;
    createArgs->numStream = 1;

    createArgs->vipParserInstConfig = NULL;
    createArgs->vipParserPortConfig = NULL;
    createArgs->cscConfig = NULL;

    pScParams = &createArgs->scParams;

    pScParams->inScanFormat = FVID2_SF_PROGRESSIVE;
    pScParams->inWidth = 720;
    pScParams->inHeight = 480;
    pScParams->inCropCfg.cropStartX = 0;
    pScParams->inCropCfg.cropStartY = 0;
    pScParams->inCropCfg.cropWidth = pScParams->inWidth;
    pScParams->inCropCfg.cropHeight = pScParams->inHeight;
    pScParams->outWidth = pScParams->inWidth;
    pScParams->outHeight = pScParams->inHeight;
    pScParams->scConfig = NULL;
    pScParams->scCoeffConfig = NULL;

    for (streamId = 0; streamId < VPS_CAPT_STREAM_ID_MAX; streamId++)
    {
        pOutInfo = &createArgs->outStreamInfo[streamId];

        pOutInfo->memType = VPS_VPDMA_MT_NONTILEDMEM;
        pOutInfo->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_UNLIMITED;

        pOutInfo->dataFormat = FVID2_DF_INVALID;

        pOutInfo->scEnable = FALSE;
        pOutInfo->subFrameModeEnable = FALSE;
        pOutInfo->numLinesInSubFrame = 0;
        pOutInfo->subFrameCb = NULL;

        if (streamId == 0)
        {
            pOutInfo->dataFormat = FVID2_DF_YUV422I_YUYV;
        }

        for (chId = 0; chId < VPS_CAPT_CH_PER_PORT_MAX; chId++)
        {
            createArgs->channelNumMap[streamId][chId] =
                Vps_captMakeChannelNum(0, streamId, chId);
        }
    }

    return 0;
}

Int32 CaptureLink_getCpuLoad()
{
    gCaptureLink_obj.totalCpuLoad += Load_getCPULoad();
    gCaptureLink_obj.cpuLoadCount++;

    return 0;
}

Int32 CaptureLink_drvPrintStatus(CaptureLink_Obj * pObj)
{
    UInt32 fps;

    FVID2_control(pObj->fvidHandleVipAll,
                  IOCTL_VPS_CAPT_PRINT_ADV_STATISTICS,
                  (Ptr) pObj->exeTime, NULL);

    fps = (pObj->captureDequeuedFrameCount * 100) / (pObj->exeTime / 10);

    Vps_printf(" %d: CAPTURE: Fields = %d (fps = %d, CPU Load = %d)\r\n",
               Utils_getCurTimeInMsec(),
               pObj->captureDequeuedFrameCount,
               fps, pObj->totalCpuLoad / pObj->cpuLoadCount);

    Vps_printf(" %d: CAPTURE: Num Resets = %d (Avg %d ms per reset)\r\n",
               Utils_getCurTimeInMsec(),
               pObj->resetCount, pObj->resetTotalTime / pObj->resetCount);

    System_memPrintHeapStatus();

    return 0;
}

Int32 CaptureLink_drvDetectVideoStandard(CaptureLink_Obj * pObj)
{
    CaptureLink_VipInstParams *pInstPrm;

    Vps_VideoDecoderVideoStatusParams videoStatusArgs;
    Vps_VideoDecoderVideoStatus videoStatus;
    Vps_VideoDecoderCreateParams vidDecCreateArgs;
    Vps_VideoDecoderCreateStatus vidDecCreateStatus;

    FVID2_Handle videoDecoderHandle;
    Int32 status, instId=0;

    pInstPrm = &pObj->createArgs.vipInst[instId];

    if (pInstPrm->videoDecoderId != FVID2_VPS_VID_DEC_TVP5158_DRV)
    {
        /* auto-detect only supported for TVP5158 as of now */
        return FVID2_SOK;
    }

    /* detecting standard at VIP0 - Port A - CH0 ONLY - assuming same is used
     * for all CHs */

    vidDecCreateArgs.deviceI2cInstId = Vps_platformGetI2cInstId();
    vidDecCreateArgs.numDevicesAtPort = 1;
    vidDecCreateArgs.deviceI2cAddr[0] = Vps_platformGetVidDecI2cAddr(pInstPrm->videoDecoderId, pInstPrm->vipInstId);
    vidDecCreateArgs.deviceResetGpio[0] = VPS_VIDEO_DECODER_GPIO_NONE;

    videoDecoderHandle = FVID2_create(pInstPrm->videoDecoderId,
                                      0,
                                      &vidDecCreateArgs,
                                      &vidDecCreateStatus, NULL);

    UTILS_assert(videoDecoderHandle != NULL);

	#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf
        (" %d: CAPTURE: VIP %d: VID DEC %d (0x%02x): Video Standard Detect in Progress !!!\n",
         Utils_getCurTimeInMsec(), pInstPrm->vipInstId, pInstPrm->videoDecoderId,
         vidDecCreateArgs.deviceI2cAddr[0]);
	#endif

    videoStatusArgs.channelNum = 0;
    status = FVID2_control(videoDecoderHandle,
                               IOCTL_VPS_VIDEO_DECODER_GET_VIDEO_STATUS,
                               &videoStatusArgs, &videoStatus);
	UTILS_assert(status == FVID2_SOK);

    if (videoStatus.isVideoDetect)
    {
		#ifdef SYSTEM_DEBUG_CAPTURE
        Vps_printf(" %d: CAPTURE: Detected video (%dx%d@%dHz, %d)!!!\n",
                   Utils_getCurTimeInMsec(),
                   videoStatus.frameWidth,
                   videoStatus.frameHeight,
                   1000000 / videoStatus.frameInterval,
                   videoStatus.isInterlaced);
		#endif

        if (videoStatus.frameHeight == 288)
            pObj->isPalMode = TRUE;

    }

    FVID2_delete(videoDecoderHandle, NULL);

    return FVID2_SOK;
}

Int32 CaptureLink_drvSetColor(CaptureLink_Obj *pObj, Int32 contrast,
                              Int32 brightness, Int32 saturation, Int32 chId)
{
    CaptureLink_VipInstParams *pInstPrm;
    CaptureLink_InstObj *pInst;
    Vps_VideoDecoderColorParams colorPrm;
    Int32 instId, status = FVID2_SOK;

	//# serarch first 5158/6124 instId
	for(instId=0; instId<pObj->createArgs.numVipInst; instId++) {
		pInstPrm = &pObj->createArgs.vipInst[instId];
		if( (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_TVP5158_DRV)
			|| (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP6124B_DRV) ) {
			break;
		}
	}
	instId += chId/4;		//# set instId

	pInstPrm = &pObj->createArgs.vipInst[instId];

	if (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_TVP5158_DRV
			|| pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP6124B_DRV)
	{
		if (brightness < 0)
	        brightness = VPS_VIDEO_DECODER_NO_CHANGE;
	    if (brightness > 254)
	        brightness = 254;

	    if (contrast < 0)
	        contrast = VPS_VIDEO_DECODER_NO_CHANGE;
	    if (contrast > 254)
	        contrast = 254;

	    if (saturation < 0)
	        saturation = VPS_VIDEO_DECODER_NO_CHANGE;
	    if (saturation > 254)
	        saturation = 254;

	    pInst = &pObj->instObj[instId];

        colorPrm.channelNum      = chId%4;
        colorPrm.videoBrightness = brightness;
        colorPrm.videoContrast   = contrast;
        colorPrm.videoSaturation = saturation;
        colorPrm.videoSharpness  = VPS_VIDEO_DECODER_NO_CHANGE;
        colorPrm.videoHue        = VPS_VIDEO_DECODER_NO_CHANGE;

        if(pInst->videoDecoderHandle)
        {
            status = FVID2_control(pInst->videoDecoderHandle,
                                   IOCTL_VPS_VIDEO_DECODER_SET_VIDEO_COLOR,
                                   &colorPrm, NULL);
        }
	}

    return FVID2_SOK;
}


Int32 CaptureLink_drvSetAudioParams(CaptureLink_Obj * pObj, Capture_AudioModeParams * audArgs)
{
    Int32 aud_status = FVID2_SOK;
    UInt32 instId;
    CaptureLink_InstObj *pInst;
    Vps_VideoDecoderChipIdStatus vidDecChipIdStatus;
    Int32 tvpCascadedStage =0;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];

        switch (pObj->createArgs.numAudioChannels)/* numAudiChannels should be passed from Application*/
        /*pObj->createArgs.numAudioChannels : Reference in multich_progressive_vcap_venc_vdec_vdis.c*/
        {
            case 4:
            {
                audArgs->tdmChannelNum       = 1; // pDrvState->audiohwPortProperties.tdmChannelNum;
                audArgs->cascadeStage        = 0;
            }
        break;
        case 16:
            {
                audArgs->tdmChannelNum       = 4; // pDrvState->audiohwPortProperties.tdmChannelNum;
                audArgs->cascadeStage        = tvpCascadedStage;
                tvpCascadedStage++; /* In case of Cascading the cascadeStage  should be incremented by 1 */
            }
        break;
        default:
            printf("\n Audio Channels are not supported\n");
            break;
        }

        aud_status = FVID2_control(
                 pInst->videoDecoderHandle,
                 IOCTL_VPS_TVP5158_SET_AUDIO_MODE,
                 audArgs,
                 &vidDecChipIdStatus
                 );
    }
    return aud_status;
}

Int32 CaptureLink_printBufferStatus(CaptureLink_Obj * pObj)
{
    Uint8 i, str[256];

    Vps_rprintf(
        " \n"
        " *** CAPTURE Statistics *** \n"
        "%d: CAPTURE: Queued to driver = %d, Dequeued from driver = %d\r\n",
        Utils_getCurTimeInMsec(), pObj->captureQueuedFrameCount, pObj->captureDequeuedFrameCount);

    for (i=0; i<1; i++)
//        for (i=0; i<CAPTURE_LINK_MAX_OUT_QUE; i++)
    {
        sprintf ((char *) str, "CAPTURE [%d]", i);
        Utils_bufPrintStatus(str, &pObj->bufQue[i]);
    }
    return 0;
}

Int32 CaptureLink_drvWriteSensor(CaptureLink_Obj * pObj, Vps_VideoDecoderRegRdWrParams *param)
{
	CaptureLink_VipInstParams *pInstPrm;
    CaptureLink_InstObj *pInst;
    Int32 instId, chId=0;
    Int32 status = FVID2_SOK;

	instId = (chId/pObj->createArgs.numVipInst);
    pInstPrm = &pObj->createArgs.vipInst[instId];
    pInst = &pObj->instObj[instId];

	if (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_PH1100K_DRV
			|| pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_PH3100K_DRV
			|| pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP6124B_DRV
			|| pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP2440H_DRV)
    {
	    status = FVID2_control(pInst->videoDecoderHandle,
						IOCTL_VPS_VIDEO_DECODER_REG_WRITE,
						(Vps_VideoDecoderRegRdWrParams *)param, NULL);
	    //UTILS_assert(status == FVID2_SOK);
	}

    return status;
}

Int32 CaptureLink_drvReadSensor(CaptureLink_Obj * pObj, Vps_VideoDecoderRegRdWrParams *param)
{
	CaptureLink_VipInstParams *pInstPrm;
    CaptureLink_InstObj *pInst;
    Int32 instId, chId=0;
    Int32 status = FVID2_SOK;

	instId = (chId/pObj->createArgs.numVipInst);
    pInstPrm = &pObj->createArgs.vipInst[instId];
    pInst = &pObj->instObj[instId];

	if (pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_PH1100K_DRV
			|| pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_PH3100K_DRV
			|| pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP6124B_DRV
			|| pInstPrm->videoDecoderId == FVID2_VPS_VID_DEC_NVP2440H_DRV)
    {
		status = FVID2_control(pInst->videoDecoderHandle,
						IOCTL_VPS_VIDEO_DECODER_REG_READ,
						(Vps_VideoDecoderRegRdWrParams *)param, NULL);
	    //UTILS_assert(status == FVID2_SOK);
	}

    return status;
}
