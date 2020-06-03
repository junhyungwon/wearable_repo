/*
 *  @file   SharedRegionApp.c
 *
 *  @brief      Sample application for SharedRegion module
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
#include <ti/syslink/utils/String.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/utils/Memory.h>

/* Module level headers */
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/SharedRegion.h>

#include "SharedRegionApp.h"



/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
/*!
 *  @brief  Shared memory base virtual address
 */
UInt32 SharedRegionApp_shAddrBase;


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
Int SharedRegionApp_startup (Void)
{
    Int    status = 0;
    UInt16 i;
    SharedRegion_Entry entry;
    Char    tmpStr[80];

    Osal_printf ("Entered SharedRegionApp_startup\n");

    for (i = 1 ; i < NUM_SHAREDREGIONS ; i++) {
        /*  Create a shared region with an id  identified by i */
        /* base address of the region */
        entry.base = (Ptr) (  SharedRegionApp_shAddrBase
                                       + (i * SHAREDREGION_SIZE));

        /*Length of the region */
        entry.len = SHAREDREGION_SIZE;

        /*MultiProc id of the owner of the region */
        entry.ownerProcId = MultiProc_self();

        /*Whether the region is valid */
        entry.isValid = TRUE;

        /*Whether to perform cache operations for the region */
        entry.cacheEnable = FALSE;

        /*The cache line size of the region */
        entry.cacheLineSize = 128;
        entry.createHeap = TRUE;
        /*The name of the region */
        sprintf(tmpStr,"AppSharedRegion%d" ,i);
        entry.name = tmpStr;

        status = SharedRegion_setEntry ((SHAREDREGION_ID + i), &entry);

        if (status < 0) {
            Osal_printf ("Error in SharedRegion_setEntry. Status [0x%x]"
                         " for:\n"
                         "    ID     : [%d]\n"
                         "    Address: [0x%x]\n"
                         "    Size   : [0x%x]\n",
                         status,
                         (SHAREDREGION_ID + i),
                         (Ptr) (  SharedRegionApp_shAddrBase
                                        + (i * SHAREDREGION_SIZE)),
                         SHAREDREGION_SIZE);
            break;
        }
        else {
            Osal_printf ("SharedRegion_setEntry Status [0x%x]\n", status);
        }
    }

    return status;
}


Int
SharedRegionApp_execute (Void)
{
    SharedRegion_SRPtr  srPtr;
    Int32               index;
    UInt32              usrPtr;
    UInt16              i;
    UInt32              curAddr;
    Ptr                 heapMemMPHandle;
    Ptr                 allocBufPtr;

    Osal_printf (" In SharedRegionApp_execute\n");
    /* Allocate and Free 0x1000 bytes from each heapMemMP created internally */
    for (i = 1 ; i < NUM_SHAREDREGIONS ; i++) {
        heapMemMPHandle = SharedRegion_getHeap((SHAREDREGION_ID + i));
        allocBufPtr = Memory_alloc(heapMemMPHandle, 0x1000, 0, NULL);
        if (allocBufPtr == NULL) {
            Osal_printf ("\nFailed to allocate 0x1000 bytes from shared"
                         "region  %d\n", (SHAREDREGION_ID + i));
        }
        else {
            Osal_printf ("\nSuccessfully allocated 0x1000 bytes from heap"
                         " associated with  shared"
                         "region  %d\n", (SHAREDREGION_ID + i));
            Memory_free(heapMemMPHandle, allocBufPtr, 0x1000);
            Osal_printf ("\nSuccessfully freed 0x1000 bytes to heap"
                         " associated with  shared"
                         "region  %d\n", (SHAREDREGION_ID + i));

        }
    }

    /* Verify translations for the different regions. */
    for (i = 1 ; i < NUM_SHAREDREGIONS ; i++) {
        curAddr = SharedRegionApp_shAddrBase + (i * SHAREDREGION_SIZE);
        index = SharedRegion_getId ((Ptr) curAddr);
        Osal_printf ("\nTesting shared region %d\n", index);

        curAddr += 0x100;

        Osal_printf ("User vitual address   =  [0x%x]\n", curAddr);
        index = SharedRegion_getId ((Ptr) curAddr);
        srPtr = SharedRegion_getSRPtr ((Ptr) curAddr, index);
        Osal_printf ("Shared region pointer =  [0x%x]\n", srPtr);

        /* Translate it back to get the original pointer. */
        usrPtr = (UInt32) SharedRegion_getPtr(srPtr);
        if (usrPtr != curAddr) {
            Osal_printf ("*** Failed to get the same user vitual pointer ***\n"
                         "    Expected [0x%x]\n"
                         "    Received [0x%x]\n",
                         curAddr,
                         usrPtr);
        }

        curAddr += 0x200;

        Osal_printf ("User vitual address   =  [0x%x]\n", curAddr);
        index = SharedRegion_getId ((Ptr) curAddr);
        srPtr = SharedRegion_getSRPtr((Ptr) curAddr, index);
        Osal_printf ("Shared region pointer =  [0x%x]\n", srPtr);

        /* Translate it back to get the original pointer. */
        usrPtr = (UInt32) SharedRegion_getPtr(srPtr);
        if (usrPtr != curAddr) {
            Osal_printf ("*** Failed to get the same user vitual pointer ***\n"
                         "    Expected [0x%x]\n"
                         "    Received [0x%x]\n",
                         curAddr,
                         usrPtr);
        }

        curAddr += 0x1000;

        Osal_printf ("User vitual address   =  [0x%x]\n", curAddr);
        index = SharedRegion_getId ((Ptr) curAddr);
        srPtr = SharedRegion_getSRPtr((Ptr) curAddr, index);
        Osal_printf ("Shared region pointer =  [0x%x]\n", srPtr);

        /* Translate it back to get the original pointer. */
        usrPtr = (UInt32) SharedRegion_getPtr(srPtr);
        if (usrPtr != curAddr) {
            Osal_printf ("*** Failed to get the same user vitual pointer ***\n"
                         "    Expected [0x%x]\n"
                         "    Received [0x%x]\n",
                         curAddr,
                         usrPtr);
        }

        curAddr += 0x2000;

        Osal_printf ("User vitual address   =  [0x%x]\n", curAddr);
        index = SharedRegion_getId ((Ptr) curAddr);
        srPtr = SharedRegion_getSRPtr((Ptr) curAddr, index);
        Osal_printf ("Shared region pointer =  [0x%x]\n", srPtr);

        /* Translate it back to get the original pointer. */
        usrPtr = (UInt32) SharedRegion_getPtr(srPtr);
        if (usrPtr != curAddr) {
            Osal_printf ("*** Failed to get the same user vitual pointer ***\n"
                         "    Expected [0x%x]\n"
                         "    Received [0x%x]\n",
                         curAddr,
                         usrPtr);
        }
    }

    return (0);
}


Int
SharedRegionApp_shutdown (Void)
{
    Int status = 0;
    UInt16 i;

    for (i = 0 ; i < NUM_SHAREDREGIONS ; i++) {
        /* Clearing 0 entry is not supported */
        if (i != 0) {
            status = SharedRegion_clearEntry (SHAREDREGION_ID + i);
            Osal_printf ("SharedRegion_remove for %d. Status [0x%x]\n",
                         SHAREDREGION_ID + i,
                         status);
        }
    }

    Osal_printf ("Leaving SharedRegionApp_shutdown\n");

    return (0);

}
