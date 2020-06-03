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
import ti.catalog.msp430.peripherals.clock.IClock;

/*!
 *  ======== ITimer ========
 *  MSP430 ITimer interface
 */
metaonly interface ITimer inherits xdc.platform.IPeripheral {

    /*!
    *  ======== ForceSetDefaultRegister_t ========
    *  Force Set Default Register
    *
    *  Type to store if each register needs to be forced initialized
    *  even if the register is in default state.
    *
    *  @see #ForceSetDefaultRegister_t
    */
    struct ForceSetDefaultRegister_t {
        String     register;
        Bool       regForceSet;
    }
    
    create(IClock.Instance clock);

instance:
    /*!
     *  ======== baseAddr ========
     *  Address of the peripheral's control register.
     *
     *  A peripheral's registers are commonly accessed through a structure
     *  that defines the offsets of a particular register from the lowest
     *  address mapped to a peripheral. That lowest address is specified by
     *  this parameter.
     */
    config UInt baseAddr;
      
    /*!
     *  ======== intNum ========
     *  Interrupt source number
     *
     */
    config UInt intNum;

    /*!
     *  ======== numberOfTimers ========
     *  Stores the number of available timer capture compare blocks
     *
     */
    config UChar numberOfTimers;

    /*!
    *  ======== setTxxCCRx ========
    *  Sets TxxCCRx register value based on which CCR register
    *
    *  @see #setTxxCCRx
    */
    void setTxxCCRx(UChar ccrNumber , Bits16 value);
    
    /*!
    *  ======== getTxxCCRx ========
    *  Returns TxxCCRx register value based on which CCR register
    *
    *  @see #getTxxCCRx
    */
    Bits16 getTxxCCRx(UChar ccrNumber);

    /*! @_nodoc */
    config IClock.Instance clock;
}
