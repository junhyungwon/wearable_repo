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
 *  ======== BIOS.xs ========
 *
 */

var BIOS = null;
var Program = null;
var Memory = null;

var custom28xOpts = " -q -mo "; 
var custom430xOpts = " -q --advice:power=1 ";
var custom6xOpts = " -q -mi10 -mo -pdr -pden -pds=238 -pds=880 -pds1110 "; 
var customARP32xOpts = " -q --gen_func_subsections ";
var customArmOpts = " -q -ms --opt_for_speed=2 ";
var customGnuArmM3Opts = " -mcpu=cortex-m3 -mthumb -mabi=aapcs -mapcs-frame ";
var customGnuArmM4Opts = " -mcpu=cortex-m4 -mthumb -mabi=aapcs -mapcs-frame ";
var customGnuArmM4FOpts = " -mcpu=cortex-m4 -mthumb -mabi=aapcs -mapcs-frame -mhard-float ";

var ccOptsList = {
    "ti.targets.C28_large"                      : custom28xOpts,
    "ti.targets.C28_float"                      : custom28xOpts,
    "ti.targets.msp430.MSP430"                  : custom430xOpts,
    "ti.targets.msp430.MSP430X"                 : custom430xOpts,
    "ti.targets.msp430.MSP430X_small"           : custom430xOpts,
    "ti.targets.msp430.elf.MSP430"              : custom430xOpts,
    "ti.targets.msp430.elf.MSP430X"             : custom430xOpts,
    "ti.targets.msp430.elf.MSP430X_small"       : custom430xOpts,
    "ti.targets.C64"                            : custom6xOpts,
    "ti.targets.C64P"                           : custom6xOpts,
    "ti.targets.elf.C64P"                       : custom6xOpts,
    "ti.targets.C64P_big_endian"                : custom6xOpts,
    "ti.targets.elf.C64P_big_endian"            : custom6xOpts,
    "ti.targets.C674"                           : custom6xOpts,
    "ti.targets.elf.C674"                       : custom6xOpts,
    "ti.targets.elf.C67P"                       : custom6xOpts,
    "ti.targets.elf.C64T"                       : custom6xOpts,
    "ti.targets.elf.C66"                        : custom6xOpts,
    "ti.targets.elf.C66_big_endian"             : custom6xOpts,
    "ti.targets.arp32.elf.ARP32"                : customARP32xOpts,
    "ti.targets.arm.elf.Arm9"                   : customArmOpts,
    "ti.targets.arm.elf.A8F"                    : customArmOpts,
    "ti.targets.arm.elf.A8Fnv"                  : customArmOpts,
    "ti.targets.arm.elf.M3"                     : customArmOpts,
    "ti.targets.arm.elf.M4"                     : customArmOpts,
    "ti.targets.arm.elf.M4F"                    : customArmOpts,
    "ti.targets.arm.elf.R4t_big_endian"         : customArmOpts,
    "ti.targets.arm.elf.R4Ft_big_endian"        : customArmOpts,
    "gnu.targets.arm.M3"                        : customGnuArmM3Opts,
    "gnu.targets.arm.M4"                        : customGnuArmM4Opts,
    "gnu.targets.arm.M4F"                       : customGnuArmM4FOpts,
};

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    BIOS = this;
    Program = xdc.module('xdc.cfg.Program');

    var GetSet = xdc.module("xdc.services.getset.GetSet");
    GetSet.onSet(this, "libType", _setLibType);
    GetSet.onSet(this, "assertsEnabled", _setLibOption);
    GetSet.onSet(this, "logsEnabled", _setLibOption);
    GetSet.onSet(this, "runtimeCreatesEnabled", _setMemoryPolicy);
    GetSet.onSet(this, "customCCOpts", _setCustomCCOpts);

    /* 
     * Need to initialize GetSet on the 'cpuFreq' structure so that Grace is
     * updated when the fields of 'cpuFreq' are changed.
     */
    GetSet.init(BIOS.cpuFreq);
    
    /* 
     * For all targets except MSP430: convert the platform clock rate
     * in MHz to Hz, and set BIOS.cpuFreq.
     *
     * For MSP430: Leave undefined for now, set it later, after application 
     * config script has been processed.
     *
     */
    if (!(Program.build.target.name.match(/430/))) {

        var freq =   Program.cpu.clockRate * 1000000;
        BIOS.cpuFreq.lo = freq & 0xffffffff;
        BIOS.cpuFreq.hi =  Number((freq / Math.pow(2,32)).toFixed(0));
    }

    /* 
     * On Concerto devices register BIOS with the M3-side Boot module to listen
     * for changes to the CPU frequency and PLL and divider configuration.
     */
    if (Program.cpu.deviceName.match(/F28M35/)) {
        if (Program.build.target.name.match(/M3/)) {
            var BootM3 =xdc.module('ti.catalog.arm.cortexm3.concertoInit.Boot');
        
            if ('registerFreqListener' in BootM3) {        
                BootM3.registerFreqListener(this);
            }
        }
    }
    /* 
     * On 28x devices, register BIOS with the 28x Boot module to listen
     * for changes to the CPU frequency from the PLL configuration.
     */
    else if (Program.build.target.name.match(/28/)) {
        var Boot28 = xdc.module('ti.catalog.c2800.init.Boot');
        
        if ('registerFreqListener' in Boot28) {        
            Boot28.registerFreqListener(this);
        }
    }
    
    /* 
     * For Stellaris LM3 devices, listen to lm3init Boot module
     * for changes to the CPU frequency from the PLL configuration.
     */
    if (Program.cpu.deviceName.match(/LM3.*/)
        || Program.cpu.deviceName.match(/LM4.*/)) {
        var BootLM3 = xdc.module('ti.catalog.arm.cortexm3.lm3init.Boot');
        
        if ('registerFreqListener' in BootLM3) {        
            BootLM3.registerFreqListener(this);
        }
    }
    
    // Have to bind in a default proxy
    BIOS.RtsGateProxy = xdc.module('xdc.runtime.GateNull');

    /* used by Task, Swi, Event modules for error checking */
    BIOS.bitsPerInt = Program.build.target.bitsPerChar * BIOS.intSize.$sizeof();

    var targName = Program.build.target.$name;

    if (targName.indexOf("C28") != -1) {
        /* 
         * 28x targets have limited memory and require Task stacks to 
         * be placed in memory entirely below the address 0x10000.
         * Setting 'sectionName' to '.ebss:taskStackSection' accomplishes 
         * this.
         */
        BIOS.heapSection =".ebss:taskStackSection"; 
    }

    Memory = xdc.module("xdc.runtime.Memory");

    /* 
     * Construct default BIOS.customCCOpts 
     * User can override this in their cfg file.
     */
    BIOS.customCCOpts = Program.build.target.cc.opts;
    
    if (!(ccOptsList[Program.build.target.$name] === undefined)) {
        BIOS.customCCOpts += ccOptsList[Program.build.target.$name];
    }

    if (Program.build.target.$name.match(/gnu/)) {
        BIOS.customCCOpts += " -ffunction-sections -fdata-sections -O3 -combine ";
    }
    else {
        BIOS.customCCOpts += " --program_level_compile -o3 -g --optimize_with_debug";
    }

    /* tell module$use that customCCOpts has not been written */
    this.$private.customCCOptsWritten = false;

    /* 
     * Set default verbose level for custom build flow 
     * User can override this in their cfg file.
     */
    var SourceDir = xdc.module("xdc.cfg.SourceDir");
    SourceDir.verbose = 2;

}

/*
 *  ======== module$use ========
 */
