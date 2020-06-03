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
 *  ======== Timer_A2.xs ========
 */

/*
 *  ======== setTxxCCRx ========
 */
function setTxxCCRx(ccrNumber , value) {
    switch (ccrNumber) {
        case 0:
            this.TACCR0 = value;
            break;
        case 1:
            this.TACCR1 = value;
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
            return this.TACCR0;
            break;
        case 1:
            return this.TACCR1;
            break;
        default:
            return null;
            break;
    }
}
