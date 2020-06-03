/*
 *  @file   GateMPApp.c
 *
 *  @brief      Sample application for MessageQ module
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
#include <ti/syslink/utils/String.h>
#include <ti/syslink/utils/Cache.h>
#include <ti/syslink/utils/OsalSemaphore.h>

#include <ti/syslink/ProcMgr.h>

/* Module level headers */
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/GateMP.h>
#include <ti/ipc/Notify.h>

/* Ipc header */
#include <ti/ipc/Ipc.h>
#include <ti/syslink/IpcHost.h>

#if !defined(__KERNEL__)
#include <ti/syslink/samples/hlos/slaveLoader/SlaveLoader.h>
#else
#include <OsalThread.h>
#endif /* if !defined(__KERNEL__) */
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>



/* Logic:
    Take the lock
    Each processor reads 1000 bytes (shared memory), all bytes should be same.
    If there is a mismatch, then it is Gate Bug.
    After reading, each processor write same value to every byte.
    and release the lock.
    Do this n times.
 */
#define NUM_ITERATIONS      10000
#define BYTES_TO_PROCESS    10000
#define STAMP               0xBABA0000
#define ERROR_NOTIFY_EVENT  10
#define SYNC_NOTIFY_EVENT   12
#define NOTIFY_LINEID       0
#define TAGID               0xABCD1234
/*!
 *  @brief  Shared Region ID
 */
#define APP_SHAREDREGION_ENTRY_ID   0u


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
#if !defined(__KERNEL__)
extern String  GateMPApp_filePath [];
extern ProcMgr_BootMode  GateMPApp_bootMode;
extern UInt32            GateMPApp_resetVector[];
#else/* if !defined(__KERNEL__) */
ProcMgr_BootMode         GateMPApp_bootMode = ProcMgr_BootMode_Boot;
UInt32                   GateMPApp_resetVector[SysLinkSamples_MAXPROCESSORS] = {0};
#endif /* if !defined(__KERNEL__) */
extern UInt16  GateMPApp_numProcs;
extern UInt16  GateMPApp_procId   [];
extern Bool    GateMPApp_runAll;

/* Semaphore Handle to sync with slave cores  */
OsalSemaphore_Handle  GateMPApp_semSyncHandle;


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Callback function for synch with slave cores.
 *
 *  @param  procId  Processor ID from which the event is received
 *  @param  eventNo Event number received
 *  @param  arg     Argument associated with the event number
 *  @param  payload Payload received with the event
 */
Void
GateMPApp_sync_callback (UInt16       procId,
                    UInt16       lineId,
                    UInt32       eventNo,
                    Void *       arg,
                    UInt32       payload)
{
    /* Received sync notify */
    OsalSemaphore_post (GateMPApp_semSyncHandle);
}


/*!
 *  @brief  Callback function for receiving events.
 *
 *  @param  procId  Processor ID from which the event is received
 *  @param  eventNo Event number received
 *  @param  arg     Argument associated with the event number
 *  @param  payload Payload received with the event
 */
Void
GateMPApp_callback (UInt16       procId,
                    UInt16       lineId,
                    UInt32       eventNo,
                    Void *       arg,
                    UInt32       payload)
{
    /* Received error */
    Osal_printf ("********************* BUG! **************************\n");
    Osal_printf ("Encountered error on processor %d and stamp value: 0x%x\n",
                 procId,
                 payload);
    Osal_printf ("********************* BUG! **************************\n");
}


/*!
 *  @brief  Function to execute the startup for GateMPApp sample application
 *
 *  @sa
 */
Int GateMPApp_startup(String mapFile)
{
    Int status = 0;
    UInt32 i;
    Ptr arg;
    
    Osal_printf("Entered GateMPApp_startup\n");

    /* Set the slaves to run the application with. */
    status = SysLinkSamples_setToRunProcIds(GateMPApp_numProcs,
            GateMPApp_procId, GateMPApp_runAll);

#if !defined(__KERNEL__)
    for (i = 0; i < GateMPApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(GateMPApp_procId[i])) {
            if (GateMPApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Loading and starting procId [%d] with [%s]\n",
                        GateMPApp_procId[i], GateMPApp_filePath[i]);
                status = SlaveLoader_startup(GateMPApp_procId[i],
                        "startup", GateMPApp_filePath[i], mapFile);
                Osal_printf("SlaveLoader_startup status [%d]\n", status);
            }
        }
    }
