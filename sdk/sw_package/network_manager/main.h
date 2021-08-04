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
#include <syslog.h>
#include "msg.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define dprintf(x...) do { printf(" [NETMGR ] %s: ", __func__); printf(x);} while(0)
#define eprintf(x...) do { printf(" [NETMGR ERR!] %s: ", __func__); printf(x);} while(0)

#define sysprint(x...) do { printf(" [NETMGR LOG] %s: ", __func__); printf(x); syslog(LOG_INFO, x);} while(0)

#ifndef TRUE
#define TRUE 		1
#endif

#ifndef FALSE
#define FALSE 		0
#endif

#define ARRAY_SIZE(x) 			(sizeof(x) / sizeof(x[0]))
#define GPIO_N(b, n)			((32*b) + n)
#define BACKUP_DET				GPIO_N(1, 13)	//# cradle detect

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef	union {
	unsigned int word;

	struct {
		unsigned int wlan:1;		/* usb wi-fi가 연결되면 1 아니면 0 */
		unsigned int rndis:1;		/* rndis 연결되면 1 아니면 0 */
		unsigned int usb2eth:1;		/* usb2eth 연결되면 1 아니면 0 */
		unsigned int eth0:1;		/* ethernet0 연결되면 1 아니면 0 */
		unsigned int cradle:1;		/* cradle 연결되면 1 아니면 0 */
	} bit;

} app_state_t;

typedef struct {
	app_thr_obj mObj; //# main thread
	app_state_t ste;
	
	int shmid;
	unsigned char *shm_buf;
	
	/* usb wi-fi vid, pid를 저장한다.*/
	int wlan_vid;
	int wlan_pid;
	
} app_netmgr_cfg_t;

extern app_netmgr_cfg_t *app_cfg;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#endif	/* __MAIN_H__ */
