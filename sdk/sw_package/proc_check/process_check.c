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
	FILE *f = NULL;
	char lineBuf[256 + 1];
	
	while (1)
	{
		f = popen("/bin/ps ax", "r");
		if (f == NULL) {
			fprintf(stderr, "Failed to execute ps ax\n");
			return -1;
		}
	
		while (fgets(lineBuf, sizeof(lineBuf), f) != NULL)
		{
			char *s, *ptr;
			int pid;
			
			/* find defunct: */
			if ((s = strstr(lineBuf, "defunct")) != NULL) 
			{
				/*
				* ps ax
				* 400 ?        Z      0:00 [wis-streamer] <defunct>
				*/
				ptr = strtok(lineBuf, " "); //# delimiter space
				if (ptr != NULL) {
					sscanf(ptr, "%d\n", &pid);
					kill(pid, SIGKILL); //# kill -9
					fprintf(stderr, "Founded <defunct> PID %d killed!\n", pid);
				}
			}
		}
		
		if (f != NULL)
			pclose(f);
		
		sleep(2);
		/*
		 * zombie process일 경우 kill (xargs -I {} <- null process id 방지)
		 */
		//system("ps -ef | grep defunct | grep -v grep | grep wis-streamer | awk '{print $3}' | xargs -I{} kill -9");
    }
}

