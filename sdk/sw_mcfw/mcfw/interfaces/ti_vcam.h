/** ==================================================================
 *  @file   ti_vcam.h
 *
 *  @path    ipnc_mcfw/mcfw/interfaces/
 *
 *  @desc   This  File contains McFW Video Camera (VCAM) API.
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
    \ingroup MCFW_API
    \defgroup MCFW_VCAM_API McFW Video Camera (VCAM) API

    @{
*/

/**
    \file ti_vcam.h
    \brief McFW Video Camera (VCAM) API
*/

#ifndef __TI_VCAM_H__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define __TI_VCAM_H__

#include "ti_vsys.h"

/**==========
 * Defines
 * ============================================================================= */

/** Maximum Video Camera channels */
#define VCAM_CHN_MAX		(1)

/** Maximum Video Camera streams */
#define VCAM_STRM_MAX       (2)

/**==========
 * Enums
 * ============================================================================= */

/**
    \brief Camera Device ID
*/
typedef enum {
    VCAM_DEV_VP = 0,
    /**< Parallel video port */

    VCAM_DEV_CSI2 = 1,
    /**< CSI2 Port */

    VCAM_DEV_MAX = 2
    /**< Maximum Camera Devices */
} VCAM_DEV;

/**
    \brief Camera bus interface mode
*/
typedef enum {
    VCAM_MODE_YUV422 = 0,
    /**< 16-bit Embedded sync YUV422 mode */

    VCAM_MODE_YUV420,
    /**< 16-bit Embedded sync YUV420 mode */

} VCAM_MODE_E;

/**
    \brief Camera Multi-Channel mode
*/
typedef enum {

    VCAM_MULTICHN_OPTION_SINGLE_CHN = 0
    /**< 1CH Non-Multiplexed mode */
} VCAM_MULTICHN_OPTION_E;

/**
    \brief Camera Param Set/Get ID
*/
typedef enum {
    VCAM_CONTRAST,
    /**< Get/Set Contrast */

    VCAM_SATURATION,
    /**< Get/Set Saturation */

    VCAM_BRIGHTNESS,
    /**< Get/Set Brightness */

    VCAM_SHARPNESS,
    /**< Get/Set Sharpness */

    VCAM_MIRROR_MODE,
    /**< Mirror/Flip mode */

    VCAM_BLC,
    /**< Get/Set BLC */

    VCAM_AWBMODE,
    /**< Get/Set AWB Mode */

    VCAM_AEMODE,
    /**< Get/Set AE Mode */

    VCAM_HUE,
    /**< Get/Set HUE */

    VCAM_RESOLUTION,
    /**< Get/Set Resolution */

    VCAM_FORMAT,
    /**< Get/Set Data Format */

    VCAM_PORTMODE,
    /**< Get/Set Port Mode */

    VCAM_SIGNALTYPE,
    /**< Get/Set Signal Type */

    VCAM_DCCBUFADDR,
    /**< Set DCC PARAM Buffer */

    VCAM_AEWB_VENDOR,
    /**< Set AEWB Vendor Type */

    VCAM_AEWB_MODE,
    /**< Set AEWB Vendor Type */

    VCAM_AEWB_PRIORITY,
    /**< Set AEWB Priority */

    VCAM_IRIS,
    /**< Set IRIS */

    VCAM_FRAMECTRL,
    /**< Set Frame rate control */

    VCAM_ENV_50_60HZ,
    /**< Set ENV 50 60 hz */

    VCAM_BINNING_SKIP,
    /**< Set Binning Skip mode */

    VCAM_FRAMERATE,
    /**< Set Frame Rate */

    VCAM_LDC,
    /**< Lens Distorsion Correction */

    VCAM_SNF,
    /**< Spatial Noise Filter */

    VCAM_TNF,
    /**< Temporal Noise Filter */

	VCAM_SD_FORMAT,
	/**< NTSC/PAL */

    VCAM_ALL
    /**< Get/Set All Params */
} VCAM_PARAMS_E;

/**==========
 * Structures
 * ============================================================================= */

