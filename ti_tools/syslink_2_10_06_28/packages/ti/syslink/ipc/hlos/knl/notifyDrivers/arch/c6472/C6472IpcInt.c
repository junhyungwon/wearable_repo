/*
 *  @file   C6472IpcInt.c
 *
 *  @brief      C6472 DSP IPC interrupts.
 *              Defines necessary functions for Interrupt Handling.
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

/* OSAL headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/Cache.h>
#include <Bitops.h>

/* Utils headers */
#include <ti/ipc/MultiProc.h>
#include <_MultiProc.h>
#include <ti/syslink/utils/Memory.h>

/* Hardware Abstraction Layer */
#include <_ArchIpcInt.h>
#include <_C6472IpcInt.h>
#include <C6472IpcInt.h>


#if defined (__cplusplus)
extern "C" {
#endif


/* =============================================================================
 *  Macros and types
 * =============================================================================
 */

/*!
 *  @def    C6472_NUMPROCS
 *  @brief  Number of processors supported on this platform
 */
#define C6472_NUMPROCS 6
/*!
 *  @def    C6472_INDEX_CORE0
 *  @brief  core0 index.
 */
#define C6472_INDEX_CORE0 0
/*!
 *  @def    C6472_INDEX_CORE1 
 *  @brief  core1 index.
 */
#define C6472_INDEX_CORE1 1
/*!
 *  @def    C6472_INDEX_CORE2
 *  @brief  core2 index.
 */
#define C6472_INDEX_CORE2 2

/*!
 *  @def    C6472_INDEX_CORE3
 *  @brief  core3 index.
 */
#define C6472_INDEX_CORE3 3
/*!
 *  @def    C6472_INDEX_CORE4 
 *  @brief  core4 index.
 */
#define C6472_INDEX_CORE4 4
/*!
 *  @def    C6472_INDEX_CORE5
 *  @brief  core5 index.
 */
#define C6472_INDEX_CORE5 5

/* Macro to make a correct module magic number with refCount */
#define C6472IPCINT_MAKE_MAGICSTAMP(x) \
                                    ((C6472IPCINT_MODULEID << 12u) | (x))

/*!
 *  @def    REG
 *  @brief  Regsiter access method.
 */
#define REG(x)          *((volatile UInt32 *) (x))
#define REG32(x)        (*(volatile UInt32 *) (x))

/* Register access method. */
#define REG16(A)        (*(volatile UInt16 *) (A))

/*!
 *  @def    IPCGRX_BASE
 *  @brief  IPC interrupt generation registers base address
 */
#define IPCGRX_BASE                 0x02A80540

/*!
 *  @def    IPCGRX_SIZE
 *  @brief  Size of IPCGRx register space
 */
#define IPCGRX_SIZE            24

/*!
 *  @def    IPCGR_OFFSET
 *  @brief  IPCGRn register offset
 */
#define IPCGR_OFFSET(procId)          (4*procId)

/*!
 *  @def    IPCARX_BASE
 *  @brief  IPC interrupt acklowledge register
 */
#define IPCARX_BASE            0x02A80580

/*!
 *  @def    IPCARX_SIZE
 *  @brief  Size of IPCGR0 register space
 */
#define IPCARX_SIZE            24

/*!
 *  @def    IPCAR_OFFSET
 *  @brief  IPCARn register offset
 */
#define IPCAR_OFFSET(procId)          (4*procId)

/*!
 *  @def    SRCSX_SHIFT
 *  @brief  shift value used for setting/identifying interrupt source 
 */
#define SRCSX_SHIFT            4

/*!
 *  @brief  Device specific object
 *          It can be populated as per device need and it is used internally in
 *          the device specific implementation only.
 */
typedef struct C6472IpcInt_Object_tag {
    Atomic                 isrRefCount;
    /*!< ISR Reference count */
    Atomic                   asserted;
    /*!< Indicates receipt of interrupt from particular processor */
    UInt32                 recvIntId;
    /*!<recevive interrupt id */
    ArchIpcInt_CallbackFxn fxn;
    /*!< Callbck function to be registered for particular instance of driver*/
    Ptr                    fxnArgs;
    /*!< Argument to the call back function */
} C6472IpcInt_Object;


/*!
 *  @brief  Device specific object
 *          It can be populated as per device need and it is used internally in
 *          the device specific implementation only.
 */
typedef struct C6472IpcInt_ModuleObject_tag {
    Atomic             isrRefCount;
    /*!< ISR Reference count */
    OsalIsr_Handle     isrHandle;
    /*!< Handle to the OsalIsr object */
    UInt16             procIds [C6472_NUMPROCS];
    /*!< Processors supported */
    UInt16             maxProcessors;
    /*!< Maximum number of processors supported by this platform*/
    C6472IpcInt_Object isrObjects [MultiProc_MAXPROCESSORS];
    /*!< Array of Isr objects */
    UInt32         ipcgrx_base;
    /*!< IPCGRX register address */
    UInt32         ipcarx_base;
    /*!< IPCARX register address */
} C6472IpcInt_ModuleObject;



/* =============================================================================
 * Forward declarations of internal functions.
 * =============================================================================
 */
/* This function implements the interrupt service routine for the interrupt
 * received from the remote processor.
 */
static Bool _C6472IpcInt_isr (Ptr ref);

/*!
 *  @brief  Forward declaration of check and clear function
 */
static Bool _C6472IpcInt_checkAndClearFunc (Ptr arg);


/* =============================================================================
 *  Globals
 * =============================================================================
 */
/*!
 *  @brief  State object for C6472IpcInt
 */
C6472IpcInt_ModuleObject C6472IpcInt_state;

/*!
 *  @brief  Function table for C6472
 */
ArchIpcInt_FxnTable C6472IpcInt_fxnTable = {
    C6472IpcInt_interruptRegister,
    C6472IpcInt_interruptUnregister,
    C6472IpcInt_interruptEnable,
    C6472IpcInt_interruptDisable,
    C6472IpcInt_waitClearInterrupt,
    C6472IpcInt_sendInterrupt,
    C6472IpcInt_clearInterrupt,
};


/* =============================================================================
 *  APIs
 * =============================================================================
 */

/*!
 *  @brief      Function to initialize the C6472IpcInt module.
 *
 *  @param      cfg  Configuration for setup
 *
 *  @sa         C6472IpcInt_destroy
 */
Void
C6472IpcInt_setup (C6472IpcInt_Config * cfg)
{
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    Int            status = C6472IPCINT_SUCCESS;
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    Int i = 0;
    Memory_MapInfo mapInfo;

    GT_1trace (curTrace, GT_ENTER, "C6472IpcInt_setup", cfg);

    GT_assert (curTrace, (cfg != NULL));

    /* The setup will be called only once, either from SysMgr or from
     * archipcevmx6472 module. Hence it does not need to be atomic.
     */
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (cfg == NULL) {
        GT_setFailureReason (curTrace,
                        GT_4CLASS,
                        "C6472IpcInt_setup",
                        C6472IPCINT_E_FAIL,
                        "config for driver specific setup can not be NULL");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

        /* Map general control base */
        mapInfo.src      = IPCGRX_BASE;
        mapInfo.size     = IPCGRX_SIZE;
        mapInfo.isCached = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        status =
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        Memory_map (&mapInfo);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "C6472IpcInt_setup",
                                 status,
                                 "Failure in Memory_map for IPCGRX");
            C6472IpcInt_state.ipcgrx_base = 0;
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            C6472IpcInt_state.ipcgrx_base = mapInfo.dst;
            /* Map mailboxBase */
            mapInfo.src      = IPCARX_BASE;
            mapInfo.size     = IPCARX_SIZE;
            mapInfo.isCached = FALSE;
 #if !defined(SYSLINK_BUILD_OPTIMIZE)
            status =
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                Memory_map (&mapInfo);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (status < 0) {
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "C6472IpcInt_setup",
                                     status,
                                     "Failure in Memory_map for IPCARX");
                C6472IpcInt_state.ipcarx_base = 0;
            }
            else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                C6472IpcInt_state.ipcarx_base = mapInfo.dst;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            }
        }
        if (status >= 0) {
            /* Registering C6472 platform with ArchIpcInt*/
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            ArchIpcInt_object.fxnTable = &C6472IpcInt_fxnTable;
            ArchIpcInt_object.obj      = &C6472IpcInt_state;

            for (i = 0; i < MultiProc_getNumProcessors(); i++ ) {
                Atomic_set (&(C6472IpcInt_state.isrObjects [i].asserted), 0);
            }

            /* Calling MultiProc APIs here in setup save time in ISR and makes
             * it small and fast with less overhead.
             */
            C6472IpcInt_state.procIds [C6472_INDEX_CORE0] = MultiProc_getId ("CORE0");
            C6472IpcInt_state.procIds [C6472_INDEX_CORE1] = MultiProc_getId ("CORE1");
            C6472IpcInt_state.procIds [C6472_INDEX_CORE2] = MultiProc_getId ("CORE2");
            C6472IpcInt_state.procIds [C6472_INDEX_CORE3] = MultiProc_getId ("CORE3");
            C6472IpcInt_state.procIds [C6472_INDEX_CORE4] = MultiProc_getId ("CORE4");
            C6472IpcInt_state.procIds [C6472_INDEX_CORE5] = MultiProc_getId ("CORE5");
            C6472IpcInt_state.maxProcessors = MultiProc_getNumProcessors();

            ArchIpcInt_object.isSetup  = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_0trace (curTrace, GT_LEAVE, "C6472IpcInt_setup");
}


