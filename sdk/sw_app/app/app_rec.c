/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_rec.c
 * @brief	app record thread
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>

#include "av_rec_ipc_cmd_defs.h"
#include "app_comm.h"
#include "app_main.h"
#include "app_gmem.h"
#include "app_dev.h"
#include "app_rec.h"
#include "app_set.h"
#include "app_file.h"
#include "app_util.h"
#include "app_snd.h"
#include "app_buzz.h"
#include "app_process.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define REC_SEC_01MIN			1	//# max 1 min.
#define REC_SEC_O5MIN			5	//# max 5 min.

#define PRE_REC_SEC             15      // max 15 secs.

typedef struct {
	app_thr_obj sObj;		//# rec message send thread
	app_thr_obj rObj;		//# rec message receive thread
	app_thr_obj bObj;		//# event buzzer thread
	
	int init;
	int qid;
	int rec_state;			//# 1:recording, 0:idle
	
	int snd_ch;				//# sound channel
	int snd_rate;			//# sampling rate
	int snd_btime;			//# buffer size
	
} app_rec_t;

static unsigned int grec_time[REC_PERIOD_MAX] = { REC_SEC_01MIN, REC_SEC_O5MIN };

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_rec_t rec_obj;
static app_rec_t *irec=&rec_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

static int send_msg(int cmd)
{
	to_rec_msg_t msg;
	
	msg.type = REC_MSG_TYPE_TO_REC;
	msg.cmd = cmd;

	//# rec param
	//# set the record period.
	msg.stime = grec_time[app_set->rec_info.period_idx];   	//# save time
	msg.en_snd = app_set->rec_info.audio_rec;  				//# sound enable
	msg.en_pre = app_set->rec_info.pre_rec; 				//# todo
	msg.fr = 0;        										//# frame rate..
	
	if (cmd != AV_CMD_REC_STOP) {
		msg.snd_ch    = irec->snd_ch; 
		msg.snd_rate  = irec->snd_rate; 
		msg.snd_btime = irec->snd_btime; 
	}
	
	memcpy(msg.deviceId, app_set->sys_info.deviceId, MAX_CHAR_32);
	
	return Msg_Send(irec->qid, (void *)&msg, sizeof(to_rec_msg_t));
}

static int recv_msg(void)
{
	to_main_msg_t msg;
	
	//# blocking
	if (Msg_Rsv(irec->qid, REC_MSG_TYPE_TO_MAIN, (void *)&msg, sizeof(to_main_msg_t)) < 0)
		return -1;

	if (msg.cmd == AV_CMD_REC_FLIST) {
		notice("REC File %s done!!(size %u)\n", msg.fname, msg.du);
		app_file_add_list(msg.fname, msg.du);
	}

	return msg.cmd;
}

/*****************************************************************************
* @brief    event record message function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_rec_recv_msg(void *prm)
{
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	//# message queue
	irec->qid = Msg_Init(REC_MSG_KEY);
	
	while (!exit) {
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive rec process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case AV_CMD_REC_READY:
			irec->init = 1; /* from record process */
			dprintf("received rec ready!\n");
			break;
		
		case AV_CMD_REC_EXIT:
			exit = 1;
			dprintf("received rec exit!\n");
			break;
		
		case AV_CMD_REC_EVT_END:
			dprintf("received Event record done!\n");
		    event_send(&irec->bObj, APP_CMD_STOP, 0, 0);  // for stop buzzer
            send_msg(AV_CMD_REC_STOP);

		    irec->rec_state = 0;
			app_buzz_ctrl(100, 2);
		    app_leds_rec_ctrl(LED_REC_OFF);
			
			break;

		case AV_CMD_REC_RESTART:
			dprintf("received Event record Restart!\n");
		    event_send(&irec->bObj, APP_CMD_STOP, 0, 0);  // for stop buzzer
 
            send_msg(AV_CMD_REC_START);
			irec->rec_state = 1;
			app_buzz_ctrl(100, 1);
			app_leds_rec_ctrl(LED_REC_ON);
			
			break ;
		case AV_CMD_REC_ERR:
			dprintf("received rec error!\n");
			irec->rec_state = 0; /* record stop...*/
			app_cfg->ste.b.mmc_err = 1; /* for watchdog */
			app_leds_rec_ctrl(LED_REC_FAIL);
			break;

		default:
			break;	
		}
	}
	
	Msg_Kill(irec->qid);
	aprintf("exit...\n");
		
	return NULL;
}

