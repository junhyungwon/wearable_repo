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
#include "app_set.h"
#include "app_comm.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define P2P_SERVER "./bin/P2PTunnelServer_ti"

#define P2P_NAME   "P2PTunnelServer"  // name size of process in /proc/../status is 16
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
    FILE *fp = NULL ;

    if((fp = fopen("/tmp/passwd.txt", "w")) != NULL) 
    { 
        fprintf(fp,"%s\n",app_set->sys_info.p2p_id) ;
        fprintf(fp,"%s\n",app_set->sys_info.p2p_passwd) ;
        fclose(fp) ;
    }

    return 0;
}

int app_p2p_start(void)
{
    int ret = 0 ;
    char p2p_cmd[MAX_CHAR_128] = {0, } ;

    //if(app_cfg->ste.b.wifi || app_cfg->ste.b.dial_run || app_cfg->ste.b.eth1_run || dev_ste_ethernet(0))
	if(app_cfg->ste.b.wifi || app_cfg->ste.b.dial_run || app_cfg->ste.b.eth1_run)
    {
//        sprintf(p2p_cmd, "%s %s &",P2P_SERVER, app_set->sys_info.deviceId) ;
        sprintf(p2p_cmd, "%s %s &",P2P_SERVER, app_set->sys_info.uid) ;
     
        ret = system(p2p_cmd) ;   
    }
    return 0;
}

int app_p2p_stop(void)
{
    int ret = 0 ;
    char p2p_cmd[MAX_CHAR_128] = {0, } ;
    sprintf(p2p_cmd, "killall P2PTunnelServer_ti") ;

    ret = system(p2p_cmd) ;

    return 0;
}

int check_process(char *process_name)
{   
    DIR* pdir;
    struct dirent *pinfo;
    int is_live = 0;

    pdir = opendir("/proc");
    if(pdir == NULL)
    {
        printf("err: NO_DIR\n");
        return 0;
    }

    while(1)
    {
        pinfo = readdir(pdir);
        if(pinfo == NULL)
            break;
 
        if(pinfo->d_type != 4 || pinfo->d_name[0] == '.' || pinfo->d_name[0] > 57)
            continue;

        FILE* fp;
        char buff[128];
        char path[128];

        sprintf(path, "/proc/%s/status", pinfo->d_name);

        fp = fopen(path, "rt");
        if(fp)
        {
            fgets(buff, 128, fp);
            fclose(fp);
        
//            if(strstr(buff, P2P_NAME))   
            if(strstr(buff, process_name))   
            {
                is_live = 1;
                break;
            }
        }
        else
        {
            printf("Can't read file [%s]\n", path);
        }
    }

    closedir(pdir);

    return is_live;
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
    app_msleep(CHECK_MSEC) ;

    while (!exit)
    {
//      #wait cmd 
        cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP) {
            break;
        }
        ret = check_process(P2P_NAME) ;

        if(!ret)
        {
            app_p2p_start() ;
        } 
        app_msleep(CHECK_MSEC) ;
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
    if (thread_create(tObj, THR_p2p, APP_THREAD_PRI, NULL) < 0) {
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
