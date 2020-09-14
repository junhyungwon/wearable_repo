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
#include "app_voip.h"
#include "app_buzz.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define UI_CYCLE_TIME			500	//# ms
#define UI_WATCHDOG_TIME		(UI_CYCLE_TIME*3)

#define UI_STREAMER_TIME		(2000)
#define CNT_STREAMER_CHECK		(UI_STREAMER_TIME/UI_CYCLE_TIME)

/* (60s - 10)*1000 */
#define WD_LOG_TIMEOUT			((TIME_WATCHDOG-10)*1000) //# watcho dog log wrtite time out (ms)
/* 50000 / 1500 = 33 */
#define WD_LOG_CNT				(WD_LOG_TIMEOUT / UI_WATCHDOG_TIME)

#define WD_ENC_NAME				"ENCODER "
#define WD_TMR_NAME				"TIMMER "
#define WD_DEV_NAME				"GPS "
#define WD_FILE_NAME			"FILE_MGR "
#define WD_MICOM_NAME			"MICOM "

#define UI_TVO_WI               720
#define UI_TVO_HE               480

#define UI_LCD_WI               480
#define UI_LCD_HE               800

#define UI_DRAW         		1
#define UI_CLEAR        		0

#define UI_MENU         		0       //# 0: lcd, 1:tv-out

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj uObj;	//# ui thread
    app_thr_obj hObj;	//# change resolution thread
#ifdef OSD_SWVERSION
    app_gfx_t gfx_obj;
#endif
    int start;
	int tmr_cnt;
} app_gui_t;

static app_gui_t t_gui;
static app_gui_t *igui = &t_gui;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

static void get_wd_name(char *name)
{
	if ((app_cfg->wd_tot & WD_ENC) && ((app_cfg->wd_flags & WD_ENC)==0))
		strcat(name, WD_ENC_NAME);
	if ((app_cfg->wd_tot & WD_DEV) && ((app_cfg->wd_flags & WD_DEV)==0))
		strcat(name, WD_DEV_NAME);
	if ((app_cfg->wd_tot & WD_MICOM) && ((app_cfg->wd_flags & WD_MICOM)==0))
		strcat(name, WD_MICOM_NAME);
	if ((app_cfg->wd_tot & WD_FILE) && ((app_cfg->wd_flags & WD_FILE)==0))
		strcat(name, WD_FILE_NAME);
}

#define TVO_GAP_X   40
#define TVO_GAP_Y   25

#define formREC     "NO REC"
#define formGPS     "GPS OFF"
#define formSSID    "000-00000"
#define formTemp    "000`C"
#define formVolt    "00.0V"
#define formGlv     "S-0"
#define formMlv     "M-0"
#define formMIC     "MIC 0"
#define formBUZZER  "BZ 0"
#define SPACE_W     20

#ifdef OSD_SWVERSION

void ui_draw_clear(int dest)
{
    app_gfx_t *gfx = &igui->gfx_obj;

    if(dest == DISP_TVO) {
        draw_fill_color(gfx->tvo.buf, RGB_KEY);
    }
}

void ui_draw_txt_tvo(void)
{
    char version[20];
    char micom_ver[128] = {0, } ;
    ui_pos_t st;
    int str_w=0, str_h=0;

//    ui_draw_clear(DISP_TVO);

    str_h = draw_font_height(FITT360_SW_VER, FNT_SZ01);

    if (app_cfg->ste.b.cap) {
        //# ----------------- Top positon ------------------ //

        st.x = 280;
/*
        if(app_cfg->ste.b.gps){
            draw_text(igui->gfx_obj.tvo.buf, "GPS ON", FNT_SZ01, st.x, TVO_GAP_Y, RGB_WHITE, RGB_BLACK);
        }
        else{
            draw_text(igui->gfx_obj.tvo.buf, "GPS OFF", FNT_SZ01, st.x, TVO_GAP_Y, RGB_WHITE, RGB_BLACK);
        }
*/

        //# ----------------- Bottom positon ---------------- //
        st.y    = UI_TVO_HE - ( str_h + TVO_GAP_Y );

        //# Clear bottom info line

        if(app_cfg->tvo_flag & TVO_VERSION){
            //# Ver. info
            str_w   = draw_string_len( FITT360_SW_VER, FNT_SZ01 );
            str_w   = str_w + draw_string_len( TCX_MODEL, FNT_SZ01 );
            st.x    = UI_TVO_WI - ( str_w + 60 );
            str_w   = draw_string_len(micom_ver, FNT_SZ01);
            st.x    = st.x - str_w;

            sprintf(version, "%s_%s", TCX_MODEL, FITT360_SW_VER);
            draw_text(igui->gfx_obj.tvo.buf, version, FNT_SZ01, st.x, st.y, RGB_WHITE, RGB_BLACK);
        }
    }
}

static void *gui_tvo_init(void)
{
    int buf;

    dev_gfx_init(GFX2, &buf, 720, 480);

    return (void *)buf;
}

static void gui_tvo_exit(void)
{
    dev_gfx_exit(GFX2);
}

#endif

