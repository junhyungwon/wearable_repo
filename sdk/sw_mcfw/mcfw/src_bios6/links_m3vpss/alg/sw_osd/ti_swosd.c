/** ==================================================================
 *  @file   ti_swosd.c
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

/******************************************************************************

              SW OSD implementation for IPNC application

******************************************************************************/

/* include files */

#include <stdio.h>
#include <string.h>
#include <xdc/std.h>
#include "swosd_priv.h"
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/xdm.h>
#include <ti/xdais/dm/ivideo.h>
#include <mcfw/src_bios6/utils/utils_mem.h>
#include "alg_ti_imx_swosd.h"
#include <mcfw/src_bios6/links_m3vpss/alg/common/ti_draw.h>
#include <mcfw/src_bios6/links_m3vpss/camera/cameraLink_priv.h>

/* macros */
#define SWOSD_PRINT_DEBUG

#define SWOSD_ERROR(fmt,args...) Vps_printf(fmt,## args)

#ifdef SWOSD_PRINT_DEBUG
#define SWOSD_DBG(fmt,args...) Vps_printf(fmt,## args)
#else
#define SWOSD_DBG(fmt,args...)
#endif

#define USE_WHITE_FONT		1

/* extern variables */
extern char YData_1540_40_WFONT[];
extern char UVData_1540_40_WFONT[];
#if USE_WHITE_FONT
extern char YData_1232_30_WFONT[];
extern char UVData_1232_30_WFONT[];
extern char YData_770_20_WFONT[];
extern char UVData_770_20_WFONT[];
#else
extern char YData_1232_30[];
extern char UVData_1232_30[];
extern char YData_770_20[];
extern char UVData_770_20[];
#endif

/* Gobal variables */
char *strbuf_Y[AVSERVER_MAX_STREAMS][MAX_STR_BUFF], *strbuf_UV;

char *fontBuff1540_Y1 = NULL, *fontBuff1540_UV1 = NULL;
char *fontBuff1232_Y = NULL, *fontBuff1232_UV = NULL;
char *fontBuff770_Y = NULL, *fontBuff770_UV = NULL;

int fontDB1540_Y1[256], fontDB1540_UV1[256];
int fontDB1232_Y[256], fontDB1232_UV[256];
int fontDB770_Y[256], fontDB770_UV[256];

int idxHndl[AVSERVER_MAX_STREAMS];

/* Global SWOSD config structure */
SWOSD_config_t gSWOSD_config;


/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static char *SWOSD_ALLOC(int size, int alignment)
{
    char *ptr;

    ptr = (char *) Utils_memAlloc(size, alignment);

    return ptr;
}

static void SWOSD_FREE(char *ptr, int size)
{
    Utils_memFree(ptr, size);
}

/*----------------------------------------------------------------------------
 Window enable/disable functions
-----------------------------------------------------------------------------*/
void DM81XX_SWOSD_setSwOsdEnable(int streamId, int enable)
{
    if (streamId < gSWOSD_config.numCaptureStream) {
        gSWOSD_config.swosdConfig[streamId].swosdEnable = enable;
    }
}

void DM81XX_SWOSD_setWindowEnable(int streamId, int win, int enable)
{
	if (streamId < gSWOSD_config.numCaptureStream
				&& (gSWOSD_config.swosdConfig[streamId].en_win[win] != enable))
	{
		if(enable) {
			SWOSD_setBmpWinEnable(gSWOSD_config.swosdConfig[streamId].swosdHndl,
					win, SWOSD_BMPWINENABLE);
			if(win == OSDWIN_3)		//# event
				gSWOSD_config.swosdConfig[streamId].evt_cnt = 5;
			if(win == OSDWIN_4)		//# motion
				gSWOSD_config.swosdConfig[streamId].mot_cnt = 5;
		}
		else {
			SWOSD_setBmpWinEnable(gSWOSD_config.swosdConfig[streamId].swosdHndl,
					win, SWOSD_BMPWINDISABLE);
		}

		gSWOSD_config.swosdConfig[streamId].en_win[win] = enable;
    }
}

