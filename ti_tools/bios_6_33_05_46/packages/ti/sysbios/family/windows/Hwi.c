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
 *  ======== Hwi.c ========
 */
#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Log.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/interfaces/IHwi.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/windows/IntrinsicsSupport.h>
#include <ti/sysbios/family/windows/TaskSupport.h>

#include "package/internal/Hwi.xdc.h"

#define _WIN32_WINNT 0x0400     /* needed for SingalObjectAndWait */
#include <windows.h>
#include <winbase.h>

#define MAX_BIT 31  /* should get this from some windows header file */


typedef struct Hwi_PrivateData {
    UInt        interruptNumber;
    HANDLE      createEvent;
};


static Void     isrHandler(UInt intr);
static Int      postInit(Hwi_Object *hwi, Error_Block *eb);
static Void     restore(UInt key);
static Void     takeInterrupt(Void);


/*
 *************************************************************************
 *                      MODULE FUNCTIONS
 *************************************************************************
 */


/*
 *  ======== Hwi_Module_startup ========
 */
Int Hwi_Module_startup(Int phase)
{
    Int i;
    Hwi_Object *hwi;

    /* must wait for these modules to initialize first */
    if (!TaskSupport_Module_startupDone()) {
        return (Startup_NOTDONE);
    }

    /* okay to proceed with initialization */

    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].registerFxn != NULL) {
            Hwi_hooks.elem[i].registerFxn(i);
        }
    }

    /* initialize the module state */
    Hwi_module->GIE = FALSE;
    Hwi_module->IER = 0;
    Hwi_module->IFR = 0;
    Hwi_module->intrNum = 0;

    for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
        Hwi_module->IST[i] = NULL;
    }

    /* allocate a private data object */
    Hwi_module->privateData =
        (Hwi_PrivateData*)calloc(1, sizeof(Hwi_PrivateData));

    if (Hwi_module->privateData == NULL) {
        Error_raise(NULL, Hwi_E_calloc, 0, 0);
    }

    Hwi_module->privateData->interruptNumber = 0;

    /* create the create-thread event object */
    Hwi_module->privateData->createEvent =
        CreateEvent(NULL, FALSE, FALSE, NULL);

    if (Hwi_module->privateData->createEvent == NULL) {
        Error_raise(NULL, Hwi_E_event, 0, 0);
    }

    /* plug all statically created dispatched interrupts */
    for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
        if (Hwi_module->dispatchTable[i] != NULL) {
            Hwi_plug(i, Hwi_dispatch);
        }
    }

    /* plug all statically created non-dispatched interrupts */
    Hwi_init();

    /* set IER per static Hwi settings */
    Hwi_enableIER(Hwi_module->ierMask);

    for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
        hwi = Hwi_module->dispatchTable[i];
        if (hwi !=  NULL) {
            postInit(hwi, NULL);
        }
    }

    return (Startup_DONE);
}


/*
 *  ======== Hwi_startup ========
 */
Void Hwi_startup()
{
    Hwi_enable();
}

/*
 *  ======== Hwi_disable ========
 */
UInt Hwi_disable(Void)
{
    UInt key;

    ti_sysbios_family_windows_TaskSupport_enterCS__I();
    key = Hwi_module->GIE;
    Hwi_module->GIE = FALSE;
    ti_sysbios_family_windows_TaskSupport_leaveCS__I();
    return (key);
}


/*
 *  ======== Hwi_enable ========
 */
UInt Hwi_enable(Void)
{
    UInt key;

    ti_sysbios_family_windows_TaskSupport_enterCS__I();
    key = Hwi_module->GIE;
    restore(TRUE);  // will release critical section
    return (key);
}


/*
 *  ======== Hwi_restore ========
 */
Void Hwi_restore(UInt key)
{
    /* enter emulation critical section */
    ti_sysbios_family_windows_TaskSupport_enterCS__I();

    /* private version which does not acquire emulation critical section */
    restore(key);  // will release critical section
}


/*
 *  ======== restore ========
 *  Private function. Must have ownership of emulation critical section
 *  before calling this function; it will be released before this function
 *  returns.
 */
static Void restore(UInt key)
{

    /* restore GIE to give value */
    Hwi_module->GIE = key;

    /* if interrupts enabled, process highest priority pending interrupt now */
    if (Hwi_module->privateData
        && Hwi_module->GIE && (Hwi_module->IFR & Hwi_module->IER)) {
        takeInterrupt();  // will release emulation critical section
    }

    /* leave emulation critical section */
    else {
        ti_sysbios_family_windows_TaskSupport_leaveCS__I();
    }
}


/*
 *  ======== Hwi_disableInterrupt ========
 */
