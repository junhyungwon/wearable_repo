/*
 *  @file   SharedRegionAppDrv.c
 *
 *  @brief      kernel side sample application file.
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

/* Ipc header */
#include <ti/ipc/Ipc.h>
#include <ti/syslink/IpcHost.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/utils/Memory.h>

#include <ti/ipc/MultiProc.h>
#include <ti/syslink/ProcMgr.h>
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>

/* Sample app headers */
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>

#include "../../SharedRegionApp.h"


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/*!
 *  @brief  Shared memory base virtual address
 */
extern UInt32 SharedRegionApp_shAddrBase;

/** ============================================================================
 *  Forward declarations of internal functions
 *  ============================================================================
 */
/*!
 *  @brief  Module initialization function for Linux driver.
 */
static int __init SharedRegionApp_initializeModule (void);


/*!
 *  @brief  Module finalization  function for Linux driver.
 */
static void  __exit SharedRegionApp_finalizeModule (void);


/** ============================================================================
 *  Extern declarations
 *  ============================================================================
 */

/** ============================================================================
 *  Globals
 *  ============================================================================
 */
UInt32  SharedRegionApp_resetVector[SysLinkSamples_MAXPROCESSORS]={0};
ProcMgr_BootMode  SharedRegionApp_bootMode = ProcMgr_BootMode_Boot;

/*!
 *  @brief  Indicates whether trace should be enabled.
 */
static Char * TRACE = FALSE;
module_param (TRACE, charp, S_IRUGO);
Bool SharedRegionApp_enableTrace = FALSE;

/*!
 *  @brief  Indicates whether entry/leave trace should be enabled.
 */
static Char * TRACEENTER = FALSE;
module_param (TRACEENTER, charp, S_IRUGO);
Bool SharedRegionApp_enableTraceEnter = FALSE;

/*!
 *  @brief  Indicates whether SetFailureReason trace should be enabled.
 */
static Char * TRACEFAILURE = FALSE;
module_param (TRACEFAILURE, charp, S_IRUGO);
Bool SharedRegionApp_enableTraceFailure = FALSE;

/*!
 *  @brief  Indicates class of trace to be enabled
 */
static Char * TRACECLASS = NULL;
module_param (TRACECLASS, charp, S_IRUGO);
UInt32 SharedRegionApp_traceClass = 0;

/*!
 *  @brief  Indicates physical address of shared memory to be used for the
 *          SharedRegion application
 */
static Char * SHAREDMEM = NULL;
module_param (SHAREDMEM, charp, S_IRUGO);
UInt32  SharedRegionApp_sharedMem = 0;

/*!
 *  @brief  Indicates number of slaves to run
 */
static Char * NUMPROCS = NULL;
module_param (NUMPROCS, charp, S_IRUGO);
UInt16 SharedRegionApp_numProcs = 0;

/*!
 *  @brief  Indicates proc IDs of slaves to run
 */
static Char * PROCID = NULL;
module_param (PROCID, charp, S_IRUGO);
UInt16  SharedRegionApp_procId   [SysLinkSamples_MAXPROCESSORS];

/*!
 *  @brief  Indicates that all slaves are to be run
 */
Bool    SharedRegionApp_runAll = TRUE;

/*!
 *  @brief  Indicates new trace mask to be enabled
 */
static UInt32 SharedRegionApp_traceMask = 0;

/*!
 *  @brief  Indicates old value of trace mask
 */
static UInt32 SharedRegionApp_oldMask   = 0;


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Module initialization  function for Linux driver.
 */
