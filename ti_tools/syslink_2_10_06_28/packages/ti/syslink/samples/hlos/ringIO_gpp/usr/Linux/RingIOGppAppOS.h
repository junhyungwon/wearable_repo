/** 
 *  @file   RingIOGppAppOS.h
 *
 *  @brief      OS related functions
 *
 *
 */
/* 
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


/* OS-specific headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

/** ============================================================================
 *  @brief  Structure to hold thread/process information.
 *
 *  @field  tid
 *              Thread Identification number.
 *  @field  processorId
 *              ID of the dsp processor.
 *
 *  ============================================================================
 */
typedef struct RingIOGppApp_ClientInfo_tag {
#ifdef RingIOGppApp_MULTIPROCESS
    pid_t      pid;
    char       processName[32];
#else
    pthread_t  tid;
#endif
    UInt8      processorId;
} RingIOGppApp_ClientInfo;

/** ============================================================================
 *  @brief   Yields tasks execution.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\ring_io_os.c
 *
 *  @sa     None
 *  ============================================================================
 */
Void
RingIOGppApp_YieldClient (Void);

/** ============================================================================
 *  @brief   Sleeps for the specified number of microseconds.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\ring_io_os.c
 *  @sa     None
 *  ============================================================================
 */
Void
RingIOGppApp_Sleep (UInt32 uSec);

/** ============================================================================
 *  @brief   Function to create a new thread or a Process.
 *  ============================================================================
 */
UInt32
RingIOGppApp_Create_client (RingIOGppApp_ClientInfo * pInfo, Ptr funcPtr, Ptr args);

/** ============================================================================
 *  @brief   Wait for thread/process to terminate.
 *  ============================================================================
 */
Int
RingIOGppApp_Join_client (RingIOGppApp_ClientInfo *pInfo);

/** ============================================================================
 *  @brief   Function to exit from a caller task(thread/process).
 *
 *  ============================================================================
 */
Int
RingIOGppApp_Exit_client (RingIOGppApp_ClientInfo *pInfo);


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
