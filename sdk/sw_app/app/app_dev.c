/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_dev.c
 * @brief   device control routine
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "dev_gpio.h"
#include "dev_accel.h"
#include "dev_buzzer.h"
#include "dev_common.h"
#include "dev_disk.h"
#include "dev_gps.h"
#include "dev_micom.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_rec.h"
#include "app_ctrl.h"
#include "app_dev.h"
#include "app_set.h"
#include "app_hotplug.h"
#include "app_wcli.h"
#include "app_ftp.h"
#include "app_fms.h"
#include "ti_vcap.h"
#include "app_mcu.h"
#include "app_gui.h"
#include "app_process.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define EN_REC_META			1
#define EN_GSN				0
#define EN_GPS				1

#define TIME_DEV_CYCLE		100		//# msec
#define TIME_GSN_CYCLE		250		//# msec
#define TIME_GPS_CYCLE		500		//# msec

#define GSENS_CYCLE_TIME    100     //# g-sensor
#define DEV_CYCLE_TIME      500     //# temp

#define GSENS_FIRST_DELAY_TIME  2000
#define GSENS_FIRST_DELAY_CNT   ( GSENS_FIRST_DELAY_TIME / GSENS_CYCLE_TIME )

#define FITTMETA_CYCLE_TIME  1000
#define TIME_META_SENDING    60000   //# 1Minute
#define CNT_FITTMETA        (TIME_META_SENDING/FITTMETA_CYCLE_TIME)

#define LSG                 (255.0) //-128 ~ 127    offset 0.064 (+/- 8G)

float SENSITIVITY_TABLE[10] = {1.2, 0.9, 0.6, 0.3, 0.1};                   //# driving mode

typedef struct {
	app_thr_obj devObj;		//# dev thread
	app_thr_obj gsnObj;		//# g-sensor thread
    app_thr_obj metaObj;   	//# Meta send thread for FMS
    app_thr_obj gpsObj;		//# gps thread

    gps_rmc_t gps;

    OSA_MutexHndl mutex_gps;
    OSA_MutexHndl mutex_gsn;
    OSA_MutexHndl mutex_meta;

    int old_evt_state;
} app_dev_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_dev_t t_dev;
static app_dev_t *idev=&t_dev;

static char fitt_meta_str[META_REC_TOTAL];
static app_gvalue_t fitt_level;
static int fitt_cnt = 0;

static app_gs_t gs_meta[GSENSOR_CNT_MAX];
static int gs_cnt = 0;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 gpio init/exit functions
-----------------------------------------------------------------------------*/
static void dev_gpio_init(void)
{
	gpio_input_init(GPS_PWR_EN);

	gpio_input_init(REC_KEY);
	gpio_input_init(BACKUP_DET);
	gpio_input_init(USB_DET);
}

static void dev_gpio_exit(void)
{
	gpio_exit(GPS_PWR_EN);

	gpio_exit(REC_KEY);
	gpio_exit(BACKUP_DET);
	gpio_exit(USB_DET);
}

/*----------------------------------------------------------------------------
 input check functions
-----------------------------------------------------------------------------*/
/*****************************************************************************
* @brief    check time function
* @section  [return] 1: exist file, 0:no file
*****************************************************************************/
static int _chk_time_file(void)
{
	FILE *fp;
	char *file = "/mmc/fitt_time.txt";
	char line[1024], prefix[8]={0,}, date[64]={0,}, time[64]={0,};
	char cmd[128];

	//# time update file check
	if(-1 == access(file, 0)) {
		return 0;
	}

	//# change time
	fp = fopen(file, "r");
	if (fp == NULL) {
		eprintf("cannot open %s\n", file);
		return 0;
	}

	while (fgets(line, sizeof(line), fp)) {
		sscanf(line, "%s %s %s", prefix, date, time);
		if(!strcmp(prefix, "time")) {
			sprintf(cmd, "date -s \"%s %s\"", date, time);
			system(cmd);
			util_sys_exec("hwclock -w");
			break;
		}
	}

	fclose(fp);

	//# delete time file
	sprintf(cmd, "rm -rf %s", file);
	system(cmd);
	sync();
	app_msleep(100);

	return 1;
}

