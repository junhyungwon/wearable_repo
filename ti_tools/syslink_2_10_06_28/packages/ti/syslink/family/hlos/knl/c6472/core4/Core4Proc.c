/*
 *  @file   Core4Proc.c
 *
 * @brief       Processor implementation for Core4 of C6472.
 *
 *              This module is responsible for taking care of device-specific
 *              operations for the processor. This module can be used
 *              stand-alone or as part of ProcMgr.
 *              The implementation is specific to C6472.
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
#include <ti/syslink/utils/String.h>
#include <ti/syslink/utils/IGateProvider.h>
#include <ti/syslink/utils/GateMutex.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/syslink/utils/Trace.h>

/* Module level headers */
#include <ProcDefs.h>
#include <Processor.h>
#include <C6472Core4Proc.h>
#include <_C6472Core4Proc.h>
#include <ti/ipc/MultiProc.h>
#include <_MultiProc.h>

#if defined (__cplusplus)
extern "C" {
#endif

/* =============================================================================
 *  Macros and types
 * =============================================================================
 */
/*!
 *  @brief  Core4 DDR2 start address.
 */
#define DDR2_START               0xE0000000

/*!
 *  @brief  Core4 DDR2 end address.
 */
#define DDR2_END                 0xE8000000

/*!
 *  @brief  Checks if a value lies in given range.
 */
#define IS_RANGE_VALID(x,min,max) (((x) < (max)) && ((x) >= (min)))

/*!
 *  @brief  C6472CORE4PROC Module state object
 */
typedef struct C6472CORE4PROC_ModuleObject_tag {
    UInt32              configSize;
    /*!< Size of configuration structure */
    C6472CORE4PROC_Config cfg;
    /*!< C6472CORE4PROC configuration structure */
    C6472CORE4PROC_Config defCfg;
    /*!< Default module configuration */
    Bool                isSetup;
    /*!< Indicates whether the C6472CORE4PROC module is setup. */
    C6472CORE4PROC_Handle procHandles [MultiProc_MAXPROCESSORS];
    /*!< Processor handle array. */
    IGateProvider_Handle         gateHandle;
    /*!< Handle of gate to be used for local thread safety */
} C6472CORE4PROC_ModuleObject;

/*!
 *  @brief  C6472CORE4PROC instance object.
 */
typedef struct C6472CORE4PROC_Object_tag {
    C6472CORE4PROC_Params params;
    /*!< Instance parameters (configuration values) */
    ProcMgr_Handle      pmHandle;
    /*!< Handle to proc manager */
} C6472CORE4PROC_Object;


/* Default memory regions for Core4 */
static ProcMgr_AddrInfo C6472CORE4PROC_defaultMemRegions [] =
    {
        {
            .addr   = { -1u, -1u, 0x200000, 0x200000, 0x200000},
            .size   = 0xc0000
        },
        {
            .addr   = { -1u, -1u, 0x14800000, 0x00800000, 0x00800000},
            .size   = 0x98000
        }, /* UMAP0 */
        {
            .addr   = { -1u, -1u, 0x14E00000, 0x00E00000, 0x00E00000},
            .size   = 0x00008000
        }, /* L1P */
        {
            .addr   = { -1u, -1u, 0x14F00000, 0x00F00000, 0x00F00000},
            .size   = 0x00008000
        }, /* L1D */
    };

/* =============================================================================
 *  Globals
 * =============================================================================
 */
/*!
 *  @var    C6472CORE4PROC_state
 *
 *  @brief  C6472CORE4PROC state object variable
 */
#if !defined(SYSLINK_BUILD_DEBUG)
static
#endif /* if !defined(SYSLINK_BUILD_DEBUG) */
C6472CORE4PROC_ModuleObject C6472CORE4PROC_state =
{
    .isSetup                           = FALSE,
    .configSize                        = sizeof (C6472CORE4PROC_Config),
    .gateHandle                        = NULL,
};


/* =============================================================================
 * APIs directly called by applications
 * =============================================================================
 */
/*!
 *  @brief      Function to get the default configuration for the C6472CORE4PROC
 *              module.
 *
 *              This function can be called by the application to get their
 *              configuration parameter to C6472CORE4PROC_setup filled in by the
 *              C6472CORE4PROC module with the default parameters. If the user
 *              does not wish to make any change in the default parameters, this
 *              API is not required to be called.
 *
 *  @param      cfg        Pointer to the C6472CORE4PROC module configuration
 *                         structure in which the default config is to be
 *                         returned.
 *
 *  @sa         C6472CORE4PROC_setup
 */
Void
C6472CORE4PROC_getConfig (C6472CORE4PROC_Config * cfg)
{
    GT_1trace (curTrace, GT_ENTER, "C6472CORE4PROC_getConfig", cfg);

    GT_assert (curTrace, (cfg != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (cfg == NULL) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_getConfig",
                             PROCESSOR_E_INVALIDARG,
                             "Argument of type (C6472CORE4PROC_Config *) passed "
                             "is null!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        Memory_copy (cfg,
                     &(C6472CORE4PROC_state.defCfg),
                     sizeof (C6472CORE4PROC_Config));
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_0trace (curTrace, GT_LEAVE, "C6472CORE4PROC_getConfig");
}


/*!
 *  @brief      Function to setup the C6472CORE4PROC module.
 *
 *              This function sets up the C6472CORE4PROC module. This function
 *              must be called before any other instance-level APIs can be
 *              invoked.
 *              Module-level configuration needs to be provided to this
 *              function. If the user wishes to change some specific config
 *              parameters, then C6472CORE4PROC_getConfig can be called to get the
 *              configuration filled with the default values. After this, only
 *              the required configuration values can be changed. If the user
 *              does not wish to make any change in the default parameters, the
 *              application can simply call C6472CORE4PROC_setup with NULL
 *              parameters. The default parameters would get automatically used.
 *
 *  @param      cfg   Optional C6472CORE4PROC module configuration. If provided as
 *                    NULL, default configuration is used.
 *
 *  @sa         C6472CORE4PROC_destroy
 *              GateMutex_create
 */
Int
C6472CORE4PROC_setup (C6472CORE4PROC_Config * cfg)
{
    Int                   status = PROCESSOR_SUCCESS;
    C6472CORE4PROC_Config tmpCfg;
    Error_Block           eb;

    GT_1trace (curTrace, GT_ENTER, "C6472CORE4PROC_setup", cfg);

    Error_init(&eb);

    if (cfg == NULL) {
        C6472CORE4PROC_getConfig (&tmpCfg);
        cfg = &tmpCfg;
    }

    /* Create a default gate handle for local module protection. */
    C6472CORE4PROC_state.gateHandle = (IGateProvider_Handle)
                                GateMutex_create ((GateMutex_Params *)NULL, &eb);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (C6472CORE4PROC_state.gateHandle == NULL) {
        /*! @retval PROCESSOR_E_FAIL Failed to create GateMutex! */
        status = PROCESSOR_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_setup",
                             status,
                             "Failed to create GateMutex!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        /* Copy the user provided values into the state object. */
        Memory_copy (&C6472CORE4PROC_state.cfg,
                     cfg,
                     sizeof (C6472CORE4PROC_Config));

        /* Initialize the name to handles mapping array. */
        Memory_set (&C6472CORE4PROC_state.procHandles,
                    0,
                    (sizeof (C6472CORE4PROC_Handle) * MultiProc_MAXPROCESSORS));
        C6472CORE4PROC_state.isSetup = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_setup", status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return (status);
}


/*!
 *  @brief      Function to destroy the C6472CORE4PROC module.
 *
 *              Once this function is called, other C6472CORE4PROC module APIs,
 *              except for the C6472CORE4PROC_getConfig API cannot be called
 *              anymore.
 *
 *  @sa         C6472CORE4PROC_setup
 *              GateMutex_delete
 */
Int
C6472CORE4PROC_destroy (Void)
{
    Int    status = PROCESSOR_SUCCESS;
    UInt16 i;

    GT_0trace (curTrace, GT_ENTER, "C6472CORE4PROC_destroy");

    /* Check if any C6472CORE4PROC instances have not been deleted so far. If not,
     * delete them.
     */
    for (i = 0 ; i < MultiProc_MAXPROCESSORS ; i++) {
        GT_assert (curTrace, (C6472CORE4PROC_state.procHandles [i] == NULL));
        if (C6472CORE4PROC_state.procHandles [i] != NULL) {
            C6472CORE4PROC_delete (&(C6472CORE4PROC_state.procHandles [i]));
        }
    }

    if (C6472CORE4PROC_state.gateHandle != NULL) {
        GateMutex_delete ((GateMutex_Handle *)
                                &(C6472CORE4PROC_state.gateHandle));
    }

    C6472CORE4PROC_state.isSetup = FALSE;

    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_destroy", status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return (status);
}


/*!
 *  @brief      Function to initialize the parameters for this Processor
 *              instance.
 *
 *  @param      params  Configuration parameters to be returned
 *
 *  @sa         C6472CORE4PROC_create
 */
Void
C6472CORE4PROC_Params_init (C6472CORE4PROC_Handle   handle,
                          C6472CORE4PROC_Params * params)
{
    C6472CORE4PROC_Object * procObject = (C6472CORE4PROC_Object *) handle;

    GT_2trace (curTrace, GT_ENTER, "C6472CORE4PROC_Params_init", handle, params);

    GT_assert (curTrace, (params != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (params == NULL) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_Params_init",
                             PROCESSOR_E_INVALIDARG,
                             "Argument of type (C6472CORE4PROC_Params *) "
                             "passed is null!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        if (handle == NULL) {
            params->numMemEntries = 4;
            Memory_copy ((Ptr) params->memEntries,
                         C6472CORE4PROC_defaultMemRegions,
                         sizeof (C6472CORE4PROC_defaultMemRegions));
        }
        else {
            /* Return updated C6472CORE4PROC instance specific parameters. */
            Memory_copy (params,
                         &(procObject->params),
                         sizeof (C6472CORE4PROC_Params));
        }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_0trace (curTrace, GT_LEAVE, "C6472CORE4PROC_Params_init");
}

/*!
 *  @brief      Function to create an instance of this Processor.
 *
 *  @param      name    Name of the Processor instance.
 *  @param      params  Configuration parameters.
 *
 *  @sa         C6472CORE4PROC_delete
 */
C6472CORE4PROC_Handle
C6472CORE4PROC_create (      UInt16                procId,
                     const C6472CORE4PROC_Params * params)
{
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    Int                   status    = PROCESSOR_SUCCESS;
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    Processor_Object *    handle    = NULL;
    C6472CORE4PROC_Object * object    = NULL;
    IArg                  key;

    GT_2trace (curTrace, GT_ENTER, "C6472CORE4PROC_create", procId, params);

    GT_assert (curTrace, IS_VALID_PROCID (procId));
    GT_assert (curTrace, (params != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (!IS_VALID_PROCID (procId)) {
        /* Not setting status here since this function does not return status.*/
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_create",
                             PROCESSOR_E_INVALIDARG,
                             "Invalid procId specified");
    }
    else if (params == NULL) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_create",
                             PROCESSOR_E_INVALIDARG,
                             "params passed is NULL!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        /* Enter critical section protection. */
        key = IGateProvider_enter (C6472CORE4PROC_state.gateHandle);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        /* Check if the Processor already exists for specified procId. */
        if (C6472CORE4PROC_state.procHandles [procId] != NULL) {
            status = PROCESSOR_E_ALREADYEXIST;
            GT_setFailureReason (curTrace,
                              GT_4CLASS,
                              "C6472CORE4PROC_create",
                              status,
                              "Processor already exists for specified procId!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            /* Allocate memory for the handle */
            handle = (Processor_Object *) Memory_calloc (NULL,
                                                      sizeof (Processor_Object),
                                                      0,
                                                      NULL);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (handle == NULL) {
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "C6472CORE4PROC_create",
                                     PROCESSOR_E_MEMORY,
                                     "Memory allocation failed for handle!");
            }
            else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                /* Populate the handle fields */
                handle->procFxnTable.attach        = &C6472CORE4PROC_attach;
                handle->procFxnTable.detach        = &C6472CORE4PROC_detach;
                handle->procFxnTable.start         = &C6472CORE4PROC_start;
                handle->procFxnTable.stop          = &C6472CORE4PROC_stop;
                handle->procFxnTable.read          = &C6472CORE4PROC_read;
                handle->procFxnTable.write         = &C6472CORE4PROC_write;
                handle->procFxnTable.control       = &C6472CORE4PROC_control;
                handle->procFxnTable.map           = &C6472CORE4PROC_map;
                handle->procFxnTable.unmap         = &C6472CORE4PROC_unmap;
                handle->procFxnTable.translateAddr = &C6472CORE4PROC_translate;
                handle->state = ProcMgr_State_Unknown;

                /* Allocate memory for the C6472CORE4PROC handle */
                handle->object = Memory_calloc (NULL,
                                                sizeof (C6472CORE4PROC_Object),
                                                0,
                                                NULL);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                if (handle->object == NULL) {
                    status = PROCESSOR_E_MEMORY;
                    GT_setFailureReason (curTrace,
                                GT_4CLASS,
                                "C6472CORE4PROC_create",
                                status,
                                "Memory allocation failed for handle->object!");
                }
                else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                    handle->procId = procId;
                    object = (C6472CORE4PROC_Object *) handle->object;
                    /* Copy params into instance object. */
                    Memory_copy (&(object->params),
                                 (Ptr) params,
                                 sizeof (C6472CORE4PROC_Params));
                    /* Set the handle in the state object. */
                    C6472CORE4PROC_state.procHandles [procId] =
                                                   (C6472CORE4PROC_Handle) handle;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
                }
            }
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

        /* Leave critical section protection. */
        IGateProvider_leave (C6472CORE4PROC_state.gateHandle, key);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (status < 0) {
        if (handle !=  NULL) {
            if (handle->object != NULL) {
                Memory_free (NULL,
                             handle->object,
                             sizeof (C6472CORE4PROC_Object));
            }
            Memory_free (NULL, handle, sizeof (Processor_Object));
        }
        /*! @retval NULL Function failed */
        handle = NULL;
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_create", handle);

    /*! @retval Valid-Handle Operation successful */
    return (C6472CORE4PROC_Handle) handle;
}


/*!
 *  @brief      Function to delete an instance of this Processor.
 *
 *              The user provided pointer to the handle is reset after
 *              successful completion of this function.
 *
 *  @param      handlePtr  Pointer to Handle to the Processor instance
 *
 *  @sa         C6472CORE4PROC_create
 */
Int
C6472CORE4PROC_delete (C6472CORE4PROC_Handle * handlePtr)
{
    Int                   status = PROCESSOR_SUCCESS;
    C6472CORE4PROC_Object * object = NULL;
    Processor_Object *    handle;
    IArg                  key;

    GT_1trace (curTrace, GT_ENTER, "C6472CORE4PROC_delete", handlePtr);

    GT_assert (curTrace, (handlePtr != NULL));
    GT_assert (curTrace, ((handlePtr != NULL) && (*handlePtr != NULL)));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handlePtr == NULL) {
        /*! @retval PROCESSOR_E_INVALIDARG Invalid NULL handlePtr pointer
                                         specified*/
        status = PROCESSOR_E_INVALIDARG;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_delete",
                             status,
                             "Invalid NULL handlePtr pointer specified");
    }
    else if (*handlePtr == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid NULL *handlePtr specified */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_delete",
                             status,
                             "Invalid NULL *handlePtr specified");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        handle = (Processor_Object *) (*handlePtr);
        /* Enter critical section protection. */
        key = IGateProvider_enter (C6472CORE4PROC_state.gateHandle);

        /* Reset handle in PwrMgr handle array. */
        GT_assert (curTrace, IS_VALID_PROCID (handle->procId));
        C6472CORE4PROC_state.procHandles [handle->procId] = NULL;

        /* Free memory used for the C6472CORE4PROC object. */
        if (handle->object != NULL) {
            object = (C6472CORE4PROC_Object *) handle->object;
            Memory_free (NULL,
                         handle->object,
                         sizeof (C6472CORE4PROC_Object));
            handle->object = NULL;
        }

        /* Free memory used for the Processor object. */
        Memory_free (NULL, handle, sizeof (Processor_Object));
        *handlePtr = NULL;

        /* Leave critical section protection. */
        IGateProvider_leave (C6472CORE4PROC_state.gateHandle, key);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_delete", status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return (status);
}


/*!
 *  @brief      Function to open a handle to an instance of this Processor. This
 *              function is called when access to the Processor is required from
 *              a different process.
 *
 *  @param      handlePtr   Handle to the Processor instance
 *  @param      procId      Processor ID addressed by this Processor instance.
 *
 *  @sa         C6472CORE4PROC_close
 */
Int
C6472CORE4PROC_open (C6472CORE4PROC_Handle * handlePtr, UInt16 procId)
{
    Int status = PROCESSOR_SUCCESS;

    GT_2trace (curTrace, GT_ENTER, "C6472CORE4PROC_open", handlePtr, procId);

    GT_assert (curTrace, (handlePtr != NULL));
    GT_assert (curTrace, IS_VALID_PROCID (procId));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handlePtr == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid NULL handlePtr specified */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_open",
                             status,
                             "Invalid NULL handlePtr specified");
    }
    else if (!IS_VALID_PROCID (procId)) {
        /*! @retval PROCESSOR_E_INVALIDARG Invalid procId specified */
        status = PROCESSOR_E_INVALIDARG;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_open",
                             status,
                             "Invalid procId specified");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        /* Initialize return parameter handle. */
        *handlePtr = NULL;

        /* Check if the PwrMgr exists and return the handle if found. */
        if (C6472CORE4PROC_state.procHandles [procId] == NULL) {
            /*! @retval PROCESSOR_E_NOTFOUND Specified instance not found */
            status = PROCESSOR_E_NOTFOUND;
            GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_open",
                             status,
                             "Specified C6472CORE4PROC instance does not exist!");
        }
        else {
            *handlePtr = C6472CORE4PROC_state.procHandles [procId];
        }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_open", status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return status;
}


