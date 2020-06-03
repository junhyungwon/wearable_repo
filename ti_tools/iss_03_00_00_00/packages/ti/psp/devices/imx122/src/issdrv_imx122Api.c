/** ==================================================================
 *  @file   issdrv_imx122Api.c
 *
 *  @path   /ti/psp/devices/imx122/src/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <ti/psp/devices/imx122/src/issdrv_imx122Priv.h>
#include <ti/psp/platforms/iss_platform.h>
#include <ti/psp/iss/hal/iss/isp/isif/inc/isif_reg.h>
#include <ti/psp/iss/hal/iss/isp/isif/inc/isif.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
/* Global object storing all information related to all IMX122 driver handles */
static Iss_Imx122CommonObj gIss_Imx122CommonObj;

static UInt8 old_gain = 0;
static UInt16 old_exposure = 0;

extern isif_regs_ovly isif_reg;
extern ti2a_output ti2a_output_params;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static Int32 Iss_Imx122LockObj(Iss_Imx122Obj * pObj);
static Int32 Iss_Imx122UnlockObj(Iss_Imx122Obj * pObj);
static Int32 Iss_Imx122Lock(void);
static Int32 Iss_Imx122Unlock(void);

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static Int32 Transplant_DRV_imgsSetEshutter(void)
{
	Int32 status = FVID2_SOK;
	UInt8 regValue[2];

	if(old_exposure == ti2a_output_params.sensorExposure)
		return status;

	regValue[0] = (UInt8)(ti2a_output_params.sensorExposure & 0xFF);
	regValue[1] = (UInt8)((ti2a_output_params.sensorExposure >> 8) & 0xFF);
	old_exposure = ti2a_output_params.sensorExposure;

	status = Iss_Imx122Lock();

	if (FVID2_SOK == status)
	{
		imx122_spi_write(SHS1_L, regValue[0]);
		imx122_spi_write(SHS1_H, regValue[1]);
	}

	Iss_Imx122Unlock();

	return status;
}

/* Update exposure and gain value from the 2A */
static Int32 Iss_Imx122UpdateExpGain(Iss_Imx122Obj * pObj, Ptr createArgs)
{
    Int32 status = FVID2_SOK;
    UInt8 gain;

    if(old_gain == ti2a_output_params.sensorGain)
		return status;

	gain = ti2a_output_params.sensorGain;
	old_gain = ti2a_output_params.sensorGain;
	if(gain > IMX122_GAIN_MAX)
		gain = IMX122_GAIN_MAX;

    status = Iss_Imx122Lock();

    if (FVID2_SOK == status) {
        imx122_spi_write(GAIN, gain);
    }

    Iss_Imx122Unlock();

    return status;
}

/* Control API that gets called when FVID2_control is called
 *
 * This API does handle level semaphore locking
 *
 * handle - MN34041 driver handle cmd - command cmdArgs - command arguments
 * cmdStatusArgs - command status
 *
 * returns error in case of - illegal parameters - I2C command RX/TX error */
/* ===================================================================
 *  @func     Iss_Imx122Control
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
Int32 Iss_Imx122Control(Fdrv_Handle handle,
                         UInt32 cmd, Ptr cmdArgs, Ptr cmdStatusArgs)
{
    Iss_Imx122Obj *pObj = (Iss_Imx122Obj *) handle;
    Int32 status;

    if (pObj == NULL)
        return FVID2_EBADARGS;

    /*
     * lock handle
     */
    Iss_Imx122LockObj(pObj);

    switch (cmd)
    {
        case FVID2_START:
            // status = Iss_Imx122Start ( pObj );
            break;

        case FVID2_STOP:
            // status = Iss_Imx122Stop ( pObj );
            break;

        case IOCTL_ISS_SENSOR_GET_CHIP_ID:
            // status = Iss_Imx122GetChipId ( pObj, cmdArgs, cmdStatusArgs
            // );
            break;

        case IOCTL_ISS_SENSOR_RESET:
			status = Iss_Imx122_Reset(pObj);
            break;

        case IOCTL_ISS_SENSOR_REG_WRITE:
            status = Iss_Imx122_RegWrite (pObj, cmdArgs);
            break;

        case IOCTL_ISS_SENSOR_REG_READ:
            status = Iss_Imx122_RegRead (pObj, cmdArgs);
            break;

		case IOCTL_ISS_SENSOR_UPDATE_EXP_GAIN:
			Transplant_DRV_imgsSetEshutter();
            status = Iss_Imx122UpdateExpGain(pObj, cmdArgs);
            break;

        default:
            status = FVID2_EUNSUPPORTED_CMD;
            break;
    }

    /*
     * unlock handle
     */
    Iss_Imx122UnlockObj(pObj);

    return status;
}

