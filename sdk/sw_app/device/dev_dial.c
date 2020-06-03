/*
 * File : dev_dial.c
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
 * this implements a modem(3G/4G) hardware library for B10.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dev_common.h"
#include "dev_dial.h"

struct dial_usb_list_t {
	int d_vid;
	int d_pid;
};

static struct dial_usb_list_t dial_list[DIAL_USB_MAX_NUM] = {
	{
		.d_vid = PIX_DIAL_VID,
		.d_pid = PIX_DIAL_PID,
	},
	{
		.d_vid = HUC_DIAL_VID,
		.d_pid = HUC_DIAL_PID,
	},
	{
		.d_vid = HIV_DIAL_VID,
		.d_pid = HIV_DIAL_PID,
	}
};

/****************************************************
 * NAME : int dev_dial_is_exist(int *dst_vid, int *dst_pid)
 ****************************************************/
int dev_dial_is_exist(int *dst_vid, int *dst_pid)
{
	struct dial_usb_list_t *pusb;
	int i, ret;

	*dst_vid = 0; *dst_pid = 0;

	pusb = dial_list;
	for (i = 0; i < ARRAY_SIZE(dial_list); i++, pusb++) {
		ret = dev_usb_is_exist(pusb->d_vid, pusb->d_pid);
		if (ret) {
			dev_dbg("find device [%x, %x]\n", pusb->d_vid, pusb->d_pid);
			*dst_vid = pusb->d_vid;  *dst_pid = pusb->d_pid;
			return 0;
		}
	}

	return (-1); //# error
}
