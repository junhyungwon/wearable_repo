/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_led.c
 * @brief	app led control.
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include "app_comm.h"
#include "app_file.h"
#include "app_rec.h"
#include "app_main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define LED_BASE_PATH		"/sys/class/leds/"

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef enum {
	LED_IDX_RF_G = 0,       //# kernel LED_RF_G
	LED_IDX_RF_R,			//# kernel LED_RF_R 
	LED_IDX_GPS_G,			//# kernel LED_GPS_G
	LED_IDX_GPS_R,			//# kernel LED_GPS_R
	LED_IDX_BAT_G1,
	LED_IDX_BAT_G2,
	LED_IDX_BAT_G3,
	LED_IDX_BAT_G4,
	LED_IDX_CAM1,  			//# CAM1
	LED_IDX_CAM3,  			//# CAM2
	LED_IDX_CAM4,  			//# CAM3 
	LED_IDX_CAM2,  			//# CAM4
	LED_IDX_SD_G,
	LED_IDX_SD_R,
	LED_IDX_SD_G1,
	LED_IDX_SD_G2,
	LED_IDX_SD_G3,
	LED_IDX_REC,
	LED_IDX_BACKUP,
	LED_IDX_ETH_STATUS,
	LED_IDX_ALL
} LEDS_IDX;

static char ledste[LED_IDX_ALL]; //# led state
static OSA_MutexHndl led_lock;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

static int leds_ctrl(int index, int state)
{
	char path[256] = {0,};
	int fd, ret = 0;
	
	if (ledste[index] == -2) {
		TRACE_ERR("Invalid %d[th]LED!\n", index);
		return -1;
	} else if (ledste[index] == state)
		return 0;

	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), "%s/led%d/trigger", LED_BASE_PATH, index);
	if ((fd = open(path, O_WRONLY)) < 0) {
		return EFAIL;
	}

	if (state == DEV_LED_BLINK) {
		write(fd, "timer", 5);
		close(fd);

		memset(path, 0, sizeof(path));
		snprintf(path, sizeof(path), "%s/led%d/delay_on", LED_BASE_PATH, index);
		if ((fd = open(path, O_WRONLY)) < 0) {
			return EFAIL;
		}

		write(fd, "300", 3);
		close(fd);

		memset(path, 0, sizeof(path));
		snprintf(path, sizeof(path), "%s/led%d/delay_off", LED_BASE_PATH, index);
		if ((fd = open(path, O_WRONLY)) < 0) {
			return EFAIL;
		}

		write(fd, "300", 3);
		close(fd);
	} else {
		write(fd, "none", 4);
		close(fd);

		memset(path, 0, sizeof(path));
		snprintf(path, sizeof(path), "%s/led%d/brightness", LED_BASE_PATH, index);
		if ((fd = open(path, O_WRONLY)) < 0) {
			return EFAIL;
		}
		
		if (state == DEV_LED_ON) {
			write(fd, "1", 1); //strlen("255")+1
		} else {
			write(fd, "0", 1); //strlen("0")+1
		}
		close(fd);
	}
	
	ledste[index] = state;
	return SOK;
}

/*----------------------------------------------------------------------------
 LED all off
-----------------------------------------------------------------------------*/
void app_leds_init(void)
{
	int i, ret;
	
	//#--- create mutex
	ret = OSA_mutexCreate(&led_lock);
    OSA_assert(ret == OSA_SOK);
	
	OSA_mutexLock(&led_lock);
	for (i = 0; i < LED_IDX_ALL; i++) {
		ledste[i] = (char)-1;
		ret = leds_ctrl(i, 0);
		if (ret < 0) {
			ledste[i] = (char)-2;
		}
	}
	OSA_mutexUnlock(&led_lock);
	
	TRACE_INFO("LED init Success!\n");
}

void app_leds_exit(void)
{
	int i, ret;
	
	OSA_mutexLock(&led_lock);
	for (i = 0; i < LED_IDX_ALL; i++) {
		ledste[i] = (char)-1;
		leds_ctrl(i, 0);
	}
	OSA_mutexUnlock(&led_lock);
	
	//#--- delete mutex
	ret = OSA_mutexDelete(&led_lock);
	OSA_assert(ret == OSA_SOK);
	
	TRACE_INFO("...done!\n");
}

