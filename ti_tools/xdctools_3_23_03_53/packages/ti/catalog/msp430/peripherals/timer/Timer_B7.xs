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
 *  ======== Timer_B7.xs ========
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
        case 3:
            this.TBCCR3 = value;
            break;
        case 4:
            this.TBCCR4 = value;
            break;
        case 5:
            this.TBCCR5 = value;
            break;
        case 6:
            this.TBCCR6 = value;
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
        case 3:
            return this.TBCCR3;
            break;
        case 4:
            return this.TBCCR4;
            break;
        case 5:
            return this.TBCCR5;
            break;
        case 6:
            return this.TBCCR6;
            break;
        default:
            return null;
            break;
    }
}
