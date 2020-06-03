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
var device;

if (xdc.om.$name == "cfg") {
    var deviceTable = {
        "Vayu": {
            vectorTableAddress : 0x80000000,
        },
        "Arctic": {
            vectorTableAddress : 0x80000000,
        },
        "TMS320C6A8149": {
            vectorTableAddress : 0x90000000,
        },
    }
}

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

    /* set fxntab default */
    Hwi.common$.fxntab = false;
    
    device = deviceTable[Program.cpu.deviceName];

    if (device == undefined) {
        throw new Error ("Timers unsupported on device: " +
            Program.cpu.deviceName);
    }

    /*
     * Initialize meta-only Hwi object array
     */

    /* initialize reset vector and other special interrupts */
    var name = "ti_sysbios_family_arp32_Hwi0";
    Hwi.nonDispatchedInterrupts[name] = new Hwi.NonDispatchedInterrupt();
    Hwi.nonDispatchedInterrupts[name].fxn = '&_c_int00';
    Hwi.nonDispatchedInterrupts[name].enableInt = true;
    Hwi.nonDispatchedInterrupts[name].vectorNum = 0;

    var name = "ti_sysbios_family_arp32_Hwi1";
    Hwi.nonDispatchedInterrupts[name] = new Hwi.NonDispatchedInterrupt();
    Hwi.nonDispatchedInterrupts[name].fxn = Hwi.nmiInterrupt;
    Hwi.nonDispatchedInterrupts[name].enableInt = true;
    Hwi.nonDispatchedInterrupts[name].vectorNum = 1;

    var name = "ti_sysbios_family_arp32_Hwi2";
    Hwi.nonDispatchedInterrupts[name] = new Hwi.NonDispatchedInterrupt();
    Hwi.nonDispatchedInterrupts[name].fxn = Hwi.swiInterrupt;
    Hwi.nonDispatchedInterrupts[name].enableInt = false;
    Hwi.nonDispatchedInterrupts[name].vectorNum = 2;

    var name = "ti_sysbios_family_arp32_Hwi3";
    Hwi.nonDispatchedInterrupts[name] = new Hwi.NonDispatchedInterrupt();
    Hwi.nonDispatchedInterrupts[name].fxn = Hwi.undefInterrupt;
    Hwi.nonDispatchedInterrupts[name].enableInt = false;
    Hwi.nonDispatchedInterrupts[name].vectorNum = 3;

    /* all others use vector to Hwi_unPluggedInterrupt by default */
    for (var vectorNum = 4; vectorNum < 8; vectorNum++) {
        var name = "&ti_sysbios_family_arp32_Hwi" + vectorNum;
        Hwi.vectors[vectorNum].used = false;
        Hwi.vectors[vectorNum].useDispatcher = true;
        Hwi.vectors[vectorNum].fxn = Hwi.selfLoop;
        Hwi.vectors[vectorNum].pfxn = name;
        Hwi.vectors[vectorNum].name = "";
    }
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    xdc.useModule('xdc.runtime.Log');

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
            Hwi.$logError("Dispatcher Swi support can't be enabled if ti.sysbios.BIOS.swiEnabled is false.", Hwi, "dispatcherSwiSupport");
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
            Hwi.$logError ("Dispatcher Task support can't be enabled if ti.sysbios.BIOS.taskEnabled is false.", Hwi, "dispatcherTaskSupport");
        }
    }
    else {
        Hwi.taskDisable = null;
        Hwi.taskRestoreHwi = null;
    }

    /* place .vecs section into platform's codeMemory */
    if (Program.sectMap[".vecs"] === undefined) {
        Program.sectMap[".vecs"] = new Program.SectionSpec();

        var found = false;
        /* need to loop through platform memory for EVEVECS */
        for (memmap in Program.cpu.memoryMap) {
            if (memmap.match("EVEVECS")) {
                found = true;
                Program.sectMap[".vecs"].loadSegment = memmap;
            }
        }

        /* if EVEVECS not found, then place in default addr specified above */
        if (!found) {
            Program.sectMap[".vecs"].loadAddress = device.vectorTableAddress;
        }
    }
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    mod.interruptMask = 0;
    mod.disableIerMask = 0;
    mod.restoreIerMask = 0;
    mod.vectNum = 0;
    mod.taskSP = null;

    /* 
     * ROM 
     * These members of the module state are added for ROM. They are tied to
     * their respective symbol name because the symbols will not be defined
     * at the time the ROM assembly is made.
     */
    mod.isrStack = null;
    mod.bss = $externPtr('__TI_STATIC_BASE');
    
    for (var i = 0; i < Hwi.NUM_VECTORS; i++) {
        mod.irp[i] = null;
    }
    
    for (var intNum = 0; intNum < Hwi.NUM_INTERRUPTS; intNum++) {
        mod.dispatchTable[intNum] = null;
    }

    /* 
     * parse the nonDispatchedInterrupts array and populate 
     * the Hwi.vectors array accordingly
     */
    for (var i in Hwi.nonDispatchedInterrupts) {
        var thisInt = Hwi.nonDispatchedInterrupts[i];
        var vectorNum = thisInt.vectorNum;

        if (Hwi.inUseMeta(vectorNum)) {
            Hwi.$logError("Hwi vector " + vectorNum + " already in use (by " +
                Hwi.vectors[vectorNum].pfxn + ").", i);
        }

        Hwi.vectors[vectorNum].used = true;
        Hwi.vectors[vectorNum].useDispatcher = false;
        Hwi.vectors[vectorNum].pfxn = thisInt.fxn;
        Hwi.vectors[vectorNum].name = i;

        if (thisInt.enableInt) {
            Hwi.initialIerMask |= 1 << vectorNum;
        }
    }
}

