/*
 * File : app_comm.h
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

#ifndef __APP_COMM_H__
#define __APP_COMM_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <pthread.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
//#define __APP_DEBUG__
#define MAX_PENDING_SEM_CNT		(1)

#define FXN_THR_PRI_MAX          sched_get_priority_max(SCHED_FIFO)
#define FXN_THR_PRI_MIN          sched_get_priority_min(SCHED_FIFO)

#define FXN_THR_PRI_DEFAULT      (FXN_THR_PRI_MIN + (FXN_THR_PRI_MAX - FXN_THR_PRI_MIN)/2)

//# error macro
#define FXN_ERR_SHM_CREATE		(0x00000001)
#define FXN_ERR_UDEV_INIT		(0x00000002)
#define FXN_ERR_IWSCAN_INIT		(0x00000004)
#define FXN_ERR_IPC_INIT		(0x00000008)

#define FXN_ERR_NET_MGR_INIT	(0x00000010)

//# error type
#define SOK			(0)
#define EFAIL		(-1)
#define EPARAM		(-2)
#define EINVALID	(-3)
#define EMEM		(-4)

#define ON			1
#define OFF			0
#define ENA			1	//# enable
#define DIS			0	//# disable

#ifndef KB
#define KB	1024
#endif
#ifndef MB
#define MB	(KB*KB)
#endif

#define MAX_STR_LEN		128		//# string length

/* for debugging macro */
#define aprintf(x...) do { printf(" [iwscan ] %s: ", __func__); printf(x); } while(0)
#define eprintf(x...) do { printf(" [iwscan ERR!] %s: ", __func__); printf(x); } while(0)

#ifdef __APP_DEBUG__
#define dprintf(x...) do { printf(" [iwscan ] %s: ", __func__); printf(x); } while(0)
#else
#define dprintf(x...)
#endif

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	pthread_t hndl;

} THR_thrHndl;

typedef struct {
  	pthread_mutex_t lock;
  	pthread_cond_t  cond;

	unsigned int count;
  	unsigned int maxCount;

} THR_semHndl;

typedef struct {
	THR_thrHndl thr;
	THR_semHndl sem;
	int active;

	int cmd;
	int param0;
	int param1;

} app_thr_obj;

typedef struct {
	int shmid;
	unsigned char *shm_buf;
} app_shm_t;

typedef enum {
	//# for thread
	APP_CMD_START = 0x1,
	APP_CMD_STOP,
	APP_CMD_PAUSE,
	APP_CMD_NOTY,
	APP_CMD_EXIT,

	//# for key
	APP_KEY_UP,
	APP_KEY_LEFT,
	APP_KEY_SEL,
	APP_KEY_RIGHT,
	APP_KEY_DOWN,

	//# for Wi-Fi Client
	APP_CLI_START,
	APP_CLI_STOP,
	APP_CLI_WAIT_FOR_AUTH,
	APP_CLI_GET_AUTH_STATUS,
	APP_CLI_GET_IP,
	APP_CLI_GET_NET_STATUS,

	MAX_APP_CMD

} app_cmd_e;

extern app_shm_t *app_shm;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#endif /* #ifndef __APP_COMMON_H__ */
