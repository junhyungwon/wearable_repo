/*
 *  @file   RingIOGppApp.c
 *
 *  @brief      Sample application for RingIO intra-GPP Application.
 *              Shows the API calling sequence
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



/* Standard headers */
#include <ti/syslink/Std.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/syslink/utils/String.h>
#include <ti/syslink/utils/OsalSemaphore.h>

#include <ti/syslink/ProcMgr.h>

#include <ti/ipc/MultiProc.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/syslink/RingIO.h>
#include <ti/syslink/RingIOShm.h>

/* Ipc header */
#include <ti/ipc/Ipc.h>
#include <ti/syslink/IpcHost.h>


#if defined(__KERNEL__)
#include "knl/Linux/RingIOGppAppOS.h"
#else
#include "usr/Linux/RingIOGppAppOS.h"
#endif

#if !defined(__KERNEL__)
#include <ti/syslink/samples/hlos/slaveLoader/SlaveLoader.h>
#endif /* if !defined(__KERNEL__) */
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>


/** ============================================================================
 *  Macros
 *  ============================================================================
 */
/* Data stamp for data integrity check */
#define XFER_VALUE              128u

#define RingIOGppApp_MULTIPROCESS
//#define RINGIOAPP_DEBUG


/** ============================================================================
 *  Forward declarations of internal functions
 *  ============================================================================
 */
/* Function to do RingIO operations for Writer Client */
Ptr
RingIOGppApp_WriterClient (Ptr ptr);

/* Function to do RingIO operations for Reader Client */
Ptr
RingIOGppApp_ReaderClient (Ptr ptr);

/* Function to prime buffer for data stamp for data integrity check */
Void
RingIO_InitBuffer (Ptr buffer, UInt32 size);

/* Function to verify buffer for data stamp for data integrity check */
Int
RingIO_VerifyData (Ptr    buffer,
                   UInt32 size);


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
UInt32                        RingIOGppApp_localId   = (UInt32) -1;
OsalSemaphore_Handle          RingIOGppApp_semHandle = NULL;
RingIOGppApp_ClientInfo       RingIOGppApp_writerClientInfo;
RingIOGppApp_ClientInfo       RingIOGppApp_readerClientInfo;

#if !defined(__KERNEL__)
extern String  RingIOGppApp_filePath [];
extern ProcMgr_BootMode  RingIOGppApp_bootMode;
extern UInt32            RingIOGppApp_resetVector[];
#else/* if !defined(__KERNEL__) */
ProcMgr_BootMode         RingIOGppApp_bootMode = ProcMgr_BootMode_Boot;
UInt32                   RingIOGppApp_resetVector[SysLinkSamples_MAXPROCESSORS] = {0};
#endif /* if !defined(__KERNEL__) */
extern UInt16  RingIOGppApp_numProcs;
extern UInt16  RingIOGppApp_procId   [];
extern Bool    RingIOGppApp_runAll;


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/* Call back function for writer RingIO.  */
Void APP_RingIOCallback(RingIO_Handle handle, Ptr arg, RingIO_NotifyMsg msg)
{
    OsalSemaphore_post ((OsalSemaphore_Handle) arg);
}


