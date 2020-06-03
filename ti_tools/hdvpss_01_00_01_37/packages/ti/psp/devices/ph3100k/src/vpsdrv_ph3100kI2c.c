/******************************************************************************
 * FITT360 Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	vpsdrv_ph3100kI2c.c
 * @brief
 * @author	phoong
 * @section	MODIFY history
 *	   - 2012.01.01	: First	Created	based vpsdrv_tvp5158I2c.c
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <ti/psp/devices/ph3100k/src/vpsdrv_ph3100kPriv.h>
#include <ti/psp/platforms/vps_platform.h>
#include <ti/psp/cslr/soc_TI8107.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define PH3100K_I2C_ID		2

#define PH3100K_I2C_ADDR	0x32
#define MAX9272_I2C_ADDR	0x48
#define MAX9271_I2C_ADDR	0x40

#define MAX_DEV_NUM			4

//# sensor dev command (using deviceNum)
#define DEV_PWR_OFF			0x10
#define DEV_R_STATE			0x20

#define WRITE_RETRY_CNT		3
#define PLL_DELAY			200		//# ms

#define SZ_720				0
#define SZ_480				1

struct regval_list {
	unsigned char reg;	//# register address
	unsigned char val;	//# register value
};

typedef struct {
	char pwr;		//# camer module power
	char pdn;		//# diserialize pdn
	char led;		//# sensor module led - if initialization is successful, led on
	char lock;		//# diserialize lock, input
} gio_ctrl_t;

//# control gpio
static gio_ctrl_t gio[MAX_DEV_NUM] = {
	/* CAM_PWR   PDN        LED        LOCK */
	{ GIO(0,18), GIO(3,8),  GIO(0,14), GIO(3,11) },	//# VIN[1]B (1)
	{ GIO(3,14), GIO(3,22), GIO(0,25), GIO(0,23) },	//# VIN[0]A (4)
	{ GIO(0,17), GIO(3,7),  GIO(3,20), GIO(2,28) },	//# VIN[1]A (2)
	{ GIO(0,28), GIO(3,21), GIO(0,24), GIO(3,18) },	//# VIN[0]B (3)
};

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static int d_init = 0;				//# deserialize init flag
static int s_init[MAX_DEV_NUM];		//# sensor init flag
static int s_size[MAX_DEV_NUM];		//# sensor image size
static int eq_lv[MAX_DEV_NUM];		//# serdes equalizer/preemphasis level

