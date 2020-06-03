/*
 *  @file   HeapBufMPDrv.c
 *
 *  @brief      OS-specific implementation of HeapBufMP driver for Linux
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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/io.h>

/* Standard headers */
#include <ti/syslink/Std.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/List.h>
#include <OsalDriver.h>
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/Gate.h>
#include <ti/syslink/utils/Memory.h>

/* Module specific header files */
#include <ti/ipc/HeapBufMP.h>
#include <_HeapBufMP.h>
#include <HeapBufMPDrv.h>
#include <HeapBufMPDrvDefs.h>
#include <ti/ipc/SharedRegion.h>
#include <_SharedRegion.h>


/** ============================================================================
 *  Export kernel utils functions
 *  ============================================================================
 */
/* HeapBufMP functions */
EXPORT_SYMBOL(HeapBufMP_getConfig);
EXPORT_SYMBOL(HeapBufMP_setup);
EXPORT_SYMBOL(HeapBufMP_destroy);
EXPORT_SYMBOL(HeapBufMP_Params_init);
EXPORT_SYMBOL(HeapBufMP_create);
EXPORT_SYMBOL(HeapBufMP_delete);
EXPORT_SYMBOL(HeapBufMP_open);
EXPORT_SYMBOL(HeapBufMP_openByAddr);
EXPORT_SYMBOL(HeapBufMP_close);
EXPORT_SYMBOL(HeapBufMP_alloc);
EXPORT_SYMBOL(HeapBufMP_free);
EXPORT_SYMBOL(HeapBufMP_getStats);
EXPORT_SYMBOL(HeapBufMP_getExtendedStats);
EXPORT_SYMBOL(HeapBufMP_sharedMemReq);

/* HeapBufMPDrv functions */
EXPORT_SYMBOL(HeapBufMPDrv_registerDriver);
EXPORT_SYMBOL(HeapBufMPDrv_unregisterDriver);


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/*!
 *  @brief  Driver minor number for HeapBufMP.
 */
#define HEAPBUFMP_DRV_MINOR_NUMBER 4

/** ============================================================================
 *  Forward declarations of internal functions
 *  ============================================================================
 */
/* Linux driver function to open the driver object. */
static int HeapBufMPDrv_open (struct inode * inode, struct file * filp);

/* Linux driver function to close the driver object. */
static int HeapBufMPDrv_close (struct inode * inode, struct file * filp);

/* Linux driver function to ioctl of the driver object. */
static long HeapBufMPDrv_ioctl (struct file *  filp,
                                unsigned int   cmd,
                                unsigned long  args);


#if defined (SYSLINK_MULTIPLE_MODULES)
/* Module initialization function for Linux driver. */
static int __init HeapBufMPDrv_initializeModule (void);

/* Module finalization  function for Linux driver. */
static void  __exit HeapBufMPDrv_finalizeModule (void);
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


/** ============================================================================
 *  Globals
 *  ============================================================================
 */

/*!
 *  @brief  Function to invoke the APIs through ioctl.
 */
static struct file_operations HeapBufMPDrv_driverOps = {
    open:    HeapBufMPDrv_open,
    release: HeapBufMPDrv_close,
    unlocked_ioctl:   HeapBufMPDrv_ioctl,
} ;


#if defined (SYSLINK_MULTIPLE_MODULES)
/*!
 *  @brief  Indicates whether trace should be enabled.
 */
static Char * TRACE = FALSE;
module_param (TRACE, charp, S_IRUGO);
Bool HeapBufMPDrv_enableTrace = FALSE;

/*!
 *  @brief  Indicates whether entry/leave trace should be enabled.
 */
static Char * TRACEENTER = FALSE;
module_param (TRACEENTER, charp, S_IRUGO);
Bool HeapBufMPDrv_enableTraceEnter = FALSE;

/*!
 *  @brief  Indicates whether SetFailureReason trace should be enabled.
 */
