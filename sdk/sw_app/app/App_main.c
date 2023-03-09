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
		TRACE_ERR("create thread\n");
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

	TRACE_INFO("\n MC_GetStatus rv = 0x%04x (%s), flag = 0x%08x \n", rv, MC_GetErrorString(rv), flag);

	if (flag & MC_STAT_INITIALIZED) {
		TRACE_INFO(" MC status is normal. (%s) \n", MC_GetErrorString(rv));

	} else if (flag & MC_STAT_FATAL) {
	    TRACE_ERR(" MC status is fatal error ! (%s) \n", MC_GetErrorString(rv));
		exit(0);
	}else { /* (flag & MC_STAT_NONE)*/
		TRACE_INFO(" MC status is not initialized. (%s) \n", MC_GetErrorString(rv));
	} 

	TRACE_INFO("\n");
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
		TRACE_INFO("nameserver %s\n", app_set->net_info.dns_server1);
		
		sprintf(buffer, "nameserver %s\n", app_set->net_info.dns_server2) ;
		fwrite(buffer, strlen(buffer), 1, fp);
		TRACE_INFO("nameserver %s\n", app_set->net_info.dns_server2);
		
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
	if (dev_disk_mmc_check_writable() > 0) { 
		/* mmc prepare 성공 */
		app_cfg->ste.b.mmc = 1;
	} else 
		return -1;
					
	return SOK;
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

	TRACE_INFO("enter...\n");
	tObj->active = 1;

	app_mcu_start();
    ctrl_get_mcu_version(micom_ver) ;
    LOGD("[main] Starting NEXX Application with SW_Ver: %s, HW_Ver: %s, Micom_Ver: %s\n", 
					FITT360_SW_VER, FITT360_HW_VER, micom_ver);

    if(app_set->sslvpn_info.ON_OFF) {
		app_sslvpn_start();
	}

    if(app_set->rtmp.ON_OFF)
	{
		struct sigaction act;
		
    	app_libuv_start();
	    app_rtmp_start();
    	// disable SIGPIPE
		act.sa_handler = SIG_IGN;
		sigemptyset(&act.sa_mask);
		act.sa_flags=0;
		sigaction(SIGPIPE, &act, NULL);
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
/*	
    if(app_set->voip.ON_OFF)
		app_voip_init();
    else
    	app_call_control_init() ;
*/

// for ssh tunneling
//	if(app_set->sys_info.rec_encryption)
//		app_sshd_start() ;

	app_buzz_ctrl(80, 2, 0);	//# buzz: power on

    if(app_set->voip.ON_OFF)
		app_voip_init();
    else
    	app_call_control_init() ;

	app_gui_init();
#ifdef USE_KCMVP
    struct tm ts ;
	time_t tm1 ;
	
	putenv("LD_LIBRARY_PATH=/usr/lib/");
	time(&tm1) ;
	localtime_r(&tm1, &ts) ;

    TRACE_INFO("%d-%d-%d-%d%d%d\n",	ts.tm_year + 1900, ts.tm_mon + 1, ts.tm_mday, ts.tm_hour, ts.tm_min, ts.tm_sec) ;

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

	TRACE_INFO("--------- app_main() EXIT....\n");
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
	int mmc_ste, mcu_ste, led_ste;	
	
	/* micom ready 신호 전달 */
	mcu_ste = app_mcu_init();
	app_leds_init();
	app_buzz_init(); //# buzzer mutex init..	
	app_cfg_init();
	
	//# ------- SD 카드 상태 확인 및 마운트 점검 ----------------------
	if (dev_disk_mmc_check() < 0) {
		fprintf(stderr, "system will be shutdown. because mmc card is removed\n");
		__mmc_error_shutdown();
		return -1;
	}
	
	/* SD 카드 fsck 실행 */
	if (dev_disk_check_mount(MMC_MOUNT_POINT)) {
		system("/bin/umount /mmc"); /* umount */
		app_msleep(100);
	}
	//# execute to repair
	system("/sbin/fsck.fat -a -w /dev/mmcblk0p1");
	//# mount sd card.
	system("/bin/mount -t vfat /dev/mmcblk0p1 /mmc");
	//# To remove recovery file!! FSCK****.REC
	system("/bin/rm -rf /mmc/*.REC");
		
	/* mmc prepare: 쓰기 가능한 지 확인 및 log 디렉토리 생성 */
	mmc_ste = __mmc_prepare();
	/* After MMC mount, log data be written */
	if (mmc_ste == SOK) {
		//# start log system
		system("/etc/init.d/logging.sh start");
		/* wait for syslogd init */
		app_msleep(100);
		
		/* checking mcu state */
		if (mcu_ste < 0)
			LOGE("[main] Unable to connect with micom!\n"); 
		else
			LOGD("[main] Connecting with micom succeed!(IPC ready)\n");	
		/* Buzzer device log */
		LOGD("[main] Initializing Buzzer Device success!\n");
		
		led_ste = app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
		if (led_ste < 0)
			LOGE("[main] Failed to Initialize LED!\n");
		else
			LOGD("[main] Initializing LED Device success!\n");
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
	LOGD("[main] Initializing NAND Flash success!\n");
	
	//----------  SD 카드 ----------------------------------------------
    app_setdns() ;  // set resolv.conf
	//#--- system init
	if (main_thread_init() < 0) {
		LOGE("[main] Failed to init main thread!. system exit!\n");
		return -1;
	}
	
	TRACE_INFO("Starting Initialize Video Processor: HD %d[CH]---\n", MODEL_CH_NUM);
	mcfw_linux_init(0, app_set->ch[MODEL_CH_NUM].resol); 
	g_mem_init();
    app_ipins_init();
//	app_gui_init();
	app_dev_init();
	LOGD("[main] Initializing Input system(KEY) success!\n");
	
    app_tsync_init();
//    setting_txtbase() ;
	if (app_file_init() == SOK) {
		app_rec_init();
	}
	
#if SYS_CONFIG_GPS	
	app_gps_init();
	LOGD("[main] Initializing GPS system success!\n");
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
	
	/* stopping syslogd */
	system("/etc/init.d/logging.sh stop");
	app_msleep(20);
	
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
	app_leds_exit();	
	g_mem_exit();
	mcfw_linux_exit();
	main_thread_exit();
//	app_mcu_exit();		//# will power off after 200mS
	TRACE_INFO(stderr, "--- NEXX end ---\n\n");
	return 0;
}