/*!
 *  @brief      Function to close a handle to an instance of this Processor.
 *
 *  @param      handlePtr  Pointer to Handle to the Processor instance
 *
 *  @sa         C6472CORE4PROC_open
 */
Int
C6472CORE4PROC_close (C6472CORE4PROC_Handle * handlePtr)
{
    Int status = PROCESSOR_SUCCESS;

    GT_1trace (curTrace, GT_ENTER, "C6472CORE4PROC_close", handlePtr);

    GT_assert (curTrace, (handlePtr != NULL));
    GT_assert (curTrace, ((handlePtr != NULL) && (*handlePtr != NULL)));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handlePtr == NULL) {
        /*! @retval PROCESSOR_E_INVALIDARG Invalid NULL handlePtr pointer
                                         specified*/
        status = PROCESSOR_E_INVALIDARG;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_close",
                             status,
                             "Invalid NULL handlePtr pointer specified");
    }
    else if (*handlePtr == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid NULL *handlePtr specified */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_close",
                             status,
                             "Invalid NULL *handlePtr specified");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        /* Nothing to be done for close. */
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_close", status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return status;
}


/* =============================================================================
 * APIs called by Processor module (part of function table interface)
 * =============================================================================
 */
/*!
 *  @brief      Function to initialize the slave processor
 *
 *  @param      handle  Handle to the Processor instance
 *  @param      params  Attach parameters
 *
 *  @sa         C6472CORE4PROC_detach
 */
