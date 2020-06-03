/*
 *  @file   MessageQApp.c
 *
 *  @brief      MessageQ application.
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
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/IHeap.h>

/*  ----------------------------------- IPC module Headers           */
#include <ti/ipc/GateMP.h>
#include <ti/ipc/Ipc.h>
#include <ti/sdo/ipc/Ipc.h>
#include <ti/ipc/MessageQ.h>
#include <ti/ipc/HeapMemMP.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>

/*  ----------------------------------- BIOS6 module Headers         */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/hal/Cache.h>
/*  ----------------------------------- To get globals from .cfg Header */
#include <xdc/cfg/global.h>


/* App info tag ID */
#define APP_INFO_TAG        0xBABA0000

#define APPNOTIFY_EVENT_NO                11u

/* key to generate data pattern for messages */
#define PATTERNKEY               1u

/*Functions declarations*/
Int32
MessageQApp_writePattern(Ptr bufPtr, UInt32 offset,UInt32 bufSize, Int32 key);
Int32
MessageQApp_readPattern(Ptr bufPtr, UInt32 offset,UInt32 bufSize, Int32 key);

/* App info struct passed to HOST */
typedef struct App_Info {
    Char   lMsgqName [80];
    Char   rMsgqName [80];
    Char   heapName  [80];
    UInt32 heapAlign;
    UInt32 heapMsgSize;
    UInt32 heapNumMsg;
    UInt32 heapId;
    UInt32 numLoops;
} App_Info;

Semaphore_Handle   semHandle1 ;

Void APPNotify_callbackFxn (UInt16 procId,UInt16 lineId, UInt32 eventNo,
           UArg arg, UInt32 payload)
{
    Semaphore_post((Semaphore_Object*)arg);
}

/*
 *  ======== tsk1_func ========
 *  Send and receive messages
 */
