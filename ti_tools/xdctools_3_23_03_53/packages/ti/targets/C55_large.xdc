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

/*
 *  ======== C55_large.xdc ========
 *
 */

/*!
 *  ======== C55_large ========
 *  TI C55 large model
 */
metaonly module C55_large inherits ti.targets.ITarget {
    override readonly config string name                = "C55_large";  
    override readonly config string suffix              = "55L";
    override readonly config string isa                 = "55"; 
    override readonly config xdc.bld.ITarget.Model model= {
        endian: "big",
        dataModel: "large"
    };
    override readonly config string rts                 = "ti.targets.rts5500";

    override readonly config xdc.bld.ITarget.Module base= ti.targets.C55;

    override readonly config xdc.bld.ITarget2.Command ar = {
        cmd: "ar55",
        opts: "rq"
    };

    override readonly config xdc.bld.ITarget2.Command cc = {
        cmd: "cl55 -c",
        opts: "-ml"
    };

    override readonly config xdc.bld.ITarget2.Command vers = {
        cmd: "cl55",
        opts: "--compiler_revision"
    };

    override readonly config xdc.bld.ITarget2.Command asm = {
        cmd: "cl55 -c",
        opts: "-ml"
    };

    override readonly config xdc.bld.ITarget2.Command lnk = {
        cmd: "lnk55",
        opts: ""
    };

    /*!
     *  ======== asmOpts ========
     *  User configurable assembler options.
     *
     *  Defaults:
     *  @p(dlist)
     *      -`-qq`
     *          super quiet mode
     */
    override config xdc.bld.ITarget2.Options asmOpts = {
        prefix: "-qq",
        suffix: ""
    };

    /*!
     *  ======== ccOpts ========
     *  User configurable compiler options.
     *
     *  Defaults:
     *  @p(dlist)
     *      -`-qq`
     *          super quiet mode
     *      -`-pdsw225`
     *          generate a warning for implicitly declared functions; i.e.,
     *          functions without prototypes
     *      -`-Dfar= `
     *          ignore keyword far; this allows one to write code that can
     *          be compiled in large model and small model without #ifdef's
     */
    override config xdc.bld.ITarget2.Options ccOpts = {
        prefix: "-qq -pdsw225 -Dfar= ",
        suffix: ""
    };

    /*!
     *  ======== lnkOpts ========
     *  User configurable linker options.
     *
     *  Defaults:
     *  @p(dlist)
     *      -`-w`
     *          Display linker warnings
     *      -`-q`
     *          Quite run
     *      -`-u`
     *          Place unresolved external symbol into symbol table
     *      -`-c`
     *          ROM autoinitialization model
     *      -`-m`
     *          create a map file
     *      -`-l`
     *          archive library file as linker input
     */
    override config xdc.bld.ITarget2.Options lnkOpts = {
        prefix: "-w -q -u _c_int00",
        suffix: "-c -m $(XDCCFGDIR)/$@.map -l $(rootDir)/lib/rts55x.lib"
    };

    override config string includeOpts = "-I$(rootDir)/include";

    final override readonly config string sectMap[string] =
        ti.targets.C55.sectMap;

    override readonly config xdc.bld.ITarget.StdTypes stdTypes = {
        t_IArg          : { size: 2, align: 2 },
        t_Char          : { size: 1, align: 1 },
        t_Double        : { size: 2, align: 2 },
        t_Float         : { size: 2, align: 2 },
        t_Fxn           : { size: 2, align: 2 },
        t_Int           : { size: 1, align: 1 },
        t_Int8          : { size: 1, align: 1 },
        t_Int16         : { size: 1, align: 1 },
        t_Int32         : { size: 2, align: 2 },
        t_Int40         : { size: 4, align: 2 },
        t_Long          : { size: 2, align: 2 },
        t_LDouble       : { size: 2, align: 2 },
        t_LLong         : { size: 4, align: 2 },
        t_Ptr           : { size: 2, align: 2 },
        t_Short         : { size: 1, align: 1 },
        t_Size          : { size: 1, align: 1 },
    };

    override readonly config Int bitsPerChar = 16;
}
/*
 *  @(#) ti.targets; 1, 0, 3,531; 4-27-2012 17:07:36; /db/ztree/library/trees/xdctargets/xdctargets-f21x/src/ xlibrary

 */

