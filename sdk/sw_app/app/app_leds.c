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
#define LED_BLINK_MS		300

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
static pthread_mutex_t leds_lock = PTHREAD_MUTEX_INITIALIZER;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

static int leds_ctrl(int index, int state)
{
	char path[256] = {0,};
	char buf[16];
	int fd, len, brightness;
	int ret = 0;

	if (ledste[index] == state)
		return 0;

	pthread_mutex_lock(&leds_lock);

	ledste[index] = state;

	buf[sizeof(buf) - 1] = '\0';
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), "%s/led%d/trigger", LED_BASE_PATH, index);
	if ((fd = open(path, O_WRONLY)) < 0) {
		//eprintf("Error open %s\n", path);
		ret = -1;
		goto err_exit;
	}

	if (state == DEV_LED_BLINK) {
		len = snprintf(buf, sizeof(buf), "timer");
		write(fd, buf, len);
		close(fd);

		memset(path, 0, sizeof(path));
		snprintf(path, sizeof(path), "%s/led%d/delay_on", LED_BASE_PATH, index);
		if ((fd = open(path, O_WRONLY)) < 0) {
			//eprintf("Error open %s\n", path);
			ret = -1;
			goto err_exit;
		}

		buf[sizeof(buf) - 1] = '\0';
		len = snprintf(buf, sizeof(buf), "%d", LED_BLINK_MS);
		write(fd, buf, len);
		close(fd);

		memset(path, 0, sizeof(path));
		snprintf(path, sizeof(path), "%s/led%d/delay_off", LED_BASE_PATH, index);
		if ((fd = open(path, O_WRONLY)) < 0) {
			//eprintf("Error open %s\n", path);
			ret = -1;
			goto err_exit;
		}

		buf[sizeof(buf) - 1] = '\0';
		len = snprintf(buf, sizeof(buf), "%d", LED_BLINK_MS);
		write(fd, buf, len);
		close(fd);

	} else {
		if (state == DEV_LED_ON) brightness = 255;
		else					 brightness = 0;

		len = snprintf(buf, sizeof(buf), "none");
		write(fd, buf, len);
		close(fd);

		memset(path, 0, sizeof(path));
		snprintf(path, sizeof(path), "%s/led%d/brightness", LED_BASE_PATH, index);
		if ((fd = open(path, O_WRONLY)) < 0) {
			//eprintf("Error open %s\n", path);
			ret = -1;
			goto err_exit;
		}

		buf[sizeof(buf) - 1] = '\0';
		len = snprintf(buf, sizeof(buf), "%d", brightness);
		write(fd, buf, len);
		close(fd);
	}

err_exit:
	pthread_mutex_unlock(&leds_lock);

	return ret;
}

