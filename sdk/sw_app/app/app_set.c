/******************************************************************************
 * fitt360 Black Box Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_set.c
 * @brief
 * @author	hwjun
 * @section	MODIFY history
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <osa_file.h>
#include <glob.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "dev_common.h"
#include "dev_micom.h"

#include "app_comm.h"
#include "app_version.h"
#include "app_main.h"
#include "app_set.h"
#include "app_encrypt.h"
#include "app_decrypt.h"
#include "app_base64.h"
#include "app_mcu.h"
#include "app_rec.h"
#include "app_file.h"
#include "app_ctrl.h"
#include "js_settings.h"

#include "app_voip.h"
#include "app_sslvpn.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/**
 * @brief 기본값 검사 매크로
 * @param orless : 이하
 * @param above:   초과
 * @param default: 기본값
 * @param prm:     검사할 변수
 */
#define CHECK_PARAM_MIN_MAX(orless, above, default, prm) { if ( prm <= orless || prm > above ) prm = default; }
#define MAX_HTTPS_MODE       2
#define MAX_ONVIF_ENABLE     1
#define MAX_HTTP_ENABLE      1
#define MAX_HTTPS_ENABLE     1
#define DEFAULT_HTTPS_MODE   1
#define DEFAULT_ONVIF_ENABLE 1
#define DEFAULT_HTTP_ENABLE  0
#define DEFAULT_HTTPS_ENABLE 1

typedef enum{
	CFG_NAND=0,
	CFG_MMC,
	CFG_MAX
} app_cfg_e;

const char* const RTSP_DEFAULT_ID   = "admin";
const char* const RTSP_DEFAULT_PW   = "admin001!";
const char* const P2P_DEFAULT_ID    = "linkflow";
const char* const P2P_DEFAULT_PW    = "12345678";
const char* const WEB_DEFAULT_ID    = "admin";
const char* const WEB_DEFAULT_PW    = "admin001!";
const char* const ONVIF_DEFAULT_ID  = "admin";
const char* const ONVIF_DEFAULT_PW  = "admin001!";

/*
 * Nexx360/Fitt360 --> 15,10,5 Fps
 * Nexxone ----------> 30,15,5
 */
static char *cfg_dir[CFG_MAX] = {CFG_DIR_NAND, CFG_DIR_MMC};
static char cMacAddr[18]; // Server's MAC address

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
app_set_t *app_set=NULL;
app_set_t app_sys_set;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static size_t get_cfg_size (const char *file_name)
{
    struct stat sb;

    if (stat(file_name, &sb) != 0) {
        TRACE_INFO("Failed the CFG file size.\n");
        return 0;
    }

    return sb.st_size;
}

static void set_uid()
{
    char uid[MAX_CHAR_32] = {0, };

    if(dev_board_uid_read(uid, MAX_CHAR_16) == 0)
    {
	    if(!strncmp(uid, "LFS", 3))
	    {
            sprintf(app_set->sys_info.uid, "%s", uid); 
			LOGD("P2P UID Registration succeed!!");
	        sprintf(app_set->voip.userid, "%d%d%s", 'A', 1, &uid[12]);
			LOGD("VOIP User ID Registration succeed!!");
			if(!strcmp(app_set->voip.peerid, "")) 
                sprintf(app_set->voip.peerid, "%d%d%s", 'V', 1, &uid[12]);

        }
	}	
}

static void char_memset(void)
{
    int i = 0 ;

    for (i = 0; i < MODEL_CH_NUM + 1; i++)
    {
        app_set->ch[i].resol = CFG_INVALID ;
        app_set->ch[i].framerate = CFG_INVALID ;
        app_set->ch[i].quality = CFG_INVALID ;
        app_set->ch[i].rate_ctrl = CFG_INVALID ;
        app_set->ch[i].motion = CFG_INVALID ;
        app_set->ch[i].gop = CFG_INVALID ;
        memset(app_set->ch[i].reserved, CFG_INVALID, 36) ;
    }

    app_set->wd.gsn = CFG_INVALID ;
    memset(app_set->wd.reserved, CFG_INVALID, 40);

    app_set->net_info.type = CFG_INVALID ;
    memset(app_set->net_info.wlan_ipaddr, CHAR_MEMSET, MAX_CHAR_16) ;
    memset(app_set->net_info.wlan_netmask, CHAR_MEMSET, MAX_CHAR_16) ;
    memset(app_set->net_info.wlan_gateway, CHAR_MEMSET, MAX_CHAR_16) ;

    memset(app_set->net_info.eth_ipaddr, CHAR_MEMSET, MAX_CHAR_16) ;
    memset(app_set->net_info.eth_netmask, CHAR_MEMSET, MAX_CHAR_16) ;
    memset(app_set->net_info.eth_gateway, CHAR_MEMSET, MAX_CHAR_16) ;

    memset(app_set->net_info.dns_server1, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->net_info.dns_server2, CHAR_MEMSET, MAX_CHAR_16);

    app_set->net_info.http_port = CFG_INVALID ;
    app_set->net_info.https_port = CFG_INVALID ;
    app_set->net_info.rtsp_port = CFG_INVALID ;
    app_set->net_info.onvif_port = CFG_INVALID ;

    memset(app_set->net_info.rtsp_name, CHAR_MEMSET, 6);
    app_set->net_info.wtype = CFG_INVALID ;
    app_set->net_info.http_enable  = CFG_INVALID;
    app_set->net_info.https_enable = CFG_INVALID;
    app_set->net_info.rtsp_enable  = CFG_INVALID;
    app_set->net_info.enable_onvif = CFG_INVALID;
    app_set->net_info.dnsFromDHCP  = CFG_INVALID;
    app_set->net_info.ntpFromDHCP  = CFG_INVALID;
    app_set->net_info.https_mode   = CFG_INVALID;

	memset(app_set->net_info.ssc_C,  CHAR_MEMSET, MAX_CHAR_64);
	memset(app_set->net_info.ssc_ST, CHAR_MEMSET, MAX_CHAR_64);
	memset(app_set->net_info.ssc_L,  CHAR_MEMSET, MAX_CHAR_64);
	memset(app_set->net_info.ssc_O,  CHAR_MEMSET, MAX_CHAR_64);
	memset(app_set->net_info.ssc_OU, CHAR_MEMSET, MAX_CHAR_64);
	memset(app_set->net_info.ssc_CN, CHAR_MEMSET, MAX_CHAR_64);
	memset(app_set->net_info.ssc_Email, CHAR_MEMSET, MAX_CHAR_64);
	memset(app_set->net_info.cert_name, CHAR_MEMSET, MAX_CHAR_64);

    memset(app_set->net_info.reserved, CFG_INVALID, 73);
    
	//# Server information
    app_set->srv_info.port = CFG_INVALID ;
    memset(app_set->srv_info.ipaddr, CHAR_MEMSET, SERVER_URL_SIZE);
    app_set->srv_info.ON_OFF = CFG_INVALID ;
    memset(app_set->srv_info.reserved, CFG_INVALID, 126) ;

	//# FTP information
    app_set->ftp_info.port = CFG_INVALID ;
    memset(app_set->ftp_info.ipaddr, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->ftp_info.id, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->ftp_info.pwd, CHAR_MEMSET, MAX_CHAR_16);

    app_set->ftp_info.ON_OFF = OFF ;
    app_set->ftp_info.file_type = OFF ; // OFF NORMAL, ON Event file

    memset(app_set->ftp_info.reserved, CFG_INVALID, 126);

	//# FOTA information
    app_set->fota_info.port = CFG_INVALID ;
    app_set->fota_info.svr_info = CFG_INVALID ;  // manual input or use ftp ipaddress
    memset(app_set->fota_info.ipaddr, CHAR_MEMSET, MAX_CHAR_32);
    memset(app_set->fota_info.id, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->fota_info.pwd, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->fota_info.confname, CHAR_MEMSET, MAX_CHAR_32);
    app_set->fota_info.ON_OFF = OFF ;
    app_set->fota_info.type = OFF ; // OFF FTP, ON SFTP
    memset(app_set->fota_info.reserved, CFG_INVALID, 64);

	//# Wifi AP information
    app_set->wifiap.en_key = CFG_INVALID ;
    memset(app_set->wifiap.ssid, CHAR_MEMSET, MAX_CHAR_32 + 3) ;
    memset(app_set->wifiap.pwd, CHAR_INVALID, MAX_CHAR_64 + 1) ;
	app_set->wifiap.stealth = CFG_INVALID ;
    memset(app_set->wifiap.reserved, CFG_INVALID, 72) ;
    
	//# Wifilist information
	for(i = 0; i < WIFIAP_CNT; i++)
	{
		app_set->wifilist[i].en_key = CFG_INVALID ;
		memset(app_set->wifilist[i].ssid, CHAR_MEMSET, MAX_CHAR_32 + 3) ;
		memset(app_set->wifilist[i].pwd, CHAR_MEMSET, MAX_CHAR_64 + 1) ;
		app_set->wifilist[i].stealth = CFG_INVALID ;
		memset(app_set->wifilist[i].reserved, CFG_INVALID, 72) ;
    }

	//# Versions
    memset(app_set->sys_info.fw_ver, CHAR_MEMSET, MAX_CHAR_32);
    memset(app_set->sys_info.hw_ver, CHAR_MEMSET, MAX_CHAR_32);
    memset(app_set->sys_info.deviceId, CHAR_MEMSET, MAX_CHAR_32);
    app_set->sys_info.osd_set = CFG_INVALID; 
    app_set->sys_info.beep_sound = CFG_INVALID; 
    app_set->sys_info.aes_encryption = CFG_INVALID; 
    memset(app_set->sys_info.aes_key, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->sys_info.aes_iv, CHAR_MEMSET, MAX_CHAR_16);
    app_set->sys_info.P2P_ON_OFF = CFG_INVALID; 
    memset(app_set->sys_info.p2p_id, CHAR_MEMSET, MAX_CHAR_32); 
    memset(app_set->sys_info.p2p_passwd, CHAR_MEMSET, MAX_CHAR_32); 
    memset(app_set->sys_info.uid, CHAR_MEMSET, MAX_CHAR_32); 

    //# DDNS 
    app_set->ddns_info.ON_OFF = CFG_INVALID ; 
    memset(app_set->ddns_info.userId, CHAR_MEMSET, MAX_CHAR_32);
    memset(app_set->ddns_info.passwd, CHAR_MEMSET, MAX_CHAR_32);
    memset(app_set->ddns_info.serveraddr, CHAR_MEMSET, MAX_CHAR_64);
    memset(app_set->ddns_info.hostname, CHAR_MEMSET, MAX_CHAR_32);
    app_set->ddns_info.interval = CFG_INVALID ;

    // # Time
    app_set->time_info.time_zone = CFG_INVALID ;
    memset(app_set->time_info.time_server, CHAR_MEMSET, MAX_CHAR_32) ;
    app_set->time_info.daylight_saving = CFG_INVALID ;
    app_set->time_info.timesync_type = CFG_INVALID ;
    memset(app_set->time_info.time_zone_abbr, CHAR_MEMSET, 6) ;
    memset(app_set->time_info.reserved, CFG_INVALID, 24) ;
    
    app_set->account_info.ON_OFF = CFG_INVALID ; 
    app_set->account_info.enctype = CFG_INVALID ; 
    memset(app_set->account_info.rtsp_userid, CHAR_MEMSET, MAX_CHAR_32) ;
    memset(app_set->account_info.rtsp_passwd, CHAR_MEMSET, MAX_CHAR_32) ;

    app_set->account_info.webuser.authtype = 1 ;
	app_set->account_info.webuser.lv = 0 ;
	memset(app_set->account_info.webuser.id, CHAR_MEMSET, MAX_CHAR_32) ; 
	memset(app_set->account_info.webuser.pw, CHAR_MEMSET, MAX_CHAR_32) ;

	app_set->account_info.onvif.lv = 0 ;
	memset(app_set->account_info.onvif.id, CHAR_MEMSET, MAX_CHAR_32) ; 
	memset(app_set->account_info.onvif.pw, CHAR_MEMSET, MAX_CHAR_32) ;

    memset(app_set->account_info.reserved, CFG_INVALID, 121) ; // WOW, This is a super TRAP...
    app_set->multi_ap.ON_OFF = CFG_INVALID ;

	// VOIP size : 66 
    app_set->voip.port = CFG_INVALID ;
    memset(app_set->voip.ipaddr, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->voip.userid, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->voip.passwd, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->voip.peerid, CHAR_MEMSET, MAX_CHAR_16);
	app_set->voip.use_stun = 0 ;
	app_set->voip.ON_OFF = CFG_INVALID ;
    memset(app_set->voip.reserved, CFG_INVALID, 38) ;

	app_set->rtmp.ON_OFF = CFG_INVALID ;
	app_set->rtmp.USE_URL = CFG_INVALID ;
	app_set->rtmp.port = CFG_INVALID ;
	memset(app_set->rtmp.ipaddr, CHAR_MEMSET, MAX_CHAR_16) ;
	memset(app_set->rtmp.FULL_URL, CHAR_MEMSET, MAX_CHAR_64) ;

	app_set->sslvpn_info.ON_OFF = CFG_INVALID ;
	app_set->sslvpn_info.vendor = CFG_INVALID ;
	memset(app_set->sslvpn_info.vpn_id, CHAR_MEMSET, MAX_CHAR_32) ;
	app_set->sslvpn_info.heartbeat_interval = CFG_INVALID ;
	app_set->sslvpn_info.heartbeat_threshold = CFG_INVALID ;
    app_set->sslvpn_info.protocol = CFG_INVALID ;
	app_set->sslvpn_info.port = CFG_INVALID ;
	app_set->sslvpn_info.queue = CFG_INVALID ;
	memset(app_set->sslvpn_info.key, CHAR_MEMSET, MAX_CHAR_16) ;
	app_set->sslvpn_info.encrypt_type = CFG_INVALID ;
	memset(app_set->sslvpn_info.ipaddr, CHAR_MEMSET, MAX_CHAR_32) ;
	app_set->sslvpn_info.NI = CFG_INVALID ;

//	memset(app_set->rtmp.userid, CHAR_MEMSET, MAX_CHAR_16) ;
//	memset(app_set->rtmp.passwd, CHAR_MEMSET, MAX_CHAR_16) ;

//    memset(app_set->reserved, CFG_INVALID, 90) ;
}

