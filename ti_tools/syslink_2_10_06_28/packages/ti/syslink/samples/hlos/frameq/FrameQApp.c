/*
 *  @file   FrameQApp.c
 *
 *  @brief      Sample application for FrameQ on Netra/OMAP3530/OMAPL1XX.
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
#endif

/* Standard headers */
#include <ti/syslink/Std.h>

#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/GateMP.h>
#include <ti/ipc/Notify.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/syslink/utils/String.h>

#include <ti/syslink/ProcMgr.h>
#include <ti/syslink/SysLink.h>
#include <ti/syslink/FrameQBufMgr.h>
#include <ti/syslink/FrameQ.h>
#include <ti/syslink/FrameQ_ShMem.h>
#include <ti/syslink/FrameQBufMgr_ShMem.h>
#include <ti/syslink/utils/OsalSemaphore.h>

#if defined(__KERNEL__)
#include <OsalThread.h>
#endif

/* Ipc header */
#include <ti/ipc/Ipc.h>
#include <ti/syslink/IpcHost.h>
#include <ti/ipc/MultiProc.h>

#if !defined(__KERNEL__)
#include <ti/syslink/samples/hlos/slaveLoader/SlaveLoader.h>
#endif /* if !defined(__KERNEL__) */
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>
#include "FrameQApp.h"


/** ============================================================================
 *  Macros
 *  ============================================================================
 */
//#define TRACE_FRAME_WRITES


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
#if !defined(__KERNEL__)
extern String  FrameQApp_filePath [];
extern ProcMgr_BootMode  FrameQApp_bootMode;
extern UInt32            FrameQApp_resetVector[];
#else/* if !defined(__KERNEL__) */
ProcMgr_BootMode         FrameQApp_bootMode = ProcMgr_BootMode_Boot;
UInt32                   FrameQApp_resetVector[SysLinkSamples_MAXPROCESSORS] = {0};
#endif /* if !defined(__KERNEL__) */
extern UInt16  FrameQApp_numProcs;
extern UInt16  FrameQApp_procId   [];
extern Bool    FrameQApp_runAll;

OsalSemaphore_Handle  FrameQApp_semHandle0[SysLinkSamples_MAXPROCESSORS];
FrameQBufMgr_Handle   FrameQApp_bufMgrHandle[SysLinkSamples_MAXPROCESSORS] ={ NULL};
FrameQ_Handle         FrameQApp_frmQCrehandle[SysLinkSamples_MAXPROCESSORS]={ NULL};
FrameQ_Handle         FrameQApp_frmQWriterhandle[SysLinkSamples_MAXPROCESSORS]={ NULL};

/* Global variable to hold the FrameQBufMgr Id */
static UInt32  FrameQApp_bufMgrId[SysLinkSamples_MAXPROCESSORS] = {0};

/* To hold local processor id */
UInt16          FrameQApp_localProcId;
String          FrameQApp_aframeQName[SysLinkSamples_MAXPROCESSORS][32u];
String          FrameQApp_aframeQBufMgrName[SysLinkSamples_MAXPROCESSORS][32u];

FrameQBufMgr_ShMem_FrameBufParams
FrameQApp_frameBufferInfo[APP_NUM_FRAMEBUFS_IN_FRAME]= {{0},};


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/* Call back function for writer FrameQ.
 * Call back will be invoked by FrameQBufMgr when free buffers are available
 */
Void APP_writerFrameQCallback(FrameQBufMgr_Handle handle, Ptr arg, FrameQ_NotifyMsg msg)
{
    OsalSemaphore_Handle sem;
#if defined TRACE_FRAME_WRITES
    Osal_printf ( " *****APP_writerFrameQCallback*****\n");
#endif
    sem = (OsalSemaphore_Handle)*((UInt32*)arg);
    OsalSemaphore_post (sem) ;

}

Void APPNotify_callbackFxn (UInt16 procId, UInt16 lineId, UInt32 eventNo,
           UArg arg, UInt32 payload)
{
    OsalSemaphore_Handle  sem ;
    sem = (OsalSemaphore_Handle)*((UInt32*)arg) ;
    OsalSemaphore_post (sem)  ;
    Osal_printf("\n******APPNotify_callbackFxn *****\n");
}

