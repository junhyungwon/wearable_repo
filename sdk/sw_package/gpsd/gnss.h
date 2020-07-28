/*
 * File : agps.h
 *
 * Copyright (C) 2020 Texas Instruments
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
 */

#ifndef __AGPS_H__
#define __AGPS_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <time.h>
#include <stdint.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#ifndef bool
typedef int bool;
#define true 1
#define false 0
#endif

#define TIMESPEC_LEN						22	/* required length of a timespec buffer */

#ifndef NAN
#define NAN 								(0.0f/0.0f)
#endif

#define MONTHSPERYEAR						12	/* months per calendar year */
#define BUILD_CENTURY 						2000
#define BUILD_LEAPSECONDS 					18
#define GPS_EPOCH							((time_t)315964800)   /* 6 Jan 1980 00:00:00 UTC */
#define NMEA_MAX							102	/* max length of NMEA sentence */
#define MAX_PACKET_LENGTH					9216		/* 4 + 16 + (256 * 32) + 2 + fudge */
#define BAD_PACKET      					-1
#define packet_buffered_input(lexer) 		((lexer)->inbuffer + (lexer)->inbuflen - (lexer)->inbufptr)
#define KNOTS_TO_MPS						0.51444444	/* Knots to meters per second */
#define MAXCHANNELS							140	/* u-blox 9 tracks 140 signals */
#define MAXUSERDEVS							4	/* max devices per user */
#define GPS_PATH_MAX						128	/* for names like /dev/serial/by-id/... */
#define GPS_PI      	3.1415926535897932384626433832795029
#define RAD_2_DEG	57.2957795130823208767981548141051703
#define DEG_2_RAD	0.0174532925199432957692369076848861271
/* time constant */
#define SECS_PER_DAY	((time_t)(60*60*24))  /* seconds per day */
#define SECS_PER_WEEK	(7*SECS_PER_DAY)        /* seconds per week */
#define GPS_ROLLOVER	(1024*SECS_PER_WEEK)    /* rollover period */

typedef unsigned long long gps_mask_t; /* for return status */
typedef struct timespec timespec_t;			/* Unix time as sec, nsec */
typedef int clockid_t;

#define INTERNAL_SET(n)	((gps_mask_t)(1llu<<(SET_HIGH_BIT+(n))))
#define RAW_IS  	INTERNAL_SET(1) 	/* raw pseudoranges available */
#define USED_IS 	INTERNAL_SET(2) 	/* sat-used count available */
#define DRIVER_IS	INTERNAL_SET(3) 	/* driver type identified */
#define CLEAR_IS	INTERNAL_SET(4) 	/* starts a reporting cycle */
#define REPORT_IS	INTERNAL_SET(5) 	/* ends a reporting cycle */
#define NODATA_IS	INTERNAL_SET(6) 	/* no data read from fd */
#define NTPTIME_IS	INTERNAL_SET(7) 	/* precision time is available */
#define PERR_IS 	INTERNAL_SET(8) 	/* PDOP set */
#define PASSTHROUGH_IS 	INTERNAL_SET(9) 	/* passthrough mode */
#define EOF_IS		INTERNAL_SET(10)	/* synthetic EOF */
#define GOODTIME_IS	INTERNAL_SET(11) 	/* time good even if no pos fix */
#define DATA_IS	~(ONLINE_SET|PACKET_SET|CLEAR_IS|REPORT_IS)

#define TS_GEZ(ts) (0 <= (ts)->tv_sec && 0 <= (ts)->tv_nsec)
#define TSTONS(ts) ((double)((ts)->tv_sec + ((ts)->tv_nsec / 1e9)))

#define NS_IN_SEC	1000000000LL     /* nanoseconds in a second */
#define US_IN_SEC	1000000LL        /* microseconds in a second */
#define MS_IN_SEC	1000LL           /* milliseconds in a second */

struct timedelta_t {
    timespec_t	real;
    timespec_t	clock;
};

