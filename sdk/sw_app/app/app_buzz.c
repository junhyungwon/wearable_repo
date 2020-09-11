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

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define BEEP_DRV_BASE_PATH		"/sys/class/backlight/pwm-backlight/brightness"

typedef struct {
	OSA_MutexHndl b_lock; //# buzzer lock
	
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
	char buf[128] = {0,};
	int brightness;
	int fd, len;

	if (en) brightness = 50;
	else	brightness = 0;

	/* initialize beep driver */
	len = snprintf(buf, sizeof(buf), "%d", brightness);
	if (len <= 0)
		return;

	if ((fd = open(BEEP_DRV_BASE_PATH, O_WRONLY)) < 0) {
		eprintf("Error open %s\n", BEEP_DRV_BASE_PATH);
		return;
	}

	write(fd, buf, len);
	close(fd);
}

/*****************************************************************************
* @brief    buzzer manager init/exit function
* @section  [desc]
*****************************************************************************/
int app_buzz_init(void)
{
	int status;
	
    memset(ibuzz, 0, sizeof(app_buzz_t));
	status = OSA_mutexCreate(&(ibuzz->b_lock));
    OSA_assert(status == OSA_SOK);
	
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
}

/*****************************************************************************
* @brief    buzzer control
* @section  [param] time: ms
*****************************************************************************/
void app_buzz_ctrl(int time, int cnt)
{
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
