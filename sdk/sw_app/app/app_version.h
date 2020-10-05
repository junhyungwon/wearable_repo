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

#if defined(NEXX360)
#if SW_RELEASE
#if SYS_CONFIG_NET
#	define FITT360_SW_VER      		"2.06.00N"
#else
#	define FITT360_SW_VER      		"2.06.00B"
#endif /* #if SYS_CONFIG_NET */
#else
#	define FITT360_SW_VER			"2.06.00.D"
#endif	/* #if SW_RELEASE */
#elif defined(NEXXONE)
	#define FITT360_SW_VER      	"0.90.06" 	//# alpha version
#endif
#define FITT360_HW_VER				"Rev.0.3"

#endif	/* _APP_VERSION_H_ */
