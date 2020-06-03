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
 *  ======== Timer.c ========
 */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Types.h>

#include <ti/sysbios/family/windows/Hwi.h>
#include <ti/sysbios/family/windows/TaskSupport.h>
#include <ti/sysbios/interfaces/ITimer.h>

#include "package/internal/Timer.xdc.h"

#define _WIN32_WINNT 0x0400     /* needed for SingalObjectAndWait */
#include <windows.h>
#include <winbase.h>


typedef struct {
    ITimer_Status       status;
    ITimer_RunMode      mode;
    UInt32              period;         /* number of counts per tick    */
    UInt32              counter;        /* clock cycle counter          */
    UInt                interrupt;      /* interrupt number             */
    Bool                running;        /* has timer been started       */
} TimerData;

typedef struct Timer_PrivateData {
    HANDLE      createEvent;
    Int         runningTimers;          /* number of running timers     */
    TimerData   timer[Timer_NUM_TIMER_DEVICES];
    HANDLE      timerEvent;
    HANDLE      timerThread;
};



static char const intNumDef[2] = {14, 15};

static Int              postInit(Timer_Object *obj, Error_Block *eb);
static DWORD WINAPI     timerThreadProc(LPVOID param);



/*
 *************************************************************************
 *                      MODULE FUNCTIONS
 *************************************************************************
 */


/*
 *  ======== Timer_Module_startup ========
 */
Int Timer_Module_startup(Int phase)
{
    DWORD           waitStatus;
    Timer_Object    *obj;
    Int             i;


    /* must wait for these modules to initialize first */
    if (!Startup_rtsDone()
        || !Hwi_Module_startupDone()) {

        return (Startup_NOTDONE);
    }

    /* okay to proceed with initialization */

    /* allocate a private data object */
    Timer_module->privateData = 
        (Timer_PrivateData*)calloc(1, sizeof(Timer_PrivateData));

    if (Timer_module->privateData == NULL) {
        Error_raise(NULL, Timer_E_calloc, 0, 0);
    }

    /* create the create-thread event object */
    Timer_module->privateData->createEvent = 
        CreateEvent(NULL, FALSE, FALSE, NULL);

    if (Timer_module->privateData->createEvent == NULL) {
        Error_raise(NULL, Timer_E_event, 0, 0);
    }

    /* no timers running to start with */
    Timer_module->privateData->runningTimers = 0;

    /* mark all timers as free */
    for (i = 0; i < Timer_NUM_TIMER_DEVICES; i++) {
        Timer_module->privateData->timer[i].status = ITimer_Status_FREE;
    }

    /* create the timer event object */
    Timer_module->privateData->timerEvent = 
        CreateEvent(NULL, FALSE, FALSE, NULL);

    if (Timer_module->privateData->timerEvent == NULL) {
        Error_raise(NULL, Timer_E_event, 0, 0);
    }

    /* create the timer thread */
    Timer_module->privateData->timerThread = CreateThread(
        NULL, 0, timerThreadProc, (LPVOID)(Timer_module->privateData), 0, NULL);

    if (Timer_module->privateData->timerThread == NULL) {
        Error_raise(NULL, Timer_E_thread, 0, 0);
    }

    /* wait here until timer thread starts */
    waitStatus = WaitForSingleObject(
        Timer_module->privateData->createEvent, INFINITE);

    if (waitStatus != WAIT_OBJECT_0) {
        Error_raise(NULL, Timer_E_wait, 0, 0);
    }

    /* complete initialization for statically created/constructed timers */
    for (i = 0; i < Timer_NUM_TIMER_DEVICES; i++) {
        obj = Timer_module->handles[i];
        /* if timer was statically created/constructed */
        if (obj != NULL) {
            postInit(obj, NULL);
        }
    }

    return (Startup_DONE);
}


/*
 *  ======== Timer_getNumTimers ========
 */
UInt Timer_getNumTimers()
{
    return (Timer_NUM_TIMER_DEVICES);
}


