/******************************************************************************
 * NEXTT360 Board
 * Copyright by LF, Incoporated. All Rights Reserved.
 * based on gpsd.
 *  refer to GPSD project
 *---------------------------------------------------------------------------*/
 /**
 * @file    nmea.c
 * @brief
 * @section MODIFY history
 *     - 2020.07.21 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

#include "gnss_ipc_cmd_defs.h"
#include "nmea_parse.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define __PARSER_DBG__ 		0

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

enum {
	GROUND_STATE,	/* we don't know what packet type to expect */

	COMMENT_BODY,	/* pound comment for a test load */
	COMMENT_RECOGNIZED,	/* comment recognized */
	NMEA_DOLLAR,		/* we've seen first character of NMEA leader */

	NMEA_BANG,		/* we've seen first character of an AIS message '!' */
	NMEA_PUB_LEAD,	/* seen second character of NMEA G leader */
	NMEA_VENDOR_LEAD,	/* seen second character of NMEA P leader */
	NMEA_LEADER_END,	/* seen end char of NMEA leader, in body */
	NMEA_PASHR_A,	/* grind through recognizing $PASHR */
	NMEA_PASHR_S,	/* grind through recognizing $PASHR */
	NMEA_PASHR_H,	/* grind through recognizing $PASHR */
	NMEA_BINARY_BODY,	/* Ashtech-style binary packet body, skip until \r\n */
	NMEA_BINARY_CR,	/* \r on end of Ashtech-style binary packet */
	NMEA_BINARY_NL,	/* \n on end of Ashtech-style binary packet */
	NMEA_CR,	   	/* seen terminating \r of NMEA packet */
	NMEA_RECOGNIZED,	/* saw trailing \n of NMEA packet */
};

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------------------------
static int nmeaid_to_prn(char *talker, int nmea_satnum, int nmea_gnssid,
                         unsigned char *ubx_gnssid, unsigned char *ubx_svid)
{
    int nmea2_prn = nmea_satnum;

    *ubx_gnssid = 0;   /* default to ubx_gnssid is GPS */
    *ubx_svid = 0;     /* default to unnknown ubx_svid */

    if (1 > nmea_satnum) {
        /* uh, oh... */
        nmea2_prn = 0;
    } else if (0 < nmea_gnssid) {
        /* this switch handles case where nmea_gnssid is known */
        switch (nmea_gnssid) {
        default:
            /* x = IMES                Not defined by NMEA 4.10 */
            /* FALLTHROUGH */
        case 0:
            /* none given, ignore */
            nmea2_prn = 0;
            break;
        case 1:
            if (33 > nmea_satnum) {
                /* 1 = GPS       1-32 */
                *ubx_gnssid = 0;
                *ubx_svid = nmea_satnum;
            } else if (65 > nmea_satnum) {
                /* 1 = SBAS      33-64 */
                *ubx_gnssid = 1;
                *ubx_svid = 87 + nmea_satnum;
            } else if (152 > nmea_satnum) {
                /* Huh? */
                *ubx_gnssid = 0;
                *ubx_svid = 0;
                nmea2_prn = 0;
            } else if (158 > nmea_satnum) {
                /* 1 = SBAS      152-158 */
                *ubx_gnssid = 1;
                *ubx_svid = nmea_satnum;
            } else if (193 > nmea_satnum) {
                /* Huh? */
                *ubx_gnssid = 0;
                *ubx_svid = 0;
                nmea2_prn = 0;
            } else if (200 > nmea_satnum) {
                /* 1 = QZSS      193-197 */
                /* undocumented u-blox goes to 199 */
                *ubx_gnssid = 3;
                *ubx_svid = nmea_satnum - 192;
            } else {
                /* Huh? */
                *ubx_gnssid = 0;
                *ubx_svid = 0;
                nmea2_prn = 0;
            }
            break;
        case 2:
            /*  2 = GLONASS   65-96, nul */
            *ubx_gnssid = 6;
            *ubx_svid = nmea_satnum;
            break;
        case 3:
            /*  3 = Galileo   1-36 */
            *ubx_gnssid = 2;
            *ubx_svid = nmea_satnum;
            nmea2_prn = 300 + nmea_satnum;
            break;
        case 4:
            /*  4 - BeiDou    1-37 */
            *ubx_gnssid = 3;
            *ubx_svid = nmea_satnum;
            nmea2_prn = 300 + nmea_satnum;
            break;
        }

    /* left with NMEA 2.x to NMEA 4.0 satnums
     * use talker ID to disambiguate */
    } else if (32 >= nmea_satnum) {
        *ubx_svid = nmea_satnum;
        switch (talker[0]) {
        case 'G':
            if (talker[1] == 'A') {
                /* Galileo */
                nmea2_prn = 300 + nmea_satnum;
                *ubx_gnssid = 2;
            } else if (talker[1] == 'B') {
                /* map Beidou IDs 1..37 to 401..437 */
                *ubx_gnssid = 3;
                nmea2_prn = 400 + nmea_satnum;
            } else if (talker[1] == 'L') {
                /* GLONASS GL doesn't seem to do this, better safe than sorry */
                nmea2_prn = 64 + nmea_satnum;
                *ubx_gnssid = 6;
            } else if (talker[1] == 'N') {
                /* all of them, but only GPS is 0 < PRN < 33 */
            } else if (talker[1] == 'P') {
                /* GPS,SBAS,QZSS, but only GPS is 0 < PRN < 33 */
            } /* else ?? */
            break;
        case 'B':
            if (talker[1] == 'D') {
                /* map Beidou IDs */
                nmea2_prn = 400 + nmea_satnum;
                *ubx_gnssid = 3;
            } /* else ?? */
            break;
        case 'P':
            /* Quectel EC25 & EC21 use PQxxx for BeiDou */
            if (talker[1] == 'Q') {
                /* map Beidou IDs */
                nmea2_prn = 400 + nmea_satnum;
                *ubx_gnssid = 3;
            } /* else ?? */
            break;
        case 'Q':
            if (talker[1] == 'Z') {
                /* QZSS */
                nmea2_prn = 192 + nmea_satnum;
                *ubx_gnssid = 5;
            } /* else ? */
            break;
        default:
            /* huh? */
            break;
        }
    } else if (64 >= nmea_satnum) {
        // NMEA-ID (33..64) to SBAS PRN 120-151.
        /* SBAS */
        *ubx_gnssid = 1;
        *ubx_svid = 87 + nmea_satnum;
    } else if (96 >= nmea_satnum) {
        /* GLONASS 65..96  */
        *ubx_gnssid = 6;
        *ubx_svid = nmea_satnum - 64;
    } else if (120 > nmea_satnum) {
        /* Huh? */
        *ubx_gnssid = 0;
        *ubx_svid = 0;
        nmea2_prn = 0;
    } else if (158 >= nmea_satnum) {
        /* SBAS 120..158 */
        *ubx_gnssid = 1;
        *ubx_svid = nmea_satnum;
    } else if (173 > nmea_satnum) {
        /* Huh? */
        *ubx_gnssid = 0;
        *ubx_svid = 0;
        nmea2_prn = 0;
    } else if (182 >= nmea_satnum) {
        /* IMES 173..182 */
        *ubx_gnssid = 4;
        *ubx_svid = nmea_satnum - 172;
    } else if (193 > nmea_satnum) {
        /* Huh? */
        *ubx_gnssid = 0;
        *ubx_svid = 0;
        nmea2_prn = 0;
    } else if (197 >= nmea_satnum) {
        /* QZSS 193..197 */
        /* undocumented u-blox goes to 199 */
        *ubx_gnssid = 5;
        *ubx_svid = nmea_satnum - 192;
    } else if (201 > nmea_satnum) {
        /* Huh? */
        *ubx_gnssid = 0;
        *ubx_svid = 0;
        nmea2_prn = 0;
    } else if (237 >= nmea_satnum) {
        /* BeiDou, non-standard, some SiRF put BeiDou 201-237 */
        /* $GBGSV,2,2,05,209,07,033,*62 */
        *ubx_gnssid = 3;
        *ubx_svid = nmea_satnum - 200;
        nmea2_prn += 200;           /* move up to 400 where NMEA 2.x wants it. */
    } else if (301 > nmea_satnum) {
        /* Huh? */
        *ubx_gnssid = 0;
        *ubx_svid = 0;
        nmea2_prn = 0;
    } else if (356 >= nmea_satnum) {
        /* Galileo 301..356 */
        *ubx_gnssid = 2;
        *ubx_svid = nmea_satnum - 300;
    } else if (401 > nmea_satnum) {
        /* Huh? */
        *ubx_gnssid = 0;
        *ubx_svid = 0;
        nmea2_prn = 0;
    } else if (437 >= nmea_satnum) {
        /* BeiDou */
        *ubx_gnssid = 3;
        *ubx_svid = nmea_satnum - 400;
    } else {
        /* greater than 437 Huh? */
        *ubx_gnssid = 0;
        *ubx_svid = 0;
        nmea2_prn = 0;
    }

    return nmea2_prn;
}

