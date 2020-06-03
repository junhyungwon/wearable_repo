/*
 *  @file   MessageQDrv.c
 *
 *  @brief      OS-specific implementation of MessageQ driver for Linux
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



/*  Defined to include MACROS EXPORT_SYMBOL. This must be done before including
 *  module.h
 */
#if !defined (EXPORT_SYMTAB)
#define EXPORT_SYMTAB
#endif

/* OS-specific headers */
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <asm/io.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>

/* Standard headers */
#include <ti/syslink/Std.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/List.h>
#include <OsalDriver.h>
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/syslink/utils/Gate.h>

/* Module specific header files */
#include <ti/ipc/MessageQ.h>
#include <TransportShm.h>
#include <_MessageQ.h>
#include <MessageQDrv.h>
#include <MessageQDrvDefs.h>
#include <ti/ipc/SharedRegion.h>
#include <_SharedRegion.h>


/** ============================================================================
 *  Export kernel utils functions
 *  ============================================================================
 */
/* MessageQ functions */
EXPORT_SYMBOL(MessageQ_getConfig);
EXPORT_SYMBOL(MessageQ_setup);
EXPORT_SYMBOL(MessageQ_destroy);
EXPORT_SYMBOL(MessageQ_Params_init);
EXPORT_SYMBOL(MessageQ_create);
EXPORT_SYMBOL(MessageQ_delete);
EXPORT_SYMBOL(MessageQ_open);
EXPORT_SYMBOL(MessageQ_close);
EXPORT_SYMBOL(MessageQ_get);
EXPORT_SYMBOL(MessageQ_count);
EXPORT_SYMBOL(MessageQ_free);
EXPORT_SYMBOL(MessageQ_alloc);
EXPORT_SYMBOL(MessageQ_put);
EXPORT_SYMBOL(MessageQ_unblock);
EXPORT_SYMBOL(MessageQ_registerHeap);
EXPORT_SYMBOL(MessageQ_unregisterHeap);
EXPORT_SYMBOL(MessageQ_registerTransport);
EXPORT_SYMBOL(MessageQ_unregisterTransport);
EXPORT_SYMBOL(MessageQ_setReplyQueue);
EXPORT_SYMBOL(MessageQ_getQueueId);

/* MessageQDrv functions */
EXPORT_SYMBOL(MessageQDrv_registerDriver);
EXPORT_SYMBOL(MessageQDrv_unregisterDriver);


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/*!
 *  @brief  Driver minor number for MessageQ.
 */
#define MESSAGEQ_DRV_MINOR_NUMBER 9


/** ============================================================================
 *  Forward declarations of internal functions
 *  ============================================================================
 */
/*!
 *  @brief  Linux driver function to open the driver object.
 */
static int MessageQDrv_open (struct inode * inode, struct file * filp);

/*!
 *  @brief  Linux driver function to close the driver object.
 */
static int MessageQDrv_close (struct inode * inode, struct file * filp);

/*!
 *  @brief  Linux driver function to ioctl of the driver object.
 */
static long MessageQDrv_ioctl (struct file *  filp,
                               unsigned int   cmd,
                               unsigned long  args);


#if defined (SYSLINK_MULTIPLE_MODULES)
/*!
 *  @brief  Module initialization function for Linux driver.
 */
static int __init MessageQDrv_initializeModule (void);

/*!
 *  @brief  Module finalization  function for Linux driver.
 */
static void  __exit MessageQDrv_finalizeModule (void);
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


/** ============================================================================
 *  Globals
 *  ============================================================================
 */

/*!
 *  @brief  Function to invoke the APIs through ioctl.
 */
static struct file_operations MessageQDrv_driverOps = {
    open:    MessageQDrv_open,
    release: MessageQDrv_close,
    unlocked_ioctl:   MessageQDrv_ioctl,
} ;


#if defined (SYSLINK_MULTIPLE_MODULES)
/*!
 *  @brief  Indicates whether trace should be enabled.
 */
