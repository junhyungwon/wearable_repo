/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_mcu.c
 * @brief	micom(volt, temp, acc, switch) check thread
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "app_comm.h"
#include "app_main.h"
#include "app_dev.h"
#include "app_cap.h"
#include "app_rec.h"
#include "app_mcu.h"
#include "app_set.h"
#include "app_rtsptx.h"
#include "app_ftp.h"
#include "app_file.h"
#include "app_ctrl.h"
#include "app_gui.h"
#include "app_buzz.h"

#if SYS_CONFIG_VOIP
#include "app_voip.h"
#endif

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_DATA_CYCLE		  100	//# msec, data receive period from micom
#define TIME_CHK_VLOW		10000	//# msec, low power check time
#define TIME_CHK_VLEVEL		 5000	//# msec, battery level check time

#define CNT_CHK_VLOW		(TIME_CHK_VLOW/TIME_DATA_CYCLE)
#define CNT_CHK_VLEVEL		(TIME_CHK_VLEVEL/TIME_DATA_CYCLE)

#define IBATT_MIN		    620 	//590		//#  5.90 V->6.4V, minimum battery voltage
#define EBATT_MIN		    970					//#  9.7 V (3s->1s per 1V)minimum battery voltage
#define MAX_TIME_GAP		3000	//3sec

#define PSW_EVT_LONG		2

typedef struct {
	app_thr_obj cObj;	//# micom thread
	OSA_MutexHndl mutex_3delay;

	dev_val_t val;
} app_mcu_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_mcu_t mcu_obj;
static app_mcu_t *imcu=&mcu_obj;

static void delay_3sec_exit(void)
{
	struct timeval t1, t2;
	int tgap=0;

	gettimeofday(&t1, NULL);
	while (tgap < MAX_TIME_GAP && (imcu->val.ibatt > IBATT_MIN || imcu->val.ebatt > EBATT_MIN)) {
		gettimeofday(&t2, NULL);
		tgap = ((t2.tv_sec*1000)+(t2.tv_usec/1000))-((t1.tv_sec*1000)+(t1.tv_usec/1000));
		if (tgap <= 0) {
			gettimeofday(&t1, NULL);
		}
		OSA_waitMsecs(1);
	}
	gettimeofday(&t2, NULL);

	printf(" [app] msec[%ld] >>>>>>>> DELAY EXIT  <<<<<<< \n",
			((t2.tv_sec*1000)+(t2.tv_usec/1000))-((t1.tv_sec*1000)+(t1.tv_usec/1000)));

	mic_msg_exit();
}

void app_mcu_pwr_off(int type)
{
	if(imcu == NULL)
		return;

	OSA_mutexLock(&imcu->mutex_3delay);

#ifdef SYS_LOG_ENABLE
	system("/etc/init.d/S30logging stop");
#else	
	app_log_exit();
#endif
	mic_exit_state(type, 0);
	app_cfg->ste.b.pwr_off = 1;

	delay_3sec_exit();
	OSA_mutexUnlock(&imcu->mutex_3delay);
}

/*----------------------------------------------------------------------------
 check voltage
-----------------------------------------------------------------------------*/
//# max (8.43V)
//# //# check battery gauge level
//	if(mval.ibatt < 600) 		{bg_lv = 0;}
//	else if(mval.ibatt < 710) 	{bg_lv = 1;}
//	else if(mval.ibatt < 760)	{bg_lv = 2;}
//	else						{bg_lv = 3;}

static int c_volt_chk = 0;
static int c_volt_lv = 0;
static int power_on_lv = 1;

static int mcu_chk_pwr(short mbatt, short ibatt, short ebatt)
{
    char msg[128] = {0, };
	int bg_lv = -1;

	if (power_on_lv) {
		power_on_lv = 0;
		if (ibatt < 731) 		{bg_lv = 0;}
		else if(ibatt < 745) 	{bg_lv = 1;}
		else if(ibatt < 775)	{bg_lv = 2;}
		else					{bg_lv = 3;}

		app_leds_int_batt_ctrl(bg_lv);
	} else {
		if (ibatt >= 776) 					   {bg_lv = 3;}
		else if (ibatt >= 745 && ibatt < 774)  {bg_lv = 2;}
		else if (ibatt >= 731 && ibatt < 743)  {bg_lv = 1;}
		else if (ibatt >= 600 && ibatt < 729)  {bg_lv = 0;}

		if (c_volt_lv) {
			c_volt_lv--;
			if (c_volt_lv == 0) {
				if (bg_lv >= 0) {
					app_leds_int_batt_ctrl(bg_lv);
				}
				//aprintf("ibatt %d(V) ebatt %d(V) bg_lv %d\n", ibatt,  ebatt, bg_lv);
			}
		} else {
			c_volt_lv = CNT_CHK_VLEVEL;
		}
	} //# if (first_bg_lv)

	//# low power check (first internal battery)
	if(ibatt < IBATT_MIN && ebatt < EBATT_MIN) {
		if(c_volt_chk) {
			c_volt_chk--;
			if(c_volt_chk == 0) 
			{
				snprintf(msg, sizeof(msg), "Peek Low Voltage Detected(%d, %d)", ibatt, ebatt);
				eprintf("%s!\n", msg);
                app_log_write(MSG_LOG_SHUTDOWN, msg);
				ctrl_sys_shutdown();
				
				return 1;
			}
		} else {
			c_volt_chk = CNT_CHK_VLOW;
		}
	}
	else {
		c_volt_chk = 0;
	}

	return 0;
}