/*----------------------------------------------------------------------------
 Set functions
-----------------------------------------------------------------------------*/
void DM81XX_SWOSD_setDateTime(int streamId, DATE_TIME_INFO iDateTime)
{
	int i;

    if (streamId < gSWOSD_config.numCaptureStream)
    {
		if(streamId == 0) {
			memcpy(&gSWOSD_config.dateTimeInfo, &iDateTime, sizeof(DATE_TIME_INFO));
		}

		gSWOSD_config.swosdConfig[streamId].update_datetime = OSD_YES;
		gSWOSD_config.swosdConfig[streamId].update = OSD_YES;
    }

    for(i=0; i<gSWOSD_config.numCaptureStream; i++)
    {
	    if(gSWOSD_config.swosdConfig[i].evt_cnt > 0) {
	    	gSWOSD_config.swosdConfig[i].evt_cnt--;
	    	if(!gSWOSD_config.swosdConfig[i].evt_cnt)
	    		DM81XX_SWOSD_setWindowEnable(i, OSDWIN_3, 0);
	    }
	    if(gSWOSD_config.swosdConfig[i].mot_cnt > 0) {
	    	gSWOSD_config.swosdConfig[i].mot_cnt--;
	    	if(!gSWOSD_config.swosdConfig[i].mot_cnt)
	    		DM81XX_SWOSD_setWindowEnable(i, OSDWIN_4, 0);
	    }
	}
}

void DM81XX_SWOSD_setVolt(int streamId, int value)
{
    if (streamId < gSWOSD_config.numCaptureStream)
    {
		gSWOSD_config.userInfo.voltage = value;
		gSWOSD_config.swosdConfig[streamId].update_volt = OSD_YES;
		gSWOSD_config.swosdConfig[streamId].update = OSD_YES;
    }
}

void DM81XX_SWOSD_setTemp(int streamId, int value)
{
    if (streamId < gSWOSD_config.numCaptureStream)
    {
		gSWOSD_config.userInfo.temp = value;
		gSWOSD_config.swosdConfig[streamId].update_temp = OSD_YES;
		gSWOSD_config.swosdConfig[streamId].update = OSD_YES;
    }
}

void DM81XX_SWOSD_setSpeed(int streamId, int value)
{
    if (streamId < gSWOSD_config.numCaptureStream)
    {
		gSWOSD_config.userInfo.speed = value;
		gSWOSD_config.swosdConfig[streamId].update_speed = OSD_YES;
		gSWOSD_config.swosdConfig[streamId].update = OSD_YES;
    }
}

void DM81XX_SWOSD_setGsens(int streamId, int x, int y, int z)
{
    if (streamId < gSWOSD_config.numCaptureStream)
    {
		gSWOSD_config.userInfo.gsens.x = x;
		gSWOSD_config.userInfo.gsens.y = y;
		gSWOSD_config.userInfo.gsens.z = z;
		gSWOSD_config.swosdConfig[streamId].update_gsens = OSD_YES;
		gSWOSD_config.swosdConfig[streamId].update = OSD_YES;
    }
}

void DM81XX_SWOSD_setUserString(int streamId, char *usrString)
{
    if (streamId < gSWOSD_config.numCaptureStream)
    {
        strcpy(gSWOSD_config.swosdConfig[streamId].swosdUserString, usrString);
        gSWOSD_config.swosdConfig[streamId].update_userstr = OSD_YES;
        gSWOSD_config.swosdConfig[streamId].update = OSD_YES;
    }
}

/*----------------------------------------------------------------------------
 Get functions
-----------------------------------------------------------------------------*/
static int SWOSD_getDateTimeString(int streamId, char *str)
{
    DATE_TIME_INFO *DTIMEInfo;

    DTIMEInfo = &gSWOSD_config.dateTimeInfo;

    sprintf(str, "%04d/%02d/%02d %02d:%02d:%02d",
    		DTIMEInfo->year, DTIMEInfo->month, DTIMEInfo->mday,
    		DTIMEInfo->hours, DTIMEInfo->min, DTIMEInfo->sec);

    return SWOSD_SOK;
}

static void SWOSD_getVoltString(int streamId, char *str)
{
    if (streamId < gSWOSD_config.numCaptureStream) {
        sprintf(str, "%2d.%01dV", gSWOSD_config.userInfo.voltage/10, gSWOSD_config.userInfo.voltage%10);
    }
    else {
        sprintf(str, "0");
    }
}

static void SWOSD_getTempString(int streamId, char *str)
{
    if (streamId < gSWOSD_config.numCaptureStream) {
        sprintf(str, "%3d.%01d'c", gSWOSD_config.userInfo.temp/10, gSWOSD_config.userInfo.temp%10);
    }
    else {
        sprintf(str, "0");
    }
}

static void SWOSD_getSpeedString(int streamId, char *str)
{
    if (streamId < gSWOSD_config.numCaptureStream) {
        sprintf(str, "%3dKm", gSWOSD_config.userInfo.speed);
    }
    else {
        sprintf(str, "0");
    }
}

