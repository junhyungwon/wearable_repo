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

/*
 *  ======== USCI_B1_I2C_2xx.xs ========
 */

/*
 *  ======== module$meta$init ========
 */
function module$meta$init() 
{
}

/*
 *  ======== instance$meta$init ========
 */
function instance$meta$init(clock) 
{
    this.clock = clock;
}

/* Gets UCxxTXIE bit */
function getUCxxTXIE() {
    var REGS = this.$module;
    if (this.UC1IE.UCB1TXIE == REGS.UCB1TXIE) {
        return true;
    }
    else {
        return false;
    }
}

/* Sets UCxxTXIE bit */
function setUCxxTXIE(set) {
    var REGS = this.$module;
    if (set) {
        this.UC1IE.UCB1TXIE = REGS.UCB1TXIE;
    }
    else {
        this.UC1IE.UCB1TXIE = REGS.UCB1TXIE_OFF;
    }
    return set;
}

/* Gets UCxxRXIE bit */
function getUCxxRXIE() {
    var REGS = this.$module;
    if (this.UC1IE.UCB1RXIE == REGS.UCB1RXIE) {
        return true;
    }
    else {
        return false;
    }
}

/* Sets UCxxRXIE bit */
function setUCxxRXIE(set) {
    var REGS = this.$module;
    if (set) {
        this.UC1IE.UCB1RXIE = REGS.UCB1RXIE;
    }
    else {
        this.UC1IE.UCB1RXIE = REGS.UCB1RXIE_OFF;
    }
    return set;
}

/*
 *  ======== setUCNACKIE ========
 */
function setUCNACKIE(set)
{
    var REGS = this.$module;
    if (set) {
        this.USCI_B1_I2C_2xx.UCB1I2CIE.UCNACKIE = REGS.UCNACKIE;
    }
    else {
        this.USCI_B1_I2C_2xx.UCB1I2CIE.UCNACKIE = REGS.UCNACKIE_OFF;
    }
    return set;
}

/*
 *  ======== getUCNACKIE ========
 */
function getUCNACKIE()
{
    var REGS = this.$module;
    if (this.USCI_B1_I2C_2xx.UCB1I2CIE.UCNACKIE == REGS.UCNACKIE) {
        return true;
    }
    else {
        return false;
    }
}

/*
 *  ======== setUCSTPIE ========
 */
function setUCSTPIE(set)
{
    var REGS = this.$module;
    if (set) {
        this.USCI_B1_I2C_2xx.UCB1I2CIE.UCSTPIE = REGS.UCSTPIE;
    }
    else {
        this.USCI_B1_I2C_2xx.UCB1I2CIE.UCSTPIE = REGS.UCSTPIE_OFF;
    }
    return set;
}

/*
 *  ======== getUCSTPIE ========
 */
function getUCSTPIE()
{
    var REGS = this.$module;
    if (this.USCI_B1_I2C_2xx.UCB1I2CIE.UCSTPIE == REGS.UCSTPIE) {
        return true;
    }
    else {
        return false;
    }
}
    
/*
 *  ======== setUCSTTIE ========
 */
function setUCSTTIE(set)
{
    var REGS = this.$module;
    if (set) {
        this.USCI_B1_I2C_2xx.UCB1I2CIE.UCSTTIE = REGS.UCSTTIE;
    }
    else {
        this.USCI_B1_I2C_2xx.UCB1I2CIE.UCSTTIE = REGS.UCSTTIE_OFF;
    }
    return set;
}

/*
 *  ======== getUCSTTIE ========
 */
function getUCSTTIE()
{
    var REGS = this.$module;
    if (this.USCI_B1_I2C_2xx.UCB1I2CIE.UCSTTIE == REGS.UCSTTIE) {
        return true;
    }
    else {
        return false;
    }
}
    
/*
 *  ======== setUCALIE ========
 */
function setUCALIE(set)
{
    var REGS = this.$module;
    if (set) {
        this.USCI_B1_I2C_2xx.UCB1I2CIE.UCALIE = REGS.UCALIE;
    }
    else {
        this.USCI_B1_I2C_2xx.UCB1I2CIE.UCALIE = REGS.UCALIE_OFF;
    }
    return set;
}

/*
 *  ======== getUCALIE ========
 */
function getUCALIE()
{
    var REGS = this.$module;
    if (this.USCI_B1_I2C_2xx.UCB1I2CIE.UCALIE == REGS.UCALIE) {
        return true;
    }
    else {
        return false;
    }
}