//----------------------------------------------------------------------------------------------------------------------------
static size_t strlcpy(char *dst, const char *src, size_t siz)
{
    size_t len = strlen(src);
    if (siz != 0) {
	if (len >= siz) {
	    memcpy(dst, src, siz - 1);
	    dst[siz - 1] = '\0';
	} else
	    memcpy(dst, src, len + 1);
    }
    return len;
}

#if __PARSER_DBG__
static const char *__hexdump(char *scbuf, size_t scbuflen, char *binbuf, size_t binbuflen)
{
    size_t i, j = 0;
    size_t len =
	(size_t) ((binbuflen > MAX_PACKET_LENGTH) ? MAX_PACKET_LENGTH : binbuflen);
    const char *ibuf = (const char *)binbuf;
    const char *hexchar = "0123456789abcdef";

    if (NULL == binbuf || 0 == binbuflen)
		return "";

    for (i = 0; i < len && j < (scbuflen - 3); i++) {
		scbuf[j++] = hexchar[(ibuf[i] & 0xf0) >> 4];
		scbuf[j++] = hexchar[ibuf[i] & 0x0f];
    }
	
    scbuf[j] = '\0';
    return scbuf;
}

static const char *__packetdump(char *scbuf, size_t scbuflen, char *binbuf, size_t binbuflen)
{
    char *cp;
    bool printable = true;

    for (cp = binbuf; cp < binbuf + binbuflen; cp++)
		if (!isprint((unsigned char) *cp) && !isspace((unsigned char) *cp)) {
			printable = false;
			break;	/* no need to keep iterating */
		}
	if (printable)
		return binbuf;
    else
		return __hexdump(scbuf, scbuflen, binbuf, binbuflen);
}
#endif

static int faa_mode(char mode)
{
    int newstatus = STATUS_FIX;
	
//	dprintf("faa_mode default status %d(%c)\n", newstatus, mode);
	
	switch (mode) {
    case '\0':  /* missing */
        newstatus = STATUS_NO_FIX;
        break;
    case 'A':   /* Autonomous */
    default:
        newstatus = STATUS_FIX;
        break;
    case 'D':   /* Differential */
        newstatus = STATUS_DGPS_FIX;
        break;
    }
	
//	dprintf("return faa_mode status %d\n", newstatus);
	
    return newstatus;
}

static double safe_atof(const char *string)
{
    static int maxExponent = 511;   
    static double powersOf10[] = {
	10.,			
	100.,			
	1.0e4,
	1.0e8,
	1.0e16,
	1.0e32,
	1.0e64,
	1.0e128,
	1.0e256
    };

    bool sign, expSign = false;
    double fraction, dblExp, *d;
    const char *p;
    int c;
    int exp = 0;		
    int fracExp = 0;	
    int mantSize;		
    int decPt;			
    const char *pExp;	
    
	p = string;
    while (isspace((unsigned char) *p)) {
		p += 1;
    }
    
	if (*p == '\0') {
        return NAN;
    } else if (*p == '-') {
		sign = true;
		p += 1;
    } else {
		if (*p == '+') {
	    	p += 1;
		}
		sign = false;
    }

    decPt = -1;
    for (mantSize = 0; ; mantSize += 1)
    {
		c = *p;
		if (!isdigit(c)) {
			if ((c != '.') || (decPt >= 0)) {
			break;
			}
			decPt = mantSize;
		}
		p += 1;
    }

    pExp  = p;
    p -= mantSize;
    if (decPt < 0) {
		decPt = mantSize;
    } else {
		mantSize -= 1;			/* One of the digits was the point. */
    }
    if (mantSize > 18) {
		fracExp = decPt - 18;
		mantSize = 18;
    } else {
		fracExp = decPt - mantSize;
    }
    if (mantSize == 0) {
		fraction = 0.0;
		//p = string;
		goto done;
    } else {
		int frac1, frac2;
		frac1 = 0;
		for ( ; mantSize > 9; mantSize -= 1)
		{
			c = *p;
			p += 1;
			if (c == '.') {
			c = *p;
			p += 1;
			}
			frac1 = 10*frac1 + (c - '0');
		}
		frac2 = 0;
		for (; mantSize > 0; mantSize -= 1)
		{
			c = *p;
			p += 1;
			if (c == '.') {
			c = *p;
			p += 1;
			}
			frac2 = 10*frac2 + (c - '0');
		}
		fraction = (1.0e9 * frac1) + frac2;
    }

    p = pExp;
    if ((*p == 'E') || (*p == 'e')) {
		p += 1;
		if (*p == '-') {
			expSign = true;
			p += 1;
		} else {
			if (*p == '+') {
			p += 1;
			}
			expSign = false;
		}
		while (isdigit((unsigned char) *p)) {
			exp = exp * 10 + (*p - '0');
			p += 1;
		}
    }
    if (expSign) {
		exp = fracExp - exp;
    } else {
		exp = fracExp + exp;
    }

    if (exp < 0) {
		expSign = true;
		exp = -exp;
    } else {
		expSign = false;
    }
    if (exp > maxExponent) {
		exp = maxExponent;
		errno = ERANGE;
    }
    dblExp = 1.0;
    for (d = powersOf10; exp != 0; exp >>= 1, d += 1) {
		if (exp & 01) {
			dblExp *= *d;
		}
    }
    if (expSign) {
		fraction /= dblExp;
    } else {
		fraction *= dblExp;
    }

done:
    if (sign) {
		return -fraction;
    }
    return fraction;
}

