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
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define STAGE_IDLE					(0x00)
#define STAGE_WAIT_DEV_ACTIVE		(0x01)
#define STAGE_WAIT_DHCPC			(0x02)
#define STAGE_RUN					(0x03)
 
#define RNDIS_OPER_PATH		  		"/sys/class/net/usb0/operstate"
#define USBETHER_OPER_PATH	  		"/sys/class/net/eth1/operstate"
#define RNDIS_DEV_NAME_USB	  		1
#define RNDIS_DEV_NAME_ETH	  		2
#define UDHCPC_PID_PATH		 		"/tmp/udhcpc.pid"

typedef struct {
	app_thr_obj rObj; /* rndis mode */
	
	int stage;
	int status;
	
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
	int ret = -1, r;

    if (0 == access(RNDIS_OPER_PATH, R_OK)) {
	    f = fopen(RNDIS_OPER_PATH, "r");
	    if (f != NULL) 
		{
			fclose(f);
			//# get hardware address
			netmgr_get_net_info("usb0", irndis->hw_addr, irndis->ip, irndis->mask, irndis->gw);
			//# detected rndis usb0 interface.
			ret = RNDIS_DEV_NAME_USB;
			if (strncmp(irndis->hw_addr, "00:00:00:00:00:00", 17) == 0) {
				//# set mac address temporarily
				f = popen("/sbin/ifconfig usb0 hw ether 00:11:22:33:44:55", "r");
				if (f != NULL) {
					pclose(f); 
				}
			} else { 
				dprintf("get tmp_buf mac = %s\n", irndis->hw_addr);
			}
        }
	} 
	//# S10+ device recognized as eth1 
	else if (0 == access(USBETHER_OPER_PATH, R_OK)) { 
		f = fopen(USBETHER_OPER_PATH, "r");
		if (f != NULL) 
		{
	        fclose(f);
			//# get hardware address
			netmgr_get_net_info("eth1", irndis->hw_addr, irndis->ip, irndis->mask, irndis->gw);
			//# detected rndis eth1 interface.
			ret = RNDIS_DEV_NAME_ETH;
			if (strncmp(irndis->hw_addr, "00:00:00:00:00:00", 17) == 0) {
				//# set mac address temporarily
				f = popen("/sbin/ifconfig eth1 hw ether 00:11:22:33:44:55", "r");
				if (f != NULL) {
					pclose(f); 
				}
			} else {
				dprintf("get tmp_buf mac = %s\n", irndis->hw_addr);
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
	int exit = 0, cmd;
	char tmp_buf[128];
	FILE *f;
	int response = 0;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT)
			break;
		
		irndis->stage = STAGE_WAIT_DEV_ACTIVE;
		irndis->status = 0;
		
		//# 루프에 진입하기 위해서는 APP_CMD_START나 APP_CMD_STOP을 수신 해야 함.
		while(!exit)
		{
			int st = 0;
			int type = 0;
			
			cmd = tObj->cmd;
			if (cmd == APP_CMD_STOP) {
				if (irndis->status) 
				{
					irndis->status = 0;
					f = popen("/usr/bin/killall udhcpc", "w");
					if (f != NULL)
						pclose(f);
					
					//if (!app_cfg->ste.b.wifi_run && !app_cfg->ste.b.eth1_run) {
					//	app_leds_rf_ctrl(LED_RF_OFF);
					//}	
					/* 접속이 종료상태를 mainapp에 알려줘야 함. */	 
				}
				break;
			}
			
			st = irndis->stage;
			switch (st) {
			case STAGE_WAIT_DHCPC:
				//# check done pipe(udhcpc...)
				if (0 == access(UDHCPC_PID_PATH, F_OK)) {
					irndis->status = 1;
					//app_leds_rf_ctrl(LED_RF_OK);
					irndis->stage = STAGE_IDLE;
					memset(tmp_buf, 0, sizeof(tmp_buf));
					snprintf(tmp_buf, sizeof(tmp_buf), "rndis ipaddress %s", irndis->ip);
					log_write(tmp_buf);
					response = 1;
				} else {
					irndis->stage = STAGE_WAIT_DHCPC;
					/* 항상 같은 wait 상태일 수 있음..??? */
				}
				break;
			case STAGE_WAIT_DEV_ACTIVE:
				type = __wait_for_active();
				if (type > 0) 
				{
					memset(tmp_buf, 0, sizeof(tmp_buf));
					snprintf(tmp_buf, sizeof(tmp_buf)-1, "/sbin/udhcpc -t 5 -n -i %s -p "UDHCPC_PID_PATH, 
											(type == RNDIS_DEV_NAME_USB) ? "usb0": "eth1");
					f = popen(tmp_buf, "w");
					if (f != NULL) {
						//app_leds_rf_ctrl(LED_RF_OFF);
						pclose(f);
						/* Device 상황을 mainapp에 전달함. */ 
						irndis->stage = STAGE_WAIT_DHCPC;
						irndis->status = NETMGR_RNDIS_STATUS_IDLE;
						response = 1;
					} else {
						irndis->stage = STAGE_IDLE;
						irndis->status = NETMGR_RNDIS_STATUS_ERR;
						response = 1;	
					}
				} else {
					irndis->stage = STAGE_IDLE;
					irndis->status = NETMGR_RNDIS_STATUS_ERR;
					response = 1;
				}
				break;
			case STAGE_IDLE:
			default:
				/* nothing to do */
				break;		
			}
			
			if (response) {
				response = 0;
				netmgr_event_hub_rndis_status(irndis->status);
			}
			
			delay_msecs(200);
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