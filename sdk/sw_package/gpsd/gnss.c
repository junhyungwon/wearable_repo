/******************************************************************************
 * NEXTT360 Board
 * Copyright by LF, Incoporated. All Rights Reserved.
 * based on gpsd.
 *---------------------------------------------------------------------------*/
 /**
 * @file    gnss.c
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
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <termios.h>
#include <ctype.h>

#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#include <time.h>
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/*
 * gpsd defines
 */
#define TV_NORM(tv)  \
    do { \
		if ( US_IN_SEC <= (tv)->tv_usec ) { \
			(tv)->tv_usec -= US_IN_SEC; \
			(tv)->tv_sec++; \
		} else if ( 0 > (tv)->tv_usec ) { \
			(tv)->tv_usec += US_IN_SEC; \
			(tv)->tv_sec--; \
		} \
    } while (0)

#define TS_SUB(r, ts1, ts2) \
    do { \
		(r)->tv_sec = (ts1)->tv_sec - (ts2)->tv_sec; \
		(r)->tv_nsec = (ts1)->tv_nsec - (ts2)->tv_nsec; \
			TS_NORM( r ); \
    } while (0)
		
#define TS_SUB_D(ts1, ts2) \
    ((double)((ts1)->tv_sec - (ts2)->tv_sec) + \
      ((double)((ts1)->tv_nsec - (ts2)->tv_nsec) * 1e-9))


#define DTOTS(ts, d) \
    do { \
        double int_part; \
	(ts)->tv_nsec = (long)(modf(d, &int_part) * 1e9); \
	(ts)->tv_sec = (time_t)int_part; \
    } while (0)
		  
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
#define DD(s)   ((int)((s)[0]-'0')*10+(int)((s)[1]-'0'))

/* convert NMEA sigid to ublox sigid */
static unsigned char nmea_sigid_to_ubx(unsigned char nmea_sigid)
{
    unsigned char ubx_sigid = 0;

    switch (nmea_sigid) {
    default:
        /* FALLTHROUGH */
    case 0:
        /* missing, assume GPS L1 */
        ubx_sigid = 0;
        break;
    case 1:
        /* L1 */
        ubx_sigid = 0;
        break;
    case 2:
        /* E5, could be 5 or 6. */
        ubx_sigid = 5;
        break;
    case 3:
        /* B2 or L2, could be 2 or 3. */
        ubx_sigid = 2;
        break;
    case 5:
        /* L2 */
        ubx_sigid = 4;
        break;
    case 6:
        /* L2CL */
        ubx_sigid = 3;
        break;
    case 7:
        /* E1, could be 0 or 1. */
        ubx_sigid = 0;
        break;
    }

    return ubx_sigid;
}

static int nmeaid_to_prn(char *talker, int nmea_satnum,
                         int nmea_gnssid,
                         unsigned char *ubx_gnssid,
                         unsigned char *ubx_svid)
{
    /*
     * According to https://github.com/mvglasow/satstat/wiki/NMEA-IDs
     * and u-blox documentation.
     * NMEA IDs can be roughly divided into the following ranges:
     *
     *   1..32:  GPS
     *   33..64: Various SBAS systems (EGNOS, WAAS, SDCM, GAGAN, MSAS)
     *   65..96: GLONASS
     *   152..158: Various SBAS systems (EGNOS, WAAS, SDCM, GAGAN, MSAS)
     *   173..182: IMES
     *   193..197: QZSS   (undocumented u-blox goes to 199)
     *   201..235: Beidou (not NMEA, not u-blox?)
     *   301..336: Galileo
     *   401..437: Beidou
     *   null: GLONASS unused
     *
     * The issue is what to do when GPSes from these different systems
     * fight for IDs in the  1-32 range, as in this pair of Beidou sentences
     *
     * $BDGSV,2,1,07,01,00,000,45,02,13,089,35,03,00,000,37,04,00,000,42*6E
     * $BDGSV,2,2,07,05,27,090,,13,19,016,,11,07,147,*5E
     *
     * Because the PRNs are only used for generating a satellite
     * chart, mistakes here aren't dangerous.  The code will record
     * and use multiple sats with the same ID in one skyview; in
     * effect, they're recorded by the order in which they occur
     * rather than by PRN.
     */
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

static int faa_mode(char mode)
{
    int newstatus = STATUS_FIX;
	
	//dprintf("faa_mode default status %d(%c)\n", newstatus, mode);
	
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
    case 'E':   /* Estimated dead reckoning */
        newstatus = STATUS_DR;
        break;
    case 'F':   /* Float RTK */
        newstatus = STATUS_RTK_FLT;
        break;
    case 'N':   /* Data Not Valid */
        /* already handled, for paranoia sake also here */
        newstatus = STATUS_NO_FIX;
        break;
    case 'P':   /* Precise (NMEA 4+) */
        newstatus = STATUS_DGPS_FIX;    /* sort of DGPS */
        break;
    case 'R':   /* fixed RTK */
        newstatus = STATUS_RTK_FIX;
        break;
    case 'S':   /* simulator */
        newstatus = STATUS_NO_FIX;      /* or maybe MODE_FIX? */
        break;
    }
	
	//dprintf("return faa_mode status %d\n", newstatus);
	
    return newstatus;
}

/* sentence supplied ddmmyy, but no century part
 *
 * return: 0 == OK,  greater than zero on failure
 */
static int merge_ddmmyy(char *ddmmyy, struct gps_device_t *session)
{
    int yy;
    int mon;
    int mday;
    int year;
    unsigned i;    /* NetBSD complains about signed array index */

    if (NULL == ddmmyy) {
        return 1;
    }
    for (i = 0; i < 6; i++) {
        /* NetBSD 6 wants the cast */
        if (0 == isdigit((int)ddmmyy[i])) {
            /* catches NUL and non-digits */
            /* Telit HE910 can set year to "-1" (1999 - 2000) */
            dprintf("merge_ddmmyy(%s), malformed date\n",  ddmmyy);
            return 2;
        }
    }
    /* check for termination */
    if ('\0' != ddmmyy[6]) {
        /* missing NUL */
        dprintf("merge_ddmmyy(%s), malformed date\n",  ddmmyy);
        return 3;
    }

    /* should be no defects left to segfault DD() */
    yy = DD(ddmmyy + 4);
    mon = DD(ddmmyy + 2);
    mday = DD(ddmmyy);

    /* check for century wrap */
    if (session->nmea.date.tm_year % 100 == 99 && yy == 0)
        gpsd_century_update(session, session->context->century + 100);
    
	year = (session->context->century + yy);

    /* 32 bit systems will break in 2038.
     * Telix fails on GPS rollover to 2099, which 32 bit system
     * can not handle.  So wrap at 2080.  That way 64 bit systems
     * work until 2080, and 2099 gets reported as 1999.
     * since GPS epoch started in 1980, allows for old NMEA to work.
     */
    if (2080 <= year) {
        year -= 100;
    }

    if ( (1 > mon ) || (12 < mon ) ) {
        dprintf("merge_ddmmyy(%s), malformed month\n",  ddmmyy);
        return 4;
    } else if ( (1 > mday ) || (31 < mday ) ) {
        dprintf("merge_ddmmyy(%s), malformed day\n",  ddmmyy);
        return 5;
    } else {
		dprintf("merge_ddmmyy(%s) sets year %d\n",
                 ddmmyy, year);
        session->nmea.date.tm_year = year - 1900;
        session->nmea.date.tm_mon = mon - 1;
        session->nmea.date.tm_mday = mday;
    }
    dprintf("merge_ddmmyy(%s) %d %d %d\n",
             ddmmyy,
             session->nmea.date.tm_mon,
             session->nmea.date.tm_mday,
             session->nmea.date.tm_year);
    return 0;
}

/* decode an hhmmss.ss string into struct tm data and nsecs
 *
 * return: 0 == OK,  otherwise failure
 */
static int decode_hhmmss(struct tm *date, long *nsec, char *hhmmss,
                         struct gps_device_t *session)
{
    int old_hour = date->tm_hour;
    int i, sublen;

    if (NULL == hhmmss) {
        return 1;
    }
    for (i = 0; i < 6; i++) {
        /* NetBSD 6 wants the cast */
        if (0 == isdigit((int)hhmmss[i])) {
            /* catches NUL and non-digits */
            dprintf("decode_hhmmss(%s), malformed time\n",  hhmmss);
            return 2;
        }
    }
    /* don't check for termination, might have fractional seconds */
    date->tm_hour = DD(hhmmss);
    if (date->tm_hour < old_hour)  /* midnight wrap */
        date->tm_mday++;
    date->tm_min = DD(hhmmss + 2);
    date->tm_sec = DD(hhmmss + 4);