/*****************************************************************************
* @brief    event record thread function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_rec_send_msg(void *prm)
{
	app_thr_obj *tObj = &irec->sObj;
	int cmd = 0;
	int exit = 0;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	irec->rec_state = 0;
	app_leds_rec_ctrl(LED_REC_OFF);
		
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			/* send exit command to rec process */
			exit = 1;
			break;
		} else if (cmd == APP_REC_START) {
			send_msg(AV_CMD_REC_START);
			
			irec->rec_state = 1;
			app_leds_rec_ctrl(LED_REC_ON);
			/* TODO */
		} else if (cmd == APP_REC_EVT) {
			send_msg(AV_CMD_REC_EVT);

            irec->rec_state  = 1;  
			app_leds_rec_ctrl(LED_REC_ON);
			/* TODO */
		} else if (cmd == APP_CMD_STOP) {
			send_msg(AV_CMD_REC_STOP);
			
			irec->rec_state = 0 ;
			app_leds_rec_ctrl(LED_REC_OFF);

		} else if (cmd == APP_CMD_GSTOP) {
			send_msg(AV_CMD_REC_GSTOP);
			
			irec->rec_state = 0 ;
			app_leds_rec_ctrl(LED_REC_OFF);
		}
	}
	
	tObj->active = 0;
	irec->rec_state = 0;
	app_leds_rec_ctrl(LED_REC_OFF);
		
	aprintf("exit\n");
	
	return NULL;
}

/*****************************************************************************
* @brief    event buzzer thread function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_evt_buzzer(void *prm)
{
	app_thr_obj *tObj = &irec->bObj;
	int cmd = 0, buzzer_cnt = 0, evt_sndcnt = 0;
	int exit = 0;
	int status = 0, i = 0 ;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			/* send exit command to event buzzer process */
			exit = 1;
			break;
		}  
		else if (cmd == APP_CMD_STOP) {
            continue ;
		} 
		while(1) 
		{
            if (tObj->cmd == APP_CMD_EXIT || tObj->cmd == APP_CMD_STOP) {
			    break;
			} 
			if (cmd == APP_REC_EVT) {

				if(!(buzzer_cnt % 10))
				{
					if(status)
						status = OFF ;
					else
						status = ON ;

#if defined(NEXXONE) || defined(NEXX360H)
					app_leds_cam_ctrl(0, status) ;
#else
					for(i = 0 ; i < MODEL_CH_NUM; i++)
					{
						app_leds_cam_ctrl(i, status);
					}
#endif
				}
				if(!(buzzer_cnt % 20))
                {
					if(evt_sndcnt < 10)   // event 시 10회만 전달 
					{
					    eventdata_send() ;
					    evt_sndcnt += 1 ;
					}

					app_buzz_ctrl(100, 3) ;
                    buzzer_cnt = 0 ;
				}
			}
			status = ON ;
			for(i = 0 ; i < MODEL_CH_NUM; i++)
			{
				app_leds_cam_ctrl(i, status);
			}

			OSA_waitMsecs(50);
			buzzer_cnt += 1 ;
		}
		evt_sndcnt = 0 ;
	}
	
	tObj->active = 0;
		
	aprintf("exit\n");
	
	return NULL;
}


static int _is_enable_rec_start(int rec_type)
{	
	//# currently record
	if (irec->rec_state && !rec_type) {
		eprintf("currently recording...\n");
		return EFAIL;
	}

	if (!app_cfg->en_rec || !app_cfg->ste.b.cap || !app_cfg->ste.b.mmc || 
		app_cfg->ste.b.busy || (app_cfg->vid_count == 0)) 
	{
		eprintf("can't record cuz %s %s %s %s %s\n",
			app_cfg->ste.b.mmc?"":"no MMC!", app_cfg->ste.b.busy?"system busy":"",
			app_cfg->ste.b.cap?"":"no Capture", app_cfg->en_rec?"":"no Codec",
			(app_cfg->vid_count > 0)?"":"no video detect");
		return EFAIL;
	}

	if ((!app_set->rec_info.overwrite) && (app_file_check_disk_free_space() == EFAIL)) {
		eprintf("Bypass start record!\n");
		return EFAIL;
	}

	return SOK;
}

