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
 *  ======== M4F.xdc ========
 *  Cortex M4 with floating point unit, little endian thumb2 (ELF)
 */
metaonly module M4F inherits ti.targets.arm.elf.IM4 {
    override readonly config string name        = "M4F";
    override readonly config string suffix      = "em4f";
    override readonly config string rts         = "ti.targets.arm.rtsarm";

    override readonly config xdc.bld.ITarget2.Command cc = {
        cmd:  "cl470 -c",
        opts: "--endian=little -mv7M4 --abi=eabi --float_support=fpv4spd16"
    };

    override readonly config xdc.bld.ITarget2.Command asm = {
        cmd:  "cl470 -c",
        opts: "--endian=little -mv7M4 --abi=eabi --float_support=fpv4spd16"
    };

    /*!
     *  ======== linkLib ========
     *  Default TMS470 cgtools runtime library to link with executable
     *  (comes from $rootDir/lib)
     */
    config string linkLib = "rtsv7M4_T_le_v4SPD16_eabi.lib";
}
/*
 *  @(#) ti.targets.arm.elf; 1, 0, 0,280; 4-27-2012 17:07:38; /db/ztree/library/trees/xdctargets/xdctargets-f21x/src/ xlibrary

 */

