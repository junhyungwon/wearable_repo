/*
 *  @file   NotifyDrv.c
 *
 *  @brief      OS-specific implementation of Notify driver for Linux
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

/* Standard headers */
#include <ti/syslink/Std.h>

/* OSAL & Utils headers */
#include <OsalDriver.h>
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalSemaphore.h>
#include <ti/syslink/utils/IGateProvider.h>
#include <ti/syslink/utils/GateSpinlock.h>
#include <ti/syslink/utils/Memory.h>

/* Linux specific header files */
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/mman.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>

/* Module headers */
#include <ti/ipc/Notify.h>
#include <NotifyDrv.h>
#include <NotifyDrvDefs.h>
#include <_NotifyDefs.h>


/** ============================================================================
 *  Export kernel utils functions
 *  ============================================================================
 */
/* Notify functions */
EXPORT_SYMBOL(Notify_getConfig);
EXPORT_SYMBOL(Notify_setup);
EXPORT_SYMBOL(Notify_destroy);
EXPORT_SYMBOL(Notify_attach);
EXPORT_SYMBOL(Notify_detach);
EXPORT_SYMBOL(Notify_registerEvent);
EXPORT_SYMBOL(Notify_registerEventSingle);
EXPORT_SYMBOL(Notify_sendEvent);
EXPORT_SYMBOL(Notify_unregisterEvent);
EXPORT_SYMBOL(Notify_unregisterEventSingle);
EXPORT_SYMBOL(Notify_disable);
EXPORT_SYMBOL(Notify_restore);
EXPORT_SYMBOL(Notify_disableEvent);
EXPORT_SYMBOL(Notify_enableEvent);
EXPORT_SYMBOL(Notify_intLineRegistered);
EXPORT_SYMBOL(Notify_numIntLines);
/* NotifyDrv functions */
EXPORT_SYMBOL(NotifyDrv_registerDriver);
EXPORT_SYMBOL(NotifyDrv_unregisterDriver);


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */

extern Notify_ModuleObject Notify_state;
/* NotifyDrv variables */
EXPORT_SYMBOL(Notify_state);


/*!
 *  @brief  Driver minor number for Notify.
 */
#define NOTIFY_DRV_MINOR_NUMBER 1u

/*!
 *  @brief  Maximum number of user processes supported.
 */
#define  MAX_PROCESSES          32u


/*!
 *  @brief  Structure of Event callback argument passed to register fucntion.
 */
typedef struct NotifyDrv_EventCbck_tag {
    List_Elem          element;
    /*!< List element header */
    UInt16             procId;
    /*!< Processor identifier */
    UInt16             lineId;
    /*!< line identifier */
    UInt32             eventId;
    /*!< Event identifier */
    Notify_FnNotifyCbck func;
    /*!< Callback function for the event. */
    Ptr                param;
    /*!< User callback argument. */
    UInt32             pid;
    /*!< Process Identifier for user process. */
} NotifyDrv_EventCbck ;
/*!
 *  @brief  Keeps the information related to Event.
 */
typedef struct NotifyDrv_EventState_tag {
    List_Handle            bufList;
    /*!< Head of received event list. */
    UInt32                 pid;
    /*!< User process ID. */
    UInt32                 refCount;
    /*!< Reference count, used when multiple Notify_registerEvent are called
         from same process space (multi threads/processes). */
    OsalSemaphore_Handle   semHandle;
    /*!< Semphore for waiting on event. */
    OsalSemaphore_Handle   terSemHandle;
    /*!< Termination synchronization semaphore. */
} NotifyDrv_EventState;

/*!
 *  @brief  NotifyDrv Module state object
 */
typedef struct NotifyDrv_ModuleObject_tag {
    Bool                 isSetup;
    /*!< Indicates whether the module has been already setup */
    Bool                 openRefCount;
    /*!< Open reference count. */
    IGateProvider_Handle gateHandle;
    /*!< Handle of gate to be used for local thread safety */
    List_Handle          eventCbckList;
    /*!< List containg callback arguments for all registered handlers from
         user mode. */
    List_Handle          singleEventCbckList;
    /*!< List containg callback arguments for all registered handlers from
         user mode for 'single' registrations. */
    NotifyDrv_EventState eventState [MAX_PROCESSES];
    /*!< List for all user processes registered. */
} NotifyDrv_ModuleObject;


/** ============================================================================
 *  Forward declarations of internal functions
 *  ============================================================================
 */
/* Linux driver function to open the driver object. */
static int NotifyDrv_open (struct inode * inode, struct file * filp);

/* Linux driver function to close the driver object. */
static int NotifyDrv_close (struct inode * inode, struct file * filp);

/* Linux driver function to read driver memory */
static int NotifyDrv_read (struct file *  filp,
                           char *         dst,
                           size_t         size,
                           loff_t *       offset);

/* Linux driver function to map memory regions to user space. */
static int NotifyDrv_mmap (struct file * filp, struct vm_area_struct * vma);

/* Linux driver function to invoke the APIs through ioctl. */
static long NotifyDrv_ioctl (struct file *     filp,
                             unsigned int      cmd,
                             unsigned long     args);

/* Attach a process to notify user support framework. */
static Int NotifyDrv_attach (UInt32 pid);

/* Detach a process from notify user support framework. */
static Int NotifyDrv_detach (UInt32 pid);

#if defined (SYSLINK_MULTIPLE_MODULES)
/* Module initialization function for Linux driver. */
static int __init NotifyDrv_initializeModule (void);

/* Module finalization  function for Linux driver. */
static void  __exit NotifyDrv_finalizeModule (void);
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


/** ============================================================================
 *  Globals
 *  ============================================================================
 */
/*!
 *  @var    Notify_state
 *
 *  @brief  Notify state object variable
 */
#if !defined(SYSLINK_BUILD_DEBUG)
static
#endif /* if !defined(SYSLINK_BUILD_DEBUG) */
NotifyDrv_ModuleObject NotifyDrv_state =
{
    .gateHandle = NULL,
    .isSetup = FALSE,
    .eventCbckList = NULL,
    .singleEventCbckList = NULL,
    .openRefCount = 0
};


/*!
 *  @brief  File operations table for NotifyDrv.
 */
static struct file_operations NotifyDrv_driverOps = {
    open :    NotifyDrv_open,
    unlocked_ioctl:    NotifyDrv_ioctl,
    release:  NotifyDrv_close,
    read:     NotifyDrv_read,
    mmap:     NotifyDrv_mmap,
} ;

#if defined (SYSLINK_MULTIPLE_MODULES)
/*!
 *  @brief  Indicates whether trace should be enabled.
 */
static Char * TRACE = FALSE;
module_param (TRACE, charp, S_IRUGO);
Bool NotifyDrv_enableTrace = FALSE;

/*!
 *  @brief  Indicates whether entry/leave trace should be enabled.
 */
static Char * TRACEENTER = FALSE;
module_param (TRACEENTER, charp, S_IRUGO);
Bool NotifyDrv_enableTraceEnter = FALSE;

/*!
 *  @brief  Indicates whether SetFailureReason trace should be enabled.
 */
