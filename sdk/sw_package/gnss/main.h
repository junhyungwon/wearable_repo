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
#include "nmea_parse.h"

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
	int shmid;				/* shared memory id */
	int qid;
	
	unsigned char *shmbuf;	/* shared memory buffer address */
	
	int gps_fd;
	int rate;				/* gps 데이터 rate */	
	char dev_name[128];     /* gps 장치명(/dev/ttyOX) */
	
	struct gps_device_t t_device;
	struct gps_context_t t_context;

} app_gnss_cfg_t;

extern app_gnss_cfg_t *app_cfg;
extern struct gps_device_t *session;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int send_msg(int cmd);
int recv_msg(void);

#endif	/* __MAIN_H__ */