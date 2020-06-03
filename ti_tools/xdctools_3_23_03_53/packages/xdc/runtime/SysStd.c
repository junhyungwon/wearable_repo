/* 
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
 * */
/*
 *  ======== SysStd.c ========
 */
 
#include <xdc/std.h>

#include <stdio.h>
#include <stdlib.h>

#include "package/internal/SysStd.xdc.h"

/*
 *  ======== SysStd_abort ========
 */
Void SysStd_abort(String str)
{
    Char ch;
   
    if (str != NULL) {
        while ((ch = *str++) != '\0') {
            putchar(ch);
        }
    }
    fflush(stdout);
}

/*
 *  ======== SysStd_exit ========
 */
Void SysStd_exit(Int stat)
{
    fflush(stdout);
}

/*
 *  ======== SysStd_putch ========
 */
Void SysStd_putch(Char ch)
{
    putchar(ch);
}

/*
 *  ======== SysStd_flush ========
 */
Void SysStd_flush()
{
    fflush(stdout);
}

/*
 *  ======== SysStd_ready ========
 */
Bool SysStd_ready()
{
    return (TRUE);
}
/*
 *  @(#) xdc.runtime; 2, 1, 0,375; 4-27-2012 14:30:34; /db/ztree/library/trees/xdc/xdc-y25x/src/packages/
 */