static Char * TRACEFAILURE = FALSE;
module_param (TRACEFAILURE, charp, S_IRUGO);
Bool NotifyDrv_enableTraceFailure = FALSE;

/*!
 *  @brief  Indicates class of trace to be enabled
 */
static Char * TRACECLASS = NULL;
module_param (TRACECLASS, charp, S_IRUGO);
UInt32 NotifyDrv_traceClass = 0;

/*!
 *  @brief  OsalDriver handle for Notify
 */
static Ptr NotifyDrv_osalDrvHandle = NULL;
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


/** ============================================================================
 *  Forward declaration of internal functions
 *  ============================================================================
 */
/*!
 *  This function implements the callback registered with IPS. Here to pass
 *  event no. back to user function (so that it can do another level of
 *  demultiplexing of callbacks)
 */
static Void _Notify_drvCallback (UInt16 procId,
                                 UInt16 lineId,
                                 UInt32 eventId,
                                 UArg   arg,
                                 UInt32 payload);

/* This function adds a data to a registered process. */
static Int _NotifyDrv_addBufByPid (UInt16             procId,
                                   UInt16             lineId,
                                   UInt32             pid,
                                   UInt32             eventId,
                                   UInt32             data,
                                   Notify_FnNotifyCbck cbFxn,
                                   Ptr                param);

/* Module setup function. */
static Void _NotifyDrv_setup (Void);

/* Module destroy function. */
static Void _NotifyDrv_destroy (Void);


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Register the Notify with OsalDriver
 *
 *  @sa     NotifyDrv_unregisterDriver
 */
Ptr
NotifyDrv_registerDriver (Void)
{
    OsalDriver_Handle osalHandle;

    GT_0trace (curTrace, GT_ENTER, "NotifyDrv_registerDriver");

    /* Setup the module. */
    _NotifyDrv_setup ();

    osalHandle = OsalDriver_registerDriver ("Notify",
                                            &NotifyDrv_driverOps,
                                            NOTIFY_DRV_MINOR_NUMBER);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (osalHandle == NULL) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "NotifyDrv_registerDriver",
                             Notify_E_INVALIDARG,
                             "OsalDriver_registerDriver failed!");
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "NotifyDrv_registerDriver",
               osalHandle);

    return (Ptr) osalHandle;
}


/*!
 *  @brief  Unregister the Notify with OsalDriver
 *
 *  @param  drvHandle   Pointer to the driver handle to be unregistered.
 *
 *  @sa     NotifyDrv_registerDriver
 */
Void
NotifyDrv_unregisterDriver (Ptr * drvHandle)
{
    GT_1trace (curTrace, GT_ENTER, "NotifyDrv_unregisterDriver", drvHandle);

    OsalDriver_unregisterDriver ((OsalDriver_Handle *) drvHandle);

    _NotifyDrv_destroy ();

    GT_0trace (curTrace, GT_LEAVE, "NotifyDrv_unregisterDriver");
}


/*!
 *  @brief       Linux specific function to open the driver.
 *
 *  @param      inode    inode pointer
 *  @param      filep    file pointer
 *
 *  @sa         NotifyDrv_close
 */
Int
NotifyDrv_open (struct inode * inode, struct file * filp)
{
    return 0;
}


/*!
 *  @brief      Linux specific function to close the driver.
 *
 *  @param      inode    inode pointer
 *  @param      filep    file pointer
 *
 *  @sa         NotifyDrv_open
 */
Int
NotifyDrv_close (struct inode * inode, struct file * filp)
{
    return 0;
}


/*!
 *  @brief      Linux specific function to read data from the driver.
 *
 *  @param      filp    File structure pointer.
 *  @param      dst     Buffer to be filled.
 *  @param      size    Number of bytes to read.
 *  @param      offset  Offset from where to read.
 *
 *  @sa         NotifyDrv_open
 */
