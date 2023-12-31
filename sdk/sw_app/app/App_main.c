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
#include "app_snd_capt.h"
#include "app_snd_play.h"
#include "app_fms.h"
#include "app_gps.h"
#include "app_netmgr.h"
#include "app_ipinstall.h"
#include "app_buzz.h"
#include "app_libuv.h"
#include "app_watchdog.h"
#include "app_rtmp.h"
#include "app_uds.h"

#include "app_voip.h"
#include "app_bcall.h"
#include "app_sslvpn.h"
#include "dev_common.h"

#ifdef USE_KCMVP
#include "mcapi.h"
#include "mcapi_type.h"
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

#ifdef USE_KCMVP

#define MC_STAT_NONE				0x00000000
#define MC_STAT_INITIALIZED			0x10000000
#define MC_STAT_FATAL				0x20000000

void getStatus()
{
	MC_RV rv = 0;
	MC_UINT flag = 0;
	rv = MC_GetStatus(&flag);

	printf("\n MC_GetStatus rv = 0x%04x (%s), flag = 0x%08x \n", rv, MC_GetErrorString(rv), flag);

	if (flag & MC_STAT_INITIALIZED) {
		printf(" MC status is normal. (%s) \n", MC_GetErrorString(rv));

	} else if (flag & MC_STAT_FATAL) {
	    printf(" MC status is fatal error ! (%s) \n", MC_GetErrorString(rv));
		exit(0);
	}else { /* (flag & MC_STAT_NONE)*/
		printf(" MC status is not initialized. (%s) \n", MC_GetErrorString(rv));
	} 

	printf("\n");
}

#endif 

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
		sysprint(buffer);
		
		sprintf(buffer, "nameserver %s\n", app_set->net_info.dns_server2) ;
		fwrite(buffer, strlen(buffer), 1, fp);
		sysprint(buffer);
		
		sprintf(buffer, "options timeout:1 retry:1\n") ;
		fwrite(buffer, strlen(buffer), 1, fp);

		fclose(fp);
	}
}

static int __mmc_prepare(void)
{
	DIR *logdir=NULL;
	
	/* mmc mount 확인 */
	app_cfg->ste.b.mmc = 0; //# default 0
	if (ctrl_mmc_check_writable() > 0) { 
		/* log 디렉토리 생성 */
		logdir = opendir("/mmc/log") ;
		if (logdir == NULL) {
			mkdir("/mmc/log", 0775);
		} else 
			closedir(logdir);
		sync();		
		
		/* mmc prepare 성공 */
		app_cfg->ste.b.mmc = 1;
	} else 
		return -1;
					
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
    char micom_ver[128] = {0, } ;
	char sw_ver[128] = {0, } ;

	app_thr_obj *tObj = &app_cfg->mObj;
	#if !USE_CONSOLE_MENU
	char cmd, exit=0;
	#endif

	aprintf("enter...\n");
	tObj->active = 1;

	app_mcu_start();
    sysprint("[APP_MAIN]>>>>> %s_SYSTEM STARTED!! <<<<<\n", FITT360_SW_VER);

    ctrl_get_mcu_version( micom_ver ) ;
    sysprint("[APP_MAIN]SW_Ver: %s, HW_Ver: %s, Micom_Ver: %s\n", 
					FITT360_SW_VER, FITT360_HW_VER, micom_ver);

    if(app_set->sslvpn_info.ON_OFF)
	{
		app_sslvpn_start() ;
	}

    if(app_set->rtmp.ON_OFF)
	{
    	app_libuv_start();
	    app_rtmp_start();

    // disable SIGPIPE
	sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);

    // enable in default
	    app_rtmp_enable();
	}

    app_cap_start();
	app_snd_capt_init();
	
#if SYS_CONFIG_SND_OUT	
	app_snd_bcplay_init();
	app_snd_iplay_init();