struct dop_t {
    /* Dilution of precision factors */
    double xdop, ydop, pdop, hdop, vdop, tdop, gdop;
};

struct satellite_t {
    /* SNR. signal-to-noise ratio, 0 to 254 dB, u-blox can be 0 to 63.
     * -1 for n/a */
    double ss;
    bool used;		/* this satellite used in solution */
    /* PRN of this satellite, 1 to 437, 0 for n/a
     * sadly there is no standard, but many different implementations of
     * how to code PRN
     */
    short PRN;          /* PRN numbering per NMEA 2.x to 4.0, not 4.10 */
    double elevation;	/* elevation of satellite, -90 to 90 deg, NAN for n/a */
    double azimuth;	/* azimuth, 0 to 359 deg, NAN1 for n/a */
    /* gnssid:svid:sigid, as defined by u-blox 8/9:
     *  gnssid        svid (native PRN)
     *  0 = GPS       1-32
     *  1 = SBAS      120-158
     *  2 = Galileo   1-36
     *  3 - BeiDou    1-37
     *  4 = IMES      1-10
     *  5 = QZSS      1-5       Undocumented u-blox goes to 7
     *  6 = GLONASS   1-32, 255
     *  x = IRNSS    1-11       Not defined by u-blox:
     *
     * gnssid:svid:sigid, as defined by NMEA 4.10, NOT USED HERE!
     *  1 = GPS       1-32
     *  1 = SBAS      33-64, 152-158
     *  1 = QZSS      193-197  Undocuemtned u-blox goes to 199
     *  2 = GLONASS   1-32, nul
     *  3 = Galileo   1-36
     *  4 - BeiDou    1-37
     *  x = IMES                Not defined by NMEA 4.10
     *
     * Note: other GNSS receivers use different mappings!
     */
    unsigned char gnssid;
/* defines for u-blox gnssId, as used in satellite_t */
#define GNSSID_GPS 0
#define GNSSID_SBAS 1
#define GNSSID_GAL 2
#define GNSSID_BD 3
#define GNSSID_IMES 4
#define GNSSID_QZSS 5
#define GNSSID_GLO 6
#define GNSSID_IRNSS 7            /* Not defined by u-blox */
#define GNSSID_CNT 8              /* count for array size */

    /* ignore gnssid and sigid if svid is zero */
    unsigned char svid;
    /* sigid as defined by u-blox 9, and used here
     * GPS:      0 = L1C/A, 3 = L2 CL, 4 = L2 CM
     * SBAS:     0 = L1C/A, ? = L5I
     * Galileo:  0 = E1 C,  1 = E1 B,  5 = E5 bl, 6 = E5 bQ
     * BeiDou:   0 = B1I D1, 1 = B1I D2, 2 = B2I D1, 3 = B2I D2
     * QZSS:     0 = L1C/A, 4 = L2 CM, 5 = L2 CL
     * GLONASS:  0 = L1 OF, 2 = L2 OF
     *
     * sigid as defined by NMEA 4.10, NOT used here
     * GPS:      1 = L1C/A, 6 = L2 CL, 5 = L2 CM
     * Galileo:  7 = E1 C,  7 = E1 B,  2 = E5 bl, 2 = E5 bQ
     * BeiDou:   1 = B1|D1, 1 = B1|D2, 3 = B2|D1, 3 = B2|D2
     * QZSS:     not defined
     * GLONASS:  1 = L1 OF, 3 = L2 OF
     */
    unsigned char sigid;
    signed char freqid;         /* The GLONASS (Only) frequency, 0 - 13 */
    unsigned char health;       /* 0 = unknown, 1 = healthy, 2 = unhealthy */
#define SAT_HEALTH_UNK 0
#define SAT_HEALTH_OK 1
#define SAT_HEALTH_BAD 2
};

