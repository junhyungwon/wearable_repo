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

#include "srs_librtmp.h"
#include "app_gmem.h"
#include "app_set.h"

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern uv_loop_t *loop;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_rtmp_start(void);
void app_rtmp_stop(void);

bool app_rtmp_is_ready(void);
void rtmp_video_async_cb(uv_async_t* async);

void app_rtmp_enable(void);
void app_rtmp_disable(void);
void app_rtmp_set_endpoint(const char*);
const char* app_rtmp_get_endpoint();

#endif // USE_RTMP