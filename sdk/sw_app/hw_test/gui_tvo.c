/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    gui_tvo.c
 * @brief	gui draw tv-out function
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

#include "draw_text.h"
#include "draw_gui.h"
#include "img_ud_logo_tvo.h"

#include "dev_snd.h"
#include "dev_disk.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_ctrl.h"
#include "app_dev.h"
#include "app_mcu.h"
#include "gui_main.h"
#include "gui_tvo.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
//# status string layout
#define FNTsz		FNT_SZ02
#define FNTct		RGB_WHITE	//# text color
#define FNTcb		RGB_BLACK	//# text bg color

#define ICON_WI		24
#define LINE_HE		28
#define SPACE		8

//# status bar
#define STEx		420
#define STE0y		20			//# date, time, status icon

typedef struct {
	int (*func)(void *);
	int	pass;
	char *str;
	char *title;
} test_func_t;

typedef struct {
	Upix *buf;			//# tvo graphic(fb2) buffer
	int init;

	app_thr_obj aObj;	//# test thread
	int exit;

	//# position
	int datex;
	int timex;
	int iconx;
	int iconw;
} gui_tvo_t;

#define ROK			0
#define RFAIL		1
#define RSKIP		2
#define RNONE		3
#define REXIT		4

#define OKFAIL		1
#define OKFAILSKIP	0

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static gui_tvo_t tvo_obj={0,};
static gui_tvo_t *itvo=&tvo_obj;

//# ts main menu area
#define TS_MENU_W	80
#define TS_MENU_H	30
static ui_pos_t ts_menu[] = {
	{ 90, 45, (UI_TVO_WI-120), (UI_TVO_HE-60)},
	{130,100, TS_MENU_W, TS_MENU_H},	//# start
	{230,100, TS_MENU_W, TS_MENU_H},	//# exit
};
static Upix menu_color[] = {RGB_BLUE, RGB_RED};
static char *str_menu[] = {
	"시 작",
	"종 료",
};

//# ts button area
#define TS_BTN_W	80
static ui_pos_t ts_btn[] = {
	{           90,             50, (UI_TVO_WI-80), (UI_TVO_HE-50)},
	{TS_BTN_W*0+110, (UI_TVO_HE-60), TS_BTN_W-8, 30},	//# skip
	{TS_BTN_W*1+110, (UI_TVO_HE-60), TS_BTN_W-8, 30},	//# fail
	{TS_BTN_W*2+110, (UI_TVO_HE-60), TS_BTN_W-8, 30},	//# ok
};
static Upix btn_color[] = {RGB_B_GRAY, RGB_RED, RGB_BLUE};
static char *str_btn[] = {
	"취 소",
	"아니오",
	"예",
};

//# ts status area
#define MAX_STE		16
#define TS_STE_W	60
#define TS_STE_H	30
static ui_pos_t ts_ste[] = {
	{30, 15, TS_STE_W, UI_TVO_HE-60},
	{30, 15, TS_STE_W, TS_STE_H},
	{30, 45, TS_STE_W, TS_STE_H},
	{30, 75, TS_STE_W, TS_STE_H},
	{30,105, TS_STE_W, TS_STE_H},
	{30,135, TS_STE_W, TS_STE_H},
	{30,165, TS_STE_W, TS_STE_H},
	{30,195, TS_STE_W, TS_STE_H},
	{30,225, TS_STE_W, TS_STE_H},
	{30,255, TS_STE_W, TS_STE_H},
	{30,285, TS_STE_W, TS_STE_H},
	{30,315, TS_STE_W, TS_STE_H},
	{30,345, TS_STE_W, TS_STE_H},
	{30,375, TS_STE_W, TS_STE_H},
	{30,405, TS_STE_W, TS_STE_H},
	{30,435, TS_STE_W, TS_STE_H},
	{30,465, TS_STE_W, TS_STE_H},
};
static Upix ste_color[] = {RGB_BLUE, RGB_RED, RGB_B_GRAY, RGB_BLACK};

