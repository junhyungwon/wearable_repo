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
#include <fcntl.h>

#include "ti_vsys.h"
#include "ti_vcap.h"
#include "ti_venc.h"
#include "ti_vdis.h"
#include <errno.h>
#define __USE_GNU
#include <pthread.h>
#include <sys/ioctl.h>

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
#include "app_rec.h"
#include "app_cap.h"
#include "app_rtsptx.h"
#include "app_file.h"
#include "app_mcu.h"
#include "app_buzz.h"
#include "app_watchdog.h"
#include "app_version.h"
#include "app_rsa.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define FW_EXT       			".dat"
#define KEY_NAME 		        "authorized_keys"
#define RSA_PATH                "/root/.ssh/"
#define RSA_FULL_PATH           "/root/.ssh/authorized_keys"

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 firmware update
-----------------------------------------------------------------------------*/
#define FW_FILE_NUM 			8
#define FW_TYPE     			0   //# "release" or "debug"
#define DEV_MODEL				1	//# "NEXXONE"
#define F_RELEASE   			"release"

#define FW_DIR      			SD_MOUNT_PATH //# "mmc"
#define FW_BOOTSCR_FNAME		"boot.scr"
#define FW_MLO_FNAME			"MLO"
#define FW_UBOOTMIN_FNAME		"u-boot_fit.min.nand"
#define FW_UBOOT_FNAME			"u-boot_fit.bin"
#define FW_VINFO_FNAME			"fw_version.txt"
#define FW_KERNEL_FNAME			"uImage_fit"
#define FW_APP_FNAME			"app_fitt.out"

#ifdef EXT_BATT_ONLY
#define FW_MICOM_FNAME			"mcu_extb.txt"
#else
#if defined(NEXX360C) || defined(NEXX360W_CCTV)
#define FW_MICOM_FNAME			"mcu_cctv.txt"
#else
#define FW_MICOM_FNAME			"mcu_fitt.txt"
#endif /* #if defined(NEXX360C) || defined(NEXX360W_CCTV) */
#endif /* #ifdef EXT_BATT_ONLY */

#define FW_UBIFS_FNAME			"rfs_fit.ubifs"
#define FW_UBIFSMD5_FNAME		"rfs_fit.ubifs.md5"

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

typedef struct {
    char item[8];
    char value[32];
} fw_version_t;

//------------------------------------------------------//
static int _is_firmware_for_release(void)
{
    fw_version_t fw[FW_FILE_NUM];
    FILE *F_fw;
	char path[256]={0,};
	int i=0, ret = FALSE;
	
	snprintf(path, sizeof(path), "%s/%s", FW_DIR, FW_VINFO_FNAME);
    F_fw = fopen(path, "r"); //# /mmc/fw_version.txt
    if (F_fw != NULL) {
        while (!feof(F_fw)) {
            fscanf(F_fw,"%s %s", fw[i].item, fw[i].value);
            i++;        

            if (i == FW_FILE_NUM)
                break;          
        }       
        fclose(F_fw);

        if (strcmp(fw[FW_TYPE].value, F_RELEASE) == 0) {
           	TRACE_INFO("\n TYPE RELEASE!!\n");
        	ret = TRUE;
        }

		if (strcmp(fw[DEV_MODEL].value, MODEL_NAME) != 0) {
			LOGD("[main] This Firmware file is not for %s !!!\n", MODEL_NAME);
			ret = EFAIL;
		}
    }
    return ret ;
}

//# find /mmc/*.dat 
static char *_findFirmware(const char *root)
{
	static char extPath[255];
	char path[255]={0,};
	glob_t globbuf;

	memset(&globbuf, 0, sizeof(glob_t));
	memset(extPath, 0, sizeof(extPath));
	
	//# /mmc/*.dat scanning
	//# FW_EXT -> .dat
	snprintf(path, sizeof(path), "%s/*%s", root, FW_EXT);
	if (glob(path, GLOB_DOOFFS, NULL, &globbuf) == 0) {
		if(globbuf.gl_pathc > 0) {
			strcpy(extPath,globbuf.gl_pathv[0]);
			globfree(&globbuf);
			return extPath;
		}
	} else {
		TRACE_INFO("Not found firmware file in %s\n", root);
	}
	globfree(&globbuf);

	return NULL;
}

