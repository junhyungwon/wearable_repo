/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_rec.c
 * @brief	app record thread
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "netmgr_ipc_cmd_defs.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_msg.h"
#include "app_set.h"
#include "app_file.h"
#include "app_util.h"
#include "app_netmgr.h"
#include "app_set.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj sObj;		//# rec message send thread
	app_thr_obj rObj;		//# rec message send thread
	
	int qid;
	
	int device;
	int insert;
	int link_status;
	int wlan_5G_enable;
	int rssi_level;
	
	int shmid;  /* shared memory qid */
	unsigned char *sbuf;  /* shared memory address */
	
} app_netmgr_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_netmgr_t netmgr_obj;
static app_netmgr_t *inetmgr=&netmgr_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static void *THR_netmgr_send_msg(void *prm);

static char *__get_netdev_string(int type)
{
	switch (type) {
	case NETMGR_DEV_TYPE_WIFI: return "wlan";
	case NETMGR_DEV_TYPE_USB2ETHER: return "usb2ether";
	case NETMGR_DEV_TYPE_RNDIS: return "rndis";
	case NETMGR_DEV_TYPE_CRADLE: return "eth0";
	}
	
	return "unknown";
}

static int send_msg(int cmd)
{
	to_netmgr_msg_t msg;
	
	msg.des = NETMGR_MSG_TYPE_TO_NETMGR;
	msg.cmd = cmd;

	return Msg_Send(inetmgr->qid, (void *)&msg, sizeof(to_netmgr_msg_t));
}

static int recv_msg(void)
{
	to_netmgr_main_msg_t msg;
	int size;
	
	//# blocking
	if (Msg_Rsv(inetmgr->qid, NETMGR_MSG_TYPE_TO_MAIN, (void *)&msg, sizeof(to_netmgr_main_msg_t)) < 0)
		return -1;
	
	if (msg.cmd == NETMGR_CMD_DEV_DETECT) {
		inetmgr->device = msg.device;
		inetmgr->insert = msg.status;
		inetmgr->wlan_5G_enable = msg.wlan_5G_enable;
	} 
	else if (msg.cmd == NETMGR_CMD_DEV_LINK_STATUS) {
		inetmgr->device      = msg.device;
		inetmgr->link_status = msg.status;
	}
	else if (msg.cmd == NETMGR_CMD_WLAN_CLIENT_RSSI) {
		inetmgr->rssi_level = msg.wlan_rssi;
	}
	
	return msg.cmd;
}

/*****************************************************************************
* @brief    app netmgr device detection handler
* @section  [desc]
*****************************************************************************/
static void __netmgr_hotplug_noty(void)
{
	app_thr_obj *tObj = &inetmgr->sObj;
	event_send(tObj, APP_KEY_UP, 0, 0);
}