int GetSvrMacAddress(char *mac_address)
{
    FILE *fd;
    int readlen;

    fd = fopen("/sys/class/net/eth0/address","r");
    if(fd == NULL)
        return -1;

    if((readlen = fread(mac_address, sizeof(char), 17, fd)) > 0)
    {
        TRACE_INFO("Mac:%s len:%d\n", mac_address, readlen) ;
    }

    fclose(fd);

    return 0;
}

int DefaultGetMac(char *mac_address)
{
    int cur_idx = 0 ;

    bzero( (void *)&cMacAddr[0], sizeof(cMacAddr) );
    if ( !GetSvrMacAddress(cMacAddr) )
    {
        char *ptr = strtok(cMacAddr, ":") ;
        while(ptr != NULL)
        {
            sprintf(&mac_address[cur_idx], "%s", ptr) ;
            cur_idx = strlen(mac_address) ;
            ptr = strtok(NULL, ":") ;
        }
        return 0;
    }
    else
    {
        TRACE_ERR( "Fatal error: Failed to get local host's MAC address\n" );
    }
    return -1 ;
}

int show_all_cfg(app_set_t* pset)
{
	int i;
	for(i=0;i<MODEL_CH_NUM+1;i++){
		printf("pset->ch[%d].resol     = %d\n", i, pset->ch[i].resol    );
		printf("pset->ch[%d].framerate = %d\n", i, pset->ch[i].framerate);
		printf("pset->ch[%d].quality   = %d\n", i, pset->ch[i].quality  );
		printf("pset->ch[%d].rate_ctrl = %d\n", i, pset->ch[i].rate_ctrl);
		printf("pset->ch[%d].motion    = %d\n", i, pset->ch[i].motion   );
		printf("pset->ch[%d].gop       = %d\n", i, pset->ch[i].gop      );
		printf("pset->ch[%d].reserved  = %s\n", i, pset->ch[i].reserved );
	}
	printf("\n");

	printf("pset->wd.gsn  = %d\n", pset->wd.gsn);
	printf("\n");

    printf("pset->net_info.type         = %d\n", pset->net_info.type        );
    printf("pset->net_info.wlan_ipaddr  = %s\n", pset->net_info.wlan_ipaddr );
    printf("pset->net_info.wlan_netmask = %s\n", pset->net_info.wlan_netmask);
    printf("pset->net_info.wlan_gateway = %s\n", pset->net_info.wlan_gateway);
    printf("pset->net_info.eth_ipaddr   = %s\n", pset->net_info.eth_ipaddr  );
    printf("pset->net_info.eth_netmask  = %s\n", pset->net_info.eth_netmask );
    printf("pset->net_info.eth_gateway  = %s\n", pset->net_info.eth_gateway );
    printf("pset->net_info.dns_server1  = %s\n", pset->net_info.dns_server1 );
    printf("pset->net_info.dns_server2  = %s\n", pset->net_info.dns_server2 );
    printf("pset->net_info.http_port    = %d\n", pset->net_info.http_port   );
    printf("pset->net_info.https_port   = %d\n", pset->net_info.https_port  );
    printf("pset->net_info.rtsp_port    = %d\n", pset->net_info.rtsp_port   );
    printf("pset->net_info.onvif_port   = %d\n", pset->net_info.onvif_port  );
    printf("pset->net_info.rtsp_name[6] = %s\n", pset->net_info.rtsp_name   );
    printf("pset->net_info.wtype        = %d\n", pset->net_info.wtype       );
    printf("pset->net_info.http_enable  = %d\n", pset->net_info.http_enable );
    printf("pset->net_info.https_mode   = %d\n", pset->net_info.https_mode  );
    printf("pset->net_info.https_enable = %d\n", pset->net_info.https_enable);
    printf("pset->net_info.rtsp_enable  = %d\n", pset->net_info.rtsp_enable );
    printf("pset->net_info.enable_onvif = %d\n", pset->net_info.enable_onvif);
    printf("pset->net_info.dnsFromDHCP  = %d\n", pset->net_info.dnsFromDHCP );
    printf("pset->net_info.ntpFromDHCP  = %d\n", pset->net_info.ntpFromDHCP );
	printf("\n");

    printf("pset->srv_info.port   = %d\n", pset->srv_info.port  );
    printf("pset->srv_info.ipaddr = %s\n", pset->srv_info.ipaddr);
    printf("pset->srv_info.ON_OFF = %d\n", pset->srv_info.ON_OFF);
	printf("\n");

    printf("pset->ftp_info.port   = %d\n", pset->ftp_info.port        );
    printf("pset->ftp_info.ipaddr = %s\n", pset->ftp_info.ipaddr);
    printf("pset->ftp_info.id     = %s\n", pset->ftp_info.id);
    printf("pset->ftp_info.pwd    = %s\n", pset->ftp_info.pwd);
    printf("pset->ftp_info.ON_OFF = %d\n", pset->ftp_info.ON_OFF);
    printf("pset->ftp_info.file_type = %d\n", pset->ftp_info.file_type);
	printf("\n");

    printf("pset->fota_info.port   = %d\n", pset->fota_info.port        );
    printf("pset->fota_info.svr_info   = %d\n", pset->fota_info.svr_info        );
    printf("pset->fota_info.ipaddr = %s\n", pset->fota_info.ipaddr);
    printf("pset->fota_info.id     = %s\n", pset->fota_info.id);
    printf("pset->fota_info.pwd    = %s\n", pset->fota_info.pwd);
    printf("pset->fota_info.confname = %s\n", pset->fota_info.confname);
    printf("pset->fota_info.ON_OFF = %d\n", pset->fota_info.ON_OFF);
	printf("\n");

    printf("pset->wifiap.en_key = %d\n", pset->wifiap.en_key);
    printf("pset->wifiap.ssid   = %s\n", pset->wifiap.ssid);
    printf("pset->wifiap.pwd    = %s\n", pset->wifiap.pwd);
    printf("pset->wifiap.stealth    = %d\n", pset->wifiap.stealth);
	printf("\n");

	for(i = 0 ; i < WIFIAP_CNT; i++)
	{
		printf("pset->wifilist[%d].en_key = %d\n",i,  pset->wifilist[i].en_key);
		printf("pset->wifilist[%d].ssid   = %s\n",i, pset->wifilist[i].ssid);
		printf("pset->wifilist[%d].pwd    = %s\n",i, pset->wifilist[i].pwd);
		printf("pset->wifilist[%d].stealth    = %d\n",i, pset->wifilist[i].stealth);
		printf("\n");
	}

    printf("pset->sys_info.fw_ver   = %s\n", pset->sys_info.fw_ver);
    printf("pset->sys_info.hw_ver   = %s\n", pset->sys_info.hw_ver);
    printf("pset->sys_info.deviceId = %s\n", pset->sys_info.deviceId); 
    printf("pset->sys_info.osd_set  = %d\n", pset->sys_info.osd_set); 
    printf("pset->sys_info.beep_sound  = %d\n", pset->sys_info.beep_sound); 
    printf("pset->sys_info.aes_encryption  = %d\n", pset->sys_info.aes_encryption); 
    printf("pset->sys_info.aes_key = %s\n", pset->sys_info.aes_key); 
    printf("pset->sys_info.aes_iv = %s\n", pset->sys_info.aes_iv); 
    printf("pset->sys_info.P2P_ON_OFF  = %d\n", pset->sys_info.P2P_ON_OFF); 
    
    printf("pset->sys_info.p2p_id  = %s\n", pset->sys_info.p2p_id); 
    printf("pset->sys_info.p2p_passwd  = %s\n", pset->sys_info.p2p_passwd); 
    printf("pset->sys_info.uid  = %s\n", pset->sys_info.uid); 

	printf("\n");

	printf("pset->stm_info.enable_audio = %d\n",pset->stm_info.enable_audio) ;
	printf("\n");

	printf("pset->rec_info.period_idx = %d\n", pset->rec_info.period_idx);
	printf("pset->rec_info.overwrite  = %d\n", pset->rec_info.overwrite );
    printf("pset->rec_info.auto_rec = %d\n",pset->rec_info.auto_rec) ;
	printf("pset->rec_info.pre_rec = %d\n",pset->rec_info.pre_rec) ;
	printf("pset->rec_info.audio_rec = %d\n",pset->rec_info.audio_rec) ;
	printf("\n");

    printf("pset->ddns_info.ON_OFF     = %d\n", pset->ddns_info.ON_OFF);
    printf("pset->ddns_info.userId     = %s\n", pset->ddns_info.userId); 
    printf("pset->ddns_info.passwd     = %s\n", pset->ddns_info.passwd);
    printf("pset->ddns_info.serveraddr = %s\n", pset->ddns_info.serveraddr);
    printf("pset->ddns_info.hostname   = %s\n", pset->ddns_info.hostname);
    printf("pset->ddns_info.interval   = %d\n", pset->ddns_info.interval);
	printf("\n");

    printf("pset->time_info.time_zone       = %d\n", pset->time_info.time_zone);
    printf("pset->time_info.time_server     = %s\n", pset->time_info.time_server);
    printf("pset->time_info.daylight_saving = %d\n", pset->time_info.daylight_saving);
    printf("pset->time_info.timesync_type   = %d\n", pset->time_info.timesync_type);
    printf("pset->time_info.time_zone_abbr  = %s\n", pset->time_info.time_zone_abbr);
	printf("\n");

    printf("pset->account_info.ON_OFF = %d\n", pset->account_info.ON_OFF) ;
    printf("pset->account_info.enctype = %d\n", pset->account_info.enctype) ;
    printf("pset->account_info.rtsp_userid = %s\n", pset->account_info.rtsp_userid) ;
    printf("pset->account_info.rtsp_passwd = %s\n", pset->account_info.rtsp_passwd) ;

    printf("pset->account_info.webuser.authtype = %d\n", pset->account_info.webuser.authtype) ;
    printf("pset->account_info.webuser.lv = %d\n", pset->account_info.webuser.lv) ;
    printf("pset->account_info.webuser.id = %s\n", pset->account_info.webuser.id) ;
    printf("pset->account_info.webuser.pw = %s\n", pset->account_info.webuser.pw) ;

    printf("pset->account_info.onvif.lv = %d\n", pset->account_info.onvif.lv) ;
    printf("pset->account_info.onvif.id = %s\n", pset->account_info.onvif.id) ;
    printf("pset->account_info.onvif.pw = %s\n", pset->account_info.onvif.pw) ;
    printf("pset->multi_ap.ON_OFF = %d\n", pset->multi_ap.ON_OFF) ;

    printf("pset->voip.ipaddr = %s\n", pset->voip.ipaddr);
    printf("pset->voip.port   = %d\n", pset->voip.port);
    printf("pset->voip.userid = %s\n", pset->voip.userid);
    printf("pset->voip.passwd = %s\n", pset->voip.passwd);
    printf("pset->voip.peerid = %s\n", pset->voip.peerid);
    printf("pset->voip.use_stun = %d\n", pset->voip.use_stun);
    printf("pset->voip.ON_OFF = %d\n", pset->voip.ON_OFF);

    printf("pset->rtmp.ON_OFF = %d\n", pset->rtmp.ON_OFF);
    printf("pset->rtmp.USE_URL    = %d\n", pset->rtmp.USE_URL);
    printf("pset->rtmp.ipaddr = %s\n", pset->rtmp.ipaddr);
    printf("pset->rtmp.FULL_URL = %s\n", pset->rtmp.FULL_URL);
    printf("pset->rtmp.port   = %d\n", pset->rtmp.port);

	printf("pset->sslvpn_info.ON_OFF = %d\n", pset->sslvpn_info.ON_OFF) ;
	printf("pset->sslvpn_info.vendor = %d\n", pset->sslvpn_info.vendor) ;
	printf("pset->sslvpn_info.vpn_id = %s\n", pset->sslvpn_info.vpn_id) ;
	printf("pset->sslvpn_info.heartbeat_interval = %d\n",pset->sslvpn_info.heartbeat_interval) ;
	printf("pset->sslvpn_info.heartbeat_threshold = %d\n", pset->sslvpn_info.heartbeat_threshold) ;
    printf("pset->sslvpn_info.protocol = %d\n", pset->sslvpn_info.protocol) ;
	printf("pset->sslvpn_info.port = %d\n", pset->sslvpn_info.port) ;
	printf("pset->sslvpn_info.queue = %d\n", pset->sslvpn_info.queue) ;
	printf("pset->sslvpn_info.key = %s\n", pset->sslvpn_info.key) ;
	printf("pset->sslvpn_info.encrypt_type = %d\n", pset->sslvpn_info.encrypt_type) ;
	printf("pset->sslvpn_info.ipaddr = %s\n", pset->sslvpn_info.ipaddr) ;
	printf("pset->sslvpn_info.NI = %d\n", pset->sslvpn_info.NI) ;

//    printf("pset->rtmp.userid = %s\n", pset->rtmp.userid);
//    printf("pset->rtmp.passwd = %s\n", pset->rtmp.passwd);

	printf("\n");

    return 0;
}

