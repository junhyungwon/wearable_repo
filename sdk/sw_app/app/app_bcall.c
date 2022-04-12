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
#define BACKCHANNEL_DEBUG
#define CHECK_MSEC        50

#ifdef BACHCHANNEL_DEBUG
#define DEBUG_PRI(msg, args...) DEBUG_PRI("[BACKCHANNEL] - %s(%d):\t%s:" msg, __FILE__, __LINE__, __FUNCTION__, ##args)
#else
#define DEBUG_PRI(msg, args...) ((void)0)
#endif


/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
    app_thr_obj callObj ;
	int status ;
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
	DEBUG_PRI("app_incoming_call send APP_CMD_CALL_START\n") ;
    event_send(tObj, APP_CMD_CALL_START, 0, 0) ;

    return 0;
}

int app_cancel_call()
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
	DEBUG_PRI("app_cancel_call send APP_CMD_CALL_CANCEL\n") ;
    event_send(tObj, APP_CMD_CALL_CANCEL, 0, 0) ;

    return 0;

}

int app_accept_call()  // ACCEPT CALL(nexx)
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
	DEBUG_PRI("app_accept_call send APP_CMD_CALL_ACCEPT\n") ;
    event_send(tObj, APP_CMD_CALL_ACCEPT, 0, 0) ;

    return 0;
}

int app_close_call()  // Close CALL (nexx)
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
	DEBUG_PRI("app_close_call send APP_CMD_CALL_CLOSE\n") ;
    event_send(tObj, APP_CMD_CALL_CLOSE, 0, 0) ;

    return 0;
}


// nexx --> nexx manager
int app_call_send()
{
	app_thr_obj *tObj ;
	tObj = &icall->callObj;
	DEBUG_PRI("app_call_send send APP_CMD_CALL_SEND") ;
	event_send(tObj, APP_CMD_CALL_SEND, 0, 0) ;
}

int get_calling_state()
{
	app_thr_obj *tObj ;
	tObj = &icall->callObj;
    int CALL_STATUS ;

    OSA_mutexLock(&icall->lock) ;
	CALL_STATUS = icall->status  ;
	DEBUG_PRI("get_calling_state CALL_STATUS = %d\n", CALL_STATUS) ;
    OSA_mutexUnlock(&icall->lock) ;

	return CALL_STATUS ;
}

int set_calling_state(int callvalue)
{
	app_thr_obj *tObj ;
	tObj = &icall->callObj;

    OSA_mutexLock(&icall->lock) ;
	icall->status = callvalue ; 
	DEBUG_PRI("set_calling_state CALL_STATUS = %d\n", icall->status) ;
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
	int call_state = 0 ;
	DEBUG_PRI("enter...\n");
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
		    if(tObj->cmd == APP_CMD_CALL_START)  // From nexx manager
			{
				buzzer_interval += 1;
				if(!(buzzer_interval % 20))
				{
					DEBUG_PRI("buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
					if(buzz_cnt <= 20 )
					{
						set_calling_state(APP_STATE_INCOMING);
						app_buzz_ctrl(100, 2) ;
						buzz_cnt += 1 ;
					}
					else
					{
						buzzer_interval = 0 ;
						buzz_cnt = 0 ;
						tObj->cmd = APP_CMD_NONE ;
						set_calling_state(APP_STATE_NONE);

						app_cfg->stream_enable_audio = app_set->stm_info.enable_audio ;
					}
				}

				if(!(buzzer_interval % 400)) // 20 sec
				{
					buzzer_interval = 0 ;
					buzz_cnt = 0 ;
					tObj->cmd = APP_CMD_NONE ;
					set_calling_state(APP_STATE_NONE);
					app_cfg->stream_enable_audio = app_set->stm_info.enable_audio ;
				}
			}

		    if(tObj->cmd == APP_CMD_CALL_CANCEL)  // from client
			{
				buzzer_interval = 0 ;
				buzz_cnt = 0 ;
				tObj->cmd = APP_CMD_NONE ;
				app_buzz_ctrl(100, 2) ;
				set_calling_state(APP_STATE_NONE);
				app_cfg->stream_enable_audio = app_set->stm_info.enable_audio ;
			}

		    if(tObj->cmd == APP_CMD_CALL_ACCEPT)  // NEXX(accept call) & NEXX Manager
			{
				DEBUG_PRI("APP_CMD_CALL_ACCEPT\n") ;
				call_state = get_calling_state() ;
				buzzer_interval = 0 ;
				buzz_cnt = 0 ;
				tObj->cmd = APP_CMD_NONE ;
				DEBUG_PRI("app_cfg->stream_enable_audio = %d get_calling_state() = %d\n", app_cfg->stream_enable_audio, get_calling_state()) ;
				if(!app_cfg->stream_enable_audio && (call_state == APP_STATE_INCOMING) || (call_state == APP_STATE_OUTCOMING))
				{
					app_cfg->stream_enable_audio = 1 ;
				}
				set_calling_state(APP_STATE_ACCEPT);
			}

		    if(tObj->cmd == APP_CMD_CALL_CLOSE)  // NEXX(Close call)
			{
				DEBUG_PRI("APP_CMD_CALL_CLOSE\n") ;
				call_state = get_calling_state() ;
				if((call_state == APP_STATE_ACCEPT || call_state == APP_STATE_OUTCOMING)) 
				{
//					if(get_calling_state() == APP_STATE_OUTCOMING)
					{
						send_call_close() ;
					}
					app_buzz_ctrl(100, 2) ;
					app_cfg->stream_enable_audio = 0 ;
					buzzer_interval = 0 ;
					buzz_cnt = 0 ;
					tObj->cmd = APP_CMD_NONE ;
					set_calling_state(APP_STATE_NONE);
				}
			}

		    if(tObj->cmd == APP_CMD_CALL_SEND)  // NEXX(send call signal)
			{
				call_state = get_calling_state() ;
				buzzer_interval += 1;
				if(!(buzzer_interval % 20))
				{
					if(call_state == APP_STATE_NONE || call_state == APP_STATE_OUTCOMING)
					{
						DEBUG_PRI("buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
						if(buzz_cnt <= 20 )
						{
							if(send_call_req())
							{
								set_calling_state(APP_STATE_OUTCOMING);
								app_buzz_ctrl(100, 2) ;
								buzz_cnt += 1 ;
							}
							else
							{
								DEBUG_PRI("there is no client to send packet\n") ;
								tObj->cmd = APP_CMD_NONE ;
								set_calling_state(APP_STATE_NONE);
							}
						}
						else
						{
							buzzer_interval = 0 ;
							buzz_cnt = 0 ;
							tObj->cmd = APP_CMD_NONE ;
							send_call_close() ;
							set_calling_state(APP_STATE_NONE);

							app_cfg->stream_enable_audio = app_set->stm_info.enable_audio ;
						}
					}

					if(!(buzzer_interval % 400)) // 20 sec
					{
						DEBUG_PRI("222 buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
						buzzer_interval = 0 ;
						buzz_cnt = 0 ;
						tObj->cmd = APP_CMD_NONE ;
						send_call_close() ;
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
    DEBUG_PRI("...exit\n");

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
        DEBUG_PRI("create backchannel call control thread\n");
        return EFAIL;
    }   
    DEBUG_PRI(".done!\n");

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

    DEBUG_PRI(".done!\n") ;
}
