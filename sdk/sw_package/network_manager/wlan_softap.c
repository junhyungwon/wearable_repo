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
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "netmgr_ipc_cmd_defs.h"
#include "wlan_softap.h"
#include "common.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj hObj; /* wlan client mode */
	
	char ssid[NETMGR_WLAN_SSID_MAX_SZ+1];      //32
	char passwd[NETMGR_WLAN_PASSWD_MAX_SZ+1];  //64
	
	int freq;        /* 0-> 2.4GHz 1-> 5GHz */
	int stealth;     /* Hiddel SSID */
	int channel;     /* channel number (2.4GH ->6, 5GHz -> 36) */
	
	int stage;
	int tmr_count;
	
} netmgr_wlan_hostapd_t;

#define HOSTAPD_PID_PATH			"/var/run/hostapd.pid"
#define HOSTAPD_CONFIG				"/etc/hostapd.conf"

/* Wi-Fi Module Path */
#define RTL_8188E_PATH			"/lib/modules/8188eu.ko"
#define RTL_8188C_PATH			"/lib/modules/8192cu.ko"
#define RTL_8821A_PATH			"/lib/modules/8821au.ko"
#define RTL_8812A_PATH			"/lib/modules/8812au.ko"

/* Wi-Fi Module name */
#define RTL_8188E_NAME			"8188eu"
#define RTL_8188C_NAME			"8192cu"
#define RTL_8821A_NAME			"8821au"
#define RTL_8812A_NAME			"8812au"

#define PROC_MODULE_FNAME		"/proc/modules"

#define __STAGE_MOD_IDLE		(0x00)
#define __STAGE_MOD_LOAD		(0x01)
#define __STAGE_MOD_WAIT		(0x02)
#define __STAGE_MOD_RUN			(0x03)

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_wlan_hostapd_t t_hostapd;
static netmgr_wlan_hostapd_t *ihost = &t_hostapd;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static int __create_hostapd_conf(const char *usr_id, char *usr_pw, int ch, int is_5g, int stealth)
{
	int ret = 0;
	FILE *fp;

	char ssid_buf[NETMGR_WLAN_SSID_MAX_SZ + 1] = {0,};
	char pass_buf[NETMGR_WLAN_PASSWD_MAX_SZ + 1] = {0,};

	/* Before starting the daemon, make sure its config file exists */
	ret = access(HOSTAPD_CONFIG, R_OK|W_OK);
    if ((ret == 0) || (errno == EACCES)) {
      	unlink(HOSTAPD_CONFIG);
    }

	fp = fopen(HOSTAPD_CONFIG, "wb");
	if (fp == NULL) {
		eprintf("couldn't create %s config file\n", HOSTAPD_CONFIG);
		return -1;
	}

	/* update ssid */
	memset(ssid_buf, 0, NETMGR_WLAN_SSID_MAX_SZ);
	if (strcmp(usr_id, "") == 0) {
		/* not supported */
		eprintf("wlan ssid is null!!\n");
		return -1;
	} else {
		strncpy(ssid_buf, usr_id, strlen(usr_id));
	}

	/* update password */
	memset(pass_buf, 0, NETMGR_WLAN_PASSWD_MAX_SZ);
	if (strcmp(usr_pw, "") == 0) {
		/* set default password */
		strncpy(pass_buf, "1234567890", 12);
	} else {
		strncpy(pass_buf, usr_pw, strlen(usr_pw));
	}

	/* write hostapd config */
	fprintf(fp, "##### hostapd configuration file ########################\n");
	fprintf(fp, "interface=wlan0\n");
	fprintf(fp, "ctrl_interface=/var/run/hostapd\n");
	fprintf(fp, "ssid=%s\n", ssid_buf);
	fprintf(fp, "channel=%d\n", ch);
	fprintf(fp, "wpa=2\n");
	fprintf(fp, "wpa_passphrase=%s\n", pass_buf);
	fprintf(fp, "\n");
	fprintf(fp, "##### Wi-Fi Protected Setup (WPS) #######################\n");
	fprintf(fp, "eap_server=0\n");
	fprintf(fp, "wps_state=2\n");
	fprintf(fp, "uuid=12345678-9abc-def0-1234-56789abcdef0\n");
	fprintf(fp, "device_name=RTL8192CU\n");
	fprintf(fp, "manufacturer=Realtek\n");
	fprintf(fp, "model_name=RTW_SOFTAP\n");
	fprintf(fp, "model_number=WLAN_CU\n");
	fprintf(fp, "serial_number=12345\n");
	fprintf(fp, "device_type=6-0050F204-1\n");
	fprintf(fp, "os_version=01020300\n");
	fprintf(fp, "config_methods=label display push_button keypad\n");
	fprintf(fp, "##### default configuration #############################\n");
	fprintf(fp, "driver=rtl871xdrv\n");
	fprintf(fp, "beacon_int=100\n");

	if (is_5g) {
		fprintf(fp, "hw_mode=a\n");
	} else {
		fprintf(fp, "hw_mode=g\n");
	}
	fprintf(fp, "ieee80211n=1\n"); //# 1->802.11n on 0->802.11n off
	fprintf(fp, "wme_enabled=1\n");
	fprintf(fp, "ht_capab=[SHORT-GI-20][SHORT-GI-40][HT40+]\n");
	fprintf(fp, "wpa_key_mgmt=WPA-PSK\n");
	fprintf(fp, "wpa_pairwise=CCMP\n");
	fprintf(fp, "max_num_sta=2\n");
	fprintf(fp, "wpa_group_rekey=86400\n");

	fflush(fp);
	fclose(fp);

	/* chmod is needed because open() didn't set permisions properly */
	chmod(HOSTAPD_CONFIG, 0660);

	return 0;
}

