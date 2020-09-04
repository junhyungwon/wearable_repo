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

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef enum{
	CFG_NAND=0,
	CFG_MMC,
	CFG_MAX
} app_cfg_e;

const char* const RTSP_DEFAULT_ID   = "admin";
const char* const RTSP_DEFAULT_PW   = "admin";
const char* const P2P_DEFAULT_ID    = "linkflow";
const char* const P2P_DEFAULT_PW    = "12345678";
const char* const WEB_DEFAULT_ID    = "admin";
const char* const WEB_DEFAULT_PW    = "admin";
const char* const ONVIF_DEFAULT_ID  = "admin";
const char* const ONVIF_DEFAULT_PW  = "admin";

static int qbr[MAX_RESOL][MAX_QUALITY] = {
	{ 2000, 1000, 512},		//# 480p
	{ 4000, 3000, 2000},	//# 720p
	{ 8000, 6000, 4000}	    //# 1080p
};

/*
 * Nexx360/Fitt360 --> 15,10,5 Fps
 * Nexxone ----------> 30,15,5
 */ 
static int FPS_IDX[FPS_MAX]	= {30, 15, 5};
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
#define CFG_RE       ".re" //setting default file
static char *find_reset(void) //setting default file search
{
	char path[255];
	static char	extPath[255];
	glob_t globbuf;

	memset(&globbuf, 0, sizeof(glob_t));

	sprintf(path, "%s/*%s", SD_MOUNT_PATH, CFG_RE);
    if (glob(path, GLOB_DOOFFS, NULL, &globbuf) == 0)
   	{
   		if(globbuf.gl_pathc > 0)
		{
			strcpy(extPath,globbuf.gl_pathv[0]);
			globfree(&globbuf);
			return extPath;
		}
   	}

	globfree(&globbuf);

	return NULL;
}

static size_t get_cfg_size (const char * file_name)
{
    struct stat sb;

    if (stat(file_name, &sb) != 0) {
        eprintf("Failed the CFG file size.\n");
        return 0;
    }

    return sb.st_size;
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

    memset(app_set->ftp_info.reserved, CFG_INVALID, 126);

	//# Wifi AP information
    app_set->wifiap.en_key = CFG_INVALID ;
    memset(app_set->wifiap.ssid, CHAR_MEMSET, MAX_CHAR_32 + 3) ;
    memset(app_set->wifiap.pwd, CHAR_MEMSET, MAX_CHAR_64 + 1) ;
	app_set->wifiap.stealth = CFG_INVALID ;
    memset(app_set->wifiap.reserved, CFG_INVALID, 72) ;

	//# Versions
    memset(app_set->sys_info.fw_ver, CHAR_MEMSET, MAX_CHAR_32);
    memset(app_set->sys_info.hw_ver, CHAR_MEMSET, MAX_CHAR_32);
    memset(app_set->sys_info.deviceId, CHAR_MEMSET, MAX_CHAR_32);
    app_set->sys_info.osd_set = CFG_INVALID; 
    app_set->sys_info.P2P_ON_OFF = CFG_INVALID; 
    memset(app_set->sys_info.p2p_id, CHAR_MEMSET, MAX_CHAR_32); 
    memset(app_set->sys_info.p2p_passwd, CHAR_MEMSET, MAX_CHAR_32); 
    memset(app_set->sys_info.uid, CHAR_MEMSET, MAX_CHAR_32); 
    memset(app_set->sys_info.reserved, CFG_INVALID, 30) ;

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

    app_set->account_info.webuser.authtype = 0 ;
	app_set->account_info.webuser.lv = 0 ;
	memset(app_set->account_info.webuser.id, CHAR_MEMSET, MAX_CHAR_32) ; 
	memset(app_set->account_info.webuser.pw, CHAR_MEMSET, MAX_CHAR_32) ;

	app_set->account_info.onvif.lv = 0 ;
	memset(app_set->account_info.onvif.id, CHAR_MEMSET, MAX_CHAR_32) ; 
	memset(app_set->account_info.onvif.pw, CHAR_MEMSET, MAX_CHAR_32) ;

    memset(app_set->account_info.reserved, CFG_INVALID, 121) ; // WOW, This is a super TRAP...

	// VOIP size : 66 
    app_set->voip.port = CFG_INVALID ;
    memset(app_set->voip.ipaddr, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->voip.userid, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->voip.passwd, CHAR_MEMSET, MAX_CHAR_16);
    memset(app_set->voip.peerid, CHAR_MEMSET, MAX_CHAR_16);

    memset(app_set->reserved, CFG_INVALID, 408) ;
//    memset(app_set->reserved, CFG_INVALID, 474) ;  -66 (voip)
//    memset(app_set->reserved, CFG_INVALID, 794) ;
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
        printf("Mac:%s len:%d\n", mac_address, readlen) ;
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
        printf( "Fatal error: Failed to get local host's MAC address\n" );
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
	printf("\n");

    printf("pset->wifiap.en_key = %d\n", pset->wifiap.en_key);
    printf("pset->wifiap.ssid   = %s\n", pset->wifiap.ssid);
    printf("pset->wifiap.pwd    = %s\n", pset->wifiap.pwd);
    printf("pset->wifiap.stealth    = %d\n", pset->wifiap.stealth);
	printf("\n");

    printf("pset->sys_info.fw_ver   = %s\n", pset->sys_info.fw_ver);
    printf("pset->sys_info.hw_ver   = %s\n", pset->sys_info.hw_ver);
    printf("pset->sys_info.deviceId = %s\n", pset->sys_info.deviceId); 
    printf("pset->sys_info.osd_set  = %d\n", pset->sys_info.osd_set); 
    printf("pset->sys_info.P2P_ON_OFF  = %d\n", pset->sys_info.P2P_ON_OFF); 
    
    printf("pset->sys_info.p2p_id  = %s\n", pset->sys_info.p2p_id); 
    printf("pset->sys_info.p2p_passwd  = %s\n", pset->sys_info.p2p_passwd); 
    printf("pset->sys_info.uid  = %s\n", pset->sys_info.uid); 

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

    printf("pset->voip.ipaddr = %s\n", pset->voip.ipaddr);
    printf("pset->voip.port   = %d\n", pset->voip.port);
    printf("pset->voip.userid = %s\n", pset->voip.userid);
    printf("pset->voip.passwd = %s\n", pset->voip.passwd);
    printf("pset->voip.peerid = %s\n", pset->voip.peerid);

	printf("\n");

    return 0;
}

