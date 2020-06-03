/*
 *  @file   HeapBufMPAppDrv.c
 *
 *  @brief      OS-specific sample application driver module for HeapBufMP module
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



/* OS-specific headers */
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/pgtable.h>

/* Standard headers */
#include <ti/syslink/Std.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>

/* Sample app headers */
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  Forward declarations of internal functions
 *  ============================================================================
 */
/*!
 *  @brief  Module initialization function for Linux driver.
 */
static int __init HeapBufMP_initializeModule (void);

/*!
 *  @brief  Module finalization  function for Linux driver.
 */
static void  __exit HeapBufMP_finalizeModule (void);


/** ============================================================================
 *  Extern declarations
 *  ============================================================================
 */
/* Function to execute the startup for HeapBufMPApp sample application */
extern Int HeapBufMPApp_startup (Void);

/* Function to execute the HeapBufMPApp sample application */
extern Int HeapBufMPApp_execute (Void);

/* Function to execute the shutdown for HeapBufMPApp sample application */
extern Int HeapBufMPApp_shutdown (Void);


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
/*!
 *  @brief  Indicates whether trace should be enabled.
 */
static Char * TRACE = FALSE;
module_param (TRACE, charp, S_IRUGO);
Bool HeapBufMP_enableTrace = FALSE;

/*!
 *  @brief  Indicates whether entry/leave trace should be enabled.
 */
static Char * TRACEENTER = FALSE;
module_param (TRACEENTER, charp, S_IRUGO);
Bool HeapBufMP_enableTraceEnter = FALSE;

/*!
 *  @brief  Indicates whether SetFailureReason trace should be enabled.
 */
static Char * TRACEFAILURE = FALSE;
module_param (TRACEFAILURE, charp, S_IRUGO);
Bool HeapBufMP_enableTraceFailure = FALSE;

/*!
 *  @brief  Indicates class of trace to be enabled
 */
static Char * TRACECLASS = NULL;
module_param (TRACECLASS, charp, S_IRUGO);
UInt32 HeapBufMP_traceClass = 0;

/*!
 *  @brief  Indicates number of slaves to run
 */
static Char * NUMPROCS = NULL;
module_param (NUMPROCS, charp, S_IRUGO);
UInt16 HeapBufMPApp_numProcs = 0;

/*!
 *  @brief  Indicates proc IDs of slaves to run
 */
static Char * PROCID = NULL;
module_param (PROCID, charp, S_IRUGO);
UInt16  HeapBufMPApp_procId   [SysLinkSamples_MAXPROCESSORS];

/*!
 *  @brief  Indicates that all slaves are to be run
 */
Bool    HeapBufMPApp_runAll = TRUE;

/*!
 *  @brief  Indicates new trace mask to be enabled
 */
static UInt32 HeapBufMPApp_traceMask = 0;

/*!
 *  @brief  Indicates old value of trace mask
 */
static UInt32 HeapBufMPApp_oldMask   = 0;


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Module initialization  function for Linux driver.
 */
