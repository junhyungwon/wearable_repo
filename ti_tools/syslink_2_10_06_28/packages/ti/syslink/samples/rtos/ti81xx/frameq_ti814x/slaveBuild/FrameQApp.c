/*
 *  @file   FrameQApp.c
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
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Assert.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>
#include <ti/ipc/Ipc.h>


#include <ti/syslink/SysLink.h>

/* Shim layer includes */
#include <ti/syslink/FrameQ.h>
#include <ti/syslink/FrameQ_ShMem.h>
#include <ti/syslink/FrameQBufMgr_ShMem.h>
#include <ti/syslink/FrameQBufMgr.h>

/*  ----------------------------------- To get globals from .cfg Header */
#include <xdc/cfg/global.h>


#include "../FrameQ_sample.h"


FrameQAPP_instInfo frameQApp_instInfo[] = {
    {
        /* Instance 0's FrameQ and FrameQbufMgr memory  distribution */
        FRAMEQBUFMGR_INST1_NAME,
        FRAMEQ_INST1_NAME,
    },
    {
        /* Instance 1's FrameQ and FrameQbufMgr memory  distribution */
        FRAMEQBUFMGR_INST2_NAME,
        FRAMEQ_INST2_NAME,
    },
    {
        /* Instance 2's FrameQ and FrameQbufMgr memory  distribution */
        FRAMEQBUFMGR_INST3_NAME,
        FRAMEQ_INST3_NAME,
    }
};


Void tsk1_func(UArg arg0, UArg arg1) ;

Semaphore_Handle   semHandle1 ;
Char   tempStr[80];

Int32 APP_ipcInit()
{
    Int32                   status        = 0 ;

    do {
        status = Ipc_attach(REMOTEPROCID);
    } while (status < 0);

    return (status) ;
}


int main()
{
    Int status = Ipc_S_SUCCESS;
    Task_Handle  tskHandle;
    Task_Params  tskParams;

    System_printf ( " APP_ipcInit status :%d\n",status) ;

    Task_Params_init(&tskParams) ;
    System_printf ( "Default stack size %d \n",tskParams.stackSize) ;
    tskParams.stackSize = 0x4000 ;

    /* Create task 1 function */
    tskHandle = Task_create (tsk1_func,
                            &tskParams,
                            NULL) ;
    if (tskHandle == NULL) {
       System_printf ( "ERROR: Failed to crate task tsk1_func \n") ;
    }

    do{
        status = Ipc_start();
        if (status < 0) {
            System_printf ( "ERROR: Ipc_start Failed status 0x%x \n",status);
        }
    } while (status != Ipc_S_SUCCESS);
    BIOS_start();

    return (0) ;
}

/* Call back function for writer FrameQ */
Void APP_readerFrameQCallback(FrameQ_Handle handle, Ptr arg, UInt32 msg)
{
    Semaphore_post((Semaphore_Object*)arg);
}

Void APPNotify_callbackFxn (UInt16 procId,UInt16 lineId, UInt32 eventNo,
           UArg arg, UInt32 payload)
{
    Semaphore_post((Semaphore_Object*)arg);
}

    Error_Block             eb;
    Semaphore_Params        semParams;
    FrameQ_ShMem_OpenParams frmQOpenParams;
    FrameQ_NotifyParams     frmQNotifyParams;

