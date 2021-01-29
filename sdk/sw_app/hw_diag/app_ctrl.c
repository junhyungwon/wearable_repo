/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_ctrl.c
 * @brief	control for mcfw functions
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "ti_vcam.h"
#include "ti_vcap.h"
#include "ti_venc.h"
#include "ti_vdis.h"

#include "dev_common.h"
#include "dev_micom.h"

#include "app_comm.h"
#include "app_main.h"
#include "gui_main.h"
#include "app_ctrl.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define LED_BASE_PATH		"/sys/class/leds/"
#define LED_BLINK_MS		300

#define DEFAULT_FPS			30
#define FLOOR_ALIGN(val, align)  (((val) / (align)) * (align))

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    SW mosaic(Video layout) control function
* @section  DESC Description
*   - desc
*****************************************************************************/
int ctrl_swms_set(int win_num, int ch)
{
	VDIS_MOSAIC_S vdMosaicParam;
	int out_wi, out_he, wi_align;
	int win_id, win_wi, win_he;
	int i;

	out_wi = 720;
	out_he = 480;
	wi_align = 8;

	vdMosaicParam.displayWindow.width		= out_wi;
	vdMosaicParam.displayWindow.height		= out_he;
	vdMosaicParam.numberOfWindows 			= win_num;
	vdMosaicParam.displayWindow.start_X		= 0;
	vdMosaicParam.displayWindow.start_Y		= 0;
	vdMosaicParam.onlyCh2WinMapChanged		= FALSE;
	vdMosaicParam.outputFPS 				= 30;

	win_id = 0;
	if(win_num == 1)
	{
		win_wi = FLOOR_ALIGN(out_wi, wi_align);
		win_he = out_he;
		vdMosaicParam.chnMap[win_id] = ch;
    	vdMosaicParam.winList[win_id].start_X = 0;
		vdMosaicParam.winList[win_id].start_Y = 0;
		vdMosaicParam.winList[win_id].width = win_wi;
		vdMosaicParam.winList[win_id].height = win_he;
		vdMosaicParam.useLowCostScaling[win_id] = FALSE;
	}
	else if(win_num == 4)
	{
		win_wi = 288;
    	win_he = 162;
		for(i=0; i<2; i++) {
	    	vdMosaicParam.chnMap[win_id] = ch++;
	    	vdMosaicParam.winList[win_id].start_X = 96 + (i*win_wi);
			vdMosaicParam.winList[win_id].start_Y = 46;
			vdMosaicParam.winList[win_id].width = win_wi;
			vdMosaicParam.winList[win_id].height = win_he;
			vdMosaicParam.useLowCostScaling[win_id] = FALSE;
			win_id++;
		}
		for(i=0; i<2; i++) {
	    	vdMosaicParam.chnMap[win_id] = ch++;
	    	vdMosaicParam.winList[win_id].start_X = 96 + (i*win_wi);
			vdMosaicParam.winList[win_id].start_Y = 46+162;
			vdMosaicParam.winList[win_id].width = win_wi;
			vdMosaicParam.winList[win_id].height = win_he;
			vdMosaicParam.useLowCostScaling[win_id] = FALSE;
			win_id++;
		}
	}
	else {
		return -1;
	}

	Vdis_setMosaicParams(VDIS_DEV_SD, &vdMosaicParam);

	//# wait for the info prints to complete
	OSA_waitMsecs(100);

	return 0;
}

/*****************************************************************************
* @brief    Network function. set ip and linkup
* @section  DESC Description
*   - desc
*****************************************************************************/
int ctrl_ether_linkup(const char *naddr, const char *nmask, const char *ngate)
{
	FILE *f = NULL;
	char cmd[256]={0,};
	
	if ((naddr == NULL) || (nmask == NULL) || (ngate == NULL)) {
		return -1;
	}
	
	if ((strcmp(naddr, "0.0.0.0") == 0) || 
		(strcmp(nmask, "0.0.0.0") == 0) || 
		(strcmp(ngate, "0.0.0.0") == 0))
	{	
		return -1;
	}
	
	memset(cmd, 0 , sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "/sbin/ifconfig eth0 %s netmask %s up", naddr, nmask);
	f = popen(cmd, "r");
	if (f != NULL) {
		pclose(f);
	} else {
		return -1;
	}
	/* required wait */
	sleep(2);
	
	memset(cmd, 0 , sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "/sbin/route add default gw %s dev eth0", ngate);
	f = popen(cmd, "r");
	if (f != NULL) {
		pclose(f);
	} else {
		return -1;
	}
		
	return 0;
}

/*****************************************************************************
* @brief    Network deactive function
* @section  DESC Description
*   - desc
*****************************************************************************/
void ctrl_ether_linkdown(void)
{
	FILE *f = NULL;
	char buf[128]={0,};
	
	f = popen("/sbin/ifconfig eth0 down", "r");
	if (f != NULL) {
		pclose(f);
	}
	/* required wait */
	sleep(2);
}

/*****************************************************************************
* @brief    get version function
* @section  DESC Description
*   - desc
*****************************************************************************/
int ctrl_get_hw_version(char *version)
{
	int ver;

	ver = dev_get_board_info();
	if(ver < 0) {
		sprintf(version, "UNKNOWN");
		return -1;
	}

	if(version != NULL) {
		sprintf(version, "00.%02d", (ver+1));
	}

	return ver;
}

