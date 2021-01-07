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
#include <glob.h>

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
#include "app_buzz.h"

#if SYS_CONFIG_VOIP
#include "app_voip.h"
#endif

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define FW_EXT       			".dat"

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static const char *fw_app_name  = "/mmc/app_fitt.out";
	
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

    if(app_set->ch[MODEL_CH_NUM].resol == RESOL_720P)
        params.packetSize = 12 ;  // --> slice count 8
    else if(app_set->ch[MODEL_CH_NUM].resol == RESOL_1080P)
        params.packetSize = 10 ;  // -- slice count 10
    else
        params.packetSize = 16 ;  // -- slice count 7

    Venc_setDynamicParam(MODEL_CH_NUM, 0, &params, VENC_PACKETSIZE);

    return SOK ;
}

/*****************************************************************************
* @brief    set video framerate
* @section  [desc]
*****************************************************************************/
int ctrl_vid_framerate(int ch, int framerate)
{
	int ret = 0 ;

	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };

    app_set->ch[ch].framerate = framerate ;
    app_cfg->ich[ch].fr = framerate;

    params.frameRate = app_cfg->ich[ch].fr;
// to do rec stop
    if(ch != MODEL_CH_NUM)
	{
        ret = app_rec_state() ;
	    if (ret) { 
	        app_rec_stop(1);
	        sleep(1); /* wait for file close */
        }
	}
    Venc_setInputFrameRate(ch, DEFAULT_FPS);
    Venc_setDynamicParam(ch, 0, &params, VENC_FRAMERATE);
// to do rec start
    if(ch != MODEL_CH_NUM)
	{
	    if (ret) {
            app_rec_start();
        }
	}

    app_rtsptx_stop_start() ;

	return SOK ;
}

/*****************************************************************************
* @brief    set video bitrate
* @section  [desc]
*****************************************************************************/
int ctrl_vid_bitrate(int ch, int bitrate) 
{
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };
    int br;

    app_set->ch[ch].quality = bitrate;
	// resol 480P 0 720P 1 1080P 2
//    app_cfg->ich[ch].br = (br * app_cfg->ich[ch].fr)/DEFAULT_FPS;
    app_cfg->ich[ch].br = bitrate;
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
    char buf[128] = {0, };
    int ret ;

    //# to prevent key input
    app_cfg->ste.b.nokey = 1;
    Vdis_disp_ctrl_exit();

    ret = app_rec_state() ;
    if (ret) {
        app_rec_stop(1);
		sleep(1); /* wait for file close */
	}
	
    app_cap_stop() ;
    app_buzz_ctrl(100, 1);
    app_msleep(200);

	app_set->ch[MODEL_CH_NUM].resol = resol_idx;

    Vdis_disp_ctrl_init(resol_idx);

    app_cap_start();    
    if (ret) {
        app_rec_start();
    }

    app_rtsptx_stop_start() ;
    if (!app_set->sys_info.osd_set)
        ctrl_swosd_enable(STE_DTIME, 0, 0) ;  // osd disable
	
	switch(resol_idx) {
	default:
	case 0:
		snprintf(buf, sizeof(buf), "[APP_CTRL] --- change Display Mode to 480P ---");
		break;
	case 1:
		snprintf(buf, sizeof(buf), "[APP_CTRL] --- change Display Mode to 720P ---");
		break;
	case 2:
		snprintf(buf, sizeof(buf), "[APP_CTRL] --- change Display Mode to 1080P ---");
		break;
    }
	
    app_log_write(MSG_LOG_WRITE, buf);
	
    app_cfg->ste.b.nokey = 0;
	printf("vid_resolution nokey = %d\n",app_cfg->ste.b.nokey) ;

    return SOK ;
}

