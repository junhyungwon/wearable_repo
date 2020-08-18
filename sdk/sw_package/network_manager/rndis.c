/*
 * File : wlan_station.c
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define __USE_GNU
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "netmgr_ipc_cmd_defs.h"
#include "rndis.h"
#include "event_hub.h"
#include "common.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_RNDIS_CYCLE			200		//# msec
#define TIME_RNDIS_WAIT_ACTIVE		10000   //# 10sec
#define CNT_RNDIS_WAIT_ACTIVE  		(TIME_RNDIS_WAIT_ACTIVE/TIME_RNDIS_CYCLE)

#define TIME_RNDIS_WAIT_DHCP		10000   //# 10sec
#define CNT_RNDIS_WAIT_DHCP  		(TIME_RNDIS_WAIT_DHCP/TIME_RNDIS_CYCLE)

#define __STAGE_RNDIS_WAIT_ACTIVE	(0x00)
#define __STAGE_RNDIS_WAIT_DHCP		(0x01)
#define __STAGE_RNDIS_GET_STATUS	(0x02)
 
#define RNDIS_OPER_PATH		  		"/sys/class/net/usb0/operstate"
#define USBETHER_OPER_PATH	  		"/sys/class/net/eth1/operstate"
#define RNDIS_DEVNAME(a)			((a==1)?"usb0":"eth1")
#define RNDIS_DEV_NAME_USB	  		1
#define RNDIS_DEV_NAME_ETH	  		2

typedef struct {
	app_thr_obj rObj; /* rndis mode */
	
	int stage;
	int iftype;
	int tmr_count;
	
	char ip[16];		//# ip address
	char hw_addr[32];
	char mask[16];
	char gw[16];
	
} netmgr_rndis_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_rndis_t t_rndis;
static netmgr_rndis_t *irndis = &t_rndis;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
 * @brief    checking rndis device mode
 * @section  DESC Description
 *   - desc
*****************************************************************************/
static int __wait_for_active(void)
{
    FILE *f = NULL;
	char devname[16];
	char path[128]={0,};
	int ret = -1, r;
	
	memset(devname, 0, sizeof(devname));
	
	if (0 == access(RNDIS_OPER_PATH, R_OK)) {
		strcpy(devname, "usb0");
		ret = RNDIS_DEV_NAME_USB;
	} else if (0 == access(USBETHER_OPER_PATH, R_OK)) {
		strcpy(devname, "eth1");
		ret = RNDIS_DEV_NAME_ETH;
	} else {
		eprintf("unknown rndis device!!\n");
		return ret;
	}
	
	snprintf(path, sizeof(path), "/sys/class/net/%s/operstate", devname);
    if (0 == access(RNDIS_OPER_PATH, R_OK)) 
	{
	    f = fopen(path, "r");
	    if (f != NULL) 
		{
			fclose(f);
			//# get hardware address
			netmgr_get_net_info(devname, irndis->hw_addr, irndis->ip, irndis->mask, irndis->gw);
			//# detected rndis usb0 interface.
			if (strncmp(irndis->hw_addr, "00:00:00:00:00:00", 17) == 0) {
				//# set mac address temporarily
				memset(path, 0, sizeof(path));
				snprintf(path, sizeof(path), "/sbin/ifconfig %s hw ether 00:11:22:33:44:55", devname);
				f = popen(path, "r");
				if (f != NULL) {
					pclose(f); 
				}
			} else { 
				dprintf("rndis get mac = %s\n", irndis->hw_addr);
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
static void *THR_rndis_main(void *prm)
{
	app_thr_obj *tObj = &irndis->rObj;
	int exit = 0;
	int quit = 0;
	char tmp_buf[256]={0,};
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		int cmd = 0;
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT)
			break;
		else if (cmd == APP_CMD_START) {
			quit = 0; /* for loop */ 
		}
		//# 루프에 진입하기 위해서는 APP_CMD_START나 APP_CMD_STOP을 수신 해야 함.
		while (!quit)
		{
			int st = 0;
			int type = 0;
			
			cmd = tObj->cmd;
			if (cmd == APP_CMD_STOP) {
				netmgr_udhcpc_stop(RNDIS_DEVNAME(irndis->iftype));
				netmgr_event_hub_dev_link_status(NETMGR_DEV_TYPE_RNDIS, NETMGR_DEV_INACTIVE);
				quit = 1;
				continue;
			}
			
			st = irndis->stage;
			switch (st) {
			case __STAGE_RNDIS_GET_STATUS:
				/* TODO */
				netmgr_is_netdev_active(RNDIS_DEVNAME(irndis->iftype));
				break;
			case __STAGE_RNDIS_WAIT_DHCP:
				//# check done pipe(udhcpc...)
				if (netmgr_udhcpc_is_run(RNDIS_DEVNAME(irndis->iftype))) {
					irndis->stage = __STAGE_RNDIS_GET_STATUS;
					netmgr_event_hub_dev_link_status(NETMGR_DEV_TYPE_RNDIS, NETMGR_DEV_ACTIVE);
					
					memset(tmp_buf, 0, sizeof(tmp_buf));
					netmgr_get_net_info(RNDIS_DEVNAME(irndis->iftype), NULL, irndis->ip, irndis->mask, irndis->gw);
					snprintf(tmp_buf, sizeof(tmp_buf), "rndis ipaddress %s", irndis->ip);
					log_write(tmp_buf);
				} else {
					if (irndis->tmr_count >= CNT_RNDIS_WAIT_DHCP) {
						/* error */
						netmgr_event_hub_dev_link_status(NETMGR_DEV_TYPE_RNDIS, NETMGR_DEV_ERROR);
						quit = 1; /* loop exit */
					} else {
						irndis->tmr_count++;
					}
				}
				break;
				
			case __STAGE_RNDIS_WAIT_ACTIVE:
				irndis->iftype = __wait_for_active();
				if (irndis->iftype > 0) 
				{
					netmgr_set_ip_dhcp(RNDIS_DEVNAME(irndis->iftype));
					irndis->stage = __STAGE_RNDIS_WAIT_DHCP;
					irndis->tmr_count = 0;
				} else {
					if (irndis->tmr_count >= CNT_RNDIS_WAIT_ACTIVE) {
						/* error */
						netmgr_event_hub_dev_link_status(NETMGR_DEV_TYPE_RNDIS, NETMGR_DEV_ERROR);
						quit = 1; /* loop exit */
					} else {
						irndis->tmr_count++;
					}
				}
				break;
			default:
				/* nothing to do */
				break;		
			}
			
			delay_msecs(TIME_RNDIS_CYCLE);
		} /* while (!exit) */	
	}
	
	tObj->active = 0;
	aprintf("exit...\n");
	
	return NULL;
}

/*****************************************************************************
* @brief    int netmgr_wlan_station_init(void)
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_rndis_init(void)
{
	app_thr_obj *tObj = &irndis->rObj;
	
	if (thread_create(tObj, THR_rndis_main, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr rndis thread\n");
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
int netmgr_rndis_exit(void)
{
	app_thr_obj *tObj = &irndis->rObj;

   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
    aprintf("... done!\n");
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_rndis_event_start(void)
{
	app_thr_obj *tObj = &irndis->rObj;
	
	irndis->stage = __STAGE_RNDIS_WAIT_ACTIVE;
	irndis->tmr_count = 0;
   	event_send(tObj, APP_CMD_START, 0, 0);
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_rndis_event_stop(void)
{
	app_thr_obj *tObj = &irndis->rObj;

   	event_send(tObj, APP_CMD_STOP, 0, 0);
	
	return 0;
}