int ctrl_get_mcu_version(char *version)
{
	int ver;

	ver = (int)mic_get_version();
	sprintf(version, "%02d.%02X", (ver>>8)&0xFF, ver&0xFF);

	return ver;
}

/*****************************************************************************
* @brief    leds control function
* @section  DESC Description
*   - desc
*****************************************************************************/
int ctrl_leds(int index, int state)
{
	char path[256] = {0,};
	char buf[16];
	int fd, len, brightness;

	if (state) brightness = 255;
	else	   brightness = 0;

	buf[sizeof(buf) - 1] = '\0';
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), "%s/led%d/trigger", LED_BASE_PATH, index);
	if ((fd = open(path, O_WRONLY)) < 0) {
		printf("Error open %s\n", path);
		return -1;
	}

	len = snprintf(buf, sizeof(buf), "none");
	write(fd, buf, len);
	close(fd);

	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), "%s/led%d/brightness", LED_BASE_PATH, index);
	if ((fd = open(path, O_WRONLY)) < 0) {
		printf("Error open %s\n", path);
		return -1;
	}

	buf[sizeof(buf) - 1] = '\0';
	len = snprintf(buf, sizeof(buf), "%d", brightness);
	write(fd, buf, len);
	close(fd);

	return 0;
}

/*****************************************************************************
* @brief    rate control (VRB/CBR)
* @section  [desc]
*****************************************************************************/
int ctrl_vid_rate(int ch, int rc)
{
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };
    
	params.rateControl = rc;
	Venc_setDynamicParam(ch, 0, &params, VENC_RATECONTROL);  // set rate control

	if (rc == RATE_CTRL_VBR) {
		params.qpMax 	= 45;
		params.qpInit 	= -1;
        params.qpMin    = 10;
	}
	else	//# RATE_CTRL_CBR
	{
		params.qpMin	= 10;//10;	//# for improve quality: 10->0
		params.qpMax 	= 40;
		params.qpInit 	= -1;
	}

	Venc_setDynamicParam(ch, 0, &params, VENC_QPVAL_P);  // set QP range
	return SOK;
}

int ctrl_enc_multislice(void)
{
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };

    /* fixed HD */
	params.packetSize = 12;  // --> slice count 8
    Venc_setDynamicParam(MODEL_CH_NUM, 0, &params, VENC_PACKETSIZE);

    return SOK ;
}

/*****************************************************************************
* @brief    set video gop
* @section  [desc]
*****************************************************************************/
int ctrl_vid_gop_set(int ch, int gop)
{
    VENC_CHN_DYNAMIC_PARAM_S params = { 0 };

    params.intraFrameInterval = gop;
    Venc_setDynamicParam(ch, 0, &params, VENC_IPRATIO);

    return SOK ;
}

/*****************************************************************************
* @brief    read network config from ethernet.conf
* @section  [desc]
*****************************************************************************/
int ctrl_ether_cfg_read(const char *fname, const char *ip, const char *mask, const char *gate)
{
	FILE *f = NULL;
	char lineBuf[256 + 1];
	/*
	 * IPADDR="xxx.xxx.xxx.xxx"
	 * NETMASK="xxx.xxx.xxx.xxx"
	 * GATEWAY="xxx.xxx.xxx.xxx"
	 */ 
	f = fopen(fname, "r");
	if (f == NULL) {
		eprintf("Can't read %s\n", fname);
		return -1;
	}
	
	while (fgets(lineBuf, sizeof(lineBuf), f) != NULL)
	{
		char *s, *data;
		int i;
		
		if ((s = strstr(lineBuf, "IPADDR=")) != NULL)
		{
			/* skip IPADDR=" ==>8 */
			s += 8; data = (char *)ip;
			for (i = 0; i < 16; i++) /* xxx.xxx.xxx.xxx */ 
			{
				if ((s[i] == '\0') || (s[i] == '"')) {
					/* break character */
					break;
				}
				/* fill ssid */
				data[i] = s[i];
			}
			data[i] = '\0';
			dprintf("IP: %s\n", data);
		}
		else if ((s = strstr(lineBuf, "NETMASK=")) != NULL)
		{
			/* skip NETMASK=" ==>9 */
			s += 9; data = (char *)mask;
			for (i = 0; i < 16; i++) /* xxx.xxx.xxx.xxx */ 
			{
				if ((s[i] == '\0') || (s[i] == '"')) {
					/* break character */
					break;
				}
				/* fill ssid */
				data[i] = s[i];
			}
			data[i] = '\0';
			dprintf("MASK: %s\n", data);
		}
		else if ((s = strstr(lineBuf, "GATEWAY=")) != NULL)
		{
			/* skip GATEWAY=" ==>9 */
			s += 9; data = (char *)gate;
			for (i = 0; i < 16; i++) /* xxx.xxx.xxx.xxx */ 
			{
				if ((s[i] == '\0') || (s[i] == '"')) {
					/* break character */
					break;
				}
				/* fill ssid */
				data[i] = s[i];
			}
			data[i] = '\0';
			dprintf("GATE: %s\n", data);
		}
	}
	
	fclose(f);
	
    return 0;
}
