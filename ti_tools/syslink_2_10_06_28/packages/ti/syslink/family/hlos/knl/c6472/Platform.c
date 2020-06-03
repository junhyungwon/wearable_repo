/*
 *  @file   Platform.c
 *
 *  @brief      Implementation of C6472 Platform initialization logic.
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



/* Standard header files */
#include <ti/syslink/Std.h>

/* Utilities & Osal headers */
#include <ti/syslink/utils/Gate.h>
#include <ti/syslink/utils/GateMutex.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/syslink/utils/Trace.h>
#include <ti/ipc/MultiProc.h>
#include <_MultiProc.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/inc/knl/OsalThread.h>
#include <ti/syslink/utils/String.h>
#include <asm/cache.h>

/* SysLink device specific headers */
#include <C6472IpcInt.h>
#include <C6472Core0Proc.h>
#include <C6472Core1Proc.h>
#include <C6472Core2Proc.h>
#include <C6472Core3Proc.h>
#include <C6472Core4Proc.h>

/* Module level headers */
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/ListMP.h>
#include <ti/ipc/MessageQ.h>
#include <ti/ipc/Notify.h>
#include <ti/ipc/HeapBufMP.h>
#include <ti/ipc/NameServer.h>
#include <ti/ipc/SharedRegion.h>

#include <ClientNotifyMgr.h>
#include <ti/syslink/ProcMgr.h>
#include <_ProcMgr.h>
#include <ti/syslink/inc/knl/Platform.h>

#include <ti/syslink/inc/_GateMP.h>
#include <GatePeterson.h>
#include <GateAAMonitor.h>
#include <TransportShm.h>
#include <_Notify.h>
#include <NotifyDriverShm.h>
#include <_NameServer.h>
#include <_SharedRegion.h>
#include <_MessageQ.h>
#include <_HeapBufMP.h>
#include <_HeapMemMP.h>
#include <_ListMP.h>
#include <NameServerRemote.h>
#include <NameServerRemoteNotify.h>
#include <ClientNotifyMgr.h>
#include <ti/syslink/utils/IHeap.h>
#if 0 /* TBD:Temporarily comment. */
#include <HeapMultiBuf.h>
#endif

#include <ti/ipc/Ipc.h>
#include <_Ipc.h>
#include <IpcKnl.h>