static int _unpack_N_check(const char* pFile, const char* root, int* release)
{
	char buf[256]={0,};
	int ret = SOK;
	
	sprintf(buf, "/bin/tar xvf %s -C %s", pFile, root);
	system(buf);
	sync();
	sleep(1); /* wait for sync complate */
	
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "%s/%s", FW_DIR, FW_VINFO_FNAME);
	if(-1 == access(buf, 0)) {
		LOGD("[main] Couldn't access %s!!!\n", buf);
		return EFAIL;
	}

	*release = _is_firmware_for_release();
	//# *release TRUE = release(normal). *release == EFAIL (model name is not matched). release == FALSE (factory release)
	if (*release == EFAIL) 
		ret = EFAIL;
	
	return ret;
}

/* check mcu firmware and delete */
static void _check_micom_update(void)
{
	char cmd[256]={0,};
	char path[256]={0,};
	FILE *f = NULL;
	
	unsigned int byte1, byte2;
	int ret = 0, mcu_ver;
	
	snprintf(path, sizeof(path), "%s/%s", FW_DIR, FW_MICOM_FNAME);
	f = fopen(path, "r");
	if (f != NULL) 
	{
		/*
		 * 1st: @f000
		 * 2nd: 20 00 .....(ascii 코드로 구성)
		 *    : 0x32 0x30 0x20(space) 0x30 0x30
		 */
		memset(cmd, 0, sizeof(cmd));
		fgets(cmd, sizeof(cmd), f);
		/* 2번째 라인에 버전 정보가 있다. */
		fgets(cmd, sizeof(cmd), f);
		sscanf(cmd, "%02x %02x\n", &byte1, &byte2);
		ret = ((byte2 << 8) | byte1);
		//TRACE_INFO("ver = 0x%04x\n", ret);
		fclose(f);
		
		mcu_ver = ctrl_get_mcu_version(NULL);
		if ((ret > 0) && (ret == mcu_ver)) {
			if (access(path, F_OK)==0) {
				remove(path);
				//# update 함수 마지막에 sync() 가 호출됨
				//sync();
				//TRACE_INFO("remove %s\n", path);
			}
		}
	} else {
		TRACE_ERR("Not founded micom update file!\n");
	}
}

/* only application binary update */
static int __app_only_replace(void)
{
	char path[64] = {0, };
	char cmd[255] = {0, };
	
	snprintf(path, sizeof(path), "%s/%s", FW_DIR,  FW_APP_FNAME);
	if (0 == access(path, 0)) { // existence only
		TRACE_INFO("\n######### COPY APP_FIIT.OUT !!!! #########\n");
		sprintf(cmd, "/bin/cp %s /opt/fit/bin/.", path);
		util_sys_exec(cmd);
		sprintf(cmd, "/bin/rm %s", path);
		util_sys_exec(cmd);
		sync();
		//# wait for safe
		app_msleep(500);		
		return 0;
	}
	return -1;
}