/**
    \brief Camera Device Parameters
*/
typedef struct {
    Bool portEnable;
    /**< Enable/Disable Camera Port */

    VCAM_MODE_E portMode;
    /**< Camera bus interface mode */

    VCAM_MULTICHN_OPTION_E dataFormat;
    /**< Camera Multi-channel mode */

    VIDEO_STANDARD_E signalType;
    /**< Camera video standard or signal type */

} VCAM_DEV_PARAM_S;

/**
    \brief Camera Channel Specific Dynamic Parameters
*/
typedef struct {
    Int32 contrast;
    /**< Constrast, 0..255 */

    Int32 saturation;
    /**< Saturation, 0..255 */

    Int32 brightness;
    /**< Brightness, 0..255 */

    Int32 sharpness;
    /**< Sharpness, 0..255 */

    Int32 frameRate;
    /**< Framerate, 0..60 */

    Int32 resolution;
    /**< Resolution, 0..255 */

    Int32 blc;
    /**< blc, 0..5 */

    Int32 awbMode;
    /**< AWB mode, 0..2 */

    Int32 AEMode;
    /**< AE mode, 0..1 */

    Int32 hue;
    /**< HUE, 0..255 */

    Int32 dccBufAddr;
    /**< DCC Buffer Address */

	UInt32 dccSize;
    /**< DCC Buffer Size */

    Int32 aewbVendor;
    /**< AEWB Vendor Type */

    Int32 aewbMode;
    /**< AEWB Mode*/

    Int32 mirrorMode;
    /**< Mirror Mode*/

    Int32 aewbPriority;
    /**< AEWB Priority*/

    Int32 env50_60hz;
    /**< ENV Mode*/

    Int32 binningMode;
    /**< Binning Skip mode*/

    Int32 IRIS;

    Int32 frameCtrl;

    Int32 enableLDC;

    Int32 enableTNF;

    Int32 enableSNF;

	Int32 sdFormat;

} VCAM_CHN_DYNAMIC_PARAM_S;

/**
    \brief Camera Channel Specific Static Parameters
*/
typedef struct {
    VCAM_DEV deviceId;
    /**< Device ID to which this channel is mapped */

    Bool strmEnable[VCAM_STRM_MAX];
    /**< Enable/Disable Streams in camera, stream 0 should be enabled always */

    VIDEO_FORMATE_E strmFormat[VCAM_STRM_MAX];
    /**< Data Format, YUV422 or YUV420SP of the camera stream. MUST be VF_YUV422I_YUYV for multi-channel camera */

    WINDOW_S strmResolution[VCAM_STRM_MAX];
    /**< Resolution of the stream, stream 0 is always 1:1 resolution, stream 1 can downsized resolution of stream 0 */

    VCAM_CHN_DYNAMIC_PARAM_S dynamicParams;
    /**< Initial values for dynamic parameters of camera channel */

} VCAM_CHN_PARAM_S;

/**
    \brief Camera resolution parameters
*/
typedef struct {
    VCAM_DEV_PARAM_S deviceParams[VCAM_DEV_MAX];
    /**< Device Parameters */

    VCAM_CHN_PARAM_S channelParams[VCAM_CHN_MAX];
    /**< Channel Specific Parameters */

	int width;
	/**< camera width */

	int height;
	/**< camera height */

} VCAM_PARAMS_S;

/**
    \brief Callback for new captured video frames available

    \return NONE
*/
typedef Void (*VCAM_CALLBACK_NEW_DATA)(Ptr cbCtx);

/**
    \brief Capture User Callbacks
*/
typedef struct {

    VCAM_CALLBACK_NEW_DATA  newDataAvailableCb;
    /**< New encoded bitstream available callback */

} VCAM_CALLBACK_S;

#ifdef UDISP_TOOL
/*----------------------------------------------------------------------------
 ISP tunning parameter
 -----------------------------------------------------------------------------*/
#define D2F_THR_CNT         (8)
#define D2F_STR_CNT         (8)

#define IPIPE_MUL_CNT       (9)
#define IPIPE_YEE_THR_CNT	(2)
#define IPIPE_YEE_OFT_CNT	(2)
#define IPIPE_OFT_CNT       (3)
#define IPIPE_WB2_CNT       (4)

