/** ==================================================================
 *  @file   ti_vcam.c
 *
 *  @path    ipnc_mcfw/mcfw/src_linux/mcfw_api/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

#include "ti_vsys_priv.h"
#include "ti_vcam_priv.h"

/* =============================================================================
 * Globals
 * ============================================================================= */

VCAM_MODULE_CONTEXT_S gVcamModuleContext;

/* =============================================================================
 * Vcam module APIs
 * ============================================================================= */

/* ===================================================================
 *  @func     Vcam_params_init
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
Void Vcam_params_init(VCAM_PARAMS_S * pContext)
{
    UInt16 devId, chnId, strmId;

    for (devId = 0; devId < VCAM_DEV_MAX; devId++)
    {
        pContext->deviceParams[devId].portEnable = FALSE;
        pContext->deviceParams[devId].portMode = VCAM_MODE_YUV420;
        pContext->deviceParams[devId].dataFormat = VCAM_MULTICHN_OPTION_SINGLE_CHN;
        pContext->deviceParams[devId].signalType = VS_AUTO_DETECT;
    }

    for (chnId = 0; chnId < VCAM_CHN_MAX; chnId++)
    {
        for (strmId = 0; strmId < VCAM_STRM_MAX; strmId++)
        {
            pContext->channelParams[chnId].strmEnable[strmId] = FALSE;
            pContext->channelParams[chnId].strmFormat[strmId] = VF_YUV422I_UYVY;
            pContext->channelParams[chnId].strmResolution[strmId].start_X = 0;
            pContext->channelParams[chnId].strmResolution[strmId].start_Y = 0;
            pContext->channelParams[chnId].strmResolution[strmId].width = 0;
            pContext->channelParams[chnId].strmResolution[strmId].height = 0;
        }
        pContext->channelParams[chnId].deviceId = 0;
        pContext->channelParams[chnId].dynamicParams.contrast = 0;
        pContext->channelParams[chnId].dynamicParams.saturation = 0;
        pContext->channelParams[chnId].dynamicParams.brightness = 0;
        pContext->channelParams[chnId].dynamicParams.hue = 0;
    }
}

/* ===================================================================
 *  @func     Vcam_init
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
Int32 Vcam_init(VCAM_PARAMS_S * pContext)
{
    UInt16 devId, chnId, strmId;

    /* Mark all links related to camera as invalid by default, they will be
     * setup with valid IDs later */
    gVcamModuleContext.cameraId	= SYSTEM_LINK_ID_INVALID;
    gVcamModuleContext.ipcFramesOutVpssToHostId = SYSTEM_LINK_ID_INVALID;
    gVcamModuleContext.ipcFramesInHostId = SYSTEM_LINK_ID_INVALID;

    /* initialize counters */
    gVcamModuleContext.numDevices = 0;
    gVcamModuleContext.numChannels = 0;

    if (pContext == NULL)
    {
        Vcam_params_init(&gVcamModuleContext.vcamConfig);
    }
    else
    {
        for (devId = 0; devId < VCAM_DEV_MAX; devId++)
        {
            if (pContext->deviceParams[devId].portEnable) {
                gVcamModuleContext.numDevices++;
            }
        }

        for (chnId = 0; chnId < VCAM_CHN_MAX; chnId++)
        {
            for (strmId = 0; strmId < VCAM_STRM_MAX; strmId++)
            {
                if (pContext->channelParams[chnId].strmEnable[strmId]) {
                    gVcamModuleContext.numChannels++;
                }
            }
        }

        memcpy(&gVcamModuleContext.vcamConfig, pContext, sizeof(VCAM_PARAMS_S));
    }

    return 0;
}

/* ===================================================================
 *  @func     Vcam_exit
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
Void Vcam_exit()
{
    /* Empty for now */
}

/* ===================================================================
 *  @func     Vcam_start
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
Int32 Vcam_start()
{
    /* start can happen in any order, except its recommended to start camera
     * Link the last */
    if (gVcamModuleContext.ipcFramesOutVpssToHostId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkStart(gVcamModuleContext.ipcFramesOutVpssToHostId);
    }

    if (gVcamModuleContext.ipcFramesInHostId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkStart(gVcamModuleContext.ipcFramesInHostId);
    }

    /* Start taking CPU load just before starting of camera link */
    MultiCh_prfLoadCalcEnable(TRUE, FALSE, FALSE);

    if (gVcamModuleContext.cameraId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkStart(gVcamModuleContext.cameraId);
    }

    return 0;
}

