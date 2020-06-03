#
#  Copyright 2012 by Texas Instruments Incorporated.
#  
#

#!/bin/sh
#
unset INCLUDE
unset LIB
unset CL

dir=${1%/*}
cmdbin=/cygdrive/${dir%%:*}${dir#?:}
export PATH=$cmdbin:$cmdbin/../../common/msdev98/bin

"$@"
#
#  @(#) microsoft.targets; 1, 0, 2, 0,534; 4-27-2012 17:07:29; /db/ztree/library/trees/xdctargets/xdctargets-f21x/src/ xlibrary

#

