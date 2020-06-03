/*
 *  @file   HeapMemMPApp.c
 *
 *  @brief      Sample application for HeapMemMP module
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



#if !defined(__KERNEL__)
#include <string.h>
#include <unistd.h>
#endif /* if defined(__KERNEL__) */

/* Standard headers */
#include <ti/syslink/Std.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/syslink/utils/String.h>
#include <ti/syslink/utils/OsalSemaphore.h>

#include <ti/syslink/ProcMgr.h>

/* Module level headers */
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/HeapMemMP.h>
#include <ti/ipc/SharedRegion.h>
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


/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/*!
 *  @brief  Name of the locally created heap.
 */
#define HEAPMEMMPSHMEM_SIZE      0x10000

/*!
 *  @brief  Name of the locally created heap.
 */
#define HEAPMEMMPHOST_PREFIX     "HEAPMEMMPHOST"

/*!
 *  @brief  Name of the remotely created heap.
 */
#define HEAPMEMMPSLAVE_PREFIX    "HEAPMEMMPSLAVE"

/*!
 *  @brief  Length of message Q Names
 */
#define  HEAPMEMMPAPP_NAMELENGTH   80u

/*!
 *  @brief  Buffer size
 */
#define BUFSIZE                  256u

/*!
 *  @brief  Start entry number for shared region for app.
 */
#define APP_SHAREDREGION_ENTRY_ID   0u
#define APPNOTIFY_EVENT_NO          11u

#define SYNC_NOTIFY_EVENT           12
#define NOTIFY_LINEID               0


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
#if !defined(__KERNEL__)
extern String  HeapMemMPApp_filePath [];
extern ProcMgr_BootMode  HeapMemMPApp_bootMode;
extern UInt32            HeapMemMPApp_resetVector[];
#else/* if !defined(__KERNEL__) */
ProcMgr_BootMode         HeapMemMPApp_bootMode = ProcMgr_BootMode_Boot;
UInt32                   HeapMemMPApp_resetVector[SysLinkSamples_MAXPROCESSORS] = {0};
#endif /* if !defined(__KERNEL__) */
extern UInt16  HeapMemMPApp_numProcs;
extern UInt16  HeapMemMPApp_procId   [];
extern Bool    HeapMemMPApp_runAll;

/* Semaphore Handle to sync with slave cores  */
OsalSemaphore_Handle  HeapMemMPApp_semSyncHandle;


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
HeapMemMPApp_sync_callback (UInt16       procId,
                    UInt16       lineId,
                    UInt32       eventNo,
                    Void *       arg,
                    UInt32       payload)
{
    /* Received sync notify */
    OsalSemaphore_post (HeapMemMPApp_semSyncHandle);
}


/*!
 *  @brief  Thread entry point
 *
 *  @sa
 */