static int check_module_version(char *SSHV, char *SSLV, char *WEBV)
{
	char cmd[64] = {0,}, buf[64] = {0,} ;
	int cnt = 0, ret = 0 ;

	FILE *fp = NULL, *fd = NULL;

	char *ptr = NULL ;

	sprintf(cmd, "ssh -V > /mmc/module.ver 2>&1") ;
	fp = popen(cmd, "r") ;
	pclose(fp) ;
	
	sprintf(cmd, "lighttpd -v cat >> /mmc/module.ver 2>&1") ;
	fp = popen(cmd, "r") ;
	pclose(fp) ;

	while(1)
	{
		if(access("/mmc/module.ver", F_OK) !=0)
		{
			if(cnt == 3)
			{
				break ;
			}
			cnt += 1 ;
			OSA_waitMsecs(500) ;
		}
		else
		{
			if((fd = fopen("/mmc/module.ver", "r")) != NULL)
			{
				int readsize = fread(buf, sizeof(buf), 1, fd) ;
				if(readsize > 0)
				{
					ptr = strstr(buf, "OpenSSH") ;
					{
						strncpy(SSHV, &ptr[8], 5) ;
						printf("SSHV = %s\n",SSHV) ;
					}
					ptr = strstr(buf, "OpenSSL") ;
					{
						strncpy(SSLV, &ptr[8], 6) ;
						printf("SSLV = %s\n",SSLV) ;
					}
					ptr = strstr(buf, "lighttpd") ;
					{
						strncpy(WEBV, &ptr[9], 12) ;
						printf("WEBV = %s\n",WEBV) ;
					}
					ret = TRUE ;
					break ;
				}
			}
		}
	}
	return ret ;
}

void Create_random_string(char *outdata, int length)
{
	char str_list[MAX_CHAR_32 + 1] = {0, };
	char str_outlist[MAX_CHAR_32 + 1] = {0, };
	char temp = 0;
	int sub_i, i = 0 ;
	int addr = 0 ;
	int in_idx = 0, out_idx = 0 ;

	srand((unsigned int)time(NULL)) ;
	for(sub_i = 0 ; sub_i < length; sub_i++)
	{
	    str_list[sub_i] = 'a' + rand() % 26 ;
	}
	str_list[sub_i] = 0 ;
    sprintf(outdata, "%s", str_list) ;
    
	return ;
}

