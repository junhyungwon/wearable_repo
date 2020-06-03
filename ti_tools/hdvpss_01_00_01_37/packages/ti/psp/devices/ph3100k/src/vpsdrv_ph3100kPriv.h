/******************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	vpsdrv_ph3100kPriv.h
 * @brief
 * @author	phoong
 */
/*****************************************************************************/

#ifndef _VPSDRV_PH3100K_PRIV_H_
#define _VPSDRV_PH3100K_PRIV_H_

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
#include <ti/psp/devices/ph3100k/vpsdrv_ph3100k.h>
#include <ti/psp/platforms/vps_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define eprintf(x...) Vps_printf("\033[41m [ph3100k] err: \033[0m" x);
#ifdef SYS_DEBUG
#define dprintf(x...) Vps_printf(" [ph3100k] " x);
#else
#define dprintf(x...)
#endif

/** \brief Driver object state - NOT IN USE. */
#define VPS_PH3100K_OBJ_STATE_UNUSED    (0u)
/** \brief Driver object state - IN USE and IDLE. */
#define VPS_PH3100K_OBJ_STATE_IDLE      (1u)

#define TERM	0xFF
#define DELAY	0xFE

/*
 * PH3100K Register Offsets.
 */

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
/**
 *  \brief PH3100K driver handle object.
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

} Vps_ph3100kHandleObj;

/**
 *  \brief PH3100K Global driver object.
 */
typedef struct
{
	Semaphore_Handle        lock;
	/* Global driver lock. */
	Vps_ph3100kHandleObj    handlePool[VPS_DEVICE_MAX_HANDLES];
	/**< Handle objects. */
} Vps_ph3100kObj;


/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
Int32 Vps_ph3100kResetIoctl(Vps_ph3100kHandleObj *pObj);
Int32 Vps_ph3100kGetChipIdIoctl(Vps_ph3100kHandleObj *pObj,
			Vps_VideoDecoderChipIdParams *pPrm,
			Vps_VideoDecoderChipIdStatus *pStatus);
Int32 Vps_ph3100kSetVideoModeIoctl(Vps_ph3100kHandleObj *pObj,
			Vps_VideoDecoderVideoModeParams *pPrm);
Int32 Vps_ph3100kGetVideoStatusIoctl(Vps_ph3100kHandleObj *pObj,
			Vps_VideoDecoderVideoStatusParams *pPrm,
			Vps_VideoDecoderVideoStatus *pStatus);
Int32 Vps_ph3100kSensorDetect(Vps_ph3100kHandleObj *pObj);
Int32 Vps_ph3100kStartIoctl(Vps_ph3100kHandleObj *pObj);
Int32 Vps_ph3100kStopIoctl(Vps_ph3100kHandleObj *pObj);
Int32 Vps_ph3100kRegWriteIoctl(Vps_ph3100kHandleObj *pObj,
			Vps_VideoDecoderRegRdWrParams *pPrm);
Int32 Vps_ph3100kRegReadIoctl(Vps_ph3100kHandleObj *pObj,
			Vps_VideoDecoderRegRdWrParams *pPrm);

Int32 Vps_ph3100kSerdes_init(void);
Int32 Vps_ph3100kSerdes_deinit(void);
Int32 Vps_ph3100kInitSensor(Vps_ph3100kHandleObj *pObj);
Int32 Vps_ph3100kDeinitSensor(Vps_ph3100kHandleObj *pObj);

void Vps_ph3100kPinMux(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _VPSDRV_PH3100K_PRIV_H_ */
