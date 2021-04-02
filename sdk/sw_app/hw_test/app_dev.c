/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_dev.c
 * @brief	app device functions
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <time.h>
#include <fcntl.h>
#include <termios.h>

#include "dev_common.h"
#include "dev_gpio.h"
#include "dev_gps.h"
#include "dev_accel.h"
#include "dev_buzzer.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_ctrl.h"
#include "app_key.h"
#include "gui_main.h"
#include "app_dev.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_DEV_CYCLE		  100		//# msec
#define TIME_GPS_CYCLE		  500		//# msec
#define TIME_GSN_CYCLE		  250		//# msec
#define TIME_GSN_SKIP		 1000		//# msec
#define TIME_DRAW_DATA		  500		//# msec

#define CNT_GSN_SKIP		(TIME_GSN_SKIP/TIME_GSN_CYCLE)

typedef struct {
	app_thr_obj dObj;	//# dev thread
	app_thr_obj gObj;	//# gps thread
	app_thr_obj bObj;	//# buzzer thread

	int init;			//# initialize
	int gsn;			//# g-sensor state
} app_dev_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_dev_t dev_obj;
static app_dev_t *idev=&dev_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 gpio init/exit function
-----------------------------------------------------------------------------*/
static void dev_gpio_init(void)
{
	gpio_input_init(GPS_PWR_EN);
}

static void dev_gpio_exit(void)
{
	gpio_exit(GPS_PWR_EN);
}

/*****************************************************************************
* @brief    mmc status
* @section  [desc]
*****************************************************************************/
int app_ste_mmc(void)
{
	int status;

	status = util_check_mount("/dev/mmcblk0");
	//dprintf("--- value %d\n", status);

	return status;
}

int app_wait_mmc_remove(void)
{
	int state=0, cnt;

	cnt = (20*1000)/TIME_DEV_CYCLE;		//# wait 20sec.
	while(cnt--)
	{
		state = app_ste_mmc();
		if(!state) {
			break;
		}
		OSA_waitMsecs(TIME_DEV_CYCLE);
	}

	return state;
}

