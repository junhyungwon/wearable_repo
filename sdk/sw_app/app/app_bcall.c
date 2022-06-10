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
#include "app_buzz.h"
#include "app_process.h"
#include "app_bcall.h"
#include "app_snd_play.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define BACKCHANNEL_DEBUG  
#define CHECK_MSEC        50

#ifdef BACKCHANNEL_DEBUG
#define DEBUG_PRI(msg, args...) printf("[BACKCHANNEL] - %s(%d):\t%s:" msg, __FILE__, __LINE__, __FUNCTION__, ##args)
#else
#define DEBUG_PRI(msg, args...) ((void)0)
#endif

#define  RING_WAV            "/usr/baresip/ring.wav"
#define  AUDIO_END_WAV       "/usr/baresip/audio_end.wav"
#define  RING_BACK_WAV       "/usr/baresip/ringback.wav"


/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
    app_thr_obj callObj ;
	int status ;
	long sec ;
    OSA_MutexHndl       lock;
} app_call_t ;

static app_call_t t_call;
static app_call_t *icall=&t_call;
//static int CALL_STATUS = APP_STATE_NONE;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

void app_incoming_call(void)
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
#ifdef BACKCHANNEL_DEBUG
	DEBUG_PRI("app_incoming_call send APP_CMD_CALL_START\n") ;
#endif
    event_send(tObj, APP_CMD_CALL_START, 0, 0) ;
/*
#if SYS_CONFIG_SND_OUT
	app_snd_iplay_start(RING_WAV, 30) ;
#endif
*/
    return ;
}

void app_cancel_call()
{
    app_thr_obj *tObj;
	int status ;

    tObj = &icall->callObj;
#ifdef BACKCHANNEL_DEBUG
	DEBUG_PRI("app_cancel_call send APP_CMD_CALL_CANCEL\n") ;
#endif
    status = get_calling_state() ;

    if(status == APP_STATE_INCOMING)
    	event_send(tObj, APP_CMD_CALL_CANCEL, 1, 0) ; 
	else
    	event_send(tObj, APP_CMD_CALL_CANCEL, 0, 0) ;  // communicate, none, outcoming...
/*
#if SYS_CONFIG_SND_OUT
	app_snd_iplay_stop() ;
	app_snd_iplay_start(AUDIO_END_WAV, 1) ;
#endif
*/
    return ;
}

void app_accept_call()  // ACCEPT CALL(nexx)
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
#ifdef BACKCHANNEL_DEBUG
	DEBUG_PRI("app_accept_call send APP_CMD_CALL_ACCEPT\n") ;
#endif
    event_send(tObj, APP_CMD_CALL_ACCEPT, 0, 0) ;
/*
#if SYS_CONFIG_SND_OUT
	app_snd_iplay_stop() ;
#endif 
*/
    return ;
}

void app_close_call()  // Close CALL (nexx)
{
    app_thr_obj *tObj;

    tObj = &icall->callObj;
#ifdef BACKCHANNEL_DEBUG
	DEBUG_PRI("app_close_call send APP_CMD_CALL_CLOSE\n") ;
#endif
    event_send(tObj, APP_CMD_CALL_CLOSE, 0, 0) ;
/*
#if SYS_CONFIG_SND_OUT
	app_snd_iplay_stop() ;
	app_snd_iplay_start(AUDIO_END_WAV, 1) ;
#endif 
*/
    return;
}

// nexx --> nexx manager
void app_call_send()
{
	app_thr_obj *tObj ;
	tObj = &icall->callObj;
#ifdef BACKCHANNEL_DEBUG
	DEBUG_PRI("app_call_send send APP_CMD_CALL_SEND\n") ;
#endif
	event_send(tObj, APP_CMD_CALL_SEND, 0, 0) ;
/*	
#if SYS_CONFIG_SND_OUT
	app_snd_iplay_start(RING_BACK_WAV, 30) ;
#endif 
*/
    return ;

}

int get_calling_state()
{
	app_thr_obj *tObj ;
	struct timeval tv ;
    long sec = 0 ;

	tObj = &icall->callObj;
    int CALL_STATUS ;

    OSA_mutexLock(&icall->lock) ;
	CALL_STATUS = icall->status  ;
	gettimeofday(&tv, NULL) ;
	sec = tv.tv_sec ; 

	if(CALL_STATUS == APP_STATE_CANCEL)
	{
//  	    if((abs(sec - icall->sec)) > 1)
	    {
			icall->status = APP_STATE_NONE ;
			CALL_STATUS = APP_STATE_NONE ;
    	}
	}


#ifdef BACKCHANNEL_DEBUG
	DEBUG_PRI("get_calling_state CALL_STATUS = %d\n", CALL_STATUS) ;
#endif
    OSA_mutexUnlock(&icall->lock) ;

	return CALL_STATUS ;
}

