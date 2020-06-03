/*
 *  @file   MessageQApp.c
 *
 *  @brief      Sample application for MessageQ module
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
#include <ti/ipc/HeapMemMP.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/MessageQ.h>
#include <ti/ipc/Notify.h>

/* Ipc header */
#include <ti/ipc/Ipc.h>
#include <ti/syslink/IpcHost.h>

#if !defined(__KERNEL__)
#include <ti/syslink/samples/hlos/slaveLoader/SlaveLoader.h>
#else
#include <OsalThread.h>
#endif /* if !defined(__KERNEL__) */
#include <ti/syslink/samples/hlos/common/SysLinkSamples.h>


/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/* App info tag ID */
#define APP_INFO_TAG        0xBABA0000
#define APPNOTIFY_EVENT_NO                11u
/* key to generate data pattern for messages */
#define PATTERNKEY               1u
/*!
 *  @brief  Shared Region ID
 */
#define APP_SHAREDREGION_ENTRY_ID   0u

/** ============================================================================
 *  Structures
 *  ============================================================================
 */
/* App info struct passed to HOST */
typedef struct App_Info {
    Char   rMsgqName [80];
    Char   lMsgqName [80];
    Char   heapName  [80];
    UInt32 heapAlign;
    UInt32 heapMsgSize;
    UInt32 heapNumMsg;
    UInt32 heapId;
    UInt32 numLoops;
} App_Info;


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
#if !defined(__KERNEL__)
extern String  MessageQApp_filePath [];
extern ProcMgr_BootMode  MessageQApp_bootMode;
extern UInt32            MessageQApp_resetVector[];
#else/* if !defined(__KERNEL__) */
ProcMgr_BootMode         MessageQApp_bootMode = ProcMgr_BootMode_Boot;
UInt32                   MessageQApp_resetVector[SysLinkSamples_MAXPROCESSORS] = {0};
#endif /* if !defined(__KERNEL__) */
extern UInt16  MessageQApp_numProcs;
extern UInt16  MessageQApp_procId   [];
extern Bool    MessageQApp_runAll;


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
Int32 MessageQApp_writePattern(Ptr bufPtr, UInt32 offset, UInt32 bufSize,
        Int32 key);
Int32 MessageQApp_readPattern(Ptr bufPtr, UInt32 offset, UInt32 bufSize,
        Int32 key);


/*!
 *  @brief  Thread entry point
 *
 *  @sa
 */
