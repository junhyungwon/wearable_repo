/*
 * File : iw_scan_ipc_def.h
 *
 * Copyright (C) 2015 UDWORKs
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * @file msg_def.h
 * @brief Definition of message command, message key, and message type.
 */
#ifndef __IPC_COMMON_DEF_H__
#define __IPC_COMMON_DEF_H__

#if defined (__cplusplus)
extern "C" {
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define IPC_MSG_KEY        	0xc54be5  	//# message key.
#define IPC_MSG_TYPE_1		1           //# main app -> iwscan
#define IPC_MSG_TYPE_2		2           //# iwscan -> main app

#define SHM_KEY				0x0A18		//# shared memory id
#define WIFI_REQUEST_DONE	0xCF		//# working done flag for request.
#define WIFI_CONNECT_DONE	0xD0		//# connect to ap is success.
#define WIFI_CONNECT_FAIL	0xD1		//# connect to ap is failed.

#define MAX_LIST_SIZE		64			//# max wifi ap count
#define CHAR_16				16			//# IPC message length

typedef enum {
	IPC_MSG_QUIT = 0,  	       		//# Let wifi manager shutdown.
	IPC_MSG_WIFI_SCAN, 	       		//# ap scanning from wifi manager.
	IPC_MSG_WIFI_CLI_START,	   		//# connect to selected ap with password.
	IPC_MSG_WIFI_CLI_STOP,	   		//# stop to selected ap.
	IPC_MSG_WIFI_WAIT_FOR_AUTH,     //# Wait Wi-Fi authentication.
	IPC_MSG_WIFI_GET_AUTH_STATUS,   //# Get Wi-Fi authentication status.
	IPC_MSG_WIFI_GET_IP,       		//# Get Wi-Fi address from AP.
	IPC_MSG_WIFI_GET_NET_STATUS,  	//# Get Wi-Fi network status (signal level).
	IPC_MSG_MAX 			   		//# wifi manager command number.

} ipc_wifi_cmd_t;

/*
 * @brief ipc message buffer type
 */
typedef struct {
	long  des; 		///< Where this message go.
	int	  cmd; 		///< Message command ID.
	int	  src; 		///< Where this message from.
	int	  param1;
	int	  param2;
	int	  param3;
} ipc_msg_buf_t;

/*
 * @brief wifi ap information
 */
typedef struct {
	int level;		 /* signal level */
	int en_key; 	 /* encryption mode */
	char ssid[35];
	char pwd[65];
} scan_info_t;

/*
 * @brief wifi ap scan list
 */
typedef struct	{
	int num;		 /* the number of ap list */

	scan_info_t info[MAX_LIST_SIZE];
}iwscan_list_t;

#if defined (__cplusplus)
}
#endif /* __cplusplus */
#endif /* __IPC_COMMON_DEF_H__ */