/*----------------------------------------------------------------------------
 LED RF Green control.
 *
 * connection OK --> GREEN
 * connection fail --> RED
 * usb not connected --> off
-----------------------------------------------------------------------------*/
int app_leds_rf_ctrl(int ste)
{
	int g_val = DEV_LED_OFF;
	int r_val = DEV_LED_OFF;
	int ret=0;

	/* don't control for update */
	if (app_cfg->ste.b.busy)
		return SOK;

	if (ste == LED_RF_OFF) {
		g_val = DEV_LED_OFF; r_val = DEV_LED_OFF;
	} else if (ste == LED_RF_OK) {
		g_val = DEV_LED_ON; r_val = DEV_LED_OFF;
	} else if (ste == LED_RF_FAIL) {
		g_val = DEV_LED_OFF; r_val = DEV_LED_ON;
	}
	
	OSA_mutexLock(&led_lock);
	ret = leds_ctrl(LED_IDX_RF_G, g_val);
	ret |= leds_ctrl(LED_IDX_RF_R, r_val);
	OSA_mutexUnlock(&led_lock);
	if (ret < 0) {
		//TRACE_ERR("invalid RF led control (%d)\n", ste);
	} else {
		//TRACE_INFO("RF led control set %d success\n", ste);
	}
	return ret;
}

#if SYS_CONFIG_GPS
/*----------------------------------------------------------------------------
 LED GPS Green control.
 *
 * connection OK --> GREEN
 * connection fail --> RED
 * usb not connected --> off
-----------------------------------------------------------------------------*/
int app_leds_gps_ctrl(int ste)
{
	int ret = 0;
	int g_val = DEV_LED_OFF;
	int r_val = DEV_LED_OFF;

	/* don't control for update */
	if (app_cfg->ste.b.busy) {
		return SOK;
	}

	if (ste == LED_GPS_OFF) {
		g_val = DEV_LED_OFF; r_val = DEV_LED_OFF;
	} else if (ste == LED_GPS_ON) {
		g_val = DEV_LED_ON; r_val = DEV_LED_OFF;
	} else if (ste == LED_GPS_FAIL) {
		g_val = DEV_LED_OFF; r_val = DEV_LED_ON;
	}
	
	OSA_mutexLock(&led_lock);
	ret = leds_ctrl(LED_IDX_GPS_G, g_val);
	ret |= leds_ctrl(LED_IDX_GPS_R, r_val);
	OSA_mutexUnlock(&led_lock);
	if (ret < 0) {
		//TRACE_INFO("invalid GPS led control (%d)\n", ste);
	} else {
		//TRACE_INFO("GPS led control set %d success\n", ste);
	}
	return ret;
}
#endif

