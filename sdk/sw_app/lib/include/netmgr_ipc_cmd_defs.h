/*
 * File : netmgr_ipc_cmd_defs.h
 *
 * Copyright (C) 2020 LF
 *
 */
#ifndef __NETMGR_IPC_CMD_DEFS_H__
#define __NETMGR_IPC_CMD_DEFS_H__

#if defined (__cplusplus)
extern "C" {
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define NETMGR_MSG_KEY        			0xc54be5  	//# message key.
#define NETMGR_MSG_TYPE_TO_NETMGR		1           //# main app -> netmgr
#define NETMGR_MSG_TYPE_TO_MAIN			2           //# netmgr -> main app

//------------------ Shared Memory ------------------------------------------------------
#define NETMGR_SHM_KEY					0x0A18		//# shared memory id
#define NETMGR_SHM_SIZE					2048		//# wi-fi ssid, passwd, 등을 공유
#define NETMGR_SHM_REQUEST_INFO_SZ		1024
#define NETMGR_SHM_RESPONSE_INFO_SZ		1024

#define NETMGR_SHM_REQUEST_INFO_OFFSET			0
#define NETMGR_SHM_RESPONSE_INFO_OFFSET			1024

//# shared memory layout
//|--------------------------------------------
//|          1024 Byte (REQUEST)             | 
//|                                          |
//|------------------------------------------|
//|          1024 Byte (RESPONSE)            |
//|                                          |
//|------------------------------------------|

//--------------------------------------------------------------------------------------------------------

//#-------------------------------- NETMGR COMMAND LIST --------------------------------------------------
typedef enum {
	NETMGR_CMD_READY = 0x600,
	NETMGR_CMD_PROG_START,     			//# program exit
 	NETMGR_CMD_PROG_EXIT,     			//# program exit
 	NETMGR_CMD_DEV_DETECT,	     		//# usb wi-fi, ethernet, usb2ethernet등이 감지된 경우
	NETMGR_CMD_DEV_LINK_STATUS,   		//# netdevice status!
	NETMGR_CMD_DEV_IP_STATUS,   		//# netdevice ip status!
	NETMGR_CMD_WLAN_SOFTAP_START,   	//# usb wi-fi AP start!
 	NETMGR_CMD_WLAN_SOFTAP_STOP,		//# usb wi-fi AP stop!
 	NETMGR_CMD_WLAN_CLIENT_START,		//# usb wi-fi Client start!
	NETMGR_CMD_WLAN_CLIENT_STOP,    	//# usb wi-fi Client stop!
 	NETMGR_CMD_WLAN_CLIENT_RSSI,   		//# Wi-Fi RSSI!
	NETMGR_CMD_RNDIS_START,     		//# rndis start!
 	NETMGR_CMD_RNDIS_STOP,     			//# rndis stop!
	NETMGR_CMD_USB2ETH_START,   		//# usb2eth start!
 	NETMGR_CMD_USB2ETH_STOP,     		//# usb2eth stop!
	NETMGR_CMD_CRADLE_ETH_START,   		//# cradle eth start!
 	NETMGR_CMD_CRADLE_ETH_STOP,     	//# cradle eth stop!
	
} NETMGR_CMD_LIST_T;

//########################################################################################################

#define NETMGR_DEV_TYPE_WIFI			0x10 
#define NETMGR_DEV_TYPE_USB2ETHER		0x11
#define NETMGR_DEV_TYPE_RNDIS			0x12
#define NETMGR_DEV_TYPE_CRADLE			0x13

#define NETMGR_WLAN_PASSWD_MAX_SZ		64
#define NETMGR_WLAN_SSID_MAX_SZ			32           //# Wi-Fi 표준에 32 char

#define NETMGR_NET_STR_MAX_SZ			16

//----------------------------------------------------------------------------------------------------------
#define NETMGR_DEV_REMOVE				0   /* Network 장치가 해제된 상태 */
#define NETMGR_DEV_INSERT				1   /* Network 장치가 연결된 상태 */ 

#define NETMGR_DEV_ERROR				-1   /* Network 장치가 연결되고 동작에 문제가 발생 */ 
#define NETMGR_DEV_INACTIVE				0   /* Network 장치가 연결되고 IP 할당이 완료 */ 
#define NETMGR_DEV_ACTIVE				1   /* Network 장치가 연결되고 IP 할당이 완료 */ 

/*
 * @brief ipc message buffer type
 */
typedef struct {
	long  des; 		///< Where this message go.
	int	  cmd; 		///< Message command ID.
	
	int wlan_mode;  //# AP or Station
	
} to_netmgr_msg_t;

typedef struct {
	long type;
	int cmd;
	
	int device;           /* 매크로 참조 */
	int status;			  /* 0->remove, 1->insert */
	int wlan_rssi;		  /* Wi-Fi RSSI Level return */
	int wlan_5G_enable;
	
} to_netmgr_main_msg_t;

/*
 * @brief wi-fi 접속 시 필요한 정보를 전달..
 */
typedef struct {
	char ssid[NETMGR_WLAN_SSID_MAX_SZ+1];      //32
	char passwd[NETMGR_WLAN_PASSWD_MAX_SZ+1];  //64
	
	char ip_address[NETMGR_NET_STR_MAX_SZ+1];
    char mask_address[NETMGR_NET_STR_MAX_SZ+1];
    char gw_address[NETMGR_NET_STR_MAX_SZ+1];
	
	int level;		 /* signal level (ap mode에서는 사용 안 함) */
	int en_key; 	 /* encryption mode (ap mode에서는 사용 안 함) */
	int freq;        /* 0-> 2.4GHz 1-> 5GHz */
	int stealth;     /* Hiddel SSID */
	int channel;     /* channel number (2.4GH ->6, 5GHz -> 36) */
	int dhcp;	 	/* 0-> static, 1-> dhcp */
	
} netmgr_shm_request_info_t;

/*
 * @brief response info..
 */
typedef struct {
	char ip_address[NETMGR_NET_STR_MAX_SZ+1];
    char mask_address[NETMGR_NET_STR_MAX_SZ+1];
    char gw_address[NETMGR_NET_STR_MAX_SZ+1];
	
	int device;
	
} netmgr_shm_response_info_t;

#if defined (__cplusplus)
}
#endif /* __cplusplus */
#endif /* __NETMGR_IPC_CMD_DEFS_H__ */
