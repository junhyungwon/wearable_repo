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
int app_sipc_init(void);
void app_sipc_exit(void);
void app_sipc_set_event(void);

void app_sipc_create_user(const char *info, const char *addr, const char *passwd);
void app_sipc_update_account(const char *login, const char *domain, const char *pass);
void app_sipc_client_exit(void);

#endif	/* __APP_VOIP_H__ */
