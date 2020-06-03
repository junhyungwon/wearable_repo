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

/*
 *  ======== getLibs ========
 */
function getLibs(prog)
{
    var profile = this.profile;
    var suffix;
    var libName = "/loggers.a";
    var lib;

    // XDCtools 3.10 has new findSuffix() function
    if ("findSuffix" in prog.build.target) {
        suffix = prog.build.target.findSuffix(this);
        if (suffix == null) {
            return "";  // nothing to contribute
        }
    }
    else {
        /*
         * Might assert here, instead.  This pkg needs newer XDC
         * that should have .findSuffix()
         */
        suffix = prog.build.target.suffix;
    }

    if (suffix == null) {
        return ("");
    }

    lib = "lib/" + profile + libName + suffix;

    // If the library doesn't exist, maybe we don't build for the given
    // profile. Choose the release profile.
    if (!java.io.File(this.packageBase + lib).exists()) {
        print(this.$name + "': Unable to locate lib for requested '" +
                this.profile + "' profile.  Using 'release' profile.");
        lib = "lib/release" + libName + suffix;
    }

    return (lib);
}
/*
 *  @(#) ti.sdo.utils.loggers; 1, 0, 0,1; 2-15-2012 07:44:56; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

