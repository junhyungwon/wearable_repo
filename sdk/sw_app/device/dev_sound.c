/*
 * File : dev_sound.c
 *
 * Copyright (C) 2013 UDWORKs
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * this implements a sound hardware library for UBX.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "dev_common.h"
#include "dev_sound.h"

#define SND_PCM_NAME_AIC3X			"plughw:0,0"  //# AIC3X
#define SND_PCM_NAME_NVP6X			"plughw:1,0"  //# NVP6x

#define SW_ON		                (1)
#define SW_OFF		                (0)

/* alsa amixer enum */
#define SND_LINE_PLAYBACK_SWITCH		20 //# boolean
#define SND_HPCOM_PLAYBACK_SWITCH		22 //# boolean
#define SND_PGA_CAPTURE_SWITCH			32 //# boolean
#define SND_OUTPUT_POWERON_TIME			34 //# enum (headphone driver power on time)
#define SND_OUTPUT_RAMPUP_STEP			35 //# enum (0->0ms, 1->1ms, 2->2ms, 3->4ms)
#define SND_LEFT_HPCOM_DACL1_SWITCH		53 //# boolean
#define SND_RIGHT_HPCOM_DACR1_SWITCH	61 //# boolean

#define SND_LEFT_LINE_DACL1_SWITCH		77 //# boolean (default on)
#define SND_RIGHT_PGA_LINE1R_SWITCH		83 //# boolean (default on)
#define SND_RIGHT_PGA_LINE1L_SWITCH		84 //# boolean (default on)
#define SND_RIGHT_PGA_MIC3L_SWITCH		86 //# boolean (default off)
#define SND_RIGHT_PGA_MIC3R_SWITCH		87 //# boolean (default off)
#define SND_LEFT_PGA_LINE1L_SWITCH		89 //# boolean (default on)
#define SND_LEFT_PGA_LINE1R_SWITCH		90 //# boolean
#define SND_LEFT_PGA_MIC3L_SWITCH		92 //# boolean (default off)
#define SND_LEFT_PGA_MIC3R_SWITCH		93 //# boolean (default off)
#define SND_RIGHT_LINE1R_MUX			94 //# enum (0->single ended, 1->differential)
#define SND_LEFT_LINE1L_MUX				97 //# enum (0->single ended, 1->differential)

#define SND_RIGHT_HPCOM_MUX			    98 //# enum
#define SND_RIGHT_DAC_MUX				99 //# enum (0->DAC_R1, 1->DAC_R3, 2->DAC_R2)
#define SND_LEFT_HPCOM_MUX			   100 //# enum
#define SND_LEFT_DAC_MUX			   101 //# enum (0->DAC_L1, 1->DAC_L3, 2->DAC_L2)

#define check_range(val, min, max) \
	((val < min) ? (min) : (val > max) ? (max) : (val))

#define convert_prange1(val, min, max) \
	ceil((val) * ((max) - (min)) * 0.01 + (min))

struct volume_ops {
	int (*get_range)(snd_mixer_elem_t *elem, long *min, long *max);
	int (*get)(snd_mixer_elem_t *elem, snd_mixer_selem_channel_id_t c,
		   long *value);
	int (*set)(snd_mixer_elem_t *elem, snd_mixer_selem_channel_id_t c,
		   long value, int dir);
};

struct alsa_volume_ops {
	char *vname; /* control name */
	int (*has_volume)(snd_mixer_elem_t *elem);
	int (*get_range)(snd_mixer_elem_t *elem, long *min, long *max);
	int (*get)(snd_mixer_elem_t *elem, snd_mixer_selem_channel_id_t c,
		   long *value);
	int (*set)(snd_mixer_elem_t *elem, snd_mixer_selem_channel_id_t c,
		   long value);
};