static int __wlan_get_module(const char *name)
{
    FILE *proc;

    char line[256] = {};

	if (name == NULL)
		return 0;

    if ((proc = fopen(PROC_MODULE_FNAME, "r")) == NULL) {
        eprintf("Could not open /proc/modules!!\n");
        return 0;
    }

    while ((fgets(line, sizeof(line), proc)) != NULL) {
        if (strncmp(line, name, strlen(name)) == 0) {
            fclose(proc);
            return 1;
        }
    }
    fclose(proc);

    return 0;
}

static void __wlan_insmod(const char *module_path)
{
	char buf[256];
	FILE *f = NULL;

	/* load driver */
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "/sbin/insmod %s", module_path);

	f = popen(buf, "r");
	if (f != NULL)
		pclose(f);
}

static int __wlan_wait_mod_active(void)
{
	int ret;
	
	ret = access("/sys/class/net/wlan0/operstate", R_OK);
	if ((ret == 0) || (errno == EACCES)) {
		return 0;
	} 
	return -1;
}

static int __wlan_hostapd_run(void)
{
	char buf[256] = {0,};
	int ret = 0;
	FILE *f;
	
	ret = __create_hostapd_conf(ihost->ssid, ihost->passwd, 
				ihost->channel, ihost->freq, ihost->stealth);
	if (ret < 0)
		return -1;
	
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "/usr/sbin/hostapd %s -B -P %s", HOSTAPD_CONFIG,
					HOSTAPD_PID_PATH);
	f = popen(buf, "r");
	if (f != NULL)
		pclose(f);
	
	/* ip setup */
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "/sbin/ifconfig wlan0 192.168.0.1 up");
	f = popen(buf, "r");
	if (f != NULL)
		pclose(f);
		
	/* dhcpd start */
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "/usr/sbin/udhcpd /etc/udhcpd.conf");
	f = popen(buf, "r");
	if (f != NULL)
		pclose(f);
	
	return 0;
}

