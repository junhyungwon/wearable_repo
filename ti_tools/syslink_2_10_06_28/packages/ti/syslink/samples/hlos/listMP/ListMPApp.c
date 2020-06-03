/*
 *  @file   ListMPApp.c
 *
 *  @brief      Sample application for ListMP module
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



#if !defined(__KERNEL__)
#include <string.h>
#include <unistd.h>
#endif /* if defined(__KERNEL__) */

/* Standard headers */
#include <ti/syslink/Std.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/syslink/utils/String.h>
#include <ti/syslink/utils/Cache.h>

#include <ti/syslink/ProcMgr.h>

/* Module level headers */
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/ListMP.h>
#include <ti/syslink/utils/IHeap.h>

/* Ipc header */
#include <ti/ipc/Ipc.h>
#include <ti/syslink/IpcHost.h>

#if defined(__KERNEL__)
#include <OsalThread.h>
#endif /* if defined(__KERNEL__) */

#if !defined(__KERNEL__)
#include <ti/syslink/samples/hlos/slaveLoader/SlaveLoader.h>
#endif /* if !defined(__KERNEL__) */
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>


/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
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
#define LOCALSTAMP                  0xABAB

/*!
 *  @brief  Indicates that the buffer was put into the list from remote
 *          processor
 */
#define REMOTESTAMP                 0xCDCD

/*!
 *  @brief  Shared Region ID
 */
#define APP_SHAREDREGION_ENTRY_ID   0u

/*!
 *  @brief  List node
 */
typedef struct ListMP_Node_Tag {
    ListMP_Elem elem;
    UInt32      value;
    UInt32      procId;
    UInt32 buffer [  (BUFSIZE - (sizeof (ListMP_Elem) + (2 * sizeof (UInt32))))
                   / sizeof (UInt32)];
} ListMP_Node;


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
#if !defined(__KERNEL__)
extern String  ListMPApp_filePath [];
extern ProcMgr_BootMode  ListMPApp_bootMode;
extern UInt32            ListMPApp_resetVector[];
#else/* if !defined(__KERNEL__) */
ProcMgr_BootMode         ListMPApp_bootMode = ProcMgr_BootMode_Boot;
UInt32                   ListMPApp_resetVector[SysLinkSamples_MAXPROCESSORS] = {0};
#endif /* if !defined(__KERNEL__) */
extern UInt16  ListMPApp_numProcs;
extern UInt16  ListMPApp_procId   [];
extern Bool    ListMPApp_runAll;


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Thread entry point
 *
 *  @sa
 */