static
Int
NotifyDrv_read (struct file * filp, char * dst, size_t size, loff_t *offset)
{
    Int32                   status   = Notify_S_SUCCESS;
    Bool                    flag     = FALSE;
    NotifyDrv_EventPacket * uBuf     = NULL;
    UInt32                  retVal   = 0;
    NotifyDrv_EventPacket   tBuf;
    UInt32                  i;

    GT_4trace (curTrace,
               GT_ENTER,
               "NotifyDrv_read",
               filp,
               dst,
               size,
               offset) ;

    GT_assert (curTrace, (filp != NULL));
    GT_assert (curTrace, (offset != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (NotifyDrv_state.isSetup == FALSE) {
        /*! @retval 0 The Notify OS driver was not setup */
        status = Notify_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "NotifyDrv_read",
                             Notify_E_FAIL,
                             "The Notify OS driver was not setup!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        retVal = copy_from_user ((Ptr) &tBuf,
                                 (Ptr) dst,
                                  sizeof (NotifyDrv_EventPacket));
        GT_assert (curTrace, (retVal == 0));


        for (i = 0 ; i < MAX_PROCESSES ; i++) {
            if (NotifyDrv_state.eventState [i].pid == tBuf.pid) {
                flag = TRUE;
                break;
            }
        }

        /*  Let the check remain at run-time. */
        if (flag == TRUE) {
            /* Let the check remain at run-time for handling any run-time
             * race conditions.
             */
            if (NotifyDrv_state.eventState [i].bufList != NULL) {
                /* Wait for the event */
                status = OsalSemaphore_pend (
                                      NotifyDrv_state.eventState [i].semHandle,
                                      OSALSEMAPHORE_WAIT_FOREVER);
                /*  Let the check remain at run-time. */
                if (status > 0) {
                    uBuf = (NotifyDrv_EventPacket *)
                              List_get (NotifyDrv_state.eventState [i].bufList);
                    /*  Let the check remain at run-time. */
                    if (uBuf != NULL) {
                        retVal = copy_to_user ((Ptr) dst,
                                               uBuf,
                                               sizeof (NotifyDrv_EventPacket));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
                        if (retVal != 0) {
                            /*! @retval 0 copy_to_user failed */
                            GT_setFailureReason (curTrace,
                                                 GT_4CLASS,
                                                 "NotifyDrv_read",
                                                 status,
                                                 "copy_to_user failed");
                        }
                        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                            retVal = sizeof (NotifyDrv_EventPacket);
                            if (uBuf->isExit == TRUE) {
                                /* Post the semphore */
                                OsalSemaphore_post (
                                   NotifyDrv_state.eventState [i].terSemHandle);
                            }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                        /* Free the processed event callback packet. */
                        kfree (uBuf);
                    }
                    else {
                        GT_setFailureReason (curTrace,
                                             GT_4CLASS,
                                             "NotifyDrv_read",
                                             status,
                                             "List_get returned NULL");
                    }
                }
                else {
                    if (status == -ERESTARTSYS) {
                        /*! @retval -ERESTARTSYS Semaphore wait interrupted */
                        /* If semaphore wait was interrupted, propagate the
                         * status back to the driver.
                         */
                        retVal = -ERESTARTSYS;
                    }
                    else {
                        /*! @retval 0 Semaphore wait failed with error. */
                        GT_setFailureReason (curTrace,
                                           GT_4CLASS,
                                           "NotifyDrv_read",
                                           Notify_E_FAIL,
                                           "Semaphore wait failed with error!");
                    }
                }
            }
        }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "NotifyDrv_read", retVal);

    /*! @retval Number-of-bytes-read Number of bytes read. */
    return retVal;
}


/*!
 *  @brief  Linux driver function to map memory regions to user space.
 *
 *  @param  filp    File structure pointer.
 *  @param  vma     User virtual memory area structure pointer.
 *
 *  @sa     NotifyDrv_open
 */
static
int
NotifyDrv_mmap (struct file * filp, struct vm_area_struct * vma)
{
#ifdef CONFIG_MMU
    vma->vm_page_prot = pgprot_noncached (vma->vm_page_prot);
#endif

    if (remap_pfn_range (vma,
                         vma->vm_start,
                         vma->vm_pgoff,
                         vma->vm_end - vma->vm_start,
                         vma->vm_page_prot)) {
        return -EAGAIN;
    }
    return 0;
}


/*!
 *  @brief      Linux driver function to invoke the APIs through ioctl.
 *
 *  @param      inode    Iinode pointer
 *  @param      filep    File pointer
 *  @param      cmd      Command id
 *  @param      args     Command arguments
 *
 *  @sa         NotifyDrv_open
 */
static
long
NotifyDrv_ioctl (struct file *     filp,
                 unsigned int      cmd,
                 unsigned long     args)
{
    Int32                 status    = Notify_S_SUCCESS;
    Int                   osStatus  = 0;
    Int                   retVal    = 0;
    Notify_CmdArgs *      cmdArgs   = (Notify_CmdArgs *) args;
    Notify_CmdArgs        commonArgs;

    GT_3trace (curTrace,
               GT_ENTER,
               "NotifyDrv_ioctl",
               filp,
               cmd,
               args) ;

    switch (cmd) {
        case CMD_NOTIFY_SENDEVENT:
        {
            Notify_CmdArgsSendEvent srcArgs;

            /* Copy the full args from user-side. */
            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsSendEvent));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                status = Notify_sendEvent (srcArgs.procId,
                                           srcArgs.lineId,
                                           srcArgs.eventId,
                                           srcArgs.payload,
                                           srcArgs.waitClear);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                /* Notify_E_DRIVERINIT and Notify_E_NOTREADY are run-time
                 * failures.
                 */
                if (   (status < 0)
                    && (status != Notify_E_NOTINITIALIZED)
                    && (status != Notify_E_EVTNOTREGISTERED)
                    && (status != Notify_E_EVTDISABLED)) {
                    /* This does not impact return status of this function,
                     * so retVal comment is not used.
                     */
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "Kernel-side Notify_sendEvent failed");
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }

            /* copy_to_user is not needed for Notify_sendEvent, since nothing
             * is to be returned back.
             */
        }
        break;

        case CMD_NOTIFY_DISABLE:
        {
            Notify_CmdArgsDisable srcArgs;

            /* Copy the full args from user-side. */
            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsDisable));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                srcArgs.flags = Notify_disable (srcArgs.procId, srcArgs.lineId);
            }

            if (osStatus == 0) {
                /* Copy the full args to user-side */
                retVal = copy_to_user ((Ptr) (args),
                                       (const Ptr) &srcArgs,
                                       sizeof (Notify_CmdArgsDisable));
                /* This check is needed at run-time also since it depends on
                 * run environment. It must not be optimized out.
                 */
                if (retVal != 0) {
                    GT_1trace (curTrace,
                              GT_1CLASS,
                              "    NotifyDrv_ioctl: copy_to_user call failed\n"
                              "        status [%d]",
                              retVal);
                    osStatus = -EFAULT;
                }
            }
        }
        break;

        case CMD_NOTIFY_RESTORE:
        {
            Notify_CmdArgsRestore srcArgs;

            /* Copy the full args from user-side. */
            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsRestore));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                Notify_restore (srcArgs.procId, srcArgs.lineId, srcArgs.key);
            }

            /* copy_to_user is not needed for Notify_restore, since nothing
             * is to be returned back.
             */
        }
        break;

        case CMD_NOTIFY_DISABLEEVENT:
        {
            Notify_CmdArgsDisableEvent srcArgs;

            /* Copy the full args from user-side. */
            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsDisableEvent));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                Notify_disableEvent (srcArgs.procId,
                                     srcArgs.lineId,
                                     srcArgs.eventId);
            }

            /* copy_to_user is not needed for Notify_disableEvent, since nothing
             * is to be returned back.
             */
        }
        break;

        case CMD_NOTIFY_ENABLEEVENT:
        {
            Notify_CmdArgsEnableEvent srcArgs;

            /* Copy the full args from user-side. */
            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsEnableEvent));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                Notify_enableEvent (srcArgs.procId,
                                    srcArgs.lineId,
                                    srcArgs.eventId);
            }

            /* copy_to_user is not needed for Notify_enableEvent, since nothing
             * is to be returned back.
             */
        }
        break;

        case CMD_NOTIFY_REGISTEREVENTSINGLE:
        {
            Notify_CmdArgsRegisterEvent srcArgs;
            NotifyDrv_EventCbck *       cbck;

            /* Copy the full args from user-side. */
            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsRegisterEvent));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                cbck = Memory_alloc (NULL,
                                     sizeof (NotifyDrv_EventCbck),
                                     0u,
                                     NULL);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (cbck == NULL) {
                    /* This does not impact return status of this function,
                     * so retVal comment is not used.
                     */
                    status = Notify_E_MEMORY ;
                    GT_setFailureReason (curTrace,
                                        GT_4CLASS,
                                        "NotifyDrv_ioctl",
                                        status,
                                        "Memory_alloc failed for event "
                                        "registration structure");
                }
                else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                    cbck->procId = srcArgs.procId;
                    cbck->lineId = srcArgs.lineId;
                    cbck->eventId = srcArgs.eventId;
                    cbck->func   = srcArgs.fnNotifyCbck;
                    cbck->param  = srcArgs.cbckArg;
                    cbck->pid    = srcArgs.pid;
                    status = Notify_registerEventSingle (srcArgs.procId,
                                                         srcArgs.lineId,
                                                         srcArgs.eventId,
                                                        _Notify_drvCallback,
                                                        (UArg) cbck);
                    /* This check is needed at run-time also to propagate the
                     * status to user-side. This must not be optimized out.
                     */
                    if (status < 0) {
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                        /* This does not impact return status of this function,
                         * so retVal comment is not used.
                         */
                        Memory_free (NULL, cbck, sizeof (NotifyDrv_EventCbck));
                        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "NotifyDrv_ioctl",
                             status,
                             "Kernel-side Notify_registerEventSingle failed");
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                    }
                    else {
                        /* Add the cbck to the event cbck list */
                        List_elemClear (&(cbck->element));
                        List_put (NotifyDrv_state.singleEventCbckList,
                                  &(cbck->element));
                    }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }

            /* copy_to_user is not needed for Notify_registerEvent, since
             * nothing is to be returned back.
             */
        }
        break;

        case CMD_NOTIFY_REGISTEREVENT:
        {
            Notify_CmdArgsRegisterEvent srcArgs;
            NotifyDrv_EventCbck *       cbck;

            /* Copy the full args from user-side. */
            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsRegisterEvent));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                cbck = Memory_alloc (NULL,
                                     sizeof (NotifyDrv_EventCbck),
                                     0u,
                                     NULL);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (cbck == NULL) {
                    /* This does not impact return status of this function,
                     * so retVal comment is not used.
                     */
                    status = Notify_E_MEMORY ;
                    GT_setFailureReason (curTrace,
                                        GT_4CLASS,
                                        "NotifyDrv_ioctl",
                                        status,
                                        "Memory_alloc failed for event "
                                        "registration structure");
                }
                else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                    cbck->procId = srcArgs.procId;
                    cbck->lineId = srcArgs.lineId;
                    cbck->eventId = srcArgs.eventId;
                    cbck->func   = srcArgs.fnNotifyCbck;
                    cbck->param  = srcArgs.cbckArg;
                    cbck->pid    = srcArgs.pid;
                    status = Notify_registerEvent (srcArgs.procId,
                                                   srcArgs.lineId,
                                                   srcArgs.eventId,
                                                   _Notify_drvCallback,
                                                   (UArg) cbck);
                    /* This check is needed at run-time also to propagate the
                     * status to user-side. This must not be optimized out.
                     */
                    if (status < 0) {
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                        /* This does not impact return status of this function,
                         * so retVal comment is not used.
                         */
                        Memory_free (NULL, cbck, sizeof (NotifyDrv_EventCbck));
                        GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "NotifyDrv_ioctl",
                                     status,
                                     "Kernel-side Notify_registerEvent failed");
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                    }
                    else {
                        /* Add the cbck to the event cbck list */
                        List_elemClear (&(cbck->element));
                        List_put (NotifyDrv_state.eventCbckList,
                                  &(cbck->element));
                    }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }

            /* copy_to_user is not needed for Notify_registerEvent, since
             * nothing is to be returned back.
             */
        }
        break;

        case CMD_NOTIFY_UNREGISTEREVENTSINGLE:
        {
            Bool                          found = FALSE;
            UInt32                        pid;
            List_Elem *                   cbck  = NULL;
            Notify_CmdArgsUnregisterEvent srcArgs;
            IArg                          key;

            /* Copy the full args from user-side. */
            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsUnregisterEvent));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                key = IGateProvider_enter (NotifyDrv_state.gateHandle) ;
                pid = srcArgs.pid;
                List_traverse (cbck, NotifyDrv_state.singleEventCbckList) {
                    GT_assert (curTrace, (cbck != NULL));
                    if (    (((NotifyDrv_EventCbck *) cbck)->procId ==
                                                                 srcArgs.procId)
                        &&  (((NotifyDrv_EventCbck *) cbck)->lineId ==
                                                                 srcArgs.lineId)
                        &&  (((NotifyDrv_EventCbck *) cbck)->eventId ==
                                                                srcArgs.eventId)
                        &&  (((NotifyDrv_EventCbck *) cbck)->pid == pid)) {
                        found = TRUE;
                        break;
                    }
                }
                IGateProvider_leave (NotifyDrv_state.gateHandle, key);

                if (found == FALSE) {
                    /* This does not impact return status of this function,
                     * so retVal comment is not used.
                     */
                    status = Notify_E_NOTFOUND;
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "Could not find the registered event");
                }
                else {
                    status = Notify_unregisterEventSingle(srcArgs.procId,
                                                          srcArgs.lineId,
                                                          srcArgs.eventId);
                    /* This check is needed at run-time also to propagate the
                     * status to user-side. This must not be optimized out.
                     */
                    if (status < 0) {
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                        /* This does not impact return status of this function,
                         * so retVal comment is not used.
                         */
                        GT_setFailureReason (curTrace,
                                            GT_4CLASS,
                                            "NotifyDrv_ioctl",
                                            status,
                                            "Kernel-side Notify_unregisterEvent"
                                            " failed");
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                    }
                    else {
                        key = IGateProvider_enter (NotifyDrv_state.gateHandle);
                        List_remove (NotifyDrv_state.singleEventCbckList, cbck);
                        IGateProvider_leave (NotifyDrv_state.gateHandle, key);
                        Memory_free (NULL, cbck, sizeof (NotifyDrv_EventCbck));
                    }
                }
            }

            /* copy_to_user is not needed for Notify_unregisterEvent, since
             * nothing is to be returned back.
             */
        }
        break;

        case CMD_NOTIFY_UNREGISTEREVENT:
        {
            Bool                          found = FALSE;
            UInt32                        pid;
            List_Elem *                   cbck  = NULL;
            Notify_CmdArgsUnregisterEvent srcArgs;
            IArg                          key;

            /* Copy the full args from user-side. */
            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsUnregisterEvent));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                key  = IGateProvider_enter (NotifyDrv_state.gateHandle) ;
                pid = srcArgs.pid;
                List_traverse (cbck, NotifyDrv_state.eventCbckList) {
                    GT_assert (curTrace, (cbck != NULL));
                    if (    (((NotifyDrv_EventCbck *) cbck)->func ==
                                                          srcArgs.fnNotifyCbck)
                        &&  (((NotifyDrv_EventCbck *) cbck)->param ==
                                                                srcArgs.cbckArg)
                        &&  (((NotifyDrv_EventCbck *) cbck)->procId ==
                                                                 srcArgs.procId)
                        &&  (((NotifyDrv_EventCbck *) cbck)->lineId ==
                                                                 srcArgs.lineId)
                        &&  (((NotifyDrv_EventCbck *) cbck)->eventId ==
                                                                srcArgs.eventId)
                        &&  (((NotifyDrv_EventCbck *) cbck)->pid == pid)) {
                        found = TRUE;
                        break;
                    }
                }
                IGateProvider_leave (NotifyDrv_state.gateHandle, key);

                if (found == FALSE) {
                    /* This does not impact return status of this function,
                     * so retVal comment is not used.
                     */
                    status = Notify_E_NOTFOUND;
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "Could not find the registered event");
                }
                else {
                    status = Notify_unregisterEvent (srcArgs.procId,
                                                     srcArgs.lineId,
                                                     srcArgs.eventId,
                                                     _Notify_drvCallback,
                                                     (Ptr) cbck);
                    /* This check is needed at run-time also to propagate the
                     * status to user-side. This must not be optimized out.
                     */
                    if (status < 0) {
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                        /* This does not impact return status of this function,
                         * so retVal comment is not used.
                         */
                        GT_setFailureReason (curTrace,
                                            GT_4CLASS,
                                            "NotifyDrv_ioctl",
                                            status,
                                            "Kernel-side Notify_unregisterEvent"
                                            " failed");
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                    }
                    else {
                        key = IGateProvider_enter (NotifyDrv_state.gateHandle);
                        List_remove (NotifyDrv_state.eventCbckList, cbck);
                        IGateProvider_leave (NotifyDrv_state.gateHandle, key);
                        Memory_free (NULL, cbck, sizeof (NotifyDrv_EventCbck));
                    }
                }
            }

            /* copy_to_user is not needed for Notify_unregisterEvent, since
             * nothing is to be returned back.
             */
        }
        break;

        case CMD_NOTIFY_GETCONFIG:
        {
            Notify_CmdArgsGetConfig * srcArgs =
                                              (Notify_CmdArgsGetConfig *) args;
            Notify_Config             cfg;

            /* copy_from_user is not needed for Notify_getConfig, since the
             * user's config is not used.
             */
            Notify_getConfig (&cfg);

            retVal = copy_to_user ((Ptr) (srcArgs->cfg),
                                   (const Ptr) &cfg,
                                   sizeof (Notify_Config));
            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                          GT_1CLASS,
                          "    NotifyDrv_ioctl: copy_to_user call failed\n"
                          "        status [%d]",
                          retVal);
                osStatus = -EFAULT;
            }
        }
        break;

        case CMD_NOTIFY_SETUP:
        {
            Notify_CmdArgsSetup * srcArgs = (Notify_CmdArgsSetup *) args;
            Notify_Config         cfg;

            retVal = copy_from_user ((Ptr) &cfg,
                                     (const Ptr) (srcArgs->cfg),
                                     sizeof (Notify_Config));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                status = Notify_setup (&cfg);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (status < 0) {
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "Kernel-side Notify_setup failed");
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }

            /* copy_to_user is not needed for Notify_setup, since the config
             * is not to be returned back.
             */
        }
        break;

        case CMD_NOTIFY_DESTROY:
        {
            /* copy_from_user is not needed for Notify_getConfig, since the
             * user's config is not used.
             */
            status = Notify_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (status < 0) {
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "NotifyDrv_ioctl",
                                     status,
                                     "Kernel-side Notify_destroy failed");
            }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

            /* copy_to_user is not needed for Notify_destroy */
        }
        break;
        case CMD_NOTIFY_ATTACH:
        {
            Notify_CmdArgsAttach srcArgs;
            Ptr                  knlsharedAddr;

            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsAttach));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {

                knlsharedAddr = Memory_translate (srcArgs.sharedAddr,
                                           Memory_XltFlags_Phys2Virt);

                status = Notify_attach (srcArgs.procId,
                                        knlsharedAddr);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (status < 0) {
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "Kernel-side Notify_attach failed");
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }

            /* copy_to_user is not needed for Notify_setup, since the config
             * is not to be returned back.
             */

        }
        break;
        case CMD_NOTIFY_DETACH:
        {
            Notify_CmdArgsDetach srcArgs;

            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsDetach));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                status = Notify_detach (srcArgs.procId);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (status < 0) {
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "Kernel-side Notify_detach failed");
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }

            /* copy_to_user is not needed for Notify_setup, since the config
             * is not to be returned back.
             */
        }
        break;
        case CMD_NOTIFY_SHAREDMEMREQ:
        {
            Notify_CmdArgsSharedMemReq srcArgs;
            Ptr                        knlsharedAddr;

            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsSharedMemReq));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {

                knlsharedAddr  = Memory_translate (srcArgs.sharedAddr,
                                           Memory_XltFlags_Phys2Virt);
                status = Notify_sharedMemReq (srcArgs.procId,
                                              knlsharedAddr);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (status < 0) {
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "Kernel-side Notify_sharedMemReq failed");
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }

            /* copy_to_user is not needed for Notify_setup, since the config
             * is not to be returned back.
             */
        }
        break;

        case CMD_NOTIFY_INTLINEREGISTERED:
        {
            Notify_CmdArgsIntLineRegistered srcArgs;

            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsIntLineRegistered));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                srcArgs.isRegistered = Notify_intLineRegistered (srcArgs.procId,
                                                           srcArgs.lineId);
                retVal = copy_to_user ((Ptr) args,
                                       &srcArgs,
                                       sizeof (Notify_CmdArgsIntLineRegistered));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (retVal != 0) {
                    /*! @retval 0 copy_to_user failed */
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "copy_to_user failed");
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }
        }
        break;

        case CMD_NOTIFY_EVENTAVAILABLE:
        {
            Notify_CmdArgsEventAvailable srcArgs;

            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsEventAvailable));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                srcArgs.available = Notify_eventAvailable (srcArgs.procId,
                                                           srcArgs.lineId,
                                                           srcArgs.eventId);
                retVal = copy_to_user ((Ptr) args,
                                       &srcArgs,
                                       sizeof (Notify_CmdArgsEventAvailable));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (retVal != 0) {
                    /*! @retval 0 copy_to_user failed */
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "copy_to_user failed");
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }
        }
        break;

        case CMD_NOTIFY_THREADATTACH:
        {
            Notify_CmdArgsThreadAttach srcArgs;

            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsThreadAttach));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                status = NotifyDrv_attach (srcArgs.pid);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (status < 0) {
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "Kernel-side NotifyDrv_attach failed");
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }
        }
        break;

        case CMD_NOTIFY_THREADDETACH:
        {
            Notify_CmdArgsThreadDetach srcArgs;

            retVal = copy_from_user ((Ptr) &srcArgs,
                                     (const Ptr) (args),
                                     sizeof (Notify_CmdArgsThreadDetach));

            /* This check is needed at run-time also since it depends on
             * run environment. It must not be optimized out.
             */
            if (retVal != 0) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "    NotifyDrv_ioctl: copy_from_user call failed\n"
                           "        status [%d]",
                           retVal);
                osStatus = -EFAULT;
            }
            else {
                status = NotifyDrv_detach (srcArgs.pid);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (status < 0) {
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "NotifyDrv_ioctl",
                                         status,
                                         "Kernel-side NotifyDrv_detach failed");
                }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            }
        }
        break;

        default:
        {
            /* This does not impact return status of this function, so retVal
             * comment is not used.
             */
            status = Notify_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "NotifyDrv_ioctl",
                                 status,
                                 "Unsupported ioctl command specified");
        }
        break;
    }

    /* Set the status and copy the common args to user-side. */
    commonArgs.apiStatus = status;
    retVal = copy_to_user ((Ptr) cmdArgs,
                           (const Ptr) &commonArgs,
                           sizeof (Notify_CmdArgs));

    if (retVal != 0) {
        GT_1trace (curTrace,
                   GT_1CLASS,
                   "    NotifyDrv_ioctl: copy_from_user call failed\n"
                   "        status [%d]",
                   retVal);
        osStatus = -EFAULT;
    }
    else {
        if (status == -ERESTARTSYS) {
            /*! @retval -ERESTARTSYS Call was interrupted */
            osStatus = -ERESTARTSYS;
        }
    }

    GT_1trace (curTrace, GT_LEAVE, "NotifyDrv_ioctl", osStatus);

    /*! @retval 0 Operation successfully completed. */
    return osStatus;
}


