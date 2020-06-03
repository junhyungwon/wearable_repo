/** 
 *  @file   SysLinkSamples_common.cfg.xs
 *
 *  @brief      xs file
 *
 *
 *  ============================================================================
 *
 *  Copyright (c) 2008-2012, Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  
 *  *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  
 *  *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  
 *  *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  Contact information for paper mail:
 *  Texas Instruments
 *  Post Office Box 655303
 *  Dallas, Texas 75265
 *  Contact information: 
 *  http://www-k.ext.ti.com/sc/technical-support/product-information-centers.htm?
 *  DCMP=TIHomeTracking&HQS=Other+OT+home_d_contact
 *  ============================================================================
 *  
 */



var Ipc                     = xdc.useModule('ti.sdo.ipc.Ipc');
var BIOS                    = xdc.useModule('ti.sysbios.BIOS');
var MultiProc               = xdc.useModule('ti.sdo.utils.MultiProc');
var SharedRegion            = xdc.useModule('ti.sdo.ipc.SharedRegion');
var Syslink                 = xdc.useModule ('ti.syslink.ipc.rtos.Syslink');

xdc.loadPackage ('ti.syslink.utils.rtos.bios6');

if ( (Program.platform.$name != undefined) && (Program.platform.$name.match(/evmDA830/)) ) {
    /* Ipc */
    Ipc.sr0MemorySetup = true;
}

/* Ipc configuration */
if ( (Program.platform.$name != undefined) && (Program.platform.$name.match(/evm6472/)) ) {
    /* Set host id */
    Ipc.hostProcId = MultiProc.getIdMeta("CORE5");
    Ipc.generateSlaveDataForHost = true;
}
else if ( (Program.platform.$name != undefined) && (Program.platform.$name.match(/evm6474/)) ) {
    /* Set host id */
    Ipc.hostProcId = MultiProc.getIdMeta("CORE2");
    Ipc.generateSlaveDataForHost = true;
}
else {
    /* Set host id */
    Ipc.hostProcId = MultiProc.getIdMeta("HOST");
}

Ipc.procSync = Ipc.ProcSync_PAIR;
/* To avoid wasting shared memory for MessageQ transports */
for (var i=0; i < MultiProc.numProcessors; i++) {
    Ipc.setEntryMeta({
        remoteProcId: i,
    });
}

/* Set Shared Region variables by picking up the information from Platform
 * memory map
 */
var sr0MemSection = Program.cpu.memoryMap['SR0'];
var SHAREDREG_0_MEM     = sr0MemSection.base;
var SHAREDREG_0_MEMSIZE = sr0MemSection.len;
var SHAREDREG_0_ENTRYID = 0;
var SHAREDREG_0_OWNERPROCID = Ipc.hostProcId;

var sr1MemSection = Program.cpu.memoryMap['SR1'];
var SHAREDREG_1_MEM     = sr1MemSection.base;
var SHAREDREG_1_MEMSIZE = sr1MemSection.len;
var SHAREDREG_1_ENTRYID = 1;
var SHAREDREG_1_OWNERPROCID = Ipc.hostProcId;

/*
 *  Need to define the shared region. The IPC modules use this
 *  to make portable pointers. All processors need to add this
 *  call with their base address of the shared memory region.
 *  If the processor cannot access the memory, do not add it.
 */
SharedRegion.setEntryMeta(SHAREDREG_0_ENTRYID,
    {
      base:        SHAREDREG_0_MEM,
      len:         SHAREDREG_0_MEMSIZE,
      ownerProcId: SHAREDREG_0_OWNERPROCID,
      isValid:     true,
      name:        "shared_region_0",
    });

SharedRegion.setEntryMeta(SHAREDREG_1_ENTRYID,
    {
      base:        SHAREDREG_1_MEM,
      len:         SHAREDREG_1_MEMSIZE,
      ownerProcId: SHAREDREG_1_OWNERPROCID,
      isValid:     true,
      name:        "shared_region_1",
      createHeap:  true
    });
