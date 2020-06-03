/*
 *  @file   NameServerDrv.c
 *
 *  @brief      OS-specific implementation of NameServer driver for Linux
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
#include <ti/syslink/utils/List.h>
#include <OsalDriver.h>
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/Memory.h>

/* Linux specific header files */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>

/* Module specific header files */
#include <_NameServer.h>
#include <ti/ipc/NameServer.h>
#include <NameServerDrv.h>
#include <NameServerDrvDefs.h>


/** ============================================================================
 *  Export kernel utils functions
 *  ============================================================================
 */
/* NameServer functions */
EXPORT_SYMBOL(NameServer_setup);
EXPORT_SYMBOL(NameServer_destroy);
EXPORT_SYMBOL(NameServer_Params_init);
EXPORT_SYMBOL(NameServer_create);
EXPORT_SYMBOL(NameServer_delete);
EXPORT_SYMBOL(NameServer_add);
EXPORT_SYMBOL(NameServer_addUInt32);
EXPORT_SYMBOL(NameServer_get);
EXPORT_SYMBOL(NameServer_getLocal);
EXPORT_SYMBOL(NameServer_match);
EXPORT_SYMBOL(NameServer_remove);
EXPORT_SYMBOL(NameServer_removeEntry);
EXPORT_SYMBOL(NameServer_getHandle);
EXPORT_SYMBOL(NameServer_registerRemoteDriver);
EXPORT_SYMBOL(NameServer_unregisterRemoteDriver);

/* NameServerDrv functions */
EXPORT_SYMBOL(NameServerDrv_registerDriver);
EXPORT_SYMBOL(NameServerDrv_unregisterDriver);


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/*!
 *  @brief  Driver minor number for NameServer.
 */
#define NAMESERVER_DRV_MINOR_NUMBER 2


/** ============================================================================
 *  Forward declarations of internal functions
 *  ============================================================================
 */
/*!
 *  @brief  Linux driver function to open the driver object.
 */
static int NameServerDrv_open (struct inode * inode, struct file * filp);

/*!
 *  @brief  Linux driver function to close the driver object.
 */
static int NameServerDrv_close (struct inode * inode, struct file * filp);

/*!
 *  @brief  Linux driver function to ioctl of the driver object.
 */
static long NameServerDrv_ioctl (struct file *  filp,
                                 unsigned int   cmd,
                                 unsigned long  args);


#if defined (SYSLINK_MULTIPLE_MODULES)
/*!
 *  @brief  Module initialization function for Linux driver.
 */
static int __init NameServerDrv_initializeModule (void);

/*!
 *  @brief  Module finalization  function for Linux driver.
 */
static void  __exit NameServerDrv_finalizeModule (void);
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


/** ============================================================================
 *  Globals
 *  ============================================================================
 */

/*!
 *  @brief  Function to invoke the APIs through ioctl.
 */
static struct file_operations NameServerDrv_driverOps = {
    open:    NameServerDrv_open,
    release: NameServerDrv_close,
    unlocked_ioctl:   NameServerDrv_ioctl,
} ;


#if defined (SYSLINK_MULTIPLE_MODULES)
/*!
 *  @brief  Indicates whether trace should be enabled.
 */
static Char * TRACE = FALSE;
module_param (TRACE, charp, S_IRUGO);
Bool NameServerDrv_enableTrace = FALSE;

/*!
 *  @brief  Indicates whether entry/leave trace should be enabled.
 */
static Char * TRACEENTER = FALSE;
module_param (TRACEENTER, charp, S_IRUGO);
Bool NameServerDrv_enableTraceEnter = FALSE;

/*!
 *  @brief  Indicates whether SetFailureReason trace should be enabled.
 */
static Char * TRACEFAILURE = FALSE;
module_param (TRACEFAILURE, charp, S_IRUGO);
Bool NameServerDrv_enableTraceFailure = FALSE;

/*!
 *  @brief  Indicates class of trace to be enabled
 */
