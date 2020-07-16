/******************************************************************************
 * UCX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_wifi_client.c
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

#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>

//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <sys/vfs.h>
#include <sys/stat.h>

#include <dev_common.h>

#include "app_comm.h"
#include "app_set.h"
#include "app_ctrl.h"
#include "app_wcli.h"
#include "app_dev.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define WCLI_CYCLE_TIME      	100
#define SCAN_TIME_MAX			20000 //# max 20s
#define SCAN_COUNT_MAX			(SCAN_TIME_MAX / WCLI_CYCLE_TIME)

#define WCONN_CYCLE_TIME		(200)
#define WCONN_TIME_MAX			(5000)	//# 5sec
#define WCONN_COUNT_MAX			(WCONN_TIME_MAX / WCONN_CYCLE_TIME)

#define WCONN_LEVEL_MAX			(30)

#define WCLI_STE_NOT_READY     		(-1)
#define WCLI_STE_IDLE     			(0x00)
#define WCLI_STE_SCANNING     		(0x01)
#define WCLI_STE_WAIT_FOR_START		(0x02) /* wait for Wi-Fi start */
#define WCLI_STE_WAIT_FOR_AUTH		(0x04) /* wait for Wi-Fi authentication */
#define WCLI_STE_GET_AUTH_STATUS	(0x08) /* Wi-Fi authentication status */
#define WCLI_STE_GET_NET_STATUS		(0x10) /* connected network status (signal level) */
#define WCLI_STE_GET_IP				(0x12) /* get ipaddress */
#define WCLI_STE_WAIT_FOR_STOP		(0x14) /* wait for Wi-Fi stop */

typedef struct {
	app_thr_obj cliObj;
	unsigned char *sbuf;  /* shared memory address */
	char *databuf;        /* temporary buffer */

	int msgqid; /* message qid */
	int shmid;  /* shared memory qid */

	int ste;  /* Wi-Fi Client wait reponse state */

} app_wcli_t;

typedef struct {
	app_thr_obj wiObj;
	int ste;	/* Wi-Fi connect thread run state */
	int count;	/* check timmer */
} app_wconn_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_wcli_t t_wcli = {
	.sbuf    = NULL,
	.databuf = NULL,
	.msgqid  = -1,
	.shmid   = -1,
	.ste     = WCLI_STE_NOT_READY,
};

static app_wconn_t t_wconn = {
	.ste	= WCONN_STE_IDLE,
	.count	= 0,
};

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static int iwscan_get_state(void)
{
	FILE *fs = NULL;
	char lbuf[128];

	pid_t pid;

	fs = popen("/bin/pidof netdev.out", "r");
	if (fs == NULL) {
		eprintf("couldn't execute pidof iw_scan!!\n");
		return 0;
	}

	fgets(lbuf, 128, fs);
	pclose(fs);

	pid = strtoul(lbuf, NULL, 10);
	if (pid > 0)
		return 1;

	return 0;
}

/*****************************************************************************
* @brief    Wi-Fi Scanning..
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_wcli_req_scan(iwscan_list_t *plist)
{
	app_wcli_t *iwcli = (app_wcli_t *)&t_wcli;
    app_thr_obj *tObj = (app_thr_obj *)&iwcli->cliObj;

	ipc_msg_buf_t ap_msg;

	if (iwcli->ste == WCLI_STE_NOT_READY) {
		eprintf("Not ready iwscan!!\n");
		return -1;
	}

	/* set to default null */
	iwcli->databuf = NULL;
	if (plist == NULL) {
		eprintf("invalid params!!\n");
		return -1;
	}

	memset(plist, 0, sizeof(iwscan_list_t));
	iwcli->databuf = (char *)plist;

	//# reqeust wifi scan...
	wcli_dbg("Scanning start!!....\n");
	iwcli->ste = WCLI_STE_SCANNING;

	memset(&ap_msg, 0, sizeof(ipc_msg_buf_t));
	ap_msg.des = IPC_MSG_TYPE_1;
	ap_msg.cmd = IPC_MSG_WIFI_SCAN;
	ap_msg.src = IPC_MSG_TYPE_2;

	msgsnd(iwcli->msgqid, &ap_msg, sizeof(ap_msg) - sizeof(long), 0);

	OSA_semWait(&tObj->sem, OSA_TIMEOUT_FOREVER);

