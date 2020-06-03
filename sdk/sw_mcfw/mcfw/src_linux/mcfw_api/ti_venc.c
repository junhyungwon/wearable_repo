/*==============================================================================
 * @file:       ti_venc.c
 *
 * @brief:      Video capture mcfw function definition.
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

#include <osa.h>
#include <osa_debug.h>
#include "ti_vsys_priv.h"
#include "ti_venc_priv.h"
#include "ti_vsys.h"
#include "ti_venc.h"
#include "ti_vdec_priv.h"

#include <mcfw/interfaces/link_api/encLink.h>
#include <mcfw/interfaces/link_api/ipcLink.h>

#define VENC_TRACE_ENABLE_FXN_ENTRY_EXIT           (0)
#define VENC_TRACE_INFO_PRINT_INTERVAL             (8192)


#if VENC_TRACE_ENABLE_FXN_ENTRY_EXIT
#define VENC_TRACE_FXN(str,...)                    do {                           \
                                                     static Int printInterval = 0;\
                                                     if ((printInterval % VENC_TRACE_INFO_PRINT_INTERVAL) == 0) \
                                                     {                                                          \
                                                         OSA_printf("TI_VENC:%s function:%s",str,__func__);     \
                                                         OSA_printf(__VA_ARGS__);                               \
                                                     }                                                          \
                                                     printInterval++;                                           \
                                                   } while (0)
#define VENC_TRACE_FXN_ENTRY(...)                  VENC_TRACE_FXN("Entered",__VA_ARGS__)
#define VENC_TRACE_FXN_EXIT(...)                   VENC_TRACE_FXN("Leaving",__VA_ARGS__)
#else
#define VENC_TRACE_FXN_ENTRY(...)
#define VENC_TRACE_FXN_EXIT(...)
#endif

/* =============================================================================
 * Internal APIs declaration
 * =============================================================================
 */
/* =============================================================================
 * Globals
 * =============================================================================
 */

VENC_MODULE_CONTEXT_S gVencModuleContext;

/* =============================================================================
 * Externs
 * =============================================================================
 */

