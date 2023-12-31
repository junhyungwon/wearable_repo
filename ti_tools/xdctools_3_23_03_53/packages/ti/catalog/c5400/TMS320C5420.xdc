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
 *  ======== TMS320C5420.xdc ========
 *
 */
package ti.catalog.c5400;

/*!
 *  ======== TMS320C5420 ========
 *  The C5420 device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is used
 *  by platforms to obtain "data sheet" information about this device.
 */
metaonly module TMS320C5420 inherits ITMS320C54xx
{
    config xdc.platform.IPlatform.Memory memBlock[string]  = [
        ["P_DARAM0", {
            comment: "On-Chip Program DARAM",
            name: "P_DARAM0",
            base: 0x0080, 
            len:  0x3F80,
            space: "code"
        }],
    
        ["P_SARAM1", {
            comment: "On-Chip Program SARAM1",
            name: "P_SARAM1",
            base: 0x4000,
            len:  0x4000,
            space:  "code"
        }],
    
        ["P_SARAM2", {
            comment: "On-Chip Program SARAM2",
            name: "P_SARAM2",
            base: 0x8000,
            len:  0x7F80,
            space:  "code"
        }],
    
        ["VECT", {
            comment: "On-Chip Interrupts",
            name: "VECT",
            base: 0xFF80,
            len:  0x0080,
            space: "code"
        }],
    
        ["P_SARAM3", {
            comment: "On-Chip Program SARAM3",
            name: "P_SARAM3",
            base: 0x18000,
            len:  0x08000,
            space:  "code"
        }],
    
        ["D_SPRAM", {
            comment: "On-Chip Scratch-Pad RAM",
            name: "D_SPRAM",
            base: 0x60,
            len:  0x20, 
            space: "data"
        }],
    
        ["D_DARAM0", {
            comment: "On-Chip Data DARAM",
            name: "D_DARAM0",
            base: 0x0080,
            len:  0x3F80,
            space: "data"
        }],
    
        ["D_SARAM1", {
            comment: "On-Chip Data SARAM1",
            name: "D_SARAM1",
            base: 0x4000,
            len:  0x4000,
            space: "data"
        }],
    
        ["D_SARAM2", {
            comment: "On-Chip Data SARAM2",
            name: "D_SARAM2",
            base: 0x8000,
            len:  0x8000,
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
 *  @(#) ti.catalog.c5400; 1, 0, 0, 0,390; 4-24-2012 14:55:59; /db/ztree/library/trees/platform/platform-n20x/src/
 */

