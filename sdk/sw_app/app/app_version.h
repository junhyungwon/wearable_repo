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

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
// USING LTE(LTE + WIFI) => USE_LTE = 1 & USE_WIRELESS = 1
// USING WIFI(WIFI ONLY) => USE_LTE = 0 & USE_WIRELESS = 1 

#define EN_WIFI_CLIENT          1
#define SW_RELEASE				1

#define TCX_MODEL   MODEL_NAME

#if SW_RELEASE
#define FITT360_SW_VER      "2.04.00Ns"
#else
#define FITT360_SW_VER		"2.04.00.Ds"
#endif

#define FITT360_HW_VER		"Rev.0.3"

#endif	/* _APP_VERSION_H_ */