/*
 * Allocate driver object
 *
 * Searches in list of driver handles and allocate's a 'NOT IN USE' handle
 * Also create's handle level semaphore lock
 *
 * return NULL in case handle could not be allocated */
/* ===================================================================
 *  @func     Iss_Imx122AllocObj
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
static Iss_Imx122Obj *Iss_Imx122AllocObj(void)
{
    UInt32 handleId;

    Iss_Imx122Obj *pObj;

    Semaphore_Params semParams;

    UInt32 found = FALSE;

    /*
     * Take global lock to avoid race condition
     */
    Iss_Imx122Lock();

    /*
     * find a unallocated object in pool
     */
    for (handleId = 0; handleId < ISS_DEVICE_MAX_HANDLES; handleId++)
    {

        pObj = &gIss_Imx122CommonObj.Imx122Obj[handleId];

        if (pObj->state == ISS_IMX122_OBJ_STATE_UNUSED)
        {
            /*
             * free object found
             */

            /*
             * init to 0's
             */
            memset(pObj, 0, sizeof(*pObj));

            /*
             * init state and handle ID
             */
            pObj->state = ISS_IMX122_OBJ_STATE_IDLE;
            pObj->handleId = handleId;

            /*
             * create driver object specific semaphore lock
             */
            Semaphore_Params_init(&semParams);

            semParams.mode = Semaphore_Mode_BINARY;

            pObj->lock = Semaphore_create(1u, &semParams, NULL);

            found = TRUE;

            if (pObj->lock == NULL)
            {
                /*
                 * Error - release object
                 */
                found = FALSE;
                pObj->state = ISS_IMX122_OBJ_STATE_UNUSED;
            }

            break;
        }
    }

    /*
     * Release global lock
     */
    Iss_Imx122Unlock();

    if (found)
        return pObj;                                       /* Free object
                                                            * found return it
                                                            */

    /*
     * free object not found, return NULL
     */
    return NULL;
}

/*
 * De-Allocate driver object
 *
 * Marks handle as 'NOT IN USE' Also delete's handle level semaphore lock */
/* ===================================================================
 *  @func     Iss_Imx122FreeObj
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
static Int32 Iss_Imx122FreeObj(Iss_Imx122Obj * pObj)
{
    /*
     * take global lock
     */
    Iss_Imx122Lock();

    if (pObj->state != ISS_IMX122_OBJ_STATE_UNUSED)
    {
        /*
         * mark state as unused
         */
        pObj->state = ISS_IMX122_OBJ_STATE_UNUSED;

        /*
         * delete object locking semaphore
         */
        Semaphore_delete(&pObj->lock);
    }

    /*
     * release global lock
     */
    Iss_Imx122Unlock();

    return FVID2_SOK;
}

/*
 * Handle level lock */
/* ===================================================================
 *  @func     Iss_Imx122LockObj
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
static Int32 Iss_Imx122LockObj(Iss_Imx122Obj * pObj)
{
    Semaphore_pend(pObj->lock, BIOS_WAIT_FOREVER);

    return FVID2_SOK;
}

/*
 * Handle level unlock */
/* ===================================================================
 *  @func     Iss_Imx122UnlockObj
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
static Int32 Iss_Imx122UnlockObj(Iss_Imx122Obj * pObj)
{
    Semaphore_post(pObj->lock);

    return FVID2_SOK;
}

/*
 * Global driver level lock */
/* ===================================================================
 *  @func     Iss_Imx122Lock
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
static Int32 Iss_Imx122Lock(void)
{
    Semaphore_pend(gIss_Imx122CommonObj.lock, BIOS_WAIT_FOREVER);

    return FVID2_SOK;
}

/*
 * Global driver level unlock */