//# For sound
#define APP_SND_SRATE			8000 //# for baresip
#define APP_SND_PTIME			250 //# fixed 고정해야 함.
#define APP_SND_CH				1   /* sound channel */

static OSA_ThrHndl sndInThr;
#if defined(NEXX360W) /* mic in + earphone out */
static OSA_ThrHndl sndOutThr;
#endif
static int snd_pipe[2];

static snd_prm_t snd_in_data;
static snd_prm_t snd_out_data;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 draw string
-----------------------------------------------------------------------------*/
static inline int ui_draw_txt(char *str, int sx, int sy, Upix col_txt, Upix col_bg)
{
	return draw_text(itvo->buf, str, FNTsz, sx, sy, col_txt, col_bg);
}

static inline int ui_draw_text(char *str, int sx, int sy, int size, Upix col_txt, Upix col_bg)
{
	return draw_text(itvo->buf, str, size, sx, sy, col_txt, col_bg);
}

static int ui_draw_title(char *str, int font, ui_pos_t txt, int align, Upix col_txt, Upix col_bg)
{
	int str_wi, str_he, sx, sy;

	if(str == NULL) {
		return -1;
	}
	if(font > 3) {
		font = 3;
	}

	if(font == 0 || font == 1)	str_he = 24;
	else if(font == 2)			str_he = 28;
	else						str_he = 32;

	str_wi = draw_string_len(str, font);
	if(str_wi > txt.w) {
		eprintf("str length great than drawing area!\n");
		eprintf("(%s)\n", str);
		return -1;
	}

	if(align == XCENTER) {
		sx = txt.x + ((txt.w-str_wi)/2);
	}
	else if(align == XRIGHT) {
		sx = txt.x + (txt.w-str_wi);
	}
	else {	//# XLEFT
		sx = txt.x + SPACE;
	}

	sy = txt.y + ((txt.h-str_he)/2);

	return draw_text(itvo->buf, str, font, sx, sy, col_txt, col_bg);
}

/*****************************************************************************
* @brief    draw tvo function
* @section  [desc]
*****************************************************************************/
void tvo_draw_logo(int draw)
{
	ui_pos_t st;

	if(itvo->init) {
		st.x=0; st.y=0; st.w=720; st.h=480;
		draw_img((Upix *)itvo->buf, (Upix *)img_ud_logo_tvo, st);
	}
}

void tvo_draw_clear(Upix color)
{
	if(itvo->init) {
		draw_fill_color(itvo->buf, color);
	}
}

void tvo_draw_time(char *date, char *time, int up_date)
{
	if(itvo->init) {
		ui_draw_txt(date, itvo->datex, STE0y, FNTct, FNTcb);
		ui_draw_txt(time, itvo->timex, STE0y, FNTct, FNTcb);
	}
}

void tvo_draw_state(int idx, int value, int draw)
{
	int sx;
	Upix bgcol;

	if(!itvo->init) {
		return;
	}
	if(idx > UI_STE_MAX-1) {
		return;
	}

	sx = itvo->iconx+(itvo->iconw*idx);
	if(value)	bgcol = RGB_BLUE;
	else		bgcol = RGB_RED;

	switch(idx)
	{
		case STE_GPS:
			ui_draw_txt(" G ", sx, STE0y, FNTct, bgcol);
			break;
		default:
			break;
	}
}

