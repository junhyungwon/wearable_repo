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
#include "fifo.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define GPS_PROC_LENGTH			1000 /*  nmea 데이터 약 1000개 76 *1000 = 760KB 가 필요함 */

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj mObj;		//# gps data poll
	app_thr_obj gObj;		//# gps data send
	
	FIFO fifo;
	gnss_shm_data_t r_data;
	gnss_shm_data_t w_data;
	
} app_gps_proc_t;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
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

/*****************************************************************************
* @brief    GET GPS NMEA function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_gps_poll(void *prm)
{
	app_thr_obj *tObj = &iproc->mObj;
	int exit = 0, cmd, ret;
	int poll_start = 0;
	
	aprintf("enter...\n");
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
			/* STOP 후 START 시 GPS UART 초기화를 다시 수행하지 않음 */
			if (app_cfg->gps_fd <= 0) {
				app_cfg->gps_fd = gps_dev_init((const char *)app_cfg->dev_name, 
								app_cfg->rate);
				if (app_cfg->gps_fd < 0) {
					eprintf("failed to init gps dev(%s, %d)\n", app_cfg->dev_name,	app_cfg->rate);
					/* TODO 에러 처리를 어떻게 해야 하는 지 ??? */
					send_msg(GNSS_CMD_GPS_DEV_ERR);
					return NULL;
				}
				/* gps 데이터 수신을 위한 자료구조 초기화 */
				app_nmea_parse_init();
			}
		}
		
		/* GPS PAUSE 위해서 추가함 */
		while (poll_done == 0)
		{
			cmd = tObj->cmd;
			if (cmd == APP_CMD_EXIT) {
				exit = 1; /* while(exit) 쓰레드 종료 */
				break;
			} else if (cmd == APP_CMD_STOP) {
				/* parser 만 일시 정지 */
				break;
			}
			
			changed = app_nmea_parse_get_data();
			if (changed == ERROR_SET) {
				/* ERROR?? */
			} else if (changed == NODATA_IS) {
				/* 9600bps일경우 1ms에 대략 1바이트 수신됨. */
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
				/* 1초마다 데이터가 수신됨 */
				/* REPORT_IS 플래스가 설정되면 모든 데이터가 수신된것으로 판단 */
				if (FIFO_isFull(&iproc->fifo) == 0) 
				{
					/* NMEA DATA copy */
					iproc->w_data.gps_fixed = session->gpsdata.status;
					iproc->w_data.speed     = session->gpsdata.fix.speed;
					iproc->w_data.lat     	= session->gpsdata.fix.latitude;
					iproc->w_data.lot     	= session->gpsdata.fix.longitude;
					iproc->w_data.dir     	= session->gpsdata.fix.track;
					
					iproc->w_data.gtm.tm_year  = session->nmea.date.tm_year; //# 1900년을 더해야 함.
					iproc->w_data.gtm.tm_mon  = session->nmea.date.tm_mon; //# +1을 해야 1월 부터 시작함.
					iproc->w_data.gtm.tm_mday  = session->nmea.date.tm_mday;
					iproc->w_data.gtm.tm_hour  = session->nmea.date.tm_hour;
					iproc->w_data.gtm.tm_min  = session->nmea.date.tm_min;
					iproc->w_data.gtm.tm_sec  = session->nmea.date.tm_sec;
					iproc->w_data.subsec    = session->nmea.subseconds.tv_nsec / 1000000L; //# nano 초로 표시.. ms로 변경하기 위해서
					
					FIFO_put(&iproc->fifo, (unsigned int)&iproc->w_data, sizeof(gnss_shm_data_t));
						
					#if 0	
					dprintf("GPS - DATE %04d-%02d-%02d, UTC %02d:%02d:%02d, speed=%.2f, (LAT:%.2f, LOT:%.2f)\n",
							iproc->w_data.gtm.tm_year+1900, iproc->w_data.gtm.tm_mon+1, iproc->w_data.gtm.tm_mday,
							iproc->w_data.gtm.tm_hour, iproc->w_data.gtm.tm_min, iproc->w_data.gtm.tm_sec,
							iproc->w_data.speed, iproc->w_data.lat, iproc->w_data.lot);
					#endif	
				} else {
					eprintf("Fifo is full\n");
					/* TODO */
				}
			}
			/* 타쓰레드를 동작시키기 위한 최소한의 delay */
			delay_msecs(20);
		} /* while (poll_done == 0) */
			
	} /* while(!exit) */
	
	gps_dev_exit(app_cfg->gps_fd);
	tObj->active = 0;

	aprintf("exit...\n");
		
	return NULL;
}