/*****************************************************************************
 * @brief    gui main function
 * @section  DESC Description
 *   - desc
*****************************************************************************/
static void *THR_gui(void *prm)
{
	app_thr_obj *tObj = &igui->uObj;
	int cmd, exit=0;
	int wd_cycle = 0, wd_detect=0, pre_wd = 0;
	char wd_name[MAX_CHAR_64], msg[MAX_CHAR_128];

	aprintf("enter...\n");
	tObj->active = 1;

	while (!exit)
	{
		//# wait cmd
		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP) {
            break;
        }

		//# ------------------- watchdog handler --------------------------------------
		if ((wd_cycle != 0) && (wd_cycle % UI_WATCHDOG_TIME == 0))
		{
			//if (app_cfg->wd_flags == WD_TOT)
			if (app_cfg->wd_flags == app_cfg->wd_tot)
			{
				app_mcu_wd_keep_alive();
				app_cfg->wd_flags = 0;
				wd_detect = 0;
				if(pre_wd)
				{	
				    app_leds_sys_normal_ctrl();
					pre_wd = 0 ;
				    sprintf(msg, " !!! GetOut of WATCHDOG !!!");
				    dprintf("%s\n", msg);
				}
			}
			else {
				wd_detect++;
				memset(wd_name, 0, sizeof(wd_name));
				get_wd_name(wd_name);
				sprintf(msg, " !!! WATCHDOG DETECTED flag[%x,%x]: %s !!!", app_cfg->wd_flags, 
								app_cfg->wd_tot, wd_name);
				dprintf("%s\n", msg);
				if ((wd_detect > 10) && (pre_wd == 0)) {
					/* LED blink */
					app_leds_sys_error_ctrl();
					pre_wd = 1;
				}
				
				if (wd_detect == WD_LOG_CNT)
					app_log_write(MSG_LOG_SHUTDOWN, msg);
			}
		}
		//# ------------------ End of watchdog ----------------------------------------------------
#ifdef OSD_SWVERSION
        if(!app_set->ch[MODEL_CH_NUM].resol)
        {
            if(app_cfg->ste.b.cradle_eth_ready)
            {
                if(!igui->start)
                {
                    igui->gfx_obj.tvo.buf = gui_tvo_init();
                    igui->start = 1  ;
                    ui_draw_clear(DISP_TVO);
                }
                ui_draw_txt_tvo();
            }
            else
            {
                if(igui->start)
                    gui_tvo_exit() ;

                igui->start = 0 ;

            }
        }
#endif
#if 0
		//# wis-stream keep alive....
		if (app_cfg->ste.b.rtsptx) 
		{
			if (igui->tmr_cnt >= CNT_STREAMER_CHECK) {
				igui->tmr_cnt = 0;
				/* check wis-streamer */
				//system("ps -ef | grep defunct | grep -v grep | grep wis-streamer | awk '{print $3}' | xargs kill -9");
				//if (!ctrl_is_live_process((const char *)"wis-streamer"))
				//	app_rtsptx_start();
			} else {
				igui->tmr_cnt++;
			}
		}
#endif
		
		//# ----------------- VOIP Handler -----------------------------------------------------
		if (!app_cfg->ste.b.voip) 
		{
			/* 유선망은 제외 USB 네트워크 */
			if (app_cfg->ste.b.usbnet_run) 
			{
				int network = 0, res;
				
				res = app_netmgr_get_usbnet_dev();
				if (res == NETMGR_DEV_TYPE_WIFI) {
					network = SIPC_NET_TYPE_WLAN;
				} else if (res == NETMGR_DEV_TYPE_USB2ETHER) {
					network = SIPC_NET_TYPE_USB2ETH;
				} else {
					network = SIPC_NET_TYPE_RNDIS;
				}

				/* voip register start */
				app_cfg->ste.b.voip = 1;
				app_voip_start(network, 1, app_set->voip.port, app_set->voip.userid, app_set->voip.ipaddr, 
						app_set->voip.passwd, app_set->voip.peerid, VOIP_STUN_PATH);	
			}
		} else {
			/* voip unregister */
			if (app_cfg->ste.b.usbnet_run == 0) {
				/* 네트워크 연결이 해제되면 재등록을 해야 함 */
				app_cfg->ste.b.voip = 0;
				app_voip_stop();
			}
		}
		
		/* VOIP Buzzer */
		if (app_cfg->ste.b.voip_buzz) {
			app_buzz_ctrl(100, 2);
		}
		//# -------------- End of VOIP ----------------------------------------------------------------
		tObj->cmd = 0;
		wd_cycle += UI_CYCLE_TIME;
		app_msleep(UI_CYCLE_TIME);
	}

#ifdef OSD_SWVERSION
    if(igui->start)
        gui_tvo_exit() ;
#endif

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
		if(app_set->ch[MODEL_CH_NUM].resol == RESOL_480P)     
            ctrl_vid_resolution(RESOL_720P);
		else if(app_set->ch[MODEL_CH_NUM].resol == RESOL_720P)     
            ctrl_vid_resolution(RESOL_1080P);
		else if(app_set->ch[MODEL_CH_NUM].resol == RESOL_1080P)     
            ctrl_vid_resolution(RESOL_480P);

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
    igui->tmr_cnt = 0;
	tObj = &igui->uObj;
    if (thread_create(tObj, THR_gui, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create gui thread\n");
		return EFAIL;
    }
	
    tObj = &igui->hObj;
    if (thread_create(tObj, THR_hdmi, APP_THREAD_PRI, NULL) < 0) {
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

