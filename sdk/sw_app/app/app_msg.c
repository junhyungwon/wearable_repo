/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_msg.c
 * @brief	application message function
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

//# for proc_msg
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "app_comm.h"
#include "app_msg.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    message util function
* @section  DESC Description
*   - modify msg_util.c
*****************************************************************************/
int proc_msg_init(int msg_key)
{
	int qid;
	key_t key = msg_key;

	qid = msgget(key, 0);
	if(qid < 0) {
		qid = msgget(key, IPC_CREAT|0666);
	}

	return qid;
}

int proc_msg_send(int qid, void *pdata, int size)
{
	return msgsnd(qid, pdata, size-sizeof(long), MSG_NOERROR);
}

int proc_msg_recv(int qid, int msg_type, void *pdata, int size)
{
	return msgrcv(qid, pdata, size-sizeof(long), msg_type, MSG_NOERROR);
}

int proc_msg_exit(int qid)
{
	msgctl(qid, IPC_RMID, NULL);

	return 0;
}

/*****************************************************************************
* @brief    event function
* @section  [desc]
*****************************************************************************/
int event_wait(app_thr_obj *tObj)
{
	if(!tObj->active) {
		return EINVALID;
	}

	OSA_semWait(&tObj->sem, OSA_TIMEOUT_FOREVER);

	return tObj->cmd;
}

int event_send(app_thr_obj *tObj, int cmd, int prm0, int prm1)
{
	if(!tObj->active) {
		return EINVALID;
	}

	tObj->cmd = cmd;
	tObj->param0 = prm0;
	tObj->param1 = prm1;
	OSA_semSignal(&tObj->sem);

	return SOK;
}

/*****************************************************************************
* @brief    create/delete thread function
* @section  [desc]
*****************************************************************************/
int thread_create(app_thr_obj *tObj, void *fxn, int pri, void *prm)
{
	int ret;

	if(tObj == NULL)
		return EPARAM;

	ret = OSA_semCreate(&tObj->sem, MAX_PENDING_SEM_CNT, 0);
	if(ret != 0) {
		return EFAIL;
	}

	if(fxn != NULL) {
		ret = OSA_thrCreate(&tObj->thr, fxn, pri, 0, prm);
		if(ret != 0) {
			OSA_semDelete(&tObj->sem);
			return EFAIL;
		}
	}

	return SOK;
}

void thread_delete(app_thr_obj *tObj)
{
	if(tObj == NULL)
		return;

	OSA_semDelete(&tObj->sem);
	if(tObj->thr.hndl != (pthread_t)NULL) {
		OSA_thrDelete(&tObj->thr);
	}

	memset(tObj, 0, sizeof(app_thr_obj));
}
