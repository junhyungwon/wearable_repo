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
 *  ======== TMS320C6711.xdc ========
 *
 */
package ti.catalog.c6000;

/*!
 *  ======== TMS320C6711 ========
 *  The C6711B device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is 
 *  used by platforms to obtain "data sheet" information about this device.
 */
metaonly module TMS320C6711 inherits ITMS320C6x1x
{

instance:
    override config string   cpuCore        = "6700";
    override config string   isa = "67";

    /*!
     *  ======== memMap ========
     *  The default memory map for this device
     */
 
    override config xdc.platform.IPlatform.Memory memMap[string] = TMS320C6211.memMap;
};
/*
 *  @(#) ti.catalog.c6000; 1, 0, 0, 0,390; 4-24-2012 14:56:07; /db/ztree/library/trees/platform/platform-n20x/src/
 */

