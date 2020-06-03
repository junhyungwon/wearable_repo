/*
 *  @file   IpcDrv.c
 *
 *  @brief      OS-specific implementation of Ipc driver for Linux
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
#include <ti/ipc/Ipc.h>
#include <ti/syslink/IpcHost.h>
#include <_Ipc.h>
#include <IpcKnl.h>
#include <IpcDrv.h>
#include <IpcDrvDefs.h>
#include <ti/ipc/SharedRegion.h>


/** ============================================================================
 *  Export kernel utils functions
 *  ============================================================================
 */
/* Ipc functions */
EXPORT_SYMBOL(Ipc_getConfig);
EXPORT_SYMBOL(Ipc_setup);
EXPORT_SYMBOL(Ipc_destroy);
EXPORT_SYMBOL(Ipc_create);
EXPORT_SYMBOL(Ipc_control);
EXPORT_SYMBOL(Ipc_readConfig);
EXPORT_SYMBOL(Ipc_writeConfig);

/* IpcDrv functions */
EXPORT_SYMBOL(IpcDrv_registerDriver);
EXPORT_SYMBOL(IpcDrv_unregisterDriver);


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/*!
 *  @brief  Driver minor number for Ipc.
 */
#define IPC_DRV_MINOR_NUMBER  11


/** ============================================================================
 *  Forward declarations of internal functions
 *  ============================================================================
 */
/*!
 *  @brief  Linux driver function to open the driver object.
 */
static int IpcDrv_drvopen (struct inode * inode, struct file * filp);

/*!
 *  @brief  Linux driver function to close the driver object.
 */
static int IpcDrv_drvclose (struct inode * inode, struct file * filp);

/*!
 *  @brief  Linux driver function to ioctl of the driver object.
 */
static long IpcDrv_drvioctl (struct file *  filp,
                             unsigned int   cmd,
                             unsigned long  args);


#if defined (SYSLINK_MULTIPLE_MODULES)
/*!
 *  @brief  Module initialization function for Linux driver.
 */
static int __init IpcDrv_initializeModule (void);

/*!
 *  @brief  Module finalization  function for Linux driver.
 */
static void  __exit IpcDrv_finalizeModule (void);
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


/** ============================================================================
 *  Globals
 *  ============================================================================
 */

/*!
 *  @brief  Function to invoke the APIs through ioctl.
 */
static struct file_operations IpcDrv_driverOps = {
    open:    IpcDrv_drvopen,
    release: IpcDrv_drvclose,
    unlocked_ioctl:   IpcDrv_drvioctl,
} ;


#if defined (SYSLINK_MULTIPLE_MODULES)
/*!
 *  @brief  Indicates whether trace should be enabled.
 */
static Char * TRACE = FALSE;
module_param (TRACE, charp, S_IRUGO);
Bool IpcDrv_enableTrace = FALSE;

/*!
 *  @brief  Indicates whether entry/leave trace should be enabled.
 */
static Char * TRACEENTER = FALSE;
module_param (TRACEENTER, charp, S_IRUGO);
Bool IpcDrv_enableTraceEnter = FALSE;

/*!
 *  @brief  Indicates whether SetFailureReason trace should be enabled.
 */
static Char * TRACEFAILURE = FALSE;
module_param (TRACEFAILURE, charp, S_IRUGO);
Bool IpcDrv_enableTraceFailure = FALSE;

/*!
 *  @brief  Indicates class of trace to be enabled
 */
static Char * TRACECLASS = NULL;
module_param (TRACECLASS, charp, S_IRUGO);
UInt32 IpcDrv_traceClass = 0;

/*!
 *  @brief  OsalDriver handle for Ipc
 */
static Ptr IpcDrv_osalDrvHandle = NULL;
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Register the Ipc with OsalDriver
 */
Ptr
IpcDrv_registerDriver (Void)
{
    OsalDriver_Handle osalHandle;

    GT_0trace (curTrace, GT_ENTER, "IpcDrv_registerDriver");

    osalHandle = OsalDriver_registerDriver ("Ipc",
                                            &IpcDrv_driverOps,
                                            IPC_DRV_MINOR_NUMBER);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (osalHandle == NULL) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "IpcDrv_registerDriver",
                             Ipc_E_INVALIDARG,
                             "OsalDriver_registerDriver failed!");
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "IpcDrv_registerDriver",
               osalHandle);

    return (Ptr) osalHandle;
}