/** ============================================================================
 *  Internal functions
 *  ============================================================================
 */
/*!
 *  @brief      This function implements the callback registered with IPS. Here
 *              to pass event no. back to user function (so that it can do
 *              another level of demultiplexing of callbacks)
 *
 *  @param      procId    processor Id from which interrupt is received
 *  @param      lineId    Interrupt line ID to be used
 *  @param      eventId   eventId registered
 *  @param      arg       argument to call back
 *  @param      payload   payload received
 *
 *  @sa
 */
static
Void
_Notify_drvCallback (UInt16 procId,
                     UInt16 lineId,
                     UInt32 eventId,
                     UArg   arg,
                     UInt32 payload)
{
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    Int32                   status = 0;
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    NotifyDrv_EventCbck *   cbck;

    GT_4trace (curTrace,
               GT_ENTER,
               "_Notify_drvCallback",
               procId,
               eventId,
               arg,
               payload);

    GT_assert (curTrace, (NotifyDrv_state.isSetup == TRUE));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (NotifyDrv_state.isSetup == FALSE) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "_Notify_drvCallback",
                             Notify_E_FAIL,
                             "The Notify OS driver was not setup!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        cbck = (NotifyDrv_EventCbck *) arg;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        status =
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        _NotifyDrv_addBufByPid (procId,
                                lineId,
                                cbck->pid,
                                eventId,
                                payload,
                                cbck->func,
                                cbck->param);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Notify_drvCallback",
                                 Notify_E_MEMORY,
                                 "Failed to add callback packet for pid");
        }
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_0trace (curTrace, GT_LEAVE, "_Notify_drvCallback");
}


