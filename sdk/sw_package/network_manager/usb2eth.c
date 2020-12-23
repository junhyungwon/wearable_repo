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
#include <poll.h>

#include "netmgr_ipc_cmd_defs.h"
#include "usb2eth.h"
#include "common.h"
#include "event_hub.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_USB2ETH_CYCLE				200		//# msec
#define TIME_USB2ETH_WAIT_DHCP			10000   //# 10sec
#define CNT_USB2ETH_WAIT_DHCP  			(TIME_USB2ETH_WAIT_DHCP/TIME_USB2ETH_CYCLE)

#define __STAGE_USB2ETH_WAIT_ACTIVE		(0x00)
#define __STAGE_USB2ETH_WAIT_DHCP		(0x01)
#define __STAGE_USB2ETH_DHCP_VERIFY		(0x02)
#define __STAGE_USB2ETH_DHCP_NOTY		(0x03)
#define __STAGE_USB2ETH_GET_STATUS		(0x04)
#define __STAGE_USB2ETH_ERROR_STOP		(0x05)

#define NETMGR_USB2ETH_DEVNAME			"eth1"

typedef struct {
	app_thr_obj uObj;
	
	char ip[NETMGR_NET_STR_MAX_SZ+1];
    char mask[NETMGR_NET_STR_MAX_SZ+1];
    char gw[NETMGR_NET_STR_MAX_SZ+1];
	
	int dhcp;
	int stage;
	int usb2eth_timer;
	
} netmgr_usb2ether_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_usb2ether_t t_usb2ether;
static netmgr_usb2ether_t *iusb2eth = &t_usb2ether;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_usb2eth_main(void *prm)
{
	app_thr_obj *tObj = &iusb2eth->uObj;
	int exit = 0, cmd;
	int quit = 0;
	
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT)
			break;
		else if (cmd == APP_CMD_START) {
			quit = 0; /* for loop */ 
		}
		
		while (quit == 0)
		{
			int st, res;
			
			cmd = tObj->cmd;
			if (cmd == APP_CMD_STOP) {
				if (iusb2eth->dhcp == 1) 
					netmgr_udhcpc_stop(NETMGR_USB2ETH_DEVNAME);
				
				netmgr_net_link_down(NETMGR_USB2ETH_DEVNAME);
				netmgr_event_hub_link_status(NETMGR_DEV_TYPE_USB2ETHER, NETMGR_DEV_INACTIVE);
				quit = 1;
				continue;
			}
			
			st = iusb2eth->stage;
			switch (st) {
			case __STAGE_USB2ETH_GET_STATUS:
				res = netmgr_is_netdev_active(NETMGR_USB2ETH_DEVNAME);
				if (!res) {
					iusb2eth->stage = __STAGE_USB2ETH_WAIT_ACTIVE;
					netmgr_event_hub_link_status(NETMGR_DEV_TYPE_USB2ETHER, NETMGR_DEV_INACTIVE);
				}
				break;
			
			case __STAGE_USB2ETH_DHCP_NOTY:
				netmgr_set_shm_ip_info(NETMGR_DEV_TYPE_USB2ETHER, iusb2eth->ip, iusb2eth->mask, iusb2eth->gw);
				netmgr_event_hub_dhcp_noty(NETMGR_DEV_TYPE_USB2ETHER);
				break;
			
			case __STAGE_USB2ETH_DHCP_VERIFY:
				res = netmgr_get_net_info(NETMGR_USB2ETH_DEVNAME, NULL, iusb2eth->ip, iusb2eth->mask, iusb2eth->gw);
				if (res < 0) {
					iusb2eth->stage = __STAGE_USB2ETH_ERROR_STOP;
				} else {
					if (!strcmp(iusb2eth->ip, "0.0.0.0")) {
						/* dhcp로부터 IP 할당이 안된 경우 */
						dprintf("couln't get dhcp ip address!\n");
						iusb2eth->stage = __STAGE_USB2ETH_ERROR_STOP;
					} else {
						dprintf("usb2eth ip is %s\n", iusb2eth->ip);
						netmgr_event_hub_link_status(NETMGR_DEV_TYPE_USB2ETHER, NETMGR_DEV_ACTIVE);
						iusb2eth->stage = __STAGE_USB2ETH_DHCP_NOTY;
					}
				}
				break;
					
			case __STAGE_USB2ETH_WAIT_DHCP:
				//# check done pipe(udhcpc...)
				if (netmgr_udhcpc_is_run(NETMGR_USB2ETH_DEVNAME)) {
					iusb2eth->stage = __STAGE_USB2ETH_DHCP_VERIFY;
					iusb2eth->usb2eth_timer = 0;
				} else {
					iusb2eth->usb2eth_timer++;
					if (iusb2eth->usb2eth_timer >= CNT_USB2ETH_WAIT_DHCP) {
						iusb2eth->stage = __STAGE_USB2ETH_ERROR_STOP;
					} 
				}
				break;
			
			case __STAGE_USB2ETH_WAIT_ACTIVE:
				res = netmgr_is_netdev_active(NETMGR_USB2ETH_DEVNAME);
				if (res) {
					/* 케이블이 연결되고 IP 할당이 안된 경우 */
					if (iusb2eth->dhcp == 0) {
						/* static ip alloc */
						netmgr_set_ip_static(NETMGR_USB2ETH_DEVNAME, iusb2eth->ip, 
									iusb2eth->mask, iusb2eth->gw);
						netmgr_event_hub_link_status(NETMGR_DEV_TYPE_USB2ETHER, NETMGR_DEV_ACTIVE);			
						iusb2eth->stage = __STAGE_USB2ETH_GET_STATUS;
					} else {
						/* dhcp ip alloc */
						iusb2eth->usb2eth_timer = 0;
						netmgr_set_ip_dhcp(NETMGR_USB2ETH_DEVNAME);
						iusb2eth->stage = __STAGE_USB2ETH_WAIT_DHCP;
					}
				}
				break;
				
			case __STAGE_USB2ETH_ERROR_STOP:
				/* error */
				netmgr_event_hub_link_status(NETMGR_DEV_TYPE_USB2ETHER, NETMGR_DEV_ERROR);
				quit = 1; /* loop exit */
				break;
					
			default:
				break;	
			}
			
			delay_msecs(TIME_USB2ETH_CYCLE);	
		} 
	}
	
	tObj->active = 0;
	
	return NULL;
}

