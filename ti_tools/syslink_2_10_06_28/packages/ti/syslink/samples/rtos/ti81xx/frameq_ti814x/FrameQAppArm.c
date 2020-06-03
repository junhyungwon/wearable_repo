/*
 *  @file   FrameQAppArm.c
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



#include <xdc/std.h>
#include <string.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/Memory.h>
/*-------------------------   BIOS headers   -------------------------------  */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/gates/GateMutex.h>
/*-------------------------   Module headers   -------------------------------  */
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/NameServer.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>
#include <ti/ipc/HeapBufMP.h>
#include <ti/ipc/MessageQ.h>

#include <ti/ipc/Ipc.h>
#include <ti/syslink/inc/knl/Platform.h>
#include <ti/syslink/SysLink.h>

/*-------------------------   Shim layer includes   ------------------------  */
#include <ti/syslink/FrameQ.h>
#include <ti/syslink/FrameQ_ShMem.h>
#include <ti/syslink/FrameQBufMgr_ShMem.h>
#include <ti/syslink/FrameQBufMgr.h>
#include <ti/syslink/samples/rtos/procMgr/ProcMgrApp.h>
/*-------------------------   OSAL and utils   -----------------------------  */
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/inc/knl/OsalKfile.h>
/*-------------------------   Application headers   ------------------------  */
#include "FrameQ_sample.h"
#include "FrameQAppArm.h"
/*  ------------------------To get globals from .cfg Header ******************/
#include <xdc/cfg/global.h>

#define IPC_BUFFER_ALIGN(x, y) (UInt32)((UInt32)((x + y - 1) / y) * y)


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
Char   tempStr[80];
IHeap_Handle        heapBufMPAppHandle = NULL;

FrameQApp_ProcInfo ProcInfo[]= {

    {0,FrameQApp_SLVFILEPATH1},
    {1,FrameQApp_SLVFILEPATH2},
    {2,FrameQApp_SLVFILEPATH3},
};

//#define TRACE_FRAME_READ
//#define TRACE_FRAME_WRITES

static WriterTaskInfo wrTaskInfo[APP_MAX_SLAVES];

/* Handle used for syncing between FrameQApp_execute and tsk_WriterFunc */
static Semaphore_Handle            taskSemHandle;


FrameQBufMgr_ShMem_FrameBufParams  frameBufferInfo[APP_NUM_FREE_FRMPOOL][APP_NUM_FRAMEBUFS_IN_FRAME]= {{0},};

/** ============================================================================
 *  Functions
 *  ============================================================================
 */

Int32 FrameQApp_startup (Void);
Int32 FrameQApp_shutdown (Void);
Int32 FrameQApp_execute (Void);


Void tsk_func(UArg arg0, UArg arg1);
Void tsk_WriterFunc(UArg arg0, UArg arg1);

/* Call back function for writer FrameQ.
 * Call back will be invoked by FrameQBufMgr when free buffers are available
 */
