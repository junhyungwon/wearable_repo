/*
 * dev_accel.h
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
 * the Sensor (Temperature, 3-xais Accelerometer) Hardware library
 * interface definitions.
 */

#ifndef _DEV_ACCEL_H_
#define _DEV_ACCEL_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
	int x;
	int y;
	int z;

} accel_data_t;

int dev_accel_init(void);
int dev_accel_start(void);
int dev_accel_stop(void);
int dev_accel_set_delay(int delay_ms);
int dev_accel_read(accel_data_t *pdata, int timeout);
void dev_accel_exit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif //# _DEV_ACCEL_H_

