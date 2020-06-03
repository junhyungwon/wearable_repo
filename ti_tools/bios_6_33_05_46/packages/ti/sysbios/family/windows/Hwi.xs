/* 
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * */
/*
 *  ======== Hwi.xs ========
 *
 */

var Hwi = null;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    Hwi = this;

    /*
     *  Initialize meta-only Hwi object array
     */

    /* initialize all others vectors to unused state */
    for (var intr = 1; intr < Hwi.NUM_INTERRUPTS; intr++) {
        Hwi.interrupt[intr].used = false;
        Hwi.interrupt[intr].useDispatcher = false;
        Hwi.interrupt[intr].fxn = null;
        Hwi.interrupt[intr].pfxn = null;
        Hwi.interrupt[intr].name = "";
    }
}


/*
 *  ======== module$use ========
 */
function module$use()
{
    xdc.useModule("xdc.runtime.Log");

    var BIOS = xdc.useModule("ti.sysbios.BIOS");

    if (Hwi.dispatcherSwiSupport == undefined) {
        Hwi.dispatcherSwiSupport = BIOS.swiEnabled;
    }
    if (Hwi.dispatcherTaskSupport == undefined) {
        Hwi.dispatcherTaskSupport = BIOS.taskEnabled;
    }
    if (Hwi.dispatcherSwiSupport) {
        if (BIOS.swiEnabled) {
            xdc.useModule("ti.sysbios.knl.Swi");
            Hwi.swiDisable = '&ti_sysbios_knl_Swi_disable__E';
            Hwi.swiRestoreHwi = '&ti_sysbios_knl_Swi_restoreHwi__E';
        }
        else {
            Hwi.$logError("Dispatcher Swi support can't be enabled "
                            + "if ti.sysbios.BIOS.swiEnabled is "
                            + "false.\n", Hwi, "dispatcherSwiSupport");
        }
    }
    else {
        Hwi.swiDisable = null;
        Hwi.swiRestoreHwi = null;
    }

    if (Hwi.dispatcherTaskSupport) {
        if (BIOS.taskEnabled) {
            xdc.useModule("ti.sysbios.knl.Task");
            Hwi.taskDisable = '&ti_sysbios_knl_Task_disable__E';
            Hwi.taskRestoreHwi = '&ti_sysbios_knl_Task_restoreHwi__E';
        }
        else {
            Hwi.$logError ("Dispatcher Task support can't be enabled "
                            + "if ti.sysbios.BIOS.taskEnabled is "
                            + "false.\n", Hwi, "dispatcherTaskSupport");
        }
    }
    else {
        Hwi.taskDisable = null;
        Hwi.taskRestoreHwi = null;
    }

    /* set USER3 bit in diags mask to enable warning messages */
    var Diags = xdc.useModule("xdc.runtime.Diags");
    Hwi.common$.diags_USER3 = Diags.ALWAYS_ON;

    /* sibling modules used by this module */
    xdc.useModule("ti.sysbios.family.windows.TaskSupport");
    xdc.useModule("ti.sysbios.family.windows.IntrinsicsSupport");
}


/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    mod.ierMask = 2;  // enabled NMI

    for (var intr = 0; intr < Hwi.NUM_INTERRUPTS; intr++) {
        mod.dispatchTable[intr] = null;
    }

    mod.GIE = false;
    mod.IER = 0;
    mod.IFR = 0;
    mod.intrNum = 0;
    mod.privateData = null;

    for (i = 0; i < this.NUM_INTERRUPTS; i++) {
        mod.IST[i] = null;
    }

    /* 
     * parse the nonDispatchedInterrupts array and populate 
     * the Hwi.interrupt array accordingly
     */
    for (var i in Hwi.nonDispatchedInterrupts) {
        var thisInt = Hwi.nonDispatchedInterrupts[i];
        var intNum = thisInt.intNum;

        if (Hwi.inUseMeta(intNum)) {
            Hwi.$logError("Hwi " + intNum + " already in use (by " +
                Hwi.interrupt[intNum].pfxn + ").", i);
        }

        Hwi.interrupt[intNum].used = true;
        Hwi.interrupt[intNum].useDispatcher = false;
        Hwi.interrupt[intNum].pfxn = thisInt.fxn;
        Hwi.interrupt[intNum].name = i;

        if (thisInt.enableInt) {
            mod.ierMask |= 1 << intNum;
        }

        if ((thisInt.eventId != -1) && (thisInt.eventId !== undefined)) {
            if (thisInt.eventId < 32) {
                eventMapMeta(thisInt.intNum, thisInt.eventId);
            }
            else {
                Hwi.$logError(
                    "eventId (" + thisInt.eventId + ") must be less than 32",
                    Hwi, thisInt);
            }
        }
    }
}