void FrameQApp_setupInstInfo(UInt16 procId)
{

    UInt16  lProcId = MultiProc_self();
    memset  (FrameQApp_aframeQName[procId], 0, 32);
    /* FrameQ name to be opened will be
     * FrameQxy where x is the remote proc id and y is the local procid
     */
    sprintf ((Ptr)FrameQApp_aframeQName[procId], "%s%d%d", FRAMEQ_NAME, procId, lProcId);

    memset  (FrameQApp_aframeQBufMgrName[procId], 0, 32);
    /* FrameQ name to be opened will be
     * FrameQxy where x is the remote proc id and y is the local procid
     */
    sprintf ((Ptr)FrameQApp_aframeQBufMgrName[procId], "%s%d%d",
             FRAMEQBUFMGR_NAME, procId, lProcId);
}

/*!
 *  @brief  Function to execute the instance startup for FrameQApp sample
 *  application
 *
 */
Int32 FrameQApp_instStartup(UInt16 procId)
{
    Int32                   status          = 0;
    UInt32                  i               = 0;
    FrameQBufMgr_ShMem_Params instParams;
    FrameQ_ShMem_Params     frmQInstParams;
    UInt32                  headerBufChunkSize;
    UInt32                  frmBufChunkSize;
    UInt32                  lBaseHeaderSize;
    UInt32                  k;

    Osal_printf ("Entered FrameQApp_instStartup()\n");

    Osal_printf ( " Creating OsalSemaphore");
    /* Create Semaphore for FramQBufMgr notifications*/
    FrameQApp_semHandle0[procId]=  OsalSemaphore_create (OsalSemaphore_Type_Counting);
    if (FrameQApp_semHandle0[procId] == NULL) {
         Osal_printf ( "Error: Failed OsalSemaphore_create \n");
         status = -1;
         return (status);
    }

    /* Create  FrameQBufMgr instance */
   /* Get the default values for the FrameQBufMgr instance params */
    FrameQBufMgr_ShMem_Params_init(&instParams);

    /* Set the create paramters common to all the implementations */

    /* Assign the string pointer of the instance name */
    instParams.commonCreateParams.name = (Ptr)FrameQApp_aframeQBufMgrName[procId];
    /* Set the ShMem implementation  for this FrameQBufMgr instance */
    instParams.commonCreateParams.ctrlInterfaceType =
                                               FrameQBufMgr_INTERFACE_SHAREDMEM;

    /*Set the ShMem specific paramters */

    /* Configure it for free frame pools.Here configuring all the frame pools
     * with  the same attributes.
     */
    instParams.numFreeFramePools = APP_NUM_FREE_FRMPOOL;

    for (k = 0; k < APP_NUM_FRAMEBUFS_IN_FRAME; k++) {
        FrameQApp_frameBufferInfo[k].size = IPC_BUFFER_ALIGN(
                                               1024u,
                                      SharedRegion_getCacheLineSize(
                                                  SHAREDREG_1_ENTRYID));
    }
    for (i = 0; i < instParams.numFreeFramePools; i++) {
        /* How many frames in this pool */
        instParams.numFreeFramesInPool[i] = 10;
        /* How many extra header buffers in this pool
         * For dup purposes.
         */
        instParams.numFreeHeaderBufsInPool[i] = 5u;

        /* How many frame buffers  frame should contain.
         */
        instParams.numFrameBufsInFrame[i] = APP_NUM_FRAMEBUFS_IN_FRAME;

        /* Calculate the base frame header size for the frame containing
         * instParams.numFrameBufsInFrame[i].
         */
         lBaseHeaderSize =
              FrameQBufMgr_getBaseHeaderSize(instParams.numFrameBufsInFrame[i]);

         /* Align base header size to cache aligend */
        instParams.frameHeaderBufSize[i] = IPC_BUFFER_ALIGN(lBaseHeaderSize,
                                            SharedRegion_getCacheLineSize(
                                                      SHAREDREG_0_ENTRYID));
        /* What are the sizes of frame buffers */

        instParams.frameBufParams[i] = FrameQApp_frameBufferInfo;

    }
    /* Pass the shared region id  to allocate internal shared control structures
     * from
     */
    instParams.regionId = SHAREDREG_0_ENTRYID;
    /* Pass the shared region id  to allocate frame Headers from */
    instParams.hdrBuf_regionId = SHAREDREG_0_ENTRYID;
    /* Pass the shared region id  to allocate frame Frame buffers from */
    instParams.frmBuf_regionId = SHAREDREG_1_ENTRYID;

    instParams.localProtect    = GateMP_LocalProtect_INTERRUPT;
    instParams.remoteProtect   = GateMP_RemoteProtect_SYSTEM;

    instParams.headerInterfaceType  = FrameQBufMgr_HDRINTERFACE_SHAREDMEM;
    instParams.bufInterfaceType = FrameQBufMgr_BUFINTERFACE_SHAREDMEM ;

    /* Get the caller processor id */
    instParams.cpuAccessFlags   = (
                                FrameQBufMgr_FRAMEBUF0_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF1_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF2_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF3_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF4_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF5_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF6_CPUACCESS |
                                FrameQBufMgr_FRAMEBUF7_CPUACCESS);

    instParams.sharedAddrSize = FrameQBufMgr_ShMem_sharedMemReq (
                                                           &instParams,
                                                           &headerBufChunkSize,
                                                           &frmBufChunkSize);

    Osal_printf ("Required memory for FrameQ bufMgr is  \n"
                   " For Control structure:[0x%x]\n"
                   " For Frame Headers    :[0x%x]\n"
                   " For Frame Buffers    :[0x%x]\n",
                    instParams.sharedAddrSize,
                    headerBufChunkSize,
                    frmBufChunkSize);
    do {
        FrameQApp_bufMgrHandle[procId] =  FrameQBufMgr_create(&instParams);
        if (FrameQApp_bufMgrHandle[procId] == NULL) {
            Osal_printf ("FrameQBufMgr_create  failed for %s \n",
                           instParams.commonCreateParams.name);
            status = -1;
        }
    } while (FrameQApp_bufMgrHandle[procId] == NULL);

    Osal_printf ("Created FrameQBufMgr %s:FrameQApp_bufMgrHandle 0x%x\n",
                 instParams.commonCreateParams.name,
                 FrameQApp_bufMgrHandle[procId]);

    if (status >= 0) {

        /* Get id for the  FrameQApp_bufMgrHandle */
        FrameQApp_bufMgrId[procId] = FrameQBufMgr_getId (FrameQApp_bufMgrHandle[procId]);
    /**************** FrameQ instance creation **********************/
        /* Get the default values for the FrameQ instance params */
        FrameQ_ShMem_Params_init(&frmQInstParams);

        frmQInstParams.commonCreateParams.name = (Ptr)FrameQApp_aframeQName[procId];

        frmQInstParams.commonCreateParams.size = sizeof(FrameQ_ShMem_Params);
        frmQInstParams.commonCreateParams.ctrlInterfaceType =
                                    FrameQ_INTERFACE_SHAREDMEM;

        /* Set it for   number of readers specified through macro */
        frmQInstParams.numReaders = APP_NUM_FRMQREADERS;
        frmQInstParams.numQueues  = APP_NUM_FRMQUEUES_FOR_READER;
                                        /* Set it for queues for each reader*/
        frmQInstParams.cpuAccessFlags = (FrameQ_FRAMEBUF0_CPUACCESS |
                                     FrameQ_FRAMEBUF1_CPUACCESS |
                                     FrameQ_FRAMEBUF2_CPUACCESS |
                                     FrameQ_FRAMEBUF3_CPUACCESS |
                                     FrameQ_FRAMEBUF4_CPUACCESS |
                                     FrameQ_FRAMEBUF5_CPUACCESS |
                                     FrameQ_FRAMEBUF6_CPUACCESS |
                                     FrameQ_FRAMEBUF7_CPUACCESS);

        frmQInstParams.localProtect  = GateMP_LocalProtect_INTERRUPT;
        frmQInstParams.remoteProtect = GateMP_RemoteProtect_SYSTEM;
        /* Pass the name of the FrameQBufMgr  to be used in this FrameQ*/
        frmQInstParams.frameQBufMgrName = (Ptr)FrameQApp_aframeQBufMgrName[procId];

        Osal_printf ("frmQInstParams.frameQBufMgrName :[%s] \n",
                    frmQInstParams.frameQBufMgrName);

        frmQInstParams.sharedAddrSize =
                                    FrameQ_ShMem_sharedMemReq (&frmQInstParams);
        Osal_printf ("Required Memory for FrameQ :[0x%x] \n",
                    FrameQ_ShMem_sharedMemReq (&frmQInstParams));

        /* Pass the shared region id  to allocate internal shared control structures from */
        frmQInstParams.regionId = SHAREDREG_0_ENTRYID;

        FrameQApp_frmQCrehandle[procId] = FrameQ_create (&frmQInstParams);
        if (FrameQApp_frmQCrehandle[procId] == NULL) {
            Osal_printf ("ERROR:Failed to create FrameQ instance %s\n",
                                        frmQInstParams.commonCreateParams.name);
            return -1;
         }
         else {
            Osal_printf ("FrameQ instance %s created successfully\n",
                                       frmQInstParams.commonCreateParams.name);
         }

   }
    Osal_printf ("Leaving FrameQApp_instStartup().status [0x%x]\n", status);
    return (status);
}/* FrameQApp_instStartup */



