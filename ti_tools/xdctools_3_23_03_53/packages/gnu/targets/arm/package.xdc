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
requires xdc.bld;
requires gnu.targets;

/*!
 *  ======== gnu.targets.arm ========
 *  GCC-based code generation targets for Arm
 *
 *  This package offers a generic support for different GNU Arm compilers.
 *  For the targets in this package, a user must set configuration parameter
 *  `LONGNAME` in `config.bld`. That parameter specifies the relative path to
 *  the compiler executable from the directory specified in `rootDir`.
 *
 *  @a(Example)
 *  If a version 2007q3 of the CodeSourcery GNU toolchain for Arm is installed
 *  in C:/CodeSourcery/arm-2007q3, the following settings in `config.bld`
 *  configure `gnu.targets.arm.GCArmv6` target to use that toolchain:
 *  @p(code)
 *  var GCArmv6 = xdc.module("gnu.targets.arm.GCArmv6");
 *  GCArmv6.rootDir = "C:/CodeSourcery/arm-2007q3";
 *  GCArmv6.LONGNAME = "bin/arm-none-linux-gnueabi-gcc";
 *  @p
 *
 *  @see gnu.targets.ITarget#LONGNAME
 *
 */
package gnu.targets.arm [1,0,0,0] {
    module GCArmv5T, GCArmv6, GCArmv7A, M3, M4, M4F;
    module MVArm9, UCArm9;
}
/*
 *  @(#) gnu.targets.arm; 1, 0, 0, 0,364; 4-27-2012 17:31:24; /db/ztree/library/trees/xdctargets/xdctargets-f21x/src/ xlibrary

 */