function module$use()
{
    /*
     * warn users that are trying to build for SMP/BIOS
     */
    if (BIOS.smpEnabled == true) {
        this.$logError("This SYS/BIOS version does not support SMP!", this, "smpEnabled");
    }

    /* If app config has not specified an argSize, set it to zero */
    if (Program.$written("argSize") == false) {
        Program.argSize = 0x0;
    }

    /* 
     * For MSP430: Check if app config has explicitly defined BIOS.cpuFreq. 
     * If yes, use it; else, use defaults.
     *
     */
    if (Program.build.target.name.match(/430/)) {
        if (BIOS.cpuFreq.lo === undefined) {

            /* set default based upon whether Boot is doing boost or not ... */
            var Boot = xdc.module('ti.catalog.msp430.init.Boot');

            if (Boot.configureDCO == true) {
                /* currently fixed in Boot, and here too */
                BIOS.cpuFreq.lo = 8192000; 
            }
            else {
                /* currently fixed in IClock.xdc, and here too */
                BIOS.cpuFreq.lo = 1000000;
            }
        }

        if (BIOS.cpuFreq.hi === undefined) {
            BIOS.cpuFreq.hi = 0;
        }

        /* 
         * Issue deprecation warning for MSP430 COFF targets 
         *
         * !!!! Disabled for 6.33.02. Will enable when CCS and MSP430 !!!!
         * !!!! linker command files are ready                        !!!!
         */
//      if (!(Program.build.target.$name.match(/elf/))) {
        if (0) {
            this.$logWarning("Support for the MSP430 COFF targets will be " +
                "eliminated in future SYS/BIOS releases. Please switch to " +
                "building with the corresponding ti.targets.msp430.elf " +
                "MSP430 target.", Program.build.target.$name);
        }
    }
        
    /* 
     * For C28: Check to see if BIOS.cpuFreq is zero, indicating that 
     * the tms320x28 generic platform is being used, and the application has
     * not defined the CPU frequency.
     */
    if ((Program.build.target.name.match(/28/)) &&
        (BIOS.cpuFreq.lo == 0) && (BIOS.cpuFreq.hi == 0)) {
        this.$logError("BIOS.cpuFreq is undefined.  You must define the CPU frequency in your application configuration.  For example, if the PLL is being configured for 100MHz, add this to your application configuration script: BIOS.cpuFreq.lo = 100000000;", this);
    }

    /*
     * For 28x, if not using the generic platform, the stacksMemory is defined
     * and should be used.  If using generic platform, we have to rely on the
     * placement of .ebss to be at a low address so task stacks fall below 
     * the 0xffff page boundary.  If stack is above 0x10000, we get a runtime
     * error.
     */
    if ( (Program.build.target.name.match(/28/)) &&
            (Program.platform.stackMemory != undefined) && 
            (Program.sectMap[".ebss:taskStackSection"] === undefined) ) {
        Program.sectMap[".ebss:taskStackSection"] = new Program.SectionSpec();
        Program.sectMap[".ebss:taskStackSection"].loadSegment = 
            Program.platform.stackMemory;
    }

    /* Bring in Settings module for bios 6 osal */
    var Settings = xdc.useModule('ti.sysbios.xdcruntime.Settings', true);
    xdc.useModule('ti.sysbios.knl.Idle', true);

    /*
     *  Only use the types of gates that support nesting by not need the 
     *  matching leaves and enters (e.g. its ok not to call the leave if
     *  it was nested) as long as at least one leave is called with 
     *  the first returned key.
     */
    if (this.rtsGateType === undefined) {
        if (this.taskEnabled == true) {
            this.rtsGateType = BIOS.GateMutex;
        }
        else if (this.swiEnabled == true) {
            this.rtsGateType = BIOS.GateSwi;
        }
    }
    
    /* Make sure the correct gate module is used and that it makes sense. */
    if (this.rtsGateType == BIOS.GateMutexPri) {
        if (this.taskEnabled == true) {
            this.RtsGateProxy = xdc.useModule('ti.sysbios.gates.GateMutexPri', true);
        }
        else {
            this.$logError("Cannot use GateMutexPri when taskEnable is false.", this, "rtsGateType");
        }
    }
    else if (this.rtsGateType == BIOS.GateMutex) {
        if (this.taskEnabled == true) {
            this.RtsGateProxy = xdc.useModule('ti.sysbios.gates.GateMutex', true);
        }
        else {
            this.$logError("Cannot use GateMutex when taskEnable is false.", this, "rtsGateType");
        }        
    }
    else if (this.rtsGateType == BIOS.GateSwi) {
        if (this.swiEnabled == true) {
            this.RtsGateProxy = xdc.useModule('ti.sysbios.gates.GateSwi', true);
        }
        else {
            this.$logError("Cannot use GateSwi when swiEnable is false.", this, "rtsGateType");
        }             
    }    
    else if (this.rtsGateType == BIOS.GateHwi) {
        this.RtsGateProxy = xdc.useModule('ti.sysbios.gates.GateHwi', true);
    }

    /* Plug in a gate to be used by rts. */
    if (BIOS.rtsGateType != BIOS.NoLocking) {
        BIOS.addUserStartupFunction('&ti_sysbios_BIOS_registerRTSLock');        
    }

    /* Setting default heap section */
    var Memory = xdc.useModule('xdc.runtime.Memory');
    if (Memory.$written("defaultHeapInstance") == true) {
        /* cfg has written defaultHeapInstance, so use it! */
        if (BIOS.$written("heapSize") == true) {
            this.$logWarning("BIOS.heapSize and Memory.defaultHeapInstance " +
                "have both been set.  BIOS.heapSize ignored.  " +
                "Using Memory.defaultHeapInstance.", this);
        }
    }
    else {
        if (BIOS.$written("heapSize")) {
            if (Memory.$written("defaultHeapSize") &&
                    Memory.defaultHeapSize != BIOS.heapSize) {
                this.$logWarning("BIOS.heapSize and Memory.defaultHeapSize " +
                "have both been set.  Memory.defaultHeapSize ignored.  " +
                "Using BIOS.heapSize.", this); 
            }
            if (Program.$written("heap") && Program.heap != BIOS.heapSize) {
                this.$logWarning("BIOS.heapSize and Program.heap " +
                "have both been set.  Program.heap ignored.  " +
                "Using BIOS.heapSize.", this); 
            }
        }
        else if (Memory.$written("defaultHeapSize")) {
            /* app .cfg set Memory.defaultHeapSize, but not BIOS.heapSize */
            BIOS.heapSize = Memory.defaultHeapSize;

            if (Program.$written("heap") &&
                    Program.heap != Memory.defaultHeapSize) {
                this.$logWarning("Memory.defaultHeapSize and Program.heap " +
                "have both been set.  Program.heap ignored.  " +
                "Using Memory.defaultHeapSize.", this);
            }
        }
        else if (Program.$written("heap")) {
            /* app .cfg only set Program.heap, so we use that. */
            BIOS.heapSize = Program.heap;
        }
        
        if (BIOS.heapSize == 0) {
            var HeapNull = xdc.useModule('ti.sysbios.heaps.HeapNull', true);
            HeapNull.common$.fxntab = true;
            var heap0 = HeapNull.create();

            Memory.defaultHeapInstance = heap0;
        }
        else {
            var HeapMem = xdc.useModule('ti.sysbios.heaps.HeapMem', true);
            HeapMem.common$.fxntab = true;
            var heapMemParams = new HeapMem.Params;
            heapMemParams.size = BIOS.heapSize;
            heapMemParams.sectionName = BIOS.heapSection;
            var heap0 = HeapMem.create(heapMemParams);

            Memory.defaultHeapInstance = heap0;
        }
    }    

    /*
     * If HeapStd is not in use, then BIOS will override malloc(), free(), etc.
     * so there is no need for Program.heap.
     */
    if (xdc.module("xdc.runtime.HeapStd").$used == false) {
        Program.heap = 0;
    }

    /* export Error and memory because it is used by malloc.c */
    Program.exportModule('xdc.runtime.Error');
    Program.exportModule('xdc.runtime.Memory');

    /* Hijack Error.raiseHook and insert ours in before it */
    var Error = xdc.module('xdc.runtime.Error');
    BIOS.installedErrorHook = Error.raiseHook;
    Error.raiseHook = BIOS.errorRaiseHook;

    var profile;

    /*
     * Get the profile associated with the ti.sysbios package.
     * The profile can be specified on a per package basis with a line like
     * this in your .cfg script:     
     *
     * xdc.loadPackage('ti.sysbios').profile = "release";
     */
    if (this.$package.profile != undefined) {
        profile = this.$package.profile;
    }
    else {
        profile = Program.build.profile;
    }

    /*
     * Gracefully handle non-supported mix of BIOS.libType and build
     * profiles/targets
     */
    if (BIOS.libType != BIOS.LibType_Debug 
        && (cList[Program.build.target.name] === undefined)) {
        /* if we don't support custom or pre-built libs for this target */
        BIOS.libType = BIOS.LibType_Debug;
    }
    else if (profile.match(/whole_program/) 
        && (BIOS.libType != BIOS.LibType_Debug)) {
        /* allow build to proceed */
        BIOS.libType = BIOS.LibType_Debug;
        /* but warning the user */
        BIOS.$logWarning("The '" + profile + 
            "' build profile will not be supported " +
            "in future releases of SYS/BIOS.  " +
            "Use 'release' or 'debug' profiles together with the " +
            "'BIOS.libType' configuration parameter to specify your " +
            "preferred library.  See the compatibility section of " +
            "your SYS/BIOS release notes for more information.", 
            "Profile Deprecation Warning", BIOS);
    }

    /* inform getLibs() about location of library */
    switch (BIOS.libType) {
        case BIOS.LibType_Instrumented:
            this.$private.libraryName = "/sysbios.lib";
            this.$private.outputDir = this.$package.packageBase 
                        + "lib/instrumented_" 
                        + Program.build.target.suffix
                        + "/sysbios";
            break;

        case BIOS.LibType_NonInstrumented:
            this.$private.libraryName = "/sysbios.lib";
            this.$private.outputDir = this.$package.packageBase 
                        + "lib/nonInstrumented_" 
                        + Program.build.target.suffix
                        + "/sysbios";
            break;

        case BIOS.LibType_Custom:
            this.$private.libraryName = "/sysbios.lib";
            var SourceDir = xdc.useModule("xdc.cfg.SourceDir");
            if (BIOS.libDir !== null) {
                SourceDir.outputDir = BIOS.libDir;
                var src = SourceDir.create("sysbios");
                this.$private.outputDir = src.getGenSourceDir();
            }
            else {
                var curPath = java.io.File(".").getCanonicalPath();
                /* if package.bld world */
                if (java.io.File(curPath).getName() != "configPkg") {
                    var appName = Program.name.substring(0, Program.name.lastIndexOf('.'));
                    appName = appName + "_p" + Program.build.target.suffix + ".src";
                    SourceDir.outputDir = "package/cfg/" + appName;
                    var src = SourceDir.create("sysbios");
                    this.$private.outputDir = src.getGenSourceDir();
                }
                /* else ccs world */
                else {
                    /* request output source directory for generated files */
                    var appName = Program.name.substring(0, Program.name.lastIndexOf('.'));
                    appName = appName + "_" + Program.name.substr(Program.name.lastIndexOf('.')+1);
                    var src = SourceDir.create("sysbios/");

                    /* save this directory in our private state (to be read during
                    * generation, see Gen.xdt)
                    */
                    this.$private.outputDir = src.getGenSourceDir();
                }
            }
            break;
    }
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    mod.cpuFreq = params.cpuFreq;
    mod.rtsGateCount = 0;
    mod.rtsGateKey   = 0;
    mod.threadType   = BIOS.ThreadType_Main;
    
    /* Plug in a gate to be used by rts. */
    if (BIOS.rtsGateType != BIOS.NoLocking) {        
        mod.rtsGate = BIOS.RtsGateProxy.create();
    }
    else {
        mod.rtsGate = null;
    }
    
    /* Assign BIOS_start to it's symbol for ROM */
    mod.startFunc = '&ti_sysbios_BIOS_startFunc';
}

