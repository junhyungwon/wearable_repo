/** ==================================================================
 *  @file   issdrv_imx122.h
 *
 *  @path   /ti/psp/devices/imx122/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

#ifndef _ISSDRV_IMX122_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _ISSDRV_IMX122_H_

#include <ti/psp/devices/iss_sensorDriver.h>
#include <ti/psp/vps/drivers/fvid2_drvMgr.h>


Int32 Iss_Imx122Init(void);
Int32 Iss_Imx122DeInit(void);

Int32 Iss_Imx122PinMux(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
