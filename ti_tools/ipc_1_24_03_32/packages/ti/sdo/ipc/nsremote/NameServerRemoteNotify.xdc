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
 *  ======== NameServerRemoteNotify.xdc ========
 */
 
import xdc.runtime.Error;
import xdc.runtime.Assert;

import xdc.rov.ViewInfo;

import ti.sysbios.knl.Swi;
import ti.sysbios.knl.Semaphore;
import ti.sdo.ipc.GateMP;
import ti.sdo.utils.INameServerRemote;

/*!
 *  ======== NameServerRemoteNotify ========
 *  Used by NameServer to communicate to remote processors.
 *
 *  This module is used by {@link #ti.sdo.utils.NameServer} to communicate
 *  to remote processors using {@link ti.sdo.ipc.Notify} and shared memory.
 *  There needs to be one instance between each two cores in the system.
 *  Interrupts must be enabled before using this module.  For critical
 *  memory management, a GateMP {@link #gate} can be specified.  Currently
 *  supports transferring up to 300-bytes between two cores.
 */
@InstanceInitError
@InstanceFinalize

module NameServerRemoteNotify inherits INameServerRemote
{
    /*! @_nodoc */
    metaonly struct BasicView {
        UInt        remoteProcId;
        String      remoteProcName;
        String      localRequestStatus;
        String      localInstanceName;
        String      localName;
        String      localValue;
        String      remoteRequestStatus;
        String      remoteInstanceName;
        String      remoteName;
        String      remoteValue;
    }
    
    /*!
     *  ======== rovViewInfo ========
     */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
                ['Basic',
                    {
                        type: ViewInfo.INSTANCE,
                        viewInitFxn: 'viewInitBasic',
                        structName: 'BasicView'
                    }
                ],
            ]
        });

    /* structure in shared memory for retrieving value */
    struct Message {
        Bits32  request;            /* if this is a request set to 1    */
        Bits32  response;           /* if this is a response set to 1   */
        Bits32  requestStatus;      /* if request sucessful set to 1    */
        Bits32  value;              /* holds value if len <= 4          */
        Bits32  valueLen;           /* len of value                     */
        Bits32  instanceName[8];    /* name of NameServer instance      */
        Bits32  name[8];            /* name of NameServer entry         */
        Bits32  valueBuf[75];       /* supports up to 300-byte value    */
    };

    /*!
     *  Assert raised when length of value larger then 300 bytes.
     */
    config xdc.runtime.Assert.Id A_invalidValueLen =
        {msg: "A_invalidValueLen: Invalid valueLen (too large)"};

    /*!
     *  ======== notifyEventId ========
     *  The Notify event ID.
     */
    config UInt notifyEventId = 4;
    
    /*!
     *  ======== timeoutInMicroSecs ========
     *  The timeout value in terms of microseconds
     *
     *  A NameServer request will return after this amout of time
     *  without a response. The default timeout value is to wait forever.
     *  To not wait, use the value of '0'.
     */
    config UInt timeoutInMicroSecs = ~(0);

instance:

    /*!
     *  ======== sharedAddr ========
     *  Physical address of the shared memory
     *
     *  The shared memory that will be used for maintaining shared state
     *  information.  This value must be the same for both processors when
     *  creating the instance between a pair of processors.
     */
    config Ptr sharedAddr = null;

    /*!
     *  ======== gate ========
     *  GateMP used for critical region management of the shared memory
     *
     *  Using the default value of NULL will result in the default GateMP
     *  being used for context protection.
     */
    config GateMP.Handle gate = null;

internal:    

    /*
     *  ======== timeout ========
     *  The timeout value to pass into Semaphore_pend
     *
     *  This value is calculated based on timeoutInMicroSecs and the
     *  SYSBIOS clock.tickPeriod.
     */
    config UInt timeout;
    
    /*!
     *  ======== cbFxn ========
     *  The call back function registered with Notify.
     *
     *  This function is registered with Notify as a call back function
     *  when the specified event is triggered.  This function simply posts
     *  a Swi which will process the event.
     *
     *  @param(procId)          Source proc id
     *  @param(lineId)          Interrupt line id
     *  @param(eventId)         the Notify event id.
     *  @param(arg)             the argument for the function.
     *  @param(payload)         a 32-bit payload value.
     */
    Void cbFxn(UInt16 procId, UInt16 lineId, UInt32 eventId, UArg arg, 
               UInt32 payload);
    
    /*!
     *  ======== swiFxn ========
     *  The swi function that will be executed during the call back.
     *
     *  @param(arg)     argument to swi function
     */
    Void swiFxn(UArg arg);
    
    /* instance state */
    struct Instance_State {
        Message             *msg[2];        /* Ptrs to messages in shared mem */
        UInt16              regionId;       /* SharedRegion ID                */
        GateMP.Handle       gate;           /* remote and local gate protect  */
        Semaphore.Object    semRemoteWait;  /* sem to wait on remote proc     */
        Semaphore.Object    semMultiBlock;  /* sem to block multiple threads  */
        Swi.Object          swiObj;         /* instance swi object            */
        UInt16              remoteProcId;   /* remote MultiProc id            */
        Bool                cacheEnable;    /* cacheability                   */
    };
}
/*
 *  @(#) ti.sdo.ipc.nsremote; 1, 0, 0, 0,1; 5-22-2012 16:21:31; /db/vtree/library/trees/ipc/ipc-h32/src/ xlibrary

 */

