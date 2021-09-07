/*
 * File : app_msg.h
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

#ifndef __APP_MSG_H__
#define __APP_MSG_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define MAX_PENDING_SEM_CNT		(1)

#define OSA_THR_PRI_MAX          sched_get_priority_max(SCHED_FIFO)
#define OSA_THR_PRI_MIN          sched_get_priority_min(SCHED_FIFO)
#define OSA_THR_PRI_DEFAULT     (OSA_THR_PRI_MIN + (OSA_THR_PRI_MAX-OSA_THR_PRI_MIN)/2)

#define APP_THREAD_PRI			(OSA_THR_PRI_DEFAULT + 1)

//# OSA error macro
#define OSA_SOK      			0  ///< Status : OK
#define OSA_EFAIL   			-1  ///< Status : Generic error
#define OSA_TIMEOUT_NONE        ((Uint32) 0)  // no timeout
#define OSA_TIMEOUT_FOREVER     ((Uint32)-1)  // wait forever

//# error macro
#define FXN_ERR_SHM_CREATE		(0x00000001)
#define FXN_ERR_UDEV_INIT		(0x00000002)
#define FXN_ERR_IWSCAN_INIT		(0x00000004)
#define FXN_ERR_IPC_INIT		(0x00000008)
#define FXN_ERR_NET_MGR_INIT	(0x00000010)

//# error type
#define SOK						(0)
#define EFAIL					(-1)
#define EPARAM					(-2)
#define EINVALID				(-3)
#define EMEM					(-4)

/* unsigned quantities */
typedef unsigned long long 		Uint64;     ///< Unsigned 64-bit integer
typedef unsigned int 			Uint32;     ///< Unsigned 32-bit integer
typedef unsigned short 			Uint16;     ///< Unsigned 16-bit integer
typedef unsigned char 			Uint8;      ///< Unsigned  8-bit integer

#define ON						1
#define OFF						0
#define ENA						1	//# enable
#define DIS						0	//# disable

#ifndef KB
#define KB						1024
#endif
#ifndef MB
#define MB						(KB*KB)
#endif

#define MAX_STR_LEN				128		//# string length

typedef void * (*OSA_ThrEntryFunc)(void *);

typedef struct {
	pthread_t hndl;
  
} OSA_ThrHndl;

typedef struct {
	Uint32 count;
	Uint32 maxCount;
	pthread_mutex_t lock;
	pthread_cond_t  cond;

} OSA_SemHndl;

typedef struct {
	OSA_ThrHndl thr;
	OSA_SemHndl sem;
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
	APP_CMD_EVT,
	APP_CMD_SOS,
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

	MAX_APP_CMD

} app_cmd_e;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
void delay_msecs(unsigned int msecs);
int event_wait(app_thr_obj *tObj);
int event_send(app_thr_obj *tObj, int cmd, int prm0, int prm1);
int thread_create(app_thr_obj *tObj, void *fxn, int pri, void *prm);\
void thread_delete(app_thr_obj *tObj);

int Msg_Init(int msgKey);
int Msg_Kill(int qid);
int Msg_Send(int qid, void *pdata, int size);
int Msg_Rsv(int qid, int msg_type, void *pdata , int size);
int Msg_Send_Rsv(int qid, int msg_type, void *pdata , int size);

#endif	/* __APP_MSG_H__ */
