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
 *  ======== Ducati.xdc ========
 */

/*!
 *  ======== Ducati ========
 *  The Ducati device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is 
 *  used by platforms to obtain "data sheet" information about this device.
 *
 */
metaonly module Ducati inherits ti.catalog.ICpuDataSheet
{
instance:
    override config string cpuCore           = "CM3";
    override config string isa               = "v7M";
    override config string cpuCoreRevision   = "1.0";
    override config int    minProgUnitSize   = 1;
    override config int    minDataUnitSize   = 1;
    override config int    dataWordSize      = 4;

    /*!
     *  ======== memMap ========
     *  The memory map returned be getMemoryMap().
     */
    config xdc.platform.IPlatform.Memory memMap[string] = [

        /* 
         * AMMU mapped L2 ROM virtual address
         * Physical address is 0x55000000
         */
        ["L2_ROM", {
            name: "L2_ROM",
            base: 0x00000000,
            len:  0x00004000
        }],

        /* 
         * AMMU mapped L2 RAM virtual address
         * Physical address is 0x55020000
         */
        ["L2_RAM", {
            name: "L2_RAM",
            base: 0x20000000, 
            len:  0x00010000
        }],
    ];
};
/*
 *  @(#) ti.catalog.arm.nda; 1, 0, 0, 0,147; 4-24-2012 14:55:51; /db/ztree/library/trees/platform/platform-n20x/src/
 */