static Char * TRACEFAILURE = FALSE;
module_param (TRACEFAILURE, charp, S_IRUGO);
Bool HeapBufMPDrv_enableTraceFailure = FALSE;

/*!
 *  @brief  Indicates class of trace to be enabled
 */
static Char * TRACECLASS = NULL;
module_param (TRACECLASS, charp, S_IRUGO);
UInt32 HeapBufMPDrv_traceClass = 0;

/*!
 *  @brief  OsalDriver handle for HeapBufMP
 */
static Ptr HeapBufMPDrv_osalDrvHandle = NULL;
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Register the HeapBufMP with OsalDriver
 */
Ptr
HeapBufMPDrv_registerDriver (Void)
{
    OsalDriver_Handle osalHandle;

    GT_0trace (curTrace, GT_ENTER, "HeapBufMPDrv_registerDriver");

    osalHandle = OsalDriver_registerDriver ("HeapBufMP",
                                            &HeapBufMPDrv_driverOps,
                                            HEAPBUFMP_DRV_MINOR_NUMBER);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (osalHandle == NULL) {
        /*! @retval NULL OsalDriver_registerDriver failed */
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "HeapBufMPDrv_registerDriver",
                             HeapBufMP_E_FAIL,
                             "OsalDriver_registerDriver failed!");
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "HeapBufMPDrv_registerDriver",
               osalHandle);

    /*! @retval Valid-Handle Operation successfully completed. */
    return (Ptr) osalHandle;
}


/*!
 *  @brief  Register the HeapBufMP with OsalDriver
 */
Void
HeapBufMPDrv_unregisterDriver (Ptr * drvHandle)
{
    GT_1trace (curTrace, GT_ENTER, "HeapBufMPDrv_unregisterDriver",
               drvHandle);

    OsalDriver_unregisterDriver ((OsalDriver_Handle *) drvHandle);

    GT_0trace (curTrace, GT_LEAVE, "HeapBufMPDrv_unregisterDriver");
}


/*!
 *  @brief  Linux specific function to open the driver.
 */
int
HeapBufMPDrv_open (struct inode * inode, struct file * filp)
{
    OsalDriver_Handle handle = container_of (inode->i_cdev,
                                             OsalDriver_Object,
                                             cdev);
    filp->private_data = handle;

    /*! @retval 0 Operation successfully completed. */
    return 0;
}


/*!
 *  @brief  Linux specific function to close the driver.
 */
int
HeapBufMPDrv_close (struct inode * inode, struct file * filp)
{
    /*! @retval 0 Operation successfully completed. */
    return 0;
}


/*!
 *  @brief  Linux specific function to close the driver.
 */
