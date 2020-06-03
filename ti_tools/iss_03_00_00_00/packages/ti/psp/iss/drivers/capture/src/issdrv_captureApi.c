/** ==================================================================
 *  @file   issdrv_captureApi.c
 *
 *  @path   /ti/psp/iss/drivers/capture/src/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/
#include <xdc/std.h>
#include <ti/psp/iss/drivers/capture/src/issdrv_capturePriv.h>
#include <ti/psp/iss/drivers/alg/vstab/src/issdrv_algVstabPriv.h>
#include <ti/psp/iss/hal/iss/isp/isif/inc/isif.h>
#include <ti/psp/iss/hal/iss/isp/rsz/inc/rsz.h>
#include <ti/psp/iss/hal/iss/isp/ipipe/inc/ipipe.h>
#include <ti/psp/iss/core/inc/iss_drv_common.h>
#include <ti/psp/iss/core/inc/iss_drv.h>
#include <ti/psp/iss/drivers/alg/vstab/alg_vstab.h>
#include <ti/psp/iss/core/isp_msp.h>
#include <ti/psp/iss/drivers/alg/2A/inc/issdrv_alg2APriv.h>
#include <ti/psp/iss/drivers/capture/src/issdrv_ispPriv.h>

#include <ti/psp/iss/drivers/iss_init.h>
/*========================================================================= */
/*========================================================================= */
//#define IMAGE_SENSOR_FULL_FRAME
#define USE_NOT_2A		0		//# phoong - for test close

#define VS_floor(value, align)   (( (value) / (align) ) * (align) )
#ifndef ISS_EXAMPLES
extern Ptr Utils_memAlloc(UInt32 size, UInt32 align);
extern Int32 Utils_memFree(Ptr addr, UInt32 size);
#endif
#define BOXCAR_ELEMENT_SIZE     	(8)
#define BOXCAR_MAX_LINE_8       	(4096)
#define VIDEO_ISR_EVENT_RSZ         (1 << 0)
#define VIDEO_ISR_EVENT_H3A         (1 << 1)
#define VIDEO_ISR_EVENT_BSC         (1 << 2)
#define VIDEO_ISR_EVENT_CCP         (1 << 3)
#define EXPECT_EVENT(x,y)           (x->nRemainIsrEvents |= (y))
#define CLEAR_EVENT(x,y)            (x->nRemainIsrEvents &= (~(y)))
#define NO_EVENTS_REMAIN(x)         (!(x->nRemainIsrEvents))
#define CLEAR_ALL_EVENTS(x)         (x->nRemainIsrEvents = 0)
#define MSP_MEMALLOC(SIZE)			malloc(SIZE, 32);
#define MSP_MEMFREE(PTR)

Void Iss_ispWriteReg(volatile UInt32 *pRegAddr,UInt32 value,UInt32 bitPosn,UInt32 numBits);
UInt32 Utils_tilerGetOriAddr(UInt32 tilerAddr, UInt32 cntMode, UInt32 oriFlag, UInt32 width, UInt32 height);
UInt32 Utils_tilerAddr2CpuAddr(UInt32 tilerAddr);

const int anIirCoef1[11] = {8, -95, -49, 38, 76, 38, -111, -54, 17, -34, 17};
const int anIirCoef2[11] = {11, -72, -50, 26, 51, 26, -92, -53, 19, -38, 19};

//#ifdef ENABLE_TILER
//#define TILER_ENABLE    TRUE
//#else
#define TILER_ENABLE    FALSE
#undef ENABLE_BTE
//#endif

/*========================================================================= */
void isp_h3a_dma_end_handle();
void isp_isif_linenum_handle();
void isp_ipipeif_m2m_handle();
void isp_rsz_dma_end_handle();
void video_bsc_handle();
Int32 bufSwitchFull(UInt32 streamId);
int VIDEO_vsTskDelete();

static MSP_ERROR_TYPE drvIspCallback(MSP_PTR hMSP, MSP_PTR pAppData,
                                     MSP_EVENT_TYPE tEvent,
                                     MSP_OPAQUE nEventData1,
                                     MSP_OPAQUE nEventData2);
Int32 Iss_Disable_RszB();
/*========================================================================= */

volatile int raw_data_done;
extern isif_regs_ovly isif_reg;
extern rsz_A_regs_ovly rszA_reg;
extern rsz_B_regs_ovly rszB_reg;
extern ipipe_regs_ovly ipipe_reg;
extern ipipeif_regs_ovly ipipeif_reg;
extern rsz_regs_ovly rsz_reg;
extern h3a_regs_ovly h3a_reg;
extern iss_regs_ovly iss_regs;
extern isp_regs_ovly isp_regs;
extern bte_regs_ovly bte_reg;
Iss_CaptCommonObj gIss_captCommonObj = {.tskUpdate = NULL};
FVID2_Frame *PrimaryFrame_RszA;
FVID2_Frame *PrimaryFrame_RszB;
tStreamBuf curStreamBuf[2];
void *h3a_AEWBBuff;
void *h3a_AFBuff;
extern ti2a_output ti2a_output_params;
static Int32 gRawCaptureTriggerd;
static Int32 gYUVCaptureTriggerd;
static Int32 gTiler_xw_swap = 0;
static Int32 gMirrorMode;
Int32 isp_reset_trigger = 0;

#ifdef ENABLE_BTE
#define BTE_BWL 15

#define DIV_UP(n, alignment) ((n + alignment - 1)/alignment)

static Int32 configureBteContext(bteHandle *handleBTE, int streamNumber, int width, int height, int format, int rotationMode)
{
	bte_config_t *contextHandle;
	int i;
	int pitch;

    GT_assert(GT_DEFAULT_MASK, handleBTE != NULL);

	GT_assert(GT_DEFAULT_MASK, streamNumber < handleBTE->contextNumber/2);

	i = streamNumber * 2;
	contextHandle = &handleBTE->bte_config[i];

	/* First context */
	if (format == FVID2_DF_YUV422I_VYUY)
	{
		pitch = width * 2;
	}
	else
	{
		pitch = width;
	}

	contextHandle->context_end_addr = ((DIV_UP(contextHandle->context_start_addr + pitch, 16) - 1) << CSL_BTE_BTE_CONTEXT_END_0_X_SHIFT) +
		((height - 1) << CSL_BTE_BTE_CONTEXT_END_0_Y_SHIFT);

	if (rotationMode < 4)
	{
		if (format == FVID2_DF_YUV422I_VYUY)
			contextHandle->context_ctrl.grid = 0x2;
		else
			contextHandle->context_ctrl.grid = 0x0;
	}
	else
	{
		contextHandle->context_ctrl.grid = 0x1;
	}

	/* 4 lines */
	contextHandle->context_ctrl.trigger_threshold = pitch * 3 / 16 + 2;

	/* Second context for YUV420 */
	if (format != FVID2_DF_YUV422I_VYUY)
	{
		contextHandle++;

		contextHandle->context_end_addr = ((DIV_UP(contextHandle->context_start_addr + pitch, 16) - 1) << CSL_BTE_BTE_CONTEXT_END_0_X_SHIFT) +
			((height/2 - 1) << CSL_BTE_BTE_CONTEXT_END_0_Y_SHIFT);

		if (rotationMode < 4)
		{
			contextHandle->context_ctrl.grid = 0x2;
		}
		else
		{
			contextHandle->context_ctrl.grid = 0x1;
		}

		contextHandle->context_ctrl.trigger_threshold = pitch * 3 / 16 + 2;
	}

	return 0;
}

static Int32 startBteContext(bte_config_t *contextHandle, uint32 tilerAddress)
{
    GT_assert(GT_DEFAULT_MASK, contextHandle != NULL);

	contextHandle->frame_buffer_addr = tilerAddress;

	contextHandle->context_ctrl.start = 1;

	GT_assert(GT_DEFAULT_MASK, bte_config_context_ctrl(contextHandle) == BTE_SUCCESS);

	return 0;
}

static Int32 stopBteContext(bte_config_t *contextHandle)
{
    GT_assert(GT_DEFAULT_MASK, contextHandle != NULL);

	contextHandle->context_ctrl.start = 0;
	contextHandle->context_ctrl.flush = 0;
	contextHandle->context_ctrl.stop = 1;

	GT_assert(GT_DEFAULT_MASK, bte_config_ctrl(&contextHandle->context_ctrl, contextHandle->context_num) == BTE_SUCCESS);

	contextHandle->context_ctrl.start = 0;
	contextHandle->context_ctrl.flush = 1;
	contextHandle->context_ctrl.stop = 0;

	GT_assert(GT_DEFAULT_MASK, bte_config_ctrl(&contextHandle->context_ctrl, contextHandle->context_num) == BTE_SUCCESS);

	while ((bte_reg->BTE_HL_IRQSTATUS_RAW & (CSL_BTE_BTE_HL_IRQSTATUS_RAW_IRQ_CTX0_DONE_MASK << (contextHandle->context_num))) == 0);

	return 0;
}

/*
static Int32 checkBteStatus(int contextNumber)
{

	GT_assert(GT_DEFAULT_MASK, (bte_reg->BTE_HL_IRQSTATUS_RAW & CSL_BTE_BTE_HL_IRQSTATUS_RAW_IRQ_OCP_ERR_MASK) == 0);
	GT_assert(GT_DEFAULT_MASK, (bte_reg->BTE_HL_IRQSTATUS_RAW & CSL_BTE_BTE_HL_IRQSTATUS_RAW_IRQ_INVALID_MASK) == 0);
	GT_assert(GT_DEFAULT_MASK, (bte_reg->BTE_HL_IRQSTATUS_RAW & (CSL_BTE_BTE_HL_IRQSTATUS_RAW_IRQ_CTX0_DONE_MASK << contextNumber)) == 1);
	GT_assert(GT_DEFAULT_MASK, (bte_reg->BTE_HL_IRQSTATUS_RAW & (CSL_BTE_BTE_HL_IRQSTATUS_RAW_IRQ_CTX0_INVALID_MASK << contextNumber)) == 1);
	GT_assert(GT_DEFAULT_MASK, (bte_reg->BTE_HL_IRQSTATUS_RAW & (CSL_BTE_BTE_HL_IRQSTATUS_RAW_IRQ_CTX0_ERR_MASK << contextNumber)) == 1);

	return 0;
}*/
#endif

/*========================================================================= */

/* ===================================================================
 *  @func     bufSwitchFull
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
Int32 bufSwitchFull(UInt32 streamId)
{
    Int32 status = FVID2_SOK;
	Int32 offsetUV = 0;
    FVID2_Frame *pframe;
    Iss_CaptObj *pObj;
    Iss_CaptChObj *pChObj;
	Iss_CaptRtParams *pRtParams = NULL;
    pObj = &gIss_captCommonObj.captureObj[0];
    pChObj = &pObj->chObj[streamId][0];
    pChObj->pTmpFrame = NULL;
    status = VpsUtils_queGet(&pChObj->emptyQue, (Ptr *) & pframe, 1, BIOS_NO_WAIT);
    if (status == FVID2_SOK)
    {
        if (curStreamBuf[streamId].ptmpFrame != NULL)
        {
            FVID2_Frame *pframeSave;
			if(streamId == 0)
			{
				pframeSave = curStreamBuf[streamId].ptmpFrame;
				if (gYUVCaptureTriggerd == 1)
				{
					if ((pObj->createArgs.outStreamInfo[streamId].dataFormat == FVID2_DF_YUV420SP_UV) ||
						(pObj->createArgs.outStreamInfo[streamId].dataFormat == FVID2_DF_YUV420SP_VU))
					{
						if(pObj->vsEnable == 0)
						{
						    offsetUV  = (pObj->createArgs.pitch[0] * pObj->createArgs.scParams[0].outHeight * 3)/2;
						}
						else
						{
						    offsetUV  = (pObj->createArgs.pitch[0] * \
							            ((pObj->createArgs.scParams[0].outHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR) * 3)/2;
						}
					}
					else if ((pObj->createArgs.outStreamInfo[streamId].dataFormat == FVID2_DF_YUV422I_VYUY))
					     {
							if(pObj->vsEnable == 0)
							{
								offsetUV  = pObj->createArgs.pitch[0] * pObj->createArgs.scParams[0].outHeight * 2;
							}
							else
							{
							    offsetUV  = pObj->createArgs.pitch[0] * \
								            ((pObj->createArgs.scParams[0].outHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR) * 2;
							}
						 }
					memcpy(gIss_captCommonObj.YUVcaptureAddr, pframeSave->addr[1][0], offsetUV);
					gIss_captCommonObj.gIttParams->Raw_Data_Ready = 1;
					gYUVCaptureTriggerd = 0;
					gIss_captCommonObj.YUVcaptureAddr = NULL;
				}
            }
			if(pObj->createArgs.captureMode == ISS_CAPT_INMODE_ISIF)
			{
				if(NULL != curStreamBuf[streamId].ptmpFrame->perFrameCfg)
				{
					pRtParams = (Iss_CaptRtParams *)curStreamBuf[streamId].ptmpFrame->perFrameCfg;
					pRtParams->captureOutWidth = (pObj->createArgs.scParams[streamId].outWidth & 0xFFFFFFF0);
					pRtParams->captureOutHeight = pObj->createArgs.scParams[streamId].outHeight;
					pRtParams->captureOutPitch = pObj->createArgs.pitch[streamId];
					if ((pObj->createArgs.outStreamInfo[streamId].dataFormat == FVID2_DF_YUV422I_VYUY))
						pRtParams->captureOutPitch = pObj->createArgs.pitch[streamId] * 2;
					if(gTiler_xw_swap == 1)
					{
						pRtParams->captureOutWidth = (pObj->createArgs.scParams[streamId].outHeight & 0xFFFFFFF0);
						pRtParams->captureOutHeight = pObj->createArgs.scParams[streamId].outWidth;
					}
				}
			    status = VpsUtils_quePut(&pObj->chObj[streamId][0].tmpQue,
                                         curStreamBuf[streamId].ptmpFrame,
                                         BIOS_WAIT_FOREVER);
            }
            else
            {
			    // ISS_CAPT_INMODE_DDR mode
			    curStreamBuf[streamId].ptmpFrame->addr[0][0] = curStreamBuf[streamId].ptmpFrame->addr[1][0];
				status = VpsUtils_quePut(&pObj->fullQue[streamId],
                                         curStreamBuf[streamId].ptmpFrame,
                                         BIOS_WAIT_FOREVER);
            }
            if (status == FVID2_SOK)
            {
                curStreamBuf[streamId].ptmpFrame = pframe;
            }
            else
            {
                // Output queue is full.
                // Send the filled buffer back to empty pool.
                status = VpsUtils_quePut(&pObj->chObj[streamId][0].emptyQue,
                                         curStreamBuf[streamId].ptmpFrame,
                                         BIOS_WAIT_FOREVER);
                if (status == FVID2_SOK)
                {
                    curStreamBuf[streamId].ptmpFrame = pframe;
                }
                else
                {
                    Vps_rprintf(" Intput and Output queues FULL!!!!");
                }
            }
        }
        else
        {
            curStreamBuf[streamId].ptmpFrame = pframe;
        }
    }
    else
    {
#ifdef ISS_DEBUG_RT
        Vps_rprintf("Stream ID %d: Input queue empty condition.", streamId);
#endif
    }
    return status;
}
/* ===================================================================
 *  @func     isp_h3a_dma_end_handle
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
void isp_h3a_dma_end_handle()
{
    // Buffer switch logic implementation - TODO
}

/* ===================================================================
 *  @func     isp_rsz_dma_end_handle
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
void isp_rsz_dma_end_handle()
{
#ifdef ENABLE_BTE
    Iss_CaptObj *pObj = &gIss_captCommonObj.captureObj[0];
	UInt32 oriBufOffsetY, oriBufOffsetC;
#endif

    if (gMirrorMode == 1)
    {
#if (TILER_ENABLE == FALSE)
        rsz_cfg_flip(RESIZER_A, (RSZ_FLIP_CFG_T) curStreamBuf[0].mirrorMode);
        rsz_cfg_flip(RESIZER_B, (RSZ_FLIP_CFG_T) curStreamBuf[1].mirrorMode);
#endif
        gMirrorMode = 0;
    }

#ifdef ENABLE_BTE
/*
	checkBteStatus(0);
	if (pObj->createArgs.outStreamInfo[0].dataFormat != FVID2_DF_YUV422I_VYUY)
		checkBteStatus(1);
*/
	stopBteContext(&gIss_captCommonObj.handleBTE.bte_config[0]);
	if (pObj->createArgs.outStreamInfo[0].dataFormat != FVID2_DF_YUV422I_VYUY)
		stopBteContext(&gIss_captCommonObj.handleBTE.bte_config[1]);

	configureBteContext(&gIss_captCommonObj.handleBTE, 0,
		pObj->createArgs.scParams[0].outWidth, pObj->createArgs.scParams[0].outHeight,
		pObj->createArgs.outStreamInfo[0].dataFormat, curStreamBuf[0].mirrorMode);

    oriBufOffsetY = Utils_tilerGetOriAddr((uint32)curStreamBuf[0].ptmpFrame->addr[1][0], 0, curStreamBuf[0].mirrorMode,
		pObj->createArgs.scParams[0].outWidth, pObj->createArgs.scParams[0].outHeight);
	startBteContext(&gIss_captCommonObj.handleBTE.bte_config[0], oriBufOffsetY);

	if (pObj->createArgs.outStreamInfo[0].dataFormat != FVID2_DF_YUV422I_VYUY)
	{
		oriBufOffsetC = Utils_tilerGetOriAddr((uint32)curStreamBuf[0].ptmpFrame->addr[1][1], 1, curStreamBuf[0].mirrorMode,
			pObj->createArgs.scParams[0].outWidth, pObj->createArgs.scParams[0].outHeight/2);
		startBteContext(&gIss_captCommonObj.handleBTE.bte_config[1], oriBufOffsetC);
	}
#endif

	// PP DRV callback
	Iss_ispDrvCallBack();
}