//# PH3100K init data --------------------------------------------------------
static const struct regval_list ph3100k_init_reg_fs[] = {	//# front side
	 #if 0
	 //# pll init ---
	{0x03, 0x00}, //# A bank
	{0x25, 0x0A}, //# ppclk 1/2 (140102)
   	{0x41, 0x01}, //# pll_ms 2->1(140102)
   	{0x42, 0x0B}, //# pll_ns		//# 27M*(ns/ms) = 148.5M
   	{0x40, 0x3C},
   	{DELAY, PLL_DELAY},

	{0x40, 0x2C},
	{0x29, 0x08}, //# pad ctrl
   	{DELAY, 30},
   	#endif

   	//{0x03, 0x00}, // A bank
   	{0x06, 0x06}, //# framewidth_h
   	{0x07, 0x71}, //# framewidth_l
   	{0x08, 0x02}, //# fd_fheight_a_h
   	{0x09, 0xED}, //# fd_fheight_a_l
   	{0x0A, 0x02}, //# fd_fheight_b_h
   	{0x0B, 0xED}, //# fd_fheight_b_l
   	{0x0C, 0x00}, //# windowx1_h
   	{0x0D, 0x05}, //# windowx1_l
   	{0x0E, 0x00}, //# windowy1_h
   	{0x0F, 0x05}, //# windowy1_l
   	{0x10, 0x05}, //# windowx2_h
   	{0x11, 0x04}, //# windowx2_l
   	{0x12, 0x02}, //# windowy2_h
   	{0x13, 0xD4}, //# windowy2_l
   	{0x14, 0x00}, //# vsyncstartrow_f0_h
   	{0x15, 0x15}, //# vsyncstartrow_f0_l
   	{0x16, 0x02}, //# vsyncstoprow_f0_h
   	{0x17, 0xE9}, //# vsyncstoprow_f0_l
   	{0xD3, 0x80}, //# ???

/* ############# Start Settings ################ */
   	{0x28, 0xF0}, //# pad_control2 //0xF4
   	{0x33, 0x02}, //# pixelbias
   	{0x34, 0x01}, //# compbias
	{0x36, 0x80}, //# tx_bais, recommended by design 1
	{0x38, 0x58}, //# black_bias '011', rangesel "000", recommended by design 1

   	{0x03, 0x01}, //# B bank
   	{0x1E, 0x0E}, //# bsmode '0'
   	{0x26, 0x03}, //# blacksunth_h

/* ############# BLACK ################ */
   	{0xB1, 0x28}, //# adc offset //0x30
   	{0x03, 0x04}, //# E bank
   	{0x06, 0x98}, //# front_black_fitting[4], ycont/ybright[3], adcoffset_fitting[2]

   	{0x03, 0x01}, //# B bank
   	{0xA4, 0x88}, //# front_black_ref0
	{0xA5, 0x00}, //# front_black_ref1
    {0xA6, 0x00}, //# front_black_ref2
    {0xA7, 0x00}, //# front_black_ref3
    {0xA8, 0x00}, //# front_black_ref4
    {0xA9, 0x08}, //# front_black_ref5
/* ############# AWB ################ */
    /* AWB gain control */
    {0x03, 0x04}, //# E bank
    {0x06, 0xB8}, //# auto_control_3 enable
	/* AWB gain control */
	{0x75, 0x28}, //# awb_rgain_min1 LOW TEMP
	{0x76, 0x28}, //# awb_rgain_min2
	{0x77, 0x78}, //# awb_rgain_max1 HIGH TEMP
	{0x78, 0x78}, //# awb_rgain_max2
	{0x79, 0x38}, //# awb_bgain_min1 HIGH TEMP
	{0x7A, 0x38}, //# awb_bgain_min2
	{0x7B, 0x80}, //# awb_bgain_max1 LOW TEMP
	{0x7C, 0x80}, //# awb_bgain_max2
	{0x7D, 0x01},
	{0x7E, 0x00},
	{0x7F, 0x02},
	{0x80, 0x07},
	{0x73, 0x0C}, //08->0C(140102) //# AWB lock range
	{0x74, 0x08}, //04->08(140102) //# AWB speed
	/* Set AWB Sampling Boundary */
	{0x51, 0x10},
	{0x52, 0xE0},
	{0x53, 0x02},
	{0x54, 0x02},
	{0x55, 0x40},
	{0x56, 0xC0},
	{0x57, 0x04},
	{0x58, 0x6E},
	{0x59, 0x45},
	{0x5A, 0x26}, //# awb_rmin1 LOW TEMP white point
	{0x5B, 0x47}, //# awb_rmax1 HIGH TEMP
	{0x5C, 0x70}, //# awb_bmin1 HIGH TMEP
	{0x5D, 0xAF}, //# awb_bmax1 LOW TEMP
	{0x5E, 0x14},
	{0x5F, 0x1C},
	{0x60, 0x47}, //# awb_rmin2 white point
	{0x61, 0x62}, //# awb_rmax2
	{0x62, 0x3A}, //# awb_bmin2
	{0x63, 0x8E}, //# awb_bmax2
	{0x64, 0x1E},
	{0x65, 0x14},
	/* awb rg/bg ratio axis */
	{0x6E, 0x2F},
	{0x6F, 0x58},
	{0x70, 0x5E},
	/* lens / cs axis */
	{0x03, 0x03},
	{0x16, 0x2F},
	{0x17, 0x58},
	{0x18, 0x5E},
	{0x19, 0x20}, //# user CS gain
/* ############# AE ################ */
	{0x03, 0x04}, //# E bank
	{0x05, 0x64}, //# AE on
	{0x30, 0x08}, //# 63 AE weight left up
	{0x31, 0x08}, //# AE weight right up
	{0x32, 0x08}, //# AE weight left down
	{0x33, 0x08}, //# AE weight right up
	{0x34, 0x08}, //# AE weight center
	{0x3B, 0x73}, //# 70 #max_yt1 y target
	{0x3C, 0x68}, //# 70 #max_yt2
	{0x3D, 0x73}, //# 70 #min_yt1
	{0x3E, 0x68}, //# 70 #min_yt2
	{0x3F, 0x08},
	{0x40, 0x10},
	/* Auto exposure control */
	{0x12, 0x02},
	{0x13, 0xE8},
	{0x14, 0x02},
	{0x15, 0xE8},
	{0x16, 0x02},
	{0x17, 0xE8},
	{0x1B, 0x00},
	{0x1C, 0x17}, //# X8 11 #x6 17 #0B #low light
	{0x1D, 0x40}, //# 70 #40 #C0 #low light
	{0x1E, 0x00},
	{0x1F, 0x17}, //# 11 #17 #0B #low light
	{0x20, 0x40}, //# 70 #40 #C0 #low light
	/* Auto exposure option */
	{0x48, 0x08}, //04->08(140102) //# AE Speed
	{0x49, 0x08}, //04->08(140102)
	{0x4A, 0x0E}, //08->0E(140102)
	/* saturation level th */
	{0x2C, 0xBB}, //# 66 Saturation decision TH
	/* saturation ratio fitting */
	{0x41, 0x04},
	{0x42, 0x08},
	{0x43, 0x10},
	{0x44, 0x20},
	{0x2E, 0x04}, //# 05 Saturation ratio
	/* Flicker canceling mode - manual 60hz */
	{0x03, 0x00}, //# A bank
	{0x4F, 0x08},
	{0x59, 0x00},
	{0x5A, 0xBA}, //# BB
	{0x5B, 0x00}, //# 80
/* ############### Data range ################ */
	{0x03, 0x02}, //# C bank
	{0x09, 0x40}, //# Yrange selection BIT[6]0 b: 0~255 (reference 8bit output), 1b : 16~235 (reference 8bit output)
	{0x9B, 0x02},
/* ############### COLOR ################ */
	/* Color correction */
	{0x33, 0x37},
	{0x34, 0x90},
	{0x35, 0x87},
	{0x36, 0x8E},
	{0x37, 0x3B},
	{0x38, 0x8C},
	{0x39, 0x82},
	{0x3A, 0x98},
	{0x3B, 0x3A},
	/* Color saturation */
	{0x03, 0x03}, //# D bank
	{0x0C, 0x25}, //# Color saturation matrix fitting reference
	{0x0D, 0x88},
	{0x0E, 0x00},
	{0x0F, 0x25},
/* ############### GAMMA ################ */
	/* gamma curve fitting */
	{0x03, 0x02}, //# C bank
	{0x3D, 0x00},
	{0x3E, 0x03},
	{0x3F, 0x0C},
	{0x40, 0x18},
	{0x41, 0x22},
	{0x42, 0x34},
	{0x43, 0x43},
	{0x44, 0x5A},
	{0x45, 0x6D},
	{0x46, 0x8D},
	{0x47, 0xA8},
	{0x48, 0xC0},
	{0x49, 0xD7},
	{0x4A, 0xEC},
	{0x4B, 0xFF},
	/* gamma curve fitting */
	{0x4C, 0x00},
	{0x4D, 0x0F},
	{0x4E, 0x26},
	{0x4F, 0x37},
	{0x50, 0x43},
	{0x51, 0x54},
	{0x52, 0x62},
	{0x53, 0x77},
	{0x54, 0x88},
	{0x55, 0xA4},
	{0x56, 0xBB},
	{0x57, 0xCF},
	{0x58, 0xE0},
	{0x59, 0xF1},
	{0x5A, 0xFF},
	/* gamma curve fitting (0.75) */
	{0x5B, 0x00},
	{0x5C, 0x0B},
	{0x5D, 0x13},
	{0x5E, 0x1A},
	{0x5F, 0x20},
	{0x60, 0x2B},
	{0x61, 0x36},
	{0x62, 0x49},
	{0x63, 0x5A},
	{0x64, 0x7B},
	{0x65, 0x98},
	{0x66, 0xB4},
	{0x67, 0xCE},
	{0x68, 0xE7},
	{0x69, 0xFF},
	/* gamma curve fitting (0.75) */
	{0x6A, 0x00},
	{0x6B, 0x0B},
	{0x6C, 0x13},
	{0x6D, 0x1A},
	{0x6E, 0x20},
	{0x6F, 0x2B},
	{0x70, 0x36},
	{0x71, 0x49},
	{0x72, 0x5A},
	{0x73, 0x7B},
	{0x74, 0x98},
	{0x75, 0xB4},
	{0x76, 0xCE},
	{0x77, 0xE7},
	{0x78, 0xFF},
	/* Y weight control */
	{0x8D, 0x30},
/* ############### EDGE ################ */
	/* sharpness control */
	{0x2F, 0x20}, //# 10 #Edge gain
	{0x30, 0x30}, //# Positive max edge clamp gain
	{0x31, 0x30}, //# Negative max edge clamp gain
/* ############### DARK ################ */
	{0x03, 0x04}, //# E bank
	{0x09, 0x00}, //# 01->00
	/* dark_dpc_p */
	{0x03, 0x03}, //# C bank
	{0x26, 0x00},
	{0x27, 0x00},
	{0x28, 0x00},
	/* dark_blf */
	{0x2E, 0x7F},
	{0x2F, 0x7F},
	{0x30, 0x7F},
	/* dark_nf */
	{0x32, 0x00},
	{0x33, 0x00},
	{0x34, 0x00},
	{0x77, 0x00}, //# dark_dc
	{0x78, 0x04}, //0x00
	{0x79, 0x06}, //0x00
	/* dark_e_blf */
	{0xA1, 0x40},
	{0xA2, 0x7F},
	{0xA3, 0x7F},
	/* Darkness X reference */
	{0x03, 0x04}, //# E bank
	{0x82, 0x03},
	{0x83, 0x05},
	{0x84, 0x07},

	{0x03, 0x03}, //# D bank
	{0x2A, 0x00},
	{0x2B, 0x00},
	{0x2C, 0x00},
	/* lens scale */
	{0x03, 0x02}, //# C bank
	{0x0D, 0x0A},
	{0x0E, 0x0A},
	{0x0F, 0x0A},
	{0x10, 0x0A},
	{0x11, 0x0A},
	{0x12, 0x0A},
	{0x13, 0x0A},
	{0x14, 0x0A},
	{0x15, 0x0A},
	{0x16, 0x0A},
	{0x17, 0x0A},
	{0x18, 0x0A},
	{0x19, 0x0A},
	{0x1A, 0x0A},
	{0x1B, 0x0A},
	{0x1C, 0x0A},
	/* awb rg/bg ratio fitting */
	{0x03, 0x04}, //# E bank
	{0x68, 0x81},
	{0x69, 0x7F},
	{0x6A, 0x81},
	{0x6B, 0x7F},
	{0x6C, 0x82},
	{0x6D, 0x80},
	/* lens gain fitting */
	{0x03, 0x02}, //# C bank
	{0x1E, 0x80},
	{0x1F, 0x80},

	{0x03, 0x03}, //# D bank
	{0x10, 0xC0},
	{0x11, 0x84},
	{0x12, 0xC0},
	{0x13, 0x68},
	{0x14, 0xC8},
	{0x15, 0x68},

	{0x03, 0x02}, //# C bank
	{0x8B, 0x00}, //# ???

	#if 0
	{0x03, 0x00},
	{0x05, 0x03}, //# mirror off
	#endif

	//{DELAY, 30},

	{TERM, 0xff}
};