static void SWOSD_getGsensString(int streamId, char *str)
{
    if (streamId < gSWOSD_config.numCaptureStream) {
        sprintf(str, "x:%3d y:%3d z:%3d",
        	gSWOSD_config.userInfo.gsens.x, gSWOSD_config.userInfo.gsens.y, gSWOSD_config.userInfo.gsens.z);
    }
    else {
        sprintf(str, "0");
    }
}

static void SWOSD_getUserString(int streamId, char *usrStr)
{
	if (streamId < gSWOSD_config.numCaptureStream) {
        strcpy(usrStr, gSWOSD_config.swosdConfig[streamId].swosdUserString);
    }
    else {
        sprintf(usrStr, "0");
    }
    //Vps_printf("---- %s: streamId %d, usrStr %s\n", __func__, streamId, usrStr);
}

/*----------------------------------------------------------------------------
 create font DB
-----------------------------------------------------------------------------*/
static inline void DM81XX_SWOSD_createFontDB(void)
{
    SWOSD_Fontdata *fontInfo;

    /* font width = 10,height = 18 */
    fontInfo = &gSWOSD_config.fontInfo[0];
	fontInfo->format = SWOSD_FORMATYUV420p;
    fontInfo->charWidth = OSD_CHAR_W_770_20;
    fontInfo->charHeight = OSD_CHAR_H_770_20;
    fontInfo->totFontWidth = OSD_STR_W_770_20;
    fontInfo->totFontHeight = OSD_CHAR_H_770_20;
    fontInfo->totFontLineoffset = OSD_STR_W_770_20;
    fontInfo->fontAddr = fontBuff770_Y;
    fontInfo->fontAddrUV = fontBuff770_UV;
    fontInfo->fontDB_Y = fontDB770_Y;
	fontInfo->fontDB_UV = fontDB770_UV;

    SWOSD_createFontDatabase(fontDB770_Y, fontDB770_UV,
							(char *) SWOSD_STRINGPATTERN, fontInfo);

    /* font width = 16, height = 28 */
    fontInfo = &gSWOSD_config.fontInfo[1];
    fontInfo->format = SWOSD_FORMATYUV420p;
    fontInfo->charWidth = OSD_CHAR_W_1232_30;
    fontInfo->charHeight = OSD_CHAR_H_1232_30;
    fontInfo->totFontWidth = OSD_STR_W_1232_30;
    fontInfo->totFontHeight = OSD_CHAR_H_1232_30;
    fontInfo->totFontLineoffset = OSD_STR_W_1232_30;
    fontInfo->fontAddr = fontBuff1232_Y;
    fontInfo->fontAddrUV = fontBuff1232_UV;
    fontInfo->fontDB_Y = fontDB1232_Y;
	fontInfo->fontDB_UV = fontDB1232_UV;

	SWOSD_createFontDatabase(fontDB1232_Y, fontDB1232_UV,
							(char *) SWOSD_STRINGPATTERN, fontInfo);

	/* font width = 20, height = 38 */
	fontInfo = &gSWOSD_config.fontInfo[2];
    fontInfo->format = SWOSD_FORMATYUV420p;
    fontInfo->charWidth = OSD_CHAR_W_1540_40;
    fontInfo->charHeight = OSD_CHAR_H_1540_40;
    fontInfo->totFontWidth = OSD_STR_W_1540_40;
    fontInfo->totFontHeight = OSD_CHAR_H_1540_40;
    fontInfo->totFontLineoffset = OSD_STR_W_1540_40;
    fontInfo->fontAddr = fontBuff1540_Y1;
    fontInfo->fontAddrUV = fontBuff1540_UV1;
    fontInfo->fontDB_Y = fontDB1540_Y1;
	fontInfo->fontDB_UV = fontDB1540_UV1;

    SWOSD_createFontDatabase(fontDB1540_Y1, fontDB1540_UV1,
							(char *) SWOSD_STRINGPATTERN, fontInfo);
}

