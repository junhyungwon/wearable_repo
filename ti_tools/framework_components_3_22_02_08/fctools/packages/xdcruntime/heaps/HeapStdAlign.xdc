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

import xdc.runtime.Memory;
import xdc.runtime.Error;

/*
 *  ======== HeapStdAlign.xdc ========
 *
 */

/*!
 *  ======== HeapStdAlign ========
 *  Malloc/free based heap implementation.
 *
 *  This heap is based on the ANSI C Standard Library functions
 *  `malloc()` and `free()` and assumes that these functions are thread-safe.
 *  Please refer to the target specific documentation of the ANSI C Standard
 *  Library for details.
 *
 *  The largest free block that can be returned form `malloc()` cannot be
 *  determined. Therefore, the property `largestFreeSize` in
 *  `{@link Memory#Stats}` returned from `{@link #getStats()}` always returns
 *  0.
 *
 *  @a(Constraints)
 *  The `{@link #alloc()}` function only supports alignment requests up to
 *  value returned from
 *  `{@link Memory#getMaxDefaultTypeAlign()}`.
 */

@InstanceInitError  /* instance init can fail, call finalize if so */

module HeapStdAlign inherits xdc.runtime.IHeap {

    /*! @_nodoc */
    @XmlDtd
    metaonly struct Instance_View {
        Ptr             address;
        String          label;
        Memory.Size     remainSize;
        Memory.Size     startSize;
    }

    /*!
     *  ======== E_noRTSMemory ========
     *  Error raised if all the RTS heap is used up
     *
     *  The total size of all `HeapStdAlign` instance allocations added
     *  together cannot exceed the `malloc`/`free` heap size determined by
     *  `{@link xdc.cfg.Program#heap}`.
     */
    config xdc.runtime.Error.Id E_noRTSMemory  =
        {msg: "The RTS heap is used up. Examine Program.heap."};

    /*!
     *  ======== A_zeroSize ========
     *  Assert that the `{@link #size}` is non-zero on the create
     */
    config xdc.runtime.Assert.Id A_zeroSize  =
            {msg: "HeapStdAlign_create cannot have a zero size value"};

    /*!
     *  Assert raised when the requested alignment is not a power of 2.
     */
    config xdc.runtime.Assert.Id A_align =
        {msg: "A_align: Requested align is not a power of 2"};

    /*!
     *  ======== A_invalidTotalFreeSize ========
     *  Assert that remaining size is less than or equal to starting size.
     *
     *  If this assertion is raised, it means that either incorrect sizes
     *  were passed to `{@link #free}` or multiple calls to `{@link #free}`
     *  were made with the same buffer.
     */
    config xdc.runtime.Assert.Id A_invalidTotalFreeSize = {
        msg: "HeapStdAlign instance totalFreeSize > than starting size"
    };

instance:

    /*!
     *  ======== create ========
     *  Create a `HeapStdAlign` heap
     *
     *  This heap uses the ANSI C Standard Library functions
     *  `malloc()` and `free()` to manage memory and assumes that these
     *  functions are thread-safe.
     *
     *  @see HeapStdAlign#Params
     */
    create();

    /*!
     *  ======== size ========
     *  Size (in MAUs) of the heap.
     *
     *  This parameter specifies the size of the heap managed by a
     *  `HeapStdAlign` instance. `HeapStdAlign` is built upon the ANSI C
     *  Standard Library functions `malloc()` and `free()`.
     *
     *  The total size of all `HeapStdAlign` instance allocations added
     *  together cannot exceed the `malloc`/`free` heap size determined by
     *  `{@link xdc.cfg.Program#heap Program.heap}`.
     *
     *  This is a required parameter. It must be set by the caller. Failure
     *  to do so, will result in a build error for the static create or an
     *  assert for the runtime create.
     */
    config Memory.Size size = 0;

    /*!
     *  ======== alloc ========
     *  Allocates a block of memory from the heap.
     *
     *  @a(Constraints)
     *  The only alignment currently supported is the default
     *  alignment returned by the underlying `malloc()` implementation.
     *  The align value must be less than or equal to the value returned from
     *  `{@link Memory#getMaxDefaultTypeAlign()}`.
     *
     *  @see IHeap#alloc
     */
    override Ptr alloc(SizeT size, SizeT align, Error.Block *eb);

    /*!
     *  ======== isBlocking ========
     *  Returns whether the heap may block during an `HeapStdAlign_alloc()` or
     *  `HeapStdAlign_free()`.
     *
     *  @a(returns)
     *  Since the implementation of the underlaying ANSI C Standard Library
     *  is not known, this function always returns the more restrictive case
     *  which is `TRUE`.
     */
    override Bool isBlocking();

internal:

    struct Module_State {
        Memory.Size remainRTSSize;      /* Remaining size of rts heap      */
    };

    struct Instance_State {
        Memory.Size remainSize;         /* Size of the remaining heap.      */
        Memory.Size startSize;          /* Starting size of the heap.       */
    };
}

/*
 *  @(#) xdcruntime.heaps; 1, 0, 0,1; 2-15-2012 07:45:06; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

