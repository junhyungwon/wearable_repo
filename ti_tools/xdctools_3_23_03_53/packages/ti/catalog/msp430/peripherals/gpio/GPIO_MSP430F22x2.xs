/* --COPYRIGHT--,EPL
 *  Copyright (c) 2008 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 * 
 *  Contributors:
 *      Texas Instruments - initial implementation
 * 
 * --/COPYRIGHT--*/

/*
 *  ======== GPIO_MSP430F22x2.xs ========
 */

var IMod;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
	/* Store a reference to the module and call super's initializer */
    IMod = this.$super;
    IMod.module$meta$init.$apply(this, []);
}

/*
 *  ======== instance$meta$init ========
 */
function instance$meta$init(adc10, clock) 
{
	/* Call super's initializer */
    IMod.instance$meta$init.$apply(this, []);

	/* Initialize this config */
    this.adc10 = adc10;
    this.clock = clock;
}
