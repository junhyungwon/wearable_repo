/******************************************************************************
 * FITT360 Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	vpsdrv_nvp2440hI2c.c
 * @brief
 * @author	phoong
 * @section	MODIFY history
 *	   - 2012.01.01	: First	Created	based vpsdrv_tvp5158I2c.c
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <ti/psp/devices/nvp2440h/src/vpsdrv_nvp2440hPriv.h>
#include <ti/psp/platforms/vps_platform.h>
#include <ti/psp/cslr/soc_TI8107.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define NVP2440H_I2C_ID		2

//# for partron 0x66, afo 0x7F
#define NVP2440H_I2C_ADDR	0x66 //0x7F //# 0xxFE

/* NVP2440H Command List */
#define NVP2440H_CMD_ISP_REG_WRITE			0x02
#define NVP2440H_CMD_ISP_REG_READ			0x01

/* NVP2440H Rx Response List */
#define NVP2440H_RES_CODE_PROCESSING		(0xF0)
#define NVP2440H_RES_CODE_SUCCESS			(0xF1)
#define NVP2440H_RES_CODE_PACKET_LEN_FAIL	(0xF2)

#define MAX9272_I2C_ADDR	0x48
#define MAX9271_I2C_ADDR	0x40

#define MAX_DEV_NUM			4

//# sensor dev command (using deviceNum)
#define DEV_PWR_OFF			0x10
#define DEV_R_STATE			0x20

#define WRITE_RETRY_CNT		10//5
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

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 I2C read/write wrapper function.
-----------------------------------------------------------------------------*/
/*
 * write command / read command -> ISP or Sensor Register control
 * control command -> OSD or ISP register value to save flash memory
 * 1.Tx packet->Interrupt Packet->Response packet (Ext MCU <--> Int MCU)
 */