static const struct regval_list ph3100k_720p[] = {
	{0x03, 0x00},
	{0x0C, 0x00}, //# windowx1_h
	{0x0D, 0x05}, //# windowx1_l (5)
	{0x0E, 0x00}, //# windowy1_h
	{0x0F, 0x05}, //# windowy1_l (5)
	{0x10, 0x05}, //# windowx2_h
	{0x11, 0x04}, //# windowx2_l (1284)
	{0x12, 0x02}, //# windowy2_h
	{0x13, 0xD4}, //# windowy2_l (724)
	{DELAY,   1},

	{0x03, 0x02}, //# C bank
	{0x7B, 0x20}, //# H scale factor
	{0x7C, 0x20}, //# V scale factor
	{0x7D, 0x00}, //# scale buffer thres
	{0x7E, 0x0A}, //0x17

	{0xB3, 0x00}, //# AE window X start_h
	{0xB4, 0x05}, //# AE window X start_l	(5)
	{0xB5, 0x05}, //# AE window X stop_h
	{0xB6, 0x04}, //# AE window X stop_l	(1284)
	{0xB7, 0x00}, //# AE window Y start_h
	{0xB8, 0x05}, //# AE window Y start_l 	(5)
	{0xB9, 0x02}, //# AE window Y stop_h
	{0xBA, 0xD4}, //# AE window Y stop_l	(724)
	{0xBB, 0x01}, //# AE center X start_h
	{0xBC, 0xAF}, //# AE center X start_l 	(431)
	{0xBD, 0x03}, //# AE center X stop_h
	{0xBE, 0x5A}, //# AE center X stop_l	(858)
	{0xBF, 0x00}, //# AE center Y start_h
	{0xC0, 0xF5}, //# AE center Y start_l 	(245)
	{0xC1, 0x01}, //# AE center Y stop_h
	{0xC2, 0xE4}, //# AE center Y stop_l	(484)
	{0xC3, 0x02}, //# AE window X axis_h
	{0xC4, 0x85}, //# AE window X axis_l
	{0xC5, 0x01}, //# AE window Y axis_h
	{0xC6, 0x6D}, //# AE window Y axis_l
	{0xC7, 0x00}, //# AWB window X start_h
	{0xC8, 0x05}, //# AWB window X start_l
	{0xC9, 0x05}, //# AWB window X stop_h
	{0xCA, 0x04}, //# AWB window X stop_l
	{0xCB, 0x00}, //# AWB window Y start_h
	{0xCC, 0x05}, //# AWB window Y start_l
	{0xCD, 0x02}, //# AWB window Y stop_h
	{0xCE, 0xD4}, //# AWB window Y stop_l

	{TERM, 0xff}
};

