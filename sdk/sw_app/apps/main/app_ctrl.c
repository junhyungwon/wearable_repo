/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_ctrl.c
 * @brief	app control functions
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "ti_vsys.h"
#include "ti_vcap.h"
#include "ti_venc.h"
#include "ti_vdis.h"
#include <errno.h>

#include "dev_common.h"
#include "dev_micom.h"
#include "dev_env.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_ctrl.h"
#include "app_dev.h"
#include "dev_wifi.h"
#include "dev_disk.h"
#include "app_set.h"
#include "app_log.h"
#include "app_rec.h"
#include "app_cap.h"
#include "app_rtsptx.h"
#include "app_file.h"
#include "app_mcu.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define FLOOR_ALIGN(val, align)  (((val) / (align)) * (align))

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    rate control (VRB/CBR)
* @section  [desc]
*****************************************************************************/
int ctrl_vid_rate(int ch, int rc, int br)
{
    char log[128] = {0, };

	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };
  
    params.rateControl = rc ;

	Venc_setDynamicParam(ch, 0, &params, VENC_RATECONTROL);  // set rate control

	if(rc == RATE_CTRL_VBR)
	{
        sprintf(log, "[APP_CTRL] --- ch %d set vid rate control to VBR ---",ch);

		params.qpMax 	= 45;
		params.qpInit 	= -1;
        params.qpMin    = 10 ;
/*
        if(br >= 8000) {
            params.qpMin = 10;
        }
		else if(br >= 6000) {
			params.qpMin 	= 10;
		}
		else if(br >= 4000) {
			params.qpMin 	= 10;  //# for VBR setting (1.8M~5.6M when bitrate is 5M)
		}
		else {
			params.qpMin 	= 22;
		}
*/
	}
	else	//# RATE_CTRL_CBR
	{
        sprintf(log, "[APP_CTRL] --- ch %d set vid rate control to CBR ---",ch);
 
		params.qpMin	= 10;//10;	//# for improve quality: 10->0
		params.qpMax 	= 40;
		params.qpInit 	= -1;
	}

	Venc_setDynamicParam(ch, 0, &params, VENC_QPVAL_P);  // set QP range

    app_log_write( MSG_LOG_WRITE, log );

	return SOK;
}

int ctrl_enc_multislice()
{
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };
    int ch = 4;

    if(app_set->ch[MAX_CH_NUM].resol == RESOL_720P)
        params.packetSize = 40;  // 5 180bytes   40  hdmi slice size = 1440bytes....
    else if(app_set->ch[MAX_CH_NUM].resol == RESOL_1080P)
        params.packetSize = 18;  //   17  hdmi slice size = 1387bytes....  18 1468
    else
        params.packetSize = 80;  // 14 189bytes  80  480p  slice size = 1080....

    Venc_setDynamicParam(ch, 0, &params, VENC_PACKETSIZE);

    return SOK ;
}

/*****************************************************************************
* @brief    set video framerate
* @section  [desc]
*****************************************************************************/

int ctrl_vid_framerate(int ch, int framerate) // framerate FPS_30 0, FPS_15 1, FPS 5 2
{
    char log[128] = {0, };
    char msg[128] = {0, } ;

	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };

    int br ;
    int default_fps = DEFAULT_FPS ;

    app_ch_cfg_t *ch_prm;

    switch(framerate)
    {
        case FPS_30 :
            sprintf(msg, "FPS_30") ; 
            app_set->ch[ch].framerate = FPS_30 ;
            break ;
        case FPS_15 :
            sprintf(msg, "FPS_15") ; 
            app_set->ch[ch].framerate = FPS_15 ;
            break ;
        case FPS_5 :
            sprintf(msg, "FPS_5") ; 
            app_set->ch[ch].framerate = FPS_5 ;
            break ;
        default :
            sprintf(msg, "FPS_30") ;
            app_set->ch[ch].framerate = FPS_30 ;
            printf("ctrl vid framerate default %s\n",msg) ;
            break ;
    }

    ch_prm = &app_set->ch[ch];

    sprintf(log, "[APP_CTRL] --- ch = %d set vid framerate %s ---",ch, msg);
    app_log_write( MSG_LOG_WRITE, log );

    br = get_bitrate_val(app_set->ch[ch].quality, ch_prm->resol);  // bitrate HIGH 0, MID 1, LOW 2
    
    // resol 1080P 0, 720P 1, 480P 2
    app_cfg->ich[ch].br = (br * framerate)/DEFAULT_FPS;
    app_cfg->ich[ch].fr = get_fps_val(ch_prm->framerate);

    params.frameRate = app_cfg->ich[ch].fr;
    params.targetBitRate = app_cfg->ich[ch].br*1000 ;

    Venc_setInputFrameRate(ch, default_fps);

    Venc_setDynamicParam(ch, 0, &params, VENC_FRAMERATE);

    return SOK ;
}

/*****************************************************************************
* @brief    set video bitrate
* @section  [desc]
*****************************************************************************/

