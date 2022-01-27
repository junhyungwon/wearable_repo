#ifndef __CGI_PARAM_H__
#define __CGI_PARAM_H__
////////////////////////////////////////////////////////////////////////////////

enum _enumUserLevel{
    USER_LV_ADMINISTRATOR=0,
    USER_LV_OPERATOR,
    USER_LV_VIEWER,
    USER_LV_MAX
};

typedef struct _tagCgiParam {
    char *name;
    char *value;
}T_CGIPRM;

/* start of servers settings */
typedef struct _tagBackupServer {
	int  enable;
    int  upload_files;      // Type of record files to upload to backup server. 0:all, 1:event
	char serveraddr[32];
	char id[32];
	char pw[32];
	int  port;
}T_CGI_BACKUP_SERVER;

typedef struct _tagFotaServer {
	int  enable;
    int  server_info; // backup server와 같은지, 아니면 직접입력할건지
	char serveraddr[32];
	char id[32];
	char pw[32];
	int  port;
}T_CGI_FOTA_SERVER;

typedef struct _tagMediaServer {
	int  enable;
    int  use_full_path_url;  // 따로 입력할건지, 주소를 통으로 입력할건지
	char full_path_url[64];
	char serveraddr[16];
	int  port;
	char id[32];
	char pw[32];
}T_CGI_MEDIA_SERVER;

typedef struct _tagManageServer {
	int  enable;
	char serveraddr[128];				// ref) SERVER_URL_SIZE on app_set.h
	int  port;
}T_CGI_MANAGE_SERVER;

typedef struct _tagDdnsInfo {
	int  enable;
	char serveraddr[64];
	char hostname[32];
	char id[32];
	char pw[32];
}T_CGI_DDNS_INFO;
typedef struct _tagDnsInfo{
	char server1[32];
	char server2[32];
}T_CGI_DNS_INFO;
typedef struct _tagNtpInfo {
	int  enable;
	char serveraddr[32];
}T_CGI_NTP_INFO;

typedef struct _tagCgiOnvifConfig {
    int  enable; // 1:enable, 0:disable
	int  lv;
    char id[32]; // fixed "admin"
    char pw[32];
}T_CGI_ONVIF_CONFIG;

typedef struct _tagCgiP2pServerConfig {
    int  enable;
    char uid[32];
    char username[32];
    char password[32];
}T_CGI_P2PSERVER_CONFIG;

typedef struct _tagCgiVoipConfig{
    unsigned short private_network_only;
    int enable;
    int use_stun;
    char ipaddr[16];
    unsigned short  port;
    char userid[16];
    char passwd[16];
    char peerid[16];
}T_CGI_VOIP_CONFIG;

typedef struct _tagCgiServersConfig {
	T_CGI_BACKUP_SERVER bs;    //ftp
	T_CGI_FOTA_SERVER   fota;          // remote update
	T_CGI_MEDIA_SERVER  mediaserver;   // media server(rtmp)
	T_CGI_MANAGE_SERVER ms;            // manage server
	T_CGI_DDNS_INFO     ddns;
	T_CGI_DNS_INFO      dns;
	T_CGI_NTP_INFO      ntp;
	int					time_zone;
    char 				time_zone_abbr[6] ; // timezone 문자열...
	int					daylight_saving;
	T_CGI_ONVIF_CONFIG  onvif;
    T_CGI_P2PSERVER_CONFIG p2p;
    T_CGI_VOIP_CONFIG voip;
	
}T_CGI_SERVERS_CONFIG;
/* end of servers settings */

typedef struct _tagCgiRecordingConfig {
    int pre_rec;        // on or off
    int auto_rec;         // enable on startup
    int audio_rec;         // on / off
    int interval;       // rec interval minutes
    int overwrite;      // on,off;
}T_CGI_RECORDING_CONFIG;

typedef struct _tagCgiOperationConfiguration {
    T_CGI_RECORDING_CONFIG rec;
    int display_datetime;
    //T_CGI_P2PSERVER_CONFIG p2p;
}T_CGI_OPERATION_CONFIG;

typedef struct _tagCgiEncoderSettings{
    int codec;                  // 264, mjpeg
    int res;                    // 0:1080p, 1:720p, 2:480p
    int fps;                    // HIGH MEDIUM LOW
    int bps;                    // HIGH MEDIUM LOW
    int gop;        // 1~30
    int rc;           // VBR CBR
}T_CGI_ENCODER_SETTINGS;

typedef struct _tagCgiVideoQuality{
    T_CGI_ENCODER_SETTINGS rec;         // recording
    T_CGI_ENCODER_SETTINGS stm;         // streaming
}T_CGI_VIDEO_QUALITY;


typedef struct _tagCgiUser {
    char id[256];         // ID
    char pw[256];         // PW
    int  lv;              // 0:adminitrator, 1:operator, 2:viewer
    int  authtype;        // 0:basic, 1:digest(default)

}T_CGI_USER;

typedef struct _tagCgiOnvifUser {
    char id[256];         // ID
    char pw[256];         // PW
    int  lv;              // 0:adminitrator, 1:operator, 2:viewer
}T_CGI_ONVIF_USER;

typedef struct _tagCgiNetworkInterface{
    int  addr_type;          // static or dhcp
    char ipv4[32];       // IP
    char gw[32];         // Gateway
    char mask[32];       // Net MASK
}T_CGI_NETWORK_INTERFACE;

typedef struct _tagCgiAccount {
    char id[128];         // ID
    char pw[128];         // PW
}T_CGI_ACCOUNT;

typedef struct _tagCgiRtspConfig{
    int  enable;
	int	 enctype;
    char id[128];         // ID
    char pw[128];         // PW
}T_CGI_RTSP_CONFIG;

typedef struct _tagCgiNetworkConfiguration {
    T_CGI_NETWORK_INTERFACE wireless;
    T_CGI_NETWORK_INTERFACE cradle;
    T_CGI_ACCOUNT wifi_ap;
    int       live_stream_account_enable;
	int		  live_stream_account_enctype;
    T_CGI_ACCOUNT live_stream_account;
}T_CGI_NETWORK_CONFIG;
typedef struct _tagCgiNetworkConfiguration2 {
    T_CGI_NETWORK_INTERFACE wireless;
    T_CGI_NETWORK_INTERFACE cradle;
    T_CGI_ACCOUNT wifi_ap;
    T_CGI_ACCOUNT wifi_ap_list[4];
    int       wifi_ap_multi;                // 1:enable, 0:disable
    int       live_stream_account_enable;
	int		  live_stream_account_enctype;
    T_CGI_ACCOUNT live_stream_account;
}T_CGI_NETWORK_CONFIG2;

typedef struct _tagCgiSystemConfiguration{
	char model[32];
	char fwver[32];
	char devid[32];
	char mac[32];
	char uid[32];
}T_CGI_SYSTEM_CONFIG;

typedef struct _tagCgiUserConfig {
	T_CGI_ACCOUNT      web;
	T_CGI_ONVIF_CONFIG onvif;
	T_CGI_RTSP_CONFIG  rtsp;
}T_CGI_USER_CONFIG;

typedef struct _tagCgiSystemInfo{
	char model[32];
	char fwver[32];
    int ftp;
    int onvif;
    int p2p;
    int https;
    int rec;
}T_CGI_SYSTEM_INFO;

////////////////////////////////////////////////////////////////////////////////
#endif//__CGI_PARAM_H__