/*----------------------------------------------------------------------------
 wait result
 type 0: draw skip/ok/fail, type 1: draw ok/fail
-----------------------------------------------------------------------------*/
static int wait_result(app_thr_obj *tObj, int type, int btn)
{
	int cmd, exit=0, ret=ROK;
	int i, sel, draw=1;

	if(btn < type) {
		btn = type;
	}

	sel = btn;
	while(!exit)
	{
		if(draw)
		{
			draw = 0;

			//# draw button
			for(i=type; i<3; i++)
			{
				if(i==sel) {
					draw_fill_rect(itvo->buf, ts_btn[i+1], btn_color[i]);
					ui_draw_title(str_btn[i], FNT_SZ02, ts_btn[i+1], XCENTER, RGB_L_SKY, RGB_NO);
				} else {
					draw_fill_rect(itvo->buf, ts_btn[i+1], RGB_BLACK);
					draw_rect_line(itvo->buf, ts_btn[i+1], 2, btn_color[i]);
					ui_draw_title(str_btn[i], FNT_SZ02, ts_btn[i+1], XCENTER, RGB_L_SKY, RGB_NO);
				}
			}
		}

		//# wait cmd
		cmd = event_wait(tObj);

		switch(cmd)
		{
			case APP_KEY_OK:
				switch(sel)
				{
					case 0:		//# skip
						ret = RSKIP;
						break;
					case 1:		//# fail
						ret = RFAIL;
						break;
					case 2:		//# ok
						ret = ROK;
						break;
				}
				exit = 1;
				break;
			case APP_KEY_RIGHT:
				sel++;
				if(sel > 2)		{sel = type;}
				draw = 1;
				break;
			case APP_KEY_LEFT:
				sel--;
				if(sel < type)	{sel = 2;}
				draw = 1;
				break;
			case APP_KEY_PWR:
				ret = REXIT;
				exit = 1;
				break;
		}
	}

	return ret;
}

/*----------------------------------------------------------------------------
 draw question
-----------------------------------------------------------------------------*/
static int draw_qna(app_thr_obj *tObj, char *str, Upix col_txt, int type)
{
	ui_pos_t st;
	int btn;

	//# Question
	st.x = ts_menu[0].x+20; st.y = 378;
	ui_draw_text(str, st.x, st.y, FNT_SZ03, col_txt, RGB_BLACK);

	btn = 2;		//# default OK
	return wait_result(tObj, type, btn);
}

#if 0
static int draw_confirm(app_thr_obj *tObj)
{
	int cmd, exit=0;

	while(!exit)
	{
		//# draw button
		draw_fill_rect(itvo->buf, ts_btn[3], btn_color[2]);
		ui_draw_title(str_btn[2], FNT_SZ02, ts_btn[3], XCENTER, RGB_L_SKY, RGB_NO);

		//# wait cmd
		cmd = event_wait(tObj);

		switch(cmd)
		{
			case APP_KEY_OK:
				exit = 1;
				break;
		}
	}

	return ROK;
}
#endif

/*----------------------------------------------------------------------------
 draw test info
-----------------------------------------------------------------------------*/
static void draw_sub_title(char *str)
{
	ui_pos_t st;

	st.x = ts_menu[0].x+20; st.y = 46;
	ui_draw_text(str, st.x, st.y, FNT_SZ04, RGB_L_SKY, RGB_BLACK);
}

static void draw_sub_text(char *str, int line, Upix col_txt)
{
	ui_pos_t st;

	st.x = ts_menu[0].x+20; st.y = 76+(line*LINE_HE);
	ui_draw_text(str, st.x, st.y, FNT_SZ03, col_txt, RGB_BLACK);
}

static void draw_sub_text_e(char *str, int line, Upix col_txt, int offset)
{
	ui_pos_t st;

	st.x = ts_menu[0].x+20; st.y = 76+(line*LINE_HE);
	ui_draw_text(str, st.x+offset, st.y, FNT_SZ03, col_txt, RGB_BLACK);
}

static void clear_sub_text(int line, Upix color)
{
	ui_pos_t st;

	st.x = ts_menu[0].x; st.y = 76+(line*LINE_HE); st.w = ts_menu[0].w; st.h = LINE_HE;
	draw_fill_rect(itvo->buf, st, color);
}

/*----------------------------------------------------------------------------
 clear test function area
-----------------------------------------------------------------------------*/
static void draw_sub_clear(void)
{
	draw_fill_rect(itvo->buf, ts_menu[0], RGB_BLACK);
}

