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
 *  ======== TMS320CDM320.xdc ========
 *
 */
package ti.catalog.c5400;

/*!
 *  ======== TMS320CDM320 ========
 *  The DM320 device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is used
 *  by platforms to obtain "data sheet" information about this device.
 */
metaonly module TMS320CDM320 inherits ITMS320C54xx
{
    config xdc.platform.IPlatform.Memory memBlock[string]  = [
        ["P_DARAM", {
            comment: "On-Chip Program DARAM",
            name: "P_DARAM",
            base: 0x0080,
            len:  0x7F80,
            space: "code"
        }],
    
        ["P_ROM", {
            comment: "On-Chip Program ROM",
            name: "P_ROM",
            base: 0x8000,
            len:  0x7F00,
            space: "code"
        }],
    
        ["VECT", {
            comment: "On-Chip Interrupts",
            name: "VECT",
            base: 0xFF00,
            len:  0x0100,
            space: "code"
        }],
    
        ["P_SARAM", {
            comment: "On-Chip SARAM",
            name:    "P_SARAM",
            base:    0x1C000,
            len:     0x04000,
            space:   "code"
        }],

        ["D_SPRAM", {
            comment: "Scratch-Pad RAM",
            name: "D_SPRAM",
            base: 0x60,
            len:  0x20, 
            space: "data"
        }],
    
        ["D_DARAM", {
            comment: "On-Chip Data DARAM",
            name: "D_DARAM",
            base: 0x0080,
            len:  0x7F80,
            space: "data"
        }],
    
        ["D_SARAM", {
            comment: "On-Chip SARAM",
            name:    "D_SARAM",
            base:    0x8000,
            len:     0x4000,
            space:   "data"
        }],

        ["D_ROM", {
            comment: "On-Chip Data ROM",
            name: "D_ROM", 
            base: 0xC000,
            len:  0x4000, 
            space: "data"
        }],
        
        ["IMAGE_BUFFER", {
            comment: "Co-processor Shared Memory (Image Buffer)",
            name:    "IMAGE_BUFFER",
            base:    0x0000C000,
            len:     0x00004000,
            space:   "data"
        }],
    ];

instance:
    override config string   cpuCoreRevision = "1.0";

    /*!
     *  ======== memMap ========
     *  The default memory map for this device
     */
    config xdc.platform.IPlatform.Memory memMap[string];
};
/*
 *  @(#) ti.catalog.c5400; 1, 0, 0, 0,390; 4-24-2012 14:56:00; /db/ztree/library/trees/platform/platform-n20x/src/
 */

