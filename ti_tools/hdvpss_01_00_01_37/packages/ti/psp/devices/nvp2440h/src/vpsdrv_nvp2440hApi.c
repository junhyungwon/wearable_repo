/******************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	vpsdrv_nvp2440hApi.c
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
#include <ti/psp/devices/nvp2440h/vpsdrv_nvp2440h.h>
#include <ti/psp/devices/nvp2440h/src/vpsdrv_nvp2440hPriv.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
/**
 *  \brief Global object storing all information related to all NVP2440H driver
 *  handles.
 */
Vps_nvp2440hObj gVpsnvp2440hObj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static Int32 Vps_nvp2440hFreeObj(Vps_nvp2440hHandleObj *pObj);
static Int32 Vps_nvp2440hLockObj(Vps_nvp2440hHandleObj *pObj);
static Int32 Vps_nvp2440hUnlockObj(Vps_nvp2440hHandleObj *pObj);
static Int32 Vps_nvp2440hLock(void);
static Int32 Vps_nvp2440hUnlock(void);

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

/**
 *  \brief Control API that gets called when FVID2_control is called.
 *
 *  This API does handle level semaphore locking
 *
 *  \param handle           [IN] NVP2440H driver handle returned by
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
static Int32 Vps_nvp2440hControl(Fdrv_Handle handle,
								UInt32 cmd,
								Ptr cmdArgs,
								Ptr cmdStatusArgs)
{
	Int32                   ret = FVID2_SOK;
	Vps_nvp2440hHandleObj   *pObj = NULL;

	//dprintf("Vps_nvp2440hControl 0x%x\n", cmd);

	/* Check parameters */
	if (NULL == handle)
	{
		GT_0trace(VpsDeviceTrace, GT_ERR, "Null pointer\n");
		ret = FVID2_EBADARGS;
	}

	if (FVID2_SOK == ret)
	{
		pObj = (Vps_nvp2440hHandleObj *) handle;

		/* lock handle */
		Vps_nvp2440hLockObj(pObj);

		switch (cmd)
		{
			case FVID2_START:
				ret = Vps_nvp2440hStartIoctl(pObj);
				break;

			case FVID2_STOP:
				ret = Vps_nvp2440hStopIoctl(pObj);
				break;

			case IOCTL_VPS_VIDEO_DECODER_RESET:
				ret = Vps_nvp2440hResetIoctl(pObj);
				break;

			case IOCTL_VPS_VIDEO_DECODER_GET_CHIP_ID:
				ret = Vps_nvp2440hGetChipIdIoctl(pObj, cmdArgs, cmdStatusArgs);
				break;

			case IOCTL_VPS_VIDEO_DECODER_SET_VIDEO_MODE:
				ret = Vps_nvp2440hSetVideoModeIoctl(pObj, cmdArgs);
				break;

			case IOCTL_VPS_VIDEO_DECODER_GET_VIDEO_STATUS:
				ret = Vps_nvp2440hGetVideoStatusIoctl(pObj, cmdArgs, cmdStatusArgs);
				break;

			case IOCTL_VPS_VIDEO_DECODER_SENSOR_DETECT:
				ret = Vps_nvp2440hSensorDetect(pObj);
				break;

			case IOCTL_VPS_VIDEO_DECODER_REG_WRITE:
				ret = Vps_nvp2440hRegWriteIoctl(pObj, cmdArgs);
				break;

			case IOCTL_VPS_VIDEO_DECODER_REG_READ:
				ret = Vps_nvp2440hRegReadIoctl(pObj, cmdArgs);
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
		Vps_nvp2440hUnlockObj(pObj);
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
static Vps_nvp2440hHandleObj *Vps_nvp2440hAllocObj(void)
{
	UInt32                  handleId;
	Semaphore_Params        semPrms;
	Vps_nvp2440hHandleObj   *pObj = NULL;

	/* Take global lock to avoid race condition */
	Vps_nvp2440hLock();

	/* Find a unallocated object in pool */
	for (handleId = 0u; handleId < VPS_DEVICE_MAX_HANDLES; handleId++)
	{
		if (VPS_NVP2440H_OBJ_STATE_UNUSED ==
			gVpsnvp2440hObj.handlePool[handleId].state)
		{
			/* Free object found */
			pObj = &gVpsnvp2440hObj.handlePool[handleId];

			/* Init state and handle ID */
			VpsUtils_memset(pObj, 0, sizeof (*pObj));
			pObj->state = VPS_NVP2440H_OBJ_STATE_IDLE;
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
				pObj->state = VPS_NVP2440H_OBJ_STATE_UNUSED;
				pObj = NULL;
			}
			break;
		}
	}

	/* Release global lock */
	Vps_nvp2440hUnlock();

	return (pObj);
}

/**
 *  \brief De-Allocate driver object.
 *
 *  Marks handle as 'NOT IN USE'.
 *  Also delete's handle level semaphore lock.
 */
static Int32 Vps_nvp2440hFreeObj(Vps_nvp2440hHandleObj *pObj)
{
	/* Check for NULL pointers */
	GT_assert(VpsDeviceTrace, (NULL != pObj));

	/* Take global lock to avoid race condition */
	Vps_nvp2440hLock();

	if (pObj->state != VPS_NVP2440H_OBJ_STATE_UNUSED)
	{
		/* Mark state as unused */
		pObj->state = VPS_NVP2440H_OBJ_STATE_UNUSED;

		/* Delete object locking semaphore */
		Semaphore_delete(&pObj->lock);
	}

	/* Release global lock */
	Vps_nvp2440hUnlock();

	return (FVID2_SOK);
}

/**
 *  \brief Handle level lock.
 */
static Int32 Vps_nvp2440hLockObj(Vps_nvp2440hHandleObj *pObj)
{
	/* Check for NULL pointers */
	GT_assert(VpsDeviceTrace, (NULL != pObj));

	Semaphore_pend(pObj->lock, BIOS_WAIT_FOREVER);

	return (FVID2_SOK);
}

/**
 *  \brief Handle level unlock
 */
static Int32 Vps_nvp2440hUnlockObj(Vps_nvp2440hHandleObj *pObj)
{
	/* Check for NULL pointers */
	GT_assert(VpsDeviceTrace, (NULL != pObj));

	Semaphore_post(pObj->lock);

	return (FVID2_SOK);
}

/**
 *  \brief Global driver level lock.
 */
static Int32 Vps_nvp2440hLock(void)
{
	Semaphore_pend(gVpsnvp2440hObj.lock, BIOS_WAIT_FOREVER);

	return (FVID2_SOK);
}

/**
 *  \brief Global driver level unlock.
 */
static Int32 Vps_nvp2440hUnlock(void)
{
	Semaphore_post(gVpsnvp2440hObj.lock);

	return (FVID2_SOK);
}

/**
 *  \brief NVP2440H create API that gets called when FVID2_create is called.
 *
 *  This API does not configure the NVP2440H is any way.
 *
 *  This API
 *      - validates parameters
 *      - allocates driver handle
 *      - stores create arguments in its internal data structure.
 *
 *  Later the create arguments will be used when doing I2C communcation with
 *  NVP2440H.
 *
 *  \param drvId            [IN] Driver ID, must be
 *                          FVID2_VPS_VID_DEC_NVP2440H_DRV.
 *  \param instId           [IN] Must be 0.
 *  \param createArgs       [IN] Create arguments - pointer to valid
 *                          Vps_VideoDecoderCreateParams. This parameter should
 *                          be non-NULL.
 *  \param createStatusArgs [OUT] NVP2440H driver return parameter -
 *                          pointer to Vps_VideoDecoderCreateStatus.
 *                          This parameter could be NULL and the driver fills
 *                          the ret information only if this is not NULL.
 *  \param fdmCbPrms        [IN] Not used. Set to NULL
 *
 *  \return                 Returns NULL in case of any error. Otherwise
 *                          returns a valid handle.
 */
static Fdrv_Handle Vps_nvp2440hCreate(UInt32 drvId,
									 UInt32 instId,
									 Ptr createArgs,
									 Ptr createStatusArgs,
									 const FVID2_DrvCbParams *fdmCbPrms)
{
	Int32                           ret = FVID2_SOK;
	Vps_nvp2440hHandleObj           *pObj = NULL;
	Vps_VideoDecoderCreateParams   *vidDecCreateArgs;
	Vps_VideoDecoderCreateStatus   *vidDecCreateStatus;

	/* Check parameters */
	if ((NULL == createArgs) ||
		(drvId != FVID2_VPS_VID_DEC_NVP2440H_DRV) ||
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
		pObj = Vps_nvp2440hAllocObj();
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

	Vps_nvp2440hInitSensor(pObj);

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
static Int32 Vps_nvp2440hDelete(Fdrv_Handle handle, Ptr deleteArgs)
{
	Int32                   ret = FVID2_SOK;
	Vps_nvp2440hHandleObj   *pObj;

	/* Check parameters */
	if (NULL == handle)
	{
		eprintf("%s: Null pointer\n", __func__);
		ret = FVID2_EBADARGS;
	}

	pObj = (Vps_nvp2440hHandleObj *) handle;

	Vps_nvp2440hDeinitSensor(pObj);

	/* Free driver handle object */
	ret = Vps_nvp2440hFreeObj(pObj);
	if (FVID2_SOK != ret) {
		eprintf("%s: Free object failed\n", __func__);
	}

	dprintf("%s: ...done!\n", __func__);

	return (ret);
}

/*----------------------------------------------------------------------------
 PH3100K driver function pointer
-----------------------------------------------------------------------------*/
static const FVID2_DrvOps gVpsnvp2440hDrvOps =
{
	FVID2_VPS_VID_DEC_NVP2440H_DRV,	/* Driver ID */
	Vps_nvp2440hCreate,				/* Create */
	Vps_nvp2440hDelete,				/* Delete */
	Vps_nvp2440hControl,				/* Control */
	NULL,							/* Queue */
	NULL,							/* Dequeue */
	NULL,							/* ProcessFrames */
	NULL							/* GetProcessedFrames */
};

/*****************************************************************************
* @brief	System init for NVP2440H driver
* @section	DESC Description
*	- desc :
*		. create semaphore locks needed
*		. registers driver to FVID2 sub-system
*		. gets called as part of Vps_deviceInit()
*****************************************************************************/
Int32 Vps_nvp2440hInit(void)
{
	Int32               ret = FVID2_SOK;
	Semaphore_Params    semPrms;

	Vps_nvp2440hPinMux();

	/* Memset global object */
	VpsUtils_memset(&gVpsnvp2440hObj, 0, sizeof (gVpsnvp2440hObj));

	/* Create global PH3100K lock */
	Semaphore_Params_init(&semPrms);
	semPrms.mode = Semaphore_Mode_BINARY;
	gVpsnvp2440hObj.lock = Semaphore_create(1u, &semPrms, NULL);
	if (NULL == gVpsnvp2440hObj.lock)
	{
		GT_0trace(VpsDeviceTrace, GT_ERR, "Global semaphore create failed\n");
		ret = FVID2_EALLOC;
	}

	if (FVID2_SOK == ret)
	{
		/* Register NVP2440H driver with FVID2 sub-system */
		ret = FVID2_registerDriver(&gVpsnvp2440hDrvOps);
		if (FVID2_SOK != ret)
		{
			GT_0trace(VpsDeviceTrace, GT_ERR, "Registering to FVID2 driver manager failed\n");
			/* Error - free acquired resources */
			Semaphore_delete(&gVpsnvp2440hObj.lock);
		}
	}

	if (FVID2_SOK == ret) {
		ret = Vps_nvp2440hSerdes_init();
	}

	if(ret == FVID2_SOK) {
		Vps_printf(" [nvp2440h] %s done\n", __func__);
	} else {
		eprintf("%s fail!\n", __func__);
	}

	return (ret);
}

/*****************************************************************************
* @brief	System de-init for NVP2440H driver
* @section	DESC Description
*	- desc :
*		. de-registers driver from FVID2 sub-system
*		. delete's allocated semaphore locks
*		. gets called as part of Vps_deviceDeInit()
*****************************************************************************/
Int32 Vps_nvp2440hDeInit(void)
{
	Vps_nvp2440hSerdes_deinit();

	/* Unregister FVID2 driver */
	FVID2_unRegisterDriver(&gVpsnvp2440hDrvOps);

	/* Delete semaphore's. */
	Semaphore_delete(&gVpsnvp2440hObj.lock);

	dprintf(" %s done!\n", __func__);

	return (FVID2_SOK);
}