/*
 *  ======== Timer_getStatus ========
 */
Timer_Status Timer_getStatus(UInt id)
{
    Timer_Status    rval;
    
    Assert_isTrue(id < Timer_NUM_TIMER_DEVICES, NULL);

    if (Timer_module->availMask & (1 << id)) {
        rval = Timer_Status_FREE;
    }
    else {
        rval = Timer_Status_INUSE;
    }

    return (rval);
}

/*
 *  ======== Timer_getMaxTicks ========
 */
UInt32 Timer_getMaxTicks(Timer_Object *obj, UInt32 periodCounts)
{
    return (0);
}

/*
 *  ======== Timer_setNextTick ========
 */
Void Timer_setNextTick(Timer_Object *obj, UInt32 newPeriodCounts,
    UInt32 countsPerTick)
{
}

/*
 *  ======== Timer_startup ========
 *  Here after main(). Called from BIOS_start()
 *  Start all statically created/constructed timers 
 *  Start all timers with startMode == StartMode_AUTO.
 */
Void Timer_startup()
{
    Int i;
    Timer_Object *obj;

    for (i = 0; i < Timer_NUM_TIMER_DEVICES; i++) {
        obj = Timer_module->handles[i];
        /* if timer was statically created/constructed */
        if ((obj != NULL) && (obj->startMode == Timer_StartMode_AUTO)) {
            Timer_start(obj);
        }
    }
}


/*
 *************************************************************************
 *                      INSTANCE FUNCTIONS
 *************************************************************************
 */


/*
 *  ======== Timer_Instance_init ========
 */
Int Timer_Instance_init(Timer_Object *obj, Int id, Timer_FuncPtr tickFxn, const Timer_Params *params,
        Error_Block *eb)
{
    Hwi_Params hwiParams;
    Int status;
    Int i;

    /* validate timer id */
    if ((id < -1) || (Timer_NUM_TIMER_DEVICES <= id)) {
        Error_raise(eb, Timer_E_invalidTimer, id, 0);
        return (1);
    }

    /* set id to invalid value to check for error below */
    obj->id = Timer_NUM_TIMER_DEVICES;

    /* find any available timer */
    if (id == Timer_ANY) {
        for (i = 0; i < Timer_NUM_TIMER_DEVICES; i++) {
            if ((Timer_anyMask & (1 << i))
                && (Timer_module->availMask & (1 << i))) {
                Timer_module->availMask &= ~(1 << i);
                obj->id = i;
                break;
            }
        }
        if (Timer_NUM_TIMER_DEVICES == obj->id) {
            Error_raise(eb, Timer_E_noneAvailable, 0, 0);
            return (2);
        }
    }

    /* check if requested timer is available */
    else if (Timer_module->availMask & (1 << id)) {
        Timer_module->availMask &= ~(1 << id);
        obj->id = id;
    }

    if (obj->id == Timer_NUM_TIMER_DEVICES) {
        Error_raise(eb, Timer_E_notAvailable, id, 0);
        return (2);
    }

    /* assign interrupt to this timer */
    obj->intNum = intNumDef[obj->id];

    /* initialize timer object */
    obj->runMode = params->runMode;
    obj->startMode = params->startMode;
    obj->period = params->period;
    obj->tickFxnArg = params->arg;
    obj->tickFxn = tickFxn;

    if (obj->tickFxn != NULL) {
        Hwi_Params_init(&hwiParams);
        hwiParams.arg = (UArg)obj->tickFxnArg;
        obj->hwi = Hwi_create(obj->intNum, obj->tickFxn, &hwiParams, eb);
        if (Error_check(eb)) {
            return (2);
        }
    }
    else {
        obj->hwi = NULL;
    }

    status = postInit(obj, eb);
    if (status != 0) {
        return (status);
    }

    /* if start mode == AUTO, start timer now */
    if (obj->startMode == Timer_StartMode_AUTO) {
        Timer_start(obj);
    }

    return (0);
}


