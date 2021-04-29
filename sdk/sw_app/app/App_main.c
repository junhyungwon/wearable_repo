/******************************************************************************
 * FITT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_main.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2017.03.22 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

//# include mcfw_linux
#include "ti_vsys.h"

#include "app_version.h"
#include "app_comm.h"
#include "app_main.h"
#include "app_gmem.h"
#include "app_cap.h"
#include "app_rec.h"
#include "app_dev.h"
#include "app_mcu.h"
#include "app_ctrl.h"
#include "app_ftp.h"
#include "app_set.h"
#include "app_rtsptx.h"
#include "app_onvifserver.h"
#include "app_networkcfg.h"
#include "app_gui.h"
#include "app_timesrv.h"
#include "app_sockio.h"
#include "app_file.h"
#include "app_p2p.h"
#include "app_web.h"
#include "app_encrypt.h"
#include "app_snd.h"
#include "app_fms.h"
#include "app_gps.h"
#include "app_netmgr.h"
#include "app_ipinstall.h"
#include "app_buzz.h"
#include "app_libuv.h"

#ifdef USE_RTMP
#include "app_rtmp.h"
#endif

#if SYS_CONFIG_VOIP
#include "app_voip.h"
#endif

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static app_cfg_t cfg_obj;
app_cfg_t *app_cfg = &cfg_obj;

/*----------------------------------------------------------------------------
 main thread function - use only communication
-----------------------------------------------------------------------------*/
static int main_thread_init(void)
{
	app_thr_obj *tObj = &app_cfg->mObj;

	//#--- create thread
	if(thread_create(tObj, NULL, APP_THREAD_PRI, NULL, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}

	return 0;
}

static void main_thread_exit(void)
{
	app_thr_obj *tObj = &app_cfg->mObj;

	//#--- stop thread
	thread_delete(tObj);
}

static void app_setdns(void)
{
   	FILE *fp = NULL ;
   	char buffer[64]= {0,};
	
//   sprintf(buffer, "%s", "nameserver 8.8.8.8") ;

	fp = fopen("/etc/resolv.conf","w") ;
	if (fp != NULL)
	{
		sprintf(buffer, "nameserver %s\n", app_set->net_info.dns_server1) ;
		fwrite(buffer, strlen(buffer), 1, fp);
		app_log_write(MSG_LOG_WRITE, buffer);
		
		sprintf(buffer, "nameserver %s\n", app_set->net_info.dns_server2) ;
		fwrite(buffer, strlen(buffer), 1, fp);
		app_log_write(MSG_LOG_WRITE, buffer);
		
		fclose(fp);
	}
}

static int __mmc_prepare(void)
{
	DIR *logdir=NULL;
	FILE *f=NULL;
	
	char buf[256 + 1]={0,};
	char mntd[64]={0,};
	int res=0;
	
	/* mmc mount 확인 */
	app_cfg->ste.b.mmc = 0; //# default 0
	/* SD 카드 속성확인 (read only file system) */
	f = fopen("/proc/mounts", "r");
	if (f == NULL) {
		return -1;
	}
	
	/* 
	 * normal fs       -> /dev/mmcblk0p1 /mmc vfat rw,noatime,nodiratime,fmask=0000,
	 * if read-only fs -> /dev/mmcblk0p1 /mmc vfat ro,noatime,......
	 */
	while (fgets(buf, 256, f) != NULL) 
	{
		char *tmp, *tmp2;
		/* %*s->discard input */
		sscanf(buf, "%*s%s", mntd);
		if (strcmp(mntd, "/mmc") == 0) {
			tmp = strtok(buf, ",");
			if (tmp != NULL) {
				/* */
				tmp2 = strstr(tmp, "ro");
				if (tmp2 != NULL) {
					fprintf(stderr, "sd card read-only filesystem!!\n");
					fclose(f);
					return -1;
				}
			}
		}
	}
	fclose(f);
	
	/* log 디렉토리 생성 */
	logdir = opendir("/mmc/log") ;
	if (logdir == NULL) {
		mkdir("/mmc/log", 0775);
	} else 
		closedir(logdir);
	sync();		
	
	/* mmc prepare 성공 */
	app_cfg->ste.b.mmc = 1;
				
	return SOK;
}

static void __syslogd_enable(int en)
{
	char command[128]={0,};
	
	if (en) {
		snprintf(command, sizeof(command), "/etc/init.d/logging.sh start");
	} else {
		snprintf(command, sizeof(command), "/etc/init.d/logging.sh stop");
	}
	system(command);
}

/*****************************************************************************
* @brief    app main function
* @section  [desc] console menu
*****************************************************************************/
int app_main(void)
{
    char msg[128] = {0, };
    char micom_ver[128] = {0, } ;

	app_thr_obj *tObj = &app_cfg->mObj;
	#if !USE_CONSOLE_MENU
	char cmd, exit=0;
	#endif

	aprintf("enter...\n");
	tObj->active = 1;

	app_mcu_start();
    sprintf(msg, ">>>>> %s_SYSTEM STARTED!! <<<<<", FITT360_SW_VER);
    app_log_write(MSG_LOG_WRITE, msg);

    ctrl_get_mcu_version( micom_ver ) ;
    sprintf(msg, "SW_Ver: %s, HW_Ver: %s, Micom_Ver: %s",FITT360_SW_VER, FITT360_HW_VER, micom_ver);
    app_log_write(MSG_LOG_WRITE, msg);

    app_libuv_start();
#ifdef USE_RTMP
    app_rtmp_start();

    // disable SIGPIPE
    sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);

    // enable in default
    app_rtmp_enable();
#endif

    app_cap_start();
	app_snd_start(); 
	app_netmgr_init();
	
    if (!app_set->sys_info.osd_set)
        ctrl_swosd_enable(STE_DTIME, 0, 0) ;  // osd disable 

	if (app_set->srv_info.ON_OFF)
        app_fms_init() ;

	app_rtsptx_start();

	//if(app_set->net_info.enable_onvif==1)
	{
		if (app_onvifserver_start() == 0) {
			app_cfg->ste.b.onvifserver = 1; // have to add this flag....
		}
	}
	
	app_web_boot_passwordfile();
	if (app_web_start_server() ==0) {
        app_cfg->ste.b.web_server = 1; // have to add this flag....
	}

    CSock_init() ;

	if (app_set->ftp_info.ON_OFF)
        app_ftp_init();
	
    if (app_set->sys_info.P2P_ON_OFF == ON) 
	{
        add_p2p_account() ;
//        app_p2p_start() ;  // p2p_init 과 동시 동작시 p2p server 2개 동시 동작되는 경우 발생 
        app_p2p_init() ;
    }

#if SYS_CONFIG_VOIP	
	app_voip_init();
#endif	
	app_buzz_ctrl(80, 2);	//# buzz: power on

	while(!exit)
	{
		//# wait cmd
		cmd = event_wait(tObj);
		if(cmd == APP_CMD_EXIT) {
			break;
		}
	}
    CSock_exit() ;

	//if(app_set->net_info.enable_onvif==1)
	{
		if (app_cfg->ste.b.onvifserver) // have to add this flag....
		{
			app_onvifserver_stop();
			app_cfg->ste.b.onvifserver = 0;
		}
	}
    
    if(app_set->sys_info.P2P_ON_OFF == ON)
    {
        app_p2p_exit() ;
        app_p2p_stop() ;
    }

    app_rec_stop(0);
    app_snd_stop(); 
    app_cap_stop();

#ifdef USE_RTMP
    app_rtmp_stop();
#endif
    app_libuv_stop();

    if (app_cfg->ste.b.rtsptx) {
        app_rtsptx_stop();
    }
    
	app_tsync_exit() ;
	app_netmgr_exit();
	app_mcu_stop();
	tObj->active = 0;

	app_leds_init(); /* set leds off state */

	app_log_write(MSG_LOG_WRITE, "[APP_FITT360] app_main() EXIT....");

	aprintf("exit\n");

	return SOK;
}