static int __normal_update(void)
{
	char cmd[256]={0,};
	char *pFile = NULL;
	
	int ret = 0;
	int release = 1;
	
	TRACE_INFO("start...\n");
	
	//# buzz: update
	app_buzz_ctrl(50, 3);
	
	/* firmware update 시 종료키 이벤트 skip */
	app_cfg->ste.b.busy = 1;
	pFile = _findFirmware(FW_DIR); //# /mmc
	if (pFile == NULL) {
		TRACE_INFO("Firmware file is not exist !!!\n");
        app_cfg->ste.b.busy = 0;
		return EFAIL;
	}
	
	//# unpack fw file and type check, release/debug and update full or binary only
	// pFile = /mmc/xxxxxx.dat
	// disk  = /mmc
	if (_unpack_N_check((const char *)pFile, (const char *)FW_DIR, &release) == EFAIL) {
		LOGE("[main] Founded Firmware File, but It's not matched model name! cancel update!\n");
        ret = EFAIL;
		/* TODO : delete unpack update files.... */
		goto fw_exit;
	}
	
	_check_micom_update();
	//# LED work for firmware update.
	app_leds_fw_update_ctrl();
	dev_fw_setenv("nand_update", "1", 0);
	LOGD("[main] Starting Firmware update....system will be restart!\n");
	ret = SOK;
	
fw_exit:
	//# clear busy flag...
	app_cfg->ste.b.busy = 0;
	if (release) { // RELEASE Version .. --> update file delete
	    sprintf(cmd, "rm -rf %s", pFile);
	    system(cmd);
    }
	sync();
	//# wait for safe
	app_msleep(500);
	return ret;
}

static int __emergency_update(void)
{
	char *pFile = NULL;
	char cmd[256]={0,};
	
	/* firmware update 시 종료키 이벤트 skip */
	app_cfg->ste.b.busy = 1;
	//# /mmc/firmware/*.dat 파일이 있는 지 확인.
	pFile = _findFirmware(EMERGENCY_UPDATE_DIR);
	if (pFile == NULL) {
		TRACE_INFO("Emergency Firmware file is not exist !!!\n");
        app_cfg->ste.b.busy = 0;
		return -1;
	}
	
	//# buzz: update
	TRACE_INFO("start...\n");
	app_buzz_ctrl(50, 3);
	
	/* unpack firmware */
	sprintf(cmd, "/bin/tar xvf %s -C %s", pFile, FW_DIR);
	system(cmd);
	sync();
	/* wait untar done! */
	sleep(1);
	
	/* 만일 micom 버전이 현재 버전과 같은면 업데이트 파일 삭제 */
	_check_micom_update();
	
	//# LED work for firmware update.
	app_leds_fw_update_ctrl();
	dev_fw_setenv("nand_update", "1", 0);
	
	//# clear busy flag...
	app_cfg->ste.b.busy = 0;
	
	//# delete update files.
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "/bin/rm -rf %s", pFile);
	system(cmd);
	sync();
	//# wait for safe
	app_msleep(500);
		
	return 0;
}

static void Delete_updatefile(void)
{
    char cmd[MAX_CHAR_255] = {0,};
    int i;
	char *full_upfiles[FW_FILE_NUM-1] = {
		FW_BOOTSCR_FNAME, 
		FW_UBOOTMIN_FNAME, 
		FW_UBOOT_FNAME, 
		FW_MLO_FNAME, 
		FW_VINFO_FNAME,
		FW_KERNEL_FNAME, 
		FW_UBIFS_FNAME
	};
	
	//# remove micom firmware,-->(-1) 
	for (i = 0; i < FW_FILE_NUM-1; i++) {
		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "%s/%s", FW_DIR, full_upfiles[i]);
		if (access(cmd, F_OK) == 0) {
			remove(cmd);
			TRACE_INFO("@@@@@@@@@ DELETE %s @@@@@@@@@@@@\n", cmd);	
		}
	}

	//# delete "/mmc/rfs_fit.ubifs.md5"
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "%s/%s", FW_DIR, FW_UBIFSMD5_FNAME); 
	if (access(cmd, F_OK) == 0) {
		remove(cmd);
		TRACE_INFO("@@@@@@@@@ DELETE %s @@@@@@@@@@@@\n", cmd);
	}
	
	//# delete micom firmware
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "%s/%s", FW_DIR, FW_MICOM_FNAME);
	if (access(cmd, F_OK) == 0) {
		remove(cmd);
		TRACE_INFO("@@@@@@@@@ DELETE %s @@@@@@@@@@@@\n", cmd);
	}
	
	//# delete mcu_extb.txt
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "/mmc/mcu_extb.txt");
	if (access(cmd, F_OK) == 0) {
		remove(cmd);
		TRACE_INFO("@@@@@@@@@ DELETE %s @@@@@@@@@@@@\n", cmd);
	}
	
	// delete mcu_cctv.txt
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "/mmc/mcu_cctv.txt");
	if (access(cmd, F_OK) == 0) {
		remove(cmd);
		TRACE_INFO("@@@@@@@@@ DELETE %s @@@@@@@@@@@@\n", cmd);
	}
}

