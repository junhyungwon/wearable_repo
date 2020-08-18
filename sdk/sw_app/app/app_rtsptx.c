/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_rtsptx.c
 * @brief
 * @author  sksung
 * @section MODIFY history
 *     - 2013.08.26 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//#include "app_comm.h"
#include "app_main.h"

#include "app_cap.h"
#include "app_set.h"
#include "app_comm.h"
#include "stream.h"
#include "app_rtsptx.h"


/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define RTSP_STREAMER "./bin/wis-streamer"

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
const char alphabet[] = "abcdefghijklmnopqrstuvwxyz0123456789";

int intN(int n) { return rand() % n; }

char *randomString(int len)
{
    char *rstr = malloc((len + 1) * sizeof(char));
    int i;
    for (i = 0; i < len; i++)
    {
        rstr[i] = alphabet[intN(strlen(alphabet))];
    }
    rstr[len] = '\0';
    return rstr;
}

/*****************************************************************************
 * @brief    rtsptx write function
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
int app_rtsptx_write(void *addr, int offset, int size, int frametype, int strmtype, unsigned int timestamp)
{
#ifdef USE_GMEM
    stream_write(offset, size, frametype, strmtype, timestamp);
#else
    stream_write(addr, size, frametype, strmtype, timestamp);
#endif

    return 0;
}

/*****************************************************************************
* @brief    rtsptx start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_rtsptx_start(void)
{
	STREAM_SET streamSet;
	FILE *fd = NULL;
    char rtsp_cmd[MAX_CHAR_128] = {0, } ;
/*
    while(1)
	{
		app_msleep(10) ;
		if(app_cfg->ste.b.cap)
		{
		    app_msleep(100) ;
		    break ;
		}
	}
*/
#ifdef RTSP_ENCRYPT
    char *rname ;
    srand(time(NULL)) ;
#endif

    if(app_set->ch[MAX_CH_NUM].resol == RESOL_1080P)
    {
	    streamSet.stream_wi[0] = 1920;
	    streamSet.stream_he[0] = 1080;
	    streamSet.stream_wi[1] = 1920;
	    streamSet.stream_he[1] = 1080;
        msg_resol(RESOL_1080P) ;
    }
    if(app_set->ch[MAX_CH_NUM].resol == RESOL_720P)
    {
	    streamSet.stream_wi[0] = 1280;
	    streamSet.stream_he[0] = 720;
	    streamSet.stream_wi[1] = 1280;
	    streamSet.stream_he[1] = 720;
        msg_resol(RESOL_720P) ;
    }
    if(app_set->ch[MAX_CH_NUM].resol == RESOL_480P)
    {
	    streamSet.stream_wi[0] = 720;
	    streamSet.stream_he[0] = 480;
	    streamSet.stream_wi[1] = 720;
	    streamSet.stream_he[1] = 480;
        msg_resol(RESOL_480P) ;
    }

#ifdef RTSP_ENCRYPT
    rname = randomString(5) ;
    strcpy(streamSet.stream_name, rname) ;
    printf("RTSP STREAMER address name = %s\n",rname) ;

    free(rname) ;
#else
    strcpy(streamSet.stream_name, "all") ;
#endif

	if(stream_init(&streamSet) < 0){
        eprintf_rt("rtsptx stream init failed!!!\n");
        return -1;
    }
	sprintf(rtsp_cmd, "%s %d &",RTSP_STREAMER, app_set->net_info.rtsp_port) ;

    fd = popen(rtsp_cmd, "r");
	if(fd == NULL) {
		return -1;
	}
	pclose(fd);

	app_cfg->ste.b.rtsptx = 1 ;
    return 0;
}

int app_rtsptx_stop(void)
{
	FILE *fd = NULL;

	stream_end();

    fd = popen("killall wis-streamer", "r");
	if(fd == NULL)
		return -1;

	pclose(fd);
	app_cfg->ste.b.rtsptx = 0 ;

    return 0;
}

int app_rtsptx_stop_start()
{
    FILE *fd = NULL;
    char rtsp_cmd[MAX_CHAR_64] = {0, } ;

     if(app_set->ch[MAX_CH_NUM].resol == RESOL_1080P)
         msg_resol(RESOL_1080P) ;
     if(app_set->ch[MAX_CH_NUM].resol == RESOL_720P)
         msg_resol(RESOL_720P) ;
     if(app_set->ch[MAX_CH_NUM].resol == RESOL_480P)
         msg_resol(RESOL_480P) ;

    fd = popen("killall wis-streamer", "r");
    if (fd == NULL)
        return -1;

    pclose(fd);
    sleep(1);

    sprintf(rtsp_cmd, "%s %d &",RTSP_STREAMER, app_set->net_info.rtsp_port) ;

    fd = popen(rtsp_cmd, "r");
    if (fd == NULL) {
        return -1;
    }
    pclose(fd);

    return 0 ;
}