/*!
 *  @brief      Function to finalize the C6472IpcInt module
 *
 *  @sa         C6472IpcInt_setup
 */
Void
C6472IpcInt_destroy (Void)
{
    Memory_UnmapInfo unmapInfo;

    GT_0trace (curTrace, GT_ENTER, "C6472IpcInt_destroy");

    GT_assert (curTrace,(ArchIpcInt_object.isSetup == TRUE));

    ArchIpcInt_object.isSetup  = FALSE;
    ArchIpcInt_object.obj      = NULL;
    ArchIpcInt_object.fxnTable = NULL;

    if (C6472IpcInt_state.ipcgrx_base != (UInt32) NULL) {
        unmapInfo.addr = C6472IpcInt_state.ipcgrx_base;
        unmapInfo.size = IPCGRX_SIZE;
        unmapInfo.isCached = FALSE;
        Memory_unmap (&unmapInfo);
        C6472IpcInt_state.ipcgrx_base = (UInt32) NULL;
    }

    if (C6472IpcInt_state.ipcarx_base != (UInt32) NULL) {
        unmapInfo.addr = C6472IpcInt_state.ipcarx_base;
        unmapInfo.size = IPCARX_SIZE;
        unmapInfo.isCached = FALSE;
        Memory_unmap (&unmapInfo);
        C6472IpcInt_state.ipcarx_base = (UInt32) NULL;
    }

    GT_0trace (curTrace, GT_ENTER, "C6472IpcInt_destroy");
}