/*****************************************************************************
* @brief    test functions
* @section  [desc]
*****************************************************************************/
int test_time(app_thr_obj *tObj)
{
	int ret;
	time_t now;
	struct tm *ts;
	char buf[128];

	aprintf("start...\n");
	draw_sub_clear();
	draw_sub_title("시간 확인");

	//# get current date & time
	now = time(NULL);
	ts = localtime(&now);

	draw_sub_text("현재시간 : ", 1, RGB_B_GRAY);
	strftime(buf, sizeof(buf), "%Y/%2m/%2d  %2I:%2M:%2S %p", ts);
	draw_sub_text(buf, 2, RGB_F_GRAY);

	//# Question
	ret = draw_qna(tObj, "시간이 맞습니까?", RGB_F_GRAY, OKFAIL);

	return ret;
}

static void *thr_gps(void *prm)
{
	int ste;

	while(1)
	{
		ste = iapp->ste.b.gps;
		draw_sub_text("GPS 상태", 1, RGB_B_GRAY);
		draw_sub_text_e(ste?"연결":"연결안됨", 1, ste?RGB_BLUE:RGB_RED, 160);

		OSA_waitMsecs(200);
	}

	return NULL;
}

int test_gps(app_thr_obj *tObj)
{
	app_thr_obj *testObj = &itvo->aObj;
	int ret;

	aprintf("start...\n");
	draw_sub_clear();
	draw_sub_title("GPS 테스트");

	//# action read sens
	thread_create(testObj, thr_gps, APP_THREAD_PRI, NULL);

	//# Question
	ret = draw_qna(tObj, "GPS가 연결 되었습니까?", RGB_F_GRAY, OKFAILSKIP);

	thread_delete(testObj);

	return ret;
}

int test_net(app_thr_obj *tObj)
{
	int ret;

	aprintf("start...\n");

	while(1)
	{
		draw_sub_clear();
		draw_sub_title("Network 테스트");
		draw_sub_text("Wait...", 1, RGB_B_GRAY);

		ret = ctrl_chk_network();
		if(ret <= 0 ) {
			draw_sub_text("연결 실패", 1, RGB_RED);
			ret = draw_qna(tObj, "재시도 하겠습니까?", RGB_F_GRAY, OKFAIL);
			if(ret != ROK) {
				break;
			}
		} else {
			draw_sub_text("연결 성공", 1, RGB_BLUE);
			break;
		}
	}

	//# Question
	ret = draw_qna(tObj, "네트웍이 연결 되었습니까?", RGB_F_GRAY, OKFAIL);

	return ret;
}

/*
 * Bus 001 Device 001: ID 1d6b:0002
 * Bus 002 Device 001: ID 1d6b:0002
 * Bus 001 Device 002: ID 0ea0:2168
 * Bus 002 Device 002: ID 0bda:8179
 */
static int get_usb_id(int bus, int *v, int *p)
{
	char cmd[128] = {0,};
	char buf[256] = {0,};
	char *sptr;

	FILE *f;
	int find = 0;
	int busnum = -1;

	snprintf(cmd, sizeof(cmd), "/usr/bin/lsusb");
	f = popen(cmd, "r");
	if (f == NULL) {
		return -1;
	}

	while (fgets(buf, 255, f) != NULL)
	{
		char *vid, *pid;

		/* %*s->discard input */
		memset(cmd, 0, sizeof(cmd));
		sscanf(buf, "%*s%d%*s%*s%*s%*s", &busnum);
		sscanf(buf, "%*s%*s%*s%*s%*s%s", cmd);

		if (cmd != NULL)
		{
			vid = strtok_r(cmd, ":", &sptr);
			pid = strtok_r(NULL, ":", &sptr);

			if (bus == busnum) {
				/* ignore usb controller id */
				if ((strncmp(vid, "1d6b", 4) == 0) &&
					(strncmp(pid, "0002", 4) == 0))
					continue;

				if (vid != NULL && pid != NULL) {
					*v = (int)strtol(vid, NULL, 16);
					*p = (int)strtol(pid, NULL, 16);
					find = 1;
					break;
				}
			}
		}
	}

	pclose(f);

	return find;
}