static void cfg_param_check_nexx(app_set_t *pset)
{
    char enc_ID[32] = {0, } ;
    char enc_Passwd[32] = {0, } ;
    char MacAddr[12]  ;
    char compbuff[32];
    char uid[MAX_CHAR_32] = {0, };
	int ich=0, channels = 0, i = 0, retval = 0;
	
	channels = MODEL_CH_NUM+1;
	//# Encoding cfg per channel
	for(ich = 0; ich < channels; ich++)
	{
	    if(ich < MODEL_CH_NUM)
	        pset->ch[ich].resol  = RESOL_720P;

		pset->ch[ich].motion = OFF;

		if(pset->ch[ich].gop > DEFAULT_FPS || pset->ch[ich].gop <= 0)
            pset->ch[ich].gop = DEFAULT_FPS ;

		if(pset->ch[ich].framerate <= 0  || pset->ch[ich].framerate > DEFAULT_FPS)
		{
			if(ich == MODEL_CH_NUM)
			{
#if defined(NEXXONE) || defined(NEXXB_ONE)
		        pset->ch[ich].framerate	= DEFAULT_FPS/2;
#else
		        pset->ch[ich].framerate	= DEFAULT_FPS;
#endif
			}
			else
		        pset->ch[ich].framerate	= DEFAULT_FPS;
        }

		if(ich == MODEL_CH_NUM) // streaming channel 1Mbps
		{
			if (pset->ch[ich].quality < MIN_STM_BITRATE || pset->ch[ich].quality > MAX_STM_BITRATE) {
				pset->ch[ich].quality = DEFAULT_STM_QUALITY;
			}
		}
		else // recording channels 4Mbps
		{
			if (pset->ch[ich].quality < MIN_REC_BITRATE || pset->ch[ich].quality > MAX_REC_BITRATE) {
				pset->ch[ich].quality = DEFAULT_REC_QUALITY;
			}
		}
		if(pset->ch[ich].rate_ctrl	!= RATE_CTRL_VBR && pset->ch[ich].rate_ctrl	!= RATE_CTRL_CBR)
		    pset->ch[ich].rate_ctrl	= RATE_CTRL_VBR;

        if(ich == MODEL_CH_NUM)
        {
	     	//# streaming channel...
#if defined(NEXXONE)|| defined(NEXXB_ONE)
            if(pset->ch[ich].resol < RESOL_480P || pset->ch[ich].resol >= RESOL_1080P)
	            pset->ch[ich].resol= RESOL_720P;
#else
            if(pset->ch[ich].resol < RESOL_480P || pset->ch[ich].resol > RESOL_1080P)
	            pset->ch[ich].resol= RESOL_720P;
#endif
        }
	}

	//# Watchdog...
	if(pset->wd.gsn < GSN_IDX_01 || pset->wd.gsn >= GSN_IDX_MAX)
		pset->wd.gsn = GSN_IDX_03;

	//# Network information for device
    if(((int)pset->net_info.wlan_ipaddr[0]) == CHAR_INVALID)
		strcpy(pset->net_info.wlan_ipaddr, "192.168.0.252");

    if(((int)pset->net_info.wlan_netmask[0]) == CHAR_INVALID)
		strcpy(pset->net_info.wlan_netmask, "255.255.0.0");

    if(((int)pset->net_info.wlan_gateway[0]) == CHAR_INVALID)
		strcpy(pset->net_info.wlan_gateway, "192.168.0.1");

    if(((int)pset->net_info.eth_ipaddr[0]) == CHAR_INVALID)
		strcpy(pset->net_info.eth_ipaddr, "192.168.1.252");

    if(((int)pset->net_info.eth_netmask[0]) == CHAR_INVALID)
		strcpy(pset->net_info.eth_netmask, "255.255.0.0");

    if(((int)pset->net_info.eth_gateway[0]) == CHAR_INVALID)
		strcpy(pset->net_info.eth_gateway, "192.168.1.1");

    if(((int)pset->net_info.dns_server1[0]) == CHAR_INVALID || ((int)pset->net_info.dns_server1[0]) == 0)
		strcpy(pset->net_info.dns_server1, "8.8.8.8"); // google dns server address

    if(((int)pset->net_info.dns_server2[0]) == CHAR_INVALID || ((int)pset->net_info.dns_server2[0]) == 0)
		strcpy(pset->net_info.dns_server2, "168.154.160.4");  // Microsoft dns
        
    if(pset->net_info.type < NET_TYPE_STATIC || pset->net_info.type > NET_TYPE_DHCP)
        pset->net_info.type = NET_TYPE_DHCP;

    if(pset->net_info.http_port <= 0 || pset->net_info.http_port >= 65535)
        pset->net_info.http_port = DEFAULT_HTTP_PORT ;

    if(pset->net_info.https_port <= 0 || pset->net_info.https_port >= 65535)
        pset->net_info.https_port = DEFAULT_HTTPS_PORT ;

    if(pset->net_info.rtsp_port <= 0 || pset->net_info.rtsp_port >= 65535)
        pset->net_info.rtsp_port = 8551 ;
		
    if(pset->net_info.onvif_port <= 0 || pset->net_info.onvif_port >= 65535)
        pset->net_info.onvif_port = 9221 ;

    if(((int)pset->net_info.rtsp_name[0]) == CHAR_INVALID)
        strcpy(pset->net_info.rtsp_name, "all") ; 

	if(pset->net_info.wtype <= CFG_INVALID)
        pset->net_info.wtype = NET_TYPE_DHCP ;

	CHECK_PARAM_MIN_MAX(CFG_INVALID, MAX_HTTP_ENABLE,  DEFAULT_HTTP_ENABLE,  pset->net_info.http_enable );
	CHECK_PARAM_MIN_MAX(CFG_INVALID, MAX_HTTPS_MODE,   DEFAULT_HTTPS_MODE,   pset->net_info.https_mode  );
	CHECK_PARAM_MIN_MAX(CFG_INVALID, MAX_HTTPS_ENABLE, DEFAULT_HTTPS_ENABLE, pset->net_info.https_enable);
	CHECK_PARAM_MIN_MAX(CFG_INVALID, MAX_ONVIF_ENABLE, DEFAULT_ONVIF_ENABLE, pset->net_info.enable_onvif);

	if(pset->net_info.rtsp_enable <= CFG_INVALID || pset->net_info.rtsp_enable > 1)
		pset->net_info.rtsp_enable   = 1;
	if(pset->net_info.dnsFromDHCP <= CFG_INVALID || pset->net_info.dnsFromDHCP > 1) 
		pset->net_info.dnsFromDHCP = 0;
	if(pset->net_info.ntpFromDHCP <= CFG_INVALID || pset->net_info.ntpFromDHCP > 1) 
		pset->net_info.ntpFromDHCP = 0;

	//# Server information
	if(pset->srv_info.port <= CFG_INVALID)
		pset->srv_info.port = 80;

    if(pset->srv_info.ON_OFF <= CFG_INVALID)
        pset->srv_info.ON_OFF = OFF ;

    if(((int)pset->srv_info.ipaddr[0]) == CHAR_INVALID || (int)pset->srv_info.ipaddr[0] == 0)
		strcpy(pset->srv_info.ipaddr, "192.168.1.23");

	if(((int)pset->sys_info.deviceId[0] == CHAR_INVALID) || (int)pset->sys_info.deviceId[0] == 0)
	{
		strcpy(app_set->sys_info.deviceId, MODEL_NAME);
		strcat(app_set->sys_info.deviceId, "_0000");
	}

	if(strcmp(pset->sys_info.deviceId, "_MACADDRESS_")== 0)
    {
        if(!DefaultGetMac(MacAddr))
        {
            strncpy(app_set->sys_info.deviceId ,MacAddr, 12);
        }
        else
        {
            strcpy(pset->sys_info.deviceId, "_MACADDRESS_") ;
        }
    }

    if(pset->sys_info.osd_set < OFF || pset->sys_info.osd_set > ON)
        pset->sys_info.osd_set = ON ; 

    if(pset->sys_info.beep_sound < OFF || pset->sys_info.beep_sound > ON)
        pset->sys_info.beep_sound = ON ; 
//    if(pset->sys_info.P2P_ON_OFF < OFF || pset->sys_info.P2P_ON_OFF > ON)  
    if(pset->sys_info.aes_encryption < OFF || pset->sys_info.aes_encryption > ON)
        pset->sys_info.aes_encryption = OFF ; 

	if(((int)pset->sys_info.aes_key[0]) == CHAR_INVALID || (int)pset->sys_info.aes_key[0] == 0)
	{
 		Create_random_string(pset->sys_info.aes_key, 16) ;
        // random하게 key create
	}
	if(((int)pset->sys_info.aes_iv[0]) == CHAR_INVALID || (int)pset->sys_info.aes_iv[0] == 0)
	{
		sprintf(pset->sys_info.aes_iv, "%s", pset->sys_info.aes_key) ;
        // random하게 iv create
	}
    pset->sys_info.P2P_ON_OFF = ON ;  // Ignore previous version values, Always ON

    if((int)pset->sys_info.p2p_id[0] == CHAR_INVALID)
        strcpy(pset->sys_info.p2p_id, P2P_DEFAULT_ID) ;

    if((int)pset->sys_info.p2p_passwd[0] == CHAR_INVALID)
        strcpy(pset->sys_info.p2p_passwd, P2P_DEFAULT_PW) ;

	strcpy(compbuff, MODEL_NAME) ;

	if((int)pset->sys_info.uid[0] == CHAR_INVALID || (int)pset->sys_info.uid[0] == 0)
    {
		if(strcmp(compbuff, "FITT360_Security") != 0)
		{
    		if(dev_board_uid_read(uid, MAX_CHAR_16) == 0)
    		{
	    		if(!strncmp(uid, "LFS", 3))
	    		{
      			    sprintf(app_set->sys_info.uid, "%s", uid); 
				}
			}
			else
			{
   	        	strcpy(app_set->sys_info.uid ,"LFS-LSCS-A1-xxxx");
			}
		}
	}
	else
	{
        if(dev_board_uid_read(uid, MAX_CHAR_16) == 0)
        {
	        if(!strncmp(uid, "empty", 5))   // not exist uid on nand area
	        {
                dev_board_uid_write(pset->sys_info.uid, 16) ;
			}
		}
	}

	pset->sys_info.dev_cam_ch = MODEL_CH_NUM;
	//# FTP information
	if(pset->ftp_info.port <= CFG_INVALID)
		pset->ftp_info.port	= 21;
 
    if(pset->ftp_info.ON_OFF <= CFG_INVALID)
        pset->ftp_info.ON_OFF = OFF ;

	if((int)pset->ftp_info.ipaddr[0] == CHAR_INVALID || (int)pset->ftp_info.ipaddr[0] == 0)
		strcpy(pset->ftp_info.ipaddr, "192.168.1.23");

	if((int)pset->ftp_info.id[0] == CHAR_INVALID || (int)pset->ftp_info.id[0] == 0)
		strcpy(pset->ftp_info.id, "FTP_ID");

	if((int)pset->ftp_info.pwd[0]  == CHAR_INVALID || (int)pset->ftp_info.pwd[0] == 0)
		strcpy(pset->ftp_info.pwd, "FTP_PASSWORD");

    if(pset->ftp_info.file_type <= CFG_INVALID)
        pset->ftp_info.file_type = OFF ;
	//# FOTA information
	if(pset->fota_info.port <= CFG_INVALID)
		pset->fota_info.port	= 21;
 
	if(pset->fota_info.svr_info <= CFG_INVALID)
		pset->fota_info.svr_info	= 0;

    if(pset->fota_info.ON_OFF <= CFG_INVALID)
        pset->fota_info.ON_OFF = OFF ;

	if((int)pset->fota_info.ipaddr[0] == CHAR_INVALID || (int)pset->fota_info.ipaddr[0] == 0)
		strcpy(pset->fota_info.ipaddr, "192.168.40.6");

	if((int)pset->fota_info.id[0] == CHAR_INVALID || (int)pset->fota_info.id[0] == 0)
		strcpy(pset->fota_info.id, "FTP_ID");

	if((int)pset->fota_info.pwd[0]  == CHAR_INVALID || (int)pset->fota_info.pwd[0] == 0)
		strcpy(pset->fota_info.pwd, "FTP_PASSWORD");

	if((int)pset->fota_info.confname[0]  == CHAR_INVALID || (int)pset->fota_info.confname[0] == 0)
		sprintf(pset->fota_info.confname,"%s.conf", MODEL_NAME);
	//# Wifi AP information
	if(pset->wifiap.en_key != ON && pset->wifiap.en_key != OFF)
	    pset->wifiap.en_key = ON;

	if((int)pset->wifiap.ssid[0] == CHAR_INVALID || (int)pset->wifiap.ssid[0] == 0)
	    strcpy(pset->wifiap.ssid, "AP_SSID");

	if((int)pset->wifiap.stealth == CFG_INVALID || (int)pset->wifiap.stealth == CHAR_INVALID)
	        pset->wifiap.stealth = OFF ;

    #if 0
	if((int)pset->wifiap.pwd[0] == CHAR_INVALID || (int)pset->wifiap.pwd[0] == 0)
    #else
	if((int)pset->wifiap.pwd[0] == CHAR_INVALID ) // bk 2020.02.26 allow null password
    #endif
        strcpy(pset->wifiap.pwd,"AP_PASSWORD");
	//# Wifilist information
	for(i = 0 ; i < WIFIAP_CNT; i++)
	{
		if(pset->wifilist[i].en_key != ON && pset->wifilist[i].en_key != OFF)
			pset->wifilist[i].en_key = ON;

		if((int)pset->wifilist[i].ssid[0] == CHAR_INVALID)
			memset(pset->wifilist[i].ssid, 0, MAX_CHAR_32);

		if((int)pset->wifilist[i].stealth == CFG_INVALID || (int)pset->wifilist[i].stealth == CHAR_INVALID)
	        pset->wifilist[i].stealth = OFF ;

		#if 0
		if((int)pset->wifilist[i].pwd[0] == CHAR_INVALID || (int)pset->wifilist[i].pwd[0] == 0)
		#else
		if((int)pset->wifilist[i].pwd[0] == CHAR_INVALID ) // bk 2020.02.26 allow null password
		#endif
		    memset(pset->wifilist[i].pwd, 0, MAX_CHAR_64);
	}

	if(pset->stm_info.enable_audio != ON && pset->stm_info.enable_audio != OFF)
	    pset->stm_info.enable_audio = OFF ;

	if(pset->rec_info.period_idx < REC_PERIOD_01 && pset->rec_info.period_idx >= REC_PERIOD_MAX)
		pset->rec_info.period_idx = REC_PERIOD_01;

	if(pset->rec_info.overwrite != ON && pset->rec_info.overwrite != OFF)
		pset->rec_info.overwrite = ON;
#if defined(NEXX360C) || defined(NEXX360W_CCTV)
	/* default auto record on */
	pset->rec_info.auto_rec = ON;
#else 
	//# NEXX360B/NEXX360W/NEXX360W_MUX/NEXXB/NEXXB_ONE/NEXX360M/NEXXONE
    if(pset->rec_info.auto_rec != ON && pset->rec_info.auto_rec != OFF)
	    pset->rec_info.auto_rec = OFF ;
#endif		
	if(pset->rec_info.pre_rec != ON && pset->rec_info.pre_rec != OFF)
	    pset->rec_info.pre_rec = OFF ;
		
	if(pset->rec_info.audio_rec != ON && pset->rec_info.audio_rec != OFF)
	    pset->rec_info.audio_rec = OFF ;

    if(pset->ddns_info.ON_OFF <= CFG_INVALID) 
        pset->ddns_info.ON_OFF = OFF ;
	if((int)pset->ddns_info.userId[0] == CHAR_INVALID || (int)pset->ddns_info.userId[0] == 0)
		strcpy(pset->ddns_info.userId, "ID");

	if((int)pset->ddns_info.passwd[0] == CHAR_INVALID || (int)pset->ddns_info.passwd[0] == 0)
		strcpy(pset->ddns_info.passwd, "PASSWORD");

	if((int)pset->ddns_info.serveraddr[0] == CHAR_INVALID || (int)pset->ddns_info.serveraddr[0] == 0)
		strcpy(pset->ddns_info.serveraddr, "DDNS_Server");

	if((int)pset->ddns_info.hostname[0] == CHAR_INVALID || (int)pset->ddns_info.hostname[0] == 0)
		strcpy(pset->ddns_info.hostname, "Host_Name");

    if(pset->ddns_info.interval <= 0)
        pset->ddns_info.interval = 1 ;

    if(pset->time_info.time_zone <= 0 || pset->time_info.time_zone>26)
		pset->time_info.time_zone = TIME_ZONE + 12;


    if((int)pset->time_info.time_zone_abbr[0] == CHAR_INVALID || (int)pset->time_info.time_zone_abbr[0] == 0)
        strcpy(pset->time_info.time_zone_abbr, "KST") ;

    if((int)pset->time_info.time_server[0] == CHAR_INVALID || (int)pset->time_info.time_server[0] == 0)
        strcpy(pset->time_info.time_server, "time.google.com") ;

    if(pset->time_info.daylight_saving < 0 || pset->time_info.daylight_saving > 1 )
        pset->time_info.daylight_saving = 0 ;

    if(  pset->time_info.timesync_type < 0 || pset->time_info.timesync_type > 2 )
        pset->time_info.timesync_type = 1 ;

    if((int)pset->account_info.ON_OFF <= CFG_INVALID || (int)pset->account_info.ON_OFF > 1 )
        pset->account_info.ON_OFF = ON ;

    printf("pset->account_info.ON_OFF		= %d\n", pset->account_info.ON_OFF) ;

//    if(pset->account_info.enctype <= CFG_INVALID || pset->account_info.enctype > 1)
    pset->account_info.enctype = 0 ;
	
    if((int)pset->account_info.rtsp_userid[0] == CHAR_INVALID || (int)pset->account_info.rtsp_userid[0] == 0)
	{
		strcpy(pset->account_info.rtsp_userid, RTSP_DEFAULT_ID) ;
	}
		
	if((int)pset->account_info.rtsp_passwd[0] == CHAR_INVALID || (int)pset->account_info.rtsp_passwd[0] == 0)
	{
		Create_random_string(pset->account_info.rtsp_passwd, 16) ;
	}
	// webuser
    if((int)pset->account_info.webuser.id[0] == CHAR_INVALID || (int)pset->account_info.webuser.id[0] == 0){
		strcpy(pset->account_info.webuser.id, WEB_DEFAULT_ID);
		strcpy(pset->account_info.webuser.pw, WEB_DEFAULT_PW);
		pset->account_info.webuser.lv = 0;       // 0:Administrator default
		pset->account_info.webuser.authtype = 1; // 0:base64 1:digest(default)
	}
    if((int)pset->account_info.webuser.pw[0] == CHAR_INVALID || (int)pset->account_info.webuser.pw[0] == 0){
		strcpy(pset->account_info.webuser.pw, WEB_DEFAULT_PW);
		pset->account_info.webuser.lv = 0;
		pset->account_info.webuser.authtype = 1; // default 0
	}
    if(pset->account_info.webuser.lv <= CFG_INVALID || pset->account_info.webuser.lv > 2) {
		pset->account_info.webuser.lv       = 0;
		pset->account_info.webuser.authtype = 1; // default 0
	}
    if(pset->account_info.webuser.authtype <= CFG_INVALID || pset->account_info.webuser.authtype > 1)
		pset->account_info.webuser.authtype = 1;

    TRACE_INFO("pset->account_info.webuser.id		= %s\n", pset->account_info.webuser.id) ;
    TRACE_INFO("pset->account_info.webuser.pw		= %s\n", pset->account_info.webuser.pw) ;
    TRACE_INFO("pset->account_info.webuser.lv		= %d\n", pset->account_info.webuser.lv) ;
    TRACE_INFO("pset->account_info.webuser.authtype = %d\n", pset->account_info.webuser.authtype) ;

	// onvif user
    if((int)pset->account_info.onvif.id[0] == CHAR_INVALID || (int)pset->account_info.onvif.id[0] == 0){
		strcpy(pset->account_info.onvif.id, ONVIF_DEFAULT_ID);
		strcpy(pset->account_info.onvif.pw, ONVIF_DEFAULT_PW);
		pset->account_info.onvif.lv = 0;       // 0:Administrator default
	}
    if((int)pset->account_info.onvif.pw[0] == CHAR_INVALID || (int)pset->account_info.onvif.pw[0] == 0){
		strcpy(pset->account_info.onvif.pw, ONVIF_DEFAULT_PW);
		pset->account_info.onvif.lv = 0;
	}
    if(pset->account_info.onvif.lv <= CFG_INVALID || pset->account_info.onvif.lv > 4) {
		pset->account_info.onvif.lv       = 0;
	}
    TRACE_INFO("pset->account_info.onvif.lv		= %d\n", pset->account_info.onvif.lv) ;
    TRACE_INFO("pset->account_info.onvif.id		= %s\n", pset->account_info.onvif.id) ;
    TRACE_INFO("pset->account_info.onvif.pw		= %s\n", pset->account_info.onvif.pw) ;

    if((int)pset->multi_ap.ON_OFF <= CFG_INVALID || (int)pset->multi_ap.ON_OFF > 1 )
        pset->multi_ap.ON_OFF = OFF ;

    TRACE_INFO("pset->multi_ap.ON_OFF = %d\n", pset->multi_ap.ON_OFF) ;
	
	//# ----------------- VOIP Parameters Start -----------------------------------------
	if((int)pset->voip.ipaddr[0] == CHAR_INVALID || (int)pset->voip.ipaddr[0] == 0)
		strcpy(pset->voip.ipaddr, PBX_SERVER_ADDR);

	if(pset->voip.port <= CFG_INVALID)
		pset->voip.port = PBX_SERVER_PORT;

	if((int)pset->voip.userid[0] == CHAR_INVALID || (int)pset->voip.userid[0] == 0)
		strcpy(pset->voip.userid, "");

	if((int)pset->voip.passwd[0]  == CHAR_INVALID || (int)pset->voip.passwd[0] == 0)
		strcpy(pset->voip.passwd, PBX_SERVER_PW);

	if((int)pset->voip.peerid[0] == CHAR_INVALID || (int)pset->voip.peerid[0] == 0)
		strcpy(pset->voip.peerid, "");

	if(pset->voip.use_stun <= CFG_INVALID)
		pset->voip.use_stun = 0;

#if defined(NEXXONE) || defined(NEXXB) || defined(NEXXB_ONE)	
   	if(pset->voip.ON_OFF <= CFG_INVALID)
		pset->voip.ON_OFF = ON;

	if(pset->voip.ON_OFF != ON) // Backchannel audio
		pset->stm_info.enable_audio = ON ;
	else // voip
		pset->stm_info.enable_audio = OFF ;

#else
	//# NEXX360B/NEXX360W/NEXX360W_MUX/NEXX360C/NEXX360W_CCTV
   	if(pset->voip.ON_OFF <= CFG_INVALID)
		pset->voip.ON_OFF = OFF;
#endif	

	app_cfg->voip_set_ON_OFF = pset->voip.ON_OFF ;
	app_cfg->stream_enable_audio = pset->stm_info.enable_audio;
	app_cfg->rec_overwrite = pset->rec_info.overwrite ;

	//# ----------------- VOIP Parameters End -----------------------------------------

	if(pset->rtmp.ON_OFF <= CFG_INVALID)
		pset->rtmp.ON_OFF = OFF;

//	pset->rtmp.ON_OFF = OFF;

	if(pset->rtmp.USE_URL <= CFG_INVALID)
		pset->rtmp.USE_URL = OFF;

	if(pset->rtmp.port <= CFG_INVALID)
		pset->rtmp.port = RTMP_SERVER_PORT; // RTMP_DEFAULT_PORT

	if((int)pset->rtmp.ipaddr[0] == CHAR_INVALID || (int)pset->rtmp.ipaddr[0] == 0)
		strcpy(pset->rtmp.ipaddr, RTMP_SERVER_ADDR);

	if((int)pset->rtmp.FULL_URL[0] == CHAR_INVALID || (int)pset->rtmp.FULL_URL[0] == 0)
		strcpy(pset->rtmp.FULL_URL, RTMP_SERVER_URL);

	
	if(pset->sslvpn_info.ON_OFF <= CFG_INVALID)
		pset->sslvpn_info.ON_OFF = OFF ;

	if(pset->sslvpn_info.vendor <= CFG_INVALID)
		pset->sslvpn_info.vendor = OFF ;  // 0 AXGATE VPN
	
	if((int)pset->sslvpn_info.vpn_id[0] == CHAR_INVALID || (int)pset->sslvpn_info.vpn_id[0] == 0)
		strcpy(pset->sslvpn_info.vpn_id, SSLVPN_ID) ;

	if(pset->sslvpn_info.heartbeat_interval <= CFG_INVALID || pset->sslvpn_info.heartbeat_interval > 32767)
		pset->sslvpn_info.heartbeat_interval = 3000 ;
	
	if(pset->sslvpn_info.heartbeat_threshold <= CFG_INVALID || pset->sslvpn_info.heartbeat_threshold > 32767)
		pset->sslvpn_info.heartbeat_threshold = 3 ;

	if(pset->sslvpn_info.protocol <= CFG_INVALID)
		pset->sslvpn_info.protocol = OFF; // 0 TCP, 1 UDP

	if((pset->sslvpn_info.port < 1024) || pset->sslvpn_info.port > 49151)
		pset->sslvpn_info.port = 3900 ;

	if(pset->sslvpn_info.queue <= CFG_INVALID || pset->sslvpn_info.queue > 32767)
		pset->sslvpn_info.queue = 16384 ;
	
	if((int)pset->sslvpn_info.key[0] == CHAR_INVALID || (int)pset->sslvpn_info.key[0] == 0)
		strcpy(pset->sslvpn_info.key, SSLVPN_KEY) ;

	if(pset->sslvpn_info.encrypt_type <= CFG_INVALID)
		pset->sslvpn_info.encrypt_type = OFF ; // 0 aes128, 1 aes256, 2 aria128, 3 aria256, 4 lea128, 5lea256, 6 seed

	if((int)pset->sslvpn_info.ipaddr[0] == CHAR_INVALID || (int)pset->sslvpn_info.ipaddr[0] == 0)
		strcpy(pset->sslvpn_info.ipaddr, SSLVPN_IPADDRESS) ;
	
	if(pset->sslvpn_info.NI <= CFG_INVALID)
		pset->sslvpn_info.NI = 2 ; // 0 eth0, 1 wlan0, 2 usb0, 3 eth1

	if(!check_module_version(pset->module_ver.OpenSSH_ver, pset->module_ver.OpenSSL_ver, pset->module_ver.Webserver_ver))
	{
		strcpy(pset->module_ver.OpenSSH_ver, "7.9p1") ;
		strcpy(pset->module_ver.OpenSSL_ver, "1.1.1o") ;
		strcpy(pset->module_ver.Webserver_ver, "1.4.65(ssl)") ;
	}

	if(0 == access("/mmc/show_all_cfg", F_OK))
		show_all_cfg(pset); // BKKIM
}

