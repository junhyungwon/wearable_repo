/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_cap.c
 * @brief	app video capture thread
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <mcfw/src_linux/mcfw_api/ti_vsys_priv.h>
#include "ti_vsys.h"
#include "ti_vcam.h"
#include "ti_vcap.h"
#include "ti_vdis.h"

#include "app_comm.h"
#include "app_gmem.h"
#include "app_main.h"
#include "app_ctrl.h"
#include "app_cap.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj vObj;		//# video capture(ipcbits) thread

	g_mem_info_t *imem;
} app_cap_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
//static app_cap_t cap_obj;
//static app_cap_t *icap=&cap_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    app capture start/stop function
* @section  [desc]
*****************************************************************************/
int app_cap_start(void)
{
	VSYS_PARAMS_S   vsysParams;
	VCAP_PARAMS_S   vcapParams;

	if(iapp->ste.b.cap)		//# already start
		return SOK;

	dprintf("start ...\n");

	//#--- init params
	Vsys_params_init(&vsysParams);
	Vcap_params_init(&vcapParams);

	//#--- init module
	vsysParams.enableEncode	= 0;
	vsysParams.enableMjpeg	= 0;
	vsysParams.enableHDMI	= 0; /* don't enable hdmi */

	vsysParams.systemUseCase = VSYS_USECASE_CAPTURE;
	vsysParams.captMode = CAPT_MODE_720P;
	vsysParams.decoderHD = SYSTEM_DEVICE_VID_DEC_NVP2440H_DRV;
	vsysParams.numChs = 4;
	vsysParams.serdesEQ = 2;

	Vsys_init(&vsysParams);
	Vcap_init(&vcapParams);
	Vdis_init(NULL);

	//#--- start link
#if defined(NEXXONE)
	Vsys_create();
#else	
	Vsys_create(0);
#endif	
	Vsys_datetime_init();	//# m3 Date/Time init

	Vcap_start();
	Vdis_start();

	iapp->ste.b.cap = 1;

	dprintf("... done!\n");

	return SOK;
}

int app_cap_stop(void)
{
	if(!iapp->ste.b.cap)
		return EINVALID;

	dprintf("start ...\n");
	iapp->ste.b.cap = 0;

	//#--- stop link
	Vdis_stop();
	Vcap_stop();

	Vsys_delete();

	//#--- exit module
	Vdis_exit();
	Vcap_exit();
	Vsys_exit();

	dprintf("... done!\n");

	return SOK;
}