static void __netmgr_wlan_event_handler(int ste)
{
	netmgr_shm_request_info_t *info;
	char *databuf;
	int mode = 0; /* set default client mode */
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
	if ((strcmp(MODEL_NAME, "NEXX360") == 0) && (app_cfg->vid_count == 0)) {
		mode = 1; /* AP Mode */			
	} 
	/* memory clear */
	memset(databuf, 0, NETMGR_SHM_REQUEST_INFO_SZ);
					
	if (ste) {
		/* Wi-Fi 장치가 연결되었을 때 필요한 루틴을 수행 */
		dprintf("Wi-Fi %s start.........\n", mode?"AP":"CLIENT");
		if (mode)
		{
		    /* AP MODE */
			int enable = inetmgr->wlan_5G_enable;
			
			/* Wi-Fi AP Mode */
			/* shared memory에 접속에 필요한 정보를 기록한다. */
			//app_set->wifiap.ssid
			//app_set->wifiap.pwd
			//#app_set->wifiap.stealth;
			//#channel = 36 or 44, 2.4G 6
			
			/* ssid set */
			snprintf(info->ssid, NETMGR_WLAN_SSID_MAX_SZ, "%s", app_set->sys_info.deviceId);
			strcpy(info->passwd,"12345678");
			strcpy(info->ip_address, "192.168.0.1");
			strcpy(info->mask_address, "255.255.255.0");
			strcpy(info->gw_address, "192.168.0.1");
			info->stealth = 0;
			info->freq = enable;
			info->dhcp = 0;
			
			if (enable) {
				/* 5GHz Wi-Fi */
				info->channel = 36; //44
			} else {
				info->channel = 6;
			}
			
			send_msg(NETMGR_CMD_WLAN_SOFTAP_START);
        } else {
			/* client mode */
			/* Wi-Fi Client Mode */
			info->en_key = app_set->wifiap.en_key;
			strcpy(info->ssid, app_set->wifiap.ssid);

			if (strlen(app_set->wifiap.pwd) == 0) {
				info->en_key = 0;
			} else {
				strcpy(info->passwd, app_set->wifiap.pwd);
			}
#if 0			
			if (app_set->net_info.type == NET_TYPE_STATIC) 
			{
				info->dhcp = 0;
				strcpy(info->ip_address, app_set->net_info.wlan_ipaddr);
				strcpy(info->mask_address, app_set->net_info.wlan_netmask);
				strcpy(info->gw_address, app_set->net_info.wlan_gateway);
			} else {
				info->dhcp = 1;
			}
#else
			/* static ip not supported??*/
			info->dhcp = 1;
#endif			
			send_msg(NETMGR_CMD_WLAN_CLIENT_START);
		}
		
	} else {
		dprintf("Wi-Fi %s STOP.........\n", mode?"AP":"CLIENT");
		/* Wi-Fi 장치가 제거되었을 때 필요한 루틴을 수행 */
		if (mode) {
			/* AP mode stop */
			send_msg(NETMGR_CMD_WLAN_SOFTAP_STOP);
		} else {
			send_msg(NETMGR_CMD_WLAN_CLIENT_STOP);
		}
	}
}

static void __netmgr_rndis_event_handler(int ste)
{
	netmgr_shm_request_info_t *info;
	char *databuf;
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
	/* memory clear */
	memset(databuf, 0, NETMGR_SHM_REQUEST_INFO_SZ);
	
	if (ste) {
		/* rndis 장치가 연결되었을 때 필요한 루틴을 수행 */
		info->dhcp = 1;
		send_msg(NETMGR_CMD_RNDIS_START);
	} else {
		/* rndis 장치가 제거되었을 때 필요한 루틴을 수행 */
		send_msg(NETMGR_CMD_RNDIS_STOP);
	}
}

static void __netmgr_usb2eth_event_handler(int ste)
{
	netmgr_shm_request_info_t *info;
	char *databuf;
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
	/* memory clear */
	memset(databuf, 0, NETMGR_SHM_REQUEST_INFO_SZ);
	
	if (ste) {
		/* usb2eth 장치가 연결되었을 때 필요한 루틴을 수행 */
		if (app_set->net_info.type == NET_TYPE_STATIC) {
			info->dhcp = 0;
			strcpy(info->ip_address, app_set->net_info.eth_ipaddr);
			strcpy(info->mask_address, app_set->net_info.eth_netmask);
			strcpy(info->gw_address, app_set->net_info.eth_gateway);
		} else {
			info->dhcp = 1;
		}
		send_msg(NETMGR_CMD_USB2ETH_START);
	} else {
		/* usb2eth 장치가 제거되었을 때 필요한 루틴을 수행 */
		send_msg(NETMGR_CMD_USB2ETH_STOP);
	}
}

