/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_gps.c
 * @brief	app gps thread
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "dev_gpio.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_dev.h"
#include "app_set.h"
#include "app_util.h"
#include "app_gps.h"
#include "gnss_ipc_cmd_defs.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_GPS_CYCLE		200		//# msec
#define EN_REC_META			1
#define EN_GPS				1

#define TIME_META_SENDING    60000   //# 1Minute
#define CNT_FITTMETA        (TIME_META_SENDING/TIME_GPS_CYCLE)

typedef struct {
	app_thr_obj sObj;		//# gps message send thread
	app_thr_obj rObj;		//# gps message receive thread
	app_thr_obj hObj;		//# gps (meta & port detect) receive thread
	
	int init;
	int qid;
	
	int shmid;
	unsigned char *sbuf;
	
	FIFO *pfifo;
	
	gps_rmc_t gps;
	
	OSA_MutexHndl mutex_gps;
	OSA_MutexHndl mutex_meta;
	
} app_gps_obj_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_gps_obj_t t_gps_obj;
static app_gps_obj_t *igps=&t_gps_obj;

static char fitt_meta_str[META_REC_TOTAL];
static int fitt_cnt = 0;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static int dev_ste_gps_port(void)
{
	int status;

	gpio_get_value(GPS_PWR_EN, &status);
	//dprintf("--- [gps port] value %d\n", status);

	return status;
}

static int send_msg(int cmd)
{
	to_gnss_msg_t msg;
	
	msg.type = GNSS_MSG_TYPE_TO_GPS;
	msg.cmd = cmd;

	//# rec param
	msg.rate = 9600;   //# gps uart rate
	strcpy(msg.dev_name, "/dev/ttyO1");
	
	return Msg_Send(igps->qid, (void *)&msg, sizeof(to_gnss_msg_t));
}

static int recv_msg(void)
{
	to_gnss_main_msg_t msg;
	int size;
	
	//# blocking
	if (Msg_Rsv(igps->qid, GNSS_MSG_TYPE_TO_MAIN, (void *)&msg, sizeof(to_gnss_main_msg_t)) < 0)
		return -1;

	return msg.cmd;
}

static void fitt_meta_reset(void)
{
    fitt_cnt = 0;
    memset(fitt_meta_str, 0, META_REC_TOTAL);
}

static int __gps_start(void)
{
	/* gps 프로세스가 시작하지 않은 경우... */
	if (!igps->init) {
		OSA_waitMsecs(50);
	}
	
	aprintf("GPS Process Start!!\n");
	event_send(&igps->sObj, APP_CMD_START, 0, 0);
	
	return SOK;
}

static int __gps_stop(void)
{
	event_send(&igps->sObj, APP_CMD_STOP, 0, 0);
	
	return SOK;
}

/*****************************************************************************
 * @brief    get device data
 * @section  DESC Description
 *   - desc   POWER_HOLD pin to Low(Power OFF)
 *****************************************************************************/
void dev_get_gps_rmc(gps_rmc_t *gps)
{
	OSA_mutexLock(&igps->mutex_gps);

	//memcpy((void *)gps, &idev->gps, sizeof(gps_rmc_t));

	OSA_mutexUnlock(&igps->mutex_gps);
}

/*****************************************************************************
* @brief    event gps message function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_gps_recv_msg(void *prm)
{
	app_thr_obj *tObj = &igps->rObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	
	//# message queue
	igps->qid = Msg_Init(GNSS_MSG_KEY);
	
	while (!exit) {
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive gps process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case GNSS_CMD_GPS_READY:
			igps->init = 1; /* from record process */
			dprintf("received gps ready!\n");
			break;
		case GNSS_CMD_GPS_EXIT:
			exit = 1;
			dprintf("received gps exit!\n");
			break;
		default:
			break;	
		}
	}
	
	Msg_Kill(igps->qid);
	
	aprintf("exit...\n");
		
	return NULL;
}

/*****************************************************************************
* @brief    event record thread function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_gps_send_msg(void *prm)
{
	app_thr_obj *tObj = &igps->sObj;
	int cmd = 0;
	int exit = 0;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			/* send exit command to rec process */
			exit = 1;
			break;
		} else if (cmd == APP_CMD_START) {
			send_msg(GNSS_CMD_GPS_START);
		} else if (cmd == APP_CMD_STOP) {
			send_msg(GNSS_CMD_GPS_STOP);
		}
	}
	
	tObj->active = 0;
	aprintf("...exit\n");
	
	return NULL;
}