/* ===================================================================
 *  @func     Iss_Imx122Unlock
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
static Int32 Iss_Imx122Unlock(void)
{
    Semaphore_post(gIss_Imx122CommonObj.lock);

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     pintISIFRegDirectly
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
void pintISIFRegDirectly()
{
    static int tmpIndex = 0x064;

    int i = 0;

    if (tmpIndex > 0)
    {
        tmpIndex--;
    }
    else
    {

        uint32 *pData = NULL;

        // pData=(uint32 *)(&(isp_regs->SYNCEN));
        pData = (uint32 *) (isif_reg);
        if (NULL != pData)
        {
            for (i = 0; i < 127; i++)
            {
                dprintf("Debug : global structure  isp_regs [%d] = 0x%08x ", i, *pData);
                pData++;
            }

        }
        else
        {
            eprintf("%s:%s:%d !!!\n", __FILE__, __FUNCTION__, __LINE__);
        }
        int i = 0;

        volatile uint32 *pdata;

        // volatile
        for (i = 0; i < 127; i++)
        {
            pdata = (uint32 *)(ISIF_BASE_ADDRESS + i * 4);
            dprintf("Debug : Directly print ISIFReg [%d] = 0x%08x ", i, *pdata);
        }

        tmpIndex = 0x64;

    }

}

/*
 * Create API that gets called when FVID2_create is called
 *
 * This API does not configure the IMX122 is any way.
 *
 * This API - validates parameters - allocates driver handle - stores create
 * arguments in its internal data structure.
 *
 * Later the create arguments will be used when doing I2C communcation with
 * IMX122
 *
 * drvId - driver ID, must be FVID2_ISS_VID_DEC_IMX122_DRV instanceId - must
 * be 0 createArgs - create arguments createStatusArgs - create status
 * fdmCbParams - NOT USED, set to NULL
 *
 * returns NULL in case of any error */
/* ===================================================================
 *  @func     Iss_Imx122Create
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
Fdrv_Handle Iss_Imx122Create(UInt32 drvId,
                              UInt32 instanceId,
                              Ptr createArgs,
                              Ptr createStatusArgs,
                              const FVID2_DrvCbParams * fdmCbParams)
{
    Iss_Imx122Obj *pObj;
    Iss_SensorCreateParams *sensorCreateArgs = (Iss_SensorCreateParams *) createArgs;
    Iss_SensorCreateStatus *sensorCreateStatus = (Iss_SensorCreateStatus *) createStatusArgs;

    /* check parameters */
    if (sensorCreateStatus == NULL)
        return NULL;

    sensorCreateStatus->retVal = FVID2_SOK;

    if (drvId != FVID2_ISS_SENSOR_IMX122_DRV
        || instanceId != 0 || sensorCreateArgs == NULL)
    {
        sensorCreateStatus->retVal = FVID2_EBADARGS;
        return NULL;
    }

    if (sensorCreateArgs->deviceI2cInstId >= ISS_DEVICE_I2C_INST_ID_MAX)
    {
        sensorCreateStatus->retVal = FVID2_EINVALID_PARAMS;
        return NULL;
    }

    /* allocate driver handle */
    pObj = Iss_Imx122AllocObj();
    if (pObj == NULL)
    {
        sensorCreateStatus->retVal = FVID2_EALLOC;
        return NULL;
    }

    /* copy parameters to allocate driver handle */
    memcpy(&pObj->createArgs, sensorCreateArgs, sizeof(*sensorCreateArgs));

	/* return driver handle object pointer */
    return pObj;
}

/*
 * Delete function that is called when FVID2_delete is called
 *
 * This API - free's driver handle object
 *
 * handle - driver handle deleteArgs - NOT USED, set to NULL
 *
 */