static Char * TRACECLASS = NULL;
module_param (TRACECLASS, charp, S_IRUGO);
UInt32 NameServerDrv_traceClass = 0;

/*!
 *  @brief  OsalDriver handle for NameServer
 */
static Ptr NameServerDrv_osalDrvHandle = NULL;
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Register the NameServer with OsalDriver
 */
Ptr
NameServerDrv_registerDriver (Void)
{
    OsalDriver_Handle osalHandle;

    GT_0trace (curTrace, GT_ENTER, "NameServerDrv_registerDriver");

    osalHandle = OsalDriver_registerDriver ("NameServer",
                                            &NameServerDrv_driverOps,
                                            NAMESERVER_DRV_MINOR_NUMBER);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (osalHandle == NULL) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "NameServerDrv_registerDriver",
                             NameServer_E_INVALIDARG,
                             "OsalDriver_registerDriver failed!");
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "NameServerDrv_registerDriver", osalHandle);

    return (Ptr) osalHandle;
}


/*!
 *  @brief  Register the NameServer with OsalDriver
 */
Void
NameServerDrv_unregisterDriver (Ptr * drvHandle)
{
    GT_1trace (curTrace, GT_ENTER, "NameServerDrv_unregisterDriver", drvHandle);

    OsalDriver_unregisterDriver ((OsalDriver_Handle *) drvHandle);

    GT_0trace (curTrace, GT_LEAVE, "NameServerDrv_unregisterDriver");
}


/*!
 *  @brief  Linux specific function to open the driver.
 */
int
NameServerDrv_open (struct inode * inode, struct file * filp)
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
NameServerDrv_close (struct inode * inode, struct file * filp)
{
    return 0;
}

/*!
 *  @brief  Linux specific function to close the driver.
 */
