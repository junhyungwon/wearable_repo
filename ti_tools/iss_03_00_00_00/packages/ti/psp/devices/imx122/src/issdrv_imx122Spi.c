/** ==================================================================
 *  @file   issdrv_imx122I2c.c
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

#include <ti/psp/devices/imx122/src/issdrv_imx122Priv.h>

struct regval_list {
	unsigned char addr;
	unsigned char val;
};

static struct regval_list imx122_1080p_init_data[] =
{
	{0x00, 0x31},		/* Standby*/
	//{0x11, 0x88},
	{0x11, 0x80},
	{0x2d, 0x40},
	{0x02, 0x0F},
	{0x16, 0x3c},
	{0x17, 0x00},
	{0x14, 0x00},
	{0x15, 0x00},
	{0x1A, 0x51},
	{0x1B, 0x04},
	{0x18, 0xc0},
	{0x19, 0x07},
	{0x01, 0x00},		/* V-reverse */
	//{0x01, 0x01},		/* V-reverse */
	{0x12, 0x82},
	//{0x11, 0x88},
	{0x11, 0x80},
	{0x0f, 0x00},
	{0x10, 0x00},
	{0x0d, 0x00},
	{0x0e, 0x00},
	{0x08, 0x00},
	{0x09, 0x00},
	{0x1e, 0x00},
	{0x20, 0x3c},		/* black off */
	{0x22, 0x00},
	{0x05, 0x65},
	{0x06, 0x04},

	{0x03, 0x4c},
	{0x04, 0x04},
	//{0x03, 0x98},
	//{0x04, 0x08},

	//{0x2c, 0x01},		/*  Slave */
	{0x2c, 0x00},		/*  Slave */
	{0x9a, 0x26},
	{0x9b, 0x02},
	{0x00, 0x00},		/* Standby cancel */

	{0xff, 0xff}
};

static struct regval_list imx122_720p_init_data[] =
{
	{0x00, 0x31},		/* Standby*/
	//{0x11, 0x88},
	{0x11, 0x80},
	{0x2d, 0x40},
	{0x02, 0x01},//0xF
	{0x16, 0xF0},//0x3c WINPV
	{0x17, 0x00},//0x00
	{0x14, 0x40},//0x00 WINPH
	{0x15, 0x01},//0x00
	{0x1A, 0xe9},//0x51 WINWV
	{0x1B, 0x02},//0x04
	{0x18, 0x40},//0xc0 WINWH
	{0x19, 0x05},//0x07
	{0x01, 0x00},		/* V-reverse */
	//{0x01, 0x01},		/* V-reverse */
	{0x12, 0x82},//12bit resolution
	//{0x11, 0x88},
	{0x11, 0x81},//0x80
	{0x0f, 0x00},
	{0x10, 0x00},
	{0x0d, 0x00},
	{0x0e, 0x00},
	{0x08, 0x00},
	{0x09, 0x00},
	{0x1e, 0x00},
	{0x20, 0x3c},		/* black off */
	{0x22, 0x80},//0x00
	{0x05, 0xee},//0x65
	{0x06, 0x02},//0x04

	{0x03, 0x72},//0x4c
	{0x04, 0x06},//0x04
	//{0x03, 0x98},
	//{0x04, 0x08},

	//{0x2c, 0x01},		/*  Slave */
	{0x2c, 0x00},		/*  Slave */
	{0x9a, 0x4c},//0x26
	{0x9b, 0x04},//0x02
	{0x00, 0x00},		/* Standby cancel */

	{0xff, 0xff}
};

#define IMX122_SET_CS		gpio_write_data(CAM_SPI_CS, 1)
#define IMX122_CLR_CS		gpio_write_data(CAM_SPI_CS, 0)
#define IMX122_SET_CLK		gpio_write_data(CAM_SPI_CLK, 1)
#define IMX122_CLR_CLK		gpio_write_data(CAM_SPI_CLK, 0)
#define IMX122_SET_DOUT		gpio_write_data(CAM_SPI_DOUT, 1)
#define IMX122_CLR_DOUT		gpio_write_data(CAM_SPI_DOUT, 0)

/*----------------------------------------------------------------------------
 GPIO read/write function
-----------------------------------------------------------------------------*/
static UInt32 gpio_get_base_address(UInt32 gpio, UInt32 *io_no)
{
	unsigned int address;

	if(gpio > (96-1))		address = GP3_MODULE_BASE_ADDR;
	else if(gpio > (64-1))	address = GP2_MODULE_BASE_ADDR;
	else if(gpio > (32-1))	address = GP1_MODULE_BASE_ADDR;
	else					address = GP0_MODULE_BASE_ADDR;

	*io_no = (gpio % 32);

	return address;
}

void gpio_direct_output(UInt32 gpio)
{
	unsigned int addr, io_no;

	addr = gpio_get_base_address(gpio, &io_no);

	REG32(addr+GPIO_OE) &= ~(1<<io_no);
}

void gpio_write_data(UInt32 gpio, UInt32 data)
{
	unsigned int addr, io_no;

	addr = gpio_get_base_address(gpio, &io_no);

	if(data)	REG32(addr+GPIO_SETDATAOUT) = (1<<io_no);
	else		REG32(addr+GPIO_CLEARDATAOUT) = (1<<io_no);
}