/*!
 *  @brief      Function to register the interrupt.
 *
 *  @param      procId  destination procId.
 *  @param      intId   interrupt id.
 *  @param      fxn     callback funxction to be called on receiving interrupt.
 *  @param      fxnArgs arguments to the callback function.
 *
 *  @sa         C6472IpcInt_interruptEnable
 */

Int32
C6472IpcInt_interruptRegister  (UInt16                     procId,
                                UInt32                     intId,
                                ArchIpcInt_CallbackFxn     fxn,
                                Ptr                        fxnArgs)
{
    Int32 status = C6472IPCINT_SUCCESS;
    OsalIsr_Params isrParams;

    GT_4trace (curTrace,
               GT_ENTER,
               "C6472IpcInt_interruptRegister",
               procId,
               intId,
               fxn,
               fxnArgs);

    GT_assert (curTrace,(ArchIpcInt_object.isSetup == TRUE));
    GT_assert(curTrace, (procId < MultiProc_MAXPROCESSORS));
    GT_assert(curTrace, (fxn != NULL));


    /* This sets the refCount variable is not initialized, upper 16 bits is
     * written with module Id to ensure correctness of refCount variable.
     */
    Atomic_cmpmask_and_set (&C6472IpcInt_state.isrObjects [procId].isrRefCount,
                            C6472IPCINT_MAKE_MAGICSTAMP(0),
                            C6472IPCINT_MAKE_MAGICSTAMP(0));

    /* This is a normal use-case, so should not be inside
     * SYSLINK_BUILD_OPTIMIZE.
     */
    if (Atomic_inc_return (&C6472IpcInt_state.isrObjects [procId].isrRefCount)
        != C6472IPCINT_MAKE_MAGICSTAMP(1u)) {
        /*! @retval C6472IPCINT_S_ALREADYREGISTERED ISR already registered! */
        status = C6472IPCINT_S_ALREADYREGISTERED;
        GT_0trace (curTrace,
                   GT_2CLASS,
                   "ISR already registered!");
    }
    else {
        C6472IpcInt_state.isrObjects [procId].fxn       = fxn;
        C6472IpcInt_state.isrObjects [procId].fxnArgs   = fxnArgs;
        C6472IpcInt_state.isrObjects [procId].recvIntId = intId;
    }

    Atomic_cmpmask_and_set (&C6472IpcInt_state.isrRefCount,
                            C6472IPCINT_MAKE_MAGICSTAMP(0),
                            C6472IPCINT_MAKE_MAGICSTAMP(0));

    /* This is a normal use-case, so should not be inside
     * SYSLINK_BUILD_OPTIMIZE.
     */
    if (   Atomic_inc_return (&C6472IpcInt_state.isrRefCount)
        != C6472IPCINT_MAKE_MAGICSTAMP(1u)) {
        /*! @retval C6472IPCINT_S_ALREADYREGISTERED Generic ISR already set! */
        status = C6472IPCINT_S_ALREADYREGISTERED;
        GT_0trace (curTrace,
                   GT_2CLASS,
                   "Generic ISR already set !");
    }
    else {
        isrParams.sharedInt        = FALSE;
        isrParams.checkAndClearFxn = &_C6472IpcInt_checkAndClearFunc;
        isrParams.fxnArgs          = NULL;
        isrParams.intId            = intId;

        C6472IpcInt_state.isrHandle = OsalIsr_create (&_C6472IpcInt_isr,
                                                      NULL,
                                                      &isrParams);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (C6472IpcInt_state.isrHandle == NULL) {
            /*! @retval C6472IPCINT_E_FAIL OsalIsr_create failed */
            status = C6472IPCINT_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "C6472IpcInt_interruptRegister",
                                 status,
                                 "OsalIsr_create failed");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            status = OsalIsr_install (C6472IpcInt_state.isrHandle);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (status < 0) {
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "C6472IpcInt_interruptRegister",
                                     status,
                                     "OsalIsr_install failed");
            }
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    GT_1trace (curTrace, GT_LEAVE, "C6472IpcInt_interruptRegister", status);

    /*! @retval C6472IPCINT_SUCCESS Interrupt successfully registered */
    return status;
}