UInt Hwi_disableInterrupt(UInt intNum)
{
    Assert_isTrue(intNum < Hwi_NUM_INTERRUPTS, Hwi_A_interNum);
    return (Hwi_disableIER(1 << intNum));
}


/*
 *  ======== Hwi_enableInterrupt ========
 */
UInt Hwi_enableInterrupt(UInt intNum)
{
    Assert_isTrue(intNum < Hwi_NUM_INTERRUPTS, Hwi_A_interNum);
    return (Hwi_enableIER(1 << intNum));
}


/*
 *  ======== Hwi_restoreInterrupt ========
 */
Void Hwi_restoreInterrupt(UInt intNum, UInt key)
{
    if (key & (1 << intNum)) {
        Hwi_enableInterrupt(intNum);
    }
    else {
        Hwi_disableInterrupt(intNum);
    }
}


/*
 *  ======== Hwi_clearInterrupt ========
 */
Void Hwi_clearInterrupt(UInt intNum)
{
    ti_sysbios_family_windows_TaskSupport_enterCS__I();
    Hwi_module->IFR &= ~(1 << intNum);
    ti_sysbios_family_windows_TaskSupport_leaveCS__I();
}

/*
 *  ======== Hwi_disableIER ========
 */
Bits16 Hwi_disableIER(Bits16 mask)
{
    Bits16 oldIER;

    ti_sysbios_family_windows_TaskSupport_enterCS__I();
    oldIER = Hwi_module->IER;
    Hwi_module->IER &= ~mask;
    ti_sysbios_family_windows_TaskSupport_leaveCS__I();
    return (oldIER);
}


/*
 *  ======== Hwi_enableIER ========
 */
Bits16 Hwi_enableIER(Bits16 mask)
{
    Bits16 oldIER;

    ti_sysbios_family_windows_TaskSupport_enterCS__I();
    oldIER = Hwi_module->IER;
    Hwi_module->IER |= mask;
    ti_sysbios_family_windows_TaskSupport_leaveCS__I();
    return (oldIER);
}


/*
 *  ======== Hwi_restoreIER ========
 */
Bits16 Hwi_restoreIER(Bits16 mask)
{
    Bits16 oldIER;

    ti_sysbios_family_windows_TaskSupport_enterCS__I();
    oldIER = Hwi_module->IER;
    Hwi_module->IER = mask;
    ti_sysbios_family_windows_TaskSupport_leaveCS__I();
    return (oldIER);
}


/*
 *  ======== Hwi_getHandle ========
 */
Hwi_Handle Hwi_getHandle(UInt intNum)
{
    return (Hwi_module->dispatchTable[intNum]);
}


/*
 *  ======== Hwi_plug ========
 */
Void Hwi_plug(UInt intr, Hwi_PlugFuncPtr fxn)
{
    Assert_isTrue(intr < Hwi_NUM_INTERRUPTS, Hwi_A_interNum);
    Hwi_module->IST[intr] = fxn;
}


/*
 *  ======== Hwi_post ========
 */
Void Hwi_post(UInt intr)
{
    Bool        takeInter = FALSE;
    UInt        intrBit = (1 << intr);


    Assert_isTrue(intr < Hwi_NUM_INTERRUPTS, Hwi_A_interNum);
    Assert_isTrue((Hwi_module->IST[intr] != NULL)
                  || !(Hwi_module->IER & intrBit), Hwi_A_pluggedIntr);

    /* enter emulation critical section */
    ti_sysbios_family_windows_TaskSupport_enterCS__I();


    /* check for missed interrupt */
    if (Hwi_module->IFR & intrBit) {
        Log_write1(Hwi_LW_missedIntr, (IArg)intr);
    }

    /* record the interrupt in the IFR register */
    Hwi_module->IFR |= intrBit;


    /* decide if CPU should take the interrupt */
    if (Hwi_module->GIE && (Hwi_module->IER & intrBit)) {
        takeInterrupt();  // will release emulation critical section
    }

    /* leave emulation critical section */
    else {
        ti_sysbios_family_windows_TaskSupport_leaveCS__I();
    }
}

/*
 *  ======== Hwi_getTaskSP ========
 */
Char *Hwi_getTaskSP()
{
    return (NULL);
}


/*
 *  ======== Hwi_switchFromBootStack ========
 */
Void Hwi_switchFromBootStack()
{
    /* no implementation needed */
}


/*
 *  ======== ti_sysbios_family_xxx_Hwi_switchToIsrStack ========
 */
Char *ti_sysbios_family_xxx_Hwi_switchToIsrStack()
{
    return (0);
}


/*
 *  ======== ti_sysbios_family_xxx_Hwi_switchToTaskStack ========
 *  If at bottom of ISR stack already, switch to Task stack.
 */
