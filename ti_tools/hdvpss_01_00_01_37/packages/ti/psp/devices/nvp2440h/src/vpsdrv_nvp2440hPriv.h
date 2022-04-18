/******************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	vpsdrv_nvp2440hPriv.h
 * @brief
 * @author	phoong
 */
/*****************************************************************************/

#ifndef _VPSDRV_NVP2440H_PRIV_H_
#define _VPSDRV_NVP2440H_PRIV_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/psp/vps/common/vps_types.h>
#include <ti/psp/vps/common/vps_config.h>
#include <ti/psp/vps/common/trace.h>
#include <ti/psp/vps/common/vps_utils.h>
#include <ti/psp/devices/vps_videoDecoder.h>
#include <ti/psp/devices/nvp2440h/vpsdrv_nvp2440h.h>
#include <ti/psp/platforms/vps_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define eprintf(x...) Vps_printf("\033[41m [nvp2440h] err: \033[0m" x);
#ifdef SYS_DEBUG
#define dprintf(x...) Vps_printf(" [nvp2440h] " x);
#else
#define dprintf(x...)
#endif

/** \brief Driver object state - NOT IN USE. */
#define VPS_NVP2440H_OBJ_STATE_UNUSED    (0u)
/** \brief Driver object state - IN USE and IDLE. */
#define VPS_NVP2440H_OBJ_STATE_IDLE      (1u)

/*
 * NVP2440H Register Offsets.
 */

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
/**
 *  \brief NVP2440H driver handle object.
 */
typedef struct
{
	UInt32                  state;
	/**< Handle state - used or free. */
	UInt32                  handleId;
	/**< Handle ID, 0..VPS_DEVICE_MAX_HANDLES-1. */
	Semaphore_Handle        lock;
	/**< Driver lock. */
	Vps_VideoDecoderCreateParams createArgs;
	/**< Create time arguments. */
	UInt32 					serdes_eq;
	/**< device equalizer/preemphasis level */

} Vps_nvp2440hHandleObj;

/**
 *  \brief NVP2440H Global driver object.
 */
typedef struct
{
	Semaphore_Handle        lock;
	/* Global driver lock. */
	Vps_nvp2440hHandleObj    handlePool[VPS_DEVICE_MAX_HANDLES];
	/**< Handle objects. */
} Vps_nvp2440hObj;


/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
Int32 Vps_nvp2440hResetIoctl(Vps_nvp2440hHandleObj *pObj);
Int32 Vps_nvp2440hGetChipIdIoctl(Vps_nvp2440hHandleObj *pObj,
			Vps_VideoDecoderChipIdParams *pPrm,
			Vps_VideoDecoderChipIdStatus *pStatus);
Int32 Vps_nvp2440hSetVideoModeIoctl(Vps_nvp2440hHandleObj *pObj,
			Vps_VideoDecoderVideoModeParams *pPrm);
Int32 Vps_nvp2440hGetVideoStatusIoctl(Vps_nvp2440hHandleObj *pObj,
			Vps_VideoDecoderVideoStatusParams *pPrm,
			Vps_VideoDecoderVideoStatus *pStatus);
Int32 Vps_nvp2440hSensorDetect(Vps_nvp2440hHandleObj *pObj);
Int32 Vps_nvp2440hStartIoctl(Vps_nvp2440hHandleObj *pObj);
Int32 Vps_nvp2440hStopIoctl(Vps_nvp2440hHandleObj *pObj);
Int32 Vps_nvp2440hRegWriteIoctl(Vps_nvp2440hHandleObj *pObj,
			Vps_VideoDecoderRegRdWrParams *pPrm);
Int32 Vps_nvp2440hRegReadIoctl(Vps_nvp2440hHandleObj *pObj,
			Vps_VideoDecoderRegRdWrParams *pPrm);

Int32 Vps_nvp2440hSerdes_init(void);
Int32 Vps_nvp2440hSerdes_deinit(void);
Int32 Vps_nvp2440hInitSensor(Vps_nvp2440hHandleObj *pObj);
Int32 Vps_nvp2440hDeinitSensor(Vps_nvp2440hHandleObj *pObj);

void Vps_nvp2440hPinMux(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _VPSDRV_NVP2440H_PRIV_H_ */