static const struct regval_list ph3100k_d1[] = {
	{0x03, 0x00},
	{0x0C, 0x00}, //# windowx1_h
	{0x0D, 0x03}, //# windowx1_l (3)
	{0x0E, 0x00}, //# windowy1_h
	{0x0F, 0x03}, //# windowy1_l (3)
	{0x10, 0x03}, //# windowx2_h
	{0x11, 0x56}, //# windowx2_l (854)
	{0x12, 0x01}, //# windowy2_h
	{0x13, 0xE2}, //# windowy2_l (482)
	{DELAY,   1},

	{0x03, 0x02}, //# C bank
	{0x7B, 0x30}, //# H scale factor
	{0x7C, 0x30}, //# V scale factor
	{0x7D, 0x02}, //# scale buffer thres
	{0x7E, 0x56},

	{0xB3, 0x00}, //# AE window X start_h
	{0xB4, 0x03}, //# AE window X start_l	(3)
	{0xB5, 0x03}, //# AE window X stop_h
	{0xB6, 0x56}, //# AE window X stop_l	(854)
	{0xB7, 0x00}, //# AE window Y start_h
	{0xB8, 0x03}, //# AE window Y start_l	(3)
	{0xB9, 0x01}, //# AE window Y stop_h
	{0xBA, 0xE2}, //# AE window Y stop_l	(482)
	{0xBB, 0x00}, //# AE center X start_h
	{0xBC, 0xC5}, //# AE center X start_l	(197)
	{0xBD, 0x01}, //# AE center X stop_h
	{0xBE, 0xBF}, //# AE center X stop_l	(447)
	{0xBF, 0x00}, //# AE center Y start_h
	{0xC0, 0xA3}, //# AE center Y start_l	(163)
	{0xC1, 0x01}, //# AE center Y stop_h
	{0xC2, 0x43}, //# AE center Y stop_l	(323)
	{0xC3, 0x01}, //# AE window X axis_h
	{0xC4, 0x42}, //# AE window X axis_l	(322)
	{0xC5, 0x00}, //# AE window Y axis_h
	{0xC6, 0xF3}, //# AE window Y axis_l	(243)
	{0xC7, 0x00}, //# AWB window X start_h
	{0xC8, 0x03}, //# AWB window X start_l
	{0xC9, 0x03}, //# AWB window X stop_h
	{0xCA, 0x56}, //# AWB window X stop_l
	{0xCB, 0x00}, //# AWB window Y start_h
	{0xCC, 0x03}, //# AWB window Y start_l
	{0xCD, 0x01}, //# AWB window Y stop_h
	{0xCE, 0xE2}, //# AWB window Y stop_l

	{TERM, 0xff}
};

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 I2C read/write wrapper function.
-----------------------------------------------------------------------------*/
static Int32 dev_i2c_read8(UInt8 i2cInstId, UInt8 i2cDevAddr,
								UInt8 regAddr, UInt8 *regVal)
{
	Int32 ret = FVID2_SOK;

	if(NULL == regVal) {
		return FVID2_EFAIL;
	}

	ret = Vps_deviceRead8(i2cInstId, i2cDevAddr, &regAddr, regVal, 1);
	if (FVID2_SOK != ret) {
		//eprintf("%s: dev 0x%x, addr 0x%x\n", __func__, i2cDevAddr, regAddr);
	}

	return (ret);
}

