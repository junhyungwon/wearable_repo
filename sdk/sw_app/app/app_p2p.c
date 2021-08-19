/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_p2p.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *     - 2019.06.26 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>

#include "app_main.h"
#include "app_ctrl.h"
#include "app_set.h"
#include "app_comm.h"
#include "app_decrypt.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define P2P_SERVER 		"/opt/fit/bin/P2PTunnelServer_ti"

#define P2P_NAME   		"P2PTunnelServer"  // name size of process in /proc/../status is 16
#define CHECK_MSEC        1000

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
    app_thr_obj p2pObj ;
    OSA_MutexHndl       mutex;
} app_p2p_t ;

static app_p2p_t t_p2p;
static app_p2p_t *ip2p=&t_p2p;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
int add_p2p_account()
{
	char rtsp_user[32] = {0} ;
	char rtsp_passwd[32] = {0} ;

    FILE *fp = NULL ;

    if((fp = fopen("/tmp/passwd.txt", "w")) != NULL) 
    { 
#if 0 // RTSP 계정을 공유하기로 함...
        fprintf(fp,"%s\n",app_set->sys_info.p2p_id) ;
        fprintf(fp,"%s\n",app_set->sys_info.p2p_passwd) ;
#endif
	    if(app_set->account_info.enctype)
	   	{
            decrypt_aes(app_set->account_info.rtsp_userid, rtsp_user, 32) ;
            decrypt_aes(app_set->account_info.rtsp_passwd, rtsp_passwd, 32) ;
            fprintf(fp,"%s\n",rtsp_user) ;
            fprintf(fp,"%s\n",rtsp_passwd) ;
		}
		else
		{	
            fprintf(fp,"%s\n",app_set->account_info.rtsp_userid) ;
            fprintf(fp,"%s\n",app_set->account_info.rtsp_passwd) ;
		}
        fclose(fp) ;
    }

    return 0;
}

int app_p2p_start(void)
{
	char p2p_cmd[MAX_CHAR_128] = {0, } ;
    int ret = 0 ;
	char *s = NULL;
	
    if (app_cfg->ste.b.usbnet_run || app_cfg->ste.b.cradle_eth_run)
    {
		int res;
//        sprintf(p2p_cmd, "%s %s &",P2P_SERVER, app_set->sys_info.deviceId) ;
		/* "LFS-LSCS-A1-xxxx" */
		s = strstr(app_set->sys_info.uid, "LFS-LSCS-");
		if (s != NULL) 
		{
			s += 12; /* xxxx */
			ret = sscanf(s, "%d", &res);
			if (ret == 1) 
			{
				if (res >= 1 && res <= 5000) 
				{
#if defined(NEXXONE)
					snprintf(p2p_cmd, sizeof(p2p_cmd), "%s %s %s %s %s &", P2P_SERVER, app_set->sys_info.uid ,"NEXXONE", app_set->sys_info.fw_ver, app_set->sys_info.deviceId);
#elif defined(NEXX360W)
					snprintf(p2p_cmd, sizeof(p2p_cmd), "%s %s %s %s %s &", P2P_SERVER, app_set->sys_info.uid ,"NEXX360W", app_set->sys_info.fw_ver, app_set->sys_info.deviceId);
#elif defined(NEXX360H)
					snprintf(p2p_cmd, sizeof(p2p_cmd), "%s %s %s %s %s &", P2P_SERVER, app_set->sys_info.uid ,"NEXX360H", app_set->sys_info.fw_ver, app_set->sys_info.deviceId);
#elif defined(NEXXB)
					snprintf(p2p_cmd, sizeof(p2p_cmd), "%s %s %s %s %s &", P2P_SERVER, app_set->sys_info.uid ,"NEXXB", app_set->sys_info.fw_ver, app_set->sys_info.deviceId);
#endif
					printf("%s!\n", p2p_cmd) ;
					system(p2p_cmd) ;
					return 0;
				}
			} 
		}
		//eprintf("invalid UID...Couln't start p2p service!!\n");
    }
    return -1;
}

int app_p2p_stop(void)
{
    int ret = 0 ;
    char p2p_cmd[MAX_CHAR_128] = {0, } ;
	
    sprintf(p2p_cmd, "killall -9 %s", P2P_SERVER) ;
	printf("%s !\n", p2p_cmd) ;
    ret = system(p2p_cmd) ;

    return 0;
}

/*****************************************************************************
* @brief    p2p main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_p2p(void *prm)
{
    app_thr_obj *tObj = &ip2p->p2pObj;
    int exit = 0, ret = 0, cmd;

	dprintf("enter...\n");
    tObj->active = 1;
	
    while (!exit)
    {
//      #wait cmd 
        cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP) {
            break;
        }

        ret = ctrl_is_live_process((const char *)P2P_NAME);
        if (!ret) {
            app_p2p_start();
        } 

        app_msleep(CHECK_MSEC);
    }

    tObj->active = 0;
    aprintf("...exit\n");

    return NULL;    
}

/*****************************************************************************
* @brief    p2p thread start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_p2p_init(void)
{
    app_thr_obj *tObj;

    memset(ip2p, 0, sizeof(app_p2p_t));

    tObj = &ip2p->p2pObj;
    if (thread_create(tObj, THR_p2p, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
        eprintf("create p2p thread\n");
        return EFAIL;
    }   
    aprintf(".done!\n");

    return 0;
}

void app_p2p_exit(void)
{
    app_thr_obj *tObj;

    tObj = &ip2p->p2pObj;
    event_send(tObj, APP_CMD_STOP, 0, 0) ;

    while(tObj->active)
        app_msleep(20);

    thread_delete(tObj);

    aprintf(".done!\n") ;
}
