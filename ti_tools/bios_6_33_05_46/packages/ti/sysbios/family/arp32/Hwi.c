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


#include <arp32.h>
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Log.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/interfaces/IHwi.h>
#include <ti/sysbios/BIOS.h>

#include "package/internal/Hwi.xdc.h"

extern Void ti_sysbios_family_arp32_Hwi_dispatchAlways(Int intnum);
extern Char *ti_sysbios_family_xxx_Hwi_switchToIsrStack();
extern Void ti_sysbios_family_xxx_Hwi_switchToTaskStack(Char *oldTaskSP);

#define Hwi_switchToIsrStack ti_sysbios_family_xxx_Hwi_switchToIsrStack
#define Hwi_switchToTaskStack ti_sysbios_family_xxx_Hwi_switchToTaskStack
#define Hwi_dispatchAlways ti_sysbios_family_arp32_Hwi_dispatchAlways

extern char *__bss__;

static Int postInit(Hwi_Object *hwi, Error_Block *eb);

extern cregister volatile unsigned int IRP;
extern cregister volatile unsigned int IER;
extern cregister volatile unsigned int ISR;
extern cregister volatile unsigned int ICR;

#ifdef ti_sysbios_family_arp32_Hwi_dispatcherTaskSupport__D
/* disable unused local variable warning during optimized compile */
#pragma diag_suppress=179
#define TASK_DISABLE Task_disable
#define TASK_RESTORE Task_restoreHwi
#else
#define TASK_DISABLE Hwi_taskDisable
#define TASK_RESTORE Hwi_taskRestoreHwi
#endif

#ifdef ti_sysbios_family_arp32_Hwi_dispatcherSwiSupport__D
/* disable unused local variable warning during optimized compile */
#pragma diag_suppress=179
#define SWI_DISABLE Swi_disable
#define SWI_RESTORE Swi_restoreHwi
#else
#define SWI_DISABLE Hwi_swiDisable
#define SWI_RESTORE Hwi_swiRestoreHwi
#endif

/*
 *  ======== Hwi_Module_startup ========
 */
Int Hwi_Module_startup(Int phase)
{
    int i;
    Hwi_Object *hwi;
    volatile Hwi_IntcRegs *intc = (volatile Hwi_IntcRegs *)Hwi_INTCREGSBASEADDR;

    /* Task and Swi APIs used not called until BIOS_start() */
    /* okay to proceed with initialization */

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].registerFxn != NULL) {
            Hwi_hooks.elem[i].registerFxn(i);
        }
    }
#endif

    /* 
     * Initialize the pointer to the isrStack. These symbols are part of the
     * Hwi_module (instead of floating) in order to support ROM.
     */
    Hwi_module->isrStack = Hwi_getIsrStackAddress();

    /* signal that we're executing on the ISR stack */
    Hwi_module->taskSP = (Char *)-1;

    /* start with a clean slate, clear the IFR */
    ICR = 0xffff;

    /* clear all INT4-INT7 (index 1 - 4) registers */
    for (i = 1; i < 5; i++) {
        /* clear all status bits */
        intc[i].status = 0xffffffff;
        
        /* clear all enabled bits */
        intc[i].clr = 0xffffffff;
    }
    
    Hwi_enableIER(0x3);     // enable RESET and NMI
    
    for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
        /* run postInit for statically created instances */
        hwi = Hwi_module->dispatchTable[i];
        if (hwi != NULL) {
            postInit(hwi, NULL);
            
            /* enable specified static interrupts */
            if (Hwi_module->interruptMask & (1 << i)) {
                Hwi_enableInterrupt(i);
            }
        }
    }

    return (Startup_DONE);
}

/*
 *  ======== Hwi_Instance_init ========
 */