void *thrRunFWUpdate(void *arg)
{
	LOGD("[main] Web Remote Update Temp version Firmware update done....\n");

	app_buzz_ctrl(50, 3); //# buzz: update
	dev_fw_setenv("nand_update", "1", 0);
	sync();

	TRACE_INFO("\nfw update ready ! It will restart\n\n");
	app_mcu_pwr_off(OFF_RESET);
	return NULL;
}

/* example remote update
 *
curl -v -u admin:1111 --http1.0 -F 'fw=@bin/fitt_firmware_full_N.dat' http://192.168.40.129/cgi/upload.cgi
*/
int temp_ctrl_update_fw_by_bkkim(char *fwpath, char *disk)
{
	char cmd[256]={0,};
	int ret, release;
	
	/* recording stop */
	ret = app_rec_state();
	if (ret) {
	    app_cfg->ste.b.prerec_state = 1 ;
		app_rec_stop(OFF);
		sleep(1); /* wait for file close */
	}

	// check tar content list	
#if 1
	{
		sprintf(cmd, "/bin/tar tf %s > /tmp/fw.list", fwpath);
		TRACE_INFO("cmd:%s\n", cmd);
		system(cmd);
		
		FILE *fp = fopen("/tmp/fw.list", "r");
		if (!fp)
		{
			TRACE_ERR("failed /bin/tar tf %s > /tmp/fw.list\n", fwpath);
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

				if(strcmp(line, FW_BOOTSCR_FNAME)==0){
					TRACE_INFO("checked %s\n", FW_BOOTSCR_FNAME);
				}
				else if(strcmp(line, FW_MLO_FNAME)==0){
					TRACE_INFO("checked %s\n", FW_MLO_FNAME);
				}
				else if(strcmp(line, FW_VINFO_FNAME)==0){
					TRACE_INFO("checked %s\n", FW_VINFO_FNAME);
				}
				else if(strcmp(line, FW_UBIFS_FNAME)==0){
					TRACE_INFO("checked %s\n", FW_UBIFS_FNAME);
				}
				else if(strcmp(line, FW_UBIFSMD5_FNAME)==0){
					TRACE_INFO("checked %s\n", FW_UBIFSMD5_FNAME);
				}
			}
		}
		fclose(fp);
	}
#endif

#if 1 // untar
	sprintf(cmd, "/bin/tar xvf %s -C %s", fwpath, disk);
	//sprintf(cmd, "cp -f %s %s/", fwpath, disk);
	TRACE_INFO("fwupdate cmd:%s\n", cmd);
	system(cmd);
	sync();
#endif
    release = _is_firmware_for_release() ; 
    if((EFAIL == release) || (FALSE == release)) // RELEASE Version .. --> update file delete
	{
		TRACE_INFO("The model does not match, It is not release version, or the fw_version.txt is missing.\n");
		Delete_updatefile() ;	
		return -1;
    }
	else // release or factory release version
	{
		app_file_exit(); /* 파일리스트 갱신 작업이 추가됨 */
		TRACE_INFO("Good, this must be my pot\n. And the next checking is very horrible md5sum. Good luck!!\n");
	}

	//# micom version check..
	_check_micom_update();
	// check md5sum
	sprintf(cmd, "cd %s && md5sum -c %s", disk, FW_UBIFSMD5_FNAME);
	FILE *fp = popen(cmd, "r");
	if(fp){
		char line[255]={0};
		fgets(line, 255, fp);
		//TRACE_INFO("%s\n", line);
		if(NULL == strstr(line, " OK")){
			//TODO: 실패할 경우, 압축해제한 파일들 처리
			pclose(fp);
			return -1;
		}

		pclose(fp);
		// OK, ready to firmware upgrade
	}
	else {
		TRACE_ERR("Failed popen(md5sum -c %s) , please check firmware file!!\n", FW_UBIFSMD5_FNAME);
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
			TRACE_ERR("thrRunFWUpdate pthread_create failed, ret = %d\r\n", ret);
			return -1;
		}
		pthread_setname_np(tid_fw, __FILENAME__);
		pthread_detach(tid_fw);
	}