#define TBL_CNT             (1024)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//        Black Clamp
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct{
	unsigned char isif_clamp_clen;
	unsigned char isif_clamp_clhmd;
	unsigned char isif_clamp_clmd;
    int isif_cldc;
}VCAM_UDISP_CLAMP_S;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//         ISIF WB
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct{
	unsigned char isif_wben1;
	unsigned char isif_ofsten1;
	unsigned char isif_cfap;
	unsigned int  isif_cgr;
	unsigned int isif_cggr;
	unsigned int isif_cggb;
	unsigned int isif_cgb;
	unsigned int isif_coft;
}VCAM_UDISP_ISIFWB_S;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//        Noise Filter1
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct{
	unsigned char d2f_en;
	unsigned char d2f_lsc;
	unsigned char d2f_typ;
	unsigned char d2f_shf;
	unsigned int  d2f_spr;
	unsigned int d2f_thr[D2F_THR_CNT];
	unsigned int d2f_str[D2F_STR_CNT];
	unsigned int d2f_edg_min;
	unsigned int d2f_edg_max;
}VCAM_UDISP_NF_S;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//      Edge Enhancer
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct{
	unsigned char ipipe_yee_en;
	unsigned char ipipe_yee_hal;
	unsigned char ipipe_yee_sel;
	unsigned int ipipe_yee_shf;
	int ipipe_yee_mul[IPIPE_MUL_CNT];
	unsigned int ipipe_yee_thr;
	unsigned int ipipe_yee_e_gain;
	unsigned int ipipe_yee_e_thr[IPIPE_YEE_THR_CNT];
	unsigned int ipipe_yee_g_gain;
	unsigned int ipipe_yee_g_oft[IPIPE_YEE_OFT_CNT];
}VCAM_UDISP_EE_S;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//     RGB2RGB, RGB2YCbCr, WB
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct{
    unsigned int ipipe_rgb_mul[IPIPE_MUL_CNT];
    int ipipe_rgb_oft[IPIPE_OFT_CNT];
}VCAM_UDISP_RGB2RGB_S;

typedef struct{
    unsigned int ipipe_yuv_brt;
    unsigned int ipipe_yuv_crt;
    int ipipe_yuv_mul[IPIPE_MUL_CNT];
    unsigned int ipipe_yuv_oft[IPIPE_OFT_CNT];
}VCAM_UDISP_RGB2YUV_S;

typedef struct{
    int ipipe_wb2_oft[IPIPE_WB2_CNT];
	unsigned int ipipe_wb2_wgn[IPIPE_WB2_CNT];
}VCAM_UDISP_IPIPEWB_S;

typedef struct{
    VCAM_UDISP_RGB2RGB_S	rgb2rgb1;
    VCAM_UDISP_RGB2RGB_S	rgb2rgb2;
	VCAM_UDISP_RGB2YUV_S	rgb2ycbcr;
    VCAM_UDISP_IPIPEWB_S	white_balance;
}VCAM_UDISP_COLOR_S;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//             GBCE
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct{
	unsigned char gbce_en;
	int gbce_typ;
	unsigned int gbce_lut[TBL_CNT];
}VCAM_UDISP_GBCE_S;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//        Gamma
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct{
	unsigned char ipipe_gmm_tbl;
	unsigned char ipipe_gmm_siz;
	unsigned char ipipe_gmm_bypb;
	unsigned char ipipe_gmm_bypg;
	unsigned char ipipe_gmm_bypr;
	unsigned int gmm_tbl[TBL_CNT];
}VCAM_UDISP_GAMMA_S;

typedef struct{
	VCAM_UDISP_CLAMP_S		clamp;
    VCAM_UDISP_ISIFWB_S		isif_wb;
    VCAM_UDISP_NF_S			noise_filter1;
    VCAM_UDISP_NF_S			noise_filter2;
    VCAM_UDISP_EE_S			edge_enhancer;
    VCAM_UDISP_RGB2RGB_S	rgb2rgb1;
    VCAM_UDISP_RGB2RGB_S	rgb2rgb2;
	VCAM_UDISP_RGB2YUV_S	rgb2ycbcr;
    VCAM_UDISP_IPIPEWB_S	ipipe_wb;
    VCAM_UDISP_GBCE_S		gbce;
    VCAM_UDISP_GAMMA_S		gamma;
}VCAM_UDISP_PARAMS_S;

