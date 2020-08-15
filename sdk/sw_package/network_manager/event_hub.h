/*
 * File : event_hub.h
 *
 * Copyright (C) 2020 LF
 *
 */

#ifndef __EVENT_HUB_H__
#define __EVENT_HUB_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int netmgr_event_hub_init(void);
int netmgr_event_hub_exit(void);

int netmgr_event_hub_polldev_noty(int type, int ste);
int netmgr_event_hub_usb2eth_link_status(int status);
int netmgr_event_hub_rndis_link_status(int status);
int netmgr_event_hub_cradle_link_status(int status);

#endif	/* __EVENT_HUB_H__ */
