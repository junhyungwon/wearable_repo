/*
 *  @file   FrameQ_ti81xx_intracore.c
 *
 *  @brief      Sample application for FrameQ. Shows the API calling sequence
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
#include <xdc/runtime/Assert.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/GateMP.h>
#include <ti/ipc/NameServer.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>
#include <ti/ipc/Ipc.h>

#include <ti/syslink/SysLink.h>

/* Shim layer includes */
#include <ti/syslink/FrameQ.h>
#include <ti/syslink/FrameQ_ShMem.h>
#include <ti/syslink/FrameQBufMgr.h>
#include <ti/syslink/FrameQBufMgr_ShMem.h>


#include "FrameQ_sample.h"

/*  ------------------------To get globals from .cfg Header ******************/
#include <xdc/cfg/global.h>

#define IPC_BUFFER_ALIGN(x, y) (UInt32)((UInt32)((x + y - 1) / y) * y)

Void tsk_func(UArg arg0, UArg arg1);
Void tsk_WriterFunc(UArg arg0, UArg arg1);
Void tsk_ReaderFunc(UArg arg0, UArg arg1);

//#define TRACE_FRAME_READ
//#define TRACE_FRAME_WRITES

static WriterTaskInfo wrTaskInfo;
static ReaderTaskInfo rdTaskInfo;
Char   tempStr[80];
Char   tempStr1[80];
Char   tempStr3[80];

FrameQBufMgr_Handle     bufMgrHandle[APP_FQ_INSTANCES];
FrameQ_Handle           frmQCrehandle[APP_FQ_INSTANCES];
FrameQ_Handle           frmQWriterhandle[APP_FQ_INSTANCES];
FrameQ_Handle           frmQReaderHandle[APP_FQ_INSTANCES];

FrameQBufMgr_ShMem_FrameBufParams  frameBufferInfo[APP_NUM_FREE_FRMPOOL][APP_NUM_FRAMEBUFS_IN_FRAME]= {{0},};

/* Call back function for reader FrameQ */
Void APP_readerFrameQCallback(FrameQ_Handle handle, Ptr arg, FrameQ_NotifyMsg msg)
{
    Semaphore_post((Semaphore_Object*)arg);
}

/* Call back function for writer FrameQ.
 * Call back will be invoked by FrameQBufMgr when free buffers are available
 */
Void APP_writerFrameQCallback(FrameQBufMgr_Handle handle, Ptr arg, FrameQ_NotifyMsg msg)
{
    Semaphore_post((Semaphore_Object*)arg);
}


Void APPWriterNotify_callbackFxn (UInt16 procId,
                            UInt16 lineNo,
                            UInt32 eventNo,
                            UArg   arg,
                            UInt32 payload)
{
    Semaphore_post((Semaphore_Object*)arg);
}

Void APPReaderNotify_callbackFxn (UInt16 procId,
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
    SharedRegion_Entry          srInfo;
    /*
     *  Need to define the shared region. The IPC modules use this
     *  to make portable pointers. All processors need to add this
     *  same call with their base address of the shared memory region.
     *  If the processor cannot access the memory, do not add it.
     */
    /* Need to zero out the shared memory only if initSharedMemory is set to
     * 1 in cfg file
     */
    if (initSharedMemory == 1) {
        memset((Void *)SHAREDMEM, 0, SHAREDMEMSIZE);
    }

    /*
     *  Need to define the shared region. The IPC modules use this
     *  to make portable pointers. All processors need to add this
     *  same call with their base address of the shared memory region.
     *  If the processor cannot access the memory, do not add it.
     */
    srInfo.isValid        = TRUE;
    srInfo.base           = (Ptr)SHAREDMEM;
    srInfo.len            = SHAREDMEMSIZE;
    srInfo.ownerProcId    = APP_SHAREDREGION_OWNERPROCID;
    srInfo.cacheEnable    = FALSE; /* Setting False for shared region that is
                                    * going to be used with in processor
                                    */
    srInfo.cacheLineSize  = 128;
    srInfo.createHeap  = TRUE; /* Create Heap */

    status = SharedRegion_setEntry (APP_SHARED_REGION_INDEX,
                                    &srInfo);
    return (status);

}


