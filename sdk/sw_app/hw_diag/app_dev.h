/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_dev.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_DEV_H_
#define _APP_DEV_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_ZONE			9		//# korea

#define GPIO_N(b, n)		((32*b) + n)

#define GPS_PWR_EN 			GPIO_N(2, 4)	//# gps power enable
#define REC_KEY				GPIO_N(0, 6)	//# record switch
#define BACKUP_DET			GPIO_N(1, 13)	//# cradle detect

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_dev_init(void);
void app_dev_exit(void);
int app_dev_start(void);
void app_dev_stop(void);
int app_gps_ctrl(int en);

int app_ste_mmc(void);
int app_ste_ether(void);
int app_get_gsn(int *x, int *y, int *z);
int app_wait_mmc_remove(void);

int app_buzzer(int ms, int cnt);

#endif	/* _APP_DEV_H_ */