/*****************************************************************************
* @brief    record start/evt_rec/stop function
* @section
*****************************************************************************/
int app_rec_start(void)
{
	int type = 0; // normal  0, event 1

	//# Check the status of recording.
	if (_is_enable_rec_start(type) == EFAIL)
		return EFAIL;
	
	if (!irec->init) {
		OSA_waitMsecs(50);
	}
	
	aprintf("Record Process Start!!\n");
	
	//# Record start if captuer is not zero.
    app_buzz_ctrl(100, 1);			//# buzz: rec start
	event_send(&irec->sObj, APP_REC_START, 0, 0);
	
	return SOK;
}

int app_rec_evt(void)
{
	int start = 1, type = 1; // normal  0, event 1
	unsigned long sz;

	//# Check the status of recording.
	if (_is_enable_rec_start(type) == EFAIL)
		return EFAIL;
	
	if (!irec->init) {
		OSA_waitMsecs(50);
	}
	
	aprintf("Event Record Process Start!!\n");
	
	//# Record start if captuer is not zero.
    app_buzz_ctrl(500, 1);			//# buzz: rec start
	event_send(&irec->sObj, APP_REC_EVT, 0, 0);
	event_send(&irec->bObj, APP_REC_EVT, 0, 0);
	
	return SOK;
}

int app_rec_stop(int prerec_flag)
{
	if (irec->rec_state) {
		app_buzz_ctrl(100, 2);	//# buzz: rec stop
		if (prerec_flag) // record 종료시 이전 record 상태를 유지 종료 후 이전 record 상태로 돌아감 
		{
			event_send(&irec->sObj, APP_CMD_STOP, 0, 0);
		}
		else // record 종료시 이전 record 상태를 초기화 해서 바로 record 종료 되도록 
		{
			event_send(&irec->sObj, APP_CMD_GSTOP, 0, 0);
		}

		event_send(&irec->bObj, APP_CMD_STOP, 0, 0);
	}

	return SOK;
}

/*****************************************************************************
* @brief    current record state
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_rec_state(void)
{
	/* 0-> idle, 1 -> recording */
	return irec->rec_state;
}

/*****************************************************************************
* @brief    app record init/exit function
* @section  [desc]
*****************************************************************************/
int app_rec_init(void)
{
	app_thr_obj *tObj;
	char cmd[128] = {0,};
	
	//# 0-> idle, 1-> recording
	if (!app_cfg->en_rec) {
		return SOK;
	}
	
	//# static config clear - when Variable declaration
	memset((void *)irec, 0x0, sizeof(app_rec_t));
	
	//# set default sound param
	irec->snd_rate = APP_SND_SRATE;
	irec->snd_ch = APP_SND_CH;
	irec->snd_btime = APP_SND_SRATE * APP_SND_PTIME / 1000;
	
	/* start rec process */
	snprintf(cmd, sizeof(cmd), "/opt/fit/bin/av_rec.out %x &", (int)g_mem_get_phyaddr());
	system(cmd);
	
	//#--- create msg receive thread
	tObj = &irec->rObj;
	if(thread_create(tObj, THR_rec_recv_msg, APP_THREAD_PRI, tObj, __FILENAME__) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	//#--- create msg send thread
	tObj = &irec->sObj;
	if(thread_create(tObj, THR_rec_send_msg, APP_THREAD_PRI, tObj, __FILENAME__) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}

	//#--- create event buzzer thread
	tObj = &irec->bObj;
	if(thread_create(tObj, THR_evt_buzzer, APP_THREAD_PRI, tObj, __FILENAME__) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}

	aprintf("... done!\n");

	return SOK;
}

int app_rec_exit(void)
{
	app_thr_obj *tObj;

	//#--- stop message send thread
	if (irec->rec_state)
	{
		tObj = &irec->sObj;
		event_send(tObj, APP_CMD_EXIT, 0, 0);
		while (tObj->active) {
			app_msleep(20);
		}
	}
	thread_delete(tObj);
	
	//#--- stop message receive thread. 
//	tObj = &irec->rObj;
//	thread_delete(tObj);
	
	aprintf("done!...\n");

	return SOK;
}