Ptr RingIOGppApp_ReaderClient(Ptr ptr)
{
    Int32                          status = 0;
    RingIO_BufPtr                  bufPtr       = NULL;
    RingIO_openParams              ringIOParams;
    RingIO_Handle                  readerHandle;
    UInt16                         type;
    UInt32                         param;
    UInt32                         attrSize;
    RingIO_BufPtr                  attrBufPtr [20];
    UInt32                         acqSize;

#if defined RINGIOAPP_DEBUG
    Osal_printf ("Entering RingIOGppApp_ReaderClient () \n") ;
#endif /* if defined RINGIOAPP_DEBUG */

    ringIOParams.openMode = RingIO_MODE_READER;
    ringIOParams.flags = RingIO_DATABUF_MAINTAINCACHE
                       | RingIO_NEED_EXACT_SIZE;

    do {
          status = RingIO_open ("RingIO",
                                &ringIOParams,
                                NULL,
                                &readerHandle);
#if defined(__KERNEL__)
            OsalThread_sleep (2);
#else /* if defined(__KERNEL__) */
            sleep (2);
#endif /* if defined(__KERNEL__) */
    } while (status < 0);

    Osal_printf ( "RingIO Reader Client open ... Success\n");


#if defined RINGIOAPP_DEBUG
    if (status  >= 0) {
        Osal_printf ( "Reader:RingIO Valid Size : %x\n", RingIO_getValidSize (readerHandle));
        Osal_printf ( "Reader:RingIO Empty Size : %x\n", RingIO_getEmptySize (readerHandle));
        Osal_printf ( "Reader:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (readerHandle));
        Osal_printf ( "Reader:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (readerHandle));
        Osal_printf ( "Reader:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(readerHandle));
        Osal_printf ( "Reader:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (readerHandle));
        Osal_printf ( "Reader:RingIO WaterMark: %x\n", RingIO_getWaterMark (readerHandle));
    }
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        /* Register call back fucntion   with the RingIObufMgr created */
        status = RingIO_registerNotifier (readerHandle,
                                          RingIO_NOTIFICATION_ONCE,
                                          1,
                                          (RingIO_NotifyFxn )APP_RingIOCallback,
                                          RingIOGppApp_semHandle);
#if defined RINGIOAPP_DEBUG
        if (status < 0) {
            Osal_printf ("Reader: Failed to register call back with RingIO status: [0x%x]\n",status);
        }
        else {
            Osal_printf ("Reader: Passed to register call back with RingIO status: [0x%x]\n",status);
        }
#endif /* if defined RINGIOAPP_DEBUG */
    }

    if (status >= 0) {
        do {
            acqSize = 100;
            status = RingIO_acquire (readerHandle, &bufPtr , &acqSize) ;
        } while (status < 0);


        if (status >= 0) {
#if defined RINGIOAPP_DEBUG
            Osal_printf  ("RingIO acquire for Reader succeeded:%x\n", status);
            Osal_printf  ("RingIO acquire for Reader succeeded:%x\n",acqSize );
#endif /* if defined RINGIOAPP_DEBUG */
            status = RingIO_VerifyData (bufPtr, acqSize);
        }
        else {
#if defined RINGIOAPP_DEBUG
            Osal_printf  ("RingIO acquire for Reader failed :%x\n", status);
            Osal_printf  ("RingIO acquire for Reader failed :%x\n",acqSize );
#endif /* if defined RINGIOAPP_DEBUG */
        }

    }

    if (status >= 0) {
        Osal_printf ( "Data integrity check ... Success\n");
    }
    else {
        Osal_printf ( "Data integrity check FAILED: %x\n", status);
    }

#if defined RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Reader:RingIO Valid Size : %x\n", RingIO_getValidSize (readerHandle));
        Osal_printf ( "Reader:RingIO Empty Size : %x\n", RingIO_getEmptySize (readerHandle));
        Osal_printf ( "Reader:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (readerHandle));
        Osal_printf ( "Reader:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (readerHandle));
        Osal_printf ( "Reader:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(readerHandle));
        Osal_printf ( "Reader:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (readerHandle));
        Osal_printf ( "Reader:RingIO WaterMark: %x\n", RingIO_getWaterMark (readerHandle));
    }
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (readerHandle, acqSize) ;

#if defined RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO release for Reader succeeded:%x\n", status);
            Osal_printf  ("RingIO release for Reader succeeded:%x\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO release for Reader failed :%x\n", status);
            Osal_printf  ("RingIO release for Reader failed:%x\n",acqSize );
        }
#endif /* if defined RINGIOAPP_DEBUG */
    }

    if (status >= 0) {
        Osal_printf ("Reader successfully consumed valid data Expected valid"
                     " data size:%x\t Actual valid data size: %x\n", 100, acqSize);
    }


#if defined RINGIOAPP_DEBUG
    if (status  >= 0) {
        Osal_printf  ("Reader has notified Writer\n");
    }

    if (status >= 0) {
        Osal_printf ( "Reader:RingIO Valid Size : %x\n", RingIO_getValidSize (readerHandle));
        Osal_printf ( "Reader:RingIO Empty Size : %x\n", RingIO_getEmptySize (readerHandle));
        Osal_printf ( "Reader:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (readerHandle));
        Osal_printf ( "Reader:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (readerHandle));
        Osal_printf ( "Reader:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(readerHandle));
        Osal_printf ( "Reader:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (readerHandle));
        Osal_printf ( "Reader:RingIO WaterMark: %x\n", RingIO_getWaterMark (readerHandle));
    }
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        /* Get data transfer attribute (Fixed attribute) */
        do {
            attrSize = sizeof (attrBufPtr);
            status = RingIO_getvAttribute(readerHandle, &type, &param,
                    attrBufPtr, &attrSize);
        } while (status < 0);
    }

#if defined RINGIOAPP_DEBUG
    if (status  >= 0) {
        Osal_printf  ("RingIO getAttribute for reader succeeded:%x\n", status);
    }
    else {
        Osal_printf  ("RingIO getAttribute for reader failed :%x\n", status);
    }
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        status = RingIO_VerifyData (attrBufPtr, attrSize);
        if (status >= 0) {
            Osal_printf ( "Attribute integrity check ... Success\n");
        }
    }

#if defined RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf  ("RingIO getAttribute for Reader succeeded:%x\n", status);
        Osal_printf ("RingIO type: %x\n", type);
        Osal_printf ("RingIO param: %x\n", param);
    }
    else {
        Osal_printf  ("RingIO getAttribute for Reader failed :%x\n", status);
    }

    if (status >= 0) {
        Osal_printf ( "Reader:RingIO Valid Size : %x\n", RingIO_getValidSize (readerHandle));
        Osal_printf ( "Reader:RingIO Empty Size : %x\n", RingIO_getEmptySize (readerHandle));
        Osal_printf ( "Reader:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (readerHandle));
        Osal_printf ( "Reader:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (readerHandle));
        Osal_printf ( "Reader:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(readerHandle));
        Osal_printf ( "Reader:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (readerHandle));
        Osal_printf ( "Reader:RingIO WaterMark: %x\n", RingIO_getWaterMark (readerHandle));
    }
#endif

    if (status >= 0) {
        do {
            status = RingIO_sendNotify (readerHandle, 0xABCD);
        }
        while (status < 0);
    }

    /* Unegister call back function   with the RingIObufMgr created */
    status = RingIO_unregisterNotifier (readerHandle);

#if defined RINGIOAPP_DEBUG
    if (status < 0) {
        Osal_printf ("ERROR:  Failed to unregister call back"
                     "with RingIO: [0x%x]\n",
                     status);
    }
    else {
        Osal_printf("RingIO_unregisterNotifier done \n");
    }
#endif /* if defined RINGIOAPP_DEBUG */

    status = RingIO_close (&readerHandle);

    Osal_printf ( "RingIO Reader Client closed\n");

#if defined RINGIOAPP_DEBUG
    Osal_printf ( "Reader:RingIO close Status: %x\n", status);

    Osal_printf ("Leaving RingIOGppApp_ReaderClient () \n") ;
#endif /* if defined RINGIOAPP_DEBUG */

    /* Exit  */
    RingIOGppApp_Exit_client(&RingIOGppApp_readerClientInfo) ;

    return (NULL) ;
}

Ptr
RingIOGppApp_WriterClient (Ptr ptr)
{
    Int32                          status = 0;
    RingIOShm_Params               ringIOShmParams;
    RingIO_openParams              openRingIOParams;
    RingIO_Handle                  ringIOHandle = NULL;
    RingIO_Handle                  writerHandle = NULL;
    RingIO_BufPtr                  bufPtr       = NULL;
    RingIO_BufPtr                  attrBufPtr [20];
    UInt32                         acqSize ;
    UInt16                         type;
    UInt32                         param;
    UInt32                         bytesFlushed;
    UInt32                         acquiredDataSize;

#if defined RINGIOAPP_DEBUG
    Osal_printf ("Entering RingIOGppApp_WriterClient () \n") ;
#endif /* if defined RINGIOAPP_DEBUG */

    /* Initialize Params */
    RingIOShm_Params_init (&ringIOShmParams);

#if defined RINGIOAPP_DEBUG
    Osal_printf ("RingIO_Params_init done successfully."
                 "status:[0x%x] \n",
                 status);

    Osal_printf ("Default values For RingIO Instance:\n"
                 "Gate for protection : %x "
                 "Shared region address for control structure :%x"
                 "Shared Region Address for data buffer :%x"
                 "Data Buffer size.  :%x"
                 "Foot Buffer size.  :%x"
                 "Shared Region Address for attr buffer :%x"
                 "Attr Buffer size.  :%x"
                 "Remote processor id with which the RingIO communicates.  :%x",
                 ringIOShmParams.gateHandle,
                 ringIOShmParams.commonParams.name,
                 ringIOShmParams.ctrlSharedAddr,
                 ringIOShmParams.dataSharedAddr,
                 ringIOShmParams.footBufSize,
                 ringIOShmParams.attrSharedAddr,
                 ringIOShmParams.attrSharedAddrSize,
                 ringIOShmParams.remoteProcId);
#endif /* if defined RINGIOAPP_DEBUG */

    ringIOShmParams.commonParams.name  = "RingIO";
    ringIOShmParams.ctrlRegionId       = 0x1;
    ringIOShmParams.dataRegionId       = 0x1;
    ringIOShmParams.attrRegionId       = 0x1;
    ringIOShmParams.dataSharedAddrSize = 0x1000;
    ringIOShmParams.footBufSize        = 0x0;
    ringIOShmParams.attrSharedAddrSize =  0x100;
    ringIOShmParams.remoteProcId       = MultiProc_self();
    ringIOHandle = RingIO_create (&ringIOShmParams);

#if defined RINGIOAPP_DEBUG
    if (ringIOHandle == NULL) {
        Osal_printf ( "Writer:RingIO Creation failed\n");
    }
    else {
        Osal_printf ( "Writer:RingIO Creation passed: %x\n", ringIOHandle);
    }
#endif /* if defined RINGIOAPP_DEBUG */
    if (ringIOHandle == NULL) {
        return (Ptr)-1;
    }
    else {
        Osal_printf ( "RingIO creation ... Success\n");
    }

#if defined RINGIOAPP_DEBUG
   if (status >= 0) {
       Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (ringIOHandle));
       Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (ringIOHandle));
       Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (ringIOHandle));
       Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (ringIOHandle));

   }
#endif /* if defined RINGIOAPP_DEBUG */

    /* Initialize Params */
    openRingIOParams.openMode = RingIO_MODE_WRITER;
    openRingIOParams.flags = RingIO_DATABUF_MAINTAINCACHE
                           | RingIO_NEED_EXACT_SIZE;

    status = RingIO_open ("RingIO",
                          &openRingIOParams,
                          NULL,
                          &writerHandle);
    if (status >= 0) {
        Osal_printf ( "RingIO Writer Client open ... Success\n");
    }
    else {
        Osal_printf ( "RingIO Writer Client open ... Failed :%x\n", status);
    }

    if (status >= 0) {
        /* Register call back fucntion   with the RingIObufMgr created */
        status = RingIO_registerNotifier (writerHandle,
                                          RingIO_NOTIFICATION_ONCE,
                                          1,
                                          (RingIO_NotifyFxn )APP_RingIOCallback,
                                          RingIOGppApp_semHandle);
#if defined RINGIOAPP_DEBUG
        if (status < 0) {
            Osal_printf ("Writer: Failed to register call back with RingIO status: [0x%x]\n",status);
        }
        else {
            Osal_printf ("Writer: Passed to register call back with RingIO status: [0x%x]\n",status);
        }
#endif /* if defined RINGIOAPP_DEBUG */
    }

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (writerHandle, &bufPtr , &acqSize) ;
#if defined RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO acquire for Writer failed :%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
        }
#endif /* if defined RINGIOAPP_DEBUG */
    }

    if (status >= 0) {
        RingIO_InitBuffer (bufPtr, acqSize) ;
    }

#if defined RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (writerHandle));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (writerHandle));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (writerHandle));
    }
#endif


    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (writerHandle, &bufPtr , &acqSize) ;
#if defined RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO acquire for Writer failed :%x\n", status);
            Osal_printf  ("RingIO acquire for Writer failed :%x\n",acqSize );
        }
#endif /* if defined RINGIOAPP_DEBUG */
    }

#if defined RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (writerHandle));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (writerHandle));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (writerHandle));
    }
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (writerHandle, acqSize) ;
#if defined RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO release for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO release for Writer succeeded:%x\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO release for Writer failed :%x\n", status);
            Osal_printf  ("RingIO release for Writer failed:%x\n",acqSize );
        }