/*----------------------------------------------------------------------------
 Set win parameters
-----------------------------------------------------------------------------*/
static int DM81XX_SWOSD_setWinParam(SWOSD_Hndl *osdHndl, int idx, int height, int width)
{
	SWOSD_BmpWinPrm bmpWinPrm;
	SWOSD_Fontdata *fontInfo;
	int char_w, char_h, win;
	int xoffset, yoffset;

	if (width <= VIDEO_RESOL_WIDTH_LARGE) {
		fontInfo = &gSWOSD_config.fontInfo[1];
    }
    else {
    	fontInfo = &gSWOSD_config.fontInfo[2];
    }
    char_w = fontInfo->charWidth;
	char_h = fontInfo->charHeight;

	/*
	bmpWinPrm.userTransparancy = SWOSD_RBOOLTRUE;
	bmpWinPrm.enable = SWOSD_RBOOLTRUE;
	bmpWinPrm.format = SWOSD_FORMATYUV420p;
	bmpWinPrm.width = fontInfo->totFontWidth;
	bmpWinPrm.height = char_h;
	bmpWinPrm.lineOffset = fontInfo->totFontWidth;
	bmpWinPrm.transperencyVal = SWOSD_OSDBLACKTRANSVAL;
	bmpWinPrm.transperencyRange = SWOSD_OSDWHITETRANSRANGE;
	bmpWinPrm.transperencyEnable = SWOSD_BMPTRANSDISABLE;//SWOSD_BMPTRANSENABLE
	bmpWinPrm.resizeEnable = 0;
	bmpWinPrm.bmpWinAddr = SWOSD_NULL;
	bmpWinPrm.bmpWinAddrUV = SWOSD_NULL;
	*/

	xoffset = 96;
	yoffset = 48;

//	xoffset = 32;
//	yoffset = 16;

	//# win0 position - date/time
	win = OSDWIN_0;
	bmpWinPrm.startX = xoffset;
	bmpWinPrm.startY = yoffset;
	bmpWinPrm.transperencyEnable = SWOSD_BMPTRANSDISABLE;
	SWOSD_setBmpWinPrm(osdHndl, win, &bmpWinPrm);
	//SWOSD_setBmpchangeWinXYPrm(osdHndl, win, bmpWinPrm.startX, bmpWinPrm.startY);
	//Vps_printf("--- %s: (%d) startX %d, startY %d\n", __func__, win, bmpWinPrm.startX, bmpWinPrm.startY);

	//# win1 position, G
	win = OSDWIN_1;
	bmpWinPrm.startX += SWOSD_align((STR_DATETIME_MAX_LEN * char_w), 4) + (STR_SPACE_LENGTH*2);
	SWOSD_setBmpWinPrm(osdHndl, win, &bmpWinPrm);

	SWOSD_MakeOsdwinstring(fontInfo, "G", strbuf_Y[idx][win], &osdHndl->bmpWinPrm[win], 1);
	strbuf_UV = (char *)(strbuf_Y[idx][win] + (1 * char_w * char_h));
	SWOSD_setBmpWinAddr(osdHndl, strbuf_Y[idx][win], strbuf_UV, win);

	//# win2 position, P
	win = OSDWIN_2;
	bmpWinPrm.startX += SWOSD_align((1 * char_w), 4) + STR_SPACE_LENGTH;
	SWOSD_setBmpWinPrm(osdHndl, win, &bmpWinPrm);

	SWOSD_MakeOsdwinstring(fontInfo, "P", strbuf_Y[idx][win], &osdHndl->bmpWinPrm[win], 1);
	strbuf_UV = (char *)(strbuf_Y[idx][win] + (1 * char_w * char_h));
	SWOSD_setBmpWinAddr(osdHndl, strbuf_Y[idx][win], strbuf_UV, win);

	//# win3 position, E
	win = OSDWIN_3;
	bmpWinPrm.startX += SWOSD_align((1 * char_w), 4) + STR_SPACE_LENGTH;
	SWOSD_setBmpWinPrm(osdHndl, win, &bmpWinPrm);

	SWOSD_MakeOsdwinstring(fontInfo, "E", strbuf_Y[idx][win], &osdHndl->bmpWinPrm[win], 1);
	strbuf_UV = (char *)(strbuf_Y[idx][win] + (1 * char_w * char_h));
	SWOSD_setBmpWinAddr(osdHndl, strbuf_Y[idx][win], strbuf_UV, win);

	//# win4 position, M
	win = OSDWIN_4;
	bmpWinPrm.startX += SWOSD_align((1 * char_w), 4) + STR_SPACE_LENGTH;
	SWOSD_setBmpWinPrm(osdHndl, win, &bmpWinPrm);

	SWOSD_MakeOsdwinstring(fontInfo, "M", strbuf_Y[idx][win], &osdHndl->bmpWinPrm[win], 1);
	strbuf_UV = (char *)(strbuf_Y[idx][win] + (1 * char_w * char_h));
	SWOSD_setBmpWinAddr(osdHndl, strbuf_Y[idx][win], strbuf_UV, win);

	//# win5 position, voltage
	win = OSDWIN_5;
	bmpWinPrm.startX = xoffset;
	bmpWinPrm.startY = yoffset + char_h + STR_SPACE_LENGTH;
	SWOSD_setBmpWinPrm(osdHndl, win, &bmpWinPrm);

	//# win6 position, temp
	win = OSDWIN_6;
	bmpWinPrm.startX += SWOSD_align((STR_VOLT_MAX_LEN * char_w), 4) + STR_SPACE_LENGTH;
	SWOSD_setBmpWinPrm(osdHndl, win, &bmpWinPrm);

	//# win7 position, speed
	win = OSDWIN_7;
	bmpWinPrm.startX += SWOSD_align((STR_TEMP_MAX_LEN * char_w), 4) + STR_SPACE_LENGTH;
	SWOSD_setBmpWinPrm(osdHndl, win, &bmpWinPrm);

	//# win8 position, g-senser
	win = OSDWIN_8;
	bmpWinPrm.startX = xoffset;
	bmpWinPrm.startY = yoffset + ((char_h + STR_SPACE_LENGTH)*2);
	//bmpWinPrm.transperencyEnable = SWOSD_BMPTRANSENABLE;
	SWOSD_setBmpWinPrm(osdHndl, win, &bmpWinPrm);

	//# win9 position
	win = OSDWIN_9;
	bmpWinPrm.startX = xoffset;
	bmpWinPrm.startY = height - (char_h*2);
	bmpWinPrm.transperencyEnable = SWOSD_BMPTRANSDISABLE;
	SWOSD_setBmpWinPrm(osdHndl, win, &bmpWinPrm);

    return SWOSD_SOK;
}

