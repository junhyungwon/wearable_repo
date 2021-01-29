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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>
#include <linux/rtc.h>
#include <fcntl.h>
#include <unistd.h>

#define __USE_GNU
#include <sys/ioctl.h>
#include <sys/time.h>

#include "dev_common.h"
#include "dev_gpio.h"
#include "dev_gps.h"
#include "dev_buzzer.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_ctrl.h"
#include "gui_main.h"
#include "app_dev.h"
#include "app_mcu.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define LED_BLINK_TEST		  (0)
#define SENSOR_TEST			  (1)
#define USB_TEST			  (1)
#define RTC_TEST			  (1)
#define GPS_TEST			  (1)

#define TIME_DEV_CYCLE		  100		//# msec
#define TIME_GPS_CYCLE		  500		//# msec
#define TIME_LED_CYCLE		  500		//# msec
#define TIME_RTC_CYCLE		  1000		//# msec
#define TIME_ETH_CYCLE		  1000		//# msec

#define CNT_GPS_CHECK		(TIME_GPS_CYCLE/TIME_DEV_CYCLE)
#define CNT_LED_CHECK		(TIME_LED_CYCLE/TIME_DEV_CYCLE)
#define CNT_RTC_CHECK		(TIME_RTC_CYCLE/TIME_DEV_CYCLE)
#define CNT_ETH_CHECK		(TIME_ETH_CYCLE/TIME_DEV_CYCLE)

typedef struct {
	app_thr_obj dObj;	//# dev thread
	app_thr_obj bObj;	//# buzzer thread
	app_thr_obj cObj;	//# cradle ether thread
	
	int init;			//# initialize
	int gps_tmr;
	int led_tmr;
	int rtc_tmr;
	int eth_tmr;
	
	int ether_init;
	
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
	gpio_input_init(BACKUP_DET);
}

