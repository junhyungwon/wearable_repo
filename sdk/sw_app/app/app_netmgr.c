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
	netmgr_shm_request_info_t *info;
	char *databuf;
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
	/* memory clear */
	memset(databuf, 0, NETMGR_SHM_REQUEST_INFO_SZ);
					
	if (ste) {
		/* Wi-Fi 장치가 연결되었을 때 필요한 루틴을 수행 */
		app_cfg->ste.b.usbnet_ready = 1;
		
		if (mode)
		{
		    /* AP MODE */
			int enable = inetmgr->wlan_5G_enable;
			
			/* Wi-Fi AP Mode */
			/* shared memory에 접속에 필요한 정보를 기록한다. */
			snprintf(info->ssid, NETMGR_WLAN_SSID_MAX_SZ, "%s", app_set->sys_info.deviceId);
			strcpy(info->passwd, NETMGR_WLAN_AP_PASSWD);
			strcpy(info->ip_address, NETMGR_WLAN_AP_IPADDR);
			strcpy(info->mask_address, NETMGR_WLAN_AP_MASKADDR);
			strcpy(info->gw_address, NETMGR_WLAN_AP_GWADDR);
			info->stealth = 0;
			info->freq = enable;
			info->dhcp = 0;
			
			if (enable) {
				/* 5GHz Wi-Fi */
				info->channel = NETMGR_WLAN_AP_5G_CHANNEL; //44
			} else {
				info->channel = NETMGR_WLAN_AP_2G_CHANNEL;
			}
			
			dprintf("Wi-Fi SOFTAP start (NAME=%s).........\n", info->ssid);
			send_msg(NETMGR_CMD_WLAN_SOFTAP_START);
        } else {
			/* client mode */
			/* Wi-Fi Client Mode */    
			// under working
			
			
			info->en_key = app_set->wifiap.en_key;
			strcpy(info->ssid, app_set->wifiap.ssid);

			if (strlen(app_set->wifiap.pwd) == 0) {
				info->en_key = 0;
			} else {
				strcpy(info->passwd, app_set->wifiap.pwd);
			}
/*			
            for(i = 0 ;i < 4; i++)
			{
			    info->en_key = app_set->wifilist[i].en_key;
			    strcpy(info->ssid, app_set->wifilist[i].ssid);

			    if (strlen(app_set->wifilist[i].pwd) == 0) {
				    info->en_key = 0;
			    } else {
				    strcpy(info->passwd, app_set->wifilist[i].pwd);
			    }
			}
*/			
			if (app_set->net_info.wtype == NET_TYPE_STATIC) {
				info->dhcp = 0;
				strcpy(info->ip_address, app_set->net_info.wlan_ipaddr);
				strcpy(info->mask_address, app_set->net_info.wlan_netmask);
				strcpy(info->gw_address, app_set->net_info.wlan_gateway);
			} else {
				info->dhcp = 1;
			}
			
			if(app_set->multi_ap.ON_OFF) // using multi ap 
				info->dhcp = 1 ;

			if ((strcmp(info->ssid, "AP_SSID") == 0) && (strcmp(info->passwd, "AP_PASSWORD") == 0)) {
				/* 기본값이면 Wi-Fi 실행 안 함 : Notice 할 방법은 없다...... */
			} else {
				dprintf("Wi-Fi STATION start (NAME=%s).........\n", info->ssid);
				send_msg(NETMGR_CMD_WLAN_CLIENT_START);
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
	netmgr_shm_request_info_t *info;
	char *databuf;
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
	/* memory clear */
	memset(databuf, 0, NETMGR_SHM_REQUEST_INFO_SZ);
	
	if (ste) {
		/* rndis 장치가 연결되었을 때 필요한 루틴을 수행 */
		app_cfg->ste.b.usbnet_ready = 1;
		info->dhcp = 1;
		send_msg(NETMGR_CMD_RNDIS_START);
	} else {
		/* rndis 장치가 제거되었을 때 필요한 루틴을 수행 */
		app_cfg->ste.b.usbnet_ready = 0;
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
	netmgr_shm_request_info_t *info;
	char *databuf;
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
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
		eprintf("invalid netdevice --> %x\n", device);
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
	
	//dprintf("current dev 0x%x link status %d\n", device, link);
}

static void __netmgr_dev_ip_status_handler(void)
{
	netmgr_shm_response_info_t *info;
	char *databuf;
	int device = inetmgr->device;
	
	if (device < NETMGR_DEV_TYPE_WIFI || device > NETMGR_DEV_TYPE_CRADLE) {
		eprintf("invalid netdevice --> %x\n", device);
		return;
	}
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(inetmgr->sbuf + NETMGR_SHM_RESPONSE_INFO_OFFSET);
	info = (netmgr_shm_response_info_t *)databuf;

#if 0	
	//# for debugging
	dprintf("[Dev %x] Get dhcp ip address is %s\n", device, info->ip_address);
	dprintf("[Dev %x] Get dhcp mask address is %s\n", device, info->mask_address);
	dprintf("[Dev %x] Get dhcp gateway address is %s\n", device, info->gw_address);
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
	int device = inetmgr->device;
	char msg[128]={0,};
	
	if (level < 0) {
		snprintf(msg, sizeof(msg), "Wi-Fi RSSI is minus, connection closed...");
		dprintf("%s\n", msg);
		app_log_write(MSG_LOG_WRITE, msg);					
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
	char msg[128]={0,};
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
		
		/* log buffer clear */
		memset(msg, 0, sizeof(msg));
		switch (cmd) {
		case NETMGR_CMD_READY:
			__netmgr_start();
			snprintf(msg, sizeof(msg), "app: netmgr ready!");
			//dprintf("%s\n", msg);
			app_log_write(MSG_LOG_WRITE, msg);
			break;
			
		case NETMGR_CMD_DEV_DETECT:
			__netmgr_hotplug_noty();
			snprintf(msg, sizeof(msg), "app: netdevice type %x, state %s", 
						inetmgr->device, inetmgr->insert?"insert":"remove");
			dprintf("%s\n", msg);
			app_log_write(MSG_LOG_WRITE, msg);
			break;
		
		case NETMGR_CMD_DEV_LINK_STATUS:
			__netmgr_dev_link_status_handler();
			snprintf(msg, sizeof(msg), "app: device type %x, link status %x", 
						inetmgr->device, inetmgr->link_status);
			//dprintf("%s\n", msg);
			app_log_write(MSG_LOG_WRITE, msg);
			break;
		
		case NETMGR_CMD_DEV_IP_STATUS:
			__netmgr_dev_ip_status_handler();
			snprintf(msg, sizeof(msg), "app: get device type %x, ip status!", 
						inetmgr->device);
			dprintf("%s\n", msg);
			app_log_write(MSG_LOG_WRITE, msg);
			break;
			
		case NETMGR_CMD_WLAN_CLIENT_RSSI:
			/* if rssi == -1, then connection should closed... */
			__netmgr_rssi_status_handler(inetmgr->rssi_level);
			break;
			
		case NETMGR_CMD_PROG_EXIT:
			exit = 1;
			snprintf(msg, sizeof(msg), "app: netmgr exit!");
			//dprintf("%s\n", msg);
			app_log_write(MSG_LOG_WRITE, msg);
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
	
	aprintf("done!...\n");

	return SOK;
}

int app_netmgr_get_usbnet_dev(void) 
{
	return inetmgr->cur_usb_net;
}
