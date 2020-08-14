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
#include <sys/types.h>
#include <sys/stat.h>

#include "ti_vsys.h"

#include "dev_gpio.h"
#include "dev_common.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_dev.h"
#include "app_set.h"
#include "app_util.h"
#include "app_gps.h"
#include "app_process.h"

#include "gnss_ipc_cmd_defs.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define GNSS_BIN_STR		"/opt/fit/bin/app_gnss.out"
#define GNSS_CMD_STR		"/opt/fit/bin/app_gnss.out &"

#define GPS_PROC_LENGTH		1000 /*  nmea ?∞Ïù¥????1000Í∞?76 *1000 = 760KB Í∞Ä ?ÑÏöî??*/

#define TIME_GPS_CYCLE		200		//# msec
#define EN_REC_META			1
#define EN_GPS				1

#define TIME_META_SENDING   1000 // 60000   //# 1Minute
#define CNT_FITTMETA        (TIME_META_SENDING/TIME_GPS_CYCLE)

typedef struct FIFO {
	unsigned int count;
	unsigned int readIndex;
	unsigned int writeIndex;
	unsigned int len;
	
	pthread_mutex_t lock;
	pthread_cond_t  condRd;
	pthread_cond_t  condWr;
  
	unsigned char *buf;
} FIFO;

typedef struct {
	app_thr_obj rObj;		//# gps message receive thread
	app_thr_obj hObj;		//# gps (meta & port detect) receive thread
	
	int init;
	int qid;
	int isPlugIn;
	int shmid;
	unsigned char *sbuf;
	
	FIFO fifo;
	gnss_shm_data_t r_data;
	
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
static int gps_fifo_init(FIFO *fifo, int len)
{
	pthread_mutexattr_t mutex_attr;
	int status = 0;
	
	if (fifo == NULL)
		return -1;
	
	fifo->count = 0;
	fifo->readIndex = 0;
	fifo->writeIndex = 0;
	fifo->len = len;
	fifo->buf = (unsigned char *)malloc(len); 
	if (fifo->buf == NULL) {
    	eprintf("failed to init gps fifo!\n");
    	return -1;
  	}
	
	status |= pthread_mutexattr_init(&mutex_attr);
	status |= pthread_mutex_init(&fifo->lock, &mutex_attr);
	if (status != 0)
    	eprintf("failed to create gps fifo %d!\n", status);
	
  	pthread_mutexattr_destroy(&mutex_attr);
  	
	return 0;
}

static int gps_fifo_get(FIFO *fifo, unsigned int addr, int size) 
{
	int status = 0;
	
	pthread_mutex_lock(&fifo->lock);
	
	if ((fifo->len-fifo->readIndex) < size)
		fifo->readIndex = 0;
	
	memcpy((char *)addr, (char *)&fifo->buf[fifo->readIndex], size);
	fifo->readIndex += size;
	if (fifo->readIndex >= fifo->len)
		fifo->readIndex=0;
	fifo->count--;
	
	pthread_mutex_unlock(&fifo->lock);
	
	return status;
}

static int gps_fifo_put(FIFO *fifo, unsigned int addr, unsigned int size) 
{
	int status = 0;
	
  	pthread_mutex_lock(&fifo->lock);
  
	if ((fifo->len-fifo->writeIndex) < size)
		fifo->writeIndex = 0;
		
	memcpy((char *)&fifo->buf[fifo->writeIndex], (char *)addr, size);
	fifo->writeIndex += size; 
	
	if (fifo->writeIndex >= fifo->len)
		fifo->writeIndex = 0;
	fifo->count++;
	
	pthread_mutex_unlock(&fifo->lock);
	
	return status;
}

static int gps_fifo_isEmpty(FIFO *fifo)
{
	if (fifo->count == 0) {
		return 1;
	} else {
		return 0;
	}
}

static int gps_fifo_isFull(FIFO *fifo) 
{
	if (fifo->count >= fifo->len)
		return 1;
	else
		return 0;
}

static int gps_fifo_clear(FIFO *fifo) 
{
	if (fifo == NULL)
		return -1;
	
	fifo->count		 = 0;
	fifo->readIndex  = 0;
	fifo->writeIndex = 0;
	free(fifo->buf);
	
	pthread_cond_destroy(&fifo->condRd);
  	pthread_cond_destroy(&fifo->condWr);
  	pthread_mutex_destroy(&fifo->lock);  
  
	return 0;
}

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
	
	//# blocking
	if (Msg_Rsv(igps->qid, GNSS_MSG_TYPE_TO_MAIN, (void *)&msg, sizeof(to_gnss_main_msg_t)) < 0)
		return -1;

	return msg.cmd;
}

