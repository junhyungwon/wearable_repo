/*
 * File : sipc_ipc_cmd_defs.h
 *
 * Copyright (C) 2020 LF
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
#ifndef __SIPC_IPC_CMD_DEFS_H__
#define __SIPC_IPC_CMD_DEFS_H__

#if defined (__cplusplus)
extern "C" {
#endif

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define SIPC_MSG_KEY					0x255AA
#define SIPC_MSG_TYPE_TO_CLIENT			1   /* SMAIN - > BARESIP */
#define SIPC_MSG_TYPE_TO_MAIN			2   /* BARESIP -> SMAIN */

#define SIPC_CMD_SIP_EXIT				(0x700) /* 프로세서 종료 */
#define SIPC_CMD_SIP_READY				(0x701)
#define SIPC_CMD_SIP_START				(0x702) /* 전화 걸기 */
#define SIPC_CMD_SIP_STOP				(0x703) /* 전화 종료 */
#define SIPC_CMD_SIP_ANSWER				(0x704) /* Call 수신 */
#define SIPC_CMD_SIP_GET_STATUS			(0x705) /* main--> baresip call Status */
#define SIPC_CMD_SIP_REGISTER_UA		(0x706) /* set user agent account */
#define SIPC_CMD_SIP_UNREGISTER_UA		(0x707) /* unset user agent account */

/* baresip 상태와 동일한 이름으로 정의함 */
#define SIPC_STATE_CALL_IDLE			0
#define SIPC_STATE_CALL_INCOMING		1 /* 전화 수신 대기 중 */
#define SIPC_STATE_CALL_RINGING			2 /* 전화 송신 중 */
#define SIPC_STATE_CALL_ESTABLISHED		3 /* 전화 연결 됨 (송/수신 포함) */
#define SIPC_STATE_CALL_STOP			4 /* 전화 종료. (연결이 안돼서 자동으로 종료되는 것도 포함) */

#define SIPC_DATA_SZ					128

#define SIPC_NET_TYPE_WLAN				1
#define SIPC_NET_TYPE_RNDIS				2
#define SIPC_NET_TYPE_USB2ETH			3

typedef struct {
	int en_stun;					/* enable stun server */
	int net_if;						/* network interface type */
	
	short port;						/* SIP port */
	
	char ua_uri[SIPC_DATA_SZ];     	/* local information. 일반적으로 단말기 번호 */
	char pbx_uri[SIPC_DATA_SZ];    	/* 교환기 주소 */
	char passwd[SIPC_DATA_SZ];     	/* 단말기 등록 비밀번호 */
	char peer_uri[SIPC_DATA_SZ];	/* 연결할 상대 단말 정보 */
	char stun_uri[SIPC_DATA_SZ];    /* stun server address */
	
} sipc_uri_t;

typedef struct {
	int call_ste;    /* call 상태정보 */
	int call_dir;    /* call 방향 (0->incoming, 1->outgoing) */    
	int call_reg; 	 /* pbx 등록 여부 */
	int call_err;    /* call error 404-> 등록되지 않은 사용자, 486 통화중 487 무시, 이외 error */
	
} sipc_status_t;

typedef struct {
	long type;
	int cmd;
	
	sipc_uri_t uri;
	
} to_sipc_msg_t;

typedef struct {
	long type;
	int cmd;
	
	/* baresip 상태를 전달 */
	sipc_status_t ste;
	
} to_sipc_main_msg_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#if defined (__cplusplus)
}
#endif /* __cplusplus */
#endif /* __SIPC_IPC_CMD_DEFS_H__ */