    if ('.' == hhmmss[6] &&
        /* NetBSD 6 wants the cast */
        0 != isdigit((int)hhmmss[7])) {
        i = atoi(hhmmss + 7);
        sublen = strlen(hhmmss + 7);
        *nsec = (long)i * (long)pow(10.0, 9 - sublen);
    } else {
        *nsec = 0;
    }

    return 0;
}

/* update from a UTC time
 *
 * return: 0 == OK,  greater than zero on failure
 */
static int merge_hhmmss(char *hhmmss, struct gps_device_t *session)
{
    int old_hour = session->nmea.date.tm_hour;
    int i, sublen;

    if (NULL == hhmmss) {
        return 1;
    }
    for (i = 0; i < 6; i++) {
        /* NetBSD 6 wants the cast */
        if (0 == isdigit((int)hhmmss[i])) {
            /* catches NUL and non-digits */
            dprintf("merge_hhmmss(%s), malformed time\n",  hhmmss);
            return 2;
        }
    }
    /* don't check for termination, might have fractional seconds */

    session->nmea.date.tm_hour = DD(hhmmss);
    if (session->nmea.date.tm_hour < old_hour)  /* midnight wrap */
        session->nmea.date.tm_mday++;
    session->nmea.date.tm_min = DD(hhmmss + 2);
    session->nmea.date.tm_sec = DD(hhmmss + 4);

    session->nmea.subseconds.tv_sec = 0;
    if ('.' == hhmmss[6] &&
        /* NetBSD 6 wants the cast */
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
    char ts_buf[TIMESPEC_LEN];

    if (fld[0] != '\0') {
        session->nmea.last_frac_time = session->nmea.this_frac_time;
        DTOTS(&session->nmea.this_frac_time, safe_atof(fld));
        session->nmea.latch_frac_time = true;
        dprintf("%s: registers fractional time %s\n",
                 tag,
                 timespec_str(&session->nmea.this_frac_time, ts_buf,
                              sizeof(ts_buf)));
    }
}


static gps_mask_t fill_dop(const struct gps_data_t * gpsdata,
			   struct dop_t * dop)
{
    double prod[4][4];
    double inv[4][4];
    double satpos[MAXCHANNELS][4];
    double xdop, ydop, hdop, vdop, pdop, tdop, gdop;
    int i, j, k, n;

    memset(satpos, 0, sizeof(satpos));

    for (n = k = 0; k < gpsdata->satellites_visible; k++) {
        if (!gpsdata->skyview[k].used) {
             /* skip unused sats */
             continue;
        }
        if (1 > gpsdata->skyview[k].PRN) {
             /* skip bad PRN */
             continue;
        }
		
		if (0 == isfinite(gpsdata->skyview[k].azimuth) ||
            0 > gpsdata->skyview[k].azimuth ||
            359 < gpsdata->skyview[k].azimuth) {
             /* skip bad azimuth */
             continue;
        }
		
		if (0 == isfinite(gpsdata->skyview[k].elevation) ||
            90 < fabs(gpsdata->skyview[k].elevation)) {
             /* skip bad elevation */
             continue;
        }
        
		const struct satellite_t *sp = &gpsdata->skyview[k];
        satpos[n][0] = sin(sp->azimuth * DEG_2_RAD)
            * cos(sp->elevation * DEG_2_RAD);
        satpos[n][1] = cos(sp->azimuth * DEG_2_RAD)
            * cos(sp->elevation * DEG_2_RAD);
        satpos[n][2] = sin(sp->elevation * DEG_2_RAD);
        satpos[n][3] = 1;
        dprintf("PRN=%3d az=%.1f ael%.1f (%f, %f, %f)\n",
                 gpsdata->skyview[k].PRN,
                 gpsdata->skyview[k].azimuth,
                 gpsdata->skyview[k].elevation,
                 satpos[n][0], satpos[n][1], satpos[n][2]);
        n++;
    }
    
	/* can't use gpsdata->satellites_used as that is a counter for xxGSA,
     * and gets cleared at odd times */
    dprintf("Sats used (%d):\n", n);

    /* If we don't have 4 satellites then we don't have enough information to calculate DOPS */
    if (n < 4) {
		return 0;		/* Is this correct return code here? or should it be ERROR_SET */
    }

    memset(prod, 0, sizeof(prod));
    memset(inv, 0, sizeof(inv));

    for (i = 0; i < 4; ++i) {	//< rows
		for (j = 0; j < 4; ++j) {	//< cols
			prod[i][j] = 0.0;
			for (k = 0; k < n; ++k) {
			prod[i][j] += satpos[k][i] * satpos[k][j];
			}
		}
    }

    if (matrix_invert(prod, inv)) {
    } else {
		dprintf("LOS matrix is singular, can't calculate DOPs\n");
		return 0;
    }

    xdop = sqrt(inv[0][0]);
    ydop = sqrt(inv[1][1]);
    hdop = sqrt(inv[0][0] + inv[1][1]);
    vdop = sqrt(inv[2][2]);
    pdop = sqrt(inv[0][0] + inv[1][1] + inv[2][2]);
    tdop = sqrt(inv[3][3]);
    gdop = sqrt(inv[0][0] + inv[1][1] + inv[2][2] + inv[3][3]);
	
	#if 0
    dprintf("DOPS computed/reported: X=%f/%f, Y=%f/%f, H=%f/%f, V=%f/%f, "
	     "P=%f/%f, T=%f/%f, G=%f/%f\n",
	     xdop, dop->xdop, ydop, dop->ydop, hdop, dop->hdop, vdop,
	     dop->vdop, pdop, dop->pdop, tdop, dop->tdop, gdop, dop->gdop);
	#endif
    /* Check to see which DOPs we already have.  Save values if no value
     * from the GPS.  Do not overwrite values which came from the GPS */
    if (isfinite(dop->xdop) == 0) {
		dop->xdop = xdop;
    }
    if (isfinite(dop->ydop) == 0) {
		dop->ydop = ydop;
    }
    if (isfinite(dop->hdop) == 0) {
		dop->hdop = hdop;
    }
    if (isfinite(dop->vdop) == 0) {
		dop->vdop = vdop;
    }
    if (isfinite(dop->pdop) == 0) {
		dop->pdop = pdop;
    }
    if (isfinite(dop->tdop) == 0) {
		dop->tdop = tdop;
    }
    if (isfinite(dop->gdop) == 0) {
		dop->gdop = gdop;
    }

    return DOP_SET;
}

ssize_t generic_get(struct gps_device_t *session)
{
    return packet_get(session->gpsdata.gps_fd, &session->lexer);
}

/* Global Positioning System Fix Data */
static gps_mask_t processGGA(int c, char *field[],
                               struct gps_device_t *session)
{
    /*
     * GGA,123519,4807.038,N,01131.324,E,1,08,0.9,545.4,M,46.9,M, , *42
     * 1     123519       Fix taken at 12:35:19 UTC
     * 2,3   4807.038,N   Latitude 48 deg 07.038' N
     * 4,5   01131.324,E  Longitude 11 deg 31.324' E
     * 6     1            Fix quality:
     *                     0 = invalid,
     *                     1 = GPS,
     *                         u-blox may use 1 for Estimated
     *                     2 = DGPS,
     *                     3 = PPS (Precise Position Service),
     *                     4 = RTK (Real Time Kinematic) with fixed integers,
     *                     5 = Float RTK,
     *                     6 = Estimated,
     *                     7 = Manual,
     *                     8 = Simulator
     * 7     08           Number of satellites in use
     * 8     0.9          Horizontal dilution of position
     * 9,10  545.4,M      Altitude, Meters MSL
     * 11,12 46.9,M       Height of geoid (mean sea level) above WGS84
     *                    ellipsoid, in Meters
     * 13    33           time in seconds since last DGPS update
     *                    usually empty
     * 14    1023         DGPS station ID number (0000-1023)
     *                    usually empty
     *
     * Some GPS, like the SiRFstarV in NMEA mode, send both GPGSA and
     * GLGPSA with identical data.
     */
    gps_mask_t mask = ONLINE_SET;
    int newstatus;
    char last_last_gga_talker = session->nmea.last_gga_talker;
    int fix;
    int satellites_visible;
    session->nmea.last_gga_talker = field[0][1];

    if (0 == strlen(field[6])) {
        /* no data is no data, assume no fix
         * the test/daemon/myguide-3100.log shows lat/lon/alt but
         * no status, and related RMC shows no fix. */
        fix = -1;
    } else {
        fix = atoi(field[6]);
    }
    switch (fix) {
    case 0:     /* no fix */
        newstatus = STATUS_NO_FIX;
        break;
    case 1:
        /* could be 2D, 3D, GNSSDR */
        newstatus = STATUS_FIX;
        break;
    case 2:     /* differential */
        newstatus = STATUS_DGPS_FIX;
        break;
    case 3:
        /* GPS PPS, fix valid, could be 2D, 3D, GNSSDR */
        newstatus = STATUS_PPS_FIX;
        break;
    case 4:     /* RTK integer */
        newstatus = STATUS_RTK_FIX;
        break;
    case 5:     /* RTK float */
        newstatus = STATUS_RTK_FLT;
        break;
    case 6:
        /* dead reckoning, could be valid or invalid */
        newstatus = STATUS_DR;
        break;
    case 7:
        /* manual input, surveyed */
        newstatus = STATUS_TIME;
        break;
    case 8:
        /* simulated mode */
        /* Garmin GPSMAP and Gecko sends an 8, but undocumented why */
        newstatus = STATUS_SIM;
        break;
    default:
        newstatus = -1;
        break;
    }
    if (0 <= newstatus) {
        session->gpsdata.status = newstatus;
        mask = STATUS_SET;
    }
    /*
     * There are some receivers (the Trimble Placer 450 is an example) that
     * don't ship a GSA with mode 1 when they lose satellite lock. Instead
     * they just keep reporting GGA and GSA on subsequent cycles with the
     * timestamp not advancing and a bogus mode.
     *
     * On the assumption that GGA is only issued once per cycle we can
     * detect this here (it would be nicer to do it on GSA but GSA has
     * no timestamp).
     *
     * SiRFstarV breaks this assumption, sending GGA with different
     * talker IDs.
     */
    if ('\0' != last_last_gga_talker &&
        last_last_gga_talker != session->nmea.last_gga_talker) {
        /* skip the time check */
        session->nmea.latch_mode = 0;
    } else {
        session->nmea.latch_mode = strncmp(field[1],
                          session->nmea.last_gga_timestamp,
                          sizeof(session->nmea.last_gga_timestamp))==0;
    }

    if (session->nmea.latch_mode) {
        session->gpsdata.status = STATUS_NO_FIX;
        session->newdata.mode = MODE_NO_FIX;
        dprintf("xxGGA: latch mode\n");
    } else
        (void)strlcpy(session->nmea.last_gga_timestamp, field[1],
                      sizeof(session->nmea.last_gga_timestamp));

    /* satellites_visible is used as an accumulator in xxGSV
     * so if we set it here we break xxGSV
     * Some GPS, like SiRFstarV NMEA, report per GNSS used
     * counts in GPGGA and GLGGA.
     * session->gpsdata.satellites_visible = atoi(field[7]);
     */
    satellites_visible = atoi(field[7]);

    if (0 == merge_hhmmss(field[1], session)) {
        register_fractional_time(field[0], field[1], session);
        if (session->nmea.date.tm_year == 0)
            dprintf("can't use GGA time until after ZDA or RMC"
                     " has supplied a year.\n");
        else {
            mask |= TIME_SET;
        }
    }

    if (0 == do_lat_lon(&field[2], &session->newdata)) {
        session->newdata.mode = MODE_2D;
        mask |= LATLON_SET;
        if ('\0' != field[11][0]) {
            session->newdata.geoid_sep = safe_atof(field[11]);
        } else {
			dprintf("wgs84 separation\n");
            session->newdata.geoid_sep = wgs84_separation(
                session->newdata.latitude, session->newdata.longitude);
        }
        /*
         * SiRF chipsets up to version 2.2 report a null altitude field.
         * See <http://www.sirf.com/Downloads/Technical/apnt0033.pdf>.
         * If we see this, force mode to 2D at most.
         */
        if ('\0' != field[9][0]) {
            /* altitude is MSL */
            session->newdata.altMSL = safe_atof(field[9]);
            /* Let gpsd_error_model() deal with altHAE */
            mask |= ALTITUDE_SET;
			dprintf("GPGGA altMSL set!!\n");
            /*
             * This is a bit dodgy.  Technically we shouldn't set the mode
             * bit until we see GSA.  But it may be later in the cycle,
             * some devices like the FV-18 don't send it by default, and
             * elsewhere in the code we want to be able to test for the
             * presence of a valid fix with mode > MODE_NO_FIX.
             *
             * Use satellites_visible as double check on MODE_3D
             */
            if (4 <= satellites_visible) {
                session->newdata.mode = MODE_3D;
				dprintf("GPGGA MODE_3D\n");
            }
        }
        if (3 > satellites_visible) {
            session->newdata.mode = MODE_NO_FIX;
        }
    } else {
        session->newdata.mode = MODE_NO_FIX;
    }
    mask |= MODE_SET;

    if ('\0' != field[8][0]) {
        /* why not to newdata? */
        session->gpsdata.dop.hdop = safe_atof(field[8]);
    }

    /* get DGPS stuff */
    if ('\0' != field[13][0] &&
        '\0' != field[14][0]) {
		/* both, or neither */
        double age;
        int station;
		
		dprintf("GET DGPS Dtuff\n");
        age = safe_atof(field[13]);
        station = atoi(field[14]);
        if (0.09 < age || 0 < station) {
            /* ignore both zeros */
            //session->newdata.dgps_age = age;
            //session->newdata.dgps_station = station;
			dprintf("dgps ignore both zeros!!\n");
        }
    }

#if 0
    dprintf("GGA: hhmmss=%s lat=%.2f lon=%.2f altMSL=%.2f mode=%d status=%d\n",
             field[1],
             session->newdata.latitude,
             session->newdata.longitude,
             session->newdata.altMSL,
             session->newdata.mode,
             session->gpsdata.status);
#endif			 
    return mask;
}

/* Geographic position - Latitude, Longitude */
static gps_mask_t processGLL(int count, char *field[],
                             struct gps_device_t *session)
{
    /* Introduced in NMEA 3.0.
     *
     * $GPGLL,4916.45,N,12311.12,W,225444,A,A*5C
     *
     * 1,2: 4916.46,N    Latitude 49 deg. 16.45 min. North
     * 3,4: 12311.12,W   Longitude 123 deg. 11.12 min. West
     * 5:   225444       Fix taken at 22:54:44 UTC
     * 6:   A            Data valid
     * 7:   A            Autonomous mode
     * 8:   *5C          Mandatory NMEA checksum
     *
     * 1,2 Latitude, N (North) or S (South)
     * 3,4 Longitude, E (East) or W (West)
     * 5 UTC of position
     * 6 A = Active, V = Invalid data
     * 7 Mode Indicator
     *    See faa_mode() for possible mode values.
     *
     * I found a note at <http://www.secoh.ru/windows/gps/nmfqexep.txt>
     * indicating that the Garmin 65 does not return time and status.
     * SiRF chipsets don't return the Mode Indicator.
     * This code copes gracefully with both quirks.
     *
     * Unless you care about the FAA indicator, this sentence supplies nothing
     * that GPRMC doesn't already.  But at least two (Garmin GPS 48 and
     * Magellan Triton 400) actually ship updates in GLL that aren't redundant.
     *
     */
    char *status = field[7];
    gps_mask_t mask = ONLINE_SET;

    if (field[5][0] != '\0') {
        if (0 == merge_hhmmss(field[5], session)) {
            register_fractional_time(field[0], field[5], session);
            if (session->nmea.date.tm_year == 0)
                dprintf("can't use GLL time until after ZDA or RMC"
                         " has supplied a year.\n");
            else {
                mask = TIME_SET;
            }
        }
    }
    if ('\0' == field[6][0] ||
        'V' == field[6][0]) {
        /* Invalid */
        session->gpsdata.status = STATUS_NO_FIX;
        session->newdata.mode = MODE_NO_FIX;
        mask |= STATUS_SET | MODE_SET;
    } else if ('A' == field[6][0] &&
        (count < 8 || *status != 'N') &&
        0 == do_lat_lon(&field[1], &session->newdata)) {
        int newstatus;

        mask |= LATLON_SET;

        newstatus = STATUS_FIX;
        if (count >= 8) {
            newstatus = faa_mode(*status);
        }
        /*
         * This is a bit dodgy.  Technically we shouldn't set the mode
         * bit until we see GSA, or similar.  But it may be later in the
         * cycle, some devices like the FV-18 don't send it by default,
         * and elsewhere in the code we want to be able to test for the
         * presence of a valid fix with mode > MODE_NO_FIX.
         */
        if (0 != isfinite(session->gpsdata.fix.altHAE) ||
            0 != isfinite(session->gpsdata.fix.altMSL)) {
            session->newdata.mode = MODE_3D;
            mask |= MODE_SET;
        } else if (3 < session->gpsdata.satellites_used) {
            /* 4 sats used means 3D */
            session->newdata.mode = MODE_3D;
            mask |= MODE_SET;
        } else if (MODE_2D > session->gpsdata.fix.mode ||
                   (0 == isfinite(session->oldfix.altHAE) &&
                    0 == isfinite(session->oldfix.altMSL))) {
            session->newdata.mode = MODE_2D;
            mask |= MODE_SET;
        }
        session->gpsdata.status = newstatus;
    } else {
        session->gpsdata.status = STATUS_NO_FIX;
        session->newdata.mode = MODE_NO_FIX;
        mask |= STATUS_SET | MODE_SET;
    }
#if 0
    dprintf("GLL: hhmmss=%s lat=%.2f lon=%.2f mode=%d status=%d\n",
             field[5],
             session->newdata.latitude,
             session->newdata.longitude,
             session->newdata.mode,
             session->gpsdata.status);
#endif			 
    return mask;
}

/* Geographic position - Latitude, Longitude, and more */
static gps_mask_t processGNS(int count, char *field[],
                               struct gps_device_t *session)
{
    /* Introduced in NMEA 4.0?
     *
     * This mostly duplicates RMC, except for the multi GNSS mode
     * indicatore.
     *
     * Example.  Ignore the line break.
     * $GPGNS,224749.00,3333.4268304,N,11153.3538273,W,D,19,0.6,406.110,
     *        -26.294,6.0,0138,S,*6A
     *
     * 1:  224749.00     UTC HHMMSS.SS.  22:47:49.00
     * 2:  3333.4268304  Latitude DDMM.MMMMM. 33 deg. 33.4268304 min
     * 3:  N             Latitude North
     * 4:  12311.12      Longitude 111 deg. 53.3538273 min
     * 5:  W             Longitude West
     * 6:  D             FAA mode indicator
     *                     see faa_mode() for possible mode values
     *                     May be one to four characters.
     *                       Char 1 = GPS
     *                       Char 2 = GLONASS
     *                       Char 3 = ?
     *                       Char 4 = ?
     * 7:  19           Number of Satellites used in solution
     * 8:  0.6          HDOP
     * 9:  406110       MSL Altitude in meters
     * 10: -26.294      Geoid separation in meters
     * 11: 6.0          Age of differential corrections, in seconds
     * 12: 0138         Differential reference station ID
     * 13: S            NMEA 4.1+ Navigation status
     *                   S = Safe
     *                   C = Caution
     *                   U = Unsafe
     *                   V = Not valid for navigation
     * 8:   *6A          Mandatory NMEA checksum
     *
     */
    int newstatus;
    int satellites_used;
    gps_mask_t mask = ONLINE_SET;

    if (field[1][0] != '\0') {
        if (0 == merge_hhmmss(field[1], session)) {
            register_fractional_time(field[0], field[1], session);
            if (session->nmea.date.tm_year == 0) {
                dprintf("can't use GNS time until after ZDA or RMC"
                         " has supplied a year.\n");
            } else {
                mask = TIME_SET;
            }
        }
    }

    /* FAA mode: not valid, ignore
     * Yes, in 2019 a GLONASS only fix may be valid, but not worth
     * the confusion */
    if ('\0' == field[6][0] ||      /* FAA mode: missing */
        'N' == field[6][0]) {       /* FAA mode: not valid */
        session->newdata.mode = MODE_NO_FIX;
        mask |= MODE_SET;
        return mask;
    }
    /* navigation status, assume S=safe and C=caution are OK */
    /* can be missing on valid fix */
    if ('U' == field[13][0] ||      /* Unsafe */
        'V' == field[13][0]) {      /* not valid */
        return mask;
    }

    satellites_used = atoi(field[7]);

    if (0 == do_lat_lon(&field[2], &session->newdata)) {
        mask |= LATLON_SET;
        session->newdata.mode = MODE_2D;

        if ('\0' != field[9][0]) {
            /* altitude is MSL */
            session->newdata.altMSL = safe_atof(field[9]);
            if (0 != isfinite(session->newdata.altMSL)) {
                mask |= ALTITUDE_SET;
                if (3 < satellites_used) {
                    /* more than 3 sats used means 3D */
                    session->newdata.mode = MODE_3D;
                }
            }
            /* only need geoid_sep if in 3D mode */
            if ('\0' != field[10][0]) {
                session->newdata.geoid_sep = safe_atof(field[10]);
            }
            /* Let gpsd_error_model() deal with geoid_sep and altHAE */
        }
    } else {
        session->newdata.mode = MODE_NO_FIX;
        mask |= MODE_SET;
    }

    if (field[8][0] != '\0') {
        session->gpsdata.dop.hdop = safe_atof(field[8]);
    }

    newstatus = faa_mode(field[6][0]);

    session->gpsdata.status = newstatus;
    mask |= MODE_SET;

    /* get DGPS stuff */
    if ('\0' != field[11][0] &&
        '\0' != field[12][0]) {
        /* both, or neither */
        session->newdata.dgps_age = safe_atof(field[11]);
        session->newdata.dgps_station = atoi(field[12]);
    }
#if 0
    dprintf( "GNS: hhmmss=%s lat=%.2f lon=%.2f mode=%d status=%d\n",
             field[1],
             session->newdata.latitude,
             session->newdata.longitude,
             session->newdata.mode,
             session->gpsdata.status);
#endif			 
    return mask;
}

/* process xxVTG
 *     $GPVTG,054.7,T,034.4,M,005.5,N,010.2,K
 *     $GPVTG,054.7,T,034.4,M,005.5,N,010.2,K,A
 *
 * where:
 *         1,2     054.7,T      True track made good (degrees)
 *         3,4     034.4,M      Magnetic track made good
 *         5,6     005.5,N      Ground speed, knots
 *         7,8     010.2,K      Ground speed, Kilometers per hour
 *         9       A            Mode Indicator (optional)
 *                                see faa_mode() for possible mode values
 *
 * see also:
 * https://gpsd.gitlab.io/gpsd/NMEA.html#_vtg_track_made_good_and_ground_speed
 */
static gps_mask_t processVTG(int count,
                             char *field[],
                             struct gps_device_t *session)
{
    gps_mask_t mask = ONLINE_SET;

    if( (field[1][0] == '\0') || (field[5][0] == '\0')){
        return mask;
    }

    /* ignore empty/missing field, fix mode of last resort */
    if ((count > 9) && ('\0' != field[9][0])) {

        switch (field[9][0]) {
        case 'A':
            /* Autonomous, 2D or 3D fix */
            /* FALL THROUGH */
        case 'D':
            /* Differential, 2D or 3D fix */
            // MODE_SET here causes issues
            // mask |= MODE_SET;
            break;
        case 'E':
            /* Estimated, DR only */
            /* FALL THROUGH */
        case 'N':
            /* Not Valid */
            // MODE_SET here causes issues
            // mask |= MODE_SET;
            // nothing to use here, leave
            return mask;
        default:
            /* Huh? */
            break;
        }
    }

    // set true track
    session->newdata.track = safe_atof(field[1]);
    mask |= TRACK_SET;

    // set magnetic variation
    if (field[3][0] != '\0'){  // ignore empty fields
        //session->newdata.magnetic_track = safe_atof(field[3]);
        mask |= MAGNETIC_TRACK_SET;
    }

    session->newdata.speed = safe_atof(field[5]) * KNOTS_TO_MPS;
    mask |= SPEED_SET;
#if 0
    dprintf( "VTG: course(T)=%.2f, course(M)=%.2f, speed=%.2f",
             session->newdata.track, session->newdata.magnetic_track,
             session->newdata.speed);
#endif			 
    return mask;
}

static gps_mask_t processRMC(int count, char *field[],
                             struct gps_device_t *session)
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
     * SiRF chipsets don't return either Mode Indicator or magnetic variation.
     */
    gps_mask_t mask = ONLINE_SET;
    char status = field[2][0];
    int newstatus;

    switch (status) {
    default:
        /* missing */
        /* FALLTHROUGH */
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

        /*
         * This copes with GPSes like the Magellan EC-10X that *only* emit
         * GPRMC. In this case we set mode and status here so the client
         * code that relies on them won't mistakenly believe it has never
         * received a fix.
         */
        if (3 < session->gpsdata.satellites_used) {
            /* 4 sats used means 3D */
            session->newdata.mode = MODE_3D;
            mask |= MODE_SET;
        } else if (0 != isfinite(session->gpsdata.fix.altHAE) ||
                   0 != isfinite(session->gpsdata.fix.altMSL)) {
            /* we probably have at least a 3D fix */
            /* this handles old GPS that do not report 3D */
            session->newdata.mode = MODE_3D;
            mask |= MODE_SET;
        }
        session->gpsdata.status = newstatus;
    }
#if 0
    dprintf("RMC: ddmmyy=%s hhmmss=%s lat=%.2f lon=%.2f "
             "speed=%.2f track=%.2f mode=%d var=%.1f status=%d\n",
             field[9], field[1],
             session->newdata.latitude,
             session->newdata.longitude,
             session->newdata.speed,
             session->newdata.track,
             session->newdata.mode,
             session->newdata.magnetic_var,
             session->gpsdata.status);
#endif			 
    return mask;
}