static void __wlan_hostapd_stop(void)
{
	char buf[256] = {0,};
	int pid = 0;
	FILE *f;
	
    f = fopen(HOSTAPD_PID_PATH, "r");
    if (f == NULL) {
    	eprintf("couldn't open %s\n", HOSTAPD_PID_PATH);
    	return;
    }

    fscanf(f, "%d", &pid);
    fclose(f);
    unlink((const char *)HOSTAPD_PID_PATH);

    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);

	/* wifi-link down */
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "/sbin/ifconfig wlan0 down");
	f = popen(buf, "r");
	if (f != NULL)
		pclose(f);
	
	/* dhcpd stop */
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "killall udhcpd");
	f = popen(buf, "r");
	if (f != NULL)
		pclose(f);
}

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_wlan_hostapd_main(void *prm)
{
	app_thr_obj *tObj = &ihost->hObj;
	int exit = 0, cmd;
	char path[128] = {0,};
	char name[128] = {0,};
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT)
			break;
		
		/* START or STOP  명령을 수신하면 실행됨 */
		ihost->stage = __STAGE_MOD_LOAD;
		ihost->tmr_count = 0;
		
		while (!exit)
		{
			int vid = app_cfg->wlan_vid;
			int pid = app_cfg->wlan_pid;
			int st = 0;
			
			cmd = tObj->cmd;
        	if (cmd == APP_CMD_STOP) {
				/* TODO */
            	__wlan_hostapd_stop();
				break;
			}
			
			st = ihost->stage;
			switch (st) {
			case __STAGE_MOD_RUN:
				__wlan_hostapd_run();
				ihost->stage = __STAGE_MOD_IDLE;
				/* 현재 상태를 알려줘야 함 */
				break;
			
			case __STAGE_MOD_WAIT:
				if (__wlan_wait_mod_active() == 0) {
					ihost->stage = __STAGE_MOD_RUN;
				} else {
					/* timeout 계산 */
					ihost->tmr_count++;
					if (ihost->tmr_count >= 10) {
						/* fail */
						ihost->stage = __STAGE_MOD_IDLE;
						/* error 상태를 mainapp에 알려줘야 함 */
					} 
				}
				break;	
			
			case __STAGE_MOD_LOAD:
				/* kernel module 파일명을 확인 */
				if ((vid == RTL_8188E_VID) && (pid == RTL_8188E_PID)) {
					strcpy(path, RTL_8188E_PATH);
					strcpy(name, RTL_8188E_NAME);
				} else if ((vid == RTL_8188C_VID) && (pid == RTL_8188C_PID)) {
					strcpy(path, RTL_8188C_PATH);
					strcpy(name, RTL_8188C_NAME);
				} else if ((vid == RTL_8192C_VID) && (pid == RTL_8192C_PID)) {
					strcpy(path, RTL_8188C_PATH);
					strcpy(name, RTL_8188C_NAME);
				} else if ((vid == RTL_8821A_VID) && (pid == RTL_8821A_PID)) {
					strcpy(path, RTL_8821A_PATH);
					strcpy(name, RTL_8821A_NAME);
				} else {
					/* invalid wifi usb module */
					dprintf("Not Supported WiFi USB Module!!(%x, %x)\n", vid, pid);
					/* TODO */
					ihost->stage = __STAGE_MOD_IDLE;
					continue;
				}
				
				/* 이미 모듈이 loading된 상태이면 insmod 수행 안 함 */
				if (__wlan_get_module(name) == 0) 
				{
					__wlan_insmod(path);
				} else {
					/* unload driver TODO */
					//snprintf(buf, sizeof(buf), "/sbin/rmmod %s", module_name);
				}
				ihost->stage = __STAGE_MOD_WAIT;
				break;
			
			case __STAGE_MOD_IDLE:
			default:
				break;
			}
			
			delay_msecs(200);	
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
int netmgr_wlan_hostapd_init(void)
{
	app_thr_obj *tObj = &ihost->hObj;
	
	if (thread_create(tObj, THR_wlan_hostapd_main, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr wlan hostap thread\n");
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
int netmgr_wlan_hostapd_exit(void)
{
	app_thr_obj *tObj = &ihost->hObj;

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
int netmgr_wlan_hostapd_start(void)
{
	app_thr_obj *tObj = &ihost->hObj;
	char *databuf;
	netmgr_shm_request_info_t *info;
	
	/* shared memory로부터 ssid / password / channel등의 정보를 읽어온다 */
	databuf = (char *)(app_cfg->shm_buf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_shm_request_info_t *)databuf;
	
	memcpy(ihost->ssid, info->ssid, NETMGR_WLAN_SSID_MAX_SZ);
	memcpy(ihost->passwd, info->passwd, NETMGR_WLAN_PASSWD_MAX_SZ);
	ihost->stealth = info->stealth;
	ihost->freq = info->freq;
	ihost->channel = info->channel;
			
	/* delete usb scan object */
   	event_send(tObj, APP_CMD_START, 0, 0);
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_wlan_hostapd_stop(void)
{
	app_thr_obj *tObj = &ihost->hObj;
			
	/* delete usb scan object */
   	event_send(tObj, APP_CMD_STOP, 0, 0);
	
	return 0;
}
