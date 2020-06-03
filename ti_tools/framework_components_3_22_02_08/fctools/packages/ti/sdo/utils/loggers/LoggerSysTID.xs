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
 * 
 */

var Logger;

/*
 *  ======== module$use ========
 */
function module$use()
{
    var Timestamp = xdc.module('xdc.runtime.Timestamp');

    Logger = this;

    /*
     *  Bind LoggerSysTID.TimestampProxy to whatever Timestamp is using,
     *  unless LoggerSys.TimestampProxy was set already.
     */
    if (Logger.TimestampProxy == null) {
        Logger.TimestampProxy = Timestamp.SupportProxy.delegate$;
    }

    /* Disable trace on timestamp proxy to prevent recursive callbacks */
    var Diags = xdc.module("xdc.runtime.Diags");
    var modName = Logger.TimestampProxy.delegate$.$name;
    //print("Turning of logging for module: " + modName);
    Diags.setMaskMeta(modName, Diags.ALL & (~Diags.ASSERT), Diags.ALWAYS_OFF);

    /* This module uses Thread to get thread ID */
    xdc.useModule('xdc.runtime.knl.Thread');

    /* This module needs a GateThread */
    if (Logger.common$.gate === undefined) {
        //print("Logger.common$.gate undefined, creating a gate...");
        var Gate = xdc.useModule('xdc.runtime.knl.GateThread');
        Logger.common$.gate = Gate.create();
    }

    Logger.gate = Logger.common$.gate;

    var del = Gate.Proxy.delegate$;
    //print("GateProxy delegate = " + del);

    /* Disable trace on the Gate proxy to prevent recursive callbacks */
    //modName = Logger.gate.$name;
    //print("LoggerSysTID.gate.$name = " + modName);
    //modName = "xdc.runtime.knl.GateThread";
    //Diags.setMaskMeta(modName, Diags.ALL & (~Diags.ASSERT),Diags.ALWAYS_OFF);
    //Diags.setMaskMeta(del, Diags.ALL & (~Diags.ASSERT), Diags.ALWAYS_OFF);
}

/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj, params)
{
    obj.enabled = true;
}
/*
 *  @(#) ti.sdo.utils.loggers; 1, 0, 0,1; 2-15-2012 07:44:56; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