/*!
 *  @brief      Function to unregister interrupt.
 *
 *  @param      procId  destination procId
 *
 *  @sa         C6472IpcInt_interruptRegister
 */
Int32
C6472IpcInt_interruptUnregister  (UInt16 procId)
{
    Int32 status = C6472IPCINT_SUCCESS;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    Int32 tmpStatus = C6472IPCINT_SUCCESS;
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace,GT_ENTER,"C6472IpcInt_interruptUnregister", procId);

    GT_assert (curTrace,(ArchIpcInt_object.isSetup == TRUE));
    GT_assert(curTrace, (procId < MultiProc_MAXPROCESSORS));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (   Atomic_cmpmask_and_lt (
                            &C6472IpcInt_state.isrObjects [procId].isrRefCount,
                            C6472IPCINT_MAKE_MAGICSTAMP(0),
                            C6472IPCINT_MAKE_MAGICSTAMP(1))
        == TRUE) {
        /*! @retval C6472IPCINT_E_INVALIDSTATE ISR was not registered */
        status = C6472IPCINT_E_INVALIDSTATE;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472IpcInt_interruptUnregister",
                             status,
                             "ISR was not registered!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        /* This is a normal use-case, so should not be inside
         * SYSLINK_BUILD_OPTIMIZE.
         */
        if (Atomic_dec_return(&C6472IpcInt_state.isrObjects[procId].isrRefCount)
            == C6472IPCINT_MAKE_MAGICSTAMP(0)) {
            C6472IpcInt_state.isrObjects [procId].fxn       = NULL;
            C6472IpcInt_state.isrObjects [procId].fxnArgs   = NULL;
            C6472IpcInt_state.isrObjects [procId].recvIntId = -1u;
        }
        if (   Atomic_dec_return (&C6472IpcInt_state.isrRefCount)
            == C6472IPCINT_MAKE_MAGICSTAMP(0)) {
            status = OsalIsr_uninstall (C6472IpcInt_state.isrHandle);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (status < 0) {
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "C6472IpcInt_interruptUnregister",
                                     status,
                                     "OsalIsr_uninstall failed");
            }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

#if !defined(SYSLINK_BUILD_OPTIMIZE)
            tmpStatus =
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                OsalIsr_delete (&(C6472IpcInt_state.isrHandle));
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if ((status >= 0) && (tmpStatus < 0)) {
                status = tmpStatus;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "C6472IpcInt_interruptUnregister",
                                     status,
                                     "OsalIsr_delete failed");
            }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_LEAVE, "C6472IpcInt_interruptUnregister",
               status);

    /*! @retval C6472IPCINT_SUCCESS Interrupt successfully unregistered */
    return status;
}

