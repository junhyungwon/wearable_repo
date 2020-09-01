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

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define REC_SEC_01MIN			1	//# max 1 min.
#define REC_SEC_O5MIN			5	//# max 5 min.

#define PRE_REC_SEC             15      // max 15 secs.

typedef struct {
	app_thr_obj sObj;		//# rec message send thread
	app_thr_obj rObj;		//# rec message receive thread
	
	int init;
	int qid;
	int evt_rec;			//# 1:recording, 0:idle
	
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
	msg.stime = grec_time[app_set->rec_info.period_idx];   //# save time
	msg.en_snd = app_set->rec_info.audio_rec;  //# sound enable
	msg.en_pre = app_set->rec_info.pre_rec; //# todo
	msg.fr = 0;        //# frame rate..
	
	memcpy(msg.deviceId, app_set->sys_info.deviceId, MAX_CHAR_32);
	
	return Msg_Send(irec->qid, (void *)&msg, sizeof(to_rec_msg_t));
}

static int recv_msg(void)
{
	to_main_msg_t msg;
	int size;
	
	//# blocking
	if (Msg_Rsv(irec->qid, REC_MSG_TYPE_TO_MAIN, (void *)&msg, sizeof(to_main_msg_t)) < 0)
		return -1;

	if (msg.cmd == AV_CMD_REC_FLIST) {
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
	app_thr_obj *tObj = &irec->rObj;
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
	
	irec->evt_rec = 0;
	app_leds_rec_ctrl(LED_REC_OFF);
		
	while (!exit)
	{
		cmd = event_wait(tObj);
		/* file ?°ë ˆ?œì—??ê´€ë¦¬í•˜?„ë¡ ë³€ê²?*/
		//if (cmd == APP_CMD_STOP || app_cfg->ste.b.mmc_err || (app_set->rec_info.overwrite == OFF && app_cfg->ste.b.disk_full)) {
		//	continue;
		//}
		
		//# move to file task
		//if(_get_disk_kb_info(ifile, &sz_info) != EFAIL && app_set->rec_info.overwrite==OFF)
		//{
		//	if(sz_info.free < (1024*MB)/KB)
		//	{
		//		break ;
		//	}
		//}
		
		if (cmd == APP_CMD_EXIT) {
			/* send exit command to rec process */
			exit = 1;
			break;
		} else if (cmd == APP_REC_START) {
			send_msg(AV_CMD_REC_START);
			
			irec->evt_rec = 1;
			app_leds_rec_ctrl(LED_REC_ON);
			/* TODO */
		} else if (cmd == APP_CMD_STOP) {
			send_msg(AV_CMD_REC_STOP);
			
			irec->evt_rec = 0;
			app_leds_rec_ctrl(LED_REC_OFF);
			/* TODO */
		}
	}
	
	tObj->active = 0;
	irec->evt_rec = 0;
	
	app_leds_rec_ctrl(LED_REC_OFF);
		
	aprintf("exit\n");
	
	return NULL;
}

static int _is_enable_rec_start()
{	
	//# currently record
	if (irec->evt_rec) {
		eprintf("currently recording...\n");
		return EFAIL;
	}

	/* ì¹´ë©”???´ìƒ ?ëŠ” SD ì¹´ë“œ ?´ìƒ,, ?ëŠ” ?Œì›¨???…ë°?´íŠ¸ */
	if (!app_cfg->en_rec || !app_cfg->ste.b.cap || !app_cfg->ste.b.mmc || 
		app_cfg->ste.b.busy || app_cfg->ste.b.mmc_err || (app_cfg->vid_count == 0)) 
	{
		eprintf("can't record cuz %s %s %s %s\n",
			app_cfg->ste.b.mmc?"":"no MMC!", app_cfg->ste.b.busy?"system busy":"",
			app_cfg->ste.b.cap?"":"no Capture", app_cfg->en_rec?"":"no Codec",
			(app_cfg->vid_count > 0)?"":"no video detect");
		return EFAIL;
	}

	/* overwrite ëª¨ë“œê°€ ?„ë‹ˆë©?SD ì¹´ë“œ ?©ëŸ‰??1GB ?´ìƒ ?¨ì„ ê²½ìš°?ë§Œ ?œìž‘ */
	if (!app_set->rec_info.overwrite && app_file_check_disk_free_space() == EFAIL) {
		eprintf("Bypass start record!\n");
		return EFAIL;
	}

	return SOK;
}

/*****************************************************************************
* @brief    record start/stop function
* @section
*****************************************************************************/
int app_rec_start(void)
{
	int start = 1;
	unsigned long sz;

	//# Check the status of recording.
	if (_is_enable_rec_start() == EFAIL)
		return EFAIL;

	
	/* record ?„ë¡œ?¸ìŠ¤ê°€ ?œìž‘?˜ì? ?Šì? ê²½ìš°... */
	if (!irec->init) {
		OSA_waitMsecs(50);
	}
	
	aprintf("Record Process Start!!\n");
	
	//# Record start if captuer is not zero.
    dev_buzz_ctrl(100, 1);			//# buzz: rec start
	event_send(&irec->sObj, APP_REC_START, 0, 0);
	
	return SOK;
}


/* SD ì¹´ë“œ??ë¬¸ì œë¡??¸í•œ ì¢…ë£Œ. ?±ë“± */
int app_rec_stop(int buzz)
{
	if (irec->evt_rec) {
		if (buzz) dev_buzz_ctrl(100, 2);	//# buzz: rec stop
		event_send(&irec->sObj, APP_CMD_STOP, 0, 0);
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
	return irec->evt_rec;
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
	
	/* start rec process */
	snprintf(cmd, sizeof(cmd), "/opt/fit/bin/av_rec.out %x &", (int)g_mem_get_phyaddr());
	system(cmd);
	
	//#--- create msg receive thread
	tObj = &irec->rObj;
	if(thread_create(tObj, THR_rec_recv_msg, APP_THREAD_PRI, tObj) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	//#--- create msg send thread
	tObj = &irec->sObj;
	if(thread_create(tObj, THR_rec_send_msg, APP_THREAD_PRI, tObj) < 0) {
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
	if (irec->evt_rec)
	{
		tObj = &irec->sObj;
		event_send(tObj, APP_CMD_EXIT, 0, 0);
		while (tObj->active) {
			app_msleep(20);
		}
	}
	thread_delete(tObj);
	
	//#--- stop message receive thread. 
	//# ?„ë¡œ?¸ìŠ¤?ì„œ ?´ë? ì¢…ë£Œê°€ ?˜ë?ë¡?APP_CMD_EXITë¥??˜ë©´ ?ˆë¨.
//	tObj = &irec->rObj;
//	thread_delete(tObj);
	
	aprintf("done!...\n");

	return SOK;
}
