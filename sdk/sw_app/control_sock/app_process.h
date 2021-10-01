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

#endif // _APP_PROCESS_H
