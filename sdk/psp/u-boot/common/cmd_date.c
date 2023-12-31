/*
 * (C) Copyright 2001
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
 * RTC, Date & Time support: get and set date & time
 */
#include <common.h>
#include <command.h>
#include <rtc.h>
#include <i2c.h>

DECLARE_GLOBAL_DATA_PTR;

const char *weekdays[] = {
	"Sun", "Mon", "Tues", "Wednes", "Thurs", "Fri", "Satur",
};

#ifdef CONFIG_RELOC_FIXUP_WORKS
#define RELOC(a)	a
#else
#define RELOC(a)	((typeof(a))((unsigned long)(a) + gd->reloc_off))
#endif

int mk_date (char *, struct rtc_time *);

//#-------------- from linux kernel --------------------------------
/*
 * Does the rtc_time represent a valid date/time?
 */
/*
 * The number of days in the month.
 */
static const unsigned char rtc_days_in_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int is_leap_year(unsigned int year)
{
	return (!(year % 4) && (year % 100)) || !(year % 400);
}

static int rtc_month_days(unsigned int month, unsigned int year)
{
	return rtc_days_in_month[month] + (is_leap_year(year) && month == 1);
}

static int rtc_valid_tm(struct rtc_time *tm)
{
	if (tm->tm_year < 70
		|| ((unsigned)tm->tm_mon) >= 12
		|| tm->tm_mday < 1
		|| tm->tm_mday > rtc_month_days(tm->tm_mon, tm->tm_year + 1900)
		|| ((unsigned)tm->tm_hour) >= 24
		|| ((unsigned)tm->tm_min) >= 60
		|| ((unsigned)tm->tm_sec) >= 60)
		return -1;

	return 0;
}

int do_date (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	struct rtc_time tm;
	int rcode = 0;
	int old_bus;

	/* switch to correct I2C bus */
	old_bus = I2C_GET_BUS();

#ifdef COMMON_UD_FEATURE
	if(old_bus != CONFIG_SYS_RTC_BUS_NUM)
		I2C_SET_BUS(CONFIG_SYS_RTC_BUS_NUM);
#else
	I2C_SET_BUS(CONFIG_SYS_RTC_BUS_NUM);
#endif

	switch (argc) {
	case 2:			
		/* rtc valid */
		if (strcmp(argv[1],"valid") == 0) {
			rcode = rtc_get (&tm);
			if (rcode) {
				puts("## Get date failed\n");
				break;
			}
			rcode = rtc_valid_tm (&tm);
			if (rcode) {
				puts("## Invalid date, reset time(2000.01.01.12:00:00)...\n");
				/* MMDDhhmm[[CC]YY][.ss] */
				mk_date ("010112002000", &tm);
			}
		}
		/* set date & time */
		else if (strcmp(argv[1],"reset") == 0) {
			puts ("Reset RTC...\n");
			rtc_reset ();
		} else {
			/* initialize tm with current time */
			rcode = rtc_get (&tm);

			if(!rcode) {
				/* insert new date & time */
				if (mk_date (argv[1], &tm) != 0) {
					puts ("## Bad date format\n");
					break;
				}
				/* and write to RTC */
				rcode = rtc_set (&tm);
				if(rcode)
					puts("## Set date failed\n");
			} else {
				puts("## Get date failed\n");
			}
		}
		/* FALL TROUGH */
	case 1:			/* get date & time */
		rcode = rtc_get (&tm);

		if (rcode) {
			puts("## Get date failed\n");
			break;
		}

		printf ("Date: %4d-%02d-%02d (%sday)    Time: %2d:%02d:%02d\n",
			tm.tm_year, tm.tm_mon, tm.tm_mday,
			(tm.tm_wday<0 || tm.tm_wday>6) ?
				"unknown " : RELOC(weekdays[tm.tm_wday]),
			tm.tm_hour, tm.tm_min, tm.tm_sec);

		break;
	default:
		cmd_usage(cmdtp);
		rcode = 1;
	}

	/* switch back to original I2C bus */
	I2C_SET_BUS(old_bus);

	return rcode;
}

/*
 * simple conversion of two-digit string with error checking
 */
static int cnvrt2 (char *str, int *valp)
{
	int val;

	if ((*str < '0') || (*str > '9'))
		return (-1);

	val = *str - '0';

	++str;

	if ((*str < '0') || (*str > '9'))
		return (-1);

	*valp = 10 * val + (*str - '0');

	return (0);
}

/*
 * Convert date string: MMDDhhmm[[CC]YY][.ss]
 *
 * Some basic checking for valid values is done, but this will not catch
 * all possible error conditions.
 */
int mk_date (char *datestr, struct rtc_time *tmp)
{
	int len, val;
	char *ptr;

	ptr = strchr (datestr,'.');
	len = strlen (datestr);

	/* Set seconds */
	if (ptr) {
		int sec;

		*ptr++ = '\0';
		if ((len - (ptr - datestr)) != 2)
			return (-1);

		len = strlen (datestr);

		if (cnvrt2 (ptr, &sec))
			return (-1);

		tmp->tm_sec = sec;
	} else {
		tmp->tm_sec = 0;
	}

	if (len == 12) {		/* MMDDhhmmCCYY	*/
		int year, century;

		if (cnvrt2 (datestr+ 8, &century) ||
		    cnvrt2 (datestr+10, &year) ) {
			return (-1);
		}
		tmp->tm_year = 100 * century + year;
	} else if (len == 10) {		/* MMDDhhmmYY	*/
		int year, century;

		century = tmp->tm_year / 100;
		if (cnvrt2 (datestr+ 8, &year))
			return (-1);
		tmp->tm_year = 100 * century + year;
	}

	switch (len) {
	case 8:			/* MMDDhhmm	*/
		/* fall thru */
	case 10:		/* MMDDhhmmYY	*/
		/* fall thru */
	case 12:		/* MMDDhhmmCCYY	*/
		if (cnvrt2 (datestr+0, &val) ||
		    val > 12 || val < 1) {
			break;
		}
		tmp->tm_mon  = val;
		if (cnvrt2 (datestr+2, &val) ||
		    val > ((tmp->tm_mon==2) ? 29 : 31)) {
			break;
		}
		tmp->tm_mday = val;

		if (cnvrt2 (datestr+4, &val) ||
		    val > 23) {
			break;
		}
		tmp->tm_hour = val;

		if (cnvrt2 (datestr+6, &val) ||
		    val > 59) {
			break;
		}
		tmp->tm_min  = val;

		/* calculate day of week */
		GregorianDay (tmp);

		return (0);
	default:
		break;
	}

	return (-1);
}

/***************************************************/

U_BOOT_CMD(
	date,	2,	1,	do_date,
	"get/set/reset date & time",
	"[MMDDhhmm[[CC]YY][.ss]]\ndate reset\n"
	"  - without arguments: print date & time\n"
	"  - with numeric argument: set the system date & time\n"
	"  - with 'reset' argument: reset the RTC"
);
