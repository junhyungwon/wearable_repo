/*
 * File : dev_gps.c
 *
 * Copyright (C) 2013 UDWORKs
 * based on gpsd.
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
 * this implements a GPS hardware library for UBX.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <linux/serial.h>	/* For Linux-specific struct serial_struct */
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include "dev_common.h"
#include "dev_gps.h"

#define GPS_PKT_DEBUG			0
#define GPS_DATA_DEBUG			0
#define GPS_PARSE_DEBUG			0

#if GPS_PARSE_DEBUG
#define gps_dbg(fmt, arg...)  	fprintf(stderr, "%s:%d: " fmt, __FILE__, __LINE__, ## arg)
#else
#define gps_dbg(fmt, arg...)  	do { } while (0)
#endif

/* max length of NMEA sentence */
#define NMEA_MAX				91

#define GPS_EPOCH				315964800	/* 6 Jan 1981 00:00:00 UTC */
#define GPS_LEAPSECOND_NOW		16

#define GPS_DEV_RATE			B9600
#define GPS_DEV_NAME  			"/dev/ttyO1"

#define GPS_PKT_BAD      		-1
#define GPS_PKT_NMEA     		1

#define KNOTS_TO_MPS			0.51444444	/* Knots to meters per second */
#define KNOTS_TO_KPH			1.852		/* Knots to kilometers per hour */

/*
 * SiRF Measured data(188 bytes) + UBLOX SVINFO(206 bytes)
 */
#define GPS_MAX_PKT_LENGTH		516	/* 7 + 506 + 3 */
#define GPS_BUFFER_LENGTH		(GPS_MAX_PKT_LENGTH * 2)
#define DD(s)					((int)((s)[0]-'0')*10+(int)((s)[1]-'0'))

#define packet_buffered_input(a) ((a)->inbuffer + (a)->inbuflen - (a)->inbufptr)

struct gps_device_t;

typedef unsigned long gps_mask_t; /* for return status */
typedef gps_mask_t(*nmea_decoder)(int count, char *f[], struct gps_device_t *session, struct gps_data_t *newdata);

struct gps_packet_t {
	unsigned char inbuffer[GPS_BUFFER_LENGTH + 1];
	unsigned char *inbufptr;
    size_t inbuflen;

    unsigned char outbuffer[GPS_BUFFER_LENGTH + 1];
    size_t outbuflen;

	unsigned int state;
	unsigned long char_counter;	   /* count characters processed */
	unsigned long counter;  /* count valid nmea tags */

	int	type;
};

struct gps_device_t {
	struct gps_packet_t packet;
	time_t start_time;

	int leap_seconds;	/* Unix seconds to UTC (GPS-UTC offset) */
	int killzero;

	char *field[NMEA_MAX];
	unsigned char fieldcopy[NMEA_MAX + 1];
};

enum {
	GROUND_STATE = 0,

	NMEA_DOLLAR, 		/* '$' */
   	NMEA_PUB_LEAD,		/* 'G' */
   	NMEA_LEADER_END,	/* seen end char of NMEA leader, in body */
   	NMEA_CR,	   		/* seen terminating \r of NMEA packet */
   	NMEA_RECOGNIZED,	/* saw trailing \n of NMEA packet */
};

static struct gps_device_t gdevices; //# only 1 device support
static int gps_fd = -1;
static int gps_century = 0;	/* for NMEA-only devices without ZDA */

/*************************************
 * local function
 *************************************/
#if GPS_PKT_DEBUG
static char *gps_hex_dump(char *scbuf, size_t scbuflen,
					  char *binbuf, size_t binbuflen)
{
    size_t i, j = 0;
    size_t len = (size_t) ((binbuflen >
		   GPS_MAX_PKT_LENGTH) ? GPS_MAX_PKT_LENGTH : binbuflen);

    const char *ibuf = (const char *)binbuf;
    const char *hexchar = "0123456789abcdef";

    if (NULL == binbuf || 0 == binbuflen)
		return "";

    for (i = 0; i < len && i * 2 < scbuflen - 2; i++) {
		scbuf[j++] = hexchar[(ibuf[i] & 0xf0) >> 4];
		scbuf[j++] = hexchar[ibuf[i] & 0x0f];
    }

    scbuf[j] = '\0';

    return scbuf;
}

static char *gps_packet_dump(char *scbuf, size_t scbuflen,
						char *binbuf, size_t binbuflen)
{
    char *cp;
	int printable = 1;

	if (binbuf == NULL)
		return NULL;

    for (cp = binbuf; cp < binbuf + binbuflen; cp++)
		if (!isprint(*cp) && !isspace(*cp))
	    	printable = 0;

    if (printable)
		return binbuf;

	return gps_hex_dump(scbuf, scbuflen, binbuf, binbuflen);
}
#endif

