/*
 *  @file   RingIOAppArm.c
 *
 *  @brief      Sample application for RingIO that runs on bios
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


/*-------------------------   XDC headers   -------------------------------  */
#include <xdc/std.h>
#include <string.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Assert.h>
/*-------------------------   BIOS headers   -------------------------------  */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/gates/GateMutex.h>
/*-------------------------   Module headers   -----------------------------  */
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/NameServer.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>
#include <ti/ipc/Ipc.h>

#include <ti/syslink/SysLink.h>
#include <ti/syslink/RingIO.h>
#include <ti/syslink/RingIOShm.h>
#include <ti/syslink/samples/rtos/procMgr/ProcMgrApp.h>
#include <ti/syslink/inc/knl/Platform.h>
/*-------------------------   OSAL and utils   -----------------------------  */
#include <ti/syslink/utils/OsalPrint.h>
/*-------------------------   Application headers   ------------------------  */
#include "RingIO_sample.h"
#include "RingIOAppArm.h"
/*  ------------------------To get globals from .cfg Header ******************/
#include <xdc/cfg/global.h>

/** ============================================================================
 *  Macros
 *  ============================================================================
 */
#define IPC_BUFFER_ALIGN(x, y) (UInt32)((UInt32)((x + y - 1) / y) * y)
#define XFER_VALUE              128u
#define RINGIOAPPARM_MAXPROCESSORS 10u

/** ============================================================================
 *  Globals
 *  ============================================================================
 */
Char* RingIOAppArm_FilePath[RINGIOAPPARM_MAXPROCESSORS]= {

    RingIOApp_DSPFILEPATH,
    RingIOApp_VIDEOFILEPATH,
    RingIOApp_VPSSFILEPATH,
};

static WriterTaskInfo RingIOAppArm_WrTaskInfo[APP_MAX_SLAVES];

/* Handle used for syncing between RingIOApp_execute and tsk_WriterFunc */
static Semaphore_Handle  RingIOAppArm_TaskSemHandle;

/* Defines whether application is to be run with specific slave as indexed here.
 */
static UInt32 RingIOAppArm_toRunProcId [RINGIOAPPARM_MAXPROCESSORS];


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
Void RingIOApp_taskFxn(UArg arg0, UArg arg1);
Void tsk_WriterFunc(UArg arg0, UArg arg1);

Void
RingIOApp_InitBuffer (Ptr buffer, UInt32 size);

Int32 RingIOApp_startup (Void);
Int32 RingIOApp_shutdown (Void);
Int32 RingIOApp_execute (Void);


/* Call back function for writer RingIO */
Void APP_writerRingIOCallback (RingIO_Handle handle, Ptr arg, RingIO_NotifyMsg msg)
{
    Semaphore_post((Semaphore_Object*)arg);
}

Void APPNotify_callbackFxn (UInt16 procId,
                            UInt16 lineNo,
                            UInt32 eventNo,
                            UArg   arg,
                            UInt32 payload)
{
    Semaphore_post((Semaphore_Object*)arg);
}

Int32 APP_ipcInit()
{
    Int32                       status = 0;
    UInt16 remoteProcId;

    /*
     *  Need to define the shared region. The IPC modules use this
     *  to make portable pointers. All processors need to add this
     *  same call with their base address of the shared memory region.
     *  If the processor cannot access the memory, do not add it.
     */
    if (ipcWithDSP == 1) {
        remoteProcId = MultiProc_getId("DSP");
        do {
            status = Ipc_attach(remoteProcId);
        } while (status < 0);

    }

    if (ipcWithVIDEOM3 == 1) {
        remoteProcId = MultiProc_getId("VIDEO-M3");
        do {
            status = Ipc_attach(remoteProcId);
        } while (status < 0);

    }

    if (ipcWithVPSSM3 == 1) {
        remoteProcId = MultiProc_getId("VPSS-M3");
        do {
            status = Ipc_attach(remoteProcId);
        } while (status < 0);

    }


    return (status);

}


int main()
{
    Int status = Ipc_S_SUCCESS;
    do{
    status = Ipc_start();
    if (status < 0) {
            Osal_printf ( "ERROR: Ipc_start Failed status 0x%x \n",status);
    }
    } while (status != Ipc_S_SUCCESS);
    BIOS_start();
    return (0);
}