/*!
 *  @brief      This function adds a data to a registered process.
 *
 *  @param      pid       Process ID of current process
 *  @param      eventId   eventId registered
 *  @param      data      Data to be added
 *  @param      cbFxn     Callback function
 *  @param      param     Parameter associated with callback function.
 *
 *  @sa
 */
static
Int
_NotifyDrv_addBufByPid (UInt16             procId,
                        UInt16             lineId,
                        UInt32             pid,
                        UInt32             eventId,
                        UInt32             data,
                        Notify_FnNotifyCbck cbFxn,
                        Ptr                param)
{
    Int32                   status = 0;
    Bool                    flag   = FALSE;
    Bool                    isExit = FALSE;
    NotifyDrv_EventPacket * uBuf   = NULL;
    IArg                    key;
    UInt32                  i;

    GT_5trace (curTrace,
               GT_ENTER,
               "_NotifyDrv_addBufByPid",
               procId,
               pid,
               eventId,
               data,
               cbFxn);

    GT_assert (curTrace, (NotifyDrv_state.isSetup == TRUE));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (NotifyDrv_state.isSetup == FALSE) {
        /*! @retval Notify_E_FAIL The Notify OS driver was not setup */
        status = Notify_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "_NotifyDrv_addBufByPid",
                             status,
                             "The Notify OS driver was not setup!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        key = IGateProvider_enter (NotifyDrv_state.gateHandle);
        /* Find the registration for this callback */
        for (i = 0 ; i < MAX_PROCESSES ; i++) {
            if (NotifyDrv_state.eventState [i].pid == pid) {
                flag = TRUE;
                break;
            }
        }
        IGateProvider_leave (NotifyDrv_state.gateHandle, key);

#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (flag != TRUE) {
            /*! @retval Notify_E_NOTFOUND Could not find a registered handler
                                          for this process. */
            status = Notify_E_NOTFOUND;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_NotifyDrv_addBufByPid",
                                 status,
                                 "Could not find a registered handler "
                                 "for this process.!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            uBuf = (NotifyDrv_EventPacket *)
                        kmalloc (sizeof (NotifyDrv_EventPacket), GFP_ATOMIC);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (uBuf == NULL) {
                /*! @retval Notify_E_MEMORY Failed to allocate memory for event
                                    packet for received callback. */
                status = Notify_E_MEMORY;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "_NotifyDrv_addBufByPid",
                                     status,
                                     "Failed to allocate memory for event"
                                     " packet for received callback.!");
            }
            else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                List_elemClear (&(uBuf->element));
                GT_assert (curTrace,
                           (NotifyDrv_state.eventState [i].bufList != NULL));

                uBuf->procId  = procId;
                uBuf->lineId  = lineId;
                uBuf->data    = data;
                uBuf->eventId = eventId;
                uBuf->func    = cbFxn;
                uBuf->param   = param;
                uBuf->isExit  = FALSE;
                if (uBuf->eventId == (UInt32) -1) {
                    uBuf->isExit = TRUE;
                    isExit = TRUE;
                }

                List_put (NotifyDrv_state.eventState [i].bufList,
                          &(uBuf->element));
                /* Post the semphore */
                OsalSemaphore_post (
                                NotifyDrv_state.eventState [i].semHandle);
                /* Termination packet */
                if (isExit == TRUE) {
                    GT_0trace (curTrace,
                               GT_1CLASS,
                               "    Notify: Termination packet\n");
                    OsalSemaphore_pend (
                              NotifyDrv_state.eventState [i].terSemHandle,
                                  OSALSEMAPHORE_WAIT_FOREVER) ;
                }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            }
        }
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "_NotifyDrv_addBufByPid", status);

    return status;
}