static void dev_gpio_exit(void)
{
	gpio_exit(GPS_PWR_EN);
	gpio_exit(BACKUP_DET);
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

int app_ste_ether(void)
{
	int status;
	
	/* 0-> connect 1-> remove */
	gpio_get_value(BACKUP_DET, &status);

	return (status?0:1);
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
* @brief	USB State
* @section  [desc]
*****************************************************************************/
/*
 * Bus 001 Device 001: ID 1d6b:0002
 * Bus 002 Device 001: ID 1d6b:0002
 * Bus 001 Device 002: ID 0ea0:2168
 * Bus 002 Device 002: ID 0bda:8179
 */
int app_get_usb_id(int bus, int *v, int *p)
{
	char cmd[128] = {0,};
	char buf[256] = {0,};
	char *sptr;

	FILE *f;
	int find = 0;
	int busnum = -1;

	snprintf(cmd, sizeof(cmd), "/usr/bin/lsusb");
	f = popen(cmd, "r");
	if (f == NULL) {
		return -1;
	}

	while (fgets(buf, 255, f) != NULL)
	{
		char *vid, *pid;

		/* %*s->discard input */
		memset(cmd, 0, sizeof(cmd));
		sscanf(buf, "%*s%d%*s%*s%*s%*s", &busnum);
		sscanf(buf, "%*s%*s%*s%*s%*s%s", cmd);

		if (cmd != NULL)
		{
			vid = strtok_r(cmd, ":", &sptr);
			pid = strtok_r(NULL, ":", &sptr);

			if (bus == busnum) {
				/* ignore usb controller id */
				if ((strncmp(vid, "1d6b", 4) == 0) &&
					(strncmp(pid, "0002", 4) == 0))
					continue;

				if (vid != NULL && pid != NULL) {
					*v = (int)strtol(vid, NULL, 16);
					*p = (int)strtol(pid, NULL, 16);
					find = 1;
					break;
				}
			}
		}
	}

	pclose(f);

	return find;
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

int app_get_hwtime(struct tm *rd_tm)
{
	int fd = -1, ret;

	fd = open("/dev/rtc0", O_RDONLY);
	if (fd < 0) {
		return -1;
	}
	
	memset(rd_tm, 0, sizeof(*rd_tm));
	ioctl(fd, RTC_RD_TIME, rd_tm);
	close(fd);

	return SOK;
}

/*****************************************************************************
* @brief	device thread function
* @section  [desc] check hotplug
*****************************************************************************/
static void *THR_dev(void *prm)
{
	app_thr_obj *tObj = &idev->dObj;
	struct gps_data_t gps_data;
	gps_nmea_t *gps_nmea;
	
	int cmd, exit=0;
	int ret, val, state;
	int i, on=ON;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	//# gps init
	idev->gps_tmr = 0; idev->led_tmr=0;
	ret = dev_gps_init();
	if (ret < 0) {
		eprintf("gps init\n");
		return NULL;
	}
	
	while (!exit)
	{
		cmd = tObj->cmd;
		if (cmd == APP_CMD_EXIT) {
			break;
		}
		
		#if SENSOR_TEST
		{
			/* MAIN Power (ADC) Test */
			memset(iapp->vbuf, 0, sizeof(iapp->vbuf));
			val = app_get_volt();
			//# min 1600mV
			if (val > 1600) iapp->ste.b.pwr = 1;
			else 			iapp->ste.b.pwr = 0;
			
			sprintf(iapp->vbuf, "%02d.%02d V", (val/100), (val%100));
			//dprintf("current input voltage %s\n", iapp->vbuf);
		}
		#endif
		
		#if USB_TEST
		{
			int vid=0;
			int pid=0;
			
			ret = app_get_usb_id(1, &vid, &pid);
			if (ret == 1) {
				//dprintf("Detected usb (%x, %x)\n", vid, pid);
				iapp->ste.b.usb = 1;
			} else {
				iapp->ste.b.usb = 0;
			}
		}
		#endif
		
		#if LED_BLINK_TEST
		{
			if (idev->led_tmr >= CNT_LED_CHECK) {
				idev->led_tmr=0;
				for (i = 0; i < LED_IDX_ALL; i++) {
					ret |= ctrl_leds(i, on);
				}
				on = 1 - on;
				/* 한 개라도 fail되면 에러 처리 */
				if (!ret) 	iapp->ste.b.led = 1;
				else		iapp->ste.b.led = 0;
			} else 
				idev->led_tmr++;
		}
		#endif
		
		#if RTC_TEST
		{
			struct tm hw_tm;
			
			if (idev->rtc_tmr >= CNT_RTC_CHECK)
			{ 
				idev->rtc_tmr=0;
				ret = app_get_hwtime(&hw_tm);
				if (!ret) {
					iapp->ste.b.rtc=1;
					#if 0
					dprintf("Current RTC date/time is %02d-%02d-%d, %02d:%02d:%02d.\n", hw_tm.tm_mday, hw_tm.tm_mon + 1, 
								hw_tm.tm_year + 1900, hw_tm.tm_hour, hw_tm.tm_min, hw_tm.tm_sec);
					#endif			 
				} else {
					iapp->ste.b.rtc=0;
				} 
			} else {
				idev->rtc_tmr++;
			}
		}
		#endif
		
		#if GPS_TEST
		/* get GPS Jack state */
		if (!gpio_get_value(GPS_PWR_EN, &val)) {
			//dprintf("GPS Jack GET value %d\n", val);
			if (val == 1) {
				iapp->ste.b.gps_jack = 1;
			} else {
				iapp->ste.b.gps_jack = 0;
			}
		} else {
			eprintf("Failed to read GPS Jack Detect\n");
		}
		
		if (idev->gps_tmr >= CNT_GPS_CHECK)
		{
			idev->gps_tmr = 0;
			if(iapp->ste.b.gps_jack)
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
						#if 0
						if (gps_data.set & G_TIME_SET) {
							app_sys_time(&gps_nmea->date);
							time_set = 0;
						}
						#endif
						iapp->ste.b.gps = 1;
					}
				}
				else	//# disconnect
				{
					iapp->ste.b.gps = 0;
				}
			}
		}
		/* if (idev->gps_tmr >= CNT_GPS_CHECK) */ 
		else {
			idev->gps_tmr++;
		} 
		#endif
		
		OSA_waitMsecs(TIME_DEV_CYCLE);
	} /* while (!exit) */
	
	//# device close
	dev_gps_close();
	
	iapp->ste.b.gps = 0;
	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    network polling function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_ether_poll(void *prm)
{
	app_thr_obj *tObj = &idev->cObj;
	int exit = 0, cmd;
	int ret;
	
	/* for buzzer alert */
	ret = ctrl_ether_cfg_read(ETHER_CFG_NAME, inetapp->ip, inetapp->mask, inetapp->gw); 
	if (ret < 0)
		/* TODO error alert */
		iapp->ste.b.eth = 0;
	else 
		iapp->ste.b.eth = 1;
		
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = tObj->cmd;
        if (cmd == APP_CMD_EXIT) {
		    break;
		}
		
		//# wait cradle Device
		if (iapp->ste.b.eth)
		{
			ret = app_ste_ether();
			if (ret) {
				/* set attach event */
				if (!idev->ether_init) {
					idev->ether_init = 1;
					/* eth enable */
					ctrl_ether_linkup(inetapp->ip, inetapp->mask, inetapp->gw);
				}
			} else {
				/* set remove event */
				if (idev->ether_init) {
					idev->ether_init = 0;
					/* eth disable */
					ctrl_ether_linkdown();
				}
			}
		} 
		else 
		{
			if (idev->eth_tmr >= CNT_ETH_CHECK) {
				idev->eth_tmr = 0;
				app_buzzer(200, 1);
			} else {
				idev->eth_tmr++;
			} 
		}
		
		// for next event : wait
		OSA_waitMsecs(TIME_DEV_CYCLE);
	} 
	
	tObj->active = 0;
	
	return NULL;
}

/*****************************************************************************
* @brief    device thread start/stop function
* @section  [desc]
*****************************************************************************/
int app_dev_start(void)
{
	app_thr_obj *tObj;
	int i, ret=0;
	
	/* default led on */
	for (i = 0; i < LED_IDX_ALL; i++) {
		ret |= ctrl_leds(i, 1);
	}
	/* 한 개라도 fail되면 에러 처리 */
	if (!ret) 	iapp->ste.b.led = 1;
	else		iapp->ste.b.led = 0;
				
	//#--- create dev thread
	tObj = &idev->dObj;
	if(thread_create(tObj, THR_dev, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}

	//#--- create buzzer thread
	tObj = &idev->bObj;
	if(thread_create(tObj, THR_buzzer, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	//#--- create cradle ether thread
	tObj = &idev->cObj;
	if(thread_create(tObj, THR_ether_poll, APP_THREAD_PRI, NULL) < 0) {
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

	//#--- stop dev thread
	tObj = &idev->dObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		OSA_waitMsecs(10);
	}
	thread_delete(tObj);
	
	/* delete ether poll object */
    tObj = &idev->cObj;
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		OSA_waitMsecs(10);

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
