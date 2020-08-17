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
int netmgr_event_hub_dev_link_status(int type, int status);
int netmgr_event_hub_dev_ip_status(int type);
int netmgr_event_hub_dev_rssi_status(int type, int level);

#endif	/* __EVENT_HUB_H__ */