Int
C6472CORE4PROC_attach (Processor_Handle handle, Processor_AttachParams * params)
{

    Int                   status       = PROCESSOR_SUCCESS ;
    Processor_Object *    procHandle   = (Processor_Object *) handle;
    C6472CORE4PROC_Object * object       = NULL;
    ProcMgr_AddrInfo *    me;

    GT_2trace (curTrace, GT_ENTER, "C6472CORE4PROC_attach", handle, params);
    GT_assert (curTrace, (handle != NULL));
    GT_assert (curTrace, (params != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handle == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid argument */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_attach",
                             status,
                             "Invalid handle specified");
    }
    else if (params == NULL) {
            /*! @retval PROCESSOR_E_INVALIDARG Invalid argument */
            status = PROCESSOR_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "C6472CORE4PROC_attach",
                                 status,
                                 "Invalid params specified");
    }
    else if ( (procHandle->bootMode != ProcMgr_BootMode_NoBoot) ) {
            status = PROCESSOR_E_INVALIDARG;
             GT_setFailureReason (curTrace,
                                  GT_4CLASS,
                                  "C6472CORE4PROC_attach",
                                  status,
                                  "BootMode not supported");
    } else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        object = (C6472CORE4PROC_Object *) procHandle->object;
        GT_assert (curTrace, (object != NULL));
        object->pmHandle = params->pmHandle;
        params->procArch = Processor_ProcArch_C64x;

        GT_0trace (curTrace,
                   GT_2CLASS,
                   "    C6472CORE4PROC_attach: Mapping memory regions\n");

        /* Populate the return params */
        me = object->params.memEntries;
        params->numMemEntries = object->params.numMemEntries;
        Memory_copy ((Ptr) params->memEntries,
                     (Ptr) object->params.memEntries,
                     sizeof (ProcMgr_AddrInfo) * params->numMemEntries);


