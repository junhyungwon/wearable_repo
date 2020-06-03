/*
 *  @file   RingIOApp.c
 *
 *  @brief      Sample application for RingIO. Shows the API calling sequence
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
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Assert.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/gates/GateMutex.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/NameServer.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>
#include <ti/ipc/Ipc.h>

#include <ti/syslink/SysLink.h>
#include <ti/syslink/RingIO.h>

/*  ----------------------------------- To get globals from .cfg Header */
#include <xdc/cfg/global.h>

/* Data stamp for data integrity check.  */
#define XFER_VALUE              128u

#define RIO_NAME                "rio"

Void RingIOApp_taskFxn(UArg arg0, UArg arg1);

Char             RingIOApp_instName [32u];

Semaphore_Handle   RingIOApp_semHandle ;

Int
RingIO_VerifyData (Ptr    buffer,
                   UInt32 size) ;

int main()
{
    Int status = Ipc_S_SUCCESS;
    Task_Handle  tskHandle;
    Task_Params  tskParams;

    Task_Params_init(&tskParams) ;
    System_printf ( "Default stack size %d \n",tskParams.stackSize) ;
    tskParams.stackSize = 0x5000 ;

    /* Create task 1 function */
    tskHandle = Task_create (RingIOApp_taskFxn,
                            &tskParams,
                            NULL) ;
    if (tskHandle == NULL) {
       System_printf ( "ERROR: Failed to crate task tsk1_func \n") ;
    }

    do {
        /* Call Ipc_start() */
    status = Ipc_start();
    } while (status != Ipc_S_SUCCESS);


    System_printf ("Ipc_start status %d\n", status);
    BIOS_start();

    return (0) ;
}

/* Call back function for writer RingIO */
Void APP_readerRingIOCallback(RingIO_Handle handle, Ptr arg, RingIO_NotifyMsg msg)
{
    /* Post the semaphore only when it is meant for termination */
    if (msg == 0x1234) {
        Semaphore_post((Semaphore_Object*)arg);
    }
}


