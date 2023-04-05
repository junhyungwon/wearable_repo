/******************************************************************************
 * NEXX SYSTEM
 * Copyright by Linkflow, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    process_check.c
 * @brief
 * @section MODIFY history
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	while (1)
	{
		//system("ps -ef | grep defunct | grep -v grep | grep wis-streamer | awk '{print $3}' | kill -9");
		system("ps -ef | grep defunct | grep -v grep | grep wis-streamer | awk '{print $3}' | xargs -r kill -9");
		sleep(2);
    }
}
