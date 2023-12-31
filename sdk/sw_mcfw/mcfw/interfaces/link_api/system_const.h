/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2012 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/
/**
    \ingroup LINK_API
    \defgroup SYSTEM_CONST_API System constants API

    Enlists system wide constants and enums

    @{
*/

/**
    \file system_const.h
    \brief Common system constants
*/

#ifndef _SYSTEM_CONST_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _SYSTEM_CONST_H_

/**
    \brief Buffer alignment

    SUPPORTED in ALL platforms
*/
#define SYSTEM_BUFFER_ALIGNMENT         (16)

/**
    \brief Maximum output queues

    SUPPORTED in ALL platforms
*/
#define SYSTEM_MAX_OUT_QUE              (6u)

/**
    \brief Maximum channels per output queue

    SUPPORTED in ALL platforms
*/
#define SYSTEM_MAX_CH_PER_OUT_QUE       (64u)

/**
    \brief Maximum planes

    SUPPORTED in ALL platforms
*/
#define SYSTEM_MAX_PLANES               (3)

/* Pre-defined resolution types */
#define SYSTEM_DISPLAY_RES_1080P30  (0)
#define SYSTEM_DISPLAY_RES_1080I60  (1)
#define SYSTEM_DISPLAY_RES_1080P60  (2)
#define SYSTEM_DISPLAY_RES_720P60   (3)
#define SYSTEM_DISPLAY_RES_NTSC     (4)
#define SYSTEM_DISPLAY_RES_PAL      (5)
#define SYSTEM_DISPLAY_RES_XGA      (6)
#define SYSTEM_DISPLAY_RES_SXGA     (7)
#define SYSTEM_DISPLAY_RES_MAX      (8)

#define SYSTEM_CAMERA_INST_VP_CSI2	(0u)

#define SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE (0x10000500u)

#define SYSTEM_DEVICE_VID_CAM_MT9J003_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x0000u)
#define SYSTEM_DEVICE_VID_CAM_AR0331_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x0001u)
#define SYSTEM_DEVICE_VID_CAM_MN34041_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x0002u)
#define SYSTEM_DEVICE_VID_CAM_IMX035_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x0003u)
#define SYSTEM_DEVICE_VID_CAM_OV2715_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x0004u)
#define SYSTEM_DEVICE_VID_CAM_IMX036_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x0005u)
#define SYSTEM_DEVICE_VID_CAM_OV9712_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x0006u)
#define SYSTEM_DEVICE_VID_CAM_OV10630_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x0007u)
#define SYSTEM_DEVICE_VID_CAM_MT9P031_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x0008u)
#define SYSTEM_DEVICE_VID_CAM_MT9D131_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x0009u)
#define SYSTEM_DEVICE_VID_CAM_MT9M034_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x000Au)
#define SYSTEM_DEVICE_VID_CAM_TVP514X_DRV    (SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x000Bu)

#define SYSTEM_DEVICE_VID_CAM_IMX122_DRV	(SYSTEM_DEVICE_VID_CAM_SENSOR_DRV_BASE + 0x000Du)
/** \brief Capture instance - VIP0 - Port A  */
#define SYSTEM_CAPTURE_INST_VIP0_PORTA (0u)

/** \brief Capture instance - VIP0 - Port B  */
#define SYSTEM_CAPTURE_INST_VIP0_PORTB (1u)

/** \brief Capture instance - VIP1 - Port A  */
#define SYSTEM_CAPTURE_INST_VIP1_PORTA (2u)

/** \brief Capture instance - VIP1 - Port B  */
#define SYSTEM_CAPTURE_INST_VIP1_PORTB (3u)

/** \brief Capture instance - MAX instances  */
#define SYSTEM_CAPTURE_INST_MAX        (4u)

/** \brief Camera instance - VIP0 - Port A  */
#define SYSTEM_CAMERA_INST_VP0_PORTA (0u)

/** \brief Camera instance - VIP0 - Port B  */
#define SYSTEM_CAMERA_INST_VP0_PORTB (1u)

