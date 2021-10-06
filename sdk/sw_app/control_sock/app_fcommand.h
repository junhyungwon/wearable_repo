/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /*
 * @file        app_fcommand.h
 * @brief
 * @author      hwjun
 * @section     MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_FCOMMAND_H_
#define _APP_FCOMMAND_H_

/*----------------------------------------------------------------------------
 Defines referenced     header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define CMD_GPSDATA_REQ		(0x1001)
#define CMD_GPSREQ_RCV		(0x1011)
#define CMD_GPSDATA_RES		(0x1002)
#define CMD_EVENTDATA_REQ	(0x1003)
#define CMD_EVENTREQ_RCV	(0x1013)
#define CMD_EVENTDATA_RES	(0x1004)
#define CMD_SOSDATA_RES		(0x1005)
#define CMD_USERAUTH_REQ    (0x1006)
#define CMD_USERAUTH_RES    (0x1007)

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a     function prototype
-----------------------------------------------------------------------------*/

#endif /*  _APP_FCOMMAND_H_ */
