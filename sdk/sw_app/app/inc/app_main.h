/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_main.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_MAIN_H_
#define _APP_MAIN_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include "app_msg.h"
#include "app_log.h"
#include "app_leds.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define NET_TYPE_STATIC 		0
#define NET_TYPE_DHCP                   1

#define MAX_CH_NUM				4
#define MAX_CH_NUM_ONEEYE		1		
#define MAX_AVI_CNT             4096 //  About 64G / 120Mbyte(1minute)

#define MAX_CHAR_10             10
#define MAX_CHAR_16             16
#define MAX_CHAR_32             32
#define MAX_CHAR_64             64
#define MAX_CHAR_100            100
#define MAX_CHAR_128            128
#define MAX_CHAR_255            255

#define ON                       1
#define OFF                      0

#define SD_MOUNT_PATH   		"/mmc"
#define NAND_PATH               "/opt"
#define NAND_ROOT               "/media/nand"
#define AVI_EXT					"avi"

#define JPEG_QUALITY            80      //# 1~100       100 is max.
#define JPEG_FPS                1      //# default fps of jpeg.

#define CFG_DIR_MMC             "/mmc/cfg"
#define CFG_FILE_MMC            "/mmc/cfg/fbx_cfg.ini"

#define CFG_DIR_NAND            "/media/nand/cfg"
#define CFG_FILE_NAND           "/media/nand/cfg/fbx_cfg.ini"

/* micom watchdog */
#define WD_ENC					(1<<0)
#define WD_DEV					(1<<1)
#define WD_MICOM            	(1<<2)
#define WD_FILE					(1<<3)
#define WD_TOT					(WD_ENC+WD_DEV+WD_MICOM+WD_FILE)

#define WD_FSCAN				(0xDAAF)

#define TVO_REC                 (1<<0)
#define TVO_TEMP                (1<<1)
#define TVO_VOLT                (1<<2)
#define TVO_GSENSOR             (1<<3)
#define TVO_MOTION              (1<<4)
#define TVO_MIC                 (1<<5)
#define TVO_BUZZER              (1<<6)
#define TVO_VERSION             (1<<7)

#define MAX_PENDING_SEM_CNT     (1)
#define FXN_TSK_PRI             (1)
#define FXN_TSK_STACK_SIZE      (0) /* 0 means system default will be used */

#define SOK                     (0)
#define EFAIL                   (-1)
#define EPARAM                  (-2)
#define EINVALID                (-3)
#define EMEM                    (-4)

typedef enum {
	STE_GPS = 0,
	STE_DTIME,

	UI_STE_MAX
} app_swosd_ste_e;

typedef enum {
	MSG_TYPE_UCX,
	MSG_TYPE_GUI,
	MSG_TYPE_LOG,

	MSG_TYPE_MAX
} ucx_msg_e;

typedef struct{
    int msgDes;
    int msgSrc;
    int msgCmd;
    int msgParm1;
    int msgParm2;
    int msgParm3;
    char msgstr[MAX_CHAR_128];
    int msgAck;
} ucx_msg_info;

typedef struct
{
    unsigned char *buf;
    int size;
} app_jpeg_t;

typedef struct {
    unsigned short  year;
    unsigned short  month;
    unsigned short  day;
    unsigned short  hour;
    unsigned short  min;
    unsigned short  sec;
    unsigned short  msec;
} datetime_t;

typedef struct{
    datetime_t time;
    app_jpeg_t evtimg_info;
} app_evt_jpg_t;

typedef struct {
    int year;
    int mon;
    int day;
    int hour;
    int min;
    int sec;
    double subseconds;
} app_time_t;

typedef union {
	struct {
		unsigned int cap		: 1;	//# capturing
		unsigned int rec		: 1;	//# recording
		unsigned int mmc		: 1;	//# detect mmc
		unsigned int busy		: 1;	//# system busy (format, update)
		unsigned int evt		: 1;    //# event recording
		unsigned int wifi_ready	: 1;    //# Wi-Fi usb detect
		unsigned int wifi_run	: 1;    //# Wi-Fi usb driver loading done
		unsigned int wifi		: 1;
		unsigned int rtsptx 	: 1;
	    unsigned int mmc_err    : 1;  	//# mmc error
	    unsigned int log        : 1;
	    unsigned int sock       : 1;
	    unsigned int shock      : 1;
        unsigned int gps        : 1;
        unsigned int dial_ready : 1; 	// LTE usb detect
        unsigned int dial_run 	: 1; 	// LTE running
        unsigned int disk_full	: 1; 	// disk full because overwrite off.
        unsigned int st_cradle  : 1;    // status of attached or detached cradle
        unsigned int pwr_off	: 1;	// check power off
        unsigned int ftp_run    : 1;    // under running ftp
        unsigned int onvifserver : 1;   
        unsigned int eth1_run   : 1;    // USB2ETHERNET running
        unsigned int web_server : 1;    // web_server
		unsigned int prerec_state  : 1; // record state before using FTP
		unsigned int nokey  	   : 1; // prevent key
	} b;
	unsigned int w;
} app_state;

typedef struct {
	int wi;			//# width
	int he;			//# height
	int fr;			//# framerate
	int br;			//# bitrate (KB)
	int rc;			//# rate ctrl(VBR/CBR)
} ch_info_t;

typedef struct {
    char wifi_mode ;
	char en_key ;
	char pwd[MAX_CHAR_32] ;
	char ssid[MAX_CHAR_32] ;
	int stealth ;
    char wlan_ipaddr[MAX_CHAR_16] ;
    char wlan_netmask[MAX_CHAR_16] ;
    char wlan_gateway[MAX_CHAR_16] ;
} wifi_apmode ;

typedef struct {
	app_thr_obj mObj;	//# main thread

	app_state ste;
    wifi_apmode wmode ;

	int hw_ver;
	int num_ch;
	int disp_dev;		//# DISP_HDMI/DISP_LCD/DISP_TVO

	ch_info_t ich[MAX_CH_NUM + 1];

    int msgqId;

	//# module
	int en_jpg;         //# mjpeg enable
	int en_rec;

	//# display
	int en_tvo;
	int en_hdmi;

    int en_sock;

	int wd_flags;		//# watchdog flags - rec, adc, temp, etc...
	int wd_tot;			//# watchdog total flag value;

    int set_state;      //# refer to app_set_state_e
    int tvo_flag;

    int evtjpg;                 //# jpg buffer copy when event such sa motion and shock.
    int evt_send;          // send flag for 4 jpegs
    int evt_type;          // distinguish event type shock and others
    char ftp_enable;
    int vid_count ; 

	char cfg_mmc_path[MAX_CHAR_128];
	char cfg_nand_path[MAX_CHAR_128];
} app_cfg_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern app_cfg_t *app_cfg;
 
/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
void app_main_ctrl(int cmd, int p0, int p1);

#endif	/* _APP_MAIN_H_ */
