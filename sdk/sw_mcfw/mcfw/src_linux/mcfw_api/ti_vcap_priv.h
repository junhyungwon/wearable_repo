#ifndef __TI_VCAP_PRIV_H__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define __TI_VCAP_PRIV_H__


#include "ti_vcap.h"

#include <mcfw/interfaces/link_api/system.h>
#include <mcfw/interfaces/link_api/captureLink.h>
#include <mcfw/interfaces/link_api/deiLink.h>
#include <mcfw/interfaces/link_api/nsfLink.h>
#include <mcfw/interfaces/link_api/algLink.h>

#define MAX_DEI_LINK    (3)
#define NUM_CH_PER_DEI  (8)
#define MAX_IPC_FRAMES_LINK    (2)
#define MAX_ALG_LINK    (2)
#define MAX_SCLR_LINK   (2)
#define MAX_NSF_LINK   (2)

/* =============================================================================
 * Structure
 * =============================================================================
 */
typedef struct
{
    UInt32                         captureId;
    UInt32                         dspAlgId[MAX_ALG_LINK];
    UInt32                         nsfId[MAX_NSF_LINK];
    UInt32                         sclrId[MAX_SCLR_LINK];
    UInt32                         deiId[MAX_DEI_LINK];
    UInt32                         ipcFramesInDspId[MAX_IPC_FRAMES_LINK];
    UInt32                         ipcFramesOutVpssId[MAX_IPC_FRAMES_LINK];
    UInt32                         nullSrcId;
    UInt32                         ipcFramesOutVpssToHostId;
    UInt32                         ipcFramesInHostId;
    UInt32                         ipcBitsInHLOSId;
    UInt32                         capSwMsId;
    UInt32                         numChannels;
    VCAP_PARAMS_S                  vcapConfig;
    VCAP_CALLBACK_S                callbackFxn;
    VCAP_CALLBACK_S                bitscallbackFxn;
    VCAP_VIDEO_SOURCE_STATUS_S     videoStatus;
    Ptr callbackArg;
    Ptr bitscallbackArg;
}VCAP_MODULE_CONTEXT_S;

extern VCAP_MODULE_CONTEXT_S gVcapModuleContext;

/**
    \brief Video decoder mode params

    \return NONE
*/

Int32 Vcap_delete();
Void Vcap_ipcFramesInCbFxn(Ptr cbCtx);
Int32 Vcap_setExtraFramesChId(UInt32 chId);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

