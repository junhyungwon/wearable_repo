/** 
 *  @file   IpcHost.h
 *
 *  @brief  This file contains APIs and defines that are used by applications
 *          only on the host-side of the device.
 *
 *
 */
/* 
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



#ifndef _IPCHOST_H__
#define _IPCHOST_H__


#if defined (__cplusplus)
extern "C" {
#endif

/**
 *  @file       IpcHost.h
 *
 *  @remarks    The typical flow for an IPC-using application is to first
 *              load the processor (typically via slaveloader or the ProcMgr
 *              APIs), and then use the Ipc_control() API to attach/detach
 *              from the running slave.
 *
 *  @par Example:
 *
 *  @code
 *      // invoke the SysLink load callback
 *      remoteProcId = MultiProc_getId("DSP");
 *      status = Ipc_control(remoteProcId, Ipc_CONTROLCMD_LOADCALLBACK, NULL);
 *
 *      // invoke the SysLink start callback
 *      status = Ipc_control(remoteProcId, Ipc_CONTROLCMD_STARTCALLBACK, NULL);
 *
 *      // IPC is established, app-specific code is here
 *
 *      // invoke the SysLink stop callback
 *      status = Ipc_control(remoteProcId, Ipc_CONTROLCMD_STOPCALLBACK, NULL);
 *  @endcode
 */

/**
 *  @brief      Control command ID for load callback.
 *
 *  @remarks    This command must be sent (via Ipc_control()) before using IPC
 *              with the slave, and after loading the slave.
 *
 *  @remarks    This call ensures resources (e.g. Shared Region 0) are visible
 *              to the slave (for example, by enabling the slave-side MMU).
 *              This call then typically enables the slave-side call to
 *              Ipc_start() to succeed.
 *
 *  @remarks    When ProcMgr is used to load the slave (the typical case),
 *              the @c arg passed to Ipc_control() should be NULL.
 *
 *  @remarks    When ProcMgr is <i>not</i> used to load the slave (supported,
 *              but not the typical case), the @c arg passed to Ipc_control()
 *              must be a pointer to the slave-side address of the 
 *              @c _Ipc_ResetVector symbol.  This is often obtained by 
 *              inspecting the slave-side executable's .map file.
 *
 *  @sa         Ipc_control()
 */
#define Ipc_CONTROLCMD_LOADCALLBACK  (0xBABE0000)


/**
 *  @brief      Control command ID for start callback.
 *
 *  @remarks    This command must be sent (via Ipc_control()) before using IPC
 *              with the slave, and after sending the
 *              #Ipc_CONTROLCMD_LOADCALLBACK command.
 *
 *  @remarks    This call performs a handshake with the slave processor,
 *              typically enabling the slave-side call to Ipc_attach() to
 *              succeed.
 *
 *  @remarks    When sending this command, the @c arg passed to Ipc_control()
 *              should be NULL.
 *
 *  @sa         Ipc_control()
 */
#define Ipc_CONTROLCMD_STARTCALLBACK (0xBABE0001)


/**
 *  @brief      Control command ID for stop callback.
 *
 *  @remarks    This command must be sent (via Ipc_control()) to detach from
 *              an attached slave.
 *
 *  @remarks    This call first performs a handshake with the slave processor,
 *              typically enabling the slave-side calls to Ipc_detach() and
 *              Ipc_stop() to succeed.  It then may make resources
 *              (e.g. Shared Region 0) inaccessible to the slave (for example
 *              by disabling the slave's MMU).
 *
 *  @remarks    When ProcMgr is used to load the slave (the typical case),
 *              the @c arg passed to Ipc_control() should be NULL.
 *
 *  @remarks    When ProcMgr is <i>not</i> used to load the slave (supported,
 *              but not the typical case), the @c arg passed to Ipc_control()
 *              must be a pointer to the slave-side address of the 
 *              @c _Ipc_ResetVector symbol.  This is often obtained by 
 *              inspecting the slave-side executable's .map file.
 *
 *  @sa         Ipc_control()
 */
#define Ipc_CONTROLCMD_STOPCALLBACK  (0xBABE0002)


/**
 *  @brief      Perform IPC-related control operations with a specific slave.
 *
 *  @param[in]  procId  Remote processor ID
 *  @param[in]  cmdId   Command ID
 *  @param[in]  arg     Argument.  The value of @c arg depends on the command
 *                      being sent in @c cmdId.
 *
 *  @remarks    Each specific @c cmdId describes its usage of the @c arg param.
 *
 *  @sa         Ipc_CONTROLCMD_LOADCALLBACK
 *  @sa         Ipc_CONTROLCMD_STARTCALLBACK
 *  @sa         Ipc_CONTROLCMD_STOPCALLBACK
 */
Int Ipc_control (UInt16 procId, Int32 cmdId, Ptr arg);


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* _IPCHOST_H__ */
