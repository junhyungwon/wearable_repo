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
 *  ======== Core.xs ========
 *
 */

var Hwi = null;
var Core = null;

if (xdc.om.$name == "cfg") {
    var deviceTable = {
        "TMS320C6A8168": {
        },
    };
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

    Core = this;

    Core.id = 0;

    /* devices in deviceTable have logically only one core, core 1 */
    for (device in deviceTable) {
        if (device == Program.cpu.deviceName) {
            Core.id = 1;
            return;
        }
    }
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Hwi = xdc.module("ti.sysbios.family.arm.m3.Hwi");

    if ( (Core.id != 0) && (Core.id != 1) ) {
        Core.$logFatal("Invalid Core.id: " + Core.id + 
                       ", must be 0 or 1.", 
                       this);
    }

    /* 
     * by default, Core 0's vector table is placed at 0x400
     * and Core 1's vector table is placed at 0x800
     */

    if ((Hwi.resetVectorAddress == 0) && (Hwi.vectorTableAddress == 0)) {

        /* 
         * generate and place a single, core-specific const vector table 
         */

        Hwi.resetVectorAddress = (Core.id + 1) * 0x400;
        Hwi.vectorTableAddress = Hwi.resetVectorAddress;

        /* place common boot code at 0 */
        Program.sectMap[".ducatiBoot"] = new Program.SectionSpec();
        Program.sectMap[".ducatiBoot"].loadAddress = 0;
    }
}
/*
 *  @(#) ti.sysbios.family.arm.ducati; 2, 0, 0, 0,302; 5-18-2012 06:04:12; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

