/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_ctrl.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_CTRL_H_
#define _APP_CTRL_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef enum {
    SERDES_EQ_LV0,		//# B, 0.5m~15m
    SERDES_EQ_LV1,		//# D, 3m~25m
    SERDES_EQ_LV2,		//# C, 0.5m~20m
    SERDES_EQ_LV3,		//# E, 3m~30m
    SERDES_EQ_LV4,		//# A, 0.15m~15m
    MAX_SERDES_EQ_LV
} serdes_eq_e;

typedef enum {
	LED_IDX_RF_G = 0,
	LED_IDX_RF_R,
	LED_IDX_GPS_G,
	LED_IDX_GPS_R,
	LED_IDX_BAT_G1,
	LED_IDX_BAT_G2,
	LED_IDX_BAT_G3,
	LED_IDX_BAT_G4,
	LED_IDX_CAM1,
	LED_IDX_CAM2,
	LED_IDX_CAM3,
	LED_IDX_CAM4,
	LED_IDX_SD_G,
	LED_IDX_SD_R,
	LED_IDX_SD_G1,
	LED_IDX_SD_G2,
	LED_IDX_SD_G3,
	LED_IDX_REC,
	LED_IDX_BACKUP,
	LED_IDX_ALL
} leds_index_e;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int ctrl_swms_set(int win_num, int ch);

int ctrl_chk_network(void);
int ctrl_get_hw_version(char *version);
int ctrl_get_mcu_version(char *version);

int ctrl_leds(int index, int state);

#endif	/* _APP_CTRL_H_ */
