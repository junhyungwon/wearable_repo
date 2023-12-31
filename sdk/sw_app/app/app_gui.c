/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_gui.c
 * @brief   device control routine
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "app_comm.h"
#include "app_main.h"
#include "app_mcu.h"
#include "app_gui.h"
#include "draw_text.h"
#include "app_version.h"
#include "dev_gfx.h"

#include "netmgr_ipc_cmd_defs.h"
#include "sipc_ipc_cmd_defs.h"
#include "app_ctrl.h"
#include "app_set.h"
#include "app_rtsptx.h"
#include "app_netmgr.h"
#include "app_buzz.h"
#include "app_voip.h"
#include "app_file.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define UI_CYCLE_TIME			500	//# ms

#define UI_STREAMER_TIME		(2000)
#define CNT_STREAMER_CHECK		(UI_STREAMER_TIME/UI_CYCLE_TIME)

#define UI_VOIP_REG_TIME		(60000) //1min
#define CNT_VOIP_REG_CHECK		(UI_VOIP_REG_TIME/UI_CYCLE_TIME)

#define UI_SD_ERR_TIME			(2000)
#define CNT_SD_ERR_CHECK		(UI_SD_ERR_TIME/UI_CYCLE_TIME)

#define UI_SYS_SHUTDOWN_TIME	(30000) //# 30s
#define CNT_SYS_SHUTDOWN_CHECK	(UI_SYS_SHUTDOWN_TIME/UI_CYCLE_TIME)

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj uObj;	//# ui thread
    app_thr_obj hObj;	//# change resolution thread
    int start;
	int tmr_cnt;
	int voip_tmr;
	int sd_err_tmr;
} app_gui_t;

static app_gui_t t_gui;
static app_gui_t *igui = &t_gui;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
 * @brief    gui main function
 * @section  DESC Description
 *   - desc
