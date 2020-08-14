/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_ctrl.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_CTRL_H_
#define _APP_CTRL_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include "ti_vdis.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define DISP_HDMI	VDIS_DEV_HDMI
#define DISP_LCD	VDIS_DEV_DVO2
#define DISP_TVO	VDIS_DEV_SD

typedef enum {
        TYPE_BASIC,
        TYPE_WIRELESS,
        TYPE_MAX
} app_update_ctrl_e;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int ctrl_swms_set(int dev, int win_num, int ch);
int ctrl_vid_bitrate(int ch, int bitrate);              //# runtime
int ctrl_vid_framerate(int ch, int framerate);          //# runtime  
int ctrl_vid_rate(int ch, int rc, int br);		//# runtime
int ctrl_vid_gop_set(int ch, int gop) ;                 //# runtime 
int ctrl_jpg_quality(int ch, int value);		//# runtime
int ctrl_vid_resolution(int resol) ;                    //# runtime 

int ctrl_set_port(int http_port, int https_port, int rtsp_port) ; //# runtime
int ctrl_set_NTP(char *server_addr) ;
int ctrl_set_DNS(char *server_addr) ;

int ctrl_get_hw_version(char *version);
int ctrl_get_mcu_version(char *version);
int ctrl_mmc_check_exfat(unsigned long *size);
int ctrl_mmc_exfat_format(unsigned long size);
int ctrl_mmc_check_partitions(void);
int ctrl_mmc_check_fsck(void);
int ctrl_mmc_run_fsck(void);
void ctrl_swosd_enable(int idx, int ch, int draw);
void ctrl_swosd_userstr(char *str, int draw);
int ctrl_time_set(int year, int mon, int day, int hour, int min, int sec);

int ctrl_sw_update(char *disk);
void ctrl_reset_nand_update(void);
int ctrl_enc_multislice(void) ;
int ctrl_get_resolution(void) ;
void fitt360_reboot() ;

int ctrl_update_firmware(char *fwpath, char *disk);
int ctrl_update_firmware_by_cgi(char *path);

#endif	/* _APP_CTRL_H_ */
