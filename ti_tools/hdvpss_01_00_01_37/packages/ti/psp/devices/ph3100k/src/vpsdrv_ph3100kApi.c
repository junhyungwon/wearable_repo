/******************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	vpsdrv_ph3100kApi.c
 * @brief
 * @author	phoong
 * @section	MODIFY history
 *	   - 2012.01.01	: First	Created	based vpsdrv_tvp5158Api.c
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/psp/vps/common/vps_types.h>
#include <ti/psp/vps/common/vps_config.h>
#include <ti/psp/vps/common/trace.h>
#include <ti/psp/vps/common/vps_utils.h>
#include <ti/psp/vps/drivers/fvid2_drvMgr.h>

#include <ti/psp/devices/vps_videoDecoder.h>
#include <ti/psp/devices/ph3100k/vpsdrv_ph3100k.h>
#include <ti/psp/devices/ph3100k/src/vpsdrv_ph3100kPriv.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
/**
 *  \brief Global object storing all information related to all PH3100K driver
 *  handles.
 */
Vps_ph3100kObj gVpsph3100kObj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static Int32 Vps_ph3100kFreeObj(Vps_ph3100kHandleObj *pObj);
static Int32 Vps_ph3100kLockObj(Vps_ph3100kHandleObj *pObj);
static Int32 Vps_ph3100kUnlockObj(Vps_ph3100kHandleObj *pObj);
static Int32 Vps_ph3100kLock(void);
static Int32 Vps_ph3100kUnlock(void);

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

/**
 *  \brief Control API that gets called when FVID2_control is called.
 *
 *  This API does handle level semaphore locking
 *
 *  \param handle           [IN] PH3100K driver handle returned by
 *                          create function.
 *  \param cmd              [IN] Supports the standard video decoder interface
 *                          commands.
 *  \param cmdArgs          [IN] Depending on the command this will vary.
 *  \param cmdStatusArgs    [OUT] Depending on the command this will vary.
 *
 *  \return                 Returns FVID2_SOK on success else returns
 *                          appropriate error codes for illegal parameters and
 *                          I2C command RX/TX error.
 */
static Int32 Vps_ph3100kControl(Fdrv_Handle handle,
								UInt32 cmd,
								Ptr cmdArgs,
								Ptr cmdStatusArgs)
{
	Int32                   ret = FVID2_SOK;
	Vps_ph3100kHandleObj   *pObj = NULL;

	//dprintf("Vps_ph3100kControl 0x%x\n", cmd);

	/* Check parameters */
	if (NULL == handle)
	{
		GT_0trace(VpsDeviceTrace, GT_ERR, "Null pointer\n");
		ret = FVID2_EBADARGS;
	}

	if (FVID2_SOK == ret)
	{
		pObj = (Vps_ph3100kHandleObj *) handle;

		/* lock handle */
		Vps_ph3100kLockObj(pObj);

		switch (cmd)
		{
			case FVID2_START:
				ret = Vps_ph3100kStartIoctl(pObj);
				break;

			case FVID2_STOP:
				ret = Vps_ph3100kStopIoctl(pObj);
				break;

			case IOCTL_VPS_VIDEO_DECODER_RESET:
				ret = Vps_ph3100kResetIoctl(pObj);
				break;

			case IOCTL_VPS_VIDEO_DECODER_GET_CHIP_ID:
				ret = Vps_ph3100kGetChipIdIoctl(pObj, cmdArgs, cmdStatusArgs);
				break;

			case IOCTL_VPS_VIDEO_DECODER_SET_VIDEO_MODE:
				ret = Vps_ph3100kSetVideoModeIoctl(pObj, cmdArgs);
				break;

			case IOCTL_VPS_VIDEO_DECODER_GET_VIDEO_STATUS:
				ret = Vps_ph3100kGetVideoStatusIoctl(pObj, cmdArgs, cmdStatusArgs);
				break;

			case IOCTL_VPS_VIDEO_DECODER_SENSOR_DETECT:
				ret = Vps_ph3100kSensorDetect(pObj);
				break;

			case IOCTL_VPS_VIDEO_DECODER_REG_WRITE:
				ret = Vps_ph3100kRegWriteIoctl(pObj, cmdArgs);
				break;

			case IOCTL_VPS_VIDEO_DECODER_REG_READ:
				ret = Vps_ph3100kRegReadIoctl(pObj, cmdArgs);
				break;

			default:
				GT_0trace(VpsDeviceTrace, GT_ERR, "Unsupported command\n");
				ret = FVID2_EUNSUPPORTED_CMD;
				break;
		}
	}

	if (NULL != pObj)
	{
		/* Unlock handle */
		Vps_ph3100kUnlockObj(pObj);
	}

	return (ret);
}

