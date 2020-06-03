/*
 *  Copyright 2012 by Texas Instruments Incorporated.
 *
 */

/*
 *  ======== hello.c ========
 *  The hello example serves as a basic sanity check program for DSP/BIOS. It
 *  demonstrates how to print the string "hello world" to stdout. The DSP/BIOS 
 *  configuration script for this example is located in the ../common directory.
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>

/*
 *  ======== main ========
 */
Void main()
{ 
    System_printf("hello world\n");

    BIOS_exit(0);
}


/*
 *  @(#) ti.sysbios.genx; 2, 0, 0, 0,527; 5-18-2012 06:05:28; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

