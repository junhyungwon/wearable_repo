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
 *  ======== LoggerPosix.c ========
 *  Just like LoggerPosix, but also prints thread ID.
 */
#include <xdc/std.h>

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include <xdcruntime/cstubs/LoggerFlexSupport.h>

#include <xdc/runtime/Log.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Text.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Gate.h>

#include <ti/sdo/xdcruntime/linux/TimestampPosix.h>


#if (xdc_target__bitsPerChar * xdc_target__sizeof_Int) == 32
#define SERIAL "#%010u "
#define HI "%0.8x:"
#define LO "%0.8x] "
#elif (xdc_target__bitsPerChar * xdc_target__sizeof_Long) == 32
#define SERIAL "#%010lu "
#define HI "%0.8lx:"
#define LO "%0.8lx] "
#else
#error LoggerFlexSupport does not support this target.
#endif

Bool LoggerPosix_enabled;
static Void doPrint(Log_EventRec *er);

/*
 *  ======== LoggerFlexSupport_init ========
 */
Void LoggerFlexSupport_init(Void)
{
    LoggerPosix_enabled = TRUE;
}

/*
 *  ======== LoggerFlexSupport_exit ========
 */
Void LoggerFlexSupport_exit(Void)
{
}

/*
 *  ======== LoggerFlexSupport_disable ========
 */
Bool LoggerFlexSupport_disable()
{
    Bool prev = LoggerPosix_enabled;
    LoggerPosix_enabled = FALSE;
    return (prev);
}

/*
 *  ======== LoggerFlexSupport_enable ========
 */
Bool LoggerFlexSupport_enable()
{
    Bool prev = LoggerPosix_enabled;
    LoggerPosix_enabled = TRUE;
    return (prev);
}

/*
 *  ======== write0 =========
 */
Void LoggerFlexSupport_write0( Log_Event evt,
                      Types_ModuleId mid)
{
    LoggerFlexSupport_write8(evt, mid, 0, 0, 0, 0, 0, 0, 0, 0);
}

/*
 *  ======== write1 =========
 */
Void LoggerFlexSupport_write1( Log_Event evt,
                      Types_ModuleId mid, IArg a1)
{
    LoggerFlexSupport_write8(evt, mid, a1, 0, 0, 0, 0, 0, 0, 0);
}

/*
 *  ======== write2 =========
 */
Void LoggerFlexSupport_write2( Log_Event evt,
                      Types_ModuleId mid, IArg a1, IArg a2)
{
    LoggerFlexSupport_write8(evt, mid, a1, a2, 0, 0, 0, 0, 0, 0);
}

/*
 *  ======== write4 =========
 */
Void LoggerFlexSupport_write4( Log_Event evt,
    Types_ModuleId mid, IArg a1, IArg a2, IArg a3, IArg a4)
{
    LoggerFlexSupport_write8(evt, mid, a1, a2, a3, a4, 0, 0, 0, 0);
}

/*
 *  ======== write8 =========
 */
Void LoggerFlexSupport_write8( Log_Event evt,
    Types_ModuleId mid, IArg a1, IArg a2, IArg a3, IArg a4, IArg a5,
    IArg a6, IArg a7, IArg a8)
{
    Log_EventRec evrec;
    TimestampPosix_get64(&evrec.tstamp);

    evrec.serial = 0;
    evrec.evt = Types_makeEvent(Log_getEventId(evt), mid);
    evrec.arg[0] = a1;
    evrec.arg[1] = a2;
    evrec.arg[2] = a3;
    evrec.arg[3] = a4;
    evrec.arg[4] = a5;
    evrec.arg[5] = a6;
    evrec.arg[6] = a7;
    evrec.arg[7] = a8;

    doPrint(&evrec);
}


/*
 *  ======== doPrint ========
 */
static Void doPrint(Log_EventRec *er)
{
    Text_RopeId rope;
    String      fmt;
    Bits32      hi, lo;
    static Bool busy = FALSE;
    IArg key;

    if (LoggerPosix_enabled == FALSE) {
        return;
    }

    //key = Gate_enterSystem();
    if (!busy) {
        busy = TRUE;

        //Gate_leaveSystem(key);

        /*
         *  print serial number if there is one;
         *  0 isn't a valid serial number
         */
        if (er->serial) {
            System_printf(SERIAL, er->serial);
        }

        /* print timestamp if there is one; ~0 isn't a valid timestamp value */
        hi = er->tstamp.hi;
        lo = er->tstamp.lo;
        if (lo != ~0 && hi != ~0) {
            System_printf("[t=0x");
            if (hi) {
                System_printf(HI, hi);
            }
            System_printf(LO, lo);
        }

        System_printf("[tid=0x%x] ", pthread_self());

        /* print module name */
        Text_putMod((Text_RopeId)Types_getModuleId(er->evt), NULL, -1);
        System_printf(": ");

        /* print event */
        rope = Types_getEventId(er->evt);  /* event id is the message rope */
        if (rope == 0) {
            /* Log_print() event */
            System_aprintf((String)iargToPtr(er->arg[0]),
                    er->arg[1], er->arg[2], er->arg[3], er->arg[4],
                    er->arg[5], er->arg[6], 0,          0);
        }
        else {
            /* Log_write() event */
            fmt = Text_ropeText(rope);

            if (Text_isLoaded) {
                System_aprintf(fmt, er->arg[0], er->arg[1], er->arg[2],
                        er->arg[3], er->arg[4], er->arg[5], er->arg[6],
                        er->arg[7]);
            }
            else {
                System_aprintf("{evt: fmt=%p, args=[0x%x, 0x%x ...]}",
                        fmt, er->arg[0], er->arg[1]);
            }
        }

        System_putch('\n');

        busy = FALSE;
    }
    else {
        //Gate_leaveSystem(key);
    }
}
/*
 *  @(#) linuxdist; 1, 0, 0,1; 2-15-2012 07:46:08; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