*****************************************************************************/
static void *THR_gui(void *prm)
{
	app_thr_obj *tObj = &igui->uObj;
	int cmd, res, exit=0;

	aprintf("enter...\n");
	tObj->active = 1;

	while (!exit)
	{
		//# wait cmd
		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP) {
            break;
        }

#if 1
		if (app_cfg->ste.b.rtsptx) 
		{
			if (igui->tmr_cnt >= CNT_STREAMER_CHECK) {
				igui->tmr_cnt = 0;
				/* check wis-streamer */
//	 	    	system("ps -ef | grep defunct | grep -v grep | grep wis-streamer | awk '{print $3}' | xargs kill -9");
				if (!ctrl_is_live_process((const char *)"wis-streamer"))
					app_rtsptx_start();
			} else {
				igui->tmr_cnt++;
			}
		}
#endif		
//# ----------------- VOIP Handler -----------------------------------------------------
    if(app_set->voip.ON_OFF)
	{
		if (!app_cfg->ste.b.voip) 
		{
			/* 유선망은 제외 USB 네트워크 */
			if (app_cfg->ste.b.usbnet_run) 
			{
				int network = 0;
				
				res = app_netmgr_get_usbnet_dev();
				if (res == NETMGR_DEV_TYPE_WIFI) {
					network = SIPC_NET_TYPE_WLAN;
				} else if (res == NETMGR_DEV_TYPE_USB2ETHER) {
					network = SIPC_NET_TYPE_USB2ETH;
				} else {
					network = SIPC_NET_TYPE_RNDIS;
				}
				
				/* 기본값 651xxxx인 경우 무시 */
				if (strstr(app_set->voip.userid, "xxxx") == NULL) {
					/* voip register start */
					app_cfg->ste.b.voip = 1;
					igui->voip_tmr = 0;
					app_voip_start(network, app_set->voip.use_stun, app_set->voip.port, app_set->voip.userid, 
								app_set->voip.ipaddr, app_set->voip.passwd, app_set->voip.peerid, VOIP_STUN_PATH);
				}
			}
		} else {
			/* checking voip registration */
			res = app_voip_is_registered();
			if (res == 0) 
			{
				/* PBX not registered.. retry */
				if (igui->voip_tmr >= CNT_VOIP_REG_CHECK) {
					dprintf("voip timer expired for registeration.\n");
					igui->voip_tmr = 0; app_cfg->ste.b.voip = 1;
				} else 
					igui->voip_tmr++;
			} else {
				/* voip unregister */
				if (app_cfg->ste.b.usbnet_run == 0) {
					/* 네트워크 연결이 해제되면 재등록을 해야 함 */
					app_cfg->ste.b.voip = 0;
					igui->voip_tmr = 0;
					app_voip_stop();
				}
			}
		}
		
		/* VOIP Buzzer */
		if (app_cfg->ste.b.voip_buzz) {
			app_buzz_ctrl(100, 2, 0);
		}
	}
//# -------------- End of VOIP ----------------------------------------------------------------
//# -------------- Damaged SD Card EVENT Handler-----------------------------------------------
		if (app_cfg->ste.b.mmc_err) {
			igui->sd_err_tmr++;
			if ((igui->sd_err_tmr % CNT_SD_ERR_CHECK) == 0) {
				app_buzz_ctrl(100, 1, 0); /* 5초에 한 번씩 buzzer on */
			} 
			else if (igui->sd_err_tmr >= CNT_SYS_SHUTDOWN_CHECK) {
				/* system off : 정해진 시나리오가 없다. */
				app_file_exit(); /* 파일리스트 갱신 작업이 추가됨 */
				app_set_write();
				app_mcu_pwr_off(OFF_NORMAL);
			}
		}

//# -------------- End of Damaged SD Card EVENT Handler----------------------------------------
		tObj->cmd = 0;
		app_msleep(UI_CYCLE_TIME);
	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*----------------------------------------------------------------------------
 Change HDMI Resolution
-----------------------------------------------------------------------------*/
void change_video_fxn(void)
{
	event_send(&igui->hObj, APP_CMD_NOTY, 0, 0);
}

/*****************************************************************************
 * @brief    gui main function
 * @section  DESC Description
 *   - desc
*****************************************************************************/
static void *THR_hdmi(void *prm)
{
	app_thr_obj *tObj = &igui->hObj;
	int cmd, exit=0;

	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		cmd = event_wait(tObj);
		if(cmd == APP_CMD_EXIT) {
			break;
		}
        //# change resolution 720P <----> 480P <-----> 1080P  toggle.

#if defined(NEXXONE) || defined(NEXXB_ONE) // not support 1080P 
		if(app_set->ch[MODEL_CH_NUM].resol == RESOL_480P)     
            ctrl_vid_resolution(RESOL_720P);
		else    
            ctrl_vid_resolution(RESOL_480P);
#else
		if(app_set->ch[MODEL_CH_NUM].resol == RESOL_480P)     
            ctrl_vid_resolution(RESOL_720P);
		else if(app_set->ch[MODEL_CH_NUM].resol == RESOL_720P)     
            ctrl_vid_resolution(RESOL_1080P);
		else if(app_set->ch[MODEL_CH_NUM].resol == RESOL_1080P)     
            ctrl_vid_resolution(RESOL_480P);
#endif

	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    gui thread start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_gui_init(void)
{
	app_thr_obj *tObj;

	//#--- create thread
    igui->tmr_cnt=0; igui->voip_tmr=0; igui->sd_err_tmr=0;
	tObj = &igui->uObj;
    if (thread_create(tObj, THR_gui, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
    	eprintf("create gui thread\n");
		return EFAIL;
    }
	
    tObj = &igui->hObj;
    if (thread_create(tObj, THR_hdmi, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
    	eprintf("create chagen video output thread\n");
		return EFAIL;
    }

    aprintf("... done!\n");

    return 0;
}

void app_gui_exit(void)
{
	app_thr_obj *tObj;

    tObj = &igui->uObj;
    event_send(tObj, APP_CMD_STOP, 0, 0);

    while(tObj->active)
    	app_msleep(20);
    thread_delete(tObj);

    tObj = &igui->hObj;
    event_send(tObj, APP_CMD_STOP, 0, 0);

    while(tObj->active)
    	app_msleep(20);
    thread_delete(tObj);

	aprintf("... done!\n");
}
