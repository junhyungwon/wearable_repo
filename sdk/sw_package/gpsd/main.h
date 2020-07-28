/*
 * File : main.h
 *
 * Copyright (C) 2020 Texas Instruments
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __MAIN_H__
#define __MAIN_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include "gnss.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/* for debugging macro */
#define __APP_DEBUG__

#define aprintf(x...) do { printf(" [AGPS ] %s: ", __func__); printf(x); } while(0)
#define eprintf(x...) do { printf(" [AGPS ERR!] %s: ", __func__); printf(x); } while(0)

#ifdef __APP_DEBUG__
#define dprintf(x...) do { printf(" [AGPS ] %s: ", __func__); printf(x); } while(0)
#else
#define dprintf(x...)
#endif

#ifndef TRUE
#define TRUE 		1
#endif

#ifndef FALSE
#define FALSE 		0
#endif

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#endif	/* __MAIN_H__ */
