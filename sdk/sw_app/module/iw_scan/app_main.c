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
app_cfg_t *app_cfg = NULL;
app_shm_t *app_shm = NULL;
/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    THR_waitmsecs
* @section  [desc]
*****************************************************************************/
void THR_waitmsecs(unsigned int msecs)
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
int THR_event_wait(app_thr_obj *tObj)
{
	THR_semHndl *pSem;
	int r = -1;

	if (!tObj->active)
		return -1;

	pSem = &tObj->sem;

  	pthread_mutex_lock(&pSem->lock);
	while (1) {
		if (pSem->count > 0) {
			pSem->count--;
			r = 0;
			break;
		} else {
			pthread_cond_wait(&pSem->cond, &pSem->lock);
		}
	}
  	pthread_mutex_unlock(&pSem->lock);

	return tObj->cmd;
}

/*****************************************************************************
* @brief    event send to thread.
* @section  [desc]
*****************************************************************************/
int THR_event_send(app_thr_obj *tObj, int cmd, int prm0, int prm1)
{
	THR_semHndl *pSem;
	int r = 0;

	if (tObj == NULL || !tObj->active) {
		return -1;
	}

	tObj->cmd = cmd;
	tObj->param0 = prm0;
	tObj->param1 = prm1;

	pSem = &tObj->sem;
  	pthread_mutex_lock(&pSem->lock);
  	if (pSem->count < pSem->maxCount) {
    	pSem->count++;
    	r |= pthread_cond_signal(&pSem->cond);
  	}
  	pthread_mutex_unlock(&pSem->lock);

	return 0;
}

static int main_thr_init(void)
{
	THR_semHndl *pSem;

	pthread_mutexattr_t muattr;
  	pthread_condattr_t cond_attr;

  	int res = 0;

    memset((void *)app_cfg, 0, sizeof(app_cfg_t));
	app_cfg->ste.word = 0; /* all clear */

	app_cfg->mObj = (app_thr_obj *)malloc(sizeof(app_thr_obj));
	if (app_cfg->mObj == NULL) {
		eprintf("Failed to alloc main thread obj!!\n");
		return -1;
	}

	pSem = &app_cfg->mObj->sem;

  	res |= pthread_mutexattr_init(&muattr);
  	res |= pthread_condattr_init(&cond_attr);

  	res |= pthread_mutex_init(&pSem->lock, &muattr);
  	res |= pthread_cond_init(&pSem->cond, &cond_attr);

  	pSem->count = 0;
  	pSem->maxCount = MAX_PENDING_SEM_CNT;

  	if (res != 0)
    	eprintf("Failed to pthread mutext init = %d\n", res);

  	pthread_condattr_destroy(&cond_attr);
  	pthread_mutexattr_destroy(&muattr);

	return 0;
}

static void main_thr_exit(void)
{
	THR_semHndl *pSem;

	if (app_cfg->mObj != NULL) {
		pSem = &app_cfg->mObj->sem;
		pthread_cond_destroy(&pSem->cond);
  		pthread_mutex_destroy(&pSem->lock);

		free(app_cfg->mObj);
	}
}

static int scan_shm_init(void)
{
	int r = FXN_ERR_SHM_CREATE;
	app_shm = (app_shm_t *)malloc(sizeof(app_shm_t));
	if (app_shm == NULL) {
		dprintf("Shared memory create faild!\n");
		return r;
	}

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
	app_thr_obj *tObj = (app_thr_obj *)app_cfg->mObj;
	int exit = 0, cmd;

	printf(" [task] %s start\n", __func__);

	tObj->active = 1;
	while (!exit)
	{
		//# wait cmd
		cmd = THR_event_wait(tObj);
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

	app_cfg = (app_cfg_t *)malloc(sizeof(app_cfg_t));
	if (app_cfg == NULL)
		return -1;

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

	if (app_cfg != NULL) {
		free(app_cfg);
		app_cfg = NULL;
	}

	if (app_shm != NULL) {
		free(app_shm);
		app_shm = NULL;
	}


	if (r)
		fprintf(stderr, "Failed to system init(ret = 0x%08x)\n", r);
	else
		printf("\n--- IWSCAN end!! ---\n");

	return 0;
}