int ctrl_vid_bitrate(int ch, int bitrate) 
{
    char log[128] = {0, };
    char msg[128] = {0, } ;
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };

    int br ;

    app_ch_cfg_t *ch_prm;

    ch_prm = &app_set->ch[ch];

    switch(bitrate)
    {
        case Q_HIGH :
            sprintf(msg, "HIGH") ;
            app_set->ch[ch].quality = Q_HIGH;
            break ;

        case Q_MID :
            sprintf(msg, "MID") ;
            app_set->ch[ch].quality = Q_MID;

            break ;

        case Q_LOW :
            sprintf(msg, "LOW") ;
            app_set->ch[ch].quality = Q_LOW;
            break ;

        default :
            app_set->ch[ch].quality = Q_LOW;
            break ;
    }

    sprintf(log, "[APP_CTRL] --- ch %d set vid bitrate %s ---",ch,  msg);
    app_log_write( MSG_LOG_WRITE, log );

    br = get_bitrate_val(bitrate, ch_prm->resol);  // bitrate HIGH 0, MID 1, LOW 2
    // resol 480P 0 720P 1 1080P 2

    app_cfg->ich[ch].br = (br * app_cfg->ich[ch].fr)/DEFAULT_FPS;

    params.targetBitRate = app_cfg->ich[ch].br*1000 ;

    Venc_setDynamicParam(ch, 0, &params, VENC_BITRATE);

    return SOK ;
}

/*****************************************************************************
* @brief    set video gop
* @section  [desc]
*****************************************************************************/
int ctrl_vid_gop_set(int ch, int gop)
{
    VENC_CHN_DYNAMIC_PARAM_S params = { 0 };

    params.intraFrameInterval = gop;

    Venc_setDynamicParam(ch, 0, &params, VENC_IPRATIO);

    app_set->ch[ch].gop = gop ;

    return SOK ;
}

/*****************************************************************************
* @brief    set video resolution
* @section  [desc]
*****************************************************************************/
int ctrl_vid_resolution(int resol_idx) 
{
    char log[128] = {0, };
    int ret ;

    //# to prevent key input
    app_cfg->ste.b.nokey = 1;
    Vdis_disp_ctrl_exit();

    ret = app_rec_state() ;
 
    if(ret)
        app_rec_stop(1);

    app_cap_stop() ;

    dev_buzz_ctrl(100, 1);
    app_msleep(200);


    if(resol_idx == RESOL_480P)
    {     
		app_set->ch[MAX_CH_NUM].resol = RESOL_720P ;
    }
    else if(resol_idx == RESOL_720P)
    {
		app_set->ch[MAX_CH_NUM].resol = RESOL_1080P ;
    }
    else if(resol_idx == RESOL_1080P)
    {
		app_set->ch[MAX_CH_NUM].resol = RESOL_480P ;
    }   

    Vdis_disp_ctrl_init(app_set->ch[MAX_CH_NUM].resol);

    app_cap_start();    
    if(ret)
    {
        app_rec_start();
    }

    app_rtsptx_stop_start() ;

    if(!app_set->sys_info.osd_set)
        ctrl_swosd_enable(STE_DTIME, 0, 0) ;  // osd disable

	switch(app_set->ch[MAX_CH_NUM].resol)
    {
        case  0 :
            sprintf(log, "[APP_CTRL] --- change Display Mode to 480P ---");
            break ;
        case  1 :
            sprintf(log, "[APP_CTRL] --- change Display Mode to 720P ---");
            break ;
        case  2 :
            sprintf(log, "[APP_CTRL] --- change Display Mode to 1080P ---");
            break ;

        default :
            sprintf(log, "[APP_CTRL] --- change Display Mode to 480P ---");
            break ;
    }

    app_log_write( MSG_LOG_WRITE, log );
    app_cfg->ste.b.nokey = 0;

    return SOK ;
}

/*****************************************************************************
* @brief    get resolution function
* @section  DESC Description
*   - desc
*****************************************************************************/
int ctrl_get_resolution()
{
    return app_set->ch[MAX_CH_NUM].resol;
}

/*****************************************************************************
* @brief    set DNS SERVER
* @section  [desc]
*****************************************************************************/
int ctrl_set_DNS(char *server_addr)
{
    if(server_addr)
        strcpy(app_set->net_info.dns_server1, server_addr) ;

    return SOK ;
}

/*****************************************************************************
* @brief    set NTP SERVER
* @section  [desc]
*****************************************************************************/
int ctrl_set_NTP(char *server_addr)
{
    if(server_addr)
        strcpy(app_set->time_info.time_server, server_addr) ;

    return SOK ;
}

/*****************************************************************************
* @brief    set network port 
* @section  [desc]
*****************************************************************************/
int ctrl_set_port(int http_port, int https_port, int rtsp_port)
{
    if(http_port > 0 && http_port < 65535)
    {
        if(app_set->net_info.http_port != http_port)
            app_set->net_info.http_port = http_port ;
    }

    if(https_port > 0 && https_port < 65535)
    {
        if(app_set->net_info.https_port != https_port)
            app_set->net_info.https_port = https_port ;

    }
    
    if(rtsp_port > 0 && rtsp_port < 65535)
    {
        if(app_set->net_info.rtsp_port != rtsp_port)
           app_set->net_info.rtsp_port = rtsp_port ;

        app_rtsptx_stop_start() ; 

    }

    return SOK ;
}