/*
 *  ======== getEnumString ========
 *  Get the enum value string name, not 0, 1, 2 or 3, etc.  For an enumeration
 *  type property.
 *
 *  Example usage:
 *  if obj contains an enumeration type property "Enum enumProp"
 *
 *  view.enumString = getEnumString(obj.enumProp);
 *
 */
function getEnumString(enumProperty)
{
    /*
     *  Split the string into tokens in order to get rid of the huge package
     *  path that precedes the enum string name. Return the last 2 tokens
     *  concatenated with "_" 
     */
    var enumStrArray = String(enumProperty).split(".");
    var len = enumStrArray.length;
    return (enumStrArray[len - 1]);
}

/*
 *  ======== module$validate ========
 *  some redundant tests are here to catch changes since
 *  module$static$init() and instance$static$init()
 */
function module$validate()
{
    var Settings = xdc.module("ti.sysbios.family.Settings");
    var HwiDelegate = xdc.module(Settings.getDefaultHwiDelegate());
    var Swi = xdc.module("ti.sysbios.knl.Swi");
    var Semaphore = xdc.module("ti.sysbios.knl.Semaphore");
    var Load = xdc.module("ti.sysbios.utils.Load");
    var Defaults = xdc.module('xdc.runtime.Defaults');
    var Diags = xdc.module("xdc.runtime.Diags");
    var libType = getEnumString(BIOS.libType);

    switch (BIOS.libType) {
        case BIOS.LibType_Instrumented:
        case BIOS.LibType_NonInstrumented:
            if (BIOS.taskEnabled == false) {
                BIOS.$logError("BIOS.taskEnabled must be set to 'true' when " +
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 BIOS, "taskEnabled");
            }
            if (BIOS.swiEnabled == false) {
                BIOS.$logError("BIOS.swiEnabled must be set to 'true' when " + 
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 BIOS, "swiEnabled");
            }
            if ((HwiDelegate.hooks.length != 0) 
                 && (Program.build.target.name != "C64T")) {
                if (Load.hwiEnabled == true) {
                    BIOS.$logError("Load.hwiEnabled must be set to 'false' when " + 
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 Load, "hwiEnabled");
                }
                else {
                    BIOS.$logError("Hwi Hooks are not supported when " + 
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 BIOS);
                }
            }
            if (Swi.hooks.length != 0) {
                if (Load.swiEnabled == true) {
                    BIOS.$logError("Load.swiEnabled must be set to 'false' when " + 
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 Load, "swiEnabled");
                }
                else {
                    BIOS.$logError("Swi Hooks are not supported when " + 
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 BIOS);
                }
            }
            if (Semaphore.supportsEvents == true) {
                BIOS.$logError("Semaphore.supportsEvents must be set to 'false' when " + 
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 Semaphore, "supportsEvents");
            }
            break;

        case BIOS.LibType_Custom:
        case BIOS.LibType_Debug:
        default:
            break;
    }

    switch (BIOS.libType) {
        case BIOS.LibType_Instrumented:
            if (BIOS.assertsEnabled == false) {
                BIOS.$logWarning("BIOS.assertsEnabled must be set to 'true' when " +
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 BIOS, "assertsEnabled");
            }
            if (BIOS.logsEnabled == false) {
                BIOS.$logWarning("BIOS.logsEnabled must be set to 'true' when " +
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 BIOS, "logsEnabled");
            }
            break;

        case BIOS.LibType_NonInstrumented:
            if ((BIOS.assertsEnabled == true) && BIOS.$written("assertsEnabled")) {
                BIOS.$logWarning("BIOS.assertsEnabled must be set to 'false' when " +
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 BIOS, "assertsEnabled");
            }
            if ((BIOS.logsEnabled == true) && BIOS.$written("logsEnabled")) {
                BIOS.$logWarning("BIOS.logsEnabled must be set to 'false' when " +
                                 "BIOS.libType == BIOS." + libType + ".  " +
                                 "Set 'BIOS.libType = BIOS.LibType_Custom' " +
                                 "to build a custom library or update your configuration.",
                                 BIOS, "logsEnabled");
            }
            break;

        case BIOS.LibType_Custom:
            if ((BIOS.assertsEnabled == true) 
                && (Defaults.common$.diags_ASSERT == Diags.ALWAYS_OFF) 
                && (Defaults.common$.diags_INTERNAL == Diags.ALWAYS_OFF)) {
                BIOS.$logWarning("BIOS.assertsEnabled should be set to 'false' when " +
                                 "Defaults.common$.diags_ASSERT == Diags.ALWAYS_OFF.",
                                 BIOS, "assertsEnabled");
            }
            break;

        case BIOS.LibType_Debug:
        default:
            break;
    }

    /* 
     * Check that user did not write Program.cpu.clockRate; if they did, 
     * tell them to write BIOS.cpuFreq instead.
     */
    if (Program.cpu.$written("clockRate") == true) {
        BIOS.$logError("Program.cpu.clockRate should not be modified.  " + 
            "Set BIOS.cpuFreq to specify a new CPU frequency.",
                                 Program, "cpu.clockRate");
    }
    
    /* 
     * Verify that the user did not set a BIOS cpuFreq which is different from
     * what's configured in the Boot module.
     *
     * For 28x devices...
     */
    if (Program.build.target.name.match(/28/)) {
        var Boot28 = xdc.module('ti.catalog.c2800.init.Boot');

        /* Only do this check if the Boot module is configuring the PLL. */
        if (Boot28.configurePll && 
            (Boot28.getFrequency() != BIOS.cpuFreq.lo)) {
            BIOS.$logWarning("BIOS.cpuFreq does not match the frequency " +
                             "computed from the PLL configuration in the " +
                             "Boot module. Remove the BIOS.cpuFreq line " +
                             "from your .cfg script to allow the Boot " +
                             "module to set the frequency.", BIOS, 
                             "cpuFreq.lo");
        }
    }
}

/*
 *  ======== addUserStartupFunction ========
 */
function addUserStartupFunction (func)
{
    this.startupFxns.$add(func);
}

var sysbiosSources = "BIOS.c ";

var gatesSources = "gates/GateHwi.c gates/GateAll.c " +
                   "gates/GateSwi.c gates/GateTask.c " +
                   "gates/GateTest.c gates/GateMutex.c " +
                   "gates/GateMutexPri.c ";

var halSources   = "hal/Hwi.c hal/Hwi_stack.c hal/Hwi_startup.c " +
                   "hal/Timer.c hal/TimerNull.c hal/Cache.c " +
                   "hal/CacheNull.c ";

var heapsSources = "heaps/HeapMem.c " +
                   "heaps/HeapBuf.c " +
                   "heaps/HeapNull.c " +
                   "heaps/HeapMultiBuf.c ";

var ioSources    = "io/DEV.c io/GIO.c io/IOM_stub.c ";

var knlSources   = "knl/Clock.c knl/Idle.c knl/Intrinsics.c " +
                   "knl/Swi.c knl/Swi_andn.c knl/Task.c " +
                   "knl/Event.c knl/Semaphore.c knl/Mailbox.c " +
                   "knl/Queue.c ";

var miscSources  = "misc/Stats.c ";

var rtaSources   = "rta/SupportLogger.c ";