#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_attach",status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return status;

}


/*!
 *  @brief      Function to detach from the Processor.
 *
 *  @param      handle  Handle to the Processor instance
 *
 *  @sa         C6472CORE4PROC_attach
 */
Int
C6472CORE4PROC_detach (Processor_Handle handle)
{
    Int                   status       = PROCESSOR_SUCCESS;
    Int                   tmpStatus    = PROCESSOR_SUCCESS;
    Processor_Object *    procHandle   = (Processor_Object *) handle;
    C6472CORE4PROC_Object * object       = NULL;

    GT_1trace (curTrace, GT_ENTER, "C6472CORE4PROC_detach", handle);

    GT_assert (curTrace, (handle != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handle == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid argument */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_detach",
                             PROCESSOR_E_HANDLE,
                             "Invalid handle specified");
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_detach", status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return status;
}


/*!
 *  @brief      Function to start the slave processor
 *
 *              Start the slave processor running from its entry point.
 *              Depending on the boot mode, this involves configuring the boot
 *              address and releasing the slave from reset.
 *
 *  @param      handle    Handle to the Processor instance
 *
 *  @sa         C6472CORE4PROC_stop, C6472CORE4_halBootCtrl, C6472CORE4_halResetCtrl
 */
Int
C6472CORE4PROC_start (Processor_Handle        handle,
                    UInt32                  entryPt,
                    Processor_StartParams * params)
{

    GT_3trace (curTrace, GT_ENTER, "C6472CORE4PROC_start",
               handle, entryPt, params);

    /* Nothing to do here, since this device does not support Boot mode. */

    return PROCESSOR_E_NOTSUPPORTED;
}


/*!
 *  @brief      Function to stop the slave processor
 *
 *              Stop the execution of the slave processor. Depending on the boot
 *              mode, this may result in placing the slave processor in reset.
 *
 *  @param      handle    Handle to the Processor instance
 *
 *  @sa         C6472CORE4PROC_start, C6472CORE4_halResetCtrl
 */
Int
C6472CORE4PROC_stop (Processor_Handle handle)
{

    GT_1trace (curTrace, GT_ENTER, "C6472CORE4PROC_stop", handle);

    /* Nothing to do here, since this device does not support Boot mode. */

    return PROCESSOR_E_NOTSUPPORTED;
}


/*!
 *  @brief      Function to read from the slave processor's memory.
 *
 *              Read from the slave processor's memory and copy into the
 *              provided buffer.
 *
 *  @param      handle     Handle to the Processor instance
 *  @param      procAddr   Address in host processor's address space of the
 *                         memory region to read from.
 *  @param      numBytes   IN/OUT parameter. As an IN-parameter, it takes in the
 *                         number of bytes to be read. When the function
 *                         returns, this parameter contains the number of bytes
 *                         actually read.
 *  @param      buffer     User-provided buffer in which the slave processor's
 *                         memory contents are to be copied.
 *
 *  @sa         C6472CORE4PROC_write
 */
Int
C6472CORE4PROC_read (Processor_Handle   handle,
                   UInt32             procAddr,
                   UInt32 *           numBytes,
                   Ptr                buffer)
{
    Int       status   = PROCESSOR_SUCCESS ;
    UInt8  *  procPtr8 = NULL;

    GT_4trace (curTrace, GT_ENTER, "C6472CORE4PROC_read",
               handle, procAddr, numBytes, buffer);

    GT_assert (curTrace, (handle   != NULL));
    GT_assert (curTrace, (numBytes != NULL));
    GT_assert (curTrace, (buffer   != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handle == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid argument */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_read",
                             status,
                             "Invalid handle specified");
    }
    else if (numBytes == 0) {
            /*! @retval PROCESSOR_E_INVALIDARG Invalid argument */
            status = PROCESSOR_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "C6472CORE4PROC_read",
                                 status,
                                 "Invalid numBytes specified");
    }
    else if (buffer == NULL) {
            /*! @retval PROCESSOR_E_INVALIDARG Invalid argument */
            status = PROCESSOR_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "C6472CORE4PROC_read",
                                 status,
                                 "Invalid buffer specified");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        procPtr8 = (UInt8 *) procAddr ;
        buffer = Memory_copy (buffer, procPtr8, *numBytes);
        GT_assert (curTrace, (buffer != (UInt32) NULL));
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (buffer == (UInt32) NULL) {
            /*! @retval PROCESSOR_E_FAIL Failed in Memory_copy */
            status = PROCESSOR_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "C6472CORE4PROC_read",
                                 status,
                                 "Failed in Memory_copy");
            *numBytes = 0;
        }
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_read",status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return status;
}