/* ===================================================================
 *  @func     calc_flip_offsets
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
void calc_flip_offsets(Int16 dataFormat, Int16 streamId, Int32 pitch, Int32 height, Int32 width)
{
#if (TILER_ENABLE == FALSE)
    if (dataFormat == 0)
    {
        curStreamBuf[streamId].flipVOffsetY = (width * 2 * (height - 1)) / 4;
        curStreamBuf[streamId].flipVOffsetC = (width * (height / 2 - 1)) / 4;
        curStreamBuf[streamId].flipHOffsetYC = (width * 2 - 1) / 4;
    }
    else if (dataFormat == 1)
    {
        curStreamBuf[streamId].flipVOffsetY = (width * (height - 1)) / 4;
        curStreamBuf[streamId].flipVOffsetC = (width * (height / 2 - 1)) / 4;
        curStreamBuf[streamId].flipHOffsetYC = (width - 1) / 4;
    }
    else
    {
        curStreamBuf[streamId].flipVOffsetY = 0;
        curStreamBuf[streamId].flipVOffsetC = 0;
        curStreamBuf[streamId].flipHOffsetYC = 0;
    }
    if (curStreamBuf[streamId].flipV == TRUE)
    {
        gIss_captCommonObj.buffOffsetY += curStreamBuf[streamId].flipVOffsetY;
        gIss_captCommonObj.buffOffsetYC += curStreamBuf[streamId].flipVOffsetC;
    }
    if (curStreamBuf[streamId].flipH == TRUE)
    {
        gIss_captCommonObj.buffOffsetY += curStreamBuf[streamId].flipHOffsetYC;
        gIss_captCommonObj.buffOffsetYC += curStreamBuf[streamId].flipHOffsetYC;
    }
#else
	Int32 oriBufOffsetY, oriBufOffsetC;
	Int32 widthYC, heightYC;

	if (curStreamBuf[streamId].mirrorMode <= 3)
	{
		gTiler_xw_swap = 0;
    	gIss_captCommonObj.pitch[0] =  16u*1024u;
		gIss_captCommonObj.pitch[1] =  32u*1024u;
	}
	else
	{
		gTiler_xw_swap = 1;
		gIss_captCommonObj.pitch[0] =  8u*1024u;
		gIss_captCommonObj.pitch[1] =  8u*1024u;
	}

	widthYC = width;
	heightYC = height/2;
	switch (curStreamBuf[streamId].mirrorMode)
	{
		case 1:
	    	*(volatile unsigned *)0x4E000224 = 0x9000;
			break;
    	case 2:
	    	*(volatile unsigned *)0x4E000224 = 0xA000;
			break;
    	case 3:
	    	*(volatile unsigned *)0x4E000224 = 0xB000;
			break;
		case 4:
			*(volatile unsigned *)0x4E000224 = 0xC000;
			break;
		case 5:
			*(volatile unsigned *)0x4E000224 = 0xD000;
			break;
		case 6:
			*(volatile unsigned *)0x4E000224 = 0xE000;
			break;
		default:
	    	*(volatile unsigned *)0x4E000224 = 0x8000;
			break;
    }

    oriBufOffsetY = Utils_tilerGetOriAddr((Int32)(curStreamBuf[streamId].ptmpFrame->addr[1][0]), 0, curStreamBuf[streamId].mirrorMode, width, height);
    oriBufOffsetC = Utils_tilerGetOriAddr((Int32)(curStreamBuf[streamId].ptmpFrame->addr[1][1]), 1, curStreamBuf[streamId].mirrorMode, widthYC, heightYC);
    gIss_captCommonObj.buffOffsetY = (Ptr)Utils_tilerAddr2CpuAddr((Int32)oriBufOffsetY);
    gIss_captCommonObj.buffOffsetYC = (Ptr)Utils_tilerAddr2CpuAddr((Int32)oriBufOffsetC);
#endif
}

/* ===================================================================
 *  @func     isp_isif_linenum_handle
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
void isp_isif_linenum_handle()
{
    Int32 status = FVID2_SOK;
    Int32 streamId;
    Iss_CaptObj *pObj;
	Int32 imgHeight, imgWidth;
	FVID2_Frame *pframe;
	#if TILER_ENABLE
	Int32 magic;
	Int32 point;
	#endif
	MSP_IspQueryHistDataT histData;

	Int32 rsz_err = *(MSP_U32 *) (0x55050024);

	pObj = &gIss_captCommonObj.captureObj[0];

	if ((rsz_err & 0xc0000)&& (isp_reset_trigger == 0))
	{
		Vps_rprintf("\r\n --- Resizer overflow! restarting...\n");
		for(streamId = 0; streamId < pObj->numStream; streamId++)
		{
			status = VpsUtils_queGet(&pObj->chObj[streamId][0].tmpQue, (Ptr *) & pframe, 1, BIOS_NO_WAIT);

			while (status == FVID2_SOK)
			{
				#if TILER_ENABLE
				if (gIss_captCommonObj.magic_point == 2)
					point = 0;
				else
					point = gIss_captCommonObj.magic_point + 1;

				if (gIss_captCommonObj.magic[point] != NULL) {
					magic = *(MSP_U32 *)(gIss_captCommonObj.magic[point]);
					if( magic == 0x12341234) {
						VpsUtils_quePut(&pObj->chObj[streamId][0].emptyQue, pframe, BIOS_WAIT_FOREVER);
					} else {
						VpsUtils_quePut(&pObj->fullQue[streamId], pframe, BIOS_WAIT_FOREVER);
					}
				}
				#else
				VpsUtils_quePut(&pObj->chObj[streamId][0].emptyQue, pframe, BIOS_WAIT_FOREVER);
				#endif
				status = VpsUtils_queGet(&pObj->chObj[streamId][0].tmpQue, (Ptr *) & pframe, 1, BIOS_NO_WAIT);
			}
		}
		if (pObj->fullQue[0].count > 0)
			pObj->cbPrm.fdmCbFxn(pObj->cbPrm.fdmData,pObj);

		isp_reset_trigger = 1;
	} else {
		for(streamId = 0; streamId < pObj->numStream; streamId++)
		{
			status = VpsUtils_queGet(&pObj->chObj[streamId][0].tmpQue, (Ptr *) & pframe, 1, BIOS_NO_WAIT);

			while (status == FVID2_SOK)
			{
				#if TILER_ENABLE
				if (gIss_captCommonObj.magic_point == 2)
					point = 0;
				else
					point = gIss_captCommonObj.magic_point + 1;

				if (gIss_captCommonObj.magic[point] != NULL) {
					magic = *(MSP_U32 *)(gIss_captCommonObj.magic[point]);
					if( magic == 0x12341234) {
						VpsUtils_quePut(&pObj->chObj[streamId][0].emptyQue, pframe, BIOS_WAIT_FOREVER);
					} else {
						VpsUtils_quePut(&pObj->fullQue[streamId], pframe, BIOS_WAIT_FOREVER);
					}
				} else {
					VpsUtils_quePut(&pObj->chObj[streamId][0].emptyQue, pframe, BIOS_WAIT_FOREVER);
				}
				#else
				VpsUtils_quePut(&pObj->fullQue[streamId], pframe, BIOS_WAIT_FOREVER);
				#endif
				status = VpsUtils_queGet(&pObj->chObj[streamId][0].tmpQue, (Ptr *) & pframe, 1, BIOS_NO_WAIT);
			}
			if (pObj->fullQue[0].count > 0) {
				pObj->cbPrm.fdmCbFxn(pObj->cbPrm.fdmData,pObj);
			}
		}
	}
	for (streamId = 0; streamId < pObj->numStream; streamId++)
	{
		if (((pObj->chObj[streamId][0].frameCount < 30) && (((pObj->chObj[streamId][0].frameSkipMask >> pObj->chObj[streamId][0].frameCount) & 0x1) == 0u)) ||
		((pObj->chObj[streamId][0].frameCount >= 30) && (((pObj->chObj[streamId][0].frameSkipMaskHigh >> (pObj->chObj[streamId][0].frameCount - 30)) & 0x1) == 0u)) ||
		(pObj->createArgs.captureMode == ISS_CAPT_INMODE_DDR))
        {
			if (isp_reset_trigger == 0) {
				status = bufSwitchFull(streamId);
			} else {
				status = FVID2_SOK;
			}

			if(pObj->vsEnable == 0)
			{
				imgHeight = pObj->createArgs.scParams[streamId].outHeight;
				imgWidth = pObj->createArgs.scParams[streamId].outWidth;
			}
			else
			{
				imgHeight = (pObj->createArgs.scParams[streamId].outHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
				imgWidth = (pObj->createArgs.scParams[streamId].outWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
			}
			if (status == FVID2_SOK)
			{
				if(pObj->createArgs.captureMode == ISS_CAPT_INMODE_ISIF)
				{
					gIss_captCommonObj.buffOffsetY = curStreamBuf[streamId].ptmpFrame->addr[1][0];
					gIss_captCommonObj.buffOffsetYC = curStreamBuf[streamId].ptmpFrame->addr[1][1];
					gIss_captCommonObj.pitch[0] =  pObj->createArgs.pitch[streamId];
					gIss_captCommonObj.pitch[1] =  pObj->createArgs.pitch[streamId];
					if (streamId == 0)
					{
						if(pObj->glbcEnable == 1){
							ipipe_config_boxcar_addr((UInt32)curStreamBuf[streamId].ptmpFrame->blankData);
						}
						if ((pObj->createArgs.outStreamInfo[streamId].dataFormat == FVID2_DF_YUV420SP_UV) ||
						(pObj->createArgs.outStreamInfo[streamId].dataFormat == FVID2_DF_YUV420SP_VU))
						{
							calc_flip_offsets(1, streamId, pObj->createArgs.pitch[0], imgHeight, imgWidth);
							#if TILER_ENABLE
							if(streamId == 0) {
								if (pObj->createArgs.scParams[0].outHeight == 720) {
									gIss_captCommonObj.magic[gIss_captCommonObj.magic_point] = gIss_captCommonObj.buffOffsetY + 0x2CF13F;
								}
								else if(pObj->createArgs.scParams[0].outHeight == 480) {
									gIss_captCommonObj.magic[gIss_captCommonObj.magic_point] = gIss_captCommonObj.buffOffsetY + 0x1DF09F;
								}
								else {
									gIss_captCommonObj.magic[gIss_captCommonObj.magic_point] = gIss_captCommonObj.buffOffsetY + 0x4371DF;
								}
								*(MSP_U32 *) (gIss_captCommonObj.magic[gIss_captCommonObj.magic_point]) = 0x12341234;
								if (gIss_captCommonObj.magic_point == 2)
									gIss_captCommonObj.magic_point = 0;
								else
									gIss_captCommonObj.magic_point++;
							}
							#endif
							#ifndef ENABLE_BTE
							status = issSetRszOutAddress(RESIZER_A, RSZ_YUV420_Y_OP, gIss_captCommonObj.buffOffsetY, gIss_captCommonObj.pitch[0]);
							status = issSetRszOutAddress(RESIZER_A, RSZ_YUV420_C_OP, gIss_captCommonObj.buffOffsetYC, gIss_captCommonObj.pitch[1]);
							#else
							status = issSetRszOutAddress(RESIZER_A, RSZ_YUV420_Y_OP,
							(void *)(gIss_captCommonObj.handleBTE.baseAddress + gIss_captCommonObj.handleBTE.bte_config[0].context_start_addr), BTE_PITCH);
							status = issSetRszOutAddress(RESIZER_A, RSZ_YUV420_C_OP,
							(void *)(gIss_captCommonObj.handleBTE.baseAddress + gIss_captCommonObj.handleBTE.bte_config[1].context_start_addr), BTE_PITCH);
							#endif
						}
						else if ((pObj->createArgs.outStreamInfo[streamId].dataFormat == FVID2_DF_YUV422I_VYUY))
						{
							calc_flip_offsets(0, streamId, pObj->createArgs.pitch[0], imgHeight, imgWidth);
							#ifndef ENABLE_BTE
							status = issSetRszOutAddress(RESIZER_A, RSZ_YUV422_RAW_RGB_OP, gIss_captCommonObj.buffOffsetY, gIss_captCommonObj.pitch[0] * 2);
							#else
							status = issSetRszOutAddress(RESIZER_A, RSZ_YUV422_RAW_RGB_OP,
							(void *)(gIss_captCommonObj.handleBTE.baseAddress + gIss_captCommonObj.handleBTE.bte_config[0].context_start_addr), BTE_PITCH);
							#endif
						}
					}
					else
					{
						if ((pObj->createArgs.outStreamInfo[streamId].dataFormat == FVID2_DF_YUV420SP_UV) ||
						(pObj->createArgs.outStreamInfo[streamId].dataFormat == FVID2_DF_YUV420SP_VU))
						{
							calc_flip_offsets(1, streamId, pObj->createArgs.pitch[1], imgHeight, imgWidth);
							status = issSetRszOutAddress(RESIZER_B, RSZ_YUV420_Y_OP, gIss_captCommonObj.buffOffsetY, gIss_captCommonObj.pitch[0]);
							status = issSetRszOutAddress(RESIZER_B, RSZ_YUV420_C_OP, gIss_captCommonObj.buffOffsetYC, gIss_captCommonObj.pitch[1]);
						}
						else if ((pObj->createArgs.outStreamInfo[streamId].dataFormat == FVID2_DF_YUV422I_VYUY))
						{
							calc_flip_offsets(0, streamId, pObj->createArgs.pitch[1], imgHeight, imgWidth);
							status = issSetRszOutAddress(RESIZER_B, RSZ_YUV422_RAW_RGB_OP, gIss_captCommonObj.buffOffsetY, gIss_captCommonObj.pitch[0] * 2);
						}
					}
				}
				else
				{
					// ISS_CAPT_INMODE_DDR mode
					Iss_ispWriteReg(&isif_reg->CADU,(((UInt32)curStreamBuf[streamId].ptmpFrame->addr[1][0] >> 21) & 0x7FF),0,11);
					Iss_ispWriteReg(&isif_reg->CADL,(((UInt32)curStreamBuf[streamId].ptmpFrame->addr[1][0] >> 5) & 0xFFFF),0,16);
					if(pObj->glbcEnable == 1)
						ipipe_config_boxcar_addr((UInt32)curStreamBuf[streamId].ptmpFrame->blankData);
				}
			}
			else
			{
				// Vps_rprintf(" Buffer switch failed for stream ID: %d", streamId);
			}
		}
		pObj->chObj[streamId][0].frameCount = (pObj->chObj[streamId][0].frameCount + 1u) % 60u;
	}

	if (gRawCaptureTriggerd > 0)
	{
		gRawCaptureTriggerd++;
		if (gRawCaptureTriggerd > 2)
		{
			gRawCaptureTriggerd = 0;
			issTriggerRawCapture(NULL, ISIF_STOP_WRITE);
			raw_data_done = 1;
			gIss_captCommonObj.gIttParams->Raw_Data_Ready = 1;
		}
	}

	if (status == FVID2_SOK)
	{
		if(pObj->vsEnable == 0)
		{
			//Semaphore_post(gIss_captCommonObj.semUpdate);
		}
	}

	/* Get the Histogram data */
	histData.pHistBuffer = pObj->histData;
	histData.nHistRegion = 0;
	histData.eHistColor  = MSP_IPIPE_HIST_Y;
	MSP_ISP_query(gIss_captCommonObj.pModuleInstance->hIspHandle,(MSP_INDEXTYPE) MSP_ISP_QUERY_GET_HIST,&histData);

	pObj->histSize = histData.nOutHistSize;

	if (isp_reset_trigger == 1)
	{
		pObj->cbPrm.fdmCbFxn(pObj->cbPrm.fdmData,pObj);
	}

}

/* ===================================================================
 *  @func     Iss_captInit
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
Int32 Iss_captInit()
{
    Int32 status = FVID2_SOK, instId;
    /*
     * set to 0's
     */
    memset(&gIss_captCommonObj, 0, sizeof(gIss_captCommonObj));
    /*
     * create driver locks
     */
    for (instId = 0; instId < ISS_CAPT_INST_MAX; instId++)
        status |= Iss_captLockCreate(&gIss_captCommonObj.captureObj[instId]);
    if (status != FVID2_SOK)
        return status;
    gRawCaptureTriggerd = 0;
    gYUVCaptureTriggerd = 0;
    gMirrorMode = 0;
    if (status == FVID2_SOK)
    {
        /*
         * register driver to FVID2 layer
         */
        gIss_captCommonObj.fvidDrvOps.create = (FVID2_DrvCreate) Iss_captCreate;
        gIss_captCommonObj.fvidDrvOps.delete = Iss_captDelete;
        gIss_captCommonObj.fvidDrvOps.control = Iss_captControl;
        gIss_captCommonObj.fvidDrvOps.queue = Iss_captQueue;
        gIss_captCommonObj.fvidDrvOps.dequeue = Iss_captDequeue;
        gIss_captCommonObj.fvidDrvOps.processFrames = NULL;
        gIss_captCommonObj.fvidDrvOps.getProcessedFrames = NULL;
        gIss_captCommonObj.fvidDrvOps.drvId = FVID2_ISS_CAPT_DRV;
        status = FVID2_registerDriver(&gIss_captCommonObj.fvidDrvOps);
        if (status != FVID2_SOK)
        {
            // Error - de-init previously inited stuff's
        }
    }
    return status;
}