static Char * TRACE = FALSE;
module_param (TRACE, charp, S_IRUGO);
Bool MessageQDrv_enableTrace = FALSE;

/*!
 *  @brief  Indicates whether entry/leave trace should be enabled.
 */
static Char * TRACEENTER = FALSE;
module_param (TRACEENTER, charp, S_IRUGO);
Bool MessageQDrv_enableTraceEnter = FALSE;

/*!
 *  @brief  Indicates whether SetFailureReason trace should be enabled.
 */
static Char * TRACEFAILURE = FALSE;
module_param (TRACEFAILURE, charp, S_IRUGO);
Bool MessageQDrv_enableTraceFailure = FALSE;

/*!
 *  @brief  Indicates class of trace to be enabled
 */
static Char * TRACECLASS = NULL;
module_param (TRACECLASS, charp, S_IRUGO);
UInt32 MessageQDrv_traceClass = 0;

/*!
 *  @brief  OsalDriver handle for MessageQ
 */
static Ptr MessageQDrv_osalDrvHandle = NULL;
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Register the MessageQ with OsalDriver
 */
Ptr
MessageQDrv_registerDriver (Void)
{
    OsalDriver_Handle osalHandle;

    GT_0trace (curTrace, GT_ENTER, "MessageQDrv_registerDriver");

    osalHandle = OsalDriver_registerDriver ("MessageQ",
                                            &MessageQDrv_driverOps,
                                            MESSAGEQ_DRV_MINOR_NUMBER);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (osalHandle == NULL) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "MessageQDrv_registerDriver",
                             MessageQ_E_INVALIDARG,
                             "OsalDriver_registerDriver failed!");
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "MessageQDrv_registerDriver",
               osalHandle);

    return (Ptr) osalHandle;
}


/*!
 *  @brief  Register the MessageQ with OsalDriver
 */
Void
MessageQDrv_unregisterDriver (Ptr * drvHandle)
{
    GT_1trace (curTrace, GT_ENTER, "MessageQDrv_unregisterDriver",
               drvHandle);

    OsalDriver_unregisterDriver ((OsalDriver_Handle *) drvHandle);

    GT_0trace (curTrace, GT_LEAVE, "MessageQDrv_unregisterDriver");
}


/*!
 *  @brief  Linux specific function to open the driver.
 */
int
MessageQDrv_open (struct inode * inode, struct file * filp)
{
    OsalDriver_Handle handle = container_of (inode->i_cdev,
                                             OsalDriver_Object,
                                             cdev);
    filp->private_data = handle;

    return 0;
}

/*!
 *  @brief  Linux specific function to close the driver.
 */
int
MessageQDrv_close (struct inode * inode, struct file * filp)
{
    return 0;
}

/*!
 *  @brief  Linux specific function to close the driver.
 */
