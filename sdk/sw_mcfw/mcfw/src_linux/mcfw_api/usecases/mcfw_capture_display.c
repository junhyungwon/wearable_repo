/******************************************************************************
 * Copyright Texas Instruments Inc 2011, 2012
 * Use of this software	is controlled by the terms and conditions found
 * in the license agreement	under which	this software has been supplied
 *---------------------------------------------------------------------------*/
 /**
 * @file	mcfw_capture_display.c
 * @brief
 * @author	phoong
 * @section	MODIFY history
 *	   - 2012.12.07	: First	Created	based multich_Stream_CaptureDisplay.c
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include "mcfw/src_linux/mcfw_api/usecases/multich_common.h"
#include "mcfw/src_linux/mcfw_api/usecases/multich_ipcbits.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
//# link buffer numbers
#define	NUM_CAPTURE_BUFFERS		8
#define	NUM_ENCODE_BUFFERS		8
#define	NUM_NSF_BUFFERS			8
#define	NUM_SWMS_BUFFERS		8

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
#define	eprintf(x...) printf(" [mcfw_api] err: " x);
#define	dprintf(x...) printf(" [mcfw_api] "	x);
//#define dprintf(x...)

#if defined(LF_SYS_NEXXONE_VOIP) || defined(LF_SYS_NEXX360H) || defined(LF_SYS_NEXXB_ONE)
#define VPS_FPS				30
static int vid_port[] = {
	SYSTEM_CAPTURE_INST_VIP1_PORTB,		//# (1)
	SYSTEM_CAPTURE_INST_VIP0_PORTA,		//# (4)
	SYSTEM_CAPTURE_INST_VIP1_PORTA,		//# (2)
	SYSTEM_CAPTURE_INST_VIP0_PORTB,		//# (3)
};
#else
#define VPS_FPS				15
#endif

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
#if defined(LF_SYS_NEXXONE_VOIP) || defined(LF_SYS_NEXX360H) || defined(LF_SYS_NEXXB_ONE)
static void	capture_link_params_init(CaptureLink_CreateParams *capturePrm, int num_ch)
{
	CaptureLink_VipInstParams	*pCaptureInstPrm;
	CaptureLink_OutParams		*pCaptureOutPrm;
	int i;

	CaptureLink_CreateParams_Init(capturePrm);

	capturePrm->tilerEnable		= FALSE;
	capturePrm->numBufsPerCh	= NUM_CAPTURE_BUFFERS;
	capturePrm->enableSdCrop	= FALSE;
	capturePrm->isPalMode		= FALSE;
	capturePrm->numVipInst		= num_ch;

	for(i=0; i<num_ch; i++)
	{
		pCaptureInstPrm						= &capturePrm->vipInst[i];
		pCaptureInstPrm->vipInstId			= vid_port[i];
		pCaptureInstPrm->videoDecoderId		= gVsysModuleContext.vsysConfig.decoderHD;
		pCaptureInstPrm->captureMode		= SYSTEM_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;
		pCaptureInstPrm->inDataFormat		= SYSTEM_DF_YUV422P;
		pCaptureInstPrm->standard			= SYSTEM_STD_720P_60;
		pCaptureInstPrm->serdesEQ			= gVsysModuleContext.vsysConfig.serdesEQ;
		pCaptureInstPrm->numOutput			= 1;

		pCaptureOutPrm						= &pCaptureInstPrm->outParams[0];
		pCaptureOutPrm->dataFormat			= SYSTEM_DF_YUV422I_YUYV;//SYSTEM_DF_YUV420SP_UV;
		pCaptureOutPrm->scEnable			= FALSE;
		pCaptureOutPrm->scOutWidth			= 1280;
		pCaptureOutPrm->scOutHeight			= 720;
		pCaptureOutPrm->outQueId			= 0;
	}
}
#else
static void	capture_link_params_init(CaptureLink_CreateParams *capturePrm, int num_ch, int mode)
{
	CaptureLink_VipInstParams	*pCaptureInstPrm;
	CaptureLink_OutParams		*pCaptureOutPrm;
	int i;

	CaptureLink_CreateParams_Init(capturePrm);

	capturePrm->tilerEnable		= FALSE;
	capturePrm->numBufsPerCh	= NUM_CAPTURE_BUFFERS;
	capturePrm->enableSdCrop	= FALSE;
	capturePrm->isPalMode		= FALSE;
	capturePrm->numVipInst		= num_ch;

	for(i=0; i<num_ch; i++)
	{
		pCaptureInstPrm		= &capturePrm->vipInst[i];
		switch (i) {
		case 0 :
			pCaptureInstPrm->vipInstId	= SYSTEM_CAPTURE_INST_VIP1_PORTB ;
			break ;
		case 1 :
			pCaptureInstPrm->vipInstId	= SYSTEM_CAPTURE_INST_VIP0_PORTA ;
			break ;
		case 2 :
			if(mode) {
				 // fitt360
				pCaptureInstPrm->vipInstId  = SYSTEM_CAPTURE_INST_VIP1_PORTA ;
			} else
				pCaptureInstPrm->vipInstId  = SYSTEM_CAPTURE_INST_VIP0_PORTB ;
			break ;
		case 3 :
			if(mode) {
				 // fitt360
				pCaptureInstPrm->vipInstId			= SYSTEM_CAPTURE_INST_VIP0_PORTB ;
			} else 
				pCaptureInstPrm->vipInstId			= SYSTEM_CAPTURE_INST_VIP1_PORTA ;

			break ;

		default :
			break ;
		}

		pCaptureInstPrm->videoDecoderId		= gVsysModuleContext.vsysConfig.decoderHD;
		pCaptureInstPrm->captureMode		= SYSTEM_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;
		pCaptureInstPrm->inDataFormat		= SYSTEM_DF_YUV422P;
		pCaptureInstPrm->standard			= SYSTEM_STD_720P_60;
		pCaptureInstPrm->serdesEQ			= gVsysModuleContext.vsysConfig.serdesEQ;
		pCaptureInstPrm->numOutput			= 1;

		pCaptureOutPrm						= &pCaptureInstPrm->outParams[0];
		pCaptureOutPrm->dataFormat			= SYSTEM_DF_YUV422I_YUYV;//SYSTEM_DF_YUV420SP_UV;
		pCaptureOutPrm->scEnable			= FALSE;
		pCaptureOutPrm->scOutWidth			= 1280;
		pCaptureOutPrm->scOutHeight			= 720;
		pCaptureOutPrm->outQueId			= 0;
	}
}
#endif

static void	nsf_link_params_init(NsfLink_CreateParams *nsfPrm)
{
	NsfLink_CreateParams_Init(nsfPrm);

	nsfPrm->tilerEnable			= FALSE;
	nsfPrm->numOutQue			= 1;
	nsfPrm->numBufsPerCh		= NUM_NSF_BUFFERS;
	nsfPrm->inputFrameRate		= VPS_FPS;
	nsfPrm->outputFrameRate		= VPS_FPS;
}

static void	null_link_params_init(NullLink_CreateParams *nullPrm, UInt32 prev_id, UInt32 prev_que_id)
{
	nullPrm->numInQue						= 1;
	nullPrm->inQueParams[0].prevLinkId		= prev_id;
	nullPrm->inQueParams[0].prevLinkQueId	= prev_que_id;
}

#if defined(LF_SYS_NEXXONE_VOIP)
static void	enc_link_params_init(EncLink_CreateParams *encPrm)
{
	EncLink_ChCreateParams *pLinkChPrm;
	EncLink_ChDynamicParams	*pLinkDynPrm;
	VENC_CHN_DYNAMIC_PARAM_S *pDynPrm;
	VENC_CHN_PARAMS_S *pChPrm;
	UInt32 i ;

	EncLink_CreateParams_Init(encPrm);

	encPrm->numCh = gVsysModuleContext.vsysConfig.numChs + 3;

	encPrm->numBufPerCh[0]	= NUM_ENCODE_BUFFERS;

	for	(i=0; i<encPrm->numCh; i++)
	{
		if(i < 2)
		{
			pLinkChPrm	= &encPrm->chCreateParams[i];
			pLinkDynPrm	= &pLinkChPrm->defaultDynamicParams;

			pChPrm		= &gVencModuleContext.vencConfig.encChannelParams[i];
			pDynPrm		= &pChPrm->dynamicParam;

			pLinkChPrm->format					= IVIDEO_H264HP;
			pLinkChPrm->profile					= gVencModuleContext.vencConfig.h264Profile[i];
			pLinkChPrm->dataLayout				= IVIDEO_PROGRESSIVE;
			pLinkChPrm->fieldMergeEncodeEnable	= FALSE;
			pLinkChPrm->enableAnalyticinfo		= pChPrm->enableAnalyticinfo;
			pLinkChPrm->maxBitRate				= pChPrm->maxBitRate;
			pLinkChPrm->encodingPreset			= pChPrm->encodingPreset;
			pLinkChPrm->rateControlPreset		= pChPrm->rcType;
			pLinkChPrm->enableHighSpeed         = FALSE;
			pLinkChPrm->StreamPreset            = 1;  //  1  ALG_VID_ENC_PRESET_HIGHSPEED  3 ALG_VID_ENC_PRESET_SVC_AUTO 

	      	pLinkDynPrm->outputFrameRate		= pDynPrm->frameRate * 1000;
		    pLinkDynPrm->intraFrameInterval		= pDynPrm->intraFrameInterval;
			pLinkDynPrm->targetBitRate			= pDynPrm->targetBitRate;
			pLinkDynPrm->interFrameInterval		= 1;
			pLinkDynPrm->mvAccuracy				= IVIDENC2_MOTIONVECTOR_QUARTERPEL;
			pLinkDynPrm->inputFrameRate			= pDynPrm->inputFrameRate;
			pLinkDynPrm->rcAlg					= pDynPrm->rcAlg;
			pLinkDynPrm->qpMin					= pDynPrm->qpMin;
			pLinkDynPrm->qpMax					= pDynPrm->qpMax;
			pLinkDynPrm->qpInit					= pDynPrm->qpInit;
			pLinkDynPrm->vbrDuration			= pDynPrm->vbrDuration;
			pLinkDynPrm->vbrSensitivity			= pDynPrm->vbrSensitivity;

			gVencModuleContext.encFormat[i]		= pLinkChPrm->format;
		}
		else // mjpeg
		{
			pLinkChPrm	= &encPrm->chCreateParams[i];
			pLinkDynPrm = &pLinkChPrm->defaultDynamicParams;

			pChPrm		= &gVencModuleContext.vencConfig.encChannelParams[i];
			pDynPrm		= &pChPrm->dynamicParam;

			pLinkChPrm->format					= IVIDEO_MJPEG;
			pLinkChPrm->profile					= 0;
			pLinkChPrm->dataLayout				= IVIDEO_PROGRESSIVE;
			pLinkChPrm->fieldMergeEncodeEnable	= FALSE;
			pLinkChPrm->enableAnalyticinfo		= 0;
			pLinkChPrm->maxBitRate				= 0;
			pLinkChPrm->encodingPreset			= 0;
			pLinkChPrm->rateControlPreset		= 0;


			pLinkDynPrm->outputFrameRate		= pDynPrm->frameRate * 1000;
			pLinkDynPrm->intraFrameInterval		= 1;
			pLinkDynPrm->targetBitRate			= 100*1000;
			pLinkDynPrm->interFrameInterval		= 1;
			pLinkDynPrm->mvAccuracy				= 0;
//			pLinkDynPrm->inputFrameRate			= pDynPrm->inputFrameRate;
			pLinkDynPrm->inputFrameRate			= VPS_FPS;
			pLinkDynPrm->qpMin					= 0;
			pLinkDynPrm->qpMax					= 0;
			pLinkDynPrm->qpInit					= 0;
			pLinkDynPrm->vbrDuration			= 0;
			pLinkDynPrm->vbrSensitivity			= 0;
			
		}
	}
}

#else
static void	enc_link_params_init(EncLink_CreateParams *encPrm)
{
	EncLink_ChCreateParams *pLinkChPrm;
	EncLink_ChDynamicParams	*pLinkDynPrm;
	VENC_CHN_DYNAMIC_PARAM_S *pDynPrm;
	VENC_CHN_PARAMS_S *pChPrm;
	UInt32 i, vidch;

#if defined(LF_SYS_NEXX360W_MUX)
    vidch = 1 ;
#else
	vidch = gVsysModuleContext.vsysConfig.numChs;
#endif

	EncLink_CreateParams_Init(encPrm);

	encPrm->numCh = vidch + 1;

	encPrm->numBufPerCh[0]	= NUM_ENCODE_BUFFERS;

	for	(i=0; i<encPrm->numCh; i++)
	{
		pLinkChPrm	= &encPrm->chCreateParams[i];
		pLinkDynPrm	= &pLinkChPrm->defaultDynamicParams;

		pChPrm		= &gVencModuleContext.vencConfig.encChannelParams[i];
		pDynPrm		= &pChPrm->dynamicParam;

		pLinkChPrm->format					= IVIDEO_H264HP;
		pLinkChPrm->profile					= gVencModuleContext.vencConfig.h264Profile[i];
		pLinkChPrm->dataLayout				= IVIDEO_PROGRESSIVE;
		pLinkChPrm->fieldMergeEncodeEnable	= FALSE;
		pLinkChPrm->enableAnalyticinfo		= pChPrm->enableAnalyticinfo;
		pLinkChPrm->maxBitRate				= pChPrm->maxBitRate;
		pLinkChPrm->encodingPreset			= pChPrm->encodingPreset;
		pLinkChPrm->rateControlPreset		= pChPrm->rcType;
		pLinkChPrm->enableHighSpeed         = FALSE;
		pLinkChPrm->StreamPreset            = 1;  //  1  ALG_VID_ENC_PRESET_HIGHSPEED  3 ALG_VID_ENC_PRESET_SVC_AUTO 

      	pLinkDynPrm->outputFrameRate		= pDynPrm->frameRate * 1000;
	    pLinkDynPrm->intraFrameInterval		= pDynPrm->intraFrameInterval;
		pLinkDynPrm->targetBitRate			= pDynPrm->targetBitRate;
		pLinkDynPrm->interFrameInterval		= 1;
		pLinkDynPrm->mvAccuracy				= IVIDENC2_MOTIONVECTOR_QUARTERPEL;
		pLinkDynPrm->inputFrameRate			= pDynPrm->inputFrameRate;
		pLinkDynPrm->rcAlg					= pDynPrm->rcAlg;
		pLinkDynPrm->qpMin					= pDynPrm->qpMin;
		pLinkDynPrm->qpMax					= pDynPrm->qpMax;
		pLinkDynPrm->qpInit					= pDynPrm->qpInit;
		pLinkDynPrm->vbrDuration			= pDynPrm->vbrDuration;
		pLinkDynPrm->vbrSensitivity			= pDynPrm->vbrSensitivity;

		gVencModuleContext.encFormat[i]		= pLinkChPrm->format;
	}

	//#	MJPEG
	if(gVsysModuleContext.vsysConfig.enableMjpeg)
	{
		i = encPrm->numCh ;
		{
			pLinkChPrm	= &encPrm->chCreateParams[i];
			pLinkDynPrm = &pLinkChPrm->defaultDynamicParams;

			pChPrm		= &gVencModuleContext.vencConfig.encChannelParams[i];
			pDynPrm		= &pChPrm->dynamicParam;

			pLinkChPrm->format					= IVIDEO_MJPEG;
			pLinkChPrm->profile					= 0;
			pLinkChPrm->dataLayout				= IVIDEO_PROGRESSIVE;
			pLinkChPrm->fieldMergeEncodeEnable	= FALSE;
			pLinkChPrm->enableAnalyticinfo		= 0;
			pLinkChPrm->maxBitRate				= 0;
			pLinkChPrm->encodingPreset			= 0;
			pLinkChPrm->rateControlPreset		= 0;


			pLinkDynPrm->outputFrameRate		= pDynPrm->frameRate * 1000;
			pLinkDynPrm->intraFrameInterval		= 1;
			pLinkDynPrm->targetBitRate			= 100*1000;
			pLinkDynPrm->interFrameInterval		= 1;
			pLinkDynPrm->mvAccuracy				= 0;
//			pLinkDynPrm->inputFrameRate			= pDynPrm->inputFrameRate;
			pLinkDynPrm->inputFrameRate			= VPS_FPS;
			pLinkDynPrm->qpMin					= 0;
			pLinkDynPrm->qpMax					= 0;
			pLinkDynPrm->qpInit					= 0;
			pLinkDynPrm->vbrDuration			= 0;
			pLinkDynPrm->vbrSensitivity			= 0;
		}
	}
}
#endif

static void	swms_link_params_init(SwMsLink_CreateParams	*swMsPrm, int dev_id, int win_num)
{
	SwMsLink_CreateParams_Init(swMsPrm);

	swMsPrm->numSwMsInst			= 1;
	swMsPrm->swMsInstId[0]			= SYSTEM_SW_MS_SC_INST_DEI_SC_NO_DEI;
//	swMsPrm->swMsInstId[0]			= SYSTEM_SW_MS_SC_INST_SC5;
	swMsPrm->maxInputQueLen			= 4;
	swMsPrm->numOutBuf				= NUM_SWMS_BUFFERS;
	swMsPrm->maxOutRes				= gVdisModuleContext.vdisConfig.deviceParams[dev_id].resolution;
	swMsPrm->lineSkipMode			= FALSE;
	swMsPrm->enableLayoutGridDraw	= FALSE;

	MultiCh_swMsGetUserLayoutPrm(dev_id, swMsPrm, win_num);
}

static void	disp_link_pararms_init(DisplayLink_CreateParams *dispPrm, int dev_id)
{
	DisplayLink_CreateParams_Init(dispPrm);

	dispPrm->displayRes = gVdisModuleContext.vdisConfig.deviceParams[dev_id].resolution;
}

/*----------------------------------------------------------------------------
 encoder & ipc link create/delete function
-----------------------------------------------------------------------------*/
static void encoder_link_create(UInt32 prevLinkId, UInt32 prevLinkQueId)
{
	SwosdLink_CreateParams			swosdPrm;
	IpcLink_CreateParams			ipcOutVpssPrm;
	IpcLink_CreateParams 			ipcInVideoPrm;
	EncLink_CreateParams			encPrm;
	IpcBitsOutLinkRTOS_CreateParams	ipcBitsOutVideoPrm;
	IpcBitsInLinkHLOS_CreateParams	ipcBitsInHostPrm0;

	UInt32 ipcOutVpssId, ipcInVideoId;

	//# link IDs
	gVsysModuleContext.swOsdId				= SYSTEM_LINK_ID_SWOSD_0;
	ipcOutVpssId 							= SYSTEM_VPSS_LINK_ID_IPC_OUT_M3_0;
	ipcInVideoId 							= SYSTEM_VIDEO_LINK_ID_IPC_IN_M3_0;
	gVencModuleContext.encId				= SYSTEM_LINK_ID_VENC_0;
	gVencModuleContext.ipcBitsOutRTOSId		= SYSTEM_VIDEO_LINK_ID_IPC_BITS_OUT_0;
	gVencModuleContext.ipcBitsInHLOSId		= SYSTEM_HOST_LINK_ID_IPC_BITS_IN_0;

	//#--- swOsd link params
	swosdPrm.inQueParams.prevLinkId 	= prevLinkId;
	swosdPrm.inQueParams.prevLinkQueId	= prevLinkQueId;
	swosdPrm.frameSync 					= 0;
	swosdPrm.frameSycChId				= 0;
	swosdPrm.outQueParams.nextLink		= ipcOutVpssId;
	//--------------------------------------------------------------------------
	//# IPC struct init
	MULTICH_INIT_STRUCT(IpcLink_CreateParams, ipcOutVpssPrm);
	MULTICH_INIT_STRUCT(IpcLink_CreateParams, ipcInVideoPrm);
	MULTICH_INIT_STRUCT(IpcBitsOutLinkRTOS_CreateParams, ipcBitsOutVideoPrm);
	MULTICH_INIT_STRUCT(IpcBitsInLinkHLOS_CreateParams, ipcBitsInHostPrm0);

	//#--- ipcOutVpss link params
	ipcOutVpssPrm.inQueParams.prevLinkId 	= gVsysModuleContext.swOsdId;
	ipcOutVpssPrm.inQueParams.prevLinkQueId	= 0;
	ipcOutVpssPrm.numOutQue					= 1;
	ipcOutVpssPrm.notifyNextLink			= TRUE;
	ipcOutVpssPrm.notifyPrevLink			= TRUE;
	ipcOutVpssPrm.noNotifyMode				= FALSE;
	ipcOutVpssPrm.outQueParams[0].nextLink	= ipcInVideoId;

	//#--- ipcInVideo link params
	ipcInVideoPrm.inQueParams.prevLinkId	= ipcOutVpssId;
	ipcInVideoPrm.inQueParams.prevLinkQueId	= 0;
	ipcInVideoPrm.numOutQue					= 1;
	ipcInVideoPrm.notifyNextLink			= TRUE;
	ipcInVideoPrm.notifyPrevLink			= TRUE;
	ipcInVideoPrm.noNotifyMode				= FALSE;
	ipcInVideoPrm.outQueParams[0].nextLink	= gVencModuleContext.encId;

	//#--- encoder link params
	MULTICH_INIT_STRUCT(EncLink_CreateParams, encPrm);
	enc_link_params_init(&encPrm);
	encPrm.inQueParams.prevLinkId		= ipcInVideoId;
	encPrm.inQueParams.prevLinkQueId	= 0;
	encPrm.outQueParams.nextLink		= gVencModuleContext.ipcBitsOutRTOSId;

	//#--- ipcBitsOutVideo link params
	ipcBitsOutVideoPrm.baseCreateParams.inQueParams.prevLinkId = gVencModuleContext.encId;
	ipcBitsOutVideoPrm.baseCreateParams.inQueParams.prevLinkQueId = 0;
	ipcBitsOutVideoPrm.baseCreateParams.numOutQue = 1;
	ipcBitsOutVideoPrm.baseCreateParams.outQueParams[0].nextLink = gVencModuleContext.ipcBitsInHLOSId;
	MultiCh_ipcBitsInitCreateParams_BitsOutRTOS(&ipcBitsOutVideoPrm, TRUE);

	//#--- ipcBitsInHost link params
	ipcBitsInHostPrm0.baseCreateParams.inQueParams.prevLinkId = gVencModuleContext.ipcBitsOutRTOSId;
	ipcBitsInHostPrm0.baseCreateParams.inQueParams.prevLinkQueId = 0;
	MultiCh_ipcBitsInitCreateParams_BitsInHLOS(&ipcBitsInHostPrm0);

	//#--- encoder link creation -------------------------------------
	System_linkCreate(gVsysModuleContext.swOsdId, &swosdPrm, sizeof(swosdPrm));
	System_linkCreate(ipcOutVpssId, &ipcOutVpssPrm, sizeof(ipcOutVpssPrm));
	System_linkCreate(ipcInVideoId, &ipcInVideoPrm, sizeof(ipcInVideoPrm));
	System_linkCreate(gVencModuleContext.encId, &encPrm, sizeof(encPrm));
	System_linkCreate(gVencModuleContext.ipcBitsOutRTOSId, &ipcBitsOutVideoPrm, sizeof(ipcBitsOutVideoPrm));
	System_linkCreate(gVencModuleContext.ipcBitsInHLOSId, &ipcBitsInHostPrm0, sizeof(ipcBitsInHostPrm0));
}

