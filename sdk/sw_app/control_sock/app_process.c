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
#define STR_SIZE 	32
#define FILECNT		100
#define PACKETSIZE	20480

const int GPSPACKET_SIZE = sizeof(GPSPACKET) ;
const int GPSREQRCV_SIZE = sizeof(GPSREQRCV) ;
const int EVENTPACKET_SIZE = sizeof(EVENTPACKET) ;
const int EVENTREQRCV_SIZE = sizeof(EVENTREQRCV) ;
/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

void gpsdatareq(int channel, char *data, int len)
{
	int sendlen = 0, i ; 

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
            sendlen = send(SystemInfo.Channel[i], m_SendBuffer, GPSREQRCV_SIZE, 0) ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("gpsdata res packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
        }
	}
}


void gpsdata_send(void *data)
{
	app_gps_meta_t *Gpsdata = (app_gps_meta_t *)data;
    int sendlen = 0, i;
    int timezone = 0 ;

	timezone = app_set->time_info.time_zone - 12 ;

//    printf("GPSPACKET_SIZE = %d\n",GPSPACKET_SIZE) ;
//    printf("GPSDATA_SIZE = %d\n",GPSDATA_SIZE) ;
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
            sendlen = send(SystemInfo.Channel[i], m_SendBuffer, GPSPACKET_SIZE, 0) ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("gpsdata packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
		}
    }
}

void eventdatareq(int channel, char *data, int len)
{
	int sendlen = 0, i ;

	EVENTREQRCV Eventreqrcv ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("eventdatareq packet receive...\n") ;
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
            sendlen = send(SystemInfo.Channel[i], m_SendBuffer, EVENTREQRCV_SIZE, 0) ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("gpsdata res packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
        }
	}
}


void eventdata_send(void)
{
    int sendlen = 0, i;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("Eventdata reached...\n") ;
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
            sendlen = send(SystemInfo.Channel[i], m_SendBuffer, EVENTPACKET_SIZE, 0) ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("Eventdata  packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
		}
    }
}

void sosdata_send(void)
{
    int sendlen = 0, i;
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
            sendlen = send(SystemInfo.Channel[i], m_SendBuffer, EVENTPACKET_SIZE, 0) ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("Sosdata  packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
		}
    }
}