/*****************************************************************************
* @brief	gps thread function
* @section  [desc]
*****************************************************************************/
static void *THR_gps_main(void *prm)
{
	app_thr_obj *tObj = &igps->hObj;
	int exit=0, cmd, state;
	int meta_timer = CNT_FITTMETA;
	int connect_state = 0; /* default off */

	aprintf("enter...\n");
	tObj->active = 1;
	
    gpio_input_init(GPS_PWR_EN);
	app_leds_gps_ctrl(LED_GPS_OFF); //#default LED OFF
	
	fitt_meta_reset();
	
	while(!exit)
	{
		app_cfg->wd_flags |= WD_DEV;

		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP || app_cfg->ste.b.pwr_off) {
            break;
        }
		
		state = dev_ste_gps_port();
		if (state) {
			if (!connect_state) {
				/* GPS를 시작한다. */
				connect_state = 1;
				__gps_start();
			}
		}
		
		#if 0
		if (meta_timer <= 0) {
			dev_get_gps_rmc((void*)&Gpsdata);
			gpsdata_send((void*)&Gpsdata);
			meta_timer = CNT_FITTMETA;
		} else {
			meta_timer--;
		}
		#endif
		app_msleep(TIME_GPS_CYCLE);
	}

	gpio_exit(GPS_PWR_EN);
	
	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    app gps init function
* @section  [desc]
*****************************************************************************/
int app_gps_init(void)
{
	app_thr_obj *tObj;
	char cmd[128] = {0,};
	int offset = 0, status;
	
	//# static config clear - when Variable declaration
	memset((void *)igps, 0x0, sizeof(app_gps_obj_t));
	
	/* start gps process */
	snprintf(cmd, sizeof(cmd), "/opt/fit/bin/app_gnss.out &");
	system(cmd);
	
	/* Create Mutex Handle */
	status = OSA_mutexCreate(&(igps->mutex_gps));
    OSA_assert(status == OSA_SOK);

    if (app_set->srv_info.ON_OFF) {
        status = OSA_mutexCreate(&igps->mutex_meta);
        OSA_assert(status == OSA_SOK);
    }
	
	//#--- create msg receive thread
	tObj = &igps->rObj;
	if (thread_create(tObj, THR_gps_recv_msg, APP_THREAD_PRI, tObj) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	//#--- create msg send thread
	tObj = &igps->sObj;
	if (thread_create(tObj, THR_gps_send_msg, APP_THREAD_PRI, tObj) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	tObj = &igps->hObj;
	if (thread_create(tObj, THR_gps_main, APP_THREAD_PRI, tObj) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	/*  gps 프로세서 초기화가 수행될때까지 delay를 준다. */
	app_msleep(500);
	igps->shmid = shmget((key_t)GNSS_SHM_KEY, 0, 0);
	if (igps->shmid == -1) {
		eprintf("shared memory for gps is not created!!\n");
	}
	
	//# get shared memory
	igps->sbuf = (unsigned char *)shmat(igps->shmid, NULL, 0);
	if (igps->sbuf == NULL) {
		eprintf("shared memory for gps scan is NULL!!!");
	}
	
	igps->pfifo = igps->sbuf;
	dprintf("shared memory 0x%08x\n", (int)igps->pfifo);
	
	aprintf("... done!\n");

	return SOK;
}

/*****************************************************************************
* @brief    app gps exit function
* @section  [desc]
*****************************************************************************/
int app_gps_exit(void)
{
	app_thr_obj *tObj;
	
	/* gps_main thread */
	tObj = &igps->hObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active) {
		app_msleep(20);
	}
	thread_delete(tObj);
	
	//#--- stop message send thread
	tObj = &igps->sObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active) {
		app_msleep(20);
	}
	thread_delete(tObj);
	
	//#--- stop message receive thread. 
	//# 프로세스에서 이미 종료가 되므로 APP_CMD_EXIT를 하면 안됨.
//	tObj = &irec->rObj;
//	thread_delete(tObj);

	/* mutex delete */
    OSA_mutexDelete(&igps->mutex_gps);

    if (app_set->srv_info.ON_OFF)
        OSA_mutexDelete(&igps->mutex_meta);
	
	aprintf("done!...\n");

	return SOK;
}