static void encoder_link_delete(void)
{
	System_linkDelete(gVencModuleContext.ipcBitsInHLOSId);
	System_linkDelete(gVencModuleContext.ipcBitsOutRTOSId);
	System_linkDelete(gVencModuleContext.encId);
	System_linkDelete(SYSTEM_VIDEO_LINK_ID_IPC_IN_M3_0);
	System_linkDelete(SYSTEM_VPSS_LINK_ID_IPC_OUT_M3_0);
	System_linkDelete(gVsysModuleContext.swOsdId);
}

#if 0
/*----------------------------------------------------------------------------
 display link create/delete function
-----------------------------------------------------------------------------*/
static void display_link_create(UInt32 prevLinkId, UInt32 prevLinkQueId, int layout)
{
	SwMsLink_CreateParams		swMsPrm;
	DisplayLink_CreateParams	dispPrm, dispPrm1;

	//# link IDs
	gVdisModuleContext.swMsId[0]				= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
	gVdisModuleContext.displayId[VDIS_DEV_HDMI]	= SYSTEM_LINK_ID_DISPLAY_0;
	gVdisModuleContext.displayId[VDIS_DEV_SD]	= SYSTEM_LINK_ID_DISPLAY_2;

	if(gVsysModuleContext.vsysConfig.enableHDMI)
	{
		//#--- swMs Link Params
		swms_link_params_init(&swMsPrm, VDIS_DEV_HDMI, layout);
		swMsPrm.inQueParams.prevLinkId		= prevLinkId;
		swMsPrm.inQueParams.prevLinkQueId	= prevLinkQueId;
		swMsPrm.outQueParams.nextLink		= gVdisModuleContext.displayId[VDIS_DEV_HDMI];

		//#--- display link params (HDMI-out)
		disp_link_pararms_init(&dispPrm1, VDIS_DEV_HDMI);
		dispPrm1.inQueParams[0].prevLinkId		= gVdisModuleContext.swMsId[0];
		dispPrm1.inQueParams[0].prevLinkQueId 	= 0;

		//#--- Display link creation -------------------------------------
		System_linkCreate(gVdisModuleContext.swMsId[0], &swMsPrm, sizeof(swMsPrm));
		System_linkCreate(gVdisModuleContext.displayId[VDIS_DEV_HDMI], &dispPrm1, sizeof(dispPrm1));
	}
	else
	{
		//#--- swMs Link Params
		swms_link_params_init(&swMsPrm, VDIS_DEV_SD, layout);
		swMsPrm.inQueParams.prevLinkId		= prevLinkId;
		swMsPrm.inQueParams.prevLinkQueId	= prevLinkQueId;
		swMsPrm.outQueParams.nextLink		= gVdisModuleContext.displayId[VDIS_DEV_SD];

		//#--- display link params (TV-out)
		disp_link_pararms_init(&dispPrm, VDIS_DEV_SD);
		dispPrm.inQueParams[0].prevLinkId	= gVdisModuleContext.swMsId[0];
		dispPrm.inQueParams[0].prevLinkQueId 	= 0;

		//#--- Display link creation -------------------------------------
		System_linkCreate(gVdisModuleContext.swMsId[0], &swMsPrm, sizeof(swMsPrm));
		System_linkCreate(gVdisModuleContext.displayId[VDIS_DEV_SD], &dispPrm, sizeof(dispPrm));
	}
}
#endif

