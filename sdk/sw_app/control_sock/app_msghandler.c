/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_msghandler.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *     - 2013.09.23 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <app_fcommand.h>
#include <app_message.h>
#include <app_msghandler.h>
#include <app_process.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

#define MSG_MAP(msg,func) msgfunc[(msg)]=(func);
#define MSG_MAP_SRV(msg,func) msgfunc_srv[(msg)]=(func);


/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

void (*msgfunc [MAXCOMMANDS])(int channel, char* data, int len) ;
void (*msgfunc_srv [MAXCOMMANDS])(char* data, int len) ;

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

/*****************************************************************************
 * @brief    message init function
 * @section  DESC Description
 *      - desc
 *****************************************************************************/


void msginit (void)
{
    int i ;

//    Intialize the message handler map
    for (i = 0; i < MAXCOMMANDS; i++)
    {
        msgfunc [i] = NULL ;
        msgfunc_srv [i] = NULL ;
    }

    MSG_MAP (CMD_GPSDATA_REQ, gpsdatareq)
}