/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj, intNum, fxn, params)
{
    var mod = this.$module.$object;

    if (Hwi.interrupt[intNum].used == true) {
        Hwi.$logError("Hwi " + intNum + " already in use (by " +
                Hwi.interrupt[intNum].fxn + ").", this);
    }

    Hwi.interrupt[intNum].used = true;
    Hwi.interrupt[intNum].useDispatcher = true;
    Hwi.interrupt[intNum].fxn = fxn;

    mod.dispatchTable[intNum] = this;
    obj.arg = params.arg;
    obj.fxn = fxn;
    obj.irp = null;
    obj.vectorNum = params.vectorNum;
    obj.index = obj.vectorNum - 3;

    if ((Hwi.dispatcherAutoNestingSupport == false) && 
        (params.maskSetting != Hwi.MaskingOption_SELF)) {
        Hwi.$logWarning("Non default mask setting but " +
                        "dispatcherAutoNestingSupport is disabled.",
                        this, "maskSetting");
    }

    switch (params.maskSetting) {
        case Hwi.MaskingOption_NONE:
            obj.disableIerMask = 0;
            obj.restoreIerMask = 0;
            break;
        case Hwi.MaskingOption_ALL:
            obj.disableIerMask = 0xf0;
            obj.restoreIerMask = 0xf0;
            break;
        case Hwi.MaskingOption_LOWER:
            Hwi.$logWarning("Hwi.MaskingOption_LOWER not supported," + 
                            " converting to Hwi.MaskingOption_SELF.",
                            this, "maskSetting");
            params.maskSetting = Hwi.MaskingOption_SELF;
        case Hwi.MaskingOption_SELF:
            obj.disableIerMask = 1 << params.vectorNum;
            obj.restoreIerMask = 1 << params.vectorNum;
            break;
        case Hwi.MaskingOption_BITMASK:
            obj.disableIerMask = params.disableIerMask;
            obj.restoreIerMask = params.restoreIerMask;
            break;
    }

    if (params.enableInt) {
        mod.interruptMask |= 1 << intNum;
    }

    obj.hookEnv.length = Hwi.hooks.length;
}

/*
 *  ======== plugMeta ========
 */
function plugMeta(vectorNum, fxn)
{
    var name = "ti_sysbios_family_arp32_Hwi" + vectorNum;
    Hwi.nonDispatchedInterrupts[name] = new Hwi.NonDispatchedInterrupt();
    Hwi.nonDispatchedInterrupts[name].fxn = fxn;
    Hwi.nonDispatchedInterrupts[name].enableInt = false;
    Hwi.nonDispatchedInterrupts[name].vectorNum = vectorNum;
}

/*
 *  ======== inUseMeta ========
 */