/*
 *  ======== RingIOApp_taskFxn ========
 *  Calls the  RingIO  setup API.and Spwans tasks to talk to M3Vido and DSS
 */
Void RingIOApp_taskFxn(UArg arg0, UArg arg1)
{
    Int32 status = 0;
    UInt32 remoteProcId;
    UInt32 i;
    Osal_printf("Entered tsk_func()\n");

    /* Initialize all to FALSE */
    for (i = 0 ; i < RINGIOAPPARM_MAXPROCESSORS; i++) {
        RingIOAppArm_toRunProcId [i] = FALSE;
    }

    if (ipcWithDSP == 1) {
        remoteProcId = MultiProc_getId("DSP");
        RingIOAppArm_toRunProcId [remoteProcId] = TRUE;
    }
    if (ipcWithVIDEOM3 == 1) {
        remoteProcId = MultiProc_getId("VIDEO-M3");
        RingIOAppArm_toRunProcId [remoteProcId] = TRUE;
    }
    if (ipcWithVPSSM3 == 1) {
        remoteProcId = MultiProc_getId("VPSS-M3");
        RingIOAppArm_toRunProcId [remoteProcId] = TRUE;
    }

    SysLink_setup();
    status = RingIOApp_startup ();
    if (status >= 0) {
        status = RingIOApp_execute ();
        if (status < 0) {
            Osal_printf("RingIOApp_execute failed!\n");
        }
    }
    else {
        Osal_printf("RingIOApp_startup failed!\n");
    }
    status = RingIOApp_shutdown ();
    if (status < 0) {
        Osal_printf("RingIOApp_shutdown failed!\n");
    }
    //SysLink_destroy ();
    Osal_printf("Leaving tsk_func()\n");
}

/*!
 *  @brief  Function to execute the startup for FrameQ sample application
 *
 *  @sa
 */
Int32 RingIOApp_startup(Void)
{
    Int32    status  = 0;
    UInt16 i = 0;
    Osal_printf("Entered   RingIOApp_startup\n");
    for (i = 0;
         (i < MultiProc_getNumProcessors ()) && (i != MultiProc_self ());
         i++) {
        if (RingIOAppArm_toRunProcId [i]) {
            Osal_printf("Starting slave with procId = %d, file path = %s \n",
                          i,
                          RingIOAppArm_FilePath[i] );

            status = ProcMgrApp_startup (i,
                                         RingIOAppArm_FilePath [i]);
        }
    }
    Osal_printf("Leaving RingIOApp_startup\n");
    return (status);
}

Int32 RingIOApp_shutdown(Void)
{
    Int32    status  = 0;
    UInt16   i = 0;

    for (i = 0;
         (i < MultiProc_getNumProcessors ()) && (i != MultiProc_self ());
         i++) {
        if (RingIOAppArm_toRunProcId [i]) {
            Osal_printf("Shutting down slave with procId = %d \n", i);

            status = ProcMgrApp_shutdown (i);
        }
    }
    return (status);
}

