/*
 *  Copyright 2012 by Texas Instruments Incorporated.
 *
 */

/*!
 * File generated by platform wizard. DO NOT MODIFY
 *
 */

metaonly module Platform inherits xdc.platform.IPlatform {

    config ti.platforms.generic.Platform.Instance CPU =
        ti.platforms.generic.Platform.create("CPU", {
            clockRate:      400,                                       
            catalogName:    "ti.catalog.arp32",
            deviceName:     "TMS320C6A8149",
            externalMemoryMap:
           [
                ["DDR3_EVEVECS",
                     {
                        base: 0x90000000,                    
                        name: "DDR3_EVEVECS",
                        len: 0x00000100,
                        access: "RWX",
                        page:  0,
                     }
                ],
                ["DDR3_EVE",
                     {
                        base: 0x90000100,
                        space: "code/data",
                        name: "DDR3_EVE",
                        len: 0x0FFFFF00,
                        access: "RWX",
                        page:  1,
                     }
                ],
                ["DDR_SR0",
                     {
                        base: 0x8E000000,
                        space: "code/data",
                        name: "DDR_SR0",
                        len: 0x01000000,
                        access: "RWX",
                        page:  1,
                     }
                ],
           ],

    });

instance :

    override config string codeMemory  = "DDR3_EVE";
    override config string dataMemory  = "DDR3_EVE";
    override config string stackMemory = "DDR3_EVE";
}
/*
 *  @(#) ti.sdo.ipc.examples.platforms.evmC6A8149.eve; 1,0,0,1; 5-22-2012 16:24:45; /db/vtree/library/trees/ipc/ipc-h32/src/ xlibrary

 */