/** \brief Camera instance - VIP1 - Port A  */
#define SYSTEM_CAMERA_INST_VP1_PORTA (2u)

/** \brief Camera instance - VIP1 - Port B  */
#define SYSTEM_CAMERA_INST_VP1_PORTB (3u)

/** \brief Camera instance - MAX instances  */
#define SYSTEM_CAMERA_INST_MAX        (2u)

/** \brief Driver ID base for video decoder driver class. */
#define SYSTEM_DEVICE_VID_DEC_DRV_BASE       (0x00000400u)

/** \brief TVP5158 video decoder driver ID used at the time of SYSTEM_create() */
#define SYSTEM_DEVICE_VID_DEC_TVP5158_DRV    (SYSTEM_DEVICE_VID_DEC_DRV_BASE + 0x0000u)

/** \brief TVP7002 video decoder driver ID used at the time of SYSTEM_create() */
#define SYSTEM_DEVICE_VID_DEC_TVP7002_DRV    (SYSTEM_DEVICE_VID_DEC_DRV_BASE + 0x0001u)

/** \brief HDMI SII9135 video decoder driver ID used at the time of SYSTEM_create() */
#define SYSTEM_DEVICE_VID_DEC_SII9135_DRV    (SYSTEM_DEVICE_VID_DEC_DRV_BASE + 0x0002u)

/** \brief MT9D111 sensor driver ID used at the time of FVID2_create() */
#define SYSTEM_DEVICE_VID_DEC_MT9D111_DRV    (SYSTEM_DEVICE_VID_DEC_DRV_BASE + 0x0003u)

/** \brief THCV218 SERDES driver ID used at the time of FVID2_create() */
#define SYSTEM_DEVICE_VID_DEC_THCV218_DRV    (SYSTEM_DEVICE_VID_DEC_DRV_BASE + 0x0004u)

/** \brief PHX100K sensor driver ID used at the time of FVID2_create() */
#define SYSTEM_DEVICE_VID_DEC_PH1100K_DRV    (SYSTEM_DEVICE_VID_DEC_DRV_BASE + 0x0005u)
#define SYSTEM_DEVICE_VID_DEC_PH3100K_DRV    (SYSTEM_DEVICE_VID_DEC_DRV_BASE + 0x0006u)

/** \brief NVP6124B decoder driver ID used at the time of FVID2_create() */
#define SYSTEM_DEVICE_VID_DEC_NVP6124B_DRV   (SYSTEM_DEVICE_VID_DEC_DRV_BASE + 0x0007u)

/** \brief NVP2440H decoder driver ID used at the time of FVID2_create() */
#define SYSTEM_DEVICE_VID_DEC_NVP2440H_DRV    (SYSTEM_DEVICE_VID_DEC_DRV_BASE + 0x0008u)

/**
 *  \brief Video Data format.
 */
