/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_ipc_frames.h
 * @brief
 * @author	phoong
 * @section	MODIFY history
 *     - 2013.06.20	: First	Created
 */
/*****************************************************************************/

#ifndef _APP_IPC_FRAMES_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _APP_IPC_FRAMES_H_

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <osa.h>
#include <osa_que.h>
#include <osa_mutex.h>
#include <osa_thr.h>
#include <osa_sem.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#ifndef KB
#define KB		(1024)
#endif
#ifndef MB
#define MB		(KB*KB)
#endif

#define MCFW_MAIN_WAIT_TIME		(.2 * 1000)

#define FRAMSBUF_MAXSIZE		(1280*720*2)

typedef struct {
	char buf[FRAMSBUF_MAXSIZE];
	int size;
	int width;
	int height;
} frame_info_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
Int32 ipcFramesInit(void);
void ipcFramesStop(void);
void ipcFramesExit(void);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	//_APP_IPC_FRAMES_H_