/*!
 *  @brief  Module setup function.
 *
 *  @sa     _NotifyDrv_destroy
 */
static
Void
_NotifyDrv_setup (Void)
{
    Error_Block  eb = 0;
    UInt16       i;
    List_Params  listparams;

    GT_0trace (curTrace, GT_ENTER, "_NotifyDrv_setup");

    List_Params_init (&listparams);
    NotifyDrv_state.eventCbckList = List_create (&listparams, &eb);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (NotifyDrv_state.eventCbckList == NULL) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "_NotifyDrv_setup",
                             Notify_E_FAIL,
                             "Failed to create event callback list!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        NotifyDrv_state.singleEventCbckList = List_create (&listparams, &eb);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (NotifyDrv_state.singleEventCbckList == NULL) {
            GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "_NotifyDrv_setup",
                             Notify_E_FAIL,
                             "Failed to create single event callback list!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            NotifyDrv_state.gateHandle = (IGateProvider_Handle)
                         GateSpinlock_create ((GateSpinlock_Handle) NULL, &eb);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (NotifyDrv_state.gateHandle == NULL) {
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "_NotifyDrv_setup",
                                     Notify_E_FAIL,
                                     "Failed to create spinlock gate!");
            }
            else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                for (i = 0 ; i < MAX_PROCESSES ; i++) {
                    NotifyDrv_state.eventState [i].bufList = NULL;
                    NotifyDrv_state.eventState [i].pid = -1;
                    NotifyDrv_state.eventState [i].refCount = 0;
                }
                NotifyDrv_state.isSetup = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            }
        }
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_0trace (curTrace, GT_LEAVE, "_NotifyDrv_setup");
}


/*!
 *  @brief  Module destroy function.
 *
 *  @sa     _NotifyDrv_setup
 */
