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
 *  ======== Boot.xdc ========
 *
 */

package ti.catalog.c2800.concertoInit;

/*!
 *  ======== Boot ========
 *  Concerto C28 Boot Support.
 *
 *  This Boot module can be used to generate a small section that supports 
 *  booting the C28 processor from Flash.  It consists of a single 
 *  "LB _c_int00" instruction.  
 *
 *  When `{@link #bootFromFlash}` is true, the instruction will be placed
 *  at "BEGIN", as defined in the linker command file.
 */
@NoRuntime
module Boot
{
    /*!
     *  Boot from FLASH flag.  Default is true.
     *
     *  Set to true to enable booting the C28 from Flash.
     */
    metaonly config Bool bootFromFlash = true;
};   
/*
 *  @(#) ti.catalog.c2800.concertoInit; 1, 0, 0,27; 4-24-2012 14:55:55; /db/ztree/library/trees/platform/platform-n20x/src/
 */

