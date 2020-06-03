/*
 *  @file   SlaveLoaderDrv.c
 *
 *  @brief      OS-specific slave loader that uses the ProcMgr module
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
static int __init SlaveLoader_initializeModule (void);

/*!
 *  @brief  Module finalization  function for Linux driver.
 */
static void  __exit SlaveLoader_finalizeModule (void);

/*!
 *  @brief  Function to execute the startup for SlaveLoader sample application
 */
extern Int SlaveLoader_startup (UInt16 procId, String filePath);

/*!
 *  @brief  Function to execute the shutdown for SlaveLoader sample application
 */
extern Int SlaveLoader_shutdown (UInt16 procId);

/** ============================================================================
 *  Globals
 *  ============================================================================
 */
/*!
 *  @brief  Indicates whether trace should be enabled.
 */
static Char * TRACE = FALSE;
module_param (TRACE, charp, S_IRUGO);
Bool SlaveLoader_enableTrace = FALSE;

/*!
 *  @brief  Indicates whether entry/leave trace should be enabled.
 */
static Char * TRACEENTER = FALSE;
module_param (TRACEENTER, charp, S_IRUGO);
Bool SlaveLoader_enableTraceEnter = FALSE;

/*!
 *  @brief  Indicates whether SetFailureReason trace should be enabled.
 */
static Char * TRACEFAILURE = FALSE;
module_param (TRACEFAILURE, charp, S_IRUGO);
Bool SlaveLoader_enableTraceFailure = FALSE;

/*!
 *  @brief  Indicates class of trace to be enabled
 */
static Char * TRACECLASS = NULL;
module_param (TRACECLASS, charp, S_IRUGO);
UInt32 SlaveLoader_traceClass = 0;

/*!
 *  @brief  Gives the path of the file to be loaded
 */
static Char * FILEPATH = NULL;
module_param (FILEPATH, charp, S_IRUGO);

/*!
 *  @brief  Indicates new trace mask to be enabled
 */
static UInt32 SlaveLoader_traceMask = 0;

/*!
 *  @brief  Indicates old value of trace mask
 */
static UInt32 SlaveLoader_oldMask   = 0;


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Module initialization  function for Linux driver.
 */
static
int
__init SlaveLoader_initializeModule (Void)
{
    int result = 0;

    /* Display the version info and created date/time */
    Osal_printf ("Slave loader created on Date:%s Time:%s\n",
                 __DATE__,
                 __TIME__);

    /* Enable/disable levels of tracing. */
    if (TRACE != NULL) {
        SlaveLoader_enableTrace = simple_strtol (TRACE, NULL, 16);
        if ((SlaveLoader_enableTrace != 0) && (SlaveLoader_enableTrace != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACE\n") ;
        }
        else if (SlaveLoader_enableTrace == TRUE) {
            Osal_printf ("Trace enabled\n");
            SlaveLoader_traceMask = GT_TraceState_Enable;
        }
        else if (SlaveLoader_enableTrace == FALSE) {
            Osal_printf ("Trace enabled\n");
            SlaveLoader_traceMask = GT_TraceState_Disable;
        }
    }

    if (TRACEENTER != NULL) {
        SlaveLoader_enableTraceEnter = simple_strtol (TRACEENTER, NULL, 16);
        if (    (SlaveLoader_enableTraceEnter != 0)
            &&  (SlaveLoader_enableTraceEnter != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (SlaveLoader_enableTraceEnter == TRUE) {
            Osal_printf ("Trace entry/leave prints enabled\n");
            SlaveLoader_traceMask |= GT_TraceEnter_Enable;
        }
    }

    if (TRACEFAILURE != NULL) {
        SlaveLoader_enableTraceFailure = simple_strtol (TRACEFAILURE, NULL, 16);
        if (    (SlaveLoader_enableTraceFailure != 0)
            &&  (SlaveLoader_enableTraceFailure != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (SlaveLoader_enableTraceFailure == TRUE) {
            Osal_printf ("Trace SetFailureReason enabled\n");
            SlaveLoader_traceMask |= GT_TraceSetFailure_Enable;
        }
    }

    if (TRACECLASS != NULL) {
        SlaveLoader_traceClass = simple_strtol (TRACECLASS, NULL, 16);
        if (    (SlaveLoader_traceClass != 1)
            &&  (SlaveLoader_traceClass != 2)
            &&  (SlaveLoader_traceClass != 3)) {
            Osal_printf ("Error! Only 1/2/3 supported for TRACECLASS\n") ;
        }
        else {
            Osal_printf ("Trace class %s\n", TRACECLASS);
            SlaveLoader_traceClass =
                            SlaveLoader_traceClass << (32 - GT_TRACECLASS_SHIFT);
            SlaveLoader_traceMask |= SlaveLoader_traceClass;
        }
    }

    /* Set the desired trace value. */
    SlaveLoader_oldMask = GT_setTrace (SlaveLoader_traceMask,
                                       GT_TraceType_Kernel);

    if (FILEPATH != NULL) {
        Osal_printf ("File to be loaded %s\n", FILEPATH);
        result = SlaveLoader_startup (0, FILEPATH);
    }
    else {
        Osal_printf ("Error! Please provide FILEPATH for file"
                     " to be loaded\n");
    }

    return result;
}


/*!
 *  @brief  Linux driver function to finalize the driver module.
 */
static
void
__exit SlaveLoader_finalizeModule (void)
{
    int result = 0;

    result = SlaveLoader_shutdown (0);

    /* Restore the trace value. */
    SlaveLoader_traceMask = GT_setTrace (SlaveLoader_oldMask,
                                        GT_TraceType_Kernel);
}


/*!
 *  @brief  Macro calls that indicate initialization and finalization functions
 *          to the kernel.
 */
MODULE_LICENSE ("GPL v2");
module_init (SlaveLoader_initializeModule);
module_exit (SlaveLoader_finalizeModule);


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