/* =============================================================================
 * Venc module APIs
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
Void Venc_params_init(VENC_PARAMS_S *pContext)
{
    UInt16 chId;
    VENC_CHN_DYNAMIC_PARAM_S *pDynPrm;
    VENC_CHN_PARAMS_S *pChPrm;

    memset(pContext, 0, sizeof(VENC_PARAMS_S));
    pContext->numPrimaryChn = 0;
    pContext->numSecondaryChn = 0;

    for (chId=0; chId < VENC_CHN_MAX; chId++)
    {
    	//# VENC_CHN_HIGH_PROFILE, VENC_CHN_MAIN_PROFILE, VENC_CHN_BASELINE_PROFILE
        pContext->h264Profile[chId] = VENC_CHN_HIGH_PROFILE;

        pChPrm  = &pContext->encChannelParams[chId];
        pDynPrm = &pChPrm->dynamicParam;

        pChPrm->enableAnalyticinfo = 0;
        pChPrm->encodingPreset     = VENC_XDM_USER_DEFINED;
        pChPrm->maxBitRate         = -1;
        pChPrm->rcType             = IVIDEO_USER_DEFINED;

        pDynPrm->frameRate          = 30;
        pDynPrm->targetBitRate      = 2 * 1000 * 1000;
        pDynPrm->intraFrameInterval = 30;
        pDynPrm->inputFrameRate     = pDynPrm->frameRate;
        pDynPrm->rcAlg              = VENC_RATE_CTRL_CBR;	//# use only default
        pDynPrm->qpMin              = 10;
        pDynPrm->qpMax              = 40;
        pDynPrm->qpInit             = -1;	//# SHOULD BE SET to -1 ALWAYS
        pDynPrm->vbrDuration        =  8;	//# 1~3600
        pDynPrm->vbrSensitivity     =  0;	//# 0~8
    }

    return;
}

Void Venc_params_set(VENC_PARAMS_S *pContext, VENC_CHN chId, VENC_CHN_DYNAMIC_PARAM_S *pChParam, VENC_PARAM_E veParamId)
{
	VENC_CHN_DYNAMIC_PARAM_S *pDynPrm;
    VENC_CHN_PARAMS_S *pChPrm;

	pChPrm  = &pContext->encChannelParams[chId];
	pDynPrm = &pChPrm->dynamicParam;

	switch (veParamId)
    {
    	case VENC_BITRATE:
    		if(pChParam->targetBitRate == 0)
    			break;
    		pDynPrm->targetBitRate = pChParam->targetBitRate;
    		break;
    	case VENC_FRAMERATE:
    		if(pChParam->frameRate == 0)
    			break;
    		pDynPrm->frameRate = pChParam->frameRate;
    		break;
    	case VENC_INFRAMERATE:
    		if(pChParam->inputFrameRate == 0)
    			break;
    		pDynPrm->inputFrameRate = pChParam->inputFrameRate;
    		break;
    	case VENC_ALL:
    		if(pChParam->targetBitRate == 0 || pChParam->frameRate == 0 || pChParam->inputFrameRate == 0)
    			break;
    		pDynPrm->targetBitRate = pChParam->targetBitRate;
    		pDynPrm->inputFrameRate = pChParam->inputFrameRate;
    		pDynPrm->frameRate = pChParam->frameRate;
    		pDynPrm->intraFrameInterval = pChParam->frameRate;
    		break;
    	default:
    		break;
	}
	return;
}

/**
 * \brief:
 *      Initialize Venc instance
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_init(VENC_PARAMS_S * pContext)
{
    gVencModuleContext.encId = SYSTEM_LINK_ID_INVALID;
    gVencModuleContext.ipcBitsOutRTOSId = SYSTEM_LINK_ID_INVALID;
    gVencModuleContext.ipcBitsInHLOSId = SYSTEM_LINK_ID_INVALID;
    gVencModuleContext.ipcM3InId = SYSTEM_LINK_ID_INVALID;
    gVencModuleContext.ipcM3OutId = SYSTEM_LINK_ID_INVALID;

    gVencModuleContext.callbackFxn.newDataAvailableCb = NULL;
    gVencModuleContext.callbackArg = NULL;

    if(pContext==NULL)
    {
        Venc_params_init(&gVencModuleContext.vencConfig);
    }
    else
    {
        memcpy(&gVencModuleContext.vencConfig, pContext, sizeof(VENC_PARAMS_S));
    }
    return 0;
}

/**
 * \brief:
 *      Finalize Venc instance
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Void Venc_exit()
{
}

/**
 * \brief:
 *      Start Venc instance for capturing and proccessing
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_start()
{
    if(gVencModuleContext.ipcBitsInHLOSId!=SYSTEM_LINK_ID_INVALID)
        System_linkStart(gVencModuleContext.ipcBitsInHLOSId);

    if(gVencModuleContext.ipcBitsOutRTOSId!=SYSTEM_LINK_ID_INVALID)
        System_linkStart(gVencModuleContext.ipcBitsOutRTOSId);

    if(gVencModuleContext.encId!=SYSTEM_LINK_ID_INVALID)
        System_linkStart(gVencModuleContext.encId);

    if(gVencModuleContext.ipcM3InId!=SYSTEM_LINK_ID_INVALID)
        System_linkStart(gVencModuleContext.ipcM3InId);

    if(gVencModuleContext.ipcM3OutId!=SYSTEM_LINK_ID_INVALID)
        System_linkStart(gVencModuleContext.ipcM3OutId);

    return 0;
}

/**
 * \brief:
 *      Stop Venc instance
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_stop()
{
    if(gVencModuleContext.ipcM3OutId!=SYSTEM_LINK_ID_INVALID)
        System_linkStop(gVencModuleContext.ipcM3OutId);

    if(gVencModuleContext.ipcM3InId!=SYSTEM_LINK_ID_INVALID)
        System_linkStop(gVencModuleContext.ipcM3InId);

    if(gVencModuleContext.encId!=SYSTEM_LINK_ID_INVALID)
        System_linkStop(gVencModuleContext.encId);

    if(gVencModuleContext.ipcBitsOutRTOSId!=SYSTEM_LINK_ID_INVALID)
        System_linkStop(gVencModuleContext.ipcBitsOutRTOSId);

    if(gVencModuleContext.ipcBitsInHLOSId!=SYSTEM_LINK_ID_INVALID)
        System_linkStop(gVencModuleContext.ipcBitsInHLOSId);

    return 0;
}

/* =============================================================================
 * Venc device related APIs
 * =============================================================================
 */