static void display_link_delete(void)
{
	if(gVsysModuleContext.vsysConfig.enableHDMI) {
		System_linkDelete(gVdisModuleContext.displayId[VDIS_DEV_HDMI]);
	} else {
		System_linkDelete(gVdisModuleContext.displayId[VDIS_DEV_SD]);
	}
	System_linkDelete(gVdisModuleContext.swMsId[0]);
}

/*****************************************************************************
* @brief	mcfw_capture_display_init function for ONE Channel
* @section	DESC Description
*	- desc : HD 2ch, D1 8ch
*****************************************************************************/
#if defined(LF_SYS_NEXXONE_VOIP) || defined(LF_SYS_NEXX360H) || defined(LF_SYS_NEXXB_ONE)
void mcfw_capture_display_init(void)
{
	CaptureLink_CreateParams		capturePrm;
	NsfLink_CreateParams			nsfPrm, nsfPrm1;
	DupLink_CreateParams			dupPrm0, dupPrm1;
	MergeLink_CreateParams			mergePrm0;
	NullLink_CreateParams			nullPrm0;
    SwMsLink_CreateParams           swMsPrm;
	DisplayLink_CreateParams    	dispPrm ;

	int num_ch = gVsysModuleContext.vsysConfig.numChs;

	dprintf("%s: num_ch %d\n", __func__, num_ch);

	//#	link IDs
	gVcapModuleContext.captureId	= SYSTEM_LINK_ID_CAPTURE;
	gVcapModuleContext.nsfId[0]		= SYSTEM_LINK_ID_NSF_0;
	gVcapModuleContext.nsfId[1]		= SYSTEM_LINK_ID_NSF_1;
	gVdisModuleContext.swMsId[0]                = SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
	gVdisModuleContext.displayId[VDIS_DEV_HDMI] = SYSTEM_LINK_ID_DISPLAY_0;
	gVdisModuleContext.displayId[VDIS_DEV_SD]   = SYSTEM_LINK_ID_DISPLAY_2;

	//#-------------------------------------------
	//# Capture link
	//#-------------------------------------------
	//#--- capture link params
	capture_link_params_init(&capturePrm, num_ch);
	capturePrm.outQueParams[0].nextLink	= gVcapModuleContext.nsfId[0];

	//#--- nsf link params
	nsf_link_params_init(&nsfPrm);
	nsfPrm.inQueParams.prevLinkId		= gVcapModuleContext.captureId;
	nsfPrm.inQueParams.prevLinkQueId	= 0;
    nsfPrm.outQueParams[0].nextLink		= SYSTEM_VPSS_LINK_ID_DUP_0;
	
	//#--- dup link params
	dupPrm0.inQueParams.prevLinkId		= gVcapModuleContext.nsfId[0];
	dupPrm0.inQueParams.prevLinkQueId	= 0;
    if(gVsysModuleContext.vsysConfig.enableMjpeg)
		dupPrm0.numOutQue					= 3;
	else
		dupPrm0.numOutQue					= 2;
	dupPrm0.notifyNextLink				= TRUE;

	if(gVsysModuleContext.vsysConfig.enableHDMI)
	{
        dupPrm0.outQueParams[0].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
        dupPrm0.outQueParams[1].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
  	    dupPrm0.outQueParams[2].nextLink	= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;

        swms_link_params_init(&swMsPrm, VDIS_DEV_HDMI, num_ch);
        swMsPrm.inQueParams.prevLinkId      = SYSTEM_VPSS_LINK_ID_DUP_0;
        swMsPrm.inQueParams.prevLinkQueId   = 2;
	    swMsPrm.outQueParams.nextLink       = SYSTEM_VPSS_LINK_ID_DUP_1;

        dupPrm1.inQueParams.prevLinkId		= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
	    dupPrm1.inQueParams.prevLinkQueId	= 0;
	    dupPrm1.numOutQue					= 2;
	    dupPrm1.notifyNextLink				= TRUE;
	    dupPrm1.outQueParams[0].nextLink	= gVdisModuleContext.displayId[VDIS_DEV_HDMI];
	    dupPrm1.outQueParams[1].nextLink	= gVcapModuleContext.nsfId[1];

        nsf_link_params_init(&nsfPrm1);
        nsfPrm1.inQueParams.prevLinkId       = SYSTEM_VPSS_LINK_ID_DUP_1;
        nsfPrm1.inQueParams.prevLinkQueId    = 1;

        if(gVsysModuleContext.vsysConfig.enableMjpeg)
        {
			nsfPrm1.outQueParams[0].nextLink    = SYSTEM_VPSS_LINK_ID_MERGE_0;
            mergePrm0.numInQue = 3;
		    mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_0;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
		    mergePrm0.inQueParams[1].prevLinkId     = gVcapModuleContext.nsfId[1];
            mergePrm0.inQueParams[1].prevLinkQueId  = 0;
            mergePrm0.inQueParams[2].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_0;
            mergePrm0.inQueParams[2].prevLinkQueId  = 1;
        }
		else
		{
		    nsfPrm1.outQueParams[0].nextLink     = SYSTEM_VPSS_LINK_ID_MERGE_0;

            mergePrm0.numInQue = 2;
            mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_0;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
            mergePrm0.inQueParams[1].prevLinkId     = gVcapModuleContext.nsfId[1];
            mergePrm0.inQueParams[1].prevLinkQueId  = 0;   
		}	
		mergePrm0.notifyNextLink  				= TRUE;

	    if(gVsysModuleContext.vsysConfig.enableEncode) {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_LINK_ID_SWOSD_0; //SYSTEM_VPSS_LINK_ID_IPC_OUT_M3_0;
		} else {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_VPSS_LINK_ID_NULL_0;
			null_link_params_init(&nullPrm0, SYSTEM_VPSS_LINK_ID_MERGE_0, 0);
		}

        disp_link_pararms_init(&dispPrm, VDIS_DEV_HDMI);
		dispPrm.inQueParams[0].prevLinkId	= SYSTEM_VPSS_LINK_ID_DUP_1;
	    dispPrm.inQueParams[0].prevLinkQueId 	= 0;

	    //#--- capture link
	    System_linkCreate(gVcapModuleContext.captureId, &capturePrm, sizeof(capturePrm));
	    System_linkCreate(gVcapModuleContext.nsfId[0], &nsfPrm,	sizeof(nsfPrm));
        System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_0, &dupPrm0, sizeof(dupPrm0));

        //#--- Display link creation -------------------------------------

        System_linkCreate(gVdisModuleContext.swMsId[0], &swMsPrm, sizeof(swMsPrm));
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_1, &dupPrm1, sizeof(dupPrm1));
	    System_linkCreate(gVcapModuleContext.nsfId[1], &nsfPrm1, sizeof(nsfPrm1));

	    System_linkCreate(SYSTEM_VPSS_LINK_ID_MERGE_0, &mergePrm0, sizeof(mergePrm0));

	    //#--- encoder link
		if(gVsysModuleContext.vsysConfig.enableEncode) {
	    	encoder_link_create(SYSTEM_VPSS_LINK_ID_MERGE_0, 0);

		} else {
			System_linkCreate(SYSTEM_VPSS_LINK_ID_NULL_0, &nullPrm0, sizeof(nullPrm0));
		}

	    //#--- display link
        System_linkCreate(gVdisModuleContext.displayId[VDIS_DEV_HDMI], &dispPrm, sizeof(dispPrm));