/*****************************************************************************
* @brief    set network info(ip, subnet, DHCP or STATIC)
* @section  [desc]
* parameter token is device name(eth0, wlan0, usb1...)
*****************************************************************************/

int ctrl_set_network(int net_type, char *token, char *ipaddr, char *subnet)
{
    FILE *f = NULL;

    dev_net_info_t  inet;

    char log[256] = {0,};
    char buff[32] ;
    int ret ;

	if(strcmp(token, "wlan0")==0){
		if(!net_type)  // STATIC
		{
			if(ipaddr)
				strcpy(app_set->net_info.wlan_ipaddr, ipaddr);
			if(subnet)
				strcpy(app_set->net_info.wlan_netmask, subnet);

			ret = app_rec_state();
			if(ret)
			{
				sleep(1) ;
				app_rec_stop(1);
			}
	        app_file_stop();

			app_set->net_info.type = net_type ;

			sprintf(log, "[APP] --- Wireless ipaddress changed System Restart ---");
			app_log_write( MSG_LOG_SHUTDOWN, log);
		}
		else  // DHCP 
		{
            sprintf(buff, "/sbin/udhcpc -t 5 -n -i %s",token) ;
// -t retries -n exit with failure if lease is not immediately obtained 
            f = popen(buff, "w");
            if (f != NULL)
            {
                pclose(f);
                dev_get_net_info(token, &inet);
                sprintf(app_set->net_info.wlan_ipaddr, "%s", inet.ip);
                sprintf(app_set->net_info.wlan_gateway, "%s", inet.gate);
                sprintf(app_set->net_info.wlan_netmask, "%s", inet.mask);

                sprintf(log, "[APP] --- Wireless allocated ipaddress %s", app_set->net_info.wlan_ipaddr);
                app_log_write( MSG_LOG_WRITE, log);
            }
		}
	}
	else {
		if(!net_type)  // STATIC
		{
			if(ipaddr)
				strcpy(app_set->net_info.eth_ipaddr, ipaddr);
			if(subnet)
				strcpy(app_set->net_info.eth_netmask, subnet);

			ret = app_rec_state();
			if(ret)
			{
				sleep(1) ;
				app_rec_stop(1);
			}

			app_set->net_info.type = net_type ;

			sprintf(log, "[APP] --- Ethernet ipaddress changed System Restart ---");
			app_log_write( MSG_LOG_SHUTDOWN, log );
		}
		else  // DHCP
		{
			app_set->net_info.type = net_type ;
			sprintf(log, "[APP] --- Ethernet change to DHCP.. System Restart ---");
			app_log_write( MSG_LOG_SHUTDOWN, log );
		}
	}

    app_set_write();

//    mic_exit_state(OFF_RESET, 0);
//    app_main_ctrl(APP_CMD_EXIT, 0, 0);

	mcu_pwr_off(OFF_RESET) ;
    return SOK ;
}

/*****************************************************************************
* @brief    set network gateway info(gateway)
* @section  [desc]
*****************************************************************************/

int ctrl_set_gateway(char *gw)
{
    char log[255] ;
    int ret ;

    if(gw)
        strcpy(app_set->net_info.eth_gateway, gw);

    ret = app_rec_state();
    if(ret)
    {
        sleep(1) ;
        app_rec_stop(1);
    }

    app_file_stop();

    sprintf(log, "[APP] --- Ethernet gateway changed System Restart ---");
    app_log_write( MSG_LOG_SHUTDOWN, log );
    app_set_write();

//    mic_exit_state(OFF_RESET, 0);
//    app_main_ctrl(APP_CMD_EXIT, 0, 0);

	mcu_pwr_off(OFF_RESET);

    return SOK ;
}

/*****************************************************************************
* @brief    get version function
* @section  DESC Description
*   - desc
*****************************************************************************/
int ctrl_get_hw_version(char *version)
{
	int ver;

	/* fitt
	   0x0 -> ver0.1
	   ...
	*/
	ver = dev_get_board_info();
	if(ver < 0) {
		sprintf(version, "UNKNOWN");
		return -1;
	}

	if(version != NULL) {
		sprintf(version, "00.%02d", (ver+1));
	}

	return ver;
}

int ctrl_get_mcu_version(char *version)
{
	int ver;

	ver = (int)mic_get_version();
	sprintf(version, "%02d.%02X", (ver>>8)&0xFF, ver&0xFF);

	return ver;
}

