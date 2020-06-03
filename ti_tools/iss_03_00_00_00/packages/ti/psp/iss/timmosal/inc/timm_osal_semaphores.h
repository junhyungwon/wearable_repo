/*
 *  Copyright (c) 2010-2011, Texas Instruments Incorporated
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
 *
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

/*
*  @file timm_osal_defines.h
*  The osal header file defines 
*  @path
*
*/
/* -------------------------------------------------------------------------- */
/* =========================================================================
 *!
 *! Revision History
 *! ===================================
 *! 0.1: Created the first draft version, ksrini@ti.com
 * ========================================================================= */

#ifndef _TIMM_OSAL_SEMAPHORE_H_
#define _TIMM_OSAL_SEMAPHORE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
* Includes
*******************************************************************************/

#include "timm_osal_types.h"
#include "timm_osal_error.h"



/* ===========================================================================*/
/**
 * @fn TIMM_OSAL_SemaphoreCreate() - Creates a new counting semaphore instance.
 *
 *  @ param pSemaphore               :Handle of the semaphore to be created.
 *
 *  @ param uInitCount               :Initial count of the semaphore.
 */
/* ===========================================================================*/
TIMM_OSAL_ERRORTYPE TIMM_OSAL_SemaphoreCreate(TIMM_OSAL_PTR *pSemaphore, 
											  TIMM_OSAL_U32 uInitCount);



/* ===========================================================================*/
/**
 * @fn TIMM_OSAL_SemaphoreDelete() - Deletes a previously created semaphore 
 *                                   instance.
 *
 *  @ param pSemaphore               :Handle of the semaphore to be deleted.
 */
/* ===========================================================================*/
TIMM_OSAL_ERRORTYPE TIMM_OSAL_SemaphoreDelete(TIMM_OSAL_PTR pSemaphore);



/* ===========================================================================*/
/**
 * @fn TIMM_OSAL_SemaphoreObtain() - Obtain the semaphore. If current count of
 *                                   the semaphore is positive then decrement
 *                                   the count and return else wait.
 *
 *  @ param pSemaphore               :Handle of the semaphore to be obtained.
 *
 *  @ param uTimeOut                 :Time (in millisec) to wait for the 
 *                                    semaphore count to become positive.
 */
/* ===========================================================================*/
TIMM_OSAL_ERRORTYPE TIMM_OSAL_SemaphoreObtain(TIMM_OSAL_PTR pSemaphore, 
											  TIMM_OSAL_U32 uTimeOut);



/* ===========================================================================*/
/**
 * @fn TIMM_OSAL_SemaphoreRelease() - Release the semaphore. Will increment the
 *                                    semaphore count
 *
 *  @ param pSemaphore               :Handle of the semaphore to be released.
 */
/* ===========================================================================*/
TIMM_OSAL_ERRORTYPE TIMM_OSAL_SemaphoreRelease(TIMM_OSAL_PTR pSemaphore);



/* ===========================================================================*/
/**
 * @fn TIMM_OSAL_SemaphoreReset() - Reset the semaphore count.
 *
 *  @ param pSemaphore              :Handle of the semaphore.
 *
 *  @ param uInitCount              :Value to which the semaphore count will 
 *                                   be reset.
 */
/* ===========================================================================*/
TIMM_OSAL_ERRORTYPE TIMM_OSAL_SemaphoreReset(TIMM_OSAL_PTR pSemaphore, 
											 TIMM_OSAL_U32 uInitCount);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TIMM_OSAL_SEMAPHORE_H_ */
