/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_dev.c
 * @brief   device control routine
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "dev_gpio.h"
#include "dev_accel.h"
#include "dev_common.h"
#include "dev_disk.h"
#include "dev_gps.h"
#include "dev_micom.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_rec.h"
#include "app_ctrl.h"
#include "app_dev.h"
#include "app_set.h"
#include "app_ftp.h"
#include "app_fms.h"
#include "ti_vcap.h"
#include "app_mcu.h"
#include "app_gui.h"
#include "app_process.h"
#include "app_buzz.h"
#include "app_bcall.h"
#include "app_voip.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define REC_KEY						GPIO_N(0, 6)	//# record switch
#if defined(NEXXB) || defined(NEXXB_ONE)
#define SOS_KEY						GPIO_N(1, 14)	//# sos switch
#endif

#define TIME_DEV_CYCLE				(100)		//# msec

typedef struct {
	app_thr_obj devObj;		//# dev thread

    int old_evt_state;
} app_dev_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_dev_t t_dev;
static app_dev_t *idev=&t_dev;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 gpio init/exit functions
-----------------------------------------------------------------------------*/
static void dev_gpio_init(void)
{
	gpio_input_init(REC_KEY);
#if defined(NEXXB) || defined(NEXXB_ONE)
	gpio_input_init(SOS_KEY);
#endif
}

static void dev_gpio_exit(void)
{
	gpio_exit(REC_KEY);
#if defined(NEXXB) || defined(NEXXB_ONE)	
	gpio_exit(SOS_KEY);
#endif	
}

/*----------------------------------------------------------------------------
 input check functions
-----------------------------------------------------------------------------*/
/*****************************************************************************
* @brief    check time function
* @section  [return] 1: exist file, 0:no file
*****************************************************************************/
static int _chk_time_file(void)
{
	FILE *fp;
	char *file = "/mmc/fitt_time.txt";
	char line[1024], prefix[8]={0,}, date[64]={0,}, time[64]={0,};
	char cmd[128];

	//# time update file check
	if(-1 == access(file, 0)) {
		return 0;
	}

	//# change time
	fp = fopen(file, "r");
	if (fp == NULL) {
		eprintf("cannot open %s\n", file);
		return 0;
	}

	while (fgets(line, sizeof(line), fp)) {
		sscanf(line, "%s %s %s", prefix, date, time);
		if(!strcmp(prefix, "time")) {
			sprintf(cmd, "date -s \"%s %s\"", date, time);
			system(cmd);
			util_sys_exec("hwclock -w");
			break;
		}
	}

	fclose(fp);
	//# delete time file
	sprintf(cmd, "rm -rf %s", file);
	system(cmd);
	sync();
	app_msleep(100);

	return 1;
}

int dev_ste_mmc(void)
{
	int status;

	status = dev_disk_check_mount(MMC_MOUNT_POINT);
	//dprintf("--- value %d\n", status);

	if (status) {
		_chk_time_file();
	}

	return status;
}

int dev_ste_key(int gio)
{
	int status;

	gpio_get_value(gio, &status);
	//dprintf("--- [key] value %d\n", status);

	return status;
}

static int cnt_key=0, ste_key=KEY_NONE;
static int chk_input_key(int gpiokey)
{
	int key = dev_ste_key(gpiokey);

	if(ste_key != KEY_NONE) {
		if(key == 1) {
			ste_key = KEY_NONE;
		}
		return KEY_NONE;
	}

	if(cnt_key)
	{
		if(key == 0) {	//# press
			cnt_key--;
			if(cnt_key == 0) {
				ste_key = KEY_LONG;
			}
		}
		else {
			cnt_key = 0;
			ste_key = KEY_SHORT;
		}
	}
	else {
		if(ste_key == KEY_NONE) {
			if(key == 0) {
				cnt_key = 20;		//# 1sec
			}
		}
	}

	return ste_key;
}

