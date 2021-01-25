/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    gui_main.c
 * @brief
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "app_comm.h"
#include "app_main.h"
#include "app_cap.h"
#include "app_ctrl.h"
#include "gui_main.h"
#include "app_util.h"
#include "app_dev.h"
#include "dev_gpio.h"
#include "dev_snd.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define USE_MENU				1

//# For sound
#define APP_SND_SRATE			8000 //# for baresip
#define APP_SND_PTIME			250 //# fixed 고정해야 함.
#define APP_SND_CH				1   /* sound channel */

#define UI_CYCLE_TIME			200	//# ms

typedef enum {
	KEY_NONE = 0,
	KEY_SHORT,
	KEY_LONG
} key_type_e;

typedef struct {
	app_thr_obj mObj;	//# gui main thread
	app_thr_obj uObj;	//# gui run thread
	app_thr_obj iObj;	//# ir thread
	
	app_thr_obj sndIn;	//# ir thread
	app_thr_obj sndOut;	//# ir thread
	
} app_gui_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_gui_t gui_obj;
static app_gui_t *igui=&gui_obj;

static int snd_pipe[2];

static snd_prm_t snd_in_data;
static snd_prm_t snd_out_data;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

//#----------------------- KEY -----------------------------------------------
/*****************************************************************************
* @brief    rec key switch check function
* @section  [desc]
*****************************************************************************/
static int dev_ste_key(int gio)
{
	int status;

	gpio_get_value(gio, &status);
	//dprintf("--- [key] value %d\n", status);

	return status;
}

static int cnt_key=0, ste_key=KEY_NONE;
static int chk_rec_key(void)
{
	int key = dev_ste_key(REC_KEY);

	if(ste_key != KEY_NONE) {
		if(key == 1) {
			ste_key = KEY_NONE;
		}
		return KEY_NONE;
	}

	if(cnt_key)
	{
		if(key == 0) {	//# press
			cnt_key--;
			if(cnt_key == 0) {
				ste_key = KEY_LONG;
			}
		}
		else {
			cnt_key = 0;
			ste_key = KEY_SHORT;
		}
	}
	else {
		if(ste_key == KEY_NONE) {
			if(key == 0) {
				cnt_key = 20;		//# 1sec
			}
		}
	}

	return ste_key;
}

