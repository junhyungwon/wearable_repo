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
 *  ======== TaskSupport.xdc ========
 *
 *
 */

package ti.sysbios.family.windows;

import xdc.runtime.Error;
import xdc.runtime.Types;


/*!
 *  ======== TaskSupport ========
 *  Windows Task Support Module.
 */

@ModuleStartup  /* generate a call to Task_Module_startup at startup */


module TaskSupport inherits ti.sysbios.interfaces.ITaskSupport
{
    // -------- Module Constants --------


    // -------- Module Types --------

    //! Prototype of task enter and exit functions.
    typedef Void (*GlueFxn)();

    //! Task function type definition.
    typedef Void (*TaskFxn)(UArg, UArg);

    //! Isr function type definition.
    typedef Void (*IsrFxn)(UInt);

    //! Emulation task object.
    struct Task;

    /*! Error: memory allocation request failed.
     *
     *  A request into the Windows runtime library for memory has failed.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #start}<br />
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
     *  {@link #start}<br />
     *  {@link #startup}<br />
     *  @p
     */
    config Error.Id E_event = {
        msg: "CreateEvent failed"
    };

    /*! Error: invalid incoming task state.
     *
     *  A task swap was attempted where the incoming task is in an
     *  invalid state.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #swap}<br />
     *  @p
     */
    config Error.Id E_taskState = {
        msg: "invalid incoming task state"
    };

    /*! Error: CreateThread failed.
     *
     *  A CreateThread call into the Win32 API has failed.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #processInterrupt}<br />
     *  {@link #start}<br />
     *  @p
     */
    config Error.Id E_thread = {
        msg: "CreateThread failed"
    };

    /*! Error: WaitForSingleObject failed.
     *
     *  A WaitForSingleObject call into the Win32 API has failed.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #processInterrupt}<br />
     *  {@link #start}<br />
     *  @p
     */
    config Error.Id E_wait = {
        msg: "WaitForSingleObject failed"
    };


    // -------- Module Proxys --------

    // -------- Module Parameters --------

    //! Default task stack size.
    override readonly config SizeT defaultStackSize = 128;

    //! Required stack alignment (in MAUs).
    override readonly config UInt stackAlignment = 0;

    // -------- Module Functions --------


internal:   /* not for client use */

    /*
     *  ======== enterCS ========
     *  Acquire ownership of the emulation gate.
     */
    Void enterCS();

    /*
     *  ======== leaveCS ========
     *  Release ownership of the emulation gate.
     */
    Void leaveCS();

    /*
     *  ======== processInterrupt ========
     *  Handle the necessary thread switching and call the interrupt ISR.
     */
    Void processInterrupt(IsrFxn fxn, UInt intr);


    enum State {
        State_PREEMPTED,
        State_RUNNING,
        State_TERMINATING,
        State_YIELDED
    };

    struct PrivateData;

    struct TaskData;

    struct Module_State {
        Task            *currentTask;
        PrivateData     *privateData;   /* Windows types not allowed in xdc */
    };

}
/*
 *  @(#) ti.sysbios.family.windows; 2, 0, 0, 0,553; 5-18-2012 06:05:19; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

