/*
 * alsa_mixer.h
 *
 * Copyright (C) 2020 LF.
 */

#ifndef __ALSA_MIXER_H_
#define __ALSA_MIXER_H_

#include <alsa/asoundlib.h>

#define SND_VOLUME_P		0 /* playback volume */
#define SND_VOLUME_C		1 /* capture  volume */

int alsa_mixer_get_volume(int dir, int *volume);
int alsa_mixer_set_volume(int dir, int volume);

#endif //# __ALSA_MIXER_H_