static
long MessageQDrv_ioctl (struct file *  filp,
                        unsigned int   cmd,
                        unsigned long  args)
{
    int                   osStatus = 0;
    MessageQDrv_CmdArgs * dstArgs  = (MessageQDrv_CmdArgs *) args;
    Int32                 status   = MessageQ_S_SUCCESS;
    Int32                 ret;
    MessageQDrv_CmdArgs   cargs;

    GT_3trace (curTrace, GT_ENTER, "MessageQDrv_ioctl",
               filp, cmd, args);

    /* Copy the full args from user-side */
    ret = copy_from_user (&cargs,
                          dstArgs,
                          sizeof (MessageQDrv_CmdArgs));
    GT_assert (curTrace, (ret == 0));

    switch (cmd) {
        case CMD_MESSAGEQ_PUT:
        {
            MessageQ_Msg msg;
            msg = SharedRegion_getPtr (cargs.args.put.msgSrPtr);
            status = MessageQ_put (cargs.args.put.queueId,
                                   msg);
        }
        break;

        case CMD_MESSAGEQ_GET:
        {
            MessageQ_Msg        msg      = NULL;
            SharedRegion_SRPtr  msgSrPtr = SharedRegion_INVALIDSRPTR;
            UInt16               index;
            status = MessageQ_get (cargs.args.get.handle,
                                   &msg,
                                   cargs.args.get.timeout);
            if (status >= 0) {
                index    = SharedRegion_getId (msg);
                msgSrPtr = SharedRegion_getSRPtr (msg, index);
            }

            cargs.args.get.msgSrPtr = msgSrPtr;
        }
        break;

        case CMD_MESSAGEQ_COUNT:
        {
            cargs.args.count.count = MessageQ_count (cargs.args.count.handle);
        }
        break;

        case CMD_MESSAGEQ_ALLOC:
        {
            MessageQ_Msg        msg;
            SharedRegion_SRPtr  msgSrPtr = SharedRegion_INVALIDSRPTR;
            UInt16               index;

            msg = MessageQ_alloc (cargs.args.alloc.heapId,
                                  cargs.args.alloc.size);
            if (msg != NULL) {
                index = SharedRegion_getId (msg);
                msgSrPtr = SharedRegion_getSRPtr (msg, index);
            }

            cargs.args.alloc.msgSrPtr = msgSrPtr;
        }
        break;

        case CMD_MESSAGEQ_FREE:
        {
            MessageQ_Msg msg;
            msg = SharedRegion_getPtr (cargs.args.free.msgSrPtr);
            status = MessageQ_free (msg);
        }
        break;

        case CMD_MESSAGEQ_PARAMS_INIT :
        {
            MessageQ_Params params;

            MessageQ_Params_init (&params);
            GT_assert (curTrace, (status >= 0));

            ret = copy_to_user (cargs.args.ParamsInit.params,
                                &params,
                                sizeof (MessageQ_Params));
            GT_assert (curTrace, (ret == 0));
        }
        break;

        case CMD_MESSAGEQ_CREATE:
        {
            String          name = NULL;
            MessageQ_Params params;

            if (cargs.args.create.params != NULL) {
                ret = copy_from_user (&params,
                                      cargs.args.create.params,
                                      sizeof (MessageQ_Params));
                GT_assert (curTrace, (ret == 0));
            }

            /* Allocate memory for the name */
            if (cargs.args.create.nameLen > 0) {
                name = Memory_alloc (NULL,
                                     cargs.args.create.nameLen,
                                     0,
                                     NULL);
                GT_assert (curTrace, (name != NULL));
                /* Copy the name */
                ret = copy_from_user (name,
                                      cargs.args.create.name,
                                      cargs.args.create.nameLen);
                GT_assert (curTrace, (ret == 0));
            }

            if (cargs.args.create.params != NULL) {
                cargs.args.create.handle = MessageQ_create (name, &params);
            }
            else {
                cargs.args.create.handle = MessageQ_create (name, NULL);
            }
            GT_assert (curTrace, (cargs.args.create.handle != NULL));

            /* Set failure status if create has failed. */
            if (cargs.args.create.handle == NULL) {
                status = MessageQ_E_FAIL;
            }

            if (cargs.args.create.handle != NULL) {
                cargs.args.create.queueId = MessageQ_getQueueId (
                                                     cargs.args.create.handle);
            }
            if (cargs.args.create.nameLen > 0) {
                Memory_free (NULL, name, cargs.args.create.nameLen);
            }
        }
        break;

        case CMD_MESSAGEQ_DELETE:
        {
            status = MessageQ_delete ((MessageQ_Handle *)
                                            &cargs.args.deleteMessageQ.handle);
            GT_assert (curTrace, (status >= 0));
        }
        break;

        case CMD_MESSAGEQ_OPEN:
        {
            String name    = NULL;
            MessageQ_QueueId queueId = MessageQ_INVALIDMESSAGEQ;

            /* Allocate memory for the name */
            if (cargs.args.open.nameLen > 0) {
                name = Memory_alloc (NULL,
                                     cargs.args.open.nameLen,
                                     0,
                                     NULL);
                GT_assert (curTrace, (name != NULL));

                ret = copy_from_user (name,
                                      cargs.args.open.name,
                                      cargs.args.open.nameLen);
                GT_assert (curTrace, (ret == 0));
            }

            status = MessageQ_open (name, &queueId);
            GT_1trace (curTrace,
                       GT_2CLASS,
                       "    MessageQ_open ioctl queueId [0x%x]",
                       queueId);

            cargs.args.open.queueId = queueId;
            if (cargs.args.open.nameLen > 0) {
                Memory_free (NULL, name, cargs.args.open.nameLen);
            }
        }
        break;

        case CMD_MESSAGEQ_CLOSE:
        {
            MessageQ_QueueId queueId = cargs.args.close.queueId;
            MessageQ_close (&queueId);
            cargs.args.close.queueId = queueId;
        }
        break;

        case CMD_MESSAGEQ_UNBLOCK:
        {
            MessageQ_unblock (cargs.args.unblock.handle);
        }
        break;

        case CMD_MESSAGEQ_GETCONFIG:
        {
            MessageQ_Config config;

            MessageQ_getConfig (&config);

            ret = copy_to_user (cargs.args.getConfig.config,
                                &config,
                                sizeof (MessageQ_Config));
            GT_assert (curTrace, (ret == 0));
        }
        break;

        case CMD_MESSAGEQ_SETUP:
        {
            MessageQ_Config config;

            ret = copy_from_user (&config,
                                  cargs.args.setup.config,
                                  sizeof (MessageQ_Config));
            GT_assert (curTrace, (ret == 0));

            status = MessageQ_setup (&config);
            GT_assert (curTrace, (status >= 0));
        }
        break;

        case CMD_MESSAGEQ_DESTROY:
        {
            status = MessageQ_destroy ();
            GT_assert (curTrace, (status >= 0));
        }
        break;

        case CMD_MESSAGEQ_REGISTERHEAP:
        {
            status = MessageQ_registerHeap (cargs.args.registerHeap.heap,
                                            cargs.args.registerHeap.heapId);
        }
        break;

        case CMD_MESSAGEQ_UNREGISTERHEAP:
        {
            status = MessageQ_unregisterHeap(cargs.args.unregisterHeap.heapId);
        }
        break;

        case CMD_MESSAGEQ_SHAREDMEMREQ:
        {
            Ptr sharedAddr;
            sharedAddr = SharedRegion_getPtr (
                                    cargs.args.sharedMemReq.sharedAddrSrPtr);
            cargs.args.sharedMemReq.memReq = MessageQ_sharedMemReq (sharedAddr);
        }
        break;

        case CMD_MESSAGEQ_ATTACH:
        {
            Ptr sharedAddr;
            sharedAddr = SharedRegion_getPtr (
                                            cargs.args.attach.sharedAddrSrPtr);
            status = MessageQ_attach (cargs.args.attach.remoteProcId,
                                      sharedAddr);
        }
        break;

        case CMD_MESSAGEQ_DETACH:
        {
            status = MessageQ_detach (cargs.args.attach.remoteProcId);
        }
        break;

        default:
        {
            /* This does not impact return status of this function, so retVal
             * comment is not used.
             */
            status = MessageQ_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "MessageQDrv_ioctl",
                                 status,
                                 "Unsupported ioctl command specified");
        }
        break;
    }

    cargs.apiStatus = status;

    /* Copy the full args to the user-side. */
    ret = copy_to_user (dstArgs,
                        &cargs,
                        sizeof (MessageQDrv_CmdArgs));
    GT_assert (curTrace, (ret == 0));

    GT_1trace (curTrace, GT_LEAVE, "MessageQDrv_ioctl",
               osStatus);

    /*! @retval 0 Operation successfully completed. */
    return osStatus;
}


