/* --COPYRIGHT--,ESD
 *  Copyright (c) 2008 Texas Instruments. All rights reserved. 
 *  This program and the accompanying materials are made available under the 
 *  terms of the Eclipse Public License v1.0 and Eclipse Distribution License
 *  v. 1.0 which accompanies this distribution. The Eclipse Public License is
 *  available at http://www.eclipse.org/legal/epl-v10.html and the Eclipse
 *  Distribution License is available at 
 *  http://www.eclipse.org/org/documents/edl-v10.php.
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 * --/COPYRIGHT--*/
/*
 *  ======== GateGIE.c ========
 */
#include  <ti/mcu/msp430/include/msp430x22x4.h>

#include "package/internal/GateGIE.xdc.h"

/*
 *  ======== enter ========
 */
IArg GateGIE_enter(GateGIE_Object *obj)
{
    IArg key = _get_SR_register();  /* get current interrupt state */
    _disable_interrupts();          /* disable interrupts */
    return (key);
}

/*
 *  ======== leave ========
 */
Void GateGIE_leave(GateGIE_Object *obj, IArg key)
{
    if (key & GIE) {
        /* re-enable interrupts _only_ if we disabled them */
        _enable_interrupts();
    }
}

/*
 *  ======== query ========
 */
Bool GateGIE_query(Int qual)
{
    return (FALSE);
}
