/** ==================================================================
 *  @file   cameraLink_drv.c
 *
 *  @path    ipnc_mcfw/mcfw/src_bios6/links_m3vpss/camera/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

#include <xdc/std.h>
#include <mcfw/interfaces/link_api/system_tiler.h>
#include "cameraLink_priv.h"
#include <ti/psp/platforms/iss_platform.h>
#include <ti/psp/devices/iss_sensorDriver.h>
#include <ti/psp/iss/alg/dcc/inc/idcc.h>
#include <mcfw/interfaces/link_api/ispLink.h>
#include <mcfw/interfaces/link_api/vaLink.h>
#include <mcfw/interfaces/link_api/vstabLink.h>

UInt32 gCameraLink_framesPerChannel;

static Int32 gSensorFramerate = 30;
#ifdef USE_ISS_BOXCAR
Int32 gGLBCEnable = 1;
#else
Int32 gGLBCEnable = 0;
#endif
static Int32 gFramerate_secondaryStream = 30;
static Int32 PrimaryStreamRtParams = 0;
static Int32 SecondaryStreamRtParams = 0;

Iss_SensorVideoModeParams sensorVideoModeArgs;

Int32 gVstabEnable = 0;

extern Uint32 dcc_buf_size;
extern Int8 dcc_buf[];

char *gCameraLink_portName[] = {
    "VP CSI2", "VIP0 PortA", "VIP0 PortB", "VIP1 PortA", "VIP1 PortB",
};

char *gCameraLink_ifName[] = {
    "Bayer 12-bit", "YUV 8-bit", "YUV 16-bit",
};

char *gCameraLink_modeName[] = {
    "Single stream", "Dual stream",
};

/* driver callback */
/* ===================================================================
 *  @func     CameraLink_drvCallback
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvCallback(FVID2_Handle handle, Ptr appData, Ptr reserved)
{
    CameraLink_Obj *pObj = (CameraLink_Obj *) appData;

    Utils_tskSendCmd(&pObj->tsk, SYSTEM_CMD_NEW_DATA);
    pObj->cbCount++;

    return FVID2_SOK;
}

/* Create Sensor for MT9J003 */
/* ===================================================================
 *  @func     CameraLink_drvCreateSensor
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvCreateSensor(CameraLink_Obj * pObj, UInt16 instId)
{
    CameraLink_VipInstParams *pInstPrm;

    CameraLink_InstObj *pInst;

    Iss_CaptCreateParams *pVipCreateArgs;

    Int32 status = 0;

    pInstPrm = &pObj->createArgs.vipInst[instId];
    pInst = &pObj->instObj[instId];

    pVipCreateArgs = &pInst->createArgs;

    pInst->sensorCreateArgs.deviceI2cInstId = Iss_platformGetI2cInstId();
    pInst->sensorCreateArgs.numDevicesAtPort = 1;
    pInst->sensorCreateArgs.deviceI2cAddr[0] =
    Iss_platformGetSensorI2cAddr(pInstPrm->videoDecoderId,
                                     pInstPrm->vipInstId);
    pInst->sensorCreateArgs.deviceResetGpio[0] = ISS_SENSOR_GPIO_NONE;

    if((gVstabEnable == 1) && (pObj->createArgs.issVnfEnable == 1)) {
	    pInst->sensorCreateArgs.InputStandard = SYSTEM_STD_1080P_60;
    } else {
	    pInst->sensorCreateArgs.InputStandard = pInstPrm->standard;
    }

    pInst->sensorCreateArgs.vstabEnabled = gVstabEnable;

	#ifdef IMGS_MICRON_MT9J003
	if ((pInstPrm->videoDecoderId == FVID2_ISS_SENSOR_MT9J003_DRV) && (pInstPrm->vipInstId == ISS_CAPT_INST_VP || pInstPrm->vipInstId == ISS_CAPT_INST_VP))
	{
		status = Iss_platformSelectSensor(pInstPrm->videoDecoderId, pInstPrm->vipInstId);
	}
	#endif

    pInst->sensorVideoModeArgs.videoIfMode = pVipCreateArgs->videoIfMode;
    pInst->sensorVideoModeArgs.videoDataFormat = pVipCreateArgs->inDataFormat;

    if((gVstabEnable == 1) && (pObj->createArgs.issVnfEnable == 1)) {
	    pInst->sensorVideoModeArgs.standard = SYSTEM_STD_1080P_60;
    } else {
	    pInst->sensorVideoModeArgs.standard = pInstPrm->standard;
    }

    pInst->sensorVideoModeArgs.videoSystem =
        ISS_SENSOR_VIDEO_SYSTEM_AUTO_DETECT;
    pInst->sensorVideoModeArgs.videoAutoDetectTimeout = BIOS_WAIT_FOREVER;
    sensorVideoModeArgs.standard = pInstPrm->standard;

    pInst->SensorHandle = FVID2_create(pInstPrm->videoDecoderId,
                                       0,
                                       &pInst->sensorCreateArgs,
                                       &pInst->sensorCreateStatus, NULL);

    UTILS_assert(pInst->SensorHandle != NULL);

	#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf
        (" %d: CAMERA: VIP %d: VID DEC 0x%x (0x%02x)\n",
         Clock_getTicks(), pInstPrm->vipInstId, pInstPrm->videoDecoderId,
         pInst->sensorCreateArgs.deviceI2cAddr[0]);
	#endif

    return status;
}

/* ===================================================================
 *  @func     CameraLink_drvInstSetFrameSkip
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvInstSetFrameSkip(CameraLink_Obj * pObj, UInt16 instId,
                                     UInt16 outId, UInt32 frameSkipMask, UInt32 frameSkipMaskHigh)
{
    Iss_CaptFrameSkip frameSkip;

    UInt16 chId;

    Int32 status = 0;

    CameraLink_InstObj *pInst;

    Iss_CaptCreateParams *pVipCreateArgs;

    pInst = &pObj->instObj[instId];
    pVipCreateArgs = &pInst->createArgs;

    /*
     * set frame skip using a IOCTL if enabled
     */
    for (chId = 0; chId < pVipCreateArgs->numCh; chId++)
    {
        frameSkip.channelNum = pVipCreateArgs->channelNumMap[outId][chId];
        frameSkip.frameSkipMask[outId] = frameSkipMask;
		frameSkip.frameSkipMaskHigh[outId] = frameSkipMaskHigh;
        status = FVID2_control(pInst->cameraVipHandle,
                               IOCTL_ISS_CAPT_SET_FRAME_SKIP, &frameSkip, NULL);
        UTILS_assert(status == FVID2_SOK);
    }

    return status;
}

/* ===================================================================
 *  @func     CameraLink_drvSetMirrorMode
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetMirrorMode(CameraLink_Obj * pObj, Int32 mirrorMode)
{
    Int32 status;

    CameraLink_InstObj *pInst;

    pInst = &pObj->instObj[0];

    /*
     * set mirror mode using a IOCTL
     */

	status =
        FVID2_control(pInst->cameraVipHandle, IOCTL_ISS_CAPT_SET_MIRROR_MODE,
                      &mirrorMode, NULL);
    UTILS_assert(status == FVID2_SOK);

	if (mirrorMode > 3)
	{
		PrimaryStreamRtParams = 1;
		SecondaryStreamRtParams = 1;
	}

    return status;
}