#endif /* if defined RINGIOAPP_DEBUG */
    }

    if (status >= 0) {
        Osal_printf ( "Writer client produced valid data Expected valid data size:%x\tProduced valid data size:%x\n", 100, acqSize);
    }

#if defined RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (writerHandle));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (writerHandle));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (writerHandle));
    }
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        acquiredDataSize = RingIO_getAcquiredSize (writerHandle);
        status = RingIO_cancel (writerHandle) ;
#if defined RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO cancel for Writer succeeded:%x\n", status);
        }
        else {
            Osal_printf  ("RingIO cancel for Writer failed :%x\n", status);
        }
#endif /* if defined RINGIOAPP_DEBUG */
    }

    if (status >= 0) {
        Osal_printf ( "Writer client cancelled data Expected cancelled data size: %x\t Actual Cancelled data size\n", 100, acquiredDataSize);
    }

#if defined RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (writerHandle));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (writerHandle));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (writerHandle));
    }
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        /* Send data transfer attribute (Fixed attribute) */
        type = (UInt16) 1;
        RingIO_InitBuffer (attrBufPtr, sizeof (attrBufPtr)) ;
        status = RingIO_setvAttribute(writerHandle,
                                      type,
                                      0,
                                      attrBufPtr,
                                      sizeof (attrBufPtr),
                                      FALSE);
    }

