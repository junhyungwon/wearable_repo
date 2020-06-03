#ifndef __TI_VSYS_PRIV_H__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define __TI_VSYS_PRIV_H__

#include <mcfw/interfaces/link_api/system_const.h>

#include "ti_vsys.h"
#include "ti_vcam_priv.h"
#include "ti_mjpeg_priv.h"


#include <osa.h>
#include <mcfw/interfaces/link_api/system.h>
#include <mcfw/interfaces/link_api/captureLink.h>
#include <mcfw/interfaces/link_api/cameraLink.h>
#include <mcfw/interfaces/link_api/mjpegLink.h>
#include <mcfw/interfaces/link_api/deiLink.h>
#include <mcfw/interfaces/link_api/nsfLink.h>
#include <mcfw/interfaces/link_api/displayLink.h>
#include <mcfw/interfaces/link_api/nullLink.h>
#include <mcfw/interfaces/link_api/grpxLink.h>
#include <mcfw/interfaces/link_api/dupLink.h>
#include <mcfw/interfaces/link_api/selectLink.h>
#include <mcfw/interfaces/link_api/swMsLink.h>
#include <mcfw/interfaces/link_api/mergeLink.h>
#include <mcfw/interfaces/link_api/nullSrcLink.h>
#include <mcfw/interfaces/link_api/ipcLink.h>
#include <mcfw/interfaces/link_api/systemLink_m3vpss.h>
#include <mcfw/interfaces/link_api/systemLink_m3video.h>
#include <mcfw/interfaces/link_api/encLink.h>
#include <mcfw/interfaces/link_api/decLink.h>
#include <mcfw/interfaces/link_api/fdLink.h>
#include <mcfw/interfaces/link_api/swosdLink.h>
#include <mcfw/interfaces/link_api/vnfLink.h>
#include <mcfw/interfaces/link_api/glbceLink.h>
#include <mcfw/interfaces/link_api/ispLink.h>
#include <mcfw/interfaces/link_api/vaLink.h>
#include <mcfw/interfaces/link_api/vstabLink.h>
#include <mcfw/interfaces/link_api/glbceSupportLink.h>

#include <mcfw/interfaces/link_api/sclrLink.h>
#include <mcfw/interfaces/link_api/avsync.h>

#include <ti/syslink/utils/IHeap.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/ipc/SharedRegion.h>
#include <string.h>

#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/xdm.h>
#include <ti/xdais/dm/ivideo.h>
#include <ih264enc.h>
#include <ih264vdec.h>

#include <osa_thr.h>

#define VSYS_TIMER_TSK_PRI			(1)
#define VSYS_TIMER_TSK_STACK_SIZE	(0)

/* =============================================================================
 * Structure
 * =============================================================================
 */
typedef struct
{
    VSYS_PARAMS_S vsysConfig;
    UInt32 swOsdId;
} VSYS_MODULE_CONTEXT_S;

typedef struct
{
    OSA_ThrHndl hndl;
    UInt32 startCnt;
    UInt32 exitTimerTh;
    UInt32 exitTimerDone;

} VSYS_TIMER_OBJ;

#define MULTICH_INIT_STRUCT(structName,structObj)  structName##_Init(&structObj)

#include "ti_vdis_priv.h"
#include "ti_venc_priv.h"
#include "ti_vdec_priv.h"
#include "ti_vcap_priv.h"


extern VSYS_MODULE_CONTEXT_S gVsysModuleContext;

Int32 MultiCh_prfLoadCalcEnable(Bool enable, Bool printStatus, Bool printTskLoad);
Int32 MultiCh_prfLoadPrint(Bool printTskLoad,Bool resetTaskLoad);

//# for ubx
void mcfw_capture_display_init(void);
void mcfw_capture_display_exit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