var syncsSources = "syncs/SyncSem.c syncs/SyncSwi.c " +
                   "syncs/SyncEvent.c ";

var utilsSources = "utils/Load.c ";

var xdcruntimeSources =
                   "xdcruntime/GateThreadSupport.c " +
                   "xdcruntime/GateProcessSupport.c " +
                   "xdcruntime/SemThreadSupport.c " +
                   "xdcruntime/SemProcessSupport.c " +
                   "xdcruntime/ThreadSupport.c " +
                   "xdcruntime/CacheSupport.c ";

var commonSources = sysbiosSources + knlSources + heapsSources + gatesSources + 
                    halSources + rtaSources + utilsSources + syncsSources + 
                    xdcruntimeSources + ioSources + miscSources;

var dmtimerSources = "timers/dmtimer/Timer.c " +
                     "timers/dmtimer/TimestampProvider.c ";

var gptimerSources = "timers/gptimer/Timer.c ";

var timer64Sources = "timers/timer64/Timer.c " +
                     "timers/timer64/TimestampProvider.c ";

var C62Sources  = "family/c62/TaskSupport.c " +
                  "family/c62/IntrinsicsSupport.c ";

var C66Sources =  "family/c64p/Hwi.c " +
                  "family/c64p/Hwi_startup.c " +
                  "family/c64p/TimestampProvider.c " +
                  "family/c64p/Exception.c " +
                  "family/c64p/EventCombiner.c " +
                  "family/c64p/MemoryProtect.c " +
                  "family/c66/Cache.c " +
                  "family/c66/tci66xx/CpIntc.c " +
                  "family/c64p/tci6488/TimerSupport.c " +
                  "timers/dmtimer/vayu/TimerSupport.c ";

var C674Sources = "family/c64p/Hwi.c " +
                  "family/c64p/Hwi_startup.c " +
                  "family/c64p/TimestampProvider.c " +
                  "family/c64p/Exception.c " +
                  "family/c64p/EventCombiner.c " +
                  "family/c64p/Cache.c " +
                  "family/c64p/MemoryProtect.c " +
                  "family/c64p/omap3430/TimerSupport.c " +
                  "family/c64p/omap3430/Wugen.c " +
                  "family/c64p/primus/TimerSupport.c " +
                  "family/c64p/ti81xx/TimerSupport.c " +
                  "family/c674/Power.c ";

var C67PSources = "family/c67p/Hwi.c " +
                  "family/c67p/Timer.c " +
                  "family/c67p/TimestampProvider.c " +
                  "family/c67p/Cache.c ";

var C64Sources =  "family/c64/Hwi.c " +
                  "family/c64/Timer.c " +
                  "family/c64/TimestampProvider.c ";
        
var C64PSources = "family/c64p/Hwi.c " +
                  "family/c64p/Hwi_startup.c " +
                  "family/c64p/TimestampProvider.c " +
                  "family/c64p/Exception.c " +
                  "family/c64p/EventCombiner.c " +
                  "family/c64p/Cache.c " +
                  "family/c64p/MemoryProtect.c " +
                  "family/c64p/dm6437/TimerSupport.c " +
                  "family/c64p/dm6446/TimerSupport.c " +
                  "family/c64p/omap3430/TimerSupport.c " +
                  "family/c64p/omap3430/Wugen.c " +
                  "family/c64p/tci6482/TimerSupport.c " +
                  "family/c64p/tci6488/TimerSupport.c " +
                  "family/c64p/ti81xx/TimerSupport.c ";

var C64TSources = "family/c64p/Hwi.c " + 
                  "family/c64p/Hwi_startup.c " +
                  "family/c64p/TimestampProvider.c " +
                  "family/c64p/Exception.c " +
                  "family/c64p/EventCombiner.c " +
                  "family/c64p/tesla/Power.c " +
                  "family/c64p/tesla/TimerSupport.c " +
                  "family/c64p/tesla/Wugen.c " +
                  "hal/unicache/Cache.c " +
                  "hal/ammu/AMMU.c ";

var A8FSources  = "family/arm/TaskSupport.c " +
                  "family/arm/IntrinsicsSupport.c " +
                  "family/arm/a8/Cache.c " +
                  "family/arm/a8/Mmu.c " +
                  "family/arm/a8/TimestampProvider.c " +
                  "family/arm/a8/intcps/Hwi.c " +
                  "family/arm/a8/omap3430/TimerSupport.c " +
                  "family/arm/a8/ti81xx/TimerSupport.c " +
                  "family/arm/exc/Exception.c ";

var M3Sources   = "family/arm/m3/Hwi.c " +
                  "family/arm/m3/TaskSupport.c " +
                  "family/arm/m3/Timer.c " +
                  "family/arm/m3/TimestampProvider.c " +
                  "family/arm/lm3/Timer.c " +
                  "family/arm/lm3/TimestampProvider.c " +
                  "family/arm/ducati/Core.c " +
                  "family/arm/ducati/GateDualCore.c " +
                  "family/arm/ducati/Timer.c " +
                  "family/arm/ducati/TimerSupport.c " +
                  "family/arm/ducati/TimestampProvider.c " +
                  "family/arm/ducati/Wugen.c " +
                  "family/arm/ducati/omap4430/Power.c " +
                  "family/arm/ducati/dm8148/IntMux.c " +
                  "family/arm/f28m35x/TimestampProvider.c " +
                  "hal/unicache/Cache.c " +
                  "hal/ammu/AMMU.c ";

var M4Sources   = "family/arm/m3/Hwi.c " +
                  "family/arm/m3/TaskSupport.c " +
                  "family/arm/m3/Timer.c " +
                  "family/arm/m3/TimestampProvider.c " +
                  "family/arm/lm3/Timer.c " +
                  "family/arm/lm3/TimestampProvider.c " +
                  "family/arm/ducati/Core.c " +
                  "family/arm/ducati/GateDualCore.c " +
                  "family/arm/ducati/Timer.c " +
                  "family/arm/ducati/TimerSupport.c " +
                  "family/arm/ducati/TimestampProvider.c " +
                  "family/arm/ducati/Wugen.c " +
                  "family/arm/ducati/omap4430/Power.c " +
                  "family/arm/ducati/dm8148/IntMux.c " +
                  "hal/unicache/Cache.c " +
                  "hal/ammu/AMMU.c ";

var Arm9Sources = "family/arm/TaskSupport.c " +
                  "family/arm/HwiCommon.c " +
                  "family/arm/IntrinsicsSupport.c " +
                  "family/arm/arm9/Cache.c " +
                  "family/arm/arm9/Mmu.c " +
                  "family/arm/exc/Exception.c " +
                  "family/arm/da830/TimerSupport.c " +
                  "family/arm/da830/Clobber.c " +
                  "family/arm/da830/Hwi.c " +
                  "family/arm/dm6446/TimerSupport.c " +
                  "family/arm/dm6446/Clobber.c " +
                  "family/arm/dm6446/Hwi.c " +
                  "family/arm/omap1030/TimestampProvider.c " +
                  "family/arm/omap1030/Hwi.c " +
                  "family/arm/sim1030/Timer.c " +
                  "family/arm/sim1030/TimestampProvider.c " +
                  "family/arm/sim1030/Hwi.c ";

var R4Sources =   "family/arm/TaskSupport.c " +
                  "family/arm/HwiCommon.c " +
                  "family/arm/IntrinsicsSupport.c " +
                  "family/arm/tms570/TimestampProvider.c " +
                  "family/arm/tms570/Hwi.c " +
                  "family/arm/tms570/Timer.c " +
                  "family/arm/exc/Exception.c ";

var C28Sources =  "family/c28/Hwi.c " + 
                  "family/c28/TaskSupport.c " +
                  "family/c28/Timer.c " +
                  "family/c28/TimestampProvider.c " +
                  "family/c28/f28m35x/TimestampProvider.c ";

var MSP430Sources = "family/msp430/Hwi.c " + 
                  "family/msp430/IntrinsicsSupport.c " +
                  "family/msp430/Power.c " +
                  "family/msp430/TaskSupport.c " +
                  "family/msp430/Timer.c " +
                  "family/msp430/TimestampProvider.c ";

var ARP32Sources =  "family/arp32/Hwi.c " + 
                  "family/arp32/IntrinsicsSupport.c " +
                  "family/arp32/TaskSupport.c " +
                  "family/arp32/Timer.c " +
                  "family/arp32/TimestampProvider.c ";