static
Void
_NotifyDrv_destroy (Void)
{
    NotifyDrv_EventPacket * packet;
    NotifyDrv_EventCbck *   cbck;
    UInt32                  i;

    GT_0trace (curTrace, GT_ENTER, "_NotifyDrv_destroy");

    for (i = 0 ; i < MAX_PROCESSES ; i++) {
        NotifyDrv_state.eventState [i].bufList = NULL;
        NotifyDrv_state.eventState [i].pid = -1;
        NotifyDrv_state.eventState [i].refCount = 0;
        if (NotifyDrv_state.eventState [i].bufList != NULL) {
            /* Free event packets for any received but unprocessed events. */
            while (List_empty (NotifyDrv_state.eventState [i].bufList) != TRUE){
                packet = (NotifyDrv_EventPacket *)
                              List_get (NotifyDrv_state.eventState [i].bufList);
                if (packet != NULL){
                    kfree (packet);
                }
            }
            List_delete (&(NotifyDrv_state.eventState [i].bufList));
        }
    }

    /* Clear any event registrations that were not unregistered. */
    if (NotifyDrv_state.eventCbckList != NULL) {
        while (List_empty (NotifyDrv_state.eventCbckList) != TRUE) {
            cbck = (NotifyDrv_EventCbck *)
                                    List_get (NotifyDrv_state.eventCbckList);
            if (cbck != NULL){
                Memory_free (NULL,
                             cbck,
                             sizeof (NotifyDrv_EventCbck));
            }
        }
        List_delete (&(NotifyDrv_state.eventCbckList));
    }

    /* Clear any event registrations that were not unregistered from single
     * list.
     */
    if (NotifyDrv_state.singleEventCbckList != NULL) {
        while (List_empty (NotifyDrv_state.singleEventCbckList) != TRUE) {
            cbck = (NotifyDrv_EventCbck *)
                                List_get (NotifyDrv_state.singleEventCbckList);
            if (cbck != NULL){
                Memory_free (NULL,
                             cbck,
                             sizeof (NotifyDrv_EventCbck));
            }
        }
        List_delete (&(NotifyDrv_state.singleEventCbckList));
    }

    if (NotifyDrv_state.gateHandle != NULL) {
        GateSpinlock_delete ((GateSpinlock_Handle *)
                                       &(NotifyDrv_state.gateHandle));
    }

    NotifyDrv_state.isSetup = FALSE ;

    GT_0trace (curTrace, GT_LEAVE, "_NotifyDrv_destroy");
}


/*!
 *  @brief      Attach a process to notify user support framework.
 *
 *  @param      pid    Process identifier
 *
 *  @sa         NotifyDrv_detach
 */
static
Int
NotifyDrv_attach (UInt32 pid)
{
    Int32                status   = Notify_S_SUCCESS;
    Bool                 flag     = FALSE;
    Bool                 isInit   = FALSE;
    List_Object *        bufList  = NULL;
    IArg                 key      = 0;
    Error_Block          eb;
    List_Params          listparams;
    UInt32               i;

    OsalSemaphore_Handle semHandle;
    OsalSemaphore_Handle terSemHandle;

    GT_1trace (curTrace, GT_ENTER, "NotifyDrv_attach", pid);
    Error_init (&eb);

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (NotifyDrv_state.isSetup == FALSE) {
        /*! @retval Notify_E_FAIL The Notify OS driver was not setup */
        status = Notify_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "NotifyDrv_attach",
                             Notify_E_FAIL,
                             "The Notify OS driver was not setup!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        key = IGateProvider_enter (NotifyDrv_state.gateHandle);
        for (i = 0 ; (i < MAX_PROCESSES) ; i++) {
            if (NotifyDrv_state.eventState [i].pid == pid) {
                NotifyDrv_state.eventState [i].refCount++;
                isInit = TRUE;
                status = Notify_S_ALREADYSETUP;
                break;
            }
        }

        if (isInit == FALSE) {
            List_Params_init (&listparams);
            bufList = List_create (&listparams, &eb) ;
            /* Create the semaphore */
            semHandle = OsalSemaphore_create (
                                 (   OsalSemaphore_Type_Counting
                                  |  OsalSemaphore_IntType_Interruptible));
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (semHandle == NULL) {
                /*! @retval Notify_E_FAIL Failed to create semaphore for waiting
                              on events!*/
                status = Notify_E_FAIL;
                GT_setFailureReason (curTrace,
                                GT_4CLASS,
                                "NotifyDrv_attach",
                                status,
                                "Failed to create semaphore for waiting on"
                                "events!");
            }
            else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                /* Create the termination semaphore */
                terSemHandle = OsalSemaphore_create (
                                   OsalSemaphore_Type_Binary
                                |  OsalSemaphore_IntType_Interruptible);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (terSemHandle == NULL) {
                    /*! @retval Notify_E_FAIL Failed to create termination
                        semaphore!*/
                    status = Notify_E_FAIL;
                    GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "NotifyDrv_attach",
                                 status,
                                 "Failed to create termination semaphore!");
                }
                else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                    /* Search for an available slot for user process. */
                    for (i = 0 ; i < MAX_PROCESSES ; i++) {
                        if (NotifyDrv_state.eventState [i].pid == -1) {
                            NotifyDrv_state.eventState [i].semHandle =
                                                                semHandle;
                            NotifyDrv_state.eventState [i].terSemHandle =
                                                              terSemHandle;
                            NotifyDrv_state.eventState [i].pid = pid;
                            NotifyDrv_state.eventState [i].refCount = 1;
                            NotifyDrv_state.eventState [i].bufList =
                                                                    bufList;
                            flag = TRUE;
                            break;
                        }
                    }

                /* No free slots found. Let this check remain at run-time,
                 * since it is dependent on user environment.
                 */
                    if (flag != TRUE) {
                        /*! @retval Notify_E_RESOURCE Maximum number of
                         supported user clients have already been registered. */
                        status = Notify_E_RESOURCE;
                        GT_setFailureReason (curTrace,
                                          GT_4CLASS,
                                          "NotifyDrv_attach",
                                          status,
                                          "Maximum number of supported user"
                                          " clients have already been "
                                          "registered.");
                        if (bufList != NULL) {
                            List_delete (&bufList);
                        }
                        if (semHandle != NULL) {
                            OsalSemaphore_delete (&semHandle);
                        }
                        if (terSemHandle != NULL) {
                            OsalSemaphore_delete (&terSemHandle);
                        }
                    }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                }
            }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    IGateProvider_leave (NotifyDrv_state.gateHandle, key);

    GT_1trace (curTrace, GT_LEAVE, "NotifyDrv_attach", status);

    /*! @retval Notify_S_SUCCESS Operation successfully completed. */
    return status ;
}


/*!
 *  @brief      Detach a process from notify user support framework.
 *
 *  @param      pid    Process identifier
 *
 *  @sa         NotifyDrv_attach
 */