Int32 RingIOApp_execute(Void)
{
    Int32                   status = 0;
    Task_Handle             tskWriterHandle;
    Task_Params             tskParams;
    UInt16                  remoteProcId;
    Error_Block                 eb;
    Semaphore_Params            semParams;

    Osal_printf ("Entered RingIOApp_execute()\n");

    status = APP_ipcInit();

    if (status < 0) {
       Osal_printf ( "ERROR: APP_ipcInit Failed \n");
       return status;
    }
    Error_init(&eb);
    /* Create Counting Semaphore for task notifications*/
    Semaphore_Params_init (&semParams);
    semParams.mode = Semaphore_Mode_COUNTING;
    RingIOAppArm_TaskSemHandle = Semaphore_create (0, &semParams, &eb);
    if (RingIOAppArm_TaskSemHandle == NULL) {
        Error_check (&eb);
        Osal_printf("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__);
        return(-1);
    }

    /* Spawn the tasks */
    if  (RingIOAppArm_toRunProcId[MultiProc_getId("DSP")]) {
        /* Create the task  to produce frames to DSP */
        remoteProcId = MultiProc_getId("DSP");
        Task_Params_init(&tskParams);
        tskParams.stackSize = 0x4000;
        strcpy(RingIOAppArm_WrTaskInfo[remoteProcId].rioName,RINGIO_INST1_NAME);
        /* let tasks create it internally */
        RingIOAppArm_WrTaskInfo[remoteProcId].semHandle = NULL;
        tskParams.arg0 = (UArg)remoteProcId;
        tskParams.arg1 = (UArg)&(RingIOAppArm_WrTaskInfo [remoteProcId]);
        tskWriterHandle = Task_create (tsk_WriterFunc,
                                       &tskParams,
                                       NULL);
        if (tskWriterHandle == NULL) {
            Osal_printf ( "ERROR: Failed to create task tsk_WriterFunc \n");
        }
        Semaphore_pend(RingIOAppArm_TaskSemHandle, BIOS_WAIT_FOREVER);
    }

    /* Spawn the tasks */
    if  (RingIOAppArm_toRunProcId[MultiProc_getId ("VIDEO-M3")]) {
        /* Create the task  to produce frames to VIDEO */
        remoteProcId = MultiProc_getId("VIDEO-M3");
        Task_Params_init(&tskParams);
        tskParams.stackSize = 0x4000;
        strcpy(RingIOAppArm_WrTaskInfo[remoteProcId].rioName,RINGIO_INST2_NAME);
        /* let tasks create it internally */
        RingIOAppArm_WrTaskInfo[remoteProcId].semHandle = NULL;
        tskParams.arg0 = (UArg)remoteProcId ;
        tskParams.arg1 = (UArg)&(RingIOAppArm_WrTaskInfo [remoteProcId]);
        tskWriterHandle = Task_create (tsk_WriterFunc,
                                       &tskParams,
                                       NULL);
        if (tskWriterHandle == NULL) {
            Osal_printf ( "ERROR: Failed to create task tsk_WriterFunc \n");
        }
        Semaphore_pend(RingIOAppArm_TaskSemHandle, BIOS_WAIT_FOREVER);
    }

    /* Spawn the tasks */
    if  (RingIOAppArm_toRunProcId[MultiProc_getId ("VPSS-M3")]) {
        /* Create the task  to produce frames to DSS */
        remoteProcId = MultiProc_getId("VPSS-M3");
        Task_Params_init(&tskParams);
        tskParams.stackSize = 0x4000;
        strcpy(RingIOAppArm_WrTaskInfo[remoteProcId].rioName,RINGIO_INST3_NAME);
        /* let tasks create it internally */
        RingIOAppArm_WrTaskInfo[remoteProcId].semHandle = NULL;
        tskParams.arg0 = (UArg)remoteProcId;
        tskParams.arg1 = (UArg)&(RingIOAppArm_WrTaskInfo [remoteProcId]);
        tskWriterHandle = Task_create (tsk_WriterFunc,
                                       &tskParams,
                                       NULL);
        if (tskWriterHandle == NULL) {
            Osal_printf ( "ERROR: Failed to create task tsk_WriterFunc \n");
    }
        Semaphore_pend(RingIOAppArm_TaskSemHandle, BIOS_WAIT_FOREVER);
}

    Osal_printf ("Leave RingIOApp_execute()\n");
    return (status);

}
/*
 *  ======== tsk_WriterFunc ========
 */
Void tsk_WriterFunc(UArg arg0, UArg arg1)
{
    Int32                   status    = 0;
    UInt16                  remoteProcId    = (UInt16) arg0;
    WriterTaskInfo          *taskInfo       = (WriterTaskInfo *) arg1;
    RingIO_BufPtr           bufPtr    = NULL;
    UInt32                  acqSize   = 100;
    Semaphore_Params        semParams;
    Error_Block             eb;
    RingIO_Handle           rioHandle;
    RingIO_Handle           wrioHandle;
    RingIOShm_Params        rioShmParams;
    RingIO_openParams       openRioParams;
    UInt16                  type;


    Osal_printf ("Entered tsk_WriterFunc()\n");
    Error_init (&eb);
    /* Create Counting Semaphore for RingIO notifications*/
    Semaphore_Params_init (&semParams);
    semParams.mode = Semaphore_Mode_COUNTING;
    taskInfo->semHandle = Semaphore_create (0, &semParams, &eb);
    if (taskInfo->semHandle == NULL) {
        Error_check (&eb);
        Osal_printf ("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__);
        return;
    }
    /* Register with the remote processor*/
    Notify_registerEvent(remoteProcId,
                         0,
                         APPNOTIFY_EVENT_NO,
                         APPNotify_callbackFxn,
                         (UArg) taskInfo->semHandle);
    Osal_printf("registered for notification from procid %d on event no %d\n",
                remoteProcId,APPNOTIFY_EVENT_NO);
    RingIOShm_Params_init (&rioShmParams);
    rioShmParams.commonParams.name  = taskInfo->rioName;
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

    status = RingIO_open (taskInfo->rioName,
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
            Osal_printf ("Writer: Passed to register call back with RingIO status: [0x%x]\n",status);
        }
        else {
            Osal_printf ("Writer: Failed to register call back with RingIO status: [0x%x]\n",status);
        }
    }

    if (status >= 0) {
        status = RingIO_acquire (wrioHandle, &bufPtr , &acqSize) ;
        if (status >= 0) {
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%d\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO acquire for Writer failed :%x\n", status);
            Osal_printf  ("RingIO acquire for Writer failed:%d\n",acqSize );
        }
    }

    if (status >= 0) {
        RingIOApp_InitBuffer (bufPtr, acqSize) ;
    }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (wrioHandle, acqSize) ;
        if (status >= 0) {
            Osal_printf  ("RingIO release for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO release for Writer succeeded:%d\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO release for Writer failed :%x\n", status);
            Osal_printf  ("RingIO release for Writer failed:%d\n",acqSize );
        }
    }


    if (RingIO_getValidSize (wrioHandle) == 100) {
        Osal_printf ( "Writer has produced valid data\n");
    }

    if (status >= 0) {
        /* Send data transfer attribute (Fixed attribute) */
        type = (UInt16) 1;
        status = RingIO_setAttribute (wrioHandle, type, 0, FALSE);
    if (status >= 0) {
        Osal_printf  ("RingIO setAttribute for Writer succeeded:%x\n", status);
    }
    else {
        Osal_printf  ("RingIO setAttribute for Writer failed :%x\n", status);
    }
    }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (wrioHandle, &bufPtr , &acqSize) ;
        if (status >= 0) {
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%d\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO acquire for Writer failed :%x\n", status);
            Osal_printf  ("RingIO acquire for Writer failed:%d\n",acqSize );
        }
    }

    if (status >= 0) {
        status = RingIO_cancel (wrioHandle) ;
    }

    if (status >= 0) {
        if (RingIO_getAcquiredSize (wrioHandle) == 0) {
            Osal_printf ( "Writer has cancelled acquired data\n");
        }
    }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (wrioHandle, &bufPtr , &acqSize) ;
        if (status >= 0) {
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%d\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO acquire for Writer failed :%x\n", status);
            Osal_printf  ("RingIO acquire for Writer failed:%d\n",acqSize );
        }
    }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (wrioHandle, acqSize) ;
        if (status >= 0) {
            Osal_printf  ("RingIO release for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO release for Writer succeeded:%d\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO release for Writer failed :%x\n", status);
            Osal_printf  ("RingIO release for Writer failed:%d\n",acqSize );
        }
    }

        status = RingIO_sendNotify (wrioHandle, 0x1234);
    Osal_printf  ("Sending notification to reader status = %x\n",status);

    status = RingIO_close (&wrioHandle);
    Semaphore_pend((Semaphore_Object*)taskInfo->semHandle, BIOS_WAIT_FOREVER);
    Osal_printf("Got completeion confirmation from proc %d\n",remoteProcId);
    status = RingIO_delete (&rioHandle);
    Semaphore_post((Semaphore_Object*)RingIOAppArm_TaskSemHandle);
    Osal_printf ("Leave tsk_WriterFunc()\n");

}


Void
RingIOApp_InitBuffer (Ptr buffer, UInt32 size)
{

    UInt8 *    ptr8   = (UInt8 *)  (buffer) ;
    Int16      i ;

    if (buffer != NULL) {
        for (i = 0 ; i < (size) ; i++) {
            *ptr8 = XFER_VALUE ;
            ptr8++ ;
        }
    }
}