#ifdef WCLI_DBG
	//# for debugging
	wcli_dbg("[scan] Scaned ssid -- [%02d] ==\n", plist->num);
	if (plist->num > 0 ) {
		int i;
		for (i = 0; i < plist->num; i++)
			wcli_dbg( " %02d	%s \n", i, plist->info[i].ssid);
	}
#endif

	return 0;
}

/*****************************************************************************
* @brief    Wi-Fi Connect to select AP..
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_wcli_req_link_start(scan_info_t *winfo, char *pwd)
{
	app_wcli_t *iwcli = (app_wcli_t *)&t_wcli;
    app_thr_obj *tObj = (app_thr_obj *)&iwcli->cliObj;

	ipc_msg_buf_t ap_msg;

	if (iwcli->ste == WCLI_STE_NOT_READY) {
		eprintf("Not ready iwscan!!\n");
		return -1;
	}

	/* set to default null */
#if 0
	if (winfo == NULL || pwd == NULL) 
#else // bk 2020.02.26 allow null password
	if (winfo == NULL)
#endif
	{
		eprintf("invalid params!!\n");
		return -1;
	}
	strcpy(winfo->pwd, pwd);

	wcli_dbg("Connecting to ap....\n");
	iwcli->ste = WCLI_STE_WAIT_FOR_START;

	memcpy((iwcli->sbuf + 1), winfo, sizeof(scan_info_t));

	//# reqeust wifi connect...
	memset(&ap_msg, 0, sizeof(ipc_msg_buf_t));
	ap_msg.des = IPC_MSG_TYPE_1;
	ap_msg.cmd = IPC_MSG_WIFI_CLI_START;
	ap_msg.src = IPC_MSG_TYPE_2;

	msgsnd(iwcli->msgqid, &ap_msg, sizeof(ap_msg) - sizeof(long), 0);
	OSA_semWait(&tObj->sem, OSA_TIMEOUT_FOREVER);

	return 0;
}

/*****************************************************************************
* @brief    Wi-Fi disconnect to AP..
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_wcli_req_link_stop(void)
{
	app_wcli_t *iwcli = (app_wcli_t *)&t_wcli;
	app_thr_obj *tObj = (app_thr_obj *)&iwcli->cliObj;

	ipc_msg_buf_t ap_msg;

	if (iwcli->ste == WCLI_STE_NOT_READY) {
		eprintf("Not ready iwscan!!\n");
		return -1;
	}

	wcli_dbg("disonnect to ap....\n");
	iwcli->ste = WCLI_STE_WAIT_FOR_STOP;

	//# reqeust wifi disconnet...
	memset(&ap_msg, 0, sizeof(ipc_msg_buf_t));
	ap_msg.des = IPC_MSG_TYPE_1;
	ap_msg.cmd = IPC_MSG_WIFI_CLI_STOP;
	ap_msg.src = IPC_MSG_TYPE_2;

	msgsnd(iwcli->msgqid, &ap_msg, sizeof(ap_msg) - sizeof(long), 0);
	OSA_semWait(&tObj->sem, OSA_TIMEOUT_FOREVER);

	return 0;
}

/*****************************************************************************
* @brief    Wi-Fi wait authentiction (Max 10s)
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_wcli_req_wait_for_auth(void)
{
	app_wcli_t *iwcli = (app_wcli_t *)&t_wcli;
    app_thr_obj *tObj = (app_thr_obj *)&iwcli->cliObj;

	ipc_msg_buf_t ap_msg;
	int r = -1;

	if (iwcli->ste == WCLI_STE_NOT_READY) {
		eprintf("Not ready iwscan!!\n");
		return r;
	}

	wcli_dbg("wait for auth connection....!!!\n");
	iwcli->ste = WCLI_STE_WAIT_FOR_AUTH;

	memset(&ap_msg, 0, sizeof(ipc_msg_buf_t));
	ap_msg.des = IPC_MSG_TYPE_1;
	ap_msg.cmd = IPC_MSG_WIFI_WAIT_FOR_AUTH;
	ap_msg.src = IPC_MSG_TYPE_2;

	msgsnd(iwcli->msgqid, &ap_msg, sizeof(ap_msg) - sizeof(long), 0);
	OSA_semWait(&tObj->sem, OSA_TIMEOUT_FOREVER);

	if (tObj->param0 == 1) r = 0;  /* success */
	else 				   r = -1; /* failure */

	return r;
}