typedef enum
{
    SYSTEM_DF_YUV422I_UYVY = 0x0000,
    /**< YUV 422 Interleaved format - UYVY. */
    SYSTEM_DF_YUV422I_YUYV,
    /**< YUV 422 Interleaved format - YUYV. */
    SYSTEM_DF_YUV422I_YVYU,
    /**< YUV 422 Interleaved format - YVYU. */
    SYSTEM_DF_YUV422I_VYUY,
    /**< YUV 422 Interleaved format - VYUY. */
    SYSTEM_DF_YUV422SP_UV,
    /**< YUV 422 Semi-Planar - Y separate, UV interleaved. */
    SYSTEM_DF_YUV422SP_VU,
    /**< YUV 422 Semi-Planar - Y separate, VU interleaved. */
    SYSTEM_DF_YUV422P,
    /**< YUV 422 Planar - Y, U and V separate. */
    SYSTEM_DF_YUV420SP_UV,
    /**< YUV 420 Semi-Planar - Y separate, UV interleaved. */
    SYSTEM_DF_YUV420SP_VU,
    /**< YUV 420 Semi-Planar - Y separate, VU interleaved. */
    SYSTEM_DF_YUV420P,
    /**< YUV 420 Planar - Y, U and V separate. */
    SYSTEM_DF_YUV444P,
    /**< YUV 444 Planar - Y, U and V separate. */
    SYSTEM_DF_YUV444I,
    /**< YUV 444 interleaved - YUVYUV... */
    SYSTEM_DF_RGB16_565 = 0x1000,
    /**< RGB565 16-bit - 5-bits R, 6-bits G, 5-bits B. */
    SYSTEM_DF_ARGB16_1555,
    /**< ARGB1555 16-bit - 5-bits R, 5-bits G, 5-bits B, 1-bit Alpha (MSB). */
    SYSTEM_DF_RGBA16_5551,
    /**< RGBA5551 16-bit - 5-bits R, 5-bits G, 5-bits B, 1-bit Alpha (LSB). */
    SYSTEM_DF_ARGB16_4444,
    /**< ARGB4444 16-bit - 4-bits R, 4-bits G, 4-bits B, 4-bit Alpha (MSB). */
    SYSTEM_DF_RGBA16_4444,
    /**< RGBA4444 16-bit - 4-bits R, 4-bits G, 4-bits B, 4-bit Alpha (LSB). */
    SYSTEM_DF_ARGB24_6666,
    /**< ARGB6666 24-bit - 6-bits R, 6-bits G, 6-bits B, 6-bit Alpha (MSB). */
    SYSTEM_DF_RGBA24_6666,
    /**< RGBA6666 24-bit - 6-bits R, 6-bits G, 6-bits B, 6-bit Alpha (LSB). */
    SYSTEM_DF_RGB24_888,
    /**< RGB24 24-bit - 8-bits R, 8-bits G, 8-bits B. */
    SYSTEM_DF_ARGB32_8888,
    /**< ARGB32 32-bit - 8-bits R, 8-bits G, 8-bits B, 8-bit Alpha (MSB). */
    SYSTEM_DF_RGBA32_8888,
    /**< RGBA32 32-bit - 8-bits R, 8-bits G, 8-bits B, 8-bit Alpha (LSB). */
    SYSTEM_DF_BGR16_565,
    /**< BGR565 16-bit -   5-bits B, 6-bits G, 5-bits R. */
    SYSTEM_DF_ABGR16_1555,
    /**< ABGR1555 16-bit - 5-bits B, 5-bits G, 5-bits R, 1-bit Alpha (MSB). */
    SYSTEM_DF_ABGR16_4444,
    /**< ABGR4444 16-bit - 4-bits B, 4-bits G, 4-bits R, 4-bit Alpha (MSB). */
    SYSTEM_DF_BGRA16_5551,
    /**< BGRA5551 16-bit - 5-bits B, 5-bits G, 5-bits R, 1-bit Alpha (LSB). */
    SYSTEM_DF_BGRA16_4444,
    /**< BGRA4444 16-bit - 4-bits B, 4-bits G, 4-bits R, 4-bit Alpha (LSB). */
    SYSTEM_DF_ABGR24_6666,
    /**< ABGR6666 24-bit - 6-bits B, 6-bits G, 6-bits R, 6-bit Alpha (MSB). */
    SYSTEM_DF_BGR24_888,
    /**< BGR888 24-bit - 8-bits B, 8-bits G, 8-bits R. */
    SYSTEM_DF_ABGR32_8888,
    /**< ABGR8888 32-bit - 8-bits B, 8-bits G, 8-bits R, 8-bit Alpha (MSB). */
    SYSTEM_DF_BGRA24_6666,
    /**< BGRA6666 24-bit - 6-bits B, 6-bits G, 6-bits R, 6-bit Alpha (LSB). */
    SYSTEM_DF_BGRA32_8888,
    /**< BGRA8888 32-bit - 8-bits B, 8-bits G, 8-bits R, 8-bit Alpha (LSB). */
    SYSTEM_DF_BITMAP8 = 0x2000,
    /**< BITMAP 8bpp. */
    SYSTEM_DF_BITMAP4_LOWER,
    /**< BITMAP 4bpp lower address in CLUT. */
    SYSTEM_DF_BITMAP4_UPPER,
    /**< BITMAP 4bpp upper address in CLUT. */
    SYSTEM_DF_BITMAP2_OFFSET0,
    /**< BITMAP 2bpp offset 0 in CLUT. */
    SYSTEM_DF_BITMAP2_OFFSET1,
    /**< BITMAP 2bpp offset 1 in CLUT. */
    SYSTEM_DF_BITMAP2_OFFSET2,
    /**< BITMAP 2bpp offset 2 in CLUT. */
    SYSTEM_DF_BITMAP2_OFFSET3,
    /**< BITMAP 2bpp offset 3 in CLUT. */
    SYSTEM_DF_BITMAP1_OFFSET0,
    /**< BITMAP 1bpp offset 0 in CLUT. */
    SYSTEM_DF_BITMAP1_OFFSET1,
    /**< BITMAP 1bpp offset 1 in CLUT. */
    SYSTEM_DF_BITMAP1_OFFSET2,
    /**< BITMAP 1bpp offset 2 in CLUT. */
    SYSTEM_DF_BITMAP1_OFFSET3,
    /**< BITMAP 1bpp offset 3 in CLUT. */
    SYSTEM_DF_BITMAP1_OFFSET4,
    /**< BITMAP 1bpp offset 4 in CLUT. */
    SYSTEM_DF_BITMAP1_OFFSET5,
    /**< BITMAP 1bpp offset 5 in CLUT. */
    SYSTEM_DF_BITMAP1_OFFSET6,
    /**< BITMAP 1bpp offset 6 in CLUT. */
    SYSTEM_DF_BITMAP1_OFFSET7,
    /**< BITMAP 1bpp offset 7 in CLUT. */
    SYSTEM_DF_BITMAP8_BGRA32,
    /**< BITMAP 8bpp BGRA32. */
    SYSTEM_DF_BITMAP4_BGRA32_LOWER,
    /**< BITMAP 4bpp BGRA32 lower address in CLUT. */
    SYSTEM_DF_BITMAP4_BGRA32_UPPER,
    /**< BITMAP 4bpp BGRA32 upper address in CLUT. */
    SYSTEM_DF_BITMAP2_BGRA32_OFFSET0,
    /**< BITMAP 2bpp BGRA32 offset 0 in CLUT. */
    SYSTEM_DF_BITMAP2_BGRA32_OFFSET1,
    /**< BITMAP 2bpp BGRA32 offset 1 in CLUT. */
    SYSTEM_DF_BITMAP2_BGRA32_OFFSET2,
    /**< BITMAP 2bpp BGRA32 offset 2 in CLUT. */
    SYSTEM_DF_BITMAP2_BGRA32_OFFSET3,
    /**< BITMAP 2bpp BGRA32 offset 3 in CLUT. */
    SYSTEM_DF_BITMAP1_BGRA32_OFFSET0,
    /**< BITMAP 1bpp BGRA32 offset 0 in CLUT. */
    SYSTEM_DF_BITMAP1_BGRA32_OFFSET1,
    /**< BITMAP 1bpp BGRA32 offset 1 in CLUT. */
    SYSTEM_DF_BITMAP1_BGRA32_OFFSET2,
    /**< BITMAP 1bpp BGRA32 offset 2 in CLUT. */
    SYSTEM_DF_BITMAP1_BGRA32_OFFSET3,
    /**< BITMAP 1bpp BGRA32 offset 3 in CLUT. */
    SYSTEM_DF_BITMAP1_BGRA32_OFFSET4,
    /**< BITMAP 1bpp BGRA32 offset 4 in CLUT. */
    SYSTEM_DF_BITMAP1_BGRA32_OFFSET5,
    /**< BITMAP 1bpp BGRA32 offset 5 in CLUT. */
    SYSTEM_DF_BITMAP1_BGRA32_OFFSET6,
    /**< BITMAP 1bpp BGRA32 offset 6 in CLUT. */
    SYSTEM_DF_BITMAP1_BGRA32_OFFSET7,
    /**< BITMAP 1bpp BGRA32 offset 7 in CLUT. */
    SYSTEM_DF_BAYER_RAW = 0x3000,
    /**< Bayer pattern. */
    SYSTEM_DF_RAW_VBI,
    /**< Raw VBI data. */
    SYSTEM_DF_RAW,
    /**< Raw data - Format not interpreted. */
    SYSTEM_DF_MISC,
    /**< For future purpose. */
    SYSTEM_DF_INVALID
    /**< Invalid data format. Could be used to initialize variables. */
}System_VideoDataFormat;


