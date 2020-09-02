/******************************************************************************
 * NEXTT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    gps_proc.c
 * @brief
 * @section MODIFY history
 *     - 2020.07.08 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/serial.h>	/* For Linux-specific struct serial_struct */
#include <termios.h>
#include <errno.h>
	   
#include "gnss_ipc_cmd_defs.h"
#include "main.h"
#include "nmea_parse.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define GPS_PROC_LENGTH			1000 /*  nmea ?°ì´????1000ê°?76 *1000 = 760KB ê°€ ?„ìš”??*/

#define GPS_DISABLED			0
#define	GPS_ENABLED				1

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj mObj;		//# gps data poll
	app_thr_obj gObj;		//# gps data send

	int qid;
	
//	FIFO fifo;
	gnss_shm_data_t r_data;
	gnss_shm_data_t w_data;
	pthread_mutex_t lock;
} app_gps_proc_t;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static char __mode_chg_nav_bei[] = {
	0xB5, 0x62, 0x06, 0x3E, 0x3C, 0x00, 0x00, 0x00, 0x20, 0x07,  //# 10
	0x00, 0x08, 0x10, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01,  //# 20
	0x03, 0x00, 0x01, 0x00, 0x01, 0x01, 0x02, 0x04, 0x08, 0x00,  //# 30
	0x00, 0x00, 0x01, 0x01, 0x03, 0x08, 0x10, 0x00, 0x01, 0x00,  //# 40
	0x01, 0x01, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x01,  //# 50
	0x05, 0x00, 0x03, 0x00, 0x01, 0x00, 0x01, 0x01, 0x06, 0x08,  //# 60
	0x0E, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2F, 0xA1		         //# 68
};

static app_gps_proc_t t_proc_obj;
static app_gps_proc_t *iproc=&t_proc_obj;

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
	options.c_cc[VTIME] = 1; //# 1-> 100ms, 3-> 300ms

	tcflush(fd, TCIFLUSH); //# input flush.
	
	/* ?„ìž¬ ?¤ì •??ë°”ë¡œ ?ìš©?œë‹¤. (TCSANOW) */
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

static void gps_set_rmc_data(int isEnable)
{
	pthread_mutex_lock(&iproc->lock);

	if(isEnable) {
		/* NMEA DATA copy */
		iproc->w_data.gps_fixed = session->gpsdata.status;
		iproc->w_data.speed     = session->gpsdata.fix.speed;
		iproc->w_data.lat     	= session->gpsdata.fix.latitude;
		iproc->w_data.lot     	= session->gpsdata.fix.longitude;
		iproc->w_data.dir     	= session->gpsdata.fix.track;
		
		iproc->w_data.gtm.tm_year  	= session->nmea.date.tm_year; //# 1900?„ì„ ?”í•´????
		iproc->w_data.gtm.tm_mon  	= session->nmea.date.tm_mon; //# +1???´ì•¼ 1??ë¶€???œìž‘??
		iproc->w_data.gtm.tm_mday  	= session->nmea.date.tm_mday;
		iproc->w_data.gtm.tm_hour  	= session->nmea.date.tm_hour;
		iproc->w_data.gtm.tm_min  	= session->nmea.date.tm_min;
		iproc->w_data.gtm.tm_sec  	= session->nmea.date.tm_sec;
		iproc->w_data.subsec    	= session->nmea.subseconds.tv_nsec / 1000000L; //# nano ì´ˆë¡œ ?œì‹œ.. msë¡?ë³€ê²½í•˜ê¸??„í•´??					
			
		#if 0	
		dprintf("GPS - DATE %04d-%02d-%02d, UTC %02d:%02d:%02d, speed=%.2f, (LAT:%.2f, LOT:%.2f)\n",
				iproc->w_data.gtm.tm_year+1900, iproc->w_data.gtm.tm_mon+1, iproc->w_data.gtm.tm_mday,
				iproc->w_data.gtm.tm_hour, iproc->w_data.gtm.tm_min, iproc->w_data.gtm.tm_sec,
				iproc->w_data.speed, iproc->w_data.lat, iproc->w_data.lot);
		#endif	
	} else {
		// GPS is not stable... 
		iproc->w_data.gps_fixed = 0;
	}

	pthread_mutex_unlock(&iproc->lock);
}

