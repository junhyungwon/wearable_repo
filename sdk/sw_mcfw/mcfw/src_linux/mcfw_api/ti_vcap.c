/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

#include "ti_vsys_priv.h"
#include "ti_vcap_priv.h"

#define VCAP_TRACE_ENABLE_FXN_ENTRY_EXIT           (0)
#define VCAP_TRACE_INFO_PRINT_INTERVAL             (8192)

#if VCAP_TRACE_ENABLE_FXN_ENTRY_EXIT
#define VCAP_TRACE_FXN(str,...)		do { \
											static Int printInterval = 0; \
											if ((printInterval % VCAP_TRACE_INFO_PRINT_INTERVAL) == 0) { \
												OSA_printf("TI_VCAP:%s function:%s",str,__func__);     \
												OSA_printf(__VA_ARGS__);                               \
											}                                                          \
											printInterval++;                                           \
											} while (0)
#define VCAP_TRACE_FXN_ENTRY(...)	VCAP_TRACE_FXN("Entered",__VA_ARGS__)
#define VCAP_TRACE_FXN_EXIT(...)	VCAP_TRACE_FXN("Leaving",__VA_ARGS__)
#else
#define VCAP_TRACE_FXN_ENTRY(...)
#define VCAP_TRACE_FXN_EXIT(...)
#endif

/* =============================================================================
 * Globals
 * =============================================================================
 */

VCAP_MODULE_CONTEXT_S gVcapModuleContext;

/* =============================================================================
 * Vcap module APIs
 * =============================================================================
 */

void Vcap_params_init(VCAP_PARAMS_S * pContext)
{
    UInt16 devId, chnId, strmId;

    memset(pContext, 0, sizeof(VCAP_PARAMS_S));
    for(devId = 0; devId < VCAP_DEV_MAX; devId++)
    {
        pContext->deviceParams[devId].portEnable = FALSE;
        pContext->deviceParams[devId].portMode   = VCAP_MODE_BT656_8BIT_YUV422;
        pContext->deviceParams[devId].dataFormat = VCAP_MULTICHN_OPTION_4D1;
        pContext->deviceParams[devId].signalType = VS_AUTO_DETECT;
    }
    for(chnId = 0; chnId < VCAP_CHN_MAX; chnId++)
    {
        for(strmId=0; strmId<VCAP_STRM_MAX; strmId++)
        {
            pContext->channelParams[chnId].strmEnable[strmId]             = FALSE;
            pContext->channelParams[chnId].strmFormat[strmId]             = VF_YUV422I_UYVY;
            pContext->channelParams[chnId].strmResolution[strmId].start_X = 0;
            pContext->channelParams[chnId].strmResolution[strmId].start_Y = 0;
            pContext->channelParams[chnId].strmResolution[strmId].width   = 0;
            pContext->channelParams[chnId].strmResolution[strmId].height  = 0;
        }
        pContext->channelParams[chnId].deviceId                   = 0;
        pContext->channelParams[chnId].dynamicParams.contrast     = 0;
        pContext->channelParams[chnId].dynamicParams.satauration  = 0;
        pContext->channelParams[chnId].dynamicParams.brightness   = 0;
        pContext->channelParams[chnId].dynamicParams.hue          = 0;
    }

    pContext->enableConfigExtVideoDecoder = TRUE;
}