Int Hwi_Instance_init(Hwi_Object *hwi, Int intNum, 
                      Hwi_FuncPtr fxn, const Hwi_Params *params, 
                      Error_Block *eb)
{
    Int status;

    if (Hwi_module->dispatchTable[intNum] != NULL) {
        Error_raise(eb, Hwi_E_alreadyDefined, intNum, 0);
        return (1);
    }

    Hwi_module->dispatchTable[intNum] = hwi;
    Hwi_module->disableIerMask |= hwi->disableIerMask;
    Hwi_module->restoreIerMask |= hwi->restoreIerMask;
    Hwi_reconfig(hwi, fxn, params);

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    if (Hwi_hooks.length > 0) {
        /* Allocate environment space for each hook instance. */
        hwi->hookEnv = Memory_calloc(Hwi_Object_heap(),
                       Hwi_hooks.length * sizeof(Ptr), 0, eb);

        if (hwi->hookEnv == NULL) {
            return (1);
        }
    }
#endif

    Hwi_module->irp[hwi->vectorNum] = 0;

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

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    for (i = 0; i < Hwi_hooks.length; i++) {
        hwi->hookEnv[i] = (Ptr)0;
        if (Hwi_hooks.elem[i].createFxn != NULL) {
            Hwi_hooks.elem[i].createFxn((IHwi_Handle)hwi, eb);

            if (Error_check(eb)) {
                return (i);
            }
        }
    }
#endif

    return (0);
}

/*
 *  ======== Hwi_Instance_finalize ========
 *  Here on Hwi_delete() or failed Hwi_create().
 */
Void Hwi_Instance_finalize(Hwi_Object *hwi, Int status)
{
    Int i, cnt;
    UInt intNum;

    for (intNum = 0; intNum < Hwi_NUM_INTERRUPTS; intNum++) {
        if (Hwi_module->dispatchTable[intNum] == hwi) {
            break;
        }
    }

    Hwi_disableInterrupt(intNum);
    Hwi_module->dispatchTable[intNum] = NULL;

    if (status == 1) {  /* failed Hwi_create */
        return;
    }

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
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
#endif
}

/*
 *  ======== Hwi_selfLoop ========
 *  Unplugged vectors are plugged with this by default
 */
Void Hwi_selfLoop(UArg arg) {
    _disable_interrupts();
    while(TRUE);
}

/*
 *  ======== Hwi_unPluggedInterrupt ========
 *  Here on interrupt unPlugged by Hwi_delete().
 */
Void Hwi_unPluggedInterrupt() {
    while(TRUE);
}

/*
 *  ======== Hwi_nmiInterrupt ========
 */
Void Hwi_nmiInterrupt() {
    while(TRUE);
}

/*
 *  ======== Hwi_swiInterrupt ========
 */
Void Hwi_swiInterrupt() {
    while(TRUE);
}

/*
 *  ======== Hwi_undefInterrupt ========
 */
Void Hwi_undefInterrupt() {
    while(TRUE);
}

/*
 *  ======== Hwi_startup ========
 */
Void Hwi_startup()
{
    Hwi_enable();
}

/*
 * Below APIs provided for backward compatability.
 */

#ifdef __ti__
#pragma CODE_SECTION(ti_sysbios_family_arp32_Hwi_disable__E, ".text:ti_sysbios_family_arp32_Hwi_disable__E")
#pragma CODE_SECTION(ti_sysbios_family_arp32_Hwi_enable__E, ".text:ti_sysbios_family_arp32_Hwi_enable__E")
#pragma CODE_SECTION(ti_sysbios_family_arp32_Hwi_restore__E, ".text:ti_sysbios_family_arp32_Hwi_restore__E")
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arp32_Hwi_disable__E);
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arp32_Hwi_enable__E);
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arp32_Hwi_restore__E);
#endif

/*
 *  ======== Hwi_disable ========
 */
UInt ti_sysbios_family_arp32_Hwi_disable__E() 
{
    return _disable_interrupts();
}
 
/*
 *  ======== Hwi_enable ========
 */
UInt ti_sysbios_family_arp32_Hwi_enable__E()
{
    return _enable_interrupts();
}

/*
 *  ======== Hwi_restore ========
 */
Void ti_sysbios_family_arp32_Hwi_restore__E(UInt key)
{
    _restore_interrupts(key);
}

/*
 *  ======== Hwi_disableIER ========
 */
Bits16 Hwi_disableIER(Bits16 mask)
{
    UInt key;
    Bits16 oldIER;

    key = Hwi_disable();
    oldIER = IER;
    IER &= ~mask;
    Hwi_restore(key);

    return (oldIER);
}

/*
 *  ======== Hwi_enableIER ========
 */
Bits16 Hwi_enableIER(Bits16 mask)
{
    UInt key;
    Bits16 oldIER;

    key = Hwi_disable();
    oldIER = IER;
    IER |= mask;
    Hwi_restore(key);

    return (oldIER);
}

