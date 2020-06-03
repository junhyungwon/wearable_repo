/*
 *  @file   NotifyApp.c
 *
 *  @brief      Sample application for Notify module
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



/* Standard headers */
#include <ti/syslink/Std.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/syslink/utils/OsalSemaphore.h>

#include <ti/syslink/ProcMgr.h>

/* Module level headers */
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>

/* Ipc header */
#include <ti/ipc/Ipc.h>
#include <ti/syslink/IpcHost.h>

#if !defined(__KERNEL__)
#include <ti/syslink/samples/hlos/slaveLoader/SlaveLoader.h>
#endif /* if !defined(__KERNEL__) */
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>
#include "NotifyApp.h"


/** ============================================================================
 *  Globals
 *  ============================================================================
 */

/*!
 *  @brief  Received event count for remote notifications.
 */
UInt32 NotifyApp_recvEventCount
        [SysLinkSamples_MAXPROCESSORS][NOTIFYAPP_NUMEVENTS];

/* Semaphore Handle to sync with slave cores  */
OsalSemaphore_Handle  NotifyApp_semSyncHandle = NULL;

#if !defined(__KERNEL__)
extern String  NotifyApp_filePath [SysLinkSamples_MAXPROCESSORS];
extern ProcMgr_BootMode  NotifyApp_bootMode;
extern UInt32            NotifyApp_resetVector[];
#else/* if !defined(__KERNEL__) */
ProcMgr_BootMode         NotifyApp_bootMode = ProcMgr_BootMode_Boot;
UInt32                   NotifyApp_resetVector[SysLinkSamples_MAXPROCESSORS] = {0};
#endif /* if !defined(__KERNEL__) */
extern UInt16  NotifyApp_numProcs;
extern UInt16  NotifyApp_procId   [SysLinkSamples_MAXPROCESSORS];
extern Bool    NotifyApp_runAll;

UInt32 SysLogging_UserCallbackCount = 0;

/** ============================================================================
 *  Functions
 *  ============================================================================
 */
Void
NotifyApp_rtosInfocallback (UInt16       procId,
                            UInt16       lineId,
                            UInt32       eventNo,
                            Void *       arg,
                            UInt32       payload)
{
    Osal_printf ("Number of RTOS-side received/sent events:\n"
             "    procId: [%d]\n"
             "    Number of events: [%d]\n",
             procId,
             payload);

    if (payload != (NOTIFYAPP_NUMEVENTS * NOTIFYAPP_NUM_TRANSFERS)) {
        Osal_printf ("*****Failure on RTOS-side*****\n"
                 "    Expected num events: [%d]\n"
                 "    Received num events: [%d]\n",
                 (NOTIFYAPP_NUMEVENTS * NOTIFYAPP_NUM_TRANSFERS),
                 payload);
    }
    OsalSemaphore_post ((OsalSemaphore_Handle) arg);
}


/*!
 *  @brief  Callback function for receiving events.
 *
 *  @param  procId  Processor ID from which the event is received
 *  @param  eventNo Event number received
 *  @param  arg     Argument associated with the event number
 *  @param  payload Payload received with the event
 *
 *  @sa     NotifyApp_execute
 */
