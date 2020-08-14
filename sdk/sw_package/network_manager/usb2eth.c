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
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj uObj;
	
	char ip[NETMGR_NET_STR_MAX_SZ+1];
    char mask[NETMGR_NET_STR_MAX_SZ+1];
    char gw[NETMGR_NET_STR_MAX_SZ+1];
	
	int dhcp;
	
} netmgr_usb2ether_t;

#define NETMGR_USB2ETH_DEVNAME	"eth1"

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_usb2ether_t t_usb2ether;
static netmgr_usb2ether_t *iusb2eth = &t_usb2ether;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
/*
 * 이 함수는 ifconfig ethX up을 해야 값을 읽을 수 있다.
 */
static int __is_connected_usb2eth(void)
{
	FILE *fp = NULL;
    char buf[32] = {0, };
    int status=0;
    unsigned char val;

    snprintf(buf, sizeof(buf), "/sys/class/net/eth1/carrier");
    
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
static void *THR_usb2eth_main(void *prm)
{
	app_thr_obj *tObj = &iusb2eth->uObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT)
			break;
		else if (cmd == APP_CMD_STOP) {
			netmgr_net_link_down(NETMGR_USB2ETH_DEVNAME);
		}
		else if (cmd == APP_CMD_START) 
		{
			if (iusb2eth->dhcp == 0)  {//
				netmgr_set_ip_static(NETMGR_USB2ETH_DEVNAME, iusb2eth->ip, iusb2eth->mask, iusb2eth->gw);
			} else {
				netmgr_set_ip_dhcp(NETMGR_USB2ETH_DEVNAME);
			}
		} 
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
int netmgr_usb2eth_init(void)
{
	app_thr_obj *tObj = &iusb2eth->uObj;
	
	if (thread_create(tObj, THR_usb2eth_main, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr usb2eth thread\n");
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
int netmgr_usb2eth_exit(void)
{
	app_thr_obj *tObj = &iusb2eth->uObj;
	
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
int netmgr_usb2eth_event_start(void)
{
	app_thr_obj *tObj = &iusb2eth->uObj;
	char *databuf;
	netmgr_shm_request_info_t *info;
	
	/* shared memory로부터 IP 정보를 읽어온다 */
	databuf = (char *)(app_cfg->shm_buf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
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
