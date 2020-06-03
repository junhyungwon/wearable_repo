/*
 *  @file   RingIO_ti81xx_dsp.c
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

/* Shim layer includes */
#include <ti/syslink/RingIO.h>
#include <ti/syslink/RingIOShm.h>
#include <ti/syslink/inc/ClientNotifyMgr.h>


#include "RingIO_sample.h"

/*  ------------------------To get globals from .cfg Header ******************/
#include <xdc/cfg/global.h>

#define IPC_BUFFER_ALIGN(x, y) (UInt32)((UInt32)((x + y - 1) / y) * y)

Void tsk_func(UArg arg0, UArg arg1);
Void tsk_WriterFunc(UArg arg0, UArg arg1);
Void tsk_ReaderFunc(UArg arg0, UArg arg1);

static WriterTaskInfo wrTaskInfo;
static ReaderTaskInfo rdTaskInfo;
UInt16 remoteProcId;


/* Call back function for writer RingIO */
Void APP_writerRingIOCallback (RingIO_Handle handle, Ptr arg, RingIO_NotifyMsg msg)
{
    Semaphore_post((Semaphore_Object*)arg);
}

/* Call back function for reader RingIO */
Void APP_readerRingIOCallback (RingIO_Handle handle, Ptr arg, RingIO_NotifyMsg msg)
{
    Semaphore_post((Semaphore_Object*)arg);
}

Char   tempStr[80];

Int32 APP_ipcInit()
{
    Int32                       status = 0;

    /*
     *  Need to define the shared region. The IPC modules use this
     *  to make portable pointers. All processors need to add this
     *  same call with their base address of the shared memory region.
     *  If the processor cannot access the memory, do not add it.
     */

    if  (ipcWithVIDEOM3 == 1) {
        remoteProcId = MultiProc_getId("VIDEO-M3");
        do {
            status = Ipc_attach(remoteProcId);
        } while (status < 0);
    }

    if  (ipcWithVPSSM3 == 1) {
        remoteProcId = MultiProc_getId("VPSS-M3");
        do {
            status = Ipc_attach(remoteProcId);
        } while (status < 0);
    }

    return (status);

}


int main()
{
    Task_Handle             tskHandle;
    Task_Params             tskParams;
    Int32                   status;

    Task_Params_init(&tskParams);
    tskParams.stackSize = 0x4000;
    tskHandle = Task_create (tsk_func,
                             &tskParams,
                             NULL);
    if (tskHandle == NULL) {
        System_printf ( "ERROR: Failed to crate task tsk0_func \n");
    }


    status = Ipc_start();
    if (status < 0) {
        System_printf ( "ERROR: Ipc_start Failed status 0x%x \n",status);
    }
    BIOS_start();
    return (0);
}


/*
 *  ======== tsk_func ========
 *  Calls the  RingIO  setup API.and Spwans tasks to talk to M3Vido and VPSSM3
 */
Void tsk_func(UArg arg0, UArg arg1)
{
    Task_Handle             tskWriterHandle;
    Task_Handle             tskReaderHandle;
    Task_Params             tskParams;
    Int32                   status;


    System_printf ("Entered tsk_func()\n");

    status = APP_ipcInit();

    if (status < 0) {
       System_printf ( "ERROR: APP_ipcInit Failed \n");
       return;
    }

   /* Spawn  tasks */
   if(ipcWithVIDEOM3 == 1) {
      /* Create the task  to produce frames to Video */
        Task_Params_init(&tskParams);
        tskParams.stackSize = 0x4000;
        /* let tasks create it internally */
        wrTaskInfo.semHandle = NULL;
        tskParams.arg1 = (UArg)&wrTaskInfo;
        tskWriterHandle = Task_create (tsk_WriterFunc,
                                       &tskParams,
                                       NULL);
        if (tskWriterHandle == NULL) {
            System_printf ( "ERROR: Failed to crate task tsk_WriterFunc \n");
        }
    }

   if(ipcWithVPSSM3 == 1) {
      /* Creat a task to retrieve frames from VPSSM3 */
        Task_Params_init(&tskParams);
        tskParams.stackSize = 0x2000;
        /* let tasks create it internally */
        rdTaskInfo.semHandle = NULL;

        tskParams.arg0 = (UArg)&rdTaskInfo;
        tskReaderHandle = Task_create (tsk_ReaderFunc,
                                       &tskParams,
                                       NULL);
        if (tskReaderHandle == NULL) {
            System_printf ( "ERROR: Failed to create task tsk_WriterFunc \n");
        }
    }

    System_printf ("Leave tsk_func()\n");
}


/*
 *  ======== tsk_WriterFunc ========
 */
