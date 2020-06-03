/*
 *  @file   RingIOGppApp.c
 *
 *  @brief      Application for testing functionality of RingIO module
 *
 *
 *  ============================================================================
 *
 *  Copyright (c) 2008-2012, Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  
 *  *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  
 *  *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  
 *  *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  Contact information for paper mail:
 *  Texas Instruments
 *  Post Office Box 655303
 *  Dallas, Texas 75265
 *  Contact information: 
 *  http://www-k.ext.ti.com/sc/technical-support/product-information-centers.htm?
 *  DCMP=TIHomeTracking&HQS=Other+OT+home_d_contact
 *  ============================================================================
 *  
 */



#include <xdc/std.h>
#include <string.h>

/*  -----------------------------------XDC.RUNTIME module Headers    */
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/*  ----------------------------------- IPC module Headers           */
#include <ti/sysbios/knl/Task.h>
#include <ti/ipc/Ipc.h>

/*  ----------------------------------- Notify module Headers        */
#include <ti/ipc/Notify.h>

/*  ----------------------------------- BIOS6 module Headers         */
#include <ti/sysbios/BIOS.h>

/*  ----------------------------------- SysLink module Headers         */
#include <ti/syslink/RingIO.h>
#include <ti/syslink/SysLink.h>


/*  ----------------------------------- To get globals from .cfg Header */
#include <xdc/cfg/global.h>

Void tsk1_func(UArg arg0, UArg arg1) ;

/** ============================================================================
 *  Functions
 *  ============================================================================
 */
Int main(Int argc, Char* argv[])
{
    Int status = Ipc_S_SUCCESS;
    Task_Handle  tskHandle;
    Task_Params  tskParams;

    Task_Params_init(&tskParams) ;
    System_printf ( "Default stack size %d \n",tskParams.stackSize) ;
    tskParams.stackSize = 0x2000 ;

    /* Create task 1 function */
    tskHandle = Task_create (tsk1_func,
                             &tskParams,
                             NULL) ;
    if (tskHandle == NULL) {
       System_printf ( "ERROR: Failed to crate task tsk1_func \n") ;
    }

    do {
        /* Call Ipc_start() */
        status = Ipc_start();
    } while (status != Ipc_S_SUCCESS);
    BIOS_start ();

    return (0) ;
}

Void tsk1_func(UArg arg0, UArg arg1)
{
    Int32                   status        = 0;
    
    SysLink_setup();
    
    do {
        status = Ipc_attach(REMOTEPROCID);
    } while (status < 0);
}

