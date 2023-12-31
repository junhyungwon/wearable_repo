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
 *  ======== Platform.xdc ========
 *  This module implements xdc.IPlatform and defines configuration
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
 *
 */

package ti.platforms.ezdsp28235;

metaonly module Platform inherits xdc.platform.IPlatform
{
    readonly config xdc.platform.IPlatform.Board BOARD = {
        id:             "0",
        boardName:      "ezdsp28235",
        boardFamily:    "ezdsp28235",
        boardRevision:  null
    };

    readonly config xdc.platform.IExeContext.Cpu   CPU = {      
        id:             "0",
        clockRate:      150.0,
        catalogName:    "ti.catalog.c2800",
        deviceName:     "TMS320C28235",
        revision:       null
    };
    
instance:

    override readonly config xdc.platform.IPlatform.MemoryMap
        externalMemoryMap = [
            ["SRAM",
                {
                    name: "SRAM",
                    base: 0x200000,
                    len:  0x020000,
                    page: 0,
                    space: "code/data",
                }
            ],
        ];

    override config string codeMemory = "SRAM";
    
    override config string dataMemory = "SRAM";

    override config string stackMemory = "L03SARAM";

};
/*
 *  @(#) ti.platforms.ezdsp28235; 1, 0, 0,264; 4-24-2012 14:59:11; /db/ztree/library/trees/platform/platform-n20x/src/
 */

