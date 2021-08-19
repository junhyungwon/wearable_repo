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
#define BLACKLIST_USB_VID				0x1076   //# PID9082 or 9003

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
/*
 * lsusb. (format)
 * 1076:9082
 */
static int __check_blacklist(int usb_v)
{
	FILE *f=NULL;

	char cmd[128] = {0,};
	char buffer[256] = {0,};
	char vendor[5]={0,};
	char *save_ptr;

	snprintf(vendor, sizeof(vendor), "%04x", usb_v);

	snprintf(cmd, sizeof(cmd), "/usr/bin/lsusb");
	f = popen(cmd, "r");
	if (f == NULL) {
		eprintf("couldn't access %s\n", cmd);
		return 0;
	}

	while (fgets(buffer, 255, f) != NULL) 
	{
		char *v;
		/* %*s->discard input */
		memset(cmd, 0, sizeof(cmd));
		sscanf(buffer, "%*s%*s%*s%*s%*s%s", cmd);
		/* splite ":" */
		if (cmd != NULL) {
			v = strtok_r(cmd, ":", &save_ptr);
			//dprintf("compare %s with %s\n", v, vendor);	
			if (strncmp(v, vendor, 4) == 0)
			{
				pclose(f);
				//dprintf("detected usb %s\n", vendor);
				return 1; //# founded usb
			}
		}
	}
	pclose(f);

	return 0;
}

static int __is_rndis_connect(void)
{
	FILE *f;
	char buf[256] = {0,};
	int ret = 0;
	
    if ((f = fopen("/proc/modules", "r")) == NULL) {
        return ret;
    }
	
	/*
	 * rndis_host 5101 0 - Live 0xbf1a4000
	 */  
    while ((fgets(buf, sizeof(buf), f)) != NULL) {
		char *s;
		if ((s = strstr(buf, "rndis_host")) != NULL) {
			ret = 1;
			break;
        }
    }
    fclose(f);
	#if 0
	if (ret > 0) {
		dprintf("search --> %s\n", buf);
	}
	#endif
    return ret;
}

static int __is_usb2eth_connnect(void)
{
	FILE *f = NULL;
	int ret = 0, res;
	
	if (0 == access(USBETHER_OPER_PATH, R_OK)) {
		/* 특정 제조사 동글이 검색되는 경우 무시함 */
		res = __check_blacklist(BLACKLIST_USB_VID);
		if (!res) 
		{
			/* USB2Ether Device */
			f = fopen(USBETHER_OPER_PATH, "r");
			if (f != NULL) {
				fclose(f);
				ret = 1;
			}
		}
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
	dprintf("enter...!\n");
	
	while (!exit)
	{
		cmd = tObj->cmd;
        if (cmd == APP_CMD_EXIT) {
		    break;
		}
		
		//# wait USB Rndis Device
		ret = __is_rndis_connect();
		if (ret > 0) {
			/* set rndis attach event */
			if (app_cfg->ste.bit.rndis == 0) {
				dprintf("detected rndis_host device!!\n");
				app_cfg->ste.bit.rndis = 1;
				netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_RNDIS, 1);
			}
		} else {
			/* rndis remove event */
			if (app_cfg->ste.bit.rndis == 1) {
				dprintf("removed rndis_host device!!\n");
				app_cfg->ste.bit.rndis = 0;
				netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_RNDIS, 0);
			} 
			else {
				/* rndis flag가 0일 경우에만 usb2eth 장치를 확인:
				 * rndis 장치가 usb2eth와 동일하게 eth1으로 생성되는 경우.
				 */
				ret = __is_usb2eth_connnect();
				if (ret > 0) {
					/* set usb2eth attach event */
					if (app_cfg->ste.bit.usb2eth == 0) {
						dprintf("detected usb2ether device!!\n");
						app_cfg->ste.bit.usb2eth = 1;
						netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_USB2ETHER, 1);
					}	
				} else {
					/* usb2eth remove event */
					if (app_cfg->ste.bit.usb2eth == 1) {
						dprintf("removed usb2ether device!!\n");
						app_cfg->ste.bit.usb2eth = 0;
						netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_USB2ETHER, 0);
					}
				}
				
			}
		}
		delay_msecs(TIME_USBNET_POLL_CYCLE);
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
