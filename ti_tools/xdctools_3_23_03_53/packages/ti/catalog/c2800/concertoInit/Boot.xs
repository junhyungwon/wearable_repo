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
 *  ======== Boot.xs ========
 *
 */

var Boot = null;
var Program = null;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }
    Boot = this;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Program = xdc.useModule('xdc.cfg.Program');

    if (Program.build.rtsName !== null) {

        if ( (Boot.bootFromFlash == true) && 
             (Program.sectMap[".ti_catalog_c2800_concertoInit_begin"] === undefined)) {
            Program.sectMap[".ti_catalog_c2800_concertoInit_begin"] = 
                new Program.SectionSpec();
            Program.sectMap[".ti_catalog_c2800_concertoInit_begin"].loadSegment = 
                "BEGIN";
        }
    }
}

/*
 *  ======== module$validate ========
 */
function module$validate()
{
}
/*
 *  @(#) ti.catalog.c2800.concertoInit; 1, 0, 0,27; 4-24-2012 14:55:55; /db/ztree/library/trees/platform/platform-n20x/src/
 */