/*****************************************************************************
* @brief    Wi-Fi wpa_supplicant authentiction status(success or failure)
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_wcli_req_auth_status(void)
{
	app_wcli_t *iwcli = (app_wcli_t *)&t_wcli;
    app_thr_obj *tObj = (app_thr_obj *)&iwcli->cliObj;

	ipc_msg_buf_t ap_msg;
	int r = -1;

	if (iwcli->ste == WCLI_STE_NOT_READY) {
		eprintf("Not ready iwscan!!\n");
		return r;
	}

	wcli_dbg("wait for auth status....!!!\n");
	iwcli->ste = WCLI_STE_GET_AUTH_STATUS;

	memset(&ap_msg, 0, sizeof(ipc_msg_buf_t));
	ap_msg.des = IPC_MSG_TYPE_1;
	ap_msg.cmd = IPC_MSG_WIFI_GET_AUTH_STATUS;
	ap_msg.src = IPC_MSG_TYPE_2;

	msgsnd(iwcli->msgqid, &ap_msg, sizeof(ap_msg) - sizeof(long), 0);
	OSA_semWait(&tObj->sem, OSA_TIMEOUT_FOREVER);

	if (tObj->param0 == 1) r = 0;  /* success */
	else 				   r = -1; /* failure */

	return r;
}

