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
 *  ======== Timer1_A2.xs ========
 */

/*
 *  ======== setTxxCCRx ========
 */
function setTxxCCRx(ccrNumber , value) {
    switch (ccrNumber) {
        case 0:
            this.TA1CCR0 = value;
            break;
        case 1:
            this.TA1CCR1 = value;
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
            return this.TA1CCR0;
            break;
        case 1:
            return this.TA1CCR1;
            break;
        default:
            return null;
            break;
    }
}