static void app_set_version_read(void)
{
	if (app_set != NULL) {
		sprintf(app_set->sys_info.fw_ver, "%s", FITT360_SW_VER);
		sprintf(app_set->sys_info.hw_ver, "%s", FITT360_HW_VER);
	}
}

static int cfg_read(int is_mmc, char* cfg_path)
{
    int readSize=0, app_set_size=0;
    int saved_cfg_size=0;
	
	if(is_mmc){
		if (!app_cfg->ste.b.mmc) {
			TRACE_INFO("#### NO INSERTED SD CARD !! ####\n");
			return EFAIL;
		}
	}

    if (-1 == access(cfg_dir[is_mmc], 0)) {
        mkdir(cfg_dir[is_mmc], 0775);
        chmod(cfg_dir[is_mmc], 0775);

		TRACE_INFO("#### NOT EXIST CFG DIR [%s] !! ####\n", cfg_path);
		return EFAIL;
    }

    if (-1 == access(cfg_path, 0)) {
		//# cpoy nand cfg file to SD
		TRACE_INFO("#### NOT EXIST CFG FILE IN [%s] !! ####\n", cfg_path);
		return EFAIL;
	}

	app_set_size 	= sizeof(app_set_t);		//# current version cfg_size
	saved_cfg_size	= get_cfg_size(cfg_path);	//# sd cfg_size (old version)

	if (app_set_size != saved_cfg_size) {
		//# cfg is different
		LOGD("[main] #### [%s] DIFF CFG SIZE - app_set:%d / read:%d !!! ####\n", 
										cfg_path, app_set_size, saved_cfg_size);
		return EFAIL;
	} else {
	    //#--- ucx app setting param
    	OSA_fileReadFile(cfg_path, (Uint8*)app_set, app_set_size, (Uint32*)&readSize);
		if(readSize == 0 || readSize != app_set_size) {
			TRACE_INFO(" #### CFG Read Failed [%s] !! ####\n", cfg_path);
			return EFAIL;
		}

	    cfg_param_check_nexx(app_set);
		app_set_version_read();
	}

	return SOK;
}