/**
 *  \brief Allocates driver object.
 *
 *  Searches in list of driver handles and allocate's a 'NOT IN USE' handle
 *  Also create's handle level semaphore lock.
 *
 *  Returns NULL in case handle could not be allocated.
 */
static Vps_ph3100kHandleObj *Vps_ph3100kAllocObj(void)
{
	UInt32                  handleId;
	Semaphore_Params        semPrms;
	Vps_ph3100kHandleObj   *pObj = NULL;

	/* Take global lock to avoid race condition */
	Vps_ph3100kLock();

	/* Find a unallocated object in pool */
	for (handleId = 0u; handleId < VPS_DEVICE_MAX_HANDLES; handleId++)
	{
		if (VPS_PH3100K_OBJ_STATE_UNUSED ==
			gVpsph3100kObj.handlePool[handleId].state)
		{
			/* Free object found */
			pObj = &gVpsph3100kObj.handlePool[handleId];

			/* Init state and handle ID */
			VpsUtils_memset(pObj, 0, sizeof (*pObj));
			pObj->state = VPS_PH3100K_OBJ_STATE_IDLE;
			pObj->handleId = handleId;

			/* Create driver object specific semaphore lock */
			Semaphore_Params_init(&semPrms);
			semPrms.mode = Semaphore_Mode_BINARY;
			pObj->lock = Semaphore_create(1u, &semPrms, NULL);
			if (NULL == pObj->lock)
			{
				GT_0trace(VpsDeviceTrace, GT_ERR,
					"Handle semaphore create failed\n");
				/* Error - release object */
				pObj->state = VPS_PH3100K_OBJ_STATE_UNUSED;
				pObj = NULL;
			}
			break;
		}
	}

	/* Release global lock */
	Vps_ph3100kUnlock();

	return (pObj);
}

/**
 *  \brief De-Allocate driver object.
 *
 *  Marks handle as 'NOT IN USE'.
 *  Also delete's handle level semaphore lock.
 */
static Int32 Vps_ph3100kFreeObj(Vps_ph3100kHandleObj *pObj)
{
	/* Check for NULL pointers */
	GT_assert(VpsDeviceTrace, (NULL != pObj));

	/* Take global lock to avoid race condition */
	Vps_ph3100kLock();

	if (pObj->state != VPS_PH3100K_OBJ_STATE_UNUSED)
	{
		/* Mark state as unused */
		pObj->state = VPS_PH3100K_OBJ_STATE_UNUSED;

		/* Delete object locking semaphore */
		Semaphore_delete(&pObj->lock);
	}

	/* Release global lock */
	Vps_ph3100kUnlock();

	return (FVID2_SOK);
}

/**
 *  \brief Handle level lock.
 */
static Int32 Vps_ph3100kLockObj(Vps_ph3100kHandleObj *pObj)
{
	/* Check for NULL pointers */
	GT_assert(VpsDeviceTrace, (NULL != pObj));

	Semaphore_pend(pObj->lock, BIOS_WAIT_FOREVER);

	return (FVID2_SOK);
}

/**
 *  \brief Handle level unlock
 */
static Int32 Vps_ph3100kUnlockObj(Vps_ph3100kHandleObj *pObj)
{
	/* Check for NULL pointers */
	GT_assert(VpsDeviceTrace, (NULL != pObj));

	Semaphore_post(pObj->lock);

	return (FVID2_SOK);
}

/**
 *  \brief Global driver level lock.
 */
