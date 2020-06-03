/*
 * dev_gps.h
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
 * GPS Hardware library interface definitions.
 */

#ifndef _DEV_GPS_H_
#define _DEV_GPS_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <time.h>

#define GPS_ERR_STATE			-2
#define GPS_OFFLINE				-1
#define GPS_ONLINE				0

#define STATUS_NO_FIX			0 /* received 'V' */
#define STATUS_FIX				1 /* received 'A' */

#define G_ONLINE_SET			(0x000000001u)
#define G_TIME_SET				(0x000000002u)
#define G_PSTIMERR_SET			(0x000000004u)
#define G_LATLON_SET			(0x000000008u)
#define G_ALTITUDE_SET			(0x000000010u)
#define G_SPEED_SET				(0x000000020u)
#define G_TRACK_SET				(0x000000040u)
#define G_CLIMB_SET				(0x000000080u)
#define G_STATUS_SET			(0x000000100u)
#define G_MODE_SET				(0x000000200u)
#define G_DOP_SET  				(0x000000400u)
#define G_HERR_SET				(0x000000800u)
#define G_VERR_SET				(0x000001000u)
#define G_ATTITUDE_SET			(0x000002000u)
#define G_SATELLITE_SET			(0x000004000u)

#define MAXCHANNELS				72	/* must be > 12 GPS + 12 GLONASS + 2 WAAS */
#define GPS_PRNMAX				32	/* above this number are SBAS satellites */

#define MODE_NOT_SEEN			0	/* mode update not seen yet */
#define MODE_NO_FIX				1	/* none */
#define MODE_2D  				2	/* good for latitude/longitude */
#define MODE_3D  				3	/* good for altitude/climb too */

/* Unix time in seconds with fractional part */
typedef double timestamp_t;

struct dop_t {
    double xdop;
    double ydop;
    double pdop;
    double hdop;
    double vdop;
    double tdop;
    double gdop;
};

typedef struct {
    int part;
    int await;				/* for tracking GSV parts */

    struct tm date;			/* date part of last sentence time */
    double subseconds;		/* subsec part of last sentence time */

    unsigned char latch_mode;
    timestamp_t this_frac_time;
	timestamp_t last_frac_time;

	unsigned char latch_frac_time;

    double latitude;
    double longitude;
    double altitude;
    double track;
    double speed; /* for km/h */
} gps_nmea_t;

struct gps_data_t {
	unsigned long set; 			/* if tag value set, */
	int rmc_status;
	int mode;					/* Mode of fix */

	gps_nmea_t nmea;

	struct dop_t dop;

    int satellites_used;	/* Number of satellites used in solution */
    int used[MAXCHANNELS];	/* PRNs of satellites used in solution */

    int satellites_visible;	/* # of satellites in view */
    int PRN[MAXCHANNELS];	/* PRNs of satellite */
    int elevation[MAXCHANNELS];	/* elevation of satellite */
    int azimuth[MAXCHANNELS];	/* azimuth */

    int ss[MAXCHANNELS];	/* signal-to-noise ratio (dB) */
};

int dev_gps_init(void);
int dev_gps_get_data(struct gps_data_t *gdata);
void dev_gps_close(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _DEV_GPS_H_ */
