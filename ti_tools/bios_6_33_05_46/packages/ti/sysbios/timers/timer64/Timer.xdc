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

package ti.sysbios.timers.timer64;

import xdc.rov.ViewInfo;

import xdc.runtime.Assert;
import xdc.runtime.Error;
import xdc.runtime.Types;
import ti.sysbios.interfaces.ITimer;
import ti.sysbios.hal.Hwi;

/*!
 *  ======== Timer ========
 *  Timer Peripheral Manager.
 *
 *  This Timer module manages the timer64 peripheral available on many devices.
 *  It is supported on the ARM and c64x+ DSP targets. This module supports the
 *  timer in '32-bit chained' and '32-bit unchained' mode.  In the
 *  '32-bit unchained' mode, specify the lower or upper half to be use.
 *  The physical timer being used will be taken out of reset, only when
 *  its specified to be the master.
 *
 *  For shared timers on a homogeneous multicore device (ie. c6472), each
 *  core can create the timer, but only one core will initialize the timer
 *  and take it out of reset.  The core that does the initialization can be
 *  specified by the module configuration parameter
 *  timerSettings[].ownerCoreId in the *.cfg file.
 *  By default, Core 0 is the owner for all shared timers.
 *
 *  Note:  Creating a timer with 'Timer.ANY' specified as the id will not
 *         return a shared timer on a homogeneous multicore device.  To use
 *         a shared timer, specify the timer id explicitly when creating it.
 *         On these devices Timer.ANY specifies the local timer id.  This
 *         allows a single image to run on multiple cores since each core
 *         will program a different local timer.
 *
 *  The following sample .cfg code sets core 1 to initialize a shared timer
 *  with id 4.
 *
 *  @p(code)
 *  var Timer = xdc.useModule('ti.sysbios.timers.timer64.Timer');
 *  
 *  // sets core 1 to init and release Timer 4.
 *  Timer.timerSettings[4].ownerCoreId = 1;
 *  @p
 *
 *  The following sample .cfg code sets core 0 to initialize a shared timer
 *  with id 4.  It also configures the Clock module to use this timer.
 *  This allows multiple cores to share timer 4 for the Clock module's
 *  interrupt source.
 *
 *  @p(code)
 *  // sets core 0 to init and release Timer 4.
 *  var Timer = xdc.useModule('ti.sysbios.timers.timer64.Timer');
 *  Timer.timerSettings[4].ownerCoreId = 0;
 *
 *  var Clock = xdc.useModule('ti.sysbios.knl.Clock');
 *  Clock.timerId = 4;
 *  @p
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center">
 *    </colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th>
 *    <th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                                -->
 *    <tr><td> {@link #getNumTimers}            </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getStatus}               </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #Params_init}             </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #construct}               </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #create}                  </td><td>   N    </td>
 *    <td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #delete}                  </td><td>   N    </td>
 *    <td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #destruct}                </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getCount}                </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getFreq}                 </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getFunc}                 </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getPeriod}               </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #reconfig}                </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setFunc}                 </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setPeriod}               </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setPeriodMicroSecs}      </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #start}                   </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #stop}                    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started 
 *    (e.g. Timer_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup before this module is started 
 *   (e.g. Timer_Module_startupDone() returns FALSE).</li>
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
 *  ID when calling {@link ti.sysbios.hal.Timer#Timer_create}. 
 *  However, the timer ID
 *  specified may not always map to that timer; for example, specifying an ID
 *  value of 1 does not necessarily mean that this will map to "GPTimer1".
 *  These tables are provided to show which timers map to which timer IDs.
 *  </p>
 *  {@link ./doc-files/TimerTables.html Timer Mapping Tables}
 *  @p
 *
 */
@InstanceFinalize       /* To cleanup */
@InstanceInitError      /* To report unavailability of timer */
@ModuleStartup                /* to configure static timers */

module Timer inherits ti.sysbios.interfaces.ITimer
{
    /*! In 32-bit modes, used to specify which half of Timer to use */
    enum Half {
        Half_LOWER,
        Half_UPPER,
        Half_DEFAULT
    };

