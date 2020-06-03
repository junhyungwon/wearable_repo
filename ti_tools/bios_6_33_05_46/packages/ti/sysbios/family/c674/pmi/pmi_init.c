/*
 *  Copyright 2012 by Texas Instruments Incorporated.
 *
 */

/*
 *  ======== pmi_init.c ========
 *
 *  PMI module initialization.
 *
 */

#include <ti/sysbios/family/c674/pmi.h>

/*
 *  ======== PMI_init ========
 */
PMI_Status PMI_init(void)
{
    PMI_Status status = PMI_OK;
    static unsigned initialized = 0;

    if (initialized == 0) {
        PMI_initPLL(PMI_CPU);
        PMI_initPLL(PMI_PER);
        PMI_initVoltageControl();
        initialized = 1;
    }

    return(status);
}


/*
 *  @(#) ti.sysbios.family.c674.pmi; 1, 0, 0,24; 5-18-2012 06:05:10; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