#if defined(NEXXONE)
/*****************************************************************************
* @brief    REC KEY & SD Card Insert/remove Detection thread function (NEXXONE)
* @section  [desc]
*****************************************************************************/
static void *THR_dev(void *prm)
{
    app_thr_obj *tObj = &idev->devObj;
	int exit=0;
	int mmc, cmd, value = 0;
	int rkey, call_state;
	
	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		cmd = tObj->cmd;
		if (cmd == APP_CMD_STOP) {
			break;
		}

		//# check mmc card
		mmc = dev_ste_mmc();
		if (mmc != app_cfg->ste.b.mmc) {
			app_cfg->ste.b.mmc = mmc;
			aprintf("SD Card removed.. system will restart!\n");
			app_rec_stop(OFF);
			app_mcu_pwr_off(OFF_RESET);
		}
		
		/* record key --> call function */
		rkey = chk_input_key(REC_KEY);
		if (rkey == KEY_SHORT) {
			if(app_cfg->voip_set_ON_OFF)
				app_voip_event_noty();
			else
			{
				call_state = get_calling_state() ;		
				switch(call_state)
				{
					case APP_STATE_INCOMING :
						app_accept_call() ; // Back channel signal
						break;
					case APP_STATE_ACCEPT :
					    break;
					case APP_STATE_CALLING :
						app_close_call() ;
						break ;
					case APP_STATE_NONE :
					    app_call_send() ;
						break ;
					case APP_STATE_OUTCOMING :  // call cancel 
						app_close_call() ;
						break ;
				}
			}
		} else if (rkey == KEY_LONG) {	
			if (app_rec_state()) {
				app_rec_stop(ON);
			} else {
				app_rec_start();
			}
		}
		app_msleep(TIME_DEV_CYCLE);
	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

#elif defined(NEXX360W) || defined(NEXX360W_MUX) || defined(NEXX360B) || defined(NEXX360H) || defined(NEXX360C) || defined(NEXX360W_CCTV)
/*****************************************************************************
* @brief    REC KEY & SD Card Insert/remove Detection thread function 
*          (NEXX360W/NEX360W_MUX/NEXX360B/NEXX360H/NEXX360C/NEXX360W_CCTV)
* @section  [desc]
*****************************************************************************/
static void *THR_dev(void *prm)
{
    app_thr_obj *tObj = &idev->devObj;
	int exit=0;
	int mmc, cmd, value = 0;
	int rkey, call_state;
	
	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		cmd = tObj->cmd;
		if (cmd == APP_CMD_STOP) {
			break;
		}

		//# check mmc card
		mmc = dev_ste_mmc();
		if (mmc != app_cfg->ste.b.mmc) {
			app_cfg->ste.b.mmc = mmc;
			aprintf("SD Card removed.. system will restart!\n");
			app_rec_stop(OFF);
			app_mcu_pwr_off(OFF_RESET);
		}
		
		/* record key --> call function */
		rkey = chk_input_key(REC_KEY);
		if (rkey == KEY_SHORT) {
			call_state = get_calling_state() ;		
			switch(call_state)
			{
				case APP_STATE_INCOMING :
					app_accept_call() ; // Back channel signal
					break;
				case APP_STATE_ACCEPT :
				    break;
				case APP_STATE_CALLING :
					app_close_call() ;
					break ;
				case APP_STATE_NONE :
				    app_call_send() ;
					break ;
				case APP_STATE_OUTCOMING :  // call cancel 
					app_close_call() ;
					break ;
			}
				
//			app_voip_event_noty();
		} else if (rkey == KEY_LONG) {	
			if (app_rec_state()) {
				app_rec_stop(ON);
			} else {
				app_rec_start();
			}
		}

		app_msleep(TIME_DEV_CYCLE);
	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

#elif defined(NEXXB) || defined(NEXXB_ONE)
/*****************************************************************************
* @brief    REC KEY & SD Card Insert/remove Detection thread function(NEXXB/NEXXB_ONE) 
* @section  [desc]
*****************************************************************************/
static void *THR_dev(void *prm)
{
    app_thr_obj *tObj = &idev->devObj;
	int exit=0;
	int mmc, cmd, value = 0;
	int rkey, skey;
	
	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		cmd = tObj->cmd;
		if (cmd == APP_CMD_STOP) {
			break;
		}

		//# check mmc card
		mmc = dev_ste_mmc();
		if (mmc != app_cfg->ste.b.mmc) {
			app_cfg->ste.b.mmc = mmc;
			aprintf("SD Card removed.. system will restart!\n");
			app_rec_stop(OFF);
			app_mcu_pwr_off(OFF_RESET);
		}
		
		/* record key --> call function */
		rkey = chk_input_key(REC_KEY);
		//# For button enable, when camera didn't connected 
		if (rkey == KEY_SHORT) {		
			/* Short KEY */
			if (app_cfg->voip_set_ON_OFF)
				app_voip_event_noty();
		} else if (rkey == KEY_LONG) {
			//# Normal Rec ( 영업 요청으로 Event rec에서 normal로 변경) 
			if (!app_cfg->ste.b.ftp_run) {
				if (app_rec_state()) {
					app_rec_stop(ON);
				} else {
					app_rec_start();
				}
			}
		}
		
		skey = chk_input_key(SOS_KEY);
		if (!app_cfg->ste.b.ftp_run) {
			if (skey == KEY_SHORT) {  // SOS REC ON/OFF toggle 
				value = app_rec_state() ;
				/*
				 * return 1-> EVENT, return 2-> SOS
				 */
				if (value == 2) { 
					// value 2, SOS Rec  , Value 1 Normal/Event REc
					app_rec_stop(ON) ;  //  ON --> rollback pre_rec status, OFF ignore pre_rec status
					app_sos_send_stop(ON) ;
					sysprint("[APP_SOS] End SOS Event !! \n");	
				} else {
					//  Rec off or normal/event rec -> SOS ON
					app_rec_stop(ON) ;  //  ON --> rollback pre_rec status, OFF ignore pre_rec status
					app_rec_evt(ON) ;  // SOS REC
					sysprint("[APP_SOS] Occurs SOS Event !! \n");
				}
				dprintf("SOS Short Key Pressed!\n");
			} else if (skey == KEY_LONG) {	
				dprintf("SOS Long Key Pressed!\n");
			}
		}
		app_msleep(TIME_DEV_CYCLE);
	}
	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}
#else
#error "Not defined Build Model!"
#endif

/*****************************************************************************
* @brief    IO device thread start/stop
* @section  [desc]
*****************************************************************************/
int app_dev_init(void)
{
	app_thr_obj *tObj;

	//# static config clear - when Variable declaration
	memset((void *)idev, 0x0, sizeof(app_dev_t));
	dev_gpio_init();

	/* initialize uid area */
	dev_board_uid_init();

	//#--- create thread
    tObj = &idev->devObj;
    if (thread_create(tObj, THR_dev, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
    	eprintf("create dev thread\n");
		return EFAIL;
    }

	aprintf("....done!\n");

	return SOK;
}

void app_dev_exit(void)
{
	app_thr_obj *tObj;

    //#--- dev_stop
    tObj = &idev->devObj;
    event_send(tObj, APP_CMD_STOP, 0, 0);
    while(tObj->active)
    	app_msleep(20);
	thread_delete(tObj);

	/* gpio free */
	dev_gpio_exit();

	aprintf("... done!\n");
}