struct gps_fix_t {
    timespec_t time;	/* Time of update */
    int    mode;	/* Mode of fix */
#define MODE_NOT_SEEN	0	/* mode update not seen yet */
#define MODE_NO_FIX	1	/* none */
#define MODE_2D  	2	/* good for latitude/longitude */
#define MODE_3D  	3	/* good for altitude/climb too */
    double ept;		/* Expected time uncertainty, seconds */
    double latitude;	/* Latitude in degrees (valid if mode >= 2) */
    double epy;  	/* Latitude position uncertainty, meters */
    double longitude;	/* Longitude in degrees (valid if mode >= 2) */
    double epx;  	/* Longitude position uncertainty, meters */
    double altitude;    // DEPRECATED, undefined.
    double altHAE;	/* Altitude, height above ellipsoid.
                         * in meters and probably WGS84
                         * (valid if mode == 3)
                         * MSL = altHAE - geoid_sep */
    double altMSL;      /* Altitude MSL (maybe EGM2008) */
    double epv;  	/* Vertical position uncertainty, meters */
    double track;	/* Course made good (relative to true north) */
    double epd;		/* Track uncertainty, degrees */
    double speed;	/* Speed over ground, meters/sec */
    double eps;		/* Speed uncertainty, meters/sec */
    double climb;       /* Vertical speed, meters/sec */
    double epc;		/* Vertical speed uncertainty */
    /* estimated position error horizontal (2D). Meters, maybe 50%, maybe 95% */
    /* aka estimated position error (epe) */
    double eph;		/* estimated position error horizontal (2D) */
    /* spherical error probability, 3D. Meters, maybe 50%, maybe 95% */
    /* Garmin, not gpsd, calls this estimated position error (epe) */
    double sep;
    /* Geoid separation (ellipsoid separation)
     * Height of MSL ellipsoid (geoid) above WGS84 ellipsoid.
     * Postive is MSL above WGS84. In meters */
    double geoid_sep;

    double magnetic_track;  /* Course (relative to Magnetic North) */
    double magnetic_var;    /* magnetic variation in degrees */
    double depth;           /* depth in meters, probably depth of water
                             * under the keel */

    /* ECEF data, all data in meters, and meters/second, or NaN */
    struct {
		double x, y, z; 	/* ECEF x, y, z */
		double pAcc;            /* 3D Position Accuracy Estimate, likely SEP */
		double vx, vy, vz;	/* ECEF x, y, z velocity */
		double vAcc;            /* Velocity Accuracy Estimate, probably SEP */
    } ecef;
    /* NED data, all data in meters, and meters/second, or NaN */
    struct {
        double relPosN, relPosE, relPosD;   /* NED relative positions */
        double velN, velE, velD;            /* NED velocities */
    } NED;
    char datum[40];             /* map datum */
    /* DGPS stuff, often from xxGGA, or xxGNS */
    double dgps_age;            /* age of DGPS data in tenths of seconds,
                                 * -1 invalid */
    /* DGPS Station used, max size is a guess
     * NMEA 2 says 0000-1023
     * RTCM 3, station ID is 0 to 4095.
     * u-blox UBX-NAV-DGPS is 16 bit integer */
    int dgps_station;           /* DGPS station ID, -1 invalid */
};