/*****************************************************************************
* @brief    set full video setting function
* @section  DESC Description
*   - desc
*****************************************************************************/
int ctrl_full_vid_setting(int ch, int resol, int bitrate, int fps, int gop)
{
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };

	params.frameRate = fps ;
	params.targetBitRate = bitrate*1000;
	params.intraFrameInterval = gop ;
	app_set->ch[ch].framerate = fps;
	app_set->ch[ch].quality = bitrate;
    app_set->ch[ch].gop = gop ;

	Venc_setInputFrameRate(ch, DEFAULT_FPS) ;
	Venc_setDynamicParam(ch, 0, &params, VENC_BITRATE) ;
	Venc_setDynamicParam(ch, 0, &params, VENC_IPRATIO) ;
	Venc_setDynamicParam(ch, 0, &params, VENC_FRAMERATE) ;

	if (app_set->ch[ch].resol != resol)
	{
		int ret = 0;
		Vdis_disp_ctrl_exit();

		ret = app_rec_state();

		if (ret)
		{
			app_rec_stop(1);
			sleep(1);
		}

		app_cap_stop();

		app_buzz_ctrl(100, 1);
		app_msleep(200);

		app_set->ch[ch].resol = resol;

		Vdis_disp_ctrl_init(resol);
		app_cap_start();
		if (ret)
		{
			app_rec_start();
		}

		app_rtsptx_stop_start();

		if (!app_set->sys_info.osd_set)
			ctrl_swosd_enable(STE_DTIME, 0, 0); // osd disable
	}

	return SOK ;
}