static
long NameServerDrv_ioctl (struct file *  filp,
                          unsigned int   cmd,
                          unsigned long  args)
{
    int                     osStatus = 0;
    Int32                   status = NameServer_S_SUCCESS;
    NameServerDrv_CmdArgs * cargs  = (NameServerDrv_CmdArgs *) args;
    Int32                   ret;

    GT_3trace (curTrace, GT_ENTER, "NameServerDrv_ioctl",
               filp, cmd, args);

    switch (cmd) {
        case CMD_NAMESERVER_ADD:
        {
            String name;
            Ptr    buf;
            Ptr    entry;

            /* Allocate memory for the name */
            name = Memory_alloc (NULL, cargs->args.add.nameLen, 0, NULL);
            GT_assert (curTrace, (name != NULL));
            /* Copy the name */
            ret = copy_from_user (name,
                                  cargs->args.add.name,
                                  cargs->args.add.nameLen);
            GT_assert (curTrace, (ret == 0));

            /* Allocate memory for the buf */
            buf = Memory_alloc (NULL, cargs->args.add.len, 0, NULL);
            GT_assert (curTrace, (buf != NULL));
            /* Copy the value */
            ret = copy_from_user (buf,
                                  cargs->args.add.buf,
                                  cargs->args.add.len);
            GT_assert (curTrace, (ret == 0));

            entry = NameServer_add (cargs->args.add.handle,
                                    name,
                                    buf,
                                    cargs->args.add.len);
            GT_assert (curTrace, (entry != NULL));
            cargs->args.add.entry = entry;

            /* free the allocated memory */
            Memory_free (NULL, name, cargs->args.add.nameLen);
            Memory_free (NULL, buf,  cargs->args.add.len);
        }
        break;

        case CMD_NAMESERVER_ADDUINT32:
        {
            String name;
            Ptr    entry;

            /* Allocate memory for the name */
            name = Memory_alloc (NULL, cargs->args.addUInt32.nameLen, 0, NULL);
            GT_assert (curTrace, (name != NULL));
            /* Copy the name */
            ret = copy_from_user (name,
                                  cargs->args.addUInt32.name,
                                  cargs->args.addUInt32.nameLen);
            GT_assert (curTrace, (ret == 0));

            entry = NameServer_addUInt32 (cargs->args.addUInt32.handle,
                                          name,
                                          cargs->args.addUInt32.value);
            GT_assert (curTrace, (entry != NULL));
            cargs->args.addUInt32.entry = entry;

            /* free the allocated memory */
            Memory_free (NULL, name, cargs->args.addUInt32.nameLen);
        }
        break;

        case CMD_NAMESERVER_GET:
        {
            String   name;
            Ptr      value;
            UInt16 * procId = NULL;

            /* Allocate memory for the name */
            name = Memory_alloc (NULL, cargs->args.get.nameLen, 0, NULL);
            GT_assert (curTrace, (name != NULL));
            /* Copy the name */
            ret = copy_from_user (name,
                                  cargs->args.get.name,
                                  cargs->args.get.nameLen);
            GT_assert (curTrace, (ret == 0));

            /* Allocate memory for the buf */
            value = Memory_alloc (NULL, cargs->args.get.len, 0, NULL);
            GT_assert (curTrace, (value != NULL));

            /* Allocate memory for the procId */
            if (cargs->args.get.procLen > 0) {
                procId = Memory_alloc (NULL,
                                       cargs->args.get.procLen * sizeof(UInt16),
                                       0,
                                       NULL);
                GT_assert (curTrace, (procId != NULL));
            }
            /* Copy the procIds */
            ret = copy_from_user (procId,
                                  cargs->args.get.procId,
                                  cargs->args.get.procLen);
            GT_assert (curTrace, (ret == 0));

            status = NameServer_get (cargs->args.get.handle,
                                     name,
                                     value,
                                     &cargs->args.get.len,
                                     procId);
            /* Do not assert. This can return NameServer_E_NOTFOUND
             * as a valid runtime failure.
             */

            /* Copy the value */
            ret = copy_to_user (cargs->args.get.value,
                                value,
                                cargs->args.get.len);
            GT_assert (curTrace, (ret == 0));

            /* free the allocated memory */
            Memory_free (NULL, name, cargs->args.get.nameLen);
            Memory_free (NULL, value, cargs->args.get.len);
            if (procId != NULL) {
                Memory_free (NULL,
                             procId,
                             cargs->args.get.procLen *sizeof(UInt16));
            }

        }
        break;

        case CMD_NAMESERVER_GETLOCAL:
        {
            String   name;
            Ptr      value;

            /* Allocate memory for the name */
            name = Memory_alloc (NULL, cargs->args.getLocal.nameLen, 0, NULL);
            GT_assert (curTrace, (name != NULL));
            /* Copy the name */
            ret = copy_from_user (name,
                                  cargs->args.getLocal.name,
                                  cargs->args.getLocal.nameLen);
            GT_assert (curTrace, (ret == 0));

            /* Allocate memory for the buf */
            value = Memory_alloc (NULL, cargs->args.getLocal.len, 0, NULL);
            GT_assert (curTrace, (value != NULL));

            status = NameServer_getLocal (cargs->args.getLocal.handle,
                                          name,
                                          value,
                                          &cargs->args.getLocal.len);
            GT_assert (curTrace, (status >= 0));

            /* Copy the value */
            ret = copy_to_user (cargs->args.getLocal.value,
                                value,
                                cargs->args.getLocal.len);
            GT_assert (curTrace, (ret == 0));

            /* free the allocated memory */
            Memory_free (NULL, name, cargs->args.getLocal.nameLen);
            Memory_free (NULL, value,  cargs->args.getLocal.len);
        }
        break;

        case CMD_NAMESERVER_MATCH:
        {
            String name;

            /* Allocate memory for the name */
            name = Memory_alloc (NULL, cargs->args.match.nameLen, 0, NULL);
            GT_assert (curTrace, (name != NULL));
            /* Copy the name */
            ret = copy_from_user (name,
                                  cargs->args.match.name,
                                  cargs->args.match.nameLen);
            GT_assert (curTrace, (ret == 0));

            cargs->args.match.count = NameServer_match (
                                                   cargs->args.match.handle,
                                                   name,
                                                   &cargs->args.match.value);
            GT_assert (curTrace, (cargs->args.match.count >= 0));

            /* free the allocated memory */
            Memory_free (NULL, name, cargs->args.match.nameLen);
        }
        break;

        case CMD_NAMESERVER_REMOVE:
        {
            String   name;

            /* Allocate memory for the name */
            name = Memory_alloc (NULL, cargs->args.remove.nameLen, 0, NULL);
            GT_assert (curTrace, (name != NULL));
            /* Copy the name */
            ret = copy_from_user (name,
                                  cargs->args.remove.name,
                                  cargs->args.remove.nameLen);
            GT_assert (curTrace, (ret == 0));

            NameServer_remove (cargs->args.remove.handle,  name);

            /* free the allocated memory */
            Memory_free (NULL, name, cargs->args.remove.nameLen);
        }
        break;

        case CMD_NAMESERVER_REMOVEENTRY:
        {
            NameServer_removeEntry (cargs->args.removeEntry.handle,
                                    cargs->args.removeEntry.entry);
        }
        break;

        case CMD_NAMESERVER_PARAMS_INIT:
        {
            NameServer_Params params;

            NameServer_Params_init (&params);
            ret = copy_to_user (cargs->args.ParamsInit.params,
                                &params,
                                sizeof (NameServer_Params));
            GT_assert (curTrace, (ret == 0));
        }
        break;

        case CMD_NAMESERVER_CREATE:
        {
            NameServer_Params params;
            String            name;
            NameServer_Handle handle;

            /* Allocate memory for the name */
            name = Memory_alloc (NULL, cargs->args.create.nameLen, 0, NULL);
            GT_assert (curTrace, (name != NULL));
            /* Copy the name */
            ret = copy_from_user (name,
                                  cargs->args.create.name,
                                  cargs->args.create.nameLen);
            GT_assert (curTrace, (ret == 0));

            /* Copy the parameters */
            ret = copy_from_user (&params,
                                  cargs->args.create.params,
                                  sizeof (NameServer_Params));
            GT_assert (curTrace, (ret == 0));

            handle = NameServer_create (name, &params);
            GT_assert (curTrace, (handle != NULL));
            Memory_free (NULL, name, cargs->args.create.nameLen);
        }
        break;

        case CMD_NAMESERVER_DELETE:
        {
            status = NameServer_delete (&cargs->args.delete.handle);
            GT_assert (curTrace, (status >= 0));
        }
        break;

        case CMD_NAMESERVER_GETHANDLE:
        {
            String   name;

            /* Allocate memory for the name */
            name = Memory_alloc (NULL, cargs->args.getHandle.nameLen, 0, NULL);
            GT_assert (curTrace, (name != NULL));
            /* Copy the name */
            ret = copy_from_user (name,
                                  cargs->args.getHandle.name,
                                  cargs->args.getHandle.nameLen);
            GT_assert (curTrace, (ret == 0));

            cargs->args.getHandle.handle = NameServer_getHandle (name);

            /* free the allocated memory */
            Memory_free (NULL, name, cargs->args.getHandle.nameLen);
        }
        break;

        case CMD_NAMESERVER_SETUP:
        {
            status = NameServer_setup ();
            GT_assert (curTrace, (status >= 0));
        }
        break;

        case CMD_NAMESERVER_DESTROY:
        {
            status = NameServer_destroy ();
            GT_assert (curTrace, (status >= 0));
        }
        break;

        default:
        {
            /* This does not impact return status of this function, so retVal
             * comment is not used.
             */
            status = NameServer_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "NameServerDrv_ioctl",
                                 status,
                                 "Unsupported ioctl command specified");
        }
        break;
    }

    cargs->apiStatus = status;

    GT_1trace (curTrace, GT_LEAVE, "NameServerDrv_ioctl", osStatus);

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
int __init NameServerDrv_initializeModule (Void)
{
    int result = 0;

    /* Display the version info and created date/time */
    Osal_printf ("NameServer sample module created on Date:%s Time:%s\n",
                 __DATE__,
                 __TIME__);

    Osal_printf ("NameServerDrv_initializeModule\n");

    /* Enable/disable levels of tracing. */
    if (TRACE != NULL) {
        Osal_printf ("Trace enable %s\n", TRACE) ;
        NameServerDrv_enableTrace = simple_strtol (TRACE, NULL, 16);
        if (    (NameServerDrv_enableTrace != 0)
            &&  (NameServerDrv_enableTrace != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACE\n") ;
        }
        else if (NameServerDrv_enableTrace == TRUE) {
            curTrace = GT_TraceState_Enable;
        }
        else if (NameServerDrv_enableTrace == FALSE) {
            curTrace = GT_TraceState_Disable;
        }
    }

    if (TRACEENTER != NULL) {
        Osal_printf ("Trace entry/leave prints enable %s\n", TRACEENTER) ;
        NameServerDrv_enableTraceEnter = simple_strtol (TRACEENTER, NULL, 16);
        if (    (NameServerDrv_enableTraceEnter != 0)
            &&  (NameServerDrv_enableTraceEnter != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (NameServerDrv_enableTraceEnter == TRUE) {
            curTrace |= GT_TraceEnter_Enable;
        }
    }

    if (TRACEFAILURE != NULL) {
        Osal_printf ("Trace SetFailureReason enable %s\n", TRACEFAILURE) ;
        NameServerDrv_enableTraceFailure = simple_strtol (TRACEFAILURE,
                                                          NULL,
                                                          16);
        if (    (NameServerDrv_enableTraceFailure != 0)
            &&  (NameServerDrv_enableTraceFailure != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (NameServerDrv_enableTraceFailure == TRUE) {
            curTrace |= GT_TraceSetFailure_Enable;
        }
    }

    if (TRACECLASS != NULL) {
        Osal_printf ("Trace class %s\n", TRACECLASS) ;
        NameServerDrv_traceClass = simple_strtol (TRACECLASS, NULL, 16);
        if (    (NameServerDrv_enableTraceFailure != 1)
            &&  (NameServerDrv_enableTraceFailure != 2)
            &&  (NameServerDrv_enableTraceFailure != 3)) {
            Osal_printf ("Error! Only 1/2/3 supported for TRACECLASS\n") ;
        }
        else {
            NameServerDrv_traceClass =
                         NameServerDrv_traceClass << (32 - GT_TRACECLASS_SHIFT);
            curTrace |= NameServerDrv_traceClass;
        }
    }

    Osal_printf ("curTrace value: 0x%x\n", curTrace);

    /* Initialize the OsalDriver */
    OsalDriver_setup ();

    NameServerDrv_osalDrvHandle = NameServerDrv_registerDriver ();
    if (NameServerDrv_osalDrvHandle == NULL) {
        /*! @retval NameServer_E_OSFAILURE Failed to register NameServer
                                        driver with OS! */
        result = -EFAULT;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "NameServerDrv_initializeModule",
                             NameServer_E_OSFAILURE,
                             "Failed to register NameServer driver with OS!");
    }

    Osal_printf ("NameServerDrv_initializeModule 0x%x\n", result);

    return result;
}


/*!
 *  @brief  Linux driver function to finalize the driver module.
 */
static
void __exit NameServerDrv_finalizeModule (void)
{
    Osal_printf ("Entered NameServerDrv_finalizeModule\n");

    NameServerDrv_unregisterDriver (&(NameServerDrv_osalDrvHandle));

    /* Finalize the OsalDriver */
    OsalDriver_destroy ();

    Osal_printf ("Leaving NameServerDrv_finalizeModule\n");
}


/*!
 *  @brief  Macro calls that indicate initialization and finalization functions
 *          to the kernel.
 */
MODULE_LICENSE ("GPL v2");
module_init (NameServerDrv_initializeModule);
module_exit (NameServerDrv_finalizeModule);
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