Void
NotifyApp_callback (UInt16       procId,
                    UInt16       lineId,
                    UInt32       eventNo,
                    Void *       arg,
                    UInt32       payload)
{
    UInt32 eventCount;
    Int eventIdx;

    SysLogging_UserCallbackCount++;

    eventIdx = eventNo - NOTIFYAPP_EVENT_NO;
    eventCount = ++NotifyApp_recvEventCount[procId][eventIdx];

#ifdef DEBUG_NOTIFY_SAMPLE
    Osal_printf("Received %d events for event ID %d from processor %d\n",
        eventCount, eventNo, procId);
#else
    if ((eventCount % NOTIFY_PRINT_COUNT) == 0) {
        Osal_printf("Received %d events for event ID %d from processor %d\n",
            eventCount, eventNo, procId);
    }
#endif

    /* Verify received parameters and flag if error. */
    if (procId >= MultiProc_getNumProcessors ()) {
        Osal_printf ("*****Data integrity mismatch*****\n"
                 "    Expected procId: < [%d]\n"
                 "    Received procId: [%d]\n",
                 MultiProc_getNumProcessors (),
                 procId);
    }
    else if (lineId != NOTIFYAPP_LINEID) {
        Osal_printf ("*****Data integrity mismatch*****\n"
                 "    Expected lineId: [%d]\n"
                 "    Received lineId: [%d]\n",
                 NOTIFYAPP_LINEID,
                 lineId);
    }
    else if (   (eventNo < NOTIFYAPP_EVENT_NO)
             || (eventNo >= (NOTIFYAPP_EVENT_NO + NOTIFYAPP_NUMEVENTS))) {
        Osal_printf ("*****Data integrity mismatch*****\n"
                 "    Expected eventNo: between [%d] & [%d]\n"
                 "    Received eventNo: [%d]\n",
                 NOTIFYAPP_EVENT_NO,
                 NOTIFYAPP_EVENT_NO + NOTIFYAPP_NUMEVENTS - 1,
                 eventNo);
    }
    else if ((UInt32) arg != NOTIFYAPP_ARGVALUE) {
        Osal_printf ("*****Data integrity mismatch*****\n"
                 "    Expected arg: [0x%x]\n"
                 "    Received arg: [0x%x]\n",
                 NOTIFYAPP_ARGVALUE,
                 arg);
    }
    else if (payload != eventCount) {
        Osal_printf ("*****Data integrity mismatch*****\n"
                 "    Event ID:         [%d]\n"
                 "    Expected payload: [%d]\n"
                 "    Received payload: [%d]\n",
                 eventNo, eventCount, payload);

        /* fix the count to limit error messages */
        NotifyApp_recvEventCount[procId][eventIdx] = payload;
    }
}


/*!
 *  @brief  Function to execute the startup for NotifyApp sample application
 *
 *  @sa     NotifyApp_callback
 */
Int NotifyApp_startup(String mapFile)
{
    Int     status  = 0;
    UInt32  i;
    UInt32  eventId;
    Ptr arg;
    
    Osal_printf ("Entered NotifyApp_startup. Number of processors: %d\n",
                 MultiProc_getNumProcessors());

    /* Set the slaves to run the application with. */
    status = SysLinkSamples_setToRunProcIds(NotifyApp_numProcs,
            NotifyApp_procId, NotifyApp_runAll);

#if !defined(__KERNEL__)
    for (i = 0; i < NotifyApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId (NotifyApp_procId [i])) {
            if (NotifyApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Loading and starting procId [%d] with [%s]\n",
                        NotifyApp_procId[i], NotifyApp_filePath[i]);
                status = SlaveLoader_startup(NotifyApp_procId[i], "startup",
                        NotifyApp_filePath[i], mapFile);
                Osal_printf("SlaveLoader_startup status [0x%08x]\n", status);
            }
        }
    }