/* ===================================================================
 *  @func     Vcam_stop
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
Int32 Vcam_stop()
{
    /* stop needs to be in the reseverse order of create */
    if (gVcamModuleContext.cameraId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkStop(gVcamModuleContext.cameraId);
    }

    if (gVcamModuleContext.ipcFramesOutVpssToHostId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkStop(gVcamModuleContext.ipcFramesOutVpssToHostId);
    }

    if (gVcamModuleContext.ipcFramesInHostId != SYSTEM_LINK_ID_INVALID)
    {
        System_linkStop(gVcamModuleContext.ipcFramesInHostId);
    }

    return 0;
}

/* ===================================================================
 *  @func     Vcam_setParamDevice
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
Int32 Vcam_setParamDevice(VCAM_DEV vcDevId, VCAM_DEV_PARAM_S * psvcDevParam,
                          VCAM_PARAMS_E paramId)
{
    memcpy(&gVcamModuleContext.vcamConfig.deviceParams[vcDevId], psvcDevParam,
           sizeof(VCAM_DEV_PARAM_S));
    return 0;
}

/* ===================================================================
 *  @func     Vcam_getParamDevice
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
Int32 Vcam_getParamDevice(VCAM_DEV vcDevId, VCAM_DEV_PARAM_S * psvcDevParam,
                          VCAM_PARAMS_E paramId)
{
    memcpy(psvcDevParam, &gVcamModuleContext.vcamConfig.deviceParams[vcDevId],
           sizeof(VCAM_DEV_PARAM_S));
    return 0;
}

/* ===================================================================
 *  @func     Vcam_enableDevice
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
Int32 Vcam_enableDevice(VCAM_DEV vcDevId)
{
    return 0;
}

/* ===================================================================
 *  @func     Vcam_disableDevice
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
Int32 Vcam_disableDevice(VCAM_DEV vcDevId)
{
    return 0;
}

/* ===================================================================
 *  @func     Vcam_setParamChn
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
Int32 Vcam_setParamChn(VCAM_CHN vcChnId, VCAM_CHN_PARAM_S * psCamChnParam,
                       VCAM_PARAMS_E paramId)
{
    memcpy(&gVcamModuleContext.vcamConfig.channelParams[vcChnId], psCamChnParam,
           sizeof(VCAM_CHN_PARAM_S));
    return 0;
}

/* ===================================================================
 *  @func     Vcam_getParamChn
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
Int32 Vcam_getParamChn(VCAM_CHN vcChnId, VCAM_CHN_PARAM_S * psCamChnParam,
                       VCAM_PARAMS_E paramId)
{
    memcpy(psCamChnParam, &gVcamModuleContext.vcamConfig.channelParams[vcChnId],
           sizeof(VCAM_CHN_PARAM_S));
    return 0;
}

/* ===================================================================
 *  @func     Vcam_setDynamicParamChn
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
Int32 Vcam_setDynamicParamChn(VCAM_CHN vcChnId,
                              VCAM_CHN_DYNAMIC_PARAM_S * psCamChnDynaParam,
                              VCAM_PARAMS_E paramId)
{
    Int32 status = ERROR_NONE;

    if (gVcamModuleContext.cameraId == SYSTEM_LINK_ID_INVALID)
        return ERROR_FAIL;

    switch (paramId)
    {
        case VCAM_FORMAT:
            break;
        case VCAM_RESOLUTION:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_RESOLUTION,
                                        &(psCamChnDynaParam->resolution),
                                        sizeof(psCamChnDynaParam->resolution),
                                        TRUE);
            break;
        case VCAM_DCCBUFADDR:
        {
            CameraLink_DccParams params;
            params.dccBufAddr = psCamChnDynaParam->dccBufAddr;
            params.dccSize = psCamChnDynaParam->dccSize;
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_DCC_BUFFER,
                                        &(params), sizeof(params), TRUE);
            break;
        }
        case VCAM_AEWB_VENDOR:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_AEWB_VENDOR,
                                        &(psCamChnDynaParam->aewbVendor),
                                        sizeof(psCamChnDynaParam->aewbVendor),
                                        TRUE);
            break;

        case VCAM_MIRROR_MODE:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_MIRROR_MODE,
                                        &(psCamChnDynaParam->mirrorMode),
                                        sizeof(psCamChnDynaParam->mirrorMode),
                                        TRUE);
            break;
        case VCAM_AEWB_MODE:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_AEWB_MODE,
                                        &(psCamChnDynaParam->aewbMode),
                                        sizeof(psCamChnDynaParam->aewbMode),
                                        TRUE);
            break;
        case VCAM_AEWB_PRIORITY:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_AEWB_PRIORITY,
                                        &(psCamChnDynaParam->aewbPriority),
                                        sizeof(psCamChnDynaParam->aewbPriority),
                                        TRUE);
            break;
        case VCAM_IRIS:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_IRIS,
                                        &(psCamChnDynaParam->IRIS),
                                        sizeof(psCamChnDynaParam->IRIS), TRUE);
            break;
        case VCAM_FRAMECTRL:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_FRAMECTRL,
                                        &(psCamChnDynaParam->frameCtrl),
                                        sizeof(psCamChnDynaParam->frameCtrl),
                                        TRUE);
            break;
        case VCAM_CONTRAST:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_CONTRAST,
                                        &(psCamChnDynaParam->contrast),
                                        sizeof(psCamChnDynaParam->contrast),
                                        TRUE);
            break;
        case VCAM_SATURATION:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_SATURATION,
                                        &(psCamChnDynaParam->saturation),
                                        sizeof(psCamChnDynaParam->saturation),
                                        TRUE);
            break;
        case VCAM_BRIGHTNESS:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_BRIGHTNESS,
                                        &(psCamChnDynaParam->brightness),
                                        sizeof(psCamChnDynaParam->brightness),
                                        TRUE);
            break;
        case VCAM_SHARPNESS:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_SHARPNESS,
                                        &(psCamChnDynaParam->sharpness),
                                        sizeof(psCamChnDynaParam->sharpness),
                                        TRUE);
            break;
        case VCAM_BLC:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_BLC,
                                        &(psCamChnDynaParam->blc),
                                        sizeof(psCamChnDynaParam->blc), TRUE);
            break;
        case VCAM_AWBMODE:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_AWBMODE,
                                        &(psCamChnDynaParam->awbMode),
                                        sizeof(psCamChnDynaParam->awbMode),
                                        TRUE);
            break;
        case VCAM_FRAMERATE:
        {
            CameraLink_FpsParams params;

            params.StreamId = vcChnId;
            params.targetFps = psCamChnDynaParam->frameRate;
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_FRAMERATE,
                                        &(params), sizeof(params), TRUE);
            break;
        }
        case VCAM_AEMODE:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_AEMODE,
                                        &(psCamChnDynaParam->AEMode),
                                        sizeof(psCamChnDynaParam->AEMode),
                                        TRUE);
            break;
        case VCAM_ENV_50_60HZ:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_ENV_50_60HZ,
                                        &(psCamChnDynaParam->env50_60hz),
                                        sizeof(psCamChnDynaParam->env50_60hz),
                                        TRUE);
            break;
        case VCAM_BINNING_SKIP:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_BINNING,
                                        &(psCamChnDynaParam->binningMode),
                                        sizeof(psCamChnDynaParam->binningMode),
                                        TRUE);
            break;

        case VCAM_HUE:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_HUE,
                                        &(psCamChnDynaParam->hue),
                                        sizeof(psCamChnDynaParam->hue), TRUE);
            break;
        case VCAM_LDC:
            break;
        case VCAM_TNF:
            break;
        case VCAM_SNF:
            break;
        case VCAM_PORTMODE:
            break;
        case VCAM_SIGNALTYPE:
            break;
		case VCAM_SD_FORMAT:
			status = System_linkControl(gVcamModuleContext.cameraId,
										CAMERA_LINK_CMD_SET_FORMAT,
										&(psCamChnDynaParam->sdFormat),
										sizeof(psCamChnDynaParam->
											   sdFormat), TRUE);
			break;
        case VCAM_ALL:
            status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_CHANGE_CONTRAST,
                                        &(psCamChnDynaParam->contrast),
                                        sizeof(psCamChnDynaParam->contrast),
                                        TRUE);
            if (status >= 0)
            {
                status = System_linkControl(gVcamModuleContext.cameraId,
                                            CAMERA_LINK_CMD_CHANGE_SATURATION,
                                            &(psCamChnDynaParam->saturation),
                                            sizeof(psCamChnDynaParam->
                                                   saturation), TRUE);
            }
            if (status >= 0)
            {
                status = System_linkControl(gVcamModuleContext.cameraId,
                                            CAMERA_LINK_CMD_CHANGE_BRIGHTNESS,
                                            &(psCamChnDynaParam->brightness),
                                            sizeof(psCamChnDynaParam->
                                                   brightness), TRUE);
            }
            if (status >= 0)
            {
                status = System_linkControl(gVcamModuleContext.cameraId,
                                            CAMERA_LINK_CMD_CHANGE_HUE,
                                            &(psCamChnDynaParam->hue),
                                            sizeof(psCamChnDynaParam->hue),
                                            TRUE);
            }
            break;

    }

    return status;
}

/* ===================================================================
 *  @func     Vcam_delete
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
Int32 Vcam_delete()
{
    /* delete can be done in any order */
    if (gVcamModuleContext.cameraId != SYSTEM_LINK_ID_INVALID)
        System_linkDelete(gVcamModuleContext.cameraId);

    return 0;
}