/*****************************************************************************
* @brief	buzzer thread function
* @section  [desc]
*****************************************************************************/
static void *THR_buzzer(void *prm)
{
	app_thr_obj *tObj = &idev->bObj;
	int cmd, exit=0;
	int i, time, cnt;

	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		cmd = event_wait(tObj);
		if(cmd == APP_CMD_EXIT) {
			break;
		}
		time = tObj->param0;
		cnt = tObj->param1;

		for(i=0; i<cnt; i++)
		{
			dev_buzzer_enable(1);
			OSA_waitMsecs(time);
			dev_buzzer_enable(0);

			if(cnt > 1) {
				app_msleep(time);
			}
		}
	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief	buzzer ctrl function
* @section  [desc]
*****************************************************************************/
int app_buzzer(int ms, int cnt)
{
	//#--- create buzzer thread
	event_send(&idev->bObj, APP_CMD_NOTY, ms, cnt);

	return 0;
}

/*****************************************************************************
* @brief	get gsn data function
* @section  [desc]
*****************************************************************************/
static accel_data_t acc={0,};
int app_get_gsn(int *x, int *y, int *z)
{
	*x = acc.x;
	*y = acc.y;
	*z = acc.z;

	return idev->gsn;
}

/*****************************************************************************
* @brief	system time sync function
* @section  [desc]
*****************************************************************************/
int app_sys_time(struct tm *ts)
{
	time_t now, set;

    //# get current time
	now = time(NULL);

	//# get set time
	set = mktime(ts) + (TIME_ZONE*3600);

	//# if difference 1min.
	//if( abs(now-set) > 60 )
	{
		stime(&set);
		util_sys_exec("hwclock -w");
		sleep(1);
		util_sys_exec("hwclock -w");	//# more once
		sleep(1);
		aprintf("--- changed time from GPS ---\n");
	}

	return SOK;
}

/*****************************************************************************
* @brief	gps thread function
* @section  [desc]
*****************************************************************************/
static void *THR_gps(void *prm)
{
	app_thr_obj *tObj = &idev->gObj;
	int cmd, exit=0, ret;
	int state;
	struct gps_data_t gps_data;
	gps_nmea_t *gps_nmea;
	int time_set=1;

	//# gps init
	ret = dev_gps_init();
	if (ret < 0) {
		eprintf("gps init\n");
		return NULL;
	}

	aprintf("enter...\n");
	tObj->active = 1;
	while(!exit)
	{
		cmd = tObj->cmd;
		if(cmd == APP_CMD_EXIT) {
			break;
		}

		if(iapp->ste.b.cap)
		{
			//# get gps value
			state = dev_gps_get_data(&gps_data);
			if (state == GPS_ONLINE)
			{
				//# check status mask
				if (gps_data.rmc_status == STATUS_FIX)
				{
					gps_nmea = &gps_data.nmea;
					#if 0
					dprintf("GPS - DATE %04d-%02d-%02d, UTC %02d:%02d:%02d, speed=%.2f, (LAT:%.2f, LOT:%.2f) \n",
						gps_nmea->date.tm_year+1900, gps_nmea->date.tm_mon+1, gps_nmea->date.tm_mday,
						gps_nmea->date.tm_hour, gps_nmea->date.tm_min, gps_nmea->date.tm_sec,
						gps_nmea->speed, gps_nmea->latitude, gps_nmea->longitude
					);
					#endif

					//# time sync
					if(time_set)
					{
						if (gps_data.set & G_TIME_SET) {
							app_sys_time(&gps_nmea->date);
							time_set = 0;
						}
					}
					iapp->ste.b.gps = 1;
				}
			}
			else	//# disconnect
			{
				iapp->ste.b.gps = 0;
			}
		}
		OSA_waitMsecs(TIME_GPS_CYCLE);
	}

	iapp->ste.b.gps = 0;

	//# device close
	dev_gps_close();

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief	device thread function
* @section  [desc] check hotplug
*****************************************************************************/
static void *THR_dev(void *prm)
{
	app_thr_obj *tObj = &idev->dObj;
	int cmd, exit=0;

	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		cmd = tObj->cmd;
		if(cmd == APP_CMD_EXIT) {
			break;
		}

		OSA_waitMsecs(TIME_DEV_CYCLE);
	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    gps thread start/stop function
* @section  [desc]
*****************************************************************************/
int app_gps_ctrl(int en)
{
	app_thr_obj *tObj;

	if(en) {
		tObj = &idev->gObj;
		if(thread_create(tObj, THR_gps, APP_THREAD_PRI, NULL, NULL) < 0) {
			eprintf("create thread\n");
			return EFAIL;
		}
	}
	else {
		//#--- stop gps thread
		tObj = &idev->gObj;
		event_send(tObj, APP_CMD_EXIT, 0, 0);
		while(tObj->active) {
			OSA_waitMsecs(10);
		}
		thread_delete(tObj);
	}

	return SOK;
}

/*****************************************************************************
* @brief    device thread start/stop function
* @section  [desc]
*****************************************************************************/
int app_dev_start(void)
{
	app_thr_obj *tObj;

	//#--- create dev thread
	tObj = &idev->dObj;
	if(thread_create(tObj, THR_dev, APP_THREAD_PRI, NULL, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}

	//#--- create gps thread
	app_gps_ctrl(ENA);

	//#--- create buzzer thread
	tObj = &idev->bObj;
	if(thread_create(tObj, THR_buzzer, APP_THREAD_PRI, NULL, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}

	return SOK;
}

void app_dev_stop(void)
{
	app_thr_obj *tObj;

	//#--- stop buzzer thread
	tObj = &idev->bObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		OSA_waitMsecs(10);
	}
	thread_delete(tObj);

	//#--- stop gps thread
	app_gps_ctrl(DIS);

	//#--- stop dev thread
	tObj = &idev->dObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		OSA_waitMsecs(10);
	}
	thread_delete(tObj);
}

/*****************************************************************************
* @brief    device thread start/stop function
* @section  [desc]
*****************************************************************************/
int app_dev_init(void)
{
	//# static config clear
	memset((void *)idev, 0x0, sizeof(app_dev_t));

	dev_gpio_init();

	idev->init = 1;

	return SOK;
}

void app_dev_exit(void)
{
	idev->init = 0;

	dev_gpio_exit();
}
