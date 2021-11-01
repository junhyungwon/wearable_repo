/******************************************************************************
 * Copyright by	Linkflow, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_rtmp.h
 * @brief
 */
/*****************************************************************************/

#pragma once

#ifdef USE_RTMP

#include <uv.h>
#include <stdbool.h>

#include "srs_librtmp.h"
#include "app_gmem.h"
#include "app_set.h"

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define RTMP_TIMEOUT	(1000*3) // ms

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern uv_loop_t *loop;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_rtmp_start(void);
void app_rtmp_stop(void);

void app_rtmp_publish_video(stream_info_t *ifr);

void app_rtmp_enable(void);
void app_rtmp_disable(void);
void app_rtmp_set_endpoint(const char*);
void app_rtmp_get_endpoint();
#endif // USE_RTMP
