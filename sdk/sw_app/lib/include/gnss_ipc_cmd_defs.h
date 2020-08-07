/*
 * File : gnss_ipc_cmd_defs.h
 *
 * Copyright (C) 2020 LINKFLOW
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
#ifndef __GNSS_IPC_CMD_DEFS_H__
#define __GNSS_IPC_CMD_DEFS_H__

#if defined (__cplusplus)
extern "C" {
#endif

#include <time.h>

#define GNSS_MSG_KEY					0x155AA
#define GNSS_MSG_TYPE_TO_GPS			1 //# main --> gps process
#define GNSS_MSG_TYPE_TO_MAIN			2 //# gps --> main process

#define GNSS_CMD_GPS_START				(0x600)
#define GNSS_CMD_GPS_STOP				(0x601)
#define GNSS_CMD_GPS_EXIT				(0x602)
#define GNSS_CMD_GPS_DONE				(0x603)
#define GNSS_CMD_GPS_READY				(0x604)
#define GNSS_CMD_GPS_CONFIG				(0x605)
#define GNSS_CMD_GPS_POLL_DATA			(0x606) /* reponse gps data */
#define GNSS_CMD_GPS_DEV_ERR			(0x607) /* device error */
#define GNSS_CMD_GPS_NOTY				(0x608) /* 최초 실행 시 wait_event를 빠져나오기 위해서 */

//# shared memory id
#define GNSS_SHM_KEY					(0x0A20)
/* 대략 100개의 데이터를 버퍼링 */
#define GNSS_SHM_SIZE					(4096)
#define GNSS_FIFO_SIZE					(2048)

/*
 * @brief ipc message buffer type
 */
typedef struct {
	long type;
	int cmd;

	int rate;  				//# gps date rate..
	char dev_name[128];		//# gps device name
} to_gnss_msg_t;

typedef struct {
	long type;
	int cmd;
	
	int status;              //# GPS 상태 전달. (-2 error, -1 off line 0 on-line)
} to_gnss_main_msg_t;

/* ((double)8*5) + ((int)4*8) = 72 byte. */
typedef struct {
	struct tm gtm;			//# GPS time 
	int gps_fixed;			//# 0: invalid, 1:valid
	int view_num;			//# GPS 위성 갯수
	
	double subsec;
	double speed;
	double lat; 			//# latitude : (+)N, (-)S
	double lot; 			//# longitude: (+)E, (-) W
	double dir; 			//# forward direction (degree)

} gnss_shm_data_t;

#if defined (__cplusplus)
}
#endif /* __cplusplus */
#endif /* __GNSS_IPC_CMD_DEFS_H__ */
