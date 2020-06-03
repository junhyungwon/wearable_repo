/*
 * File : app_ipc.c
 *
 * Copyright (C) 2015 UDWORKs
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include <ipc_common_def.h>

#include <app_main.h>
#include <app_ipc.h>
#include <app_iwscan.h>
#include <app_udev.h>
#include <app_net_manager.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define FXN_IPC_THR_PRI		(FXN_THR_PRI_DEFAULT + 1)

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj *iObj;
	int msgid;

} app_ipc_data_t;

static app_ipc_data_t app_ipc_data;
static app_ipc_data_t *app_ipc_pdata = &app_ipc_data;

/*****************************************************************************
* @brief    ipc thread main function.
*   - desc
*****************************************************************************/
static void *app_ipc_main(void *prm)
{
	app_thr_obj *tObj = (app_thr_obj *)prm;
	ipc_msg_buf_t msgbuf;


	int exit = 0, r;
	int qid;

	printf(" [task] %s start\n", __func__);

	qid = app_ipc_pdata->msgid;
	tObj->active = 1;

	while (!exit)
	{
		//# blocking.
		r = msgrcv(qid, &msgbuf, sizeof(msgbuf) - sizeof(long), IPC_MSG_TYPE_1, 0);
		if (r >= 0) {
			ipc_msg_buf_t *pMsg = &msgbuf;
			switch (pMsg->cmd) {
				case IPC_MSG_WIFI_SCAN:
					dprintf("[scan_ipc] Recv CMD \"IPC_MSG_WIFI_SCAN\"...\n");
					app_iscan_start();
					break;
				case IPC_MSG_WIFI_CLI_START:
					dprintf("[scan_ipc] Recv CMD \"IPC_MSG_WIFI_CLI_START\"...\n");
					app_net_mgr_cli_start();
					break;
				case IPC_MSG_WIFI_CLI_STOP:
					dprintf("[scan_ipc] Recv CMD \"IPC_MSG_WIFI_CLI_STOP\"...\n");
					app_net_mgr_cli_stop();
					break;
				case IPC_MSG_WIFI_WAIT_FOR_AUTH:
					dprintf("[scan_ipc] Recv CMD \"IPC_MSG_WIFI_WAIT_FOR_AUTH\"...\n");
					app_net_mgr_cli_wait_for_auth();
					break;
				case IPC_MSG_WIFI_GET_AUTH_STATUS:
					dprintf("[scan_ipc] Recv CMD \"IPC_MSG_WIFI_GET_AUTH_STATUS\"...\n");
					app_net_mgr_cli_auth_status();
					break;
				case IPC_MSG_WIFI_GET_NET_STATUS:
					dprintf("[scan_ipc] Recv CMD \"IPC_MSG_WIFI_NET_STATUS\"...\n");
					app_net_mgr_cli_net_status();
					break;
				case IPC_MSG_WIFI_GET_IP:
					dprintf("[scan_ipc] Recv CMD \"IPC_MSG_WIFI_GET_IP\"...\n");
					app_net_mgr_cli_get_ipaddr();
					break;
				case IPC_MSG_QUIT:
					exit = 1;
					break;
			default:
				break;
			}
		}
	}
	tObj->active = 0;

	printf(" [app ] %s exit\n", __func__);

	return NULL;
}

/*****************************************************************************
* @brief    initialize for ipc
* @section  [desc]
*****************************************************************************/
int app_ipc_init(void)
{
	struct sched_param schedprm;
	app_thr_obj *tObj;

	pthread_attr_t tattr;
	pthread_mutexattr_t muattr;
  	pthread_condattr_t cond_attr;

	THR_semHndl *pSem;

	int r = -1, qid;

	tObj = (app_thr_obj *)malloc(sizeof(app_thr_obj));
	if (tObj != NULL) {
		/* message queue init */
		qid = msgget((key_t)IPC_MSG_KEY, IPC_CREAT | 0666);
		if (qid < 0) {
			eprintf("msgget failed with error %d\n", qid);
			r = -1;
			goto err_ipc;
		}
		dprintf("create queue id: %d\n", qid);
		app_ipc_pdata->msgid = qid;

		/* mutex and thread create */
		pSem = &tObj->sem;
		pthread_mutexattr_init(&muattr);
  		pthread_condattr_init(&cond_attr);

  		pthread_mutex_init(&pSem->lock, &muattr);
  		pthread_cond_init(&pSem->cond, &cond_attr);

  		pSem->count = 0;
  		pSem->maxCount = MAX_PENDING_SEM_CNT;

		pthread_condattr_destroy(&cond_attr);
 	 	pthread_mutexattr_destroy(&muattr);

		/* initialize thread attributes structure */
	  	pthread_attr_init(&tattr);
	  	pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	  	pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);

	  	schedprm.sched_priority = FXN_IPC_THR_PRI;
	  	pthread_attr_setschedparam(&tattr, &schedprm);

		r = pthread_create(&tObj->thr.hndl, &tattr,
						app_ipc_main, (void *)tObj);
		pthread_attr_destroy(&tattr);

		if (!r) {
			//# resgister thread obj
			app_ipc_pdata->iObj = tObj;
		} else {
			eprintf("Couldn't Create thread [ret = %d]!\n", r);
			r = -1;
			goto err_ipc;
		}
	} else {
		eprintf("Failed to alloc app ipc obj!!\n");
	}

	return 0;

err_ipc:
	if (tObj != NULL) {
		free(tObj);
		app_ipc_pdata->iObj = NULL;
	}

	return FXN_ERR_IPC_INIT;
}

/*****************************************************************************
* @brief    deinitialize for ipc
* @section  [desc]
*****************************************************************************/
int app_ipc_exit(void)
{
	app_thr_obj *tObj;
	THR_semHndl *pSem;
  	void *rVal;
	int r = -1;

  	if (app_ipc_pdata == NULL)
  		return r;

	/* delete usb monitor object */
	tObj = (app_thr_obj *)app_ipc_pdata->iObj;
	if (tObj != NULL) {
		THR_event_send(tObj, APP_CMD_STOP, 0, 0);
		while (tObj->active) {
			THR_waitmsecs(20);
		}

		pSem = &tObj->sem;
		pthread_cond_destroy(&pSem->cond);
	  	pthread_mutex_destroy(&pSem->lock);

	  	r |= pthread_cancel(tObj->thr.hndl);
	  	r |= pthread_join(tObj->thr.hndl, &rVal);

		/* delete message key */
		if (app_ipc_pdata->msgid > 0) {
			msgctl(app_ipc_pdata->msgid, IPC_RMID, 0);
		}

		free(tObj);
		tObj = NULL;
	}

	printf("[%s] done!!!\n", __func__);

	return r;
}