/* =============================================================================
 * Venc channel related APIs
 * =============================================================================
 */

/**
 * \brief:
 *      Enable the specific encoder channel
 * \input:
 *      vencChnId       -- encoder channel id
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_enableChn(VENC_CHN vencChnId)
{
    EncLink_ChannelInfo params = {0};

    OSA_printf("\r\nEnable Channel: %d", vencChnId);

    params.chId = vencChnId;

    System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_ENABLE_CHANNEL,
                       &params, sizeof(params), TRUE);

    return 0;

}
/**
 * \brief:
 *      Disable the specific encoder channel
 * \input:
 *      vencChnId       -- encoder channel id
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_disableChn(VENC_CHN vencChnId)
{
    EncLink_ChannelInfo params = {0};

    OSA_printf("\r\nDisable Channel: %d", vencChnId);

    params.chId = vencChnId;

    System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_DISABLE_CHANNEL,
                       &params, sizeof(params), TRUE);

    return 0;
}


/* =============================================================================
 * Helper APIs
 * =============================================================================
 */

/**
 * \brief:
 *      Get primary channels enabled
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       Number of primary channels
*/
Int32 Venc_getPrimaryChannels()
{
    return gVencModuleContext.vencConfig.numPrimaryChn;
}

/**
 * \brief:
 *      Get Secondary channels enabled
 * \input:
 *      NA
 * \output:
 *      NA
 * \return
*       Number of secondary channels
*/
Int32 Venc_getSecondaryChannels()
{
    return gVencModuleContext.vencConfig.numSecondaryChn;
}

/* =============================================================================
 * Internal APIs
 * =============================================================================
 */