static Void Vcam_copyVidFrameInfoLink2McFw(VIDEO_FRAMEBUF_S *dstBuf,
                                           VIDFrame_Buf    *srcBuf)
{
    Int i,j;
    OSA_assert(VIDEO_MAX_FIELDS == VIDFRAME_MAX_FIELDS);
    OSA_assert(VIDEO_MAX_PLANES == VIDFRAME_MAX_PLANES);

    for (i = 0; i < VIDEO_MAX_FIELDS; i++)
    {
        for (j = 0; j < VIDEO_MAX_PLANES; j++)
        {
            dstBuf->addr[i][j] = srcBuf->addr[i][j];
            dstBuf->phyAddr[i][j] = srcBuf->phyAddr[i][j];
        }
    }
    dstBuf->channelNum  = srcBuf->channelNum;
    dstBuf->fid         = srcBuf->fid;
    dstBuf->frameWidth  = srcBuf->frameWidth;
    dstBuf->frameHeight = srcBuf->frameHeight;
    dstBuf->linkPrivate = srcBuf->linkPrivate;
    dstBuf->timeStamp   = srcBuf->timeStamp;
    dstBuf->framePitch[0] = srcBuf->framePitch[0];
    dstBuf->framePitch[1] = srcBuf->framePitch[1];
}

