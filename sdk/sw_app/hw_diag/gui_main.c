/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    gui_main.c
 * @brief
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <alsa/asoundlib.h>

//# include mcfw_linux
#include "ti_vsys.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_cap.h"
#include "app_ctrl.h"
#include "gui_main.h"
#include "app_util.h"
#include "app_dev.h"
#include "dev_gpio.h"
#include "dev_common.h"
#include "dev_micom.h"
#include "app_udpsock.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define USE_MENU				0
#define USE_NTP                 0

//# For sound
#define APP_SND_SRATE			8000 //# for baresip
#define APP_SND_PTIME			250 //# fixed 고정해야 함.
#define APP_SND_CH				1   /* sound channel */

#define UI_CYCLE_TIME			200	//# ms
#define KEY_CYCLE_TIME          100

typedef enum {
	KEY_NONE = 0,
	KEY_SHORT,
	KEY_LONG
} key_type_e;

typedef struct {
	app_thr_obj mObj;	//# gui main thread
	app_thr_obj uObj;	//# gui run thread
	app_thr_obj iObj;	//# ir thread
	
	app_thr_obj sndIn;	//# ir thread
	app_thr_obj sndOut;	//# ir thread
	
} app_gui_t;

#define SND_PCM_BITS		16 /* bits per sample */
typedef struct {
	char path[256];			//# pcm virtual name
	snd_pcm_t *pcm_t;
	
	int channel;
	int sample_rate;
	int mode;
	int num_frames;
	
	char *sampv;

} snd_prm_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_gui_t gui_obj;
static app_gui_t *igui=&gui_obj;

static int snd_pipe[2];

static snd_prm_t snd_in_data;
static snd_prm_t snd_out_data;

#define PATH_LOCALTIME  "/etc/localtime"
#define PATH_ZONE_INFO  "/usr/share/zoneinfo"
#define PATH_ZONE_LOCAL "/usr/share/zoneinfo/localtime"
#define PATH_ZONE_POSIX "/usr/share/zoneinfo/posixrules"

static char *TZfiles[][2] = {
    {"Etc/GMT+12", "Pacific/Kwajalein"},
    {"Etc/GMT+11", "Pacific/Midway"},
    {"Etc/GMT+10", "US/Hawaii"},
    {"Etc/GMT+9",  "US/Alaska"},
    {"Etc/GMT+8",  "PST8PDT"},
    {"MST"      ,  "MST7MDT"},
    {"Etc/GMT+6",  "US/Central"},
    {"EST",        "EST5EDT"},                  /* Eastern Standard, Eastern Daylight */
    {"Etc/GMT+4",  "Canada/Atlantic"},
    {"Etc/GMT+3",  "America/Buenos_Aires"},
    {"Etc/GMT+2",  "Etc/GMT+2"},
    {"Etc/GMT+1",  "Atlantic/Azores"},
    {"Etc/GMT",    "Europe/London"},
    {"Etc/GMT-1",  "Europe/Berlin"},
    {"Etc/GMT-2",  "Europe/Athens"},
    {"Etc/GMT-3",  "Europe/Moscow"},
    {"Etc/GMT-4",  "Asia/Muscat"},
    {"Etc/GMT-5",  "Asia/Karachi"},
    {"Etc/GMT-6",  "Asia/Dhaka"},
    {"Etc/GMT-7",  "Asia/Bangkok"},
    {"Etc/GMT-8",  "Asia/Taipei"},
    {"Etc/GMT-9",  "Asia/Seoul"},
    {"Etc/GMT-10", "Australia/Brisbane"},
    {"Etc/GMT-11", "Asia/Magadan"},
    {"Etc/GMT-12", "Pacific/Fiji"},
    {"Etc/GMT-13", "Etc/GMT-13"},
    {"Etc/GMT-14", "Etc/GMT-14"}
};

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