Void ti_sysbios_family_xxx_Hwi_switchToTaskStack(Char *oldTaskSP)
{
    /* no implementation needed */
}


/*
 *************************************************************************
 *                      INSTANCE FUNCTIONS
 *************************************************************************
 */


/*
 *  ======== Hwi_Instance_init ========
 */
Int Hwi_Instance_init(Hwi_Object *hwi, Int intNum, Hwi_FuncPtr fxn,
                        const Hwi_Params *params, Error_Block *eb)
{
    Int status;

    if (Hwi_module->dispatchTable[intNum] != NULL) {
        Error_raise(eb, Hwi_E_alreadyDefined, intNum, 0);
    }

    Hwi_module->dispatchTable[intNum] = hwi;
    Hwi_plug(intNum, Hwi_dispatch);
    Hwi_reconfig(hwi, fxn, params);

    if (Hwi_hooks.length > 0) {
        /* Allocate environment space for each hook instance. */
        hwi->hookEnv = Memory_calloc(Hwi_Object_heap(),
                Hwi_hooks.length * sizeof(Ptr), 0, eb);

        if (hwi->hookEnv == NULL) {
            return (1);
        }
    }

    hwi->irp = 0;

    status = postInit(hwi, eb);

    if (Error_check(eb)) {
        return (2 + status);
    }

    return (0);
}


/*
 *  ======== postInit ========
 *  Function to be called during module startup to complete the
 *  initialization of any statically created or constructed Hwi.
 *  returns (0) and clean 'eb' on success
 *  returns 'eb' *and* 'n' for number of successful createFxn() calls iff
 *      one of the createFxn() calls fails
 */
static Int postInit (Hwi_Object *hwi, Error_Block *eb)
{
    Int i;

    for (i = 0; i < Hwi_hooks.length; i++) {
        hwi->hookEnv[i] = (Ptr)0;
        if (Hwi_hooks.elem[i].createFxn != NULL) {
            Hwi_hooks.elem[i].createFxn((IHwi_Handle)hwi, eb);

            if (Error_check(eb)) {
                return (i);
            }
        }
    }

    return (0);
}

/*
 *  ======== Hwi_Instance_finalize ========
 */
Void Hwi_Instance_finalize(Hwi_Object *hwi, Int status)
{
    Int i, cnt;
    UInt intNum;

    for (intNum = 0; intNum < 16; intNum++) {
        if (Hwi_module->dispatchTable[intNum] == hwi) {
            break;
        }
    }

    Hwi_disableInterrupt(intNum);
    Hwi_module->dispatchTable[intNum] = NULL;
    Hwi_plug(intNum, Hwi_unPluggedInterrupt);

    if (status == 1) {  /* failed Hwi_create */
        return;
    }

    if (Hwi_hooks.length > 0) {
        if (status == 0) {
            cnt = Hwi_hooks.length;
        }
        else {
            cnt = status - 2;
        }

        for (i = 0; i < cnt; i++) {
            if (Hwi_hooks.elem[i].deleteFxn != NULL) {
                Hwi_hooks.elem[i].deleteFxn((IHwi_Handle)hwi);
            }
        }

        Memory_free(Hwi_Object_heap(), hwi->hookEnv,
            Hwi_hooks.length * sizeof(Ptr));
    }
}

/*
 *  ======== Hwi_unPluggedInterrupt ========
 *  Here on interrupt unPlugged by Hwi_delete().
 */
Void Hwi_unPluggedInterrupt(UInt num) {
    while(TRUE);
}

/*
 *  ======== Hwi_getHookContext ========
 */
Ptr Hwi_getHookContext(Hwi_Object *hwi, Int id)
{
    return (hwi->hookEnv[id]);
}


/*
 *  ======== Hwi_setHookContext ========
 */
Void Hwi_setHookContext(Hwi_Object *hwi, Int id, Ptr hookContext)
{
    hwi->hookEnv[id] = hookContext;
}


/*
 *  ======== Hwi_reconfig ========
 */
Void Hwi_reconfig(Hwi_Object *hwi, Hwi_FuncPtr fxn, const Hwi_Params *params)
{
    UInt intNum;

    for (intNum = 0; intNum < Hwi_NUM_INTERRUPTS; intNum++) {
        if (Hwi_module->dispatchTable[intNum] == hwi) {
            break;
        }
    }

    Hwi_disableInterrupt(intNum);

    hwi->fxn = fxn;
    hwi->arg = params->arg;

    switch (params->maskSetting) {
        case Hwi_MaskingOption_NONE:
            hwi->disableMask = 0;
            hwi->restoreMask = 0;
            break;
        case Hwi_MaskingOption_ALL:
            hwi->disableMask = 0xffff;
            hwi->restoreMask = 0xffff;
            break;
        default:
        case Hwi_MaskingOption_SELF:
            hwi->disableMask = 1 << intNum;
            hwi->restoreMask = 1 << intNum;
            break;
        case Hwi_MaskingOption_BITMASK:
            hwi->disableMask = params->disableMask;
            hwi->restoreMask = params->restoreMask;
            break;
    }

    if (params->enableInt) {
        Hwi_enableInterrupt(intNum);
    }
}

