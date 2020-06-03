/*
 *  @file   RingIOApp.c
 *
 *  @brief      Sample application for RingIO on Netra platform.
 *              Demonstrate the RingIO between
 *              CortexA8 <--> C674P
 *              CortexA8 <--> M3Video
 *              CortexA8 <--> M3Dss
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

#if !defined(__KERNEL__)
#include <string.h>
#include <unistd.h>
#endif /* if defined(__KERNEL__) */

/* Ipc header */
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>
#include <ti/ipc/Ipc.h>
#include <ti/syslink/IpcHost.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/syslink/utils/String.h>

/* Module level headers */
#include <ti/syslink/ProcMgr.h>
#include <ti/syslink/SysLink.h>
#include <ti/syslink/RingIO.h>
#include <ti/syslink/RingIOShm.h>
#include <ti/syslink/utils/OsalSemaphore.h>

#if !defined(__KERNEL__)
#include <ti/syslink/samples/hlos/slaveLoader/SlaveLoader.h>
#endif /* if !defined(__KERNEL__) */
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>


//#define RINGIOAPP_DEBUG 1
/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/* Data stamp for data integrity check.  */
#define XFER_VALUE              128u

#define RIO_NAME                "rio"


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
#if !defined(__KERNEL__)
extern String  RingIOApp_filePath [];
extern ProcMgr_BootMode  RingIOApp_bootMode;
extern UInt32            RingIOApp_resetVector[];
#else/* if !defined(__KERNEL__) */
ProcMgr_BootMode         RingIOApp_bootMode = ProcMgr_BootMode_Boot;
UInt32                   RingIOApp_resetVector[SysLinkSamples_MAXPROCESSORS] = {0};
#endif /* if !defined(__KERNEL__) */
extern UInt16  RingIOApp_numProcs;
extern UInt16  RingIOApp_procId   [];
extern Bool    RingIOApp_runAll;

OsalSemaphore_Handle  RingIOApp_ringIOSemHandle[SysLinkSamples_MAXPROCESSORS];
GateMP_Handle         RingIOApp_gateHandle[SysLinkSamples_MAXPROCESSORS];
RingIO_Handle         RingIOApp_rioCreHandle[SysLinkSamples_MAXPROCESSORS];
RingIO_Handle         RingIOApp_rioWriterHandle[SysLinkSamples_MAXPROCESSORS];
String                RingIOApp_rioInstName[SysLinkSamples_MAXPROCESSORS][32u];
UInt16                RingIOApp_localProcId;


/** ============================================================================
 *  Forward declarations of functions
 *  ============================================================================
 */

/* Function to prime buffer for data integrity check.  */
Void RingIO_InitBuffer (Ptr buffer, UInt32 size);


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/* Call back function for writer RingIO.  */
Void APP_writerRingIOCallback(RingIO_Handle handle, Ptr arg, RingIO_NotifyMsg msg)
{
#ifdef RINGIOAPP_DEBUG
    Osal_printf ( " *****APP_writerRingIOCallback*****\n");
#endif
    OsalSemaphore_post ((OsalSemaphore_Handle)arg);

}


Void RingIOApp_setupInstName(UInt16 procId)
{
    UInt16  lProcId = MultiProc_self();
    memset  (RingIOApp_rioInstName[procId], 0, 32);
    /* RingIO name to be opened will be
     * RingIOxy where x is the remote proc id and y is the local procid
     */
    sprintf ((Ptr)RingIOApp_rioInstName[procId], "%s%d%d", RIO_NAME, procId, lProcId);
}


/*!
 *  @brief  Function to execute the instance startup for RingIOApp sample
 *  application
 *
 */