/*
 *  ======== inUseMeta ========
 */
function inUseMeta(intr)
{
    return Hwi.interrupt[intr].used;
}


/*
 *  ======== plugMeta ========
 */
function plugMeta(intNum, fxn)
{
    var name = "ti_sysbios_family_win_Hwi" + intNum;
    Hwi.nonDispatchedInterrupts[name] = new Hwi.NonDispatchedInterrupt();
    Hwi.nonDispatchedInterrupts[name].fxn = fxn;
    Hwi.nonDispatchedInterrupts[name].eventId = -1;
    Hwi.nonDispatchedInterrupts[name].enableInt = false;
    Hwi.nonDispatchedInterrupts[name].intNum = intNum;
}


/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj, intr, fxn, params)
{
    var mod = this.$module.$object;

    /* check for configuration errors */
    if (intr == 0) {
        Hwi.$logError(
            "Reset (intNum = 0) can't be assigned to the dispatcher", this);
    }

    if (intr == 1) {
        Hwi.$logError(
            "NMI (intNum = 1) can't be assigned to the dispatcher", this);
    }

    if (Hwi.interrupt[intr].used == true) {
        Hwi.$logError("Hwi " + intr + " already in use (by " +
                Hwi.interrupt[intr].fxn + ").", this);
    }

    if ((Hwi.dispatcherAutoNestingSupport == false) && 
        (params.maskSetting != Hwi.MaskingOption_SELF)) {
        Hwi.$logWarning("Non-default mask setting but"
            + " dispatcherAutoNestingSupport is disabled.", this);
    }

    /* initialize the instance object */
    obj.arg = params.arg;
    obj.fxn = fxn;
    obj.irp = null;

    obj.hookEnv.length = Hwi.hooks.length;

    switch (params.maskSetting) {
        case Hwi.MaskingOption_NONE:
            obj.disableMask = 0;
            obj.restoreMask = 0;
            break;
        case Hwi.MaskingOption_ALL:
            obj.disableMask = 0xffff;
            obj.restoreMask = 0xffff;
            break;
        case Hwi.MaskingOption_LOWER:
            Hwi.$logWarning("Hwi.MaskingOption_LOWER not supported," + 
                            " converting to Hwi.MaskingOption_SELF.",
                            this);
            params.maskSetting = Hwi.MaskingOption_SELF;
        case Hwi.MaskingOption_SELF:
            obj.disableMask = 1 << intr;
            obj.restoreMask = 1 << intr;
            break;
        case Hwi.MaskingOption_BITMASK:
            obj.disableMask = params.disableMask;
            obj.restoreMask = params.restoreMask;
            break;
    }

    /* plug the module's dispatch table with this object */
    mod.dispatchTable[intr] = this;

    /* enable interrupt in the IER register */
    if (params.enableInt) {
        mod.ierMask |= 1 << intr;
    }

    /* keep track in meta-world of plugged interrupts */
    Hwi.interrupt[intr].used = true;
    Hwi.interrupt[intr].useDispatcher = true;
    Hwi.interrupt[intr].fxn = fxn;
}

/*
 *  ======== addHookSet ========
 */
function addHookSet(hookSet)
{
    /* use "===" so 'null' is not flagged */
    if (hookSet.registerFxn === undefined) {
        hookSet.registerFxn = null;
    }
    if (hookSet.createFxn === undefined) {
        hookSet.createFxn = null;
    }
    if (hookSet.beginFxn === undefined) {
        hookSet.beginFxn = null;
    }
    if (hookSet.endFxn === undefined) {
        hookSet.endFxn = null;
    }
    if (hookSet.deleteFxn === undefined) {
        hookSet.deleteFxn = null;
    }

    this.hooks.$add(hookSet);
}



/*
 *  @(#) ti.sysbios.family.windows; 2, 0, 0, 0,553; 5-18-2012 06:05:19; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

