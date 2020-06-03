/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/



#include "system_priv_ipc.h"
#include <mcfw/interfaces/link_api/ipcLink.h>
#include <ti/syslink/SysLink.h>
#include <ti/syslink/ProcMgr.h>

System_IpcObj gSystem_ipcObj;


Int32 System_printProcId(UInt32 procId)
{
    char *procName;
    UInt32 syslinkProcId;

    procName = System_getProcName(procId);
    syslinkProcId = System_getSyslinkProcId(procId);

    if(syslinkProcId == MultiProc_INVALIDID)
    {
        printf(" %u: SYSTEM: CPU [%s] is NOT available on this platform !!!\n",
            OSA_getCurTimeInMsec(),
            procName
            );
    }
    else
    {
        printf(" %u: SYSTEM: CPU [%s] syslink proc ID is [%d] !!!\n",
            OSA_getCurTimeInMsec(),
            procName,
            syslinkProcId
            );
    }

    return 0;
}

Int32 System_ipcInit()
{
    #ifdef SYS_DEBUG
    //OSA_printf(" %u: SYSTEM: IPC init in progress !!!\n", OSA_getCurTimeInMsec());
    #endif

    SysLink_setup ();

    {
        UInt32 procId;

        for(procId=0; procId<SYSTEM_PROC_MAX; procId++)
            System_printProcId(procId);
    }

    System_ipcMsgQInit();

    System_ipcNotifyInit();

	#ifdef SYS_DEBUG
    OSA_printf(" %u: SYSTEM: IPC init DONE !!!\n", OSA_getCurTimeInMsec());
    #endif

    return OSA_SOK;
}

Int32 System_ipcDeInit()
{
	#ifdef SYS_DEBUG
    //OSA_printf(" %u: SYSTEM: IPC de-init in progress !!!\n", OSA_getCurTimeInMsec());
    #endif

    System_ipcNotifyDeInit();

    System_ipcMsgQDeInit();

    SysLink_destroy ();

	#ifdef SYS_DEBUG
    OSA_printf(" %u: SYSTEM: IPC de-init DONE !!!\n", OSA_getCurTimeInMsec());
    #endif

    return OSA_SOK;
}