/*
 *  ======== Timer_Instance_finalize ========
 */
Void Timer_Instance_finalize(Timer_Object *obj, Int status)
{

    switch (status) {
        case 0: /* Timer_delete() */
        case 3: /* device config (setPeriodMicroSecs) failed */
            if (obj->hwi != NULL) {
                Hwi_delete(&obj->hwi);
            }
            Timer_module->availMask |= (1 << obj->id);
            Timer_module->privateData->timer[obj->id].status =
                ITimer_Status_FREE;

        case 2: /* timer not available */
        case 1: /* invalid timer id */
        default:
            break;      
    }
}


/*
 *  ======== Timer_start ========
 */
Void Timer_start(Timer_Object *obj)
{
    TimerData   *timer;

    ti_sysbios_family_windows_TaskSupport_enterCS__I();

    timer = &(Timer_module->privateData->timer[obj->id]);
    if (FALSE == timer->running) {
        timer->counter = timer->period;
        timer->running = TRUE;

        /* if needed, start the timer thread clocking; always increment count */
        if (Timer_module->privateData->runningTimers++ == 0) {
            SetEvent(Timer_module->privateData->timerEvent);
        }
    }

    ti_sysbios_family_windows_TaskSupport_leaveCS__I();
}


/*
 *  ======== Timer_trigger ========
 */
Void Timer_trigger(Timer_Object *obj, UInt32 insts)
{
    Error_raise(NULL, Timer_E_notSupported, 0, 0);
}


/*
 *  ======== Timer_stop ========
 */
Void Timer_stop(Timer_Object *obj)
{
    TimerData   *timer;

    ti_sysbios_family_windows_TaskSupport_enterCS__I();

    timer = &(Timer_module->privateData->timer[obj->id]);
    if (timer->running) {
        timer->running = FALSE;
        Timer_module->privateData->runningTimers--;
    }

    ti_sysbios_family_windows_TaskSupport_leaveCS__I();
}


/*
 *  ======== Timer_getFreq ========
 */
Void Timer_getFreq(Timer_Object *obj, Types_FreqHz *freq) 
{
    /* The timer is emulated by a thread which calls Sleep(). The sleep
     * value is specified in milliseconds, which emulates the timer period.
     * For example, to set the timer to raise an interrupt every second, a
     * period of 1000 must be specified. Thus, the emulated clock frequency 
     * driving the timer is 1000 Hz, although the resolution of the timer
     * is betwee 10 - 15 milliseconds.
     */
    freq->lo = 1000;
    freq->hi = 0;
}


/*
 *  ======== Timer_getPeriod ========
 */
UInt32 Timer_getPeriod(Timer_Object *obj)
{
    return (obj->period);
}

/*
 *  ======== Timer_getCount ========
 */
UInt32 Timer_getCount(Timer_Object *obj)
{
    return (0);
}

/*
 *  ======== Timer_getExpiredCounts ========
 */
UInt32 Timer_getExpiredCounts(Timer_Object *obj)
{
    return (0);
}

/*
 *  ======== Timer_setPeriod ========
 */
Void Timer_setPeriod(Timer_Object *obj, UInt32 period)
{
    Timer_stop(obj);
    obj->period = period;
    Timer_module->privateData->timer[obj->id].period = period;
}


/*
 *  ======== Timer_setPeriodMicroSecs ========
 */
Bool Timer_setPeriodMicroSecs(Timer_Object *obj, UInt32 period)
{
    UInt32      periodMilliSec = period / 1000;

    Timer_stop(obj);

    /* convert microsecond period to milliseconds */
    periodMilliSec = period / 1000;

    if (periodMilliSec < 1) {
        return (FALSE);
    }
    else {
        Timer_module->privateData->timer[obj->id].period = periodMilliSec;
        return (TRUE);
    }
}

