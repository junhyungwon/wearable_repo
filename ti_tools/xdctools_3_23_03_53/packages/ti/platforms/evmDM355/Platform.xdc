/*
 *  Copyright (c) 2012 by Texas Instruments and others.
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
 *  ======== Platform.xdc ========
 */

/*!
 *  ======== Platform ========
 *  Platform support for the evmDM355
 *
 *  This module implements xdc.platform.IPlatform and defines configuration
 *  parameters that correspond to this platform's Cpu's, Board's, etc.
 *
 *  The configuration parameters are initialized in this package's
 *  configuration script (package.cfg) and "bound" to the TCOM object
 *  model.  Once they are part of the model, these parameters are
 *  queried by a program's configuration script.
 *
 *  This particular platform has a single Cpu, and therefore, only
 *  declares a single CPU configuration object.  Multi-CPU platforms
 *  would declare multiple Cpu configuration parameters (one per
 *  platform CPU).
 */
metaonly module Platform inherits xdc.platform.IPlatform
{
    /*!
     *  ======== BOARD ========
     *  This platform's board attributes
     */
    readonly config xdc.platform.IPlatform.Board BOARD = { 
        id:             "0",
        boardName:      "evmDM355",
        boardFamily:    "evmDM355",
        boardRevision:  null
    };
    
    readonly config xdc.platform.IExeContext.Cpu CPU = { 
        id:             "0",
        clockRate:      216.0,
        catalogName:    "ti.catalog.arm",
        deviceName:     "TMS320DM355",
        revision:       "",
    };

instance:

    /*
     *  ======== memTab ========
     *  Define the evmDM355 memory map
     */
    override readonly config xdc.platform.IPlatform.MemoryMap
        externalMemoryMap = [
            ["DDR2",  {name: "DDR2",  base: 0x80000000, len: 0x8000000}],
        ];

    /*
     *  ======== sectMap ========
     *  Define a placement of compiler generated output sections into
     *  memory regions defined in the memTab above.
     */
    override config string codeMemory = "DDR2";
    override config string dataMemory = "DDR2";
    override config string stackMemory = "DDR2";
};

/*
 *  @(#) ti.platforms.evmDM355; 1, 0, 0,293; 4-24-2012 14:57:47; /db/ztree/library/trees/platform/platform-n20x/src/
 */

