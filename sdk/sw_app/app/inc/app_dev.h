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

#include "app_main.h"
#include "dev_gps.h"
#include "dev_accel.h"

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_ZONE			(9)	//# korea, japan

#define GPIO_N(b, n)		((32*b) + n)
#define GIO_HI			1
#define GIO_LO			0

//# gpio
#define REC_KEY					GPIO_N(0, 6)	//# record switch
#define BACKUP_DET				GPIO_N(1, 13)	//# cradle detect
#define USB_DET 				GPIO_N(1, 14)	//# usb detect
#define GPS_PWR_EN 				GPIO_N(2, 4)	//# gps power enable

/* Wi-Fi Module Path */
#define RTL_8188E_PATH          "/lib/modules/8188eu.ko"
#define RTL_8188C_PATH          "/lib/modules/8192cu.ko"
#define RTL_8821A_PATH          "/lib/modules/8821au.ko"

/* Wi-Fi Module name */
#define RTL_8188E_NAME          "8188eu"
#define RTL_8188C_NAME          "8192cu"
#define RTL_8821A_NAME          "8821au"

#define GSENSOR_CNT_MAX         10

#define META_REC_SIZE           120
#define META_MAX_COUNT          60
#define META_REC_TOTAL          (META_REC_SIZE*META_MAX_COUNT)

typedef enum {
	KEY_NONE,
	KEY_SHORT,
	KEY_LONG
} key_type_e;

typedef struct{
    float gx;
    float gy;
    float gz;
} app_gvalue_t;

typedef struct {
    short enabled;
    short incomplete;
} fb_t;

typedef struct {
	int enable;		//# 0: invalid, 1:valid

	int utc_year;
	int utc_month;
	int utc_mday;
	int utc_hour;
	int utc_min;
	int utc_sec;
	int utc_msec;

	double subsec;
	double speed;
	double lat; 				//# latitude : (+)N, (-)S
	double lot; 				//# longitude: (+)E, (-) W
	double dir; 				//# forward direction (degree)
} gps_rmc_t;

typedef struct{
    int acc_sec;
    int acc_msec;
    accel_data_t acc_level;
} app_gs_t;

typedef struct {
    gps_rmc_t gps;
} app_meta_t;

typedef struct {
    datetime_t  time;
    gps_rmc_t gps;
    app_gvalue_t acc_level;
} app_extmeta_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_dev_init(void);
void app_dev_exit(void);

int dev_ste_mmc(void);
int dev_ste_key(int gio);
int dev_ste_cradle(void);
int dev_ste_gps_port(void);
void dev_buzz_ctrl(int time, int cnt);
void dev_get_gps_rmc(gps_rmc_t *gps);
int dev_ste_ethernet(int devnum) ;


#endif	/* _APP_DEV_H_ */