void MessageQApp_threadHandler (Ptr arg)
{
    UInt16              rProcId = (UInt16) ((UInt32)arg);
    Int                 status  = 0;
    UInt32              msgId = 0;
    MessageQ_Msg        msg    = NULL;
    MessageQ_Handle     messageQ = NULL;
    UInt32              cleanupCode = 0;
    IHeap_Handle        srHeap = NULL;
    Ptr                 ptr = NULL;
    SizeT               heapSize = 0;
    MessageQ_QueueId    remoteQueueId;
    HeapMemMP_Handle    heapHandle;
    UInt16              lProcId;
    App_Info            aInfo;
    HeapMemMP_Params    heapMemParams;

    Osal_printf("MessageQApp_threadHandler entered\n");

    lProcId = MultiProc_self();

    /* Read the config info written by the slave. */
    do {
        status = Ipc_readConfig(rProcId, APP_INFO_TAG, &aInfo,
                sizeof(App_Info));
    } while (status == Ipc_E_FAIL);

    /* Create Heap and register it with MessageQ */
    if (status >= 0) {
        HeapMemMP_Params_init(&heapMemParams);
        heapMemParams.sharedAddr = NULL;
        heapSize = (HeapMemMP_sharedMemReq(&heapMemParams) +
                (aInfo.heapNumMsg * aInfo.heapMsgSize));
        srHeap = SharedRegion_getHeap(APP_SHAREDREGION_ENTRY_ID);
        if (srHeap == NULL) {
            status = MessageQ_E_FAIL;
            Osal_printf("SharedRegion_getHeap failed for %d processor."
                    " srHeap: [0x%x]\n", rProcId, srHeap);
        }
        else {
            ptr = Memory_alloc(srHeap, heapSize, 0, NULL);
            if (ptr == NULL) {
                status = MessageQ_E_FAIL;
                Osal_printf ("Memory_alloc failed for %d processor."
                    " ptr: [0x%x]\n", rProcId, ptr);
            }
            else {
                cleanupCode = 1;
                heapMemParams.name = aInfo.heapName;
                heapMemParams.sharedAddr = ptr;
                heapMemParams.sharedBufSize = aInfo.heapNumMsg *
                        aInfo.heapMsgSize;
                heapHandle = HeapMemMP_create(&heapMemParams);
                if (heapHandle == NULL) {
                    status = MessageQ_E_FAIL;
                    Osal_printf("HeapMemMP_create failed for %d processor."
                            " Handle: [0x%x]\n", rProcId, heapHandle);
                }
                else {
                    cleanupCode = 2;
                }
            }
        }
    }

    if (status >= 0) {
        /* Register this heap */
        Osal_printf("Registering heapId %d with MessageQ for procId: %d\n",
                aInfo.heapId, rProcId);
        MessageQ_registerHeap(heapHandle, aInfo.heapId);
    }

    if (status >= 0) {
        /* Construct the messageQ name for the master */
        messageQ = MessageQ_create(aInfo.lMsgqName, NULL);
        if (messageQ == NULL) {
            status = MessageQ_E_FAIL;
            Osal_printf("Error in MessageQ_create [0x%x] : procId [%d]\n",
                    status, rProcId);
        }
        else {
            cleanupCode = 4;
            Osal_printf("MessageQ_create name %s  status [0x%x] : procId [%d]\n",
                    aInfo.lMsgqName, status, rProcId);
        }
    }

    if (status >= 0) {
        Osal_printf("Sending synchronizaion notification to ProcId: %d\n",
                rProcId);
        Notify_sendEvent(rProcId, 0, APPNOTIFY_EVENT_NO, 0, TRUE);
        Osal_printf("Sent synchronizaion notification to ProcId: %d\n", rProcId);

        /* Construct the messageQ name for the slave */
        do {
            status = MessageQ_open(aInfo.rMsgqName, &remoteQueueId);
#if defined(__KERNEL__)
            OsalThread_sleep(10);
#else
            usleep(10000);
#endif
        } while (status == MessageQ_E_NOTFOUND);

        if (status < 0) {
            Osal_printf("Error in MessageQ_open [0x%x] : procId [%d]\n",
                    status, rProcId);
        }
        else {
            cleanupCode = 3;
            Osal_printf("MessageQ_open Status [0x%x] : procId [%d]\n",
                    status, rProcId);
            Osal_printf("MessageQApp_queueId  [0x%x] : procId [%d]\n",
                    remoteQueueId, rProcId);
        }
    }

    if (status >= 0) {
        if (rProcId < lProcId) {
            /* Ping-pong the same message around the processors */
            msg = MessageQ_alloc(aInfo.heapId, aInfo.heapMsgSize);
            if (msg == NULL) {
                Osal_printf ("Error in MessageQ_alloc\n");
                status = MessageQ_E_FAIL;
            }
            else {
                cleanupCode = 5;
            }
        }
    }

    if (status >= 0) {
        while (TRUE) {
            if (rProcId > lProcId) {
                /* Get a message */
                status = MessageQ_get(messageQ, &msg, MessageQ_FOREVER);
                if (status != MessageQ_S_SUCCESS) {
                   Osal_printf("This should not happen since timeout is "
                           "forever\n");
                   status = MessageQ_E_FAIL;
                   break;
                }
                /* Validate the data pattern in the message */
                status = MessageQApp_readPattern(msg,
                        sizeof(MessageQ_MsgHeader),
                        (MessageQ_getMsgSize(msg) -
                        sizeof(MessageQ_MsgHeader)),
                        lProcId + PATTERNKEY);
                if (status < 0) {
                       Osal_printf ("Data integrity failed\n");
                       break;
                }
                msgId = MessageQ_getMsgId(msg);
                if ((msgId != 0) && ((msgId % 100) == 0)) {
                    Osal_printf("Sending a message #%d to %d\n",
                            MessageQ_getMsgId(msg), rProcId);
                }
                /*Write data pattern to the message to check the data integrity on receiver side*/
                status = MessageQApp_writePattern(msg,
                                                   sizeof(MessageQ_MsgHeader),
                                                 (  MessageQ_getMsgSize(msg)
                                                  - sizeof(MessageQ_MsgHeader)),
                                                   rProcId+PATTERNKEY);
                if (status < 0) {
                    Osal_printf("Writing data pattern to message failed\n");
                    break;
                }
                status = MessageQ_put(remoteQueueId, msg);
                if (status != MessageQ_S_SUCCESS) {
                   Osal_printf("MessageQ_put had a failure/error\n");
                   status = MessageQ_E_FAIL;
                   break;
                }
            }
            else {
                msgId++;
                MessageQ_setMsgId(msg, msgId);

                /*Write data pattern to the message to check the data integrity on receiver side*/
                status = MessageQApp_writePattern(msg,
                        sizeof(MessageQ_MsgHeader),
                        (MessageQ_getMsgSize(msg) -
                        sizeof(MessageQ_MsgHeader)),
                        rProcId + PATTERNKEY);
                if (status < 0) {
                    Osal_printf("Writing data pattern to message failed\n");
                    break;
                }
                if ((msgId != 0) && ((msgId % 100) == 0)) {
                    Osal_printf("Sending a message #%d to %d\n",
                            MessageQ_getMsgId(msg), rProcId);
                }
                status = MessageQ_put(remoteQueueId, msg);
                if (status != MessageQ_S_SUCCESS) {
                   Osal_printf ("MessageQ_put had a failure/error\n");
                   status = MessageQ_E_FAIL;
                   break;
                }

                /* Get a message */
                status = MessageQ_get(messageQ, &msg, MessageQ_FOREVER);
                if (status != MessageQ_S_SUCCESS) {
                   Osal_printf ("This should not happen since timeout is"
                           " forever\n");
                   status = MessageQ_E_FAIL;
                   break;
                }
                else {
                    /*Read the data pattern in the message to check the data integrity*/
                    status = MessageQApp_readPattern(msg,
                            sizeof(MessageQ_MsgHeader),
                            (MessageQ_getMsgSize(msg)
                            - sizeof(MessageQ_MsgHeader)),
                            lProcId + PATTERNKEY);
                }
                if(status < 0) {
                    Osal_printf("Data integrity failed\n");
                    break;
                }
            }

            /* test done */
            if (msgId >= aInfo.numLoops) {
                break;
            }
        }
    }

    switch (cleanupCode) {
        case 5:
        {
            if (rProcId < lProcId) {
                status = MessageQ_free(msg);
                if (status < 0) {
                    Osal_printf("MessageQ_free failed. Status [0x%xx]\n",
                            status);
                }
            }
        }

        case 4:
        {
            status = MessageQ_delete(&messageQ);
            if (status < 0) {
                Osal_printf("MessageQ_delete failed. Status [0x%xx]\n",
                        status);
            }
        }

        case 3:
        {
            status = MessageQ_close(&remoteQueueId);
            if (status < 0) {
                Osal_printf("MessageQ_close failed. Status [0x%xx]\n",
                        status);
            }
        }

        case 2:
        {
            MessageQ_unregisterHeap(aInfo.heapId);
            status = HeapMemMP_delete(&heapHandle);
            if (status < 0) {
                Osal_printf("HeapMemMP_delete failed. Status [0x%xx]\n",
                        status);
            }
        }

        case 1:
        {
            Memory_free(srHeap, ptr, heapSize);
        }
        break;
    }

    Osal_printf("Leaving MessageQApp_threadHandler %x\n", status);
}