var cList = {
    "C28_large"    : commonSources + C28Sources,
    "C28_float"    : commonSources + C28Sources,
    "MSP430"       : commonSources + MSP430Sources,
    "MSP430X"      : commonSources + MSP430Sources,
    "MSP430X_small" : commonSources + MSP430Sources,
    "C64"          : commonSources + C62Sources + C64Sources,
    "C64P"         : commonSources + timer64Sources + dmtimerSources + 
                     gptimerSources + C62Sources + C64PSources,
    "C64P_big_endian" : commonSources + timer64Sources + dmtimerSources + 
                     gptimerSources + C62Sources + C64PSources,
    "C674"         : commonSources + timer64Sources + dmtimerSources +
                     C62Sources + C674Sources,
    "C67P"         : commonSources + C62Sources + C67PSources,
    "C64T"         : commonSources + dmtimerSources + C62Sources + C64TSources,
    "C66"          : commonSources + timer64Sources + dmtimerSources +
                     C62Sources + C66Sources,
    "C66_big_endian" : commonSources + timer64Sources +
                     C62Sources + C66Sources,
    "ARP32"        : commonSources + ARP32Sources,
    "Arm9"         : commonSources + timer64Sources + dmtimerSources + 
                     Arm9Sources,
    "A8F"          : commonSources + dmtimerSources + gptimerSources + 
                     A8FSources,
    "A8Fnv"         : commonSources + dmtimerSources + gptimerSources + 
                     A8FSources,
    "M3"           : commonSources + dmtimerSources + M3Sources,
    "M4"           : commonSources + M4Sources,
    "M4F"          : commonSources + M4Sources,
    "R4t_big_endian" : commonSources + R4Sources,
    "R4Ft_big_endian": commonSources + R4Sources,
};

/* 
 * Used for trimming the set of C files in the custom BIOS.lib 
 * to the minimum required for the application. 
 * Only odd case modules are in this list:
 * -More than just Mod.c files
 * -No Mod.c file
 */
var cFiles = {
    "ti.sysbios.hal.Hwi" : 
        { cSources: ["Hwi.c", "Hwi_stack.c", "Hwi_startup.c"] },
    "ti.sysbios.io.GIO" : 
        { cSources: ["GIO.c", "IOM_stub.c"] },
    "ti.sysbios.family.arm.m3.Clobber" : 
        { cSources: [] },
    "ti.sysbios.family.arm.m3.IntrinsicsSupport" : 
        { cSources: [] },
    "ti.sysbios.family.arm.ducati.CTM" : 
        { cSources: [] },
    "ti.sysbios.family.c28.Clobber" : 
        { cSources: [] },
    "ti.sysbios.family.c28.IntrinsicsSupport" : 
        { cSources: [] },
    "ti.sysbios.family.arp32.CTM" : 
        { cSources: [] },
    "ti.sysbios.knl.Swi" : 
        { cSources: ["Swi.c", "Swi_andn.c"] },
    "ti.sysbios.family.c64p.Hwi" : 
        { cSources: ["Hwi.c", "Hwi_startup.c"] },
    "xdc.runtime.Defaults" : 
        { cSources: [] },
    "xdc.runtime.Main" : 
        { cSources: [] },
    "xdc.runtime.Core" : 
        { cSources: ["Core-mem.c", "Core-label.c", "Core-params.c"] },
};

var biosPackages = [
    "ti.sysbios.family.arm",
    "ti.sysbios.family.arm.a8",
    "ti.sysbios.family.arm.a8.intcps",
    "ti.sysbios.family.arm.a8.omap3430",
    "ti.sysbios.family.arm.a8.sim",
    "ti.sysbios.family.arm.a8.ti81xx",
    "ti.sysbios.family.arm.arm9",
    "ti.sysbios.family.arm.da830",
    "ti.sysbios.family.arm.dm6446",
    "ti.sysbios.family.arm.ducati",
    "ti.sysbios.family.arm.ducati.dm8148",
    "ti.sysbios.family.arm.ducati.omap4430",
    "ti.sysbios.family.arm.exc",
    "ti.sysbios.family.arm.f28m35x",
    "ti.sysbios.family.arm.lm3",
    "ti.sysbios.family.arm.m3",
    "ti.sysbios.family.arm.omap1030",
    "ti.sysbios.family.arm.sim1030",
    "ti.sysbios.family.arm.tms570",
    "ti.sysbios.family.c28",
    "ti.sysbios.family.c28.f28m35x",
    "ti.sysbios.family.c62",
    "ti.sysbios.family.c64",
    "ti.sysbios.family.c64p",
    "ti.sysbios.family.c64p.dm6437",
    "ti.sysbios.family.c64p.dm6446",
    "ti.sysbios.family.c64p.omap3430",
    "ti.sysbios.family.c64p.primus",
    "ti.sysbios.family.c64p.tci6482",
    "ti.sysbios.family.c64p.tci6488",
    "ti.sysbios.family.c64p.tesla",
    "ti.sysbios.family.c64p.ti81xx",
    "ti.sysbios.family.c66",
    "ti.sysbios.family.c66.tci66xx",
    "ti.sysbios.family.c67p",
    "ti.sysbios.family.c674",
    "ti.sysbios.family.msp430",
    "ti.sysbios.family.arp32",
    "ti.sysbios.gates",
    "ti.sysbios.hal",
    "ti.sysbios.hal.ammu",
    "ti.sysbios.hal.unicache",
    "ti.sysbios.heaps",
    "ti.sysbios.io",
    "ti.sysbios.knl",
    "ti.sysbios.misc",
    "ti.sysbios.rta",
    "ti.sysbios.syncs",
    "ti.sysbios.timers.dmtimer",
    "ti.sysbios.timers.gptimer",
    "ti.sysbios.timers.timer64",
    "ti.sysbios.timers.dmtimer.vayu",
    "ti.sysbios.utils",
    "ti.sysbios.xdcruntime",
];

var asmList66 = [
    "family/c64p/Hwi_disp_always.s64P",
    "family/c64p/Hwi_asm.s62",
    "family/c64p/Hwi_asm_switch.s62",
    "family/c64p/Clobber_asm.s62",
    "family/c64p/Exception_asm.s64P",
    "family/c64p/MemoryProtect_asm.s64P",
    "family/c62/TaskSupport_asm.s62",
    "timers/timer64/Timer_asm.s64P",
    "timers/dmtimer/Timer_asm.s64P"
];

var asmList64 = [
    "family/c64/Clobber_asm.s62",
    "family/c64/Hwi_asm.s62",
    "family/c64/Hwi_asm_switch.s62",
    "family/c64/Hwi_disp_always.s64",
    "family/c64/Timer_asm.s64",
    "family/c62/TaskSupport_asm.s62"
];

var asmList64P = [
    "family/c64p/Hwi_disp_always.s64P",
    "family/c64p/Hwi_asm.s62",
    "family/c64p/Hwi_asm_switch.s62",
    "family/c64p/Clobber_asm.s62",
    "family/c64p/Exception_asm.s64P",
    "family/c64p/MemoryProtect_asm.s64P",
    "family/c62/TaskSupport_asm.s62",
    "timers/timer64/Timer_asm.s64P",
    "timers/gptimer/Timer_asm.s64P",
    "timers/dmtimer/Timer_asm.s64P"
];

var asmList67P = [
    "family/c67p/Clobber_asm.s62",
    "family/c67p/Hwi_asm.s62",
    "family/c67p/Hwi_asm_switch.s62",
    "family/c67p/Hwi_disp_always.s67P",
    "family/c67p/Timer_asm.s67P",
    "family/c62/TaskSupport_asm.s62",
];

var asmList64T = [
    "family/c64p/Hwi_disp_always.s64T",
    "family/c64p/Hwi_asm.s64T",
    "family/c64p/Hwi_asm_switch.s64T",
    "family/c64p/Clobber_asm.s64T",
    "family/c64p/Exception_asm.s64T",
    "family/c64p/tesla/Power_resume.s64T",
    "family/c64p/tesla/Power_standby.s64T",
    "family/c62/TaskSupport_asm.s64T",
    "timers/dmtimer/Timer_asm.s64T"
];

var asmListM3 = [
    "family/arm/m3/Clobber_asm.sv7M",
    "family/arm/m3/IntrinsicsSupport_asm.sv7M",
    "family/arm/m3/TaskSupport_asm.sv7M",
    "family/arm/m3/Hwi_asm.sv7M",
    "family/arm/m3/Hwi_asm_switch.sv7M",
    "family/arm/ducati/CTM_asm.sv7M",
    "family/arm/ducati/Core_asm.sv7M",
    "family/arm/ducati/omap4430/Power_saveCpu.sv7M", 
    "family/arm/ducati/omap4430/Power_resumeCpu.sv7M",
    "timers/dmtimer/Timer_asm.sv7M"
];

var asmListM4 = [
    "family/arm/m3/Clobber_asm.sv7M",
    "family/arm/m3/IntrinsicsSupport_asm.sv7M",
    "family/arm/m3/TaskSupport_asm.sv7M",
    "family/arm/m3/Hwi_asm.sv7M",
    "family/arm/m3/Hwi_asm_switch.sv7M",
    "family/arm/ducati/CTM_asm.sv7M",
    "family/arm/ducati/Core_asm.sv7M",
    "family/arm/ducati/omap4430/Power_saveCpu.sv7M", 
    "family/arm/ducati/omap4430/Power_resumeCpu.sv7M",
    "timers/dmtimer/Timer_asm.sv7M"
];

