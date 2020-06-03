/*
 *  @file   Cache.c
 *
 *  @brief      Cache API implementation for C6472 platform
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
#include <asm/cache.h>

/* OSAL & Utils headers */
#include <ti/syslink/utils/Cache.h>
#include <ti/syslink/utils/Trace.h>
#include <Bitops.h>

/* Module level headers */
#include <CacheDrv.h>
#include <CacheDrvDefs.h>

#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

/* =============================================================================
 *  Macros and types
 * =============================================================================
 */

#define L1DCC_OPER_FREEZE       0x00000001      /* L1D OPER freeze */
#define L1DCC_OPER_NORMAL       0x00000000      /* L1D OPER normal */
#define L1DCC_POPER_MASK        0x00070000      /* L1D POPER mask */
#define L1DCC_POPER_FREEZE      0x00010000      /* L1D POPER freeze */

/* =============================================================================
 *  APIs
 * =============================================================================
 */
/* Function to invalidate the Cache module */
Void Cache_inv(Ptr blockPtr, UInt32 byteCnt, Bits16 type, Bool wait) {
    GT_4trace (curTrace, GT_ENTER, "Cache_inv", blockPtr, byteCnt, type, wait);

    L1D_cache_block_invalidate(blockPtr, (blockPtr + byteCnt));

    GT_0trace (curTrace, GT_LEAVE, "Cache_inv");
}

/* Function to write back the Cache module */
Void Cache_wb(Ptr blockPtr, UInt32 byteCnt, Bits16 type, Bool wait) {
    GT_4trace (curTrace, GT_ENTER, "Cache_wb", blockPtr, byteCnt, type, wait);

    L1D_cache_block_writeback(blockPtr, (blockPtr + byteCnt));

    GT_0trace (curTrace, GT_LEAVE, "Cache_wb");
}

/* Function to write back invalidate the Cache module */
Void Cache_wbInv(Ptr blockPtr, UInt32 byteCnt, Bits16 type, Bool wait) {
    GT_4trace (curTrace, GT_ENTER, "Cache_wbInv", blockPtr, byteCnt, type, wait);

    L1D_cache_block_writeback_invalidate(blockPtr, (blockPtr + byteCnt));

    GT_0trace (curTrace, GT_LEAVE, "Cache_wbInv");
}

/* Function to write back invalidate the Cache module */
Void Cache_wait(Void) {
    GT_0trace (curTrace, GT_ENTER, "Cache_wait");

    GT_0trace (curTrace, GT_LEAVE, "Cache_wait");
}

/*
 *  ======== mapL1dPoper ========
 *  Maps the L1D cache mode Poper field to one of the values of Cache_Mode
 */
static inline enum Cache_Mode mapL1dPoper(UInt mode)
{
    if (mode == L1DCC_POPER_FREEZE) {
        return Cache_Mode_FREEZE;
    }
    else {
        return Cache_Mode_NORMAL;
    }
}

enum Cache_Mode Cache_setMode(Bits16 type, enum Cache_Mode mode)
{
    unsigned long flags;
    enum Cache_Mode returnVal;
    UInt prevmode;

    GT_2trace (curTrace, GT_ENTER, "Cache_setMode", type, mode);

    save_global_flags(flags);
    global_cli();

    if (type == Cache_Type_L1P) {
        /*
         * case we want to set L1D cache
         */
        if (mode == Cache_Mode_NORMAL) {
            *((volatile unsigned int *)IMCR_L1DCC) = L1DCC_OPER_NORMAL;
        }    
        else if (mode == Cache_Mode_FREEZE) {
            /*
             *  there should be no data accesses two cycles prior to changing
             *  L1D cache to freeze to work around silicon issue.
             *  GEM_arch.bts_dmc.26
             */
            asm("       nop 2");
            *((volatile unsigned int *)IMCR_L1DCC) = L1DCC_OPER_FREEZE;
        }
        prevmode = *((volatile unsigned int *)IMCR_L1DCC) & L1DCC_POPER_MASK,
        returnVal = mapL1dPoper(prevmode);
    }
    restore_global_flags(flags);

    GT_1trace (curTrace, GT_LEAVE, "Cache_setMode", returnVal);
    return returnVal;
}

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

