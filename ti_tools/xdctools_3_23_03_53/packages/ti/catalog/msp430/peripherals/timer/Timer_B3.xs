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
 *  ======== Timer_B3.xs ========
 */

/*
 *  ======== instance$meta$init ========
 */
function instance$meta$init(clock) 
{
    this.clock = clock;
}

/*
 *  ======== module$meta$init ========
 */
function module$meta$init() 
{
}

/*
 *  ======== setTxxCCRx ========
 */
function setTxxCCRx(ccrNumber , value) 
{
    switch (ccrNumber) {
        case 0:
            this.TBCCR0 = value;
            break;
        case 1:
            this.TBCCR1 = value;
            break;
        case 2:
            this.TBCCR2 = value;
            break;
        default:
    }
}

/*
 *  ======== getTxxCCRx ========
 */
function getTxxCCRx(ccrNumber)
{
    switch (ccrNumber) {
        case 0:
            return this.TBCCR0;
            break;
        case 1:
            return this.TBCCR1;
            break;
        case 2:
            return this.TBCCR2;
            break;
        default:
            return null;
            break;
    }
}