static gps_mask_t processGSA(int count, char *field[],
                             struct gps_device_t *session)
/* GPS DOP and Active Satellites */
{
#define GSA_TALKER      field[0][1]
    /*
     * eg1. $GPGSA,A,3,,,,,,16,18,,22,24,,,3.6,2.1,2.2*3C
     * eg2. $GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*35
     * NMEA 4.10: $GNGSA,A,3,13,12,22,19,08,21,,,,,,,1.05,0.64,0.83,4*0B
     * 1    = Mode:
     *         M=Manual, forced to operate in 2D or 3D
     *         A=Automatic, 3D/2D
     * 2    = Mode:
     *         1=Fix not available,
     *         2=2D,
     *         3=3D
     * 3-14 = PRNs of satellites used in position fix (null for unused fields)
     * 15   = PDOP
     * 16   = HDOP
     * 17   = VDOP
     * 18   - NMEA 4.1+ GNSS System ID, u-blox extended
     *             1 = GPS L1C/A, L2CL, L2CM
     *             2 = GLONASS L1 OF, L2 OF
     *             3 = Galileo E1C, E1B, E5 bl, E5 bQ
     *             4 = BeiDou B1I D1, B1I D2, B2I D1, B2I D12
     *
     * Not all documentation specifies the number of PRN fields, it
     * may be variable.  Most doc that specifies says 12 PRNs.
     *
     * The Navior-24 CH-4701 outputs 30 fields, 24 PRNs!
     * GPGSA,A,3,27,23,13,07,25,,,,,,,,,,,,,,,,,,,,07.9,06.0,05.2
     *
     * The Skytraq S2525F8-BD-RTK output both GPGSA and BDGSA in the
     * same cycle:
     * $GPGSA,A,3,23,31,22,16,03,07,,,,,,,1.8,1.1,1.4*3E
     * $BDGSA,A,3,214,,,,,,,,,,,,1.8,1.1,1.4*18
     * These need to be combined like GPGSV and BDGSV
     *
     * Some GPS emit GNGSA.  So far we have not seen a GPS emit GNGSA
     * and then another flavor of xxGSA
     *
     * Some Skytraq will emit all GPS in one GNGSA, Then follow with
     * another GNGSA with the BeiDou birds.
     *
     * SEANEXX, SiRFstarIV, and others also do it twice in one cycle:
     * $GNGSA,A,3,31,26,21,,,,,,,,,,3.77,2.55,2.77*1A
     * $GNGSA,A,3,75,86,87,,,,,,,,,,3.77,2.55,2.77*1C
     * seems like the first is GNSS and the second GLONASS
     *
     * u-blox 9 outputs one per GNSS on each cycle.  Note the
     * extra last parameter which is NMEA gnssid:
     * $GNGSA,A,3,13,16,21,15,10,29,27,20,,,,,1.05,0.64,0.83,1*03
     * $GNGSA,A,3,82,66,81,,,,,,,,,,1.05,0.64,0.83,2*0C
     * $GNGSA,A,3,07,12,33,,,,,,,,,,1.05,0.64,0.83,3*0A
     * $GNGSA,A,3,13,12,22,19,08,21,,,,,,,1.05,0.64,0.83,4*0B
     * Also note the NMEA 4.0 GLONASS PRN (82) in an NMEA 4.1
     * sentence.
     */
    gps_mask_t mask = ONLINE_SET;
    char last_last_gsa_talker = session->nmea.last_gsa_talker;
    int nmea_gnssid = 0;
    int nmea_sigid = 0;
    int ubx_sigid = 0;

    /*
     * One chipset called the i.Trek M3 issues GPGSA lines that look like
     * this: "$GPGSA,A,1,,,,*32" when it has no fix.  This is broken
     * in at least two ways: it's got the wrong number of fields, and
     * it claims to be a valid sentence (A flag) when it isn't.
     * Alarmingly, it's possible this error may be generic to SiRFstarIII.
     */
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

        dprintf("xxGSA sets mode %d\n", session->newdata.mode);

        if (19 < count ) {
            dprintf("xxGSA: count %d too long!\n", count);
        } else {
            /* Just ignore the last fields of the Navior CH-4701 */
            if (field[15][0] != '\0')
                session->gpsdata.dop.pdop = safe_atof(field[15]);
            if (field[16][0] != '\0')
                session->gpsdata.dop.hdop = safe_atof(field[16]);
            if (field[17][0] != '\0')
                session->gpsdata.dop.vdop = safe_atof(field[17]);
            if (19 == count && '\0' != field[18][0]) {
                /* get the NMEA 4.10 sigid */
                nmea_sigid = atoi(field[18]);
                /* FIXME: ubx_sigid not used yet */
                ubx_sigid = nmea_sigid_to_ubx(nmea_sigid);
            }
        }
        /*
         * might have gone from GPGSA to GLGSA/BDGSA
         * or GNGSA to GNGSA
         * in which case accumulate
         */
        if ( '\0' == session->nmea.last_gsa_talker
          || (GSA_TALKER == session->nmea.last_gsa_talker
              && 'N' != GSA_TALKER) ) {
            session->gpsdata.satellites_used = 0;
            memset(session->nmea.sats_used, 0, sizeof(session->nmea.sats_used));
            dprintf("xxGSA: clear sats_used\n");
        }
        session->nmea.last_gsa_talker = GSA_TALKER;
        if ((session->nmea.last_gsa_talker == 'B') ||
            (session->nmea.last_gsa_talker == 'D') ||
            (session->nmea.last_gsa_talker == 'Q'))
            /* Quectel EC25 & EC21 use PQGSA for BeiDou */
            session->nmea.seen_bdgsa = true;
        else if (session->nmea.last_gsa_talker == 'L')
            session->nmea.seen_glgsa = true;
        else if (session->nmea.last_gsa_talker == 'N')
            session->nmea.seen_gngsa = true;
        else if (session->nmea.last_gsa_talker == 'A')
            session->nmea.seen_gagsa = true;

        /* the magic 6 here counts the tag, two mode fields, and DOP fields */
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

#if 0
            /* debug */
            GPSD_LOG(LOG_ERROR, &session->context->errout,
                     "%s nmeaid_to_prn: nmea_gnssid %d nmea_satnum %d "
                     "ubx_gnssid %d ubx_svid %d nmea2_prn %d\n",
                     field[0],
                     nmea_gnssid, nmea_satnum, ubx_gnssid, ubx_svid, prn);
            GPSD_LOG(LOG_ERROR, &session->context->errout,
                     "%s count %d\n", field[0], count);
#endif  /*  __UNUSED__ */

            if (prn > 0) {
                /* check first BEFORE over-writing memory */
                if (MAXCHANNELS <= session->gpsdata.satellites_used) {
                    /* This should never happen as xxGSA is limited to 12,
                     * except for the Navior-24 CH-4701.
                     * But it could happen with multiple GSA per cycle */
                    break;
                }
                session->nmea.sats_used[session->gpsdata.satellites_used++] =
                    (unsigned short)prn;
            }
        }
        mask |= DOP_SET | USED_IS;
        dprintf("xxGSA: mode=%d used=%d pdop=%.2f hdop=%.2f vdop=%.2f "
                 "ubx_sigid %d\n",
                 session->newdata.mode,
                 session->gpsdata.satellites_used,
                 session->gpsdata.dop.pdop,
                 session->gpsdata.dop.hdop,
                 session->gpsdata.dop.vdop, ubx_sigid);
    }
    /* assumes GLGSA or BDGSA, if present, is emitted  directly
     * after the GPGSA*/
    if ((session->nmea.seen_glgsa || session->nmea.seen_bdgsa ||
         session->nmea.seen_gagsa) && GSA_TALKER == 'P') {
        mask = ONLINE_SET;

    /* first of two GNGSA */
    /* if mode == 1 some GPS only output 1 GNGSA, so ship mode always */
    } else if ( 'N' != last_last_gsa_talker && 'N' == GSA_TALKER) {
        mask =  ONLINE_SET | MODE_SET;
    }

    /* cast for 32/64 compatibility */
 //   dprintf("xxGSA: mask %#llx\n", (long long unsigned)mask);
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
     * NMEA 4.1+:
     * $GAGSV,3,1,09,02,00,179,,04,09,321,,07,11,134,11,11,10,227,,7*7F
     * after the satellite block, before the checksum, new field:
     * 7           NMEA Signal ID
     *             1 = GPS L1C/A, BeiDou B1I D1, BeiDou B1I D2, GLONASS L1 OF
     *             2 = Galileo E5 bl, E5 bQ
     *             3 = BeiDou B2I D1, B2I D2
     *             5 = GPS L2 CM
     *             6 = GPS L2 CL
     *             7 = Galileo E1C, E1B
     *
     * Can occur with talker IDs:
     *   BD (Beidou),
     *   GA (Galileo),
     *   GB (Beidou),
     *   GL (GLONASS),
     *   GN (GLONASS, any combination GNSS),
     *   GP (GPS, SBAS, QZSS),
     *   QZ (QZSS).
     *
     * As of April 2019:
     *    no gpsd regressions have GNGSV
     *    every xxGSV cycle starts with GPGSV
     *    xxGSV cycles may be spread over several xxRMC cycles
     *
     * GL may be (incorrectly) used when GSVs are mixed containing
     * GLONASS, GN may be (incorrectly) used when GSVs contain GLONASS
     * only.  Usage is inconsistent.
     *
     * In the GLONASS version sat IDs run from 65-96 (NMEA0183
     * standardizes this). At least two GPSes, the BU-353 GLONASS and
     * the u-blox NEO-M8N, emit a GPGSV set followed by a GLGSV set.
     * We have also seen two GPSes, the Skytraq S2525F8-BD-RTK and a
     * SiRF-IV variant, that emit GPGSV followed by BDGSV. We need to
     * combine these.
     *
     * The following shows how the Skytraq S2525F8-BD-RTK output both
     * GPGSV and BDGSV in the same cycle:
     * $GPGSV,4,1,13,23,66,310,29,03,65,186,33,26,43,081,27,16,41,124,38*78
     * $GPGSV,4,2,13,51,37,160,38,04,37,066,25,09,34,291,07,22,26,156,37*77
     * $GPGSV,4,3,13,06,19,301,,31,17,052,20,193,11,307,,07,11,232,27*4F
     * $GPGSV,4,4,13,01,03,202,30*4A
     * $BDGSV,1,1,02,214,55,153,40,208,01,299,*67
     *
     * The driver automatically adapts to either case, but it takes until the
     * second cycle (usually 10 seconds from device connect) for it to
     * learn to expect BSDGV or GLGSV.
     *
     * Some GPS (Garmin 17N) spread the xxGSV over several cycles.  So
     * cycles, or cycle time, can not be used to determine start of
     * xxGSV cycle.
     *
     * NMEA 4.1 adds a signal-ID field just before the checksum. First
     * seen in May 2015 on a u-blox M8.  It can output 2 sets of GPGSV
     * in one cycle, one for L1C and the other for L2C.
     */

    int n, fldnum;
    unsigned char  nmea_sigid = 0;
    int nmea_gnssid = 0;
    unsigned char  ubx_sigid = 0;

    if (count <= 3) {
        dprintf("malformed xxGSV - fieldcount %d <= 3\n",
                 count);
        gpsd_zero_satellites(&session->gpsdata);
        return ONLINE_SET;
    }
	
	#if 0
    dprintf("x%cGSV: part %s of %s, last_gsv_talker '%#x' "
             " last_gsv_sigid %u\n",
             GSV_TALKER, field[2], field[1],
             session->nmea.last_gsv_talker,
             session->nmea.last_gsv_sigid);
	#endif
    /*
     * This check used to be !=0, but we have loosen it a little to let by
     * NMEA 4.1 GSVs with an extra signal-ID field at the end.
     */
    switch (count % 4) {
    case 0:
        /* normal, pre-NMEA 4.10 */
        break;
    case 1:
        /* NMEA 4.10, get the signal ID */
        nmea_sigid = atoi(field[count - 1]);
        ubx_sigid = nmea_sigid_to_ubx(nmea_sigid);
        break;
    default:
        /* bad count */
        dprintf("malformed GPGSV - fieldcount %d %% 4 != 0\n", count);
        gpsd_zero_satellites(&session->gpsdata);
        return ONLINE_SET;
    }

    session->nmea.await = atoi(field[1]);
    if ((session->nmea.part = atoi(field[2])) < 1) {
        dprintf("malformed GPGSV - bad part\n");
        gpsd_zero_satellites(&session->gpsdata);
        return ONLINE_SET;
    }

    if (session->nmea.part == 1) {
        /*
         * might have gone from GPGSV to GLGSV/BDGSV/QZGSV,
         * in which case accumulate
         *
         * NMEA 4.1 might have gone from GPGVS,sigid=x to GPGSV,sigid=y
         *
         * session->nmea.last_gsv_talker is zero at cycle start
         */
        if (session->nmea.last_gsv_talker == '\0' ||
            ('P' == GSV_TALKER &&
             0 == ubx_sigid)) {
            dprintf("x%cGSV: new part %d, last_gsv_talker '%#x', zeroing\n",
                     GSV_TALKER,
                     session->nmea.part,
                     session->nmea.last_gsv_talker);
            gpsd_zero_satellites(&session->gpsdata);
        }
        session->nmea.last_gsv_talker = GSV_TALKER;
        session->nmea.last_gsv_sigid = ubx_sigid; /* UNUSED */
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
            gpsd_zero_satellites(&session->gpsdata);
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

#if 0 
        {
        /* debug */
            char ts_buf[TIMESPEC_LEN];
            GPSD_LOG(LOG_ERROR, &session->context->errout,
                     "%s nmeaid_to_prn: nmea_gnssid %d nmea_satnum %d "
                     "ubx_gnssid %d ubx_svid %d nmea2_prn %d\n", field[0],
                     nmea_gnssid, nmea_svid, sp->gnssid, sp->svid, sp->PRN);
        }
#endif  /* __UNUSED__ */

        sp->elevation = (double)atoi(field[fldnum++]);
        sp->azimuth = (double)atoi(field[fldnum++]);
        sp->ss = (double)atoi(field[fldnum++]);
        sp->used = false;
        sp->sigid = ubx_sigid;

        /* sadly NMEA 4.1 does not tell us which sigid (L1, L2) is
         * used.  So if the ss is zero, do not mark used */
        if (0 < sp->PRN && 0 < sp->ss) {
            for (n = 0; n < MAXCHANNELS; n++)
                if (session->nmea.sats_used[n] == (unsigned short)sp->PRN) {
                    sp->used = true;
                    break;
                }
        }
        /*
         * Incrementing this unconditionally falls afoul of chipsets like
         * the Motorola Oncore GT+ that emit empty fields at the end of the
         * last sentence in a GPGSV set if the number of satellites is not
         * a multiple of 4.
         */
        session->gpsdata.satellites_visible++;
    }

