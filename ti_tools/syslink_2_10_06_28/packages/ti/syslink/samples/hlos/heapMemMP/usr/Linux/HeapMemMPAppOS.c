/*
 *  @file   HeapMemMPAppOS.c
 *
 *  @brief      OS-specific sample application driver module for heapBuf
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
#include <ti/syslink/ProcMgr.h>
#include <ti/ipc/MultiProc.h>

/* Sample app headers */
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>


/** ============================================================================
 *  Extern declarations
 *  ============================================================================
 */
extern Int HeapMemMPApp_startup(String mapFile);
extern Int HeapMemMPApp_execute(Void);
extern Int HeapMemMPApp_shutdown(String mapFile);


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
UInt16  HeapMemMPApp_numProcs           = 0;
String  HeapMemMPApp_filePath [SysLinkSamples_MAXPROCESSORS]={NULL};
UInt16  HeapMemMPApp_procId   [SysLinkSamples_MAXPROCESSORS]={0};
Bool    HeapMemMPApp_runAll = FALSE;
UInt32  HeapMemMPApp_resetVector[SysLinkSamples_MAXPROCESSORS]={0};

/* Setting the bootmode */
ProcMgr_BootMode  HeapMemMPApp_bootMode = ProcMgr_BootMode_Boot;
/*ProcMgr_BootMode  HeapMemMPApp_bootMode = ProcMgr_BootMode_NoBoot;*/


/** ============================================================================
 *  Function declarations
 *  ============================================================================
 */
Void HeapMemMPApp_printUsageInfo (Void);


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
    String mapFile = NULL;

    Osal_printf ("HeapMemMPApp sample application\n");

    SysLink_setup ();

    /* Execute common startup functionality for all sample applications */
    SysLinkSamples_startup ();

    if ((argc == 2) && (strcmp (argv [1], "--help") == 0)) {
        status = -1;
        HeapMemMPApp_printUsageInfo();
    }
    else if (argc > 1) {
        HeapMemMPApp_numProcs = strtol (argv [1], NULL, 16);
        j = 2;
        for (i = 0 ; i < HeapMemMPApp_numProcs; i++, j=j+2) {
            /* Validate passed core name and get its ID. */
            HeapMemMPApp_procId [i] = MultiProc_getId (argv [j]);
            if (HeapMemMPApp_procId [i] == MultiProc_INVALIDID) {
                status = -1;
                Osal_printf("Invalid <Core name> specified!\n");
                HeapMemMPApp_printUsageInfo ();
                break;
            }
            else {
                if (HeapMemMPApp_bootMode == ProcMgr_BootMode_Boot) {
                    HeapMemMPApp_filePath [i] = argv [j + 1];
                }
                else {
                    HeapMemMPApp_resetVector [HeapMemMPApp_procId[i]] = 
                        strtoll (argv [j + 1], NULL, 16);
                }
            }
        }

        if ((i == HeapMemMPApp_numProcs) && (argc > (2 + (2 * i)))) {
            /* another arg available, it's our map file */
            mapFile = argv[2 + (2 * i)];
            Osal_printf("Received %s\n", mapFile);
        }
    }
    else {
        /* If no special run instructions are given, run for all procs. */
        HeapMemMPApp_runAll = TRUE;
        Osal_printf ("Run for all supported proc IDs\n");
    }

    if (status >= 0) {
        status = HeapMemMPApp_startup(mapFile);
        if (status >= 0) {
            status = HeapMemMPApp_execute();
            if (status < 0) {
                Osal_printf("HeapMemMPApp_execute failed!\n");
            }

            HeapMemMPApp_shutdown(mapFile);
        }
    }

    /* Execute common shutdown functionality for all sample applications */
    SysLinkSamples_shutdown();

    SysLink_destroy();

    return 0;
}


Void HeapMemMPApp_printUsageInfo (Void)
{
    UInt16 numProcs;
    UInt16 i;

    Osal_printf ("Usage : ./heapmemmpapp_<debug|release> <Number of slave cores>"
              " <Core name 1> <File path 1> <Core name 2> <File path 2> "
              "[mapFile]...\n\nSupported core names:\n");
    numProcs = MultiProc_getNumProcessors ();
    for (i = 0 ; (i < numProcs) && (i != MultiProc_self ()) ; i++) {
        if (SysLinkSamples_isAvailableProcId (i)) {
            Osal_printf ("- %s\n", MultiProc_getName (i));
        }
    }
}
