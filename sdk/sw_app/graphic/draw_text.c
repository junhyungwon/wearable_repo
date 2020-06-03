/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    draw_font.c
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

#include "draw_text.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern ui_size_t gfbz;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief	draw_text function
* @section	DESC Description
*	- draw 8x8 english txt
*****************************************************************************/
int draw_text(Upix *dst_addr, char *str, int font, int sx, int sy, Upix txt_color, Upix bg_color)
{
	font_info_t ifnt;
	unsigned char *pfnt, *pdata;

	Upix *dst;
	int i, j, bg_on=0;;
	char mask;

	if(dst_addr==NULL)
		return -1;
	if(bg_color!=RGB_NO)
		bg_on = 1;
	if(font > FNT_DB_MAX-1)
		return -1;

	while (*str != 0)
	{
		get_font_db(&str, &ifnt, font);

		pfnt = (unsigned char *)ifnt.p;
		dst = (Upix *)(dst_addr + (sy*gfbz.w0) + sx);
		for(j=0; j<ifnt.he; j++)
		{
			pdata = (unsigned char *)pfnt;
			mask = 0x80;
			for(i=0; i<ifnt.wi; i++)
			{
				if( mask == 0 ) {
					mask = 0x80;
					pdata++;
				}
				if (*pdata & mask)
					*dst = txt_color;
				else {
					if(bg_on)
						*dst = bg_color;
				}
				dst++;
				mask >>= 1;
			}
			dst += (gfbz.w0 - ifnt.wi);
			pfnt += ifnt.bz;
		}

		sx += ifnt.wi;
	}

	return (sx);
}

int draw_text_rot(Upix *dst_addr, char *str, int font, int sx, int sy, Upix txt_color, Upix bg_color)
{
	font_info_t ifnt;
	unsigned char *pfnt, *pdata;

	Upix *dst, *dst_tmp;
	int i, j, bg_on=0;
	char mask;

	if(dst_addr==NULL)
		return -1;
	if(bg_color!=RGB_NO)
		bg_on = 1;
	if(font > FNT_DB_MAX-1)
		return -1;

	while (*str != 0)
	{
		get_font_db(&str, &ifnt, font);

		pfnt = (unsigned char *)ifnt.p;
		dst = (Upix *)(dst_addr + (sx*gfbz.w1) + (gfbz.w1-sy));

		for(j=0; j<ifnt.he; j++)
		{
			pdata = (unsigned char *)pfnt;
			dst_tmp = dst;
			mask = 0x80;
			for(i=0; i<ifnt.wi; i++)
			{
				if( mask == 0 ) {
					mask = 0x80;
					pdata++;
				}
				if (*pdata & mask)
					*dst_tmp = txt_color;
				else {
					if(bg_on)
						*dst_tmp = bg_color;
				}
				dst_tmp += gfbz.w1;
				mask >>= 1;
			}
			dst--;
			pfnt += ifnt.bz;
		}
		sx += ifnt.wi;
	}

	return (sx);
}

int draw_string_len(char *str, int font)
{
	font_info_t ifnt;
	int len=0;

	if(font > FNT_DB_MAX-1)
		return -1;

	while (*str != 0)
	{
		get_font_db(&str, &ifnt, font);

		len += ifnt.wi;
	}

	return len;
}

int draw_font_height(char *str, int font)
{
	font_info_t ifnt;
	int he=0;

	if(font > FNT_DB_MAX-1)
		return -1;

	while (*str != 0)
	{
		get_font_db(&str, &ifnt, font);

		he = ifnt.he;
	}

	return he;
}