static
long
HeapBufMPDrv_ioctl (struct file *  filp,
                    unsigned int   cmd,
                    unsigned long  args)
{
    int                  osStatus = 0;
    HeapBufMPDrv_CmdArgs * dstArgs  = (HeapBufMPDrv_CmdArgs *) args;
    Int32                status = HeapBufMP_S_SUCCESS;
    Int32                ret;
    HeapBufMPDrv_CmdArgs   cargs;

    GT_3trace (curTrace, GT_ENTER, "HeapBufMPDrv_ioctl",
               filp, cmd, args);


    /* Copy the full args from user-side */
    ret = copy_from_user (&cargs,
                          dstArgs,
                          sizeof (HeapBufMPDrv_CmdArgs));
    GT_assert (curTrace, (ret == 0));

    switch (cmd) {
        case CMD_HEAPBUFMP_ALLOC:
        {
            Char *             block;
            SharedRegion_SRPtr blockSrPtr = SharedRegion_INVALIDSRPTR;
            UInt16              index;
            block = HeapBufMP_alloc (cargs.args.alloc.handle,
                                     cargs.args.alloc.size,
                                     cargs.args.alloc.align);
            if (block != NULL) {
                index = SharedRegion_getId (block);
                blockSrPtr = SharedRegion_getSRPtr (block, index);
            }

            cargs.args.alloc.blockSrPtr = blockSrPtr;
        }
        break;

        case CMD_HEAPBUFMP_FREE:
        {
            Char * block;
            block = SharedRegion_getPtr (cargs.args.free.blockSrPtr);
            HeapBufMP_free (cargs.args.free.handle,
                            block,
                            cargs.args.free.size);
        }
        break;

        case CMD_HEAPBUFMP_PARAMS_INIT:
        {
            HeapBufMP_Params params;

            HeapBufMP_Params_init ( &params);

            ret = copy_to_user (cargs.args.ParamsInit.params,
                                &params,
                                sizeof (HeapBufMP_Params));
            GT_assert (curTrace, (ret == 0));
        }
        break;

        case CMD_HEAPBUFMP_CREATE:
        {
            HeapBufMP_Params params;

            ret = copy_from_user (&params,
                                  cargs.args.create.params,
                                  sizeof (HeapBufMP_Params));
            GT_assert (curTrace, (ret == 0));

            /* Allocate memory for the name */
            if (cargs.args.create.nameLen > 0) {
                params.name = Memory_alloc (NULL,
                                            cargs.args.create.nameLen,
                                            0,
                                            NULL);
                GT_assert (curTrace, (params.name != NULL));
                /* Copy the name */
                ret = copy_from_user (params.name,
                                      cargs.args.create.params->name,
                                      cargs.args.create.nameLen);
                GT_assert (curTrace, (ret == 0));
            }

            params.sharedAddr = SharedRegion_getPtr (
                                             cargs.args.create.sharedAddrSrPtr);
            GT_1trace (curTrace,
                       GT_2CLASS,
                       "    HeapBufMP_create knl ioctl: Shared addr [0x%x]\n",
                       params.sharedAddr);


            /* Update gate in params. */
            params.gate = cargs.args.create.knlGate;

            cargs.args.create.handle = HeapBufMP_create (&params);
            GT_assert (curTrace, (cargs.args.create.handle != NULL));

            /* Set failure status if create has failed. */
            if (cargs.args.create.handle == NULL) {
                status = HeapBufMP_E_FAIL;
            }

            if (cargs.args.create.nameLen > 0) {
                Memory_free (NULL, params.name, cargs.args.create.nameLen);
            }
        }
        break;

        case CMD_HEAPBUFMP_DELETE:
        {
            status = HeapBufMP_delete ((HeapBufMP_Handle *)
                                            &cargs.args.deleteInstance.handle);
            GT_assert (curTrace, (status >= 0));
        }
        break;

        case CMD_HEAPBUFMP_OPEN:
        {
            String           name  = NULL;
            HeapBufMP_Handle handle;


            /* Allocate memory for the name */
            if (cargs.args.open.nameLen > 0) {
                name = Memory_alloc (NULL,
                                     cargs.args.open.nameLen,
                                     0,
                                     NULL);
                GT_assert (curTrace, (name != NULL));
                /* Copy the name */
                ret = copy_from_user (name,
                                      cargs.args.open.name,
                                      cargs.args.open.nameLen);
                GT_assert (curTrace, (ret == 0));
            }

            status = HeapBufMP_open (name, &handle);
            /* HeapBufMP_open can fail at run-time, so don't assert on its
             * failure.
             */
            cargs.args.open.handle = handle;

            if (cargs.args.open.nameLen > 0) {
                Memory_free (NULL, name, cargs.args.open.nameLen);
            }
        }
        break;

        case CMD_HEAPBUFMP_OPENBYADDR:
        {
            HeapBufMP_Handle handle;
            Ptr              sharedAddr;

            sharedAddr = SharedRegion_getPtr (cargs.args.openByAddr.sharedAddrSrPtr);

            status = HeapBufMP_openByAddr (sharedAddr, &handle);
            /* HeapBufMP_open can fail at run-time, so don't assert on its
             * failure.
             */
            cargs.args.openByAddr.handle = handle;

        }
        break;

        case CMD_HEAPBUFMP_CLOSE:
        {
            status = HeapBufMP_close ((HeapBufMP_Handle *)
                                                &(cargs.args.close.handle));
            GT_assert (curTrace, (status >= 0));
        }
        break;

        case CMD_HEAPBUFMP_SHAREDMEMREQ:
        {
            HeapBufMP_Params params;

            ret = copy_from_user (&params,
                                  cargs.args.sharedMemReq.params,
                                  sizeof (HeapBufMP_Params));
            GT_assert (curTrace, (ret == 0));

            cargs.args.sharedMemReq.bytes =
                       HeapBufMP_sharedMemReq (&params);
        }
        break;

        case CMD_HEAPBUFMP_GETCONFIG:
        {
            HeapBufMP_Config config;

            HeapBufMP_getConfig (&config);

            ret = copy_to_user (cargs.args.getConfig.config,
                                &config,
                                sizeof (HeapBufMP_Config));
            GT_assert (curTrace, (ret == 0));
        }
        break;

        case CMD_HEAPBUFMP_SETUP:
        {
            HeapBufMP_Config config;

            ret = copy_from_user (&config,
                                  cargs.args.getConfig.config,
                                  sizeof (HeapBufMP_Config));
            GT_assert (curTrace, (ret == 0));

            status = HeapBufMP_setup (&config);
            GT_assert (curTrace, (status >= 0));
        }
        break;

        case CMD_HEAPBUFMP_DESTROY:
        {
            status = HeapBufMP_destroy ();
            GT_assert (curTrace, (status >= 0));
        }
        break;

        case CMD_HEAPBUFMP_GETSTATS:
        {
            Memory_Stats  stats;

            HeapBufMP_getStats (cargs.args.getStats.handle,
                                &stats);

            ret = copy_to_user (cargs.args.getStats.stats,
                                &stats,
                                sizeof (Memory_Stats));
            GT_assert (curTrace, (ret == 0));
        }
        break;

        case CMD_HEAPBUFMP_GETEXTENDEDSTATS:
        {
            HeapBufMP_ExtendedStats stats;

            HeapBufMP_getExtendedStats (
                                         cargs.args.getExtendedStats.handle,
                                         &stats);

            ret = copy_to_user (cargs.args.getExtendedStats.stats,
                                &stats,
                                sizeof (HeapBufMP_ExtendedStats));
            GT_assert (curTrace, (ret == 0));
        }
        break;

        default :
        {
            /* This does not impact return status of this function, so retVal
             * comment is not used.
             */
            status = HeapBufMP_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "HeapBufMPDrv_ioctl",
                                 status,
                                 "Unsupported ioctl command specified");
        }
        break;
  }

    cargs.apiStatus = status;

    /* Copy the full args to the user-side. */
    ret = copy_to_user (dstArgs,
                        &cargs,
                        sizeof (HeapBufMPDrv_CmdArgs));
    GT_assert (curTrace, (ret == 0));

    GT_1trace (curTrace, GT_LEAVE, "HeapBufMPDrv_ioctl",
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
int __init HeapBufMPDrv_initializeModule (Void)
{
    int result = 0;

    /* Display the version info and created date/time */
    Osal_printf ("HeapBufMP sample module created on Date:%s Time:%s\n",
                 __DATE__,
                 __TIME__);

    Osal_printf ("HeapBufMPDrv_initializeModule\n");

    /* Enable/disable levels of tracing. */
    if (TRACE != NULL) {
        HeapBufMPDrv_enableTrace = simple_strtol (TRACE, NULL, 16);
        if ((HeapBufMPDrv_enableTrace != 0) && (HeapBufMPDrv_enableTrace != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACE\n") ;
        }
        else if (HeapBufMPDrv_enableTrace == TRUE) {
            Osal_printf ("Trace enabled\n");
            curTrace = GT_TraceState_Enable;
        }
        else if (HeapBufMPDrv_enableTrace == FALSE) {
            Osal_printf ("Trace disabled\n");
            curTrace = GT_TraceState_Disable;
        }
    }

    if (TRACEENTER != NULL) {
        HeapBufMPDrv_enableTraceEnter = simple_strtol (TRACEENTER, NULL, 16);
        if (    (HeapBufMPDrv_enableTraceEnter != 0)
            &&  (HeapBufMPDrv_enableTraceEnter != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n") ;
        }
        else if (HeapBufMPDrv_enableTraceEnter == TRUE) {
            Osal_printf ("Trace entry/leave prints enabled\n");
            curTrace |= GT_TraceEnter_Enable;
        }
    }

    if (TRACEFAILURE != NULL) {
        HeapBufMPDrv_enableTraceFailure = simple_strtol (TRACEFAILURE, NULL, 16);
        if (    (HeapBufMPDrv_enableTraceFailure != 0)
            &&  (HeapBufMPDrv_enableTraceFailure != 1)) {
            Osal_printf ("Error! Only 0/1 supported for TRACEENTER\n");
        }
        else if (HeapBufMPDrv_enableTraceFailure == TRUE) {
            Osal_printf ("Trace SetFailureReason enabled\n");
            curTrace |= GT_TraceSetFailure_Enable;
        }
    }

    if (TRACECLASS != NULL) {
        HeapBufMPDrv_traceClass = simple_strtol (TRACECLASS, NULL, 16);
        if (    (HeapBufMPDrv_traceClass != 1)
            &&  (HeapBufMPDrv_traceClass != 2)
            &&  (HeapBufMPDrv_traceClass != 3)) {
            Osal_printf ("Error! Only 1/2/3 supported for TRACECLASS\n");
        }
        else {
            Osal_printf ("Trace class %s\n", TRACECLASS);
            HeapBufMPDrv_traceClass =
                            HeapBufMPDrv_traceClass << (32 - GT_TRACECLASS_SHIFT);
            curTrace |= HeapBufMPDrv_traceClass;
        }
    }

    Osal_printf ("curTrace value: 0x%x\n", curTrace);

    /* Initialize the OsalDriver */
    OsalDriver_setup ();

    HeapBufMPDrv_osalDrvHandle = HeapBufMPDrv_registerDriver ();
    if (HeapBufMPDrv_osalDrvHandle == NULL) {
        /*! @retval HeapBufMP_E_OSFAILURE Failed to register HeapBufMP
                                        driver with OS! */
        result = -EFAULT;
        GT_setFailureReason (curTrace,
                          GT_4CLASS,
                          "HeapBufMPDrv_initializeModule",
                          HeapBufMP_E_OSFAILURE,
                          "Failed to register HeapBufMP driver with OS!");
    }

    Osal_printf ("HeapBufMPDrv_initializeModule 0x%x\n", result);

    return result;
}


/*!
 *  @brief  Linux driver function to finalize the driver module.
 */
static
void __exit HeapBufMPDrv_finalizeModule (void)
{
    Osal_printf ("HeapBufMPDrv_finalizeModule\n");
    Osal_printf ("Entered HeapBufMPDrv_finalizeModule\n");

    HeapBufMPDrv_unregisterDriver (&(HeapBufMPDrv_osalDrvHandle));

    /* Finalize the OsalDriver */
    OsalDriver_destroy ();

    Osal_printf ("Leaving HeapBufMPDrv_finalizeModule\n");
}


/*!
 *  @brief  Macro calls that indicate initialization and finalization functions
 *          to the kernel.
 */
MODULE_LICENSE ("GPL v2");
module_init (HeapBufMPDrv_initializeModule);
module_exit (HeapBufMPDrv_finalizeModule);
#endif /* if defined (SYSLINK_MULTIPLE_MODULES) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
