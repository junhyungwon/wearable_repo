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

#if defined (__cplusplus)
extern "C" {
#endif

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define EFAIL                   (-1)
#define MAX_CH_NUM               (4+1)

#if defined(NEXXONE)||defined(NEXX360)
#define CFG_FILE_MMC            "/mmc/cfg/nexx_cfg.ini"
#elif defined(FITT360)
#define CFG_FILE_MMC            "/mmc/cfg/fbx_cfg.ini"
#else
#error "Please, check $PRODUCT_NAME"
#endif


#define DEFAULT_FPS				15 // 20 //24

#define FTP_HOUR_MIN            0
#define FTP_HOUR_MAX            24
#define FTP_HOUR_DEFAULT        3
#define SERVER_URL_SIZE         128

#define CFG_INVALID     		-1
#define CHAR_INVALID            255
#define CHAR_MEMSET     		"UNKNOWN"

#define PARTIAL_RESET          0
#define FULL_RESET             1

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

typedef enum {
    FPS_15=0,
    FPS_10,
    FPS_5,
    FPS_MAX
} app_frate_e;

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

    char ipaddr[16];
    char id[16];
    char pwd[16];
    short ON_OFF ;
	char reserved[126];

} app_network_ftp_t;

typedef struct{
    int type ;

    char wlan_ipaddr[16];
    char wlan_netmask[16];
    char wlan_gateway[16];

    char eth_ipaddr[16];
    char eth_netmask[16];
    char eth_gateway[16];

    char dns_server1[16] ;
    char dns_server2[16] ;

    short http_port ;
    short https_port ;
    short rtsp_port ;
    short onvif_port ;

    char rtsp_name[6] ;
    short wtype ;

    char reserved[80];
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
    char ssid[32 + 3];
    char pwd[64 + 1];
	char reserved[76];
} app_network_wifiap_t;

typedef struct {
    char fw_ver[32];
    char hw_ver[32];
    char deviceId[32] ;
    char osd_set ;
    char P2P_ON_OFF;
    char p2p_id[32] ;
    char p2p_passwd[32] ;
	char reserved[62];
} app_system_t;

typedef struct {
	int period_idx;
	int overwrite;

	char reserved[20];
}app_rec_cfg_t;

typedef struct{
    short ON_OFF ;
    char userId[32] ;
    char passwd[32] ;
    char serveraddr[64] ;
    char hostname[32] ;
    short interval ;
} app_ddnscfg_t;

typedef struct {
    short time_zone ;        // 0 ~ 26 = ( -12 ~ +14 ) + 12 ; BCZ : unset value is -1 in PCViewer
    char time_server[32] ;

    char daylight_saving;             // 0:false, 1:automatically for daylight saving time changes.
    char timesync_type;               // 0:computer(same as manual), 1:syncronize with NTP server

    char reserved[32-2] ; // used 2 byte for daylightsaving, timesync_type
} app_timecfg_t ; // 66 bytes

typedef struct {
    unsigned char       lv;        // 0:administrator, 1:operator, 2:viewer, 3:guest
    char                id[32];
    char                pw[32];
} app_onvifuser_t; // 65

typedef struct {
    unsigned char       authtype;      // 0:basic, 1:digest
    char                id[32];
    char                pw[32];
    unsigned char       lv;        // 0:administrator, 1:operator, 2:viewer, 3:guest
} app_webuser_t; // 66

typedef struct {
    short ON_OFF ;
    unsigned short enctype ; // AES : 0 others : increase no...
    char rtsp_userid[32] ;
    char rtsp_passwd[32] ;

    app_webuser_t           webuser;   //66 

    app_onvifuser_t        onvif; // 65

    char reserved[121] ;
} app_account_t ;

typedef struct {
    char  ipaddr[16];
    short port ;
    char  userid[16] ;
    char  passwd[16] ;
    char  peerid[16] ;
} app_voip_t; // 66 

typedef struct {
	app_ch_cfg_t			ch[MAX_CH_NUM]; // 4 + 1
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

    app_voip_t              voip; // 66

	//char reserved[474]; // 1024 - 164 (ddns) - 66 (time) - 320(account)
	char reserved[408];   // 1024 - 164 (ddns) - 66 (time) - 320(account) - 66(voip)
} app_set_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern app_set_t *app_set;

int get_account_open(char *name, char *passwd) ;

#if defined (__cplusplus)
}
#endif


#endif	/* _APP_SET_H_ */
