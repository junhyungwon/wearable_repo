/****************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_udpsock.h
 * @brief
 * @author	hwjun
 * @section	MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_UDPSOCK_H_
#define _APP_UDPSOCK_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

#include <osa.h>
#include <osa_mutex.h>
#include <osa_thr.h>
#include <osa_pipe.h>
#include <osa_sem.h>
#include <osa_que.h>
#include <osa_buf.h>
#include "app_comm.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define HWSOCK_ERROR -1
#define HWTEST_PORT 9100

/*----------------------------------------------------------------------------
 Defines referenced	STRUCTURE
-----------------------------------------------------------------------------*/
#define IDENTIFIER      0x3600
#define CMD_INFO_REQ    0x0001
#define CMD_INFO_RES    0x0002
#define CMD_LED_REQ     0x0003
#define CMD_BUTTON_RES  0x0004
#define CMD_MIC_REQ     0x0005
#define CMD_BUZZER_REQ  0x0006
#define CMD_SERIAL_SET  0x0007
#define CMD_CAMERA_REQ  0x0008
#define CMD_CAMERA_RES  0x0009
#define CMD_UID_SET     0x000A
#define CMD_TIME_SET    0x000B

#define BROADCAST_ADDR "255.255.255.255"
#define PACKETSIZE 8192


#pragma pack(1)
typedef struct TAG_STRUCT_BASE 
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
} STRUCT_BASE ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_INFO_REQ
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
} INFO_REQ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_INFO_RES
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18] ;
    char serialno[16] ;
    char ipaddr[16] ;
    char micom[8] ;
    char hwver[8] ;
    char fwver[16] ;
    unsigned short voltage ;
	unsigned short rtc_status  ;
    unsigned short year;
	unsigned short month;
	unsigned short day;
	unsigned short hour;
	unsigned short min;
	unsigned short sec;
    unsigned short usb_status;
    unsigned short gps_status ;
	char gps_data[32] ;
	char uid[32] ;
} INFO_RES ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_LED_REQ 
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
} LED_REQ ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_BUTTON_RES 
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
	unsigned short dir ;
} BUTTON_RES ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_MIC_REQ
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
} MIC_REQ ;
#pragma pack()


#pragma pack(1)
typedef struct TAG_BUZZER_REQ
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
} BUZZER_REQ ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_SERIAL_SET
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
	char serial_no[16] ;
} SERIAL_SET ;
#pragma pack()


#pragma pack(1)
typedef struct TAG_UID_SET
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
	char uid[32] ;
} UID_SET ;
#pragma pack()


#pragma pack(1)
typedef struct TAG_CAMERA_REQ
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
} CAMERA_REQ ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_CAMERA_RES
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
	unsigned int framesize ;
    unsigned short total_fragment ;
    unsigned short fragno ;
} CAMERA_RES ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_TIME_SET 
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
	unsigned short year ;
	unsigned short month ;
	unsigned short day ;
	unsigned short hour ;
	unsigned short min ;
	unsigned short sec ;
} TIME_SET ;
#pragma packe()


/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_udpsock_init(void);
void app_udpsock_exit(void);
void send_keyPress(int dir) ;
void send_sysinfo(char *data) ;
int GetMacAddress(char *mac_address) ;


#endif	/* _APP_UDPSOCK__H_ */