#if defined RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf  ("RingIO setAttribute for Writer succeeded:%x\n", status);
    }
    else {
        Osal_printf  ("RingIO setAttribute for Writer failed :%x\n", status);
    }

    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (writerHandle));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (writerHandle));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (writerHandle));
    }
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (writerHandle, &bufPtr , &acqSize) ;
#if defined RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO acquire for Writer failed :%x\n", status);
            Osal_printf  ("RingIO acquire for Writer failed :%x\n",acqSize );
        }
#endif /* if defined RINGIOAPP_DEBUG */
    }

#if defined RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (writerHandle));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (writerHandle));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (writerHandle));
    }
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (writerHandle, acqSize) ;
#if defined RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO release for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO release for Writer succeeded:%x\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO release for Writer failed :%x\n", status);
            Osal_printf  ("RingIO release for Writer failed:%x\n",acqSize );
        }
#endif /* if defined RINGIOAPP_DEBUG */
    }

    Osal_printf ("Waiting for Reader to send notification to flush un needed data.\n");
    OsalSemaphore_pend(RingIOGppApp_semHandle, OSALSEMAPHORE_WAIT_FOREVER);

#if defined(__KERNEL__)
    OsalThread_sleep (2);
#else /* if defined(__KERNEL__) */
    sleep (2);