#if defined (__cplusplus)
extern "C" {
#endif


/** ============================================================================
 *  Macros.
 *  ============================================================================
 */
#define RESETVECTOR_SYMBOL          "_Ipc_ResetVector"


/* This tag is used as an identifier by Ipc_readConfig
 * to get different modules' configuration on slave
 */
#define SLAVE_CONFIG_TAG            0xDADA0000

/* Defines used for timeout value for Ipc_detach/stop calls */
#define TIMEOUT_LOOPCNT             1000
#define TIMEOUT_SLEEPTIME           10

/** ============================================================================
 *  Application specific configuration, please change these value according to
 *  your application's need.
 *  ============================================================================
 */

 /* This structure is used to get different modules' configuration on host
 * to match it with that of Slaves'
 */
typedef struct Platform_ModuleConfig {
    UInt16 sharedRegionNumEntries;
}Platform_ModuleConfig;

/*!
 *  @brief  Structure defining config parameters for overall System.
 */
typedef struct Platform_Config {
    MultiProc_Config                multiProcConfig;
    /*!< Multiproc config parameter */

    GateMP_Config                   gateMPConfig;
    /*!< GateMP config parameter */

    GateAAMonitor_Config            gateAAMonitorConfig;
    /*!< Gatepeterson config parameter */

    GatePeterson_Config             gatePetersonConfig;
    /*!< Gatepeterson config parameter */

    SharedRegion_Config             sharedRegionConfig;
    /*!< SharedRegion config parameter */

    MessageQ_Config                 messageQConfig;
    /*!< MessageQ config parameter */

    Notify_Config                   notifyConfig;
    /*!< Notify config parameter */

    ProcMgr_Config                  procMgrConfig;
    /*!< Processor manager config parameter */

    HeapBufMP_Config                heapBufConfig;
    /*!< Heap Buf config parameter */

    HeapMemMP_Config                heapMemMPConfig;
    /*!< Heap MemMP config parameter */

#if 0 /* TBD:Temporarily comment. */
    HeapMultiBuf_Config             heapMultiBufConfig;
    /*!< HeapMultiBuf config parameter */
#endif

    ListMP_Config                    listMPConfig;
    /*!<ListMP config parameter */

    TransportShm_Config     messageQTransportShmConfig;
    /*!< TransportShm config parameter */

    NotifyDriverShm_Config          notifyDriverShmConfig;
    /*!< NotifyDriverShm config parameter */

    NameServerRemoteNotify_Config   nameServerRemoteNotifyConfig;
    /*!< NameServerRemoteNotify config parameter */
    ClientNotifyMgr_Config          cliNotifyMgrCfgParams;
    /*!< ClientNotifyMgr config parameter */

} Platform_Config;


/* Struct embedded into slave binary */
typedef struct Platform_SlaveConfig {
    UInt32  cacheLineSize;
    UInt32  brOffset;
    UInt32  sr0MemorySetup;
    UInt32  setupMessageQ;
    UInt32  setupNotify;
    UInt32  procSync;
    UInt32  numSRs;
} Platform_SlaveConfig;

/* Shared region configuration */
typedef struct Platform_SlaveSRConfig {
    UInt32 entryBase;
    UInt32 entryLen;
    UInt32 ownerProcId;
    UInt32 id;
    UInt32 createHeap;
    UInt32 cacheLineSize;
} Platform_SlaveSRConfig;

/*! @brief structure for platform instance */
typedef struct Platform_Object {
    /*!< Flag to indicate platform initialization status */
    ProcMgr_Handle                pmHandle;
    /*!< Handle to the ProcMgr instance used */
    union {
        struct {
            C6472CORE0PROC_Handle pHandle;
        } core0;
        struct {
            C6472CORE1PROC_Handle pHandle;
        } core1;
        struct {
            C6472CORE2PROC_Handle pHandle;
        } core2;
        struct {
            C6472CORE3PROC_Handle pHandle;
        } core3;
        struct {
            C6472CORE4PROC_Handle pHandle;
        } core4;
    } sHandles;
    /*!< Slave specific handles */
    Platform_SlaveConfig          slaveCfg;
    /*!< Slave embedded config */
    Platform_SlaveSRConfig *      slaveSRCfg;
    /*!< Shared region details from slave */

    UInt8               *bslaveAdditionalReg;
    /*!< To keep track of  additional shared regions configured in the slave */
} Platform_Object, *Platform_Handle;


/*! @brief structure for platform instance */
typedef struct Platform_Module_State {
    Bool              multiProcInitFlag;
    /*!< MultiProc Initialize flag */
    Bool              gateMPInitFlag;
    /*!< GateMP Initialize flag */
    Bool              gateAAMonitorInitFlag;
    /*!< Gatepeterson Initialize flag */
    Bool              gatePetersonInitFlag;
    /*!< Gatepeterson Initialize flag */
    Bool              sharedRegionInitFlag;
    /*!< SHAREDREGION Initialize flag */
    Bool              listMpInitFlag;
    /*!< ListMP Initialize flag */
    Bool              messageQInitFlag;
    /*!< MessageQ Initialize flag */
    Bool              ringIOInitFlag;
    /*!< ringIO Initialize flag */
    Bool              notifyInitFlag;
    /*!< Notify Initialize flag */
    Bool              procMgrInitFlag;
    /*!< Processor manager Initialize flag */
    Bool              heapBufInitFlag;
    /*!< HeapBufMP Initialize flag */
    Bool              heapMemMPInitFlag;
    /*!< HeapMemMP Initialize flag */
#if 0 /* TBD:Temporarily comment. */
    Bool              heapMultiBufInitFlag;
    /*!< HeapBufMP Initialize flag */
#endif
    Bool              nameServerInitFlag;
    /*!< nameServerRemoteNotify Initialize flag */
    Bool              listMPInitFlag;
    /*!< LISTMPSHAREDMEMORY Initialize flag */
    Bool              messageQTransportShmInitFlag;
    /*!< messageQTransportShm Initialize flag */
    Bool              notifyDriverShmInitFlag;
    /*!< notifyDriverShm Initialize flag */
    Bool              nameServerRemoteNotifyInitFlag;
    /*!< nameServerRemoteNotify Initialize flag */
    Bool              clientNotifyMgrInitFlag;
    /*!< clientNotifierMgr Initialize flag */
    Bool              platformInitFlag;
    /*!< Flag to indicate platform initialization status */
    Platform_ModuleConfig hostModuleConfig;
    /*!< Configuration of various Modules' parameters on host */

} Platform_Module_State;


/* =============================================================================
 * GLOBALS
 * =============================================================================
 */
static Platform_Object Platform_objects [MultiProc_MAXPROCESSORS];
static Platform_Module_State Platform_Module_state;
static Platform_Module_State * Platform_module = &Platform_Module_state;
static UInt16 Platform_refCount = 0;


Int32 _Platform_setup  (void);
Int32 _Platform_destroy (void);

/** ============================================================================
 *  APIs.
 *  ============================================================================
 */
/* Function to read slave memory */
Int32
_Platform_readSlaveMemory (UInt16   procId,
                           UInt32   addr,
                           Ptr      value,
                           UInt32 * numBytes);

/* Function to write slave memory */
Int32
_Platform_writeSlaveMemory (UInt16   procId,
                            UInt32   addr,
                            Ptr      value,
                            UInt32 * numBytes);
/*!
 *  @brief      Function to get tyhe default values for confiurations.
 *
 *  @param      config   Configuration values.
 */
Void
Platform_getConfig (Platform_Config * config)
{
    GT_1trace (curTrace, GT_ENTER, "Platform_getConfig", config);

    GT_assert (curTrace, (config != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (config == NULL) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "Platform_getConfig",
                             Platform_E_INVALIDARG,
                             "Argument of type (Platform_getConfig *) passed "
                             "is null!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

        /* Get the gatepeterson default config */
        MultiProc_getConfig (&config->multiProcConfig);

        /* Get the gateMP default config */
        GateMP_getConfig (&config->gateMPConfig);

        /* Get the gatepeterson default config */
        GatePeterson_getConfig (&config->gatePetersonConfig);

        /* Get the sharedregion default config */
        SharedRegion_getConfig (&config->sharedRegionConfig);

        /* Get the MESSAGEQ default config */
        MessageQ_getConfig (&config->messageQConfig);

        /* Get the NOTIFY default config */
        Notify_getConfig (&config->notifyConfig);

        /* Get the PROCMGR default config */
        ProcMgr_getConfig (&config->procMgrConfig);

        /* Get the HeapBufMP default config */
        HeapBufMP_getConfig (&config->heapBufConfig);

        /* Get the HeapMemMP default config */
        HeapMemMP_getConfig (&config->heapMemMPConfig);

#if 0 /* TBD:Temporarily comment. */
        /* Get the HeapMultiBuf default config */
        HeapMultiBuf_getConfig (&config->heapMultiBufConfig);
#endif

        /* Get the LISTMPSHAREDMEMORY default config */
        ListMP_getConfig (&config->listMPConfig);

        /* Get the MESSAGEQTRANSPORTSHM default config */
        TransportShm_getConfig (&config->messageQTransportShmConfig);

        /* Get the NOTIFYSHMDRIVER default config */
        NotifyDriverShm_getConfig (&config->notifyDriverShmConfig);

        /* Get the NAMESERVERREMOTENOTIFY default config */
        NameServerRemoteNotify_getConfig(&config->nameServerRemoteNotifyConfig);

        /* Get the ClientNotifyMgr default config */
        ClientNotifyMgr_getConfig (&config->cliNotifyMgrCfgParams) ;


#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_0trace (curTrace, GT_LEAVE, "Platform_getConfig");
}

/*!
 *  @brief      Function to override the default confiuration values.
 *
 *  @param      config   Configuration values.
 */
Int32
Platform_overrideConfig (Platform_Config * config)
{
    Int32  status = Platform_S_SUCCESS;

    GT_1trace (curTrace, GT_ENTER, "Platform_overrideConfig", config);

    GT_assert (curTrace, (config != NULL));

#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (config == NULL) {
        /*! @retval Platform_E_INVALIDARG Argument of type
         *  (Platform_Config *) passed is null*/
        status = Platform_E_INVALIDARG;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "Platform_overrideConfig",
                             status,
                             "Argument of type (Platform_getConfig *) passed "
                             "is null!");
    }
    else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

        config->multiProcConfig.numProcessors = 6;
        config->multiProcConfig.id            = 5;
        String_cpy (config->multiProcConfig.nameList [0],
                    "CORE0");
        String_cpy (config->multiProcConfig.nameList [1],
                    "CORE1");
        String_cpy (config->multiProcConfig.nameList [2],
                    "CORE2");
        String_cpy (config->multiProcConfig.nameList [3],
                    "CORE3");
        String_cpy (config->multiProcConfig.nameList [4],
                    "CORE4");
        String_cpy (config->multiProcConfig.nameList [5],
                    "CORE5");
        /* Override the gatepeterson default config */

        /* Override the Sharedregion default config */
        config->sharedRegionConfig.cacheLineSize = 128;

        /* Override the LISTMP default config */

        /* Override the MESSAGEQ default config */

        /* Override the NOTIFY default config */

        /* Override the PROCMGR default config */

        /* Override the HeapBuf default config */

        /* Override the LISTMPSHAREDMEMORY default config */

        /* Override the MESSAGEQTRANSPORTSHM default config */

        /* Override the NOTIFYSHMDRIVER default config */

        /* Override the NAMESERVERREMOTENOTIFY default config */

        /* Override the  ClientNotifyMgr default config */


#if !defined(SYSLINK_BUILD_OPTIMIZE)
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    GT_1trace (curTrace, GT_ENTER, "Platform_overrideConfig", status);

    /*! @retval Platform_S_SUCCESS operation was successful */
    return status;
}

/*!
 *  @brief      Function to setup platform.
 *              TBD: logic would change completely in the final system.
 */
Int32
Platform_setup (void)
{
    Int32             status = Platform_S_SUCCESS;
    Platform_Config   _config;
    Platform_Config * config;
    C6472IpcInt_Config c6472cfg;
    UInt32            i;
    Ptr     sharedAddr;

    Platform_getConfig (&_config);
    config = &_config;

    Platform_overrideConfig (config);

    status = MultiProc_setup (&(config->multiProcConfig));
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (status < 0) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "Platform_setup",
                             status,
                             "MultiProc_setup failed!");
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

/* Initialize PROCMGR */
    if (status >= 0) {
        status = ProcMgr_setup (&(config->procMgrConfig));
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "ProcMgr_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->procMgrInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }

/* Initialize SharedRegion */
    if (status >= 0) {
        status = SharedRegion_setup (&config->sharedRegionConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "SharedRegion_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->sharedRegionInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

#if defined (USE_SYSLINK_NOTIFY)
    /* Initialize IpcInt required for Notify. */
    if (status >= 0) {
        /* Do the IPC interrupts setup for the full platform (cfg is not used) */
        C6472IpcInt_setup (&c6472cfg);
    }
#endif

/* Get the NOTIFYSHMDRIVER default config */
    if (status >= 0) {
        status = NotifyDriverShm_setup (&config->notifyDriverShmConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "NotifyDriverShm_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->notifyDriverShmInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

/* Initialize NOTIFY */
    if (status >= 0) {
        status = Notify_setup (&config->notifyConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "Notify_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->notifyInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

/* Initialize NAMESERVER */
    if (status >= 0) {
        status = NameServer_setup ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "NameServer_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->nameServerInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

   if (status >= 0) {
        status = GateMP_setup (&config->gateMPConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "GateMP_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->gateMPInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

/* Initialize Platform */
    if (status >= 0) {
        status = GatePeterson_setup (&config->gatePetersonConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "GatePeterson_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->gatePetersonInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    if (status >= 0) {
            status = GateAAMonitor_setup (&config->gateAAMonitorConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            if (status < 0) {
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "Platform_setup",
                                     status,
                                     "GateAAMonitor_setup failed!");
            }
            else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
                Platform_module->gateAAMonitorInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }

/* Intialize MESSAGEQ */
    if (status >= 0) {
        status = MessageQ_setup (&config->messageQConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "MessageQ_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->messageQInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

/* Intialize heap buf */
    if (status >= 0) {
        status = HeapBufMP_setup (&config->heapBufConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "HeapBufMP_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->heapBufInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

/* Intialize heap MemMP */
    if (status >= 0) {
        status = HeapMemMP_setup (&config->heapMemMPConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "HeapMemMP_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->heapMemMPInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

#if 0 /* TBD:Temporarily comment. */
/* Intialize HeapMultiBuf */
    if (status >= 0) {
        status = HeapMultiBuf_setup (&config->heapMultiBufConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "HeapMultiBuf_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->heapMultiBufInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }
#endif

/* Get the LISTMPSHAREDMEMORY default config */
    if (status >= 0) {
        status = ListMP_setup (&config->listMPConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "ListMP_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->listMPInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }


/* Get the MESSAGEQTRANSPORTSHM default config */
    if (status >= 0) {
        status = TransportShm_setup (&config->messageQTransportShmConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "TransportShm_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->messageQTransportShmInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }


/* Get the NAMESERVERREMOTENOTIFY default config */
    if (status >= 0) {
        status = NameServerRemoteNotify_setup (
                                     &config->nameServerRemoteNotifyConfig);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "NameServerRemoteNotify_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->nameServerRemoteNotifyInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Get the ClientNotifyMgr default config */
    if (status >= 0) {
        status = ClientNotifyMgr_setup (&config->cliNotifyMgrCfgParams);
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "ClientNotifyMgr_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->clientNotifyMgrInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    if (status >= 0) {
        Memory_set (Platform_objects,
                    0,
                    (sizeof (Platform_Object) * MultiProc_getNumProcessors()));
    }

    if (status >= 0) {
        status = _Platform_setup ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_setup",
                                 status,
                                 "_Platform_setup failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->platformInitFlag = TRUE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }
    if (status >= 0) {
        /* Store SharedRegion numEntries configuration on Host to compare with
        *  the value configured on of salves
        */
        Platform_module->hostModuleConfig.sharedRegionNumEntries =
                                          config->sharedRegionConfig.numEntries;

    }
    return status;
}


/*!
 *  @brief      Function to destroy the System.
 *
 *  @sa         Platform_setup
 */
Int32
Platform_destroy (void)
{
    Int32  status = Platform_S_SUCCESS;
    UInt32 i;

    GT_0trace (curTrace, GT_ENTER, "Platform_destroy");

    /* Finalize Platform-specific destroy */
    if (Platform_module->platformInitFlag == TRUE) {
        status = _Platform_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "Platform_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->platformInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize ClientNotifyMgr module */
    if (Platform_module->clientNotifyMgrInitFlag == TRUE) {
        status = ClientNotifyMgr_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "ClientNotifyMgr_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->clientNotifyMgrInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize NAMESERVERREMOTENOTIFY */
    if (Platform_module->nameServerRemoteNotifyInitFlag == TRUE) {
        status = NameServerRemoteNotify_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "NameServerRemoteNotify_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->nameServerRemoteNotifyInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize MESSAGEQTRANSPORTSHM */
    if (Platform_module->messageQTransportShmInitFlag == TRUE) {
        status = TransportShm_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "TransportShm_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->messageQTransportShmInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize LISTMPSHAREDMEMORY */
    if (Platform_module->listMPInitFlag == TRUE) {
        status = ListMP_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "ListMP_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->listMPInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

#if 0 /* TBD:Temporarily comment. */
    /* Finalize HeapMultiBuf */
    if (Platform_module->heapMultiBufInitFlag == TRUE) {
        status = HeapMultiBuf_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "HeapMultiBuf_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->heapMultiBufInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }
#endif

    /* Finalize heap buf */
    if (Platform_module->heapBufInitFlag == TRUE) {
        status = HeapBufMP_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "HeapBufMP_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->heapBufInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize heap MemMP */
    if (Platform_module->heapMemMPInitFlag == TRUE) {
        status = HeapMemMP_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "HeapMemMP_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->heapMemMPInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize MESSAGEQ */
    if (Platform_module->messageQInitFlag == TRUE) {
        status = MessageQ_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "MessageQ_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->messageQInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize GatePeterson */
    if (Platform_module->gatePetersonInitFlag == TRUE) {
        status = GatePeterson_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "GatePeterson_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->gatePetersonInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize GateAAMonitor */
    if (Platform_module->gateAAMonitorInitFlag == TRUE) {
        status = GateAAMonitor_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "GateAAMonitor_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->gateAAMonitorInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    if (Platform_module->gateMPInitFlag == TRUE) {
        status = GateMP_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "GateMP_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->gateMPInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize NAMESERVER */
    if (Platform_module->nameServerInitFlag == TRUE) {
        status = NameServer_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "NameServer_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->nameServerInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize NOTIFY */
    if (Platform_module->notifyInitFlag == TRUE) {
        status = Notify_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "Notify_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->notifyInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize NOTIFYSHMDRIVER */
    if (Platform_module->notifyDriverShmInitFlag == TRUE) {
            status = NotifyDriverShm_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "NotifyDriverShm_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->notifyDriverShmInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

#if defined (USE_SYSLINK_NOTIFY)
    C6472IpcInt_destroy ();
#endif

    /* Finalize SharedRegion */
    if (Platform_module->sharedRegionInitFlag == TRUE) {
        status = SharedRegion_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "SharedRegion_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->sharedRegionInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize PROCMGR */
    if (Platform_module->procMgrInitFlag == TRUE) {
        status = ProcMgr_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_destroy",
                                 status,
                                 "ProcMgr_destroy failed!");
        }
        else {
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
            Platform_module->procMgrInitFlag = FALSE;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* Finalize MultiProc */
    status = MultiProc_destroy ();
#if !defined(SYSLINK_BUILD_OPTIMIZE)
    if (status < 0) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "Platform_destroy",
                             status,
                             "MultiProc_destroy failed!");
    }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */

    if (status >= 0) {
        Memory_set (Platform_objects,
                    0,
                    (sizeof (Platform_Object) * MultiProc_getNumProcessors()));
    }

    GT_1trace (curTrace, GT_LEAVE, "Platform_destroy", status);

    /*! @retval Platform_S_SUCCESS Operation successful */
    return status;
}

/*
 * union _Platform_setup_Local exists so that we don't waste stack or
 * alloc'ed memory on storage for things that exist for just a few
 * statements of the function _Platform_setup().  The *PROC_Params
 * elements are large and variably sized, depending on the macro
 * ProcMgr_MAX_MEMORY_REGIONS.
 */
typedef union _Platform_setup_Local {
    ProcMgr_Params          params;
    C6472CORE0PROC_Config   core0ProcConfig;
    C6472CORE1PROC_Config   core1ProcConfig;
    C6472CORE2PROC_Config   core2ProcConfig;
    C6472CORE3PROC_Config   core3ProcConfig;
    C6472CORE4PROC_Config   core4ProcConfig;
    C6472CORE0PROC_Params   core0ProcParams;
    C6472CORE1PROC_Params   core1ProcParams;
    C6472CORE2PROC_Params   core2ProcParams;
    C6472CORE3PROC_Params   core3ProcParams;
    C6472CORE4PROC_Params   core4ProcParams;
} _Platform_setup_Local;

/*!
 *  @brief      Function to setup platform.
 *              TBD: logic would change completely in the final system.
 */
Int32
_Platform_setup (void)
{
    Int32                   status = Platform_S_SUCCESS;
    _Platform_setup_Local   *lv;
    UInt16                  procId;
    Platform_Handle         handle;

    GT_0trace (curTrace, GT_ENTER, "_Platform_setup");

    lv = Memory_alloc(NULL, sizeof(_Platform_setup_Local), 0, NULL);
    if (lv == NULL) {
        status = Platform_E_FAIL;
        GT_setFailureReason (curTrace,
                                GT_4CLASS,
                                "_Platform_setup",
                                status,
                                "Memory_alloc failed");
        goto ret;
    }

    /* Get MultiProc ID by name. */
    procId = MultiProc_getId ("CORE0");

    handle = &Platform_objects [procId];
    C6472CORE0PROC_getConfig (&lv->core0ProcConfig);
    status = C6472CORE0PROC_setup (&lv->core0ProcConfig);
    if (status < 0) {
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "_Platform_setup",
                             status,
                             "C6472CORE0PROC_setup failed!");
    }

    if (status >= 0) {
        /* Create an instance of the Processor object for
         * C6472 Core 0 */
        C6472CORE0PROC_Params_init (NULL, &lv->core0ProcParams);
        handle->sHandles.core0.pHandle = C6472CORE0PROC_create (procId,
                                                        &lv->core0ProcParams);
        if (handle->sHandles.core0.pHandle == NULL) {
            status = Platform_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_setup",
                                 status,
                                 "C6472CORE0PROC_create failed!");
        } else {

            /* Initialize parameters */
            ProcMgr_Params_init (NULL, &lv->params);
            String_cpy (lv->params.rstVectorSectionName, RESETVECTOR_SYMBOL);
            lv->params.procHandle = handle->sHandles.core0.pHandle;
            handle->pmHandle = ProcMgr_create (procId, &lv->params);
            if (handle->pmHandle == NULL) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "_Platform_setup",
                                     status,
                                     "ProcMgr_create failed!");
            }
        }
    }

    if (status >= 0) {
        /* Get MultiProc ID by name. */
        procId = MultiProc_getId ("CORE1");

        handle = &Platform_objects [procId];
        C6472CORE1PROC_getConfig (&lv->core1ProcConfig);
        status = C6472CORE1PROC_setup (&lv->core1ProcConfig);
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_setup",
                                 status,
                                 "C6472CORE1PROC_setup failed!");
        }

        if (status > 0) {
            /* Create an instance of the Processor object for
            * C6472 Core 0 */
            C6472CORE1PROC_Params_init (NULL, &lv->core1ProcParams);
            handle->sHandles.core1.pHandle = C6472CORE1PROC_create (procId,
                                                        &lv->core1ProcParams);
            if (handle->sHandles.core1.pHandle == NULL) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "_Platform_setup",
                                     status,
                                     "C6472CORE0PROC_create failed!");
            } else {

                /* Initialize parameters */
                ProcMgr_Params_init (NULL, &lv->params);
                String_cpy (lv->params.rstVectorSectionName,
                            RESETVECTOR_SYMBOL);
                lv->params.procHandle = handle->sHandles.core1.pHandle;
                handle->pmHandle = ProcMgr_create (procId, &lv->params);
                if (handle->pmHandle == NULL) {
                    status = Platform_E_FAIL;
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "_Platform_setup",
                                         status,
                                         "ProcMgr_create failed!");
                }
            }
        }
    }

    if (status >= 0) {
        /* Get MultiProc ID by name. */
        procId = MultiProc_getId ("CORE2");

        handle = &Platform_objects [procId];
        C6472CORE2PROC_getConfig (&lv->core2ProcConfig);
        status = C6472CORE2PROC_setup (&lv->core2ProcConfig);
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_setup",
                                 status,
                                 "C6472CORE2PROC_setup failed!");
        }

        if (status > 0) {
            /* Create an instance of the Processor object for
            * C6472 Core 2 */
            C6472CORE2PROC_Params_init (NULL, &lv->core2ProcParams);
            handle->sHandles.core2.pHandle = C6472CORE2PROC_create (procId,
                                                        &lv->core2ProcParams);
            if (handle->sHandles.core2.pHandle == NULL) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "_Platform_setup",
                                     status,
                                     "C6472CORE2PROC_create failed!");
            } else {

                /* Initialize parameters */
                ProcMgr_Params_init (NULL, &lv->params);
                String_cpy (lv->params.rstVectorSectionName,
                            RESETVECTOR_SYMBOL);
                lv->params.procHandle = handle->sHandles.core2.pHandle;
                handle->pmHandle = ProcMgr_create (procId, &lv->params);
                if (handle->pmHandle == NULL) {
                    status = Platform_E_FAIL;
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "_Platform_setup",
                                         status,
                                         "ProcMgr_create failed!");
                }
            }
        }
    }

    if (status >= 0) {
        /* Get MultiProc ID by name. */
        procId = MultiProc_getId ("CORE3");

        handle = &Platform_objects [procId];
        C6472CORE3PROC_getConfig (&lv->core3ProcConfig);
        status = C6472CORE3PROC_setup (&lv->core3ProcConfig);
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_setup",
                                 status,
                                 "C6472CORE3PROC_setup failed!");
        }

        if (status > 0) {
            /* Create an instance of the Processor object for
            * C6472 Core 3 */
            C6472CORE3PROC_Params_init (NULL, &lv->core3ProcParams);
            handle->sHandles.core3.pHandle = C6472CORE3PROC_create (procId,
                                                        &lv->core3ProcParams);
            if (handle->sHandles.core3.pHandle == NULL) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "_Platform_setup",
                                     status,
                                     "C6472CORE3PROC_create failed!");
            } else {

                /* Initialize parameters */
                ProcMgr_Params_init (NULL, &lv->params);
                lv->params.procHandle = handle->sHandles.core3.pHandle;
                String_cpy (lv->params.rstVectorSectionName,
                            RESETVECTOR_SYMBOL);
                handle->pmHandle = ProcMgr_create (procId, &lv->params);
                if (handle->pmHandle == NULL) {
                    status = Platform_E_FAIL;
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "_Platform_setup",
                                         status,
                                         "ProcMgr_create failed!");
                }
            }
        }
    }

    if (status >= 0) {
        /* Get MultiProc ID by name. */
        procId = MultiProc_getId ("CORE4");

        handle = &Platform_objects [procId];
        C6472CORE4PROC_getConfig (&lv->core4ProcConfig);
        status = C6472CORE4PROC_setup (&lv->core4ProcConfig);
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_setup",
                                 status,
                                 "C6472CORE4PROC_setup failed!");
        }

        if (status >= 0) {
            /* Create an instance of the Processor object for
             * C6472 Core 4 */
            C6472CORE4PROC_Params_init (NULL, &lv->core4ProcParams);
            handle->sHandles.core4.pHandle = C6472CORE4PROC_create (procId,
                                                        &lv->core4ProcParams);
            if (handle->sHandles.core4.pHandle == NULL) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "_Platform_setup",
                                     status,
                                     "C6472CORE4PROC_create failed!");
            } else {

                /* Initialize parameters */
                ProcMgr_Params_init (NULL, &lv->params);
                lv->params.procHandle = handle->sHandles.core4.pHandle;
                String_cpy (lv->params.rstVectorSectionName,
                            RESETVECTOR_SYMBOL);
                handle->pmHandle = ProcMgr_create (procId, &lv->params);
                if (handle->pmHandle == NULL) {
                    status = Platform_E_FAIL;
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "_Platform_setup",
                                         status,
                                         "ProcMgr_create failed!");
                }
            }
        }
    }

    Memory_free(NULL, lv, sizeof(_Platform_setup_Local));

ret:
    GT_1trace (curTrace, GT_LEAVE, "_Platform_setup", status);

    /*! @retval Platform_S_SUCCESS operation was successful */
    return status;
}


/*!
 *  @brief      Function to setup platform.
 *              TBD: logic would change completely in the final system.
 */
Int32
_Platform_destroy (void)
{
    Int32           status    = Platform_S_SUCCESS;
    Int32           tmpStatus = Platform_S_SUCCESS;
    Platform_Handle handle;

    GT_0trace (curTrace, GT_ENTER, "_Platform_destroy");

    /* ------------------------- CORE0 cleanup -------------------------------- */
    handle = &Platform_objects [MultiProc_getId ("CORE0")];
    if (handle->pmHandle != NULL) {
        status = ProcMgr_delete (&handle->pmHandle);
        GT_assert (curTrace, (status >= 0));
#if !defined(SYSLINK_BUILD_OPTIMIZE)
        if (status < 0) {
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_destroy",
                                 status,
                                 "ProcMgr_delete failed!");
        }
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
    }

    /* ------------------------- CORE1 cleanup ------------------------------ */
    handle = &Platform_objects [MultiProc_getId ("CORE1")];
    if (handle->pmHandle != NULL) {
        tmpStatus = ProcMgr_delete (&handle->pmHandle);
        GT_assert (curTrace, (tmpStatus >= 0));
        if ((status >= 0) && (tmpStatus < 0)) {
            status = tmpStatus;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_destroy",
                                 status,
                                 "ProcMgr_delete failed!");
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }
    }

/* ------------------------- CORE2 cleanup ------------------------------ */
    handle = &Platform_objects [MultiProc_getId ("CORE2")];
    if (handle->pmHandle != NULL) {
        tmpStatus = ProcMgr_delete (&handle->pmHandle);
        GT_assert (curTrace, (tmpStatus >= 0));
        if ((status >= 0) && (tmpStatus < 0)) {
            status = tmpStatus;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_destroy",
                                 status,
                                 "ProcMgr_delete failed!");
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }
    }

/* ------------------------- CORE3 cleanup ------------------------------ */
    handle = &Platform_objects [MultiProc_getId ("CORE3")];
    if (handle->pmHandle != NULL) {
        tmpStatus = ProcMgr_delete (&handle->pmHandle);
        GT_assert (curTrace, (tmpStatus >= 0));
        if ((status >= 0) && (tmpStatus < 0)) {
            status = tmpStatus;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_destroy",
                                 status,
                                 "ProcMgr_delete failed!");
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }
    }

/* ------------------------- CORE4 cleanup ------------------------------ */
    handle = &Platform_objects [MultiProc_getId ("CORE4")];
    if (handle->pmHandle != NULL) {
        tmpStatus = ProcMgr_delete (&handle->pmHandle);
        GT_assert (curTrace, (tmpStatus >= 0));
        if ((status >= 0) && (tmpStatus < 0)) {
            status = tmpStatus;
#if !defined(SYSLINK_BUILD_OPTIMIZE)
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_destroy",
                                 status,
                                 "ProcMgr_delete failed!");
#endif /* if !defined(SYSLINK_BUILD_OPTIMIZE) */
        }
    }

    GT_1trace (curTrace, GT_LEAVE, "_Platform_destroy", status);

    /*! @retval Platform_S_SUCCESS operation was successful */
    return status;
}


/*!
 *  @brief      Function called by ProcMgr when slave is in loaded state.
 *
 *  @param      arg   Registered argument.
 */
Int32
Platform_loadCallback (UInt16 procId, Ptr arg)
{
    Int32                    status = Platform_S_SUCCESS;
    Platform_Handle          handle;
    UInt32                   start;
    UInt32                   numBytes;
    SharedRegion_Entry       entry;
    UInt32                   mAddr;
    ProcMgr_AddrInfo         ai;
    Ipc_Params               iParams;
    Int                      i;
    UInt32                   fileId;

    GT_2trace (curTrace, GT_ENTER, "Platform_loadCallback", procId, arg);

    handle = (Platform_Handle) &Platform_objects [procId];

    if (arg != NULL) {
        /* If arg is provided by user, then it is the slave virtual address of
         * the RESETVECTOR_SYMBOL.
         */
        start = (*(UInt32 *) arg);
    }
    else {
        fileId = ProcMgr_getLoadedFileId (handle->pmHandle);

        /* read the symbol from slave binary */
        status = ProcMgr_getSymbolAddress (handle->pmHandle,
                                           fileId,
                                           RESETVECTOR_SYMBOL,
                                           &start);
        if (status < 0) {
            status = Platform_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_loadCallback",
                                 status,
                                 "ProcMgr_getSymbolAddress failed");
        }
    }

    if (status >= 0) {
        /* Read the slave config */
        numBytes = sizeof (Platform_SlaveConfig);
        status = _Platform_readSlaveMemory (procId,
                                            start,
                                            &handle->slaveCfg,
                                            &numBytes);
        if (status < 0) {
            status = Platform_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_loadCallback",
                                 status,
                                 "_Platform_readSlaveMemory failed");
        }
        else if (handle->slaveCfg.numSRs > 0) {
            numBytes = handle->slaveCfg.numSRs * sizeof (Platform_SlaveSRConfig);
            handle->slaveSRCfg = Memory_alloc (NULL, numBytes, 0, NULL);

            if (handle->slaveSRCfg == NULL) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                        GT_4CLASS,
                                        "Platform_loadCallback",
                                        status,
                                        "Memory_alloc failed");
            }
            else {
                handle->bslaveAdditionalReg = Memory_calloc (NULL,
                                      (handle->slaveCfg.numSRs * sizeof (UInt8)),
                                       0,
                                       NULL);

                if (handle->bslaveAdditionalReg == NULL) {
                    status = Platform_E_FAIL;
                    GT_setFailureReason (curTrace,
                                            GT_4CLASS,
                                            "Platform_loadCallback",
                                            status,
                                            "Memory_alloc failed");
                }
                else {
                    status = _Platform_readSlaveMemory (
                                            procId,
                                            start + sizeof (Platform_SlaveConfig),
                                            handle->slaveSRCfg,
                                            &numBytes);
                    if (status < 0) {
                        status = Platform_E_FAIL;
                        GT_setFailureReason (curTrace,
                                                GT_4CLASS,
                                                "Platform_loadCallback",
                                                status,
                                            "_Platform_readSlaveMemory failed");
                    }
                }
            }
        }

        if (status >= 0) {
            iParams.setupMessageQ = handle->slaveCfg.setupMessageQ;
            iParams.setupNotify   = handle->slaveCfg.setupNotify;
            iParams.procSync      = handle->slaveCfg.procSync;
            status = Ipc_create (procId, &iParams);
            if (status < 0) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "Platform_loadCallback",
                                     status,
                                     "Ipc_create failed");
            }
        }

        if (status >= 0) {
            /* Setup the shared memory for region with owner == host */
            for (i = 0; i < handle->slaveCfg.numSRs; i++) {
                GT_1trace (curTrace,
                           GT_1CLASS,
                           "handle->slaveSRCfg [i].entryBase %p\n",
                           handle->slaveSRCfg [i].entryBase);
                status = SharedRegion_getEntry (i, &entry);
                if (status < 0) {
                    status = Platform_E_FAIL;
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "Platform_loadCallback",
                                         status,
                                         "SharedRegion_getEntry failed");
                }
                else {
                    GT_assert (curTrace,
                             (    (entry.isValid == FALSE)
                               || (   (entry.isValid == TRUE)
                                   && (   entry.len
                                       == (handle->slaveSRCfg [i].entryLen)))));
                    /* Translate the slave address to master */
                    status = ProcMgr_translateAddr (
                                      handle->pmHandle,
                                      (Ptr *)&mAddr,
                                      ProcMgr_AddrType_MasterPhys,
                                      (Ptr)handle->slaveSRCfg [i].entryBase,
                                      ProcMgr_AddrType_SlaveVirt);
                    if (status < 0) {
                        status = Platform_E_FAIL;
                        GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "Platform_loadCallback",
                                         status,
                                         "ProcMgr_translateAddr failed");
                    }
                    else {
                        ai.addr [ProcMgr_AddrType_MasterPhys] = mAddr;
                        ai.addr [ProcMgr_AddrType_SlaveVirt]  =
                                           handle->slaveSRCfg [i].entryBase;
                        ai.size     = handle->slaveSRCfg [i].entryLen;
                        ai.isCached = TRUE;
                        GT_4trace (curTrace,
                              GT_1CLASS,
                              "Platform_loadCallback:\n"
                              "    Mapping SharedRegion %d\n"
                              "    addr [ProcMgr_AddrType_MasterPhys] [0x%x]\n"
                              "    addr [ProcMgr_AddrType_SlaveVirt]  [0x%x]\n"
                              "    size                               [0x%x]\n",
                              i,
                              ai.addr [ProcMgr_AddrType_MasterPhys],
                              ai.addr [ProcMgr_AddrType_SlaveVirt],
                              ai.size);
                        status = ProcMgr_map (handle->pmHandle,
                                          (  ProcMgr_MapType_SlaveVirt
                                           | ProcMgr_MapType_MasterKnlVirt),
                                          &ai,
                                          ProcMgr_AddrType_MasterPhys);
                        if (status < 0) {
                            GT_setFailureReason (curTrace,
                                                 GT_4CLASS,
                                                 "Platform_loadCallback",
                                                 status,
                                                 "ProcMgr_map failed!");
                        }
                        else {
                            /* Add the entry only if previously not added */
                            if (entry.isValid == FALSE) {
                                Memory_set (
                                 (Ptr) ai.addr [ProcMgr_AddrType_MasterKnlVirt],
                                 0,
                                 handle->slaveSRCfg [i].entryLen);
                                Memory_set (&entry,
                                            0,
                                            sizeof (SharedRegion_Entry));
                                entry.base = (Ptr)
                                       ai.addr [ProcMgr_AddrType_MasterKnlVirt];
                                entry.len = handle->slaveSRCfg [i].entryLen;
                                entry.ownerProcId =
                                            handle->slaveSRCfg [i].ownerProcId;
                                entry.isValid = TRUE;
                                entry.cacheLineSize =
                                           handle->slaveSRCfg [i].cacheLineSize;
                                entry.cacheEnable = TRUE;
                                entry.createHeap =
                                              handle->slaveSRCfg [i].createHeap;
                                /* Check if shared region has been started
                                 * then only create  additional shared regions
                                 */
                                if (_SharedRegion_isStarted()) {
                                    status = SharedRegion_setEntry(handle->slaveSRCfg [i].id,&entry);
                                    if (status < 0) {
                                        status = Platform_E_FAIL;
                                        GT_setFailureReason (curTrace,
                                                             GT_4CLASS,
                                                             "Platform_loadCallback",
                                                             status,
                                                             "SharedRegion_setEntry failed");
                                    }
                                    else {
                                        handle->bslaveAdditionalReg[i] = TRUE;
                                    }
                                }
                                else {
                                    _SharedRegion_setEntry (handle->slaveSRCfg [i].id,
                                                            &entry);
                                }
                            }
                            else { /* cross check the entry attributes  and raise asserts*/
                                GT_assert(curTrace, (entry.base == (Ptr)
                                       ai.addr [ProcMgr_AddrType_MasterKnlVirt]));
                                GT_assert(curTrace,
                                         (entry.len == handle->slaveSRCfg [i].entryLen));
                                GT_assert(curTrace,
                                         (entry.ownerProcId == handle->slaveSRCfg [i].ownerProcId));
                                GT_assert(curTrace,
                                         (entry.createHeap == handle->slaveSRCfg [i].createHeap));

                                /* Add shared region entry for this slave */
                                if (_SharedRegion_isStarted()) {
                                    status = SharedRegion_setEntry(handle->slaveSRCfg [i].id,
                                                                   &entry);
                                    if (status < 0) {
                                        status = Platform_E_FAIL;
                                        GT_setFailureReason (curTrace,
                                                             GT_4CLASS,
                                                             "Platform_loadCallback",
                                                             status,
                                                             "SharedRegion_setEntry failed");
                                    }
                                    else {
                                        handle->bslaveAdditionalReg[i] = TRUE;
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }

        if (status >= 0) {
            /* Read sr0MemorySetup */
            numBytes = sizeof (Platform_SlaveConfig);
            handle->slaveCfg.sr0MemorySetup = 1;
            status = _Platform_writeSlaveMemory (procId,
                                                 start,
                                                 &handle->slaveCfg,
                                                 &numBytes);
            if (status < 0) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "Platform_loadCallback",
                                     status,
                                     "_Platform_writeSlaveMemory failed");
            }
        }

        if (status >= 0) {
            status = Ipc_start ();
            if (status < 0) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "Platform_loadCallback",
                                     status,
                                     "Ipc_start failed");
            }
            else {
                /* Increase refCount for number of successful loadCallbacks. */
                Platform_refCount++;
            }
        }
    }

    GT_1trace (curTrace, GT_LEAVE, "Platform_loadCallback", status);

    return status;
}
EXPORT_SYMBOL(Platform_loadCallback);


/*!
 *  @brief      Function called by ProcMgr when slave is in started state.
 *              In case of self boot mode, slave and host both has to create
 *              the notifyDriver outside of SysMgr context. Since an event is
 *              sent to slave to indicate that HOST has done initialization on
 *              its side and it is safe for slave to do the rest of
 *              initialization.
 *              TBD: logic would change completely in the final system.
 *              TBD: update other slave about this slave's state change.
 *
 *  @param      arg   Registered argument.
 */
Int32
Platform_startCallback (UInt16 procId, Ptr arg)
{
    Int32 status = Platform_S_SUCCESS;
    Platform_ModuleConfig slaveModuleConfig;

    GT_2trace (curTrace, GT_ENTER, "Platform_startCallback", procId, arg);

    do {
        status = Ipc_attach (procId);
    } while (status < 0);

    /* Get shared region max numEntries from Slave */
    do {
        status = Ipc_readConfig (procId,
                                 SLAVE_CONFIG_TAG,
                                 (Ptr) &slaveModuleConfig,
                                 sizeof (Platform_ModuleConfig));
    } while (status == Ipc_E_FAIL);

    GT_assert (curTrace,
              (    Platform_module->hostModuleConfig.sharedRegionNumEntries
               ==  slaveModuleConfig.sharedRegionNumEntries));

    if (    Platform_module->hostModuleConfig.sharedRegionNumEntries
        !=  slaveModuleConfig.sharedRegionNumEntries) {

        status = Platform_E_FAIL;
        GT_setFailureReason (curTrace,
                     GT_4CLASS,
                     "Platform_startCallback",
                     status,
                     "SharedRegion numEntries did not match with slave side");

        GT_4trace (curTrace,
                   GT_4CLASS,
                   "Platform_startCallback:\n"
                   "    Master procId = [%u]\n"
                   "    Slave  procId = [%u]\n"
                   "    SharedRegion numEntries on (Hlos)Master = [%u]\n"
                   "    SharedRegion numEntries on  (Rtos)Slave = [%u]\n",
                   MultiProc_self(),
                   procId,
                   Platform_module->hostModuleConfig.sharedRegionNumEntries,
                   slaveModuleConfig.sharedRegionNumEntries);
    }

    GT_1trace (curTrace, GT_LEAVE, "Platform_startCallback", status);

    return status;
}
/* TBD: since application has to call this API for now */
EXPORT_SYMBOL(Platform_startCallback);

/*!
 *  @brief      Function called by ProcMgr when slave is in stopped state.
 *              TBD: logic would change completely in the final system.
 *
 *  @param      arg   Registered argument.
 */
Int32
Platform_stopCallback (UInt16 procId, Ptr arg)
{
    Int32                    status = Platform_S_SUCCESS;
    UInt32                   i;
    UInt32                   mAddr;
    UInt32                   start;
    UInt32                   numBytes;
    UInt32                   detachTimeout=0;
    UInt32                   fileId;
    ProcMgr_AddrInfo         ai;
    Platform_Handle          handle;
    SharedRegion_Entry       entry;
    UInt32 *                 entryBaseAddrs;

    GT_2trace (curTrace, GT_ENTER, "Platform_stopCallback", procId, arg);

    handle = (Platform_Handle) &Platform_objects [procId];

    GT_assert (curTrace, (handle != NULL));

    if (arg != NULL) {
        /* If arg is provided by user, then it is the slave virtual address of
         * the RESETVECTOR_SYMBOL.
         */
        start = (*(UInt32 *) arg);
    }
    else {
        fileId = ProcMgr_getLoadedFileId (handle->pmHandle);

        /* get the RESETVECTOR_SYMBOL address from slave binary */
        status = ProcMgr_getSymbolAddress (handle->pmHandle,
                                           fileId,
                                           RESETVECTOR_SYMBOL,
                                           &start);
        if (status < 0) {
            status = Platform_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "Platform_stopCallback",
                                 status,
                                 "ProcMgr_getSymbolAddress failed");
        }
    }

    /* Get and temporarily store the master kernel virtual addresses of the
     * SharedRegion entries.
     */
    entryBaseAddrs = Memory_alloc (NULL,
                                   (sizeof (UInt32) * handle->slaveCfg.numSRs),\
                                   0,
                                   NULL);
    if (entryBaseAddrs == NULL) {
        status = Platform_E_FAIL;
        GT_setFailureReason (curTrace,
                                GT_4CLASS,
                                "Platform_stopCallback",
                                status,
                                "Memory_alloc failed");
    }
    else {
        for (i = 0; (i < handle->slaveCfg.numSRs); i++) {
            status = SharedRegion_getEntry (i, &entry);
            if (status < 0) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "Platform_stopCallback",
                                     status,
                                     "SharedRegion_getEntry failed");
            }
            else {
                GT_assert (curTrace, (entry.isValid == TRUE));
                entryBaseAddrs [i] = (UInt32) entry.base;
            }
        }
    }

    /* Do IPC cleanup */
    do {
        status = Ipc_detach (procId);
        detachTimeout++;
        OsalThread_sleep(TIMEOUT_SLEEPTIME);
    } while ((status < 0 ) && (detachTimeout < TIMEOUT_LOOPCNT));

    if (detachTimeout >= TIMEOUT_LOOPCNT) {
        status = Platform_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "Platform_stopCallback",
                             status,
                             "Ipc_detach timeout");
    }

    /* Reset timeout value */
    detachTimeout=0;

    /* Check for IPC stop on slave until succesful or times out */
    do {
        /* Read sr0MemorySetup */
        numBytes = sizeof (Platform_SlaveConfig);
        status = _Platform_readSlaveMemory (procId,
                                            start,
                                            &handle->slaveCfg,
                                            &numBytes);

        detachTimeout++;
        OsalThread_sleep(TIMEOUT_SLEEPTIME);
    } while ( (handle->slaveCfg.sr0MemorySetup == 1)
              && (detachTimeout < TIMEOUT_LOOPCNT) );

    if (detachTimeout >= TIMEOUT_LOOPCNT) {
        status = Platform_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "Platform_stopCallback",
                             status,
                             "Ipc_stop timeout - Ipc_stop not called by slave?");
     }

    Ipc_stop ();

    if (entryBaseAddrs != NULL) {
        /* Cleanup the shared region entries mapping here */
        for (i = 0;
             (   (handle->slaveSRCfg != NULL)
              && (i < handle->slaveCfg.numSRs));
             i++) {

            /* Stop the shared regions that are exclsuively started for  this
             * slave
             */
            if (handle->bslaveAdditionalReg[i] == TRUE) {
                SharedRegion_clearEntry (i);
                handle->bslaveAdditionalReg[i] = FALSE;
            }
            /* Translate the slave address to master */
            status = ProcMgr_translateAddr (handle->pmHandle,
                                        (Ptr *)&mAddr,
                                        ProcMgr_AddrType_MasterPhys,
                                        (Ptr)handle->slaveSRCfg [i].entryBase,
                                        ProcMgr_AddrType_SlaveVirt);
            if (status < 0) {
                status = Platform_E_FAIL;
                GT_setFailureReason (curTrace,
                                     GT_4CLASS,
                                     "Platform_stopCallback",
                                     status,
                                     "ProcMgr_translateAddr failed");
            }
            else {
                ai.addr [ProcMgr_AddrType_MasterPhys] = mAddr;
                ai.addr [ProcMgr_AddrType_SlaveVirt]  =
                                               handle->slaveSRCfg [i].entryBase;
                ai.addr [ProcMgr_AddrType_MasterKnlVirt] = entryBaseAddrs [i];
                ai.size = handle->slaveSRCfg [i].entryLen;
                ai.isCached = FALSE;
                status = ProcMgr_unmap (handle->pmHandle,
                                        (  ProcMgr_MapType_SlaveVirt
                                         | ProcMgr_MapType_MasterKnlVirt),
                                        &ai,
                                        ProcMgr_AddrType_MasterPhys);
                if (status < 0) {
                    GT_setFailureReason (curTrace,
                                         GT_4CLASS,
                                         "Platform_stopCallback",
                                         status,
                                         "ProcMgr_unmap failed!");
                }
            }
        }
    }

    Platform_refCount--;
    if (Platform_refCount == 0) {
        if (handle->bslaveAdditionalReg != NULL) {
            Memory_free (NULL,
                         handle->bslaveAdditionalReg,
                         handle->slaveCfg.numSRs * sizeof (UInt8));
            handle->bslaveAdditionalReg = NULL;
        }
        if (handle->slaveSRCfg != NULL) {
            Memory_free (NULL,
                         handle->slaveSRCfg,
                         handle->slaveCfg.numSRs * sizeof (Platform_SlaveSRConfig));
            handle->slaveSRCfg = NULL;
        }
    }

    if (entryBaseAddrs != NULL) {
        Memory_free (NULL,
                     entryBaseAddrs,
                     handle->slaveCfg.numSRs * sizeof (UInt32));
        entryBaseAddrs = NULL;
    }

    GT_1trace (curTrace, GT_LEAVE, "Platform_stopCallback", status);

    return status;
}

