/** 
 *  @file   C6472Core0Proc.h
 *
 *  @brief      Processor interface for C6472 SLAVE
 *
 *
 */
/* 
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


#ifndef C6472CORE00PROC_H_0xbab0
#define C6472CORE00PROC_H_0xbab0


/* Module headers */
#include <ti/syslink/ProcMgr.h>
#include <ProcDefs.h>


#if defined (__cplusplus)
extern "C" {
#endif


/*!
 *  @def    C6472C0RE0PROC_MODULEID
 *  @brief  Module ID for C6472 Core0 slave.
 */
#define C6472C0RE0PROC_MODULEID (UInt16) 0xbab0

/* =============================================================================
 *  All success and failure codes for the module
 * =============================================================================
 */
/*!
 *  @def    C6472C0RE0PROC_STATUSCODEBASE
 *  @brief  Error code base for ProcMgr.
 */
#define C6472CORE0PROC_STATUSCODEBASE      (C6472C0RE0PROC_MODULEID << 12u)

/*!
 *  @def    C6472CORE0PROC_MAKE_FAILURE
 *  @brief  Macro to make failure code.
 */
#define C6472CORE0PROC_MAKE_FAILURE(x)    ((Int)(  0x80000000                    \
                                         | (C6472CORE0PROC_STATUSCODEBASE + (x))))

/*!
 *  @def    C6472CORE0PROC_MAKE_SUCCESS
 *  @brief  Macro to make success code.
 */
#define C6472CORE0PROC_MAKE_SUCCESS(x)    (C6472CORE0PROC_STATUSCODEBASE + (x))

/*!
 *  @def    C6472CORE0PROC_E_OSFAILURE
 *  @brief  Failure in an OS-specific operation.
 */
#define C6472CORE0PROC_E_OSFAILURE        C6472CORE0PROC_MAKE_FAILURE(4)

/*!
 *  @def    C6472CORE0PROC_E_INVALIDARG
 *  @brief  Argument passed to a function is invalid.
 */
#define C6472CORE0PROC_E_INVALIDARG       C6472CORE0PROC_MAKE_FAILURE(5)

/*!
 *  @def    C6472CORE0PROC_E_MEMORY
 *  @brief  Memory allocation failed.
 */
#define C6472CORE0PROC_E_MEMORY           C6472CORE0PROC_MAKE_FAILURE(6)

/*!
 *  @def    C6472CORE0PROC_E_HANDLE
 *  @brief  Invalid object handle specified
 */
#define C6472CORE0PROC_E_HANDLE           C6472CORE0PROC_MAKE_FAILURE(7)

/*!
 *  @def    C6472CORE0PROC_E_ACCESSDENIED
 *  @brief  The operation is not permitted in this process.
 */
#define C6472CORE0PROC_E_ACCESSDENIED     C6472CORE0PROC_MAKE_FAILURE(8)

/*!
 *  @def    C6472CORE0PROC_E_FAIL
 *  @brief  Generic failure.
 */
#define C6472CORE0PROC_E_FAIL             C6472CORE0PROC_MAKE_FAILURE(9)

/*!
 *  @def    C6472CORE0PROC_SUCCESS
 *  @brief  Operation successful.
 */
#define C6472CORE0PROC_SUCCESS           C6472CORE0PROC_MAKE_SUCCESS(0)

/*!
 *  @def    C6472CORE0PROC_S_ALREADYSETUP
 *  @brief  The C6472CORE0PROC module has already been setup in this process.
 */
#define C6472CORE0PROC_S_ALREADYSETUP     C6472CORE0PROC_MAKE_SUCCESS(1)

/*!
 *  @def    C6472CORE0PROC_S_OPENHANDLE
 *  @brief  Other C6472CORE0PROC clients have still setup the C6472CORE0PROC module.
 */
#define C6472CORE0PROC_S_SETUP            C6472CORE0PROC_MAKE_SUCCESS(2)

/*!
 *  @def    C6472CORE0PROC_S_OPENHANDLE
 *  @brief  Other C6472CORE0PROC handles are still open in this process.
 */
#define C6472CORE0PROC_S_OPENHANDLE       C6472CORE0PROC_MAKE_SUCCESS(3)

/*!
 *  @def    C6472CORE0PROC_S_ALREADYEXISTS
 *  @brief  The C6472CORE0PROC instance has already been created/opened in this
 *          process
 */
#define C6472CORE0PROC_S_ALREADYEXISTS    C6472CORE0PROC_MAKE_SUCCESS(4)


/* =============================================================================
 *  Macros and types
 * =============================================================================
 */
/*!
 *  @brief  Module configuration structure.
 */
typedef struct C6472CORE0PROC_Config {
    UInt32 reserved;
    /*!< Reserved field (not currently required) */
} C6472CORE0PROC_Config;

/*!
 *  @brief  Configuration parameters specific to this processor.
 */
typedef struct C6472CORE0PROC_Params_tag {
    UInt32           numMemEntries;
    /*!< Number of memory regions to be configured. */
    ProcMgr_AddrInfo memEntries [ProcMgr_MAX_MEMORY_REGIONS];
    /*!< Array of information structures for memory regions to be configured. */
} C6472CORE0PROC_Params;

/*!
 *  @brief  Defines C6472CORE0PROC object handle
 */
typedef struct C6472CORE0PROC_Object * C6472CORE0PROC_Handle;


/* =============================================================================
 *  APIs
 * =============================================================================
 */

/* Function to get the default configuration for the C6472CORE0PROC module. */
Void C6472CORE0PROC_getConfig (C6472CORE0PROC_Config * cfg);

/* Function to setup the C6472CORE0PROC module. */
Int C6472CORE0PROC_setup (C6472CORE0PROC_Config * cfg);

/* Function to destroy the C6472CORE0PROC module. */
Int C6472CORE0PROC_destroy (Void);

/* Function to initialize the parameters for this processor instance. */
Void C6472CORE0PROC_Params_init (C6472CORE0PROC_Handle    handle,
                            C6472CORE0PROC_Params *  params);

/* Function to create an instance of this processor. */
C6472CORE0PROC_Handle C6472CORE0PROC_create (      UInt16                   procId,
                                         const C6472CORE0PROC_Params * params);

/* Function to delete an instance of this processor. */
Int C6472CORE0PROC_delete (C6472CORE0PROC_Handle * handlePtr);

/* Function to open an instance of this processor. */
Int C6472CORE0PROC_open (C6472CORE0PROC_Handle * handlePtr, UInt16 procId);

/* Function to close an instance of this processor. */
Int C6472CORE0PROC_close (C6472CORE0PROC_Handle * handlePtr);


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

#endif /* C6472CORE0PROC_H_0xbbec */