Int32 gpio_read_data(UInt32 gpio)
{
	unsigned int addr, io_no, value;

	addr = gpio_get_base_address(gpio, &io_no);
	value = REG32(addr+GPIO_DATAIN);

	return ((value>>io_no) & 0x1);
}

/*----------------------------------------------------------------------------
 SPI read/write function
-----------------------------------------------------------------------------*/
int imx122_spi_read(UInt8 regAddr, UInt8 *regValue)
{
	int status = FVID2_SOK;
	UInt8 data;
	int j;

	if(!regValue) {
		eprintf(" imx122 SPI parameter error.\n");
		return FVID2_EBADARGS;
	}

	/* Clock signal to "Low"*/
	IMX122_CLR_CLK;

	{
		/* CS to Low*/
		IMX122_CLR_CS;

		/* LSB First, Write Read Command Byte */
		data = IMX122_READ_CMD;
		for(j = 0; j < 8; j++)
		{
			if(data & 0x01)		IMX122_SET_DOUT;
			else				IMX122_CLR_DOUT;
			IMX122_SET_CLK;
			IMX122_CLR_CLK;
			data = data >> 1;
		}

		/* Write Register address to Read */
		data = regAddr;
		for(j = 0; j < 8; j++)
		{
			if(data & 0x01)		IMX122_SET_DOUT;
			else				IMX122_CLR_DOUT;
			IMX122_SET_CLK;
			IMX122_CLR_CLK;
			data = data >> 1;
		}

		data = 0;
		for(j = 0; j < 8; j++)
		{
			IMX122_SET_CLK;
			if(gpio_read_data(CAM_SPI_DIN))	data |= (1 << j);
			IMX122_CLR_CLK;
		}

		*regValue = data;

		/* CS to High */
		IMX122_SET_CS;
	}

	/* Clock Signal to "High" */
	IMX122_SET_CLK;

	return status;
}

int imx122_spi_write(UInt8 regAddr, UInt8 regValue)
{
	int status = FVID2_SOK;
	Uint16 data;
	int j;

	/* Clock signal to "Low"*/
	IMX122_CLR_CLK;

	{
		/* CS to Low*/
		IMX122_CLR_CS;

		data = IMX122_WRITE_CMD;
		for(j = 0; j < 8; j++)
		{
			if(data & 0x01)		IMX122_SET_DOUT;
			else				IMX122_CLR_DOUT;
			IMX122_SET_CLK;
			IMX122_CLR_CLK;
			data = data >> 1;
		}

		//delay = 1000;
		//while(delay--);
		Task_sleep(1);

		data = regAddr;
		for(j = 0; j < 8; j++)
		{
			if(data & 0x01)		IMX122_SET_DOUT;
			else				IMX122_CLR_DOUT;
			IMX122_SET_CLK;
			IMX122_CLR_CLK;
			data = data >> 1;
		}

		//delay = 1000;
		//while(delay--);
		Task_sleep(1);

		data = regValue;
		for(j = 0; j < 8; j++)
		{
			if(data & 0x01)		IMX122_SET_DOUT;
			else				IMX122_CLR_DOUT;
			IMX122_SET_CLK;
			IMX122_CLR_CLK;
			data = data >> 1;
		}

		//delay = 2000;
		//while(delay--);
		Task_sleep(1);

		/* CS to High */
		IMX122_SET_CS;
	}

	/* Clock Signal to "High" */
	IMX122_SET_CLK;

	return status;
}

static void imx122_reset(void)
{
	gpio_write_data(CAM_RESET, 0);
	Task_sleep(5);
	gpio_write_data(CAM_RESET, 1);
	Task_sleep(1);
}

Int32 Iss_Imx122_init_sensor(void)
{
	Int32 status = FVID2_SOK;
	struct regval_list *vals = imx122_1080p_init_data;

	/* Hardware Reset */
	imx122_reset();

    while(vals->addr != 0xff)
    {
        imx122_spi_write(vals->addr, vals->val);
		vals++;
		Task_sleep(1);
	}	//# 128mS

	return status;
}

/*
 * write to device registers */
/* ===================================================================
 *  @func     Iss_Imx122RegWrite
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
Int32 Iss_Imx122_RegWrite(Iss_Imx122Obj * pObj, Iss_VideoDecoderRegRdWrParams * pPrm)
{
	Int32 status = FVID2_SOK;

	if (pPrm == NULL)
		return FVID2_EBADARGS;

	imx122_spi_write(pPrm->reg_addr, pPrm->reg_val);

	return status;
}

/*
 * read from device registers */
/* ===================================================================
 *  @func     Iss_Imx122RegRead
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
Int32 Iss_Imx122_RegRead(Iss_Imx122Obj *pObj, Iss_VideoDecoderRegRdWrParams *pPrm)
{
	Int32 status = FVID2_SOK;

	if (pPrm == NULL)
		return FVID2_EBADARGS;

	if (pPrm->regAddr8 == NULL || pPrm->regValue8 == NULL || pPrm->numRegs == 0)
		return FVID2_EBADARGS;

	imx122_spi_read(pPrm->reg_addr, &pPrm->reg_val);

	return status;
}

Int32 Iss_Imx122_Reset(Iss_Imx122Obj *pObj)
{
	Int32 status = FVID2_SOK;

	imx122_reset();

	return status;
}
