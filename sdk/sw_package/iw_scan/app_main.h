/*
 * File : app_main.h
 *
 * Copyright (C) 2015 UDWORKs
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

#ifndef __APP_MAIN_H__
#define __APP_MAIN_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include "app_msg.h"
#include "ipc_common_def.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define APP_TAG		"IWA"

/* for debugging macro */
//#define __APP_DEBUG__
#define DBG_RED		"\x1b[31m"
#define DBG_GREEN	"\x1b[32m"
#define DBG_YELLOW	"\x1b[33m"
#define DBG_BLUE	"\x1b[34m"
#define DBG_RESET	"\x1b[0m"

#define aprintf(x...) do { printf(DBG_GREEN " [iwscan ] %s: ", __func__); printf(x); printf(DBG_RESET); } while(0)
#define eprintf(x...) do { printf(DBG_RED " [iwscan ERR!] %s: ", __func__); printf(x); printf(DBG_RESET); } while(0)

#ifdef __APP_DEBUG__
#define dprintf(x...) do { printf(" [iwscan ] %s: ", __func__); printf(x); } while(0)
#else
#define dprintf(x...)
#endif

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef	union {
	unsigned int word;

	struct {
		unsigned int udev:1;        /* usb device state */
		unsigned int wifi_on:1;     /* Wi-Fi device state */
		unsigned int link:1;        /* Wi-Fi link succeed */
	} bit;

} app_state_t;

typedef struct {
	app_thr_obj mObj; //# main thread
	app_state_t ste;

	char link_essid[32]; //# connecting AP essid..
} app_cfg_t;

extern app_cfg_t *app_cfg;
extern app_shm_t *app_shm;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#endif	/* __APP_MAIN_H__ */
