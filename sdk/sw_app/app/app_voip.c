/******************************************************************************
 * UCX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_voip.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2015.10.19 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <baresip_ipc_def.h>

#include "app_comm.h"
#include "app_voip.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

typedef struct {
	app_thr_obj sObj; /* sip object */
	void *handle;
	
} app_voip_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_voip_t t_voip;
static app_voip_t *pVoip = &t_voip;

extern void *gZmqContext;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static int _send_msg(void *skt, char *string)
{
	zmq_msg_t message;
	int size;
	
	zmq_msg_init_size(&message, strlen(string));
	memcpy(zmq_msg_data(&message), string, strlen(string));
	size = zmq_msg_send(&message, skt, 0);
	zmq_msg_close(&message);
	
	return (size);
}

/*****************************************************************************
* @brief    voip main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_voip(void *prm)
{
	app_thr_obj *tObj = &pVoip->sObj;
	int exit = 0, cmd;
	void *handle = NULL;
	zmq_msg_t message;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	handle = zmq_socket(gZmqContext, ZMQ_REQ);
	zmq_connect(handle, ZMQ_IPC_STR);
	zmq_msg_init(&message);
	
	while (!exit)
	{
		int size;
		
		size = zmq_msg_recv (&message, handle, 0);
		if (size == -1)
			// error
		
		//# wait cmd
		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP)
            break;

        app_msleep(200);
    }

    tObj->active = 0;
	
	zmq_msg_close(&message);
	zmq_close(handle);
	
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    voip manager
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_voip_init(void)
{
	app_thr_obj *tObj = &pVoip->sObj;
	FILE *f = NULL;
	/* execute baresip */
	f = popen("/opt/fitt/bin/baresip &", "r");
	if (f != NULL) {
		pclose(f); 
	}
	
	//# create check thread.
	if (thread_create(tObj, THR_voip, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create Voip Check thread\n");
		return EFAIL;
    }

    aprintf("... done!\n");

    return 0;
}

/*****************************************************************************
* @brief    voip manager
* @section  DESC Description
*   - desc
*****************************************************************************/
void app_voip_exit(void)
{
	app_thr_obj *tObj = &pVoip->sObj;

	/* delete Wi-Fi Connect Check object */
   	event_send(tObj, APP_CMD_STOP, 0, 0);
	while(tObj->active)
    	app_msleep(20);
	thread_delete(tObj);

	dprintf("... done!\n");
}
