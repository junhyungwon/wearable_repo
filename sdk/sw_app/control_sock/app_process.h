/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_process.h
 * @brief
 * @author  hwjun
 * @section MODIFY history
 */
/*****************************************************************************/
#ifndef _APP_PROCESS_H_
#define _APP_PROCESS_H_

/*----------------------------------------------------------------------------
  Defines referenced header files
-----------------------------------------------------------------------------*/
#define CALL_DEFAULT_RES			0x00
#define CALL_CONNECT_ESTABLISHED    0x01 
#define CALL_CONNECT_FAIL           0x63 // 99
#define NOT_SUPPORTED_DEVICE        0x64 // 100
#define NOT_SUPPORTED_PROTOCOL		0x65 // 101

/*----------------------------------------------------------------------------
  Definitions and macro
-----------------------------------------------------------------------------*/
#pragma pack(1)
typedef struct TAG_GPSPACKET {
	unsigned short identifier;
	unsigned short cmd;
	unsigned short length;
	unsigned short gps_Enable;
	unsigned short gps_UTC_Year; 
	unsigned short gps_UTC_Month; 
	unsigned short gps_UTC_Day; 
	unsigned short gps_UTC_Hour; 
	unsigned short gps_UTC_Min; 
	unsigned short gps_UTC_Sec; 
	unsigned short gps_UTC_Msec; 
	
	char gps_Speed[4] ;
	char gps_LAT[16] ;
	char gps_LOT[16] ;
	char gps_Dir[16] ;
	char deviceid[32] ;
} GPSPACKET ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_GPSREQ_RCV {
	unsigned short identifier ;
	unsigned short cmd ;
	unsigned short length ;
} GPSREQRCV ;
#pragma pack()


#pragma pack(1)
typedef struct TAG_EVENTPACKET {
	unsigned short identifier ;
	unsigned short cmd ;
	unsigned short length ;
	char uid[32] ;
	char deviceId[32] ;
} EVENTPACKET ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_EVENTREQ_RCV {
	unsigned short identifier ;
	unsigned short cmd ;
	unsigned short length ;
} EVENTREQRCV ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_USERAUTHREQ {
	char id[32] ;
	char passwd[32] ;
	unsigned short encrypt_value ;
} USERAUTHREQ ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_USERAUTHRES {
	unsigned short identifier ;
	unsigned short cmd ;
	unsigned short length ;
	unsigned short result ;
} USERAUTHRES ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_CALL_REQ{
	unsigned short identifier ;
	unsigned short cmd ;
	unsigned short length ;
	char UID[32] ;
	char DeviceId[32] ;
	char Reserved[32] ;
} CALL_REQ ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_CALL_RES{
	unsigned short identifier ;
	unsigned short cmd ;
	unsigned short length ;
	unsigned short result ;
	char Reserved[32] ;
} CALL_RES ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_RCALL_RES{
	unsigned short result ;
	char Reserved[32] ;
} RCALL_RES ;
#pragma pack()

#pragma pack(1)
typedef struct TAG_CALL_COMMON{
	unsigned short identifier ;
	unsigned short cmd ;
	unsigned short length ;
} CALL_COMMON ;
#pragma pack()

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a     function prototype
-----------------------------------------------------------------------------*/
void gpsdatareq(int, char*, int) ;
void eventdatareq(int, char*, int) ;
void gpsdata_send(void *data);
void eventdata_send(void);
void sosdata_send(void);
void userauthreq(int, char*, int) ;
void stop_sos_send(void) ;
void recv_call_req(int, char*, int) ; // nexx manager -> nexx series
void recv_call_res(int, char*, int) ; // nexx manager -> nexx series
void recv_call_close(int, char*, int) ; // nexx manager -> nexx series

int send_call_req() ;
void send_call_close() ;

#endif // _APP_PROCESS_H
