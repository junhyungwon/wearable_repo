/** ==================================================================
 *  @file   emu_cm_cred.h                                                  
 *                                                                    
 *  @path   /ti/psp/iss/hal/cred_cram/cred/inc/cortexm3_sys_ape/                                                  
 *                                                                    
 *  @desc   This  File contains.                                      
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012                    
 *                                                                    
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

/* ============================================================================ 
 * TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION Property of Texas
 * Instruments For Unrestricted Internal Use Only Unauthorized reproduction
 * and/or distribution is strictly prohibited.  This product is protected
 * under copyright law and trade secret law as an unpublished work.  Created
 * 2008, (C) Copyright 2008 Texas Instruments.  All rights reserved. */

/**
 *  @Component:   EMU_CM
 *
 *  @Filename:    emu_cm_cred.h
 *
 *  @Description: Component description is not available
 *
 *  Generated by: Socrates CRED generator prototype
 *
    *//* ====================================================================== */

#ifndef __EMU_CM_CRED_H
#define __EMU_CM_CRED_H

#ifdef __cplusplus
extern "C" {
#endif

    /* 
     * Instance EMU_CM of component EMU_CM mapped in MONICA at address 0x4A307A00
     */

                                                                              /*-------------------------------------------------------------------------*//**
 * @DEFINITION   BITFIELD
 *
 * @BRIEF        The bitfield must be defined according to register width
 *               of the component - 64/32/16/8
 *
    *//*------------------------------------------------------------------------ */
#undef BITFIELD
#define BITFIELD BITFIELD_32

    /* 
     *  List of Register arrays for component EMU_CM
     *
     */

    /* 
     *  List of bundle arrays for component EMU_CM
     *
     */

    /* 
     *  List of bundles for component EMU_CM
     *
     */

    /* 
     * List of registers for component EMU_CM
     *
     */

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL
 *
 * @BRIEF        This register enables the EMU domain power state transition. 
 *               It controls the HW supervised domain power state transition 
 *               between ON-ACTIVE and ON-INACTIVE states. It also hold one 
 *               status bit per clock input of the domain. 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL                           0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DYNAMICDEP
 *
 * @BRIEF        This register controls the dynamic domain depedencies from 
 *               EMU domain towards 'target' domains. It is relevant only for 
 *               domain having OCP master port(s). 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DYNAMICDEP                          0x8ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL
 *
 * @BRIEF        This register manages the DEBUGSS clocks. 
 *               [warm reset insensitive] 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL                     0x20ul

    /* 
     * List of register bitfields for component EMU_CM
     *
     */

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_PER_DPLL_EMU_CLK   
 *
 * @BRIEF        This field indicates the state of the PER_DPLL_EMU_CLK clock 
 *               in the domain. 
 *               [warm reset insensitive] - (RO) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_PER_DPLL_EMU_CLK BITFIELD(10, 10)
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_PER_DPLL_EMU_CLK__POS 10

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_CORE_DPLL_EMU_CLK   
 *
 * @BRIEF        This field indicates the state of the CORE_DPLL_EMU_CLK 
 *               clock in the domain. 
 *               [warm reset insensitive] - (RO) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_CORE_DPLL_EMU_CLK BITFIELD(9, 9)
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_CORE_DPLL_EMU_CLK__POS 9

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_EMU_SYS_CLK   
 *
 * @BRIEF        This field indicates the state of the EMU_SYS_CLK clock in 
 *               the domain. 
 *               [warm reset insensitive] - (RO) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_EMU_SYS_CLK BITFIELD(8, 8)
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_EMU_SYS_CLK__POS 8

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL   
 *
 * @BRIEF        Controls the clock state transition of the EMU clock domain. 
 *               - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL           BITFIELD(1, 0)
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL__POS      0

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DYNAMICDEP__WINDOWSIZE   
 *
 * @BRIEF        Size of sliding window  used to monitor OCP interface 
 *               activity for determination of auto-sleep feature. Time unit 
 *               defined by CM_DYN_DEP_PRESCAL register. - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DYNAMICDEP__WINDOWSIZE         BITFIELD(27, 24)
#define EMU_CM__CM_EMU_DYNAMICDEP__WINDOWSIZE__POS    24

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DYNAMICDEP__L3_2_DYNDEP   
 *
 * @BRIEF        Dynamic dependency towards L3_2 clock domain - (RO) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DYNAMICDEP__L3_2_DYNDEP        BITFIELD(6, 6)
#define EMU_CM__CM_EMU_DYNAMICDEP__L3_2_DYNDEP__POS   6

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK   
 *
 * @BRIEF        Selection of STM clock division - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK BITFIELD(29, 27)
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__POS 27

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK   
 *
 * @BRIEF        Selection of TRACE clock division - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK BITFIELD(26, 24)
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__POS 24

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL   
 *
 * @BRIEF        Selection of TRACE source clock - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL BITFIELD(23, 22)
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL__POS 22

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL   
 *
 * @BRIEF        Selection of STM source clock - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL BITFIELD(21, 20)
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL__POS 20

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__STBYST   
 *
 * @BRIEF        Module standby status - (RO) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__STBYST        BITFIELD(18, 18)
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__STBYST__POS   18

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST   
 *
 * @BRIEF        Module idle status - (RO) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST        BITFIELD(17, 16)
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST__POS   16

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__MODULEMODE   
 *
 * @BRIEF        Control the way mandatory clocks are managed. - (RO) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__MODULEMODE    BITFIELD(1, 0)
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__MODULEMODE__POS 0

    /* 
     * List of register bitfields values for component EMU_CM
     *
     */

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_PER_DPLL_EMU_CLK__INACT
 *
 * @BRIEF        Corresponding clock is definitely gated - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_PER_DPLL_EMU_CLK__INACT 0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_PER_DPLL_EMU_CLK__ACT
 *
 * @BRIEF        Corresponding clock is running or gating/ungating transition 
 *               is on-going - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_PER_DPLL_EMU_CLK__ACT 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_CORE_DPLL_EMU_CLK__INACT
 *
 * @BRIEF        Corresponding clock is definitely gated - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_CORE_DPLL_EMU_CLK__INACT 0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_CORE_DPLL_EMU_CLK__ACT
 *
 * @BRIEF        Corresponding clock is running or gating/ungating transition 
 *               is on-going - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_CORE_DPLL_EMU_CLK__ACT 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_EMU_SYS_CLK__INACT
 *
 * @BRIEF        Corresponding clock is definitely gated - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_EMU_SYS_CLK__INACT 0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_EMU_SYS_CLK__ACT
 *
 * @BRIEF        Corresponding clock is running or gating/ungating transition 
 *               is on-going - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKACTIVITY_EMU_SYS_CLK__ACT 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL__RESERVED
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL__RESERVED 0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL__RESERVED_1
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL__RESERVED_1 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL__SW_WKUP
 *
 * @BRIEF        SW_WKUP: Start a software forced wake-up transition on the 
 *               domain. - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL__SW_WKUP  0x2ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL__HW_AUTO
 *
 * @BRIEF        HW_AUTO: Automatic transition is enabled. Sleep and wakeup 
 *               transition are based upon hardware conditions. - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_CLKSTCTRL__CLKTRCTRL__HW_AUTO  0x3ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DYNAMICDEP__L3_2_DYNDEP__ENABLED
 *
 * @BRIEF        Dependency is enabled - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DYNAMICDEP__L3_2_DYNDEP__ENABLED 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__RESERVED_0
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__RESERVED_0 0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__0X1
 *
 * @BRIEF        STM_CLK is the selected STM source clock divided by 1 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__0X1 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__0X2
 *
 * @BRIEF        STM_CLK is the selected STM source clock divided by 2 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__0X2 0x2ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__RESERVED_3
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__RESERVED_3 0x3ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__0X4
 *
 * @BRIEF        STM_CLK is the selected STM source clock divided by 4 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__0X4 0x4ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__RESERVED_5
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__RESERVED_5 0x5ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__RESERVED_6
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__RESERVED_6 0x6ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__RESERVED_7
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_STM_CLK__RESERVED_7 0x7ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__RESERVED_0
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__RESERVED_0 0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__0X1
 *
 * @BRIEF        TRACE_CLK is the selected TRACE source clock divided by 1 - 
 *               (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__0X1 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__0X2
 *
 * @BRIEF        TRACE_CLK is the selected TRACE source clock divided by 2 - 
 *               (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__0X2 0x2ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__RESERVED_3
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__RESERVED_3 0x3ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__0X4
 *
 * @BRIEF        TRACE_CLK is the selected TRACE source clock divided by 4 - 
 *               (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__0X4 0x4ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__RESERVED_5
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__RESERVED_5 0x5ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__RESERVED_6
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__RESERVED_6 0x6ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__RESERVED_7
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__CLKSEL_PMD_TRACE_CLK__RESERVED_7 0x7ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL__SYS_CLK
 *
 * @BRIEF        TRACE source clock is SYS_CLK - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL__SYS_CLK 0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL__CORE_DPLL_CLK
 *
 * @BRIEF        TRACE source clock is CORE_DPLL_EMU_CLK - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL__CORE_DPLL_CLK 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL__PER_DPLL_CLK
 *
 * @BRIEF        TRACE source clock is PER_DPLL_EMU_CLK - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL__PER_DPLL_CLK 0x2ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL__RESERVED
 *
 * @BRIEF        Enumeration value description is not available - (Read)
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_TRACE_MUX_CTRL__RESERVED 0x3ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL__SYS_CLK
 *
 * @BRIEF        STM source clock is SYS_CLK - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL__SYS_CLK 0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL__CORE_DPLL_CLK
 *
 * @BRIEF        STM source clock is CORE_DPLL_EMU_CLK - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL__CORE_DPLL_CLK 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL__PER_DPLL_CLK
 *
 * @BRIEF        STM source clock is PER_DPLL_EMU_CLK - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL__PER_DPLL_CLK 0x2ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL__RESERVED
 *
 * @BRIEF        Reserved - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__PMD_STM_MUX_CTRL__RESERVED 0x3ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__STBYST__FUNC
 *
 * @BRIEF        Module is functional (not in standby) - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__STBYST__FUNC  0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__STBYST__STANDBY
 *
 * @BRIEF        Module is in standby - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__STBYST__STANDBY 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST__FUNC
 *
 * @BRIEF        Module is fully functional, including OCP - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST__FUNC  0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST__TRANS
 *
 * @BRIEF        Module is performing transition: wakeup, or sleep, or sleep 
 *               abortion - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST__TRANS 0x1ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST__IDLE
 *
 * @BRIEF        Module is in Idle mode (only OCP part). It is functional if 
 *               using separate functional clock - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST__IDLE  0x2ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST__DISABLE
 *
 * @BRIEF        Module is disabled and cannot be accessed - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__IDLEST__DISABLE 0x3ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__MODULEMODE__AUTO
 *
 * @BRIEF        Module is managed automatically by HW according to clock 
 *               domain transition. A clock domain sleep transition put 
 *               module into idle. A wakeup domain transition put it back 
 *               into function. If CLKTRCTRL=3, any OCP access to module is 
 *               always granted. Module clocks may be gated according to the 
 *               clock domain state. - (Read) 
 *
    *//*------------------------------------------------------------------------ */
#define EMU_CM__CM_EMU_DEBUGSS_CLKCTRL__MODULEMODE__AUTO 0x1ul

#ifdef __cplusplus
}
#endif
#endif                                                     /* __EMU_CM_CRED_H 
                                                            */
