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
#define VOIP_STUN_PATH		"stun.l.google.com:19302"

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_voip_init(void);
void app_voip_exit(void);
void app_voip_start(int net_type, int enable_stun, short server_port, const char *uag, const char *server, 
			const char *passwd, const char *peer, const char *stun_server);
void app_voip_stop(void);
void app_voip_event_noty(void);
void app_voip_event_call_close(void);
int app_voip_is_registered(void);

#endif	/* __APP_VOIP_H__ */
