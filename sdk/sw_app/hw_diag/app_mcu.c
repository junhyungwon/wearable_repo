/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_mcu.c
 * @brief	micom(volt, switch) check thread
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "app_comm.h"
#include "app_main.h"
#include "app_ctrl.h"
#include "gui_main.h"
#include "app_mcu.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define TIME_DATA_CYCLE		  100	//# msec, data receive period from micom

//# ms --------------------------
#define TIMD_DRAW_DATA		  500
//-------------------------------

#define MVOLT_MIN		    950		//#  9.50 V, minimum main voltage
#define SVOLT_MIN		 	450		//#  4.50 V, minimum system voltage

typedef struct {
	app_thr_obj cObj;		//# micom thread

	dev_val_t val;
} app_mcu_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_mcu_t mcu_obj;
static app_mcu_t *imcu=&mcu_obj;

/*****************************************************************************
* @brief	micom message main function
* @section  [desc] check power switch and acc power
*****************************************************************************/
static void *THR_micom(void *prm)
{
	app_thr_obj *tObj = &imcu->cObj;
	int exit=0, ret=0;
	mic_msg_t msg;

	aprintf("enter...\n");
	tObj->active = 1;

	mic_data_send(1, TIME_DATA_CYCLE);

	while(!exit)
	{
		ret = mic_recv_msg((char*)&msg, sizeof(mic_msg_t));
		if(ret < 0) {
			continue;
		}

		switch(msg.cmd)
		{
			case CMD_DEV_VAL:
			{
				dev_val_t *val = (dev_val_t *)msg.data;

				imcu->val.mvolt = val->mvolt;		//# XX.xx V format
				imcu->val.ibatt = val->ibatt;		//# XX.xx V format
				imcu->val.ebatt  = val->ebatt;		//# XX.xx V format

				#if 0
				printf("[%3d] mpwr: %d.%d V, ibatt:%d.%d V, ebatt:%d.%d V\n", msg.param,
					(imcu->val.mvolt/100), (imcu->val.mvolt%100), (imcu->val.ibatt/100), (imcu->val.ibatt%100),
					(imcu->val.ebatt/100), (imcu->val.ebatt%100));
				#endif
				break;
			}
			case CMD_PSW_EVT:
			{
				short key_type = msg.data[0];
				dprintf("[evt] pwr switch %s event\n", key_type==2?"long":"short");
				if (key_type == 2)
					gui_ctrl(APP_KEY_PWR, 0, 0);
				else
					//gui_ctrl(APP_KEY_OK, 0, 0);
					gui_ctrl(APP_KEY_RIGHT, 0, 0);
				break;
			}
			case CMD_DAT_STOP:		//# response data send stop
			{
				exit = 1;
				break;
			}
			default:
				break;
		}
	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    micom value i/f function
* @section	[desc]
*****************************************************************************/
int app_get_volt(void)
{
    return imcu->val.mvolt;
}

/*****************************************************************************
* @brief    micom thread start/stop function
* @section  [desc]
*****************************************************************************/
int app_mcu_start(void)
{
	app_thr_obj *tObj;

	//# static config clear
	memset((void *)imcu, 0x0, sizeof(app_mcu_t));

	//#--- create dio thread
	tObj = &imcu->cObj;
	if(thread_create(tObj, THR_micom, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}

	return SOK;
}

void app_mcu_stop(void)
{
	app_thr_obj *tObj;

	//#--- stop thread
	tObj = &imcu->cObj;
	//event_send(tObj, UBX_CMD_STOP, 0, 0);		//# need not
	mic_data_send(0, 0);		//# data send stop
	while(tObj->active) {
		OSA_waitMsecs(10);
	}
	thread_delete(tObj);
}

/*****************************************************************************
* @brief    micom init/exit function
* @section  [desc]
*****************************************************************************/
int app_mcu_init(void)
{
	int ret;

	ret = mic_msg_init();
	if(ret < 0) {
		return -1;
	}
	mic_send_ready();

	#if !(APP_RELEASE)
	mic_exit_state(OFF_NONE, 0);	//# for test - no power off
	#endif

	return 0;
}

int app_mcu_exit(void)
{
    mic_msg_exit();

	return 0;
}
