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
#include <dirent.h>
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
#include "event_hub.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_SOFTAP_CYCLE			200		//# msec
#define TIME_SOFTAP_ACTIVE    		10000   //# 10sec
#define CNT_SOFTWAP_ACTIVE        	(TIME_SOFTAP_ACTIVE/TIME_SOFTAP_CYCLE)

#define HOSTAPD_PID_PATH			"/var/run/hostapd.pid"
#define HOSTAPD_CONFIG				"/etc/hostapd.conf"

#define __STAGE_SOFTAP_MOD_EXEC		(0x01)
#define __STAGE_SOFTAP_GET_STATUS	(0x02)
#define __STAGE_SOFTAP_ERROR_STOP	(0x04)

#define NETMGR_SOFTAP_DEVNAME		"wlan0"

typedef struct {
	app_thr_obj hObj; /* wlan client mode */
	
	char ssid[NETMGR_WLAN_SSID_MAX_SZ+1];      //32
	char passwd[NETMGR_WLAN_PASSWD_MAX_SZ+1];  //64
	
	int freq;        /* 0-> 2.4GHz 1-> 5GHz */
	int stealth;     /* Hidden SSID */
	int channel;     /* channel number (2.4GH ->6, 5GHz -> 36) */
	
	int stage;
	
} netmgr_wlan_hostapd_t;

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
	fprintf(fp, "ignore_broadcast_ssid=%d\n",stealth) ;

	fflush(fp);
	fclose(fp);

	/* chmod is needed because open() didn't set permisions properly */
	chmod(HOSTAPD_CONFIG, 0660);

	return 0;
}

static int __wlan_hostapd_run(void)
{
	char buf[256] = {0,};
	int ret = 0;
	
	ret = __create_hostapd_conf(ihost->ssid, ihost->passwd, 
				ihost->channel, ihost->freq, ihost->stealth);
	if (ret < 0)
		return -1;
	
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "/usr/sbin/hostapd %s -B -P %s", HOSTAPD_CONFIG,
					HOSTAPD_PID_PATH);
	system(buf);
	
	/* ip setup IP 변경될 경우 main에서 수신해야 함 */
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "/sbin/ifconfig %s 192.168.0.1 up", NETMGR_SOFTAP_DEVNAME);
	system(buf);
		
	/* dhcpd start */
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "/usr/sbin/udhcpd /etc/udhcpd.conf");
	system(buf);
	
	dprintf("Wi-Fi HOSTAPD Done!!\n");
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
	snprintf(buf, sizeof(buf), "/sbin/ifconfig %s down", NETMGR_SOFTAP_DEVNAME);
	system(buf);
	
	/* dhcpd stop */
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "killall udhcpd");
	system(buf);
}

static int __wlan_hostapd_is_active(const char *process_name)
{   
    DIR* pdir;
    struct dirent *pinfo;
    int is_live = 0;

    pdir = opendir("/proc");
    if (pdir == NULL)
    {
        eprintf("err: NO_DIR\n");
        return 0;
    }

    while (1)
    {
		FILE* fp = NULL;
        char buff[128]={0,};
        char path[128]={0,};
		
        pinfo = readdir(pdir);
        if (pinfo == NULL)
            break;
 
        if (pinfo->d_type != 4 || pinfo->d_name[0] == '.' || pinfo->d_name[0] > 57)
            continue;

        sprintf(path, "/proc/%s/status", pinfo->d_name);
        fp = fopen(path, "rt");
        if (fp)
        {
            fgets(buff, 128, fp);
            fclose(fp);
        
            if (strstr(buff, process_name))   
            {
                is_live = 1;
                break;
            }
        } else {
            dprintf(" %s inactive!!\n", process_name);
        }
    }

    closedir(pdir);

    return is_live;
}

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_wlan_hostapd_main(void *prm)
{
	app_thr_obj *tObj = &ihost->hObj;
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
		
		while (!quit)
		{
			int st = 0;
			
			cmd = tObj->cmd;
        	if (cmd == APP_CMD_STOP) {
            	netmgr_event_hub_link_status(NETMGR_DEV_TYPE_WIFI, NETMGR_DEV_INACTIVE);
				__wlan_hostapd_stop();
				quit = 1;
				continue;
			}
			
			st = ihost->stage;
			switch (st) {
			case __STAGE_SOFTAP_GET_STATUS:
				/* TODO */
				st = __wlan_hostapd_is_active("hostapd");
				if (st > 0)
					ihost->stage = __STAGE_SOFTAP_GET_STATUS;
				else 
					ihost->stage = __STAGE_SOFTAP_ERROR_STOP;
				break;
			
			case __STAGE_SOFTAP_MOD_EXEC:
				__wlan_hostapd_run();
				ihost->stage = __STAGE_SOFTAP_GET_STATUS;
				/* 현재 상태를 알려줘야 함 */
				netmgr_event_hub_link_status(NETMGR_DEV_TYPE_WIFI, NETMGR_DEV_ACTIVE);
				break;
			
			case __STAGE_SOFTAP_ERROR_STOP:
				/* error 상태를 mainapp에 알려줘야 함 */
				quit = 1; /* exit loop */
				netmgr_event_hub_link_status(NETMGR_DEV_TYPE_WIFI, NETMGR_DEV_ERROR);
				__wlan_hostapd_stop();
				break;
			}
			
			delay_msecs(TIME_SOFTAP_CYCLE);	
		}
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
int netmgr_wlan_hostapd_init(void)
{
	app_thr_obj *tObj = &ihost->hObj;
	
	if (thread_create(tObj, THR_wlan_hostapd_main, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr wlan hostap thread\n");
		return EFAIL;
    }
	
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
	netmgr_iw_hostapd_req_info_t *info;
	
	/* shared memory로부터 ssid / password / channel등의 정보를 읽어온다 */
	databuf = (char *)(app_cfg->shm_buf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_iw_hostapd_req_info_t *)databuf;
	
	memcpy(ihost->ssid, info->ssid, NETMGR_WLAN_SSID_MAX_SZ);
	memcpy(ihost->passwd, info->passwd, NETMGR_WLAN_PASSWD_MAX_SZ);
	ihost->stealth = info->stealth;
	ihost->freq = info->freq;
	ihost->channel = info->channel;
	
	/* START or STOP  명령을 수신하면 실행됨 */
	ihost->stage = __STAGE_SOFTAP_MOD_EXEC;
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