static void gps_packet_reset(struct gps_packet_t *pkt)
{
    /* reset the packet parser */
	memset(pkt, 0, sizeof(*pkt));

	pkt->char_counter = 0;
	pkt->counter = 0;
    pkt->type = GPS_PKT_BAD;
    pkt->state = GROUND_STATE;
    pkt->inbuflen = 0;
    pkt->inbufptr = pkt->inbuffer;
}

/**************************************************************************
 * NMEA utils
 **************************************************************************/
static void gps_zero_satellites(struct gps_data_t *out)
{
    memset(out->PRN, 0, sizeof(out->PRN));
    memset(out->elevation, 0, sizeof(out->elevation));
    memset(out->azimuth, 0, sizeof(out->azimuth));
    memset(out->ss, 0, sizeof(out->ss));

    out->satellites_visible = 0;
}

static size_t gps_strlcpy(char *dst, const char *src, size_t siz)
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

static double gps_safe_atof(const char *string)
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

    char sign, expSign = 0;
    double fraction, dblExp, *d;

    register const char *p;
    register int c;

    int exp = 0;		/* Exponent read from "EX" field. */
    int fracExp = 0;	/* Exponent that derives from the fractional part */
    int mantSize;		/* Number of digits in mantissa. */
    int decPt;			/* Number of mantissa digits BEFORE decimal point. */
    const char *pExp;	/* Temporarily holds location of exponent in string. */

    p = string;
    while (isspace(*p)) {
		p += 1;
    }

    if (*p == '-') {
		sign = 1;
		p += 1;
    } else {
		if (*p == '+') {
	    	p += 1;
		}
		sign = 0;
    }

    decPt = -1;
    for (mantSize = 0; ; mantSize += 1) {
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
		for ( ; mantSize > 9; mantSize -= 1) {
	    	c = *p;
	    	p += 1;
	    	if (c == '.') {
				c = *p;
				p += 1;
	    	}
	    	frac1 = 10*frac1 + (c - '0');
		}
		frac2 = 0;
		for (; mantSize > 0; mantSize -= 1) {
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
	    	expSign = 1;
	    	p += 1;
		} else {
	    	if (*p == '+') {
				p += 1;
	    	}
	    	expSign = 0;
		}

		while (isdigit(*p)) {
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
		expSign = 1;
		exp = -exp;
    } else {
		expSign = 0;
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

/**************************************************************************
 * NMEA sentence handling
 **************************************************************************/
static void gps_do_lat_lon(char *field[], struct gps_data_t *newdata)
{
    double d, m;
    double lat;
	double lon;

    char str[20], *p;

    if (*(p = field[0]) != '\0') {
		gps_strlcpy(str, p, sizeof(str));
		lat = atof(str);

		m = 100.0 * modf(lat / 100.0, &d);
		lat = d + m / 60.0;
		p = field[1];

		if (*p == 'S')
	    	lat = -lat;

		newdata->nmea.latitude = lat;
    }

    if (*(p = field[2]) != '\0') {
		gps_strlcpy(str, p, sizeof(str));
		lon = atof(str);
		m = 100.0 * modf(lon / 100.0, &d);
		lon = d + m / 60.0;

		p = field[3];
		if (*p == 'W')
	    	lon = -lon;

		newdata->nmea.longitude = lon;
    }
}

static void gps_merge_date(char *ddmmyy, struct gps_data_t *newdata)
{
    int yy = DD(ddmmyy + 4);
    int mon = DD(ddmmyy + 2);
    int mday = DD(ddmmyy);
    int year;

    year = (gps_century + yy);
    if ((mon < 1 ) || (mon > 12)) {
		dev_err("merge_ddmmyy(%s), undefined month\n", ddmmyy);
    } else if ((mday < 1) || (mday > 31 )) {
		dev_err("merge_ddmmyy(%s), undefined day\n", ddmmyy);
    } else {
		gps_dbg("merge_ddmmyy(%s) sets year %d\n", ddmmyy, year);

		newdata->nmea.date.tm_year = year - 1900;
		newdata->nmea.date.tm_mon = mon - 1;
		newdata->nmea.date.tm_mday = mday;

#if GPS_DATA_DEBUG
		gps_dbg("time = %d-%02d-%02dT\n",
		    1900 + newdata->nmea.date.tm_year,
		    newdata->nmea.date.tm_mon + 1,
		    newdata->nmea.date.tm_mday);
#endif
    }
}

/* UTC time */
static void gps_merge_time(char *hhmmss, struct gps_data_t *newdata)
{
    int old_hour = newdata->nmea.date.tm_hour;

    newdata->nmea.date.tm_hour = DD(hhmmss);

    if (newdata->nmea.date.tm_hour < old_hour)	/* midnight wrap */
		newdata->nmea.date.tm_mday++;

    newdata->nmea.date.tm_min = DD(hhmmss + 2);
    newdata->nmea.date.tm_sec = DD(hhmmss + 4);
    newdata->nmea.subseconds = gps_safe_atof(hhmmss + 4) - newdata->nmea.date.tm_sec;

#if GPS_DATA_DEBUG
	gps_dbg("time = %02d:%02d:%02.2fZ\n",
		    newdata->nmea.date.tm_hour,
		    newdata->nmea.date.tm_min,
		    newdata->nmea.date.tm_sec + newdata->nmea.subseconds);
#endif
}

static void gps_fractional_time(const char *fld, struct gps_data_t *newdata)
{
    if (fld[0] != '\0') {
		newdata->nmea.last_frac_time = newdata->nmea.this_frac_time;
		newdata->nmea.this_frac_time = gps_safe_atof(fld);
		newdata->nmea.latch_frac_time = 1;
		gps_dbg("registers fractional time %.2f\n", newdata->nmea.this_frac_time);
    }
}

static gps_mask_t gps_process_RMC(int count, char *field[],
					struct gps_device_t *session, struct gps_data_t *newdata)
{
    /*
     * RMC,225446.33,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E,A*68
     * 1     225446.33    Time of fix 22:54:46 UTC
     * 2     A          Status of Fix: A = Autonomous, valid;
     * D = Differential, valid; V = invalid
     * 3,4   4916.45,N    Latitude 49 deg. 16.45 min North
     * 5,6   12311.12,W   Longitude 123 deg. 11.12 min West
     * 7     000.5      Speed over ground, Knots
     * 8     054.7      Course Made Good, True north
     * 9     181194       Date of fix  18 November 1994
     * 10,11 020.3,E      Magnetic variation 20.3 deg East
     * 12    A      FAA mode indicator (NMEA 2.3 and later)
     * A=autonomous, D=differential, E=Estimated,
     * N=not valid, S=Simulator, M=Manual input mode
     * *68        mandatory nmea_checksum
     *
     * * SiRF chipsets don't return either Mode Indicator or magnetic variation.
     */
    gps_mask_t mask = 0;

    if (strcmp(field[2], "V") == 0) {
    	if (newdata->rmc_status != STATUS_NO_FIX) {
    		newdata->rmc_status = STATUS_NO_FIX;
    		mask |= G_STATUS_SET;
    	}
    	if (newdata->mode >= MODE_2D) {
	    	newdata->mode = MODE_NO_FIX;
	    	mask |= G_MODE_SET;
		}
    	mask |= G_ONLINE_SET;
    } else if (strcmp(field[2], "A") == 0) {
		if (count > 9 && field[1][0] != '\0' && field[9][0] != '\0') {
		    gps_merge_time(field[1], newdata);
		    gps_merge_date(field[9], newdata);
			mask |= G_TIME_SET;

		    gps_fractional_time(field[1], newdata);
		}

		gps_do_lat_lon(&field[3], newdata);
		mask |= G_LATLON_SET;

		newdata->nmea.speed = gps_safe_atof(field[7]) * KNOTS_TO_KPH;
		newdata->nmea.track = gps_safe_atof(field[8]);
		mask |= (G_TRACK_SET | G_SPEED_SET);

	    newdata->rmc_status = STATUS_FIX;
	    if (newdata->mode < MODE_2D) {
	    	newdata->mode = MODE_2D;
	    	mask |= G_MODE_SET;
		}
	    mask |= G_STATUS_SET;
    }

#if GPS_DATA_DEBUG
    gps_dbg("RMC: ddmmyy=%s hhmmss=%s lat=%.2f lon=%.2f "
			"speed=%.2f(Km/h) track=%.2f mode=%d status=%d\n",
			field[9], field[1],
			newdata->nmea.latitude,
			newdata->nmea.longitude,
			newdata->nmea.speed,
			newdata->nmea.track,
			newdata->mode,
			newdata->rmc_status);
#endif

    return mask;
}

/* GPS DOP and Active Satellites */
static gps_mask_t gps_process_GSA(int count, char *field[],
					struct gps_device_t *session, struct gps_data_t *newdata)
{
	gps_mask_t mask = 0;
	int i;

    /*
     * eg1. $GPGSA,A,3,,,,,,16,18,,22,24,,,3.6,2.1,2.2*3C
     * eg2. $GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*35
     * 1    = Mode:
     * M=Manual, forced to operate in 2D or 3D
     * A=Automatic, 3D/2D
     * 2    = Mode: 1=Fix not available, 2=2D, 3=3D
     * 3-14 = PRNs of satellites used in position fix (null for unused fields)
     * 15   = PDOP
     * 16   = HDOP
     * 17   = VDOP
     */
    if (count < 17) {
		gps_dbg("GPGSA: malformed, setting ONLINE_SET only.\n");
		mask = G_ONLINE_SET;
    } else {
		newdata->mode = atoi(field[2]);

		if (newdata->mode == 0 && field[2][0] == 'E')
	    	mask = 0;
		else
	    	mask = G_MODE_SET;

		/*
		 * 1-> not fixed, 2-> 2D mode, 3-> 3D mode
		 */
		gps_dbg("GPGSA sets mode(%s) %d\n", field[1], newdata->mode);

		newdata->dop.pdop = gps_safe_atof(field[15]);
		newdata->dop.hdop = gps_safe_atof(field[16]);
		newdata->dop.vdop = gps_safe_atof(field[17]);

		newdata->satellites_used = 0;
		memset(newdata->used, 0, sizeof(newdata->used));
		/* the magic 6 here counts the tag, two mode fields, and the DOP fields */
		for (i = 0; i < count - 6; i++) {
		    int prn = atoi(field[i + 3]);
		    /* pseudo random noise code (max 12)*/
		    if (prn > 0)
				newdata->used[newdata->satellites_used++] = prn;
		}

		mask |= G_DOP_SET;
#if GPS_DATA_DEBUG
		gps_dbg("GPGSA: mode=%d used=%d pdop=%.2f hdop=%.2f vdop=%.2f\n",
			    newdata->mode,
			    newdata->satellites_used,
			    newdata->dop.pdop,
			    newdata->dop.hdop,
			    newdata->dop.vdop);
#endif
    }

    return mask;
}

/* GPS Satellites in View */
static gps_mask_t gps_process_GSV(int count, char *field[],
			       struct gps_device_t *session, struct gps_data_t *newdata)
{
    /*
     * GSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75
     * 2           Number of sentences for full data
     * 1           Sentence 1 of 2
     * 08          Total number of satellites in view
     * 01          Satellite PRN number
     * 40          Elevation, degrees
     * 083         Azimuth, degrees
     * 46          Signal-to-noise ratio in decibels
     * <repeat for up to 4 satellites per sentence>
     * There my be up to three GSV sentences in a data packet
     */
    int n, fldnum;

    if (count <= 3) {
		gps_dbg("malformed GPGSV - fieldcount %d <= 3\n", count);
		gps_zero_satellites(newdata);
		newdata->satellites_visible = 0;
		return G_ONLINE_SET;
    }

	/* must be 1 or 4 times */
    if (count % 4 != 0) {
		gps_dbg("malformed GPGSV - fieldcount %d %% 4 != 0\n", count);
		gps_zero_satellites(newdata);
		newdata->satellites_visible = 0;
		return G_ONLINE_SET;
    }

	/* total number of GPGSV message */
    newdata->nmea.await = atoi(field[1]);
    if ((newdata->nmea.part = atoi(field[2])) < 1) {
		gps_dbg("malformed GPGSV - bad part\n");
		gps_zero_satellites(newdata);
		return 0;
    } else if (newdata->nmea.part == 1)
		gps_zero_satellites(newdata);

    for (fldnum = 4; fldnum < count;)
    {
		if (newdata->satellites_visible >= MAXCHANNELS) {
		    dev_err("internal error - too many satellites [%d]!\n",
	                    newdata->satellites_visible);
		    gps_zero_satellites(newdata);
		    break;
		}

		newdata->PRN[newdata->satellites_visible] = atoi(field[fldnum++]);

		//# NMEA-ID (33..64) to SBAS PRN.
		if (newdata->PRN[newdata->satellites_visible] >= 33 &&
			newdata->PRN[newdata->satellites_visible] <= 64)
		{
		    newdata->PRN[newdata->satellites_visible] += 87;
		}

		newdata->elevation[newdata->satellites_visible] = atoi(field[fldnum++]);
		newdata->azimuth[newdata->satellites_visible] = atoi(field[fldnum++]);
		/* SNR */
		newdata->ss[newdata->satellites_visible] = atoi(field[fldnum++]);

		if (newdata->PRN[newdata->satellites_visible] != 0)
		    newdata->satellites_visible++;
	}

    if (newdata->nmea.part == newdata->nmea.await &&
    	atoi(field[3]) != newdata->satellites_visible)
    {
		gps_dbg("GPGSV field 3 value of %d != actual count %d\n",
		    atoi(field[3]), newdata->satellites_visible);
	}

    /* not valid data until we've seen a complete set of parts */
    if (newdata->nmea.part < newdata->nmea.await) {
#if GPS_DATA_DEBUG
		gps_dbg("Partial satellite data (%d of %d).\n",
		    newdata->nmea.part, newdata->nmea.await);
#endif
		return G_ONLINE_SET;
	}

	for (n = 0; n < newdata->satellites_visible; n++)
		if (newdata->azimuth[n] != 0)
	    	goto sane;

    gps_dbg("Satellite data no good (%d of %d).\n",
		newdata->nmea.part, newdata->nmea.await);
    gps_zero_satellites(newdata);

	return G_ONLINE_SET;

sane:
    gps_dbg("GSV: Satellite data OK (%d of %d).\n",
		newdata->nmea.part, newdata->nmea.await);

#if GPS_DATA_DEBUG
	gps_dbg("GSV: Number of Satellite = %d\n",
				newdata->satellites_visible);
	for (n = 0; n < newdata->satellites_visible; n++) {
		gps_dbg("GSV: Satellite[%d] data PRN %d elv %d azi %d SNR %f\n", n,
			newdata->PRN[n],
			newdata->elevation[n],
			newdata->azimuth[n],
			newdata->ss[n]);
	}
#endif

    return G_SATELLITE_SET;
}

static void gps_packet_accept(struct gps_packet_t *pkt, int packet_type)
{
#if GPS_PKT_DEBUG
    char scratchbuf[GPS_BUFFER_LENGTH + 1];
#endif
    size_t packetlen = pkt->inbufptr - pkt->inbuffer;

    if (packetlen < sizeof(pkt->outbuffer)) {
		memcpy(pkt->outbuffer, pkt->inbuffer, packetlen);
		pkt->outbuflen = packetlen;
		pkt->outbuffer[packetlen] = '\0';
		pkt->type = packet_type;

#if GPS_PKT_DEBUG
		fprintf(stderr, "Packet type %d accepted %zu = %s\n", packet_type, packetlen,
			gps_packet_dump(scratchbuf,  sizeof(scratchbuf), (char *)pkt->outbuffer, pkt->outbuflen));
#endif
    } else {
		dev_err("Rejected too long packet type %d len %zu\n", packet_type, packetlen);
    }
}

static void gps_packet_discard(struct gps_packet_t *pkt)
{
#if GPS_PKT_DEBUG
    char scratchbuf[GPS_BUFFER_LENGTH + 1];
#endif
    size_t discard = pkt->inbufptr - pkt->inbuffer;
 	size_t remaining = pkt->inbuflen - discard;

    pkt->inbufptr = memmove(pkt->inbuffer, pkt->inbufptr, remaining);
    pkt->inbuflen = remaining;

#if GPS_PKT_DEBUG
	fprintf(stderr, "Packet discard of %zu, chars remaining is %zu = %s\n", discard, remaining,
		    gps_packet_dump(scratchbuf, sizeof(scratchbuf), (char *)pkt->inbuffer, pkt->inbuflen));
#endif
}

static void gps_character_pushback(struct gps_packet_t *pkt)
{
    --pkt->inbufptr;
    --pkt->char_counter;

#if GPS_PKT_DEBUG
    gps_dbg("%08ld: character pushed back\n", pkt->char_counter);
#endif
}

static void gps_character_discard(struct gps_packet_t *pkt)
{
#if GPS_PKT_DEBUG
	char scratchbuf[GPS_BUFFER_LENGTH + 1];
#endif
    memmove(pkt->inbuffer, pkt->inbuffer + 1, (size_t)--pkt->inbuflen);
    pkt->inbufptr = pkt->inbuffer;

#if GPS_PKT_DEBUG
	fprintf(stderr, "Character discarded, buffer %zu chars = %s\n",
		    pkt->inbuflen, gps_packet_dump(scratchbuf, sizeof(scratchbuf),
				    (char *)pkt->inbuffer, pkt->inbuflen));
#endif
}

static void gps_next_state(struct gps_packet_t *pkt, unsigned char c)
{
    switch (pkt->state) {
    case GROUND_STATE:
		if (c == '$') {
	    	pkt->state = NMEA_DOLLAR;
	    	break;
		}
		break;
    case NMEA_DOLLAR:
    	/* not supported vendor specific sentance */
		if (c == 'G')
		    pkt->state = NMEA_PUB_LEAD;
		else /* others not supported */
		    pkt->state = GROUND_STATE;
		break;
    case NMEA_PUB_LEAD:
		/*
		 * $GP == GPS, $GL = GLONASS only, $GN = mixed GPS and GLONASS,
		 */
		if (c == 'P' || c == 'N' || c == 'L')
		    pkt->state = NMEA_LEADER_END;
		else
		    pkt->state = GROUND_STATE;
		break;
    case NMEA_LEADER_END:
		if (c == '\r')
		    pkt->state = NMEA_CR;
		else if (c == '\n')
		    pkt->state = NMEA_RECOGNIZED;
		else if (c == '$') {
		    pkt->state = GROUND_STATE;
		    gps_character_pushback(pkt);
		} else if (!isprint(c))
		    pkt->state = GROUND_STATE;
		break;
    case NMEA_CR:
		if (c == '\n')
	    	pkt->state = NMEA_RECOGNIZED;
		/*
		 * There's a GPS called a Jackson Labs Firefly-1a that emits \r\r\n
		 * at the end of each sentence.  Don't be confused by this.
		 */
		else if (c == '\r')
		    pkt->state = NMEA_CR;
		else
		    pkt->state = GROUND_STATE;
		break;
    case NMEA_RECOGNIZED:
		if (c == '$')
		    pkt->state = NMEA_DOLLAR;
		else
		    pkt->state = GROUND_STATE;
		break;
    }
}

static void gps_parse_packet(struct gps_packet_t *pkt)
{
#if GPS_PKT_DEBUG
	char *state_table[] = {
 		"GROUND_STATE",
 		"NMEA_DOLLAR",
 		"NMEA_PUB_LEAD",
 		"NMEA_LEADER_END",
 		"NMEA_CR",
 		"NMEA_RECOGNIZED",
	};
#endif
	unsigned char c;

	pkt->outbuflen = 0;
	while (packet_buffered_input(pkt) > 0) {
		c = *pkt->inbufptr++;
		gps_next_state(pkt, c);

		#if GPS_PKT_DEBUG
		gps_dbg("%08ld: character '%c' [%02x], new state: %s\n",
		    pkt->char_counter, (isprint(c) ? c : '.'), c,
		    state_table[pkt->state]);
		#endif

		pkt->char_counter++;
		if (pkt->state == GROUND_STATE) {
	    	gps_character_discard(pkt);
		} else if (pkt->state == NMEA_RECOGNIZED) {
		    /*
		     * $PASHR packets have no checksum. Avoid the possibility
		     * that random garbage might make it look like they do.
		     */
		    if (strncmp((const char *)pkt->inbuffer, "$PASHR,", 7) != 0)
		    {
		    	int checksum_ok = 1;
				char csum[3] = { '0', '0', '0' };
				char *end;

				for (end = (char *)pkt->inbufptr - 1; isspace(*end); end--)
				    continue;

				while (strchr("0123456789ABCDEF", *end))
				    --end;

				/* calc checksum */
				if (*end == '*') {
				    unsigned int n, crc = 0;

					/* if $GPRMC,155123.000,A,4043.8432,N,07359.7653,W,0.15,83.25,200407,,*28
					 * checksum is XOR of all bytes -> (GPRMC,155123.000,A,4043.8432,N,07359.7653,W,0.15,83.25,200407,,)
					 */
				    for (n = 1; (char *)pkt->inbuffer + n < end; n++)
						crc ^= pkt->inbuffer[n];

				    snprintf(csum, sizeof(csum), "%02X", crc);
				    checksum_ok = (csum[0] == toupper(end[1]) && csum[1] == toupper(end[2]));
				}

				if (!checksum_ok) {
				    dev_err("bad checksum in NMEA packet; expected %s (r = %c%c%c).\n", csum, end[0], end[1], end[2]);
				    gps_packet_accept(pkt, GPS_PKT_BAD);
				    pkt->state = GROUND_STATE;
				    gps_packet_discard(pkt);
				    break;    /* exit case */
				}
	    	} /* if (strncmp((const char *)pkt->inbuffer, "$PASHR,", 7) != 0) { */
	    	/* checksum passed or not present */
	    	gps_packet_accept(pkt, GPS_PKT_NMEA);
	    	gps_packet_discard(pkt);
	    	break;
		}
	} /* while */
}

static ssize_t gps_get_packet(struct gps_packet_t *pkt)
{
    ssize_t recvd = 0;

	if (gps_fd < 0) {
		dev_err("Please open gps device before reading data!!.\n");
		return 0;
	}

    errno = 0;
	recvd = read(gps_fd, pkt->inbuffer + pkt->inbuflen, sizeof(pkt->inbuffer) - pkt->inbuflen);
	if (recvd == -1) {
		if ((errno == EAGAIN) || (errno == EINTR)) {
	    	gps_dbg("no bytes ready!!\n");
	    	recvd = 0;
	    	/* fall through, input inbuffer may be nonempty */
		} else {
	    	dev_err("Failed to read from gps device(errno: %s)\n", strerror(errno));
	    	return -1;
		}
	} else {
		#if GPS_PKT_DEBUG //# for debugging
		char scratchbuf[GPS_BUFFER_LENGTH + 1];

		gps_dbg("Read %zd chars to inbuffer offset %zd (total %zd): %s\n",
				recvd, pkt->inbuflen, pkt->inbuflen + recvd,
			gps_packet_dump(scratchbuf, sizeof(scratchbuf), (char *)pkt->inbufptr, (size_t)recvd));
		#endif

		pkt->inbuflen += recvd;
	}
//	gps_dbg("gps_get_packet -> %zd (%d)\n", recvd, errno);

	return recvd;
}

static gps_mask_t gps_nmea_parser(struct gps_device_t *pdev, struct gps_data_t *pdata)
{
	char *sentence;
	char *p, *s, *e;

	volatile char *t;

	int count;
	int retval = 0;

	unsigned int i;

	static struct {
		char *name;
		int nf;	/* minimum number of fields required to parse */
		nmea_decoder decoder;
    } nmea_phrase[] = {
		{"RMC", 8,  gps_process_RMC},
//		{"GGA", 13, NULL}, /* for future use */
//		{"GST", 8,  NULL}, /* for future use */
//		{"GLL", 7,  NULL}, /* for future use */
		{"GSA", 17, gps_process_GSA},
		{"GSV", 0,  gps_process_GSV},
//		{"VTG", 0,  NULL},	/* ignore Velocity Track made Good */
    };

    sentence = (char *)pdev->packet.outbuffer;
	if (strlen(sentence) > NMEA_MAX) {
		gps_dbg("packet of %zd chars rejected.\n", strlen(sentence));
		return G_ONLINE_SET;
    }

    /* copy to (n-1) last byte as null */
    gps_strlcpy((char *)pdev->fieldcopy, sentence, NMEA_MAX);
	/* discard the checksum part */
	for (p = (char *)pdev->fieldcopy; (*p != '*') && (*p >= ' ');) {
		++p;
	}

    if (*p == '*')
		*p++ = ',';	/* otherwise we drop the last field */

    *p = '\0';
    e = p;

    /* split sentence copy on commas, filling the field array */
   	count = 0;
   	t = p;	/* end of sentence */
    p = (char *)pdev->fieldcopy + 1;	/* beginning of tag, 'G' not '$' */
    while ((p != NULL) && (p <= t)) {
		pdev->field[count] = p;
		/* search for the next delimiter */
		if ((p = strchr(p, ',')) != NULL) {
	    	*p = '\0';	/* replace it with a NUL */
	    	count++; p++;
		}
    }

    /* point remaining fields at empty string, just in case */
    for (i = (unsigned int)count; i < (unsigned)(sizeof(pdev->field) / sizeof(pdev->field[0])); i++)
		pdev->field[i] = e;

	/* sentences handlers will tell us whren they have fractional time */
    pdata->nmea.latch_frac_time = 0;
	/* dispatch on field zero, the sentence tag */
    for (i = 0; i < (unsigned)(sizeof(nmea_phrase) / sizeof(nmea_phrase[0])); ++i) {
		s = pdev->field[0];

		if (strlen(nmea_phrase[i].name) == 3)
		    s += 2;		/* skip talker ID */

//		gps_dbg("nmea parse %s tags founded (field count %d)\n", s, count);

		if (strcmp(nmea_phrase[i].name, s) == 0) {
		    if (nmea_phrase[i].decoder != NULL && (count >= nmea_phrase[i].nf)) {
				retval = (nmea_phrase[i].decoder) (count, pdev->field, pdev, pdata);
		    } else
				retval = G_ONLINE_SET;	/* unknown sentence */
		    break;
		}
    }

    return retval;
}

/****************************************************
 * NAME : int dev_gps_get_data(struct gps_data_t *gdata)
 ****************************************************/
int dev_gps_get_data(struct gps_data_t *gdata)
{
	struct gps_device_t *session = (struct gps_device_t *)&gdevices;
	struct gps_packet_t *packet = (struct gps_packet_t *)&session->packet;

	gps_mask_t received;
	ssize_t res = 0;

	if (gdata == NULL) {
		dev_err("input buffer is null!!\n");
		return GPS_ERR_STATE;
	}

	gdata->set = 0; /* clear mask */
	packet->counter = 0;
	res = gps_get_packet(packet);
	/* %z is size_t */
//    gps_dbg("%zd new characters\n", res);
	if (res < 0) {
		dev_err("The GPS have problem (err = %d)!!\n", res);
		return GPS_ERR_STATE;
	} else if (res == 0 && packet_buffered_input(packet) <= 0) {
//		gps_dbg("GPS no more input data!! (%d)\n", res);
		/* reinitialize gps device!! */
		if (!session->killzero) {
			/* clear rmc status */
			gdata->rmc_status = 0;
			session->killzero = 1;
		}
		return GPS_OFFLINE;
	}

	if (session->killzero)
		session->killzero = 0; /* for rmc_status clear */

	/* others from uart nput data or buffered data */
	while (packet_buffered_input(packet) > 0) {
		/* consume from the packet input inbuffer */
		gps_parse_packet(packet);

		/* if input inbuffer is full, discard */
	    if (sizeof(packet->inbuffer) == (packet->inbuflen)) {
			gps_packet_discard(packet);
			packet->state = GROUND_STATE;
			gps_dbg("packet buffer is full\n");
			continue;
	    }

		/* got new data, but no packet */
		if (packet->outbuflen == 0) {
			gps_dbg("New data on GPS, not yet a packet\n");
			continue;
		}

		if (packet->type == GPS_PKT_NMEA) {
			received = gps_nmea_parser(session, gdata);
			gdata->set |= received;
			packet->counter++;
		}
	}

//	gps_dbg("parser %d packets done.!!(mask 0x%08lx)!!\n", packet->counter, gdata->set);

	return GPS_ONLINE;
}

/****************************************************
 * NAME : int dev_gps_open(void)
 *
 * Desc   : Configure the GPS Hardware.
 *
 * INPUT  :
 *   PARAMETERS: none
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values: 0(succeed) -1(failure)
 ****************************************************/
int dev_gps_init(void)
{
	struct gps_device_t *session = (struct gps_device_t *)&gdevices;
	struct termios options;

	int fd = -1, fl;

	fd = open(GPS_DEV_NAME, O_RDONLY | O_NOCTTY | O_NONBLOCK);
	if (fd < 0) {
		dev_err("Failed to open %s device.\n", GPS_DEV_NAME);
		return -1;
	}
	gps_fd = fd;

	/* exclusion-lock the device before doing any reads */
	ioctl(fd, (unsigned long)TIOCEXCL);

	/* include <asm/fcntl.h>
	 * O_NONBLOCK: if no data, return -1
	 * O_NDELAY: if no data, return 0
	 */
    fl = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, fl & ~O_NONBLOCK);

	/* Get the current options for the port */
	tcgetattr(fd, &options);

	/* set baud rate */
	cfsetispeed(&options, (speed_t)GPS_DEV_RATE);
    cfsetospeed(&options, (speed_t)GPS_DEV_RATE);
	tcsetattr(fd, TCSANOW, &options);
	tcflush(fd, TCIOFLUSH); /* in/out flush */

	options.c_oflag = 0; //# ~(OPOST); post-process disable
	/*
	 * if neither IGNPAR nor PARMRK is set,
	 * read character with a parity error or framing error as '\0'
	 */
	options.c_iflag = 0; //# not set IGNPAR
	/* noncanonical, no echo, no signal(INTR,QUIT) */
	options.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
	options.c_cflag &= ~(PARENB | PARODD | CRTSCTS | CSTOPB);
	options.c_cflag |= CREAD | CLOCAL | CS8;

	options.c_cc[VMIN]  = 0;  /* blocking read until 0 chars received */
	/*
	 * if VMIN = 0, when read is called returns either when at least 1 byte
	 * of data available, or when timer expired. if the timer expires without
	 * any input read returns 0
	 */
	options.c_cc[VTIME] = 3; //# 1-> 100ms, 3-> 300ms

	tcflush(fd, TCIFLUSH); //# input flush.
	tcsetattr(fd, TCSANOW, &options);

	gps_packet_reset(&session->packet);

	session->killzero = 0;
	session->leap_seconds = GPS_LEAPSECOND_NOW; //# refer to leap seconds.

#if 0
	session->start_time = time(NULL);
	if (session->start_time < GPS_EPOCH)
		dev_err("system time looks bogus, dates may not be reliable.\n");
	else {
		now = localtime(&session->start_time);
		now->tm_year += 1900;
		gps_century = now->tm_year - (now->tm_year % 100);
		gps_dbg("startup at century %d (%d)\n", gps_century, (int)session->start_time);
	}
#else
	gps_century = 2000; //# fixed gps 2000 year..
#endif
	return 0;
}

/****************************************************
 * NAME : int dev_gps_close(void)
 *
 * Desc   : close the GPS Hardware.
 *
 * INPUT  :
 *   PARAMETERS: none
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values: 0(succeed) -1(failure)
 ****************************************************/
void dev_gps_close(void)
{
	if (gps_fd >= 0) {
		tcflush(gps_fd, TCIFLUSH); /* input flush */
		close(gps_fd);
		gps_fd = -1;
	}
}