static Int32 dev_i2c_write8(UInt8 i2cInstId, UInt8 i2cDevAddr,
								UInt8 regAddr, UInt8 regVal)
{
	Int32 ret = FVID2_SOK;

	ret = Vps_deviceWrite8(i2cInstId, i2cDevAddr, &regAddr, &regVal, 1);
	if (FVID2_SOK != ret) {
		//eprintf("%s: dev 0x%x, addr 0x%x\n", __func__, i2cDevAddr, regAddr);
	}

	return (ret);
}

/*----------------------------------------------------------------------------
 Control status led
-----------------------------------------------------------------------------*/
static void ctrl_status_led(int idx, int on)
{
	vps_gpio_write_data(gio[idx].led, on);

	//# sensor sucess
	s_init[idx] = on;
}

/*----------------------------------------------------------------------------
 Control Power
-----------------------------------------------------------------------------*/
static void ctrl_main_pwr(int idx, int on)
{
	vps_gpio_write_data(gio[idx].pwr, on);
}

static void ctrl_pwr_reset(int idx)
{
	vps_gpio_write_data(gio[idx].pwr, VPS_OFF);
	Task_sleep(45);
	vps_gpio_write_data(gio[idx].pwr, VPS_ON);
	Task_sleep(20);

	ctrl_status_led(idx, VPS_OFF);
}

static void ctrl_des_pwr(int idx, int on)
{
	//# diserialize pdn - high : disable, low : enable
	vps_gpio_write_data(gio[idx].pdn, on);

	if(on) {
		Task_sleep(10);
	}
}

/*----------------------------------------------------------------------------
 serdes initialize function
-----------------------------------------------------------------------------*/
static Int32 max9272_init(int idx)
{
	Int32 ret=FVID2_SOK;
	unsigned char addr;

	ret |= dev_i2c_write8(PH3100K_I2C_ID, MAX9272_I2C_ADDR, 0x07, 0x80);
	ret |= dev_i2c_write8(PH3100K_I2C_ID, MAX9272_I2C_ADDR, 0x02, 0x5F);	//# 2% spread
	ret |= dev_i2c_write8(PH3100K_I2C_ID, MAX9272_I2C_ADDR, 0x05, 0x37);	//# 8.2dB equalizer default

	if(FVID2_SOK == ret) {
		if(idx > 0) {	//# change i2c address when multi-connected
			addr = (MAX9272_I2C_ADDR + idx) << 1;
			ret |= dev_i2c_write8(PH3100K_I2C_ID, MAX9272_I2C_ADDR, 0x01, addr);
		}
	}
	if(FVID2_SOK != ret) {
		eprintf("%s: #%d\n", __func__, idx);
	}

	Task_sleep(10);

	return ret;
}

static Int32 max9271_init(int idx)
{
	Int32 ret=FVID2_SOK;
	unsigned char eq, pe;

	if(eq_lv[idx] == 1) {       //# 3m~25m
		eq = 0x3A;				//# 11.7dB
		pe = 0xAC;				//#  6.0dB
	} else if(eq_lv[idx] == 2) {//# 0.5m~20m
		eq = 0x39;				//# 10.7dB
		pe = 0xAA;				//#  3.3dB
	} else if(eq_lv[idx] == 3) {//# 3m~30m
		eq = 0x39;				//# 10.7dB
		pe = 0xAD;				//#  8.0dB
	} else if(eq_lv[idx] == 4) {//# 0.15m~15m
		eq = 0x3A;				//# 10.7dB
		pe = 0xA0;				//#    off
	} else {                    //# 0.5m~15m
		eq = 0x37;				//# 8.2dB equalizer
		pe = 0xA9;				//# 2.2dB preemphasis
	}

	//# set max9272 equalizer
	ret |= dev_i2c_write8(PH3100K_I2C_ID, MAX9272_I2C_ADDR+idx, 0x05, eq);
	Task_sleep(10);

	//# init max9271
	ret |= dev_i2c_write8(PH3100K_I2C_ID, MAX9271_I2C_ADDR, 0x07, 0x80);
	ret |= dev_i2c_write8(PH3100K_I2C_ID, MAX9271_I2C_ADDR, 0x02, 0x3F);	//# 0.5% spread
	ret |= dev_i2c_write8(PH3100K_I2C_ID, MAX9271_I2C_ADDR, 0x06, pe);		//# preemphasis

	if(FVID2_SOK != ret) {
		eprintf("%s: #%d\n", __func__, idx);
	}

	Task_sleep(10);

	return ret;
}

/*----------------------------------------------------------------------------
 serdes fwd/rev enable
-----------------------------------------------------------------------------*/
static int max927x_select(int idx)
{
	int i;

	for(i=0; i<MAX_DEV_NUM; i++)
	{
		if(i == idx) {		//# FWDCCEN/REVCCEN enable
			dev_i2c_write8(PH3100K_I2C_ID, MAX9272_I2C_ADDR+i, 0x04, 0x07);
		} else {			//# FWDCCEN/REVCCEN disable
			dev_i2c_write8(PH3100K_I2C_ID, MAX9272_I2C_ADDR+i, 0x04, 0x04);
		}
	}
	Task_sleep(10);

	return FVID2_SOK;
}

/*----------------------------------------------------------------------------
 Check serdes lock.
-----------------------------------------------------------------------------*/
static Int32 max927x_check_lock(int idx)
{
	unsigned int locked=0;

	//# max9272 lock check
	locked = vps_gpio_read_data(gio[idx].lock);

	return locked;
}

