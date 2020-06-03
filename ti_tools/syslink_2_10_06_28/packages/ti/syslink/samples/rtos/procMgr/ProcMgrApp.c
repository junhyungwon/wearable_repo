/*
 *  @file   ProcMgrApp.c
 *
 *  @brief      Sample application for ProcMgr module
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



/*-------------------------   Standard headers   -----------------------------*/
#include <stdio.h>
/*-------------------------   XDC headers   ----------------------------------*/
#include <xdc/std.h>
#include <xdc/runtime/System.h>
/*-------------------------   OSAL and utils   -------------------------------*/
#include <ti/syslink/utils/Trace.h>
/*-------------------------   Module headers   -------------------------------*/
#include <ti/syslink/ProcMgr.h>
#include <ti/ipc/MultiProc.h>

#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

/** ============================================================================
 *  Globals
 *  ============================================================================
 */
#define NUM_ARGS 1

/** ============================================================================
 *  Functions
 *  ============================================================================
 */

/*!
 *  @brief  Function to execute the startup for ProcMgrApp sample application
 */
Int
ProcMgrApp_startup (UInt16 procId, String filePath)
{
    Int                          status = 0;
    ProcMgr_Handle               handle = NULL;
    ProcMgr_AttachParams         attachParams;
    ProcMgr_StartParams          startParams;
    String                       args [NUM_ARGS];
    UInt32                       fileId;

    printf ("Entered ProcMgrApp_startup\n");

    status = ProcMgr_open (&handle, procId);

    if (status >= 0) {
        ProcMgr_getAttachParams (NULL, &attachParams);
        /* Default params will be used if NULL is passed. */

        status = ProcMgr_attach (handle, &attachParams);
        if (status < 0) {
            printf ("ProcMgr_attach failed [0x%x]\n", status);
        }
        else {
            printf ("ProcMgr_attach status: [0x%x]\n", status);

            /* Send filePath as the args to the slave to demonstrate how args
             * are used.
             */
            args [0] = filePath;
            status = ProcMgr_load (handle,
                                   filePath,
                                   NUM_ARGS,
                                   args,
                                   NULL,
                                   &fileId);
            if (status < 0) {
                printf ("Error in ProcMgr_load [0x%x]\n", status);
            }
            else {
                printf ("ProcMgr_load status: [0x%x]\n", status);
            }

            if (status >= 0) {
                ProcMgr_getStartParams (handle, &startParams);
                status = ProcMgr_start (handle, &startParams);
                if (status < 0) {
                    printf ("ProcMgr_start failed [0x%x]\n", status);
                }
                else {
                    printf ("ProcMgr_start passed [0x%x]\n", status);
                }
            }
        }

        status = ProcMgr_close (&handle);
        printf ("ProcMgr_close status: [0x%x]\n", status);
    }

    printf ("Leaving ProcMgrApp_startup\n");

    return 0;
}

/*!
 *  @brief  Function to execute the shutdown for ProcMgrApp sample application
 */
Int
ProcMgrApp_shutdown (UInt16 procId)
{
    Int                          status = 0;
    ProcMgr_Handle               handle = NULL;
    UInt32                       fileId;

    printf ("Entered ProcMgrApp_shutdown\n");

    status = ProcMgr_open (&handle, procId);

    if (status >= 0) {

        status = ProcMgr_stop (handle);
        printf ("ProcMgr_stop status: [0x%x]\n", status);

        fileId = ProcMgr_getLoadedFileId (handle);
        status = ProcMgr_unload (handle, fileId) ;
        printf ("ProcMgr_unload status: [0x%x]\n", status);

        status = ProcMgr_detach (handle);
        printf ("ProcMgr_detach status: [0x%x]\n", status);

        status = ProcMgr_close (&handle);
        printf ("ProcMgr_close status: [0x%x]\n", status);
    }

    printf ("Leaving ProcMgrApp_shutdown\n");

    return 0;
}

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
