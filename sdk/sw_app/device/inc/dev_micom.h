/******************************************************************************
 * UCX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_micom.h
 * @brief
 */
/*****************************************************************************/

#ifndef _DEV_MICOM_H_
#define _DEV_MICOM_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

//# must same micom_cmd_e of micom
typedef enum {
	CMD_NONE=0x00,
	CMD_ACK,
	CMD_NACK,

	//# config cmd (host->mic)
	CMD_ACC_CHK=0x10,	//# set acc check time
	CMD_PSW_CHK,		//# set power switch check time
	CMD_WATCHDOG,		//# set watchdog enable/time
	CMD_SYS_WDOG,		//# set system watchdog enable/time

	//# event cmd (mic ->host)
	CMD_DEV_VAL=0x20,	//# volts, temp, acc
	CMD_ACC_EVT,		//# evt acc change state
	CMD_PSW_EVT,		//# press power switch

	//# control cmd (host->mic)
	CMD_HOST_RDY=0x30,	//# ready application
	CMD_HOST_EXIT,		//# exit application
	CMD_DAT_SEND,		//# data send enable/disable
	CMD_ALIVE,			//# watchdog keep alive

	//# control cmd (mic->host)
	CMD_MIC_VER=0x40,	//# mic version, response host ready
	CMD_DAT_STOP,		//# for exit thread of host

	MAX_MIC_CMD
} micom_cmd_e;

typedef enum {
	OFF_NONE=0x00,
	OFF_LOWPWR,			//# off by low power
	OFF_HITEMP,			//# off by high temperature
	OFF_NORMAL,			//# off by switch, acc(dis parking mode)
	OFF_RESET,			//# request reset

	MAX_HOST_OFF
} host_off_e;

typedef struct {
	short mvolt;			//# volt of main power (15V)
	short ibatt;			//# volt of internal batt (8.4V)
	short ebatt;			//# volt of internal batt (12.6V)
} dev_val_t;

//# host <-> mic meassge
#define MAX_DATA_SIZE	8		//# short, currently use 4(dev_val_t)
#define MSG_SYNC_S		0xFF
#define MSG_SYNC_E		0xFE

typedef struct {
	char cmd;		//# command
	char param;		//# param - enable/disable
	short data;
} hst_msg_t;

typedef struct {
	char cmd;
	char param;		//# counter
	short data[MAX_DATA_SIZE];
} mic_msg_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int mic_msg_init(void);
int mic_msg_exit(void);
int mic_send_msg(char cmd, char param, short data);
int mic_recv_msg(char *data, int size);

int mic_data_send(int en, short msec);
int mic_set_watchdog(int en, short sec);
int mic_keep_alive(void);
int mic_exit_state(int state, short data);
short mic_get_version(void);
int mic_send_ready(void);

#endif	/* _DEV_MICOM_H_ */
