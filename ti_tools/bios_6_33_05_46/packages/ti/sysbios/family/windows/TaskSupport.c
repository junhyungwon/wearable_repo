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
 *  ======== TaskSupport.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Types.h>


#define ti_sysbios_knl_Task__internalaccess
#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/interfaces/ITaskSupport.h>

#include "package/internal/TaskSupport.xdc.h"

#define _WIN32_WINNT 0x0400     /* needed for SingalObjectAndWait */
#include <windows.h>
#include <winbase.h>


#define Internal                /* identify module internal functions */
#define MAX_INTERRUPTS  32


typedef struct {
    HANDLE      handle;
    DWORD       id;
} ThreadInfo;

typedef struct TaskSupport_Task {
    TaskSupport_State   state;
    BOOL                preemptable;
    HANDLE              taskEvent;
    HANDLE              taskThread;
    DWORD               taskThreadId;
    HANDLE              interruptEvent; // TODO combine with taskEvent
    ThreadInfo          intrThreads[MAX_INTERRUPTS];
    UInt                intrCount;
    TaskSupport_GlueFxn enter;
    TaskSupport_GlueFxn exit;
    TaskSupport_TaskFxn fxn;
    UArg                arg0;
    UArg                arg1;
};

typedef struct TaskSupport_PrivateData {
    DWORD               bootThreadId;
    HANDLE              bootEvent;
    HANDLE              createTaskEvent;
    HANDLE              createInterruptEvent;
    CRITICAL_SECTION    emulationCS;
};

typedef struct {
    TaskSupport_Task    *task;
    TaskSupport_IsrFxn  isrFxn;
    UInt                intr;
} InterruptThreadData;


static DWORD WINAPI     interruptThreadProc(LPVOID param);
static DWORD WINAPI     taskThreadProc(LPVOID param);


/*
 *************************************************************************
 *                      MODULE FUNCTIONS
 *************************************************************************
 */


/*
 *  ======== TaskSupport_Module_startup ========
 *  Initialize and start the TaskSupport Module.
 *  Called at system init time before main().
 */
Int TaskSupport_Module_startup(Int phase)
{
    /* lower the process priority class */
    SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

    /* initialize the module state */
    TaskSupport_module->currentTask = NULL;

    /* allocate a private data object */
    TaskSupport_module->privateData = 
        (TaskSupport_PrivateData*)calloc(1, sizeof(TaskSupport_PrivateData));

    if (TaskSupport_module->privateData == NULL) {
        Error_raise(NULL, TaskSupport_E_calloc, 0, 0);
    }

    TaskSupport_module->privateData->bootThreadId = GetCurrentThreadId();

    /* create boot thread event object */
    TaskSupport_module->privateData->bootEvent = 
        CreateEvent(NULL, FALSE, FALSE, NULL);

    if (TaskSupport_module->privateData->bootEvent == NULL) {
        Error_raise(NULL, TaskSupport_E_event, 0, 0);
    }

    /* create event objects */
    TaskSupport_module->privateData->createTaskEvent = 
        CreateEvent(NULL, FALSE, FALSE, NULL);

    if (TaskSupport_module->privateData->createTaskEvent == NULL) {
        Error_raise(NULL, TaskSupport_E_event, 0, 0);
    }

    TaskSupport_module->privateData->createInterruptEvent = 
        CreateEvent(NULL, FALSE, FALSE, NULL);

    if (TaskSupport_module->privateData->createInterruptEvent == NULL) {
        Error_raise(NULL, TaskSupport_E_event, 0, 0);
    }

    /* initialize critical section object */
    InitializeCriticalSection(&TaskSupport_module->privateData->emulationCS);

    /* okay to proceed with initialization */
    return (Startup_DONE);
}


/*
 *  ======== TaskSupport_checkStack ========
 */
Bool TaskSupport_checkStack(Char *stack, SizeT size)
{
    return (TRUE);      /* stack OK */
}


/*
 *  ======== TaskSupport_stackUsed ========
 */
SizeT TaskSupport_stackUsed(Char *stack, SizeT size)
{
    return (0);
}


/*
 *  ======== TaskSupport_start ========
 */
