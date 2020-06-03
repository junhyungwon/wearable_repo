/** ==================================================================
 *  @file   issdrv_device.c
 *
 *  @path   /ti/psp/devices/src/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

#include <ti/psp/devices/src/vpsdrv_devicePriv.h>
#include "issdrv_devicePriv.h"

#ifdef PLATFORM_UBX
#define REG32	*(volatile unsigned int*)
static UInt32 iss_gpio_get_base_address(Int32 bank)
{
	unsigned int address;

	if(bank==0)			address = 0x48032000;
	else if(bank==1)	address = 0x4804C000;
	else if(bank==2)	address = 0x481AC000;
	else if(bank==3)	address = 0x481AE000;
	else				return 0;

	return address;
}

Int32 iss_gpio_write_data(UInt32 band, UInt32 io_no, UInt32 data)
{
	UInt32 gpio_base;

	gpio_base = iss_gpio_get_base_address(band);
	if(!gpio_base)
		return FVID2_EFAIL;

	if(data)	REG32(gpio_base+0x194) = 1<<io_no;
	else		REG32(gpio_base+0x190) = 1<<io_no;

	return FVID2_SOK;
}
#endif

/* ===================================================================
 *  @func     Iss_deviceInit
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
Int32 Iss_deviceInit(Vps_DeviceInitParams * pPrm)
{
    Int32 status = FVID2_SOK;

    Iss_PlatformId platformId = Iss_platformGetId();

    Iss_PlatformBoardId boardId;

#ifdef POWER_OPT_DSS_OFF
    memset(&gVps_deviceObj, 0, sizeof(gVps_deviceObj));
#endif
    platformId = ISS_PLATFORM_ID_EVM_TI814x;
    if ((platformId == ISS_PLATFORM_ID_UNKNOWN) ||
        (platformId >= ISS_PLATFORM_ID_MAX))
    {
        status = FVID2_EFAIL;
    }
#ifdef POWER_OPT_DSS_OFF

    if ((FVID2_SOK == status) && (TRUE == pPrm->isI2cInitReq))
    {
        status = Vps_deviceI2cInit(pPrm);
#else
    if (FVID2_SOK == status)
    {
#endif
        if (FVID2_SOK == status)
        {
            boardId = Iss_platformGetBoardId();
            if (boardId >= ISS_PLATFORM_BOARD_MAX)
            {
                status |= FVID2_EFAIL;
            }
            else if (ISS_PLATFORM_BOARD_UBX == boardId)
            {
				#ifdef IMGS_SONY_IMX122
				Iss_Imx122PinMux();
                status |= Iss_Imx122Init();
				#endif
            }
            else
            {
                status = FVID2_EFAIL;
            }
        }
    }

    return (status);
}

/* ===================================================================
 *  @func     Iss_deviceDeInit
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
Int32 Iss_deviceDeInit()
{
    Int32 status = FVID2_SOK;

    Iss_PlatformId platformId = Iss_platformGetId();

    Iss_PlatformBoardId boardId = Iss_platformGetBoardId();

    if ((platformId == ISS_PLATFORM_ID_UNKNOWN) ||
        (platformId >= ISS_PLATFORM_ID_MAX) ||
        (boardId >= ISS_PLATFORM_BOARD_MAX))
    {
        status = FVID2_EFAIL;
    }
    else if (ISS_PLATFORM_BOARD_UBX == boardId)
	{
		#ifdef IMGS_SONY_IMX122
		status |= Iss_Imx122DeInit();
    	#endif
    }
    else
    {
        status |= FVID2_EFAIL;
    }
#ifdef POWER_OPT_DSS_OFF
    Vps_deviceI2cDeInit();
#endif

    return status;
}