#if __UNUSED
    /* debug code */
    GPSD_LOG(LOG_ERROR, &session->context->errout,
        "x%cGSV: vis %d gagsv %d bdgsv %d glgsv %d qzss %d\n",
        GSV_TALKER,
        session->gpsdata.satellites_visible,
        session->nmea.seen_gagsv,
        session->nmea.seen_bdgsv,
        session->nmea.seen_glgsv,
        session->nmea.seen_qzss);
#endif

    /*
     * Alas, we can't sanity check field counts when there are multiple sat
     * pictures, because the visible member counts *all* satellites - you
     * get a bad result on the second and later SV spans.  Note, this code
     * assumes that if any of the special sat pics occur they come right
     * after a stock GPGSV one.
     *
     * FIXME: Add per-talker totals so we can do this check properly.
     */
    if (!(session->nmea.seen_glgsv || session->nmea.seen_bdgsv
        || session->nmea.seen_qzss || session->nmea.seen_gagsv))
        if (session->nmea.part == session->nmea.await
                && atoi(field[3]) != session->gpsdata.satellites_visible)
            dprintf("GPGSV field 3 value of %d != actual count %d\n",
                     atoi(field[3]), session->gpsdata.satellites_visible);

    /* not valid data until we've seen a complete set of parts */
    if (session->nmea.part < session->nmea.await) {
        dprintf("xxGSV: Partial satellite data (%d of %d).\n",
                 session->nmea.part, session->nmea.await);
        return ONLINE_SET;
    }
    /*
     * This sanity check catches an odd behavior of SiRFstarII receivers.
     * When they can't see any satellites at all (like, inside a
     * building) they sometimes cough up a hairball in the form of a
     * GSV packet with all the azimuth entries 0 (but nonzero
     * elevations).  This behavior was observed under SiRF firmware
     * revision 231.000.000_A2.
     */
    for (n = 0; n < session->gpsdata.satellites_visible; n++)
        if (session->gpsdata.skyview[n].azimuth != 0)
            goto sane;
    dprintf("xxGSV: Satellite data no good (%d of %d).\n",
             session->nmea.part, session->nmea.await);
    gpsd_zero_satellites(&session->gpsdata);

    return ONLINE_SET;