/*----------------------------------------------------------------------------
 Construct the strings for all the windows - runtime
-----------------------------------------------------------------------------*/
static int DM81XX_SWOSD_makeWinString(int idx, SWOSD_Hndl * osdHndl)
{
	SWOSD_Fontdata *fontInfo;
	int char_w, char_h, win;
    char str[SWOSD_MAX_STRING_LENGTH];

	if (gSWOSD_config.swosdConfig[idx].cropWidth <= VIDEO_RESOL_WIDTH_LARGE) {
    	fontInfo = &gSWOSD_config.fontInfo[1];
    }
    else {
    	fontInfo = &gSWOSD_config.fontInfo[2];
    }
    char_w = fontInfo->charWidth;
	char_h = fontInfo->charHeight;
	//Vps_printf("ch(%d): fontInfo 0x%x, %dx%d\n", idx, fontInfo, char_w, char_h);

    if (gSWOSD_config.swosdConfig[idx].en_win[OSDWIN_0]
    		&& gSWOSD_config.swosdConfig[idx].update_datetime == OSD_YES)
    {
    	//# string
    	SWOSD_getDateTimeString(idx, str);

		win = OSDWIN_0;
		SWOSD_MakeOsdwinstring(fontInfo, str, strbuf_Y[idx][win], &osdHndl->bmpWinPrm[win], strlen(str));
		strbuf_UV = (char *) (strbuf_Y[idx][win] + (strlen(str) * char_w * char_h));
		SWOSD_setBmpWinAddr(osdHndl, strbuf_Y[idx][win], strbuf_UV, win);

		gSWOSD_config.swosdConfig[idx].update_datetime = OSD_NO;
    }

    if (gSWOSD_config.swosdConfig[idx].en_win[OSDWIN_5]
    		&& gSWOSD_config.swosdConfig[idx].update_volt == OSD_YES)
    {
		SWOSD_getVoltString(idx, str);

		win = OSDWIN_5;
		SWOSD_MakeOsdwinstring(fontInfo, str, strbuf_Y[idx][win], &osdHndl->bmpWinPrm[win], strlen(str));
		strbuf_UV = (char *) (strbuf_Y[idx][win] + (strlen(str) * char_w * char_h));
		SWOSD_setBmpWinAddr(osdHndl, strbuf_Y[idx][win], strbuf_UV, win);

		gSWOSD_config.swosdConfig[idx].update_volt = OSD_NO;
    }

    if (gSWOSD_config.swosdConfig[idx].en_win[OSDWIN_6]
    		&& gSWOSD_config.swosdConfig[idx].update_temp == OSD_YES)
    {
		SWOSD_getTempString(idx, str);

		win = OSDWIN_6;
		SWOSD_MakeOsdwinstring(fontInfo, str, strbuf_Y[idx][win], &osdHndl->bmpWinPrm[win], strlen(str));
		strbuf_UV = (char *) (strbuf_Y[idx][win] + (strlen(str) * char_w * char_h));
		SWOSD_setBmpWinAddr(osdHndl, strbuf_Y[idx][win], strbuf_UV, win);

		gSWOSD_config.swosdConfig[idx].update_temp = OSD_NO;
    }

    if (gSWOSD_config.swosdConfig[idx].en_win[OSDWIN_7]
    		&& gSWOSD_config.swosdConfig[idx].update_speed == OSD_YES)
    {
		SWOSD_getSpeedString(idx, str);

		win = OSDWIN_7;
		SWOSD_MakeOsdwinstring(fontInfo, str, strbuf_Y[idx][win], &osdHndl->bmpWinPrm[win], strlen(str));
		strbuf_UV = (char *) (strbuf_Y[idx][win] + (strlen(str) * char_w * char_h));
		SWOSD_setBmpWinAddr(osdHndl, strbuf_Y[idx][win], strbuf_UV, win);

		gSWOSD_config.swosdConfig[idx].update_speed = OSD_NO;
    }

    if (gSWOSD_config.swosdConfig[idx].en_win[OSDWIN_8]
    		&& gSWOSD_config.swosdConfig[idx].update_gsens == OSD_YES)
    {
		SWOSD_getGsensString(idx, str);

		win = OSDWIN_8;
		SWOSD_MakeOsdwinstring(fontInfo, str, strbuf_Y[idx][win], &osdHndl->bmpWinPrm[win], strlen(str));
		strbuf_UV = (char *)(strbuf_Y[idx][win] + (strlen(str) * char_w * char_h));
		SWOSD_setBmpWinAddr(osdHndl, strbuf_Y[idx][win], strbuf_UV, win);

		gSWOSD_config.swosdConfig[idx].update_gsens = OSD_NO;
    }

    if (gSWOSD_config.swosdConfig[idx].en_win[OSDWIN_9]
    		&& gSWOSD_config.swosdConfig[idx].update_userstr == OSD_YES)
    {
		SWOSD_getUserString(idx, str);

		win = OSDWIN_9;
		SWOSD_MakeOsdwinstring(fontInfo, str, strbuf_Y[idx][win], &osdHndl->bmpWinPrm[win], strlen(str));
		strbuf_UV = (char *) (strbuf_Y[idx][win] + (strlen(str) * char_w * char_h));
		SWOSD_setBmpWinAddr(osdHndl, strbuf_Y[idx][win], strbuf_UV, win);

		gSWOSD_config.swosdConfig[idx].update_userstr = OSD_NO;
    }

    return SWOSD_SOK;
}