struct gps_data_t {
    gps_mask_t set;	/* has field been set since this was last cleared? */
#define ONLINE_SET	(1llu<<1)
#define TIME_SET	(1llu<<2)
#define TIMERR_SET	(1llu<<3)
#define LATLON_SET	(1llu<<4)
#define ALTITUDE_SET	(1llu<<5)
#define SPEED_SET	(1llu<<6)
#define TRACK_SET	(1llu<<7)
#define CLIMB_SET	(1llu<<8)
#define STATUS_SET	(1llu<<9)
#define MODE_SET	(1llu<<10)
#define DOP_SET  	(1llu<<11)
#define HERR_SET	(1llu<<12)
#define VERR_SET	(1llu<<13)
#define ATTITUDE_SET	(1llu<<14)
#define SATELLITE_SET	(1llu<<15)
#define SPEEDERR_SET	(1llu<<16)
#define TRACKERR_SET	(1llu<<17)
#define CLIMBERR_SET	(1llu<<18)
#define DEVICE_SET	(1llu<<19)
#define DEVICELIST_SET	(1llu<<20)
#define DEVICEID_SET	(1llu<<21)
#define RTCM2_SET	(1llu<<22)
#define RTCM3_SET	(1llu<<23)
#define AIS_SET 	(1llu<<24)
#define PACKET_SET	(1llu<<25)
#define SUBFRAME_SET	(1llu<<26)
#define GST_SET 	(1llu<<27)
#define VERSION_SET	(1llu<<28)
#define POLICY_SET	(1llu<<29)
#define LOGMESSAGE_SET	(1llu<<30)
#define ERROR_SET	(1llu<<31)
#define TOFF_SET	(1llu<<32)	/* not yet used */
#define PPS_SET 	(1llu<<33)
#define NAVDATA_SET     (1llu<<34)
#define OSCILLATOR_SET	(1llu<<35)
#define ECEF_SET	(1llu<<36)
#define VECEF_SET	(1llu<<37)
#define MAGNETIC_TRACK_SET (1llu<<38)
#define RAW_SET         (1llu<<39)
#define NED_SET         (1llu<<40)
#define VNED_SET        (1llu<<41)
#define SET_HIGH_BIT	42
    timespec_t online;		/* NZ if GPS is on line, 0 if not.
				 *
				 * Note: gpsd clears this time when sentences
				 * fail to show up within the GPS's normal
				 * send cycle time. If the host-to-GPS
				 * link is lossy enough to drop entire
				 * sentences, this field will be
				 * prone to false zero values.
				 */

    struct gps_fix_t	fix;	/* accumulated PVT data */

    /* GPS status -- always valid */
    int    status;		/* Do we have a fix? */
#define STATUS_NO_FIX	0	/* no */
/* yes, plain GPS (SPS Mode), without DGPS, PPS, RTK, DR, etc. */
#define STATUS_FIX	1
#define STATUS_DGPS_FIX	2	/* yes, with DGPS */
#define STATUS_RTK_FIX	3	/* yes, with RTK Fixed */
#define STATUS_RTK_FLT	4	/* yes, with RTK Float */
#define STATUS_DR	5	/* yes, with dead reckoning */
#define STATUS_GNSSDR	6	/* yes, with GNSS + dead reckoning */
#define STATUS_TIME	7	/* yes, time only (surveyed in, manual) */
#define STATUS_SIM	8	/* yes, simulated */
/* yes, Precise Positioning Service (PPS)
 * Not to be confused with Pulse per Second (PPS)
 * PPS is the encrypted military P(Y)-code */
#define STATUS_PPS_FIX	9

    /* precision of fix -- valid if satellites_used > 0 */
    int satellites_used;	/* Number of satellites used in solution */
    struct dop_t dop;
	int gps_fd;
	
    /* satellite status -- valid when satellites_visible > 0 */
    timespec_t skyview_time;	/* skyview time */
    int satellites_visible;	/* # of satellites in view */
    struct satellite_t skyview[MAXCHANNELS];

    /* pack things never reported together to reduce structure size */
#define UNION_SET	(AIS_SET|ATTITUDE_SET|ERROR_SET|GST_SET| \
			 LOGMESSAGE_SET|OSCILLATOR_SET|PPS_SET|RAW_SET| \
			 RTCM2_SET|RTCM3_SET|SUBFRAME_SET|TOFF_SET|VERSION_SET)

    /* FIXME! next lib rev need to add a place to put PPS precision */
    struct timedelta_t toff;
    struct timedelta_t pps;
    /* quantization error adjustment to PPS. aka "sawtooth" correction */
    long qErr;                  /* offset in picoseconds (ps) */
    /* time of PPS pulse that qErr applies to */
    timespec_t qErr_time;

    /* Private data - client code must not set this */
    void *privdata;
};

