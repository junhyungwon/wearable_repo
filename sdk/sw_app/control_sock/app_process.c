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

//#include "app_hls.h"
#include "app_process.h"
#include "app_mcu.h"
#include "app_sockio.h"
#include "app_avi.h"

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
/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/


void gpsdatareq(int channel, char *data, int len)
{
#ifdef NETWORK_DEBUG
    DEBUG_PRI("gpsdatareq packet receive...\n") ;
#endif
    int sendlen = 0, retval = 0 ;
/*
	GPSDATA Gpsdatares ;

    Gpsdatares.identifier = htons(IDENTIFIER) ;
    Gpsdatares.cmd        = htons(CMD_GPSDATA_RES);
    Gpsdatares.length     = htons(GPSDATA_SIZE) ;

    memcpy(m_SendBuffer[channel], &Timesetcfgres, TIMESETCFGRES_SIZE) ;

    sendlen = send(SystemInfo.Channel[channel], m_SendBuffer[channel], TIMESETCFGRES_SIZE, 0) ;
*/

#ifdef NETWORK_DEBUG
    DEBUG_PRI("timesetcfgres packet sendlen = %d\n",sendlen) ;
#endif
}

void gpsdata_send(void *data)
{
    int sendlen = 0, i, speed ;
    int timezone = 0 ;

	timezone = app_set->time_info.time_zone - 12 ;

//    printf("GPSPACKET_SIZE = %d\n",GPSPACKET_SIZE) ;
//    printf("GPSDATA_SIZE = %d\n",GPSDATA_SIZE) ;
    gps_rmc_t *Gpsdata ;
	Gpsdata = (gps_rmc_t *)data ;


	GPSPACKET Gpspacket ;

	Gpspacket.identifier = htons(IDENTIFIER) ;
	Gpspacket.cmd = htons(CMD_GPSDATA_RES) ;
	Gpspacket.length = htons(GPSPACKET_SIZE) ;
 
    Gpspacket.gps_Enable = htons(Gpsdata->enable) ;
    Gpspacket.gps_UTC_Year = htons(Gpsdata->utc_year + 1900);
    Gpspacket.gps_UTC_Month = htons(Gpsdata->utc_month + 1) ;
    Gpspacket.gps_UTC_Day = htons(Gpsdata->utc_mday);
    Gpspacket.gps_UTC_Hour = htons(Gpsdata->utc_hour + timezone) ;
    Gpspacket.gps_UTC_Min = htons(Gpsdata->utc_min) ;
    Gpspacket.gps_UTC_Sec = htons(Gpsdata->utc_sec) ;
    Gpspacket.gps_UTC_Msec = htons(Gpsdata->utc_msec);
    sprintf(Gpspacket.gps_Speed, "%3.0f", Gpsdata->speed) ;
    sprintf(Gpspacket.gps_LAT, "%10.7f", Gpsdata->lat);
    sprintf(Gpspacket.gps_LOT, "%10.7f", Gpsdata->lot);
    sprintf(Gpspacket.gps_Dir, "%3.2f", Gpsdata->dir);
   

	memcpy(m_SendBuffer, &Gpspacket, GPSPACKET_SIZE) ;

	for(i = 0 ; i < MAXUSER; i++)
	{
		if(SystemInfo.Channel[i] != 0)
		{
            sendlen = send(SystemInfo.Channel[i], m_SendBuffer, GPSPACKET_SIZE, 0) ;
#ifdef NETWORK_DEBUG
    DEBUG_PRI("gpsdata res packet sendlen = %d, channel = %d\n",sendlen, i) ;
#endif
		}
    }

}