/* VS Process function */
/* ===================================================================
 *  @func     CameraLink_vsProcessFunc
 *
 *  @desc     Function does the following
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
Int32 CameraLink_vsProcessFunc(Iss_CaptVsProcessPrm *pVsProcessPrm)
{
    CameraLink_Obj *pObj = &gCameraLink_obj;
	VstabLink_processPrm vsProcessPrm;

	if(pObj->createArgs.vstabLinkId != SYSTEM_LINK_ID_INVALID)
	{
		vsProcessPrm.bscBufferCurrV = pVsProcessPrm->bscBufferCurrV;
		vsProcessPrm.bscBufferCurrH = pVsProcessPrm->bscBufferCurrH;
		vsProcessPrm.bscBufferPrevV = pVsProcessPrm->bscBufferPrevV;
		vsProcessPrm.bscBufferPrevH = pVsProcessPrm->bscBufferPrevH;

		System_linkControl(pObj->createArgs.vstabLinkId,
                           SYSTEM_CMD_NEW_DATA,
                           &vsProcessPrm,
						   sizeof(VstabLink_processPrm),TRUE);

		pVsProcessPrm->startX = vsProcessPrm.startX;
		pVsProcessPrm->startY = vsProcessPrm.startY;

		return FVID2_SOK;
	}
	else
	{
		return FVID2_EFAIL;
	}
}

/* Create camera driver */
/* ===================================================================
 *  @func     CameraLink_drvCreateInst
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvCreateInst(CameraLink_Obj * pObj, UInt16 instId)
{
    CameraLink_VipInstParams *pInstPrm;
    CameraLink_InstObj *pInst;
    Iss_CaptCreateParams *pVipCreateArgs;
    Iss_CaptOutInfo *pVipOutPrm;
    CameraLink_OutParams *pOutPrm;
    System_LinkChInfo *pQueChInfo;
    UInt16 queId, queChId, outId, chId;
    FVID2_ScanFormat inScanFormat;

    pInstPrm = &pObj->createArgs.vipInst[instId];

    pInst = &pObj->instObj[instId];
    pVipCreateArgs = &pInst->createArgs;

    pInst->instId = pInstPrm->vipInstId;

    CameraLink_drvInitCreateArgs(pVipCreateArgs, pInstPrm);

    pVipCreateArgs->inDataFormat = pInstPrm->inDataFormat;

    inScanFormat = FVID2_SF_PROGRESSIVE;
    if (instId == 0)
    {
        pObj->maxWidth = 1920;
        pObj->maxHeight = 1080;
    }

    if (pVipCreateArgs->inDataFormat == FVID2_DF_RGB24_888)
    {
        pVipCreateArgs->videoCaptureMode =
            ISS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_STREAM;
        pVipCreateArgs->videoIfMode = ISS_CAPT_BAYER_12BIT;
    }
    else
    {
        pVipCreateArgs->videoCaptureMode =
            ISS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_STREAM;
        pVipCreateArgs->videoIfMode = ISS_CAPT_BAYER_12BIT;
    }
    pVipCreateArgs->numCh = 1;
    pVipCreateArgs->vsEnable = pObj->createArgs.vsEnable;
	pVipCreateArgs->glbcEnable = gGLBCEnable;

    Vps_printf(" %d: CAMERA: %s camera mode is [%s, %s] !!! \n",
               Clock_getTicks(),
               gCameraLink_portName[pInstPrm->vipInstId],
               gCameraLink_ifName[pVipCreateArgs->videoIfMode],
               gCameraLink_modeName[pVipCreateArgs->videoCaptureMode]);

    pVipCreateArgs->periodicCallbackEnable = TRUE;

    pVipCreateArgs->numStream = pInstPrm->numOutput;

    for (outId = 0; outId < pVipCreateArgs->numStream; outId++)
    {
        pVipOutPrm = &pVipCreateArgs->outStreamInfo[outId];
        pOutPrm = &pInstPrm->outParams[outId];

        pVipOutPrm->dataFormat = pOutPrm->dataFormat;

        pVipOutPrm->memType = VPS_VPDMA_MT_NONTILEDMEM;

        if (pObj->createArgs.tilerEnable)
        {
            pVipOutPrm->memType = VPS_VPDMA_MT_TILEDMEM;
        }

		pVipOutPrm->pitch[0] = pOutPrm->scOutWidth;

		if(gVstabEnable == 1)
		{
		    pVipOutPrm->pitch[0] = (pOutPrm->scOutWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
		}

		if(pVipOutPrm->dataFormat != FVID2_DF_BAYER_RAW)
		{
		    if (pInstPrm->outParams[0].scOutHeight > 1080)
			{
			    gCameraLink_framesPerChannel = CAMERA_LINK_FRAMES_PER_CH_HIGH_RESOLUTION;
			}
			else
			{
			    gCameraLink_framesPerChannel = CAMERA_LINK_FRAMES_PER_CH;
			}
		}
		else
		{
		    gCameraLink_framesPerChannel = CAMERA_LINK_FRAMES_PER_CH_RAW;
		}

        if ((pVipOutPrm->dataFormat == FVID2_DF_YUV422I_YUYV) ||
            (pVipOutPrm->dataFormat == FVID2_DF_YUV422I_VYUY) ||
			(pVipOutPrm->dataFormat == FVID2_DF_BAYER_RAW))
            pVipOutPrm->pitch[0] *= 2;
        if (pVipOutPrm->dataFormat == FVID2_DF_RGB24_888)
            pVipOutPrm->pitch[0] *= 3;
        pVipOutPrm->pitch[1] = pVipOutPrm->pitch[0];

        if (CameraLink_drvIsDataFormatTiled(pVipCreateArgs, outId))
        {
            pVipOutPrm->pitch[0] = VPSUTILS_TILER_CNT_8BIT_PITCH;
            pVipOutPrm->pitch[1] = VPSUTILS_TILER_CNT_16BIT_PITCH;
        }

        pVipOutPrm->pitch[2] = 0;

        pVipOutPrm->scEnable = pOutPrm->scEnable;

        for (chId = 0; chId < pVipCreateArgs->numCh; chId++)
        {
            queId = pOutPrm->outQueId;
            queChId = pObj->info.queInfo[queId].numCh;

            pQueChInfo = &pObj->info.queInfo[queId].chInfo[queChId];

            pQueChInfo->dataFormat = (FVID2_DataFormat) pVipOutPrm->dataFormat;
            pQueChInfo->memType = (Vps_VpdmaMemoryType) pVipOutPrm->memType;

            if (pVipOutPrm->scEnable)
            {
                pQueChInfo->width = pOutPrm->scOutWidth;
                pQueChInfo->height = pOutPrm->scOutHeight;
            }
            else
            {
                pQueChInfo->width    = pOutPrm->scOutWidth;
                pQueChInfo->height   = pOutPrm->scOutHeight;
				pQueChInfo->pitch[0] = pVipOutPrm->pitch[0];
            }

            pQueChInfo->pitch[1] = pQueChInfo->pitch[0];
            pQueChInfo->pitch[2] = 0;
            pQueChInfo->scanFormat = inScanFormat;

            pObj->info.queInfo[queId].numCh++;

            pVipCreateArgs->channelNumMap[outId][chId] =
                CameraLink_makeChannelNum(queId, queChId);
        }
    }

    memset(&pInst->cbPrm, 0, sizeof(pInst->cbPrm));

    pInst->cbPrm.appData = pObj;

    if (instId == 0)
        pInst->cbPrm.cbFxn = CameraLink_drvCallback;

    pObj->enableCheckOverflowDetect = FALSE;

    /* creating the sensor before capture instance,need to pass sensor handle to */
    pInst->SensorHandle = NULL;

#ifdef SYSTEM_USE_SENSOR
    CameraLink_drvCreateSensor(pObj, instId);