static void *thread_key(void *prm)
{
	app_thr_obj *tObj = &igui->iObj;
	int exit=0, ret;
	char cmd;

	tObj->active = 1;

	while (!exit)
	{
		#if USE_MENU
		cmd = menu_get_cmd();
		gui_ctrl(APP_KEY_OK, cmd, 0);
		#else
		/* TODO */
		#endif
	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}
//# ---------------------------------------------------------------------------
//################## TEST Functions ##########################################
/*----------------------------------------------------------------------------
 wait result
 type 0: draw skip/ok/fail, type 1: draw ok/fail
-----------------------------------------------------------------------------*/
static int wait_result(app_thr_obj *tObj)
{
	int cmd, exit=0;

	while (!exit)
	{
		//# wait cmd
		cmd = event_wait(tObj);
		switch (cmd) {
		case APP_KEY_OK:
		default:
			exit = 1;
			break;
		}
	}

	return 0;
}

static char menu_exit[] = {
	"\r\n ============="
	"\r\n  EXIT        "
	"\r\n ============="
	"\r\n"
	"\r\n 0: exit"
	"\r\n"
	"\r\n Enter Choice: "
};

static int test_info(app_thr_obj *tObj)
{
	int res;
	char ver[64]={0,};

	aprintf("version test start...\n");

	//# sw version
	sprintf(ver, "%s", FITT360_SW_VER);
	dprintf("sw version %s\n", ver);
	strcpy(iapp->sw_ver, ver);

	res = ctrl_get_hw_version(NULL);
	iapp->hw_ver = res;
	dprintf("hw version %d\n", res);
	
	res = ctrl_get_mcu_version(ver);
	iapp->mcu_ver = res;
	/* sprintf(version, "%02d.%02X", (ver>>8)&0xFF, ver&0xFF); */
	dprintf("mcu version %s\n", ver);
	
	printf(menu_exit);
	res = wait_result(tObj);
	
	return res;
}

static int test_buzzer(app_thr_obj *tObj)
{
	int res = 0;
	
	aprintf("buzzer test start...\n");
	
	app_buzzer(100, 10);
	
	printf(menu_exit);
	res = wait_result(tObj);
	
	return res;
}

static int test_key(app_thr_obj *tObj)
{
	int res = 0, exit=0;
	
	aprintf("key test start...\n");
	printf("Press REC Key!!\n");
	while (!exit) 
	{
		res = chk_rec_key();
		if (res == KEY_SHORT) {
			dprintf("KEY OK!!\n");
			break;
		}
		
		app_msleep(UI_CYCLE_TIME);	
	}
	
	printf(menu_exit);
	res = wait_result(tObj);
	
	return res;
}

/* Network (ethernet) */
static int test_net(app_thr_obj *tObj)
{
	int res, count=0;

	aprintf("net test start...\n");
	while (count >= 10)
	{
		res = ctrl_chk_network();
		if (res <= 0 ) {
			count++;
		} else {
			dprintf("ping test ok!!\n");
			break;
		}
	}

	printf(menu_exit);
	res = wait_result(tObj);

	return res;
}

/* Sound In/Out */
static void thr_snd_in_cleanup(void *prm)
{
	dev_snd_stop(&snd_in_data, SND_PCM_CAP);
	dev_snd_param_free(&snd_in_data);
}

static void *thr_snd_in(void *prm)
{
	int buf_sz;
	int exit = 0, r;

	dev_snd_set_input_path(SND_MIC_IN);
	/* get alsa period size (in sec) */
	buf_sz = APP_SND_SRATE * APP_SND_PTIME / 1000; //# 
	r = dev_snd_set_param("aic3x", &snd_in_data, SND_PCM_CAP, 
				APP_SND_CH, APP_SND_SRATE, buf_sz);

	r |= dev_snd_open("plughw:0,0", &snd_in_data);
	if (r) {
		eprintf("Failed to init sound device!\n");
	}
	
	dev_snd_start(&snd_in_data);
	dev_snd_set_volume(SND_VOLUME_C, 60);	//# set default volume 60%
	pthread_cleanup_push(thr_snd_in_cleanup, (void *)&snd_in_data);

	while (!exit)
	{
		int bytes = 0;
		
		bytes = dev_snd_read(&snd_in_data);
		if (bytes > 0) {
			write(snd_pipe[1], snd_in_data.sampv, bytes);
		}
	}

	pthread_cleanup_pop(0);

	return NULL;
}

static void thr_snd_out_cleanup(void *prm)
{
	OSA_waitMsecs(100);
	dev_snd_stop(&snd_out_data, SND_PCM_PLAY);
	dev_snd_param_free(&snd_out_data);
}

void *thr_snd_out(void *prm)
{
	int buf_sz, so_size;
	int exit = 0, r;

	dev_snd_set_volume(SND_VOLUME_P, 70);

	/* get alsa period size (in sec) */
	buf_sz  = APP_SND_SRATE * APP_SND_PTIME / 1000; //# 
	so_size = (buf_sz * APP_SND_CH * (SND_PCM_BITS / 8)); 
	r = dev_snd_set_param("aic3x", &snd_out_data, SND_PCM_PLAY, 
				APP_SND_CH, APP_SND_SRATE, buf_sz);

	r |= dev_snd_open("plughw:0,0", &snd_out_data);
	if (r) {
		eprintf("Failed to init sound device!\n");
		return NULL;
	}

	dev_snd_set_swparam(&snd_out_data, SND_PCM_PLAY);
	pthread_cleanup_push(thr_snd_out_cleanup, (void *)&snd_out_data);

	while (!exit)
	{
		r = read(snd_pipe[0], snd_out_data.sampv, so_size);
		if (r > 0) {
			dev_snd_write(&snd_out_data, r/2);
		}
	}

	pthread_cleanup_pop(0);

	return NULL;
}

int test_snd(app_thr_obj *tObj)
{
	int res;

	aprintf("sount test start...\n");
	
	pipe(snd_pipe);

	/* create sound in/out thread */
	thread_create(&igui->sndIn, thr_snd_in, APP_THREAD_PRI, NULL);
	thread_create(&igui->sndOut, thr_snd_out, APP_THREAD_PRI, NULL);
	
	//# Question
	printf(menu_exit);
	res = wait_result(tObj);
	
	thread_delete(&igui->sndOut);
	thread_delete(&igui->sndIn);

	return res;
}

//############################################################################

/*****************************************************************************
* @brief    test menu function
* @section  [desc]
*****************************************************************************/
static char menu_main[] = {
	"\r\n ============="
	"\r\n Demo Menu"
	"\r\n ============="
	"\r\n"
	"\r\n 0: exit"
	"\r\n 1: Jpeg"
	"\r\n"
	"\r\n 2: Get version"
	"\r\n 3: Buzzer test"
	"\r\n 4: Key test"
	"\r\n 5: Network test"
	"\r\n 6: Sound test"
	"\r\n"
	"\r\n Enter Choice: "
};

static int gui_test_main(void *thr)
{
	app_thr_obj *tObj = (app_thr_obj *)thr;
	char cmd, exit=0;
	
	app_buzzer(100, 2);
	
	while (!exit)
	{
		//# wait cmd
		printf(menu_main);
		cmd = event_wait(tObj);
		if ((cmd == APP_CMD_EXIT) || (cmd == APP_KEY_PWR)) {
			break;
		}
		
		if (cmd == APP_KEY_OK) 
		{
			switch (tObj->param0) {
			case '0':
				/* exit program */
				exit=1;
				continue;
				
			case '1':
				/* camera */
				iapp->snapshot = 1; //# temp
				break;
			
			case '2':
				/* Get Version */
				test_info(tObj);
				break;
			
			case '3':
				/* Buzzer Version */
				test_buzzer(tObj);
				break;
			
			case '4':
				/* REC KEY */
				test_key(tObj);
				break;
			
			case '5':
				test_net(tObj);
				break;
			
			case '6':
				test_snd(tObj);
				break;		
			}
		}
	}
	
	return 0;
}

/*****************************************************************************
* @brief    gui run thread function
* @section  [desc]
*****************************************************************************/
static void *THR_gui_run(void *prm)
{
	app_thr_obj *tObj = &igui->uObj;
	int cmd, exit=0;

	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = tObj->cmd;
		if (cmd == APP_CMD_EXIT) {
			break;
		}

		/* TODO */
		switch (cmd) {
		default:
			break;
		}

		//# cmd clear
		tObj->cmd = 0;
		//# wait time
		app_msleep(UI_CYCLE_TIME);
	}

	tObj->active = 0;
	aprintf("exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    gui main function
* @section  [desc]
*****************************************************************************/
int gui_main(void)
{
	app_thr_obj *tObj = (app_thr_obj *)&igui->mObj;

	//#--- create thread - for communcation
	if(thread_create(tObj, NULL, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	tObj->active = 1;

	app_cap_start();

	gui_test_main((void *)tObj);

	app_cap_stop();

	//#--- stop thread
	tObj->active = 0;
	thread_delete(tObj);

	return SOK;
}

/*****************************************************************************
* @brief    gui control function
* @section  [desc]
*****************************************************************************/
int gui_ctrl(int cmd, int p0, int p1)
{
	event_send(&igui->mObj, cmd, p0, p1);

	return 0;
}

/*****************************************************************************
* @brief    gui init/exit function
* @section  [desc]
*****************************************************************************/
int app_gui_init(void)
{
	app_thr_obj *tObj;

	//# static config clear
	memset((void *)igui, 0x0, sizeof(app_gui_t));
	
	gpio_input_init(REC_KEY);
	
	//#--- create gui run thread
	tObj = &igui->uObj;
	if (thread_create(tObj, THR_gui_run, UI_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}
	
	//#--- create ir thread
	tObj = &igui->iObj;
	if(thread_create(tObj, thread_key, UI_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}

	return SOK;
}

int app_gui_exit(void)
{
	app_thr_obj *tObj;
	
	//#--- stop thread
	tObj = &igui->iObj; //# key thread..
	thread_delete(tObj);
	
	tObj = &igui->uObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		app_msleep(10);
	}
	thread_delete(tObj);
	
	gpio_exit(REC_KEY);
	
	return SOK;
}