/*----------------------------------------------------------------------------
 sensor initialize function
-----------------------------------------------------------------------------*/
static int ph3100k_check_sensor(int idx)
{
	int ret, locked=0;

	max927x_select(idx);
	ret = max9271_init(idx);
	if (FVID2_SOK != ret) {
		return 0;
	}

	//# sensor pad control enable
	ret  = dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x03, 0x00);
	ret |= dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x29, 0x08);

	if(FVID2_SOK == ret)
	{
		Task_sleep(10);		//# wait link established(~3ms)
		locked = max927x_check_lock(idx);
	}

	return locked;
}

static int ph3100k_pll_init(int idx)
{
	int locked=0;

	dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x03, 0x00);	 //# A bank
	/*
	 * REG 0x25->0x0A:ppclk diver = 1/2 [010]
	 *      [4:3]: mclk_div  ->1 (1/2)
	 *      [2:0]: ppclk_div ->2 (1/2)
	 * REG 0x41[5:0]:pll_ms = 5
	 * REG 0x42[5:0]:pll_ns = 0x2c
	 * 0x40[3:2]->frange = 11 (100MHz ~ 300MHz)
	 * fRef->27MHz
	 * fRef->PLL->VCO: VCO = fRef * (pll_ns[5:0] / pll_ms[5:0])
	 *                     = 27MHz * (44 / 5)
	 *                     = 237.6MHz
	 *                 ppclk = VCO / 2 = 118.8
	 */
	dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x25, 0x0A);
	//dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x41, 0x01);
	//dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x42, 0x0B);
	dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x41, 0x05);
	dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x42, 0x2C);
	dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x40, 0x3C);
	Task_sleep(PLL_DELAY);
	dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x40, 0x2C);
	dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x29, 0x08);	//# pad_ctrl
	Task_sleep(30);

	locked = max927x_check_lock(idx);
	if(!locked) {
		eprintf("#%d pll lock fail\n", idx);
	}

	return locked;
}

static int ph3100k_sensor_init(int idx)
{
	Int32 ret, recnt;
	struct regval_list *vals;
	int locked=0;

	//# serializer init
	max927x_select(idx);
	ret = max9271_init(idx);
	if (FVID2_SOK != ret) {
		return FVID2_EFAIL;
	}

	locked = ph3100k_pll_init(idx);
	if(!locked) {
		return FVID2_EFAIL;
	}

	vals = (struct regval_list *)ph3100k_init_reg_fs;
	while (vals->reg != 0xff)
	{
		//# delay
		if (vals->reg == DELAY) {
			Task_sleep(vals->val);
			vals++;
			continue;
		}

		/* Write the common settings for all the modes */
		recnt = WRITE_RETRY_CNT;
		while(recnt--)
		{
			ret = dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR,
					vals->reg, vals->val);
			if(FVID2_SOK == ret)
				break;
			Task_sleep(5);
		}

		if (FVID2_SOK != ret) {
			//eprintf("%s: 0x%x, 0x%x\n", __func__, vals->reg, vals->val);
			break;
		}

		vals++;
	}

	//# mirror 0:VH, 1:V, 2:H, 3:off
	dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x03, 0x00);
#if 1
	if(idx == 0) {
		dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x05, 0x00);
	} else if(idx == 1) {
		dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x05, 0x03);
	} else if(idx == 2) {
		dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x05, 0x01);
	} else if(idx == 3) {
		dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x05, 0x02);
	}
#else
	if(idx == 0) {
		dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x05, 0x00);
	} else if(idx == 1) {
		dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x05, 0x01);
	} else if(idx == 2) {
		dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x05, 0x02);
	} else if(idx == 3) {
		dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, 0x05, 0x03);
	}
#endif
	if(ret == FVID2_SOK) {
		locked = max927x_check_lock(idx);
		if(locked) {
			s_size[idx] = SZ_720;
			ctrl_status_led(idx, VPS_ON);
			return FVID2_SOK;
		}
	}

	ctrl_status_led(idx, VPS_OFF);
	eprintf("%s #%d fail!\n", __func__, idx);

	return FVID2_EFAIL;
}

/*****************************************************************************
* @brief	Deserializer init/deinit function
* @section	DESC Description
*	- desc :
*****************************************************************************/
Int32 Vps_ph3100kSerdes_init(void)
{
	Int32 ret, i;

	if(d_init) {			//# init already
		return FVID2_SOK;
	}

	//# deserializer init
	for(i=(MAX_DEV_NUM-1); i>=0; i--)
	{
		ctrl_des_pwr(i, VPS_ON);
		ret = max9272_init(i);
		if(ret != FVID2_SOK) {
			return ret;
		}
	}

	//# camera module power on
	for(i=0; i<MAX_DEV_NUM; i++) {
		ctrl_main_pwr(i, VPS_ON);
		Task_sleep(10);
	}
	Task_sleep(30);			//# wait after camera module power on

	//# init config
	for(i=0; i<MAX_DEV_NUM; i++) {
		s_init[i] = 0;
		s_size[i] = SZ_720;
	}
	d_init = 1;

	return FVID2_SOK;
}

Int32 Vps_ph3100kSerdes_deinit(void)
{
	int i;

	if(!d_init) {			//# exit already
		return FVID2_SOK;
	}

	//# serdes power down
	for(i=0; i<MAX_DEV_NUM; i++)
	{
		ctrl_status_led(i, VPS_OFF);
		ctrl_des_pwr(i, VPS_OFF);
		ctrl_main_pwr(i, VPS_OFF);
	}

	d_init = 0;

	return FVID2_SOK;
}