/* capture, playback volume */
static const struct alsa_volume_ops vol_ops[2] = {
	{
		.vname		= "PCM",
		.has_volume = snd_mixer_selem_has_playback_volume,
		.get_range  = snd_mixer_selem_get_playback_volume_range,
		.get	    = snd_mixer_selem_get_playback_volume,
		.set 	    = snd_mixer_selem_set_playback_volume,
	},
	{
		.vname		= "PGA",
		.has_volume = snd_mixer_selem_has_capture_volume,
		.get_range  = snd_mixer_selem_get_capture_volume_range,
		.get        = snd_mixer_selem_get_capture_volume,
		.set        = snd_mixer_selem_set_capture_volume,
	},
};

/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/* Fuction to convert from volume to percentage. val = volume */
static int convert_prange(int val, int min, int max)
{
	int range = max - min;
	int tmp;

	if (range == 0)
		return 0;

	val -= min;
	tmp = rint((double)val/(double)range * 100);

	return tmp;
}

static inline void *advance(void *p, int incr)
{
    unsigned char *d = p;
    return (d + incr);
}

/* alsa-mixer cset implementation */
static int alsa_mixer_cset(int numid, int value)
{
	int err;

	snd_ctl_t *handle = NULL;
	snd_ctl_elem_info_t *info;
	snd_ctl_elem_id_t *id;
	snd_ctl_elem_value_t *control;

	unsigned int idx, count;
	long tmp, val;
	long min, max;

	snd_ctl_elem_type_t type;

	if (numid <= 0) {
		dev_err("Invalid numid %d\n", numid);
		return -1;
	}

	snd_ctl_elem_info_alloca(&info);
	snd_ctl_elem_id_alloca(&id);
	snd_ctl_elem_value_alloca(&control);

	/* default */
	snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);
	snd_ctl_elem_id_set_numid(id, numid);

	if ((err = snd_ctl_open(&handle, "default", 0)) < 0) {
		dev_err("Control open mixer handle\n");
		return err;
	}

	snd_ctl_elem_info_set_id(info, id);
	if ((err = snd_ctl_elem_info(handle, info)) < 0) {
		dev_err("Cannot find the given element from control\n");
		snd_ctl_close(handle);
		return err;
	}

	snd_ctl_elem_info_get_id(info, id);	/* FIXME: Remove it when hctl find works ok !!! */
	type = snd_ctl_elem_info_get_type(info);
	count = snd_ctl_elem_info_get_count(info);
	snd_ctl_elem_value_set_id(control, id);

	//# count = 2 channels (left and right)
	for (idx = 0; idx < count && idx < 128; idx++) {
		switch (type) {
		case SND_CTL_ELEM_TYPE_BOOLEAN:
			tmp = value ? 1:0;
			snd_ctl_elem_value_set_boolean(control, idx, tmp);
			break;
		case SND_CTL_ELEM_TYPE_INTEGER:
			min = snd_ctl_elem_info_get_min(info);
			max = snd_ctl_elem_info_get_max(info);

			val = (long)convert_prange1(value, min, max);
			tmp = check_range(val, min, max);
			snd_ctl_elem_value_set_integer(control, idx, tmp);
			break;
		case SND_CTL_ELEM_TYPE_ENUMERATED:
			min = 0;
			max = snd_ctl_elem_info_get_items(info) - 1;

			tmp = check_range(value, min, max);
			snd_ctl_elem_value_set_enumerated(control, idx, tmp);
			break;
		default:
			break;
		}
	}

	if ((err = snd_ctl_elem_write(handle, control)) < 0) {
		dev_err("Control element write error\n");
		snd_ctl_close(handle);
		return err;
	}

	snd_ctl_close(handle);
	return 0;
}

static int alsa_recover (snd_pcm_t *handle)
{
	int err = 0;

    err = snd_pcm_prepare(handle);
    if (err < 0) {
        dev_err("Failed to prepare handle %p\n", handle);
        return -1;
    }

    return 0;
}

static int alsa_resume(snd_pcm_t *handle)
{
	int ret = 0;

	ret = snd_pcm_resume(handle);
	if (ret < 0) {
		dev_err("Failed. Restarting stream.\n");
        return -1;
	}

	return ret;
}

