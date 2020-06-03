/*
 *  Copyright 2012 by Texas Instruments Incorporated.
 *
 */

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    if (xdc.om.$name == 'cfg') {
        /* set default SupportProxy to LoggerMFPNull */
        this.Proxy = xdc.module('ti.sdo.utils.loggers.LoggerMFPNull');
    }
}
/*
 *  @(#) ti.sdo.utils.loggers; 1, 0, 0,1; 2-15-2012 07:44:55; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