static void *thr_usb(void *prm)
{
	int ste_usb=0;
	int ret, usb_v, usb_p;

	while(!itvo->exit)
	{
		//# check usb0
		usb_v = 0; usb_p = 0;
		ret = get_usb_id(1, &usb_v, &usb_p);
		if (ret == 1) {
			//printf("[bus 0] detected usb (%x, %x)\n", usb_v, usb_p);
			ste_usb = 1;
		} else {
			ste_usb = 0;
		}

		clear_sub_text(2, RGB_BLACK);
		draw_sub_text("USB 상태", 2, RGB_B_GRAY);
		draw_sub_text_e(ste_usb?"정상":"비정상", 2, ste_usb?RGB_BLUE:RGB_RED, 160);

		if(!ste_usb) {
			draw_sub_text("USB를 연결해 주십시요", 5, RGB_RED);
		}

		OSA_waitMsecs(200);
	}

	itvo->exit = 0;

	return NULL;
}

int test_usb(app_thr_obj *tObj)
{
	app_thr_obj *testObj = &itvo->aObj;
	int ret=0;

	aprintf("start...\n");
	draw_sub_clear();
	draw_sub_title("USB 테스트");

	//# action read sens
	itvo->exit = 0;
	thread_create(testObj, thr_usb, APP_THREAD_PRI, NULL);

	//# Question
	ret = draw_qna(tObj, "USB가 연결 되었습니까?", RGB_F_GRAY, OKFAILSKIP);

	itvo->exit = 1;
	while(itvo->exit) {
		app_msleep(100);
	}
	thread_delete(testObj);

	return ret;
}

static void *thr_sens(void *prm)
{
	int data;
	char buf[64];

	while(1)
	{
		//# power
		clear_sub_text(1, RGB_BLACK);
		draw_sub_text("전원 :", 1, RGB_B_GRAY);
		data = app_get_volt();
		sprintf(buf, "%02d.%02d V", (data/100), (data%100));
		draw_sub_text_e(buf, 1, RGB_F_GRAY, 100);
		if(data < 1100) {
			draw_sub_text_e("비정상 (체크 필요)", 1, RGB_RED, 320);
		}

		OSA_waitMsecs(100);
	}

	return NULL;
}

int test_sens(app_thr_obj *tObj)
{
	app_thr_obj *testObj = &itvo->aObj;
	int ret;

	aprintf("start...\n");
	draw_sub_clear();
	draw_sub_title("센서 및 입력 테스트");

	//# action read sens
	thread_create(testObj, thr_sens, APP_THREAD_PRI, NULL);

	//# Question
	ret = draw_qna(tObj, "센서 및 입력이 정상입니까?", RGB_F_GRAY, OKFAIL);

	thread_delete(testObj);

	return ret;
}

static void *thr_led(void *prm)
{
	int i, on=ON;

	while(1)
	{
		for (i = 0; i < LED_IDX_ALL; i++) {
			ctrl_leds(i, on);
		}
		OSA_waitMsecs(500);

		on = 1 - on;
	}

	return NULL;
}

int test_led(app_thr_obj *tObj)
{
	app_thr_obj *testObj = &itvo->aObj;
	int ret;

	aprintf("start...\n");
	draw_sub_clear();
	draw_sub_title("LED 테스트");

	draw_sub_text("모든 LED가 점멸합니다", 1, RGB_B_GRAY);

	//# action led blink
	thread_create(testObj, thr_led, APP_THREAD_PRI, NULL);

	//# Question
	ret = draw_qna(tObj, "LED가 점멸을 합니까?", RGB_F_GRAY, OKFAIL);

	thread_delete(testObj);

	return ret;
}

