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
 *  ======== Clobber.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Log.h>

#include "package/internal/Hwi.xdc.h"
#include "Clobber.h"

static volatile UInt intComplete;
static volatile UInt clobberIntNum;

Void ti_sysbios_family_arm_dm6446_Clobber_timerFunc__F(UArg arg)
{
    Hwi_Object *hwi;
    hwi = Hwi_module->dispatchTable[clobberIntNum];
    (hwi->fxn)(hwi->arg);
    intComplete = TRUE;
}

Void ti_sysbios_family_arm_dm6446_Clobber_postIntr__F(UInt intNum)
{
    intComplete = FALSE;

    clobberIntNum = intNum;

    while (intComplete == FALSE) {
        ;
    }
}

/*
 */
/*
 *  @(#) ti.sysbios.family.arm.dm6446; 2, 0, 0, 0,295; 5-18-2012 06:04:09; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

