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
 *  ======== LoggerSysTID.c ========
 *  Just like LoggerSys, but also prints thread ID.
 */
#include <xdc/std.h>

#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Text.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Startup.h>

#include <pthread.h>
#include <string.h>

#include "package/internal/LoggerSysTID.xdc.h"


#if (xdc_target__bitsPerChar * xdc_target__sizeof_Int) == 32
#define SERIAL "#%010u "
#define HI "%0.8x:"
#define LO "%0.8x] "
#define LO_U "%0.8x us] "
#define LO_DU "+%03d,%03d us] "
#elif (xdc_target__bitsPerChar * xdc_target__sizeof_Long) == 32
#define SERIAL "#%010lu "
#define HI "%0.8lx:"
#define LO "%0.8lx] "
#define LO_U "%0.8lx us] "
#define LO_DU "+%03d,%03d us] "
#else
#error xdc.runtime.Log does not support this target.
#endif

/*
 *  time formats, that the user can set through
 *  LoggerSysTID_control(LoggerSysTID_SETTRACE, (UArg)"time=<class>");
 *    : @0x:<microseconds in hex>:
 *   1: @<seconds>,<milliseconds>,<microseconds>us, e.g. @4,021,348us:
 *   2: <seconds>.<milliseconds>s                   e.g. @0.001s:
 *   3: +<milliseconds>,<microseconds>us -- delta from previous, not counting
 *      print time, e.g. @+0,000,259us:
 */
#define TIMEMASK      "time"
#define TIME_DEFAULT   0
#define TIME_USEC      1
#define TIME_SEC       2
#define TIME_DELTAUSEC 4
#define TIME_TICKS     8

#define MAXMODNAMELENGTH 80
#define WILD '%'
#define CLEAR        '='
#define ON           '+'
#define OFF          '-'

typedef enum {
    SEP,
    NAME,
    OP,
    DIGITS
} State;


static Void applyOp(UInt8 *mask, Char op, UInt8 opMask);
static Void doPrint(Log_EventRec *er);
static Void initFreq(Void);
static Void setMask(String modName, Char op, UInt8 mask);
static Void setTrace(String trace);

static UInt32 freq;
static String separator = " ,;/";
static UInt8  timeMask = TIME_DEFAULT;  /* default time mask */
static UInt8  timeModes = 4;            /* Mask of supported timestamp modes */

static Bits32 prev_lo = 0;      /* Previous timestamp (ticks) */
static Bits32 prev_hi = 0;
static Bits32 delta_lo = 0;     /* current ts - previous ts (ticks) */
static Bits32 delta_hi = 0;

static UInt32 delta_hi_u = 0;   /* current ts - previous ts (usecs) */
static UInt32 delta_lo_u = 0;

static Types_FreqHz cpufreq;


/*
 *  ======== Log_Module_startup ========
 */
Int LoggerSysTID_Module_startup(Int phase)
{
    Types_Timestamp64 time64;

    LoggerSysTID_TimestampProxy_get64(&time64);

    /* Initialize first time stamp */
    prev_hi = time64.hi;
    prev_lo = time64.lo;

    /* Initialize the CPU frequency */
    initFreq();

    return (Startup_DONE);
}

/*
 *  ======== Instance_init =========
 */
Void LoggerSysTID_Instance_init(LoggerSysTID_Object *obj,
    const LoggerSysTID_Params *prms)
{
    obj->enabled = TRUE;
}

/*
 *  ======== LoggerSysTID_control =========
 */
Void LoggerSysTID_control(Int cmd, UArg cmdArgs)
{
    switch (cmd) {
        case LoggerSysTID_SETTRACE:
            /* Set output format based on cmdArgs */
            setTrace((String)cmdArgs);
            break;

        default:
            /* Unsupported command */
            break;
    }
}

/*
 *  ======== LoggerSysTID_disable ========
 */
Bool LoggerSysTID_disable(LoggerSysTID_Object *obj)
{
    Bool prev = obj->enabled;
    obj->enabled = FALSE;
    return (prev);
}

/*
 *  ======== LoggerSysTID_enable ========
 */
Bool LoggerSysTID_enable(LoggerSysTID_Object *obj)
{
    Bool prev = obj->enabled;
    obj->enabled = TRUE;
    return (prev);
}

/*
 *  ======== write0 =========
 */
Void LoggerSysTID_write0(LoggerSysTID_Object *obj, Log_Event evt,
                      Types_ModuleId mid)
{
    LoggerSysTID_write8(obj, evt, mid, 0, 0, 0, 0, 0, 0, 0, 0);
}

/*
 *  ======== write1 =========
 */
Void LoggerSysTID_write1(LoggerSysTID_Object *obj, Log_Event evt,
                      Types_ModuleId mid, IArg a1)
{
    LoggerSysTID_write8(obj, evt, mid, a1, 0, 0, 0, 0, 0, 0, 0);
}

/*
 *  ======== write2 =========
 */
Void LoggerSysTID_write2(LoggerSysTID_Object *obj, Log_Event evt,
                      Types_ModuleId mid, IArg a1, IArg a2)
{
    LoggerSysTID_write8(obj, evt, mid, a1, a2, 0, 0, 0, 0, 0, 0);
}

/*
 *  ======== write4 =========
 */
Void LoggerSysTID_write4(LoggerSysTID_Object *obj, Log_Event evt,
                      Types_ModuleId mid, IArg a1, IArg a2, IArg a3, IArg a4)
{
    LoggerSysTID_write8(obj, evt, mid, a1, a2, a3, a4, 0, 0, 0, 0);
}

/*
 *  ======== write8 =========
 */