static Void Vcam_copyVidFrameInfoMcFw2Link(VIDFrame_Buf *dstBuf,
                                           VIDEO_FRAMEBUF_S    *srcBuf)
{
    Int i,j;
    OSA_assert(VIDEO_MAX_FIELDS == VIDFRAME_MAX_FIELDS);
    OSA_assert(VIDEO_MAX_PLANES == VIDFRAME_MAX_PLANES);

    for (i = 0; i < VIDEO_MAX_FIELDS; i++)
    {
        for (j = 0; j < VIDEO_MAX_PLANES; j++)
        {
            dstBuf->addr[i][j] = srcBuf->addr[i][j];
            dstBuf->phyAddr[i][j] = srcBuf->phyAddr[i][j];
        }
    }
    dstBuf->channelNum  = srcBuf->channelNum;
    dstBuf->fid         = srcBuf->fid;
    dstBuf->frameWidth  = srcBuf->frameWidth;
    dstBuf->frameHeight = srcBuf->frameHeight;
    dstBuf->linkPrivate = srcBuf->linkPrivate;
    dstBuf->timeStamp   = srcBuf->timeStamp;
    dstBuf->framePitch[0] = srcBuf->framePitch[0];
    dstBuf->framePitch[1] = srcBuf->framePitch[1];
}

