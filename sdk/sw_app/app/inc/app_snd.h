/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_snd.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_SND_H_
#define _APP_SND_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define APP_SND_PCM_BITS		16
#define APP_SND_CAPT_DEV		0   //# recording device
#define APP_SND_PLAY_DEV		1   //# playback device

/* CAPTURE (MIC) SOUND PARAM */
#define APP_SND_SRATE			8000  //# for baresip
#define APP_SND_PTIME			80    //# 고정해야 함.
#define APP_SND_CH				1     /* sound channel */

/* PLAYBACK (BACKCHANNEL) SOUND PARAM */
#define APP_SND_BC_SRATE		8000  //# for baresip
#define APP_SND_BC_PTIME		80    //# 고정해야 함.
#define APP_SND_BC_CH			1     /* sound channel */

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_snd_start(void);
void app_snd_stop(void);

#endif	/* _APP_SND_H_ */
