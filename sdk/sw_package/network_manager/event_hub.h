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

int netmgr_event_hub_dev_status(int type, int ste);
int netmgr_event_hub_link_status(int type, int ste);
int netmgr_event_hub_dhcp_noty(int type);
int netmgr_event_hub_rssi_status(int type, int ste);

#endif	/* __EVENT_HUB_H__ */