/* TBD: since application has to call this API for now */
EXPORT_SYMBOL(Platform_stopCallback);

/*  ----------------------------------------------------------------------------
 *  Internal functions
 *  ----------------------------------------------------------------------------
 */
/* Function to read slave memory */
Int32
_Platform_readSlaveMemory (UInt16   procId,
                           UInt32   addr,
                           Ptr      value,
                           UInt32 * numBytes)
{
    Int              status = 0;
    Bool             done = FALSE;
    Platform_Handle  handle;
    ProcMgr_AddrInfo aInfo;
    UInt32           mAddr;

    GT_4trace (curTrace, GT_ENTER, "_Platform_readSlaveMemory",
    procId, addr, value, numBytes);

    handle = (Platform_Handle) &Platform_objects [procId];

    /* Translate the slave address to master address */
    status = ProcMgr_translateAddr (handle->pmHandle,
                                    (Ptr *)&mAddr,
                                    ProcMgr_AddrType_MasterPhys,
                                    (Ptr)addr,
                                    ProcMgr_AddrType_SlaveVirt);
    if (status < 0) {
        status = Platform_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "_Platform_readSlaveMemory",
                             status,
                             "ProcMgr_translateAddr failed");
    }

    if (status >= 0) {
        status = ProcMgr_translateAddr (handle->pmHandle,
                                        (Ptr *)&mAddr,
                                        ProcMgr_AddrType_MasterKnlVirt,
                                        (Ptr)mAddr,
                                        ProcMgr_AddrType_MasterPhys);
       if (status >= 0) {
            Memory_copy (value, (Ptr) mAddr, *numBytes);
            done = TRUE;
       }
    }

    if ((status >= 0) && (done == FALSE)) {
        /* Map the address */
        aInfo.addr [ProcMgr_AddrType_MasterPhys] = mAddr;
        aInfo.addr [ProcMgr_AddrType_SlaveVirt]  = addr;
        aInfo.size = *numBytes;
        aInfo.isCached = FALSE;
        status = ProcMgr_map (handle->pmHandle,
                              (  ProcMgr_MapType_MasterKnlVirt
                               | ProcMgr_MapType_SlaveVirt),
                              &aInfo,
                              ProcMgr_AddrType_MasterPhys);
        if (status < 0) {
            status = Platform_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_readSlaveMemory",
                                 status,
                                 "ProcMgr_map failed");
        }
    }

    if ((status >= 0) && (done == FALSE)) {
        status = ProcMgr_read (handle->pmHandle,
                               addr,
                               numBytes,
                               value);
        if (status < 0) {
            status = Platform_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_readSlaveMemory",
                                 status,
                                 "ProcMgr_read failed");
        }
    }

    if ((status >= 0) && (done == FALSE)) {
        /* Map the address */
        status = ProcMgr_unmap (handle->pmHandle,
                                (  ProcMgr_MapType_MasterKnlVirt
                                 | ProcMgr_MapType_SlaveVirt),
                                &aInfo,
                                ProcMgr_AddrType_MasterPhys);
        if (status < 0) {
            status = Platform_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_readSlaveMemory",
                                 status,
                                 "ProcMgr_unmap failed");
        }
    }

    GT_1trace (curTrace, GT_LEAVE, "_Platform_readSlaveMemory", status);

    return status;
}


