/*
 *  @file   ListMPApp.c
 *
 *  @brief      ListMP application on RTOS side
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


#include <xdc/std.h>
#include <string.h>

/*  -----------------------------------XDC.RUNTIME module Headers    */
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/knl/Cache.h>

/*  ----------------------------------- Utils Headers                */
#include <ti/ipc/MultiProc.h>

/*  ----------------------------------- IPC module Headers           */
#include <ti/ipc/GateMP.h>
#include <ti/ipc/HeapBufMP.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/ListMP.h>
#include <ti/ipc/Ipc.h>


/*  ----------------------------------- BIOS6 module Headers         */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/*  ----------------------------------- To get globals from .cfg Header */
#include <xdc/cfg/global.h>


/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/*!
 *  @brief  Shared Region ID
 */
#define APP_SHAREDREGION_ENTRY_ID   0u

/*!
 *  @brief  prefix for the host listmp.
 */
#define LISTMPHOST_PREFIX          "LISTMPHOST"

/*!
 *  @brief  prefix for the slave listmp.
 */
#define LISTMPSLAVE_PREFIX         "LISTMPSLAVE"

/*!
 *  @brief  Length of message Q Names
 */
#define  LISTMPAPP_NAMELENGTH       80u

/*!
 *  @brief  Buffer size
 */
#define BUFSIZE                     256u

/*!
 *  @brief  Indicates that the buffer was put into the list locally
 */
#define LOCALSTAMP                  0xCDCD

/*!
 *  @brief  Indicates that the buffer was put into the list from remote
 *          processor
 */
#define REMOTESTAMP                 0xABAB

typedef struct ListMP_Node_Tag {
    ListMP_Elem elem;
    UInt32      value;
    UInt32      procId;
    UInt32 buffer [  (BUFSIZE - (sizeof (ListMP_Elem) + (2 * sizeof (UInt32))))
                   / sizeof (UInt32)];
} ListMP_Node;


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
Int main()
{
    Int32 status = Ipc_S_SUCCESS;
    do {
        /* Call Ipc_start() */
        status = Ipc_start();
    } while (status != Ipc_S_SUCCESS);
    BIOS_start();
    return (0);
}

/*
 *  ======== ListMPApp_taskFxn ========
 *  Receive and return buffers
 */