typedef enum {
VCAM_UDISP_BALCKCLAMP,
VCAM_UDISP_ISIFWB,
VCAM_UDISP_NOISEFILTER1,
VCAM_UDISP_NOISEFILTER2,
VCAM_UDISP_EDGEENHANCER,
VCAM_UDISP_RGB2RGB1,
VCAM_UDISP_RGB2RGB2,	
VCAM_UDISP_RGB2YCBCR,	
VCAM_UDISP_IPIPEWB,	
VCAM_UDISP_GBCE,	
VCAM_UDISP_GAMMA		
}VCAM_UDISP_E;

Int32 Vcam_set_ud_tuning(void *params, VCAM_UDISP_E paramId);
Int32 Vcam_get_ud_tuning(void *params, VCAM_UDISP_E paramId);
#endif //# UDISP_TOOL

/**==========
 * APIs
 * ============================================================================= */

/**
 *  @func     Vcam_params_init
 *
 *  @desc     Function does the Set defaults for VCAM_PARAMS_S
 *            By default camera parameters are setup for 16CH D1 camera mode
 *
 *  @modif
 *
 *  @inputs   None
 *
 *  @outputs  pContext    [OUT] VCAM_PARAMS_S filled with default parameters
 *
 *  @return   None
 *
 */
Void Vcam_params_init(VCAM_PARAMS_S * pContext);

/**
 *  @func     Vcam_init
 *
 *  @desc     Function does the Initialize camera sub-system
 *
 *  @modif
 *
 *  @inputs   pContext [IN] params for creation / Camera configuration
 *
 *  @outputs  None
 *
 *  @return   ERROR_NONE on success
 *
 */
Int32 Vcam_init(VCAM_PARAMS_S * pContext);

/**
 *  @func     Vcam_exit
 *
 *  @desc     Function does the De-Initialize camera sub-system
 *            MUST be called after Vcam_stop() and Vsys_delete()
 *
 *  @modif
 *
 *  @inputs   None
 *
 *  @outputs  None
 *
 *  @return  None
 *
 */
Void Vcam_exit();

/**
 *  @func     Vcam_start
 *
 *  @desc     Function does the Start camera sub-system
 *            MUST be called after calling Vcam_init() and Vsys_create()
 *
 *  @modif
 *
 *  @inputs   None
 *
 *  @outputs  None
 *
 *  @return   ERROR_NONE on success
 *
 */
Int32 Vcam_start();

/**
 *  @func     Vcam_stop
 *
 *  @desc     Function does Stop camera sub-system
 *            MUST be called before calling Vsys_delete()
 *
 *  @modif
 *
 *  @inputs   None
 *
 *  @outputs  None
 *
 *  @return   ERROR_NONE on success
 *
 */
Int32 Vcam_stop();

/**
 *  @func     Vcam_enableChn
 *
 *  @desc     Function does the Enable a channel in the camera sub-system
 *             By default all channels and streams are enabled after Vcam_start()
 *
 *  @modif
 *
 *  @inputs   vcChnId  [IN] Camera channel ID
 *            vcStrmId [IN] Camera stream ID
 *
 *  @outputs  None
 *
 *  @return   ERROR_NONE on success
 *
 */
Int32 Vcam_enableChn(VCAM_CHN vcChnId, VCAM_STRM vcStrmId);

/**
 *  @func     Vcam_disableChn
 *
 *  @desc     Function does the  Disable a channel in the camera sub-system
 *            Disabling a channel will make the channel unavailable for encode and display.
 *            If encode is enabled on that channel then user will stop getting bitstream for that channel.
 *            If display is enabled on that channel then display will hold on the last frame
 *            that was shown for that window on the display.
 *            When a camera channel is disabled, it is recommended to disable the channel on the display as well.
 *
 *  @modif
 *
 *  @inputs   vcChnId  [IN] Camera channel ID
 *            vcStrmId [IN] Camera stream ID
 *
 *  @outputs None
 *
 *  @return   ERROR_NONE on success
 *
 */
