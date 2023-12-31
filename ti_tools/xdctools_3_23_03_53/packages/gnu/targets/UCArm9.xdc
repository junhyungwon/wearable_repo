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
 *  ======== UCArm9.xdc ========
 *  @_nodoc
 *
 *  uClibc Arm9 little endian embedded Linux target
 *
 *  Arm nomenclature:
 *  @p(blist)
 *      Arm7*  devices contain v4 Cores
 *      Arm9*  devices contain v5 Cores
 *      Arm11* devices contain v6 thumb2 Cores
 *  @p
 */
metaonly module UCArm9 inherits gnu.targets.ITarget {
    override readonly config string name                = "UCArm9";     
    override readonly config string os                  = "Linux";      
    override readonly config string suffix              = "470uC";
    override readonly config string isa                 = "v5T";
    override readonly config xdc.bld.ITarget.Model model= {
        endian: "little"
    };

    override readonly config string rts = "gnu.targets.rts470uC";
    override config string platform     = "ti.platforms.evmDM6446";
    
    override config string LONGNAME = "/bin/arm-linux-gcc";

    override config xdc.bld.ITarget2.Options ccOpts = {
        prefix: "-Wunused",
        suffix: "-Dfar= "
    };

    override config xdc.bld.ITarget2.Options lnkOpts = {
        prefix: "-static",
        suffix: "-Wl,-Map=$(XDCCFGDIR)/$@.map -L$(rootDir)/lib"
    };
        
    /*!
     *  ======== versionMap ========
     *  This map translates version string information from the compiler
     *  into a compatibility key.  The compatibilty key is used to
     *  validate consistency among a collection of packages used in
     *  a configuration.
     *
     *  The compiler version string is "gcc<ver>", where <ver> is
     *  GCCVERS.
     *
     *  If a compiler version is not found in this map the default is
     *  "1,0,<ver>", where <ver> is the compiler version number.  Thus,
     *  the user only needs to extend this table when a significant
     *  incompatibility occurs or when two versions of the compiler should
     *  be treated as 100% compatible.
     */
    override config string versionMap[string] = [
        ["gcc3.2", "1,0,3.2,0"],
    ];

    override readonly config xdc.bld.ITarget.StdTypes stdTypes = {
        t_IArg          : { size: 4, align: 4 },
        t_Char          : { size: 1, align: 1 },
        t_Double        : { size: 8, align: 4 },
        t_Float         : { size: 4, align: 4 },
        t_Fxn           : { size: 4, align: 4 },
        t_Int           : { size: 4, align: 4 },
        t_Int8          : { size: 1, align: 1 },
        t_Int16         : { size: 2, align: 2 },
        t_Int32         : { size: 4, align: 4 },
        t_Int64         : { size: 8, align: 4 },
        t_Long          : { size: 4, align: 4 },
        t_LDouble       : { size: 8, align: 4 },
        t_LLong         : { size: 8, align: 4 },
        t_Ptr           : { size: 4, align: 4 },
        t_Short         : { size: 2, align: 2 },
        t_Size          : { size: 4, align: 4 },
    };
}
/*
 *  @(#) gnu.targets; 1, 0, 1,534; 4-27-2012 17:07:22; /db/ztree/library/trees/xdctargets/xdctargets-f21x/src/ xlibrary

 */

