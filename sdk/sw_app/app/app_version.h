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
#	define FITT360_SW_VER      		"2.09.28B"
#	else
#	define FITT360_SW_VER      		"2.09.28.D"
#	endif
#elif defined(NEXX360C)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.28C"
#	else
#	define FITT360_SW_VER      		"2.09.28.D"
#	endif
#elif defined(NEXX360W)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.28N"
#	else
#	define FITT360_SW_VER      		"2.09.28.D"
#	endif
#elif defined(NEXX360W_MUX)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.28N_MUX"
#	else
#	define FITT360_SW_VER      		"2.09.28.D"
#	endif
#elif defined(NEXXB)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.28"
#	else
#	define FITT360_SW_VER      		"2.09.28.D"
#	endif
#elif defined(NEXXB_ONE)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.28"
#	else
#	define FITT360_SW_VER      		"2.09.28.D"
#	endif
#elif defined(NEXX360H)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.28H"
#	else
#	define FITT360_SW_VER      		"2.09.28D"
#	endif
#elif defined(NEXXONE)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"1.01.28"
#	else
#	define FITT360_SW_VER      		"1.01.28.D"
#	endif
#endif
#endif	/* _APP_VERSION_H_ */