#endif

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (GateMPApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL;
            }
            else {
                arg = (Ptr)&GateMPApp_resetVector[i];
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
        for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
            if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
                status = Notify_registerEvent (i, NOTIFY_LINEID,
                        ERROR_NOTIFY_EVENT,
                        (Notify_FnNotifyCbck) GateMPApp_callback, NULL);
                if (status < 0) {
                    Osal_printf ("Registering error event failed\n");
                }
            }
        }

        /* Incase of Notify Registration fails. Safely Unregister the registered
         * Notify Events
         */
        if (status < 0) {
            for (i = (i - 1) ; i >= 0; i--) {
                if (SysLinkSamples_toRunProcId(i)) {
                    status = Notify_unregisterEvent(i, NOTIFY_LINEID,
                            ERROR_NOTIFY_EVENT,
                            (Notify_FnNotifyCbck)GateMPApp_callback, NULL);
                    GT_assert(curTrace, (status >= 0));
                }
            }
        }
    }

    if (status >= 0) {
        /* Create Counting Semaphore for GateMPApp synchronization*/
        GateMPApp_semSyncHandle = OsalSemaphore_create (OsalSemaphore_Type_Counting);
        if (GateMPApp_semSyncHandle == NULL) {
            Osal_printf ( "Error: Failed OsalSemaphore_create \n");
            status = -1;
            return (status);
        }
    }

    if (status >= 0) {
        /* Register Notify Events */
        for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
            if (SysLinkSamples_toRunProcId(i)) {
                status = Notify_registerEvent (i, NOTIFY_LINEID,
                        SYNC_NOTIFY_EVENT,
                        (Notify_FnNotifyCbck) GateMPApp_sync_callback, NULL);
                if (status < 0) {
                    Osal_printf ("Registering error event failed\n");
                }
            }
        }

        /* Incase of Notify Registration fails. Safely Unregister the registered
         * Notify Events
         */
        if (status < 0) {
            for (i = (i - 1) ; i >= 0; i--) {
                if (SysLinkSamples_toRunProcId (i)) {
                    status = Notify_unregisterEvent( i, NOTIFY_LINEID,
                            SYNC_NOTIFY_EVENT,
                            (Notify_FnNotifyCbck) GateMPApp_sync_callback,
                            NULL);
                    GT_assert (curTrace, (status >= 0));
                }
            }
        }
    }

    Osal_printf("Leaving GateMPApp_startup() (0x%x)\n", status);

    return (status);
}


/*!
 *  @brief  Function to execute the GateMPApp sample application
 *
 *  @sa
 */
Int GateMPApp_execute(Void)
{
    Int32         status = 0;
    UInt32        sharedMemSize = 0;
    UInt32  *     buffer = NULL;
    UInt32        i;
    UInt32        j;
    IHeap_Handle  srHeap = NULL;
    GateMP_Params gateMPParams;
    GateMP_Handle hostGate;
    IArg          key;
    SharedRegion_SRPtr srBuffer;

    Osal_printf("Entered GateMPApp_execute()\n");

    /* Create the Host Gate */
    srHeap = SharedRegion_getHeap (APP_SHAREDREGION_ENTRY_ID);
    if (srHeap == NULL) {
        status = -1;
        Osal_printf ("SharedRegion_getHeap failed!."
                    " srHeap: [0x%x]\n",
                    srHeap);
    }

    if (status >= 0) {
        GateMP_Params_init (&gateMPParams);
        gateMPParams.remoteProtect = GateMP_RemoteProtect_SYSTEM;
        gateMPParams.sharedAddr    = NULL;
        /* Calculate the size for System Gate and shared buffer*/
        sharedMemSize = GateMP_sharedMemReq (&gateMPParams) + BYTES_TO_PROCESS;
        /* Allocate memory for GateMP */
        gateMPParams.sharedAddr = Memory_alloc (srHeap,
                                                sharedMemSize,
                                                0,
                                                NULL);
        if (NULL == gateMPParams.sharedAddr) {
            Memory_free (srHeap,
                         (Ptr) (buffer),
                         BYTES_TO_PROCESS);
            status = -1;
            Osal_printf ("Memory_alloc failed."
                         " when allocating %x bytes!.\n",
                         sharedMemSize);
        }
        else {

            /* Now create the actual gate */
            Osal_printf ("Creating the GateMP\n");
            gateMPParams.name = "HOSTGATE";
            hostGate = GateMP_create (&gateMPParams);
            if (NULL == hostGate) {
                Memory_free (srHeap,
                             (Ptr) (buffer),
                             BYTES_TO_PROCESS);
                status = -1;
                Osal_printf ("GateMP_create failed!\n");
            }
        }
    }

    /* Write configuration information to any running slave processor */
    if (status >= 0) {
        buffer = (UInt32 *) Memory_alloc (srHeap,
                                          BYTES_TO_PROCESS,
                                          0,
                                          NULL);
        if (NULL == buffer) {
            status = -1;
            Osal_printf ("Buffer allocation failed."
                         " when allocating %x bytes!.\n",
                         BYTES_TO_PROCESS);
        }
        else {
            srBuffer = SharedRegion_getSRPtr (buffer, APP_SHAREDREGION_ENTRY_ID);

            /* Initialize the buffer */
            for (j = 0; (j < (BYTES_TO_PROCESS/4)) && (status >= 0); j++) {
                 buffer [j] = STAMP;
            }

            if (SharedRegion_isCacheEnabled(
                      SharedRegion_getId((Ptr)buffer)) ) {
                Cache_wb ( (Ptr)buffer, BYTES_TO_PROCESS,
                           Cache_Type_ALL, TRUE);
            }

            for (i = 0; i < MultiProc_getNumProcessors (); i++) {
                if (    SysLinkSamples_toRunProcId (i)
                    &&  (i != MultiProc_self ())) {
                    status = Ipc_writeConfig (i, TAGID, &srBuffer, 4);
                    Osal_printf("Ipc_writeConfig status %d\n",status);
                    do {
                        status = Notify_sendEvent(i,
                                                  0,
                                                  SYNC_NOTIFY_EVENT,
                                                  0,
                                                  TRUE);
                    }
                    while (status != 0);
                    Osal_printf("Sent sync notification to procId [%d]= %d \n",
                                i,i);
                }
                Osal_printf("GateMPApp_procId [%d]= %d buffer is %p\n",
                            i, i, buffer);
            }

        }
    }

    Osal_printf ("Running the main loop:\n");
    if (status >= 0) {
        for (i = 0; (i < NUM_ITERATIONS) && (status >= 0); i++) {
            key = GateMP_enter (hostGate);

            if (SharedRegion_isCacheEnabled(
                      SharedRegion_getId((Ptr)buffer) ) ) {
                Cache_inv ((Ptr)buffer, BYTES_TO_PROCESS,
                           Cache_Type_ALL, TRUE);
            }

            /* Read bytes to check integrity */
            for (j = 1; (j < (BYTES_TO_PROCESS/4)) && (status >= 0); j++) {
                if (buffer [j - 1] != buffer [j]) {
                    status = -2;
                    Osal_printf ("GateMP integrity failed!\n");
                    break;
                }
            }

            /* Write bytes */
            for (j = 0; (j < (BYTES_TO_PROCESS/4)) && (status >= 0); j++) {
                buffer [j] = STAMP + MultiProc_self();
            }

            if (SharedRegion_isCacheEnabled(
                      SharedRegion_getId((Ptr)buffer)) ) {
                Cache_wb ( (Ptr)buffer, BYTES_TO_PROCESS,
                           Cache_Type_ALL, TRUE);
            }

            GateMP_leave (hostGate, key);

            if ((i != 0) && ((i % 1000) == 0)) {
                Osal_printf ("Completed %d iterations successfully\n", i);
            }
        }
    }

    if (status >= 0) {
        for (i = 0; i < MultiProc_getNumProcessors (); i++) {
            if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
                Osal_printf("Waiting for notification from processor %d\n", i);
                OsalSemaphore_pend (GateMPApp_semSyncHandle,
                                    OSALSEMAPHORE_WAIT_FOREVER);
            }
        }
    }

    if ((status >= 0) || (status == -2)) {
        GateMP_delete (&hostGate);
        Memory_free (srHeap, (Ptr) (buffer),BYTES_TO_PROCESS);
        Memory_free (srHeap, gateMPParams.sharedAddr, sharedMemSize);
    }

    Osal_printf ("Leaving GateMPApp_execute\n");

    return (status);
}