/*
 *  ======== Timer_getFunc ========
 */
Timer_FuncPtr Timer_getFunc(Timer_Object *obj, UArg *arg)
{
    *arg = obj->tickFxnArg;
    return (obj->tickFxn);
}

/*
 *  ======== Timer_setFunc ========
 */
Void Timer_setFunc(Timer_Object *obj, Timer_FuncPtr fxn, UArg arg)
{
    obj->tickFxn = fxn;
    obj->tickFxnArg = arg;
}

/*
 *************************************************************************
 *                      OTHER FUNCTIONS
 *************************************************************************
 */


/*
 *  ======== Timer_postInit ========
 */
Int postInit(Timer_Object *obj, Error_Block *eb)
{
    Timer_PrivateData   *privateData = Timer_module->privateData;


    /* allocate and initialize the timer */
    privateData->timer[obj->id].status = ITimer_Status_INUSE;
    privateData->timer[obj->id].mode = obj->runMode;
    privateData->timer[obj->id].interrupt = obj->intNum;
    privateData->timer[obj->id].running = FALSE;

    if (obj->periodType == ITimer_PeriodType_MICROSECS) {
        if (!Timer_setPeriodMicroSecs(obj, obj->period)) {
            Error_raise(eb, Timer_E_cannotSupport, obj->period, 0);
            return (3);
        }
    }
    else {
        privateData->timer[obj->id].period = obj->period;
    }

    return (0);
}


/*
 *  ======== timerThreadProc ========
 *  Start routine for the timer thread.
 */
static DWORD WINAPI timerThreadProc(LPVOID param)
{
    Timer_PrivateData   *privateData = (Timer_PrivateData *)param;
    TimerData           *timer;
    DWORD               waitStatus;
    Int                 i;
    UInt32              minCount;


    /* raise this thread to highest priority level */
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    /* signal create thread to indicate this thread is ready */
    SetEvent(privateData->createEvent);

    /* main loop */
    do {

        /* block here until timer clock is really needed */
        if (privateData->runningTimers == 0) {
            waitStatus = WaitForSingleObject(privateData->timerEvent, INFINITE);

            if (waitStatus != WAIT_OBJECT_0) {
                Error_raise(NULL, Timer_E_wait, 0, 0);
            }
        }

        ti_sysbios_family_windows_TaskSupport_enterCS__I();

        /* determine the sleep period to the next timer event */
        minCount = ~0;

        for (i = 0; i < Timer_NUM_TIMER_DEVICES; i++) {
            timer = &(privateData->timer[i]);

            if ((timer->status == ITimer_Status_INUSE) && (timer->running)) {
                if (minCount  > timer->counter) {
                    minCount = timer->counter;
                }
            }
        }

        ti_sysbios_family_windows_TaskSupport_leaveCS__I();

        /* sleep until next timer event */
        Sleep(minCount);  /* specified in milliseconds */

        ti_sysbios_family_windows_TaskSupport_enterCS__I();

        /* increment timer counter, raise interrupt if period expired */
        for (i = 0; i < Timer_NUM_TIMER_DEVICES; i++) {
            timer = &(privateData->timer[i]);

            if ((timer->status == ITimer_Status_INUSE) && timer->running) {
                if (timer->counter <= minCount) {
                    Hwi_post(timer->interrupt);

                    if (timer->mode == ITimer_RunMode_ONESHOT) {
                        timer->running = FALSE;
                        privateData->runningTimers--;
                    }
                    else {
                        timer->counter = timer->period;
                    }
                }
                else {
                    timer->counter -= minCount;
                }
            }
        }

        ti_sysbios_family_windows_TaskSupport_leaveCS__I();

    } while (1);  // TODO - need a terminal check here

    /* terminate this thread, implicit call to ExitThread */
    return (0);
}

/*
 */

/*
 *  @(#) ti.sysbios.family.windows; 2, 0, 0, 0,553; 5-18-2012 06:05:19; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