#endif

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (NotifyApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL;
            }
            else {
                arg = (Ptr)&NotifyApp_resetVector[i];
            }
            /* Call Ipc_control with reset vector for 
             * ProcMgr_BootMode_NoLoad_Pwr,
             * ProcMgr_BootMode_NoLoad_NoPwr
             * and ProcMgr_BootMode_NoBoot modes
             */
            status = Ipc_control(i, 
                Ipc_CONTROLCMD_LOADCALLBACK, arg);

            if (status < 0) {
                Osal_printf ("Error in Ipc_control "
                             "Ipc_CONTROLCMD_LOADCALLBACK [0x%x]\n",
                             status);
            }
            else {
                Osal_printf ("After Ipc_loadcallback:\n"
                             "    status [0x%x]\n",
                             status);
            }
            if (status >= 0) {
                status = Ipc_control(i, Ipc_CONTROLCMD_STARTCALLBACK, NULL);
                if (status < 0) {
                    Osal_printf ("Error in Ipc_control "
                            "Ipc_CONTROLCMD_STARTCALLBACK[0x%x]\n", status);
                }
                else {
                    Osal_printf ("After Ipc_startcallback: \n"
                            "    status [0x%x]\n", status);
                }
            }
        }
    }

    if (status >= 0) {
        /* Create semaphore for application synchronization */
        NotifyApp_semSyncHandle =
                OsalSemaphore_create(OsalSemaphore_Type_Counting);
        if (NotifyApp_semSyncHandle == NULL) {
            Osal_printf ( "Error: Failed OsalSemaphore_create \n");
            status = -1;
        }
    }

    if (status >= 0) {
        /* Register for events from local as well as remote processors */
        for (i = 0; (i < MultiProc_getNumProcessors ()) && (status >= 0); i++) {
            if ((SysLinkSamples_toRunProcId (i)) || (i == MultiProc_self ())) {
                if (i != MultiProc_self ()) {
                    /* Register event to receive information from RTOS-side */
                    status = Notify_registerEvent (i,
                               NOTIFYAPP_LINEID,
                               (NOTIFYAPP_RTOSINFO_EVENT_NO + i),
                               (Notify_FnNotifyCbck) NotifyApp_rtosInfocallback,
                               (Ptr) NotifyApp_semSyncHandle);
                }
                for (eventId = 0 ; eventId < NOTIFYAPP_NUMEVENTS ; eventId++) {
                    NotifyApp_recvEventCount[i][eventId] = 0;
                    status = Notify_registerEvent (i,
                                       NOTIFYAPP_LINEID,
                                       (NOTIFYAPP_EVENT_NO + eventId),
                                       (Notify_FnNotifyCbck) NotifyApp_callback,
                                       (Ptr) NOTIFYAPP_ARGVALUE);
                    if (status < 0) {
                        Osal_printf ("Error in Notify_registerEvent %d"
                                     " status [0x%x] for processor %d\n",
                                     eventId,
                                     status,
                                     i);
                    }
                    else {
                        Osal_printf ("Registered event number %d with"
                                     " Notify module for processor %d\n",
                                     (NOTIFYAPP_EVENT_NO + eventId),
                                     i);
                    }
                }
            }
        }
    }

    Osal_printf("Leaving NotifyApp_startup() (0x%x)\n", status);

    return (status);
}


/*!
 *  @brief  Function to execute the NotifyApp sample application
 *
 *  @sa     NotifyApp_callback
 */
Int NotifyApp_execute(Void)
{
    Int32  status = 0;
    /* Consider all processors including host processor for notify */
    UInt32 i = 0;
    UInt32 j;
    UInt16 procId;

    Osal_printf("Entered NotifyApp_execute()\n");

    for (procId = 0;
         (procId < MultiProc_getNumProcessors ()) && (status >= 0);
         procId++) {
        if (   (SysLinkSamples_toRunProcId (procId))
            || (procId == MultiProc_self ())) {
            Osal_printf ("\nSending events to procId %d\n", procId);
            for (i = 1 ; i <= NOTIFYAPP_NUM_TRANSFERS ; i++) {
                for (j = 0 ; j < NOTIFYAPP_NUMEVENTS ; j++) {
                    do {
                        status = Notify_sendEvent (procId,
                                                   NOTIFYAPP_LINEID,
                                                   (NOTIFYAPP_EVENT_NO + j),
                                                   i,
                                                   TRUE);
                    } while (    (status == Notify_E_NOTINITIALIZED)
                              || (status == Notify_E_EVTNOTREGISTERED)
                              || (status == Notify_E_EVTDISABLED));

                    if (status < 0) {
                        Osal_printf("Error in Notify_sendEvent: procId=%d, "
                            "status=0x%08x\n", procId, status);
                        break;
                    }
                    else {
#ifdef DEBUG_NOTIFY_SAMPLE
    Osal_printf("Sent %d events to event ID %d to processor %d\n",
        i, NOTIFYAPP_EVENT_NO + j, procId);

#else
                        if ((i % NOTIFY_PRINT_COUNT) == 0) {
                            Osal_printf("Sent %d events to event ID %d to "
                                "processor %d\n", i, NOTIFYAPP_EVENT_NO + j,
                                 procId);
                        }
#endif
                    }
                }
            }
        }
    }

    if (status >= 0) {
        for (i = 0; i < MultiProc_getNumProcessors (); i++) {
            if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
                Osal_printf("Waiting for notification from processor %d\n", i);
                OsalSemaphore_pend (NotifyApp_semSyncHandle,
                                    OSALSEMAPHORE_WAIT_FOREVER);
            }
        }
    }

    Osal_printf ("Leaving NotifyApp_execute\n");

    return(0);
}