/*****************************************************************************
* @brief    get resolution function
* @section  DESC Description
*   - desc
*****************************************************************************/
int ctrl_get_resolution()
{
    return app_set->ch[MODEL_CH_NUM].resol;
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
int ctrl_set_network(int net_type, const char *token, const char *ipaddr, const char *subnet)
{
    char log[256] = {0,};
	
	if (!net_type)  // STATIC
	{
		if (strcmp(token, "wlan0") == 0) {
			if (ipaddr != NULL)
				strcpy(app_set->net_info.wlan_ipaddr, ipaddr);
			if (subnet != NULL)
				strcpy(app_set->net_info.wlan_netmask, subnet);
			
			sprintf(log, "[APP] --- Wireless ipaddress changed System Restart ---");
			app_log_write(MSG_LOG_SHUTDOWN, log);		
		} else {
			if (ipaddr != NULL)
				strcpy(app_set->net_info.eth_ipaddr, ipaddr);
			if (subnet != NULL)
				strcpy(app_set->net_info.eth_netmask, subnet);
			
			sprintf(log, "[APP] --- Ethernet ipaddress changed System Restart ---");
			app_log_write(MSG_LOG_SHUTDOWN, log);	
		}
	}	
	
	app_set->net_info.type = net_type;
	ctrl_sys_reboot();
	
    return SOK ;
}

/*****************************************************************************
* @brief    set network gateway info(gateway)
* @section  [desc]
*****************************************************************************/
int ctrl_set_gateway(const char *gw)
{
    char log[255]={0,};

    if (gw != NULL)
        strcpy(app_set->net_info.eth_gateway, gw);
	
	sprintf(log, "[APP] --- Ethernet gateway changed System Restart ---");
    dprintf("%s\n", log);
	app_log_write( MSG_LOG_SHUTDOWN, log );
	ctrl_sys_reboot();	

    return SOK;
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
#define FW_FILE_NUM 		8
#define FW_TYPE     		0   //# "release" or "debug"
#define DEV_MODEL			1	//# "NEXXONE"
#define F_RELEASE   		"release"
#define FW_DIR      		"/mmc/fw_version.txt"
#define FW_UBIFS			"/mmc/rfs_fit.ubifs"

static char *full_upfiles[FW_FILE_NUM] = {
	"boot.scr", "u-boot_fit.min.nand", "u-boot_fit.bin", "MLO", "fw_version.txt",
	"uImage_fit", "rfs_fit.ubifs", "mcu_fitt.txt"
};
	
typedef struct {
    char item[8];
    char value[10];
} fw_version_t;

//------------------------------------------------------//
static int _is_firmware_for_release(void)
{
    fw_version_t fw[FW_FILE_NUM];
    FILE *F_fw;
    F_fw = fopen(FW_DIR, "r");
    int i=0, ret = FALSE;
	char buf[256]={0,};

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

		if (strcmp(fw[DEV_MODEL].value, MODEL_NAME) != 0) {
			sprintf(buf, "This FW is not for %s !!!", MODEL_NAME);
			printf("%s\n", buf);
			app_log_write(MSG_LOG_WRITE, buf);
			ret = EFAIL;
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

static char *_findFirmware(const char* root)
{
	char path[255];
	static char extPath[255];
	glob_t globbuf;

	memset(&globbuf, 0, sizeof(glob_t));
	
	//# /mmc/*.dat scanning
	sprintf(path, "%s/*%s", root, FW_EXT);
	if(glob(path, GLOB_DOOFFS, NULL, &globbuf) == 0)
	{
		if(globbuf.gl_pathc > 0)
		{
			strcpy(extPath,globbuf.gl_pathv[0]);
			globfree(&globbuf);
			return extPath;
		}
	}
	else
	{
		eprintf("Not found firmware file: %s\n",root);
	}

	globfree(&globbuf);

	return NULL;
}

static int _unpack_N_check(const char* pFile, const char* root, int* release)
{
	char buf[256]={0,};
	int ret = SOK;
	sprintf(buf, "tar xvf %s -C %s", pFile, root);
	system(buf);
	/* change 3--> 1*/
	sleep(1/*3*/);

	if(-1 == access(FW_DIR, 0)) {
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "Failed to read %s!!!", FW_DIR);
		app_log_write(MSG_LOG_WRITE, buf);
		return EFAIL;
	}

	*release = _is_firmware_for_release();
	
	//# *release is EFAIL means firmware is not for NEXONE.
	if (*release == EFAIL)
		ret = EFAIL;
	
	return ret;
}

/*****************************************************************************
* @brief    Firmware update
* @section  DESC Description
*   - desc
*****************************************************************************/
static int _sw_update(const char *disk)
{
	char cmd[256]={0,};
	char msg[128]={0,};
	char *pFile = NULL;
	int ret = 0;
	int release = 1;
	
	aprintf("start...\n");
	
	app_cfg->ste.b.busy = 1;
	pFile = _findFirmware(disk);
	if(pFile == NULL) {
		sprintf(msg, "Firmware file is not exist !!!");
		app_log_write(MSG_LOG_WRITE, msg);
		printf("%s\n", msg);
        app_cfg->ste.b.busy = 0;
		return EFAIL;
	}
	/* firmware update 시 종료키 이벤트를 막기 위해서 위치를 변경함 */
	//app_cfg->ste.b.busy = 0;
	
	//# unpack fw file and type check, release/debug and update full or binary only
	// pFile = /mmc/xxxxxx.dat
	// disk  = /mmc
	if (_unpack_N_check((const char *)pFile, (const char *)disk, &release) == EFAIL) {
		sprintf(msg, "It is not firmware file !!!");
		app_log_write(MSG_LOG_WRITE, msg);
		printf("%s\n", msg);
        ret = EFAIL;
		/* TODO : delete unpack update files.... */
		goto fw_exit;
	}
	app_cfg->ste.b.busy = 0;
	
	//# buzz: update
	app_buzz_ctrl(50, 3);		
	//# LED work for firmware update.
	app_leds_fw_update_ctrl();
	dev_fw_setenv("nand_update", "1", 0);
	app_log_write(MSG_LOG_WRITE, "Full version Firmware update done....");

	aprintf("done! will restart\n");
	ret = SOK;
	
fw_exit:
	if (release) // RELEASE Version .. --> update file delete
    { 
	    sprintf(cmd, "rm -rf %s", pFile);
	    system(cmd);
    }
	
	sync();
	app_msleep(500);		//# wait for safe	
	
	return ret;
}

void *thrRunFWUpdate(void *arg)
{
	app_log_write( MSG_LOG_WRITE, "[APP_FITT360] Web Remote Update Temp version Firmware update done....");

	app_buzz_ctrl(50, 3);		//# buzz: update
	dev_fw_setenv("nand_update", "1", 0);
	sync();

	printf("\nfw update ready ! It will restart\n\n");

	app_mcu_pwr_off(OFF_RESET);

	return NULL;
}

/* example remote update
 *
curl -v -u admin:1111 --http1.0 -F 'fw=@bin/fitt_firmware_full_N.dat' http://192.168.40.129/cgi/upload.cgi
*/
int temp_ctrl_update_fw_by_bkkim(char *fwpath, char *disk)
{
	char cmd[256];
	int ret;
	
	/* recording stop */
	ret = app_rec_state();
	if (ret) {
		app_rec_stop(1);
		sleep(1); /* wait for file close */
	}
	app_file_save_flist(); /* save file list */

	// check tar content list	
#if 1
	{
		sprintf(cmd, "tar tf %s > /tmp/fw.list", fwpath);
		printf("cmd:%s\n", cmd);
		system(cmd);
		FILE *fp = fopen("/tmp/fw.list", "r");
		if (!fp)
		{
			printf("failed tar tf %s > /tmp/fw.list\n", fwpath);
			return -1;
		}
		char line[255] = {0};
		while(fgets(line, 255, fp)){

			int len = strlen(line);
			if (len > 1)
			{
				// remote CR
				if (line[len - 1] == '\n')
					line[len - 1] = '\0';

				if(strcmp(line, "boot.scr")==0){
					printf("checked boot.scr\n");
				}
				else if(strcmp(line, "MLO")==0){
					printf("checked MLO\n");
				}
				else if(strcmp(line, "fw_version.txt")==0){
					printf("checked fw_version.txt\n");
				}
				else if(strcmp(line, "rfs_fit.ubifs")==0){
					printf("checked %s\n", "rfs_fit.ubifs");
				}
				else if(strcmp(line, "rfs_fit.ubifs.md5")==0){
					printf("checked rfs_fit.ubifs.md5\n");
				}
			}
		}
		fclose(fp);
	}
#endif

#if 1 // untar
	sprintf(cmd, "tar xvf %s -C %s", fwpath, disk);
	//sprintf(cmd, "cp -f %s %s/", fwpath, disk);
	printf("fwupdate cmd:%s\n", cmd);
	system(cmd);
#endif

    if(TRUE != _is_firmware_for_release()) // RELEASE Version .. --> update file delete
	{
		printf("firmware factory release version..\n") ;
/*
		printf("The model does not match, It is not release version, or the fw_version.txt is missing.\n");

		int i;
		for (i = 0; i < FW_FILE_NUM; i++)
		{
			sprintf(cmd, "rm -rf %s/%s", SD_MOUNT_PATH, full_upfiles[i]);
			printf("@@@@@@@@@ DELETE %s @@@@@@@@@@@@\n", full_upfiles[i]);
			util_sys_exec(cmd);
		}

		// delete rfs_fit.ubifs.md5
		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "/mmc/rfs_fit.ubifs.md5");
		if (access(cmd, F_OK) == 0)
			remove(cmd);

		return -1;
*/
	} else {
		printf("Good, this must be my pot\n. And the next checking is very horrible md5sum. Good luck!!\n");
	}

	// check md5sum
	sprintf(cmd, "cd %s && md5sum -c rfs_fit.ubifs.md5",disk);
	FILE *fp = popen(cmd, "r");
	if(fp){
		char line[255]={0};
		fgets(line, 255, fp);
		printf("%s\n", line);

		if(NULL == strstr(line, " OK")){

			//TODO: 실패할 경우, 압축해제한 파일들 처리
			pclose(fp);
			return -1;
		}

		pclose(fp);
		// OK, ready to firmware upgrade
	}
	else {
		eprintf("Failed popen(md5sum -c rfs_fit.ubifs.md5) , please check firmware file!!\n");
		//TODO: 실패할 경우, 압축해제한 파일들 처리
		return -1;
	}

#if 0
		thrRunFWUpdate(NULL);
#else
	// thread로 전환..웹에 펌업 시작을 알릴 목적으로..
	{
		pthread_t tid_fw;
		int ret = pthread_create(&tid_fw, NULL, thrRunFWUpdate, NULL);
		if (ret != 0) {
			eprintf("thrRunFWUpdate pthread_create failed, ret = %d\r\n", ret);
			return -1;
		}
		pthread_detach(tid_fw);
	}
#endif
	
	return 0;
}

/*
 * if /mmc/app_fitt.out is exist, copy app_fitt.out to /opt/fit/bin/.
 * if /mmc/fitt_firmware_full_N.dat is exist, firmware update...
 */
void ctrl_auto_update(void)
{
	char path[64] = {0, };
	char cmd[255] = {0, };
		
	/* First, full firmware check.. */
	//# 업데이트 파일명이 비정상적인 경우를 제외하고는 
	if (_sw_update(SD_MOUNT_PATH) == 0) {
		app_mcu_pwr_off(OFF_RESET);
	}
		
	memset(path, 0, sizeof(path));
	sprintf(path, fw_app_name);
	if(0 == access(path, 0)) // existence only
	{
		dprintf("\n######### COPY APP_FIIT.OUT !!!! #########\n");
		sprintf(cmd, "cp %s /opt/fit/bin/.", path);
		util_sys_exec(cmd);
		
		sprintf(cmd, "rm %s", path);
		util_sys_exec(cmd);

		sync();
		OSA_waitMsecs(300);
		app_mcu_pwr_off(OFF_RESET);
	}
}

void ctrl_reset_nand_update(void)
{
	int i;
	char cmd[MAX_CHAR_255] = {0,};

	//# delete full updated file
	if(dev_fw_printenv("nand_update") == 2)
	{
        if(_is_firmware_for_release()) // RELEASE Version .. --> update file delete
		{
		    for(i=0; i<FW_FILE_NUM; i++) {
			    sprintf(cmd, "rm -rf %s/%s", SD_MOUNT_PATH, full_upfiles[i]);
				printf("@@@@@@@@@ DELETE %s @@@@@@@@@@@@\n",full_upfiles[i]);
			    util_sys_exec(cmd);
		    }
			
			/* delete rfs_fit.ubifs.md5 */
			memset(cmd, 0, sizeof(cmd));
			snprintf(cmd, sizeof(cmd), "/mmc/rfs_fit.ubifs.md5");
			if (access(cmd, F_OK) == 0)
				remove(cmd);
		}
		
		dev_fw_setenv("nand_update", "0", 0);
	}
}

//########################### End of Firmware Update ############################################
/*
 * testing live555 process 
 */  
int ctrl_is_live_process(const char *process_name)
{   
    DIR* pdir;
    struct dirent *pinfo;
    int is_live = 0;

    pdir = opendir("/proc");
    if (pdir == NULL)
    {
        printf("err: NO_DIR\n");
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
        if(fp)
        {
            fgets(buff, 128, fp);
            fclose(fp);
        
//            if(strstr(buff, P2P_NAME))   
            if(strstr(buff, process_name))   
            {
                is_live = 1;
                break;
            }
        }
        else
        {
            dprintf("Can't read file [%s]\n", path);
        }
    }

    closedir(pdir);

    return is_live;
}

/*
 * @param : fwpath : full path (maybe /tmp/rfs_fit.ubifs)
 */
int ctrl_update_firmware_by_cgi(char *fwpath)
{
    app_leds_fw_update_ctrl();
	
	int ret = temp_ctrl_update_fw_by_bkkim(fwpath, SD_MOUNT_PATH);
	if (ret < 0)
	{
		app_leds_sys_normal_ctrl();
		app_rec_start();
	}

	return ret ;     
}

/*
 * record stop and reboot.
 */
void ctrl_sys_reboot(void)
{
    int recording = 0;

    recording = app_rec_state();
    if (recording) {
        app_rec_stop(1); /* buzzer on */
		app_msleep(500);
    }
	app_file_save_flist(); /* save file list */
#if SYS_CONFIG_VOIP
	app_voip_save_config(); /* save voip volume */	
#endif
//    app_file_exit(); ??
    app_set_write();
	app_buzz_ctrl(80, 2); //# Power Off Buzzer
	app_mcu_pwr_off(OFF_RESET);
}

/*
 * record stop and halt.
 */
void ctrl_sys_shutdown(void)
{
    int recording = 0;

    recording = app_rec_state();
    if (recording) {
		app_rec_stop(0);
    }
	app_buzz_ctrl(80, 2); //# Power Off Buzzer
	if (recording) {
		/* 먼저 buzzer를 울리기 위해서 */
		app_msleep(500);	
	}
	app_file_save_flist(); /* save file list */
#if SYS_CONFIG_VOIP
	app_voip_save_config(); /* save voip volume */	
#endif
//    app_file_exit(); ??
    app_set_write();
	app_mcu_pwr_off(OFF_NORMAL);
}