function inUseMeta(vectorNum)
{
    return Hwi.vectors[vectorNum].used;
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
 *  ======== viewInitBasic ========
 *  Initialize the 'Basic' Task instance view.
 */
function viewInitBasic(view, obj)
{    
    var Hwi = xdc.useModule('ti.sysbios.family.arp32.Hwi');
    var Program = xdc.useModule('xdc.rov.Program');
    var halHwi = xdc.useModule('ti.sysbios.hal.Hwi');

    view.halHwiHandle =  halHwi.viewGetHandle(obj.$addr);
    view.label = Program.getShortName(obj.$label);
    var fxn = Program.lookupFuncName(Number(obj.fxn));
    view.fxn = fxn[0];
    view.arg = obj.arg;
    view.irp = 0;
    view.vector = obj.vectNum;
    view.disableIerMask = "0x" + obj.disableIerMask.toString(16);
    view.restoreIerMask = "0x" + obj.restoreIerMask.toString(16);

    try {
        var hwiRawView = Program.scanRawView('ti.sysbios.family.arp32.Hwi');
        var dispatchTable = hwiRawView.modState.dispatchTable;
        for (var i = 0; i < Hwi.NUM_INTERRUPTS; i++) {
            if (dispatchTable[i] == view.address) {
                view.intNum = i;
                break;
            }
        }
    }
    catch (e) {
        view.$status["intNum"] = "Unable to obtain intNum: " + e;
    }
}

/*!
 *  ======== viewGetStackInfo ========
 */
function viewGetStackInfo()
{
    var IHwi = xdc.useModule('ti.sysbios.interfaces.IHwi');

    var stackInfo = new IHwi.StackInfo();

    /* Fetch the Hwi stack */
    try {
        var size = Program.getSymbolValue("__TI_STACK_SIZE");
        var stackBase = Program.getSymbolValue("__stack");
        var stackData = Program.fetchArray({type: 'xdc.rov.support.ScalarStructs.S_UChar', isScalar: true}, stackBase, size);
    }
    catch (e) {
            stackInfo.hwiStackSize = 0; /* signal error to caller */
        return (stackInfo);
    }

    var index = 0;

    /* 
     * The stack is filled with 0xbe.
     */
    while (stackData[index] == 0xbe) {
        index++;
    }

    stackInfo.hwiStackPeak = size - index;
    stackInfo.hwiStackSize = size;
    stackInfo.hwiStackBase = stackBase;

    return (stackInfo);
}

/*
 *  ======== viewInitModule ========
 *  Initialize the Task module view.
 */
function viewInitModule(view, mod)
{
    var Program = xdc.useModule('xdc.rov.Program');

    var hwiModCfg = Program.getModuleConfig('ti.sysbios.family.arp32.Hwi');

    view.options[0] = "Hwi.autoNestingSupport = ";
    view.options[1] = "Hwi.swiSupport = ";
    view.options[2] = "Hwi.taskSupport = ";
    view.options[3] = "Hwi.irpSupport = ";

    view.options[0] += hwiModCfg.dispatcherAutoNestingSupport ? "true" : "false";
    view.options[1] += hwiModCfg.dispatcherSwiSupport ? "true" : "false";
    view.options[2] += hwiModCfg.dispatcherTaskSupport ? "true" : "false";
    view.options[3] += hwiModCfg.dispatcherIrpTrackingSupport ? "true" : "false";

    var stackInfo = viewGetStackInfo();

    if (stackInfo.hwiStackSize == 0) {
        view.$status["hwiStackPeak"] =
        view.$status["hwiStackSize"] =
        view.$status["hwiStackBase"] = "Error fetching Hwi stack info!"; 
    }
    else {
        view.hwiStackPeak = stackInfo.hwiStackPeak;
        view.hwiStackSize = stackInfo.hwiStackSize;
        view.hwiStackBase = "0x"+ stackInfo.hwiStackBase.toString(16);

        if (view.hwiStackPeak == view.hwiStackSize) {
            view.$status["hwiStackPeak"] = "Overrun!  "; 
            /*                                  ^^  */
            /* (extra spaces to overcome right justify) */
        }
    }
}

/*
 *  @(#) ti.sysbios.family.arp32; 2, 0, 0, 0,79; 5-18-2012 06:04:37; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