/*!
 *  @brief      Function to write into the slave processor's memory.
 *
 *              Read from the provided buffer and copy into the slave
 *              processor's memory.
 *
 *  @param      handle     Handle to the Processor object
 *  @param      procAddr   Address in host processor's address space of the
 *                         memory region to write into.
 *  @param      numBytes   IN/OUT parameter. As an IN-parameter, it takes in the
 *                         number of bytes to be written. When the function
 *                         returns, this parameter contains the number of bytes
 *                         actually written.
 *  @param      buffer     User-provided buffer from which the data is to be
 *                         written into the slave processor's memory.
 *
 *  @sa         C6472CORE4PROC_read, C6472CORE4PROC_translateAddr
 */
Int
C6472CORE4PROC_write (Processor_Handle handle,
                    UInt32           procAddr,
                    UInt32 *         numBytes,
                    Ptr              buffer)
{
    Int                   status       = PROCESSOR_SUCCESS ;
    Processor_Object *    procHandle   = (Processor_Object *) handle;
    C6472CORE4PROC_Object * object       = NULL;
    UInt8  *              procPtr8     = NULL;
    UInt8                 temp8_1;
    UInt8                 temp8_2;
    UInt8                 temp8_3;
    UInt8                 temp8_4;
    UInt32                temp;

    GT_4trace (curTrace, GT_ENTER, "C6472CORE4PROC_write",
               handle, procAddr, numBytes, buffer);

    Osal_printf ("C6472CORE4PROC_write, buffer = 0x%x, numBytes = %d\n",
		buffer, numBytes);
    GT_assert (curTrace, (handle   != NULL));
    GT_assert (curTrace, (numBytes != NULL));
    GT_assert (curTrace, (buffer   != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handle == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid argument */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_write",
                             status,
                             "Invalid handle specified");
    }
    else if (numBytes == 0) {
            /*! @retval PROCESSOR_E_INVALIDARG Invalid argument */
            status = PROCESSOR_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "C6472CORE4PROC_write",
                                 status,
                                 "Invalid numBytes specified");
    }
    else if (buffer == NULL) {
            /*! @retval PROCESSOR_E_INVALIDARG Invalid argument */
            status = PROCESSOR_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "C6472CORE4PROC_write",
                                 status,
                                 "Invalid buffer specified");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        object = (C6472CORE4PROC_Object *) procHandle->object;
        GT_assert (curTrace, (object != NULL));
        if (*numBytes != sizeof (UInt32)) {
            procPtr8 = (UInt8 *) procAddr ;
            procAddr = (UInt32) Memory_copy (procPtr8,
                                             buffer,
                                             *numBytes);
            GT_assert (curTrace, (procAddr != (UInt32) NULL));
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (procAddr == (UInt32) NULL) {
                /*! @retval PROCESSOR_E_FAIL Failed in Memory_copy */
                status = PROCESSOR_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "C6472CORE4PROC_write",
                                     status,
                                     "Failed in Memory_copy");
                *numBytes = 0;
            }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }
        else  {
             /* For 4 bytes, directly write as a UInt32 */
            temp8_1 = ((UInt8 *) buffer) [0];
            temp8_2 = ((UInt8 *) buffer) [1];
            temp8_3 = ((UInt8 *) buffer) [2];
            temp8_4 = ((UInt8 *) buffer) [3];
            temp = (UInt32) (    ((UInt32) temp8_4 << 24)
                             |   ((UInt32) temp8_3 << 16)
                             |   ((UInt32) temp8_2 << 8)
                             |   ((UInt32) temp8_1));
            *((UInt32*) procAddr) = temp;
        }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_write",status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return status;
}