/**
 *  \brief Scan format.
 */
typedef enum
{
    SYSTEM_SF_INTERLACED = 0,
    /**< Interlaced mode. */
    SYSTEM_SF_PROGRESSIVE,
    /**< Progressive mode. */
    SYSTEM_SF_MAX
    /**< Should be the last value of this enumeration.
         Will be used by driver for validating the input parameters. */
} System_VideoScanFormat;

/**
 *  enum System_VpdmaMemoryType
 *  \brief Enum for buffer memory type.
 */
typedef enum
{
    SYSTEM_MT_NONTILEDMEM = 0,
    /**< 1D non-tiled memory. */
    SYSTEM_MT_TILEDMEM,
    /**< 2D tiled memory. */
    SYSTEM_MT_MAX
    /**< Should be the last value of this enumeration.
         Will be used by driver for validating the input parameters. */
} System_MemoryType;

/**
 *  \brief Video standards.
 */
typedef enum
{
    SYSTEM_STD_NTSC = 0u,
    /**< 720x480 30FPS interlaced NTSC standard. */
    SYSTEM_STD_PAL,
    /**< 720x576 30FPS interlaced PAL standard. */

    SYSTEM_STD_480I,
    /**< 720x480 30FPS interlaced SD standard. */
    SYSTEM_STD_576I,
    /**< 720x576 30FPS interlaced SD standard. */

    SYSTEM_STD_CIF,
    /**< Interlaced, 360x120 per field NTSC, 360x144 per field PAL. */
    SYSTEM_STD_HALF_D1,
    /**< Interlaced, 360x240 per field NTSC, 360x288 per field PAL. */
    SYSTEM_STD_D1,
    /**< Interlaced, 720x240 per field NTSC, 720x288 per field PAL. */

    SYSTEM_STD_480P,
    /**< 720x480 60FPS progressive ED standard. */
    SYSTEM_STD_576P,
    /**< 720x576 60FPS progressive ED standard. */

    SYSTEM_STD_720P_60,
    /**< 1280x720 60FPS progressive HD standard. */
    SYSTEM_STD_720P_50,
    /**< 1280x720 50FPS progressive HD standard. */

    SYSTEM_STD_1080I_60,
    /**< 1920x1080 30FPS interlaced HD standard. */
    SYSTEM_STD_1080I_50,
    /**< 1920x1080 50FPS interlaced HD standard. */

    SYSTEM_STD_1080P_60,
    /**< 1920x1080 60FPS progressive HD standard. */
    SYSTEM_STD_1080P_50,
    /**< 1920x1080 50FPS progressive HD standard. */

    SYSTEM_STD_1080P_24,
    /**< 1920x1080 24FPS progressive HD standard. */
    SYSTEM_STD_1080P_30,
    /**< 1920x1080 30FPS progressive HD standard. */

    /* Vesa standards from here Please add all SMTPE and CEA standard enums
       above this only. this is to ensure proxy Oses compatibility
     */
    SYSTEM_STD_VGA_60 = 0x100,
    /**< 640x480 60FPS VESA standard. */
    SYSTEM_STD_VGA_72,
    /**< 640x480 72FPS VESA standard. */
    SYSTEM_STD_VGA_75,
    /**< 640x480 75FPS VESA standard. */
    SYSTEM_STD_VGA_85,
    /**< 640x480 85FPS VESA standard. */

    SYSTEM_STD_WVGA_60,
    /**< 800x480 60PFS WVGA */

    SYSTEM_STD_SVGA_60,
    /**< 800x600 60FPS VESA standard. */
    SYSTEM_STD_SVGA_72,
    /**< 800x600 72FPS VESA standard. */
    SYSTEM_STD_SVGA_75,
    /**< 800x600 75FPS VESA standard. */
    SYSTEM_STD_SVGA_85,
    /**< 800x600 85FPS VESA standard. */

    SYSTEM_STD_WSVGA_70,
    /**< 1024x600 70FPS standard. */

    SYSTEM_STD_XGA_60,
    /**< 1024x768 60FPS VESA standard. */
    SYSTEM_STD_XGA_70,
    /**< 1024x768 72FPS VESA standard. */
    SYSTEM_STD_XGA_75,
    /**< 1024x768 75FPS VESA standard. */
    SYSTEM_STD_XGA_85,
    /**< 1024x768 85FPS VESA standard. */

    SYSTEM_STD_1368_768_60,
    /**< 1368x768 60 PFS VESA>*/
    SYSTEM_STD_1366_768_60,
    /**< 1366x768 60 PFS VESA>*/
    SYSTEM_STD_1360_768_60,
    /**< 1360x768 60 PFS VESA>*/

    SYSTEM_STD_WXGA_60,
    /**< 1280x768 60FPS VESA standard. */
    SYSTEM_STD_WXGA_75,
    /**< 1280x768 75FPS VESA standard. */
    SYSTEM_STD_WXGA_85,
    /**< 1280x768 85FPS VESA standard. */

    SYSTEM_STD_1440_900_60,
    /**< 1440x900 60 PFS VESA>*/

    SYSTEM_STD_SXGA_60,
    /**< 1280x1024 60FPS VESA standard. */
    SYSTEM_STD_SXGA_75,
    /**< 1280x1024 75FPS VESA standard. */
    SYSTEM_STD_SXGA_85,
    /**< 1280x1024 85FPS VESA standard. */

    SYSTEM_STD_WSXGAP_60,
    /**< 1680x1050 60 PFS VESA>*/

    SYSTEM_STD_SXGAP_60,
    /**< 1400x1050 60FPS VESA standard. */
    SYSTEM_STD_SXGAP_75,
    /**< 1400x1050 75FPS VESA standard. */

    SYSTEM_STD_UXGA_60,
    /**< 1600x1200 60FPS VESA standard. */

    SYSTEM_STD_QVGA_60,
    /**< 320x240 60FPS VESA standard. */

    /* Multi channel standards from here Please add all VESA standards enums
       above this only. this is to ensure proxy Oses compatibility */
    SYSTEM_STD_MUX_2CH_D1 = 0x200,
    /**< Interlaced, 2Ch D1, NTSC or PAL. */
    SYSTEM_STD_MUX_2CH_HALF_D1,
    /**< Interlaced, 2ch half D1, NTSC or PAL. */
    SYSTEM_STD_MUX_2CH_CIF,
    /**< Interlaced, 2ch CIF, NTSC or PAL. */
    SYSTEM_STD_MUX_4CH_D1,
    /**< Interlaced, 4Ch D1, NTSC or PAL. */
    SYSTEM_STD_MUX_4CH_CIF,
    /**< Interlaced, 4Ch CIF, NTSC or PAL. */
    SYSTEM_STD_MUX_4CH_HALF_D1,
    /**< Interlaced, 4Ch Half-D1, NTSC or PAL. */
    SYSTEM_STD_MUX_8CH_CIF,
    /**< Interlaced, 8Ch CIF, NTSC or PAL. */
    SYSTEM_STD_MUX_8CH_HALF_D1,
    /**< Interlaced, 8Ch Half-D1, NTSC or PAL. */

	/* Megapixel resolution in 4:3 */
    SYSTEM_STD_5MP_2560_1920 = 0x250,
    SYSTEM_STD_8MP_3264_2448,
    SYSTEM_STD_10MP_3648_2736,

    /* Auto detect and Custom standards Please add all multi channel standard
       enums above this only. this is to ensure proxy Oses compatibility */
    SYSTEM_STD_AUTO_DETECT = 0x300,
    /**< Auto-detect standard. Used in capture mode. */
    SYSTEM_STD_CUSTOM
    /**< Custom standard used when connecting to external LCD etc...
         The video timing is provided by the application.
         Used in display mode. */
} SYSTEM_Standard;