var asmListM3g = [
    "family/arm/m3/Clobber_asm_gnu.sv7M",
    "family/arm/m3/IntrinsicsSupport_asm_gnu.sv7M",
    "family/arm/m3/TaskSupport_asm_gnu.sv7M",
    "family/arm/m3/Hwi_asm_gnu.sv7M",
    "family/arm/m3/Hwi_asm_switch_gnu.sv7M",
    "family/arm/ducati/CTM_asm_gnu.sv7M",
    "family/arm/ducati/Core_asm_gnu.sv7M",
    "family/arm/ducati/omap4430/Power_saveCpu_gnu.sv7M", 
    "family/arm/ducati/omap4430/Power_resumeCpu_gnu.sv7M",
    "timers/dmtimer/Timer_asm_gnu.sv7M"
];

var asmListM4g = [
    "family/arm/m3/Clobber_asm_gnu.sv7M",
    "family/arm/m3/IntrinsicsSupport_asm_gnu.sv7M",
    "family/arm/m3/TaskSupport_asm_gnu.sv7M",
    "family/arm/m3/Hwi_asm_gnu.sv7M",
    "family/arm/m3/Hwi_asm_switch_gnu.sv7M",
    "family/arm/ducati/CTM_asm_gnu.sv7M",
    "family/arm/ducati/Core_asm_gnu.sv7M",
    "family/arm/ducati/omap4430/Power_saveCpu_gnu.sv7M", 
    "family/arm/ducati/omap4430/Power_resumeCpu_gnu.sv7M",
    "timers/dmtimer/Timer_asm_gnu.sv7M"
];

var asmListA8F = [
    "family/arm/IntrinsicsSupport_asm.asm",
    "family/arm/TaskSupport_asm.asm",
    "family/arm/exc/Exception_asm.asm",
    "family/arm/a8/TimestampProvider_asm.asm",
    "family/arm/a8/Mmu_asm.asm",
    "family/arm/a8/Cache_asm.asm",
    "family/arm/a8/intcps/Hwi_asm.s470",
    "family/arm/a8/intcps/Hwi_asm_switch.s470",
    "timers/gptimer/Timer_asm.s470",
    "timers/dmtimer/Timer_asm.s470"
];

var asmListArm9 = [
    "family/arm/IntrinsicsSupport_asm.asm",
    "family/arm/TaskSupport_asm.asm",
    "family/arm/HwiCommon_asm.asm",
    "family/arm/exc/Exception_asm.asm",
    "family/arm/da830/Hwi_asm.s470",
    "family/arm/dm6446/Hwi_asm.s470",
    "family/arm/omap1030/Hwi_asm.s470",
    "family/arm/sim1030/Hwi_asm.s470",
    "family/arm/sim1030/Clobber_asm.s470",
    "family/arm/arm9/Mmu_asm.asm",
    "family/arm/arm9/Cache_asm.asm",
    "timers/timer64/Timer_asm.s470",
    "timers/dmtimer/Timer_asm.s470"
];

var asmListR4 = [
    "family/arm/tms570/Hwi_asm.asm",
    "family/arm/tms570/TimestampProvider_asm.asm",
    "family/arm/IntrinsicsSupport_asm.asm",
    "family/arm/TaskSupport_asm.asm",
    "family/arm/HwiCommon_asm.asm",
    "family/arm/exc/Exception_asm.asm",
];

var asmList28 = [
    "family/c28/Clobber_asm.s28",
    "family/c28/Hwi_asm.s28",
    "family/c28/Hwi_disp.s28",
    "family/c28/IntrinsicsSupport_asm.s28",
    "family/c28/TaskSupport_asm.s28"
];

var asmList430 = [
    "family/msp430/TaskSupport_asm.asm",
    "family/msp430/Hwi_switch_asm.asm"
];

var asmListARP32 = [
    "family/arp32/CTM_asm.sarp32",
    "family/arp32/Hwi_disp_always.sarp32",
    "family/arp32/Hwi_switch_asm.sarp32",
    "family/arp32/TaskSupport_asm.sarp32"
];

var asmList = {
    "ti.targets.C28_large"              : asmList28, 
    "ti.targets.C28_float"              : asmList28, 
    "ti.targets.msp430.MSP430"          : asmList430, 
    "ti.targets.msp430.MSP430X"         : asmList430, 
    "ti.targets.msp430.MSP430X_small"   : asmList430, 
    "ti.targets.msp430.elf.MSP430"      : asmList430, 
    "ti.targets.msp430.elf.MSP430X"     : asmList430, 
    "ti.targets.msp430.elf.MSP430X_small" : asmList430, 
    "ti.targets.C64"                    : asmList64, 
    "ti.targets.C64P"                   : asmList64P, 
    "ti.targets.elf.C64P"               : asmList64P, 
    "ti.targets.C64P_big_endian"        : asmList64P, 
    "ti.targets.elf.C64P_big_endian"    : asmList64P, 
    "ti.targets.C674"                   : asmList64P,
    "ti.targets.elf.C674"               : asmList64P,
    "ti.targets.elf.C67P"               : asmList67P,
    "ti.targets.elf.C64T"               : asmList64T, 
    "ti.targets.elf.C66"                : asmList66,
    "ti.targets.elf.C66_big_endian"     : asmList66,
    "ti.targets.arp32.elf.ARP32"        : asmListARP32, 
    "ti.targets.elf.arp32.ARP32"        : asmListARP32,  // deprecated
    "ti.targets.arm.elf.Arm9"           : asmListArm9, 
    "ti.targets.arm.elf.A8F"            : asmListA8F, 
    "ti.targets.arm.elf.A8Fnv"          : asmListA8F, 
    "ti.targets.arm.elf.M3"             : asmListM3, 
    "ti.targets.arm.elf.M4"             : asmListM4, 
    "ti.targets.arm.elf.M4F"            : asmListM4, 
    "ti.targets.arm.elf.R4t_big_endian" : asmListR4, 
    "ti.targets.arm.elf.R4Ft_big_endian" : asmListR4, 
    "gnu.targets.arm.M3"                : asmListM3g, 
    "gnu.targets.arm.M4"                : asmListM4g, 
    "gnu.targets.arm.M4F"               : asmListM4g, 
};

/*
 *  ======== getDefs ========
 */