/* =============================================================================
 * Stream Related APIs
 * =============================================================================
 */
 /**
 * \brief:
 *      Get the stream dynamic parameter
 * \input:
 *      vencChnId       -- encoder channel id
 *      vencStrmID      -- the stream id
 *      ptEncDynamicParam -- encoder dynamic parameter to get
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_getDynamicParam(VENC_CHN vencChnId, VENC_STRM vencStrmID,
                            VENC_CHN_DYNAMIC_PARAM_S *ptEncDynamicParam, VENC_PARAM_E veParamId)
{
    EncLink_GetDynParams params = { 0 };

    OSA_printf("\r\nGet Dynamic params of Channel: %d", vencChnId);
    params.chId = vencChnId;
    System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_GET_CODEC_PARAMS,
                       &params, sizeof(params), TRUE);
    switch (veParamId)
    {
        case VENC_ALL:
            /* Normalize frame rate to application expected scale */
            ptEncDynamicParam->frameRate = params.targetFps/VENC_FRAMERATE_LINK_MULTIPLICATION_FACTOR;
            ptEncDynamicParam->targetBitRate = params.targetBitRate;
            ptEncDynamicParam->intraFrameInterval = params.intraFrameInterval;
            gVencModuleContext.vencConfig.encChannelParams[vencChnId].dynamicParam.frameRate
                = params.targetFps/VENC_FRAMERATE_LINK_MULTIPLICATION_FACTOR;
            gVencModuleContext.vencConfig.encChannelParams[vencChnId].dynamicParam.intraFrameInterval
                = params.intraFrameInterval;
            gVencModuleContext.vencConfig.encChannelParams[vencChnId].dynamicParam.targetBitRate
                = params.targetBitRate;
            gVencModuleContext.vencConfig.encChannelParams[vencChnId].videoWidth = params.inputWidth;
            gVencModuleContext.vencConfig.encChannelParams[vencChnId].videoHeight = params.inputHeight;

            break;
        case VENC_BITRATE:
            ptEncDynamicParam->targetBitRate = params.targetBitRate;
            gVencModuleContext.vencConfig.encChannelParams[vencChnId].dynamicParam.targetBitRate
                = params.targetBitRate;
            break;
        case VENC_FRAMERATE:
            ptEncDynamicParam->frameRate = params.targetFps/VENC_FRAMERATE_LINK_MULTIPLICATION_FACTOR;
            gVencModuleContext.vencConfig.encChannelParams[vencChnId].dynamicParam.frameRate
                = params.targetFps/VENC_FRAMERATE_LINK_MULTIPLICATION_FACTOR;
            break;
        case VENC_IPRATIO:
            ptEncDynamicParam->intraFrameInterval = params.intraFrameInterval;
            gVencModuleContext.vencConfig.encChannelParams[vencChnId].dynamicParam.intraFrameInterval
                = params.intraFrameInterval;
            break;
        default:
            break;
    }

    return 0;
}

 /**
 * \brief:
 *      Set the stream dynamic parameters
 * \input:
 *      vencChnId       -- encoder channel id
 *      vencStrmID      -- the stream id
 *      ptEncDynamicParam -- encoder dynamic parameter to be set
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_setDynamicParam(VENC_CHN vencChnId, VENC_STRM vencStrmID,
                            VENC_CHN_DYNAMIC_PARAM_S *ptEncDynamicParam, VENC_PARAM_E veParamId)
{
	if(gVencModuleContext.encId == SYSTEM_LINK_ID_INVALID)
		return ERROR_FAIL;

    switch (veParamId)
    {
        case VENC_ALL:
        {
            OSA_printf("\r\n Not Supported");
            break;
        }
        case VENC_RATECONTROL:
        {
            EncLink_ChRateControlParams params;

            params.chId = vencChnId;
            OSA_printf(" Channel Selected: %d, rate control = %d\n", params.chId,
                   ptEncDynamicParam->rateControl);

            params.rateControl = ptEncDynamicParam->rateControl;
            System_linkControl(gVencModuleContext.encId,
                               ENC_LINK_CMD_SET_CODEC_RATECONTROL, &params,
                               sizeof(params), TRUE);
            break;
        }
        case VENC_BITRATE:
        {
            EncLink_ChBitRateParams params;
            params.chId = vencChnId;
            OSA_printf(" CH%d: Bitrate %d\n", params.chId, ptEncDynamicParam->targetBitRate);

            /* New bitrate value */
            params.targetBitRate = ptEncDynamicParam->targetBitRate;
            System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_SET_CODEC_BITRATE,
                               &params, sizeof(params), TRUE);

            break;
        }
        case VENC_FRAMERATE:
        {
            EncLink_ChFpsParams params;
            params.chId = vencChnId;

            /* New fps vaule in fps x 1000 formate */
            params.targetFps = (1000 * ptEncDynamicParam->frameRate);
            /* Corresponding bitrate value */
            params.targetBitRate = ptEncDynamicParam->targetBitRate;
            System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_SET_CODEC_FPS,
                               &params, sizeof(params), TRUE);

            break;
        }
        case VENC_IPRATIO:
        {
            EncLink_ChIntraFrIntParams params;
            params.chId = vencChnId;
            OSA_printf(" CH%d: IP ratio %d\n", params.chId, ptEncDynamicParam->intraFrameInterval);
            /* new GOP value */
            params.intraFrameInterval = ptEncDynamicParam->intraFrameInterval;
            System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_SET_CODEC_INTRAI,
                               &params, sizeof(params), TRUE);
            break;
        }
        case VENC_QPVAL_I:
        {
            EncLink_ChQPParams params;
            params.chId = vencChnId;
            /* new QP values */
            params.qpMin    = ptEncDynamicParam->qpMin;
            params.qpMax    = ptEncDynamicParam->qpMax;
            params.qpInit   = ptEncDynamicParam->qpInit;
            System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_SET_CODEC_QP_I,
                               &params, sizeof(params), TRUE);
            break;
        }
        case VENC_QPVAL_P:
        {
            EncLink_ChQPParams params;
            params.chId = vencChnId;
            /* new QP values */
            params.qpMin    = ptEncDynamicParam->qpMin;
            params.qpMax    = ptEncDynamicParam->qpMax;
            params.qpInit   = ptEncDynamicParam->qpInit;
            System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_SET_CODEC_QP_P,
                               &params, sizeof(params), TRUE);
            break;
        }
        case VENC_PACKETSIZE:
        {
            EncLink_ChPacketSizeParams params;

            params.chId = vencChnId;
            OSA_printf("CH%d: packet size = %d\n", params.chId,
                   ptEncDynamicParam->packetSize);
            /* new PacketSize values */
            params.packetSize = ptEncDynamicParam->packetSize;
            System_linkControl(gVencModuleContext.encId,
                               ENC_LINK_CMD_SET_CODEC_PACKETSIZE, &params,
                               sizeof(params), TRUE);
            break;
        }
        case VENC_ROI:
        {
            EncLink_ChROIParams params;

            params.chId = vencChnId;
            params.numOfRegion = ptEncDynamicParam->roiNumOfRegion;
            OSA_printf("CH%d: enable = %d\n", params.chId,
                   params.numOfRegion);
            int i = 0;

            for (i = 0; i < ptEncDynamicParam->roiNumOfRegion; i++)
            {
                params.startX[i] = ptEncDynamicParam->roiStartX[i];
                params.startY[i] = ptEncDynamicParam->roiStartY[i];
                params.width[i] = ptEncDynamicParam->roiWidth[i];
                params.height[i] = ptEncDynamicParam->roiHeight[i];
                params.type[i] = ptEncDynamicParam->roiType[i];
                OSA_printf(" CH%d: %d,%d,%d,%d\n",
                       i, params.startX[i], params.startY[i], params.width[i],
                       params.height[i]);
            }
            System_linkControl(gVencModuleContext.encId,
                               ENC_LINK_CMD_SET_CODEC_ROI, &params,
                               sizeof(params), TRUE);
            break;
        }
        case VENC_INPUTKEY:
        {
            EncLink_ChInputKeyParams params;

            params.chId = vencChnId;
            params.inputKey = ptEncDynamicParam->inputKey;
            OSA_printf("CH%d: enable = %d\n", params.chId,
                   params.inputKey);

            System_linkControl(gVencModuleContext.encId,
                               ENC_LINK_CMD_SET_CODEC_INPUTKEY, &params,
                               sizeof(params), TRUE);
            break;
        }
        case VENC_ENCPRESET:
        {
            EncLink_ChEncodePresetParams params;

            params.chId = vencChnId;
            params.encodePreset = ptEncDynamicParam->encPreset;
            OSA_printf(" CH%d: encPreset = %d\n", params.chId,
                   params.encodePreset);
            System_linkControl(gVencModuleContext.encId,
                               ENC_LINK_CMD_SET_CODEC_ENCPRESET, &params,
                               sizeof(params), TRUE);
            break;
        }
        case VENC_MDPARAM:
        {
            EncLink_ChEncodePresetParams params;

            params.chId = vencChnId;
            params.bMotionEnable = ptEncDynamicParam->bMotionEnable;
            params.bMotionCEnale = ptEncDynamicParam->bMotionCEnale;
            params.MotionLevel = ptEncDynamicParam->MotionLevel;
            params.MotionCValue = ptEncDynamicParam->MotionCValue;
            params.MotionBlock = ptEncDynamicParam->MotionBlock;

            System_linkControl(gVencModuleContext.encId,
                               ENC_LINK_CMD_SET_CODEC_MDPRM, &params,
                               sizeof(params), TRUE);
            break;
        }
        case VENC_VBRDURATION:
        {
            EncLink_ChCVBRDurationParams params;
            params.chId = vencChnId;

            params.vbrDuration = ptEncDynamicParam->vbrDuration;

            System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_SET_CODEC_VBRD,
                               &params, sizeof(params), TRUE);
            break;
        }
        case VENC_VBRSENSITIVITY:
        {
            EncLink_ChCVBRSensitivityParams params;
            params.chId = vencChnId;

            params.vbrSensitivity = ptEncDynamicParam->vbrSensitivity;

            System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_SET_CODEC_VBRS,
                               &params, sizeof(params), TRUE);
            break;
        }
        default:
            break;


    }

    return 0;
}