void HeapMemMPApp_threadHandler (Ptr arg)
{
    Int32                 status = 0;
    UInt32                cleanupCode = 0;
    HeapMemMP_Handle      HeapMemMPApp_handleLocal;
    HeapMemMP_Handle      HeapMemMPApp_handleRemote;
    HeapMemMP_Params      heapParams;
    UInt32                shAddr;
    Ptr                   heapBlock1;
    Ptr                   heapBlock2;
    Memory_Stats          stats;
    UInt16                procId;
    Char                  tempStr [HEAPMEMMPAPP_NAMELENGTH];
    UInt16                localProcId;
    IHeap_Handle          heapMemMPAppHandle = NULL ;

    Osal_printf ("Entered HeapMemMPApp_threadHandler\n");

    localProcId = MultiProc_self ();
    procId = (UInt16) ((UInt32) arg);

    heapMemMPAppHandle = SharedRegion_getHeap (APP_SHAREDREGION_ENTRY_ID);
    if (heapMemMPAppHandle == NULL) {
        status = HeapMemMP_E_FAIL;
        Osal_printf ("Error in SharedRegion_getHeap\n");
    }
    else {
        Osal_printf ("Heap in SharedRegion_getHeap : %x\n", heapMemMPAppHandle);

        shAddr = (UInt32) Memory_alloc (heapMemMPAppHandle,
                                        HEAPMEMMPSHMEM_SIZE,
                                        0,
                                        NULL);
        if (shAddr == (UInt32) NULL) {
            status = HeapMemMP_E_FAIL;
            Osal_printf("Error in Memory_alloc\n");
        }
        else {
            Osal_printf ("Memory_alloc buffer [0x%x]\n", shAddr);
            cleanupCode = 1;
        }
    }

    if (status >= 0) {
        HeapMemMP_Params_init(&heapParams);
        Memory_set  (tempStr, 0, HEAPMEMMPAPP_NAMELENGTH);
        sprintf (tempStr, "%s_%d%d", HEAPMEMMPHOST_PREFIX, localProcId, procId);
        heapParams.name           = tempStr;
        heapParams.sharedAddr     = (Ptr)(shAddr);
        heapParams.sharedBufSize  = 0x4000;
        heapParams.regionId       = APP_SHAREDREGION_ENTRY_ID  ;
        heapParams.gate           = NULL ;
        HeapMemMPApp_handleLocal = HeapMemMP_create (&heapParams);
        if (HeapMemMPApp_handleLocal == NULL) {
            status = HeapMemMP_E_FAIL;
            Osal_printf ("Error in HeapMemMP_create\n");
        }
        else {
            cleanupCode = 2;
            Osal_printf ("HeapMemMP_create handle [0x%x]\n",
                         HeapMemMPApp_handleLocal);
        }
    }

    /* Open the remote HeapMemMP. */
    if (status >= 0) {
        Memory_set  (tempStr, 0, HEAPMEMMPAPP_NAMELENGTH);
        sprintf (tempStr, "%s_%d%d", HEAPMEMMPSLAVE_PREFIX, localProcId, procId);

        Osal_printf ("Opening the HeapMemMP created by remote processor\n");
        do {
            status = HeapMemMP_open (tempStr, &HeapMemMPApp_handleRemote);
            /* Sleep for a while before trying again. */
            if (status == HeapMemMP_E_NOTFOUND) {
#if defined(__KERNEL__)
                OsalThread_sleep (10);
#else /* if defined(__KERNEL__) */
                usleep (10);
#endif /* if defined(__KERNEL__) */
            }
        }
        while (status == HeapMemMP_E_NOTFOUND);

        if (status < 0){
            Osal_printf ("Error in HeapMemMP_open. Status [0x%x]\n", status);
        }
        else {
            cleanupCode = 3;
            Osal_printf ("HeapMemMP_open Status [0x%x]\n", status);
        }

        Osal_printf ("Sending sync notification to procId: %d\n", procId);
        Notify_sendEvent(procId,
                         0,
                         APPNOTIFY_EVENT_NO,
                         0,
                         TRUE);
        Osal_printf ("Sent synchronizaion notification to procId: %d \n",procId);
    }

    if (status >= 0) {
        Osal_printf ("-----------------------------------------------\n");
        Osal_printf ("Allocating from created Heap\n");

        /* Get stats */
        HeapMemMP_getStats (HeapMemMPApp_handleLocal, &stats);

        Osal_printf ("Before allocating buffers totalSize of HeapMemMP = 0x%x\n",
                     stats.totalSize);
        Osal_printf ("Before allocating buffers stats.totalFreeSize  of HeapMemMP = 0x%x\n",
                     stats.totalFreeSize);
        Osal_printf ("Before allocating buffers largestFreeSize  of HeapMemMP = 0x%x\n",
                     stats.largestFreeSize);

        /* Allocate buffers from locally created heap. */
        heapBlock1 = HeapMemMP_alloc (HeapMemMPApp_handleLocal,
                                      BUFSIZE,
                                      0u);

        if (heapBlock1 == NULL) {
            status = HeapMemMP_E_FAIL;
            Osal_printf ("HeapMemMP_alloc failed [0x%x]\n", heapBlock1);
        }
        else {
            Osal_printf ("HeapMemMP_alloc. Buffer [0x%x]\n", heapBlock1);

            heapBlock2 = HeapMemMP_alloc(HeapMemMPApp_handleLocal, BUFSIZE,
                    0u);

            if (heapBlock2 == NULL) {
                status = HeapMemMP_E_FAIL;
                Osal_printf ("HeapMemMP_alloc failed [0x%x]\n", heapBlock2);
            }
            else {
                Osal_printf ("HeapMemMP_alloc. Buffer [0x%x]\n", heapBlock2);
                /* Get stats */
                HeapMemMP_getStats (HeapMemMPApp_handleLocal, &stats);

                Osal_printf ("Before freeing buffers totalSize of HeapMemMP = 0x%x\n",
                             stats.totalSize);
                Osal_printf ("Before freeing buffers stats.totalFreeSize  of HeapMemMP = 0x%x\n",
                             stats.totalFreeSize);
                Osal_printf ("Before  freeing buffers largestFreeSize  of HeapMemMP = 0x%x\n",
                             stats.largestFreeSize);

                HeapMemMP_free(HeapMemMPApp_handleLocal, heapBlock1, BUFSIZE);
                Osal_printf("HeapMemMP_free. heapBlock1 [0x%x]\n", heapBlock1);

                HeapMemMP_free(HeapMemMPApp_handleLocal, heapBlock2, BUFSIZE);
                Osal_printf("HeapMemMP_free. heapBlock2 [0x%x]\n", heapBlock2);

                /* Get stats */
                HeapMemMP_getStats (HeapMemMPApp_handleLocal, &stats);

                Osal_printf ("After freeing buffers totalSize of HeapMemMP = 0x%x\n",
                             stats.totalSize);
                Osal_printf ("After freeing buffers stats.totalFreeSize  of HeapMemMP = 0x%x\n",
                             stats.totalFreeSize);
                Osal_printf ("After freeing buffers largestFreeSize  of HeapMemMP = 0x%x\n",
                             stats.largestFreeSize);
            }
        }
    }

    if (status >= 0) {
        Osal_printf ("-----------------------------------------------\n");
        Osal_printf ("Allocating from opened Heap\n");

        /* Allocate buffers from remotely created heap. */
        heapBlock1 = HeapMemMP_alloc(HeapMemMPApp_handleRemote, BUFSIZE, 0u);

        if (heapBlock1 == NULL) {
            status = HeapMemMP_E_FAIL;
            Osal_printf("HeapMemMP_alloc failed [0x%x]\n", heapBlock1);
        }
        else {
            Osal_printf("HeapMemMP_alloc. Buffer [0x%x]\n", heapBlock1);

            heapBlock2 = HeapMemMP_alloc(HeapMemMPApp_handleRemote, BUFSIZE,
                    0u);

            if (heapBlock2 == NULL) {
                status = HeapMemMP_E_FAIL;
                Osal_printf("HeapMemMP_alloc failed [0x%x]\n", heapBlock2);
            }
            else {
                Osal_printf("HeapMemMP_alloc. Buffer [0x%x]\n", heapBlock2);

                HeapMemMP_free(HeapMemMPApp_handleRemote, heapBlock1, BUFSIZE);
                Osal_printf("HeapMemMP_free. heapBlock1 [0x%x]\n", heapBlock1);

                HeapMemMP_free(HeapMemMPApp_handleRemote, heapBlock2, BUFSIZE);
                Osal_printf ("HeapMemMP_free. heapBlock2 [0x%x]\n", heapBlock2);
                /* Get stats */
                HeapMemMP_getStats(HeapMemMPApp_handleRemote, &stats);

                Osal_printf ("After allocating  and freeing buffers totalSize of HeapMemMP = 0x%x\n",
                             stats.totalSize);
                Osal_printf ("Before allocating  and freeing stats.totalFreeSize  of HeapMemMP = 0x%x\n",
                             stats.totalFreeSize);
                Osal_printf ("Before allocating  and freeing largestFreeSize  of HeapMemMP = 0x%x\n",
                             stats.largestFreeSize);
            }
        }
    }

    if (status >= 0) {
        Osal_printf("Waiting for notification from processor %d\n", procId);
        OsalSemaphore_pend(HeapMemMPApp_semSyncHandle,
                            OSALSEMAPHORE_WAIT_FOREVER);
    }

    switch (cleanupCode) {
        case 3:
        {
            status = HeapMemMP_close(&HeapMemMPApp_handleRemote);
            if (status < 0) {
                Osal_printf ("HeapMemMP_close failed. Status [0x%x]\n",
                        status);
            }
        }

        case 2:
        {
            status = HeapMemMP_delete (&HeapMemMPApp_handleLocal);
            if (status < 0) {
                Osal_printf("HeapMemMP_delete failed. Status [0x%x]\n",
                        status);
            }
        }

        case 1:
        {
            Memory_free(heapMemMPAppHandle, (Ptr)shAddr, HEAPMEMMPSHMEM_SIZE);
        }
        break;
    }

    Osal_printf("Leaving HeapMemMPApp_threadHandler\n");
}


