/** ==================================================================
 *  @file   cameraLink.h
 *
 *  @path    ipnc_mcfw/mcfw/interfaces/link_api/
 *
 *  @desc   This  File contains Camera Link API.
 *
 * ===================================================================
 *
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 *
 * ===================================================================*/

/**
    \ingroup LINK_API
    \defgroup CAMERA_LINK_API Camera Link API

    Camera Link can be used to instantiate camera upto
    SYSTEM_CAMERA_INST_MAX Video input port instances.

    Each instance can have upto two outputs.

    The frames from these camera outputs can be put in upto
    four output queues.

    Each output queue can inturn to be connected to a link like
    Display or DEI or NSF.

    @{
*/

/**
    \file cameraLink.h
    \brief Camera Link API
*/

#ifndef _CAMERA_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _CAMERA_H_

#include <mcfw/interfaces/link_api/system.h>

/** \brief Max outputs per VIP instance */
#define CAMERA_LINK_MAX_OUTPUT_PER_INST  (2)

/** \brief Max output queues in the camera link */
#define CAMERA_LINK_MAX_OUT_QUE          (2)

/** \brief Max Channels per output queue */
#define CAMERA_LINK_MAX_CH_PER_OUT_QUE   (1)

/**
    \brief Link CMD: Detect camera video source format

    This command can make the camera link wait until video
    source is detect on all the expected input video sources

    \param UInt32 timeout  [IN] BIOS_WAIT_FOREVER or BIOS_NO_WAIT
*/
#define CAMERA_LINK_CMD_DETECT_VIDEO           (0x1000)

#define CAMERA_LINK_CMD_FORCE_RESET            (0x1001)
#define CAMERA_LINK_CMD_PRINT_ADV_STATISTICS   (0x1002)
#define CAMERA_LINK_CMD_CHANGE_BRIGHTNESS      (0x1003)
#define CAMERA_LINK_CMD_CHANGE_CONTRAST        (0x1004)
#define CAMERA_LINK_CMD_CHANGE_SATURATION      (0x1005)
#define CAMERA_LINK_CMD_CHANGE_HUE             (0x1006)
#define CAMERA_LINK_CMD_HALT_EXECUTION         (0x1007)
#define CAMERA_LINK_CMD_DCC_BUFFER             (0x1008)
#define CAMERA_LINK_CMD_AEWB_VENDOR			   (0x1009)
#define CAMERA_LINK_CMD_AEWB_MODE			   (0x100A)
#define CAMERA_LINK_CMD_AEWB_PRIORITY		   (0x100B)
#define CAMERA_LINK_CMD_CHANGE_SHARPNESS	   (0x100C)
#define CAMERA_LINK_CMD_CHANGE_BLC			   (0x100D)
#define CAMERA_LINK_CMD_CHANGE_AWBMODE		   (0x100E)
#define CAMERA_LINK_CMD_CHANGE_AEMODE		   (0x100F)
#define CAMERA_LINK_CMD_CHANGE_ENV_50_60HZ	   (0x1010)
#define CAMERA_LINK_CMD_CHANGE_BINNING		   (0x1011)
#define CAMERA_LINK_CMD_CHANGE_RESOLUTION	   (0x1012)
#define CAMERA_LINK_CMD_CHANGE_FRAMERATE	   (0x1013)
#define CAMERA_LINK_CMD_IRIS				   (0x1014)
#define CAMERA_LINK_CMD_FRAMECTRL			   (0x1015)
#define CAMERA_LINK_CMD_MIRROR_MODE			   (0x1016)
#define CAMERA_LINK_CMD_SET_FORMAT	   		   (0x1017)
#define CAMERA_LINK_CMD_REG_WR_SENSOR          (0x1018)
#define CAMERA_LINK_CMD_REG_RD_SENSOR          (0x1019)
#define CAMERA_LINK_CMD_CHANGE_CROP			   (0x101A)
#define CAMERA_LINK_CMD_GET_ALG_VERSION        (0x101B)

