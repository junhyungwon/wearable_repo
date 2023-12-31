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
 */

package ti.sysbios.family.msp430;

import xdc.rov.ViewInfo;

import xdc.runtime.Types;
import xdc.runtime.Error;

import ti.sysbios.interfaces.ITimer;

/*!
 *  ======== Timer ========
 *  MSP430 Timer Peripherals Manager
 *
 *  This module manages the timer peripherals available on MSP430 devices
 *  (see {@link ./doc-files/TimerTables.html Timer Mapping Tables} for
 *  supported device information).
 *
 *  The Timer module supports the timer in 'one shot', 'continuous', and
 *  'dynamic' modes.
 *
 *  In 'one shot' mode, a timer function will "fire" (run) when the timer
 *  period expires. In 'one shot' mode this will only happen once.
 *
 *  In 'continuous' mode, the specified timer function will "fire" every
 *  time the period expires, throughout the lifetime of the program.
 *
 *  In 'dynamic' mode, the specified timer function will "fire" every
 *  time the period expires.  But the period of the timer can be changed
 *  dynamically, to correspond to the next tick interrupt needed from the
 *  timer.  This mode is used by the SYS/BIOS 
 *  {@link ti.sysbios.knl.Clock Clock} module for implementing
 *  dynamic tick suppression, to reduce the number of interrupts from the
 *  timer to the minimum required for currently scheduled timeouts.
 *
 *  NOTE: In the current implementation on MSP430 Timers cannot be 
 *  created dynamically at runtime by the application.  Timers must be
 *  created statically in the application configuration.
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center">
 *  </colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th>
 *  <th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                          -->
 *    <tr><td> {@link #getNumTimers}            </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getStatus}               </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #Params_init}             </td><td>   N    </td>
 *  <td>   N    </td><td>   N    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #construct}               </td><td>   N    </td>
 *  <td>   N    </td><td>   N    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #create}                  </td><td>   N    </td>
 *  <td>   N    </td><td>   N    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #delete}                  </td><td>   N    </td>
 *  <td>   N    </td><td>   N    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #destruct}                </td><td>   N    </td>
 *  <td>   N    </td><td>   N    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getCount}                </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getFreq}                 </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getFunc}                 </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getPeriod}               </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #reconfig}                </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setFunc}                 </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setPeriod}               </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setPeriodMicroSecs}      </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #start}                   </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #stop}                    </td><td>   Y    </td>
 *  <td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started
 *  (e.g. Timer_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup before this module is started
 *  (e.g. Timer_Module_startupDone() returns FALSE).</li>
 *           </ul>
 *       </ul>
 *    </td></tr>
 *
 *  </table>
 *  @p
 *
 *  @p(html)
 *  <h3> Timer Mapping Tables </h3>
 *  <p>
 *  The Timer module allows the user to use and configure the various timers
 *  that exist on a particular device.  This is achieved by specifying a timer
 *  ID when statically creating the Timer instance.
 *  A convention is used to assign logical IDs to physical timers.  ID '0' 
 *  corresponds to the first Timer_A peripheral (TA0).  ID '1' is assigned to
 *  the next Timer_A (TA1), until there are no more Timer_A peripherals.  Then
 *  the next ID is assigned to the first Timer_B peripheral (if present), and
 *  so on.  
 *  These tables are provided to show which timers map to which timer IDs.
 *  </p>
 *  {@link ./doc-files/TimerTables.html Timer Mapping Tables}
 *  @p
 */
@InstanceFinalize       /* To cleanup */
@InstanceInitError      /* To report unavailability of timer */
@ModuleStartup          /* To configure static timers */

module Timer inherits ti.sysbios.interfaces.ITimer
{
    /*! override supportsDynamic - this Timer does support RunMode_DYNAMIC */
    override metaonly config Bool supportsDynamic = true;

    /*! override defaultMode - use RunMode_DYNAMIC by default */
    override metaonly config Bool defaultDynamic = true;

    // -------- Module Types --------

    /*! Lists of input clock sources for timers */
    enum Source {
        Source_ACLK,                    /*! ACLK */
        Source_SMCLK,                   /*! SMCLK */
        Source_EXTERNAL,                /*! External clock */
        Source_EXTERNAL_INVERTED        /*! Inverted external clock */
    };

    /*! Max value of Timer period for PeriodType_COUNTS */
    const UInt MAX_PERIOD = 0x0000ffff;

    /*! @_nodoc
     *  Min instructions to use in trigger().
     */
    const Int MIN_SWEEP_PERIOD = 1;

    /*! Timer Configuration struct.
     *
     *  @field(source)  Timer clock source.
     */
    struct Control {
        UInt source;   /*! 0x100=ACLK, 0x200=SMCLK, 0=EXT, 0x300=EXT_INVERT */
    };

    /*! @_nodoc */
    @XmlDtd
    metaonly struct BasicView {
        Ptr         halTimerHandle;
        String      label;
        UInt        id;
        Control     configuration;
        String      startMode;
        String      runMode;
        UInt        period;
        String      periodType;
        Bool        synchronous;
        UInt        intNum;
        String      tickFxn[];
        UArg        arg;
        UInt        frequency;
        String      hwiHandle;
    };

    /*! @_nodoc */
    metaonly struct DeviceView {
        UInt        id;
        String      device;
        String      devAddr;
        UInt        intNum;
        String      runMode;
        UInt        period;
        UInt        currCount;
        UInt        remainingCount;
        UInt        prevThreshold;
        String      state;
    };

