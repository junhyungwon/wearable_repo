/*
 *  Copyright 2012 by Texas Instruments Incorporated.
 *
 */

/*
 *  ======== getVersion ========
 */
function getVersion()
{
    print("Warning: gnu.targets.codesourcery targets are deprecated. Please "
        + "use\ngnu.targets.arm targets instead. See the documentation for "
        + "gnu.targets.arm\nand gnu.targets.codesourcery packages for more "
        + "information.");
    return this.$super.getVersion();
}
 
/*
 *  @(#) gnu.targets.codesourcery; 1, 0, 0, 0,389; 4-27-2012 17:31:30; /db/ztree/library/trees/xdctargets/xdctargets-f21x/src/ xlibrary

 */