/** ============================================================================
 *  Functions required for multiple .ko modules configuration
 *  ============================================================================
 */
#if defined (SYSLINK_MULTIPLE_MODULES)
/*!
 *  @brief  Module initialization  function for Linux driver.
 */
static
int __init MessageQDrv_initializeModule (Void)
{
    int result = 0;

    /* Display the version info and created date/time */
    Osal_printf ("MessageQ sample module created on Date:%s Time:%s\n",
                 __DATE__,
                 __TIME__);

    Osal_printf ("MessageQDrv_initializeModule\n");

    /* Enable/disable levels of tracing. */
    if (TRACE != NULL) {
        Osal_printf ("Trace enable %s\n", TRACE) ;
        MessageQDrv_enableTrace = simple_strtol (TRACE, NULL, 16);
        if ((MessageQDrv_enableTrace != 0) && (MessageQDrv_enableTrace != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACE\n") ;
        }
        else if (MessageQDrv_enableTrace == TRUE) {
            curTrace = GT_TraceState_Enable;
        }
        else if (MessageQDrv_enableTrace == FALSE) {
            curTrace = GT_TraceState_Disable;
        }
    }

    if (TRACEENTER != NULL) {
        Osal_printf ("Trace entry/leave prints enable %s\n", TRACEENTER) ;
        MessageQDrv_enableTraceEnter = simple_strtol (TRACEENTER, NULL, 16);
        if (    (MessageQDrv_enableTraceEnter != 0)
            &&  (MessageQDrv_enableTraceEnter != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (MessageQDrv_enableTraceEnter == TRUE) {
            curTrace |= GT_TraceEnter_Enable;
        }
    }

    if (TRACEFAILURE != NULL) {
        Osal_printf ("Trace SetFailureReason enable %s\n", TRACEFAILURE) ;
        MessageQDrv_enableTraceFailure = simple_strtol (TRACEFAILURE, NULL, 16);
        if (    (MessageQDrv_enableTraceFailure != 0)
            &&  (MessageQDrv_enableTraceFailure != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (MessageQDrv_enableTraceFailure == TRUE) {
            curTrace |= GT_TraceSetFailure_Enable;
        }
    }

    if (TRACECLASS != NULL) {
        Osal_printf ("Trace class %s\n", TRACECLASS) ;
        MessageQDrv_traceClass = simple_strtol (TRACECLASS, NULL, 16);
        if (    (MessageQDrv_enableTraceFailure != 1)
            &&  (MessageQDrv_enableTraceFailure != 2)
            &&  (MessageQDrv_enableTraceFailure != 3)) {
            Osal_printf ("Error! Only 1/2/3 supported for TRACECLASS\n") ;
        }
        else {
            MessageQDrv_traceClass =
                           MessageQDrv_traceClass << (32 - GT_TRACECLASS_SHIFT);
            curTrace |= MessageQDrv_traceClass;
        }
    }

    Osal_printf ("curTrace value: 0x%x\n", curTrace);

    /* Initialize the OsalDriver */
    OsalDriver_setup ();

    MessageQDrv_osalDrvHandle = MessageQDrv_registerDriver ();
    if (MessageQDrv_osalDrvHandle == NULL) {
        /*! @retval MessageQ_E_OSFAILURE Failed to register MessageQ
                                        driver with OS! */
        result = -EFAULT;
        GT_setFailureReason (curTrace,
                          GT_4CLASS,
                          "MessageQDrv_initializeModule",
                          MessageQ_E_OSFAILURE,
                          "Failed to register MessageQ driver with OS!");
    }

    Osal_printf ("MessageQDrv_initializeModule 0x%x\n", result);

    return result;
}


/*!
 *  @brief  Linux driver function to finalize the driver module.
 */
static
void __exit MessageQDrv_finalizeModule (void)
{
    Osal_printf ("MessageQDrv_finalizeModule\n");
    Osal_printf ("Entered MessageQDrv_finalizeModule\n");

    MessageQDrv_unregisterDriver (&(MessageQDrv_osalDrvHandle));

    /* Finalize the OsalDriver */
    OsalDriver_destroy ();

    Osal_printf ("Leaving MessageQDrv_finalizeModule\n");
}


/*!
 *  @brief  Macro calls that indicate initialization and finalization functions
 *          to the kernel.
 */
MODULE_LICENSE ("GPL v2");
module_init (MessageQDrv_initializeModule);
module_exit (MessageQDrv_finalizeModule);
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
