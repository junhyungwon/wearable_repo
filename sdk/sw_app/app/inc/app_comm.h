/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_comm.h
 * @brief	application common define
 */
/*****************************************************************************/

#ifndef _APP_COMM_H_
#define _APP_COMM_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <syslog.h>
#include "app_msg.h"
#include "app_util.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
//# error type
#define SOK			(0)
#define EFAIL		(-1)
#define EPARAM		(-2)
#define EINVALID	(-3)
#define EMEM		(-4)
#define EFILE		(-5)

//#define ON			1
//#define OFF			0
#define ENA			1	//# enable
#define DIS			0	//# disable

#ifndef KB
#define KB	1024
#endif
#ifndef MB
#define MB	(KB*KB)
#endif

#define SYS_LOG_ENABLE
/*
 * if defined VOIP_CTRL_PWR_KEY -> short power key, volume control
 */  
//#define VOIP_CTRL_PWR_KEY
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
#define app_msleep(ms)		OSA_waitMsecs(ms)
#define app_get_time()		OSA_getCurTimeInMsec()

#define aprintf(x, ...)	printf(" [app ] \033[32m%s: \033[0m" x, __func__, ##__VA_ARGS__);
#define eprintf(x...) do { printf(" [app !err] %s: ", __func__); printf(x); } while(0)

//# mcfw/build/build_config.mk
#ifdef SYS_DEBUG
#define dprintf(x...) do { printf(" [app ] %s: ", __func__); printf(x); } while(0)
#else
#define dprintf(x...)
#endif

#endif	/* _APP_COMM_H_ */
