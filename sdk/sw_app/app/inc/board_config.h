/******************************************************************************
 * Copyright by	LF. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	board_config.h
 * @brief
 */
/*****************************************************************************/
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/*
 * LF_SYS_XXXX is defines in sw_mcfw/makerules/common_header_a8.mk
 */ 
#if defined(LF_SYS_NEXX360W)            //# NEXX360 Wireless
#	define NEXX360W						
#	define MODEL_NAME					"NEXX360W"
#	define MODEL_CH_NUM					4
#	define STREAM_CH_NUM				4
#	define JPEG_CH_NUM					5
#	define MAX_FPS						15
#	define SYS_CONFIG_VOIP				0        /* VOIP */
#	define SYS_CONFIG_WLAN				1        /* Wi-Fi */  
#	define SYS_CONFIG_GPS				1        /* GPS */
#	define SYS_CONFIG_SND_OUT			0        /* SOUND Output */
#	define SYS_MCU_VER					0x0020
#elif defined(LF_SYS_NEXX360B)          //# NEXX360 Basic
#	define NEXX360B
#	define MODEL_NAME					"NEXX360B"
#	define MODEL_CH_NUM					4
#	define STREAM_CH_NUM				4
#	define JPEG_CH_NUM					5
#	define MAX_FPS						15	
#	define SYS_CONFIG_VOIP				0
#	define SYS_CONFIG_WLAN				0
#	define SYS_CONFIG_GPS				1
#	define SYS_CONFIG_SND_OUT			0
#	define SYS_MCU_VER					0x0020
#elif defined(LF_SYS_NEXX360M)          //# NEXX360 Basic + Military
#	define NEXX360M
#	define MODEL_NAME					"NEXX360M"
#	define MODEL_CH_NUM					4
#	define STREAM_CH_NUM				4
#	define JPEG_CH_NUM					5
#	define MAX_FPS						15	
#	define SYS_CONFIG_VOIP				0
#	define SYS_CONFIG_WLAN				0
#	define SYS_CONFIG_GPS				1
#	define SYS_CONFIG_SND_OUT			0
#	define SYS_MCU_VER					0x0020
#elif defined(LF_SYS_NEXXB)
#	define NEXXB
#	define MODEL_NAME					"NEXXB"
#	define MODEL_CH_NUM					4
#	define STREAM_CH_NUM				4
#	define JPEG_CH_NUM					5
#	define MAX_FPS						15	
#	define SYS_CONFIG_VOIP				1
#	define SYS_CONFIG_WLAN				1
#	define SYS_CONFIG_GPS				1
#	define SYS_CONFIG_SND_OUT			1
#	define SYS_MCU_VER					0x0010
#elif defined(LF_SYS_NEXXB_ONE)
#	define NEXXB_ONE						
#	define MODEL_NAME					"NEXXB_ONE"
#	define MODEL_CH_NUM					1
#	define STREAM_CH_NUM				1
#	define JPEG_CH_NUM					2
#	define MAX_FPS						30	
#	define SYS_CONFIG_VOIP				1
#	define SYS_CONFIG_WLAN				1
#	define SYS_CONFIG_GPS				1
#	define SYS_CONFIG_SND_OUT			1
#	define SYS_MCU_VER					0x0010
#elif defined(LF_SYS_NEXXONE_VOIP)
#	define NEXXONE						
#	define MODEL_NAME					"NEXXONE"
#	define MODEL_CH_NUM					1
#	define STREAM_CH_NUM				1
#	define JPEG_CH_NUM					2
#	define MAX_FPS						30	
#	define SYS_CONFIG_VOIP				1
#	define SYS_CONFIG_WLAN				1
#	define SYS_CONFIG_GPS				1
#	define SYS_CONFIG_SND_OUT			1
#	define SYS_MCU_VER					0x0020
#elif defined(LF_SYS_NEXX360W_MUX)
#	define NEXX360W_MUX						
#	define MODEL_NAME					"NEXX360W_MUX"
#	define MODEL_CH_NUM					1 
#	define STREAM_CH_NUM				1 
#	define JPEG_CH_NUM					2 
#	define MAX_FPS						15
#	define SYS_CONFIG_VOIP				0
#	define SYS_CONFIG_WLAN				1
#	define SYS_CONFIG_GPS				1
#	define SYS_CONFIG_SND_OUT			0
#	define SYS_MCU_VER					0x0020
#elif defined(LF_SYS_NEXX360C)          //# NEXX360 CCTV (based on nexx360 basic + external battery)
#	define NEXX360C						
#	define MODEL_NAME					"NEXX360C"
#	define MODEL_CH_NUM					4 
#	define STREAM_CH_NUM				4 
#	define JPEG_CH_NUM					5 
#	define MAX_FPS						15
#	define SYS_CONFIG_VOIP				0
#	define SYS_CONFIG_WLAN				0
#	define SYS_CONFIG_GPS				0
#	define SYS_CONFIG_SND_OUT			0
#	define SYS_MCU_VER					0x0020
#elif defined(LF_SYS_NEXX360W_CCTV)     //# NEXX360 Wireless CCTV (external battery)
#	define NEXX360W_CCTV						
#	define MODEL_NAME					"NEXX360W_CCTV"
#	define MODEL_CH_NUM					4 
#	define STREAM_CH_NUM				4 
#	define JPEG_CH_NUM					5 
#	define MAX_FPS						15
#	define SYS_CONFIG_VOIP				0
#	define SYS_CONFIG_WLAN				1
#	define SYS_CONFIG_GPS				0
#	define SYS_CONFIG_SND_OUT			1
#	define SYS_MCU_VER					0x0020
#else
    #error "Not Defined SYSTEM_PLATFORM in Rules.make."
#endif

#if defined(NEXX360W_MUX)
#define EXCHANNEL  3
#else
#define EXCHANNEL  0
#endif

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#endif	/* __BOARD_CONFIG_H__ */