Void tsk1_func(UArg arg0, UArg arg1)
{
    Int32                   status        = 0;
    UInt32                  i             = 0;
    /* FrameQBufMgr instance params */
    FrameQ_Handle           frmQReaderHandle;
#if defined TRACE_FRAME_READ
    UInt8                  *frmbuf;
    UInt32                  frmCnt;
    UInt32                  j;
#endif
    FrameQ_Frame            outFrame;

    Error_init (&eb) ;
    
    SysLink_setup();
    
    status = APP_ipcInit() ;
    if (status < 0) {
        System_printf ( "ERROR: APP_ipcInit Failed \n") ;
        return;
    }


    /* Create Binary Semaphore for FramQ notifications*/
    Semaphore_Params_init (&semParams) ;
    semParams.mode = Semaphore_Mode_BINARY;
    semHandle1 = Semaphore_create (0, &semParams, &eb) ;
    if (semHandle1 == NULL) {
        Error_check (&eb) ;
        System_printf ("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__) ;
        return ;
    }

    Notify_registerEvent(
                     REMOTEPROCID,
                     0,
                     APPNOTIFY_EVENT_NO,
                     (Notify_FnNotifyCbck) APPNotify_callbackFxn,
                     (UArg) semHandle1);

        Semaphore_pend(semHandle1, BIOS_WAIT_FOREVER) ;

        System_printf ("Got App notification ....:\n") ;


   /**************** Open FrameQ instance in Read  mode  **********************/
    /* Allocate frmQInstParams.sharedAddrSize memory from shared region for this
     * instance
     */
    /* Get the memory address from shared region and pass it here */
    frmQOpenParams.commonOpenParams.name = frameQApp_instInfo[INSTREGION].frmQInstName;
    frmQOpenParams.commonOpenParams.openMode   = FrameQ_MODE_READER;
    /* Application access the framebuffers directly using CPU */
    frmQOpenParams.commonOpenParams.cpuAccessFlags = (
                                                  FrameQ_FRAMEBUF0_CPUACCESS |
                                                  FrameQ_FRAMEBUF1_CPUACCESS |
                                                  FrameQ_FRAMEBUF2_CPUACCESS |
                                                  FrameQ_FRAMEBUF3_CPUACCESS |
                                                  FrameQ_FRAMEBUF4_CPUACCESS |
                                                  FrameQ_FRAMEBUF5_CPUACCESS |
                                                  FrameQ_FRAMEBUF6_CPUACCESS |
                                                  FrameQ_FRAMEBUF7_CPUACCESS );

    do {
        status  = FrameQ_open (&frmQOpenParams, &frmQReaderHandle);
        if ((frmQReaderHandle == NULL)|| (status < 0)){
            System_printf ("ERROR:Failed to open FrameQ in Read mode."
                           "status: %d\n",status) ;
        }
    } while (frmQReaderHandle == NULL) ;

    if (frmQReaderHandle != NULL){
        System_printf ("Opened FrameQ in Read mode\n") ;
    }
    Notify_sendEvent(REMOTEPROCID,
                     0,
                     APPNOTIFY_EVENT_NO +1,
                     0,
                     TRUE);
    frmQNotifyParams.notifyType = FrameQ_NOTIFICATION_ONCE;
    frmQNotifyParams.cbFxnPtr   =
                                (FrameQ_NotifyFunc)APP_readerFrameQCallback;
    frmQNotifyParams.watermark = 1; /* Water mark one frame */
    frmQNotifyParams.cbContext = semHandle1; /*Context pointer */
    frmQNotifyParams.cbClientHandle = frmQReaderHandle;

    status = FrameQ_registerNotifier (frmQReaderHandle,
                                      &frmQNotifyParams);

   /* Retrieve frame from FrameQ */
    for (i = 0 ; i <  ITER_COUNT ; i++) {
        do {
            status = FrameQ_get (frmQReaderHandle, &outFrame );
            if (status < 0) {
#if defined TRACE_FRAME_READ
                System_printf ("FrameQReader:FrameQ_get Failed.Waiting for Notification...\n") ;
#endif
                Semaphore_pend(semHandle1, BIOS_WAIT_FOREVER) ;
#if defined TRACE_FRAME_READ
                System_printf ("FrameQReader:Got notification...\n") ;
#endif
            }

       } while ((status < 0)) ;
#if defined TRACE_FRAME_READ
       System_printf ("FrameQ Reader:FrameQ_get succesful status:%d:IterCount %d\n",
                       status,
                       i) ;
#endif
       if (outFrame != NULL){
             /* Process the retrieved frame here */
#if defined TRACE_FRAME_READ
            System_printf ("Frame Address          :0x%x\n",outFrame) ;

            frmCnt = FrameQ_getNumFrameBuffers (outFrame) ;
            System_printf ("Number Of Frame Buffers:%d\n",frmCnt );

            for (j = 0; j < frmCnt ; j++) {
                frmbuf = FrameQ_getFrameBuffer(outFrame, j);

                System_printf ("FrameBuffer Address :0x%x\n",frmbuf) ;
                System_printf ("FrameBuffer Byte  0 val:0x%x\n",frmbuf[0]) ;
                System_printf ("FrameBuffer Byte  1 val:0x%x\n",frmbuf[1]) ;
                System_printf ("FrameBuffer Byte  2 val:0x%x\n",frmbuf[2]) ;
                System_printf ("FrameBuffer Byte  3 val:0x%x\n",frmbuf[3]) ;
            }
#endif
          /* Free the frame  */
            status = FrameQ_free (frmQReaderHandle, outFrame );
        }
        if (((i % 100) == 0) && (i > 0)) {
           System_printf ("FrameQReader:Received %d frames \n", i);
        }
    }
    if (i == ITER_COUNT) {
        System_printf ("FrameQReader:Received %d frames \n", i);
    }
    status = FrameQ_unregisterNotifier (frmQReaderHandle);

    status = FrameQ_close (&frmQReaderHandle) ;
    if (status >= 0) {
        System_printf ("FrameQ_close successful. status :%d\n",status) ;
    }
    else {
        System_printf ("FrameQ_close failed.status :%d\n",status) ;
    }

    status = Notify_sendEvent(MultiProc_getId ("HOST"),
                              0,
                              APPNOTIFY_EVENT_NO,
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


