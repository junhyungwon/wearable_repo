#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	while(1)
	{
		sleep(1) ;
		/*
		 * zombie process�� ��� kill (xargs -I {} <- null process id ����)
		 */
		system("ps -ef | grep defunct | grep -v grep | grep wis-streamer | awk '{print $3}' | xargs -I{} kill -9");
    }
}