/*----------------------------------------------------------------------------
 SWOSD reset function
-----------------------------------------------------------------------------*/
int DM81XX_SWOSD_Reset(int streamId,int pitch,int width,int height)
{
	SWOSD_Hndl *osdHndl;

	if (streamId < gSWOSD_config.numCaptureStream)
	{
	    osdHndl = gSWOSD_config.swosdConfig[streamId].swosdHndl;

	    gSWOSD_config.swosdConfig[streamId].capLineOffset = pitch;
	    gSWOSD_config.swosdConfig[streamId].cropHeight    = height;
	    gSWOSD_config.swosdConfig[streamId].cropWidth     = width;

	    DM81XX_SWOSD_setWinParam(osdHndl, streamId,
	                             gSWOSD_config.swosdConfig[streamId].cropHeight,
	                             gSWOSD_config.swosdConfig[streamId].cropWidth);
	}

	return SWOSD_SOK;
}

/*----------------------------------------------------------------------------
 SWOSD create function
-----------------------------------------------------------------------------*/
int DM81XX_SWOSD_create(SWOSD_config_t * pSwOsdPrm)
{
    int i=0, j;

    SWOSD_Hndl *osdHndl = NULL;

    /* copy the SW OSD params to the global structure */
    memcpy(&gSWOSD_config, pSwOsdPrm, sizeof(SWOSD_config_t));

    /* initialize the SWOSD lib */
    if (SWOSD_init(SWOSD_EDMA_CH_ID, SWOSD_EDMA_QUEUEID) != 0)
    {
        SWOSD_ERROR("SWOSD_ERROR:SWOSD_init Failure\n");
        return SWOSD_EFAIL;
    }

    /* Allocate the buffers for holding the display strings for each stream */
	for (i = 0; i < gSWOSD_config.numCaptureStream; i++)
    {
        for (j = 0; j < MAX_STR_BUFF; j++)
        {
        	strbuf_Y[i][j] = (char *)SWOSD_ALLOC((SWOSD_MAX_STRING_LENGTH *
					OSD_CHAR_W_MAX * OSD_CHAR_H_MAX * 3) / 2, 32);
        }
    }

	/* Allocate buffers for the fonts */
	fontBuff1540_Y1 = (char *)SWOSD_ALLOC((OSD_STR_W_1540_40 * OSD_CHAR_H_1540_40), 32);
	fontBuff1540_UV1 = (char *)SWOSD_ALLOC((OSD_STR_W_1540_40 * (OSD_CHAR_H_1540_40 / 2)), 32);
	memcpy(fontBuff1540_Y1, YData_1540_40_WFONT, OSD_STR_W_1540_40 * OSD_CHAR_H_1540_40);
	memcpy(fontBuff1540_UV1, UVData_1540_40_WFONT, OSD_STR_W_1540_40 * (OSD_CHAR_H_1540_40 / 2));

	fontBuff1232_Y  = (char *) SWOSD_ALLOC((OSD_STR_W_1232_30 * OSD_CHAR_H_1232_30), 32);
	fontBuff1232_UV = (char *) SWOSD_ALLOC((OSD_STR_W_1232_30 * (OSD_CHAR_H_1232_30 / 2)), 32);
	#if USE_WHITE_FONT
	memcpy(fontBuff1232_Y,  YData_1232_30_WFONT, OSD_STR_W_1232_30 * OSD_CHAR_H_1232_30);
	memcpy(fontBuff1232_UV, UVData_1232_30_WFONT, OSD_STR_W_1232_30 * (OSD_CHAR_H_1232_30 / 2));
	#else
	memcpy(fontBuff1232_Y,  YData_1232_30, OSD_STR_W_1232_30 * OSD_CHAR_H_1232_30);
	memcpy(fontBuff1232_UV, UVData_1232_30, OSD_STR_W_1232_30 * (OSD_CHAR_H_1232_30 / 2));
	#endif

	fontBuff770_Y  = (char *) SWOSD_ALLOC((OSD_STR_W_770_20 * OSD_CHAR_H_770_20), 32);
	fontBuff770_UV = (char *) SWOSD_ALLOC((OSD_STR_W_770_20 * (OSD_CHAR_H_770_20 / 2)), 32);
	#if USE_WHITE_FONT
	memcpy(fontBuff770_Y,  YData_770_20_WFONT, OSD_STR_W_770_20 * OSD_CHAR_H_770_20);
	memcpy(fontBuff770_UV, UVData_770_20_WFONT, OSD_STR_W_770_20 * (OSD_CHAR_H_770_20 / 2));
	#else
	memcpy(fontBuff770_Y,  YData_770_20, OSD_STR_W_770_20 * OSD_CHAR_H_770_20);
	memcpy(fontBuff770_UV, UVData_770_20, OSD_STR_W_770_20 * (OSD_CHAR_H_770_20 / 2));
	#endif

    /* create font DB from the font buffers */
    DM81XX_SWOSD_createFontDB();

    /* create SWOSD handles for each stream */
    for (i = 0; i < gSWOSD_config.numCaptureStream; i++)
    {
        if (SWOSD_createHandle(&osdHndl, &idxHndl[i]) != SUCCESS)
        {
            SWOSD_ERROR("SWOSD_ERROR:Handle Creation Failed in SWOSD(%d)\n", i);
            return SWOSD_EFAIL;
        }
        else
        {
            SWOSD_setNumWindows(osdHndl, SWOSD_WINMAX);
            DM81XX_SWOSD_setWinParam(osdHndl, i,
                                     gSWOSD_config.swosdConfig[i].cropHeight,
                                     gSWOSD_config.swosdConfig[i].cropWidth);
        }

        gSWOSD_config.swosdConfig[i].swosdHndl = osdHndl;

        //# swosd window enable
        for(j=0; j<MAX_OSDWIN; j++)
        {
	        if (gSWOSD_config.swosdConfig[i].en_win[j]) {
				SWOSD_setBmpWinEnable(osdHndl, j, SWOSD_BMPWINENABLE);
			}
			else {
				SWOSD_setBmpWinEnable(osdHndl, j, SWOSD_BMPWINDISABLE);
			}
		}
    }

    return SWOSD_SOK;
}

