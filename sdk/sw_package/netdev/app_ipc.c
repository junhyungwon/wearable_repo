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

#include <netdev_ipc_def.h>

#include <app_main.h>
#include <app_ipc.h>
#include <app_iwscan.h>
#include <app_udev.h>
#include <app_net_manager.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj iObj;
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
	app_thr_obj *tObj = &app_ipc_pdata->iObj;
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
	app_thr_obj *tObj = &app_ipc_pdata->iObj;
	int qid;
	
	/* message queue init */
	qid = msgget((key_t)IPC_MSG_KEY, IPC_CREAT | 0666);
	if (qid < 0) {
		eprintf("msgget failed with error %d\n", qid);
		return FXN_ERR_IPC_INIT;
	}
	
	dprintf("create queue id: %d\n", qid);
	app_ipc_pdata->msgid = qid;
	
	if (thread_create(tObj, app_ipc_main, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return FXN_ERR_IPC_INIT;
	}
		
	return 0;
}

/*****************************************************************************
* @brief    deinitialize for ipc
* @section  [desc]
*****************************************************************************/
int app_ipc_exit(void)
{
	app_thr_obj *tObj = &app_ipc_pdata->iObj;

	/* delete ipc object */
	event_send(tObj, APP_CMD_STOP, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
	/* delete message key */
	if (app_ipc_pdata->msgid > 0) {
		msgctl(app_ipc_pdata->msgid, IPC_RMID, 0);
	}
		
	printf("[%s] done!!!\n", __func__);

	return 0;
}
