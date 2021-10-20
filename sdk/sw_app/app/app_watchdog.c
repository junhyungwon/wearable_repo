/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_watchdog.c
 * @brief   watchdog control
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "app_comm.h"
#include "app_main.h"
#include "app_mcu.h"
#include "app_leds.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define WD_CYCLE_TIME			500	//# ms
#define WD_TIME					(WD_CYCLE_TIME*3)

/* (60s - 10)*1000 */
#define WD_LOG_TIMEOUT			((TIME_WATCHDOG-10)*1000) //# watcho dog log wrtite time out (ms)
/* 50000 / 1500 = 33 */
#define WD_LOG_CNT				(WD_LOG_TIMEOUT / WD_TIME)

#define WD_ENC_NAME				"ENCODER "
#define WD_DEV_NAME				"GPS "
#define WD_MICOM_NAME			"MICOM "

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj wObj;	//# watchdog thread
} app_watchdog_t;

static app_watchdog_t t_watch;
static app_watchdog_t *iwatch = &t_watch;

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
}

/*****************************************************************************
 * @brief    gui main function
 * @section  DESC Description
 *   - desc
*****************************************************************************/
static void *THR_watchdog(void *prm)
{
	app_thr_obj *tObj = &iwatch->wObj;
	int cmd, res, exit=0;
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
		if ((wd_cycle != 0) && (wd_cycle % WD_TIME == 0))
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
				
				if (wd_detect == WD_LOG_CNT) {
					sysprint("%s\n", msg);
					sync();
				}
			}
		}
		
		tObj->cmd = 0;
		wd_cycle += WD_CYCLE_TIME;
		app_msleep(WD_CYCLE_TIME);
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
int app_watchdog_init(void)
{
	app_thr_obj *tObj;

	//#--- create thread
	tObj = &iwatch->wObj;
    if (thread_create(tObj, THR_watchdog, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
    	eprintf("create gui thread\n");
		return EFAIL;
    }
	
    aprintf("... done!\n");
    return 0;
}

void app_watchdog_exit(void)
{
	app_thr_obj *tObj;

    tObj = &iwatch->wObj;
    event_send(tObj, APP_CMD_STOP, 0, 0);

    while(tObj->active)
    	app_msleep(20);
    thread_delete(tObj);

	aprintf("... done!\n");
}