/*!
 *  @brief  Function to perform execution phase of FrameQ sample Application.
 *
 */
Int32 FrameQApp_instExecute(UInt16 procId)
{
    Int32                   status        = 0;
    UInt32                  frmCnt        = 0;
    FrameQ_Frame            frame         = NULL;
    FrameQ_ShMem_OpenParams frmQOpenParams;
    FrameQ_NotifyParams     frmQNotifyParams;
    UInt32                  j;
    UInt8                   *frmBuf;
    UInt32                  i;

    Osal_printf ("Entered FrameQApp_instExecute \n");

    frmQOpenParams.commonOpenParams.name = (Ptr)FrameQApp_aframeQName[procId];

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
    Osal_printf ("Opening FrameQ  %s \n", frmQOpenParams.commonOpenParams.name);
    do {
        status = FrameQ_open (&frmQOpenParams,&FrameQApp_frmQWriterhandle[procId]);
        if (status < 0){
            Osal_printf ("ERROR:FrameQ_open  failed for writer."
                         "status: %d \n",
                         status);

        }
        else {
             Osal_printf ("FrameQ_open  success for writer."
                          "status: %d\n",
                           status);
        }
    } while ((FrameQApp_frmQWriterhandle == NULL) || (status < 0));
   /**************** Open FrameQ instance in write mode  **********************/
    Notify_registerEvent(procId,
                         0,
                         APPNOTIFY_EVENT_NO + 1,
                        (Notify_FnNotifyCbck)APPNotify_callbackFxn,
                        (UArg) &(FrameQApp_semHandle0[procId]));
    Osal_printf ("Sending sync notification to ProcId: %d\n", procId);
    Notify_sendEvent(procId,
                     0,
                     APPNOTIFY_EVENT_NO,
                     0,
                     TRUE);
    Osal_printf ("Sent sync notification to ProcId: %d\n", procId);
    OsalSemaphore_pend(FrameQApp_semHandle0[procId],
                       OSALSEMAPHORE_WAIT_FOREVER);
    Notify_unregisterEvent(procId,
                         0,
                         APPNOTIFY_EVENT_NO +1,
                        (Notify_FnNotifyCbck)APPNotify_callbackFxn,
                        (UArg) &(FrameQApp_semHandle0[procId]));

    /* Register call back fucntion   with the FrameQ for free frames */
    frmQNotifyParams.notifyType = FrameQ_NOTIFICATION_ONCE;
    frmQNotifyParams.cbFxnPtr   =
                                (FrameQ_NotifyFunc)APP_writerFrameQCallback;
    frmQNotifyParams.watermark = 1; /* Water mark one free frame */
    frmQNotifyParams.cbContext = &FrameQApp_semHandle0[procId]; /*Context pointer */
    frmQNotifyParams.cbClientHandle = FrameQApp_frmQWriterhandle[procId];
    status = FrameQ_registerNotifier (FrameQApp_frmQWriterhandle[procId],
                                      &frmQNotifyParams);

    if (status < 0) {
     Osal_printf ("ERROR: Failed to register call back with FrameQ writer"
                    "For free buffers.status: %d\n",
                    status);
    }
    else {
     Osal_printf ("Registered notify call back with FrameQ writer"
                    "For free buffers.status: %d\n",
                    status);

    }

//    #if defined(__KERNEL__)
//         OsalThread_sleep(100);
//    #else
//        sleep (2);
//    #endif

    /* Send force notification to reader client */
    do {
        status = FrameQ_sendNotify (FrameQApp_frmQWriterhandle[procId],
                                    APP_FRAMEQ_FORCE_NOTIFCATION_MSG);
        if (status < 0) {
//             Osal_printf ("ERROR: Failed to send force notification to reader"
//                            "status: 0x%x\n",
//                             status);

            #if defined(__KERNEL__)
                 OsalThread_sleep(2000);
            #else
                sleep (2);
            #endif
        }
        else {
            Osal_printf ("FrameQ Writer: Sent force notification to reader"
                           "status: %d\n",
                            status);
        }

    }
    while (status < 0);

   /* Allocate frame by writer */
    for (i = 0; i <  ITER_COUNT; i++) {
        /* Input frame pointer to  First Alloc call   should be set to NULL.
         * If alloc fails due  insufficient buffers. Successive alloc calls
         * should pass the same frame  pointer that is just returned from the
         * previous call.
         * This allows alloc to fully cook the frame with frame header and frame
         * buffer.
         */
        do {
            status = FrameQ_alloc (FrameQApp_frmQWriterhandle[procId], &frame);
            if (status < 0) {
#if defined TRACE_FRAME_WRITES
                Osal_printf ("FrameQWriter:FrameQ_alloc Failed."
                             "Waiting for Notification...\n");
#endif
                OsalSemaphore_pend(FrameQApp_semHandle0[procId],
                                   OSALSEMAPHORE_WAIT_FOREVER);
#if defined TRACE_FRAME_WRITES
                Osal_printf ("FrameQWriter:Got notification...\n");
#endif
            }
       } while (status < 0);


       if (frame != NULL){
             /* Process the retrieved frame here */
#if defined TRACE_FRAME_WRITES
            Osal_printf ("Frame Address          :0x%x\n",frame);
#endif
            frmCnt = FrameQ_getNumFrameBuffers (frame);
#if defined TRACE_FRAME_WRITES
                Osal_printf ("Number Of Frame Buffers:%d\n",frmCnt );
#endif

            for (j = 0; j < frmCnt; j++) {
                frmBuf = FrameQ_getFrameBuffer(frame, j);
#if defined TRACE_FRAME_WRITES
                Osal_printf ("FrameBuffer Address :0x%x\n",(UInt32)frmBuf);
                 /* Here populte the frame extended header and frame buffers */
                  Osal_printf ("FrameBuffer size :0x%x\n",
                               FrameQ_getFrameBufSize(frame, j) );
#endif
                  status = FrameQ_setFrameBufDataStartOffset (frame ,j, 0);
                  status = FrameQ_setFrameBufValidSize(frame , j, 4);
                  frmBuf[0] = 0xA;
                  frmBuf[1] = 0xB;
                  frmBuf[2] = 0xC;
                  frmBuf[3] = 0xD;
            }

            /* Free the frame  */
            if (frame != NULL) {
                 /* Put the frame in to FrameQ */
                 status = FrameQ_put (FrameQApp_frmQWriterhandle[procId], frame);
           }
        }
        if (((i % 100) == 0) && (i > 0)) {
           Osal_printf ("FrameQWriter:Transferred %d frames \n", i);
        }

    }
    if (i == ITER_COUNT) {
       Osal_printf ("FrameQWriter:Transferred %d frames \n", i);
    }
    Osal_printf ("Leave FrameQApp_instExecute.status [0x%x]\n",status);
    return 0;
}/* FrameQApp_execute */