Void LoggerSysTID_write8(LoggerSysTID_Object *obj, Log_Event evt,
                      Types_ModuleId mid, IArg a1, IArg a2, IArg a3, IArg a4,
                      IArg a5, IArg a6, IArg a7, IArg a8)
{
    Log_EventRec evrec;
    Bits32       cur_hi, cur_lo;

    if (!obj->enabled) {
        return;
    }

    LoggerSysTID_TimestampProxy_get64(&evrec.tstamp);

    if (timeMask == TIME_DELTAUSEC) {
        /*
         *  NOTE: This is not thread-safe. For thread-safe timestamp, stick
         *  to the default.
         */
        cur_hi = evrec.tstamp.hi;
        cur_lo = evrec.tstamp.lo;

        if (cur_lo < prev_lo) {
            /* Lower 32 bits of counter wrapped */
            delta_lo = (Bits32)(0xFFFFFFFF -
                    ((UInt32)prev_lo - (UInt32)cur_lo) + 1);
            cur_hi--;
            delta_hi = cur_hi - prev_hi - 1;
        }
        else {
            delta_lo = cur_lo - prev_lo;
            delta_hi = cur_hi - prev_hi;
        }

        prev_hi = cur_hi;
        prev_lo = cur_lo;
        delta_lo_u = delta_lo / freq;
        delta_hi_u = delta_hi / freq;

        evrec.tstamp.hi = delta_hi_u;
        evrec.tstamp.lo = delta_lo_u;
    }

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
 *  ======== applyOp ========
 *  applies given operation and mask on the set of flags
 */
static Void applyOp(UInt8 *mask, Char op, UInt8 opMask)
{
    switch (op) {
        case (Int)CLEAR:
            *mask = opMask;
            break;
        case (Int)ON:
            *mask |= opMask;
            break;
        case (Int)OFF:
            *mask &= ~opMask;
            break;
        default :
            break;
    }
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

    if (!busy) {
        busy = TRUE;

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
            if (timeMask == TIME_DELTAUSEC) {
                System_printf("[t=");
                System_printf(LO_DU, lo / 1000, lo % 1000);
            }
            else {
                System_printf("[t=0x");
                if (hi) {
                    System_printf(HI, hi);
                }
                System_printf(LO, lo);
            }
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
}

/*
 *  ======== initFreq ========
 */
static Void initFreq()
{
    Types_Timestamp64 time64;

    LoggerSysTID_TimestampProxy_getFreq(&cpufreq);

    /* Don't support frequencies greater than 2**32 - 1 */
    if (cpufreq.hi > 0) {
        timeModes = 0;      /* Only allow display of timer ticks */
    }
    else {
        freq = (cpufreq.lo != 0) ? cpufreq.lo : 1;
        freq /= 1000000;    /* convert to ticks per microsecond */
        if (freq <= 0) {
            timeModes = 0;  /* Only allow display of timer ticks */
        }
    }
}


/*
 *  ======== setMask ========
 */
static Void setMask(String modName, Char op, UInt8 mask)
{
    if (!strcmp(modName, TIMEMASK)) {
        /* Change time output format if allowed */
        if (mask == 0) {
            timeMask = TIME_DEFAULT;
        }
        else if ((timeModes & mask)) {
            applyOp(&timeMask, op, mask);
        }
    }
}

/*
 *  ======== setTrace ========
 */
Void setTrace(String trace)
{
    State   state = SEP;
    String  sep;
    Bool    maskValid = FALSE;
    Int16   digit;
    Char    op = CLEAR;
    Char    modName[MAXMODNAMELENGTH];
    Int     modNameLength = 0;
    UInt8   mask = 0x0;             /* no tracing */

    while (*trace != '\0') {
        switch ((Int)state) {
            case (Int)SEP:
                sep = separator;
                while (*sep != '\0') {
                    if (*trace == *sep) {
                        trace++;
                        break;
                    }
                    else {
                        sep++;
                    }
                }
                if (*sep == '\0') {
                    state = NAME;
                    modName[modNameLength = 0] = '\0';
                }
                break;

            case (Int)NAME:
                if ((*trace >= 'A' && *trace <= 'Z') ||
                        (*trace >= 'a' && *trace <= 'z') ||
                        (*trace >= '0' && *trace <= '9') ||
                        (*trace == '.' || *trace == '_') ||
                        (*trace == WILD)
                    ) {
                    if (modNameLength < MAXMODNAMELENGTH - 1) {
                        modName[modNameLength++] = *trace;
                    }
                    trace++;
                }
                else {
                    /* we assume anything else must be an operator */
                    modName[modNameLength] = '\0';
                    state = OP;
                }
                break;

            case (Int) OP:
                op = *trace;
                mask = 0x0;     /* no tracing */
                switch (op) {
                    case (Int)CLEAR:
                        maskValid = TRUE;
                    case (Int)ON:
                    case (Int)OFF:
                        state = DIGITS;
                        trace++;
                        break;
                    default :
                        state = SEP;
                        break;
                }
                break;

            case (Int)DIGITS:
                digit = (Int16)(*trace - '0');
                if ((digit >= 0) && (digit <= 7)) {
                    mask |= (0x01 << digit);
                    maskValid = TRUE;
                    trace++;
                }
                else {
                    if (maskValid == TRUE) {
                        setMask(modName, op, mask);
                        maskValid = FALSE;
                    }
                    state = SEP;
                }
                break;

            default :
                break;
        }
    }

    if (maskValid) {
        modName[modNameLength] = '\0';
        setMask(modName, op, mask);
    }
}
/*
 *  @(#) ti.sdo.utils.loggers; 1, 0, 0,1; 2-15-2012 07:44:56; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

