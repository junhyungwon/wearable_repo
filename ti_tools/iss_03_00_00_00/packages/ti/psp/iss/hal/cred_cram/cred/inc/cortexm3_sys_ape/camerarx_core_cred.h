/** ==================================================================
 *  @file   camerarx_core_cred.h                                                  
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
 *  @Component:   CAMERARX_CORE
 *
 *  @Filename:    camerarx_core_cred.h
 *
 *  @Description: Component description is not available
 *
 *  Generated by: Socrates CRED generator prototype
 *
    *//* ====================================================================== */

#ifndef __CAMERARX_CORE_CRED_H
#define __CAMERARX_CORE_CRED_H

#ifdef __cplusplus
extern "C" {
#endif

    /* 
     * Instance RX_PHY of component CAMERARX_CORE mapped in MONICA at address 0x4A068170
     * Instance CAMERARX_CORE1 of component CAMERARX_CORE mapped in MONICA at address 0x55041170
     * Instance CAMERARX_CORE2 of component CAMERARX_CORE mapped in MONICA at address 0x55041570
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
     *  List of Register arrays for component CAMERARX_CORE
     *
     */

    /* 
     *  List of bundle arrays for component CAMERARX_CORE
     *
     */

    /* 
     *  List of bundles for component CAMERARX_CORE
     *
     */

    /* 
     * List of registers for component CAMERARX_CORE
     *
     */

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER0
 *
 * @BRIEF        First Register 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER0                           0x0ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER1
 *
 * @BRIEF        Second Register 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER1                           0x4ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER2
 *
 * @BRIEF        Third register 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER2                           0x8ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3
 *
 * @BRIEF        Fourth Register 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3                           0xCul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER4
 *
 * @BRIEF        Fifth Register 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER4                           0x10ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER5
 *
 * @BRIEF        Sixth Register 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER5                           0x14ul

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER6
 *
 * @BRIEF        Seventh Register 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER6                           0x18ul

    /* 
     * List of register bitfields for component CAMERARX_CORE
     *
     */

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER0__RESERVED1   
 *
 * @BRIEF        Reserved fields - (NA) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER0__RESERVED1           BITFIELD(31, 25)
#define CAMERARX_CORE__REGISTER0__RESERVED1__POS      25

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER0__HSCLOCKCONFIG   
 *
 * @BRIEF        Disable clock missing detector - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER0__HSCLOCKCONFIG       BITFIELD(24, 24)
#define CAMERARX_CORE__REGISTER0__HSCLOCKCONFIG__POS  24

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER0__THS_TERM   
 *
 * @BRIEF        Ths-Term timing parameter in multiples of DDR clock - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER0__THS_TERM            BITFIELD(15, 8)
#define CAMERARX_CORE__REGISTER0__THS_TERM__POS       8

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER0__THS_SETTLE   
 *
 * @BRIEF        THS-Settle timing parameter in multiples of DDR clock 
 *               frequency - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER0__THS_SETTLE          BITFIELD(7, 0)
#define CAMERARX_CORE__REGISTER0__THS_SETTLE__POS     0

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER1__RESET_DONE_STATUS   
 *
 * @BRIEF        Reset done read bits. 
 *               28:RESETDONERXBYTECLK;29:RESETDONECTRLCLK - (RO) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER1__RESET_DONE_STATUS   BITFIELD(29, 28)
#define CAMERARX_CORE__REGISTER1__RESET_DONE_STATUS__POS 28

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER1__RESERVED   
 *
 * @BRIEF        Write 0 for future compatibility - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER1__RESERVED            BITFIELD(27, 26)
#define CAMERARX_CORE__REGISTER1__RESERVED__POS       26

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER1__CLOCK_MISS_DETECTOR_STATUS   
 *
 * @BRIEF        1:Error in clock missing detector. 0:Clock missing detector 
 *               successful - (RO) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER1__CLOCK_MISS_DETECTOR_STATUS BITFIELD(25, 25)
#define CAMERARX_CORE__REGISTER1__CLOCK_MISS_DETECTOR_STATUS__POS 25

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER1__TCLK_TERM   
 *
 * @BRIEF        TCLK_TERM timing parameter in multiples of CTRLCLK - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER1__TCLK_TERM           BITFIELD(24, 18)
#define CAMERARX_CORE__REGISTER1__TCLK_TERM__POS      18

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER1__DPHY_HS_SYNC_PATTERN   
 *
 * @BRIEF        DPHY mode HS sync pattern in byte order(reverse of received 
 *               order) - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER1__DPHY_HS_SYNC_PATTERN BITFIELD(17, 10)
#define CAMERARX_CORE__REGISTER1__DPHY_HS_SYNC_PATTERN__POS 10

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER1__CTRLCLK_DIV_FACTOR   
 *
 * @BRIEF        Divide factor for CTRLCLK for CLKMISS detector - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER1__CTRLCLK_DIV_FACTOR  BITFIELD(9, 8)
#define CAMERARX_CORE__REGISTER1__CTRLCLK_DIV_FACTOR__POS 8

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER1__TCLK_SETTLE   
 *
 * @BRIEF        TClk_Settle timing parameter in multiples of DDR Clock - 
 *               (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER1__TCLK_SETTLE         BITFIELD(7, 0)
#define CAMERARX_CORE__REGISTER1__TCLK_SETTLE__POS    0

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC0   
 *
 * @BRIEF        Mapping of Trigger escape entry command to PPI output 
 *               RXTRIGGERESC0 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC0 BITFIELD(31, 30)
#define CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC0__POS 30

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC1   
 *
 * @BRIEF        Mapping of Trigger escape entry command to PPI output 
 *               RXTRIGGERESC1 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC1 BITFIELD(29, 28)
#define CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC1__POS 28

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC2   
 *
 * @BRIEF        Mapping of Trigger escape entry command to PPI output 
 *               RXTRIGGERESC2 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC2 BITFIELD(27, 26)
#define CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC2__POS 26

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC3   
 *
 * @BRIEF        Mapping of Trigger escape entry command to PPI output 
 *               RXTRIGGERESC3 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC3 BITFIELD(25, 24)
#define CAMERARX_CORE__REGISTER2__TRIGGER_CMD_RXTRIGESC3__POS 24

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER2__CCP2_SYNC_PATTERN   
 *
 * @BRIEF        CCP2 mode sync pattern in byte order - (RO) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER2__CCP2_SYNC_PATTERN   BITFIELD(23, 0)
#define CAMERARX_CORE__REGISTER2__CCP2_SYNC_PATTERN__POS 0

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__OVRD_HSRXEN   
 *
 * @BRIEF        1:Override.0:Default - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__OVRD_HSRXEN         BITFIELD(31, 31)
#define CAMERARX_CORE__REGISTER3__OVRD_HSRXEN__POS    31

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__ENHSRX   
 *
 * @BRIEF        HSRX Enable on LANE5-0. 1:Enable. 0:Disable - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__ENHSRX              BITFIELD(30, 26)
#define CAMERARX_CORE__REGISTER3__ENHSRX__POS         26

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__OVRD_HSRXTERM   
 *
 * @BRIEF        Override with register bit. 1:Override. 0:Default - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__OVRD_HSRXTERM       BITFIELD(25, 25)
#define CAMERARX_CORE__REGISTER3__OVRD_HSRXTERM__POS  25

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__ENRXTERM   
 *
 * @BRIEF        HS-RX Termination enable on LANE5-0. 1:Enable. 0:Default - 
 *               (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__ENRXTERM            BITFIELD(24, 20)
#define CAMERARX_CORE__REGISTER3__ENRXTERM__POS       20

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__OVRD_LPRXEN_ULPRXEN   
 *
 * @BRIEF        Override LP-RX and ULP-RX Enable. 1:Override. 0:Default - 
 *               (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__OVRD_LPRXEN_ULPRXEN BITFIELD(19, 19)
#define CAMERARX_CORE__REGISTER3__OVRD_LPRXEN_ULPRXEN__POS 19

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__ENLPRX   
 *
 * @BRIEF        Enable for LP-RX on LANE5-0 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__ENLPRX              BITFIELD(18, 14)
#define CAMERARX_CORE__REGISTER3__ENLPRX__POS         14

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__ENULPRX   
 *
 * @BRIEF        Enable for ULP-RX on LANE5-0 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__ENULPRX             BITFIELD(13, 9)
#define CAMERARX_CORE__REGISTER3__ENULPRX__POS        9

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__LDO_EN_OVRD   
 *
 * @BRIEF        LDO Enable Override. 1:Override. 0:Default - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__LDO_EN_OVRD         BITFIELD(8, 8)
#define CAMERARX_CORE__REGISTER3__LDO_EN_OVRD__POS    8

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__EN_LDO   
 *
 * @BRIEF        Enable LDO. 1:Enable.0:Default - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__EN_LDO              BITFIELD(7, 7)
#define CAMERARX_CORE__REGISTER3__EN_LDO__POS         7

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__BIAS_EN_OVRD   
 *
 * @BRIEF        BIAS Enable Override.1 :Override with register bit. 
 *               0:Default - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__BIAS_EN_OVRD        BITFIELD(6, 6)
#define CAMERARX_CORE__REGISTER3__BIAS_EN_OVRD__POS   6

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__ENABLE_BIAS   
 *
 * @BRIEF        Enable BIAS. 1:Enable; 0:Disable - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__ENABLE_BIAS         BITFIELD(5, 5)
#define CAMERARX_CORE__REGISTER3__ENABLE_BIAS__POS    5

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__OVERRIDE_ENCCP   
 *
 * @BRIEF        Override ENCCP to anatop. 1: Override with register bit; 
 *               0:Default - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__OVERRIDE_ENCCP      BITFIELD(4, 4)
#define CAMERARX_CORE__REGISTER3__OVERRIDE_ENCCP__POS 4

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__ENCCP_OVRRD_HSRX   
 *
 * @BRIEF        ENCCP override to HSRX. 1: Enable; 0: Disable - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__ENCCP_OVRRD_HSRX    BITFIELD(3, 3)
#define CAMERARX_CORE__REGISTER3__ENCCP_OVRRD_HSRX__POS 3

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__RECALIB_HSRX_COMP_OFFSET   
 *
 * @BRIEF        Recalibrate HS-RX Comparator offset - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__RECALIB_HSRX_COMP_OFFSET BITFIELD(1, 1)
#define CAMERARX_CORE__REGISTER3__RECALIB_HSRX_COMP_OFFSET__POS 1

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER3__RECALIB_BIAS_CURRENT   
 *
 * @BRIEF        Recalibrate biasgen - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER3__RECALIB_BIAS_CURRENT BITFIELD(0, 0)
#define CAMERARX_CORE__REGISTER3__RECALIB_BIAS_CURRENT__POS 0

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER4__TRIM_BIASGEN_CURRENT   
 *
 * @BRIEF        Trim biasgen current - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER4__TRIM_BIASGEN_CURRENT BITFIELD(31, 27)
#define CAMERARX_CORE__REGISTER4__TRIM_BIASGEN_CURRENT__POS 27

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE4   
 *
 * @BRIEF        Trim termination resistor of lane 4 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE4     BITFIELD(26, 22)
#define CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE4__POS 22

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE3   
 *
 * @BRIEF        Trim termination resistor of lane 3 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE3     BITFIELD(21, 17)
#define CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE3__POS 17

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE2   
 *
 * @BRIEF        Trim termination resistor of lane 2 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE2     BITFIELD(16, 12)
#define CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE2__POS 12

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE1   
 *
 * @BRIEF        Trim termination resistor of lane 1 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE1     BITFIELD(11, 7)
#define CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE1__POS 7

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE0   
 *
 * @BRIEF        Trim termination resistor of lane 0 - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE0     BITFIELD(6, 2)
#define CAMERARX_CORE__REGISTER4__TRIM_TERM_LANE0__POS 2

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER4__BYPASS_EFUSE_TERM_RES   
 *
 * @BRIEF        Bypass efuse bits for termination resistor. 1:Bypass EFUSE 
 *               bits 0:Use EFUSE bits - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER4__BYPASS_EFUSE_TERM_RES BITFIELD(1, 1)
#define CAMERARX_CORE__REGISTER4__BYPASS_EFUSE_TERM_RES__POS 1

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE4HS_RX   
 *
 * @BRIEF        Trim Offset of Lane4 HS-RX. Sign Magnitude Trim code. - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE4HS_RX BITFIELD(31, 26)
#define CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE4HS_RX__POS 26

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE3HS_RX   
 *
 * @BRIEF        Trim Offset of Lane3 HS-RX. Sign Magnitude Trim code. - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE3HS_RX BITFIELD(25, 20)
#define CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE3HS_RX__POS 20

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE2_HS_RX   
 *
 * @BRIEF        Trim Offset of Lane2 HS-RX. Sign Magnitude Trim code. - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE2_HS_RX BITFIELD(19, 14)
#define CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE2_HS_RX__POS 14

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE1_HS_RX   
 *
 * @BRIEF        Trim Offset of Lane1 HS-RX. Sign Magnitude Trim code. - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE1_HS_RX BITFIELD(13, 8)
#define CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE1_HS_RX__POS 8

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE0_HS_RX   
 *
 * @BRIEF        Trim Offset of Lane0 HS-RX. Sign Magnitude Trim code. - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE0_HS_RX BITFIELD(7, 2)
#define CAMERARX_CORE__REGISTER5__TRIMOFFSET_LANE0_HS_RX__POS 2

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER5__BYPASS_CALIBRATED_OFFSET   
 *
 * @BRIEF        1: Bypass the calibrated offset; 0:Donot bypass - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER5__BYPASS_CALIBRATED_OFFSET BITFIELD(1, 1)
#define CAMERARX_CORE__REGISTER5__BYPASS_CALIBRATED_OFFSET__POS 1

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER6__OVRD_AFE_INPUTS   
 *
 * @BRIEF        Override LANEENABLE and POLARITY AFE inputs. 0:Normal. 
 *               1:Override - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER6__OVRD_AFE_INPUTS     BITFIELD(20, 20)
#define CAMERARX_CORE__REGISTER6__OVRD_AFE_INPUTS__POS 20

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER6__AFE_LANE_SELECT   
 *
 * @BRIEF        8 bit LANESEL for AFE - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER6__AFE_LANE_SELECT     BITFIELD(19, 12)
#define CAMERARX_CORE__REGISTER6__AFE_LANE_SELECT__POS 12

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER6__SEL_AFE_LANE_POLARITY   
 *
 * @BRIEF        Select AFE lane polarity - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER6__SEL_AFE_LANE_POLARITY BITFIELD(11, 11)
#define CAMERARX_CORE__REGISTER6__SEL_AFE_LANE_POLARITY__POS 11

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER6__HSCOMPOUT_FAR   
 *
 * @BRIEF        Select FAR lane HSCOMP output - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER6__HSCOMPOUT_FAR       BITFIELD(10, 10)
#define CAMERARX_CORE__REGISTER6__HSCOMPOUT_FAR__POS  10

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER6__BYPASS_LDO_REF   
 *
 * @BRIEF        0:Normal. 1:Bypass reference with VDD - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER6__BYPASS_LDO_REF      BITFIELD(9, 9)
#define CAMERARX_CORE__REGISTER6__BYPASS_LDO_REF__POS 9

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER6__LDO_VLTG_DYA   
 *
 * @BRIEF        Observe LDO voltage on DXA pad. 0:Normal. 1:Observe LDO 
 *               voltage - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER6__LDO_VLTG_DYA        BITFIELD(8, 8)
#define CAMERARX_CORE__REGISTER6__LDO_VLTG_DYA__POS   8

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER6__BIAS_CRNT_DXA   
 *
 * @BRIEF        Observe bias current on DXA pad. 0:Normal. 1:Observe bias 
 *               current - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER6__BIAS_CRNT_DXA       BITFIELD(7, 7)
#define CAMERARX_CORE__REGISTER6__BIAS_CRNT_DXA__POS  7

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER6__OVRD_BIASGEN_CALIB   
 *
 * @BRIEF        1:Override the EFUSE bits with register value. 0:Default - 
 *               (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER6__OVRD_BIASGEN_CALIB  BITFIELD(5, 5)
#define CAMERARX_CORE__REGISTER6__OVRD_BIASGEN_CALIB__POS 5

                                                                             /*-------------------------------------------------------------------------*//**
 * @DEFINITION   CAMERARX_CORE__REGISTER6__BIAS_CALIB_OVRD_VAL   
 *
 * @BRIEF        Biasgen calibration code override value - (RW) 
 *
    *//*------------------------------------------------------------------------ */
#define CAMERARX_CORE__REGISTER6__BIAS_CALIB_OVRD_VAL BITFIELD(4, 0)
#define CAMERARX_CORE__REGISTER6__BIAS_CALIB_OVRD_VAL__POS 0

    /* 
     * List of register bitfields values for component CAMERARX_CORE
     *
     */

#ifdef __cplusplus
}
#endif
#endif                                                     /* __CAMERARX_CORE_CRED_H 
                                                            */