/* ===================================================================
 *  @func     Iss_Imx122Delete
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
Int32 Iss_Imx122Delete(Fdrv_Handle handle, Ptr deleteArgs)
{
    Iss_Imx122Obj *pObj = (Iss_Imx122Obj *) handle;

    if (pObj == NULL)
        return FVID2_EBADARGS;

    /*
     * free driver handle object
     */
    Iss_Imx122FreeObj(pObj);

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     Iss_Imx122PinMux
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
Int32 Iss_Imx122PinMux(void)
{
	*(volatile UInt32*)(CTRL_MODULE_BASE_ADDR + 0x0A7C) = 0x60080;		// SPI_CS  : GP0[14]
	*(volatile UInt32*)(CTRL_MODULE_BASE_ADDR + 0x0A80) = 0x60080;		// SPI_CLK : GP0[15]
	*(volatile UInt32*)(CTRL_MODULE_BASE_ADDR + 0x0A88) = 0x60080;		// SPI_DIN : GP0[17]
	*(volatile UInt32*)(CTRL_MODULE_BASE_ADDR + 0x0A84) = 0x60080;		// SPI_DOUT: GP0[16]

	*(volatile UInt32*)(CTRL_MODULE_BASE_ADDR + 0x0AB4) = 0x60080;		// CAM_RESET : GP0[28]
	*(volatile UInt32*)(CTRL_MODULE_BASE_ADDR + 0x0A18) = 0x60080;		// CAM_POWER : GP2[0]

	gpio_direct_output(CAM_SPI_CS);
	gpio_direct_output(CAM_SPI_CLK);
	gpio_direct_output(CAM_SPI_DOUT);

	gpio_direct_output(CAM_RESET);
	gpio_direct_output(CAM_PWREN);

    return 0;
}

/* ===================================================================
 *  @func     Iss_Imx122Init
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
Int32 Iss_Imx122Init(void)
{
    Semaphore_Params semParams;
    Int32 status = FVID2_SOK;

    /* Set to 0's for global object, descriptor memory */
    memset(&gIss_Imx122CommonObj, 0, sizeof(gIss_Imx122CommonObj));

    /* Create global IMX122 lock */
    Semaphore_Params_init(&semParams);

    semParams.mode = Semaphore_Mode_BINARY;

    gIss_Imx122CommonObj.lock = Semaphore_create(1u, &semParams, NULL);

    if (gIss_Imx122CommonObj.lock == NULL)
        status = FVID2_EALLOC;

    if (status == FVID2_SOK)
    {
        gIss_Imx122CommonObj.fvidDrvOps.create = (FVID2_DrvCreate) Iss_Imx122Create;
        gIss_Imx122CommonObj.fvidDrvOps.delete = Iss_Imx122Delete;
        gIss_Imx122CommonObj.fvidDrvOps.control = Iss_Imx122Control;
        gIss_Imx122CommonObj.fvidDrvOps.queue = NULL;
        gIss_Imx122CommonObj.fvidDrvOps.dequeue = NULL;
        gIss_Imx122CommonObj.fvidDrvOps.processFrames = NULL;
        gIss_Imx122CommonObj.fvidDrvOps.getProcessedFrames = NULL;
        gIss_Imx122CommonObj.fvidDrvOps.drvId = FVID2_ISS_SENSOR_IMX122_DRV;

        status = FVID2_registerDriver(&gIss_Imx122CommonObj.fvidDrvOps);

        if (status != FVID2_SOK)
        {
            /* Error - free acquired resources */
            Semaphore_delete(&gIss_Imx122CommonObj.lock);
        }

		gpio_write_data(CAM_PWREN, 1);	//# cam power enable
        Iss_Imx122_init_sensor();

        //# check sensor
	    {
			UInt8 reg_val=0;
	    	imx122_spi_read(0x20, &reg_val);	// Black level
			if(reg_val != 0x3c){
				eprintf("%s: reg_val 0x%x\n", __func__, reg_val);
				status = FVID2_EFAIL;
			}
		}
    }

    if (status == FVID2_SOK) {
    	dprintf("%s done!\n", __func__);
	} else {
        eprintf("%s\n", __func__);
    }

    return status;
}

/*
 * System de-init for IMX122 driver
 *
 * This API - de-registers driver from FVID2 sub-system - delete's allocated
 * semaphore locks - gets called as part of Iss_deviceDeInit() */
/* ===================================================================
 *  @func     Iss_Imx122DeInit
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
Int32 Iss_Imx122DeInit()
{
    /*
     * Unregister FVID2 driver
     */
    FVID2_unRegisterDriver(&gIss_Imx122CommonObj.fvidDrvOps);

    /*
     * Delete semaphore's
     */
    Semaphore_delete(&gIss_Imx122CommonObj.lock);

	gpio_write_data(CAM_PWREN, 0);	//# cam power disable

    return 0;
}
