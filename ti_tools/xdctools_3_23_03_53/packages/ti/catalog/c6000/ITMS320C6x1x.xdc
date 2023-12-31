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
 *  ======== ITMS320C6x1x.xdc ========
 *
 */
package ti.catalog.c6000;

/*!
 *  ======== ITMS320C6x1x ========
 *  An interface implemented by all TMS320C6x1x devices.
 *
 *  This interface is defined to factor common data about this family into
 *  a single place; all TMS320C6x1x devices extend this interface.
 */
metaonly interface ITMS320C6x1x inherits ti.catalog.ICpuDataSheet
{
    config long cacheSize[string] = [
    ["SRAM",        0x0000],
        ["1-way cache", 0x4000],
        ["2-way cache", 0x8000],
        ["3-way cache", 0xc000],
        ["4-way cache", 0x10000],
        ["0k",  0x0000],
        ["16k", 0x4000],
        ["32k", 0x8000],
        ["48k", 0xc000],
        ["64k", 0x10000],
    ];

    readonly config ti.catalog.c6000.ICacheInfo.CacheDesc cacheMap[string] =  [
         ['l2Mode',{desc:"L2 Cache",
                     map : [["SRAM",0x0000],
                            ["1-way cache",0x4000],
                            ["2-way cache",0x8000],
                            ["3-way cache",0xc000],
                            ["4-way cache",0x10000]],
                     defaultValue: "4-way cache",
                     memorySection: "IRAM"}]
    ];

instance:
    override config string   cpuCoreRevision = "1.0";

    override config int     minProgUnitSize = 1;
    override config int     minDataUnitSize = 1;    
    override config int     dataWordSize    = 4;
    
    /*!
     *  ======== memMap ========
     *  The default memory map for this device
     */
    config xdc.platform.IPlatform.Memory memMap[string];
}
/*
 *  @(#) ti.catalog.c6000; 1, 0, 0, 0,390; 4-24-2012 14:56:06; /db/ztree/library/trees/platform/platform-n20x/src/
 */