Int32 RingIOApp_instStartup(UInt16 procId)
{
    Int32                         status           = 0;
    RingIOShm_Params              rioShmParams;
    RingIO_openParams             openRingIOInstParams;
    GateMP_Params                 gateParams;

    Osal_printf ("Entered RingIOApp_instStartup()\n");

#ifdef RINGIOAPP_DEBUG

    Osal_printf ("Creating OsalSemaphore");
#endif

    if (status >= 0) {
        Osal_printf ("Creating OsalSemaphore");
        /* Create Semaphore for notifications*/
        RingIOApp_ringIOSemHandle [procId] =  OsalSemaphore_create (OsalSemaphore_Type_Counting);
#ifdef RINGIOAPP_DEBUG
        if (RingIOApp_ringIOSemHandle [procId] == NULL) {
            Osal_printf ( "Error: Failed OsalSemaphore_create \n");
            status = -1;
            return (status);
        }
        else {
            Osal_printf ( "OsalSemaphore_create succeeded \n");
        }
#endif
    }

    if (status >= 0) {
        GateMP_Params_init(&gateParams);
        gateParams.localProtect = GateMP_LocalProtect_PROCESS;
        
        /**************** RingIO instance creation **********************/
        RingIOShm_Params_init (&rioShmParams);
        rioShmParams.commonParams.name  =  (String)RingIOApp_rioInstName[procId];
        rioShmParams.ctrlRegionId       = 0x1;
        rioShmParams.dataRegionId       = 0x1;
        rioShmParams.attrRegionId       = 0x1;
        rioShmParams.attrSharedAddrSize = 0x100;
        rioShmParams.dataSharedAddrSize = 0x1000;
        rioShmParams.remoteProcId       = procId;
        rioShmParams.gateHandle = RingIOApp_gateHandle[procId] = GateMP_create(&gateParams);

        if (rioShmParams.gateHandle == NULL) {
            Osal_printf ("ERROR:Failed to create instance of GateMP\n");
            status = -1;
            return (status);
        }
        else {
            Osal_printf ("Create instance of GateMP... Success\n");
        }
        
        RingIOApp_rioCreHandle [procId] = RingIO_create ((Ptr)&rioShmParams);

        if (RingIOApp_rioCreHandle[procId] == NULL) {
            Osal_printf ("ERROR:Failed to create instance of RingIO\n");
            status = -1;
            return (status);
        }
        else {
            Osal_printf ("RingIO instance created successfully\n");
        }
    }
    if (status  >= 0) {
        Osal_printf ("RingIO Client creation .... Success\n");
        }

#ifdef RINGIOAPP_DEBUG
    if (status  >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (RingIOApp_rioCreHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (RingIOApp_rioCreHandle [procId]));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (RingIOApp_rioCreHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (RingIOApp_rioCreHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(RingIOApp_rioCreHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (RingIOApp_rioCreHandle [procId]));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (RingIOApp_rioCreHandle [procId]));
    }
#endif

    if (status >= 0) {
        /**************** RingIO instance open**********************/
        openRingIOInstParams.openMode   = RingIO_MODE_WRITER;
        openRingIOInstParams.flags      = RingIO_DATABUF_MAINTAINCACHE
                                        | RingIO_NEED_EXACT_SIZE;
        do {
            status = RingIO_open (
                                  (Ptr)RingIOApp_rioInstName[procId],
                                  &openRingIOInstParams,
                                  NULL,
                                  &RingIOApp_rioWriterHandle [procId]);
#ifdef RINGIOAPP_DEBUG
            if (status < 0) {
                Osal_printf ("ERROR:Failed to open instance of RingIO\n");
            }
            else {
                Osal_printf ("RingIO instance opened successfully\n");
            }
#endif
        } while (status < 0);
    }

    if (status  >= 0) {
        Osal_printf ("RingIO Writer Client Open ... Success\n");
    }

#ifdef RINGIOAPP_DEBUG
    if (status  >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (RingIOApp_rioWriterHandle [procId]));
    }
#endif
    if (status >= 0) {
        /* Register call back function with the RingIO created */
        status = RingIO_registerNotifier (RingIOApp_rioWriterHandle [procId],
                                          RingIO_NOTIFICATION_ONCE,
                                          1,
                                          (RingIO_NotifyFxn)APP_writerRingIOCallback,
                                          RingIOApp_ringIOSemHandle [procId]);
#ifdef RINGIOAPP_DEBUG
        if (status < 0) {
            Osal_printf ("Writer: Failed to register call back with RingIO status: [0x%x]\n",status);
        }
        else {
            Osal_printf ("Writer: Passed to register call back with RingIO status: [0x%x]\n",status);
        }
#endif
    }

    Osal_printf ("Leaving RingIOApp_instStartup().status [0x%x]\n", status);
    return (status);
}