/*****************************************************************************
* @brief    Wi-Fi get network status for AP..
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_wcli_req_net_status(void)
{
	app_wcli_t *iwcli = (app_wcli_t *)&t_wcli;
    app_thr_obj *tObj = (app_thr_obj *)&iwcli->cliObj;

	ipc_msg_buf_t ap_msg;
	int level = 0;

	if (iwcli->ste == WCLI_STE_NOT_READY) {
		eprintf("Not ready iwscan!!\n");
		return -1;
	}

	wcli_dbg("request Wi-Fi network signal level....\n");
	iwcli->ste = WCLI_STE_GET_NET_STATUS;

	//# reqeust wifi disconnet...
	memset(&ap_msg, 0, sizeof(ipc_msg_buf_t));
	ap_msg.des = IPC_MSG_TYPE_1;
	ap_msg.cmd = IPC_MSG_WIFI_GET_NET_STATUS;
	ap_msg.src = IPC_MSG_TYPE_2;

	msgsnd(iwcli->msgqid, &ap_msg, sizeof(ap_msg) - sizeof(long), 0);
	OSA_semWait(&tObj->sem, OSA_TIMEOUT_FOREVER);

#ifdef WCLI_DBG
	level = (int)(*(iwcli->sbuf + 1) & 0xff);
	wcli_dbg("current Wi-Fi link signal level = %d\n", level);
#endif

	return level;
}

/*****************************************************************************
* @brief    Wi-Fi get ip from AP..
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_wcli_req_get_ipaddr(void)
{
	app_wcli_t *iwcli = (app_wcli_t *)&t_wcli;
    app_thr_obj *tObj = (app_thr_obj *)&iwcli->cliObj;

	ipc_msg_buf_t ap_msg;

	if (iwcli->ste == WCLI_STE_NOT_READY) {
		eprintf("Not ready iwscan!!\n");
		return -1;
	}

	wcli_dbg("[WCLI] Run UDHCPC....\n");
	iwcli->ste = WCLI_STE_GET_IP;

	memset(&ap_msg, 0, sizeof(ipc_msg_buf_t));
	ap_msg.des = IPC_MSG_TYPE_1;
	ap_msg.cmd = IPC_MSG_WIFI_GET_IP;
	ap_msg.src = IPC_MSG_TYPE_2;

	msgsnd(iwcli->msgqid, &ap_msg, sizeof(ap_msg) - sizeof(long), 0);
	OSA_semWait(&tObj->sem, OSA_TIMEOUT_FOREVER);

	return 0;
}

/*****************************************************************************
* @brief    Wi-Fi Client monitor main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_wcli(void *prm)
{
	app_wcli_t *iwcli = (app_wcli_t *)&t_wcli;
	app_thr_obj *tObj = &iwcli->cliObj;

	int cmd, ste;
	int exit = 0;
	int count;

	if (iwscan_get_state() == 0) {
		eprintf("Not running iwscan process!!\n");
		return NULL;
	}

	//# get shared memory id
	iwcli->shmid = shmget((key_t)SHM_KEY, 0, 0);
	if (iwcli->shmid == -1) {
		eprintf("shared memory for wifi scan is not created!!\n");
		goto thr_exit;
	}

	//# get shared memory
	iwcli->sbuf = (unsigned char *)shmat(iwcli->shmid, NULL, 0);
	if (iwcli->sbuf == NULL) {
		eprintf("shared memory for wifi scan is NULL!!!");
		goto thr_exit;
	}

	iwcli->msgqid = msgget(IPC_MSG_KEY, 0666);
	if (iwcli->msgqid < 0) {
		eprintf("IPC Message failed!!!\n");
		goto thr_exit;
	}

	iwcli->ste = WCLI_STE_IDLE;
	count = 0;

	aprintf("enter...\n");
	tObj->active = 1;

	while (!exit)
	{
		//# wait cmd
		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP)
            break;

		ste = iwcli->ste;
		switch (ste) {
		case WCLI_STE_SCANNING:
		{
			if (count >= SCAN_COUNT_MAX) {
				eprintf("failed to scanning..(count = %d)\n", count);

				iwcli->ste = WCLI_STE_IDLE;
				OSA_semSignal(&tObj->sem);
			} else {
				count++;
				/* checking shared memory [0] */
				if (iwcli->sbuf[0] == WIFI_REQUEST_DONE) {
					iwcli->ste = WCLI_STE_IDLE;
					iwcli->sbuf[0] = 0;

					if (iwcli->databuf == NULL) {
						eprintf("Failed to copy Wi-Fi List(buffer is null)\n");
					} else {
						memcpy((iwscan_list_t *)iwcli->databuf, (iwscan_list_t *)(iwcli->sbuf + 1),
									sizeof(iwscan_list_t));
					}

					tObj->param0 = 1; /* scanning succeed */
					OSA_semSignal(&tObj->sem);
				}
			}
			break;
		}
		case WCLI_STE_WAIT_FOR_START:
		{
			if (count >= SCAN_COUNT_MAX) {
				eprintf("failed to link start..(count = %d)\n", count);

				iwcli->ste = WCLI_STE_IDLE;
				OSA_semSignal(&tObj->sem);
			} else {
				count++;
				/* checking shared memory [0] */
				if (iwcli->sbuf[0] == WIFI_REQUEST_DONE) {
					iwcli->sbuf[0] = 0;
					iwcli->ste = WCLI_STE_IDLE;
					tObj->param0 = 1;
					OSA_semSignal(&tObj->sem);
				}
			}
			break;
		}
		case WCLI_STE_WAIT_FOR_AUTH:
		{
			if (count >= SCAN_COUNT_MAX) {
				eprintf("failed to wait for auth..(count = %d)\n", count);

				iwcli->ste = WCLI_STE_IDLE;
				OSA_semSignal(&tObj->sem);
			} else {
				count++;
				/* checking shared memory [0] */
				if (iwcli->sbuf[0] == WIFI_REQUEST_DONE) {
					//#printf("[WCLI] AP wait for authentication done!!\n");
					iwcli->sbuf[0] = 0;

					if (iwcli->sbuf[1] > 0)
						tObj->param0 = 1; /* authentication succeed */
					else
						tObj->param0 = 0; /* authentication failed */

					iwcli->ste = WCLI_STE_IDLE;
					OSA_semSignal(&tObj->sem);
				} else{
					/* auth progress... */
				}
			}
			break;
		}
		case WCLI_STE_GET_AUTH_STATUS:
		{
			if (count >= SCAN_COUNT_MAX) {
				eprintf("failed to auth..(count = %d)\n", count);

				iwcli->ste = WCLI_STE_IDLE;
				OSA_semSignal(&tObj->sem);
			} else {
				count++;
				/* checking shared memory [0] */
				if (iwcli->sbuf[0] == WIFI_REQUEST_DONE) {
					//#printf("[WCLI] AP wait for authentication done!!\n");
					iwcli->sbuf[0] = 0;

					if (iwcli->sbuf[1] > 0)
						tObj->param0 = 1; /* authentication succeed */
					else
						tObj->param0 = 0; /* authentication failed */

					iwcli->ste = WCLI_STE_IDLE;
					OSA_semSignal(&tObj->sem);
				} else{
					/* auth progress... */
				}
			}
			break;
		}
		case WCLI_STE_GET_IP:
		{
			if (count >= SCAN_COUNT_MAX) {
				eprintf("failed to get IP..(count = %d)\n", count);

				iwcli->ste = WCLI_STE_IDLE;
				OSA_semSignal(&tObj->sem);
			} else {
				count++;
				/* checking shared memory [0] */
				if (iwcli->sbuf[0] == WIFI_REQUEST_DONE) {
					//#printf("[WCLI] AP wait for authentication done!!\n");
					iwcli->sbuf[0] = 0;
					iwcli->ste = WCLI_STE_IDLE;
					tObj->param0 = 1;
					OSA_semSignal(&tObj->sem);
				} else {
					/* wating response */
				}
			}
			break;
		}
		case WCLI_STE_GET_NET_STATUS:
		{
			if (count >= SCAN_COUNT_MAX) {
				eprintf("failed to get status..(count = %d)\n", count);

				iwcli->ste = WCLI_STE_IDLE;
				OSA_semSignal(&tObj->sem);
			} else {
				count++;
				/* checking shared memory [0] */
				if (iwcli->sbuf[0] == WIFI_REQUEST_DONE) {
					iwcli->sbuf[0] = 0;

					iwcli->ste = WCLI_STE_IDLE;
					OSA_semSignal(&tObj->sem);
				}
			}
			break;
		}
		case WCLI_STE_WAIT_FOR_STOP:
		{
			if (count >= SCAN_COUNT_MAX) {
				eprintf("failed to scanning..(count = %d)\n", count);

				iwcli->ste = WCLI_STE_IDLE;
				OSA_semSignal(&tObj->sem);
			} else {
				count++;
				/* checking shared memory [0] */
				if (iwcli->sbuf[0] == WIFI_REQUEST_DONE) {
					iwcli->sbuf[0] = 0;
					iwcli->ste = WCLI_STE_IDLE;
					tObj->param0 = 1;
					OSA_semSignal(&tObj->sem);
				}
			}
			break;
		}
		case WCLI_STE_IDLE:
		default:
			/* reset scanning counter */
			count = 0;
			tObj->param0 = 0;
			break;
		}

        app_msleep(WCLI_CYCLE_TIME);
    }

    tObj->active = 0;
	aprintf("...exit\n");