int ctrl_time_set(int year, int mon, int day, int hour, int min, int sec)
{
    struct tm ts;
    time_t set;
    char msg[MAX_CHAR_128], buf[MAX_CHAR_64];

    ts.tm_year = (year - 1900);
    ts.tm_mon  = (mon - 1);
    ts.tm_mday = day;
    ts.tm_hour = hour;
    ts.tm_min  = min;
    ts.tm_sec  = sec;

    set = mktime(&ts);

    strftime( buf, sizeof(buf), "%Y%2m%2d_%2H%2M%2S", &ts );
    sprintf( msg, "[APP_CTRL] Time Change : %s", buf );
    app_log_write( MSG_LOG_WRITE, msg );

    stime(&set);
    Vsys_datetime_init();   //# m3 Date/Time init
    OSA_waitMsecs(100);

    if (dev_rtc_set_time(ts) < 0) {
        sprintf(msg, "[APP_CTRL] !!! Failed to set system time to rtc !!!");
        app_log_write( MSG_LOG_WRITE, msg );
    }   

    return 0;
}

/*****************************************************************************
 * @brief    mmc partition check function
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
struct mmc_part_info mmc_part = {
	.part_no = 0,
	.part_type = 0,
	.part_size = 0
};

int ctrl_mmc_check_exfat(unsigned long *size)
{
	int ret = 0;

	if (size == NULL)
		return -1;

	ret = dev_disk_mmc_part_check_info(MMC_BLK_DEV_NAME, &mmc_part);
	if (ret) {
		eprintf("please check sd card!!\n");
		return -1;
	}

	if (mmc_part.part_size > MMC_SIZE_128GB) {
		eprintf("Not suppoted sd card (%lu MB)!!\n", mmc_part.part_size);
		return -1;
	}
	if (mmc_part.part_no != 1) {
		eprintf("Invalid partition sd card!!\n");
		return -1;
	}

	if (mmc_part.part_type == MMC_PART_TYPE_EXFAT) {
		/*
		 * we have to check mount from mdev mount.sh
		 */
		ret = 1;
	}

	*size = mmc_part.part_size;

	return ret;
}

int ctrl_mmc_exfat_format(unsigned long size)
{
    char cmd[256] = {0,};
    int blkid = 0, i;
    DIR *mount_dir = NULL;

    if (dev_disk_check_mount(MMC_MOUNT_POINT)) {
		dev_disk_mmc_part_unmount(MMC_PART_NAME);
		/* wait done */
		app_msleep(300);
	}
	/*
	 * first loop-> changed partition fat32
	 * second loop-> make mmcblk0p1
	 */
	for (i = 0; i < 2; i++) {
		dev_disk_mmc_part_delete(mmc_part.part_no);
		usleep(500000); //# wait for done!
		dev_disk_mmc_part_create();
	}

	blkid = dev_disk_mmc_part_format(size);
	if (blkid < 0) {
		dprintf("Failed to format sdcard\n");
		/* todo: return or reboot?? */
		return -1;
	}
	usleep(500000); //# wait for done!

	//# check /mmc directory.
	mount_dir = opendir("/mmc");
	if (mount_dir == NULL) {
		mkdir("/mmc", 0775);

        mount_dir = opendir("/mmc/log") ;
        if(mount_dir == NULL)
		{
		    mkdir("/mmc/log", 0775);
		}
        else
			closedir(mount_dir) ;

		sync();
	} else
		closedir(mount_dir);

	snprintf(cmd, sizeof(cmd),
			"/bin/mount -t vfat /dev/mmcblk%d""p1 /mmc", blkid);
	dev_execlp(cmd);

    return 0;
}

/*****************************************************************************
 * @brief    int ctrl_mmc_check_partitions(int *num_part)
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
int ctrl_mmc_check_partitions(void)
{
	FILE *part_f = NULL;

	char buf[256 + 1] = {0,};
	char name[128 + 1] = {0,};

	int ma, mi; //# major, minor
	int count = 0;

	unsigned long long sz;

	part_f = fopen("/proc/partitions", "r");
	if (part_f == NULL) {
		eprintf("couldn't open /proc/partitions\n");
		return -1;
	}

	/* major minor  #blocks  name
  	 * 	31     0     128 	 mtdblock0
 	 *  179    0   30915584  mmcblk0
 	 *  179    1   30915552  mmcblk0p1
     */
	while (fgets(buf, sizeof(buf) - 1, part_f) != NULL)
	{
		/* except \n */
		if (sscanf(buf, " %d %d %llu %128[^\n ]", &ma, &mi, &sz, name) != 4)
			continue;

		if (strncmp(name, "mmcblk0", 7) == 0) {
		    count += 1;
		}
	}
    fclose(part_f);

    return ((count == 2)?0:-1);
}

/*****************************************************************************
 * @brief    int ctrl_mmc_check_fsck(void)
 * -n : no-op, check non-interactively without changing
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
int ctrl_mmc_check_fsck(void)
{
/*
 * EXIT STATUS
 *   0 : No recoverable errors have been detected.
 *   1 : Recoverable errors have been detected.
 *   2 : Usage error. fsck.fat did not access the filesystem.(required reboot)
 */
	char *cmd = "/sbin/fsck.fat -n /dev/mmcblk0p1";

	return dev_execlp(cmd);
}

/*****************************************************************************
 * @brief    int ctrl_mmc_run_fsck(void)
 * -w : write changes to disk immediately
 * -a : automatically repair the filesystem
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
int ctrl_mmc_run_fsck(void)
{
/* -a: Automatically repair the filesystem.
 * -w: Write changes to disk immediately.
 */
	char *cmd = "/sbin/fsck.fat -a -w /dev/mmcblk0p1";

	return dev_execlp(cmd);
}