sane:
    session->gpsdata.skyview_time.tv_sec = 0;
    session->gpsdata.skyview_time.tv_nsec = 0;
    dprintf("xxGSV: Satellite data OK (%d of %d).\n",
             session->nmea.part, session->nmea.await);

    /* assumes GLGSV or BDGSV group, if present, is emitted after the GPGSV */
    if ((session->nmea.seen_glgsv || session->nmea.seen_bdgsv
         || session->nmea.seen_qzss  || session->nmea.seen_gagsv)
        && GSV_TALKER == 'P')
        return ONLINE_SET;

#if 0
    /* debug code */
    GPSD_LOG(LOG_ERROR, &session->context->errout,
        "x%cGSV: set skyview_time %s frac_time %.2f\n", GSV_TALKER,
         timespec_str(&session->gpsdata.skyview_time, ts_buf, sizeof(ts_buf)),
        session->nmea.this_frac_time);
#endif

    /* clear computed DOPs so they get recomputed. */
    /* FIXME: this kills GPS reported dops... */
    session->gpsdata.dop.xdop = NAN;
    session->gpsdata.dop.ydop = NAN;
    session->gpsdata.dop.tdop = NAN;
    session->gpsdata.dop.gdop = NAN;
    return SATELLITE_SET;
#undef GSV_TALKER
}

