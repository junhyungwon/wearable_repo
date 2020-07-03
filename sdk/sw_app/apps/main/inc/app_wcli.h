/******************************************************************************
 * FITT360 Wi-Fi Client
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_wcli.h
 * @brief
 * @author	phoong
 * @section	MODIFY history
 */
/*****************************************************************************/
#ifndef __APP_WCLI_H__
#define __APP_WCLI_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
//# wifi ssid scan module...
#include <ipc_common_def.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
//#define WCLI_DBG

/* Debugging */
#ifdef WCLI_DBG
#define wcli_dbg(fmt, arg...)  fprintf(stdout, "\x1b[34m%s:%d: \x1b[0m " fmt, __FILE__, __LINE__, ## arg)
#else
#define wcli_dbg(fmt, arg...)  do { } while (0)
#endif

typedef enum{
	WCONN_STE_IDLE=0,
	WCONN_STE_CHECKING,		/* Wi-Fi connect check running */
	WCONN_STE_START,		/* Wi-Fi connect first, then connect check running */
	WCONN_STE_MAX
} wconn_ste_e;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_wcli_start(void);
void app_wcli_stop(void);

int app_wcli_req_scan(iwscan_list_t *plist);
int app_wcli_req_link_start(scan_info_t *winfo, char *pwd);
int app_wcli_req_link_stop(void);
int app_wcli_req_wait_for_auth(void);
int app_wcli_req_auth_status(void);
int app_wcli_req_net_status(void);
int app_wcli_req_get_ipaddr(void);

///////////////////////////////////
//*  For wi-fi connect monitor
///////////////////////////////////
int app_wconn_start(void);
void app_wconn_stop(void);
void app_wconn_check_state(int ste);

#endif	/* __APP_WCLI_H__ */