/*****************************************************************************
* @brief	micom message main function
* @section  [desc] check power switch and acc power
*****************************************************************************/
static void *THR_micom(void *prm)
{
	app_thr_obj *tObj = &imcu->cObj;
	int exit=0, ret=0;
	mic_msg_t msg;
    char log[128] = {0, } ;

	aprintf("enter...\n");
	tObj->active = 1;

	mic_set_watchdog(ENA, TIME_WATCHDOG);
	mic_data_send(1, TIME_DATA_CYCLE);

	while (!exit)
	{
		app_cfg->wd_flags |= WD_MICOM;

		ret = mic_recv_msg((char*)&msg, sizeof(mic_msg_t));
		if(ret < 0) {
			continue;
		}

		switch(msg.cmd)
		{
			case CMD_DEV_VAL:
			{
				dev_val_t *val = (dev_val_t *)msg.data;

				imcu->val.mvolt = val->mvolt;		//# XX.xx V format
				imcu->val.ibatt = val->ibatt;		//# XX.xx V format
				imcu->val.ebatt  = val->ebatt;		//# XX.xx V format

				#if 0
				printf("[%3d] mpwr: %d.%d V, ibatt:%d.%d V, ebatt:%d.%d V\n", msg.param,
					(imcu->val.mvolt/100), (imcu->val.mvolt%100), (imcu->val.ibatt/100), (imcu->val.ibatt%100),
					(imcu->val.ebatt/100), (imcu->val.ebatt%100));
				#endif

				ret = mcu_chk_pwr(val->mvolt, val->ibatt, val->ebatt);
				if(ret) {
					exit = 1;
				}

				break;
			}
			case CMD_PSW_EVT:
			{
				short key_type = msg.data[0];
				dprintf("[evt] pwr switch %s event\n", msg.data[0]==2?"long":"short");
				if (key_type == PSW_EVT_LONG) 
				{
					if (!app_cfg->ste.b.busy) {
						snprintf(log, sizeof(log), "[APP_MICOM] --- Power Switch Pressed. It Will be Shutdown ---");
						app_log_write(MSG_LOG_SHUTDOWN, log);
						dprintf("%s\n", log);
						//# add rupy
						ctrl_sys_shutdown();
						exit = 1;
					} else {
						dprintf("skip power switch event!!!\n");
					}
				} else {
#if defined(NEXXONE) || defined(NEXX360W)
					#if SYS_CONFIG_VOIP 
					if (!app_cfg->ste.b.ftp_run) 
					{     
						if (app_rec_state()) {
							app_rec_stop(1);
						} else {
							app_rec_start();
						}
					}
					#else
					if (!app_cfg->ste.b.ftp_run && app_cfg->ste.b.cap) {
						if (!app_cfg->ste.b.nokey)
				    		change_video_fxn();
					}
					#endif
#elif defined(NEXX360B)
					if (!app_cfg->ste.b.ftp_run && app_cfg->ste.b.cap) {
						if (!app_cfg->ste.b.nokey)
				    		change_video_fxn();
					}
#endif 
				}
				break;
			}
			case CMD_DAT_STOP:		//# response data send stop
			{
				dprintf("[evt] data stop event\n");
				exit = 1;
				break;
			}
			default:
				break;
		}
	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    micom watchdog function
* @section  [desc]
*****************************************************************************/
void app_mcu_wd_keep_alive(void)
{
	mic_keep_alive();
}

/*****************************************************************************
* @brief    micom thread start/stop function
* @section  [desc]
*****************************************************************************/
int app_mcu_start(void)
{
	int status = SOK;
	app_thr_obj *tObj;
	
	//# static config clear
	memset((void *)imcu, 0x0, sizeof(app_mcu_t));

    status = OSA_mutexCreate(&(imcu->mutex_3delay));
	OSA_assert(status == OSA_SOK);
	
	//#--- create mcu thread
	app_cfg->wd_tot |= WD_MICOM;
	tObj = &imcu->cObj;
	if(thread_create(tObj, THR_micom, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}

	return SOK;
}

void app_mcu_stop(void)
{
	int status = SOK;
	app_thr_obj *tObj;

	//#--- stop thread
	tObj = &imcu->cObj;
	//event_send(tObj, UBX_CMD_STOP, 0, 0);		//# need not
	mic_data_send(0, 0);		//# data send stop
	while(tObj->active) {
		app_msleep(20);
	}

    status = OSA_mutexDelete(&(imcu->mutex_3delay));
	OSA_assert(status == OSA_SOK);	
	
	thread_delete(tObj);
}

/*****************************************************************************
* @brief    micom init/exit function
* @section  [desc]
*****************************************************************************/
int app_mcu_init(void)
{
	int ret;

	ret = mic_msg_init();
	if(ret < 0) {
		return -1;
	}
	mic_send_ready();

//	mic_exit_state(OFF_NONE, 0);	//# for test - no power off
	aprintf("... done!\n");
	
	return 0;
}

int app_mcu_exit(void)
{
    mic_msg_exit();
	
	aprintf("... done!\n");
	
	return 0;
}