Int32 Vcap_init(VCAP_PARAMS_S * pContext)
{
    //UInt16 devId,
    UInt16 chnId, strmId, linkId;

    /* Mark all links related to capture as invalid by default, they will be setup with valid IDs later */
    gVcapModuleContext.captureId             = SYSTEM_LINK_ID_INVALID;
    for(linkId=0; linkId<MAX_NSF_LINK; linkId++)
    {
        gVcapModuleContext.nsfId[linkId]                 = SYSTEM_LINK_ID_INVALID;
    }
    gVcapModuleContext.capSwMsId             = SYSTEM_LINK_ID_INVALID;
    for(linkId=0; linkId<MAX_SCLR_LINK; linkId++)
    {
        gVcapModuleContext.sclrId[linkId]             = SYSTEM_LINK_ID_INVALID;
    }
    for(linkId=0; linkId<MAX_ALG_LINK; linkId++)
    {
        gVcapModuleContext.dspAlgId[linkId]           = SYSTEM_LINK_ID_INVALID;
    }
    for(linkId=0; linkId<MAX_IPC_FRAMES_LINK; linkId++)
    {
        gVcapModuleContext.ipcFramesOutVpssId[linkId]    = SYSTEM_LINK_ID_INVALID;
        gVcapModuleContext.ipcFramesInDspId[linkId]      = SYSTEM_LINK_ID_INVALID;
    }
    for(linkId=0; linkId<MAX_DEI_LINK; linkId++)
    {
        gVcapModuleContext.deiId[linkId] = SYSTEM_LINK_ID_INVALID;
    }
    gVcapModuleContext.nullSrcId                = SYSTEM_LINK_ID_INVALID;
    gVcapModuleContext.ipcFramesOutVpssToHostId = SYSTEM_LINK_ID_INVALID;
    gVcapModuleContext.ipcFramesInHostId        = SYSTEM_LINK_ID_INVALID;
    gVcapModuleContext.ipcBitsInHLOSId          = SYSTEM_LINK_ID_INVALID;
    gVcapModuleContext.callbackFxn.newDataAvailableCb       = NULL;
    gVcapModuleContext.callbackArg                          = NULL;
    gVcapModuleContext.bitscallbackFxn.newDataAvailableCb   = NULL;
    gVcapModuleContext.bitscallbackArg                      = NULL;

    /* initialize counters */
    gVcapModuleContext.numChannels = 0;

    if(pContext==NULL)
    {
        Vcap_params_init(&gVcapModuleContext.vcapConfig);
    }
    else
    {
        for (chnId = 0; chnId < VCAP_CHN_MAX; chnId++)
        {
            for(strmId=0; strmId<VCAP_STRM_MAX; strmId++)
            {
                if(pContext->channelParams[chnId].strmEnable[strmId])
                {
                    gVcapModuleContext.numChannels++;
                }
            }
        }

        memcpy(&gVcapModuleContext.vcapConfig, pContext, sizeof(VCAP_PARAMS_S));
        gVcapModuleContext.numChannels = pContext->numChn;
    }

    return 0;
}


Void Vcap_exit()
{
    /* Empty for now */
}


Int32 Vcap_start()
{
    UInt32 linkId;

	/* start can happen in any order, except its recommended to start capture Link the last */
	if(gVcapModuleContext.ipcFramesOutVpssToHostId!=SYSTEM_LINK_ID_INVALID)
	{
	    System_linkStart(gVcapModuleContext.ipcFramesOutVpssToHostId);
	}

	if(gVcapModuleContext.ipcFramesInHostId!=SYSTEM_LINK_ID_INVALID)
	{
	    System_linkStart(gVcapModuleContext.ipcFramesInHostId);
	}

	if(gVcapModuleContext.ipcBitsInHLOSId!=SYSTEM_LINK_ID_INVALID)
	{
	    System_linkStart(gVcapModuleContext.ipcBitsInHLOSId);
	}

	for(linkId = 0; linkId < MAX_IPC_FRAMES_LINK; linkId++)
	{
	    if(gVcapModuleContext.ipcFramesOutVpssId[linkId]!=SYSTEM_LINK_ID_INVALID)
	    {
	        System_linkStart(gVcapModuleContext.ipcFramesOutVpssId[linkId]);
	    }
	    if(gVcapModuleContext.ipcFramesInDspId[linkId]!=SYSTEM_LINK_ID_INVALID)
	    {
	        System_linkStart(gVcapModuleContext.ipcFramesInDspId[linkId]);
	    }
	}

	for(linkId=0; linkId<MAX_NSF_LINK; linkId++)
	{
	    if(gVcapModuleContext.nsfId[linkId]!=SYSTEM_LINK_ID_INVALID)
	    {
	        System_linkStart(gVcapModuleContext.nsfId[linkId]);
	    }
	}

	for(linkId = 0; linkId < MAX_DEI_LINK; linkId++)
	{
	    if(gVcapModuleContext.deiId[linkId]!=SYSTEM_LINK_ID_INVALID)
	    {
	        System_linkStart(gVcapModuleContext.deiId[linkId]);
	    }
	}

	for(linkId=0; linkId<MAX_SCLR_LINK; linkId++)
	{
	    if(gVcapModuleContext.sclrId[linkId]!=SYSTEM_LINK_ID_INVALID)
	    {
	        System_linkStart(gVcapModuleContext.sclrId[linkId]);
	    }
	}

	if(gVcapModuleContext.capSwMsId!=SYSTEM_LINK_ID_INVALID)
	{
	    System_linkStart(gVcapModuleContext.capSwMsId);
	}

	if(gVcapModuleContext.nullSrcId!=SYSTEM_LINK_ID_INVALID)
	{
	    System_linkStart(gVcapModuleContext.nullSrcId);
	}

	/* Start taking CPU load just before starting of capture link */
	MultiCh_prfLoadCalcEnable(TRUE, FALSE, FALSE);

	if(gVcapModuleContext.captureId!=SYSTEM_LINK_ID_INVALID)
	{
	    System_linkStart(gVcapModuleContext.captureId);
	}

    return 0;
}

