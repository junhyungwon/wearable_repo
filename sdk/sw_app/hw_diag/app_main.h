/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_main.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_MAIN_H_
#define _APP_MAIN_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include "app_version.h"

#define APP_RELEASE		1

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef union {
	unsigned int w;
	struct {
		unsigned int cap		: 1;
		unsigned int gps		: 1;  /* GPS Position valid / invalid */
		unsigned int gps_jack   : 1;  /* GPS Jack insert/remove */
		unsigned int pwr      	: 1;  /* MAIN Power Valid / invalid */
		unsigned int usb      	: 1;  /* USB Port Valid / invalid */
		unsigned int led      	: 1;  /* LED Port Valid / invalid */
		unsigned int rtc      	: 1;  /* RTC Valid / invalid */
		unsigned int key      	: 1;  /* KEY Valid / invalid */
	} b;
	
} app_state;

typedef struct {
	app_state ste;

	char vbuf[16];      /* voltage */
	char sw_ver[64];
	
	int hw_ver;
	int mcu_ver;
	int snapshot;
	
} __attribute__((packed)) app_cfg_t;

typedef enum {
	RATE_CTRL_VBR,
	RATE_CTRL_CBR,
	MAX_RATE_CTRL
} app_rate_ctrl_e;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern app_cfg_t *iapp;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#endif	/* _APP_MAIN_H_ */
