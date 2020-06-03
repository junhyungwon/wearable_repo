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
 *  ======== IE2.xs ========
 */
var REGS;
var GetSet;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init() 
{
	REGS = this;
    GetSet = xdc.module("xdc.services.getset.GetSet");
}

/*
 *  ======== instance$meta$init ========
 */
function instance$meta$init() 
{
    // Activate getter/setter support on current instance
    GetSet.init(this);
    GetSet.init(this.IE2);
	
	/* save "this" for use inside setter */
    var inst = this;
	var group = GetSet.createGroup();
	
    function monitorUcb0TxIe() {
        if (inst.IE2.UCB0TXIE == REGS.UCB0TXIE) {
	        inst.IFG2.UCB0TXIFG = REGS.UCB0TXIFG;
	    }
	    else {
	        inst.IFG2.UCB0TXIFG = REGS.UCB0TXIFG_OFF;
	    }
    }
    group.onSet(inst.IE2, "UCB0TXIE", monitorUcb0TxIe);
    
    function monitorUcb0RxIe() {
        if (inst.IE2.UCB0RXIE == REGS.UCB0RXIE) {
	        inst.IFG2.UCB0RXIFG = REGS.UCB0RXIFG;
	    }
	    else {
	        inst.IFG2.UCB0RXIFG = REGS.UCB0RXIFG_OFF;
	    }
    }
    group.onSet(inst.IE2, "UCB0RXIE", monitorUcb0RxIe);
    
    function monitorUca0TxIe() {
        if (inst.IE2.UCA0TXIE == REGS.UCA0TXIE) {
	        inst.IFG2.UCA0TXIFG = REGS.UCA0TXIFG;
	    }
	    else {
	        inst.IFG2.UCA0TXIFG = REGS.UCA0TXIFG_OFF;
	    }
    }
    group.onSet(inst.IE2, "UCA0TXIE", monitorUca0TxIe);
    
    function monitorUca0RxIe() {
        if (inst.IE2.UCA0RXIE == REGS.UCA0RXIE) {
	        inst.IFG2.UCA0RXIFG = REGS.UCA0RXIFG;
	    }
	    else {
	        inst.IFG2.UCA0RXIFG = REGS.UCA0RXIFG_OFF;
	    }
    }
    group.onSet(inst.IE2, "UCA0RXIE", monitorUca0RxIe);
}

/*
 *  ======== getUCB0TXIE ========
 */
function getUCB0TXIE() 
{
    var REGS = this.$module;
    if (this.IE2.UCB0TXIE == REGS.UCB0TXIE) {
        return true;
    }
    else {
        return false;
    }
}

/*
 *  ======== setUCB0TXIE ========
 */
function setUCB0TXIE(set) 
{
    var REGS = this.$module;
    
    if (set) {
        this.IE2.UCB0TXIE = REGS.UCB0TXIE;
    }
    else {
        this.IE2.UCB0TXIE = REGS.UCB0TXIE_OFF;
    }
    
    return set;
}

/*
 *  ======== getUCB0RXIE ========
 */
function getUCB0RXIE() 
{
    var REGS = this.$module;
    if (this.IE2.UCB0RXIE == REGS.UCB0RXIE) {
        return true;
    }
    else {
        return false;
    }
}

/*
 *  ======== setUCB0RXIE ========
 */
function setUCB0RXIE(set) 
{
    var REGS = this.$module;
    
    if (set) {
        this.IE2.UCB0RXIE = REGS.UCB0RXIE;
    }
    else {
        this.IE2.UCB0RXIE = REGS.UCB0RXIE_OFF;
    }
    
    return set;
}

/*
 *  ======== getUCA0TXIE ========
 */
function getUCA0TXIE() 
{
    var REGS = this.$module;
    if (this.IE2.UCA0TXIE == REGS.UCA0TXIE) {
        return true;
    }
    else {
        return false;
    }
}

/*
 *  ======== setUCA0TXIE ========
 */
function setUCA0TXIE(set) 
{
    var REGS = this.$module;
    
    if (set) {
        this.IE2.UCA0TXIE = REGS.UCA0TXIE;
    }
    else {
        this.IE2.UCA0TXIE = REGS.UCA0TXIE_OFF;
    }
    
    return set;
}

/*
 *  ======== getUCA0RXIE ========
 */
function getUCA0RXIE() 
{
    var REGS = this.$module;
    if (this.IE2.UCA0RXIE == REGS.UCA0RXIE) {
        return true;
    }
    else {
        return false;
    }
}

/*
 *  ======== setUCB0TXIE ========
 */
function setUCA0RXIE(set) 
{
    var REGS = this.$module;
    
    if (set) {
        this.IE2.UCA0RXIE = REGS.UCA0RXIE;
    }
    else {
        this.IE2.UCA0RXIE = REGS.UCA0RXIE_OFF;
    }
    
    return set;
}