function getDefs()
{
    var BIOS = xdc.module("ti.sysbios.BIOS");
    var Hwi = xdc.module("ti.sysbios.hal.Hwi");
    var Settings = xdc.module("ti.sysbios.family.Settings");
    var HwiDelegate = Settings.getDefaultHwiDelegate();
    var HwiDelegateName = HwiDelegate.replace(/\./g, "_");

    var Swi = xdc.module("ti.sysbios.knl.Swi");
    var Task = xdc.module("ti.sysbios.knl.Task");
    var Clock = xdc.module("ti.sysbios.knl.Clock");
    var Semaphore = xdc.module("ti.sysbios.knl.Semaphore");
    var Defaults = xdc.module('xdc.runtime.Defaults');
    var Diags = xdc.module("xdc.runtime.Diags");

    var defs =   " -Dti_sysbios_BIOS_swiEnabled__D=" + (this.swiEnabled ? "TRUE" : "FALSE")
               + " -Dti_sysbios_BIOS_taskEnabled__D=" + (this.taskEnabled ? "TRUE" : "FALSE") 
               + " -Dti_sysbios_BIOS_clockEnabled__D=" + (this.clockEnabled ? "TRUE" : "FALSE");

    if (xdc.module(HwiDelegate).hooks.length == 0) {
        defs += " -Dti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS";
    }

    if (Swi.hooks.length == 0) {
        defs += " -Dti_sysbios_knl_Swi_DISABLE_ALL_HOOKS";
    }

    defs += " -Dti_sysbios_knl_Swi_numPriorities__D=" + Swi.numPriorities;

    if (Task.hooks.length == 0) {
        defs += " -Dti_sysbios_knl_Task_DISABLE_ALL_HOOKS";
    }

    if (BIOS.buildingAppLib == true) {
        defs += " -Dti_sysbios_knl_Task_deleteTerminatedTasks__D=" + (Task.deleteTerminatedTasks ? "TRUE" : "FALSE");
        defs += " -Dti_sysbios_knl_Task_numPriorities__D=" + Task.numPriorities;

        switch (Clock.tickSource) {
            case Clock.TickSource_TIMER:
                defs += " -Dti_sysbios_knl_Clock_TICK_SOURCE=ti_sysbios_knl_Clock_TickSource_TIMER";
                break;
            case Clock.TickSource_USER:
                defs += " -Dti_sysbios_knl_Clock_TICK_SOURCE=ti_sysbios_knl_Clock_TickSource_USER";
                break;
            case Clock.TickSource_NULL:
                defs += " -Dti_sysbios_knl_Clock_TICK_SOURCE=ti_sysbios_knl_Clock_TickSource_NULL";
                break;
        }

        if (Clock.tickMode == Clock.TickMode_DYNAMIC) {
            defs += " -Dti_sysbios_knl_Clock_TICK_MODE=ti_sysbios_knl_Clock_TickMode_DYNAMIC";
        }
        else {
            defs += " -Dti_sysbios_knl_Clock_TICK_MODE=ti_sysbios_knl_Clock_TickMode_PERIODIC";
        }
    }

    if (Hwi.dispatcherSwiSupport) {
        defs += " -D"+HwiDelegateName+"_dispatcherSwiSupport__D=TRUE";
    }
    else {
        defs += " -D"+HwiDelegateName+"_dispatcherSwiSupport__D=FALSE";
    }

    if (Hwi.dispatcherTaskSupport) {
        defs += " -D"+HwiDelegateName+"_dispatcherTaskSupport__D=TRUE";
    }
    else {
        defs += " -D"+HwiDelegateName+"_dispatcherTaskSupport__D=FALSE";
    }

    if (Hwi.dispatcherAutoNestingSupport) {
        defs += " -D"+HwiDelegateName+"_dispatcherAutoNestingSupport__D=TRUE";
    }
    else {
        defs += " -D"+HwiDelegateName+"_dispatcherAutoNestingSupport__D=FALSE";
    }

    if (Hwi.dispatcherIrpTrackingSupport) {
        defs += " -D"+HwiDelegateName+"_dispatcherIrpTrackingSupport__D=TRUE";
    }
    else {
        defs += " -D"+HwiDelegateName+"_dispatcherIrpTrackingSupport__D=FALSE";
    }

    if (Semaphore.supportsEvents) {
        defs += " -Dti_sysbios_knl_Semaphore_supportsEvents__D=TRUE";
    }
    else {
        defs += " -Dti_sysbios_knl_Semaphore_supportsEvents__D=FALSE";
    }

    if ((BIOS.assertsEnabled == false) ||
        ((Defaults.common$.diags_ASSERT == Diags.ALWAYS_OFF)
            && (Defaults.common$.diags_INTERNAL == Diags.ALWAYS_OFF))) {
        defs += " -Dxdc_runtime_Assert_DISABLE_ALL";
    }

    if (BIOS.logsEnabled == false) {
        defs += " -Dxdc_runtime_Log_DISABLE_ALL";
    }

    switch (Program.build.target.name) {
        case "C64P" :
        case "C64P_big_endian" :
        case "C64T" :
        case "C66"  :
        case "C66_big_endian"  :
        case "C674" :
        case "C67P" :
            var Exception = xdc.module("ti.sysbios.family.c64p.Exception");
            defs += 
             " -Dti_sysbios_family_c64p_Exception_enableExternalMPC__D=" 
             + (Exception.enableExternalMPC ? "TRUE" : "FALSE");
            defs += 
             " -Dti_sysbios_family_c64p_Exception_enablePrint__D=" 
             + (Exception.enablePrint ? "TRUE" : "FALSE");
            break;
        case "M3" :
        case "M4" :
        case "M4F" :
            var m3Hwi = xdc.module("ti.sysbios.family.arm.m3.Hwi");

            defs += 
             " -Dti_sysbios_family_arm_m3_Hwi_enableException__D=" 
             + (m3Hwi.enableException ? "TRUE" : "FALSE");

            if (BIOS.buildingAppLib == true) {
                defs += 
                 " -Dti_sysbios_family_arm_m3_Hwi_disablePriority__D=" 
                 + m3Hwi.disablePriority + "U";

                var GateDualCore = 
                    xdc.module("ti.sysbios.family.arm.ducati.GateDualCore");

                defs += 
                 " -Dti_sysbios_family_arm_ducati_GateDualCore_enableStats__D="
                 + (GateDualCore.enableStats ? "TRUE" : "FALSE");
            }
            break;
        case "R4t_big_endian" :
        case "R4Ft_big_endian" :
        case "Arm9" :
        case "A8Fnv" :
        case "A8F" :
            var Exception = xdc.module("ti.sysbios.family.arm.exc.Exception");
            defs += 
             " -Dti_sysbios_family_arm_exc_Exception_enableDecode__D=" 
             + (Exception.enableDecode ? "TRUE" : "FALSE");
            break;
        case "C28_large"  :
        case "C28_float"  :
            var c28Hwi = xdc.module("ti.sysbios.family.c28.Hwi");
            defs += 
             " -Dti_sysbios_family_c28_Hwi_zeroLatencyIERMask__D=" 
             + c28Hwi.zeroLatencyIERMask + "U";
            break;
    }

    return (defs);
}

/*
 *  ======== getCCOpts ========
 */
function getCCOpts(target)
{
    return (BIOS.customCCOpts);
}

/*
 *  ======== getCFiles ========
 */
function getCFiles(target)
{
    var biosSources ="";
    var xdcSources = "";

    /* 
     * logic to trim the C files down to just what the application needs 
     * 3/2/11 disabled for now ...
     */

    if (BIOS.buildingAppLib == true) {
        biosSources += "BIOS.c ";
        for each (var mod in Program.targetModules()) {
            var mn = mod.$name;
            var pn = mn.substring(0, mn.lastIndexOf("."));
            var packageMatch = false;

            /* sanity check package path */
            for (var i = 0; i < biosPackages.length; i++) {
                if (pn == biosPackages[i]) {
                    packageMatch = true;
                    break;
                }
            }

            if (packageMatch && !mn.match(/Proxy/) && !mn.match(/BIOS/)) {
                if (cFiles[mn] === undefined) {
                    var prefix = mn.substr(mn.indexOf("sysbios") + 8);
                    var mod = mn.substr(mn.lastIndexOf(".")+1);
                    prefix = prefix.substring(0, prefix.lastIndexOf('.')+1);
                    prefix = prefix.replace(/\./g, "/");
                    biosSources += prefix + mod + ".c ";
                }
                else {
                    for (i in cFiles[mn].cSources) {
                        var prefix = mn.substr(mn.indexOf("sysbios") + 8);
                        prefix = prefix.substring(0, prefix.lastIndexOf('.')+1);
                        prefix = prefix.replace(/\./g, "/");
                        biosSources += prefix + cFiles[mn].cSources[i] + " ";
                    }
                }
            }
        }
        if (Program.build.target.name.match(/430/)) {
            biosSources += "./HwiFuncs.c ";
        }
    }
    else {
        /* build with ALL sysbios C sources */
        biosSources = cList[target];
    }

    if (BIOS.includeXdcRuntime == true) {
        if (BIOS.buildingAppLib == true) {
        biosSources += "xdc/runtime/xdc_noinit.c ";
        for each (var mod in Program.targetModules()) {
            var mn = mod.$name;
            if (mn.match(/xdc\.runtime/) && !mn.match(/Proxy/)) {
                if (cFiles[mn] === undefined) {
                    var prefix = mn.substr(mn.indexOf("xdc"));
                    var mod = mn.substr(mn.lastIndexOf(".")+1);
                    prefix = prefix.substring(0, prefix.lastIndexOf('.')+1);
                    prefix = prefix.replace(/\./g, "/");
                    biosSources += prefix + mod + ".c ";
                }
                else {
                    for (i in cFiles[mn].cSources) {
                        var prefix = mn.substr(mn.indexOf("xdc"));
                        prefix = prefix.substring(0, prefix.lastIndexOf('.')+1);
                        prefix = prefix.replace(/\./g, "/");
                        biosSources += prefix + cFiles[mn].cSources[i] + " ";
                    }
                }
            }
        }
        }
        else {
            var RtsBuild = xdc.loadCapsule('xdc/runtime/build.xs');
            var KnlBuild = xdc.loadCapsule('xdc/runtime/knl/build.xs');

            var xdcObjs = RtsBuild.c_objs.concat(KnlBuild.c_objs);
            for each (var s in xdcObjs) {
                biosSources += s + " ";
            }
        }
    }

    /* remove trailing " " */
    biosSources = biosSources.substring(0, biosSources.length-1);

    return (biosSources);
}

/*
 *  ======== getAsmFiles ========
 */
function getAsmFiles(target)
{
    return (asmList[target]);
}

/*
 *  ======== viewInitModule ========
 *  Display the module properties in ROV
 */
function viewInitModule(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var BIOS = xdc.useModule('ti.sysbios.BIOS');
    var biosModConfig = Program.getModuleConfig(BIOS.$name);

    view.rtsGateType  = "" + biosModConfig.rtsGateType;
    view.cpuFreqLow   = obj.cpuFreq.lo;
    view.cpuFreqHigh  = obj.cpuFreq.hi;
    view.clockEnabled = biosModConfig.clockEnabled;
    view.swiEnabled   = biosModConfig.swiEnabled;
    view.taskEnabled  = biosModConfig.taskEnabled;
    view.startFunc    = "0x" + Number(obj.startFunc).toString(16);

    if (obj.threadType == BIOS.ThreadType_Hwi) {
        view.currentThreadType = "Hwi";
    }
    else if (obj.threadType == BIOS.ThreadType_Swi) {
        view.currentThreadType = "Swi";
    }
    else if (obj.threadType == BIOS.ThreadType_Task) {
        view.currentThreadType = "Task";
    }
    else if (obj.threadType == BIOS.ThreadType_Main) {
        view.currentThreadType = "Main";
    }
    else {
        view.currentThreadType = "undefined";
        view.$status["currentThreadType"] =
                "Error: unknown thread type for current thread.";
    }
}