/*!
 *  @brief  Function to execute the RingIOApp sample application.
 *
 */
Int RingIOApp_instExecute(UInt16 procId)
{
    Int32                          status       = 0;
    Int32                          tmpStatus    = 0;
    RingIO_BufPtr                  bufPtr       = NULL;
    UInt16                         type;
    UInt32                         param;
    UInt32                         acqSize;
    UInt32                         bytesFlushed;
    UInt32                         acquiredDataSize;

    Osal_printf ("Entered RingIOApp_instExecute for proc id %d \n", procId);

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (RingIOApp_rioWriterHandle [procId], &bufPtr , &acqSize);

#ifdef RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", acqSize );
        }
        else {
            Osal_printf  ("RingIO acquire for Writer failed :%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", acqSize );
        }
#endif
    }

    if (status >= 0) {
        RingIO_InitBuffer (bufPtr, acqSize) ;
    }

#ifdef RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (RingIOApp_rioWriterHandle [procId]));
    }
#endif

    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (RingIOApp_rioWriterHandle [procId], &bufPtr , &acqSize);
#ifdef RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO acquire for Writer failed :%x\n", status);
            Osal_printf  ("RingIO acquire for Writer failed :%x\n",acqSize );
        }
#endif
    }

#ifdef RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (RingIOApp_rioWriterHandle [procId]));
    }
#endif


    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (RingIOApp_rioWriterHandle [procId], acqSize);
#ifdef RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO release for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO release for Writer succeeded:%x\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO release for Writer failed :%x\n", status);
            Osal_printf  ("RingIO release for Writer failed:%x\n",acqSize );
        }
#endif
    }

    if (status >= 0) {
        Osal_printf  ("Writer has produced valid data. Expected valid data size:%x\tActual valid data size: %x\n",100, acqSize);
    }

#ifdef RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (RingIOApp_rioWriterHandle [procId]));
    }
#endif

    if (status >= 0) {
        acquiredDataSize = RingIO_getAcquiredSize (RingIOApp_rioWriterHandle [procId]);
        status = RingIO_cancel (RingIOApp_rioWriterHandle [procId]);
#ifdef RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO cancel for Writer succeeded:%x\n", status);
        }
        else {
            Osal_printf  ("RingIO cancel for Writer failed :%x\n", status);
        }
#endif
    }
    if (status >= 0) {
        Osal_printf  ("Writer has cancelled not needed data. Expected cancelled data size: %x\t Actual cancelled data size:%x\n", 100, acquiredDataSize);
    }

#ifdef RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (RingIOApp_rioWriterHandle [procId]));
    }
#endif

    if (status >= 0) {
        /* Send data transfer attribute (Fixed attribute) */
        type = (UInt16) 1;
        status = RingIO_setAttribute(RingIOApp_rioWriterHandle [procId], type, 0, FALSE);
    }

#ifdef RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf  ("RingIO setAttribute for Writer succeeded:%x\n", status);
    }
    else {
        Osal_printf  ("RingIO setAttribute for Writer failed :%x\n", status);
    }

    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (RingIOApp_rioWriterHandle [procId]));
    }
#endif

    if (status >= 0) {
        Osal_printf  ("Writer has set attribute for Reader to consume\n");
    }


    if (status >= 0) {
        acqSize = 100;
        status = RingIO_acquire (RingIOApp_rioWriterHandle [procId], &bufPtr , &acqSize);
#ifdef RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO acquire for Writer succeeded:%x\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO acquire for Writer failed :%x\n", status);
            Osal_printf  ("RingIO acquire for Writer failed :%x\n",acqSize );
        }
