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
#if defined(LF_SYS_NEXX360W)
#	define NEXX360W						
#	define REC_CH_NUM					4
#	define MAX_FPS						15
#elif defined(LF_SYS_NEXXB)
#	define NEXXB
#	define REC_CH_NUM					3
#	define MAX_FPS						15
#elif defined(LF_SYS_NEXXB_ONE)
#	define NEXXB_ONE
#	define REC_CH_NUM					1
#	define MAX_FPS						30
#elif defined(LF_SYS_NEXX360B)
#	define NEXX360B
#	define REC_CH_NUM					4
#	define MAX_FPS						15	
#elif defined(LF_SYS_NEXX360M)
#	define NEXX360M
#	define REC_CH_NUM					4
#	define MAX_FPS						15
#elif defined(LF_SYS_NEXXONE_VOIP)
#	define NEXXONE						
#	define REC_CH_NUM					1
#	define MAX_FPS						30	
#elif defined(LF_SYS_NEXX360W_MUX)
#	define NEXX360W_MUX				
#	define REC_CH_NUM					1
#	define MAX_FPS						15
#elif defined(LF_SYS_NEXX360C)
#	define NEXX360C
#	define REC_CH_NUM					4
#	define MAX_FPS						15
#elif defined(LF_SYS_NEXX360W_CCTV)
#	define NEXX360W_CCTV
#	define REC_CH_NUM					4
#	define MAX_FPS						15
#elif defined(LF_SYS_NEXX360W_CCTV_SA)
#	define NEXX360W_CCTV_SA
#	define REC_CH_NUM					4
#	define MAX_FPS						15
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