static int do_lat_lon(char *field[], struct gps_fix_t *out)
{
    double d, m;
    double lon;
    double lat;

    if ('\0' == field[0][0] ||
        '\0' == field[1][0] ||
        '\0' == field[2][0] ||
        '\0' == field[3][0]) {
        return 1;
    }

    lat = safe_atof(field[0]);
    m = 100.0 * modf(lat / 100.0, &d);
    lat = d + m / 60.0;
    if ('S' == field[1][0])
        lat = -lat;

    lon = safe_atof(field[2]);
    m = 100.0 * modf(lon / 100.0, &d);
    lon = d + m / 60.0;
    if ('W' == field[3][0])
        lon = -lon;

    if (0 == isfinite(lat) ||
        0 == isfinite(lon)) {
        return 2;
    }

    out->latitude = lat;
    out->longitude = lon;
    return 0;
}

//############################ GPS_MATH_UTIL######################################################

///----------------------------GPS TIME UTILS#####################################################
static void TS_NORM(struct timespec *ts)
{
    if ( (  1 <= ts->tv_sec ) ||
         ( (0 == ts->tv_sec ) && (0 <= ts->tv_nsec ) ) ) {
		if ( NS_IN_SEC <= ts->tv_nsec ) {
				/* borrow from tv_sec */
			ts->tv_nsec -= NS_IN_SEC;
			ts->tv_sec++;
		} else if ( 0 > (ts)->tv_nsec ) {
				/* carry to tv_sec */
			ts->tv_nsec += NS_IN_SEC;
			ts->tv_sec--;
		}
    }  else {
		if (-NS_IN_SEC >= ts->tv_nsec ) {
			ts->tv_nsec += NS_IN_SEC;
			ts->tv_sec--;
		} else if ( 0 < ts->tv_nsec ) {
			ts->tv_nsec -= NS_IN_SEC;
			ts->tv_sec++;
		}
    }
}

#if __PARSER_DBG__
static const char *timespec_str(const struct timespec *ts, char *buf, size_t buf_size)
{
    char sign = ' ';

    if (!TS_GEZ(ts)) {
		sign = '-';
    }

    (void)snprintf(buf, buf_size, "%c%lld.%09ld",
		   sign,
		   (long long)llabs(ts->tv_sec),
		   (long)labs(ts->tv_nsec));
    return  buf;
}
#endif

static time_t mkgmtime(struct tm * t)
{
    int year;
    time_t result;
    static const int cumdays[MONTHSPERYEAR] =
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

    year = 1900 + t->tm_year + t->tm_mon / MONTHSPERYEAR;
    result = (year - 1970) * 365 + cumdays[t->tm_mon % MONTHSPERYEAR];
    result += (year - 1968) / 4;
    result -= (year - 1900) / 100;
    result += (year - 1600) / 400;
    
	if ((year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0) && (t->tm_mon % MONTHSPERYEAR) < 2)
		result--;
    
	result += t->tm_mday - 1;
    result *= 24;
    result += t->tm_hour;
    result *= 60;
    result += t->tm_min;
    result *= 60;
    result += t->tm_sec;
    
	return (result);
}

static char *timespec_to_iso8601(timespec_t fixtime, char isotime[], size_t len)
{
    struct tm when;
    char timestr[30];
    long fracsec;

    if (0 > fixtime.tv_sec) {
        return strncpy(isotime, "NaN", len);
    }
    if (999499999 < fixtime.tv_nsec) {
        /* round up */
        fixtime.tv_sec++;
        fixtime.tv_nsec = 0;
    }
	
    (void)gmtime_r(&fixtime.tv_sec, &when);

    fracsec = (fixtime.tv_nsec + 500000) / 1000000;

    (void)strftime(timestr, sizeof(timestr), "%Y-%m-%dT%H:%M:%S", &when);
    (void)snprintf(isotime, len, "%s.%03ldZ",timestr, fracsec);

    return isotime;
}

static timespec_t __utc_resolve(struct gps_device_t *session)
{
    timespec_t t;
    char scr[128];

    t.tv_sec = (time_t)mkgmtime(&session->nmea.date);
    t.tv_nsec = session->nmea.subseconds.tv_nsec;
    session->context->valid &=~ GPS_TIME_VALID;

    if (session->context->start_time < GPS_EPOCH)
		return t;

    if (17 < session->context->leap_seconds && 1483228800LL > t.tv_sec) {
        long long old_tv_sec = t.tv_sec;
        t.tv_sec += 619315200LL;                    // fast forward 1024 weeks
        (void)gmtime_r(&t.tv_sec, &session->nmea.date);   // fix NMEA date
		(void)timespec_to_iso8601(t, scr, sizeof(scr));
		
		dprintf("WARNING: WKRO bug: leap second %d inconsistent "
                 "with %lld, corrected to %lld (%s)\n",
                 session->context->leap_seconds,
		 old_tv_sec, (long long)t.tv_sec, scr);
    }

    return t;
}

static int merge_ddmmyy(char *ddmmyy, struct gps_device_t *session)
{
    int yy;
    int mon;
    int mday;
    int year;
    unsigned i; 

    if (NULL == ddmmyy) {
        return 1;
    }
    for (i = 0; i < 6; i++) {
        if (0 == isdigit((int)ddmmyy[i])) {
            dprintf("merge_ddmmyy(%s), malformed date\n",  ddmmyy);
            return 2;
        }
    }
    if ('\0' != ddmmyy[6]) {
        /* missing NUL */
        dprintf("merge_ddmmyy(%s), malformed date\n",  ddmmyy);
        return 3;
    }

    yy = DD(ddmmyy + 4);
    mon = DD(ddmmyy + 2);
    mday = DD(ddmmyy);

	year = (session->context->century + yy);

    if (2080 <= year) {
        year -= 100;
    }

    if ( (1 > mon ) || (12 < mon ) ) {
        return 4;
    } else if ( (1 > mday ) || (31 < mday ) ) {
        return 5;
    } else {
        session->nmea.date.tm_year = year - 1900;
        session->nmea.date.tm_mon = mon - 1;
        session->nmea.date.tm_mday = mday;
    }
	
#if __PARSER_DBG__
    dprintf("merge_ddmmyy(%s) %d %d %d\n",
             ddmmyy,
             session->nmea.date.tm_mon,
             session->nmea.date.tm_mday,
             session->nmea.date.tm_year);
#endif
			 
    return 0;
}

#if __PARSER_DBG__
static int decode_hhmmss(struct tm *date, long *nsec, char *hhmmss,
                         struct gps_device_t *session)
{
    int old_hour = date->tm_hour;
    int i, sublen;

