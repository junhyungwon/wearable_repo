/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    draw_gui.h
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2012.04.03 : First Created
 */
/*****************************************************************************/

#ifndef _GUI_DRAW_H_
#define _GUI_DRAW_H_

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

#define PIXEL16
//#define PIXEL32

#ifdef PIXEL16
typedef	unsigned short	Upix;

/* RGB565 Color Define */
#define RGB_BLACK		0x0000
#define RGB_BLACK_BG	0x2104
#define RGB_D_GRAY		0x5AEB
#define RGB_L_GRAY		0xAD55
#define RGB_B_GRAY		0x4229
#define RGB_F_GRAY		0xACF3
#define RGB_WHITE		0xFFFF
#define RGB_IVORY		0xCE78
#define RGB_RED			0xF800
#define RGB_D_RED		0x8000
#define RGB_L_RED		0xFD55
#define RGB_GREEN		0x07E0
#define RGB_DD_GREEN	0x0360
#define RGB_BLUE		0x001F
#define RGB_D_BLUE		0x000A
#define RGB_DD_BLUE		0x0003
#define RGB_L_BLUE		0xAD5F
#define RGB_L_SKY    	0xAEDC
#define RGB_G_BLUE   	0x9E79
#define RGB_YELLOW		0xFFE0
#define RGB_L_YELLOW	0xFFF5
#define RGB_D_YELLOW	0x8400
#define RGB_CYAN		0x07FF
#define RGB_L_CYAN   	0xE7FF
#define RGB_MAGENTA		0xF81F
#define RGB_BROWN       0x3000
#define RGB_D_ORANGE    0xFC60
///////gui menu bar color/////
#define RGB_D_BG        0x31C9
#define RGB_L_BLACK     0x1905
#endif

#ifdef PIXEL32
typedef	unsigned int	Upix;

/* RGB888 Color Define */
#define RGB_BLACK		0x00000000
#define RGB_WHITE       0x00FFFFFF
#define RGB_D_GRAY      0x00666666
#define RGB_L_GRAY      0x00999999
#define RGB_RED			0x00FF0000
#define RGB_GREEN		0x0000FF00
#define RGB_BLUE		0x000000FF
#define RGB_YELLOW		0x00FFFF00
#define RGB_MAGENTA		0x00FF00FF
#define RGB_BROWN       0x00330000
#define RGB_ORANGE      0x00FF6600
#define RGB_L_YELLOW    0x00FAED7D
#endif

#define RGB_KEY_24		0x8080A0
#define RGB_KEY			0x8414//RGB_MAGENTA
#define RGB_NO			RGB_KEY-1

//----------------------------------------------------

typedef struct {
	int x;
	int y;
	int w;		// width
	int h;		// height
} ui_pos_t;

typedef struct {
	int w0;		//# normal
	int h0;
	int w1;		//# rotate
	int h1;
} ui_size_t;

typedef enum {
	XCENTER=0,
	XLEFT,
	XRIGHT
} TXT_ALIGN;

#define UI_NORMAL	0
#define UI_ROTATE	1

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
int draw_fill_color(Upix *buf_addr, Upix color);
int draw_fill_rect(Upix *buf_addr, ui_pos_t st, Upix color);
int draw_mesh_rect(Upix *buf_addr, ui_pos_t st, Upix color);
int draw_line(Upix *buf_addr, ui_pos_t st, int direct, Upix color);
int draw_rect_line(Upix *buf_addr, ui_pos_t st, int thick, Upix color);
int draw_img(Upix *buf_addr, Upix *img_addr, ui_pos_t st);

int draw_fill_color_rot(Upix *buf_addr, Upix color);
int draw_fill_rect_rot(Upix *buf_addr, ui_pos_t st, Upix color);
int draw_mesh_rect_rot(Upix *buf_addr, ui_pos_t st, Upix color);
int draw_line_rot(Upix *buf_addr, ui_pos_t st, int direct, Upix color);
int draw_rect_line_rot(Upix *buf_addr, ui_pos_t st, int thick, Upix color);
int draw_img_rot(Upix *buf_addr, Upix *img_addr, ui_pos_t st);

int draw_size_set(int dst, int width, int height);

#endif //_GUI_DRAW_H_