/*----------------------------------------------------------------------------
 LED camera control. (CAM1->0, CAM2->1, CAM3->2, CAM4->3
 *
 * connection OK --> GREEN
 * connection fail --> RED
-----------------------------------------------------------------------------*/
int app_leds_cam_ctrl(int no, int ste)
{
	int ret = 0;
	int index = 0;

	/* don't control for update */
	if (app_cfg->ste.b.busy) {
		return SOK;
	}

	/*
	 * NEXXB CAM4 LED is voip led
	 */
#if defined(NEXXB) || defined(NEXXB_ONE)
	switch (no) {
	case 0:
		index = LED_IDX_CAM1; 
		break;
	case 1:
		return 0 ;
	case 2:
		index = LED_IDX_CAM3; 
		break;
	case 3:
		index = LED_IDX_CAM4; 
		break;
#else
	switch (no) {
	case 0:
		index = LED_IDX_CAM1; break;
	case 1:
		index = LED_IDX_CAM2; break;
	case 2:
		index = LED_IDX_CAM3; break;
	case 3:
		index = LED_IDX_CAM4; break;
#endif	
	default:
		TRACE_INFO("invalid camera led index (%d)\n", no);
		return -1;
	}
	
	OSA_mutexLock(&led_lock);
	ret = leds_ctrl(index, ste?DEV_LED_ON:DEV_LED_OFF);
	OSA_mutexUnlock(&led_lock);
	if (ret < 0) {
		//TRACE_INFO("Failed to control %d [th] camera led\n", index);
	} else {
		//TRACE_INFO("%d[th] Camera led control set %d success\n", index, ste);
	}
	return ret;
}

/*----------------------------------------------------------------------------
 LED VOIP control. (
 *
 * connection OK --> GREEN
 * connection busy --> BLINK
-----------------------------------------------------------------------------*/
int app_leds_voip_ctrl(int ste)
{
	int ret = 0;
	int index = 0;

	/* don't control for update */
	if (app_cfg->ste.b.busy)
		return SOK;

#if defined(NEXXB)	
	index = LED_IDX_CAM2;
#else
	index = LED_IDX_CAM4;
#endif
	OSA_mutexLock(&led_lock);
	ret = leds_ctrl(index, ste);
	OSA_mutexUnlock(&led_lock);
	if (ret < 0) {
		//TRACE_INFO("failed to control voip LED\n");
	} else {
		//TRACE_INFO("voip LED set %d state success\n", ste);
	}
	return ret;
}

/*----------------------------------------------------------------------------
 LED SD card (insert/remove) control.
 *
 * insert --> GREEN
 * remove --> RED
-----------------------------------------------------------------------------*/
int app_leds_mmc_ctrl(int ste)
{
	int ret = 0;

	/* don't control for update */
	if (app_cfg->ste.b.busy)
		return SOK;
	
	OSA_mutexLock(&led_lock);
	switch(ste) {
	case LED_MMC_RED_ON:
		ret = leds_ctrl(LED_IDX_SD_G, DEV_LED_OFF);
		ret |= leds_ctrl(LED_IDX_SD_R, DEV_LED_ON);
		break;
	case LED_MMC_RED_BLINK:
		ret = leds_ctrl(LED_IDX_SD_G, DEV_LED_OFF);
		ret |= leds_ctrl(LED_IDX_SD_R, DEV_LED_BLINK);
		break;
	case LED_MMC_GREEN_ON:
		ret = leds_ctrl(LED_IDX_SD_G, DEV_LED_ON);
		ret |= leds_ctrl(LED_IDX_SD_R, DEV_LED_OFF);
		break;
	case LED_MMC_GREEN_BLINK:
		ret = leds_ctrl(LED_IDX_SD_G, DEV_LED_BLINK);
		ret |= leds_ctrl(LED_IDX_SD_R, DEV_LED_OFF);
		break;
	case LED_MMC_ALL_OFF:
	default:
		ret = leds_ctrl(LED_IDX_SD_G, DEV_LED_OFF);
		ret |= leds_ctrl(LED_IDX_SD_R, DEV_LED_OFF);
		break;
	}
	OSA_mutexUnlock(&led_lock);
	
	if (ret < 0) {
		//TRACE_INFO("failed SD card led control!\n");
	} else {
		//TRACE_INFO("SD LED set %d state success\n", ste);
	}
	return ret;
}

/*----------------------------------------------------------------------------
 LED SD card capacity control.
 *
 * 100 ~ 67% : G3, G2, G1 on
 * 66 ~ 34%  : G2, G1 on
 * 33 ~ 0%   : G1 on
-----------------------------------------------------------------------------*/
int app_leds_mmc_capacity_ctrl(int step)
{
	int i, ret=0;

	/* don't control for update */
	if (app_cfg->ste.b.busy)
		return SOK;
	
	OSA_mutexLock(&led_lock);
	for (i = LED_IDX_SD_G1; i <= LED_IDX_SD_G3; i++) {
		leds_ctrl(i, DEV_LED_OFF);
	}

	switch(step) {
	case LED_DISK_USAGE_ON_1:
		ret = leds_ctrl(LED_IDX_SD_G1, DEV_LED_ON);
		break;
	case LED_DISK_USAGE_ON_2:
		ret = leds_ctrl(LED_IDX_SD_G1, DEV_LED_ON);
		ret |=leds_ctrl(LED_IDX_SD_G2, DEV_LED_ON);
		break;
	case LED_DISK_USAGE_ON_3:
		ret = leds_ctrl(LED_IDX_SD_G1, DEV_LED_ON);
		ret |= leds_ctrl(LED_IDX_SD_G2, DEV_LED_ON);
		ret |= leds_ctrl(LED_IDX_SD_G3, DEV_LED_ON);
		break;
	default:
		break;
	}
	OSA_mutexUnlock(&led_lock);
	
	if (ret < 0) {
		//TRACE_INFO("failed SD card led control!\n");
	} else {
		//TRACE_INFO("MMC LED set %d step success\n", step);
	}
	return ret;
}

/*----------------------------------------------------------------------------
 LED Record control.
 *
 * normal --> RED blink
 * error --> RED solid + beep
-----------------------------------------------------------------------------*/
int app_leds_rec_ctrl(int ste)
{
	int ret = 0;

	/* don't control for update */
	if (app_cfg->ste.b.busy)
		return SOK;
	
	OSA_mutexLock(&led_lock);
	if (ste == LED_REC_OFF) {
		ret = leds_ctrl(LED_IDX_REC, DEV_LED_OFF);
	} else if (ste == LED_REC_ON) {
		ret = leds_ctrl(LED_IDX_REC, DEV_LED_BLINK);
	} else if (ste == LED_REC_FAIL) {
		ret = leds_ctrl(LED_IDX_REC, DEV_LED_ON);
	} else {
		ret = -1;
	}
	OSA_mutexUnlock(&led_lock);
	
	if (ret < 0) {
		TRACE_INFO("failed REC led control!(%d)\n", ste);
	} else {
		//TRACE_INFO("REC LED set %d success\n", ste);
	}
	return ret;
}

/*----------------------------------------------------------------------------
 LED Firmware update control.
 *
 * Total Green LED blink (running)
-----------------------------------------------------------------------------*/
int app_leds_fw_update_ctrl(void)
{
	int i, ret=0;
	
	OSA_mutexLock(&led_lock);
	for (i = 0; i < LED_IDX_ALL; i++) {
		leds_ctrl(i, DEV_LED_OFF);
	}

	/* GREEN LEDS BLINK */
	ret = leds_ctrl(LED_IDX_RF_G, DEV_LED_BLINK);
	ret |= leds_ctrl(LED_IDX_GPS_G, DEV_LED_BLINK);
	ret |= leds_ctrl(LED_IDX_BAT_G1, DEV_LED_BLINK);
	ret |= leds_ctrl(LED_IDX_BAT_G2, DEV_LED_BLINK);
	ret |= leds_ctrl(LED_IDX_BAT_G3, DEV_LED_BLINK);
	ret |= leds_ctrl(LED_IDX_BAT_G4, DEV_LED_BLINK);
	ret |= leds_ctrl(LED_IDX_SD_G, DEV_LED_BLINK);
	ret |= leds_ctrl(LED_IDX_SD_G1, DEV_LED_BLINK);
	ret |= leds_ctrl(LED_IDX_SD_G2, DEV_LED_BLINK);
	ret |= leds_ctrl(LED_IDX_SD_G3, DEV_LED_BLINK);
	OSA_mutexUnlock(&led_lock);
	
	if (ret < 0) {
		TRACE_INFO("failed FW Update LED control!\n");
	} else {
		TRACE_INFO("FW Update LED success\n");
	}
	return ret;
}

/*----------------------------------------------------------------------------
 LED internal battery capacity
 *
 * 0 ~ 25% (solid1)
-----------------------------------------------------------------------------*/
int app_leds_int_batt_ctrl(int step)
{
	int i, ret=0;

	/* don't control for update */
	if (app_cfg->ste.b.busy)
		return SOK;
	
	OSA_mutexLock(&led_lock);
	for (i = LED_IDX_BAT_G1; i <= LED_IDX_BAT_G4; i++) {
		leds_ctrl(i, DEV_LED_OFF);
	}

	switch(step) {
	case 0:
		ret = leds_ctrl(LED_IDX_BAT_G1, DEV_LED_ON);
		break;
	case 1:
		ret = leds_ctrl(LED_IDX_BAT_G1, DEV_LED_ON);
		ret |= leds_ctrl(LED_IDX_BAT_G2, DEV_LED_ON);
		break;
	case 2:
		ret = leds_ctrl(LED_IDX_BAT_G1, DEV_LED_ON);
		ret |= leds_ctrl(LED_IDX_BAT_G2, DEV_LED_ON);
		ret |= leds_ctrl(LED_IDX_BAT_G3, DEV_LED_ON);
		break;
	case 3:
		ret = leds_ctrl(LED_IDX_BAT_G1, DEV_LED_ON);
		ret |= leds_ctrl(LED_IDX_BAT_G2, DEV_LED_ON);
		ret |= leds_ctrl(LED_IDX_BAT_G3, DEV_LED_ON);
		ret |= leds_ctrl(LED_IDX_BAT_G4, DEV_LED_ON);
		break;
	default:
		break;
	}
	OSA_mutexUnlock(&led_lock);
	
	if (ret < 0) {
		//TRACE_INFO("failed BATT LED control!\n");
	} else {
		//TRACE_INFO("BATT LED set %d step success\n", step);
	}
	return ret;
}

/*----------------------------------------------------------------------------
 LED backup state control.
 *
 * ftp sending --> blink
 * ftp --> off
-----------------------------------------------------------------------------*/
int app_leds_backup_state_ctrl(int ste)
{
	int ret = 0;
	int val = DEV_LED_OFF;

	/* don't control for update */
	if (app_cfg->ste.b.busy)
		return SOK;

	if (ste == LED_FTP_ON) {
		val = DEV_LED_BLINK;
	}
	
	OSA_mutexLock(&led_lock);
	ret = leds_ctrl(LED_IDX_BACKUP, val);
	OSA_mutexUnlock(&led_lock);
	if (ret < 0) {
		//TRACE_INFO("invalid backup state led control (%d)\n", ste);
	} else {
		//TRACE_INFO("BACKUP LED set %d success\n", ste);
	}
	return ret;
}

/*----------------------------------------------------------------------------
  LED system normal 
  *
-----------------------------------------------------------------------------*/
void app_leds_sys_normal_ctrl(void)
{
    int i,state;
	
	OSA_mutexLock(&led_lock);
	for (i = LED_IDX_CAM1; i < LED_IDX_SD_G; i++)
		leds_ctrl(i, DEV_LED_ON);
	OSA_mutexUnlock(&led_lock);
	
    if((state = get_write_status()))
	{
		switch(state)
		{
			case FILE_STATE_OVERWRITE :
				if(app_rec_state())
				    app_leds_mmc_ctrl(LED_MMC_GREEN_BLINK) ;
                else
				    app_leds_mmc_ctrl(LED_MMC_GREEN_ON) ;
				break ;

			case FILE_STATE_FULL :
				if(app_cfg->ste.b.disk_full == OFF)
				{
				    app_leds_mmc_ctrl(LED_MMC_RED_ON) ;
					app_cfg->ste.b.disk_full = ON ;
                } 
				break ;

			default :
				app_leds_mmc_ctrl(LED_MMC_GREEN_ON) ;
				break ;
		}
	}

#if SYS_CONFIG_GPS	
    app_leds_gps_ctrl(LED_GPS_OFF) ;
#endif
    app_file_update_disk_usage();
	OSA_mutexLock(&led_lock);
	if(app_rec_state())
        leds_ctrl(LED_IDX_REC, DEV_LED_BLINK);
	else
        leds_ctrl(LED_IDX_REC, DEV_LED_OFF);
	OSA_mutexUnlock(&led_lock);
}

/*----------------------------------------------------------------------------
 LED system error control
 *
-----------------------------------------------------------------------------*/
void app_leds_sys_error_ctrl(void)
{
	int i;
	
	OSA_mutexLock(&led_lock);
	for (i = 0; i < LED_IDX_ALL; i++)
		leds_ctrl(i, DEV_LED_OFF);

	/* GREEN LEDS BLINK */
	leds_ctrl(LED_IDX_RF_R, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_GPS_R, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_SD_R, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_REC, DEV_LED_BLINK);
	OSA_mutexUnlock(&led_lock);
}

/*----------------------------------------------------------------------------
 LED ETH Status control.
 * TODO
-----------------------------------------------------------------------------*/
int app_leds_eth_status_ctrl(int ste)
{
	int ret = 0;
	int val = DEV_LED_OFF;

	/* don't control for update */
	if (app_cfg->ste.b.busy)
		return SOK;

    if (ste == LED_FTP_ON) {
		val = DEV_LED_ON;
	}
    if (ste == LED_FTP_ERROR) {
		val = DEV_LED_BLINK;
	}
	
	OSA_mutexLock(&led_lock);
	ret = leds_ctrl(LED_IDX_ETH_STATUS, val);
	OSA_mutexUnlock(&led_lock);
	if (ret < 0) {
		//TRACE_INFO("invalid eth status led control (%d)\n", ste);
	} else {
		//TRACE_INFO("eth status led control (%d) success\n", ste);
	}
	return ret;
}