/*****************************************************************************
* @brief    SWOSD control function
* @section  DESC Description
*   - desc
*****************************************************************************/
void ctrl_swosd_enable(int idx, int ch, int draw)
{
	Vsys_swOsdPrm swosdGuiPrm;

	swosdGuiPrm.streamId = ch;
	swosdGuiPrm.enable = draw;
	switch(idx)
	{
		case STE_GPS:
			Vsys_setSwOsdPrm(VSYS_SWOSD_GPS, &swosdGuiPrm);
			break;
		case STE_DTIME:
			Vsys_setSwOsdPrm(VSYS_SWOSD_EN_DATETIME, &swosdGuiPrm);
			break;
	}

	return;
}

/*****************************************************************************
* @brief    SWOSD user string function
* @section  DESC Description
*   - desc
*****************************************************************************/
void ctrl_swosd_userstr(char *str, int draw)
{
	Vsys_swOsdPrm swosdGuiPrm;

	swosdGuiPrm.streamId = 0;
	swosdGuiPrm.enable = draw;
	strcpy((char*)swosdGuiPrm.usrString, str);

	Vsys_setSwOsdPrm(VSYS_SWOSD_USERSTR, &swosdGuiPrm);
}

/*----------------------------------------------------------------------------
 firmware update
-----------------------------------------------------------------------------*/
#define FW_FILE_NUM 8
#define FW_TYPE     0   //# "normal" or "debug"
#define FW_CTAG     5   //# L(lte) N(wifi) B(basic)
#define F_RELEASE   "release"
#define FW_DIR      "/mmc/fw_version.txt"

#define FW_PACKAGE_NONE		0
#define FW_PACKAGE_BIN		1
#define FW_PACKAGE_FULL		2

// #define F_LTE_UPDATE	    "fitt_firmware_L.dat"
// #define F_LTE_UPDATE_FULL	"fitt_firmware_full_L.dat"  // support LTE
#define FITT_WIRELESS_UPDATE	    "fitt_firmware_N.dat"
#define FITT_WIRELESS_UPDATE_FULL	"fitt_firmware_full_N.dat"  // support WIRELESS
#define FITT_UPDATE 	        "fitt_firmware_B.dat"
#define FITT_UPDATE_FULL	    "fitt_firmware_full_B.dat"   // basic

#define NEXX_WIRELESS_UPDATE	    "nexx_firmware_N.dat"
#define NEXX_WIRELESS_UPDATE_FULL	"nexx_firmware_full_N.dat"  // support WIRELESS
#define NEXX_UPDATE 	        "nexx_firmware_B.dat"
#define NEXX_UPDATE_FULL	    "nexx_firmware_full_B.dat"   // basic

#define NUM_FULL_UPFILES		8
static char *full_upfiles[NUM_FULL_UPFILES] = {
	"boot.scr", "u-boot_fit.min.nand", "u-boot_fit.bin", "MLO", "fw_version.txt",
	"uImage_fit", "rfs_fit.ubifs", "mcu_fitt.txt"
};
	
typedef struct {
    char item[8];
    char value[8];
//    char CTAG[8];
} fw_version_t;

//------------------------------------------------------//

static int _is_supported_wireless()
{
	int ret = TYPE_BASIC;  

    if(0 == access("/opt/fit/distinction" ,0))  // WIFI + LTE
    {
        ret = TYPE_WIRELESS ;
    }

    return ret ;
}

static int _chk_update_file(char *disk, char *firmware_name)
{
	char fname[128];
    char fw_fulltype[128] ;
    char fw_type[128] ;
    char msg[128] = {0, } ;

    int ret = 0;

    ret = _is_supported_wireless() ;

    if(ret > TYPE_BASIC) // exist fw_distinction  --> wireless model
    {
         // WIRELESS 2.xx.xxN
        sprintf(fw_fulltype, "%s",FITT_WIRELESS_UPDATE_FULL) ;
        sprintf(fw_type, "%s",FITT_WIRELESS_UPDATE) ;
         
    }
    else // BASIC 2.xx.xxB
    {
        sprintf(fw_fulltype, "%s",FITT_UPDATE_FULL) ;
        sprintf(fw_type, "%s",FITT_UPDATE) ;
    }

	//# update files check
	sprintf(fname, "%s/%s", disk, fw_fulltype);
    sprintf(firmware_name, "%s", fw_fulltype) ;
	if(0 == access(fname, 0)) {
		return FW_PACKAGE_FULL;
	}

	sprintf(fname, "%s/%s", disk, fw_type);
    sprintf(firmware_name, "%s", fw_type) ;
	if(0 == access(fname, 0)) {
		return FW_PACKAGE_BIN;
	}

    if(ret > TYPE_BASIC) // exist fw_distinction  --> wireless model
    {
         // WIRELESS 2.xx.xxN
        sprintf(fw_fulltype, "%s",NEXX_WIRELESS_UPDATE_FULL) ;
        sprintf(fw_type, "%s",NEXX_WIRELESS_UPDATE) ;
         
    }
    else // BASIC 2.xx.xxB
    {
        sprintf(fw_fulltype, "%s",NEXX_UPDATE_FULL) ;
        sprintf(fw_type, "%s",NEXX_UPDATE) ;
    }

	//# update files check
	sprintf(fname, "%s/%s", disk, fw_fulltype);
    sprintf(firmware_name, "%s", fw_fulltype) ;
	if(0 == access(fname, 0)) {
		return FW_PACKAGE_FULL;
	}

	sprintf(fname, "%s/%s", disk, fw_type);
    sprintf(firmware_name, "%s", fw_type) ;
	if(0 == access(fname, 0)) {
		return FW_PACKAGE_BIN;
    }

    sprintf(msg, "[APP_CTRL] !!! firmware update file is not exist !!!");
    app_log_write(MSG_LOG_WRITE, msg);

	return FW_PACKAGE_NONE;
}