/*
 *  ======== Hwi_restoreIER ========
 */
Bits16 Hwi_restoreIER(Bits16 mask)
{
    UInt key;
    Bits16 oldIER;

    key = Hwi_disable();
    oldIER = IER;
    IER = mask;
    Hwi_restore(key);

    return (oldIER);
}

/*
 *  ======== Hwi_disableInterrupt ========
 */
UInt Hwi_disableInterrupt(UInt intNum)
{
    UInt key;
    UInt oldIntc;
    Hwi_Handle hwi = Hwi_module->dispatchTable[intNum];
    volatile Hwi_IntcRegs *intc = (volatile Hwi_IntcRegs *)Hwi_INTCREGSBASEADDR;

    /* disable interrupts */
    key = Hwi_disable();

    /* get the current enabled interrupts */
    oldIntc = intc[hwi->index].set;

    /* clear the specified interrupt */
    intc[hwi->index].clr = 1 << intNum;

    /* if last disabled interrupt in group, disable IER */
    if ((oldIntc == 0) || ((oldIntc & ~(1 << intNum)) == 0)) {
        Hwi_disableIER(1 << hwi->vectorNum);
    }

    /* restore interrupts */
    Hwi_restore(key);

    /* return the old enabled interrupts */
    return (oldIntc);
}

/*
 *  ======== Hwi_enableInterrupt ========
 */
UInt Hwi_enableInterrupt(UInt intNum)
{
    UInt key;
    UInt oldIntc;
    Hwi_Handle hwi = Hwi_module->dispatchTable[intNum];
    volatile Hwi_IntcRegs *intc = (volatile Hwi_IntcRegs *)Hwi_INTCREGSBASEADDR;

    /* enable the IER */
    Hwi_enableIER(1 << hwi->vectorNum);

    /* disable interrupts */
    key = Hwi_disable();

    /* get the current enabled interrupts */
    oldIntc = intc[hwi->index].set;

    /* set the specified interrupt */
    intc[hwi->index].set = (1 << intNum);

    /* restore interrupts */
    Hwi_restore(key);

    /* return the old enabled interrupts */
    return (oldIntc);
}

/*
 *  ======== Hwi_RestoreInterrupt ========
 */
