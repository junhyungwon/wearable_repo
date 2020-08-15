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
#define TIME_ZONE				(9)	//# korea, japan

#define GPIO_N(b, n)			((32*b) + n)
#define GIO_HI					1
#define GIO_LO					0

//# gpio
#define REC_KEY					GPIO_N(0, 6)	//# record switch

#define META_REC_SIZE           120
#define META_MAX_COUNT          60
#define META_REC_TOTAL          (META_REC_SIZE*META_MAX_COUNT)

typedef enum {
	KEY_NONE,
	KEY_SHORT,
	KEY_LONG
} key_type_e;

typedef struct {
    short enabled;
    short incomplete;
} fb_t;

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
void dev_buzz_ctrl(int time, int cnt);

#endif	/* _APP_DEV_H_ */
