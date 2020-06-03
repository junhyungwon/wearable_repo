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
 *  ======== USCI_B1_SPI_2xx.xs ========
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