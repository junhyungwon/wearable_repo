/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	gui_main.h
 * @brief
 */
/*****************************************************************************/

#ifndef _GUI_MAIN_H_
#define _GUI_MAIN_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
//# common include
#include "dev_gfx.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define UI_HDMI			GFX0
#define UI_LCD			GFX1
#define UI_TVO			GFX2

#define UI_TVO_WI		720
#define UI_TVO_HE		480

#define UI_DRAW			1
#define UI_CLEAR		0

//# Do not set lower than APP_THREAD_PRI
#define UI_THREAD_PRI	2

typedef enum {
	STE_GPS,

	UI_STE_MAX
} gui_state_e;

//# string format
#define formDATE	"0000/00/00"
#define formTIME	"00:00:00"//"00:00:00 AM"

//# language
typedef enum {
	LANG_KOREAN,
	LANG_ENGLISH,
	MAX_LANG
} gui_lang_e;

typedef struct {
	char *txt[MAX_LANG];
} gui_str_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_gui_init(void);
int app_gui_exit(void);

int gui_main(void);
int gui_ctrl(int cmd, int p0, int p1);

void gui_draw_logo(void);
void gui_draw_clear(void);
void gui_draw_time(void);
void gui_draw_state(int idx, int value, int draw);

#endif	/* _GUI_MAIN_H_ */