void ListMPApp_threadHandler (Ptr arg)
{
    Int                 status = 0;
    ListMP_Handle       ListMPApp_handleRemote = NULL;
    ListMP_Handle       ListMPApp_handleLocal = NULL;
    Bool                done   = FALSE;
    UInt32              cleanupCode = 0;
    Ptr                 ListMPApp_listMpHeapHandle;
    ListMP_Params       listMPParamsLocal;
    ListMP_Node *       node;
    UInt16              localProcId;
    UInt16              procId;
    Char                tempStr [LISTMPAPP_NAMELENGTH];

    Osal_printf ("Entered ListMPApp_threadHandler\n");

    localProcId = MultiProc_self ();
    procId = (UInt16) ((UInt32) arg);

    ListMPApp_listMpHeapHandle = SharedRegion_getHeap (
                                                    APP_SHAREDREGION_ENTRY_ID);

    if (ListMPApp_listMpHeapHandle == NULL) {
        status = ListMP_E_FAIL;
        Osal_printf ("Error in SharedRegion_getHeap\n");
    }
    else {
        Osal_printf ("Heap in SharedRegion_getHeap : %x\n",
                     ListMPApp_listMpHeapHandle);
    }

    if (status >= 0) {
        Memory_set  (tempStr, 0, LISTMPAPP_NAMELENGTH);
        sprintf (tempStr, "%s_%d%d", LISTMPSLAVE_PREFIX, localProcId, procId);

        Osal_printf ("Opening the list %s created by remote processor\n",
                     tempStr);
        do {
            status = ListMP_open (tempStr,
                                  &ListMPApp_handleRemote);
            /* Sleep for a while before trying again. */
            if (status == ListMP_E_NOTFOUND) {
#if defined(__KERNEL__)
                OsalThread_sleep (10);
#else /* if defined(__KERNEL__) */
                usleep (10);
#endif /* if defined(__KERNEL__) */
            }
        }
        while (status == ListMP_E_NOTFOUND);
        if (status < 0){
            Osal_printf ("Error in ListMP_open. Status [0x%x]\n", status);
        }
        else {
            cleanupCode = 1;
            Osal_printf ("ListMP_open Status [0x%x]\n", status);
        }
    }

    if (status >= 0) {
        ListMP_Params_init (&listMPParamsLocal);
        Memory_set  (tempStr, 0, LISTMPAPP_NAMELENGTH);
        sprintf (tempStr, "%s_%d%d", LISTMPHOST_PREFIX, localProcId, procId);
        listMPParamsLocal.name           = tempStr;
        listMPParamsLocal.regionId       = APP_SHAREDREGION_ENTRY_ID ;

        ListMPApp_handleLocal = ListMP_create (&listMPParamsLocal);
        if (ListMPApp_handleLocal == NULL) {
            status = ListMP_E_FAIL;
            Osal_printf ("Error in ListMP_create.\n");
        }
        else {
            cleanupCode = 2;
            Osal_printf ("ListMP_create Handle [0x%x]\n",
                         ListMPApp_handleLocal);
        }
    }

    if (status >= 0) {
        /* Get and free buffer from locally created list. */
        Osal_printf ("Verifying ListMP_getHead on locally  created list ...\n");
        do {
            node = (ListMP_Node *) ListMP_getHead (ListMPApp_handleLocal);

            /* Sleep for a while if the element is not yet available. */
            if (node == NULL) {
#if defined(__KERNEL__)
                OsalThread_sleep (10);
#else /* if defined(__KERNEL__) */
                usleep (10);
#endif /* if defined(__KERNEL__) */
            }
        } while (node == NULL);

        Osal_printf ("Got the node 0x%x\n",node);

        /* Verify data integrity */
        if ((node->procId != procId) || (node->value != REMOTESTAMP)) {
            status = ListMP_E_FAIL;
            Osal_printf ("Data Integrity Error!!!\n"
                    "   node->procId: Expected [%d] Received [%d]\n"
                    "   node->value:  Expected [0x%x] Received [0x%x]\n",
                    procId, node->procId, REMOTESTAMP, node->value);
        }

        Memory_free((IHeap_Handle)ListMPApp_listMpHeapHandle, node,
                sizeof(ListMP_Node));

        /* ---------------------------------------------------------------------
         * Allocate and put buffer in remotely created list.
         * ---------------------------------------------------------------------
         */
        if (status >= 0) {
            node = (ListMP_Node *)Memory_alloc(
                (IHeap_Handle)ListMPApp_listMpHeapHandle, sizeof(ListMP_Node),
                0, NULL);
            if (node == NULL) {
                status = ListMP_E_FAIL;
                Osal_printf ("Error in Memory_alloc. node [0x%x]\n", node);
            }
            else {
                Osal_printf ("Memory_alloc Buffer [0x%x]\n", node);
                node->procId = localProcId;
                node->value = LOCALSTAMP;

                if (SharedRegion_isCacheEnabled(
                          SharedRegion_getId((Ptr)node)) ) {
                    Cache_wbInv ((Ptr)node, sizeof(ListMP_Node),
                                 Cache_Type_ALL, TRUE);
                }

                Osal_printf ("Verifying ListMP_putTail on remotely"
                             " created list ...\n");
                status = ListMP_putTail ((ListMP_Handle) ListMPApp_handleRemote,
                                         &(node->elem));
                if (status < 0){
                    Osal_printf ("Error in ListMP_putTail - Remote."
                                 " Status [0x%x]\n",
                                 status);
                }
                else {
                    Osal_printf ("ListMP_putTail - Remote. Status [0x%x]\n",
                                 status);
                }
            }
        }

        /* -------------------------------------------------------------------------
         * Allocate and put two buffers in locally created list.
         * -------------------------------------------------------------------------
         */
        if (status >= 0) {
            node = (ListMP_Node *) Memory_alloc (
                                       (IHeap_Handle)ListMPApp_listMpHeapHandle,
                                        sizeof (ListMP_Node),
                                        0,
                                        NULL);
            if (node == NULL) {
                status = ListMP_E_FAIL;
                Osal_printf ("Error in Memory_alloc. node [0x%x]\n", node);
            }
            else {
                Osal_printf ("Memory_alloc Buffer [0x%x]\n", node);
                node->procId = localProcId;
                node->value = LOCALSTAMP;

                if (SharedRegion_isCacheEnabled(
                          SharedRegion_getId((Ptr)node)) ) {
                    Cache_wbInv ((Ptr)node, sizeof(ListMP_Node),
                                 Cache_Type_ALL, TRUE);
                }

                Osal_printf ("Verifying ListMP_putTail on locally"
                             " created list ...\n");
                status = ListMP_putTail ((ListMP_Handle) ListMPApp_handleLocal,
                                         &(node->elem));
                if (status < 0){
                    Osal_printf ("Error in ListMP_putTail - Local."
                                 " Status [0x%x]\n",
                                 status);
                }
                else {
                    Osal_printf ("ListMP_putTail - Local. Status [0x%x]\n",
                                 status);
                }
            }

            if (status >= 0) {
                node = (ListMP_Node *) Memory_alloc (
                                        (IHeap_Handle)ListMPApp_listMpHeapHandle,
                                        sizeof (ListMP_Node),
                                        0,
                                        NULL);
                if (node == NULL) {
                    status = ListMP_E_FAIL;
                    Osal_printf ("Error in Memory_alloc. node [0x%x]\n", node);
                }
                else {
                    Osal_printf ("Memory_alloc Buffer [0x%x]\n", node);
                    node->procId = localProcId;
                    node->value = LOCALSTAMP;

                    if (SharedRegion_isCacheEnabled(
                              SharedRegion_getId((Ptr)node)) ) {
                        Cache_wbInv ((Ptr)node, sizeof(ListMP_Node),
                                     Cache_Type_ALL, TRUE);
                    }

                    Osal_printf ("Verifying ListMP_putTail on locally"
                                 " created list ...\n");
                    status = ListMP_putTail (
                                          (ListMP_Handle) ListMPApp_handleLocal,
                                          &(node->elem));
                    if (status < 0){
                        Osal_printf ("Error in ListMP_putTail - Local."
                                     " Status [0x%x]\n",
                                     status);
                    }
                    else {
                        Osal_printf ("ListMP_putTail - Local. Status [0x%x]\n",
                                     status);
                    }
                }
            }
        }

        /* ---------------------------------------------------------------------
         * Get and free two buffers from remotely created list.
         * ---------------------------------------------------------------------
         */
        if (status >= 0) {
            /* Wait till ListMP_next returns a buffer from remote processor. */
            Osal_printf ("Wait till remote processor removes the "
                         "earlier element ...\n");
            do {
                node = (ListMP_Node *) ListMP_next (ListMPApp_handleRemote,
                                                    NULL);
                if (node == NULL) {
                    /* Sleep for a while if the element is not yet available. */
#if defined(__KERNEL__)
                    OsalThread_sleep (10);
#else /* if defined(__KERNEL__) */
                    usleep (10);
#endif /* if defined(__KERNEL__) */
                }
                else {
                    if (SharedRegion_isCacheEnabled(
                              SharedRegion_getId((Ptr)node)) ) {
                        Cache_inv ((Ptr)node, sizeof(ListMP_Node),
                                     Cache_Type_ALL, TRUE);
                    }

                    if (    (node->procId == localProcId)
                        ||  (node->value  == LOCALSTAMP)){
                        /* Sleep for a while if the other proc has still not
                         * removed the element we put in. */
#if defined(__KERNEL__)
                        OsalThread_sleep (10);
#else /* if defined(__KERNEL__) */
                        usleep (10);
#endif /* if defined(__KERNEL__) */
                    }
                    else {
                        done = TRUE;
                    }
                }
            }
            while (done == FALSE);

            /* Verifying ListMP_getTail */
            Osal_printf ("Verifying ListMP_getTail on remotely created list "
                         "...\n");
            do {
                node = (ListMP_Node *) ListMP_getTail (ListMPApp_handleRemote);
                /* Sleep for a while if the element is not yet available. */
                if (node == NULL) {
#if defined(__KERNEL__)
                    OsalThread_sleep (10);
#else /* if defined(__KERNEL__) */
                    usleep (10);
#endif /* if defined(__KERNEL__) */
                }
            }
            while (node == NULL);
            Osal_printf ("ListMP_getTail - Remote. node [0x%x]\n", node);

            if (SharedRegion_isCacheEnabled(
                      SharedRegion_getId((Ptr)node)) ) {
                Cache_inv ((Ptr)node, sizeof(ListMP_Node),
                           Cache_Type_ALL, TRUE);
            }

            /* Verify data integrity */
            if (    (node->procId != procId)
                ||  (node->value != REMOTESTAMP)){
                status = ListMP_E_FAIL;
                Osal_printf ("Data Integrity Error!!!\n"
                             "   node->procId: Expected [%d] Received [%d]\n"
                             "   node->value:  Expected [0x%x] Received [0x%x]\n",
                             procId,
                             node->procId,
                             REMOTESTAMP,
                             node->value);
            }

            Memory_free ((IHeap_Handle)ListMPApp_listMpHeapHandle,
                         node,
                         sizeof (ListMP_Node));
        }

        if (status >= 0) {
            Osal_printf ("Verifying ListMP_getHead on remotely created"
                         " list ...\n");
            do {
                node = (ListMP_Node *) ListMP_getHead (ListMPApp_handleRemote);
                /* Sleep for a while if the element is not yet available. */
                if (node == NULL) {
#if defined(__KERNEL__)
                    OsalThread_sleep (10);
#else /* if defined(__KERNEL__) */
                    usleep (10);
#endif /* if defined(__KERNEL__) */
                }
            }
            while (node == NULL);
            Osal_printf ("ListMP_getHead - Remote. node [0x%x]\n", node);

            if (SharedRegion_isCacheEnabled(
                      SharedRegion_getId((Ptr)node)) ) {
                Cache_inv ((Ptr)node, sizeof(ListMP_Node),
                           Cache_Type_ALL, TRUE);
            }

            /* Verify data integrity */
            if (    (node->procId != procId)
                ||  (node->value != REMOTESTAMP)){
                status = ListMP_E_FAIL;
                Osal_printf ("Data Integrity Error!!!\n"
                           "   node->procId: Expected [%d] Received [%d]\n"
                           "   node->value:  Expected [0x%x] Received [0x%x]\n",
                           procId,
                           node->procId,
                           REMOTESTAMP,
                           node->value);
            }

            Memory_free ((IHeap_Handle)ListMPApp_listMpHeapHandle,
                         node,
                         sizeof (ListMP_Node));
        }
    }

    switch (cleanupCode) {
        case 2:
        {
            status = ListMP_delete (&ListMPApp_handleLocal);
            if (status < 0) {
                Osal_printf ("ListMP_delete failed. Status [0x%xx]\n", status);
            }
        }

        case 1:
        {
            status = ListMP_close (&ListMPApp_handleRemote);
            if (status < 0) {
                Osal_printf ("ListMP_close failed. Status [0x%xx]\n", status);
            }
        }
        break;
    }
}

