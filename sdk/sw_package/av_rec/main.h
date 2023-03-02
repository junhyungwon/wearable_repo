/*
 * File : app_main.h
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

#ifndef __APP_MAIN_H__
#define __APP_MAIN_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <syslog.h>

#include "msg.h"
#include "board_config.h"
#include "app_gmem.h"

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/evp.h>


/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define __DEBUG__
#define USE_SYSLOGD  (1) //# 0-> disable syslogd

#ifdef __DEBUG__
#define eprintf(x...) do { printf(" [AVREC ERR!] %s: ", __func__); printf(x); fflush(stdout);} while(0)
#define dprintf(x...) do { printf(" [AVREC ] %s: ", __func__); printf(x); fflush(stdout);} while(0)
#	if USE_SYSLOGD
#	define LOGD(x...)	do {printf(" [AVREC ] %s: ", __func__); printf(x); fflush(stdout); syslog(LOG_INFO, x);} while(0)
#	define LOGE(x...)	do {printf(" [AVREC err!] %s: ", __func__); printf(x); fflush(stdout); syslog(LOG_ERR, x);} while(0)
#	else
#	define LOGD(x...)	do {printf(" [AVREC ] %s: ", __func__); printf(x); fflush(stdout);} while(0)
#	define LOGE(x...)	do {printf(" [AVREC err!] %s: ", __func__); printf(x); fflush(stdout);} while(0)
#	endif
#else
#define eprintf(x...)
#define dprintf(x...)
#define LOGD(x...)
#define LOGE(x...)
#endif /* #ifdef __DEBUG__ */

#ifndef TRUE
#define TRUE 		1
#endif

#ifndef FALSE
#define FALSE 		0
#endif

AES_KEY aes_key_128;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef	union {
	unsigned int word;
	struct {
		unsigned int mmc:1;        /* mmc device state */
	} bit;

} app_state_t;

typedef struct {
	app_thr_obj mObj; //# main thread
	app_state_t ste;
	
} app_rec_cfg_t;

extern app_rec_cfg_t *app_cfg;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#endif	/* __APP_MAIN_H__ */
