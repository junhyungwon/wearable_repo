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
 *  ======== LoggerFlexSupport.h ========
 *  Backend of LoggerFlex module
 *
 *  A module implementing LoggerFlexSupport allocates all memory required during
 *  LoggerFlexSupport_init() and frees all memory used during 
 *  LoggerFlexSupport_exit().
 *
 *  All the write functions get a Log_Event which contains information useful 
 *  for decoding and logging the event. 
 *  A Log_EventId can be obtained by calling Log_getEventId(evt). The 
 *  Log_EventId can be used by offline tools to get format strings previosuly 
 *  specified in a module xdc file. A Diags_mid can be obtained by calling
 *  Log_getmid(evt). A Diags_Level can be obtained by calling 
 *  Diags_getLevel(mask).
 *  The logger can use the Log_Event information to filter log messages and even
 *  route them to different output ports.
 
 */
#ifndef xdcruntime_cstubs_ILOGGERFLEX
#define xdcruntime_cstubs_ILOGGERFLEX

#include <xdc/std.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Types.h>

/* Init LoggerFlexSupport module */
Void LoggerFlexSupport_init(Void);

/* Exit LoggerFlexSupport module */
Void LoggerFlexSupport_exit(Void);

/* write a logger event which has no arguments */
Void LoggerFlexSupport_write0(Log_Event evt, Types_ModuleId mid);

/* write a logger event which has 1 argument */
Void LoggerFlexSupport_write1(Log_Event evt, Types_ModuleId mid, IArg a1);

/* write a logger event which has 2 arguments */
Void LoggerFlexSupport_write2(Log_Event evt, Types_ModuleId mid, IArg a1, 
    IArg a2);

/* write a logger event which has 4 arguments */
Void LoggerFlexSupport_write4(Log_Event evt, Types_ModuleId mid, IArg a1, 
    IArg a2, IArg a3, IArg a4);

/* write a logger event which has 8 arguments */
Void LoggerFlexSupport_write8(Log_Event evt, Types_ModuleId mid, IArg a1, 
    IArg a2, IArg a3, IArg a4, IArg a5, IArg a6, IArg a7, IArg a8);

/* Disable logging */
Bool LoggerFlexSupport_disable();

/* Enable logging */
Bool LoggerFlexSupport_enable();

#endif

/*
 *  @(#) xdcruntime.cstubs; 1, 0, 0,1; 2-15-2012 07:45:06; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

