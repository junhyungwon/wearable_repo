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
#define MAX_TIME_GAP		 	3000	//# 3sec
#define TIME_DATA_CYCLE		  	100	//# msec, data receive period from micom
#define TIME_CHK_VLOW			10000	//# msec, low power check time
#define TIME_CHK_VLEVEL		 	5000	//# msec, battery level check time

#define CNT_CHK_VLOW			(TIME_CHK_VLOW/TIME_DATA_CYCLE)
#define CNT_CHK_VLEVEL			(TIME_CHK_VLEVEL/TIME_DATA_CYCLE)

#define PSW_EVT_LONG			2

/* 
 * NEXXB and NEXX Common Voltage 
 */
/* 어탭터 사용 시 -> 16V 이상
 * 내장 배터리 사용 시 -> 내장 배터리 전압이 측정됨.
 * 외장 배터리 사용 -> 외장 배터리 전압 측정됨.
 */
#define MBATT_MIN					600
#define IBATT_MIN		    		620 //590 //#  5.90 V->6.4V, minimum battery voltage

#ifdef NEXXB
#define EBATT_MIN		    		880	//#  8.80 V (외장 트레블 어탭터 사용 시)

#define BOOTUP_BATT_THRES_0			900
#define BOOTUP_BATT_THRES_1			905
#define BOOTUP_BATT_THRES_2			915

#define RUNTIME_BATT_THRES_3_MAX	915
#define RUNTIME_BATT_THRES_2_MAX	914
#define RUNTIME_BATT_THRES_2_MIN	910
#define RUNTIME_BATT_THRES_1_MAX	908
#define RUNTIME_BATT_THRES_1_MIN	900
#define RUNTIME_BATT_THRES_0_MAX	890
#define RUNTIME_BATT_THRES_0_MIN	880
#else
#define EBATT_MIN		    		970	//#  9.7 V (3s->1s per 1V) minimum battery voltage

#define BOOTUP_BATT_THRES_0			731
#define BOOTUP_BATT_THRES_1			745
#define BOOTUP_BATT_THRES_2			775

#define RUNTIME_BATT_THRES_3_MAX	776
#define RUNTIME_BATT_THRES_2_MAX	774
#define RUNTIME_BATT_THRES_2_MIN	745
#define RUNTIME_BATT_THRES_1_MAX	743
#define RUNTIME_BATT_THRES_1_MIN	731
#define RUNTIME_BATT_THRES_0_MAX	729
#define RUNTIME_BATT_THRES_0_MIN	600
#endif

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
			/* timesync 에 의해서 gettimeofday 값이 변경되면 무한 루프에 빠진다 */
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
	system("/etc/init.d/logging.sh stop");
	mic_exit_state(type, 0);
	app_cfg->ste.b.pwr_off = 1;
	delay_3sec_exit();
	OSA_mutexUnlock(&imcu->mutex_3delay);
}

/*----------------------------------------------------------------------------
 check voltage
-----------------------------------------------------------------------------*/
static int c_volt_chk = 0;
static int c_volt_lv = 0;
static int power_on_lv = 1;

/*
 * Case NEXX_B
 * remove internal batt. external batt support trable adapter. so min 8.75V.
 *
 * Others
 * LF External Batt full charge level is 11.7V
 * mbatt는 전원에 따라서 다르게 측정됨. 어탭터는 16V, 외장은 9V 또는 11V 내장은 8V
 * 내장 max (8.43V), 외장 배터리 최소 8.75V 이상. 
 */
static int mcu_chk_pwr(short mbatt, short ibatt, short ebatt)
{
	int bg_lv = -1;
	int threshold=0;
	
#ifdef NEXXB
	threshold = ebatt;
#else
	threshold = ibatt;
#endif	
	
	if (power_on_lv) {
		power_on_lv = 0;
		if (threshold < BOOTUP_BATT_THRES_0) {
			bg_lv = 0;
		} else if (threshold < BOOTUP_BATT_THRES_1) 	{
			bg_lv = 1;
		} else if (threshold < BOOTUP_BATT_THRES_2)	{
			bg_lv = 2;
		} else	{
			bg_lv = 3;
		}
		
		app_leds_int_batt_ctrl(bg_lv);
	} else {
		if (threshold >= RUNTIME_BATT_THRES_3_MAX) { 
			bg_lv = 3;
		} else if (threshold >= RUNTIME_BATT_THRES_2_MIN && threshold < RUNTIME_BATT_THRES_2_MAX) {
			bg_lv = 2;
		} else if (threshold >= RUNTIME_BATT_THRES_1_MIN && threshold < RUNTIME_BATT_THRES_1_MAX) {
			bg_lv = 1;
		} else if (threshold >= RUNTIME_BATT_THRES_0_MIN && threshold < RUNTIME_BATT_THRES_0_MAX) {
			bg_lv = 0;
		}

		if (c_volt_lv) {
			c_volt_lv--;
			if (c_volt_lv == 0) {
				if (bg_lv >= 0) {
					app_leds_int_batt_ctrl(bg_lv);
				}
#if 0
#ifdef NEXXB				
				dprintf("ibatt %d(V): ebatt %d(V): mbatt %d(V): gauge %d(via Ext)\n", ibatt, ebatt, mbatt, bg_lv);
#else
				dprintf("ibatt %d(V): ebatt %d(V): mbatt %d(V): gauge %d(via Int)\n", ibatt, ebatt, mbatt, bg_lv);
#endif
#endif				
			}
		} else {
			c_volt_lv = CNT_CHK_VLEVEL;
		}
	} //# if (first_bg_lv)

	//# low power check
	if ((ibatt < IBATT_MIN) && (ebatt < EBATT_MIN) && (mbatt < MBATT_MIN)) {
		if (c_volt_chk) {
			c_volt_chk--;
			if (c_volt_chk == 0) {
				sysprint("Peek Low Voltage Detected(thres=%d, m=%d)", threshold, mbatt);
				ctrl_sys_halt(1); /* for shutdown */
				return -1;
			}
		} else {
			c_volt_chk = CNT_CHK_VLOW;
		}
	} else {
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
				if(ret < 0) {
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
						sysprint("[APP_MICOM] --- Power Switch Pressed. It Will be Shutdown ---\n");
						//# add rupy
						ctrl_sys_halt(1); /* for shutdown */
						exit = 1;
					} else {
						dprintf("skip power switch event!!!\n");
					}
				} else {
#if defined(NEXXONE) || defined(NEXX360W) || defined(NEXXB) || defined(NEXX360W_MUX)
					#if SYS_CONFIG_VOIP 
					if (!app_cfg->ste.b.ftp_run) 
					{     
						if (app_rec_state()) {
							app_rec_stop(ON);
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
#elif defined(NEXX360H)
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
	if(thread_create(tObj, THR_micom, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
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
	int ret,ver;
	
	ret = mic_msg_init();
	if(ret < 0) {
		return -1;
	}
	
	mic_send_ready();
	ver = (int)mic_get_version();
	if(ver < SYS_MCU_VER) {
		printf(" [warning] micom version is old!!!\n");
	}
	
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
