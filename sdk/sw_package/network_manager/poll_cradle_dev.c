/*
 * File : poll_cradle_dev.c
 *
 * Copyright (C) 2020 LF
 *
 */
/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "netmgr_ipc_cmd_defs.h"
#include "poll_cradle_dev.h"
#include "common.h"
#include "event_hub.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_CRADLE_POLL_CYCLE			200		//# msec

typedef struct {
	app_thr_obj pObj; /* device detect */
	
} netmgr_poll_cradle_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_poll_cradle_t t_proc;
static netmgr_poll_cradle_t *iethX = &t_proc;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static int __is_connected_cradle(void)
{
	int status;
	
	/* 0-> connect 1-> remove */
	gpio_get_value(BACKUP_DET, &status);

	return (status?0:1);
}

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_cradle_poll(void *prm)
{
	app_thr_obj *tObj = &iethX->pObj;
	int exit = 0, cmd;
	int ret;
	
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = tObj->cmd;
        if (cmd == APP_CMD_EXIT) {
		    break;
		}
		
		//# wait cradle Device
		ret = __is_connected_cradle();
		if (ret) {
			/* set attach event */
			if (app_cfg->ste.bit.cradle == 0) {
				app_cfg->ste.bit.cradle = 1;
				netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_CRADLE, 1);
			}
		} else {
			/* set remove event */
			if (app_cfg->ste.bit.cradle == 1) {
				app_cfg->ste.bit.cradle = 0;
				netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_CRADLE, 0);
			}
		}
		
		// for next event : wait
		delay_msecs(TIME_CRADLE_POLL_CYCLE);
	} 
	
	tObj->active = 0;
	
	return NULL;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) start!
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_poll_cradle_init(void)
{
	app_thr_obj *tObj;
	
	gpio_input_init(BACKUP_DET);
	
	tObj = &iethX->pObj;
	if (thread_create(tObj, THR_cradle_poll, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr usb device poll thread\n");
		return EFAIL;
    }
	
	aprintf("done!...\n");
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_poll_cradle_exit(void)
{
	app_thr_obj *tObj;

	/* delete usb scan object */
    tObj = &iethX->pObj;
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
	gpio_exit(BACKUP_DET);
	
    aprintf("... done!\n");
	
	return 0;
}