Int FrameQApp_instShutdown(UInt16 procId)
{
    UInt32     frmCnt;
    Int32      status;

    Osal_printf ("Entered FrameQApp_instShutdown()\n");
    /* Wait for  Reader to consume all the frames */
    do {
        status = FrameQ_getNumFrames(FrameQApp_frmQWriterhandle[procId], &frmCnt);

        if (frmCnt > 0) {
            #if defined(__KERNEL__)
                OsalThread_sleep(1000);
            #else
                sleep (1);
            #endif
        }

    } while (frmCnt != 0);

  /* Register call back fucntion   with the FrameQbufMgr created */
    status = FrameQ_unregisterNotifier (FrameQApp_frmQWriterhandle[procId]);
    if (status < 0) {
        Osal_printf ("ERROR:  Failed to unregister call back"
                     "with FrameQBufMgr: status [%d]\n",
                     status);
    }
    else {
        Osal_printf("FrameQBufMgr_unregisterNotifier done \n");
    }

   /* Close the  FrameQ handles */
    status = FrameQ_close (&FrameQApp_frmQWriterhandle[procId]);
    if (status < 0) {
        Osal_printf ("ERROR:FrameQ_close failed. status :[%d]\n", status);
    }

     /* Delete FrameQ */
    status = FrameQ_delete (&FrameQApp_frmQCrehandle[procId]);
    if (status < 0) {
        Osal_printf ("ERROR:FrameQ_delete failed. status :[%d]\n", status);
    }

    /* Delete the FrameQ BufMgr instance */
    status = FrameQBufMgr_delete ((FrameQBufMgr_Handle *)&FrameQApp_bufMgrHandle[procId]);
    if (status < 0) {
        Osal_printf ("ERROR:FrameQBufMgr_delete failed. status :[%d]\n",
                     status);
    }

    /* Delete Semaphore for FramQBufMgr notifications*/
    OsalSemaphore_delete (&FrameQApp_semHandle0[procId]);

    Osal_printf ("Leave FrameQApp_instShutdown()\n");
    return (status);
}