/*****************************************************************************
* @brief    GET GPS NMEA function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_gps_poll(void *prm)
{
	app_thr_obj *tObj = &iproc->mObj;
	int exit = 0, cmd;
	
//	aprintf("enter...\n");
	tObj->active = 1;
	
	while(!exit)
	{
		gps_mask_t changed;
		int poll_done = 0;
		
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			break;
		} 
		else if (cmd == APP_CMD_START) 
		{
			if (app_cfg->gps_fd <= 0) {
				app_cfg->gps_fd = gps_dev_init((const char *)app_cfg->dev_name, 
								app_cfg->rate);
				if (app_cfg->gps_fd < 0) {
					eprintf("failed to init gps dev(%s, %d)\n", app_cfg->dev_name,	app_cfg->rate);
					send_msg(GNSS_CMD_GPS_DEV_ERR);
					return NULL;
				}
				app_nmea_parse_init();
			}
		}
		
		while (poll_done == 0)
		{
			cmd = tObj->cmd;
			if (cmd == APP_CMD_STOP) {
				break;
			}
			
			changed = app_nmea_parse_get_data();
			if (changed == ERROR_SET) {
				/* ERROR?? */
				gps_set_rmc_data(GPS_DISABLED);
				continue;
			} else if (changed == NODATA_IS) {
				/* not yet GPRMC data receive!! ignore */
				delay_msecs(100);
				continue;
			} 
			
			//# NTPTIME_IS | TIME_SET | LATLON_SET | MODE_SET | TRACK_SET | SPEED_SET
			//if ((changed & REPORT_IS) != 0)
			if (((changed & REPORT_IS) != 0)  ||
				((changed & TIME_SET) != 0)   ||
				((changed & LATLON_SET) != 0) ||
				((changed & MODE_SET) != 0)   ||
				((changed & TRACK_SET) != 0)  ||
				((changed & SPEED_SET) != 0))
			
			{
				gps_set_rmc_data(GPS_ENABLED);
			}
				
			delay_msecs(20);
		} /* while (poll_done == 0) */
			
	} /* while(!exit) */
	
	gps_dev_exit(app_cfg->gps_fd);
	tObj->active = 0;

//	aprintf("exit...\n");
		
	return NULL;
}

/*****************************************************************************
* @brief    gnss start thread function
* @section  [desc]
*****************************************************************************/
int app_gps_proc_init(void)
{
	app_thr_obj *tObj;
	pthread_mutexattr_t mutex_attr;
	int status = 0;

	status |= pthread_mutexattr_init(&mutex_attr);
	status |= pthread_mutex_init(&iproc->lock, &mutex_attr);
	if (status != 0) {
    	eprintf("failed to create gps proc %d!\n", status);
		return -1;
	}
	
  	pthread_mutexattr_destroy(&mutex_attr);
	memset(iproc, 0, sizeof(app_gps_proc_t));
	
	tObj = &iproc->mObj;
	if (thread_create(tObj, THR_gps_poll, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}
	
	//aprintf("... done!\n");
	
	return 0;
}

/*****************************************************************************
* @brief    gnss request gps data
* @section  [desc]
*****************************************************************************/
void app_gps_data_request()
{
	pthread_mutex_lock(&iproc->lock);
 	/* shared memory¿¡ ÀúÀå ÈÄ ¸Þ¼¼Áö Àü´Þ */
	memcpy((char *)app_cfg->shmbuf, (char *)&iproc->w_data, sizeof(gnss_shm_data_t));
	send_msg(GNSS_CMD_GPS_POLL_DATA);
	pthread_mutex_unlock(&iproc->lock);
}

/*****************************************************************************
* @brief    gnss parseë¥??¤í–‰?œí‚¨??
* @section  [desc]
*****************************************************************************/
int app_gps_proc_start(void)
{
	event_send(&iproc->mObj, APP_CMD_START, 0, 0);
	return 0;
}

/*****************************************************************************
* @brief    gnss parseë¥??¼ì‹œ?•ì?.
* @section  [desc]
*****************************************************************************/
int app_gps_proc_stop(void)
{
	event_send(&iproc->mObj, APP_CMD_STOP, 0, 0);
	
	tcflush(app_cfg->gps_fd, TCIFLUSH); //# input flush.
	
	return 0;
}

/*****************************************************************************
* @brief    gnss parseë¥??„ì „??ì¢…ë£Œ
* @section  [desc]
*****************************************************************************/
void app_gps_proc_exit(void)
{
    app_thr_obj *tObj;
		
	tObj = &iproc->mObj;
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
	pthread_mutex_destroy(&iproc->lock);  
}
