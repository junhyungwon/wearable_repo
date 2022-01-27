/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_version.h
 * @brief
 */
/*****************************************************************************/
#ifndef _APP_VERSION_H_
#define _APP_VERSION_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include "board_config.h"
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
// USING LTE(LTE + WIFI) => USE_LTE = 1 & USE_WIRELESS = 1
// USING WIFI(WIFI ONLY) => USE_LTE = 0 & USE_WIRELESS = 1 

#define SW_RELEASE					1
#define TCX_MODEL   				MODEL_NAME

/* Fixed Hardware Version */
#define FITT360_HW_VER				"Rev.0.3" 

#if defined(NEXX360B)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.32B"
#	else
#	define FITT360_SW_VER      		"2.09.32.D"
#	endif
#elif defined(NEXX360C)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.32C"
#	else
#	define FITT360_SW_VER      		"2.09.32.D"
#	endif
#elif defined(NEXX360W)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.32N"
#	else
#	define FITT360_SW_VER      		"2.09.32.D"
#	endif
#elif defined(NEXX360W_MUX)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.32N_MUX"
#	else
#	define FITT360_SW_VER      		"2.09.32.D"
#	endif
#elif defined(NEXXB)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.32"
#	else
#	define FITT360_SW_VER      		"2.09.32.D"
#	endif
#elif defined(NEXXB_ONE)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.32"
#	else
#	define FITT360_SW_VER      		"2.09.32.D"
#	endif
#elif defined(NEXX360H)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.32H"
#	else
#	define FITT360_SW_VER      		"2.09.32D"
#	endif
#elif defined(NEXXONE)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"1.01.32"
#	else
#	define FITT360_SW_VER      		"1.01.32.D"
#	endif
#elif defined(NEXX360W_CCTV)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.32N_CCTV"
#	else
#	define FITT360_SW_VER      		"2.09.32.D"
#	endif
#endif
#endif	/* _APP_VERSION_H_ */
