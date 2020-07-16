/*
 * File : av_rec_ipc_def.h
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
#ifndef __AV_REC_IPC_DEF_H__
#define __AV_REC_IPC_DEF_H__

#if defined (__cplusplus)
extern "C" {
#endif

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define REC_MSG_KEY						0x1AA55
#define REC_MSG_TYPE_TO_REC				1
#define REC_MSG_TYPE_TO_MAIN			2

/* record command */
#define AV_CMD_REC_START				(0x600)
#define AV_CMD_REC_STOP					(0x601)
#define AV_CMD_REC_EXIT					(0x602)
#define AV_CMD_REC_CANCEL				(0x603)
#define AV_CMD_REC_DONE					(0x604)
#define AV_CMD_REC_READY				(0x605)
#define AV_CMD_REC_FLIST				(0x606)

typedef struct {
	long type;
	int cmd;

	int en_snd; 	//# sound enable
	int en_pre;     //# pre recording
	int fr;  		//# frame rate..
	unsigned int stime;		//# save time
	char deviceId[32];
} to_rec_msg_t;

typedef struct {
	long type;
	int cmd;

	//# file list
	int du;
	char fname[256];
} to_main_msg_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#if defined (__cplusplus)
}
#endif /* __cplusplus */
#endif /* __AV_REC_IPC_DEF_H__ */