#ifdef UDISP_TOOL
#define CAMERA_LINK_CMD_SET_UD_TUNING				(0x101C)
#define CAMERA_LINK_CMD_GET_UD_TUNING				(0x101D)
#define CAMERA_LINK_CMD_UDISP_SET_BALCKCLAMP		(0x101E)
#define CAMERA_LINK_CMD_UDISP_SET_ISIFWB			(0x101F)
#define CAMERA_LINK_CMD_UDISP_SET_NOISEFILTER1      (0x1020)
#define CAMERA_LINK_CMD_UDISP_SET_NOISEFILTER2      (0x1021)
#define CAMERA_LINK_CMD_UDISP_SET_EDGEENHANCER      (0x1022)
#define CAMERA_LINK_CMD_UDISP_SET_RGB2RGB1          (0x1023)
#define CAMERA_LINK_CMD_UDISP_SET_RGB2RGB2	        (0x1024)
#define CAMERA_LINK_CMD_UDISP_SET_RGB2YCBCR	        (0x1025)
#define CAMERA_LINK_CMD_UDISP_SET_IPIPEWB	        (0x1026)
#define CAMERA_LINK_CMD_UDISP_SET_GBCE	            (0x1027)
#define CAMERA_LINK_CMD_UDISP_SET_GAMMA             (0x1028)
#define CAMERA_LINK_CMD_UDISP_GET_BALCKCLAMP		(0x1029)
#define CAMERA_LINK_CMD_UDISP_GET_ISIFWB			(0x102A)
#define CAMERA_LINK_CMD_UDISP_GET_NOISEFILTER1      (0x102B)
#define CAMERA_LINK_CMD_UDISP_GET_NOISEFILTER2      (0x102C)
#define CAMERA_LINK_CMD_UDISP_GET_EDGEENHANCER      (0x102D)
#define CAMERA_LINK_CMD_UDISP_GET_RGB2RGB1          (0x102F)
#define CAMERA_LINK_CMD_UDISP_GET_RGB2RGB2	        (0x1030)
#define CAMERA_LINK_CMD_UDISP_GET_RGB2YCBCR	        (0x1031)
#define CAMERA_LINK_CMD_UDISP_GET_IPIPEWB	        (0x1032)
#define CAMERA_LINK_CMD_UDISP_GET_GBCE	            (0x1033)
#define CAMERA_LINK_CMD_UDISP_GET_GAMMA             (0x1034)
#endif //# UDISP_TOOL

#define FD_LINK_CMD_PRINT_STATISTICS           (0x1100)
#define SWOSD_LINK_CMD_PRINT_STATISTICS        (0x1101)

/**
    \brief Camera output parameters
*/
typedef enum {

    CAMERA_LINK_CAPMODE_ISIF,
	CAMERA_LINK_CAPMODE_DDR

} CameraLink_capMode;

/**
    \brief Camera output parameters
*/
typedef struct {
    UInt32 dataFormat;
    /**< output data format, YUV422, YUV420, RGB, see System_VideoDataFormat */

    UInt32 scEnable;
    /**< TRUE: enable scaling, FALSE: disable scaling */

    UInt32 scOutWidth;
    /**< Scaler output width */

    UInt32 scOutHeight;
    /**< Scaler output height */

    UInt32 outQueId;
    /**< Link output que ID to which this VIP instance output frames are put */

	UInt32 standard;
	/**< NTSC/PAL */
} CameraLink_OutParams;

/**
    \brief VIP instance information
*/
typedef struct {
    UInt32 vipInstId;
    /**< VIP camera driver instance ID, see SYSTEM_CAMERA_INST_VIPx_PORTy */

    UInt32 videoDecoderId;
    /**< Video decoder instance ID, see SYSTEM_DEVICE_VID_DEC_xxx_DRV */

    UInt32 inDataFormat;
    /**< Input source data format, RGB or YUV422, see System_VideoDataFormat */

    UInt32 standard;
    /**< Required video standard, see System_VideoStandard */

    UInt32 numOutput;
    /**< Number of outputs per VIP */

    CameraLink_OutParams outParams[CAMERA_LINK_MAX_OUTPUT_PER_INST];
    /**< Information about each output */

	UInt32 sensorOutWidth;
	/**< Senor out width */

	UInt32 sensorOutHeight;
	/**< Sensor out height */

} CameraLink_VipInstParams;

/**
    \brief 2A information
*/
typedef struct {
    UInt32 n2A_vendor;
    /**< Select 2A algorithm to use. */

    UInt32 n2A_mode;
    /**< Run auto explosion, auto white balance, both or none. */

} CameraLink_2AParams;