/* Function to write slave memory */
Int32
_Platform_writeSlaveMemory (UInt16   procId,
                            UInt32   addr,
                            Ptr      value,
                            UInt32 * numBytes)
{
    Int              status = 0;
    Bool             done = FALSE;
    Platform_Handle  handle;
    ProcMgr_AddrInfo aInfo;
    UInt32           mAddr;

    GT_4trace (curTrace, GT_ENTER, "_Platform_writeSlaveMemory",
    procId, addr, value, numBytes);

    handle = (Platform_Handle) &Platform_objects [procId];

    /* Translate the slave address to master address */
    status = ProcMgr_translateAddr (handle->pmHandle,
                                    (Ptr *)&mAddr,
                                    ProcMgr_AddrType_MasterPhys,
                                    (Ptr)addr,
                                    ProcMgr_AddrType_SlaveVirt);
    if (status < 0) {
        status = Platform_E_FAIL;
        GT_setFailureReason (curTrace,
                             GT_4CLASS,
                             "_Platform_writeSlaveMemory",
                             status,
                             "ProcMgr_translateAddr failed");
    }

    if (status >= 0) {
        status = ProcMgr_translateAddr (handle->pmHandle,
                                        (Ptr *)&mAddr,
                                        ProcMgr_AddrType_MasterKnlVirt,
                                        (Ptr)mAddr,
                                        ProcMgr_AddrType_MasterPhys);
       if (status >= 0) {
            Memory_copy ((Ptr) mAddr, value, *numBytes);
            done = TRUE;
       }
    }

    if ((status >= 0) && (done == FALSE)) {
        /* Map the address */
        aInfo.addr [ProcMgr_AddrType_MasterPhys] = mAddr;
        aInfo.addr [ProcMgr_AddrType_SlaveVirt]  = addr;
        aInfo.size = *numBytes;
        aInfo.isCached = FALSE;
        status = ProcMgr_map (handle->pmHandle,
                              (  ProcMgr_MapType_MasterKnlVirt
                               | ProcMgr_MapType_SlaveVirt),
                              &aInfo,
                              ProcMgr_AddrType_MasterPhys);
        if (status < 0) {
            status = Platform_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_writeSlaveMemory",
                                 status,
                                 "ProcMgr_map failed");
        }
    }

    if ((status >= 0) && (done == FALSE)) {
        status = ProcMgr_write (handle->pmHandle,
                                addr,
                                numBytes,
                                value);
        if (status < 0) {
            status = Platform_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_writeSlaveMemory",
                                 status,
                                 "ProcMgr_write failed");
        }
    }

    if ((status >= 0) && (done == FALSE))  {
        /* Map the address */
        status = ProcMgr_unmap (handle->pmHandle,
                                (  ProcMgr_MapType_MasterKnlVirt
                                 | ProcMgr_MapType_SlaveVirt),
                                &aInfo,
                                ProcMgr_AddrType_MasterPhys);
        if (status < 0) {
            status = Platform_E_FAIL;
            GT_setFailureReason (curTrace,
                                 GT_4CLASS,
                                 "_Platform_writeSlaveMemory",
                                 status,
                                 "ProcMgr_unmap failed");
        }
    }

    GT_1trace (curTrace, GT_LEAVE, "_Platform_writeSlaveMemory", status);

    return status;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
