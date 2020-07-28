/******************************************************************************
 * NEXTT360 Board
 * Copyright by LF, Incoporated. All Rights Reserved.
 * based on gpsd.
 *---------------------------------------------------------------------------*/
 /**
 * @file    main.h
 * @brief
 * @section MODIFY history
 *     - 2020.07.21 : First Created
 */
/*****************************************************************************/

#ifndef __MAIN_H__
#define __MAIN_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include "msg.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/* for debugging macro */
#define __APP_DEBUG__

#define aprintf(x...) do { printf(" [GNSS ] %s: ", __func__); printf(x); } while(0)
#define eprintf(x...) do { printf(" [GNSS ERR!] %s: ", __func__); printf(x); } while(0)

#ifdef __APP_DEBUG__
#define dprintf(x...) do { printf(" [GNSS ] %s: ", __func__); printf(x); } while(0)
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

typedef struct {
	app_thr_obj gObj;		/* GPS thread Handle */
	int shmid;				/* shared memory id */
	int qid;
	
	unsigned char *shmbuf;	/* shared memory buffer address */
	
	int gps_fd;
	int rate;				/* gps 데이터 rate */	
	char dev_name[128];     /* gps 장치명(/dev/ttyOX) */
	
	FIFO *pfifo;
	
} app_gnss_cfg_t;

extern app_gnss_cfg_t *app_cfg;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#endif	/* __MAIN_H__ */