static Int32 dev_i2c_read8(UInt8 i2cInstId, UInt8 i2cDevAddr,
								UInt8 regAddr, UInt8 *regVal)
{
	Int32 ret = FVID2_SOK;

	if(NULL == regVal) {
		return FVID2_EFAIL;
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

static Int32 dev_nicp_write_irq(UInt8 i2cInstId, UInt8 i2cDevAddr)
{
	Int32 ret = FVID2_SOK;
	UInt8 irq_buf[3] = {0x10, 0x80, 0xff};

	ret = Vps_deviceRawWrite8(i2cInstId, i2cDevAddr, &irq_buf[0], 3);
	if (FVID2_SOK != ret) {
		//eprintf("%s: dev 0x%x\n", __func__, i2cDevAddr);
	}

	return (ret);
}

static Int32 dev_nicp_write_reg(UInt8 i2cInstId, UInt8 i2cDevAddr, UInt16 regAddr, UInt8 regVal)
{
	Int32 ret = FVID2_SOK;
	Int32 checksum = 0, cnt;
	/*
	 * buf[0-1]:mcu buffer address, buf[2]: packet length, buf[3]: command type
	 * buf[4]: number of register, buf[5-6]: register address
	 * buf[7]: register value, buf[8]: checksum
	 */
	UInt8 tx_buf[9] = {
		/* mcu buffer address */ /* packet length */
		0x10, 0x00, 0x07, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00
	};

	tx_buf[5] = ((regAddr >> 8) & 0xff);
	tx_buf[6] = (regAddr & 0xff);
	tx_buf[7] = (regVal & 0xff);

	for (cnt = 2; cnt < 8; cnt++)
		checksum += tx_buf[cnt];

	/* 6. checksum */
	tx_buf[8] = (checksum&0xff);

	ret = Vps_deviceRawWrite8(i2cInstId, i2cDevAddr, &tx_buf[0], 9);
	if (FVID2_SOK != ret) {
		return FVID2_EFAIL;
	}

	/* send interrupt packet */
	ret = dev_nicp_write_irq(i2cInstId, i2cDevAddr);
	if (FVID2_SOK != ret) {
		return FVID2_EFAIL;
	}

	Task_sleep(200);

//	dev_nicp_wait_response(i2cInstId, i2cDevAddr);

//	Vps_printf("%s: dev 0x%x(0x%x)write done!\n", __func__, i2cDevAddr, regAddr);

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
	Task_sleep(300);//#required more 300ms
	vps_gpio_write_data(gio[idx].pwr, VPS_ON);
	Task_sleep(20);

	ctrl_status_led(idx, VPS_OFF);

	/* wait cam boot-up time */
	Task_sleep(1000);
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

	ret |= dev_i2c_write8(NVP2440H_I2C_ID, MAX9272_I2C_ADDR, 0x15, 0x0E);  //# hidden register (pulse amp 150mV)
	ret |= dev_i2c_write8(NVP2440H_I2C_ID, MAX9272_I2C_ADDR, 0x07, 0x80);
	ret |= dev_i2c_write8(NVP2440H_I2C_ID, MAX9272_I2C_ADDR, 0x02, 0x5F);	//# 2% spread
	ret |= dev_i2c_write8(NVP2440H_I2C_ID, MAX9272_I2C_ADDR, 0x05, 0x37);	//# 8.2dB equalizer default

	if(FVID2_SOK == ret) {
		if(idx > 0) {	//# change i2c address when multi-connected
			addr = (MAX9272_I2C_ADDR + idx) << 1;
			ret |= dev_i2c_write8(NVP2440H_I2C_ID, MAX9272_I2C_ADDR, 0x01, addr);
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

	/* pe -> 0xAX (coax cable), 0x8X (shield twisted)*/
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
	ret |= dev_i2c_write8(NVP2440H_I2C_ID, MAX9272_I2C_ADDR+idx, 0x05, eq);
	Task_sleep(10);

	//# init max9271
	ret |= dev_i2c_write8(NVP2440H_I2C_ID, MAX9271_I2C_ADDR, 0x07, 0x80);
	ret |= dev_i2c_write8(NVP2440H_I2C_ID, MAX9271_I2C_ADDR, 0x02, 0x3F); //# 0.5% spread
	ret |= dev_i2c_write8(NVP2440H_I2C_ID, MAX9271_I2C_ADDR, 0x06, pe);	  //# preemphasis
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
			dev_i2c_write8(NVP2440H_I2C_ID, MAX9272_I2C_ADDR+i, 0x04, 0x07);
		} else {			//# FWDCCEN/REVCCEN disable
			dev_i2c_write8(NVP2440H_I2C_ID, MAX9272_I2C_ADDR+i, 0x04, 0x04);
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
static int nvp2440h_check_sensor(int idx)
{
	int ret, locked=0;

	max927x_select(idx);
	ret = max9271_init(idx);
	if (FVID2_SOK != ret) {
		return 0;
	}

	Task_sleep(10);		//# wait link established(~3ms)
	locked = max927x_check_lock(idx);

	return locked;
}

static int nvp2440h_sensor_init(int idx)
{
	Int32 ret, recnt;
	Uint8 val;
	int locked=0;

	//# serializer init
	max927x_select(idx);
	ret = max9271_init(idx);
	if (FVID2_SOK != ret) {
		return FVID2_EFAIL;
	}

	/* deserializer lock check */
	recnt = 10;
	while (recnt--) {
		locked = max927x_check_lock(idx);
		if (locked) {
			break;
		}
		/* wait for isp firmware download */
		Task_sleep(100);
	}

	if (recnt <= 0)
		return FVID2_EFAIL;

	//# mirror 0:VH, 1:V, 2:H, 3:off (for 3100k)
	//# mirror 0:off, 1:H, 2:V, 3:180 rotate (for nvp2440)
	val = 0x03; //0x01
	if (val != 0x00) 
	{
		recnt = WRITE_RETRY_CNT;
		while (recnt--) {
			ret = dev_nicp_write_reg(NVP2440H_I2C_ID, NVP2440H_I2C_ADDR, 0x82e1, val);
			if (FVID2_SOK == ret) {
				break;
			} else {
				eprintf("cam %d: i2c write failed!(cnt %d)\n", idx, recnt);
				Task_sleep(50);
			}
		}
		
		recnt = WRITE_RETRY_CNT;
		while (recnt--) {
			ret = dev_nicp_write_reg(NVP2440H_I2C_ID, NVP2440H_I2C_ADDR, 0x0183, 0xc2);
			if (FVID2_SOK == ret) {
				break;
			} else {
				eprintf("cam %d: i2c write failed!(cnt %d)\n", idx, recnt);
				Task_sleep(50);
			}
		}
	} 

	/* deserializer lock check */
	locked = max927x_check_lock(idx);
	if(locked) {
		s_size[idx] = SZ_720;
		ctrl_status_led(idx, VPS_ON);
		return FVID2_SOK;
	}

	ctrl_status_led(idx, VPS_OFF);

	return FVID2_EFAIL;
}

/*****************************************************************************
* @brief	Deserializer init/deinit function
* @section	DESC Description
*	- desc :
*****************************************************************************/
Int32 Vps_nvp2440hSerdes_init(void)
{
	Int32 ret, i;

	if (d_init) {			//# init already
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
		Task_sleep(50);
	}
	Task_sleep(30);	//# wait after camera module power on

	//# init config
	for(i=0; i<MAX_DEV_NUM; i++) {
		s_init[i] = 0;
		s_size[i] = SZ_720;
	}
	d_init = 1;

	return FVID2_SOK;
}

Int32 Vps_nvp2440hSerdes_deinit(void)
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
Int32 Vps_nvp2440hInitSensor(Vps_nvp2440hHandleObj *pObj)
{
	Int32 ret=FVID2_SOK;
	int idx = pObj->handleId;

	if(s_init[idx]) {		//# init already
		return FVID2_SOK;
	}

	/* 1 ~ 5 */
	eq_lv[idx] = pObj->serdes_eq;

	ret = nvp2440h_sensor_init(idx);

	return ret;
}

Int32 Vps_nvp2440hDeinitSensor(Vps_nvp2440hHandleObj *pObj)
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
* @brief	Vps_nvp2440hRegWriteIoctl function
* @section	DESC Description
*	- desc : Writes to device registers.
*****************************************************************************/
Int32 Vps_nvp2440hRegWriteIoctl(Vps_nvp2440hHandleObj *pObj,
							   Vps_VideoDecoderRegRdWrParams *pPrm)
{
	Int32 ret = FVID2_SOK;
	Int32 recnt;

	/* Check for errors */
	if (NULL == pPrm) {
		GT_0trace(VpsDeviceTrace, GT_ERR, "Null pointer/Invalid arguments\n");
		return FVID2_EBADARGS;
	}

	if(!s_init[pPrm->deviceNum]) {
		return FVID2_SOK;
	}

	//# deviceNum field is command
	//# regAddr field is device number
	if (pPrm->deviceNum == DEV_PWR_OFF) {		//# power off
		Vps_nvp2440hSerdes_deinit();
		return FVID2_SOK;
	}

	max927x_select(pPrm->deviceNum);
	recnt = WRITE_RETRY_CNT;
	while (recnt--) {
		ret = dev_nicp_write_reg(NVP2440H_I2C_ID, NVP2440H_I2C_ADDR, 0x82e1, pPrm->regValue);
		if (FVID2_SOK == ret) {
			break;
		} else {
			eprintf("cam %d: i2c write failed!(cnt %d)\n", pPrm->deviceNum, recnt);
			Task_sleep(30);
		}
	}

	return (ret);
}

/*****************************************************************************
* @brief	Vps_nvp2440hRegReadIoctl function
* @section	DESC Description
*	- desc : Reads from device registers.
*****************************************************************************/
Int32 Vps_nvp2440hRegReadIoctl(Vps_nvp2440hHandleObj *pObj,
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

	return (ret);
}

/*****************************************************************************
* @brief	Vps_nvp2440hGetVideoStatusIoctl function
* @section	DESC Description
*	- desc : Gets the video status of the detected video.
*            Only used video loss detect
*****************************************************************************/
Int32 Vps_nvp2440hGetVideoStatusIoctl(Vps_nvp2440hHandleObj *pObj,
									 Vps_VideoDecoderVideoStatusParams *pPrm,
									 Vps_VideoDecoderVideoStatus *pStatus)
{
	pStatus->isVideoDetect = s_init[pObj->handleId];

	return FVID2_SOK;
}

/* vsysParams.serdesEQ-> vidDecCreateArgs.serdesEQ-> IOCTL_VPS_VIDEO_DECODER_SENSOR_DETECT */
Int32 Vps_nvp2440hSensorDetect(Vps_nvp2440hHandleObj *pObj)
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
		//ctrl_pwr_reset(idx);	//# for protect unknown state
		locked = nvp2440h_check_sensor(idx);
		if(locked) {
			ctrl_pwr_reset(idx);
			nvp2440h_sensor_init(idx);
		}
	}

	return FVID2_SOK;
}

/*****************************************************************************
* @brief	Vps_nvp2440hSetVideoModeIoctl function
* @section	DESC Description
*	- desc : Sets the required video standard and output formats depending
*            on requested parameters.
*****************************************************************************/
Int32 Vps_nvp2440hSetVideoModeIoctl(Vps_nvp2440hHandleObj *pObj,
								   Vps_VideoDecoderVideoModeParams *pPrm)
{
	Int32 ret = FVID2_SOK;
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

			win_size = SZ_480;
            break;

		case FVID2_STD_720P_60:
        default:
    		if(win_size == SZ_720)
        		return ret;

			win_size = SZ_720;
            break;
    }

	if (FVID2_SOK == ret) {
		s_size[pObj->handleId] = win_size;
	}

	return (ret);
}

/*****************************************************************************
* @brief	Vps_nvp2440hGetChipIdIoctl function
* @section	DESC Description
*	- desc : Gets NVP2440H Chip ID and revision ID.
*****************************************************************************/
Int32 Vps_nvp2440hGetChipIdIoctl(Vps_nvp2440hHandleObj *pObj,
								Vps_VideoDecoderChipIdParams *pPrm,
								Vps_VideoDecoderChipIdStatus *pStatus)
{
	Int32 ret = FVID2_SOK;

	pStatus->chipId = 0x2440;
	pStatus->chipRevision = 0;
	pStatus->firmwareVersion = 0;

	pPrm->deviceNum = pObj->handleId;

	return ret;
}

/*****************************************************************************
* @brief	Vps_nvp2440hResetIoctl function
* @section	DESC Description
*	- desc : Resets the PH3100K.
*****************************************************************************/
Int32 Vps_nvp2440hResetIoctl(Vps_nvp2440hHandleObj *pObj)
{
	Int32 ret = FVID2_SOK;

	return (ret);
}

/*****************************************************************************
* @brief	Vps_ph3100kStartIoctl function
* @section	DESC Description
*	- desc : Starts PH3100K.
*****************************************************************************/
Int32 Vps_nvp2440hStartIoctl(Vps_nvp2440hHandleObj *pObj)
{
	Int32 ret = FVID2_SOK;

	return (ret);
}

/*****************************************************************************
* @brief	Vps_nvp2440hStopIoctl function
* @section	DESC Description
*	- desc : Stops PH3100K.
*****************************************************************************/
Int32 Vps_nvp2440hStopIoctl(Vps_nvp2440hHandleObj *pObj)
{
	Int32           ret = FVID2_SOK;

	return (ret);
}

/*****************************************************************************
* @brief	Pin Mux for NVP2440H driver
* @section	DESC Description
*	- desc :
*****************************************************************************/
void Vps_nvp2440hPinMux(void)
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
