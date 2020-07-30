/******************************************************************************
 * FITT Board
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_sipc.c
 * @brief   sip protocol client proc
 * @author  
 * @section MODIFY history
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
#include <sys/types.h>
#include <sys/stat.h>

#include "sipc_ipc_cmd_defs.h"
#include "app_comm.h"
#include "app_sipc.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define SIPC_BIN_STR		"/opt/fit/bin/baresip.out"
#define SIPC_CMD_STR		"/opt/fit/bin/baresip.out &"

typedef struct {
	app_thr_obj sObj; /* sip send object */
	app_thr_obj rObj; /* sip receive object */
	
	int init;
	int qid;
	
} app_sipc_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_sipc_t t_sip;
static app_sipc_t *isip = &t_sip;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static int send_msg(int cmd)
{
	to_sipc_msg_t msg;
	
	msg.type = SIPC_MSG_TYPE_TO_CLIENT;
	msg.cmd = cmd;

	//# set param (TODO)
	
	return Msg_Send(isip->qid, (void *)&msg, sizeof(to_sipc_msg_t));
}

static int recv_msg(void)
{
	to_smain_msg_t msg;
	int size;
	
	//# blocking
	if (Msg_Rsv(isip->qid, SIPC_MSG_TYPE_TO_MAIN, (void *)&msg, sizeof(to_smain_msg_t)) < 0)
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
static void *THR_sipc_recv_msg(void *prm)
{
	app_thr_obj *tObj = &isip->rObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	
	//# message queue
	isip->qid = Msg_Init(SIPC_MSG_KEY);
	
	while (!exit) {
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive voip process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case SIPC_CMD_SIP_READY:
			isip->init = 1; /* from record process */
			dprintf("received voip ready!\n");
			break;
		case SIPC_CMD_SIP_EXIT:
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
static void *THR_sipc_send_msg(void *prm)
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
int app_sipc_init(void)
{
	struct stat sb;
	app_thr_obj *tObj;
	FILE *f = NULL;
	
	/* execute baresip */
    if (stat(SIPC_BIN_STR, &sb) != 0) {
		eprintf("can't access baresip execute file!\n");
        return -1;
	}
	system(SIPC_CMD_STR);
	
	//# create recv msg thread.
	tObj = &isip->rObj;
	if (thread_create(tObj, THR_sipc_recv_msg, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create SIP Client Receive Msg thread\n");
		return EFAIL;
    }
	
	//# create send msg thread.
	tObj = &isip->sObj;
	if (thread_create(tObj, THR_sipc_send_msg, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create SIP Client Send Msg thread\n");
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
void app_sipc_exit(void)
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