#if defined(NEXXONE) /* mic in + earphone out */
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
		return NULL;
	}
	
	dev_snd_start(&snd_in_data);
	dev_snd_set_volume(SND_VOLUME_C, 60);	//# set default volume 60%
	pthread_cleanup_push(thr_snd_in_cleanup, (void *)&snd_in_data);

	while(!exit)
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
	int buf_sz;
	int exit = 0, r;

	dev_snd_set_volume(SND_VOLUME_P, 80);

	/* get alsa period size (in sec) */
	buf_sz = APP_SND_SRATE * APP_SND_PTIME / 1000; //# 
	r = dev_snd_set_param("aic3x", &snd_out_data, SND_PCM_PLAY, 
				APP_SND_CH, APP_SND_SRATE, buf_sz);

	r |= dev_snd_open("plughw:0,0", &snd_out_data);
	if (r) {
		eprintf("Failed to init sound device!\n");
		return NULL;
	}

	dev_snd_set_swparam(&snd_out_data, SND_PCM_PLAY);
	pthread_cleanup_push(thr_snd_out_cleanup, (void *)pSnd);

	while(!exit)
	{
		r = read(snd_pipe[0], pSnd->buf, so_size);
		if (r > 0) {
			dev_snd_write((void *)pSnd->pcm_t, r/2);
		}
	}

	pthread_cleanup_pop(0);

	return NULL;
}

int test_snd(app_thr_obj *tObj)
{
	int ret;

	aprintf("start...\n");
	draw_sub_clear();
	draw_sub_title("오디오 입/출력 테스트");
	draw_sub_text("PC의 사운드 출력을 오디오 입력에 연결합니다", 1, RGB_F_GRAY);
	
	pipe(snd_pipe);

	/* create sound input thread */
	OSA_thrCreate(&sndInThr, thr_snd_in, APP_THREAD_PRI, 0, NULL);
	/* create sound output thread */
	OSA_thrCreate(&sndOutThr, thr_snd_out, APP_THREAD_PRI, 0, NULL);

	//# Question
	ret = draw_qna(tObj, "스피커로 사운드가 출력됩니까?", RGB_F_GRAY, OKFAIL);
	
	OSA_thrDelete(&sndOutThr);
	OSA_thrDelete(&sndInThr);

	return ret;
}
#else
static void tvo_draw_bar(int step)
{
	ui_pos_t st;
	int bar_grade=16, color;

	//# line 7
	//# clear
	st.x=ts_menu[0].x+20; st.y=(ts_menu[0].y+32)+(7*LINE_HE); st.w=(bar_grade*10); st.h=20;
	draw_fill_rect(itvo->buf, st, RGB_BLACK);

	st.w = (step * bar_grade);
	if(step > 1) 	color = RGB_D_RED;
	else			color = RGB_RED;
	draw_fill_rect(itvo->buf, st, color);
}

static int snd_average(short *pcm, int size, int offset)
{
	int i, sum=0;

	size /= 2;
	for (i=0; i<size; i++) {
		sum += abs(*pcm++ - offset);
	}

	return (sum / size);
}

static void thr_snd_in_cleanup(void *prm)
{
	dev_snd_stop(&snd_in_data, SND_PCM_CAP);
	dev_snd_param_free(&snd_in_data);
}

static void *thr_snd_in(void *prm)
{
	int buf_sz;
	int exit = 0, r;
	int cnt_skip = 15;

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

	dev_snd_set_volume(SND_VOLUME_C, 80);
	pthread_cleanup_push(thr_snd_in_cleanup, (void *)&snd_in_data);

	while(!exit)
	{
		int bytes = 0;
		
		bytes = dev_snd_read(&snd_in_data);
		if (bytes > 0) {
			if (cnt_skip >= 0) {
				cnt_skip--;
				tvo_draw_bar(1);
			}
			else
			{
				//# draw sound level
				int av;
				av = snd_average((short *)snd_in_data.sampv, r, 370);	//# aic3x input offset: 370
				if(av < 100)		tvo_draw_bar(1);	//# noise
				else if(av < 500)	tvo_draw_bar(2);
				else if(av < 1000)	tvo_draw_bar(3);
				else if(av < 1500)	tvo_draw_bar(4);
				else if(av < 2000)	tvo_draw_bar(5);
				else if(av < 2500)	tvo_draw_bar(6);
				else if(av < 3000)	tvo_draw_bar(7);
				else if(av < 3500)	tvo_draw_bar(8);
				else if(av < 4000)	tvo_draw_bar(9);
				else {
					tvo_draw_bar(10);
				}
			}
		}
	}

	pthread_cleanup_pop(0);

	return NULL;
}