/*!
 *  @brief      Function to perform device-dependent operations.
 *
 *              Performs device-dependent control operations as exposed by this
 *              implementation of the Processor module.
 *
 *  @param      handle     Handle to the Processor object
 *  @param      cmd        Device specific processor command
 *  @param      arg        Arguments specific to the type of command.
 *
 *  @sa
 */
Int
C6472CORE4PROC_control (Processor_Handle handle, Int32 cmd, Ptr arg)
{
    Int                   status       = PROCESSOR_SUCCESS ;
    Processor_Object *    procHandle   = (Processor_Object *) handle;
    C6472CORE4PROC_Object * object       = NULL;

    GT_3trace (curTrace, GT_ENTER, "C6472CORE4PROC_control", handle, cmd, arg);

    GT_assert (curTrace, (handle   != NULL));
    /* cmd and arg can be 0/NULL, so cannot check for validity. */

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handle == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid argument */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_control",
                             status,
                             "Invalid handle specified");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        object = (C6472CORE4PROC_Object *) procHandle->object;
        GT_assert (curTrace, (object != NULL));
        /* No control operations currently implemented. */
        /*! @retval PROCESSOR_E_NOTSUPPORTED No control operations are supported
                                             for this device. */
        status = PROCESSOR_E_NOTSUPPORTED;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_control",status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return status;
}


