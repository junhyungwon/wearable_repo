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
 *  ======== TimestampProvider.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Types.h>

#include <ti/sysbios/family/windows/TaskSupport.h>

#include "package/internal/TimestampProvider.xdc.h"

#define _WIN32_WINNT 0x0400     /* needed for SingalObjectAndWait */
#include <windows.h>
#include <winbase.h>


static Void getTimestampCount64(Types_Timestamp64 *result);


/*
 *************************************************************************
 *                      MODULE FUNCTIONS
 *************************************************************************
 */


/*
 *  ======== TimestampProvider_get32 ========
 */
Bits32 TimestampProvider_get32() 
{
    Types_Timestamp64   result;

    getTimestampCount64(&result);
    return (result.lo);
}


/*
 *  ======== TimestampProvider_get64 ========
 */
Void TimestampProvider_get64(Types_Timestamp64 *result) 
{
    getTimestampCount64(result);
}

/*
 *  ======== TimestampProvider_getFreq ========
 */
Void TimestampProvider_getFreq(Types_FreqHz *freq) 
{
    freq->lo = 10000000;
    freq->hi = 0;
}


/*
 *************************************************************************
 *                      INSTANCE FUNCTIONS
 *************************************************************************
 */




/*
 *************************************************************************
 *                      OTHER FUNCTIONS
 *************************************************************************
 */


/*
 *  ======== getTimestampCount64 ========
 */
Void getTimestampCount64(Types_Timestamp64 *result)
{
    FILETIME    createTime;
    FILETIME    exitTime;
    FILETIME    kernelTime;
    FILETIME    userTime;


    /* enter hwi critical section to prevent interrupt preemption */
    ti_sysbios_family_windows_TaskSupport_enterCS__I();

    /* get process times, this updates once every 10-15 milliseconds */
    if (!GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime,
        &kernelTime, &userTime)) {
        Error_raise(NULL, TimestampProvider_E_processTimes, 0, 0);
    }

    /* return current timestamp (may be same as previous value) */
    result->lo = userTime.dwLowDateTime;
    result->hi = userTime.dwHighDateTime;

    /* leave hwi critical section */
    ti_sysbios_family_windows_TaskSupport_leaveCS__I();
}

/*
 */
/*
 *  @(#) ti.sysbios.family.windows; 2, 0, 0, 0,553; 5-18-2012 06:05:20; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