static
Int
NotifyDrv_detach (UInt32 pid)
{
    Int32                status    = Notify_S_SUCCESS;
    Int32                tmpStatus = Notify_S_SUCCESS;
    Bool                 flag      = FALSE;
    List_Object *        bufList   = NULL;
    UInt32               i;
    IArg                 key;
    OsalSemaphore_Handle semHandle;
    OsalSemaphore_Handle terSemHandle;

    GT_1trace (curTrace, GT_ENTER, "NotifyDrv_detach", pid);

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (NotifyDrv_state.isSetup == FALSE) {
        /*! @retval Notify_E_FAIL The Notify OS driver was not setup */
        status = Notify_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "NotifyDrv_detach",
                             Notify_E_FAIL,
                             "The Notify OS driver was not setup!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        key = IGateProvider_enter (NotifyDrv_state.gateHandle);

        for (i = 0 ; i < MAX_PROCESSES ; i++) {
            if (NotifyDrv_state.eventState [i].pid == pid) {
                if (NotifyDrv_state.eventState [i].refCount == 1) {
                    NotifyDrv_state.eventState [i].refCount = 0;

                    flag = TRUE;
                    break;
                }
                else {
                    NotifyDrv_state.eventState [i].refCount--;
                    status = Notify_S_ALREADYSETUP;
                }
            }
        }
        IGateProvider_leave (NotifyDrv_state.gateHandle, key);

        if (flag == TRUE) {
            /* Send the termination packet to notify thread */
            status = _NotifyDrv_addBufByPid (0, /* Ignored. */
                                             0,
                                             (UInt32) pid,
                                             (UInt32) -1,
                                             (UInt32) 0,
                                             NULL,
                                             NULL);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (status < 0) {
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "NotifyDrv_detach",
                                     status,
                                     "Failed to send termination packet.");
            }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            key = IGateProvider_enter (NotifyDrv_state.gateHandle);
            /* Last client being unregistered for this process. */
            NotifyDrv_state.eventState [i].pid = -1;

            /* Store in local variable to delete outside lock. */
            bufList = NotifyDrv_state.eventState [i].bufList;
            semHandle = NotifyDrv_state.eventState [i].semHandle;
            terSemHandle = NotifyDrv_state.eventState [i].terSemHandle;

            NotifyDrv_state.eventState [i].bufList = NULL;
            NotifyDrv_state.eventState [i].semHandle = NULL;
            NotifyDrv_state.eventState [i].terSemHandle = NULL;

            IGateProvider_leave (NotifyDrv_state.gateHandle, key);
        }

        if (flag != TRUE) {
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (i == MAX_PROCESSES) {
                /*! @retval Notify_E_NOTFOUND The specified user process was
                         not found registered with Notify Driver module. */
                status = Notify_E_NOTFOUND;
                GT_setFailureReason (curTrace,
                                  GT_4CLASS,
                                  "NotifyDrv_detach",
                                  status,
                                  "The specified user process was not found"
                                  " registered with Notify Driver module.");
            }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }
        else {
            /* Last client being unregistered with Notify module. */
            List_delete (&bufList);

            /* Remove/delete the semphore */
            tmpStatus = OsalSemaphore_delete (&semHandle);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if ((tmpStatus < 0) && (status >= 0)) {
                status =  tmpStatus;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "NotifyDrv_detach",
                                     status,
                                     "Failed to delete semaphore!");
            }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

            /* Remove/delete the termination semphore */
            tmpStatus = OsalSemaphore_delete (&terSemHandle);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if ((tmpStatus < 0) && (status >= 0)) {
                status =  tmpStatus;
                GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "NotifyDrv_detach",
                                 status,
                                 "Failed to delete termination semaphore!");
            }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "NotifyDrv_detach", status);

    /*! @retval Notify_S_SUCCESS Operation successfully completed */
    return status;
}


/** ============================================================================
 *  Functions required for multiple .ko modules configuration
 *  ============================================================================
 */
#if defined (SYSLINK_MULTIPLE_MODULES)
/*!
 *  @brief  Module initialization function for Linux driver.
 *
 *  @sa     NotifyDrv_finalizeModule
 */
static
Int
__init NotifyDrv_initializeModule (void)
{
    int result = 0;

    /* Display the version info and created date/time */
    Osal_printf ("Notify module created on Date:%s Time:%s\n",
                 __DATE__,
                 __TIME__);

    Osal_printf ("NotifyDrv_initializeModule\n");

    /* Enable/disable levels of tracing. */
    if (TRACE != NULL) {
        Osal_printf ("Trace enable %s\n", TRACE) ;
        Notify_enableTrace = simple_strtol (TRACE, NULL, 16);
        if ((Notify_enableTrace != 0) && (Notify_enableTrace != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACE\n") ;
        }
        else if (Notify_enableTrace == TRUE) {
            curTrace = GT_TraceState_Enable;
        }
        else if (Notify_enableTrace == FALSE) {
            curTrace = GT_TraceState_Disable;
        }
    }

    if (TRACEENTER != NULL) {
        Osal_printf ("Trace entry/leave prints enable %s\n", TRACEENTER) ;
        Notify_enableTraceEnter = simple_strtol (TRACEENTER, NULL, 16);
        if (    (Notify_enableTraceEnter != 0)
            &&  (Notify_enableTraceEnter != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (Notify_enableTraceEnter == TRUE) {
            curTrace |= GT_TraceEnter_Enable;
        }
    }

    if (TRACEFAILURE != NULL) {
        Osal_printf ("Trace SetFailureReason enable %s\n", TRACEFAILURE) ;
        Notify_enableTraceFailure = simple_strtol (TRACEFAILURE, NULL, 16);
        if (    (Notify_enableTraceFailure != 0)
            &&  (Notify_enableTraceFailure != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (Notify_enableTraceFailure == TRUE) {
            curTrace |= GT_TraceSetFailure_Enable;
        }
    }

    if (TRACECLASS != NULL) {
        Osal_printf ("Trace class %s\n", TRACECLASS) ;
        Notify_traceClass = simple_strtol (TRACECLASS, NULL, 16);
        if (    (Notify_enableTraceFailure != 1)
            &&  (Notify_enableTraceFailure != 2)
            &&  (Notify_enableTraceFailure != 3)) {
            Osal_printf ("Error! Only 1/2/3 supported for TRACECLASS\n") ;
        }
        else {
            Notify_traceClass =
                            Notify_traceClass << (32 - GT_TRACECLASS_SHIFT);
            curTrace |= Notify_traceClass;
        }
    }

    Osal_printf ("curTrace value: 0x%x\n", curTrace);

    /* Initialize the OsalDriver */
    OsalDriver_setup ();

    NotifyDrv_osalDrvHandle = NotifyDrv_registerDriver ();
    if (NotifyDrv_osalDrvHandle == NULL) {
        /*! @retval Notify_E_OSFAILURE Failed to register Notify
                                        driver with OS! */
        result = -EFAULT;
        GT_setFailureReason (curTrace,
                          GT_4CLASS,
                          "NotifyDrv_initializeModule",
                          Notify_E_OSFAILURE,
                          "Failed to register Notify driver with OS!");
    }

    Osal_printf ("NotifyDrv_initializeModule 0x%x\n", result);

    return result;
}

/*!
 *  @brief  Linux driver function to finalize the driver module.
 *
 *  @sa     NotifyDrv_initializeModule
 */
static
Void
__exit NotifyDrv_finalizeModule (Void)
{
    Osal_printf ("Entered NotifyDrv_finalizeModule\n");

    NotifyDrv_unregisterDriver (&(NotifyDrv_osalDrvHandle));

    /* Finalize the OsalDriver */
    OsalDriver_destroy ();

    Osal_printf ("Leaving NotifyDrv_finalizeModule\n");
}


/*!
 *  @brief  Macro calls that indicate initialization and finalization functions
 *          to the kernel.
 */
MODULE_LICENSE ("GPL v2");
module_init (NotifyDrv_initializeModule) ;
module_exit (NotifyDrv_finalizeModule) ;
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