Int32 Vcap_stop()
{
    UInt32 linkId;

    /* stop needs to be in the reseverse order of create */
	if(gVcapModuleContext.captureId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkStop(gVcapModuleContext.captureId);
    }

    if(gVcapModuleContext.nullSrcId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkStop(gVcapModuleContext.nullSrcId);
    }

    if(gVcapModuleContext.capSwMsId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkStop(gVcapModuleContext.capSwMsId);
    }

    for(linkId=0; linkId<MAX_SCLR_LINK; linkId++)
    {
        if(gVcapModuleContext.sclrId[linkId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkStop(gVcapModuleContext.sclrId[linkId]);
        }
    }

    for(linkId=0; linkId<MAX_NSF_LINK; linkId++)
    {
        if(gVcapModuleContext.nsfId[linkId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkStop(gVcapModuleContext.nsfId[linkId]);
        }
    }

    for(linkId = 0;linkId < MAX_DEI_LINK;linkId++)
    {
        if(gVcapModuleContext.deiId[linkId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkStop(gVcapModuleContext.deiId[linkId]);
        }
    }

	for(linkId = 0; linkId < MAX_IPC_FRAMES_LINK; linkId++)
    {
        if(gVcapModuleContext.ipcFramesOutVpssId[linkId]!=SYSTEM_LINK_ID_INVALID)
        {
           System_linkStop(gVcapModuleContext.ipcFramesOutVpssId[linkId]);
        }
        if(gVcapModuleContext.ipcFramesInDspId[linkId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkStop(gVcapModuleContext.ipcFramesInDspId[linkId]);
        }
    }

    if(gVcapModuleContext.ipcFramesOutVpssToHostId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkStop(gVcapModuleContext.ipcFramesOutVpssToHostId);
    }

    if(gVcapModuleContext.ipcFramesInHostId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkStop(gVcapModuleContext.ipcFramesInHostId);
    }

    if(gVcapModuleContext.ipcBitsInHLOSId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkStop(gVcapModuleContext.ipcBitsInHLOSId);
    }

    return 0;
}


/*
    Select the channel in capture link for whom extra buffers are needed
*/
Int32 Vcap_setExtraFramesChId(UInt32 chId)
{
    CaptureLink_ExtraFramesChId  prm;
    Int32 status;

    /* currently in all McFW use-case only QUE0 of capture is used */
    prm.queId = 0;
    prm.chId  = chId;

    status = System_linkControl(
            gVcapModuleContext.captureId,
            CAPTURE_LINK_CMD_SET_EXTRA_FRAMES_CH_ID,
            &prm,
            sizeof(prm),
            TRUE
            );

    return status;
}

/**
 * \brief:
 *      Get capture channels enabled
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       Number of capture channels
*/
Int32 Vcap_getNumChannels(Void)
{
    return gVcapModuleContext.vcapConfig.numChn;
}

Int32 Vcap_setParamDevice(VCAP_DEV vcDevId, VCAP_DEV_PARAM_S *psvcDevParam, VCAP_PARAMS_E paramId)
{
    memcpy(&gVcapModuleContext.vcapConfig.deviceParams[vcDevId], psvcDevParam, sizeof(VCAP_DEV_PARAM_S));
    return 0;
}

Int32 Vcap_getParamDevice(VCAP_DEV vcDevId, VCAP_DEV_PARAM_S *psvcDevParam, VCAP_PARAMS_E paramId)
{
    memcpy(psvcDevParam,&gVcapModuleContext.vcapConfig.deviceParams[vcDevId],sizeof(VCAP_DEV_PARAM_S));
    return 0;
}

Int32 Vcap_setParamChn(VCAP_CHN vcChnId, VCAP_CHN_PARAM_S *psCapChnParam, VCAP_PARAMS_E paramId)
{
    memcpy(&gVcapModuleContext.vcapConfig.channelParams[vcChnId], psCapChnParam, sizeof(VCAP_CHN_PARAM_S));
    return 0;
}

Int32 Vcap_getParamChn(VCAP_CHN vcChnId, VCAP_CHN_PARAM_S *psCapChnParam, VCAP_PARAMS_E paramId)
{
    memcpy(psCapChnParam, &gVcapModuleContext.vcapConfig.channelParams[vcChnId], sizeof(VCAP_CHN_PARAM_S));
    return 0;
}

Int32 Vcap_setDynamicParamChn(VCAP_CHN vcChnId, VCAP_CHN_DYNAMIC_PARAM_S *psCapChnDynaParam, VCAP_PARAMS_E paramId)
{
    Int32 status = ERROR_NONE;
    DeiLink_chDynamicSetMirror params = {0};

    if(gVcapModuleContext.captureId==SYSTEM_LINK_ID_INVALID)
        return ERROR_FAIL;

    switch(paramId)
    {
        case VCAP_RESOLUTION:
            break;
        case VCAP_MIRROR:
        	params.chId = vcChnId;
        	params.mirror = psCapChnDynaParam->mirror;
        	status = System_linkControl(gVcapModuleContext.deiId[0],
							DEI_LINK_CMD_SET_MIRROR,
							&(params), sizeof(params),
							TRUE);
            break;
        case VCAP_ALL:
            break;
        default:
        	break;
    }

    return status;
}

/**
 * \brief:
 *      Get capture Channel dynamic parameters
 * \input:
 *      vcChnId             -- capture Channel id
 *      psCapChnDynaParam   -- Device dynamic parameter structure
 * \output:
 *      NA
 * \return
*       ERROR_NONE    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vcap_getDynamicParamChn(VCAP_CHN vcChnId, VCAP_CHN_DYNAMIC_PARAM_S *psCapChnDynaParam, VCAP_PARAMS_E paramId)
{
    Int32 status = ERROR_FAIL;

    if(gVcapModuleContext.captureId==SYSTEM_LINK_ID_INVALID)
        return ERROR_FAIL;

    switch(paramId)
    {
        case VCAP_RESOLUTION:
            return ERROR_NONE;
        default:
            status = ERROR_FAIL;
    }

    return status;
}


/**
 * \brief:
 *      Skip any specific FID type. This is an additional control in capture side itself; is really useful for secondary stream <CIF>.
 *      Stream 0 is D1 & Stream 1 is CIF.
 * \input:
 *      vcChnId             -- capture Channel id
 *      fidType             -- TOP/BOTTOM field
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vcap_skipFidType(VCAP_CHN vcChnId, Int32 fidType)
{
    Int32 status = ERROR_NONE;
    SclrLink_chDynamicSkipFidType params;

    params.chId = vcChnId;
    params.fidType = fidType;
    status = System_linkControl(
                                gVcapModuleContext.sclrId[0],
                                SCLR_LINK_CMD_SKIP_FID_TYPE,
                                &(params),
                                sizeof(params),
                                TRUE
                               );

    return status;
}

/**
 * \brief:
 *      Set capture frame rate. This is an additional control in capture side itself; is really useful for secondary stream <CIF>.
 *      Stream 0 is D1 & Stream 1 is CIF.
 * \input:
 *      vcChnId             -- capture Channel id
 *      vStrmId             -- Stream Id
 *      frameRate          -- Frame Rate
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vcap_setFrameRate(VCAP_CHN vcChnId, VCAP_STRM vStrmId, Int32 inputFrameRate, Int32 outputFrameRate)
{
    Int32 status = ERROR_NONE;

	//# todo

	return status;
}

/**
 * \brief:
 *      Get capture frame rate. Not available now
 * \input:
 *      vcChnId             -- capture Channel id
 *      vStrmId             -- Stream Id
 *      frameRate          -- Frame Rate
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vcap_getFrameRate(VCAP_CHN vcChnId, VCAP_STRM vStrmId)
{
    return 0;
}

Int32 Vcap_enableDisableChn(VCAP_CHN vcChnId, VCAP_STRM vcStrmId, Bool enableChn)
{
    Int32 status = ERROR_FAIL;

    return status;
}

Int32 Vcap_enableChn(VCAP_CHN vcChnId, VCAP_STRM vcStrmId)
{
    return Vcap_enableDisableChn(vcChnId, vcStrmId, TRUE);
}

Int32 Vcap_disableChn(VCAP_CHN vcChnId, VCAP_STRM vcStrmId)
{
    return Vcap_enableDisableChn(vcChnId, vcStrmId, FALSE);
}

Int32 Vcap_delete()
{
    UInt32 linkId;
    /* delete can be done in any order */

    if(gVcapModuleContext.captureId!=SYSTEM_LINK_ID_INVALID)
        System_linkDelete(gVcapModuleContext.captureId);

    if(gVcapModuleContext.capSwMsId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkDelete(gVcapModuleContext.capSwMsId);
    }

    for(linkId=0; linkId<MAX_IPC_FRAMES_LINK; linkId++)
    {
        if(gVcapModuleContext.ipcFramesOutVpssId[linkId]!=SYSTEM_LINK_ID_INVALID)
        {
           System_linkDelete(gVcapModuleContext.ipcFramesOutVpssId[linkId]);
        }

        if(gVcapModuleContext.ipcFramesInDspId[linkId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkDelete(gVcapModuleContext.ipcFramesInDspId[linkId]);
        }
    }

    for(linkId=0; linkId<MAX_ALG_LINK; linkId++)
    {
        if(gVcapModuleContext.dspAlgId[linkId]!=SYSTEM_LINK_ID_INVALID)
        {
            System_linkDelete(gVcapModuleContext.dspAlgId[linkId]);
        }
    }

    for(linkId=0; linkId<MAX_DEI_LINK; linkId++)
    {
        if(gVcapModuleContext.deiId[linkId]!=SYSTEM_LINK_ID_INVALID)
            System_linkDelete(gVcapModuleContext.deiId[linkId]);
    }

    for(linkId=0; linkId<MAX_NSF_LINK; linkId++)
    {
        if(gVcapModuleContext.nsfId[linkId]!=SYSTEM_LINK_ID_INVALID)
            System_linkDelete(gVcapModuleContext.nsfId[linkId]);
    }

    for(linkId=0; linkId<MAX_SCLR_LINK; linkId++)
    {
        if(gVcapModuleContext.sclrId[linkId]!=SYSTEM_LINK_ID_INVALID)
            System_linkDelete(gVcapModuleContext.sclrId[linkId]);
    }

    if(gVcapModuleContext.ipcFramesOutVpssToHostId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkDelete(gVcapModuleContext.ipcFramesOutVpssToHostId);
    }

    if(gVcapModuleContext.ipcFramesInHostId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkDelete(gVcapModuleContext.ipcFramesInHostId);
    }

    if(gVcapModuleContext.ipcBitsInHLOSId!=SYSTEM_LINK_ID_INVALID)
    {
        System_linkDelete(gVcapModuleContext.ipcBitsInHLOSId);
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
Int32 Vcap_registerCallback(VCAP_CALLBACK_S * callback, Ptr arg)
{
    gVcapModuleContext.callbackFxn = *callback;
    gVcapModuleContext.callbackArg = arg;

    return 0;
}

/**
 * \brief:
 *              register bits call back which will post the message
 * \input:
 *              callback                -- callback function
 * \output:
 *              NA
 * \return
*               TI_MEDIA_SUCCESS        --      while success
*               ERROR_CODE                      --      refer for err defination
*/
Int32 Vcap_registerBitsCallback(VCAP_CALLBACK_S * callback, Ptr arg)
{
    gVcapModuleContext.bitscallbackFxn = *callback;
    gVcapModuleContext.bitscallbackArg = arg;

    return 0;
}

/**
    \brief Returns Bits Callback info registered by the application

    \param callback            [OUT] Pointer to User specified callbacks

    \param arg                 [OUT] Pointer to Callback context

    \return ERROR_NONE on success
*/
Int32 Vcap_getBitsCallbackInfo(VCAP_CALLBACK_S ** callback, Ptr *arg)
{
    *callback = &gVcapModuleContext.bitscallbackFxn;
    *arg      = gVcapModuleContext.bitscallbackArg;

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
Void Vcap_ipcFramesInCbFxn(Ptr cbCtx)
{
    OSA_assert(cbCtx == &gVcapModuleContext);

    VCAP_TRACE_FXN_ENTRY("Vcap_ipcFramesInCbFxn");

    if (gVcapModuleContext.callbackFxn.newDataAvailableCb)
    {
        gVcapModuleContext.callbackFxn.newDataAvailableCb(gVcapModuleContext.callbackArg);
    }
}

static Void Vcap_copyVidFrameInfoLink2McFw(VIDEO_FRAMEBUF_S *dstBuf,
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

    VCAP_TRACE_FXN_EXIT("VidFrameInfo:"
                         "virt[0][0]:%p,"
                         "phy[0][0]:%p,"
                         "channelNum:%d,"
                         "fid:%d,"
                         "frameWidth:%d,"
                         "frameHeight:%d,"
                         "framePitch[0]:%d,"
                         "framePitch[1]:%d,"
                         "timeStamp:%d,",
                         dstBuf->addr[0][0],
                         dstBuf->phyAddr[0][0],
                         dstBuf->channelNum,
                         dstBuf->fid,
                         dstBuf->frameWidth,
                         dstBuf->frameHeight,
                         dstBuf->framePitch[0],
                         dstBuf->framePitch[1],
                         dstBuf->timeStamp);
}

static Void Vcap_copyVidFrameInfoMcFw2Link(VIDFrame_Buf *dstBuf,
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

    VCAP_TRACE_FXN_EXIT("VidFrameInfo:"
                         "virt[0][0]:%p,"
                         "phy[0][0]:%p,"
                         "channelNum:%d,"
                         "fid:%d,"
                         "frameWidth:%d,"
                         "frameHeight:%d,"
                         "framePitch[0]:%d,"
                         "framePitch[1]:%d,"
                         "timeStamp:%d,",
                         dstBuf->addr[0][0],
                         dstBuf->phyAddr[0][0],
                         dstBuf->channelNum,
                         dstBuf->fid,
                         dstBuf->frameWidth,
                         dstBuf->frameHeight,
                         dstBuf->framePitch[0],
                         dstBuf->framePitch[1],
                         dstBuf->timeStamp);
}

static Void Vcap_copyBitBufInfoLink2McFw(VCODEC_BITSBUF_S *dstBuf,
                                         Bitstream_Buf    *srcBuf)
{
    dstBuf->reserved       = (UInt32)srcBuf;
    dstBuf->bufVirtAddr    = srcBuf->addr;
    dstBuf->bufSize        = srcBuf->bufSize;
    dstBuf->chnId          = srcBuf->channelNum;
    dstBuf->codecType      = srcBuf->codingType;
    dstBuf->mvDataFilledSize = srcBuf->mvDataFilledSize;
    dstBuf->mvDataOffset = srcBuf->mvDataOffset;
    dstBuf->filledBufSize  = srcBuf->fillLength;
    dstBuf->timestamp      = srcBuf->timeStamp;
    dstBuf->encodeTimestamp      = srcBuf->encodeTimeStamp;
    dstBuf->upperTimeStamp = srcBuf->upperTimeStamp;
    dstBuf->lowerTimeStamp = srcBuf->lowerTimeStamp;
    dstBuf->bottomFieldBitBufSize = srcBuf->bottomFieldBitBufSize;

    if (srcBuf->isKeyFrame)
        dstBuf->frameType      = VCODEC_FRAME_TYPE_I_FRAME;
    else
        dstBuf->frameType      = VCODEC_FRAME_TYPE_P_FRAME;
    dstBuf->bufPhysAddr    = (Void *)srcBuf->phyAddr;
    dstBuf->frameWidth     = srcBuf->frameWidth;
    dstBuf->frameHeight    = srcBuf->frameHeight;
    dstBuf->doNotDisplay   = FALSE;
    /*TODO the following members are to be added to bitStream bf structure */
    dstBuf->fieldId        = 0;
    dstBuf->strmId         = 0;
    VCAP_TRACE_FXN_EXIT("BitBufInfo:"
                         "virt:%p,"
                         "bufSize:%d,"
                         "chnId:%d,"
                         "codecType:%d,"
                         "filledBufSize:%d,"
                         "mvDataFilledSize:%d,"
                         "timeStamp:%d,"
                         "isKeyFrame:%d,"
                         "phy:%p,"
                         "width:%d"
                         "height:%d",
                         dstBuf->bufVirtAddr,
                         dstBuf->bufSize,
                         dstBuf->chnId,
                         dstBuf->codecType,
                         dstBuf->filledBufSize,
                         dstBuf->mvDataFilledSize,
                         dstBuf->timestamp,
                         srcBuf->isKeyFrame,
                         dstBuf->bufPhysAddr,
                         dstBuf->frameWidth,
                         dstBuf->frameHeight);
}

/**
    \brief Get encoded buffers from McFW

    \param pBitsBufList [OUT]   List of Bistream Buffers returned by the function
    \param timeout      [IN]    VSYS_WAIT_FOREVER or VSYS_NO_WAIT or timeout in units of msec

    \return SUCCESS or FAIL
 */
Int32 Vcap_getBitstreamBuffer(VCODEC_BITSBUF_LIST_S *pBitsBufList, UInt32 timeout)
{
    Bitstream_BufList ipcBufList;
    Bitstream_Buf *pInBuf;
    VCODEC_BITSBUF_S *pOutBuf;
    UInt32 i;

    VCAP_TRACE_FXN_ENTRY();
    pBitsBufList->numBufs = 0;
    ipcBufList.numBufs = 0;

    IpcBitsInLink_getFullVideoBitStreamBufs(gVcapModuleContext.ipcBitsInHLOSId,
                                            &ipcBufList);

    pBitsBufList->numBufs = ipcBufList.numBufs;
    for (i = 0; i < ipcBufList.numBufs; i++)
    {
        pOutBuf = &pBitsBufList->bitsBuf[i];
        pInBuf = ipcBufList.bufs[i];

        Vcap_copyBitBufInfoLink2McFw(pOutBuf,pInBuf);
    }

    VCAP_TRACE_FXN_EXIT("NumBufs Received:%d",pBitsBufList->numBufs);
    return 0;
}
/**
    \brief Release encoded buffers to McFW

    Buffers returned by Vcap_getBitstreamBuffer() are returned to the framework
    for resue after user is done using the encoded bitstreams

    \param pBitsBufList [IN]   List of Bistream Buffers

    \return SUCCESS or FAIL
 */
Int32 Vcap_releaseBitstreamBuffer(VCODEC_BITSBUF_LIST_S *pBitsBufList)
{
    VCODEC_BITSBUF_S *pOutBuf;
    Bitstream_BufList ipcBufList;
    UInt32 i;
    Int status = 0;

    VCAP_TRACE_FXN_ENTRY("Num bufs released:%d",pBitsBufList->numBufs);
    ipcBufList.numBufs = pBitsBufList->numBufs;
    for (i = 0; i < ipcBufList.numBufs; i++)
    {
        pOutBuf = &pBitsBufList->bitsBuf[i];
        ipcBufList.bufs[i] = (Bitstream_Buf*)pBitsBufList->bitsBuf[i].reserved;
    }
    if (ipcBufList.numBufs)
    {
        status =
        IpcBitsInLink_putEmptyVideoBitStreamBufs(gVcapModuleContext.ipcBitsInHLOSId,
                                                 &ipcBufList);
    }
    VCAP_TRACE_FXN_ENTRY("Buf release status:%d",status);
    return 0;
}


/**
    \brief Request filled video buffers from framework

    User calls this API to get full video frames from the framework.
    After getting the video frames, user will
    - consume the video frames
    - and then call Vcap_putEmptyVideoFrames() to free the video frames back to the framework

    \param pFrameBufList    [OUT]  List of video frames returned by the framework
    \param timeout          [IN]   TIMEOUT_WAIT_FOREVER or TIMEOUT_NO_WAIT or timeout in msecs

    \return ERROR_NONE on success
*/
Int32 Vcap_getFullVideoFrames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList, UInt32 timeout)
{
    VIDFrame_BufList  vidBufList;
    VIDFrame_Buf     *pInBuf;
    VIDEO_FRAMEBUF_S *pOutBuf;
    UInt32 i;

    VCAP_TRACE_FXN_ENTRY();
    pFrameBufList->numFrames = 0;
    vidBufList.numFrames = 0;
    IpcFramesInLink_getFullVideoFrames(gVcapModuleContext.ipcFramesInHostId,
                                       &vidBufList);

    pFrameBufList->numFrames = vidBufList.numFrames;
    for (i = 0; i < vidBufList.numFrames; i++)
    {
        pOutBuf = &pFrameBufList->frames[i];
        pInBuf = &vidBufList.frames[i];

        Vcap_copyVidFrameInfoLink2McFw(pOutBuf,pInBuf);
    }

    VCAP_TRACE_FXN_EXIT("NumFrames Received:%d",pFrameBufList->numFrames);
    return 0;
}
/**
    \brief Give consumed video frames back to the application to be freed

    Buffers that are were previously got from Vcap_getFullVideoFrames can be
    freed back to the framework by invoking this API.

    \param pFrameBufList [IN]   List of video frames

    \return ERROR_NONE on success
*/
Int32 Vcap_putEmptyVideoFrames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList)
{
    VIDEO_FRAMEBUF_S *pSrcBuf;
    VIDFrame_Buf     *pDstBuf;
    VIDFrame_BufList  vidBufList;
    UInt32 i;
    Int status = 0;

    VCAP_TRACE_FXN_ENTRY("Num bufs released:%d",pFrameBufList->numFrames);
    vidBufList.numFrames = pFrameBufList->numFrames;
    for (i = 0; i < vidBufList.numFrames; i++)
    {
        pSrcBuf = &pFrameBufList->frames[i];
        pDstBuf = &vidBufList.frames[i];
        Vcap_copyVidFrameInfoMcFw2Link(pDstBuf,pSrcBuf);
    }
    if (vidBufList.numFrames)
    {
        status =
        IpcFramesInLink_putEmptyVideoFrames(gVcapModuleContext.ipcFramesInHostId,
                                            &vidBufList);
    }
    VCAP_TRACE_FXN_ENTRY("VIDFrame release status:%d",status);
    return 0;
}
/**
    \brief Detects video per channel at the video input.

    \param pFrameBufList [IN]   List of video frames

    \return ERROR_NONE on success
*/

Int32 Vcap_detectVideo()
{

    Int status = 0;

    if((   gVcapModuleContext.vcapConfig.enableConfigExtVideoDecoder)
       && (gVcapModuleContext.captureId!=SYSTEM_LINK_ID_INVALID))
    {
        status = System_linkControl(gVcapModuleContext.captureId,
                                CAPTURE_LINK_CMD_DETECT_VIDEO,
                                NULL,
                                0,
                                TRUE);
    }
    return status;

}

Int32 Vcap_get_video_status(int max_ch, Int32 *detect, Int32 *is_pal)
{
    Int32 status = 0;
    VCAP_VIDEO_SOURCE_STATUS_S 		videoSourceStatus;
    Int32 i;

    if(gVcapModuleContext.captureId!=SYSTEM_LINK_ID_INVALID)
    {
        status = System_linkControl(gVcapModuleContext.captureId,
                                CAPTURE_LINK_CMD_GET_VIDEO_STATUS,
                                &videoSourceStatus,
                                sizeof(videoSourceStatus),
                                TRUE);
		if(status != OSA_SOK)
			return -1;

		if(max_ch > videoSourceStatus.numChannels)
			max_ch = videoSourceStatus.numChannels;

		for(i=0; i<max_ch; i++) {
			*detect++ = videoSourceStatus.chStatus[i].isVideoDetect;
			*is_pal++ = videoSourceStatus.chStatus[i].isPalMode;
		}
    }

	return max_ch;
}

Int32 Vcap_set_video_reg(UInt32 dev_num, UInt8 regaddr, UInt8 regval)
{
	Int32 status = 0;
	Vcap_viddec_reg_rw_t regPrm;

	regPrm.dev_num = dev_num;
	regPrm.addr = regaddr;
	regPrm.val = regval;

	if(gVcapModuleContext.captureId != SYSTEM_LINK_ID_INVALID)
    {
		status = System_linkControl(gVcapModuleContext.captureId,
						CAPTURE_LINK_CMD_VIDEO_REG_WRITE,
						&regPrm, sizeof(regPrm),
						TRUE);

		if(status != OSA_SOK)
			return -1;
    }

	return 0;
}

Int32 Vcap_get_video_reg(UInt32 dev_num, UInt8 regaddr, UInt8 *regval)
{
	Int32 status = 0;
	Vcap_viddec_reg_rw_t regPrm;

	regPrm.dev_num = dev_num;
	regPrm.addr = regaddr;
	regPrm.val = 0;

	if(gVcapModuleContext.captureId != SYSTEM_LINK_ID_INVALID)
    {
		status = System_linkControl(gVcapModuleContext.captureId,
						CAPTURE_LINK_CMD_VIDEO_REG_READ,
						&regPrm, sizeof(regPrm),
						TRUE);
    	if(status != OSA_SOK)
			return -1;

		*regval = regPrm.val;
    }

	return 0;
}

Int32 Vcap_setColor(Int32 chId, Int32 brightness, Int32 contrast, Int32 saturation)
{
	if(gVcapModuleContext.vcapConfig.enableConfigExtVideoDecoder)
	{
		Int32 status = 0;
		CaptureLink_ColorParams colorPrm;

		if(gVcapModuleContext.captureId != SYSTEM_LINK_ID_INVALID)
		{
			colorPrm.chId = chId;
			colorPrm.brightness = brightness;
			colorPrm.contrast = contrast;
			colorPrm.saturation = saturation;

			status = System_linkControl(gVcapModuleContext.captureId,
							CAPTURE_LINK_CMD_CHANGE_COLOR,
							&colorPrm, sizeof(colorPrm),
							TRUE);
			if(status != OSA_SOK)
				return -1;
		}
    }

	return 0;
}

Int32 Vcap_link_reset(void)
{
	Int32 status = 0;

	if(gVcapModuleContext.captureId != SYSTEM_LINK_ID_INVALID)
	{
		status = System_linkControl(gVcapModuleContext.captureId,
							CAPTURE_LINK_CMD_FORCE_RESET,
							NULL, 0,
							TRUE);
		if(status != OSA_SOK)
			return -1;
	}

	return 0;
}
