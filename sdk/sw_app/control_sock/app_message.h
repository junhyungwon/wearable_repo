/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file        app_message.h
 * @brief
 * @author      hwjun
 * @section     MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_MESSAGE_H_
#define _APP_MESSAGE_H_

/*----------------------------------------------------------------------------
 Defines referenced     header files
-----------------------------------------------------------------------------*/

#include <app_sockdef.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

typedef struct __Message {
        int channel ;
        char buff [BUFFSIZE] ;
        int len ;
} Message ;

typedef struct __MessageManager {
        int cnt ;
        Message* message [MAXUSER] ;
} MessageManager ;

MessageManager* init_message_manager () ;
void release_message_manager (MessageManager* manager) ;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a     function prototype
-----------------------------------------------------------------------------*/

void zero_message (Message*) ;
void zero_message_manager (MessageManager*) ;
void set_message (MessageManager*, int, char*, int) ;
Message* get_message (MessageManager*, int) ;

extern MessageManager* gMessageManager ;

#endif // _APP_MESSAGE_H_
