/*
 * File : poll_wlan.c
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
#include <sys/poll.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define __USE_GNU
#include <linux/types.h>
#include <errno.h>

#include "netmgr_ipc_cmd_defs.h"
#include "poll_wlan.h"
#include "common.h"
#include "event_hub.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_WLAN0_POLL_CYCLE		200		//# msec
#define WLAN_OPER_PATH	  			"/sys/class/net/wlan0/operstate"

/* Wi-Fi Module name */
#define RTL_8821A_NAME				"8811au"
#define RTL_88X2B_NAME				"88x2bu"

typedef struct {
	app_thr_obj pObj; /* device detect */
	
} netmgr_poll_wlan_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_poll_wlan_t t_proc;
static netmgr_poll_wlan_t *iwlan = &t_proc;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
#if 0
static int __get_usbid(int *v, int *p)
{
    char buf[256] = {0,};
	FILE *f=NULL;
	
    if ((f = fopen("/proc/modules", "r")) == NULL) {
        eprintf("Could not open /proc/modules!!\n");
        return 0;
    }

    while ((fgets(buf, sizeof(buf), f)) != NULL) 
	{
        if (strncmp(buf, RTL_8821A_NAME, strlen(RTL_8821A_NAME)) == 0) {
			*v = RTL_8821A_VID; *p = RTL_8821A_PID;
            fclose(f);
            return 0;
        } 
		
		if (strncmp(buf, RTL_88X2B_NAME, strlen(RTL_88X2B_NAME)) == 0) {
			*v = RTL_88X2B_VID; *p = RTL_88X2B_PID;
            fclose(f);
            return 0;
		}
		
    }
    fclose(proc);

    return -1;
}
#endif

static int __is_connected_wlan(void)
{
	int ret=0;
	
	ret = access(WLAN_OPER_PATH, R_OK);
	if ((ret == 0) || (errno == EACCES)) {
		/* WLAN0 Device */
		return 1;
	} 
	
	return ret;
}

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_wlan_poll(void *prm)
{
	app_thr_obj *tObj = &iwlan->pObj;
	int exit = 0, cmd, ret;
	
	tObj->active = 1;
	dprintf("enter...!\n");
	
	while (!exit)
	{
		cmd = tObj->cmd;
        if (cmd == APP_CMD_EXIT) {
		    break;
		}
		
		//# wait USB Wi-Fi event
		ret = __is_connected_wlan();
		if (ret > 0) {
			/* set attach event */
			if (app_cfg->ste.bit.wlan == 0) {
				dprintf("attached wlan0 device!!\n");
				app_cfg->ste.bit.wlan = 1;
				netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_WIFI, 1);
			}
		} else {
			if (app_cfg->ste.bit.wlan == 1) {
				dprintf("removed wlan0 device!!\n");
				app_cfg->ste.bit.wlan = 0;
				netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_WIFI, 0);
			} 
		}
		
		delay_msecs(TIME_WLAN0_POLL_CYCLE);
	} 
	
	tObj->active = 0;
	dprintf("...exit!\n");
	
	return NULL;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) start!
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_poll_wlan_init(void)
{
	app_thr_obj *tObj;
	
	tObj = &iwlan->pObj;
	if (thread_create(tObj, THR_wlan_poll, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr usb wifi poll thread\n");
		return EFAIL;
    }
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_poll_wlan_exit(void)
{
	app_thr_obj *tObj;

	/* delete usb scan object */
    tObj = &iwlan->pObj;
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
	return 0;
}
