/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

/**
    \ingroup MCFW_API
    \defgroup MCFW_SYS_API McFW System Level (VSYS) API

    @{
*/

/**
    \file ti_vsys.h
    \brief McFW System Level (VSYS) API
*/

#ifndef __TI_VSYS_H__
#define __TI_VSYS_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "ti_media_common_def.h"
#include "common_def/ti_vsys_common_def.h"

/* =============================================================================
 * Defines
 * =============================================================================
 */

/** \brief Floor a integer value. */
#define VsysUtils_floor(val, align)  (((val) / (align)) * (align))

/** \brief Align a integer value. */
#define VsysUtils_align(val, align)  VsysUtils_floor(((val) + (align)-1), (align))

/* =============================================================================
 * Enums
 * =============================================================================
 */


/**
    \brief User specified callback that gets called when a event occurs on the slave processor

    \param eventId  [OUT] This tells the user which event occured
                    based on this user can call other McFW APIs to get more details about the event

    \param appData  [OUT] User specified app data pointer during Vsys_registerEventHandler()
                    is returned here to the user.

    \return ERROR_NONE on sucess
*/
typedef Int32 (*VSYS_EVENT_HANDLER_CALLBACK)(UInt32 eventId, Ptr pPrm, Ptr appData);

/**
    \brief McFW Sub-system's or modules
*/
typedef enum
{
    VCAP,
    /**< Video Capture Sub-system */

    VENC,
    /**< Video Encode Sub-system */

    VDEC,
    /**< Video Decode Sub-system */

    VDIS,
    /**< Video Display Sub-system */

    VSYS_MODULES_MAX
    /**< Max sub-system's in McFW */

} VSYS_MODULES_E;

/**
    \brief System use-case's

    Inter-connection of sub-system's and internal HW processing
    block's depends on the system use-case that is selected.
*/
typedef enum
{
    //# usecase for ubx
    VSYS_USECASE_CAPTURE,
	VSYS_USECASE_DECODE,

	VSYS_USECASE_MAX
    /**< Maximum use-case ID */

} VSYS_USECASES_E;

typedef enum
{
	CAPT_MODE_D1,
	CAPT_MODE_720P,
	CAPT_MODE_1080P,

	CAPT_MODE_720P_D1,
	CAPT_MODE_1080P_720P,

	CAPT_MODE_MAX
} VSYS_CAPTMODE_E;

/* =============================================================================
 * Structures
 * =============================================================================
 */

/**
    \brief Top level System Configuration structure
*/
typedef struct
{
    Bool enableEncode;
    /**< Enable/Disable VENC subsystem */

    Bool enableDecode;
    /**< Enable/Disable VDEC subsystem */

    Bool enableMjpeg;
    /**< Enable/Disable MjpegEnc */

    Bool enableDual;
    /**< Enable/Disable Dual stream */

    Bool enableLcd;
	Bool enableHDMI;	//# ubx used only

    UInt32 numChs;
    /**< Number of channels */

	UInt32 numFhdChs;
    UInt32 numHdChs;
    UInt32 numD1Chs;

    UInt32 captMode;
    /**< capture mode FHD/HD/D1 */

    VSYS_USECASES_E systemUseCase;
    /**< System use-case to use */

    UInt32 decoderHD;
    UInt32 decoderD1;

    UInt32 serdesEQ;	//# ph3100k equalizer/preemphasis level

} VSYS_PARAMS_S;

typedef enum {

    VSYS_SR0_SHAREMEM = 0,

    VSYS_SR1_SHAREMEM,

    VSYS_SR2_SHAREMEM,

    VSYS_SR3_SHAREMEM
} VSYS_SR_DESC;

/**
    \brief Allocated buffer info
*/
typedef struct {

    UInt8  *physAddr;
    /**< Physical address */

    UInt8  *virtAddr;
    /**< Virtual address */

    UInt32  srPtr;
    /**< Shared region Pointer SRPtr */

} Vsys_AllocBufInfo;

/**
    \brief Top level System Configuration structure
*/
typedef struct {
    UInt32 streamId;

	UInt32 enable;	//# window enable/disable
	UInt32 volt;
	UInt32 temp;
	UInt32 speed;
	Int32 x;
	Int32 y;
	Int32 z;
    UInt8 usrString[32];

} Vsys_swOsdPrm;	//# should same as the SwosdLink_GuiParams;