#endif
    // 파일 삭제를 하면 업데이트가 안됨.
	//if(FALSE == release)
	//    Delete_updatefile() ;
	
	return 0;
}

/*
 * if /mmc/app_fitt.out is exist, copy app_fitt.out to /opt/fit/bin/.
 * if /mmc/fitt_firmware_full_N.dat is exist, firmware update...
 */
void ctrl_firmware_update(void)
{
	/* 1. application binary만 업데이트 하는 경우 */
	if (__app_only_replace() == 0) {
		app_mcu_pwr_off(OFF_RESET);
		//# 다른 업데이트 진행 안되게 막음.
		return;
	}
	
	/* 2. 일반 업데이트 */
	if (__normal_update() == 0) {
		app_mcu_pwr_off(OFF_RESET);
		//# 다른 업데이트 진행 안되게 막음.
		return;
	}
	
	/* 3. 강제 업데이트 (basic / wireless 전환을 위해서) */
	if (__emergency_update() == 0) {
		app_mcu_pwr_off(OFF_RESET);
		//# 다른 업데이트 진행 안되게 막음.
		return;
	}
}

void ctrl_reset_nand_update(void)
{
	//# delete full updated file
	if(dev_fw_printenv("nand_update") == 2){
        if(_is_firmware_for_release()) // RELEASE Version .. --> update file delete
		    Delete_updatefile() ;
		
		dev_fw_setenv("nand_update", "0", 0);
	}
}
//########################### End of Firmware Update ############################################

/*****************************************************************************
* @brief    rate control (VRB/CBR)
* @section  [desc]
*****************************************************************************/
int ctrl_vid_rate(int ch, int rc, int br)
{
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };
  
    params.rateControl = rc ;

    app_set->ch[ch].rate_ctrl = rc;
	Venc_setDynamicParam(ch, 0, &params, VENC_RATECONTROL);  // set rate control

	if(rc == RATE_CTRL_VBR)
	{
        LOGD("[main] --- ch %d set vid rate control to VBR ---\n", ch);
		params.qpMax 	= 45;
		params.qpInit 	= -1;
        params.qpMin    = 2;
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
        LOGD("[main] --- ch %d set vid rate control to CBR ---\n",ch);
 
		params.qpMin	= 2;//10;	//# for improve quality: 10->0
		params.qpMax 	= 40;
		params.qpInit 	= -1;
	}

	Venc_setDynamicParam(ch, 0, &params, VENC_QPVAL_P);  // set QP range

	return SOK;
}

