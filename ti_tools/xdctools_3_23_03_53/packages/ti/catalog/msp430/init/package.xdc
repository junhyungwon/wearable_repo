requires xdc.runtime;

/*!
 *  ======== ti.catalog.msp430.init ========
 *  MSP430 boot timer initialization support
 *
 *  This package contain a `Boot` module that supplies minimal boot timer
 *  initialization code.  In the case of the MSP430, this code simply
 *  disables the watchdog timer (to allow normal C initialization to
 *  complete before the watchdog resets the device).
 */
package ti.catalog.msp430.init[1,0,0] {
    module Boot;
}
