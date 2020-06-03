/*
 *  @file   FrameQ_ShMem_common.c
 *
 *  @brief      Implements FrameQ SharedMemory common functions independent of
 *              knl and user virtual space.
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



/* Standard headers */
#include <ti/syslink/Std.h>

/* Utilities & OSAL headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/Memory.h>

#include <ti/ipc/GateMP.h>
#include <_GateMP.h>

/* Module level headers */
#include <_ListMP.h>
#include <ti/ipc/ListMP.h>
#include <ti/syslink/inc/ClientNotifyMgr.h>

#include <ti/syslink/FrameQBufMgr.h>


#include <ti/syslink/FrameQDefs.h>
#include <_FrameQ.h>
#include <ti/syslink/FrameQ.h>

#include <_FrameQ_ShMem.h>
#include <ti/syslink/FrameQ_ShMem.h>


#if defined (__cplusplus)
extern "C" {
#endif

#if 0
/*
 *  @brief  Function to find out the  shared memory
 *          requirements for the instance.
 *  @param  instParams Instance param structure.
 */
UInt32
FrameQ_ShMem_sharedMemReq (const FrameQ_ShMem_Params *params)
{
    UInt32                 totalSize     = 0;
    ListMP_Params          listMPParams;
    ClientNotifyMgr_Params clientMgrParams;
    GateMP_Params          gateParams;
    SizeT minAlign;
    UInt16 regionId;

    GT_1trace (curTrace, GT_ENTER, "FrameQ_ShMem_sharedMemReq", params);

    GT_assert (curTrace, (params != NULL));

    if (NULL != params) {

        if (params->sharedAddr != NULL) {
            regionId = SharedRegion_getId(params->sharedAddr);
        }
        else {
            regionId = params->regionId;
        }
        GT_assert(curTrace, (regionId != SharedRegion_INVALIDREGIONID));

        minAlign = Memory_getMaxDefaultTypeAlign();
        if (SharedRegion_getCacheLineSize(regionId) > minAlign) {
            minAlign = SharedRegion_getCacheLineSize(regionId);
        }


        totalSize = ROUND_UP(sizeof (FrameQ_NameServerEntry), minAlign);
        /* Attrs size */
        totalSize += ROUND_UP(sizeof (FrameQ_ShMem_Attrs), minAlign);
        /* Writer client */
        totalSize += ROUND_UP(sizeof (FrameQ_ShMem_WriterClient), minAlign);
        /* Reader Clients */
        totalSize += (ROUND_UP(sizeof (FrameQ_ShMem_ReaderClient),minAlign))
                      * params->numReaders;
        /* Reader queues */
        totalSize += (ROUND_UP(sizeof (FrameQ_ShMem_FrameQueue),minAlign))
                     * params->numReaders;

        GateMP_Params_init(&gateParams);
        gateParams.remoteProtect = params->remoteProtect;
        gateParams.localProtect  = params->localProtect;

        if (FrameQ_ShMem_module->cfg.usedefaultgate == FALSE) {
            if(params->gate == NULL) {
                GateMP_Params_init(&gateParams);
                gateParams.remoteProtect =
                                  (GateMP_RemoteProtect) params->remoteProtect;

                gateParams.localProtect  =
                                      (GateMP_LocalProtect)params->localProtect;
                totalSize += ROUND_UP(GateMP_sharedMemReq(&gateParams),
                                       minAlign);/* for self */
                gateParams.localProtect  =
                             (GateMP_LocalProtect)GateMP_LocalProtect_INTERRUPT;
                totalSize += ROUND_UP(GateMP_sharedMemReq(&gateParams),
                                       minAlign);/* For ClientNotifyMgr */
            }
        }
        /* Get the default params from the client notify manager */
        ClientNotifyMgr_Params_init(&clientMgrParams);
        totalSize += ROUND_UP(ClientNotifyMgr_sharedMemReq (
                                   (ClientNotifyMgr_Params*)&clientMgrParams ),
                              minAlign);

        ListMP_Params_init (&listMPParams);
        /* Calculate the shared memory required for the ListMP objects  that are
         * required for all configured readers.
         */
        totalSize += (  ROUND_UP(ListMP_sharedMemReq(&listMPParams),
                                 minAlign)
                      * params->numReaders
                      * params->numQueues);
    }

    GT_0trace (curTrace, GT_LEAVE, "FrameQ_ShMem_sharedMemReq");

    return (totalSize) ;
}
#endif

#if defined (__cplusplus)
 }
#endif


