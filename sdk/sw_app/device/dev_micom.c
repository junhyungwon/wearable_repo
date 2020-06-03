/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    dev_micom.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2014.05.27 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <termios.h>

#include "dev_common.h"
#include "dev_micom.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define UART2_DEV		"/dev/ttyO2"	//# micom uart

typedef struct {
	int fd;

	short ver;		//# micom version
	host_off_e off_ste;
    short off_val;
} dev_mic_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static dev_mic_t imic;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 uart function
-----------------------------------------------------------------------------*/
static int uart_init(void)
{
	int fd;
	struct termios oldtio, newtio;

	fd = open((const char *)UART2_DEV, O_RDWR);
	if (fd  < 0) {
		return -1;
	}

	tcgetattr(fd, &oldtio);

	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;		//# non-parity
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;

	newtio.c_cc[VTIME]    = 0;
	newtio.c_cc[VMIN]     = 1;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

	return fd;
}

static char uart_read_byte(void)
{
	char data=0;

	read(imic.fd, &data, 1);

	return data;
}

static void uart_write_byte(char data)
{
	write(imic.fd, &data, 1);
}

/*****************************************************************************
* @brief    receive message function
* @section	[desc]
*****************************************************************************/
int mic_recv_msg(char *data, int size)
{
	int i;
	char sync, len, checksum, cs=0;

	//# wait sync byte
	while(1) {
		sync = uart_read_byte();
		if(sync == MSG_SYNC_S) {
			break;
		}
	}
	len = uart_read_byte();
	if(len > size) {
		return -1;
	}

	//# read message
	for(i=0; i<len; i++)
	{
		*data = uart_read_byte();
		cs += *data++;
	}

	checksum = uart_read_byte();
	if(cs != checksum) {
		printf("checksum error! (recv 0x%x != calc 0x%x\n", cs, checksum);
		return -1;
	}

	return 0;
}

/*****************************************************************************
* @brief    send message function
* @section	[desc]
*****************************************************************************/
static int uart_send(char *data, int size)
{
	char i;

	uart_write_byte(MSG_SYNC_S);

	for(i=0; i<size; i++) {
		uart_write_byte(*data++);
	}

	uart_write_byte(MSG_SYNC_E);

	usleep(500);	//# process time in micom

	return 0;
}

int mic_send_msg(char cmd, char param, short data)
{
	int ret;
	hst_msg_t tx_msg;

	tx_msg.cmd = cmd;
	tx_msg.param = param;
	tx_msg.data = data;

	ret = uart_send((char *)&tx_msg, sizeof(hst_msg_t));

	return ret;
}

/*----------------------------------------------------------------------------
 micom set function
-----------------------------------------------------------------------------*/
int mic_data_send(int en, short msec)
{
	return mic_send_msg(CMD_DAT_SEND, en, msec);
}

int mic_set_watchdog(int en, short sec)
{
	return mic_send_msg(CMD_WATCHDOG, en, sec);
}

int mic_keep_alive(void)
{
	return mic_send_msg(CMD_ALIVE, 0, 0);
}

int mic_exit_state(int state, short data)
{
    imic.off_ste = (host_off_e)state;
    imic.off_val = data;

    return 0;
}

short mic_get_version(void)
{
	return imic.ver;	//# xx.xx format
}

int mic_send_ready(void)
{
	int ret;
	mic_msg_t msg;
	int major, minor;

	printf(" [dev ] micom check ...\n");

	//# send command for app start, data field : system period time
	mic_send_msg(CMD_HOST_RDY, 0, 0);

	ret = mic_recv_msg((char *)&msg, sizeof(mic_msg_t));
	if((ret < 0) || (msg.cmd != CMD_MIC_VER)) {
		close(imic.fd);
		printf(" [dev ] err: micom sync fail\n");
		return -1;
	}

	imic.ver = msg.data[0];
	major = (imic.ver>>8) & 0xFF;
	minor = imic.ver & 0xFF;
	printf(" [dev ] micom sync ok! (v%02x.%02x)\n", major, minor );

	if(imic.ver < MIC_VERSION) {
		printf(" [warning] micom version is old!!!\n");
	}

	return 0;
}

/*****************************************************************************
* @brief    micom init/exit function
* @section	[desc]
*****************************************************************************/
int mic_msg_init(void)
{
	imic.fd = uart_init();
	if(imic.fd < 0) {
		printf(" [dev ] err: uart init fail\n");
		return -1;
	}

	imic.off_ste = OFF_NORMAL;
	imic.off_val = 0;

	return 0;
}

int mic_msg_exit(void)
{
	if(imic.off_ste != OFF_NONE) {
		//# volt :  XX.xx V format
		//# temp : XXX.x 'C format
		mic_send_msg(CMD_HOST_EXIT, imic.off_ste, imic.off_val);
	}

	if(imic.fd >= 0) {
		close(imic.fd);
	}

	printf(" [dev ] micom disconnect(0x%x-%d)!\n", imic.off_ste, imic.off_val);

	return 0;
}
