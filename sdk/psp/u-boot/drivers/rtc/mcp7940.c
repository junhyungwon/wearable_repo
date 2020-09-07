/*
 * (C) Copyright 2001-2008
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 * Keith Outwater, keith_outwater@mvis.com`
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Date & Time support (no alarms) for MCP7940
 */

#include <common.h>
#include <command.h>
#include <rtc.h>
#include <i2c.h>

#if defined(CONFIG_CMD_DATE)
#ifndef	CONFIG_SYS_I2C_RTC_ADDR
#define	CONFIG_SYS_I2C_RTC_ADDR		0x6f
#endif

/*---------------------------------------------------------------------*/
#undef DEBUG_RTC

#ifdef DEBUG_RTC
#define DEBUGR(fmt,args...) printf(fmt ,##args)
#else
#define DEBUGR(fmt,args...)
#endif
/*---------------------------------------------------------------------*/

/* RTC registers don't differ much, except for the century flag */
#define MCP7940_REG_SECS			0x00	/* 00-59 */
#	define MCP7940_BIT_CH			0x80
#	define DS1340_BIT_nEOSC			0x80
#	define MCP7940_BIT_ST          	0x80
#define MCP7940_REG_MIN				0x01	/* 00-59 */
#define MCP7940_REG_HOUR			0x02	/* 00-23, or 1-12{am,pm} */
#	define MCP7940_BIT_12HR			0x40	/* in REG_HOUR */
#	define MCP7940_BIT_PM			0x20	/* in REG_HOUR */
#	define DS1340_BIT_CENTURY_EN	0x80	/* in REG_HOUR */
#	define DS1340_BIT_CENTURY		0x40	/* in REG_HOUR */
#define MCP7940_REG_WDAY			0x03	/* 01-07 */
#define MCP7940_REG_MDAY			0x04	/* 01-31 */
#define MCP7940_REG_MONTH			0x05	/* 01-12 */
#	define DS1337_BIT_CENTURY		0x80	/* in REG_MONTH */
#define MCP7940_REG_YEAR			0x06	/* 00-99 */
#      define MCP7940_BIT_VBATEN    0x08

/* Other registers (control, status, alarms, trickle charge, NVRAM, etc)
 * start at 7, and they differ a LOT. Only control and status matter for
 * basic RTC date and time functionality; be careful using them.
 */
#define MCP7940_REG_CONTROL			0x07		/* or ds1338 */
#	define MCP7940_BIT_OUT			0x80
#	define DS1338_BIT_OSF			0x20
#	define MCP7940_BIT_SQWE			0x10
#	define MCP7940_BIT_RS1			0x02
#	define MCP7940_BIT_RS0			0x01
#define DS1337_REG_CONTROL			0x0e
#	define DS1337_BIT_nEOSC			0x80
#	define DS1339_BIT_BBSQI			0x20
#	define DS3231_BIT_BBSQW			0x40 /* same as BBSQI */
#	define DS1337_BIT_RS2			0x10
#	define DS1337_BIT_RS1			0x08
#	define DS1337_BIT_INTCN			0x04
#	define DS1337_BIT_A2IE			0x02
#	define DS1337_BIT_A1IE			0x01
#define DS1340_REG_CONTROL			0x07
#	define DS1340_BIT_OUT			0x80
#	define DS1340_BIT_FT			0x40
#	define DS1340_BIT_CALIB_SIGN	0x20
#	define DS1340_M_CALIBRATION		0x1f
#define DS1340_REG_FLAG				0x09
#	define DS1340_BIT_OSF			0x80
#define DS1337_REG_STATUS			0x0f
#	define DS1337_BIT_OSF			0x80
#	define DS1337_BIT_A2I			0x02
#	define DS1337_BIT_A1I			0x01
#define DS1339_REG_ALARM1_SECS		0x07
#define DS1339_REG_TRICKLE			0x10

#define RX8025_REG_CTRL1			0x0e
#	define RX8025_BIT_2412			0x20
#define RX8025_REG_CTRL2			0x0f
#	define RX8025_BIT_PON			0x10
#	define RX8025_BIT_VDET			0x40
#	define RX8025_BIT_XST			0x20

