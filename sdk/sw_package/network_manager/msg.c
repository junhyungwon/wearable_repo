/*
 * File : msg.c
 *
 * Copyright (C) 2020 LF
 *
 */
/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "msg.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define OSA_THR_STACK_SIZE_DEFAULT      0

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

///------------------------------------- implements OSA(OS Abstration Layer) -----------------------------------------------------//
static int OSA_semCreate(OSA_SemHndl *hndl, Uint32 maxCount, Uint32 initVal)
{
	pthread_mutexattr_t mutex_attr;
	pthread_condattr_t cond_attr;
	int status = OSA_SOK;

	status |= pthread_mutexattr_init(&mutex_attr);
	status |= pthread_condattr_init(&cond_attr);  

	status |= pthread_mutex_init(&hndl->lock, &mutex_attr);
	status |= pthread_cond_init(&hndl->cond, &cond_attr);  

	hndl->count = initVal;
	hndl->maxCount = maxCount;

	if (hndl->maxCount == 0)  
		hndl->maxCount=1;

	if (hndl->count>hndl->maxCount)
		hndl->count = hndl->maxCount;

	if (status!=OSA_SOK)
		printf("OSA_semCreate() = %d\r\n", status);

	pthread_condattr_destroy(&cond_attr);
	pthread_mutexattr_destroy(&mutex_attr);

	return status;
}

static int OSA_semWait(OSA_SemHndl *hndl, Uint32 timeout)
{
	int status = OSA_EFAIL;

	pthread_mutex_lock(&hndl->lock);

	while (1) {
		if (hndl->count > 0) {
			hndl->count--;
			status = OSA_SOK;
			break;
		} else {
			if (timeout == OSA_TIMEOUT_NONE)
				break;

			pthread_cond_wait(&hndl->cond, &hndl->lock);
		}
	}

	pthread_mutex_unlock(&hndl->lock);

	return status;
}

static int OSA_semSignal(OSA_SemHndl *hndl)
{
	int status = OSA_SOK;

	pthread_mutex_lock(&hndl->lock);

	if (hndl->count<hndl->maxCount) {
		hndl->count++;
		status |= pthread_cond_signal(&hndl->cond);
	}

	pthread_mutex_unlock(&hndl->lock);

	return status;
}

static int OSA_semDelete(OSA_SemHndl *hndl)
{
	pthread_cond_destroy(&hndl->cond);
	pthread_mutex_destroy(&hndl->lock);  

	return OSA_SOK;
}

static int OSA_thrCreate(OSA_ThrHndl *hndl, OSA_ThrEntryFunc entryFunc, Uint32 pri, 
				Uint32 stackSize, void *prm)
{
	pthread_attr_t thread_attr;
	struct sched_param schedprm;
	int status = OSA_SOK;
	
	// initialize thread attributes structure
	status = pthread_attr_init(&thread_attr);
	if (status != OSA_SOK) {
		printf("OSA_thrCreate() - Could not initialize thread attributes\n");
		return status;
	}

	if (stackSize != OSA_THR_STACK_SIZE_DEFAULT)
		pthread_attr_setstacksize(&thread_attr, stackSize);

	status |= pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
	status |= pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO);

	if (pri > OSA_THR_PRI_MAX)   
		pri = OSA_THR_PRI_MAX;
	else if (pri < OSA_THR_PRI_MIN)   
		pri = OSA_THR_PRI_MIN;

	schedprm.sched_priority = pri;
	status |= pthread_attr_setschedparam(&thread_attr, &schedprm);

	if (status != OSA_SOK) {
		printf("OSA_thrCreate() - Could not initialize thread attributes\n");
		goto error_exit;
	}

	status = pthread_create(&hndl->hndl, &thread_attr, entryFunc, prm);
	if (status != OSA_SOK) {
		printf("OSA_thrCreate() - Could not create thread [%d]\n", status);
	}

error_exit:  
	pthread_attr_destroy(&thread_attr);

	return status;
}