#endif /* if defined(__KERNEL__) */

    if (status >= 0) {
        status = RingIO_flush (writerHandle,
              TRUE,
              &type,
              &param,
              &bytesFlushed);
    }

#if defined RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf  ("RingIO flush for Writer succeeded:%x\n", status);
    }
    else {
        Osal_printf  ("RingIO flush for Writer failed :%x\n", status);
    }

    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (writerHandle));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(writerHandle));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (writerHandle));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (writerHandle));
        Osal_printf ( "Writer:RingIO type: %x\n", type);
        Osal_printf ( "Writer:RingIO param: %x\n", param);
        Osal_printf ( "Writer:RingIO bytesFlushed: %x\n", bytesFlushed);
    }
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        Osal_printf  ("Writer data flushed Success.Expected bytes flushed:%x\t Actual bytes flushed:%x\n", 100, bytesFlushed);
    }

    /* Unegister call back function   with the RingIObufMgr created */
    status = RingIO_unregisterNotifier (writerHandle);

#if defined RINGIOAPP_DEBUG
    if (status < 0) {
        Osal_printf ("ERROR:  Failed to unregister call back"
                     "with RingIO: [0x%x]\n",
                     status);
    }
    else {
        Osal_printf("RingIO_unregisterNotifier done \n");
    }
