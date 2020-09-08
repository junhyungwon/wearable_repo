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
#include "dev_buzzer.h"
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
#include "app_voip.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_DEV_CYCLE		100		//# msec
#define DEV_CYCLE_TIME      500     //# temp

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
}

static void dev_gpio_exit(void)
{
	gpio_exit(REC_KEY);
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

/*****************************************************************************
* @brief    buzzer control
* @section  [param] time: ms
*****************************************************************************/
void dev_buzz_ctrl(int time, int cnt)
{
	while (1)
	{
		dev_buzzer_enable(1);
		app_msleep(time);
		dev_buzzer_enable(0);
		cnt--;
		if(cnt == 0) {
			break;
		}
		app_msleep(time);
	}
}

static int cnt_key=0, ste_key=KEY_NONE;
static int chk_rec_key(void)
{
	int key = dev_ste_key(REC_KEY);

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

/*****************************************************************************
* @brief    dev thread function
* @section  [desc]
*****************************************************************************/
static void *THR_dev(void *prm)
{
    app_thr_obj *tObj = &idev->devObj;
	int ret, exit=0;
	int mmc, rkey, cmd;

	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		cmd = tObj->cmd;
		if (cmd == APP_CMD_STOP || app_cfg->ste.b.pwr_off) {
			break;
		}

		//# check mmc card
		mmc = dev_ste_mmc();
		if (mmc != app_cfg->ste.b.mmc) {
			app_cfg->ste.b.mmc = mmc;
			//dprintf("SD Card %s\n", mmc?"insert":"remove");
			aprintf("done! will restart\n");
			app_rec_stop(0);
			mcu_pwr_off(OFF_RESET);
		}
		
		if (!app_cfg->ste.b.ftp_run)
		{	
			//# For button enable, when camera didn't connected 
			rkey = chk_rec_key();
			if (rkey == KEY_SHORT) {		
				/* Short KEY */
				app_voip_event_noty();
			} else if (rkey == KEY_LONG) {	//# sw update
				dev_buzz_ctrl(50, 3);		//# buzz: update
				//# 업데이트 파일명이 비정상적인 경우를 제외하고는 
				//# 무조건 Reboot를 하기 위해서 위치를 이곳으로 변경함. 
				if (ctrl_sw_update(SD_MOUNT_PATH) == 0) {
					mcu_pwr_off(OFF_RESET);
				}
			}
		}	
		app_msleep(TIME_DEV_CYCLE);
	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    IO device thread start/stop
* @section  [desc]
*****************************************************************************/
int app_dev_init(void)
{
	app_thr_obj *tObj;
	int status;

	//# static config clear - when Variable declaration
	memset((void *)idev, 0x0, sizeof(app_dev_t));
	dev_gpio_init();

	//#--- create thread
    tObj = &idev->devObj;
    if (thread_create(tObj, THR_dev, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create dev thread\n");
		return EFAIL;
    }

	aprintf("... done!\n");

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
