/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_buzzer.c
 * @brief	buzzer functions
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "app_comm.h"
#include "app_main.h"
#include "app_buzz.h"
#include "app_set.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define BEEP_DRV_BASE_PATH		"/sys/class/backlight/pwm-backlight/brightness"

typedef struct {
	OSA_MutexHndl b_lock; //# buzzer lock
	int fd;
	
} app_buzz_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_buzz_t t_buzz;
static app_buzz_t *ibuzz = &t_buzz;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static void __buzzer_set__(int en)
{
	int fd = ibuzz->fd;
	
	if (en) {
		write(fd, "50", 3);
	} else {
		write(fd, "0", 2);
	}		
}

/*****************************************************************************
* @brief    buzzer control
* @section  [param] time: ms
*****************************************************************************/
void app_buzz_ctrl(int time, int cnt)
{
	if(!app_set->sys_info.beep_sound)
		return ;

	OSA_mutexLock(&ibuzz->b_lock);		
	//# buzzer active
	while (1)
	{
		__buzzer_set__(1);
		app_msleep(time);
		__buzzer_set__(0);
		cnt--;
		if(cnt == 0) {
			break;
		}
		app_msleep(time);
	}
	OSA_mutexUnlock(&ibuzz->b_lock);
}

/*****************************************************************************
* @brief    buzzer manager init/exit function
* @section  [desc]
*****************************************************************************/
int app_buzz_init(void)
{
	int status;
	int res = -1;
	
    memset(ibuzz, 0, sizeof(app_buzz_t));
	status = OSA_mutexCreate(&(ibuzz->b_lock));
    OSA_assert(status == OSA_SOK);
	
	if ((res = open(BEEP_DRV_BASE_PATH, O_WRONLY)) < 0) {
		eprintf("Error open %s\n", BEEP_DRV_BASE_PATH);
		return -1;
	}
	
	ibuzz->fd = res;
	return 0;
}

/*****************************************************************************
* @brief    buzzer manager init/exit function
* @section  [desc]
*****************************************************************************/
void app_buzz_exit(void)
{
	int status;
	
	status = OSA_mutexDelete(&(ibuzz->b_lock));
	OSA_assert(status == OSA_SOK);
	
	close(ibuzz->fd);
}