static int __gps_send_cmd(int cmd)
{
	send_msg(cmd);
	//dprintf("send command to GPS process(%x)!!\n", cmd);
	
	return SOK;
}

static int app_sys_time(struct tm *ts)
{
	int timezone = app_set->time_info.time_zone - 12;
	time_t now, set;

    //# get current time
	now = time(NULL);

	//# get set time
	set = mktime(ts) + (timezone*3600);

	//# if difference 1min.
	if(abs(now-set) > 60)
	{
		stime(&set);
		Vsys_datetime_init();	//# m3 Date/Time init
    	app_msleep(100);
		if (dev_rtc_set_time(*ts) < 0) {
			eprintf("Failed to set system time to rtc\n!!!");
		}
		aprintf("--- changed time from GPS ---\n");
	}

	return SOK;
}

/*****************************************************************************
 * @brief    get device data
 * @section  DESC Description
 *   - desc   POWER_HOLD pin to Low(Power OFF)
 *****************************************************************************/
int app_gps_get_rmc_data(app_gps_meta_t *p_meta)
{
	gnss_shm_data_t tmp_data;
	int status = -1;
	
	if (p_meta == NULL)
		return status;
	
	//OSA_mutexLock(&igps->mutex_gps);

	p_meta->enable     = igps->r_data.gps_fixed;
	p_meta->subsec     = igps->r_data.subsec;
	p_meta->speed      = igps->r_data.speed;
	p_meta->lat        = igps->r_data.lat; 
	p_meta->lot        = igps->r_data.lot;
	p_meta->dir        = igps->r_data.dir;
	
	memcpy(&p_meta->gtm, &igps->r_data.gtm, sizeof(struct tm));
	//p_meta->gtm.tm_year = tmp_data.gtm.tm_year; /* +1900 ?àÎê® */
	//p_meta->gtm.tm_mon  = tmp_data.gtm.tm_mon; 
	//p_meta->gtm.tm_mday = tmp_data.gtm.tm_mday; 
	//p_meta->gtm.tm_hour = tmp_data.gtm.tm_hour; 
	//p_meta->gtm.tm_min  = tmp_data.gtm.tm_min; 
	//p_meta->gtm.tm_sec  = tmp_data.gtm.tm_sec;
	status = 0;

	//OSA_mutexUnlock(&igps->mutex_gps);
	
	return status;
}

static void send_gps_data()
{
	app_gps_meta_t Gpsdata;
	if (app_gps_get_rmc_data((app_gps_meta_t *)&Gpsdata) == 0) {
			gpsdata_send((void*)&Gpsdata);
	} else {
		/* GPS ?∞Í≤∞?Ä ?àÏ?Îß??òÏã†???àÎê† Í≤ΩÏö∞ */
		//dprintf("Failed to get GPRMC Data\n");
	}
}

