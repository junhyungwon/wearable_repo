/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_key.c
 * @brief	ir remocon input thread
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <linux/input.h>

#include "app_comm.h"
#include "app_util.h"
#include "app_main.h"
#include "app_dev.h"
#include "gui_main.h"
#include "app_key.h"
#include "dev_gpio.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define USE_MENU		0	//# use consol menu

typedef struct {
	app_thr_obj iObj;		//# ir thread

} app_key_t;

typedef enum {
	KEY_NONE = 0,
	KEY_SHORT,
	KEY_LONG
} key_type_e;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_key_t key_obj;
static app_key_t *ikey=&key_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    rec key switch check function
* @section  [desc]
*****************************************************************************/
static int dev_ste_key(int gio)
{
	int status;

	gpio_get_value(gio, &status);
	//dprintf("--- [key] value %d\n", status);

	return status;
}

static int cnt_key=0, ste_key=KEY_NONE;
static int chk_rec_key(void)
{
	int key = dev_ste_key(REC_KEY);

	if(ste_key != KEY_NONE) {
		if(key == 1) {
			ste_key = KEY_NONE;
		}
		return KEY_NONE;
	}

	if(cnt_key)
	{
		if(key == 0) {	//# press
			cnt_key--;
			if(cnt_key == 0) {
				ste_key = KEY_LONG;
			}
		}
		else {
			cnt_key = 0;
			ste_key = KEY_SHORT;
		}
	}
	else {
		if(ste_key == KEY_NONE) {
			if(key == 0) {
				cnt_key = 20;		//# 1sec
			}
		}
	}

	return ste_key;
}

/*****************************************************************************
* @brief    key thread function
* @section  [desc]
*****************************************************************************/
static void *thread_key(void *prm)
{
	app_thr_obj *tObj = &ikey->iObj;
	int exit=0, ret;
	char cmd;

	gpio_input_init(REC_KEY);

	tObj->active = 1;

	while(!exit)
	{
		#if USE_MENU
		cmd = menu_get_cmd();
		switch(cmd)
		{
			case '1':		//# pwr
				gui_ctrl(APP_KEY_PWR, 0, 0);
				break;
			case '4':		//# left
				gui_ctrl(APP_KEY_LEFT, 0, 0);
				break;
			case '6':		//# right
				gui_ctrl(APP_KEY_RIGHT, 0, 0);
				break;
			case '5':		//# ok
				gui_ctrl(APP_KEY_OK, 0, 0);
				break;
		}
		#else
		//# wait key
		cmd = tObj->cmd;
		if (cmd == APP_CMD_EXIT) {
			break;
		}

		ret = chk_rec_key();
		if (ret == KEY_SHORT) {
			//gui_ctrl(APP_KEY_RIGHT, 0, 0);
			gui_ctrl(APP_KEY_OK, 0, 0);
		}

		OSA_waitMsecs(100);
		#endif
	}

	gpio_exit(REC_KEY);

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    key thread start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_key_start(void)
{
	app_thr_obj *tObj;

	//# static config clear
	memset((void *)ikey, 0x0, sizeof(app_key_t));

	//#--- create ir thread
	tObj = &ikey->iObj;
	if(thread_create(tObj, thread_key, UI_THREAD_PRI, NULL, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}

	return 0;
}

int app_key_stop(void)
{
#if !defined(USE_MENU)
	app_thr_obj *tObj;

	//#--- stop thread
	tObj = &ikey->iObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		OSA_waitMsecs(10);
	}
	thread_delete(tObj);
#endif
	return 0;
}