/*****************************************************************************
* @brief    file manage control
* @section  [desc]
*****************************************************************************/
void app_main_ctrl(int cmd, int p0, int p1)
{
	event_send(&app_cfg->mObj, cmd, p0, p1);
}

/*----------------------------------------------------------------------------
 application config function
-----------------------------------------------------------------------------*/
int app_cfg_init(void)
{
	memset((void *)app_cfg, 0, sizeof(app_cfg_t));

	//# module ctrl
	app_cfg->en_rec = 1;

	//# display ctrl
	app_cfg->en_hdmi = 0; //0; default enable

    app_cfg->en_jpg = 1 ;
	app_cfg->ftp_enable = OFF ;

    app_cfg->tvo_flag |= TVO_REC;
    app_cfg->tvo_flag |= TVO_TEMP;
    app_cfg->tvo_flag |= TVO_VOLT;
    app_cfg->tvo_flag |= TVO_GSENSOR;
    app_cfg->tvo_flag |= TVO_MOTION;
    app_cfg->tvo_flag |= TVO_MIC;
    app_cfg->tvo_flag |= TVO_BUZZER;
    app_cfg->tvo_flag |= TVO_VERSION;

	return 0;
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
int main(int argc, char **argv)
{
	unsigned long part_size = 0;
	int ret, mmc_state;

	printf("\n--- FITT360 start (v%s) ---\n", FITT360_SW_VER);
	
	/* micom ready 신호 전달 */
	app_mcu_init();
	//# LED 초기 상태 설정
	app_leds_init();
	app_buzz_init(); //# buzzer mutex init..	
	app_cfg_init();
	
	//# ------- SD 카드 상태 확인 및 마운트 점검 ----------------------
	ret = ctrl_mmc_check_exfat(&part_size);
	if (ret == 1) {
		ctrl_mmc_exfat_format(part_size);
	}
	
	/* mmc prepare: 쓰기 가능한 지 확인 및 log 디렉토리 생성 */
	mmc_state = __mmc_prepare();
	if (mmc_state == SOK) {
		app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
#if defined(NEXXONE) || defined(NEXX360W)
	#if SYS_CONFIG_VOIP
			/* copy app_fitt.out or full update */
			ctrl_auto_update();
			/* app_mcu_pwr_off() 에서 종료 시 1로 만든다. */
			if (app_cfg->ste.b.pwr_off)
				return 0;
	#endif
#endif
		/* remove update files */
		ctrl_reset_nand_update();
	} else {	//# mmc error
		app_leds_mmc_ctrl(LED_MMC_RED_BLINK);
		app_buzz_ctrl(100, 1);
		app_msleep(5000);
		mic_exit_state(OFF_NORMAL, 0);
		app_msleep(100);
		mic_msg_exit();
		return -1;
	}
	//----------  SD 카드 ----------------------------------------------
	app_set_open();
    app_setdns() ;  // set resolv.conf

	//#--- system init
	ret = main_thread_init();
	if (ret < 0) {
		return -1;
	}
	
	aprintf("app_set->ch[%d].resol = %d\n", MODEL_CH_NUM, app_set->ch[MODEL_CH_NUM].resol) ;
	mcfw_linux_init(0, app_set->ch[MODEL_CH_NUM].resol) ; 
	g_mem_init();

	//# start log system
    app_ipins_init();
	
#ifdef __SYSLOGD_ENABLE__
	__syslogd_enable(1);
#else
	app_log_init();
#endif	
	app_gui_init();
	app_dev_init();
    app_tsync_init() ;

    setting_txtbase() ;

	if (app_file_init() == SOK) {
		app_rec_init();
	}
	
#if SYS_CONFIG_GPS	
	app_gps_init();
#endif	
	//#--- app main ----------
	app_main();
	//#-----------------------

	//#--- system de-init
	app_rec_exit();
	app_file_exit();
	
#ifdef __SYSLOGD_ENABLE__
	__syslogd_enable(0);
#else
	app_log_exit();
#endif	
	app_mcu_exit();		//# will power off after 200mS
	app_dev_exit();
	app_gui_exit();
	app_buzz_exit();
	
	if(app_set->srv_info.ON_OFF)
        app_fms_exit();

	if(app_set->ftp_info.ON_OFF)
        app_ftp_exit();

    app_ipins_exit();
#if SYS_CONFIG_VOIP	
	app_voip_exit(); /* voip exit */
#endif
#if SYS_CONFIG_GPS	
	app_gps_exit();
#endif	
	g_mem_exit();
	mcfw_linux_exit();
	main_thread_exit();

//	app_mcu_exit();		//# will power off after 200mS
	
	printf("--- FITT360 end ---\n\n");

	return 0;
}