/*!
 *  @brief  Function to execute the startup for MessageQApp sample application
 *
 *  @sa
 */
Int MessageQApp_startup(String mapFile)
{
    Int status = 0;
    UInt16 i = 0;
    Ptr arg;
    
    Osal_printf("Entered MessageQApp_startup\n");

    /* Set the slaves to run the application with. */
    status = SysLinkSamples_setToRunProcIds(MessageQApp_numProcs,
            MessageQApp_procId, MessageQApp_runAll);

#if !defined(__KERNEL__)
    for (i = 0; (i < MessageQApp_numProcs) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(MessageQApp_procId[i])) {
            if (MessageQApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Loading and starting procId [%d] with [%s]\n",
                        MessageQApp_procId[i], MessageQApp_filePath[i]);
                status = SlaveLoader_startup(MessageQApp_procId[i],
                        "startup", MessageQApp_filePath[i], mapFile);
                Osal_printf("SlaveLoader_startup status [%d]\n", status);
            }
        }
    }
#endif

    for (i = 0; (i < MultiProc_getNumProcessors()) && (status >= 0); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (MessageQApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL;
            }
            else {
                arg = (Ptr)&MessageQApp_resetVector[i];
            }
            /* Call Ipc_control with reset vector for 
             * ProcMgr_BootMode_NoLoad_Pwr,
             * ProcMgr_BootMode_NoLoad_NoPwr
             * and ProcMgr_BootMode_NoBoot modes
             */
            status = Ipc_control(i, 
                Ipc_CONTROLCMD_LOADCALLBACK, arg);

            if (status < 0) {
                Osal_printf("Error in Ipc_control "
                        "Ipc_CONTROLCMD_LOADCALLBACK [0x%x]\n", status);
            }
            else {
                Osal_printf ("After Ipc_loadcallback:\n"
                        "    status [0x%x]\n", status);
            }
            if (status >= 0) {
                status = Ipc_control(i, Ipc_CONTROLCMD_STARTCALLBACK, NULL);
                if (status < 0) {
                    Osal_printf ("Error in Ipc_control "
                            "Ipc_CONTROLCMD_STARTCALLBACK[0x%x]\n", status);
                }
                else {
                    Osal_printf ("After Ipc_startcallback: \n"
                            "    status [0x%x]\n", status);
                }
            }
        }
    }

    Osal_printf("Leaving MessageQApp_startup %x\n", status);

    return (status);
}