/*!
 *  @brief      Function to enable the specified interrupt
 *
 *  @param      procId  Remote processor ID
 *  @param      intId   interrupt id
 *
 *  @sa         C6472IpcInt_interruptDisable
 */
Void
C6472IpcInt_interruptEnable (UInt16 procId, UInt32 intId)
{
    GT_2trace (curTrace, GT_ENTER, "C6472IpcInt_interruptEnable",
               procId, intId);

    GT_assert (curTrace,(ArchIpcInt_object.isSetup == TRUE));
    GT_assert (curTrace, (procId < MultiProc_MAXPROCESSORS));

    GT_assert (curTrace, (C6472IpcInt_state.isrHandle != NULL));
    OsalIsr_enableIsr (C6472IpcInt_state.isrHandle);

    GT_0trace (curTrace, GT_LEAVE, "C6472IpcInt_interruptEnable");
}


/*!
 *  @brief      Function to disable the specified interrupt
 *
 *  @param      procId  Remote processor ID
 *  @param      intId   interrupt id
 *
 *  @sa         C6472IpcInt_interruptEnable
 */
Void
C6472IpcInt_interruptDisable (UInt16 procId, UInt32 intId)
{
    GT_2trace (curTrace, GT_ENTER, "C6472IpcInt_interruptDisable",
               procId, intId);

    GT_assert (curTrace,(ArchIpcInt_object.isSetup == TRUE));
    GT_assert (curTrace, (procId < MultiProc_MAXPROCESSORS));

    GT_assert (curTrace, (C6472IpcInt_state.isrHandle != NULL));
    OsalIsr_disableIsr (C6472IpcInt_state.isrHandle);

    GT_0trace (curTrace, GT_LEAVE, "C6472IpcInt_interruptDisable");
}