Ptr TaskSupport_start(Ptr currTsk, ITaskSupport_FuncPtr enter,
        ITaskSupport_FuncPtr exit, Error_Block *eb)
{
    TaskSupport_Task    *emuTask;
    DWORD               waitStatus;
    Int                 i;
    Task_Object         *tsk = (Task_Object *)currTsk;

    /* create an emulation task object */
    emuTask = (TaskSupport_Task *)calloc(1, sizeof(TaskSupport_Task));

    if (emuTask == NULL) {
        Error_raise(eb, TaskSupport_E_calloc, 0, 0);
        return (NULL);
    }

    /* initialize the task object */
    emuTask->state = TaskSupport_State_YIELDED;
    emuTask->preemptable = FALSE;
    emuTask->taskEvent = NULL;
    emuTask->taskThread = NULL;
    emuTask->taskThreadId = 0;
    emuTask->interruptEvent = NULL;

    for (i = 0; i < MAX_INTERRUPTS; i++) {
        emuTask->intrThreads[i].handle = NULL;
        emuTask->intrThreads[i].id = 0;
    }
    emuTask->intrCount = 0;

    emuTask->enter = NULL;
    emuTask->exit = NULL;
    emuTask->fxn = NULL;

    /* create task thread event object */
    emuTask->taskEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    if (emuTask->taskEvent == NULL) {
        Error_raise(eb, TaskSupport_E_event, 0, 0);
        return (NULL);
    }

    /* create the interrupt thread event object */
    emuTask->interruptEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    if (emuTask->interruptEvent == NULL) {
        Error_raise(eb, TaskSupport_E_event, 0, 0);
        return (NULL);
    }

    /* create the task thread */
    emuTask->enter = enter;
    emuTask->exit = exit;
    emuTask->fxn = tsk->fxn;
    emuTask->arg0 = tsk->arg0;
    emuTask->arg1 = tsk->arg1;

    emuTask->taskThread = CreateThread(NULL, 0,
        taskThreadProc, (LPVOID)emuTask, 0, &(emuTask->taskThreadId));

    if (emuTask->taskThread == NULL) {
        Error_raise(eb, TaskSupport_E_thread, 0, 0);
        return (NULL);
    }

    /* wait here until task thread starts */
    waitStatus = WaitForSingleObject(
        TaskSupport_module->privateData->createTaskEvent, INFINITE);

    if (waitStatus != WAIT_OBJECT_0) {
        Error_raise(eb, TaskSupport_E_wait, 0, 0);
        return (NULL);
    }

    /* success - return task pointer cast to generic Ptr */
    return ((Ptr)emuTask);
}


/*
 *  ======== TaskSupport_swap ========
 */
Void TaskSupport_swap(Ptr *oldContext, Ptr *newContext)
{
    HANDLE              outgoingEvent;
    HANDLE              incomingEvent;
    DWORD               waitStatus;
    DWORD               currentThreadId = GetCurrentThreadId();
    TaskSupport_Task    *outgoingTask = (TaskSupport_Task*)(*oldContext);
    TaskSupport_Task    *incomingTask = (TaskSupport_Task*)(*newContext);
    static Bool         firstTime = TRUE;


    /* this happens when Task_yield called and no other thread can run */
    if (incomingTask == outgoingTask) {
        return;
    }

    /* decide which outgoing event to wait on */
    if (firstTime) {
        outgoingEvent = TaskSupport_module->privateData->bootEvent;
        firstTime = FALSE;
        outgoingTask = NULL; /* cannot use outgoingTask on first time */
    }
    else if (outgoingTask->state == TaskSupport_State_TERMINATING) {
        /* don't need an event object for outgoing task */
    }
    else if (currentThreadId == outgoingTask->taskThreadId) {
        outgoingEvent = outgoingTask->taskEvent;
        outgoingTask->state = TaskSupport_State_YIELDED;
    }
    else if (currentThreadId == TaskSupport_module->privateData->bootThreadId) {
        outgoingEvent = TaskSupport_module->privateData->bootEvent;
    }
    else {
        outgoingEvent = outgoingTask->interruptEvent;
        outgoingTask->state = TaskSupport_State_PREEMPTED;
    }

    /* decide which incoming event to signal */
    if (incomingTask->state == TaskSupport_State_YIELDED) {
        incomingEvent = incomingTask->taskEvent;
    }
    else if (incomingTask->state == TaskSupport_State_PREEMPTED) {
        incomingEvent = incomingTask->interruptEvent;
    }
    else {
        /* error condition */
        Error_raise(NULL, TaskSupport_E_taskState, 0, 0);
    }

    /* keep track of the currently running task */
    TaskSupport_module->currentTask = incomingTask;

    /* signal the incoming thread and exit the outgoing thread */
    if (outgoingTask && (outgoingTask->state==TaskSupport_State_TERMINATING)) {

        SetEvent(incomingEvent);

        /* release all resources owned by the thread */
        CloseHandle(outgoingTask->taskEvent);
        CloseHandle(outgoingTask->interruptEvent);
        free(outgoingTask);

        /* exit the current thread */
        ExitThread(0);
    }

    /* compel the HLOS kernel to switch threads */
    else {
        waitStatus = SignalObjectAndWait(incomingEvent, outgoingEvent,
            INFINITE, FALSE);

        if (waitStatus != WAIT_OBJECT_0) {
            Error_raise(NULL, TaskSupport_E_wait, 0, 0);
        }
    }

    /* getting here means the outgoing task is running again */
    outgoingTask->state = TaskSupport_State_RUNNING;
}


