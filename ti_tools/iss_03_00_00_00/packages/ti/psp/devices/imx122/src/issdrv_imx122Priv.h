/** ==================================================================
 *  @file   issdrv_imx122Priv.h
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

#ifndef _ISS_IMX122_PRIV_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _ISS_IMX122_PRIV_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <string.h>
#include <xdc/runtime/System.h>

#include <ti/psp/vps/common/trace.h>
#include <ti/psp/devices/imx122/issdrv_imx122.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/psp/iss/drivers/alg/2A/inc/issdrv_algTIaewb.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define eprintf(x...) Vps_printf(" [imx122] err: " x);
#define dprintf(x...) Vps_printf(" [imx122] " x);
//#define dprintf(x...)

/* Driver object state - NOT IN USE */
#define ISS_IMX122_OBJ_STATE_UNUSED   (0)

/* Driver object state - IN USE and IDLE */
#define ISS_IMX122_OBJ_STATE_IDLE     (1)

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
/* Driver handle object */
typedef struct {
    UInt32 state;						/* handle state */
    UInt32 handleId;					/* handle ID, 0..ISS_DEVICE_MAX_HANDLES-1 */
    Semaphore_Handle lock;				/* handle lock */
    Iss_SensorCreateParams createArgs;	/* create time arguments */
} Iss_Imx122Obj;

/* Glabal driver object */
typedef struct {
    FVID2_DrvOps fvidDrvOps;	/* FVID2 driver ops */
    Semaphore_Handle lock;		/* global driver lock */

    /* handle objects */
    Iss_Imx122Obj Imx122Obj[ISS_DEVICE_MAX_HANDLES];
} Iss_Imx122CommonObj;

extern Iss_Imx122CommonObj gIss_Imx122CommonObj;


#define IMX122_WRITE_CMD		0x02
#define IMX122_READ_CMD			0x82

#define IMX122_GAIN_MAX			0x8C

/* Register definition */
#define STANDBY					0x00
#define VREVERSE				0x01
#define MODE					0x02
#define HMAX_L					0x03
#define HMAX_H					0x04
#define VMAX_L					0x05
#define VMAX_H					0x06
#define SHS1_L					0x08		/* Integration time adjustment, designated in line units */
#define SHS1_H					0x09
#define SPL_L					0x0D		/* Integration time adjustment(Low Speed Shutter), designated in frame units */
#define SPL_H					0x0E
#define SVS_L					0x0F		/* Integration time adjustment(Low Speed Shutter), designated in frame units */
#define SVS_H					0x10

#define GAIN					0x1E		/* From 0x00(0dB) ~ 0x8C(13.8dB) */


//# gpio-spi setting
#define CTRL_MODULE_BASE_ADDR	(0x48140000u)

#define GP0_MODULE_BASE_ADDR	(0x48032000u)
#define GP1_MODULE_BASE_ADDR	(0x4804C000u)
#define GP2_MODULE_BASE_ADDR	(0x481AC000u)
#define GP3_MODULE_BASE_ADDR	(0x481AE000u)

#define GPIO_OE					0x134
#define GPIO_DATAIN				0x138
#define GPIO_CLEARDATAOUT		0x190
#define GPIO_SETDATAOUT			0x194

#define REG32		*(volatile unsigned int*)
#define GIO(bank, no)			(bank*32)+no

#define CAM_SPI_CS				GIO(0,14)
#define CAM_SPI_CLK				GIO(0,15)
#define CAM_SPI_DIN				GIO(0,16)
#define CAM_SPI_DOUT			GIO(0,17)

#define CAM_RESET				GIO(0,28)
#define CAM_PWREN				GIO(2,0)

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
Int32 Iss_Imx122_init_sensor(void);

Int32 Iss_Imx122_Reset(Iss_Imx122Obj * pObj);
Int32 Iss_Imx122_RegWrite(Iss_Imx122Obj * pObj, Iss_VideoDecoderRegRdWrParams * pPrm);
Int32 Iss_Imx122_RegRead(Iss_Imx122Obj * pObj, Iss_VideoDecoderRegRdWrParams * pPrm);
Int32 Iss_Imx122_GetChipId(Iss_Imx122Obj * pObj,
			Iss_SensorChipIdParams * pPrm, Iss_SensorChipIdStatus * pStatus);
Int32 Iss_Imx122Start(Iss_Imx122Obj * pObj);
Int32 Iss_Imx122Stop(Iss_Imx122Obj * pObj);

void gpio_direct_output(UInt32 gpio);
void gpio_write_data(UInt32 gpio, UInt32 data);
Int32 gpio_read_data(UInt32 gpio);

int imx122_spi_read(UInt8 regAddr, UInt8 *regValue);
int imx122_spi_write(UInt8 regAddr, UInt8 regValue);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif	/* _ISS_IMX122_PRIV_H_ */
