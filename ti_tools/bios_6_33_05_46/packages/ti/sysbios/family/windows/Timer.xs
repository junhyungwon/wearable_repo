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
 *  ======== Timer.xs ========
 *
 */

var Timer = null;
var Hwi = null;

var intNumDef = [14, 15];

/*
 *  ======== module$use ========
 */
function module$use()
{
    Timer = this;
    Hwi = xdc.useModule("ti.sysbios.family.windows.Hwi");
}


/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    mod.availMask = 0x3; 
    mod.privateData = null;

    if (params.anyMask > mod.availMask) {
        Timer.$logError("Incorrect anyMask (" + params.anyMask
            + "). Should be <= " + mod.availMask + ".", Timer, "anyMask");
    }

    for (var i = 0; i < Timer.NUM_TIMER_DEVICES; i++) {
        mod.handles[i] = null;
    }
}


/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj, id, tickFxn, params)
{
    var modObj = this.$module.$object;

    if ((id < -1) || (Timer.NUM_TIMER_DEVICES <= id)) {
        Timer.$logFatal("Invalid Timer ID " + id + "!", this);
    }

    if (id == Timer.ANY) {
        for (var i = 0; i < Timer.NUM_TIMER_DEVICES; i++) {
            if ((Timer.anyMask & (1 << i)) && (modObj.availMask & (1 << i))) {
                modObj.availMask &= ~(1 << i);
                obj.id = i;
                break;
            }
        }
    }
    else if (modObj.availMask & (1 << id)) {
        modObj.availMask &= ~(1 << id);
        obj.id = id;
    }

    if (obj.id == undefined) {
        Timer.$logFatal("Timer device unavailable\n", this)
    }
    
    obj.runMode = params.runMode;
    obj.startMode = params.startMode;
    obj.period = params.period;
    obj.periodType = params.periodType;
    obj.extFreq.lo = params.extFreq.lo;
    obj.extFreq.hi = params.extFreq.hi;
    obj.intNum = intNumDef[obj.id];
    obj.tickFxn = tickFxn;
    obj.tickFxnArg = params.arg;
 
    if (obj.tickFxn) {
        var hwiParams = new Hwi.Params();
        hwiParams.arg = obj.tickFxnArg;
        obj.hwi = Hwi.create(obj.intNum, obj.tickFxn, hwiParams);
    }
    else {
        obj.hwi = null;
    }

    modObj.handles[obj.id] = this;
}
/*
 *  @(#) ti.sysbios.family.windows; 2, 0, 0, 0,553; 5-18-2012 06:05:20; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

