/* 
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
/*
 *  ======== HeapStdAlign.c ========
 *
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Gate.h>
#include <xdc/runtime/Memory.h>
#include "package/internal/HeapStdAlign.xdc.h"

#include <stdlib.h>

/*
 *  ======== HeapStdAlign_Instance_init ========
 *  Initialize the heap object's fields.
 *
 *  If there is not enough RTS heap memory, then fail by raising an error.
 *  The return value is only used by finailze methods which are not defined
 *  for this module.
 */
Int HeapStdAlign_Instance_init(HeapStdAlign_Object *obj,
        const HeapStdAlign_Params *params, Error_Block *eb)
{
    /* Make sure size is non-zero */
    Assert_isTrue((params->size != 0), HeapStdAlign_A_zeroSize);
    
    /* Make sure there is remaining memory in RTS */
    if (module->remainRTSSize < params->size) {
       Error_raise(eb, HeapStdAlign_E_noRTSMemory, 0, 0);
    }
    else {
        module->remainRTSSize -= params->size;
    
        obj->remainSize = params->size;
        obj->startSize  = params->size;
    }

    return (0);        /* return status passed to finalize method */
}

/*
 *  ======== HeapStdAlign_alloc ========
 *  This heap uses the 'C' rts malloc call. 
 *
 *  Only support alignment requests that will be honored by malloc.
 */
Ptr HeapStdAlign_alloc(HeapStdAlign_Object *obj, SizeT size, SizeT align,
                  Error_Block *eb)
{
    UInt32     alignSize;
    UInt32     alignBytes;
    UInt32     remainBytes;
    Ptr        buf;
    Ptr        alignBuf;
    UInt32     tmpAlign;
    UInt32    *addrPtr;
    IArg key;
    
    /* Assert that requested align is a power of 2 */
    Assert_isTrue(((align & (align - 1)) == 0), HeapStdAlign_A_align);
    
    /* Assert that requested block size is non-zero */
    Assert_isTrue((size != 0), HeapStdAlign_A_zeroSize);

    /*
     *  Must at least align on 32-bit boundary since we're storing the
     *  32-bit address of the malloc'd buffer.
     */
    alignBytes = (align < sizeof(UInt32)) ? sizeof(UInt32) : align;

    /*
     *  We will allocate a buffer larger than "size", to allow for alignment
     *  and to store the address and original size of the buffer returned by
     *  malloc() at the end of the buffer.
     *  Make size a multiple of 32-bits so it will be easier to find the
     *  address of the buffer returned by malloc(). 
     */
    alignSize = size;
    if ((remainBytes = alignSize & (sizeof(UInt32) - 1)) != 0) {
        /* size is not a multiple of 32-bits */
        alignSize += sizeof(UInt32) - remainBytes;
        remainBytes = sizeof(UInt32) - remainBytes;
    }

    /* Add 2 32-bit words to store address and original size of buffer */
    alignSize += sizeof(UInt32) * 2;

    alignSize += alignBytes;

    /* Determine if there is enough memory */
    key = Gate_enterSystem();
    if ((SizeT)(obj->remainSize) < alignSize) {
        Gate_leaveSystem(key);
        return (NULL);
    }
    obj->remainSize -= alignSize;
    Gate_leaveSystem(key);
    
    /* malloc the buffer! */
    if ((buf = malloc(alignSize)) == NULL) {
        /* Undo the size change in case of a failure */
        key = Gate_enterSystem();
        obj->remainSize += alignSize;
        Gate_leaveSystem(key);
        return (NULL);
    }

    /* Get the aligned address */
    tmpAlign = (UInt32)buf & (alignBytes - 1);
    if (tmpAlign) {
        alignBuf = (Void *)((UInt32)buf + alignBytes - tmpAlign);
    }
    else {
        /* The buffer was already aligned */
        alignBuf = buf;
    }

    /* Store the malloc'd address and original size for freeing later. */
    addrPtr = (UInt32 *)((UInt32)alignBuf + size + remainBytes);
    *addrPtr = (UInt32)buf;
    *(addrPtr + 1) = alignSize;

    return (alignBuf);
}

/*
 *  ======== HeapStdAlign_free ========
 *  Free the memory back and adjust the remaining free accordingly
 */
Void HeapStdAlign_free(HeapStdAlign_Object *obj, Ptr alignBuf, SizeT size)
{
    UInt32 *addrPtr;
    UInt32  bufSize;
    Ptr     buf;
    UInt32  remainBytes = 0;
    IArg    key;
    

    if ((sizeof(UInt32) - 1) & size) {
        remainBytes = sizeof(UInt32) - ((sizeof(UInt32) - 1) & size);
    }

    /* Get the address and original size of the buffer from the end */
    addrPtr = (UInt32 *)((UInt32)alignBuf + size + remainBytes);
    buf = (Ptr)*addrPtr;
    bufSize = (UInt32)(*(addrPtr + 1));

    free(buf);

    key = Gate_enterSystem();
    
    /* Adjust the remaining size */
    obj->remainSize += bufSize;    
    
    /* Make sure something fishy is not going on */
    Assert_isTrue((obj->remainSize <= obj->startSize),
        HeapStdAlign_A_invalidTotalFreeSize);
    
    Gate_leaveSystem(key);    
}

/*
 *  ======== HeapStdAlign_isBlocking ========
 */
Bool HeapStdAlign_isBlocking(HeapStdAlign_Object *obj)
{
    return (TRUE);
}

/*
 *  ======== HeapStdAlign_getStats ========
 *  Return the stats.
 */
Void HeapStdAlign_getStats(HeapStdAlign_Object *obj, Memory_Stats *stats)
{
    stats->totalSize         = obj->startSize;
    stats->totalFreeSize     = obj->remainSize;
    stats->largestFreeSize   = obj->remainSize;
}
/*
 *  @(#) xdcruntime.heaps; 1, 0, 0,1; 2-15-2012 07:45:06; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

