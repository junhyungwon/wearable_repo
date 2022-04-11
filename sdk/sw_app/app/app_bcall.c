/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_bcall.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *     - 2022.03.29 : First Created
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
#include "app_bcall.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

#define CHECK_MSEC        50

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
    app_thr_obj callObj ;
    OSA_MutexHndl       lock;
} app_call_t ;

static app_call_t t_call;
static app_call_t *icall=&t_call;
//static int CALL_STATUS = APP_STATE_NONE;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

int app_incoming_call(void)
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
	printf("app_incoming_call send APP_CMD_CALL_START\n") ;
    event_send(tObj, APP_CMD_CALL_START, 0, 0) ;

    return 0;
}

int app_cancel_call()
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
	printf("app_cancel_call send APP_CMD_CALL_CANCEL\n") ;
    event_send(tObj, APP_CMD_CALL_CANCEL, 0, 0) ;

    return 0;

}

int app_accept_call()  // ACCEPT CALL(nexx)
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
	printf("app_accept_call send APP_CMD_CALL_STOP\n") ;
    event_send(tObj, APP_CMD_CALL_STOP, 0, 0) ;

    return 0;
}

int app_close_call()  // Close CALL (nexx)
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
	printf("app_close_call send APP_CMD_CALL_CLOSE\n") ;
    event_send(tObj, APP_CMD_CALL_CLOSE, 0, 0) ;

    return 0;
}


// nexx --> nexx manager
int app_call_send()
{
	app_thr_obj *tObj ;
	tObj = &icall->callObj;
	printf("app_call_send send APP_CMD_CALL_SEND") ;
	event_send(tObj, APP_CMD_CALL_SEND, 0, 0) ;
}

int get_calling_state()
{
	app_thr_obj *tObj ;
	tObj = &icall->callObj;
    int CALL_STATUS ;

    OSA_mutexLock(&icall->lock) ;
	CALL_STATUS = tObj->param0  ;
    OSA_mutexUnlock(&icall->lock) ;

	return CALL_STATUS ;
}

int set_calling_state(int callvalue)
{
	app_thr_obj *tObj ;
	tObj = &icall->callObj;

    OSA_mutexLock(&icall->lock) ;
	tObj->param0 = callvalue ; 
    OSA_mutexUnlock(&icall->lock) ;
}