Void tsk1_func(UArg arg0, UArg arg1)
{
    UInt16           rProcId = (UInt16) arg0;
    Int              status  = 0;
    UInt32           msgId = 0;
    Char             tStr [80u];
    MessageQ_Msg     msg;
    MessageQ_Handle  messageQ;
    MessageQ_QueueId remoteQueueId;
    HeapMemMP_Handle heapHandle;
    UInt16           lProcId;
    App_Info         aInfo;
    Semaphore_Params        semParams;
    Error_Block             eb;

    Error_init (&eb) ;

    lProcId = MultiProc_self ();

    /* Connect to remote processor */
    do {
        status = Ipc_attach (rProcId);
    } while (status < 0);

    /* Write the app info, This info is only read by HOST as it does not have
     * static configuration feature as provided by XDC.
     */
    memset  (tStr, 0, 80);
    System_sprintf (tStr, "%s%d%d", MSGQ_NAME, lProcId, rProcId);
    strcpy (aInfo.lMsgqName, tStr);
    memset  (tStr, 0, 80);
    System_sprintf (tStr, "%s%d%d", MSGQ_NAME, rProcId, lProcId);
    strcpy (aInfo.rMsgqName, tStr);
    System_sprintf (tStr, "%s%d%d", HEAP_NAME, rProcId, lProcId);
    strcpy (aInfo.heapName, tStr);
    aInfo.heapMsgSize = HEAP_MSGSIZE;
    aInfo.heapNumMsg  = HEAP_NUMMSGS;
    aInfo.heapAlign   = HEAP_ALIGN;
    aInfo.heapId      = HEAPID;
    aInfo.numLoops    = NUMLOOPS;

    status = Ipc_writeConfig (rProcId, APP_INFO_TAG, (Ptr) &aInfo,
                              sizeof (App_Info));

    if (status == Ipc_E_FAIL) {
        System_printf ("Failed to write Ipc config entry exiting ....%s: %d :\n",
                       __FILE__, __LINE__) ;
        return ;
    }

    Semaphore_Params_init (&semParams) ;
    semParams.mode = Semaphore_Mode_BINARY;
    semHandle1 = Semaphore_create (0, &semParams, &eb) ;
    if (semHandle1 == NULL) {
        Error_check (&eb) ;
        System_printf ("Failed to Create the semaphore exiting ....%s: %d :\n",
                       __FILE__, __LINE__) ;
        return ;
    }

    /* Create a message queue */
    messageQ = MessageQ_create(aInfo.lMsgqName, NULL);
    if (messageQ == NULL) {
        System_abort("MessageQ_create failed\n" );
    }

    Notify_registerEvent(
                     rProcId,
                     0,
                     APPNOTIFY_EVENT_NO,
                     (Notify_FnNotifyCbck) APPNotify_callbackFxn,
                     (UArg) semHandle1);

    Semaphore_pend(semHandle1, BIOS_WAIT_FOREVER) ;
    System_printf("\n******APPNotify_callbackFxn *****\n");

    System_printf ("Got App notification ....:\n") ;

    /* Open the heap created by the other processor. Loop until opened. */
    do {
        status = HeapMemMP_open(aInfo.heapName, &heapHandle);
        if (status == HeapMemMP_E_NOTFOUND) {
            /* Sleep for a while before trying again. */
            Task_sleep (10);
        }
    }
    while (status != HeapMemMP_S_SUCCESS);

    /* Register this heap with MessageQ */
    MessageQ_registerHeap((IHeap_Handle)heapHandle, HEAPID);

    /* Open the remote message queue. Spin until it is ready. */
    do {
        status = MessageQ_open(aInfo.rMsgqName, &remoteQueueId);
        if (status == MessageQ_E_NOTFOUND) {
            /* Sleep for a while before trying again. */
            Task_sleep (10);
        }
    }
    while (status != MessageQ_S_SUCCESS);

    if (rProcId < lProcId) {
        /* Ping-pong the same message around the processors */
        msg = MessageQ_alloc(HEAPID, HEAP_MSGSIZE);
        if (msg == NULL) {
           System_abort("MessageQ_alloc failed\n" );
        }
    }

    /* Wait for a message from videom3 and send it back to videom3 */
    System_printf("Start the main loop\n");
    while (TRUE) {
        if (rProcId > lProcId) {
            /* Get a message */
            status = MessageQ_get(messageQ, &msg, MessageQ_FOREVER);
            if (status != MessageQ_S_SUCCESS) {
               System_abort("This should not happen since timeout is forever\n");
            }
            /*Read the data pattern in the message to check the data integrity*/
            status = MessageQApp_readPattern(msg,
                                             sizeof(MessageQ_MsgHeader),
                                             (  MessageQ_getMsgSize(msg)
                                              - sizeof(MessageQ_MsgHeader)),
                                             lProcId+PATTERNKEY);
            if(status < 0)
            {
                System_abort ("Data integrity failed\n");
            }
            msgId = MessageQ_getMsgId(msg);
            System_printf("Sending a message #%d to %d\n",
                          msgId,
                          rProcId);


            /*Write data pattern to the message to check the data integrity on receiver side*/
            status = MessageQApp_writePattern (msg,
                                               sizeof(MessageQ_MsgHeader),
                                               (  MessageQ_getMsgSize(msg)
                                                - sizeof(MessageQ_MsgHeader)),
                                               rProcId+PATTERNKEY);
            if(status < 0)
            {
                    System_abort  ("Writing data pattern to message failed\n");
            }


            status = MessageQ_put(remoteQueueId, msg);
            if (status != MessageQ_S_SUCCESS) {
               System_abort("MessageQ_put had a failure/error\n");
            }
        }
        else {
            msgId++;
            MessageQ_setMsgId(msg, msgId);
            /*Write data pattern to the message to check the data integrity on receiver side*/
            status = MessageQApp_writePattern (msg,
                                               sizeof(MessageQ_MsgHeader),
                                               (  MessageQ_getMsgSize(msg)
                                                - sizeof(MessageQ_MsgHeader)),
                                               rProcId+PATTERNKEY);
            if(status < 0)
            {
                    System_abort  ("Writing data pattern to message failed\n");
            }
            System_printf("Sending a message #%d to %d\n",
                          MessageQ_getMsgId(msg),
                          rProcId);
            status = MessageQ_put(remoteQueueId, msg);
            if (status != MessageQ_S_SUCCESS) {
               System_abort("MessageQ_put had a failure/error\n");
            }

            /* Get a message */
            status = MessageQ_get(messageQ, &msg, MessageQ_FOREVER);
            if (status != MessageQ_S_SUCCESS) {
               System_abort("This should not happen since timeout is forever\n");
            }
            status = MessageQApp_readPattern(msg,
                                             sizeof(MessageQ_MsgHeader),
                                             (  MessageQ_getMsgSize(msg)
                                              - sizeof(MessageQ_MsgHeader)),
                                             lProcId+PATTERNKEY);
            if(status < 0)
            {
                   System_abort ("Data integrity failed\n");
            }
        }

        /* test done */
        if (msgId >= NUMLOOPS) {
            break;
        }
    }

    /* Cleanup. */
    if (rProcId < lProcId) {
        status = MessageQ_free (msg);
        if (status < 0) {
            System_abort("MessageQ_free failed\n" );
        }
    }

    status = MessageQ_delete (&messageQ);
    if (status < 0) {
        System_abort("MessageQ_delete failed\n" );
    }

    status = MessageQ_close (&remoteQueueId);
    if (status < 0) {
        System_abort("MessageQ_close failed\n" );
    }

    MessageQ_unregisterHeap (HEAPID);
    status = HeapMemMP_close (&heapHandle);
    if (status < 0) {
        System_abort("HeapMemMP_close failed\n" );
    }

    /* Spin until Ipc_detach success then stop */
    do {
        status = Ipc_detach(rProcId);
    } while (status < 0);

    Ipc_stop();

    System_printf("The test is complete\n");
}


