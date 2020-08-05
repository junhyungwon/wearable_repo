/**
 * @file menu_custom/alsa_mixer.h
 *
 * Copyright (C) 2020 LF
 */

#ifndef __ALSA_MiXER_H__
#define __ALSA_MiXER_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <alsa/asoundlib.h>

#define SND_VOLUME_P		0 /* playback volume */
#define SND_VOLUME_C		1 /* capture  volume */

int amixer_set_volume(int dir, int percent);
void amixer_set_input_path(void);
void amixer_set_output_path(void);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif //# __ALSA_MiXER_H__