#endif
    }

#ifdef RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (RingIOApp_rioWriterHandle [procId]));
    }
#endif


    if (status >= 0) {
        acqSize = 100;
        status = RingIO_release (RingIOApp_rioWriterHandle [procId], acqSize);
#ifdef RINGIOAPP_DEBUG
        if (status >= 0) {
            Osal_printf  ("RingIO release for Writer succeeded:%x\n", status);
            Osal_printf  ("RingIO release for Writer succeeded:%x\n",acqSize );
        }
        else {
            Osal_printf  ("RingIO release for Writer failed :%x\n", status);
            Osal_printf  ("RingIO release for Writer failed:%x\n",acqSize );
        }
#endif
    }

    Osal_printf ("Waiting for reader to tell it to proceed\n");
    OsalSemaphore_pend(RingIOApp_ringIOSemHandle [procId], OSALSEMAPHORE_WAIT_FOREVER);
    Osal_printf ("Reader has given go ahead to flush the remaining data in RingIO\n");

    if (status >= 0) {
        status = RingIO_flush (RingIOApp_rioWriterHandle [procId],
                               TRUE,
                               &type,
                               &param,
                               &bytesFlushed);
    }

    if (status >= 0) {
       tmpStatus = RingIO_sendNotify (RingIOApp_rioWriterHandle [procId], 0x1234);
    }

#ifdef RINGIOAPP_DEBUG
    if (status >= 0) {
        Osal_printf  ("RingIO flush for Writer succeeded:%x\n", status);
    }
    else {
        Osal_printf  ("RingIO flush for Writer failed :%x\n", status);
    }

    if (status >= 0) {
        Osal_printf ( "Writer:RingIO Valid Size : %x\n", RingIO_getValidSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Size : %x\n", RingIO_getEmptySize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Valid Attr Size : %x\n", RingIO_getValidAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Empty Attr Size : %x\n", RingIO_getEmptyAttrSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Offset: %x\n", RingIO_getAcquiredOffset(RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO Acquired Size : %x\n", RingIO_getAcquiredSize (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO WaterMark: %x\n", RingIO_getWaterMark (RingIOApp_rioWriterHandle [procId]));
        Osal_printf ( "Writer:RingIO type: %x\n", type);
        Osal_printf ( "Writer:RingIO param: %x\n", param);
        Osal_printf ( "Writer:RingIO bytesFlushed: %x\n", bytesFlushed);
    }
#endif
    if (status >= 0) {
        Osal_printf  ("Writer has flushed data. Expected bytes to be flushed:%x\tActual bytes flushed %x\n",100,  bytesFlushed);
    }

    Osal_printf ("Leave RingIOApp_instExecute.status [0x%x]\n",status);
    return 0;
}/* RingIOApp_execute */


Int RingIOApp_instShutdown(UInt16 procId)
{
    Int32      status = 0;

    Osal_printf ("Entered RingIOApp_instShutdown() for procId\n", procId);

    /* Register call back function   with the RingIObufMgr created */
    status = RingIO_unregisterNotifier (RingIOApp_rioWriterHandle [procId]);

#ifdef RINGIOAPP_DEBUG
    if (status < 0) {
        Osal_printf ("ERROR:  Failed to unregister call back"
                     "with RingIOShm: [0x%x]\n",
                     status);
    }
    else {
        Osal_printf("RingIO_unregisterNotifier done \n");
    }
#endif

    /* Close the  RingIO handles */
    status = RingIO_close (&RingIOApp_rioWriterHandle[procId]);
    if (status < 0) {
        Osal_printf ("ERROR:RingIO_close failed. status :[0x%x]\n", status);
    }
    else {
        Osal_printf ("Writer Client close ... Success\n");
    }
#if !defined (__KERNEL__)
    usleep(1);
#endif
    /* Delete RingIO */
    status = RingIO_delete (&RingIOApp_rioCreHandle[procId]);
    if (status < 0) {
        Osal_printf ("ERROR:RingIO_delete failed. status :[0x%x]\n", status);
    }
    else {
        Osal_printf ("Writer Client delete ... Success\n");
    }

    status = GateMP_delete(&RingIOApp_gateHandle[procId]);
    if (status < 0) {
        Osal_printf ("ERROR:GateMP_delete failed. status :[0x%x]\n", status);
    }
    else {
        Osal_printf ("GateMP delete ... Success\n");
    }
        
    OsalSemaphore_delete  (&RingIOApp_ringIOSemHandle [procId]);

    Osal_printf ("Leave RingIOApp_instShutdown()\n");
    return (status);
}


