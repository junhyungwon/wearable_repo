/******************************************************************************
 * Xenovo Black Box Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	xbx_set.h
 * @brief
 * @author	sksung
 * @section	MODIFY history
 */
/*****************************************************************************/
#ifndef _APP_SET_H_
#define _APP_SET_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include "app_main.h"
#include "app_dev.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define ENC_JPEG 					0
#define ENC_MPEG 					1
#define ENC_H264 					2

#define MAX_GOV						MAX_FPS
#define DEFAULT_FPS					MAX_FPS
#define DEFAULT_GOV					DEFAULT_FPS

#define MIN_FPS 					1
#define MAX_BITRATE 				8000 // Kbps
#define MIN_BITRATE 				512  // Kbps

#define FTP_HOUR_MIN            	0
#define FTP_HOUR_MAX            	24
#define FTP_HOUR_DEFAULT       	 	3
#define SERVER_URL_SIZE         	128

#define CFG_INVALID     			-1
#define CHAR_INVALID            	255
#define CHAR_MEMSET     			0x00

#define PARTIAL_RESET          		0
#define FULL_RESET             		1

#define MIN_BITRATE            		512
#define MAX_BITRATE            		8000
#define DEFAULT_QUALITY        		4000

#if SYS_CONFIG_VOIP
#define PBX_SERVER_ADDR         	"52.78.124.88"
#define PBX_SERVER_PORT         	6060
#define PBX_SERVER_PW           	"9999"
#endif

typedef enum {
	RATE_CTRL_VBR,
	RATE_CTRL_CBR,
	MAX_RATE_CTRL
} app_rate_ctrl_e;

typedef enum {
	RESOL_480P=0,
	RESOL_720P,
	RESOL_1080P,
	MAX_RESOL
} app_resol_e;

typedef enum {
    Q_HIGH = 0,
	Q_MID,
	Q_LOW,

	MAX_QUALITY
} app_quality_e;

#if defined(NEXXONE)
typedef enum {
    FPS_30=0,
    FPS_15,
    FPS_5,
    FPS_MAX
} app_frate_e;
#else
typedef enum {
    FPS_15=0,
    FPS_10,
    FPS_5,
    FPS_MAX
} app_frate_e;
#endif

typedef enum {
    GSN_IDX_01=0,
    GSN_IDX_02,
    GSN_IDX_03,
    GSN_IDX_04,
    GSN_IDX_05,
    GSN_IDX_OFF,
    GSN_IDX_MAX
}app_gsn_idx_e;

typedef enum {
	REC_PERIOD_01=0,	//# 1 minute
	REC_PERIOD_02,		//# 5 minutes
	REC_PERIOD_MAX
}app_rec_period_e;

typedef struct {
	int resol;
	int framerate;
	int quality;
	int rate_ctrl;
	int motion;
    int gop ;
	char reserved[36];
} app_ch_cfg_t;

typedef struct {
    int gsn;	//Gsensor sensitivity
    char reserved[40];
} app_watchdog_t;

typedef struct{
    int port;

    char ipaddr[MAX_CHAR_16];
    char id[MAX_CHAR_16];
    char pwd[MAX_CHAR_16];
    short ON_OFF ;
	char reserved[126];

} app_network_ftp_t;

typedef struct{
    int type ;		// cradle, ethernet

    char wlan_ipaddr[MAX_CHAR_16];
    char wlan_netmask[MAX_CHAR_16];
    char wlan_gateway[MAX_CHAR_16];

    char eth_ipaddr[MAX_CHAR_16];
    char eth_netmask[MAX_CHAR_16];
    char eth_gateway[MAX_CHAR_16];

    char dns_server1[MAX_CHAR_16] ;
    char dns_server2[MAX_CHAR_16] ;

    short http_port ;
    short https_port ;
    short rtsp_port ;
    short onvif_port ;

    char rtsp_name[6] ;
    short wtype ;			// wireless

    char  http_enable;		// onvif device-2.1.34 mandatory
    char  https_enable;		// onvif device-2.1.34 mandatory
    char  rtsp_enable;		// onvif device-2.1.34 mandatory
    char  enable_onvif;
	char  dnsFromDHCP;		// onvif device-2.1.8 mandatory
	char  ntpFromDHCP;		// onvif device-2.1.14 mandatory if supported NTP
	char  hostnameFromDHCP;		// onvif device-2.1.1, 2.1.3 mandatory

    char reserved[73];
} app_network_dev_t;

// # to connect, fms server info
typedef struct{
    int port;
    char ipaddr[SERVER_URL_SIZE];
    short ON_OFF ;
    char reserved[126];

}app_network_srv_t;

