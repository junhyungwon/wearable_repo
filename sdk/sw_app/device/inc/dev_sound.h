/*
 * dev_sound.h
 *
 * Copyright (C) 2013 UDWORKs.
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
 * sound Hardware library interface definitions.
 */

#ifndef _DEV_SOUND_H_
#define _DEV_SOUND_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <alsa/asoundlib.h>

#define SND_VOLUME_P		0 /* playback volume */
#define SND_VOLUME_C		1 /* capture  volume */

#define SND_LINE_IN			0
#define SND_MIC_IN			1

#define SND_HEADPHONE_OUT	0
#define SND_LINE_OUT		1

#define SND_HEADER_SIZE		0x200//0x2C

#define SND_PCM_REC			0
#define SND_PCM_PLAY		1

void *dev_snd_aic3x_init(int mode, int rate, int ch, int bufsz, int prd_sz);
void dev_snd_aic3x_close(void *hndl, int mode);
ssize_t dev_snd_aic3x_read(void *hndl, void *buf, int ch, int rcnt);
ssize_t dev_snd_aic3x_write(void *hndl, void *buf, int ch, int wcnt, int period);
int dev_snd_get_aic3x_volume(int dir, int *volume);
int dev_snd_set_aic3x_volume(int dir, int volume);
void dev_snd_set_aic3x_input_path(int path);
void dev_snd_set_aic3x_output_path(int path);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif //# _DEV_SOUND_H_