int main()
{
    Task_Handle             tskHandle;
    Task_Params             tskParams;
    Int32                   status;

    Task_Params_init(&tskParams);
    tskParams.stackSize = 0x2000;
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
 *  Calls the  FrameQ  setup API.and Spwans tasks to talk to M3Vido and VPSSM3
 */
Void tsk_func(UArg arg0, UArg arg1)
{

    Task_Handle             tskWriterHandle;
    Task_Handle             tskReaderHandle;
    Task_Params             tskParams;
    Int32                   status;


    System_printf ("Entered tsk_func()\n");

    SysLink_setup();
    
    status = APP_ipcInit();

    if (status < 0) {
       System_printf ( "ERROR: APP_ipcInit Failed \n");
       return;
    }

    /* Spawn the Writer and Reader tasks */
     Task_Params_init(&tskParams);
    tskParams.stackSize = 0x3000;
    strcpy(wrTaskInfo.frmQBufMgrname,FRAMEQBUFMGR_INST1_NAME);
    strcpy(wrTaskInfo.frmQName,FRAMEQ_INST1_NAME);
    /* let tasks create it internally */
    wrTaskInfo.semHandle = NULL;
    tskParams.arg1 = (UArg)&wrTaskInfo;
    tskWriterHandle = Task_create (tsk_WriterFunc,
                                   &tskParams,
                                   NULL);
    if (tskWriterHandle == NULL) {
        System_printf ( "ERROR: Failed to crate task tsk_WriterFunc \n");
    }

    Task_Params_init(&tskParams);
    tskParams.stackSize = 0x2000;
    strcpy(rdTaskInfo.frmQBufMgrname,FRAMEQBUFMGR_INST1_NAME);
    strcpy(rdTaskInfo.frmQName,FRAMEQ_INST1_NAME);
    /* let tasks create it internally */
    rdTaskInfo.semHandle = NULL;
    tskParams.arg0 = (UArg)&rdTaskInfo;
    tskParams.priority = 11;
    tskReaderHandle = Task_create (tsk_ReaderFunc,
                                   &tskParams,
                                   NULL);
    if (tskReaderHandle == NULL) {
        System_printf ( "ERROR: Failed to crate task tsk_ReaderFunc \n");
    }

    System_printf ("Leave tsk_func()\n");
}


/*
 *  ======== tsk_WriterFunc ========
 */
Void tsk_WriterFunc(UArg arg0, UArg arg1)
{
    Int32                   status          = 0;
    UInt32                  i               = 0;
    FrameQBufMgr_ShMem_Params instParams;
    FrameQ_ShMem_Params     frmQInstParams;
    FrameQ_Frame            frame = NULL;
    UInt32                  j;
    UInt8                   *frmBuf;
    UInt32                  frmCnt;
    Semaphore_Params        semParams;
    Error_Block             eb;
    UInt32                  headerBufChunkSize;
    UInt32                  frmBufChunkSize;
    UInt32                  lBaseHeaderSize;
    UInt32                  k;
    FrameQ_NotifyParams     frmQNotifyParams;
    FrameQ_ShMem_OpenParams frmQOpenParams;
    WriterTaskInfo          *taskInfo = &wrTaskInfo;
#if 0
    FrameQ_Frame            dupFrame = NULL;
#endif
//    WriterTaskInfo          *taskInfo = arg0;

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

    /* Register with the VPSSM3 processor*/
    Notify_registerEvent(MultiProc_self(),
                         0,
                         APP_WRITER_NOTIFY_EVENT_NO,
                         APPWriterNotify_callbackFxn,
                         (UArg) taskInfo->semHandle);

    /* Get the default values for the FrameQBufMgr instance params */
    FrameQBufMgr_ShMem_Params_init(&instParams);

    /* Set the create paramters common to all the implementations */
    instParams.commonCreateParams.size = sizeof(FrameQBufMgr_ShMem_Params);
    /* Set the ShMem implementation  for this FrameQBufMgr instance */
    instParams.commonCreateParams.ctrlInterfaceType =
                                               FrameQBufMgr_INTERFACE_SHAREDMEM;
    instParams.headerInterfaceType  = FrameQBufMgr_HDRINTERFACE_SHAREDMEM;

    instParams.bufInterfaceType = FrameQBufMgr_BUFINTERFACE_SHAREDMEM ;


    /*Set the ShMem specific paramters */

    /* Configure it for one free frame pool */
    instParams.numFreeFramePools = 1u;

    for (i = 0; i < instParams.numFreeFramePools; i++) {
        /* How many frames in this pool */
        instParams.numFreeFramesInPool[i] = 10;
        /* How many extra header buffers in this pool
         * For dup purposes.
         */
        instParams.numFreeHeaderBufsInPool[i] = 5u;

        /* How many frame buffers  frame should contain.
         */
        instParams.numFrameBufsInFrame[i] = 1u;

        /* Calculate the base frame header size for the frame containing
         * instParams.numFrameBufsInFrame[i].
         */
         lBaseHeaderSize =
              FrameQBufMgr_getBaseHeaderSize(instParams.numFrameBufsInFrame[i]);

         /* Align base header size to cache aligend */
        instParams.frameHeaderBufSize[i] = IPC_BUFFER_ALIGN(lBaseHeaderSize,
                                         SharedRegion_getCacheLineSize(
                                                      APP_SHARED_REGION_INDEX));
        /* What are the sizes of frame buffers */

        for (k = 0; k < instParams.numFrameBufsInFrame[i]; k++) {
             frameBufferInfo[i][k].size = IPC_BUFFER_ALIGN(
                                                   128,
                                          SharedRegion_getCacheLineSize(
                                                      APP_SHARED_REGION_INDEX));
        }
    }

    for (i = 0; i < instParams.numFreeFramePools; i++) {
            /* What are the sizes of frame buffers */
            instParams.frameBufParams[i] = frameBufferInfo[i];
    }

    instParams.sharedAddrSize = FrameQBufMgr_ShMem_sharedMemReq (
                                                           &instParams,
                                                           &headerBufChunkSize,
                                                           &frmBufChunkSize);
    /* Allocate instParams.sharedAddrSize memory from shared region for this
     * instance
     */
    /* Pass the shared region id  to allocate internal shared control structures from */
    instParams.regionId = APP_SHARED_REGION_INDEX;

    /* Pass the shared region id  to allocate frame Headers from */
    instParams.hdrBuf_regionId = APP_SHARED_REGION_INDEX;
    /* Pass the shared region id  to allocate frame Frame buffers from */
    instParams.frmBuf_regionId = APP_SHARED_REGION_INDEX;

    instParams.localProtect    = APP_INST_LOCALPROTECTION;
    instParams.remoteProtect   = GateMP_RemoteProtect_CUSTOM1;

    instParams.cpuAccessFlags   = (
                                FrameQBufMgr_FRAMEBUF0_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF1_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF2_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF3_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF4_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF5_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF6_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF7_CPUACCESS);
    instParams.numNotifyEntries = 1;/* Only one client( FrameQ writer) is going
                                     * to use notifications of FrameQBufMgr
                                     */

    for (j = 0;j < APP_FQ_INSTANCES;j++) {
        memset (tempStr, 0, 80);
        System_sprintf((String)tempStr,"%s%d",taskInfo->frmQBufMgrname,j);
        instParams.commonCreateParams.name = tempStr;
        do {
            bufMgrHandle[j] =  FrameQBufMgr_create(&instParams);
            if (bufMgrHandle[j] == NULL) {
                System_printf ("FrameQBufMgr_create  failed for %s \n",
                               tempStr);
            }
            else {
                System_printf ("Success:Created FrameQBufMgr %s:bufMgrHandle 0x%x\n",
                               tempStr,
                               bufMgrHandle[j]);

            }
        } while (bufMgrHandle[j] == NULL);


        /**************** FrameQ instance creation **********************/
        /* Get the default values for the FrameQ instance params */
        FrameQ_ShMem_Params_init(&frmQInstParams);
        memset (tempStr1, 0, 80);
        System_sprintf((String)tempStr1,"%s%d",taskInfo->frmQName,j);

        frmQInstParams.commonCreateParams.name = tempStr1;
        frmQInstParams.commonCreateParams.size = sizeof(FrameQ_ShMem_Params);
        frmQInstParams.commonCreateParams.ctrlInterfaceType =
                                                         FrameQ_INTERFACE_SHAREDMEM;
        /* Set it for  only one reader */
        frmQInstParams.numReaders = 1; /* Set it for one reader  */
        frmQInstParams.numQueues  = 1; /* Set it for one queue for reader*/
        frmQInstParams.cpuAccessFlags = (FrameQ_FRAMEBUF0_CPUACCESS |
                                    FrameQ_FRAMEBUF1_CPUACCESS |
                                    FrameQ_FRAMEBUF2_CPUACCESS |
                                    FrameQ_FRAMEBUF3_CPUACCESS |
                                    FrameQ_FRAMEBUF4_CPUACCESS |
                                    FrameQ_FRAMEBUF5_CPUACCESS |
                                    FrameQ_FRAMEBUF6_CPUACCESS |
                                    FrameQ_FRAMEBUF7_CPUACCESS);
        frmQInstParams.localProtect  = APP_INST_LOCALPROTECTION;
        frmQInstParams.remoteProtect = GateMP_RemoteProtect_CUSTOM1;
         /* Pass the name of the FrameQBufMgr  to be used in this FrameQ*/
        frmQInstParams.frameQBufMgrName = tempStr;

        System_printf ("Shared memory consumed for FrameQ instance 0x%x \n",
                       FrameQ_ShMem_sharedMemReq (&frmQInstParams));

        /* Pass the shared region id  to allocate internal shared control structures from */
        frmQInstParams.regionId = APP_SHARED_REGION_INDEX;

        do {
            frmQCrehandle[j] = FrameQ_create (&frmQInstParams);
            if (frmQCrehandle[j] == NULL) {
                System_printf ("FrameQ_create  failed for %s \n",
                               frmQInstParams.commonCreateParams.name);
            }
            else {
                System_printf ("FrameQ_create  success for %s ",
                frmQInstParams.commonCreateParams.name);
                System_printf ("Created FrameQ  0x%x \n",frmQCrehandle[j]);

            }
        } while (frmQCrehandle[j] == NULL);

       /**************** Open FrameQ instance in write mode  **********************/
        /* Open using name */
        frmQOpenParams.commonOpenParams.name = tempStr1;
        frmQOpenParams.commonOpenParams.openMode = FrameQ_MODE_WRITER;
        frmQOpenParams.commonOpenParams.cpuAccessFlags =
                                   (FrameQ_FRAMEBUF0_CPUACCESS |
                                    FrameQ_FRAMEBUF1_CPUACCESS |
                                    FrameQ_FRAMEBUF2_CPUACCESS |
                                    FrameQ_FRAMEBUF3_CPUACCESS |
                                    FrameQ_FRAMEBUF4_CPUACCESS |
                                    FrameQ_FRAMEBUF5_CPUACCESS |
                                    FrameQ_FRAMEBUF6_CPUACCESS |
                                    FrameQ_FRAMEBUF7_CPUACCESS);

        do {
            status = FrameQ_open (&frmQOpenParams, &frmQWriterhandle[j]);
            if (status < 0){
                System_printf ("ERROR:FrameQ_open instance failed for writer."
                               "status: 0x%x \n",
                               status);

            }
            else {
                 System_printf ("FrameQ_open  success for %s writer handle 0x%x."
                                "status: 0x%x\n",tempStr1, frmQWriterhandle[j],
                                status);
            }

        } while ((frmQWriterhandle[j] == NULL) || (status < 0));


        status = FrameQ_getNumFreeFrames(frmQWriterhandle[j], &i);
        if  (status >= 0) {
            System_printf ("FrameQ writer:NumBer of Free Fraemes available"
                           "to allocate %d\n", i);
        }

       /* Register call back fucntion   with the FrameQ for free frames */
        frmQNotifyParams.notifyType = FrameQ_NOTIFICATION_ONCE;
        frmQNotifyParams.cbFxnPtr   =
                                    (FrameQ_NotifyFunc)APP_writerFrameQCallback;
        frmQNotifyParams.watermark = 1; /* Water mark one free frame */
        frmQNotifyParams.cbContext = taskInfo->semHandle; /*Context pointer */
        frmQNotifyParams.cbClientHandle = frmQWriterhandle[j];

       /* Register call back fucntion   with the FrameQbufMgr created */
        status = FrameQ_registerNotifier (frmQWriterhandle[j], &frmQNotifyParams);
        if (status < 0) {
         System_printf ("ERROR: Failed to register call back with FrameQ writer"
                        "For free buffers.status: 0x%x\n",
                        status);
        }
    }

    /* Sleep for a while before trying again. */
    Task_sleep (10);

    status = Notify_sendEvent(MultiProc_self(),
                              0,
                              APP_READER_NOTIFY_EVENT_NO,
                              0,
                              TRUE);
    if (status >= 0) {
        System_printf ("Notify_sendEvent for synchronization to Reader is "
                       "successful. status :0x%x\n",
                       status);
    }
    else {
        System_printf ("Notify_sendEvent for synchronization to Reader is "
                       "Failed.status :0x%x\n",
                       status);
    }

    for (j = 0; j < APP_FQ_INSTANCES; j++) {
        /* Send force notification to one reader client */
        do {
            status = FrameQ_sendNotify (frmQWriterhandle[j],
                                        APP_FRAMEQ_FORCE_NOTIFCATION_MSG);
            if (status < 0) {
                 System_printf ("ERROR: Failed to send force notification to  FrameQ instance %d reader."
                                "status: 0x%x\n",j,
                                 status);
                /* Sleep for a while before trying again. */
                Task_sleep (10);
            }
            else {
                System_printf ("FrameQ Writer: Sent force notification to FrameQ instance %d reader."
                               "status: 0x%x\n",j,
                                status);
            }

        }
        while (status < 0);

       /* Allocate frame by writer */
        for (i = 0; i <  ITER_COUNT; i++) {
            do {
                status = FrameQ_alloc (frmQWriterhandle[j],&frame);
                if (status < 0) {
    #if defined TRACE_FRAME_WRITES
                    System_printf ("FrameQWriter:FrameQ_alloc Failed.Waiting for Notification...\n");
    #endif
                    Semaphore_pend(taskInfo->semHandle, BIOS_WAIT_FOREVER);
    #if defined TRACE_FRAME_WRITES
                    System_printf ("FrameQWriter:Got notification...\n");
    #endif
                }
           } while (status < 0);
    #if defined TRACE_FRAME_WRITES
           System_printf ("FrameQWriter:FrameQ_alloc succesful "
                          "status:0x%08x:IterCount %d\n",
                          status,
                          i);
    #endif
           if (frame != NULL){
                 /* Process the retrieved frame here */
    #if defined TRACE_FRAME_WRITES
                System_printf ("Frame Address          :0x%x\n",frame);
    #endif
                frmCnt = FrameQ_getNumFrameBuffers (frame);
    #if defined TRACE_FRAME_WRITES
                System_printf ("Number Of Frame Buffers:%d\n",frmCnt );
    #endif

                for (k = 0; k < frmCnt; k++) {
                    frmBuf = FrameQ_getFrameBuffer(frame, k);
    #if defined TRACE_FRAME_WRITES
                    System_printf ("FrameBuffer Address :0x%x\n",(UInt32)frmBuf);
                    /* Here populte the frame extended header and frame buffers */
                    System_printf ("FrameBuffer size :0x%x\n",
                                   FrameQ_getFrameBufSize(frame, k) );
    #endif
                    status = FrameQ_setFrameBufDataStartOffset (frame ,k, 0);
                    status = FrameQ_setFrameBufValidSize(frame , k, 4);
                    frmBuf[0] = 0x1A;
                    frmBuf[1] = 0x1B;
                    frmBuf[2] = 0x1C;
                    frmBuf[3] = 0x1D;
                }
                /* Free the frame  */
                if (frame != NULL) {
#if 0
                    /* Call dup to duplicate frame */
                    status = FrameQ_dup (frmQWriterhandle[j], frame, &dupFrame);
                    if (dupFrame != NULL) {
                         /* Put the frame in to FrameQ */
                        status = FrameQ_put (frmQWriterhandle[j], dupFrame);
                        i++;
                    }
#endif
                     /* Put the frame in to FrameQ */
                    status = FrameQ_put (frmQWriterhandle[j], frame);
               }
            }
            if (((i % 100) == 0) && (i > 0)) {
               System_printf ("FrameQWriter:Transferred %d frames \n", i);
            }

        }
        if (i == ITER_COUNT) {
           System_printf ("FrameQWriter of FrameQ instance %d:Transferred %d frames \n",j, i);
           i = 0;
        }
    }
    for (j = 0; j < APP_FQ_INSTANCES; j++) {
        /* Wait for  Reader to consume all the frames */
        do {
           status = FrameQ_getNumFrames(frmQWriterhandle[j], &frmCnt);
            /* Sleep for a while before trying again. */
            Task_sleep (10);
        } while (frmCnt != 0);

       /* Close the  FrameQ handles */
        status = FrameQ_close (&frmQWriterhandle[j]);
    }

    /* Delete is force ful. So wait for the other side to finish up */
    Semaphore_pend( taskInfo->semHandle, BIOS_WAIT_FOREVER);

    for (j = 0; j < APP_FQ_INSTANCES; j++) {
         /* Delete FrameQ */
        FrameQ_delete (&frmQCrehandle[j]);

        /* Delete the FrameQ BufMgr instance */
        FrameQBufMgr_delete (&bufMgrHandle[j]);
    }

    System_printf ("Leave tsk_WriterFunc()\n");
}

/* Reader task  retrieves Frames from writer */
Void tsk_ReaderFunc(UArg arg0, UArg arg1)
{
    Int32                   status        = 0;
    UInt32                  i             = 0;
    /* FrameQBufMgr instance params */
#if defined TRACE_FRAME_READ
    UInt8                   *frmbuf ;
    UInt32                  frmCnt;
    UInt32                  k;
#endif
    FrameQ_Frame           outFrame;
    Semaphore_Params        semParams;
    Error_Block             eb;
    ReaderTaskInfo          *taskInfo = &rdTaskInfo;
//  ReaderTaskInfo          *taskInfo = arg0;
    FrameQ_ShMem_OpenParams frmQOpenParams;
    FrameQ_NotifyParams     frmQNotifyParams;
    UInt32                  j;

    System_printf ( " In tsk_ReaderFunc\n");
    Error_init (&eb);

    /* Create Binary Semaphore for FramQ notifications*/
    Semaphore_Params_init (&semParams);
    semParams.mode = Semaphore_Mode_COUNTING;
    taskInfo->semHandle = Semaphore_create (0, &semParams, &eb);
    if (taskInfo->semHandle == NULL) {
        Error_check (&eb);
        System_printf ("Failed to Create the semaphore  in Reader task."
                       "exiting ....%s: %d :\n",
                       __FILE__, __LINE__);
        return;
    }

    Notify_registerEvent(MultiProc_self(),
                         0,
                        APP_READER_NOTIFY_EVENT_NO ,
                        APPReaderNotify_callbackFxn,
                        (UArg) taskInfo->semHandle);
    /* Retrieve frame from FrameQ */
    System_printf ("FrameQReader:Waiting for Synchronizatioon notification"
                   "from FrameQ writer\n");
    Semaphore_pend(taskInfo->semHandle, BIOS_WAIT_FOREVER);
    System_printf ("FrameQReader:Got Synchronizatioon notification from FrameQ "
                   "writer\n");

    for (j = 0; j < APP_FQ_INSTANCES; j++) {

        /**************** Open FrameQ instance in Read  mode  **********************/
        memset (tempStr3, 0, 80);
        System_sprintf((String)tempStr3,"%s%d",taskInfo->frmQName, j);

        frmQOpenParams.commonOpenParams.name = tempStr3;
        frmQOpenParams.commonOpenParams.openMode = FrameQ_MODE_READER;
        frmQOpenParams.commonOpenParams.cpuAccessFlags =
                                   (FrameQ_FRAMEBUF0_CPUACCESS |
                                    FrameQ_FRAMEBUF1_CPUACCESS |
                                    FrameQ_FRAMEBUF2_CPUACCESS |
                                    FrameQ_FRAMEBUF3_CPUACCESS |
                                    FrameQ_FRAMEBUF4_CPUACCESS |
                                    FrameQ_FRAMEBUF5_CPUACCESS |
                                    FrameQ_FRAMEBUF6_CPUACCESS |
                                    FrameQ_FRAMEBUF7_CPUACCESS);

        do {
            status  = FrameQ_open (&frmQOpenParams, &frmQReaderHandle[j]);
            if ((frmQReaderHandle[j] == NULL)|| (status < 0)){
                System_printf ("ERROR:Failed to open FrameQ in Read mode."
                               "status 0x%x\n",
                               status);
                /* Sleep for a while before trying again. */
                Task_sleep (10);
           }
        } while (frmQReaderHandle[j] == NULL);

        if (frmQReaderHandle[j] != NULL){
            System_printf ("Opened FrameQ in Read mode for %s\n",tempStr3);
        }

        frmQNotifyParams.notifyType = FrameQ_NOTIFICATION_ONCE;
        frmQNotifyParams.cbFxnPtr   =
                                    (FrameQ_NotifyFunc)APP_readerFrameQCallback;
        frmQNotifyParams.watermark = 1; /* Water mark one frame */
        frmQNotifyParams.cbContext = taskInfo->semHandle; /*Context pointer */
        frmQNotifyParams.cbClientHandle = frmQReaderHandle[j];
        status = FrameQ_registerNotifier (
                                          frmQReaderHandle[j],&frmQNotifyParams);
        /* Retrieve frame from FrameQ */
        System_printf ("FrameQReader:Waiting for Force notification from FrameQ "
                       "writer\n");
        Semaphore_pend(taskInfo->semHandle, BIOS_WAIT_FOREVER);
        System_printf ( "FrameQReader:Got Force notification from FrameQ "
                        "writer\n");

        for (i = 0; i <  ITER_COUNT; i++) {
            do {
                status = FrameQ_get (frmQReaderHandle[j], &outFrame );
                if (status < 0) {
#if defined TRACE_FRAME_READ
                    System_printf ("FrameQReader:FrameQ_get Failed."
                                   "Waiting for Notification...\n");
#endif
                    Semaphore_pend(taskInfo->semHandle, BIOS_WAIT_FOREVER);
#if defined TRACE_FRAME_READ
                    System_printf ("FrameQReader:Got notification...\n");
#endif
                }

           } while ((status < 0));
#if defined TRACE_FRAME_READ
           System_printf ("FrameQ Reader:FrameQ_get succesful."
                          "status:0x%08x:IterCount %d\n",
                          status,
                          i);
#endif
           if (outFrame != NULL){
                 /* Process the retrieved frame here */
#if defined TRACE_FRAME_READ
                System_printf ("Frame Address          :0x%x\n",outFrame);
                frmCnt = FrameQ_getNumFrameBuffers (outFrame);
                System_printf ("Number Of Frame Buffers:%d\n",frmCnt );
                for (k = 0; k < frmCnt; k++) {
                    frmbuf = FrameQ_getFrameBuffer(outFrame, k);
                    System_printf ("FrameBuffer Address :0x%x\n",frmbuf);
                    System_printf ("FrameBuffer Byte  0 val:0x%x\n",frmbuf[0]);
                    System_printf ("FrameBuffer Byte  1 val:0x%x\n",frmbuf[1]);
                    System_printf ("FrameBuffer Byte  2 val:0x%x\n",frmbuf[2]);
                    System_printf ("FrameBuffer Byte  3 val:0x%x\n",frmbuf[3]);
                }
#endif
                /* Free the frame  */
                status = FrameQ_free (frmQReaderHandle[j],outFrame );
            }
            if (((i % 100) == 0) && (i > 0)) {
                System_printf ("FrameQReader:Received %d frames \n", i);
            }
        }
        if (i == ITER_COUNT) {
            System_printf ("FrameQReader of FrameQ instance %d:Received %d frames \n",j, i);
            i = 0;
        }
        status = FrameQ_close (&frmQReaderHandle[j]);
        if (status >= 0) {
            System_printf ("FrameQ_close successful For %s. status :0x%x\n",tempStr3,status);
        }
        else {
            System_printf ("FrameQ_close failed.For %s status :0x%x\n",tempStr3,status);
        }

    }
    status = Notify_sendEvent(MultiProc_self(),
                              0,
                              APP_WRITER_NOTIFY_EVENT_NO,
                              0,
                              TRUE);
    if (status >= 0) {
        System_printf ("Notify_sendEvent for synchronization to Video is "
                       "successful. status :0x%x\n",
                       status);
    }
    else {
        System_printf ("Notify_sendEvent for synchronization to Video is "
                       "Failed.status :0x%x\n",
                       status);
    }
    System_printf ( " Leaving tsk_ReaderFunc ...\n");
}

