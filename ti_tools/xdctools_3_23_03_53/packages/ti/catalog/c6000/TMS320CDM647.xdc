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
 *  ======== TMS320CDM647.xdc ========
 *
 */
package ti.catalog.c6000;

/*!
 *  ======== TMS320CDM647 ========
 *  The DM647 device data sheet module.
 *
 *  This module implements the ICpuDataSheet interface and is 
 *  used by platforms to obtain "data sheet" information about this device.
 */
metaonly module TMS320CDM647 inherits ITMS320C6452
{

instance:

    /*!
     *  ======== memMap ========
     *  The default memory map for this device
     */
    override config xdc.platform.IPlatform.Memory memMap[string] = [
        ["IRAM", {
            comment:    "Internal 256KB L2 RAM/CACHE",
            name:       "IRAM",
            base:       0xA00000,
            len:        0x040000,
            space:      "code/data",
            access:     "RWX"
        }],

        ["L1PSRAM", {
            comment:    "Internal 32KB RAM/CACHE L1 program memory",
            name:       "L1PSRAM",
            base:       0xE00000,
            len:        0x008000,
            space:      "code",
            access:     "RWX"
        }],

        ["L1DSRAM", {
            comment:    "Internal 32KB RAM/CACHE L1 data memory",
            name:       "L1DSRAM",
            base:       0xF00000,
            len:        0x008000,
            space:      "data",
            access:     "RW"
        }],
    ];
};
/*
 *  @(#) ti.catalog.c6000; 1, 0, 0, 0,390; 4-24-2012 14:56:08; /db/ztree/library/trees/platform/platform-n20x/src/
 */