static void app_set_default(int default_type)
{
    char MacAddr[12] ;
    char enc_ID[32] = {0, } ;
    char enc_Passwd[32] = {0, } ;
    app_set_t tmp_set ;

	int ich=0, channels = 0, i = 0;
	
	LOGD("[main] - SET DEFAULT CFG... !!! MODEL_NAME=%s\n", MODEL_NAME);

    if (app_set == NULL);
        app_set = (app_set_t *)&app_sys_set;

    if (!default_type)
        memcpy(&tmp_set, app_set, sizeof(app_set_t)) ;

	//# Encoding cfg per channel
	channels = MODEL_CH_NUM+1;

	for (ich = 0; ich < channels; ich++)
	{
		app_set->ch[ich].resol		= RESOL_720P;

		if(ich == MODEL_CH_NUM) // streaming channel
		{
#if defined(NEXXONE) || defined(NEXXB_ONE)	
		    app_set->ch[ich].framerate	= DEFAULT_FPS/2;  // 15fps
#else
		    app_set->ch[ich].framerate	= DEFAULT_FPS;    // 15fps
#endif
		    app_set->ch[ich].quality	= DEFAULT_STM_QUALITY;  // 1Mbps for live streaming
		}
        else // recording channels
		{
		    app_set->ch[ich].framerate	= DEFAULT_FPS;     // 15fps or 30fps
		    app_set->ch[ich].quality	= DEFAULT_REC_QUALITY; // 4Mbps for live streaming
		}

		app_set->ch[ich].rate_ctrl	= RATE_CTRL_VBR;
		app_set->ch[ich].motion 	= OFF;
		app_set->ch[ich].gop 	    = DEFAULT_FPS;
	}
	
	app_set->wd.gsn = GSN_IDX_03;

	/******************** begin of net_info ********************/
    if (default_type)  // FULL reset or hw reset
    {
    	//# Network information for device
        app_set->net_info.type = NET_TYPE_DHCP ;
        strcpy(app_set->net_info.wlan_ipaddr, "192.168.0.252");
	    app_set->wifiap.stealth = OFF;
        strcpy(app_set->net_info.wlan_netmask, "255.255.0.0");
        strcpy(app_set->net_info.wlan_gateway, "192.168.0.1");

        strcpy(app_set->net_info.eth_ipaddr, "192.168.1.252");
        strcpy(app_set->net_info.eth_netmask, "255.255.0.0");
        strcpy(app_set->net_info.eth_gateway, "192.168.1.1");
    }
    else  // soft reset
    {
        app_set->net_info.type = tmp_set.net_info.type ;

        strcpy(app_set->net_info.wlan_ipaddr, tmp_set.net_info.wlan_ipaddr);
        strcpy(app_set->net_info.wlan_netmask, tmp_set.net_info.wlan_netmask);
        strcpy(app_set->net_info.wlan_gateway, tmp_set.net_info.wlan_netmask);

        strcpy(app_set->net_info.eth_ipaddr, tmp_set.net_info.eth_ipaddr);
        strcpy(app_set->net_info.eth_netmask, tmp_set.net_info.eth_netmask);
        strcpy(app_set->net_info.eth_gateway, tmp_set.net_info.eth_gateway);
    }

    strcpy(app_set->net_info.dns_server1, "8.8.8.8"); // google dns server
    strcpy(app_set->net_info.dns_server2, "168.154.160.4"); // Microsoft dns server

    app_set->net_info.http_port  = DEFAULT_HTTP_PORT;
    app_set->net_info.https_port = DEFAULT_HTTPS_PORT;
    app_set->net_info.rtsp_port  = 8551;
    app_set->net_info.onvif_port = 9221;

    strcpy(app_set->net_info.rtsp_name, "all") ;
    app_set->net_info.wtype = NET_TYPE_DHCP ;  // wifi dhcp(from tethering of phone or from AP) or static

    app_set->net_info.http_enable  = DEFAULT_HTTP_ENABLE;
    app_set->net_info.https_enable = DEFAULT_HTTPS_ENABLE;
    app_set->net_info.rtsp_enable  = 1;
    app_set->net_info.enable_onvif = 1;
    app_set->net_info.dnsFromDHCP  = 0;
    app_set->net_info.ntpFromDHCP  = 0;

    app_set->net_info.https_mode   = DEFAULT_HTTPS_MODE;
	memset(app_set->net_info.ssc_C,  0, MAX_CHAR_64);
	memset(app_set->net_info.ssc_ST, 0, MAX_CHAR_64);
	memset(app_set->net_info.ssc_L,  0, MAX_CHAR_64);
	memset(app_set->net_info.ssc_O,  0, MAX_CHAR_64);
	memset(app_set->net_info.ssc_OU, 0, MAX_CHAR_64);
	memset(app_set->net_info.ssc_CN, 0, MAX_CHAR_64);
	memset(app_set->net_info.ssc_Email, 0, MAX_CHAR_64);
	memset(app_set->net_info.cert_name, 0, MAX_CHAR_64);
	/******************** end of net_info ********************/
	//# Meta Server information
    app_set->srv_info.ON_OFF = OFF ;
    app_set->srv_info.port = 80 ;
    strcpy(app_set->srv_info.ipaddr, "0.0.0.0") ;

	//# FTP information
    app_set->ftp_info.port = 21;
    app_set->ftp_info.ON_OFF = OFF ;
    strcpy(app_set->ftp_info.ipaddr, "192.168.1.23");
    strcpy(app_set->ftp_info.id, "FTP_ID");
    strcpy(app_set->ftp_info.pwd, "FTP_PASSWORD");

	//# FOTA information
    app_set->fota_info.port = 21;
    app_set->fota_info.svr_info = 0;
    app_set->fota_info.ON_OFF = OFF ;
    strcpy(app_set->fota_info.ipaddr, "192.168.40.6");
    strcpy(app_set->fota_info.id, "FTP_ID");
    strcpy(app_set->fota_info.pwd, "FTP_PASSWORD");

	//# Wifi AP information
    app_set->wifiap.en_key = ON;
    strcpy(app_set->wifiap.ssid, "AP_SSID") ;
    strcpy(app_set->wifiap.pwd,"AP_PASSWORD") ;
	app_set->wifiap.stealth = OFF;

	//# Wifilist information
    for(i = 0 ; i < WIFIAP_CNT; i++)
	{
		app_set->wifilist[i].en_key = ON;
		//# 0으로 초기화
		memset(app_set->wifilist[i].ssid, 0, MAX_CHAR_32);
		memset(app_set->wifilist[i].pwd, 0, MAX_CHAR_64);
		app_set->wifilist[i].stealth = OFF;
    }

	app_set_version_read();

    if (!DefaultGetMac(MacAddr)) {
        strncpy(app_set->sys_info.deviceId, MacAddr, 12);
    }
    else {
        TRACE_ERR( "Fatal error: Failed to get local host's MAC address\n" );
    }

    app_set->sys_info.osd_set = ON ;
    app_set->sys_info.beep_sound = ON ;
    app_set->sys_info.aes_encryption = OFF ;

//  random 함수 이용 
 	Create_random_string(app_set->sys_info.aes_key, 16) ;
    sprintf(app_set->sys_info.aes_iv, "%s", app_set->sys_info.aes_key) ;

    app_set->sys_info.P2P_ON_OFF = ON ;
	app_set->sys_info.dev_cam_ch = MODEL_CH_NUM;
    strcpy(app_set->sys_info.p2p_id,     P2P_DEFAULT_ID) ; 
    strcpy(app_set->sys_info.p2p_passwd, P2P_DEFAULT_PW) ;

	//# streaming information
	app_set->stm_info.enable_audio     = OFF ;

	//# rec information
	app_set->rec_info.period_idx 	= REC_PERIOD_01;
	app_set->rec_info.overwrite 	= ON;
	app_cfg->rec_overwrite 	= app_set->rec_info.overwrite;


#if defined(NEXXONE) || defined(NEXX360W)	|| defined(NEXXB) || defined(NEXXB_ONE)
	app_set->rec_info.auto_rec      = OFF ;
#elif defined(NEXX360C) || defined(NEXX360W_CCTV)
	/* default auto record on */
	app_set->rec_info.auto_rec = ON;
#else //# NEXX360B, NEXX360M, NEXX360W_MUX
    app_set->rec_info.auto_rec      = OFF ;
#endif

	app_set->rec_info.pre_rec       = OFF ;
	app_set->rec_info.audio_rec     = OFF ;

    app_set->ddns_info.ON_OFF = OFF ; 
	strcpy(app_set->ddns_info.userId, "ID");
	strcpy(app_set->ddns_info.passwd, "PASSWORD");
	strcpy(app_set->ddns_info.serveraddr, "DDNS_Server");
	strcpy(app_set->ddns_info.hostname, "Host_Name");

/*
    app_set->ddns_info.ON_OFF = ON ; 

	strcpy(app_set->ddns_info.userId, "hwjun@udworks.com");
	strcpy(app_set->ddns_info.passwd, "rornfl71");
	strcpy(app_set->ddns_info.serveraddr, "http://dynupdate.no-ip.com/nic/update");
	strcpy(app_set->ddns_info.hostname, "fitt360.ddns.net");
*/  
    app_set->ddns_info.interval = 1 ;
	strcpy(app_set->sys_info.deviceId, MODEL_NAME);
	
	if (strcmp(app_set->sys_info.deviceId, "FITT360 Security")==0) {
		strcpy(app_set->sys_info.deviceId, "FITT360_0000");
	    strcpy(app_set->time_info.time_zone_abbr, "JST");
	} else {
		strcat(app_set->sys_info.deviceId, "_0000");
	    strcpy(app_set->time_info.time_zone_abbr, "KST");
	}
    
	strcpy(app_set->sys_info.uid ,"LFS-LSCS-A1-xxxx");

    app_set->time_info.time_zone = TIME_ZONE + 12;
    strcpy(app_set->time_info.time_server, "time.google.com") ;
    app_set->time_info.daylight_saving = 0 ;
    app_set->time_info.timesync_type = 1 ;

	app_set->account_info.ON_OFF = ON;
    app_set->account_info.enctype = 0 ;
//    if(app_set->account_info.enctype) // 1, AES encryption

//    strcpy(app_set->account_info.rtsp_userid, RTSP_DEFAULT_ID);
//    strcpy(app_set->account_info.rtsp_passwd, RTSP_DEFAULT_PW);

 	strcpy(app_set->account_info.rtsp_userid, RTSP_DEFAULT_ID) ;
 	Create_random_string(app_set->account_info.rtsp_passwd, 16) ;
	printf("create rtsp id... from random() %s \n",app_set->account_info.rtsp_passwd) ;
 

	app_set->account_info.webuser.lv = 0;			// 0:Administrator, 1:operator?(TBD)
	app_set->account_info.webuser.authtype = 1;		// 0:basic, 1:digest(default)
	strcpy(app_set->account_info.webuser.id, WEB_DEFAULT_ID);
	strcpy(app_set->account_info.webuser.pw, WEB_DEFAULT_PW);

	app_set->account_info.onvif.lv = 0;	// 0:Administrator
	strcpy(app_set->account_info.onvif.id, ONVIF_DEFAULT_ID); // fixed
	strcpy(app_set->account_info.onvif.pw, ONVIF_DEFAULT_PW);
    app_set->multi_ap.ON_OFF = OFF ; 
	
	//#------------- VOIP Params Start ---------------------------------------------------
    strcpy(app_set->voip.ipaddr, PBX_SERVER_ADDR);
    app_set->voip.port = PBX_SERVER_PORT;
    strcpy(app_set->voip.passwd, PBX_SERVER_PW);
	memset((void*)app_set->voip.userid, 0x00, sizeof(app_set->voip.userid));
	memset((void*)app_set->voip.peerid, 0x00, sizeof(app_set->voip.peerid));
    app_set->voip.use_stun = 0;
	
#if defined(NEXXONE) || defined(NEXXB) || defined(NEXXB_ONE)	
    app_set->voip.ON_OFF = ON;
#else
	//# NEXX360B/NEXX360W/NEXX360W_MUX/NEXX360C/NEXX360W_CCTV
    app_set->voip.ON_OFF = OFF;
#endif	
    
	app_cfg->voip_set_ON_OFF = app_set->voip.ON_OFF ;
	app_cfg->stream_enable_audio = app_set->stm_info.enable_audio;

	memset((void*)app_set->voip.reserved, 0x00, 38);
	//#------------- VOIP Params End ---------------------------------------------------

	app_set->rtmp.ON_OFF = OFF;
	app_set->rtmp.USE_URL = OFF;
	app_set->rtmp.port = RTMP_SERVER_PORT; // RTMP_DEFAULT_PORT
	strcpy(app_set->rtmp.ipaddr, RTMP_SERVER_ADDR);
	strcpy(app_set->rtmp.FULL_URL, RTMP_SERVER_URL);

	app_set->sslvpn_info.ON_OFF = OFF ;
	app_set->sslvpn_info.vendor = OFF ; // 0 AXGATE
	strcpy(app_set->sslvpn_info.vpn_id, SSLVPN_ID) ;
	app_set->sslvpn_info.heartbeat_interval = 3000 ;
	app_set->sslvpn_info.heartbeat_threshold = 3 ;
	app_set->sslvpn_info.protocol = OFF ; // 0 tcp , 1 udp 
	app_set->sslvpn_info.port = 3900 ;
	app_set->sslvpn_info.queue = 16384 ;
	strcpy(app_set->sslvpn_info.key, SSLVPN_KEY) ;
	app_set->sslvpn_info.encrypt_type = OFF ; // 0 aes128, 1 aes256, 2 aria128, 3 aria256, 4 lea128, 5lea256, 6 seed
	strcpy(app_set->sslvpn_info.ipaddr, SSLVPN_IPADDRESS) ;
	app_set->sslvpn_info.NI = 2 ; //  0 eth0, 1 wlan0, 2 usb0, 3 eth1

	if(check_module_version(app_set->module_ver.OpenSSH_ver, app_set->module_ver.OpenSSL_ver, app_set->module_ver.Webserver_ver))
	{
		printf("app_set->module_ver.OpenSSH_ver = %s\n",app_set->module_ver.OpenSSH_ver) ;
		printf("app_set->module_ver.OpenSSL_ver = %s\n",app_set->module_ver.OpenSSL_ver) ;
		printf("app_set->module_ver.Webserver_ver = %s\n",app_set->module_ver.Webserver_ver) ;
	}
	else
	{
		strcpy(app_set->module_ver.OpenSSH_ver, "7.9p1") ;
		strcpy(app_set->module_ver.OpenSSL_ver, "1.1.1o") ;
		strcpy(app_set->module_ver.Webserver_ver, "1.4.65(ssl)") ;
	}
}