Void APP_writerFrameQCallback_m3v(FrameQBufMgr_Handle handle, Ptr arg, FrameQ_NotifyMsg msg)
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
    Error_Block                 eb;
    Semaphore_Params            semParams;
    Semaphore_Handle            localSemHandle;
    UInt16                      remoteProcId;


    Error_init(&eb);
    /* Create Counting Semaphore for App notifications*/
    Semaphore_Params_init (&semParams);
    semParams.mode = Semaphore_Mode_COUNTING;
    localSemHandle = Semaphore_create (0, &semParams, &eb);
    if (localSemHandle == NULL) {
        Error_check (&eb);
        Osal_printf ("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__);
        return(-1);
    }
    /*
     *  Need to define the shared region. The IPC modules use this
     *  to make portable pointers. All processors need to add this
     *  same call with their base address of the shared memory region.
     *  If the processor cannot access the memory, do not add it.
     */

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

    if (ipcWithDSP == 1) {
        remoteProcId = MultiProc_getId("DSP");
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
    taskSemHandle = NULL; /* initilize global variable*/

    Task_Params_init(&tskParams);
    tskParams.stackSize = 0x8000;
    tskHandle = Task_create (tsk_func,
                             &tskParams,
                             NULL);
    if (tskHandle == NULL) {
        Osal_printf ( "ERROR: Failed to crate task tsk0_func \n");
    }
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
 *  ======== tsk_func ========
 *  Calls the  FrameQ  setup API.and Spwans tasks to talk to M3Vido and DSS
 */
Void tsk_func(UArg arg0, UArg arg1)
{
    Int32                   status;

    Osal_printf("Entered tsk_func()\n");

    SysLink_setup();
    status = FrameQApp_startup ();
    if (status >= 0) {
        status = FrameQApp_execute ();
        if (status < 0) {
            Osal_printf("FrameQApp_execute failed!\n");
        }
    }
    else {
        Osal_printf("FrameQApp_startup failed!\n");
    }
    status = FrameQApp_shutdown ();
    if (status < 0) {
        Osal_printf("FrameQApp_shutdown failed!\n");
    }
    //SysLink_destroy ();
    Osal_printf("Leaving tsk_func()\n");

}

/*!
 *  @brief  Function to execute the startup for FrameQ sample application
 *
 *  @sa
 */
Int32 FrameQApp_startup(Void)
{
    Int32    status  = 0;
    UInt16   numProc = 0;
    Osal_printf("Entered   FrameQApp_startup\n");
    for (numProc = 0;numProc < FrameQApp_NUMSLAVES; numProc++)
    {
        Osal_printf("starting slave with procId = %d, file path = %s \n",
                      ProcInfo[numProc].procId,
                      ProcInfo[numProc].filePath );

        status = ProcMgrApp_startup(ProcInfo[numProc].procId,ProcInfo[numProc].filePath);
    }
    Osal_printf("Leaving FrameQApp_startup\n");
    return (status);
}

Int32 FrameQApp_shutdown(Void)
{
    Int32    status  = 0;
    UInt16   numProc = 0;

    for (numProc = 0;numProc < FrameQApp_NUMSLAVES; numProc++)
    {
        Osal_printf("Shutting down slave with procId = %d \n", 
                      ProcInfo[numProc].procId);

        status = ProcMgrApp_shutdown (ProcInfo[numProc].procId);
    }
    return (status);
}


Int32 FrameQApp_execute(Void)
{

    Task_Handle             tskWriterHandle;
    Task_Params             tskParams;
    Int32                   status;
    UInt16                  remoteProcId;
    Error_Block                 eb;
    Semaphore_Params            semParams;

    Osal_printf("Entered FrameQApp_execute()\n");

    status = APP_ipcInit();

    if (status < 0) {
       Osal_printf( "ERROR: APP_ipcInit Failed \n");
       return status;
    }

    Error_init(&eb);
    /* Create Counting Semaphore for task notifications*/
    Semaphore_Params_init (&semParams);
    semParams.mode = Semaphore_Mode_COUNTING;
    taskSemHandle = Semaphore_create (0, &semParams, &eb);
    if (taskSemHandle == NULL) {
        Error_check (&eb);
        Osal_printf("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__);
        return(-1);
    }

    /* Spawn the tasks */
    if  (ipcWithDSP == 1) {
        /* Create the task  to produce frames to DSP */
        remoteProcId = MultiProc_getId("DSP");
        Task_Params_init(&tskParams);
        tskParams.stackSize = 0x4000;
        strcpy(wrTaskInfo[remoteProcId].frmQBufMgrname,FRAMEQBUFMGR_INST1_NAME);
        strcpy(wrTaskInfo[remoteProcId].frmQName,FRAMEQ_INST1_NAME);
        /* let tasks create it internally */
        wrTaskInfo[remoteProcId].semHandle = NULL;
        tskParams.arg0 = (UArg)remoteProcId;
        tskParams.arg1 = (UArg)&wrTaskInfo;
        tskWriterHandle = Task_create (tsk_WriterFunc,
                                       &tskParams,
                                       NULL);
        if (tskWriterHandle == NULL) {
            Osal_printf ( "ERROR: Failed to create task tsk_WriterFunc \n");
        }
        Semaphore_pend(taskSemHandle, BIOS_WAIT_FOREVER);

    }

    /* Spawn the tasks */
    if  (ipcWithVIDEOM3 == 1) {
        /* Create the task  to produce frames to VIDEO */
        remoteProcId = MultiProc_getId("VIDEO-M3");
        Task_Params_init(&tskParams);
        tskParams.stackSize = 0x4000;
        strcpy(wrTaskInfo[remoteProcId].frmQBufMgrname,FRAMEQBUFMGR_INST2_NAME);
        strcpy(wrTaskInfo[remoteProcId].frmQName,FRAMEQ_INST2_NAME);
        /* let tasks create it internally */
        wrTaskInfo[remoteProcId].semHandle = NULL;
        tskParams.arg0 = (UArg)remoteProcId ;
        tskParams.arg1 = (UArg)&wrTaskInfo;
        tskWriterHandle = Task_create (tsk_WriterFunc,
                                       &tskParams,
                                       NULL);
        if (tskWriterHandle == NULL) {
            Osal_printf ( "ERROR: Failed to create task tsk_WriterFunc \n");
        }
        Semaphore_pend(taskSemHandle, BIOS_WAIT_FOREVER);
    }

    /* Spawn the tasks */
    if  (ipcWithVPSSM3 == 1) {
        /* Create the task  to produce frames to DSS */
        remoteProcId = MultiProc_getId("VPSS-M3");
        Task_Params_init(&tskParams);
        tskParams.stackSize = 0x4000;
        strcpy(wrTaskInfo[remoteProcId].frmQBufMgrname,FRAMEQBUFMGR_INST3_NAME);
        strcpy(wrTaskInfo[remoteProcId].frmQName,FRAMEQ_INST3_NAME);
        /* let tasks create it internally */
        wrTaskInfo[remoteProcId].semHandle = NULL;
        tskParams.arg0 = (UArg)remoteProcId;
        tskParams.arg1 = (UArg)&wrTaskInfo;
        tskWriterHandle = Task_create (tsk_WriterFunc,
                                       &tskParams,
                                       NULL);
        if (tskWriterHandle == NULL) {
            Osal_printf ( "ERROR: Failed to create task tsk_WriterFunc \n");
        }
        Semaphore_pend(taskSemHandle, BIOS_WAIT_FOREVER);
    }
    Osal_printf("Leave FrameQApp_execute()\n");
    return (status);
}


/*
 *  ======== tsk_WriterFunc ========
 */
Void tsk_WriterFunc(UArg arg0, UArg arg1)
{
    Int32                   status          = 0;
    UInt32                  i               = 0;
    UInt16                  remoteProcId    = (UInt16) arg0;
    WriterTaskInfo          *taskInfo       = &wrTaskInfo[remoteProcId];
    FrameQBufMgr_Handle     bufMgrHandle;
    FrameQBufMgr_ShMem_Params instParams;
    FrameQ_ShMem_Params     frmQInstParams;
    FrameQ_Handle           frmQCrehandle;
    FrameQ_Handle           frmQWriterhandle;
    Uint32                  bufMgrId;
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
//    FrameQ_Frame            dupFrame = NULL;
//    WriterTaskInfo          *taskInfo = arg0;


    Osal_printf ("Entered tsk_WriterFunc()\n");
    Error_init (&eb);
    /* Create Binary Semaphore for FramQ notifications*/
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

   /* Get the default values for the FrameQBufMgr instance params */
    FrameQBufMgr_ShMem_Params_init(&instParams);

    /* Set the create paramters common to all the implementations */

    /* Assign the string pointer of the instance name */
    instParams.commonCreateParams.size = sizeof(FrameQBufMgr_ShMem_Params);
    instParams.commonCreateParams.name = taskInfo->frmQBufMgrname;
    /* Set the ShMem implementation  for this FrameQBufMgr instance */
    instParams.commonCreateParams.ctrlInterfaceType =
                                               FrameQBufMgr_INTERFACE_SHAREDMEM;
    instParams.headerInterfaceType  = FrameQBufMgr_HDRINTERFACE_SHAREDMEM;
        instParams.bufInterfaceType = FrameQBufMgr_BUFINTERFACE_SHAREDMEM ;

    /*Set the ShMem specific paramters */

    /* Configure it for one free frame pool */
    instParams.numFreeFramePools = APP_NUM_FREE_FRMPOOL;

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
                                                      SHAREDREG_1_ENTRYID));
    }

    for (i = 0; i < instParams.numFreeFramePools; i++) {

        for (k = 0; k < instParams.numFrameBufsInFrame[i]; k++) {
            frameBufferInfo[i][k].size = IPC_BUFFER_ALIGN(
                                                   1024u,
                                          SharedRegion_getCacheLineSize(
                                                      SHAREDREG_1_ENTRYID));
        }
    }

    for (i = 0; i < instParams.numFreeFramePools; i++) {
            /* What are the sizes of frame buffers */
            instParams.frameBufParams[i] = frameBufferInfo[i];
    }
    /* Pass the shared region id  to allocate internal shared control structures.
     * Using shareregion that is configured  with cacheflag false.
     * because Between DSS and VIDEO  cache invalidate/writeback calls are not
     *required
     */
    instParams.regionId = SHAREDREG_1_ENTRYID;

    /* Pass the shared region id  to allocate frame Headers
     * Using shareregion that is configured  with cacheflag false.
     * because Between DSS and VIDEO  cache invalidate/writeback calls are not
     * required.
     */
    instParams.hdrBuf_regionId = SHAREDREG_1_ENTRYID;

    /* Pass the shared region id  to allocate frame Frame buffers
     * Using shareregion that is configured  with cacheflag false.
     * because Between DSS and VIDEO  cache invalidate/writeback calls are not
     * required.
     */
    instParams.frmBuf_regionId = SHAREDREG_1_ENTRYID;


    instParams.localProtect    = APP_INST_LOCALPROTECTION;
    instParams.remoteProtect   = GateMP_RemoteProtect_SYSTEM;


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
        bufMgrHandle =  FrameQBufMgr_create(&instParams);
        if (bufMgrHandle == NULL) {
            Osal_printf ("FrameQBufMgr_create  failed for %s \n",
                           instParams.commonCreateParams.name);
        }
    } while (bufMgrHandle == NULL);

    Osal_printf ("Created FrameQBufMgr %s:bufMgrHandle 0x%x\n",
                   taskInfo->frmQBufMgrname,
                   bufMgrHandle);
    /* Get id for the  bufMgrHandle */
    bufMgrId = FrameQBufMgr_getId (bufMgrHandle);
    Osal_printf ("Video writer bufMgrId 0x%x\n",bufMgrId);


    /**************** FrameQ instance creation **********************/
    /* Get the default values for the FrameQ instance params */
    FrameQ_ShMem_Params_init(&frmQInstParams);

    frmQInstParams.commonCreateParams.name = taskInfo->frmQName;
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
    /* Not setting flags for FrameQ */
    frmQInstParams.localProtect  = APP_INST_LOCALPROTECTION;
    frmQInstParams.remoteProtect = GateMP_RemoteProtect_SYSTEM;
    /* Pass the name of the FrameQBufMgr  to be used in this FrameQ*/
    frmQInstParams.frameQBufMgrName = taskInfo->frmQBufMgrname;

    frmQInstParams.sharedAddrSize = FrameQ_ShMem_sharedMemReq (&frmQInstParams);
    Osal_printf ("Required Memory for FrameQ :[0x%x] \n",
                    FrameQ_ShMem_sharedMemReq (&frmQInstParams));

    /* Pass the shared region id  to allocate internal shared control structures.
     * Using shareregion that is configured  with cacheflag false.
     * because Between DSS and VIDEO  cache invalidate/writeback calls are not
     * required.
     */
    frmQInstParams.regionId = SHAREDREG_1_ENTRYID;
    do {
        frmQCrehandle = FrameQ_create (&frmQInstParams);
        if (frmQCrehandle == NULL) {
            Osal_printf ("FrameQ_create  failed for %s \n",
                           frmQInstParams.commonCreateParams.name);
        }
    } while (frmQCrehandle == NULL);

    if (frmQCrehandle != NULL) {
        Osal_printf ("Created FrameQ  %s \n",taskInfo->frmQName);
    }
   /**************** Open FrameQ instance in write mode  **********************/
    /* Open using name */
    frmQOpenParams.commonOpenParams.name = taskInfo->frmQName;
    frmQOpenParams.commonOpenParams.openMode = FrameQ_MODE_WRITER;
    frmQOpenParams.commonOpenParams.cpuAccessFlags = (
                                            FrameQ_FRAMEBUF0_CPUACCESS |
                                            FrameQ_FRAMEBUF1_CPUACCESS |
                                            FrameQ_FRAMEBUF2_CPUACCESS |
                                            FrameQ_FRAMEBUF3_CPUACCESS |
                                            FrameQ_FRAMEBUF4_CPUACCESS |
                                            FrameQ_FRAMEBUF5_CPUACCESS |
                                            FrameQ_FRAMEBUF6_CPUACCESS |
                                            FrameQ_FRAMEBUF7_CPUACCESS);
    do {
        status = FrameQ_open (&frmQOpenParams,&frmQWriterhandle);
        if (status < 0){
            Osal_printf ("ERROR:FrameQ_open instance failed for writer."
                           "status: 0x%x \n",
                           status);

        }
        else {
             Osal_printf ("FrameQ_open  success for writer."
                            "status: 0x%x\n",
                            status);
        }

    } while ((frmQWriterhandle == NULL) || (status < 0));

    status = FrameQ_getNumFreeFrames(frmQWriterhandle, &i);
    if  (status >= 0) {
        Osal_printf ("FrameQ writer:NumBer of Free Fraemes available"
                       "to allocate %d\n", i);
    }
    /* Sending synchonization notification */
    status = Notify_sendEvent(remoteProcId,
                              0,
                              10,
                              0,
                              TRUE);
    if  (status >= 0) {
        Osal_printf ("FrameQ writer:Sent Synchronizatioon notification"
                   "from FrameQReader\n");
    }

    Error_print(&eb);
    Error_init(&eb);
  /* Register call back fucntion   with the FrameQ for free frames */
    frmQNotifyParams.notifyType = FrameQ_NOTIFICATION_ONCE;
    frmQNotifyParams.cbFxnPtr   =
                                (FrameQ_NotifyFunc)APP_writerFrameQCallback_m3v;
    frmQNotifyParams.watermark = 1; /* Water mark one free frame */
    frmQNotifyParams.cbContext = taskInfo->semHandle; /*Context pointer */
    frmQNotifyParams.cbClientHandle = frmQWriterhandle;

   /* Register call back fucntion   with the FrameQbufMgr created */
    status = FrameQ_registerNotifier (frmQWriterhandle, &frmQNotifyParams);
    if (status < 0) {
     Osal_printf ("ERROR: Failed to register call back with FrameQ writer"
                    "For free buffers.status: 0x%x\n",
                    status);
    }
    /* Send force notification to reader client */
    do {
        status = FrameQ_sendNotify (frmQWriterhandle,
                                    APP_FRAMEQ_FORCE_NOTIFCATION_MSG);
        if (status < 0) {
             Osal_printf ("ERROR: Failed to send force notification to reader"
                            "status: 0x%x\n",
                             status);
            /* Sleep for a while before trying again. */
            //Task_sleep (10);
        }
        else {
            Osal_printf ("FrameQ Writer: Sent force notification to reader"
                           "status: 0x%x\n",
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
            status = FrameQ_alloc (frmQWriterhandle,&frame);
            if (status < 0) {
#if defined TRACE_FRAME_WRITES
                Osal_printf ("FrameQWriter:FrameQ_alloc Failed."
                               "Waiting for Notification.After itercount %d.\n",
                               i);
#endif
                Semaphore_pend(taskInfo->semHandle, BIOS_WAIT_FOREVER);
#if defined TRACE_FRAME_WRITES
                Osal_printf ("FrameQWriter:Got notification...\n");
#endif
            }
       } while (status < 0);
#if defined TRACE_FRAME_WRITES
       Osal_printf ("FrameQWriter:FrameQ_alloc succesful. "
                      "status:0x%08x:IterCount %d\n",
                      status,
                      i);
#endif
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
                frmBuf[0] = 0x1A;
                frmBuf[1] = 0x1B;
                frmBuf[2] = 0x1C;
                frmBuf[3] = 0x1D;
            }
            /* Free the frame  */
            if (frame != NULL) {
#if 0
                /* Call dup to duplicate frame */
                status = FrameQ_dup (frmQWriterhandle, frame, &dupFrame);
                if (dupFrame != NULL) {
                     /* Put the frame in to FrameQ */
                    status = FrameQ_put (frmQWriterhandle, dupFrame);
                    i++;
                }
#endif
                 /* Put the frame in to FrameQ */
                status = FrameQ_put (frmQWriterhandle, frame);
           }
        }
        if (((i % 100) == 0) && (i > 0)) {
           Osal_printf ("FrameQWriter:Transferred %d frames \n", i);
        }

    }
    if (i == ITER_COUNT) {
       Osal_printf ("FrameQWriter:Transferred %d frames \n", i);
    }
    /* Wait for  Reader to consume all the frames */
    do {
       status = FrameQ_getNumFrames(frmQWriterhandle, &frmCnt);
    } while (frmCnt != 0);

   /* Close the  FrameQ handles */
    status = FrameQ_close (&frmQWriterhandle);
    Osal_printf ("Waiting for done notification from %s \n", MultiProc_getName(remoteProcId));
    /* Delete is force ful. So wait for the other side to finish up */
    Semaphore_pend( taskInfo->semHandle, BIOS_WAIT_FOREVER);
    Osal_printf("\n******APPNotify_callbackFxn *****\n");
     /* Delete FrameQ */
    FrameQ_delete (&frmQCrehandle);

    /* Delete the FrameQ BufMgr instance */
    FrameQBufMgr_delete (&bufMgrHandle);

    Semaphore_post((Semaphore_Object*)taskSemHandle);

    Osal_printf ("Leave tsk_WriterFunc()\n");
}



#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

