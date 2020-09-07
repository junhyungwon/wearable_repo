/*
 * Copyright (C) UDWORKs, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */
#ifndef __TMP105_H__
#define __TMP105_H__

#define TMP105_CONF_SHUTDOWN	 	0x1<<0
#define TMP105_CONF_NORMAL		 	0x0<<0
#define TMP105_CONF_COMPARATOR_MODE	0x0<<1
#define TMP105_CONF_INTERRUPT_MODE	0x1<<1
#define TMP105_CONF_RISING_EDGE	 	0x1<<2
#define TMP105_CONF_FALLING_EDGE 	0x0<<2
#define TMP105_CONF_FAULT_QUEUE(a)	(a&0x3)<<3
#define TMP105_CONF_RESOLUTION(a)	(a&0x3)<<5
#define TMP105_CONF_ONESHOT			0x1<<7

struct tmp105_platform_data {
	u8 config;
	int poll_interval;
	int (*init)(void);
	void (*exit)(void);
};
#endif  /* __TMP105_H__ */