/*!
 *  @brief      Function to translate slave physical address to master physical
 *              address.
 *
 *  @param      handle     Handle to the Processor object
 *  @param      dstAddr    Returned: master physical address.
 *  @param      srcAddr    Slave physical address.
 *
 *  @sa
 */
Int
C6472CORE4PROC_translate (Processor_Handle handle,
                            UInt32 *         dstAddr,
                            UInt32           srcAddr)
{
    Int                       status       = PROCESSOR_SUCCESS ;
    Processor_Object *        procHandle   = (Processor_Object *) handle;
    C6472CORE4PROC_Object * object       = NULL;
    UInt32                    i;
    ProcMgr_AddrInfo *        ai;

    GT_3trace (curTrace, GT_ENTER, "C6472CORE4PROC_translate",
               handle, dstAddr, srcAddr);

    GT_assert (curTrace, (handle != NULL));
    GT_assert (curTrace, (dstAddr != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handle == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid argument */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_translate",
                             status,
                             "Invalid handle specified");
    }
    else if (dstAddr == NULL) {
        /*! @retval PROCESSOR_E_INVALIDARG sglist provided as NULL */
        status = PROCESSOR_E_INVALIDARG;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_translate",
                             status,
                             "dstAddr provided as NULL");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        object = (C6472CORE4PROC_Object *) procHandle->object;
        GT_assert (curTrace, (object != NULL));

        *dstAddr = -1u;
        for (i = 0;
             i < (  sizeof (C6472CORE4PROC_defaultMemRegions)
                  / sizeof (ProcMgr_AddrInfo));
             i++) {
             ai = &C6472CORE4PROC_defaultMemRegions [i];
             if (   (srcAddr >= ai->addr [ProcMgr_AddrType_SlavePhys])
                 && (srcAddr < (  ai->addr [ProcMgr_AddrType_SlavePhys]
                                + ai->size))) {
                 *dstAddr =   ai->addr [ProcMgr_AddrType_MasterPhys]
                            + (srcAddr - ai->addr [ProcMgr_AddrType_SlavePhys]);
                 break;
             }
        }

        /* DDR */
        if ((srcAddr >= DDR2_START) && (srcAddr < DDR2_END)) {
            *dstAddr = srcAddr;
        }

        if (*dstAddr == -1u) {
            /*! @retval PROCESSOR_E_FAIL srcAddr not found in slave address
             *          space */
            status = PROCESSOR_E_INVALIDARG;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "C6472CORE4PROC_translate",
                                 status,
                                 "srcAddr not found in slave address space");
        }

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_translate",status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return status;
}