static int _is_firmware_type_check(void)
{
    fw_version_t fw[FW_FILE_NUM];
    FILE *F_fw;
    int i=0, ret = FALSE;

    F_fw = fopen(FW_DIR, "r");
    if (F_fw != NULL) {
        while (!feof(F_fw)) {
            fscanf(F_fw,"%s %s", fw[i].item, fw[i].value);
            i++;        

            if (i == FW_FILE_NUM)
                break;          
        }       
        fclose(F_fw);

        switch(_is_supported_wireless())
        {
            case TYPE_BASIC :  

                if (strcmp(fw[FW_CTAG].value, "B") == 0) {
                    printf("\n UPDATE FILE Basic!!\n");
                    ret = TRUE;
                }
                break ;

            case TYPE_WIRELESS :
                if (strcmp(fw[FW_CTAG].value, "N") == 0) {
                    printf("\n UPDATE FILE Wireless!!\n");
                    ret = TRUE;
                }
                break ;

            default :
                break ;
        }
    }
    return ret ;
}

static int _is_firmware_for_release(void)
{
    fw_version_t fw[FW_FILE_NUM];
    FILE *F_fw;
    F_fw = fopen(FW_DIR, "r");
    int i=0, ret = FALSE;

    if (F_fw != NULL) {
        while (!feof(F_fw)) {
            fscanf(F_fw,"%s %s", fw[i].item, fw[i].value);
            i++;        

            if (i == FW_FILE_NUM)
                break;          
        }       
        fclose(F_fw);

        if (strcmp(fw[FW_TYPE].value, F_RELEASE) == 0) {
            printf("\n TYPE RELEASE!!\n");
            ret = TRUE;
        }
    }
    return ret ;
}

#if 0
static int _chk_firmware_checksum(const char *filename)
{
    unsigned long stored_crc32 = 0;
    FILE *infile = NULL;
    int ret = -1;
    int fsize = 0, tmp_sz;
    unsigned int calc_crc = 0;

    infile = fopen(filename, "r");
    if (infile != NULL) {
        size_t bytes;

        //# Get file size.
        fseek(infile, 0L, SEEK_END);
        fsize = ftell(infile);
        fseek(infile, 0L, SEEK_SET); //# back to beginning of file.

        //# read generated crc32 value.
        tmp_sz = fsize - 4; //# 파일의 맨 마지막 4 바이트에 저장함.
        fseek(infile, tmp_sz, SEEK_SET); //# back to beginning of file.
        fread(&stored_crc32, 4, 1, infile);
        //printf("read crc %x\n", stored_crc32);

        fseek(infile, 0L, SEEK_SET); //# back to beginning of file.
        while (tmp_sz > 0) {
            char buf[1024];

            bytes = fread(buf, 1, 1024, infile);
            calc_crc = util_gen_crc32(calc_crc, buf, bytes);
            tmp_sz -= bytes;
        }

        //printf("n byte read %d, crc32 = %08x\n", bytes, calc_crc);
        fclose(infile);

        if (stored_crc32 == calc_crc) ret = 0;
        else                          ret = -1;      
    }

    return ret;
}
#endif

void ctrl_reset_nand_update(void)
{
	int i;
	char cmd[MAX_CHAR_255] = {0,};
	
	//# delete full updated file
	if(dev_fw_printenv("nand_update") == 2)
	{
        if(_is_firmware_for_release()) // RELEASE Version .. --> update file delete
		{
  		    if(0 == access("/mmc/MLO", 0)) {
			    for(i=0; i<NUM_FULL_UPFILES; i++) {
				    sprintf(cmd, "rm -rf %s/%s", SD_MOUNT_PATH, full_upfiles[i]);
				    util_sys_exec(cmd);
			    }
		    }
		}
		
		dev_fw_setenv("nand_update", "0", 0);
	}
}