Int32 Vcam_disableChn(VCAM_CHN vcChnId, VCAM_STRM vcStrmId);

/**
 *  @func     Vcam_setParamChn
 */
Int32 Vcam_setParamChn(VCAM_CHN vcChnId, VCAM_CHN_PARAM_S * psCamChnParam,
                       VCAM_PARAMS_E paramId);

/**
 *  @func     Vcam_getParamChn
 */
Int32 Vcam_getParamChn(VCAM_CHN vcChnId, VCAM_CHN_PARAM_S * psCamChnParam,
                       VCAM_PARAMS_E paramId);

/**
 *  @func     Vcam_setDynamicParamChn
 */
Int32 Vcam_setDynamicParamChn(VCAM_CHN vcChnId,
                              VCAM_CHN_DYNAMIC_PARAM_S * psCamChnDynaParam,
                              VCAM_PARAMS_E paramId);

/**
 *  @func     Vcam_setFrameRate
 *
 *  @desc     Function does  Set Camera frame rate.
 *            This is an additional control in camera side itself;
 *            is really useful for secondary stream <CIF>. Stream 0 is D1 & Stream 1 is CIF.
 *
 *  @modif
 *
 *  @inputs   vcChnId [IN] video capture Channel ID
 *            vStrmID [IN] stream ID
 *            frameRate [IN] Frame rate to be set
 *
 *  @outputs  None
 *
 *  @return   ERROR_NONE on success
 *
 */
Int32 Vcam_setFrameRate(VCAM_CHN vcChnId, VCAM_STRM vStrmId, Int32 frameRate);

/**
 *  @func     Vcam_getFrameRate
 */
Int32 Vcam_getFrameRate(VCAM_CHN vcChnId, VCAM_STRM vStrmId);

/**
 *  @func     Vcam_getDynamicParamChn
 */
Int32 Vcam_getDynamicParamChn(VCAM_CHN vcChnId,
                              VCAM_CHN_DYNAMIC_PARAM_S * psCamChnParam,
                              VCAM_PARAMS_E paramId);

/**
 *  @func     Vcam_enableDevice
 */
Int32 Vcam_enableDevice(VCAM_DEV vcDevId);

/**
 *  @func     Vcam_disableDevice
 */
Int32 Vcam_disableDevice(VCAM_DEV vcDevId);

/**
 *  @func     Vcam_setParamDevice
 */
Int32 Vcam_setParamDevice(VCAM_DEV vcDevId, VCAM_DEV_PARAM_S * psvcDevParam,
                          VCAM_PARAMS_E paramId);

/**
 *  @func     Vcam_getParamDevice
 */
Int32 Vcam_getParamDevice(VCAM_DEV vcDevId, VCAM_DEV_PARAM_S * psvcDevParam,
                          VCAM_PARAMS_E paramId);

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
Int32 Vcam_getFullVideoFrames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList, UInt32 timeout);

/**
    \brief Give consumed video frames back to the application to be freed

    Buffers that are were previously got from Vcam_getFullVideoFrames can be
    freed back to the framework by invoking this API.

    \param pFrameBufList [IN]   List of video frames

    \return ERROR_NONE on success
*/
Int32 Vcam_putEmptyVideoFrames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList);

/**
    \brief Register user specified callbacks

    \param callback            [IN] User specified callbacks

    \param arg                 [IN] Callback context

    \return ERROR_NONE on success
*/
Int32 Vcam_registerCallback(VCAM_CALLBACK_S * callback, Ptr arg);

/**
 *  \brief	Sensor write/read register
 */
Int32 Vcam_set_sensor_reg(UInt8 regaddr, UInt8 regval);
Int32 Vcam_get_sensor_reg(UInt8 regaddr, UInt8 *regval);

Int32 Vcam_get_alg_version(UInt32 *version);

/**
 *  \brief	Sensor crop area
 */
Int32 Vcam_set_crop(Int32 stream, Int32 sx, Int32 sy, Int32 wi, Int32 he);

/**
 *  \brief	2A set Brightness/Sharpness/Contrast
 */
Int32 Vcam_set_video_color(char option, Int32 value);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

/* @} */