/*!
 *  @brief  Function to execute the shutdown for GateMPApp sample application
 *
 *  @sa
 */
Int GateMPApp_shutdown(String mapFile)
{
    Int32 status = 0;
    UInt32 i;
    Ptr arg;
    
    Osal_printf ("Entered GateMPApp_shutdown()\n");

    /* Unregister the notifications */
    for (i = 0; i < MultiProc_getNumProcessors(); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            status = Notify_unregisterEvent( i, NOTIFY_LINEID,
                    ERROR_NOTIFY_EVENT,
                    (Notify_FnNotifyCbck) GateMPApp_callback, NULL);
            GT_assert(curTrace, (status >= 0));
        }
    }

    for (i = 0; i < MultiProc_getNumProcessors(); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            status = Notify_unregisterEvent(
                                       i,
                                       NOTIFY_LINEID,
                                       SYNC_NOTIFY_EVENT,
                                       (Notify_FnNotifyCbck) GateMPApp_sync_callback,
                                       NULL);
            GT_assert (curTrace, (status >= 0));
        }
    }

    /* Delete Semaphore for sync notify event */
    OsalSemaphore_delete(&GateMPApp_semSyncHandle);

    for (i = 0; i < MultiProc_getNumProcessors(); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (GateMPApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL; 
            }
            else {
                arg = (Ptr)&GateMPApp_resetVector[i];
            }
            /* Call Ipc_control with resetVector for 
             * ProcMgr_BootMode_NoLoad_Pwr,
             * ProcMgr_BootMode_NoLoad_NoPwr
             * and ProcMgr_BootMode_NoBoot modes
             */
            status = Ipc_control(i, 
                Ipc_CONTROLCMD_STOPCALLBACK, arg);
            Osal_printf("After Ipc_stopCallback status: [0x%x]\n", status);
        }
    }

#if !defined(__KERNEL__)
    for (i = 0 ; i < GateMPApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(GateMPApp_procId[i])) {
            if (GateMPApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Shutting down procId [%d]\n",
                        GateMPApp_procId[i]);
                status = SlaveLoader_shutdown(GateMPApp_procId[i],
                        "shutdown", mapFile);
                Osal_printf("SlaveLoader_shutdown status [%d]\n", status);
            }
        }
    }
#endif

    Osal_printf("Leaving GateMPApp_shutdown() (0x%x)\n", status);

    return (status);
}