/*****************************************************************************
 * @brief    wifi state function
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
int ctrl_wifi_set(int on)
{
	int ret = SOK;
	int vid, pid;
    int stealth = 0 ;
	int i, channel;
	int run_5g = 0;

	char path[128] = {0,};
	char name[128] = {0,};
	char msg[MAX_CHAR_128] = {0,};

	channel = 6; //# default channel

    if (on)
    {
    	sprintf( msg, "[APP_CTRL] WiFi ON ssid:%s", app_set->wifiap.ssid);
		printf("%s\n",msg) ;
		app_log_write( MSG_LOG_WRITE, msg );

    	if (dev_wifi_is_exist(&vid, &pid)) {
			/* not connected wifi usb module */
			app_log_write( MSG_LOG_WRITE, "!!! Not Connected WiFi USB Module !!!");
			return EFAIL;
		}

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
			run_5g = 1; channel = 149;
			strcpy(path, RTL_8821A_PATH);
			strcpy(name, RTL_8821A_NAME);
		} else {
			/* invalid wifi usb module */
			app_log_write( MSG_LOG_WRITE, "!!! Not Supported WiFi USB Module !!!");
			printf("%s\n",msg) ;
			return EFAIL;
		}

        if (app_cfg->ste.b.wifi_ready) {
        	if (dev_wifi_get_module_state(name) == 0) {
        		dev_wifi_load((const char *)path);
				/* waiting for driver loaded */
				for (i = 0; i < 20; i++)
				{
					ret = access("/sys/class/net/wlan0/operstate", R_OK);
				    if ((ret == 0) || (errno == EACCES)) {
				      	break;
				    } else
						OSA_waitMsecs(100);
				}
				if (i >= 20)
	                ret = EFAIL;
        	}


			if (!ret) {
	            if (run_5g) {
					channel = 44;
				} else {
					channel = 6;
				}

				/*
				 * 1st -> Wi-Fi SSID
				 * 2nd -> Wi-Fi Password
				 * 3th -> Wi-Fi Channel
				 * 4th -> 0:2.4GHz 1:5GHz.
				 */
                if(app_cfg->wmode.wifi_mode)
                {
				    stealth = app_set->wifiap.stealth;
	                if (dev_wifi_on(app_set->wifiap.ssid, app_set->wifiap.pwd, channel, run_5g, stealth) != 0)
                    {
	                    app_log_write( MSG_LOG_WRITE, "!!! WIFI Start failed !!!");
		                printf("%s\n",msg) ;
	                    return EFAIL;
	                }
				}
				else
				{
				    stealth = app_cfg->wmode.stealth;
	                if (dev_wifi_on(app_cfg->wmode.ssid, app_cfg->wmode.pwd, channel, run_5g, stealth) != 0)
                    {
	                    app_log_write( MSG_LOG_WRITE, "!!! WIFI Start failed !!!");
		                printf("%s\n",msg) ;
	                    return EFAIL;
	                }

				}
	            app_cfg->ste.b.wifi_run = 1;
	        } else
	        	app_cfg->ste.b.wifi_run = 0; /* Failed to load wi-fi driver */
        }
    } else {
        if (app_cfg->ste.b.wifi_ready) {
        	app_cfg->ste.b.wifi_ready = 0;
	       	app_cfg->ste.b.wifi_run = 0; 
           	dev_wifi_off();
			app_log_write( MSG_LOG_WRITE, "WIFI Module OFF...");
        }
    }

	return ret;
}








/*****************************************************************************
* @brief    Firmware update
* @section  DESC Description
*   - desc
*****************************************************************************/
int ctrl_sw_update(char *disk)
{
	char cmd[256], fname[256], firmware_name[128];
	int type, ret = 0;

	aprintf("start...\n");

	app_cfg->ste.b.busy = 1;
	type = _chk_update_file(disk, firmware_name);
	if (type == FW_PACKAGE_NONE) {
		eprintf("no update file!\n");
		app_cfg->ste.b.busy = 0;
		//# Reboot를 하지 않기 위해서 리턴값을 -1에서 0으로 변경
        return -1;
	}

    ret = app_rec_state();
    if (ret) {
        sleep(1) ;
        app_rec_stop(1);
    }
//	app_file_stop();

    sprintf(fname, "%s/%s", disk, firmware_name);  // FULL UPDATE FILE
#if 0    
    //# to verify update file checksum.
    ret = _chk_firmware_checksum(fname);
    if (ret) {
        eprintf("failed to verify firmware checksum!\n");
		app_cfg->ste.b.busy = 0;
		return -1;
    }
#endif
	if(type == FW_PACKAGE_FULL)		//# full update
	{
		//# extract update file
		sprintf(cmd, "tar xvf %s -C %s", fname, disk);
		system(cmd);
		sleep(3);	//# wait tar done

        if(!_is_firmware_type_check())
        {
		    sprintf(cmd, "rm -rf %s", fname);
		    system(cmd);
            return -1 ;
        }

	    app_leds_fw_update_ctrl();
		dev_fw_setenv("nand_update", "1", 0);

        if(_is_firmware_for_release()) // RELEASE Version .. --> update file delete
        { 
		    sprintf(cmd, "rm -rf %s", fname);
		    system(cmd);
        }

		app_log_write( MSG_LOG_WRITE, "[APP_FITT360] Full version Firmware update done....");
	}
	else if(type == FW_PACKAGE_BIN)
	{
	    app_leds_fw_update_ctrl();

		//# extract update file
		sprintf(cmd, "tar xvf %s -C %s", fname, disk);
		system(cmd);
		sleep(2);	//# wait tar done

		//# update files
		sprintf(fname, "%s/update/rfs", disk);
		if(0 == access(fname, 0)) {
			sprintf(cmd, "cp -rfa %s/* /", fname);
			system(cmd);
		}

		//# delete update directory
		sprintf(cmd, "rm -rf %s/update", disk);
		system(cmd);
	}

	sync();
	app_msleep(200);		//# wait for safe
	aprintf("done! will restart\n");

	return 0;
}

