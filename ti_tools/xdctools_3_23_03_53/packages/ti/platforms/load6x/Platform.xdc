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
 *  ======== Platform.xdc ========
 */

package ti.platforms.load6x;

/*!
 *  ======== Platform ========
 *  Simulation-based Platform support for 6xxx platforms
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
        boardName:      "sim6xxx",
        boardFamily:    "sim6xxx",
        boardRevision:  null
    };

    /*!
     *  ======== CPU ========
     *  The CPU simulated by this platform.
     *
     *  The device simulated is determined by the platform instance name
     *  or if this does not name a ti.catalog.c6000 module, we simulate
     *  a TMS320C6416.
     */
    readonly config xdc.platform.IExeContext.Cpu CPU = {        
        id:             "0",
        clockRate:      600.0,
        catalogName:    "ti.catalog.c6000",
        deviceName:     "TMS320C6416",
        revision:       "",
    };

instance:
    
    /*!
     *  ======== deviceName ========
     *  The CPU simulated by this simulator platform.
     *
     *  This parameter is optional. If it's not set, then the Platform module
     *  parameter CPU.deviceName is used.
     */
    config string deviceName;

    override readonly config xdc.platform.IPlatform.Memory
        externalMemoryMap[string] = [
            //["IRAM", {name: "IRAM", base: 0x00000200, len: 0x0000FE00}],
            ["EXT0", {name: "EXT0", base: 0x00400000, len: 0x00400000}],
            ["EXT1", {name: "EXT1", base: 0x01000000, len: 0x00800000}],
            ["EXT2", {name: "EXT2", base: 0x02000000, len: 0x01000000}],
            ["EXT3", {name: "EXT3", base: 0x03000000, len: 0x01000000}],
            ["BMEM", {name: "BMEM", base: 0x80000000, len: 0x00010000}],
        ];

    override config string codeMemory = "IRAM";

    override config string dataMemory = "EXT3";

    override config string stackMemory = "BMEM";

    /*
     *  ======== l1PMode ========
     *  Define the amount of L1P RAM used for L1 Program Cache.
     *
     *  Check the device documentation for valid values.
     */
    config String l1PMode = "32k";
    
    /*
     *  ======== l1DMode ========
     *  Define the amount of L1D RAM used for L1 Data Cache.
     *
     *  Check the device documentation for valid values.
     */
    config String l1DMode = "32k";
    
    /*
     *  ======== l2Mode ========
     *  Define the amount of L2 RAM used for L2 Cache.
     *
     *  Check the device documentation for valid values.
     */
    config String l2Mode = "0k";
};
/*
 *  @(#) ti.platforms.load6x; 1, 0, 1, 1,109; 4-24-2012 14:59:27; /db/ztree/library/trees/platform/platform-n20x/src/
 */

