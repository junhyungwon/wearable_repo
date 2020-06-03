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
 *  ======== ADC10.xs ========
 */

var ADC10;
var GetSet;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    ADC10 = this;
    GetSet = xdc.module("xdc.services.getset.GetSet");
}

/*
 *  ======== instance$meta$init ========
 */
function instance$meta$init(clock)
{
    this.clock = clock;
    GetSet.init(this);
}

/*
 *  ======== instance_validate ========
 */
function instance_validate(inst)
{
    /* add error checking / resource constraints here */
    switch (inst.ADC10CTL1.CONSEQ) {
        case ADC10.CONSEQ_0:
            inst.ADC10CTL0.MSC = ADC10.MSC_OFF;
            break;
        case ADC10.CONSEQ_1:
            break;
        case ADC10.CONSEQ_2:
            break;
        case ADC10.CONSEQ_3:
            break;
        default:
            break;
    }
    
    /* When more than 1 ADC channel selected, check that CONSEQ can only be 1 or 3 */
    
    
    /* When single ADC channel selected, provide warning if CONSEQ is set as 1 or 3  */
    
    /* When an ADC channel is selected, set ADC10AE0 appropriately */
    
    /* Get clock type and frequency. Calculate possible sampling time based on divider. */

    /* If DTC is enabled, if Memory block Size == 0, set Memory Block size = 1  */
    
    /* If  Memory Block size > 0, set DTC Enabled. */
    
    /* If DTC is enabled, check if address is entered, else provide a warning */
    
    /* When VCC is set for ADC, automatically set REFON and 2.5V. Provide warning to user to enable other ADC, ADC neeeds to be re-configured during runtime.*/
    
}

/*
 *  ======== module$validate ========
 */
function module$validate() 
{
    /* boilerplate text to validate each instance */
    for each (var inst in this.$instances) {
        instance_validate(inst);
    }
}

