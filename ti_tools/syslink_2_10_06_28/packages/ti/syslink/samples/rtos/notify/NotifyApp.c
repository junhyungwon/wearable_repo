/*
 *  @file   NotifyApp.c
 *
 *  @brief      Application for testing functionality of Notify module and
 *              Notify driver
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



/*  -----------------------------------XDC.RUNTIME module Headers    */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <string.h>


/*  ----------------------------------- IPC module Headers           */
#include <ti/ipc/MultiProc.h>

/*  ----------------------------------- Notify module Headers        */
#include <ti/ipc/Notify.h>
#include <ti/ipc/Ipc.h>
/*  ----------------------------------- BIOS6 module Headers         */
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/*  ----------------------------------- To get globals from .cfg Header */
#include <xdc/cfg/global.h>


/* Macros */
#define  NOTIFYAPP_EVENT_NO       10
#define  NOTIFY_NUM_TRANSFERS     10000
/* Number of event numbers to be used for the sample application. */
#define  NOTIFYAPP_NUMEVENTS      2/* 3,4,5 Event numbers can not be used
                                          * in this sample as FrameQ,RingIO, and
                                          * FrameQBufMgr uses these event numbers
                                          * at their module startup time itself
                                          */
/* Event number to be used for sending RTOS-side information to HLOS */
#define  NOTIFYAPP_RTOSINFO_EVENT_NO  20


/* Globals */
Semaphore_Handle            NotifyApp_sem;
Error_Block                 eb;
UInt32                      count = 0;
UInt32                      sentCount = 0;
volatile UInt16             remoteProcId;

/*
 *  ======== main ========
 */
Int main(Int argc, Char* argv[])
{
    Int status = Ipc_S_SUCCESS;
    /* Create a semaphore. */
    NotifyApp_sem = Semaphore_create(0, NULL, NULL);

    do {
        /* Call Ipc_start() */
        status = Ipc_start();
    } while (status != Ipc_S_SUCCESS);
    BIOS_start();

    return (0);
}

/*
 *  ======== NotifyApp_callbackFxn ========
 */
Void NotifyApp_callbackFxn (UInt16 procId, UInt16 lineId, UInt32 eventNo,
           UArg arg, UInt32 payload)
{
    count++;
    Semaphore_post((Semaphore_Object*)arg);
}

/*
 *  ======== NotifyApp_taskFxn ========
 */
Void NotifyApp_taskFxn(UArg arg0, UArg arg1)
{
    UInt32 i;
    UInt32 j;
    UInt32 num_printfs = NOTIFY_NUM_TRANSFERS * NOTIFYAPP_NUMEVENTS / 5000;
    Int32  status = 0;

    Error_init(&eb);

    remoteProcId = (UInt16) arg0;

    do {
        status = Ipc_attach(remoteProcId);
    } while (status < 0);

    System_printf ("\nRegistering NotifyApp_callbackFxn on %s\n",
                   MultiProc_getName (MultiProc_self()));

    for (j = 0 ; j < NOTIFYAPP_NUMEVENTS ; j++) {
        Notify_registerEvent( remoteProcId,
                              0,
                             (NOTIFYAPP_EVENT_NO + j),
                             (Notify_FnNotifyCbck) NotifyApp_callbackFxn,
                             (UArg) NotifyApp_sem);
    }

    for (i = 1 ; i <= NOTIFY_NUM_TRANSFERS ; i++) {
        for (j = 0 ; j < NOTIFYAPP_NUMEVENTS ; j++) {
            Semaphore_pend(NotifyApp_sem, BIOS_WAIT_FOREVER);
            if (((count % 5000) == 0) && (num_printfs > 0)) {
                System_printf("\nCall back function executed successfully for "
                              "interrupt no %d received from %d\n",
                              count,
                              remoteProcId);
                /* printf only needs to occur at most once per 5000 callBacks */
                num_printfs--;
            }
            if ((i % 5000) == 0) {
                System_printf("\nSending interrupt number %d",i);
            }
            do {
                status = Notify_sendEvent (remoteProcId,
                                           0,
                                           (NOTIFYAPP_EVENT_NO + j),
                                           i,
                                           TRUE);
            } while (   (status == Notify_E_NOTINITIALIZED)
                      ||(status == Notify_E_EVTNOTREGISTERED)
                      ||(status == Notify_E_EVTDISABLED));
            sentCount++;
        }
    }

    System_printf("\nUnregistering NotifyApp_callbackFxn\n");

    for (j = 0 ; j < NOTIFYAPP_NUMEVENTS ; j++) {
        Notify_unregisterEvent(remoteProcId,
                               0,
                              (NOTIFYAPP_EVENT_NO + j),
                              (Notify_FnNotifyCbck)NotifyApp_callbackFxn,
                              (UArg) NotifyApp_sem);
    }

    /* Send information about number of events received/sent to the host. */
    status = Notify_sendEvent (remoteProcId,
                               0,
                               (NOTIFYAPP_RTOSINFO_EVENT_NO + MultiProc_self ()),
                               sentCount,
                               TRUE);

    /* Spin until Ipc_detach success then stop */
    do {
        status = Ipc_detach(remoteProcId);
    } while (status < 0);

    Ipc_stop();
}
