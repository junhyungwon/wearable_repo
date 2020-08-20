/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_voip.h
 * @brief
 * @author	phoong
 * @section	MODIFY history
 */
/*****************************************************************************/
#ifndef __APP_VOIP_H__
#define __APP_VOIP_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_voip_init(void);
void app_voip_exit(void);
void app_voip_stop(void);
void app_voip_event_noty(void);
void app_voip_start(const char *uag, const char *server, const char *passwd, const char *peer);

#endif	/* __APP_VOIP_H__ */