thr_exit:
	if (iwcli->msgqid >= 0) {
		/* kill message queue. */
//		msgctl(iwcli->msgqid, IPC_RMID, NULL); //# don't execute kill message
		iwcli->msgqid = -1;
	}

	/* detach shared memory */
	if (iwcli->sbuf != NULL) {
//		shmdt(iwcli->sbuf);
		iwcli->sbuf = NULL;
	}

	return NULL;
}

/*****************************************************************************
* @brief    Wi-Fi Client thread start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_wcli_start(void)
{
	app_thr_obj *tObj;

	//# create Wi-Fi Client thread
	tObj = &t_wcli.cliObj;
	if (thread_create(tObj, THR_wcli, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create Wi-Fi Client thread\n");
		return EFAIL;
    }

	aprintf("... done!\n");

    return 0;
}

void app_wcli_stop(void)
{
	//# Stop Wi-Fi Client thread
	app_wcli_t *iwcli = (app_wcli_t *)&t_wcli;
    app_thr_obj *tObj = &iwcli->cliObj;

    /* delete Wi-Fi Client object */
   	event_send(tObj, APP_CMD_STOP, 0, 0);
	while(tObj->active)
		app_msleep(20);

	thread_delete(tObj);

    dprintf("... done!\n");
}

//////////////////////////////
//
/** Wi-Fi Connect Check		**/
//
//////////////////////////////
static scan_info_t winfo;

static int wconn_connect_wifi(void)
{
//	if (strcmp(app_set->wifiap.ssid, CHAR_MEMSET) == 0)
    if((int)app_set->wifiap.ssid[0] == CHAR_INVALID)
		return -1;

	winfo.en_key = app_set->wifiap.en_key;
	strcpy(winfo.ssid, app_set->wifiap.ssid);

	if (strlen(app_set->wifiap.pwd) == 0)
	{
		winfo.en_key = 0;
		memset(winfo.pwd, 0, sizeof(winfo.pwd));
	}
	else
	{
		strcpy(winfo.pwd, app_set->wifiap.pwd);
	}

	wcli_dbg("%s: en_key[%d], ssid[%s] pwd[%s]=====\n", __func__, winfo.en_key, winfo.ssid, app_set->wifiap.pwd);
	app_wcli_req_link_start(&winfo, app_set->wifiap.pwd);
	app_msleep(500);

	return 0;
}