#endif /* if defined RINGIOAPP_DEBUG */

    status = RingIO_close (&writerHandle);
    Osal_printf ( "RingIO Writer Client closed\n");
#if defined RINGIOAPP_DEBUG
    Osal_printf ( "Writer:RingIO close Status: %x\n", status);
#endif /* if defined RINGIOAPP_DEBUG */

    status = RingIO_delete (&ringIOHandle);
#if defined RINGIOAPP_DEBUG
    Osal_printf ( "Writer:RingIO delete Status: %x\n", status);
#endif /* if defined RINGIOAPP_DEBUG */

    if (status >= 0) {
        Osal_printf  ("Writer RingIO deleted\n");
    }

    /* Exit  */
    RingIOGppApp_Exit_client(&RingIOGppApp_writerClientInfo) ;

#if defined RINGIOAPP_DEBUG
    Osal_printf ("Leaving RingIOGppApp_WriterClient () \n") ;
#endif /* if defined RINGIOAPP_DEBUG */

    return (NULL) ;
}


/*!
 *  @brief  Function to execute the startup for RingIOGppApp sample application
 *
 */
Int32 RingIOGppApp_startup(String mapFile)
{
    Int status = 0;
    UInt16 i = 0;
    Ptr arg;
    
    Osal_printf ("Entered RingIOGppApp_startup()\n");

    /* Set the slaves to run the application with. */
    status = SysLinkSamples_setToRunProcIds (RingIOGppApp_numProcs,
            RingIOGppApp_procId, RingIOGppApp_runAll);

#if !defined(__KERNEL__)
    for (i = 0; i < RingIOGppApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(RingIOGppApp_procId[i])) {
            if (RingIOGppApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Loading and starting procId [%d] with [%s]\n",
                        RingIOGppApp_procId[i], RingIOGppApp_filePath[i]);
                status = SlaveLoader_startup(RingIOGppApp_procId[i],
                        "startup", RingIOGppApp_filePath[i], mapFile);
                Osal_printf("SlaveLoader_startup status [%d]\n", status);
            }
        }
    }
#endif

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (RingIOGppApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL;
            }
            else {
                arg = (Ptr)&RingIOGppApp_resetVector[i];
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
        RingIOGppApp_semHandle = OsalSemaphore_create (OsalSemaphore_Type_Counting);
        if (RingIOGppApp_semHandle == NULL) {
             Osal_printf ( "Error: Failed OsalSemaphore_create \n");
             status = -1;
        }
    }

    Osal_printf ("Leaving RingIOGppApp_startup().status [0x%x]\n", status);

    return (status);
}


/*!
 *  @brief  Function to execute the RingIOGppApp sample application.
 *
 */
