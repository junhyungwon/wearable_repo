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
 *  ======== Timer0_A3.xs ========
 */

/*
 *  ======== setTxxCCRx ========
 */
function setTxxCCRx(ccrNumber , value) {
    switch (ccrNumber) {
        case 0:
            this.TA0CCR0 = value;
            break;
        case 1:
            this.TA0CCR1 = value;
            break;
        case 2:
            this.TA0CCR2 = value;
            break;
        default:
            break;
    }
}

/*
 *  ======== getTxxCCRx ========
 */
function getTxxCCRx(ccrNumber) {
    switch (ccrNumber) {
        case 0:
            return this.TA0CCR0;
            break;
        case 1:
            return this.TA0CCR1;
            break;
        case 2:
            return this.TA0CCR2;
            break;
        default:
            return null;
            break;
    }
}
