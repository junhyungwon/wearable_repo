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
#include "app_msg.h"
#include "app_util.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define DBG_HWDIAG
//# error type
#define SOK			(0)
#define EFAIL		(-1)
#define EPARAM		(-2)
#define EINVALID	(-3)
#define EMEM		(-4)
#define EFILE		(-5)

#define ON			1
#define OFF			0
#define ENA			1	//# enable
#define DIS			0	//# disable

#ifndef KB
#define KB	1024
#endif
#ifndef MB
#define MB	(KB*KB)
#endif

#define MAX_STR_LEN		128		//# string length
#define MAX_CHAR_32     32
#define MAX_CHAR_16     16
#define MAX_CHAR_8       8

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
#define app_msleep(ms)		OSA_waitMsecs(ms)
#define app_get_time()		OSA_getCurTimeInMsec()

#ifdef DBG_HWDIAG
#define ERR_HWD(x, ...)	printf(" [hwtest err! ] \033[41m ERR! \033[0m%s: " x, __func__, ##__VA_ARGS__);
#define DBG_HWD(x, ...)	printf(" [hwtest ] %s: " x, __func__, ##__VA_ARGS__);
#else
#define ERR_HWD(x, ...)	
#define DBG_HWD(x, ...)	
#endif

#endif	/* _APP_COMM_H_ */
