/*
 * File : dev_buzzer.c
 *
 * Copyright (C) 2014 UDWORKs
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
 * this implements a buzzer hardware library.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <dev_common.h>
#include <dev_buzzer.h>

#define BEEP_DRV_BASE_PATH		"/sys/class/backlight/pwm-backlight/brightness"

/*************************************************************
 * NAME : int dev_buzzer_init(void)
 ************************************************************/
void dev_buzzer_enable(int en)
{
	char buf[128] = {0,};
	int brightness;
	int fd, len;

	if (en) brightness = 50;
	else	brightness = 0;

	/* initialize beep driver */
	len = snprintf(buf, sizeof(buf), "%d", brightness);
	if (len <= 0)
		return;

	if ((fd = open(BEEP_DRV_BASE_PATH, O_WRONLY)) < 0) {
		dev_err("Error open %s\n", BEEP_DRV_BASE_PATH);
		return;
	}

	write(fd, buf, len);
	close(fd);
}