/*!
 *  @brief  Function to execute the startup for HeapMemMPApp sample application
 *
 *  @sa
 */
Int HeapMemMPApp_startup(String mapFile)
{
    Int status = 0;
    UInt32 i = 0;
    Ptr arg;
    
    Osal_printf("Entered HeapMemMPApp_startup\n");

    /* Set the slaves to run the application with. */
    status = SysLinkSamples_setToRunProcIds (HeapMemMPApp_numProcs,
            HeapMemMPApp_procId, HeapMemMPApp_runAll);

#if !defined(__KERNEL__)
    for (i = 0; (i < HeapMemMPApp_numProcs) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(HeapMemMPApp_procId[i])) {
            if (HeapMemMPApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Loading and starting procId [%d] with [%s]\n",
                        HeapMemMPApp_procId[i], HeapMemMPApp_filePath[i]);
                status = SlaveLoader_startup(HeapMemMPApp_procId[i],
                        "startup", HeapMemMPApp_filePath[i], mapFile);
                Osal_printf("SlaveLoader_startup status [%d]\n", status);
            }
        }
    }
#endif

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (HeapMemMPApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL;
            }
            else {
                arg = (Ptr)&HeapMemMPApp_resetVector[i];
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
        /* Create Binary Semaphore for application synchronization*/
        HeapMemMPApp_semSyncHandle = OsalSemaphore_create (OsalSemaphore_Type_Counting);
        if (HeapMemMPApp_semSyncHandle == NULL) {
            Osal_printf( "Error: Failed OsalSemaphore_create\n");
            status = -1;
            return (status);
        }
    }

    if (status >= 0) {
        /* Register Notify Events */
        for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
            if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
                status = Notify_registerEvent (i, NOTIFY_LINEID,
                        SYNC_NOTIFY_EVENT,
                        (Notify_FnNotifyCbck)HeapMemMPApp_sync_callback, NULL);
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
                            SYNC_NOTIFY_EVENT,
                            (Notify_FnNotifyCbck)HeapMemMPApp_sync_callback,
                            NULL);
                    GT_assert(curTrace, (status >= 0));
                }
            }
        }
    }

    Osal_printf("Leaving HeapMemMPApp_startup() (0x%x)\n", status);

    return (status);
}