/*!
 *  @brief      Function to wait for interrupt to be cleared.
 *
 *  @param      procId  Remote processor ID
 *  @param      intId   interrupt id
 *
 *  @sa         C6472IpcInt_waitClearInterrupt
 */
Int32
C6472IpcInt_waitClearInterrupt (UInt16 procId, UInt32 intId)
{
    Int32 status = C6472IPCINT_SUCCESS;
    extern volatile cregister unsigned int DNUM;
    UInt32 offset = 0, mask;

    /*
     * bit 4-6 of the IPCAR register indicates the status of the remote/local
     * IPC interrupt. Check DNUM to see which IPCAR to use
     */
    offset = IPCGR_OFFSET (procId);

    GT_2trace (curTrace,GT_ENTER,"C6472IpcInt_waitClearInterrupt",
               procId, intId);

    GT_assert (curTrace,(ArchIpcInt_object.isSetup == TRUE));
    GT_assert (curTrace, (procId < MultiProc_MAXPROCESSORS));

    /* Check bit [4+procId] */
    mask = (1 << (SRCSX_SHIFT + procId) );

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if(procId >= C6472IpcInt_state.maxProcessors) {
        /*! @retval C6472IPCINT_E_FAIL Invalid procId specified */
        status = C6472IPCINT_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472IpcInt_waitClearInterrupt",
                             status,
                             "Invalid procId specified");
        return status;
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    /* Wait for DSP to clear the previous interrupt */
    while( (  REG32(( C6472IpcInt_state.ipcarx_base + offset)) & mask));
    GT_1trace (curTrace,GT_LEAVE,"C6472IpcInt_waitClearInterrupt", status);

    /*! @retval C6472IPCINT_SUCCESS Wait for interrupt clearing successfully
                completed. */
    return status ;
}


/*!
 *  @brief      Function to send a specified interrupt to the DSP.
 *
 *  @param      procId  Remote processor ID
 *  @param      intId   interrupt id
 *  @param      value   Value to be sent with the interrupt
 *
 *  @sa         C6472IpcInt_clearInterrupt
 */
Int32
C6472IpcInt_sendInterrupt (UInt16 procId, UInt32 intId,  UInt32 value)
{
    Int32 status = C6472IPCINT_SUCCESS;
    //extern volatile cregister Uns DNUM;
    extern volatile cregister unsigned DNUM;
    UInt32 val, offset = 0;
    UInt32 remote_mask;

    GT_3trace (curTrace, GT_ENTER, "C6472IpcInt_sendInterrupt",
               procId, intId, value);

    GT_assert (curTrace,(ArchIpcInt_object.isSetup == TRUE));
    GT_assert (curTrace, (procId < MultiProc_MAXPROCESSORS));

    /*
     *  bit 0 is set to generate interrupt.
     *  bits 4-7 is set to specify the interrupt generation source.
     *  The convention is that bit 4 (SRCS0) is used for core 0,
     *  bit 5 (SRCS1) for core 1, etc... .
     */

    val = (1 << (DNUM + SRCSX_SHIFT)) | 1;

    /* IPCGRn is used for IPC with COREn, where n from 0 to 5*/
    offset = IPCGR_OFFSET (procId);

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if(procId >= C6472IpcInt_state.maxProcessors) {
        /*! @retval C6472IPCINT_E_FAIL Invalid procId specified */
        status = C6472IPCINT_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472IpcInt_sendInterrupt",
                             status,
                             "Invalid procId specified");
        return status;
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    REG32(C6472IpcInt_state.ipcgrx_base + offset) = val;
    
    GT_1trace (curTrace, GT_LEAVE, "C6472IpcInt_sendInterrupt",status);

     /*! @retval C6472IPCINT_SUCCESS Interrupt successfully sent */
    return status;
}


/*!
 *  @brief      Function to clear the specified interrupt received from the DSP.
 *
 *  @param      procId  Remote processor ID
 *  @param      intId   interrupt id
 *
 *  @sa         C6472IpcInt_sendInterrupt
 */