//	    display_link_create(SYSTEM_VPSS_LINK_ID_DUP_0, 1, num_ch /* or 1 */);
	    dprintf("%s done!\n", __func__);
	}
	else
	{
        dupPrm0.outQueParams[0].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
        dupPrm0.outQueParams[1].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
  	    dupPrm0.outQueParams[2].nextLink	= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;

        swms_link_params_init(&swMsPrm, VDIS_DEV_SD, num_ch);
        swMsPrm.inQueParams.prevLinkId      = SYSTEM_VPSS_LINK_ID_DUP_0;
        swMsPrm.inQueParams.prevLinkQueId   = 2;
	    swMsPrm.outQueParams.nextLink       = SYSTEM_VPSS_LINK_ID_DUP_1;

        dupPrm1.inQueParams.prevLinkId		= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
	    dupPrm1.inQueParams.prevLinkQueId	= 0;
	    dupPrm1.numOutQue					= 2;
	    dupPrm1.notifyNextLink				= TRUE;
	    dupPrm1.outQueParams[0].nextLink	= gVdisModuleContext.displayId[VDIS_DEV_SD];
	    dupPrm1.outQueParams[1].nextLink	= gVcapModuleContext.nsfId[1];

        nsf_link_params_init(&nsfPrm1);
        nsfPrm1.inQueParams.prevLinkId       = SYSTEM_VPSS_LINK_ID_DUP_1;
        nsfPrm1.inQueParams.prevLinkQueId    = 1;
		
        if(gVsysModuleContext.vsysConfig.enableMjpeg)
        {
			nsfPrm1.outQueParams[0].nextLink    = SYSTEM_VPSS_LINK_ID_MERGE_0;
            mergePrm0.numInQue = 3;
		    mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_0;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
		    mergePrm0.inQueParams[1].prevLinkId     = gVcapModuleContext.nsfId[1];
            mergePrm0.inQueParams[1].prevLinkQueId  = 0;
            mergePrm0.inQueParams[2].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_0;
            mergePrm0.inQueParams[2].prevLinkQueId  = 1;
        }
		else
		{
		    nsfPrm1.outQueParams[0].nextLink     = SYSTEM_VPSS_LINK_ID_MERGE_0;

            mergePrm0.numInQue = 2;
            mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_0;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
            mergePrm0.inQueParams[1].prevLinkId     = gVcapModuleContext.nsfId[1];
            mergePrm0.inQueParams[1].prevLinkQueId  = 0;
		}	
		mergePrm0.notifyNextLink  				= TRUE;

	    if(gVsysModuleContext.vsysConfig.enableEncode) {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_LINK_ID_SWOSD_0; //SYSTEM_VPSS_LINK_ID_IPC_OUT_M3_0;
		} else {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_VPSS_LINK_ID_NULL_0;
			null_link_params_init(&nullPrm0, SYSTEM_VPSS_LINK_ID_MERGE_0, 0);
		}

        disp_link_pararms_init(&dispPrm, VDIS_DEV_SD);
        dispPrm.inQueParams[0].prevLinkId	= SYSTEM_VPSS_LINK_ID_DUP_1;
	    dispPrm.inQueParams[0].prevLinkQueId 	= 0;

	    //#--- capture link
	    System_linkCreate(gVcapModuleContext.captureId, &capturePrm, sizeof(capturePrm));
	    System_linkCreate(gVcapModuleContext.nsfId[0], &nsfPrm,	sizeof(nsfPrm));
		System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_0, &dupPrm0, sizeof(dupPrm0));

	 	//#--- Display link creation -------------------------------------
        System_linkCreate(gVdisModuleContext.swMsId[0], &swMsPrm, sizeof(swMsPrm));
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_1, &dupPrm1, sizeof(dupPrm1));
	    System_linkCreate(gVcapModuleContext.nsfId[1], &nsfPrm1, sizeof(nsfPrm1));

	    System_linkCreate(SYSTEM_VPSS_LINK_ID_MERGE_0, &mergePrm0, sizeof(mergePrm0));

	    //#--- encoder link
		if(gVsysModuleContext.vsysConfig.enableEncode) {
	    	encoder_link_create(SYSTEM_VPSS_LINK_ID_MERGE_0, 0);

		} else {
			System_linkCreate(SYSTEM_VPSS_LINK_ID_NULL_0, &nullPrm0, sizeof(nullPrm0));
		}

	    //#--- display link
        System_linkCreate(gVdisModuleContext.displayId[VDIS_DEV_SD], &dispPrm, sizeof(dispPrm));
