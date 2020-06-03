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
 *  ======== ifcpy.c ========
 *  Default creation parameters for all implementations of the FCPY
 *  module abstract interface. 
 *
 *  These parameters are included by the client of the fcpy algorithm(s), 
 *  and referenced by the algorithms.
 */
#include <xdc/std.h>

#include <ifcpy.h>

/*
 *  ======== IFCPY_PARAMS ========
 *  This static initialization defines the default parameters used to
 *  create an instance of a FCPY object.
 */
const IFCPY_Params IFCPY_PARAMS = {
    sizeof(IFCPY_PARAMS),    /* Size of this structure */
    8,                       /* Source Frame length */
    8,                       /* Number of frames for source */
    0,                       /* Stride between frames for source */
    8,                       /* Destination Frame length */
    8,                       /* Number of frames for destination */
    0                        /* Stride between frames for destination */
};



/*
 *  @(#) ti.sdo.fc.dman3.examples.fastcopy; 1, 0, 0,3; 4-16-2012 00:00:08; /db/atree/library/trees/fc/fc-q08/src/ xlibrary

 */

