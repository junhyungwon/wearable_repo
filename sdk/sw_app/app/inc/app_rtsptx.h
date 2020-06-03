/******************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_rtsptx.h
 * @brief
 * @author	sksung
 * @section	MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_RTSPTX_H_
#define _APP_RTSPTX_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

//#define RTSP_ENCRYT


//# define enum of mem_mng.h
typedef enum {
    FTYPE_AUD = 0,		//# AUDIO_FRAME
    FTYPE_VID_I,		//# I_FRAME
    FTYPE_VID_P,		//# P_FRAME
    MAX_FTYPE
} frame_type_e;

typedef enum {
	STYPE_VID_CH1 = 0,	//# VIDOE_INFO_CH1
    STYPE_VID_CH2,		//# VIDOE_INFO_CH2
	STYPE_AUD_CH1,		//# AUDIO_INFO_G711_CH1
    STYPE_AUD_CH2,		//# AUDIO_INFO_G711_CH2
	MAX_STYPE
} stream_type_e;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_rtsptx_start(void);
int app_rtsptx_stop(void);
int app_rtsptx_stop_start(void);
int app_rtsptx_write(void *addr, int offset, int size, int frametype, int strmtype, unsigned int timestamp);

#endif	/* _APP_RTSPTX_H_ */