Void tsk_WriterFunc(UArg arg0, UArg arg1)
{
    Int32                   status          = 0;
    WriterTaskInfo          *taskInfo = &wrTaskInfo;
    RingIO_BufPtr           bufPtr    = NULL;
    UInt32                  acqSize   = 100;
    Semaphore_Params        semParams;
    Error_Block             eb;
    RingIO_Handle           rioHandle;
    RingIO_Handle           wrioHandle;
    RingIOShm_Params        rioShmParams;
    RingIO_openParams       openRioParams;
    UInt16                  type;


    System_printf ("Entered tsk_WriterFunc()\n");
    Error_init (&eb);
    /* Create Binary Semaphore for FramQ notifications*/
    Semaphore_Params_init (&semParams);
    semParams.mode = Semaphore_Mode_COUNTING;
    taskInfo->semHandle = Semaphore_create (0, &semParams, &eb);
    if (taskInfo->semHandle == NULL) {
        Error_check (&eb);
        System_printf ("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__);
        return;
    }


    RingIOShm_Params_init (&rioShmParams);
    rioShmParams.commonParams.name  = RINGIO_INST1_NAME;
    rioShmParams.ctrlRegionId       = 0x0;
    rioShmParams.dataRegionId       = 0x0;
    rioShmParams.attrRegionId       = 0x0;
    rioShmParams.attrSharedAddrSize = 0x100;
    rioShmParams.dataSharedAddrSize = 0x1000;
    rioShmParams.attrSharedAddrSize = 0x100;
    rioShmParams.remoteProcId       = remoteProcId;
    rioHandle = RingIO_create (&rioShmParams);

    if (rioHandle != NULL) {
    openRioParams.flags = RingIO_DATABUF_MAINTAINCACHE
                       | RingIO_NEED_EXACT_SIZE;

    openRioParams.openMode = RingIO_MODE_WRITER;

    status = RingIO_open (
                          RINGIO_INST2_NAME,
                          &openRioParams,
                          NULL,
                          &wrioHandle);
    }


    if (status >= 0) {
        /* Register call back fucntion   with the RingIObufMgr created */
        status = RingIO_registerNotifier (wrioHandle,
                                          RingIO_NOTIFICATION_ONCE,
                                          1,
                                          (RingIO_NotifyFxn )APP_writerRingIOCallback,
                                          taskInfo->semHandle);
        if (status >= 0) {
            System_printf ("Writer: Passed to register call back with RingIO status: [0x%x]\n",status);
        }
        else {
            System_printf ("Writer: Failed to register call back with RingIO status: [0x%x]\n",status);
        }
    }

    if (status >= 0) {
        status = RingIO_acquire (wrioHandle, &bufPtr , &acqSize) ;
        if (status >= 0) {
            System_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            System_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
        }
        else {
            System_printf  ("RingIO acquire for Writer failed :%x\n", status);
            System_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
        }
    }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (wrioHandle, acqSize) ;
        if (status >= 0) {
            System_printf  ("RingIO release for Writer succeeded:%x\n", status);
            System_printf  ("RingIO release for Writer succeeded:%x\n",acqSize );
        }
        else {
            System_printf  ("RingIO release for Writer failed :%x\n", status);
            System_printf  ("RingIO release for Writer failed:%x\n",acqSize );
        }
        }


    if (RingIO_getValidSize (wrioHandle) == 100) {
        System_printf ( "Writer has produced valid data\n");
    }

    if  (status >= 0) {
        /* Send data transfer attribute (Fixed attribute) */
        type = (UInt16) 1;
        status = RingIO_setAttribute (wrioHandle, type, 0, FALSE);
    }

    if (status >= 0) {
        System_printf  ("RingIO setAttribute for Writer succeeded:%x\n", status);
    }
    else {
        System_printf  ("RingIO setAttribute for Writer failed :%x\n", status);
    }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (wrioHandle, &bufPtr , &acqSize) ;
        if (status >= 0) {
            System_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            System_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
        }
        else {
            System_printf  ("RingIO acquire for Writer failed :%x\n", status);
            System_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
        }
        }

    Semaphore_pend (taskInfo->semHandle, BIOS_WAIT_FOREVER) ;
    System_printf ( " *****APP_writerRingIOCallback\n") ;

    if (status >= 0) {
        status = RingIO_cancel (wrioHandle) ;
    }

    if (status >= 0) {
        if (RingIO_getAcquiredSize (wrioHandle) == 0) {
            System_printf ( "Writer has cancelled acquired data\n");
        }
    }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (wrioHandle, &bufPtr , &acqSize) ;
        if (status >= 0) {
            System_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            System_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
            }
        else {
            System_printf  ("RingIO acquire for Writer failed :%x\n", status);
            System_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
           }
        }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (wrioHandle, acqSize) ;
        if (status >= 0) {
            System_printf  ("RingIO release for Writer succeeded:%x\n", status);
            System_printf  ("RingIO release for Writer succeeded:%x\n",acqSize );
        }
        else {
            System_printf  ("RingIO release for Writer failed :%x\n", status);
            System_printf  ("RingIO release for Writer failed:%x\n",acqSize );
    }
    }

    do {
        status = RingIO_sendNotify (wrioHandle, 0x1234);
    } while (status < 0);
    System_printf  ("Sending notification to reader to flush data\n");


    //status = RingIO_close (&wrioHandle);
    //status = RingIO_delete (&rioHandle);

    System_printf ("Leave tsk_WriterFunc()\n");
}

