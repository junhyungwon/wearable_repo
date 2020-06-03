/******************************************************************************
 * FITT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_main.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2018.05.03 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//# include mcfw_linux
#include "ti_vsys.h"

#include "app_comm.h"
#include "app_gmem.h"
#include "app_key.h"
#include "app_dev.h"
#include "app_ctrl.h"
#include "app_mcu.h"
#include "gui_main.h"
#include "app_main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_cfg_t cfg_obj;
app_cfg_t *iapp=&cfg_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 application config function
-----------------------------------------------------------------------------*/
int app_cfg_init(void)
{
	memset((void *)iapp, 0, sizeof(app_cfg_t));

	iapp->is_pal = 0;	//# 0: NTSC, 1: PAL

	//# h/w version
	iapp->hw_ver = ctrl_get_hw_version(NULL);

	return SOK;
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
int main(int argc, char **argv)
{
	printf("\n----- FITT360 TEST start -----\n");

	app_cfg_init();
	app_mcu_init();

	//#--- system init
	mcfw_linux_init(iapp->is_pal);
	g_mem_init();

	//# app thread start
	app_dev_init();
	app_gui_init();

	app_key_start();
	app_mcu_start();
	app_dev_start();

	//#--- app test main ----------
	gui_main();
	//#-----------------------

	//# app thread stop
	app_key_stop(); //# for gpio key

	app_dev_stop();
	app_mcu_stop();
	//app_key_stop();

	app_gui_exit();
	app_dev_exit();

	//#--- system de-init
	g_mem_exit();
	mcfw_linux_exit();

	app_mcu_exit();		//# will power off after 200mS

	printf("----- FITT360 TEST end -----\n\n");

	return 0;
}
