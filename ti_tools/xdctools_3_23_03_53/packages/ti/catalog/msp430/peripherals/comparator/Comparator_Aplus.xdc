/* --COPYRIGHT--,EPL
 *  Copyright (c) 2008 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 * 
 *  Contributors:
 *      Texas Instruments - initial implementation
 * 
 * --/COPYRIGHT--*/
/*!
 *  ======== Comparator_Aplus ========
 *  MSP430F2xx Family Comparator_Aplus Module
 */
metaonly module Comparator_Aplus inherits IComparator {
  
    /*! CACTL1 Register */
    struct CACTL1_t {
        CAEX_t      CAEX;               /*! Comparator_A+ exchange. This bit exchanges the comparator inputs and
                                         *inverts the comparator output. */
        CARSEL_t    CARSEL;             /*! Comparator_A+ reference select. This bit selects which terminal the
                                         * VCAREF is applied to.
                                         * When CAEX = 0:
                                         * 0     VCAREF is applied to the + terminal
                                         * 1     VCAREF is applied to the � terminal
                                         * When CAEX = 1:
                                         * 0     VCAREF is applied to the � terminal
                                         * 1     VCAREF is applied to the + terminal */
                                         
        CAREF_t     CAREF;              /*! Comparator_A+ reference. These bits select the reference voltage VCAREF.
                                         * 00 Internal reference off. An external reference can be applied.
                                         * 01 0.25*VCC
                                         * 10 0.50*VCC
                                         * 11 Diode reference is selected */
                                         
        CAON_t      CAON;               /*! Comparator_A+ on. This bit turns on the comparator. When the
                                         * comparator is off it consumes no current. The reference circuitry is enabled
                                         * or disabled independently.
                                         * 0     Off
                                         * 1     On */
                                         
        CAIES_t     CAIES;              /*! Comparator_A interrupt edge select
                                         * 0  Rising edge
                                         * 1  Falling edge */
                                         
        CAIE_t      CAIE;               /*! Comparator_A+ interrupt enable
                                         * 0 Disabled
                                         * 1 Enabled */
                                         
        CAIFG_t     CAIFG;              /*! The Comparator_A+ interrupt flag
                                         * 0 No interrupt pending
                                         * 1 Interrupt pending */ 
    }
    
    /*! CACTL1 Register */
    struct CACTL2_t {
        CASHORT_t   CASHORT;            /*! Input short. This bit shorts the + and - input terminals.
                                         * 0 Inputs not shorted.
                                         * 1 Inputs shorted. */    
        P2CA4_t     P2CA4;              /*! Input select. This bit together with P2CA0 selects the + terminal input when
                                         * CAEX = 0 and the - terminal input when CAEX = 1 */
    
        P2CA3_t     P2CA3;              /*! Input select. These bits select the - terminal input when CAEX = 0 and the
                                         * + terminal input when CAEX = 1.
                                         * 000 No connection
                                         * 001 CA1
                                         * 010 CA2
                                         * 011 CA3
                                         * 100 CA4
                                         * 101 CA5
                                         * 110 CA6
                                         * 111 CA7 */
        P2CA2_t     P2CA2;              /*! Input select. These bits select the - terminal input when CAEX = 0 and the
                                         * + terminal input when CAEX = 1.
                                         * 000 No connection
                                         * 001 CA1
                                         * 010 CA2
                                         * 011 CA3
                                         * 100 CA4
                                         * 101 CA5
                                         * 110 CA6
                                         * 111 CA7 */
       P2CA1_t     P2CA1;              /*! Input select. These bits select the - terminal input when CAEX = 0 and the
                                         * + terminal input when CAEX = 1.
                                         * 000 No connection
                                         * 001 CA1
                                         * 010 CA2
                                         * 011 CA3
                                         * 100 CA4
                                         * 101 CA5
                                         * 110 CA6
                                         * 111 CA7 */
        P2CA0_t     P2CA0;              /*! Input select. This bit, together with P2CA4, selects the + terminal input
                                         * when CAEX = 0 and the - terminal input when CAEX = 1.
                                         * 00 No connection
                                         * 01 CA0
                                         * 10 CA1
                                         * 11 CA2 */
        CAF_t       CAF;                /*! Comparator_A+ output filter
                                         * 0 Comparator_A+ output is not filtered
                                         * 1 Comparator_A+ output is filtered */
    }
        
    /*! CAPD, Register */
    struct CAPD_t {
        CAPD7_t     CAPD7;              /*! CAPD7 Comparator_A+ port disable 
                                         * 0 The input buffer is enabled.
                                         * 1 The input buffer is disabled.*/

        CAPD6_t     CAPD6;              /*! CAPD6 Comparator_A+ port disable 
                                         *  0 The input buffer is enabled.
                                         *  1 The input buffer is disabled.*/
        CAPD5_t     CAPD5;              /*! CAPD5 Comparator_A+ port disable 
                                         *  0 The input buffer is enabled.
                                         *  1 The input buffer is disabled.*/
        CAPD4_t     CAPD4;              /*! CAPD4 Comparator_A+ port disable 
                                         *  0 The input buffer is enabled.
                                         *  1 The input buffer is disabled.*/
        CAPD3_t     CAPD3;              /*! CAPD3 Comparator_A+ port disable 
                                         *  0 The input buffer is enabled.
                                         *  1 The input buffer is disabled.*/
        CAPD2_t     CAPD2;              /*! CAPD2 Comparator_A+ port disable 
                                         *  0 The input buffer is enabled.
                                         *  1 The input buffer is disabled.*/
        CAPD1_t     CAPD1;              /*! CAPD1 Comparator_A+ port disable 
                                         *  0 The input buffer is enabled.
                                         *  1 The input buffer is disabled.*/
        CAPD0_t     CAPD0;              /*! CAPD0 Comparator_A+ port disable 
                                         *  0 The input buffer is enabled.
                                         *  1 The input buffer is disabled.*/
    }
      
    /*!
     *  ======== create ========
     *  Create an instance of this peripheral. 
     */
    create();
    
  instance:
   /*! CACTL1 Register */
    config CACTL1_t CACTL1 = {
        CAEX    :    CAEX_OFF,                
        CARSEL  :    CARSEL_OFF,                
        CAREF   :    CAREF_0,                
        CAON    :    CAON_OFF,                
        CAIES   :    CAIES_OFF,                
        CAIE    :    CAIE_OFF,                
        CAIFG   :    CAIFG_OFF                
     };   
     
    /*! CACTL2 Register */
    config CACTL2_t CACTL2 = {
        CASHORT :    CASHORT_OFF,            
        P2CA4   :     P2CA4_OFF,                
        P2CA3   :     P2CA3_OFF,   
        P2CA2   :     P2CA2_OFF,   
        P2CA1   :     P2CA1_OFF,             
        P2CA0   :    P2CA0_OFF,                
        CAF     :    CAF_OFF                
    };
    
    /*! CAPD, Register */
    config CAPD_t CAPD = {
        CAPD7   :    CAPD7_OFF,            
        CAPD6   :    CAPD6_OFF,                
        CAPD5   :    CAPD5_OFF,                
        CAPD4   :    CAPD4_OFF,                
        CAPD3   :    CAPD3_OFF,                
        CAPD2   :    CAPD2_OFF,                
        CAPD1   :    CAPD1_OFF,                
        CAPD0   :    CAPD0_OFF                
    };
    
     /*! Determine if each Register needs to be forced set or not */
    readonly config ForceSetDefaultRegister_t forceSetDefaultRegister[] =
    [
        { register : "CACTL1" , regForceSet : false },
        { register : "CACTL2" , regForceSet : false },
        { register : "CAPD" , regForceSet : false }
    ];
    
}
