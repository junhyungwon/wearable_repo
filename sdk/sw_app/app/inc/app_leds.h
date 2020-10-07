/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_leds.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_LEDS_H_
#define _APP_LEDS_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include "board_config.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define DEV_LED_OFF			0
#define DEV_LED_ON			1
#define DEV_LED_BLINK		2

#define LED_RF_OFF			0 /* device not connected */
#define LED_RF_OK			1 /* device connected and connection succeed */
#define LED_RF_FAIL			2 /* device connected but connection fail */

#define LED_REC_OFF			0
#define LED_REC_ON			1
#define LED_REC_FAIL		2

#define LED_GPS_OFF			0  /* gps not connected */
#define LED_GPS_ON			1  /* gps connected, rmc on */
#define LED_GPS_FAIL		2  /* gps connected but rmc off */

#define LED_MMC_ALL_OFF		0
#define LED_MMC_RED_ON		1
#define LED_MMC_RED_BLINK	2
#define LED_MMC_GREEN_ON	3
#define LED_MMC_GREEN_BLINK	4

#define LED_FTP_OFF			0   // send done
#define LED_FTP_ON			1   // file sending
#define LED_FTP_ERROR       2   // send fail

#define LED_DISK_USAGE_ON_1			0	/* turn on only one sd led */
#define LED_DISK_USAGE_ON_2			1	/* turn on two sd leds */
#define LED_DISK_USAGE_ON_3			2	/* turn on three sd leds */

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_leds_init(void);
int app_leds_rf_ctrl(int ste);
int app_leds_gps_ctrl(int ste);
int app_leds_mmc_ctrl(int ste);
int app_leds_mmc_capacity_ctrl(int step);
int app_leds_rec_ctrl(int ste);
int app_leds_int_batt_ctrl(int step);
int app_leds_fw_update_ctrl(void);
int app_leds_backup_state_ctrl(int ste);
int app_leds_sys_normal_ctrl(void);
int app_leds_sys_error_ctrl(void);
int app_leds_eth_status_ctrl(int ste);

#if defined(NEXXONE) || defined(NEXX360W)
int app_leds_cam_ctrl(int ste);
int app_leds_voip_ctrl(int ste);
#else
int app_leds_cam_ctrl(int no, int ste);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _APP_LEDS_H_ */