/**
    \brief Camera Link create parameters
*/
typedef struct {
    UInt16 numVipInst;
    /**< Number of VIP instances in this link */

    CameraLink_VipInstParams vipInst[SYSTEM_CAMERA_INST_MAX];
    /**< VIP instance information */

    System_LinkOutQueParams outQueParams[CAMERA_LINK_MAX_OUT_QUE];
    /**< Output queue information */

    UInt32 tilerEnable;
    /**< TRUE/FALSE: Enable/disable tiler */

    UInt32 vsEnable;
    /**< TRUE/FALSE: Enable/disable tiler */

    UInt32 mirrorMode;
    /**< Mirror mode configuration */

    UInt32 numAudioChannels;
   /**< 4 Audio channels or 16 Audio Channels*/

    UInt32 fakeHdMode;
    /**< Camera in D1 but tells link size is 1080p, useful to test HD data flow on DVR HW or VS EVM */

    UInt32 enableSdCrop;
    /**< Applicable only for D1/CIF camera input, crops 720 to 704 and 360 to 352 at video decoder.

        */
    CameraLink_2AParams t2aConfig;
    /**< Configuration of 2A */

    UInt32 issVnfEnable;
    /*****ISS VnF is enabled***/

	UInt32 ispLinkId;
	/*****ISP link ID***/

	UInt32 vaLinkId;
	/*****VA link Mode***/

	UInt32 vaStreamId;
	/*****VA stream Id***/

	CameraLink_capMode captureMode;
	/*****input Mode***/

	UInt32 vstabLinkId;
	/*****vstab link Id***/

	UInt32 vnfLinkId;
	/*****vnf link Id***/
	UInt32 glbceLinkId;
	/*****glbce link Id***/

} CameraLink_CreateParams;

/**
    \brief Camera link channel dynamic get dynamic parameters

    Defines Camera dynamic parameters that can be probed dynamically
    on a per channel basis for the encode link
*/
typedef struct CameraLink_GetDynParams {
    UInt32 chId;
    /**< Camera channel number */
    UInt32 inputWidth;
    /**< Camera input width */
    UInt32 inputHeight;
    /**< Camera input height */
    UInt32 targetFps;
    /**< Modified Camera Frames per second (FPS) value in fps x 1000 format */
} CameraLink_GetDynParams;

/**
    \brief Camera link channel dynamic set config params

    Defines Camera FPS parameters that can be changed dynamically
    on a per channel basis for the Camera link
*/
typedef struct CameraLink_FpsParams {
    UInt32 StreamId; 	/**< Encoder channel number */
    UInt32 targetFps;
    /**< Modified encoder Frames per second (FPS) value in fps x 1000 format */
} CameraLink_FpsParams;

/**
    \brief Camera link channel DCC set config params
    Defines DCC parameters that can be changed dynamically
*/
typedef struct CameraLink_DccParams {

    Int32 dccBufAddr; 	/**< DCC Buffer Address */
	UInt32 dccSize; 	/**< DCC Buffer Size */
} CameraLink_DccParams;

/**
    \brief Camera link Image sensor register read/write params
*/
typedef struct {
    UInt8 regAddr;		/**< Register Address */
    UInt8 regValue;		/**< Register value */
} CameraLink_SensorRegParams;

/**
    \brief Crop information
*/
typedef struct {
    UInt32 stream;
    UInt32 sx;
    UInt32 sy;
    UInt32 wi;
    UInt32 he;
} CameraLink_CropParams;

/**
 *  @func     CameraLink_init
 *
 *  @desc     Function does the Camera link register and init
 *               - Creates link task
 *               - Registers as a link with the system API
 *
 *  @modif
 *
 *  @inputs   This function takes the following inputs
 *
 *  @outputs
 *
 *  @return   FVID2_SOK on success
 *
 */
Int32 CameraLink_init();

/**
 *  @func     CameraLink_deInit
 *
 *  @desc     Function does the Camera link de-register and de-init
 *             - Deletes link task
 *             - De-registers as a link with the system API
 *
 *  @modif
 *
 *  @inputs
 *
 *  @outputs
 *
 *  @return  FVID2_SOK on success
 *
 */
Int32 CameraLink_deInit();

/**
 *  @func     CameraLink_CreateParams_Init
 *
 *  @desc     Function does the initialization of create params of
 *            Camera Link
 *
 *  @modif
 *
 *  @inputs   This function takes the following inputs
 *            pPrm
 *            pointer to the create params to be initialized
 *
 *  @outputs  pPrm
 *            pointer of the create params
 *
 *  @return   None
 *
 */
static inline void CameraLink_CreateParams_Init(CameraLink_CreateParams * pPrm)
{
    memset(pPrm, 0, sizeof(*pPrm));

    pPrm->numVipInst = 0;
    pPrm->tilerEnable = FALSE;
    pPrm->fakeHdMode = FALSE;
    pPrm->enableSdCrop = TRUE;
	pPrm->ispLinkId = SYSTEM_LINK_ID_INVALID;
	pPrm->vaLinkId  = SYSTEM_LINK_ID_INVALID;
	pPrm->vstabLinkId = SYSTEM_LINK_ID_INVALID;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

/* @} */