/**
 *  \brief Platform ID.
 */
typedef enum
{
    SYSTEM_PLATFORM_ID_UNKNOWN,
    /**< Unknown platform. */
    SYSTEM_PLATFORM_ID_EVM_TI816x,
    /**< TI816x EVMs. */
    SYSTEM_PLATFORM_ID_SIM_TI816x,
    /**< TI816x Simulator. */
    SYSTEM_PLATFORM_ID_EVM_TI814x,
    /**< TI814x EVMs. */
    SYSTEM_PLATFORM_ID_SIM_TI814x,
    /**< TI814x Simulator. */
    SYSTEM_PLATFORM_ID_EVM_TI8107,
    /**< TI8107 EVMs. */
    SYSTEM_PLATFORM_ID_MAX
    /**< Max Platform ID. */
} System_PlatformId;

/**
 *  \brief EVM Board ID.
 */
typedef enum
{
    SYSTEM_PLATFORM_BOARD_UNKNOWN,
    /**< Unknown board. */
    SYSTEM_PLATFORM_BOARD_VS,
    /**< TVP5158 based board. */
    SYSTEM_PLATFORM_BOARD_VC,
    /**< TVP7002/SII9135 based board. */
    SYSTEM_PLATFORM_BOARD_CATALOG,
    /**< TVP7002/SIL1161A. */
    SYSTEM_PLATFORM_BOARD_UBX,
    /**< BUS/CAR */
    SYSTEM_PLATFORM_BOARD_MAX
    /**< Max board ID. */
} System_PlatformBoardId;