static
int
__init SharedRegionApp_initializeModule (void)
{
    Int32               status    = 0;
    Memory_MapInfo      mapInfo;
    UInt16              i;

    /* Display the version info and created date/time */
    Osal_printf ("SharedRegionApp sample app module created on"
                 " Date:%s Time:%s\n",
                 __DATE__,
                 __TIME__);

    Osal_printf ("Entered SharedRegionApp_initializeModule\n");

    /* Enable/disable levels of tracing. */
    if (TRACE != NULL) {
        SharedRegionApp_enableTrace = simple_strtol (TRACE, NULL, 16);
        if (    (SharedRegionApp_enableTrace != 0)
            &&  (SharedRegionApp_enableTrace != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACE\n") ;
        }
        else if (SharedRegionApp_enableTrace == TRUE) {
            Osal_printf ("Trace enabled\n");
            SharedRegionApp_traceMask |= GT_TraceState_Enable;
        }
        else if (SharedRegionApp_enableTrace == FALSE) {
            Osal_printf ("Trace disabled\n");
            SharedRegionApp_traceMask |= GT_TraceState_Disable;
        }
    }

    if (TRACEENTER != NULL) {
        SharedRegionApp_enableTraceEnter = simple_strtol (TRACEENTER, NULL, 16);
        if (    (SharedRegionApp_enableTraceEnter != 0)
            &&  (SharedRegionApp_enableTraceEnter != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (SharedRegionApp_enableTraceEnter == TRUE) {
            Osal_printf ("Trace entry/leave prints enabled\n");
            SharedRegionApp_traceMask |= GT_TraceEnter_Enable;
        }
    }

    if (TRACEFAILURE != NULL) {
        SharedRegionApp_enableTraceFailure = simple_strtol (TRACEFAILURE,
                                                            NULL,
                                                            16);
        if (    (SharedRegionApp_enableTraceFailure != 0)
            &&  (SharedRegionApp_enableTraceFailure != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (SharedRegionApp_enableTraceFailure == TRUE) {
            Osal_printf ("Trace SetFailureReason enabled\n");
            SharedRegionApp_traceMask |= GT_TraceSetFailure_Enable;
        }
    }

    if (TRACECLASS != NULL) {
        SharedRegionApp_traceClass = simple_strtol (TRACECLASS, NULL, 16);
        if (    (SharedRegionApp_traceClass != 0)
            &&  (SharedRegionApp_traceClass != 1)
            &&  (SharedRegionApp_traceClass != 2)
            &&  (SharedRegionApp_traceClass != 3)) {
            Osal_printf ("Error! Only 0/1/2/3 supported for TRACECLASS\n") ;
        }
        else {
            Osal_printf ("Trace class %s\n", TRACECLASS) ;
            SharedRegionApp_traceClass =
                    SharedRegionApp_traceClass << (32 - GT_TRACECLASS_SHIFT);
            SharedRegionApp_traceMask |= SharedRegionApp_traceClass;
        }
    }

    /* Set the desired trace value. */
    SharedRegionApp_oldMask = GT_setTrace (SharedRegionApp_traceMask,
                                           GT_TraceType_Kernel);

    /* Get value of numProcs */
    if (NUMPROCS != NULL) {
        SharedRegionApp_runAll = FALSE;
        SharedRegionApp_numProcs = simple_strtol (NUMPROCS, NULL, 16);
        Osal_printf ("Number of procs to run: %d\n", SharedRegionApp_numProcs);

        /* Get value of numProcs */
        if (PROCID != NULL) {
            for (i = 0; i < SharedRegionApp_numProcs; i++) {
                if (PROCID [i] == '\0') {
                    Osal_printf ("Insufficient procIDs specified. "
                                 "Need to specify: %d procIds\n",
                                 SharedRegionApp_numProcs);
                    status = -1;
                    break;
                }
                else {
                    SharedRegionApp_procId [i] = PROCID [i] - '0';
                    Osal_printf ("Proc ID to run: %d\n",
                                 SharedRegionApp_procId [i]);
                }
            }
        }
        else {
            Osal_printf ("Error! Must specify PROCID as well\n") ;
        }
    }

    /* Get value of sharedMem */
    if (SHAREDMEM == NULL) {
        Osal_printf ("Error! Must specify SHAREDMEM for physical address base"
                     "of shared memory to be used\n");
        status = -1;
    }
    else {
        SharedRegionApp_sharedMem = simple_strtoul (SHAREDMEM, NULL, 16);
        Osal_printf ("Physical address of shared memory to be used: [0x%x]\n",
                     SharedRegionApp_sharedMem);
    }

    if (status >= 0) {
        /* Execute common startup functionality for all sample applications */
        SysLinkSamples_startup ();

        /* Set the slaves to run the application with. */
        status = SysLinkSamples_setToRunProcIds (SharedRegionApp_numProcs,
                                                 SharedRegionApp_procId,
                                                 SharedRegionApp_runAll);

        for (i = 0; (i < MultiProc_getNumProcessors ()) && (status >= 0); i++) {
            if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
                if (SharedRegionApp_bootMode == ProcMgr_BootMode_Boot) {
                    status = Ipc_control (i,
                                          Ipc_CONTROLCMD_LOADCALLBACK,
                                          NULL);
                }
                else {
                    /* Call Ipc_control for ProcMgr_BootMode_NoLoad_Pwr,
                     * ProcMgr_BootMode_NoLoad_NoPwr
                     * and ProcMgr_BootMode_NoBoot modes
                     */
                    status = Ipc_control (i,
                                          Ipc_CONTROLCMD_LOADCALLBACK,
                                          (Ptr)&SharedRegionApp_resetVector[i]);
                }
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
                    status = Ipc_control (i,
                                          Ipc_CONTROLCMD_STARTCALLBACK,
                                          NULL);
                    if (status < 0) {
                        Osal_printf ("Error in Ipc_control "
                                     "Ipc_CONTROLCMD_STARTCALLBACK[0x%x]\n",
                                     status);
                    }
                    else {
                        Osal_printf ("After Ipc_startcallback: \n"
                                     "    status [0x%x]\n",
                                     status);
                    }
                }
            }
        }
    }

    if (status >= 0) {
        /* Map and create the shared region for  user created shared memory for
         *instances
         */
        mapInfo.src  = SharedRegionApp_sharedMem;
        mapInfo.size = SHAREDMEMSIZE;
        mapInfo.isCached = FALSE;

        status = Memory_map (&mapInfo);
        if (status < 0) {
                Osal_printf ("Error in Memory_map.Exiting [0x%x]\n", status);
                return (status) ;
        }
        else {
            SharedRegionApp_shAddrBase = mapInfo.dst;
            status = SharedRegionApp_startup ();
            if (status >= 0) {
                status = SharedRegionApp_execute ();
            }
        }
    }

    Osal_printf ("Leaving SharedRegionApp_initializeModule 0x%x\n", status);

    return 0;
}


/*!
 *  @brief  Linux driver function to finalize the driver module.
 */
static
void
__exit SharedRegionApp_finalizeModule (void)
{
    Int status = 0;
    Memory_UnmapInfo unmapInfo;
    UInt16  i;

    Osal_printf ("Entered SharedRegionApp_finalizeModule\n");

    status = SharedRegionApp_shutdown ();

    unmapInfo.addr = SharedRegionApp_shAddrBase;
    status = Memory_unmap (&unmapInfo);

    /* Restore the trace value. */
    SharedRegionApp_traceMask = GT_setTrace (SharedRegionApp_oldMask,
                                             GT_TraceType_Kernel);

    for (i = 0; i < MultiProc_getNumProcessors (); i++) {
        if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
            status = Ipc_control (i,
                                  Ipc_CONTROLCMD_STOPCALLBACK,
                                  NULL);
            Osal_printf ("After Ipc_stopCallback status: [0x%x]\n",
                         status);
        }
    }

    Osal_printf ("Leaving SharedRegionApp_finalizeModule 0x%x\n", status);
}


/*!
 *  @brief  Macro calls that indicate initialization and finalization functions
 *          to the kernel.
 */
MODULE_LICENSE ("GPL v2");
module_init (SharedRegionApp_initializeModule);
module_exit (SharedRegionApp_finalizeModule);


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
