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
#include "cradle_eth.h"
#include "event_hub.h"
#include "common.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_CRADLE_ETH_CYCLE			200		//# msec
#define TIME_CRADLE_ETH_WAIT_DHCP		10000   //# 10sec
#define CNT_CRADLE_ETH_WAIT_DHCP  		(TIME_CRADLE_ETH_WAIT_DHCP/TIME_CRADLE_ETH_CYCLE)

#define __STAGE_CRADLE_ETH_WAIT_ACTIVE	(0x00)
#define __STAGE_CRADLE_ETH_WAIT_DHCP	(0x01)
#define __STAGE_CRADLE_ETH_DHCP_NOTY	(0x02)
#define __STAGE_CRADLE_ETH_GET_STATUS	(0x03)

#define NETMGR_CRADLE_ETH_DEVNAME		"eth0"

typedef struct {
	app_thr_obj cObj;
	
	char ip[NETMGR_NET_STR_MAX_SZ+1];
    char mask[NETMGR_NET_STR_MAX_SZ+1];
    char gw[NETMGR_NET_STR_MAX_SZ+1];
	
	int dhcp;
	int stage;
	int cradle_eth_timer;
	
} netmgr_cradle_eth_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_cradle_eth_t t_cradle_eth;
static netmgr_cradle_eth_t *icradle = &t_cradle_eth;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_cradle_eth_main(void *prm)
{
	app_thr_obj *tObj = &icradle->cObj;
	int exit = 0, cmd;
	int quit = 0;
	
	tObj->active = 1;
	
	while (!exit)
	{
		int res;
		
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT)
			break;
		else if (cmd == APP_CMD_START) {
			quit = 0; /* for loop */ 
		}
		
		while (!quit)
		{
			int st;
			
			cmd = tObj->cmd;
			if (cmd == APP_CMD_STOP) {
				if (icradle->dhcp == 1) {
					netmgr_udhcpc_stop(NETMGR_CRADLE_ETH_DEVNAME);
				}
				netmgr_net_link_down(NETMGR_CRADLE_ETH_DEVNAME);
				netmgr_event_hub_link_status(NETMGR_DEV_TYPE_CRADLE, NETMGR_DEV_INACTIVE);
				quit = 1;
				continue;
			}
			
			st = icradle->stage;
			switch (st) {
			case __STAGE_CRADLE_ETH_GET_STATUS:
				res = netmgr_is_netdev_active(NETMGR_CRADLE_ETH_DEVNAME);
				if (!res) {
					icradle->stage = __STAGE_CRADLE_ETH_WAIT_ACTIVE;
					netmgr_event_hub_link_status(NETMGR_DEV_TYPE_CRADLE, NETMGR_DEV_INACTIVE);
				}
				break;
			
			case __STAGE_CRADLE_ETH_DHCP_NOTY:
				/* 현재 sema_wait이 1로 구현되어 있어서 event_send를 동시에 진행할 수 업다. 따라서 별도의 상태로 구분함..*/
				netmgr_get_net_info(NETMGR_CRADLE_ETH_DEVNAME, NULL, icradle->ip, icradle->mask, icradle->gw);
				dprintf("cradle ip is %s\n", icradle->ip);
				netmgr_set_shm_ip_info(NETMGR_DEV_TYPE_CRADLE, icradle->ip, icradle->mask, icradle->gw);
				netmgr_event_hub_dhcp_noty(NETMGR_DEV_TYPE_CRADLE);
				icradle->stage = __STAGE_CRADLE_ETH_GET_STATUS;
				break;
				
			case __STAGE_CRADLE_ETH_WAIT_DHCP:
				//# check done pipe(udhcpc...)
				if (netmgr_udhcpc_is_run(NETMGR_CRADLE_ETH_DEVNAME)) {
					icradle->stage = __STAGE_CRADLE_ETH_DHCP_NOTY;
					netmgr_event_hub_link_status(NETMGR_DEV_TYPE_CRADLE, NETMGR_DEV_ACTIVE);
				} else {
					if (icradle->cradle_eth_timer >= CNT_CRADLE_ETH_WAIT_DHCP) {
						/* error */
						netmgr_event_hub_link_status(NETMGR_DEV_TYPE_CRADLE, NETMGR_DEV_ERROR);
						icradle->cradle_eth_timer = 0;
						quit = 1; /* loop exit */
					} else {
						icradle->cradle_eth_timer++;
					}
				}
				break;
			
			case __STAGE_CRADLE_ETH_WAIT_ACTIVE:
				res = netmgr_is_netdev_active(NETMGR_CRADLE_ETH_DEVNAME);
				if (res) {
					/* 케이블이 연결되고 IP 할당이 안된 경우 */
					if (icradle->dhcp == 0) {
						/* static ip alloc */
						netmgr_set_ip_static(NETMGR_CRADLE_ETH_DEVNAME, icradle->ip, 
									icradle->mask, icradle->gw);
						netmgr_event_hub_link_status(NETMGR_DEV_TYPE_CRADLE, NETMGR_DEV_ACTIVE);			
						icradle->stage = __STAGE_CRADLE_ETH_GET_STATUS;
					} else {
						/* dhcp ip alloc */
						netmgr_set_ip_dhcp(NETMGR_CRADLE_ETH_DEVNAME);
						icradle->stage = __STAGE_CRADLE_ETH_WAIT_DHCP;
					}
				}
				break;
			default:
				break;	
			}
			
			delay_msecs(TIME_CRADLE_ETH_CYCLE);	
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
int netmgr_cradle_eth_init(void)
{
	app_thr_obj *tObj = &icradle->cObj;
	
	if (thread_create(tObj, THR_cradle_eth_main, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr cradle eth thread\n");
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
int netmgr_cradle_eth_exit(void)
{
	app_thr_obj *tObj = &icradle->cObj;
	
	/* delete usb scan object */
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
int netmgr_cradle_eth_event_start(void)
{
	app_thr_obj *tObj = &icradle->cObj;
	char *databuf;
	netmgr_shm_request_info_t *info;
	
	/* shared memory로부터 IP 정보를 읽어온다 */
	databuf = (char *)(app_cfg->shm_buf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
	icradle->dhcp = info->dhcp;
	icradle->stage = __STAGE_CRADLE_ETH_WAIT_ACTIVE;
	icradle->cradle_eth_timer = 0;
	
	memset(icradle->ip, 0, NETMGR_NET_STR_MAX_SZ);
	memset(icradle->mask, 0, NETMGR_NET_STR_MAX_SZ);
	memset(icradle->gw, 0, NETMGR_NET_STR_MAX_SZ);
	
	if (icradle->dhcp == 0) { //# static 
		strcpy(icradle->ip, info->ip_address);
		strcpy(icradle->mask, info->mask_address);
		strcpy(icradle->gw, info->gw_address);
		//dprintf("cradle eth set ip static!\n");
	} else {
		//dprintf("cradle eth set ip dhcp!\n");
	}
	
	netmgr_net_link_up(NETMGR_CRADLE_ETH_DEVNAME); 
   	event_send(tObj, APP_CMD_START, 0, 0);
	
	aprintf("... done!\n");
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_cradle_eth_event_stop(void)
{
	app_thr_obj *tObj = &icradle->cObj;
	
   	event_send(tObj, APP_CMD_STOP, 0, 0);
	
	aprintf("... done!\n");
	
	return 0;
}
