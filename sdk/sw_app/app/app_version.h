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
#	define FITT360_SW_VER      		"2.09.21B"
#	else
#	define FITT360_SW_VER      		"2.09.21.D"
#	endif
#elif defined(NEXX360W)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.21N"
#	else
#	define FITT360_SW_VER      		"2.09.21.D"
#	endif
#elif defined(NEXX360W_MUX)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.21N_MUX"
#	else
#	define FITT360_SW_VER      		"2.09.21.D"
#	endif
#elif defined(NEXXB)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.21"
#	else
#	define FITT360_SW_VER      		"2.09.21.D"
#	endif
#elif defined(NEXX360H)
#	if SW_RELEASE
#	define FITT360_SW_VER      		"2.09.21H"
#	else
#	define FITT360_SW_VER      		"2.09.21D"
#	endif
#elif defined(NEXXONE)


#	if SW_RELEASE
#	define FITT360_SW_VER      		"1.01.21"
#	else
#	define FITT360_SW_VER      		"1.01.21.D"
#	endif
#endif
#endif	/* _APP_VERSION_H_ */
