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
#if defined(LF_SYS_NEXX360_VOIP)
#	define NEXX360						
#	define MODEL_NAME					"NEXX360"
#	define MODEL_CH_NUM					4
#	define MAX_FPS						15
#	define SYS_CONFIG_VOIP				1
#	define SYS_CONFIG_NET				1
#elif defined(LF_SYS_NEXX360)
#	define NEXX360
#	define MODEL_NAME					"NEXX360"
#	define MODEL_CH_NUM					4
#	define MAX_FPS						15	
#	define SYS_CONFIG_VOIP				0
#	define SYS_CONFIG_NET				1
#elif defined(LF_SYS_NEXXONE_VOIP)
#	define NEXXONE						
#	define MODEL_NAME					"NEXXONE"
#	define MODEL_CH_NUM					1
#	define MAX_FPS						30	
#	define SYS_CONFIG_VOIP				1
#	define SYS_CONFIG_NET				1
#elif defined(LF_SYS_FITT360)
#	define FITT360_SECURITY
#	define MODEL_NAME					"FITT360 Security"
#	define MODEL_CH_NUM					4
#	define MAX_FPS						30
#	define SYS_CONFIG_VOIP				1
#	define SYS_CONFIG_NET				1
#elif defined(LF_SYS_FITT360_BASIC)
#	define FITT360_SECURITY
#	define MODEL_NAME					"FITT360 Security"
#	define MODEL_CH_NUM					4
#	define MAX_FPS						30
#	define SYS_CONFIG_VOIP				0
#	define SYS_CONFIG_NET				0
#else
    #error "Not Defined SYSTEM_PLATFORM in Rules.make."
#endif

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

#endif	/* __BOARD_CONFIG_H__ */