/* Reader task  retrieves frames from DSP */
Void tsk_ReaderFunc(UArg arg0, UArg arg1)
{
    Int32                   status        = 0;
    RingIO_BufPtr           bufPtr    = NULL;
    UInt32                  acqSize   = 100;
    UInt32                  bytesFlushed = 0;
    Error_Block             eb;
    Semaphore_Params        semParams;
    RingIO_Handle           rioHandle;
    RingIO_openParams       rioParams;
    UInt16                  type;
    UInt32                  param;

    ReaderTaskInfo          *taskInfo = &rdTaskInfo;

    Error_init (&eb);
    System_printf ( " Entered tsk_ReaderFunc\n");

    /* Create Binary Semaphore for FramQ notifications*/
    Semaphore_Params_init (&semParams);
    semParams.mode = Semaphore_Mode_COUNTING;
    taskInfo->semHandle = Semaphore_create (0, &semParams, &eb);
    if (taskInfo->semHandle == NULL) {
        Error_check (&eb);
        System_printf ("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__);
        return;
    }

    rioParams.flags = RingIO_DATABUF_MAINTAINCACHE
                       | RingIO_NEED_EXACT_SIZE;

    rioParams.openMode = RingIO_MODE_READER;


    do {
       status = RingIO_open (
                             RINGIO_INST2_NAME,
                             &rioParams,
                             NULL,
                             &rioHandle);
       //Task_sleep (10);
    } while (status < 0);

    if (rioHandle != NULL) {
        /* Register call back fucntion   with the RingIObufMgr created */
        status = RingIO_registerNotifier (rioHandle,
                                          RingIO_NOTIFICATION_ONCE,
                                          1,
                                          (RingIO_NotifyFxn )APP_readerRingIOCallback,
                                          taskInfo->semHandle);
        if (status >= 0) {
            System_printf ("Reader: Passed to register call back with RingIO status: [0x%x]\n",status);
            }
        else {
            System_printf ("Reader: Failed to register call back with RingIO status: [0x%x]\n",status);
        }
        }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (rioHandle, &bufPtr , &acqSize) ;
        if (status >= 0) {
            System_printf  ("RingIO acquire for Reader succeeded:%x\n", status);
            System_printf  ("RingIO acquire for Reader succeeded:%x\n",acqSize );
        }
        else {
            System_printf  ("RingIO acquire for Reader failed :%x\n", status);
            System_printf  ("RingIO acquire for Reader succeeded:%x\n",acqSize );
    }
    }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (rioHandle, acqSize) ;
    if (status >= 0) {
            System_printf  ("RingIO release for Reader succeeded:%x\n", status);
            System_printf  ("RingIO release for Reader succeeded:%x\n",acqSize );
    }
    else {
            System_printf  ("RingIO release for Reader failed :%x\n", status);
            System_printf  ("RingIO release for Reader failed:%x\n",acqSize );
    }
    }


    if (RingIO_getValidSize (rioHandle) == 0) {
        System_printf ( "Reader has consumed all valid data\n");
    }

    status = RingIO_getAttribute (rioHandle, &type, &param);
    if (status >= 0) {
        System_printf  ("RingIO getAttribute for Reader succeeded:%x\n", status);
    }
    else {
        System_printf  ("RingIO getAttribute for Reader failed :%x\n", status);
    }

    status = RingIO_sendNotify (rioHandle, 0x1234);
    System_printf  ("Sending notification to writer to cancel acquired data\n");

    Semaphore_pend (taskInfo->semHandle, BIOS_WAIT_FOREVER) ;
    System_printf ( " *****APP_readerRingIOCallback\n") ;

    if (status >= 0) {
        status = RingIO_flush
                       (rioHandle,
                        TRUE,
                        &type,
                        &param,
                        &bytesFlushed);
    }

    if (bytesFlushed == 100) {
        System_printf  ("Reader successfully flushed valid data\n");
    }

    status = RingIO_close (&rioHandle);
}


