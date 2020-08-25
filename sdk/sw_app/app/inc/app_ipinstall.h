/****************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_ipinstall.h
 * @brief
 * @author	hwjun
 * @section	MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_IPINSTALL_H_
#define _APP_IPINSTALL_H_

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

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define IPSOCK_ERROR -1
#define IPINSTALL_PORT 9000

/*----------------------------------------------------------------------------
 Defines referenced	STRUCTURE
-----------------------------------------------------------------------------*/
#define IDENTIFIER   0x3600
#define CMD_INFO_REQ 0x0001
#define CMD_INFO_RES 0x0002
#define CMD_CHANGE_INFO 0x0003
#define BROADCAST_ADDR "255.255.255.255"

typedef struct TAG_INFO_REQ
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char reserved[32] ; 
} INFO_REQ;

#pragma pack(1)
typedef struct TAG_INFO_RES
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    unsigned short eth_type ; 
    char ipaddr[16] ;
    char subnet[16] ;
    char gateway[16] ;
    unsigned short wireless_type ;
    char w_ipaddr[16] ;
    char w_subnet[16] ;
    char w_gateway[16] ;
    char fw_version[32];
    char macaddr[20];
    char deviceid[32] ;
    char uid[32] ;
    char reserved[64] ;
} INFO_RES ;
#pragma pack()

typedef struct TAG_CHANGE_INFO 
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned short length ;
    char macaddr[18];
    unsigned short eth_type ;
    char ipaddr[16] ;
    char subnet[16] ;
    char gateway[16] ;
    unsigned short wireless_type ;
    char w_ipaddr[16] ;
    char w_subnet[16] ;
    char w_gateway[16] ;
    char deviceid[32] ;
    char uid[32];
	char reserved[64];
} CHANGE_INFO ;




/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_ipins_init(void);
void app_ipins_exit(void);

#endif	/* _APP_IPINSTALL_H_ */