//	    display_link_create(SYSTEM_VPSS_LINK_ID_DUP_0, 1, num_ch /* or 1 */);
	    dprintf("%s done!\n", __func__);
	}

}

#elif defined(LF_SYS_NEXX360W_MUX)
/*****************************************************************************
* @brief	mcfw_capture_display_init function for 1 channel mux capture.
* @section	DESC Description
*	- desc : HD 2ch, D1 8ch
*****************************************************************************/
void mcfw_capture_display_init(int mode)
{
	CaptureLink_CreateParams		capturePrm;
	NsfLink_CreateParams			nsfPrm, nsfPrm1;
	DupLink_CreateParams			dupPrm0, dupPrm1, dupPrm2;
	MergeLink_CreateParams			mergePrm0;
	NullLink_CreateParams			nullPrm0;
    SwMsLink_CreateParams           swMsPrm;
	DisplayLink_CreateParams    	dispPrm ;

	int num_ch = gVsysModuleContext.vsysConfig.numChs;

	dprintf("%s: num_ch %d\n", __func__, num_ch);

	//#	link IDs
	gVcapModuleContext.captureId	= SYSTEM_LINK_ID_CAPTURE;
	gVcapModuleContext.nsfId[0]		= SYSTEM_LINK_ID_NSF_0;
	gVcapModuleContext.nsfId[1]		= SYSTEM_LINK_ID_NSF_1;
	gVdisModuleContext.swMsId[0]                = SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
	gVdisModuleContext.displayId[VDIS_DEV_HDMI] = SYSTEM_LINK_ID_DISPLAY_0;
	gVdisModuleContext.displayId[VDIS_DEV_SD]   = SYSTEM_LINK_ID_DISPLAY_2;

	//#-------------------------------------------
	//# Capture link
	//#-------------------------------------------
	//#--- capture link params
	capture_link_params_init(&capturePrm, num_ch, mode);
	capturePrm.outQueParams[0].nextLink	= gVcapModuleContext.nsfId[0];

	//#--- nsf link params
	nsf_link_params_init(&nsfPrm);
	nsfPrm.inQueParams.prevLinkId		= gVcapModuleContext.captureId;
	nsfPrm.inQueParams.prevLinkQueId	= 0;
	nsfPrm.outQueParams[0].nextLink		= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;

	if(gVsysModuleContext.vsysConfig.enableHDMI)
	{
        swms_link_params_init(&swMsPrm, VDIS_DEV_HDMI, num_ch);
        swMsPrm.inQueParams.prevLinkId      = gVcapModuleContext.nsfId[0];
        swMsPrm.inQueParams.prevLinkQueId   = 0;
	    swMsPrm.outQueParams.nextLink       = SYSTEM_VPSS_LINK_ID_DUP_0;

		//#--- dup link params
		dupPrm0.inQueParams.prevLinkId		= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
		dupPrm0.inQueParams.prevLinkQueId	= 0;
		dupPrm0.numOutQue					= 2;
		dupPrm0.notifyNextLink				= TRUE;
		dupPrm0.outQueParams[0].nextLink	= gVdisModuleContext.displayId[VDIS_DEV_HDMI];
	    dupPrm0.outQueParams[1].nextLink	= gVcapModuleContext.nsfId[1];

        nsf_link_params_init(&nsfPrm1);
        nsfPrm1.inQueParams.prevLinkId       = SYSTEM_VPSS_LINK_ID_DUP_0;
        nsfPrm1.inQueParams.prevLinkQueId    = 1;
        nsfPrm1.outQueParams[0].nextLink     = SYSTEM_VPSS_LINK_ID_DUP_1;

        dupPrm1.inQueParams.prevLinkId		= gVcapModuleContext.nsfId[1];
	    dupPrm1.inQueParams.prevLinkQueId	= 0;
	    dupPrm1.numOutQue					= 3;
	    dupPrm1.notifyNextLink				= TRUE;
	    dupPrm1.outQueParams[0].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
	    dupPrm1.outQueParams[1].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
        
		if(gVsysModuleContext.vsysConfig.enableMjpeg)
        {
	        dupPrm1.outQueParams[2].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;

            mergePrm0.numInQue = 3;
		    mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_1;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
            mergePrm0.inQueParams[1].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_1;
            mergePrm0.inQueParams[1].prevLinkQueId  = 1;
            mergePrm0.inQueParams[2].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_1;
            mergePrm0.inQueParams[2].prevLinkQueId  = 2;
        }
		else
		{
            dupPrm1.numOutQue					= 2;

            mergePrm0.numInQue = 2;
            mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_1;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
            mergePrm0.inQueParams[1].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_1;
            mergePrm0.inQueParams[1].prevLinkQueId  = 1;   
  
		}	
		mergePrm0.notifyNextLink  				= TRUE;

	    if(gVsysModuleContext.vsysConfig.enableEncode) {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_LINK_ID_SWOSD_0; //SYSTEM_VPSS_LINK_ID_IPC_OUT_M3_0;
		} else {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_VPSS_LINK_ID_NULL_0;
			null_link_params_init(&nullPrm0, SYSTEM_VPSS_LINK_ID_MERGE_0, 0);
		}

        disp_link_pararms_init(&dispPrm, VDIS_DEV_HDMI);
	    dispPrm.inQueParams[0].prevLinkId	= SYSTEM_VPSS_LINK_ID_DUP_0;
	    dispPrm.inQueParams[0].prevLinkQueId 	= 0;

	    //#--- capture link
	    System_linkCreate(gVcapModuleContext.captureId, &capturePrm, sizeof(capturePrm));
	    System_linkCreate(gVcapModuleContext.nsfId[0], &nsfPrm,	sizeof(nsfPrm));
		System_linkCreate(gVdisModuleContext.swMsId[0], &swMsPrm, sizeof(swMsPrm));
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_0, &dupPrm0, sizeof(dupPrm0));

        //#--- Display link creation -------------------------------------
	    System_linkCreate(gVcapModuleContext.nsfId[1], &nsfPrm1, sizeof(nsfPrm1));
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_1, &dupPrm1, sizeof(dupPrm1));
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_MERGE_0, &mergePrm0, sizeof(mergePrm0));
	    //#--- encoder link
		if(gVsysModuleContext.vsysConfig.enableEncode) {
	    	encoder_link_create(SYSTEM_VPSS_LINK_ID_MERGE_0, 0);

		} else {
			System_linkCreate(SYSTEM_VPSS_LINK_ID_NULL_0, &nullPrm0, sizeof(nullPrm0));
		}

	    //#--- display link
        System_linkCreate(gVdisModuleContext.displayId[VDIS_DEV_HDMI], &dispPrm, sizeof(dispPrm));
