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
 *  ======== Settings.xs ========
 *
 */

var settings = {
    target: {
        "Win32": {
            hwiDelegate : "ti.sysbios.family.windows.Hwi",
            timerDelegate : "ti.sysbios.family.windows.Timer",
            timestampDelegate : "ti.sysbios.family.windows.TimestampProvider",
            taskSupportDelegate : "ti.sysbios.family.windows.TaskSupport",
            intrinsicsSupportDelegate : "ti.sysbios.family.windows.IntrinsicsSupport",
            cacheDelegate : null,
            clockTickPeriod : 30000 // microseconds (0.03 sec)
        }
    }
}

settings.target["VC98"] = settings.target["Win32"];


/*
 *  ======== targetSupportCheck ========
 *  Check validity of target
 */
function targetSupportCheck() 
{
    for (target in settings.target) {
        if (target == Program.build.target.name) {
            return;
        }
    }
    print("The " + Program.build.target.name
        + " target is not currently supported.");
    print("The following windows targets are supported:");
    for (target in settings.target) {
        print("\t" + target);
    }
    throw new Error("Unsupported Target!");
}

/*
 *  ======== getDefaultHwiDelegate ========
 */
function getDefaultHwiDelegate()
{
    print("warning: Windows native support will be removed in a future version of SYS/BIOS.\n");

    targetSupportCheck();
    return (settings.target[Program.build.target.name].hwiDelegate);
}

/*
 *  ======== getDefaultTimerDelegate ========
 */
function getDefaultTimerDelegate()
{
    targetSupportCheck();
    return(settings.target[Program.build.target.name].timerDelegate);
}

/*
 *  ======== getDefaultTimestampDelegate ========
 */
function getDefaultTimestampDelegate()
{
    targetSupportCheck();
    return(settings.target[Program.build.target.name].timestampDelegate);
}

/*
 *  ======== getDefaultTaskSupportDelegate ========
 */
function getDefaultTaskSupportDelegate()
{
    targetSupportCheck();
    return(settings.target[Program.build.target.name].taskSupportDelegate);
}

/*
 *  ======== getDefaultIntrinsicsSupportDelegate ========
 */
function getDefaultIntrinsicsSupportDelegate()
{
    targetSupportCheck();
    return(settings.target[Program.build.target.name].intrinsicsSupportDelegate);
}

/*
 *  ======== getDefaultCacheDelegate ========
 */
function getDefaultCacheDelegate()
{
    targetSupportCheck();
    return(settings.target[Program.build.target.name].cacheDelegate);
}

/*
 *  ======== getDefaultClockTickPeriod ========
 */
function getDefaultClockTickPeriod()
{
    targetSupportCheck();
    return(settings.target[Program.build.target.name].clockTickPeriod);
}

/*
 *  ======== getDefaultBootModule ========
 */
function getDefaultBootModule()
{
    return (null);
}

/*
 *  ======== getDefaultMmuModule ========
 */
function getDefaultMmuModule()
{
    return (null);
}

/*
 *  ======== getFamilySettingsXml ========
 */
function getFamilySettingsXml()
{
    return("family/windows/Settings.xml");
}
/*
 *  @(#) ti.sysbios.family.windows; 2, 0, 0, 0,553; 5-18-2012 06:05:19; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

