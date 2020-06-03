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
 *  ======== LoggerCallback.xdc ========
 */

/*!
 *  ======== LoggerCallback ========
 *  A logger that passes events to a user supplied call back function
 *
 *  @a(Examples)
 *  Configuration example: The following XDC configuration statements
 *  create a logger instance, assign it as the default logger for all
 *  modules, and enable `USER1` logging in all modules of the package
 *  `my.pkg`. See the `{@link Diags#setMaskMeta Diags.setMaskMeta()}` function
 *  for details on specifying the module names.
 *
 *  @p(code)
 *  var Defaults = xdc.useModule('xdc.runtime.Defaults');
 *  var Diags = xdc.useModule('xdc.runtime.Diags');
 *  var LoggerCallback = xdc.useModule('xdc.runtime.LoggerCallback');
 *
 *  var LoggerCallbackParams = new LoggerCallback.Params();
 *  LoggerCallbackParams.callbackFxn = "&myCallbackFxn";
 *
 *  Defaults.common$.logger = LoggerCallback.create(LoggerCallbackParams);
 *  Diags.setMaskMeta("my.pkg.%", Diags.USER1, Diags.RUNTIME_ON);
 *  @p
 */

module LoggerCallback inherits xdc.runtime.ILogger {

    /*!
     *  ======== ITimestampProxy ========
     *  User supplied time-stamp proxy
     *
     *  This proxy allows `LoggerCallback` to use a timestamp server different
     *  from the server used by `{@link xdc.runtime.Timestamp}`. However, if
     *  not supplied by a user, this proxy defaults to whichever timestamp
     *  server is used by `Timestamp`.
     */
    proxy TimestampProxy inherits xdc.runtime.ITimestampClient;

    /*!
     *  ======== CallbackFxn ========
     *  Callback character callback function
     *
     *  This function is called each time a Log event is generated by the 
     *  application.
     */
    typedef Void (*CallbackFxn)(xdc.runtime.Log.EventRec *);

    /*!
     *  ======== callbackFxn ========
     *  User suplied character callback function
     *
     *  If this parameter is set to a non-`null` value, the specified
     *  function will be called for each character received.
     *
     *  For example, if you define a function named `myCallbackFxn`, the
     *  following configuration fragment will cause `LoggerCallback` to call
     *  `myCallbackFxn` whenever a character is received.
     *  @p(code)
     *      var LoggerCallback = xdc.useModule("xdc.runtime.LoggerCallback");
     *      LoggerCallback.callbackFxn = "&myCallbackFxn";
     *  @p
     *
     *  If this parameter is not set, a default function will be used which
     *  simply drops the callback characters.
     *
     *  @see #CallbackFxn
     */
    config CallbackFxn callbackFxn = null;

instance:

    /*!
     *  ======== create ========
     *  Create a `LoggerCallback` logger
     *     
     *  The logger instance will route all log events it receives to
     *  the {@link System#printf} function.
     */
    create();
 
internal:
    
    struct Instance_State {
        Bool enabled;
    };
}