/*!
 *  @brief  Function to execute the startup for ListMPApp sample application
 *
 *  @sa
 */
Int ListMPApp_startup(String mapFile)
{
    Int status = 0;
    UInt16 i = 0;
    Ptr arg;
    
    Osal_printf ("Entered ListMPApp_startup\n");

    /* Set the slaves to run the application with. */
    status = SysLinkSamples_setToRunProcIds(ListMPApp_numProcs,
            ListMPApp_procId, ListMPApp_runAll);

#if !defined(__KERNEL__)
    for (i = 0; (i < ListMPApp_numProcs) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(ListMPApp_procId[i])) {
            if (ListMPApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Loading and starting procId [%d] with [%s]\n",
                        ListMPApp_procId[i], ListMPApp_filePath[i]);
                status = SlaveLoader_startup(ListMPApp_procId[i],
                        "startup", ListMPApp_filePath[i], mapFile);
                Osal_printf("SlaveLoader_startup status [%d]\n", status);
            }
        }
    }
#endif

    for (i = 0; (i < MultiProc_getNumProcessors ()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
            if (ListMPApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL;
            }
            else {
                arg = (Ptr)&ListMPApp_resetVector[i];
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
                Osal_printf ("After Ipc_loadcallback for proc %d:\n"
                             "    status [0x%x]\n",
                             i,
                             status);
            }
            if (status >= 0) {
                status = Ipc_control (i, Ipc_CONTROLCMD_STARTCALLBACK, NULL);
                if (status < 0) {
                    Osal_printf ("Error in Ipc_control "
                            "Ipc_CONTROLCMD_STARTCALLBACK[0x%x]\n", status);
                }
                else {
                    Osal_printf ("After Ipc_startcallback for proc %d: \n"
                                 "    status [0x%x]\n", i, status);
                }
            }
        }
    }

    Osal_printf("Leaving ListMPApp_startup 0x%x\n", status);

    return (status);
}