struct gps_context_t {
    int valid;				/* member validity flags */
#define LEAP_SECOND_VALID	0x01	/* we have or don't need correction */
#define GPS_TIME_VALID  	0x02	/* GPS week/tow is valid */
#define CENTURY_VALID		0x04	/* have received ZDA or 4-digit year */
    bool readonly;			/* if true, never write to device */
    speed_t fixed_port_speed;           // Fixed port speed, if non-zero
    char fixed_port_framing[4];         // Fixed port framing, if non-blank
    /* DGPS status */
    int fixcnt;				/* count of good fixes seen */
    /* timekeeping */
    time_t start_time;			/* local time of daemon startup */
    int leap_seconds;			/* Unix seconds to UTC (GPS-UTC offset) */
    unsigned short gps_week;            /* GPS week, usually 10 bits */
    timespec_t gps_tow;                 /* GPS time of week */
    int century;			/* for NMEA-only devices without ZDA */
    int rollovers;			/* rollovers since start of run */
    int leap_notify;			/* notification state from subframe */
#define LEAP_NOWARNING  0x0     /* normal, no leap second warning */
#define LEAP_ADDSECOND  0x1     /* last minute of day has 60 seconds */
#define LEAP_DELSECOND  0x2     /* last minute of day has 59 seconds */
#define LEAP_NOTINSYNC  0x3     /* overload, clock is free running */
};

struct gps_lexer_t {
    /* packet-getter internals */
    int	type;
#define BAD_PACKET      	-1
#define COMMENT_PACKET  	0
#define NMEA_PACKET     	1
#define AIVDM_PACKET    	2
#define GARMINTXT_PACKET	3
#define MAX_TEXTUAL_TYPE	3	/* increment this as necessary */
#define SIRF_PACKET     	4
#define ZODIAC_PACKET   	5
#define TSIP_PACKET     	6
#define EVERMORE_PACKET 	7
#define ITALK_PACKET    	8
#define GARMIN_PACKET   	9
#define NAVCOM_PACKET   	10
#define UBX_PACKET      	11
#define SUPERSTAR2_PACKET	12
#define ONCORE_PACKET   	13
#define GEOSTAR_PACKET   	14
#define NMEA2000_PACKET 	15
#define GREIS_PACKET		16
#define MAX_GPSPACKET_TYPE	16	/* increment this as necessary */
#define RTCM2_PACKET    	17
#define RTCM3_PACKET    	18
#define JSON_PACKET    	    	19
#define PACKET_TYPES		20	/* increment this as necessary */
#define SKY_PACKET     		21
#define TEXTUAL_PACKET_TYPE(n)	((((n)>=NMEA_PACKET) && ((n)<=MAX_TEXTUAL_TYPE)) || (n)==JSON_PACKET)
#define GPS_PACKET_TYPE(n)	(((n)>=NMEA_PACKET) && ((n)<=MAX_GPSPACKET_TYPE))
#define LOSSLESS_PACKET_TYPE(n)	(((n)>=RTCM2_PACKET) && ((n)<=RTCM3_PACKET))
#define PACKET_TYPEMASK(n)	(1 << (n))
#define GPS_TYPEMASK	(((2<<(MAX_GPSPACKET_TYPE+1))-1) &~ PACKET_TYPEMASK(COMMENT_PACKET))
    unsigned int state;
    size_t length;
    unsigned char inbuffer[MAX_PACKET_LENGTH*2+1];
    size_t inbuflen;
    unsigned char *inbufptr;
    /* outbuffer needs to be able to hold 4 GPGSV records at once */
    unsigned char outbuffer[MAX_PACKET_LENGTH*2+1];
    size_t outbuflen;
    unsigned long char_counter;		/* count characters processed */
    unsigned long retry_counter;	/* count sniff retries */
    unsigned counter;			/* packets since last driver switch */
    timespec_t start_time;		/* time of first input */
    unsigned long start_char;		/* char counter at first input */
};