int test_snd(app_thr_obj *tObj)
{
	int ret;

	aprintf("start...\n");
	draw_sub_clear();
	draw_sub_title("오디오 입력 테스트");

	/* create sound input thread */
	OSA_thrCreate(&sndInThr, thr_snd_in, APP_THREAD_PRI, 0, NULL);

	//# Question
	ret = draw_qna(tObj, "사운드가 입력됩니까?", RGB_F_GRAY, OKFAIL);

	OSA_thrDelete(&sndInThr);

	return ret;
}
#endif /* #if defined (NEXXONE) */

int test_video(app_thr_obj *tObj)
{
	int ret;
	ui_pos_t st;

	aprintf("start...\n");
	draw_sub_clear();

	//# video layout
	ctrl_swms_set(4, 0);

	//# clear video area
	st.x = 96; st.y = 46; st.w = 576; st.h = 324;
	draw_fill_rect(itvo->buf, st, RGB_KEY);

	//# draw channel number
	ui_draw_text("CH1", 100, 182, FNT_SZ02, RGB_WHITE, RGB_KEY);
	ui_draw_text("CH2", 100+288, 182, FNT_SZ02, RGB_WHITE, RGB_KEY);
	ui_draw_text("CH3", 100, 182+162, FNT_SZ02, RGB_WHITE, RGB_KEY);
	ui_draw_text("CH4", 100+288, 182+162, FNT_SZ02, RGB_WHITE, RGB_KEY);

	//# Question
	ret = draw_qna(tObj, "모든 채널영상이 출력됩니까?", RGB_F_GRAY, OKFAIL);

	return ret;
}

static int test_info(app_thr_obj *tObj)
{
	int ret;
	char ver[64];

	aprintf("start...\n");
	draw_sub_clear();
	draw_sub_title("시스템 정보");

	//# sw version
	draw_sub_text("SW version :", 1, RGB_B_GRAY);
	sprintf(ver, "%s", FITT360_SW_VER);
	draw_sub_text_e(ver, 1, RGB_F_GRAY, 180);

	draw_sub_text("HW version :", 2, RGB_B_GRAY);
	ctrl_get_hw_version(ver);
	draw_sub_text_e(ver, 2, RGB_F_GRAY, 180);

	draw_sub_text("MCU version :", 3, RGB_B_GRAY);
	ctrl_get_mcu_version(ver);
	draw_sub_text_e(ver, 3, RGB_F_GRAY, 180);

	//# Question
	ret = draw_qna(tObj, "시스템 정보가 맞습니까?", RGB_F_GRAY, OKFAIL);

	return ret;
}

//# test function list
test_func_t tf[] = {
	{(void *)test_info,		RNONE, "VER", "버젼"},
	{(void *)test_video,	RNONE, "VID", "영상"},
	{(void *)test_snd,		RNONE, "SND", "음성"},
	{(void *)test_led,		RNONE, "LED", "LED"},
	{(void *)test_sens,		RNONE, "SEN", "센서"},
	{(void *)test_usb,		RNONE, "USB", "USB"},
	{(void *)test_net,		RNONE, "NET", "NET"},
	{(void *)test_gps,		RNONE, "GPS", "GPS"},
	{(void *)test_time,		RNONE, "TIM", "시간"},
};

/*----------------------------------------------------------------------------
 draw menu & state
-----------------------------------------------------------------------------*/
static void draw_ste(int cnt)
{
	int i;

	if(cnt > MAX_STE-1) {
		cnt = MAX_STE;
	}

	for(i=0; i<cnt; i++)
	{
		draw_fill_rect(itvo->buf, ts_ste[i+1], ste_color[tf[i].pass]);
		draw_rect_line(itvo->buf, ts_ste[i+1], 2, RGB_B_GRAY);
		ui_draw_title(tf[i].str, FNT_SZ02, ts_ste[i+1], XCENTER, RGB_F_GRAY, RGB_NO);
	}
}

