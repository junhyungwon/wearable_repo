/******************************************************************************
 * NEXTT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    main.c
 * @brief
 * @section MODIFY history
 *     - 2020.07.08 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h> //# mmap
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#include "netmgr_ipc_cmd_defs.h"

#include "wlan_station.h"
#include "wlan_softap.h"
#include "rndis.h"
#include "event_hub.h"
#include "poll_wlan.h"
#include "poll_cradle_dev.h"
#include "poll_usbnet.h"
#include "main.h"
#include "common.h"
#include "usb2eth.h"
#include "cradle_eth.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_netmgr_cfg_t cfg_obj;
app_netmgr_cfg_t *app_cfg = &cfg_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 shared memory init - use only communication
-----------------------------------------------------------------------------*/
static void shared_mem_init(void)
{
	/* wi-fi AP 리스트를 전달하기 위한 shared memory 할당 */
	app_cfg->shmid = shmget((key_t)NETMGR_SHM_KEY, (size_t)NETMGR_SHM_SIZE, 0777|IPC_CREAT);
	if (app_cfg->shmid == -1) {
		eprintf("Get shared memory ID faild!\n");
	}
	
	app_cfg->shm_buf = (unsigned char *)shmat(app_cfg->shmid, 0, 0);
	if (app_cfg->shm_buf == (unsigned char *)-1) {
		eprintf("alloc Shared memory buffer faild!\n");
	}
}

/*----------------------------------------------------------------------------
 main thread function - use only communication
-----------------------------------------------------------------------------*/
static int main_thread_init(void)
{
	app_thr_obj *tObj = &app_cfg->mObj;
	
	memset((void *)app_cfg, 0, sizeof(app_netmgr_cfg_t));
	
	//#--- create thread
	if (thread_create(tObj, NULL, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}

	return 0;
}

static void main_thread_exit(void)
{
	app_thr_obj *tObj = &app_cfg->mObj;

	//#--- stop thread
	thread_delete(tObj);
}

static void ___thread_start(void)
{
	/* 쓰레드가 먼저 시작되면 동작을 안 함
	 * main app이 데이터를 수신할 수 있을 때 시작함.
	 */
	/* Wi-Fi Client Mode */
	netmgr_wlan_cli_init();
	/* Wi-Fi Host Mode */
	netmgr_wlan_hostapd_init();
	/* rndis */
	netmgr_rndis_init();
	/* usb2eth */
	netmgr_usb2eth_init();
	/* eth0 */
	netmgr_cradle_eth_init();
	
	/* 연결된 USB 장치를 확인 (가장 늦게 실행 해야 함) */
	netmgr_poll_wlan_init();
	netmgr_poll_cradle_init();
	netmgr_poll_usbnet_init();
	
	dprintf("enter...!\n");
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
static void app_main(void)
{
	app_thr_obj *tObj = &app_cfg->mObj;
	int exit = 0,cmd;
	
	tObj->active = 1;
	
	/* mapp와 message를 송/수신..*/
	netmgr_event_hub_init(); //# 위치 변경하면 안됨.
	
	while(!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT)
			break;
		else if (cmd == APP_CMD_START)
			___thread_start();
	}
	
	netmgr_wlan_cli_exit();
	netmgr_wlan_hostapd_exit();
	netmgr_rndis_exit();
	netmgr_usb2eth_exit();
	netmgr_cradle_eth_exit();
	
	netmgr_poll_wlan_exit();
	netmgr_poll_cradle_exit();
	netmgr_poll_usbnet_exit();
	netmgr_event_hub_exit();
	
	tObj->active = 0;
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
int main(int argc, char **argv)
{
	main_thread_init();
	shared_mem_init();
	
	//#--- main --------------
	app_main();
	//#-----------------------
	
	main_thread_exit();
	
	dprintf("...exit!\n");
	
	return 0;
}