Void Hwi_restoreInterrupt(UInt intNum, UInt key)
{
    if (key & (1 << (intNum))) {
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
    Hwi_Handle hwi = Hwi_module->dispatchTable[intNum];
    volatile Hwi_IntcRegs *intc = (volatile Hwi_IntcRegs *)Hwi_INTCREGSBASEADDR;

    /* clear the status register */
    intc[hwi->index].status = 1 << intNum;

    /* clear the IFR */
    ICR = 1 << hwi->vectorNum;
}

/*
 *  ======== Hwi_getHandle ========
 */
Hwi_Handle Hwi_getHandle(UInt intNum)
{
    return (Hwi_module->dispatchTable[intNum]);
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
Void Hwi_setFunc(Hwi_Object *hwi, Void (*fxn)(), UArg arg)
{
    hwi->fxn = fxn;
    hwi->arg = arg;
}

/*
 *  ======== Hwi_getIrp ========
 */
Hwi_Irp Hwi_getIrp(Hwi_Object *hwi)
{
    return (Hwi_module->irp[hwi->vectorNum]);
}

/*
 *  ======== Hwi_post ========
 */
Void Hwi_post(UInt intNum)
{       
    Hwi_Handle hwi = Hwi_module->dispatchTable[intNum];
    volatile Hwi_IntcRegs *intc = (volatile Hwi_IntcRegs *)Hwi_INTCREGSBASEADDR;
    
    /* set the status register */
    intc[hwi->index].status = 1 << intNum;
    
    /* set the ISR bit corresponding to the vector */
    ISR = (1 << hwi->vectorNum);
    asm("    nop    ");
    asm("    nop    ");
    asm("    nop    ");
    asm("    nop    ");
}

/*
 *  ======== Hwi_getTaskSP ========
 */
Char *Hwi_getTaskSP()
{
    return (Hwi_module->taskSP);
}

/*
 *  ======== Hwi_reconfig ========
 *  Reconfigure a dispatched interrupt.
 */
Void Hwi_reconfig(Hwi_Object *hwi, Hwi_FuncPtr fxn, const Hwi_Params *params)
{
    UInt intNum;

    /* determine the intNum */
    for (intNum = 0; intNum < Hwi_NUM_INTERRUPTS; intNum++) {
        if (Hwi_module->dispatchTable[intNum] == hwi) {
            break;
        }
    }

    /* disable the interrupt */
    Hwi_disableInterrupt(intNum);

    /* set the instance fields */
    hwi->fxn = fxn;
    hwi->arg = params->arg;
    hwi->vectorNum = params->vectorNum;
    
    /*
     *  Compute the index for accessing the Interrupt controller registers.
     *  Since INT4 = 1, INT5 = 2, INT6 = 3, and INT7 = 4 respectively,
     *  subtract 3 from the vectorNum to get the index.
     */
    hwi->index = hwi->vectorNum - 3;

    switch (params->maskSetting) {
        case Hwi_MaskingOption_NONE:
            hwi->disableIerMask = 0;
            hwi->restoreIerMask = 0;
            break;
        case Hwi_MaskingOption_ALL:
            hwi->disableIerMask = 0xf0;
            hwi->restoreIerMask = 0xf0;
            break;
        default:
        case Hwi_MaskingOption_SELF:
            hwi->disableIerMask = 1 << params->vectorNum;
            hwi->restoreIerMask = 1 << params->vectorNum;
            break;
        case Hwi_MaskingOption_BITMASK:
            hwi->disableIerMask = params->disableIerMask;
            hwi->restoreIerMask = params->restoreIerMask;
            break;
    }

    if (params->enableInt) {
        Hwi_enableInterrupt(intNum);
    }
}

/*
 *  ======== switchFromBootStack ========
 *  Indicate that we are leaving the boot stack and
 *  are about to switch to a task stack.
 */
Void Hwi_switchFromBootStack()
{
    Hwi_module->taskSP = 0;
}

/*
 *  ======== Hwi_getIsrStackAddress ========
 */
Char *Hwi_getIsrStackAddress()
{
    __extern __FAR__ char _stack[32];
    __extern __FAR__ UInt8 __TI_STACK_SIZE;
    UInt32 isrStack;

    isrStack = (UInt32)_stack + (UInt32)&__TI_STACK_SIZE - 16;

    isrStack &= ~0x7;   /* make sure stack is aligned to long word boundary */

    return ((Char *)isrStack);
}

/*
 *  ======== Hwi_getStackInfo ========
 *  Used to get Hwi stack usage info.
 */
Bool Hwi_getStackInfo(Hwi_StackInfo *stkInfo, Bool computeStackDepth)
{
    __extern __FAR__ char _stack[32];
    __extern __FAR__ UInt8 __TI_STACK_SIZE;

    Char *isrSP;
    Bool stackOverflow;

    /* Copy the stack base address and size */
    stkInfo->hwiStackSize = (SizeT)&__TI_STACK_SIZE;
    stkInfo->hwiStackBase = (Ptr)_stack;

    isrSP = stkInfo->hwiStackBase;

    /* Check for stack overflow */
    stackOverflow = (*isrSP != (Char)0xbe ? TRUE : FALSE);

    if (computeStackDepth && !stackOverflow) {
        /* Compute stack depth */
        do {
        } while(*isrSP++ == (Char)0xbe);
        stkInfo->hwiStackPeak = stkInfo->hwiStackSize - (SizeT)(--isrSP - (Char *)stkInfo->hwiStackBase);
    }
    else {
        stkInfo->hwiStackPeak = 0;
    }

    return stackOverflow;
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
 *  ======== Hwi_intcDispatch ========
 *  Function that gets called from Hwi_dispatchC
 */
Void Hwi_intcDispatch(Int vectorNum)
{
    Hwi_Object *hwi;
    Int i;
    Hwi_FuncPtr fxn;
    UArg arg;
    UInt32 status;
    volatile Hwi_IntcRegs *intc = (volatile Hwi_IntcRegs *)Hwi_INTCREGSBASEADDR;

    status = intc[vectorNum - 3].status;
    while (status) {
        intc[vectorNum - 3].status = status;
        for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
            if (status & (1 << i)) {
                status &= ~(1 << i);
                
                /* 
                 * pre-read local copies of the variables used 
                 * within to eliminate memory fetch nops
                 */
                hwi = Hwi_module->dispatchTable[i];
                fxn = hwi->fxn;
                arg = hwi->arg;

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
                /* call the begin hooks */
                for (i = 0; i < Hwi_hooks.length; i++) {
                    if (Hwi_hooks.elem[i].beginFxn != NULL) {
                        Hwi_hooks.elem[i].beginFxn((IHwi_Handle)hwi);
                    }
                }
#endif

                Log_write4(Hwi_LM_begin, (IArg)hwi, (IArg)fxn, 
                          (IArg)vectorNum, Hwi_module->irp[vectorNum]);
               
                (fxn)(arg);

                Log_write1(Hwi_LD_end, (IArg)hwi);

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
                /* call the end hooks */
                for (i = 0; i < Hwi_hooks.length; i++) {
                    if (Hwi_hooks.elem[i].endFxn != NULL) {
                        Hwi_hooks.elem[i].endFxn((IHwi_Handle)hwi);
                    }
                }
#endif
            }
        }
        
        /* do final read of status register */
        status = intc[vectorNum - 3].status;
    }
}
 
/*
 *  ======== Hwi_dispatchCore ========
 *  The "FUNC_CANNOT_INLINE" pragma below is really important.  This keeps
 *  this function from being inlined into Hwi_dispatchC which exposes a
 *  problem with local variables being stored on one stack and referenced on
 *  the other (isr stack and task stack).
 */
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arp32_Hwi_dispatchCore);
#pragma FUNC_CANNOT_INLINE(ti_sysbios_family_arp32_Hwi_dispatchCore);
Void Hwi_dispatchCore()
{
    Int vectorNum;
    BIOS_ThreadType prevThreadType;
    UInt16 oldIER, disableIerMask, restoreIerMask;
    Int swiKey;

    /* 
     * all references to local variables beyond this point
     * will be on the isr stack
     */
    vectorNum = Hwi_module->vectNum;

    if (Hwi_dispatcherIrpTrackingSupport) {
        Hwi_module->irp[vectorNum] = IRP;
    }

    if (Hwi_dispatcherAutoNestingSupport) {
        disableIerMask = Hwi_module->disableIerMask;
        restoreIerMask = Hwi_module->restoreIerMask;
    }

    if (Hwi_dispatcherSwiSupport) {
        swiKey = SWI_DISABLE();
    }
        
    /* set thread type to Hwi */
    prevThreadType = BIOS_setThreadType(BIOS_ThreadType_Hwi);

    /* call the user's isr */
    if (Hwi_dispatcherAutoNestingSupport) {
        oldIER = IER;
        IER &= ~disableIerMask;
        Hwi_enable();

        Hwi_intcDispatch(vectorNum);

        Hwi_disable();
        IER |= (restoreIerMask & oldIER);
    }
    else {
        Hwi_intcDispatch(vectorNum);
    }

    /* Run Swi scheduler */
    if (Hwi_dispatcherSwiSupport) {
        SWI_RESTORE(swiKey);
    }

    /* restore thread type */
    BIOS_setThreadType(prevThreadType);
}

/*
 *  ======== Hwi_dispatchC ========
 *  Configurable dispatcher.
 */
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arp32_Hwi_dispatchC);
Void Hwi_dispatchC(register Int vectorNum)
{
    Int tskKey;
    Char *oldTaskSP;

    /* save away vectNum in module state because it might be saved on stack */
    Hwi_module->vectNum = vectorNum;

    if (Hwi_dispatcherTaskSupport) {
        tskKey = TASK_DISABLE();
    }

    /* 
     * Switch to Hwi stack if not already on it.
     * This step, and the corresponding switch back to the task
     * stack are performed outside the "if (Hwi_dispatcherTaskSupport)"
     * conditionals because sometimes the generated code placed a copy
     * of Hwi_dispatcherTaskSupport on the task stack for use below.
     */
    oldTaskSP = Hwi_switchToIsrStack();

    Hwi_dispatchCore();

    Hwi_switchToTaskStack(oldTaskSP);

    /* Run Task scheduler */
    if (Hwi_dispatcherTaskSupport) {
        /* tskKey fetched from task stack if this is a non-nested interrupt */
        TASK_RESTORE(tskKey);   /* returns with ints disabled */
    }
}

/*
 */
/*
 *  @(#) ti.sysbios.family.arp32; 2, 0, 0, 0,79; 5-18-2012 06:04:37; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

