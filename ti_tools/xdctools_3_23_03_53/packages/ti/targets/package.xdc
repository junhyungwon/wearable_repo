/* 
 *  Copyright (c) 2008 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 *
 * 
 */
/*
 *  ======== package.xdc ========
 *
 */

requires xdc.bld;

/*!
 *  ======== ti.targets ========
 *  Package of targets for TI code generation tools
 *
 *  The modules in this package implement the xdc.bld.ITarget interface; this
 *  interface "wraps" the compiler tool-chain with an abstract interface
 *  that enables these tools to to be used by the XDC Build Engine.
 */
package ti.targets [1,0,3] {
    module C62, C62_big_endian;
    module C64, C64_big_endian;
    module C64P, C64P_big_endian, C674, C674_big_endian;
    module C67, C67_big_endian;
    module C67P;
    module C64T, C64T_big_endian, C66, C66_big_endian;
    module C54, C54_far;
    module C55, C55_large, C55_huge, C55P_word;
    module C28, C28_large, C28_float;
    module TMS470, TMS470_big_endian;

    interface ITarget;
}
/*
 *  @(#) ti.targets; 1, 0, 3,531; 4-27-2012 17:07:37; /db/ztree/library/trees/xdctargets/xdctargets-f21x/src/ xlibrary

 */