struct gps_device_t {
/* session object, encapsulates all global state */
    struct gps_data_t gpsdata;
    unsigned int cfg_stage;	/* configuration stage counter */
    unsigned int cfg_step;	/* configuration step counter */
    struct gps_context_t	*context;
    int mode;
    struct gps_lexer_t lexer;
    int badcount;
    int subframe_count;
    /* firmware version or subtype ID, 96 too small for ZED-F9 */
    char subtype[128];
    char subtype1[128];
    time_t opentime;
    time_t releasetime;
    bool zerokill;
    time_t reawake;
    timespec_t sor;	        /* time start of this reporting cycle */
    unsigned long chars;	/* characters in the cycle */
    bool ship_to_ntpd;
    int observed;			/* which packet type`s have we seen? */
    bool cycle_end_reliable;		/* does driver signal REPORT_MASK */
    int fixcnt;				/* count of fixes from this device */
    struct gps_fix_t newdata;		/* where drivers put their data */
    struct gps_fix_t lastfix;		/* not quite yet ready for oldfix */
    struct gps_fix_t oldfix;		/* previous fix for error modeling */
    struct {
		unsigned short sats_used[MAXCHANNELS];
		int part, await;		/* for tracking GSV parts */
		struct tm date;	                /* date part of last sentence time */
		timespec_t subseconds;		/* subsec part of last sentence time */
		char *field[NMEA_MAX];
		unsigned char fieldcopy[NMEA_MAX+1];
		/* detect receivers that ship GGA with non-advancing timestamp */
		bool latch_mode;
		char last_gga_timestamp[16];
		char last_gga_talker;
			/* GSV stuff */
		bool seen_bdgsv;
		bool seen_gagsv;
		bool seen_glgsv;
		bool seen_gpgsv;
		bool seen_qzss;
		char last_gsv_talker;
		unsigned char last_gsv_sigid;           /* NMEA 4.1 */
			/* GSA stuff */
		bool seen_glgsa;
		bool seen_gngsa;
		bool seen_bdgsa;
		bool seen_gagsa;
		char last_gsa_talker;
		/*
		* State for the cycle-tracking machinery.
		* The reason these timestamps are separate from the
		* general sentence timestamps is that we can
		* use the minutes and seconds part of a sentence
		* with an incomplete timestamp (like GGA) for
		* end-cycle recognition, even if we don't have a previous
		* RMC or ZDA that lets us get full time from it.
		*/
		timespec_t this_frac_time, last_frac_time;
		bool latch_frac_time;
		int lasttag;              /* index into nmea_phrase[] */
		uint64_t cycle_enders;    /* bit map into nmea_phrase{} */
		bool cycle_continue;
    } nmea;
};

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
gps_mask_t gpsd_poll(struct gps_device_t *session);

void gpsd_init(struct gps_device_t *session, struct gps_context_t *context, const char *device);
void gpsd_time_init(struct gps_context_t *context, time_t starttime);
timespec_t gpsd_utc_resolve(struct gps_device_t *session);
void gpsd_set_century(struct gps_device_t *session);

void packet_reset(struct gps_lexer_t *lexer);
void lexer_init(struct gps_lexer_t *lexer);
double safe_atof(const char *string);
int do_lat_lon(char *field[], struct gps_fix_t *out);
void gpsd_century_update(struct gps_device_t *session, int century);
size_t strlcpy(char *dst, const char *src, size_t siz);
bool str_starts_with(const char *str, const char *prefix);
void TS_NORM( struct timespec *ts);
int clock_gettime(clockid_t clk_id, struct timespec *ts);
double wgs84_separation(double lat, double lon);

#endif	/* __AGPS_H__ */