Void RingIOApp_taskFxn(UArg arg0, UArg arg1)
{
    Int32                   status        = 0;
    RingIO_Handle           rioReaderHandle;
    Semaphore_Params        semParams;
    Error_Block             eb;
    RingIO_openParams       rioOpenParams;
    RingIO_BufPtr           bufPtr    = NULL;
    UInt32                  acqSize   = 100;
    UInt16                  remoteProcId = (UInt16)MultiProc_getId("HOST");
    UInt16                  lProcId;

    Error_init (&eb) ;


    SysLink_setup();

    do {
        status = Ipc_attach(remoteProcId);
    } while (status < 0);


    /* Create Binary Semaphore for RingIO notifications*/
    Semaphore_Params_init (&semParams) ;
    semParams.mode = Semaphore_Mode_BINARY;
    RingIOApp_semHandle = Semaphore_create (0, &semParams, &eb) ;
    if (RingIOApp_semHandle == NULL) {
        Error_check (&eb) ;
        System_printf ("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__) ;
        return ;
    }

    lProcId = MultiProc_self();
    memset  (RingIOApp_instName, 0, 32);
    /* RingIO name to be opened will be
     * rioxy where x is the local proc id and y is the remote procid
     */

    System_sprintf (RingIOApp_instName, "%s%d%d", RIO_NAME, lProcId, remoteProcId);

    rioOpenParams.openMode   = RingIO_MODE_READER;
    rioOpenParams.flags = RingIO_DATABUF_MAINTAINCACHE
                       | RingIO_NEED_EXACT_SIZE;


    do {
        Error_init (&eb) ;
        status  = RingIO_open (RingIOApp_instName,
                               &rioOpenParams,
                               NULL,
                               &rioReaderHandle);
        if ((rioReaderHandle == NULL)|| (status < 0)){
            System_printf ("ERROR:Failed to open RingIO in Read mode."
                           "status 0x%x\n",status) ;
        }
    } while (rioReaderHandle == NULL) ;

    if (rioReaderHandle != NULL){
        System_printf ("RingIO Reader client open ... Success\n") ;
    }

    if (status >= 0) {
        /* Register call back function with the RingIO created */
        status = RingIO_registerNotifier (rioReaderHandle,
                                          RingIO_NOTIFICATION_ONCE,
                                          1,
                                          (RingIO_NotifyFxn)APP_readerRingIOCallback,
                                          RingIOApp_semHandle);
        if (status < 0) {
            System_printf ("Reader: Failed to register call back with RingIO status: [0x%x]\n",status);
        }
        else {
            System_printf ("Reader: Passed to register call back with RingIO status: [0x%x]\n",status);
        }
    }


    if (status >= 0) {
        do {
            acqSize = 100;
            status = RingIO_acquire (rioReaderHandle, &bufPtr , &acqSize);
        } while (status < 0);
        if (status >= 0) {
            System_printf  ("RingIO acquire for Reader succeeded:%x\n", status);
            System_printf  ("RingIO acquire for Reader succeeded:%x\n", acqSize );
            status = RingIO_VerifyData (bufPtr, acqSize);
            if (status >= 0) {
                System_printf ( "Data integrity check passed \n");
            }
            else {
                System_printf ( "Data integrity check FAILED:%x\n", status);
            }
        }
        else {
            System_printf  ("RingIO acquire for Reader failed :%x\n", status);
            System_printf  ("RingIO acquire for Reader succeeded:%x\n", acqSize );
        }
    }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (rioReaderHandle, acqSize);
        if (status >= 0) {
            System_printf  ("RingIO release for Reader succeeded:%x\n", status);
            System_printf  ("RingIO release for Reader succeeded:%x\n",acqSize );
        }
        else {
            System_printf  ("RingIO release for Reader failed :%x\n", status);
            System_printf  ("RingIO release for Reader failed:%x\n",acqSize );
        }
    }

//    if (status >= 0) {
//        status = RingIO_getAttribute (rioReaderHandle, &type, &param);
//        if (status >= 0) {
//            System_printf  ("RingIO getAttribute for Reader succeeded:%x\n", status);
//        }
//        else {
//            System_printf  ("RingIO getAttribute for Reader failed :%x\n", status);
//        }
//    }


    if (status >= 0) {
        System_printf  ("Sending notification to writer to cancel acquired data\n");
        do {
            status = RingIO_sendNotify (rioReaderHandle, 0x1234);
        } while (status < 0);
    }

    Semaphore_pend(RingIOApp_semHandle, BIOS_WAIT_FOREVER);

    status = RingIO_unregisterNotifier (rioReaderHandle);

    status = RingIO_close (&rioReaderHandle) ;
    if (status >= 0) {
        System_printf ("RingIO_close successful. status :0x%x\n",status) ;
    }
    else {
        System_printf ("RingIO_close failed.status :0x%x\n",status) ;
    }
    
    Semaphore_delete(&RingIOApp_semHandle);

    /* Spin until Ipc_detach success then stop */
    do {
        status = Ipc_detach(remoteProcId);
    } while (status < 0);

    Ipc_stop();

}


Int
RingIO_VerifyData (Ptr    buffer,
                   UInt32 size)
{
    Int        status = 0;
    UInt8  *   ptr8   = (UInt8  *)  (buffer) ;
    Int16      i ;

    /*
     *  Verify the data
     */
    for (i = 0 ;
         (status >= 0) && (i < size) ;
         i++) {

        if (*ptr8 != (UInt8) (XFER_VALUE)) {
            System_printf ("ERROR! Data integrity check failed\n") ;
            System_printf   ("    Expected [0x%x]\n",
                        (XFER_VALUE)) ;
            System_printf   ("    Received [0x%x]\n", *ptr8) ;
            status = RingIO_E_FAIL;
        }

        ptr8++ ;
    }

    return (status) ;
}