int dev_ste_mmc(void)
{
	int status;

	status = dev_disk_check_mount(MMC_MOUNT_POINT);
	//dprintf("--- value %d\n", status);

	if (status) {
		_chk_time_file();
	}

	return status;
}

int dev_ste_key(int gio)
{
	int status;

	gpio_get_value(gio, &status);
	//dprintf("--- [key] value %d\n", status);

	return status;
}

int dev_ste_cradle(void)
{
	int status;

	gpio_get_value(BACKUP_DET, &status);
	//dprintf("--- [cradle] value %d\n", status);

	return status;
}

int dev_ste_usbcradle(void)
{
	int status;

	gpio_get_value(USB_DET, &status);
	//dprintf("--- [cradle] value %d\n", status);

	return status;
}

int dev_ste_gps_port(void)
{
	int status;

	gpio_get_value(GPS_PWR_EN, &status);
	//dprintf("--- [gps port] value %d\n", status);

	return status;
}

int dev_ste_ethernet(int devnum)
{
    char netdevice[32] = {0, } ;
    int status=0 ;

    unsigned char network ;
    FILE *fp ;

    if(devnum)
        sprintf(netdevice, "/sys/class/net/%s/carrier",DEV_NET_NAME_ETH1) ;
    else
        sprintf(netdevice, "/sys/class/net/%s/carrier",DEV_NET_NAME_ETH) ;

    fp = fopen(netdevice, "r") ;
  
    if(fp != NULL)
    {   
        fread(&network, 1, 1, fp) ;

        if(network == '1')
        {
            status = 1 ; // connect
        }
        else
        { 
            status = 0 ; // disconnect
        } 
        fclose(fp) ;
    }
    return status ;
}


/*****************************************************************************
* @brief    buzzer control
* @section  [param] time: ms
*****************************************************************************/
void dev_buzz_ctrl(int time, int cnt)
{
	while (1)
	{
		dev_buzzer_enable(1);
		app_msleep(time);
		dev_buzzer_enable(0);
		cnt--;
		if(cnt == 0) {
			break;
		}
		app_msleep(time);
	}
}

static void gs_meta_reset(void)
{
    gs_cnt = 0;
    memset(gs_meta, 0, (sizeof(app_gs_t)*GSENSOR_CNT_MAX));
}

static void gs_meta_add(accel_data_t* acc)
{
    OSA_mutexLock(&idev->mutex_gsn);

    if(gs_cnt >= GSENSOR_CNT_MAX)
         gs_meta_reset();

    struct timeval tv;
    gettimeofday(&tv, NULL);

    gs_meta[gs_cnt].acc_sec       = tv.tv_sec;
    gs_meta[gs_cnt].acc_msec      = tv.tv_usec/1000;
    gs_meta[gs_cnt].acc_level.x   = acc->x;
    gs_meta[gs_cnt].acc_level.y   = acc->z;
    gs_meta[gs_cnt].acc_level.z   = acc->y;

    gs_cnt++;
    OSA_mutexUnlock(&idev->mutex_gsn);
}

static int cnt_key=0, ste_key=KEY_NONE;
static int chk_rec_key(void)
{
	int key = dev_ste_key(REC_KEY);

	if(ste_key != KEY_NONE) {
		if(key == 1) {
			ste_key = KEY_NONE;
		}
		return KEY_NONE;
	}

	if(cnt_key)
	{
		if(key == 0) {	//# press
			cnt_key--;
			if(cnt_key == 0) {
				ste_key = KEY_LONG;
			}
		}
		else {
			cnt_key = 0;
			ste_key = KEY_SHORT;
		}
	}
	else {
		if(ste_key == KEY_NONE) {
			if(key == 0) {
				cnt_key = 20;		//# 1sec
			}
		}
	}

	return ste_key;
}