//	    display_link_create(SYSTEM_VPSS_LINK_ID_DUP_0, 1, num_ch /* or 1 */);
	    dprintf("%s done!\n", __func__);
	}
	else
	{
	    swms_link_params_init(&swMsPrm, VDIS_DEV_SD, num_ch);
        swMsPrm.inQueParams.prevLinkId      = gVcapModuleContext.nsfId[0];
        swMsPrm.inQueParams.prevLinkQueId   = 0;
	    swMsPrm.outQueParams.nextLink       = SYSTEM_VPSS_LINK_ID_DUP_0;

		//#--- dup link params
		dupPrm0.inQueParams.prevLinkId		= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
		dupPrm0.inQueParams.prevLinkQueId	= 0;
		dupPrm0.numOutQue					= 2;
		dupPrm0.notifyNextLink				= TRUE;
		dupPrm0.outQueParams[0].nextLink	= gVdisModuleContext.displayId[VDIS_DEV_SD];
	    dupPrm0.outQueParams[1].nextLink	= gVcapModuleContext.nsfId[1];

        nsf_link_params_init(&nsfPrm1);
        nsfPrm1.inQueParams.prevLinkId       = SYSTEM_VPSS_LINK_ID_DUP_0;
        nsfPrm1.inQueParams.prevLinkQueId    = 1;
        nsfPrm1.outQueParams[0].nextLink     = SYSTEM_VPSS_LINK_ID_DUP_1;

        dupPrm1.inQueParams.prevLinkId		= gVcapModuleContext.nsfId[1];
	    dupPrm1.inQueParams.prevLinkQueId	= 0;
	    dupPrm1.numOutQue					= 3;
	    dupPrm1.notifyNextLink				= TRUE;
	    dupPrm1.outQueParams[0].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
	    dupPrm1.outQueParams[1].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
		
        if(gVsysModuleContext.vsysConfig.enableMjpeg)
        {
	        dupPrm1.outQueParams[2].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;

            mergePrm0.numInQue = 3;
		    mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_1;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
            mergePrm0.inQueParams[1].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_1;
            mergePrm0.inQueParams[1].prevLinkQueId  = 1;
            mergePrm0.inQueParams[2].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_1;
            mergePrm0.inQueParams[2].prevLinkQueId  = 2;
        }
		else
		{
            dupPrm1.numOutQue					= 2;

            mergePrm0.numInQue = 2;
            mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_1;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
            mergePrm0.inQueParams[1].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_1;
            mergePrm0.inQueParams[1].prevLinkQueId  = 1;   
		}	
		mergePrm0.notifyNextLink  				= TRUE;

	    if(gVsysModuleContext.vsysConfig.enableEncode) {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_LINK_ID_SWOSD_0; //SYSTEM_VPSS_LINK_ID_IPC_OUT_M3_0;
		} else {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_VPSS_LINK_ID_NULL_0;
			null_link_params_init(&nullPrm0, SYSTEM_VPSS_LINK_ID_MERGE_0, 0);
		}

        disp_link_pararms_init(&dispPrm, VDIS_DEV_SD);
	    dispPrm.inQueParams[0].prevLinkId	= SYSTEM_VPSS_LINK_ID_DUP_0;
	    dispPrm.inQueParams[0].prevLinkQueId 	= 0;

	    //#--- capture link
	    System_linkCreate(gVcapModuleContext.captureId, &capturePrm, sizeof(capturePrm));
	    System_linkCreate(gVcapModuleContext.nsfId[0], &nsfPrm,	sizeof(nsfPrm));
		System_linkCreate(gVdisModuleContext.swMsId[0], &swMsPrm, sizeof(swMsPrm));
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_0, &dupPrm0, sizeof(dupPrm0));

        //#--- Display link creation -------------------------------------
	    System_linkCreate(gVcapModuleContext.nsfId[1], &nsfPrm1, sizeof(nsfPrm1));
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_1, &dupPrm1, sizeof(dupPrm1));
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_MERGE_0, &mergePrm0, sizeof(mergePrm0));

	    //#--- encoder link
		if(gVsysModuleContext.vsysConfig.enableEncode) {
	    	encoder_link_create(SYSTEM_VPSS_LINK_ID_MERGE_0, 0);

		} else {
			System_linkCreate(SYSTEM_VPSS_LINK_ID_NULL_0, &nullPrm0, sizeof(nullPrm0));
		}

	    //#--- display link
        System_linkCreate(gVdisModuleContext.displayId[VDIS_DEV_SD], &dispPrm, sizeof(dispPrm));
