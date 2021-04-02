/******************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_onvifserver.h
 * @brief
 * @author  BKKIM
 * @section MODIFY history
 *     - 2018.12.17 : First Created
 */
/*****************************************************************************/

#ifndef _APP_ONVIFSERVER_H_
#define _APP_ONVIFSERVER_H_

#include <stdint.h>
/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define ONVIF_TOKEN_LEN 100
#define MAX_SERVER_PORT 1

typedef struct tagOnvifNetworkInterface
{
	int enabled;
	char token[ONVIF_TOKEN_LEN];	// profile token size 100, eth0 or wlan0 or ...
	int  mtu;
	int  ipv4_flag;
	int  ipv4_enabled;
	int  ipv4_dhcp;
	char ipv4_addr[32];
	int  ipv4_prefixlen;
} T_ONVIF_NETWORK_INTERFACE;

typedef struct
{
	int  	HTTPFlag;								    // Indicates if the http protocol required
	int  	HTTPEnabled;							    // Indicates if the http protocol is enabled or not
	int  	HTTPSFlag;								    // Indicates if the https protocol required
	int  	HTTPSEnabled;							    // Indicates if the https protocol is enabled or not
	int  	RTSPFlag;								    // Indicates if the rtsp protocol required
	int  	RTSPEnabled;							    // Indicates if the rtsp protocol is enabled or not

	int  	HTTPPort[MAX_SERVER_PORT];				    // The port that is used by the protocol
	int  	HTTPSPort[MAX_SERVER_PORT];				    // The port that is used by the protocol
	int  	RTSPPort[MAX_SERVER_PORT];				    // The port that is used by the protocol
} onvif_NetworkProtocol;

typedef struct tagOnvifNetInfo {
	int  dhcp;						// 1:dhcp, else:off
	int  up;						// 1:up, else:???
    char ipaddr[16];
    char netmask[16];
    char gateway[16];
} ONVIF_NET_INFO, T_NETWORK_INFO;

typedef struct tagOnvifUser {
    uint32_t	PasswordFlag    : 1;		    	        // Indicates whether the field Password is valid
	uint32_t	Reserved        : 31;

	int 		fixed;										// used by onvif server
	
	char    UserName[100];	                            // required, User name
	char    Password[100];	                            // optional, optional password

	int     UserLevel;
} ONVIF_USER, T_ONVIF_USER;

//# define enum
#define FILE_UDS_ONVIF  "/tmp/onvif.socket"

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int init_onvifserver();
int app_onvifserver_start(void);
int app_onvifserver_stop(void);
int app_onvifserver_restart(void);
int app_onvifserver_restart_all(void);

#ifdef __cplusplus
}
#endif

#endif	/* _APP_ONVIFSERVER_H_ */