/*----------------------------------------------------------------------------
 gps functions
-----------------------------------------------------------------------------*/
static int app_sys_time(struct tm *ts)
{
	time_t now, set;

	int timezone = app_set->time_info.time_zone - 12;

    //# get current time
	now = time(NULL);

	//# get set time
	set = mktime(ts) + (timezone*3600);

	//# if difference 1min.
	if(abs(now-set) > 60)
	{
		stime(&set);
		Vsys_datetime_init();	//# m3 Date/Time init
    	app_msleep(100);
		if (dev_rtc_set_time(*ts) < 0) {
			eprintf("Failed to set system time to rtc\n!!!");
		}

		aprintf("--- changed time from GPS ---\n");
	}

	return SOK;
}

#if 0
static void fitt_get_gs(app_gvalue_t* plevel)
{
    plevel->gx = fitt_level.gx;
    plevel->gy = fitt_level.gy;
    plevel->gz = fitt_level.gz;
}
#endif

static void fitt_meta_reset(void)
{
    fitt_cnt = 0;
    memset(fitt_meta_str, 0, META_REC_TOTAL);
}

static void fitt_meta_add(app_extmeta_t *pdata)
{
    OSA_mutexLock(&idev->mutex_meta);

    struct timeval tv;
    struct tm ts;
    time_t now;
    char str_buf[30];
    int cur_idx = 0;

    cur_idx = strlen(fitt_meta_str);
    //# save add time
    gettimeofday(&tv, NULL);
    now = tv.tv_sec;
    localtime_r(&now, &ts);

    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = '{';

    strftime(str_buf, sizeof(str_buf), "%Y-%2m-%2d %2H:%2M:%2S", &ts);

    sprintf(&fitt_meta_str[cur_idx], "\"%s\"", "date");
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ':';

    sprintf(&fitt_meta_str[cur_idx], "\"%s\"", str_buf);
    cur_idx = strlen(fitt_meta_str);

    sprintf(&fitt_meta_str[cur_idx], ".%ld", tv.tv_usec/100000);
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ',';

    sprintf(&fitt_meta_str[cur_idx], "\"%s\"", "gps");
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ':';

    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = '{';


    sprintf(&fitt_meta_str[cur_idx++], "\"%s\"", "enabled");
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ':';

    //# gps data
    if(pdata->gps.enable)
    {
        sprintf(&fitt_meta_str[cur_idx++], "%s", "true");
        cur_idx = strlen(fitt_meta_str);
        fitt_meta_str[cur_idx++] = ',';

    }
    else
    {
        sprintf(&fitt_meta_str[cur_idx++], "%s", "false");
        cur_idx = strlen(fitt_meta_str);
        fitt_meta_str[cur_idx++] = ',';

    }

    sprintf(&fitt_meta_str[cur_idx++], "\"%s\"", "time");
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ':';

    app_time_t gtime;
    memset(&gtime, 0, sizeof(gtime));

    if (pdata->gps.enable)//pdata->gps.set & G_TIME_SET)
    {
        gtime.year          = (pdata->gps.utc_year + 1900);
        gtime.mon           = pdata->gps.utc_month;
        gtime.day           = pdata->gps.utc_mday;
        gtime.hour          = (pdata->gps.utc_hour + TIME_ZONE);
        gtime.min           = pdata->gps.utc_min;
        gtime.sec           = pdata->gps.utc_sec;
        gtime.subseconds    = 0;
    }

    sprintf(&fitt_meta_str[cur_idx],"\"%04d-%02d-%02d %02d:%02d:%02.2f\"", gtime.year,
                                                                gtime.mon+1,
                                                                gtime.day,
                                                                gtime.hour,
                                                                gtime.min,
                                                                gtime.sec + pdata->gps.subsec);
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ',';

    sprintf(&fitt_meta_str[cur_idx++], "\"%s\"", "lat");
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ':';

    /* latitude->double number */
    sprintf(&fitt_meta_str[cur_idx], "%f", pdata->gps.lat);
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ',';

    sprintf(&fitt_meta_str[cur_idx++], "\"%s\"", "lot");
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ':';

    sprintf(&fitt_meta_str[cur_idx], "%f", pdata->gps.lot);
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ',';

    sprintf(&fitt_meta_str[cur_idx++], "\"%s\"", "dir");
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ':';

    sprintf(&fitt_meta_str[cur_idx], "%3.2f", pdata->gps.dir);
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = '}';

    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ',';
/*
    sprintf(&fitt_meta_str[cur_idx++], "\"%s\"", "gyro");
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ':';

    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = '[';

    //# G-sensor data
    sprintf(&fitt_meta_str[cur_idx], "%1.2f", pdata->acc_level.gx);
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ',';

    sprintf(&fitt_meta_str[cur_idx], "%1.2f", pdata->acc_level.gy);
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ',';

    sprintf(&fitt_meta_str[cur_idx], "%1.2f", pdata->acc_level.gz);
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ']';

    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = ',';
*/
    sprintf(&fitt_meta_str[cur_idx++], "\"%s\"", "battery");
    cur_idx = strlen(fitt_meta_str);
    fitt_meta_str[cur_idx++] = '9';  // exam..
    fitt_meta_str[cur_idx++] = '0';  // exam..

    OSA_mutexUnlock(&idev->mutex_meta);
}