    if (NULL == hhmmss) {
        return 1;
    }
    for (i = 0; i < 6; i++) {
        if (0 == isdigit((int)hhmmss[i])) {
            dprintf("decode_hhmmss(%s), malformed time\n",  hhmmss);
            return 2;
        }
    }
    date->tm_hour = DD(hhmmss);
    if (date->tm_hour < old_hour)  /* midnight wrap */
        date->tm_mday++;
    date->tm_min = DD(hhmmss + 2);
    date->tm_sec = DD(hhmmss + 4);

    if ('.' == hhmmss[6] &&
        0 != isdigit((int)hhmmss[7])) {
        i = atoi(hhmmss + 7);
        sublen = strlen(hhmmss + 7);
        *nsec = (long)i * (long)pow(10.0, 9 - sublen);
    } else {
        *nsec = 0;
    }

    return 0;
}
#endif

static int merge_hhmmss(char *hhmmss, struct gps_device_t *session)
{
    int old_hour = session->nmea.date.tm_hour;
    int i, sublen;

    if (NULL == hhmmss) {
        return 1;
    }
    for (i = 0; i < 6; i++) {
        if (0 == isdigit((int)hhmmss[i])) {
            dprintf("merge_hhmmss(%s), malformed time\n",  hhmmss);
            return 2;
        }
    }
    session->nmea.date.tm_hour = DD(hhmmss);
    if (session->nmea.date.tm_hour < old_hour)
        session->nmea.date.tm_mday++;
    session->nmea.date.tm_min = DD(hhmmss + 2);
    session->nmea.date.tm_sec = DD(hhmmss + 4);

    session->nmea.subseconds.tv_sec = 0;
    if ('.' == hhmmss[6] &&
        0 != isdigit((int)hhmmss[7])) {
        i = atoi(hhmmss + 7);
        sublen = strlen(hhmmss + 7);
        session->nmea.subseconds.tv_nsec = (long)i *
                                           (long)pow(10.0, 9 - sublen);
    } else {
        session->nmea.subseconds.tv_nsec = 0;
    }

    return 0;
}

static void register_fractional_time(const char *tag, const char *fld,
                                     struct gps_device_t *session)
{
    if (fld[0] != '\0') {
        session->nmea.last_frac_time = session->nmea.this_frac_time;
        DTOTS(&session->nmea.this_frac_time, safe_atof(fld));
        session->nmea.latch_frac_time = true;
    }
}

//########################### END OF GPS_TIME#####################################################
//--------------------------- GPS NMEA UTILS -----------------------------------------------------
static void gps_clear_fix(struct gps_fix_t *fixp)
{
    memset(fixp, 0, sizeof(struct gps_fix_t));
    
    fixp->latitude = NAN;
    fixp->longitude = NAN;
    fixp->mode = MODE_NOT_SEEN;
    fixp->speed = NAN;
    fixp->track = NAN;
}

static void gps_zero_satellites(struct gps_data_t *out)
{
    int sat;

    memset(out->skyview, '\0', sizeof(out->skyview));
    out->satellites_visible = 0;
    
    for (sat = 0; sat < MAXCHANNELS; sat++ ) {
        out->skyview[sat].azimuth = NAN;
        out->skyview[sat].elevation = NAN;
        out->skyview[sat].ss = NAN;
        out->skyview[sat].freqid = -1;
    }
}

static void gps_merge_fix(struct gps_fix_t *to, gps_mask_t transfer, struct gps_fix_t *from)
{
    if ((NULL == to) || (NULL == from))
		return;
    
	if ((transfer & TIME_SET) != 0)
		to->time = from->time;
    
	if ((transfer & LATLON_SET) != 0) {
		to->latitude = from->latitude;
		to->longitude = from->longitude;
    }
    
	if ((transfer & MODE_SET) != 0)
		to->mode = from->mode;
    
    if ((transfer & TRACK_SET) != 0)
        to->track = from->track;
    
    if ((transfer & SPEED_SET) != 0)
		to->speed = from->speed;
}

//-------------------------------- GPS Packet Function ---------------------------------------------
static void gps_packet_reset(struct gps_lexer_t *lexer)
{
    lexer->type = BAD_PACKET;
    lexer->state = GROUND_STATE;
    lexer->inbuflen = 0;
    lexer->inbufptr = lexer->inbuffer;
	
	/* clear in/out buffer */
	memset(lexer->inbuffer, 0, sizeof(lexer->inbuffer));
	memset(lexer->outbuffer, 0, sizeof(lexer->outbuffer));
}

static void gps_lexer_init(struct gps_lexer_t *lexer)
{
    lexer->char_counter = 0;
    gps_packet_reset(lexer);
}

static void packet_accept(struct gps_lexer_t *lexer, int packet_type)
{
    size_t packetlen = lexer->inbufptr - lexer->inbuffer;

    if (packetlen < sizeof(lexer->outbuffer)) {
		memcpy(lexer->outbuffer, lexer->inbuffer, packetlen);
		lexer->outbuflen = packetlen;
		lexer->outbuffer[packetlen] = '\0';
		lexer->type = packet_type;
    } else {
		dprintf("Rejected too long packet type %d len %zu\n",
			packet_type, packetlen);
    }
}

static void packet_discard(struct gps_lexer_t *lexer)
{
    size_t discard = lexer->inbufptr - lexer->inbuffer;
    size_t remaining = lexer->inbuflen - discard;
    lexer->inbufptr = memmove(lexer->inbuffer, lexer->inbufptr, remaining);
    lexer->inbuflen = remaining;
}