static void __netmgr_cradle_eth_event_handler(int ste)
{
	netmgr_shm_request_info_t *info;
	char *databuf;
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
	/* memory clear */
	memset(databuf, 0, NETMGR_SHM_REQUEST_INFO_SZ);
	
	if (ste) {
		app_cfg->ste.b.st_cradle = 1;
		/* cradle 장치가 연결되었을 때 필요한 루틴을 수행 */
		if (app_set->net_info.type == NET_TYPE_STATIC) {
			info->dhcp = 0;
			strcpy(info->ip_address, app_set->net_info.eth_ipaddr);
			strcpy(info->mask_address, app_set->net_info.eth_netmask);
			strcpy(info->gw_address, app_set->net_info.eth_gateway);
		} else {
			info->dhcp = 1;
		}
		send_msg(NETMGR_CMD_CRADLE_ETH_START);
	} else {
		app_cfg->ste.b.st_cradle = 0;
		/* cradle 장치가 제거되었을 때 필요한 루틴을 수행 */
		send_msg(NETMGR_CMD_CRADLE_ETH_STOP);
	}
}

static void __netmgr_dev_link_status_handler(void)
{
	int device = inetmgr->device;
	int link   = inetmgr->link_status;
	
	if (device == NETMGR_DEV_TYPE_WIFI) {
		if (link == NETMGR_DEV_ACTIVE) {
			app_cfg->ste.b.wifi = 1;
			app_leds_rf_ctrl(LED_RF_OK);
		} else if (link == NETMGR_DEV_ERROR)  {
			app_cfg->ste.b.wifi = 0;
			app_leds_rf_ctrl(LED_RF_FAIL);
		} else {
			app_cfg->ste.b.wifi = 0;
			app_leds_rf_ctrl(LED_RF_OFF);
		} 
	} else if (device == NETMGR_DEV_TYPE_USB2ETHER) {
		if (link == NETMGR_DEV_ACTIVE) {
			app_cfg->ste.b.eth1_run = 1;
			app_leds_rf_ctrl(LED_RF_OK);
		} else if (link == NETMGR_DEV_ERROR) {
			app_cfg->ste.b.eth1_run = 0;
			app_leds_rf_ctrl(LED_RF_FAIL);	
		} else {
			app_cfg->ste.b.eth1_run = 0;
			app_leds_rf_ctrl(LED_RF_OFF);
		}
	} else if (device == NETMGR_DEV_TYPE_RNDIS) {
		if (link == NETMGR_DEV_ACTIVE) {
			app_cfg->ste.b.dial_run = 1;
			app_leds_rf_ctrl(LED_RF_OK);
		} else if (link == NETMGR_DEV_ERROR)  {
			app_cfg->ste.b.dial_run = 0;
			app_leds_rf_ctrl(LED_RF_FAIL);
		} else {
			app_cfg->ste.b.dial_run = 0;
			app_leds_rf_ctrl(LED_RF_OFF);
		}
	} else if (device == NETMGR_DEV_TYPE_CRADLE) {
		if (link == NETMGR_DEV_ACTIVE) {
			app_cfg->ste.b.eth0_run = 1;
			app_cfg->ftp_enable = ON;
		} else {
			app_cfg->ste.b.eth0_run = 0;
			app_cfg->ftp_enable = OFF;
		}
	} 
}

static void __netmgr_start(void)
{
	app_thr_obj *tObj = &inetmgr->sObj;
	
	/* 프로세서가 시작된 후 초기화 작업이 필요한 경우 */
	//# get shared memory id
	inetmgr->shmid = shmget((key_t)NETMGR_SHM_KEY, 0, 0);
	if (inetmgr->shmid == -1) {
		/* debugging 목적 */
		eprintf("shared memory for netmgr is not created!!\n");
	}

	//# get shared memory
	inetmgr->sbuf = (unsigned char *)shmat(inetmgr->shmid, NULL, 0);
	if (inetmgr->sbuf == NULL) {
		/* debugging 목적 */
		eprintf("shared memory for netmgr is NULL!!!");
	}
	
	//#--- create msg send thread
	if (thread_create(tObj, THR_netmgr_send_msg, APP_THREAD_PRI, tObj) < 0) {
		eprintf("create thread\n");
	}
	
	send_msg(NETMGR_CMD_PROG_START);
}

