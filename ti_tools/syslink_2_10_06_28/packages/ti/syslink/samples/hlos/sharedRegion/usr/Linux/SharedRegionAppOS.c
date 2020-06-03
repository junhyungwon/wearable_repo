/*
 *  @file   SharedRegionAppOS.c
 *
 *  @brief      OS-specific sample application driver module for SharedRegion
 *              module
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Standard headers */
#include <ti/syslink/Std.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/SysLink.h>

/* Ipc header */
#include <ti/ipc/Ipc.h>
#include <ti/syslink/IpcHost.h>
/* Module level headers */
#include <ti/ipc/MultiProc.h>
#include <ti/syslink/ProcMgr.h>
#include <ti/syslink/samples/hlos/slaveLoader/SlaveLoader.h>
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>

#include "../../SharedRegionApp.h"

/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */

/** ============================================================================
 *  Extern declarations
 *  ============================================================================
 */
/*!
 *  @brief  Function to execute the startup for sharedRegionApp sample
 *          application
 */
extern Int SharedRegionApp_startup (Void);
/*!
 *  @brief  Function to execute the startup for sharedRegionApp sample
 *          application
 */
extern Int SharedRegionApp_execute (Void);

/*!
 *  @brief  Function to execute the shutdown for sharedRegionApp sample
 *          application
 */
extern Int SharedRegionApp_shutdown (Void);


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
UInt32  SharedRegionApp_sharedMem = 0;
UInt16  SharedRegionApp_numProcs  = 0;
String  SharedRegionApp_filePath[SysLinkSamples_MAXPROCESSORS]={NULL};
UInt16  SharedRegionApp_procId[SysLinkSamples_MAXPROCESSORS]={0};
Bool    SharedRegionApp_runAll = FALSE;
UInt32  SharedRegionApp_resetVector[SysLinkSamples_MAXPROCESSORS]={0};

/* Setting the bootmode */
ProcMgr_BootMode  SharedRegionApp_bootMode = ProcMgr_BootMode_Boot; 
/* ProcMgr_BootMode  SharedRegionApp_bootMode = ProcMgr_BootMode_NoBoot; */

extern UInt32 SharedRegionApp_shAddrBase;
ProcMgr_AddrInfo SharedRegionApp_addrInfo;
ProcMgr_Handle SharedRegionApp_procMgrHandle = NULL;


/** ============================================================================
 *  Function declarations
 *  ============================================================================
 */