static void cfg_param_check_nexx(app_set_t *pset)
{
    char enc_ID[32] = {0, } ;
    char enc_Passwd[32] = {0, } ;
    char dec_ID[32] = {0, } ;
    char dec_Passwd[32] = {0, } ;
    char MacAddr[12]  ;
    char compbuff[32];
	char ver_compare[16] ;

	int ich=0, channels = 0, check_version = 0, i;
	int default_fps;
	
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
		    pset->ch[ich].framerate	= DEFAULT_FPS;

        printf("pset->ch[ich].framerate = %d\n",pset->ch[ich].framerate) ;
        printf("pset->ch[ich].quality = %d\n",pset->ch[ich].quality) ;

		if(pset->ch[ich].quality < MIN_BITRATE || pset->ch[ich].quality	> MAX_BITRATE)
		    pset->ch[ich].quality = DEFAULT_QUALITY;
		
		if(pset->ch[ich].rate_ctrl	!= RATE_CTRL_VBR && pset->ch[ich].rate_ctrl	!= RATE_CTRL_CBR)
		    pset->ch[ich].rate_ctrl	= RATE_CTRL_VBR;

        if(ich == MODEL_CH_NUM)
        {
	     	//# streaming channel...
            if(pset->ch[ich].resol < RESOL_480P || pset->ch[ich].resol > RESOL_1080P)
	            pset->ch[ich].resol= RESOL_720P;
        }

        printf("pset->ch[ich].quality = %d\n",pset->ch[ich].quality) ;
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

    if(((int)pset->net_info.dns_server1[0]) == CHAR_INVALID)
		strcpy(pset->net_info.dns_server1, "8.8.8.8"); // google dns server address

    if(((int)pset->net_info.dns_server2[0]) == CHAR_INVALID)
		strcpy(pset->net_info.dns_server2, "168.154.160.4");  // Microsoft dns
        
    if(pset->net_info.type < NET_TYPE_STATIC || pset->net_info.type > NET_TYPE_DHCP)
        pset->net_info.type = NET_TYPE_STATIC ;

    if(pset->net_info.http_port <= 0 || pset->net_info.http_port >= 65535)
        pset->net_info.http_port = 80 ;

    if(pset->net_info.https_port <= 0 || pset->net_info.https_port >= 65535)
        pset->net_info.https_port = 443 ;

    if(pset->net_info.rtsp_port <= 0 || pset->net_info.rtsp_port >= 65535)
        pset->net_info.rtsp_port = 8551 ;
		
    if(pset->net_info.onvif_port <= 0 || pset->net_info.onvif_port >= 65535)
        pset->net_info.onvif_port = 9221 ;

    if(((int)pset->net_info.rtsp_name[0]) == CHAR_INVALID)
        strcpy(pset->net_info.rtsp_name, "all") ; 

	if(pset->net_info.wtype <= CFG_INVALID)
        pset->net_info.wtype = NET_TYPE_DHCP ;

	if(pset->net_info.http_enable <= CFG_INVALID || pset->net_info.http_enable > 1 ) 
		pset->net_info.http_enable   = 1;
	if(pset->net_info.https_enable <= CFG_INVALID ||pset->net_info.https_enable > 1) 
		pset->net_info.https_enable = 0; // not supported
	if(pset->net_info.rtsp_enable <= CFG_INVALID || pset->net_info.rtsp_enable > 1)
		pset->net_info.rtsp_enable   = 1;
	if(pset->net_info.enable_onvif <= CFG_INVALID || pset->net_info.enable_onvif > 1) 
		pset->net_info.enable_onvif = 1;
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

	if(((int)pset->sys_info.deviceId[0] == CHAR_INVALID) || strcmp(pset->sys_info.deviceId, "_MACADDRESS_")== 0)
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

    if(pset->sys_info.P2P_ON_OFF < OFF || pset->sys_info.P2P_ON_OFF > ON)
        pset->sys_info.P2P_ON_OFF = ON ; 

    if((int)pset->sys_info.p2p_id[0] == CHAR_INVALID)
        strcpy(pset->sys_info.p2p_id, P2P_DEFAULT_ID) ;

    if((int)pset->sys_info.p2p_passwd[0] == CHAR_INVALID)
        strcpy(pset->sys_info.p2p_passwd, P2P_DEFAULT_PW) ;

	strcpy(compbuff, MODEL_NAME) ;

	if((int)pset->sys_info.uid[0] == CHAR_INVALID)
    {
		if(strcmp(compbuff, "FITT360_Security") != 0)
		{
            strcpy(app_set->sys_info.uid ,"LFS-LSCS-A1-xxxx");
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
/*
	{
	    if(pset->wifiap.en_key != ON && pset->wifiap.en_key != OFF)
		    pset->wifiap.en_key = ON;

	    if((int)pset->wifiap.ssid[0] == CHAR_INVALID || (int)pset->wifiap.ssid[0] == 0)
		    strcpy(pset->wifiap.ssid, "NEXX360_AP");

	    if((int)pset->wifiap.stealth == CFG_INVALID )
	        pset->wifiap.stealth = OFF ;

        #if 0
	    if((int)pset->wifiap.pwd[0] == CHAR_INVALID || (int)pset->wifiap.pwd[0] == 0)
        #else
	    if((int)pset->wifiap.pwd[0] == CHAR_INVALID ) // bk 2020.02.26 allow null password
        #endif
		    strcpy(pset->wifiap.pwd,"12345678");
    }
*/
	if(pset->rec_info.period_idx < REC_PERIOD_01 && pset->rec_info.period_idx >= REC_PERIOD_MAX)
		pset->rec_info.period_idx = REC_PERIOD_01;

	if(pset->rec_info.overwrite != ON && pset->rec_info.overwrite != OFF)
		pset->rec_info.overwrite = ON;

    if(pset->rec_info.auto_rec != ON && pset->rec_info.auto_rec != OFF)
	    pset->rec_info.auto_rec = ON ; //# NEXXONE --> ON
		
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

    if(pset->time_info.time_zone < 0 || pset->time_info.time_zone>26)
		pset->time_info.time_zone = TIME_ZONE + 12;

    if((int)pset->time_info.time_server[0] == CHAR_INVALID)
        strcpy(pset->time_info.time_server, "time.google.com") ;

    if(pset->time_info.daylight_saving < 0 || pset->time_info.daylight_saving > 1 )
        pset->time_info.daylight_saving = 0 ;

    if(  pset->time_info.timesync_type < 0 || pset->time_info.timesync_type > 2 )
        pset->time_info.timesync_type = 1 ;

    if((int)pset->account_info.ON_OFF <= CFG_INVALID || (int)pset->account_info.ON_OFF > 1 )
        pset->account_info.ON_OFF = ON ;

    printf("pset->account_info.ON_OFF		= %d\n", pset->account_info.ON_OFF) ;
 
    if(pset->account_info.enctype <= CFG_INVALID || pset->account_info.enctype > 1)
        pset->account_info.enctype = 0 ;

    if((int)pset->account_info.rtsp_userid[0] == CHAR_INVALID || (int)pset->account_info.rtsp_userid[0] == 0)
    {
        if(pset->account_info.enctype) // AES type 
        { 
            encrypt_aes(RTSP_DEFAULT_ID, enc_ID, 32) ;
            strncpy(pset->account_info.rtsp_userid, enc_ID, strlen(enc_ID)) ;
        }
		else
		{
            strcpy(pset->account_info.rtsp_userid, RTSP_DEFAULT_ID) ;
		}
    }    

    if((int)pset->account_info.rtsp_passwd[0] == CHAR_INVALID || (int)pset->account_info.rtsp_passwd[0] == 0)
    {
        if(pset->account_info.enctype) // AES type 
        { 
            encrypt_aes(RTSP_DEFAULT_PW, enc_Passwd, 32) ;
            strncpy(pset->account_info.rtsp_passwd, enc_Passwd, strlen(enc_Passwd)) ;
        } 
		else
		{
            strcpy(pset->account_info.rtsp_passwd, RTSP_DEFAULT_PW) ;
		}
    }

	// webuser
    if((int)pset->account_info.webuser.id[0] == CHAR_INVALID || (int)pset->account_info.webuser.id[0] == 0){
		strcpy(pset->account_info.webuser.id, WEB_DEFAULT_ID);
		strcpy(pset->account_info.webuser.pw, WEB_DEFAULT_PW);
		pset->account_info.webuser.lv = 0;       // 0:Administrator default
		pset->account_info.webuser.authtype = 0; // 0:base64(default) 1:digest
	}
    if((int)pset->account_info.webuser.pw[0] == CHAR_INVALID || (int)pset->account_info.webuser.pw[0] == 0){
		strcpy(pset->account_info.webuser.pw, WEB_DEFAULT_PW);
		pset->account_info.webuser.lv = 0;
		pset->account_info.webuser.authtype = 0; // default 0
	}
    if(pset->account_info.webuser.lv <= CFG_INVALID || pset->account_info.webuser.lv > 2) {
		pset->account_info.webuser.lv       = 0;
		pset->account_info.webuser.authtype = 0; // default 0
	}
    if(pset->account_info.webuser.authtype <= CFG_INVALID || pset->account_info.webuser.authtype > 1)
		pset->account_info.webuser.authtype = 0;

    printf("pset->account_info.webuser.id		= %s\n", pset->account_info.webuser.id) ;
    printf("pset->account_info.webuser.pw		= %s\n", pset->account_info.webuser.pw) ;
    printf("pset->account_info.webuser.lv		= %d\n", pset->account_info.webuser.lv) ;
    printf("pset->account_info.webuser.authtype = %d\n", pset->account_info.webuser.authtype) ;

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
    printf("pset->account_info.onvif.lv		= %d\n", pset->account_info.onvif.lv) ;
    printf("pset->account_info.onvif.id		= %s\n", pset->account_info.onvif.id) ;
    printf("pset->account_info.onvif.pw		= %s\n", pset->account_info.onvif.pw) ;
	
	if((int)pset->voip.ipaddr[0] == CHAR_INVALID || (int)pset->voip.ipaddr[0] == 0)
		strcpy(pset->voip.ipaddr, "0.0.0.0");

	if(pset->voip.port <= CFG_INVALID) pset->voip.port	= 9999;

	if((int)pset->voip.userid[0] == CHAR_INVALID || (int)pset->voip.userid[0] == 0)
		strcpy(pset->voip.userid, "");

	if((int)pset->voip.passwd[0]  == CHAR_INVALID || (int)pset->voip.passwd[0] == 0)
		strcpy(pset->voip.passwd, "");

	if((int)pset->voip.peerid[0] == CHAR_INVALID || (int)pset->voip.peerid[0] == 0)
		strcpy(pset->voip.peerid, "");
		
	if(0 == access("/mmc/show_all_cfg", F_OK))
		show_all_cfg(pset); // BKKIM

}

static void app_set_version_read(void)
{
	if (app_set != NULL)
	{
		sprintf(app_set->sys_info.fw_ver, "%s", FITT360_SW_VER);
		sprintf(app_set->sys_info.hw_ver, "%s", FITT360_HW_VER);
	}
}

static int cfg_read(int is_mmc, char* cfg_path)
{
    int readSize=0, app_set_size=0;
    int saved_cfg_size=0;

	if(is_mmc){
		if (!app_cfg->ste.b.mmc || app_cfg->ste.b.mmc_err) {
			eprintf("#### NO INSERTED SD CARD !! ####\n");
			return EFAIL;
		}
	}

    if (-1 == access(cfg_dir[is_mmc], 0)) {
        mkdir(cfg_dir[is_mmc], 0775);
        chmod(cfg_dir[is_mmc], 0775);

		eprintf("#### NOT EXIST CFG DIR [%s] !! ####\n", cfg_path);
		return EFAIL;
    }

    if (-1 == access(cfg_path, 0)) {
		//# cpoy nand cfg file to SD
		eprintf("#### NOT EXIST CFG FILE IN [%s] !! ####\n", cfg_path);
		return EFAIL;
	}

	app_set_size 	= sizeof(app_set_t);		//# current version cfg_size
	saved_cfg_size	= get_cfg_size(cfg_path);	//# sd cfg_size (old version)

	if (app_set_size != saved_cfg_size) {
		//# cfg is different
		eprintf("\n #### [%s] DIFF CFG SIZE - app_set:%d / read:%d !!! #### \n", cfg_path, app_set_size, saved_cfg_size);
		return EFAIL;
	}
	else{
	    //#--- ucx app setting param
    	OSA_fileReadFile(cfg_path, (Uint8*)app_set, app_set_size, (Uint32*)&readSize);
		if(readSize == 0 || readSize != app_set_size) {
			eprintf(" #### CFG Read Failed [%s] !! ####\n", cfg_path);
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

    char dec_ID[32] = {0, } ;
    char dec_Passwd[32] = {0, } ;

    app_set_t tmp_set ;

	int ich=0, channels = 0;

	printf(" [CFG] - SET DEFAULT CFG... !!! MODEL_NAME=%s\n", MODEL_NAME);

    if (app_set == NULL);
        app_set = (app_set_t *)&app_sys_set;

    if (!default_type)
        memcpy(&tmp_set, app_set, sizeof(app_set_t)) ;

#if defined(NEXXONE) || defined(NEXX360)
	//# Encoding cfg per channel
	channels = MODEL_CH_NUM+1;

	for (ich = 0; ich < channels; ich++)
	{
		app_set->ch[ich].resol		= RESOL_720P;
		app_set->ch[ich].framerate	= DEFAULT_FPS;
		app_set->ch[ich].quality	= DEFAULT_QUALITY;
		app_set->ch[ich].rate_ctrl	= RATE_CTRL_VBR;
		app_set->ch[ich].motion 	= OFF;
		app_set->ch[ich].gop 	    = DEFAULT_FPS;
	}
#else
	/* FITT360 (index ±¸Á¶) */
	channels = MODEL_CH_NUM+1;

	for (ich = 0; ich < channels; ich++)
	{
		app_set->ch[ich].resol		= RESOL_720P;
		app_set->ch[ich].framerate	= FPS_15;
		app_set->ch[ich].quality	= Q_HIGH;
		app_set->ch[ich].rate_ctrl	= RATE_CTRL_VBR;
		app_set->ch[ich].motion 	= OFF;
		app_set->ch[ich].gop 	    = DEFAULT_FPS;
	}
#endif
	
	app_set->wd.gsn = GSN_IDX_03;

	/******************** begin of net_info ********************/
    if (default_type)  // FULL reset or hw reset
    {
    	//# Network information for device
        app_set->net_info.type = NET_TYPE_STATIC ;
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

    app_set->net_info.http_port  = 80;
    app_set->net_info.https_port = 443;
    app_set->net_info.rtsp_port  = 8551;
    app_set->net_info.onvif_port = 9221;

    strcpy(app_set->net_info.rtsp_name, "all") ;
    app_set->net_info.wtype = NET_TYPE_DHCP ;  // wifi dhcp(from tethering of phone or from AP) or static

    app_set->net_info.http_enable  = 1;
    app_set->net_info.https_enable = 0;			// not supported
    app_set->net_info.rtsp_enable  = 1;
    app_set->net_info.enable_onvif = 1;
    app_set->net_info.dnsFromDHCP  = 0;
    app_set->net_info.ntpFromDHCP  = 0;
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

	//# Wifi AP information
    app_set->wifiap.en_key = ON;
    strcpy(app_set->wifiap.ssid, "AP_SSID") ;
    strcpy(app_set->wifiap.pwd,"AP_PASSWORD") ;
	app_set->wifiap.stealth = OFF;

	app_set_version_read();

    if (!DefaultGetMac(MacAddr)) {
        strncpy(app_set->sys_info.deviceId, MacAddr, 12);
    }
    else {
        printf( "Fatal error: Failed to get local host's MAC address\n" );
    }

    app_set->sys_info.osd_set = ON ;
    app_set->sys_info.P2P_ON_OFF = ON ;
	app_set->sys_info.dev_cam_ch = MODEL_CH_NUM;
    strcpy(app_set->sys_info.p2p_id,     P2P_DEFAULT_ID) ; 
    strcpy(app_set->sys_info.p2p_passwd, P2P_DEFAULT_PW) ;

	//# rec information
	app_set->rec_info.period_idx 	= REC_PERIOD_01;
	app_set->rec_info.overwrite 	= ON;
    app_set->rec_info.auto_rec      = ON ;
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

#if defined(NEXXONE) || defined(NEXX360)
	app_set->account_info.ON_OFF = ON;
#else
	app_set->account_info.ON_OFF = OFF;
#endif
    
    app_set->account_info.enctype = 0 ;
    if(app_set->account_info.enctype) // 1, AES encryption
	{
        encrypt_aes(RTSP_DEFAULT_ID, enc_ID, 32) ;
        strncpy(app_set->account_info.rtsp_userid, enc_ID, strlen(enc_ID)) ;
        encrypt_aes(RTSP_DEFAULT_PW, enc_Passwd, 32) ;
        strncpy(app_set->account_info.rtsp_passwd, enc_Passwd, strlen(enc_Passwd)) ;
	}
	else // 0 do not encryption
	{
        strcpy(app_set->account_info.rtsp_userid, RTSP_DEFAULT_ID);
        strcpy(app_set->account_info.rtsp_passwd, RTSP_DEFAULT_PW);
	}

	app_set->account_info.webuser.lv = 0;			// 0:Administrator, 1:operator?(TBD)
	app_set->account_info.webuser.authtype = 0;		// 0:basic, 1:digest(TBD)
	strcpy(app_set->account_info.webuser.id, WEB_DEFAULT_ID);
	strcpy(app_set->account_info.webuser.pw, WEB_DEFAULT_PW);

	app_set->account_info.onvif.lv = 0;	// 0:Administrator
	strcpy(app_set->account_info.onvif.id, ONVIF_DEFAULT_ID); // fixed
	strcpy(app_set->account_info.onvif.pw, ONVIF_DEFAULT_PW);

    strcpy(app_set->voip.ipaddr, "0.0.0.0");
    app_set->voip.port = 9999;
	memset((void*)app_set->voip.userid, 0x00, sizeof(app_set->voip.userid));
	memset((void*)app_set->voip.passwd, 0x00, sizeof(app_set->voip.passwd));
	memset((void*)app_set->voip.peerid, 0x00, sizeof(app_set->voip.peerid));
}

static void app_set_delete_cfg(void)
{
    char cmd[MAX_CHAR_128]={0,};

	sprintf(cmd, "rm -Rf %s", CFG_DIR_NAND);
	dev_execlp(cmd);

	if (app_cfg->ste.b.mmc) {
		sprintf(cmd, "rm -Rf %s", CFG_DIR_MMC);
		dev_execlp(cmd);
	}
	sync();
}

int app_set_open(void)
{
    int ret=SOK;
    char *pFile = NULL;
	char cmd[MAX_CHAR_128]={0,};

	//# delete cfg file
	pFile = find_reset();
	if (pFile != NULL) {
		sprintf(cmd, "rm -f %s", pFile);
		dev_execlp(cmd);

		app_set_delete_cfg();
		printf("[%s] CFG delete done!! \n", __func__);
	}

    //#--- ucx app setting param
    app_set = (app_set_t *)&app_sys_set;
	char_memset();
	
	/* 
	 * Fitt360 CFG Path 
	 *            MMC  : ---> /mmc/cfg/fbx_cfg.ini
	 *            NAND : ---> /media/nand/cfg/fbx_cfg.ini
	 * 
	 * NEXX360/NEXXONE CFG Path 
	 *            MMC  : --> /mmc/cfg/nexx_cfg.ini
	 *            NAND : --> /media/nand/cfg/nexx_cfg.ini 
	 */
#if defined(NEXXONE) || defined(NEXX360)
	if (cfg_read(CFG_MMC, NEXX_CFG_FILE_MMC) == EFAIL)	//# sd read first.
		ret = cfg_read(CFG_NAND, NEXX_CFG_FILE_NAND);	//# nand read if sd read fail
#else
	/* FITT360 */
	if (cfg_read(CFG_MMC, CFG_FILE_MMC) == EFAIL)	//# read previous setting type.
		ret = cfg_read(CFG_NAND, CFG_FILE_NAND);	//# nand read if sd read fail
#endif
	
	if (ret == EFAIL) 
	    app_set_default(FULL_RESET);
	
    app_set_delete_cfg(); // another verion setting file
	app_set_write();
	printf("done\n");

	return 0;
}

void app_setting_reset(int type)  // sw reset, hw reset(include network setting) 
{
    int ret ;

    if (type >= 0 && type < 2)
    {
        app_set_default(type);  // onvif factory default
	    app_set_write();

        ret = app_rec_state();
        if (ret)
        {
            sleep(1);
            app_rec_stop(1);
        }

//        mic_exit_state(OFF_RESET, 0);
//        app_main_ctrl(APP_CMD_EXIT, 0, 0);

		mcu_pwr_off(OFF_RESET);
    } 
}

int app_set_write(void)
{
	char path[128] ={0,};

	if (app_cfg->ste.b.mmc && !app_cfg->ste.b.mmc_err)
	{
		if (-1 == access(CFG_DIR_MMC, 0)) {
			mkdir(CFG_DIR_MMC, 0775);
			chmod(CFG_DIR_MMC, 0775);
		}

#if defined(NEXXONE) || defined(NEXX360)
		snprintf(path, sizeof(path), "%s", NEXX_CFG_FILE_MMC);
#else
		snprintf(path, sizeof(path), "%s", CFG_FILE_MMC);
#endif

		if (OSA_fileWriteFile(path, (Uint8*)app_set, sizeof(app_set_t)) != OSA_SOK) {
			eprintf("couldn't open %s file\n", path);
		}
   }

	//# save cfg in nand.
	if (-1 == access(CFG_DIR_NAND, 0)) {
		mkdir(CFG_DIR_NAND, 0775);
		chmod(CFG_DIR_NAND, 0775);
	}
	
	memset(path, 0, sizeof(path));

#if defined(NEXXONE) || defined(NEXX360)
	snprintf(path, sizeof(path), "%s", NEXX_CFG_FILE_NAND);
#else
	snprintf(path, sizeof(path), "%s", CFG_FILE_NAND);
#endif
	
	if (OSA_fileWriteFile(path, (Uint8*)app_set, sizeof(app_set_t)) != OSA_SOK) {
		eprintf("couldn't open %s file\n", path);
	}

	sync();
	printf(" [app] %s done...!\n", __func__);

	return 0;
}

int get_bitrate_val(int quality, int resol)
{
	return qbr[resol][quality];
}

int get_fps_val(app_frate_e idx)
{
	int fps;

    if (idx >= FPS_MAX)
        idx = FPS_MAX-1;

	fps = FPS_IDX[idx];

	return fps;
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
		eprintf("Invaild resol param %d\n", resol);
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

		return 0;
	}
	aprintf(" Can't set %s's password\n", id);

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
	aprintf("ONVIF Can't set %s's password to %s\n", id, pw);

	return -1;
}