/*****************************************************************************
* @brief	Sensor init/deinit function
* @section	DESC Description
*	- desc :
*****************************************************************************/
Int32 Vps_ph3100kInitSensor(Vps_ph3100kHandleObj *pObj)
{
	Int32 ret=FVID2_SOK;
	int idx = pObj->handleId;

	if(s_init[idx]) {		//# init already
		return FVID2_SOK;
	}

	eq_lv[idx] = pObj->serdes_eq;
	ret = ph3100k_sensor_init(idx);

	return ret;
}

Int32 Vps_ph3100kDeinitSensor(Vps_ph3100kHandleObj *pObj)
{
	#if 0	//# not used currently
	int i;

	for(i=0; i<MAX_DEV_NUM; i++) {
		ctrl_status_led(i, VPS_OFF);
	}
	#endif

	return FVID2_SOK;
}

/*****************************************************************************
* @brief	Vps_ph3100kRegWriteIoctl function
* @section	DESC Description
*	- desc : Writes to device registers.
*****************************************************************************/
Int32 Vps_ph3100kRegWriteIoctl(Vps_ph3100kHandleObj *pObj,
							   Vps_VideoDecoderRegRdWrParams *pPrm)
{
	Int32 ret = FVID2_SOK;
	Int32 recnt;

	/* Check for errors */
	if (NULL == pPrm) {
		GT_0trace(VpsDeviceTrace, GT_ERR, "Null pointer/Invalid arguments\n");
		return FVID2_EBADARGS;
	}

	//# deviceNum field is command
	//# regAddr field is device number
	if(pPrm->deviceNum == DEV_PWR_OFF) {		//# power off
		Vps_ph3100kSerdes_deinit();
		return FVID2_SOK;
	}

	max927x_select(pPrm->deviceNum);

	recnt = WRITE_RETRY_CNT;
	while(recnt--)
	{
		ret = dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, pPrm->regAddr, pPrm->regValue);
		if(FVID2_SOK == ret) {
			break;
		}
	}

	return (ret);
}

/*****************************************************************************
* @brief	Vps_ph3100kRegReadIoctl function
* @section	DESC Description
*	- desc : Reads from device registers.
*****************************************************************************/
Int32 Vps_ph3100kRegReadIoctl(Vps_ph3100kHandleObj *pObj,
							  Vps_VideoDecoderRegRdWrParams *pPrm)
{
	Int32	ret = FVID2_SOK;
	int idx;

	/* Check for errors */
	if (NULL == pPrm) {
		GT_0trace(VpsDeviceTrace, GT_ERR, "Null pointer/Invalid arguments\n");
		ret = FVID2_EBADARGS;
	}

	//# deviceNum field is command
	//# regAddr field is device number
	idx = pPrm->regAddr;
	if(pPrm->deviceNum == DEV_R_STATE) {
		pPrm->regValue = s_init[idx];
		return FVID2_SOK;
	}

	max927x_select(pPrm->deviceNum);
	ret = dev_i2c_read8(PH3100K_I2C_ID, PH3100K_I2C_ADDR, pPrm->regAddr, &pPrm->regValue);

	return (ret);
}

/*****************************************************************************
* @brief	Vps_ph3100kGetVideoStatusIoctl function
* @section	DESC Description
*	- desc : Gets the video status of the detected video.
*            Only used video loss detect
*****************************************************************************/
Int32 Vps_ph3100kGetVideoStatusIoctl(Vps_ph3100kHandleObj *pObj,
									 Vps_VideoDecoderVideoStatusParams *pPrm,
									 Vps_VideoDecoderVideoStatus *pStatus)
{
	pStatus->isVideoDetect = s_init[pObj->handleId];

	return FVID2_SOK;
}

/* vsysParams.serdesEQ-> vidDecCreateArgs.serdesEQ-> IOCTL_VPS_VIDEO_DECODER_SENSOR_DETECT */
Int32 Vps_ph3100kSensorDetect(Vps_ph3100kHandleObj *pObj)
{
	Int32 locked, idx;

    if(!d_init) {
		return FVID2_SOK;
	}

    idx = pObj->handleId;
	if(s_init[idx])
	{
		locked = max927x_check_lock(idx);
		if(!locked) {
			ctrl_status_led(idx, VPS_OFF);
		}
	}
	else
	{
		ctrl_pwr_reset(idx);	//# for protect unknown state
		locked = ph3100k_check_sensor(idx);
		if(locked) {
			ctrl_pwr_reset(idx);
			ph3100k_sensor_init(idx);
		}
	}

	return FVID2_SOK;
}

/*****************************************************************************
* @brief	Vps_ph3100kSetVideoModeIoctl function
* @section	DESC Description
*	- desc : Sets the required video standard and output formats depending
*            on requested parameters.
*****************************************************************************/
Int32 Vps_ph3100kSetVideoModeIoctl(Vps_ph3100kHandleObj *pObj,
								   Vps_VideoDecoderVideoModeParams *pPrm)
{
	Int32 ret = FVID2_SOK;
	struct regval_list *vals;
	Int32 recnt;
	UInt16 win_size;

	if(!s_init[pObj->handleId]) {
		return FVID2_SOK;
	}

	win_size=s_size[pObj->handleId];

	switch (pPrm->standard)
    {
        case FVID2_STD_D1:
			if(win_size == SZ_480)
				return ret;

			vals = (struct regval_list *)ph3100k_d1;
			win_size = SZ_480;
            break;

		case FVID2_STD_720P_60:
        default:
    		if(win_size == SZ_720)
        		return ret;

			vals = (struct regval_list *)ph3100k_720p;
			win_size = SZ_720;
            break;
    }

    while (vals->reg != TERM)
	{
		//# delay
		if (vals->reg == DELAY) {
			Task_sleep(vals->val);
			vals++;
			continue;
		}

		recnt = WRITE_RETRY_CNT;
		while(recnt--)
		{
			ret = dev_i2c_write8(PH3100K_I2C_ID, PH3100K_I2C_ADDR,
					vals->reg, vals->val);
			if(FVID2_SOK == ret)
				break;
		}

		if (FVID2_SOK != ret) {
			eprintf("%s: 0x%x, 0x%x\n", __func__, vals->reg, vals->val);
			break;
		}

		vals++;
	}

	if (FVID2_SOK == ret) {
		s_size[pObj->handleId] = win_size;
		Vps_printf(" [ph3100k] set %s done(%d)\n", (win_size==SZ_720)?"720P":"480P", ret);
	}

	return (ret);
}