Void SharedRegionApp_printUsageInfo(Void);


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
int
main (int argc, char ** argv)
{
    Int     status = 0;
    UInt16  i      = 0;
    UInt16  j      = 0;
    UInt16  validProcId = MultiProc_INVALIDID;
    String  mapFile = NULL;
    Ptr     arg;

    Osal_printf("SharedRegionApp sample application\n");

    SysLink_setup();

    /* Execute common startup functionality for all sample applications */
    SysLinkSamples_startup();

    if ((argc == 2) && (strcmp (argv [1], "--help") == 0)) {
        status = -1;
        SharedRegionApp_printUsageInfo ();
    }
    else if (argc < 2) {
        status = -1;
        SharedRegionApp_printUsageInfo ();
    }
    else {
        /* Get the physical address of shared memory to be used for the
         * application from the user.
         */
        SharedRegionApp_sharedMem = strtoul (argv [1], NULL, 16);
        Osal_printf ("Physical address of shared memory to be used: [0x%x]\n",
                     SharedRegionApp_sharedMem);
    }

    if ((status >= 0) && (argc > 2)) {
        SharedRegionApp_numProcs = strtol (argv [2], NULL, 16);
        j = 3;
        for (i = 0 ; i < SharedRegionApp_numProcs; i++, j=j+2) {
            /* Validate passed core name and get its ID. */
            SharedRegionApp_procId [i] = MultiProc_getId (argv [j]);
            if (SharedRegionApp_procId [i] == MultiProc_INVALIDID) {
                status = -1;
                Osal_printf("Invalid <Core name> specified!\n");
                SharedRegionApp_printUsageInfo ();
                break;
            }
            else {
                if (SharedRegionApp_bootMode == ProcMgr_BootMode_Boot) {
                    SharedRegionApp_filePath [i] = argv [j + 1];
                }
                else {
                    SharedRegionApp_resetVector [SharedRegionApp_procId[i]] = 
                        strtoll (argv [j + 1], NULL, 16);
                }
            }
        }

        if ((i == SharedRegionApp_numProcs) && (argc > (3 + (2 * i)))) {
            /* another arg available, it's our map file */
            mapFile = argv[3 + (2 * i)];
            Osal_printf("Received %s\n", mapFile);
        }
    }
    else if (status >= 0) {
        /* If no special run instructions are given, run for all procs. */
        SharedRegionApp_runAll = TRUE;
        Osal_printf ("Run for all supported proc IDs\n");
    }

    if (status >= 0) {
        /* Set the slaves to run the application with. */
        status = SysLinkSamples_setToRunProcIds (SharedRegionApp_numProcs,
                                                 SharedRegionApp_procId,
                                                 SharedRegionApp_runAll);

        for (i = 0; (i < SharedRegionApp_numProcs) && (status >= 0); i++) {
            if (SysLinkSamples_toRunProcId (SharedRegionApp_procId [i])) {
                if (SharedRegionApp_bootMode == ProcMgr_BootMode_Boot) {
                    Osal_printf ("Loading and starting procId [%d] with [%s]\n",
                                 SharedRegionApp_procId [i],
                                 SharedRegionApp_filePath [i]);
                    status = SlaveLoader_startup (SharedRegionApp_procId [i],
                                                  "startup",
                                                  SharedRegionApp_filePath [i], mapFile);
                    Osal_printf ("SlaveLoader_startup status [%d]\n", status);
                }
            }
        }

        for (i = 0; (i < MultiProc_getNumProcessors ()) && (status >= 0); i++) {
            if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
                /* Store any valid proc ID so that we can use it for opening
                 * the handle and map/translate later.
                 */
                validProcId = i;
                if (SharedRegionApp_bootMode == ProcMgr_BootMode_Boot) {
                    arg = NULL;
                }
                else {
                    arg = (Ptr)&SharedRegionApp_resetVector[i];
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
                            "Ipc_CONTROLCMD_LOADCALLBACK [0x%x]\n", status);
                }
                else {
                    Osal_printf ("After Ipc_loadcallback:\n"
                                 "    status [0x%x]\n", status);
                }
                if (status >= 0) {
                    status = Ipc_control(i, Ipc_CONTROLCMD_STARTCALLBACK,
                            NULL);
                    if (status < 0) {
                        Osal_printf("Error in Ipc_control "
                                "Ipc_CONTROLCMD_STARTCALLBACK[0x%x]\n",
                                status);
                    }
                    else {
                        Osal_printf("After Ipc_startcallback: \n"
                                "    status [0x%x]\n", status);
                    }
                }
            }
        }
    }

    if (status >= 0) {
        /* Open a handle to any slave processor to get a handle that can be used
         * for mapping and translating into master address space. It is not
         * important which slave is used, but it just needs to be a slave
         * that gives a valid handle from ProcMgr_open API.
         */
        status = ProcMgr_open (&SharedRegionApp_procMgrHandle, validProcId);
        if (status < 0) {
            Osal_printf ("Error in ProcMgr_open [0x%x]\n", status);
        }
        else {
            Osal_printf ("ProcMgr_open Status [0x%x]\n", status);

            SharedRegionApp_addrInfo.addr [ProcMgr_AddrType_MasterPhys] = SharedRegionApp_sharedMem;
            SharedRegionApp_addrInfo.addr [ProcMgr_AddrType_SlaveVirt]  = SharedRegionApp_sharedMem;
            SharedRegionApp_addrInfo.size     = SHAREDMEMSIZE;
            SharedRegionApp_addrInfo.isCached = FALSE;
            status = ProcMgr_map(SharedRegionApp_procMgrHandle,
                    (ProcMgr_SLAVEVIRT | ProcMgr_MASTERUSRVIRT
                            | ProcMgr_MASTERKNLVIRT),
                    &SharedRegionApp_addrInfo,
                    ProcMgr_AddrType_MasterPhys);
            if (status < 0) {
                Osal_printf ("Error in ProcMgr_map [0x%x]\n", status);
            }
            else {
                Osal_printf ("ProcMgr_map Status [0x%x]\n", status);
                Osal_printf ("\nCalling ProcMgr_translateAddr\n");
                status = ProcMgr_translateAddr (SharedRegionApp_procMgrHandle,
                                                (Ptr) &SharedRegionApp_shAddrBase,
                                                ProcMgr_AddrType_MasterUsrVirt,
                                                (Ptr) SharedRegionApp_sharedMem,
                                                ProcMgr_AddrType_MasterPhys);

                if (status < 0) {
                    Osal_printf ("Error in ProcMgr_translateAddr [0x%x]\n", status);
                }
                else {
                    Osal_printf ("\n\nProcMgr_translateAddr Status [0x%x]"
                                 " SharedRegionApp_shAddrBase [0x%x]\n",
                                 status, SharedRegionApp_shAddrBase);
                }
            }
        }
    }

    if (status >= 0) {
        status = SharedRegionApp_startup ();
        if (status >= 0) {
            SharedRegionApp_execute ();
        }

        SharedRegionApp_shutdown();
    }

    if (status >= 0) {
        ProcMgr_unmap(SharedRegionApp_procMgrHandle,
                (ProcMgr_SLAVEVIRT | ProcMgr_MASTERUSRVIRT |
                        ProcMgr_MASTERKNLVIRT),
                &SharedRegionApp_addrInfo, ProcMgr_AddrType_MasterPhys);

        ProcMgr_close (&SharedRegionApp_procMgrHandle);
        for (i = 0; i < MultiProc_getNumProcessors (); i++) {
            if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
                if (SharedRegionApp_bootMode == ProcMgr_BootMode_Boot) {
                    arg = NULL; 
                }
                else {
                    arg = (Ptr)&SharedRegionApp_resetVector[i];
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

        for (i = 0; i < SharedRegionApp_numProcs; i++) {
            if (SysLinkSamples_toRunProcId (SharedRegionApp_procId [i])) {
                if (SharedRegionApp_bootMode == ProcMgr_BootMode_Boot) {
                    Osal_printf ("Shutting down procId [%d]\n",
                                 SharedRegionApp_procId [i]);
                    status = SlaveLoader_shutdown (SharedRegionApp_procId [i],
                                                   "shutdown", mapFile);
                    Osal_printf ("SlaveLoader_shutdown status [%d]\n", status);
                }
            }
        }
    }

    /* Execute common shutdown functionality for all sample applications */
    SysLinkSamples_shutdown();

    SysLink_destroy();

    return 0;
}


Void SharedRegionApp_printUsageInfo(Void)
{
    UInt16 numProcs;
    UInt16 i;

    Osal_printf ("Usage : ./sharedregionapp_<debug|release> "
              "<physical address for shared region> <Number of slave cores>"
              " <Core name 1> <File path 1> <Core name 2> <File path 2> "
              "[mapFile]...\n\nSupported core names:\n");
    numProcs = MultiProc_getNumProcessors ();
    for (i = 0 ; (i < numProcs) && (i != MultiProc_self ()) ; i++) {
        if (SysLinkSamples_isAvailableProcId (i)) {
            Osal_printf ("- %s\n", MultiProc_getName (i));
        }
    }
}
