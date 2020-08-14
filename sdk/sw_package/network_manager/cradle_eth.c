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
#include "common.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj cObj;
	
	char ip[NETMGR_NET_STR_MAX_SZ+1];
    char mask[NETMGR_NET_STR_MAX_SZ+1];
    char gw[NETMGR_NET_STR_MAX_SZ+1];
	
	int dhcp;
	
} netmgr_cradle_eth_t;

#define NETMGR_CRADLE_ETH_DEVNAME	"eth0"

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_cradle_eth_t t_cradle_eth;
static netmgr_cradle_eth_t *icradle = &t_cradle_eth;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
/*
 * 이 함수는 ifconfig ethX up을 해야 값을 읽을 수 있다.
 */
static int __is_connected_cradle_eth(void)
{
	FILE *fp = NULL;
    char buf[32] = {0, };
    int status=0;
    unsigned char val;

    snprintf(buf, sizeof(buf), "/sys/class/net/eth0/carrier");
    
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
static void *THR_cradle_eth_main(void *prm)
{
	app_thr_obj *tObj = &icradle->cObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT)
			break;
		else if (cmd == APP_CMD_STOP) {
			netmgr_net_link_down(NETMGR_CRADLE_ETH_DEVNAME);
		}
		else if (cmd == APP_CMD_START) 
		{
			if (icradle->dhcp == 0)  {//
				netmgr_set_ip_static(NETMGR_CRADLE_ETH_DEVNAME, icradle->ip, icradle->mask, icradle->gw);
			} else {
				netmgr_set_ip_dhcp(NETMGR_CRADLE_ETH_DEVNAME);
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
	
	memset(icradle->ip, 0, NETMGR_NET_STR_MAX_SZ);
	memset(icradle->mask, 0, NETMGR_NET_STR_MAX_SZ);
	memset(icradle->gw, 0, NETMGR_NET_STR_MAX_SZ);
	
	if (icradle->dhcp == 0) { //# static 
		strcpy(icradle->ip, info->ip_address);
		strcpy(icradle->mask, info->mask_address);
		strcpy(icradle->gw, info->gw_address);
		dprintf("cradle eth set ip static!\n");
	} else {
		dprintf("cradle eth set ip dhcp!\n");
	}
	 
   	event_send(tObj, APP_CMD_START, 0, 0);
	
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
	
	return 0;
}
