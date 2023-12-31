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
 *  ======== TMS320C5471.xdc ========
 *
 */
package ti.catalog.c5400;

/*!
 *  ======== TMS320C5471 ========
 *  The C5470 device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is used
 *  by platforms to obtain "data sheet" information about this device.
 */
metaonly module TMS320C5471 inherits ITMS320C54xx
{
    config xdc.platform.IPlatform.Memory memBlock[string]  = [
        ["P_DARAM", {
            comment: "On-Chip Program DARAM",
            name: "P_DARAM",
            base: 0x0080, 
            len:  0x1F80,
            space: "code"
        }],
    
        ["P_APIDARAM", {
            comment: "On-Chip DARAM API Accessible",
            name: "P_APIDARAM",
            base: 0x2000, 
            len:  0x2000,
            space: "code"
        }],
    
        ["P_SARAM0", {
            comment: "On-Chip Program SARAM0",
            name: "P_SARAM0",
            base: 0x4000,
            len:  0x2000,
            space:  "code"
        }],
    
        ["P_SARAM1", {
            comment: "On-Chip Program SARAM1",
            name: "P_SARAM1",
            base: 0x6000,
            len:  0x2000,
            space:  "code"
        }],
    
        ["P_SARAM2", {
            comment: "On-Chip Program SARAM2",
            name: "P_SARAM2",
            base: 0x8000,
            len:  0x8000,
            space:  "code"
        }],
    
        ["D_SPRAM", {
            comment: "On-Chip Scratch-Pad RAM",
            name: "D_SPRAM",
            base: 0x60,
            len:  0x1A, 
            space: "data"
        }],
    
        ["D_DARAM", {
            comment: "On-Chip Data DARAM",
            name: "D_DARAM",
            base: 0x0080,
            len:  0x1F80,
            space: "data"
        }],
    
        ["D_APIDARAM", {
            comment: "On-Chip DARAM API Accessible",
            name: "D_APIDARAM",
            base: 0x2000,
            len:  0x2000,
            space: "data"
        }],
    
        ["D_SARAM0", {
            comment: "On-Chip Data SARAM0",
            name: "D_SARAM0",
            base: 0x4000,
            len:  0x2000,
            space: "data"
        }],
    
        ["D_SARAM1", {
            comment: "On-Chip Data SARAM1",
            name: "D_SARAM1",
            base: 0x6000,
            len:  0x2000,
            space: "data"
        }],
    
        ["D_SARAM2", {
            comment: "On-Chip Data SARAM2",
            name: "D_SARAM2",
            base: 0xC000,
            len:  0x3800,
            space: "data"
        }],

    ];

instance:
    override config string cpuCoreRevision = "1.0";

    /*!
     *  ======== memMap ========
     *  The default memory map for this device
     */
    config xdc.platform.IPlatform.Memory memMap[string];
}
/*
 *  @(#) ti.catalog.c5400; 1, 0, 0, 0,390; 4-24-2012 14:56:00; /db/ztree/library/trees/platform/platform-n20x/src/
 */