Int RingIOGppApp_execute(Void)
{
    Int32                   status        = 0;

    Osal_printf ("Entered RingIOGppApp_execute \n");

    if (status >= 0) {
        RingIOGppApp_writerClientInfo.processorId = RingIOGppApp_localId ;
        status = RingIOGppApp_Create_client(&RingIOGppApp_writerClientInfo,
                                         (Ptr)RingIOGppApp_WriterClient,
                                         NULL) ;
        if (status >= 0) {
            RingIOGppApp_readerClientInfo.processorId = RingIOGppApp_localId ;
            status = RingIOGppApp_Create_client(&RingIOGppApp_readerClientInfo,
                                            (Ptr)RingIOGppApp_ReaderClient,
                                            NULL) ;
            if (status < 0) {
#if defined RingIOGppApp_MULTIPROCESS
                Osal_printf ("ERROR! Failed to create Reader Client "
                             "Process in RingIOGppApp application\n") ;

#else /* if defined RingIOGppApp_MULTIPROCESS */
                Osal_printf ("ERROR! Failed to create Reader Task  "
                             "RingIOGppApp application\n") ;
#endif /* if defined RingIOGppApp_MULTIPROCESS */
            }

        }
        else {
#if defined RingIOGppApp_MULTIPROCESS
            Osal_printf ("ERROR! Failed to create Writer Client "
                         "Process in RingIOGppApp application\n") ;

#else /* if defined RingIOGppApp_MULTIPROCESS */
            Osal_printf ("ERROR! Failed to create Writer Task  "
                         "RingIOGppApp application\n") ;
#endif /* if defined RingIOGppApp_MULTIPROCESS */
        }
    }

    if (status >= 0) {
        /* Wait for the threads/process to  terminate*/
        RingIOGppApp_Join_client (&RingIOGppApp_writerClientInfo) ;
        RingIOGppApp_Join_client (&RingIOGppApp_readerClientInfo) ;
    }

    Osal_printf ("Leave RingIOGppApp_execute.status [0x%x]\n",status);
    return 0;
}/* RingIOGppApp_execute */


Int RingIOGppApp_shutdown(String mapFile)
{
    Int32      status = 0;
    UInt16 i = 0;
    Ptr arg;
    
    Osal_printf("Entered RingIOGppApp_shutdown()");

    OsalSemaphore_delete(&RingIOGppApp_semHandle);
    for (i = 0; i < MultiProc_getNumProcessors (); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (RingIOGppApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL; 
            }
            else {
                arg = (Ptr)&RingIOGppApp_resetVector[i];
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
    for (i = 0 ; i < RingIOGppApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(RingIOGppApp_procId[i])) {
            if (RingIOGppApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Shutting down procId [%d]\n",
                        RingIOGppApp_procId[i]);
                status = SlaveLoader_shutdown(RingIOGppApp_procId[i],
                        "shutdown", mapFile);
                Osal_printf("SlaveLoader_shutdown status [%d]\n", status);
            }
        }
    }
#endif

    Osal_printf("Leave RingIOGppApp_shutdown()\n");

    return (status);
}


Void RingIO_InitBuffer(Ptr buffer, UInt32 size)
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


Int RingIO_VerifyData(Ptr buffer, UInt32 size)
{
    Int        status = 0;
    UInt8  *    ptr8   = (UInt8  *)  (buffer) ;
    Int16      i ;

    /*
     *  Verify the data
     */
    for (i = 0; (status >= 0) && (i < size); i++) {

        if (*ptr8 != (Int8 ) (XFER_VALUE)) {
            Osal_printf ("ERROR! Data integrity check failed\n") ;
            Osal_printf   ("    Expected [0x%x]\n",
                        (XFER_VALUE)) ;
            Osal_printf   ("    Received [0x%x]\n", *ptr8) ;
            status = RingIO_E_FAIL;
        }

        ptr8++ ;
    }

    return (status) ;
}
