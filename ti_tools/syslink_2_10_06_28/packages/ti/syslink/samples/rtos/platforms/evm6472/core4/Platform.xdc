/** 
 *  @file   Platform.xdc
 *
 *  @brief      
 *
 *
 *  ============================================================================
 *
 *  Copyright (c) 2008-2012, Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  
 *  *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  
 *  *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  
 *  *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  Contact information for paper mail:
 *  Texas Instruments
 *  Post Office Box 655303
 *  Dallas, Texas 75265
 *  Contact information: 
 *  http://www-k.ext.ti.com/sc/technical-support/product-information-centers.htm?
 *  DCMP=TIHomeTracking&HQS=Other+OT+home_d_contact
 *  ============================================================================
 *  
 */


metaonly module Platform inherits xdc.platform.IPlatform {

    config ti.platforms.generic.Platform.Instance plat =
        ti.platforms.generic.Platform.create("plat", {
            clockRate:      700,
            catalogName:    "ti.catalog.c6000",
            deviceName:     "TMS320C6472",
            customMemoryMap: [
                ["DDR2",
                    {name: "DDR2",        base: 0xe4000000, len: 0x01000000, space: "code/data",access: "RWX"}],
                ["SR0",
                    {name: "SR0",         base: 0x00200000, len: 0x00040000, space: "code/data",access: "RWX"}],
                ["SR1",
                    {name: "SR1",         base: 0x00250000, len: 0x0006F000, space: "code/data",access: "RWX"}],
                ["LL2RAM",
                    {name: "LL2RAM",      base: 0x00800000, len: 0x00098000, space: "code/data",access: "RWX"}],
                ["L1PSRAM",
                    {name: "L1PSRAM",     base: 0x00E00000, len: 0x0008000, space: "code",access: "RWX"}],
                ["L1DSRAM",
                    {name: "L1DSRAM",     base: 0x00F00000, len: 0x008000 , space: "data",access: "RW"}],
            ],
          l1DMode:"32k",
          l1PMode:"32k",
          l2Mode:"0k",
    });

instance :

    override config string codeMemory  = "LL2RAM";
    override config string dataMemory  = "LL2RAM";
    override config string stackMemory = "LL2RAM";
}
