/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    draw_gui.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2012.04.03 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "draw_gui.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
ui_size_t gfbz;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief	draw gui functions
* @section	DESC Description
*	- desc
*****************************************************************************/
int draw_fill_color(Upix *buf_addr, Upix color)
{
	int i, j;
	Upix *p;

	if(buf_addr==NULL)
		return -1;

	p = (Upix *)buf_addr;
	for(i=0; i<gfbz.h0; i++) {
		for(j=0; j<gfbz.w0; j++) {
			*p++ = color;
		}
	}

	return 0;
}

int draw_fill_color_rot(Upix *buf_addr, Upix color)
{
	int i, j;
	Upix *p;

	if(buf_addr==NULL)
		return -1;

	p = (Upix *)buf_addr;
	for(i=0; i<gfbz.h1; i++) {
		for(j=0; j<gfbz.w1; j++) {
			*p++ = color;
		}
	}

	return 0;
}

int draw_fill_rect(Upix *buf_addr, ui_pos_t st, Upix color)
{
	int i, j;
	Upix *p;

	if(buf_addr==NULL)
		return -1;
	if(st.w > gfbz.w0 || st.h > gfbz.h0)
		return -1;

	p = (Upix *)(buf_addr + ((st.x)+(st.y*gfbz.w0)));
	for(j=0; j<st.h; j++)
	{
		for(i=0; i<st.w; i++) {
			*p++ = color;
		}
		p += (gfbz.w0-st.w);
	}
    return 0;
}

int draw_mesh_rect(Upix *buf_addr, ui_pos_t st, Upix color)
{
	int i, j, k;
	Upix *p, *q;

	if(buf_addr==NULL)
		return -1;
	if(st.w > gfbz.w0 || st.h > gfbz.h0)
		return -1;

	p = (Upix *)(buf_addr + ((st.x)+(st.y*gfbz.w0)));
	k = 0;
	for(j=0; j<st.h; j++)
	{
		q = p+k;
		for(i=k; i<st.w/2; i++) {
			*q = color;
			q += 2;
		}
		p += gfbz.w0;
		k = 1 - k;
	}
    return 0;
}

int draw_fill_rect_rot(Upix *buf_addr, ui_pos_t st, Upix color)
{
	int i, j;
	Upix *p;

	if(buf_addr==NULL)
		return -1;
	if(st.w > gfbz.h1 || st.h > gfbz.w1)
		return -1;

	p = (Upix *)(buf_addr + (st.x*gfbz.w1) + ((gfbz.w1-1)-st.y));
	for(j=0; j<st.w; j++)
	{
		for(i=0; i<st.h; i++) {
			*p-- = color;
		}
		p += (gfbz.w1+st.h);
	}
    return 0;
}

int draw_mesh_rect_rot(Upix *buf_addr, ui_pos_t st, Upix color)
{
	int i, j, k;
	Upix *p, *q;

	if(buf_addr==NULL)
		return -1;
	if(st.w > gfbz.h1 || st.h > gfbz.w1)
		return -1;

	p = (Upix *)(buf_addr + (st.x*gfbz.w1) + ((gfbz.w1-1)-st.y));
	k = 0;
	for(j=0; j<st.w; j++)
	{
		q = p-k;
		for(i=0; i<st.h/2; i++) {
			*q = color;
			q -= 2;
		}
		p += gfbz.w1;
		k = 1 - k;
	}
    return 0;
}

int draw_line(Upix *buf_addr, ui_pos_t st, int direct, Upix color)
{
	int i, j;
	Upix *p, *q;

	if(buf_addr==NULL)
		return -1;

	//# st.w = thick
	//# st.h = lenght

	p = (Upix *)(buf_addr + ((st.x)+(st.y*gfbz.w0)));
	if(!direct)		// left->right
	{
		for(j=0; j<st.w; j++)
		{
			for(i=0; i<st.h; i++) {
				*p++ = color;
			}
			p += gfbz.w0-st.h;
		}
	}
	else			// top->bottom
	{
		for(j=0; j<st.w; j++)
		{
			q = (Upix *)p;
			for(i=0; i<st.h; i++) {
				*q = color;
				q += gfbz.w0;
			}
			p += 1;
		}
	}

	return 0;
}