/* ===================================================================
 *  @func     Iss_captDeInit
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
Int32 Iss_captDeInit()
{
    Int32 instId;
    /*
     * unregister from FVID2 layer
     */
    FVID2_unRegisterDriver(&gIss_captCommonObj.fvidDrvOps);
    /*
     * delete driver locks
     */
    for (instId = 0; instId < ISS_CAPT_INST_MAX; instId++)
        Iss_captLockDelete(&gIss_captCommonObj.captureObj[instId]);

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     Iss_captCreate
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
Fdrv_Handle Iss_captCreate(UInt32 drvId,
                           UInt32 instanceId,
                           Iss_CaptCreateParams * createArgs,
                           Iss_CaptCreateStatus * createStatus,
                           const FVID2_CbParams * cbPrm)
{
	Iss_CaptObj *pObj;
	int count;
    Semaphore_Params semParams;
    Int32 status;
    Int32 numPaxels;
    Int32 AEWBBuffSize;
    Int32 pixCtWin;
    MSP_U32 eMSP = MSP_ERROR_NONE;
    MSP_IspInterruptCfgT tIspInterruptCfg;
    MSP_IspConfigSizesT *pConfigSizes;
    MSP_IssCallbackRegCfgT tIssCallbackRegCfg;
    MSP_IspParamCfgT tIspParamCfg;
    MSP_IspQueryBscSizesT tIspBscSizes;
    MSP_IspYuvRangeCfgT tIspYuvRangeCfg;
    MSP_IspCallbackRegCfgT tCallbackRegCfg;
	Iss_2AObj *p2AObj;

#ifdef ENABLE_BTE
	int i;

	bte_config_t *contextHandle;
#endif
    if (instanceId == ISS_CAPT_INST_ALL)
    {
        /*
         * gloabl handle open requested, no action required,
         * just return a special handle ID
         */
        return (Fdrv_Handle) ISS_CAPT_INST_ALL;
    }
    if (createStatus == NULL)
        return NULL;

    pConfigSizes = NULL;
    curStreamBuf[0].bufIsNew = FALSE;
    curStreamBuf[0].ptmpFrame = NULL;
    curStreamBuf[0].flipH = FALSE;
    curStreamBuf[0].flipV = FALSE;
    curStreamBuf[0].flipVOffsetY = 0;
    curStreamBuf[0].flipVOffsetC = 0;
    curStreamBuf[0].flipHOffsetYC = 0;
    curStreamBuf[0].mirrorMode = 0;
    curStreamBuf[1].bufIsNew = FALSE;
    curStreamBuf[1].ptmpFrame = NULL;
    curStreamBuf[1].flipH = FALSE;
    curStreamBuf[1].flipV = FALSE;
    curStreamBuf[1].flipVOffsetY = 0;
    curStreamBuf[1].flipVOffsetC = 0;
    curStreamBuf[1].flipHOffsetYC = 0;
    curStreamBuf[1].mirrorMode = 0;

    /*
     * parameter checking
     */
    if (instanceId >= ISS_CAPT_INST_MAX
        || createArgs == NULL || (drvId != FVID2_ISS_CAPT_DRV))
    {
        /*
         * invalid parameters - return NULL
         */
        createStatus->retVal = FVID2_EBADARGS;
        return NULL;
    }

    /*
     * get instance specific handle
     */
    pObj = &gIss_captCommonObj.captureObj[instanceId];

    /*
     * lock driver instance
     */
    Iss_captLock(pObj);

    /*
     * check if object is already opended
     */
    if (pObj->state != ISS_CAPT_STATE_IDLE)
    {
        createStatus->retVal = FVID2_EDEVICE_INUSE;
        Iss_captUnlock(pObj);
        return NULL;
    }

    /*
     * copy create arguments
     */
    memcpy(&pObj->createArgs, createArgs, sizeof(pObj->createArgs));

    /*
     * set instance Id
     */
    pObj->instanceId = instanceId;

    /*
     * copy callback
     */
    if (cbPrm != NULL)
        memcpy(&pObj->cbPrm, cbPrm, sizeof(pObj->cbPrm));

    /*
     * set number of streams
     */
    gIss_captCommonObj.magic[0] = NULL;	//jaeki
    gIss_captCommonObj.magic[1] = NULL;	//jaeki
    gIss_captCommonObj.magic[2] = NULL;	//jaeki
	gIss_captCommonObj.magic_point = 0;	//jaeki

    pObj->numStream = createArgs->numStream;

	pObj->histSize = 0;
	pObj->vsEnable = createArgs->vsEnable;
	pObj->glbcEnable = createArgs->glbcEnable;
	if(pObj->createArgs.captureMode == ISS_CAPT_INMODE_DDR)
	{
	    pObj->numStream = 1;
		pObj->vsEnable  = 0;
	}
    pObj->createArgs.inDataFormat = createArgs->inDataFormat;

    /*
     * create start and stop sync semaphores
     */
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    pObj->semStopDone = Semaphore_create(0, &semParams, NULL);
    if (pObj->semStopDone == NULL)
    {
        createStatus->retVal = FVID2_EALLOC;
        Iss_captUnlock(pObj);
        return NULL;
    }

    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    pObj->semStartDone = Semaphore_create(0, &semParams, NULL);
    if (pObj->semStartDone == NULL)
    {
        Semaphore_delete(&pObj->semStopDone);
        createStatus->retVal = FVID2_EALLOC;
        Iss_captUnlock(pObj);
        return NULL;
    }

    /*
     * create channel specific objects
     */
    status = Iss_captCreateChObj(pObj);
    if (status == FVID2_SOK)
    {
        /*
         * create input and output queues
         */
        status = Iss_captCreateQueues(pObj);
    }
    createStatus->retVal = status;
    if (status != FVID2_SOK)
    {
        /*
         * Error - free previously allocated objects
         */
        Semaphore_delete(&pObj->semStartDone);
        Semaphore_delete(&pObj->semStopDone);
        Iss_captUnlock(pObj);
        return NULL;
    }
    pObj->createArgs.pCompPrivate = (MSP_PTR) malloc(sizeof(VideoModuleInstanceT));
    if (pObj->createArgs.pCompPrivate == NULL)
    {
        return NULL;
    }

    gIss_captCommonObj.pModuleInstance = (VideoModuleInstanceT *) (pObj->createArgs.pCompPrivate);
    gIss_captCommonObj.pModuleInstance->pH3aAewbBuff = NULL;
    gIss_captCommonObj.pModuleInstance->pH3aAfBuff = NULL;
    gIss_captCommonObj.pModuleInstance->pH3aAewbBuffNoPad = NULL;
    gIss_captCommonObj.pModuleInstance->hInit = NULL;
    gIss_captCommonObj.pModuleInstance->hIspHandle = NULL;
    gIss_captCommonObj.pModuleInstance->nLscEnable = 0;
    gIss_captCommonObj.pModuleInstance->nLscTableIsValid = MSP_FALSE;
    gIss_captCommonObj.pModuleInstance->bBscFromFD = MSP_TRUE;
    gIss_captCommonObj.pModuleInstance->bAllocateOutBuff = MSP_TRUE;
    gIss_captCommonObj.pModuleInstance->nAlterSeqNum = 0;
    gIss_captCommonObj.pModuleInstance->nBscAlterSeqNum = 0;
    gIss_captCommonObj.pModuleInstance->nIssInX = 0;
    gIss_captCommonObj.pModuleInstance->nIssInY = 0;
    gIss_captCommonObj.pModuleInstance->nARPrvAlterSeqNum = 0;
    gIss_captCommonObj.pModuleInstance->nARCapAlterSeqNum = 0;
    gIss_captCommonObj.pModuleInstance->bFilledBuffsInvalidate = MSP_FALSE;
    gIss_captCommonObj.pModuleInstance->bVidStab = MSP_FALSE;
    gIss_captCommonObj.pModuleInstance->nLscEnable = 0;
    gIss_captCommonObj.pModuleInstance->nLscTableIsValid = MSP_FALSE;
    gIss_captCommonObj.pModuleInstance->bAllocateOutBuff = MSP_FALSE;
    gIss_captCommonObj.pModuleInstance->tIspAppParam.pAppData = gIss_captCommonObj.pModuleInstance;
    gIss_captCommonObj.pModuleInstance->tIspAppParam.MSP_callback = drvIspCallback;
    CLEAR_ALL_EVENTS(gIss_captCommonObj.pModuleInstance);

    gIss_captCommonObj.pModuleInstance->hIspHandle =
        (MSP_COMPONENT_TYPE *) malloc(sizeof(MSP_COMPONENT_TYPE));
    if (gIss_captCommonObj.pModuleInstance->hIspHandle == NULL)
    {
        goto exit0;
    }

    ((MSP_COMPONENT_TYPE *) (gIss_captCommonObj.pModuleInstance->hIspHandle))->tAppCBParam =
        gIss_captCommonObj.pModuleInstance->tIspAppParam;

    ((MSP_COMPONENT_TYPE*)gIss_captCommonObj.pModuleInstance->hIspHandle)->pCompPrivate = NULL;

    MSP_ISP_init(gIss_captCommonObj.pModuleInstance->hIspHandle, MSP_PROFILE_REMOTE);
    pConfigSizes = (MSP_PTR) malloc(sizeof(MSP_IspConfigSizesT));
    if (pConfigSizes == NULL)
    {
        goto exit0;
    }

    gIss_captCommonObj.pIssConfig = (MSP_PTR) calloc(1, sizeof(MSP_IspConfigProcessingT));
    if (gIss_captCommonObj.pIssConfig == NULL)
    {
        goto exit0;
    }

    // memset(gIss_captCommonObj.pIssConfig, 0, sizeof(MSP_IspConfigProcessingT));
    MSP_ISP_control(gIss_captCommonObj.pModuleInstance->hIspHandle, MSP_CTRLCMD_START, NULL);
    tCallbackRegCfg.nContext = MSP_ISP_CALLBACK_CONTEXT_HWI;
    tCallbackRegCfg.nPriority = 3;

    // REGISTER ISP INTERRUPTS
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_RSZ_END_ISR;
    eMSP =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_REG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_ISIF_VD_ISR;
    eMSP =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_REG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_IPIPE_INT_BSC;
    eMSP =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_REG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_H3A_END_ISR;
    eMSP =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_REG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_ISIF_2DLSC_ISR;
    eMSP =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_REG_CALLBACK,
                       &tCallbackRegCfg);
    tIssCallbackRegCfg.eInterruptId = MSP_ISS_DRV_CCP_LCM;
    eMSP =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISS_CFG_REG_CALLBACK,
                       &tIssCallbackRegCfg);

	isif_reg->VDINT0 = isif_reg->VDINT1 = isif_reg->VDINT2 = pObj->createArgs.scParams[0].inHeight - 5;

    *(MSP_U32 *) (0x5505002C) |= 1;                        // isp IRQ set
                                                           // register
    tIspInterruptCfg.bEnable = MSP_TRUE;
    tIspInterruptCfg.eInterruptId = MSP_ISS_DRV_CCP_LCM;
    eMSP =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISS_CFG_IRQ, &tIspInterruptCfg);
    tIspInterruptCfg.eInterruptId = MSP_ISS_DRV_IPIPE_INT_BSC;
    eMSP =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_INT, &tIspInterruptCfg);
    tIspInterruptCfg.eInterruptId = MSP_ISS_DRV_ISIF_2DLSC_ISR;
    MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle, (MSP_INDEXTYPE) MSP_ISP_CFG_INT,
                   &tIspInterruptCfg);

    pConfigSizes->nInSizeX = pObj->createArgs.scParams[0].inWidth;
    pConfigSizes->nInSizeY = pObj->createArgs.scParams[0].inHeight;
    pConfigSizes->nInSizePpln = pObj->createArgs.pitch[0];
    pConfigSizes->nInStartX = pObj->createArgs.scParams[0].InStartX;
    pConfigSizes->nInStartY = pObj->createArgs.scParams[0].InStartY;
	if(pObj->vsEnable == 0)
	{
		pConfigSizes->nOutSizeAX = pObj->createArgs.scParams[0].outWidth;
		pConfigSizes->nOutSizeAY = pObj->createArgs.scParams[0].outHeight;
	}
	else
	{
		pConfigSizes->nOutSizeAX = (pObj->createArgs.scParams[0].outWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
		pConfigSizes->nOutSizeAY = (pObj->createArgs.scParams[0].outHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;;
	}
	if((pConfigSizes->nOutSizeAX > pConfigSizes->nInSizeX) || (pConfigSizes->nOutSizeAY > pConfigSizes->nInSizeY))
	{
	    goto exit0;
	}

    pConfigSizes->nOutSizeABpln = pObj->createArgs.pitch[0];
    pConfigSizes->nOutSizeABplnC = 0;

    if (pObj->numStream == 2)
    {
	    if(pObj->vsEnable == 0)
		{
			pConfigSizes->nOutSizeBX = pObj->createArgs.scParams[1].outWidth;
			pConfigSizes->nOutSizeBY = pObj->createArgs.scParams[1].outHeight;
		}
		else
		{
			pConfigSizes->nOutSizeBX = (pObj->createArgs.scParams[1].outWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
			pConfigSizes->nOutSizeBY = (pObj->createArgs.scParams[1].outHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
		}
		if((pConfigSizes->nOutSizeBX > pConfigSizes->nInSizeX) || (pConfigSizes->nOutSizeBY > pConfigSizes->nInSizeY))
		{
			goto exit0;
		}

        pConfigSizes->nOutSizeBBpln = pObj->createArgs.pitch[1];
        pConfigSizes->nOutSizeBBplnC = 0;
    }
    else
    {
        pConfigSizes->nOutSizeBX = 720;
        pConfigSizes->nOutSizeBY = 480;
        pConfigSizes->nOutSizeBBpln = 720;
        pConfigSizes->nOutSizeBBplnC = 0;
    }

    pConfigSizes->nCropX = pObj->createArgs.scParams[0].inCropCfg.cropStartX;
    pConfigSizes->nCropY = pObj->createArgs.scParams[0].inCropCfg.cropStartY;
    pConfigSizes->nClockPercents = MSP_ISS_SPEED_MAX;
    eMSP =
        MSP_ISP_query(gIss_captCommonObj.pModuleInstance->hIspHandle,
                      (MSP_INDEXTYPE) MSP_ISP_QUERY_MAX_WIDTH, pConfigSizes);
    eMSP =
        MSP_ISP_query(gIss_captCommonObj.pModuleInstance->hIspHandle,
                      (MSP_INDEXTYPE) MSP_ISP_QUERY_CROP_SIZES, pConfigSizes);
    if (eMSP != MSP_ERROR_NONE)
    {
        // Vps_printf("Error MSP_ISP_query failed!!!\n");
    }

    gIss_captCommonObj.pModuleInstance->nLscTableIsValid = MSP_FALSE;
    gIss_captCommonObj.pModuleInstance->nOutStartX = 0;
    gIss_captCommonObj.pModuleInstance->nOutStartY = 0;
    gIss_captCommonObj.pModuleInstance->bVidStab = MSP_FALSE;
    gIss_captCommonObj.pModuleInstance->bAllocateOutBuff = MSP_FALSE;
    IssFillDefaultParams((iss_config_processing_t *) gIss_captCommonObj.pIssConfig);
    gIss_captCommonObj.pIssConfig->eMsbPos = MSP_IPIPE_BAYER_MSB_BIT11;
    gIss_captCommonObj.pIssConfig->ptBsc = &(gIss_captCommonObj.pModuleInstance->tBscCfg);
    gIss_captCommonObj.pIssConfig->ptLsc2D->nHDirDataOffset = 16;
    gIss_captCommonObj.pIssConfig->eColorPattern = MSP_IPIPE_BAYER_PATTERN_GRBG;

#ifdef IMGS_SONY_IMX122
	gIss_captCommonObj.pIssConfig->eColorPattern = MSP_IPIPE_BAYER_PATTERN_RGGB;
#endif

    gIss_captCommonObj.pIssConfig->nVpDevice = MSP_IPIPE_VP_DEV_PI;
    gIss_captCommonObj.pIssConfig->eFH3aValidity =
        (MSP_PROC_H3A_VALID_ID) (MSP_PROC_H3A_VALID_AEWB |
                                 MSP_PROC_H3A_VALID_H3A |
                                 MSP_PROC_H3A_VALID_AF);
    gIss_captCommonObj.pModuleInstance->tConfigParams.eInFormat = MSP_IPIPE_IN_FORMAT_BAYER;

    if ((pObj->createArgs.outStreamInfo[0].dataFormat == FVID2_DF_YUV420SP_UV)
        || (pObj->createArgs.outStreamInfo[0].dataFormat ==
            FVID2_DF_YUV420SP_VU))
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.eOutFormatA =
            MSP_IPIPE_OUT_FORMAT_YUV420;
    }
    else if ((pObj->createArgs.outStreamInfo[0].dataFormat ==
              FVID2_DF_YUV422I_VYUY))
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.eOutFormatA =
            MSP_IPIPE_OUT_FORMAT_YUV422;
    }
    else if(pObj->createArgs.outStreamInfo[0].dataFormat == FVID2_DF_BAYER_RAW)
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.eOutFormatA = MSP_IPIPE_OUT_FORMAT_YUV422;
    }
	else
	{
	       Vps_printf("Stream 0: Invalid output format configured!!!\n");
	}
    if ((pObj->createArgs.outStreamInfo[1].dataFormat == FVID2_DF_YUV420SP_UV)
        || (pObj->createArgs.outStreamInfo[1].dataFormat ==
            FVID2_DF_YUV420SP_VU))
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.eOutFormatB =
            MSP_IPIPE_OUT_FORMAT_YUV420;
    }
    else if ((pObj->createArgs.outStreamInfo[1].dataFormat ==
              FVID2_DF_YUV422I_VYUY))
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.eOutFormatB =
            MSP_IPIPE_OUT_FORMAT_YUV422;
    }
    else
    {
        Vps_printf("Stream 1: Invalid output format configured!!!\n");
    }

    gIss_captCommonObj.pModuleInstance->tConfigParams.eCompresIn = MSP_IPIPE_PROC_COMPR_NO;
    gIss_captCommonObj.pModuleInstance->tConfigParams.eCompresOut = MSP_IPIPE_PROC_COMPR_NO;
    gIss_captCommonObj.pModuleInstance->tConfigParams.nFFlipB = 0;
    gIss_captCommonObj.pModuleInstance->tConfigParams.nFMirrorB = 0;
    gIss_captCommonObj.pModuleInstance->tConfigParams.nFFlipA = 0;
    gIss_captCommonObj.pModuleInstance->tConfigParams.nFMirrorA = 0;
    gIss_captCommonObj.pModuleInstance->tConfigParams.nVidstabEnb = 0;
    // gIss_captCommonObj.pModuleInstance->tConfigParams.eInComp = 0;
    gIss_captCommonObj.pModuleInstance->tConfigParams.ptIssConfig = gIss_captCommonObj.pIssConfig;
    gIss_captCommonObj.pModuleInstance->tConfigParams.pLsc2DTableBuffer =
        gIss_captCommonObj.pModuleInstance->p2DLscTableBuff;
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nInSizeX =
        gIss_captCommonObj.pModuleInstance->nIssInX;
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nInSizeY =
        gIss_captCommonObj.pModuleInstance->nIssInY;
    gIss_captCommonObj.pModuleInstance->tConfigParams.ptIssConfig->eRszIpPortSel =
        MSP_RSZ_IP_IPIPEIF;
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeAX =
        pConfigSizes->nOutSizeAX; //pObj->createArgs.scParams[0].outWidth;
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeAY =
        pConfigSizes->nOutSizeAY; //pObj->createArgs.scParams[0].outHeight;
#ifndef ENABLE_BTE
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeABpln =
        pObj->createArgs.pitch[0];
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeABplnC =
        pObj->createArgs.pitch[0];
#else
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeABpln =
        BTE_PITCH;
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeABplnC =
        BTE_PITCH;
#endif
    if (pObj->numStream == 2)
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBX =
            pConfigSizes->nOutSizeBX; //pObj->createArgs.scParams[1].outWidth;
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBY =
            pConfigSizes->nOutSizeBY; //pObj->createArgs.scParams[1].outHeight;
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBBpln =
            pObj->createArgs.pitch[1];
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBBplnC =
            pObj->createArgs.pitch[1];
    }
    else
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBX = 720;
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBY = 480;
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBBpln = 720;
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBBplnC = 720;
    }
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nClockPercents =
        MSP_ISS_SPEED_MAX;
    tIspParamCfg.ptIssParams = &gIss_captCommonObj.pModuleInstance->tConfigParams;
    // RAJAT - need to correct the configuration being sent out to remove the error
    MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                   (MSP_INDEXTYPE) MSP_ISP_CFG_PROC_MEM_TO_MEM, &tIspParamCfg);
	/* RSZ A configuration */
	rszASetOutConfig(pObj->createArgs.scParams[0].inWidth,
					 pObj->createArgs.scParams[0].inHeight,
					 pConfigSizes->nOutSizeAX, pConfigSizes->nOutSizeAY);

	if(pObj->numStream == 1)
	{
		status = Iss_Disable_RszB();
		if(status == FVID2_EFAIL)
		Vps_rprintf("Iss_Disable_RszB Failed \n");
	}
	else if (pObj->numStream == 2)
	{
		/* RSZ B configuration */
		rszBSetOutConfig(pObj->createArgs.scParams[0].inWidth,
						 pObj->createArgs.scParams[0].inHeight,
						 pConfigSizes->nOutSizeBX, pConfigSizes->nOutSizeBY);
		//# phoong - crop param init
		pObj->createArgs.scParams[1].inCropCfg.cropStartX = 0;
		pObj->createArgs.scParams[1].inCropCfg.cropStartY = 0;
		pObj->createArgs.scParams[1].inCropCfg.cropWidth = pObj->createArgs.scParams[0].inWidth;
		pObj->createArgs.scParams[1].inCropCfg.cropHeight = pObj->createArgs.scParams[0].inHeight;
	}

	isif_reg->HDW = pObj->createArgs.scParams[0].inWidth + 17;
	isif_reg->VDW = pObj->createArgs.scParams[0].inHeight + 3;
	isif_reg->PPLN = isif_reg->HDW;
	isif_reg->LPFR = isif_reg->VDW * 2 + 1;
	isif_reg->LNH = pObj->createArgs.scParams[0].inWidth + 3;
	isif_reg->LNV = pObj->createArgs.scParams[0].inHeight + 1;
    ipipe_reg->SRC_COL = 177;
	ipipe_reg->SRC_VSZ = pObj->createArgs.scParams[0].inHeight + 3;
	ipipe_reg->SRC_HSZ = pObj->createArgs.scParams[0].inWidth + 17;
    gIss_captCommonObj.pModuleInstance->nLscTableIsValid = MSP_TRUE;

#ifdef IMGS_SONY_IMX122
	/* ISIF Size config*/
	isif_reg->HDW				= 1937;
	isif_reg->VDW				= 1083;
	isif_reg->PPLN				= 1937;
	isif_reg->LPFR				= 2167;
	isif_reg->LNH				= 1923;
	isif_reg->LNV				= 1081;

//	isif_reg->CCOLP				= 0x1B1B;//R-Gr-Gb-B
//	isif_reg->CCOLP				= 0x0606;//R-Gr-Gb-B
//	isif_reg->CCOLP 				= 0xB1B1;//Gb-B-R-Gr
//	isif_reg->CCOLP 				= 0x4E4E;//B-Gb-Gr-R
	isif_reg->CCOLP 				= 0xE4E4;//Gr-R-B-Gb

	/* IPIPE Size config */
//	ipipe_reg->SRC_COL			= 0x1B;//R-Gr-Gb-B
//	ipipe_reg->SRC_COL			= 0xB1;//Gb-B-R-Gr
//	ipipe_reg->SRC_COL			= 0x4E;//B-Gb-Gr-R
	ipipe_reg->SRC_COL			= 0xE4;//Gr-R-B-Gb
	ipipe_reg->SRC_VSZ			= 1081;//1083;
	ipipe_reg->SRC_HSZ			= 1921;//1929;
	ipipe_reg->SRC_HPS			= 156;//kjs
	ipipe_reg->SRC_VPS			= 32;//kjs
//	isif_reg->MODESET			=0;
#endif

	//*(MSP_U32 *)(0x5505006C) &= 0xFFFFFFF3;

    // ///////////////////////////////////////////////////////////////////////////////////////////////////////
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVCount = 32;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHCount = 16;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVSize = 32;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHSize = 120;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHIncr = 8;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVIncr = 8;

#ifdef IMGS_SONY_IMX122
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHPos = 0+156;//kjs
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVPos = 30+32;
#endif

    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptBlkWinDims->nVPos = 0;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptBlkWinDims->nHPos = 2;  // AEWINBLK WINH
                                                           // win height
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->nShiftValue = 0;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->nSaturationLimit = 980;//0x3FE;

	gIss_captCommonObj.pIssConfig->ptH3aAfParams->eAfEnable = MSP_H3A_FEATURE_ENABLE;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->eRgbPos = MSP_H3A_AF_RGBPOS_RG_GB_BAYER;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ePeakModeEn = MSP_H3A_FEATURE_ENABLE;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->eVerticalFocusEn = MSP_H3A_FEATURE_ENABLE;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->nIirStartPos = 30;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nVPos = 34;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nVSize = 64;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nHPos = 32;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nHSize = 308;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nVCount = 16;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nVIncr = 8;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nHCount = 6;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nHIncr = 8;

	for(count=0;count<11;count++){

		gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptIir1->anIirCoef[count] = anIirCoef1[count];

		gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptIir2->anIirCoef[count] = anIirCoef2[count];
	}

	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptIir1->nHfvThres = 100;

	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptIir2->nHfvThres = 100;

	for(count=0;count<5;count++){
		gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptFir1->anFirCoef[count] = 2;
		gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptFir2->anFirCoef[count] = 2;
	}
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptFir1->nVfvThres = 500;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptFir2->nVfvThres = 500;

    gIss_captCommonObj.pIssConfig->ptLsc2D->nGainTableAddress = (MSP_U32)memalign(256, 40240);
    GT_assert(GT_DEFAULT_MASK, gIss_captCommonObj.pIssConfig->ptLsc2D->nGainTableAddress != NULL);
    gIss_captCommonObj.pIssConfig->ptLsc2D->nGainTableLength = 40240;
    gIss_captCommonObj.pIssConfig->ptLsc2D->nOffsetTableAddress  = (MSP_U32)memalign(256, 40240);
    GT_assert(GT_DEFAULT_MASK, gIss_captCommonObj.pIssConfig->ptLsc2D->nOffsetTableAddress != NULL);
	gIss_captCommonObj.pIssConfig->ptLsc2D->nOffsetTableLength = 40240;
    numPaxels =
        (gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVCount +
         1) * gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHCount;
    AEWBBuffSize = (numPaxels * 32) + (((numPaxels + 7) / 8) * 32);
    pixCtWin =
        (gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVSize /
         gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVIncr) *
        (gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHSize /
         gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHIncr);
    gIss_captCommonObj.gH3ABufParams.h3aUpdate = 1;
    gIss_captCommonObj.gH3ABufParams.emptyFrame = 1;
    gIss_captCommonObj.gH3ABufParams.aewbNumPix = pixCtWin;
    gIss_captCommonObj.gH3ABufParams.aewbNumWinH =
        gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHCount;
    gIss_captCommonObj.gH3ABufParams.aewbNumWinV =
        gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVCount;

    gIss_captCommonObj.pModuleInstance->p2AObj = (Iss_2AObj*)malloc(sizeof(Iss_2AObj));
    if ( gIss_captCommonObj.pModuleInstance->p2AObj == NULL) {
        goto exit0;
    }
    p2AObj = (Iss_2AObj*)gIss_captCommonObj.pModuleInstance->p2AObj;
    p2AObj->pIspHandle      = gIss_captCommonObj.pModuleInstance->hIspHandle;
    p2AObj->pIssConfig      = gIss_captCommonObj.pIssConfig;
    p2AObj->h3aBufSize      = AEWBBuffSize;
	p2AObj->aewbNumPix      = pixCtWin;
	p2AObj->aewbNumWinH     = gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHCount;
	p2AObj->aewbNumWinV     = gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVCount;
	p2AObj->aewbVendor      = (AEWB_VENDOR)pObj->createArgs.aewbVendor;
	p2AObj->aewbMode        = pObj->createArgs.aewbMode;

	p2AObj->aewbModeFlag    = 1;
	p2AObj->aewbPriority    = pObj->createArgs.aewbPriority;
	p2AObj->SensorHandle    = pObj->createArgs.SensorHandle;
	p2AObj->cameraVipHandle = pObj;

	((Iss_2AObj*)(gIss_captCommonObj.pModuleInstance->p2AObj))->taskHndl = NULL;

	#if !(USE_NOT_2A)
    IssAlg_capt2AInit((Iss_2AObj*)gIss_captCommonObj.pModuleInstance->p2AObj);
    #endif

    // //////////////////////////////////////////////////////////////////////////////////////////////////////
    tIspBscSizes.pRowsBufferSize = 0;
    tIspBscSizes.pColsBufferSize = 0;
    tIspBscSizes.ptBscCfg = gIss_captCommonObj.pModuleInstance->tConfigParams.ptIssConfig->ptBsc;
    MSP_ISP_query(gIss_captCommonObj.pModuleInstance->hIspHandle,
                  (MSP_INDEXTYPE) MSP_ISP_QUERY_BSC_BUFF_SIZES, &tIspBscSizes);
    gIss_captCommonObj.pModuleInstance->nBscRowBuffSize = tIspBscSizes.pRowsBufferSize;
    gIss_captCommonObj.pModuleInstance->nBscColBuffSize = tIspBscSizes.pColsBufferSize;
    tIspYuvRangeCfg.eIspYuvRange = MSP_ISS_YUV_RANGE_FULL;
    MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                   (MSP_INDEXTYPE) MSP_ISP_CFG_YUV_RANGE, &tIspYuvRangeCfg);

	/* Histogram Config */
	{
		MSP_IpipeHistCfgT histogramCfg;
		MSP_IpipeHistDimT histogramDim[4];
		MSP_U8            gainTbl[4];

		memcpy(&histogramCfg,gIss_captCommonObj.pIssConfig->ptHistogram,sizeof(MSP_IpipeHistCfgT));

		histogramCfg.nOst  = 0;		// Free Run
		histogramCfg.nSel  = 1;		// Input Selection - From noise filter
		histogramCfg.nType = 2;		// G selection in Bayer mode - (Gb + Gr)/2
		histogramDim[0].nVPos  = 0;
		histogramDim[0].nVSize = pObj->createArgs.scParams[0].inHeight - 33;
		histogramDim[0].nHPos  = 0;
		histogramDim[0].nHSize = pObj->createArgs.scParams[0].inWidth - 1;

		gainTbl[0] = 6;				// R
		gainTbl[1] = 6;				// Gr
		gainTbl[2] = 6;				// Gb
		gainTbl[3] = 6;				// B
		histogramCfg.ptHistDim = histogramDim;
		histogramCfg.pnGainTbl = gainTbl;
		MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,(MSP_INDEXTYPE)MSP_ISP_CFG_HIST,&histogramCfg);
	}
	if(pObj->glbcEnable == 1)
	{
		MSP_IpipeBoxcarCfgT BoxCarCfg;
		memcpy(&BoxCarCfg,gIss_captCommonObj.pIssConfig->ptBoxcar,sizeof(MSP_IpipeBoxcarCfgT));
		BoxCarCfg.nEnable = TRUE;
		if(pObj->createArgs.captureMode == ISS_CAPT_INMODE_ISIF)
		BoxCarCfg.nOst = 0;
		else
		BoxCarCfg.nOst = 1;
		BoxCarCfg.nBoxSize = 1;
		BoxCarCfg.nShift = 3;
		BoxCarCfg.pAddr = NULL;

		MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,(MSP_INDEXTYPE)MSP_ISP_CFG_BOXCAR,&BoxCarCfg);
	}

    /*
     * mark state as created
     */
    pObj->state = ISS_CAPT_STATE_CREATED;
    /*
     * unlock driver instance
     */
    Iss_captUnlock(pObj);
    IssAlg_captInit();