/**
    \brief Request filled video buffers from framework

    User calls this API to get full video frames from the framework.
    After getting the video frames, user will
    - consume the video frames
    - and then call Vcam_putEmptyVideoFrames() to free the video frames back to the framework

    \param pFrameBufList    [OUT]  List of video frames returned by the framework
    \param timeout          [IN]   TIMEOUT_WAIT_FOREVER or TIMEOUT_NO_WAIT or timeout in msecs

    \return ERROR_NONE on success
*/
Int32 Vcam_getFullVideoFrames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList, UInt32 timeout)
{
    VIDFrame_BufList  vidBufList;
    VIDFrame_Buf     *pInBuf;
    VIDEO_FRAMEBUF_S *pOutBuf;
    UInt32 i;

    pFrameBufList->numFrames = 0;
    vidBufList.numFrames = 0;
    IpcFramesInLink_getFullVideoFrames(gVcamModuleContext.ipcFramesInHostId,
                                       &vidBufList);

    pFrameBufList->numFrames = vidBufList.numFrames;
    for (i = 0; i < vidBufList.numFrames; i++)
    {
        pOutBuf = &pFrameBufList->frames[i];
        pInBuf = &vidBufList.frames[i];

        Vcam_copyVidFrameInfoLink2McFw(pOutBuf,pInBuf);
    }

    return 0;
}
/**
    \brief Give consumed video frames back to the application to be freed

    Buffers that are were previously got from Vcam_getFullVideoFrames can be
    freed back to the framework by invoking this API.

    \param pFrameBufList [IN]   List of video frames

    \return ERROR_NONE on success
*/
Int32 Vcam_putEmptyVideoFrames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList)
{
    VIDEO_FRAMEBUF_S *pSrcBuf;
    VIDFrame_Buf     *pDstBuf;
    VIDFrame_BufList  vidBufList;
    UInt32 i;
    Int status = 0;

    vidBufList.numFrames = pFrameBufList->numFrames;
    for (i = 0; i < vidBufList.numFrames; i++)
    {
        pSrcBuf = &pFrameBufList->frames[i];
        pDstBuf = &vidBufList.frames[i];
        Vcam_copyVidFrameInfoMcFw2Link(pDstBuf,pSrcBuf);
    }
    if (vidBufList.numFrames)
    {
        status =
        IpcFramesInLink_putEmptyVideoFrames(gVcamModuleContext.ipcFramesInHostId,
                                            &vidBufList);
    }
    return 0;
}

/**
 * \brief:
 *              register call back which will post the message
 * \input:
 *              callback                -- callback function
 * \output:
 *              NA
 * \return
*               TI_MEDIA_SUCCESS        --      while success
*               ERROR_CODE                      --      refer for err defination
*/
Int32 Vcam_registerCallback(VCAM_CALLBACK_S * callback, Ptr arg)
{
    gVcamModuleContext.callbackFxn = *callback;
    gVcamModuleContext.callbackArg = arg;

    return 0;
}

/**
 * \brief:
 *              MCFW layer call back which will invoke the app registered callback
 * \input:
 *              callback                -- callback function
 * \output:
 *              NA
 * \return
*               TI_MEDIA_SUCCESS        --      while success
*               ERROR_CODE                      --      refer for err defination
*/
Void Vcam_ipcFramesInCbFxn(Ptr cbCtx)
{
    OSA_assert(cbCtx == &gVcamModuleContext);

    if (gVcamModuleContext.callbackFxn.newDataAvailableCb)
    {
        gVcamModuleContext.callbackFxn.newDataAvailableCb(gVcamModuleContext.callbackArg);
    }
}

Int32 Vcam_get_alg_version(UInt32 *version)
{
	Int32 status;
	UInt32 value;

	if(gVcamModuleContext.cameraId != SYSTEM_LINK_ID_INVALID)
    {
		status = System_linkControl(gVcamModuleContext.cameraId,
						CAMERA_LINK_CMD_GET_ALG_VERSION,
						&value, sizeof(value),
						TRUE);
		if(status != OSA_SOK)
			return -1;

		*version = value;
    }

	return 0;
}

