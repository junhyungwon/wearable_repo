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
#define NETMGR_WLAN_AP_PASSWD			"12345678"
#define NETMGR_WLAN_AP_IPADDR			"192.168.0.1"
#define NETMGR_WLAN_AP_MASKADDR			"255.255.255.0"
#define NETMGR_WLAN_AP_GWADDR			"192.168.0.1"
#define NETMGR_WLAN_AP_5G_CHANNEL		36
#define NETMGR_WLAN_AP_2G_CHANNEL		6
		
typedef struct {
	app_thr_obj sObj;		//# net message send thread
	app_thr_obj rObj;		//# net message send thread
	app_thr_obj wObj;		//# wlan event thread
	
	int qid;
	
	int device;
	int insert;
	int link_status;
	int wlan_5G_enable;
	int rssi_level;
	int cur_usb_net;
	
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
static const char *netdev_str(int device)
{
	switch (device) {
	case NETMGR_DEV_TYPE_WIFI:          return "WIFI";
	case NETMGR_DEV_TYPE_USB2ETHER:     return "USB2ETH";
	case NETMGR_DEV_TYPE_RNDIS:       	return "RNDIS";
	case NETMGR_DEV_TYPE_CRADLE:   		return "CRADLE";
	default: return "?";
	}
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
	
	//# blocking
	if (Msg_Rsv(inetmgr->qid, NETMGR_MSG_TYPE_TO_MAIN, (void *)&msg, sizeof(to_netmgr_main_msg_t)) < 0) {
		eprintf("invalid netmgr message (cmd = %x)\n", msg.cmd);
		return -1;
	}
	
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

static void __netmgr_wlan_event_handler(int ste, int mode)
{
	char *databuf;
	int i;
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.(+2048)
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	/* memory clear */
	memset(databuf, 0, NETMGR_SHM_REQUEST_INFO_SZ);
					
	if (ste) {
		/* Wi-Fi 장치가 연결되었을 때 필요한 루틴을 수행 */
		app_cfg->ste.b.usbnet_ready = 1;
		
		if (mode)
		{
			netmgr_iw_hostapd_req_info_t *info = 
							(netmgr_iw_hostapd_req_info_t *)databuf;
			/* sharded memory 이므로 NULL 검사 안 해도 됨 */
		    /* AP MODE (5G or 2.4G) */
			int enable = inetmgr->wlan_5G_enable;
			
			/* Wi-Fi AP Mode */
			/* shared memory에 접속에 필요한 정보를 기록한다. */
			snprintf(info->ssid, NETMGR_WLAN_SSID_MAX_SZ, "%s", app_set->sys_info.deviceId);
			strcpy(info->passwd, NETMGR_WLAN_AP_PASSWD);
			strcpy(info->ip_address, NETMGR_WLAN_AP_IPADDR);
			strcpy(info->mask_address, NETMGR_WLAN_AP_MASKADDR);
			strcpy(info->gw_address, NETMGR_WLAN_AP_GWADDR);
			if (enable) info->channel = NETMGR_WLAN_AP_5G_CHANNEL; //36
			else 		info->channel = NETMGR_WLAN_AP_2G_CHANNEL;
			info->stealth = 0;
			info->freq = enable;
			
			dprintf("Wi-Fi SOFTAP start (NAME=%s).........\n", info->ssid);
			send_msg(NETMGR_CMD_WLAN_SOFTAP_START);
        } else {
			/* Wi-Fi Client Mode */    
			netmgr_iw_supplicant_req_info_t *info = 
							(netmgr_iw_supplicant_req_info_t *)databuf;
			
			/* 이전 CFG와 호환성을 위해서 */
			strcpy(info->iw_data[0].ssid, app_set->wifiap.ssid);
			if (strcmp(app_set->wifiap.pwd, "")==0) {
				info->iw_data[0].en_key = 0;
			} else {
				info->iw_data[0].en_key = 1;
				strcpy(info->iw_data[0].passwd, app_set->wifiap.pwd);
			}
			
			/* 신규로 추가된 총 4개의 접속 정보를 저장 
			 * info->iw_data[0]가 wifiap 구조체 값을 저장하기 때문에
			 * info->iw_data[1]부터 저장해야 함.
			 */
            for (i = 0; i < WIFIAP_CNT; i++) 
			{
				/* NULL 문자 체크 */
				if (strcmp(app_set->wifilist[i].ssid, "") != 0)
			    	strcpy(info->iw_data[i+1].ssid, app_set->wifilist[i].ssid);
			    if (strcmp(app_set->wifilist[i].pwd, "")!=0) {
				    strcpy(info->iw_data[i+1].passwd, app_set->wifilist[i].pwd);
					info->iw_data[i+1].en_key = 1;
			    } else {
				    info->iw_data[i+1].en_key = 0;
			    }
			}
			
			 // using multi ap 
			if (app_set->multi_ap.ON_OFF) {
				info->dhcp = 1 ;
			} else {
				if (app_set->net_info.wtype == NET_TYPE_STATIC) {
					info->dhcp = 0;
					strcpy(info->ip_address, app_set->net_info.wlan_ipaddr);
					strcpy(info->mask_address, app_set->net_info.wlan_netmask);
					strcpy(info->gw_address, app_set->net_info.wlan_gateway);
				} else {
					info->dhcp = 1;
				}
			}

			for (i = 0; i <= WIFIAP_CNT; i++) {
				if (((strcmp(info->iw_data[i].ssid, "") == 0) && (strcmp(info->iw_data[i].passwd, "") == 0)) ||
				    ((strcmp(info->iw_data[i].ssid, "AP_SSID") == 0) && (strcmp(info->iw_data[i].passwd, "AP_PASSWORD") == 0))
				   ) {
					/* 기본값이면 Wi-Fi 실행 안 함 : Notice 할 방법은 없다...... */
				} else {
					dprintf("Wi-Fi STATION start.........\n");
					send_msg(NETMGR_CMD_WLAN_CLIENT_START);
					break;
				}
			}
		}
		
	} else {
		app_cfg->ste.b.usbnet_ready = 0;
		
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
	/*
	 * DHCP로 고정되기 때문에 전달해야 할 정보가 필요없다.
	 */
	if (ste) {
		/* rndis 장치가 연결되었을 때 필요한 루틴을 수행 */
		app_cfg->ste.b.usbnet_ready = 1;
		send_msg(NETMGR_CMD_RNDIS_START);
	} else {
		/* rndis 장치가 제거되었을 때 필요한 루틴을 수행 */
		app_cfg->ste.b.usbnet_ready = 0;
		send_msg(NETMGR_CMD_RNDIS_STOP);
	}
}

static void __netmgr_usb2eth_event_handler(int ste)
{
	char *databuf;
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	/* memory clear */
	memset(databuf, 0, NETMGR_SHM_REQUEST_INFO_SZ);
	
	if (ste) {
		netmgr_usb2eth_req_info_t *info = 
						(netmgr_usb2eth_req_info_t *)databuf;
		
		/* usb2eth 장치가 연결되었을 때 필요한 루틴을 수행 */
		app_cfg->ste.b.usbnet_ready = 1;
		if (app_set->net_info.wtype == NET_TYPE_STATIC) {
			info->dhcp = 0;
			strcpy(info->ip_address, app_set->net_info.wlan_ipaddr);
			strcpy(info->mask_address, app_set->net_info.wlan_netmask);
			strcpy(info->gw_address, app_set->net_info.wlan_gateway);
		} else {
			info->dhcp = 1;
		}
		send_msg(NETMGR_CMD_USB2ETH_START);
	} else {
		/* usb2eth 장치가 제거되었을 때 필요한 루틴을 수행 */
		app_cfg->ste.b.usbnet_ready = 0;
		send_msg(NETMGR_CMD_USB2ETH_STOP);
	}
}

static void __netmgr_cradle_eth_event_handler(int ste)
{
	netmgr_cradle_eth_req_info_t *info;
	char *databuf;
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_cradle_eth_req_info_t *)databuf;
	
	/* memory clear */
	memset(databuf, 0, NETMGR_SHM_REQUEST_INFO_SZ);
	
	if (ste) {
		app_cfg->ste.b.cradle_eth_ready = 1;
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
		app_cfg->ste.b.cradle_eth_ready = 0;
		/* cradle 장치가 제거되었을 때 필요한 루틴을 수행 */
		send_msg(NETMGR_CMD_CRADLE_ETH_STOP);
	}
}

static void __netmgr_dev_link_status_handler(void)
{
	int device = inetmgr->device;
	int link   = inetmgr->link_status;
	
	if (device < NETMGR_DEV_TYPE_WIFI || device > NETMGR_DEV_TYPE_CRADLE) {
		eprintf("invalid netdevice --> %s\n", netdev_str(device));
		return;
	}
	
	/* cradle network device를 제외하고 나머지는 동일한 루틴에서 처리 */
	if (device == NETMGR_DEV_TYPE_CRADLE) {
		if (link == NETMGR_DEV_ACTIVE) {
			app_cfg->ste.b.cradle_eth_run = 1;
			app_cfg->ftp_enable = ON;
		} 
		else if (link == NETMGR_DEV_ERROR)  {
			app_cfg->ste.b.cradle_eth_run = 0;
			app_cfg->ftp_enable = OFF;
		}
		else {
			app_cfg->ste.b.cradle_eth_run = 0;
			app_cfg->ftp_enable = OFF;
		}
	} else {
		if (link == NETMGR_DEV_ACTIVE) {
			app_cfg->ste.b.usbnet_run = 1;
			/* 현재 연결된 USB 장치를 저장 */
			inetmgr->cur_usb_net = device;
			app_leds_rf_ctrl(LED_RF_OK);
		} 
		else if (link == NETMGR_DEV_ERROR)  {
			app_cfg->ste.b.usbnet_run = 0;
			app_leds_rf_ctrl(LED_RF_FAIL);
		} 
		else {
			app_cfg->ste.b.usbnet_run = 0;
			app_leds_rf_ctrl(LED_RF_OFF);
		} 
	}
}

static void __netmgr_dev_ip_status_handler(void)
{
	netmgr_shm_response_info_t *info;
	char *databuf;
	int device = inetmgr->device;
	
	if (device < NETMGR_DEV_TYPE_WIFI || device > NETMGR_DEV_TYPE_CRADLE) {
		eprintf("invalid netdevice --> %s\n", netdev_str(device));
		return;
	}
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_RESPONSE_INFO_OFFSET);
	info = (netmgr_shm_response_info_t *)databuf;

#if 0	
	//# for debugging
	dprintf("[Dev %s] Get dhcp ip address is %s\n", netdev_str(device), info->ip_address);
	dprintf("[Dev %s] Get dhcp mask address is %s\n", netdev_str(device), info->mask_address);
	dprintf("[Dev %s] Get dhcp gateway address is %s\n", netdev_str(device), info->gw_address);
#endif
	
	/* cradle network device를 제외하고 나머지는 동일한 루틴에서 처리 */
	if (device == NETMGR_DEV_TYPE_CRADLE) {
		sprintf(app_set->net_info.eth_ipaddr, "%s", info->ip_address);
        sprintf(app_set->net_info.eth_gateway, "%s", info->gw_address);
        sprintf(app_set->net_info.eth_netmask, "%s", info->mask_address);
	} else {
		if (device == NETMGR_DEV_TYPE_RNDIS) {
			/* 
			 * 기존 설정이 Wi-Fi Static인 경우 RNDIS는 DHCP 모드로 동작하므로
			 * IP 값이 변경된다. 따라서 Wi-Fi IP 설정을 따르는 경우 IP를 복사하지 않음
			 */
			if (app_set->net_info.wtype != NET_TYPE_STATIC) {
				sprintf(app_set->net_info.wlan_ipaddr, "%s", info->ip_address);
				sprintf(app_set->net_info.wlan_gateway, "%s", info->gw_address);
				sprintf(app_set->net_info.wlan_netmask, "%s", info->mask_address);	
			}
		} else {
			sprintf(app_set->net_info.wlan_ipaddr, "%s", info->ip_address);
			sprintf(app_set->net_info.wlan_gateway, "%s", info->gw_address);
			sprintf(app_set->net_info.wlan_netmask, "%s", info->mask_address);
		}
	}
}

static void __netmgr_rssi_status_handler(int level)
{
	if (level < 0) {
		sysprint("Wi-Fi RSSI is minus, connection closed...\n");
	} else {					
		//dprintf("current rssi level is (%d/100)\n", level);	
		/* level 값을 확인 후 추가적인 작업이 필요할 경우를 위해서....*/
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
	if (thread_create(tObj, THR_netmgr_send_msg, APP_THREAD_PRI, tObj, __FILENAME__) < 0) {
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
					#if SYS_CONFIG_WLAN
					if (status == 0) {
						/* USB remove */
						__netmgr_wlan_event_handler(0, 0);
					} else {
						/* insert 이벤트는 AP 모드 진입 여부를 확인하기 위해서 2초 정도 여유를 둔다 */	
						event_send(&inetmgr->wObj, APP_CMD_NOTY, status, 0);
					}
					#endif
				}
				else if (type == NETMGR_DEV_TYPE_RNDIS) 
				{
					#if SYS_CONFIG_WLAN
					__netmgr_rndis_event_handler(status);
					#endif
				}
				else if (type == NETMGR_DEV_TYPE_USB2ETHER)
				{
					#if SYS_CONFIG_WLAN
					__netmgr_usb2eth_event_handler(status);
					#endif
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
			__netmgr_start();
			dprintf("netmgr starting....!\n");
			break;
			
		case NETMGR_CMD_DEV_DETECT:
			__netmgr_hotplug_noty();
			sysprint("[APP_NET] netdevice type %s, state %s\n", 
						netdev_str(inetmgr->device), inetmgr->insert?"insert":"remove");
			break;
		
		case NETMGR_CMD_DEV_LINK_STATUS:
			__netmgr_dev_link_status_handler();
			sysprint("[APP_NET] device type %s, link status 0x%x\n", 
						netdev_str(inetmgr->device), inetmgr->link_status);
			break;
		
		case NETMGR_CMD_DEV_IP_STATUS:
			__netmgr_dev_ip_status_handler();
			sysprint("[APP_NET] Get device type %s, ip status!\n", netdev_str(inetmgr->device));
			break;
			
		case NETMGR_CMD_WLAN_CLIENT_RSSI:
			/* if rssi == -1, then connection should closed... */
			__netmgr_rssi_status_handler(inetmgr->rssi_level);
			break;
			
		case NETMGR_CMD_PROG_EXIT:
			exit = 1;
			sysprint("[APP_NET] netmgr exit!\n");
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
* @brief    wlan event thread function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_netmgr_wlan_thr(void *prm)
{
	app_thr_obj *tObj = &inetmgr->wObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		int done, count;
		
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			break;
		}
		
		//# noty
		count = 10; //# 200ms * 10 = 2sec..
		done = 0;
		while (!done)
		{
			if (tObj->cmd == APP_CMD_EXIT || tObj->cmd == APP_CMD_STOP) {
				break;
			}
			
			if (count <= 0) {
				if (app_cfg->vid_count == 0) {
					__netmgr_wlan_event_handler(1, 1); /* AP mode */
				} else {
					__netmgr_wlan_event_handler(1, 0); /* station mode */
				}
				/* exit wlan processing */
				done = 1;
			} else {
				count--;
			}
			app_msleep(200);
		}
	}
	
	tObj->active = 0;
	aprintf("exit\n");
	
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
	
	#if SYS_CONFIG_WLAN
	//# --create wlan event thread
	tObj = &inetmgr->wObj;
	if(thread_create(tObj, THR_netmgr_wlan_thr, APP_THREAD_PRI, tObj, __FILENAME__) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	#endif
	
	//#--- create msg receive thread
	tObj = &inetmgr->rObj;
	if(thread_create(tObj, THR_netmgr_recv_msg, APP_THREAD_PRI, tObj, __FILENAME__) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	aprintf("... exit!\n");

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
	
	#if SYS_CONFIG_WLAN
	//#--- stop wlan event thread
	tObj = &inetmgr->wObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active) {
		app_msleep(20);
	}
	thread_delete(tObj);
	#endif
	
	//#--- stop message receive thread. 
	//# 프로세스에서 이미 종료가 되므로 APP_CMD_EXIT를 하면 안됨.
//	tObj = &inetmgr->rObj;
//	thread_delete(tObj);
	
	aprintf("...exit!\n");

	return SOK;
}

int app_netmgr_get_usbnet_dev(void) 
{
	return inetmgr->cur_usb_net;
}