typedef enum {
    VSYS_SWOSDDATETIME,	/**< Set SW OSD Date Time params */
    VSYS_SWOSD_EN_DATETIME,
    VSYS_SWOSD_GPS,
    VSYS_SWOSD_PARK,
    VSYS_SWOSD_EVENT,
    VSYS_SWOSD_MOTION,
	VSYS_SWOSD_CALLSTAT,
    VSYS_SWOSD_VOLTAGE,
    VSYS_SWOSD_SPEED,
    VSYS_SWOSD_GSENS,
    VSYS_SWOSD_USERSTR,
    VSYS_SWOSD_ENABLE,
    VSYS_SWOSD_MAX

} VSYS_SWOSD_SETPARAM;

typedef enum {

    VSYS_VAGUI_SETUP,
    /**< Set VA GUI params */

	VSYS_VAGUI_SETUPALGOFRAMERATE,
	/**< Set VA GUI params */

	VSYS_VAGUI_SETUPALGODETECTFREQ,
	/**< Set VA GUI params */

	VSYS_VAGUI_SETUPEVTRECORDENABLE,
	/**< Set VA GUI params */

	VSYS_VAGUI_SETUPSMETATRACKENABLE,
	/**< Set VA GUI params */

    VSYS_VAGUI_SETUPTZPRM,
	/**< Set VA GUI params */

	VSYS_VAGUI_SETUPMAINPRM,
	/**< Set VA GUI params */

	VSYS_VAGUI_SETUPCTDPRM,
	/**< Set VA GUI params */

	VSYS_VAGUI_SETUPOCPRM,
	/**< Set VA GUI params */

	VSYS_VAGUI_SETUPSMETAPRM,
	/**< Set VA GUI params */

	VSYS_VAGUI_SETUPIMDPRM,
	/**< Set VA GUI params */

	VSYS_VAGUI_SETUPENCROIENABLE
	/**< Set VA GUI params */

} VSYS_VA_GUISETPARAM;

/**
    \brief Top level System Configuration structure
*/
typedef struct {

    UInt32 fdetect;
    /**< FD enable */

    UInt32 startX;
    /**< FD ROI start X */

    UInt32 startY;
    /**< FD ROI start Y */

    UInt32 width;
    /**< FD ROI width */

    UInt32 height;
    /**< FD ROI height */

    UInt32 fdconflevel;
    /**< FD confidence level */

    UInt32 fddirection;
    /**< FD direction */

    UInt32 pmask;
    /**< FD privacy mask */

} Vsys_fdPrm;


/**
    \brief Board Type detect call
*/
typedef struct {

    UInt32  boardRev;
    /**< boardRev */

    UInt32  cpuRev;
    /**< CPU Rev */

    UInt32  boardId;
    /** < Board ID */
} VSYS_PLATFORM_INFO_S;

/**
    \brief Top level System Configuration structure
*/
typedef struct {

	UInt32 dccSize;
    /**< DCC Buffer Size */

} Vsys_dccPrm;


/* =============================================================================
 * APIs
 * =============================================================================
 */

/**
    \brief Set VSYS_PARAMS_S to default parameters

    Call this before calling Vsys_init() and then overide
    the required parameters in user application.

    \param pContext     [OUT] VSYS_PARAMS_S initialized to default values

    \return ERROR_NONE on success
*/
Int32 Vsys_params_init(VSYS_PARAMS_S * pContext);

/**
    \brief Initialize system for a given system level configuration

    This should be the first McFW API that should be happen

    \param pContext     [IN] System configuration

    \return ERROR_NONE on success
*/
Int32 Vsys_init(VSYS_PARAMS_S * pContext);

/**
    \brief Create and setup processing block's based on sub-system configuration

    This API should be called after sub-system init's ( Vxxx_init() ).
    This API will allocate and setup all HW resource in order for the system
    use-case to run.
    Once this API succeds now the sub-system's can be started to start the system execution

    \return ERROR_NONE on success
*/
#if defined(LF_SYS_NEXXONE_VOIP) || defined(LF_SYS_NEXX360H)
Int32 Vsys_create(void);
#else
/* nexx360-->fitt360-->1 */
Int32 Vsys_create(int mode);
#endif

