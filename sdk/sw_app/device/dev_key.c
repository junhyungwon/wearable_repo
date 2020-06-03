/*
 * File : dev_key.c
 *
 * Copyright (C) 2013 UDWORKs
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * this implements a key-event hardware library for UBX.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <poll.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

#include <linux/input.h>
#include <osa.h>

#include "dev_common.h"
#include "dev_key.h"

#define KEY_DEV_NAME 	"gpio-keys"

static int kbd_fd = -1;

/*****************************************************************************
* @brief    key function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void input_flush(int fd)
{
	struct timeval tv;
	struct input_event ev;

    fd_set fdset;
    int nfds;

    while (1) {
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        nfds = select(fd + 1, &fdset, NULL, NULL, &tv);
        if (nfds == 0)
        	break;

		if (read(fd, &ev, sizeof(struct input_event)) < 0)
			break;
    }
}

/****************************************************
 * NAME : void dev_key_flush(void)
 *
 * Desc : flush input-subsystem buffer
 *
 ****************************************************/
void dev_key_flush(void)
{
	input_flush(kbd_fd);
}

/****************************************************
 * NAME : int dev_key_state_read(int type, int code)
 *
 * Desc   : Get current key state.
 *
 * INPUT  :
 *   PARAMETERS:
 *         int type : event type. (EV_SW, EV_KEY etc...)
 *         int code : event code.
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values: 0        nothing
 *               1        detected event
 *               -1       invalid type or code.
 ****************************************************/
int dev_key_state_read(int type, int code)
{
	int ret;
	char state[KEY_MAX > SW_MAX ? KEY_MAX:SW_MAX];
	int cmd = (type == EV_KEY) ? EVIOCGKEY(KEY_MAX):EVIOCGSW(SW_MAX);

	ret = ioctl(kbd_fd, cmd, state);
	if (ret == -1) {
		dev_err("ioctl failed!\n");
		return -1;
	}

	return TEST_BIT(code, state);
}

/****************************************************
 * NAME : int dev_key_read(int *longkey, int timeout)
 *
 * Desc   : Get current key code from input-core.
 *
 * INPUT  :
 *   PARAMETERS:
 *         int *longkey : buffer for key code.
 *         int timeout : timeout (-1 forever)
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values:    0          event not present. (nothing)
 *            nonzero( > 0)    event key code
 *                 -1          invalid type or code.
 ****************************************************/
int dev_key_read(struct key_recv_data_t *data, int timeout)
{
	struct pollfd e_poll;
	struct input_event ev;

	int res = 0;
	int exit = 0;

	if (kbd_fd < 0 || data == NULL) {
		dev_err("invalid params\n");
		return -1;
	}

	memset((void *)&e_poll, 0, sizeof(struct pollfd));
	e_poll.fd     = kbd_fd;
	e_poll.events = POLLIN | POLLERR;
	e_poll.revents = 0;

	do {
		res = poll(&e_poll, 1, timeout);
		if (res < 0) {
			dev_err("poll() failed!..exit\n");
			return -1;
		}
		if (res == 0) {
			/* timeout failed!! */
			return -1;
		}
		if (e_poll.revents & POLLERR)
        	/* poll error!! */
        	return -1;

		if (e_poll.revents & POLLIN) {
			/* received events */
			res = read(kbd_fd, &ev, sizeof(struct input_event));
			if (res < (int) sizeof(struct input_event)) {
				//dev_err("error reading\n");
				return -1;
			}

			if (ev.type == EV_KEY) {
				data->code = ev.code;
				data->value = ev.value;
				return 0;
			}
		}
	} while (exit == 0);

	return (-1);
}

/****************************************************
 * NAME : int dev_key_init(void)
 *
 * Desc   : initialize key.
 *
 * INPUT  :
 *   PARAMETERS:
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values:    0    succeed.
 *                 -1    failure.
 ****************************************************/
int dev_key_init(void)
{
	char input_path[256] = {};
	int num, ret;

	if (kbd_fd >= 0)
		return -1; /* not supported multi-process */

	num = dev_input_get_bus_num(KEY_DEV_NAME);
	if (num < 0) {
		dev_err("invalid device name %s\n", KEY_DEV_NAME);
		return -1;
	}
	sprintf(input_path, "/dev/input/event%d", num);

	/* blocking mode */
	ret = open(input_path, O_RDONLY);
	if (ret < 0) {
		dev_err("Failed to open %s device.\n", input_path);
		return -1;
	}
	kbd_fd = ret;

	return 0;
}

/****************************************************
 * NAME : void dev_key_exit(void)
 *
 * Desc   : Deinitialize key Hardware.
 *
 * INPUT  :
 *   PARAMETERS : none.
 *
 * OUTPUT :
 *   RETURN : none
 ****************************************************/
void dev_key_exit(void)
{
	if (kbd_fd >= 0) {
		close(kbd_fd);
		kbd_fd = -1;
	}
}