void set_calling_state(int callvalue)
{
	app_thr_obj *tObj ;
	struct timeval tv ;

	tObj = &icall->callObj;

    OSA_mutexLock(&icall->lock) ;

	icall->status = callvalue ; 
	gettimeofday(&tv, NULL) ;
	icall->sec = tv.tv_sec ;

#ifdef BACKCHANNEL_DEBUG
	DEBUG_PRI("set_calling_state CALL_STATUS = %d\n", icall->status) ;
#endif
    OSA_mutexUnlock(&icall->lock) ;
	
	return ;
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
#ifdef BACKCHANNEL_DEBUG
	DEBUG_PRI("enter...\n");
#endif
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
#ifdef BACKCHANNEL_DEBUG
					DEBUG_PRI("buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
#endif
					if(buzz_cnt <= 20 )
					{
						set_calling_state(APP_STATE_INCOMING);
						app_buzz_ctrl(100, 2) ;
						buzz_cnt += 1 ;
#if SYS_CONFIG_SND_OUT
	app_snd_iplay_start(RING_WAV, 1) ;
#endif
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
				if(tObj->param0)
					set_calling_state(APP_STATE_CANCEL);
				else
					set_calling_state(APP_STATE_NONE);

				app_cfg->stream_enable_audio = app_set->stm_info.enable_audio ;
#if SYS_CONFIG_SND_OUT
//	app_snd_iplay_stop() ;
	app_snd_iplay_start(AUDIO_END_WAV, 1) ;
#endif 
			}

		    if(tObj->cmd == APP_CMD_CALL_ACCEPT)  // NEXX(accept call) & NEXX Manager
			{
#ifdef BACKCHANNEL_DEBUG
				DEBUG_PRI("APP_CMD_CALL_ACCEPT\n") ;
#endif
				call_state = get_calling_state() ;
				buzzer_interval = 0 ;
				buzz_cnt = 0 ;
				tObj->cmd = APP_CMD_NONE ;
#ifdef BACKCHANNEL_DEBUG
				DEBUG_PRI("app_cfg->stream_enable_audio = %d get_calling_state() = %d\n", app_cfg->stream_enable_audio, call_state) ;
#endif
				if(!app_cfg->stream_enable_audio && (call_state == APP_STATE_INCOMING || call_state == APP_STATE_OUTCOMING))
				{
					app_cfg->stream_enable_audio = 1 ;
				}
				set_calling_state(APP_STATE_ACCEPT);
#if SYS_CONFIG_SND_OUT
	app_snd_iplay_stop() ;
#endif 
			}

		    if(tObj->cmd == APP_CMD_CALL_CLOSE)  // NEXX(Close call)
			{
#ifdef BACKCHANNEL_DEBUG
				DEBUG_PRI("APP_CMD_CALL_CLOSE\n") ;
#endif
				call_state = get_calling_state() ;
				if((call_state == APP_STATE_ACCEPT || call_state == APP_STATE_OUTCOMING) || call_state == APP_STATE_INCOMING || call_state == APP_STATE_CANCEL) 
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

#if SYS_CONFIG_SND_OUT
//	app_snd_iplay_stop() ;
	app_snd_iplay_start(AUDIO_END_WAV, 1) ;
#endif 

				}
			}

		    if(tObj->cmd == APP_CMD_CALL_SEND)  // NEXX(send call signal)
			{
				buzzer_interval += 1;
				if(!(buzzer_interval % 20))
				{
					call_state = get_calling_state() ;
					if(call_state == APP_STATE_NONE || call_state == APP_STATE_OUTCOMING  || call_state == APP_STATE_CANCEL )
					{
#ifdef BACKCHANNEL_DEBUG
						DEBUG_PRI("buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
#endif
						if(buzz_cnt <= 20 )
						{
							if(send_call_req())
							{
								set_calling_state(APP_STATE_OUTCOMING);
								app_buzz_ctrl(100, 2) ;
								buzz_cnt += 1 ;
#if SYS_CONFIG_SND_OUT
	app_snd_iplay_start(RING_BACK_WAV, 1) ;
#endif
							}
							else
							{
#ifdef BACKCHANNEL_DEBUG
								DEBUG_PRI("there is no client to send packet\n") ;
#endif
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
#ifdef BACKCHANNEL_DEBUG
						DEBUG_PRI("222 buzz_cnt = %d buzzer_interval = %d\n",buzz_cnt, buzzer_interval) ;
#endif
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
#ifdef BACKCHANNEL_DEBUG
    DEBUG_PRI("...exit\n");
#endif

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
#ifdef BACKCHANNEL_DEBUG
        DEBUG_PRI("create backchannel call control thread\n");
#endif
        return EFAIL;
    }   
#ifdef BACKCHANNEL_DEBUG
    DEBUG_PRI(".done!\n");
#endif

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

#ifdef BACKCHANNEL_DEBUG
    DEBUG_PRI(".done!\n") ;
#endif
	
	return 0;
}