/*****************************************************************************
* @brief    dev_snd_aic3x init/close function
* @section  [return] pcm handle
*****************************************************************************/
void *dev_snd_aic3x_init(int mode, int rate, int ch, int bufsz, int prd_sz)
{
	unsigned int freq, nchannels;
	int err;

	/* Handle for the PCM device */
	snd_pcm_t *handle = NULL;

	/* This structure contains information about    */
    /* the hardware and can be used to specify the  */
    /* configuration to be used for the PCM stream. */
	snd_pcm_hw_params_t *hw_params = NULL;
	snd_pcm_sw_params_t *sw_params = NULL;

	/* Name of the PCM device, like plughw:0,0          */
    /* The first number is the number of the soundcard, */
    /* the second number is the number of the device.   */
	char *pcm_name = SND_PCM_NAME_AIC3X;

	snd_pcm_uframes_t buffer_size, period_size;

	/* Open PCM. The last parameter of this function is the mode. */
    /* If this is set to 0, the standard mode is used. */
	err = snd_pcm_open(&handle,
					   pcm_name,
					   mode ? SND_PCM_STREAM_PLAYBACK : SND_PCM_STREAM_CAPTURE,
					   0);
	if (err < 0) {
		dev_err("Failed to open `%s':\n", pcm_name);
		return (void *)NULL;
	}

	freq = (unsigned int)rate;
	nchannels = (unsigned int)ch;

	snd_pcm_hw_params_alloca(&hw_params);
	/* Init hwparams with full configuration space */
	err = snd_pcm_hw_params_any(handle, hw_params);
	if (err < 0) {
		dev_err("Failed to initialize hardware parameters\n");
		goto err_aic3x_init;
	}

	/* Set access type.*/
	err = snd_pcm_hw_params_set_access(handle, hw_params,
							SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0) {
		dev_err("Failed to set access type\n");
		goto err_aic3x_init;
	}

	/* Set sample format */
	err = snd_pcm_hw_params_set_format(handle, hw_params,
							SND_PCM_FORMAT_S16_LE);
	if (err < 0) {
		dev_err("cannot set sample format!\n");
		goto err_aic3x_init;
	}

	/* Set sample rate. If the exact rate is not supported */
	/* by the hardware, use nearest possible rate.         */
	err = snd_pcm_hw_params_set_rate_near(handle, hw_params, &freq, 0);
	if (err < 0) {
		dev_err("Failed to set frequency %d\n", freq);
		goto err_aic3x_init;
	}

	/* Set number of channels */
	err = snd_pcm_hw_params_set_channels(handle, hw_params, nchannels);
	if (err < 0) {
		dev_err("Failed to set number of channels %d\n", nchannels);
		goto err_aic3x_init;
	}

	period_size = (snd_pcm_uframes_t)prd_sz;
    err = snd_pcm_hw_params_set_period_size_near(handle, hw_params, &period_size, 0);
    if (err < 0) {
        dev_err("Failed to set period size to %ld\n", period_size);
        goto err_aic3x_init;
    }

	buffer_size = (snd_pcm_uframes_t)bufsz;
    err = snd_pcm_hw_params_set_buffer_size_near(handle, hw_params, &buffer_size);
    if (err < 0) {
        dev_err("Failed to set buffer size to %ld\n", buffer_size);
        goto err_aic3x_init;
    }

	/* Apply HW parameter settings to */
    /* PCM device and prepare device  */
    err = snd_pcm_hw_params(handle, hw_params);
	if (err < 0) {
		dev_err("Unable to install hw params\n");
		goto err_aic3x_init;
	}

#if 1 //# for debugging.
	/* returned approximate maximum buffer size in frames  */
	snd_pcm_hw_params_get_period_size(hw_params, &period_size, 0);
	snd_pcm_hw_params_get_buffer_size(hw_params, &buffer_size);

	printf(" [AIC3X] ALSA Period Size %ld\n", period_size);
	printf(" [AIC3X] ALSA Buffer Size %ld\n", buffer_size);
#endif

	/* prepare for audio device */
	err = snd_pcm_prepare(handle);
    if (err < 0) {
        dev_err("Could not prepare handle %p\n", handle);
        goto err_aic3x_init;
    }

	/* set software params */
	snd_pcm_sw_params_alloca(&sw_params);
    err = snd_pcm_sw_params_current(handle, sw_params);
    if (err < 0) {
        dev_err("Failed to get current software parameters\n");
        goto err_aic3x_init;
    }

	snd_pcm_sw_params_set_avail_min(handle, sw_params, period_size);

	if (mode == SND_PCM_REC) {
	    snd_pcm_sw_params_set_start_threshold(handle, sw_params, 1);
	    snd_pcm_sw_params_set_stop_threshold(handle, sw_params, buffer_size);
	} else {
		snd_pcm_sw_params_set_start_threshold(handle, sw_params, buffer_size);
	    snd_pcm_sw_params_set_stop_threshold(handle, sw_params, buffer_size);
	}

    err = snd_pcm_sw_params(handle, sw_params);
    if (err < 0) {
        dev_err("Failed to set software parameters\n");
        goto err_aic3x_init;
    }

	return (void *)handle;

err_aic3x_init:
	dev_snd_aic3x_close((void *)handle, mode);

	return (void *)NULL;
}