Int FrameQApp_startup(String mapFile)
{
    Int32   status = 0;
    UInt16  i = 0;
    UInt32  index;
    Ptr arg;
    
    Osal_printf ("Entered FrameQApp_startup\n");

    /* Set the slaves to run the application with. */
    status = SysLinkSamples_setToRunProcIds(FrameQApp_numProcs,
            FrameQApp_procId, FrameQApp_runAll);

#if !defined(__KERNEL__)
    for (i = 0; i < FrameQApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(FrameQApp_procId[i])) {
            if (FrameQApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Loading and starting procId [%d] with [%s]\n",
                        FrameQApp_procId [i], FrameQApp_filePath [i]);
                status = SlaveLoader_startup(FrameQApp_procId[i],
                        "startup", FrameQApp_filePath [i], mapFile);
                Osal_printf("SlaveLoader_startup status [%d]\n", status);
            }
        }
    }
#endif

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (FrameQApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL;
            }
            else {
                arg = (Ptr)&FrameQApp_resetVector[i];
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
        for (index = 0; index < MultiProc_getNumProcessors(); index++) {
            if (SysLinkSamples_toRunProcId(index) &&
                    (index != MultiProc_self())) {
                FrameQApp_setupInstInfo (index);
            }
        }

        for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
            if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
                /* Execute the logic one by one for each processor */
                status = FrameQApp_instStartup(i);
                if (status < 0) {
                    Osal_printf ("FrameQApp_instStartup Failed for "
                         "procId %d \n", i);
                }
            }
        }
    }

    Osal_printf("Leaving FrameQApp_startup() (0x%x)\n", status);

    return (status);
}


