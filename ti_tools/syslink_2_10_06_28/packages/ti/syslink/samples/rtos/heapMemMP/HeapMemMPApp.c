/*
 *  @file   HeapMemMPApp.c
 *
 *  @brief      HeapMemMP application on RTOS side
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



#include <xdc/std.h>
#include <string.h>

/*  -----------------------------------XDC.RUNTIME module Headers    */
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

/*  ----------------------------------- BIOS6 module Headers         */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/*  ----------------------------------- Utils Headers                */
#include <ti/ipc/Ipc.h>
#include <xdc/runtime/IHeap.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/GateMP.h>
#include <ti/ipc/HeapMemMP.h>

#include <ti/sysbios/knl/Semaphore.h>
/*  ----------------------------------- To get globals from .cfg Header */
#include <xdc/cfg/global.h>


/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/* Memory for remotely created heap */
#define HEAPMEMMPSHMEM_SIZE           0x10000

/*!
 *  @brief  Name of the heap created by hostapp.
 */
#define HEAPMEMMPHOST_PREFIX     "HEAPMEMMPHOST"

/*!
 *  @brief  Name of the locally created heap.
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

/* Sync notify event */
#define SYNC_NOTIFY_EVENT           12u

/** ============================================================================
 *  Globals
 *  ============================================================================
 */
HeapMemMP_Handle     HeapMemMPApp_handleLocal;
HeapMemMP_Handle     HeapMemMPApp_handleRemote;

Semaphore_Handle   semHandle1 ;

Void APPNotify_callbackFxn (UInt16 procId,UInt16 lineId, UInt32 eventNo,
           UArg arg, UInt32 payload)
{
    Semaphore_post((Semaphore_Object*)arg);
}


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
Int main()
{
    Int32 status = Ipc_S_SUCCESS;
    do {
        /* Call Ipc_start() */
        status = Ipc_start();
    } while (status != Ipc_S_SUCCESS);
    BIOS_start();
    return (0);
}

/*
 *  ======== HeapMemMPApp_taskFxn ========
 *  Task function for HeapMemMP application
 */
Void HeapMemMPApp_taskFxn(UArg arg0, UArg arg1)
{
    Int32               status = 0;
    UInt16              procId = (UInt16) arg0;
    UInt32              shAddr = 0;
    HeapMemMP_Params    heapMemMPParams;
    Ptr                 heapBlock;
    UInt16              localProcId;
    Error_Block         eb;
    Char                tempStr [HEAPMEMMPAPP_NAMELENGTH];
    UInt32              appAddr;
    IHeap_Handle        heapMemMPAppHandle = NULL;
    UInt16              remoteProcId = (UInt16) arg0;
    Semaphore_Params    semParams;

    System_printf("\nHeapMemMP Application starts\n");

    Error_init(&eb);

    do {
        status = Ipc_attach(remoteProcId);
    } while (status < 0);

    localProcId = MultiProc_self ( );

    Semaphore_Params_init (&semParams) ;
    semParams.mode = Semaphore_Mode_BINARY;
    semHandle1 = Semaphore_create (0, &semParams, &eb) ;
    if (semHandle1 == NULL) {
        Error_check (&eb) ;
        System_printf ("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__) ;
        return ;
    }

    heapMemMPAppHandle = SharedRegion_getHeap (APP_SHAREDREGION_ENTRY_ID);
    if (heapMemMPAppHandle == NULL)
    {
        System_printf ("Error in SharedRegion_getHeap\n");
    }
    else {
        System_printf ("Heap in SharedRegion_getHeap : %x\n", heapMemMPAppHandle);
    }

    Error_init(&eb);
    shAddr = (UInt32) Memory_alloc ((IHeap_Handle)heapMemMPAppHandle,
                                 HEAPMEMMPSHMEM_SIZE,
                                 0,
                                 &eb);
    System_printf ("shAddr: %x\n", shAddr);

    Notify_registerEvent(
                     procId,
                     0,
                     APPNOTIFY_EVENT_NO,
                     (Notify_FnNotifyCbck) APPNotify_callbackFxn,
                     (UArg) semHandle1);

    /*
     *  Create the heap.
     */
    appAddr = shAddr;
    HeapMemMP_Params_init(&heapMemMPParams);
    memset (tempStr, 0, HEAPMEMMPAPP_NAMELENGTH);
    System_sprintf (tempStr,
                    "%s_%d%d",
                    HEAPMEMMPSLAVE_PREFIX,
                    procId,
                    localProcId);
    heapMemMPParams.name           = tempStr;
    heapMemMPParams.regionId       = APP_SHAREDREGION_ENTRY_ID;
    heapMemMPParams.sharedAddr     = (Ptr) appAddr;
    heapMemMPParams.sharedBufSize  = 0x5000;
    heapMemMPParams.gate           = NULL ;
    HeapMemMPApp_handleLocal = HeapMemMP_create (&heapMemMPParams);

    Semaphore_pend(semHandle1, BIOS_WAIT_FOREVER) ;

    System_printf ("Got App notification ....:\n") ;

    /*
     *  Open the heap that is created on the Arm. Loop until opened.
     */
    memset (tempStr, 0, HEAPMEMMPAPP_NAMELENGTH);
    System_sprintf (tempStr,
                    "%s_%d%d",
                    HEAPMEMMPHOST_PREFIX,
                    procId,
                    localProcId);
    do {
        status = HeapMemMP_open(tempStr, &HeapMemMPApp_handleRemote);
        if (status == HeapMemMP_E_NOTFOUND) {
            Task_sleep (10);
        }
    }
    while (status == HeapMemMP_E_NOTFOUND);

    if (HeapMemMPApp_handleLocal == NULL){
        System_printf("handle null");
    }
    else {
        heapBlock = HeapMemMP_alloc (HeapMemMPApp_handleLocal,
                                     BUFSIZE,
                                     /*0u*/Memory_getMaxDefaultTypeAlign ());

        if (heapBlock != NULL){
            System_printf("\nBlock allocated successfully\n");

            HeapMemMP_free (HeapMemMPApp_handleLocal,
                            heapBlock,
                            BUFSIZE);
        }

        /* Send information about number of events received/sent to the host. */
        status = Notify_sendEvent (procId,
                                   0,
                                   SYNC_NOTIFY_EVENT,
                                   0,
                                   TRUE);
        if(status < 0) {
            System_printf("Error: Notify_sendEvent() failed. \n");
        }

    }

    HeapMemMP_close(&HeapMemMPApp_handleRemote);
    /* Cannot delete the HeapMemMP because the remote processor may still be
     * accessing it.
     */

    /* Spin until Ipc_detach success then stop */
    do {
        status = Ipc_detach(remoteProcId);
    } while (status < 0);

    Ipc_stop();

    System_printf("\nHeapMemMP Application ends\n");
}

