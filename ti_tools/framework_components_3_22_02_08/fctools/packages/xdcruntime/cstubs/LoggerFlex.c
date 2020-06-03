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
 *  ======== LoggerFlex.c ========
 */
#include <xdc/std.h>

#include <xdc/runtime/Log.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/System.h>

#include <xdcruntime/cstubs/LoggerFlexSupport.h>

#include "package/internal/LoggerFlex.xdc.h"

/*
 *  ======== LoggerFlex_Module_startup ========
 */
Int LoggerFlex_Module_startup(Int state)
{
    if (LoggerFlex_Object_count() != 0) {
        LoggerFlexSupport_init();
        if (System_atexit(LoggerFlex_shutdown) == FALSE) {
            System_abort("System_atexit failed");
        }
    }

    return (Startup_DONE);
}

/*
 *  ======== LoggerFlex_shutdown ========
 */
Void LoggerFlex_shutdown(Int status)
{
    LoggerFlexSupport_exit();
}

/*
 *  ======== LoggerFlex_Instance_init ========
 */
Void LoggerFlex_Instance_init(LoggerFlex_Object* logger,
    const LoggerFlex_Params* params)
{
}

/*
 *  ======== LoggerFlex_disable ========
 */
Bool LoggerFlex_disable(LoggerFlex_Object *obj)
{
    return(LoggerFlexSupport_disable());
}

/*
 *  ======== LoggerFlex_enable ========
 */
Bool LoggerFlex_enable(LoggerFlex_Object *obj)
{
    return(LoggerFlexSupport_enable());
}

/*
 *  ======== write0 =========
 */
Void LoggerFlex_write0(LoggerFlex_Object *obj, Log_Event evt,
    Types_ModuleId mid)
{
    LoggerFlexSupport_write0(evt, mid);
}

/*
 *  ======== write1 =========
 */
Void LoggerFlex_write1(LoggerFlex_Object *obj, Log_Event evt,
    Types_ModuleId mid, IArg a1)
{
    LoggerFlexSupport_write1(evt, mid, a1);
}

/*
 *  ======== write2 =========
 */
Void LoggerFlex_write2(LoggerFlex_Object *obj, Log_Event evt,
    Types_ModuleId mid, IArg a1, IArg a2)
{
    LoggerFlexSupport_write2(evt, mid, a1, a2);
}
/*
 *  ======== write4 =========
 */
Void LoggerFlex_write4(LoggerFlex_Object *obj, Log_Event evt,
    Types_ModuleId mid, IArg a1, IArg a2, IArg a3, IArg a4)
{
    LoggerFlexSupport_write4(evt, mid, a1, a2, a3, a4);
}

/*
 *  ======== write8 =========
 */
Void LoggerFlex_write8(LoggerFlex_Object *obj, Log_Event evt,
    Types_ModuleId mid, IArg a1, IArg a2, IArg a3, IArg a4, IArg a5,
    IArg a6, IArg a7, IArg a8)
{
    LoggerFlexSupport_write8(evt, mid, a1, a2, a3, a4, a5, a6, a7, a8);
}

/*
 *  @(#) xdcruntime.cstubs; 1, 0, 0,1; 2-15-2012 07:45:05; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