/*!
 *  @brief  Function to execute the HeapMemMPApp sample application
 *
 *  @sa
 */
Int HeapMemMPApp_execute(Void)
{
    Int32  status = 0;
    UInt32 i;

    Osal_printf ("Entered HeapMemMPApp_execute\n");

    /* Create threads */
    for (i = 0; i < MultiProc_getNumProcessors (); i++) {
        if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
            /* Execute the logic one by one for each processor */
            Osal_printf ("Calling HeapMemMPApp_threadHandler for procId %d\n",
                         i);
            HeapMemMPApp_threadHandler ((Ptr) i);
        }
    }

    Osal_printf ("Leaving HeapMemMPApp_execute\n");

    return (status);
}


/*!
 *  @brief  Function to execute the shutdown for HeapMemMPApp sample application
 *
 *  @sa     HeapMemMPApp_callback
 */
Int HeapMemMPApp_shutdown(String mapFile)
{
    Int32 status = 0;
    UInt32 i = 0;
    Ptr arg;
    
    Osal_printf("Entered HeapMemMPApp_shutdown\n");

    /* Unregister the notifications */
    for (i = 0; i < MultiProc_getNumProcessors (); i++) {
        if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
            status = Notify_unregisterEvent (
                               i,
                               NOTIFY_LINEID,
                               SYNC_NOTIFY_EVENT,
                               (Notify_FnNotifyCbck) HeapMemMPApp_sync_callback,
                               NULL);
            GT_assert (curTrace, (status >= 0));
        }
    }

    /* Delete Binary Semaphore for sync notify event */
    OsalSemaphore_delete(&HeapMemMPApp_semSyncHandle);
    for (i = 0; i < MultiProc_getNumProcessors(); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (HeapMemMPApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL; 
            }
            else {
                arg = (Ptr)&HeapMemMPApp_resetVector[i];
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
    for (i = 0; i < HeapMemMPApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(HeapMemMPApp_procId[i])) {
            if (HeapMemMPApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Shutting down procId [%d]\n",
                        HeapMemMPApp_procId[i]);
                status = SlaveLoader_shutdown(HeapMemMPApp_procId[i],
                        "shutdown", mapFile);
                Osal_printf("SlaveLoader_shutdown status [%d]\n", status);
            }
        }
    }
#endif /* if !defined(__KERNEL__) */

    Osal_printf("Leaving HeapMemMPApp_shutdown() (0x%x)\n", status);

    return (status);
}