/*
 *  ======== getDefaultStackSize ========
 */
SizeT TaskSupport_getDefaultStackSize()
{
    return (TaskSupport_defaultStackSize);
}


/*
 *  ======== getDefaultStackAlignment ========
 */
UInt TaskSupport_getStackAlignment()
{
    return (TaskSupport_stackAlignment);
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
 *  ======== TaskSupport_enterCS ========
 */
Internal Void TaskSupport_enterCS()
{
    if (TaskSupport_module->privateData != NULL) {
        EnterCriticalSection(&TaskSupport_module->privateData->emulationCS);
    }
}


/*
 *  ======== TaskSupport_leaveCS ========
 */
Internal Void TaskSupport_leaveCS()
{
    if (TaskSupport_module->privateData != NULL) {
        LeaveCriticalSection(&TaskSupport_module->privateData->emulationCS);
    }
}


/*
 *  ======== TaskSupport_processInterrupt ========
 *  The emulation critical section must be held before calling this
 *  function; it will be released in this function.
 */
Internal Void TaskSupport_processInterrupt(TaskSupport_IsrFxn fxn, UInt intr)
{
    HANDLE              intrThread;
    DWORD               intrThreadId;
    DWORD               waitStatus;
    DWORD               curThrId;
    TaskSupport_Task    *curTask;
    HANDLE              tskThr;
    DWORD               tskThrId;
    UInt                intrCnt;
    HANDLE              intrThr;
    DWORD               intrThrId;
    Bool                preemptingMyself;
    InterruptThreadData *intrThreadData;


    /* setup some useful aliases */
    curThrId = GetCurrentThreadId();

    curTask = TaskSupport_module->currentTask;
    if (curTask != NULL) {
        tskThr = curTask->taskThread;
        tskThrId = curTask->taskThreadId;

        intrCnt = curTask->intrCount;
        if (intrCnt != 0) {
            intrThr = curTask->intrThreads[intrCnt - 1].handle;
            intrThrId = curTask->intrThreads[intrCnt - 1].id;
        }
    }

    /* determine if current task thread is preempting itself */
    if ((curTask != NULL) && (intrCnt == 0) && (tskThrId == curThrId)) {
        preemptingMyself = TRUE;
    }
    /* determine if current interrupt thread is preempting itself */
    else if ((curTask != NULL) && (intrCnt != 0) && (intrThrId == curThrId)) {
        preemptingMyself = TRUE;
    }
    /* this thread is preempting another thread */
    else {
        preemptingMyself = FALSE;
    }

    /* if thread is self preempting, then simply call ISR directly */
    if (preemptingMyself) {
        ti_sysbios_family_windows_TaskSupport_leaveCS__I();
        fxn(intr);
        return;
    }

    /* if first interrupt, then suspend task thread */
    if ((curTask != NULL) && (intrCnt == 0) && (tskThrId != curThrId)) {
        SuspendThread(tskThr);
    }
    /* if nested interrupt, then suspend previous interrupt thread */
    else if ((curTask != NULL) && (intrCnt != 0) && (intrThrId != curThrId)) {
        SuspendThread(intrThr);
    }

    /*  create a new interrupt thread */
    intrThreadData = 
        (InterruptThreadData *)calloc(1, sizeof(InterruptThreadData));
    intrThreadData->task = curTask;
    intrThreadData->isrFxn = fxn;
    intrThreadData->intr = intr;

    intrThread = CreateThread(NULL, 0, interruptThreadProc,
        (LPVOID)intrThreadData, CREATE_SUSPENDED, &intrThreadId);

    if (intrThread == NULL) {
        Error_raise(NULL, TaskSupport_E_thread, 0, 0);
    }

    /* TODO - need to save interrupt threads on a system stack to handle
     * nested interrupt before the first task swap has been made. */
    if (curTask != NULL) {
        curTask->intrThreads[curTask->intrCount].handle = intrThread;
        curTask->intrThreads[curTask->intrCount].id = intrThreadId;
        curTask->intrCount++;
    }

    /* raise interrupt thread priority and resume its execution */
    SetThreadPriority(intrThread, THREAD_PRIORITY_HIGHEST);
    ResumeThread(intrThread);

    /* release the emulation critical section */
    ti_sysbios_family_windows_TaskSupport_leaveCS__I();

    /* wait here until interrupt thread starts */
    waitStatus = WaitForSingleObject(
        TaskSupport_module->privateData->createInterruptEvent, INFINITE);

    if (waitStatus != WAIT_OBJECT_0) {
        Error_raise(NULL, TaskSupport_E_wait, 0, 0);
    }
}


/*
 *  ======== interruptThreadProc ========
 *  Start routine for the interrupt thread. This thread is only used
 *  for processing asynchronous interrupts.
 */
static DWORD WINAPI interruptThreadProc(LPVOID param)
{
    InterruptThreadData *data = (InterruptThreadData *)param;
    TaskSupport_Task    *task = data->task;


    /* signal parent thread now */
    SetEvent(TaskSupport_module->privateData->createInterruptEvent);

    /* invoke the isr routine */
    data->isrFxn(data->intr);

    /* critical section needed to safely pop interrupt thread stack */
    ti_sysbios_family_windows_TaskSupport_enterCS__I();

    /* if nested interrupt, resume previous interrupt thread */
    if ((task != NULL) && (--(task->intrCount) != 0)) {
        ResumeThread(task->intrThreads[task->intrCount - 1].handle);
    }

    /* if first interrupt, then resume task thread */
    else if (task != NULL) {
        ResumeThread(task->taskThread);
    }

    /* leave emulation critical section */
    ti_sysbios_family_windows_TaskSupport_leaveCS__I();

    /* free param structure */
    free(param);

    /* terminate this thread, implicit call to ExitThread */
    return (0);
}


/*
 *  ======== taskThreadProc ========
 *  Start routine for the task thread.
 */
static DWORD WINAPI taskThreadProc(LPVOID param)
{
    TaskSupport_Task    *data = (TaskSupport_Task*)param;
    DWORD               waitStatus;

    /* raise thread priority */
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

    /* wait here until first swap transition from BIOS scheduler */
    waitStatus = SignalObjectAndWait(
        TaskSupport_module->privateData->createTaskEvent, data->taskEvent,
        INFINITE, FALSE);

    if (waitStatus != WAIT_OBJECT_0) {
        Error_raise(NULL, TaskSupport_E_wait, 0, 0);
    }

    /* call the BIOS entry function */
    data->enter();

    /* call the BIOS fxn function with two arguments */
    data->fxn(data->arg0, data->arg1);

    /* tag this thread as terminating */
    data->state = TaskSupport_State_TERMINATING;

    /* call the BIOS exit function */
    data->exit();

    /* terminate this thread, implicit call to ExitThread */
    return (0);
}

/*
 *
 */
/*
 *  @(#) ti.sysbios.family.windows; 2, 0, 0, 0,553; 5-18-2012 06:05:19; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