/*!
 *  @brief  Register the Ipc with OsalDriver
 */
Void
IpcDrv_unregisterDriver (Ptr * drvHandle)
{
    GT_1trace (curTrace, GT_ENTER, "IpcDrv_unregisterDriver",
               drvHandle);

    OsalDriver_unregisterDriver ((OsalDriver_Handle *) drvHandle);

    GT_0trace (curTrace, GT_LEAVE, "IpcDrv_unregisterDriver");
}


/*!
 *  @brief  Linux specific function to open the driver.
 */
int
IpcDrv_drvopen (struct inode * inode, struct file * filp)
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
IpcDrv_drvclose (struct inode * inode, struct file * filp)
{
    return 0;
}

/*!
 *  @brief  Linux specific function to close the driver.
 */
static
long IpcDrv_drvioctl (struct file *  filp,
                      unsigned int   cmd,
                      unsigned long  args)
{
    Int32                status = Ipc_S_SUCCESS;
    int                  osStatus = 0;
    IpcDrv_CmdArgs *     cargs  = (IpcDrv_CmdArgs *) args;
    Int32                ret;

    GT_3trace (curTrace, GT_ENTER, "IpcDrv_drvioctl",
               filp, cmd, args);

    switch (cmd) {
        case CMD_IPC_CONTROL:
        {
            if (cargs->args.control.arg != NULL) {
                status = Ipc_control (cargs->args.control.procId,
                              cargs->args.control.cmdId,
                              &(cargs->args.control.arg));
        	}
        	else {
                status = Ipc_control (cargs->args.control.procId,
                                  cargs->args.control.cmdId,
                                  NULL);
            }
            GT_assert (curTrace, (status >= 0));
        }
        break;

        case CMD_IPC_READCONFIG:
        {
            Ptr cfg;

            /* Allocate memory for the cfg */
            cfg = Memory_alloc (NULL, cargs->args.readConfig.size, 0, NULL);
            GT_assert (curTrace, (cfg != NULL));

            status = Ipc_readConfig (cargs->args.readConfig.remoteProcId,
                                     cargs->args.readConfig.tag,
                                     cfg,
                                     cargs->args.readConfig.size);
            GT_assert (curTrace, (status >= 0));

            /* Copy the value */
            ret = copy_to_user (cargs->args.readConfig.cfg,
                                cfg,
                                cargs->args.readConfig.size);
            GT_assert (curTrace, (ret == 0));

            if (cargs->args.readConfig.cfg != NULL) {
                Memory_free (NULL, cfg, cargs->args.readConfig.size);
            }
        }
        break;

        case CMD_IPC_WRITECONFIG:
        {
            Ptr cfg;

            /* Allocate memory for the buf */
            cfg = Memory_alloc (NULL, cargs->args.writeConfig.size, 0, NULL);
            GT_assert (curTrace, (cfg != NULL));
            ret = copy_from_user (cfg,
                                  cargs->args.writeConfig.cfg,
                                  cargs->args.writeConfig.size);
            GT_assert (curTrace, (ret == 0));

            status = Ipc_writeConfig (cargs->args.writeConfig.remoteProcId,
                                      cargs->args.writeConfig.tag,
                                      cargs->args.writeConfig.cfg,
                                      cargs->args.writeConfig.size);
            GT_assert (curTrace, (status >= 0));
            if (cargs->args.writeConfig.cfg != NULL) {
                Memory_free (NULL, cfg, cargs->args.writeConfig.size);
            }
        }
        break;

        default:
        {
            /* This does not impact return status of this function, so retVal
             * comment is not used.
             */
            status = Ipc_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "IpcDrv_drvioctl",
                                 status,
                                 "Unsupported ioctl command specified");
        }
        break;
    }

    cargs->apiStatus = status;

    GT_1trace (curTrace, GT_LEAVE, "IpcDrv_drvioctl",
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
int __init IpcDrv_initializeModule (Void)
{
    int result = 0;

    /* Display the version info and created date/time */
    Osal_printf ("Ipc sample module created on Date:%s Time:%s\n",
                 __DATE__,
                 __TIME__);

    Osal_printf ("IpcDrv_initializeModule\n");

    /* Enable/disable levels of tracing. */
    if (TRACE != NULL) {
        Osal_printf ("Trace enable %s\n", TRACE) ;
        IpcDrv_enableTrace = simple_strtol (TRACE, NULL, 16);
        if (    (IpcDrv_enableTrace != 0)
            &&  (IpcDrv_enableTrace != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACE\n") ;
        }
        else if (IpcDrv_enableTrace == TRUE) {
            curTrace = GT_TraceState_Enable;
        }
        else if (IpcDrv_enableTrace == FALSE) {
            curTrace = GT_TraceState_Disable;
        }
    }

    if (TRACEENTER != NULL) {
        Osal_printf ("Trace entry/leave prints enable %s\n", TRACEENTER) ;
        IpcDrv_enableTraceEnter = simple_strtol (TRACEENTER, NULL, 16);
        if (    (IpcDrv_enableTraceEnter != 0)
            &&  (IpcDrv_enableTraceEnter != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (IpcDrv_enableTraceEnter == TRUE) {
            curTrace |= GT_TraceEnter_Enable;
        }
    }

    if (TRACEFAILURE != NULL) {
        Osal_printf ("Trace SetFailureReason enable %s\n", TRACEFAILURE) ;
        IpcDrv_enableTraceFailure = simple_strtol (TRACEFAILURE,
                                                            NULL,
                                                            16);
        if (    (IpcDrv_enableTraceFailure != 0)
            &&  (IpcDrv_enableTraceFailure != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (IpcDrv_enableTraceFailure == TRUE) {
            curTrace |= GT_TraceSetFailure_Enable;
        }
    }

    if (TRACECLASS != NULL) {
        Osal_printf ("Trace class %s\n", TRACECLASS) ;
        IpcDrv_traceClass = simple_strtol (TRACECLASS, NULL, 16);
        if (    (IpcDrv_enableTraceFailure != 1)
            &&  (IpcDrv_enableTraceFailure != 2)
            &&  (IpcDrv_enableTraceFailure != 3)) {
            Osal_printf ("Error! Only 1/2/3 supported for TRACECLASS\n") ;
        }
        else {
            IpcDrv_traceClass =
                       IpcDrv_traceClass << (32 - GT_TRACECLASS_SHIFT);
            curTrace |= IpcDrv_traceClass;
        }
    }

    Osal_printf ("curTrace value: 0x%x\n", curTrace);

    /* Initialize the OsalDriver */
    OsalDriver_setup ();

    IpcDrv_osalDrvHandle = IpcDrv_registerDriver ();
    if (IpcDrv_osalDrvHandle == NULL) {
        /*! @retval Ipc_E_OSFAILURE Failed to register Ipc
                                        driver with OS! */
        result = -EFAULT;
        GT_setFailureReason (curTrace,
                          GT_4CLASS,
                          "IpcDrv_initializeModule",
                          Ipc_E_OSFAILURE,
                          "Failed to register Ipc driver with OS!");
    }

    Osal_printf ("IpcDrv_initializeModule 0x%x\n", result);

    return result;
}


/*!
 *  @brief  Linux driver function to finalize the driver module.
 */
static
void __exit IpcDrv_finalizeModule (void)
{
    Osal_printf ("Entered IpcDrv_finalizeModule\n");

    IpcDrv_unregisterDriver (&(IpcDrv_osalDrvHandle));

    /* Finalize the OsalDriver */
    OsalDriver_destroy ();

    Osal_printf ("Leaving IpcDrv_finalizeModule\n");
}


/*!
 *  @brief  Macro calls that indicate initialization and finalization functions
 *          to the kernel.
 */
MODULE_LICENSE ("GPL v2");
module_init (IpcDrv_initializeModule);
module_exit (IpcDrv_finalizeModule);
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