#endif	
	app_netmgr_init();
	
    if (!app_set->sys_info.osd_set)
        ctrl_swosd_enable(STE_DTIME, 0, 0) ;  // osd disable 
	
	//# swosd version set.
	snprintf(sw_ver, sizeof(sw_ver), "%s", FITT360_SW_VER);
	ctrl_swosd_userstr(sw_ver, 1);
	
	if (app_set->srv_info.ON_OFF)
        app_fms_init() ;

    if(!app_set->rtmp.ON_OFF)
		app_rtsptx_start();

	//if(app_set->net_info.enable_onvif==1)
    if(!app_set->rtmp.ON_OFF)
	{
		if (app_onvifserver_start() == 0) {
			app_cfg->ste.b.onvifserver = 1; // have to add this flag....
		}
	}
	app_uds_start();
	app_web_boot_passwordfile();
	if (app_web_start_server() ==0) {
        app_cfg->ste.b.web_server = 1; // have to add this flag....
	}

    CSock_init() ;

	if (app_set->ftp_info.ON_OFF)
        app_ftp_init();
	
    if (app_set->sys_info.P2P_ON_OFF == ON && !app_set->rtmp.ON_OFF) 
	{
		#if SYS_CONFIG_WLAN
        add_p2p_account() ;
//        app_p2p_start() ;  // p2p_init 과 동시 동작시 p2p server 2개 동시 동작되는 경우 발생 
        app_p2p_init() ;
		#endif		
    }
    if(app_set->voip.ON_OFF)
		app_voip_init();
    else
    	app_call_control_init() ;

	app_buzz_ctrl(80, 2, 0);	//# buzz: power on

#ifdef USE_KCMVP
    struct tm ts ;
	time_t tm1 ;
	
	putenv("LD_LIBRARY_PATH=/usr/lib/");
	time(&tm1) ;
	localtime_r(&tm1, &ts) ;

    printf("%d-%d-%d-%d%d%d\n",	ts.tm_year + 1900, ts.tm_mon + 1, ts.tm_mday, ts.tm_hour, ts.tm_min, ts.tm_sec) ;

    MC_RV rv = MC_OK ;
    MC_VERSION mcVer;
    MC_HSESSION hSession = 0 ;

    rv = MC_Initialize(NULL) ;
	if(rv != MC_OK) 
		printf("%s\n", MC_GetErrorString(rv));

    MC_GetVersion((MC_VERSION*)&mcVer);
	printf("===============================================================================\n");
	printf("          Dreamsecurity %s API Ver.%d.%d.%d Tester \n", mcVer.name, mcVer.major, mcVer.minor, mcVer.release);
	printf("===============================================================================\n\n");

	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) 
		printf("%s\n", MC_GetErrorString(rv));

    rv = MC_Selftest(); printf("\n");	
	if (rv !=0) {
		printf("%s\n", MC_GetErrorString(rv));
	}
		printf("%s\n", MC_GetErrorString(rv));

	getStatus();
#endif

	while(!exit)
	{
		//# wait cmd
		cmd = event_wait(tObj);
		if(cmd == APP_CMD_EXIT) {
			break;
		}
	}
    CSock_exit() ;

#ifdef USE_KCMVP
	rv = MC_Finalize();
	if(rv != MC_OK) printf("%s\n", MC_GetErrorString(rv));

	getStatus();
#endif

	//if(app_set->net_info.enable_onvif==1)
    if(!app_set->rtmp.ON_OFF)
	{
		if (app_cfg->ste.b.onvifserver) // have to add this flag....
		{
			app_onvifserver_stop();
			app_cfg->ste.b.onvifserver = 0;
		}
	}
    
    if(app_set->sys_info.P2P_ON_OFF == ON && !app_set->rtmp.ON_OFF)
    {
		#if SYS_CONFIG_WLAN
        app_p2p_exit() ;
        app_p2p_stop() ;
		#endif
    }
    app_rec_stop(OFF);
    app_snd_capt_exit();