/*****************************************************************************
 * @brief    get device data
 * @section  DESC Description
 *   - desc   POWER_HOLD pin to Low(Power OFF)
 *****************************************************************************/
void dev_get_gps_rmc(gps_rmc_t *gps)
{
	OSA_mutexLock(&idev->mutex_gps);

	memcpy((void *)gps, &idev->gps, sizeof(gps_rmc_t));

	OSA_mutexUnlock(&idev->mutex_gps);
}

/*****************************************************************************
* @brief    dev thread function
* @section  [desc]
*****************************************************************************/
static void *THR_dev(void *prm)
{
    app_thr_obj *tObj = &idev->devObj;
	int ret, exit=0;
	int mmc, rkey, cmd;

	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		cmd = tObj->cmd;
		if (cmd == APP_CMD_STOP || app_cfg->ste.b.pwr_off) {
			break;
		}

        if(app_cfg->ste.b.cap)
        {
        	//# check mmc card
			mmc = dev_ste_mmc();
			if(mmc != app_cfg->ste.b.mmc) {
				app_cfg->ste.b.mmc = mmc;
				dprintf("SD Card %s\n", mmc?"insert":"remove");
				aprintf("done! will restart\n");
				app_rec_stop(0);
//				mic_exit_state(OFF_RESET, 0);
//				app_main_ctrl(APP_CMD_EXIT, 0, 0);
				mcu_pwr_off(OFF_RESET);
			}

		    rkey = chk_rec_key();
		    if(rkey == KEY_SHORT) 
            {			//# record start/stop
                if(!app_cfg->ste.b.ftp_run) 
                {     
			        if(app_rec_state()) 
                    {
				        app_rec_stop(1);
			        } 
                    else  
                    {
				        app_rec_start();
			        }
                }
		    } else if(rkey == KEY_LONG) 
			{	//# sw update
                if(!app_cfg->ste.b.ftp_run)
				{
			        dev_buzz_ctrl(50, 3);		//# buzz: update
			 	    //# 업데이트 파일명이 비정상적인 경우를 제외하고는 
                    //# 무조건 Reboot를 하기 위해서 위치를 이곳으로 변경함. 
                    if (ctrl_sw_update(SD_MOUNT_PATH) == 0) {
                        //# for micom exit..
                        //	mic_exit_state(OFF_RESET, 0);
                        //	app_main_ctrl(APP_CMD_EXIT, 0, 0);
                        mcu_pwr_off(OFF_RESET);
                     }
                }     
		    }
        }

		//# check cradle state
        if (!dev_ste_cradle()) 
        {
            if (app_cfg->ftp_enable == OFF) 
            {
                util_set_net_info(DEV_NET_NAME_ETH); //# "eth0"
                app_cfg->ste.b.st_cradle = 1 ;
                app_cfg->ftp_enable = ON;
            } 
        } 
        else 
        {
            if(app_cfg->ste.b.st_cradle)
            {
                app_cfg->ste.b.st_cradle = 0 ;
                dev_net_link_down("eth0") ;
            }
            app_cfg->ftp_enable = OFF;
        }

        if(dev_ste_ethernet(0))
		{
            if(!strcmp(app_set->net_info.eth_ipaddr, "0.0.0.0") && app_set->net_info.type != NET_TYPE_STATIC) 
			{
                util_set_net_info(DEV_NET_NAME_ETH); //# "eth0"
			}
		}

		app_msleep(TIME_DEV_CYCLE);
	}

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    dev thread function
* @section  [desc]
*****************************************************************************/
void *THR_gsn(void *prm)
{
	app_thr_obj *tObj = &idev->gsnObj;
    int cmd, exit = 0;
    int first=GSENS_FIRST_DELAY_CNT, sensing_time = 0;
    accel_data_t acc, level;
    float impact, threshold = 30.0;
    accel_data_t acc_obj;
    int ret = 0;

    memset(&acc, 0, sizeof(accel_data_t));
    gs_meta_reset();

    ret = dev_accel_init();
    if (ret < 0) {
        eprintf("Gsensor init\n");
        app_log_write( MSG_LOG_WRITE, "[APP_DEV] !!! G-Sensor Initialize failed !!!" );
        goto gsn_exit;
    }
    dev_accel_start();

	aprintf("enter...\n");
	tObj->active = 1;

    while (!exit)
    {
        cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP || app_cfg->ste.b.pwr_off) {
            break;
        }

        if (app_cfg->ste.b.cap)
        {
            /* get accelerator sensor value*/
            if(dev_accel_read(&acc_obj, 100) == 0)
            {
                if (first > 0) {
                    acc.x = acc_obj.x;
                    acc.y = acc_obj.y;
                    acc.z = acc_obj.z;
                    first--;
                }
                else
                {
                    level.x = abs(acc_obj.x - acc.x);
                    level.y = abs(acc_obj.y - acc.y);
                    level.z = abs(acc_obj.z - acc.z);

                    fitt_level.gx = (float)acc_obj.x/LSG;
                    fitt_level.gy = (float)acc_obj.z/LSG;
                    fitt_level.gz = (float)acc_obj.y/LSG;

                    if (level.x > level.y)
                        impact = level.x;
                    else
                        impact = level.y;
                    if (level.z > impact)
                        impact = level.z;

                    if (app_set->wd.gsn < GSN_IDX_OFF) {
                        //#--- accident!!
                        threshold = SENSITIVITY_TABLE[app_set->wd.gsn] * LSG;
                        if(impact > threshold)
                        {
                            dprintf("G_Sensor event(%f/%f)===\n", impact, threshold);

                            if (!app_cfg->ste.b.evt)
                                app_event_set();

                            app_cfg->ste.b.shock = 1;
                            app_cfg->ste.b.evt = 1;

#if 1
                            printf("\n***********************************************\n");
                            printf("Prev Data x[%03d] y[%03d] z[%03d]\n",acc.x, acc.y, acc.z);
                            printf("Cur  Data x[%03d] y[%03d] z[%03d]\n",acc_obj.x, acc_obj.y, acc_obj.z);
                            printf("LevelData x[%03d] y[%03d] z[%03d]\n",level.x ,level.y, level.z);

                            printf("impact detect: Impact = %03f, Threshold = %03f\n", impact, threshold);
                            printf("gsensor_lvl[%d]===\n",app_set->wd.gsn);
                            printf("***********************************************\n\n");
#endif
                            sensing_time = 0;
                        }
                    }

                    if (app_cfg->ste.b.mmc && app_rec_state())
                        gs_meta_add(&level);

                    acc.x = acc_obj.x;
                    acc.y = acc_obj.y;
                    acc.z = acc_obj.z;
                }
            }
        }

        if (app_cfg->ste.b.evt) {
            sensing_time += GSENS_CYCLE_TIME;
            if (GSENS_CYCLE_TIME * 50 < sensing_time) {
                app_cfg->ste.b.evt = 0 ;
            }
        }

        app_msleep(GSENS_CYCLE_TIME);
    }

