/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /*
 * @file        app_rawsock.h
 * @brief
 * @author      hwjun
 * @section     MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_RAWSOCK_H_
#define _APP_RAWSOCK_H_

/*----------------------------------------------------------------------------
 Defines referenced     header files
-----------------------------------------------------------------------------*/

#include <app_sockdef.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

#define ERRNO 	          -1 
#define INVALID_SOCKET    -1 
#define SERVER_PORT       8401

#pragma pack(1)
typedef struct TAG_SYSTEM_INFO
{
        int Channel[MAXUSER] ;
        int Qposition[MAXUSER] ;
        char QBUFF[MAXUSER][BUFFSIZE] ;
        char Nosignal[MAXCHANNEL] ;
        int framerate ;
        int bitrate ;
        unsigned long Privatekey[MAXUSER] ;
        unsigned long Publickey[MAXUSER] ;
        unsigned long Modnumber[MAXUSER] ;
        int mutex ;
} SYSTEM_INFO;
#pragma pack()

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a     function prototype
-----------------------------------------------------------------------------*/

int MainSocketListen(void) ;
void ProcessSocket(int) ;
void ChangeNonblock(int) ;
void CloseNowChannel(int) ;
void CloseAllChannel(void) ;
int ReadSocketData(int); 
int ReadSocketQue(int, int) ;
int GetQue(int) ;


#endif//_APP_RAWSOCK_H_
