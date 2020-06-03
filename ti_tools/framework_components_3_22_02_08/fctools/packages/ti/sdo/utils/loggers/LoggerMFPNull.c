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
 *  ======== LoggerMFPNull.c ========
 *  Dummy logger to support control() calls.
 */
#include <xdc/std.h>

#include <xdc/runtime/Log.h>
#include <xdc/runtime/Types.h>

#include "package/internal/LoggerMFPNull.xdc.h"


/*
 *  ======== Instance_init =========
 */
Void LoggerMFPNull_Instance_init(LoggerMFPNull_Object *obj,
    const LoggerMFPNull_Params *prms)
{
}

/*
 *  ======== LoggerMFPNull_control =========
 */
Void LoggerMFPNull_control(Int cmd, UArg cmdArgs)
{
}

/*
 *  ======== LoggerMFPNull_disable ========
 */
Bool LoggerMFPNull_disable(LoggerMFPNull_Object *obj)
{
    return (TRUE);
}

/*
 *  ======== LoggerMFPMULL_enable ========
 */
Bool LoggerMFPNull_enable(LoggerMFPNull_Object *obj)
{
    return (TRUE);
}

/*
 *  ======== write0 =========
 */
Void LoggerMFPNull_write0(LoggerMFPNull_Object *obj, Log_Event evt,
                      Types_ModuleId mid)
{
}

/*
 *  ======== write1 =========
 */
Void LoggerMFPNull_write1(LoggerMFPNull_Object *obj, Log_Event evt,
                      Types_ModuleId mid, IArg a1)
{
}

/*
 *  ======== write2 =========
 */
Void LoggerMFPNull_write2(LoggerMFPNull_Object *obj, Log_Event evt,
                      Types_ModuleId mid, IArg a1, IArg a2)
{
}

/*
 *  ======== write4 =========
 */
Void LoggerMFPNull_write4(LoggerMFPNull_Object *obj, Log_Event evt,
                      Types_ModuleId mid, IArg a1, IArg a2, IArg a3, IArg a4)
{
}

/*
 *  ======== write8 =========
 */
Void LoggerMFPNull_write8(LoggerMFPNull_Object *obj, Log_Event evt,
                      Types_ModuleId mid, IArg a1, IArg a2, IArg a3, IArg a4,
                      IArg a5, IArg a6, IArg a7, IArg a8)
{
}
/*
 *  @(#) ti.sdo.utils.loggers; 1, 0, 0,1; 2-15-2012 07:44:55; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