static
int
__init HeapBufMP_initializeModule (void)
{
    int   result = 0;
    Int32 status = 0;
    UInt16 i;

    /* Display the version info and created date/time */
    Osal_printf ("HeapBufMP sample app module created on Date:%s Time:%s\n",
                 __DATE__,
                 __TIME__);

    Osal_printf ("Entered HeapBufMP_initializeModule\n");

    /* Enable/disable levels of tracing. */
    if (TRACE != NULL) {
        HeapBufMP_enableTrace = simple_strtol (TRACE, NULL, 16);
        if ((HeapBufMP_enableTrace != 0) && (HeapBufMP_enableTrace != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACE\n") ;
        }
        else if (HeapBufMP_enableTrace == TRUE) {
            Osal_printf ("Trace enabled\n");
            HeapBufMPApp_traceMask = GT_TraceState_Enable;
        }
        else if (HeapBufMP_enableTrace == FALSE) {
            Osal_printf ("Trace disabled\n");
            HeapBufMPApp_traceMask = GT_TraceState_Disable;
        }
    }

    if (TRACEENTER != NULL) {
        HeapBufMP_enableTraceEnter = simple_strtol (TRACEENTER, NULL, 16);
        if (    (HeapBufMP_enableTraceEnter != 0)
            &&  (HeapBufMP_enableTraceEnter != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (HeapBufMP_enableTraceEnter == TRUE) {
            Osal_printf ("Trace entry/leave prints enabled\n");
            HeapBufMPApp_traceMask |= GT_TraceEnter_Enable;
        }
    }

    if (TRACEFAILURE != NULL) {
        HeapBufMP_enableTraceFailure = simple_strtol (TRACEFAILURE, NULL, 16);
        if (    (HeapBufMP_enableTraceFailure != 0)
            &&  (HeapBufMP_enableTraceFailure != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (HeapBufMP_enableTraceFailure == TRUE) {
            Osal_printf ("Trace SetFailureReason enabled\n");
            HeapBufMPApp_traceMask |= GT_TraceSetFailure_Enable;
        }
    }

    if (TRACECLASS != NULL) {
        HeapBufMP_traceClass = simple_strtol (TRACECLASS, NULL, 16);
        if (    (HeapBufMP_traceClass != 0)
            &&  (HeapBufMP_traceClass != 1)
            &&  (HeapBufMP_traceClass != 2)
            &&  (HeapBufMP_traceClass != 3)) {
            Osal_printf ("Error! Only 0/1/2/3 supported for TRACECLASS\n");
        }
        else {
            Osal_printf ("Trace class %s\n", TRACECLASS);
            HeapBufMP_traceClass =
                            HeapBufMP_traceClass << (32 - GT_TRACECLASS_SHIFT);
            HeapBufMPApp_traceMask |= HeapBufMP_traceClass;
        }
    }

    /* Set the desired trace value. */
    HeapBufMPApp_oldMask = GT_setTrace (HeapBufMPApp_traceMask,
                                        GT_TraceType_Kernel);

    /* Get value of numProcs */
    if (NUMPROCS != NULL) {
        HeapBufMPApp_runAll = FALSE;
        HeapBufMPApp_numProcs = simple_strtol (NUMPROCS, NULL, 16);
        Osal_printf ("Number of procs to run: %d\n", HeapBufMPApp_numProcs);

        /* Get value of numProcs */
        if (PROCID != NULL) {
            for (i = 0; i < HeapBufMPApp_numProcs; i++) {
                if (PROCID [i] == '\0') {
                    Osal_printf ("Insufficient procIDs specified. "
                                 "Need to specify: %d procIds\n",
                                 HeapBufMPApp_numProcs);
                    status = -1;
                    break;
                }
                else {
                    HeapBufMPApp_procId [i] = PROCID [i] - '0';
                    Osal_printf ("Proc ID to run: %d\n",
                                 HeapBufMPApp_procId [i]);
                }
            }
        }
        else {
            Osal_printf ("Error! Must specify PROCID as well\n") ;
        }
    }

    if (status >= 0) {
        /* Execute common startup functionality for all sample applications */
        SysLinkSamples_startup ();

        status = HeapBufMPApp_startup ();
        if (status >= 0) {
            status = HeapBufMPApp_execute ();
        }
    }

    Osal_printf ("Leaving HeapBufMP_initializeModule 0x%x\n", result);

    return result;
}


/*!
 *  @brief  Linux driver function to finalize the driver module.
 */
static
void
__exit HeapBufMP_finalizeModule (void)
{
    Int status = 0;

    Osal_printf ("Entered HeapBufMP_finalizeModule\n");

    status = HeapBufMPApp_shutdown ();

    /* Execute common shutdown functionality for all sample applications */
    SysLinkSamples_shutdown ();

    /* Restore the trace value. */
    HeapBufMPApp_traceMask = GT_setTrace (HeapBufMPApp_oldMask,
                                          GT_TraceType_Kernel);
}


/*!
 *  @brief  Macro calls that indicate initialization and finalization functions
 *          to the kernel.
 */
MODULE_LICENSE ("GPL v2");
module_init (HeapBufMP_initializeModule);
module_exit (HeapBufMP_finalizeModule);


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
