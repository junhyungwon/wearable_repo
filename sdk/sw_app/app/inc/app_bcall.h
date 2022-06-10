/******************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_bcall.h
 * @brief
 * @author	hwjun
 * @section	MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_BCALL_H_
#define _APP_BCALL_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
void app_incoming_call(void);
void app_cancel_call(void);
void app_accept_call(void);
void app_close_call(void);
void app_call_send(void) ;
int get_calling_state(void);
void set_calling_state(int) ;
int app_call_control_init(void);
int app_call_control_exit(void);

#endif	/* _APP_BCALL_H_ */
