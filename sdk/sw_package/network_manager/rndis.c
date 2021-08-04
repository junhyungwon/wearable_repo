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
#define TIME_RNDIS_WAIT_ACTIVE		20000   //# max 20sec
#define CNT_RNDIS_WAIT_ACTIVE  		(TIME_RNDIS_WAIT_ACTIVE/TIME_RNDIS_CYCLE)

#define TIME_RNDIS_WAIT_DHCP		10000   //# 10sec
#define CNT_RNDIS_WAIT_DHCP  		(TIME_RNDIS_WAIT_DHCP/TIME_RNDIS_CYCLE)

#define __STAGE_RNDIS_WAIT_ACTIVE	(0x00)
#define __STAGE_RNDIS_WAIT_DHCP		(0x01)
#define __STAGE_RNDIS_DHCP_VERIFY	(0x02)
#define __STAGE_RNDIS_DHCP_NOTY		(0x03)
#define __STAGE_RNDIS_GET_STATUS	(0x04)
#define __STAGE_RNDIS_ERROR_STOP	(0x05)
 
#define RNDIS_DEVNAME(a)			((a==1)?"usb0":"eth1")
#define RNDIS_DEV_NAME_USB	  		1
#define RNDIS_DEV_NAME_ETH	  		2

typedef struct {
	app_thr_obj rObj; /* rndis mode */
	
	int stage;
	int iftype;
	int first;
	int rndis_timer;
	
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
	int ret = -1;
	
	memset(devname, 0, sizeof(devname));
	
	if (0 == access(RNDIS_OPER_PATH, R_OK)) {
		strcpy(devname, "usb0");
		ret = RNDIS_DEV_NAME_USB;
	} else if (0 == access(USBETHER_OPER_PATH, R_OK)) {
		strcpy(devname, "eth1");
		ret = RNDIS_DEV_NAME_ETH;
	} else {
		//eprintf("unknown rndis device!!\n");
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

/*
 * 이 함수는 ifconfig ethX up을 해야 값을 읽을 수 있다.
 */
static int __is_rndis_active(const char *ifname)
{
	FILE *fp = NULL;
    char buf[32] = {0, };
    int status=0;
    unsigned char val;

    snprintf(buf, sizeof(buf), "/sys/class/net/%s/carrier", ifname);
    
	fp = fopen(buf, "r") ;
    if (fp != NULL) {   
        fread(&val, 1, 1, fp);
        if (val == '1') {
            status = 1 ; // connect
        } else { 
            status = 0 ; // disconnect
        } 
        fclose(fp);
    }
	
    return status;
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
	
	tObj->active = 1;
	dprintf("enter...!\n");
	
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
			int st = 0, res;
			
			cmd = tObj->cmd;
			if (cmd == APP_CMD_STOP) {
				dprintf("rndis stopping.....\n");
				netmgr_udhcpc_stop(RNDIS_DEVNAME(irndis->iftype));
				netmgr_event_hub_link_status(NETMGR_DEV_TYPE_RNDIS, NETMGR_DEV_INACTIVE);
				quit = 1;
				continue;
			}
			
			st = irndis->stage;
			switch (st) {
			case __STAGE_RNDIS_GET_STATUS:
				/* TODO */
				res = __is_rndis_active(RNDIS_DEVNAME(irndis->iftype));
				if (!res) {
					irndis->first = 0;
					irndis->stage = __STAGE_RNDIS_WAIT_ACTIVE;
					netmgr_event_hub_link_status(NETMGR_DEV_TYPE_RNDIS, NETMGR_DEV_INACTIVE);
				}
				break;
			
			case __STAGE_RNDIS_DHCP_NOTY:
				netmgr_set_shm_ip_info(NETMGR_DEV_TYPE_RNDIS, irndis->ip, irndis->mask, irndis->gw);
				netmgr_event_hub_dhcp_noty(NETMGR_DEV_TYPE_RNDIS);
				irndis->stage = __STAGE_RNDIS_GET_STATUS;
				break;
			
			case __STAGE_RNDIS_DHCP_VERIFY:
				/* 현재 sema_wait이 1로 구현되어 있어서 event_send를 동시에 진행할 수 업다. 따라서 별도의 상태로 구분함..*/
				res = netmgr_get_net_info(RNDIS_DEVNAME(irndis->iftype), NULL, irndis->ip, irndis->mask, irndis->gw);
				if (res < 0) {
					irndis->stage = __STAGE_RNDIS_ERROR_STOP;
				} else {
					if (!strcmp(irndis->ip, "0.0.0.0")) {
						/* dhcp로부터 IP 할당이 안된 경우 */
						dprintf("couln't get dhcp ip address!\n");	
						irndis->stage = __STAGE_RNDIS_ERROR_STOP;
					} else {
						dprintf("rndis ip is %s\n", irndis->ip);
						netmgr_event_hub_link_status(NETMGR_DEV_TYPE_RNDIS, NETMGR_DEV_ACTIVE);
						irndis->stage = __STAGE_RNDIS_DHCP_NOTY;	
					}
				}
				break;
					
			case __STAGE_RNDIS_WAIT_DHCP:
				//# check done pipe(udhcpc...)
				if (netmgr_udhcpc_is_run(RNDIS_DEVNAME(irndis->iftype))) {
					irndis->stage = __STAGE_RNDIS_DHCP_VERIFY;
					irndis->rndis_timer = 0;
				} else {
					irndis->rndis_timer++;
					if (irndis->rndis_timer >= CNT_RNDIS_WAIT_DHCP) {
						dprintf("rndis can't alloc ip address..stopping...\n");
						irndis->stage = __STAGE_RNDIS_ERROR_STOP;
					} 
				}
				break;
				
			case __STAGE_RNDIS_WAIT_ACTIVE:
				res = __wait_for_active();
				if (res > 0) {
					if (!irndis->first) {
						irndis->iftype = res;
						irndis->first = 1;
					}
					//dprintf("active rndis %d(%s)\n", irndis->iftype, RNDIS_DEVNAME(irndis->iftype));
					netmgr_set_ip_dhcp(RNDIS_DEVNAME(irndis->iftype));
					irndis->stage = __STAGE_RNDIS_WAIT_DHCP;
					irndis->rndis_timer = 0;
				} else {
					irndis->rndis_timer++;
					if (irndis->rndis_timer >= CNT_RNDIS_WAIT_ACTIVE) {
						irndis->stage = __STAGE_RNDIS_ERROR_STOP;
					} 
				}
				break;
			
			case __STAGE_RNDIS_ERROR_STOP:
				/* error */
				netmgr_event_hub_link_status(NETMGR_DEV_TYPE_RNDIS, NETMGR_DEV_ERROR);
				quit = 1; /* loop exit */
				break;
				
			default:
				/* nothing to do */
				break;		
			}
			
			delay_msecs(TIME_RNDIS_CYCLE);
		} /* while (!exit) */	
	}
	
	tObj->active = 0;
	dprintf("...exit!\n");
	
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
	irndis->rndis_timer = 0;
	irndis->first = 0;
	
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