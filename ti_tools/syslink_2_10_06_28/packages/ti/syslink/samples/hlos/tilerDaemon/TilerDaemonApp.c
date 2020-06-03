/*
 *  @file   TilerDaemonApp.c
 *
 *  @brief      xdc file
 *
 *
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



/*
 *  ======== AppTest.c ========
 *
 *  Overview
 *  ----------------
 *   1. SysLink initialize (SysLink_setup)
 *   2. Map application shared region
 *   3. Initialize remote program state
 *   4. Create message queue (for commands)
 *   5. Load and run remote processor
 *   6. Wait for remote processor ready signal
 *   7. Setup ipc layer, open shared resources
 *
 *   8. Send start message to remote processor
 *   9. Wait for shutdown message
 *
 *  10. Finalize ipc layer, close shared resources
 *  11. Send shutdown signal to remote processor
 *  12. Wait for remote processor done signal
 *  13. Stop remote processor
 *  14. Delete message queue
 *  15. Unmap application shared region
 *  16. SysLink finalize (SysLink_destroy)
 *
 */
/* Standard headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>


/* OSAL & Utils headers */
#include <xdc/runtime/Error.h>
#include <xdc/runtime/knl/Thread.h>

/* Linux header files (temporary, to be removed) */

/* Module header files */
#include <ti/sdo/tiler/MemMgr.h>
#include <ti/sdo/tiler/Settings.h>
#include <ti/syslink/ProcMgr.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/Notify.h>

/* Utils header files */
#include <ti/ipc/MultiProc.h>
#include <ti/syslink/SysLink.h>



static ProcMgr_Handle       gProcMgrHndl = NULL;
static MemMgr_Daemon_Handle gDaemonHndl;
ProcMgr_AddrInfo            addrInfo;

/*!
 *  @brief  Event number to be used for the sample application.
 */
#define  TILERDAEMONAPP_EVENT_NO           10u

/*
 *  ======== app_main ========
 */
Int TilerDaemonApp_startup (Void)
{
    Int32                status = 0;
    Ptr                  gAppSharedData;

    System_printf ("\nEntered TilerDaemonApp_startup\n");

    System_printf("\n======== initialize SysLink ========\n");

    /* SysLink initialize */
    SysLink_setup();

    System_printf("\nMemMgr_createDaemon(name: %s)\n",ti_sdo_tiler_Settings_tilerDaemonName);
    gDaemonHndl = MemMgr_createDaemon(ti_sdo_tiler_Settings_tilerDaemonName);

    if (gDaemonHndl == NULL) {
        System_printf ("\nError in MemMgr_createDaemon\n");
    }
    else {
        System_printf ("\nMemMgr_createDaemon successful\n");
    }

    System_printf("\n======== ProcMgr_open ========\n");

    /* map application shared region into current process address space */
    status = ProcMgr_open(&gProcMgrHndl, MultiProc_getId("VIDEO-M3"));

    if (status < 0) {
        System_printf("\nProcMgr_open failed\n");
    }

    System_printf("\n======== ProcMgr_map ========\n");

    /* map application shared region into current process address space */
    addrInfo.addr[ProcMgr_AddrType_MasterPhys] = Global_appSharedRegionBase;
    addrInfo.size = Global_appSharedRegionLength;
    addrInfo.isCached = FALSE;

    status = ProcMgr_map(gProcMgrHndl,
        (ProcMgr_MapType_MasterKnlVirt | ProcMgr_MapType_MasterUsrVirt),
        &addrInfo, ProcMgr_AddrType_MasterPhys);

    if (status < 0) {
        System_printf("\nProcMgr_map failed\n");

    }
    System_printf("\n======== SharedRegion_add ========\n");
    gAppSharedData = (Ptr)addrInfo.addr[ProcMgr_AddrType_MasterUsrVirt];
    System_printf("\nuser virtual addr = [0x%x] \n",gAppSharedData);

//     status = SharedRegion_add(Global_appSharedRegionIndex,
//                               gAppSharedData, Global_appSharedRegionLength);
//     if (status < 0) {
//         System_printf("\nSharedRegion_add failed\n");
//     }

    System_printf ("\nLeaving TilerDaemonApp_startup\n");
    return(status);
}


Int TilerDaemonApp_shutdown (Void)
{
    Int32 status = 0;
    System_printf ("\nEntered TilerDaemonApp_shutdown\n");

    System_printf("\n======== finalize shared region ========\n");

//     /* remove the mapped memroy from shared region table */
//     System_printf("\nSharedRegion_remove()\n");
//     SharedRegion_remove(Global_appSharedRegionIndex);

    /* unmap the shared memory */
    addrInfo.addr[ProcMgr_AddrType_MasterPhys] = Global_appSharedRegionBase;
    addrInfo.size = Global_appSharedRegionLength;
    addrInfo.isCached = FALSE;

    System_printf("\nProcMgr_unmap()\n");
    status = ProcMgr_unmap(gProcMgrHndl,
        (ProcMgr_MapType_MasterKnlVirt | ProcMgr_MapType_MasterUsrVirt),
        &addrInfo, ProcMgr_AddrType_MasterPhys);

    if (status < 0) {
        System_printf("\nProcMgr_unmap failed\n");
    }
    /* close the ProcMgr object */
    System_printf("\nProcMgr_close()\n");
    status = ProcMgr_close(&(gProcMgrHndl));

    if (status < 0) {
        System_printf("\nProcMgr_close failed\n");
    }

    System_printf("========  Stop Tiler daemon ========");

    getchar();
    MemMgr_deleteDaemon(&gDaemonHndl);

    System_printf("======== finalize SysLink ========");
    SysLink_destroy();
    System_printf ("\nLeaving TilerDaemonApp_startup\n");

    return(status);

}

Int main()
{
    Int32  status = 0;

    Error_Block eb;

    /* must initialize the error block before using it */
    Error_init(&eb);

    System_printf("\nThread_start()\n");
    Thread_start(&eb);

    TilerDaemonApp_startup();

    System_printf("\nNotifying M3Video about Tiler daemon creation...\n");
    /* Notify M3Video about creation of Tiler daemon*/
    getchar();
    status = Notify_sendEvent (MultiProc_getId("VIDEO-M3"),
                               0,
                               TILERDAEMONAPP_EVENT_NO,
                               0,
                               TRUE);
    if(status < 0) {
        System_printf ("Notify_sendEvent to M3Video Failed status: [0x%x]\n", status);
    }

    System_printf("\nPress any key to shut down\n");
    getchar();
    TilerDaemonApp_shutdown();
    return(0);
}