#ifdef ENABLE_BTE
	GT_assert(GT_DEFAULT_MASK, bte_init() == BTE_SUCCESS);

	GT_assert(GT_DEFAULT_MASK, bte_open(1) == BTE_SUCCESS);/*0x20000000~0x3FFFFFFF*/

	gIss_captCommonObj.handleBTE.baseAddress = 0x20000000;

	gIss_captCommonObj.handleBTE.contextNumber =
		2 << BTE_FEXT(bte_reg->BTE_HL_HWINFO, CSL_BTE_BTE_HL_HWINFO_CONTEXTS_SHIFT, CSL_BTE_BTE_HL_HWINFO_CONTEXTS_MASK);

	gIss_captCommonObj.handleBTE.bwLimiter = BTE_BWL;
	BTE_SET32(bte_reg->BTE_CTRL, gIss_captCommonObj.handleBTE.bwLimiter, CSL_BTE_BTE_CTRL_BW_LIMITER_SHIFT, CSL_BTE_BTE_CTRL_BW_LIMITER_MASK);
	BTE_SET32(bte_reg->BTE_CTRL, 1, CSL_BTE_BTE_CTRL_POSTED_SHIFT, CSL_BTE_BTE_CTRL_POSTED_MASK);

	for (i = 0; i < gIss_captCommonObj.handleBTE.contextNumber; i++)
	{
		contextHandle = &gIss_captCommonObj.handleBTE.bte_config[i];

		contextHandle->context_num = (BTE_CONTEXT)i;

		contextHandle->context_ctrl.init_sx = 0;
		contextHandle->context_ctrl.init_sy = 0;

		contextHandle->context_ctrl.mode = 0x0;/*write*/
		contextHandle->context_ctrl.one_shot = 0x1;/*one shot*/
		contextHandle->context_ctrl.addr32 = 1;/*TILER*/
		contextHandle->context_ctrl.autoflush = 0;

		contextHandle->context_ctrl.start = 0;
		contextHandle->context_ctrl.stop = 0;
		contextHandle->context_ctrl.flush = 0;
	}
#endif

    if (pConfigSizes != NULL)
    {
        free(pConfigSizes);
    }

    Vps_printf("%s done!\n", __func__);

    return pObj;

exit0:
    if (gIss_captCommonObj.pModuleInstance->hIspHandle != NULL)
    {
        free(gIss_captCommonObj.pModuleInstance->hIspHandle);
    }
    if (pConfigSizes != NULL)
    {
        free(pConfigSizes);
    }
    if (gIss_captCommonObj.pIssConfig != NULL)
    {
        free(gIss_captCommonObj.pIssConfig);
    }

    return NULL;
}