static bool str_starts_with(const char *str, const char *prefix)
{
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

static bool character_pushback(struct gps_lexer_t *lexer, unsigned int newstate)
{
    --lexer->inbufptr;
    --lexer->char_counter;
    lexer->state = newstate;
    return false;
}

static void character_discard(struct gps_lexer_t *lexer)
{
    memmove(lexer->inbuffer, lexer->inbuffer + 1, (size_t)-- lexer->inbuflen);
    lexer->inbufptr = lexer->inbuffer;
}

static bool nextstate(struct gps_lexer_t *lexer, unsigned char c)
{
    static int n = 0;
	
    n++;
    switch (lexer->state) {
    case GROUND_STATE:
		n = 0;
		if (c == '#') {
			lexer->state = COMMENT_BODY;
			break;
		}
		if (c == '$') {
			lexer->state = NMEA_DOLLAR;
			break;
		}
		if (c == '!') {
			lexer->state = NMEA_BANG;
			break;
		}
		break;
    case COMMENT_BODY:
		if (c == '\n')
			lexer->state = COMMENT_RECOGNIZED;
		else if (!isprint(c))
			return character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_DOLLAR:
		if (c == 'G')
			lexer->state = NMEA_PUB_LEAD;
		else if (c == 'P')	/* vendor sentence */
			lexer->state = NMEA_VENDOR_LEAD;
		else {
			(void) character_pushback(lexer, GROUND_STATE);
		}
		break;
    case NMEA_PUB_LEAD:
		/*
		* $GP == GPS, $GL = GLONASS only, $GN = mixed GPS and GLONASS,
		* according to NMEA (IEIC 61162-1) DRAFT 02/06/2009.
		* We have a log from China with a Beidou device using $GB
		* rather than $BD.
		*/
		if (c == 'B' || c == 'P' || c == 'N' || c == 'L' || c == 'A')
			lexer->state = NMEA_LEADER_END;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
	case NMEA_VENDOR_LEAD:
		if (c == 'A')
			lexer->state = NMEA_PASHR_A;
		else if (isalpha(c))
			lexer->state = NMEA_LEADER_END;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_PASHR_A:
		if (c == 'S')
			lexer->state = NMEA_PASHR_S;
		else if (isalpha(c))
			lexer->state = NMEA_LEADER_END;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_PASHR_S:
		if (c == 'H')
			lexer->state = NMEA_PASHR_H;
		else if (isalpha(c))
			lexer->state = NMEA_LEADER_END;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_PASHR_H:
		if (c == 'R')
			lexer->state = NMEA_BINARY_BODY;
		else if (isalpha(c))
			lexer->state = NMEA_LEADER_END;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_BINARY_BODY:
		if (c == '\r')
			lexer->state = NMEA_BINARY_CR;
		break;
    case NMEA_BINARY_CR:
		if (c == '\n')
			lexer->state = NMEA_BINARY_NL;
		else
			lexer->state = NMEA_BINARY_BODY;
		break;
    case NMEA_BINARY_NL:
		if (c == '$')
			(void) character_pushback(lexer, NMEA_RECOGNIZED);
		else
			lexer->state = NMEA_BINARY_BODY;
		break;
    case NMEA_BANG:
		return character_pushback(lexer, GROUND_STATE);
    case NMEA_LEADER_END:
		if (c == '\r')
			lexer->state = NMEA_CR;
		else if (c == '\n')
			/* not strictly correct, but helps for interpreting logfiles */
			lexer->state = NMEA_RECOGNIZED;
		else if (c == '$') {
			(void) character_pushback(lexer, GROUND_STATE);
		} else if (!isprint(c))
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_CR:
		if (c == '\n')
			lexer->state = NMEA_RECOGNIZED;
		/*
		* There's a GPS called a Jackson Labs Firefly-1a that emits \r\r\n
		* at the end of each sentence.  Don't be confused by this.
		*/
		else if (c == '\r')
			lexer->state = NMEA_CR;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_RECOGNIZED:
		if (c == '#')
			lexer->state = COMMENT_BODY;
		else if (c == '$')
			lexer->state = NMEA_DOLLAR;
		else if (c == '!')
			lexer->state = NMEA_BANG;
		else
			return character_pushback(lexer, GROUND_STATE);
		break;
    }

    return true;	/* no pushback */
}

static void gps_packet_parse(struct gps_lexer_t *lexer)
{
    lexer->outbuflen = 0;
    
	while (packet_buffered_input(lexer) > 0) 
	{
		unsigned char c = *lexer->inbufptr++;
		
		if (!nextstate(lexer, c))
			continue;
		
		lexer->char_counter++;

		if (lexer->state == GROUND_STATE) {
			character_discard(lexer);
		} else if (lexer->state == COMMENT_RECOGNIZED) {
			packet_accept(lexer, COMMENT_PACKET);
			packet_discard(lexer);
			lexer->state = GROUND_STATE;
			break;
		}
		else if (lexer->state == NMEA_RECOGNIZED) 
		{
			if (!str_starts_with((const char *)lexer->inbuffer, "$PASHR,"))
			{
				bool checksum_ok = true;
				char csum[3] = { '0', '0', '0' };
				char *end;
				/*
				* Back up past any whitespace.  Need to do this because
				* at least one GPS (the Firefly 1a) emits \r\r\n
				*/
				for (end = (char *)lexer->inbufptr - 1; isspace((unsigned char) *end); end--)
					continue;
				while (strchr("0123456789ABCDEF", *end))
					--end;
				if (*end == '*') {
					unsigned int n, crc = 0;
					for (n = 1; (char *)lexer->inbuffer + n < end; n++)
					crc ^= lexer->inbuffer[n];
					(void)snprintf(csum, sizeof(csum), "%02X", crc);
					checksum_ok = (csum[0] == toupper((unsigned char) end[1])
						&& csum[1] == toupper((unsigned char) end[2]));
				}
				if (!checksum_ok) {
					//dprintf("bad checksum in NMEA packet; expected %s.\n", csum);
					packet_accept(lexer, BAD_PACKET);
					lexer->state = GROUND_STATE;
					packet_discard(lexer);
					break;    /* exit case */
				}
	    	}
			/* checksum passed or not present */
			packet_accept(lexer, NMEA_PACKET);
			packet_discard(lexer);
			break;
		}
    }	/* while */
}

//--------------------------------- End of GPS Packet Function -------------------------------------
/* Recommend Minimum Course Specific GPS/TRANSIT Data */
static gps_mask_t processRMC(int count, char *field[], struct gps_device_t *session)
{
	/*
     * RMC,225446.33,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E,A*68
     * 1     225446.33    Time of fix 22:54:46 UTC
     * 2     A            Status of Fix:
     *                     A = Autonomous, valid;
     *                     D = Differential, valid;
     *                     V = invalid
     * 3,4   4916.45,N    Latitude 49 deg. 16.45 min North
     * 5,6   12311.12,W   Longitude 123 deg. 11.12 min West
     * 7     000.5        Speed over ground, Knots
     * 8     054.7        Course Made Good, True north
     * 9     181194       Date of fix ddmmyy.  18 November 1994
     * 10,11 020.3,E      Magnetic variation 20.3 deg East
     * 12    A            FAA mode indicator (NMEA 2.3 and later)
     *                     see faa_mode() for possible mode values
     * 13    V            Nav Status (NMEA 4.1 and later)
     *                     A=autonomous,
     *                     D=differential,
     *                     E=Estimated,
     *                     M=Manual input mode
     *                     N=not valid,
     *                     S=Simulator,
     *                     V = Valid
     * *68        mandatory nmea_checksum
     *
     */
    gps_mask_t mask = ONLINE_SET;
    char status = field[2][0];
    int newstatus;

    switch (status) {
    default:
    case 'V':
        /* Invalid */
        session->gpsdata.status = STATUS_NO_FIX;
        session->newdata.mode = MODE_NO_FIX;
        mask |= STATUS_SET | MODE_SET;
        break;
    case 'D':
        /* Differential Fix */
        /* FALLTHROUGH */
    case 'A':
        /* Valid Fix */
        /*
         * The MTK3301, Royaltek RGM-3800, and possibly other
         * devices deliver bogus time values when the navigation
         * warning bit is set.
         */
        if ('\0' != field[1][0] &&
            9 < count &&
            '\0' !=  field[9][0]) {
            if (0 == merge_hhmmss(field[1], session) &&
                0 == merge_ddmmyy(field[9], session)) {
                /* got a good data/time */
                mask |= TIME_SET;
                register_fractional_time(field[0], field[1], session);
            }
        }
        /* else, no point to the time only case, no regressions with that */

        if (0 == do_lat_lon(&field[3], &session->newdata)) {
            newstatus = STATUS_FIX;
            mask |= LATLON_SET;
            if (MODE_2D >= session->gpsdata.fix.mode) {
                /* we have at least a 2D fix */
                /* might cause blinking */
                session->newdata.mode = MODE_2D;
                mask |= MODE_SET;
            }
        } else {
            newstatus = STATUS_NO_FIX;
            session->newdata.mode = MODE_NO_FIX;
            mask |= MODE_SET;
        }
        if ('\0' != field[7][0]) {
            session->newdata.speed = safe_atof(field[7]) * KNOTS_TO_MPS;
            mask |= SPEED_SET;
        }
        if ('\0' != field[8][0]) {
            session->newdata.track = safe_atof(field[8]);
            mask |= TRACK_SET;
        }

        if (count >= 12) {
            newstatus = faa_mode(field[12][0]);
        }

        if (3 < session->gpsdata.satellites_used) {
            /* 4 sats used means 3D */
            session->newdata.mode = MODE_3D;
            mask |= MODE_SET;
        } 
		
		/* 0 -> no fix, 1-> fix, 2-> differendial fix (not used) */
        session->gpsdata.status = (newstatus ? 1 : 0);
    }

    return mask;
}

static gps_mask_t processGSA(int count, char *field[],
                             struct gps_device_t *session)
{
#define GSA_TALKER      field[0][1]
    gps_mask_t mask = ONLINE_SET;
    char last_last_gsa_talker = session->nmea.last_gsa_talker;
    int nmea_gnssid = 0;

    if (18 > count) {
        dprintf("xxGSA: malformed, setting ONLINE_SET only.\n");
        mask = ONLINE_SET;
    } else if (session->nmea.latch_mode) {
        /* last GGA had a non-advancing timestamp; don't trust this GSA */
        mask = ONLINE_SET;
        dprintf("xxGSA: non-advancing timestamp\n");
    } else {
        int i;
        session->newdata.mode = atoi(field[2]);
        /*
         * The first arm of this conditional ignores dead-reckoning
         * fixes from an Antaris chipset. which returns E in field 2
         * for a dead-reckoning estimate.  Fix by Andreas Stricker.
         */
        if ('E' == field[2][0])
            mask = ONLINE_SET;
        else
            mask = MODE_SET;

        //dprintf("xxGSA sets mode %d\n", session->newdata.mode);

        if ( '\0' == session->nmea.last_gsa_talker
          || (GSA_TALKER == session->nmea.last_gsa_talker
              && 'N' != GSA_TALKER) ) {
            session->gpsdata.satellites_used = 0;
            memset(session->nmea.sats_used, 0, sizeof(session->nmea.sats_used));
           // dprintf("xxGSA: clear sats_used\n");
        }
        
		session->nmea.last_gsa_talker = GSA_TALKER;
        if ((session->nmea.last_gsa_talker == 'B') ||
            (session->nmea.last_gsa_talker == 'D') ||
            (session->nmea.last_gsa_talker == 'Q'))
            session->nmea.seen_bdgsa = true;
        else if (session->nmea.last_gsa_talker == 'L')
            session->nmea.seen_glgsa = true;
        else if (session->nmea.last_gsa_talker == 'N')
            session->nmea.seen_gngsa = true;
        else if (session->nmea.last_gsa_talker == 'A')
            session->nmea.seen_gagsa = true;

        for (i = 0; i < count - 6; i++) {
            int prn;
            int nmea_satnum;
			unsigned char ubx_gnssid;   /* UNUSED */
            unsigned char ubx_svid;     /* UNUSED */

            /* skip empty fields, otherwise empty becomes prn=200 */
            nmea_satnum = atoi(field[i + 3]);
            if (1 > nmea_satnum) {
                continue;
            }
            prn = nmeaid_to_prn(field[0], nmea_satnum, nmea_gnssid,
                                &ubx_gnssid, &ubx_svid);
            if (prn > 0) {
                if (MAXCHANNELS <= session->gpsdata.satellites_used) {
                    break;
                }
                session->nmea.sats_used[session->gpsdata.satellites_used++] =
                    (unsigned short)prn;
            }
        }
        mask |= USED_IS;
       // dprintf("xxGSA: mode=%d used=%d\n", session->newdata.mode, session->gpsdata.satellites_used);
    }
	
    if ((session->nmea.seen_glgsa || session->nmea.seen_bdgsa ||
         session->nmea.seen_gagsa) && GSA_TALKER == 'P') {
        mask = ONLINE_SET;
    } else if ( 'N' != last_last_gsa_talker && 'N' == GSA_TALKER) {
        mask =  ONLINE_SET | MODE_SET;
    }

    return mask;
#undef GSA_TALKER
}

/* xxGSV -  GPS Satellites in View */
static gps_mask_t processGSV(int count, char *field[],
                             struct gps_device_t *session)
{
#define GSV_TALKER      field[0][1]
	/*
     * GSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75
     *  1) 2           Number of sentences for full data
     *  2) 1           Sentence 1 of 2
     *  3) 08          Total number of satellites in view
     *  4) 01          Satellite PRN number
     *  5) 40          Elevation, degrees
     *  6) 083         Azimuth, degrees
     *  7) 46          Signal-to-noise ratio in decibels
     * <repeat for up to 4 satellites per sentence>
     *   )             NMEA 4.1 signalId
     *   )             checksum
     *
     */
    int n, fldnum;
    int nmea_gnssid = 0;

    if (count <= 3) {
        dprintf("malformed xxGSV - fieldcount %d <= 3\n",
                 count);
        gps_zero_satellites(&session->gpsdata);
        return ONLINE_SET;
    }
	
    //dprintf( "x%cGSV: part %s of %s, last_gsv_talker '%#x' \n",
    //         GSV_TALKER, field[2], field[1], session->nmea.last_gsv_talker);

    switch (count % 4) {
    case 0:
        /* normal, pre-NMEA 4.10 */
        break;
    default:
        /* bad count */
        dprintf("malformed GPGSV - fieldcount %d %% 4 != 0\n", count);
        gps_zero_satellites(&session->gpsdata);
        return ONLINE_SET;
    }

    session->nmea.await = atoi(field[1]);
    if ((session->nmea.part = atoi(field[2])) < 1) {
        dprintf("malformed GPGSV - bad part\n");
        gps_zero_satellites(&session->gpsdata);
        return ONLINE_SET;
    }

    if (session->nmea.part == 1) 
	{
        if (session->nmea.last_gsv_talker == '\0' ) {
            //dprintf("x%cGSV: new part %d, last_gsv_talker '%#x', zeroing\n",
            //         GSV_TALKER, session->nmea.part, session->nmea.last_gsv_talker);
            gps_zero_satellites(&session->gpsdata);
        }
        session->nmea.last_gsv_talker = GSV_TALKER;
        switch (GSV_TALKER) {
        case 'A':
            session->nmea.seen_gagsv = true;
            break;
        case 'B':
            /* FALLTHROUGH */
        case 'D':
            /* FALLTHROUGH */
        case 'Q':
            /* Quectel EC25 & EC21 use PQGSA for BeiDou */
            session->nmea.seen_bdgsv = true;
            break;
        case 'L':
            session->nmea.seen_glgsv = true;
            break;
        case 'P':
            session->nmea.seen_gpgsv = true;
            break;
        case 'Z':
            session->nmea.seen_qzss = true;
            break;
        default:
            /* uh, what? */
            break;
        }
    }

    for (fldnum = 4; fldnum < count / 4 * 4;) {
        struct satellite_t *sp;
        int nmea_svid;

        if (session->gpsdata.satellites_visible >= MAXCHANNELS) {
            dprintf("xxGSV: internal error - too many satellites [%d]!\n",
                     session->gpsdata.satellites_visible);
            gps_zero_satellites(&session->gpsdata);
            break;
        }
        sp = &session->gpsdata.skyview[session->gpsdata.satellites_visible];
        nmea_svid = atoi(field[fldnum++]);
        if (0 == nmea_svid) {
            /* skip bogus fields */
            continue;
        }
        /* FIXME: this ignores possible NMEA 4.1 Signal ID hint */
        sp->PRN = (short)nmeaid_to_prn(field[0], nmea_svid, nmea_gnssid,
                                       &sp->gnssid, &sp->svid);

        sp->elevation = (double)atoi(field[fldnum++]);
        sp->azimuth = (double)atoi(field[fldnum++]);
        sp->ss = (double)atoi(field[fldnum++]);
        sp->used = false;

        /* sadly NMEA 4.1 does not tell us which sigid (L1, L2) is
         * used.  So if the ss is zero, do not mark used */
        if (0 < sp->PRN && 0 < sp->ss) {
            for (n = 0; n < MAXCHANNELS; n++)
                if (session->nmea.sats_used[n] == (unsigned short)sp->PRN) {
                    sp->used = true;
                    break;
                }
        }
        session->gpsdata.satellites_visible++;
    }

    if (!(session->nmea.seen_glgsv || session->nmea.seen_bdgsv
        || session->nmea.seen_qzss || session->nmea.seen_gagsv))
        if (session->nmea.part == session->nmea.await
                && atoi(field[3]) != session->gpsdata.satellites_visible)
            dprintf("GPGSV field 3 value of %d != actual count %d\n",
                     atoi(field[3]), session->gpsdata.satellites_visible);

    /* not valid data until we've seen a complete set of parts */
    if (session->nmea.part < session->nmea.await) {
		/* GSV? partial? ???. ??? */
       // dprintf("xxGSV: Partial satellite data (%d of %d).\n", session->nmea.part, session->nmea.await);
        return ONLINE_SET;
    }
    
    for (n = 0; n < session->gpsdata.satellites_visible; n++)
        if (session->gpsdata.skyview[n].azimuth != 0)
            goto sane;
    dprintf("xxGSV: Satellite data no good (%d of %d).\n", 
				session->nmea.part, session->nmea.await);
	gps_zero_satellites(&session->gpsdata);
    return ONLINE_SET;

sane:
    session->gpsdata.skyview_time.tv_sec = 0;
    session->gpsdata.skyview_time.tv_nsec = 0;

    /* assumes GLGSV or BDGSV group, if present, is emitted after the GPGSV */
    if ((session->nmea.seen_glgsv || session->nmea.seen_bdgsv
         || session->nmea.seen_qzss  || session->nmea.seen_gagsv)
        && GSV_TALKER == 'P')
        return ONLINE_SET;

    return SATELLITE_SET;
#undef GSV_TALKER
}

static gps_mask_t nmea_parse(char *sentence, struct gps_device_t * session)
{
    typedef gps_mask_t(*nmea_decoder) (int count, char *f[],
                                       struct gps_device_t * session);
    static struct
    {
        char *name;
        int nf;                 /* minimum number of fields required to parse */
        bool cycle_continue;    /* cycle continuer? */
        nmea_decoder decoder;
    } nmea_phrase[] = {
//        {"GGA", 13, false, processGGA},
//        {"GLL", 7,  false, processGLL},
//        {"GNS", 13, false, processGNS},
        {"GSA", 18, false, processGSA},
//        {"GST", 8,  false, processGST},
        {"GSV", 0,  false, processGSV},
        {"RMC", 8,  false, processRMC},
//        {"VTG", 5,  false, processVTG},
    };

    int count;
    gps_mask_t mask = 0;
    unsigned int i, thistag, lasttag;
    char *p, *e;
    volatile char *t;
	uint64_t lasttag_mask = 0;
    uint64_t thistag_mask = 0;
//	char ts_buf1[TIMESPEC_LEN];
 //   char ts_buf2[TIMESPEC_LEN];

    if (strlen(sentence) > NMEA_MAX) {
        dprintf("Overlong packet of %zd chars rejected.\n", strlen(sentence));
        return ONLINE_SET;
    }

    strlcpy((char *)session->nmea.fieldcopy, sentence, sizeof(session->nmea.fieldcopy) - 1);
    for (p = (char *)session->nmea.fieldcopy; (*p != '*') && (*p >= ' ');)
        ++p;
    if (*p == '*')
        *p++ = ',';  
	*p = '\0';
    e = p;

    count = 0;
    t = p;    
    p = (char *)session->nmea.fieldcopy + 1;
    while ((p != NULL) && (p <= t)) {
        session->nmea.field[count] = p; /* we have a field. record it */
        if ((p = strchr(p, ',')) != NULL) {  /* search for the next delimiter */
            *p = '\0';          /* replace it with a NUL */
            count++;            /* bump the counters and continue */
            p++;
        }
    }

    for (i = (unsigned int)count; i < (unsigned)(sizeof(session->nmea.field) / sizeof(session->nmea.field[0])); i++)
        session->nmea.field[i] = e;

    session->nmea.latch_frac_time = false;

    /* dispatch on field zero, the sentence tag */
    for (thistag = i = 0; i < (unsigned)(sizeof(nmea_phrase) / sizeof(nmea_phrase[0])); ++i) {
        char *s = session->nmea.field[0];
        if (strlen(nmea_phrase[i].name) == 3) {
            s += 2; 
        }
		
        if (strcmp(nmea_phrase[i].name, s) == 0) {
            if (NULL == nmea_phrase[i].decoder) {
                /* no decoder for this sentence */
                mask = ONLINE_SET;
                dprintf("No decoder for sentence %s\n", session->nmea.field[0]);
                break;
            }
			
            if (count < nmea_phrase[i].nf) {
                /* sentence to short */
                mask = ONLINE_SET;
                dprintf("Sentence %s too short\n", session->nmea.field[0]);
                break;
            }
            mask = (nmea_phrase[i].decoder)(count, session->nmea.field, session);
            if (nmea_phrase[i].cycle_continue)
                session->nmea.cycle_continue = true;
            
			thistag = i + 1;
            break;
        }
    }

    if (!str_starts_with(session->nmea.field[0] + 2, "GSV"))
        session->nmea.last_gsv_talker = '\0';
    
	if (!str_starts_with(session->nmea.field[0] + 2, "GSA"))
        session->nmea.last_gsa_talker = '\0';

    if ((mask & TIME_SET) != 0) {
        session->newdata.time = __utc_resolve(session);
#if 0
        dprintf("%s time is = %d-%02d-%02dT%02d:%02d:%02d.%03ldZ\n",
                 session->nmea.field[0],
                 1900 + session->nmea.date.tm_year,
                 session->nmea.date.tm_mon + 1,
                 session->nmea.date.tm_mday,
                 session->nmea.date.tm_hour,
                 session->nmea.date.tm_min,
                 session->nmea.date.tm_sec,
                 session->nmea.subseconds.tv_nsec / 1000000L);
#endif				 
        mask |= NTPTIME_IS;
    }

#if 0
    dprintf("%s time %s last %s latch %d cont %d enders %#llx\n",
             session->nmea.field[0],
             timespec_str(&session->nmea.this_frac_time, ts_buf1,
                          sizeof(ts_buf1)),
             timespec_str(&session->nmea.last_frac_time, ts_buf2,
                          sizeof(ts_buf2)),
             session->nmea.latch_frac_time,
             session->nmea.cycle_continue,
             (unsigned long long)session->nmea.cycle_enders);
#endif    
	lasttag = session->nmea.lasttag;
    if (0 < session->nmea.lasttag) {
        lasttag_mask = (uint64_t)1 << lasttag;
    }
    
	if (0 < thistag) {
        thistag_mask = (uint64_t)1 << thistag;
    }
    
	if (session->nmea.latch_frac_time) {
        timespec_t ts_delta;
        TS_SUB(&ts_delta, &session->nmea.this_frac_time, &session->nmea.last_frac_time);
        if (0.01 < fabs(TSTONS(&ts_delta))) {
            /* time changed */
            mask |= CLEAR_IS;
            //dprintf("%s starts a reporting cycle. lasttag %d\n", session->nmea.field[0], lasttag);
            if (0 < lasttag && 0 == (session->nmea.cycle_enders & lasttag_mask) && !session->nmea.cycle_continue) 
			{
                session->nmea.cycle_enders |= lasttag_mask;
                /* (long long) cast for 32/64 bit compat */
                dprintf("tagged %s as a cycle ender. %#llx\n", nmea_phrase[lasttag - 1].name, (unsigned long long)lasttag_mask);
            }
        }
    } 

    if ((session->nmea.latch_frac_time || session->nmea.cycle_continue)
        && (session->nmea.cycle_enders & thistag_mask)!=0) {
       // dprintf("%s ends a reporting cycle.\n", session->nmea.field[0]);
        mask |= REPORT_IS;
    }
    
	if (session->nmea.latch_frac_time)
        session->nmea.lasttag = thistag;

    return mask;
}

ssize_t nmea_packet_get(int fd, struct gps_lexer_t *lexer)
{
    ssize_t recvd;

    errno = 0;
    recvd = read(fd, lexer->inbuffer + lexer->inbuflen,
		 sizeof(lexer->inbuffer) - (lexer->inbuflen));
    if (recvd == -1) {
		if ((errno == EAGAIN) || (errno == EINTR)) {
			recvd = 0;
		} else {
			eprintf("errno: %s\n", strerror(errno));
			return -1;
		}
    }  else {
		#if 0
		{
			char scratchbuf[MAX_PACKET_LENGTH*4+1];
			/* remaining packet buffer dump */
			dprintf("Read %zd chars to buffer offset %zd (total %zd): %s\n",
				recvd, lexer->inbuflen, lexer->inbuflen + recvd,
				__packetdump(scratchbuf, sizeof(scratchbuf),
						(char *)lexer->inbufptr, (size_t) recvd));
		}
		#endif
		lexer->inbuflen += recvd;
    }
	
    if (recvd <= 0 && packet_buffered_input(lexer) <= 0)
		return recvd;

    gps_packet_parse(lexer);

    /* if input buffer is full, discard */
    if (sizeof(lexer->inbuffer) == (lexer->inbuflen)) {
		/* coverity[tainted_data] */
		packet_discard(lexer);
		lexer->state = GROUND_STATE;
    }

    if (lexer->outbuflen > 0)
		return (ssize_t) lexer->outbuflen;
    else
		return recvd;
}

gps_mask_t nmea_parse_input(struct gps_device_t *session)
{
    if (session->lexer.type == BAD_PACKET)
		return 0;
	else if (session->lexer.type == NMEA_PACKET) {
		gps_mask_t st = 0;
		char *sentence = (char *)session->lexer.outbuffer;
		
		if ((st=nmea_parse(sentence, session)) == 0) {
			//dprintf("unknown sentence: %s\n", sentence);
		}
		return st;
    } else {
		dprintf("packet type %d fell through (should never happen):\n",	
					session->lexer.type);
		return 0;
    }
}

//--------------------------------------------------------------------------------------------------
void app_nmea_parse_init(void)
{
	struct gps_context_t *context = &app_cfg->t_context;
	time_t starttime;
	
	starttime = time(NULL);
    context->leap_seconds = BUILD_LEAPSECONDS;
    context->century = BUILD_CENTURY; /* 2000년 고정 */
 	context->start_time = starttime;
	session->context = context;
	
	gps_clear_fix(&session->gpsdata.fix);
    gps_clear_fix(&session->newdata);
	session->gpsdata.set = 0;
	
	gps_zero_satellites(&session->gpsdata);
    gps_lexer_init(&session->lexer);
    gps_clear_fix(&session->gpsdata.fix);
    session->gpsdata.status = STATUS_NO_FIX;
}

//############################################# NMEA DATA POLL ###########################################
gps_mask_t app_nmea_parse_get_data(void)
{
    ssize_t newlen;

    gps_clear_fix(&session->newdata);

    /* can we get a full packet from the device? */
	newlen = nmea_packet_get(app_cfg->gps_fd, &session->lexer);
	if (newlen < 0) {
		//eprintf("GPS returned error %zd\n", newlen);
		return ERROR_SET;
    } else if (newlen == 0) {
		return NODATA_IS;
    } 

    if (session->lexer.outbuflen > 0) 
	{
		gps_mask_t received = PACKET_SET;

		if (session->lexer.type != COMMENT_PACKET) {
			received |= nmea_parse_input(session);
		}
		
		session->gpsdata.set = ONLINE_SET | received;
		if ((session->gpsdata.set & CLEAR_IS) != 0) {
			gps_clear_fix(&session->gpsdata.fix);
		}
		
		gps_merge_fix(&session->gpsdata.fix, session->gpsdata.set, &session->newdata);
		
		return session->gpsdata.set;
    }
	
    return 0;
}
//#############################################################################################################
