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
 *  ======== TMS320C5405.xdc ========
 *
 */
package ti.catalog.c5400;

/*!
 *  ======== TMS320C5405 ========
 *  The C5405 device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is used
 *  by platforms to obtain "data sheet" information about this device.
 */
metaonly module TMS320C5405 inherits ITMS320C54xx
{
    config xdc.platform.IPlatform.Memory memBlock[string]  = [
        ["P_DARAM", {
            comment: "On-Chip Program DARAM",
            name: "P_DARAM",
            base: 0x0080,
            len:  0x3F80,
            space: "code"
        }],
    
        ["P_ROM", {
            comment: "On-Chip Program ROM",
            name: "P_ROM",
            base: 0xF000,
            len:  0x0F00,
            space: "code"
        }],
    
        ["VECT", {
            comment: "On-Chip Interrupts",
            name: "VECT",
            base: 0xFF80,
            len:  0x0080,
            space: "code"
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
            len:  0x3F80,
            space: "data"
        }],
    
        ["D_ROM", {
            comment: "On-Chip Data ROM",
            name: "D_ROM", 
            base: 0xF000,
            len:  0x0F00, 
            space: "data"
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
 *  @(#) ti.catalog.c5400; 1, 0, 0, 0,390; 4-24-2012 14:55:59; /db/ztree/library/trees/platform/platform-n20x/src/
 */

