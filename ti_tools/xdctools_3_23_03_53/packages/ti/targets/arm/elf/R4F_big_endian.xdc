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
 * */
/*!
 *  ======== R4F_big_endian.xdc ========
 *  Cortex R4 big endian with floating point support (vfpv3d16) (ELF)
 */
metaonly module R4F_big_endian inherits ti.targets.arm.elf.IR4_big_endian {
    override readonly config string name        = "R4F_big_endian";
    override readonly config string suffix      = "er4fe";
    override readonly config string rts         = "ti.targets.arm.rtsarm";

    override readonly config xdc.bld.ITarget.Model model = {
        endian: "big",
        shortEnums: true
    };

    override readonly config xdc.bld.ITarget2.Command cc = {
        cmd:  "cl470 -c",
        opts: "--float_support=vfpv3d16 --endian=big -mv7R4 --abi=eabi"
    };

    override readonly config xdc.bld.ITarget2.Command asm = {
        cmd:  "cl470 -c",
        opts: "--float_support=vfpv3d16 --endian=big -mv7R4 --abi=eabi"
    };

    /*!
     *  ======== linkLib ========
     *  Default TMS470 cgtools runtime library to link with executable
     *  (comes from $rootDir/lib)
     */
    config string linkLib = "rtsv7R4_A_be_v3D16_eabi.lib";
}
/*
 *  @(#) ti.targets.arm.elf; 1, 0, 0,280; 4-27-2012 17:07:39; /db/ztree/library/trees/xdctargets/xdctargets-f21x/src/ xlibrary

 */