    /*! @_nodoc */
    metaonly struct ModuleView {
        String      availMask;      /* available 32-bit timer halves */
    }

    /*! @_nodoc */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo = 
        ViewInfo.create({
            viewMap: [
            [
                'Basic',
                {
                    type: ViewInfo.INSTANCE,
                    viewInitFxn: 'viewInitBasic',
                    structName: 'BasicView'
                }
            ],
            [
                'Device',
                {
                    type: ViewInfo.INSTANCE,
                    viewInitFxn: 'viewInitDevice',
                    structName: 'DeviceView'
                }
            ],
            [
                'Module',
                {
                    type: ViewInfo.MODULE,
                    viewInitFxn: 'viewInitModule',
                    structName: 'ModuleView'
                }
            ],
            ]
        });

    /*!
     *  ======== E_invalidTimer ========
     *  Error raised when specified timer id is not supported
     */
    config Error.Id E_invalidTimer = {
        msg: "E_invalidTimer: Invalid Timer Id %d"
    };

    /*!
     *  ======== E_notAvailable ========
     *  Error raised when requested timer is in use
     */
    config Error.Id E_notAvailable = {
        msg: "E_notAvailable: Timer not available %d"
    };

    /*!
     *  ======== E_cannotSupport ========
     *  Error raised when requested period is not supported
     */
    config Error.Id E_cannotSupport = {
        msg: "E_cannotSupport: Timer cannot support requested period %d"
    };

    /*!
     *  ======== E_runtimeCreate ========
     *  Error raised when dynamic Timer create is attempted
     */
    config Error.Id E_runtimeCreate = {
        msg: "E_runtimeCreate: Runtime Timer create is not supported %d"
    };

    /*!
     *  ======== anyMask ========
     *  Mask of available timers
     *
     *  This mask is used to identify the timers that can be used when
     *  Timer_create() is called with an id equal to
     *  {@link Timer#ANY Timer_ANY}.
     */
    config UInt anyMask = 0x7;

    /*!
     *  ======== keepAwake ========
     *  Wakeup (enable) CPU when Timer ISR returns
     *
     *  If set to `true`, all Timer instances keep the CPU awake upon
     *  return from interrupt.
     */
    config Bool keepAwake = false;

    /*!
     *  ======== oneShotStub ========
     *  @_nodoc
     *
     *  @param(arg)     Unused.
     */
    Void oneShotStub(UArg arg);

    /*!
     *  ======== oneShotNestStub ========
     *  @_nodoc
     *
     *  @param(arg)     Unused.
     */
    Void oneShotNestStub(UArg arg);

    /*!
     *  ======== periodicStub ========
     *  @_nodoc
     *
     *  @param(arg)     Unused.
     */
    Void periodicStub(UArg arg);

    /*!
     *  ======== periodicNestStub ========
     *  @_nodoc
     *
     *  @param(arg)     Unused.
     */
    Void periodicNestStub(UArg arg);

    /*!
     *  ======== getHandle ========
     *  @_nodoc
     *  Used by TimestampProvider module to get hold of timer handle used by
     *  Clock.
     *
     *  @param(id)      timer Id.
     */
    Handle getHandle(UInt id);

instance:

    /*! Hwi Params for Hwi Object. Default is null. */
    config Hwi.Params *hwiParams = null;

    /*! Clock source input select. Default is ACLK. */
    config Source clockSource = Source_ACLK;

    /*! Selected clock source is synchronous to CPU clock? Default is false. */
    config Bool synchronous = false;

    /*! Enable nesting of other interrupts on top of this Timer's ISR? */
    config Bool nesting = false;

    /*! Previous threshold count value. */
    config UInt prevThreshold = 0;

    /*! Control register configuration. Default source = ACLK. */
    config Control controlRegInit = {source: Source_ACLK};

    /*!
     *  ======== reconfig ========
     *  Modify timer instances at runtime
     *
     *  @param(timerParams)     timer Params
     *  @param(tickFxn)         function that runs when timer expires.
     */
    Void reconfig(FuncPtr tickFxn, const Params *timerParams, Error.Block *eb);


internal:   /* not for client use */

    /*!
     *  ======== noStartupNeeded ========
     *  Flag used to prevent misc code from being brought in
     *  un-necessarily
     */
    config UInt startupNeeded = false;

    /*! Information about timer */
    struct TimerDevice {
        UInt intNum;
        Ptr  baseAddr;
    };

    /*!
     *  ======== numTimerDevices ========
     *  The number of physical timers on the device
     */
    config Int numTimerDevices;

    struct Instance_State {
        Bool                    staticInst;
        Int                     id;
        UInt                    controlRegInit;
        ITimer.RunMode          runMode;
        ITimer.StartMode        startMode;
        UInt                    period;
        ITimer.PeriodType       periodType;
        UInt                    intNum;
        UArg                    arg;
        Hwi.FuncPtr             tickFxn;
        Types.FreqHz            frequency;
        Hwi.Handle              hwi;
        UInt                    prevThreshold;
        Bool                    synchronous;
    }

    struct Module_State {
        UInt            availMask;      /* available peripherals */
        TimerDevice     device[];       /* timer device information */
        Handle          handles[];      /* array of handles based on id */
    }
}
/*
 *  @(#) ti.sysbios.family.msp430; 1, 0, 0, 0,215; 5-18-2012 06:05:16; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

