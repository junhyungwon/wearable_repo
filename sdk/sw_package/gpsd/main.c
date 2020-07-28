/******************************************************************************
 * NEXTT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_main.c
 * @brief
 * @section MODIFY history
 *     - 2020.07.08 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <termios.h>
#include <ctype.h>
#include <signal.h>


#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#include <time.h>

#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/*
 * 9600bps --> 1초에 1200BYTE, 19200 --> 2400BYTE, 38400 --> 4800BYTE 
 */
#define TIME_GPS_CYCLE			100		//# msec

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static struct gps_device_t session;
static struct gps_context_t context;

static int quit = 1;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

static void onsig(int sig)
{
    if (quit)
		quit = 0;
}

/****************************************************
 * NAME : int app_gps_init(int rate)
 ****************************************************/
static int gps_dev_init(const char *devname, int rate)
{
	struct termios options;
	mode_t mode = (mode_t) O_RDWR; /* uart mode */
	int fd, fl;
	
	fd = open(devname, (int)(mode | O_NOCTTY | O_NONBLOCK));
	if (fd < 0) {
		eprintf("Failed to open %s!\n", devname);
		return -1;
	}

	/* exclusion-lock the device before doing any reads */
	ioctl(fd, (unsigned long)TIOCEXCL);

	/* include <asm/fcntl.h>
	 * O_NONBLOCK: if no data, return -1
	 * O_NDELAY: if no data, return 0
	 */
    fl = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, fl & ~O_NONBLOCK);

	/* Get the current options for the port */
	tcgetattr(fd, &options);

	/* set baud rate */
	cfsetispeed(&options, (speed_t)rate);
    cfsetospeed(&options, (speed_t)rate);
	tcsetattr(fd, TCSANOW, &options);
	tcflush(fd, TCIOFLUSH); /* in/out flush */

	options.c_oflag = 0; //# ~(OPOST); post-process disable
	/*
	 * if neither IGNPAR nor PARMRK is set,
	 * read character with a parity error or framing error as '\0'
	 */
	options.c_iflag = 0; //# not set IGNPAR
	/* noncanonical, no echo, no signal(INTR,QUIT) */
	options.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
	options.c_cflag &= ~(PARENB | PARODD | CRTSCTS | CSTOPB);
	options.c_cflag |= CREAD | CLOCAL | CS8;

	options.c_cc[VMIN]  = 0;  /* blocking read until 0 chars received */
	/*
	 * if VMIN = 0, when read is called returns either when at least 1 byte
	 * of data available, or when timer expired. if the timer expires without
	 * any input read returns 0
	 */
	options.c_cc[VTIME] = 3;

	tcflush(fd, TCIFLUSH); //# input flush.
	
	/* 현재 설정이 바로 적용된다. (TCSANOW) */
	tcsetattr(fd, TCSANOW, &options);

	return fd;
}

static void gps_dev_exit(int fd)
{
	if (fd >= 0) {
		tcflush(fd, TCIFLUSH); /* input flush */
		close(fd);
	}
}

static void delay_msecs(unsigned int msecs)
{
	struct timespec delayTime, elaspedTime;

	delayTime.tv_sec  = msecs/1000;
	delayTime.tv_nsec = (msecs%1000)*1000000;

	nanosleep(&delayTime, &elaspedTime);
}

static void app_main(void)
{
	int exit = 0;
	int fd = -1, cmd;
	
	fd = gps_dev_init("/dev/ttyO1", 9600);
	if (fd < 0) {
		eprintf("open error!\n");
		return 1;
	}
	
	gpsd_time_init(&context, time(NULL));
    context.readonly = true;
    gpsd_init(&session, &context, NULL);
	session.gpsdata.gps_fd = fd; //# rupy
    gpsd_clear(&session);
	
	do {
		gps_mask_t changed;
		
		changed = gpsd_poll(&session);
		//dprintf("gpsd poll mask %llx\n", changed);
		//if (changed == ERROR_SET || changed == NODATA_IS)
		if (changed == ERROR_SET)
	    	break;
		
		if (changed == NODATA_IS) {
			/* FILL output queue */
			delay_msecs(100);
			continue;
		}
		 
		if (session.lexer.type == COMMENT_PACKET) {
	    	dprintf("gpsd set century\n");
			gpsd_set_century(&session);
		}
		
		if ((changed & REPORT_IS) != 0) {
			dprintf("gpsd REPORT_IS poll mask %llx\n", changed);
		}
		
		delay_msecs(20);
		
	} while(quit == 1);
	
	dprintf("GPSD end\n");
	
	gps_dev_exit(fd);
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
int main(int argc, char **argv)
{
	char pathname[256];
	int rc = 0;
	struct sigaction sa;

	sa.sa_handler = onsig;
	(void)sigfillset(&sa.sa_mask);
	sa.sa_flags = 0;
	(void)sigaction(SIGINT, &sa, NULL);

	printf(" [GPSD] start...\n");
	
	//#--- main --------------
	app_main();
	//#-----------------------

	printf(" [GPSD] exit!\n");
	
	return 0;
}
