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
 *  ======== ITMS320DA8xx.xdc ========
 *
 */
package ti.catalog.c6000;

/*!
 *  ======== ITMS320DA8xx ========
 *  An interface implemented by TMS320DA8xx devices.
 *
 */
metaonly interface ITMS320DA8xx inherits ti.catalog.ICpuDataSheet
{

    config long cacheSizeL1[string] = [
        ["0k",  0x0000],
        ["4k",  0x1000],
        ["8k",  0x2000],
        ["16k", 0x4000],
        ["32k", 0x8000],
    ];

    config long cacheSizeL2[string] = [
        ["0k",  0x00000],
        ["32k", 0x08000],
        ["64k", 0x10000],
        ["128k",0x20000],
        ["256k",0x40000],
    ];

    readonly config ti.catalog.c6000.ICacheInfo.CacheDesc cacheMap[string] =  [
             ['l1PMode',{desc:"L1P Cache",
                         base: 0x11E00000,
                         map : [["0k",0x0000],
                                ["4k",0x1000],
                                ["8k",0x2000],
                                ["16k",0x4000],
                                ["32k",0x8000]],
                         defaultValue: "32k",
                         memorySection: "L1PSRAM"}],
         
                 ['l1DMode',{desc:"L1D Cache",
                         base: 0x11F00000,
                         map : [["0k",0x0000],
                                ["4k",0x1000],
                                ["8k",0x2000],
                                ["16k",0x4000],
                                ["32k",0x8000]],
                         defaultValue: "32k",
                         memorySection: "L1DSRAM"}],
                     
             ['l2Mode',{desc:"L2 Cache",
                         base: 0x11800000,
                         map : [["0k",0x0000],
                                ["32k",0x8000],
                                ["64k",0x10000],
                                ["128k",0x20000],
                                ["256k",0x40000]],
                         defaultValue: "0k",
                         memorySection: "IRAM"}], 

    ];

instance:
    override config int     minProgUnitSize = 1;
    override config int     minDataUnitSize = 1;    
    override config int     dataWordSize    = 4;

    override config string   cpuCore        = "C674";
    override config string   isa = "674";
    override config string   cpuCoreRevision = "1.0";

    config xdc.platform.IPlatform.Memory memMap[string]  = [
        ["IROM", {
            comment:    "Internal 1MB L2 ROM",
            name:       "IROM",
            base:       0x11700000,
            len:        0x00100000,
            space:      "code/data",
            access:     "RX"
        }],

        ["IRAM", {
            comment:    "Internal 256KB L2 memory",
            name:       "IRAM",
            base:       0x11800000,
            len:        0x00040000,
            space:      "code/data",
            access:     "RWX"
        }],
        
        ["L1PSRAM", {
            comment:    "Internal 32KB L1 program memory",
            name:       "L1PSRAM",
            base:       0x11E00000,
            len:        0x00008000,
            space:      "code",
            access:     "RWX"
        }],

        ["L1DSRAM", {
            comment:    "Internal 32KB L1 data memory",
            name:       "L1DSRAM",
            base:       0x11F00000,
            len:        0x00008000,
            space:      "data",
            access:     "RW"
        }],
        
        ["L3_CBA_RAM", {
            comment:    "128KB ARM/DSP local shared RAM",
            name:       "L3_CBA_RAM",
            base:       0x80000000,
            len:        0x00020000,
            space:      "code/data",
            access:     "RWX"
        }],
    ];
};
/*
 *  @(#) ti.catalog.c6000; 1, 0, 0, 0,390; 4-24-2012 14:56:06; /db/ztree/library/trees/platform/platform-n20x/src/
 */