/*!
 *  @brief  Function to execute the MessageQApp sample application
 *
 *  @sa
 */
Int MessageQApp_execute(Void)
{
    Int32  status = 0;
    UInt32 i;

    Osal_printf("Entered MessageQApp_execute\n");

    /* Run for all supported processors. */
    for (i = 0; i < MultiProc_getNumProcessors (); i++) {
        if (SysLinkSamples_toRunProcId (i) && (i != MultiProc_self ())) {
            /* Execute the logic one by one for each processor */
            MessageQApp_threadHandler((Ptr) i);
        }
    }

    Osal_printf("Leaving MessageQApp_execute\n");

    return (status);
}


/*!
 *  @brief  Function to execute the shutdown for MessageQApp sample application
 *
 *  @sa
 */
Int MessageQApp_shutdown(String mapFile)
{
    Int32  status = 0;
    Int16 i = 0;
    Ptr arg;
    
    Osal_printf("Entered MessageQApp_shutdown()\n");
    for (i = 0; i < MultiProc_getNumProcessors(); i++) {
        if (SysLinkSamples_toRunProcId(i) && (i != MultiProc_self())) {
            if (MessageQApp_bootMode == ProcMgr_BootMode_Boot) {
                arg = NULL; 
            }
            else {
                arg = (Ptr)&MessageQApp_resetVector[i];
            }
            /* Call Ipc_control with resetVector for 
             * ProcMgr_BootMode_NoLoad_Pwr,
             * ProcMgr_BootMode_NoLoad_NoPwr
             * and ProcMgr_BootMode_NoBoot modes
             */
            status = Ipc_control(i, 
                Ipc_CONTROLCMD_STOPCALLBACK, arg);
            Osal_printf("After Ipc_stopCallback status: [0x%x]\n", status);
        }
    }

#if !defined(__KERNEL__)
    for (i = 0; i < MessageQApp_numProcs; i++) {
        if (SysLinkSamples_toRunProcId(MessageQApp_procId[i])) {
            if (MessageQApp_bootMode == ProcMgr_BootMode_Boot) {
                Osal_printf("Shutting down procId [%d]\n",
                        MessageQApp_procId[i]);
                status = SlaveLoader_shutdown(MessageQApp_procId[i], "shutdown",
                        mapFile);
                Osal_printf("SlaveLoader_shutdown status [%d]\n", status);
            }
        }
    }
#endif

    Osal_printf("Leaving MessageQApp_shutdown() (0x%x)\n", status);

    return (status);
}


