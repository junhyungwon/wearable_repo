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
 *  ======== NotifyCircSetup.xdc ========
 *
 */
 
import ti.sdo.utils.MultiProc;

/*!
 *  ======== NotifyCircSetup ======== 
 *  Notify setup proxy for ARP32
 *
 *  This module creates and registers all drivers necessary for inter-processor
 *  notification on ARP32. 
 */

module NotifyCircSetup inherits ti.sdo.ipc.interfaces.INotifySetup
{
    /*!
     *  Interrupt vector id for DSP.
     */
    config UInt dspIntVectId = 5;
    
    /*!
     *  Interrupt vector id for ARP32.
     */
    config UInt arp32IntVectId = 4;

internal:

    config UInt arp32ProcId     = MultiProc.INVALIDID;
    config UInt dspProcId       = MultiProc.INVALIDID;
}
/*
 *  @(#) ti.sdo.ipc.family.arctic; 1, 0, 0, 0,1; 5-22-2012 16:18:58; /db/vtree/library/trees/ipc/ipc-h32/src/ xlibrary

 */