int draw_line_rot(Upix *buf_addr, ui_pos_t st, int direct, Upix color)
{
	int i, j;
	Upix *p, *q;

	if(buf_addr==NULL)
		return -1;

	//# st.w = thick
	//# st.h = lenght

	p = (Upix *)(buf_addr + (st.x*gfbz.w1) + ((gfbz.w1-1)-st.y));
	if(!direct)		// top->bottom
	{
		for(j=0; j<st.w; j++)
		{
			q = (Upix *)p;
			for(i=0; i<st.h; i++) {
				*q = color;
				q += gfbz.w1;
			}
			p -= 1;
		}
	}
	else			// right->left
	{
		for(j=0; j<st.w; j++)
		{
			for(i=0; i<st.h; i++) {
				*p-- = color;
			}
			p += gfbz.w1+st.h;
		}
	}

	return 0;
}

int draw_rect_line(Upix *buf_addr, ui_pos_t st, int thick, Upix color)
{
	ui_pos_t t;

	if(buf_addr==NULL)
		return -1;

	t.x=st.x; t.y=st.y; t.w=thick; t.h=st.w;
	draw_line(buf_addr, t, 0, color);
	t.h=st.h;
	draw_line(buf_addr, t, 1, color);

	t.x = st.x;
	t.y = st.y + st.h - thick;
	t.h = st.w;
	draw_line(buf_addr, t, 0, color);

	t.x = st.x + st.w - thick;
	t.y = st.y;
	t.h = st.h;
	draw_line(buf_addr, t, 1, color);

	return 0;
}

int draw_rect_line_rot(Upix *buf_addr, ui_pos_t st, int thick, Upix color)
{
	ui_pos_t t;

	if(buf_addr==NULL)
		return -1;

	t.x=st.x; t.y=st.y; t.w=thick; t.h=st.w;
	draw_line_rot(buf_addr, t, 0, color);
	t.h=st.h;
	draw_line_rot(buf_addr, t, 1, color);

	t.x = st.x;
	t.y = st.y + st.h - thick;
	t.h = st.w;
	draw_line_rot(buf_addr, t, 0, color);

	t.x = st.x + st.w - thick;
	t.y = st.y;
	t.h = st.h;
	draw_line_rot(buf_addr, t, 1, color);

	return 0;
}

int draw_img(Upix *buf_addr, Upix *img_addr, ui_pos_t st)
{
	int i, j;
	Upix *p;

	if(buf_addr==NULL || img_addr==NULL)
		return -1;
	if(st.w > gfbz.w0 || st.h > gfbz.h0)
		return -1;

	p = (Upix *)(buf_addr + ((st.x)+(st.y*gfbz.w0)));
	for(j=0; j<st.h; j++)
	{
		for(i=0; i<st.w; i++)
		{
			if(*img_addr == RGB_KEY) {
				p += 1;
				img_addr += 1;
			} else {
				*p++ = *img_addr++;
			}
		}
		p += (gfbz.w0-st.w);
	}

	return 0;
}

int draw_img_rot(Upix *buf_addr, Upix *img_addr, ui_pos_t st)
{
	int i, j;
	Upix *p, *p_tmp;

	if(buf_addr==NULL || img_addr==NULL)
		return -1;
	if(st.w > gfbz.h1 || st.h > gfbz.w1)
		return -1;

	p = (Upix *)(buf_addr + (st.x*gfbz.w1) + ((gfbz.w1-1)-st.y));
	for(j=0; j<st.h; j++)
	{
		p_tmp = p;
		for(i=0; i<st.w; i++)
		{
			if(*img_addr == RGB_KEY) {
				img_addr++;
			} else {
				*p_tmp = *img_addr++;
			}
			p_tmp += gfbz.w1;
		}
		p--;
	}

	return 0;
}

int draw_size_set(int dst, int width, int height)
{
	if(dst==UI_ROTATE) {	//# rotate fb size
		gfbz.w1 = height;
		gfbz.h1 = width;
	}
	else {					//# normal fb size
		gfbz.w0 = width;
		gfbz.h0 = height;
	}

	return 0;
}