Void ListMPApp_taskFxn(UArg arg0, UArg arg1)
{
    Int32                status = 0;
    Bool                 done   = FALSE;
    UInt16               procId = (UInt16) arg0;
    ListMP_Params        listMPParamsLocal;
    ListMP_Node *        node;
    Error_Block          eb;
    ListMP_Handle        ListMPApp_handleRemote;
    ListMP_Handle        ListMPApp_handleLocal;
    IHeap_Handle         ListMPApp_heapHandle;
    UInt32               localProcId;
    Char                 tempStr [LISTMPAPP_NAMELENGTH];
    UInt16               remoteProcId = (UInt16) arg0;

    Error_init(&eb);

    do {
        status = Ipc_attach(remoteProcId);
    } while (status < 0);


    /* Create Application specific shared memory */
    localProcId = MultiProc_self ( );

    ListMPApp_heapHandle = SharedRegion_getHeap (APP_SHAREDREGION_ENTRY_ID);

    if (ListMPApp_heapHandle == NULL) {
        System_printf("ERROR:SharedRegion_getHeap failed \n");
    }

    /* -------------------------------------------------------------------------
     * Create and open lists
     * -------------------------------------------------------------------------
     */
    ListMP_Params_init(&listMPParamsLocal);
    listMPParamsLocal.regionId = APP_SHAREDREGION_ENTRY_ID;
    memset (tempStr, 0, LISTMPAPP_NAMELENGTH);
    System_sprintf (tempStr,
                    "%s_%d%d",
                    LISTMPSLAVE_PREFIX,
                    procId,
                    localProcId);
    listMPParamsLocal.name = tempStr;
    ListMPApp_handleLocal = ListMP_create(&listMPParamsLocal);

    /* Open remote list. */
    memset (tempStr, 0, LISTMPAPP_NAMELENGTH);
    System_sprintf (tempStr,
                    "%s_%d%d",
                    LISTMPHOST_PREFIX,
                    procId,
                    localProcId);

    /* Open the remote list by name */
    do {
        status = ListMP_open(tempStr,&ListMPApp_handleRemote);
        if (status == ListMP_E_NOTFOUND) {
            /* Sleep for a while before trying again. */
            Task_sleep (10);
        }
    }
    while (status != ListMP_S_SUCCESS);

    /* -------------------------------------------------------------------------
     * Allocate and put buffer in remotely created list.
     * -------------------------------------------------------------------------
     */
    node = (ListMP_Node *) Memory_alloc (ListMPApp_heapHandle,
                                         sizeof (ListMP_Node),
                                         0,
                                         &eb);
    if (node == NULL) {
        System_abort("Error in Memory_alloc.\n" );
    }
    else {
        node->procId = localProcId;
        node->value = LOCALSTAMP;
        Cache_wbInv(node, sizeof(ListMP_Node), TRUE, NULL);
        ListMP_putTail (ListMPApp_handleRemote, &(node->elem));
    }

    /* -------------------------------------------------------------------------
     * Get and free buffer from locally created list.
     * -------------------------------------------------------------------------
     */
    do {
        node = (ListMP_Node *) ListMP_getHead (ListMPApp_handleLocal);
        /* Sleep for a while if the element is not yet available. */
        if (node == NULL) {
            Task_sleep (10);
        }
    }
    while (node == NULL);

    Cache_inv(node, sizeof(ListMP_Node), TRUE, NULL);
    /* Verify data integrity */
    if (    (node->procId != procId)
        ||  (node->value != REMOTESTAMP)) {
        System_abort("Data Integrity Error!!!\n");
    }

    Memory_free (ListMPApp_heapHandle, node, sizeof (ListMP_Node));

    /* -------------------------------------------------------------------------
     * Allocate and put two buffers in locally created list.
     * -------------------------------------------------------------------------
     */
    node = (ListMP_Node *) Memory_alloc (ListMPApp_heapHandle,
                                         sizeof (ListMP_Node),
                                         0,
                                         &eb);
    if (node == NULL) {
        System_abort("Error in Memory_alloc.\n" );
    }
    else {
        node->procId = localProcId;
        node->value = LOCALSTAMP;
        Cache_wbInv(node, sizeof(ListMP_Node), TRUE, NULL);
        ListMP_putTail (ListMPApp_handleLocal, &(node->elem));
    }

    node = (ListMP_Node *) Memory_alloc (ListMPApp_heapHandle,
                                         sizeof (ListMP_Node),
                                         0,
                                         &eb);
    if (node == NULL) {
        System_abort("Error in Memory_alloc.\n" );
    }
    else {
        node->procId = localProcId;
        node->value = LOCALSTAMP;
        Cache_wbInv(node, sizeof(ListMP_Node), TRUE, NULL);
        ListMP_putTail (ListMPApp_handleLocal, &(node->elem));
    }

    /* -------------------------------------------------------------------------
     * Get and free two buffers from remotely created list.
     * -------------------------------------------------------------------------
     */
    /* Wait till ListMP_next returns a buffer from remote processor. */
    do {
        node = (ListMP_Node *) ListMP_next (ListMPApp_handleRemote, NULL);
        if (node == NULL) {
            /* Sleep for a while if the element is not yet available. */
            Task_sleep (10);
        }
        else {
            if (    (node->procId == localProcId)
                ||  (node->value  == LOCALSTAMP)){
                /* Sleep for a while if the other proc has still not removed
                 * the element we put in. */
                Task_sleep (10);
            }
            else {
                done = TRUE;
            }
        }
    }
    while (done == FALSE);

    do {
        node = (ListMP_Node *) ListMP_getTail (ListMPApp_handleRemote);
        /* Sleep for a while if the element is not yet available. */
        if (node == NULL) {
            Task_sleep (10);
        }
    }
    while (node == NULL);

    Cache_inv(node, sizeof(ListMP_Node), TRUE, NULL);
    /* Verify data integrity */
    if (    (node->procId != procId)
        ||  (node->value != REMOTESTAMP)){
        System_abort("Data Integrity Error!!!\n" );
    }
    Memory_free (ListMPApp_heapHandle, node, sizeof (ListMP_Node));

    do {
        node = (ListMP_Node *) ListMP_getHead (ListMPApp_handleRemote);
        /* Sleep for a while if the element is not yet available. */
        if (node == NULL) {
            Task_sleep (10);
        }
    }
    while (node == NULL);

    Cache_inv(node, sizeof(ListMP_Node), TRUE, NULL);
    /* Verify data integrity */
    if (    (node->procId != procId)
        ||  (node->value != REMOTESTAMP)){
        System_abort("Data Integrity Error!!!\n" );
    }
    Memory_free (ListMPApp_heapHandle, node, sizeof (ListMP_Node));

    /* -------------------------------------------------------------------------
     * Cleanup
     * -------------------------------------------------------------------------
     */
    status = ListMP_close (&ListMPApp_handleRemote);
    if (status != ListMP_S_SUCCESS) {
        System_printf("ERROR: ListMP_close failed.status [0x%x]\n", status);
    }
    status = ListMP_delete (&ListMPApp_handleLocal);
    if (status != ListMP_S_SUCCESS) {
        System_printf("ERROR: ListMP_delete failed.status [0x%x]\n", status);
    }

    /* Spin until Ipc_detach success then stop */
    do {
        status = Ipc_detach(remoteProcId);
    } while (status < 0);

    Ipc_stop();

    System_printf("The test is complete\n");
}
