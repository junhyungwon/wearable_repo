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

#include "draw_gui.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_cap.h"
#include "gui_tvo.h"
#include "gui_main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define UI_CYCLE_TIME		200	//# ms

typedef struct {
	app_thr_obj mObj;	//# gui main thread
	app_thr_obj uObj;	//# gui run thread
	app_thr_obj eObj;	//# gui menu thread

	int layout;
	int run_draw;
	int flag_rst;

	int day;			//# current day
	int c_bloff;		//# backlight off cnt
	long c_p_batt;		//# p_batt cnt when parking mode
} app_gui_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_gui_t gui_obj;
static app_gui_t *igui=&gui_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 draw_logo/clear function
-----------------------------------------------------------------------------*/
void gui_draw_time(void)
{
	time_t now;
	struct tm *ts;
	char buf_date[64], buf_time[64];
	int up_date=0;

	//# get current date & time
	now = time(NULL);
	ts = localtime(&now);

	strftime(buf_date, sizeof(buf_date), "%Y/%2m/%2d", ts);		//# %Y/%2m/%2d
	strftime(buf_time, sizeof(buf_time), "%2H:%2M:%2S", ts);	//# %2H(I-12hour):%2M:%2S or %X, %p(AM/PM)

	tvo_draw_time(buf_date, buf_time, up_date);
}

void gui_draw_state(int idx, int value, int draw)
{
	tvo_draw_state(idx, value, draw);
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

	tvo_draw_logo(ENA);

	while(!exit)
	{
		cmd = tObj->cmd;
		if(cmd==APP_CMD_EXIT) {
			break;
		}

		switch(cmd)
		{
			default:
				break;
		}

		//# cmd clear
		tObj->cmd = 0;

		//# draw state
		gui_draw_time();
		gui_draw_state(STE_GPS, iapp->ste.b.gps, UI_DRAW);

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
	iapp->lang = LANG_KOREAN;

	//ctrl_snd_volume(90);

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

	gui_tvo_init();

	//#--- create gui run thread
	tObj = &igui->uObj;
	if(thread_create(tObj, THR_gui_run, UI_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}

	return SOK;
}

int app_gui_exit(void)
{
	app_thr_obj *tObj;

	//#--- stop thread
	tObj = &igui->uObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		app_msleep(10);
	}
	thread_delete(tObj);

	gui_tvo_exit();

	return SOK;
}
