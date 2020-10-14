/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_sockio.c
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
#include <sys/types.h>
//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>

#include <osa.h>
#include <osa_mutex.h>
#include <osa_thr.h>
#include <osa_pipe.h>

#include "app_message.h"
#include "app_msghandler.h"
#include "app_rawsock.h"
#include "app_process.h"
#include "app_msg.h"
#include "app_main.h"
#include "app_set.h"
#include "app_comm.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define SOCKWAIT                30000
#define DDNS_LOOP_INTERVAL           20
#define SOCK_THRFXN_TSK_PRI     (3)

typedef struct {
    app_thr_obj sockObj;     //# csock thread
    app_thr_obj ddnsObj;     //# ddns thread

} app_net_t;

static app_net_t t_net ;
static app_net_t *inet=&t_net ;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
void msg_process(MessageManager* manager)
{
    Message* msg = NULL ;
    int i = 0 ;
    unsigned short cmd  = 0 ;

    for (i=0; i<manager->cnt; i++)
    {
        msg = get_message(manager, i) ;
        cmd = htons((msg->buff[0]&0xff) | ((msg->buff[1]<<8)&0xff00)) ;

#ifdef NETWORK_DEBUG
      DEBUG_PRI("cmd = 0x%x\n", cmd) ;
#endif
        if (msgfunc[cmd])
        {
            (*msgfunc[cmd])(msg->channel, msg->buff+4, msg->len-4) ;
        }
    }
    zero_message_manager(manager) ;
}

void refresh_message()
{
    msg_process(gMessageManager) ;
}

static Void THR_csock(Void *prm)
{
    app_thr_obj *tObj = &inet->sockObj ;
    int ControlMainsock, exit = 0, cmd ;

    msginit () ;

    aprintf("enter....\n") ;
#ifdef NETWORK_DEBUG
      DEBUG_PRI("THR_csock start done..!!\n") ;
#endif
//    CSock_ctrlThrObj *thrObj = (CSock_ctrlThrObj *)prm ;
    tObj->active = 1 ;

    gMessageManager = init_message_manager() ;
    zero_message_manager(gMessageManager) ;
    ControlMainsock = MainSocketListen() ;

    while(!exit)
    {
        cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP) {
            break;
        } 
        ProcessSocket(ControlMainsock) ;
        refresh_message() ;
        usleep(SOCKWAIT) ;
    }
    close(ControlMainsock) ;
    release_message_manager(gMessageManager);

    tObj->active = 0 ;
    aprintf("...exit\n");

}

static Void *THR_ddns(Void *prm)
{
    int exit =0, retval = 0, interval = 0, cmp_interval = 0, cmd ;
    char start_buffer[MAX_CHAR_64] ;
    char stop_buffer[MAX_CHAR_64] ;

    app_thr_obj *tObj = &inet->ddnsObj ;

    while(!app_cfg->ste.b.cap) 
    {
        app_msleep(DDNS_LOOP_INTERVAL);
    }

    tObj->active = 1 ;

    sprintf(start_buffer, "/usr/bin/wget -O - --http-user=%s --http-passwd=%s \"%s?hostname=%s\"",app_set->ddns_info.userId, app_set->ddns_info.passwd, app_set->ddns_info.serveraddr, app_set->ddns_info.hostname) ;

    sprintf(stop_buffer, "killall -9 wget") ;

    if(app_set->ddns_info.interval > 0 && app_set->ddns_info.interval < 60)  // 1min ~ 59Min
        interval = app_set->ddns_info.interval * 60 * 1000 ;
    else
		interval = 60 * 1000 ; // l min

    retval = app_cfg->ste.b.cradle_eth_run;
    
    if (app_cfg->ste.b.usbnet_ready || retval)
    {
        system(start_buffer) ;
    }

    while(!exit)
    {
        cmd = tObj->cmd ;
        if (cmd == APP_CMD_STOP) {
            break;
        } 
 
        if(app_set->ddns_info.ON_OFF)  // change ddns setting in run time 
        {
            retval = app_cfg->ste.b.cradle_eth_run;

            if (app_cfg->ste.b.usbnet_ready || retval)
            {
                if(interval <= cmp_interval )
                {   
                    printf("interval = %d cmp_interval = %d\n",interval, cmp_interval) ;                
                    system(stop_buffer) ;
                    system(start_buffer) ;
                    cmp_interval = 0 ;
                }
            }  
             
            cmp_interval += DDNS_LOOP_INTERVAL ;
            if(interval*2 <= cmp_interval )
                cmp_interval = 0 ;
        }  

        app_msleep(DDNS_LOOP_INTERVAL);//#?????
    }
    system(stop_buffer) ;

    tObj->active = 0 ;
    aprintf("...exit\n");
 
    return NULL ;
}

int CSock_init()
{
    app_thr_obj *tObj ;
   
    memset((void*)inet, 0x00, sizeof(app_net_t)) ;

    // sock thread

    tObj = &inet->sockObj ;
    if(thread_create(tObj, THR_csock, APP_THREAD_PRI, NULL) < 0) 
    {
        eprintf("create sock thread\n");
        return EFAIL;
    }

    // ddns thread
    if(app_set->ddns_info.ON_OFF)
    {
        tObj = &inet->ddnsObj ;
        if(thread_create(tObj, THR_ddns, APP_THREAD_PRI, NULL) < 0) 
        {
            eprintf("create ddns thread\n");
            return EFAIL;
        }
    }

    return 0;
}

void CSock_exit()
{
    app_thr_obj *tObj ;

    tObj = &inet->sockObj ;
    event_send(tObj, APP_CMD_STOP, 0, 0);

    while(tObj->active)
        app_msleep(20);
    thread_delete(tObj);
/*
    if(app_set->ddns_info.ON_OFF)
    {
        tObj = &inet->ddnsObj ;
        event_send(tObj, APP_CMD_STOP, 0, 0);

        while(tObj->active)
            app_msleep(20);

        thread_delete(tObj);
    }
*/
    aprintf("... done\n") ;
}