Int FrameQApp_execute(UInt16 procId)
{
    Int32   status = FrameQ_S_SUCCESS;
    UInt16  i;

    Osal_printf("Entered FrameQApp_execute\n");

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
            /* Execute the logic one by one for each processor */
            status = FrameQApp_instExecute(i);
            if (status < 0) {
                Osal_printf ("FrameQApp_instExecute Failed for remote"
                     "procId %d\n", i);
            }
        }
    }

    Osal_printf ("Leaving FrameQApp_execute\n");

    return (status);
}


/*!
 *  @brief  Function to execute the startup for FrameQApp sample application
 *
 */
Int32 FrameQApp_shutdown(String mapFile)
{
    Int32 status = FrameQ_S_SUCCESS;
    Int16 i = 0;
    Ptr arg;

    Osal_printf ("Entered FrameQApp_shutdown\n");

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            /* Execute the logic one by one for each processor */
            status = FrameQApp_instShutdown(i);
            if (status < 0) {
                Osal_printf ("FrameQApp_instShutdown Failed for remote "
                             "procId %d \n", i);
            }
        }
    }

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (FrameQApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL; 
            }
            else {
                arg = (Ptr)&FrameQApp_resetVector[i];
            }
            /* Call Ipc_control with resetVector for 
             * ProcMgr_BootMode_NoLoad_Pwr,
             * ProcMgr_BootMode_NoLoad_NoPwr
             * and ProcMgr_BootMode_NoBoot modes
             */
            status = Ipc_control(i, 
                Ipc_CONTROLCMD_STOPCALLBACK, arg);
            Osal_printf ("After Ipc_stopCallback status: [0x%x]\n",
                         status);
        }
    }

#if !defined(__KERNEL__)
    for (i = 0 ; i < FrameQApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(FrameQApp_procId[i])) {
            if (FrameQApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Shutting down procId [%d]\n",
                        FrameQApp_procId[i]);
                status = SlaveLoader_shutdown(FrameQApp_procId[i],
                        "shutdown", mapFile);
                Osal_printf("SlaveLoader_shutdown status [%d]\n", status);
            }
        }
    }
#endif

    Osal_printf("Leaving FrameQApp_shutdown() (0x%x)\n", status);

    return (status);
}