typedef struct{
    int en_key;						/* encryption mode */
    char ssid[MAX_CHAR_32 + 3];
    char pwd[MAX_CHAR_64 + 1];
	int stealth ;
	char reserved[72];
} app_network_wifiap_t;

typedef struct {
    char fw_ver[MAX_CHAR_32];
    char hw_ver[MAX_CHAR_32];
    char deviceId[MAX_CHAR_32] ;
    char osd_set ;
    char P2P_ON_OFF;
    char p2p_id[MAX_CHAR_32] ;
    char p2p_passwd[MAX_CHAR_32] ;
	char uid[MAX_CHAR_32];
	int	 dev_cam_ch;		//# NEXXONE: 1, Any others: 4
	char reserved[26];
} app_system_t;
//} __attribute__((packed)) app_system_t;

typedef struct {
	int period_idx;
	int overwrite;
    char pre_rec;         // on, off
    char auto_rec;        // on, off
    char audio_rec;       // on, off   
	 
	char reserved[17];
}app_rec_cfg_t;

typedef struct{
    short ON_OFF ;
    char userId[MAX_CHAR_32] ;
    char passwd[MAX_CHAR_32] ;
    char serveraddr[MAX_CHAR_64] ;
    char hostname[MAX_CHAR_32] ;
    short interval ;
} app_ddnscfg_t;

typedef struct {
    short time_zone ;        // 0 ~ 26 = ( -12 ~ +14 ) + 12 ; BCZ : unset value is -1 in PCViewer
    char time_server[MAX_CHAR_32] ;

    char daylight_saving;             // 0:false, 1:automatically for daylight saving time changes.
    char timesync_type;               // 0:computer(same as manual), 1:syncronize with NTP server
    char time_zone_abbr[6] ;

    char reserved[MAX_CHAR_32-2-6] ; // used 2 byte for daylightsaving, timesync_type,6 bytes(1 for \0) for time_zone_abbr
} app_timecfg_t ;  // 66

typedef struct {
    unsigned char       lv;        // 0:administrator, 1:operator, 2:viewer, 3:guest
    char                id[MAX_CHAR_32];
    char                pw[MAX_CHAR_32];
} app_onvifuser_t; // 65

typedef struct {
    unsigned char       authtype;      // 0:basic, 1:digest
    char                id[MAX_CHAR_32];
    char                pw[MAX_CHAR_32];
    unsigned char       lv;        // 0:administrator, 1:operator, 2:viewer, 3:guest
} app_webuser_t; // 66

typedef struct {
    short ON_OFF ;
    unsigned short enctype ; // AES : 0 others : increase no...
    char rtsp_userid[MAX_CHAR_32] ;
    char rtsp_passwd[MAX_CHAR_32] ;

    app_webuser_t           webuser;   //66 

    app_onvifuser_t        onvif; // 65

    char reserved[121] ;
} app_account_t ; // size:320

#if SYS_CONFIG_VOIP
typedef struct {
    char  ipaddr[MAX_CHAR_16];
    short port ;
    char  userid[MAX_CHAR_16] ;
    char  passwd[MAX_CHAR_16] ;
    char  peerid[MAX_CHAR_16] ;
    short private_network_only;
} app_voip_t; // 68 
#endif

typedef struct {
	app_ch_cfg_t			ch[TOT_CH_INFO]; // 4 + 1 = records + streaming
	app_watchdog_t			wd;

	app_network_dev_t		net_info;	//# network information for device
	app_network_srv_t		srv_info;	//# server network information for connection.
	app_network_ftp_t		ftp_info;	//# ftp server information
	app_network_wifiap_t	wifiap;		//# wifi ap information for client-mode

	app_system_t	   		sys_info;
	app_rec_cfg_t			rec_info;
    app_ddnscfg_t           ddns_info;
    app_timecfg_t           time_info;
    
    app_account_t           account_info;

#if SYS_CONFIG_VOIP
    app_voip_t              voip; // 68
	char reserved[406];   // 1024 - 164 (ddns) - 66 (time) - 320(account) - 68(voip)
#else
	char reserved[474];   // 1024 - 164 (ddns) - 66 (time) - 320(account)
#endif
	
} app_set_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
int app_set_open(void);
int app_set_write(void);
int get_bitrate_val(int quality, int resol);
int get_fps_val(app_frate_e idx);
int get_frame_size(int resol, int *wi, int *he);
int GetSvrMacAddress(char *MacAddr) ;
int DefaultGetMac(char *MacAddr) ;
void app_setting_reset(int type) ; // for onvif 
int app_set_web_password(char *id, char *pw, int lv, int authtype);
int app_set_onvif_password(char *id, char *pw, int lv);

extern app_set_t *app_set;

#endif	/* _APP_SET_H_ */