/*****************************************************************************
* @brief	Vps_ph3100kGetChipIdIoctl function
* @section	DESC Description
*	- desc : Gets PH3100K Chip ID and revision ID.
*****************************************************************************/
Int32 Vps_ph3100kGetChipIdIoctl(Vps_ph3100kHandleObj *pObj,
								Vps_VideoDecoderChipIdParams *pPrm,
								Vps_VideoDecoderChipIdStatus *pStatus)
{
	Int32 ret = FVID2_SOK;
	#if 0	//# skip read for start on low temperature
	UInt16 devId=0;

	ret = dev_i2c_read16(0x00, &devId);
	if(ret != FVID2_SOK)
		ctrl_status_led(pObj->handleId, VPS_OFF);

	pStatus->chipId = devId;
	#else
	pStatus->chipId = 0x3100;
	#endif

	pStatus->chipRevision = 0;
	pStatus->firmwareVersion = 0;

	pPrm->deviceNum = pObj->handleId;

	return ret;
}

/*****************************************************************************
* @brief	Vps_ph3100kResetIoctl function
* @section	DESC Description
*	- desc : Resets the PH3100K.
*****************************************************************************/
Int32 Vps_ph3100kResetIoctl(Vps_ph3100kHandleObj *pObj)
{
	Int32 ret = FVID2_SOK;

	//dprintf("%s done (%d)\n", __func__, ret);

	return (ret);
}

/*****************************************************************************
* @brief	Vps_ph3100kStartIoctl function
* @section	DESC Description
*	- desc : Starts PH3100K.
*****************************************************************************/
Int32 Vps_ph3100kStartIoctl(Vps_ph3100kHandleObj *pObj)
{
	Int32 ret = FVID2_SOK;

	//dprintf("%s done (%d)\n", __func__, ret);

	return (ret);
}

/*****************************************************************************
* @brief	Vps_ph3100kStopIoctl function
* @section	DESC Description
*	- desc : Stops PH3100K.
*****************************************************************************/
Int32 Vps_ph3100kStopIoctl(Vps_ph3100kHandleObj *pObj)
{
	Int32           ret = FVID2_SOK;

	//dprintf("%s done (%d)\n", __func__, ret);

	return (ret);
}

/*****************************************************************************
* @brief	Pin Mux for PH3100K driver
* @section	DESC Description
*	- desc :
*****************************************************************************/
void Vps_ph3100kPinMux(void)
{
	int i;

	//# ctrl gpio - CWX
	//# serdes #0
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0B74) = 0x60080;   //# gp3[14], cam #0 power_en
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0B94) = 0x60080;   //# gp3[22], 9272 pdn
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0AA8) = 0x60080;   //# gp0[25], 9272 led
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0AA0) = 0x60080;   //# gp0[23], 9272 lock
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0AAC) = 0x60080;   //# gp0[26], sensor ready

	//# serdes #1
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0AB4) = 0x60080;   //# gp0[28], cam #1 power_en
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0B90) = 0x60080;   //# gp3[21], 9272 pdn
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0AA4) = 0x60080;   //# gp0[24], 9272 led
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0B84) = 0x60080;   //# gp3[18], 9272 lock
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0B34) = 0x60080;   //# gp2[30], sensor ready

	//# serdes #2
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0A88) = 0x60080;   //# gp0[17], cam #2 power_en
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0B58) = 0x60080;   //# gp3[7], 9272 pdn
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0B8C) = 0x60080;   //# gp3[20], 9272 led
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0B2C) = 0x60080;   //# gp2[28], 9272 lock
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0A9C) = 0x60080;   //# gp0[22], sensor ready

	//# serdes #3
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0A8C) = 0x60080;   //# gp0[18], cam #3 power_en
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0B5C) = 0x60080;   //# gp3[8], 9272 pdn
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0A7C) = 0x60080;   //# gp0[14], 9272 led
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0B68) = 0x60080;   //# gp3[11], 9272 lock
	REG32(CSL_TI8107_CTRL_MODULE_BASE + 0x0A80) = 0x60080;   //# gp0[15], sensor ready

	//# default control
	for(i=0; i<MAX_DEV_NUM; i++) {
		vps_gpio_direction_output(gio[i].pwr, 0);		//# cam pwr : off
		vps_gpio_direction_output(gio[i].pdn, 0);		//# diserialize power : disable, must disable
		vps_gpio_direction_output(gio[i].led, 0);		//# sensor led : off
		vps_gpio_direction_input(gio[i].lock);			//# serdes lock
	}

	#if 0
	vps_gpio_direction_input(GIO(0,26));
	vps_gpio_direction_input(GIO(2,30));
	vps_gpio_direction_input(GIO(0,22));
	vps_gpio_direction_input(GIO(0,15));
	#endif
}