#endif

    pVipCreateArgs->aewbVendor   = pObj->createArgs.t2aConfig.n2A_vendor;
    pVipCreateArgs->aewbMode     = pObj->createArgs.t2aConfig.n2A_mode;
    pVipCreateArgs->aewbPriority = PRIORITY_FRAMERATE;
    pVipCreateArgs->SensorHandle = pInst->SensorHandle;
	pVipCreateArgs->captureMode  = (Iss_CaptMode)pObj->createArgs.captureMode;
	pVipCreateArgs->vsProcessFunc = CameraLink_vsProcessFunc;

    pInst->cameraVipHandle = FVID2_create(FVID2_ISS_CAPT_DRV,
                                          pInst->instId,
                                          pVipCreateArgs,
                                          &pInst->createStatus, &pInst->cbPrm);

    UTILS_assert(pInst->cameraVipHandle != NULL);

    CameraLink_drvAllocAndQueueFrames(pObj, pInst);

    CameraLink_drvInstSetFrameSkip(pObj, instId, 0, 0, 0);
    if(pVipCreateArgs->numStream > 1)
    {
    	#if 1
    	CameraLink_drvInstSetFrameSkip(pObj, instId, 1, 0, 0);
    	#else
    	//# 15fps: CAMERA_LINK_FRAMESKIPMASK_HALF, 22fps: CAMERA_LINK_FRAMESKIPMASK_TWOTHIRD
    	CameraLink_drvInstSetFrameSkip(pObj, instId, 1,
			CAMERA_LINK_FRAMESKIPMASK_TWOTHIRD, CAMERA_LINK_FRAMESKIPMASK_TWOTHIRD);
		#endif
    }

    return FVID2_SOK;
}

/* Create camera link This creates - camera driver - video decoder driver -
 * allocate and queues frames to the camera driver - DOES NOT start the
 * camera ONLY make it ready for operation */
/* ===================================================================
 *  @func     CameraLink_drvCreate
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvCreate(CameraLink_Obj * pObj,
                           CameraLink_CreateParams * pPrm)
{
    Int32 status;

    UInt32 queId, instId;

	UInt32 capHorLineOffset,capVertLineOffset;

    CameraLink_VipInstParams *pCameraInstPrm;

    CameraLink_OutParams *pCameraOutPrm;

#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf(" %d: CAMERA: Create in progress !!!\n", Clock_getTicks());
#endif

    memcpy(&pObj->createArgs, pPrm, sizeof(*pPrm));

    /* Meta data offset is calculated based on the first stream resolution
     * only */
    /* And it is assumed that meta data is present only for the first stream */
	if(pObj->createArgs.captureMode == CAMERA_LINK_CAPMODE_ISIF)
	{
		gVstabEnable = pObj->createArgs.vsEnable;
	}
	else
	{
	    /* VS is turned OFF in DDR input mode */
		gVstabEnable = 0;
	}

	pObj->vnfLinkId = pObj->createArgs.vnfLinkId;
	pObj->glbceLinkId = pObj->createArgs.glbceLinkId;

    pCameraInstPrm = &pObj->createArgs.vipInst[0];
    pCameraOutPrm = &pCameraInstPrm->outParams[0];

    #ifndef SYSTEM_USE_TILER
    if (pObj->createArgs.tilerEnable)
    {
        Vps_printf("CAMERALINK:!!WARNING.FORCIBLY DISABLING TILER since tiler is disabled at build time");
        pObj->createArgs.tilerEnable = FALSE;
    }
    #endif

	capHorLineOffset  = pCameraOutPrm->scOutWidth;
	capVertLineOffset = pCameraOutPrm->scOutHeight;

	if(gVstabEnable == 1)
	{
	    capHorLineOffset  = (pCameraOutPrm->scOutWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
		capVertLineOffset = (pCameraOutPrm->scOutHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
	}

    if (pCameraOutPrm->dataFormat == SYSTEM_DF_YUV422I_VYUY)
    {
        pObj->metaDataOffset = (capHorLineOffset * capVertLineOffset * 2);
    }
    else
    {
		pObj->metaDataOffset = (capHorLineOffset * capVertLineOffset * 3)/2;
    }

    pObj->cameraDequeuedFrameCount = 0;
    pObj->cameraQueuedFrameCount = 0;
    pObj->cbCount = 0;
    pObj->cbCountServicedCount = 0;
    pObj->prevFrameCount = 0;
    pObj->totalCpuLoad = 0;
    pObj->resetCount = 0;
    pObj->resetTotalTime = 0;
    pObj->prevResetTime = 0;
    pObj->isPalMode = FALSE;

    pObj->brightness = 0x1c;                               /* TUNED for
                                                            * specific
                                                            * scene's, to
                                                            * make black
                                                            * blacker */
    pObj->contrast = 0x89;                                 /* TUNED for
                                                            * specific
                                                            * scene's, to
                                                            * make black
                                                            * blacker */
    pObj->saturation = 128;                                /* default */

    memset(pObj->cameraFrameCount, 0, sizeof(pObj->cameraFrameCount));

    pObj->info.numQue = CAMERA_LINK_MAX_OUT_QUE;

    for (queId = 0; queId < CAMERA_LINK_MAX_OUT_QUE; queId++)
    {
        status = Utils_bufCreate(&pObj->bufQue[queId], FALSE, FALSE);
        UTILS_assert(status == FVID2_SOK);

        pObj->info.queInfo[queId].numCh = 0;
    }

    /*
     * Create global VP camera handle, used for dequeue,
     * queue from all active cameras
     */
    pObj->fvidHandleVipAll = FVID2_create(FVID2_ISS_CAPT_DRV,
                                          ISS_CAPT_INST_ALL, NULL, NULL, NULL);
    UTILS_assert(pObj->fvidHandleVipAll != NULL);

    for (instId = 0; instId < pPrm->numVipInst; instId++)
    {
        CameraLink_drvCreateInst(pObj, instId);
    }

#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf(" %d: CAMERA: Create Done !!!\n", Clock_getTicks());
#endif

    return status;
}

/* ===================================================================
 *  @func     CameraLink_drvDetectVideo
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvDetectVideo(CameraLink_Obj * pObj, UInt32 timeout)
{
    // Int32 status;
    UInt32 instId;

    CameraLink_InstObj *pInst;

#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf(" %d: CAMERA: Detect video in progress !!!\n", Clock_getTicks());
#endif

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];

        if (pInst->SensorHandle == NULL)
            return FVID2_SOK;

        pInst->sensorVideoModeArgs.videoAutoDetectTimeout = timeout;
    }

#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf(" %d: CAMERA: Detect video Done !!!\n", Clock_getTicks());
#endif

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvOverflowDetectAndReset
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvOverflowDetectAndReset(CameraLink_Obj * pObj, Bool doForceReset)
{
    Iss_CaptOverFlowStatus overFlowStatus;

    FVID2_control(pObj->fvidHandleVipAll, IOCTL_ISS_CAPT_RESET_AND_RESTART, &overFlowStatus, NULL);
    Vps_rprintf (" %d: CAMERA: Overflow detected, Total Resets = %d\n", Clock_getTicks(), pObj->resetCount);

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvPrintRtStatus
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvPrintRtStatus(CameraLink_Obj * pObj, UInt32 frameCount,
                                  UInt32 elaspedTime)
{
    UInt32 fps = (frameCount * 100) / (elaspedTime / 10);

    Vps_rprintf
		(" [i] CAMERA: Fields = %d (fps = %d), Resets = %d (CPU Load = %d)\r\n",
			/*Utils_getCurTimeInMsec(), */frameCount, fps, pObj->resetCount,
			pObj->totalCpuLoad / pObj->cpuLoadCount);

    return 0;
}

