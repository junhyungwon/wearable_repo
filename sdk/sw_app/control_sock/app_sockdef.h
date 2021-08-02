/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /*
 * @file        app_sockdef.h
 * @brief
 * @author      hwjun
 * @section     MODIFY history
 */
/*****************************************************************************/
#ifndef _APP_SOCKDEF_H_
#define _APP_SOCKDEF_H_

/*----------------------------------------------------------------------------
 Defines referenced     header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define MAXCOMMANDS     100000
#define MAXUSER         10
#define MAXCHANNEL      10
#define BUFFSIZE        2048
#define MAXBUFF		20490
#define TRUE		1 
#define FALSE		0
#define SOCK_ERROR      -1

//#define NETWORK_DEBUG

#ifdef NETWORK_DEBUG
#define DEBUG_PRI(msg, args...) printf("[NETWORK] - %s(%d):\t%s:" msg, __FILE__, __LINE__, __FUNCTION__, ##args)
#else
#define DEBUG_PRI(msg, args...) ((void)0)
#endif

#define IDENTIFIER      0x2818

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a     function prototype
-----------------------------------------------------------------------------*/

#endif//_APP_SOCKDEF_H_
