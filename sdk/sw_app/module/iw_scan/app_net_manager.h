/*
 * File : app_net_manager.h
 *
 * Copyright (C) 2015 UDWORKs
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __APP_NET_MANAGER_H__
#define __APP_NET_MANAGER_H__

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
int app_net_mgr_init(void);
int app_net_mgr_exit(void);
int app_net_mgr_cli_start(void);
int app_net_mgr_cli_stop(void);
int app_net_mgr_cli_wait_for_auth(void);
int app_net_mgr_cli_auth_status(void);
int app_net_mgr_cli_net_status(void);
int app_net_mgr_cli_get_ipaddr(void);

#endif	/* __APP_NET_MANAGER_H__ */
