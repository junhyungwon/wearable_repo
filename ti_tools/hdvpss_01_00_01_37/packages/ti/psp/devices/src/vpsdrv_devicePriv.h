/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

#ifndef _VPSDRV_DEVICE_PRIV_H_
#define _VPSDRV_DEVICE_PRIV_H_

#include <string.h>
#include <xdc/runtime/System.h>
#include <ti/psp/devices/vps_device.h>
#include <ti/psp/platforms/vps_platform.h>
#include <ti/psp/i2c/psp_i2c.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/psp/devices/tvp5158/vpsdrv_tvp5158.h>
#include <ti/psp/devices/ph3100k/vpsdrv_ph3100k.h>
/* added by jhhan */
#include <ti/psp/devices/nvp2440h/vpsdrv_nvp2440h.h>

#define VPS_DEVICE_I2C_TIMEOUT      10	//# mS, (BIOS_WAIT_FOREVER)

typedef struct
{
    UInt32 i2cDebugEnable;

    PSP_I2cHandle       i2cHndl[VPS_DEVICE_I2C_INST_ID_MAX];
    Semaphore_Handle    i2cHndlLock[VPS_DEVICE_I2C_INST_ID_MAX];

} Vps_DeviceObj;

extern Vps_DeviceObj gVps_deviceObj;

Int32 Vps_deviceI2cLock(UInt32 instId, Bool lock);

#endif  /*  _VPSDRV_DEVICE_PRIV_H_  */