/*!
 *  @brief  Function to write an integer pattern to given buffer
 *  @param bufPtr  buffer to which the pattern has to be written
 *  @param offset  index to startr writing.the pattern
 *  @param bufSize size of the buffer
 *  @param key integer size by which each element in the pattern should differ
 *  @sa
 */
Int32 MessageQApp_writePattern(Ptr bufPtr, UInt32 offset, UInt32 bufSize, Int32 key)
{
    Int32  status= 0;
    UInt32 charCount = 0;
    UInt8 * buffer = NULL;
    UInt8 pattern = 0;
    buffer = (UInt8 *) bufPtr;

    if (bufPtr == NULL) {
        status = MessageQ_E_INVALIDARG;
        Osal_printf("Null buffer pointer passed\n");
    }
    else {
        for (charCount = offset; charCount < bufSize - 2; charCount++) {
            pattern += key;
            buffer[charCount] = pattern;
        }
        buffer[bufSize] = '\0';

        if (SharedRegion_isCacheEnabled(SharedRegion_getId((Ptr)buffer))) {
            Cache_wb((Ptr)buffer, bufSize, Cache_Type_ALL, TRUE);
        }

    }
    return (status);
}

/*!
 *  @brief  Function to read an integer pattern from given buffer and
 *             verify if it matches the requirements
 *  @param bufPtr  Pointer to buffer to read the pattern from
 *  @param offset  index to start reading from
 *  @param bufSize size of the buffer
 *  @param key integer size by which each element in the pattern should differ
 *  @sa
 */
Int32 MessageQApp_readPattern(Ptr bufPtr, UInt32 offset, UInt32 bufSize,
        Int32 key)
{
    Int32  status = 0;
    UInt32 charCount = 0;
    UInt8 * buffer = NULL;
    UInt8 pattern = 0;
    buffer = (UInt8 *) bufPtr;

    if(bufPtr == NULL) {
        status = MessageQ_E_INVALIDARG;
        Osal_printf ("Null buffer pointer passed\n");
    }
    else {
        if (SharedRegion_isCacheEnabled(SharedRegion_getId((Ptr)buffer))) {
            Cache_inv((Ptr)buffer, bufSize, Cache_Type_ALL, TRUE);
        }

        for (charCount = offset; charCount < bufSize-2; charCount++) {
            pattern += key;
            if (buffer[charCount] != pattern) {
                status = MessageQ_E_FAIL;
                Osal_printf("Pattern does not match\n");
                break;
            }
        }
    }

    return (status);
}
