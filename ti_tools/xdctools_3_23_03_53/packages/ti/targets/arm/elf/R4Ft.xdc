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
 *  ======== R4Ft.xdc ========
 *  Cortex R4 little endian thumb2 with floating point support (vfpv3d16) (ELF)
 */
metaonly module R4Ft inherits ti.targets.arm.elf.IR4 {
    override readonly config string name        = "R4Ft";
    override readonly config string suffix      = "er4ft";
    override readonly config string rts         = "ti.targets.arm.rtsarm";

    override readonly config xdc.bld.ITarget.Model model = {
        endian: "little",
        codeModel: "thumb2",
        shortEnums: true
    };

    override readonly config xdc.bld.ITarget2.Command cc = {
        cmd:  "cl470 -c",
        opts: "-mt --float_support=vfpv3d16 --endian=little -mv7R4 --abi=eabi"
    };

    override readonly config xdc.bld.ITarget2.Command asm = {
        cmd:  "cl470 -c",
        opts: "-mt --float_support=vfpv3d16 --endian=little -mv7R4 --abi=eabi"
    };

    /*!
     *  ======== linkLib ========
     *  Default TMS470 cgtools runtime library to link with executable
     *  (comes from $rootDir/lib)
     */
    config string linkLib = "rtsv7R4_T_le_v3D16_eabi.lib";
}
/*
 *  @(#) ti.targets.arm.elf; 1, 0, 0,280; 4-27-2012 17:07:39; /db/ztree/library/trees/xdctargets/xdctargets-f21x/src/ xlibrary

 */