/*****************************************************************************
* @brief    set multislice mode
* @section  [desc]
*****************************************************************************/
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
	LOGD("[main] --- ch %d set frame rate %d FPS ---\n", ch, framerate);
	// to do rec stop
    if(ch != MODEL_CH_NUM)
	{
        ret = app_rec_state() ;
	    if (ret) { 
	        app_rec_stop(ON);
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

    app_set->ch[ch].quality = bitrate;
	// resol 480P 0 720P 1 1080P 2
//    app_cfg->ich[ch].br = (br * app_cfg->ich[ch].fr)/DEFAULT_FPS;
    app_cfg->ich[ch].br = bitrate;
    params.targetBitRate = app_cfg->ich[ch].br*1000 ;
    LOGD("[main] --- ch %d set bit rate %d(Kbps) ---\n", ch, (bitrate*1000));
	
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
    int ret = 0 ;
	char sw_ver[128] = {0, } ;

    //# to prevent key input
    app_cfg->ste.b.nokey = 1;
    Vdis_disp_ctrl_exit();

// to do rec stop
    ret = app_rec_state() ;
    if (ret) {
        app_rec_stop(ON);
	    sleep(1); 
	}
	app_cfg->ste.b.rec = 1; // previous flag for capture start
		
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
	
    snprintf(sw_ver, sizeof(sw_ver), "%s", FITT360_SW_VER);
	ctrl_swosd_userstr(sw_ver, 1);

	switch(resol_idx) {
	default:
	case 0:
		LOGD("[main] --- change Display Mode to 480P ---\n");
		break;
	case 1:
		LOGD("[main] --- change Display Mode to 720P ---\n");
		break;
	case 2:
		LOGD("[main] --- change Display Mode to 1080P ---\n");
		break;
    }
	
    app_cfg->ste.b.nokey = 0;
	TRACE_INFO("vid_resolution nokey = %d\n",app_cfg->ste.b.nokey) ;

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
		int rec_ste = 0;

		Vdis_disp_ctrl_exit();
		rec_ste = app_rec_state();
		if (rec_ste) {
			app_rec_stop(ON);
			sleep(1);
		}

		app_cap_stop();
		app_buzz_ctrl(100, 1);
		app_msleep(200);
		app_set->ch[ch].resol = resol;
		Vdis_disp_ctrl_init(resol);
		app_cap_start();
		
		/* previous video record state */
		if (rec_ste) {
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
	if (!net_type)  // STATIC
	{
		if (strcmp(token, "wlan0") == 0) {
			if (ipaddr != NULL)
				strcpy(app_set->net_info.wlan_ipaddr, ipaddr);
			if (subnet != NULL)
				strcpy(app_set->net_info.wlan_netmask, subnet);
			
			LOGD("[main] --- Wireless network set a static ip address. System will be restart! ---\n");
		} else {
			if (ipaddr != NULL)
				strcpy(app_set->net_info.eth_ipaddr, ipaddr);
			if (subnet != NULL)
				strcpy(app_set->net_info.eth_netmask, subnet);
			
			LOGD("[main] --- Ethernet network set a static ip address. System will be restart! ---\n");
		}
	}	
	
	app_set->net_info.type = net_type;
	ctrl_sys_halt(0); /* reboot */
	
    return SOK ;
}

/*****************************************************************************
* @brief    set network gateway info(gateway)
* @section  [desc]
*****************************************************************************/
int ctrl_set_gateway(const char *gw)
{
    if (gw != NULL)
        strcpy(app_set->net_info.eth_gateway, gw);
	
	LOGD("[main] --- Ethernet network set a gateway ip address. System will be restart! ---\n");
	ctrl_sys_halt(0); /* reboot */	

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
	if (version != NULL)
		sprintf(version, "%02d.%02X", (ver>>8)&0xFF, ver&0xFF);

	return ver;
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
void ctrl_swosd_callstatus(int ch, int draw)
{
	Vsys_swOsdPrm swosdGuiPrm;

	swosdGuiPrm.streamId = ch;
	swosdGuiPrm.enable = draw;

	Vsys_setSwOsdPrm(VSYS_SWOSD_CALLSTAT, &swosdGuiPrm);
}

void ctrl_swosd_userstr(char *str, int draw)
{
	Vsys_swOsdPrm swosdGuiPrm;

	swosdGuiPrm.streamId = 0;
	swosdGuiPrm.enable = draw;
	strcpy((char*)swosdGuiPrm.usrString, str);

	Vsys_setSwOsdPrm(VSYS_SWOSD_USERSTR, &swosdGuiPrm);
}

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
        TRACE_ERR("err: NO_DIR\n");
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
		
		/*
		 * d_type = 4->DT_DIR, d_name : filename
		 * if d_name[0] > 57, then character (ascii 57-->'9')
		 */
        if (pinfo->d_type != 4 || pinfo->d_name[0] == '.' || pinfo->d_name[0] > 57)
            continue;

        sprintf(path, "/proc/%s/status", pinfo->d_name);
        fp = fopen(path, "rt");
        if(fp)
        {
            fgets(buff, 128, fp);
            fclose(fp);
        
            if(strstr(buff, process_name))   
            {
                is_live = 1;
                break;
            }
        }
        else
        {
            TRACE_ERR("Can't read file [%s]\n", path);
        }
    }

    closedir(pdir);
    return is_live;
}

int ctrl_update_rsakey(char *fwpath)
{
	char cmd[256] = {0, } ;
	
	if(access(RSA_PATH, F_OK) != 0)
	{
		mkdir(RSA_PATH, 0775) ;
	}
	sprintf(cmd, "cat %s >> %s", fwpath, RSA_FULL_PATH) ;
	FILE *fp = popen(cmd, "r") ;
	if(!fp)
	{
		TRACE_ERR("failed cat %s >> %s",fwpath, RSA_FULL_PATH) ;
		pclose(fp) ;
		return -1 ;
	}
	else
	{
		pclose(fp) ;
		memset(cmd, 0x00, 256) ;
		sprintf(cmd, "/etc/init.d/sshd.sh restart") ;
		TRACE_INFO("%s\n","sshd restart.... ") ;
	}

	fp = popen(cmd, "w") ;

	if(!fp)
	{
		TRACE_ERR("failed sshd start !! \n") ;
		pclose(fp) ;
		return -1 ;
	}
		TRACE_INFO("%s\n","sshd restart.... ") ;
		pclose(fp) ;

	return 0 ;		
}

/*
 * @param : fwpath : full path (maybe /tmp/rfs_fit.ubifs)
 */
int ctrl_update_firmware_by_cgi(char *fwpath)
{
	int ret = temp_ctrl_update_fw_by_bkkim(fwpath, FW_DIR);
	if (ret < 0)
	{
        if(app_cfg->ste.b.prerec_state)
		    app_rec_start();
	}
	else
	    app_leds_fw_update_ctrl();

	return ret ;     
}

int app_sshd_start()
{
	char cmd[256] = {0, } ;
	int ret = 0 ;

	sprintf(cmd, "/etc/init.d/sshd.sh start") ;
	FILE *fp = popen(cmd, "w") ;
	if(fp)
	{
		TRACE_INFO("%s\n","sshd start.... ") ;
		ret = TRUE ;
	}
	pclose(fp) ;	
	return ret ;
}


/*
 * record stop and reboot.
 */
void ctrl_sys_halt(int shutdown)
{
    int ste = 0;
	
	/* watchdog keep alive disable */
	/* 종료 시 특정 쓰레드에서 루프에 빠지는 경우 */
	/* alive가 실행되어 Watchdog이 안되는 문제 수정 */
	app_watchdog_exit();
	app_rsa_passphrase_to_sd();

	ste = app_rec_state();
	if (shutdown) {
		if (ste) {
			app_rec_stop(OFF);
		}
		app_buzz_ctrl(80, 2); //# Power Off Buzzer
		if (ste)
			app_msleep(1500); /* 먼저 buzzer를 울리기 위해서 */	
	} 
	else {
		/* reboot */
		if (ste) {
       		app_rec_stop(OFF);
			app_msleep(500);
    	}
	}
	app_file_exit(); /* 파일리스트 갱신 작업이 추가됨 */
    TRACE_INFO("file manager exit success.(while system %s!)\n", shutdown ? "shutdown" : "reboot");
	app_set_write();
	
	if (shutdown) {
		app_mcu_pwr_off(OFF_NORMAL);
	} else {
		app_buzz_ctrl(80, 2); //# Power Off Buzzer
		app_mcu_pwr_off(OFF_RESET);
	}
	
	TRACE_INFO("....exit!\n");
}