/**
 *  \brief CPU revision ID.
 */
typedef enum
{
    SYSTEM_PLATFORM_CPU_REV_1_0,
    /**< CPU revision 1.0. */
    SYSTEM_PLATFORM_CPU_REV_1_1,
    /**< CPU revision 1.1. */
    SYSTEM_PLATFORM_CPU_REV_2_0,
    /**< CPU revision 2.0. */
    SYSTEM_PLATFORM_CPU_REV_2_1,
    /**< CPU revision 2.1. */
    SYSTEM_PLATFORM_CPU_REV_UNKNOWN,
    /**< Unknown/unsupported CPU revision. */
    SYSTEM_PLATFORM_CPU_REV_MAX
    /**< Max CPU revision. */
} System_PlatformCpuRev;

/**
 *  \brief Board revision ID.
 */
typedef enum
{
    SYSTEM_PLATFORM_BOARD_REV_UNKNOWN,
    /**< Unknown/unsupported board revision. */
    SYSTEM_PLATFORM_BOARD_REV_A,
    /**< Board revision A. */
    SYSTEM_PLATFORM_BOARD_REV_B,
    /**< Board revision B. */
    SYSTEM_PLATFORM_BOARD_REV_C,
    /**< Board revision B. */
    SYSTEM_PLATFORM_BOARD_DVR_REV_NONE,
    /**< Board DVR. */
    SYSTEM_PLATFORM_BOARD_REV_MAX
    /**< Max board revision. */
} System_PlatformBoardRev;