/* ===================================================================
 *  @func     CameraLink_drvProcessData
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvProcessData(CameraLink_Obj * pObj)
{
    UInt32 frameId, queId, streamId, queChId, elaspedTime;
    FVID2_FrameList frameList;
    FVID2_Frame *pFrame;
    volatile UInt32 sendMsgToTsk = 0, tmpValue;
	Iss_CaptRtParams *pRtParams = NULL;
    Int32 status;
	System_FrameInfo *pFrameInfo = NULL;
	UInt_64 cur_time;

    //pObj->cbCountServicedCount++;
    System_displayUnderflowCheck(FALSE);

    for (streamId = 0; streamId < CAMERA_LINK_MAX_OUTPUT_PER_INST; streamId++)
    {
        /* Deque frames for all active handles */
        FVID2_dequeue(pObj->fvidHandleVipAll, &frameList, streamId, BIOS_NO_WAIT);

        if (frameList.numFrames)
        {
        	Utils_get_walltime(&cur_time);
            for (frameId = 0; frameId < frameList.numFrames; frameId++)
            {
                pFrame = frameList.frames[frameId];

                queId = CameraLink_getQueId(pFrame->channelNum);
                queChId = CameraLink_getQueChId(pFrame->channelNum);

                UTILS_assert(queId < CAMERA_LINK_MAX_OUT_QUE);
                UTILS_assert(queChId < CAMERA_LINK_MAX_CH_PER_OUT_QUE);

                pObj->cameraDequeuedFrameCount++;
                pObj->cameraFrameCount[queId][queChId]++;

                tmpValue = (UInt32) pFrame->reserved;

                /* reserved field is not used so the error message printed is non-fatal */
				#if 0
                if (tmpValue > 0)
                {
                    Vps_printf
                        (" %d: CAMERA: Dequeued frame more than once (%d,%d, %08x) \n",
                         Clock_getTicks(), queId, queChId, pFrame->addr[0][0]);
                }
				#endif
                tmpValue++;
                pFrame->reserved = (Ptr) tmpValue;
                pFrame->perFrameCfg = NULL;
                pFrame->channelNum = queChId;
				//pFrame->timeStamp = Utils_getCurTimeInMsec();

                pFrameInfo = (System_FrameInfo *) pFrame->appData;
				pRtParams = (Iss_CaptRtParams *)pFrame->perFrameCfg;
				pFrameInfo->rtChInfo.height = pRtParams->captureOutHeight;
				pFrameInfo->rtChInfo.width = pRtParams->captureOutWidth;
				pFrameInfo->rtChInfo.pitch[0] = pRtParams->captureOutPitch;
				pFrameInfo->cameraChannelNum = queId;
				pFrameInfo->ts64 = cur_time;//Utils_getCurTimeInMsec();

                sendMsgToTsk |= (1 << queId);

				status = Utils_bufPutFullFrame(&pObj->bufQue[queId], pFrame);
				UTILS_assert(status == FVID2_SOK);
            }

			#ifdef SYSTEM_DEBUG_CAMERA_RT
            Vps_printf(" %d: CAMERA: Dequeued %d frames !!!\n",
                       Clock_getTicks(), frameList.numFrames);
			#endif
        }
    }

	#ifdef SYS_DEBUG
    elaspedTime = Utils_getCurTimeInMsec() - pObj->startTime;
    if ((elaspedTime - pObj->prevTime) > 60 * 1000)
    {
        CameraLink_getCpuLoad();
        CameraLink_drvPrintRtStatus(pObj,
                                    pObj->cameraDequeuedFrameCount -
                                    pObj->prevFrameCount,
                                    elaspedTime - pObj->prevTime);

        pObj->prevFrameCount = pObj->cameraDequeuedFrameCount;
        pObj->prevTime = elaspedTime;
    }
    #endif

    for (queId = 0; queId < CAMERA_LINK_MAX_OUT_QUE; queId++)
    {
        if (sendMsgToTsk & 0x1)
        {
            /* send data available message to next tsk */
            System_sendLinkCmd(pObj->createArgs.outQueParams[queId].nextLink,
                               SYSTEM_CMD_NEW_DATA);
        }

        sendMsgToTsk >>= 1;
        if (sendMsgToTsk == 0)
            break;
    }

    if (pObj->enableCheckOverflowDetect)
    {
        CameraLink_drvOverflowDetectAndReset(pObj, FALSE);
    }

    pObj->exeTime = Clock_getTicks() - pObj->startTime;

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvPutEmptyFrames
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvPutEmptyFrames(CameraLink_Obj * pObj,
                                   FVID2_FrameList * pFrameList)
{
    UInt32 frameId;

    FVID2_Frame *pFrame;

    System_FrameInfo *pFrameInfo = NULL;

    volatile UInt32 tmpValue;

    if (pFrameList->numFrames)
    {
        for (frameId = 0; frameId < pFrameList->numFrames; frameId++)
        {
            pFrame = pFrameList->frames[frameId];
            if (pFrame == NULL)
                continue;

			tmpValue = (UInt32) pFrame->reserved;
			tmpValue--;
			pFrame->reserved = (Ptr) tmpValue;

            pFrameInfo = (System_FrameInfo *) pFrame->appData;
            UTILS_assert(pFrameInfo != NULL);

            pFrame->perFrameCfg = &pFrameInfo->captureRtParams;
            pFrame->channelNum = pFrameInfo->cameraChannelNum;
			if((pFrame->channelNum == 0) && (PrimaryStreamRtParams == 1))
			{
				pFrameInfo->rtChInfoUpdate = TRUE;
			}
			if((pFrame->channelNum == 1) && (SecondaryStreamRtParams == 1))
			{
				pFrameInfo->rtChInfoUpdate = TRUE;
			}
        }

		#ifdef SYSTEM_DEBUG_CAMERA_RT
        Vps_printf(" %d: CAMERA: Queued back %d frames !!!\n",
                   Clock_getTicks(), pFrameList->numFrames);
		#endif

		pObj->cameraQueuedFrameCount += pFrameList->numFrames;

		FVID2_queue(pObj->fvidHandleVipAll,
					pFrameList, ISS_CAPT_STREAM_ID_ANY);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvStart
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvStart(CameraLink_Obj * pObj)
{
    UInt32 instId;

    CameraLink_InstObj *pInstObj;

#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf(" %d: CAMERA: Start in progress !!!\n", Clock_getTicks());
#endif

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* video decoder */
        if (pInstObj->SensorHandle)
            FVID2_start(pInstObj->SensorHandle, NULL);

    }

#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf(" %d: CAMERA: Start Done !!!\n", Clock_getTicks());
#endif

    Task_sleep(100);

    pObj->prevTime = pObj->startTime = Clock_getTicks();

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* VP camera */
        FVID2_start(pInstObj->cameraVipHandle, NULL);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvFlush
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvFlush(FVID2_Handle cameraVipHandle, char *portName)
{
    Int32 status;

    FVID2_FrameList frameList;

    do
    {
        status = FVID2_control(cameraVipHandle, NULL, NULL, &frameList);

		#ifdef SYSTEM_DEBUG_CAMERA
        Vps_rprintf(" %d: CAMERA: %s: Flushed %d frames.\n",
                    Clock_getTicks(), portName, frameList.numFrames);
		#endif

    } while (frameList.numFrames != 0 && status == FVID2_SOK);

    if (status != FVID2_SOK)
    {
		#ifdef SYSTEM_DEBUG_CAMERA_RT
        Vps_rprintf(" %d: CAMERA: %s: Flushing ... ERROR !!!\n",
                    Clock_getTicks(), portName);
		#endif
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvStop
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvStop(CameraLink_Obj * pObj)
{
    UInt32 instId;

    CameraLink_InstObj *pInstObj;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* VP camera */
        FVID2_stop(pInstObj->cameraVipHandle, NULL);
    }

    pObj->exeTime = Clock_getTicks() - pObj->startTime;

#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf(" %d: CAMERA: Stop in progress !!!\n", Clock_getTicks());
#endif

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* video decoder */
        if (pInstObj->SensorHandle)
            FVID2_stop(pInstObj->SensorHandle, NULL);
    }

#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf(" %d: CAMERA: Stop Done !!!\n", Clock_getTicks());
#endif

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvDelete
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvDelete(CameraLink_Obj * pObj)
{
    UInt32 instId;

    UInt32 queId;

    CameraLink_InstObj *pInstObj;

#ifdef SYSTEM_DEBUG_CAMERA
    CameraLink_drvPrintStatus(pObj);
#endif

#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf(" %d: CAMERA: Delete in progress !!!\n", Clock_getTicks());
#endif

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* VP camera */
        FVID2_delete(pInstObj->cameraVipHandle, NULL);

        if (pInstObj->SensorHandle)
        {
            /* delete sensor handle */
            FVID2_delete(pInstObj->SensorHandle, NULL);
        }

        CameraLink_drvFreeFrames(pObj, pInstObj);
    }

    FVID2_delete(pObj->fvidHandleVipAll, NULL);

    for (queId = 0; queId < CAMERA_LINK_MAX_OUT_QUE; queId++)
    {
        Utils_bufDelete(&pObj->bufQue[queId]);
    }

#ifdef SYSTEM_DEBUG_CAMERA
    Vps_printf(" %d: CAMERA: Delete Done !!!\n", Clock_getTicks());
#endif

    return FVID2_SOK;
}

/*
 * Allocate and queue frames to driver
 *
 * pDrvObj - camera driver information */
/* ===================================================================
 *  @func     CameraLink_drvAllocAndQueueFrames
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvAllocAndQueueFrames(CameraLink_Obj * pObj,
                                        CameraLink_InstObj * pDrvObj)
{
    Int32 status, Blanking_Data_Size = 0, BoxCarDataSize = 0;

    UInt16 streamId, chId, frameId, idx;

    Iss_CaptOutInfo *pOutInfo;

    FVID2_Frame *frames;

    System_FrameInfo *pFrameInfo;

    FVID2_FrameList frameList;

    FVID2_Format format;

    CameraLink_VipInstParams *pInstPrm;

    CameraLink_OutParams *pOutPrm;

    pInstPrm = &pObj->createArgs.vipInst[0];
    /*
     * init frameList for list of frames that are queued per CH to driver
     */
    frameList.perListCfg = NULL;
    frameList.reserved = NULL;
	format.pitch[0] = 0;

    /*
     * for every stream and channel in a camera handle
     */
    for (streamId = 0; streamId < pDrvObj->createArgs.numStream; streamId++)
    {
        for (chId = 0; chId < pDrvObj->createArgs.numCh; chId++)
        {

            pOutInfo = &pDrvObj->createArgs.outStreamInfo[streamId];
            pOutPrm = &pInstPrm->outParams[streamId];
            /*
             * base index for pDrvObj->frames[] and pDrvObj->frameInfo[]
             */
            idx =
                ISS_CAPT_CH_PER_PORT_MAX * gCameraLink_framesPerChannel *
                streamId + gCameraLink_framesPerChannel * chId;

            if (idx >= CAMERA_LINK_MAX_FRAMES_PER_HANDLE)
            {
                idx = 0u;
            }

            pFrameInfo = &pDrvObj->frameInfo[idx];
            frames = &pDrvObj->frames[idx];

            /* fill format with channel specific values */
            format.channelNum =
                pDrvObj->createArgs.channelNumMap[streamId][chId];

			format.height = pOutPrm->scOutHeight;
			format.width  = pOutPrm->scOutWidth;

			if ((gVstabEnable == 0) && (streamId == 1) && (format.height < 576))
			{
				format.height = 576;
			}

			if (gVstabEnable == 1)
			{
				format.height = (pOutPrm->scOutHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
				format.width  = (pOutPrm->scOutWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;

				if ((streamId == 1) && (format.height < 692))
				{
				    format.height = 692;
				}
			}

			if (streamId == 0)
			{
			    //Boxcar operation works on 12-bit Bayer data and outputs 16-bit data
			    //The output data is 48-bit RGB data for each 8 ?8 or 16 ?16 block.
				if(gGLBCEnable == 1)
				{
				  BoxCarDataSize = (format.height*format.width/256*2*4  + 2*1024) & 0xffff1000;//64*1024;
				  //Vps_printf("!!!!!!!!!!!! Asking Boxcar data of %d !!!!!!!!!!!!! \n", BoxCarDataSize);
				}
				Blanking_Data_Size = BLANKING_DATA_SIZE + CAMERA_LINK_FD_RESULT_SIZE;
			}
			else
			{
				BoxCarDataSize = 0;
			    Blanking_Data_Size = CAMERA_LINK_FD_RESULT_SIZE;
			}

			format.pitch[0] = pOutInfo->pitch[0];

            format.pitch[1] = format.pitch[0];             // pOutInfo->pitch[0];
            format.pitch[2] = format.pitch[1];
            format.fieldMerged[0] = FALSE;
            format.fieldMerged[1] = FALSE;
            format.fieldMerged[2] = FALSE;
            format.dataFormat = pOutInfo->dataFormat;
            format.scanFormat = FVID2_SF_PROGRESSIVE;
            format.bpp = FVID2_BPP_BITS8;                  /* ignored */

            if (format.dataFormat == FVID2_DF_RAW_VBI)
            {
                format.height = CAMERA_LINK_RAW_VBI_LINES;
            }

            /*
             * alloc memory based on 'format'
             * Allocated frame info is put in frames[]
             * CAMERA_LINK_APP_FRAMES_PER_CH is the number of buffers per channel to
             * allocate
             */
            if (CameraLink_drvIsDataFormatTiled(&pDrvObj->createArgs, streamId))
            {
                Utils_tilerFrameAlloc(&format, frames,
                                      gCameraLink_framesPerChannel);
            }
            else
            {
                Utils_memFrameAlloc_capture(&format, frames,
                                            gCameraLink_framesPerChannel,
                                            Blanking_Data_Size, BoxCarDataSize);
            }

            /*
             * Set rtParams for every frame in perFrameCfg
             */
            for (frameId = 0; frameId < gCameraLink_framesPerChannel; frameId++)
            {
                frames[frameId].perFrameCfg =
                    &pFrameInfo[frameId].captureRtParams;
                frames[frameId].subFrameInfo = NULL;
                frames[frameId].appData = &pFrameInfo[frameId];
                frames[frameId].reserved = NULL;

                pFrameInfo[frameId].cameraChannelNum =
                    frames[frameId].channelNum;
                pFrameInfo[frameId].rtChInfoUpdate = TRUE;
                pFrameInfo[frameId].rtChInfoUpdate = FALSE;

                frameList.frames[frameId] = &frames[frameId];

				#ifdef SYSTEM_VERBOSE_PRINTS
                if (pDrvObj->instId == 0 /*&& streamId == 0*/ && chId == 0)
                {
                    Vps_rprintf(" %d: CAMERA: %d: 0x%08x, %d x %d, %08x B\n",
                                Clock_getTicks(),
                                frameId, frames[frameId].addr[0][0],
                                format.pitch[0] / 2, format.height,
                                format.height * format.pitch[0]);
                }
				#endif
            }
			#ifdef SYSTEM_VERBOSE_PRINTS
            if (pDrvObj->instId == 0 && streamId == 0 && chId == 0)
            {
                Vps_rprintf(" %d: CAMERA: 0x%08x %08x B\n",
                            Clock_getTicks(),
                            frames[0].addr[0][0],
                            format.height * format.pitch[0] * frameId);
            }
			#endif

            /*
             * Set number of frame in frame list
             */
            frameList.numFrames = gCameraLink_framesPerChannel;

            /*
             * queue the frames in frameList
             * All allocate frames are queued here as an example.
             * In general atleast 2 frames per channel need to queued
             * before starting camera,
             * else frame will get dropped until frames are queued
             */
            status =
                FVID2_queue(pDrvObj->cameraVipHandle, &frameList, streamId);
            UTILS_assert(status == FVID2_SOK);
        }
    }

    return FVID2_SOK;
}

/*
 * Free allocated frames
 *
 * pDrvObj - camera driver information */
/* ===================================================================
 *  @func     CameraLink_drvFreeFrames
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvFreeFrames(CameraLink_Obj * pObj,
                               CameraLink_InstObj * pDrvObj)
{
    UInt32 idx, Blanking_Data_Size = 0, BoxCarDataSize = 0;

    UInt16 streamId, chId;

    FVID2_Format format;

    FVID2_Frame *pFrames;

    Iss_CaptOutInfo *pOutInfo;

    UInt32 tilerUsed = FALSE;

    CameraLink_OutParams *pOutPrm;

    CameraLink_VipInstParams *pInstPrm;

    pInstPrm = &pObj->createArgs.vipInst[0];

    format.pitch[0] = 0;

    for (streamId = 0; streamId < pDrvObj->createArgs.numStream; streamId++)
    {
        for (chId = 0; chId < pDrvObj->createArgs.numCh; chId++)
        {
            pOutInfo = &pDrvObj->createArgs.outStreamInfo[streamId];
            pOutPrm = &pInstPrm->outParams[streamId];
            idx = ISS_CAPT_CH_PER_PORT_MAX *
                gCameraLink_framesPerChannel * streamId +
                gCameraLink_framesPerChannel * chId;

            if (idx >= CAMERA_LINK_MAX_FRAMES_PER_HANDLE)
            {
                idx = 0u;
            }

            pFrames = &pDrvObj->frames[idx];

            /* fill format with channel specific values */
            format.channelNum =
                pDrvObj->createArgs.channelNumMap[streamId][chId];

			format.height = pOutPrm->scOutHeight;
			format.width  = pOutPrm->scOutWidth;

			if ((gVstabEnable == 0) && (streamId == 1) && (format.height < 576))
			{
				format.height = 576;
			}

			if (gVstabEnable == 1)
			{
				format.height = (pOutPrm->scOutHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
				format.width  = (pOutPrm->scOutWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;

				if ((streamId == 1) && (format.height < 692))
				{
				    format.height = 692;
				}
			}

			if (streamId == 0)
			{
				if(gGLBCEnable == 1)
				BoxCarDataSize = (format.height*format.width/256*2*4  + 2*1024) & 0xffff1000;//64*1024;
				Blanking_Data_Size = BLANKING_DATA_SIZE + CAMERA_LINK_FD_RESULT_SIZE;
			}
			else
			{
				BoxCarDataSize = 0;
			    Blanking_Data_Size = CAMERA_LINK_FD_RESULT_SIZE;
			}

			format.pitch[0] = pOutInfo->pitch[0];

            format.pitch[1] = format.pitch[0];
            format.pitch[2] = 0;
            format.fieldMerged[0] = FALSE;
            format.fieldMerged[1] = FALSE;
            format.fieldMerged[2] = FALSE;
            format.dataFormat = pOutInfo->dataFormat;
            format.scanFormat = FVID2_SF_PROGRESSIVE;
            format.bpp = FVID2_BPP_BITS8;                  /* ignored */

            if (format.dataFormat == FVID2_DF_RAW_VBI)
            {
                format.height = CAMERA_LINK_RAW_VBI_LINES;
            }

            if (CameraLink_drvIsDataFormatTiled(&pDrvObj->createArgs, streamId))
            {
                /*
                 * cannot free tiled frames
                 */
                tilerUsed = TRUE;
            }
            else
            {
                /*
                 * free frames for this channel, based on pFormat
                 */
                Utils_memFrameFree_Capture(&format, pFrames,
                                           gCameraLink_framesPerChannel,
                                           Blanking_Data_Size, BoxCarDataSize);
            }
        }
    }

    if (tilerUsed)
    {
        SystemTiler_freeAll();
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvIsDataFormatTiled
 *
 *  @desc     Function does the following
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
UInt32 CameraLink_drvIsDataFormatTiled(Iss_CaptCreateParams * createArgs,
                                       UInt16 streamId)
{
    Iss_CaptOutInfo *pOutInfo;

    pOutInfo = &createArgs->outStreamInfo[streamId];

    if (pOutInfo->memType == ISS_TILEDMEM)
    {
        return TRUE;
    }

    return FALSE;
}

/*
 * Init create arguments to default values
 *
 * createArgs - create arguments */
/* ===================================================================
 *  @func     CameraLink_drvInitCreateArgs
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvInitCreateArgs(Iss_CaptCreateParams * createArgs,
                                   CameraLink_VipInstParams * pInstPrm)
{
    UInt16 chId, StreamId;

    Iss_CaptOutInfo *pOutInfo;

    Iss_CaptScParams *pScParams[ISS_CAPT_STREAM_ID_MAX];

    CameraLink_OutParams *pOutPrm;

    memset(createArgs, 0, sizeof(*createArgs));
    createArgs->videoCaptureMode = ISS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_STREAM;

    createArgs->videoIfMode = ISS_CAPT_BAYER_12BIT;

    createArgs->inDataFormat = FVID2_DF_YUV420SP_UV;
    createArgs->periodicCallbackEnable = FALSE;
    createArgs->numCh = 1;
    createArgs->numStream = 2;

    for (StreamId = 0; StreamId < createArgs->numStream; StreamId++)
    {
        pOutPrm = &pInstPrm->outParams[StreamId];
        pScParams[StreamId] = &createArgs->scParams[StreamId];
        pScParams[StreamId]->inWidth = pInstPrm->sensorOutWidth;
        pScParams[StreamId]->inHeight = pInstPrm->sensorOutHeight;
        pScParams[StreamId]->inCropCfg.cropStartX = 0;
        pScParams[StreamId]->inCropCfg.cropStartY = 0;
        pScParams[StreamId]->inCropCfg.cropWidth = pScParams[StreamId]->inWidth;
        pScParams[StreamId]->inCropCfg.cropHeight = pScParams[StreamId]->inHeight;
        pScParams[StreamId]->outWidth = pOutPrm->scOutWidth;
        pScParams[StreamId]->outHeight = pOutPrm->scOutHeight;

		createArgs->pitch[StreamId] = pOutPrm->scOutWidth;

		if (gVstabEnable == 1)
		{
		    createArgs->pitch[StreamId] = (pOutPrm->scOutWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
		}

        pScParams[StreamId]->scConfig = NULL;
        pScParams[StreamId]->scCoeffConfig = NULL;

    }

    for (StreamId = 0; StreamId < ISS_CAPT_STREAM_ID_MAX; StreamId++)
    {
        pOutInfo = &createArgs->outStreamInfo[StreamId];

        pOutInfo->memType = ISS_TILEDMEM;//ISS_NONTILEDMEM;

        pOutInfo->dataFormat = FVID2_DF_INVALID;

        pOutInfo->scEnable = FALSE;
        pOutInfo->subFrameModeEnable = FALSE;
        pOutInfo->numLinesInSubFrame = 0;
        pOutInfo->subFrameCb = NULL;

        if (StreamId == 0 || StreamId == 1)
        {
            pOutInfo->dataFormat = FVID2_DF_YUV420SP_UV;
        }

        for (chId = 0; chId < ISS_CAPT_CH_PER_PORT_MAX; chId++)
        {
            createArgs->channelNumMap[StreamId][chId] =
                Iss_captMakeChannelNum(0, StreamId, chId);
        }
    }

    return 0;
}

/* ===================================================================
 *  @func     CameraLink_getCpuLoad
 *
 *  @desc     Function does the following
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
Int32 CameraLink_getCpuLoad()
{
    gCameraLink_obj.totalCpuLoad += Load_getCPULoad();
    gCameraLink_obj.cpuLoadCount++;

    return 0;
}

/* ===================================================================
 *  @func     CameraLink_drvPrintStatus
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvPrintStatus(CameraLink_Obj * pObj)
{
    UInt32 fps;

    FVID2_control(pObj->fvidHandleVipAll,
                  IOCTL_ISS_CAPT_PRINT_ADV_STATISTICS,
                  (Ptr) pObj->exeTime, NULL);

    fps = (pObj->cameraDequeuedFrameCount * 100) / (pObj->exeTime / 10);

    Vps_printf(" %d: CAMERA: Fields = %d (fps = %d, CPU Load = %d)\r\n",
               Clock_getTicks(),
               pObj->cameraDequeuedFrameCount,
               fps, pObj->totalCpuLoad / pObj->cpuLoadCount);

    Vps_printf(" %d: CAMERA: Num Resets = %d (Avg %d ms per reset)\r\n",
               Clock_getTicks(),
               pObj->resetCount, pObj->resetTotalTime / pObj->resetCount);

    return 0;
}

/* ===================================================================
 *  @func     CameraLink_drvSetBrightness
 *  ==================================================================
 */
Int32 CameraLink_drvSetBrightness(CameraLink_Obj * pObj, Int32 brightness)
{
    CameraLink_InstObj *pInst;
    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        pInst->brightness = brightness;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_BRIGHTNESS,
							   &pInst->brightness, NULL);

		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetContrast
 *  ==================================================================
 */
Int32 CameraLink_drvSetContrast(CameraLink_Obj * pObj, Int32 contrast)
{
    CameraLink_InstObj *pInst;
    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        pInst->contrast = contrast;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_CONTRAST,
							   &pInst->contrast, NULL);

		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetSharpness
 *  ==================================================================
 */
Int32 CameraLink_drvSetSharpness(CameraLink_Obj * pObj, Int32 sharpness)
{
    CameraLink_InstObj *pInst;
    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        pInst->sharpness = sharpness;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_SHARPNESS,
							   &pInst->sharpness, NULL);

		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvDCCBufAddr
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvDCCBufAddr(CameraLink_Obj * pObj, Int32 dccBufAddr, UInt32 dccSize)
{
    Iss_CaptDccPrm dccPrm;
    Int32 instId;
    Int32 status;
    CameraLink_InstObj *pInst;

    gCameraLink_obj.dcc_Default_Param = (UInt8 *) dccBufAddr;
    gCameraLink_obj.dcc_init_done = TRUE;

	dccPrm.dcc_Default_Param = gCameraLink_obj.dcc_Default_Param;
	dccPrm.dcc_init_done     = TRUE;
	dccPrm.dccSize           = dccSize;

	dcc_buf_size             = dccSize;
	memcpy(dcc_buf,(void *)dccBufAddr,dccSize);

	for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
	{
		pInst = &pObj->instObj[instId];
		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_DCCPRM,
							   &dccPrm, NULL);
		UTILS_assert(status == FVID2_SOK);
	}

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetAewbVendor
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetAewbVendor(CameraLink_Obj * pObj, Int32 aewbVendorId)
{
    CameraLink_InstObj *pInst;

    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        if ((aewbVendorId >= AEWB_ID_MAXNUM) || (aewbVendorId < 0))
            pInst->aewbVendor = AEWB_ID_APPRO;
        else
            pInst->aewbVendor = aewbVendorId;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_AEWBVENDOR,
							   &pInst->aewbVendor, NULL);
		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetAewbMode
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetAewbMode(CameraLink_Obj * pObj, Int32 aewbMode)
{
    CameraLink_InstObj *pInst;

    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        if ((aewbMode >= AEWB_MODE_MAXNUM) || (aewbMode < 0))
            pInst->aewbMode = AEWB_AEWB;
        else
            pInst->aewbMode = aewbMode;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_AEWBMODE,
							   &pInst->aewbMode, NULL);
		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetIRIS
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetIRIS(CameraLink_Obj * pObj, Int32 IRIS)
{
    CameraLink_InstObj *pInst;

    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        pInst->AutoIris = IRIS;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_IRIS,
							   &pInst->AutoIris, NULL);
		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetFrameCtrl
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetFrameCtrl(CameraLink_Obj * pObj, Int32 framectrl)
{
    CameraLink_InstObj *pInst;

    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        pInst->FrameRate = gSensorFramerate;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_FRAMERATE,
							   &pInst->FrameRate, NULL);
		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetAewbPriority
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetAewbPriority(CameraLink_Obj * pObj, Int32 aewbPriority)
{
    CameraLink_InstObj *pInst;

    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        if ((aewbPriority >= PRIORITY_MAXNUM) || (aewbPriority < 0))
            pInst->aewbPriority = PRIORITY_FRAMERATE;
        else
            pInst->aewbPriority = aewbPriority;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_AEWBPRI,
							   &pInst->aewbPriority, NULL);
		UTILS_assert(status == FVID2_SOK);
    }
    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetBLC
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetBLC(CameraLink_Obj * pObj, Int32 blc)
{
    CameraLink_InstObj *pInst;

    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        pInst->blc = blc;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_BLC,
							   &pInst->blc, NULL);
		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetAWBMode
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetAWBMode(CameraLink_Obj * pObj, Int32 AWBMode)
{
    CameraLink_InstObj *pInst;

    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        pInst->AWBMode = AWBMode;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_AWBMODE,
							   &pInst->AWBMode, NULL);
		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetAEMode
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetAEMode(CameraLink_Obj * pObj, Int32 AEMode)
{
    CameraLink_InstObj *pInst;

    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        pInst->AEMode = AEMode;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_AEMODE,
							   &pInst->AEMode, NULL);
		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetENV
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetENV(CameraLink_Obj * pObj, Int32 Env)
{
    CameraLink_InstObj *pInst;

    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        pInst->Env = Env;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_ENV,
							   &pInst->Env, NULL);
		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetBinning
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetBinning(CameraLink_Obj * pObj, Int32 Binning)
{
    CameraLink_InstObj *pInst;

    Int32 instId;
    Int32 status;

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInst = &pObj->instObj[instId];
        pInst->Binning = Binning;

		status = FVID2_control(pInst->cameraVipHandle,
							   IOCTL_ISS_CAPT_SET_BINNING,
							   &pInst->Binning, NULL);
		UTILS_assert(status == FVID2_SOK);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     CameraLink_drvSetResolution
 *
 *  @desc     Function does the following
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
Int32 CameraLink_drvSetResolution(CameraLink_Obj * pObj, UInt16 instId,
                                  UInt16 StreamId,Int32 Width,Int32 Height)
{
    Iss_CaptResolution resolutionParams;

    Int32 status;

    CameraLink_InstObj *pInst;

    Iss_CaptCreateParams *pVipCreateArgs;

    pInst = &pObj->instObj[instId];
    pVipCreateArgs = &pInst->createArgs;

    /*
     * set frame skip using a IOCTL if enabled
     */
	if(StreamId == 0)
	PrimaryStreamRtParams = 1;
	else if(StreamId == 1)
	SecondaryStreamRtParams = 1;
	resolutionParams.channelNum =
		pVipCreateArgs->channelNumMap[StreamId][0];

	resolutionParams.ResolutionWidth = Width;
	resolutionParams.ResolutionHeight = Height;

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_RESOLUTION,
						   &resolutionParams, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_drvSetCrop(CameraLink_Obj * pObj, UInt16 instId,
							CameraLink_CropParams *param)
{
    Iss_CaptResolution cropParams;
    Int32 status;
    CameraLink_InstObj *pInst;
    Iss_CaptCreateParams *pVipCreateArgs;

    pInst = &pObj->instObj[instId];
    pVipCreateArgs = &pInst->createArgs;

	#if 0
    /* set frame skip using a IOCTL if enabled */
	if(param->stream == 0)
		PrimaryStreamRtParams = 1;
	else if(param->stream == 1)
		SecondaryStreamRtParams = 1;
	#endif

	cropParams.channelNum = pVipCreateArgs->channelNumMap[param->stream][0];
	cropParams.CropStartX = param->sx;
	cropParams.CropStartY = param->sy;
	cropParams.ResolutionWidth = param->wi;
	cropParams.ResolutionHeight = param->he;

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_CROP,
						   &cropParams, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_drvSetFormat(CameraLink_Obj * pObj, UInt16 streamId,Int32 standard)
{
    Int32 status = FVID2_SOK;

	Int32 width, height;

	CameraLink_VipInstParams *pInstPrm = &pObj->createArgs.vipInst[0];

	if(streamId == 1)
	{
		if (standard == SYSTEM_STD_NTSC)
		{
			width  = 720;
			height = 480;
		}
		else if (standard == SYSTEM_STD_PAL)
		{
			width  = 720;
			height = 576;
		}
		else
		{
			Vps_rprintf("CameraLink_drvSetFormat %d unsupported\n", standard);
			return FVID2_EINVALID_PARAMS;
		}

		pInstPrm->outParams[1].standard = standard;
		CameraLink_drvSetResolution(pObj, 0, 1, width, height);
	}

    return status;
}

UInt32 CameraLink_getHistData(UInt32 *pHistAddr)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_HISTADDR,
						   pHistAddr, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

UInt32 CameraLink_getAfData(UInt32 *AFValue)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_AFDATA,
						   AFValue, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

#ifdef ISS_DEV_ON
Int32 CameraLink_getAlgVersion(UInt32 *param)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_ALG_VERSION,
						   param, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}
#endif

Int32 CameraLink_drvWriteSensor(CameraLink_Obj * pObj, CameraLink_SensorRegParams *param)
{
    Int32 status;
    CameraLink_InstObj *pInst;
    Iss_VideoDecoderRegRdWrParams regParam;

    pInst = &pObj->instObj[0];

    regParam.reg_addr = param->regAddr;
    regParam.reg_val = param->regValue;

    status = FVID2_control(pInst->SensorHandle,
                        IOCTL_ISS_SENSOR_REG_WRITE,
                        &regParam, NULL);

    UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;

}

Int32 CameraLink_drvReadSensor(CameraLink_Obj * pObj, CameraLink_SensorRegParams *param)
{
    Int32 status;
    CameraLink_InstObj *pInst;
    Iss_VideoDecoderRegRdWrParams regParam;

    pInst = &pObj->instObj[0];

    regParam.reg_addr = param->regAddr;
    regParam.reg_val = param->regValue;

    status = FVID2_control(pInst->SensorHandle,
                        IOCTL_ISS_SENSOR_REG_READ,
                        &regParam, NULL);

    UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;

}

Int32 CameraLink_drvGetSensorID(CameraLink_Obj * pObj)
{
	Int32 status = FVID2_SOK;
    CameraLink_InstObj *pInst = &pObj->instObj[0];

    Vps_printf("%s\n", __func__);

    status = FVID2_control(
        pInst->SensorHandle,
        IOCTL_ISS_SENSOR_GET_CHIP_ID,
        NULL,
		&pInst->sensorChipIdStatus
	);

	if(status != FVID2_SOK)
		pInst->cam_detect = 0;
	else
		pInst->cam_detect = 1;

	return status;
}

#ifdef UDISP_TOOL
Int32 CameraLink_setClamp(CameraLink_Obj * pObj, Iss_Clamp *params)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_BALCKCLAMP,
						   params, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_getClamp(CameraLink_Obj * pObj, Iss_Clamp *params)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_BALCKCLAMP,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_setIsifWb(CameraLink_Obj * pObj, Iss_isifWb *params)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_ISIFWB,
						   params, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_getIsifWb(CameraLink_Obj * pObj, Iss_isifWb *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];



	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_ISIFWB,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_setNF1(CameraLink_Obj * pObj, Iss_noiseFilter *params)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_NOISEFILTER1,
						   params, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_getNF1(CameraLink_Obj * pObj, Iss_noiseFilter *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];



	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_NOISEFILTER1,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_setNF2(CameraLink_Obj * pObj, Iss_noiseFilter *params)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_NOISEFILTER2,
						   params, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_getNF2(CameraLink_Obj * pObj, Iss_noiseFilter *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];



	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_NOISEFILTER2,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_setEE(CameraLink_Obj * pObj, Iss_edgeEnhancer *params)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_EDGEENHANCER,
						   params, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_getEE(CameraLink_Obj * pObj, Iss_edgeEnhancer *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];



	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_EDGEENHANCER,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_setRgb2rgb1(CameraLink_Obj * pObj, Iss_rgb2rgb *params)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_RGB2RGB1,
						   params, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}
Int32 CameraLink_getRgb2rgb1(CameraLink_Obj * pObj, Iss_rgb2rgb *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];



	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_RGB2RGB1,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_setRgb2rgb2(CameraLink_Obj * pObj, Iss_rgb2rgb *params)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_RGB2RGB2,
						   params, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_getRgb2rgb2(CameraLink_Obj * pObj, Iss_rgb2rgb *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];



	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_RGB2RGB2,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_setRgb2yuv(CameraLink_Obj * pObj, Iss_rgb2yuv *params)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_RGB2YCBCR,
						   params, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_getRgb2yuv(CameraLink_Obj * pObj, Iss_rgb2yuv *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];



	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_RGB2YCBCR,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_setIpipeWb(CameraLink_Obj * pObj, Iss_ipipeWb *params)
{
	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_IPIPEWB,
						   params, NULL);
	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_getIpipeWb(CameraLink_Obj * pObj, Iss_ipipeWb *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];



	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_IPIPEWB,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_setGbce(CameraLink_Obj * pObj, Iss_gbce *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];



	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_GBCE,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_getGbce(CameraLink_Obj * pObj, Iss_gbce *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];



	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_GBCE,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_setGamma(CameraLink_Obj * pObj, Iss_gamma *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_SET_GAMMA,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}

Int32 CameraLink_getGamma(CameraLink_Obj * pObj, Iss_gamma *params)
{

	Int32 status = FVID2_SOK;
	CameraLink_Obj *pObj      = &gCameraLink_obj;
	CameraLink_InstObj *pInst = &pObj->instObj[0];

	status = FVID2_control(pInst->cameraVipHandle,
						   IOCTL_ISS_CAPT_GET_GAMMA,
						   params, NULL);

	UTILS_assert(status == FVID2_SOK);

    return FVID2_SOK;
}
#endif //# UDISP_TOOL
