/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_process.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *    - 2013.09.23 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
  Defines referenced header files
-----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/param.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/kernel.h>
#include <asm/types.h>
#include <netdb.h>
#include <dirent.h>
#include <sys/stat.h>
#include <app_encrypt.h>
#include <app_process.h>
#include <app_fcommand.h>
#include <app_rawsock.h>
#include <app_sockdef.h>

#include "ti_venc.h"
#include "app_main.h"
#include "app_set.h"
//#include "app_file.h"
#include "app_ctrl.h"
#include "app_cap.h"
#include "app_dev.h"
#include "app_gps.h"

//#include "app_hls.h"
#include "app_process.h"
#include "app_mcu.h"
#include "app_sockio.h"
#include "app_decrypt.h"
#include "app_bcall.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
SYSTEM_INFO SystemInfo ;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
char m_SendBuffer[MAXBUFF] ;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

const int GPSPACKET_SIZE = sizeof(GPSPACKET) ;
const int GPSREQRCV_SIZE = sizeof(GPSREQRCV) ;
const int EVENTPACKET_SIZE = sizeof(EVENTPACKET) ;
const int EVENTREQRCV_SIZE = sizeof(EVENTREQRCV) ;
const int USERAUTHRES_SIZE = sizeof(USERAUTHRES) ;
const int CALLRES_SIZE = sizeof(CALL_RES) ;
const int RCALLRES_SIZE = sizeof(RCALL_RES) ;
const int CALLREQ_SIZE = sizeof(CALL_REQ) ;
const int CALLCLOSE_SIZE = sizeof(CALL_COMMON) ;
const int CALLACCEPT_SIZE = sizeof(CALL_COMMON) ;
/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