UInt32
C6472IpcInt_clearInterrupt (UInt16 procId, UInt32 intId)
{
    extern volatile cregister unsigned DNUM;
    UInt32 retVal = 0, val;

    GT_2trace (curTrace,GT_ENTER,"C6472IpcInt_clearInterrupt",
               procId, intId);

    GT_assert (curTrace,(ArchIpcInt_object.isSetup == TRUE));
    GT_assert (curTrace, (procId < MultiProc_MAXPROCESSORS));

    val = (1 << (procId + SRCSX_SHIFT));

    if ((procId == C6472IpcInt_state.procIds [C6472_INDEX_CORE0]) ||
        (procId == C6472IpcInt_state.procIds [C6472_INDEX_CORE1]) ||
        (procId == C6472IpcInt_state.procIds [C6472_INDEX_CORE2]) ||
        (procId == C6472IpcInt_state.procIds [C6472_INDEX_CORE3]) ||
        (procId == C6472IpcInt_state.procIds [C6472_INDEX_CORE4]) ||
        (procId == C6472IpcInt_state.procIds [C6472_INDEX_CORE5])) {
        /* Set ipcar0 register to clear interrupt */
        REG32(C6472IpcInt_state.ipcarx_base + (DNUM << 2)) = val;
    }
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    else {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "C6472IpcInt_clearInterrupt",
                             C6472IPCINT_E_FAIL,
                             "Invalid procId specified");
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_0trace (curTrace, GT_LEAVE, "C6472IpcInt_clearInterrupt");

    /*! @retval Value Value received with the interrupt. */
    return retVal;
}


/*!
 *  @brief      Function to check and clear the remote proc interrupt
 *
 *  @param      arg     Optional argument to the function.
 *
 *  @sa         _C6472IpcInt_isr
 */
static
Bool
_C6472IpcInt_checkAndClearFunc (Ptr arg)
{
    extern volatile cregister unsigned DNUM;
    UInt32 mask = 1 << SRCSX_SHIFT, val;
    UInt16 procId;
 
    val  = REG32(C6472IpcInt_state.ipcgrx_base + (DNUM << 2));
    for (procId = C6472_INDEX_CORE0; procId <= C6472_INDEX_CORE5; procId++) {
        if(val & mask ){
            C6472IpcInt_clearInterrupt (procId,
                        C6472IpcInt_state.isrObjects [procId].recvIntId);
            Atomic_inc_return (&(C6472IpcInt_state.isrObjects [procId].asserted));
        }
        mask <<= 1;
    }

    /* This is not a shared interrupt, so interrupt has always occurred */
    /*! @retval TRUE Interrupt has occurred. */
    return (TRUE);
}


/*!
 *  @brief      Interrupt Service Routine for C6472IpcInt module
 *
 *  @param      arg     Optional argument to the function.
 *
 *  @sa         _C6472IpcInt_checkAndClearFunc
 */
static
Bool
_C6472IpcInt_isr (Ptr ref)
{
    UInt16 i = 0, j;

    GT_1trace (curTrace, GT_ENTER, "_C6472IpcInt_isr", ref);

    for (i = 0 ; i < C6472IpcInt_state.maxProcessors ; i++) {
        while (Atomic_read (&(C6472IpcInt_state.isrObjects [i].asserted)) != 0) {
            if (C6472IpcInt_state.isrObjects [i].fxn) {
                 C6472IpcInt_state.isrObjects [i].fxn (
                                    C6472IpcInt_state.isrObjects [i].fxnArgs);
            }
            Atomic_dec_return (&(C6472IpcInt_state.isrObjects [i].asserted));
        }
    }

    GT_1trace (curTrace, GT_LEAVE, "_C6472IpcInt_isr", TRUE);

    /*! @retval TRUE Interrupt has been handled. */
    return (TRUE);
}

#if defined (__cplusplus)
}
#endif