    /*!
     *  The different modes of the Timer. These values match the TIMMODE
     *  bit fields of the Timer Global Control Register.
     */
    enum Mode {
        Mode_64BITGPTIMER = 0,
        Mode_UNCHAINED = 1,
        Mode_WATCHDOG = 2,
        Mode_CHAINED = 3
    };

    /*! Max value of Timer period for PeriodType_COUNTS*/
    const UInt MAX_PERIOD = 0xffffffff;

    /*! Timer Control Register struct. */
    struct Control {
        Bits8 tien;      /*! 0=Clock not gated by TINP; 1=Clock gated */
        Bits8 invout;    /*! 0=Uninverted TSTAT drives TOUT; 1=Inverted TSTAT */
        Bits8 invin;     /*! 0=Uninverted TINP drives Timer; 1=Inverted TINP */
        UInt8 pwid;      /*! TSTATx goes inactive after pwid cycles (CP=0) */
        Bits8 cp;        /*! 0=pulse mode; 1=clock mode */
    };

    /*! Timer Emulation Management Register struct. */
    struct EmuMgt {
        Bool free;        /*! 0=suspend for emu halt; 1=don't suspend */
        Bool soft;        /*! 0=stop immediately; 1=stop when count==period */
    };

    /*! Timer GPIO interrupt control and enable Management Register struct. */
    struct GpioIntEn {
        Bits8 gpint12_eni;  /*! 0=source by timer; 1=input to source event */
        Bits8 gpint12_eno;  /*! 0=source by timer; 1=output to source event */
        Bits8 gpint12_invi; /*! 0=don't invert invput; 1=invert input */
        Bits8 gpint12_invo; /*! 0=don't invert output; 1=invert output */
        Bits8 gpint34_eni;  /*! 0=source by timer; 1=input to source event */
        Bits8 gpint34_eno;  /*! 0=source by timer; 1=output to source event */
        Bits8 gpint34_invi; /*! 0=don't invert invput; 1=invert input */
        Bits8 gpint34_invo; /*! 0=don't invert output; 1=invert output */
        Bits8 gpio_eni12;   /*! 0=TINP12 as timer input; 1=TINP12 as GPIO */
        Bits8 gpio_eno12;   /*! 0=TOUTP12 as timer output; 1=TOUTP12 as GPIO */
        Bits8 gpio_eni34;   /*! 0=TINP34 as timer input; 1=TINP34 as GPIO */
        Bits8 gpio_eno34;   /*! 0=TOUTP34 as timer output; 1=TOUTP12 as GPIO */
    };

    /*! Timer GPIO Data and Direction Management Register struct. */
    struct GpioDatDir {
        Bits8 gpio_dati12; /*! 0=TINP12 is input; 1=TINP12 is output */
        Bits8 gpio_dato12; /*! 0=TOUTP12 is input; 1=TOUTP12 is output */
        Bits8 gpio_dati34; /*! 0=TINP34 is input; 1=TINP34 is output */
        Bits8 gpio_dato34; /*! 0=TOUTP34 is input; 1=TOUTP34 is output */
        Bits8 gpio_diri12; /*! 0=input as GPIO input; 1=input as GPIO output */
        Bits8 gpio_diro12; /*! 0=output as GPIO input;1=output as GPIO output */
        Bits8 gpio_diri34; /*! 0=input as GPIO input; 1=input as GPIO output */
        Bits8 gpio_diro34; /*! 0=output as GPIO input;1=output as GPIO output */
    };

    /*! Timer Settings. */
    struct TimerSetting {
        Mode mode ;         /*! mode to put each Timer into */
        Bool master;        /*! for 'unchained' mode; 1=set mode and reset */
        UInt16 ownerCoreId; /*! used only for homogeneous multicore DSPs */
    };