/**
    \brief Buffer type - Video frame or Video bitstream
*/
typedef enum  {

    SYSTEM_BUF_TYPE_VIDFRAME,
    SYSTEM_BUF_TYPE_VIDBITSTREAM,
    SYSTEM_BUF_TYPE_MAX

} System_BufType;

/* \brief Enum for selecting filter for component input/output in THS7360 */
typedef enum
{
    SYSTEM_THS7360_DISABLE_SF,
    /**< Disable THS7360 SF */
    SYSTEM_THS7360_BYPASS_SF,
    /**< Bypass THS7360 SF */
    SYSTEM_THS7360_SF_SD_MODE,
    /**< Select 9.5 MHs Filter to SD mode in THS7360 */
    SYSTEM_THS7360_SF_ED_MODE,
    /**< Select 18 MHs Filter to ED mode in THS7360 */
    SYSTEM_THS7360_SF_HD_MODE,
    /**< Select 36 MHs Filter to HD mode in THS7360 */
    SYSTEM_THS7360_SF_TRUE_HD_MODE
    /**< Select 72 MHs Filter to True HD mode in THS7360 */
} System_Ths7360SfCtrl;

/* \brief Enum to enable/bypass/disable THS360 SD and THS7535 modules */
typedef enum
{
    SYSTEM_THSFILTER_ENABLE_MODULE,
    /**< Enable the Module */
    SYSTEM_THSFILTER_BYPASS_MODULE,
    /**< Bypass the Module */
    SYSTEM_THSFILTER_DISABLE_MODULE
    /**< Disable the module */
} System_ThsFilterCtrl;


