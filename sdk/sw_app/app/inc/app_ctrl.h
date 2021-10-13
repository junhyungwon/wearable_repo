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
	TYPE_BASIC = 0,
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
int ctrl_full_vid_setting(int ch, int resol, int bitrate, int fps, int gop) ;

int ctrl_set_port(int http_port, int https_port, int rtsp_port) ; //# runtime
int ctrl_set_network(int net_type, const char *token, const char *ipaddr, const char *subnet) ;
int ctrl_set_gateway(const char *gw) ;
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
void ctrl_swosd_callstatus(int ch, int draw);
void ctrl_swosd_userstr(char *str, int draw);
int ctrl_time_set(int year, int mon, int day, int hour, int min, int sec);

void ctrl_reset_nand_update(void);
int ctrl_enc_multislice(void) ;
int ctrl_get_resolution(void) ;

int ctrl_update_firmware_by_cgi(char *path);
int ctrl_is_live_process(const char *process_name);
void ctrl_firmware_update(void);
void ctrl_sys_halt(int shutdown);

#endif	/* _APP_CTRL_H_ */