#if SYS_CONFIG_SND_OUT	
	app_snd_iplay_exit();
	app_snd_bcplay_exit();
#endif	
    app_cap_stop();

    if(app_set->rtmp.ON_OFF)
	{
		app_rtmp_stop();
    	app_libuv_stop();
	}

    if (app_cfg->ste.b.rtsptx) {
        app_rtsptx_stop();
    }

    if(app_set->sslvpn_info.ON_OFF)
		app_sslvpn_stop() ;

	app_tsync_exit() ;
	app_netmgr_exit();
	app_mcu_stop();
	tObj->active = 0;

	app_leds_init(); /* set leds off state */

	sysprint("[APP_FITT360] app_main() EXIT....\n");

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

static void __mmc_error_shutdown(void)
{
	app_leds_mmc_ctrl(LED_MMC_RED_BLINK);
	app_buzz_ctrl(80, 2, 1); //force alarm
	app_msleep(5000);
	mic_exit_state(OFF_NORMAL, 0);
	app_msleep(100);
	mic_msg_exit();
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
	if(ret == -1)
	{
		__mmc_error_shutdown();
	}
	if (ret == 1) {
		ctrl_mmc_exFAT_format(part_size);
	}

	
	/* SD 카드 fsck 실행 */
	system("/bin/umount /mmc"); /* umount */
	app_msleep(100);
	//# execute to repair
	system("/sbin/fsck.fat -a -w /dev/mmcblk0p1");
	//# mount sd card.
	system("/bin/mount -t vfat /dev/mmcblk0p1 /mmc");
	//# To remove recovery file!! FSCK****.REC
	system("/bin/rm -rf /mmc/*.REC");
		
	/* mmc prepare: 쓰기 가능한 지 확인 및 log 디렉토리 생성 */
	mmc_state = __mmc_prepare();
	if (mmc_state == SOK) {
		app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
		/* copy app_fitt.out or full update */
		app_set_open();
		ctrl_firmware_update();
		/* app_mcu_pwr_off() 에서 종료 시 1로 만든다. */
		if (app_cfg->ste.b.pwr_off)
			return 0;
		/* remove update files */
		ctrl_reset_nand_update();
	} else {
		__mmc_error_shutdown();	
		return -1;
	}
	
	//----------  SD 카드 ----------------------------------------------
    app_setdns() ;  // set resolv.conf
	//#--- system init
	ret = main_thread_init();
	if (ret < 0) {
		return -1;
	}
	
	dprintf("app_set->ch[%d].resol = %d\n", MODEL_CH_NUM, app_set->ch[MODEL_CH_NUM].resol) ;
	mcfw_linux_init(0, app_set->ch[MODEL_CH_NUM].resol) ; 
	g_mem_init();

	//# start log system
    app_ipins_init();
	__syslogd_enable(1);
	
	app_gui_init();
	app_dev_init();
    app_tsync_init() ;

//    setting_txtbase() ;

	if (app_file_init() == SOK) {
		app_rec_init();
	}
	
#if SYS_CONFIG_GPS	
	app_gps_init();
#endif
	/* watchdog alive */
	app_watchdog_init();
		
	//#--- app main ----------
	app_main();
	//#-----------------------

	//#--- system de-init
	app_watchdog_exit();
	app_rec_exit();
	app_file_exit();
	
	__syslogd_enable(0);
	app_mcu_exit();		//# will power off after 200mS
	app_dev_exit();
	app_gui_exit();
	app_buzz_exit();
	
	if(app_set->srv_info.ON_OFF)
        app_fms_exit();

	if(app_set->ftp_info.ON_OFF)
        app_ftp_exit();

    app_ipins_exit();
	if(app_set->voip.ON_OFF)
		app_voip_exit(); /* voip exit */
	else
    	app_call_control_exit() ;

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
