/******************************************************************************
 * UCX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_voip.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2015.10.19 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <baresip_ipc_cmd_defs.h>

#include "app_comm.h"
#include "app_voip.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

typedef struct {
	app_thr_obj sObj; /* sip send object */
	app_thr_obj rObj; /* sip receive object */
	
	int init;
	int qid;
	
} app_sip_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_sip_t t_sip;
static app_sip_t *isip = &t_sip;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static int send_msg(int cmd)
{
	to_sip_msg_t msg;
	
	msg.type = SIP_MSG_TYPE_TO_SIP;
	msg.cmd = cmd;

	//# set param (TODO)
	
	return Msg_Send(isip->qid, (void *)&msg, sizeof(to_sip_msg_t));
}

static int recv_msg(void)
{
	to_smain_msg_t msg;
	int size;
	
	//# blocking
	if (Msg_Rsv(isip->qid, SIP_MSG_TYPE_TO_SMAIN, (void *)&msg, sizeof(to_smain_msg_t)) < 0)
		return -1;

	//if (msg.cmd == AV_CMD_REC_FLIST) {
	//}

	return msg.cmd;
}

/*****************************************************************************
* @brief    voip main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_voip_recv_msg(void *prm)
{
	app_thr_obj *tObj = &isip->rObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	
	//# message queue
	isip->qid = Msg_Init(SIP_MSG_KEY);
	
	while (!exit) {
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive voip process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case AV_CMD_SIP_READY:
			isip->init = 1; /* from record process */
			dprintf("received voip ready!\n");
			break;
		case AV_CMD_SIP_EXIT:
			exit = 1;
			dprintf("received voip exit!\n");
			break;
		default:
			break;	
		}
	}
	
	Msg_Kill(isip->qid);
	
	aprintf("exit...\n");

	return NULL;
}

/*****************************************************************************
* @brief    
* @section  [prm] active channel
*****************************************************************************/
static void *THR_voip_send_msg(void *prm)
{
	app_thr_obj *tObj = &isip->sObj;
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
		} 
		
		/* TODO */
	}
	
	tObj->active = 0;
	aprintf("exit\n");
	
	return NULL;
}

/*****************************************************************************
* @brief    voip manager
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_voip_init(void)
{
	app_thr_obj *tObj;
	FILE *f = NULL;
	
	/* execute baresip */
	f = popen("/opt/fitt/bin/baresip &", "r");
	if (f != NULL) {
		pclose(f); 
	}
	
	//# create recv msg thread.
	tObj = &isip->rObj;
	if (thread_create(tObj, THR_voip_recv_msg, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create Voip Receive Msg thread\n");
		return EFAIL;
    }
	
	//# create send msg thread.
	tObj = &isip->sObj;
	if (thread_create(tObj, THR_voip_send_msg, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create Voip Send Msg thread\n");
		return EFAIL;
    }

    aprintf("... done!\n");

    return 0;
}

/*****************************************************************************
* @brief    voip manager
* @section  DESC Description
*   - desc
*****************************************************************************/
void app_voip_exit(void)
{
	app_thr_obj *tObj;

	/* delete object */
   	tObj = &isip->sObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
    	app_msleep(20);
	thread_delete(tObj);
	
	//#--- stop message receive thread. 
	//# 프로세스에서 이미 종료가 되므로 APP_CMD_EXIT를 하면 안됨.
//	tObj = &isip->rObj;
//	thread_delete(tObj);

	dprintf("... done!\n");
}
