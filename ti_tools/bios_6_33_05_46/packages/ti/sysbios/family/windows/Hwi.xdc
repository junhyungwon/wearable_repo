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
 *  ======== Hwi.xdc ========
 *
 */


package ti.sysbios.family.windows;

import xdc.runtime.Assert;
import xdc.runtime.Diags;
import xdc.runtime.Error;
import xdc.runtime.IHeap;
import xdc.runtime.Log;

/*!
 *  ======== Hwi ========
 *  Windows emulation Hardware Interrupt Support Module
 *
 *  This Hwi module provides Windows emulation specific implementations of 
 *  the APIs defined in {@link ti.sysbios.interfaces.IHwi IHwi}.
 *
 *  Additional Windows emulation specific APIs are also provided.
 *
 *  @a(NOTE)
 *  In this Hwi module implementation, the instance config parameter value
 *  {@link #MaskingOption_LOWER} is equivalent to {@link #MaskingOption_SELF}.
 *  Statically configuring a Hwi object's {@link #Params.maskSetting} to 
 *  {@link #MaskingOption_LOWER} will result in the generation of a benign
 *  build warning. Dynamic usages of {@link #MaskingOption_LOWER} will be
 *  silently converted to {@link #MaskingOption_SELF}.
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center"></colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th><th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                                                                                 -->
 *    <tr><td> {@link #clearInterrupt}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #create}           </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #disable}          </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #disableIER}       </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #disableInterrupt} </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enable}           </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableIER}        </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableInterrupt}  </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getHandle}        </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #Params_init}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #plug}             </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #restore}          </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #restoreIER}       </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #restoreInterrupt} </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #construct}        </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #delete}           </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #destruct}         </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getHookContext}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #reconfig}         </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setFunc}          </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setHookContext}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started (e.g. Hwi_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup before this module is started (e.g. Hwi_Module_startupDone() returns FALSE).</li>
 *           </ul>
 *       </ul>
 *    </td></tr>
 *  </table>
 *  @p
 *
 */

@Template("./Hwi.xdt")  /* generates the dispatcher */
@ModuleStartup          /* plugs signals statically plugged using Hwi_plug  */

module Hwi inherits ti.sysbios.interfaces.IHwi
{
    // -------- Module Constants --------

    //! Dispatcher supports 16 interrupts.
    const Int NUM_INTERRUPTS = 16;

    // -------- Module Types --------

    //! @_nodoc Hwi plug function type definition.
    typedef Void (*PlugFuncPtr)(UInt);

    //! Isr function type definition.
    typedef Void (*IsrFxn)(UInt);

    /*! Assert: invalid interrupt number.
     *
     *  Interrupt number must be greater than or equal to 0 and less
     *  than {@link #NUM_INTERRUPTS}.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #disableInterrupt}<br />
     *  {@link #enableInterrupt}<br />
     *  {@link #plug}<br />
     *  {@link ti.sysbios.interfaces.IHwi#post}<br />
     *
     *  @a(See_Also)
     *  @p(html)
     *  {@link xdc.runtime.Assert}<br />
     *  {@link xdc.runtime.Diags}<br />
     *  @p
     */
    config Assert.Id A_interNum = {
        msg: "A_interNum: interrupt number must be less then NUM_INTERRUPTS"
    };

    /*! Assert: there must be a pending interrupt
     *
     *  The Hwi module decided to take an interrupt, but upon inspecting
     *  the IER and IFR registers did not find an enabled pending interrupt.
     */
    config Assert.Id A_noPendingIntr = {
        msg: "A_noPendingIntr: there must be a pending interrupt"
    };

    /*! Assert: an unplugged interrupt was raised.
     *
     *  An interrupt was raised which does not have an ISR plugged into
     *  the Interrupt Service Table (IST).
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #post}<br />
     *  @p
     */
    config Assert.Id A_pluggedIntr = {
        msg: "interrupt must be plugged"
    };

    /*! Error: memory allocation request failed.
     *
     *  A request into the Windows runtime library for memory has failed.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #startup}<br />
     *  @p
     */
    config Error.Id E_calloc = {
        msg: "calloc failed"
    };

    /*! Error: CreateEvent failed.
     *
     *  A CreateEvent call into the Win32 API has failed.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #startup}<br />
     *  @p
     */
    config Error.Id E_event = {
        msg: "CreateEvent failed"
    };

    /*!
     *  Error raised when Hwi is already defined
     */
    config Error.Id E_alreadyDefined = {
        msg: "E_alreadyDefined: Hwi already defined: intr# %d"
    };