/*!
 *  @brief  Function to write an integer pattern to given buffer
 *  @param bufPtr  buffer to which the pattern has to be written
 *  @param offset  index to startr writing.the pattern
 *  @param bufSize size of the buffer
 *  @param key integer size by which each element in the pattern should differ
 *  @sa
 */
Int32
MessageQApp_writePattern(Ptr bufPtr, UInt32 offset,UInt32 bufSize, Int32 key)
{
    Int32  status    = 0;
    UInt32 charCount = 0;
    UInt8 * buffer   = NULL;
    UInt8 pattern    = 0;

    buffer           = (UInt8 *) bufPtr;

    if(bufPtr == NULL)
    {
        status = MessageQ_E_INVALIDARG;
        System_printf ("Null buffer pointer passed\n");
    }
    else {
        for (charCount = offset; charCount < bufSize-2; charCount++)
        {
            pattern += key;
            buffer[charCount] = pattern;
        }
        buffer[bufSize] = '\0';
        Cache_wbInv((Ptr)buffer,
                    bufSize,
                    Cache_Type_ALL,
                    TRUE);
    }
    return(status);
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
Int32
MessageQApp_readPattern(Ptr bufPtr, UInt32 offset,UInt32 bufSize, Int32 key)
{
    Int32  status           = 0;
    UInt32 charCount    = 0;
    UInt8 * buffer            = NULL;
    UInt8 pattern = 0;
    buffer = (UInt8 *) bufPtr;

    if(bufPtr == NULL)
    {
        status = MessageQ_E_INVALIDARG;
        System_printf ("Null buffer pointer passed\n");

    }
    else {
        Cache_inv((Ptr)buffer, bufSize , Cache_Type_ALL, TRUE);
        for (charCount = offset; charCount < bufSize-2; charCount++)
        {
            pattern += key;

            if(buffer[charCount] != pattern)
            {
                    status = MessageQ_E_FAIL;
                     System_printf ("Pattern does not match\n");
                    break;
            }
        }
    }
    return(status);
}



/*
 *  ======== main ========
 */
Int main(Int argc, Char* argv[])
{
    Int status = Ipc_S_SUCCESS;

    do {
        /* Call Ipc_start() */
        status = Ipc_start();
    } while (status != Ipc_S_SUCCESS);

    BIOS_start();
    return (0);
}