/*****************************************************************************
* @brief    system cfg open
* @section  [desc]
*****************************************************************************/
int app_set_open(void)
{
    int ret=SOK;

    //#--- ucx app setting param
    app_set = (app_set_t *)&app_sys_set;
	char_memset();

#if ENABLE_JSON_CONFIG_FILE 	
	// try read config from json file
	if(access(NEXX_CFG_JSON_ENCRYPT_MMC, F_OK) == 0){
		// decryption file  
		if(openssl_aes128_decrypt_fs(NEXX_CFG_JSON_ENCRYPT_MMC, NEXX_CFG_JSON_MMC) == SUCC)
		{
			ret = js_read_settings(app_set, NEXX_CFG_JSON_MMC) ;
	    	if( EFAIL == ret)
			{
				ret = js_read_settings(app_set, NEXX_CFG_JSON_NAND) ;
			}
		}
	}
	else
	{
		ret = js_read_settings(app_set, NEXX_CFG_JSON_MMC) ;
	   	if( EFAIL == ret)
		{
			ret = js_read_settings(app_set, NEXX_CFG_JSON_NAND) ;
		}
	}
	
	// data 검사..cfg_read 아래부분에 있는거 복붙..
	if(EFAIL != ret){
		LOGD("[main] Reading configuration file succeed!\n");
	    cfg_param_check_nexx(app_set);
		app_set_version_read();
	}
	else
#endif
	{
	/* 
	* Fitt360 CFG Path 
	*            MMC  : ---> /mmc/cfg/fbx_cfg.ini
	*            NAND : ---> /media/nand/cfg/fbx_cfg.ini
	* 
	* NEXX360/NEXXONE CFG Path 
	*            MMC  : --> /mmc/cfg/nexx_cfg.ini
	*            NAND : --> /media/nand/cfg/nexx_cfg.ini 
	*/
	    if (cfg_read(CFG_MMC, NEXX_CFG_FILE_MMC) == EFAIL) //# sd read first.
		    ret = cfg_read(CFG_NAND, NEXX_CFG_FILE_NAND);  //# nand read if sd read fail
		else
			ret = SOK ;
	}
	
	if (ret == EFAIL) {
	    LOGE("[main] Failed to read configuration file from storage!\n");
		app_set_default(FULL_RESET);
	}
	
	/* remove cfg directory in nand */
	ret = access(CFG_DIR_NAND, R_OK|W_OK);
    if ((ret == 0) || (errno == EACCES)) {
		remove(CFG_DIR_NAND);
	} 
	
	/* remove cfg directory in SD card */
	if (app_cfg->ste.b.mmc) {
		ret = access(CFG_DIR_MMC, R_OK|W_OK);
		if ((ret == 0) || (errno == EACCES)) {
			remove(CFG_DIR_MMC);
		} 
	}
	// sync(); // changed to app_ser_write()
    set_uid() ;  // read uid from nand and then set uid to app_set
	if(app_set->sslvpn_info.ON_OFF)
		app_set_sslvpnconf() ;
	app_set_write();
	
	TRACE_INFO("...done\n");
	return 0;
}