static Int32 Vps_ph3100kLock(void)
{
	Semaphore_pend(gVpsph3100kObj.lock, BIOS_WAIT_FOREVER);

	return (FVID2_SOK);
}

/**
 *  \brief Global driver level unlock.
 */
static Int32 Vps_ph3100kUnlock(void)
{
	Semaphore_post(gVpsph3100kObj.lock);

	return (FVID2_SOK);
}

/**
 *  \brief PH3100K create API that gets called when FVID2_create is called.
 *
 *  This API does not configure the PH3100K is any way.
 *
 *  This API
 *      - validates parameters
 *      - allocates driver handle
 *      - stores create arguments in its internal data structure.
 *
 *  Later the create arguments will be used when doing I2C communcation with
 *  PH3100K.
 *
 *  \param drvId            [IN] Driver ID, must be
 *                          FVID2_VPS_VID_DEC_PH3100K_DRV.
 *  \param instId           [IN] Must be 0.
 *  \param createArgs       [IN] Create arguments - pointer to valid
 *                          Vps_VideoDecoderCreateParams. This parameter should
 *                          be non-NULL.
 *  \param createStatusArgs [OUT] PH3100K driver return parameter -
 *                          pointer to Vps_VideoDecoderCreateStatus.
 *                          This parameter could be NULL and the driver fills
 *                          the ret information only if this is not NULL.
 *  \param fdmCbPrms        [IN] Not used. Set to NULL
 *
 *  \return                 Returns NULL in case of any error. Otherwise
 *                          returns a valid handle.
 */
static Fdrv_Handle Vps_ph3100kCreate(UInt32 drvId,
									 UInt32 instId,
									 Ptr createArgs,
									 Ptr createStatusArgs,
									 const FVID2_DrvCbParams *fdmCbPrms)
{
	Int32                           ret = FVID2_SOK;
	Vps_ph3100kHandleObj           *pObj = NULL;
	Vps_VideoDecoderCreateParams   *vidDecCreateArgs;
	Vps_VideoDecoderCreateStatus   *vidDecCreateStatus;

	/* Check parameters */
	if ((NULL == createArgs) ||
		(drvId != FVID2_VPS_VID_DEC_PH3100K_DRV) ||
		(instId != 0u))
	{
		GT_0trace(VpsDeviceTrace, GT_ERR, "Null pointer/Invalid parameters\n");
		ret = FVID2_EBADARGS;
	}

	if (FVID2_SOK == ret)
	{
		vidDecCreateArgs = (Vps_VideoDecoderCreateParams *) createArgs;
		if (vidDecCreateArgs->deviceI2cInstId >= VPS_DEVICE_I2C_INST_ID_MAX)
		{
			GT_0trace(VpsDeviceTrace, GT_ERR, "Invalid I2C instance ID\n");
			ret = FVID2_EINVALID_PARAMS;
		}
	}

	if (FVID2_SOK == ret)
	{
		/* Allocate driver handle */
		pObj = Vps_ph3100kAllocObj();
		if (NULL == pObj)
		{
			GT_0trace(VpsDeviceTrace, GT_ERR, "Alloc object failed\n");
			ret = FVID2_EALLOC;
		}
	}

	if (FVID2_SOK == ret)
	{
		/* Copy parameters to allocate driver handle */
		VpsUtils_memcpy(
			&pObj->createArgs,
			vidDecCreateArgs,
			sizeof (*vidDecCreateArgs));
	}

	/* Fill the ret if possible */
	if (NULL != createStatusArgs)
	{
		vidDecCreateStatus = (Vps_VideoDecoderCreateStatus *) createStatusArgs;
		vidDecCreateStatus->retVal = ret;
	}

	pObj->serdes_eq = vidDecCreateArgs->serdesEQ;

	Vps_ph3100kInitSensor(pObj);

	dprintf("%s: ...done!\n", __func__);

	return (pObj);
}