/*!
 *  @brief      Function to map slave address to host address space
 *
 *              Map the provided slave address to master address space. This
 *              function also maps the specified address to slave MMU space.
 *
 *  @param      handle      Handle to the Processor object
 *  @param      mapType     Type of mapping to be performed.
 *  @param      addrInfo    Structure containing map info.
 *  @param      srcAddrType Source address type.
 *
 *  @sa
 */
Int
C6472CORE4PROC_map (Processor_Handle handle,
                  UInt32 *         dstAddr,
                  UInt32           nSegs,
                  Memory_SGList *  sglist)
{
    Int                   status       = PROCESSOR_SUCCESS ;
    Processor_Object *    procHandle   = (Processor_Object *) handle;
    C6472CORE4PROC_Object * object       = NULL;

    GT_4trace (curTrace, GT_ENTER, "C6472CORE4PROC_map",
               handle, dstAddr, nSegs, sglist);

    GT_assert (curTrace, (handle != NULL));
    GT_assert (curTrace, (sglist != NULL));
    GT_assert (curTrace, (nSegs > 0));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handle == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid argument */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_map",
                             status,
                             "Invalid handle specified");
    }
    else if (sglist == NULL) {
        /*! @retval PROCESSOR_E_INVALIDARG sglist provided as NULL */
        status = PROCESSOR_E_INVALIDARG;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_map",
                             status,
                             "sglist provided as NULL");
    }
    else if (nSegs == 0) {
        /*! @retval PROCESSOR_E_INVALIDARG Number of segments provided is 0 */
        status = PROCESSOR_E_INVALIDARG;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_map",
                             status,
                             "Number of segments provided is 0");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        object = (C6472CORE4PROC_Object *) procHandle->object;
        GT_assert (curTrace, (object != NULL));
        /* Program the mmu with the sglist */
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_map",status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return status;
}


/*!
 *  @brief      Function to unmap slave address from host address space
 *
 *  @param      handle      Handle to the Processor object
 *  @param      dstAddr     Return parameter: Pointer to receive the mapped
 *                          address.
 *  @param      size        Size of the region to be mapped.
s *
 *  @sa
 */
Int
C6472CORE4PROC_unmap (Processor_Handle handle,
                    UInt32           addr,
                    UInt32           size)
{
    Int                   status       = PROCESSOR_SUCCESS ;
    Processor_Object *    procHandle   = (Processor_Object *) handle;
    C6472CORE4PROC_Object * object       = NULL;

    GT_3trace (curTrace, GT_ENTER, "C6472CORE4PROC_unmap",
               handle, addr, size);

    GT_assert (curTrace, (handle != NULL));
    GT_assert (curTrace, (size   != 0));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (handle == NULL) {
        /*! @retval PROCESSOR_E_HANDLE Invalid argument */
        status = PROCESSOR_E_HANDLE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_unmap",
                             status,
                             "Invalid handle specified");
    }
    else if (size == 0) {
        /*! @retval  PROCESSOR_E_INVALIDARG Size provided is zero */
        status = PROCESSOR_E_INVALIDARG;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472CORE4PROC_unmap",
                             status,
                             "Size provided is zero");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        object = (C6472CORE4PROC_Object *) procHandle->object;
        GT_assert (curTrace, (object != NULL));
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    GT_1trace (curTrace, GT_LEAVE, "C6472CORE4PROC_unmap",status);

    /*! @retval PROCESSOR_SUCCESS Operation successful */
    return status;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