Int32 Vcam_set_sensor_reg(UInt8 regaddr, UInt8 regval)
{
	Int32 status;
	CameraLink_SensorRegParams regPrm;

	if(gVcamModuleContext.cameraId != SYSTEM_LINK_ID_INVALID)
    {
    	regPrm.regAddr = regaddr;
		regPrm.regValue = regval;

		status = System_linkControl(gVcamModuleContext.cameraId,
						CAMERA_LINK_CMD_REG_WR_SENSOR,
						&regPrm, sizeof(regPrm),
						TRUE);
		if(status != OSA_SOK)
			return -1;
    }

	return 0;
}

Int32 Vcam_get_sensor_reg(UInt8 regaddr, UInt8 *regval)
{
	Int32 status;
	CameraLink_SensorRegParams regPrm;

	regPrm.regAddr = regaddr;

	if(gVcamModuleContext.cameraId != SYSTEM_LINK_ID_INVALID)
    {
		status = System_linkControl(gVcamModuleContext.cameraId,
						CAMERA_LINK_CMD_REG_RD_SENSOR,
						&regPrm, sizeof(regPrm),
						TRUE);
		if(status != OSA_SOK)
			return -1;

		*regval = regPrm.regValue = regval;
    }

	return 0;
}

Int32 Vcam_set_crop(Int32 stream, Int32 sx, Int32 sy, Int32 wi, Int32 he)
{
	Int32 status = 0;
	CameraLink_CropParams regPrm;

	//# check param
	if(wi > 1920)
		wi = 1920;
	else if(wi < 720)
		wi = 720;

	if(he > 1080)
		he = 1080;
	else if(he < 480)
		he = 480;

	if(sx > (1920-wi))
		sx = 1920-wi;
	if(sy > (1080-he))
		sy = 1080-he;


    if(gVcamModuleContext.cameraId != SYSTEM_LINK_ID_INVALID)
    {
    	regPrm.stream = stream;
		regPrm.sx = SystemUtils_floor(sx, 2);
		regPrm.sy = sy;
		regPrm.wi = wi;
		regPrm.he = he;

		status = System_linkControl(gVcamModuleContext.cameraId,
						CAMERA_LINK_CMD_CHANGE_CROP,
						&regPrm, sizeof(regPrm), TRUE);
    	if(status != OSA_SOK)
			return -1;
    }

	return 0;
}

Int32 Vcam_set_video_color(char option, Int32 value)
{
	Int32 status = 0;
	Int32 param;
	UInt32 cmd;

	//# real ranage : -3~3, input ranage: 1~7
	if(value < 0 || value > 7)
		return -1;

	if(gVcamModuleContext.cameraId != SYSTEM_LINK_ID_INVALID)
    {
    	if(option == VCAM_BRIGHTNESS)
    		cmd = CAMERA_LINK_CMD_CHANGE_BRIGHTNESS;
    	else if(option == VCAM_SHARPNESS)
    		cmd = CAMERA_LINK_CMD_CHANGE_SHARPNESS;
    	else if(option == VCAM_CONTRAST)
    		cmd = CAMERA_LINK_CMD_CHANGE_CONTRAST;
    	else
    		return -1;

    	param = value - 4;
		status = System_linkControl(gVcamModuleContext.cameraId,
									cmd,
									&param, sizeof(param), TRUE);
		if(status != OSA_SOK)
			return -1;
    }

	return 0;
}