/* ===================================================================
 *  @func     Iss_captDelete
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
Int32 Iss_captDelete(Fdrv_Handle handle, Ptr reserved)
{
    Int32 status = FVID2_EFAIL;
    Iss_CaptObj *pObj = (Iss_CaptObj *) handle;
    MSP_IspCallbackRegCfgT tCallbackRegCfg;
    MSP_ERROR_TYPE eResult = MSP_ERROR_NONE;

    if (handle == (Fdrv_Handle) ISS_CAPT_INST_ALL)
        return FVID2_SOK;
    GT_assert(GT_DEFAULT_MASK, pObj != NULL);

    /*
     * lock driver instance
     */
    Iss_captLock(pObj);
    /*
     * if driver is created or driver is ready for deletion, i.e stopped
     */
    if (pObj->state == ISS_CAPT_STATE_STOPPED
        || pObj->state == ISS_CAPT_STATE_CREATED)
    {

        /*

         *Stop H3A

         */
        #if !(USE_NOT_2A)
        IssAlg_capt2ADeInit((Iss_2AObj*)gIss_captCommonObj.pModuleInstance->p2AObj);
        #endif
        free(gIss_captCommonObj.pModuleInstance->p2AObj);
        /*
         * Unregister & Disable the ISRs
         */
        tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_H3A_END_ISR;
        eResult = MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                           (MSP_INDEXTYPE) MSP_ISP_CFG_UNREG_CALLBACK,
                           &tCallbackRegCfg);
        tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_RSZ_END_ISR;
        eResult =
            MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                           (MSP_INDEXTYPE) MSP_ISP_CFG_UNREG_CALLBACK,
                           &tCallbackRegCfg);
        tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_CCP_LCM;
        eResult =
            MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                           (MSP_INDEXTYPE) MSP_ISS_CFG_UNREG_CALLBACK,
                           &tCallbackRegCfg);
        tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_IPIPE_INT_BSC;
        eResult =
            MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                           (MSP_INDEXTYPE) MSP_ISP_CFG_UNREG_CALLBACK,
                           &tCallbackRegCfg);
        tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_ISIF_2DLSC_ISR;
        eResult =
            MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                           (MSP_INDEXTYPE) MSP_ISP_CFG_UNREG_CALLBACK,
                           &tCallbackRegCfg);
        tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_IPIPE_INT_BSC;
        eResult =
            MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                           (MSP_INDEXTYPE) MSP_ISP_CFG_UNREG_CALLBACK,
                           &tCallbackRegCfg);
        if (MSP_ERROR_NONE != eResult)
        {
            Vps_printf("Error MSP_ISP_config failed!!!\n");
        }
        /* tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_CCP_LCM;
         * tCallbackRegCfg.bEnable = MSP_FALSE;
         * MSP_ISP_config(pModuleInstance->hIspHandle,
         * (MSP_INDEXTYPE)MSP_ISS_CFG_IRQ, &tIspInterruptCfg);
         *
         * tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_ISIF_2DLSC_ISR;
         * tCallbackRegCfg.bEnable = MSP_FALSE;
         * MSP_ISP_config(pModuleInstance->hIspHandle,
         * (MSP_INDEXTYPE)MSP_ISP_CFG_INT, &tIspInterruptCfg);
         *
         * tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_H3A_END_ISR;
         * tCallbackRegCfg.bEnable = MSP_FALSE;
         * MSP_ISP_config(pModuleInstance->hIspHandle,
         * (MSP_INDEXTYPE)MSP_ISP_CFG_INT, &tIspInterruptCfg); */
        /*
         * stop Isp processing
         */
        MSP_ISP_control(gIss_captCommonObj.pModuleInstance->hIspHandle, MSP_CTRLCMD_STOP, NULL);


		/* Soft resetting ISS */
		ipipe_config_boxcar_addr(NULL);
		Iss_ispWriteReg(&iss_regs->ISS_HL_SYSCONFIG,1,0,1);
		Iss_reInit(NULL);

        /*
         * delete semaphores
         */
        Semaphore_delete(&pObj->semStopDone);
        Semaphore_delete(&pObj->semStartDone);
        /*
         * delete VP handles associated with this driver instance
         */
        // Iss_captDeleteVp ( pObj );
        /*
         * delete queues accosciated with this driver instance
         */
        Iss_captDeleteQueues(pObj);
        /*
         * free path associated with this driver
         */
        Iss_captFreePath(pObj);

		((MSP_COMPONENT_TYPE*)gIss_captCommonObj.pModuleInstance->hIspHandle)->deInit((MSP_HANDLE)gIss_captCommonObj.pModuleInstance->hIspHandle);
        /*
         * free buffer allocations
         */
        free((void *)gIss_captCommonObj.pIssConfig->ptLsc2D->nGainTableAddress);
        free((void *)gIss_captCommonObj.pIssConfig->ptLsc2D->nOffsetTableAddress);
        free(gIss_captCommonObj.pModuleInstance->hIspHandle);
        free(gIss_captCommonObj.pModuleInstance);
        free(gIss_captCommonObj.pIssConfig);

        /*
         * mark state as idle
         */
        pObj->state = ISS_CAPT_STATE_IDLE;
    }

    /*
     * unlock driver instance
     */
    Iss_captUnlock(pObj);
    IssAlg_captDeInit();

	#ifdef SYS_DEBUG
	Vps_printf("%s done!\n", __func__);
	#endif

    return status;
}
/* ===================================================================
 *  @func     Iss_captQueue
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
Int32 Iss_captQueue(Fdrv_Handle handle,
                    FVID2_FrameList * frameList, UInt32 streamId)
{
    Iss_CaptObj *pObj = (Iss_CaptObj *) handle;
    Iss_CaptChObj *pChObj;
    Int32 status = FVID2_SOK, queStatus;
    FVID2_Frame *pFrame;
    UInt16 frameId, chId, instId, lChannelNum, drvStreamId;
    /*
     * if global handle then make handle as NULL, this indicates
     * we need to check over all driver instances
     */
    if (handle == (Fdrv_Handle) ISS_CAPT_INST_ALL)
        handle = NULL;
    /*
     * parameter checking
     */
    if (frameList == NULL || frameList->numFrames == 0
        || frameList->numFrames > FVID2_MAX_FVID_FRAME_PTR)
    {
        return FVID2_EFAIL;
    }
    /*
     * for all frames that need to be queued
     */
    for (frameId = 0; frameId < frameList->numFrames; frameId++)
    {
        /*
         * get FVID2 frame pointer
         */
        pFrame = frameList->frames[frameId];
        if (pFrame == NULL)
        {
            /*
             * invalid FVID2 frame ID
             */
            status = FVID2_EFAIL;
            continue;
        }
        /*
         * map user channel number to driver channel number
         */
        lChannelNum = gIss_captCommonObj.fvidChannelNum2lChannelNumMap
            [pFrame->channelNum];
        /*
         * extract driver instance ID from driver channel number
         */
        instId = Iss_captGetInstId(lChannelNum);
        if (instId >= ISS_CAPT_INST_MAX)
        {
            /*
             * invalid instance ID
             */
            status = FVID2_EFAIL;
            continue;
        }
        if (handle == NULL)
        {
            /*
             * if global handle mode, the get instance handle
             * from driver channel number extract instance ID
             */
            pObj = &gIss_captCommonObj.captureObj[instId];
        }
        if (instId != pObj->instanceId)
        {
            /*
             * if mismatch then channelNum in FVID2 Frame is not correct,
             * skip this frame que
             */
            status = FVID2_EFAIL;
            continue;
        }
        if (pObj->state == ISS_CAPT_STATE_IDLE)
        {
            /*
             * if driver handle is not open then skip this frame que
             */
            status = FVID2_EFAIL;
            continue;
        }
        if (streamId == ISS_CAPT_STREAM_ID_ANY)
            drvStreamId = Iss_captGetStreamId(lChannelNum);
        else
            drvStreamId = streamId;
        if (drvStreamId >= pObj->numStream
            || drvStreamId != Iss_captGetStreamId(lChannelNum))
        {
            /*
             * invalid stream ID skip this frame que
             */
            status = FVID2_EFAIL;
            continue;
        }
        /*
         * get channel ID
         */
        chId = Iss_captGetChId(lChannelNum);
        if (chId >= pObj->numCh)
        {
            /*
             * invalid channel ID skip this frame que
             */
            status = FVID2_EFAIL;
            continue;
        }
        /*
         * valid instance, stream and channel
         */
        /*
         * get channel specific object in the required instance
         */
        pChObj = &pObj->chObj[drvStreamId][chId];
        /*
         * que the frame to the channel empty frame que
         */
        queStatus = VpsUtils_quePut(&pChObj->emptyQue, pFrame, BIOS_NO_WAIT);
        /*
         * this assert should never happen
         */
        GT_assert(GT_DEFAULT_MASK, queStatus == FVID2_SOK);
        /* Mark frame in frameList as NULL */
        frameList->frames[frameId] = NULL;
    }
    return status;
}
/* ===================================================================
 *  @func     Iss_captDequeue
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
Int32 Iss_captDequeue(Fdrv_Handle handle,
                      FVID2_FrameList * frameList, UInt32 streamId,
                      UInt32 timeout)
{
    Iss_CaptObj *pObj = (Iss_CaptObj *) handle;
    Int32 status = FVID2_SOK, dequeStatus, instId;
    Ptr pData;
    /*
     * if global handle then make handle as NULL, this indicates
     * we need to check over all driver instances
     */
    if (handle == (Fdrv_Handle) ISS_CAPT_INST_ALL)
        handle = NULL;
    /*
     * parameter checking
     */
    if (frameList == NULL || streamId >= ISS_CAPT_STREAM_ID_MAX)
    {
        return FVID2_EFAIL;
    }
    /*
     * init frame list fields
     */
    frameList->numFrames = 0;
    frameList->perListCfg = NULL;
    frameList->drvData = NULL;
    frameList->reserved = NULL;
    /*
     * for every instance
     */
    for (instId = 0; instId < ISS_CAPT_INST_MAX; instId++)
    {
        if (handle == NULL)
        {
            /*
             * if handle is NULL, i.e global handle, then get handle using 'instId',
             * else use user supplied handle
             */
            pObj = &gIss_captCommonObj.captureObj[instId];
        }
        /*
         * check if driver instance is not idle, i.e driver is opened
         */
        if (pObj->state != ISS_CAPT_STATE_IDLE)
        {
            /*
             * validate stream ID
             */
            if (streamId >= pObj->numStream)
            {
                status = FVID2_EFAIL;
            }
            if (status == FVID2_SOK)
            {
                /*
                 * deque for current handle
                 */
                do
                {
                    /*
                     * deque from handle, could be blocking for the first time
                     * based on 'timeout'
                     */
                    dequeStatus = VpsUtils_queGet(&pObj->fullQue[streamId],
                                                  &pData, 1, timeout);
                    if (dequeStatus == FVID2_SOK)
                    {
                        /*
                         * deque is successful, add to frame list
                         */
                        frameList->frames[frameList->numFrames] =
                            (FVID2_Frame *) pData;
                        frameList->numFrames++;
                    }
                    /*
                     * second iteration onwards do non-blocking deque
                     */
                    timeout = BIOS_NO_WAIT;
                    /*
                     * Max frames limit exceeded exit
                     */
                    if (frameList->numFrames >= FVID2_MAX_FVID_FRAME_PTR)
                        break;
                }
                while (dequeStatus == FVID2_SOK);
                /*
                 * all frames deque from current driver handle
                 */
            }
        }
        if (handle != NULL)
            break;                                         /* if not global
                                                            * handle, then we
                                                            * are done, so
                                                            * exit */
        /*
         * Max frames limit exceeded exit
         */
        if (frameList->numFrames >= FVID2_MAX_FVID_FRAME_PTR)
            break;
        /*
         * global handle mode, so check other driver instances also
         */
    }

    return status;
}
/* ===================================================================
 *  @func     Iss_captControl
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
Int32 Iss_captControl(Fdrv_Handle handle, UInt32 cmd,
                      Ptr cmdArgs, Ptr cmdStatusArgs)
{
    Int32 status;
    UInt32 value = 0;
    Iss_CaptObj *pObj = &gIss_captCommonObj.captureObj[0];
    Iss_2AObj *p2AObj;
    p2AObj = (Iss_2AObj*)gIss_captCommonObj.pModuleInstance->p2AObj;

    /*
     * switch to correct CMD handler
     */
    switch (cmd)
    {
        case FVID2_START:
            status = Iss_captStart(handle);
            break;
        case FVID2_STOP:
            status = Iss_captStop(handle);
            break;
		case IOCTL_ISS_CAPT_RESET_AND_RESTART:
			status = Iss_captResetAndRestart(cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_FRAME_SKIP:
            status = Iss_captSetFrameSkip(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_MIRROR_MODE:
            status = Iss_captSetMirrorMode(handle, cmdArgs);
            break;
        case IOCTL_ISS_ALG_2A_UPDATE:
            status = Iss_capt2AUpdate(handle);
            break;
        case IOCTL_ISS_CAPT_SET_RESOLUTION:
            status = Iss_captSetResolution(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_CROP:
            status = Iss_captSetCrop(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_FRAMERATE:
            status = Iss_captSetFrameRate(handle);
            status = IssAlg_capt2ASetFrameRate(handle, cmdArgs);
            break;
        case IOCTL_ISS_ALG_ITT_CONTROL:
            status = Iss_captITTControl(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_COLOR:
            status = IssAlg_capt2ASetColor(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_AEWBVENDOR:
            status = IssAlg_capt2ASetAEWBVendor(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_AEWBMODE:
            status = IssAlg_capt2ASetAEWBMode(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_IRIS:
            status = IssAlg_capt2ASetIris(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_AEWBPRI:
            status = IssAlg_capt2ASetAEWBPri(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_SHARPNESS:
            status = IssAlg_capt2ASetSharpness(handle, cmdArgs);
            break;
		case IOCTL_ISS_CAPT_SET_BRIGHTNESS:
			status = IssAlg_capt2ASetBrightness(handle, cmdArgs);
			break;
		case IOCTL_ISS_CAPT_SET_CONTRAST:
			status = IssAlg_capt2ASetContrast(handle, cmdArgs);
			break;
		case IOCTL_ISS_CAPT_SET_SATURATION:
			status = IssAlg_capt2ASetSaturation(handle, cmdArgs);
			break;
        case IOCTL_ISS_CAPT_SET_BLC:
            status = IssAlg_capt2ASetBlc(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_AWBMODE:
            status = IssAlg_capt2ASetAwbMode(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_AEMODE:
            status = IssAlg_capt2ASetAeMode(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_ENV:
            status = IssAlg_capt2ASetEnv(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_BINNING:
            status = IssAlg_capt2ASetBinning(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_SET_DCCPRM:
            status = IssAlg_capt2ASetDccPrm(handle, cmdArgs);
            break;
        case IOCTL_ISS_CAPT_GET_HISTADDR:
			*((UInt32*)cmdArgs) = (UInt32)pObj->histData;
			status = FVID2_SOK;
			break;
		case IOCTL_ISS_CAPT_GET_AFDATA:
			*((UInt32*)cmdArgs) = (UInt32)p2AObj->AFValue;
			status = FVID2_SOK;
			break;
		case IOCTL_ISS_CAPT_GET_AEWBDATA:
			value = (1 << 10 ); value -= 1;
			p2AObj->AEWBValue1 &= (UInt32)value; //low 10 bit
			value = (1 << 20 ); value -= 1;
			p2AObj->AEWBValue2 &= (UInt32)value; //high 20 bit
			value = (UInt32)p2AObj->AEWBValue1 + ( (UInt32)p2AObj->AEWBValue2 << 10 ) ;
			*((UInt32*)cmdArgs) = (UInt32) value;
			status = FVID2_SOK;
			break;
		case IOCTL_ISS_CAPT_GET_EXPOSURE:
		{
			Iss_ExposureData *data = (Iss_ExposureData *)cmdArgs;

			data->agc = ti2a_output_params.sensorGain;
			data->shutter = ti2a_output_params.sensorExposure;
			data->ae_err = 0;

			Vps_printf("2A agc %d, shutter %d, ae_err %d\n", data->agc, data->shutter, data->ae_err);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_ALG_VERSION:
		{
			UInt32 *alg_ver = (UInt32 *)cmdArgs;

			//# isp version
			UD_IssAlg_capt2AGetAlgVersion(alg_ver);

			status = FVID2_SOK;
			break;
		}
#ifdef UDISP_TOOL
		case IOCTL_ISS_CAPT_SET_BALCKCLAMP:
		{
			Iss_Clamp *clamp = (Iss_Clamp *)cmdArgs;

			UD_IssAlg_capt2ASetBlackClamp(clamp);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_BALCKCLAMP:
		{
			Iss_Clamp *clamp = (Iss_Clamp *)cmdArgs;

			UD_IssAlg_capt2AGetBlackClamp(clamp);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_SET_ISIFWB:
		{
			Iss_isifWb *isifWb = (Iss_isifWb *)cmdArgs;

			UD_IssAlg_capt2ASetIsifWb(isifWb);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_ISIFWB:
		{
			Iss_isifWb *isifWb = (Iss_isifWb *)cmdArgs;

			UD_IssAlg_capt2AGetIsifWb(isifWb);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_SET_NOISEFILTER1:
		{
			Iss_noiseFilter *nFilter = (Iss_noiseFilter *)cmdArgs;

			UD_IssAlg_capt2ASetNoiseFilter(nFilter);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_NOISEFILTER1:
		{
			Iss_noiseFilter *nFilter = (Iss_noiseFilter *)cmdArgs;

			UD_IssAlg_capt2AGetNoiseFilter(nFilter);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_SET_NOISEFILTER2:
		{
			Iss_noiseFilter *nFilter = (Iss_noiseFilter *)cmdArgs;

			UD_IssAlg_capt2ASetNoiseFilter2(nFilter);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_NOISEFILTER2:
		{
			Iss_noiseFilter *nFilter = (Iss_noiseFilter *)cmdArgs;

			UD_IssAlg_capt2AGetNoiseFilter2(nFilter);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_SET_EDGEENHANCER:
		{
			Iss_edgeEnhancer *eE = (Iss_edgeEnhancer *)cmdArgs;

			UD_IssAlg_capt2ASetEdgeEnhancer(eE);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_EDGEENHANCER:
		{
			Iss_edgeEnhancer *eE = (Iss_edgeEnhancer *)cmdArgs;

			UD_IssAlg_capt2AGetEdgeEnhancer(eE);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_SET_RGB2RGB1:
		{
			Iss_rgb2rgb *rgb2rgb = (Iss_rgb2rgb *)cmdArgs;

			UD_IssAlg_capt2ASetRgb2rgb(rgb2rgb);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_RGB2RGB1:
		{
			Iss_rgb2rgb *rgb2rgb = (Iss_rgb2rgb *)cmdArgs;

			UD_IssAlg_capt2AGetRgb2rgb(rgb2rgb);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_SET_RGB2RGB2:
		{
			Iss_rgb2rgb *rgb2rgb = (Iss_rgb2rgb *)cmdArgs;

			UD_IssAlg_capt2ASetRgb2rgb2(rgb2rgb);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_RGB2RGB2:
		{
			Iss_rgb2rgb *rgb2rgb = (Iss_rgb2rgb *)cmdArgs;

			UD_IssAlg_capt2AGetRgb2rgb2(rgb2rgb);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_SET_RGB2YCBCR:
		{
			Iss_rgb2yuv *rgb2yuv = (Iss_rgb2yuv *)cmdArgs;

			UD_IssAlg_capt2ASetRgb2ycbcr(rgb2yuv);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_RGB2YCBCR:
		{
			Iss_rgb2yuv *rgb2yuv = (Iss_rgb2yuv *)cmdArgs;

			UD_IssAlg_capt2AGetRgb2ycbcr(rgb2yuv);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_SET_IPIPEWB:
		{
			Iss_ipipeWb *ipipeWb = (Iss_ipipeWb *)cmdArgs;

			UD_IssAlg_capt2ASetIpipeWb(ipipeWb);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_IPIPEWB:
		{
			Iss_ipipeWb *ipipeWb = (Iss_ipipeWb *)cmdArgs;

			UD_IssAlg_capt2AGetIpipeWb(ipipeWb);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_SET_GBCE:
		{
			Iss_gbce *gbce = (Iss_gbce *)cmdArgs;

			UD_IssAlg_capt2ASetGbce(gbce);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_GBCE:
		{
			Iss_gbce *gbce = (Iss_gbce *)cmdArgs;

			UD_IssAlg_capt2AGetGbce(gbce);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_SET_GAMMA:
		{
			Iss_gamma *gamma = (Iss_Clamp *)cmdArgs;

			UD_IssAlg_capt2ASetGamma(gamma);

			status = FVID2_SOK;
			break;
		}
		case IOCTL_ISS_CAPT_GET_GAMMA:
		{
			Iss_gamma *gamma = (Iss_gamma *)cmdArgs;

			UD_IssAlg_capt2AGetGamma(gamma);

			status = FVID2_SOK;
			break;
		}
#endif //# UDISP_TOOL
        default:
            /*
             * illegal IOCTL
             */
            status = 0;
            break;
    }
    return status;
}
/*
 * Starts a drvier instance */
/* ===================================================================
 *  @func     Iss_captStart
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
Int32 Iss_captStart(Fdrv_Handle handle)
{
    Iss_CaptObj *pObj = (Iss_CaptObj *) handle;
	Iss_CaptRtParams *pRtParams = NULL;
    Int32 status = FVID2_EFAIL;
    MSP_IspMemToMemCmdParamT tIspMemToMemCmdParam;
    MSP_IspProcessParamT tIspProcessParam;
    MSP_IspQueryH3aStateT tQueryH3aState;
    MSP_IspInterruptCfgT tIspInterruptCfg;
    MSP_ERROR_TYPE eResult = MSP_ERROR_NONE;
#ifdef ENABLE_BTE
	UInt32 oriBufOffsetY, oriBufOffsetC;
#endif
    Iss_CaptChObj *pChObj;
    /*
     * check if global handle, this operation is not supportted
     * for global handles
     */
    if (handle == (Fdrv_Handle) ISS_CAPT_INST_ALL)
        return FVID2_EFAIL;
    GT_assert(GT_DEFAULT_MASK, pObj != NULL);

    /*
     * lock driver instance
     */
    Iss_captLock(pObj);
    /* Get Buffers from Empty Queue to register it to Resizer Driver */
    pChObj = &pObj->chObj[0][0];
    status =
        VpsUtils_queGet(&pChObj->emptyQue, (Ptr *) & PrimaryFrame_RszA, 1,
                        BIOS_NO_WAIT);
	if(pObj->numStream == 2)
	{
		pChObj = &pObj->chObj[1][0];
		status =
			VpsUtils_queGet(&pChObj->emptyQue, (Ptr *) & PrimaryFrame_RszB, 1,
							BIOS_NO_WAIT);
	}
	pRtParams = (Iss_CaptRtParams *)PrimaryFrame_RszA->perFrameCfg;

	/* udpate width x height in run time per frame config */
	pRtParams->captureOutWidth = pObj->createArgs.scParams[0].outWidth;
	pRtParams->captureOutHeight = pObj->createArgs.scParams[0].outHeight;
	pRtParams->captureOutPitch = pObj->createArgs.pitch[0];

	if(pObj->numStream == 2)
	{
		pRtParams = (Iss_CaptRtParams *)PrimaryFrame_RszB->perFrameCfg;
		/* udpate width x height in run time per frame config */
		pRtParams->captureOutWidth = pObj->createArgs.scParams[1].outWidth;
		pRtParams->captureOutHeight = pObj->createArgs.scParams[1].outHeight;
		pRtParams->captureOutPitch = pObj->createArgs.pitch[1];
	}

#ifdef ENABLE_BTE
	gIss_captCommonObj.handleBTE.bte_config[0].context_start_addr = 0;
	if (pObj->createArgs.outStreamInfo[0].dataFormat != FVID2_DF_YUV422I_VYUY)
		gIss_captCommonObj.handleBTE.bte_config[1].context_start_addr =
			(DIV_UP(pObj->createArgs.scParams[0].outWidth, 128) << CSL_BTE_BTE_CONTEXT_START_0_X_SHIFT);

	configureBteContext(&gIss_captCommonObj.handleBTE, 0,
		pObj->createArgs.scParams[0].outWidth, pObj->createArgs.scParams[0].outHeight,
		pObj->createArgs.outStreamInfo[0].dataFormat, curStreamBuf[0].mirrorMode);

    oriBufOffsetY = Utils_tilerGetOriAddr((Int32)PrimaryFrame_RszA->addr[1][0], 0, curStreamBuf[0].mirrorMode,
		pObj->createArgs.scParams[0].outWidth, pObj->createArgs.scParams[0].outHeight);
	startBteContext(&gIss_captCommonObj.handleBTE.bte_config[0], oriBufOffsetY);

	if (pObj->createArgs.outStreamInfo[0].dataFormat != FVID2_DF_YUV422I_VYUY)
	{
		oriBufOffsetC = Utils_tilerGetOriAddr((Int32)PrimaryFrame_RszA->addr[1][1], 1, curStreamBuf[0].mirrorMode,
			pObj->createArgs.scParams[0].outWidth, pObj->createArgs.scParams[0].outHeight/2);
		startBteContext(&gIss_captCommonObj.handleBTE.bte_config[1], oriBufOffsetC);
	}
#endif

    if (pObj->state == ISS_CAPT_STATE_CREATED
        || pObj->state == ISS_CAPT_STATE_STOPPED)
    {
        gIss_captCommonObj.pIssConfig->eFH3aValidity = MSP_PROC_H3A_VALID_NO;
        gIss_captCommonObj.pModuleInstance->tConfigParams.ptIssConfig = gIss_captCommonObj.pIssConfig;
        EXPECT_EVENT(gIss_captCommonObj.pModuleInstance, VIDEO_ISR_EVENT_RSZ);
#ifndef ENABLE_BTE
        tIspMemToMemCmdParam.pOutbuffRszA = PrimaryFrame_RszA->addr[1][0];

        tIspMemToMemCmdParam.pOutbuffRszACr = PrimaryFrame_RszA->addr[1][1];
#else
        tIspMemToMemCmdParam.pOutbuffRszA = (void *)(gIss_captCommonObj.handleBTE.baseAddress +
        	gIss_captCommonObj.handleBTE.bte_config[0].context_start_addr);

        tIspMemToMemCmdParam.pOutbuffRszACr = (void *)(gIss_captCommonObj.handleBTE.baseAddress +
        	gIss_captCommonObj.handleBTE.bte_config[1].context_start_addr);
#endif
		curStreamBuf[0].ptmpFrame = PrimaryFrame_RszA;
		if(pObj->numStream == 2)
		{
			tIspMemToMemCmdParam.pOutbuffRszB = PrimaryFrame_RszB->addr[1][0];
			tIspMemToMemCmdParam.pOutbuffRszBCr = PrimaryFrame_RszB->addr[1][1];
			curStreamBuf[1].ptmpFrame = PrimaryFrame_RszB;
		}
		else
		if(pObj->numStream == 1)
		{
			tIspMemToMemCmdParam.pOutbuffRszB = NULL;
			tIspMemToMemCmdParam.pOutbuffRszBCr = NULL;
			curStreamBuf[1].ptmpFrame = NULL;
		}
		if(pObj->glbcEnable == 1)
			ipipe_config_boxcar_addr((UInt32)PrimaryFrame_RszA->blankData);

        tIspMemToMemCmdParam.eInFormat = MSP_IPIPE_IN_FORMAT_BAYER;
        tIspMemToMemCmdParam.pOutRawBuffer = (void *) NULL;
        tIspMemToMemCmdParam.pOutRawBuffer = NULL;
        tIspProcessParam.eCmd = MSP_ISP_CMD_MEM_TO_MEM_START;
        tIspProcessParam.pCmdData = &tIspMemToMemCmdParam;
		if(pObj->createArgs.captureMode == ISS_CAPT_INMODE_ISIF)
		{
		    eResult =
                MSP_ISP_process(gIss_captCommonObj.pModuleInstance->hIspHandle, &tIspProcessParam,
                                NULL);
            if (MSP_ERROR_NONE == eResult)
            {
                // TODO TIMM_OSAL_SemaphoreObtain(pModuleInstance->pIssSem,
                // TIMM_OSAL_SUSPEND);
            }
		}
		else
		{
		    // ISS_CAPT_INMODE_DDR mode
			Iss_ispWriteReg(&isif_reg->CADU,(((UInt32)PrimaryFrame_RszA->addr[1][0] >> 21) & 0x7FF),0,11);
			Iss_ispWriteReg(&isif_reg->CADL,(((UInt32)PrimaryFrame_RszA->addr[1][0] >> 5) & 0xFFFF),0,16);
			isif_start(ISIF_START_WRITE);
		}
        tQueryH3aState.eH3aState = MSP_H3A_STOPPED;
        issDrvH3aStart();
        MSP_ISP_query(gIss_captCommonObj.pModuleInstance->hIspHandle,
                      (MSP_INDEXTYPE) MSP_ISP_QUERY_H3A_STATE, &tQueryH3aState);
        if (tQueryH3aState.eH3aState & (MSP_H3A_AEWB_ENABLED))
        {
            EXPECT_EVENT(gIss_captCommonObj.pModuleInstance, VIDEO_ISR_EVENT_H3A);
            tIspInterruptCfg.eInterruptId = MSP_ISS_DRV_H3A_END_ISR;
            tIspInterruptCfg.bEnable = MSP_TRUE;
            MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                           (MSP_INDEXTYPE) MSP_ISP_CFG_INT,
                           (MSP_PTR) & tIspInterruptCfg);
        }
    }
    isif_reg->HSIZE = pObj->createArgs.scParams[0].inWidth/16;//Width*2/32

	//Iss_capRegisterDisplay();

    /*
     * unlock driver instance
     */
    Iss_captUnlock(pObj);

	IssAlg_capt2Astart();	//# jaeki - 2A hang issue

    return status;
}

extern isp_regs_ovly isp_regs;
extern ipipeif_regs_ovly ipipeif_reg;

void Iss_capRegisterDisplay(void)
{
    /* Test Code for Register Dump */
    int i;
    UInt32 regVal[4];
    UInt32 *pReg;
    UInt32 tempAddr;

    Vps_rprintf("[ISP5 Reg]\n");
    pReg = (UInt32*)isp_regs;
    for(i = 0; i <= sizeof(CSL_ISP5Regs)/16; i++)
    {
        tempAddr = (UInt32)pReg;
        regVal[0] = *pReg++;
        regVal[1] = *pReg++;
        regVal[2] = *pReg++;
        regVal[3] = *pReg++;
        Vps_rprintf("%08x : %08x %08x %08x %08x ", tempAddr, regVal[0], regVal[1], regVal[2], regVal[3]);
    }

    Vps_rprintf("[ISIF Reg]\n");
    pReg = (UInt32*)isif_reg;
    for(i = 0; i <= sizeof(CSL_IsifRegs)/16; i++)
    {
        tempAddr = (UInt32)pReg;
        regVal[0] = *pReg++;
        regVal[1] = *pReg++;
        regVal[2] = *pReg++;
        regVal[3] = *pReg++;
        Vps_rprintf("%08x : %08x %08x %08x %08x ", tempAddr, regVal[0], regVal[1], regVal[2], regVal[3]);
    }

    Vps_rprintf("[IPIPEIF Reg]\n");
    pReg = (UInt32*)ipipeif_reg;
    for(i = 0; i <= sizeof(CSL_IpipeifRegs)/16; i++)
    {
        tempAddr = (UInt32)pReg;
        regVal[0] = *pReg++;
        regVal[1] = *pReg++;
        regVal[2] = *pReg++;
        regVal[3] = *pReg++;
        Vps_rprintf("%08x : %08x %08x %08x %08x ", tempAddr, regVal[0], regVal[1], regVal[2], regVal[3]);
    }

    Vps_rprintf("[IPIPE Reg]\n");
    pReg = (UInt32*)ipipe_reg;
    for(i = 0; i <= sizeof(CSL_IpipeRegs)/16; i++)
    {
        tempAddr = (UInt32)pReg;
        regVal[0] = *pReg++;
        regVal[1] = *pReg++;
        regVal[2] = *pReg++;
        regVal[3] = *pReg++;
        Vps_rprintf("%08x : %08x %08x %08x %08x ", tempAddr, regVal[0], regVal[1], regVal[2], regVal[3]);
    }

    Vps_rprintf("[Resize Reg]\n");
    pReg = (UInt32*)rsz_reg;
    for(i = 0; i <= sizeof(Rsz_Regs)/16; i++)
    {
        tempAddr = (UInt32)pReg;
        regVal[0] = *pReg++;
        regVal[1] = *pReg++;
        regVal[2] = *pReg++;
        regVal[3] = *pReg++;
        Vps_rprintf("%08x : %08x %08x %08x %08x ", tempAddr, regVal[0], regVal[1], regVal[2], regVal[3]);
    }
}


/*
 * Stops a driver instance */
/* ===================================================================
 *  @func     Iss_captStop
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
Int32 Iss_captStop(Fdrv_Handle handle)
{
    Iss_CaptObj *pObj = (Iss_CaptObj *) handle;
    Int32 status = FVID2_EFAIL;

	IssAlg_capt2Astop();	//# 2A hang issue
    /*
     * check if global handle, this operation is not supportted
     * for global handles
     */
    if (handle == (Fdrv_Handle) ISS_CAPT_INST_ALL)
        return FVID2_SOK;
    GT_assert(GT_DEFAULT_MASK, pObj != NULL);
    /*
     * lock driver instance
     */
    Iss_captLock(pObj);
    if (pObj->state == ISS_CAPT_STATE_RUNNING)
    {
        /*
         * stop, only if driver is running
         */
        // status = Iss_captStopVP ( pObj );
    }
    /*
     * unlock driver instance
     */
    Iss_captUnlock(pObj);

    return status;
}
/* ===================================================================
 *  @func     Iss_captFreePath
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
Int32 Iss_captFreePath(Iss_CaptObj * pObj)
{
    /*
     * free VP resources used in this driver path
     */
    return 0;
}
/* ===================================================================
 *  @func     Iss_captAllocPath
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
Int32 Iss_captAllocPath(Iss_CaptObj * pObj)
{
    Int32 status = FVID2_SOK;
    return status;
}
/*
 * Create and init channel specific driver objects */
/* ===================================================================
 *  @func     Iss_captCreateChObj
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
Int32 Iss_captCreateChObj(Iss_CaptObj * pObj)
{
    UInt16 streamId, chId;
    Iss_CaptChObj *pChObj;
    Int32 status;
    pObj->resetStatistics = TRUE;
    pObj->addMultiChDummyDesc = FALSE;
    /*
     * get channel properties like num channels,
     * in width x in height, scan format - interlaced or progressive
     */
    pObj->numCh = pObj->createArgs.numCh;
    status = Iss_captAllocPath(pObj);
    if (status != FVID2_SOK)
        return status;
    /*
     * for every stream and ever channel do ..
     */
    for (streamId = 0; streamId < pObj->numStream; streamId++)
    {
        for (chId = 0; chId < pObj->numCh; chId++)
        {
            /*
             * get channel object
             */
            pChObj = &pObj->chObj[streamId][chId];
            /*
             * mark VPDMA received channels to 0
             */
            pChObj->vChannelRecv = 0;
            /*
             * mark memory type as non-tiled by default
             */
            pChObj->memType = ISS_NONTILEDMEM;
            /*
             * make driver channel number from instance ID, stream ID,
             * thus given driver channel number we can know which
             * instance, stream, channel it belongs to
             * chID
             */
            pChObj->lChannelNum =
                Iss_captMakeChannelNum(pObj->instanceId, streamId, chId);
            /*
             * copy user channel number to FVID2 Format structure
             */
            pChObj->channelNum = pObj->createArgs.channelNumMap[streamId][chId];
            /*
             * make user channel number to driver channel number mapping
             */
            gIss_captCommonObj.
                fvidChannelNum2lChannelNumMap[pChObj->channelNum] =
                pChObj->lChannelNum;
            /*
             * fill FVID2 Format with data format
             */
            pChObj->dataFormat =
                pObj->createArgs.outStreamInfo[streamId].dataFormat;
            /*
             * set memory type for data format's which could support tiled mode
             */
            if (pChObj->dataFormat == FVID2_DF_YUV420SP_UV
                || pChObj->dataFormat == FVID2_DF_YUV422SP_UV)
            {
                pChObj->memType
                    = pObj->createArgs.outStreamInfo[streamId].memType;
            }
            /* invert FID for YUV420 and in discrete sync mode */
            pChObj->invertFid = FALSE;
#if 0
            if (pChObj->dataFormat == FVID2_DF_YUV420SP_UV
                &&
                pObj->createArgs.videoCaptureMode
                == ISS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC)
            {
                pChObj->invertFid = TRUE;
            }
#endif
            pChObj->maxOutHeight
                = pObj->createArgs.outStreamInfo[streamId].maxOutHeight;
            /* unlimited output width */
            pChObj->maxOutWidth = 0;
            /* fill pitch based on user supplied value */
            pChObj->pitch[0]
                = pObj->createArgs.outStreamInfo[streamId].pitch[0];
        }
    }
    return FVID2_SOK;
}
/*
 * Create queues for all channels and streams
 *
 * For each channel and each stream there is one input or free or empty
 * buffer queue
 *
 * Each stream there is one output or full or completed buffer queue
 *
 * Queue depth for empty buffer queue is ISS_CAPT_FRAME_QUE_LEN_PER_CH_MAX
 *
 * Queue depth for full buffer is ISS_CAPT_FRAME_QUE_LEN_PER_CH_MAX*numCh
 *
 * Inaddition to this per channel and stream there is tmp que. This que is
 * for keeping track of buffer submmited to the CLM, i.e buffers which are in
 * the process of getting captured
 *
 * In case or errors in creation, previously created queues are deleted and
 * error is returned */
/* ===================================================================
 *  @func     Iss_captCreateQueues
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
Int32 Iss_captCreateQueues(Iss_CaptObj * pObj)
{
    UInt16 chId, maxElements;
    Int16 streamId;
    Iss_CaptChObj *pChObj;
    Int32 status;
    /*
     * create full que
     */
    for (streamId = 0; streamId < pObj->numStream; streamId++)
    {
        maxElements = pObj->numCh * ISS_CAPT_FRAME_QUE_LEN_PER_CH_MAX;
        status = VpsUtils_queCreate(&pObj->fullQue[streamId],
                                    maxElements,
                                    &pObj->fullQueMem[streamId][0],
                                    VPSUTILS_QUE_FLAG_BLOCK_QUE);
        if (status != FVID2_SOK)
        {
            for (streamId--; streamId >= 0; streamId--)
            {
                status = VpsUtils_queDelete(&pObj->fullQue[streamId]);
                GT_assert(GT_DEFAULT_MASK, status == FVID2_SOK);
            }
            return FVID2_EFAIL;
        }
    }
    /*
     * create empty que and tmp que
     */
    for (streamId = 0; streamId < pObj->numStream; streamId++)
    {
        for (chId = 0; chId < pObj->numCh; chId++)
        {
            pChObj = &pObj->chObj[streamId][chId];
            maxElements = ISS_CAPT_FRAME_QUE_LEN_PER_CH_MAX;
            status = VpsUtils_queCreate(&pChObj->emptyQue,
                                        maxElements,
                                        pChObj->emptyQueMem,
                                        VPSUTILS_QUE_FLAG_NO_BLOCK_QUE);
            GT_assert(GT_DEFAULT_MASK, status == FVID2_SOK);
            Vps_rprintf("Stream ID %d: chId = %d VpsUtils_queCreate.", streamId,
                        chId);
            status =
                VpsUtils_queCreate(&pChObj->tmpQue, maxElements,
                                   pChObj->tmpQueMem,
                                   VPSUTILS_QUE_FLAG_NO_BLOCK_QUE);
            GT_assert(GT_DEFAULT_MASK, status == FVID2_SOK);
        }
    }
    return FVID2_SOK;
}
/*
 * Delete previously created queues */
/* ===================================================================
 *  @func     Iss_captDeleteQueues
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
Int32 Iss_captDeleteQueues(Iss_CaptObj * pObj)
{
    UInt16 streamId, chId;
    Iss_CaptChObj *pChObj;
    Int32 status;
    for (streamId = 0; streamId < pObj->numStream; streamId++)
    {
        status = VpsUtils_queDelete(&pObj->fullQue[streamId]);
        GT_assert(GT_DEFAULT_MASK, status == FVID2_SOK);
        for (chId = 0; chId < pObj->numCh; chId++)
        {
            pChObj = &pObj->chObj[streamId][chId];
            status = VpsUtils_queDelete(&pChObj->emptyQue);
            GT_assert(GT_DEFAULT_MASK, status == FVID2_SOK);
            status = VpsUtils_queDelete(&pChObj->tmpQue);
            GT_assert(GT_DEFAULT_MASK, status == FVID2_SOK);
        }
    }
    return FVID2_SOK;
}
/*
 * Check if data is available in any of the 'full' queues */
/* ===================================================================
 *  @func     Iss_captIsDataAvailable
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
UInt32 Iss_captIsDataAvailable(Iss_CaptObj * pObj)
{
    UInt32 isDataAvailable = FALSE;
    UInt16 streamId;
    /*
     * check is done for all streams
     */
    for (streamId = 0; streamId < pObj->numStream; streamId++)
    {
        if (!VpsUtils_queIsEmpty(&pObj->fullQue[streamId]))
        {
            isDataAvailable = TRUE;
            break;
        }
    }
    return isDataAvailable;
}
/* ===================================================================
 *  @func     Iss_capt2AUpdate
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
Int32 Iss_capt2AUpdate(Iss_CaptObj * pObj)
{
    Int32 status = FVID2_SOK;
    Int32 dGain;
    int rGain;
    int grGain;
    int gbGain;
    int bGain;
    Int32 aewbVendor;
    aewbVendor = IssAlg_capt2AGetAEWBVendor();
    if (aewbVendor == AEWB_ID_NONE)
    {
        return status;
	}
	Iss_captLock(pObj);
    if (ti2a_output_params.mask)
    {
		rGain = ti2a_output_params.ipipe_awb_gain.rGain * 4;
		grGain = ti2a_output_params.ipipe_awb_gain.grGain * 4;
		gbGain = ti2a_output_params.ipipe_awb_gain.gbGain * 4;
		bGain = ti2a_output_params.ipipe_awb_gain.bGain * 4;
		dGain = ti2a_output_params.ipipe_awb_gain.dGain * 2;
        ipipe_reg->WB2_OFT_R = 0;
        ipipe_reg->WB2_OFT_GR = 0;
        ipipe_reg->WB2_OFT_GB = 0;
        ipipe_reg->WB2_OFT_B = 0;
        ipipe_reg->WB2_WGN_R = rGain;
        ipipe_reg->WB2_WGN_GR = grGain;
        ipipe_reg->WB2_WGN_GB = gbGain;
        ipipe_reg->WB2_WGN_B = bGain;

        if(aewbVendor == AEWB_ID_UD)
        {
            ipipe_reg->RGB1_MUL_RR = ti2a_output_params.rgb2rgbparam.rgb_mul_rr;
            ipipe_reg->RGB1_MUL_GR = ti2a_output_params.rgb2rgbparam.rgb_mul_gr;
            ipipe_reg->RGB1_MUL_BR = ti2a_output_params.rgb2rgbparam.rgb_mul_br;
            ipipe_reg->RGB1_MUL_RG = ti2a_output_params.rgb2rgbparam.rgb_mul_rg;
            ipipe_reg->RGB1_MUL_GG = ti2a_output_params.rgb2rgbparam.rgb_mul_gg;
            ipipe_reg->RGB1_MUL_BG = ti2a_output_params.rgb2rgbparam.rgb_mul_bg;
            ipipe_reg->RGB1_MUL_RB = ti2a_output_params.rgb2rgbparam.rgb_mul_rb;
            ipipe_reg->RGB1_MUL_GB = ti2a_output_params.rgb2rgbparam.rgb_mul_gb;
            ipipe_reg->RGB1_MUL_BB = ti2a_output_params.rgb2rgbparam.rgb_mul_bb;
            ipipe_reg->RGB1_OFT_OR = ti2a_output_params.rgb2rgbparam.rgb_oft_or;
            ipipe_reg->RGB1_OFT_OR = ti2a_output_params.rgb2rgbparam.rgb_oft_og;
            ipipe_reg->RGB1_OFT_OR = ti2a_output_params.rgb2rgbparam.rgb_oft_ob;
            ipipe_reg->RGB2_MUL_RR =
                ti2a_output_params.rgb2rgb2param.rgb_mul_rr;
            ipipe_reg->RGB2_MUL_GR =
                ti2a_output_params.rgb2rgb2param.rgb_mul_gr;
            ipipe_reg->RGB2_MUL_BR =
                ti2a_output_params.rgb2rgb2param.rgb_mul_br;
            ipipe_reg->RGB2_MUL_RG =
                ti2a_output_params.rgb2rgb2param.rgb_mul_rg;
            ipipe_reg->RGB2_MUL_GG =
                ti2a_output_params.rgb2rgb2param.rgb_mul_gg;
            ipipe_reg->RGB2_MUL_BG =
                ti2a_output_params.rgb2rgb2param.rgb_mul_bg;
            ipipe_reg->RGB2_MUL_RB =
                ti2a_output_params.rgb2rgb2param.rgb_mul_rb;
            ipipe_reg->RGB2_MUL_GB =
                ti2a_output_params.rgb2rgb2param.rgb_mul_gb;
            ipipe_reg->RGB2_MUL_BB =
                ti2a_output_params.rgb2rgb2param.rgb_mul_bb;
            ipipe_reg->RGB2_OFT_OR = ti2a_output_params.rgb2rgb2param.rgb_oft_or;
            ipipe_reg->RGB2_OFT_OG = ti2a_output_params.rgb2rgb2param.rgb_oft_og;
            ipipe_reg->RGB2_OFT_OB = ti2a_output_params.rgb2rgb2param.rgb_oft_ob;

			isif_reg->CGRGAIN = grGain;
			isif_reg->CGBGAIN = gbGain;
        }

		if (ti2a_output_params.isifMask) {
	        isif_reg->CRGAIN = bGain;
	        isif_reg->CGRGAIN = grGain;
	        isif_reg->CGBGAIN = gbGain;
	        isif_reg->CBGAIN = rGain;
	        isif_reg->COFSTA = 0;
	    }

       	isif_reg->CGAMMAWD = (isif_reg->CGAMMAWD & (~0x7700)) | 0x4400;
    }
	Iss_captUnlock(pObj);

    return status;
}

/* ===================================================================
 *  @func     Iss_captSetFrameRate
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
Int32 Iss_captSetFrameRate(Iss_CaptObj * pObj)
{
    Int32 status = FVID2_SOK;
    return status;
}
/*
 * Set Itt Capture For RAW and YUV Frames
 *
 * RAW and YUV Capture will happen and store in the Buffer. */
/* ===================================================================
 *  @func     Iss_captITTControl
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
Int32 Iss_captITTControl(Iss_CaptObj * pObj, Iss_IttParams * ittParams)
{
    Int32 status = FVID2_EFAIL;
    /*
     * parameter checfing, not supported for global handle
     */
    if (pObj == (Iss_CaptObj *) ISS_CAPT_INST_ALL
        || pObj == NULL || ittParams == NULL)
    {
        return FVID2_EFAIL;
    }
    /*
     * lock driver instance
     */
    // Iss_captLock ( pObj );
    if (pObj->state != ISS_CAPT_STATE_IDLE)
    {
        if (ittParams->ittCommand == 4)
        {
            status = IssCaptureRawFrame(pObj, ittParams);
        }
        else if (ittParams->ittCommand == 9)
        {
            status = IssCaptureYUVFrame(pObj, ittParams);
        }
    }
    /*
     * unlock driver instance
     */
    // Iss_captUnlock ( pObj );
    return status;
}
/* ===================================================================
 *  @func     IssCaptureRawFrame
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
Int32 IssCaptureRawFrame(Iss_CaptObj * pObj, Iss_IttParams * ittParams)
{
    Int32 status = FVID2_SOK;
    gIss_captCommonObj.gIttParams = ittParams;
    Vps_rprintf("\nRAW capture started to address %x", ittParams->isif_dump);
    if (ittParams->isif_dump != NULL)
    {
        gIss_captCommonObj.gIttParams->Raw_Data_Ready = ittParams->Raw_Data_Ready;
        issTriggerRawCapture((Int32 *) ittParams->isif_dump, ISIF_START_WRITE);
        gRawCaptureTriggerd = 1;
    }
    else
    {
        status = FVID2_EFAIL;
    }
    return status;
}
/* ===================================================================
 *  @func     IssCaptureYUVFrame
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
Int32 IssCaptureYUVFrame(Iss_CaptObj * pObj, Iss_IttParams * ittParams)
{
    Int32 status = FVID2_SOK;
    gIss_captCommonObj.gIttParams = ittParams;
    Vps_rprintf("\nYUV captured to address %x", ittParams->isif_dump);
    gIss_captCommonObj.YUVcaptureAddr = (Int32 *) ittParams->isif_dump;
    gYUVCaptureTriggerd = 1;
    return status;
}
/*
 * Set frame skip mask
 *
 * Frame skip itself will happen from next list processing onwards
 *
 * This functionc can get called while the driver is running. i.e run time
 * changing of frame skip mask is supported */
/* ===================================================================
 *  @func     Iss_captSetFrameSkip
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
Int32 Iss_captSetFrameSkip(Iss_CaptObj * pObj, Iss_CaptFrameSkip * frameSkip)
{
    UInt16 lChannelNum, instId, streamId, chId;
    Int32 status = FVID2_EFAIL;
    /*
     * parameter checfing, not supported for global handle
     */
    if (pObj == (Iss_CaptObj *) ISS_CAPT_INST_ALL
        || pObj == NULL || frameSkip == NULL)
    {
        return FVID2_EFAIL;
    }
    /*
     * lock driver instance
     */
    Iss_captLock(pObj);
    if (pObj->state != ISS_CAPT_STATE_IDLE)
    {
        /*
         * driver needs to atleast be in open state for this call to work
         */
        /*
         * map from user channel number of driver channel number
         */
        lChannelNum = gIss_captCommonObj.fvidChannelNum2lChannelNumMap
            [frameSkip->channelNum];
        instId = Iss_captGetInstId(lChannelNum);
        streamId = Iss_captGetStreamId(lChannelNum);
        chId = Iss_captGetChId(lChannelNum);
        if (instId < ISS_CAPT_INST_MAX
            && streamId < pObj->numStream
            && chId < pObj->numCh && instId == pObj->instanceId)
        {
            pObj->chObj[streamId][chId].frameSkipMask =
                frameSkip->frameSkipMask[streamId];
            pObj->chObj[streamId][chId].frameSkipMaskHigh =
                frameSkip->frameSkipMaskHigh[streamId];
            status = FVID2_SOK;
        }
    }
    /*
     * unlock driver instance
     */
    Iss_captUnlock(pObj);
    return status;
}

/* ===================================================================
 *  @func     Iss_captSetMirrorMode
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
Int32 Iss_captSetMirrorMode(Iss_CaptObj * pObj, Int32 * mirrorMode)
{
    Int32 status = FVID2_SOK;
    curStreamBuf[0].mirrorMode = *mirrorMode;
    curStreamBuf[1].mirrorMode = *mirrorMode;
    switch (*mirrorMode)
    {
        case 1:
            curStreamBuf[0].flipH = TRUE;
            curStreamBuf[0].flipV = FALSE;
            curStreamBuf[1].flipH = TRUE;
            curStreamBuf[1].flipV = FALSE;
            break;
        case 2:
            curStreamBuf[0].flipH = FALSE;
            curStreamBuf[0].flipV = TRUE;
            curStreamBuf[1].flipH = FALSE;
            curStreamBuf[1].flipV = TRUE;
            break;
        case 3:
            curStreamBuf[0].flipH = TRUE;
            curStreamBuf[0].flipV = TRUE;
            curStreamBuf[1].flipH = TRUE;
            curStreamBuf[1].flipV = TRUE;
            break;
        case 0:
        default:
            curStreamBuf[0].flipH = FALSE;
            curStreamBuf[0].flipV = FALSE;
            curStreamBuf[1].flipH = FALSE;
            curStreamBuf[1].flipV = FALSE;
            break;
    }
    gMirrorMode = 1;
    return status;
}

/*
 * Set Resolution
 *
 * Frame skip itself will happen from next list processing onwards
 *
 * This functionc can get called while the driver is running. i.e run time
 * changing of frame skip mask is supported */
/* ===================================================================
 *  @func     Iss_captSetResolution
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
Int32 Iss_captSetResolution(Iss_CaptObj * pObj,
                            Iss_CaptResolution * resolutionParams)
{
    Int32 status = FVID2_EFAIL;
	UInt32 captureOutWidth,captureOutHeight;

    /*
     * parameter checfing, not supported for global handle
     */
    if (pObj == (Iss_CaptObj *) ISS_CAPT_INST_ALL
        || pObj == NULL || resolutionParams == NULL)
    {
        return FVID2_EFAIL;
    }

    /*
     * lock driver instance
     */
    Iss_captLock(pObj);
	pObj->createArgs.scParams[resolutionParams->channelNum].outWidth = resolutionParams->ResolutionWidth;
	pObj->createArgs.scParams[resolutionParams->channelNum].outHeight = resolutionParams->ResolutionHeight;

	if(pObj->vsEnable == 0)
	{
		captureOutWidth  = resolutionParams->ResolutionWidth;
		captureOutHeight = resolutionParams->ResolutionHeight;
	}
	else
	{
		captureOutWidth  = (resolutionParams->ResolutionWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
		captureOutHeight = (resolutionParams->ResolutionHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
	}

	pObj->createArgs.pitch[resolutionParams->channelNum] = captureOutWidth;

	if(resolutionParams->channelNum == 1)
		status = rszBSetOutConfig(pObj->createArgs.scParams[0].inWidth - 2,
								  pObj->createArgs.scParams[0].inHeight - 4,
								  captureOutWidth,
								  captureOutHeight);
	else if(resolutionParams->channelNum == 0)
					/* Set the resolution params even if RSZ B is going to be disabled */
		status = rszASetOutConfig(pObj->createArgs.scParams[0].inWidth - 2,
								  pObj->createArgs.scParams[0].inHeight - 4,
								  captureOutWidth,
								  captureOutHeight);
    /*
     * unlock driver instance
     */
    Iss_captUnlock(pObj);
    return status;
}

Int32 Iss_captSetCrop(Iss_CaptObj *pObj, Iss_CaptResolution *cropParams)
{
    Int32 status = FVID2_EFAIL;

    /* parameter checfing, not supported for global handle */
    if (pObj == (Iss_CaptObj *)ISS_CAPT_INST_ALL || pObj == NULL || cropParams == NULL) {
        return FVID2_EFAIL;
    }

    /* lock driver instance */
    Iss_captLock(pObj);

    //# current support only rszB
    pObj->createArgs.scParams[1].inCropCfg.cropStartX = cropParams->CropStartX;
	pObj->createArgs.scParams[1].inCropCfg.cropStartY = cropParams->CropStartY;
	pObj->createArgs.scParams[1].inCropCfg.cropWidth = cropParams->ResolutionWidth;
	pObj->createArgs.scParams[1].inCropCfg.cropHeight = cropParams->ResolutionHeight;

	if(cropParams->channelNum == 1)
		status = rszBSetCropOutConfig(cropParams->CropStartX, cropParams->CropStartY,
					cropParams->ResolutionWidth, cropParams->ResolutionHeight);

    /* unlock driver instance */
    Iss_captUnlock(pObj);

    return status;
}

/* ===================================================================
 *  @func     Iss_captGetChStatus
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
Int32 Iss_captGetChStatus(Iss_CaptObj * pObj, Iss_CaptChGetStatusArgs * args,
                          Iss_CaptChStatus * chStatus)
{
    UInt16 lChannelNum, instId, streamId, chId;
    Iss_CaptChObj *pChObj;
    UInt32 curTime;
    Int32 status = FVID2_EFAIL;
    /*
     * parameter checking, not supported for global handle
     */
    if (pObj == (Iss_CaptObj *) ISS_CAPT_INST_ALL
        || pObj == NULL || args == NULL || chStatus == NULL)
        return FVID2_EFAIL;
    /*
     * init output structure
     */
    chStatus->isVideoDetected = FALSE;
    chStatus->outWidth = 0;
    chStatus->outHeight = 0;
    chStatus->droppedFrameCount = 0;
    chStatus->captureFrameCount = 0;
    /*
     * lock driver instance
     */
    Iss_captLock(pObj);
    if (pObj->state != ISS_CAPT_STATE_IDLE)
    {
        /*
         * driver needs to atleast be in open state for this call to work
         */
        /*
         * map from user channel number of driver channel number
         */
        lChannelNum = gIss_captCommonObj.fvidChannelNum2lChannelNumMap
            [args->channelNum];
        instId = Iss_captGetInstId(lChannelNum);
        streamId = Iss_captGetStreamId(lChannelNum);
        chId = Iss_captGetChId(lChannelNum);
        if (instId < ISS_CAPT_INST_MAX
            && streamId < pObj->numStream
            && chId < pObj->numCh && instId == pObj->instanceId)
        {
            /*
             * get channel object
             */
            pChObj = &pObj->chObj[streamId][chId];
            /*
             * get latest frame width x height
             */
            chStatus->outWidth = pChObj->lastFrameWidth;
            chStatus->outHeight = pChObj->lastFrameHeight;
            /*
             * get dropped frame count
             */
            chStatus->droppedFrameCount = pChObj->droppedFrameCount;
            chStatus->captureFrameCount = pChObj->captureFrameCount;
            /*
             * get current
             */
            curTime = Clock_getTicks();
            /*
             * if current time exceeds expected frame interval,
             * then its considered that video is not detected
             */
            if (curTime < (pChObj->lastFrameTimestamp
                           + args->frameInterval + 0))
            {
                chStatus->isVideoDetected = TRUE;
            }
            status = FVID2_SOK;
        }
    }
    /*
     * unlock driver instance
     */
    Iss_captUnlock(pObj);
    return status;
}
/* Driver object lock */
/* ===================================================================
 *  @func     Iss_captLock
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
Int32 Iss_captLock(Iss_CaptObj * pObj)
{
    /*
     * take semaphore for locking
     */
    Semaphore_pend(pObj->lock, BIOS_WAIT_FOREVER);
    return FVID2_SOK;
}
/* Driver object unlock */
/* ===================================================================
 *  @func     Iss_captUnlock
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
Int32 Iss_captUnlock(Iss_CaptObj * pObj)
{
    /*
     * release semaphore for un-locking
     */
    Semaphore_post(pObj->lock);
    return FVID2_SOK;
}
/* create driver semaphore */
/* ===================================================================
 *  @func     Iss_captLockCreate
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
Int32 Iss_captLockCreate(Iss_CaptObj * pObj)
{
    Int32 retVal = FVID2_SOK;
    Semaphore_Params semParams;
    /*
     * create locking semaphore
     */
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    pObj->lock = Semaphore_create(1u, &semParams, NULL);
    if (pObj->lock == NULL)
    {
        Vps_printf(" CAPTURE:%s:%d: Semaphore_create() failed !!!\n",
                   __FUNCTION__, __LINE__);
        retVal = FVID2_EALLOC;
    }
    return retVal;
}
/* delete driver semaphore */
/* ===================================================================
 *  @func     Iss_captLockDelete
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
Int32 Iss_captLockDelete(Iss_CaptObj * pObj)
{
    /*
     * delete semaphore
     */
    Semaphore_delete(&pObj->lock);
    return FVID2_SOK;
}
/* ===================================================================
 *  @func     Iss_captCheckOverflow
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
Int32 Iss_captCheckOverflow(Iss_CaptOverFlowStatus * overFlowStatus)
{
    Int32 status = FVID2_SOK;
    return status;
}

/* ===================================================================
 *  @func     Iss_captResetAndRestart
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
#define RESET_VALUE
Int32 Iss_captResetAndRestart(Iss_CaptOverFlowStatus * overFlowStatus)
{
#ifdef RESET_VALUE
	int count;
    Int32 numPaxels;
	Int32 AEWBBuffSize;
    Int32 pixCtWin;
	Iss_2AObj *p2AObj;
#endif
	Iss_CaptObj *pObj;
	Int32 status = 0;
	MSP_U32 eMSP;
	MSP_IspInterruptCfgT tIspInterruptCfg;
	MSP_IspConfigSizesT *pConfigSizes;
	MSP_IssCallbackRegCfgT tIssCallbackRegCfg;
	MSP_IspParamCfgT tIspParamCfg;
	MSP_IspQueryBscSizesT tIspBscSizes;
	MSP_IspYuvRangeCfgT tIspYuvRangeCfg;
	MSP_IspCallbackRegCfgT tCallbackRegCfg;

    MSP_IspMemToMemCmdParamT tIspMemToMemCmdParam;
    MSP_IspProcessParamT tIspProcessParam;
    MSP_IspQueryH3aStateT tQueryH3aState;
    MSP_ERROR_TYPE eResult = MSP_ERROR_NONE;

#ifdef ENABLE_BTE
		int i;
		bte_config_t *contextHandle;
		UInt32 oriBufOffsetY, oriBufOffsetC;
#endif

	UInt32 temp_nGainTableAddress;
	UInt32 temp_nOffsetTableAddress;

    /*
     * get instance specific handle
     */
    pObj = &gIss_captCommonObj.captureObj[0];

	Iss_captLock(pObj);
	//Clear the trigger
	isp_reset_trigger = 0;

	temp_nGainTableAddress = gIss_captCommonObj.pIssConfig->ptLsc2D->nGainTableAddress;
	temp_nOffsetTableAddress = gIss_captCommonObj.pIssConfig->ptLsc2D->nOffsetTableAddress;

	//while (isp_regs->ISP5_IRQSTATUS_RAW_0 & 0x8000 == 0x0);
	rsz_start(RSZ_STOP);

	//#jaeki - for mem free
	tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_H3A_END_ISR;
	eResult =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_UNREG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_RSZ_END_ISR;
    eResult =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_UNREG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_CCP_LCM;
    eResult =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISS_CFG_UNREG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_IPIPE_INT_BSC;
    eResult =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_UNREG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_ISIF_2DLSC_ISR;
    eResult =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_UNREG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_IPIPE_INT_BSC;
    eResult =
        MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_UNREG_CALLBACK,
                       &tCallbackRegCfg);
    if (MSP_ERROR_NONE != eResult)
    {
        Vps_printf("Error MSP_ISP_config failed!!!\n");
    }

	//Reset the ISP h/w
	isp_regs->ISP5_SYSCONFIG |= 0x2;
	Task_sleep(1);
	while (isp_regs->ISP5_SYSCONFIG & 0x2 == 0x1);
	Iss_reInit(NULL);

	//Vps_printf("%s, Iss_reInit done\n", __func__);
    pConfigSizes = NULL;

    gIss_captCommonObj.pModuleInstance = (VideoModuleInstanceT *) ((pObj->createArgs).pCompPrivate);
    gIss_captCommonObj.pModuleInstance->tIspAppParam.pAppData = gIss_captCommonObj.pModuleInstance;
    gIss_captCommonObj.pModuleInstance->tIspAppParam.MSP_callback = drvIspCallback;

	((MSP_COMPONENT_TYPE *) (gIss_captCommonObj.pModuleInstance->hIspHandle))->tAppCBParam = gIss_captCommonObj.pModuleInstance->tIspAppParam;
    CLEAR_ALL_EVENTS(gIss_captCommonObj.pModuleInstance);
    MSP_ISP_init(gIss_captCommonObj.pModuleInstance->hIspHandle, MSP_PROFILE_REMOTE);
    pConfigSizes = (MSP_PTR) malloc(sizeof(MSP_IspConfigSizesT));
    GT_assert(GT_DEFAULT_MASK, pConfigSizes != NULL);

    MSP_ISP_control(gIss_captCommonObj.pModuleInstance->hIspHandle, MSP_CTRLCMD_START, NULL);
    // REGISTER ISP INTERRUPTS
    tCallbackRegCfg.nContext = MSP_ISP_CALLBACK_CONTEXT_HWI;
    tCallbackRegCfg.nPriority = 3;
	//Vps_printf("%s, register interrupts \n", __func__);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_RSZ_END_ISR;
    eMSP = MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_REG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_ISIF_VD_ISR;
    eMSP = MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_REG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_IPIPE_INT_BSC;
    eMSP = MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_REG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_H3A_END_ISR;
    eMSP = MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_REG_CALLBACK,
                       &tCallbackRegCfg);
    tCallbackRegCfg.eInterruptId = MSP_ISS_DRV_ISIF_2DLSC_ISR;
    eMSP = MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_REG_CALLBACK,
                       &tCallbackRegCfg);
    tIssCallbackRegCfg.eInterruptId = MSP_ISS_DRV_CCP_LCM;
    eMSP = MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISS_CFG_REG_CALLBACK,
                       &tIssCallbackRegCfg);
	//Vps_printf("%s, register interrupts done\n", __func__);

	isif_reg->VDINT0 = isif_reg->VDINT1 = isif_reg->VDINT2 = pObj->createArgs.scParams[0].inHeight - 5;

    *(MSP_U32 *) (0x5505002C) |= 1;                        // isp IRQ set
                                                           // register
    tIspInterruptCfg.bEnable = MSP_TRUE;
    tIspInterruptCfg.eInterruptId = MSP_ISS_DRV_CCP_LCM;
    eMSP =  MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISS_CFG_IRQ, &tIspInterruptCfg);
    tIspInterruptCfg.eInterruptId = MSP_ISS_DRV_IPIPE_INT_BSC;
    eMSP =  MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                       (MSP_INDEXTYPE) MSP_ISP_CFG_INT, &tIspInterruptCfg);
    tIspInterruptCfg.eInterruptId = MSP_ISS_DRV_ISIF_2DLSC_ISR;
    MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle, (MSP_INDEXTYPE) MSP_ISP_CFG_INT,
                   &tIspInterruptCfg);

    pConfigSizes->nInSizeX = pObj->createArgs.scParams[0].inWidth;
    pConfigSizes->nInSizeY = pObj->createArgs.scParams[0].inHeight;
    pConfigSizes->nInSizePpln = pObj->createArgs.pitch[0];
    pConfigSizes->nInStartX = pObj->createArgs.scParams[0].InStartX;
    pConfigSizes->nInStartY = pObj->createArgs.scParams[0].InStartY;
	if(pObj->vsEnable == 0)
	{
		pConfigSizes->nOutSizeAX = pObj->createArgs.scParams[0].outWidth;
		pConfigSizes->nOutSizeAY = pObj->createArgs.scParams[0].outHeight;
	}
	else
	{
		pConfigSizes->nOutSizeAX = (pObj->createArgs.scParams[0].outWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
		pConfigSizes->nOutSizeAY = (pObj->createArgs.scParams[0].outHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;;
	}
	if((pConfigSizes->nOutSizeAX > pConfigSizes->nInSizeX) || (pConfigSizes->nOutSizeAY > pConfigSizes->nInSizeY))
	{
	    GT_assert(GT_DEFAULT_MASK, 0);
	}

    pConfigSizes->nOutSizeABpln = pObj->createArgs.pitch[0];
    pConfigSizes->nOutSizeABplnC = 0;

    if (pObj->numStream == 2)
    {
	    if(pObj->vsEnable == 0)
		{
			pConfigSizes->nOutSizeBX = pObj->createArgs.scParams[1].outWidth;
			pConfigSizes->nOutSizeBY = pObj->createArgs.scParams[1].outHeight;
		}
		else
		{
			pConfigSizes->nOutSizeBX = (pObj->createArgs.scParams[1].outWidth * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
			pConfigSizes->nOutSizeBY = (pObj->createArgs.scParams[1].outHeight * VSTAB_SCALE_NUMERATOR)/VSTAB_SCALE_DENOMINATOR;
		}
		if((pConfigSizes->nOutSizeBX > pConfigSizes->nInSizeX) || (pConfigSizes->nOutSizeBY > pConfigSizes->nInSizeY))
		{
			GT_assert(GT_DEFAULT_MASK, 0);
		}

        pConfigSizes->nOutSizeBBpln = pObj->createArgs.pitch[1];
        pConfigSizes->nOutSizeBBplnC = 0;
    }
    else
    {
        pConfigSizes->nOutSizeBX = 720;
        pConfigSizes->nOutSizeBY = 480;
        pConfigSizes->nOutSizeBBpln = 720;
        pConfigSizes->nOutSizeBBplnC = 0;
    }

    pConfigSizes->nCropX = pObj->createArgs.scParams[0].inCropCfg.cropStartX;
    pConfigSizes->nCropY = pObj->createArgs.scParams[0].inCropCfg.cropStartY;
    pConfigSizes->nClockPercents = MSP_ISS_SPEED_MAX;
    eMSP =
        MSP_ISP_query(gIss_captCommonObj.pModuleInstance->hIspHandle,
                      (MSP_INDEXTYPE) MSP_ISP_QUERY_MAX_WIDTH, pConfigSizes);
    eMSP =
        MSP_ISP_query(gIss_captCommonObj.pModuleInstance->hIspHandle,
                      (MSP_INDEXTYPE) MSP_ISP_QUERY_CROP_SIZES, pConfigSizes);
    if (eMSP != MSP_ERROR_NONE)
    {
        // Vps_printf("Error MSP_ISP_query failed!!!\n");
    }

#ifdef RESET_VALUE
    gIss_captCommonObj.pModuleInstance->nLscTableIsValid = MSP_FALSE;
    gIss_captCommonObj.pModuleInstance->nOutStartX = 0;
    gIss_captCommonObj.pModuleInstance->nOutStartY = 0;
    gIss_captCommonObj.pModuleInstance->bVidStab = MSP_FALSE;
    gIss_captCommonObj.pModuleInstance->bAllocateOutBuff = MSP_FALSE;
    IssFillDefaultParams((iss_config_processing_t *) gIss_captCommonObj.pIssConfig);
    gIss_captCommonObj.pIssConfig->eMsbPos = MSP_IPIPE_BAYER_MSB_BIT11;
    gIss_captCommonObj.pIssConfig->ptBsc = &(gIss_captCommonObj.pModuleInstance->tBscCfg);
    gIss_captCommonObj.pIssConfig->ptLsc2D->nHDirDataOffset = 16;
    gIss_captCommonObj.pIssConfig->eColorPattern = MSP_IPIPE_BAYER_PATTERN_GRBG;
    gIss_captCommonObj.pIssConfig->nVpDevice = MSP_IPIPE_VP_DEV_PI;
    gIss_captCommonObj.pIssConfig->eFH3aValidity =
        (MSP_PROC_H3A_VALID_ID) (MSP_PROC_H3A_VALID_AEWB |
                                 MSP_PROC_H3A_VALID_H3A |
                                 MSP_PROC_H3A_VALID_AF);
    gIss_captCommonObj.pModuleInstance->tConfigParams.eInFormat = MSP_IPIPE_IN_FORMAT_BAYER;

    if ((pObj->createArgs.outStreamInfo[0].dataFormat == FVID2_DF_YUV420SP_UV)
        || (pObj->createArgs.outStreamInfo[0].dataFormat ==
            FVID2_DF_YUV420SP_VU))
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.eOutFormatA =
            MSP_IPIPE_OUT_FORMAT_YUV420;
    }
    else if ((pObj->createArgs.outStreamInfo[0].dataFormat ==
              FVID2_DF_YUV422I_VYUY))
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.eOutFormatA =
            MSP_IPIPE_OUT_FORMAT_YUV422;
    }
    else if(pObj->createArgs.outStreamInfo[0].dataFormat == FVID2_DF_BAYER_RAW)
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.eOutFormatA = MSP_IPIPE_OUT_FORMAT_YUV422;
    }
	else
	{
	       Vps_printf("Stream 0: Invalid output format configured!!!\n");
	}
    if ((pObj->createArgs.outStreamInfo[1].dataFormat == FVID2_DF_YUV420SP_UV)
        || (pObj->createArgs.outStreamInfo[1].dataFormat ==
            FVID2_DF_YUV420SP_VU))
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.eOutFormatB =
            MSP_IPIPE_OUT_FORMAT_YUV420;
    }
    else if ((pObj->createArgs.outStreamInfo[1].dataFormat ==
              FVID2_DF_YUV422I_VYUY))
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.eOutFormatB =
            MSP_IPIPE_OUT_FORMAT_YUV422;
    }
    else
    {
        Vps_printf("Stream 1: Invalid output format configured!!!\n");
    }

    gIss_captCommonObj.pModuleInstance->tConfigParams.eCompresIn = MSP_IPIPE_PROC_COMPR_NO;
    gIss_captCommonObj.pModuleInstance->tConfigParams.eCompresOut = MSP_IPIPE_PROC_COMPR_NO;
    gIss_captCommonObj.pModuleInstance->tConfigParams.nFFlipB = 0;
    gIss_captCommonObj.pModuleInstance->tConfigParams.nFMirrorB = 0;
    gIss_captCommonObj.pModuleInstance->tConfigParams.nFFlipA = 0;
    gIss_captCommonObj.pModuleInstance->tConfigParams.nFMirrorA = 0;
    gIss_captCommonObj.pModuleInstance->tConfigParams.nVidstabEnb = 0;
    // gIss_captCommonObj.pModuleInstance->tConfigParams.eInComp = 0;
    gIss_captCommonObj.pModuleInstance->tConfigParams.ptIssConfig = gIss_captCommonObj.pIssConfig;
    gIss_captCommonObj.pModuleInstance->tConfigParams.pLsc2DTableBuffer =
        gIss_captCommonObj.pModuleInstance->p2DLscTableBuff;
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nInSizeX =
        gIss_captCommonObj.pModuleInstance->nIssInX;
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nInSizeY =
        gIss_captCommonObj.pModuleInstance->nIssInY;
    gIss_captCommonObj.pModuleInstance->tConfigParams.ptIssConfig->eRszIpPortSel =  MSP_RSZ_IP_IPIPEIF;
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeAX =
        pConfigSizes->nOutSizeAX; //pObj->createArgs.scParams[0].outWidth;
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeAY =
        pConfigSizes->nOutSizeAY; //pObj->createArgs.scParams[0].outHeight;
#ifndef ENABLE_BTE
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeABpln =
        pObj->createArgs.pitch[0];
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeABplnC =
        pObj->createArgs.pitch[0];
#else
	gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeABpln =
		BTE_PITCH;
	gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeABplnC =
		BTE_PITCH;
#endif

    if (pObj->numStream == 2)
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBX =
            pConfigSizes->nOutSizeBX; //pObj->createArgs.scParams[1].outWidth;
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBY =
            pConfigSizes->nOutSizeBY; //pObj->createArgs.scParams[1].outHeight;
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBBpln =
            pObj->createArgs.pitch[1];
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBBplnC =
            pObj->createArgs.pitch[1];
    }
    else
    {
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBX = 720;
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBY = 480;
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBBpln = 720;
        gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nOutSizeBBplnC = 720;
    }
    gIss_captCommonObj.pModuleInstance->tConfigParams.tConfigSizes.nClockPercents =
        MSP_ISS_SPEED_MAX;
#endif

    tIspParamCfg.ptIssParams = &gIss_captCommonObj.pModuleInstance->tConfigParams;
    // RAJAT - need to correct the configuration being sent out to remove the error
    MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                   (MSP_INDEXTYPE) MSP_ISP_CFG_PROC_MEM_TO_MEM, &tIspParamCfg);

	rszASetOutConfig(pObj->createArgs.scParams[0].inWidth, pObj->createArgs.scParams[0].inHeight,
					 pObj->createArgs.scParams[0].outWidth, pObj->createArgs.scParams[0].outHeight);
	if(pObj->numStream == 1) {
		status = Iss_Disable_RszB();
		if(status == FVID2_EFAIL)
			Vps_rprintf("Iss_Disable_RszB Failed \n");
	}
	else if(pObj->numStream == 2) {
		rszBSetOutConfig(pObj->createArgs.scParams[0].inWidth,
						 pObj->createArgs.scParams[0].inHeight,
						 pObj->createArgs.scParams[1].outWidth, pObj->createArgs.scParams[1].outHeight);
		status = rszBSetCropOutConfig(pObj->createArgs.scParams[1].inCropCfg.cropStartX,
					pObj->createArgs.scParams[1].inCropCfg.cropStartY,
					pObj->createArgs.scParams[1].inCropCfg.cropWidth,
					pObj->createArgs.scParams[1].inCropCfg.cropHeight);
	}

#ifdef IMGS_SONY_IMX122
	/* ISIF Size config*/
	isif_reg->HDW				= 1937;
	isif_reg->VDW				= 1083;
	isif_reg->PPLN				= 1937;
	isif_reg->LPFR				= 2167;
	isif_reg->LNH				= 1923;
	isif_reg->LNV				= 1081;

	/* IPIPE Size config */
	ipipe_reg->SRC_VSZ			= 1081;//1083;
	ipipe_reg->SRC_HSZ			= 1921;//1929;

	isif_reg->CCOLP		= 0xE4E4;
	ipipe_reg->SRC_COL	= 0xE4;
	ipipe_reg->SRC_HPS	= 156;
	ipipe_reg->SRC_VPS	= 32;
#else
    ipipe_reg->SRC_COL = 177;
#endif

#ifdef RESET_VALUE
	*(MSP_U32 *)(0x5505006C) &= 0xFFFFFFF3;

    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVCount = 32;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHCount = 16;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVSize = 32;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHSize = 120;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHIncr = 8;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVIncr = 8;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVPos = 30;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHPos = 0;

#ifdef IMGS_SONY_IMX122
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHPos = 0+156;//kjs
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVPos = 30+32;
#endif
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptBlkWinDims->nVPos = 0;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptBlkWinDims->nHPos = 2;  // AEWINBLK WINH
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->nShiftValue = 0;
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->nSaturationLimit = 0x3FE;

	gIss_captCommonObj.pIssConfig->ptH3aAfParams->eAfEnable = MSP_H3A_FEATURE_ENABLE;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->eRgbPos = MSP_H3A_AF_RGBPOS_RG_GB_BAYER;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ePeakModeEn = MSP_H3A_FEATURE_ENABLE;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->eVerticalFocusEn = MSP_H3A_FEATURE_ENABLE;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->nIirStartPos = 30;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nVPos = 34;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nVSize = 64;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nHPos = 32;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nHSize = 308;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nVCount = 16;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nVIncr = 8;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nHCount = 6;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptAfPaxelWin->nHIncr = 8;
	for(count=0;count<11;count++){
		gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptIir1->anIirCoef[count] = anIirCoef1[count];
		gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptIir2->anIirCoef[count] = anIirCoef2[count];
	}
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptIir1->nHfvThres = 100;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptIir2->nHfvThres = 100;
	for(count=0;count<5;count++){
		gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptFir1->anFirCoef[count] = 2;
		gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptFir2->anFirCoef[count] = 2;
	}
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptFir1->nVfvThres = 500;
	gIss_captCommonObj.pIssConfig->ptH3aAfParams->ptFir2->nVfvThres = 500;
    gIss_captCommonObj.pIssConfig->ptLsc2D->nGainTableLength = 40240;
	gIss_captCommonObj.pIssConfig->ptLsc2D->nOffsetTableLength = 40240;
    numPaxels =
        (gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVCount +
         1) * gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHCount;
    AEWBBuffSize = (numPaxels * 32) + (((numPaxels + 7) / 8) * 32);
    pixCtWin =
        (gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVSize /
         gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVIncr) *
        (gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHSize /
         gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHIncr);
    gIss_captCommonObj.gH3ABufParams.h3aUpdate = 1;
    gIss_captCommonObj.gH3ABufParams.emptyFrame = 1;
    gIss_captCommonObj.gH3ABufParams.aewbNumPix = pixCtWin;
    gIss_captCommonObj.gH3ABufParams.aewbNumWinH =
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHCount;
    gIss_captCommonObj.gH3ABufParams.aewbNumWinV =
    gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVCount;
    p2AObj = (Iss_2AObj*)gIss_captCommonObj.pModuleInstance->p2AObj;
    p2AObj->pIspHandle      = gIss_captCommonObj.pModuleInstance->hIspHandle;
    p2AObj->pIssConfig      = gIss_captCommonObj.pIssConfig;
    p2AObj->h3aBufSize      = AEWBBuffSize;
	p2AObj->aewbNumPix      = pixCtWin;
	p2AObj->aewbNumWinH     = gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nHCount;
	p2AObj->aewbNumWinV     = gIss_captCommonObj.pIssConfig->ptH3aAewbParams->ptAewbPaxelWin->nVCount;
	p2AObj->aewbVendor      = (AEWB_VENDOR)pObj->createArgs.aewbVendor;
	p2AObj->aewbMode        = pObj->createArgs.aewbMode;
	p2AObj->aewbModeFlag    = 1;
	p2AObj->aewbPriority    = pObj->createArgs.aewbPriority;
	p2AObj->SensorHandle    = pObj->createArgs.SensorHandle;
	p2AObj->cameraVipHandle = pObj;
#endif

	gIss_captCommonObj.pIssConfig->ptLsc2D->nGainTableAddress = temp_nGainTableAddress;
	gIss_captCommonObj.pIssConfig->ptLsc2D->nOffsetTableAddress = temp_nOffsetTableAddress;

    IssAlg_capt2AInit((Iss_2AObj*)gIss_captCommonObj.pModuleInstance->p2AObj);
    tIspBscSizes.pRowsBufferSize = 0;
    tIspBscSizes.pColsBufferSize = 0;
    tIspBscSizes.ptBscCfg = gIss_captCommonObj.pModuleInstance->tConfigParams.ptIssConfig->ptBsc;
    MSP_ISP_query(gIss_captCommonObj.pModuleInstance->hIspHandle,
                  (MSP_INDEXTYPE) MSP_ISP_QUERY_BSC_BUFF_SIZES, &tIspBscSizes);
    tIspYuvRangeCfg.eIspYuvRange = MSP_ISS_YUV_RANGE_FULL;
    MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
                   (MSP_INDEXTYPE) MSP_ISP_CFG_YUV_RANGE, &tIspYuvRangeCfg);
	/* Histogram Config */
	{
		MSP_IpipeHistCfgT histogramCfg;
		MSP_IpipeHistDimT histogramDim[4];
		MSP_U8            gainTbl[4];

		memcpy(&histogramCfg,gIss_captCommonObj.pIssConfig->ptHistogram,sizeof(MSP_IpipeHistCfgT));

		histogramCfg.nOst  = 0;		// Free Run
		histogramCfg.nSel  = 0;		// Input Selection - From noise filter
		histogramCfg.nType = 2;		// G selection in Bayer mode - (Gb + Gr)/2
		histogramDim[0].nVPos  = 0;
		histogramDim[0].nVSize = pObj->createArgs.scParams[0].inHeight - 33;
		histogramDim[0].nHPos  = 0;
		histogramDim[0].nHSize = pObj->createArgs.scParams[0].inWidth - 1;

		gainTbl[0] = 6;				// R
		gainTbl[1] = 6;				// Gr
		gainTbl[2] = 6;				// Gb
		gainTbl[3] = 6;				// B
		histogramCfg.ptHistDim = histogramDim;
		histogramCfg.pnGainTbl = gainTbl;
		MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,(MSP_INDEXTYPE)MSP_ISP_CFG_HIST,&histogramCfg);
	}
	if(pObj->glbcEnable == 1)
	{
		MSP_IpipeBoxcarCfgT BoxCarCfg;
		memcpy(&BoxCarCfg,gIss_captCommonObj.pIssConfig->ptBoxcar,sizeof(MSP_IpipeBoxcarCfgT));
		BoxCarCfg.nEnable = TRUE;
		if(pObj->createArgs.captureMode == ISS_CAPT_INMODE_ISIF)
		BoxCarCfg.nOst = 0;
		else
		BoxCarCfg.nOst = 1;
		BoxCarCfg.nBoxSize = 1;
		BoxCarCfg.nShift = 3;
		BoxCarCfg.pAddr = NULL;

		MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,(MSP_INDEXTYPE)MSP_ISP_CFG_BOXCAR,&BoxCarCfg);
	}

    IssAlg_captInit();

#ifdef ENABLE_BTE
		GT_assert(GT_DEFAULT_MASK, bte_init() == BTE_SUCCESS);

		GT_assert(GT_DEFAULT_MASK, bte_open(1) == BTE_SUCCESS);/*0x20000000~0x3FFFFFFF*/

		gIss_captCommonObj.handleBTE.baseAddress = 0x20000000;

		gIss_captCommonObj.handleBTE.contextNumber =
			2 << BTE_FEXT(bte_reg->BTE_HL_HWINFO, CSL_BTE_BTE_HL_HWINFO_CONTEXTS_SHIFT, CSL_BTE_BTE_HL_HWINFO_CONTEXTS_MASK);

		gIss_captCommonObj.handleBTE.bwLimiter = BTE_BWL;

		BTE_SET32(bte_reg->BTE_CTRL, gIss_captCommonObj.handleBTE.bwLimiter, CSL_BTE_BTE_CTRL_BW_LIMITER_SHIFT,
				  CSL_BTE_BTE_CTRL_BW_LIMITER_MASK);
		BTE_SET32(bte_reg->BTE_CTRL, 1, CSL_BTE_BTE_CTRL_POSTED_SHIFT,
				  CSL_BTE_BTE_CTRL_POSTED_MASK);

		for (i = 0; i < gIss_captCommonObj.handleBTE.contextNumber; i++)
		{
			contextHandle = &gIss_captCommonObj.handleBTE.bte_config[i];

			contextHandle->context_num = (BTE_CONTEXT)i;

			contextHandle->context_ctrl.init_sx = 0;
			contextHandle->context_ctrl.init_sy = 0;

			contextHandle->context_ctrl.mode = 0x0;/*write*/
			contextHandle->context_ctrl.one_shot = 0x1;/*one shot*/
			contextHandle->context_ctrl.addr32 = 1;/*TILER*/
			contextHandle->context_ctrl.autoflush = 0;

			contextHandle->context_ctrl.start = 0;
			contextHandle->context_ctrl.stop = 0;
			contextHandle->context_ctrl.flush = 0;
		}

		gIss_captCommonObj.handleBTE.bte_config[0].context_start_addr = 0;
		if (pObj->createArgs.outStreamInfo[0].dataFormat != FVID2_DF_YUV422I_VYUY)
			gIss_captCommonObj.handleBTE.bte_config[1].context_start_addr =
				(DIV_UP(pObj->createArgs.scParams[0].outWidth, 128) << CSL_BTE_BTE_CONTEXT_START_0_X_SHIFT);

		configureBteContext(&gIss_captCommonObj.handleBTE, 0,
			pObj->createArgs.scParams[0].outWidth, pObj->createArgs.scParams[0].outHeight,
			pObj->createArgs.outStreamInfo[0].dataFormat, curStreamBuf[0].mirrorMode);

		oriBufOffsetY = Utils_tilerGetOriAddr((Int32)PrimaryFrame_RszA->addr[1][0], 0, curStreamBuf[0].mirrorMode,
			pObj->createArgs.scParams[0].outWidth, pObj->createArgs.scParams[0].outHeight);
		startBteContext(&gIss_captCommonObj.handleBTE.bte_config[0], oriBufOffsetY);

		if (pObj->createArgs.outStreamInfo[0].dataFormat != FVID2_DF_YUV422I_VYUY)
		{
			oriBufOffsetC = Utils_tilerGetOriAddr((Int32)PrimaryFrame_RszA->addr[1][1], 1, curStreamBuf[0].mirrorMode,
				pObj->createArgs.scParams[0].outWidth, pObj->createArgs.scParams[0].outHeight/2);
			startBteContext(&gIss_captCommonObj.handleBTE.bte_config[1], oriBufOffsetC);
		}
#endif

	PrimaryFrame_RszA = curStreamBuf[0].ptmpFrame;
	PrimaryFrame_RszB = curStreamBuf[1].ptmpFrame;
	EXPECT_EVENT(gIss_captCommonObj.pModuleInstance, VIDEO_ISR_EVENT_RSZ);
#ifndef ENABLE_BTE
	tIspMemToMemCmdParam.pOutbuffRszA = PrimaryFrame_RszA->addr[1][0];
	tIspMemToMemCmdParam.pOutbuffRszACr = PrimaryFrame_RszA->addr[1][1];
#else
	tIspMemToMemCmdParam.pOutbuffRszA = (void *)(gIss_captCommonObj.handleBTE.baseAddress +
		gIss_captCommonObj.handleBTE.bte_config[0].context_start_addr);

	tIspMemToMemCmdParam.pOutbuffRszACr = (void *)(gIss_captCommonObj.handleBTE.baseAddress +
		gIss_captCommonObj.handleBTE.bte_config[1].context_start_addr);
#endif
	curStreamBuf[0].ptmpFrame = PrimaryFrame_RszA;
	if(pObj->numStream == 2)
	{
		tIspMemToMemCmdParam.pOutbuffRszB = PrimaryFrame_RszB->addr[1][0];
		tIspMemToMemCmdParam.pOutbuffRszBCr = PrimaryFrame_RszB->addr[1][1];
		curStreamBuf[1].ptmpFrame = PrimaryFrame_RszB;
	}
	else
	if(pObj->numStream == 1)
	{
		tIspMemToMemCmdParam.pOutbuffRszB = NULL;
		tIspMemToMemCmdParam.pOutbuffRszBCr = NULL;
		curStreamBuf[1].ptmpFrame = NULL;
	}
	if(pObj->glbcEnable == 1)
		ipipe_config_boxcar_addr((UInt32)PrimaryFrame_RszA->blankData);
	tIspMemToMemCmdParam.eInFormat = MSP_IPIPE_IN_FORMAT_BAYER;

	tIspMemToMemCmdParam.pOutRawBuffer = (void *) NULL;
	tIspMemToMemCmdParam.pOutRawBuffer = NULL;
	tIspProcessParam.eCmd = MSP_ISP_CMD_MEM_TO_MEM_START;
	tIspProcessParam.pCmdData = &tIspMemToMemCmdParam;
	if(pObj->createArgs.captureMode == ISS_CAPT_INMODE_ISIF)
	{
		eResult =  MSP_ISP_process(gIss_captCommonObj.pModuleInstance->hIspHandle, &tIspProcessParam,
							NULL);
		if (MSP_ERROR_NONE == eResult)
		{
		}
	}
	else
	{
		// ISS_CAPT_INMODE_DDR mode
		Iss_ispWriteReg(&isif_reg->CADU,(((UInt32)PrimaryFrame_RszA->addr[1][0] >> 21) & 0x7FF),0,11);
		Iss_ispWriteReg(&isif_reg->CADL,(((UInt32)PrimaryFrame_RszA->addr[1][0] >> 5) & 0xFFFF),0,16);
		isif_start(ISIF_START_WRITE);
	}
	tQueryH3aState.eH3aState = MSP_H3A_STOPPED;
	issDrvH3aStart();
	MSP_ISP_query(gIss_captCommonObj.pModuleInstance->hIspHandle,
				  (MSP_INDEXTYPE) MSP_ISP_QUERY_H3A_STATE, &tQueryH3aState);
	if (tQueryH3aState.eH3aState & (MSP_H3A_AEWB_ENABLED))
	{
        EXPECT_EVENT(gIss_captCommonObj.pModuleInstance, VIDEO_ISR_EVENT_H3A);
		tIspInterruptCfg.eInterruptId = MSP_ISS_DRV_H3A_END_ISR;
		tIspInterruptCfg.bEnable = MSP_TRUE;
		MSP_ISP_config(gIss_captCommonObj.pModuleInstance->hIspHandle,
					   (MSP_INDEXTYPE) MSP_ISP_CFG_INT,
					   (MSP_PTR) & tIspInterruptCfg);
	}

#ifdef IMGS_SONY_IMX122
	isif_reg->HSIZE = 120;
#endif

	//Clear the trigger
	isp_reset_trigger = 0;

	ipipe_reg->RGB1_MUL_RR = 368;
    ipipe_reg->RGB1_MUL_GR = -34;
    ipipe_reg->RGB1_MUL_BR = -34;
    ipipe_reg->RGB1_MUL_RG = -34;
    ipipe_reg->RGB1_MUL_GG = 368;
    ipipe_reg->RGB1_MUL_BG = -34;
    ipipe_reg->RGB1_MUL_RB = -34;
    ipipe_reg->RGB1_MUL_GB = -34;
    ipipe_reg->RGB1_MUL_BB = 368;

    /*
     * unlock driver instance
     */
    Iss_captUnlock(pObj);
    if (pConfigSizes != NULL)
    {
        free(pConfigSizes);
    }
	Iss_ispDrvCallBack();

    return status;
}

/* ===================================================================
 *  @func     Iss_captPrintAdvancedStatistics
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
Int32 Iss_captPrintAdvancedStatistics(UInt32 totalTimeInMsecs)
{
    return 0;
}
/* ===================================================================
 *  @func     Iss_captDequeueFromQueToFrameList
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
Int32 Iss_captDequeueFromQueToFrameList(VpsUtils_QueHandle * queHandle,
                                        FVID2_FrameList * frameList)
{
    Int32 dequeStatus;
    volatile FVID2_Frame *pData;
    do
    {
        /*
         * Max frames limit exceeded exit
         */
        if (frameList->numFrames >= FVID2_MAX_FVID_FRAME_PTR)
            break;
        pData = NULL;
        /*
         * deque from handle, could be blocking for the first time
         * based on 'timeout'
         */
        dequeStatus = VpsUtils_queGet(queHandle,
                                      (Ptr *) & pData, 1, BIOS_NO_WAIT);
        if (dequeStatus == FVID2_SOK && pData != NULL &&
            pData->channelNum != ISS_CAPT_DROP_FRAME_CH_ID)
        {
            /*
             * deque is successful, add to frame list
             */
            frameList->frames[frameList->numFrames] = (FVID2_Frame *) pData;
            frameList->numFrames++;
        }
        /*
         * Max frames limit exceeded exit
         */
        if (frameList->numFrames >= FVID2_MAX_FVID_FRAME_PTR)
            break;
    }
    while (dequeStatus == FVID2_SOK);
    return FVID2_SOK;
}
/* ===================================================================
 *  @func     Iss_captFlush
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
Int32 Iss_captFlush(Fdrv_Handle handle, FVID2_FrameList * frameList)
{
    Iss_CaptObj *pObj = (Iss_CaptObj *) handle;
    Int32 status = FVID2_SOK, instId, streamId, chId;
    Iss_CaptChObj *pChObj;
    /*
     * if global handle then make handle as NULL, this indicates
     * we need to check over all driver instances
     */
    if (handle == (Fdrv_Handle) ISS_CAPT_INST_ALL)
        handle = NULL;
    /*
     * parameter checking
     */
    if (frameList == NULL)
    {
        return FVID2_EFAIL;
    }
    /*
     * init frame list fields
     */
    frameList->numFrames = 0;
    frameList->perListCfg = NULL;
    frameList->drvData = NULL;
    frameList->reserved = NULL;
    /*
     * for every instance
     */
    for (instId = 0; instId < ISS_CAPT_INST_MAX; instId++)
    {
        if (handle == NULL)
        {
            /*
             * if handle is NULL, i.e global handle, then get handle using 'instId',
             * else use user supplied handle
             */
            pObj = &gIss_captCommonObj.captureObj[instId];
        }
        if (pObj->state == ISS_CAPT_STATE_CREATED
            || pObj->state == ISS_CAPT_STATE_STOPPED)
        {
            /* dequeue frames to frame list for this driver inst */
            for (streamId = 0u; streamId < pObj->numStream; streamId++)
            {
                Iss_captDequeueFromQueToFrameList(&pObj->fullQue[streamId],
                                                  frameList);
                for (chId = 0u; chId < pObj->numCh; chId++)
                {
                    pChObj = &pObj->chObj[streamId][chId];
                    Iss_captDequeueFromQueToFrameList(&pChObj->tmpQue,
                                                      frameList);
                    Iss_captDequeueFromQueToFrameList(&pChObj->emptyQue,
                                                      frameList);
                }
            }
        }
        else
        {
            /* invalid state for dequeue all */
            if (handle != NULL)
                status = FVID2_EFAIL;
        }
        if (handle != NULL)
            break;                                         /* if not global
                                                            * handle, then we
                                                            * are done, so
                                                            * exit */
        /*
         * Max frames limit exceeded exit
         */
        if (frameList->numFrames >= FVID2_MAX_FVID_FRAME_PTR)
            break;
        /*
         * global handle mode, so check other driver instances also
         */
    }
    return status;
}
/* ===================================================================
 *  @func     drvIspCallback
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
static MSP_ERROR_TYPE drvIspCallback(MSP_PTR hMSP, MSP_PTR pAppData,
                                     MSP_EVENT_TYPE tEvent,
                                     MSP_OPAQUE nEventData1,
                                     MSP_OPAQUE nEventData2)
{
    MSP_ERROR_TYPE eResult = MSP_ERROR_NONE;
//    VideoModuleInstanceT *gIss_captCommonObj.pModuleInstance;
    MSP_U32 nRemainIsrEventsBefore;
    // pComponent = (MSP_COMPONENT_TYPE*)pAppData;
    gIss_captCommonObj.pModuleInstance = (VideoModuleInstanceT *) pAppData;
    // Save initial events to check if the event is expected
    nRemainIsrEventsBefore = gIss_captCommonObj.pModuleInstance->nRemainIsrEvents;
    switch (tEvent)
    {
        case MSP_ERROR_EVENT:
            break;
        case MSP_DATA_EVENT:
            switch ((MSP_U32) nEventData1)
            {
                case MSP_ISS_DRV_RSZ_END_ISR:
                    isp_rsz_dma_end_handle();
                    CLEAR_EVENT(gIss_captCommonObj.pModuleInstance, VIDEO_ISR_EVENT_RSZ);
                    // TODO
                    // TIMM_OSAL_SemaphoreRelease(gIss_captCommonObj.pModuleInstance->pIssSem);
                    break;
                case MSP_ISS_DRV_CCP_LCM:
                    CLEAR_EVENT(gIss_captCommonObj.pModuleInstance, VIDEO_ISR_EVENT_CCP);
                    break;
                case MSP_ISS_DRV_ISIF_VD_ISR:
                    isp_isif_linenum_handle();
                    break;
                case MSP_ISS_DRV_H3A_END_ISR:
					#if !(USE_NOT_2A)
					IssAlg_capt2AIsrCallBack((Iss_2AObj*)gIss_captCommonObj.pModuleInstance->p2AObj);
					#endif
                    CLEAR_EVENT(gIss_captCommonObj.pModuleInstance, VIDEO_ISR_EVENT_H3A);
                    break;
                case MSP_ISS_DRV_IPIPE_INT_BSC:
                    // Vps_rprintf("\n\nBSC interrupt received!!!");
                    /* Comment out unnecessary opetarion, Hennessy 2012-05-31*/
                    //video_bsc_copyBuffers();
                    CLEAR_EVENT(gIss_captCommonObj.pModuleInstance, VIDEO_ISR_EVENT_BSC);
                    break;
                case MSP_ISS_DRV_ISIF_2DLSC_ISR:
                    // TODO isif2DLscSofIsr(pAppData);
                    break;
                default:
                    break;
            };
            break;
        case MSP_PROFILE_EVENT:
        case MSP_OPEN_EVENT:
        case MSP_CLOSE_EVENT:
            break;
        case MSP_CTRLCMD_EVENT:
            switch ((MSP_U32) nEventData1)
            {
                case MSP_CALLBACK_CMD_START:
                    // TODO - ISP is ready for use
                    break;
                case MSP_CALLBACK_CMD_STOP:
                    break;
                default:
                    break;
            };
            break;
        default:
            break;
    };
    if ((nRemainIsrEventsBefore != gIss_captCommonObj.pModuleInstance->nRemainIsrEvents) &&
        (tEvent == MSP_DATA_EVENT) && (NO_EVENTS_REMAIN(gIss_captCommonObj.pModuleInstance)))
    {
        EXPECT_EVENT(gIss_captCommonObj.pModuleInstance, VIDEO_ISR_EVENT_H3A);
        EXPECT_EVENT(gIss_captCommonObj.pModuleInstance, VIDEO_ISR_EVENT_RSZ);
    }
    return (eResult);
}

Int32 Iss_Disable_RszB(){
	Int32 retval = 0;
	RESIZER_T resizer = RESIZER_B;
	retval = rsz_clock_enable(resizer, RSZ_CLK_DISABLE);
	return retval;
}