//#----------------------- KEY -----------------------------------------------
/*****************************************************************************
* @brief    rec key switch check function
* @section  [desc]
*****************************************************************************/
static int dev_ste_key(int gio)
{
	int status;

	gpio_get_value(gio, &status);
	//DBG_HWD("--- [key] value %d\n", status);

	return status;
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

static void *thread_key(void *prm)
{
	app_thr_obj *tObj = &igui->iObj;
	int exit=0, res;
    char cmd;

	tObj->active = 1;
			
	while (!exit)
	{
		cmd = tObj->cmd;
		if (cmd == APP_CMD_EXIT) {
			exit = 1 ;
		    break;
		}

		res = chk_rec_key();
		if (res == KEY_SHORT || res == KEY_LONG) {
			DBG_HWD("REC KEY OK!!\n");
			send_keyPress(0) ;
		}

	    OSA_waitMsecs(UI_CYCLE_TIME);
	}
	

	tObj->active = 0;
	DBG_HWD("...exit\n");

	return NULL;
}
//# ---------------------------------------------------------------------------
//################## TEST Functions ##########################################
/*----------------------------------------------------------------------------
 wait result
 type 0: draw skip/ok/fail, type 1: draw ok/fail
-----------------------------------------------------------------------------*/
static int wait_result(app_thr_obj *tObj)
{
	int cmd, exit=0;

	while (!exit)
	{
		//# wait cmd
		cmd = event_wait(tObj);
		switch (cmd) {
		    case APP_KEY_PWR:
				exit = 1 ;
			    break ;
		    default:
			    break;
		}
	}

	return 0;
}

static char menu_exit[] = {
	"\r\n ============="
	"\r\n  EXIT        "
	"\r\n ============="
	"\r\n"
	"\r\n 0: exit"
	"\r\n"
	"\r\n Enter Choice: "
};

static int test_info(app_thr_obj *tObj)
{
	int res;
	char ver[64]={0,};
//	char buf[64]={0,};
	
	DBG_HWD("version test start...\n");

	//# sw version
	sprintf(ver, "%s", FITT360_SW_VER);
	DBG_HWD("sw version %s\n", ver);
	strcpy(iapp->sw_ver, ver);

	res = ctrl_get_hw_version(NULL);
	iapp->hw_ver = res;
	DBG_HWD("hw version %d\n", res);
	
	res = ctrl_get_mcu_version(ver);
	iapp->mcu_ver = res;
	/* sprintf(version, "%02d.%02X", (ver>>8)&0xFF, ver&0xFF); */
	DBG_HWD("mcu version %s\n", ver);
	
//	util_hexdump(buf, 64);
	
	printf(menu_exit);
	res = wait_result(tObj);
	
	return res;
}

static int test_buzzer(app_thr_obj *tObj)
{
	int res = 0;
	
	DBG_HWD("buzzer test start...\n");
	
	app_buzzer(100, 10);
	
	printf(menu_exit);
	res = wait_result(tObj);
	
	return res;
}

static int test_key(app_thr_obj *tObj)
{
	int res = 0, exit=0;
	
	while (!exit) 
	{
		res = chk_rec_key();
		if (res == KEY_SHORT || res == KEY_LONG) {
			DBG_HWD("REC KEY OK!!\n");
			send_keyPress(0) ;
		}
		
	    OSA_waitMsecs(UI_CYCLE_TIME);
	}
	
	printf(menu_exit);
	res = wait_result(tObj);
	
	return res;
}

//################################ ALSA Capture / Playback Helper Routine###########################################
static inline void *advance(void *p, int incr)
{
    unsigned char *d = p;
    return (d + incr);
}

static int _snd_set_param(const char *name, snd_prm_t *prm, 
					int mode, int ch, int rate, int ptime)
{
	int sampc = 0;
	int ret = 0;
	
	/* For Debugging */
	memset(prm->path, 0, sizeof(prm->path));
	strcpy(prm->path, name);
	
	/* capture device init */
	prm->channel = ch;
	prm->mode = mode; //# capture ? playback
	prm->sample_rate = rate;
	/* 1초에 sample에 해당하는 프레임 수신. 
	 * period size = 1s:8000 = 80ms : ? --> sample rate * ptime / 1000
	 */
	prm->num_frames = ptime; //# period
	sampc = rate * ch * ptime / 1000;
	prm->sampv = (char *)malloc(sampc * SND_PCM_BITS / 8); //# 1sample 16bit..
	if (prm->sampv == NULL)
		ret = -1;
	
	return ret;
}

static void _snd_set_swparam(snd_prm_t *prm, int mode)
{
	snd_pcm_sw_params_t *sw_params = NULL;
	snd_pcm_t *handle = prm->pcm_t;
	snd_pcm_uframes_t buffer_size, period_size;
	int err;
	
	period_size = prm->num_frames;
	buffer_size = (period_size * 4); //# alsa 표준
	
	/* set software params */
	snd_pcm_sw_params_alloca(&sw_params);
    err = snd_pcm_sw_params_current(handle, sw_params);
    if (err < 0) {
        DBG_HWD("Failed to get current software parameters\n");
    }
	snd_pcm_sw_params_set_avail_min(handle, sw_params, period_size);

	if (mode == 1) {
		snd_pcm_sw_params_set_start_threshold(handle, sw_params, buffer_size);
	    snd_pcm_sw_params_set_stop_threshold(handle, sw_params, buffer_size);
	} else {
		snd_pcm_sw_params_set_start_threshold(handle, sw_params, 1);
	    snd_pcm_sw_params_set_stop_threshold(handle, sw_params, buffer_size);	
	}

    err = snd_pcm_sw_params(handle, sw_params);
    if (err < 0) {
        DBG_HWD("Failed to set software parameters\n");
    }
}

static int _snd_open(const char *pcm_name, snd_prm_t *prm)
{
	unsigned int freq, nchannels;
	int err;
	
	snd_pcm_t *handle = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;

	snd_pcm_uframes_t buffer_size, period_size;
	snd_pcm_stream_t mode;
	
	period_size = prm->num_frames;
	buffer_size = (period_size * 4); //# alsa 표준
	
	if (prm->mode == 1) {
		mode = SND_PCM_STREAM_PLAYBACK;
	} else {
		mode = SND_PCM_STREAM_CAPTURE;	
	}
	
	err = snd_pcm_open(&handle, pcm_name, mode, 0);
	if (err < 0) {
		DBG_HWD("alsa: could not open device '%s' (%s)\n", pcm_name, 
												snd_strerror(err));
		goto out;
	}
	
	snd_pcm_hw_params_alloca(&hw_params);
	err = snd_pcm_hw_params_any(handle, hw_params);
	if (err < 0) {
		DBG_HWD("Failed to initialize hardware parameters\n");
		goto out;
	}

	/* Set access type.*/
	err = snd_pcm_hw_params_set_access(handle, hw_params,
							SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0) {
		DBG_HWD("Failed to set access type\n");
		goto out;
	}

	/* Set sample format */
	err = snd_pcm_hw_params_set_format(handle, hw_params, SND_PCM_FORMAT_S16_LE);
	if (err < 0) {
		DBG_HWD("cannot set sample format!\n");
		goto out;
	}

	/* Set sample rate. If the exact rate is not supported */
	/* by the hardware, use nearest possible rate.         */
	freq = prm->sample_rate;
	err = snd_pcm_hw_params_set_rate_near(handle, hw_params, &freq, 0);
	if (err < 0) {
		DBG_HWD("Failed to set frequency %d\n", freq);
		goto out;
	}
	
	nchannels = prm->channel;
	err = snd_pcm_hw_params_set_channels(handle, hw_params, nchannels);
	if (err < 0) {
		DBG_HWD("Failed to set number of channels %d\n", nchannels);
		goto out;
	}

    err = snd_pcm_hw_params_set_period_size_near(handle, hw_params, &period_size, 0);
    if (err < 0) {
        DBG_HWD("Failed to set period size to %ld\n", period_size);
        goto out;
    }

    err = snd_pcm_hw_params_set_buffer_size_near(handle, hw_params, &buffer_size);
    if (err < 0) {
        DBG_HWD("Failed to set buffer size to %ld\n", buffer_size);
        goto out;
    }

	/* Apply HW parameter settings to */
    /* PCM device and prepare device  */
    err = snd_pcm_hw_params(handle, hw_params);
	if (err < 0) {
		DBG_HWD("Unable to install hw params\n");
		goto out;
	}

	/* prepare for audio device */
	err = snd_pcm_prepare(handle);
    if (err < 0) {
        printf("Could not prepare handle %p\n", handle);
        goto out;
    }

	prm->pcm_t = handle;
	err = 0;
	
out:
	if (err) {
		DBG_HWD("alsa: init failed: err=%d\n", err);
	}
	
	return err;
}

static void _snd_param_free(snd_prm_t *prm)
{
	memset(prm->path, 0, sizeof(prm->path));
	if (prm->sampv != NULL) {
		free(prm->sampv);
		prm->sampv = NULL;
	}
}

static void _snd_start(snd_prm_t *prm)
{
	snd_pcm_t *handle = prm->pcm_t;
	int err;
	
	/* Start */
	err = snd_pcm_start(handle);
	if (err) {
		DBG_HWD("alsa: could not start ausrc device %s, (%s)\n", 
				prm->path, snd_strerror(err));
	}
}

static void _snd_stop(snd_prm_t *prm, int mode)
{
	snd_pcm_t *handle = prm->pcm_t;
	
	if (handle != NULL) {
		if (mode == 1) {
			snd_pcm_nonblock(handle, 0);
			/* Stop PCM device after pending frames have been played */
			snd_pcm_drain(handle);
			//snd_pcm_nonblock(handle, 1);
		}  else {
			//alsa_mixer_cset(SND_PGA_CAPTURE_SWITCH, 0);
		}
		snd_pcm_hw_free(handle);
		snd_pcm_close(handle);
	}
}

static ssize_t _snd_read(snd_prm_t *prm)
{
	snd_pcm_t *handle = prm->pcm_t;
	char *rbuf = (char *)prm->sampv;

	ssize_t result = 0;
	size_t count = (size_t)prm->num_frames;
	int hwshift;

	hwshift = prm->channel * (SND_PCM_BITS / 8);

	while (count > 0)
	{
		snd_pcm_sframes_t r;

		r = snd_pcm_readi(handle, rbuf, count);
		if (r <= 0)
		{
			switch (r) {
			case 0:
				DBG_HWD(" Failed to read frames(zero)\n");
				continue;

			case -EAGAIN:
				DBG_HWD(" pcm wait (count = %d)!!\n", count);
				snd_pcm_wait(handle, 100);
				break;

			case -EPIPE:
				DBG_HWD(" pcm overrun(count = %d)!!\n", count);
				snd_pcm_prepare(handle);
				break;

			default:
				DBG_HWD(" read error!!\n");
				return -1;
			}
		}

		/* read size is frame size. For byte conversion..*/
		/* Frame = 1sample * Total Channel, 1 sample = 16bit (2Byte) */
		if (r > 0) {
			rbuf = advance(rbuf, (r * hwshift));
			result += (r * hwshift);
			count -= r;
		}
	}

	return result;
}

static ssize_t _snd_write(snd_prm_t *prm, size_t w_samples)
{
	snd_pcm_t *handle = prm->pcm_t;
	char *rbuf = (char *)prm->sampv;
	
	ssize_t result = 0;
	size_t count = (size_t)w_samples;
	size_t period = (size_t)prm->num_frames;
	int hwshift;

	hwshift = prm->channel * (SND_PCM_BITS / 8); //# 16bit * channel / 8bit

	if (count < period) {
    	snd_pcm_format_set_silence(SND_PCM_FORMAT_S16_LE, rbuf + (count * hwshift),
						(period - count) * prm->channel);
        count = period;
	}

	while (count > 0)
	{
		snd_pcm_sframes_t r;
		int ret;

		r = snd_pcm_writei(handle, rbuf, count);
		if (r == -EAGAIN || (r >= 0 && (size_t)r < count)) {
			//DBG_HWD("pcm write wait(100ms)!!\n");
			snd_pcm_wait(handle, 100);
		} else if (r == -EPIPE) {
			//DBG_HWD("pcm write underrun!!\n");
			ret = snd_pcm_prepare(handle);
			if (ret < 0) {
				DBG_HWD("Failed to prepare handle %p\n", handle);
			}
			continue;
		} else if (r == -ESTRPIPE) {
			ret = snd_pcm_resume(handle);
			if (ret < 0) {
				DBG_HWD("Failed. Restarting stream.\n");
			}
			continue;
		} else if (r < 0) {
			DBG_HWD("write error\n");
			return -1;
		}

		if (r > 0) {
			rbuf = advance(rbuf, r * hwshift);
			result += (r * hwshift);
			count -= r;
		}
	}

	return result;
}

//##################################################################################################################
/*----------------------------- Sound In/Out ------------------------------------------------------------------- */
static void thr_snd_in_cleanup(void *prm)
{
	_snd_stop(&snd_in_data, 0);
	_snd_param_free(&snd_in_data);
}

static void *thr_snd_in(void *prm)
{
	int buf_sz;
	int exit = 0, r;
	FILE *f;
	
	/* set capture volume */
	//# "/usr/bin/amixer cset numid=31 60% > /dev/null"
	f = popen("/usr/bin/amixer cset numid=31 60% > /dev/null", "r");
	if (f != NULL) {
		pclose(f);
	}
	
	/* get alsa period size (in sec) */
	buf_sz = APP_SND_SRATE * APP_SND_PTIME / 1000; //# 
	r = _snd_set_param("aic3x", &snd_in_data, 0, 
				APP_SND_CH, APP_SND_SRATE, buf_sz);

	r |= _snd_open("plughw:0,0", &snd_in_data);
	if (r) {
		ERR_HWD("Failed to init sound device!\n");
	}
	
	_snd_start(&snd_in_data);
	pthread_cleanup_push(thr_snd_in_cleanup, (void *)&snd_in_data);

	while (!exit)
	{
		int bytes = 0;
		
		bytes = _snd_read(&snd_in_data);
		if (bytes > 0) {
			write(snd_pipe[1], snd_in_data.sampv, bytes);
		}
	}

	pthread_cleanup_pop(0);

	return NULL;
}

static void thr_snd_out_cleanup(void *prm)
{
	OSA_waitMsecs(100);
	_snd_stop(&snd_out_data, 1);
	_snd_param_free(&snd_out_data);
}

void *thr_snd_out(void *prm)
{
	int buf_sz, so_size;
	int exit = 0, r;
	FILE *f;
	
	/* set playback volume */
	//# "/usr/bin/amixer cset numid=1 70% > /dev/null"
	f = popen("/usr/bin/amixer cset numid=1 70% > /dev/null", "r");
	if (f != NULL) {
		pclose(f);
	}
	
	/* get alsa period size (in sec) */
	buf_sz  = APP_SND_SRATE * APP_SND_PTIME / 1000; //# 
	so_size = (buf_sz * APP_SND_CH * (SND_PCM_BITS / 8)); 
	r = _snd_set_param("aic3x", &snd_out_data, 1, 
				APP_SND_CH, APP_SND_SRATE, buf_sz);

	r |= _snd_open("plughw:0,0", &snd_out_data);
	if (r) {
		ERR_HWD("Failed to init sound device!\n");
		return NULL;
	}

	_snd_set_swparam(&snd_out_data, 1);
	pthread_cleanup_push(thr_snd_out_cleanup, (void *)&snd_out_data);

	while (!exit)
	{
		r = read(snd_pipe[0], snd_out_data.sampv, so_size);
		if (r > 0) {
			_snd_write(&snd_out_data, r/2);
		}
	}

	pthread_cleanup_pop(0);
	return NULL;
}

int test_snd(app_thr_obj *tObj)
{
	int res;

	DBG_HWD("sount test start...\n");
	
	pipe(snd_pipe);
	/* create sound in/out thread */
	thread_create(&igui->sndIn, thr_snd_in, APP_THREAD_PRI, NULL, NULL);
	thread_create(&igui->sndOut, thr_snd_out, APP_THREAD_PRI, NULL, NULL);
	
	//# Question
//	printf(menu_exit);
	res = wait_result(tObj);
	
	thread_delete(&igui->sndOut);
	thread_delete(&igui->sndIn);
	return res;
}
//########################################################################################################

/*****************************************************************************
* @brief    test menu function
* @section  [desc]
*****************************************************************************/
/*
static char menu_main[] = {
	"\r\n ============="
	"\r\n Demo Menu"
	"\r\n ============="
	"\r\n"
	"\r\n 0: exit"
	"\r\n 1: Jpeg"
	"\r\n"
	"\r\n 2: Get version"
	"\r\n 3: Buzzer test"
	"\r\n 4: Key test"
	"\r\n 5: Sound test"
	"\r\n"
	"\r\n Enter Choice: "
};
*/

int update_m3_time()
{
    time_t now;
    struct tm *pgm;
    int    retval = FALSE;

 //  DBG_HWD("--- update m3 time ---\n");

 // time(), gmtime(), it represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 (UTC)
    time(&now);
    pgm = gmtime(&now);

    if (dev_rtc_set_time(*pgm) < 0)
	{
        ERR_HWD("Failed to set system time to rtc\n!!!");
    }
    else
    {
        char buff[128]={0};
        sprintf(buff, "/opt/fit/bin/tz_set &") ;  // it needs file create time sync with windows browser
        system(buff) ;
        retval = TRUE;
	    app_msleep(100);
        Vsys_datetime_init2(1);   //# m3 Date/Time init
    }
    return retval;
}

char *get_timezone (int timezone, int daylightsaving)
{
    if(daylightsaving<0 || daylightsaving > 1) {
        ERR_HWD("Please, check for daylightsaving(%d). It must be 0 or 1", daylightsaving);
        daylightsaving = 0;
    }

    int tz_idx = timezone + 12;
    static char buffer[128];
    memset (&buffer, 0, sizeof(buffer));

    sprintf (buffer, "%s", TZfiles[tz_idx][daylightsaving]);
//  printf("get_timezone:%s\n", buffer);
    return buffer ;
}

int set_time_zone()
{
    FILE *Fp ;
    char buf[120] ;
    int retval = 0 ;

    int timezone = 9;
    int daylight  = 1;  //# 0:false, 1:automatically for daylight saving time changes.
    sprintf(buf,"rm -f /etc/adjtime;rm -f %s; ln -s %s/%s %s",
    PATH_LOCALTIME,
    PATH_ZONE_INFO,
    get_timezone(timezone, daylight),
    PATH_LOCALTIME) ;
 //  printf("set_time_zone(), %s\n", buf);
    Fp = popen(buf, "w") ;

    if(Fp == NULL)
    {
        printf("ERROR setting Timezone \n") ;
        pclose(Fp) ;
        return retval ;
    }
    pclose(Fp) ;

    // No guaranteed completion PIPE???
    update_m3_time();

    return 1 ;
}

int set_time_via_ntp()
{
	char buff[128], server_addr[32], addr[32];
    struct hostent *hp ;

    sprintf(server_addr, "%s", "time.google.com") ;
    hp = gethostbyname(server_addr) ;
    if(!hp)
		return FALSE ;

	memset(addr, 0x00, 32) ;
	strncpy(addr, inet_ntoa(*((struct in_addr *)hp->h_addr_list[0])), strlen(inet_ntoa(*((struct in_addr *)hp->h_addr_list[0]))));
 
	sprintf(buff,"/opt/fit/bin/ntpclient -h %s -d -s -i 4",addr) ;
	system(buff) ;

	sleep(3) ;
	update_m3_time() ;
	
	return 1;
}

static int gui_test_main(void *thr)
{
	app_thr_obj *tObj = (app_thr_obj *)thr;
	char cmd, exit=0;
	INFO_REQ Inforeq ;
	char Macaddr[20] ;

    while(iapp->ste.b.cap)
	{
		OSA_waitMsecs(2000);
		break ;
	}
 
#if USE_NTP	
	set_time_zone() ; 
    set_time_via_ntp() ;
#endif

	if(GetMacAddress(Macaddr))
         sprintf(Macaddr, "%s", "UNKNOWN");

	Inforeq.identifier = htons(IDENTIFIER) ;
	Inforeq.cmd = htons(CMD_INFO_REQ) ;
	Inforeq.length = htons(sizeof(INFO_REQ)) ;
  
   // Inforeq packet transmission immediately after system booting is completed	
	send_sysinfo((char*)&Inforeq) ;   

#if defined(NEXXONE)
    test_snd(tObj) ;
#endif

	while (!exit)
	{
		cmd = event_wait(tObj);
		if ((cmd == APP_CMD_EXIT) || (cmd == APP_KEY_PWR)) {
			printf("APP_KEY_PWR Break !!\n") ;
			break;
		}

		if (cmd == APP_KEY_OK) 
		{
			switch (tObj->param0) {
			case '0':
				// exit program 
				exit=1;
				continue;
				
			case '1':
				// camera 
				iapp->snapshot = 1; //# temp
				break;
			
			case '2':
				// Get Version 
				test_info(tObj);
				break;
			
			case '3':
				// Buzzer Version 
				test_buzzer(tObj);
				break;
			
			case '4':
				// REC KEY 
				test_key(tObj);
				break;
			
			case '5':
				test_snd(tObj);
				break;		

			default :
				break ;
			}
		}

	}
	
	return 0;
}

/*****************************************************************************
* @brief    gui run thread function
* @section  [desc]
*****************************************************************************/
static void *THR_gui_run(void *prm)
{
	app_thr_obj *tObj = &igui->uObj;
	int cmd, exit=0;

	DBG_HWD("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = tObj->cmd;
		if (cmd == APP_CMD_EXIT) {
			break;
		}

		/* TODO */
		switch (cmd) {
		default:
			break;
		}

		//# cmd clear
		tObj->cmd = 0;
		//# wait time
		OSA_waitMsecs(UI_CYCLE_TIME);
	}

	tObj->active = 0;
	DBG_HWD("exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    gui main function
* @section  [desc]
*****************************************************************************/
int gui_main(void)
{
	app_thr_obj *tObj = (app_thr_obj *)&igui->mObj;
	
	//#--- create thread - for communcation
	if(thread_create(tObj, NULL, APP_THREAD_PRI, NULL, NULL) < 0) {
		ERR_HWD("create thread\n");
		return EFAIL;
	}
	tObj->active = 1;
	
	app_cap_start();
	
	app_buzzer(50, 1);
	gui_test_main((void *)tObj);
	
	app_buzzer(50, 2);
	app_cap_stop();

	//#--- stop thread
	tObj->active = 0;
	thread_delete(tObj);

	return SOK;
}

/*****************************************************************************
* @brief    gui control function
* @section  [desc]
*****************************************************************************/
int gui_ctrl(int cmd, int p0, int p1)
{
	event_send(&igui->mObj, cmd, p0, p1);

	return 0;
}

/*****************************************************************************
* @brief    gui init/exit function
* @section  [desc]
*****************************************************************************/
int app_gui_init(void)
{
	app_thr_obj *tObj;

	//# static config clear
	memset((void *)igui, 0x0, sizeof(app_gui_t));
	
	gpio_input_init(REC_KEY);
	
	//#--- create gui run thread
	tObj = &igui->uObj;
	if (thread_create(tObj, THR_gui_run, UI_THREAD_PRI, NULL, NULL) < 0) {
		ERR_HWD("create thread\n");
		return EFAIL;
	}
	
	//#--- create ir thread
	tObj = &igui->iObj;
	if(thread_create(tObj, thread_key, UI_THREAD_PRI, NULL, NULL) < 0) {
		ERR_HWD("create thread\n");
		return -1;
	}

	return SOK;
}

int app_gui_exit(void)
{
	app_thr_obj *tObj;
	
	//#--- stop thread
	tObj = &igui->iObj; //# key thread..
	thread_delete(tObj);
	
	tObj = &igui->uObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		OSA_waitMsecs(10);
	}
	thread_delete(tObj);
	
	gpio_exit(REC_KEY);
	
	return SOK;
}