gsn_exit:
    dev_accel_stop();
    dev_accel_exit();

    tObj->active = 0;
	aprintf("...exit\n");

    return NULL;
}

/*****************************************************************************
* @brief	gps thread function
* @section  [desc]
*****************************************************************************/
static void *THR_gps(void *prm)
{
	app_thr_obj *tObj = &idev->gpsObj;
	int exit=0, cmd;
	int state;
	struct gps_data_t gps_data;
	gps_nmea_t *gps_nmea;
	int time_set=1;

	//# gps init
    app_leds_gps_ctrl(LED_GPS_OFF); //#default LED OFF
	if (dev_gps_init() != OSA_SOK) {
		eprintf("failed to gps init\n");
        app_log_write( MSG_LOG_WRITE, "[APP_DEV] !!! GPS Initialize failed !!!" );
		return NULL;
	}

	aprintf("enter...\n");
	tObj->active = 1;
	while(!exit)
	{
		app_cfg->wd_flags |= WD_DEV;

		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP || app_cfg->ste.b.pwr_off) {
            break;
        }

        // GPS Port removed on hw rev1.0, external gps doesn't work   
//        app_leds_gps_ctrl(LED_GPS_OFF);
		app_msleep(TIME_GPS_CYCLE);


		//# check gps port state
		#if EN_GPS
        //# check gps port state
		state = dev_ste_gps_port();
		if (!state) {
			app_leds_gps_ctrl(LED_GPS_OFF);
		} else {
			//# get gps value
			state = dev_gps_get_data(&gps_data);
			if (state == GPS_ONLINE)
			{
				//# check status mask
				if (gps_data.rmc_status == STATUS_FIX)
				{
					app_leds_gps_ctrl(LED_GPS_ON);
					gps_nmea = &gps_data.nmea;

					#if 0
					dprintf("GPS - DATE %04d-%02d-%02d, UTC %02d:%02d:%02d, speed=%.2f, (LAT:%.2f, LOT:%.2f) \n",
						gps_nmea->date.tm_year+1900, gps_nmea->date.tm_mon+1, gps_nmea->date.tm_mday,
						gps_nmea->date.tm_hour, gps_nmea->date.tm_min, gps_nmea->date.tm_sec,
						gps_nmea->speed, gps_nmea->latitude, gps_nmea->longitude
					);

                    gps_log_interval += TIME_GPS_CYCLE ;
                    if(gps_log_interval == 20000)
                    {
                        sprintf(gps_buff, "GPS - DATE %04d-%02d-%02d, UTC %02d:%02d:%02d, speed=%.2f, (LAT:%.2f, LOT:%.2f)",gps_nmea->date.tm_year+1900, gps_nmea->date.tm_mon+1, gps_nmea->date.tm_mday,
                        gps_nmea->date.tm_hour, gps_nmea->date.tm_min, gps_nmea->date.tm_sec,
                        gps_nmea->speed, gps_nmea->latitude, gps_nmea->longitude) ;

                        app_log_write( MSG_LOG_WRITE, gps_buff);
                        gps_log_interval = 0 ;
                    }
					#endif

					//# set gps data
					idev->gps.enable = ENA;
					#if EN_REC_META
					idev->gps.utc_year = gps_nmea->date.tm_year;
					idev->gps.utc_month = gps_nmea->date.tm_mon;
					idev->gps.utc_mday = gps_nmea->date.tm_mday;
					idev->gps.utc_hour = gps_nmea->date.tm_hour;
					idev->gps.utc_min = gps_nmea->date.tm_min;
					idev->gps.utc_sec = gps_nmea->date.tm_sec;
					idev->gps.subsec = gps_nmea->subseconds;
					idev->gps.speed = gps_nmea->speed;
					idev->gps.dir = gps_nmea->track;
					#endif
					idev->gps.lat = gps_nmea->latitude;
					idev->gps.lot = gps_nmea->longitude;

					//# time sync
					if(time_set)
					{
						if (gps_data.set & G_TIME_SET) {
							app_sys_time(&gps_nmea->date);
							time_set = 0;
							dev_buzz_ctrl(100, 3);	//# gps time sync
						}
					}
				}
				else {
					app_leds_gps_ctrl(LED_GPS_FAIL);
				}
			}
			else	//# gps position not fixed.
			{
				idev->gps.enable = DIS;
			}
		}
        #endif

		app_msleep(TIME_GPS_CYCLE);
	}

	//# device close
	dev_gps_close();

	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}




