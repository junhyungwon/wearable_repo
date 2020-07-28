/******************************************************************************
 * NEXTT360 Board
 * Copyright by LF, Incoporated. All Rights Reserved.
 * based on gpsd.
 *---------------------------------------------------------------------------*/
 /**
 * @file    nmea.c
 * @brief
 * @section MODIFY history
 *     - 2020.07.21 : First Created
 */
/*****************************************************************************/

#ifndef __NMEA_H__
#define __NMEA_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <stdint.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#ifndef bool
typedef int bool;
#define true 								1
#define false 								0
#endif

#define BUILD_CENTURY 						2000
#define MONTHSPERYEAR						12
#define BUILD_LEAPSECONDS 					18
#define GPS_EPOCH							((time_t)315964800)   /* 6 Jan 1980 00:00:00 UTC */
#define MAXCHANNELS							140
#define MAX_PACKET_LENGTH					9216		/* 4 + 16 + (256 * 32) + 2 + fudge */
#define NMEA_MAX							102	
#define packet_buffered_input(lexer) 		((lexer)->inbuffer + (lexer)->inbuflen - (lexer)->inbufptr)
#define KNOTS_TO_MPS						0.51444444	/* Knots to meters per second */
#define GPS_PI      						3.1415926535897932384626433832795029
#define RAD_2_DEG							57.2957795130823208767981548141051703
#define DEG_2_RAD							0.0174532925199432957692369076848861271
#define TIMESPEC_LEN						22

//---------------------------------------------------------------------------------------------
#define LEAP_SECOND_VALID					0x01
#define GPS_TIME_VALID  					0x02
#define CENTURY_VALID						0x04

//--------------------------------------------------------------------------------------------
#define PACKET_TYPEMASK(n)					(1 << (n))
#define BAD_PACKET      					-1
#define COMMENT_PACKET  					0
#define NMEA_PACKET     					1

//---------------------------- GPS_RETURN_VALUE ----------------------------------------------
#define ONLINE_SET							(1llu << 1)
#define TIME_SET							(1llu << 2)
#define LATLON_SET							(1llu << 4)
#define SPEED_SET							(1llu << 6)
#define TRACK_SET							(1llu << 7)
#define CLIMB_SET							(1llu << 8)
#define STATUS_SET							(1llu << 9)
#define MODE_SET							(1llu << 10)
#define SATELLITE_SET						(1llu << 15)
#define PACKET_SET							(1llu << 25)
#define ERROR_SET							(1llu << 31)
#define SET_HIGH_BIT						42
//#--------------------------gps_fix_mode -----------------------------------------------------
#define MODE_NOT_SEEN						0
#define MODE_NO_FIX							1
#define MODE_2D  							2
#define MODE_3D  							3

#define STATUS_NO_FIX						0
#define STATUS_FIX							1
#define STATUS_DGPS_FIX						2

//---------------------------------------------------------------------------------------------
#define INTERNAL_SET(n)						((gps_mask_t)(1llu<<(SET_HIGH_BIT+(n))))
#define RAW_IS  							INTERNAL_SET(1) 	/* raw pseudoranges available */
#define USED_IS 							INTERNAL_SET(2) 	/* sat-used count available */
#define DRIVER_IS							INTERNAL_SET(3) 	/* driver type identified */
#define CLEAR_IS							INTERNAL_SET(4) 	/* starts a reporting cycle */
#define REPORT_IS							INTERNAL_SET(5) 	/* ends a reporting cycle */
#define NODATA_IS							INTERNAL_SET(6) 	/* no data read from fd */
#define NTPTIME_IS							INTERNAL_SET(7) 	/* precision time is available */
#define GOODTIME_IS							INTERNAL_SET(11) 	/* time good even if no pos fix */

//----------------------------------------------------------------------------------------------
#define DD(s)   							((int)((s)[0]-'0')*10+(int)((s)[1]-'0'))
#define TS_GEZ(ts) 							(0 <= (ts)->tv_sec && 0 <= (ts)->tv_nsec)
#define TSTONS(ts) 							((double)((ts)->tv_sec + ((ts)->tv_nsec / 1e9)))

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
	
#define NS_IN_SEC	1000000000LL     /* nanoseconds in a second */
#define US_IN_SEC	1000000LL        /* microseconds in a second */
#define MS_IN_SEC	1000LL           /* milliseconds in a second */

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef unsigned long long gps_mask_t; 
typedef struct timespec timespec_t;		
typedef int clockid_t;

/* GPS 데이터 수신을 위한 공용 구조체 */
struct satellite_t {
    double ss;
    bool used;
    short PRN;
    double elevation;
    double azimuth;
    unsigned char gnssid;
    unsigned char svid;
    unsigned char sigid;
    signed char freqid;         /* The GLONASS (Only) frequency, 0 - 13 */
    unsigned char health;       /* 0 = unknown, 1 = healthy, 2 = unhealthy */
};

struct gps_fix_t {
    timespec_t time;	
    int    mode;	
    double latitude;	
    double longitude;
	double track;		/* 자북의 방향 */
	double speed;
};

struct gps_data_t {
    gps_mask_t set;
    timespec_t online;	

    struct gps_fix_t fix;

    int status;		
    int satellites_used;	/* Number of satellites used in solution */
	
    timespec_t skyview_time;	/* skyview time */
    int satellites_visible;		/* # of satellites in view */
    struct satellite_t skyview[MAXCHANNELS];
};

struct gps_lexer_t {
    int	type;
    unsigned int state;
    size_t length;
    unsigned char inbuffer[MAX_PACKET_LENGTH*2+1];
    size_t inbuflen;
    unsigned char *inbufptr;
    unsigned char outbuffer[MAX_PACKET_LENGTH*2+1];
    size_t outbuflen;
    unsigned long char_counter;		/* count characters processed */
    unsigned counter;				/* packets since last driver switch */
};

struct gps_context_t {
	int valid;
	time_t start_time;
	int leap_seconds;
	int century;
};

struct gps_device_t {
    struct gps_context_t *context;
	struct gps_data_t gpsdata;
    struct gps_lexer_t lexer;
	
	struct gps_fix_t newdata;
	struct gps_fix_t lastfix;
    struct gps_fix_t oldfix;
	
	struct {
		unsigned short sats_used[MAXCHANNELS];
		int part, await;		/* for tracking GSV parts */
		struct tm date;	                /* date part of last sentence time */
		timespec_t subseconds;		/* subsec part of last sentence time */
		char *field[NMEA_MAX];
		unsigned char fieldcopy[NMEA_MAX+1];
		
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
		/* GSA stuff */
		bool seen_glgsa;
		bool seen_gngsa;
		bool seen_bdgsa;
		bool seen_gagsa;
		char last_gsa_talker;
		
		timespec_t this_frac_time, last_frac_time;
		bool latch_frac_time;
		int lasttag;              /* index into nmea_phrase[] */
		
		uint64_t cycle_enders;    /* bit map into nmea_phrase{} */
		bool cycle_continue;
    } nmea;
};

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
void nmea_parse_init(void);
gps_mask_t nmea_parse_poll(gnss_shm_data_t *data);

#endif	/* __NMEA_H__ */