/*****************************************************************************
* @brief    backchannel call main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_call(void *prm)
{
    app_thr_obj *tObj = &icall->callObj;
    int exit = 0, ret = 0, cmd, buzzer_interval = 0, buzz_cnt = 0;

	dprintf("enter...\n");
    tObj->active = 1;

    while (!exit)
    {
		cmd = event_wait(tObj) ;
		if(cmd == APP_CMD_EXIT) {
			exit = 1 ;
			break ;
		}
		else if(cmd == APP_CMD_STOP) {
			continue ;
		}

		while(1)
		{
			if (tObj->cmd == APP_CMD_STOP || tObj->cmd == APP_CMD_STOP) {
				break;
	        }
		    if(tObj->cmd == APP_CMD_CALL_START)
			{
//				CALL_STATUS = APP_STATE_INCOMING ;
				set_calling_state(APP_STATE_INCOMING);
				buzzer_interval += 1;
				if(!(buzzer_interval % 20))
				{
					printf("buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
					if(buzz_cnt <= 10 )
					{
						app_buzz_ctrl(100, 2) ;
						buzz_cnt += 1 ;
					}
					else
					{
					printf("111 buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
						buzzer_interval = 0 ;
						buzz_cnt = 0 ;
						tObj->cmd = APP_CMD_NONE ;
//						CALL_STATUS = APP_STATE_NONE ;
						set_calling_state(APP_STATE_NONE);

						app_cfg->stream_enable_audio = app_set->stm_info.enable_audio ;
					}
				}

				if(!(buzzer_interval % 200)) // 10 sec
				{
					printf("222 buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
					buzzer_interval = 0 ;
					buzz_cnt = 0 ;
					tObj->cmd = APP_CMD_NONE ;
					set_calling_state(APP_STATE_NONE);
//					CALL_STATUS = APP_STATE_NONE ;
					app_cfg->stream_enable_audio = app_set->stm_info.enable_audio ;
				}
			}

		    if(tObj->cmd == APP_CMD_CALL_CANCEL)  // from client
			{
				buzzer_interval = 0 ;
				buzz_cnt = 0 ;
				tObj->cmd = APP_CMD_NONE ;
				app_buzz_ctrl(100, 2) ;
//				CALL_STATUS = APP_STATE_NONE ;
				set_calling_state(APP_STATE_NONE);
				app_cfg->stream_enable_audio = app_set->stm_info.enable_audio ;
			}

		    if(tObj->cmd == APP_CMD_CALL_STOP)  // NEXX(accept call)
			{
				printf("APP_CMD_CALL_STOP\n") ;
				buzzer_interval = 0 ;
				buzz_cnt = 0 ;
				tObj->cmd = APP_CMD_NONE ;
				if(!app_cfg->stream_enable_audio && (get_calling_state() == APP_STATE_INCOMING))
				{
					app_cfg->stream_enable_audio = 1 ;
//					CALL_STATUS = APP_STATE_CALLING ;
					set_calling_state(APP_STATE_CALLING);
				}
			}

		    if(tObj->cmd == APP_CMD_CALL_CLOSE)  // NEXX(Close call)
			{
				if(app_cfg->stream_enable_audio && (get_calling_state() == APP_STATE_CALLING || get_calling_state() == APP_STATE_OUTCOMING)) {
					app_buzz_ctrl(100, 2) ;
					app_cfg->stream_enable_audio = 0 ;
//					CALL_STATUS = APP_STATE_NONE ;
					set_calling_state(APP_STATE_NONE);
				}
			}

		    if(tObj->cmd == APP_CMD_CALL_SEND)  // NEXX(send call signal)
			{
				if(get_calling_state() == APP_STATE_NONE)
				{
//					CALL_STATUS = APP_STATE_OUTCOMING ;
					set_calling_state(APP_STATE_OUTCOMING);
					buzzer_interval += 1;
					if(!(buzzer_interval % 20))
					{
						printf("buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
						if(buzz_cnt <= 10 )
						{
							app_buzz_ctrl(100, 2) ;
							send_call_req() ;
							buzz_cnt += 1 ;
						}
						else
						{
							printf("111 buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
							buzzer_interval = 0 ;
							buzz_cnt = 0 ;
							tObj->cmd = APP_CMD_NONE ;
//							CALL_STATUS = APP_STATE_NONE ;
							set_calling_state(APP_STATE_NONE);

							app_cfg->stream_enable_audio = app_set->stm_info.enable_audio ;
						}
					}

					if(!(buzzer_interval % 200)) // 10 sec
					{
						printf("222 buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
						buzzer_interval = 0 ;
						buzz_cnt = 0 ;
						tObj->cmd = APP_CMD_NONE ;
//						CALL_STATUS = APP_STATE_NONE ;
						set_calling_state(APP_STATE_NONE);
						app_cfg->stream_enable_audio = app_set->stm_info.enable_audio ;
					}
				}
			}
            

		    app_msleep(CHECK_MSEC);
//			buzzer_interval += 1;
		}
	}

    tObj->active = 0;
    aprintf("...exit\n");

    return NULL;    
}

/*****************************************************************************
* @brief    backchannel audio call control thread start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_call_control_init(void)
{
    app_thr_obj *tObj;

    memset(icall, 0, sizeof(app_call_t));

    tObj = &icall->callObj;
    if (thread_create(tObj, THR_call, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
        eprintf("create backchannel call control thread\n");
        return EFAIL;
    }   
    aprintf(".done!\n");

    return 0;
}

int app_call_control_exit(void)
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
    event_send(tObj, APP_CMD_STOP, 0, 0) ;

    while(tObj->active)
        app_msleep(20);

    thread_delete(tObj);

    aprintf(".done!\n") ;
}