/*****************************************************************************
* @brief    int netmgr_wlan_station_init(void)
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_usb2eth_init(void)
{
	app_thr_obj *tObj = &iusb2eth->uObj;
	
	if (thread_create(tObj, THR_usb2eth_main, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr usb2eth thread\n");
		return EFAIL;
    }
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_usb2eth_exit(void)
{
	app_thr_obj *tObj = &iusb2eth->uObj;
	
	/* delete usb scan object */
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_usb2eth_event_start(void)
{
	app_thr_obj *tObj = &iusb2eth->uObj;
	char *databuf;
	netmgr_shm_request_info_t *info;
	
	/* shared memory로부터 IP 정보를 읽어온다 */
	databuf = (char *)(app_cfg->shm_buf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
	iusb2eth->stage = __STAGE_USB2ETH_WAIT_ACTIVE;
	iusb2eth->usb2eth_timer = 0;
	iusb2eth->dhcp = info->dhcp;
	memset(iusb2eth->ip, 0, NETMGR_NET_STR_MAX_SZ);
	memset(iusb2eth->mask, 0, NETMGR_NET_STR_MAX_SZ);
	memset(iusb2eth->gw, 0, NETMGR_NET_STR_MAX_SZ);
	
	if (iusb2eth->dhcp == 0) { //# static 
		strcpy(iusb2eth->ip, info->ip_address);
		strcpy(iusb2eth->mask, info->mask_address);
		strcpy(iusb2eth->gw, info->gw_address);
		dprintf("usb2ether set ip static!\n");
	} else {
		dprintf("usb2ether set ip dhcp!\n");
	}
	
	netmgr_net_link_up(NETMGR_USB2ETH_DEVNAME);
   	event_send(tObj, APP_CMD_START, 0, 0);
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_usb2eth_event_stop(void)
{
	app_thr_obj *tObj = &iusb2eth->uObj;
	
   	event_send(tObj, APP_CMD_STOP, 0, 0);
	
	return 0;
}
