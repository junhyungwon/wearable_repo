/******************************************************************************
 * NEXTT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    main.c
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
#include <limits.h>
#include <fcntl.h>
#include <sys/mman.h> //# mmap
#include <sys/time.h>
#include <unistd.h>
#include <linux/serial.h>	/* For Linux-specific struct serial_struct */
#include <termios.h>
#include <sys/ioctl.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
	   
#include "gnss_ipc_cmd_defs.h"
#include "main.h"
#include "nmea.h"
#include "fifo.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

static app_gnss_cfg_t cfg_obj;
app_gnss_cfg_t *app_cfg = &cfg_obj;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------
 message send/recv function
-----------------------------------------------------------------------------*/
static int send_msg(int cmd, int du, char *name)
{
	to_gnss_main_msg_t msg;
	
	msg.type = GNSS_MSG_TYPE_TO_MAIN;
	msg.cmd = cmd;
	
	return Msg_Send(app_cfg->qid, (void *)&msg, sizeof(to_gnss_main_msg_t));
}

static int recv_msg(void)
{
	to_gnss_msg_t msg;
	int size;
	
	//# blocking
	if (Msg_Rsv(app_cfg->qid, GNSS_MSG_TYPE_TO_GPS, (void *)&msg, sizeof(to_gnss_msg_t)) < 0) {
		return -1;
	}
	
	if (msg.cmd == GNSS_CMD_GPS_START) {
		app_cfg->rate = msg.rate;
		strcpy(app_cfg->dev_name, msg.dev_name);
	}
	
	return msg.cmd;
}

/*****************************************************************************
* @brief    system shared memory init
* @section  [desc]
*****************************************************************************/
static int app_shared_mem_init(void)
{
	unsigned char *tbuf = NULL;
	int tmp_id;
	
	//# Shared memory create
	tmp_id = shmget((key_t)GNSS_SHM_KEY, (size_t)GNSS_SHM_SIZE, (0777|IPC_CREAT));
	if (tmp_id == -1) {
		eprintf("Shared memory ID faild!\n");
		return -1;
	}
	
	//# Get shared memory
	tbuf = (unsigned char *)shmat(tmp_id, 0, 0);
	if (app_cfg->shmbuf == (unsigned char *)-1) {
		eprintf("Shared memory buffer faild!\n");
		return -1;
	}
	
	app_cfg->shmid  = tmp_id;
	app_cfg->shmbuf = tbuf; 
	
	return 0;
}

/*****************************************************************************
* @brief    GPS NMEA function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_gps_main(void *prm)
{
	app_thr_obj *tObj = &app_cfg->gObj;
	int exit = 0, cmd, ret;
	
	aprintf("enter...\n");
	
	/* gps init (항상 초기화 실패가 발생할 가능이 없다. 에러 확인 안함. */
	ret = gps_dev_init((const char *)app_cfg->dev_name, app_cfg->rate);
	if (ret < 0) {
		eprintf("failed to init gps dev(%s, %d)\n", app_cfg->dev_name, app_cfg->rate);
	}
	app_cfg->gps_fd = ret;
	
	/* gps 데이터 수신을 위한 자료구조 초기화 */
	nmea_parse_init();
	
	while(!exit)
	{
		gps_mask_t changed;
		gnss_shm_data_t new_gnss_data;
		
		cmd = tObj->cmd;
        if (cmd == APP_CMD_EXIT) {
            break;
        }
		
		changed = nmea_parse_poll(&new_gnss_data);
		if (changed == ERROR_SET) {
			/* ERROR?? */
		} else if (changed == NODATA_IS) {
			/* 9600bps일경우 1ms에 대략 1바이트 수신됨. */
			delay_msecs(100);
			continue;
		} 
		
		if (changed & REPORT_IS)
		{
			//FIFO_put(app_cfg->pfifo, new_gnss_data);
		}
			
		/* 타쓰레드를 동작시키기 위한 최소한의 delay */
		delay_msecs(20);
	}
	
	gps_dev_exit(app_cfg->gps_fd);
	aprintf("exit...\n");
		
	return NULL;
}

/*****************************************************************************
* @brief    gnss start thread function
* @section  [desc]
*****************************************************************************/
int app_gps_start(void)
{
	app_thr_obj *tObj = &app_cfg->gObj;
	
	//#--- create msg receive thread
	if (thread_create(tObj, THR_gps_main, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}
	
	aprintf("... done!\n");
	
	return 0;
}

void app_gps_stop(void)
{
    app_thr_obj *tObj = &app_cfg->gObj;

   	event_send(tObj, APP_CMD_STOP, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);

    dprintf("... done!\n");
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
static void app_main(void)
{
	int exit = 0, cmd;

	aprintf("enter...\n");
	
	app_cfg->qid = Msg_Init(GNSS_MSG_KEY);
	send_msg(GNSS_CMD_GPS_READY, 0, 0);
	
	while (!exit) {
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive gps process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case GNSS_CMD_GPS_START:
			app_gps_start();
			break;
		case GNSS_CMD_GPS_EXIT:
			exit = 1;
			dprintf("received gps exit!\n");
			break;
		
		default:
			break;	
		}
	}
	
	Msg_Kill(app_cfg->qid);

	aprintf("exit...\n");
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
int main(int argc, char **argv)
{
	int rc = 0, offset;

	printf(" [GPS process] start...\n");
	
	/* GPS 데이터를 공유할 shared memory 초기화 및 MSGQ 초기화 */
	rc = app_shared_mem_init();
	if (rc < 0)
		goto err_exit;
	
	/* initialize FIFO */
	app_cfg->pfifo = app_cfg->shmbuf;
	offset = (int)(app_cfg->pfifo + GNSS_FIFO_INFO_SIZE);
	dprintf("shared memory 0x%08x, offset 0x%08x!\n", app_cfg->pfifo, offset);
	FIFO_init(app_cfg->pfifo, offset, GNSS_FIFO_INFO_SIZE);
	
	//#--- main --------------
	app_main();
	//#-----------------------

err_exit:	
	if (app_cfg->shmbuf != NULL)
		free(app_cfg->shmbuf);
	
	printf(" [GPS] process exit!\n");
	
	return 0;
}
