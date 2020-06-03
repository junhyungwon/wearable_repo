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
typedef struct TAG_GPSPACKET
{
        unsigned short identifier ;
        unsigned short cmd ;
        unsigned short length ;
        unsigned short gps_Enable ;
        unsigned short gps_UTC_Year ; 
        unsigned short gps_UTC_Month ; 
        unsigned short gps_UTC_Day ; 
        unsigned short gps_UTC_Hour ; 
        unsigned short gps_UTC_Min ; 
        unsigned short gps_UTC_Sec ; 
        unsigned short gps_UTC_Msec ; 
		char gps_Speed[4] ;
        char gps_LAT[16] ;
        char gps_LOT[16] ;
        char gps_Dir[16] ;

} GPSPACKET ;
#pragma pack()




/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a     function prototype
-----------------------------------------------------------------------------*/

void gpsdatareq(int, char*, int) ;



#endif // _APP_PROCESS_H
