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
/*
 *  ======== HeapStdAlign.xs ========
 *
 *
 */

var HeapStdAlign = null;
var Program = null;

/*
 *  ======== module$use ========
 */
function module$use()
{
    Program = xdc.module('xdc.cfg.Program');
    xdc.useModule('xdc.runtime.Memory');
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    HeapStdAlign = mod;

    /* Initialize module fields */
    HeapStdAlign.remainRTSSize = Program.heap;
}

/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj, params)
{
    /* Throw an error if the size is zero */
    if (params.size == 0) {
        Program.$logFatal("'size' param to HeapStdAlign.create() cannot be 0.",
            this);
    }

    /* Make sure the rts size (Program.heap) is large enough) */
    if (HeapStdAlign.remainRTSSize < params.size) {
        Program.$logFatal("The RTS heap (Program.heap = " + Program.heap +
            ") is used up.", this);
    }

    HeapStdAlign.remainRTSSize -= params.size;

    /* Fill in the instance internal fields */
    obj.remainSize         = params.size;
    obj.startSize          = params.size;
}

/*
 *  ======== instance$view$init ========
 */
function instance$view$init(view, obj)
{
    view.address = obj.$addr;
    view.label = obj.$label;

    view.remainSize = obj.remainSize;
    view.startSize = obj.startSize;
}

/*
 *  @(#) xdcruntime.heaps; 1, 0, 0,1; 2-15-2012 07:45:06; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