/*****************************************************************************
* @brief    event record thread function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_netmgr_send_msg(void *prm)
{
	app_thr_obj *tObj = &inetmgr->sObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			break;
		}
		
		switch (cmd) 
		{
			case APP_KEY_UP:
			{
				/* 장치가 연결되거나 제거된 경우 관련 루틴을 실행 */
				int type   = inetmgr->device;
				int status = inetmgr->insert;
				
				if (type == NETMGR_DEV_TYPE_WIFI) 
				{
					__netmgr_wlan_event_handler(status);
				}
				else if (type == NETMGR_DEV_TYPE_RNDIS) 
				{
					__netmgr_rndis_event_handler(status);
				}
				else if (type == NETMGR_DEV_TYPE_USB2ETHER)
				{
					__netmgr_usb2eth_event_handler(status);
				}
				else if (type == NETMGR_DEV_TYPE_CRADLE)
				{
					__netmgr_cradle_eth_event_handler(status);
				}
				break;
			}
		} 
	}
	
	tObj->active = 0;
	aprintf("exit\n");
	
	return NULL;
}

/*****************************************************************************
* @brief    event message receive function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_netmgr_recv_msg(void *prm)
{
	app_thr_obj *tObj = &inetmgr->rObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	
	//# message queue
	inetmgr->qid = Msg_Init(NETMGR_MSG_KEY);
	
	while (!exit) {
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive netmgr process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case NETMGR_CMD_READY:
			//dprintf("received netmgr ready!\n");
			__netmgr_start();
			break;
			
		case NETMGR_CMD_DEV_DETECT:
			dprintf("device type %s, state %s!\n", __get_netdev_string(inetmgr->device), inetmgr->insert?"insert":"remove");
			__netmgr_hotplug_noty();
			break;
		
		case NETMGR_CMD_DEV_LINK_STATUS:
			dprintf("received netmgr device link status!\n");
			//dprintf("device type %x, link status %x!\n", __get_netdev_string(inetmgr->device), inetmgr->link_status);
			__netmgr_dev_link_status_handler();
			break;
		
		case NETMGR_CMD_WLAN_CLIENT_RSSI:
			//dprintf("received netmgr Wi-Fi RSSi!\n");
			//dprintf("rssi level = %d\n", inetmgr->rssi_level);
			break;
			
		case NETMGR_CMD_PROG_EXIT:
			exit = 1;
			dprintf("received netmgr exit!\n");
			break;
		default:
			break;	
		}
	}
	
	Msg_Kill(inetmgr->qid);
	
	aprintf("exit...\n");
		
	return NULL;
}

/*****************************************************************************
* @brief    app netmgr init/exit function
* @section  [desc]
*****************************************************************************/
int app_netmgr_init(void)
{
	app_thr_obj *tObj;
	char cmd[128] = {0,};
	
	//# static config clear - when Variable declaration
	memset((void *)inetmgr, 0x0, sizeof(app_netmgr_t));
	
	/* start rec process */
	snprintf(cmd, sizeof(cmd), "/opt/fit/bin/net_mgr.out &");
	system(cmd);
	
	//#--- create msg receive thread
	tObj = &inetmgr->rObj;
	if(thread_create(tObj, THR_netmgr_recv_msg, APP_THREAD_PRI, tObj) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	aprintf("... done!\n");

	return SOK;
}

int app_netmgr_exit(void)
{
	app_thr_obj *tObj;

	//#--- stop message send thread
	tObj = &inetmgr->sObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active) {
		app_msleep(20);
	}
	
	thread_delete(tObj);
	
	//#--- stop message receive thread. 
	//# 프로세스에서 이미 종료가 되므로 APP_CMD_EXIT를 하면 안됨.
//	tObj = &inetmgr->rObj;
//	thread_delete(tObj);
	
	aprintf("done!...\n");

	return SOK;
}
