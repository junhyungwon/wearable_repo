/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

#include "vpsdrv_devicePriv.h"
#ifdef PROFILE
#include <ti/sysbios/knl/Clock.h>
#endif

//#ifdef PLATFORM_UBX
#define GPIO_OE				0x134
#define GPIO_DATAIN			0x138
#define GPIO_CLEARDATAOUT	0x190
#define GPIO_SETDATAOUT		0x194

static UInt32 vps_gpio_get_base_address(Int32 bank)
{
	unsigned int address;

	if(bank==0)			address = 0x48032000;
	else if(bank==1)	address = 0x4804C000;
	else if(bank==2)	address = 0x481AC000;
	else if(bank==3)	address = 0x481AE000;
	else				return 0;

	return address;
}

Int32 vps_gpio_read_data(UInt32 gpio)
{
	UInt32 gpio_base, bank, io_no;
	unsigned int value=0;

	if(gpio > 127) {
		return FVID2_EFAIL;
	}

	bank = gpio/32;
	io_no = gpio%32;

	gpio_base = vps_gpio_get_base_address(bank);
	value = REG32(gpio_base+GPIO_DATAIN);

	return ((value>>io_no) & 0x1);
}

Int32 vps_gpio_write_data(UInt32 gpio, UInt32 data)
{
	UInt32 gpio_base, bank, io_no;

	if(gpio > 127) {
		return FVID2_EFAIL;
	}

	bank = gpio/32;
	io_no = gpio%32;

	gpio_base = vps_gpio_get_base_address(bank);

	if(data)	REG32(gpio_base+GPIO_SETDATAOUT) = 1<<io_no;
	else		REG32(gpio_base+GPIO_CLEARDATAOUT) = 1<<io_no;

	return FVID2_SOK;
}

Int32 vps_gpio_direction_input(UInt32 gpio)
{
	UInt32 gpio_base, bank, io_no;

	if(gpio > 127) {
		return FVID2_EFAIL;
	}

	bank = gpio/32;
	io_no = gpio%32;

	gpio_base = vps_gpio_get_base_address(bank);
	REG32(gpio_base+GPIO_OE) |= (1<<io_no);

	return FVID2_SOK;
}

Int32 vps_gpio_direction_output(UInt32 gpio, UInt32 data)
{
	UInt32 gpio_base, bank, io_no;

	if(gpio > 127) {
		return FVID2_EFAIL;
	}

	bank = gpio/32;
	io_no = gpio%32;

	gpio_base = vps_gpio_get_base_address(bank);

	REG32(gpio_base+GPIO_OE) &= ~(1<<io_no);
	if(data)	REG32(gpio_base+GPIO_SETDATAOUT) = 1<<io_no;
	else		REG32(gpio_base+GPIO_CLEARDATAOUT) = 1<<io_no;

	return FVID2_SOK;
}
//#endif

Int32 Vps_deviceInit (Vps_DeviceInitParams * pPrm)
{
	Int32 status = FVID2_SOK;
	Vps_PlatformBoardId boardId;

	memset(&gVps_deviceObj, 0, sizeof(gVps_deviceObj));

	boardId = Vps_platformGetBoardId();
	if (boardId >= VPS_PLATFORM_BOARD_MAX) {
	    Vps_printf (" %s boardId > VPS_PLATFORM_BOARD_MAX \r\n",__FUNCTION__);
	    return FVID2_EFAIL;
	}

	if(TRUE == pPrm->isI2cInitReq) {
		status = Vps_deviceI2cInit(pPrm);
		if(FVID2_SOK != status) {
			return FVID2_EFAIL;
		}
	}

	if (VPS_PLATFORM_BOARD_UBX == boardId)
	{
		//status |= Vps_tvp5158Init();
		/* ph3100k->nvp2440h */
		//status |= Vps_ph3100kInit();
		status |= Vps_nvp2440hInit();
	}
	else
	{
		Vps_printf (" %s: No device!!!\r\n", __FUNCTION__);
	}

	return (status);
}

Int32 Vps_deviceDeInit()
{
	Int32 status = FVID2_SOK;
	Vps_PlatformBoardId boardId;

	boardId = Vps_platformGetBoardId();

	if (VPS_PLATFORM_BOARD_UBX == boardId)
	{
		status |= Vps_nvp2440hDeInit();
		//status |= Vps_ph3100kDeInit();
		//status |= Vps_tvp5158DeInit();
	}

	Vps_deviceI2cDeInit();

	return status;
}