//	    display_link_create(SYSTEM_VPSS_LINK_ID_DUP_0, 1, num_ch /* or 1 */);
	    dprintf("%s done!\n", __func__);
	}
}

#else
/*****************************************************************************
* @brief	mcfw_capture_display_init function for NEXX360W/NEXX360B/NEXXB/NEXX360C/NEXX360W_CCTV
* @section	DESC Description
*****************************************************************************/
void mcfw_capture_display_init(int mode)
{
	CaptureLink_CreateParams		capturePrm;
	NsfLink_CreateParams			nsfPrm, nsfPrm1;
	DupLink_CreateParams			dupPrm0, dupPrm1, dupPrm2;
	MergeLink_CreateParams			mergePrm0;
	NullLink_CreateParams			nullPrm0;
    SwMsLink_CreateParams           swMsPrm;
	DisplayLink_CreateParams    	dispPrm ;

	int num_ch = gVsysModuleContext.vsysConfig.numChs;

	dprintf("%s: num_ch %d\n", __func__, num_ch);

	//#	link IDs
	gVcapModuleContext.captureId	= SYSTEM_LINK_ID_CAPTURE;
	gVcapModuleContext.nsfId[0]		= SYSTEM_LINK_ID_NSF_0;
	gVcapModuleContext.nsfId[1]		= SYSTEM_LINK_ID_NSF_1;
	gVdisModuleContext.swMsId[0]                = SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
	gVdisModuleContext.displayId[VDIS_DEV_HDMI] = SYSTEM_LINK_ID_DISPLAY_0;
	gVdisModuleContext.displayId[VDIS_DEV_SD]   = SYSTEM_LINK_ID_DISPLAY_2;

	//#-------------------------------------------
	//# Capture link
	//#-------------------------------------------
	//#--- capture link params
	capture_link_params_init(&capturePrm, num_ch, mode);
	capturePrm.outQueParams[0].nextLink	= gVcapModuleContext.nsfId[0];

	//#--- nsf link params
	nsf_link_params_init(&nsfPrm);
	nsfPrm.inQueParams.prevLinkId		= gVcapModuleContext.captureId;
	nsfPrm.inQueParams.prevLinkQueId	= 0;
    nsfPrm.outQueParams[0].nextLink		= SYSTEM_VPSS_LINK_ID_DUP_0;
	
	//#--- dup link params
	dupPrm0.inQueParams.prevLinkId		= gVcapModuleContext.nsfId[0];
	dupPrm0.inQueParams.prevLinkQueId	= 0;
	dupPrm0.numOutQue					= 2;
	dupPrm0.notifyNextLink				= TRUE;

	if(gVsysModuleContext.vsysConfig.enableHDMI)
	{
        dupPrm0.outQueParams[0].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
  	    dupPrm0.outQueParams[1].nextLink	= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;

        swms_link_params_init(&swMsPrm, VDIS_DEV_HDMI, num_ch);
        swMsPrm.inQueParams.prevLinkId      = SYSTEM_VPSS_LINK_ID_DUP_0;
        swMsPrm.inQueParams.prevLinkQueId   = 1;
	    swMsPrm.outQueParams.nextLink       = SYSTEM_VPSS_LINK_ID_DUP_1;

        dupPrm1.inQueParams.prevLinkId		= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
	    dupPrm1.inQueParams.prevLinkQueId	= 0;
	    dupPrm1.numOutQue					= 2;
	    dupPrm1.notifyNextLink				= TRUE;
	    dupPrm1.outQueParams[0].nextLink	= gVdisModuleContext.displayId[VDIS_DEV_HDMI];
	    dupPrm1.outQueParams[1].nextLink	= gVcapModuleContext.nsfId[1];

        nsf_link_params_init(&nsfPrm1);
        nsfPrm1.inQueParams.prevLinkId       = SYSTEM_VPSS_LINK_ID_DUP_1;
        nsfPrm1.inQueParams.prevLinkQueId    = 1;

        if(gVsysModuleContext.vsysConfig.enableMjpeg)
        {
			nsfPrm1.outQueParams[0].nextLink     = SYSTEM_VPSS_LINK_ID_DUP_2;

            dupPrm2.inQueParams.prevLinkId		= gVcapModuleContext.nsfId[1];
	        dupPrm2.inQueParams.prevLinkQueId	= 0;
	        dupPrm2.numOutQue					= 2;
	        dupPrm2.notifyNextLink				= TRUE;
	        dupPrm2.outQueParams[0].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
	        dupPrm2.outQueParams[1].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;  

            mergePrm0.numInQue = 3;
		    mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_0;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
            mergePrm0.inQueParams[1].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_2;
            mergePrm0.inQueParams[1].prevLinkQueId  = 0;
            mergePrm0.inQueParams[2].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_2;
            mergePrm0.inQueParams[2].prevLinkQueId  = 1;
        }
		else
		{
		    nsfPrm1.outQueParams[0].nextLink     = SYSTEM_VPSS_LINK_ID_MERGE_0;

            mergePrm0.numInQue = 2;
            mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_0;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
            mergePrm0.inQueParams[1].prevLinkId     = gVcapModuleContext.nsfId[1];
            mergePrm0.inQueParams[1].prevLinkQueId  = 0;   
		}	
		mergePrm0.notifyNextLink  				= TRUE;

	    if(gVsysModuleContext.vsysConfig.enableEncode) {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_LINK_ID_SWOSD_0; //SYSTEM_VPSS_LINK_ID_IPC_OUT_M3_0;
		} else {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_VPSS_LINK_ID_NULL_0;
			null_link_params_init(&nullPrm0, SYSTEM_VPSS_LINK_ID_MERGE_0, 0);
		}

        disp_link_pararms_init(&dispPrm, VDIS_DEV_HDMI);
		dispPrm.inQueParams[0].prevLinkId	= SYSTEM_VPSS_LINK_ID_DUP_1;
	    dispPrm.inQueParams[0].prevLinkQueId 	= 0;

	    //#--- capture link
	    System_linkCreate(gVcapModuleContext.captureId, &capturePrm, sizeof(capturePrm));
	    System_linkCreate(gVcapModuleContext.nsfId[0], &nsfPrm,	sizeof(nsfPrm));
        System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_0, &dupPrm0, sizeof(dupPrm0));

        //#--- Display link creation -------------------------------------
        System_linkCreate(gVdisModuleContext.swMsId[0], &swMsPrm, sizeof(swMsPrm));
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_1, &dupPrm1, sizeof(dupPrm1));
	    System_linkCreate(gVcapModuleContext.nsfId[1], &nsfPrm1, sizeof(nsfPrm1));
		if(gVsysModuleContext.vsysConfig.enableMjpeg)
		{
		    System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_2, &dupPrm2, sizeof(dupPrm2));
        }

	    System_linkCreate(SYSTEM_VPSS_LINK_ID_MERGE_0, &mergePrm0, sizeof(mergePrm0));

	    //#--- encoder link
		if(gVsysModuleContext.vsysConfig.enableEncode) {
	    	encoder_link_create(SYSTEM_VPSS_LINK_ID_MERGE_0, 0);

		} else {
			System_linkCreate(SYSTEM_VPSS_LINK_ID_NULL_0, &nullPrm0, sizeof(nullPrm0));
		}

	    //#--- display link
        System_linkCreate(gVdisModuleContext.displayId[VDIS_DEV_HDMI], &dispPrm, sizeof(dispPrm));