gps_mask_t nmea_parse(char *sentence, struct gps_device_t * session)
/* parse an NMEA sentence, unpack it into a session structure */
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
        {"GGA", 13, false, processGGA},
        {"GLL", 7,  false, processGLL},
        {"GNS", 13, false, processGNS},
        {"GSA", 18, false, processGSA},
//        {"GST", 8,  false, processGST},
        {"GSV", 0,  false, processGSV},
        {"RMC", 8,  false, processRMC},
        {"VTG", 5,  false, processVTG},
    };

    int count;
    gps_mask_t mask = 0;
    unsigned int i, thistag, lasttag;
    char *p, *e;
    volatile char *t;
    uint64_t lasttag_mask = 0;
    uint64_t thistag_mask = 0;
    char ts_buf1[TIMESPEC_LEN];
    char ts_buf2[TIMESPEC_LEN];

    /*
     * We've had reports that on the Garmin GPS-10 the device sometimes
     * (1:1000 or so) sends garbage packets that have a valid checksum
     * but are like 2 successive NMEA packets merged together in one
     * with some fields lost.  Usually these are much longer than the
     * legal limit for NMEA, so we can cope by just tossing out overlong
     * packets.  This may be a generic bug of all Garmin chipsets.
     */
    if (strlen(sentence) > NMEA_MAX) {
        dprintf("Overlong packet of %zd chars rejected.\n",
                 strlen(sentence));
        return ONLINE_SET;
    }

    /* make an editable copy of the sentence */
    (void)strlcpy((char *)session->nmea.fieldcopy, sentence,
                  sizeof(session->nmea.fieldcopy) - 1);
    
	/* discard the checksum part */
    for (p = (char *)session->nmea.fieldcopy; (*p != '*') && (*p >= ' ');)
        ++p;
    if (*p == '*')
        *p++ = ',';             /* otherwise we drop the last field */
    
	*p = '\0';
    e = p;

    /* split sentence copy on commas, filling the field array */
    count = 0;
    t = p;                      /* end of sentence */
    p = (char *)session->nmea.fieldcopy + 1; /* beginning of tag, 'G' not '$' */
    /* while there is a search string and we haven't run off the buffer... */
    while ((p != NULL) && (p <= t)) {
        session->nmea.field[count] = p; /* we have a field. record it */
        if ((p = strchr(p, ',')) != NULL) {  /* search for the next delimiter */
            *p = '\0';          /* replace it with a NUL */
            count++;            /* bump the counters and continue */
            p++;
        }
    }

    /* point remaining fields at empty string, just in case */
    for (i = (unsigned int)count;
         i <
         (unsigned)(sizeof(session->nmea.field) /
                    sizeof(session->nmea.field[0])); i++)
        session->nmea.field[i] = e;

    /* sentences handlers will tell us when they have fractional time */
    session->nmea.latch_frac_time = false;

    /* dispatch on field zero, the sentence tag */
    for (thistag = i = 0;
         i < (unsigned)(sizeof(nmea_phrase) / sizeof(nmea_phrase[0])); ++i) {
        char *s = session->nmea.field[0];
        if (strlen(nmea_phrase[i].name) == 3
                ) {
            s += 2;             /* skip talker ID */
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
            mask = (nmea_phrase[i].decoder)(count, session->nmea.field,
                                            session);
            if (nmea_phrase[i].cycle_continue)
                session->nmea.cycle_continue = true;
            
			thistag = i + 1;
            break;
        }
    }

    /* prevent overaccumulation of sat reports */
    if (!str_starts_with(session->nmea.field[0] + 2, "GSV"))
        session->nmea.last_gsv_talker = '\0';
    
	if (!str_starts_with(session->nmea.field[0] + 2, "GSA"))
        session->nmea.last_gsa_talker = '\0';

    if ((mask & TIME_SET) != 0) {
        session->newdata.time = gpsd_utc_resolve(session);

        dprintf("%s time is %s = %d-%02d-%02dT%02d:%02d:%02d.%03ldZ\n",
                 session->nmea.field[0],
                 timespec_str(&session->newdata.time, ts_buf1, sizeof(ts_buf1)),
                 1900 + session->nmea.date.tm_year,
                 session->nmea.date.tm_mon + 1,
                 session->nmea.date.tm_mday,
                 session->nmea.date.tm_hour,
                 session->nmea.date.tm_min,
                 session->nmea.date.tm_sec,
                 session->nmea.subseconds.tv_nsec / 1000000L);
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
        TS_SUB(&ts_delta, &session->nmea.this_frac_time,
                          &session->nmea.last_frac_time);
        if (0.01 < fabs(TSTONS(&ts_delta))) {
            /* time changed */
            mask |= CLEAR_IS;
            //dprintf("%s starts a reporting cycle. lasttag %d\n",
            //         session->nmea.field[0], lasttag);
            /*
             * Have we seen a previously timestamped NMEA tag?
             * If so, designate as end-of-cycle marker.
             * But not if there are continuation sentences;
             * those get sorted after the last timestamped sentence
             *
             */
            if (0 < lasttag &&
                0 == (session->nmea.cycle_enders & lasttag_mask) &&
                !session->nmea.cycle_continue) {
                session->nmea.cycle_enders |= lasttag_mask;
                /* (long long) cast for 32/64 bit compat */
                dprintf("tagged %s as a cycle ender. %#llx\n",
                         nmea_phrase[lasttag - 1].name,
                         (unsigned long long)lasttag_mask);
            }
        }
    } else {
        /* extend the cycle to an un-timestamped sentence? */
        if (0 != (session->nmea.cycle_enders & lasttag_mask)) {
            dprintf("%s is just after a cycle ender.\n",
                     session->nmea.field[0]);
        }
        if (session->nmea.cycle_continue) {
            dprintf("%s extends the reporting cycle.\n",
                     session->nmea.field[0]);
            /* change ender */
            session->nmea.cycle_enders &= ~lasttag_mask;
            session->nmea.cycle_enders |= thistag_mask;
        }
    }

    /* here's where we check for end-of-cycle */
    if ((session->nmea.latch_frac_time || session->nmea.cycle_continue)
        && (session->nmea.cycle_enders & thistag_mask)!=0) {
       // dprintf("%s ends a reporting cycle.\n", session->nmea.field[0]);
        mask |= REPORT_IS;
    }
    
	if (session->nmea.latch_frac_time)
        session->nmea.lasttag = thistag;

    /* we might have a (somewhat) reliable end-of-cycle */
    if (session->nmea.cycle_enders != 0)
        session->cycle_end_reliable = true;

    /* don't downgrade mode if holding previous fix */
    /* usually because of xxRMC which does not report 2D/3D */
    if (MODE_SET == (mask & MODE_SET) &&
        MODE_3D == session->gpsdata.fix.mode &&
        MODE_NO_FIX != session->newdata.mode &&
        (0 != isfinite(session->lastfix.altHAE) ||
         0 != isfinite(session->oldfix.altHAE) ||
         0 != isfinite(session->lastfix.altMSL) ||
         0 != isfinite(session->oldfix.altMSL))) {
        session->newdata.mode = session->gpsdata.fix.mode;
		dprintf("-------------------------------\n");
    }
	
    return mask;
}

