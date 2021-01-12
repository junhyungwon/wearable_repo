/*
 * dev_snd.h
 *
 * Copyright (C) 2021 LF.
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
 */

#ifndef _DEV_SND_H_
#define _DEV_SND_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <alsa/asoundlib.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define SND_PCM_BITS		16 /* bits per sample */
#define SND_VOLUME_P		0 /* playback volume */
#define SND_VOLUME_C		1 /* capture  volume */

#define SND_LINE_IN			0
#define SND_MIC_IN			1

#define SND_HEADPHONE_OUT	0
#define SND_LINE_OUT		1

#define SND_HEADER_SIZE		0x200//0x2C

#define SND_PCM_CAP			0
#define SND_PCM_PLAY		1

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

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
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int dev_snd_get_volume(int dir, int *volume);
int dev_snd_set_volume(int dir, int volume);
int dev_snd_set_param(const char *name, snd_prm_t *prm, int mode, int ch, int rate, int ptime);
void dev_snd_set_swparam(snd_prm_t *prm, int mode);
void dev_snd_param_free(snd_prm_t *prm);
int dev_snd_open(const char *pcm_name, snd_prm_t *prm);
void dev_snd_start(snd_prm_t *prm);
void dev_snd_stop(snd_prm_t *prm, int mode);
ssize_t dev_snd_read(snd_prm_t *prm);
ssize_t dev_snd_write(snd_prm_t *prm, size_t w_samples);
void dev_snd_set_input_path(int path);
void dev_snd_set_output_path(int path);
					
#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif //# _DEV_SND_H_