/*----------------------------------------------------------------------------
 SWOSD delete function
-----------------------------------------------------------------------------*/
int DM81XX_SWOSD_delete(void)
{
    int i, j;

    /* free the allocated memory */

    /* display string buffers */
	for (i = 0; i < gSWOSD_config.numCaptureStream; i++)
    {
        for (j = 0; j < MAX_STR_BUFF; j++)
        {
        	if (strbuf_Y[i][j] != NULL)
                SWOSD_FREE(strbuf_Y[i][j],
					((SWOSD_MAX_STRING_LENGTH * OSD_CHAR_W_MAX * OSD_CHAR_H_MAX * 3) / 2));
        }
    }

    /* font buffers */
    if (fontBuff1540_Y1 != NULL)
        SWOSD_FREE(fontBuff1540_Y1, (OSD_STR_W_1540_40 * OSD_CHAR_H_1540_40));
    if (fontBuff1540_UV1 != NULL)
        SWOSD_FREE(fontBuff1540_UV1, (OSD_STR_W_1540_40 * (OSD_CHAR_H_1540_40 / 2)));

    if (fontBuff1232_Y != NULL)
        SWOSD_FREE(fontBuff1232_Y, (OSD_STR_W_1232_30 * OSD_CHAR_H_1232_30));
    if (fontBuff1232_UV != NULL)
        SWOSD_FREE(fontBuff1232_UV, (OSD_STR_W_1232_30 * (OSD_CHAR_H_1232_30 / 2)));

    if (fontBuff770_Y != NULL)
        SWOSD_FREE(fontBuff770_Y, (OSD_STR_W_770_20 * OSD_CHAR_H_770_20));
    if (fontBuff770_UV != NULL)
        SWOSD_FREE(fontBuff770_UV, (OSD_STR_W_770_20 * (OSD_CHAR_H_770_20 / 2)));

	//# phoong - delete idx handle
    for (i = 0; i < gSWOSD_config.numCaptureStream; i++)
    {
        if (SWOSD_deleteHandle(idxHndl[i]) != SUCCESS) {
            SWOSD_ERROR("SWOSD_ERROR:Handle delete Failed in SWOSD(%d)\n", i);
            return SWOSD_EFAIL;
        }
    }

    //# SWOSD exit
    SWOSD_exit();

    return SWOSD_SOK;
}