void fitt360_reboot()
{
    int ret ;

    ret = app_rec_state();
    if(ret)
    {
        sleep(1) ;
        app_rec_stop(1);
    }
    app_file_stop();

    app_set_write();

    dev_buzz_ctrl(80, 2);
	
//    mic_exit_state(OFF_RESET, 0);
//    app_main_ctrl(APP_CMD_EXIT, 0, 0);

	mcu_pwr_off(OFF_RESET);
}

int ctrl_update_firmware(char *fwpath, char *disk)
{
	char cmd[256], fname[256], firmware_name[128];
	int type, ret;

	aprintf("start...\n");

	app_cfg->ste.b.busy = 1;
	type = _chk_update_file(fwpath, firmware_name);
	if(type == FW_PACKAGE_NONE) {
		eprintf("no update file!\n");
		app_cfg->ste.b.busy = 0;
		return -1;
	}

    ret = app_rec_state();
    if(ret)
    {
        sleep(1) ;
        app_rec_stop(1);
    }
//	app_file_stop();

	if(type==FW_PACKAGE_FULL)		//# full update
	{
		//# extract update file
	    sprintf(fname, "%s/%s", fwpath, firmware_name);  // FULL UPDATE FILE
		sprintf(cmd, "tar xvf %s -C %s", fname, disk);
		system(cmd);
		sleep(3);	//# wait tar done

        if(!_is_firmware_type_check())
        {
            sprintf(cmd, "rm -rf %s", fname);
            system(cmd);
            
	        app_cfg->ste.b.busy = 0;

            return -1 ;
        }

	    app_leds_fw_update_ctrl();
		dev_fw_setenv("nand_update", "1", 0);

        if(_is_firmware_for_release()) // RELEASE Version .. --> update file delete
        { 
		    sprintf(cmd, "rm -rf %s", fname);
		    system(cmd);
        }

		app_log_write( MSG_LOG_WRITE, "[APP_FITT360] Full version Firmware update done....");
	}
	else if(type == FW_PACKAGE_BIN)
	{
	    app_leds_fw_update_ctrl();
		//# extract update file
		sprintf(fname, "%s/%s", fwpath, firmware_name);   // UPDATE FILE
		sprintf(cmd, "tar xvf %s -C %s", fname, disk);
		system(cmd);
		sleep(2);	//# wait tar done

		//# update files
		sprintf(fname, "%s/update/rfs", disk);
		if(0 == access(fname, 0)) {
			sprintf(cmd, "cp -rfa %s/* /", fname);
			system(cmd);
		}

		//# delete update directory
		sprintf(cmd, "rm -rf %s/update", disk);
		system(cmd);
	}

	sync();
	app_msleep(200);		//# wait for safe
	aprintf("done! will restart\n");

	//# for micom exit..
//	mic_exit_state(OFF_RESET, 0);
//	app_main_ctrl(APP_CMD_EXIT, 0, 0);

	mcu_pwr_off(OFF_RESET);
	return 0;
}

int temp_ctrl_update_fw_by_bkkim(char *fwpath, char *disk)
{
	char cmd[256];
    app_leds_fw_update_ctrl();

#if 1
	sprintf(cmd, "cp -f %s %s/", fwpath, disk);
	printf("fwupdate cmd:%s\n", cmd);
	system(cmd);
#endif

	dev_fw_setenv("nand_update", "1", 0);

	sync();

	app_log_write( MSG_LOG_WRITE, "[APP_FITT360] Temp version Firmware update done....");

	sync();
	app_msleep(200);		//# wait for safe
	printf("fw update ready ! It will restart\n");

	//# for micom exit..
//	mic_exit_state(OFF_RESET, 0);
//	app_main_ctrl(APP_CMD_EXIT, 0, 0);

	mcu_pwr_off(OFF_RESET);
	return 0;
}

/*
 * @param : fwpath : full path (maybe /tmp/rfs_fit.ubifs)
 */
int ctrl_update_firmware_by_cgi(char *fwpath)
{
	dev_buzz_ctrl(50, 3);		//# buzz: update

	if(!app_cfg->ste.b.ftp_run)
	{ 
		app_rec_stop(0);
		//ctrl_update_firmware(fwpath, SD_MOUNT_PATH);
		temp_ctrl_update_fw_by_bkkim(fwpath, SD_MOUNT_PATH);
	}
    return 0 ;     
}
