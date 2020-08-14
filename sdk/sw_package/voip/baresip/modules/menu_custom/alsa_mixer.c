/**
 * @file menu_custom/alsa_mixer.h
 *
 * Copyright (C) 2020 LF
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "alsa_mixer.h"
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

#define SW_ON		                (1)
#define SW_OFF		                (0)

/* alsa amixer enum */
#define SND_LINE_PLAYBACK_SWITCH		20 //# boolean
#define SND_HPCOM_PLAYBACK_SWITCH		22 //# boolean
#define SND_PGA_CAPTURE_SWITCH			32 //# boolean
#define SND_OUTPUT_POWERON_TIME			34 //# enum (headphone driver power on time)
#define SND_OUTPUT_RAMPUP_STEP			35 //# enum (0->0ms, 1->1ms, 2->2ms, 3->4ms)
#define SND_LEFT_HPCOM_DACL1_SWITCH		53 //# boolean
#define SND_RIGHT_HPMIX_DACL1_SWITCH	59 //# boolean (default off)
#define SND_RIGHT_HPCOM_DACR1_SWITCH	61 //# boolean
#define SND_LEFT_HPMIX_DACL1_SWITCH		65 //# boolean (default on)

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

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

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
 Declares a function prototype
-----------------------------------------------------------------------------*/

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
static int amixer_cset(int numid, int value)
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
		return -1;
	}

	snd_ctl_elem_info_alloca(&info);
	snd_ctl_elem_id_alloca(&id);
	snd_ctl_elem_value_alloca(&control);

	/* default */
	snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);
	snd_ctl_elem_id_set_numid(id, numid);

	if ((err = snd_ctl_open(&handle, "default", 0)) < 0) {
		return err;
	}

	snd_ctl_elem_info_set_id(info, id);
	if ((err = snd_ctl_elem_info(handle, info)) < 0) {
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
		snd_ctl_close(handle);
		return err;
	}

	snd_ctl_close(handle);
	return 0;
}

/*
 * set auido volume (input -> percent)
 */ 
int amixer_set_volume(int dir, int percent)
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
	val = (long)convert_prange1(percent, pmin, pmax);
	val = check_range(val, pmin, pmax);

	vol_ops[dir].set(elem, 0, val);
	vol_ops[dir].set(elem, 1, val); /* if stereo playback */
    snd_mixer_close(handle);

    return 0;
}

/*----------------------------------------------------------------------------
 dev_snd_aix3x input path control
 - path : set mic2 path
-----------------------------------------------------------------------------*/
void amixer_set_input_path(void)
{
	//# capture switch off
	amixer_cset(SND_PGA_CAPTURE_SWITCH, SW_OFF);        //# 32
	amixer_cset(SND_LEFT_PGA_LINE1R_SWITCH, SW_OFF);   //# 90
	amixer_cset(SND_RIGHT_PGA_LINE1R_SWITCH, SW_OFF);  //# 83 (1R->right PGA)
	amixer_cset(SND_RIGHT_PGA_LINE1L_SWITCH, SW_OFF);  //# 84
	amixer_cset(SND_RIGHT_PGA_MIC3L_SWITCH, SW_OFF);   //# 86
	amixer_cset(SND_LEFT_PGA_MIC3L_SWITCH, SW_OFF);    //# 92
	amixer_cset(SND_LEFT_PGA_LINE1L_SWITCH, SW_OFF);   //# 89 (1L->left PGA)
	
	/* MIC2R/LINE2R, MIC2L/LINE2L PGA mixer on */
	amixer_cset(SND_RIGHT_PGA_MIC3R_SWITCH, SW_ON);    //# 87
	amixer_cset(SND_LEFT_PGA_MIC3R_SWITCH, SW_ON);     //# 93
	
	amixer_cset(SND_LEFT_LINE1L_MUX, 0); 				//# 97 (single ended)
	amixer_cset(SND_RIGHT_LINE1R_MUX, 0); 				//# 94 (single ended)

	//# capture switch on
	amixer_cset(SND_PGA_CAPTURE_SWITCH, SW_ON); //# 32
}

/*----------------------------------------------------------------------------
 dev_snd_aix3x output path control
 - path : set line-out path
-----------------------------------------------------------------------------*/
void amixer_set_output_path(void)
{
	#if 0
	/*
	 * Left L+ / Left L- On
	 * Right R+ / Right R- Off
	 */
	amixer_cset(SND_HPCOM_PLAYBACK_SWITCH, 0);      //# 22 HPLCOM, HPRCOM Off
	amixer_cset(SND_LEFT_HPCOM_DACL1_SWITCH, 0);    //# 53 DAC_L1 is not routed to HPLCOM
	amixer_cset(SND_RIGHT_HPCOM_DACR1_SWITCH, 0);   //# 61 DAC_R1 is not routed to HPRCOM
	
	amixer_cset(SND_RIGHT_HPMIX_DACL1_SWITCH, 1);   //# 59 DAC_L1 is HPLOUT
	amixer_cset(SND_LEFT_HPMIX_DACL1_SWITCH, 1);    //# 65 DAC_L1 is HPROUT
		
	amixer_cset(SND_LINE_PLAYBACK_SWITCH, 1);       //# 20 LEFT_LOP/RIGHT_ROP On
	amixer_cset(SND_LEFT_DAC_MUX, 0);        		//# 101
	amixer_cset(SND_RIGHT_DAC_MUX, 1);       		//# 99
	#else
	amixer_cset(SND_HPCOM_PLAYBACK_SWITCH, 1);      //# 22 HPLCOM, HPRCOM Off
	amixer_cset(SND_LEFT_HPCOM_DACL1_SWITCH, 1);    //# 53 DAC_L1 is not routed to HPLCOM
	amixer_cset(SND_RIGHT_HPCOM_DACR1_SWITCH, 1);   //# 61 DAC_R1 is not routed to HPRCOM
	
	amixer_cset(SND_RIGHT_HPMIX_DACL1_SWITCH, 0);   //# 59 DAC_L1 is HPLOUT
	amixer_cset(SND_LEFT_HPMIX_DACL1_SWITCH, 0);    //# 65 DAC_L1 is HPROUT
		
	amixer_cset(SND_LINE_PLAYBACK_SWITCH, 0);       //# 20 LEFT_LOP/RIGHT_ROP On
	amixer_cset(SND_LEFT_DAC_MUX, 2);        		//# 101
	amixer_cset(SND_RIGHT_DAC_MUX, 2);       		//# 99
	#endif
	amixer_cset(SND_LEFT_HPCOM_MUX, 0);      			//# 100 HPLCOM Differential
	amixer_cset(SND_OUTPUT_POWERON_TIME, 5); 			//# 34 HPOUT Power On delay(50ms)
	amixer_cset(SND_OUTPUT_RAMPUP_STEP, 2);  			//# 35 Drvier Ramp-up delay(2ms)
}