static void *THR_gps_main(void *prm)
{
	app_thr_obj *tObj = &iproc->gObj;
	int exit = 0, cmd, ret;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while(!exit)
	{
		int gps_done = 0;
		
		cmd = event_wait(tObj);
		if(cmd == APP_CMD_EXIT) {
			break;
		}
		
		while(gps_done == 0)
		{
			cmd = tObj->cmd;
			if (cmd == APP_CMD_EXIT) {
				exit = 1; /* while(exit) 쓰레드 종료 */
				break;
			} else if (cmd == APP_CMD_STOP) {
				/* parser 만 일시 정지 */
				break;
			}
			
			/* FIFO Get */
			if (!FIFO_isEmpty(&iproc->fifo)) 
			{
				/* get data */
				FIFO_get(&iproc->fifo, (unsigned int)&iproc->r_data, sizeof(gnss_shm_data_t));
				#if 0 //# debugging	
				dprintf("GPS - DATE %04d-%02d-%02d, UTC %02d:%02d:%02d, speed=%.2f, (LAT:%.2f, LOT:%.2f)\n",
						iproc->r_data.gtm.tm_year+1900, iproc->r_data.gtm.tm_mon+1, iproc->r_data.gtm.tm_mday,
						iproc->r_data.gtm.tm_hour, iproc->r_data.gtm.tm_min, iproc->r_data.gtm.tm_sec,
						iproc->r_data.speed, iproc->r_data.lat, iproc->r_data.lot);
				#endif
				
				/* shared memory에 저장 후 메세지 전달 */
				memcpy((char *)app_cfg->shmbuf, (char *)&iproc->r_data, sizeof(gnss_shm_data_t));
				send_msg(GNSS_CMD_GPS_POLL_DATA);	
			} else {
				//dprintf("current fifo is empty!!\n");
			}
			
			delay_msecs(500);
		} /*while(done == 0)*/
	}
	
	tObj->active = 0;
	aprintf("exit...\n");
		
	return NULL;
}

/*****************************************************************************
* @brief    gnss start thread function
* @section  [desc]
*****************************************************************************/
int app_gps_proc_init(void)
{
	app_thr_obj *tObj;
	int length = 0;
	
	memset(iproc, 0, sizeof(app_gps_proc_t));
	
	/* FIFO 초기화 */
	length = (sizeof(gnss_shm_data_t)*GPS_PROC_LENGTH);
	dprintf("fifo initialized: size %d\n", length);
	FIFO_init(&iproc->fifo, length);
	
	tObj = &iproc->mObj;
	if (thread_create(tObj, THR_gps_poll, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}
	
	tObj = &iproc->gObj;
	if (thread_create(tObj, THR_gps_main, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}
	
	aprintf("... done!\n");
	
	return 0;
}

/*****************************************************************************
* @brief    gnss parse를 실행시킨다.
* @section  [desc]
*****************************************************************************/
int app_gps_proc_start(void)
{
	event_send(&iproc->mObj, APP_CMD_START, 0, 0);
	delay_msecs(50);
	
	event_send(&iproc->gObj, APP_CMD_START, 0, 0);
	
	return 0;
}

/*****************************************************************************
* @brief    gnss parse를 일시정지.
* @section  [desc]
*****************************************************************************/
int app_gps_proc_stop(void)
{
	event_send(&iproc->mObj, APP_CMD_STOP, 0, 0);
	delay_msecs(50);
	
	event_send(&iproc->gObj, APP_CMD_STOP, 0, 0);
	
	/* 확인이 필요함 */
	tcflush(app_cfg->gps_fd, TCIFLUSH); //# input flush.
	
	aprintf("... done!\n");
	
	return 0;
}

/*****************************************************************************
* @brief    gnss parse를 완전히 종료
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
	
	tObj = &iproc->gObj;
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
	FIFO_clear(&iproc->fifo);

    dprintf("... done!\n");
}
