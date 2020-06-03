/*
 * dev_dial.h
 *
 * Copyright (C) 2013 UDWORKs.
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
 * modem(3G/LTE) Hardware library interface definitions.
 */

#ifndef _DEV_DIAL_H_
#define _DEV_DIAL_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DIAL_USB_MAX_NUM	3

#define PIX_DIAL_VID		0x15a9
#define PIX_DIAL_PID		0x002d

#define HUC_DIAL_VID		0x22de
#define HUC_DIAL_PID		0x9033

#define HIV_DIAL_VID		0x258d
#define HIV_DIAL_PID		0x2000

int dev_dial_is_exist(int *dst_vid, int *dst_pid);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif //# _DEV_DIAL_H_