/**
 * \brief:
 *      Set the input frame-rate
 * \input:
 *      vencChnId     -- encoder channel id
 *      veFrameRate     -- encoder input frame-rate to be set
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_setInputFrameRate(VENC_CHN vencChnId, Int32 veFrameRate)
{
    EncLink_ChInputFpsParam params;
    params.chId = vencChnId;
	//OSA_printf(" Channel Selected: %d\n", params.chId);

    /*Update this too*/
    gVencModuleContext.vencConfig.encChannelParams[vencChnId].dynamicParam.inputFrameRate = veFrameRate;

    /* New bitrate value */
    params.inputFps = veFrameRate;
    System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_SET_CODEC_INPUT_FPS,
                       &params, sizeof(params), TRUE);
    return 0;
}


/**
 * \brief:
 *      Force IDR frame
 * \input:
 *      vencChnId       -- encoder channel id
 *      vencStrmID      -- the stream id
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_forceIDR(VENC_CHN vencChnId, VENC_STRM vencStrmID)
{
    EncLink_ChForceIFrParams params;

    params.chId = vencChnId;
    OSA_printf("\r\nForce IDR on Channel: %d", vencChnId);
    System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_SET_CODEC_FORCEI,
                       &params, sizeof(params), TRUE);
    return 0;
}

/**
 * \brief:
 *      Take a snapshot jpeg frame
 * \input:
 *      vencChnId       -- encoder channel id
 *      vencStrmID      -- the stream id
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_snapshotDump(VENC_CHN vencChnId, VENC_STRM vencStrmID)
{
    EncLink_ChannelInfo params;

    params.chId = vencChnId;
    OSA_printf("\r\nTake a snapshot of Channel: %d", vencChnId);
    System_linkControl(gVencModuleContext.encId, ENC_LINK_CMD_SET_CODEC_SNAPSHOT,
                       &params, sizeof(params), TRUE);
    return 0;
}


/**
 * \brief:
 *      register call back which will post the message
 * \input:
 *      callback        -- callback function
 * \output:
 *      NA
 * \return
*       TI_MEDIA_SUCCESS    --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Venc_registerCallback(VENC_CALLBACK_S * callback, Ptr arg)
{
    gVencModuleContext.callbackFxn = *callback;
    gVencModuleContext.callbackArg = arg;

    return 0;
}

/**
    \brief Returns Callback info registered by the application

    \param callback            [OUT] Pointer to User specified callbacks

    \param arg                 [OUT] Pointer to Callback context

    \return ERROR_NONE on success
*/
Int32 Venc_getCallbackInfo(VENC_CALLBACK_S ** callback, Ptr *arg)
{
    *callback = &gVencModuleContext.callbackFxn;
    *arg      = gVencModuleContext.callbackArg;

    return 0;
}