/*----------------------------------------------------------------------------
 LED all off
-----------------------------------------------------------------------------*/
int app_leds_init(void)
{
	int i, ret;
	for (i = 0; i < LED_IDX_ALL; i++) {
		ledste[i] = (char)-1;
		ret = leds_ctrl(i, 0);
		if (ret < 0 ) {
			//eprintf("%d led error!\n", i);
			return -1;
		}
	}
	
	aprintf("... done!\n");
	
	return SOK;
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
	int ret = 0;
	int g_val = DEV_LED_OFF;
	int r_val = DEV_LED_OFF;

	/* don't control for update */
	if (app_cfg->ste.b.busy) {
		return 0;
	}

	if (ste == LED_RF_OFF) {
		g_val = DEV_LED_OFF; r_val = DEV_LED_OFF;
	} else if (ste == LED_RF_OK) {
		g_val = DEV_LED_ON; r_val = DEV_LED_OFF;
	} else if (ste == LED_RF_FAIL) {
		g_val = DEV_LED_OFF; r_val = DEV_LED_ON;
	}

	ret = leds_ctrl(LED_IDX_RF_G, g_val);
	ret |= leds_ctrl(LED_IDX_RF_R, r_val);
	if (ret < 0) {
		//eprintf("invalid RF led control (%d)\n", ste);
	}

	return ret;
}

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
		return 0;
	}

	if (ste == LED_GPS_OFF) {
		g_val = DEV_LED_OFF; r_val = DEV_LED_OFF;
	} else if (ste == LED_GPS_ON) {
		g_val = DEV_LED_ON; r_val = DEV_LED_OFF;
	} else if (ste == LED_GPS_FAIL) {
		g_val = DEV_LED_OFF; r_val = DEV_LED_ON;
	}

	ret = leds_ctrl(LED_IDX_GPS_G, g_val);
	ret |= leds_ctrl(LED_IDX_GPS_R, r_val);
	if (ret < 0) {
		//eprintf("invalid GPS led control (%d)\n", ste);
	}

	return ret;
}

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
		return 0;
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
		eprintf("invalid camera led index (%d)\n", no);
		return -1;
	}

	ret = leds_ctrl(index, ste?DEV_LED_ON:DEV_LED_OFF);
	if (ret < 0) {
		eprintf("failed camera led control (%d)\n", index);
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
	if (app_cfg->ste.b.busy) {
		return 0;
	}

#if defined(NEXXB)	
	index = LED_IDX_CAM2;
#else
	index = LED_IDX_CAM4;
#endif	
	ret = leds_ctrl(index, ste);
	if (ret < 0) {
		//eprintf("failed voip led control (%d)\n", index);
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
	if (app_cfg->ste.b.busy) {
		return 0;
	}

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

	if (ret < 0) {
		//eprintf("failed SD card led control!\n");
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
	int i;

	/* don't control for update */
	if (app_cfg->ste.b.busy) {
		return 0;
	}

	for (i = LED_IDX_SD_G1; i <= LED_IDX_SD_G3; i++) {
		leds_ctrl(i, DEV_LED_OFF);
	}

	switch(step) {
	case LED_DISK_USAGE_ON_1:
		leds_ctrl(LED_IDX_SD_G1, DEV_LED_ON);
		break;
	case LED_DISK_USAGE_ON_2:
		leds_ctrl(LED_IDX_SD_G1, DEV_LED_ON);
		leds_ctrl(LED_IDX_SD_G2, DEV_LED_ON);
		break;
	case LED_DISK_USAGE_ON_3:
		leds_ctrl(LED_IDX_SD_G1, DEV_LED_ON);
		leds_ctrl(LED_IDX_SD_G2, DEV_LED_ON);
		leds_ctrl(LED_IDX_SD_G3, DEV_LED_ON);
		break;
	default:
		//eprintf("invalid mmc capacity led(%d)!\n", step);
		break;
	}

	return 0;
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
	if (app_cfg->ste.b.busy) {
		return 0;
	}

	if (ste == LED_REC_OFF) {
		ret = leds_ctrl(LED_IDX_REC, DEV_LED_OFF);
	} else if (ste == LED_REC_ON) {
		ret = leds_ctrl(LED_IDX_REC, DEV_LED_BLINK);
	} else if (ste == LED_REC_FAIL) {
		ret = leds_ctrl(LED_IDX_REC, DEV_LED_ON);
	} else {
		ret = -1;
	}

	if (ret < 0) {
		//eprintf("failed REC led control!(%d)\n", ste);
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
	int i;

	for (i = 0; i < LED_IDX_ALL; i++) {
		leds_ctrl(i, DEV_LED_OFF);
	}

	/* GREEN LEDS BLINK */
	leds_ctrl(LED_IDX_RF_G, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_GPS_G, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_BAT_G1, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_BAT_G2, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_BAT_G3, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_BAT_G4, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_SD_G, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_SD_G1, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_SD_G2, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_SD_G3, DEV_LED_BLINK);

	return 0;
}

/*----------------------------------------------------------------------------
 LED internal battery capacity
 *
 * 0 ~ 25% (solid1)
-----------------------------------------------------------------------------*/
int app_leds_int_batt_ctrl(int step)
{
	int i;

	/* don't control for update */
	if (app_cfg->ste.b.busy) {
		return 0;
	}

	for (i = LED_IDX_BAT_G1; i <= LED_IDX_BAT_G4; i++) {
		leds_ctrl(i, DEV_LED_OFF);
	}

	switch(step) {
	case 0:
		leds_ctrl(LED_IDX_BAT_G1, DEV_LED_ON);
		break;
	case 1:
		leds_ctrl(LED_IDX_BAT_G1, DEV_LED_ON);
		leds_ctrl(LED_IDX_BAT_G2, DEV_LED_ON);
		break;
	case 2:
		leds_ctrl(LED_IDX_BAT_G1, DEV_LED_ON);
		leds_ctrl(LED_IDX_BAT_G2, DEV_LED_ON);
		leds_ctrl(LED_IDX_BAT_G3, DEV_LED_ON);
		break;
	case 3:
		leds_ctrl(LED_IDX_BAT_G1, DEV_LED_ON);
		leds_ctrl(LED_IDX_BAT_G2, DEV_LED_ON);
		leds_ctrl(LED_IDX_BAT_G3, DEV_LED_ON);
		leds_ctrl(LED_IDX_BAT_G4, DEV_LED_ON);
		break;
	default:
		//eprintf("invalid battery led(%d)!\n", step);
		break;
	}

	return 0;
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
		return 0;

	if (ste == LED_FTP_ON) {
		val = DEV_LED_BLINK;
	}

	ret = leds_ctrl(LED_IDX_BACKUP, val);
	if (ret < 0) {
		//eprintf("invalid backup state led control (%d)\n", ste);
	}

	return ret;
}

/*----------------------------------------------------------------------------
  LED system normal 
  *
-----------------------------------------------------------------------------*/
int app_leds_sys_normal_ctrl(void)
{
    int i;
    int state ;

	for (i = LED_IDX_CAM1; i < LED_IDX_SD_G; i++)
	{
		leds_ctrl(i, DEV_LED_ON);
	}

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
    app_leds_gps_ctrl(LED_GPS_OFF) ;

    app_file_update_disk_usage() ;
	if(app_rec_state())
        leds_ctrl(LED_IDX_REC, DEV_LED_BLINK);
	else
        leds_ctrl(LED_IDX_REC, DEV_LED_OFF);

/*
 *  move to netmgr.c 
	if (app_cfg->ste.b.usbnet_run)
	{
        g_val = DEV_LED_ON;
		r_val = DEV_LED_OFF ;
	    leds_ctrl(LED_IDX_RF_G, g_val) ;
	    leds_ctrl(LED_IDX_RF_R, r_val) ;
	}
	else
	{
        g_val = DEV_LED_OFF;
		r_val = DEV_LED_OFF ;
	    leds_ctrl(LED_IDX_RF_G, g_val) ;
	    leds_ctrl(LED_IDX_RF_R, r_val) ;
	}
*/
	return 0 ;
}

/*----------------------------------------------------------------------------
 LED system error control
 *
-----------------------------------------------------------------------------*/
int app_leds_sys_error_ctrl(void)
{
	int i;

	for (i = 0; i < LED_IDX_ALL; i++) {
		leds_ctrl(i, DEV_LED_OFF);
	}

	/* GREEN LEDS BLINK */
	leds_ctrl(LED_IDX_RF_R, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_GPS_R, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_SD_R, DEV_LED_BLINK);
	leds_ctrl(LED_IDX_REC, DEV_LED_BLINK);

	return 0;
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
		return 0;

    if (ste == LED_FTP_ON) {
		val = DEV_LED_ON;
	}
    if (ste == LED_FTP_ERROR) {
		val = DEV_LED_BLINK;
	}


	ret = leds_ctrl(LED_IDX_ETH_STATUS, val);
	if (ret < 0) {
		//eprintf("invalid eth status led control (%d)\n", ste);
	}

	return ret;

}