#ifdef UDISP_TOOL
Int32 Vcam_set_ud_tuning(void *params, VCAM_UDISP_E paramId)
{
	Int32 status = 0;
	printf("Vcam_set_ud_tuning\n");
	switch (paramId)
    {    	
    	case VCAM_UDISP_BALCKCLAMP:
    	{
    		status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_BALCKCLAMP,
                                        params,
                                        sizeof(VCAM_UDISP_CLAMP_S),
                                        TRUE);  
			break;
		}          
		case VCAM_UDISP_ISIFWB:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_ISIFWB,
                                        params,
                                        sizeof(VCAM_UDISP_ISIFWB_S),
                                        TRUE);
			break;
		case VCAM_UDISP_NOISEFILTER1:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_NOISEFILTER1,
                                        params,
                                        sizeof(VCAM_UDISP_NF_S),
                                        TRUE);
			break;
		case VCAM_UDISP_NOISEFILTER2:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_NOISEFILTER2,
                                        params,
                                        sizeof(VCAM_UDISP_NF_S),
                                        TRUE);
			break;
		case VCAM_UDISP_EDGEENHANCER:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_EDGEENHANCER,
                                        params,
                                        sizeof(VCAM_UDISP_EE_S),
                                        TRUE);
			break;
		case VCAM_UDISP_RGB2RGB1:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_RGB2RGB1,
                                        params,
                                        sizeof(VCAM_UDISP_RGB2RGB_S),
                                        TRUE);
			break;
		case VCAM_UDISP_RGB2RGB2:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_RGB2RGB2,
                                        params,
                                        sizeof(VCAM_UDISP_RGB2RGB_S),
                                        TRUE);	
			break;
		case VCAM_UDISP_RGB2YCBCR:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_RGB2YCBCR,
                                        params,
                                        sizeof(VCAM_UDISP_RGB2YUV_S),
                                        TRUE);	
			break;
		case VCAM_UDISP_IPIPEWB:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_IPIPEWB,
                                        params,
                                        sizeof(VCAM_UDISP_IPIPEWB_S),
                                        TRUE);
			break;
		case VCAM_UDISP_GBCE:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_GBCE,
                                        params,
                                        sizeof(VCAM_UDISP_GBCE_S),
                                        TRUE);
			break;
		case VCAM_UDISP_GAMMA:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_SET_GAMMA,
                                        params,
                                        sizeof(VCAM_UDISP_GAMMA_S),
                                        TRUE);
            break;
		default:
			break;
    }
    
	return 0;	
}

Int32 Vcam_get_ud_tuning(void *params, VCAM_UDISP_E paramId)
{
	Int32 status = 0;

	switch (paramId)
    {
    	case VCAM_UDISP_BALCKCLAMP:
    		status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_BALCKCLAMP,
                                        params,
                                        sizeof(VCAM_UDISP_CLAMP_S),
                                        TRUE);     
			break;         
		case VCAM_UDISP_ISIFWB:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_ISIFWB,
                                        params,
                                        sizeof(VCAM_UDISP_ISIFWB_S),
                                        TRUE);
			break;
		case VCAM_UDISP_NOISEFILTER1:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_NOISEFILTER1,
                                        params,
                                        sizeof(VCAM_UDISP_NF_S),
                                        TRUE);
			break;
		case VCAM_UDISP_NOISEFILTER2:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_NOISEFILTER2,
                                        params,
                                        sizeof(VCAM_UDISP_NF_S),
                                        TRUE);
			break;
		case VCAM_UDISP_EDGEENHANCER:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_EDGEENHANCER,
                                        params,
                                        sizeof(VCAM_UDISP_EE_S),
                                        TRUE);
			break;
		case VCAM_UDISP_RGB2RGB1:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_RGB2RGB1,
                                        params,
                                        sizeof(VCAM_UDISP_RGB2RGB_S),
                                        TRUE);
			break;
		case VCAM_UDISP_RGB2RGB2:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_RGB2RGB2,
                                        params,
                                        sizeof(VCAM_UDISP_RGB2RGB_S),
                                        TRUE);	
			break;
		case VCAM_UDISP_RGB2YCBCR:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_RGB2YCBCR,
                                        params,
                                        sizeof(VCAM_UDISP_RGB2YUV_S),
                                        TRUE);	
			break;
		case VCAM_UDISP_IPIPEWB:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_IPIPEWB,
                                        params,
                                        sizeof(VCAM_UDISP_IPIPEWB_S),
                                        TRUE);
			break;
		case VCAM_UDISP_GBCE:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_GBCE,
                                        params,
                                        sizeof(VCAM_UDISP_GBCE_S),
                                        TRUE);
			break;
		case VCAM_UDISP_GAMMA:
			status = System_linkControl(gVcamModuleContext.cameraId,
                                        CAMERA_LINK_CMD_UDISP_GET_GAMMA,
                                        params,
                                        sizeof(VCAM_UDISP_GAMMA_S),
                                        TRUE);
			break;
		default:
			break;
    }
    
	return 0;	
}
#endif //# UDISP_TOOL