gps_mask_t generic_parse_input(struct gps_device_t *session)
{
    if (session->lexer.type == BAD_PACKET)
		return 0;
    else if (session->lexer.type == COMMENT_PACKET) {
		gpsd_set_century(session);
		return 0;
    } else if (session->lexer.type == NMEA_PACKET) {
		gps_mask_t st = 0;
		char *sentence = (char *)session->lexer.outbuffer;
		
		#if 0
		if (sentence[strlen(sentence)-1] != '\n')
			dprintf("<= GPS: %s\n", sentence);
		else
			dprintf("<= GPS: %s", sentence);
		#endif	
		if ((st=nmea_parse(sentence, session)) == 0) {
			dprintf("unknown sentence: %s\n", sentence);
		}
		
		return st;
    } else {
		dprintf("packet type %d fell through (should never happen):\n",	session->lexer.type);
		return 0;
    }
}

gps_mask_t gpsd_poll(struct gps_device_t *session)
/* update the stuff in the scoreboard structure */
{
    ssize_t newlen;
    bool driver_change = false;
    timespec_t ts_now;
    timespec_t delta;
    char ts_buf[TIMESPEC_LEN];

    gps_clear_fix(&session->newdata);

#define MINIMUM_QUIET_TIME	0.25
    if (session->lexer.outbuflen == 0) {
		/* beginning of a new packet */
		(void)clock_gettime(CLOCK_REALTIME, &ts_now);
		
		if ((0 < session->lexer.start_time.tv_sec || 0 < session->lexer.start_time.tv_nsec)) 
		{
			const double min_cycle = 1;
			double quiet_time = (MINIMUM_QUIET_TIME * min_cycle);
			double gap;

			gap = TS_SUB_D(&ts_now, &session->lexer.start_time);

			if (gap > min_cycle)
				dprintf("cycle-start detector failed.\n");
			else if (gap > quiet_time) {
				dprintf("transmission pause of %f\n", gap);
				session->sor = ts_now;
				session->lexer.start_char = session->lexer.char_counter;
			}
		}
		session->lexer.start_time = ts_now;
    }

    if (session->lexer.type >= COMMENT_PACKET) {
		session->observed |= PACKET_TYPEMASK(session->lexer.type);
    }

    /* can we get a full packet from the device? */
	newlen = generic_get(session);

    /* update the scoreboard structure from the GPS */
    //dprintf("%zd new characters\n", newlen);

    (void)clock_gettime(CLOCK_REALTIME, &ts_now);
    TS_SUB(&delta, &ts_now, &session->gpsdata.online);
    
	if (newlen < 0) {		/* read error */
		eprintf("GPS returned error %zd (%s sec since data)\n", newlen,
                 timespec_str(&delta, ts_buf, sizeof(ts_buf)));
		session->gpsdata.online.tv_sec = 0;
		session->gpsdata.online.tv_nsec = 0;
		return ERROR_SET;
    } else if (newlen == 0) {		/* zero length read, possible EOF */
		//dprintf("GPS is offline (%s sec since data)\n", timespec_str(&delta, ts_buf, sizeof(ts_buf)));
		session->gpsdata.online.tv_sec = 0;
		session->gpsdata.online.tv_nsec = 0;
		return NODATA_IS;
    } else /* (newlen > 0) */ {
		//dprintf("packet sniff finds type %d\n", session->lexer.type);
		if (session->lexer.type == COMMENT_PACKET) {
			if (strcmp((const char *)session->lexer.outbuffer, "# EOF\n") == 0) {
				dprintf("synthetic EOF\n");
				return EOF_IS;
			} else
				dprintf("comment, sync lock deferred\n");
			/* FALL THROUGH */
		} else if (session->lexer.type > COMMENT_PACKET) {
			session->badcount = 0;
		} 
    }

    if (session->lexer.outbuflen == 0) {      /* got new data, but no packet */
		//dprintf("New data, not yet a packet\n");
		return ONLINE_SET;
    } else {			/* we have recognized a packet */
		gps_mask_t received = PACKET_SET;
		(void)clock_gettime(CLOCK_REALTIME, &session->gpsdata.online);
	    session->lexer.counter++;

		/* Get data from current packet into the fix structure */
		if (session->lexer.type != COMMENT_PACKET) {
			received |= generic_parse_input(session);
		}
		
		/* are we going to generate a report? if so, count characters */
		if ((received & REPORT_IS) != 0) {
			session->chars = session->lexer.char_counter - session->lexer.start_char;
			dprintf("GPS REPORT_IS\n");
		}

		session->gpsdata.set = ONLINE_SET | received;

		if ((received & SATELLITE_SET) != 0
			&& session->gpsdata.satellites_visible > 0) {
			session->gpsdata.set |= fill_dop(&session->gpsdata,
							&session->gpsdata.dop);
			dprintf("GPS SATELLITE_SET\n");				
		}

		/* copy/merge device data into staging buffers */
		if ((session->gpsdata.set & CLEAR_IS) != 0) {
			/* CLEAR_IS should only be set on first sentence of cycle */
			gps_clear_fix(&session->gpsdata.fix);
			dprintf("GPS CLEAR_IS\n");
		}

		gps_merge_fix(&session->gpsdata.fix, session->gpsdata.set, &session->newdata);
		
		/* 시스템 시간이 GPS 시간보다 1년 이상 이전 시간인지 확인하기 위함. */	
		if ((session->gpsdata.set & TIME_SET) != 0) {
			dprintf("GPS TIME_SET\n");
			if (session->newdata.time.tv_sec >
					(time(NULL) + (60 * 60 * 24 * 365))) {
				dprintf("date (%lld) more than a year in the future!\n",
					(long long)session->newdata.time.tv_sec);
			} 
			else if (session->newdata.time.tv_sec < 0) {
				dprintf("date (%lld) is negative!\n",
					(long long)session->newdata.time.tv_sec);
			}
		}

		return session->gpsdata.set;
    }
	
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------
void gpsd_init(struct gps_device_t *session, struct gps_context_t *context,
	       const char *device)
{
    session->observed = 0;
    session->context = context;
    
	memset(session->subtype, 0, sizeof(session->subtype));
    memset(session->subtype1, 0, sizeof(session->subtype1));
    memset(&(session->nmea), 0, sizeof(session->nmea));
    
	gps_clear_fix(&session->gpsdata.fix);
    gps_clear_fix(&session->newdata);
    gps_clear_fix(&session->lastfix);
    gps_clear_fix(&session->oldfix);
    session->gpsdata.set = 0;
    
    gps_clear_dop(&session->gpsdata.dop);
    
    session->sor.tv_sec = 0;
    session->sor.tv_nsec = 0;
    session->chars = 0;
	
    /* necessary in case we start reading in the middle of a GPGSV sequence */
    gpsd_zero_satellites(&session->gpsdata);

    /* initialize things for the packet parser */
    packet_reset(&session->lexer);
}

void gpsd_clear(struct gps_device_t *session)
{
    (void)clock_gettime(CLOCK_REALTIME, &session->gpsdata.online);
    lexer_init(&session->lexer);
    
    gps_clear_dop(&session->gpsdata.dop);
    gps_clear_fix(&session->gpsdata.fix);
	
    session->gpsdata.status = STATUS_NO_FIX;
    session->releasetime = (time_t)0;
    session->badcount = 0;
    session->opentime = time(NULL);
}

void gpsd_time_init(struct gps_context_t *context, time_t starttime)
{
    context->leap_seconds = BUILD_LEAPSECONDS;
    context->century = BUILD_CENTURY;
    context->start_time = starttime;
    context->rollovers = (int)((context->start_time-GPS_EPOCH) / GPS_ROLLOVER);

    if (GPS_EPOCH > context->start_time) {
		dprintf("system time looks bogus, dates may not be reliable.\n");
    } else {
		struct tm *now = localtime(&context->start_time);
		char scr[128];
        timespec_t ts_start_time;

        ts_start_time.tv_sec = context->start_time;
        ts_start_time.tv_nsec = 0;

		now->tm_year += 1900;
		context->century = now->tm_year - (now->tm_year % 100);
		dprintf("startup at %s (%ld)\n", timespec_to_iso8601(ts_start_time, scr, sizeof(scr)),
			(long)context->start_time);
    }
}

void gpsd_set_century(struct gps_device_t *session)
{
    char *end;
	
    if (strstr((char *)session->lexer.outbuffer, "Date:") != NULL) {
		int year;
		unsigned char *cp = session->lexer.outbuffer + 5;
		while (isspace(*cp))
	    	++cp;
		year = (int)strtol((char *)cp, &end, 10);
		session->context->century = year - (year % 100);
    }
}

timespec_t gpsd_utc_resolve(struct gps_device_t *session)
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
