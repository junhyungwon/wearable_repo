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
#include "app_snd_capt.h"
#include "app_buzz.h"
#include "app_process.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define REC_SEC_01MIN			1	//# max 1 min.
#define REC_SEC_05MIN			5	//# max 5 min.

#define PRE_REC_SEC             15      // max 15 secs.

typedef struct {
	app_thr_obj sObj;		//# rec message send thread
	app_thr_obj rObj;		//# rec message receive thread
	app_thr_obj bObj;		//# event buzzer thread
	app_thr_obj kObj;		//# stop sos event packet
	
	int init;
	int qid;
	int rec_state;			//# 1:recording, 0:idle
	
	int snd_ch;				//# sound channel
	int snd_rate;			//# sampling rate
	int snd_btime;			//# buffer size
	
} app_rec_t;

static unsigned int grec_time[REC_PERIOD_MAX] = { REC_SEC_01MIN, REC_SEC_05MIN };

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
		TRACE_INFO("REC File %s done!!\n", msg.fname);
		app_file_add_list(msg.fname);
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
	
	TRACE_INFO("enter...\n");
	//# message queue
	irec->qid = Msg_Init(REC_MSG_KEY);
	
	while (!exit) {
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			TRACE_ERR("failed to receive rec process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case AV_CMD_REC_READY:
			irec->init = 1; /* from record process */
			break;
		
		case AV_CMD_REC_EXIT:
			exit = 1;
			break;
		
		case AV_CMD_REC_EVT_END:
		    event_send(&irec->bObj, APP_CMD_STOP, 0, 0);  // for stop buzzer
            send_msg(AV_CMD_REC_STOP);

		    irec->rec_state = 0;
			app_buzz_ctrl(100, 2);
		    app_leds_rec_ctrl(LED_REC_OFF);
			break;

		case AV_CMD_REC_RESTART:
			//TRACE_INFO("received Event record Restart!\n");
		    event_send(&irec->bObj, APP_CMD_STOP, 0, 0);  // for stop buzzer
 
            send_msg(AV_CMD_REC_START);
			irec->rec_state = 1;
			app_buzz_ctrl(100, 1);
			app_leds_rec_ctrl(LED_REC_ON);
			break;
			
		case AV_CMD_REC_FLIST:
			//TRACE_INFO("update list of record files!\n");
			app_file_fxn();
			break;
			
		case AV_CMD_REC_ERR:
			LOGE("[main] Failed to execute video record process!..Stopping System!!\n");
			irec->rec_state = 0; /* record stop...*/
			app_cfg->ste.b.mmc_err = 1; /* for watchdog */
			app_leds_rec_ctrl(LED_REC_FAIL);
			break;
	
		default:
			break;	
		}
	}
	
	Msg_Kill(irec->qid);
	TRACE_INFO("exit...\n");
		
	return NULL;
}

/*****************************************************************************
* @brief    event record thread function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_rec_send_msg(void *prm)
{
	app_thr_obj *tObj = &irec->sObj;
	int cmd = 0, option = 0;
	int exit = 0;
	
	TRACE_INFO("enter...\n");
	tObj->active = 1;
	
	irec->rec_state = 0;
	app_leds_rec_ctrl(LED_REC_OFF);
		
	while (!exit)
	{
		cmd = event_wait(tObj);
		option = tObj->param0 ;
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
			if(option) {
				irec->rec_state  = 2;// SOS
				send_msg(AV_CMD_REC_SOS);
			} else {
                irec->rec_state  = 1; // Event 
			    send_msg(AV_CMD_REC_EVT);
			}

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
		
	TRACE_INFO("exit\n");
	return NULL;
}

/*****************************************************************************
* @brief    SOS Stop thread function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_stop_sos(void *prm)
{
	app_thr_obj *tObj = &irec->kObj;
	int cmd = 0, loop_cnt = 0, evt_sndcnt = 0, option = 0;
	int exit = 0;
	
	TRACE_INFO("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		option = tObj->param0 ;
		if (cmd == APP_CMD_EXIT) {
			/* send exit command to stop sos process */
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

			if(tObj->cmd == APP_SOS_SEND_STOP)
			{
				if(!(loop_cnt % 20)) // 1초 간격
				{
					if(evt_sndcnt < 10)   // event 시 10회만 전달 
					{
						if(option) // SOS
							stop_sos_send() ;

					    evt_sndcnt += 1 ;
					}
					else
					{
					    tObj->cmd = APP_REC_EVT ;
						evt_sndcnt = 0 ;
					}
					loop_cnt = 0 ;
				}
			}
			OSA_waitMsecs(50);
			loop_cnt += 1 ;
        }
//		evt_sndcnt = 0 ;
	}
	
	tObj->active = 0;
		
	TRACE_INFO("exit\n");
	return NULL;
}