    /*!
     *  Issued just prior to Hwi function invocation (with interrupts disabled)
     */
    config Log.Event LM_begin = {
        mask: Diags.USER1 | Diags.USER2,
        msg: "LM_begin: hwi: 0x%x, func: 0x%x, preThread: %d, intNum: %d"
    };

    /*!
     *  Issued just after return from Hwi function (with interrupts disabled)
     */
    config Log.Event LD_end = {
        mask: Diags.USER2,
        msg: "LD_end: hwi: 0x%x"
    };

    /*! Log: missed interrupt detected
     *
     *  An interrupt is being raised which has a previous instance
     *  recorded in the IFR register still waiting to be serviced.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #post}<br />
     *  @p
     */
    config Log.Event LW_missedIntr = {
        mask: Diags.USER3,
        msg: "Warning: LW_missedIntr: missed interrupt detected: %d"
    };

    // -------- Module Parameters --------

    /*! Non-dispatched interrupt object.
     *
     *  Provided so that XGCONF users can easily plug non-dispatched
     *  interrupts.
     */
    metaonly struct NonDispatchedInterrupt {
        Int             intNum;         //! interrupt number
        PlugFuncPtr     fxn;            //! plugged ISR function
        Bool            enableInt;      //! interrupt enable flag
        Int             eventId;        //! source event id
    };

    /*! Non-dispatched interrupt array.
     *
     *  Provided so that XGCONF users can easily plug non-dispatched
     *  interrupts.
     */
    metaonly config NonDispatchedInterrupt nonDispatchedInterrupts[string];

    // -------- Module Functions --------

    /*!
     *  ======== disableIER ========
     *  Disable certain maskable interrupts.
     *
     *  Atomically disables specific interrupts by clearing the bits 
     *  specified by mask in the Interrupt Enable Register (IER).
     *
     *  The IER bits to be cleared should be set to 1 in the mask.
     *
     *  @param(mask)    Bitmask of interrupts to disable.
     *  @b(returns)     Previous IER settings bitmask.
     */
    Bits16 disableIER(Bits16 mask);

    /*!
     *  ======== enableIER ========
     *  Enable certain maskable interrupts.
     *
     *  Atomically enables specific interrupts by setting the bits 
     *  specified by mask in the Interrupt Enable Register (IER).
     *
     *  The IER bits to be set should be set to 1 in the mask.
     *
     *  @param(mask)    Bitmask of interrupts to enable.
     *  @b(returns)     Previous IER settings bitmask.
     */
    Bits16 enableIER(Bits16 mask);

    /*!
     *  ======== restoreIER ========
     *  Restore maskable interrupts to the state they were in 
     *  when either disableIER() or enableIER() was called.
     *
     *  Atomically writes the given mask to the IER register. Typically used
     *  to restore the IER register to the state returned from a call to
     *  either {@link #disableIER()} or {@link #enableIER()}.
     *
     *  @param(mask)    Bitmask of interrupts to restore.
     *  @b(returns)     Previous IER settings bitmask.
     */
    Bits16 restoreIER(Bits16 mask);

    /*!
     *  ======== getHandle ========
     *  Return a pointer to an Hwi instance object.
     *
     *  @param(intNum)  Interrupt number.
     */
    Handle getHandle(UInt intNum);

    /*!
     *  @_nodoc
     *  ======== inUseMeta ========
     *  Check for Hwi already in use.
     *
     *  For internal SYS/BIOS use only. Should be called prior
     *  to any internal Hwi.create().
     *
     *  @param(intNum)  Interrupt number.
     */
    metaonly Bool inUseMeta(UInt intNum);

    /*!
     *  @_nodoc
     *  ======== plug ========
     *  Plug an interrupt vector with an ISR address at runtime.
     *
     *  Writes the ISR address into the Interrupt Service Table (IST)
     *  at the address corresponding to the `intNum`. This function
     *  does not enable the interrupt. Use {@link #enableIER()} to
     *  enable the interrupt.
     *
     *  @param(intNum) Interrupt number. The interrupt number must be
     *      in the range 0 - ({@link #NUM_INTERRUPTS} - 1).
     *  @param(fxn) Pointer to ISR function. The ISR function
     *      pointer cannot be NULL.
     *
     *  @a(Asserts)
     *  @p(html)
     *  {@link #A_interNum}<br />
     *  @p
     *
     *  @a(See_Also)
     *  {@link #enableIER()}
     */
    Void plug(UInt intNum, PlugFuncPtr fxn);

    /*!
     *  ======== plugMeta ========
     *  Plug an interrupt vector with an ISR address at config time.
     *
     *  @param(intNum) Interrupt number. The interrupt number must be
     *      in the range 0 - ({@link #NUM_INTERRUPTS} - 1).
     *  @param(fxn) Pointer to ISR function. The ISR function
     *      pointer cannot be NULL.
     */
    metaonly Void plugMeta(UInt intNum, PlugFuncPtr fxn);

instance:

    /*! 
     *  Dispatcher auto-nesting interrupt disable mask.
     * 
     *  When the dispatcher's auto interrupt nesting support feature 
     *  is enabled (see {@link #dispatcherAutoNestingSupport}), 
     *  this mask defines which IER bits are disabled prior to invoking
     *  the user's ISR function with GIE = 1.
     *  
     *  disableMask bits set to 1 correspond to IER bits that will be cleared
     *  prior to invoking the ISR.
     *
     *  The value of this mask is normally auto-calculated based on the
     *  value of the maskSetting. However, manual setting of this
     *  mask is enabled by setting the maskSetting to 
     *  {@link #MaskingOption MaskingOption_BITMASK}.
     *
     *  The default value is derived from the 
     *  {@link #MaskingOption MaskingOption_SELF}
     *  maskSetting.
     */
    config Bits16 disableMask = 0;

    /*! 
     *  Dispatcher auto-nesting interrupt restore mask.
     * 
     *  When the dispatcher's auto interrupt nesting support feature 
     *  is enabled (see {@link #dispatcherAutoNestingSupport}), 
     *  this mask defines which IER bits are restored to their previous
     *  setting upon return from the user's ISR function.
     *  
     *  restoreMask bits set to 1 correspond to IER bits that will be restored.
     *  
     *  The value of this mask is normally auto-calculated based on the
     *  value of the maskSetting. However, manual setting of this
     *  mask is enabled by setting the maskSetting to 
     *  {@link #MaskingOption MaskingOption_BITMASK}.
     *
     *  The default value is derived from the 
     *  {@link #MaskingOption MaskingOption_SELF}
     *  maskSetting.
     */
    config Bits16 restoreMask = 0;

    /*!
     *  Interrupt priority. Not supported on this target.
     */
    override config Int priority = 0;

    /*!
     *  ======== reconfig ========
     *  Reconfigure a dispatched interrupt.
     */
    Void reconfig(FuncPtr fxn, const Params *params);

internal:   /* not for client use */

    /* 
     * Swi and Task module function pointers. 
     * Used to decouple Hwi from Swi and Task when 
     * dispatcherSwiSupport or
     * dispatcherTaskSupport is false.
     */
    config UInt (*swiDisable)();
    config Void (*swiRestoreHwi)(UInt);
    config UInt (*taskDisable)();
    config Void (*taskRestoreHwi)(UInt);

    /*
     *  ======== dispatch ========
     *  Interrupt Dispatcher
     */
    Void dispatch(UInt intNum);

    /*
     *  ======== init ========
     */
    Void init();

    /*
     *  ======== unPluggedInterrupt ========
     *  unPlugged interrupt handler
     */
    Void unPluggedInterrupt(UInt num);

    /* const array to hold all HookSet objects */
    config HookSet hooks[length] = [];

    /*! Meta World Only Hwi Configuration Object. */
    metaonly struct InterruptObj {
        String name;            /* symbol used for vector table entry */
        Bool        used;           /* Interrupt already defined? */
        Bool        useDispatcher;  /* Should dispatcher handle this intr?*/
        FuncPtr     fxn;            /* Dispatched ISR function. */
        PlugFuncPtr pfxn;           /* "Hwi_plug'd" ISR function. */
    };

    /*!
     *  Meta-only array of interrupt objects.
     *  This meta-only array of Hwi config objects is initialized
     *  in Hwi.xs:module$meta$init().
     */
    metaonly config InterruptObj interrupt[NUM_INTERRUPTS];

    struct PrivateData;

    struct Instance_State {
        Bits16          disableMask;    /* Interrupts to mask during ISR.   */
        Bits16          restoreMask;    /* Interrupts to restore after ISR. */
        UArg            arg;            /* Argument to Hwi function.        */
        FuncPtr         fxn;            /* Hwi function.                    */
        Irp             irp;            /* current IRP                      */
        Ptr             hookEnv[];
    };

    struct Module_State {
        Bits16          ierMask;        /* Initial IER mask                 */
        Handle          dispatchTable[NUM_INTERRUPTS]; /* dispatch table   */
        Bool            GIE;
        Bits16          IER;
        Bits16          IFR;
        UInt            intrNum;
        IsrFxn          IST[NUM_INTERRUPTS];
        PrivateData     *privateData;   /* Windows types not allowed in xdc */
    };
}
/*
 *  @(#) ti.sysbios.family.windows; 2, 0, 0, 0,553; 5-18-2012 06:05:19; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

