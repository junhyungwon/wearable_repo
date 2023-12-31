/******************************************************************************
 * Copyright by	Linkflow, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_libuv.h
 * @brief
 */
/*****************************************************************************/

#pragma once

#include <uv.h>
/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/* Function error codes */
#define SUCCESS         0
#define FAILURE         -1
#define ERR(fmt, args...) fprintf(stderr, "Encode Error: " fmt, ## args)

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern uv_loop_t *loop;
extern uv_loop_t *loop_video;
extern int* procLoad;
extern int* cpuLoad;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_libuv_start(void);
void app_libuv_stop(void);

int getArmCpuLoad(int *procLoad, int *cpuLoad);