static uchar rtc_read (uchar reg);
static void rtc_write (uchar reg, uchar val);

/*
 * Get the current time from the RTC
 */
int rtc_get (struct rtc_time *tmp)
{
	int rel = 0, status;
	uchar sec, min, hour, mday, wday, mon_cent, year;

	status = rtc_read (MCP7940_REG_WDAY);
	/* make sure that the backup battery is enabled */
	if (!(status & MCP7940_BIT_VBATEN))  {
		rtc_write(MCP7940_REG_WDAY, status | MCP7940_BIT_VBATEN);
	}

	/* clock halted?  turn it on, so clock can tick. */
	status = rtc_read (MCP7940_REG_SECS);
	if (!(status & MCP7940_BIT_ST)) {
		printf ("### Warning: RTC oscillator has stopped\n");
		rtc_write(MCP7940_REG_SECS, MCP7940_BIT_ST);
		rel = -1;
	}

	status = rtc_read (MCP7940_REG_HOUR);
	if ((status & MCP7940_BIT_12HR)) {
		/* Be sure we're in 24 hour mode.  Multi-master systems
		 * take note...
		 */
		status = bcd2bin(status & 0x1f);
		if (status == 12)
			status = 0;

		if (status & MCP7940_BIT_PM)
			status += 12;

		rtc_write(MCP7940_REG_HOUR, bin2bcd(status));
	}

	sec = rtc_read(MCP7940_REG_SECS);
	min = rtc_read(MCP7940_REG_MIN);
	hour = rtc_read(MCP7940_REG_HOUR);
	wday = rtc_read(MCP7940_REG_WDAY);
	mday = rtc_read(MCP7940_REG_MDAY);
	mon_cent = rtc_read(MCP7940_REG_MONTH);
	year = rtc_read(MCP7940_REG_YEAR);

	DEBUGR ("Get RTC year: %02x mon/cent: %02x mday: %02x wday: %02x "
		"hr: %02x min: %02x sec: %02x control: %02x status: %02x\n",
		year, mon_cent, mday, wday, hour, min, sec);

	tmp->tm_sec  = bcd2bin (sec & 0x7F);
	tmp->tm_min  = bcd2bin (min & 0x7F);
	tmp->tm_hour = bcd2bin (hour & 0x3F);
	tmp->tm_mday = bcd2bin (mday & 0x3F);
	tmp->tm_mon  = bcd2bin (mon_cent & 0x1F);
	tmp->tm_year = bcd2bin (year) + 2000;
	tmp->tm_wday = bcd2bin (wday & 0x07) - 1;
	tmp->tm_yday = 0;
	tmp->tm_isdst= 0;

	DEBUGR ("Get DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	return 0;
}

/*
 * Set the RTC
 */
int rtc_set (struct rtc_time *tmp)
{
	DEBUGR ("Set DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	/* assume 20YY not 19YY */
	rtc_write (MCP7940_REG_YEAR, bin2bcd (tmp->tm_year % 100));
	rtc_write (MCP7940_REG_MONTH, bin2bcd (tmp->tm_mon));
	rtc_write (MCP7940_REG_WDAY, bin2bcd (tmp->tm_wday + 1) | MCP7940_BIT_VBATEN);
	rtc_write (MCP7940_REG_MDAY, bin2bcd (tmp->tm_mday));
	rtc_write (MCP7940_REG_HOUR, bin2bcd (tmp->tm_hour));
	rtc_write (MCP7940_REG_MIN, bin2bcd (tmp->tm_min));
	rtc_write (MCP7940_REG_SECS, bin2bcd (tmp->tm_sec) | MCP7940_BIT_ST);

	return 0;
}

void rtc_reset (void)
{
	/* null */
}

/*
 * Helper functions
 */
static uchar rtc_read (uchar reg)
{
	return (i2c_reg_read (CONFIG_SYS_I2C_RTC_ADDR, reg));
}

static void rtc_write (uchar reg, uchar val)
{
	i2c_reg_write (CONFIG_SYS_I2C_RTC_ADDR, reg, val);
}

#endif /* #if defined(CONFIG_CMD_DATE) */