/**
 *  \brief Delete function that is called when FVID2_delete is called.
 *
 *  This API
 *      - free's driver handle object
 *
 *  \param handle           [IN] Driver handle.
 *  \param deleteArgs       [IN] Not used currently. Meant for future purpose.
 *                          Set this to NULL.
 *
 *  \return                 Returns FVID2_SOK on success else returns
 *                          appropriate error code. *
 */
static Int32 Vps_ph3100kDelete(Fdrv_Handle handle, Ptr deleteArgs)
{
	Int32                   ret = FVID2_SOK;
	Vps_ph3100kHandleObj   *pObj;

	/* Check parameters */
	if (NULL == handle)
	{
		eprintf("%s: Null pointer\n", __func__);
		ret = FVID2_EBADARGS;
	}

	pObj = (Vps_ph3100kHandleObj *) handle;

	Vps_ph3100kDeinitSensor(pObj);

	/* Free driver handle object */
	ret = Vps_ph3100kFreeObj(pObj);
	if (FVID2_SOK != ret) {
		eprintf("%s: Free object failed\n", __func__);
	}

	dprintf("%s: ...done!\n", __func__);

	return (ret);
}

/*----------------------------------------------------------------------------
 PH3100K driver function pointer
-----------------------------------------------------------------------------*/
static const FVID2_DrvOps gVpsph3100kDrvOps =
{
	FVID2_VPS_VID_DEC_PH3100K_DRV,	/* Driver ID */
	Vps_ph3100kCreate,				/* Create */
	Vps_ph3100kDelete,				/* Delete */
	Vps_ph3100kControl,				/* Control */
	NULL,							/* Queue */
	NULL,							/* Dequeue */
	NULL,							/* ProcessFrames */
	NULL							/* GetProcessedFrames */
};

/*****************************************************************************
* @brief	System init for PH3100K driver
* @section	DESC Description
*	- desc :
*		. create semaphore locks needed
*		. registers driver to FVID2 sub-system
*		. gets called as part of Vps_deviceInit()
*****************************************************************************/
Int32 Vps_ph3100kInit(void)
{
	Int32               ret = FVID2_SOK;
	Semaphore_Params    semPrms;

	Vps_ph3100kPinMux();

	/* Memset global object */
	VpsUtils_memset(&gVpsph3100kObj, 0, sizeof (gVpsph3100kObj));

	/* Create global PH3100K lock */
	Semaphore_Params_init(&semPrms);
	semPrms.mode = Semaphore_Mode_BINARY;
	gVpsph3100kObj.lock = Semaphore_create(1u, &semPrms, NULL);
	if (NULL == gVpsph3100kObj.lock)
	{
		GT_0trace(VpsDeviceTrace, GT_ERR, "Global semaphore create failed\n");
		ret = FVID2_EALLOC;
	}

	if (FVID2_SOK == ret)
	{
		/* Register PH3100K driver with FVID2 sub-system */
		ret = FVID2_registerDriver(&gVpsph3100kDrvOps);
		if (FVID2_SOK != ret)
		{
			GT_0trace(VpsDeviceTrace, GT_ERR, "Registering to FVID2 driver manager failed\n");
			/* Error - free acquired resources */
			Semaphore_delete(&gVpsph3100kObj.lock);
		}
	}

	if (FVID2_SOK == ret) {
		ret = Vps_ph3100kSerdes_init();
	}

	if(ret == FVID2_SOK) {
		Vps_printf(" [ph3100k] %s done\n", __func__);
	} else {
		eprintf("%s fail!\n", __func__);
	}

	return (ret);
}

/*****************************************************************************
* @brief	System de-init for PH3100K driver
* @section	DESC Description
*	- desc :
*		. de-registers driver from FVID2 sub-system
*		. delete's allocated semaphore locks
*		. gets called as part of Vps_deviceDeInit()
*****************************************************************************/
Int32 Vps_ph3100kDeInit(void)
{
	Vps_ph3100kSerdes_deinit();

	/* Unregister FVID2 driver */
	FVID2_unRegisterDriver(&gVpsph3100kDrvOps);

	/* Delete semaphore's. */
	Semaphore_delete(&gVpsph3100kObj.lock);

	dprintf(" %s done!\n", __func__);

	return (FVID2_SOK);
}