/*
 *  ======== viewInitErrorScan ========
 */
function viewInitErrorScan(view)
{
    var Program = xdc.useModule('xdc.rov.Program');
    
    var elements = new Array();

    /* 
     * Verify that the current XDCtools contains the Program.getStatusTable
     * API (available in 3.22.01).
     */
    if (!('getStatusTable' in Program)) {
        displayNewError(elements, "BIOS", "Scan for errors...", "N/A", "N/A",
                        "This ROV view is only supported by XDCtools " +
                        "3.22.01 and later.");
                        
        view.elements = elements;
        return;
    }
    
    /* Run every single ROV view to gather any errors. */
    scanAllViews();

    /* 
     * Get ROV's table of status messages (built up while the views 
     * were run). 
     */
    var statusTab = Program.getStatusTable();
    
    /* Display all of the entries in the status table. */
    for each (var statusElem in statusTab) {
        displayNewStatus(elements, statusElem);
    }
    
    /* Check for any Log.L_error events in the LoggerBufs. */
    scanLoggerBufsForErrors(elements);

    /* 
     * If no errors were found, display something positive to let them no
     * it was successful.
     */
    if (elements.length == 0) {
        displayNewError(elements, "BIOS", "Scan for errors...", "N/A", "N/A",
                        "All ROV views have been run and no errors were " +
                        "encountered. In BIOS, this includes a check of the " +
                        "System stack usage and all Task stacks.");    
    }
    
    view.elements = elements;
}

/*
 *  ======== scanAllViews ========
 */
function scanAllViews()
{
    var Program = xdc.useModule('xdc.rov.Program');
    var ViewInfo = xdc.useModule('xdc.rov.ViewInfo');
    
    /* For every module in the system... */
    for each (var modName in Program.moduleNames) {
        /* Get the module descriptor object. */
        var modDesc = Program.getModuleDesc(modName);
        
        /* 
         * Skip over modules which failed to load (probably missing from
         * package path).
         */
        if (modDesc.loadFailed) {
            continue;
        }
        
        /* Get the list of views supported by this module. */
        var tabs = Program.getSupportedTabs(modName);
        
        /* For each of the module's tabs... */
        for each (var tab in tabs) {
            
            /* Don't scan ourselves or we'll end up in a loop! */
            if (tab.name == "Scan for errors...") {
                continue;
            }
            
            /* 
             * Call the appropriate scan function on the tab and catch any
             * exceptions thrown.
             */
            try {
                switch (String(tab.type)) {
                    case String(ViewInfo.MODULE):
                        Program.scanModuleView(modName, tab.name);
                        break;
                    case String(ViewInfo.INSTANCE):
                        Program.scanInstanceView(modName, tab.name);
                        break;
                    case String(ViewInfo.INSTANCE_DATA):
                        Program.scanInstanceDataView(modName, tab.name);
                        break;
                    case String(ViewInfo.MODULE_DATA):
                        Program.scanModuleDataView(modName, tab.name);
                        break;
                    case String(ViewInfo.RAW):
                        Program.scanRawView(modName);
                        break;
                    case String(ViewInfo.TREE_TABLE):
                        Program.scanTreeTableView(modName, tab.name);
                        break;
                    case String(ViewInfo.TREE):
                        Program.scanTreeView(modName, tab.name);
                        break;
                    default:
                        print("Undefined view type: " + viewType);
                        break;
                }
            }
            catch (e) {
            }
        }
    }
}

/*
 *  ======== displayNewStatus ========
 *  Display a new entry in the ROV 'Scan For Errors' table based on the
 *  provided ROV status object.
 */
function displayNewStatus(elements, status)
{
    displayNewError(elements, status.mod, status.tab, status.inst,
                    status.field, status.message);
}

/*
 *  ======== displayNewError ========
 *  Display a new entry in the ROV 'Scan For Errors' table with the provided
 *  information.
 */
function displayNewError(elements, modName, tabName, inst, field, msg)
{
    var Program = xdc.useModule('xdc.rov.Program');
    
    var elem = Program.newViewStruct('ti.sysbios.BIOS', 'Scan for errors...');
                
    elem.mod = modName;
    elem.tab = tabName;
    elem.inst = inst;
    elem.field = field;
    elem.message = msg;
                   
    elements[elements.length] = elem;               
}

/*
 *  ======== getLabel ========
 *  Get a string to represent the specified instance.
 */
function getLabel(view)
{
    /* If a 'label' was specified, just return that. */
    if ((view.label != null) && 
        (view.label != undefined) && 
        (view.label != "")) {
        return (view.label);
    }
    
    /* Otherwise, return the handle address in parentheses. */
    return ("(0x" + Number(view.address).toString(16) + ")");
}

/*
 *  ======== scanLoggerBufsForErrors ========
 *  Look through all of the LoggerBuf instances for any Log.L_error events.
 */
function scanLoggerBufsForErrors(elements)
{
    var Program = xdc.useModule('xdc.rov.Program');
    
    /* Verify LoggerBuf is in this app. */
    try {
        Program.getModuleDesc('xdc.runtime.LoggerBuf');
    }
    catch (e) {
        /* If not, just bail. */
        return;
    }
    
    /* Retrieve the LoggerBuf 'Records' view. */
    var dataViews = Program.scanInstanceDataView('xdc.runtime.LoggerBuf', 
                                                 'Records');
    
    /* Look through each of the instances. */
    for each (var view in dataViews) {
        for each (var record in view.elements) {
            if (record.eventName == "xdc.runtime.Log.L_error") {
                displayNewError(elements, "xdc.runtime.LoggerBuf", "Records",
                                view.label, "Record #" + record.serial, 
                                record.text);
            }
        }
    }
}

/*
 *  ======== _setLibType ========
 *  The "real-time" setter setLibType function
 *
 *  This function is called whenever libType changes.
 */
function _setLibType(field, val)
{
    var BIOS = this;

    if (val == BIOS.LibType_Instrumented) {
        BIOS.assertsEnabled = true;
        BIOS.logsEnabled = true;
    }
    else if (val == BIOS.LibType_NonInstrumented) {
        BIOS.assertsEnabled = false;
        BIOS.logsEnabled = false;
    }
    else if (val == BIOS.LibType_Custom) {
        BIOS.assertsEnabled = true;
        BIOS.logsEnabled = true;
    }
    else if (val == BIOS.LibType_Debug) {
        BIOS.assertsEnabled = true;
        BIOS.logsEnabled = true;
    }
    else {
        print(BIOS.$name + ": unknown libType setting: " + val);
    }
}

/*
 *  ======== _setMemoryPolicy ========
 *  Set The "real-time" setter setLibType function
 *
 *  This function is called whenever runtimeCreatesEnabled changes.
 */
function _setMemoryPolicy(field, val)
{
    var Defaults = xdc.module('xdc.runtime.Defaults');

    if (val == true) {
        Defaults.common$.memoryPolicy = xdc.module("xdc.runtime.Types").DELETE_POLICY;
    }
    else {
        Defaults.common$.memoryPolicy = xdc.module("xdc.runtime.Types").STATIC_POLICY;
    }
}

/*
 *  ======== _setLibOption ========
 */
function _setLibOption(field, val)
{
    var BIOS = this;

    /* nothing to do (yet) */
}

function _setCustomCCOpts(field, val)
{
    /* tell module$use to leave customCCOpts alone */
    this.$private.customCCOptsWritten = true;
}

/* Flag used by 'fireFrequencyUpdate'. */
var freqFromBoot = false;

/*
 *  ======== fireFrequencyUpdate ========
 *  Called from a Boot module to notify BIOS of an update in configured
 *  frequency as a result of PLL configuration.
 *
 *  Updates the BIOS cpuFreq whenever one of the PLL parameters changes.
 */
function fireFrequencyUpdate(newFreq)
{
    /* Don't do anything if the new frequency is the same as the current. */
    if (BIOS.cpuFreq.lo == newFreq) {
        return;
    }
    
    /* 
     *  If the frequency has been previously modified by the user (not by the
     *  Boot module's PLL configuration), then warn them that we are 
     *  overwriting their previous setting.
     */
    if ((freqFromBoot == false) && 
        (BIOS.cpuFreq.$written("lo") || BIOS.cpuFreq.$written("hi"))) {
        
        BIOS.$logWarning("PLL configuration is overwriting previously " +
                         "set BIOS.cpuFreq.",
                         "ti.sysbios.BIOS.cpuFreq");
    }
    
    /* 
     * Set the 'freqFromBoot' flag to indicate that the frequency has been
     * modified by the PLL configuration.
     */
    freqFromBoot = true;
    
    /* Update the BIOS frequency. */
    BIOS.cpuFreq.lo = newFreq;
}

/*
 *  @(#) ti.sysbios; 2, 0, 0, 0,573; 5-18-2012 06:03:33; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