/*----------------------------------------------------------------------------
 SWOSD apply  function
-----------------------------------------------------------------------------*/
int DM81XX_SWOSD_apply(int streamId, int frameBufAddrY, int frameBufAddrUV)
{
    SWOSD_Hndl *osdHndl;

    SWOSD_MainWinPrm mainWinPrm;

    if (streamId < gSWOSD_config.numCaptureStream)
    {
        if (gSWOSD_config.swosdConfig[streamId].swosdEnable)
        {
			//Vps_printf("--- %d, 0x%x, 0x%x\n", streamId, frameBufAddrY, frameBufAddrUV);

			osdHndl = (SWOSD_Hndl *) gSWOSD_config.swosdConfig[streamId].swosdHndl;

			/* Set main window parameters */
			mainWinPrm.format = gSWOSD_config.captureYuvFormat;
			mainWinPrm.lineOffset = gSWOSD_config.swosdConfig[streamId].capLineOffset;
			mainWinPrm.width = gSWOSD_config.swosdConfig[streamId].capWidth;
			mainWinPrm.height = gSWOSD_config.swosdConfig[streamId].capHeight;

			//Vps_printf("--- %d, %d, %d\n", mainWinPrm.lineOffset, mainWinPrm.width, mainWinPrm.height);

			SWOSD_setMainWinPrm(osdHndl, &mainWinPrm);

			/* SW OSD windows are updated */
			if(gSWOSD_config.swosdConfig[streamId].update == OSD_YES) {
			    DM81XX_SWOSD_makeWinString(streamId, osdHndl);
			    gSWOSD_config.swosdConfig[streamId].update = OSD_NO;
			}

			if(gSWOSD_config.swosdConfig[streamId].memType == TRUE )
			{
				frameBufAddrY = Utils_tilerAddr2CpuAddr((uint32)frameBufAddrY);
				frameBufAddrUV = Utils_tilerAddr2CpuAddr((uint32)frameBufAddrUV);
			}

			osdHndl->memType = gSWOSD_config.swosdConfig[streamId].memType;

			/* Draw the SW OSD windows */
			SWOSD_winDrawHandle(osdHndl, (short *) frameBufAddrY, (short *) frameBufAddrUV);
        }
    }

    return SWOSD_SOK;
}