/*****************************************************************************
* @brief    event buzzer thread function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_evt_buzzer(void *prm)
{
	app_thr_obj *tObj = &irec->bObj;
	int cmd = 0, buzzer_cnt = 0, evt_sndcnt = 0, option = 0;
	int exit = 0;
	int status = 0, i = 0 ;
	
	TRACE_INFO("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		option = tObj->param0 ;
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
					
					for(i = 0 ; i < MODEL_CH_NUM + EXCHANNEL; i++) {
						app_leds_cam_ctrl(i, status);
					}
				}

				if(!(buzzer_cnt % 20)) // 1초 간격
				{
					if(evt_sndcnt < 10)   // event 시 10회만 전달 
					{
#ifndef NEXX360B
						if(option) // SOS
							sosdata_send() ;
						else
						    eventdata_send() ;					    

					    evt_sndcnt += 1 ;
#endif
					}
				}

				if(!(buzzer_cnt % 60)) // 3초 간격
                {
					if(option) // SOS
					   app_buzz_ctrl(100, 3) ;

                    buzzer_cnt = 0 ;
				}
			}
			OSA_waitMsecs(50);
			buzzer_cnt += 1 ;
        }
		
		status = ON ;
		for(i = 0 ; i < MODEL_CH_NUM + EXCHANNEL; i++) {
		    app_leds_cam_ctrl(i, status);
		}
		evt_sndcnt = 0 ;
	}
	
	tObj->active = 0;
	TRACE_INFO("exit\n");
	
	return NULL;
}

static int _is_enable_rec_start(int rec_type)
{	
	//# currently record
	if (irec->rec_state && !rec_type) {
		TRACE_ERR("currently recording...\n");
		return EFAIL;
	}

	if (!app_cfg->en_rec || !app_cfg->ste.b.cap || app_cfg->ste.b.mmc_err || 
		app_cfg->ste.b.busy || (app_cfg->vid_count == 0)) 
	{
		TRACE_ERR("can't record cuz %s %s %s %s %s\n",
			app_cfg->ste.b.mmc_err?"Damaged MMC":"", app_cfg->ste.b.busy?"system busy":"",
			app_cfg->ste.b.cap?"":"no Capture", app_cfg->en_rec?"":"no Codec",
			(app_cfg->vid_count > 0)?"":"no video detect");
		return EFAIL;
	}

//	if ((!app_cfg->rec_overwrite) && (app_file_check_disk_free_space() == EFAIL)) 
	if ((!app_set->rec_info.overwrite)) 
	{
		if(rec_type) {
			app_cfg->rec_overwrite = ON ;
		}
		else {
			if (app_file_check_disk_free_space() == EFAIL) {
				TRACE_ERR("Bypass start record!\n");
				return EFAIL;
			}
		}
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
	
	TRACE_INFO("Recording start!\n");
	
	//# Record start if captuer is not zero.
    app_buzz_ctrl(100, 1);			//# buzz: rec start
	event_send(&irec->sObj, APP_REC_START, 0, 0);
	
	return SOK;
}

int app_sos_send_stop(int etype)
{
	TRACE_INFO("STOP Sending SOS Packet!!\n");
	event_send(&irec->kObj, APP_SOS_SEND_STOP, etype, 0); // etype == 0 event, etype == 1 SOS
	
	return SOK;
}

int app_rec_evt(int etype)
{
	int type = 1; // normal  0, event 1

	//# Check the status of recording.
	if (_is_enable_rec_start(type) == EFAIL)
		return EFAIL;
	
	if (!irec->init) {
		OSA_waitMsecs(50);
	}
	
	TRACE_INFO("Event Record Process Start!!\n");
	
	//# Record start if captuer is not zero.
    app_buzz_ctrl(100, 3);			//# buzz: rec start
	event_send(&irec->sObj, APP_REC_EVT, etype, 0);
	event_send(&irec->bObj, APP_REC_EVT, etype, 0); // etype == 0 event, etype == 1 SOS
	event_send(&irec->kObj, APP_REC_EVT, etype, 0); // etype == 0 event, etype == 1 SOS
	
	return SOK;
}

int app_rec_stop(int prerec_flag)
{
	TRACE_INFO("app_rec_stop call reached app_rec.c\n") ;
	
	if (irec->rec_state) {
		if(app_cfg->rec_overwrite != app_set->rec_info.overwrite)
		{	
			app_cfg->rec_overwrite = app_set->rec_info.overwrite ;
		}
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
	irec->snd_rate = APP_SND_CAPT_SRATE;
	irec->snd_ch = APP_SND_CAPT_CH;
	irec->snd_btime = APP_SND_CAPT_SRATE * APP_SND_CAPT_PTIME / 1000;
	
	/* start rec process */
	snprintf(cmd, sizeof(cmd), "/opt/fit/bin/av_rec.out %x &", (int)g_mem_get_phyaddr());
	system(cmd);
	
	//#--- create msg receive thread
	tObj = &irec->rObj;
	if(thread_create(tObj, THR_rec_recv_msg, APP_THREAD_PRI, tObj, __FILENAME__) < 0) {
		TRACE_ERR("create thread\n");
		return EFAIL;
	}
	
	//#--- create msg send thread
	tObj = &irec->sObj;
	if(thread_create(tObj, THR_rec_send_msg, APP_THREAD_PRI, tObj, __FILENAME__) < 0) {
		TRACE_ERR("create thread\n");
		return EFAIL;
	}

	//#--- create event buzzer thread
	tObj = &irec->bObj;
	if(thread_create(tObj, THR_evt_buzzer, APP_THREAD_PRI, tObj, __FILENAME__) < 0) {
		TRACE_ERR("create thread\n");
		return EFAIL;
	}

	//#--- create sos packet thread for stop
	tObj = &irec->kObj;
	if(thread_create(tObj, THR_stop_sos, APP_THREAD_PRI, tObj, __FILENAME__) < 0) {
		TRACE_ERR("create thread\n");
		return EFAIL;
	}

	TRACE_INFO("... done!\n");
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
	
	TRACE_INFO("done!...\n");

	return SOK;
}