static int draw_result(app_thr_obj *tObj, int cnt)
{
	int ret, i, line=1, pass=1;
	char buf[128];

	aprintf("start...\n");
	app_buzzer(100, 2);

	draw_sub_clear();
	draw_sub_title("H/W 테스트 결과");

	//# draw result
	for(i=0; i<cnt; i++)
	{
		if(tf[i].pass) {
			sprintf(buf, "%s : 실패 또는 테스트 안됨", tf[i].str);
			draw_sub_text(buf, line++, RGB_RED);
			pass = 0;
		}
	}
	if(pass) {
		draw_sub_text("모든 테스트 성공", 1, RGB_BLUE);
	}

	ret = draw_qna(tObj, "테스트를 종료하겠습니까?", RGB_F_GRAY, OKFAIL);

	return ret;
}

static int draw_menu(app_thr_obj *tObj, int cnt)
{
	int cmd, exit=0, ret=ROK;
	int i, draw=1, sel=0;

	tvo_draw_clear(RGB_BLACK);	//# all clear

	while(!exit)
	{
		if(draw)
		{
			draw = 0;
			draw_sub_clear();
			draw_ste(cnt);

			//# draw title
			draw_sub_title("FITT H/W 테스트");

			for(i=0; i<2; i++)
			{
				if(i==sel) {
					draw_fill_rect(itvo->buf, ts_menu[i+1], menu_color[i]);
					ui_draw_title(str_menu[i], FNT_SZ02, ts_menu[i+1], XCENTER, RGB_L_SKY, RGB_NO);
				} else {
					draw_fill_rect(itvo->buf, ts_menu[i+1], RGB_BLACK);
					draw_rect_line(itvo->buf, ts_menu[i+1], 2, menu_color[i]);
					ui_draw_title(str_menu[i], FNT_SZ02, ts_menu[i+1], XCENTER, RGB_L_SKY, RGB_NO);
				}
			}
		}

		//# wait cmd
		cmd = event_wait(tObj);
		switch(cmd)
		{
			case APP_KEY_PWR:
				exit = 1;
				break;
			case APP_KEY_OK:
				switch(sel)
				{
					case 0:		//# start
						for(i=0; i<cnt; i++)
						{
							if(tf[i].func != NULL) {
								tf[i].pass = tf[i].func(tObj);
								if(tf[i].pass == REXIT) {
									return 0;
								}
								draw_ste(cnt);
							}
						}
						ret = draw_result(tObj, cnt);
						if(ret == ROK) {
							exit = 1;
						} else {
							draw = 1;
						}
						break;
					case 1:		//# exit
						exit = 1;
						break;
				}
				break;
			case APP_KEY_LEFT:
			case APP_KEY_RIGHT:
				sel = 1 - sel;
				draw = 1;
				break;
		}
	}

	return 0;
}

/*****************************************************************************
* @brief    test menu function
* @section  [desc]
*****************************************************************************/
int gui_test_main(void *thr)
{
	app_thr_obj *tObj = (app_thr_obj *)thr;
	int test_cnt;

	if(!itvo->init) {
		return EFAIL;
	}

	test_cnt = sizeof(tf)/sizeof(test_func_t);
	dprintf("test functions : %d\n", test_cnt);

	app_buzzer(100, 2);
	draw_menu(tObj, test_cnt);

	return 0;
}

/*****************************************************************************
* @brief    gui tv-out init function
* @section  [desc]
*****************************************************************************/
int gui_tvo_init(void)
{
	int buf;

	//# static config clear
	memset((void *)itvo, 0x0, sizeof(gui_tvo_t));

	//# graphic buffer init
	dev_gfx_init(UI_TVO, &buf, UI_TVO_WI, UI_TVO_HE);

	itvo->buf = (Upix *)buf;
	itvo->init = ENA;

	itvo->datex = STEx;
	itvo->timex = itvo->datex+draw_string_len(formDATE, FNTsz)+SPACE;
	itvo->iconx = itvo->timex+draw_string_len(formTIME, FNTsz)+SPACE;
	itvo->iconw = (ICON_WI+(SPACE/2));

	return 0;
}

int gui_tvo_exit(void)
{
	if(itvo->init) {
		dev_gfx_exit(UI_TVO);
	}

	return 0;
}
