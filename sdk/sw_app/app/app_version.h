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

#define EN_WIFI_CLIENT         1

//#define USE_LTE                 1       // LTE -> LTE + WIFI

#define USE_WIRELESS            MODEL_TYPE      // LTE + WIFI
#define SW_RELEASE				1

#define TCX_MODEL   MODEL_NAME

#if SW_RELEASE
    #if USE_WIRELESS
        #define FITT360_SW_VER      "2.04.00Nr"
    #else
        #define FITT360_SW_VER	    "2.04.00Br"
    #endif
#else
#define FITT360_SW_VER			"2.04.00.Dr"
#endif

#define FITT360_HW_VER			"Rev.0.3"

#endif	/* _APP_VERSION_H_ */