void dev_snd_aic3x_close(void *hndl, int mode)
{
	if (hndl != NULL) {
		if (mode == SND_PCM_PLAY) {
			snd_pcm_nonblock(hndl, 0);

			/* Stop PCM device after pending frames have been played */
			snd_pcm_drain(hndl);
			//snd_pcm_nonblock(handle, 1);
		} else {
			alsa_mixer_cset(SND_PGA_CAPTURE_SWITCH, 0);
		}

		snd_pcm_hw_free(hndl);
		snd_pcm_close(hndl);
	}
}

/*****************************************************************************
* @brief    dev_snd_aic3x read/write function
* @section  [desc]
*****************************************************************************/
ssize_t dev_snd_aic3x_read(void *hndl, void *buf, int ch, int rcnt)
{
	snd_pcm_t *handle = (snd_pcm_t *)hndl;
	char *rbuf = (char *)buf;

	ssize_t result = 0;
	size_t count = (size_t)rcnt;
	int hwshift;

	hwshift = ch * (16 / 8);

	while (count > 0)
	{
		snd_pcm_sframes_t r;

		r = snd_pcm_readi(handle, rbuf, count);
		if (r <= 0)
		{
			switch (r) {
			case 0:
				dev_err(" [AIC3X] Failed to read frames(zero)\n");
				return -1;

			case -EAGAIN:
				//# dev_err(" [AIC3X] pcm wait (count = %d)!!\n", count);
				snd_pcm_wait(handle, 100);
				break;

			case -EPIPE:
				//# dev_err(" [AIC3X] pcm overrun(count = %d)!!\n", count);
				if (alsa_recover(handle)) {
					dev_err("Failed to read %d frames\n", count);
					return -1;
				}
				break;

			default:
				dev_err(" [AIC3X] read error!!\n");
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

ssize_t dev_snd_aic3x_write(void *hndl, void *buf, int ch, int wcnt, int period)
{
	snd_pcm_t *handle = (snd_pcm_t *)hndl;
	char *rbuf = (char *)buf;
	ssize_t result = 0;
	size_t count = (size_t)wcnt;

	int hwshift;

	hwshift = ch * (16 / 8); //# 16bit * channel / 8bit

	if (count < period) {
    	snd_pcm_format_set_silence(SND_PCM_FORMAT_S16_LE, rbuf + (count * hwshift),
						(period - count) * ch);
        count = period;
	}

	while (count > 0)
	{
		snd_pcm_sframes_t r;

		r = snd_pcm_writei(handle, rbuf, count);
		if (r == -EAGAIN || (r >= 0 && (size_t)r < count)) {
			//dev_err("pcm write wait(100ms)!!\n");
			snd_pcm_wait(handle, 100);
		} else if (r == -EPIPE) {
			//dev_err("pcm write underrun!!\n");
			if (alsa_recover(handle)) {
				dev_err("Failed to write %d frames\n", count);
				return -1;
			}
			continue;
		} else if (r == -ESTRPIPE) {
			if (alsa_resume(handle)) {
                dev_err("Failed to write %d frames\n", count);
                return -1;
            }
			continue;
		} else if (r < 0) {
			dev_err("write error\n");
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

/*****************************************************************************
* @brief    dev_snd_aic3x control functions
* @section  [desc]
*****************************************************************************/
/*----------------------------------------------------------------------------
 aix3x volume control
 - dir : capture(SND_VOLUME_C) or playback(SND_VOLUME_P)
 - volume : percentage, 0~100%
-----------------------------------------------------------------------------*/
int dev_snd_get_aic3x_volume(int dir, int *volume)
{
	long orig, pmin, pmax;

    snd_mixer_selem_id_t *sid;
    snd_mixer_elem_t* elem;
    snd_mixer_t *handle = NULL;

    snd_mixer_selem_id_alloca(&sid);

    /* set sset, index = 0(fixed) */
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, vol_ops[dir].vname);

    /* mixer open */
    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, "default");

    /* mixer basic mode (abstraction mode-> need to mixer option */
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    elem = snd_mixer_find_selem(handle, sid);
	if (!elem) {
		dev_err("Unable to find simple control '%s',%i\n", snd_mixer_selem_id_get_name(sid),
										snd_mixer_selem_id_get_index(sid));
		snd_mixer_close(handle);
		handle = NULL;
		return -1;
	}

	if (!vol_ops[dir].has_volume(elem)) {
		snd_mixer_close(handle);
		return -1;
	}

	vol_ops[dir].get_range(elem, &pmin, &pmax);
	vol_ops[dir].get(elem, 0, &orig);

	/* convert persentage */
	*volume = convert_prange(orig, pmin, pmax);
    snd_mixer_close(handle);

    return 0;
}

int dev_snd_set_aic3x_volume(int dir, int volume)
{
	long val, pmin, pmax;

    snd_mixer_selem_id_t *sid;
    snd_mixer_elem_t* elem;
    snd_mixer_t *handle = NULL;

    snd_mixer_selem_id_alloca(&sid);

    /* set sset, index = 0(fixed) */
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, vol_ops[dir].vname);

    /* mixer open */
    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, "default");

    /* mixer basic mode (abstraction mode-> need to mixer option */
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    elem = snd_mixer_find_selem(handle, sid);
	if (!elem) {
		dev_err("Unable to find simple control '%s',%i\n", snd_mixer_selem_id_get_name(sid),
															snd_mixer_selem_id_get_index(sid));
		snd_mixer_close(handle);
		handle = NULL;
		return -1;
	}

	if (!vol_ops[dir].has_volume(elem)) {
		snd_mixer_close(handle);
		return -1;
	}

	/* get range */
	vol_ops[dir].get_range(elem, &pmin, &pmax);
	val = (long)convert_prange1(volume, pmin, pmax);
	val = check_range(val, pmin, pmax);

	vol_ops[dir].set(elem, 0, val);
	vol_ops[dir].set(elem, 1, val); /* if stereo playback */
    snd_mixer_close(handle);

    return 0;
}

/*----------------------------------------------------------------------------
 dev_snd_aix3x input path control
 - path : SND_LINE_IN/SND_MIC_IN
-----------------------------------------------------------------------------*/
void dev_snd_set_aic3x_input_path(int path)
{
	//# capture switch off
	alsa_mixer_cset(SND_PGA_CAPTURE_SWITCH, SW_OFF);        //# 32

	alsa_mixer_cset(SND_LEFT_PGA_LINE1R_SWITCH, SW_OFF);   //# 90
	alsa_mixer_cset(SND_RIGHT_PGA_LINE1R_SWITCH, SW_OFF);  //# 83 (1R->right PGA)
	alsa_mixer_cset(SND_RIGHT_PGA_LINE1L_SWITCH, SW_OFF);  //# 84
	alsa_mixer_cset(SND_RIGHT_PGA_MIC3L_SWITCH, SW_OFF);   //# 86
	alsa_mixer_cset(SND_LEFT_PGA_MIC3L_SWITCH, SW_OFF);    //# 92

	if (path == SND_LINE_IN) {
		/* LINE_1LP PGA mixer on, not used LINE_1RP */
		alsa_mixer_cset(SND_LEFT_PGA_LINE1L_SWITCH, SW_ON);    //# 89 (1L->left PGA)
		alsa_mixer_cset(SND_RIGHT_PGA_MIC3R_SWITCH, SW_OFF);   //# 87

	} else {
		/* MIC2R/LINE2R, MIC2L/LINE2L PGA mixer on */
		alsa_mixer_cset(SND_LEFT_PGA_LINE1L_SWITCH, SW_OFF);   //# 89 (1L->left PGA)
		alsa_mixer_cset(SND_RIGHT_PGA_MIC3R_SWITCH, SW_ON);    //# 87
	}

	alsa_mixer_cset(SND_LEFT_LINE1L_MUX, 0); 					//# 97 (single ended)
	alsa_mixer_cset(SND_RIGHT_LINE1R_MUX, 0); 					//# 94 (single ended)

	//# capture switch on
	alsa_mixer_cset(SND_PGA_CAPTURE_SWITCH, SW_ON); //# 32
}

/*----------------------------------------------------------------------------
 dev_snd_aix3x output path control
 - path : SND_HEADPHONE_OUT/SND_LINE_OUT
-----------------------------------------------------------------------------*/
void dev_snd_set_aic3x_output_path(int path)
{
	if (path == SND_HEADPHONE_OUT) {
		/*
		 * HPLOUT / HPLCOM Differential
		 * HPROUT / HPRCOM Off
		 */
		alsa_mixer_cset(SND_HPCOM_PLAYBACK_SWITCH, 1);     	//# 22 HPLCOM, HPRCOM On
		alsa_mixer_cset(SND_LEFT_HPCOM_DACL1_SWITCH, 1);   	//# 53 DAC_L1 is routed to HPLCOM
		alsa_mixer_cset(SND_RIGHT_HPCOM_DACR1_SWITCH, 1);  	//# 61 DAC_R1 is routed to HPRCOM
		alsa_mixer_cset(SND_LINE_PLAYBACK_SWITCH, 0);     	//# 22 LEFT_LOP/RIGHT_ROP Off
		alsa_mixer_cset(SND_LEFT_DAC_MUX, 2);        		//# 101
		alsa_mixer_cset(SND_RIGHT_DAC_MUX, 2);       		//# 99
	} else {
		/*
		 * Left L+ / Left L- On
		 * Right R+ / Right R- Off
		 */
		alsa_mixer_cset(SND_HPCOM_PLAYBACK_SWITCH, 0);      //# 22 HPLCOM, HPRCOM Off
		alsa_mixer_cset(SND_LEFT_HPCOM_DACL1_SWITCH, 0);    //# 53 DAC_L1 is not routed to HPLCOM
		alsa_mixer_cset(SND_RIGHT_HPCOM_DACR1_SWITCH, 0);   //# 61 DAC_R1 is not routed to HPRCOM

		alsa_mixer_cset(SND_LINE_PLAYBACK_SWITCH, 1);       //# 20 LEFT_LOP/RIGHT_ROP On
		alsa_mixer_cset(SND_LEFT_DAC_MUX, 1);        		//# 101
		alsa_mixer_cset(SND_RIGHT_DAC_MUX, 1);       		//# 99
	}

	alsa_mixer_cset(SND_LEFT_HPCOM_MUX, 0);      			//# 100 HPLCOM Differential
	alsa_mixer_cset(SND_OUTPUT_POWERON_TIME, 5); 			//# 34 HPOUT Power On delay(50ms)
	alsa_mixer_cset(SND_OUTPUT_RAMPUP_STEP, 2);  			//# 35 Drvier Ramp-up delay(2ms)
}