/*
 *  ======== Hwi_getFunc ========
 */
Hwi_FuncPtr Hwi_getFunc(Hwi_Object *hwi, UArg *arg)
{
    *arg = hwi->arg;

    return (hwi->fxn);
}

/*
 *  ======== Hwi_setFunc ========
 */
Void Hwi_setFunc(Hwi_Object *hwi, Hwi_FuncPtr fxn, UArg arg)
{
    hwi->fxn = fxn;
    hwi->arg = arg;
}

/*
 *  ======== Hwi_getIrp ========
 */
Hwi_Irp Hwi_getIrp(Hwi_Object *hwi)
{
    return (hwi->irp);
}

/*
 *************************************************************************
 *                      OTHER FUNCTIONS
 *************************************************************************
 */


/*
 *  ======== isrHandler ========
 */
static Void isrHandler(UInt intr)
{

    /* ISR invoked with interrupts disabled */
    (Hwi_module->IST[intr])(intr);

    /* must always enable interrupts on isr return (b irp) */
    Hwi_enable();
}


/*
 *  ======== takeInterrupt ========
 *  Emulates the CPU taking an interrupt (i.e. fetching the execute
 *  packet in the interrupt vector table and executing it). Current
 *  thread must have ownership of emulation critical section before
 *  calling this function. The gate will be released in processInterrupt.
 */
static Void takeInterrupt()
{
    UInt intr;
    UInt mask;

    Hwi_disable();

    for (intr = MAX_BIT; intr != 0; intr--) {
        mask = 1 << intr;
        if ((mask & Hwi_module->IER) && (mask & Hwi_module->IFR)) {
           break;
        }
    }

    Assert_isTrue(intr != 0, Hwi_A_interNum);

    Hwi_module->IFR &= ~(mask);

    /* handle isr in new interrupt thread context */
    ti_sysbios_family_windows_TaskSupport_processInterrupt__I(
        (TaskSupport_IsrFxn)(isrHandler), intr);
}


/*
 *  ======== Hwi_dispatch ========
 */
Void Hwi_dispatch(UInt intNum)
{
    Hwi_Object *hwi;
    BIOS_ThreadType prevThreadType;
    UInt16 oldIER;
    Int tskKey;
    Int swiKey;
    Int i;

    if (Hwi_dispatcherTaskSupport) {
        tskKey = Hwi_taskDisable();    /* tskkey saved on Task Stack */
    }

    if (Hwi_dispatcherSwiSupport) {
        swiKey = Hwi_swiDisable();
    }

    /* set thread type to Hwi */
    prevThreadType = BIOS_setThreadType(BIOS_ThreadType_Hwi);

    hwi = Hwi_module->dispatchTable[intNum];

    /* call the begin hooks */
    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].beginFxn != NULL) {
            Hwi_hooks.elem[i].beginFxn((IHwi_Handle)hwi);
        }
    }

    Log_write4(Hwi_LM_begin, (IArg)hwi, (IArg)hwi->fxn, 
               (IArg)prevThreadType, (IArg)intNum);

    /* call the user's isr */
    if (Hwi_dispatcherAutoNestingSupport) {
        oldIER = Hwi_disableIER(hwi->disableMask);
        Hwi_enable();

        (hwi->fxn)(hwi->arg);

        Hwi_disable();
        Hwi_enableIER(hwi->restoreMask & oldIER);
    }
    else {
        (hwi->fxn)(hwi->arg);
    }

    Log_write1(Hwi_LD_end, (IArg)hwi);

    /* call the end hooks */
    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].endFxn != NULL) {
            Hwi_hooks.elem[i].endFxn((IHwi_Handle)hwi);
        }
    }

    /* Run Swi scheduler */
    if (Hwi_dispatcherSwiSupport) {
        Hwi_swiRestoreHwi(swiKey);
    }

    /* restore thread type */
    BIOS_setThreadType(prevThreadType);

    if (Hwi_dispatcherTaskSupport) {
        /* Run Task scheduler */
        Hwi_taskRestoreHwi(tskKey);  /* returns with ints disabled */
    }
}

/*
 */

/*
 *  @(#) ti.sysbios.family.windows; 2, 0, 0, 0,553; 5-18-2012 06:05:19; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

