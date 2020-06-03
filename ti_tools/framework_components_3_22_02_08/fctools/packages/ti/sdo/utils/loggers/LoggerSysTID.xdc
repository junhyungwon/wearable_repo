/* 
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

import xdc.runtime.ITimestampClient;
import xdc.runtime.IGateProvider;

/*
 *  ======== LoggerSysTID.xdc ========
 *
 */

/*!
 *  ======== LoggerSysTID ========
 *  A logger which routes events to the system `printf` function.
 *
 *  This logger processes log events as they are generated and routes
 *  them through the `{@link System#printf System_printf()}` function.
 *  The final disposition of the log event is dependent on which system
 *  provider has been assigned to the
 *  `{@link System#SupportProxy System.SupportProxy}` configuration parameter.
 *
 *  Note that the log events are processed within the runtime context
 *  of the `{@link Log Log_write()}` or `{@link Log Log_print()}` function
 *  that generated the event. It is important to account for the processing
 *  overhead and stack usage imposed on the runtime context. The cost of
 *  this processing is defined by the implementation of the system provider.
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
 *  var LoggerSys = xdc.useModule('xdc.runtime.LoggerSys');
 *
 *  var LoggerSysParams = new LoggerSys.Params();
 *  Defaults.common$.logger = LoggerSys.create(LoggerSysParams);
 *  Diags.setMaskMeta("my.pkg.%", Diags.USER1, Diags.RUNTIME_ON);
 *  @p
 */

@ModuleStartup      /* Initialize module */
@Gated

module LoggerSysTID inherits ti.sdo.utils.loggers.ILoggerMFP { // xdc.runtime.ILogger {

    /*! Timestamp display format */
    enum TSMode {
        TSMode_USEC,        /*! Timestamps displayed in microseconds */
        TSMode_SEC,         /*! Timestamps displayed in seconds */
        TSMode_DELTAUSEC,   /*! Timestamp differences in microseconds */
        TSMode_TICKS        /*! Timestamps displayed in timer counter ticks */
    };

    /*!
     *  ======== control ========
     *  A hook for sending commands to the logger. For example, this can
     *  be used to re-configure the timestamp display format.
     *
     *  @param(cmd)         control command
     *  @param(cmdArgs)     command argument
     */
//    Void control(Int cmd, UArg cmdArgs);

    /*!
     *  ======== ITimestampProxy ========
     *  User supplied time-stamp proxy
     *
     *  This proxy allows `LoggerSys` to use a timestamp server different
     *  from the server used by `{@link xdc.runtime.Timestamp}`. However, if
     *  not supplied by a user, this proxy defaults to whichever timestamp
     *  server is used by `Timestamp`.
     */
    proxy TimestampProxy inherits ITimestampClient;

    // Get a handle to the module's gate.
    config IGateProvider.Handle gate;

instance:

    /*!
     *  ======== create ========
     *  Create a `LoggerSys` logger
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
/*
 *  @(#) ti.sdo.utils.loggers; 1, 0, 0,1; 2-15-2012 07:44:56; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

