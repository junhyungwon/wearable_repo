/*
 *  @file   GateMPApp.c
 *
 *  @brief      MessageQ application.
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
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/Error.h>
/*  ----------------------------------- IPC module Headers           */
#include <ti/ipc/Ipc.h>
#include <ti/sdo/ipc/Ipc.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/GateMP.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/Notify.h>
#include <ti/syslink/utils/OsalPrint.h>
/*  ----------------------------------- BIOS6 module Headers         */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Cache.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
/*  ----------------------------------- To get globals from .cfg Header */
#include <xdc/cfg/global.h>

/* Sync notify event */
#define SYNC_NOTIFY_EVENT    12


Void APPNotify_callbackFxn (UInt16 procId,
                            UInt16 lineNo,
                            UInt32 eventNo,
                            UArg   arg,
                            UInt32 payload)
{
    Semaphore_post((Semaphore_Object*)arg);
}

/*
 *  ======== tsk1_func ========
 *  Send and receive messages
 */
Void tsk1_func(UArg arg0, UArg arg1)
{
    UInt16             hostId = (UInt16) arg0;
    Int                status = 0;
    Bool               isCacheEnabled = FALSE;
    GateMP_Handle      hostGate;
    UInt32 *           buffer;
    UInt32             i;
    UInt32             j;
    IArg               key;
    UInt16             regionId;
    SharedRegion_SRPtr srBuffer;
    Semaphore_Params        semParams;
    Semaphore_Handle   semHandle;
    Error_Block             eb;

    do {
        status = Ipc_attach (hostId);
    } while (status < 0);

    Error_init(&eb);
    Semaphore_Params_init (&semParams);
    semParams.mode = Semaphore_Mode_COUNTING;
    semHandle = Semaphore_create (0, &semParams, &eb);
    if (semHandle == NULL) {
        Error_check (&eb);
        Osal_printf ("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__);
        return;
    }

    /* Register with the remote processor*/
    Notify_registerEvent(hostId,
                         0,
                         SYNC_NOTIFY_EVENT,
                         APPNotify_callbackFxn,
                         (UArg)semHandle);

    Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);

    /* Ipc read config */
    do {
        status = Ipc_readConfig (hostId, TAGID, &srBuffer, 4);
    } while (status < 0);

    buffer = SharedRegion_getPtr (srBuffer);
    regionId = SharedRegion_getId(buffer);
    isCacheEnabled = SharedRegion_isCacheEnabled (regionId);
    /* Open the host gate */
    do {
        status = GateMP_open (HOSTGATE, &hostGate);
        Task_sleep (10);
    } while (status == GateMP_E_NOTFOUND);

    if (status >= 0) {
        for (i = 0; (i < NUM_ITERATIONS) && (status >= 0); i++) {
            key = GateMP_enter (hostGate);
            if (isCacheEnabled == TRUE) {
                Cache_inv((Ptr)buffer,
                          BYTES_TO_PROCESS,
                          Cache_Type_ALL,
                          TRUE);
            }


            /* Read bytes to check integrity */
            for (j = 1; (j < (BYTES_TO_PROCESS/4)) && (status >= 0); j++) {
                if (buffer [j - 1] != buffer [j]) {
                    Notify_sendEvent (hostId,
                                      0,
                                      ERROR_NOTIFY_EVENT,
                                      i,
                                      TRUE);
                    System_abort ("GateMP conformance failed!");
                }
            }

            /* Write bytes */
            for (j = 0; (j < (BYTES_TO_PROCESS/4)) && (status >= 0); j++) {
                buffer [j] = STAMP + MultiProc_self();
            }
            if (isCacheEnabled == TRUE) {
                Cache_wbInv ((Ptr)buffer,
                             BYTES_TO_PROCESS,
                             Cache_Type_ALL,
                             TRUE);
            }

            GateMP_leave (hostGate, key);

            if ((i != 0) && ((i % 1000) == 0)) {
                Osal_printf ("Completed %d iterations successfully\n", i);
            }
        }
        

        /* Send information about number of events received/sent to the host. */
        do{
        status = Notify_sendEvent (hostId,
                                   0,
                                   SYNC_NOTIFY_EVENT,
                                   0,
                                   TRUE);
        if(status < 0) {
                Osal_printf("Error: Notify_sendEvent() failed. \n");
        }
        }while(status != Notify_S_SUCCESS);
    }

    /* Cleanup */
    status = GateMP_close (&hostGate);
    if (status < 0) {
        System_abort("GateMP_close failed\n" );
    }

    /* Spin until Ipc_detach success then stop */
    do {
        status = Ipc_detach(hostId);
    } while (status < 0);

    Ipc_stop();
}

/*
 *  ======== main ========
 */
Int main(Int argc, Char* argv[])
{
    Int32 status = Ipc_S_SUCCESS;
    do {
        /* Call Ipc_start() */
        status = Ipc_start();
    } while (status != Ipc_S_SUCCESS);
    BIOS_start();
    return (0);
}

