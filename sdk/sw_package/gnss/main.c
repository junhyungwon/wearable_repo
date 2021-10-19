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
#include <limits.h>
#include <fcntl.h>
#include <sys/mman.h> //# mmap
#include <sys/time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
	   
#include "gnss_ipc_cmd_defs.h"
#include "main.h"
#include "gps_proc.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

static app_gnss_cfg_t cfg_obj;
app_gnss_cfg_t *app_cfg = &cfg_obj;

struct gps_device_t *session;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 message send/recv function
-----------------------------------------------------------------------------*/
int send_msg(int cmd)
{
	to_gnss_main_msg_t msg;
	
	msg.type = GNSS_MSG_TYPE_TO_MAIN;
	msg.cmd = cmd;
	
	return Msg_Send(app_cfg->qid, (void *)&msg, sizeof(to_gnss_main_msg_t));
}

int recv_msg(void)
{
	to_gnss_msg_t msg;
	
	//# blocking
	if (Msg_Rsv(app_cfg->qid, GNSS_MSG_TYPE_TO_GPS, (void *)&msg, sizeof(to_gnss_msg_t)) < 0) {
		return -1;
	}
	
	if (msg.cmd == GNSS_CMD_GPS_START) {
		app_cfg->rate = msg.rate;
		strcpy(app_cfg->dev_name, msg.dev_name);
	}
	
	return msg.cmd;
}

/*****************************************************************************
* @brief    system shared memory init
* @section  [desc]
*****************************************************************************/
static int app_shared_mem_init(void)
{
	unsigned char *tbuf = NULL;
	int tmp_id;
	
	//# Shared memory create
	tmp_id = shmget((key_t)GNSS_SHM_KEY, (size_t)GNSS_SHM_SIZE, (0777|IPC_CREAT));
	if (tmp_id == -1) {
		eprintf("Shared memory ID faild!\n");
		return -1;
	}
	
	//# Get shared memory
	tbuf = (unsigned char *)shmat(tmp_id, 0, 0);
	if (app_cfg->shmbuf == (unsigned char *)-1) {
		eprintf("Shared memory buffer faild!\n");
		return -1;
	}
	
	app_cfg->shmid  = tmp_id;
	app_cfg->shmbuf = tbuf; 
	
	return 0;
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
static void app_main(void)
{
	int exit = 0, cmd;

	app_cfg->qid = Msg_Init(GNSS_MSG_KEY);
	send_msg(GNSS_CMD_GPS_READY);
	
	while (!exit) {
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive gps process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case GNSS_CMD_GPS_START:
			//dprintf("GNSS_CMD_GPS_START command!!\n");
			app_gps_proc_start();
			break;
		case GNSS_CMD_GPS_REQ_DATA:
			//dprintf("GNSS_CMD_GPS_REQ_DATA command!\n");
			app_gps_data_request();
			break;
		case GNSS_CMD_GPS_STOP:
			//dprintf("GNSS_CMD_GPS_STOP command!\n");
			app_gps_proc_stop();
			break;
		case GNSS_CMD_GPS_EXIT:
			//dprintf("received gps exit!\n");
			exit = 1;
			break;
		
		default:
			break;	
		}
	}
	
	Msg_Kill(app_cfg->qid);
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
int main(int argc, char **argv)
{
	int rc = 0;

//	dprintf(" [GPS process] start...\n");
	
	session = &app_cfg->t_device;
	rc = app_shared_mem_init();
	if (rc < 0)
		goto err_exit;
	
	app_gps_proc_init();
	
	//#--- main --------------
	app_main();
	//#-----------------------
	
	app_gps_proc_exit();
	
err_exit:	
	if (app_cfg->shmbuf != NULL)
		free(app_cfg->shmbuf);
	
	printf(" [GPS] process exit!\n");
	
	return 0;
}