/* \brief Enum for VENCs present in the system */
typedef enum
{
    SYSTEM_DC_VENC_HDMI = 0,
    /**< Enable the Module */
    SYSTEM_DC_VENC_HDCOMP = 1,
    /**< Enable the Module */
    SYSTEM_DC_VENC_DVO2 = 2,
    /**< Enable the Module */
    SYSTEM_DC_VENC_SD = 3,
    /**< Enable the Module */
    SYSTEM_DC_MAX_VENC = 4
    /**< Enable the Module */
} System_VencTypes;

/**
 *  \brief Enum for the output clock modules
 *  Caution: Do not change the enum values.
 */
typedef enum
{
    SYSTEM_VPLL_OUTPUT_VENC_RF = 0,
    /**< Pixel clock frequency for the RF,
         Note: SD Pixel frequency will RF Clock Freq/4. */
    SYSTEM_VPLL_OUTPUT_VENC_D = 1,
    /**< VencD output clock. */
    SYSTEM_VPLL_OUTPUT_VENC_A = 2,
    /**< VencA output clock. */
    SYSTEM_VPLL_OUTPUT_MAX_VENC = 3
    /**< This should be last Enum. */
} System_VPllOutputClk;

typedef enum
{
    SYSTEM_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC = 0,
    /**< Single Channel non multiplexed mode */
    SYSTEM_CAPTURE_MODE_MULTI_CH_LINE_MUX_EMBEDDED_SYNC,
    /**< Multi-channel line-multiplexed mode */
    SYSTEM_CAPTURE_MODE_MULTI_CH_PIXEL_MUX_EMBEDDED_SYNC,
    /**< Multi-channel pixel muxed */
    SYSTEM_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_HSYNC_VBLK,
    /**< Single Channel non multiplexed discrete sync mode with HSYNC and
        VBLK as control signals. */
    SYSTEM_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_HSYNC_VSYNC,
    /**< Single Channel non multiplexed discrete sync mode with HSYNC and
        VSYNC as control signals. */
    SYSTEM_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_ACTVID_VBLK,
    /**< Single Channel non multiplexed discrete sync mode with ACTVID and
        VBLK as control signals. */
    SYSTEM_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_ACTVID_VSYNC,
    /**< Single Channel non multiplexed discrete sync mode with ACTVID and
        VBLK as control signals. */
    SYSTEM_CAPTURE_MODE_MULTI_CH_LINE_MUX_SPLIT_LINE_EMBEDDED_SYNC,
    /**< Multi-channel line-multiplexed mode - split line mode */
    SYSTEM_CAPTURE_MODE_MAX
    /**< Maximum modes */
} System_CaptVideoCaptureMode;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif


/* @} */