void app_setting_reset(int type)  // sw reset, hw reset(include network setting) 
{
    if (type >= 0 && type < 2)
    {
        app_set_default(type);  // onvif factory default
        set_uid() ;  // read uid from nand and then set uid to app_set
	    ctrl_sys_halt(0); /* reboot */
    } 
}

int app_set_write(void)
{
	char path[128] ={0,};

#if ENABLE_JSON_CONFIG_FILE 	
	if (app_cfg->ste.b.mmc)
	{
		if (-1 == access(CFG_DIR_MMC, 0)) {
			mkdir(CFG_DIR_MMC, 0775);
			chmod(CFG_DIR_MMC, 0775);
		}
        if(js_write_settings(app_set, NEXX_CFG_JSON_MMC) != OSA_SOK)
			TRACE_INFO("couldn't open %s file\n", path);
	}
	//# save cfg in nand.

	if (-1 == access(CFG_DIR_NAND, 0)) {
		mkdir(CFG_DIR_NAND, 0775);
		chmod(CFG_DIR_NAND, 0775);
	}
    if(js_write_settings(app_set, NEXX_CFG_JSON_NAND) != OSA_SOK)
	    TRACE_INFO("couldn't open %s file\n", path);

#else
	if (app_cfg->ste.b.mmc)
	{
		if (-1 == access(CFG_DIR_MMC, 0)) {
			mkdir(CFG_DIR_MMC, 0775);
			chmod(CFG_DIR_MMC, 0775);
		}

		snprintf(path, sizeof(path), "%s", NEXX_CFG_FILE_MMC);
		if (OSA_fileWriteFile(path, (Uint8*)app_set, sizeof(app_set_t)) != OSA_SOK) {
			TRACE_INFO("couldn't open %s file\n", path);
		}
   }

	//# save cfg in nand.
	if (-1 == access(CFG_DIR_NAND, 0)) {
		mkdir(CFG_DIR_NAND, 0775);
		chmod(CFG_DIR_NAND, 0775);
	}
	
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), "%s", NEXX_CFG_FILE_NAND);
	
	if (OSA_fileWriteFile(path, (Uint8*)app_set, sizeof(app_set_t)) != OSA_SOK) {
		TRACE_INFO("couldn't open %s file\n", path);
	}
#endif
//	if(app_set->sys_info.aes_encryption)  
	{
		if(openssl_aes128_encrypt_fs(NEXX_CFG_JSON_MMC, NEXX_CFG_JSON_ENCRYPT_MMC) == SUCC) {
			printf("openssl_aes128_encrypt_fs......\n") ;
		    remove(NEXX_CFG_JSON_MMC);	// fixme : in memory
		} else {
			TRACE_INFO("openssl_aes128_encrypt_fs. failed!!\n");
		}
	}
	sync();
	TRACE_INFO("CFG sync done!\n");

	return 0;
}

int get_frame_size(int resol, int *wi, int *he)
{
	if(resol == RESOL_1080P) { // 2
		*wi = 1920;
		*he = 1080;
	}
	else if(resol == RESOL_720P) {
		*wi = 1280;
		*he = 720;
	}
	else if(resol == RESOL_480P) {
		*wi = 720;
		*he = 480;
	}
	else {
		TRACE_INFO("Invaild resol param %d\n", resol);
		return EFAIL;
	}

	return SOK;
}

int app_set_web_password(char *id, char *pw, int lv, int authtype)
{
	if(strcmp(id, WEB_DEFAULT_ID)==0) {
		strcpy(app_set->account_info.webuser.id, WEB_DEFAULT_ID);
		strcpy(app_set->account_info.webuser.pw, pw);
		app_set->account_info.webuser.lv = lv;
		app_set->account_info.webuser.authtype = authtype;

		app_set_write() ;
		return 0;
	}
	TRACE_INFO(" Can't set %s's password\n", id);

	return -1;
}

int app_set_onvif_password(char *id, char *pw, int lv)
{
	if(strcmp(id, ONVIF_DEFAULT_ID)==0) {
		strcpy(app_set->account_info.onvif.id, ONVIF_DEFAULT_ID);
		strcpy(app_set->account_info.onvif.pw, pw);
		app_set->account_info.onvif.lv = lv;

		return 0;
	}
	TRACE_INFO("ONVIF Can't set %s's password to %s\n", id, pw);

	return -1;
}