/*****************************************************************************
* @brief    event gps message function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_gps_recv_msg(void *prm)
{
	app_thr_obj *tObj = &igps->rObj;
	int exit = 0, cmd;
	int sync_time = 1;
	
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
		
		case GNSS_CMD_GPS_POLL_DATA:
			memset(&igps->r_data, 0, sizeof(gnss_shm_data_t));
			
			/* GPS ?ÑÎ°ú?∏Ïä§?êÏÑú shared Î©îÎ™®Î¶¨Ïóê ?Ä?•Ìïú ?∞Ïù¥?∞Î? Í∞Ä?∏Ïò®??*/
			memcpy((char *)&igps->r_data, (char *)igps->sbuf, sizeof(gnss_shm_data_t));

			//GPS LED State
			if(igps->isPlugIn) {
				(igps->r_data.gps_fixed == 0) ? app_leds_gps_ctrl(LED_GPS_FAIL) : app_leds_gps_ctrl(LED_GPS_ON);
			}
			
			/* GPS ?úÍ∞Ñ?ºÎ°ú ?úÏä§??TIME ?ôÍ∏∞??*/
			/* 3D fixed */
			if (igps->r_data.gps_fixed == 2 && sync_time)
			{
				app_sys_time(&igps->r_data.gtm);
				sync_time = 0;
				dev_buzz_ctrl(100, 3);	//# gps time sync
			}

			//Send to client
			send_gps_data();
			
			//# debugging
			#if 1	
			dprintf("GPS POLL- DATE %04d-%02d-%02d, UTC %02d:%02d:%02d, speed=%.2f, (LAT:%.2f, LOT:%.2f)\n",
					igps->r_data.gtm.tm_year+1900, igps->r_data.gtm.tm_mon+1, igps->r_data.gtm.tm_mday,
					igps->r_data.gtm.tm_hour, igps->r_data.gtm.tm_min, igps->r_data.gtm.tm_sec,
					igps->r_data.speed, igps->r_data.lat, igps->r_data.lot);
			#endif	
					
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
	
	/* meta reset */
	fitt_cnt = 0;
    memset(fitt_meta_str, 0, META_REC_TOTAL);
	
	while(!exit)
	{
		app_cfg->wd_flags |= WD_DEV;

		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP || app_cfg->ste.b.pwr_off) {
            break;
        }
		
		igps->isPlugIn = dev_ste_gps_port();
		if (igps->isPlugIn) {
			app_leds_gps_ctrl(LED_GPS_ON);
			if (!connect_state) {
				/* GPSÎ•??úÏûë?úÎã§. */
				connect_state = 1;
				__gps_send_cmd(GNSS_CMD_GPS_START);
				OSA_waitMsecs(50);
				__gps_send_cmd(GNSS_CMD_GPS_START);
				
			}
		} else {
			app_leds_gps_ctrl(LED_GPS_OFF);
			
			if (connect_state) {
				/* GPS ?ïÏ? */
				connect_state = 0;
				__gps_send_cmd(GNSS_CMD_GPS_STOP);
			}
		}
		
		if (connect_state) {
			/* META ?∞Ïù¥???ÑÎã¨ */
			if (meta_timer <= 0) {
				__gps_send_cmd(GNSS_CMD_GPS_REQ_DATA);
				meta_timer = CNT_FITTMETA;
			} else {
				meta_timer--;
			}
		}
		
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
	struct stat sb;
	app_thr_obj *tObj;
	int status;
	
	//# static config clear - when Variable declaration
	memset((void *)igps, 0x0, sizeof(app_gps_obj_t));
	
	/* start gps process */
    if (stat(GNSS_BIN_STR, &sb) != 0) {
		eprintf("can't access gps execute file!\n");
        return -1;
	}
	system(GNSS_CMD_STR);
	
	/* Create Mutex Handle */
	status = OSA_mutexCreate(&(igps->mutex_gps));
    OSA_assert(status == OSA_SOK);

    if (app_set->srv_info.ON_OFF) {
        status = OSA_mutexCreate(&igps->mutex_meta);
        OSA_assert(status == OSA_SOK);
    }
	
	/* create gps fifo */
	status = (sizeof(gnss_shm_data_t)*GPS_PROC_LENGTH);
//	dprintf("fifo initialized: size %d\n", status);
	gps_fifo_init(&igps->fifo, status);
	
	//#--- create msg receive thread
	tObj = &igps->rObj;
	if (thread_create(tObj, THR_gps_recv_msg, APP_THREAD_PRI, tObj) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	/* gps ?ÑÎ°ú?∏Ïä§Í∞Ä ?úÏûë?†Îïå ÍπåÏ? wait... */
	do {
		status = igps->init;
		if (status) {
			//aprintf("couln't start GPS process!\n");
			break;
		}
		OSA_waitMsecs(100);
	} while (1);
	
//	dprintf("GPS process start!\n");
	tObj = &igps->hObj;
	if (thread_create(tObj, THR_gps_main, APP_THREAD_PRI, tObj) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	igps->shmid = shmget((key_t)GNSS_SHM_KEY, 0, 0);
	if (igps->shmid == -1) {
		eprintf("shared memory for gps is not created!!\n");
	}
	
	//# get shared memory
	igps->sbuf = (unsigned char *)shmat(igps->shmid, NULL, 0);
	if (igps->sbuf == NULL) {
		eprintf("shared memory for gps scan is NULL!!!");
	}
	
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
	
	/* mutex delete */
    OSA_mutexDelete(&igps->mutex_gps);

    if (app_set->srv_info.ON_OFF)
        OSA_mutexDelete(&igps->mutex_meta);
	
	gps_fifo_clear(&igps->fifo);
	
	aprintf("done!...\n");

	return SOK;
}