static int OSA_thrJoin(OSA_ThrHndl *hndl)
{
	void *returnVal;
	int status = OSA_SOK;

	status = pthread_join(hndl->hndl, &returnVal); 
  
  	return status;    
}

static int OSA_thrDelete(OSA_ThrHndl *hndl)
{
	int status = OSA_SOK;

	status |= pthread_cancel(hndl->hndl); 
	status |= OSA_thrJoin(hndl);

	return status;    
}

static int OSA_thrExit(void *returnVal)
{
	pthread_exit(returnVal);
	
	return OSA_SOK;
}

//-------------------------------- End of OS Abstration Layer ------------------------------------------------
/*****************************************************************************
* @brief    OSA_waitMsecs
* @section  [desc]
*****************************************************************************/
void delay_msecs(unsigned int msecs)
{
	struct timespec delayTime, elaspedTime;

	delayTime.tv_sec  = msecs/1000;
	delayTime.tv_nsec = (msecs%1000)*1000000;

	nanosleep(&delayTime, &elaspedTime);
}

/*****************************************************************************
* @brief    wait for thread event.
* @section  [desc]
*****************************************************************************/
int event_wait(app_thr_obj *tObj)
{
	if (!tObj->active)
		return EFAIL;

	OSA_semWait(&tObj->sem, OSA_TIMEOUT_FOREVER);

	return tObj->cmd;
}

/*****************************************************************************
* @brief    event send to thread.
* @section  [desc]
*****************************************************************************/
int event_send(app_thr_obj *tObj, int cmd, int prm0, int prm1)
{
	if (tObj == NULL || !tObj->active) {
		eprintf("event send fail!!(%x)...........\n", cmd);
		return EFAIL;
	}
	
	tObj->cmd = cmd;
	tObj->param0 = prm0;
	tObj->param1 = prm1;
	
	OSA_semSignal(&tObj->sem);

	return 0;
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

/**
* @brief Initialize message queue.

* Initialize message queue.
* @note This API must be used before use any other message driver API.
* @param msgKey [I ] Key number for message queue and share memory.
* @return message ID.
*/
int Msg_Init(int msgKey)
{
	int qid;
	key_t key = msgKey;

	qid = msgget(key,0);
	if (qid < 0) {
		qid = msgget(key,IPC_CREAT|0666);
		printf("Creat queue id:%d\n", qid);
	}

	printf("queue id:%d\n", qid);

	return qid;
}

/**
* @brief Send message .

* Excute send message command.
* @param qid [I ] Message ID.
* @param *pdata [I ] pointer to the data.
* @param size [I ] Data size.
* @return send data to message queue.
*/
int Msg_Send(int qid, void *pdata, int size)
{
	return msgsnd(qid, pdata, size - sizeof(long), 0);///< send msg1
}

/**
* @brief Receive message .

* Excute receive message command.
* @param qid [I ] Message ID.
* @param msg_type [I ] Message type.
* @param *pdata [I ] pointer to the data.
* @param size [I ] Data size.
* @return receive data from message queue.
*/
int Msg_Rsv( int qid, int msg_type, void *pdata , int size )
{
	return msgrcv( qid, pdata, size-sizeof(long), msg_type, 0);
}
/**
* @brief Send and receive message .

* Excute send and receive message command.
* @param qid [I ] Message ID.
* @param msg_type [I ] Message type.
* @param *pdata [I ] pointer to the data.
* @param size [I ] Data size.
* @return receive data from message queue if send success.
*/
int Msg_Send_Rsv( int qid, int msg_type, void *pdata , int size )
{
	int ret = 0;
	
	ret = msgsnd( qid, pdata, size-sizeof(long), 0 );/* send msg1 */
	if (ret < 0) {
		return ret;
	}
	
	return msgrcv( qid, pdata, size-sizeof(long), msg_type, 0);
}

/**
* @brief Kill message queue.

* Kill message queue.
* @param qid [I ] Message ID.
* @retval 0 Queue killed.
*/
int Msg_Kill(int qid)
{
	msgctl(qid, IPC_RMID, NULL);

	printf("Kill queue id:%d\n", qid);

	return 0;
}
