/*
 * File : poll_usbnet.c
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
#include <sys/stat.h>

#include "netmgr_ipc_cmd_defs.h"
#include "poll_usbnet.h"
#include "common.h"
#include "event_hub.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_USBNET_POLL_CYCLE			200		//# msec

typedef struct {
	app_thr_obj pObj; /* device detect */
	
} netmgr_poll_usbdev_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_poll_usbdev_t t_proc;
static netmgr_poll_usbdev_t *iudev = &t_proc;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static int __is_connected_rndis(void)
{
	struct stat sb;
	char path[1024 + 1];
	int ret = 0;
	
	//# /sys/module/rndis_host ????? ???? ??? ??? ???.
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), "/sys/module/rndis_host");
	
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		/* mdev?? usb ??? ?? */
		ret = 1;
	} 
	
	return ret;
}

static int __is_connected_usb2eth(void)
{
    FILE *f = NULL;
    int ret = 0;

    if (0 == access(USBETHER_OPER_PATH, R_OK)) {
	    f = fopen(USBETHER_OPER_PATH, "r");
	    if (f != NULL) {
	        ret = 1;
	    }
    	fclose(f);
	}

	return ret;
}

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_usbnet_poll(void *prm)
{
	app_thr_obj *tObj = &iudev->pObj;
	int exit = 0, cmd;
	int ret;
	
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = tObj->cmd;
        if (cmd == APP_CMD_EXIT) {
		    break;
		}
		
		//# wait USB Rndis Device
		ret = __is_connected_rndis();
		if (ret) {
			/* set attach event */
			if (app_cfg->ste.bit.rndis == 0) {
				app_cfg->ste.bit.rndis = 1;
				netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_RNDIS, 1);
			}
		} else {
			/* set remove event */
			if (app_cfg->ste.bit.rndis == 1) {
				app_cfg->ste.bit.rndis = 0;
				netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_RNDIS, 0);
			}
		}
		
		//# wait USB2Ether Device
		ret = __is_connected_usb2eth();
		if (ret) {
			/* set attach event */
			if (app_cfg->ste.bit.usb2eth == 0) {
				app_cfg->ste.bit.usb2eth = 1;
				netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_USB2ETHER, 1);
			}
		} else {
			/* set remove event */
			if (app_cfg->ste.bit.usb2eth == 1) {
				app_cfg->ste.bit.usb2eth = 0;
				netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_USB2ETHER, 0);
			}
		}
		
		delay_msecs(TIME_USBNET_POLL_CYCLE);
	} 
	
	tObj->active = 0;
	
	return NULL;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) start!
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_poll_usbnet_init(void)
{
	app_thr_obj *tObj;
	
	tObj = &iudev->pObj;
	if (thread_create(tObj, THR_usbnet_poll, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr usb device poll thread\n");
		return EFAIL;
    }
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_poll_usbnet_exit(void)
{
	app_thr_obj *tObj;

	/* delete usb scan object */
    tObj = &iudev->pObj;
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
	return 0;
}