/*****************************************************************************
* @brief    Wi-Fi Connect monitor main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_wconn(void *prm)
{
	app_wconn_t *iwconn = (app_wconn_t *)&t_wconn;
	app_wcli_t *iwcli 	= (app_wcli_t *)&t_wcli;
	app_thr_obj *tObj 	= (app_thr_obj *)&iwconn->wiObj;

	int cmd, level;
	int exit = 0;

	iwconn->ste 	= WCONN_STE_IDLE;
	iwconn->count 	= 0;

	//# wait for IDLE state of wcli thread...
	while (iwcli->ste != WCLI_STE_IDLE)
		app_msleep(20);

	//# Connect check wifi ap...
	if (app_cfg->ste.b.wifi_run) {
		if (wconn_connect_wifi() == 0) {
			iwconn->ste = WCONN_STE_CHECKING;	//# wifi ap connected done...
			wcli_dbg("wconn_connect_wifi Success.... connect checking start.....!!\n");
		}
	}

	aprintf("enter...\n");
	tObj->active = 1;

	//# ap connect monitor start
	while (!exit)
	{
		//# wait cmd
		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP)
            break;

		if (app_cfg->ste.b.wifi_ready)
		{
			if (app_cfg->ste.b.wifi_run)
			{
				if (iwconn->ste == WCONN_STE_START && app_cfg->ste.b.cap)
				{
					if (wconn_connect_wifi() == 0) {
						iwconn->ste = WCONN_STE_CHECKING;	//# wifi ap connected done...
						wcli_dbg(" wconn_connect_wifi Success.... connect checking start.....!!\n");
					}
				}
				else if (iwconn->ste == WCONN_STE_CHECKING && app_cfg->ste.b.cap)
				{
					if (iwconn->count != 0 && (iwconn->count %= WCONN_COUNT_MAX) == 0)
					{
						/* Get Wi-Fi connection status (authentication status) */
						if (app_wcli_req_auth_status() == OSA_SOK)
	                    {
							/* success <--> AP, but unknown ip address */
							if (!app_cfg->ste.b.wifi)
	                        {
	                        	 //# Set STATIC IP "wlan0"
	                            if (util_set_net_info(DEV_NET_NAME_WLAN) == 0)
									app_cfg->ste.b.wifi = 1; /* set ipaddress succeed */
								else
									app_cfg->ste.b.wifi = 0; /* retry set ipaddress */
							}
	                        else
	                        {
								#if 1
								/* get Wi-Fi signal level */
								level = app_wcli_req_net_status();
								if (level > 0 && level <= WCONN_LEVEL_MAX /* 50 */)
	                            {
									printf(" weak signal = %d\n",level) ;
									/* TODO : weak signal, force connection terminate.. */
								}
								#endif
							}
						}
	                    else  // tethering off or ap off  --> rf led change to red led
	                    {
							app_leds_rf_ctrl(LED_RF_FAIL) ;
							app_cfg->ste.b.wifi = 0; /* retry set ipaddress */
							/* disconnected AP */
	                    }
					}
					iwconn->count++;
				}
			}
		} else {
			/* if unplug Wi-Fi usb, required stop request..*/
			if (app_cfg->ste.b.wifi_run) {
				app_cfg->ste.b.wifi_run = 0;
				fprintf(stderr, "wcli request stop....\n");
				app_wcli_req_link_stop();

				if (app_cfg->ste.b.wifi)
					app_cfg->ste.b.wifi = 0;
			}

			if (iwconn->ste != WCONN_STE_IDLE) {
				iwconn->ste = WCONN_STE_IDLE;
				iwconn->count = 0;
			}
		}

		app_msleep(WCONN_CYCLE_TIME);
	}

    tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

int app_wconn_start(void)
{
	app_thr_obj *tObj = &t_wconn.wiObj;

	//# create Wi-Fi connect check thread.
	if (thread_create(tObj, THR_wconn, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create Wi-Fi Check thread\n");
		return EFAIL;
    }

    aprintf("... done!\n");

    return 0;
}

void app_wconn_stop(void)
{
	//# Stop Wi-Fi Check thread.
	app_wconn_t *iwconn = (app_wconn_t *)&t_wconn;
    app_thr_obj *tObj   = (app_thr_obj *)&iwconn->wiObj;

	/* delete Wi-Fi Connect Check object */
   	event_send(tObj, APP_CMD_STOP, 0, 0);
	while(tObj->active)
    	app_msleep(20);
	thread_delete(tObj);

	dprintf("... done!\n");
}

void app_wconn_check_state(int ste)
{
	app_wconn_t *iwconn = (app_wconn_t *)&t_wconn;

	iwconn->count 	= 0;
	iwconn->ste 	= WCONN_STE_IDLE;

//	if (strcmp(app_set->wifiap.ssid, CHAR_MEMSET) != 0)
    if((int)app_set->wifiap.ssid[0] != CHAR_INVALID)
		iwconn->ste = ste;
}
