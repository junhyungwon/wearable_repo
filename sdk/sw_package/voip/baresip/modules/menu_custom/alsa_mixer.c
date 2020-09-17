/*
 * File : alsa_mixer.c
 *
 * Copyright (C) 2020 LF
 *
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

/*----------------------------------------------------------------------------
 aix3x volume control
 - dir : capture(SND_VOLUME_C) or playback(SND_VOLUME_P)
 - volume : percentage, 0~100%
-----------------------------------------------------------------------------*/
int alsa_mixer_get_volume(int dir, int *volume)
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

int alsa_mixer_set_volume(int dir, int volume)
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
	val = (long)convert_prange1(volume, pmin, pmax);
	val = check_range(val, pmin, pmax);

	vol_ops[dir].set(elem, 0, val);
	vol_ops[dir].set(elem, 1, val); /* if stereo playback */
    snd_mixer_close(handle);

    return 0;
}
