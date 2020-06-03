/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    draw_font.h
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2012.04.03 : First Created
 */
/*****************************************************************************/

#if !defined (_DRAW_TEXT_H_)
#define _DRAW_TEXT_H_

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "draw_gui.h"
#include "font_db.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
int draw_text(Upix *dst_addr, char *str, int font, int sx, int sy, Upix txt_color, Upix bg_color);
int draw_text_rot(Upix *dst_addr, char *str, int font, int sx, int sy, Upix txt_color, Upix bg_color);
int draw_string_len(char *str, int font);
int draw_font_height(char *str, int font);

#endif	//_DRAW_TEXT_H_
