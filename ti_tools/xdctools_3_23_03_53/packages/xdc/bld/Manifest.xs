/*
 *  ======== getAllFiles ========
 */
function getAllFiles(excludeDirs)
{
    if (xdc.om.$name != "rel") {
        throw new Error(
            "Manifest.getAllFiles can only be called from within a release script");
    }

    /* clear current files list */
    this.files = [];

    /* re-populate based on excludeDirs */
    /* rcl is alredy loaded by the release model initialization rcl.js */
    rcl.getFiles(this.files, excludeDirs);
}
/*
 *  @(#) xdc.bld; 1, 0, 2,379; 4-27-2012 14:30:16; /db/ztree/library/trees/xdc/xdc-y25x/src/packages/
 */

