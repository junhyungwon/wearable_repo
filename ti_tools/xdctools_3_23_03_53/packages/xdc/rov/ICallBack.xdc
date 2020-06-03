/* 
 *  Copyright (c) 2008 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 * 
 *  Contributors:
 *      Texas Instruments - initial implementation
 * 
 * */
package xdc.rov

metaonly interface ICallBack {

instance:

    /*!
     *  ======== updateStartupProgress ========
     *  The ROV model will call this API at particular points to update the
     *  startup progress.
     */
    Void updateStartupProgress(Int percent, String message);
    
    /*!
     *  ======== getAbortFlag ========
     */
    Bool getAbortFlag();
}

/*
 *  @(#) xdc.rov; 1, 0, 0,376; 4-27-2012 14:30:28; /db/ztree/library/trees/xdc/xdc-y25x/src/packages/
 */