/*!
 *  @brief  Function to execute the shutdown for NotifyApp sample application
 *
 *  @sa     NotifyApp_shutdown
 */
Int NotifyApp_shutdown(String mapFile)
{
    Int32 status = 0;
    UInt32 i;
    UInt32 eventId;
    Ptr arg;
    
    Osal_printf("Entered NotifyApp_shutdown\n");

    Osal_printf("Total notifications received: %d\n",
                 SysLogging_UserCallbackCount);

    /* Unregister for events from local as well as remote processors */
    for (i = 0; i < MultiProc_getNumProcessors (); i++) {
        if ((SysLinkSamples_toRunProcId (i)) || (i == MultiProc_self ())) {
            if (i != MultiProc_self ()) {
                /* Unregister event to receive information from RTOS-side */
                status = Notify_unregisterEvent (i,
                           NOTIFYAPP_LINEID,
                           (NOTIFYAPP_RTOSINFO_EVENT_NO + i),
                           (Notify_FnNotifyCbck) NotifyApp_rtosInfocallback,
                           (Ptr) NotifyApp_semSyncHandle);
            }
            for (eventId = 0; eventId < NOTIFYAPP_NUMEVENTS; eventId++) {
                status = Notify_unregisterEvent (i, NOTIFYAPP_LINEID,
                        (NOTIFYAPP_EVENT_NO + eventId),
                        (Notify_FnNotifyCbck)NotifyApp_callback,
                        (Ptr)NOTIFYAPP_ARGVALUE);
                if (status < 0) {
                    Osal_printf ("Error in Notify_unregisterEvent %d"
                            " status [0x%x] for processor %d\n",
                            eventId, status, i);
                }
                else {
                    Osal_printf ("Unregistered event number %d with"
                            " Notify module for processor %d\n",
                            (NOTIFYAPP_EVENT_NO + eventId), i);
                }
            }
        }
    }

    /* Delete the semaphore created for application synchronization */
    status = OsalSemaphore_delete (&(NotifyApp_semSyncHandle));
    Osal_printf("OsalSemaphore_delete status [%d]\n", status);

    for (i = 0; i < MultiProc_getNumProcessors(); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (NotifyApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL; 
            }
            else {
                arg = (Ptr)&NotifyApp_resetVector[i];
            }
            /* Call Ipc_control with resetVector for 
             * ProcMgr_BootMode_NoLoad_Pwr,
             * ProcMgr_BootMode_NoLoad_NoPwr
             * and ProcMgr_BootMode_NoBoot modes
             */
            status = Ipc_control(i, 
                Ipc_CONTROLCMD_STOPCALLBACK, arg);
            Osal_printf ("After Ipc_stopCallback status: [0x%x]\n", status);
        }
    }

#if !defined(__KERNEL__)
    for (i = 0; i < NotifyApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId (NotifyApp_procId [i])) {
            if (NotifyApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Shutting down procId [%d]\n",
                        NotifyApp_procId[i]);
                status = SlaveLoader_shutdown(NotifyApp_procId[i], "shutdown",
                        mapFile);
                Osal_printf("SlaveLoader_shutdown status [%d]\n", status);
            }
        }
    }
#endif

    Osal_printf("Leaving NotifyApp_shutdown\n");

    return(0);
}
