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
#define __DEBUG__
#define USE_SYSLOGD  (1) //# 0-> disable syslogd

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
#define KB			1024
#endif
#ifndef MB
#define MB			(KB*KB)
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
#define app_msleep(ms)		OSA_waitMsecs(ms)
#define app_get_time()		OSA_getCurTimeInMsec()

/** ANSI printf color **/
/** black \033[30m **/
/** red   \033[31m **/
/** green \033[32m **/
/** yellow\033[33m **/
/** blue  \033[34m **/
/** pink  \033[35m **/
/** teal  \033[36m **/
/** white \033[37m **/

#ifdef __DEBUG__
#define TRACE_INFO(x,...) do { printf(" [app ] \033[32m%s: \033[0m", __func__); printf(x, ##__VA_ARGS__); fflush(stdout); } while(0)
#define TRACE_ERR(x,...) do { printf(" [app err!] \033[31m%s: \033[0m", __func__); printf(x, ##__VA_ARGS__); fflush(stdout); } while(0)

#	if USE_SYSLOGD
#	define LOGD(x...)	do {printf(" [app ] %s: ", __func__); printf(x); fflush(stdout); syslog(LOG_INFO, x);} while(0)
#	define LOGE(x...)	do {printf(" [app err!] %s: ", __func__); printf(x); fflush(stdout); syslog(LOG_ERR, x);} while(0)
#	else
#	define LOGD(x...)	do {printf(" [app ] %s: ", __func__); printf(x); fflush(stdout);} while(0)
#	define LOGE(x...)	do {printf(" [app err!] %s: ", __func__); printf(x); fflush(stdout);} while(0)
#	endif
#else
#define TRACE_INFO(x,...)
#define TRACE_ERR(x,...)

#	define LOGD(x...)	
#	define LOGE(x...)
#endif

#endif	/* _APP_COMM_H_ */
