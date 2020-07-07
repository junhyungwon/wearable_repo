/******************************************************************************
 * UCX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_main.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2014.10.01 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include <app_main.h>
#include <app_udev.h>
#include <app_iwscan.h>
#include <app_ipc.h>
#include <app_net_manager.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_cfg_t cfg_obj;
static app_shm_t shm_obj;

app_cfg_t *app_cfg = &cfg_obj;
app_shm_t *app_shm = &shm_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

static int main_thr_init(void)
{
	app_thr_obj *tObj = &app_cfg->mObj;
	int res = 0;
	
    memset((void *)app_cfg, 0, sizeof(app_cfg_t));
	
	//#--- create thread
	if (thread_create(tObj, NULL, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}

	return 0;
}

static void main_thr_exit(void)
{
	app_thr_obj *tObj = &app_cfg->mObj;

	//#--- stop thread
	thread_delete(tObj);
}

static int scan_shm_init(void)
{
	int r = FXN_ERR_SHM_CREATE;
	
	//# Shared memory create
	app_shm->shmid = shmget((key_t)SHM_KEY, (size_t)(sizeof(iwscan_list_t)+1), 0777|IPC_CREAT);
	if (app_shm->shmid == -1) {
		dprintf("Shared memory ID faild!\n");
		return r;
	}

	//# Get shared memory
	app_shm->shm_buf = (unsigned char*)shmat(app_shm->shmid, 0, 0);
	if (app_shm->shm_buf == (unsigned char *)-1) {
		dprintf("Shared memory buffer faild!\n");
		return r;
	}

	return 0;
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
static void app_main(void)
{
	app_thr_obj *tObj = &app_cfg->mObj;
	int exit = 0, cmd;

	printf(" [task] %s start\n", __func__);

	tObj->active = 1;
	while (!exit)
	{
		//# wait cmd
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_STOP) {
			break;
		}
	}
	tObj->active = 0;

	printf(" [task] %s stop\n", __func__);
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
int main(int argc, char **argv)
{
	int r = 0;

	printf("\n--- IWSCAN start ---\n");

	main_thr_init();

	r = scan_shm_init();
	r |= app_udev_init();
	r |= app_net_mgr_init();
	r |= app_iscan_init();
	r |= app_ipc_init();
	if (r)
		goto err_exit;

	//#--- main --------------
	app_main();
	//#-----------------------

err_exit:
	app_udev_exit();
	app_net_mgr_exit();
	app_iscan_exit();
	app_ipc_exit();

	main_thr_exit();

	if (r)
		fprintf(stderr, "Failed to system init(ret = 0x%08x)\n", r);
	else
		printf("\n--- IWSCAN end!! ---\n");

	return 0;
}