Int RingIOApp_startup(String mapFile)
{
    Int32  status = 0;
    UInt16 i = 0;
    Ptr arg;

    Osal_printf ("Entered RingIOApp_startup\n");

    /* Set the slaves to run the application with. */
    status = SysLinkSamples_setToRunProcIds (RingIOApp_numProcs,
            RingIOApp_procId, RingIOApp_runAll);

#if !defined(__KERNEL__)
    for (i = 0; i < RingIOApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(RingIOApp_procId[i])) {
            if (RingIOApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Loading and starting procId [%d] with [%s]\n",
                        RingIOApp_procId[i], RingIOApp_filePath[i]);
                status = SlaveLoader_startup(RingIOApp_procId[i],
                        "startup", RingIOApp_filePath[i], mapFile);
                Osal_printf("SlaveLoader_startup status [%d]\n", status);
            }
        }
    }
#endif

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (RingIOApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL;
            }
            else {
                arg = (Ptr)&RingIOApp_resetVector[i];
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
                status = Ipc_control(i, 
                    Ipc_CONTROLCMD_STARTCALLBACK, NULL);
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
        RingIOApp_localProcId = MultiProc_self();
        for (i = 0; (i < MultiProc_getNumProcessors ()) && (status >= 0); i++) {
            if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
                RingIOApp_setupInstName (i);
            }
        }

        for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
            if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
                /* Execute the logic one by one for each processor */
                status = RingIOApp_instStartup(i);
                if (status < 0) {
                    Osal_printf ("RingIOApp_instStartup Failed for "
                         "procId %d \n", i);
                }
            }
        }
    }

    Osal_printf("Leaving RingIOApp_startup() (0x%x)\n", status);

    return (status);
}


Int RingIOApp_execute(Void)
{
    Int32 status = 0;
    UInt16  i;

    Osal_printf("Entered RingIOApp_execute\n");

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            /* Execute the logic one by one for each processor */
            status = RingIOApp_instExecute(i);
            if (status < 0) {
                Osal_printf ("RingIOApp_instExecute Failed for remote"
                     "procId %d\n", i);
            }
        }
    }

    Osal_printf ("Leaving RingIOApp_execute\n");

    return (status);
}


/*!
 *  @brief  Function to execute the startup for RingIOApp sample application
 *
 */
Int32 RingIOApp_shutdown(String mapFile)
{
    Int32 status = 0;
    Int16 i = 0;
    Ptr arg;

    Osal_printf ("Entered RingIOApp_shutdown\n");

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            /* Execute the logic one by one for each processor */
            status = RingIOApp_instShutdown(i);
            if (status < 0) {
                Osal_printf ("RingIOApp_instShutdown Failed for remote "
                             "procId %d \n", i);
            }
        }
    }

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (RingIOApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL; 
            }
            else {
                arg = (Ptr)&RingIOApp_resetVector[i];
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
    for (i = 0 ; i < RingIOApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(RingIOApp_procId[i])) {
            if (RingIOApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Shutting down procId [%d]\n",
                        RingIOApp_procId[i]);
                status = SlaveLoader_shutdown(RingIOApp_procId[i],
                        "shutdown", mapFile);
                Osal_printf("SlaveLoader_shutdown status [%d]\n", status);
            }
        }
    }
#endif

    Osal_printf("Leaving RingIOApp_shutdown\n");

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