void gpsdatareq(int channel, char *data, int len)
{
	int sendlen = 0, i ; 
    socklen_t lon ;
	int valopt = 0 ;

	GPSREQRCV Gpsreqrcv ;

#ifdef NETWORK_DEBUG
    DEBUG_PRI("gpsdatareq packet receive...\n") ;
#endif
	SystemInfo.gps_req[channel] = ON ;

	Gpsreqrcv.identifier = htons(IDENTIFIER) ;
	Gpsreqrcv.cmd = htons(CMD_GPSREQ_RCV) ;
	Gpsreqrcv.length = htons(GPSREQRCV_SIZE) ;

	memcpy(m_SendBuffer, &Gpsreqrcv, GPSREQRCV_SIZE) ;
	for(i = 0 ; i < MAXUSER; i++)
	{
		if(SystemInfo.Channel[i] != 0 )
		{
			getsockopt(SystemInfo.Channel[i], SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) ;
			if(valopt)
			{
				CloseNowChannel(i) ;
			}
			else
			{
				sendlen = send(SystemInfo.Channel[i], m_SendBuffer, GPSREQRCV_SIZE, 0) ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("gpsdata res packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
			}
        }
	}
}


void gpsdata_send(void *data)
{
	app_gps_meta_t *Gpsdata = (app_gps_meta_t *)data;
    int sendlen = 0, i;
    int timezone = 0 ;

    socklen_t lon ;
	int valopt = 0 ;

	timezone = app_set->time_info.time_zone - 12 ;

//   DEBUG_PRI("GPSPACKET_SIZE = %d\n",GPSPACKET_SIZE) ;
//   DEBUG_PRI("GPSDATA_SIZE = %d\n",GPSDATA_SIZE) ;
	GPSPACKET Gpspacket ;

	Gpspacket.identifier = htons(IDENTIFIER) ;
	Gpspacket.cmd = htons(CMD_GPSDATA_RES) ;
	Gpspacket.length = htons(GPSPACKET_SIZE) ;
 
    Gpspacket.gps_Enable = htons(Gpsdata->enable) ;
    Gpspacket.gps_UTC_Year = htons(Gpsdata->gtm.tm_year + 1900);
    Gpspacket.gps_UTC_Month = htons(Gpsdata->gtm.tm_mon + 1) ;
    Gpspacket.gps_UTC_Day = htons(Gpsdata->gtm.tm_mday);
    Gpspacket.gps_UTC_Hour = htons(Gpsdata->gtm.tm_hour + timezone) ;
    Gpspacket.gps_UTC_Min = htons(Gpsdata->gtm.tm_min) ;
    Gpspacket.gps_UTC_Sec = htons(Gpsdata->gtm.tm_sec) ;
    Gpspacket.gps_UTC_Msec = htons(Gpsdata->subsec);
    sprintf(Gpspacket.gps_Speed, "%3.0f", Gpsdata->speed) ;
    sprintf(Gpspacket.gps_LAT, "%10.7f", Gpsdata->lat);
    sprintf(Gpspacket.gps_LOT, "%10.7f", Gpsdata->lot);
    sprintf(Gpspacket.gps_Dir, "%3.2f", Gpsdata->dir);
	sprintf(Gpspacket.deviceid, "%s", app_set->sys_info.deviceId) ;

	memcpy(m_SendBuffer, &Gpspacket, GPSPACKET_SIZE) ;

	for(i = 0 ; i < MAXUSER; i++)
	{
		if(SystemInfo.Channel[i] != 0)
		{
			getsockopt(SystemInfo.Channel[i], SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) ;
			if(valopt)
			{
				CloseNowChannel(i) ;
			}
			else
			{
				sendlen = send(SystemInfo.Channel[i], m_SendBuffer, GPSPACKET_SIZE, 0) ;
#ifdef NETWORK_DEBUG
				DEBUG_PRI("gpsdata packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
			}
		}
    }
}

void eventdatareq(int channel, char *data, int len)
{
	int sendlen = 0, i ;
    socklen_t lon ;
	int valopt = 0 ;

	EVENTREQRCV Eventreqrcv ;
#ifdef NETWORK_DEBUG
//    DEBUG_PRI("eventdatareq packet receive...\n") ;
#endif
	SystemInfo.event_req[channel] = ON ;

	Eventreqrcv.identifier = htons(IDENTIFIER) ;
	Eventreqrcv.cmd = htons(CMD_EVENTREQ_RCV) ;
	Eventreqrcv.length = htons(EVENTREQRCV_SIZE) ;

	memcpy(m_SendBuffer, &Eventreqrcv, EVENTREQRCV_SIZE) ;
	for(i = 0 ; i < MAXUSER; i++)
	{
		if(SystemInfo.Channel[i] != 0 )
		{
			getsockopt(SystemInfo.Channel[i], SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) ;
			if(valopt)
			{
				CloseNowChannel(i) ;
			}
			else
			{
				sendlen = send(SystemInfo.Channel[i], m_SendBuffer, EVENTREQRCV_SIZE, 0) ;
#ifdef NETWORK_DEBUG
//    DEBUG_PRI("eventdatareq res packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
			}
        }
	}
}


void eventdata_send(void)
{
    int sendlen = 0, i;
    socklen_t lon ;
	int valopt = 0 ;
#ifdef NETWORK_DEBUG
//    DEBUG_PRI("Eventdata reached...\n") ;
#endif
    EVENTPACKET Eventpacket ;

	Eventpacket.identifier = htons(IDENTIFIER) ;
	Eventpacket.cmd = htons(CMD_EVENTDATA_RES) ;
	Eventpacket.length = htons(EVENTPACKET_SIZE) ;
 
	sprintf(Eventpacket.uid, "%s", app_set->sys_info.uid) ;
	sprintf(Eventpacket.deviceId, "%s", app_set->sys_info.deviceId) ;

	memcpy(m_SendBuffer, &Eventpacket, EVENTPACKET_SIZE) ;

	for(i = 0 ; i < MAXUSER; i++)
	{
		if(SystemInfo.Channel[i] != 0)
		{
			getsockopt(SystemInfo.Channel[i], SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) ;
			if(valopt)
			{
				CloseNowChannel(i) ;
			}
			else
			{
				sendlen = send(SystemInfo.Channel[i], m_SendBuffer, EVENTPACKET_SIZE, 0) ;
#ifdef NETWORK_DEBUG
//    DEBUG_PRI("Eventdata  packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
			}
		}
    }
}

void stop_sos_send(void)
{
    int sendlen = 0, i;
    socklen_t lon ;
	int valopt = 0 ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("STOP SOSdata reached...\n") ;
#endif
    EVENTPACKET Sospacket ;

	Sospacket.identifier = htons(IDENTIFIER) ;
	Sospacket.cmd = htons(CMD_STOP_SOSDATA) ;
	Sospacket.length = htons(EVENTPACKET_SIZE) ;
 
	sprintf(Sospacket.uid, "%s", app_set->sys_info.uid) ;
	sprintf(Sospacket.deviceId, "%s", app_set->sys_info.deviceId) ;

	memcpy(m_SendBuffer, &Sospacket, EVENTPACKET_SIZE) ;

	for(i = 0 ; i < MAXUSER; i++)
	{
		if(SystemInfo.Channel[i] != 0)
		{
			getsockopt(SystemInfo.Channel[i], SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) ;
			if(valopt)
			{
				CloseNowChannel(i) ;
			}
			else
			{
				sendlen = send(SystemInfo.Channel[i], m_SendBuffer, EVENTPACKET_SIZE, 0) ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("STOP Sosdata  packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
			}
		}
    }
}

void sosdata_send(void)
{
    int sendlen = 0, i;
    socklen_t lon ;
	int valopt = 0 ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("SOSdata reached...\n") ;
#endif
    EVENTPACKET Sospacket ;

	Sospacket.identifier = htons(IDENTIFIER) ;
	Sospacket.cmd = htons(CMD_SOSDATA_RES) ;
	Sospacket.length = htons(EVENTPACKET_SIZE) ;
 
	sprintf(Sospacket.uid, "%s", app_set->sys_info.uid) ;
	sprintf(Sospacket.deviceId, "%s", app_set->sys_info.deviceId) ;

	memcpy(m_SendBuffer, &Sospacket, EVENTPACKET_SIZE) ;

	for(i = 0 ; i < MAXUSER; i++)
	{
		if(SystemInfo.Channel[i] != 0)
		{
			getsockopt(SystemInfo.Channel[i], SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) ;
			if(valopt)
			{
				CloseNowChannel(i) ;
			}
			else
			{
				sendlen = send(SystemInfo.Channel[i], m_SendBuffer, EVENTPACKET_SIZE, 0) ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("Sosdata  packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
			}
		}
    }
}

void userauthreq(int channel, char *data, int len) 
{
	int sendlen = 0, result = 0;
    char rtsp_userid[32] = {0, } ;
	char rtsp_passwd[32] = {0, } ;


#ifdef NETWORK_DEBUG
    DEBUG_PRI("userauthreq reached...\n") ;
#endif
	USERAUTHREQ *Userauthreq ;
    USERAUTHRES Userauthres ;

    Userauthreq = (USERAUTHREQ *)data ;

	if(app_set->account_info.enctype == ntohs(Userauthreq->encrypt_value))
    {
		if(app_set->account_info.enctype)
		{
			decrypt_aes(app_set->account_info.rtsp_userid, rtsp_userid, 32) ;
			decrypt_aes(app_set->account_info.rtsp_passwd, rtsp_passwd, 32) ;
		}
		else
		{
			strncpy(rtsp_userid, app_set->account_info.rtsp_userid, 32) ; 
			strncpy(rtsp_passwd, app_set->account_info.rtsp_passwd, 32) ; 
		}

#ifdef NETWORK_DEBUG
DEBUG_PRI("Userauth req Userauthreq->id = %s, passwd = %s\n",Userauthreq->id, Userauthreq->passwd) ;
DEBUG_PRI(" Userid = %s, passwd = %s\n",rtsp_userid, rtsp_passwd) ;
#endif

		result = strcmp(rtsp_userid, Userauthreq->id) ;
		result += strcmp(rtsp_passwd, Userauthreq->passwd) ;
		if(result)
			result = FALSE ;
		else
			result = TRUE ;
	}
	else
	{
		result = 2 ;  // different encrypt value between device and nexx manager
	}

#ifdef NETWORK_DEBUG
DEBUG_PRI("Userauth req result = %d\n",result) ;
#endif

	Userauthres.identifier = htons(IDENTIFIER) ;
	Userauthres.cmd = htons(CMD_USERAUTH_RES) ;
	Userauthres.length = htons(USERAUTHRES_SIZE) ;
	Userauthres.result = htons(result) ;

	memcpy(m_SendBuffer, &Userauthres, USERAUTHRES_SIZE) ;

	if(SystemInfo.Channel[channel] != 0)
	{
        sendlen = send(SystemInfo.Channel[channel], m_SendBuffer, USERAUTHRES_SIZE, 0) ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("Userauthres packet sendlen = %d, channel = %d\n",sendlen, channel) ;
#endif
	}
}

int getconnection_list()
{
	int i, retval = 0;
	DEBUG_PRI("getconnection_list called\n") ;
	for(i = 0; i < MAXUSER; i++)
	{
		if(SystemInfo.Channel[i] !=0)
		{
			retval = TRUE ;
		}
	}
	DEBUG_PRI("getconnection_list retval = %d\n",retval) ;

	return retval ;
}

int getconnection_status(int channel)
{
	int i, retval = 0;
	DEBUG_PRI("getconnection_status channel = %d\n",channel) ;
	if(channel)
	{
		for(i = 0; i < MAXUSER; i++)
		{
			if(SystemInfo.call_status[i])
			{
				if(i == channel)
					retval = TRUE ;
			}
		}
	}
	else  //check all connection 
	{
		for(i = 0; i < MAXUSER; i++)
		{
			if(SystemInfo.call_status[i] == 1)
			{
				retval = TRUE ;
			}
		}
	}
	DEBUG_PRI("getconnection_status retval = %d\n",retval) ;

	return retval ;
}

void recv_call_req(int channel, char *data, int len)
{
#ifdef NETWORK_DEBUG
DEBUG_PRI("call req packet reached\n") ;
#endif
	CALL_RES Call_res ;
	int status = 0, sendlen = 0 ;

	status = get_calling_state() ;  // NEXX STATUS

#if defined(NEXXONE) || defined(NEXX360W_CCTV) || defined(NEXX360W_CCTV_SA)
    if(app_set->voip.ON_OFF)
	{
		Call_res.result = htons(NOT_SUPPORTED_PROTOCOL) ;
    	Call_res.identifier = htons(IDENTIFIER) ;
		Call_res.cmd = htons(CMD_CALL_RES) ;
		Call_res.length = htons(CALLRES_SIZE) ;
		memset(Call_res.Reserved, 0x00, 32) ;

		memcpy(m_SendBuffer, &Call_res, CALLRES_SIZE) ;
		if(!getconnection_status(0))  
			SystemInfo.call_status[channel] = TRUE ;

		if(SystemInfo.Channel[channel] != 0)
		{
	        sendlen = send(SystemInfo.Channel[channel], m_SendBuffer, CALLRES_SIZE, 0) ;
#ifdef NETWORK_DEBUG
	 	   	DEBUG_PRI("Callres Not supported protocol packet sendlen = %d, channel = %d\n",sendlen, channel) ;
#endif
		}
		return ;
	}
/*
#if defined(NEXX360W_CCTV)
    if(status == APP_STATE_NONE )
	{
        Call_res.identifier = htons(IDENTIFIER) ;
		Call_res.cmd = htons(CMD_CALL_RES) ;
		Call_res.length = htons(CALLRES_SIZE) ;
//		Call_res.result = htons(CALL_CONNECT_ESTABLISHED) ;
		if(getconnection_status(channel))
			Call_res.result = htons(CALL_CONNECT_ESTABLISHED) ;
		else
			Call_res.result = htons(CALL_CONNECT_FAIL) ;
		memset(Call_res.Reserved, 0x00, 32) ;

		memcpy(m_SendBuffer, &Call_res, CALLRES_SIZE) ;
//		set_calling_state(APP_STATE_CALLING) ;

		if(SystemInfo.Channel[channel] != 0)
		{
 	       sendlen = send(SystemInfo.Channel[channel], m_SendBuffer, CALLRES_SIZE, 0) ;
#ifdef NETWORK_DEBUG
 	   DEBUG_PRI("Callres ACCEPT packet sendlen = %d, channel = %d\n",sendlen, channel) ;
#endif
		}
	}
#else
*/
    if(status == APP_STATE_NONE )
	{
		app_incoming_call() ;

        Call_res.identifier = htons(IDENTIFIER) ;
		Call_res.cmd = htons(CMD_CALL_RES) ;
		Call_res.length = htons(CALLRES_SIZE) ;
		Call_res.result = htons(CALL_DEFAULT_RES) ;
		memset(Call_res.Reserved, 0x00, 32) ;

		memcpy(m_SendBuffer, &Call_res, CALLRES_SIZE) ;
		if(!getconnection_status(0))  
			SystemInfo.call_status[channel] = TRUE ;

		if(SystemInfo.Channel[channel] != 0)
		{
 	        sendlen = send(SystemInfo.Channel[channel], m_SendBuffer, CALLRES_SIZE, 0) ;
#ifdef NETWORK_DEBUG
 	   DEBUG_PRI("APP_STATE_NONE Callres Default res packet sendlen = %d, channel = %d\n",sendlen, channel) ;
#endif
		}

	}
    else if(status == APP_STATE_INCOMING)
	{
		app_accept_call() ;
        Call_res.identifier = htons(IDENTIFIER) ;
		Call_res.cmd = htons(CMD_CALL_RES) ;
		Call_res.length = htons(CALLRES_SIZE) ;
		if(getconnection_status(channel))
			Call_res.result = htons(CALL_DEFAULT_RES) ;
		else
			Call_res.result = htons(CALL_CONNECT_FAIL) ;

		memset(Call_res.Reserved, 0x00, 32) ;

		memcpy(m_SendBuffer, &Call_res, CALLRES_SIZE) ;

		if(SystemInfo.Channel[channel] != 0)
		{
 	       sendlen = send(SystemInfo.Channel[channel], m_SendBuffer, CALLRES_SIZE, 0) ;
#ifdef NETWORK_DEBUG
 	   	    DEBUG_PRI("APP_STATE_INCOMING Callres Default res packet sendlen = %d, channel = %d\n",sendlen, channel) ;
#endif
		}
	}
	else if(status == APP_STATE_ACCEPT)
	{
        Call_res.identifier = htons(IDENTIFIER) ;
		Call_res.cmd = htons(CMD_CALL_RES) ;
		Call_res.length = htons(CALLRES_SIZE) ;
//		Call_res.result = htons(CALL_CONNECT_ESTABLISHED) ;
		if(getconnection_status(channel))
			Call_res.result = htons(CALL_CONNECT_ESTABLISHED) ;
		else
			Call_res.result = htons(CALL_CONNECT_FAIL) ;
		memset(Call_res.Reserved, 0x00, 32) ;

		memcpy(m_SendBuffer, &Call_res, CALLRES_SIZE) ;
//		set_calling_state(APP_STATE_CALLING) ;

		if(SystemInfo.Channel[channel] != 0)
		{
 	       sendlen = send(SystemInfo.Channel[channel], m_SendBuffer, CALLRES_SIZE, 0) ;
#ifdef NETWORK_DEBUG
 	   DEBUG_PRI("Callres ACCEPT packet sendlen = %d, channel = %d\n",sendlen, channel) ;
#endif
		}
	}
	else  // in accept status,  sending fail.  
	{
        Call_res.identifier = htons(IDENTIFIER) ;
		Call_res.cmd = htons(CMD_CALL_RES) ;
		Call_res.length = htons(CALLRES_SIZE) ;
		Call_res.result = htons(CALL_CONNECT_FAIL) ;
		memset(Call_res.Reserved, 0x00, 32) ;

		memcpy(m_SendBuffer, &Call_res, CALLRES_SIZE) ;

		if(SystemInfo.Channel[channel] != 0)
		{
 	       sendlen = send(SystemInfo.Channel[channel], m_SendBuffer, CALLRES_SIZE, 0) ;
#ifdef NETWORK_DEBUG
 	   DEBUG_PRI("Callres FAIL packet sendlen = %d, channel = %d\n",sendlen, channel) ;
#endif
		}

		SystemInfo.call_status[channel] = FALSE ;
	}
//#endif  // except NEXX360W_CCTV 

#else

	Call_res.result = htons(NOT_SUPPORTED_DEVICE) ;
    Call_res.identifier = htons(IDENTIFIER) ;
	Call_res.cmd = htons(CMD_CALL_RES) ;
	Call_res.length = htons(CALLRES_SIZE) ;
	memset(Call_res.Reserved, 0x00, 32) ;

	memcpy(m_SendBuffer, &Call_res, CALLRES_SIZE) ;
	if(!getconnection_status(0))  
		SystemInfo.call_status[channel] = TRUE ;

	if(SystemInfo.Channel[channel] != 0)
	{
    	sendlen = send(SystemInfo.Channel[channel], m_SendBuffer, CALLRES_SIZE, 0) ;
#ifdef NETWORK_DEBUG
		DEBUG_PRI("Callres Not supported device/protocol packet sendlen = %d, channel = %d\n",sendlen, channel) ;
#endif
	
	}
#endif
	    // send result calling..
}

// nexx의 callreq에 대한 응답
void recv_call_res(int channel, char *data, int len)
{
#ifdef NETWORK_DEBUG
DEBUG_PRI("call res packet reached\n") ;
#endif
	RCALL_RES *RCall_res ;
	int status = 0, res_val ;

    RCall_res = (RCALL_RES *)data ;
	res_val = ntohs(RCall_res->result) ;

	switch(res_val)
	{
		case CALL_DEFAULT_RES :
#ifdef NETWORK_DEBUG
			DEBUG_PRI("recv CALL_DEFAULT_RES.....\n") ;
#endif
			status = get_calling_state() ;
			if(status != APP_STATE_NONE)
				set_calling_state(APP_STATE_OUTCOMING) ;
			break ;

		case CALL_CONNECT_ESTABLISHED :
#ifdef NETWORK_DEBUG
			DEBUG_PRI("recv CALL_CONNECT_ESTABLISHED.....\n") ;
#endif
			app_accept_call() ;
			SystemInfo.call_status[channel] = TRUE ;
			break ;

		case CALL_CONNECT_FAIL :
#ifdef NETWORK_DEBUG
			DEBUG_PRI("recv CALL_CONNECT_FAIL.....\n") ;
#endif
			app_close_call() ;
			SystemInfo.call_status[channel] = FALSE ;
			break ;

		case NOT_SUPPORTED_DEVICE :
#ifdef NETWORK_DEBUG
			DEBUG_PRI("recv NOT SUPPORTED DEVICE.....\n") ;
#endif
			app_close_call() ;
			SystemInfo.call_status[channel] = FALSE ;
			break;

		case NOT_SUPPORTED_PROTOCOL :
#ifdef NETWORK_DEBUG
			DEBUG_PRI("recv NOT SUPPORTED PROTOCOL.....\n") ;
#endif
			app_close_call() ;
			SystemInfo.call_status[channel] = FALSE ;
			break ;
	}
}


// nexx manager로 부터 call_close
void recv_call_close(int channel, char *data, int len)
{
#ifdef NETWORK_DEBUG
DEBUG_PRI("call close packet reached\n") ;
#endif
	int status ;

//	status = get_calling_state() ;

//    if(status == APP_STATE_ACCEPT || status == APP_STATE_INCOMING || status == APP_STATE_OUTCOMING)
		app_cancel_call() ;

	SystemInfo.call_status[channel] = FALSE ;
}

int send_call_req()
{
#ifdef NETWORK_DEBUG
DEBUG_PRI("send call req packet reached\n") ;
#endif
    int sendlen = 0, i ;

	CALL_REQ Call_req ;

	if(!getconnection_list()) 
	{
		return FALSE ;
	}

    Call_req.identifier = htons(IDENTIFIER) ;
	Call_req.cmd = htons(CMD_CALL_REQ) ;
	Call_req.length = htons(CALLREQ_SIZE) ;
	sprintf(Call_req.UID, "%s", app_set->sys_info.uid) ;
	sprintf(Call_req.DeviceId, "%s", app_set->sys_info.deviceId) ;
	memset(Call_req.Reserved, 0x00, 32) ;

	memcpy(m_SendBuffer, &Call_req, CALLREQ_SIZE) ;

    for(i = 0 ; i < MAXUSER; i++)
	{
		if(SystemInfo.Channel[i] != 0)
		{
	 	    sendlen = send(SystemInfo.Channel[i], m_SendBuffer, CALLREQ_SIZE, 0) ;
#ifdef NETWORK_DEBUG
		    DEBUG_PRI("Callreq  packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
		}
	}
	return sendlen ; 
}


void send_call_close()
{
#ifdef NETWORK_DEBUG
DEBUG_PRI("send call close packet reached\n") ;
#endif
	int sendlen = 0, i ;

	CALL_COMMON Call_close ;

	Call_close.identifier = htons(IDENTIFIER) ;
	Call_close.cmd = htons(CMD_CALL_CLOSE) ;
	Call_close.length = htons(CALLCLOSE_SIZE) ;

	memcpy(m_SendBuffer, &Call_close, CALLCLOSE_SIZE) ;
	
    for(i = 0 ; i < MAXUSER; i++)
	{
		if(SystemInfo.Channel[i] != 0)
		{
			SystemInfo.call_status[i] = FALSE ;
	 	    sendlen = send(SystemInfo.Channel[i], m_SendBuffer, CALLCLOSE_SIZE, 0) ;
#ifdef NETWORK_DEBUG
		    DEBUG_PRI("Call close  packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
		}
	}
}
