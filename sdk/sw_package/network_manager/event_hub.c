/*
 * File : event_hub.c
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
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "netmgr_ipc_cmd_defs.h"
#include "event_hub.h"
#include "rndis.h"
#include "common.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj rObj; /* device detect */
	app_thr_obj sObj; /* device detect */
	
	int qid;
	
} netmgr_event_hub_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_event_hub_t t_proc;
static netmgr_event_hub_t *ievt = &t_proc;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 message send/recv function
-----------------------------------------------------------------------------*/
static int send_msg(int cmd, int prm1, int prm2)
{
	to_netmgr_main_msg_t msg;
	
	msg.type = NETMGR_MSG_TYPE_TO_MAIN;
	msg.cmd = cmd;
	
	msg.dev_type = prm1;
	msg.dev_status = prm2;
	msg.wlan_5G_enable = 0;
	
	if (prm1 == NETMGR_DEV_TYPE_WIFI) {
		if ((app_cfg->wlan_vid == RTL_8821A_VID) && 
			(app_cfg->wlan_pid == RTL_8821A_PID))
		{
			msg.wlan_5G_enable = 1;		
		}
	}
	
	return Msg_Send(ievt->qid, (void *)&msg, sizeof(to_netmgr_main_msg_t));
}

static int recv_msg(void)
{
	to_netmgr_msg_t msg;
	int size;
	
	//# blocking
	if (Msg_Rsv(ievt->qid, NETMGR_MSG_TYPE_TO_NETMGR, (void *)&msg, sizeof(to_netmgr_msg_t)) < 0) {
		return -1;
	}
	
	return msg.cmd;
}

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_event_hub_main(void *prm)
{
	app_thr_obj *tObj = &ievt->sObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
        if (cmd == APP_CMD_EXIT)
            break;
		
		switch (cmd) {
		case APP_KEY_UP:
			send_msg(NETMGR_CMD_DEV_DETECT, tObj->param0, tObj->param1);
			break;
		}
	} 
	
	tObj->active = 0;
	aprintf("exit...\n");
	
	return NULL;
}

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_event_hub_poll(void *prm)
{
	app_thr_obj *tObj = &ievt->rObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	//# communication main process ---------------
	ievt->qid = Msg_Init(NETMGR_MSG_KEY);
	send_msg(NETMGR_CMD_READY, 0, 0);
	
	while (!exit)
	{
		cmd = recv_msg();
		if (cmd < 0) {
			dprintf("invalid cmd %d\n", cmd);
			continue;
		}
		
		dprintf("[netmgr process] receive cmd 0x%x\n", cmd);
		
		switch (cmd) {
		case NETMGR_CMD_WLAN_SOFTAP_START:
			netmgr_wlan_hostapd_start();
			break;
		
		case NETMGR_CMD_WLAN_SOFTAP_STOP:
			netmgr_wlan_hostapd_stop();
			break;
			
		case NETMGR_CMD_WLAN_CLIENT_START:
			netmgr_wlan_cli_start();
			break;
		
		case NETMGR_CMD_WLAN_CLIENT_STOP:
			netmgr_wlan_cli_stop();
			break;	
			
		case NETMGR_CMD_RNDIS_START:
			netmgr_rndis_event_start();
			break;
		
		case NETMGR_CMD_RNDIS_STOP:
			netmgr_rndis_event_stop();
			break;	
		
		case NETMGR_CMD_USB2ETH_START:
			netmgr_usb2eth_event_start();
			break;
		
		case NETMGR_CMD_USB2ETH_STOP:
			netmgr_usb2eth_event_stop();
			break;	
				
		case NETMGR_CMD_PROG_START:
			/* 각 쓰레드 시작 */
			event_send(&app_cfg->mObj, APP_CMD_START, 0, 0);
			break;
			
		case NETMGR_CMD_PROG_EXIT:
			event_send(&app_cfg->mObj, APP_CMD_EXIT, 0, 0);
			exit = 1;
			break;
		}
	}
	
	Msg_Kill(ievt->qid);
	tObj->active = 0;
	
	aprintf("exit...\n");
	
	return NULL;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) start!
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_event_hub_init(void)
{
	app_thr_obj *tObj;
	
	tObj = &ievt->rObj;
	if (thread_create(tObj, THR_event_hub_poll, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr event hub poll thread!\n");
		return EFAIL;
    }
	
	tObj = &ievt->sObj;
	if (thread_create(tObj, THR_event_hub_main, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr event hub main thread\n");
		return EFAIL;
    }
	
	aprintf("done!...\n");
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_event_hub_exit(void)
{
	app_thr_obj *tObj;

	/* delete usb scan object */
    tObj = &ievt->sObj;
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
    aprintf("... done!\n");
	
	return 0;
}

/*****************************************************************************
* @brief    send noty event (device insert / remove)
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_event_hub_set_dev_status(int type, int ste)
{
	app_thr_obj *tObj = &ievt->sObj;
	
	/* APP_KEY_UP을 이용한다 */
	event_send(tObj, APP_KEY_UP, type, ste);
	
	return 0;
}

/*****************************************************************************
* @brief    send noty event (device insert / remove)
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_event_hub_rndis_status(int ste)
{
	app_thr_obj *tObj = &ievt->sObj;
	
	/* APP_KEY_UP을 이용한다 */
	event_send(tObj, APP_KEY_DOWN, ste, 0);
	
	return 0;
}