/*****************************************************************************
* @brief	meta data thread function
* @section  [desc]
*****************************************************************************/
static void *THR_meta(void *prm)
{
	app_thr_obj *tObj = &idev->metaObj;
    int cmd, exit = 0, evt = 0, state_change = 0;
    app_extmeta_t fitt_data;
    gps_rmc_t Gpsdata ;

    fitt_meta_reset();

	aprintf("enter...\n");
	tObj->active = 1;
    while (!exit)
    {
        cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP || app_cfg->ste.b.pwr_off) {
            break;
        }

        dev_get_gps_rmc((void*)&Gpsdata);

        gpsdata_send((void*)&Gpsdata) ;

        //# get gsensor data
//        fitt_get_gs(&fitt_data.acc_level);

//        fitt_meta_add(&fitt_data);

        app_msleep(FITTMETA_CYCLE_TIME);
    }

    tObj->active = 0;
	aprintf("...exit\n");

    return NULL;
}

/*****************************************************************************
* @brief    IO device thread start/stop
* @section  [desc]
*****************************************************************************/
int app_dev_init(void)
{
	app_thr_obj *tObj;
	int status;

	//# static config clear - when Variable declaration
	memset((void *)idev, 0x0, sizeof(app_dev_t));
	dev_gpio_init();

    status = OSA_mutexCreate(&(idev->mutex_gps));
    OSA_assert(status == OSA_SOK);
#if 0

    status = OSA_mutexCreate(&idev->mutex_gsn);
    OSA_assert(status == OSA_SOK);
#endif

    if(app_set->srv_info.ON_OFF)
	{
        status = OSA_mutexCreate(&idev->mutex_meta);
        OSA_assert(status == OSA_SOK);
    }
	//#--- create thread
    tObj = &idev->devObj;
    if (thread_create(tObj, THR_dev, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create dev thread\n");
		return EFAIL;
    }

#if 0
    //#--- Gsensor
    tObj = &idev->gsnObj;
    if (thread_create(tObj, THR_gsn, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create gsensor thread\n");
		return EFAIL;
    }
#endif

    //#--- gps
    tObj = &idev->gpsObj;
    if (thread_create(tObj, THR_gps, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create gps thread\n");
		return EFAIL;
    }


//    if(app_set->srv_info.ON_OFF)
	{
        tObj = &idev->metaObj;
        if (thread_create(tObj, THR_meta, APP_THREAD_PRI, NULL) < 0) {
    	    eprintf("create meta thread\n");
		    return EFAIL;
		}
    }

	aprintf("... done!\n");

	return SOK;
}

void app_dev_exit(void)
{
	app_thr_obj *tObj;

    if(app_set->srv_info.ON_OFF)
	{
    //#-- fitt_meta stop
        tObj = &idev->metaObj;
        event_send(tObj, APP_CMD_STOP, 0, 0);

        while(tObj->active)
    	    app_msleep(20);
        thread_delete(tObj);
    }

    //#--- gps stop
    tObj = &idev->gpsObj;
    event_send(tObj, APP_CMD_STOP, 0, 0);
    while(tObj->active)
    	app_msleep(20);
    thread_delete(tObj);

#if 0
	//#--- gsensor stop
    tObj = &idev->gsnObj;
    event_send(tObj, APP_CMD_STOP, 0, 0);
    while(tObj->active)
    	app_msleep(20);
	thread_delete(tObj);
#endif
    //#--- dev_stop
    tObj = &idev->devObj;
    event_send(tObj, APP_CMD_STOP, 0, 0);
    while(tObj->active)
    	app_msleep(20);
	thread_delete(tObj);

	/* mutex delete */
    OSA_mutexDelete(&idev->mutex_gps);
#if 0
    OSA_mutexDelete(&idev->mutex_gsn);
#endif

    if(app_set->srv_info.ON_OFF)
        OSA_mutexDelete(&idev->mutex_meta);

	/* gpio free */
	dev_gpio_exit();

	aprintf("... done!\n");
}