/*!
 *  @brief  Function to execute the ListMPApp sample application
 *
 *  @sa
 */
Int ListMPApp_execute (Void)
{
    Int32  status = 0;
    UInt32 i;

    Osal_printf ("Entered ListMPApp_execute\n");

    /* Create threads */
    for (i = 0; i < MultiProc_getNumProcessors (); i++) {
        if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
            ListMPApp_threadHandler ((Ptr) i);
        }
    }

    Osal_printf ("Leaving ListMPApp_execute\n");

    return (status);
}


/*!
 *  @brief  Function to execute the shutdown for ListMPApp sample application
 *
 *  @sa
 */
Int ListMPApp_shutdown (String mapFile)
{
    Int32  status = 0;
    UInt16 i = 0;
    Ptr arg;
    
    Osal_printf("Entered ListMPApp_shutdown\n");
    for (i = 0; i < MultiProc_getNumProcessors(); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (ListMPApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL; 
            }
            else {
                arg = (Ptr)&ListMPApp_resetVector[i];
            }
            /* Call Ipc_control with resetVector for 
             * ProcMgr_BootMode_NoLoad_Pwr,
             * ProcMgr_BootMode_NoLoad_NoPwr
             * and ProcMgr_BootMode_NoBoot modes
             */
            status = Ipc_control(i, 
                Ipc_CONTROLCMD_STOPCALLBACK, arg);
            Osal_printf ("After Ipc_stopCallback status: [0x%x]\n", status);
        }
    }

#if !defined(__KERNEL__)
    for (i = 0; i < ListMPApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(ListMPApp_procId[i])) {
            if (ListMPApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Shutting down procId [%d]\n", ListMPApp_procId[i]);
                status = SlaveLoader_shutdown(ListMPApp_procId[i], "shutdown",
                        mapFile);
                Osal_printf("SlaveLoader_shutdown status [%d]\n", status);
            }
        }
    }
#endif /* if !defined(__KERNEL__) */

    Osal_printf("Leaving ListMPApp_shutdown\n");

    return (status);
}