    /*! @_nodoc */
    @XmlDtd
    metaonly struct BasicView {
        Ptr         halTimerHandle;
        String      label;
        UInt        id;
        String      startMode;
        String      runMode;
        UInt        period;
        String      periodType;
        String      half;
        UInt        prescalar;
        UInt        intNum;
        String      tickFxn[];
        UArg        arg;
        String      extFreqLow;
        String      extFreqHigh;
        String      hwiHandle;
    };


    /*! @_nodoc */
    metaonly struct ModuleView {
        String      availMask;      /* avaliable 32-bit timer halves */
        String      intFrequency[];    /* internal frequency in Hz */
    }

    /*! @_nodoc */
    metaonly struct Device_View {
        UInt        id;
        Bool        inUse;
        UInt32      intFreq;
        UInt        intNum;
        UInt        eventId;
        String      baseAddress;
    };

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
     *  Assert raised when static created timer is not available
     */
    config xdc.runtime.Assert.Id A_notAvailable =
        {msg: "A_notAvailable: static created timer not available"};
    
    /*!
     *  Error raised when timer id specified is not supported.
     */
    config Error.Id E_invalidTimer = {
        msg: "E_invalidTimer: Invalid Timer Id %d"
    };

    /*!
     *  Error raised when timer requested is in use
     */
    config Error.Id E_notAvailable  = {
        msg: "E_notAvailable: Timer not available %d"
    };

    /*!
     *  Error raised when period requested is not supported
     */
    config Error.Id E_cannotSupport = {
        msg: "E_cannotSupport: Timer cannot support requested period %d"
    };

    /*!======== anyMask ========
     *  Available mask to be used when select = Timer_ANY. Set in xs file.
     */
    config Bits32 anyMask;

    /*! 
     *  ======== defaultHalf ========
     *        The default 32-bit half of the timer to be used.
     */
    config Half defaultHalf = Half_LOWER;

    /*! 
     *  ======== timerSettings ========
     *  Global Control configuration for each physical timer.
     *
     *  mode: Mode_UNCHAINED        32-bit unchained mode.
     *  master: TRUE                If TRUE, release Timer from reset.
     */
    config TimerSetting timerSettings[] = [];

    /*!
     *  ======== intFreq ========
     *  @_nodoc
     *  Internal frequency for Timer
     */
    metaonly config Types.FreqHz intFreq;

    /*!
     *  ======== intFreq ========
     *  Internal frequency for Timers
     */
    metaonly config Types.FreqHz intFreqs[];

instance:

    /*! 
     *  ======== controlInit ========
     *  Control configuration. Default is all fields 0 or false except:
     *
     *      pwid: 1
     */
    config Control controlInit = {tien: 0, invout: 0, invin: 0,
                                  pwid: 1, cp: 0};

    /*! 
     *  ======== emuMgtInit ========
     *  Emulation Management configuration. Default is:
     *
     *      free: 0
     *      soft: 0
     */
    config EmuMgt emuMgtInit = {free: 0, soft: 0};

    /*! 
     *  ======== gpioIntEn ========
     *  General Purpose IO interrupt control and enable Management
     *  configuration. The default for all fields is 0.
     */
    config GpioIntEn gpioIntEn = {
        gpint12_eni: 0, gpint12_eno: 0, gpint12_invi: 0, gpint12_invo: 0,
        gpint34_eni: 0, gpint34_eno: 0, gpint34_invi: 0, gpint34_invo: 0,
        gpio_eni12:  0, gpio_eno12:  0, gpio_eni34:   0, gpio_eno34:   0};
    
    /*! 
     *  ======== gpioDatDir ========
     *  General Purpose IO data and direction Management configuration
     *  The default is all fields is 0.
     */
    config GpioDatDir gpioDatDir = {
        gpio_dati12: 0, gpio_dato12: 0, gpio_dati34: 0, gpio_dato34: 0,
        gpio_diri12: 0, gpio_diro12: 0, gpio_diri34: 0, gpio_diro34: 0};

    /*! 
     *  ======== half ========
     *  In 32-bit unchained mode, this field is used to specify which half
     *  of the timer to use.
     */
    config Half half = Half_DEFAULT;

