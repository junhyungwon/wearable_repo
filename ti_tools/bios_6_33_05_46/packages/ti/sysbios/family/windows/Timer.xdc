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
 *  ======== Timer.xdc ========
 *
 *
 */

package ti.sysbios.family.windows;

import xdc.runtime.Types;
import xdc.runtime.Error;
import ti.sysbios.interfaces.ITimer;

/*!
 *  ======== Timer ========
 *  Timer Peripherals Manager for Windows family.
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center"></colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th><th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                                                                                 -->
 *    <tr><td> {@link #getNumTimers}            </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getStatus}               </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #Params_init}             </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #construct}               </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #create}                  </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #delete}                  </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #destruct}                </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getFreq}                 </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getPeriod}               </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setPeriod}               </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setPeriodMicroSecs}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #start}                   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #stop}                    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started (e.g. Timer_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup before this module is started (e.g. Timer_Module_startupDone() returns FALSE).</li>
 *           </ul>
 *       </ul>
 *    </td></tr>
 *
 *  </table>
 *  @p
 *
 */

@ModuleStartup
@InstanceFinalize       /* To cleanup */
@InstanceInitError      /* To report unavailability of timer */

module Timer inherits ti.sysbios.interfaces.ITimer
{
    // -------- Module Constants --------

    /*! Emulation supports 2 timers. */
    const Int NUM_TIMER_DEVICES = 2;

    /*! Max value of Timer period for PeriodType_COUNTS*/
    const UInt MAX_PERIOD = 0xffffffff;

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

    /*! Error: unsupported timer period
     *
     *  On Windows, the timer period cannot be less than one millisecond.
     *  However, the actual minimum timer period (even when set to one
     *  millisecond) is 10 - 15 milliseconds. This latency results from the
     *  Windows clock ticking at 10 milliseconds with additional delays in
     *  the Windows kernel.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #startup}<br />
     *  @p
     */
    config Error.Id E_cannotSupport = {
        msg: "Timer cannot support requested period %d"
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

    /*! Error: Invalid timer specified.
     *
     *  An invalid timer id was specified. Timer id must be in the range
     *  0 to ({@link #NUM_TIMER_DEVICES} - 1). In the {@link #create()} call,
     *  an id of -1 may be used to request any available timer.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #init}<br />
     *  {@link #postInit}<br />
     *  {@link #start}<br />
     *  {@link #stop}<br />
     *  @p
     */
    config Error.Id E_invalidTimer = {
        msg: "E_invalidTimer: Invalid Timer Id %d"
    };

    /*! Error: no timers available.
     *
     *  A request was made for any available timer, but there are none
     *  available at this time.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #postInit}<br />
     *  @p
     */
    config Error.Id E_noneAvailable = {
        msg: "no available timer"
    };

    /*! Error: API not supported on Windows
     *
     *  This timer API is not supported on Windows.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #trigger}<br />
     *  @p
     */
    config Error.Id E_notSupported = {
        msg: "API not supported on Windows"
    };

    /*! Error: CreateThread failed.
     *
     *  A CreateThread call into the Win32 API has failed.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #startup}<br />
     *  @p
     */
    config Error.Id E_thread = {
        msg: "CreateThread failed"
    };

    /*! Error: requested timer not available.
     *
     *  A request was made for a timer, but it is not available at this time.
     *  It is currently in use.
     *
     *  @a(Raised_In)
     *  @p(html)
     *  {@link #init}<br />
     *  {@link #postInit}<br />
     *  @p
     */
    config Error.Id E_notAvailable = {
        msg: "E_notAvailable: Timer not available %d"
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

    //! Available mask to be used when select = TIMER_ANY.
    config UInt anyMask = 0x3;


internal:   /* not for client use */
   
    struct PrivateData;

    struct Instance_State {
        Int                     id;
        ITimer.RunMode          runMode;
        ITimer.StartMode        startMode;
        UInt                    period;
        ITimer.PeriodType       periodType;    
        Types.FreqHz            extFreq;
        UInt                    intNum;
        FuncPtr                 tickFxn;
        UArg                    tickFxnArg;
        Hwi.Handle              hwi;
    };

    struct Module_State {
        Char            availMask;
        Handle          handles[NUM_TIMER_DEVICES];
        PrivateData     *privateData;   /* Windows types not allowed in xdc */
    };
}
/*
 *  @(#) ti.sysbios.family.windows; 2, 0, 0, 0,553; 5-18-2012 06:05:20; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

