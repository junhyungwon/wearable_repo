/*
 * File : dev_accel.c
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
 * this implements a linux input hardware library for UBX.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <ctype.h>
#include <poll.h>
#include <limits.h>
#include <errno.h>

#include <linux/input.h>

#include "dev_common.h"
#include "dev_accel.h"

#define ACCEL_NAME		"kxcj9_accel"

static int opened = 0; /* only 1 device supported */
static int accel_fd = -1;

/****************************************************
 * NAME : int accel_init(const char *dev_name)
 *
 * Desc   : initialize the linux input sub-system
 *
 * INPUT  :
 *   PARAMETERS:
 *        const char *dev_name  : input device name.
 *
 * OUTPUT :
 *   RETURN : error code
 *       Type  : int
 *       Values:
 *         -1	failure.
 *         0    success.
 ****************************************************/
int dev_accel_init(void)
{
	char input_path[256] = {};
	int bus, fd;

	if (opened)
		return -1;

	opened = 1;
	bus = dev_input_get_bus_num(ACCEL_NAME);
	if (bus < 0){
		dev_err("invalid device name %s\n", ACCEL_NAME);
		return -1;
	}

	sprintf(input_path, "/dev/input/event%d", bus);
	fd = open(input_path, O_RDONLY);
	if (fd < 0) {
		dev_err("invalid input path %s\n", input_path);
		return -1;
	}
	accel_fd = fd;

	return 0;
}

/****************************************************
 * NAME : int dev_accel_start(void)
 ****************************************************/
int dev_accel_start(void)
{
	FILE *f;
	char path[256] = {};

	int bus;

	bus = dev_input_get_bus_num(ACCEL_NAME);
	if (bus < 0)
		return -1;

	memset(path, 0, sizeof(path));
	sprintf(path, "/sys/class/input/input%d/device/enable", bus);
	f = fopen(path, "w");
	if (f == NULL) {
		dev_err("invalid accelrometer path %s\n", path);
		return -1;
	}

	fputs("1\n", f);
	fclose(f);

	return 0;
}

/****************************************************
 * NAME : int dev_accel_stop(void)
 ****************************************************/
int dev_accel_stop(void)
{
	FILE *f;
	char path[256] = {};

	int bus;

	bus = dev_input_get_bus_num(ACCEL_NAME);
	if (bus < 0)
		return -1;

	memset(path, 0, sizeof(path));
	sprintf(path, "/sys/class/input/input%d/device/enable", bus);
	f = fopen(path, "w");
	if (f == NULL) {
		dev_err("invalid accelrometer path %s\n", path);
		return -1;
	}

	fputs("0\n", f);
	fclose(f);

	return 0;
}

/****************************************************
 * NAME : int dev_accel_stop(void)
 ****************************************************/
int dev_accel_set_delay(int delay_ms)
{
	FILE *f;
    char path[256] = {};
	int bus;

	bus = dev_input_get_bus_num(ACCEL_NAME);
	if (bus < 0)
		return -1;

	memset(path, 0, sizeof(path));
	sprintf(path, "/sys/class/input/input%d/device/poll_delay", bus);
    f = fopen(path, "w");
    if (f != NULL)
    {
        char buf[80];
        sprintf(buf, "%d\n", delay_ms);
        fputs(buf, f);
        fclose(f);
        return 0;
    }
    return -1;
}

/****************************************************
 * NAME : int accel_read(accel_data_t *pdata, int timeout)
 *
 * Desc   : Get input value from input device.
 *
 * INPUT  :
 *   PARAMETERS:
 *        accel_data_t *pdata  : input device handle.
 *
 *
 * OUTPUT :
 *   RETURN : Error Code.
 *       Type  : int
 *       Values:
 *         0     success.
 *         -1    failure.
 ****************************************************/
int dev_accel_read(accel_data_t *pdata, int timeout)
{
	struct input_event ev;
	struct pollfd e_poll;

	int exit = 0;
	int res = -1;

	if (pdata == NULL || accel_fd < 0) {
		dev_err("invalid params\n");
		return -1;
	}

	memset((void *)&e_poll, 0, sizeof(struct pollfd));
	e_poll.fd     = accel_fd;
	e_poll.events = POLLIN | POLLERR;

	while (!exit)
	{
		res = poll((struct pollfd *)&e_poll, 1, timeout);
		if (res < 0) {
			dev_err("poll() failed!..exit\n");
			return -1;
		}
		if (res == 0) {
			/* timeout failed!! */
//			dev_err("timeout failed!..exit\n");
			return -1;
		}
		if (e_poll.revents & POLLERR) {
        	/* poll error!! */
//        	dev_err("poll error!..exit\n");
        	return -1;
        }

		if (e_poll.revents & POLLIN) {
			/* received events */
			res = read(accel_fd, &ev, sizeof(struct input_event));
			if (res < (int)sizeof(struct input_event)) {
//				dev_err("accelrometer input error!!\n");
				return -1;
			}

			switch (ev.type) {
			case EV_ABS:
				switch (ev.code) {
				/* X and Y now reflect latest measurement */
				case ABS_X:
					pdata->x = ev.value;
					break;

				case ABS_Y:
					pdata->y = ev.value;
					break;

				case ABS_Z:
					pdata->z = ev.value;
					break;
				}
				break;

			case EV_SYN:
				/* read done!! */
				return 0;
			}
		}
	} /* while (!exit)*/

	return (-1);
}

/****************************************************
 * NAME : void accel_exit(void)
 *
 * Desc   : Deinitialize input device.
 *
 * INPUT  :
 *   PARAMETERS:
 *
 *
 * OUTPUT :
 *   RETURN : none.
 ****************************************************/
void dev_accel_exit(void)
{
	if (accel_fd >= 0) {
		close(accel_fd);
	}
	opened = 0;
}
