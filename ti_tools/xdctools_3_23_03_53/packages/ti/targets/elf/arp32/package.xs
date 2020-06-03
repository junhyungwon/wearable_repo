/* 
 *  Copyright (c) 2008 Texas Instruments and others.
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
 *  ======== init ========
 *  Initialize this package.
 */
function init()
{
    print("Warning: The 'ti.targets.elf.arp32' package " +
        "is deprecated. Please use 'ti.targets.arp32.elf' instead.");

    /* initialize module configuration params that are computed */
    var ma = this.$modules;
    var tiTargets = xdc.loadPackage('ti.targets');
    for (var i = 0; i < ma.length; i++) {
        if (ma[i] instanceof tiTargets.ITarget.Module) {
            var targ = tiTargets.ITarget.Module(ma[i]);
            var isas = targ.getISAChain(null);

            if (isas.length > 0) {
                /* add target-specific assembly-language extensions */
                for (var idx = 0; idx < isas.length; idx++) {
                    var ext = ".s" + isas[idx];
                    targ.extensions.$putHead(ext,
                        new xdc.om['xdc.bld.ITarget.Extension'](
                            {suf: ext, typ: "asm"}
                        )
                    );
                }
            }
            else {
                print("Warning: ISAChain is empty for target " + targ);
            }

            /* add .s<suffix> to list of recognized extensions */
            var ext = ".s" + targ.suffix;
            targ.extensions.$putHead(ext,new xdc.om['xdc.bld.ITarget.Extension']({
                suf: ext,   typ: "asm"
            }));

        }
    }
}

/*
 *  ======== Package.getLibs ========
 *  This package's implementation of xdc.IPackage.getLibs.
 */
function getLibs()
{
    return (null);    /* no library required for this package */
}

/*
 *  @(#) ti.targets.elf.arp32; 1, 0, 0,70; 4-27-2012 17:32:04; /db/ztree/library/trees/xdctargets/xdctargets-f21x/src/ xlibrary

 */