static Void Venc_copyBitBufInfoLink2McFw(VCODEC_BITSBUF_S *dstBuf,
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
    dstBuf->temporalId     = srcBuf->temporalId;
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
    VENC_TRACE_FXN_EXIT("BitBufInfo:"
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
Int32 Venc_getBitstreamBuffer(VCODEC_BITSBUF_LIST_S *pBitsBufList, UInt32 timeout)
{
    Bitstream_BufList ipcBufList;
    Bitstream_Buf *pInBuf;
    VCODEC_BITSBUF_S *pOutBuf;
    UInt32 i;

    VENC_TRACE_FXN_ENTRY();
    pBitsBufList->numBufs = 0;
    ipcBufList.numBufs = 0;
    IpcBitsInLink_getFullVideoBitStreamBufs(gVencModuleContext.ipcBitsInHLOSId,
                                            &ipcBufList);

    pBitsBufList->numBufs = ipcBufList.numBufs;
    for (i = 0; i < ipcBufList.numBufs; i++)
    {
        pOutBuf = &pBitsBufList->bitsBuf[i];
        pInBuf = ipcBufList.bufs[i];

        Venc_copyBitBufInfoLink2McFw(pOutBuf,pInBuf);
    }

    VENC_TRACE_FXN_EXIT("NumBufs Received:%d",pBitsBufList->numBufs);
    return 0;
}
/**
    \brief Release encoded buffers to McFW

    Buffers returned by Venc_getBitstreamBuffer() are returned to the framework
    for resue after user is done using the encoded bitstreams

    \param pBitsBufList [IN]   List of Bistream Buffers

    \return SUCCESS or FAIL
 */
Int32 Venc_releaseBitstreamBuffer(VCODEC_BITSBUF_LIST_S *pBitsBufList)
{
    VCODEC_BITSBUF_S *pOutBuf;
    Bitstream_BufList ipcBufList;
    UInt32 i;
    Int status = 0;

    VENC_TRACE_FXN_ENTRY("Num bufs released:%d",pBitsBufList->numBufs);
    ipcBufList.numBufs = pBitsBufList->numBufs;
    for (i = 0; i < ipcBufList.numBufs; i++)
    {
        pOutBuf = &pBitsBufList->bitsBuf[i];
        ipcBufList.bufs[i] = (Bitstream_Buf*)pBitsBufList->bitsBuf[i].reserved;
    }
    if (ipcBufList.numBufs)
    {
        status =
        IpcBitsInLink_putEmptyVideoBitStreamBufs(gVencModuleContext.ipcBitsInHLOSId,
                                                 &ipcBufList);
    }
    VENC_TRACE_FXN_ENTRY("Buf release status:%d",status);
    return 0;
}

void Venc_getContext(VENC_PARAMS_S * contextInfo)
{
    if (contextInfo != NULL)
    {
        memcpy(contextInfo,
               &gVencModuleContext.vencConfig,
               sizeof(VENC_PARAMS_S));
    }

}

void Venc_setRatectrl(int ch, int type, int bitrate)
{
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };

	//OSA_printf("--- %s: CH%d, type %s, bitrate %d\n", __func__, ch, type?"VENC_RATE_CTRL_CBR":"VENC_RATE_CTRL_VBR", bitrate);
	if(type == VENC_RATE_CTRL_CBR)
	{
		params.qpMin 	= 10;
		params.qpMax 	= 40;
		params.qpInit 	= -1;
	}
	else if(type == VENC_RATE_CTRL_VBR)
	{
		if(bitrate > 2*MB && bitrate <= 5*MB)
		{
			//# for VBR setting (1.8M~5.6M when bitrate was 5M)
			params.qpMin 	= 24;
		    params.qpMax 	= 45;
		    params.qpInit 	= -1;
		}
		else if(bitrate > 5*MB && bitrate <= 8*MB)
		{
			//# for VBR setting (4M~8.5M when bitrate was 8M)
			params.qpMin 	= 22;
		    params.qpMax 	= 45;
		    params.qpInit 	= -1;
		}
		else	//bitrate <= 2*MB
		{
			//# for VBR setting (0.7M~2M when bitrate was 2M)
			params.qpMin 	= 26;
		    params.qpMax 	= 45;
		    params.qpInit 	= -1;
		}
	}

	Venc_setDynamicParam(ch, 0, &params, VENC_QPVAL_P);
}