    /*! Hwi Params for Hwi Object. Default is null. */
    config Hwi.Params *hwiParams = null;

    /*! Hwi interrupt number to be used by Timer. */
    config Int intNum = -1;

    /*! 
     *  ======== prescalar ========
     *  32-bit pre-scalar to TIM12 in '32-bit chained' mode. 
     *  The default is 0.
     */
    config UInt prescalar = 0;

    /*!
     *  ======== reconfig ========
     *  Used to modify static timer instances at runtime.
     *
     *  @param(timerParams)     timer Params
     *  @param(tickFxn)                functions that runs when timer expires
     */
    @DirectCall
    Void reconfig(FuncPtr tickFxn, const Params *timerParams, Error.Block *eb);


internal:   /* not for client use */

    /*! Device-specific Timer implementation. */
    proxy TimerSupportProxy inherits ti.sysbios.interfaces.ITimerSupport;

    /*!
     *  ======== staticAvailMask ========
     *  The number of statically available 32-bit timer halves
     *
     *  This is required for supporting timers that are local to a
     *  particular processor.  The module availMask keeps track of
     *  what timers are available at runtime.  The two mask are the
     *  same if there is no concept of local timers.
     */
    metaonly config Int staticAvailMask;

    /*!
     *  ======== startupNeeded ========
     *  This flag is use to prevent Timer_startup code (includes postInit(),
     *  deviceConfig(), initDevice() to be brought in un-necessarily.
     */
    config Bool startupNeeded = false;
    
    /*!
     *  ======== freqDivisor ========
     *  For some devices, the timer frequency is determined by the CPU
     *  frequency divided by a value.  This parameter captures that value
     *  and is used during runtime to re-calculate the timer frequency
     *  when the CPU frequency is changed and Timer_getFreq is called.
     *  For devices with a fix timer frequency, this value is 0.
     */
    config UInt freqDivisor = 0;

    /*! Information about timer */
    struct TimerDevice {
        UInt intNum;
        UInt eventId;
        Ptr  baseAddr;
    };

    /*!
     *  ======== numTimerDevices ========
     *  The number of logical timers on a device.
     */
    config Int numTimerDevices;

    /*!
     *  ======== numLocalTimers ========
     *  The number of physical local timers on a device.
     */
    config Int numLocalTimers = 0;

    /*!
     *  ======== spinLoop ========
     *  used by trigger function.
     */
    Void spinLoop(UInt count);

    /*! Instance state structure */
    struct Instance_State {
        Bool              staticInst;   /* statically created or not */
        Int               id;           /* logical timer id. */
        Half              half;         /* which half */
        UInt              tcrInit;      /* tcr  */
        UInt              emumgtInit;   /* emu mgt */
        UInt              gpioIntEn;    /* GPIO interrupt control */
        UInt              gpioDatDir;   /* GPIO data and direction */
        ITimer.RunMode    runMode;      /* timer mode */
        ITimer.StartMode  startMode;    /* timer mode */
        UInt              period;       /* period */
        ITimer.PeriodType periodType;   /* type (microsecs, inst) */
        UInt              prescalar;    /* pre-scalar for TIM12 */
        UInt              intNum;       /* intr num */
        UArg              arg;          /* isrFxn arg */
        Hwi.FuncPtr       tickFxn;      /* Timer fxn plugged into Hwi */
        Types.FreqHz      extFreq;      /* ext freq */
        Hwi.Handle        hwi;          /* hwi inst */
    }

    /*! Module state structure */
    struct Module_State {
        Bits32        availMask;    /* avaliable 32-bit timer halves */
        Types.FreqHz  intFreqs[];   /* internal frequency in Hz */
        TimerSetting  gctrl[];      /* global control information */
        TimerDevice   device[];     /* timer device information */
        Handle        handles[];    /* handles based on logical id */
    }
}
/*
 *  @(#) ti.sysbios.timers.timer64; 2, 0, 0, 0,407; 5-18-2012 06:06:17; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