/**
    \brief Delete and de-init processing block's based on sub-system configuration

    This API should be called after all sub-system have been stopped ( Vxxx_stop() ).
    This API will de-allocate and bring all HW resources to a known state.

    Once this API succeds now the sub-system's can be exited using Vxxx_exit() APIs

    \return ERROR_NONE on success
*/
Int32 Vsys_delete();


/**
    \brief System de-init

    This is last API to be called. This API should be called after all sus-system exits have been
    called (Vxxx_exit()).

    \return ERROR_NONE on success
*/
Int32 Vsys_exit();


/**
    \brief Print detailed system statistics

    This is useful system debugging.

    \return ERROR_NONE on success
*/
Int32 Vsys_printDetailedStatistics();

/**
    \brief Print detailed buffer statistics of links <useful for debugging>

    This is useful system debugging.

    \return ERROR_NONE on success
*/
Int32 Vsys_printBufferStatistics();

/**
    \brief Allocate contigous buffer from a shared memory

    \param srRegId  [IN] Shared region ID
    \param bufSize  [IN] Buffer size in bytes
    \param bufAlign [IN] Buffer alignment in bytes
    \param bufInfo  [OUT] Allocated buffer info

    \return ERROR_NONE on success
*/
Int32 Vsys_allocBuf(UInt32 srRegId, UInt32 bufSize, UInt32 bufAlign, Vsys_AllocBufInfo *bufInfo);

/**
    \brief Free the buffer allocated from the shared region

    \param srRegId  [IN] Shared region ID
    \param virtAddr [IN] Buffer virtual address
    \param bufSize  [IN] Size of the buffer in bytes

    \return ERROR_NONE on success
*/
Int32 Vsys_freeBuf(UInt32 srRegId, UInt8 *virtAddr, UInt32 bufSize);

/**
    \brief Free the buffer allocated from the shared region

    \param srRegId  [IN] Shared region ID
    \param virtAddr [IN] Buffer virtual address
    \param bufSize  [IN] Size of the buffer in bytes

    \return ERROR_NONE on success
*/
VSYS_USECASES_E Vsys_getSystemUseCase();

/**
    \brief Free the buffer allocated from the shared region

    \param srRegId  [IN] Shared region ID
    \param virtAddr [IN] Buffer virtual address
    \param bufSize  [IN] Size of the buffer in bytes

    \return ERROR_NONE on success
*/
Int32 Vsys_setSwOsdPrm(VSYS_SWOSD_SETPARAM swOsdSetPrm,
                       Vsys_swOsdPrm * pSwOsdPrm);

/**
    \brief Free the buffer allocated from the shared region

    \param srRegId  [IN] Shared region ID
    \param virtAddr [IN] Buffer virtual address
    \param bufSize  [IN] Size of the buffer in bytes

    \return ERROR_NONE on success
*/
Int32 Vsys_setFdPrm(Vsys_fdPrm * pFdPrm);

/**
    \brief This function is called by McFW when it receives any event from the slave processor

    \param callback [IN] User specified callback
    \param appData  [IN] User specified application data pointer which is also returned during the callback

    \return ERROR_NONE on success
*/

/**
 * \brief:
 *      This function gives CPU ID, board type and revision.
 * \input:
 *      VSYS_PLATFORM_INFO_S sturcture
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vsys_getPlatformInfo(VSYS_PLATFORM_INFO_S *pInfo);

Int32 Vsys_registerEventHandler(VSYS_EVENT_HANDLER_CALLBACK callback, Ptr appData);


/**
 * \brief:
 *      This function gives context info.
 * \input:
 *      VSYS_PARAMS_S sturcture
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Void Vsys_getContext(VSYS_PARAMS_S * contextInfo);

/**
 * \brief:
 *      This function gives context info.
 * \input:
 *      VSYS_PARAMS_S sturcture
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vsys_datetimeCreate();

/**
 * \brief:
 *      This function gives context info.
 * \input:
 *      VSYS_PARAMS_S sturcture
 * \output:
 *      NA
 * \return
*       ERROR_NOERROR       --  while success
*       ERROR_CODE          --  refer for err defination
*/
Int32 Vsys_datetimeDelete();

Int32 Vsys_datetime_init(void);
Int32 Vsys_datetime_init2(int isdst);
void Vsys_get_device_status(Int32 *dev_iss, Int32 *dev_dss);

Void Vsys_switchFormatSD(Int32 standard);


void mcfw_linux_init(int pal, int resol);
void mcfw_linux_exit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


/* @} */
