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
* @brief    Network check function
* @section  DESC Description
*   - desc
*****************************************************************************/
int ctrl_chk_network(void)
{
	FILE *fp = NULL;
	size_t readSize = 0;
	char buf[1024];
	char *p;
	int recv_pack=0;

	#if APP_RELEASE
	fp = popen("ifconfig eth0 192.168.1.200 up", "r");
	if(!fp) {
		return -1;
	}
	pclose(fp);
	#endif

	sleep(2);
	fp = popen("ping -c 2 -W 1 192.168.1.1", "r");
	if(!fp) {
		return -1;
	}

	// read the result
	readSize = fread( (void*)buf, sizeof(char), 1024-1, fp);

	pclose(fp);

	p = strstr(buf, "packets received") - 2;
	recv_pack = (int)atoi(p);
	dprintf("recv_pack : %d\n", recv_pack);

	#if APP_RELEASE
	fp = popen("ifconfig eth0 down", "r");
	if(!fp) {
		return -1;
	}
	pclose(fp);
	#endif

	return recv_pack;
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
