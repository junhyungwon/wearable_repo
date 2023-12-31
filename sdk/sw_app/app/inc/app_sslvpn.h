/******************************************************************************
 * sslvpn controls
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_sslvpn.h
 * @brief
 * @author	hyung won jun
 * @section	history
 */
/*****************************************************************************/
#ifndef _APP_SSLVPN_H_
#define _APP_SSLVPN_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define SSLVPN_CONF_PATH		"/var/vpn"

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
void app_set_sslvpnconf() ;
int app_sslvpn_start() ;
int app_sslvpn_stop() ;

#endif//_APP_SSLVPN_H_
