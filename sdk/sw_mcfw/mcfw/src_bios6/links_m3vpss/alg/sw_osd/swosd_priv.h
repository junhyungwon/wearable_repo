/** ==================================================================
 *  @file   swosd_priv.h
 *
 *  @path    ipnc_mcfw/mcfw/src_bios6/links_m3vpss/alg/sw_osd/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

#ifndef _DM81XX_SWOSD_H_
#define _DM81XX_SWOSD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* include files */
#include <xdc/std.h>
#include "alg_ti_swosd.h"
#include "alg_ti_imx_swosd.h"

/* macros */

#define SWOSD_USER_TRANSPARANCY_ON		// <<< SW OSD transperancy is turned ON

#define SWOSD_align(value,align)   ((((value) + ((align) - 1 )) / (align)) * (align))

#define SWOSD_floor(value, align)   (((value) / (align)) * (align))
#define SWOSD_ceil(value, align)    SWOSD_align(value, align)

/* SW OSD EDMA CH ID and QUEUE ID */
#define SWOSD_EDMA_CH_ID		5
#define SWOSD_EDMA_QUEUEID		0

/* SW OSD window IDs */
#define OSDWIN_0				0	//# date time
#define OSDWIN_1				1	//# G:gps
#define OSDWIN_2				2	//# P:parking
#define OSDWIN_3				3	//# E:event
#define OSDWIN_4				4	//# M:motion
#define OSDWIN_5				5	//# voltage
#define OSDWIN_6				6	//# temperature
#define OSDWIN_7				7	//# speed
#define OSDWIN_8				8	//# g-sensor
#define OSDWIN_9				9	//#	User String
#define MAX_OSDWIN				10

#define MAX_STR_BUFF			MAX_OSDWIN
#define AVSERVER_MAX_STREAMS	3		// <<< TBD - move to appropriate place

#define SWOSD_STRINGPATTERN   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz:; '/\\+-_()&$#.0123456789"

#define OSD_YES					1
#define OSD_NO					0
#define SUCCESS					0

#define VIDEO_RESOL_WIDTH_LARGE		1500	//# 1080p
#define VIDEO_RESOL_WIDTH_MEDIUM	1000	//# 720p
#define VIDEO_RESOL_WIDTH_SMALL		750		//# D1

#define SWOSD_MAX_STRING_LENGTH		32

#define STR_SPACE_LENGTH		8
#define STR_DATETIME_MAX_LEN	19
#define STR_STE_MAX_LEN			1
#define STR_SPEED_MAX_LEN		5
#define STR_TEMP_MAX_LEN		7
#define STR_VOLT_MAX_LEN		5

#define SWOSD_OSDWHITETRANSVAL          0x80FF
#define SWOSD_OSDWHITETRANSRANGE        0x48
#define SWOSD_OSDBLACKTRANSVAL          0x00
#define SWOSD_OSDBLACKTRANSRANGE        0x48

#define OSD_CHAR_W_MAX			20
#define OSD_CHAR_H_MAX			38

#define OSD_CHAR_W_1540_40		20
#define OSD_CHAR_H_1540_40		38
#define OSD_STR_W_1540_40		1540
#define OSD_STR_H_1540_40		38

#define OSD_CHAR_W_1232_30		16
#define OSD_CHAR_H_1232_30		28
#define OSD_STR_W_1232_30		1232
#define OSD_STR_H_1232_30		28

#define OSD_CHAR_W_1078_28		14
#define OSD_CHAR_H_1078_28		26
#define OSD_STR_W_1078_28		1078
#define OSD_STR_H_1078_28		26

#define OSD_CHAR_W_770_20		10
#define OSD_CHAR_H_770_20		18
#define OSD_STR_W_770_20		770
#define OSD_STR_H_770_20		18

/* typedefs */

/* Data structure definitions */

typedef struct _DATE_TIME_INFO {
    int year;
    int month;
    int mday;
    int hours;
    int min;
    int sec;
} DATE_TIME_INFO;

/* SW OSD TYPE */
typedef enum {
    SWOSD_BASIC = 0,
    SWOSD_DETAIL,
    SWOSD_COMPLETE
} SWOSD_Type;

/* Time format */
typedef enum {
    SWOSD_FMT_12HR = 0,
    SWOSD_FMT_24HR
} SWOSD_TIME_FMT;

/* Year format */
typedef enum {
    SWOSD_FMT_YMD = 0,
    SWOSD_FMT_MDY,
    SWOSD_FMT_DMY
} SWOSD_DATE_FMT;

/* SWOSD config for each stream */
typedef struct {
    Bool swosdEnable;			// <<< 0 - SW OSD is disabled,1 - SW OSD enabled
    SWOSD_Hndl *swosdHndl;		// <<< Handle for SW OSD

    int capLineOffset;			// <<< Line offset of the capture buffer
    int capWidth;				// <<< Width of capture buffer
    int capHeight;				// <<< Height of capture buffer

    int cropWidth;				// <<< Encode frame width
    int cropHeight;				// <<< Encode frame height
	int memType;				// System_MemoryType - Tiled / non-tiled */

    SWOSD_Type swosdType;
    SWOSD_TIME_FMT swosdTimeFormat;
    SWOSD_DATE_FMT swosdDateFormat;

    char swosdUserString[SWOSD_MAX_STRING_LENGTH];

	Bool en_win[MAX_OSDWIN];

	//# update flag
	Bool update;
	Bool update_datetime;
	Bool update_volt;
	Bool update_temp;
	Bool update_speed;
	Bool update_gsens;
	Bool update_userstr;

	int evt_cnt;
	int mot_cnt;
} SWOSD_streamConfig_t;

typedef struct {
	int x;
	int y;
	int z;
} gsens_t;

typedef struct {
	int voltage;
	int temp;
	int speed;
	gsens_t gsens;
} SWOSD_userInfo_t;

/* SW OSD config structure */
typedef struct {
    int numCaptureStream;
    int captureYuvFormat;
    int autoFocusVal;
    SWOSD_streamConfig_t swosdConfig[AVSERVER_MAX_STREAMS];
    SWOSD_Fontdata fontInfo[3];
    DATE_TIME_INFO dateTimeInfo;
	SWOSD_userInfo_t userInfo;
} SWOSD_config_t;

/* Function prototypes */

int DM81XX_SWOSD_create(SWOSD_config_t * pSwOsdPrm);
int DM81XX_SWOSD_delete(void);
int DM81XX_SWOSD_apply(int streamId, int frameBufAddrY, int frameBufAddrUV);
int DM81XX_SWOSD_Reset(int streamId,int pitch,int width,int height);

void DM81XX_SWOSD_setSwOsdEnable(int streamId, int enable);
void DM81XX_SWOSD_setWindowEnable(int streamId, int win, int enable);

void DM81XX_SWOSD_setDateTime(int streamId, DATE_TIME_INFO iDateTime);
void DM81XX_SWOSD_setVolt(int streamId, int value);
void DM81XX_SWOSD_setTemp(int streamId, int value);
void DM81XX_SWOSD_setSpeed(int streamId, int value);
void DM81XX_SWOSD_setGsens(int streamId, int x, int y, int z);
void DM81XX_SWOSD_setUserString(int streamId, char *usrString);

#ifdef __cplusplus
}
#endif
#endif	/* _DM81XX_SWOSD_H_ */