//	    display_link_create(SYSTEM_VPSS_LINK_ID_DUP_0, 1, num_ch /* or 1 */);
	    dprintf("%s done!\n", __func__);
	}
	else
	{
        dupPrm0.outQueParams[0].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
//	    dupPrm0.outQueParams[0].nextLink	= SYSTEM_VPSS_LINK_ID_IPC_OUT_M3_0;
  	    dupPrm0.outQueParams[1].nextLink	= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;

        swms_link_params_init(&swMsPrm, VDIS_DEV_SD, num_ch);
        swMsPrm.inQueParams.prevLinkId      = SYSTEM_VPSS_LINK_ID_DUP_0;
        swMsPrm.inQueParams.prevLinkQueId   = 1;
	    swMsPrm.outQueParams.nextLink       = SYSTEM_VPSS_LINK_ID_DUP_1;
//      swMsPrm.outQueParams.nextLink       = gVdisModuleContext.displayId[VDIS_DEV_SD];

        dupPrm1.inQueParams.prevLinkId		= SYSTEM_LINK_ID_SW_MS_MULTI_INST_0;
	    dupPrm1.inQueParams.prevLinkQueId	= 0;
	    dupPrm1.numOutQue					= 2;
	    dupPrm1.notifyNextLink				= TRUE;
	    dupPrm1.outQueParams[0].nextLink	= gVdisModuleContext.displayId[VDIS_DEV_SD];
	    dupPrm1.outQueParams[1].nextLink	= gVcapModuleContext.nsfId[1];

        nsf_link_params_init(&nsfPrm1);
        nsfPrm1.inQueParams.prevLinkId       = SYSTEM_VPSS_LINK_ID_DUP_1;
        nsfPrm1.inQueParams.prevLinkQueId    = 1;
		
        if(gVsysModuleContext.vsysConfig.enableMjpeg)
        {
            nsfPrm1.outQueParams[0].nextLink     = SYSTEM_VPSS_LINK_ID_DUP_2; 

            dupPrm2.inQueParams.prevLinkId		= gVcapModuleContext.nsfId[1];
	        dupPrm2.inQueParams.prevLinkQueId	= 0;
	        dupPrm2.numOutQue					= 2;
	        dupPrm2.notifyNextLink				= TRUE;
	        dupPrm2.outQueParams[0].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;
	        dupPrm2.outQueParams[1].nextLink	= SYSTEM_VPSS_LINK_ID_MERGE_0;  

		    mergePrm0.numInQue = 3;
		    mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_0;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
            mergePrm0.inQueParams[1].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_2;
            mergePrm0.inQueParams[1].prevLinkQueId  = 0;
		    mergePrm0.inQueParams[2].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_2;
            mergePrm0.inQueParams[2].prevLinkQueId  = 1;
        }
		else
		{
		    nsfPrm1.outQueParams[0].nextLink     = SYSTEM_VPSS_LINK_ID_MERGE_0;

            mergePrm0.numInQue = 2;
            mergePrm0.inQueParams[0].prevLinkId     = SYSTEM_VPSS_LINK_ID_DUP_0;
            mergePrm0.inQueParams[0].prevLinkQueId  = 0;
            mergePrm0.inQueParams[1].prevLinkId     = gVcapModuleContext.nsfId[1];
            mergePrm0.inQueParams[1].prevLinkQueId  = 0;
		}	
		mergePrm0.notifyNextLink  				= TRUE;

	    if(gVsysModuleContext.vsysConfig.enableEncode) {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_LINK_ID_SWOSD_0; //SYSTEM_VPSS_LINK_ID_IPC_OUT_M3_0;
		} else {
		    mergePrm0.outQueParams.nextLink     = SYSTEM_VPSS_LINK_ID_NULL_0;
			null_link_params_init(&nullPrm0, SYSTEM_VPSS_LINK_ID_MERGE_0, 0);
		}

        disp_link_pararms_init(&dispPrm, VDIS_DEV_SD);
        dispPrm.inQueParams[0].prevLinkId	= SYSTEM_VPSS_LINK_ID_DUP_1;
	    dispPrm.inQueParams[0].prevLinkQueId 	= 0;

	    //#--- capture link
	    System_linkCreate(gVcapModuleContext.captureId, &capturePrm, sizeof(capturePrm));
	    System_linkCreate(gVcapModuleContext.nsfId[0], &nsfPrm,	sizeof(nsfPrm));
		System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_0, &dupPrm0, sizeof(dupPrm0));

	 	//#--- Display link creation -------------------------------------
        System_linkCreate(gVdisModuleContext.swMsId[0], &swMsPrm, sizeof(swMsPrm));
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_1, &dupPrm1, sizeof(dupPrm1));
	    System_linkCreate(gVcapModuleContext.nsfId[1], &nsfPrm1, sizeof(nsfPrm1));
		if(gVsysModuleContext.vsysConfig.enableMjpeg)
		{
		    System_linkCreate(SYSTEM_VPSS_LINK_ID_DUP_2, &dupPrm2, sizeof(dupPrm2));
        }
	    System_linkCreate(SYSTEM_VPSS_LINK_ID_MERGE_0, &mergePrm0, sizeof(mergePrm0));

	    //#--- encoder link
		if(gVsysModuleContext.vsysConfig.enableEncode) {
	    	encoder_link_create(SYSTEM_VPSS_LINK_ID_MERGE_0, 0);

		} else {
			System_linkCreate(SYSTEM_VPSS_LINK_ID_NULL_0, &nullPrm0, sizeof(nullPrm0));
		}

	    //#--- display link
        System_linkCreate(gVdisModuleContext.displayId[VDIS_DEV_SD], &dispPrm, sizeof(dispPrm));
//	    display_link_create(SYSTEM_VPSS_LINK_ID_DUP_0, 1, num_ch /* or 1 */);
	    dprintf("%s done!\n", __func__);
	}

}
#endif /* #if defined(LF_SYS_NEXXONE_VOIP) || defined(LF_SYS_NEXX360H) || defined(LF_SYS_NEXXB_ONE) */

void mcfw_capture_display_exit(void)
{
	//#-------------------------------------------
	//# link deletion
	//#-------------------------------------------
	display_link_delete();
	if(gVsysModuleContext.vsysConfig.enableEncode) {
		encoder_link_delete();
	} else {
		System_linkDelete(SYSTEM_VPSS_LINK_ID_NULL_0);
	}
	System_linkDelete(SYSTEM_VPSS_LINK_ID_MERGE_0);
	System_linkDelete(SYSTEM_VPSS_LINK_ID_DUP_0);
	System_linkDelete(gVcapModuleContext.nsfId[0]);
	System_linkDelete(SYSTEM_VPSS_LINK_ID_DUP_1);
	if(gVsysModuleContext.vsysConfig.enableMjpeg)
	    System_linkDelete(SYSTEM_VPSS_LINK_ID_DUP_2);
	System_linkDelete(gVcapModuleContext.nsfId[1]);
	System_linkDelete(gVcapModuleContext.captureId);

	dprintf("%s done!\n", __func__);
}
