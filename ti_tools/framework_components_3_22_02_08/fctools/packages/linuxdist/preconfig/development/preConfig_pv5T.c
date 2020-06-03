/*
 *  Copyright 2012 by Texas Instruments Incorporated.
 *
 */

/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-y21
 */

#define __nested__
#define __config__

#include <xdc/std.h>

/*
 * ======== GENERATED SECTIONS ========
 *     
 *     MODULE INCLUDES
 *     
 *     <module-name> INTERNALS
 *     <module-name> INHERITS
 *     <module-name> VTABLE
 *     <module-name> PATCH TABLE
 *     <module-name> DECLARATIONS
 *     <module-name> OBJECT OFFSETS
 *     <module-name> TEMPLATES
 *     <module-name> INITIALIZERS
 *     <module-name> FUNCTION STUBS
 *     <module-name> PROXY BODY
 *     <module-name> OBJECT DESCRIPTOR
 *     <module-name> SYSTEM FUNCTIONS
 *     <module-name> PRAGMAS
 *     
 *     INITIALIZATION ENTRY POINT
 *     PROGRAM GLOBALS
 *     CLINK DIRECTIVES
 */


/*
 * ======== MODULE INCLUDES ========
 */

#include <ti/sdo/xdcruntime/linux/GateProcessSupport.h>
#include <ti/sdo/xdcruntime/linux/GateThreadSupport.h>
#include <ti/sdo/xdcruntime/linux/SemProcessSupport.h>
#include <ti/sdo/xdcruntime/linux/SemThreadSupport.h>
#include <ti/sdo/xdcruntime/linux/ThreadSupport.h>
#include <ti/sdo/xdcruntime/linux/TimestampPosix.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Core.h>
#include <xdc/runtime/Defaults.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Gate.h>
#include <xdc/runtime/GateNull.h>
#include <xdc/runtime/HeapStd.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/LoggerSys.h>
#include <xdc/runtime/Main.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/Rta.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/SysStd.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Text.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/knl/GateH.h>
#include <xdc/runtime/knl/GateProcess.h>
#include <xdc/runtime/knl/GateThread.h>
#include <xdc/runtime/knl/SemProcess.h>
#include <xdc/runtime/knl/SemThread.h>
#include <xdc/runtime/knl/Semaphore.h>
#include <xdc/runtime/knl/Sync.h>
#include <xdc/runtime/knl/SyncGeneric.h>
#include <xdc/runtime/knl/SyncNull.h>
#include <xdc/runtime/knl/SyncSemThread.h>
#include <xdc/runtime/knl/Thread.h>
#include <xdcruntime/cstubs/LoggerFlex.h>
#include <xdcruntime/heaps/HeapStdAlign.h>


/*
 * ======== ti.sdo.xdcruntime.linux.GateProcessSupport INTERNALS ========
 */

/* Module__ */
typedef struct ti_sdo_xdcruntime_linux_GateProcessSupport_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} ti_sdo_xdcruntime_linux_GateProcessSupport_Module__;

/* Module__root__V */
extern ti_sdo_xdcruntime_linux_GateProcessSupport_Module__ ti_sdo_xdcruntime_linux_GateProcessSupport_Module__root__V;

/* Object__ */
typedef struct ti_sdo_xdcruntime_linux_GateProcessSupport_Object__ {
    const ti_sdo_xdcruntime_linux_GateProcessSupport_Fxns__* __fxns;
    xdc_Int id;
    xdc_Int count;
    xdc_Int owner;
} ti_sdo_xdcruntime_linux_GateProcessSupport_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sdo_xdcruntime_linux_GateProcessSupport_Object__ obj;
} ti_sdo_xdcruntime_linux_GateProcessSupport_Object2__;

/* __ParamsPtr */
#ifdef ti_sdo_xdcruntime_linux_GateProcessSupport___VERS
    #define ti_sdo_xdcruntime_linux_GateProcessSupport___ParamsPtr xdc_UChar*
#else
    #define ti_sdo_xdcruntime_linux_GateProcessSupport___ParamsPtr xdc_Ptr
#endif


/*
 * ======== ti.sdo.xdcruntime.linux.GateThreadSupport INTERNALS ========
 */

/* Module__ */
typedef struct ti_sdo_xdcruntime_linux_GateThreadSupport_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} ti_sdo_xdcruntime_linux_GateThreadSupport_Module__;

/* Module__root__V */
extern ti_sdo_xdcruntime_linux_GateThreadSupport_Module__ ti_sdo_xdcruntime_linux_GateThreadSupport_Module__root__V;

/* Object__ */
typedef struct ti_sdo_xdcruntime_linux_GateThreadSupport_Object__ {
    const ti_sdo_xdcruntime_linux_GateThreadSupport_Fxns__* __fxns;
    xdc_Void* gate;
} ti_sdo_xdcruntime_linux_GateThreadSupport_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sdo_xdcruntime_linux_GateThreadSupport_Object__ obj;
} ti_sdo_xdcruntime_linux_GateThreadSupport_Object2__;

/* __ParamsPtr */
#ifdef ti_sdo_xdcruntime_linux_GateThreadSupport___VERS
    #define ti_sdo_xdcruntime_linux_GateThreadSupport___ParamsPtr xdc_UChar*
#else
    #define ti_sdo_xdcruntime_linux_GateThreadSupport___ParamsPtr xdc_Ptr
#endif


/*
 * ======== ti.sdo.xdcruntime.linux.SemProcessSupport INTERNALS ========
 */

/* Module__ */
typedef struct ti_sdo_xdcruntime_linux_SemProcessSupport_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} ti_sdo_xdcruntime_linux_SemProcessSupport_Module__;

/* Module__root__V */
extern ti_sdo_xdcruntime_linux_SemProcessSupport_Module__ ti_sdo_xdcruntime_linux_SemProcessSupport_Module__root__V;

/* Object__ */
typedef struct ti_sdo_xdcruntime_linux_SemProcessSupport_Object__ {
    const ti_sdo_xdcruntime_linux_SemProcessSupport_Fxns__* __fxns;
    xdc_Int id;
    xdc_Int key;
    xdc_Int count;
    ti_sdo_xdcruntime_linux_SemProcessSupport_Mode mode;
} ti_sdo_xdcruntime_linux_SemProcessSupport_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sdo_xdcruntime_linux_SemProcessSupport_Object__ obj;
} ti_sdo_xdcruntime_linux_SemProcessSupport_Object2__;

/* __ParamsPtr */
#ifdef ti_sdo_xdcruntime_linux_SemProcessSupport___VERS
    #define ti_sdo_xdcruntime_linux_SemProcessSupport___ParamsPtr xdc_UChar*
#else
    #define ti_sdo_xdcruntime_linux_SemProcessSupport___ParamsPtr xdc_Ptr
#endif


/*
 * ======== ti.sdo.xdcruntime.linux.SemThreadSupport INTERNALS ========
 */

/* Module__ */
typedef struct ti_sdo_xdcruntime_linux_SemThreadSupport_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} ti_sdo_xdcruntime_linux_SemThreadSupport_Module__;

/* Module__root__V */
extern ti_sdo_xdcruntime_linux_SemThreadSupport_Module__ ti_sdo_xdcruntime_linux_SemThreadSupport_Module__root__V;

/* Object__ */
typedef struct ti_sdo_xdcruntime_linux_SemThreadSupport_Object__ {
    const ti_sdo_xdcruntime_linux_SemThreadSupport_Fxns__* __fxns;
    xdc_Void* sem;
    ti_sdo_xdcruntime_linux_SemThreadSupport_Mode mode;
} ti_sdo_xdcruntime_linux_SemThreadSupport_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sdo_xdcruntime_linux_SemThreadSupport_Object__ obj;
} ti_sdo_xdcruntime_linux_SemThreadSupport_Object2__;

/* __ParamsPtr */
#ifdef ti_sdo_xdcruntime_linux_SemThreadSupport___VERS
    #define ti_sdo_xdcruntime_linux_SemThreadSupport___ParamsPtr xdc_UChar*
#else
    #define ti_sdo_xdcruntime_linux_SemThreadSupport___ParamsPtr xdc_Ptr
#endif


/*
 * ======== ti.sdo.xdcruntime.linux.ThreadSupport INTERNALS ========
 */

/* Module__ */
typedef struct ti_sdo_xdcruntime_linux_ThreadSupport_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} ti_sdo_xdcruntime_linux_ThreadSupport_Module__;

/* Module__root__V */
extern ti_sdo_xdcruntime_linux_ThreadSupport_Module__ ti_sdo_xdcruntime_linux_ThreadSupport_Module__root__V;

/* Object__ */
typedef struct ti_sdo_xdcruntime_linux_ThreadSupport_Object__ {
    const ti_sdo_xdcruntime_linux_ThreadSupport_Fxns__* __fxns;
    xdc_Ptr tls;
    ti_sdo_xdcruntime_linux_ThreadSupport_RunFxn startFxn;
    xdc_IArg startFxnArg;
    xdc_SizeT stackSize;
    xdc_Void* tid;
    xdc_Ptr stackBase;
    xdc_Int staticOsPri;
} ti_sdo_xdcruntime_linux_ThreadSupport_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    ti_sdo_xdcruntime_linux_ThreadSupport_Object__ obj;
} ti_sdo_xdcruntime_linux_ThreadSupport_Object2__;

/* __ParamsPtr */
#ifdef ti_sdo_xdcruntime_linux_ThreadSupport___VERS
    #define ti_sdo_xdcruntime_linux_ThreadSupport___ParamsPtr xdc_UChar*
#else
    #define ti_sdo_xdcruntime_linux_ThreadSupport___ParamsPtr xdc_Ptr
#endif


/*
 * ======== ti.sdo.xdcruntime.linux.TimestampPosix INTERNALS ========
 */


/*
 * ======== xdc.runtime.Assert INTERNALS ========
 */


/*
 * ======== xdc.runtime.Core INTERNALS ========
 */


/*
 * ======== xdc.runtime.Defaults INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_Defaults_Module__ {
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_Defaults_Module__;

/* Module__root__V */
extern xdc_runtime_Defaults_Module__ xdc_runtime_Defaults_Module__root__V;


/*
 * ======== xdc.runtime.Diags INTERNALS ========
 */


/*
 * ======== xdc.runtime.Error INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_Error_Module__ {
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_Error_Module__;

/* Module__root__V */
extern xdc_runtime_Error_Module__ xdc_runtime_Error_Module__root__V;


/*
 * ======== xdc.runtime.Gate INTERNALS ========
 */


/*
 * ======== xdc.runtime.GateNull INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_GateNull_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_GateNull_Module__;

/* Module__root__V */
extern xdc_runtime_GateNull_Module__ xdc_runtime_GateNull_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_GateNull_Object__ {
    const xdc_runtime_GateNull_Fxns__* __fxns;
} xdc_runtime_GateNull_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_GateNull_Object__ obj;
} xdc_runtime_GateNull_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_GateNull___VERS
    #define xdc_runtime_GateNull___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_GateNull___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.HeapStd INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_HeapStd_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_HeapStd_Module__;

/* Module__root__V */
extern xdc_runtime_HeapStd_Module__ xdc_runtime_HeapStd_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_HeapStd_Object__ {
    const xdc_runtime_HeapStd_Fxns__* __fxns;
    xdc_runtime_Memory_Size remainSize;
    xdc_runtime_Memory_Size startSize;
} xdc_runtime_HeapStd_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_HeapStd_Object__ obj;
} xdc_runtime_HeapStd_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_HeapStd___VERS
    #define xdc_runtime_HeapStd___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_HeapStd___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.Log INTERNALS ========
 */


/*
 * ======== xdc.runtime.LoggerSys INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_LoggerSys_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_LoggerSys_Module__;

/* Module__root__V */
extern xdc_runtime_LoggerSys_Module__ xdc_runtime_LoggerSys_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_LoggerSys_Object__ {
    const xdc_runtime_LoggerSys_Fxns__* __fxns;
    xdc_Bool enabled;
} xdc_runtime_LoggerSys_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_LoggerSys_Object__ obj;
} xdc_runtime_LoggerSys_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_LoggerSys___VERS
    #define xdc_runtime_LoggerSys___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_LoggerSys___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.LoggerSys_TimestampProxy INTERNALS ========
 */


/*
 * ======== xdc.runtime.Main INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_Main_Module__ {
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_Main_Module__;

/* Module__root__V */
extern xdc_runtime_Main_Module__ xdc_runtime_Main_Module__root__V;


/*
 * ======== xdc.runtime.Main_Module_GateProxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_Main_Module_GateProxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_Main_Module_GateProxy_Module__;

/* Module__root__V */
extern xdc_runtime_Main_Module_GateProxy_Module__ xdc_runtime_Main_Module_GateProxy_Module__root__V;

/* <-- ti_sdo_xdcruntime_linux_GateThreadSupport_Object */

/* Object */
typedef ti_sdo_xdcruntime_linux_GateThreadSupport_Object__ xdc_runtime_Main_Module_GateProxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_Main_Module_GateProxy_Object__ obj;
} xdc_runtime_Main_Module_GateProxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_Main_Module_GateProxy___VERS
    #define xdc_runtime_Main_Module_GateProxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_Main_Module_GateProxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.Memory INTERNALS ========
 */


/*
 * ======== xdc.runtime.Memory_HeapProxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_Memory_HeapProxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_Memory_HeapProxy_Module__;

/* Module__root__V */
extern xdc_runtime_Memory_HeapProxy_Module__ xdc_runtime_Memory_HeapProxy_Module__root__V;

/* Object__ */
typedef struct xdcruntime_heaps_HeapStdAlign_Object__ {
    const xdcruntime_heaps_HeapStdAlign_Fxns__* __fxns;
    xdc_runtime_Memory_Size remainSize;
    xdc_runtime_Memory_Size startSize;
} xdcruntime_heaps_HeapStdAlign_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdcruntime_heaps_HeapStdAlign_Object__ obj;
} xdcruntime_heaps_HeapStdAlign_Object2__;

/* Object */
typedef xdcruntime_heaps_HeapStdAlign_Object__ xdc_runtime_Memory_HeapProxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_Memory_HeapProxy_Object__ obj;
} xdc_runtime_Memory_HeapProxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_Memory_HeapProxy___VERS
    #define xdc_runtime_Memory_HeapProxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_Memory_HeapProxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.Registry INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_Registry_Module__ {
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_Registry_Module__;

/* Module__root__V */
extern xdc_runtime_Registry_Module__ xdc_runtime_Registry_Module__root__V;


/*
 * ======== xdc.runtime.Rta INTERNALS ========
 */


/*
 * ======== xdc.runtime.Startup INTERNALS ========
 */


/*
 * ======== xdc.runtime.SysStd INTERNALS ========
 */


/*
 * ======== xdc.runtime.System INTERNALS ========
 */


/*
 * ======== xdc.runtime.System_Module_GateProxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_System_Module_GateProxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_System_Module_GateProxy_Module__;

/* Module__root__V */
extern xdc_runtime_System_Module_GateProxy_Module__ xdc_runtime_System_Module_GateProxy_Module__root__V;

/* <-- ti_sdo_xdcruntime_linux_GateThreadSupport_Object */

/* Object */
typedef ti_sdo_xdcruntime_linux_GateThreadSupport_Object__ xdc_runtime_System_Module_GateProxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_System_Module_GateProxy_Object__ obj;
} xdc_runtime_System_Module_GateProxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_System_Module_GateProxy___VERS
    #define xdc_runtime_System_Module_GateProxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_System_Module_GateProxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.System_SupportProxy INTERNALS ========
 */


/*
 * ======== xdc.runtime.Text INTERNALS ========
 */


/*
 * ======== xdc.runtime.Timestamp INTERNALS ========
 */


/*
 * ======== xdc.runtime.Timestamp_SupportProxy INTERNALS ========
 */


/*
 * ======== xdc.runtime.knl.GateH INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_GateH_Module__ {
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_GateH_Module__;

/* Module__root__V */
extern xdc_runtime_knl_GateH_Module__ xdc_runtime_knl_GateH_Module__root__V;


/*
 * ======== xdc.runtime.knl.GateH_Proxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_GateH_Proxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_knl_GateH_Proxy_Module__;

/* Module__root__V */
extern xdc_runtime_knl_GateH_Proxy_Module__ xdc_runtime_knl_GateH_Proxy_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_knl_GateThread_Object__ {
    const xdc_runtime_knl_GateThread_Fxns__* __fxns;
    xdc_runtime_knl_GateThread_Proxy_Handle proxyHandle;
} xdc_runtime_knl_GateThread_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_GateThread_Object__ obj;
} xdc_runtime_knl_GateThread_Object2__;

/* Object */
typedef xdc_runtime_knl_GateThread_Object__ xdc_runtime_knl_GateH_Proxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_GateH_Proxy_Object__ obj;
} xdc_runtime_knl_GateH_Proxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_GateH_Proxy___VERS
    #define xdc_runtime_knl_GateH_Proxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_GateH_Proxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.GateProcess INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_GateProcess_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_GateProcess_Module__;

/* Module__root__V */
extern xdc_runtime_knl_GateProcess_Module__ xdc_runtime_knl_GateProcess_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_knl_GateProcess_Object__ {
    const xdc_runtime_knl_GateProcess_Fxns__* __fxns;
    xdc_runtime_knl_GateProcess_Proxy_Handle proxyHandle;
} xdc_runtime_knl_GateProcess_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_GateProcess_Object__ obj;
} xdc_runtime_knl_GateProcess_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_GateProcess___VERS
    #define xdc_runtime_knl_GateProcess___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_GateProcess___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.GateProcess_Proxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_GateProcess_Proxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_knl_GateProcess_Proxy_Module__;

/* Module__root__V */
extern xdc_runtime_knl_GateProcess_Proxy_Module__ xdc_runtime_knl_GateProcess_Proxy_Module__root__V;

/* <-- ti_sdo_xdcruntime_linux_GateProcessSupport_Object */

/* Object */
typedef ti_sdo_xdcruntime_linux_GateProcessSupport_Object__ xdc_runtime_knl_GateProcess_Proxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_GateProcess_Proxy_Object__ obj;
} xdc_runtime_knl_GateProcess_Proxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_GateProcess_Proxy___VERS
    #define xdc_runtime_knl_GateProcess_Proxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_GateProcess_Proxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.GateThread INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_GateThread_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_GateThread_Module__;

/* Module__root__V */
extern xdc_runtime_knl_GateThread_Module__ xdc_runtime_knl_GateThread_Module__root__V;

/* <-- xdc_runtime_knl_GateThread_Object */

/* __ParamsPtr */
#ifdef xdc_runtime_knl_GateThread___VERS
    #define xdc_runtime_knl_GateThread___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_GateThread___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.GateThread_Proxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_GateThread_Proxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_knl_GateThread_Proxy_Module__;

/* Module__root__V */
extern xdc_runtime_knl_GateThread_Proxy_Module__ xdc_runtime_knl_GateThread_Proxy_Module__root__V;

/* <-- ti_sdo_xdcruntime_linux_GateThreadSupport_Object */

/* Object */
typedef ti_sdo_xdcruntime_linux_GateThreadSupport_Object__ xdc_runtime_knl_GateThread_Proxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_GateThread_Proxy_Object__ obj;
} xdc_runtime_knl_GateThread_Proxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_GateThread_Proxy___VERS
    #define xdc_runtime_knl_GateThread_Proxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_GateThread_Proxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.SemProcess INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_SemProcess_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_SemProcess_Module__;

/* Module__root__V */
extern xdc_runtime_knl_SemProcess_Module__ xdc_runtime_knl_SemProcess_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_knl_SemProcess_Object__ {
    const xdc_runtime_knl_SemProcess_Fxns__* __fxns;
    xdc_runtime_knl_SemProcess_Proxy_Handle proxyHandle;
} xdc_runtime_knl_SemProcess_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_SemProcess_Object__ obj;
} xdc_runtime_knl_SemProcess_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_SemProcess___VERS
    #define xdc_runtime_knl_SemProcess___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_SemProcess___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.SemProcess_Proxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_SemProcess_Proxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_knl_SemProcess_Proxy_Module__;

/* Module__root__V */
extern xdc_runtime_knl_SemProcess_Proxy_Module__ xdc_runtime_knl_SemProcess_Proxy_Module__root__V;

/* <-- ti_sdo_xdcruntime_linux_SemProcessSupport_Object */

/* Object */
typedef ti_sdo_xdcruntime_linux_SemProcessSupport_Object__ xdc_runtime_knl_SemProcess_Proxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_SemProcess_Proxy_Object__ obj;
} xdc_runtime_knl_SemProcess_Proxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_SemProcess_Proxy___VERS
    #define xdc_runtime_knl_SemProcess_Proxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_SemProcess_Proxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.SemThread INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_SemThread_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_SemThread_Module__;

/* Module__root__V */
extern xdc_runtime_knl_SemThread_Module__ xdc_runtime_knl_SemThread_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_knl_SemThread_Object__ {
    const xdc_runtime_knl_SemThread_Fxns__* __fxns;
    xdc_runtime_knl_SemThread_Proxy_Handle proxyHandle;
} xdc_runtime_knl_SemThread_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_SemThread_Object__ obj;
} xdc_runtime_knl_SemThread_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_SemThread___VERS
    #define xdc_runtime_knl_SemThread___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_SemThread___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.SemThread_Proxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_SemThread_Proxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_knl_SemThread_Proxy_Module__;

/* Module__root__V */
extern xdc_runtime_knl_SemThread_Proxy_Module__ xdc_runtime_knl_SemThread_Proxy_Module__root__V;

/* <-- ti_sdo_xdcruntime_linux_SemThreadSupport_Object */

/* Object */
typedef ti_sdo_xdcruntime_linux_SemThreadSupport_Object__ xdc_runtime_knl_SemThread_Proxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_SemThread_Proxy_Object__ obj;
} xdc_runtime_knl_SemThread_Proxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_SemThread_Proxy___VERS
    #define xdc_runtime_knl_SemThread_Proxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_SemThread_Proxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.Semaphore INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_Semaphore_Module__ {
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_Semaphore_Module__;

/* Module__root__V */
extern xdc_runtime_knl_Semaphore_Module__ xdc_runtime_knl_Semaphore_Module__root__V;


/*
 * ======== xdc.runtime.knl.Semaphore_Proxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_Semaphore_Proxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_knl_Semaphore_Proxy_Module__;

/* Module__root__V */
extern xdc_runtime_knl_Semaphore_Proxy_Module__ xdc_runtime_knl_Semaphore_Proxy_Module__root__V;

/* <-- xdc_runtime_knl_SemThread_Object */

/* Object */
typedef xdc_runtime_knl_SemThread_Object__ xdc_runtime_knl_Semaphore_Proxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_Semaphore_Proxy_Object__ obj;
} xdc_runtime_knl_Semaphore_Proxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_Semaphore_Proxy___VERS
    #define xdc_runtime_knl_Semaphore_Proxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_Semaphore_Proxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.Sync INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_Sync_Module__ {
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_Sync_Module__;

/* Module__root__V */
extern xdc_runtime_knl_Sync_Module__ xdc_runtime_knl_Sync_Module__root__V;


/*
 * ======== xdc.runtime.knl.SyncGeneric INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_SyncGeneric_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_SyncGeneric_Module__;

/* Module__root__V */
extern xdc_runtime_knl_SyncGeneric_Module__ xdc_runtime_knl_SyncGeneric_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_knl_SyncGeneric_Object__ {
    const xdc_runtime_knl_SyncGeneric_Fxns__* __fxns;
    xdc_runtime_knl_SyncGeneric_SignalFunc userSignal;
    xdc_UArg signalArg;
    xdc_runtime_knl_SyncGeneric_WaitFunc userWait;
    xdc_UArg waitArg;
    xdc_runtime_knl_SyncGeneric_QueryFunc userQuery;
} xdc_runtime_knl_SyncGeneric_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_SyncGeneric_Object__ obj;
} xdc_runtime_knl_SyncGeneric_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_SyncGeneric___VERS
    #define xdc_runtime_knl_SyncGeneric___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_SyncGeneric___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.SyncNull INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_SyncNull_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_SyncNull_Module__;

/* Module__root__V */
extern xdc_runtime_knl_SyncNull_Module__ xdc_runtime_knl_SyncNull_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_knl_SyncNull_Object__ {
    const xdc_runtime_knl_SyncNull_Fxns__* __fxns;
} xdc_runtime_knl_SyncNull_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_SyncNull_Object__ obj;
} xdc_runtime_knl_SyncNull_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_SyncNull___VERS
    #define xdc_runtime_knl_SyncNull___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_SyncNull___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.SyncSemThread INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_SyncSemThread_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_SyncSemThread_Module__;

/* Module__root__V */
extern xdc_runtime_knl_SyncSemThread_Module__ xdc_runtime_knl_SyncSemThread_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_knl_SyncSemThread_Object__ {
    const xdc_runtime_knl_SyncSemThread_Fxns__* __fxns;
    xdc_runtime_knl_SemThread_Handle semThread;
} xdc_runtime_knl_SyncSemThread_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_SyncSemThread_Object__ obj;
} xdc_runtime_knl_SyncSemThread_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_SyncSemThread___VERS
    #define xdc_runtime_knl_SyncSemThread___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_SyncSemThread___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.Sync_Proxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_Sync_Proxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_knl_Sync_Proxy_Module__;

/* Module__root__V */
extern xdc_runtime_knl_Sync_Proxy_Module__ xdc_runtime_knl_Sync_Proxy_Module__root__V;

/* <-- xdc_runtime_knl_SyncNull_Object */

/* Object */
typedef xdc_runtime_knl_SyncNull_Object__ xdc_runtime_knl_Sync_Proxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_Sync_Proxy_Object__ obj;
} xdc_runtime_knl_Sync_Proxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_Sync_Proxy___VERS
    #define xdc_runtime_knl_Sync_Proxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_Sync_Proxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.Thread INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_Thread_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} xdc_runtime_knl_Thread_Module__;

/* Module__root__V */
extern xdc_runtime_knl_Thread_Module__ xdc_runtime_knl_Thread_Module__root__V;

/* Object__ */
typedef struct xdc_runtime_knl_Thread_Object__ {
    xdc_runtime_knl_Thread_Proxy_Handle proxyHandle;
    xdc_Ptr tls;
} xdc_runtime_knl_Thread_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_Thread_Object__ obj;
} xdc_runtime_knl_Thread_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_Thread___VERS
    #define xdc_runtime_knl_Thread___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_Thread___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdc.runtime.knl.Thread_Proxy INTERNALS ========
 */

/* Module__ */
typedef struct xdc_runtime_knl_Thread_Proxy_Module__ {
    xdc_runtime_Types_Link link;
} xdc_runtime_knl_Thread_Proxy_Module__;

/* Module__root__V */
extern xdc_runtime_knl_Thread_Proxy_Module__ xdc_runtime_knl_Thread_Proxy_Module__root__V;

/* <-- ti_sdo_xdcruntime_linux_ThreadSupport_Object */

/* Object */
typedef ti_sdo_xdcruntime_linux_ThreadSupport_Object__ xdc_runtime_knl_Thread_Proxy_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdc_runtime_knl_Thread_Proxy_Object__ obj;
} xdc_runtime_knl_Thread_Proxy_Object2__;

/* __ParamsPtr */
#ifdef xdc_runtime_knl_Thread_Proxy___VERS
    #define xdc_runtime_knl_Thread_Proxy___ParamsPtr xdc_UChar*
#else
    #define xdc_runtime_knl_Thread_Proxy___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdcruntime.cstubs.LoggerFlex INTERNALS ========
 */

/* Module__ */
typedef struct xdcruntime_cstubs_LoggerFlex_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} xdcruntime_cstubs_LoggerFlex_Module__;

/* Module__root__V */
extern xdcruntime_cstubs_LoggerFlex_Module__ xdcruntime_cstubs_LoggerFlex_Module__root__V;

/* Object__ */
typedef struct xdcruntime_cstubs_LoggerFlex_Object__ {
    const xdcruntime_cstubs_LoggerFlex_Fxns__* __fxns;
} xdcruntime_cstubs_LoggerFlex_Object__;

/* Object2__ */
typedef struct {
    xdc_runtime_Types_InstHdr hdr;
    xdcruntime_cstubs_LoggerFlex_Object__ obj;
} xdcruntime_cstubs_LoggerFlex_Object2__;

/* __ParamsPtr */
#ifdef xdcruntime_cstubs_LoggerFlex___VERS
    #define xdcruntime_cstubs_LoggerFlex___ParamsPtr xdc_UChar*
#else
    #define xdcruntime_cstubs_LoggerFlex___ParamsPtr xdc_Ptr
#endif


/*
 * ======== xdcruntime.heaps.HeapStdAlign INTERNALS ========
 */

/* Module__ */
typedef struct xdcruntime_heaps_HeapStdAlign_Module__ {
    xdc_runtime_Types_Link link;
    xdc_runtime_Types_DiagsMask mask;
} xdcruntime_heaps_HeapStdAlign_Module__;

/* Module__root__V */
extern xdcruntime_heaps_HeapStdAlign_Module__ xdcruntime_heaps_HeapStdAlign_Module__root__V;

/* <-- xdcruntime_heaps_HeapStdAlign_Object */

/* __ParamsPtr */
#ifdef xdcruntime_heaps_HeapStdAlign___VERS
    #define xdcruntime_heaps_HeapStdAlign___ParamsPtr xdc_UChar*
#else
    #define xdcruntime_heaps_HeapStdAlign___ParamsPtr xdc_Ptr
#endif


/*
 * ======== ti.sdo.xdcruntime.linux.TimestampPosix INHERITS ========
 */

__FAR__ const xdc_runtime_Types_Base xdc_runtime_IModule_Interface__BASE__C = {0};
__FAR__ const xdc_runtime_Types_Base xdc_runtime_ITimestampClient_Interface__BASE__C = {(void*)&xdc_runtime_IModule_Interface__BASE__C};
__FAR__ const xdc_runtime_Types_Base xdc_runtime_ITimestampProvider_Interface__BASE__C = {(void*)&xdc_runtime_ITimestampClient_Interface__BASE__C};


/*
 * ======== xdc.runtime.GateNull INHERITS ========
 */

__FAR__ const xdc_runtime_Types_Base xdc_runtime_IGateProvider_Interface__BASE__C = {(void*)&xdc_runtime_IModule_Interface__BASE__C};


/*
 * ======== xdc.runtime.HeapStd INHERITS ========
 */

__FAR__ const xdc_runtime_Types_Base xdc_runtime_IHeap_Interface__BASE__C = {(void*)&xdc_runtime_IModule_Interface__BASE__C};


/*
 * ======== xdc.runtime.LoggerSys INHERITS ========
 */

__FAR__ const xdc_runtime_Types_Base xdc_runtime_ILogger_Interface__BASE__C = {(void*)&xdc_runtime_IModule_Interface__BASE__C};


/*
 * ======== xdc.runtime.SysStd INHERITS ========
 */

__FAR__ const xdc_runtime_Types_Base xdc_runtime_ISystemSupport_Interface__BASE__C = {(void*)&xdc_runtime_IModule_Interface__BASE__C};


/*
 * ======== xdc.runtime.Timestamp INHERITS ========
 */



/*
 * ======== xdc.runtime.knl.GateProcess INHERITS ========
 */



/*
 * ======== xdc.runtime.knl.GateThread INHERITS ========
 */



/*
 * ======== xdc.runtime.knl.SemProcess INHERITS ========
 */

__FAR__ const xdc_runtime_Types_Base xdc_runtime_knl_ISemaphore_Interface__BASE__C = {(void*)&xdc_runtime_IModule_Interface__BASE__C};


/*
 * ======== xdc.runtime.knl.SemThread INHERITS ========
 */



/*
 * ======== xdc.runtime.knl.SyncGeneric INHERITS ========
 */

__FAR__ const xdc_runtime_Types_Base xdc_runtime_knl_ISync_Interface__BASE__C = {(void*)&xdc_runtime_IModule_Interface__BASE__C};


/*
 * ======== xdc.runtime.knl.SyncNull INHERITS ========
 */



/*
 * ======== xdc.runtime.knl.SyncSemThread INHERITS ========
 */



/*
 * ======== xdcruntime.cstubs.LoggerFlex INHERITS ========
 */



/*
 * ======== xdcruntime.heaps.HeapStdAlign INHERITS ========
 */



/*
 * ======== ti.sdo.xdcruntime.linux.TimestampPosix VTABLE ========
 */

/* Module__FXNS__C */
const ti_sdo_xdcruntime_linux_TimestampPosix_Fxns__ ti_sdo_xdcruntime_linux_TimestampPosix_Module__FXNS__C = {
    (void*)&xdc_runtime_ITimestampProvider_Interface__BASE__C, /* base__ */
    &ti_sdo_xdcruntime_linux_TimestampPosix_Module__FXNS__C.__sfxns, /* __sysp */
    ti_sdo_xdcruntime_linux_TimestampPosix_get32__E,
    ti_sdo_xdcruntime_linux_TimestampPosix_get64__E,
    ti_sdo_xdcruntime_linux_TimestampPosix_getFreq__E,
    {
        NULL, /* __create */
        NULL, /* __delete */
        NULL, /* __label */
        0x8029, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.GateNull VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_GateNull_Fxns__ xdc_runtime_GateNull_Module__FXNS__C = {
    (void*)&xdc_runtime_IGateProvider_Interface__BASE__C, /* base__ */
    &xdc_runtime_GateNull_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_GateNull_query__E,
    xdc_runtime_GateNull_enter__E,
    xdc_runtime_GateNull_leave__E,
    {
        xdc_runtime_GateNull_Object__create__S,
        xdc_runtime_GateNull_Object__delete__S,
        xdc_runtime_GateNull_Handle__label__S,
        0x8008, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.HeapStd VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_HeapStd_Fxns__ xdc_runtime_HeapStd_Module__FXNS__C = {
    (void*)&xdc_runtime_IHeap_Interface__BASE__C, /* base__ */
    &xdc_runtime_HeapStd_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_HeapStd_alloc__E,
    xdc_runtime_HeapStd_free__E,
    xdc_runtime_HeapStd_isBlocking__E,
    xdc_runtime_HeapStd_getStats__E,
    {
        xdc_runtime_HeapStd_Object__create__S,
        xdc_runtime_HeapStd_Object__delete__S,
        xdc_runtime_HeapStd_Handle__label__S,
        0x800d, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.LoggerSys VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_LoggerSys_Fxns__ xdc_runtime_LoggerSys_Module__FXNS__C = {
    (void*)&xdc_runtime_ILogger_Interface__BASE__C, /* base__ */
    &xdc_runtime_LoggerSys_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_LoggerSys_enable__E,
    xdc_runtime_LoggerSys_disable__E,
    xdc_runtime_LoggerSys_write0__E,
    xdc_runtime_LoggerSys_write1__E,
    xdc_runtime_LoggerSys_write2__E,
    xdc_runtime_LoggerSys_write4__E,
    xdc_runtime_LoggerSys_write8__E,
    {
        xdc_runtime_LoggerSys_Object__create__S,
        xdc_runtime_LoggerSys_Object__delete__S,
        xdc_runtime_LoggerSys_Handle__label__S,
        0x800a, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.SysStd VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_SysStd_Fxns__ xdc_runtime_SysStd_Module__FXNS__C = {
    (void*)&xdc_runtime_ISystemSupport_Interface__BASE__C, /* base__ */
    &xdc_runtime_SysStd_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_SysStd_abort__E,
    xdc_runtime_SysStd_exit__E,
    xdc_runtime_SysStd_flush__E,
    xdc_runtime_SysStd_putch__E,
    xdc_runtime_SysStd_ready__E,
    {
        NULL, /* __create */
        NULL, /* __delete */
        NULL, /* __label */
        0x8012, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.Timestamp VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_Timestamp_Fxns__ xdc_runtime_Timestamp_Module__FXNS__C = {
    (void*)&xdc_runtime_ITimestampClient_Interface__BASE__C, /* base__ */
    &xdc_runtime_Timestamp_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_Timestamp_get32__E,
    xdc_runtime_Timestamp_get64__E,
    xdc_runtime_Timestamp_getFreq__E,
    {
        NULL, /* __create */
        NULL, /* __delete */
        NULL, /* __label */
        0x8014, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.knl.GateProcess VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_knl_GateProcess_Fxns__ xdc_runtime_knl_GateProcess_Module__FXNS__C = {
    (void*)&xdc_runtime_IGateProvider_Interface__BASE__C, /* base__ */
    &xdc_runtime_knl_GateProcess_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_knl_GateProcess_query__E,
    xdc_runtime_knl_GateProcess_enter__E,
    xdc_runtime_knl_GateProcess_leave__E,
    {
        xdc_runtime_knl_GateProcess_Object__create__S,
        xdc_runtime_knl_GateProcess_Object__delete__S,
        xdc_runtime_knl_GateProcess_Handle__label__S,
        0x801b, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.knl.GateThread VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_knl_GateThread_Fxns__ xdc_runtime_knl_GateThread_Module__FXNS__C = {
    (void*)&xdc_runtime_IGateProvider_Interface__BASE__C, /* base__ */
    &xdc_runtime_knl_GateThread_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_knl_GateThread_query__E,
    xdc_runtime_knl_GateThread_enter__E,
    xdc_runtime_knl_GateThread_leave__E,
    {
        xdc_runtime_knl_GateThread_Object__create__S,
        xdc_runtime_knl_GateThread_Object__delete__S,
        xdc_runtime_knl_GateThread_Handle__label__S,
        0x801a, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.knl.SemProcess VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_knl_SemProcess_Fxns__ xdc_runtime_knl_SemProcess_Module__FXNS__C = {
    (void*)&xdc_runtime_knl_ISemaphore_Interface__BASE__C, /* base__ */
    &xdc_runtime_knl_SemProcess_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_knl_SemProcess_pend__E,
    xdc_runtime_knl_SemProcess_post__E,
    {
        xdc_runtime_knl_SemProcess_Object__create__S,
        xdc_runtime_knl_SemProcess_Object__delete__S,
        xdc_runtime_knl_SemProcess_Handle__label__S,
        0x801d, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.knl.SemThread VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_knl_SemThread_Fxns__ xdc_runtime_knl_SemThread_Module__FXNS__C = {
    (void*)&xdc_runtime_knl_ISemaphore_Interface__BASE__C, /* base__ */
    &xdc_runtime_knl_SemThread_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_knl_SemThread_pend__E,
    xdc_runtime_knl_SemThread_post__E,
    {
        xdc_runtime_knl_SemThread_Object__create__S,
        xdc_runtime_knl_SemThread_Object__delete__S,
        xdc_runtime_knl_SemThread_Handle__label__S,
        0x801c, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.knl.SyncGeneric VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_knl_SyncGeneric_Fxns__ xdc_runtime_knl_SyncGeneric_Module__FXNS__C = {
    (void*)&xdc_runtime_knl_ISync_Interface__BASE__C, /* base__ */
    &xdc_runtime_knl_SyncGeneric_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_knl_SyncGeneric_query__E,
    xdc_runtime_knl_SyncGeneric_signal__E,
    xdc_runtime_knl_SyncGeneric_wait__E,
    {
        xdc_runtime_knl_SyncGeneric_Object__create__S,
        xdc_runtime_knl_SyncGeneric_Object__delete__S,
        xdc_runtime_knl_SyncGeneric_Handle__label__S,
        0x801e, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.knl.SyncNull VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_knl_SyncNull_Fxns__ xdc_runtime_knl_SyncNull_Module__FXNS__C = {
    (void*)&xdc_runtime_knl_ISync_Interface__BASE__C, /* base__ */
    &xdc_runtime_knl_SyncNull_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_knl_SyncNull_query__E,
    xdc_runtime_knl_SyncNull_signal__E,
    xdc_runtime_knl_SyncNull_wait__E,
    {
        xdc_runtime_knl_SyncNull_Object__create__S,
        xdc_runtime_knl_SyncNull_Object__delete__S,
        xdc_runtime_knl_SyncNull_Handle__label__S,
        0x801f, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdc.runtime.knl.SyncSemThread VTABLE ========
 */

/* Module__FXNS__C */
const xdc_runtime_knl_SyncSemThread_Fxns__ xdc_runtime_knl_SyncSemThread_Module__FXNS__C = {
    (void*)&xdc_runtime_knl_ISync_Interface__BASE__C, /* base__ */
    &xdc_runtime_knl_SyncSemThread_Module__FXNS__C.__sfxns, /* __sysp */
    xdc_runtime_knl_SyncSemThread_query__E,
    xdc_runtime_knl_SyncSemThread_signal__E,
    xdc_runtime_knl_SyncSemThread_wait__E,
    {
        xdc_runtime_knl_SyncSemThread_Object__create__S,
        xdc_runtime_knl_SyncSemThread_Object__delete__S,
        xdc_runtime_knl_SyncSemThread_Handle__label__S,
        0x8020, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdcruntime.cstubs.LoggerFlex VTABLE ========
 */

/* Module__FXNS__C */
const xdcruntime_cstubs_LoggerFlex_Fxns__ xdcruntime_cstubs_LoggerFlex_Module__FXNS__C = {
    (void*)&xdc_runtime_ILogger_Interface__BASE__C, /* base__ */
    &xdcruntime_cstubs_LoggerFlex_Module__FXNS__C.__sfxns, /* __sysp */
    xdcruntime_cstubs_LoggerFlex_enable__E,
    xdcruntime_cstubs_LoggerFlex_disable__E,
    xdcruntime_cstubs_LoggerFlex_write0__E,
    xdcruntime_cstubs_LoggerFlex_write1__E,
    xdcruntime_cstubs_LoggerFlex_write2__E,
    xdcruntime_cstubs_LoggerFlex_write4__E,
    xdcruntime_cstubs_LoggerFlex_write8__E,
    {
        NULL, /* __create */
        xdcruntime_cstubs_LoggerFlex_Object__delete__S,
        xdcruntime_cstubs_LoggerFlex_Handle__label__S,
        0x802d, /* __mid */
    } /* __sfxns */
};


/*
 * ======== xdcruntime.heaps.HeapStdAlign VTABLE ========
 */

/* Module__FXNS__C */
const xdcruntime_heaps_HeapStdAlign_Fxns__ xdcruntime_heaps_HeapStdAlign_Module__FXNS__C = {
    (void*)&xdc_runtime_IHeap_Interface__BASE__C, /* base__ */
    &xdcruntime_heaps_HeapStdAlign_Module__FXNS__C.__sfxns, /* __sysp */
    xdcruntime_heaps_HeapStdAlign_alloc__E,
    xdcruntime_heaps_HeapStdAlign_free__E,
    xdcruntime_heaps_HeapStdAlign_isBlocking__E,
    xdcruntime_heaps_HeapStdAlign_getStats__E,
    {
        xdcruntime_heaps_HeapStdAlign_Object__create__S,
        xdcruntime_heaps_HeapStdAlign_Object__delete__S,
        xdcruntime_heaps_HeapStdAlign_Handle__label__S,
        0x802b, /* __mid */
    } /* __sfxns */
};


/*
 * ======== ti.sdo.xdcruntime.linux.GateProcessSupport DECLARATIONS ========
 */


/*
 * ======== ti.sdo.xdcruntime.linux.GateThreadSupport DECLARATIONS ========
 */

/* Object__table__V */
ti_sdo_xdcruntime_linux_GateThreadSupport_Object__ ti_sdo_xdcruntime_linux_GateThreadSupport_Object__table__V[1];


/*
 * ======== ti.sdo.xdcruntime.linux.SemProcessSupport DECLARATIONS ========
 */


/*
 * ======== ti.sdo.xdcruntime.linux.SemThreadSupport DECLARATIONS ========
 */

/* Module_State__ */
typedef struct ti_sdo_xdcruntime_linux_SemThreadSupport_Module_State__ {
    xdc_Void* mutex;
} ti_sdo_xdcruntime_linux_SemThreadSupport_Module_State__;

/* Module__state__V */
ti_sdo_xdcruntime_linux_SemThreadSupport_Module_State__ ti_sdo_xdcruntime_linux_SemThreadSupport_Module__state__V;


/*
 * ======== ti.sdo.xdcruntime.linux.ThreadSupport DECLARATIONS ========
 */

/* Module_State__ */
typedef struct ti_sdo_xdcruntime_linux_ThreadSupport_Module_State__ {
    xdc_Int lowestPriority;
    xdc_Int belowNormalPriority;
    xdc_Int normalPriority;
    xdc_Int aboveNormalPriority;
    xdc_Int highestPriority;
} ti_sdo_xdcruntime_linux_ThreadSupport_Module_State__;

/* Module__state__V */
ti_sdo_xdcruntime_linux_ThreadSupport_Module_State__ ti_sdo_xdcruntime_linux_ThreadSupport_Module__state__V;


/*
 * ======== ti.sdo.xdcruntime.linux.TimestampPosix DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Assert DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Core DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Defaults DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Diags DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Error DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_Error_Module_State__ {
    xdc_UInt16 count;
} xdc_runtime_Error_Module_State__;

/* Module__state__V */
xdc_runtime_Error_Module_State__ xdc_runtime_Error_Module__state__V;


/*
 * ======== xdc.runtime.Gate DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.GateNull DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.HeapStd DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_HeapStd_Module_State__ {
    xdc_runtime_Memory_Size remainRTSSize;
} xdc_runtime_HeapStd_Module_State__;

/* Module__state__V */
xdc_runtime_HeapStd_Module_State__ xdc_runtime_HeapStd_Module__state__V;


/*
 * ======== xdc.runtime.Log DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.LoggerSys DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.LoggerSys_TimestampProxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Main DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Main_Module_GateProxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Memory DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_Memory_Module_State__ {
    xdc_SizeT maxDefaultTypeAlign;
} xdc_runtime_Memory_Module_State__;

/* Module__state__V */
xdc_runtime_Memory_Module_State__ xdc_runtime_Memory_Module__state__V;


/*
 * ======== xdc.runtime.Memory_HeapProxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Registry DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_Registry_Module_State__ {
    xdc_runtime_Registry_Desc* listHead;
    xdc_runtime_Types_ModuleId curId;
} xdc_runtime_Registry_Module_State__;

/* Module__state__V */
xdc_runtime_Registry_Module_State__ xdc_runtime_Registry_Module__state__V;


/*
 * ======== xdc.runtime.Rta DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Startup DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_Startup_Module_State__ {
    xdc_Int* stateTab;
    xdc_Bool execFlag;
    xdc_Bool rtsDoneFlag;
} xdc_runtime_Startup_Module_State__;

/* Module__state__V */
xdc_runtime_Startup_Module_State__ xdc_runtime_Startup_Module__state__V;

/* --> xdc_runtime_Startup_lastFxns__A */
const __T1_xdc_runtime_Startup_lastFxns xdc_runtime_Startup_lastFxns__A[1];

/* --> xdc_runtime_System_Module_startup__E */
extern xdc_Int xdc_runtime_System_Module_startup__E(xdc_Int);

/* --> ti_sdo_xdcruntime_linux_GateThreadSupport_Module_startup__E */
extern xdc_Int ti_sdo_xdcruntime_linux_GateThreadSupport_Module_startup__E(xdc_Int);

/* --> ti_sdo_xdcruntime_linux_GateProcessSupport_Module_startup__E */
extern xdc_Int ti_sdo_xdcruntime_linux_GateProcessSupport_Module_startup__E(xdc_Int);

/* --> ti_sdo_xdcruntime_linux_SemThreadSupport_Module_startup__E */
extern xdc_Int ti_sdo_xdcruntime_linux_SemThreadSupport_Module_startup__E(xdc_Int);

/* --> ti_sdo_xdcruntime_linux_SemProcessSupport_Module_startup__E */
extern xdc_Int ti_sdo_xdcruntime_linux_SemProcessSupport_Module_startup__E(xdc_Int);

/* --> ti_sdo_xdcruntime_linux_ThreadSupport_Module_startup__E */
extern xdc_Int ti_sdo_xdcruntime_linux_ThreadSupport_Module_startup__E(xdc_Int);

/* --> xdcruntime_cstubs_LoggerFlex_Module_startup__E */
extern xdc_Int xdcruntime_cstubs_LoggerFlex_Module_startup__E(xdc_Int);

/* --> xdc_runtime_Startup_sfxnTab__A */
const __T1_xdc_runtime_Startup_sfxnTab xdc_runtime_Startup_sfxnTab__A[7];

/* --> xdc_runtime_Startup_sfxnRts__A */
const __T1_xdc_runtime_Startup_sfxnRts xdc_runtime_Startup_sfxnRts__A[7];


/*
 * ======== xdc.runtime.SysStd DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.System DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_System_Module_State__ {
    __TA_xdc_runtime_System_Module_State__atexitHandlers atexitHandlers;
    xdc_Int numAtexitHandlers;
    xdc_Int exitStatus;
} xdc_runtime_System_Module_State__;

/* --> xdc_runtime_System_Module_State_0_atexitHandlers__A */
__T1_xdc_runtime_System_Module_State__atexitHandlers xdc_runtime_System_Module_State_0_atexitHandlers__A[8];

/* Module__state__V */
xdc_runtime_System_Module_State__ xdc_runtime_System_Module__state__V;

/* --> xdc_runtime_System_printfExtend__I */
extern xdc_Int xdc_runtime_System_printfExtend__I(xdc_Char**,xdc_Char**,xdc_VaList*,xdc_runtime_System_ParseData*);


/*
 * ======== xdc.runtime.System_Module_GateProxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.System_SupportProxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Text DECLARATIONS ========
 */

/* Module_State__ */
typedef struct xdc_runtime_Text_Module_State__ {
    xdc_Ptr charBase;
    xdc_Ptr nodeBase;
} xdc_runtime_Text_Module_State__;

/* Module__state__V */
xdc_runtime_Text_Module_State__ xdc_runtime_Text_Module__state__V;

/* --> xdc_runtime_Text_charTab__A */
const __T1_xdc_runtime_Text_charTab xdc_runtime_Text_charTab__A[4184];

/* --> xdc_runtime_Text_nodeTab__A */
const __T1_xdc_runtime_Text_nodeTab xdc_runtime_Text_nodeTab__A[46];


/*
 * ======== xdc.runtime.Timestamp DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.Timestamp_SupportProxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.GateH DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.GateH_Proxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.GateProcess DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.GateProcess_Proxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.GateThread DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.GateThread_Proxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.SemProcess DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.SemProcess_Proxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.SemThread DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.SemThread_Proxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.Semaphore DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.Semaphore_Proxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.Sync DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.SyncGeneric DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.SyncNull DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.SyncSemThread DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.Sync_Proxy DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.Thread DECLARATIONS ========
 */


/*
 * ======== xdc.runtime.knl.Thread_Proxy DECLARATIONS ========
 */


/*
 * ======== xdcruntime.cstubs.LoggerFlex DECLARATIONS ========
 */

/* Object__table__V */
xdcruntime_cstubs_LoggerFlex_Object__ xdcruntime_cstubs_LoggerFlex_Object__table__V[1];


/*
 * ======== xdcruntime.heaps.HeapStdAlign DECLARATIONS ========
 */

/* Object__table__V */
xdcruntime_heaps_HeapStdAlign_Object__ xdcruntime_heaps_HeapStdAlign_Object__table__V[1];

/* Module_State__ */
typedef struct xdcruntime_heaps_HeapStdAlign_Module_State__ {
    xdc_runtime_Memory_Size remainRTSSize;
} xdcruntime_heaps_HeapStdAlign_Module_State__;

/* Module__state__V */
xdcruntime_heaps_HeapStdAlign_Module_State__ xdcruntime_heaps_HeapStdAlign_Module__state__V;


/*
 * ======== xdc.cfg.Program TEMPLATE ========
 */

/*
 *  ======== __ASM__ ========
 *  Define absolute path prefix for this executable's
 *  configuration generated files.
 */
xdc__META(__ASM__, "@(#)__ASM__ = /db/atree/library/trees/osal/osal-e09/src/linuxdist/preconfig/development/configPkg_v5T/package/cfg/preConfig_pv5T");

/*
 *  ======== __ISA__ ========
 *  Define the ISA of this executable.  This symbol is used by platform
 *  specific "exec" commands that support more than one ISA; e.g., gdb
 */
xdc__META(__ISA__, "@(#)__ISA__ = v5T");

/*
 *  ======== __PLAT__ ========
 *  Define the name of the platform that can run this executable.  This
 *  symbol is used by platform independent "exec" commands
 */
xdc__META(__PLAT__, "@(#)__PLAT__ = host.platforms.arm");

/*
 *  ======== __TARG__ ========
 *  Define the name of the target used to build this executable.
 */
xdc__META(__TARG__, "@(#)__TARG__ = gnu.targets.arm.GCArmv5T");

/*
 *  ======== __TRDR__ ========
 *  Define the name of the class that can read/parse this executable.
 */
xdc__META(__TRDR__, "@(#)__TRDR__ = ti.targets.omf.elf.Elf32");


/*
 * ======== xdc.runtime.Diags TEMPLATE ========
 */

xdc_runtime_Diags_DictElem xdc_runtime_Diags_dictElems[] = {
    {32772, &xdc_runtime_Defaults_Module__root__V.mask},
    {32774, &xdc_runtime_Error_Module__root__V.mask},
    {32779, &xdc_runtime_Main_Module__root__V.mask},
    {32782, &xdc_runtime_Registry_Module__root__V.mask},
    {32790, &xdc_runtime_knl_GateH_Module__root__V.mask},
    {32791, &xdc_runtime_knl_Semaphore_Module__root__V.mask},
    {32792, &xdc_runtime_knl_Thread_Module__root__V.mask},
    {32793, &xdc_runtime_knl_Sync_Module__root__V.mask},
    {32794, &xdc_runtime_knl_GateThread_Module__root__V.mask},
    {32795, &xdc_runtime_knl_GateProcess_Module__root__V.mask},
    {32796, &xdc_runtime_knl_SemThread_Module__root__V.mask},
    {32797, &xdc_runtime_knl_SemProcess_Module__root__V.mask},
    {32798, &xdc_runtime_knl_SyncGeneric_Module__root__V.mask},
    {32799, &xdc_runtime_knl_SyncNull_Module__root__V.mask},
    {32800, &xdc_runtime_knl_SyncSemThread_Module__root__V.mask},
    {32804, &ti_sdo_xdcruntime_linux_GateThreadSupport_Module__root__V.mask},
    {32805, &ti_sdo_xdcruntime_linux_GateProcessSupport_Module__root__V.mask},
    {32806, &ti_sdo_xdcruntime_linux_SemThreadSupport_Module__root__V.mask},
    {32807, &ti_sdo_xdcruntime_linux_SemProcessSupport_Module__root__V.mask},
    {32808, &ti_sdo_xdcruntime_linux_ThreadSupport_Module__root__V.mask},
    {32811, &xdcruntime_heaps_HeapStdAlign_Module__root__V.mask},
    {32813, &xdcruntime_cstubs_LoggerFlex_Module__root__V.mask},
    {0, 0}
};


/*
 * ======== xdc.runtime.Startup TEMPLATE ========
 */

/*
 *  ======== MODULE STARTUP DONE FUNCTIONS ========
 */
xdc_Bool xdc_runtime_System_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[0] < 0;
}
xdc_Bool ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[1] < 0;
}
xdc_Bool ti_sdo_xdcruntime_linux_GateProcessSupport_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[2] < 0;
}
xdc_Bool ti_sdo_xdcruntime_linux_SemThreadSupport_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[3] < 0;
}
xdc_Bool ti_sdo_xdcruntime_linux_SemProcessSupport_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[4] < 0;
}
xdc_Bool ti_sdo_xdcruntime_linux_ThreadSupport_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[5] < 0;
}
xdc_Bool xdcruntime_cstubs_LoggerFlex_Module__startupDone__F(void) {
    return (&xdc_runtime_Startup_Module__state__V)->stateTab == 0 || (&xdc_runtime_Startup_Module__state__V)->stateTab[6] < 0;
}

/* 
 * Startup_exec__I is an internal entry point called by target/platform
 * boot code. Boot code is not brought into a partial-link assembly. So,
 * without this pragma, whole program optimizers would otherwise optimize-out
 * this function.
 */
#ifdef __ti__
#pragma FUNC_EXT_CALLED(xdc_runtime_Startup_exec__I);
#endif

#ifdef __GNUC__
#if __GNUC__ >= 4
xdc_Void xdc_runtime_Startup_exec__I(void) __attribute__ ((externally_visible));
#endif
#endif

/*
 *  ======== xdc_runtime_Startup_exec__I ========
 *  Initialize all used modules that have startup functions
 */
xdc_Void xdc_runtime_Startup_exec__I(void)
{
    xdc_Int state[7];
    xdc_runtime_Startup_startModsFxn__C(state, 7);
}

/*
 * ======== xdc.runtime.Reset TEMPLATE ========
 */

/* 
 * Startup_reset__I is an internal entry point called by target/platform
 * boot code. Boot code is not brought into a partial-link assembly. So,
 * without this pragma, whole program optimizers would otherwise optimize-out
 * this function.
 */
#ifdef __ti__
#pragma FUNC_EXT_CALLED(xdc_runtime_Startup_reset__I);
#endif

#ifdef __GNUC__
#if __GNUC__ >= 4
xdc_Void xdc_runtime_Startup_reset__I(void) __attribute__ ((externally_visible));
#endif
#endif


/*
 *  ======== xdc_runtime_Startup_reset__I ========
 *  This function is called by bootstrap initialization code as early as
 *  possible in the startup process.  This function calls all functions in
 *  the Reset.fxns array _as well as_ Startup.resetFxn (if it's non-NULL)
 */
xdc_Void xdc_runtime_Startup_reset__I(void)
{
}

/*
 * ======== xdc.runtime.System TEMPLATE ========
 */


#include <xdc/std.h>
#include <limits.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Text.h>
#include <xdc/runtime/Assert.h>

/*
 *  ======== System_printfExtend__I ========
 *  This function processes optional extended formats of printf.
 *  
 *  It returns the number of characters added to the result.
 *
 *  Precision (maximum string length) is not supported for %$S.
 *
 *  Right-justified (which is default) minimum width is not supported
 *  for %$L, %$S, or %$F.
 */
Int xdc_runtime_System_printfExtend__I(Char **pbuf, Char **pfmt, 
    VaList *pva, xdc_runtime_System_ParseData *parse)
{
    Char    *fmt = *pfmt;
    Int     res;
    Char    c;
    Bool    found = FALSE;
    
    /* 
     * Create a local variable 'va' to ensure that the dereference of
     * pva only occurs once.
     */
    va_list va = *pva;
    
    res = 0;

    c = *fmt++;
    *pfmt = *pfmt + 1;

    
    if (c == '$') {
        c = *fmt++;
        *pfmt = *pfmt + 1;
        
        if (c == 'L') {
            xdc_runtime_Types_Label *lab = parse->aFlag ? 
                (xdc_runtime_Types_Label *)iargToPtr(va_arg(va, IArg)) :
                (xdc_runtime_Types_Label *)va_arg(va, void *);
            
            /* 
             * Call Text_putLab to write out the label, taking the precision 
             * into account.
             */
            res = xdc_runtime_Text_putLab(lab, pbuf, parse->precis);
            
            /*
             * Set the length to 0 to indicate to 'doPrint' that nothing should
             * be copied from parse.ptr.
             */
            parse->len = 0;
            
            /* Update the minimum width field. */
            parse->width -= res;
            
            found = TRUE;
        }
        
        if (c == 'F') {
            xdc_runtime_Types_Site site;
            
            /* Retrieve the file name string from the argument list */
            site.file = parse->aFlag ? (Char *) iargToPtr(va_arg(va, IArg)) :
                                       (Char *) va_arg(va, Char *);
            
            /* Retrieve the line number from the argument list. */
            site.line = parse->aFlag ? (Int) va_arg(va, IArg) : 
                                       (Int) va_arg(va, Int);
            
            /* 
             * Omit the 'mod' field, set it to 0.
             * '0' is a safe sentinel value - the IDs for named modules are 
             * 0x8000 and higher, and the IDs for unnamed modules go from 0x1
             * to 0x7fff.
             */
            site.mod = 0;
            
            /* 
             * Call putSite to format the file and line number.
             * If a precision was specified, it will be used as the maximum
             * string lengrth.
             */
            res = xdc_runtime_Text_putSite(&site, pbuf, parse->precis);
            
            /*
             * Set the length to 0 to indicate to 'doPrint' that nothing should
             * be copied from parse.ptr.
             */
            parse->len = 0;
       
            /* Update the minimum width field */
            parse->width -= res;
            
            found = TRUE;
        }
        
        if (c == 'S') {
            /* Retrieve the format string from the argument list */
            parse->ptr = parse->aFlag ? 
                (Char *) iargToPtr(va_arg(va, IArg)) :
                (Char *) va_arg(va, Char *);
            
            /* Update pva before passing it to doPrint. */
            *pva = va;
            
            /* Perform the recursive format. System_doPrint does not advance
             * the buffer pointer, so it has to be done explicitly.
             */
            res = xdc_runtime_System_doPrint__I(*pbuf, parse->ptr, pva, parse->aFlag);
            if (*pbuf) {
                *pbuf += res;
            }
            
            /* Update the temporary variable with any changes to *pva */
            va = *pva;
            
            /*
             * Set the length to 0 to indicate to 'doPrint' that nothing should
             * be copied from parse.ptr
             */
            parse->len = 0;

            /* Update the minimum width field */
            parse->width -= res;
            
            /* Indicate that we were able to interpret the specifier */
            found = TRUE;
        }
        
    }

    if (c == 'f') {
        /* support arguments _after_ optional float support */
        if (parse->aFlag) {
            (void)va_arg(va, IArg);
        }
        else {
            (void)va_arg(va, double);
        }
    }    

    if (found == FALSE) {
        /* other character (like %) copy to output */
        *(parse->ptr) = c;
        parse->len = 1;
    }

    /*
     * Before returning, we must update the value of pva. We use a label here
     * so that all return points will go through this update.
     * The 'goto end' is here to ensure that there is always a reference to the
     * label (to avoid the compiler complaining). 
     */
    goto end; 
end:
    *pva = va;
    return (res);
}

/*
 * ======== xdc.runtime.Text TEMPLATE ========
 */


/*
 *  ======== xdc_runtime_Text_visitRope__I ========
 *  This function is indirectly called within Text.c through
 *  the visitRopeFxn configuration parameter of xdc.runtime.Text.
 */
Void xdc_runtime_Text_visitRope__I(xdc_runtime_Text_RopeId rope,
    xdc_Fxn visFxn, xdc_Ptr visState)
{
    xdc_String stack[5];
    xdc_runtime_Text_visitRope2__I(rope, visFxn, visState, stack);
}


/*
 * ======== ti.sdo.xdcruntime.linux.GateProcessSupport INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sdo_xdcruntime_linux_GateProcessSupport_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const ti_sdo_xdcruntime_linux_GateProcessSupport_Params ti_sdo_xdcruntime_linux_GateProcessSupport_Object__PARAMS__C = {
    sizeof (ti_sdo_xdcruntime_linux_GateProcessSupport_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sdo_xdcruntime_linux_GateProcessSupport_Object__PARAMS__C.__iprms, /* instance */
    (xdc_Int)(-0x0 - 1),  /* key */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sdo_xdcruntime_linux_GateProcessSupport_Module__ ti_sdo_xdcruntime_linux_GateProcessSupport_Module__root__V = {
    {&ti_sdo_xdcruntime_linux_GateProcessSupport_Module__root__V.link,  /* link.next */
    &ti_sdo_xdcruntime_linux_GateProcessSupport_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsEnabled ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsIncluded ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsMask ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsMask__C = ((CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsMask)((void*)&ti_sdo_xdcruntime_linux_GateProcessSupport_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__gateObj ti_sdo_xdcruntime_linux_GateProcessSupport_Module__gateObj__C = ((CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__gatePrms ti_sdo_xdcruntime_linux_GateProcessSupport_Module__gatePrms__C = ((CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__id ti_sdo_xdcruntime_linux_GateProcessSupport_Module__id__C = (xdc_Bits16)0x8025;

/* Module__loggerDefined__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerDefined ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerObj ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerObj__C = ((CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn0 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn0__C = ((CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn1 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn1__C = ((CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn2 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn2__C = ((CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn4 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn4__C = ((CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn8 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn8__C = ((CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__startupDoneFxn ti_sdo_xdcruntime_linux_GateProcessSupport_Module__startupDoneFxn__C = ((CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Object__count ti_sdo_xdcruntime_linux_GateProcessSupport_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Object__heap ti_sdo_xdcruntime_linux_GateProcessSupport_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Object__sizeof ti_sdo_xdcruntime_linux_GateProcessSupport_Object__sizeof__C = sizeof(ti_sdo_xdcruntime_linux_GateProcessSupport_Object__);

/* Object__table__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_Object__table ti_sdo_xdcruntime_linux_GateProcessSupport_Object__table__C = 0;

/* A_invalidKey__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_A_invalidKey ti_sdo_xdcruntime_linux_GateProcessSupport_A_invalidKey__C = (((xdc_runtime_Assert_Id)504) << 16 | 16);

/* A_POSIX_Error__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_A_POSIX_Error ti_sdo_xdcruntime_linux_GateProcessSupport_A_POSIX_Error__C = (((xdc_runtime_Assert_Id)455) << 16 | 16);

/* E_POSIX_Error__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_E_POSIX_Error ti_sdo_xdcruntime_linux_GateProcessSupport_E_POSIX_Error__C = (((xdc_runtime_Error_Id)997) << 16 | 0);

/* L_extant__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_L_extant ti_sdo_xdcruntime_linux_GateProcessSupport_L_extant__C = (((xdc_runtime_Log_Event)3045) << 16 | 4);

/* L_create__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_L_create ti_sdo_xdcruntime_linux_GateProcessSupport_L_create__C = (((xdc_runtime_Log_Event)3110) << 16 | 4);

/* perms__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateProcessSupport_perms ti_sdo_xdcruntime_linux_GateProcessSupport_perms__C = (xdc_Int)0x1b6;


/*
 * ======== ti.sdo.xdcruntime.linux.GateThreadSupport INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sdo_xdcruntime_linux_GateThreadSupport_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const ti_sdo_xdcruntime_linux_GateThreadSupport_Params ti_sdo_xdcruntime_linux_GateThreadSupport_Object__PARAMS__C = {
    sizeof (ti_sdo_xdcruntime_linux_GateThreadSupport_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sdo_xdcruntime_linux_GateThreadSupport_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sdo_xdcruntime_linux_GateThreadSupport_Module__ ti_sdo_xdcruntime_linux_GateThreadSupport_Module__root__V = {
    {&ti_sdo_xdcruntime_linux_GateThreadSupport_Module__root__V.link,  /* link.next */
    &ti_sdo_xdcruntime_linux_GateThreadSupport_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Object__table__V */
ti_sdo_xdcruntime_linux_GateThreadSupport_Object__ ti_sdo_xdcruntime_linux_GateThreadSupport_Object__table__V[1] = {
    {/* instance#0 */
        0,
        ((xdc_Void*)0),  /* gate */
    },
};

/* Module__diagsEnabled__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsEnabled ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsIncluded ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsMask ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsMask__C = ((CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsMask)((void*)&ti_sdo_xdcruntime_linux_GateThreadSupport_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gateObj ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gateObj__C = ((CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gatePrms ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gatePrms__C = ((CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__id ti_sdo_xdcruntime_linux_GateThreadSupport_Module__id__C = (xdc_Bits16)0x8024;

/* Module__loggerDefined__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerDefined ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerObj ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerObj__C = ((CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn0 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn0__C = ((CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn1 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn1__C = ((CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn2 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn2__C = ((CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn4 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn4__C = ((CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn8 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn8__C = ((CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDoneFxn ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDoneFxn__C = ((CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Object__count ti_sdo_xdcruntime_linux_GateThreadSupport_Object__count__C = 1;

/* Object__heap__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Object__heap ti_sdo_xdcruntime_linux_GateThreadSupport_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Object__sizeof ti_sdo_xdcruntime_linux_GateThreadSupport_Object__sizeof__C = sizeof(ti_sdo_xdcruntime_linux_GateThreadSupport_Object__);

/* Object__table__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Object__table ti_sdo_xdcruntime_linux_GateThreadSupport_Object__table__C = ti_sdo_xdcruntime_linux_GateThreadSupport_Object__table__V;

/* A_POSIX_Error__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_A_POSIX_Error ti_sdo_xdcruntime_linux_GateThreadSupport_A_POSIX_Error__C = (((xdc_runtime_Assert_Id)455) << 16 | 16);

/* E_POSIX_Error__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_E_POSIX_Error ti_sdo_xdcruntime_linux_GateThreadSupport_E_POSIX_Error__C = (((xdc_runtime_Error_Id)997) << 16 | 0);


/*
 * ======== ti.sdo.xdcruntime.linux.SemProcessSupport INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sdo_xdcruntime_linux_SemProcessSupport_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const ti_sdo_xdcruntime_linux_SemProcessSupport_Params ti_sdo_xdcruntime_linux_SemProcessSupport_Object__PARAMS__C = {
    sizeof (ti_sdo_xdcruntime_linux_SemProcessSupport_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sdo_xdcruntime_linux_SemProcessSupport_Object__PARAMS__C.__iprms, /* instance */
    xdc_runtime_knl_ISemaphore_Mode_COUNTING,  /* mode */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sdo_xdcruntime_linux_SemProcessSupport_Module__ ti_sdo_xdcruntime_linux_SemProcessSupport_Module__root__V = {
    {&ti_sdo_xdcruntime_linux_SemProcessSupport_Module__root__V.link,  /* link.next */
    &ti_sdo_xdcruntime_linux_SemProcessSupport_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsEnabled ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsIncluded ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsMask ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsMask__C = ((CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsMask)((void*)&ti_sdo_xdcruntime_linux_SemProcessSupport_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__gateObj ti_sdo_xdcruntime_linux_SemProcessSupport_Module__gateObj__C = ((CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__gatePrms ti_sdo_xdcruntime_linux_SemProcessSupport_Module__gatePrms__C = ((CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__id ti_sdo_xdcruntime_linux_SemProcessSupport_Module__id__C = (xdc_Bits16)0x8027;

/* Module__loggerDefined__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerDefined ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerObj ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerObj__C = ((CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn0 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn0__C = ((CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn1 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn1__C = ((CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn2 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn2__C = ((CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn4 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn4__C = ((CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn8 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn8__C = ((CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__startupDoneFxn ti_sdo_xdcruntime_linux_SemProcessSupport_Module__startupDoneFxn__C = ((CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Object__count ti_sdo_xdcruntime_linux_SemProcessSupport_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Object__heap ti_sdo_xdcruntime_linux_SemProcessSupport_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Object__sizeof ti_sdo_xdcruntime_linux_SemProcessSupport_Object__sizeof__C = sizeof(ti_sdo_xdcruntime_linux_SemProcessSupport_Object__);

/* Object__table__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_Object__table ti_sdo_xdcruntime_linux_SemProcessSupport_Object__table__C = 0;

/* A_unsupportedTimeout__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_A_unsupportedTimeout ti_sdo_xdcruntime_linux_SemProcessSupport_A_unsupportedTimeout__C = (((xdc_runtime_Assert_Id)546) << 16 | 16);

/* A_invalidKey__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_A_invalidKey ti_sdo_xdcruntime_linux_SemProcessSupport_A_invalidKey__C = (((xdc_runtime_Assert_Id)504) << 16 | 16);

/* A_POSIX_Error__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_A_POSIX_Error ti_sdo_xdcruntime_linux_SemProcessSupport_A_POSIX_Error__C = (((xdc_runtime_Assert_Id)455) << 16 | 16);

/* E_POSIX_Error__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_E_POSIX_Error ti_sdo_xdcruntime_linux_SemProcessSupport_E_POSIX_Error__C = (((xdc_runtime_Error_Id)997) << 16 | 0);

/* L_extant__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_L_extant ti_sdo_xdcruntime_linux_SemProcessSupport_L_extant__C = (((xdc_runtime_Log_Event)3144) << 16 | 4);

/* L_create__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_L_create ti_sdo_xdcruntime_linux_SemProcessSupport_L_create__C = (((xdc_runtime_Log_Event)3208) << 16 | 4);

/* perms__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemProcessSupport_perms ti_sdo_xdcruntime_linux_SemProcessSupport_perms__C = (xdc_Int)0x1b6;


/*
 * ======== ti.sdo.xdcruntime.linux.SemThreadSupport INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sdo_xdcruntime_linux_SemThreadSupport_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const ti_sdo_xdcruntime_linux_SemThreadSupport_Params ti_sdo_xdcruntime_linux_SemThreadSupport_Object__PARAMS__C = {
    sizeof (ti_sdo_xdcruntime_linux_SemThreadSupport_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sdo_xdcruntime_linux_SemThreadSupport_Object__PARAMS__C.__iprms, /* instance */
    xdc_runtime_knl_ISemaphore_Mode_COUNTING,  /* mode */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sdo_xdcruntime_linux_SemThreadSupport_Module__ ti_sdo_xdcruntime_linux_SemThreadSupport_Module__root__V = {
    {&ti_sdo_xdcruntime_linux_SemThreadSupport_Module__root__V.link,  /* link.next */
    &ti_sdo_xdcruntime_linux_SemThreadSupport_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__state__V */
ti_sdo_xdcruntime_linux_SemThreadSupport_Module_State__ ti_sdo_xdcruntime_linux_SemThreadSupport_Module__state__V = {
    ((xdc_Void*)0),  /* mutex */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsEnabled ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsIncluded ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsMask ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsMask__C = ((CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsMask)((void*)&ti_sdo_xdcruntime_linux_SemThreadSupport_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__gateObj ti_sdo_xdcruntime_linux_SemThreadSupport_Module__gateObj__C = ((CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__gatePrms ti_sdo_xdcruntime_linux_SemThreadSupport_Module__gatePrms__C = ((CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__id ti_sdo_xdcruntime_linux_SemThreadSupport_Module__id__C = (xdc_Bits16)0x8026;

/* Module__loggerDefined__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerDefined ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerObj ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerObj__C = ((CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn0 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn0__C = ((CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn1 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn1__C = ((CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn2 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn2__C = ((CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn4 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn4__C = ((CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn8 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn8__C = ((CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__startupDoneFxn ti_sdo_xdcruntime_linux_SemThreadSupport_Module__startupDoneFxn__C = ((CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Object__count ti_sdo_xdcruntime_linux_SemThreadSupport_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Object__heap ti_sdo_xdcruntime_linux_SemThreadSupport_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Object__sizeof ti_sdo_xdcruntime_linux_SemThreadSupport_Object__sizeof__C = sizeof(ti_sdo_xdcruntime_linux_SemThreadSupport_Object__);

/* Object__table__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_Object__table ti_sdo_xdcruntime_linux_SemThreadSupport_Object__table__C = 0;

/* A_POSIX_Error__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_A_POSIX_Error ti_sdo_xdcruntime_linux_SemThreadSupport_A_POSIX_Error__C = (((xdc_runtime_Assert_Id)455) << 16 | 16);

/* E_POSIX_Error__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_E_POSIX_Error ti_sdo_xdcruntime_linux_SemThreadSupport_E_POSIX_Error__C = (((xdc_runtime_Error_Id)997) << 16 | 0);

/* E_resetError__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_SemThreadSupport_E_resetError ti_sdo_xdcruntime_linux_SemThreadSupport_E_resetError__C = (((xdc_runtime_Error_Id)1059) << 16 | 0);


/*
 * ======== ti.sdo.xdcruntime.linux.ThreadSupport INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc ti_sdo_xdcruntime_linux_ThreadSupport_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const ti_sdo_xdcruntime_linux_ThreadSupport_Params ti_sdo_xdcruntime_linux_ThreadSupport_Object__PARAMS__C = {
    sizeof (ti_sdo_xdcruntime_linux_ThreadSupport_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&ti_sdo_xdcruntime_linux_ThreadSupport_Object__PARAMS__C.__iprms, /* instance */
    ((xdc_IArg)0),  /* arg */
    xdc_runtime_knl_IThreadSupport_Priority_NORMAL,  /* priority */
    (xdc_Int)0x0,  /* osPriority */
    (xdc_SizeT)0x0,  /* stackSize */
    ((xdc_Ptr)0),  /* tls */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
ti_sdo_xdcruntime_linux_ThreadSupport_Module__ ti_sdo_xdcruntime_linux_ThreadSupport_Module__root__V = {
    {&ti_sdo_xdcruntime_linux_ThreadSupport_Module__root__V.link,  /* link.next */
    &ti_sdo_xdcruntime_linux_ThreadSupport_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__state__V */
ti_sdo_xdcruntime_linux_ThreadSupport_Module_State__ ti_sdo_xdcruntime_linux_ThreadSupport_Module__state__V = {
    (xdc_Int)0x0,  /* lowestPriority */
    (xdc_Int)0x0,  /* belowNormalPriority */
    (xdc_Int)0x0,  /* normalPriority */
    (xdc_Int)0x0,  /* aboveNormalPriority */
    (xdc_Int)0x0,  /* highestPriority */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsEnabled ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsIncluded ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsMask ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsMask__C = ((CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsMask)((void*)&ti_sdo_xdcruntime_linux_ThreadSupport_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__gateObj ti_sdo_xdcruntime_linux_ThreadSupport_Module__gateObj__C = ((CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__gatePrms ti_sdo_xdcruntime_linux_ThreadSupport_Module__gatePrms__C = ((CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__id ti_sdo_xdcruntime_linux_ThreadSupport_Module__id__C = (xdc_Bits16)0x8028;

/* Module__loggerDefined__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerDefined ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerObj ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerObj__C = ((CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn0 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn0__C = ((CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn1 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn1__C = ((CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn2 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn2__C = ((CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn4 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn4__C = ((CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn8 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn8__C = ((CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__startupDoneFxn ti_sdo_xdcruntime_linux_ThreadSupport_Module__startupDoneFxn__C = ((CT__ti_sdo_xdcruntime_linux_ThreadSupport_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Object__count ti_sdo_xdcruntime_linux_ThreadSupport_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Object__heap ti_sdo_xdcruntime_linux_ThreadSupport_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Object__sizeof ti_sdo_xdcruntime_linux_ThreadSupport_Object__sizeof__C = sizeof(ti_sdo_xdcruntime_linux_ThreadSupport_Object__);

/* Object__table__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_Object__table ti_sdo_xdcruntime_linux_ThreadSupport_Object__table__C = 0;

/* E_priority__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_E_priority ti_sdo_xdcruntime_linux_ThreadSupport_E_priority__C = (((xdc_runtime_Error_Id)1093) << 16 | 0);

/* A_POSIX_Error__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_A_POSIX_Error ti_sdo_xdcruntime_linux_ThreadSupport_A_POSIX_Error__C = (((xdc_runtime_Assert_Id)455) << 16 | 16);

/* E_POSIX_Error__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_E_POSIX_Error ti_sdo_xdcruntime_linux_ThreadSupport_E_POSIX_Error__C = (((xdc_runtime_Error_Id)997) << 16 | 0);

/* L_start__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_L_start ti_sdo_xdcruntime_linux_ThreadSupport_L_start__C = (((xdc_runtime_Log_Event)3241) << 16 | 4);

/* L_finish__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_L_finish ti_sdo_xdcruntime_linux_ThreadSupport_L_finish__C = (((xdc_runtime_Log_Event)3257) << 16 | 4);

/* L_join__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_ThreadSupport_L_join ti_sdo_xdcruntime_linux_ThreadSupport_L_join__C = (((xdc_runtime_Log_Event)3274) << 16 | 4);


/*
 * ======== ti.sdo.xdcruntime.linux.TimestampPosix INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsEnabled ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsEnabled__C = (xdc_Bits32)0x18;

/* Module__diagsIncluded__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsIncluded ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsIncluded__C = (xdc_Bits32)0x18;

/* Module__diagsMask__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsMask ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsMask__C = ((CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__gateObj ti_sdo_xdcruntime_linux_TimestampPosix_Module__gateObj__C = ((CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__gatePrms ti_sdo_xdcruntime_linux_TimestampPosix_Module__gatePrms__C = ((CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__id ti_sdo_xdcruntime_linux_TimestampPosix_Module__id__C = (xdc_Bits16)0x8029;

/* Module__loggerDefined__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerDefined ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerObj ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerObj__C = ((CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn0 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn0__C = ((CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn1 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn1__C = ((CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn2 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn2__C = ((CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn4 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn4__C = ((CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn8 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn8__C = ((CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__startupDoneFxn ti_sdo_xdcruntime_linux_TimestampPosix_Module__startupDoneFxn__C = ((CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Object__count ti_sdo_xdcruntime_linux_TimestampPosix_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Object__heap ti_sdo_xdcruntime_linux_TimestampPosix_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Object__sizeof ti_sdo_xdcruntime_linux_TimestampPosix_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Object__table ti_sdo_xdcruntime_linux_TimestampPosix_Object__table__C = 0;


/*
 * ======== xdc.runtime.Assert INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Assert_Module__diagsEnabled xdc_runtime_Assert_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Assert_Module__diagsIncluded xdc_runtime_Assert_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Assert_Module__diagsMask xdc_runtime_Assert_Module__diagsMask__C = ((CT__xdc_runtime_Assert_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Assert_Module__gateObj xdc_runtime_Assert_Module__gateObj__C = ((CT__xdc_runtime_Assert_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Assert_Module__gatePrms xdc_runtime_Assert_Module__gatePrms__C = ((CT__xdc_runtime_Assert_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Assert_Module__id xdc_runtime_Assert_Module__id__C = (xdc_Bits16)0x8002;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Assert_Module__loggerDefined xdc_runtime_Assert_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Assert_Module__loggerObj xdc_runtime_Assert_Module__loggerObj__C = ((CT__xdc_runtime_Assert_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Assert_Module__loggerFxn0 xdc_runtime_Assert_Module__loggerFxn0__C = ((CT__xdc_runtime_Assert_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Assert_Module__loggerFxn1 xdc_runtime_Assert_Module__loggerFxn1__C = ((CT__xdc_runtime_Assert_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Assert_Module__loggerFxn2 xdc_runtime_Assert_Module__loggerFxn2__C = ((CT__xdc_runtime_Assert_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Assert_Module__loggerFxn4 xdc_runtime_Assert_Module__loggerFxn4__C = ((CT__xdc_runtime_Assert_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Assert_Module__loggerFxn8 xdc_runtime_Assert_Module__loggerFxn8__C = ((CT__xdc_runtime_Assert_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Assert_Module__startupDoneFxn xdc_runtime_Assert_Module__startupDoneFxn__C = ((CT__xdc_runtime_Assert_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Assert_Object__count xdc_runtime_Assert_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Assert_Object__heap xdc_runtime_Assert_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Assert_Object__sizeof xdc_runtime_Assert_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Assert_Object__table xdc_runtime_Assert_Object__table__C = 0;

/* E_assertFailed__C */
__FAR__ const CT__xdc_runtime_Assert_E_assertFailed xdc_runtime_Assert_E_assertFailed__C = (((xdc_runtime_Error_Id)757) << 16 | 0);


/*
 * ======== xdc.runtime.Core INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Core_Module__diagsEnabled xdc_runtime_Core_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Core_Module__diagsIncluded xdc_runtime_Core_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Core_Module__diagsMask xdc_runtime_Core_Module__diagsMask__C = ((CT__xdc_runtime_Core_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Core_Module__gateObj xdc_runtime_Core_Module__gateObj__C = ((CT__xdc_runtime_Core_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Core_Module__gatePrms xdc_runtime_Core_Module__gatePrms__C = ((CT__xdc_runtime_Core_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Core_Module__id xdc_runtime_Core_Module__id__C = (xdc_Bits16)0x8003;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Core_Module__loggerDefined xdc_runtime_Core_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Core_Module__loggerObj xdc_runtime_Core_Module__loggerObj__C = ((CT__xdc_runtime_Core_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Core_Module__loggerFxn0 xdc_runtime_Core_Module__loggerFxn0__C = ((CT__xdc_runtime_Core_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Core_Module__loggerFxn1 xdc_runtime_Core_Module__loggerFxn1__C = ((CT__xdc_runtime_Core_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Core_Module__loggerFxn2 xdc_runtime_Core_Module__loggerFxn2__C = ((CT__xdc_runtime_Core_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Core_Module__loggerFxn4 xdc_runtime_Core_Module__loggerFxn4__C = ((CT__xdc_runtime_Core_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Core_Module__loggerFxn8 xdc_runtime_Core_Module__loggerFxn8__C = ((CT__xdc_runtime_Core_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Core_Module__startupDoneFxn xdc_runtime_Core_Module__startupDoneFxn__C = ((CT__xdc_runtime_Core_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Core_Object__count xdc_runtime_Core_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Core_Object__heap xdc_runtime_Core_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Core_Object__sizeof xdc_runtime_Core_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Core_Object__table xdc_runtime_Core_Object__table__C = 0;

/* A_initializedParams__C */
__FAR__ const CT__xdc_runtime_Core_A_initializedParams xdc_runtime_Core_A_initializedParams__C = (((xdc_runtime_Assert_Id)1) << 16 | 16);


/*
 * ======== xdc.runtime.Defaults INITIALIZERS ========
 */

/* Module__root__V */
xdc_runtime_Defaults_Module__ xdc_runtime_Defaults_Module__root__V = {
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__diagsEnabled xdc_runtime_Defaults_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__diagsIncluded xdc_runtime_Defaults_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__diagsMask xdc_runtime_Defaults_Module__diagsMask__C = ((CT__xdc_runtime_Defaults_Module__diagsMask)((void*)&xdc_runtime_Defaults_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__gateObj xdc_runtime_Defaults_Module__gateObj__C = ((CT__xdc_runtime_Defaults_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__gatePrms xdc_runtime_Defaults_Module__gatePrms__C = ((CT__xdc_runtime_Defaults_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__id xdc_runtime_Defaults_Module__id__C = (xdc_Bits16)0x8004;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerDefined xdc_runtime_Defaults_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerObj xdc_runtime_Defaults_Module__loggerObj__C = ((CT__xdc_runtime_Defaults_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerFxn0 xdc_runtime_Defaults_Module__loggerFxn0__C = ((CT__xdc_runtime_Defaults_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerFxn1 xdc_runtime_Defaults_Module__loggerFxn1__C = ((CT__xdc_runtime_Defaults_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerFxn2 xdc_runtime_Defaults_Module__loggerFxn2__C = ((CT__xdc_runtime_Defaults_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerFxn4 xdc_runtime_Defaults_Module__loggerFxn4__C = ((CT__xdc_runtime_Defaults_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__loggerFxn8 xdc_runtime_Defaults_Module__loggerFxn8__C = ((CT__xdc_runtime_Defaults_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Defaults_Module__startupDoneFxn xdc_runtime_Defaults_Module__startupDoneFxn__C = ((CT__xdc_runtime_Defaults_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Defaults_Object__count xdc_runtime_Defaults_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Defaults_Object__heap xdc_runtime_Defaults_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Defaults_Object__sizeof xdc_runtime_Defaults_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Defaults_Object__table xdc_runtime_Defaults_Object__table__C = 0;


/*
 * ======== xdc.runtime.Diags INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Diags_Module__diagsEnabled xdc_runtime_Diags_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Diags_Module__diagsIncluded xdc_runtime_Diags_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Diags_Module__diagsMask xdc_runtime_Diags_Module__diagsMask__C = ((CT__xdc_runtime_Diags_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Diags_Module__gateObj xdc_runtime_Diags_Module__gateObj__C = ((CT__xdc_runtime_Diags_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Diags_Module__gatePrms xdc_runtime_Diags_Module__gatePrms__C = ((CT__xdc_runtime_Diags_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Diags_Module__id xdc_runtime_Diags_Module__id__C = (xdc_Bits16)0x8005;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Diags_Module__loggerDefined xdc_runtime_Diags_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Diags_Module__loggerObj xdc_runtime_Diags_Module__loggerObj__C = ((CT__xdc_runtime_Diags_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Diags_Module__loggerFxn0 xdc_runtime_Diags_Module__loggerFxn0__C = ((CT__xdc_runtime_Diags_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Diags_Module__loggerFxn1 xdc_runtime_Diags_Module__loggerFxn1__C = ((CT__xdc_runtime_Diags_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Diags_Module__loggerFxn2 xdc_runtime_Diags_Module__loggerFxn2__C = ((CT__xdc_runtime_Diags_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Diags_Module__loggerFxn4 xdc_runtime_Diags_Module__loggerFxn4__C = ((CT__xdc_runtime_Diags_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Diags_Module__loggerFxn8 xdc_runtime_Diags_Module__loggerFxn8__C = ((CT__xdc_runtime_Diags_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Diags_Module__startupDoneFxn xdc_runtime_Diags_Module__startupDoneFxn__C = ((CT__xdc_runtime_Diags_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Diags_Object__count xdc_runtime_Diags_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Diags_Object__heap xdc_runtime_Diags_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Diags_Object__sizeof xdc_runtime_Diags_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Diags_Object__table xdc_runtime_Diags_Object__table__C = 0;

/* setMaskEnabled__C */
__FAR__ const CT__xdc_runtime_Diags_setMaskEnabled xdc_runtime_Diags_setMaskEnabled__C = 1;

/* dictBase__C */
__FAR__ const CT__xdc_runtime_Diags_dictBase xdc_runtime_Diags_dictBase__C = ((CT__xdc_runtime_Diags_dictBase)((void*)&xdc_runtime_Diags_dictElems[0]));


/*
 * ======== xdc.runtime.Error INITIALIZERS ========
 */

/* Module__root__V */
xdc_runtime_Error_Module__ xdc_runtime_Error_Module__root__V = {
    0,  /* mask */
};

/* Module__state__V */
xdc_runtime_Error_Module_State__ xdc_runtime_Error_Module__state__V = {
    (xdc_UInt16)0x0,  /* count */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Error_Module__diagsEnabled xdc_runtime_Error_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Error_Module__diagsIncluded xdc_runtime_Error_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Error_Module__diagsMask xdc_runtime_Error_Module__diagsMask__C = ((CT__xdc_runtime_Error_Module__diagsMask)((void*)&xdc_runtime_Error_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Error_Module__gateObj xdc_runtime_Error_Module__gateObj__C = ((CT__xdc_runtime_Error_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Error_Module__gatePrms xdc_runtime_Error_Module__gatePrms__C = ((CT__xdc_runtime_Error_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Error_Module__id xdc_runtime_Error_Module__id__C = (xdc_Bits16)0x8006;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Error_Module__loggerDefined xdc_runtime_Error_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Error_Module__loggerObj xdc_runtime_Error_Module__loggerObj__C = ((CT__xdc_runtime_Error_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Error_Module__loggerFxn0 xdc_runtime_Error_Module__loggerFxn0__C = ((CT__xdc_runtime_Error_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Error_Module__loggerFxn1 xdc_runtime_Error_Module__loggerFxn1__C = ((CT__xdc_runtime_Error_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Error_Module__loggerFxn2 xdc_runtime_Error_Module__loggerFxn2__C = ((CT__xdc_runtime_Error_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Error_Module__loggerFxn4 xdc_runtime_Error_Module__loggerFxn4__C = ((CT__xdc_runtime_Error_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Error_Module__loggerFxn8 xdc_runtime_Error_Module__loggerFxn8__C = ((CT__xdc_runtime_Error_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Error_Module__startupDoneFxn xdc_runtime_Error_Module__startupDoneFxn__C = ((CT__xdc_runtime_Error_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Error_Object__count xdc_runtime_Error_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Error_Object__heap xdc_runtime_Error_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Error_Object__sizeof xdc_runtime_Error_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Error_Object__table xdc_runtime_Error_Object__table__C = 0;

/* E_generic__C */
__FAR__ const CT__xdc_runtime_Error_E_generic xdc_runtime_Error_E_generic__C = (((xdc_runtime_Error_Id)779) << 16 | 0);

/* E_memory__C */
__FAR__ const CT__xdc_runtime_Error_E_memory xdc_runtime_Error_E_memory__C = (((xdc_runtime_Error_Id)783) << 16 | 0);

/* E_msgCode__C */
__FAR__ const CT__xdc_runtime_Error_E_msgCode xdc_runtime_Error_E_msgCode__C = (((xdc_runtime_Error_Id)817) << 16 | 0);

/* policy__C */
__FAR__ const CT__xdc_runtime_Error_policy xdc_runtime_Error_policy__C = xdc_runtime_Error_UNWIND;

/* raiseHook__C */
__FAR__ const CT__xdc_runtime_Error_raiseHook xdc_runtime_Error_raiseHook__C = ((CT__xdc_runtime_Error_raiseHook)((xdc_Fxn)xdc_runtime_Error_print__E));

/* maxDepth__C */
__FAR__ const CT__xdc_runtime_Error_maxDepth xdc_runtime_Error_maxDepth__C = (xdc_UInt16)0x10;


/*
 * ======== xdc.runtime.Gate INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Gate_Module__diagsEnabled xdc_runtime_Gate_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Gate_Module__diagsIncluded xdc_runtime_Gate_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Gate_Module__diagsMask xdc_runtime_Gate_Module__diagsMask__C = ((CT__xdc_runtime_Gate_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Gate_Module__gateObj xdc_runtime_Gate_Module__gateObj__C = ((CT__xdc_runtime_Gate_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Gate_Module__gatePrms xdc_runtime_Gate_Module__gatePrms__C = ((CT__xdc_runtime_Gate_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Gate_Module__id xdc_runtime_Gate_Module__id__C = (xdc_Bits16)0x8007;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Gate_Module__loggerDefined xdc_runtime_Gate_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Gate_Module__loggerObj xdc_runtime_Gate_Module__loggerObj__C = ((CT__xdc_runtime_Gate_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Gate_Module__loggerFxn0 xdc_runtime_Gate_Module__loggerFxn0__C = ((CT__xdc_runtime_Gate_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Gate_Module__loggerFxn1 xdc_runtime_Gate_Module__loggerFxn1__C = ((CT__xdc_runtime_Gate_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Gate_Module__loggerFxn2 xdc_runtime_Gate_Module__loggerFxn2__C = ((CT__xdc_runtime_Gate_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Gate_Module__loggerFxn4 xdc_runtime_Gate_Module__loggerFxn4__C = ((CT__xdc_runtime_Gate_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Gate_Module__loggerFxn8 xdc_runtime_Gate_Module__loggerFxn8__C = ((CT__xdc_runtime_Gate_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Gate_Module__startupDoneFxn xdc_runtime_Gate_Module__startupDoneFxn__C = ((CT__xdc_runtime_Gate_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Gate_Object__count xdc_runtime_Gate_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Gate_Object__heap xdc_runtime_Gate_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Gate_Object__sizeof xdc_runtime_Gate_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Gate_Object__table xdc_runtime_Gate_Object__table__C = 0;


/*
 * ======== xdc.runtime.GateNull INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_GateNull_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_GateNull_Params xdc_runtime_GateNull_Object__PARAMS__C = {
    sizeof (xdc_runtime_GateNull_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_GateNull_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_GateNull_Module__ xdc_runtime_GateNull_Module__root__V = {
    {&xdc_runtime_GateNull_Module__root__V.link,  /* link.next */
    &xdc_runtime_GateNull_Module__root__V.link},  /* link.prev */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__diagsEnabled xdc_runtime_GateNull_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__diagsIncluded xdc_runtime_GateNull_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__diagsMask xdc_runtime_GateNull_Module__diagsMask__C = ((CT__xdc_runtime_GateNull_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__gateObj xdc_runtime_GateNull_Module__gateObj__C = ((CT__xdc_runtime_GateNull_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__gatePrms xdc_runtime_GateNull_Module__gatePrms__C = ((CT__xdc_runtime_GateNull_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__id xdc_runtime_GateNull_Module__id__C = (xdc_Bits16)0x8008;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__loggerDefined xdc_runtime_GateNull_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__loggerObj xdc_runtime_GateNull_Module__loggerObj__C = ((CT__xdc_runtime_GateNull_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__loggerFxn0 xdc_runtime_GateNull_Module__loggerFxn0__C = ((CT__xdc_runtime_GateNull_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__loggerFxn1 xdc_runtime_GateNull_Module__loggerFxn1__C = ((CT__xdc_runtime_GateNull_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__loggerFxn2 xdc_runtime_GateNull_Module__loggerFxn2__C = ((CT__xdc_runtime_GateNull_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__loggerFxn4 xdc_runtime_GateNull_Module__loggerFxn4__C = ((CT__xdc_runtime_GateNull_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__loggerFxn8 xdc_runtime_GateNull_Module__loggerFxn8__C = ((CT__xdc_runtime_GateNull_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_GateNull_Module__startupDoneFxn xdc_runtime_GateNull_Module__startupDoneFxn__C = ((CT__xdc_runtime_GateNull_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_GateNull_Object__count xdc_runtime_GateNull_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_GateNull_Object__heap xdc_runtime_GateNull_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_GateNull_Object__sizeof xdc_runtime_GateNull_Object__sizeof__C = sizeof(xdc_runtime_GateNull_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_GateNull_Object__table xdc_runtime_GateNull_Object__table__C = 0;


/*
 * ======== xdc.runtime.HeapStd INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_HeapStd_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_HeapStd_Params xdc_runtime_HeapStd_Object__PARAMS__C = {
    sizeof (xdc_runtime_HeapStd_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_HeapStd_Object__PARAMS__C.__iprms, /* instance */
    ((xdc_UArg)(0x0)),  /* size */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_HeapStd_Module__ xdc_runtime_HeapStd_Module__root__V = {
    {&xdc_runtime_HeapStd_Module__root__V.link,  /* link.next */
    &xdc_runtime_HeapStd_Module__root__V.link},  /* link.prev */
};

/* Module__state__V */
xdc_runtime_HeapStd_Module_State__ xdc_runtime_HeapStd_Module__state__V = {
    ((xdc_UArg)(0xffffffff)),  /* remainRTSSize */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__diagsEnabled xdc_runtime_HeapStd_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__diagsIncluded xdc_runtime_HeapStd_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__diagsMask xdc_runtime_HeapStd_Module__diagsMask__C = ((CT__xdc_runtime_HeapStd_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__gateObj xdc_runtime_HeapStd_Module__gateObj__C = ((CT__xdc_runtime_HeapStd_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__gatePrms xdc_runtime_HeapStd_Module__gatePrms__C = ((CT__xdc_runtime_HeapStd_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__id xdc_runtime_HeapStd_Module__id__C = (xdc_Bits16)0x800d;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__loggerDefined xdc_runtime_HeapStd_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__loggerObj xdc_runtime_HeapStd_Module__loggerObj__C = ((CT__xdc_runtime_HeapStd_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__loggerFxn0 xdc_runtime_HeapStd_Module__loggerFxn0__C = ((CT__xdc_runtime_HeapStd_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__loggerFxn1 xdc_runtime_HeapStd_Module__loggerFxn1__C = ((CT__xdc_runtime_HeapStd_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__loggerFxn2 xdc_runtime_HeapStd_Module__loggerFxn2__C = ((CT__xdc_runtime_HeapStd_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__loggerFxn4 xdc_runtime_HeapStd_Module__loggerFxn4__C = ((CT__xdc_runtime_HeapStd_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__loggerFxn8 xdc_runtime_HeapStd_Module__loggerFxn8__C = ((CT__xdc_runtime_HeapStd_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_HeapStd_Module__startupDoneFxn xdc_runtime_HeapStd_Module__startupDoneFxn__C = ((CT__xdc_runtime_HeapStd_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_HeapStd_Object__count xdc_runtime_HeapStd_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_HeapStd_Object__heap xdc_runtime_HeapStd_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_HeapStd_Object__sizeof xdc_runtime_HeapStd_Object__sizeof__C = sizeof(xdc_runtime_HeapStd_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_HeapStd_Object__table xdc_runtime_HeapStd_Object__table__C = 0;

/* E_noRTSMemory__C */
__FAR__ const CT__xdc_runtime_HeapStd_E_noRTSMemory xdc_runtime_HeapStd_E_noRTSMemory__C = (((xdc_runtime_Error_Id)902) << 16 | 0);

/* A_zeroSize__C */
__FAR__ const CT__xdc_runtime_HeapStd_A_zeroSize xdc_runtime_HeapStd_A_zeroSize__C = (((xdc_runtime_Assert_Id)95) << 16 | 16);

/* A_invalidTotalFreeSize__C */
__FAR__ const CT__xdc_runtime_HeapStd_A_invalidTotalFreeSize xdc_runtime_HeapStd_A_invalidTotalFreeSize__C = (((xdc_runtime_Assert_Id)140) << 16 | 16);

/* A_invalidAlignment__C */
__FAR__ const CT__xdc_runtime_HeapStd_A_invalidAlignment xdc_runtime_HeapStd_A_invalidAlignment__C = (((xdc_runtime_Assert_Id)201) << 16 | 16);


/*
 * ======== xdc.runtime.Log INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Log_Module__diagsEnabled xdc_runtime_Log_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Log_Module__diagsIncluded xdc_runtime_Log_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Log_Module__diagsMask xdc_runtime_Log_Module__diagsMask__C = ((CT__xdc_runtime_Log_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Log_Module__gateObj xdc_runtime_Log_Module__gateObj__C = ((CT__xdc_runtime_Log_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Log_Module__gatePrms xdc_runtime_Log_Module__gatePrms__C = ((CT__xdc_runtime_Log_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Log_Module__id xdc_runtime_Log_Module__id__C = (xdc_Bits16)0x8009;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Log_Module__loggerDefined xdc_runtime_Log_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Log_Module__loggerObj xdc_runtime_Log_Module__loggerObj__C = ((CT__xdc_runtime_Log_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Log_Module__loggerFxn0 xdc_runtime_Log_Module__loggerFxn0__C = ((CT__xdc_runtime_Log_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Log_Module__loggerFxn1 xdc_runtime_Log_Module__loggerFxn1__C = ((CT__xdc_runtime_Log_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Log_Module__loggerFxn2 xdc_runtime_Log_Module__loggerFxn2__C = ((CT__xdc_runtime_Log_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Log_Module__loggerFxn4 xdc_runtime_Log_Module__loggerFxn4__C = ((CT__xdc_runtime_Log_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Log_Module__loggerFxn8 xdc_runtime_Log_Module__loggerFxn8__C = ((CT__xdc_runtime_Log_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Log_Module__startupDoneFxn xdc_runtime_Log_Module__startupDoneFxn__C = ((CT__xdc_runtime_Log_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Log_Object__count xdc_runtime_Log_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Log_Object__heap xdc_runtime_Log_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Log_Object__sizeof xdc_runtime_Log_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Log_Object__table xdc_runtime_Log_Object__table__C = 0;

/* L_construct__C */
__FAR__ const CT__xdc_runtime_Log_L_construct xdc_runtime_Log_L_construct__C = (((xdc_runtime_Log_Event)1502) << 16 | 4);

/* L_create__C */
__FAR__ const CT__xdc_runtime_Log_L_create xdc_runtime_Log_L_create__C = (((xdc_runtime_Log_Event)1526) << 16 | 4);

/* L_destruct__C */
__FAR__ const CT__xdc_runtime_Log_L_destruct xdc_runtime_Log_L_destruct__C = (((xdc_runtime_Log_Event)1547) << 16 | 4);

/* L_delete__C */
__FAR__ const CT__xdc_runtime_Log_L_delete xdc_runtime_Log_L_delete__C = (((xdc_runtime_Log_Event)1566) << 16 | 4);

/* L_error__C */
__FAR__ const CT__xdc_runtime_Log_L_error xdc_runtime_Log_L_error__C = (((xdc_runtime_Log_Event)1583) << 16 | 192);

/* L_warning__C */
__FAR__ const CT__xdc_runtime_Log_L_warning xdc_runtime_Log_L_warning__C = (((xdc_runtime_Log_Event)1597) << 16 | 224);

/* L_info__C */
__FAR__ const CT__xdc_runtime_Log_L_info xdc_runtime_Log_L_info__C = (((xdc_runtime_Log_Event)1613) << 16 | 16384);

/* L_start__C */
__FAR__ const CT__xdc_runtime_Log_L_start xdc_runtime_Log_L_start__C = (((xdc_runtime_Log_Event)1620) << 16 | 32768);

/* L_stop__C */
__FAR__ const CT__xdc_runtime_Log_L_stop xdc_runtime_Log_L_stop__C = (((xdc_runtime_Log_Event)1631) << 16 | 32768);

/* L_startInstance__C */
__FAR__ const CT__xdc_runtime_Log_L_startInstance xdc_runtime_Log_L_startInstance__C = (((xdc_runtime_Log_Event)1641) << 16 | 32768);

/* L_stopInstance__C */
__FAR__ const CT__xdc_runtime_Log_L_stopInstance xdc_runtime_Log_L_stopInstance__C = (((xdc_runtime_Log_Event)1660) << 16 | 32768);


/*
 * ======== xdc.runtime.LoggerSys INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_LoggerSys_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_LoggerSys_Params xdc_runtime_LoggerSys_Object__PARAMS__C = {
    sizeof (xdc_runtime_LoggerSys_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_LoggerSys_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_LoggerSys_Module__ xdc_runtime_LoggerSys_Module__root__V = {
    {&xdc_runtime_LoggerSys_Module__root__V.link,  /* link.next */
    &xdc_runtime_LoggerSys_Module__root__V.link},  /* link.prev */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__diagsEnabled xdc_runtime_LoggerSys_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__diagsIncluded xdc_runtime_LoggerSys_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__diagsMask xdc_runtime_LoggerSys_Module__diagsMask__C = ((CT__xdc_runtime_LoggerSys_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__gateObj xdc_runtime_LoggerSys_Module__gateObj__C = ((CT__xdc_runtime_LoggerSys_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__gatePrms xdc_runtime_LoggerSys_Module__gatePrms__C = ((CT__xdc_runtime_LoggerSys_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__id xdc_runtime_LoggerSys_Module__id__C = (xdc_Bits16)0x800a;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__loggerDefined xdc_runtime_LoggerSys_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__loggerObj xdc_runtime_LoggerSys_Module__loggerObj__C = ((CT__xdc_runtime_LoggerSys_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__loggerFxn0 xdc_runtime_LoggerSys_Module__loggerFxn0__C = ((CT__xdc_runtime_LoggerSys_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__loggerFxn1 xdc_runtime_LoggerSys_Module__loggerFxn1__C = ((CT__xdc_runtime_LoggerSys_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__loggerFxn2 xdc_runtime_LoggerSys_Module__loggerFxn2__C = ((CT__xdc_runtime_LoggerSys_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__loggerFxn4 xdc_runtime_LoggerSys_Module__loggerFxn4__C = ((CT__xdc_runtime_LoggerSys_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__loggerFxn8 xdc_runtime_LoggerSys_Module__loggerFxn8__C = ((CT__xdc_runtime_LoggerSys_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Module__startupDoneFxn xdc_runtime_LoggerSys_Module__startupDoneFxn__C = ((CT__xdc_runtime_LoggerSys_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Object__count xdc_runtime_LoggerSys_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Object__heap xdc_runtime_LoggerSys_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Object__sizeof xdc_runtime_LoggerSys_Object__sizeof__C = sizeof(xdc_runtime_LoggerSys_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_LoggerSys_Object__table xdc_runtime_LoggerSys_Object__table__C = 0;


/*
 * ======== xdc.runtime.LoggerSys_TimestampProxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.Main INITIALIZERS ========
 */

/* Module__root__V */
xdc_runtime_Main_Module__ xdc_runtime_Main_Module__root__V = {
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Main_Module__diagsEnabled xdc_runtime_Main_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Main_Module__diagsIncluded xdc_runtime_Main_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Main_Module__diagsMask xdc_runtime_Main_Module__diagsMask__C = ((CT__xdc_runtime_Main_Module__diagsMask)((void*)&xdc_runtime_Main_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Main_Module__gateObj xdc_runtime_Main_Module__gateObj__C = ((CT__xdc_runtime_Main_Module__gateObj)((void*)(xdc_runtime_IGateProvider_Handle)&ti_sdo_xdcruntime_linux_GateThreadSupport_Object__table__V[0]));

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Main_Module__gatePrms xdc_runtime_Main_Module__gatePrms__C = ((CT__xdc_runtime_Main_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Main_Module__id xdc_runtime_Main_Module__id__C = (xdc_Bits16)0x800b;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Main_Module__loggerDefined xdc_runtime_Main_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Main_Module__loggerObj xdc_runtime_Main_Module__loggerObj__C = ((CT__xdc_runtime_Main_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Main_Module__loggerFxn0 xdc_runtime_Main_Module__loggerFxn0__C = ((CT__xdc_runtime_Main_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Main_Module__loggerFxn1 xdc_runtime_Main_Module__loggerFxn1__C = ((CT__xdc_runtime_Main_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Main_Module__loggerFxn2 xdc_runtime_Main_Module__loggerFxn2__C = ((CT__xdc_runtime_Main_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Main_Module__loggerFxn4 xdc_runtime_Main_Module__loggerFxn4__C = ((CT__xdc_runtime_Main_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Main_Module__loggerFxn8 xdc_runtime_Main_Module__loggerFxn8__C = ((CT__xdc_runtime_Main_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Main_Module__startupDoneFxn xdc_runtime_Main_Module__startupDoneFxn__C = ((CT__xdc_runtime_Main_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Main_Object__count xdc_runtime_Main_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Main_Object__heap xdc_runtime_Main_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Main_Object__sizeof xdc_runtime_Main_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Main_Object__table xdc_runtime_Main_Object__table__C = 0;


/*
 * ======== xdc.runtime.Main_Module_GateProxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.Memory INITIALIZERS ========
 */

/* Module__state__V */
xdc_runtime_Memory_Module_State__ xdc_runtime_Memory_Module__state__V = {
    (xdc_SizeT)0x4,  /* maxDefaultTypeAlign */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Memory_Module__diagsEnabled xdc_runtime_Memory_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Memory_Module__diagsIncluded xdc_runtime_Memory_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Memory_Module__diagsMask xdc_runtime_Memory_Module__diagsMask__C = ((CT__xdc_runtime_Memory_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Memory_Module__gateObj xdc_runtime_Memory_Module__gateObj__C = ((CT__xdc_runtime_Memory_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Memory_Module__gatePrms xdc_runtime_Memory_Module__gatePrms__C = ((CT__xdc_runtime_Memory_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Memory_Module__id xdc_runtime_Memory_Module__id__C = (xdc_Bits16)0x800c;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Memory_Module__loggerDefined xdc_runtime_Memory_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Memory_Module__loggerObj xdc_runtime_Memory_Module__loggerObj__C = ((CT__xdc_runtime_Memory_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Memory_Module__loggerFxn0 xdc_runtime_Memory_Module__loggerFxn0__C = ((CT__xdc_runtime_Memory_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Memory_Module__loggerFxn1 xdc_runtime_Memory_Module__loggerFxn1__C = ((CT__xdc_runtime_Memory_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Memory_Module__loggerFxn2 xdc_runtime_Memory_Module__loggerFxn2__C = ((CT__xdc_runtime_Memory_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Memory_Module__loggerFxn4 xdc_runtime_Memory_Module__loggerFxn4__C = ((CT__xdc_runtime_Memory_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Memory_Module__loggerFxn8 xdc_runtime_Memory_Module__loggerFxn8__C = ((CT__xdc_runtime_Memory_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Memory_Module__startupDoneFxn xdc_runtime_Memory_Module__startupDoneFxn__C = ((CT__xdc_runtime_Memory_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Memory_Object__count xdc_runtime_Memory_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Memory_Object__heap xdc_runtime_Memory_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Memory_Object__sizeof xdc_runtime_Memory_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Memory_Object__table xdc_runtime_Memory_Object__table__C = 0;

/* defaultHeapInstance__C */
__FAR__ const CT__xdc_runtime_Memory_defaultHeapInstance xdc_runtime_Memory_defaultHeapInstance__C = (xdc_runtime_IHeap_Handle)&xdcruntime_heaps_HeapStdAlign_Object__table__V[0];


/*
 * ======== xdc.runtime.Memory_HeapProxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.Registry INITIALIZERS ========
 */

/* Module__root__V */
xdc_runtime_Registry_Module__ xdc_runtime_Registry_Module__root__V = {
    0,  /* mask */
};

/* Module__state__V */
xdc_runtime_Registry_Module_State__ xdc_runtime_Registry_Module__state__V = {
    ((xdc_runtime_Types_RegDesc*)0),  /* listHead */
    (xdc_Bits16)0x7fff,  /* curId */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Registry_Module__diagsEnabled xdc_runtime_Registry_Module__diagsEnabled__C = (xdc_Bits32)0x0;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Registry_Module__diagsIncluded xdc_runtime_Registry_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Registry_Module__diagsMask xdc_runtime_Registry_Module__diagsMask__C = ((CT__xdc_runtime_Registry_Module__diagsMask)((void*)&xdc_runtime_Registry_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Registry_Module__gateObj xdc_runtime_Registry_Module__gateObj__C = ((CT__xdc_runtime_Registry_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Registry_Module__gatePrms xdc_runtime_Registry_Module__gatePrms__C = ((CT__xdc_runtime_Registry_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Registry_Module__id xdc_runtime_Registry_Module__id__C = (xdc_Bits16)0x800e;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Registry_Module__loggerDefined xdc_runtime_Registry_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Registry_Module__loggerObj xdc_runtime_Registry_Module__loggerObj__C = ((CT__xdc_runtime_Registry_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Registry_Module__loggerFxn0 xdc_runtime_Registry_Module__loggerFxn0__C = ((CT__xdc_runtime_Registry_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Registry_Module__loggerFxn1 xdc_runtime_Registry_Module__loggerFxn1__C = ((CT__xdc_runtime_Registry_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Registry_Module__loggerFxn2 xdc_runtime_Registry_Module__loggerFxn2__C = ((CT__xdc_runtime_Registry_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Registry_Module__loggerFxn4 xdc_runtime_Registry_Module__loggerFxn4__C = ((CT__xdc_runtime_Registry_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Registry_Module__loggerFxn8 xdc_runtime_Registry_Module__loggerFxn8__C = ((CT__xdc_runtime_Registry_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Registry_Module__startupDoneFxn xdc_runtime_Registry_Module__startupDoneFxn__C = ((CT__xdc_runtime_Registry_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Registry_Object__count xdc_runtime_Registry_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Registry_Object__heap xdc_runtime_Registry_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Registry_Object__sizeof xdc_runtime_Registry_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Registry_Object__table xdc_runtime_Registry_Object__table__C = 0;


/*
 * ======== xdc.runtime.Rta INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Rta_Module__diagsEnabled xdc_runtime_Rta_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Rta_Module__diagsIncluded xdc_runtime_Rta_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Rta_Module__diagsMask xdc_runtime_Rta_Module__diagsMask__C = ((CT__xdc_runtime_Rta_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Rta_Module__gateObj xdc_runtime_Rta_Module__gateObj__C = ((CT__xdc_runtime_Rta_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Rta_Module__gatePrms xdc_runtime_Rta_Module__gatePrms__C = ((CT__xdc_runtime_Rta_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Rta_Module__id xdc_runtime_Rta_Module__id__C = (xdc_Bits16)0x800f;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Rta_Module__loggerDefined xdc_runtime_Rta_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Rta_Module__loggerObj xdc_runtime_Rta_Module__loggerObj__C = ((CT__xdc_runtime_Rta_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Rta_Module__loggerFxn0 xdc_runtime_Rta_Module__loggerFxn0__C = ((CT__xdc_runtime_Rta_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Rta_Module__loggerFxn1 xdc_runtime_Rta_Module__loggerFxn1__C = ((CT__xdc_runtime_Rta_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Rta_Module__loggerFxn2 xdc_runtime_Rta_Module__loggerFxn2__C = ((CT__xdc_runtime_Rta_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Rta_Module__loggerFxn4 xdc_runtime_Rta_Module__loggerFxn4__C = ((CT__xdc_runtime_Rta_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Rta_Module__loggerFxn8 xdc_runtime_Rta_Module__loggerFxn8__C = ((CT__xdc_runtime_Rta_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Rta_Module__startupDoneFxn xdc_runtime_Rta_Module__startupDoneFxn__C = ((CT__xdc_runtime_Rta_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Rta_Object__count xdc_runtime_Rta_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Rta_Object__heap xdc_runtime_Rta_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Rta_Object__sizeof xdc_runtime_Rta_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Rta_Object__table xdc_runtime_Rta_Object__table__C = 0;

/* LD_cmdRcvd__C */
__FAR__ const CT__xdc_runtime_Rta_LD_cmdRcvd xdc_runtime_Rta_LD_cmdRcvd__C = (((xdc_runtime_Log_Event)2076) << 16 | 512);

/* LD_writeMask__C */
__FAR__ const CT__xdc_runtime_Rta_LD_writeMask xdc_runtime_Rta_LD_writeMask__C = (((xdc_runtime_Log_Event)2133) << 16 | 512);

/* A_invalidLogger__C */
__FAR__ const CT__xdc_runtime_Rta_A_invalidLogger xdc_runtime_Rta_A_invalidLogger__C = (((xdc_runtime_Assert_Id)261) << 16 | 16);

/* E_badCommand__C */
__FAR__ const CT__xdc_runtime_Rta_E_badCommand xdc_runtime_Rta_E_badCommand__C = (((xdc_runtime_Error_Id)949) << 16 | 0);

/* dataTransportClassName__C */
__FAR__ const CT__xdc_runtime_Rta_dataTransportClassName xdc_runtime_Rta_dataTransportClassName__C = "";

/* controlTransportClassName__C */
__FAR__ const CT__xdc_runtime_Rta_controlTransportClassName xdc_runtime_Rta_controlTransportClassName__C = "";


/*
 * ======== xdc.runtime.Startup INITIALIZERS ========
 */

/* Module__state__V */
xdc_runtime_Startup_Module_State__ xdc_runtime_Startup_Module__state__V = {
    ((xdc_Int*)0),  /* stateTab */
    0,  /* execFlag */
    0,  /* rtsDoneFlag */
};

/* --> xdc_runtime_Startup_lastFxns__A */
const __T1_xdc_runtime_Startup_lastFxns xdc_runtime_Startup_lastFxns__A[1] = {
    ((xdc_Void(*)(xdc_Void))((xdc_Fxn)xdc_runtime_System_lastFxn__I)),  /* [0] */
};

/* --> xdc_runtime_Startup_sfxnTab__A */
const __T1_xdc_runtime_Startup_sfxnTab xdc_runtime_Startup_sfxnTab__A[7] = {
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)xdc_runtime_System_Module_startup__E)),  /* [0] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sdo_xdcruntime_linux_GateThreadSupport_Module_startup__E)),  /* [1] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sdo_xdcruntime_linux_GateProcessSupport_Module_startup__E)),  /* [2] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sdo_xdcruntime_linux_SemThreadSupport_Module_startup__E)),  /* [3] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sdo_xdcruntime_linux_SemProcessSupport_Module_startup__E)),  /* [4] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)ti_sdo_xdcruntime_linux_ThreadSupport_Module_startup__E)),  /* [5] */
    ((xdc_Int(*)(xdc_Int))((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_Module_startup__E)),  /* [6] */
};

/* --> xdc_runtime_Startup_sfxnRts__A */
const __T1_xdc_runtime_Startup_sfxnRts xdc_runtime_Startup_sfxnRts__A[7] = {
    1,  /* [0] */
    1,  /* [1] */
    1,  /* [2] */
    0,  /* [3] */
    0,  /* [4] */
    0,  /* [5] */
    1,  /* [6] */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Startup_Module__diagsEnabled xdc_runtime_Startup_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Startup_Module__diagsIncluded xdc_runtime_Startup_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Startup_Module__diagsMask xdc_runtime_Startup_Module__diagsMask__C = ((CT__xdc_runtime_Startup_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Startup_Module__gateObj xdc_runtime_Startup_Module__gateObj__C = ((CT__xdc_runtime_Startup_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Startup_Module__gatePrms xdc_runtime_Startup_Module__gatePrms__C = ((CT__xdc_runtime_Startup_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Startup_Module__id xdc_runtime_Startup_Module__id__C = (xdc_Bits16)0x8010;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Startup_Module__loggerDefined xdc_runtime_Startup_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Startup_Module__loggerObj xdc_runtime_Startup_Module__loggerObj__C = ((CT__xdc_runtime_Startup_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Startup_Module__loggerFxn0 xdc_runtime_Startup_Module__loggerFxn0__C = ((CT__xdc_runtime_Startup_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Startup_Module__loggerFxn1 xdc_runtime_Startup_Module__loggerFxn1__C = ((CT__xdc_runtime_Startup_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Startup_Module__loggerFxn2 xdc_runtime_Startup_Module__loggerFxn2__C = ((CT__xdc_runtime_Startup_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Startup_Module__loggerFxn4 xdc_runtime_Startup_Module__loggerFxn4__C = ((CT__xdc_runtime_Startup_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Startup_Module__loggerFxn8 xdc_runtime_Startup_Module__loggerFxn8__C = ((CT__xdc_runtime_Startup_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Startup_Module__startupDoneFxn xdc_runtime_Startup_Module__startupDoneFxn__C = ((CT__xdc_runtime_Startup_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Startup_Object__count xdc_runtime_Startup_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Startup_Object__heap xdc_runtime_Startup_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Startup_Object__sizeof xdc_runtime_Startup_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Startup_Object__table xdc_runtime_Startup_Object__table__C = 0;

/* maxPasses__C */
__FAR__ const CT__xdc_runtime_Startup_maxPasses xdc_runtime_Startup_maxPasses__C = (xdc_Int)0x20;

/* firstFxns__C */
__FAR__ const CT__xdc_runtime_Startup_firstFxns xdc_runtime_Startup_firstFxns__C = {0, 0};

/* lastFxns__C */
__FAR__ const CT__xdc_runtime_Startup_lastFxns xdc_runtime_Startup_lastFxns__C = {1, ((__T1_xdc_runtime_Startup_lastFxns*)xdc_runtime_Startup_lastFxns__A)};

/* startModsFxn__C */
__FAR__ const CT__xdc_runtime_Startup_startModsFxn xdc_runtime_Startup_startModsFxn__C = ((CT__xdc_runtime_Startup_startModsFxn)((xdc_Fxn)xdc_runtime_Startup_startMods__I));

/* execImpl__C */
__FAR__ const CT__xdc_runtime_Startup_execImpl xdc_runtime_Startup_execImpl__C = ((CT__xdc_runtime_Startup_execImpl)((xdc_Fxn)xdc_runtime_Startup_exec__I));

/* sfxnTab__C */
__FAR__ const CT__xdc_runtime_Startup_sfxnTab xdc_runtime_Startup_sfxnTab__C = ((CT__xdc_runtime_Startup_sfxnTab)xdc_runtime_Startup_sfxnTab__A);

/* sfxnRts__C */
__FAR__ const CT__xdc_runtime_Startup_sfxnRts xdc_runtime_Startup_sfxnRts__C = ((CT__xdc_runtime_Startup_sfxnRts)xdc_runtime_Startup_sfxnRts__A);


/*
 * ======== xdc.runtime.SysStd INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__diagsEnabled xdc_runtime_SysStd_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__diagsIncluded xdc_runtime_SysStd_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__diagsMask xdc_runtime_SysStd_Module__diagsMask__C = ((CT__xdc_runtime_SysStd_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__gateObj xdc_runtime_SysStd_Module__gateObj__C = ((CT__xdc_runtime_SysStd_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__gatePrms xdc_runtime_SysStd_Module__gatePrms__C = ((CT__xdc_runtime_SysStd_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__id xdc_runtime_SysStd_Module__id__C = (xdc_Bits16)0x8012;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerDefined xdc_runtime_SysStd_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerObj xdc_runtime_SysStd_Module__loggerObj__C = ((CT__xdc_runtime_SysStd_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerFxn0 xdc_runtime_SysStd_Module__loggerFxn0__C = ((CT__xdc_runtime_SysStd_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerFxn1 xdc_runtime_SysStd_Module__loggerFxn1__C = ((CT__xdc_runtime_SysStd_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerFxn2 xdc_runtime_SysStd_Module__loggerFxn2__C = ((CT__xdc_runtime_SysStd_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerFxn4 xdc_runtime_SysStd_Module__loggerFxn4__C = ((CT__xdc_runtime_SysStd_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__loggerFxn8 xdc_runtime_SysStd_Module__loggerFxn8__C = ((CT__xdc_runtime_SysStd_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_SysStd_Module__startupDoneFxn xdc_runtime_SysStd_Module__startupDoneFxn__C = ((CT__xdc_runtime_SysStd_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_SysStd_Object__count xdc_runtime_SysStd_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_SysStd_Object__heap xdc_runtime_SysStd_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_SysStd_Object__sizeof xdc_runtime_SysStd_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_SysStd_Object__table xdc_runtime_SysStd_Object__table__C = 0;


/*
 * ======== xdc.runtime.System INITIALIZERS ========
 */

/* --> xdc_runtime_System_Module_State_0_atexitHandlers__A */
__T1_xdc_runtime_System_Module_State__atexitHandlers xdc_runtime_System_Module_State_0_atexitHandlers__A[8] = {
    ((xdc_Void(*)(xdc_Int))0),  /* [0] */
    ((xdc_Void(*)(xdc_Int))0),  /* [1] */
    ((xdc_Void(*)(xdc_Int))0),  /* [2] */
    ((xdc_Void(*)(xdc_Int))0),  /* [3] */
    ((xdc_Void(*)(xdc_Int))0),  /* [4] */
    ((xdc_Void(*)(xdc_Int))0),  /* [5] */
    ((xdc_Void(*)(xdc_Int))0),  /* [6] */
    ((xdc_Void(*)(xdc_Int))0),  /* [7] */
};

/* Module__state__V */
xdc_runtime_System_Module_State__ xdc_runtime_System_Module__state__V = {
    ((void*)xdc_runtime_System_Module_State_0_atexitHandlers__A),  /* atexitHandlers */
    (xdc_Int)0x0,  /* numAtexitHandlers */
    (xdc_Int)0xcafe,  /* exitStatus */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_System_Module__diagsEnabled xdc_runtime_System_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_System_Module__diagsIncluded xdc_runtime_System_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_System_Module__diagsMask xdc_runtime_System_Module__diagsMask__C = ((CT__xdc_runtime_System_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_System_Module__gateObj xdc_runtime_System_Module__gateObj__C = ((CT__xdc_runtime_System_Module__gateObj)((void*)(xdc_runtime_IGateProvider_Handle)&ti_sdo_xdcruntime_linux_GateThreadSupport_Object__table__V[0]));

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_System_Module__gatePrms xdc_runtime_System_Module__gatePrms__C = ((CT__xdc_runtime_System_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_System_Module__id xdc_runtime_System_Module__id__C = (xdc_Bits16)0x8011;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_System_Module__loggerDefined xdc_runtime_System_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_System_Module__loggerObj xdc_runtime_System_Module__loggerObj__C = ((CT__xdc_runtime_System_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_System_Module__loggerFxn0 xdc_runtime_System_Module__loggerFxn0__C = ((CT__xdc_runtime_System_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_System_Module__loggerFxn1 xdc_runtime_System_Module__loggerFxn1__C = ((CT__xdc_runtime_System_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_System_Module__loggerFxn2 xdc_runtime_System_Module__loggerFxn2__C = ((CT__xdc_runtime_System_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_System_Module__loggerFxn4 xdc_runtime_System_Module__loggerFxn4__C = ((CT__xdc_runtime_System_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_System_Module__loggerFxn8 xdc_runtime_System_Module__loggerFxn8__C = ((CT__xdc_runtime_System_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_System_Module__startupDoneFxn xdc_runtime_System_Module__startupDoneFxn__C = ((CT__xdc_runtime_System_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_System_Object__count xdc_runtime_System_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_System_Object__heap xdc_runtime_System_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_System_Object__sizeof xdc_runtime_System_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_System_Object__table xdc_runtime_System_Object__table__C = 0;

/* A_cannotFitIntoArg__C */
__FAR__ const CT__xdc_runtime_System_A_cannotFitIntoArg xdc_runtime_System_A_cannotFitIntoArg__C = (((xdc_runtime_Assert_Id)307) << 16 | 16);

/* maxAtexitHandlers__C */
__FAR__ const CT__xdc_runtime_System_maxAtexitHandlers xdc_runtime_System_maxAtexitHandlers__C = (xdc_Int)0x8;

/* extendFxn__C */
__FAR__ const CT__xdc_runtime_System_extendFxn xdc_runtime_System_extendFxn__C = ((CT__xdc_runtime_System_extendFxn)((xdc_Fxn)xdc_runtime_System_printfExtend__I));


/*
 * ======== xdc.runtime.System_Module_GateProxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.System_SupportProxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.Text INITIALIZERS ========
 */

/* Module__state__V */
xdc_runtime_Text_Module_State__ xdc_runtime_Text_Module__state__V = {
    ((xdc_Ptr)((void*)&xdc_runtime_Text_charTab__A[0])),  /* charBase */
    ((xdc_Ptr)((void*)&xdc_runtime_Text_nodeTab__A[0])),  /* nodeBase */
};

/* --> xdc_runtime_Text_charTab__A */
const __T1_xdc_runtime_Text_charTab xdc_runtime_Text_charTab__A[4184] = {
    (xdc_Char)0x0,  /* [0] */
    (xdc_Char)0x41,  /* [1] */
    (xdc_Char)0x5f,  /* [2] */
    (xdc_Char)0x69,  /* [3] */
    (xdc_Char)0x6e,  /* [4] */
    (xdc_Char)0x69,  /* [5] */
    (xdc_Char)0x74,  /* [6] */
    (xdc_Char)0x69,  /* [7] */
    (xdc_Char)0x61,  /* [8] */
    (xdc_Char)0x6c,  /* [9] */
    (xdc_Char)0x69,  /* [10] */
    (xdc_Char)0x7a,  /* [11] */
    (xdc_Char)0x65,  /* [12] */
    (xdc_Char)0x64,  /* [13] */
    (xdc_Char)0x50,  /* [14] */
    (xdc_Char)0x61,  /* [15] */
    (xdc_Char)0x72,  /* [16] */
    (xdc_Char)0x61,  /* [17] */
    (xdc_Char)0x6d,  /* [18] */
    (xdc_Char)0x73,  /* [19] */
    (xdc_Char)0x3a,  /* [20] */
    (xdc_Char)0x20,  /* [21] */
    (xdc_Char)0x75,  /* [22] */
    (xdc_Char)0x6e,  /* [23] */
    (xdc_Char)0x69,  /* [24] */
    (xdc_Char)0x6e,  /* [25] */
    (xdc_Char)0x69,  /* [26] */
    (xdc_Char)0x74,  /* [27] */
    (xdc_Char)0x69,  /* [28] */
    (xdc_Char)0x61,  /* [29] */
    (xdc_Char)0x6c,  /* [30] */
    (xdc_Char)0x69,  /* [31] */
    (xdc_Char)0x7a,  /* [32] */
    (xdc_Char)0x65,  /* [33] */
    (xdc_Char)0x64,  /* [34] */
    (xdc_Char)0x20,  /* [35] */
    (xdc_Char)0x50,  /* [36] */
    (xdc_Char)0x61,  /* [37] */
    (xdc_Char)0x72,  /* [38] */
    (xdc_Char)0x61,  /* [39] */
    (xdc_Char)0x6d,  /* [40] */
    (xdc_Char)0x73,  /* [41] */
    (xdc_Char)0x20,  /* [42] */
    (xdc_Char)0x73,  /* [43] */
    (xdc_Char)0x74,  /* [44] */
    (xdc_Char)0x72,  /* [45] */
    (xdc_Char)0x75,  /* [46] */
    (xdc_Char)0x63,  /* [47] */
    (xdc_Char)0x74,  /* [48] */
    (xdc_Char)0x0,  /* [49] */
    (xdc_Char)0x48,  /* [50] */
    (xdc_Char)0x65,  /* [51] */
    (xdc_Char)0x61,  /* [52] */
    (xdc_Char)0x70,  /* [53] */
    (xdc_Char)0x4d,  /* [54] */
    (xdc_Char)0x69,  /* [55] */
    (xdc_Char)0x6e,  /* [56] */
    (xdc_Char)0x5f,  /* [57] */
    (xdc_Char)0x63,  /* [58] */
    (xdc_Char)0x72,  /* [59] */
    (xdc_Char)0x65,  /* [60] */
    (xdc_Char)0x61,  /* [61] */
    (xdc_Char)0x74,  /* [62] */
    (xdc_Char)0x65,  /* [63] */
    (xdc_Char)0x20,  /* [64] */
    (xdc_Char)0x63,  /* [65] */
    (xdc_Char)0x61,  /* [66] */
    (xdc_Char)0x6e,  /* [67] */
    (xdc_Char)0x6e,  /* [68] */
    (xdc_Char)0x6f,  /* [69] */
    (xdc_Char)0x74,  /* [70] */
    (xdc_Char)0x20,  /* [71] */
    (xdc_Char)0x68,  /* [72] */
    (xdc_Char)0x61,  /* [73] */
    (xdc_Char)0x76,  /* [74] */
    (xdc_Char)0x65,  /* [75] */
    (xdc_Char)0x20,  /* [76] */
    (xdc_Char)0x61,  /* [77] */
    (xdc_Char)0x20,  /* [78] */
    (xdc_Char)0x7a,  /* [79] */
    (xdc_Char)0x65,  /* [80] */
    (xdc_Char)0x72,  /* [81] */
    (xdc_Char)0x6f,  /* [82] */
    (xdc_Char)0x20,  /* [83] */
    (xdc_Char)0x73,  /* [84] */
    (xdc_Char)0x69,  /* [85] */
    (xdc_Char)0x7a,  /* [86] */
    (xdc_Char)0x65,  /* [87] */
    (xdc_Char)0x20,  /* [88] */
    (xdc_Char)0x76,  /* [89] */
    (xdc_Char)0x61,  /* [90] */
    (xdc_Char)0x6c,  /* [91] */
    (xdc_Char)0x75,  /* [92] */
    (xdc_Char)0x65,  /* [93] */
    (xdc_Char)0x0,  /* [94] */
    (xdc_Char)0x48,  /* [95] */
    (xdc_Char)0x65,  /* [96] */
    (xdc_Char)0x61,  /* [97] */
    (xdc_Char)0x70,  /* [98] */
    (xdc_Char)0x53,  /* [99] */
    (xdc_Char)0x74,  /* [100] */
    (xdc_Char)0x64,  /* [101] */
    (xdc_Char)0x5f,  /* [102] */
    (xdc_Char)0x63,  /* [103] */
    (xdc_Char)0x72,  /* [104] */
    (xdc_Char)0x65,  /* [105] */
    (xdc_Char)0x61,  /* [106] */
    (xdc_Char)0x74,  /* [107] */
    (xdc_Char)0x65,  /* [108] */
    (xdc_Char)0x20,  /* [109] */
    (xdc_Char)0x63,  /* [110] */
    (xdc_Char)0x61,  /* [111] */
    (xdc_Char)0x6e,  /* [112] */
    (xdc_Char)0x6e,  /* [113] */
    (xdc_Char)0x6f,  /* [114] */
    (xdc_Char)0x74,  /* [115] */
    (xdc_Char)0x20,  /* [116] */
    (xdc_Char)0x68,  /* [117] */
    (xdc_Char)0x61,  /* [118] */
    (xdc_Char)0x76,  /* [119] */
    (xdc_Char)0x65,  /* [120] */
    (xdc_Char)0x20,  /* [121] */
    (xdc_Char)0x61,  /* [122] */
    (xdc_Char)0x20,  /* [123] */
    (xdc_Char)0x7a,  /* [124] */
    (xdc_Char)0x65,  /* [125] */
    (xdc_Char)0x72,  /* [126] */
    (xdc_Char)0x6f,  /* [127] */
    (xdc_Char)0x20,  /* [128] */
    (xdc_Char)0x73,  /* [129] */
    (xdc_Char)0x69,  /* [130] */
    (xdc_Char)0x7a,  /* [131] */
    (xdc_Char)0x65,  /* [132] */
    (xdc_Char)0x20,  /* [133] */
    (xdc_Char)0x76,  /* [134] */
    (xdc_Char)0x61,  /* [135] */
    (xdc_Char)0x6c,  /* [136] */
    (xdc_Char)0x75,  /* [137] */
    (xdc_Char)0x65,  /* [138] */
    (xdc_Char)0x0,  /* [139] */
    (xdc_Char)0x48,  /* [140] */
    (xdc_Char)0x65,  /* [141] */
    (xdc_Char)0x61,  /* [142] */
    (xdc_Char)0x70,  /* [143] */
    (xdc_Char)0x53,  /* [144] */
    (xdc_Char)0x74,  /* [145] */
    (xdc_Char)0x64,  /* [146] */
    (xdc_Char)0x20,  /* [147] */
    (xdc_Char)0x69,  /* [148] */
    (xdc_Char)0x6e,  /* [149] */
    (xdc_Char)0x73,  /* [150] */
    (xdc_Char)0x74,  /* [151] */
    (xdc_Char)0x61,  /* [152] */
    (xdc_Char)0x6e,  /* [153] */
    (xdc_Char)0x63,  /* [154] */
    (xdc_Char)0x65,  /* [155] */
    (xdc_Char)0x20,  /* [156] */
    (xdc_Char)0x74,  /* [157] */
    (xdc_Char)0x6f,  /* [158] */
    (xdc_Char)0x74,  /* [159] */
    (xdc_Char)0x61,  /* [160] */
    (xdc_Char)0x6c,  /* [161] */
    (xdc_Char)0x46,  /* [162] */
    (xdc_Char)0x72,  /* [163] */
    (xdc_Char)0x65,  /* [164] */
    (xdc_Char)0x65,  /* [165] */
    (xdc_Char)0x53,  /* [166] */
    (xdc_Char)0x69,  /* [167] */
    (xdc_Char)0x7a,  /* [168] */
    (xdc_Char)0x65,  /* [169] */
    (xdc_Char)0x20,  /* [170] */
    (xdc_Char)0x69,  /* [171] */
    (xdc_Char)0x73,  /* [172] */
    (xdc_Char)0x20,  /* [173] */
    (xdc_Char)0x67,  /* [174] */
    (xdc_Char)0x72,  /* [175] */
    (xdc_Char)0x65,  /* [176] */
    (xdc_Char)0x61,  /* [177] */
    (xdc_Char)0x74,  /* [178] */
    (xdc_Char)0x65,  /* [179] */
    (xdc_Char)0x72,  /* [180] */
    (xdc_Char)0x20,  /* [181] */
    (xdc_Char)0x74,  /* [182] */
    (xdc_Char)0x68,  /* [183] */
    (xdc_Char)0x61,  /* [184] */
    (xdc_Char)0x6e,  /* [185] */
    (xdc_Char)0x20,  /* [186] */
    (xdc_Char)0x73,  /* [187] */
    (xdc_Char)0x74,  /* [188] */
    (xdc_Char)0x61,  /* [189] */
    (xdc_Char)0x72,  /* [190] */
    (xdc_Char)0x74,  /* [191] */
    (xdc_Char)0x69,  /* [192] */
    (xdc_Char)0x6e,  /* [193] */
    (xdc_Char)0x67,  /* [194] */
    (xdc_Char)0x20,  /* [195] */
    (xdc_Char)0x73,  /* [196] */
    (xdc_Char)0x69,  /* [197] */
    (xdc_Char)0x7a,  /* [198] */
    (xdc_Char)0x65,  /* [199] */
    (xdc_Char)0x0,  /* [200] */
    (xdc_Char)0x48,  /* [201] */
    (xdc_Char)0x65,  /* [202] */
    (xdc_Char)0x61,  /* [203] */
    (xdc_Char)0x70,  /* [204] */
    (xdc_Char)0x53,  /* [205] */
    (xdc_Char)0x74,  /* [206] */
    (xdc_Char)0x64,  /* [207] */
    (xdc_Char)0x5f,  /* [208] */
    (xdc_Char)0x61,  /* [209] */
    (xdc_Char)0x6c,  /* [210] */
    (xdc_Char)0x6c,  /* [211] */
    (xdc_Char)0x6f,  /* [212] */
    (xdc_Char)0x63,  /* [213] */
    (xdc_Char)0x20,  /* [214] */
    (xdc_Char)0x2d,  /* [215] */
    (xdc_Char)0x20,  /* [216] */
    (xdc_Char)0x72,  /* [217] */
    (xdc_Char)0x65,  /* [218] */
    (xdc_Char)0x71,  /* [219] */
    (xdc_Char)0x75,  /* [220] */
    (xdc_Char)0x65,  /* [221] */
    (xdc_Char)0x73,  /* [222] */
    (xdc_Char)0x74,  /* [223] */
    (xdc_Char)0x65,  /* [224] */
    (xdc_Char)0x64,  /* [225] */
    (xdc_Char)0x20,  /* [226] */
    (xdc_Char)0x61,  /* [227] */
    (xdc_Char)0x6c,  /* [228] */
    (xdc_Char)0x69,  /* [229] */
    (xdc_Char)0x67,  /* [230] */
    (xdc_Char)0x6e,  /* [231] */
    (xdc_Char)0x6d,  /* [232] */
    (xdc_Char)0x65,  /* [233] */
    (xdc_Char)0x6e,  /* [234] */
    (xdc_Char)0x74,  /* [235] */
    (xdc_Char)0x20,  /* [236] */
    (xdc_Char)0x69,  /* [237] */
    (xdc_Char)0x73,  /* [238] */
    (xdc_Char)0x20,  /* [239] */
    (xdc_Char)0x67,  /* [240] */
    (xdc_Char)0x72,  /* [241] */
    (xdc_Char)0x65,  /* [242] */
    (xdc_Char)0x61,  /* [243] */
    (xdc_Char)0x74,  /* [244] */
    (xdc_Char)0x65,  /* [245] */
    (xdc_Char)0x72,  /* [246] */
    (xdc_Char)0x20,  /* [247] */
    (xdc_Char)0x74,  /* [248] */
    (xdc_Char)0x68,  /* [249] */
    (xdc_Char)0x61,  /* [250] */
    (xdc_Char)0x6e,  /* [251] */
    (xdc_Char)0x20,  /* [252] */
    (xdc_Char)0x61,  /* [253] */
    (xdc_Char)0x6c,  /* [254] */
    (xdc_Char)0x6c,  /* [255] */
    (xdc_Char)0x6f,  /* [256] */
    (xdc_Char)0x77,  /* [257] */
    (xdc_Char)0x65,  /* [258] */
    (xdc_Char)0x64,  /* [259] */
    (xdc_Char)0x0,  /* [260] */
    (xdc_Char)0x41,  /* [261] */
    (xdc_Char)0x5f,  /* [262] */
    (xdc_Char)0x69,  /* [263] */
    (xdc_Char)0x6e,  /* [264] */
    (xdc_Char)0x76,  /* [265] */
    (xdc_Char)0x61,  /* [266] */
    (xdc_Char)0x6c,  /* [267] */
    (xdc_Char)0x69,  /* [268] */
    (xdc_Char)0x64,  /* [269] */
    (xdc_Char)0x4c,  /* [270] */
    (xdc_Char)0x6f,  /* [271] */
    (xdc_Char)0x67,  /* [272] */
    (xdc_Char)0x67,  /* [273] */
    (xdc_Char)0x65,  /* [274] */
    (xdc_Char)0x72,  /* [275] */
    (xdc_Char)0x3a,  /* [276] */
    (xdc_Char)0x20,  /* [277] */
    (xdc_Char)0x54,  /* [278] */
    (xdc_Char)0x68,  /* [279] */
    (xdc_Char)0x65,  /* [280] */
    (xdc_Char)0x20,  /* [281] */
    (xdc_Char)0x6c,  /* [282] */
    (xdc_Char)0x6f,  /* [283] */
    (xdc_Char)0x67,  /* [284] */
    (xdc_Char)0x67,  /* [285] */
    (xdc_Char)0x65,  /* [286] */
    (xdc_Char)0x72,  /* [287] */
    (xdc_Char)0x20,  /* [288] */
    (xdc_Char)0x69,  /* [289] */
    (xdc_Char)0x64,  /* [290] */
    (xdc_Char)0x20,  /* [291] */
    (xdc_Char)0x25,  /* [292] */
    (xdc_Char)0x64,  /* [293] */
    (xdc_Char)0x20,  /* [294] */
    (xdc_Char)0x69,  /* [295] */
    (xdc_Char)0x73,  /* [296] */
    (xdc_Char)0x20,  /* [297] */
    (xdc_Char)0x69,  /* [298] */
    (xdc_Char)0x6e,  /* [299] */
    (xdc_Char)0x76,  /* [300] */
    (xdc_Char)0x61,  /* [301] */
    (xdc_Char)0x6c,  /* [302] */
    (xdc_Char)0x69,  /* [303] */
    (xdc_Char)0x64,  /* [304] */
    (xdc_Char)0x2e,  /* [305] */
    (xdc_Char)0x0,  /* [306] */
    (xdc_Char)0x41,  /* [307] */
    (xdc_Char)0x5f,  /* [308] */
    (xdc_Char)0x63,  /* [309] */
    (xdc_Char)0x61,  /* [310] */
    (xdc_Char)0x6e,  /* [311] */
    (xdc_Char)0x6e,  /* [312] */
    (xdc_Char)0x6f,  /* [313] */
    (xdc_Char)0x74,  /* [314] */
    (xdc_Char)0x46,  /* [315] */
    (xdc_Char)0x69,  /* [316] */
    (xdc_Char)0x74,  /* [317] */
    (xdc_Char)0x49,  /* [318] */
    (xdc_Char)0x6e,  /* [319] */
    (xdc_Char)0x74,  /* [320] */
    (xdc_Char)0x6f,  /* [321] */
    (xdc_Char)0x41,  /* [322] */
    (xdc_Char)0x72,  /* [323] */
    (xdc_Char)0x67,  /* [324] */
    (xdc_Char)0x3a,  /* [325] */
    (xdc_Char)0x20,  /* [326] */
    (xdc_Char)0x73,  /* [327] */
    (xdc_Char)0x69,  /* [328] */
    (xdc_Char)0x7a,  /* [329] */
    (xdc_Char)0x65,  /* [330] */
    (xdc_Char)0x6f,  /* [331] */
    (xdc_Char)0x66,  /* [332] */
    (xdc_Char)0x28,  /* [333] */
    (xdc_Char)0x46,  /* [334] */
    (xdc_Char)0x6c,  /* [335] */
    (xdc_Char)0x6f,  /* [336] */
    (xdc_Char)0x61,  /* [337] */
    (xdc_Char)0x74,  /* [338] */
    (xdc_Char)0x29,  /* [339] */
    (xdc_Char)0x20,  /* [340] */
    (xdc_Char)0x3e,  /* [341] */
    (xdc_Char)0x20,  /* [342] */
    (xdc_Char)0x73,  /* [343] */
    (xdc_Char)0x69,  /* [344] */
    (xdc_Char)0x7a,  /* [345] */
    (xdc_Char)0x65,  /* [346] */
    (xdc_Char)0x6f,  /* [347] */
    (xdc_Char)0x66,  /* [348] */
    (xdc_Char)0x28,  /* [349] */
    (xdc_Char)0x41,  /* [350] */
    (xdc_Char)0x72,  /* [351] */
    (xdc_Char)0x67,  /* [352] */
    (xdc_Char)0x29,  /* [353] */
    (xdc_Char)0x0,  /* [354] */
    (xdc_Char)0x41,  /* [355] */
    (xdc_Char)0x5f,  /* [356] */
    (xdc_Char)0x7a,  /* [357] */
    (xdc_Char)0x65,  /* [358] */
    (xdc_Char)0x72,  /* [359] */
    (xdc_Char)0x6f,  /* [360] */
    (xdc_Char)0x54,  /* [361] */
    (xdc_Char)0x69,  /* [362] */
    (xdc_Char)0x6d,  /* [363] */
    (xdc_Char)0x65,  /* [364] */
    (xdc_Char)0x6f,  /* [365] */
    (xdc_Char)0x75,  /* [366] */
    (xdc_Char)0x74,  /* [367] */
    (xdc_Char)0x3a,  /* [368] */
    (xdc_Char)0x20,  /* [369] */
    (xdc_Char)0x54,  /* [370] */
    (xdc_Char)0x69,  /* [371] */
    (xdc_Char)0x6d,  /* [372] */
    (xdc_Char)0x65,  /* [373] */
    (xdc_Char)0x6f,  /* [374] */
    (xdc_Char)0x75,  /* [375] */
    (xdc_Char)0x74,  /* [376] */
    (xdc_Char)0x20,  /* [377] */
    (xdc_Char)0x76,  /* [378] */
    (xdc_Char)0x61,  /* [379] */
    (xdc_Char)0x6c,  /* [380] */
    (xdc_Char)0x75,  /* [381] */
    (xdc_Char)0x65,  /* [382] */
    (xdc_Char)0x20,  /* [383] */
    (xdc_Char)0x61,  /* [384] */
    (xdc_Char)0x6e,  /* [385] */
    (xdc_Char)0x6e,  /* [386] */
    (xdc_Char)0x6f,  /* [387] */
    (xdc_Char)0x74,  /* [388] */
    (xdc_Char)0x20,  /* [389] */
    (xdc_Char)0x62,  /* [390] */
    (xdc_Char)0x65,  /* [391] */
    (xdc_Char)0x20,  /* [392] */
    (xdc_Char)0x7a,  /* [393] */
    (xdc_Char)0x65,  /* [394] */
    (xdc_Char)0x72,  /* [395] */
    (xdc_Char)0x6f,  /* [396] */
    (xdc_Char)0x0,  /* [397] */
    (xdc_Char)0x41,  /* [398] */
    (xdc_Char)0x5f,  /* [399] */
    (xdc_Char)0x69,  /* [400] */
    (xdc_Char)0x6e,  /* [401] */
    (xdc_Char)0x76,  /* [402] */
    (xdc_Char)0x61,  /* [403] */
    (xdc_Char)0x6c,  /* [404] */
    (xdc_Char)0x69,  /* [405] */
    (xdc_Char)0x64,  /* [406] */
    (xdc_Char)0x4b,  /* [407] */
    (xdc_Char)0x65,  /* [408] */
    (xdc_Char)0x79,  /* [409] */
    (xdc_Char)0x3a,  /* [410] */
    (xdc_Char)0x20,  /* [411] */
    (xdc_Char)0x74,  /* [412] */
    (xdc_Char)0x68,  /* [413] */
    (xdc_Char)0x65,  /* [414] */
    (xdc_Char)0x20,  /* [415] */
    (xdc_Char)0x6b,  /* [416] */
    (xdc_Char)0x65,  /* [417] */
    (xdc_Char)0x79,  /* [418] */
    (xdc_Char)0x20,  /* [419] */
    (xdc_Char)0x6d,  /* [420] */
    (xdc_Char)0x75,  /* [421] */
    (xdc_Char)0x73,  /* [422] */
    (xdc_Char)0x74,  /* [423] */
    (xdc_Char)0x20,  /* [424] */
    (xdc_Char)0x62,  /* [425] */
    (xdc_Char)0x65,  /* [426] */
    (xdc_Char)0x20,  /* [427] */
    (xdc_Char)0x73,  /* [428] */
    (xdc_Char)0x65,  /* [429] */
    (xdc_Char)0x74,  /* [430] */
    (xdc_Char)0x20,  /* [431] */
    (xdc_Char)0x74,  /* [432] */
    (xdc_Char)0x6f,  /* [433] */
    (xdc_Char)0x20,  /* [434] */
    (xdc_Char)0x61,  /* [435] */
    (xdc_Char)0x20,  /* [436] */
    (xdc_Char)0x6e,  /* [437] */
    (xdc_Char)0x6f,  /* [438] */
    (xdc_Char)0x6e,  /* [439] */
    (xdc_Char)0x2d,  /* [440] */
    (xdc_Char)0x64,  /* [441] */
    (xdc_Char)0x65,  /* [442] */
    (xdc_Char)0x66,  /* [443] */
    (xdc_Char)0x61,  /* [444] */
    (xdc_Char)0x75,  /* [445] */
    (xdc_Char)0x6c,  /* [446] */
    (xdc_Char)0x74,  /* [447] */
    (xdc_Char)0x20,  /* [448] */
    (xdc_Char)0x76,  /* [449] */
    (xdc_Char)0x61,  /* [450] */
    (xdc_Char)0x6c,  /* [451] */
    (xdc_Char)0x75,  /* [452] */
    (xdc_Char)0x65,  /* [453] */
    (xdc_Char)0x0,  /* [454] */
    (xdc_Char)0x41,  /* [455] */
    (xdc_Char)0x5f,  /* [456] */
    (xdc_Char)0x50,  /* [457] */
    (xdc_Char)0x4f,  /* [458] */
    (xdc_Char)0x53,  /* [459] */
    (xdc_Char)0x49,  /* [460] */
    (xdc_Char)0x58,  /* [461] */
    (xdc_Char)0x5f,  /* [462] */
    (xdc_Char)0x45,  /* [463] */
    (xdc_Char)0x72,  /* [464] */
    (xdc_Char)0x72,  /* [465] */
    (xdc_Char)0x6f,  /* [466] */
    (xdc_Char)0x72,  /* [467] */
    (xdc_Char)0x3a,  /* [468] */
    (xdc_Char)0x20,  /* [469] */
    (xdc_Char)0x61,  /* [470] */
    (xdc_Char)0x20,  /* [471] */
    (xdc_Char)0x50,  /* [472] */
    (xdc_Char)0x4f,  /* [473] */
    (xdc_Char)0x53,  /* [474] */
    (xdc_Char)0x49,  /* [475] */
    (xdc_Char)0x58,  /* [476] */
    (xdc_Char)0x20,  /* [477] */
    (xdc_Char)0x66,  /* [478] */
    (xdc_Char)0x75,  /* [479] */
    (xdc_Char)0x6e,  /* [480] */
    (xdc_Char)0x63,  /* [481] */
    (xdc_Char)0x74,  /* [482] */
    (xdc_Char)0x69,  /* [483] */
    (xdc_Char)0x6f,  /* [484] */
    (xdc_Char)0x6e,  /* [485] */
    (xdc_Char)0x20,  /* [486] */
    (xdc_Char)0x72,  /* [487] */
    (xdc_Char)0x65,  /* [488] */
    (xdc_Char)0x74,  /* [489] */
    (xdc_Char)0x75,  /* [490] */
    (xdc_Char)0x72,  /* [491] */
    (xdc_Char)0x6e,  /* [492] */
    (xdc_Char)0x65,  /* [493] */
    (xdc_Char)0x64,  /* [494] */
    (xdc_Char)0x20,  /* [495] */
    (xdc_Char)0x66,  /* [496] */
    (xdc_Char)0x61,  /* [497] */
    (xdc_Char)0x69,  /* [498] */
    (xdc_Char)0x6c,  /* [499] */
    (xdc_Char)0x75,  /* [500] */
    (xdc_Char)0x72,  /* [501] */
    (xdc_Char)0x65,  /* [502] */
    (xdc_Char)0x0,  /* [503] */
    (xdc_Char)0x41,  /* [504] */
    (xdc_Char)0x5f,  /* [505] */
    (xdc_Char)0x69,  /* [506] */
    (xdc_Char)0x6e,  /* [507] */
    (xdc_Char)0x76,  /* [508] */
    (xdc_Char)0x61,  /* [509] */
    (xdc_Char)0x6c,  /* [510] */
    (xdc_Char)0x69,  /* [511] */
    (xdc_Char)0x64,  /* [512] */
    (xdc_Char)0x4b,  /* [513] */
    (xdc_Char)0x65,  /* [514] */
    (xdc_Char)0x79,  /* [515] */
    (xdc_Char)0x3a,  /* [516] */
    (xdc_Char)0x20,  /* [517] */
    (xdc_Char)0x70,  /* [518] */
    (xdc_Char)0x61,  /* [519] */
    (xdc_Char)0x72,  /* [520] */
    (xdc_Char)0x61,  /* [521] */
    (xdc_Char)0x6d,  /* [522] */
    (xdc_Char)0x73,  /* [523] */
    (xdc_Char)0x20,  /* [524] */
    (xdc_Char)0x6b,  /* [525] */
    (xdc_Char)0x65,  /* [526] */
    (xdc_Char)0x79,  /* [527] */
    (xdc_Char)0x20,  /* [528] */
    (xdc_Char)0x68,  /* [529] */
    (xdc_Char)0x61,  /* [530] */
    (xdc_Char)0x73,  /* [531] */
    (xdc_Char)0x20,  /* [532] */
    (xdc_Char)0x6e,  /* [533] */
    (xdc_Char)0x6f,  /* [534] */
    (xdc_Char)0x74,  /* [535] */
    (xdc_Char)0x20,  /* [536] */
    (xdc_Char)0x62,  /* [537] */
    (xdc_Char)0x65,  /* [538] */
    (xdc_Char)0x65,  /* [539] */
    (xdc_Char)0x6e,  /* [540] */
    (xdc_Char)0x20,  /* [541] */
    (xdc_Char)0x73,  /* [542] */
    (xdc_Char)0x65,  /* [543] */
    (xdc_Char)0x74,  /* [544] */
    (xdc_Char)0x0,  /* [545] */
    (xdc_Char)0x41,  /* [546] */
    (xdc_Char)0x5f,  /* [547] */
    (xdc_Char)0x75,  /* [548] */
    (xdc_Char)0x6e,  /* [549] */
    (xdc_Char)0x73,  /* [550] */
    (xdc_Char)0x75,  /* [551] */
    (xdc_Char)0x70,  /* [552] */
    (xdc_Char)0x70,  /* [553] */
    (xdc_Char)0x6f,  /* [554] */
    (xdc_Char)0x72,  /* [555] */
    (xdc_Char)0x74,  /* [556] */
    (xdc_Char)0x65,  /* [557] */
    (xdc_Char)0x64,  /* [558] */
    (xdc_Char)0x54,  /* [559] */
    (xdc_Char)0x69,  /* [560] */
    (xdc_Char)0x6d,  /* [561] */
    (xdc_Char)0x65,  /* [562] */
    (xdc_Char)0x6f,  /* [563] */
    (xdc_Char)0x75,  /* [564] */
    (xdc_Char)0x74,  /* [565] */
    (xdc_Char)0x3a,  /* [566] */
    (xdc_Char)0x20,  /* [567] */
    (xdc_Char)0x4f,  /* [568] */
    (xdc_Char)0x6e,  /* [569] */
    (xdc_Char)0x6c,  /* [570] */
    (xdc_Char)0x79,  /* [571] */
    (xdc_Char)0x20,  /* [572] */
    (xdc_Char)0x53,  /* [573] */
    (xdc_Char)0x65,  /* [574] */
    (xdc_Char)0x6d,  /* [575] */
    (xdc_Char)0x50,  /* [576] */
    (xdc_Char)0x72,  /* [577] */
    (xdc_Char)0x6f,  /* [578] */
    (xdc_Char)0x63,  /* [579] */
    (xdc_Char)0x65,  /* [580] */
    (xdc_Char)0x73,  /* [581] */
    (xdc_Char)0x73,  /* [582] */
    (xdc_Char)0x5f,  /* [583] */
    (xdc_Char)0x46,  /* [584] */
    (xdc_Char)0x4f,  /* [585] */
    (xdc_Char)0x52,  /* [586] */
    (xdc_Char)0x45,  /* [587] */
    (xdc_Char)0x56,  /* [588] */
    (xdc_Char)0x45,  /* [589] */
    (xdc_Char)0x52,  /* [590] */
    (xdc_Char)0x20,  /* [591] */
    (xdc_Char)0x69,  /* [592] */
    (xdc_Char)0x73,  /* [593] */
    (xdc_Char)0x20,  /* [594] */
    (xdc_Char)0x73,  /* [595] */
    (xdc_Char)0x75,  /* [596] */
    (xdc_Char)0x70,  /* [597] */
    (xdc_Char)0x70,  /* [598] */
    (xdc_Char)0x6f,  /* [599] */
    (xdc_Char)0x72,  /* [600] */
    (xdc_Char)0x74,  /* [601] */
    (xdc_Char)0x65,  /* [602] */
    (xdc_Char)0x64,  /* [603] */
    (xdc_Char)0x0,  /* [604] */
    (xdc_Char)0x48,  /* [605] */
    (xdc_Char)0x65,  /* [606] */
    (xdc_Char)0x61,  /* [607] */
    (xdc_Char)0x70,  /* [608] */
    (xdc_Char)0x53,  /* [609] */
    (xdc_Char)0x74,  /* [610] */
    (xdc_Char)0x64,  /* [611] */
    (xdc_Char)0x41,  /* [612] */
    (xdc_Char)0x6c,  /* [613] */
    (xdc_Char)0x69,  /* [614] */
    (xdc_Char)0x67,  /* [615] */
    (xdc_Char)0x6e,  /* [616] */
    (xdc_Char)0x5f,  /* [617] */
    (xdc_Char)0x63,  /* [618] */
    (xdc_Char)0x72,  /* [619] */
    (xdc_Char)0x65,  /* [620] */
    (xdc_Char)0x61,  /* [621] */
    (xdc_Char)0x74,  /* [622] */
    (xdc_Char)0x65,  /* [623] */
    (xdc_Char)0x20,  /* [624] */
    (xdc_Char)0x63,  /* [625] */
    (xdc_Char)0x61,  /* [626] */
    (xdc_Char)0x6e,  /* [627] */
    (xdc_Char)0x6e,  /* [628] */
    (xdc_Char)0x6f,  /* [629] */
    (xdc_Char)0x74,  /* [630] */
    (xdc_Char)0x20,  /* [631] */
    (xdc_Char)0x68,  /* [632] */
    (xdc_Char)0x61,  /* [633] */
    (xdc_Char)0x76,  /* [634] */
    (xdc_Char)0x65,  /* [635] */
    (xdc_Char)0x20,  /* [636] */
    (xdc_Char)0x61,  /* [637] */
    (xdc_Char)0x20,  /* [638] */
    (xdc_Char)0x7a,  /* [639] */
    (xdc_Char)0x65,  /* [640] */
    (xdc_Char)0x72,  /* [641] */
    (xdc_Char)0x6f,  /* [642] */
    (xdc_Char)0x20,  /* [643] */
    (xdc_Char)0x73,  /* [644] */
    (xdc_Char)0x69,  /* [645] */
    (xdc_Char)0x7a,  /* [646] */
    (xdc_Char)0x65,  /* [647] */
    (xdc_Char)0x20,  /* [648] */
    (xdc_Char)0x76,  /* [649] */
    (xdc_Char)0x61,  /* [650] */
    (xdc_Char)0x6c,  /* [651] */
    (xdc_Char)0x75,  /* [652] */
    (xdc_Char)0x65,  /* [653] */
    (xdc_Char)0x0,  /* [654] */
    (xdc_Char)0x41,  /* [655] */
    (xdc_Char)0x5f,  /* [656] */
    (xdc_Char)0x61,  /* [657] */
    (xdc_Char)0x6c,  /* [658] */
    (xdc_Char)0x69,  /* [659] */
    (xdc_Char)0x67,  /* [660] */
    (xdc_Char)0x6e,  /* [661] */
    (xdc_Char)0x3a,  /* [662] */
    (xdc_Char)0x20,  /* [663] */
    (xdc_Char)0x52,  /* [664] */
    (xdc_Char)0x65,  /* [665] */
    (xdc_Char)0x71,  /* [666] */
    (xdc_Char)0x75,  /* [667] */
    (xdc_Char)0x65,  /* [668] */
    (xdc_Char)0x73,  /* [669] */
    (xdc_Char)0x74,  /* [670] */
    (xdc_Char)0x65,  /* [671] */
    (xdc_Char)0x64,  /* [672] */
    (xdc_Char)0x20,  /* [673] */
    (xdc_Char)0x61,  /* [674] */
    (xdc_Char)0x6c,  /* [675] */
    (xdc_Char)0x69,  /* [676] */
    (xdc_Char)0x67,  /* [677] */
    (xdc_Char)0x6e,  /* [678] */
    (xdc_Char)0x20,  /* [679] */
    (xdc_Char)0x69,  /* [680] */
    (xdc_Char)0x73,  /* [681] */
    (xdc_Char)0x20,  /* [682] */
    (xdc_Char)0x6e,  /* [683] */
    (xdc_Char)0x6f,  /* [684] */
    (xdc_Char)0x74,  /* [685] */
    (xdc_Char)0x20,  /* [686] */
    (xdc_Char)0x61,  /* [687] */
    (xdc_Char)0x20,  /* [688] */
    (xdc_Char)0x70,  /* [689] */
    (xdc_Char)0x6f,  /* [690] */
    (xdc_Char)0x77,  /* [691] */
    (xdc_Char)0x65,  /* [692] */
    (xdc_Char)0x72,  /* [693] */
    (xdc_Char)0x20,  /* [694] */
    (xdc_Char)0x6f,  /* [695] */
    (xdc_Char)0x66,  /* [696] */
    (xdc_Char)0x20,  /* [697] */
    (xdc_Char)0x32,  /* [698] */
    (xdc_Char)0x0,  /* [699] */
    (xdc_Char)0x48,  /* [700] */
    (xdc_Char)0x65,  /* [701] */
    (xdc_Char)0x61,  /* [702] */
    (xdc_Char)0x70,  /* [703] */
    (xdc_Char)0x53,  /* [704] */
    (xdc_Char)0x74,  /* [705] */
    (xdc_Char)0x64,  /* [706] */
    (xdc_Char)0x41,  /* [707] */
    (xdc_Char)0x6c,  /* [708] */
    (xdc_Char)0x69,  /* [709] */
    (xdc_Char)0x67,  /* [710] */
    (xdc_Char)0x6e,  /* [711] */
    (xdc_Char)0x20,  /* [712] */
    (xdc_Char)0x69,  /* [713] */
    (xdc_Char)0x6e,  /* [714] */
    (xdc_Char)0x73,  /* [715] */
    (xdc_Char)0x74,  /* [716] */
    (xdc_Char)0x61,  /* [717] */
    (xdc_Char)0x6e,  /* [718] */
    (xdc_Char)0x63,  /* [719] */
    (xdc_Char)0x65,  /* [720] */
    (xdc_Char)0x20,  /* [721] */
    (xdc_Char)0x74,  /* [722] */
    (xdc_Char)0x6f,  /* [723] */
    (xdc_Char)0x74,  /* [724] */
    (xdc_Char)0x61,  /* [725] */
    (xdc_Char)0x6c,  /* [726] */
    (xdc_Char)0x46,  /* [727] */
    (xdc_Char)0x72,  /* [728] */
    (xdc_Char)0x65,  /* [729] */
    (xdc_Char)0x65,  /* [730] */
    (xdc_Char)0x53,  /* [731] */
    (xdc_Char)0x69,  /* [732] */
    (xdc_Char)0x7a,  /* [733] */
    (xdc_Char)0x65,  /* [734] */
    (xdc_Char)0x20,  /* [735] */
    (xdc_Char)0x3e,  /* [736] */
    (xdc_Char)0x20,  /* [737] */
    (xdc_Char)0x74,  /* [738] */
    (xdc_Char)0x68,  /* [739] */
    (xdc_Char)0x61,  /* [740] */
    (xdc_Char)0x6e,  /* [741] */
    (xdc_Char)0x20,  /* [742] */
    (xdc_Char)0x73,  /* [743] */
    (xdc_Char)0x74,  /* [744] */
    (xdc_Char)0x61,  /* [745] */
    (xdc_Char)0x72,  /* [746] */
    (xdc_Char)0x74,  /* [747] */
    (xdc_Char)0x69,  /* [748] */
    (xdc_Char)0x6e,  /* [749] */
    (xdc_Char)0x67,  /* [750] */
    (xdc_Char)0x20,  /* [751] */
    (xdc_Char)0x73,  /* [752] */
    (xdc_Char)0x69,  /* [753] */
    (xdc_Char)0x7a,  /* [754] */
    (xdc_Char)0x65,  /* [755] */
    (xdc_Char)0x0,  /* [756] */
    (xdc_Char)0x61,  /* [757] */
    (xdc_Char)0x73,  /* [758] */
    (xdc_Char)0x73,  /* [759] */
    (xdc_Char)0x65,  /* [760] */
    (xdc_Char)0x72,  /* [761] */
    (xdc_Char)0x74,  /* [762] */
    (xdc_Char)0x69,  /* [763] */
    (xdc_Char)0x6f,  /* [764] */
    (xdc_Char)0x6e,  /* [765] */
    (xdc_Char)0x20,  /* [766] */
    (xdc_Char)0x66,  /* [767] */
    (xdc_Char)0x61,  /* [768] */
    (xdc_Char)0x69,  /* [769] */
    (xdc_Char)0x6c,  /* [770] */
    (xdc_Char)0x75,  /* [771] */
    (xdc_Char)0x72,  /* [772] */
    (xdc_Char)0x65,  /* [773] */
    (xdc_Char)0x25,  /* [774] */
    (xdc_Char)0x73,  /* [775] */
    (xdc_Char)0x25,  /* [776] */
    (xdc_Char)0x73,  /* [777] */
    (xdc_Char)0x0,  /* [778] */
    (xdc_Char)0x25,  /* [779] */
    (xdc_Char)0x24,  /* [780] */
    (xdc_Char)0x53,  /* [781] */
    (xdc_Char)0x0,  /* [782] */
    (xdc_Char)0x6f,  /* [783] */
    (xdc_Char)0x75,  /* [784] */
    (xdc_Char)0x74,  /* [785] */
    (xdc_Char)0x20,  /* [786] */
    (xdc_Char)0x6f,  /* [787] */
    (xdc_Char)0x66,  /* [788] */
    (xdc_Char)0x20,  /* [789] */
    (xdc_Char)0x6d,  /* [790] */
    (xdc_Char)0x65,  /* [791] */
    (xdc_Char)0x6d,  /* [792] */
    (xdc_Char)0x6f,  /* [793] */
    (xdc_Char)0x72,  /* [794] */
    (xdc_Char)0x79,  /* [795] */
    (xdc_Char)0x3a,  /* [796] */
    (xdc_Char)0x20,  /* [797] */
    (xdc_Char)0x68,  /* [798] */
    (xdc_Char)0x65,  /* [799] */
    (xdc_Char)0x61,  /* [800] */
    (xdc_Char)0x70,  /* [801] */
    (xdc_Char)0x3d,  /* [802] */
    (xdc_Char)0x30,  /* [803] */
    (xdc_Char)0x78,  /* [804] */
    (xdc_Char)0x25,  /* [805] */
    (xdc_Char)0x78,  /* [806] */
    (xdc_Char)0x2c,  /* [807] */
    (xdc_Char)0x20,  /* [808] */
    (xdc_Char)0x73,  /* [809] */
    (xdc_Char)0x69,  /* [810] */
    (xdc_Char)0x7a,  /* [811] */
    (xdc_Char)0x65,  /* [812] */
    (xdc_Char)0x3d,  /* [813] */
    (xdc_Char)0x25,  /* [814] */
    (xdc_Char)0x75,  /* [815] */
    (xdc_Char)0x0,  /* [816] */
    (xdc_Char)0x25,  /* [817] */
    (xdc_Char)0x73,  /* [818] */
    (xdc_Char)0x20,  /* [819] */
    (xdc_Char)0x30,  /* [820] */
    (xdc_Char)0x78,  /* [821] */
    (xdc_Char)0x25,  /* [822] */
    (xdc_Char)0x78,  /* [823] */
    (xdc_Char)0x0,  /* [824] */
    (xdc_Char)0x45,  /* [825] */
    (xdc_Char)0x5f,  /* [826] */
    (xdc_Char)0x62,  /* [827] */
    (xdc_Char)0x61,  /* [828] */
    (xdc_Char)0x64,  /* [829] */
    (xdc_Char)0x4c,  /* [830] */
    (xdc_Char)0x65,  /* [831] */
    (xdc_Char)0x76,  /* [832] */
    (xdc_Char)0x65,  /* [833] */
    (xdc_Char)0x6c,  /* [834] */
    (xdc_Char)0x3a,  /* [835] */
    (xdc_Char)0x20,  /* [836] */
    (xdc_Char)0x42,  /* [837] */
    (xdc_Char)0x61,  /* [838] */
    (xdc_Char)0x64,  /* [839] */
    (xdc_Char)0x20,  /* [840] */
    (xdc_Char)0x66,  /* [841] */
    (xdc_Char)0x69,  /* [842] */
    (xdc_Char)0x6c,  /* [843] */
    (xdc_Char)0x74,  /* [844] */
    (xdc_Char)0x65,  /* [845] */
    (xdc_Char)0x72,  /* [846] */
    (xdc_Char)0x20,  /* [847] */
    (xdc_Char)0x6c,  /* [848] */
    (xdc_Char)0x65,  /* [849] */
    (xdc_Char)0x76,  /* [850] */
    (xdc_Char)0x65,  /* [851] */
    (xdc_Char)0x6c,  /* [852] */
    (xdc_Char)0x20,  /* [853] */
    (xdc_Char)0x76,  /* [854] */
    (xdc_Char)0x61,  /* [855] */
    (xdc_Char)0x6c,  /* [856] */
    (xdc_Char)0x75,  /* [857] */
    (xdc_Char)0x65,  /* [858] */
    (xdc_Char)0x3a,  /* [859] */
    (xdc_Char)0x20,  /* [860] */
    (xdc_Char)0x25,  /* [861] */
    (xdc_Char)0x64,  /* [862] */
    (xdc_Char)0x0,  /* [863] */
    (xdc_Char)0x66,  /* [864] */
    (xdc_Char)0x72,  /* [865] */
    (xdc_Char)0x65,  /* [866] */
    (xdc_Char)0x65,  /* [867] */
    (xdc_Char)0x28,  /* [868] */
    (xdc_Char)0x29,  /* [869] */
    (xdc_Char)0x20,  /* [870] */
    (xdc_Char)0x69,  /* [871] */
    (xdc_Char)0x6e,  /* [872] */
    (xdc_Char)0x76,  /* [873] */
    (xdc_Char)0x61,  /* [874] */
    (xdc_Char)0x6c,  /* [875] */
    (xdc_Char)0x69,  /* [876] */
    (xdc_Char)0x64,  /* [877] */
    (xdc_Char)0x20,  /* [878] */
    (xdc_Char)0x69,  /* [879] */
    (xdc_Char)0x6e,  /* [880] */
    (xdc_Char)0x20,  /* [881] */
    (xdc_Char)0x67,  /* [882] */
    (xdc_Char)0x72,  /* [883] */
    (xdc_Char)0x6f,  /* [884] */
    (xdc_Char)0x77,  /* [885] */
    (xdc_Char)0x74,  /* [886] */
    (xdc_Char)0x68,  /* [887] */
    (xdc_Char)0x2d,  /* [888] */
    (xdc_Char)0x6f,  /* [889] */
    (xdc_Char)0x6e,  /* [890] */
    (xdc_Char)0x6c,  /* [891] */
    (xdc_Char)0x79,  /* [892] */
    (xdc_Char)0x20,  /* [893] */
    (xdc_Char)0x48,  /* [894] */
    (xdc_Char)0x65,  /* [895] */
    (xdc_Char)0x61,  /* [896] */
    (xdc_Char)0x70,  /* [897] */
    (xdc_Char)0x4d,  /* [898] */
    (xdc_Char)0x69,  /* [899] */
    (xdc_Char)0x6e,  /* [900] */
    (xdc_Char)0x0,  /* [901] */
    (xdc_Char)0x54,  /* [902] */
    (xdc_Char)0x68,  /* [903] */
    (xdc_Char)0x65,  /* [904] */
    (xdc_Char)0x20,  /* [905] */
    (xdc_Char)0x52,  /* [906] */
    (xdc_Char)0x54,  /* [907] */
    (xdc_Char)0x53,  /* [908] */
    (xdc_Char)0x20,  /* [909] */
    (xdc_Char)0x68,  /* [910] */
    (xdc_Char)0x65,  /* [911] */
    (xdc_Char)0x61,  /* [912] */
    (xdc_Char)0x70,  /* [913] */
    (xdc_Char)0x20,  /* [914] */
    (xdc_Char)0x69,  /* [915] */
    (xdc_Char)0x73,  /* [916] */
    (xdc_Char)0x20,  /* [917] */
    (xdc_Char)0x75,  /* [918] */
    (xdc_Char)0x73,  /* [919] */
    (xdc_Char)0x65,  /* [920] */
    (xdc_Char)0x64,  /* [921] */
    (xdc_Char)0x20,  /* [922] */
    (xdc_Char)0x75,  /* [923] */
    (xdc_Char)0x70,  /* [924] */
    (xdc_Char)0x2e,  /* [925] */
    (xdc_Char)0x20,  /* [926] */
    (xdc_Char)0x45,  /* [927] */
    (xdc_Char)0x78,  /* [928] */
    (xdc_Char)0x61,  /* [929] */
    (xdc_Char)0x6d,  /* [930] */
    (xdc_Char)0x69,  /* [931] */
    (xdc_Char)0x6e,  /* [932] */
    (xdc_Char)0x65,  /* [933] */
    (xdc_Char)0x20,  /* [934] */
    (xdc_Char)0x50,  /* [935] */
    (xdc_Char)0x72,  /* [936] */
    (xdc_Char)0x6f,  /* [937] */
    (xdc_Char)0x67,  /* [938] */
    (xdc_Char)0x72,  /* [939] */
    (xdc_Char)0x61,  /* [940] */
    (xdc_Char)0x6d,  /* [941] */
    (xdc_Char)0x2e,  /* [942] */
    (xdc_Char)0x68,  /* [943] */
    (xdc_Char)0x65,  /* [944] */
    (xdc_Char)0x61,  /* [945] */
    (xdc_Char)0x70,  /* [946] */
    (xdc_Char)0x2e,  /* [947] */
    (xdc_Char)0x0,  /* [948] */
    (xdc_Char)0x45,  /* [949] */
    (xdc_Char)0x5f,  /* [950] */
    (xdc_Char)0x62,  /* [951] */
    (xdc_Char)0x61,  /* [952] */
    (xdc_Char)0x64,  /* [953] */
    (xdc_Char)0x43,  /* [954] */
    (xdc_Char)0x6f,  /* [955] */
    (xdc_Char)0x6d,  /* [956] */
    (xdc_Char)0x6d,  /* [957] */
    (xdc_Char)0x61,  /* [958] */
    (xdc_Char)0x6e,  /* [959] */
    (xdc_Char)0x64,  /* [960] */
    (xdc_Char)0x3a,  /* [961] */
    (xdc_Char)0x20,  /* [962] */
    (xdc_Char)0x52,  /* [963] */
    (xdc_Char)0x65,  /* [964] */
    (xdc_Char)0x63,  /* [965] */
    (xdc_Char)0x65,  /* [966] */
    (xdc_Char)0x69,  /* [967] */
    (xdc_Char)0x76,  /* [968] */
    (xdc_Char)0x65,  /* [969] */
    (xdc_Char)0x64,  /* [970] */
    (xdc_Char)0x20,  /* [971] */
    (xdc_Char)0x69,  /* [972] */
    (xdc_Char)0x6e,  /* [973] */
    (xdc_Char)0x76,  /* [974] */
    (xdc_Char)0x61,  /* [975] */
    (xdc_Char)0x6c,  /* [976] */
    (xdc_Char)0x69,  /* [977] */
    (xdc_Char)0x64,  /* [978] */
    (xdc_Char)0x20,  /* [979] */
    (xdc_Char)0x63,  /* [980] */
    (xdc_Char)0x6f,  /* [981] */
    (xdc_Char)0x6d,  /* [982] */
    (xdc_Char)0x6d,  /* [983] */
    (xdc_Char)0x61,  /* [984] */
    (xdc_Char)0x6e,  /* [985] */
    (xdc_Char)0x64,  /* [986] */
    (xdc_Char)0x2c,  /* [987] */
    (xdc_Char)0x20,  /* [988] */
    (xdc_Char)0x69,  /* [989] */
    (xdc_Char)0x64,  /* [990] */
    (xdc_Char)0x3a,  /* [991] */
    (xdc_Char)0x20,  /* [992] */
    (xdc_Char)0x25,  /* [993] */
    (xdc_Char)0x64,  /* [994] */
    (xdc_Char)0x2e,  /* [995] */
    (xdc_Char)0x0,  /* [996] */
    (xdc_Char)0x45,  /* [997] */
    (xdc_Char)0x5f,  /* [998] */
    (xdc_Char)0x50,  /* [999] */
    (xdc_Char)0x4f,  /* [1000] */
    (xdc_Char)0x53,  /* [1001] */
    (xdc_Char)0x49,  /* [1002] */
    (xdc_Char)0x58,  /* [1003] */
    (xdc_Char)0x5f,  /* [1004] */
    (xdc_Char)0x45,  /* [1005] */
    (xdc_Char)0x72,  /* [1006] */
    (xdc_Char)0x72,  /* [1007] */
    (xdc_Char)0x6f,  /* [1008] */
    (xdc_Char)0x72,  /* [1009] */
    (xdc_Char)0x3a,  /* [1010] */
    (xdc_Char)0x20,  /* [1011] */
    (xdc_Char)0x61,  /* [1012] */
    (xdc_Char)0x20,  /* [1013] */
    (xdc_Char)0x50,  /* [1014] */
    (xdc_Char)0x4f,  /* [1015] */
    (xdc_Char)0x53,  /* [1016] */
    (xdc_Char)0x49,  /* [1017] */
    (xdc_Char)0x58,  /* [1018] */
    (xdc_Char)0x20,  /* [1019] */
    (xdc_Char)0x66,  /* [1020] */
    (xdc_Char)0x75,  /* [1021] */
    (xdc_Char)0x6e,  /* [1022] */
    (xdc_Char)0x63,  /* [1023] */
    (xdc_Char)0x74,  /* [1024] */
    (xdc_Char)0x69,  /* [1025] */
    (xdc_Char)0x6f,  /* [1026] */
    (xdc_Char)0x6e,  /* [1027] */
    (xdc_Char)0x20,  /* [1028] */
    (xdc_Char)0x72,  /* [1029] */
    (xdc_Char)0x65,  /* [1030] */
    (xdc_Char)0x74,  /* [1031] */
    (xdc_Char)0x75,  /* [1032] */
    (xdc_Char)0x72,  /* [1033] */
    (xdc_Char)0x6e,  /* [1034] */
    (xdc_Char)0x65,  /* [1035] */
    (xdc_Char)0x64,  /* [1036] */
    (xdc_Char)0x20,  /* [1037] */
    (xdc_Char)0x66,  /* [1038] */
    (xdc_Char)0x61,  /* [1039] */
    (xdc_Char)0x69,  /* [1040] */
    (xdc_Char)0x6c,  /* [1041] */
    (xdc_Char)0x75,  /* [1042] */
    (xdc_Char)0x72,  /* [1043] */
    (xdc_Char)0x65,  /* [1044] */
    (xdc_Char)0x2c,  /* [1045] */
    (xdc_Char)0x20,  /* [1046] */
    (xdc_Char)0x65,  /* [1047] */
    (xdc_Char)0x72,  /* [1048] */
    (xdc_Char)0x72,  /* [1049] */
    (xdc_Char)0x6e,  /* [1050] */
    (xdc_Char)0x6f,  /* [1051] */
    (xdc_Char)0x20,  /* [1052] */
    (xdc_Char)0x69,  /* [1053] */
    (xdc_Char)0x73,  /* [1054] */
    (xdc_Char)0x20,  /* [1055] */
    (xdc_Char)0x25,  /* [1056] */
    (xdc_Char)0x64,  /* [1057] */
    (xdc_Char)0x0,  /* [1058] */
    (xdc_Char)0x45,  /* [1059] */
    (xdc_Char)0x5f,  /* [1060] */
    (xdc_Char)0x72,  /* [1061] */
    (xdc_Char)0x65,  /* [1062] */
    (xdc_Char)0x73,  /* [1063] */
    (xdc_Char)0x65,  /* [1064] */
    (xdc_Char)0x74,  /* [1065] */
    (xdc_Char)0x45,  /* [1066] */
    (xdc_Char)0x72,  /* [1067] */
    (xdc_Char)0x72,  /* [1068] */
    (xdc_Char)0x6f,  /* [1069] */
    (xdc_Char)0x72,  /* [1070] */
    (xdc_Char)0x3a,  /* [1071] */
    (xdc_Char)0x20,  /* [1072] */
    (xdc_Char)0x25,  /* [1073] */
    (xdc_Char)0x73,  /* [1074] */
    (xdc_Char)0x20,  /* [1075] */
    (xdc_Char)0x72,  /* [1076] */
    (xdc_Char)0x65,  /* [1077] */
    (xdc_Char)0x74,  /* [1078] */
    (xdc_Char)0x75,  /* [1079] */
    (xdc_Char)0x72,  /* [1080] */
    (xdc_Char)0x6e,  /* [1081] */
    (xdc_Char)0x65,  /* [1082] */
    (xdc_Char)0x64,  /* [1083] */
    (xdc_Char)0x20,  /* [1084] */
    (xdc_Char)0x66,  /* [1085] */
    (xdc_Char)0x61,  /* [1086] */
    (xdc_Char)0x69,  /* [1087] */
    (xdc_Char)0x6c,  /* [1088] */
    (xdc_Char)0x75,  /* [1089] */
    (xdc_Char)0x72,  /* [1090] */
    (xdc_Char)0x65,  /* [1091] */
    (xdc_Char)0x0,  /* [1092] */
    (xdc_Char)0x45,  /* [1093] */
    (xdc_Char)0x5f,  /* [1094] */
    (xdc_Char)0x70,  /* [1095] */
    (xdc_Char)0x72,  /* [1096] */
    (xdc_Char)0x69,  /* [1097] */
    (xdc_Char)0x6f,  /* [1098] */
    (xdc_Char)0x72,  /* [1099] */
    (xdc_Char)0x69,  /* [1100] */
    (xdc_Char)0x74,  /* [1101] */
    (xdc_Char)0x79,  /* [1102] */
    (xdc_Char)0x3a,  /* [1103] */
    (xdc_Char)0x20,  /* [1104] */
    (xdc_Char)0x54,  /* [1105] */
    (xdc_Char)0x68,  /* [1106] */
    (xdc_Char)0x72,  /* [1107] */
    (xdc_Char)0x65,  /* [1108] */
    (xdc_Char)0x61,  /* [1109] */
    (xdc_Char)0x64,  /* [1110] */
    (xdc_Char)0x20,  /* [1111] */
    (xdc_Char)0x70,  /* [1112] */
    (xdc_Char)0x72,  /* [1113] */
    (xdc_Char)0x69,  /* [1114] */
    (xdc_Char)0x6f,  /* [1115] */
    (xdc_Char)0x72,  /* [1116] */
    (xdc_Char)0x69,  /* [1117] */
    (xdc_Char)0x74,  /* [1118] */
    (xdc_Char)0x79,  /* [1119] */
    (xdc_Char)0x20,  /* [1120] */
    (xdc_Char)0x69,  /* [1121] */
    (xdc_Char)0x73,  /* [1122] */
    (xdc_Char)0x20,  /* [1123] */
    (xdc_Char)0x69,  /* [1124] */
    (xdc_Char)0x6e,  /* [1125] */
    (xdc_Char)0x76,  /* [1126] */
    (xdc_Char)0x61,  /* [1127] */
    (xdc_Char)0x6c,  /* [1128] */
    (xdc_Char)0x69,  /* [1129] */
    (xdc_Char)0x64,  /* [1130] */
    (xdc_Char)0x20,  /* [1131] */
    (xdc_Char)0x25,  /* [1132] */
    (xdc_Char)0x64,  /* [1133] */
    (xdc_Char)0x0,  /* [1134] */
    (xdc_Char)0x45,  /* [1135] */
    (xdc_Char)0x5f,  /* [1136] */
    (xdc_Char)0x61,  /* [1137] */
    (xdc_Char)0x74,  /* [1138] */
    (xdc_Char)0x65,  /* [1139] */
    (xdc_Char)0x78,  /* [1140] */
    (xdc_Char)0x69,  /* [1141] */
    (xdc_Char)0x74,  /* [1142] */
    (xdc_Char)0x3a,  /* [1143] */
    (xdc_Char)0x20,  /* [1144] */
    (xdc_Char)0x43,  /* [1145] */
    (xdc_Char)0x6f,  /* [1146] */
    (xdc_Char)0x75,  /* [1147] */
    (xdc_Char)0x6c,  /* [1148] */
    (xdc_Char)0x64,  /* [1149] */
    (xdc_Char)0x20,  /* [1150] */
    (xdc_Char)0x6e,  /* [1151] */
    (xdc_Char)0x6f,  /* [1152] */
    (xdc_Char)0x74,  /* [1153] */
    (xdc_Char)0x20,  /* [1154] */
    (xdc_Char)0x70,  /* [1155] */
    (xdc_Char)0x6c,  /* [1156] */
    (xdc_Char)0x75,  /* [1157] */
    (xdc_Char)0x67,  /* [1158] */
    (xdc_Char)0x20,  /* [1159] */
    (xdc_Char)0x69,  /* [1160] */
    (xdc_Char)0x6e,  /* [1161] */
    (xdc_Char)0x20,  /* [1162] */
    (xdc_Char)0x61,  /* [1163] */
    (xdc_Char)0x74,  /* [1164] */
    (xdc_Char)0x65,  /* [1165] */
    (xdc_Char)0x78,  /* [1166] */
    (xdc_Char)0x69,  /* [1167] */
    (xdc_Char)0x74,  /* [1168] */
    (xdc_Char)0x20,  /* [1169] */
    (xdc_Char)0x66,  /* [1170] */
    (xdc_Char)0x75,  /* [1171] */
    (xdc_Char)0x6e,  /* [1172] */
    (xdc_Char)0x63,  /* [1173] */
    (xdc_Char)0x74,  /* [1174] */
    (xdc_Char)0x69,  /* [1175] */
    (xdc_Char)0x6f,  /* [1176] */
    (xdc_Char)0x6e,  /* [1177] */
    (xdc_Char)0x20,  /* [1178] */
    (xdc_Char)0x66,  /* [1179] */
    (xdc_Char)0x6f,  /* [1180] */
    (xdc_Char)0x72,  /* [1181] */
    (xdc_Char)0x20,  /* [1182] */
    (xdc_Char)0x73,  /* [1183] */
    (xdc_Char)0x68,  /* [1184] */
    (xdc_Char)0x75,  /* [1185] */
    (xdc_Char)0x74,  /* [1186] */
    (xdc_Char)0x64,  /* [1187] */
    (xdc_Char)0x6f,  /* [1188] */
    (xdc_Char)0x77,  /* [1189] */
    (xdc_Char)0x6e,  /* [1190] */
    (xdc_Char)0x0,  /* [1191] */
    (xdc_Char)0x2d,  /* [1192] */
    (xdc_Char)0x2d,  /* [1193] */
    (xdc_Char)0x3e,  /* [1194] */
    (xdc_Char)0x20,  /* [1195] */
    (xdc_Char)0x63,  /* [1196] */
    (xdc_Char)0x68,  /* [1197] */
    (xdc_Char)0x65,  /* [1198] */
    (xdc_Char)0x63,  /* [1199] */
    (xdc_Char)0x6b,  /* [1200] */
    (xdc_Char)0x3a,  /* [1201] */
    (xdc_Char)0x20,  /* [1202] */
    (xdc_Char)0x28,  /* [1203] */
    (xdc_Char)0x25,  /* [1204] */
    (xdc_Char)0x70,  /* [1205] */
    (xdc_Char)0x29,  /* [1206] */
    (xdc_Char)0x0,  /* [1207] */
    (xdc_Char)0x3c,  /* [1208] */
    (xdc_Char)0x2d,  /* [1209] */
    (xdc_Char)0x2d,  /* [1210] */
    (xdc_Char)0x20,  /* [1211] */
    (xdc_Char)0x63,  /* [1212] */
    (xdc_Char)0x68,  /* [1213] */
    (xdc_Char)0x65,  /* [1214] */
    (xdc_Char)0x63,  /* [1215] */
    (xdc_Char)0x6b,  /* [1216] */
    (xdc_Char)0x3a,  /* [1217] */
    (xdc_Char)0x20,  /* [1218] */
    (xdc_Char)0x25,  /* [1219] */
    (xdc_Char)0x64,  /* [1220] */
    (xdc_Char)0x0,  /* [1221] */
    (xdc_Char)0x2d,  /* [1222] */
    (xdc_Char)0x2d,  /* [1223] */
    (xdc_Char)0x3e,  /* [1224] */
    (xdc_Char)0x20,  /* [1225] */
    (xdc_Char)0x67,  /* [1226] */
    (xdc_Char)0x65,  /* [1227] */
    (xdc_Char)0x74,  /* [1228] */
    (xdc_Char)0x44,  /* [1229] */
    (xdc_Char)0x61,  /* [1230] */
    (xdc_Char)0x74,  /* [1231] */
    (xdc_Char)0x61,  /* [1232] */
    (xdc_Char)0x3a,  /* [1233] */
    (xdc_Char)0x20,  /* [1234] */
    (xdc_Char)0x28,  /* [1235] */
    (xdc_Char)0x25,  /* [1236] */
    (xdc_Char)0x70,  /* [1237] */
    (xdc_Char)0x29,  /* [1238] */
    (xdc_Char)0x0,  /* [1239] */
    (xdc_Char)0x3c,  /* [1240] */
    (xdc_Char)0x2d,  /* [1241] */
    (xdc_Char)0x2d,  /* [1242] */
    (xdc_Char)0x20,  /* [1243] */
    (xdc_Char)0x67,  /* [1244] */
    (xdc_Char)0x65,  /* [1245] */
    (xdc_Char)0x74,  /* [1246] */
    (xdc_Char)0x44,  /* [1247] */
    (xdc_Char)0x61,  /* [1248] */
    (xdc_Char)0x74,  /* [1249] */
    (xdc_Char)0x61,  /* [1250] */
    (xdc_Char)0x3a,  /* [1251] */
    (xdc_Char)0x20,  /* [1252] */
    (xdc_Char)0x25,  /* [1253] */
    (xdc_Char)0x70,  /* [1254] */
    (xdc_Char)0x0,  /* [1255] */
    (xdc_Char)0x2d,  /* [1256] */
    (xdc_Char)0x2d,  /* [1257] */
    (xdc_Char)0x3e,  /* [1258] */
    (xdc_Char)0x20,  /* [1259] */
    (xdc_Char)0x67,  /* [1260] */
    (xdc_Char)0x65,  /* [1261] */
    (xdc_Char)0x74,  /* [1262] */
    (xdc_Char)0x43,  /* [1263] */
    (xdc_Char)0x6f,  /* [1264] */
    (xdc_Char)0x64,  /* [1265] */
    (xdc_Char)0x65,  /* [1266] */
    (xdc_Char)0x3a,  /* [1267] */
    (xdc_Char)0x20,  /* [1268] */
    (xdc_Char)0x28,  /* [1269] */
    (xdc_Char)0x25,  /* [1270] */
    (xdc_Char)0x70,  /* [1271] */
    (xdc_Char)0x29,  /* [1272] */
    (xdc_Char)0x0,  /* [1273] */
    (xdc_Char)0x3c,  /* [1274] */
    (xdc_Char)0x2d,  /* [1275] */
    (xdc_Char)0x2d,  /* [1276] */
    (xdc_Char)0x20,  /* [1277] */
    (xdc_Char)0x67,  /* [1278] */
    (xdc_Char)0x65,  /* [1279] */
    (xdc_Char)0x74,  /* [1280] */
    (xdc_Char)0x43,  /* [1281] */
    (xdc_Char)0x6f,  /* [1282] */
    (xdc_Char)0x64,  /* [1283] */
    (xdc_Char)0x65,  /* [1284] */
    (xdc_Char)0x3a,  /* [1285] */
    (xdc_Char)0x20,  /* [1286] */
    (xdc_Char)0x30,  /* [1287] */
    (xdc_Char)0x78,  /* [1288] */
    (xdc_Char)0x25,  /* [1289] */
    (xdc_Char)0x78,  /* [1290] */
    (xdc_Char)0x0,  /* [1291] */
    (xdc_Char)0x2d,  /* [1292] */
    (xdc_Char)0x2d,  /* [1293] */
    (xdc_Char)0x3e,  /* [1294] */
    (xdc_Char)0x20,  /* [1295] */
    (xdc_Char)0x67,  /* [1296] */
    (xdc_Char)0x65,  /* [1297] */
    (xdc_Char)0x74,  /* [1298] */
    (xdc_Char)0x49,  /* [1299] */
    (xdc_Char)0x64,  /* [1300] */
    (xdc_Char)0x3a,  /* [1301] */
    (xdc_Char)0x20,  /* [1302] */
    (xdc_Char)0x28,  /* [1303] */
    (xdc_Char)0x25,  /* [1304] */
    (xdc_Char)0x70,  /* [1305] */
    (xdc_Char)0x29,  /* [1306] */
    (xdc_Char)0x0,  /* [1307] */
    (xdc_Char)0x3c,  /* [1308] */
    (xdc_Char)0x2d,  /* [1309] */
    (xdc_Char)0x2d,  /* [1310] */
    (xdc_Char)0x20,  /* [1311] */
    (xdc_Char)0x67,  /* [1312] */
    (xdc_Char)0x65,  /* [1313] */
    (xdc_Char)0x74,  /* [1314] */
    (xdc_Char)0x49,  /* [1315] */
    (xdc_Char)0x64,  /* [1316] */
    (xdc_Char)0x3a,  /* [1317] */
    (xdc_Char)0x20,  /* [1318] */
    (xdc_Char)0x76,  /* [1319] */
    (xdc_Char)0x6f,  /* [1320] */
    (xdc_Char)0x69,  /* [1321] */
    (xdc_Char)0x64,  /* [1322] */
    (xdc_Char)0x0,  /* [1323] */
    (xdc_Char)0x2d,  /* [1324] */
    (xdc_Char)0x2d,  /* [1325] */
    (xdc_Char)0x3e,  /* [1326] */
    (xdc_Char)0x20,  /* [1327] */
    (xdc_Char)0x67,  /* [1328] */
    (xdc_Char)0x65,  /* [1329] */
    (xdc_Char)0x74,  /* [1330] */
    (xdc_Char)0x4d,  /* [1331] */
    (xdc_Char)0x73,  /* [1332] */
    (xdc_Char)0x67,  /* [1333] */
    (xdc_Char)0x3a,  /* [1334] */
    (xdc_Char)0x20,  /* [1335] */
    (xdc_Char)0x28,  /* [1336] */
    (xdc_Char)0x25,  /* [1337] */
    (xdc_Char)0x70,  /* [1338] */
    (xdc_Char)0x29,  /* [1339] */
    (xdc_Char)0x0,  /* [1340] */
    (xdc_Char)0x3c,  /* [1341] */
    (xdc_Char)0x2d,  /* [1342] */
    (xdc_Char)0x2d,  /* [1343] */
    (xdc_Char)0x20,  /* [1344] */
    (xdc_Char)0x67,  /* [1345] */
    (xdc_Char)0x65,  /* [1346] */
    (xdc_Char)0x74,  /* [1347] */
    (xdc_Char)0x4d,  /* [1348] */
    (xdc_Char)0x73,  /* [1349] */
    (xdc_Char)0x67,  /* [1350] */
    (xdc_Char)0x3a,  /* [1351] */
    (xdc_Char)0x20,  /* [1352] */
    (xdc_Char)0x22,  /* [1353] */
    (xdc_Char)0x25,  /* [1354] */
    (xdc_Char)0x73,  /* [1355] */
    (xdc_Char)0x22,  /* [1356] */
    (xdc_Char)0x0,  /* [1357] */
    (xdc_Char)0x2d,  /* [1358] */
    (xdc_Char)0x2d,  /* [1359] */
    (xdc_Char)0x3e,  /* [1360] */
    (xdc_Char)0x20,  /* [1361] */
    (xdc_Char)0x67,  /* [1362] */
    (xdc_Char)0x65,  /* [1363] */
    (xdc_Char)0x74,  /* [1364] */
    (xdc_Char)0x53,  /* [1365] */
    (xdc_Char)0x69,  /* [1366] */
    (xdc_Char)0x74,  /* [1367] */
    (xdc_Char)0x65,  /* [1368] */
    (xdc_Char)0x3a,  /* [1369] */
    (xdc_Char)0x20,  /* [1370] */
    (xdc_Char)0x28,  /* [1371] */
    (xdc_Char)0x25,  /* [1372] */
    (xdc_Char)0x70,  /* [1373] */
    (xdc_Char)0x29,  /* [1374] */
    (xdc_Char)0x0,  /* [1375] */
    (xdc_Char)0x3c,  /* [1376] */
    (xdc_Char)0x2d,  /* [1377] */
    (xdc_Char)0x2d,  /* [1378] */
    (xdc_Char)0x20,  /* [1379] */
    (xdc_Char)0x67,  /* [1380] */
    (xdc_Char)0x65,  /* [1381] */
    (xdc_Char)0x74,  /* [1382] */
    (xdc_Char)0x53,  /* [1383] */
    (xdc_Char)0x69,  /* [1384] */
    (xdc_Char)0x74,  /* [1385] */
    (xdc_Char)0x65,  /* [1386] */
    (xdc_Char)0x3a,  /* [1387] */
    (xdc_Char)0x20,  /* [1388] */
    (xdc_Char)0x25,  /* [1389] */
    (xdc_Char)0x70,  /* [1390] */
    (xdc_Char)0x0,  /* [1391] */
    (xdc_Char)0x2d,  /* [1392] */
    (xdc_Char)0x2d,  /* [1393] */
    (xdc_Char)0x3e,  /* [1394] */
    (xdc_Char)0x20,  /* [1395] */
    (xdc_Char)0x69,  /* [1396] */
    (xdc_Char)0x6e,  /* [1397] */
    (xdc_Char)0x69,  /* [1398] */
    (xdc_Char)0x74,  /* [1399] */
    (xdc_Char)0x3a,  /* [1400] */
    (xdc_Char)0x20,  /* [1401] */
    (xdc_Char)0x28,  /* [1402] */
    (xdc_Char)0x25,  /* [1403] */
    (xdc_Char)0x70,  /* [1404] */
    (xdc_Char)0x29,  /* [1405] */
    (xdc_Char)0x0,  /* [1406] */
    (xdc_Char)0x3c,  /* [1407] */
    (xdc_Char)0x2d,  /* [1408] */
    (xdc_Char)0x2d,  /* [1409] */
    (xdc_Char)0x20,  /* [1410] */
    (xdc_Char)0x69,  /* [1411] */
    (xdc_Char)0x6e,  /* [1412] */
    (xdc_Char)0x69,  /* [1413] */
    (xdc_Char)0x74,  /* [1414] */
    (xdc_Char)0x3a,  /* [1415] */
    (xdc_Char)0x20,  /* [1416] */
    (xdc_Char)0x76,  /* [1417] */
    (xdc_Char)0x6f,  /* [1418] */
    (xdc_Char)0x69,  /* [1419] */
    (xdc_Char)0x64,  /* [1420] */
    (xdc_Char)0x0,  /* [1421] */
    (xdc_Char)0x2d,  /* [1422] */
    (xdc_Char)0x2d,  /* [1423] */
    (xdc_Char)0x3e,  /* [1424] */
    (xdc_Char)0x20,  /* [1425] */
    (xdc_Char)0x70,  /* [1426] */
    (xdc_Char)0x72,  /* [1427] */
    (xdc_Char)0x69,  /* [1428] */
    (xdc_Char)0x6e,  /* [1429] */
    (xdc_Char)0x74,  /* [1430] */
    (xdc_Char)0x3a,  /* [1431] */
    (xdc_Char)0x20,  /* [1432] */
    (xdc_Char)0x28,  /* [1433] */
    (xdc_Char)0x25,  /* [1434] */
    (xdc_Char)0x70,  /* [1435] */
    (xdc_Char)0x29,  /* [1436] */
    (xdc_Char)0x0,  /* [1437] */
    (xdc_Char)0x3c,  /* [1438] */
    (xdc_Char)0x2d,  /* [1439] */
    (xdc_Char)0x2d,  /* [1440] */
    (xdc_Char)0x20,  /* [1441] */
    (xdc_Char)0x70,  /* [1442] */
    (xdc_Char)0x72,  /* [1443] */
    (xdc_Char)0x69,  /* [1444] */
    (xdc_Char)0x6e,  /* [1445] */
    (xdc_Char)0x74,  /* [1446] */
    (xdc_Char)0x3a,  /* [1447] */
    (xdc_Char)0x20,  /* [1448] */
    (xdc_Char)0x76,  /* [1449] */
    (xdc_Char)0x6f,  /* [1450] */
    (xdc_Char)0x69,  /* [1451] */
    (xdc_Char)0x64,  /* [1452] */
    (xdc_Char)0x0,  /* [1453] */
    (xdc_Char)0x2d,  /* [1454] */
    (xdc_Char)0x2d,  /* [1455] */
    (xdc_Char)0x3e,  /* [1456] */
    (xdc_Char)0x20,  /* [1457] */
    (xdc_Char)0x72,  /* [1458] */
    (xdc_Char)0x61,  /* [1459] */
    (xdc_Char)0x69,  /* [1460] */
    (xdc_Char)0x73,  /* [1461] */
    (xdc_Char)0x65,  /* [1462] */
    (xdc_Char)0x58,  /* [1463] */
    (xdc_Char)0x3a,  /* [1464] */
    (xdc_Char)0x20,  /* [1465] */
    (xdc_Char)0x28,  /* [1466] */
    (xdc_Char)0x25,  /* [1467] */
    (xdc_Char)0x70,  /* [1468] */
    (xdc_Char)0x2c,  /* [1469] */
    (xdc_Char)0x20,  /* [1470] */
    (xdc_Char)0x25,  /* [1471] */
    (xdc_Char)0x64,  /* [1472] */
    (xdc_Char)0x2c,  /* [1473] */
    (xdc_Char)0x20,  /* [1474] */
    (xdc_Char)0x22,  /* [1475] */
    (xdc_Char)0x25,  /* [1476] */
    (xdc_Char)0x73,  /* [1477] */
    (xdc_Char)0x22,  /* [1478] */
    (xdc_Char)0x2c,  /* [1479] */
    (xdc_Char)0x20,  /* [1480] */
    (xdc_Char)0x25,  /* [1481] */
    (xdc_Char)0x64,  /* [1482] */
    (xdc_Char)0x29,  /* [1483] */
    (xdc_Char)0x0,  /* [1484] */
    (xdc_Char)0x3c,  /* [1485] */
    (xdc_Char)0x2d,  /* [1486] */
    (xdc_Char)0x2d,  /* [1487] */
    (xdc_Char)0x20,  /* [1488] */
    (xdc_Char)0x72,  /* [1489] */
    (xdc_Char)0x61,  /* [1490] */
    (xdc_Char)0x69,  /* [1491] */
    (xdc_Char)0x73,  /* [1492] */
    (xdc_Char)0x65,  /* [1493] */
    (xdc_Char)0x58,  /* [1494] */
    (xdc_Char)0x3a,  /* [1495] */
    (xdc_Char)0x20,  /* [1496] */
    (xdc_Char)0x76,  /* [1497] */
    (xdc_Char)0x6f,  /* [1498] */
    (xdc_Char)0x69,  /* [1499] */
    (xdc_Char)0x64,  /* [1500] */
    (xdc_Char)0x0,  /* [1501] */
    (xdc_Char)0x3c,  /* [1502] */
    (xdc_Char)0x2d,  /* [1503] */
    (xdc_Char)0x2d,  /* [1504] */
    (xdc_Char)0x20,  /* [1505] */
    (xdc_Char)0x63,  /* [1506] */
    (xdc_Char)0x6f,  /* [1507] */
    (xdc_Char)0x6e,  /* [1508] */
    (xdc_Char)0x73,  /* [1509] */
    (xdc_Char)0x74,  /* [1510] */
    (xdc_Char)0x72,  /* [1511] */
    (xdc_Char)0x75,  /* [1512] */
    (xdc_Char)0x63,  /* [1513] */
    (xdc_Char)0x74,  /* [1514] */
    (xdc_Char)0x3a,  /* [1515] */
    (xdc_Char)0x20,  /* [1516] */
    (xdc_Char)0x25,  /* [1517] */
    (xdc_Char)0x70,  /* [1518] */
    (xdc_Char)0x28,  /* [1519] */
    (xdc_Char)0x27,  /* [1520] */
    (xdc_Char)0x25,  /* [1521] */
    (xdc_Char)0x73,  /* [1522] */
    (xdc_Char)0x27,  /* [1523] */
    (xdc_Char)0x29,  /* [1524] */
    (xdc_Char)0x0,  /* [1525] */
    (xdc_Char)0x3c,  /* [1526] */
    (xdc_Char)0x2d,  /* [1527] */
    (xdc_Char)0x2d,  /* [1528] */
    (xdc_Char)0x20,  /* [1529] */
    (xdc_Char)0x63,  /* [1530] */
    (xdc_Char)0x72,  /* [1531] */
    (xdc_Char)0x65,  /* [1532] */
    (xdc_Char)0x61,  /* [1533] */
    (xdc_Char)0x74,  /* [1534] */
    (xdc_Char)0x65,  /* [1535] */
    (xdc_Char)0x3a,  /* [1536] */
    (xdc_Char)0x20,  /* [1537] */
    (xdc_Char)0x25,  /* [1538] */
    (xdc_Char)0x70,  /* [1539] */
    (xdc_Char)0x28,  /* [1540] */
    (xdc_Char)0x27,  /* [1541] */
    (xdc_Char)0x25,  /* [1542] */
    (xdc_Char)0x73,  /* [1543] */
    (xdc_Char)0x27,  /* [1544] */
    (xdc_Char)0x29,  /* [1545] */
    (xdc_Char)0x0,  /* [1546] */
    (xdc_Char)0x2d,  /* [1547] */
    (xdc_Char)0x2d,  /* [1548] */
    (xdc_Char)0x3e,  /* [1549] */
    (xdc_Char)0x20,  /* [1550] */
    (xdc_Char)0x64,  /* [1551] */
    (xdc_Char)0x65,  /* [1552] */
    (xdc_Char)0x73,  /* [1553] */
    (xdc_Char)0x74,  /* [1554] */
    (xdc_Char)0x72,  /* [1555] */
    (xdc_Char)0x75,  /* [1556] */
    (xdc_Char)0x63,  /* [1557] */
    (xdc_Char)0x74,  /* [1558] */
    (xdc_Char)0x3a,  /* [1559] */
    (xdc_Char)0x20,  /* [1560] */
    (xdc_Char)0x28,  /* [1561] */
    (xdc_Char)0x25,  /* [1562] */
    (xdc_Char)0x70,  /* [1563] */
    (xdc_Char)0x29,  /* [1564] */
    (xdc_Char)0x0,  /* [1565] */
    (xdc_Char)0x2d,  /* [1566] */
    (xdc_Char)0x2d,  /* [1567] */
    (xdc_Char)0x3e,  /* [1568] */
    (xdc_Char)0x20,  /* [1569] */
    (xdc_Char)0x64,  /* [1570] */
    (xdc_Char)0x65,  /* [1571] */
    (xdc_Char)0x6c,  /* [1572] */
    (xdc_Char)0x65,  /* [1573] */
    (xdc_Char)0x74,  /* [1574] */
    (xdc_Char)0x65,  /* [1575] */
    (xdc_Char)0x3a,  /* [1576] */
    (xdc_Char)0x20,  /* [1577] */
    (xdc_Char)0x28,  /* [1578] */
    (xdc_Char)0x25,  /* [1579] */
    (xdc_Char)0x70,  /* [1580] */
    (xdc_Char)0x29,  /* [1581] */
    (xdc_Char)0x0,  /* [1582] */
    (xdc_Char)0x45,  /* [1583] */
    (xdc_Char)0x52,  /* [1584] */
    (xdc_Char)0x52,  /* [1585] */
    (xdc_Char)0x4f,  /* [1586] */
    (xdc_Char)0x52,  /* [1587] */
    (xdc_Char)0x3a,  /* [1588] */
    (xdc_Char)0x20,  /* [1589] */
    (xdc_Char)0x25,  /* [1590] */
    (xdc_Char)0x24,  /* [1591] */
    (xdc_Char)0x46,  /* [1592] */
    (xdc_Char)0x25,  /* [1593] */
    (xdc_Char)0x24,  /* [1594] */
    (xdc_Char)0x53,  /* [1595] */
    (xdc_Char)0x0,  /* [1596] */
    (xdc_Char)0x57,  /* [1597] */
    (xdc_Char)0x41,  /* [1598] */
    (xdc_Char)0x52,  /* [1599] */
    (xdc_Char)0x4e,  /* [1600] */
    (xdc_Char)0x49,  /* [1601] */
    (xdc_Char)0x4e,  /* [1602] */
    (xdc_Char)0x47,  /* [1603] */
    (xdc_Char)0x3a,  /* [1604] */
    (xdc_Char)0x20,  /* [1605] */
    (xdc_Char)0x25,  /* [1606] */
    (xdc_Char)0x24,  /* [1607] */
    (xdc_Char)0x46,  /* [1608] */
    (xdc_Char)0x25,  /* [1609] */
    (xdc_Char)0x24,  /* [1610] */
    (xdc_Char)0x53,  /* [1611] */
    (xdc_Char)0x0,  /* [1612] */
    (xdc_Char)0x25,  /* [1613] */
    (xdc_Char)0x24,  /* [1614] */
    (xdc_Char)0x46,  /* [1615] */
    (xdc_Char)0x25,  /* [1616] */
    (xdc_Char)0x24,  /* [1617] */
    (xdc_Char)0x53,  /* [1618] */
    (xdc_Char)0x0,  /* [1619] */
    (xdc_Char)0x53,  /* [1620] */
    (xdc_Char)0x74,  /* [1621] */
    (xdc_Char)0x61,  /* [1622] */
    (xdc_Char)0x72,  /* [1623] */
    (xdc_Char)0x74,  /* [1624] */
    (xdc_Char)0x3a,  /* [1625] */
    (xdc_Char)0x20,  /* [1626] */
    (xdc_Char)0x25,  /* [1627] */
    (xdc_Char)0x24,  /* [1628] */
    (xdc_Char)0x53,  /* [1629] */
    (xdc_Char)0x0,  /* [1630] */
    (xdc_Char)0x53,  /* [1631] */
    (xdc_Char)0x74,  /* [1632] */
    (xdc_Char)0x6f,  /* [1633] */
    (xdc_Char)0x70,  /* [1634] */
    (xdc_Char)0x3a,  /* [1635] */
    (xdc_Char)0x20,  /* [1636] */
    (xdc_Char)0x25,  /* [1637] */
    (xdc_Char)0x24,  /* [1638] */
    (xdc_Char)0x53,  /* [1639] */
    (xdc_Char)0x0,  /* [1640] */
    (xdc_Char)0x53,  /* [1641] */
    (xdc_Char)0x74,  /* [1642] */
    (xdc_Char)0x61,  /* [1643] */
    (xdc_Char)0x72,  /* [1644] */
    (xdc_Char)0x74,  /* [1645] */
    (xdc_Char)0x49,  /* [1646] */
    (xdc_Char)0x6e,  /* [1647] */
    (xdc_Char)0x73,  /* [1648] */
    (xdc_Char)0x74,  /* [1649] */
    (xdc_Char)0x61,  /* [1650] */
    (xdc_Char)0x6e,  /* [1651] */
    (xdc_Char)0x63,  /* [1652] */
    (xdc_Char)0x65,  /* [1653] */
    (xdc_Char)0x3a,  /* [1654] */
    (xdc_Char)0x20,  /* [1655] */
    (xdc_Char)0x25,  /* [1656] */
    (xdc_Char)0x24,  /* [1657] */
    (xdc_Char)0x53,  /* [1658] */
    (xdc_Char)0x0,  /* [1659] */
    (xdc_Char)0x53,  /* [1660] */
    (xdc_Char)0x74,  /* [1661] */
    (xdc_Char)0x6f,  /* [1662] */
    (xdc_Char)0x70,  /* [1663] */
    (xdc_Char)0x49,  /* [1664] */
    (xdc_Char)0x6e,  /* [1665] */
    (xdc_Char)0x73,  /* [1666] */
    (xdc_Char)0x74,  /* [1667] */
    (xdc_Char)0x61,  /* [1668] */
    (xdc_Char)0x6e,  /* [1669] */
    (xdc_Char)0x63,  /* [1670] */
    (xdc_Char)0x65,  /* [1671] */
    (xdc_Char)0x3a,  /* [1672] */
    (xdc_Char)0x20,  /* [1673] */
    (xdc_Char)0x25,  /* [1674] */
    (xdc_Char)0x24,  /* [1675] */
    (xdc_Char)0x53,  /* [1676] */
    (xdc_Char)0x0,  /* [1677] */
    (xdc_Char)0x2d,  /* [1678] */
    (xdc_Char)0x2d,  /* [1679] */
    (xdc_Char)0x3e,  /* [1680] */
    (xdc_Char)0x20,  /* [1681] */
    (xdc_Char)0x61,  /* [1682] */
    (xdc_Char)0x64,  /* [1683] */
    (xdc_Char)0x64,  /* [1684] */
    (xdc_Char)0x4d,  /* [1685] */
    (xdc_Char)0x6f,  /* [1686] */
    (xdc_Char)0x64,  /* [1687] */
    (xdc_Char)0x75,  /* [1688] */
    (xdc_Char)0x6c,  /* [1689] */
    (xdc_Char)0x65,  /* [1690] */
    (xdc_Char)0x3a,  /* [1691] */
    (xdc_Char)0x20,  /* [1692] */
    (xdc_Char)0x28,  /* [1693] */
    (xdc_Char)0x25,  /* [1694] */
    (xdc_Char)0x70,  /* [1695] */
    (xdc_Char)0x2c,  /* [1696] */
    (xdc_Char)0x20,  /* [1697] */
    (xdc_Char)0x22,  /* [1698] */
    (xdc_Char)0x25,  /* [1699] */
    (xdc_Char)0x73,  /* [1700] */
    (xdc_Char)0x22,  /* [1701] */
    (xdc_Char)0x29,  /* [1702] */
    (xdc_Char)0x0,  /* [1703] */
    (xdc_Char)0x3c,  /* [1704] */
    (xdc_Char)0x2d,  /* [1705] */
    (xdc_Char)0x2d,  /* [1706] */
    (xdc_Char)0x20,  /* [1707] */
    (xdc_Char)0x61,  /* [1708] */
    (xdc_Char)0x64,  /* [1709] */
    (xdc_Char)0x64,  /* [1710] */
    (xdc_Char)0x4d,  /* [1711] */
    (xdc_Char)0x6f,  /* [1712] */
    (xdc_Char)0x64,  /* [1713] */
    (xdc_Char)0x75,  /* [1714] */
    (xdc_Char)0x6c,  /* [1715] */
    (xdc_Char)0x65,  /* [1716] */
    (xdc_Char)0x3a,  /* [1717] */
    (xdc_Char)0x20,  /* [1718] */
    (xdc_Char)0x25,  /* [1719] */
    (xdc_Char)0x64,  /* [1720] */
    (xdc_Char)0x0,  /* [1721] */
    (xdc_Char)0x2d,  /* [1722] */
    (xdc_Char)0x2d,  /* [1723] */
    (xdc_Char)0x3e,  /* [1724] */
    (xdc_Char)0x20,  /* [1725] */
    (xdc_Char)0x66,  /* [1726] */
    (xdc_Char)0x69,  /* [1727] */
    (xdc_Char)0x6e,  /* [1728] */
    (xdc_Char)0x64,  /* [1729] */
    (xdc_Char)0x42,  /* [1730] */
    (xdc_Char)0x79,  /* [1731] */
    (xdc_Char)0x4e,  /* [1732] */
    (xdc_Char)0x61,  /* [1733] */
    (xdc_Char)0x6d,  /* [1734] */
    (xdc_Char)0x65,  /* [1735] */
    (xdc_Char)0x3a,  /* [1736] */
    (xdc_Char)0x20,  /* [1737] */
    (xdc_Char)0x28,  /* [1738] */
    (xdc_Char)0x22,  /* [1739] */
    (xdc_Char)0x25,  /* [1740] */
    (xdc_Char)0x73,  /* [1741] */
    (xdc_Char)0x22,  /* [1742] */
    (xdc_Char)0x29,  /* [1743] */
    (xdc_Char)0x0,  /* [1744] */
    (xdc_Char)0x3c,  /* [1745] */
    (xdc_Char)0x2d,  /* [1746] */
    (xdc_Char)0x2d,  /* [1747] */
    (xdc_Char)0x20,  /* [1748] */
    (xdc_Char)0x66,  /* [1749] */
    (xdc_Char)0x69,  /* [1750] */
    (xdc_Char)0x6e,  /* [1751] */
    (xdc_Char)0x64,  /* [1752] */
    (xdc_Char)0x42,  /* [1753] */
    (xdc_Char)0x79,  /* [1754] */
    (xdc_Char)0x4e,  /* [1755] */
    (xdc_Char)0x61,  /* [1756] */
    (xdc_Char)0x6d,  /* [1757] */
    (xdc_Char)0x65,  /* [1758] */
    (xdc_Char)0x3a,  /* [1759] */
    (xdc_Char)0x20,  /* [1760] */
    (xdc_Char)0x25,  /* [1761] */
    (xdc_Char)0x70,  /* [1762] */
    (xdc_Char)0x0,  /* [1763] */
    (xdc_Char)0x2d,  /* [1764] */
    (xdc_Char)0x2d,  /* [1765] */
    (xdc_Char)0x3e,  /* [1766] */
    (xdc_Char)0x20,  /* [1767] */
    (xdc_Char)0x66,  /* [1768] */
    (xdc_Char)0x69,  /* [1769] */
    (xdc_Char)0x6e,  /* [1770] */
    (xdc_Char)0x64,  /* [1771] */
    (xdc_Char)0x42,  /* [1772] */
    (xdc_Char)0x79,  /* [1773] */
    (xdc_Char)0x4e,  /* [1774] */
    (xdc_Char)0x61,  /* [1775] */
    (xdc_Char)0x6d,  /* [1776] */
    (xdc_Char)0x65,  /* [1777] */
    (xdc_Char)0x50,  /* [1778] */
    (xdc_Char)0x61,  /* [1779] */
    (xdc_Char)0x74,  /* [1780] */
    (xdc_Char)0x74,  /* [1781] */
    (xdc_Char)0x65,  /* [1782] */
    (xdc_Char)0x72,  /* [1783] */
    (xdc_Char)0x6e,  /* [1784] */
    (xdc_Char)0x3a,  /* [1785] */
    (xdc_Char)0x20,  /* [1786] */
    (xdc_Char)0x28,  /* [1787] */
    (xdc_Char)0x22,  /* [1788] */
    (xdc_Char)0x25,  /* [1789] */
    (xdc_Char)0x73,  /* [1790] */
    (xdc_Char)0x22,  /* [1791] */
    (xdc_Char)0x2c,  /* [1792] */
    (xdc_Char)0x20,  /* [1793] */
    (xdc_Char)0x25,  /* [1794] */
    (xdc_Char)0x64,  /* [1795] */
    (xdc_Char)0x2c,  /* [1796] */
    (xdc_Char)0x20,  /* [1797] */
    (xdc_Char)0x25,  /* [1798] */
    (xdc_Char)0x70,  /* [1799] */
    (xdc_Char)0x29,  /* [1800] */
    (xdc_Char)0x0,  /* [1801] */
    (xdc_Char)0x3c,  /* [1802] */
    (xdc_Char)0x2d,  /* [1803] */
    (xdc_Char)0x2d,  /* [1804] */
    (xdc_Char)0x20,  /* [1805] */
    (xdc_Char)0x66,  /* [1806] */
    (xdc_Char)0x69,  /* [1807] */
    (xdc_Char)0x6e,  /* [1808] */
    (xdc_Char)0x64,  /* [1809] */
    (xdc_Char)0x42,  /* [1810] */
    (xdc_Char)0x79,  /* [1811] */
    (xdc_Char)0x4e,  /* [1812] */
    (xdc_Char)0x61,  /* [1813] */
    (xdc_Char)0x6d,  /* [1814] */
    (xdc_Char)0x65,  /* [1815] */
    (xdc_Char)0x50,  /* [1816] */
    (xdc_Char)0x61,  /* [1817] */
    (xdc_Char)0x74,  /* [1818] */
    (xdc_Char)0x74,  /* [1819] */
    (xdc_Char)0x65,  /* [1820] */
    (xdc_Char)0x72,  /* [1821] */
    (xdc_Char)0x6e,  /* [1822] */
    (xdc_Char)0x3a,  /* [1823] */
    (xdc_Char)0x20,  /* [1824] */
    (xdc_Char)0x25,  /* [1825] */
    (xdc_Char)0x70,  /* [1826] */
    (xdc_Char)0x0,  /* [1827] */
    (xdc_Char)0x2d,  /* [1828] */
    (xdc_Char)0x2d,  /* [1829] */
    (xdc_Char)0x3e,  /* [1830] */
    (xdc_Char)0x20,  /* [1831] */
    (xdc_Char)0x66,  /* [1832] */
    (xdc_Char)0x69,  /* [1833] */
    (xdc_Char)0x6e,  /* [1834] */
    (xdc_Char)0x64,  /* [1835] */
    (xdc_Char)0x42,  /* [1836] */
    (xdc_Char)0x79,  /* [1837] */
    (xdc_Char)0x49,  /* [1838] */
    (xdc_Char)0x64,  /* [1839] */
    (xdc_Char)0x3a,  /* [1840] */
    (xdc_Char)0x20,  /* [1841] */
    (xdc_Char)0x28,  /* [1842] */
    (xdc_Char)0x25,  /* [1843] */
    (xdc_Char)0x64,  /* [1844] */
    (xdc_Char)0x29,  /* [1845] */
    (xdc_Char)0x0,  /* [1846] */
    (xdc_Char)0x3c,  /* [1847] */
    (xdc_Char)0x2d,  /* [1848] */
    (xdc_Char)0x2d,  /* [1849] */
    (xdc_Char)0x20,  /* [1850] */
    (xdc_Char)0x66,  /* [1851] */
    (xdc_Char)0x69,  /* [1852] */
    (xdc_Char)0x6e,  /* [1853] */
    (xdc_Char)0x64,  /* [1854] */
    (xdc_Char)0x42,  /* [1855] */
    (xdc_Char)0x79,  /* [1856] */
    (xdc_Char)0x49,  /* [1857] */
    (xdc_Char)0x64,  /* [1858] */
    (xdc_Char)0x3a,  /* [1859] */
    (xdc_Char)0x20,  /* [1860] */
    (xdc_Char)0x25,  /* [1861] */
    (xdc_Char)0x70,  /* [1862] */
    (xdc_Char)0x0,  /* [1863] */
    (xdc_Char)0x2d,  /* [1864] */
    (xdc_Char)0x2d,  /* [1865] */
    (xdc_Char)0x3e,  /* [1866] */
    (xdc_Char)0x20,  /* [1867] */
    (xdc_Char)0x67,  /* [1868] */
    (xdc_Char)0x65,  /* [1869] */
    (xdc_Char)0x74,  /* [1870] */
    (xdc_Char)0x4d,  /* [1871] */
    (xdc_Char)0x61,  /* [1872] */
    (xdc_Char)0x73,  /* [1873] */
    (xdc_Char)0x6b,  /* [1874] */
    (xdc_Char)0x3a,  /* [1875] */
    (xdc_Char)0x20,  /* [1876] */
    (xdc_Char)0x28,  /* [1877] */
    (xdc_Char)0x22,  /* [1878] */
    (xdc_Char)0x25,  /* [1879] */
    (xdc_Char)0x73,  /* [1880] */
    (xdc_Char)0x22,  /* [1881] */
    (xdc_Char)0x2c,  /* [1882] */
    (xdc_Char)0x20,  /* [1883] */
    (xdc_Char)0x25,  /* [1884] */
    (xdc_Char)0x70,  /* [1885] */
    (xdc_Char)0x29,  /* [1886] */
    (xdc_Char)0x0,  /* [1887] */
    (xdc_Char)0x3c,  /* [1888] */
    (xdc_Char)0x2d,  /* [1889] */
    (xdc_Char)0x2d,  /* [1890] */
    (xdc_Char)0x20,  /* [1891] */
    (xdc_Char)0x67,  /* [1892] */
    (xdc_Char)0x65,  /* [1893] */
    (xdc_Char)0x74,  /* [1894] */
    (xdc_Char)0x4d,  /* [1895] */
    (xdc_Char)0x61,  /* [1896] */
    (xdc_Char)0x73,  /* [1897] */
    (xdc_Char)0x6b,  /* [1898] */
    (xdc_Char)0x3a,  /* [1899] */
    (xdc_Char)0x20,  /* [1900] */
    (xdc_Char)0x25,  /* [1901] */
    (xdc_Char)0x64,  /* [1902] */
    (xdc_Char)0x0,  /* [1903] */
    (xdc_Char)0x2d,  /* [1904] */
    (xdc_Char)0x2d,  /* [1905] */
    (xdc_Char)0x3e,  /* [1906] */
    (xdc_Char)0x20,  /* [1907] */
    (xdc_Char)0x69,  /* [1908] */
    (xdc_Char)0x73,  /* [1909] */
    (xdc_Char)0x4d,  /* [1910] */
    (xdc_Char)0x65,  /* [1911] */
    (xdc_Char)0x6d,  /* [1912] */
    (xdc_Char)0x62,  /* [1913] */
    (xdc_Char)0x65,  /* [1914] */
    (xdc_Char)0x72,  /* [1915] */
    (xdc_Char)0x3a,  /* [1916] */
    (xdc_Char)0x20,  /* [1917] */
    (xdc_Char)0x28,  /* [1918] */
    (xdc_Char)0x25,  /* [1919] */
    (xdc_Char)0x64,  /* [1920] */
    (xdc_Char)0x29,  /* [1921] */
    (xdc_Char)0x0,  /* [1922] */
    (xdc_Char)0x3c,  /* [1923] */
    (xdc_Char)0x2d,  /* [1924] */
    (xdc_Char)0x2d,  /* [1925] */
    (xdc_Char)0x20,  /* [1926] */
    (xdc_Char)0x69,  /* [1927] */
    (xdc_Char)0x73,  /* [1928] */
    (xdc_Char)0x4d,  /* [1929] */
    (xdc_Char)0x65,  /* [1930] */
    (xdc_Char)0x6d,  /* [1931] */
    (xdc_Char)0x62,  /* [1932] */
    (xdc_Char)0x65,  /* [1933] */
    (xdc_Char)0x72,  /* [1934] */
    (xdc_Char)0x3a,  /* [1935] */
    (xdc_Char)0x20,  /* [1936] */
    (xdc_Char)0x25,  /* [1937] */
    (xdc_Char)0x64,  /* [1938] */
    (xdc_Char)0x0,  /* [1939] */
    (xdc_Char)0x2d,  /* [1940] */
    (xdc_Char)0x2d,  /* [1941] */
    (xdc_Char)0x3e,  /* [1942] */
    (xdc_Char)0x20,  /* [1943] */
    (xdc_Char)0x67,  /* [1944] */
    (xdc_Char)0x65,  /* [1945] */
    (xdc_Char)0x74,  /* [1946] */
    (xdc_Char)0x4e,  /* [1947] */
    (xdc_Char)0x65,  /* [1948] */
    (xdc_Char)0x78,  /* [1949] */
    (xdc_Char)0x74,  /* [1950] */
    (xdc_Char)0x4d,  /* [1951] */
    (xdc_Char)0x6f,  /* [1952] */
    (xdc_Char)0x64,  /* [1953] */
    (xdc_Char)0x75,  /* [1954] */
    (xdc_Char)0x6c,  /* [1955] */
    (xdc_Char)0x65,  /* [1956] */
    (xdc_Char)0x3a,  /* [1957] */
    (xdc_Char)0x20,  /* [1958] */
    (xdc_Char)0x28,  /* [1959] */
    (xdc_Char)0x25,  /* [1960] */
    (xdc_Char)0x70,  /* [1961] */
    (xdc_Char)0x29,  /* [1962] */
    (xdc_Char)0x0,  /* [1963] */
    (xdc_Char)0x3c,  /* [1964] */
    (xdc_Char)0x2d,  /* [1965] */
    (xdc_Char)0x2d,  /* [1966] */
    (xdc_Char)0x20,  /* [1967] */
    (xdc_Char)0x67,  /* [1968] */
    (xdc_Char)0x65,  /* [1969] */
    (xdc_Char)0x74,  /* [1970] */
    (xdc_Char)0x4e,  /* [1971] */
    (xdc_Char)0x65,  /* [1972] */
    (xdc_Char)0x78,  /* [1973] */
    (xdc_Char)0x74,  /* [1974] */
    (xdc_Char)0x4d,  /* [1975] */
    (xdc_Char)0x6f,  /* [1976] */
    (xdc_Char)0x64,  /* [1977] */
    (xdc_Char)0x75,  /* [1978] */
    (xdc_Char)0x6c,  /* [1979] */
    (xdc_Char)0x65,  /* [1980] */
    (xdc_Char)0x3a,  /* [1981] */
    (xdc_Char)0x20,  /* [1982] */
    (xdc_Char)0x25,  /* [1983] */
    (xdc_Char)0x70,  /* [1984] */
    (xdc_Char)0x0,  /* [1985] */
    (xdc_Char)0x2d,  /* [1986] */
    (xdc_Char)0x2d,  /* [1987] */
    (xdc_Char)0x3e,  /* [1988] */
    (xdc_Char)0x20,  /* [1989] */
    (xdc_Char)0x67,  /* [1990] */
    (xdc_Char)0x65,  /* [1991] */
    (xdc_Char)0x74,  /* [1992] */
    (xdc_Char)0x4d,  /* [1993] */
    (xdc_Char)0x6f,  /* [1994] */
    (xdc_Char)0x64,  /* [1995] */
    (xdc_Char)0x75,  /* [1996] */
    (xdc_Char)0x6c,  /* [1997] */
    (xdc_Char)0x65,  /* [1998] */
    (xdc_Char)0x4e,  /* [1999] */
    (xdc_Char)0x61,  /* [2000] */
    (xdc_Char)0x6d,  /* [2001] */
    (xdc_Char)0x65,  /* [2002] */
    (xdc_Char)0x3a,  /* [2003] */
    (xdc_Char)0x20,  /* [2004] */
    (xdc_Char)0x28,  /* [2005] */
    (xdc_Char)0x25,  /* [2006] */
    (xdc_Char)0x70,  /* [2007] */
    (xdc_Char)0x29,  /* [2008] */
    (xdc_Char)0x0,  /* [2009] */
    (xdc_Char)0x3c,  /* [2010] */
    (xdc_Char)0x2d,  /* [2011] */
    (xdc_Char)0x2d,  /* [2012] */
    (xdc_Char)0x20,  /* [2013] */
    (xdc_Char)0x67,  /* [2014] */
    (xdc_Char)0x65,  /* [2015] */
    (xdc_Char)0x74,  /* [2016] */
    (xdc_Char)0x4d,  /* [2017] */
    (xdc_Char)0x6f,  /* [2018] */
    (xdc_Char)0x64,  /* [2019] */
    (xdc_Char)0x75,  /* [2020] */
    (xdc_Char)0x6c,  /* [2021] */
    (xdc_Char)0x65,  /* [2022] */
    (xdc_Char)0x4e,  /* [2023] */
    (xdc_Char)0x61,  /* [2024] */
    (xdc_Char)0x6d,  /* [2025] */
    (xdc_Char)0x65,  /* [2026] */
    (xdc_Char)0x3a,  /* [2027] */
    (xdc_Char)0x20,  /* [2028] */
    (xdc_Char)0x22,  /* [2029] */
    (xdc_Char)0x25,  /* [2030] */
    (xdc_Char)0x73,  /* [2031] */
    (xdc_Char)0x22,  /* [2032] */
    (xdc_Char)0x0,  /* [2033] */
    (xdc_Char)0x2d,  /* [2034] */
    (xdc_Char)0x2d,  /* [2035] */
    (xdc_Char)0x3e,  /* [2036] */
    (xdc_Char)0x20,  /* [2037] */
    (xdc_Char)0x67,  /* [2038] */
    (xdc_Char)0x65,  /* [2039] */
    (xdc_Char)0x74,  /* [2040] */
    (xdc_Char)0x4d,  /* [2041] */
    (xdc_Char)0x6f,  /* [2042] */
    (xdc_Char)0x64,  /* [2043] */
    (xdc_Char)0x75,  /* [2044] */
    (xdc_Char)0x6c,  /* [2045] */
    (xdc_Char)0x65,  /* [2046] */
    (xdc_Char)0x49,  /* [2047] */
    (xdc_Char)0x64,  /* [2048] */
    (xdc_Char)0x3a,  /* [2049] */
    (xdc_Char)0x20,  /* [2050] */
    (xdc_Char)0x28,  /* [2051] */
    (xdc_Char)0x25,  /* [2052] */
    (xdc_Char)0x70,  /* [2053] */
    (xdc_Char)0x29,  /* [2054] */
    (xdc_Char)0x0,  /* [2055] */
    (xdc_Char)0x3c,  /* [2056] */
    (xdc_Char)0x2d,  /* [2057] */
    (xdc_Char)0x2d,  /* [2058] */
    (xdc_Char)0x20,  /* [2059] */
    (xdc_Char)0x67,  /* [2060] */
    (xdc_Char)0x65,  /* [2061] */
    (xdc_Char)0x74,  /* [2062] */
    (xdc_Char)0x4d,  /* [2063] */
    (xdc_Char)0x6f,  /* [2064] */
    (xdc_Char)0x64,  /* [2065] */
    (xdc_Char)0x75,  /* [2066] */
    (xdc_Char)0x6c,  /* [2067] */
    (xdc_Char)0x65,  /* [2068] */
    (xdc_Char)0x49,  /* [2069] */
    (xdc_Char)0x64,  /* [2070] */
    (xdc_Char)0x3a,  /* [2071] */
    (xdc_Char)0x20,  /* [2072] */
    (xdc_Char)0x25,  /* [2073] */
    (xdc_Char)0x64,  /* [2074] */
    (xdc_Char)0x0,  /* [2075] */
    (xdc_Char)0x4c,  /* [2076] */
    (xdc_Char)0x44,  /* [2077] */
    (xdc_Char)0x5f,  /* [2078] */
    (xdc_Char)0x63,  /* [2079] */
    (xdc_Char)0x6d,  /* [2080] */
    (xdc_Char)0x64,  /* [2081] */
    (xdc_Char)0x52,  /* [2082] */
    (xdc_Char)0x63,  /* [2083] */
    (xdc_Char)0x76,  /* [2084] */
    (xdc_Char)0x64,  /* [2085] */
    (xdc_Char)0x3a,  /* [2086] */
    (xdc_Char)0x20,  /* [2087] */
    (xdc_Char)0x52,  /* [2088] */
    (xdc_Char)0x65,  /* [2089] */
    (xdc_Char)0x63,  /* [2090] */
    (xdc_Char)0x65,  /* [2091] */
    (xdc_Char)0x69,  /* [2092] */
    (xdc_Char)0x76,  /* [2093] */
    (xdc_Char)0x65,  /* [2094] */
    (xdc_Char)0x64,  /* [2095] */
    (xdc_Char)0x20,  /* [2096] */
    (xdc_Char)0x63,  /* [2097] */
    (xdc_Char)0x6f,  /* [2098] */
    (xdc_Char)0x6d,  /* [2099] */
    (xdc_Char)0x6d,  /* [2100] */
    (xdc_Char)0x61,  /* [2101] */
    (xdc_Char)0x6e,  /* [2102] */
    (xdc_Char)0x64,  /* [2103] */
    (xdc_Char)0x3a,  /* [2104] */
    (xdc_Char)0x20,  /* [2105] */
    (xdc_Char)0x25,  /* [2106] */
    (xdc_Char)0x64,  /* [2107] */
    (xdc_Char)0x2c,  /* [2108] */
    (xdc_Char)0x20,  /* [2109] */
    (xdc_Char)0x61,  /* [2110] */
    (xdc_Char)0x72,  /* [2111] */
    (xdc_Char)0x67,  /* [2112] */
    (xdc_Char)0x30,  /* [2113] */
    (xdc_Char)0x3a,  /* [2114] */
    (xdc_Char)0x20,  /* [2115] */
    (xdc_Char)0x30,  /* [2116] */
    (xdc_Char)0x78,  /* [2117] */
    (xdc_Char)0x25,  /* [2118] */
    (xdc_Char)0x78,  /* [2119] */
    (xdc_Char)0x2c,  /* [2120] */
    (xdc_Char)0x20,  /* [2121] */
    (xdc_Char)0x61,  /* [2122] */
    (xdc_Char)0x72,  /* [2123] */
    (xdc_Char)0x67,  /* [2124] */
    (xdc_Char)0x31,  /* [2125] */
    (xdc_Char)0x3a,  /* [2126] */
    (xdc_Char)0x20,  /* [2127] */
    (xdc_Char)0x30,  /* [2128] */
    (xdc_Char)0x78,  /* [2129] */
    (xdc_Char)0x25,  /* [2130] */
    (xdc_Char)0x78,  /* [2131] */
    (xdc_Char)0x0,  /* [2132] */
    (xdc_Char)0x4c,  /* [2133] */
    (xdc_Char)0x44,  /* [2134] */
    (xdc_Char)0x5f,  /* [2135] */
    (xdc_Char)0x77,  /* [2136] */
    (xdc_Char)0x72,  /* [2137] */
    (xdc_Char)0x69,  /* [2138] */
    (xdc_Char)0x74,  /* [2139] */
    (xdc_Char)0x65,  /* [2140] */
    (xdc_Char)0x4d,  /* [2141] */
    (xdc_Char)0x61,  /* [2142] */
    (xdc_Char)0x73,  /* [2143] */
    (xdc_Char)0x6b,  /* [2144] */
    (xdc_Char)0x3a,  /* [2145] */
    (xdc_Char)0x20,  /* [2146] */
    (xdc_Char)0x4d,  /* [2147] */
    (xdc_Char)0x61,  /* [2148] */
    (xdc_Char)0x73,  /* [2149] */
    (xdc_Char)0x6b,  /* [2150] */
    (xdc_Char)0x20,  /* [2151] */
    (xdc_Char)0x61,  /* [2152] */
    (xdc_Char)0x64,  /* [2153] */
    (xdc_Char)0x64,  /* [2154] */
    (xdc_Char)0x72,  /* [2155] */
    (xdc_Char)0x65,  /* [2156] */
    (xdc_Char)0x73,  /* [2157] */
    (xdc_Char)0x3a,  /* [2158] */
    (xdc_Char)0x20,  /* [2159] */
    (xdc_Char)0x30,  /* [2160] */
    (xdc_Char)0x78,  /* [2161] */
    (xdc_Char)0x25,  /* [2162] */
    (xdc_Char)0x78,  /* [2163] */
    (xdc_Char)0x2c,  /* [2164] */
    (xdc_Char)0x20,  /* [2165] */
    (xdc_Char)0x4e,  /* [2166] */
    (xdc_Char)0x65,  /* [2167] */
    (xdc_Char)0x77,  /* [2168] */
    (xdc_Char)0x20,  /* [2169] */
    (xdc_Char)0x6d,  /* [2170] */
    (xdc_Char)0x61,  /* [2171] */
    (xdc_Char)0x73,  /* [2172] */
    (xdc_Char)0x6b,  /* [2173] */
    (xdc_Char)0x20,  /* [2174] */
    (xdc_Char)0x76,  /* [2175] */
    (xdc_Char)0x61,  /* [2176] */
    (xdc_Char)0x6c,  /* [2177] */
    (xdc_Char)0x75,  /* [2178] */
    (xdc_Char)0x65,  /* [2179] */
    (xdc_Char)0x3a,  /* [2180] */
    (xdc_Char)0x20,  /* [2181] */
    (xdc_Char)0x30,  /* [2182] */
    (xdc_Char)0x78,  /* [2183] */
    (xdc_Char)0x25,  /* [2184] */
    (xdc_Char)0x78,  /* [2185] */
    (xdc_Char)0x0,  /* [2186] */
    (xdc_Char)0x2d,  /* [2187] */
    (xdc_Char)0x2d,  /* [2188] */
    (xdc_Char)0x3e,  /* [2189] */
    (xdc_Char)0x20,  /* [2190] */
    (xdc_Char)0x65,  /* [2191] */
    (xdc_Char)0x6e,  /* [2192] */
    (xdc_Char)0x74,  /* [2193] */
    (xdc_Char)0x65,  /* [2194] */
    (xdc_Char)0x72,  /* [2195] */
    (xdc_Char)0x3a,  /* [2196] */
    (xdc_Char)0x20,  /* [2197] */
    (xdc_Char)0x28,  /* [2198] */
    (xdc_Char)0x25,  /* [2199] */
    (xdc_Char)0x70,  /* [2200] */
    (xdc_Char)0x29,  /* [2201] */
    (xdc_Char)0x0,  /* [2202] */
    (xdc_Char)0x3c,  /* [2203] */
    (xdc_Char)0x2d,  /* [2204] */
    (xdc_Char)0x2d,  /* [2205] */
    (xdc_Char)0x20,  /* [2206] */
    (xdc_Char)0x65,  /* [2207] */
    (xdc_Char)0x6e,  /* [2208] */
    (xdc_Char)0x74,  /* [2209] */
    (xdc_Char)0x65,  /* [2210] */
    (xdc_Char)0x72,  /* [2211] */
    (xdc_Char)0x3a,  /* [2212] */
    (xdc_Char)0x20,  /* [2213] */
    (xdc_Char)0x25,  /* [2214] */
    (xdc_Char)0x70,  /* [2215] */
    (xdc_Char)0x0,  /* [2216] */
    (xdc_Char)0x2d,  /* [2217] */
    (xdc_Char)0x2d,  /* [2218] */
    (xdc_Char)0x3e,  /* [2219] */
    (xdc_Char)0x20,  /* [2220] */
    (xdc_Char)0x6c,  /* [2221] */
    (xdc_Char)0x65,  /* [2222] */
    (xdc_Char)0x61,  /* [2223] */
    (xdc_Char)0x76,  /* [2224] */
    (xdc_Char)0x65,  /* [2225] */
    (xdc_Char)0x3a,  /* [2226] */
    (xdc_Char)0x20,  /* [2227] */
    (xdc_Char)0x28,  /* [2228] */
    (xdc_Char)0x25,  /* [2229] */
    (xdc_Char)0x70,  /* [2230] */
    (xdc_Char)0x2c,  /* [2231] */
    (xdc_Char)0x20,  /* [2232] */
    (xdc_Char)0x25,  /* [2233] */
    (xdc_Char)0x70,  /* [2234] */
    (xdc_Char)0x29,  /* [2235] */
    (xdc_Char)0x0,  /* [2236] */
    (xdc_Char)0x3c,  /* [2237] */
    (xdc_Char)0x2d,  /* [2238] */
    (xdc_Char)0x2d,  /* [2239] */
    (xdc_Char)0x20,  /* [2240] */
    (xdc_Char)0x6c,  /* [2241] */
    (xdc_Char)0x65,  /* [2242] */
    (xdc_Char)0x61,  /* [2243] */
    (xdc_Char)0x76,  /* [2244] */
    (xdc_Char)0x65,  /* [2245] */
    (xdc_Char)0x3a,  /* [2246] */
    (xdc_Char)0x20,  /* [2247] */
    (xdc_Char)0x76,  /* [2248] */
    (xdc_Char)0x6f,  /* [2249] */
    (xdc_Char)0x69,  /* [2250] */
    (xdc_Char)0x64,  /* [2251] */
    (xdc_Char)0x0,  /* [2252] */
    (xdc_Char)0x2d,  /* [2253] */
    (xdc_Char)0x2d,  /* [2254] */
    (xdc_Char)0x3e,  /* [2255] */
    (xdc_Char)0x20,  /* [2256] */
    (xdc_Char)0x70,  /* [2257] */
    (xdc_Char)0x65,  /* [2258] */
    (xdc_Char)0x6e,  /* [2259] */
    (xdc_Char)0x64,  /* [2260] */
    (xdc_Char)0x3a,  /* [2261] */
    (xdc_Char)0x20,  /* [2262] */
    (xdc_Char)0x28,  /* [2263] */
    (xdc_Char)0x25,  /* [2264] */
    (xdc_Char)0x70,  /* [2265] */
    (xdc_Char)0x2c,  /* [2266] */
    (xdc_Char)0x20,  /* [2267] */
    (xdc_Char)0x30,  /* [2268] */
    (xdc_Char)0x78,  /* [2269] */
    (xdc_Char)0x25,  /* [2270] */
    (xdc_Char)0x78,  /* [2271] */
    (xdc_Char)0x2c,  /* [2272] */
    (xdc_Char)0x20,  /* [2273] */
    (xdc_Char)0x25,  /* [2274] */
    (xdc_Char)0x70,  /* [2275] */
    (xdc_Char)0x29,  /* [2276] */
    (xdc_Char)0x0,  /* [2277] */
    (xdc_Char)0x3c,  /* [2278] */
    (xdc_Char)0x2d,  /* [2279] */
    (xdc_Char)0x2d,  /* [2280] */
    (xdc_Char)0x20,  /* [2281] */
    (xdc_Char)0x70,  /* [2282] */
    (xdc_Char)0x65,  /* [2283] */
    (xdc_Char)0x6e,  /* [2284] */
    (xdc_Char)0x64,  /* [2285] */
    (xdc_Char)0x3a,  /* [2286] */
    (xdc_Char)0x20,  /* [2287] */
    (xdc_Char)0x25,  /* [2288] */
    (xdc_Char)0x64,  /* [2289] */
    (xdc_Char)0x0,  /* [2290] */
    (xdc_Char)0x2d,  /* [2291] */
    (xdc_Char)0x2d,  /* [2292] */
    (xdc_Char)0x3e,  /* [2293] */
    (xdc_Char)0x20,  /* [2294] */
    (xdc_Char)0x70,  /* [2295] */
    (xdc_Char)0x6f,  /* [2296] */
    (xdc_Char)0x73,  /* [2297] */
    (xdc_Char)0x74,  /* [2298] */
    (xdc_Char)0x3a,  /* [2299] */
    (xdc_Char)0x20,  /* [2300] */
    (xdc_Char)0x28,  /* [2301] */
    (xdc_Char)0x25,  /* [2302] */
    (xdc_Char)0x70,  /* [2303] */
    (xdc_Char)0x2c,  /* [2304] */
    (xdc_Char)0x20,  /* [2305] */
    (xdc_Char)0x25,  /* [2306] */
    (xdc_Char)0x70,  /* [2307] */
    (xdc_Char)0x29,  /* [2308] */
    (xdc_Char)0x0,  /* [2309] */
    (xdc_Char)0x3c,  /* [2310] */
    (xdc_Char)0x2d,  /* [2311] */
    (xdc_Char)0x2d,  /* [2312] */
    (xdc_Char)0x20,  /* [2313] */
    (xdc_Char)0x70,  /* [2314] */
    (xdc_Char)0x6f,  /* [2315] */
    (xdc_Char)0x73,  /* [2316] */
    (xdc_Char)0x74,  /* [2317] */
    (xdc_Char)0x3a,  /* [2318] */
    (xdc_Char)0x20,  /* [2319] */
    (xdc_Char)0x25,  /* [2320] */
    (xdc_Char)0x64,  /* [2321] */
    (xdc_Char)0x0,  /* [2322] */
    (xdc_Char)0x2d,  /* [2323] */
    (xdc_Char)0x2d,  /* [2324] */
    (xdc_Char)0x3e,  /* [2325] */
    (xdc_Char)0x20,  /* [2326] */
    (xdc_Char)0x73,  /* [2327] */
    (xdc_Char)0x74,  /* [2328] */
    (xdc_Char)0x61,  /* [2329] */
    (xdc_Char)0x72,  /* [2330] */
    (xdc_Char)0x74,  /* [2331] */
    (xdc_Char)0x3a,  /* [2332] */
    (xdc_Char)0x20,  /* [2333] */
    (xdc_Char)0x28,  /* [2334] */
    (xdc_Char)0x25,  /* [2335] */
    (xdc_Char)0x70,  /* [2336] */
    (xdc_Char)0x29,  /* [2337] */
    (xdc_Char)0x0,  /* [2338] */
    (xdc_Char)0x3c,  /* [2339] */
    (xdc_Char)0x2d,  /* [2340] */
    (xdc_Char)0x2d,  /* [2341] */
    (xdc_Char)0x20,  /* [2342] */
    (xdc_Char)0x73,  /* [2343] */
    (xdc_Char)0x74,  /* [2344] */
    (xdc_Char)0x61,  /* [2345] */
    (xdc_Char)0x72,  /* [2346] */
    (xdc_Char)0x74,  /* [2347] */
    (xdc_Char)0x3a,  /* [2348] */
    (xdc_Char)0x20,  /* [2349] */
    (xdc_Char)0x25,  /* [2350] */
    (xdc_Char)0x64,  /* [2351] */
    (xdc_Char)0x0,  /* [2352] */
    (xdc_Char)0x2d,  /* [2353] */
    (xdc_Char)0x2d,  /* [2354] */
    (xdc_Char)0x3e,  /* [2355] */
    (xdc_Char)0x20,  /* [2356] */
    (xdc_Char)0x79,  /* [2357] */
    (xdc_Char)0x69,  /* [2358] */
    (xdc_Char)0x65,  /* [2359] */
    (xdc_Char)0x6c,  /* [2360] */
    (xdc_Char)0x64,  /* [2361] */
    (xdc_Char)0x3a,  /* [2362] */
    (xdc_Char)0x20,  /* [2363] */
    (xdc_Char)0x28,  /* [2364] */
    (xdc_Char)0x25,  /* [2365] */
    (xdc_Char)0x70,  /* [2366] */
    (xdc_Char)0x29,  /* [2367] */
    (xdc_Char)0x0,  /* [2368] */
    (xdc_Char)0x3c,  /* [2369] */
    (xdc_Char)0x2d,  /* [2370] */
    (xdc_Char)0x2d,  /* [2371] */
    (xdc_Char)0x20,  /* [2372] */
    (xdc_Char)0x79,  /* [2373] */
    (xdc_Char)0x69,  /* [2374] */
    (xdc_Char)0x65,  /* [2375] */
    (xdc_Char)0x6c,  /* [2376] */
    (xdc_Char)0x64,  /* [2377] */
    (xdc_Char)0x3a,  /* [2378] */
    (xdc_Char)0x20,  /* [2379] */
    (xdc_Char)0x25,  /* [2380] */
    (xdc_Char)0x64,  /* [2381] */
    (xdc_Char)0x0,  /* [2382] */
    (xdc_Char)0x2d,  /* [2383] */
    (xdc_Char)0x2d,  /* [2384] */
    (xdc_Char)0x3e,  /* [2385] */
    (xdc_Char)0x20,  /* [2386] */
    (xdc_Char)0x63,  /* [2387] */
    (xdc_Char)0x6f,  /* [2388] */
    (xdc_Char)0x6d,  /* [2389] */
    (xdc_Char)0x70,  /* [2390] */
    (xdc_Char)0x61,  /* [2391] */
    (xdc_Char)0x72,  /* [2392] */
    (xdc_Char)0x65,  /* [2393] */
    (xdc_Char)0x4f,  /* [2394] */
    (xdc_Char)0x73,  /* [2395] */
    (xdc_Char)0x50,  /* [2396] */
    (xdc_Char)0x72,  /* [2397] */
    (xdc_Char)0x69,  /* [2398] */
    (xdc_Char)0x6f,  /* [2399] */
    (xdc_Char)0x72,  /* [2400] */
    (xdc_Char)0x69,  /* [2401] */
    (xdc_Char)0x74,  /* [2402] */
    (xdc_Char)0x69,  /* [2403] */
    (xdc_Char)0x65,  /* [2404] */
    (xdc_Char)0x73,  /* [2405] */
    (xdc_Char)0x3a,  /* [2406] */
    (xdc_Char)0x20,  /* [2407] */
    (xdc_Char)0x28,  /* [2408] */
    (xdc_Char)0x25,  /* [2409] */
    (xdc_Char)0x64,  /* [2410] */
    (xdc_Char)0x2c,  /* [2411] */
    (xdc_Char)0x20,  /* [2412] */
    (xdc_Char)0x25,  /* [2413] */
    (xdc_Char)0x64,  /* [2414] */
    (xdc_Char)0x2c,  /* [2415] */
    (xdc_Char)0x20,  /* [2416] */
    (xdc_Char)0x25,  /* [2417] */
    (xdc_Char)0x70,  /* [2418] */
    (xdc_Char)0x29,  /* [2419] */
    (xdc_Char)0x0,  /* [2420] */
    (xdc_Char)0x3c,  /* [2421] */
    (xdc_Char)0x2d,  /* [2422] */
    (xdc_Char)0x2d,  /* [2423] */
    (xdc_Char)0x20,  /* [2424] */
    (xdc_Char)0x63,  /* [2425] */
    (xdc_Char)0x6f,  /* [2426] */
    (xdc_Char)0x6d,  /* [2427] */
    (xdc_Char)0x70,  /* [2428] */
    (xdc_Char)0x61,  /* [2429] */
    (xdc_Char)0x72,  /* [2430] */
    (xdc_Char)0x65,  /* [2431] */
    (xdc_Char)0x4f,  /* [2432] */
    (xdc_Char)0x73,  /* [2433] */
    (xdc_Char)0x50,  /* [2434] */
    (xdc_Char)0x72,  /* [2435] */
    (xdc_Char)0x69,  /* [2436] */
    (xdc_Char)0x6f,  /* [2437] */
    (xdc_Char)0x72,  /* [2438] */
    (xdc_Char)0x69,  /* [2439] */
    (xdc_Char)0x74,  /* [2440] */
    (xdc_Char)0x69,  /* [2441] */
    (xdc_Char)0x65,  /* [2442] */
    (xdc_Char)0x73,  /* [2443] */
    (xdc_Char)0x3a,  /* [2444] */
    (xdc_Char)0x20,  /* [2445] */
    (xdc_Char)0x25,  /* [2446] */
    (xdc_Char)0x64,  /* [2447] */
    (xdc_Char)0x0,  /* [2448] */
    (xdc_Char)0x2d,  /* [2449] */
    (xdc_Char)0x2d,  /* [2450] */
    (xdc_Char)0x3e,  /* [2451] */
    (xdc_Char)0x20,  /* [2452] */
    (xdc_Char)0x73,  /* [2453] */
    (xdc_Char)0x6c,  /* [2454] */
    (xdc_Char)0x65,  /* [2455] */
    (xdc_Char)0x65,  /* [2456] */
    (xdc_Char)0x70,  /* [2457] */
    (xdc_Char)0x3a,  /* [2458] */
    (xdc_Char)0x20,  /* [2459] */
    (xdc_Char)0x28,  /* [2460] */
    (xdc_Char)0x30,  /* [2461] */
    (xdc_Char)0x78,  /* [2462] */
    (xdc_Char)0x25,  /* [2463] */
    (xdc_Char)0x78,  /* [2464] */
    (xdc_Char)0x2c,  /* [2465] */
    (xdc_Char)0x20,  /* [2466] */
    (xdc_Char)0x25,  /* [2467] */
    (xdc_Char)0x70,  /* [2468] */
    (xdc_Char)0x29,  /* [2469] */
    (xdc_Char)0x0,  /* [2470] */
    (xdc_Char)0x3c,  /* [2471] */
    (xdc_Char)0x2d,  /* [2472] */
    (xdc_Char)0x2d,  /* [2473] */
    (xdc_Char)0x20,  /* [2474] */
    (xdc_Char)0x73,  /* [2475] */
    (xdc_Char)0x6c,  /* [2476] */
    (xdc_Char)0x65,  /* [2477] */
    (xdc_Char)0x65,  /* [2478] */
    (xdc_Char)0x70,  /* [2479] */
    (xdc_Char)0x3a,  /* [2480] */
    (xdc_Char)0x20,  /* [2481] */
    (xdc_Char)0x25,  /* [2482] */
    (xdc_Char)0x64,  /* [2483] */
    (xdc_Char)0x0,  /* [2484] */
    (xdc_Char)0x2d,  /* [2485] */
    (xdc_Char)0x2d,  /* [2486] */
    (xdc_Char)0x3e,  /* [2487] */
    (xdc_Char)0x20,  /* [2488] */
    (xdc_Char)0x6a,  /* [2489] */
    (xdc_Char)0x6f,  /* [2490] */
    (xdc_Char)0x69,  /* [2491] */
    (xdc_Char)0x6e,  /* [2492] */
    (xdc_Char)0x3a,  /* [2493] */
    (xdc_Char)0x20,  /* [2494] */
    (xdc_Char)0x28,  /* [2495] */
    (xdc_Char)0x25,  /* [2496] */
    (xdc_Char)0x70,  /* [2497] */
    (xdc_Char)0x2c,  /* [2498] */
    (xdc_Char)0x20,  /* [2499] */
    (xdc_Char)0x25,  /* [2500] */
    (xdc_Char)0x70,  /* [2501] */
    (xdc_Char)0x29,  /* [2502] */
    (xdc_Char)0x0,  /* [2503] */
    (xdc_Char)0x3c,  /* [2504] */
    (xdc_Char)0x2d,  /* [2505] */
    (xdc_Char)0x2d,  /* [2506] */
    (xdc_Char)0x20,  /* [2507] */
    (xdc_Char)0x6a,  /* [2508] */
    (xdc_Char)0x6f,  /* [2509] */
    (xdc_Char)0x69,  /* [2510] */
    (xdc_Char)0x6e,  /* [2511] */
    (xdc_Char)0x3a,  /* [2512] */
    (xdc_Char)0x20,  /* [2513] */
    (xdc_Char)0x25,  /* [2514] */
    (xdc_Char)0x64,  /* [2515] */
    (xdc_Char)0x0,  /* [2516] */
    (xdc_Char)0x2d,  /* [2517] */
    (xdc_Char)0x2d,  /* [2518] */
    (xdc_Char)0x3e,  /* [2519] */
    (xdc_Char)0x20,  /* [2520] */
    (xdc_Char)0x67,  /* [2521] */
    (xdc_Char)0x65,  /* [2522] */
    (xdc_Char)0x74,  /* [2523] */
    (xdc_Char)0x50,  /* [2524] */
    (xdc_Char)0x72,  /* [2525] */
    (xdc_Char)0x69,  /* [2526] */
    (xdc_Char)0x6f,  /* [2527] */
    (xdc_Char)0x72,  /* [2528] */
    (xdc_Char)0x69,  /* [2529] */
    (xdc_Char)0x74,  /* [2530] */
    (xdc_Char)0x79,  /* [2531] */
    (xdc_Char)0x3a,  /* [2532] */
    (xdc_Char)0x20,  /* [2533] */
    (xdc_Char)0x28,  /* [2534] */
    (xdc_Char)0x25,  /* [2535] */
    (xdc_Char)0x70,  /* [2536] */
    (xdc_Char)0x2c,  /* [2537] */
    (xdc_Char)0x20,  /* [2538] */
    (xdc_Char)0x25,  /* [2539] */
    (xdc_Char)0x70,  /* [2540] */
    (xdc_Char)0x29,  /* [2541] */
    (xdc_Char)0x0,  /* [2542] */
    (xdc_Char)0x3c,  /* [2543] */
    (xdc_Char)0x2d,  /* [2544] */
    (xdc_Char)0x2d,  /* [2545] */
    (xdc_Char)0x20,  /* [2546] */
    (xdc_Char)0x67,  /* [2547] */
    (xdc_Char)0x65,  /* [2548] */
    (xdc_Char)0x74,  /* [2549] */
    (xdc_Char)0x50,  /* [2550] */
    (xdc_Char)0x72,  /* [2551] */
    (xdc_Char)0x69,  /* [2552] */
    (xdc_Char)0x6f,  /* [2553] */
    (xdc_Char)0x72,  /* [2554] */
    (xdc_Char)0x69,  /* [2555] */
    (xdc_Char)0x74,  /* [2556] */
    (xdc_Char)0x79,  /* [2557] */
    (xdc_Char)0x3a,  /* [2558] */
    (xdc_Char)0x20,  /* [2559] */
    (xdc_Char)0x25,  /* [2560] */
    (xdc_Char)0x64,  /* [2561] */
    (xdc_Char)0x0,  /* [2562] */
    (xdc_Char)0x2d,  /* [2563] */
    (xdc_Char)0x2d,  /* [2564] */
    (xdc_Char)0x3e,  /* [2565] */
    (xdc_Char)0x20,  /* [2566] */
    (xdc_Char)0x73,  /* [2567] */
    (xdc_Char)0x65,  /* [2568] */
    (xdc_Char)0x74,  /* [2569] */
    (xdc_Char)0x50,  /* [2570] */
    (xdc_Char)0x72,  /* [2571] */
    (xdc_Char)0x69,  /* [2572] */
    (xdc_Char)0x6f,  /* [2573] */
    (xdc_Char)0x72,  /* [2574] */
    (xdc_Char)0x69,  /* [2575] */
    (xdc_Char)0x74,  /* [2576] */
    (xdc_Char)0x79,  /* [2577] */
    (xdc_Char)0x3a,  /* [2578] */
    (xdc_Char)0x20,  /* [2579] */
    (xdc_Char)0x28,  /* [2580] */
    (xdc_Char)0x25,  /* [2581] */
    (xdc_Char)0x70,  /* [2582] */
    (xdc_Char)0x2c,  /* [2583] */
    (xdc_Char)0x20,  /* [2584] */
    (xdc_Char)0x25,  /* [2585] */
    (xdc_Char)0x64,  /* [2586] */
    (xdc_Char)0x2c,  /* [2587] */
    (xdc_Char)0x20,  /* [2588] */
    (xdc_Char)0x25,  /* [2589] */
    (xdc_Char)0x70,  /* [2590] */
    (xdc_Char)0x29,  /* [2591] */
    (xdc_Char)0x0,  /* [2592] */
    (xdc_Char)0x3c,  /* [2593] */
    (xdc_Char)0x2d,  /* [2594] */
    (xdc_Char)0x2d,  /* [2595] */
    (xdc_Char)0x20,  /* [2596] */
    (xdc_Char)0x73,  /* [2597] */
    (xdc_Char)0x65,  /* [2598] */
    (xdc_Char)0x74,  /* [2599] */
    (xdc_Char)0x50,  /* [2600] */
    (xdc_Char)0x72,  /* [2601] */
    (xdc_Char)0x69,  /* [2602] */
    (xdc_Char)0x6f,  /* [2603] */
    (xdc_Char)0x72,  /* [2604] */
    (xdc_Char)0x69,  /* [2605] */
    (xdc_Char)0x74,  /* [2606] */
    (xdc_Char)0x79,  /* [2607] */
    (xdc_Char)0x3a,  /* [2608] */
    (xdc_Char)0x20,  /* [2609] */
    (xdc_Char)0x25,  /* [2610] */
    (xdc_Char)0x64,  /* [2611] */
    (xdc_Char)0x0,  /* [2612] */
    (xdc_Char)0x2d,  /* [2613] */
    (xdc_Char)0x2d,  /* [2614] */
    (xdc_Char)0x3e,  /* [2615] */
    (xdc_Char)0x20,  /* [2616] */
    (xdc_Char)0x67,  /* [2617] */
    (xdc_Char)0x65,  /* [2618] */
    (xdc_Char)0x74,  /* [2619] */
    (xdc_Char)0x4f,  /* [2620] */
    (xdc_Char)0x73,  /* [2621] */
    (xdc_Char)0x50,  /* [2622] */
    (xdc_Char)0x72,  /* [2623] */
    (xdc_Char)0x69,  /* [2624] */
    (xdc_Char)0x6f,  /* [2625] */
    (xdc_Char)0x72,  /* [2626] */
    (xdc_Char)0x69,  /* [2627] */
    (xdc_Char)0x74,  /* [2628] */
    (xdc_Char)0x79,  /* [2629] */
    (xdc_Char)0x3a,  /* [2630] */
    (xdc_Char)0x20,  /* [2631] */
    (xdc_Char)0x28,  /* [2632] */
    (xdc_Char)0x25,  /* [2633] */
    (xdc_Char)0x70,  /* [2634] */
    (xdc_Char)0x2c,  /* [2635] */
    (xdc_Char)0x20,  /* [2636] */
    (xdc_Char)0x25,  /* [2637] */
    (xdc_Char)0x70,  /* [2638] */
    (xdc_Char)0x29,  /* [2639] */
    (xdc_Char)0x0,  /* [2640] */
    (xdc_Char)0x3c,  /* [2641] */
    (xdc_Char)0x2d,  /* [2642] */
    (xdc_Char)0x2d,  /* [2643] */
    (xdc_Char)0x20,  /* [2644] */
    (xdc_Char)0x67,  /* [2645] */
    (xdc_Char)0x65,  /* [2646] */
    (xdc_Char)0x74,  /* [2647] */
    (xdc_Char)0x4f,  /* [2648] */
    (xdc_Char)0x73,  /* [2649] */
    (xdc_Char)0x50,  /* [2650] */
    (xdc_Char)0x72,  /* [2651] */
    (xdc_Char)0x69,  /* [2652] */
    (xdc_Char)0x6f,  /* [2653] */
    (xdc_Char)0x72,  /* [2654] */
    (xdc_Char)0x69,  /* [2655] */
    (xdc_Char)0x74,  /* [2656] */
    (xdc_Char)0x79,  /* [2657] */
    (xdc_Char)0x3a,  /* [2658] */
    (xdc_Char)0x20,  /* [2659] */
    (xdc_Char)0x25,  /* [2660] */
    (xdc_Char)0x64,  /* [2661] */
    (xdc_Char)0x0,  /* [2662] */
    (xdc_Char)0x2d,  /* [2663] */
    (xdc_Char)0x2d,  /* [2664] */
    (xdc_Char)0x3e,  /* [2665] */
    (xdc_Char)0x20,  /* [2666] */
    (xdc_Char)0x73,  /* [2667] */
    (xdc_Char)0x65,  /* [2668] */
    (xdc_Char)0x74,  /* [2669] */
    (xdc_Char)0x4f,  /* [2670] */
    (xdc_Char)0x73,  /* [2671] */
    (xdc_Char)0x50,  /* [2672] */
    (xdc_Char)0x72,  /* [2673] */
    (xdc_Char)0x69,  /* [2674] */
    (xdc_Char)0x6f,  /* [2675] */
    (xdc_Char)0x72,  /* [2676] */
    (xdc_Char)0x69,  /* [2677] */
    (xdc_Char)0x74,  /* [2678] */
    (xdc_Char)0x79,  /* [2679] */
    (xdc_Char)0x3a,  /* [2680] */
    (xdc_Char)0x20,  /* [2681] */
    (xdc_Char)0x28,  /* [2682] */
    (xdc_Char)0x25,  /* [2683] */
    (xdc_Char)0x70,  /* [2684] */
    (xdc_Char)0x2c,  /* [2685] */
    (xdc_Char)0x20,  /* [2686] */
    (xdc_Char)0x25,  /* [2687] */
    (xdc_Char)0x64,  /* [2688] */
    (xdc_Char)0x2c,  /* [2689] */
    (xdc_Char)0x20,  /* [2690] */
    (xdc_Char)0x25,  /* [2691] */
    (xdc_Char)0x70,  /* [2692] */
    (xdc_Char)0x29,  /* [2693] */
    (xdc_Char)0x0,  /* [2694] */
    (xdc_Char)0x3c,  /* [2695] */
    (xdc_Char)0x2d,  /* [2696] */
    (xdc_Char)0x2d,  /* [2697] */
    (xdc_Char)0x20,  /* [2698] */
    (xdc_Char)0x73,  /* [2699] */
    (xdc_Char)0x65,  /* [2700] */
    (xdc_Char)0x74,  /* [2701] */
    (xdc_Char)0x4f,  /* [2702] */
    (xdc_Char)0x73,  /* [2703] */
    (xdc_Char)0x50,  /* [2704] */
    (xdc_Char)0x72,  /* [2705] */
    (xdc_Char)0x69,  /* [2706] */
    (xdc_Char)0x6f,  /* [2707] */
    (xdc_Char)0x72,  /* [2708] */
    (xdc_Char)0x69,  /* [2709] */
    (xdc_Char)0x74,  /* [2710] */
    (xdc_Char)0x79,  /* [2711] */
    (xdc_Char)0x3a,  /* [2712] */
    (xdc_Char)0x20,  /* [2713] */
    (xdc_Char)0x25,  /* [2714] */
    (xdc_Char)0x64,  /* [2715] */
    (xdc_Char)0x0,  /* [2716] */
    (xdc_Char)0x2d,  /* [2717] */
    (xdc_Char)0x2d,  /* [2718] */
    (xdc_Char)0x3e,  /* [2719] */
    (xdc_Char)0x20,  /* [2720] */
    (xdc_Char)0x67,  /* [2721] */
    (xdc_Char)0x65,  /* [2722] */
    (xdc_Char)0x74,  /* [2723] */
    (xdc_Char)0x4f,  /* [2724] */
    (xdc_Char)0x73,  /* [2725] */
    (xdc_Char)0x48,  /* [2726] */
    (xdc_Char)0x61,  /* [2727] */
    (xdc_Char)0x6e,  /* [2728] */
    (xdc_Char)0x64,  /* [2729] */
    (xdc_Char)0x6c,  /* [2730] */
    (xdc_Char)0x65,  /* [2731] */
    (xdc_Char)0x3a,  /* [2732] */
    (xdc_Char)0x20,  /* [2733] */
    (xdc_Char)0x28,  /* [2734] */
    (xdc_Char)0x25,  /* [2735] */
    (xdc_Char)0x70,  /* [2736] */
    (xdc_Char)0x29,  /* [2737] */
    (xdc_Char)0x0,  /* [2738] */
    (xdc_Char)0x3c,  /* [2739] */
    (xdc_Char)0x2d,  /* [2740] */
    (xdc_Char)0x2d,  /* [2741] */
    (xdc_Char)0x20,  /* [2742] */
    (xdc_Char)0x67,  /* [2743] */
    (xdc_Char)0x65,  /* [2744] */
    (xdc_Char)0x74,  /* [2745] */
    (xdc_Char)0x4f,  /* [2746] */
    (xdc_Char)0x73,  /* [2747] */
    (xdc_Char)0x48,  /* [2748] */
    (xdc_Char)0x61,  /* [2749] */
    (xdc_Char)0x6e,  /* [2750] */
    (xdc_Char)0x64,  /* [2751] */
    (xdc_Char)0x6c,  /* [2752] */
    (xdc_Char)0x65,  /* [2753] */
    (xdc_Char)0x3a,  /* [2754] */
    (xdc_Char)0x20,  /* [2755] */
    (xdc_Char)0x25,  /* [2756] */
    (xdc_Char)0x70,  /* [2757] */
    (xdc_Char)0x0,  /* [2758] */
    (xdc_Char)0x2d,  /* [2759] */
    (xdc_Char)0x2d,  /* [2760] */
    (xdc_Char)0x3e,  /* [2761] */
    (xdc_Char)0x20,  /* [2762] */
    (xdc_Char)0x67,  /* [2763] */
    (xdc_Char)0x65,  /* [2764] */
    (xdc_Char)0x74,  /* [2765] */
    (xdc_Char)0x54,  /* [2766] */
    (xdc_Char)0x6c,  /* [2767] */
    (xdc_Char)0x73,  /* [2768] */
    (xdc_Char)0x3a,  /* [2769] */
    (xdc_Char)0x20,  /* [2770] */
    (xdc_Char)0x28,  /* [2771] */
    (xdc_Char)0x25,  /* [2772] */
    (xdc_Char)0x70,  /* [2773] */
    (xdc_Char)0x29,  /* [2774] */
    (xdc_Char)0x0,  /* [2775] */
    (xdc_Char)0x3c,  /* [2776] */
    (xdc_Char)0x2d,  /* [2777] */
    (xdc_Char)0x2d,  /* [2778] */
    (xdc_Char)0x20,  /* [2779] */
    (xdc_Char)0x67,  /* [2780] */
    (xdc_Char)0x65,  /* [2781] */
    (xdc_Char)0x74,  /* [2782] */
    (xdc_Char)0x54,  /* [2783] */
    (xdc_Char)0x6c,  /* [2784] */
    (xdc_Char)0x73,  /* [2785] */
    (xdc_Char)0x3a,  /* [2786] */
    (xdc_Char)0x20,  /* [2787] */
    (xdc_Char)0x25,  /* [2788] */
    (xdc_Char)0x70,  /* [2789] */
    (xdc_Char)0x0,  /* [2790] */
    (xdc_Char)0x2d,  /* [2791] */
    (xdc_Char)0x2d,  /* [2792] */
    (xdc_Char)0x3e,  /* [2793] */
    (xdc_Char)0x20,  /* [2794] */
    (xdc_Char)0x73,  /* [2795] */
    (xdc_Char)0x65,  /* [2796] */
    (xdc_Char)0x74,  /* [2797] */
    (xdc_Char)0x54,  /* [2798] */
    (xdc_Char)0x6c,  /* [2799] */
    (xdc_Char)0x73,  /* [2800] */
    (xdc_Char)0x3a,  /* [2801] */
    (xdc_Char)0x20,  /* [2802] */
    (xdc_Char)0x28,  /* [2803] */
    (xdc_Char)0x25,  /* [2804] */
    (xdc_Char)0x70,  /* [2805] */
    (xdc_Char)0x2c,  /* [2806] */
    (xdc_Char)0x20,  /* [2807] */
    (xdc_Char)0x25,  /* [2808] */
    (xdc_Char)0x70,  /* [2809] */
    (xdc_Char)0x29,  /* [2810] */
    (xdc_Char)0x0,  /* [2811] */
    (xdc_Char)0x3c,  /* [2812] */
    (xdc_Char)0x2d,  /* [2813] */
    (xdc_Char)0x2d,  /* [2814] */
    (xdc_Char)0x20,  /* [2815] */
    (xdc_Char)0x73,  /* [2816] */
    (xdc_Char)0x65,  /* [2817] */
    (xdc_Char)0x74,  /* [2818] */
    (xdc_Char)0x54,  /* [2819] */
    (xdc_Char)0x6c,  /* [2820] */
    (xdc_Char)0x73,  /* [2821] */
    (xdc_Char)0x3a,  /* [2822] */
    (xdc_Char)0x20,  /* [2823] */
    (xdc_Char)0x76,  /* [2824] */
    (xdc_Char)0x6f,  /* [2825] */
    (xdc_Char)0x69,  /* [2826] */
    (xdc_Char)0x64,  /* [2827] */
    (xdc_Char)0x0,  /* [2828] */
    (xdc_Char)0x2d,  /* [2829] */
    (xdc_Char)0x2d,  /* [2830] */
    (xdc_Char)0x3e,  /* [2831] */
    (xdc_Char)0x20,  /* [2832] */
    (xdc_Char)0x73,  /* [2833] */
    (xdc_Char)0x74,  /* [2834] */
    (xdc_Char)0x61,  /* [2835] */
    (xdc_Char)0x74,  /* [2836] */
    (xdc_Char)0x3a,  /* [2837] */
    (xdc_Char)0x20,  /* [2838] */
    (xdc_Char)0x28,  /* [2839] */
    (xdc_Char)0x25,  /* [2840] */
    (xdc_Char)0x70,  /* [2841] */
    (xdc_Char)0x2c,  /* [2842] */
    (xdc_Char)0x20,  /* [2843] */
    (xdc_Char)0x25,  /* [2844] */
    (xdc_Char)0x70,  /* [2845] */
    (xdc_Char)0x2c,  /* [2846] */
    (xdc_Char)0x20,  /* [2847] */
    (xdc_Char)0x25,  /* [2848] */
    (xdc_Char)0x70,  /* [2849] */
    (xdc_Char)0x29,  /* [2850] */
    (xdc_Char)0x0,  /* [2851] */
    (xdc_Char)0x3c,  /* [2852] */
    (xdc_Char)0x2d,  /* [2853] */
    (xdc_Char)0x2d,  /* [2854] */
    (xdc_Char)0x20,  /* [2855] */
    (xdc_Char)0x73,  /* [2856] */
    (xdc_Char)0x74,  /* [2857] */
    (xdc_Char)0x61,  /* [2858] */
    (xdc_Char)0x74,  /* [2859] */
    (xdc_Char)0x3a,  /* [2860] */
    (xdc_Char)0x20,  /* [2861] */
    (xdc_Char)0x25,  /* [2862] */
    (xdc_Char)0x64,  /* [2863] */
    (xdc_Char)0x0,  /* [2864] */
    (xdc_Char)0x2d,  /* [2865] */
    (xdc_Char)0x2d,  /* [2866] */
    (xdc_Char)0x3e,  /* [2867] */
    (xdc_Char)0x20,  /* [2868] */
    (xdc_Char)0x71,  /* [2869] */
    (xdc_Char)0x75,  /* [2870] */
    (xdc_Char)0x65,  /* [2871] */
    (xdc_Char)0x72,  /* [2872] */
    (xdc_Char)0x79,  /* [2873] */
    (xdc_Char)0x3a,  /* [2874] */
    (xdc_Char)0x20,  /* [2875] */
    (xdc_Char)0x28,  /* [2876] */
    (xdc_Char)0x25,  /* [2877] */
    (xdc_Char)0x70,  /* [2878] */
    (xdc_Char)0x2c,  /* [2879] */
    (xdc_Char)0x20,  /* [2880] */
    (xdc_Char)0x25,  /* [2881] */
    (xdc_Char)0x64,  /* [2882] */
    (xdc_Char)0x29,  /* [2883] */
    (xdc_Char)0x0,  /* [2884] */
    (xdc_Char)0x3c,  /* [2885] */
    (xdc_Char)0x2d,  /* [2886] */
    (xdc_Char)0x2d,  /* [2887] */
    (xdc_Char)0x20,  /* [2888] */
    (xdc_Char)0x71,  /* [2889] */
    (xdc_Char)0x75,  /* [2890] */
    (xdc_Char)0x65,  /* [2891] */
    (xdc_Char)0x72,  /* [2892] */
    (xdc_Char)0x79,  /* [2893] */
    (xdc_Char)0x3a,  /* [2894] */
    (xdc_Char)0x20,  /* [2895] */
    (xdc_Char)0x25,  /* [2896] */
    (xdc_Char)0x64,  /* [2897] */
    (xdc_Char)0x0,  /* [2898] */
    (xdc_Char)0x2d,  /* [2899] */
    (xdc_Char)0x2d,  /* [2900] */
    (xdc_Char)0x3e,  /* [2901] */
    (xdc_Char)0x20,  /* [2902] */
    (xdc_Char)0x73,  /* [2903] */
    (xdc_Char)0x69,  /* [2904] */
    (xdc_Char)0x67,  /* [2905] */
    (xdc_Char)0x6e,  /* [2906] */
    (xdc_Char)0x61,  /* [2907] */
    (xdc_Char)0x6c,  /* [2908] */
    (xdc_Char)0x3a,  /* [2909] */
    (xdc_Char)0x20,  /* [2910] */
    (xdc_Char)0x28,  /* [2911] */
    (xdc_Char)0x25,  /* [2912] */
    (xdc_Char)0x70,  /* [2913] */
    (xdc_Char)0x29,  /* [2914] */
    (xdc_Char)0x0,  /* [2915] */
    (xdc_Char)0x3c,  /* [2916] */
    (xdc_Char)0x2d,  /* [2917] */
    (xdc_Char)0x2d,  /* [2918] */
    (xdc_Char)0x20,  /* [2919] */
    (xdc_Char)0x73,  /* [2920] */
    (xdc_Char)0x69,  /* [2921] */
    (xdc_Char)0x67,  /* [2922] */
    (xdc_Char)0x6e,  /* [2923] */
    (xdc_Char)0x61,  /* [2924] */
    (xdc_Char)0x6c,  /* [2925] */
    (xdc_Char)0x3a,  /* [2926] */
    (xdc_Char)0x20,  /* [2927] */
    (xdc_Char)0x76,  /* [2928] */
    (xdc_Char)0x6f,  /* [2929] */
    (xdc_Char)0x69,  /* [2930] */
    (xdc_Char)0x64,  /* [2931] */
    (xdc_Char)0x0,  /* [2932] */
    (xdc_Char)0x2d,  /* [2933] */
    (xdc_Char)0x2d,  /* [2934] */
    (xdc_Char)0x3e,  /* [2935] */
    (xdc_Char)0x20,  /* [2936] */
    (xdc_Char)0x77,  /* [2937] */
    (xdc_Char)0x61,  /* [2938] */
    (xdc_Char)0x69,  /* [2939] */
    (xdc_Char)0x74,  /* [2940] */
    (xdc_Char)0x3a,  /* [2941] */
    (xdc_Char)0x20,  /* [2942] */
    (xdc_Char)0x28,  /* [2943] */
    (xdc_Char)0x25,  /* [2944] */
    (xdc_Char)0x70,  /* [2945] */
    (xdc_Char)0x2c,  /* [2946] */
    (xdc_Char)0x20,  /* [2947] */
    (xdc_Char)0x30,  /* [2948] */
    (xdc_Char)0x78,  /* [2949] */
    (xdc_Char)0x25,  /* [2950] */
    (xdc_Char)0x78,  /* [2951] */
    (xdc_Char)0x2c,  /* [2952] */
    (xdc_Char)0x20,  /* [2953] */
    (xdc_Char)0x25,  /* [2954] */
    (xdc_Char)0x70,  /* [2955] */
    (xdc_Char)0x29,  /* [2956] */
    (xdc_Char)0x0,  /* [2957] */
    (xdc_Char)0x3c,  /* [2958] */
    (xdc_Char)0x2d,  /* [2959] */
    (xdc_Char)0x2d,  /* [2960] */
    (xdc_Char)0x20,  /* [2961] */
    (xdc_Char)0x77,  /* [2962] */
    (xdc_Char)0x61,  /* [2963] */
    (xdc_Char)0x69,  /* [2964] */
    (xdc_Char)0x74,  /* [2965] */
    (xdc_Char)0x3a,  /* [2966] */
    (xdc_Char)0x20,  /* [2967] */
    (xdc_Char)0x25,  /* [2968] */
    (xdc_Char)0x64,  /* [2969] */
    (xdc_Char)0x0,  /* [2970] */
    (xdc_Char)0x2d,  /* [2971] */
    (xdc_Char)0x2d,  /* [2972] */
    (xdc_Char)0x3e,  /* [2973] */
    (xdc_Char)0x20,  /* [2974] */
    (xdc_Char)0x71,  /* [2975] */
    (xdc_Char)0x75,  /* [2976] */
    (xdc_Char)0x65,  /* [2977] */
    (xdc_Char)0x72,  /* [2978] */
    (xdc_Char)0x79,  /* [2979] */
    (xdc_Char)0x3a,  /* [2980] */
    (xdc_Char)0x20,  /* [2981] */
    (xdc_Char)0x28,  /* [2982] */
    (xdc_Char)0x25,  /* [2983] */
    (xdc_Char)0x64,  /* [2984] */
    (xdc_Char)0x29,  /* [2985] */
    (xdc_Char)0x0,  /* [2986] */
    (xdc_Char)0x2d,  /* [2987] */
    (xdc_Char)0x2d,  /* [2988] */
    (xdc_Char)0x3e,  /* [2989] */
    (xdc_Char)0x20,  /* [2990] */
    (xdc_Char)0x67,  /* [2991] */
    (xdc_Char)0x65,  /* [2992] */
    (xdc_Char)0x74,  /* [2993] */
    (xdc_Char)0x52,  /* [2994] */
    (xdc_Char)0x65,  /* [2995] */
    (xdc_Char)0x66,  /* [2996] */
    (xdc_Char)0x65,  /* [2997] */
    (xdc_Char)0x72,  /* [2998] */
    (xdc_Char)0x65,  /* [2999] */
    (xdc_Char)0x6e,  /* [3000] */
    (xdc_Char)0x63,  /* [3001] */
    (xdc_Char)0x65,  /* [3002] */
    (xdc_Char)0x43,  /* [3003] */
    (xdc_Char)0x6f,  /* [3004] */
    (xdc_Char)0x75,  /* [3005] */
    (xdc_Char)0x6e,  /* [3006] */
    (xdc_Char)0x74,  /* [3007] */
    (xdc_Char)0x3a,  /* [3008] */
    (xdc_Char)0x20,  /* [3009] */
    (xdc_Char)0x28,  /* [3010] */
    (xdc_Char)0x25,  /* [3011] */
    (xdc_Char)0x70,  /* [3012] */
    (xdc_Char)0x2c,  /* [3013] */
    (xdc_Char)0x20,  /* [3014] */
    (xdc_Char)0x25,  /* [3015] */
    (xdc_Char)0x70,  /* [3016] */
    (xdc_Char)0x29,  /* [3017] */
    (xdc_Char)0x0,  /* [3018] */
    (xdc_Char)0x3c,  /* [3019] */
    (xdc_Char)0x2d,  /* [3020] */
    (xdc_Char)0x2d,  /* [3021] */
    (xdc_Char)0x20,  /* [3022] */
    (xdc_Char)0x67,  /* [3023] */
    (xdc_Char)0x65,  /* [3024] */
    (xdc_Char)0x74,  /* [3025] */
    (xdc_Char)0x52,  /* [3026] */
    (xdc_Char)0x65,  /* [3027] */
    (xdc_Char)0x66,  /* [3028] */
    (xdc_Char)0x65,  /* [3029] */
    (xdc_Char)0x72,  /* [3030] */
    (xdc_Char)0x65,  /* [3031] */
    (xdc_Char)0x6e,  /* [3032] */
    (xdc_Char)0x63,  /* [3033] */
    (xdc_Char)0x65,  /* [3034] */
    (xdc_Char)0x43,  /* [3035] */
    (xdc_Char)0x6f,  /* [3036] */
    (xdc_Char)0x75,  /* [3037] */
    (xdc_Char)0x6e,  /* [3038] */
    (xdc_Char)0x74,  /* [3039] */
    (xdc_Char)0x3a,  /* [3040] */
    (xdc_Char)0x20,  /* [3041] */
    (xdc_Char)0x25,  /* [3042] */
    (xdc_Char)0x64,  /* [3043] */
    (xdc_Char)0x0,  /* [3044] */
    (xdc_Char)0x2d,  /* [3045] */
    (xdc_Char)0x2d,  /* [3046] */
    (xdc_Char)0x2d,  /* [3047] */
    (xdc_Char)0x20,  /* [3048] */
    (xdc_Char)0x47,  /* [3049] */
    (xdc_Char)0x61,  /* [3050] */
    (xdc_Char)0x74,  /* [3051] */
    (xdc_Char)0x65,  /* [3052] */
    (xdc_Char)0x50,  /* [3053] */
    (xdc_Char)0x72,  /* [3054] */
    (xdc_Char)0x6f,  /* [3055] */
    (xdc_Char)0x63,  /* [3056] */
    (xdc_Char)0x65,  /* [3057] */
    (xdc_Char)0x73,  /* [3058] */
    (xdc_Char)0x73,  /* [3059] */
    (xdc_Char)0x28,  /* [3060] */
    (xdc_Char)0x25,  /* [3061] */
    (xdc_Char)0x64,  /* [3062] */
    (xdc_Char)0x29,  /* [3063] */
    (xdc_Char)0x20,  /* [3064] */
    (xdc_Char)0x61,  /* [3065] */
    (xdc_Char)0x6c,  /* [3066] */
    (xdc_Char)0x72,  /* [3067] */
    (xdc_Char)0x65,  /* [3068] */
    (xdc_Char)0x61,  /* [3069] */
    (xdc_Char)0x64,  /* [3070] */
    (xdc_Char)0x79,  /* [3071] */
    (xdc_Char)0x20,  /* [3072] */
    (xdc_Char)0x65,  /* [3073] */
    (xdc_Char)0x78,  /* [3074] */
    (xdc_Char)0x69,  /* [3075] */
    (xdc_Char)0x73,  /* [3076] */
    (xdc_Char)0x74,  /* [3077] */
    (xdc_Char)0x73,  /* [3078] */
    (xdc_Char)0x2c,  /* [3079] */
    (xdc_Char)0x20,  /* [3080] */
    (xdc_Char)0x69,  /* [3081] */
    (xdc_Char)0x6e,  /* [3082] */
    (xdc_Char)0x63,  /* [3083] */
    (xdc_Char)0x72,  /* [3084] */
    (xdc_Char)0x65,  /* [3085] */
    (xdc_Char)0x6d,  /* [3086] */
    (xdc_Char)0x65,  /* [3087] */
    (xdc_Char)0x6e,  /* [3088] */
    (xdc_Char)0x74,  /* [3089] */
    (xdc_Char)0x69,  /* [3090] */
    (xdc_Char)0x6e,  /* [3091] */
    (xdc_Char)0x67,  /* [3092] */
    (xdc_Char)0x20,  /* [3093] */
    (xdc_Char)0x72,  /* [3094] */
    (xdc_Char)0x65,  /* [3095] */
    (xdc_Char)0x66,  /* [3096] */
    (xdc_Char)0x65,  /* [3097] */
    (xdc_Char)0x72,  /* [3098] */
    (xdc_Char)0x65,  /* [3099] */
    (xdc_Char)0x6e,  /* [3100] */
    (xdc_Char)0x63,  /* [3101] */
    (xdc_Char)0x65,  /* [3102] */
    (xdc_Char)0x20,  /* [3103] */
    (xdc_Char)0x63,  /* [3104] */
    (xdc_Char)0x6f,  /* [3105] */
    (xdc_Char)0x75,  /* [3106] */
    (xdc_Char)0x6e,  /* [3107] */
    (xdc_Char)0x74,  /* [3108] */
    (xdc_Char)0x0,  /* [3109] */
    (xdc_Char)0x2d,  /* [3110] */
    (xdc_Char)0x2d,  /* [3111] */
    (xdc_Char)0x2d,  /* [3112] */
    (xdc_Char)0x20,  /* [3113] */
    (xdc_Char)0x47,  /* [3114] */
    (xdc_Char)0x61,  /* [3115] */
    (xdc_Char)0x74,  /* [3116] */
    (xdc_Char)0x65,  /* [3117] */
    (xdc_Char)0x50,  /* [3118] */
    (xdc_Char)0x72,  /* [3119] */
    (xdc_Char)0x6f,  /* [3120] */
    (xdc_Char)0x63,  /* [3121] */
    (xdc_Char)0x65,  /* [3122] */
    (xdc_Char)0x73,  /* [3123] */
    (xdc_Char)0x73,  /* [3124] */
    (xdc_Char)0x28,  /* [3125] */
    (xdc_Char)0x25,  /* [3126] */
    (xdc_Char)0x64,  /* [3127] */
    (xdc_Char)0x29,  /* [3128] */
    (xdc_Char)0x20,  /* [3129] */
    (xdc_Char)0x44,  /* [3130] */
    (xdc_Char)0x4e,  /* [3131] */
    (xdc_Char)0x45,  /* [3132] */
    (xdc_Char)0x2c,  /* [3133] */
    (xdc_Char)0x20,  /* [3134] */
    (xdc_Char)0x63,  /* [3135] */
    (xdc_Char)0x72,  /* [3136] */
    (xdc_Char)0x65,  /* [3137] */
    (xdc_Char)0x61,  /* [3138] */
    (xdc_Char)0x74,  /* [3139] */
    (xdc_Char)0x69,  /* [3140] */
    (xdc_Char)0x6e,  /* [3141] */
    (xdc_Char)0x67,  /* [3142] */
    (xdc_Char)0x0,  /* [3143] */
    (xdc_Char)0x2d,  /* [3144] */
    (xdc_Char)0x2d,  /* [3145] */
    (xdc_Char)0x2d,  /* [3146] */
    (xdc_Char)0x20,  /* [3147] */
    (xdc_Char)0x53,  /* [3148] */
    (xdc_Char)0x65,  /* [3149] */
    (xdc_Char)0x6d,  /* [3150] */
    (xdc_Char)0x50,  /* [3151] */
    (xdc_Char)0x72,  /* [3152] */
    (xdc_Char)0x6f,  /* [3153] */
    (xdc_Char)0x63,  /* [3154] */
    (xdc_Char)0x65,  /* [3155] */
    (xdc_Char)0x73,  /* [3156] */
    (xdc_Char)0x73,  /* [3157] */
    (xdc_Char)0x28,  /* [3158] */
    (xdc_Char)0x25,  /* [3159] */
    (xdc_Char)0x64,  /* [3160] */
    (xdc_Char)0x29,  /* [3161] */
    (xdc_Char)0x20,  /* [3162] */
    (xdc_Char)0x61,  /* [3163] */
    (xdc_Char)0x6c,  /* [3164] */
    (xdc_Char)0x72,  /* [3165] */
    (xdc_Char)0x65,  /* [3166] */
    (xdc_Char)0x61,  /* [3167] */
    (xdc_Char)0x64,  /* [3168] */
    (xdc_Char)0x79,  /* [3169] */
    (xdc_Char)0x20,  /* [3170] */
    (xdc_Char)0x65,  /* [3171] */
    (xdc_Char)0x78,  /* [3172] */
    (xdc_Char)0x69,  /* [3173] */
    (xdc_Char)0x73,  /* [3174] */
    (xdc_Char)0x74,  /* [3175] */
    (xdc_Char)0x73,  /* [3176] */
    (xdc_Char)0x2c,  /* [3177] */
    (xdc_Char)0x20,  /* [3178] */
    (xdc_Char)0x69,  /* [3179] */
    (xdc_Char)0x6e,  /* [3180] */
    (xdc_Char)0x63,  /* [3181] */
    (xdc_Char)0x72,  /* [3182] */
    (xdc_Char)0x65,  /* [3183] */
    (xdc_Char)0x6d,  /* [3184] */
    (xdc_Char)0x65,  /* [3185] */
    (xdc_Char)0x6e,  /* [3186] */
    (xdc_Char)0x74,  /* [3187] */
    (xdc_Char)0x69,  /* [3188] */
    (xdc_Char)0x6e,  /* [3189] */
    (xdc_Char)0x67,  /* [3190] */
    (xdc_Char)0x20,  /* [3191] */
    (xdc_Char)0x72,  /* [3192] */
    (xdc_Char)0x65,  /* [3193] */
    (xdc_Char)0x66,  /* [3194] */
    (xdc_Char)0x65,  /* [3195] */
    (xdc_Char)0x72,  /* [3196] */
    (xdc_Char)0x65,  /* [3197] */
    (xdc_Char)0x6e,  /* [3198] */
    (xdc_Char)0x63,  /* [3199] */
    (xdc_Char)0x65,  /* [3200] */
    (xdc_Char)0x20,  /* [3201] */
    (xdc_Char)0x63,  /* [3202] */
    (xdc_Char)0x6f,  /* [3203] */
    (xdc_Char)0x75,  /* [3204] */
    (xdc_Char)0x6e,  /* [3205] */
    (xdc_Char)0x74,  /* [3206] */
    (xdc_Char)0x0,  /* [3207] */
    (xdc_Char)0x2d,  /* [3208] */
    (xdc_Char)0x2d,  /* [3209] */
    (xdc_Char)0x2d,  /* [3210] */
    (xdc_Char)0x20,  /* [3211] */
    (xdc_Char)0x53,  /* [3212] */
    (xdc_Char)0x65,  /* [3213] */
    (xdc_Char)0x6d,  /* [3214] */
    (xdc_Char)0x50,  /* [3215] */
    (xdc_Char)0x72,  /* [3216] */
    (xdc_Char)0x6f,  /* [3217] */
    (xdc_Char)0x63,  /* [3218] */
    (xdc_Char)0x65,  /* [3219] */
    (xdc_Char)0x73,  /* [3220] */
    (xdc_Char)0x73,  /* [3221] */
    (xdc_Char)0x28,  /* [3222] */
    (xdc_Char)0x25,  /* [3223] */
    (xdc_Char)0x64,  /* [3224] */
    (xdc_Char)0x29,  /* [3225] */
    (xdc_Char)0x20,  /* [3226] */
    (xdc_Char)0x44,  /* [3227] */
    (xdc_Char)0x4e,  /* [3228] */
    (xdc_Char)0x45,  /* [3229] */
    (xdc_Char)0x2c,  /* [3230] */
    (xdc_Char)0x20,  /* [3231] */
    (xdc_Char)0x63,  /* [3232] */
    (xdc_Char)0x72,  /* [3233] */
    (xdc_Char)0x65,  /* [3234] */
    (xdc_Char)0x61,  /* [3235] */
    (xdc_Char)0x74,  /* [3236] */
    (xdc_Char)0x69,  /* [3237] */
    (xdc_Char)0x6e,  /* [3238] */
    (xdc_Char)0x67,  /* [3239] */
    (xdc_Char)0x0,  /* [3240] */
    (xdc_Char)0x3c,  /* [3241] */
    (xdc_Char)0x2d,  /* [3242] */
    (xdc_Char)0x2d,  /* [3243] */
    (xdc_Char)0x20,  /* [3244] */
    (xdc_Char)0x73,  /* [3245] */
    (xdc_Char)0x74,  /* [3246] */
    (xdc_Char)0x61,  /* [3247] */
    (xdc_Char)0x72,  /* [3248] */
    (xdc_Char)0x74,  /* [3249] */
    (xdc_Char)0x3a,  /* [3250] */
    (xdc_Char)0x20,  /* [3251] */
    (xdc_Char)0x28,  /* [3252] */
    (xdc_Char)0x25,  /* [3253] */
    (xdc_Char)0x70,  /* [3254] */
    (xdc_Char)0x29,  /* [3255] */
    (xdc_Char)0x0,  /* [3256] */
    (xdc_Char)0x2d,  /* [3257] */
    (xdc_Char)0x2d,  /* [3258] */
    (xdc_Char)0x3e,  /* [3259] */
    (xdc_Char)0x20,  /* [3260] */
    (xdc_Char)0x66,  /* [3261] */
    (xdc_Char)0x69,  /* [3262] */
    (xdc_Char)0x6e,  /* [3263] */
    (xdc_Char)0x69,  /* [3264] */
    (xdc_Char)0x73,  /* [3265] */
    (xdc_Char)0x68,  /* [3266] */
    (xdc_Char)0x3a,  /* [3267] */
    (xdc_Char)0x20,  /* [3268] */
    (xdc_Char)0x28,  /* [3269] */
    (xdc_Char)0x25,  /* [3270] */
    (xdc_Char)0x70,  /* [3271] */
    (xdc_Char)0x29,  /* [3272] */
    (xdc_Char)0x0,  /* [3273] */
    (xdc_Char)0x2d,  /* [3274] */
    (xdc_Char)0x2d,  /* [3275] */
    (xdc_Char)0x3e,  /* [3276] */
    (xdc_Char)0x20,  /* [3277] */
    (xdc_Char)0x6a,  /* [3278] */
    (xdc_Char)0x6f,  /* [3279] */
    (xdc_Char)0x69,  /* [3280] */
    (xdc_Char)0x6e,  /* [3281] */
    (xdc_Char)0x3a,  /* [3282] */
    (xdc_Char)0x20,  /* [3283] */
    (xdc_Char)0x28,  /* [3284] */
    (xdc_Char)0x25,  /* [3285] */
    (xdc_Char)0x70,  /* [3286] */
    (xdc_Char)0x29,  /* [3287] */
    (xdc_Char)0x0,  /* [3288] */
    (xdc_Char)0x2d,  /* [3289] */
    (xdc_Char)0x2d,  /* [3290] */
    (xdc_Char)0x3e,  /* [3291] */
    (xdc_Char)0x20,  /* [3292] */
    (xdc_Char)0x73,  /* [3293] */
    (xdc_Char)0x65,  /* [3294] */
    (xdc_Char)0x6c,  /* [3295] */
    (xdc_Char)0x66,  /* [3296] */
    (xdc_Char)0x3a,  /* [3297] */
    (xdc_Char)0x20,  /* [3298] */
    (xdc_Char)0x28,  /* [3299] */
    (xdc_Char)0x25,  /* [3300] */
    (xdc_Char)0x70,  /* [3301] */
    (xdc_Char)0x29,  /* [3302] */
    (xdc_Char)0x0,  /* [3303] */
    (xdc_Char)0x3c,  /* [3304] */
    (xdc_Char)0x2d,  /* [3305] */
    (xdc_Char)0x2d,  /* [3306] */
    (xdc_Char)0x20,  /* [3307] */
    (xdc_Char)0x73,  /* [3308] */
    (xdc_Char)0x65,  /* [3309] */
    (xdc_Char)0x6c,  /* [3310] */
    (xdc_Char)0x66,  /* [3311] */
    (xdc_Char)0x3a,  /* [3312] */
    (xdc_Char)0x20,  /* [3313] */
    (xdc_Char)0x25,  /* [3314] */
    (xdc_Char)0x70,  /* [3315] */
    (xdc_Char)0x0,  /* [3316] */
    (xdc_Char)0x2d,  /* [3317] */
    (xdc_Char)0x2d,  /* [3318] */
    (xdc_Char)0x3e,  /* [3319] */
    (xdc_Char)0x20,  /* [3320] */
    (xdc_Char)0x66,  /* [3321] */
    (xdc_Char)0x72,  /* [3322] */
    (xdc_Char)0x65,  /* [3323] */
    (xdc_Char)0x65,  /* [3324] */
    (xdc_Char)0x3a,  /* [3325] */
    (xdc_Char)0x20,  /* [3326] */
    (xdc_Char)0x28,  /* [3327] */
    (xdc_Char)0x25,  /* [3328] */
    (xdc_Char)0x70,  /* [3329] */
    (xdc_Char)0x2c,  /* [3330] */
    (xdc_Char)0x20,  /* [3331] */
    (xdc_Char)0x25,  /* [3332] */
    (xdc_Char)0x70,  /* [3333] */
    (xdc_Char)0x2c,  /* [3334] */
    (xdc_Char)0x20,  /* [3335] */
    (xdc_Char)0x30,  /* [3336] */
    (xdc_Char)0x78,  /* [3337] */
    (xdc_Char)0x25,  /* [3338] */
    (xdc_Char)0x78,  /* [3339] */
    (xdc_Char)0x29,  /* [3340] */
    (xdc_Char)0x0,  /* [3341] */
    (xdc_Char)0x3c,  /* [3342] */
    (xdc_Char)0x2d,  /* [3343] */
    (xdc_Char)0x2d,  /* [3344] */
    (xdc_Char)0x20,  /* [3345] */
    (xdc_Char)0x66,  /* [3346] */
    (xdc_Char)0x72,  /* [3347] */
    (xdc_Char)0x65,  /* [3348] */
    (xdc_Char)0x65,  /* [3349] */
    (xdc_Char)0x3a,  /* [3350] */
    (xdc_Char)0x20,  /* [3351] */
    (xdc_Char)0x76,  /* [3352] */
    (xdc_Char)0x6f,  /* [3353] */
    (xdc_Char)0x69,  /* [3354] */
    (xdc_Char)0x64,  /* [3355] */
    (xdc_Char)0x0,  /* [3356] */
    (xdc_Char)0x2d,  /* [3357] */
    (xdc_Char)0x2d,  /* [3358] */
    (xdc_Char)0x3e,  /* [3359] */
    (xdc_Char)0x20,  /* [3360] */
    (xdc_Char)0x67,  /* [3361] */
    (xdc_Char)0x65,  /* [3362] */
    (xdc_Char)0x74,  /* [3363] */
    (xdc_Char)0x53,  /* [3364] */
    (xdc_Char)0x74,  /* [3365] */
    (xdc_Char)0x61,  /* [3366] */
    (xdc_Char)0x74,  /* [3367] */
    (xdc_Char)0x73,  /* [3368] */
    (xdc_Char)0x3a,  /* [3369] */
    (xdc_Char)0x20,  /* [3370] */
    (xdc_Char)0x28,  /* [3371] */
    (xdc_Char)0x25,  /* [3372] */
    (xdc_Char)0x70,  /* [3373] */
    (xdc_Char)0x2c,  /* [3374] */
    (xdc_Char)0x20,  /* [3375] */
    (xdc_Char)0x25,  /* [3376] */
    (xdc_Char)0x70,  /* [3377] */
    (xdc_Char)0x29,  /* [3378] */
    (xdc_Char)0x0,  /* [3379] */
    (xdc_Char)0x3c,  /* [3380] */
    (xdc_Char)0x2d,  /* [3381] */
    (xdc_Char)0x2d,  /* [3382] */
    (xdc_Char)0x20,  /* [3383] */
    (xdc_Char)0x67,  /* [3384] */
    (xdc_Char)0x65,  /* [3385] */
    (xdc_Char)0x74,  /* [3386] */
    (xdc_Char)0x53,  /* [3387] */
    (xdc_Char)0x74,  /* [3388] */
    (xdc_Char)0x61,  /* [3389] */
    (xdc_Char)0x74,  /* [3390] */
    (xdc_Char)0x73,  /* [3391] */
    (xdc_Char)0x3a,  /* [3392] */
    (xdc_Char)0x20,  /* [3393] */
    (xdc_Char)0x76,  /* [3394] */
    (xdc_Char)0x6f,  /* [3395] */
    (xdc_Char)0x69,  /* [3396] */
    (xdc_Char)0x64,  /* [3397] */
    (xdc_Char)0x0,  /* [3398] */
    (xdc_Char)0x2d,  /* [3399] */
    (xdc_Char)0x2d,  /* [3400] */
    (xdc_Char)0x3e,  /* [3401] */
    (xdc_Char)0x20,  /* [3402] */
    (xdc_Char)0x61,  /* [3403] */
    (xdc_Char)0x6c,  /* [3404] */
    (xdc_Char)0x6c,  /* [3405] */
    (xdc_Char)0x6f,  /* [3406] */
    (xdc_Char)0x63,  /* [3407] */
    (xdc_Char)0x3a,  /* [3408] */
    (xdc_Char)0x20,  /* [3409] */
    (xdc_Char)0x28,  /* [3410] */
    (xdc_Char)0x25,  /* [3411] */
    (xdc_Char)0x70,  /* [3412] */
    (xdc_Char)0x2c,  /* [3413] */
    (xdc_Char)0x20,  /* [3414] */
    (xdc_Char)0x30,  /* [3415] */
    (xdc_Char)0x78,  /* [3416] */
    (xdc_Char)0x25,  /* [3417] */
    (xdc_Char)0x78,  /* [3418] */
    (xdc_Char)0x2c,  /* [3419] */
    (xdc_Char)0x20,  /* [3420] */
    (xdc_Char)0x30,  /* [3421] */
    (xdc_Char)0x78,  /* [3422] */
    (xdc_Char)0x25,  /* [3423] */
    (xdc_Char)0x78,  /* [3424] */
    (xdc_Char)0x2c,  /* [3425] */
    (xdc_Char)0x20,  /* [3426] */
    (xdc_Char)0x25,  /* [3427] */
    (xdc_Char)0x70,  /* [3428] */
    (xdc_Char)0x29,  /* [3429] */
    (xdc_Char)0x0,  /* [3430] */
    (xdc_Char)0x3c,  /* [3431] */
    (xdc_Char)0x2d,  /* [3432] */
    (xdc_Char)0x2d,  /* [3433] */
    (xdc_Char)0x20,  /* [3434] */
    (xdc_Char)0x61,  /* [3435] */
    (xdc_Char)0x6c,  /* [3436] */
    (xdc_Char)0x6c,  /* [3437] */
    (xdc_Char)0x6f,  /* [3438] */
    (xdc_Char)0x63,  /* [3439] */
    (xdc_Char)0x3a,  /* [3440] */
    (xdc_Char)0x20,  /* [3441] */
    (xdc_Char)0x25,  /* [3442] */
    (xdc_Char)0x70,  /* [3443] */
    (xdc_Char)0x0,  /* [3444] */
    (xdc_Char)0x2d,  /* [3445] */
    (xdc_Char)0x2d,  /* [3446] */
    (xdc_Char)0x3e,  /* [3447] */
    (xdc_Char)0x20,  /* [3448] */
    (xdc_Char)0x69,  /* [3449] */
    (xdc_Char)0x73,  /* [3450] */
    (xdc_Char)0x42,  /* [3451] */
    (xdc_Char)0x6c,  /* [3452] */
    (xdc_Char)0x6f,  /* [3453] */
    (xdc_Char)0x63,  /* [3454] */
    (xdc_Char)0x6b,  /* [3455] */
    (xdc_Char)0x69,  /* [3456] */
    (xdc_Char)0x6e,  /* [3457] */
    (xdc_Char)0x67,  /* [3458] */
    (xdc_Char)0x3a,  /* [3459] */
    (xdc_Char)0x20,  /* [3460] */
    (xdc_Char)0x28,  /* [3461] */
    (xdc_Char)0x25,  /* [3462] */
    (xdc_Char)0x70,  /* [3463] */
    (xdc_Char)0x29,  /* [3464] */
    (xdc_Char)0x0,  /* [3465] */
    (xdc_Char)0x3c,  /* [3466] */
    (xdc_Char)0x2d,  /* [3467] */
    (xdc_Char)0x2d,  /* [3468] */
    (xdc_Char)0x20,  /* [3469] */
    (xdc_Char)0x69,  /* [3470] */
    (xdc_Char)0x73,  /* [3471] */
    (xdc_Char)0x42,  /* [3472] */
    (xdc_Char)0x6c,  /* [3473] */
    (xdc_Char)0x6f,  /* [3474] */
    (xdc_Char)0x63,  /* [3475] */
    (xdc_Char)0x6b,  /* [3476] */
    (xdc_Char)0x69,  /* [3477] */
    (xdc_Char)0x6e,  /* [3478] */
    (xdc_Char)0x67,  /* [3479] */
    (xdc_Char)0x3a,  /* [3480] */
    (xdc_Char)0x20,  /* [3481] */
    (xdc_Char)0x25,  /* [3482] */
    (xdc_Char)0x64,  /* [3483] */
    (xdc_Char)0x0,  /* [3484] */
    (xdc_Char)0x2d,  /* [3485] */
    (xdc_Char)0x2d,  /* [3486] */
    (xdc_Char)0x3e,  /* [3487] */
    (xdc_Char)0x20,  /* [3488] */
    (xdc_Char)0x65,  /* [3489] */
    (xdc_Char)0x6e,  /* [3490] */
    (xdc_Char)0x61,  /* [3491] */
    (xdc_Char)0x62,  /* [3492] */
    (xdc_Char)0x6c,  /* [3493] */
    (xdc_Char)0x65,  /* [3494] */
    (xdc_Char)0x3a,  /* [3495] */
    (xdc_Char)0x20,  /* [3496] */
    (xdc_Char)0x28,  /* [3497] */
    (xdc_Char)0x25,  /* [3498] */
    (xdc_Char)0x70,  /* [3499] */
    (xdc_Char)0x29,  /* [3500] */
    (xdc_Char)0x0,  /* [3501] */
    (xdc_Char)0x3c,  /* [3502] */
    (xdc_Char)0x2d,  /* [3503] */
    (xdc_Char)0x2d,  /* [3504] */
    (xdc_Char)0x20,  /* [3505] */
    (xdc_Char)0x65,  /* [3506] */
    (xdc_Char)0x6e,  /* [3507] */
    (xdc_Char)0x61,  /* [3508] */
    (xdc_Char)0x62,  /* [3509] */
    (xdc_Char)0x6c,  /* [3510] */
    (xdc_Char)0x65,  /* [3511] */
    (xdc_Char)0x3a,  /* [3512] */
    (xdc_Char)0x20,  /* [3513] */
    (xdc_Char)0x25,  /* [3514] */
    (xdc_Char)0x64,  /* [3515] */
    (xdc_Char)0x0,  /* [3516] */
    (xdc_Char)0x2d,  /* [3517] */
    (xdc_Char)0x2d,  /* [3518] */
    (xdc_Char)0x3e,  /* [3519] */
    (xdc_Char)0x20,  /* [3520] */
    (xdc_Char)0x64,  /* [3521] */
    (xdc_Char)0x69,  /* [3522] */
    (xdc_Char)0x73,  /* [3523] */
    (xdc_Char)0x61,  /* [3524] */
    (xdc_Char)0x62,  /* [3525] */
    (xdc_Char)0x6c,  /* [3526] */
    (xdc_Char)0x65,  /* [3527] */
    (xdc_Char)0x3a,  /* [3528] */
    (xdc_Char)0x20,  /* [3529] */
    (xdc_Char)0x28,  /* [3530] */
    (xdc_Char)0x25,  /* [3531] */
    (xdc_Char)0x70,  /* [3532] */
    (xdc_Char)0x29,  /* [3533] */
    (xdc_Char)0x0,  /* [3534] */
    (xdc_Char)0x3c,  /* [3535] */
    (xdc_Char)0x2d,  /* [3536] */
    (xdc_Char)0x2d,  /* [3537] */
    (xdc_Char)0x20,  /* [3538] */
    (xdc_Char)0x64,  /* [3539] */
    (xdc_Char)0x69,  /* [3540] */
    (xdc_Char)0x73,  /* [3541] */
    (xdc_Char)0x61,  /* [3542] */
    (xdc_Char)0x62,  /* [3543] */
    (xdc_Char)0x6c,  /* [3544] */
    (xdc_Char)0x65,  /* [3545] */
    (xdc_Char)0x3a,  /* [3546] */
    (xdc_Char)0x20,  /* [3547] */
    (xdc_Char)0x25,  /* [3548] */
    (xdc_Char)0x64,  /* [3549] */
    (xdc_Char)0x0,  /* [3550] */
    (xdc_Char)0x2d,  /* [3551] */
    (xdc_Char)0x2d,  /* [3552] */
    (xdc_Char)0x3e,  /* [3553] */
    (xdc_Char)0x20,  /* [3554] */
    (xdc_Char)0x77,  /* [3555] */
    (xdc_Char)0x72,  /* [3556] */
    (xdc_Char)0x69,  /* [3557] */
    (xdc_Char)0x74,  /* [3558] */
    (xdc_Char)0x65,  /* [3559] */
    (xdc_Char)0x30,  /* [3560] */
    (xdc_Char)0x3a,  /* [3561] */
    (xdc_Char)0x20,  /* [3562] */
    (xdc_Char)0x28,  /* [3563] */
    (xdc_Char)0x25,  /* [3564] */
    (xdc_Char)0x70,  /* [3565] */
    (xdc_Char)0x29,  /* [3566] */
    (xdc_Char)0x0,  /* [3567] */
    (xdc_Char)0x3c,  /* [3568] */
    (xdc_Char)0x2d,  /* [3569] */
    (xdc_Char)0x2d,  /* [3570] */
    (xdc_Char)0x20,  /* [3571] */
    (xdc_Char)0x77,  /* [3572] */
    (xdc_Char)0x72,  /* [3573] */
    (xdc_Char)0x69,  /* [3574] */
    (xdc_Char)0x74,  /* [3575] */
    (xdc_Char)0x65,  /* [3576] */
    (xdc_Char)0x30,  /* [3577] */
    (xdc_Char)0x3a,  /* [3578] */
    (xdc_Char)0x20,  /* [3579] */
    (xdc_Char)0x76,  /* [3580] */
    (xdc_Char)0x6f,  /* [3581] */
    (xdc_Char)0x69,  /* [3582] */
    (xdc_Char)0x64,  /* [3583] */
    (xdc_Char)0x0,  /* [3584] */
    (xdc_Char)0x2d,  /* [3585] */
    (xdc_Char)0x2d,  /* [3586] */
    (xdc_Char)0x3e,  /* [3587] */
    (xdc_Char)0x20,  /* [3588] */
    (xdc_Char)0x77,  /* [3589] */
    (xdc_Char)0x72,  /* [3590] */
    (xdc_Char)0x69,  /* [3591] */
    (xdc_Char)0x74,  /* [3592] */
    (xdc_Char)0x65,  /* [3593] */
    (xdc_Char)0x31,  /* [3594] */
    (xdc_Char)0x3a,  /* [3595] */
    (xdc_Char)0x20,  /* [3596] */
    (xdc_Char)0x28,  /* [3597] */
    (xdc_Char)0x25,  /* [3598] */
    (xdc_Char)0x70,  /* [3599] */
    (xdc_Char)0x29,  /* [3600] */
    (xdc_Char)0x0,  /* [3601] */
    (xdc_Char)0x3c,  /* [3602] */
    (xdc_Char)0x2d,  /* [3603] */
    (xdc_Char)0x2d,  /* [3604] */
    (xdc_Char)0x20,  /* [3605] */
    (xdc_Char)0x77,  /* [3606] */
    (xdc_Char)0x72,  /* [3607] */
    (xdc_Char)0x69,  /* [3608] */
    (xdc_Char)0x74,  /* [3609] */
    (xdc_Char)0x65,  /* [3610] */
    (xdc_Char)0x31,  /* [3611] */
    (xdc_Char)0x3a,  /* [3612] */
    (xdc_Char)0x20,  /* [3613] */
    (xdc_Char)0x76,  /* [3614] */
    (xdc_Char)0x6f,  /* [3615] */
    (xdc_Char)0x69,  /* [3616] */
    (xdc_Char)0x64,  /* [3617] */
    (xdc_Char)0x0,  /* [3618] */
    (xdc_Char)0x2d,  /* [3619] */
    (xdc_Char)0x2d,  /* [3620] */
    (xdc_Char)0x3e,  /* [3621] */
    (xdc_Char)0x20,  /* [3622] */
    (xdc_Char)0x77,  /* [3623] */
    (xdc_Char)0x72,  /* [3624] */
    (xdc_Char)0x69,  /* [3625] */
    (xdc_Char)0x74,  /* [3626] */
    (xdc_Char)0x65,  /* [3627] */
    (xdc_Char)0x32,  /* [3628] */
    (xdc_Char)0x3a,  /* [3629] */
    (xdc_Char)0x20,  /* [3630] */
    (xdc_Char)0x28,  /* [3631] */
    (xdc_Char)0x25,  /* [3632] */
    (xdc_Char)0x70,  /* [3633] */
    (xdc_Char)0x29,  /* [3634] */
    (xdc_Char)0x0,  /* [3635] */
    (xdc_Char)0x3c,  /* [3636] */
    (xdc_Char)0x2d,  /* [3637] */
    (xdc_Char)0x2d,  /* [3638] */
    (xdc_Char)0x20,  /* [3639] */
    (xdc_Char)0x77,  /* [3640] */
    (xdc_Char)0x72,  /* [3641] */
    (xdc_Char)0x69,  /* [3642] */
    (xdc_Char)0x74,  /* [3643] */
    (xdc_Char)0x65,  /* [3644] */
    (xdc_Char)0x32,  /* [3645] */
    (xdc_Char)0x3a,  /* [3646] */
    (xdc_Char)0x20,  /* [3647] */
    (xdc_Char)0x76,  /* [3648] */
    (xdc_Char)0x6f,  /* [3649] */
    (xdc_Char)0x69,  /* [3650] */
    (xdc_Char)0x64,  /* [3651] */
    (xdc_Char)0x0,  /* [3652] */
    (xdc_Char)0x2d,  /* [3653] */
    (xdc_Char)0x2d,  /* [3654] */
    (xdc_Char)0x3e,  /* [3655] */
    (xdc_Char)0x20,  /* [3656] */
    (xdc_Char)0x77,  /* [3657] */
    (xdc_Char)0x72,  /* [3658] */
    (xdc_Char)0x69,  /* [3659] */
    (xdc_Char)0x74,  /* [3660] */
    (xdc_Char)0x65,  /* [3661] */
    (xdc_Char)0x34,  /* [3662] */
    (xdc_Char)0x3a,  /* [3663] */
    (xdc_Char)0x20,  /* [3664] */
    (xdc_Char)0x28,  /* [3665] */
    (xdc_Char)0x25,  /* [3666] */
    (xdc_Char)0x70,  /* [3667] */
    (xdc_Char)0x29,  /* [3668] */
    (xdc_Char)0x0,  /* [3669] */
    (xdc_Char)0x3c,  /* [3670] */
    (xdc_Char)0x2d,  /* [3671] */
    (xdc_Char)0x2d,  /* [3672] */
    (xdc_Char)0x20,  /* [3673] */
    (xdc_Char)0x77,  /* [3674] */
    (xdc_Char)0x72,  /* [3675] */
    (xdc_Char)0x69,  /* [3676] */
    (xdc_Char)0x74,  /* [3677] */
    (xdc_Char)0x65,  /* [3678] */
    (xdc_Char)0x34,  /* [3679] */
    (xdc_Char)0x3a,  /* [3680] */
    (xdc_Char)0x20,  /* [3681] */
    (xdc_Char)0x76,  /* [3682] */
    (xdc_Char)0x6f,  /* [3683] */
    (xdc_Char)0x69,  /* [3684] */
    (xdc_Char)0x64,  /* [3685] */
    (xdc_Char)0x0,  /* [3686] */
    (xdc_Char)0x2d,  /* [3687] */
    (xdc_Char)0x2d,  /* [3688] */
    (xdc_Char)0x3e,  /* [3689] */
    (xdc_Char)0x20,  /* [3690] */
    (xdc_Char)0x77,  /* [3691] */
    (xdc_Char)0x72,  /* [3692] */
    (xdc_Char)0x69,  /* [3693] */
    (xdc_Char)0x74,  /* [3694] */
    (xdc_Char)0x65,  /* [3695] */
    (xdc_Char)0x38,  /* [3696] */
    (xdc_Char)0x3a,  /* [3697] */
    (xdc_Char)0x20,  /* [3698] */
    (xdc_Char)0x28,  /* [3699] */
    (xdc_Char)0x25,  /* [3700] */
    (xdc_Char)0x70,  /* [3701] */
    (xdc_Char)0x29,  /* [3702] */
    (xdc_Char)0x0,  /* [3703] */
    (xdc_Char)0x3c,  /* [3704] */
    (xdc_Char)0x2d,  /* [3705] */
    (xdc_Char)0x2d,  /* [3706] */
    (xdc_Char)0x20,  /* [3707] */
    (xdc_Char)0x77,  /* [3708] */
    (xdc_Char)0x72,  /* [3709] */
    (xdc_Char)0x69,  /* [3710] */
    (xdc_Char)0x74,  /* [3711] */
    (xdc_Char)0x65,  /* [3712] */
    (xdc_Char)0x38,  /* [3713] */
    (xdc_Char)0x3a,  /* [3714] */
    (xdc_Char)0x20,  /* [3715] */
    (xdc_Char)0x76,  /* [3716] */
    (xdc_Char)0x6f,  /* [3717] */
    (xdc_Char)0x69,  /* [3718] */
    (xdc_Char)0x64,  /* [3719] */
    (xdc_Char)0x0,  /* [3720] */
    (xdc_Char)0x2d,  /* [3721] */
    (xdc_Char)0x2d,  /* [3722] */
    (xdc_Char)0x3e,  /* [3723] */
    (xdc_Char)0x20,  /* [3724] */
    (xdc_Char)0x73,  /* [3725] */
    (xdc_Char)0x68,  /* [3726] */
    (xdc_Char)0x75,  /* [3727] */
    (xdc_Char)0x74,  /* [3728] */
    (xdc_Char)0x64,  /* [3729] */
    (xdc_Char)0x6f,  /* [3730] */
    (xdc_Char)0x77,  /* [3731] */
    (xdc_Char)0x6e,  /* [3732] */
    (xdc_Char)0x3a,  /* [3733] */
    (xdc_Char)0x20,  /* [3734] */
    (xdc_Char)0x28,  /* [3735] */
    (xdc_Char)0x25,  /* [3736] */
    (xdc_Char)0x64,  /* [3737] */
    (xdc_Char)0x29,  /* [3738] */
    (xdc_Char)0x0,  /* [3739] */
    (xdc_Char)0x3c,  /* [3740] */
    (xdc_Char)0x2d,  /* [3741] */
    (xdc_Char)0x2d,  /* [3742] */
    (xdc_Char)0x20,  /* [3743] */
    (xdc_Char)0x73,  /* [3744] */
    (xdc_Char)0x68,  /* [3745] */
    (xdc_Char)0x75,  /* [3746] */
    (xdc_Char)0x74,  /* [3747] */
    (xdc_Char)0x64,  /* [3748] */
    (xdc_Char)0x6f,  /* [3749] */
    (xdc_Char)0x77,  /* [3750] */
    (xdc_Char)0x6e,  /* [3751] */
    (xdc_Char)0x3a,  /* [3752] */
    (xdc_Char)0x20,  /* [3753] */
    (xdc_Char)0x76,  /* [3754] */
    (xdc_Char)0x6f,  /* [3755] */
    (xdc_Char)0x69,  /* [3756] */
    (xdc_Char)0x64,  /* [3757] */
    (xdc_Char)0x0,  /* [3758] */
    (xdc_Char)0x78,  /* [3759] */
    (xdc_Char)0x64,  /* [3760] */
    (xdc_Char)0x63,  /* [3761] */
    (xdc_Char)0x2e,  /* [3762] */
    (xdc_Char)0x0,  /* [3763] */
    (xdc_Char)0x72,  /* [3764] */
    (xdc_Char)0x75,  /* [3765] */
    (xdc_Char)0x6e,  /* [3766] */
    (xdc_Char)0x74,  /* [3767] */
    (xdc_Char)0x69,  /* [3768] */
    (xdc_Char)0x6d,  /* [3769] */
    (xdc_Char)0x65,  /* [3770] */
    (xdc_Char)0x2e,  /* [3771] */
    (xdc_Char)0x0,  /* [3772] */
    (xdc_Char)0x41,  /* [3773] */
    (xdc_Char)0x73,  /* [3774] */
    (xdc_Char)0x73,  /* [3775] */
    (xdc_Char)0x65,  /* [3776] */
    (xdc_Char)0x72,  /* [3777] */
    (xdc_Char)0x74,  /* [3778] */
    (xdc_Char)0x0,  /* [3779] */
    (xdc_Char)0x43,  /* [3780] */
    (xdc_Char)0x6f,  /* [3781] */
    (xdc_Char)0x72,  /* [3782] */
    (xdc_Char)0x65,  /* [3783] */
    (xdc_Char)0x0,  /* [3784] */
    (xdc_Char)0x44,  /* [3785] */
    (xdc_Char)0x65,  /* [3786] */
    (xdc_Char)0x66,  /* [3787] */
    (xdc_Char)0x61,  /* [3788] */
    (xdc_Char)0x75,  /* [3789] */
    (xdc_Char)0x6c,  /* [3790] */
    (xdc_Char)0x74,  /* [3791] */
    (xdc_Char)0x73,  /* [3792] */
    (xdc_Char)0x0,  /* [3793] */
    (xdc_Char)0x44,  /* [3794] */
    (xdc_Char)0x69,  /* [3795] */
    (xdc_Char)0x61,  /* [3796] */
    (xdc_Char)0x67,  /* [3797] */
    (xdc_Char)0x73,  /* [3798] */
    (xdc_Char)0x0,  /* [3799] */
    (xdc_Char)0x45,  /* [3800] */
    (xdc_Char)0x72,  /* [3801] */
    (xdc_Char)0x72,  /* [3802] */
    (xdc_Char)0x6f,  /* [3803] */
    (xdc_Char)0x72,  /* [3804] */
    (xdc_Char)0x0,  /* [3805] */
    (xdc_Char)0x47,  /* [3806] */
    (xdc_Char)0x61,  /* [3807] */
    (xdc_Char)0x74,  /* [3808] */
    (xdc_Char)0x65,  /* [3809] */
    (xdc_Char)0x0,  /* [3810] */
    (xdc_Char)0x47,  /* [3811] */
    (xdc_Char)0x61,  /* [3812] */
    (xdc_Char)0x74,  /* [3813] */
    (xdc_Char)0x65,  /* [3814] */
    (xdc_Char)0x4e,  /* [3815] */
    (xdc_Char)0x75,  /* [3816] */
    (xdc_Char)0x6c,  /* [3817] */
    (xdc_Char)0x6c,  /* [3818] */
    (xdc_Char)0x0,  /* [3819] */
    (xdc_Char)0x4c,  /* [3820] */
    (xdc_Char)0x6f,  /* [3821] */
    (xdc_Char)0x67,  /* [3822] */
    (xdc_Char)0x0,  /* [3823] */
    (xdc_Char)0x4c,  /* [3824] */
    (xdc_Char)0x6f,  /* [3825] */
    (xdc_Char)0x67,  /* [3826] */
    (xdc_Char)0x67,  /* [3827] */
    (xdc_Char)0x65,  /* [3828] */
    (xdc_Char)0x72,  /* [3829] */
    (xdc_Char)0x53,  /* [3830] */
    (xdc_Char)0x79,  /* [3831] */
    (xdc_Char)0x73,  /* [3832] */
    (xdc_Char)0x0,  /* [3833] */
    (xdc_Char)0x4d,  /* [3834] */
    (xdc_Char)0x61,  /* [3835] */
    (xdc_Char)0x69,  /* [3836] */
    (xdc_Char)0x6e,  /* [3837] */
    (xdc_Char)0x0,  /* [3838] */
    (xdc_Char)0x4d,  /* [3839] */
    (xdc_Char)0x65,  /* [3840] */
    (xdc_Char)0x6d,  /* [3841] */
    (xdc_Char)0x6f,  /* [3842] */
    (xdc_Char)0x72,  /* [3843] */
    (xdc_Char)0x79,  /* [3844] */
    (xdc_Char)0x0,  /* [3845] */
    (xdc_Char)0x48,  /* [3846] */
    (xdc_Char)0x65,  /* [3847] */
    (xdc_Char)0x61,  /* [3848] */
    (xdc_Char)0x70,  /* [3849] */
    (xdc_Char)0x53,  /* [3850] */
    (xdc_Char)0x74,  /* [3851] */
    (xdc_Char)0x64,  /* [3852] */
    (xdc_Char)0x0,  /* [3853] */
    (xdc_Char)0x52,  /* [3854] */
    (xdc_Char)0x65,  /* [3855] */
    (xdc_Char)0x67,  /* [3856] */
    (xdc_Char)0x69,  /* [3857] */
    (xdc_Char)0x73,  /* [3858] */
    (xdc_Char)0x74,  /* [3859] */
    (xdc_Char)0x72,  /* [3860] */
    (xdc_Char)0x79,  /* [3861] */
    (xdc_Char)0x0,  /* [3862] */
    (xdc_Char)0x52,  /* [3863] */
    (xdc_Char)0x74,  /* [3864] */
    (xdc_Char)0x61,  /* [3865] */
    (xdc_Char)0x0,  /* [3866] */
    (xdc_Char)0x53,  /* [3867] */
    (xdc_Char)0x74,  /* [3868] */
    (xdc_Char)0x61,  /* [3869] */
    (xdc_Char)0x72,  /* [3870] */
    (xdc_Char)0x74,  /* [3871] */
    (xdc_Char)0x75,  /* [3872] */
    (xdc_Char)0x70,  /* [3873] */
    (xdc_Char)0x0,  /* [3874] */
    (xdc_Char)0x53,  /* [3875] */
    (xdc_Char)0x79,  /* [3876] */
    (xdc_Char)0x73,  /* [3877] */
    (xdc_Char)0x74,  /* [3878] */
    (xdc_Char)0x65,  /* [3879] */
    (xdc_Char)0x6d,  /* [3880] */
    (xdc_Char)0x0,  /* [3881] */
    (xdc_Char)0x53,  /* [3882] */
    (xdc_Char)0x79,  /* [3883] */
    (xdc_Char)0x73,  /* [3884] */
    (xdc_Char)0x53,  /* [3885] */
    (xdc_Char)0x74,  /* [3886] */
    (xdc_Char)0x64,  /* [3887] */
    (xdc_Char)0x0,  /* [3888] */
    (xdc_Char)0x54,  /* [3889] */
    (xdc_Char)0x65,  /* [3890] */
    (xdc_Char)0x78,  /* [3891] */
    (xdc_Char)0x74,  /* [3892] */
    (xdc_Char)0x0,  /* [3893] */
    (xdc_Char)0x54,  /* [3894] */
    (xdc_Char)0x69,  /* [3895] */
    (xdc_Char)0x6d,  /* [3896] */
    (xdc_Char)0x65,  /* [3897] */
    (xdc_Char)0x73,  /* [3898] */
    (xdc_Char)0x74,  /* [3899] */
    (xdc_Char)0x61,  /* [3900] */
    (xdc_Char)0x6d,  /* [3901] */
    (xdc_Char)0x70,  /* [3902] */
    (xdc_Char)0x0,  /* [3903] */
    (xdc_Char)0x6b,  /* [3904] */
    (xdc_Char)0x6e,  /* [3905] */
    (xdc_Char)0x6c,  /* [3906] */
    (xdc_Char)0x2e,  /* [3907] */
    (xdc_Char)0x0,  /* [3908] */
    (xdc_Char)0x47,  /* [3909] */
    (xdc_Char)0x61,  /* [3910] */
    (xdc_Char)0x74,  /* [3911] */
    (xdc_Char)0x65,  /* [3912] */
    (xdc_Char)0x48,  /* [3913] */
    (xdc_Char)0x0,  /* [3914] */
    (xdc_Char)0x53,  /* [3915] */
    (xdc_Char)0x65,  /* [3916] */
    (xdc_Char)0x6d,  /* [3917] */
    (xdc_Char)0x61,  /* [3918] */
    (xdc_Char)0x70,  /* [3919] */
    (xdc_Char)0x68,  /* [3920] */
    (xdc_Char)0x6f,  /* [3921] */
    (xdc_Char)0x72,  /* [3922] */
    (xdc_Char)0x65,  /* [3923] */
    (xdc_Char)0x0,  /* [3924] */
    (xdc_Char)0x54,  /* [3925] */
    (xdc_Char)0x68,  /* [3926] */
    (xdc_Char)0x72,  /* [3927] */
    (xdc_Char)0x65,  /* [3928] */
    (xdc_Char)0x61,  /* [3929] */
    (xdc_Char)0x64,  /* [3930] */
    (xdc_Char)0x0,  /* [3931] */
    (xdc_Char)0x53,  /* [3932] */
    (xdc_Char)0x79,  /* [3933] */
    (xdc_Char)0x6e,  /* [3934] */
    (xdc_Char)0x63,  /* [3935] */
    (xdc_Char)0x0,  /* [3936] */
    (xdc_Char)0x47,  /* [3937] */
    (xdc_Char)0x61,  /* [3938] */
    (xdc_Char)0x74,  /* [3939] */
    (xdc_Char)0x65,  /* [3940] */
    (xdc_Char)0x54,  /* [3941] */
    (xdc_Char)0x68,  /* [3942] */
    (xdc_Char)0x72,  /* [3943] */
    (xdc_Char)0x65,  /* [3944] */
    (xdc_Char)0x61,  /* [3945] */
    (xdc_Char)0x64,  /* [3946] */
    (xdc_Char)0x0,  /* [3947] */
    (xdc_Char)0x47,  /* [3948] */
    (xdc_Char)0x61,  /* [3949] */
    (xdc_Char)0x74,  /* [3950] */
    (xdc_Char)0x65,  /* [3951] */
    (xdc_Char)0x50,  /* [3952] */
    (xdc_Char)0x72,  /* [3953] */
    (xdc_Char)0x6f,  /* [3954] */
    (xdc_Char)0x63,  /* [3955] */
    (xdc_Char)0x65,  /* [3956] */
    (xdc_Char)0x73,  /* [3957] */
    (xdc_Char)0x73,  /* [3958] */
    (xdc_Char)0x0,  /* [3959] */
    (xdc_Char)0x53,  /* [3960] */
    (xdc_Char)0x65,  /* [3961] */
    (xdc_Char)0x6d,  /* [3962] */
    (xdc_Char)0x54,  /* [3963] */
    (xdc_Char)0x68,  /* [3964] */
    (xdc_Char)0x72,  /* [3965] */
    (xdc_Char)0x65,  /* [3966] */
    (xdc_Char)0x61,  /* [3967] */
    (xdc_Char)0x64,  /* [3968] */
    (xdc_Char)0x0,  /* [3969] */
    (xdc_Char)0x53,  /* [3970] */
    (xdc_Char)0x65,  /* [3971] */
    (xdc_Char)0x6d,  /* [3972] */
    (xdc_Char)0x50,  /* [3973] */
    (xdc_Char)0x72,  /* [3974] */
    (xdc_Char)0x6f,  /* [3975] */
    (xdc_Char)0x63,  /* [3976] */
    (xdc_Char)0x65,  /* [3977] */
    (xdc_Char)0x73,  /* [3978] */
    (xdc_Char)0x73,  /* [3979] */
    (xdc_Char)0x0,  /* [3980] */
    (xdc_Char)0x53,  /* [3981] */
    (xdc_Char)0x79,  /* [3982] */
    (xdc_Char)0x6e,  /* [3983] */
    (xdc_Char)0x63,  /* [3984] */
    (xdc_Char)0x47,  /* [3985] */
    (xdc_Char)0x65,  /* [3986] */
    (xdc_Char)0x6e,  /* [3987] */
    (xdc_Char)0x65,  /* [3988] */
    (xdc_Char)0x72,  /* [3989] */
    (xdc_Char)0x69,  /* [3990] */
    (xdc_Char)0x63,  /* [3991] */
    (xdc_Char)0x0,  /* [3992] */
    (xdc_Char)0x53,  /* [3993] */
    (xdc_Char)0x79,  /* [3994] */
    (xdc_Char)0x6e,  /* [3995] */
    (xdc_Char)0x63,  /* [3996] */
    (xdc_Char)0x4e,  /* [3997] */
    (xdc_Char)0x75,  /* [3998] */
    (xdc_Char)0x6c,  /* [3999] */
    (xdc_Char)0x6c,  /* [4000] */
    (xdc_Char)0x0,  /* [4001] */
    (xdc_Char)0x53,  /* [4002] */
    (xdc_Char)0x79,  /* [4003] */
    (xdc_Char)0x6e,  /* [4004] */
    (xdc_Char)0x63,  /* [4005] */
    (xdc_Char)0x53,  /* [4006] */
    (xdc_Char)0x65,  /* [4007] */
    (xdc_Char)0x6d,  /* [4008] */
    (xdc_Char)0x54,  /* [4009] */
    (xdc_Char)0x68,  /* [4010] */
    (xdc_Char)0x72,  /* [4011] */
    (xdc_Char)0x65,  /* [4012] */
    (xdc_Char)0x61,  /* [4013] */
    (xdc_Char)0x64,  /* [4014] */
    (xdc_Char)0x0,  /* [4015] */
    (xdc_Char)0x74,  /* [4016] */
    (xdc_Char)0x69,  /* [4017] */
    (xdc_Char)0x2e,  /* [4018] */
    (xdc_Char)0x0,  /* [4019] */
    (xdc_Char)0x73,  /* [4020] */
    (xdc_Char)0x64,  /* [4021] */
    (xdc_Char)0x6f,  /* [4022] */
    (xdc_Char)0x2e,  /* [4023] */
    (xdc_Char)0x0,  /* [4024] */
    (xdc_Char)0x78,  /* [4025] */
    (xdc_Char)0x64,  /* [4026] */
    (xdc_Char)0x63,  /* [4027] */
    (xdc_Char)0x72,  /* [4028] */
    (xdc_Char)0x75,  /* [4029] */
    (xdc_Char)0x6e,  /* [4030] */
    (xdc_Char)0x74,  /* [4031] */
    (xdc_Char)0x69,  /* [4032] */
    (xdc_Char)0x6d,  /* [4033] */
    (xdc_Char)0x65,  /* [4034] */
    (xdc_Char)0x2e,  /* [4035] */
    (xdc_Char)0x0,  /* [4036] */
    (xdc_Char)0x6c,  /* [4037] */
    (xdc_Char)0x69,  /* [4038] */
    (xdc_Char)0x6e,  /* [4039] */
    (xdc_Char)0x75,  /* [4040] */
    (xdc_Char)0x78,  /* [4041] */
    (xdc_Char)0x2e,  /* [4042] */
    (xdc_Char)0x0,  /* [4043] */
    (xdc_Char)0x47,  /* [4044] */
    (xdc_Char)0x61,  /* [4045] */
    (xdc_Char)0x74,  /* [4046] */
    (xdc_Char)0x65,  /* [4047] */
    (xdc_Char)0x54,  /* [4048] */
    (xdc_Char)0x68,  /* [4049] */
    (xdc_Char)0x72,  /* [4050] */
    (xdc_Char)0x65,  /* [4051] */
    (xdc_Char)0x61,  /* [4052] */
    (xdc_Char)0x64,  /* [4053] */
    (xdc_Char)0x53,  /* [4054] */
    (xdc_Char)0x75,  /* [4055] */
    (xdc_Char)0x70,  /* [4056] */
    (xdc_Char)0x70,  /* [4057] */
    (xdc_Char)0x6f,  /* [4058] */
    (xdc_Char)0x72,  /* [4059] */
    (xdc_Char)0x74,  /* [4060] */
    (xdc_Char)0x0,  /* [4061] */
    (xdc_Char)0x47,  /* [4062] */
    (xdc_Char)0x61,  /* [4063] */
    (xdc_Char)0x74,  /* [4064] */
    (xdc_Char)0x65,  /* [4065] */
    (xdc_Char)0x50,  /* [4066] */
    (xdc_Char)0x72,  /* [4067] */
    (xdc_Char)0x6f,  /* [4068] */
    (xdc_Char)0x63,  /* [4069] */
    (xdc_Char)0x65,  /* [4070] */
    (xdc_Char)0x73,  /* [4071] */
    (xdc_Char)0x73,  /* [4072] */
    (xdc_Char)0x53,  /* [4073] */
    (xdc_Char)0x75,  /* [4074] */
    (xdc_Char)0x70,  /* [4075] */
    (xdc_Char)0x70,  /* [4076] */
    (xdc_Char)0x6f,  /* [4077] */
    (xdc_Char)0x72,  /* [4078] */
    (xdc_Char)0x74,  /* [4079] */
    (xdc_Char)0x0,  /* [4080] */
    (xdc_Char)0x53,  /* [4081] */
    (xdc_Char)0x65,  /* [4082] */
    (xdc_Char)0x6d,  /* [4083] */
    (xdc_Char)0x54,  /* [4084] */
    (xdc_Char)0x68,  /* [4085] */
    (xdc_Char)0x72,  /* [4086] */
    (xdc_Char)0x65,  /* [4087] */
    (xdc_Char)0x61,  /* [4088] */
    (xdc_Char)0x64,  /* [4089] */
    (xdc_Char)0x53,  /* [4090] */
    (xdc_Char)0x75,  /* [4091] */
    (xdc_Char)0x70,  /* [4092] */
    (xdc_Char)0x70,  /* [4093] */
    (xdc_Char)0x6f,  /* [4094] */
    (xdc_Char)0x72,  /* [4095] */
    (xdc_Char)0x74,  /* [4096] */
    (xdc_Char)0x0,  /* [4097] */
    (xdc_Char)0x53,  /* [4098] */
    (xdc_Char)0x65,  /* [4099] */
    (xdc_Char)0x6d,  /* [4100] */
    (xdc_Char)0x50,  /* [4101] */
    (xdc_Char)0x72,  /* [4102] */
    (xdc_Char)0x6f,  /* [4103] */
    (xdc_Char)0x63,  /* [4104] */
    (xdc_Char)0x65,  /* [4105] */
    (xdc_Char)0x73,  /* [4106] */
    (xdc_Char)0x73,  /* [4107] */
    (xdc_Char)0x53,  /* [4108] */
    (xdc_Char)0x75,  /* [4109] */
    (xdc_Char)0x70,  /* [4110] */
    (xdc_Char)0x70,  /* [4111] */
    (xdc_Char)0x6f,  /* [4112] */
    (xdc_Char)0x72,  /* [4113] */
    (xdc_Char)0x74,  /* [4114] */
    (xdc_Char)0x0,  /* [4115] */
    (xdc_Char)0x54,  /* [4116] */
    (xdc_Char)0x68,  /* [4117] */
    (xdc_Char)0x72,  /* [4118] */
    (xdc_Char)0x65,  /* [4119] */
    (xdc_Char)0x61,  /* [4120] */
    (xdc_Char)0x64,  /* [4121] */
    (xdc_Char)0x53,  /* [4122] */
    (xdc_Char)0x75,  /* [4123] */
    (xdc_Char)0x70,  /* [4124] */
    (xdc_Char)0x70,  /* [4125] */
    (xdc_Char)0x6f,  /* [4126] */
    (xdc_Char)0x72,  /* [4127] */
    (xdc_Char)0x74,  /* [4128] */
    (xdc_Char)0x0,  /* [4129] */
    (xdc_Char)0x54,  /* [4130] */
    (xdc_Char)0x69,  /* [4131] */
    (xdc_Char)0x6d,  /* [4132] */
    (xdc_Char)0x65,  /* [4133] */
    (xdc_Char)0x73,  /* [4134] */
    (xdc_Char)0x74,  /* [4135] */
    (xdc_Char)0x61,  /* [4136] */
    (xdc_Char)0x6d,  /* [4137] */
    (xdc_Char)0x70,  /* [4138] */
    (xdc_Char)0x50,  /* [4139] */
    (xdc_Char)0x6f,  /* [4140] */
    (xdc_Char)0x73,  /* [4141] */
    (xdc_Char)0x69,  /* [4142] */
    (xdc_Char)0x78,  /* [4143] */
    (xdc_Char)0x0,  /* [4144] */
    (xdc_Char)0x68,  /* [4145] */
    (xdc_Char)0x65,  /* [4146] */
    (xdc_Char)0x61,  /* [4147] */
    (xdc_Char)0x70,  /* [4148] */
    (xdc_Char)0x73,  /* [4149] */
    (xdc_Char)0x2e,  /* [4150] */
    (xdc_Char)0x0,  /* [4151] */
    (xdc_Char)0x48,  /* [4152] */
    (xdc_Char)0x65,  /* [4153] */
    (xdc_Char)0x61,  /* [4154] */
    (xdc_Char)0x70,  /* [4155] */
    (xdc_Char)0x53,  /* [4156] */
    (xdc_Char)0x74,  /* [4157] */
    (xdc_Char)0x64,  /* [4158] */
    (xdc_Char)0x41,  /* [4159] */
    (xdc_Char)0x6c,  /* [4160] */
    (xdc_Char)0x69,  /* [4161] */
    (xdc_Char)0x67,  /* [4162] */
    (xdc_Char)0x6e,  /* [4163] */
    (xdc_Char)0x0,  /* [4164] */
    (xdc_Char)0x63,  /* [4165] */
    (xdc_Char)0x73,  /* [4166] */
    (xdc_Char)0x74,  /* [4167] */
    (xdc_Char)0x75,  /* [4168] */
    (xdc_Char)0x62,  /* [4169] */
    (xdc_Char)0x73,  /* [4170] */
    (xdc_Char)0x2e,  /* [4171] */
    (xdc_Char)0x0,  /* [4172] */
    (xdc_Char)0x4c,  /* [4173] */
    (xdc_Char)0x6f,  /* [4174] */
    (xdc_Char)0x67,  /* [4175] */
    (xdc_Char)0x67,  /* [4176] */
    (xdc_Char)0x65,  /* [4177] */
    (xdc_Char)0x72,  /* [4178] */
    (xdc_Char)0x46,  /* [4179] */
    (xdc_Char)0x6c,  /* [4180] */
    (xdc_Char)0x65,  /* [4181] */
    (xdc_Char)0x78,  /* [4182] */
    (xdc_Char)0x0,  /* [4183] */
};

/* --> xdc_runtime_Text_nodeTab__A */
const __T1_xdc_runtime_Text_nodeTab xdc_runtime_Text_nodeTab__A[46] = {
    {
        (xdc_Bits16)0x0,  /* left */
        (xdc_Bits16)0x0,  /* right */
    },  /* [0] */
    {
        (xdc_Bits16)0xeaf,  /* left */
        (xdc_Bits16)0xeb4,  /* right */
    },  /* [1] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xebd,  /* right */
    },  /* [2] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xec4,  /* right */
    },  /* [3] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xec9,  /* right */
    },  /* [4] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xed2,  /* right */
    },  /* [5] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xed8,  /* right */
    },  /* [6] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xede,  /* right */
    },  /* [7] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xee3,  /* right */
    },  /* [8] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xeec,  /* right */
    },  /* [9] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xef0,  /* right */
    },  /* [10] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xefa,  /* right */
    },  /* [11] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xeff,  /* right */
    },  /* [12] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xf06,  /* right */
    },  /* [13] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xf0e,  /* right */
    },  /* [14] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xf17,  /* right */
    },  /* [15] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xf1b,  /* right */
    },  /* [16] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xf23,  /* right */
    },  /* [17] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xf2a,  /* right */
    },  /* [18] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xf31,  /* right */
    },  /* [19] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xf36,  /* right */
    },  /* [20] */
    {
        (xdc_Bits16)0x8001,  /* left */
        (xdc_Bits16)0xf40,  /* right */
    },  /* [21] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xf45,  /* right */
    },  /* [22] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xf4b,  /* right */
    },  /* [23] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xf55,  /* right */
    },  /* [24] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xf5c,  /* right */
    },  /* [25] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xf61,  /* right */
    },  /* [26] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xf6c,  /* right */
    },  /* [27] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xf78,  /* right */
    },  /* [28] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xf82,  /* right */
    },  /* [29] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xf8d,  /* right */
    },  /* [30] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xf99,  /* right */
    },  /* [31] */
    {
        (xdc_Bits16)0x8015,  /* left */
        (xdc_Bits16)0xfa2,  /* right */
    },  /* [32] */
    {
        (xdc_Bits16)0xfb0,  /* left */
        (xdc_Bits16)0xfb4,  /* right */
    },  /* [33] */
    {
        (xdc_Bits16)0x8021,  /* left */
        (xdc_Bits16)0xfb9,  /* right */
    },  /* [34] */
    {
        (xdc_Bits16)0x8022,  /* left */
        (xdc_Bits16)0xfc5,  /* right */
    },  /* [35] */
    {
        (xdc_Bits16)0x8023,  /* left */
        (xdc_Bits16)0xfcc,  /* right */
    },  /* [36] */
    {
        (xdc_Bits16)0x8023,  /* left */
        (xdc_Bits16)0xfde,  /* right */
    },  /* [37] */
    {
        (xdc_Bits16)0x8023,  /* left */
        (xdc_Bits16)0xff1,  /* right */
    },  /* [38] */
    {
        (xdc_Bits16)0x8023,  /* left */
        (xdc_Bits16)0x1002,  /* right */
    },  /* [39] */
    {
        (xdc_Bits16)0x8023,  /* left */
        (xdc_Bits16)0x1014,  /* right */
    },  /* [40] */
    {
        (xdc_Bits16)0x8023,  /* left */
        (xdc_Bits16)0x1022,  /* right */
    },  /* [41] */
    {
        (xdc_Bits16)0xfb9,  /* left */
        (xdc_Bits16)0x1031,  /* right */
    },  /* [42] */
    {
        (xdc_Bits16)0x802a,  /* left */
        (xdc_Bits16)0x1038,  /* right */
    },  /* [43] */
    {
        (xdc_Bits16)0xfb9,  /* left */
        (xdc_Bits16)0x1045,  /* right */
    },  /* [44] */
    {
        (xdc_Bits16)0x802c,  /* left */
        (xdc_Bits16)0x104d,  /* right */
    },  /* [45] */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Text_Module__diagsEnabled xdc_runtime_Text_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Text_Module__diagsIncluded xdc_runtime_Text_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Text_Module__diagsMask xdc_runtime_Text_Module__diagsMask__C = ((CT__xdc_runtime_Text_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Text_Module__gateObj xdc_runtime_Text_Module__gateObj__C = ((CT__xdc_runtime_Text_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Text_Module__gatePrms xdc_runtime_Text_Module__gatePrms__C = ((CT__xdc_runtime_Text_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Text_Module__id xdc_runtime_Text_Module__id__C = (xdc_Bits16)0x8013;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Text_Module__loggerDefined xdc_runtime_Text_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Text_Module__loggerObj xdc_runtime_Text_Module__loggerObj__C = ((CT__xdc_runtime_Text_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Text_Module__loggerFxn0 xdc_runtime_Text_Module__loggerFxn0__C = ((CT__xdc_runtime_Text_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Text_Module__loggerFxn1 xdc_runtime_Text_Module__loggerFxn1__C = ((CT__xdc_runtime_Text_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Text_Module__loggerFxn2 xdc_runtime_Text_Module__loggerFxn2__C = ((CT__xdc_runtime_Text_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Text_Module__loggerFxn4 xdc_runtime_Text_Module__loggerFxn4__C = ((CT__xdc_runtime_Text_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Text_Module__loggerFxn8 xdc_runtime_Text_Module__loggerFxn8__C = ((CT__xdc_runtime_Text_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Text_Module__startupDoneFxn xdc_runtime_Text_Module__startupDoneFxn__C = ((CT__xdc_runtime_Text_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Text_Object__count xdc_runtime_Text_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Text_Object__heap xdc_runtime_Text_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Text_Object__sizeof xdc_runtime_Text_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Text_Object__table xdc_runtime_Text_Object__table__C = 0;

/* nameUnknown__C */
__FAR__ const CT__xdc_runtime_Text_nameUnknown xdc_runtime_Text_nameUnknown__C = "{unknown-instance-name}";

/* nameEmpty__C */
__FAR__ const CT__xdc_runtime_Text_nameEmpty xdc_runtime_Text_nameEmpty__C = "{empty-instance-name}";

/* nameStatic__C */
__FAR__ const CT__xdc_runtime_Text_nameStatic xdc_runtime_Text_nameStatic__C = "{static-instance-name}";

/* isLoaded__C */
__FAR__ const CT__xdc_runtime_Text_isLoaded xdc_runtime_Text_isLoaded__C = 1;

/* charTab__C */
__FAR__ const CT__xdc_runtime_Text_charTab xdc_runtime_Text_charTab__C = ((CT__xdc_runtime_Text_charTab)xdc_runtime_Text_charTab__A);

/* nodeTab__C */
__FAR__ const CT__xdc_runtime_Text_nodeTab xdc_runtime_Text_nodeTab__C = ((CT__xdc_runtime_Text_nodeTab)xdc_runtime_Text_nodeTab__A);

/* charCnt__C */
__FAR__ const CT__xdc_runtime_Text_charCnt xdc_runtime_Text_charCnt__C = (xdc_Int16)0x1058;

/* nodeCnt__C */
__FAR__ const CT__xdc_runtime_Text_nodeCnt xdc_runtime_Text_nodeCnt__C = (xdc_Int16)0x2e;

/* unnamedModsLastId__C */
__FAR__ const CT__xdc_runtime_Text_unnamedModsLastId xdc_runtime_Text_unnamedModsLastId__C = (xdc_UInt16)0x4000;

/* registryModsLastId__C */
__FAR__ const CT__xdc_runtime_Text_registryModsLastId xdc_runtime_Text_registryModsLastId__C = (xdc_UInt16)0x7fff;

/* visitRopeFxn__C */
__FAR__ const CT__xdc_runtime_Text_visitRopeFxn xdc_runtime_Text_visitRopeFxn__C = ((CT__xdc_runtime_Text_visitRopeFxn)((xdc_Fxn)xdc_runtime_Text_visitRope__I));

/* visitRopeFxn2__C */
__FAR__ const CT__xdc_runtime_Text_visitRopeFxn2 xdc_runtime_Text_visitRopeFxn2__C = ((CT__xdc_runtime_Text_visitRopeFxn2)((xdc_Fxn)xdc_runtime_Text_visitRope2__I));


/*
 * ======== xdc.runtime.Timestamp INITIALIZERS ========
 */

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__diagsEnabled xdc_runtime_Timestamp_Module__diagsEnabled__C = (xdc_Bits32)0x10;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__diagsIncluded xdc_runtime_Timestamp_Module__diagsIncluded__C = (xdc_Bits32)0x10;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__diagsMask xdc_runtime_Timestamp_Module__diagsMask__C = ((CT__xdc_runtime_Timestamp_Module__diagsMask)0);

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__gateObj xdc_runtime_Timestamp_Module__gateObj__C = ((CT__xdc_runtime_Timestamp_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__gatePrms xdc_runtime_Timestamp_Module__gatePrms__C = ((CT__xdc_runtime_Timestamp_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__id xdc_runtime_Timestamp_Module__id__C = (xdc_Bits16)0x8014;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__loggerDefined xdc_runtime_Timestamp_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__loggerObj xdc_runtime_Timestamp_Module__loggerObj__C = ((CT__xdc_runtime_Timestamp_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__loggerFxn0 xdc_runtime_Timestamp_Module__loggerFxn0__C = ((CT__xdc_runtime_Timestamp_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__loggerFxn1 xdc_runtime_Timestamp_Module__loggerFxn1__C = ((CT__xdc_runtime_Timestamp_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__loggerFxn2 xdc_runtime_Timestamp_Module__loggerFxn2__C = ((CT__xdc_runtime_Timestamp_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__loggerFxn4 xdc_runtime_Timestamp_Module__loggerFxn4__C = ((CT__xdc_runtime_Timestamp_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__loggerFxn8 xdc_runtime_Timestamp_Module__loggerFxn8__C = ((CT__xdc_runtime_Timestamp_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_Timestamp_Module__startupDoneFxn xdc_runtime_Timestamp_Module__startupDoneFxn__C = ((CT__xdc_runtime_Timestamp_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_Timestamp_Object__count xdc_runtime_Timestamp_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_Timestamp_Object__heap xdc_runtime_Timestamp_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_Timestamp_Object__sizeof xdc_runtime_Timestamp_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_Timestamp_Object__table xdc_runtime_Timestamp_Object__table__C = 0;


/*
 * ======== xdc.runtime.Timestamp_SupportProxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.knl.GateH INITIALIZERS ========
 */

/* Module__root__V */
xdc_runtime_knl_GateH_Module__ xdc_runtime_knl_GateH_Module__root__V = {
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__diagsEnabled xdc_runtime_knl_GateH_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__diagsIncluded xdc_runtime_knl_GateH_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__diagsMask xdc_runtime_knl_GateH_Module__diagsMask__C = ((CT__xdc_runtime_knl_GateH_Module__diagsMask)((void*)&xdc_runtime_knl_GateH_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__gateObj xdc_runtime_knl_GateH_Module__gateObj__C = ((CT__xdc_runtime_knl_GateH_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__gatePrms xdc_runtime_knl_GateH_Module__gatePrms__C = ((CT__xdc_runtime_knl_GateH_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__id xdc_runtime_knl_GateH_Module__id__C = (xdc_Bits16)0x8016;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__loggerDefined xdc_runtime_knl_GateH_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__loggerObj xdc_runtime_knl_GateH_Module__loggerObj__C = ((CT__xdc_runtime_knl_GateH_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__loggerFxn0 xdc_runtime_knl_GateH_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_GateH_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__loggerFxn1 xdc_runtime_knl_GateH_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_GateH_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__loggerFxn2 xdc_runtime_knl_GateH_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_GateH_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__loggerFxn4 xdc_runtime_knl_GateH_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_GateH_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__loggerFxn8 xdc_runtime_knl_GateH_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_GateH_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Module__startupDoneFxn xdc_runtime_knl_GateH_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_GateH_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Object__count xdc_runtime_knl_GateH_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Object__heap xdc_runtime_knl_GateH_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Object__sizeof xdc_runtime_knl_GateH_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_GateH_Object__table xdc_runtime_knl_GateH_Object__table__C = 0;


/*
 * ======== xdc.runtime.knl.GateH_Proxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.knl.GateProcess INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_GateProcess_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_knl_GateProcess_Params xdc_runtime_knl_GateProcess_Object__PARAMS__C = {
    sizeof (xdc_runtime_knl_GateProcess_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_knl_GateProcess_Object__PARAMS__C.__iprms, /* instance */
    (xdc_Int)(-0x0 - 1),  /* key */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_knl_GateProcess_Module__ xdc_runtime_knl_GateProcess_Module__root__V = {
    {&xdc_runtime_knl_GateProcess_Module__root__V.link,  /* link.next */
    &xdc_runtime_knl_GateProcess_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__diagsEnabled xdc_runtime_knl_GateProcess_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__diagsIncluded xdc_runtime_knl_GateProcess_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__diagsMask xdc_runtime_knl_GateProcess_Module__diagsMask__C = ((CT__xdc_runtime_knl_GateProcess_Module__diagsMask)((void*)&xdc_runtime_knl_GateProcess_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__gateObj xdc_runtime_knl_GateProcess_Module__gateObj__C = ((CT__xdc_runtime_knl_GateProcess_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__gatePrms xdc_runtime_knl_GateProcess_Module__gatePrms__C = ((CT__xdc_runtime_knl_GateProcess_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__id xdc_runtime_knl_GateProcess_Module__id__C = (xdc_Bits16)0x801b;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__loggerDefined xdc_runtime_knl_GateProcess_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__loggerObj xdc_runtime_knl_GateProcess_Module__loggerObj__C = ((CT__xdc_runtime_knl_GateProcess_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__loggerFxn0 xdc_runtime_knl_GateProcess_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_GateProcess_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__loggerFxn1 xdc_runtime_knl_GateProcess_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_GateProcess_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__loggerFxn2 xdc_runtime_knl_GateProcess_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_GateProcess_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__loggerFxn4 xdc_runtime_knl_GateProcess_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_GateProcess_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__loggerFxn8 xdc_runtime_knl_GateProcess_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_GateProcess_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Module__startupDoneFxn xdc_runtime_knl_GateProcess_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_GateProcess_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Object__count xdc_runtime_knl_GateProcess_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Object__heap xdc_runtime_knl_GateProcess_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Object__sizeof xdc_runtime_knl_GateProcess_Object__sizeof__C = sizeof(xdc_runtime_knl_GateProcess_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_Object__table xdc_runtime_knl_GateProcess_Object__table__C = 0;

/* A_invalidKey__C */
__FAR__ const CT__xdc_runtime_knl_GateProcess_A_invalidKey xdc_runtime_knl_GateProcess_A_invalidKey__C = (((xdc_runtime_Assert_Id)398) << 16 | 16);


/*
 * ======== xdc.runtime.knl.GateProcess_Proxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.knl.GateThread INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_GateThread_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_knl_GateThread_Params xdc_runtime_knl_GateThread_Object__PARAMS__C = {
    sizeof (xdc_runtime_knl_GateThread_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_knl_GateThread_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_knl_GateThread_Module__ xdc_runtime_knl_GateThread_Module__root__V = {
    {&xdc_runtime_knl_GateThread_Module__root__V.link,  /* link.next */
    &xdc_runtime_knl_GateThread_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__diagsEnabled xdc_runtime_knl_GateThread_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__diagsIncluded xdc_runtime_knl_GateThread_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__diagsMask xdc_runtime_knl_GateThread_Module__diagsMask__C = ((CT__xdc_runtime_knl_GateThread_Module__diagsMask)((void*)&xdc_runtime_knl_GateThread_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__gateObj xdc_runtime_knl_GateThread_Module__gateObj__C = ((CT__xdc_runtime_knl_GateThread_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__gatePrms xdc_runtime_knl_GateThread_Module__gatePrms__C = ((CT__xdc_runtime_knl_GateThread_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__id xdc_runtime_knl_GateThread_Module__id__C = (xdc_Bits16)0x801a;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__loggerDefined xdc_runtime_knl_GateThread_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__loggerObj xdc_runtime_knl_GateThread_Module__loggerObj__C = ((CT__xdc_runtime_knl_GateThread_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__loggerFxn0 xdc_runtime_knl_GateThread_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_GateThread_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__loggerFxn1 xdc_runtime_knl_GateThread_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_GateThread_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__loggerFxn2 xdc_runtime_knl_GateThread_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_GateThread_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__loggerFxn4 xdc_runtime_knl_GateThread_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_GateThread_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__loggerFxn8 xdc_runtime_knl_GateThread_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_GateThread_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Module__startupDoneFxn xdc_runtime_knl_GateThread_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_GateThread_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Object__count xdc_runtime_knl_GateThread_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Object__heap xdc_runtime_knl_GateThread_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Object__sizeof xdc_runtime_knl_GateThread_Object__sizeof__C = sizeof(xdc_runtime_knl_GateThread_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_GateThread_Object__table xdc_runtime_knl_GateThread_Object__table__C = 0;


/*
 * ======== xdc.runtime.knl.GateThread_Proxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.knl.SemProcess INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_SemProcess_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_knl_SemProcess_Params xdc_runtime_knl_SemProcess_Object__PARAMS__C = {
    sizeof (xdc_runtime_knl_SemProcess_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_knl_SemProcess_Object__PARAMS__C.__iprms, /* instance */
    xdc_runtime_knl_ISemaphore_Mode_COUNTING,  /* mode */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_knl_SemProcess_Module__ xdc_runtime_knl_SemProcess_Module__root__V = {
    {&xdc_runtime_knl_SemProcess_Module__root__V.link,  /* link.next */
    &xdc_runtime_knl_SemProcess_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__diagsEnabled xdc_runtime_knl_SemProcess_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__diagsIncluded xdc_runtime_knl_SemProcess_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__diagsMask xdc_runtime_knl_SemProcess_Module__diagsMask__C = ((CT__xdc_runtime_knl_SemProcess_Module__diagsMask)((void*)&xdc_runtime_knl_SemProcess_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__gateObj xdc_runtime_knl_SemProcess_Module__gateObj__C = ((CT__xdc_runtime_knl_SemProcess_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__gatePrms xdc_runtime_knl_SemProcess_Module__gatePrms__C = ((CT__xdc_runtime_knl_SemProcess_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__id xdc_runtime_knl_SemProcess_Module__id__C = (xdc_Bits16)0x801d;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__loggerDefined xdc_runtime_knl_SemProcess_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__loggerObj xdc_runtime_knl_SemProcess_Module__loggerObj__C = ((CT__xdc_runtime_knl_SemProcess_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__loggerFxn0 xdc_runtime_knl_SemProcess_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_SemProcess_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__loggerFxn1 xdc_runtime_knl_SemProcess_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_SemProcess_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__loggerFxn2 xdc_runtime_knl_SemProcess_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_SemProcess_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__loggerFxn4 xdc_runtime_knl_SemProcess_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_SemProcess_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__loggerFxn8 xdc_runtime_knl_SemProcess_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_SemProcess_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Module__startupDoneFxn xdc_runtime_knl_SemProcess_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_SemProcess_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Object__count xdc_runtime_knl_SemProcess_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Object__heap xdc_runtime_knl_SemProcess_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Object__sizeof xdc_runtime_knl_SemProcess_Object__sizeof__C = sizeof(xdc_runtime_knl_SemProcess_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_SemProcess_Object__table xdc_runtime_knl_SemProcess_Object__table__C = 0;


/*
 * ======== xdc.runtime.knl.SemProcess_Proxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.knl.SemThread INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_SemThread_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_knl_SemThread_Params xdc_runtime_knl_SemThread_Object__PARAMS__C = {
    sizeof (xdc_runtime_knl_SemThread_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_knl_SemThread_Object__PARAMS__C.__iprms, /* instance */
    xdc_runtime_knl_ISemaphore_Mode_COUNTING,  /* mode */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_knl_SemThread_Module__ xdc_runtime_knl_SemThread_Module__root__V = {
    {&xdc_runtime_knl_SemThread_Module__root__V.link,  /* link.next */
    &xdc_runtime_knl_SemThread_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__diagsEnabled xdc_runtime_knl_SemThread_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__diagsIncluded xdc_runtime_knl_SemThread_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__diagsMask xdc_runtime_knl_SemThread_Module__diagsMask__C = ((CT__xdc_runtime_knl_SemThread_Module__diagsMask)((void*)&xdc_runtime_knl_SemThread_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__gateObj xdc_runtime_knl_SemThread_Module__gateObj__C = ((CT__xdc_runtime_knl_SemThread_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__gatePrms xdc_runtime_knl_SemThread_Module__gatePrms__C = ((CT__xdc_runtime_knl_SemThread_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__id xdc_runtime_knl_SemThread_Module__id__C = (xdc_Bits16)0x801c;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__loggerDefined xdc_runtime_knl_SemThread_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__loggerObj xdc_runtime_knl_SemThread_Module__loggerObj__C = ((CT__xdc_runtime_knl_SemThread_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__loggerFxn0 xdc_runtime_knl_SemThread_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_SemThread_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__loggerFxn1 xdc_runtime_knl_SemThread_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_SemThread_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__loggerFxn2 xdc_runtime_knl_SemThread_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_SemThread_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__loggerFxn4 xdc_runtime_knl_SemThread_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_SemThread_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__loggerFxn8 xdc_runtime_knl_SemThread_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_SemThread_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Module__startupDoneFxn xdc_runtime_knl_SemThread_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_SemThread_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Object__count xdc_runtime_knl_SemThread_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Object__heap xdc_runtime_knl_SemThread_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Object__sizeof xdc_runtime_knl_SemThread_Object__sizeof__C = sizeof(xdc_runtime_knl_SemThread_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_SemThread_Object__table xdc_runtime_knl_SemThread_Object__table__C = 0;


/*
 * ======== xdc.runtime.knl.SemThread_Proxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.knl.Semaphore INITIALIZERS ========
 */

/* Module__root__V */
xdc_runtime_knl_Semaphore_Module__ xdc_runtime_knl_Semaphore_Module__root__V = {
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__diagsEnabled xdc_runtime_knl_Semaphore_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__diagsIncluded xdc_runtime_knl_Semaphore_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__diagsMask xdc_runtime_knl_Semaphore_Module__diagsMask__C = ((CT__xdc_runtime_knl_Semaphore_Module__diagsMask)((void*)&xdc_runtime_knl_Semaphore_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__gateObj xdc_runtime_knl_Semaphore_Module__gateObj__C = ((CT__xdc_runtime_knl_Semaphore_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__gatePrms xdc_runtime_knl_Semaphore_Module__gatePrms__C = ((CT__xdc_runtime_knl_Semaphore_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__id xdc_runtime_knl_Semaphore_Module__id__C = (xdc_Bits16)0x8017;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__loggerDefined xdc_runtime_knl_Semaphore_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__loggerObj xdc_runtime_knl_Semaphore_Module__loggerObj__C = ((CT__xdc_runtime_knl_Semaphore_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__loggerFxn0 xdc_runtime_knl_Semaphore_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_Semaphore_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__loggerFxn1 xdc_runtime_knl_Semaphore_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_Semaphore_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__loggerFxn2 xdc_runtime_knl_Semaphore_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_Semaphore_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__loggerFxn4 xdc_runtime_knl_Semaphore_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_Semaphore_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__loggerFxn8 xdc_runtime_knl_Semaphore_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_Semaphore_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Module__startupDoneFxn xdc_runtime_knl_Semaphore_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_Semaphore_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Object__count xdc_runtime_knl_Semaphore_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Object__heap xdc_runtime_knl_Semaphore_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Object__sizeof xdc_runtime_knl_Semaphore_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_Semaphore_Object__table xdc_runtime_knl_Semaphore_Object__table__C = 0;


/*
 * ======== xdc.runtime.knl.Semaphore_Proxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.knl.Sync INITIALIZERS ========
 */

/* Module__root__V */
xdc_runtime_knl_Sync_Module__ xdc_runtime_knl_Sync_Module__root__V = {
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__diagsEnabled xdc_runtime_knl_Sync_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__diagsIncluded xdc_runtime_knl_Sync_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__diagsMask xdc_runtime_knl_Sync_Module__diagsMask__C = ((CT__xdc_runtime_knl_Sync_Module__diagsMask)((void*)&xdc_runtime_knl_Sync_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__gateObj xdc_runtime_knl_Sync_Module__gateObj__C = ((CT__xdc_runtime_knl_Sync_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__gatePrms xdc_runtime_knl_Sync_Module__gatePrms__C = ((CT__xdc_runtime_knl_Sync_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__id xdc_runtime_knl_Sync_Module__id__C = (xdc_Bits16)0x8019;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__loggerDefined xdc_runtime_knl_Sync_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__loggerObj xdc_runtime_knl_Sync_Module__loggerObj__C = ((CT__xdc_runtime_knl_Sync_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__loggerFxn0 xdc_runtime_knl_Sync_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_Sync_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__loggerFxn1 xdc_runtime_knl_Sync_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_Sync_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__loggerFxn2 xdc_runtime_knl_Sync_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_Sync_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__loggerFxn4 xdc_runtime_knl_Sync_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_Sync_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__loggerFxn8 xdc_runtime_knl_Sync_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_Sync_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Module__startupDoneFxn xdc_runtime_knl_Sync_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_Sync_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Object__count xdc_runtime_knl_Sync_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Object__heap xdc_runtime_knl_Sync_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Object__sizeof xdc_runtime_knl_Sync_Object__sizeof__C = 0;

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_Sync_Object__table xdc_runtime_knl_Sync_Object__table__C = 0;


/*
 * ======== xdc.runtime.knl.SyncGeneric INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_SyncGeneric_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_knl_SyncGeneric_Params xdc_runtime_knl_SyncGeneric_Object__PARAMS__C = {
    sizeof (xdc_runtime_knl_SyncGeneric_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_knl_SyncGeneric_Object__PARAMS__C.__iprms, /* instance */
    ((xdc_Void(*)(xdc_UArg))0),  /* userSignal */
    ((xdc_UArg)0),  /* signalArg */
    ((xdc_Bool(*)(xdc_UArg,xdc_UInt))0),  /* userWait */
    ((xdc_UArg)0),  /* waitArg */
    ((xdc_Bool(*)(xdc_Int))0),  /* userQuery */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_knl_SyncGeneric_Module__ xdc_runtime_knl_SyncGeneric_Module__root__V = {
    {&xdc_runtime_knl_SyncGeneric_Module__root__V.link,  /* link.next */
    &xdc_runtime_knl_SyncGeneric_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__diagsEnabled xdc_runtime_knl_SyncGeneric_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__diagsIncluded xdc_runtime_knl_SyncGeneric_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__diagsMask xdc_runtime_knl_SyncGeneric_Module__diagsMask__C = ((CT__xdc_runtime_knl_SyncGeneric_Module__diagsMask)((void*)&xdc_runtime_knl_SyncGeneric_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__gateObj xdc_runtime_knl_SyncGeneric_Module__gateObj__C = ((CT__xdc_runtime_knl_SyncGeneric_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__gatePrms xdc_runtime_knl_SyncGeneric_Module__gatePrms__C = ((CT__xdc_runtime_knl_SyncGeneric_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__id xdc_runtime_knl_SyncGeneric_Module__id__C = (xdc_Bits16)0x801e;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__loggerDefined xdc_runtime_knl_SyncGeneric_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__loggerObj xdc_runtime_knl_SyncGeneric_Module__loggerObj__C = ((CT__xdc_runtime_knl_SyncGeneric_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn0 xdc_runtime_knl_SyncGeneric_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn1 xdc_runtime_knl_SyncGeneric_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn2 xdc_runtime_knl_SyncGeneric_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn4 xdc_runtime_knl_SyncGeneric_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn8 xdc_runtime_knl_SyncGeneric_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Module__startupDoneFxn xdc_runtime_knl_SyncGeneric_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_SyncGeneric_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Object__count xdc_runtime_knl_SyncGeneric_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Object__heap xdc_runtime_knl_SyncGeneric_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Object__sizeof xdc_runtime_knl_SyncGeneric_Object__sizeof__C = sizeof(xdc_runtime_knl_SyncGeneric_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_SyncGeneric_Object__table xdc_runtime_knl_SyncGeneric_Object__table__C = 0;


/*
 * ======== xdc.runtime.knl.SyncNull INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_SyncNull_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_knl_SyncNull_Params xdc_runtime_knl_SyncNull_Object__PARAMS__C = {
    sizeof (xdc_runtime_knl_SyncNull_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_knl_SyncNull_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_knl_SyncNull_Module__ xdc_runtime_knl_SyncNull_Module__root__V = {
    {&xdc_runtime_knl_SyncNull_Module__root__V.link,  /* link.next */
    &xdc_runtime_knl_SyncNull_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__diagsEnabled xdc_runtime_knl_SyncNull_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__diagsIncluded xdc_runtime_knl_SyncNull_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__diagsMask xdc_runtime_knl_SyncNull_Module__diagsMask__C = ((CT__xdc_runtime_knl_SyncNull_Module__diagsMask)((void*)&xdc_runtime_knl_SyncNull_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__gateObj xdc_runtime_knl_SyncNull_Module__gateObj__C = ((CT__xdc_runtime_knl_SyncNull_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__gatePrms xdc_runtime_knl_SyncNull_Module__gatePrms__C = ((CT__xdc_runtime_knl_SyncNull_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__id xdc_runtime_knl_SyncNull_Module__id__C = (xdc_Bits16)0x801f;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__loggerDefined xdc_runtime_knl_SyncNull_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__loggerObj xdc_runtime_knl_SyncNull_Module__loggerObj__C = ((CT__xdc_runtime_knl_SyncNull_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__loggerFxn0 xdc_runtime_knl_SyncNull_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_SyncNull_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__loggerFxn1 xdc_runtime_knl_SyncNull_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_SyncNull_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__loggerFxn2 xdc_runtime_knl_SyncNull_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_SyncNull_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__loggerFxn4 xdc_runtime_knl_SyncNull_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_SyncNull_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__loggerFxn8 xdc_runtime_knl_SyncNull_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_SyncNull_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Module__startupDoneFxn xdc_runtime_knl_SyncNull_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_SyncNull_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Object__count xdc_runtime_knl_SyncNull_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Object__heap xdc_runtime_knl_SyncNull_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Object__sizeof xdc_runtime_knl_SyncNull_Object__sizeof__C = sizeof(xdc_runtime_knl_SyncNull_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_SyncNull_Object__table xdc_runtime_knl_SyncNull_Object__table__C = 0;


/*
 * ======== xdc.runtime.knl.SyncSemThread INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_SyncSemThread_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_knl_SyncSemThread_Params xdc_runtime_knl_SyncSemThread_Object__PARAMS__C = {
    sizeof (xdc_runtime_knl_SyncSemThread_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_knl_SyncSemThread_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_knl_SyncSemThread_Module__ xdc_runtime_knl_SyncSemThread_Module__root__V = {
    {&xdc_runtime_knl_SyncSemThread_Module__root__V.link,  /* link.next */
    &xdc_runtime_knl_SyncSemThread_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__diagsEnabled xdc_runtime_knl_SyncSemThread_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__diagsIncluded xdc_runtime_knl_SyncSemThread_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__diagsMask xdc_runtime_knl_SyncSemThread_Module__diagsMask__C = ((CT__xdc_runtime_knl_SyncSemThread_Module__diagsMask)((void*)&xdc_runtime_knl_SyncSemThread_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__gateObj xdc_runtime_knl_SyncSemThread_Module__gateObj__C = ((CT__xdc_runtime_knl_SyncSemThread_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__gatePrms xdc_runtime_knl_SyncSemThread_Module__gatePrms__C = ((CT__xdc_runtime_knl_SyncSemThread_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__id xdc_runtime_knl_SyncSemThread_Module__id__C = (xdc_Bits16)0x8020;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__loggerDefined xdc_runtime_knl_SyncSemThread_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__loggerObj xdc_runtime_knl_SyncSemThread_Module__loggerObj__C = ((CT__xdc_runtime_knl_SyncSemThread_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn0 xdc_runtime_knl_SyncSemThread_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn1 xdc_runtime_knl_SyncSemThread_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn2 xdc_runtime_knl_SyncSemThread_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn4 xdc_runtime_knl_SyncSemThread_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn8 xdc_runtime_knl_SyncSemThread_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Module__startupDoneFxn xdc_runtime_knl_SyncSemThread_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_SyncSemThread_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Object__count xdc_runtime_knl_SyncSemThread_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Object__heap xdc_runtime_knl_SyncSemThread_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Object__sizeof xdc_runtime_knl_SyncSemThread_Object__sizeof__C = sizeof(xdc_runtime_knl_SyncSemThread_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_SyncSemThread_Object__table xdc_runtime_knl_SyncSemThread_Object__table__C = 0;


/*
 * ======== xdc.runtime.knl.Sync_Proxy INITIALIZERS ========
 */


/*
 * ======== xdc.runtime.knl.Thread INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_Thread_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdc_runtime_knl_Thread_Params xdc_runtime_knl_Thread_Object__PARAMS__C = {
    sizeof (xdc_runtime_knl_Thread_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdc_runtime_knl_Thread_Object__PARAMS__C.__iprms, /* instance */
    ((xdc_IArg)(0x0)),  /* arg */
    xdc_runtime_knl_Thread_Priority_NORMAL,  /* priority */
    (xdc_Int)0x0,  /* osPriority */
    (xdc_SizeT)0x0,  /* stackSize */
    ((xdc_Ptr)0),  /* tls */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdc_runtime_knl_Thread_Module__ xdc_runtime_knl_Thread_Module__root__V = {
    {&xdc_runtime_knl_Thread_Module__root__V.link,  /* link.next */
    &xdc_runtime_knl_Thread_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__diagsEnabled xdc_runtime_knl_Thread_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__diagsIncluded xdc_runtime_knl_Thread_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__diagsMask xdc_runtime_knl_Thread_Module__diagsMask__C = ((CT__xdc_runtime_knl_Thread_Module__diagsMask)((void*)&xdc_runtime_knl_Thread_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__gateObj xdc_runtime_knl_Thread_Module__gateObj__C = ((CT__xdc_runtime_knl_Thread_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__gatePrms xdc_runtime_knl_Thread_Module__gatePrms__C = ((CT__xdc_runtime_knl_Thread_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__id xdc_runtime_knl_Thread_Module__id__C = (xdc_Bits16)0x8018;

/* Module__loggerDefined__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__loggerDefined xdc_runtime_knl_Thread_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__loggerObj xdc_runtime_knl_Thread_Module__loggerObj__C = ((CT__xdc_runtime_knl_Thread_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__loggerFxn0 xdc_runtime_knl_Thread_Module__loggerFxn0__C = ((CT__xdc_runtime_knl_Thread_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__loggerFxn1 xdc_runtime_knl_Thread_Module__loggerFxn1__C = ((CT__xdc_runtime_knl_Thread_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__loggerFxn2 xdc_runtime_knl_Thread_Module__loggerFxn2__C = ((CT__xdc_runtime_knl_Thread_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__loggerFxn4 xdc_runtime_knl_Thread_Module__loggerFxn4__C = ((CT__xdc_runtime_knl_Thread_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__loggerFxn8 xdc_runtime_knl_Thread_Module__loggerFxn8__C = ((CT__xdc_runtime_knl_Thread_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Module__startupDoneFxn xdc_runtime_knl_Thread_Module__startupDoneFxn__C = ((CT__xdc_runtime_knl_Thread_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Object__count xdc_runtime_knl_Thread_Object__count__C = 0;

/* Object__heap__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Object__heap xdc_runtime_knl_Thread_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Object__sizeof xdc_runtime_knl_Thread_Object__sizeof__C = sizeof(xdc_runtime_knl_Thread_Object__);

/* Object__table__C */
__FAR__ const CT__xdc_runtime_knl_Thread_Object__table xdc_runtime_knl_Thread_Object__table__C = 0;

/* A_zeroTimeout__C */
__FAR__ const CT__xdc_runtime_knl_Thread_A_zeroTimeout xdc_runtime_knl_Thread_A_zeroTimeout__C = (((xdc_runtime_Assert_Id)355) << 16 | 16);

/* defaultStackSize__C */
__FAR__ const CT__xdc_runtime_knl_Thread_defaultStackSize xdc_runtime_knl_Thread_defaultStackSize__C = (xdc_SizeT)0x0;


/*
 * ======== xdc.runtime.knl.Thread_Proxy INITIALIZERS ========
 */


/*
 * ======== xdcruntime.cstubs.LoggerFlex INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdcruntime_cstubs_LoggerFlex_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdcruntime_cstubs_LoggerFlex_Params xdcruntime_cstubs_LoggerFlex_Object__PARAMS__C = {
    sizeof (xdcruntime_cstubs_LoggerFlex_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdcruntime_cstubs_LoggerFlex_Object__PARAMS__C.__iprms, /* instance */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdcruntime_cstubs_LoggerFlex_Module__ xdcruntime_cstubs_LoggerFlex_Module__root__V = {
    {&xdcruntime_cstubs_LoggerFlex_Module__root__V.link,  /* link.next */
    &xdcruntime_cstubs_LoggerFlex_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Object__table__V */
xdcruntime_cstubs_LoggerFlex_Object__ xdcruntime_cstubs_LoggerFlex_Object__table__V[1] = {
    {/* instance#0 */
        &xdcruntime_cstubs_LoggerFlex_Module__FXNS__C,
    },
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__diagsEnabled xdcruntime_cstubs_LoggerFlex_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__diagsIncluded xdcruntime_cstubs_LoggerFlex_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__diagsMask xdcruntime_cstubs_LoggerFlex_Module__diagsMask__C = ((CT__xdcruntime_cstubs_LoggerFlex_Module__diagsMask)((void*)&xdcruntime_cstubs_LoggerFlex_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__gateObj xdcruntime_cstubs_LoggerFlex_Module__gateObj__C = ((CT__xdcruntime_cstubs_LoggerFlex_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__gatePrms xdcruntime_cstubs_LoggerFlex_Module__gatePrms__C = ((CT__xdcruntime_cstubs_LoggerFlex_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__id xdcruntime_cstubs_LoggerFlex_Module__id__C = (xdc_Bits16)0x802d;

/* Module__loggerDefined__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerDefined xdcruntime_cstubs_LoggerFlex_Module__loggerDefined__C = 0;

/* Module__loggerObj__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerObj xdcruntime_cstubs_LoggerFlex_Module__loggerObj__C = ((CT__xdcruntime_cstubs_LoggerFlex_Module__loggerObj)0);

/* Module__loggerFxn0__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn0 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn0__C = ((CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn0)0);

/* Module__loggerFxn1__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn1 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn1__C = ((CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn1)0);

/* Module__loggerFxn2__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn2 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn2__C = ((CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn2)0);

/* Module__loggerFxn4__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn4 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn4__C = ((CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn4)0);

/* Module__loggerFxn8__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn8 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn8__C = ((CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn8)0);

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__startupDoneFxn xdcruntime_cstubs_LoggerFlex_Module__startupDoneFxn__C = ((CT__xdcruntime_cstubs_LoggerFlex_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Object__count xdcruntime_cstubs_LoggerFlex_Object__count__C = 1;

/* Object__heap__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Object__heap xdcruntime_cstubs_LoggerFlex_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Object__sizeof xdcruntime_cstubs_LoggerFlex_Object__sizeof__C = sizeof(xdcruntime_cstubs_LoggerFlex_Object__);

/* Object__table__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Object__table xdcruntime_cstubs_LoggerFlex_Object__table__C = xdcruntime_cstubs_LoggerFlex_Object__table__V;

/* E_atexit__C */
__FAR__ const CT__xdcruntime_cstubs_LoggerFlex_E_atexit xdcruntime_cstubs_LoggerFlex_E_atexit__C = (((xdc_runtime_Error_Id)1135) << 16 | 0);


/*
 * ======== xdcruntime.heaps.HeapStdAlign INITIALIZERS ========
 */

/* Object__DESC__C */
__FAR__ const xdc_runtime_Core_ObjDesc xdcruntime_heaps_HeapStdAlign_Object__DESC__C;

/* Object__PARAMS__C */
__FAR__ const xdcruntime_heaps_HeapStdAlign_Params xdcruntime_heaps_HeapStdAlign_Object__PARAMS__C = {
    sizeof (xdcruntime_heaps_HeapStdAlign_Params), /* __size */
    0, /* __self */
    0, /* __fxns */
    (xdc_runtime_IInstance_Params*)&xdcruntime_heaps_HeapStdAlign_Object__PARAMS__C.__iprms, /* instance */
    ((xdc_UArg)(0x0)),  /* size */
    {
        sizeof (xdc_runtime_IInstance_Params), /* __size */
        0,  /* name */
    },  /* instance */
};

/* Module__root__V */
xdcruntime_heaps_HeapStdAlign_Module__ xdcruntime_heaps_HeapStdAlign_Module__root__V = {
    {&xdcruntime_heaps_HeapStdAlign_Module__root__V.link,  /* link.next */
    &xdcruntime_heaps_HeapStdAlign_Module__root__V.link},  /* link.prev */
    0,  /* mask */
};

/* Object__table__V */
xdcruntime_heaps_HeapStdAlign_Object__ xdcruntime_heaps_HeapStdAlign_Object__table__V[1] = {
    {/* instance#0 */
        &xdcruntime_heaps_HeapStdAlign_Module__FXNS__C,
        ((xdc_UArg)(0xffffffff)),  /* remainSize */
        ((xdc_UArg)(0xffffffff)),  /* startSize */
    },
};

/* Module__state__V */
xdcruntime_heaps_HeapStdAlign_Module_State__ xdcruntime_heaps_HeapStdAlign_Module__state__V = {
    ((xdc_UArg)(0x0)),  /* remainRTSSize */
};

/* Module__diagsEnabled__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__diagsEnabled xdcruntime_heaps_HeapStdAlign_Module__diagsEnabled__C = (xdc_Bits32)0x98;

/* Module__diagsIncluded__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__diagsIncluded xdcruntime_heaps_HeapStdAlign_Module__diagsIncluded__C = (xdc_Bits32)0xff9f;

/* Module__diagsMask__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__diagsMask xdcruntime_heaps_HeapStdAlign_Module__diagsMask__C = ((CT__xdcruntime_heaps_HeapStdAlign_Module__diagsMask)((void*)&xdcruntime_heaps_HeapStdAlign_Module__root__V.mask));

/* Module__gateObj__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__gateObj xdcruntime_heaps_HeapStdAlign_Module__gateObj__C = ((CT__xdcruntime_heaps_HeapStdAlign_Module__gateObj)0);

/* Module__gatePrms__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__gatePrms xdcruntime_heaps_HeapStdAlign_Module__gatePrms__C = ((CT__xdcruntime_heaps_HeapStdAlign_Module__gatePrms)0);

/* Module__id__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__id xdcruntime_heaps_HeapStdAlign_Module__id__C = (xdc_Bits16)0x802b;

/* Module__loggerDefined__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerDefined xdcruntime_heaps_HeapStdAlign_Module__loggerDefined__C = 1;

/* Module__loggerObj__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerObj xdcruntime_heaps_HeapStdAlign_Module__loggerObj__C = ((CT__xdcruntime_heaps_HeapStdAlign_Module__loggerObj)((void*)(xdc_runtime_ILogger_Handle)&xdcruntime_cstubs_LoggerFlex_Object__table__V[0]));

/* Module__loggerFxn0__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn0 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn0__C = ((CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn0)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write0));

/* Module__loggerFxn1__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn1 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn1__C = ((CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn1)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write1));

/* Module__loggerFxn2__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn2 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn2__C = ((CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn2)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write2));

/* Module__loggerFxn4__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn4 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn4__C = ((CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn4)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write4));

/* Module__loggerFxn8__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn8 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn8__C = ((CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn8)((xdc_Fxn)xdcruntime_cstubs_LoggerFlex_write8));

/* Module__startupDoneFxn__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__startupDoneFxn xdcruntime_heaps_HeapStdAlign_Module__startupDoneFxn__C = ((CT__xdcruntime_heaps_HeapStdAlign_Module__startupDoneFxn)0);

/* Object__count__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Object__count xdcruntime_heaps_HeapStdAlign_Object__count__C = 1;

/* Object__heap__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Object__heap xdcruntime_heaps_HeapStdAlign_Object__heap__C = 0;

/* Object__sizeof__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Object__sizeof xdcruntime_heaps_HeapStdAlign_Object__sizeof__C = sizeof(xdcruntime_heaps_HeapStdAlign_Object__);

/* Object__table__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Object__table xdcruntime_heaps_HeapStdAlign_Object__table__C = xdcruntime_heaps_HeapStdAlign_Object__table__V;

/* E_noRTSMemory__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_E_noRTSMemory xdcruntime_heaps_HeapStdAlign_E_noRTSMemory__C = (((xdc_runtime_Error_Id)902) << 16 | 0);

/* A_zeroSize__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_A_zeroSize xdcruntime_heaps_HeapStdAlign_A_zeroSize__C = (((xdc_runtime_Assert_Id)605) << 16 | 16);

/* A_align__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_A_align xdcruntime_heaps_HeapStdAlign_A_align__C = (((xdc_runtime_Assert_Id)655) << 16 | 16);

/* A_invalidTotalFreeSize__C */
__FAR__ const CT__xdcruntime_heaps_HeapStdAlign_A_invalidTotalFreeSize xdcruntime_heaps_HeapStdAlign_A_invalidTotalFreeSize__C = (((xdc_runtime_Assert_Id)700) << 16 | 16);


/*
 * ======== ti.sdo.xdcruntime.linux.GateProcessSupport FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_GateProcessSupport_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_GateProcessSupport_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_GateProcessSupport_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_GateProcessSupport_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_GateProcessSupport_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_GateProcessSupport_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_GateProcessSupport_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_GateProcessSupport_Module_GateProxy_query

/* entry/exit Log events */
#define ti_sdo_xdcruntime_linux_GateProcessSupport_query__ENTRY_EVT 0xb9b0001
#define ti_sdo_xdcruntime_linux_GateProcessSupport_query__EXIT_EVT 0xb450002
#define ti_sdo_xdcruntime_linux_GateProcessSupport_enter__ENTRY_EVT 0x88b0001
#define ti_sdo_xdcruntime_linux_GateProcessSupport_enter__EXIT_EVT 0x89b0002
#define ti_sdo_xdcruntime_linux_GateProcessSupport_leave__ENTRY_EVT 0x8a90001
#define ti_sdo_xdcruntime_linux_GateProcessSupport_leave__EXIT_EVT 0x8bd0002
#define ti_sdo_xdcruntime_linux_GateProcessSupport_getReferenceCount__ENTRY_EVT 0xbab0001
#define ti_sdo_xdcruntime_linux_GateProcessSupport_getReferenceCount__EXIT_EVT 0xbcb0002

/* query__E */
xdc_Bool ti_sdo_xdcruntime_linux_GateProcessSupport_query__E( xdc_Int qual ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateProcessSupport_query__ENTRY_EVT, (xdc_IArg)qual);
    __ret = ti_sdo_xdcruntime_linux_GateProcessSupport_query__F(qual);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateProcessSupport_query__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* enter__E */
xdc_IArg ti_sdo_xdcruntime_linux_GateProcessSupport_enter__E( ti_sdo_xdcruntime_linux_GateProcessSupport_Handle __inst ) 
{
    xdc_IArg __ret;

    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateProcessSupport_enter__ENTRY_EVT, (xdc_IArg)__inst);
    __ret = ti_sdo_xdcruntime_linux_GateProcessSupport_enter__F((void*)__inst);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateProcessSupport_enter__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* leave__E */
xdc_Void ti_sdo_xdcruntime_linux_GateProcessSupport_leave__E( ti_sdo_xdcruntime_linux_GateProcessSupport_Handle __inst, xdc_IArg key ) 
{
    xdc_runtime_Log_write2(ti_sdo_xdcruntime_linux_GateProcessSupport_leave__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)key);
    ti_sdo_xdcruntime_linux_GateProcessSupport_leave__F((void*)__inst, key);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateProcessSupport_leave__EXIT_EVT, 0);
}

/* getReferenceCount__E */
xdc_Int ti_sdo_xdcruntime_linux_GateProcessSupport_getReferenceCount__E( ti_sdo_xdcruntime_linux_GateProcessSupport_Handle __inst, xdc_runtime_Error_Block* err ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write2(ti_sdo_xdcruntime_linux_GateProcessSupport_getReferenceCount__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)err);
    __ret = ti_sdo_xdcruntime_linux_GateProcessSupport_getReferenceCount__F((void*)__inst, err);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateProcessSupport_getReferenceCount__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* Module_startup */
xdc_Int ti_sdo_xdcruntime_linux_GateProcessSupport_Module_startup__E( xdc_Int state )
{
    return ti_sdo_xdcruntime_linux_GateProcessSupport_Module_startup__F(state);
}


/*
 * ======== ti.sdo.xdcruntime.linux.GateThreadSupport FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_GateThreadSupport_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_GateThreadSupport_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_GateThreadSupport_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_GateThreadSupport_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_GateThreadSupport_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_GateThreadSupport_Module_GateProxy_query

/* entry/exit Log events */
#define ti_sdo_xdcruntime_linux_GateThreadSupport_query__ENTRY_EVT 0xb9b0001
#define ti_sdo_xdcruntime_linux_GateThreadSupport_query__EXIT_EVT 0xb450002
#define ti_sdo_xdcruntime_linux_GateThreadSupport_enter__ENTRY_EVT 0x88b0001
#define ti_sdo_xdcruntime_linux_GateThreadSupport_enter__EXIT_EVT 0x89b0002
#define ti_sdo_xdcruntime_linux_GateThreadSupport_leave__ENTRY_EVT 0x8a90001
#define ti_sdo_xdcruntime_linux_GateThreadSupport_leave__EXIT_EVT 0x8bd0002

/* query__E */
xdc_Bool ti_sdo_xdcruntime_linux_GateThreadSupport_query__E( xdc_Int qual ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateThreadSupport_query__ENTRY_EVT, (xdc_IArg)qual);
    __ret = ti_sdo_xdcruntime_linux_GateThreadSupport_query__F(qual);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateThreadSupport_query__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* enter__E */
xdc_IArg ti_sdo_xdcruntime_linux_GateThreadSupport_enter__E( ti_sdo_xdcruntime_linux_GateThreadSupport_Handle __inst ) 
{
    xdc_IArg __ret;

    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateThreadSupport_enter__ENTRY_EVT, (xdc_IArg)__inst);
    __ret = ti_sdo_xdcruntime_linux_GateThreadSupport_enter__F((void*)__inst);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateThreadSupport_enter__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* leave__E */
xdc_Void ti_sdo_xdcruntime_linux_GateThreadSupport_leave__E( ti_sdo_xdcruntime_linux_GateThreadSupport_Handle __inst, xdc_IArg key ) 
{
    xdc_runtime_Log_write2(ti_sdo_xdcruntime_linux_GateThreadSupport_leave__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)key);
    ti_sdo_xdcruntime_linux_GateThreadSupport_leave__F((void*)__inst, key);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_GateThreadSupport_leave__EXIT_EVT, 0);
}

/* Module_startup */
xdc_Int ti_sdo_xdcruntime_linux_GateThreadSupport_Module_startup__E( xdc_Int state )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Module_startup__F(state);
}


/*
 * ======== ti.sdo.xdcruntime.linux.SemProcessSupport FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_SemProcessSupport_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_SemProcessSupport_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_SemProcessSupport_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_SemProcessSupport_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_SemProcessSupport_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_SemProcessSupport_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_SemProcessSupport_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_SemProcessSupport_Module_GateProxy_query

/* entry/exit Log events */
#define ti_sdo_xdcruntime_linux_SemProcessSupport_pend__ENTRY_EVT 0x8cd0001
#define ti_sdo_xdcruntime_linux_SemProcessSupport_pend__EXIT_EVT 0x8e60002
#define ti_sdo_xdcruntime_linux_SemProcessSupport_post__ENTRY_EVT 0x8f30001
#define ti_sdo_xdcruntime_linux_SemProcessSupport_post__EXIT_EVT 0x9060002

/* pend__E */
xdc_Int ti_sdo_xdcruntime_linux_SemProcessSupport_pend__E( ti_sdo_xdcruntime_linux_SemProcessSupport_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(ti_sdo_xdcruntime_linux_SemProcessSupport_pend__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_SemProcessSupport_pend__F((void*)__inst, timeout, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_SemProcessSupport_pend__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* post__E */
xdc_Bool ti_sdo_xdcruntime_linux_SemProcessSupport_post__E( ti_sdo_xdcruntime_linux_SemProcessSupport_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(ti_sdo_xdcruntime_linux_SemProcessSupport_post__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_SemProcessSupport_post__F((void*)__inst, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_SemProcessSupport_post__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* Module_startup */
xdc_Int ti_sdo_xdcruntime_linux_SemProcessSupport_Module_startup__E( xdc_Int state )
{
    return ti_sdo_xdcruntime_linux_SemProcessSupport_Module_startup__F(state);
}


/*
 * ======== ti.sdo.xdcruntime.linux.SemThreadSupport FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_SemThreadSupport_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_SemThreadSupport_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_SemThreadSupport_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_SemThreadSupport_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_SemThreadSupport_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_SemThreadSupport_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_SemThreadSupport_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_SemThreadSupport_Module_GateProxy_query

/* entry/exit Log events */
#define ti_sdo_xdcruntime_linux_SemThreadSupport_pend__ENTRY_EVT 0x8cd0001
#define ti_sdo_xdcruntime_linux_SemThreadSupport_pend__EXIT_EVT 0x8e60002
#define ti_sdo_xdcruntime_linux_SemThreadSupport_post__ENTRY_EVT 0x8f30001
#define ti_sdo_xdcruntime_linux_SemThreadSupport_post__EXIT_EVT 0x9060002

/* pend__E */
xdc_Int ti_sdo_xdcruntime_linux_SemThreadSupport_pend__E( ti_sdo_xdcruntime_linux_SemThreadSupport_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(ti_sdo_xdcruntime_linux_SemThreadSupport_pend__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_SemThreadSupport_pend__F((void*)__inst, timeout, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_SemThreadSupport_pend__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* post__E */
xdc_Bool ti_sdo_xdcruntime_linux_SemThreadSupport_post__E( ti_sdo_xdcruntime_linux_SemThreadSupport_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(ti_sdo_xdcruntime_linux_SemThreadSupport_post__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_SemThreadSupport_post__F((void*)__inst, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_SemThreadSupport_post__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* Module_startup */
xdc_Int ti_sdo_xdcruntime_linux_SemThreadSupport_Module_startup__E( xdc_Int state )
{
    return ti_sdo_xdcruntime_linux_SemThreadSupport_Module_startup__F(state);
}


/*
 * ======== ti.sdo.xdcruntime.linux.ThreadSupport FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_ThreadSupport_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_ThreadSupport_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_ThreadSupport_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_ThreadSupport_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_ThreadSupport_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_ThreadSupport_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_ThreadSupport_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_ThreadSupport_Module_GateProxy_query

/* entry/exit Log events */
#define ti_sdo_xdcruntime_linux_ThreadSupport_self__ENTRY_EVT 0xcd90001
#define ti_sdo_xdcruntime_linux_ThreadSupport_self__EXIT_EVT 0xce80002
#define ti_sdo_xdcruntime_linux_ThreadSupport_start__ENTRY_EVT 0x9130001
#define ti_sdo_xdcruntime_linux_ThreadSupport_start__EXIT_EVT 0x9230002
#define ti_sdo_xdcruntime_linux_ThreadSupport_yield__ENTRY_EVT 0x9310001
#define ti_sdo_xdcruntime_linux_ThreadSupport_yield__EXIT_EVT 0x9410002
#define ti_sdo_xdcruntime_linux_ThreadSupport_compareOsPriorities__ENTRY_EVT 0x94f0001
#define ti_sdo_xdcruntime_linux_ThreadSupport_compareOsPriorities__EXIT_EVT 0x9750002
#define ti_sdo_xdcruntime_linux_ThreadSupport_sleep__ENTRY_EVT 0x9910001
#define ti_sdo_xdcruntime_linux_ThreadSupport_sleep__EXIT_EVT 0x9a70002
#define ti_sdo_xdcruntime_linux_ThreadSupport_join__ENTRY_EVT 0x9b50001
#define ti_sdo_xdcruntime_linux_ThreadSupport_join__EXIT_EVT 0x9c80002
#define ti_sdo_xdcruntime_linux_ThreadSupport_getPriority__ENTRY_EVT 0x9d50001
#define ti_sdo_xdcruntime_linux_ThreadSupport_getPriority__EXIT_EVT 0x9ef0002
#define ti_sdo_xdcruntime_linux_ThreadSupport_setPriority__ENTRY_EVT 0xa030001
#define ti_sdo_xdcruntime_linux_ThreadSupport_setPriority__EXIT_EVT 0xa210002
#define ti_sdo_xdcruntime_linux_ThreadSupport_getOsPriority__ENTRY_EVT 0xa350001
#define ti_sdo_xdcruntime_linux_ThreadSupport_getOsPriority__EXIT_EVT 0xa510002
#define ti_sdo_xdcruntime_linux_ThreadSupport_setOsPriority__ENTRY_EVT 0xa670001
#define ti_sdo_xdcruntime_linux_ThreadSupport_setOsPriority__EXIT_EVT 0xa870002
#define ti_sdo_xdcruntime_linux_ThreadSupport_getOsHandle__ENTRY_EVT 0xa9d0001
#define ti_sdo_xdcruntime_linux_ThreadSupport_getOsHandle__EXIT_EVT 0xab30002
#define ti_sdo_xdcruntime_linux_ThreadSupport_getTls__ENTRY_EVT 0xac70001
#define ti_sdo_xdcruntime_linux_ThreadSupport_getTls__EXIT_EVT 0xad80002
#define ti_sdo_xdcruntime_linux_ThreadSupport_setTls__ENTRY_EVT 0xae70001
#define ti_sdo_xdcruntime_linux_ThreadSupport_setTls__EXIT_EVT 0xafc0002
#define ti_sdo_xdcruntime_linux_ThreadSupport_stat__ENTRY_EVT 0xb0d0001
#define ti_sdo_xdcruntime_linux_ThreadSupport_stat__EXIT_EVT 0xb240002

/* self__E */
xdc_runtime_knl_IThreadSupport_Handle ti_sdo_xdcruntime_linux_ThreadSupport_self__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_runtime_knl_IThreadSupport_Handle __ret;

    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_self__ENTRY_EVT, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_self__F(eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_self__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* start__E */
xdc_Bool ti_sdo_xdcruntime_linux_ThreadSupport_start__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_start__ENTRY_EVT, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_start__F(eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_start__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* yield__E */
xdc_Bool ti_sdo_xdcruntime_linux_ThreadSupport_yield__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_yield__ENTRY_EVT, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_yield__F(eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_yield__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* compareOsPriorities__E */
xdc_Int ti_sdo_xdcruntime_linux_ThreadSupport_compareOsPriorities__E( xdc_Int p1, xdc_Int p2, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(ti_sdo_xdcruntime_linux_ThreadSupport_compareOsPriorities__ENTRY_EVT, (xdc_IArg)p1, (xdc_IArg)p2, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_compareOsPriorities__F(p1, p2, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_compareOsPriorities__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* sleep__E */
xdc_Bool ti_sdo_xdcruntime_linux_ThreadSupport_sleep__E( xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(ti_sdo_xdcruntime_linux_ThreadSupport_sleep__ENTRY_EVT, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_sleep__F(timeout, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_sleep__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* join__E */
xdc_Bool ti_sdo_xdcruntime_linux_ThreadSupport_join__E( ti_sdo_xdcruntime_linux_ThreadSupport_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(ti_sdo_xdcruntime_linux_ThreadSupport_join__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_join__F((void*)__inst, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_join__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getPriority__E */
xdc_runtime_knl_IThreadSupport_Priority ti_sdo_xdcruntime_linux_ThreadSupport_getPriority__E( ti_sdo_xdcruntime_linux_ThreadSupport_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_runtime_knl_IThreadSupport_Priority __ret;

    xdc_runtime_Log_write2(ti_sdo_xdcruntime_linux_ThreadSupport_getPriority__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_getPriority__F((void*)__inst, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_getPriority__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* setPriority__E */
xdc_Bool ti_sdo_xdcruntime_linux_ThreadSupport_setPriority__E( ti_sdo_xdcruntime_linux_ThreadSupport_Handle __inst, xdc_runtime_knl_IThreadSupport_Priority newPri, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write3(ti_sdo_xdcruntime_linux_ThreadSupport_setPriority__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)newPri, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_setPriority__F((void*)__inst, newPri, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_setPriority__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getOsPriority__E */
xdc_Int ti_sdo_xdcruntime_linux_ThreadSupport_getOsPriority__E( ti_sdo_xdcruntime_linux_ThreadSupport_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write2(ti_sdo_xdcruntime_linux_ThreadSupport_getOsPriority__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_getOsPriority__F((void*)__inst, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_getOsPriority__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* setOsPriority__E */
xdc_Bool ti_sdo_xdcruntime_linux_ThreadSupport_setOsPriority__E( ti_sdo_xdcruntime_linux_ThreadSupport_Handle __inst, xdc_Int newPri, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write3(ti_sdo_xdcruntime_linux_ThreadSupport_setOsPriority__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)newPri, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_setOsPriority__F((void*)__inst, newPri, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_setOsPriority__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getOsHandle__E */
xdc_Ptr ti_sdo_xdcruntime_linux_ThreadSupport_getOsHandle__E( ti_sdo_xdcruntime_linux_ThreadSupport_Handle __inst ) 
{
    xdc_Ptr __ret;

    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_getOsHandle__ENTRY_EVT, (xdc_IArg)__inst);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_getOsHandle__F((void*)__inst);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_getOsHandle__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getTls__E */
xdc_Ptr ti_sdo_xdcruntime_linux_ThreadSupport_getTls__E( ti_sdo_xdcruntime_linux_ThreadSupport_Handle __inst ) 
{
    xdc_Ptr __ret;

    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_getTls__ENTRY_EVT, (xdc_IArg)__inst);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_getTls__F((void*)__inst);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_getTls__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* setTls__E */
xdc_Void ti_sdo_xdcruntime_linux_ThreadSupport_setTls__E( ti_sdo_xdcruntime_linux_ThreadSupport_Handle __inst, xdc_Ptr tls ) 
{
    xdc_runtime_Log_write2(ti_sdo_xdcruntime_linux_ThreadSupport_setTls__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)tls);
    ti_sdo_xdcruntime_linux_ThreadSupport_setTls__F((void*)__inst, tls);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_setTls__EXIT_EVT, 0);
}

/* stat__E */
xdc_Bool ti_sdo_xdcruntime_linux_ThreadSupport_stat__E( ti_sdo_xdcruntime_linux_ThreadSupport_Handle __inst, xdc_runtime_knl_IThreadSupport_Stat* buf, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write3(ti_sdo_xdcruntime_linux_ThreadSupport_stat__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)buf, (xdc_IArg)eb);
    __ret = ti_sdo_xdcruntime_linux_ThreadSupport_stat__F((void*)__inst, buf, eb);
    xdc_runtime_Log_write1(ti_sdo_xdcruntime_linux_ThreadSupport_stat__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* Module_startup */
xdc_Int ti_sdo_xdcruntime_linux_ThreadSupport_Module_startup__E( xdc_Int state )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_Module_startup__F(state);
}


/*
 * ======== ti.sdo.xdcruntime.linux.TimestampPosix FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_TimestampPosix_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_TimestampPosix_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_TimestampPosix_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_TimestampPosix_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_TimestampPosix_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_TimestampPosix_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_TimestampPosix_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_TimestampPosix_Module_GateProxy_query

/* entry/exit Log events */
#define ti_sdo_xdcruntime_linux_TimestampPosix_get32__ENTRY_EVT 0x0
#define ti_sdo_xdcruntime_linux_TimestampPosix_get32__EXIT_EVT 0x0
#define ti_sdo_xdcruntime_linux_TimestampPosix_get64__ENTRY_EVT 0x0
#define ti_sdo_xdcruntime_linux_TimestampPosix_get64__EXIT_EVT 0x0
#define ti_sdo_xdcruntime_linux_TimestampPosix_getFreq__ENTRY_EVT 0x0
#define ti_sdo_xdcruntime_linux_TimestampPosix_getFreq__EXIT_EVT 0x0

/* get32__E */
xdc_Bits32 ti_sdo_xdcruntime_linux_TimestampPosix_get32__E( void ) 
{
    return ti_sdo_xdcruntime_linux_TimestampPosix_get32__F();
}

/* get64__E */
xdc_Void ti_sdo_xdcruntime_linux_TimestampPosix_get64__E( xdc_runtime_Types_Timestamp64* result ) 
{
    ti_sdo_xdcruntime_linux_TimestampPosix_get64__F(result);
}

/* getFreq__E */
xdc_Void ti_sdo_xdcruntime_linux_TimestampPosix_getFreq__E( xdc_runtime_Types_FreqHz* freq ) 
{
    ti_sdo_xdcruntime_linux_TimestampPosix_getFreq__F(freq);
}


/*
 * ======== xdc.runtime.Diags FUNCTION STUBS ========
 */

/* setMask__E */
xdc_Void xdc_runtime_Diags_setMask__E( xdc_String control ) 
{
    xdc_runtime_Diags_setMask__F(control);
}


/*
 * ======== xdc.runtime.Error FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_Error_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_Error_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_Error_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_Error_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_Error_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_Error_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_Error_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_Error_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_Error_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_Error_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_Error_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_Error_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_Error_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_Error_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_Error_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_Error_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_Error_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_Error_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_Error_check__ENTRY_EVT 0x4a80001
#define xdc_runtime_Error_check__EXIT_EVT 0x4b80002
#define xdc_runtime_Error_getData__ENTRY_EVT 0x4c60001
#define xdc_runtime_Error_getData__EXIT_EVT 0x4d80002
#define xdc_runtime_Error_getCode__ENTRY_EVT 0x4e80001
#define xdc_runtime_Error_getCode__EXIT_EVT 0x4fa0002
#define xdc_runtime_Error_getId__ENTRY_EVT 0x50c0001
#define xdc_runtime_Error_getId__EXIT_EVT 0x51c0002
#define xdc_runtime_Error_getMsg__ENTRY_EVT 0x52c0001
#define xdc_runtime_Error_getMsg__EXIT_EVT 0x53d0002
#define xdc_runtime_Error_getSite__ENTRY_EVT 0x54e0001
#define xdc_runtime_Error_getSite__EXIT_EVT 0x5600002
#define xdc_runtime_Error_init__ENTRY_EVT 0x5700001
#define xdc_runtime_Error_init__EXIT_EVT 0x57f0002
#define xdc_runtime_Error_print__ENTRY_EVT 0x58e0001
#define xdc_runtime_Error_print__EXIT_EVT 0x59e0002
#define xdc_runtime_Error_raiseX__ENTRY_EVT 0x5ae0001
#define xdc_runtime_Error_raiseX__EXIT_EVT 0x5cd0002

/* check__E */
xdc_Bool xdc_runtime_Error_check__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(xdc_runtime_Error_check__ENTRY_EVT, (xdc_IArg)eb);
    __ret = xdc_runtime_Error_check__F(eb);
    xdc_runtime_Log_write1(xdc_runtime_Error_check__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getData__E */
xdc_runtime_Error_Data* xdc_runtime_Error_getData__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_runtime_Error_Data* __ret;

    xdc_runtime_Log_write1(xdc_runtime_Error_getData__ENTRY_EVT, (xdc_IArg)eb);
    __ret = xdc_runtime_Error_getData__F(eb);
    xdc_runtime_Log_write1(xdc_runtime_Error_getData__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getCode__E */
xdc_UInt16 xdc_runtime_Error_getCode__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_UInt16 __ret;

    xdc_runtime_Log_write1(xdc_runtime_Error_getCode__ENTRY_EVT, (xdc_IArg)eb);
    __ret = xdc_runtime_Error_getCode__F(eb);
    xdc_runtime_Log_write1(xdc_runtime_Error_getCode__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getId__E */
xdc_runtime_Error_Id xdc_runtime_Error_getId__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_runtime_Error_Id __ret;

    xdc_runtime_Log_write1(xdc_runtime_Error_getId__ENTRY_EVT, (xdc_IArg)eb);
    __ret = xdc_runtime_Error_getId__F(eb);
    xdc_runtime_Log_write1(xdc_runtime_Error_getId__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getMsg__E */
xdc_String xdc_runtime_Error_getMsg__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_String __ret;

    xdc_runtime_Log_write1(xdc_runtime_Error_getMsg__ENTRY_EVT, (xdc_IArg)eb);
    __ret = xdc_runtime_Error_getMsg__F(eb);
    xdc_runtime_Log_write1(xdc_runtime_Error_getMsg__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getSite__E */
xdc_runtime_Types_Site* xdc_runtime_Error_getSite__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_runtime_Types_Site* __ret;

    xdc_runtime_Log_write1(xdc_runtime_Error_getSite__ENTRY_EVT, (xdc_IArg)eb);
    __ret = xdc_runtime_Error_getSite__F(eb);
    xdc_runtime_Log_write1(xdc_runtime_Error_getSite__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* init__E */
xdc_Void xdc_runtime_Error_init__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Error_init__ENTRY_EVT, (xdc_IArg)eb);
    xdc_runtime_Error_init__F(eb);
    xdc_runtime_Log_write1(xdc_runtime_Error_init__EXIT_EVT, 0);
}

/* print__E */
xdc_Void xdc_runtime_Error_print__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Error_print__ENTRY_EVT, (xdc_IArg)eb);
    xdc_runtime_Error_print__F(eb);
    xdc_runtime_Log_write1(xdc_runtime_Error_print__EXIT_EVT, 0);
}

/* raiseX__E */
xdc_Void xdc_runtime_Error_raiseX__E( xdc_runtime_Error_Block* eb, xdc_runtime_Types_ModuleId mod, xdc_String file, xdc_Int line, xdc_runtime_Error_Id id, xdc_IArg arg1, xdc_IArg arg2 ) 
{
    xdc_runtime_Log_write4(xdc_runtime_Error_raiseX__ENTRY_EVT, (xdc_IArg)eb, (xdc_IArg)mod, (xdc_IArg)file, (xdc_IArg)line);
    xdc_runtime_Error_raiseX__F(eb, mod, file, line, id, arg1, arg2);
    xdc_runtime_Log_write1(xdc_runtime_Error_raiseX__EXIT_EVT, 0);
}


/*
 * ======== xdc.runtime.Gate FUNCTION STUBS ========
 */

/* enterSystem__E */
xdc_IArg xdc_runtime_Gate_enterSystem__E( void ) 
{
    return xdc_runtime_Gate_enterSystem__F();
}

/* leaveSystem__E */
xdc_Void xdc_runtime_Gate_leaveSystem__E( xdc_IArg key ) 
{
    xdc_runtime_Gate_leaveSystem__F(key);
}


/*
 * ======== xdc.runtime.GateNull FUNCTION STUBS ========
 */

/* query__E */
xdc_Bool xdc_runtime_GateNull_query__E( xdc_Int qual ) 
{
    return xdc_runtime_GateNull_query__F(qual);
}

/* enter__E */
xdc_IArg xdc_runtime_GateNull_enter__E( xdc_runtime_GateNull_Handle __inst ) 
{
    return xdc_runtime_GateNull_enter__F((void*)__inst);
}

/* leave__E */
xdc_Void xdc_runtime_GateNull_leave__E( xdc_runtime_GateNull_Handle __inst, xdc_IArg key ) 
{
    xdc_runtime_GateNull_leave__F((void*)__inst, key);
}


/*
 * ======== xdc.runtime.HeapStd FUNCTION STUBS ========
 */

/* free__E */
xdc_Void xdc_runtime_HeapStd_free__E( xdc_runtime_HeapStd_Handle __inst, xdc_Ptr block, xdc_SizeT size ) 
{
    xdc_runtime_HeapStd_free__F((void*)__inst, block, size);
}

/* getStats__E */
xdc_Void xdc_runtime_HeapStd_getStats__E( xdc_runtime_HeapStd_Handle __inst, xdc_runtime_Memory_Stats* stats ) 
{
    xdc_runtime_HeapStd_getStats__F((void*)__inst, stats);
}

/* alloc__E */
xdc_Ptr xdc_runtime_HeapStd_alloc__E( xdc_runtime_HeapStd_Handle __inst, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb ) 
{
    return xdc_runtime_HeapStd_alloc__F((void*)__inst, size, align, eb);
}

/* isBlocking__E */
xdc_Bool xdc_runtime_HeapStd_isBlocking__E( xdc_runtime_HeapStd_Handle __inst ) 
{
    return xdc_runtime_HeapStd_isBlocking__F((void*)__inst);
}


/*
 * ======== xdc.runtime.Log FUNCTION STUBS ========
 */

/* doPrint__E */
xdc_Void xdc_runtime_Log_doPrint__E( xdc_runtime_Log_EventRec* evRec ) 
{
    xdc_runtime_Log_doPrint__F(evRec);
}


/*
 * ======== xdc.runtime.LoggerSys FUNCTION STUBS ========
 */

/* enable__E */
xdc_Bool xdc_runtime_LoggerSys_enable__E( xdc_runtime_LoggerSys_Handle __inst ) 
{
    return xdc_runtime_LoggerSys_enable__F((void*)__inst);
}

/* disable__E */
xdc_Bool xdc_runtime_LoggerSys_disable__E( xdc_runtime_LoggerSys_Handle __inst ) 
{
    return xdc_runtime_LoggerSys_disable__F((void*)__inst);
}

/* write0__E */
xdc_Void xdc_runtime_LoggerSys_write0__E( xdc_runtime_LoggerSys_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid ) 
{
    xdc_runtime_LoggerSys_write0__F((void*)__inst, evt, mid);
}

/* write1__E */
xdc_Void xdc_runtime_LoggerSys_write1__E( xdc_runtime_LoggerSys_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 ) 
{
    xdc_runtime_LoggerSys_write1__F((void*)__inst, evt, mid, a1);
}

/* write2__E */
xdc_Void xdc_runtime_LoggerSys_write2__E( xdc_runtime_LoggerSys_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 ) 
{
    xdc_runtime_LoggerSys_write2__F((void*)__inst, evt, mid, a1, a2);
}

/* write4__E */
xdc_Void xdc_runtime_LoggerSys_write4__E( xdc_runtime_LoggerSys_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 ) 
{
    xdc_runtime_LoggerSys_write4__F((void*)__inst, evt, mid, a1, a2, a3, a4);
}

/* write8__E */
xdc_Void xdc_runtime_LoggerSys_write8__E( xdc_runtime_LoggerSys_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 ) 
{
    xdc_runtime_LoggerSys_write8__F((void*)__inst, evt, mid, a1, a2, a3, a4, a5, a6, a7, a8);
}


/*
 * ======== xdc.runtime.Memory FUNCTION STUBS ========
 */

/* alloc__E */
xdc_Ptr xdc_runtime_Memory_alloc__E( xdc_runtime_IHeap_Handle heap, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb ) 
{
    return xdc_runtime_Memory_alloc__F(heap, size, align, eb);
}

/* calloc__E */
xdc_Ptr xdc_runtime_Memory_calloc__E( xdc_runtime_IHeap_Handle heap, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb ) 
{
    return xdc_runtime_Memory_calloc__F(heap, size, align, eb);
}

/* free__E */
xdc_Void xdc_runtime_Memory_free__E( xdc_runtime_IHeap_Handle heap, xdc_Ptr block, xdc_SizeT size ) 
{
    xdc_runtime_Memory_free__F(heap, block, size);
}

/* getStats__E */
xdc_Void xdc_runtime_Memory_getStats__E( xdc_runtime_IHeap_Handle heap, xdc_runtime_Memory_Stats* stats ) 
{
    xdc_runtime_Memory_getStats__F(heap, stats);
}

/* query__E */
xdc_Bool xdc_runtime_Memory_query__E( xdc_runtime_IHeap_Handle heap, xdc_Int qual ) 
{
    return xdc_runtime_Memory_query__F(heap, qual);
}

/* getMaxDefaultTypeAlign__E */
xdc_SizeT xdc_runtime_Memory_getMaxDefaultTypeAlign__E( void ) 
{
    return xdc_runtime_Memory_getMaxDefaultTypeAlign__F();
}

/* valloc__E */
xdc_Ptr xdc_runtime_Memory_valloc__E( xdc_runtime_IHeap_Handle heap, xdc_SizeT size, xdc_SizeT align, xdc_Char value, xdc_runtime_Error_Block* eb ) 
{
    return xdc_runtime_Memory_valloc__F(heap, size, align, value, eb);
}


/*
 * ======== xdc.runtime.Registry FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_Registry_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_Registry_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_Registry_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_Registry_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_Registry_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_Registry_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_Registry_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_Registry_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_Registry_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_Registry_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_Registry_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_Registry_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_Registry_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_Registry_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_Registry_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_Registry_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_Registry_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_Registry_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_Registry_addModule__ENTRY_EVT 0x68e0001
#define xdc_runtime_Registry_addModule__EXIT_EVT 0x6a80002
#define xdc_runtime_Registry_findByName__ENTRY_EVT 0x6ba0001
#define xdc_runtime_Registry_findByName__EXIT_EVT 0x6d10002
#define xdc_runtime_Registry_findByNamePattern__ENTRY_EVT 0x6e40001
#define xdc_runtime_Registry_findByNamePattern__EXIT_EVT 0x70a0002
#define xdc_runtime_Registry_findById__ENTRY_EVT 0x7240001
#define xdc_runtime_Registry_findById__EXIT_EVT 0x7370002
#define xdc_runtime_Registry_getMask__ENTRY_EVT 0x7480001
#define xdc_runtime_Registry_getMask__EXIT_EVT 0x7600002
#define xdc_runtime_Registry_isMember__ENTRY_EVT 0x7700001
#define xdc_runtime_Registry_isMember__EXIT_EVT 0x7830002
#define xdc_runtime_Registry_getNextModule__ENTRY_EVT 0x7940001
#define xdc_runtime_Registry_getNextModule__EXIT_EVT 0x7ac0002
#define xdc_runtime_Registry_getModuleName__ENTRY_EVT 0x7c20001
#define xdc_runtime_Registry_getModuleName__EXIT_EVT 0x7da0002
#define xdc_runtime_Registry_getModuleId__ENTRY_EVT 0x7f20001
#define xdc_runtime_Registry_getModuleId__EXIT_EVT 0x8080002

/* addModule__E */
xdc_runtime_Registry_Result xdc_runtime_Registry_addModule__E( xdc_runtime_Registry_Desc* desc, xdc_String modName ) 
{
    xdc_runtime_Registry_Result __ret;

    xdc_runtime_Log_write2(xdc_runtime_Registry_addModule__ENTRY_EVT, (xdc_IArg)desc, (xdc_IArg)modName);
    __ret = xdc_runtime_Registry_addModule__F(desc, modName);
    xdc_runtime_Log_write1(xdc_runtime_Registry_addModule__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* findByName__E */
xdc_runtime_Registry_Desc* xdc_runtime_Registry_findByName__E( xdc_String modName ) 
{
    xdc_runtime_Registry_Desc* __ret;

    xdc_runtime_Log_write1(xdc_runtime_Registry_findByName__ENTRY_EVT, (xdc_IArg)modName);
    __ret = xdc_runtime_Registry_findByName__F(modName);
    xdc_runtime_Log_write1(xdc_runtime_Registry_findByName__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* findByNamePattern__E */
xdc_runtime_Registry_Desc* xdc_runtime_Registry_findByNamePattern__E( xdc_String namePat, xdc_Int len, xdc_runtime_Registry_Desc* prev ) 
{
    xdc_runtime_Registry_Desc* __ret;

    xdc_runtime_Log_write3(xdc_runtime_Registry_findByNamePattern__ENTRY_EVT, (xdc_IArg)namePat, (xdc_IArg)len, (xdc_IArg)prev);
    __ret = xdc_runtime_Registry_findByNamePattern__F(namePat, len, prev);
    xdc_runtime_Log_write1(xdc_runtime_Registry_findByNamePattern__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* findById__E */
xdc_runtime_Registry_Desc* xdc_runtime_Registry_findById__E( xdc_runtime_Types_ModuleId mid ) 
{
    xdc_runtime_Registry_Desc* __ret;

    xdc_runtime_Log_write1(xdc_runtime_Registry_findById__ENTRY_EVT, (xdc_IArg)mid);
    __ret = xdc_runtime_Registry_findById__F(mid);
    xdc_runtime_Log_write1(xdc_runtime_Registry_findById__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getMask__E */
xdc_Bool xdc_runtime_Registry_getMask__E( xdc_String name, xdc_runtime_Types_DiagsMask* mask ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(xdc_runtime_Registry_getMask__ENTRY_EVT, (xdc_IArg)name, (xdc_IArg)mask);
    __ret = xdc_runtime_Registry_getMask__F(name, mask);
    xdc_runtime_Log_write1(xdc_runtime_Registry_getMask__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* isMember__E */
xdc_Bool xdc_runtime_Registry_isMember__E( xdc_runtime_Types_ModuleId mid ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(xdc_runtime_Registry_isMember__ENTRY_EVT, (xdc_IArg)mid);
    __ret = xdc_runtime_Registry_isMember__F(mid);
    xdc_runtime_Log_write1(xdc_runtime_Registry_isMember__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getNextModule__E */
xdc_runtime_Registry_Desc* xdc_runtime_Registry_getNextModule__E( xdc_runtime_Registry_Desc* desc ) 
{
    xdc_runtime_Registry_Desc* __ret;

    xdc_runtime_Log_write1(xdc_runtime_Registry_getNextModule__ENTRY_EVT, (xdc_IArg)desc);
    __ret = xdc_runtime_Registry_getNextModule__F(desc);
    xdc_runtime_Log_write1(xdc_runtime_Registry_getNextModule__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getModuleName__E */
xdc_String xdc_runtime_Registry_getModuleName__E( xdc_runtime_Registry_Desc* desc ) 
{
    xdc_String __ret;

    xdc_runtime_Log_write1(xdc_runtime_Registry_getModuleName__ENTRY_EVT, (xdc_IArg)desc);
    __ret = xdc_runtime_Registry_getModuleName__F(desc);
    xdc_runtime_Log_write1(xdc_runtime_Registry_getModuleName__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getModuleId__E */
xdc_runtime_Types_ModuleId xdc_runtime_Registry_getModuleId__E( xdc_runtime_Registry_Desc* desc ) 
{
    xdc_runtime_Types_ModuleId __ret;

    xdc_runtime_Log_write1(xdc_runtime_Registry_getModuleId__ENTRY_EVT, (xdc_IArg)desc);
    __ret = xdc_runtime_Registry_getModuleId__F(desc);
    xdc_runtime_Log_write1(xdc_runtime_Registry_getModuleId__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdc.runtime.Rta FUNCTION STUBS ========
 */

/* processCommand__E */
xdc_Void xdc_runtime_Rta_processCommand__E( xdc_runtime_Rta_CommandPacket* cmd, xdc_runtime_Rta_ResponsePacket* resp ) 
{
    xdc_runtime_Rta_processCommand__F(cmd, resp);
}

/* acknowledgeCmd__E */
xdc_Void xdc_runtime_Rta_acknowledgeCmd__E( xdc_runtime_Rta_ResponsePacket* resp ) 
{
    xdc_runtime_Rta_acknowledgeCmd__F(resp);
}

/* readMask__E */
xdc_Void xdc_runtime_Rta_readMask__E( xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg addr ) 
{
    xdc_runtime_Rta_readMask__F(resp, addr);
}

/* writeMask__E */
xdc_Void xdc_runtime_Rta_writeMask__E( xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg addr, xdc_UArg val ) 
{
    xdc_runtime_Rta_writeMask__F(resp, addr, val);
}

/* enableLog__E */
xdc_Void xdc_runtime_Rta_enableLog__E( xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg log ) 
{
    xdc_runtime_Rta_enableLog__F(resp, log);
}

/* disableLog__E */
xdc_Void xdc_runtime_Rta_disableLog__E( xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg log ) 
{
    xdc_runtime_Rta_disableLog__F(resp, log);
}

/* getCpuSpeed__E */
xdc_Void xdc_runtime_Rta_getCpuSpeed__E( xdc_runtime_Rta_ResponsePacket* resp ) 
{
    xdc_runtime_Rta_getCpuSpeed__F(resp);
}

/* resetLog__E */
xdc_Void xdc_runtime_Rta_resetLog__E( xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg log ) 
{
    xdc_runtime_Rta_resetLog__F(resp, log);
}

/* changePeriod__E */
xdc_Void xdc_runtime_Rta_changePeriod__E( xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg period ) 
{
    xdc_runtime_Rta_changePeriod__F(resp, period);
}


/*
 * ======== xdc.runtime.Startup FUNCTION STUBS ========
 */

/* exec__E */
xdc_Void xdc_runtime_Startup_exec__E( void ) 
{
    xdc_runtime_Startup_exec__F();
}

/* rtsDone__E */
xdc_Bool xdc_runtime_Startup_rtsDone__E( void ) 
{
    return xdc_runtime_Startup_rtsDone__F();
}


/*
 * ======== xdc.runtime.SysStd FUNCTION STUBS ========
 */

/* abort__E */
xdc_Void xdc_runtime_SysStd_abort__E( xdc_String str ) 
{
    xdc_runtime_SysStd_abort__F(str);
}

/* exit__E */
xdc_Void xdc_runtime_SysStd_exit__E( xdc_Int stat ) 
{
    xdc_runtime_SysStd_exit__F(stat);
}

/* flush__E */
xdc_Void xdc_runtime_SysStd_flush__E( void ) 
{
    xdc_runtime_SysStd_flush__F();
}

/* putch__E */
xdc_Void xdc_runtime_SysStd_putch__E( xdc_Char ch ) 
{
    xdc_runtime_SysStd_putch__F(ch);
}

/* ready__E */
xdc_Bool xdc_runtime_SysStd_ready__E( void ) 
{
    return xdc_runtime_SysStd_ready__F();
}


/*
 * ======== xdc.runtime.System FUNCTION STUBS ========
 */

/* abort__E */
xdc_Void xdc_runtime_System_abort__E( xdc_String str ) 
{
    xdc_runtime_System_abort__F(str);
}

/* atexit__E */
xdc_Bool xdc_runtime_System_atexit__E( xdc_runtime_System_AtexitHandler handler ) 
{
    return xdc_runtime_System_atexit__F(handler);
}

/* exit__E */
xdc_Void xdc_runtime_System_exit__E( xdc_Int stat ) 
{
    xdc_runtime_System_exit__F(stat);
}

/* putch__E */
xdc_Void xdc_runtime_System_putch__E( xdc_Char ch ) 
{
    xdc_runtime_System_putch__F(ch);
}

/* flush__E */
xdc_Void xdc_runtime_System_flush__E( void ) 
{
    xdc_runtime_System_flush__F();
}

/* printf_va__E */
xdc_Int xdc_runtime_System_printf_va__E( xdc_String fmt, va_list __va ) 
{
    return xdc_runtime_System_printf_va__F(fmt, __va);
}

/* printf__E */
xdc_Int xdc_runtime_System_printf__E( xdc_String fmt, ... ) 
{
    xdc_Int __ret;

    va_list __va; va_start(__va, fmt);
    __ret = xdc_runtime_System_printf_va__F(fmt, __va);

    va_end(__va);
    return __ret;
}

/* aprintf_va__E */
xdc_Int xdc_runtime_System_aprintf_va__E( xdc_String fmt, va_list __va ) 
{
    return xdc_runtime_System_aprintf_va__F(fmt, __va);
}

/* aprintf__E */
xdc_Int xdc_runtime_System_aprintf__E( xdc_String fmt, ... ) 
{
    xdc_Int __ret;

    va_list __va; va_start(__va, fmt);
    __ret = xdc_runtime_System_aprintf_va__F(fmt, __va);

    va_end(__va);
    return __ret;
}

/* sprintf_va__E */
xdc_Int xdc_runtime_System_sprintf_va__E( xdc_Char buf[], xdc_String fmt, va_list __va ) 
{
    return xdc_runtime_System_sprintf_va__F(buf, fmt, __va);
}

/* sprintf__E */
xdc_Int xdc_runtime_System_sprintf__E( xdc_Char buf[], xdc_String fmt, ... ) 
{
    xdc_Int __ret;

    va_list __va; va_start(__va, fmt);
    __ret = xdc_runtime_System_sprintf_va__F(buf, fmt, __va);

    va_end(__va);
    return __ret;
}

/* asprintf_va__E */
xdc_Int xdc_runtime_System_asprintf_va__E( xdc_Char buf[], xdc_String fmt, va_list __va ) 
{
    return xdc_runtime_System_asprintf_va__F(buf, fmt, __va);
}

/* asprintf__E */
xdc_Int xdc_runtime_System_asprintf__E( xdc_Char buf[], xdc_String fmt, ... ) 
{
    xdc_Int __ret;

    va_list __va; va_start(__va, fmt);
    __ret = xdc_runtime_System_asprintf_va__F(buf, fmt, __va);

    va_end(__va);
    return __ret;
}

/* vprintf__E */
xdc_Int xdc_runtime_System_vprintf__E( xdc_String fmt, xdc_VaList va ) 
{
    return xdc_runtime_System_vprintf__F(fmt, va);
}

/* avprintf__E */
xdc_Int xdc_runtime_System_avprintf__E( xdc_String fmt, xdc_VaList va ) 
{
    return xdc_runtime_System_avprintf__F(fmt, va);
}

/* vsprintf__E */
xdc_Int xdc_runtime_System_vsprintf__E( xdc_Char buf[], xdc_String fmt, xdc_VaList va ) 
{
    return xdc_runtime_System_vsprintf__F(buf, fmt, va);
}

/* avsprintf__E */
xdc_Int xdc_runtime_System_avsprintf__E( xdc_Char buf[], xdc_String fmt, xdc_VaList va ) 
{
    return xdc_runtime_System_avsprintf__F(buf, fmt, va);
}

/* Module_startup */
xdc_Int xdc_runtime_System_Module_startup__E( xdc_Int state )
{
    return xdc_runtime_System_Module_startup__F(state);
}


/*
 * ======== xdc.runtime.Text FUNCTION STUBS ========
 */

/* cordText__E */
xdc_String xdc_runtime_Text_cordText__E( xdc_runtime_Text_CordAddr cord ) 
{
    return xdc_runtime_Text_cordText__F(cord);
}

/* ropeText__E */
xdc_String xdc_runtime_Text_ropeText__E( xdc_runtime_Text_RopeId rope ) 
{
    return xdc_runtime_Text_ropeText__F(rope);
}

/* matchRope__E */
xdc_Int xdc_runtime_Text_matchRope__E( xdc_runtime_Text_RopeId rope, xdc_String pat, xdc_Int* lenp ) 
{
    return xdc_runtime_Text_matchRope__F(rope, pat, lenp);
}

/* putLab__E */
xdc_Int xdc_runtime_Text_putLab__E( xdc_runtime_Types_Label* lab, xdc_Char** bufp, xdc_Int len ) 
{
    return xdc_runtime_Text_putLab__F(lab, bufp, len);
}

/* putMod__E */
xdc_Int xdc_runtime_Text_putMod__E( xdc_runtime_Types_ModuleId mid, xdc_Char** bufp, xdc_Int len ) 
{
    return xdc_runtime_Text_putMod__F(mid, bufp, len);
}

/* putSite__E */
xdc_Int xdc_runtime_Text_putSite__E( xdc_runtime_Types_Site* site, xdc_Char** bufp, xdc_Int len ) 
{
    return xdc_runtime_Text_putSite__F(site, bufp, len);
}


/*
 * ======== xdc.runtime.Timestamp FUNCTION STUBS ========
 */

/* get32__E */
xdc_Bits32 xdc_runtime_Timestamp_get32__E( void ) 
{
    return xdc_runtime_Timestamp_get32__F();
}

/* get64__E */
xdc_Void xdc_runtime_Timestamp_get64__E( xdc_runtime_Types_Timestamp64* result ) 
{
    xdc_runtime_Timestamp_get64__F(result);
}

/* getFreq__E */
xdc_Void xdc_runtime_Timestamp_getFreq__E( xdc_runtime_Types_FreqHz* freq ) 
{
    xdc_runtime_Timestamp_getFreq__F(freq);
}


/*
 * ======== xdc.runtime.knl.GateH FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_GateH_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_GateH_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_GateH_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_GateH_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_GateH_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_GateH_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_GateH_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_GateH_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_GateH_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_GateH_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_GateH_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_GateH_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_GateH_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_GateH_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_GateH_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_GateH_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_GateH_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_GateH_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_GateH_enter__ENTRY_EVT 0x88b0001
#define xdc_runtime_knl_GateH_enter__EXIT_EVT 0x89b0002
#define xdc_runtime_knl_GateH_leave__ENTRY_EVT 0x8a90001
#define xdc_runtime_knl_GateH_leave__EXIT_EVT 0x8bd0002

/* enter__E */
xdc_IArg xdc_runtime_knl_GateH_enter__E( xdc_runtime_IGateProvider_Handle hdl ) 
{
    xdc_IArg __ret;

    xdc_runtime_Log_write1(xdc_runtime_knl_GateH_enter__ENTRY_EVT, (xdc_IArg)hdl);
    __ret = xdc_runtime_knl_GateH_enter__F(hdl);
    xdc_runtime_Log_write1(xdc_runtime_knl_GateH_enter__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* leave__E */
xdc_Void xdc_runtime_knl_GateH_leave__E( xdc_runtime_IGateProvider_Handle hdl, xdc_IArg key ) 
{
    xdc_runtime_Log_write2(xdc_runtime_knl_GateH_leave__ENTRY_EVT, (xdc_IArg)hdl, (xdc_IArg)key);
    xdc_runtime_knl_GateH_leave__F(hdl, key);
    xdc_runtime_Log_write1(xdc_runtime_knl_GateH_leave__EXIT_EVT, 0);
}


/*
 * ======== xdc.runtime.knl.GateProcess FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_GateProcess_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_GateProcess_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_GateProcess_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_GateProcess_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_GateProcess_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_GateProcess_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_GateProcess_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_GateProcess_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_GateProcess_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_GateProcess_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_GateProcess_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_GateProcess_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_GateProcess_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_GateProcess_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_GateProcess_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_GateProcess_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_GateProcess_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_GateProcess_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_GateProcess_query__ENTRY_EVT 0xb9b0001
#define xdc_runtime_knl_GateProcess_query__EXIT_EVT 0xb450002
#define xdc_runtime_knl_GateProcess_enter__ENTRY_EVT 0x88b0001
#define xdc_runtime_knl_GateProcess_enter__EXIT_EVT 0x89b0002
#define xdc_runtime_knl_GateProcess_leave__ENTRY_EVT 0x8a90001
#define xdc_runtime_knl_GateProcess_leave__EXIT_EVT 0x8bd0002
#define xdc_runtime_knl_GateProcess_getReferenceCount__ENTRY_EVT 0xbab0001
#define xdc_runtime_knl_GateProcess_getReferenceCount__EXIT_EVT 0xbcb0002

/* query__E */
xdc_Bool xdc_runtime_knl_GateProcess_query__E( xdc_Int qual ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(xdc_runtime_knl_GateProcess_query__ENTRY_EVT, (xdc_IArg)qual);
    __ret = xdc_runtime_knl_GateProcess_query__F(qual);
    xdc_runtime_Log_write1(xdc_runtime_knl_GateProcess_query__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* enter__E */
xdc_IArg xdc_runtime_knl_GateProcess_enter__E( xdc_runtime_knl_GateProcess_Handle __inst ) 
{
    xdc_IArg __ret;

    xdc_runtime_Log_write1(xdc_runtime_knl_GateProcess_enter__ENTRY_EVT, (xdc_IArg)__inst);
    __ret = xdc_runtime_knl_GateProcess_enter__F((void*)__inst);
    xdc_runtime_Log_write1(xdc_runtime_knl_GateProcess_enter__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* leave__E */
xdc_Void xdc_runtime_knl_GateProcess_leave__E( xdc_runtime_knl_GateProcess_Handle __inst, xdc_IArg key ) 
{
    xdc_runtime_Log_write2(xdc_runtime_knl_GateProcess_leave__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)key);
    xdc_runtime_knl_GateProcess_leave__F((void*)__inst, key);
    xdc_runtime_Log_write1(xdc_runtime_knl_GateProcess_leave__EXIT_EVT, 0);
}

/* getReferenceCount__E */
xdc_Int xdc_runtime_knl_GateProcess_getReferenceCount__E( xdc_runtime_knl_GateProcess_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_GateProcess_getReferenceCount__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_GateProcess_getReferenceCount__F((void*)__inst, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_GateProcess_getReferenceCount__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdc.runtime.knl.GateThread FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_GateThread_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_GateThread_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_GateThread_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_GateThread_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_GateThread_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_GateThread_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_GateThread_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_GateThread_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_GateThread_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_GateThread_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_GateThread_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_GateThread_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_GateThread_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_GateThread_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_GateThread_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_GateThread_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_GateThread_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_GateThread_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_GateThread_query__ENTRY_EVT 0xb9b0001
#define xdc_runtime_knl_GateThread_query__EXIT_EVT 0xb450002
#define xdc_runtime_knl_GateThread_enter__ENTRY_EVT 0x88b0001
#define xdc_runtime_knl_GateThread_enter__EXIT_EVT 0x89b0002
#define xdc_runtime_knl_GateThread_leave__ENTRY_EVT 0x8a90001
#define xdc_runtime_knl_GateThread_leave__EXIT_EVT 0x8bd0002

/* query__E */
xdc_Bool xdc_runtime_knl_GateThread_query__E( xdc_Int qual ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(xdc_runtime_knl_GateThread_query__ENTRY_EVT, (xdc_IArg)qual);
    __ret = xdc_runtime_knl_GateThread_query__F(qual);
    xdc_runtime_Log_write1(xdc_runtime_knl_GateThread_query__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* enter__E */
xdc_IArg xdc_runtime_knl_GateThread_enter__E( xdc_runtime_knl_GateThread_Handle __inst ) 
{
    xdc_IArg __ret;

    xdc_runtime_Log_write1(xdc_runtime_knl_GateThread_enter__ENTRY_EVT, (xdc_IArg)__inst);
    __ret = xdc_runtime_knl_GateThread_enter__F((void*)__inst);
    xdc_runtime_Log_write1(xdc_runtime_knl_GateThread_enter__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* leave__E */
xdc_Void xdc_runtime_knl_GateThread_leave__E( xdc_runtime_knl_GateThread_Handle __inst, xdc_IArg key ) 
{
    xdc_runtime_Log_write2(xdc_runtime_knl_GateThread_leave__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)key);
    xdc_runtime_knl_GateThread_leave__F((void*)__inst, key);
    xdc_runtime_Log_write1(xdc_runtime_knl_GateThread_leave__EXIT_EVT, 0);
}


/*
 * ======== xdc.runtime.knl.SemProcess FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SemProcess_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SemProcess_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SemProcess_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SemProcess_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SemProcess_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SemProcess_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SemProcess_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SemProcess_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SemProcess_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SemProcess_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SemProcess_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SemProcess_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SemProcess_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SemProcess_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SemProcess_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SemProcess_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SemProcess_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SemProcess_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_SemProcess_pend__ENTRY_EVT 0x8cd0001
#define xdc_runtime_knl_SemProcess_pend__EXIT_EVT 0x8e60002
#define xdc_runtime_knl_SemProcess_post__ENTRY_EVT 0x8f30001
#define xdc_runtime_knl_SemProcess_post__EXIT_EVT 0x9060002

/* pend__E */
xdc_Int xdc_runtime_knl_SemProcess_pend__E( xdc_runtime_knl_SemProcess_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_SemProcess_pend__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_SemProcess_pend__F((void*)__inst, timeout, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_SemProcess_pend__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* post__E */
xdc_Bool xdc_runtime_knl_SemProcess_post__E( xdc_runtime_knl_SemProcess_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_SemProcess_post__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_SemProcess_post__F((void*)__inst, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_SemProcess_post__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdc.runtime.knl.SemThread FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SemThread_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SemThread_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SemThread_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SemThread_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SemThread_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SemThread_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SemThread_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SemThread_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SemThread_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SemThread_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SemThread_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SemThread_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SemThread_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SemThread_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SemThread_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SemThread_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SemThread_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SemThread_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_SemThread_pend__ENTRY_EVT 0x8cd0001
#define xdc_runtime_knl_SemThread_pend__EXIT_EVT 0x8e60002
#define xdc_runtime_knl_SemThread_post__ENTRY_EVT 0x8f30001
#define xdc_runtime_knl_SemThread_post__EXIT_EVT 0x9060002

/* pend__E */
xdc_Int xdc_runtime_knl_SemThread_pend__E( xdc_runtime_knl_SemThread_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_SemThread_pend__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_SemThread_pend__F((void*)__inst, timeout, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_SemThread_pend__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* post__E */
xdc_Bool xdc_runtime_knl_SemThread_post__E( xdc_runtime_knl_SemThread_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_SemThread_post__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_SemThread_post__F((void*)__inst, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_SemThread_post__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdc.runtime.knl.Semaphore FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_Semaphore_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_Semaphore_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_Semaphore_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_Semaphore_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_Semaphore_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_Semaphore_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_Semaphore_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_Semaphore_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_Semaphore_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_Semaphore_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_Semaphore_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_Semaphore_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_Semaphore_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_Semaphore_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_Semaphore_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_Semaphore_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_Semaphore_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_Semaphore_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_Semaphore_pend__ENTRY_EVT 0x8cd0001
#define xdc_runtime_knl_Semaphore_pend__EXIT_EVT 0x8e60002
#define xdc_runtime_knl_Semaphore_post__ENTRY_EVT 0x8f30001
#define xdc_runtime_knl_Semaphore_post__EXIT_EVT 0x9060002

/* pend__E */
xdc_Int xdc_runtime_knl_Semaphore_pend__E( xdc_runtime_knl_ISemaphore_Handle sem, xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_Semaphore_pend__ENTRY_EVT, (xdc_IArg)sem, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Semaphore_pend__F(sem, timeout, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Semaphore_pend__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* post__E */
xdc_Bool xdc_runtime_knl_Semaphore_post__E( xdc_runtime_knl_ISemaphore_Handle sem, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_Semaphore_post__ENTRY_EVT, (xdc_IArg)sem, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Semaphore_post__F(sem, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Semaphore_post__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdc.runtime.knl.Sync FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_Sync_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_Sync_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_Sync_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_Sync_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_Sync_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_Sync_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_Sync_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_Sync_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_Sync_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_Sync_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_Sync_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_Sync_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_Sync_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_Sync_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_Sync_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_Sync_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_Sync_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_Sync_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_Sync_query__ENTRY_EVT 0xb310001
#define xdc_runtime_knl_Sync_query__EXIT_EVT 0xb450002
#define xdc_runtime_knl_Sync_signal__ENTRY_EVT 0xb530001
#define xdc_runtime_knl_Sync_signal__EXIT_EVT 0xb640002
#define xdc_runtime_knl_Sync_wait__ENTRY_EVT 0xb750001
#define xdc_runtime_knl_Sync_wait__EXIT_EVT 0xb8e0002

/* query__E */
xdc_Bool xdc_runtime_knl_Sync_query__E( xdc_runtime_knl_ISync_Handle sync, xdc_Int qual ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_Sync_query__ENTRY_EVT, (xdc_IArg)sync, (xdc_IArg)qual);
    __ret = xdc_runtime_knl_Sync_query__F(sync, qual);
    xdc_runtime_Log_write1(xdc_runtime_knl_Sync_query__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* signal__E */
xdc_Void xdc_runtime_knl_Sync_signal__E( xdc_runtime_knl_ISync_Handle sync ) 
{
    xdc_runtime_Log_write1(xdc_runtime_knl_Sync_signal__ENTRY_EVT, (xdc_IArg)sync);
    xdc_runtime_knl_Sync_signal__F(sync);
    xdc_runtime_Log_write1(xdc_runtime_knl_Sync_signal__EXIT_EVT, 0);
}

/* wait__E */
xdc_Int xdc_runtime_knl_Sync_wait__E( xdc_runtime_knl_ISync_Handle sync, xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_Sync_wait__ENTRY_EVT, (xdc_IArg)sync, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Sync_wait__F(sync, timeout, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Sync_wait__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdc.runtime.knl.SyncGeneric FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SyncGeneric_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SyncGeneric_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SyncGeneric_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SyncGeneric_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SyncGeneric_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SyncGeneric_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SyncGeneric_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SyncGeneric_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SyncGeneric_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SyncGeneric_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SyncGeneric_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SyncGeneric_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SyncGeneric_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SyncGeneric_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SyncGeneric_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SyncGeneric_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SyncGeneric_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SyncGeneric_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_SyncGeneric_query__ENTRY_EVT 0xb310001
#define xdc_runtime_knl_SyncGeneric_query__EXIT_EVT 0xb450002
#define xdc_runtime_knl_SyncGeneric_signal__ENTRY_EVT 0xb530001
#define xdc_runtime_knl_SyncGeneric_signal__EXIT_EVT 0xb640002
#define xdc_runtime_knl_SyncGeneric_wait__ENTRY_EVT 0xb750001
#define xdc_runtime_knl_SyncGeneric_wait__EXIT_EVT 0xb8e0002

/* query__E */
xdc_Bool xdc_runtime_knl_SyncGeneric_query__E( xdc_runtime_knl_SyncGeneric_Handle __inst, xdc_Int qual ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_SyncGeneric_query__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)qual);
    __ret = xdc_runtime_knl_SyncGeneric_query__F((void*)__inst, qual);
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncGeneric_query__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* signal__E */
xdc_Void xdc_runtime_knl_SyncGeneric_signal__E( xdc_runtime_knl_SyncGeneric_Handle __inst ) 
{
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncGeneric_signal__ENTRY_EVT, (xdc_IArg)__inst);
    xdc_runtime_knl_SyncGeneric_signal__F((void*)__inst);
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncGeneric_signal__EXIT_EVT, 0);
}

/* wait__E */
xdc_Int xdc_runtime_knl_SyncGeneric_wait__E( xdc_runtime_knl_SyncGeneric_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_SyncGeneric_wait__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_SyncGeneric_wait__F((void*)__inst, timeout, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncGeneric_wait__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdc.runtime.knl.SyncNull FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SyncNull_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SyncNull_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SyncNull_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SyncNull_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SyncNull_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SyncNull_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SyncNull_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SyncNull_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SyncNull_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SyncNull_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SyncNull_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SyncNull_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SyncNull_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SyncNull_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SyncNull_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SyncNull_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SyncNull_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SyncNull_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_SyncNull_query__ENTRY_EVT 0xb310001
#define xdc_runtime_knl_SyncNull_query__EXIT_EVT 0xb450002
#define xdc_runtime_knl_SyncNull_signal__ENTRY_EVT 0xb530001
#define xdc_runtime_knl_SyncNull_signal__EXIT_EVT 0xb640002
#define xdc_runtime_knl_SyncNull_wait__ENTRY_EVT 0xb750001
#define xdc_runtime_knl_SyncNull_wait__EXIT_EVT 0xb8e0002

/* query__E */
xdc_Bool xdc_runtime_knl_SyncNull_query__E( xdc_runtime_knl_SyncNull_Handle __inst, xdc_Int qual ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_SyncNull_query__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)qual);
    __ret = xdc_runtime_knl_SyncNull_query__F((void*)__inst, qual);
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncNull_query__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* signal__E */
xdc_Void xdc_runtime_knl_SyncNull_signal__E( xdc_runtime_knl_SyncNull_Handle __inst ) 
{
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncNull_signal__ENTRY_EVT, (xdc_IArg)__inst);
    xdc_runtime_knl_SyncNull_signal__F((void*)__inst);
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncNull_signal__EXIT_EVT, 0);
}

/* wait__E */
xdc_Int xdc_runtime_knl_SyncNull_wait__E( xdc_runtime_knl_SyncNull_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_SyncNull_wait__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_SyncNull_wait__F((void*)__inst, timeout, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncNull_wait__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdc.runtime.knl.SyncSemThread FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SyncSemThread_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SyncSemThread_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SyncSemThread_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SyncSemThread_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SyncSemThread_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SyncSemThread_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SyncSemThread_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SyncSemThread_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SyncSemThread_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SyncSemThread_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SyncSemThread_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SyncSemThread_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SyncSemThread_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SyncSemThread_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SyncSemThread_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SyncSemThread_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SyncSemThread_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SyncSemThread_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_SyncSemThread_query__ENTRY_EVT 0xb310001
#define xdc_runtime_knl_SyncSemThread_query__EXIT_EVT 0xb450002
#define xdc_runtime_knl_SyncSemThread_signal__ENTRY_EVT 0xb530001
#define xdc_runtime_knl_SyncSemThread_signal__EXIT_EVT 0xb640002
#define xdc_runtime_knl_SyncSemThread_wait__ENTRY_EVT 0xb750001
#define xdc_runtime_knl_SyncSemThread_wait__EXIT_EVT 0xb8e0002

/* query__E */
xdc_Bool xdc_runtime_knl_SyncSemThread_query__E( xdc_runtime_knl_SyncSemThread_Handle __inst, xdc_Int qual ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_SyncSemThread_query__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)qual);
    __ret = xdc_runtime_knl_SyncSemThread_query__F((void*)__inst, qual);
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncSemThread_query__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* signal__E */
xdc_Void xdc_runtime_knl_SyncSemThread_signal__E( xdc_runtime_knl_SyncSemThread_Handle __inst ) 
{
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncSemThread_signal__ENTRY_EVT, (xdc_IArg)__inst);
    xdc_runtime_knl_SyncSemThread_signal__F((void*)__inst);
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncSemThread_signal__EXIT_EVT, 0);
}

/* wait__E */
xdc_Int xdc_runtime_knl_SyncSemThread_wait__E( xdc_runtime_knl_SyncSemThread_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_SyncSemThread_wait__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_SyncSemThread_wait__F((void*)__inst, timeout, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_SyncSemThread_wait__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdc.runtime.knl.Thread FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_Thread_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_Thread_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_Thread_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_Thread_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_Thread_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_Thread_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_Thread_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_Thread_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_Thread_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_Thread_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_Thread_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_Thread_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_Thread_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_Thread_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_Thread_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_Thread_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_Thread_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_Thread_Module_GateProxy_query

/* entry/exit Log events */
#define xdc_runtime_knl_Thread_start__ENTRY_EVT 0x9130001
#define xdc_runtime_knl_Thread_start__EXIT_EVT 0x9230002
#define xdc_runtime_knl_Thread_yield__ENTRY_EVT 0x9310001
#define xdc_runtime_knl_Thread_yield__EXIT_EVT 0x9410002
#define xdc_runtime_knl_Thread_compareOsPriorities__ENTRY_EVT 0x94f0001
#define xdc_runtime_knl_Thread_compareOsPriorities__EXIT_EVT 0x9750002
#define xdc_runtime_knl_Thread_sleep__ENTRY_EVT 0x9910001
#define xdc_runtime_knl_Thread_sleep__EXIT_EVT 0x9a70002
#define xdc_runtime_knl_Thread_join__ENTRY_EVT 0x9b50001
#define xdc_runtime_knl_Thread_join__EXIT_EVT 0x9c80002
#define xdc_runtime_knl_Thread_getPriority__ENTRY_EVT 0x9d50001
#define xdc_runtime_knl_Thread_getPriority__EXIT_EVT 0x9ef0002
#define xdc_runtime_knl_Thread_setPriority__ENTRY_EVT 0xa030001
#define xdc_runtime_knl_Thread_setPriority__EXIT_EVT 0xa210002
#define xdc_runtime_knl_Thread_getOsPriority__ENTRY_EVT 0xa350001
#define xdc_runtime_knl_Thread_getOsPriority__EXIT_EVT 0xa510002
#define xdc_runtime_knl_Thread_setOsPriority__ENTRY_EVT 0xa670001
#define xdc_runtime_knl_Thread_setOsPriority__EXIT_EVT 0xa870002
#define xdc_runtime_knl_Thread_getOsHandle__ENTRY_EVT 0xa9d0001
#define xdc_runtime_knl_Thread_getOsHandle__EXIT_EVT 0xab30002
#define xdc_runtime_knl_Thread_getTls__ENTRY_EVT 0xac70001
#define xdc_runtime_knl_Thread_getTls__EXIT_EVT 0xad80002
#define xdc_runtime_knl_Thread_setTls__ENTRY_EVT 0xae70001
#define xdc_runtime_knl_Thread_setTls__EXIT_EVT 0xafc0002
#define xdc_runtime_knl_Thread_stat__ENTRY_EVT 0xb0d0001
#define xdc_runtime_knl_Thread_stat__EXIT_EVT 0xb240002

/* start__E */
xdc_Bool xdc_runtime_knl_Thread_start__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_start__ENTRY_EVT, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Thread_start__F(eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_start__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* yield__E */
xdc_Bool xdc_runtime_knl_Thread_yield__E( xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_yield__ENTRY_EVT, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Thread_yield__F(eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_yield__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* compareOsPriorities__E */
xdc_Int xdc_runtime_knl_Thread_compareOsPriorities__E( xdc_Int p1, xdc_Int p2, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_Thread_compareOsPriorities__ENTRY_EVT, (xdc_IArg)p1, (xdc_IArg)p2, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Thread_compareOsPriorities__F(p1, p2, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_compareOsPriorities__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* sleep__E */
xdc_Bool xdc_runtime_knl_Thread_sleep__E( xdc_UInt timeout, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_Thread_sleep__ENTRY_EVT, (xdc_IArg)timeout, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Thread_sleep__F(timeout, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_sleep__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* join__E */
xdc_Bool xdc_runtime_knl_Thread_join__E( xdc_runtime_knl_Thread_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_Thread_join__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Thread_join__F((void*)__inst, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_join__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getPriority__E */
xdc_runtime_knl_Thread_Priority xdc_runtime_knl_Thread_getPriority__E( xdc_runtime_knl_Thread_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_runtime_knl_Thread_Priority __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_Thread_getPriority__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Thread_getPriority__F((void*)__inst, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_getPriority__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* setPriority__E */
xdc_Bool xdc_runtime_knl_Thread_setPriority__E( xdc_runtime_knl_Thread_Handle __inst, xdc_runtime_knl_Thread_Priority newPri, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_Thread_setPriority__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)newPri, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Thread_setPriority__F((void*)__inst, newPri, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_setPriority__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getOsPriority__E */
xdc_Int xdc_runtime_knl_Thread_getOsPriority__E( xdc_runtime_knl_Thread_Handle __inst, xdc_runtime_Error_Block* eb ) 
{
    xdc_Int __ret;

    xdc_runtime_Log_write2(xdc_runtime_knl_Thread_getOsPriority__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Thread_getOsPriority__F((void*)__inst, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_getOsPriority__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* setOsPriority__E */
xdc_Bool xdc_runtime_knl_Thread_setOsPriority__E( xdc_runtime_knl_Thread_Handle __inst, xdc_Int newPri, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_Thread_setOsPriority__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)newPri, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Thread_setOsPriority__F((void*)__inst, newPri, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_setOsPriority__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getOsHandle__E */
xdc_Ptr xdc_runtime_knl_Thread_getOsHandle__E( xdc_runtime_knl_Thread_Handle __inst ) 
{
    xdc_Ptr __ret;

    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_getOsHandle__ENTRY_EVT, (xdc_IArg)__inst);
    __ret = xdc_runtime_knl_Thread_getOsHandle__F((void*)__inst);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_getOsHandle__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* getTls__E */
xdc_Ptr xdc_runtime_knl_Thread_getTls__E( xdc_runtime_knl_Thread_Handle __inst ) 
{
    xdc_Ptr __ret;

    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_getTls__ENTRY_EVT, (xdc_IArg)__inst);
    __ret = xdc_runtime_knl_Thread_getTls__F((void*)__inst);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_getTls__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* setTls__E */
xdc_Void xdc_runtime_knl_Thread_setTls__E( xdc_runtime_knl_Thread_Handle __inst, xdc_Ptr tls ) 
{
    xdc_runtime_Log_write2(xdc_runtime_knl_Thread_setTls__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)tls);
    xdc_runtime_knl_Thread_setTls__F((void*)__inst, tls);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_setTls__EXIT_EVT, 0);
}

/* stat__E */
xdc_Bool xdc_runtime_knl_Thread_stat__E( xdc_runtime_knl_Thread_Handle __inst, xdc_runtime_knl_Thread_Stat* buf, xdc_runtime_Error_Block* eb ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write3(xdc_runtime_knl_Thread_stat__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)buf, (xdc_IArg)eb);
    __ret = xdc_runtime_knl_Thread_stat__F((void*)__inst, buf, eb);
    xdc_runtime_Log_write1(xdc_runtime_knl_Thread_stat__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdcruntime.cstubs.LoggerFlex FUNCTION STUBS ========
 */

/* enable__E */
xdc_Bool xdcruntime_cstubs_LoggerFlex_enable__E( xdcruntime_cstubs_LoggerFlex_Handle __inst ) 
{
    return xdcruntime_cstubs_LoggerFlex_enable__F((void*)__inst);
}

/* disable__E */
xdc_Bool xdcruntime_cstubs_LoggerFlex_disable__E( xdcruntime_cstubs_LoggerFlex_Handle __inst ) 
{
    return xdcruntime_cstubs_LoggerFlex_disable__F((void*)__inst);
}

/* write0__E */
xdc_Void xdcruntime_cstubs_LoggerFlex_write0__E( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid ) 
{
    xdcruntime_cstubs_LoggerFlex_write0__F((void*)__inst, evt, mid);
}

/* write1__E */
xdc_Void xdcruntime_cstubs_LoggerFlex_write1__E( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 ) 
{
    xdcruntime_cstubs_LoggerFlex_write1__F((void*)__inst, evt, mid, a1);
}

/* write2__E */
xdc_Void xdcruntime_cstubs_LoggerFlex_write2__E( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 ) 
{
    xdcruntime_cstubs_LoggerFlex_write2__F((void*)__inst, evt, mid, a1, a2);
}

/* write4__E */
xdc_Void xdcruntime_cstubs_LoggerFlex_write4__E( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 ) 
{
    xdcruntime_cstubs_LoggerFlex_write4__F((void*)__inst, evt, mid, a1, a2, a3, a4);
}

/* write8__E */
xdc_Void xdcruntime_cstubs_LoggerFlex_write8__E( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 ) 
{
    xdcruntime_cstubs_LoggerFlex_write8__F((void*)__inst, evt, mid, a1, a2, a3, a4, a5, a6, a7, a8);
}

/* shutdown__E */
xdc_Void xdcruntime_cstubs_LoggerFlex_shutdown__E( xdc_Int status ) 
{
    xdcruntime_cstubs_LoggerFlex_shutdown__F(status);
}

/* Module_startup */
xdc_Int xdcruntime_cstubs_LoggerFlex_Module_startup__E( xdc_Int state )
{
    return xdcruntime_cstubs_LoggerFlex_Module_startup__F(state);
}


/*
 * ======== xdcruntime.heaps.HeapStdAlign FUNCTION STUBS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdcruntime_heaps_HeapStdAlign_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdcruntime_heaps_HeapStdAlign_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdcruntime_heaps_HeapStdAlign_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdcruntime_heaps_HeapStdAlign_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdcruntime_heaps_HeapStdAlign_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdcruntime_heaps_HeapStdAlign_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdcruntime_heaps_HeapStdAlign_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdcruntime_heaps_HeapStdAlign_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdcruntime_heaps_HeapStdAlign_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdcruntime_heaps_HeapStdAlign_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdcruntime_heaps_HeapStdAlign_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdcruntime_heaps_HeapStdAlign_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdcruntime_heaps_HeapStdAlign_Module_GateProxy_query

/* entry/exit Log events */
#define xdcruntime_heaps_HeapStdAlign_free__ENTRY_EVT 0xcf50001
#define xdcruntime_heaps_HeapStdAlign_free__EXIT_EVT 0xd0e0002
#define xdcruntime_heaps_HeapStdAlign_getStats__ENTRY_EVT 0xd1d0001
#define xdcruntime_heaps_HeapStdAlign_getStats__EXIT_EVT 0xd340002
#define xdcruntime_heaps_HeapStdAlign_alloc__ENTRY_EVT 0xd470001
#define xdcruntime_heaps_HeapStdAlign_alloc__EXIT_EVT 0xd670002
#define xdcruntime_heaps_HeapStdAlign_isBlocking__ENTRY_EVT 0xd750001
#define xdcruntime_heaps_HeapStdAlign_isBlocking__EXIT_EVT 0xd8a0002

/* free__E */
xdc_Void xdcruntime_heaps_HeapStdAlign_free__E( xdcruntime_heaps_HeapStdAlign_Handle __inst, xdc_Ptr block, xdc_SizeT size ) 
{
    xdc_runtime_Log_write3(xdcruntime_heaps_HeapStdAlign_free__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)block, (xdc_IArg)size);
    xdcruntime_heaps_HeapStdAlign_free__F((void*)__inst, block, size);
    xdc_runtime_Log_write1(xdcruntime_heaps_HeapStdAlign_free__EXIT_EVT, 0);
}

/* getStats__E */
xdc_Void xdcruntime_heaps_HeapStdAlign_getStats__E( xdcruntime_heaps_HeapStdAlign_Handle __inst, xdc_runtime_Memory_Stats* stats ) 
{
    xdc_runtime_Log_write2(xdcruntime_heaps_HeapStdAlign_getStats__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)stats);
    xdcruntime_heaps_HeapStdAlign_getStats__F((void*)__inst, stats);
    xdc_runtime_Log_write1(xdcruntime_heaps_HeapStdAlign_getStats__EXIT_EVT, 0);
}

/* alloc__E */
xdc_Ptr xdcruntime_heaps_HeapStdAlign_alloc__E( xdcruntime_heaps_HeapStdAlign_Handle __inst, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb ) 
{
    xdc_Ptr __ret;

    xdc_runtime_Log_write4(xdcruntime_heaps_HeapStdAlign_alloc__ENTRY_EVT, (xdc_IArg)__inst, (xdc_IArg)size, (xdc_IArg)align, (xdc_IArg)eb);
    __ret = xdcruntime_heaps_HeapStdAlign_alloc__F((void*)__inst, size, align, eb);
    xdc_runtime_Log_write1(xdcruntime_heaps_HeapStdAlign_alloc__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}

/* isBlocking__E */
xdc_Bool xdcruntime_heaps_HeapStdAlign_isBlocking__E( xdcruntime_heaps_HeapStdAlign_Handle __inst ) 
{
    xdc_Bool __ret;

    xdc_runtime_Log_write1(xdcruntime_heaps_HeapStdAlign_isBlocking__ENTRY_EVT, (xdc_IArg)__inst);
    __ret = xdcruntime_heaps_HeapStdAlign_isBlocking__F((void*)__inst);
    xdc_runtime_Log_write1(xdcruntime_heaps_HeapStdAlign_isBlocking__EXIT_EVT, (xdc_IArg)__ret);

    return __ret;
}


/*
 * ======== xdc.runtime.LoggerSys_TimestampProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sdo.xdcruntime.linux.TimestampPosix */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_LoggerSys_TimestampProxy_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_TimestampPosix_Module__startupDone__S();
}

/* get32__E */
xdc_Bits32 xdc_runtime_LoggerSys_TimestampProxy_get32__E( void )
{
    return ti_sdo_xdcruntime_linux_TimestampPosix_get32();
}

/* get64__E */
xdc_Void xdc_runtime_LoggerSys_TimestampProxy_get64__E( xdc_runtime_Types_Timestamp64* result )
{
    ti_sdo_xdcruntime_linux_TimestampPosix_get64(result);
}

/* getFreq__E */
xdc_Void xdc_runtime_LoggerSys_TimestampProxy_getFreq__E( xdc_runtime_Types_FreqHz* freq )
{
    ti_sdo_xdcruntime_linux_TimestampPosix_getFreq(freq);
}


/*
 * ======== xdc.runtime.Main_Module_GateProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sdo.xdcruntime.linux.GateThreadSupport */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Main_Module_GateProxy_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_Main_Module_GateProxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_Main_Module_GateProxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Object__create__S(oa, osz, aa, (ti_sdo_xdcruntime_linux_GateThreadSupport___ParamsPtr)pa, sizeof(xdc_runtime_IGateProvider_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_Main_Module_GateProxy_Object__delete__S( Ptr instp ) 
{
    ti_sdo_xdcruntime_linux_GateThreadSupport_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_Main_Module_GateProxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sdo_xdcruntime_linux_GateThreadSupport_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_Main_Module_GateProxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Handle__label__S(obj, lab);
}

/* query__E */
xdc_Bool xdc_runtime_Main_Module_GateProxy_query__E( xdc_Int qual )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_query(qual);
}

/* enter__E */
xdc_IArg xdc_runtime_Main_Module_GateProxy_enter__E( xdc_runtime_Main_Module_GateProxy_Handle __inst )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_enter((ti_sdo_xdcruntime_linux_GateThreadSupport_Handle)__inst);
}

/* leave__E */
xdc_Void xdc_runtime_Main_Module_GateProxy_leave__E( xdc_runtime_Main_Module_GateProxy_Handle __inst, xdc_IArg key )
{
    ti_sdo_xdcruntime_linux_GateThreadSupport_leave((ti_sdo_xdcruntime_linux_GateThreadSupport_Handle)__inst, key);
}


/*
 * ======== xdc.runtime.Memory_HeapProxy PROXY BODY ========
 */

/* DELEGATES TO xdcruntime.heaps.HeapStdAlign */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Memory_HeapProxy_Module__startupDone__S( void ) 
{
    return xdcruntime_heaps_HeapStdAlign_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_Memory_HeapProxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_Memory_HeapProxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return xdcruntime_heaps_HeapStdAlign_Object__create__S(oa, osz, aa, (xdcruntime_heaps_HeapStdAlign___ParamsPtr)pa, sizeof(xdc_runtime_IHeap_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_Memory_HeapProxy_Object__delete__S( Ptr instp ) 
{
    xdcruntime_heaps_HeapStdAlign_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_Memory_HeapProxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    xdcruntime_heaps_HeapStdAlign_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_Memory_HeapProxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return xdcruntime_heaps_HeapStdAlign_Handle__label__S(obj, lab);
}

/* alloc__E */
xdc_Ptr xdc_runtime_Memory_HeapProxy_alloc__E( xdc_runtime_Memory_HeapProxy_Handle __inst, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb )
{
    return xdc_runtime_IHeap_alloc((xdc_runtime_IHeap_Handle)__inst, size, align, eb);
}

/* free__E */
xdc_Void xdc_runtime_Memory_HeapProxy_free__E( xdc_runtime_Memory_HeapProxy_Handle __inst, xdc_Ptr block, xdc_SizeT size )
{
    xdc_runtime_IHeap_free((xdc_runtime_IHeap_Handle)__inst, block, size);
}

/* isBlocking__E */
xdc_Bool xdc_runtime_Memory_HeapProxy_isBlocking__E( xdc_runtime_Memory_HeapProxy_Handle __inst )
{
    return xdc_runtime_IHeap_isBlocking((xdc_runtime_IHeap_Handle)__inst);
}

/* getStats__E */
xdc_Void xdc_runtime_Memory_HeapProxy_getStats__E( xdc_runtime_Memory_HeapProxy_Handle __inst, xdc_runtime_Memory_Stats* stats )
{
    xdc_runtime_IHeap_getStats((xdc_runtime_IHeap_Handle)__inst, stats);
}


/*
 * ======== xdc.runtime.System_Module_GateProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sdo.xdcruntime.linux.GateThreadSupport */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_System_Module_GateProxy_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_System_Module_GateProxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_System_Module_GateProxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Object__create__S(oa, osz, aa, (ti_sdo_xdcruntime_linux_GateThreadSupport___ParamsPtr)pa, sizeof(xdc_runtime_IGateProvider_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_System_Module_GateProxy_Object__delete__S( Ptr instp ) 
{
    ti_sdo_xdcruntime_linux_GateThreadSupport_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_System_Module_GateProxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sdo_xdcruntime_linux_GateThreadSupport_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_System_Module_GateProxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Handle__label__S(obj, lab);
}

/* query__E */
xdc_Bool xdc_runtime_System_Module_GateProxy_query__E( xdc_Int qual )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_query(qual);
}

/* enter__E */
xdc_IArg xdc_runtime_System_Module_GateProxy_enter__E( xdc_runtime_System_Module_GateProxy_Handle __inst )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_enter((ti_sdo_xdcruntime_linux_GateThreadSupport_Handle)__inst);
}

/* leave__E */
xdc_Void xdc_runtime_System_Module_GateProxy_leave__E( xdc_runtime_System_Module_GateProxy_Handle __inst, xdc_IArg key )
{
    ti_sdo_xdcruntime_linux_GateThreadSupport_leave((ti_sdo_xdcruntime_linux_GateThreadSupport_Handle)__inst, key);
}


/*
 * ======== xdc.runtime.System_SupportProxy PROXY BODY ========
 */

/* DELEGATES TO xdc.runtime.SysStd */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_System_SupportProxy_Module__startupDone__S( void ) 
{
    return xdc_runtime_SysStd_Module__startupDone__S();
}

/* abort__E */
xdc_Void xdc_runtime_System_SupportProxy_abort__E( xdc_String str )
{
    xdc_runtime_SysStd_abort(str);
}

/* exit__E */
xdc_Void xdc_runtime_System_SupportProxy_exit__E( xdc_Int stat )
{
    xdc_runtime_SysStd_exit(stat);
}

/* flush__E */
xdc_Void xdc_runtime_System_SupportProxy_flush__E( void )
{
    xdc_runtime_SysStd_flush();
}

/* putch__E */
xdc_Void xdc_runtime_System_SupportProxy_putch__E( xdc_Char ch )
{
    xdc_runtime_SysStd_putch(ch);
}

/* ready__E */
xdc_Bool xdc_runtime_System_SupportProxy_ready__E( void )
{
    return xdc_runtime_SysStd_ready();
}


/*
 * ======== xdc.runtime.Timestamp_SupportProxy PROXY BODY ========
 */

/* DELEGATES TO ti.sdo.xdcruntime.linux.TimestampPosix */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Timestamp_SupportProxy_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_TimestampPosix_Module__startupDone__S();
}

/* get32__E */
xdc_Bits32 xdc_runtime_Timestamp_SupportProxy_get32__E( void )
{
    return ti_sdo_xdcruntime_linux_TimestampPosix_get32();
}

/* get64__E */
xdc_Void xdc_runtime_Timestamp_SupportProxy_get64__E( xdc_runtime_Types_Timestamp64* result )
{
    ti_sdo_xdcruntime_linux_TimestampPosix_get64(result);
}

/* getFreq__E */
xdc_Void xdc_runtime_Timestamp_SupportProxy_getFreq__E( xdc_runtime_Types_FreqHz* freq )
{
    ti_sdo_xdcruntime_linux_TimestampPosix_getFreq(freq);
}


/*
 * ======== xdc.runtime.knl.GateH_Proxy PROXY BODY ========
 */

/* DELEGATES TO xdc.runtime.knl.GateThread */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_GateH_Proxy_Module__startupDone__S( void ) 
{
    return xdc_runtime_knl_GateThread_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_GateH_Proxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_GateH_Proxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return xdc_runtime_knl_GateThread_Object__create__S(oa, osz, aa, (xdc_runtime_knl_GateThread___ParamsPtr)pa, sizeof(xdc_runtime_IGateProvider_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_knl_GateH_Proxy_Object__delete__S( Ptr instp ) 
{
    xdc_runtime_knl_GateThread_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_knl_GateH_Proxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    xdc_runtime_knl_GateThread_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_GateH_Proxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return xdc_runtime_knl_GateThread_Handle__label__S(obj, lab);
}

/* query__E */
xdc_Bool xdc_runtime_knl_GateH_Proxy_query__E( xdc_Int qual )
{
    return xdc_runtime_knl_GateThread_query(qual);
}

/* enter__E */
xdc_IArg xdc_runtime_knl_GateH_Proxy_enter__E( xdc_runtime_knl_GateH_Proxy_Handle __inst )
{
    return xdc_runtime_IGateProvider_enter((xdc_runtime_IGateProvider_Handle)__inst);
}

/* leave__E */
xdc_Void xdc_runtime_knl_GateH_Proxy_leave__E( xdc_runtime_knl_GateH_Proxy_Handle __inst, xdc_IArg key )
{
    xdc_runtime_IGateProvider_leave((xdc_runtime_IGateProvider_Handle)__inst, key);
}


/*
 * ======== xdc.runtime.knl.GateProcess_Proxy PROXY BODY ========
 */

/* DELEGATES TO ti.sdo.xdcruntime.linux.GateProcessSupport */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_GateProcess_Proxy_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_GateProcessSupport_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_GateProcess_Proxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_GateProcess_Proxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_GateProcessSupport_Object__create__S(oa, osz, aa, (ti_sdo_xdcruntime_linux_GateProcessSupport___ParamsPtr)pa, sizeof(xdc_runtime_knl_IGateProcessSupport_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_knl_GateProcess_Proxy_Object__delete__S( Ptr instp ) 
{
    ti_sdo_xdcruntime_linux_GateProcessSupport_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_knl_GateProcess_Proxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sdo_xdcruntime_linux_GateProcessSupport_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_GateProcess_Proxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return ti_sdo_xdcruntime_linux_GateProcessSupport_Handle__label__S(obj, lab);
}

/* query__E */
xdc_Bool xdc_runtime_knl_GateProcess_Proxy_query__E( xdc_Int qual )
{
    return ti_sdo_xdcruntime_linux_GateProcessSupport_query(qual);
}

/* enter__E */
xdc_IArg xdc_runtime_knl_GateProcess_Proxy_enter__E( xdc_runtime_knl_GateProcess_Proxy_Handle __inst )
{
    return ti_sdo_xdcruntime_linux_GateProcessSupport_enter((ti_sdo_xdcruntime_linux_GateProcessSupport_Handle)__inst);
}

/* leave__E */
xdc_Void xdc_runtime_knl_GateProcess_Proxy_leave__E( xdc_runtime_knl_GateProcess_Proxy_Handle __inst, xdc_IArg key )
{
    ti_sdo_xdcruntime_linux_GateProcessSupport_leave((ti_sdo_xdcruntime_linux_GateProcessSupport_Handle)__inst, key);
}

/* getReferenceCount__E */
xdc_Int xdc_runtime_knl_GateProcess_Proxy_getReferenceCount__E( xdc_runtime_knl_GateProcess_Proxy_Handle __inst, xdc_runtime_Error_Block* err )
{
    return ti_sdo_xdcruntime_linux_GateProcessSupport_getReferenceCount((ti_sdo_xdcruntime_linux_GateProcessSupport_Handle)__inst, err);
}


/*
 * ======== xdc.runtime.knl.GateThread_Proxy PROXY BODY ========
 */

/* DELEGATES TO ti.sdo.xdcruntime.linux.GateThreadSupport */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_GateThread_Proxy_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_GateThread_Proxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_GateThread_Proxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Object__create__S(oa, osz, aa, (ti_sdo_xdcruntime_linux_GateThreadSupport___ParamsPtr)pa, sizeof(xdc_runtime_knl_IGateThreadSupport_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_knl_GateThread_Proxy_Object__delete__S( Ptr instp ) 
{
    ti_sdo_xdcruntime_linux_GateThreadSupport_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_knl_GateThread_Proxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sdo_xdcruntime_linux_GateThreadSupport_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_GateThread_Proxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Handle__label__S(obj, lab);
}

/* query__E */
xdc_Bool xdc_runtime_knl_GateThread_Proxy_query__E( xdc_Int qual )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_query(qual);
}

/* enter__E */
xdc_IArg xdc_runtime_knl_GateThread_Proxy_enter__E( xdc_runtime_knl_GateThread_Proxy_Handle __inst )
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_enter((ti_sdo_xdcruntime_linux_GateThreadSupport_Handle)__inst);
}

/* leave__E */
xdc_Void xdc_runtime_knl_GateThread_Proxy_leave__E( xdc_runtime_knl_GateThread_Proxy_Handle __inst, xdc_IArg key )
{
    ti_sdo_xdcruntime_linux_GateThreadSupport_leave((ti_sdo_xdcruntime_linux_GateThreadSupport_Handle)__inst, key);
}


/*
 * ======== xdc.runtime.knl.SemProcess_Proxy PROXY BODY ========
 */

/* DELEGATES TO ti.sdo.xdcruntime.linux.SemProcessSupport */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_SemProcess_Proxy_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_SemProcessSupport_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_SemProcess_Proxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_SemProcess_Proxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_SemProcessSupport_Object__create__S(oa, osz, aa, (ti_sdo_xdcruntime_linux_SemProcessSupport___ParamsPtr)pa, sizeof(xdc_runtime_knl_ISemProcessSupport_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_knl_SemProcess_Proxy_Object__delete__S( Ptr instp ) 
{
    ti_sdo_xdcruntime_linux_SemProcessSupport_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_knl_SemProcess_Proxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sdo_xdcruntime_linux_SemProcessSupport_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_SemProcess_Proxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return ti_sdo_xdcruntime_linux_SemProcessSupport_Handle__label__S(obj, lab);
}

/* pend__E */
xdc_Int xdc_runtime_knl_SemProcess_Proxy_pend__E( xdc_runtime_knl_SemProcess_Proxy_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_SemProcessSupport_pend((ti_sdo_xdcruntime_linux_SemProcessSupport_Handle)__inst, timeout, eb);
}

/* post__E */
xdc_Bool xdc_runtime_knl_SemProcess_Proxy_post__E( xdc_runtime_knl_SemProcess_Proxy_Handle __inst, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_SemProcessSupport_post((ti_sdo_xdcruntime_linux_SemProcessSupport_Handle)__inst, eb);
}


/*
 * ======== xdc.runtime.knl.SemThread_Proxy PROXY BODY ========
 */

/* DELEGATES TO ti.sdo.xdcruntime.linux.SemThreadSupport */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_SemThread_Proxy_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_SemThreadSupport_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_SemThread_Proxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_SemThread_Proxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_SemThreadSupport_Object__create__S(oa, osz, aa, (ti_sdo_xdcruntime_linux_SemThreadSupport___ParamsPtr)pa, sizeof(xdc_runtime_knl_ISemThreadSupport_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_knl_SemThread_Proxy_Object__delete__S( Ptr instp ) 
{
    ti_sdo_xdcruntime_linux_SemThreadSupport_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_knl_SemThread_Proxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sdo_xdcruntime_linux_SemThreadSupport_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_SemThread_Proxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return ti_sdo_xdcruntime_linux_SemThreadSupport_Handle__label__S(obj, lab);
}

/* pend__E */
xdc_Int xdc_runtime_knl_SemThread_Proxy_pend__E( xdc_runtime_knl_SemThread_Proxy_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_SemThreadSupport_pend((ti_sdo_xdcruntime_linux_SemThreadSupport_Handle)__inst, timeout, eb);
}

/* post__E */
xdc_Bool xdc_runtime_knl_SemThread_Proxy_post__E( xdc_runtime_knl_SemThread_Proxy_Handle __inst, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_SemThreadSupport_post((ti_sdo_xdcruntime_linux_SemThreadSupport_Handle)__inst, eb);
}


/*
 * ======== xdc.runtime.knl.Semaphore_Proxy PROXY BODY ========
 */

/* DELEGATES TO xdc.runtime.knl.SemThread */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_Semaphore_Proxy_Module__startupDone__S( void ) 
{
    return xdc_runtime_knl_SemThread_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_Semaphore_Proxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_Semaphore_Proxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return xdc_runtime_knl_SemThread_Object__create__S(oa, osz, aa, (xdc_runtime_knl_SemThread___ParamsPtr)pa, sizeof(xdc_runtime_knl_ISemaphore_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_knl_Semaphore_Proxy_Object__delete__S( Ptr instp ) 
{
    xdc_runtime_knl_SemThread_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_knl_Semaphore_Proxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    xdc_runtime_knl_SemThread_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_Semaphore_Proxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return xdc_runtime_knl_SemThread_Handle__label__S(obj, lab);
}

/* pend__E */
xdc_Int xdc_runtime_knl_Semaphore_Proxy_pend__E( xdc_runtime_knl_Semaphore_Proxy_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb )
{
    return xdc_runtime_knl_ISemaphore_pend((xdc_runtime_knl_ISemaphore_Handle)__inst, timeout, eb);
}

/* post__E */
xdc_Bool xdc_runtime_knl_Semaphore_Proxy_post__E( xdc_runtime_knl_Semaphore_Proxy_Handle __inst, xdc_runtime_Error_Block* eb )
{
    return xdc_runtime_knl_ISemaphore_post((xdc_runtime_knl_ISemaphore_Handle)__inst, eb);
}


/*
 * ======== xdc.runtime.knl.Sync_Proxy PROXY BODY ========
 */

/* DELEGATES TO xdc.runtime.knl.SyncNull */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_Sync_Proxy_Module__startupDone__S( void ) 
{
    return xdc_runtime_knl_SyncNull_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_Sync_Proxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_Sync_Proxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return xdc_runtime_knl_SyncNull_Object__create__S(oa, osz, aa, (xdc_runtime_knl_SyncNull___ParamsPtr)pa, sizeof(xdc_runtime_knl_ISync_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_knl_Sync_Proxy_Object__delete__S( Ptr instp ) 
{
    xdc_runtime_knl_SyncNull_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_knl_Sync_Proxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    xdc_runtime_knl_SyncNull_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_Sync_Proxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return xdc_runtime_knl_SyncNull_Handle__label__S(obj, lab);
}

/* query__E */
xdc_Bool xdc_runtime_knl_Sync_Proxy_query__E( xdc_runtime_knl_Sync_Proxy_Handle __inst, xdc_Int qual )
{
    return xdc_runtime_knl_ISync_query((xdc_runtime_knl_ISync_Handle)__inst, qual);
}

/* signal__E */
xdc_Void xdc_runtime_knl_Sync_Proxy_signal__E( xdc_runtime_knl_Sync_Proxy_Handle __inst )
{
    xdc_runtime_knl_ISync_signal((xdc_runtime_knl_ISync_Handle)__inst);
}

/* wait__E */
xdc_Int xdc_runtime_knl_Sync_Proxy_wait__E( xdc_runtime_knl_Sync_Proxy_Handle __inst, xdc_UInt timeout, xdc_runtime_Error_Block* eb )
{
    return xdc_runtime_knl_ISync_wait((xdc_runtime_knl_ISync_Handle)__inst, timeout, eb);
}


/*
 * ======== xdc.runtime.knl.Thread_Proxy PROXY BODY ========
 */

/* DELEGATES TO ti.sdo.xdcruntime.linux.ThreadSupport */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_Thread_Proxy_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_Module__startupDone__S();
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_Thread_Proxy_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_Thread_Proxy___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_Object__create__S(oa, osz, aa, (ti_sdo_xdcruntime_linux_ThreadSupport___ParamsPtr)pa, sizeof(xdc_runtime_knl_IThreadSupport_Params), eb);
}

/* Object__delete__S */
void xdc_runtime_knl_Thread_Proxy_Object__delete__S( Ptr instp ) 
{
    ti_sdo_xdcruntime_linux_ThreadSupport_Object__delete__S(instp);
}

/* Params__init__S */
void xdc_runtime_knl_Thread_Proxy_Params__init__S( xdc_Ptr dst, const xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz )
{
    ti_sdo_xdcruntime_linux_ThreadSupport_Params__init__S(dst, src, psz, isz);
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_Thread_Proxy_Handle__label__S( Ptr obj, xdc_runtime_Types_Label* lab )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_Handle__label__S(obj, lab);
}

/* self__E */
xdc_runtime_knl_IThreadSupport_Handle xdc_runtime_knl_Thread_Proxy_self__E( xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_self(eb);
}

/* start__E */
xdc_Bool xdc_runtime_knl_Thread_Proxy_start__E( xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_start(eb);
}

/* yield__E */
xdc_Bool xdc_runtime_knl_Thread_Proxy_yield__E( xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_yield(eb);
}

/* compareOsPriorities__E */
xdc_Int xdc_runtime_knl_Thread_Proxy_compareOsPriorities__E( xdc_Int p1, xdc_Int p2, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_compareOsPriorities(p1, p2, eb);
}

/* sleep__E */
xdc_Bool xdc_runtime_knl_Thread_Proxy_sleep__E( xdc_UInt timeout, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_sleep(timeout, eb);
}

/* join__E */
xdc_Bool xdc_runtime_knl_Thread_Proxy_join__E( xdc_runtime_knl_Thread_Proxy_Handle __inst, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_join((ti_sdo_xdcruntime_linux_ThreadSupport_Handle)__inst, eb);
}

/* getPriority__E */
xdc_runtime_knl_IThreadSupport_Priority xdc_runtime_knl_Thread_Proxy_getPriority__E( xdc_runtime_knl_Thread_Proxy_Handle __inst, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_getPriority((ti_sdo_xdcruntime_linux_ThreadSupport_Handle)__inst, eb);
}

/* setPriority__E */
xdc_Bool xdc_runtime_knl_Thread_Proxy_setPriority__E( xdc_runtime_knl_Thread_Proxy_Handle __inst, xdc_runtime_knl_IThreadSupport_Priority newPri, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_setPriority((ti_sdo_xdcruntime_linux_ThreadSupport_Handle)__inst, newPri, eb);
}

/* getOsPriority__E */
xdc_Int xdc_runtime_knl_Thread_Proxy_getOsPriority__E( xdc_runtime_knl_Thread_Proxy_Handle __inst, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_getOsPriority((ti_sdo_xdcruntime_linux_ThreadSupport_Handle)__inst, eb);
}

/* setOsPriority__E */
xdc_Bool xdc_runtime_knl_Thread_Proxy_setOsPriority__E( xdc_runtime_knl_Thread_Proxy_Handle __inst, xdc_Int newPri, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_setOsPriority((ti_sdo_xdcruntime_linux_ThreadSupport_Handle)__inst, newPri, eb);
}

/* getOsHandle__E */
xdc_Ptr xdc_runtime_knl_Thread_Proxy_getOsHandle__E( xdc_runtime_knl_Thread_Proxy_Handle __inst )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_getOsHandle((ti_sdo_xdcruntime_linux_ThreadSupport_Handle)__inst);
}

/* getTls__E */
xdc_Ptr xdc_runtime_knl_Thread_Proxy_getTls__E( xdc_runtime_knl_Thread_Proxy_Handle __inst )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_getTls((ti_sdo_xdcruntime_linux_ThreadSupport_Handle)__inst);
}

/* setTls__E */
xdc_Void xdc_runtime_knl_Thread_Proxy_setTls__E( xdc_runtime_knl_Thread_Proxy_Handle __inst, xdc_Ptr tls )
{
    ti_sdo_xdcruntime_linux_ThreadSupport_setTls((ti_sdo_xdcruntime_linux_ThreadSupport_Handle)__inst, tls);
}

/* stat__E */
xdc_Bool xdc_runtime_knl_Thread_Proxy_stat__E( xdc_runtime_knl_Thread_Proxy_Handle __inst, xdc_runtime_knl_IThreadSupport_Stat* buf, xdc_runtime_Error_Block* eb )
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_stat((ti_sdo_xdcruntime_linux_ThreadSupport_Handle)__inst, buf, eb);
}


/*
 * ======== ti.sdo.xdcruntime.linux.GateProcessSupport OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sdo_xdcruntime_linux_GateProcessSupport_Object2__ s0; char c; } ti_sdo_xdcruntime_linux_GateProcessSupport___S1;
__FAR__ const xdc_runtime_Core_ObjDesc ti_sdo_xdcruntime_linux_GateProcessSupport_Object__DESC__C = {
    (Ptr)0, /* fxnTab */
    &ti_sdo_xdcruntime_linux_GateProcessSupport_Module__root__V.link, /* modLink */
    sizeof(ti_sdo_xdcruntime_linux_GateProcessSupport___S1) - sizeof(ti_sdo_xdcruntime_linux_GateProcessSupport_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sdo_xdcruntime_linux_GateProcessSupport_Object2__), /* objSize */
    (Ptr)&ti_sdo_xdcruntime_linux_GateProcessSupport_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sdo_xdcruntime_linux_GateProcessSupport_Params), /* prmsSize */
};



/*
 * ======== ti.sdo.xdcruntime.linux.GateThreadSupport OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sdo_xdcruntime_linux_GateThreadSupport_Object2__ s0; char c; } ti_sdo_xdcruntime_linux_GateThreadSupport___S1;
__FAR__ const xdc_runtime_Core_ObjDesc ti_sdo_xdcruntime_linux_GateThreadSupport_Object__DESC__C = {
    (Ptr)0, /* fxnTab */
    &ti_sdo_xdcruntime_linux_GateThreadSupport_Module__root__V.link, /* modLink */
    sizeof(ti_sdo_xdcruntime_linux_GateThreadSupport___S1) - sizeof(ti_sdo_xdcruntime_linux_GateThreadSupport_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sdo_xdcruntime_linux_GateThreadSupport_Object2__), /* objSize */
    (Ptr)&ti_sdo_xdcruntime_linux_GateThreadSupport_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sdo_xdcruntime_linux_GateThreadSupport_Params), /* prmsSize */
};



/*
 * ======== ti.sdo.xdcruntime.linux.SemProcessSupport OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sdo_xdcruntime_linux_SemProcessSupport_Object2__ s0; char c; } ti_sdo_xdcruntime_linux_SemProcessSupport___S1;
__FAR__ const xdc_runtime_Core_ObjDesc ti_sdo_xdcruntime_linux_SemProcessSupport_Object__DESC__C = {
    (Ptr)0, /* fxnTab */
    &ti_sdo_xdcruntime_linux_SemProcessSupport_Module__root__V.link, /* modLink */
    sizeof(ti_sdo_xdcruntime_linux_SemProcessSupport___S1) - sizeof(ti_sdo_xdcruntime_linux_SemProcessSupport_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sdo_xdcruntime_linux_SemProcessSupport_Object2__), /* objSize */
    (Ptr)&ti_sdo_xdcruntime_linux_SemProcessSupport_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sdo_xdcruntime_linux_SemProcessSupport_Params), /* prmsSize */
};



/*
 * ======== ti.sdo.xdcruntime.linux.SemThreadSupport OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sdo_xdcruntime_linux_SemThreadSupport_Object2__ s0; char c; } ti_sdo_xdcruntime_linux_SemThreadSupport___S1;
__FAR__ const xdc_runtime_Core_ObjDesc ti_sdo_xdcruntime_linux_SemThreadSupport_Object__DESC__C = {
    (Ptr)0, /* fxnTab */
    &ti_sdo_xdcruntime_linux_SemThreadSupport_Module__root__V.link, /* modLink */
    sizeof(ti_sdo_xdcruntime_linux_SemThreadSupport___S1) - sizeof(ti_sdo_xdcruntime_linux_SemThreadSupport_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sdo_xdcruntime_linux_SemThreadSupport_Object2__), /* objSize */
    (Ptr)&ti_sdo_xdcruntime_linux_SemThreadSupport_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sdo_xdcruntime_linux_SemThreadSupport_Params), /* prmsSize */
};



/*
 * ======== ti.sdo.xdcruntime.linux.ThreadSupport OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { ti_sdo_xdcruntime_linux_ThreadSupport_Object2__ s0; char c; } ti_sdo_xdcruntime_linux_ThreadSupport___S1;
__FAR__ const xdc_runtime_Core_ObjDesc ti_sdo_xdcruntime_linux_ThreadSupport_Object__DESC__C = {
    (Ptr)0, /* fxnTab */
    &ti_sdo_xdcruntime_linux_ThreadSupport_Module__root__V.link, /* modLink */
    sizeof(ti_sdo_xdcruntime_linux_ThreadSupport___S1) - sizeof(ti_sdo_xdcruntime_linux_ThreadSupport_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(ti_sdo_xdcruntime_linux_ThreadSupport_Object2__), /* objSize */
    (Ptr)&ti_sdo_xdcruntime_linux_ThreadSupport_Object__PARAMS__C, /* prmsInit */
    sizeof(ti_sdo_xdcruntime_linux_ThreadSupport_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.GateNull OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_GateNull_Object2__ s0; char c; } xdc_runtime_GateNull___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_GateNull_Object__DESC__C = {
    (Ptr)&xdc_runtime_GateNull_Module__FXNS__C, /* fxnTab */
    &xdc_runtime_GateNull_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_GateNull___S1) - sizeof(xdc_runtime_GateNull_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_GateNull_Object2__), /* objSize */
    (Ptr)&xdc_runtime_GateNull_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_GateNull_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.HeapStd OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_HeapStd_Object2__ s0; char c; } xdc_runtime_HeapStd___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_HeapStd_Object__DESC__C = {
    (Ptr)&xdc_runtime_HeapStd_Module__FXNS__C, /* fxnTab */
    &xdc_runtime_HeapStd_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_HeapStd___S1) - sizeof(xdc_runtime_HeapStd_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_HeapStd_Object2__), /* objSize */
    (Ptr)&xdc_runtime_HeapStd_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_HeapStd_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.LoggerSys OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_LoggerSys_Object2__ s0; char c; } xdc_runtime_LoggerSys___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_LoggerSys_Object__DESC__C = {
    (Ptr)&xdc_runtime_LoggerSys_Module__FXNS__C, /* fxnTab */
    &xdc_runtime_LoggerSys_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_LoggerSys___S1) - sizeof(xdc_runtime_LoggerSys_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_LoggerSys_Object2__), /* objSize */
    (Ptr)&xdc_runtime_LoggerSys_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_LoggerSys_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.knl.GateProcess OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_knl_GateProcess_Object2__ s0; char c; } xdc_runtime_knl_GateProcess___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_GateProcess_Object__DESC__C = {
    (Ptr)&xdc_runtime_knl_GateProcess_Module__FXNS__C, /* fxnTab */
    &xdc_runtime_knl_GateProcess_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_knl_GateProcess___S1) - sizeof(xdc_runtime_knl_GateProcess_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_knl_GateProcess_Object2__), /* objSize */
    (Ptr)&xdc_runtime_knl_GateProcess_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_knl_GateProcess_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.knl.GateThread OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_knl_GateThread_Object2__ s0; char c; } xdc_runtime_knl_GateThread___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_GateThread_Object__DESC__C = {
    (Ptr)&xdc_runtime_knl_GateThread_Module__FXNS__C, /* fxnTab */
    &xdc_runtime_knl_GateThread_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_knl_GateThread___S1) - sizeof(xdc_runtime_knl_GateThread_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_knl_GateThread_Object2__), /* objSize */
    (Ptr)&xdc_runtime_knl_GateThread_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_knl_GateThread_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.knl.SemProcess OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_knl_SemProcess_Object2__ s0; char c; } xdc_runtime_knl_SemProcess___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_SemProcess_Object__DESC__C = {
    (Ptr)&xdc_runtime_knl_SemProcess_Module__FXNS__C, /* fxnTab */
    &xdc_runtime_knl_SemProcess_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_knl_SemProcess___S1) - sizeof(xdc_runtime_knl_SemProcess_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_knl_SemProcess_Object2__), /* objSize */
    (Ptr)&xdc_runtime_knl_SemProcess_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_knl_SemProcess_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.knl.SemThread OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_knl_SemThread_Object2__ s0; char c; } xdc_runtime_knl_SemThread___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_SemThread_Object__DESC__C = {
    (Ptr)&xdc_runtime_knl_SemThread_Module__FXNS__C, /* fxnTab */
    &xdc_runtime_knl_SemThread_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_knl_SemThread___S1) - sizeof(xdc_runtime_knl_SemThread_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_knl_SemThread_Object2__), /* objSize */
    (Ptr)&xdc_runtime_knl_SemThread_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_knl_SemThread_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.knl.SyncGeneric OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_knl_SyncGeneric_Object2__ s0; char c; } xdc_runtime_knl_SyncGeneric___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_SyncGeneric_Object__DESC__C = {
    (Ptr)&xdc_runtime_knl_SyncGeneric_Module__FXNS__C, /* fxnTab */
    &xdc_runtime_knl_SyncGeneric_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_knl_SyncGeneric___S1) - sizeof(xdc_runtime_knl_SyncGeneric_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_knl_SyncGeneric_Object2__), /* objSize */
    (Ptr)&xdc_runtime_knl_SyncGeneric_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_knl_SyncGeneric_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.knl.SyncNull OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_knl_SyncNull_Object2__ s0; char c; } xdc_runtime_knl_SyncNull___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_SyncNull_Object__DESC__C = {
    (Ptr)&xdc_runtime_knl_SyncNull_Module__FXNS__C, /* fxnTab */
    &xdc_runtime_knl_SyncNull_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_knl_SyncNull___S1) - sizeof(xdc_runtime_knl_SyncNull_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_knl_SyncNull_Object2__), /* objSize */
    (Ptr)&xdc_runtime_knl_SyncNull_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_knl_SyncNull_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.knl.SyncSemThread OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_knl_SyncSemThread_Object2__ s0; char c; } xdc_runtime_knl_SyncSemThread___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_SyncSemThread_Object__DESC__C = {
    (Ptr)&xdc_runtime_knl_SyncSemThread_Module__FXNS__C, /* fxnTab */
    &xdc_runtime_knl_SyncSemThread_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_knl_SyncSemThread___S1) - sizeof(xdc_runtime_knl_SyncSemThread_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_knl_SyncSemThread_Object2__), /* objSize */
    (Ptr)&xdc_runtime_knl_SyncSemThread_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_knl_SyncSemThread_Params), /* prmsSize */
};



/*
 * ======== xdc.runtime.knl.Thread OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdc_runtime_knl_Thread_Object2__ s0; char c; } xdc_runtime_knl_Thread___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdc_runtime_knl_Thread_Object__DESC__C = {
    (Ptr)-1, /* fxnTab */
    &xdc_runtime_knl_Thread_Module__root__V.link, /* modLink */
    sizeof(xdc_runtime_knl_Thread___S1) - sizeof(xdc_runtime_knl_Thread_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdc_runtime_knl_Thread_Object2__), /* objSize */
    (Ptr)&xdc_runtime_knl_Thread_Object__PARAMS__C, /* prmsInit */
    sizeof(xdc_runtime_knl_Thread_Params), /* prmsSize */
};



/*
 * ======== xdcruntime.cstubs.LoggerFlex OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdcruntime_cstubs_LoggerFlex_Object2__ s0; char c; } xdcruntime_cstubs_LoggerFlex___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdcruntime_cstubs_LoggerFlex_Object__DESC__C = {
    (Ptr)&xdcruntime_cstubs_LoggerFlex_Module__FXNS__C, /* fxnTab */
    &xdcruntime_cstubs_LoggerFlex_Module__root__V.link, /* modLink */
    sizeof(xdcruntime_cstubs_LoggerFlex___S1) - sizeof(xdcruntime_cstubs_LoggerFlex_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdcruntime_cstubs_LoggerFlex_Object2__), /* objSize */
    (Ptr)&xdcruntime_cstubs_LoggerFlex_Object__PARAMS__C, /* prmsInit */
    sizeof(xdcruntime_cstubs_LoggerFlex_Params), /* prmsSize */
};



/*
 * ======== xdcruntime.heaps.HeapStdAlign OBJECT DESCRIPTOR ========
 */

/* Object__DESC__C */
typedef struct { xdcruntime_heaps_HeapStdAlign_Object2__ s0; char c; } xdcruntime_heaps_HeapStdAlign___S1;
__FAR__ const xdc_runtime_Core_ObjDesc xdcruntime_heaps_HeapStdAlign_Object__DESC__C = {
    (Ptr)&xdcruntime_heaps_HeapStdAlign_Module__FXNS__C, /* fxnTab */
    &xdcruntime_heaps_HeapStdAlign_Module__root__V.link, /* modLink */
    sizeof(xdcruntime_heaps_HeapStdAlign___S1) - sizeof(xdcruntime_heaps_HeapStdAlign_Object2__), /* objAlign */
    0, /* objHeap */
    0, /* objName */
    sizeof(xdcruntime_heaps_HeapStdAlign_Object2__), /* objSize */
    (Ptr)&xdcruntime_heaps_HeapStdAlign_Object__PARAMS__C, /* prmsInit */
    sizeof(xdcruntime_heaps_HeapStdAlign_Params), /* prmsSize */
};



/*
 * ======== ti.sdo.xdcruntime.linux.GateProcessSupport SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_GateProcessSupport_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_GateProcessSupport_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_GateProcessSupport_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_GateProcessSupport_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_GateProcessSupport_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_GateProcessSupport_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_GateProcessSupport_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_GateProcessSupport_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_GateProcessSupport_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_GateProcessSupport_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sdo_xdcruntime_linux_GateProcessSupport_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_GateProcessSupport_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label* ti_sdo_xdcruntime_linux_GateProcessSupport_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32805;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sdo_xdcruntime_linux_GateProcessSupport_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &ti_sdo_xdcruntime_linux_GateProcessSupport_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sdo_xdcruntime_linux_GateProcessSupport_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((ti_sdo_xdcruntime_linux_GateProcessSupport_Object__*)oa) + i;
    }

    if (ti_sdo_xdcruntime_linux_GateProcessSupport_Object__count__C == 0) {
        return NULL;
    }

    return ((ti_sdo_xdcruntime_linux_GateProcessSupport_Object__*)ti_sdo_xdcruntime_linux_GateProcessSupport_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sdo_xdcruntime_linux_GateProcessSupport_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sdo_xdcruntime_linux_GateProcessSupport_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sdo_xdcruntime_linux_GateProcessSupport_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sdo_xdcruntime_linux_GateProcessSupport_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sdo_xdcruntime_linux_GateProcessSupport_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sdo_xdcruntime_linux_GateProcessSupport_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const ti_sdo_xdcruntime_linux_GateProcessSupport___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    ti_sdo_xdcruntime_linux_GateProcessSupport_Params prms;
    ti_sdo_xdcruntime_linux_GateProcessSupport_Object* obj;
    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sdo_xdcruntime_linux_GateProcessSupport_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sdo_xdcruntime_linux_GateProcessSupport_Instance_init__F(obj, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_GateProcessSupport_Object__DESC__C, obj, (xdc_Fxn)ti_sdo_xdcruntime_linux_GateProcessSupport_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void ti_sdo_xdcruntime_linux_GateProcessSupport_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_GateProcessSupport_Object__DESC__C, obj, (xdc_Fxn)ti_sdo_xdcruntime_linux_GateProcessSupport_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sdo_xdcruntime_linux_GateProcessSupport_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((ti_sdo_xdcruntime_linux_GateProcessSupport_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_GateProcessSupport_Object__DESC__C, *((ti_sdo_xdcruntime_linux_GateProcessSupport_Object**)instp), (xdc_Fxn)ti_sdo_xdcruntime_linux_GateProcessSupport_Instance_finalize__F, 0, FALSE);
    *((ti_sdo_xdcruntime_linux_GateProcessSupport_Handle*)instp) = NULL;
}


/*
 * ======== ti.sdo.xdcruntime.linux.GateThreadSupport SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_GateThreadSupport_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_GateThreadSupport_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_GateThreadSupport_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_GateThreadSupport_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_GateThreadSupport_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_GateThreadSupport_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label* ti_sdo_xdcruntime_linux_GateThreadSupport_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32804;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sdo_xdcruntime_linux_GateThreadSupport_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &ti_sdo_xdcruntime_linux_GateThreadSupport_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sdo_xdcruntime_linux_GateThreadSupport_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((ti_sdo_xdcruntime_linux_GateThreadSupport_Object__*)oa) + i;
    }

    if (ti_sdo_xdcruntime_linux_GateThreadSupport_Object__count__C == 0) {
        return NULL;
    }

    return ((ti_sdo_xdcruntime_linux_GateThreadSupport_Object__*)ti_sdo_xdcruntime_linux_GateThreadSupport_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sdo_xdcruntime_linux_GateThreadSupport_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sdo_xdcruntime_linux_GateThreadSupport_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sdo_xdcruntime_linux_GateThreadSupport_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sdo_xdcruntime_linux_GateThreadSupport_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sdo_xdcruntime_linux_GateThreadSupport_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sdo_xdcruntime_linux_GateThreadSupport_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const ti_sdo_xdcruntime_linux_GateThreadSupport___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    ti_sdo_xdcruntime_linux_GateThreadSupport_Params prms;
    ti_sdo_xdcruntime_linux_GateThreadSupport_Object* obj;
    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sdo_xdcruntime_linux_GateThreadSupport_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sdo_xdcruntime_linux_GateThreadSupport_Instance_init__F(obj, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_GateThreadSupport_Object__DESC__C, obj, (xdc_Fxn)ti_sdo_xdcruntime_linux_GateThreadSupport_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void ti_sdo_xdcruntime_linux_GateThreadSupport_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_GateThreadSupport_Object__DESC__C, obj, (xdc_Fxn)ti_sdo_xdcruntime_linux_GateThreadSupport_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sdo_xdcruntime_linux_GateThreadSupport_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((ti_sdo_xdcruntime_linux_GateThreadSupport_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_GateThreadSupport_Object__DESC__C, *((ti_sdo_xdcruntime_linux_GateThreadSupport_Object**)instp), (xdc_Fxn)ti_sdo_xdcruntime_linux_GateThreadSupport_Instance_finalize__F, 0, FALSE);
    *((ti_sdo_xdcruntime_linux_GateThreadSupport_Handle*)instp) = NULL;
}


/*
 * ======== ti.sdo.xdcruntime.linux.SemProcessSupport SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_SemProcessSupport_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_SemProcessSupport_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_SemProcessSupport_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_SemProcessSupport_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_SemProcessSupport_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_SemProcessSupport_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_SemProcessSupport_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_SemProcessSupport_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_SemProcessSupport_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_SemProcessSupport_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sdo_xdcruntime_linux_SemProcessSupport_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_SemProcessSupport_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label* ti_sdo_xdcruntime_linux_SemProcessSupport_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32807;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sdo_xdcruntime_linux_SemProcessSupport_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &ti_sdo_xdcruntime_linux_SemProcessSupport_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sdo_xdcruntime_linux_SemProcessSupport_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((ti_sdo_xdcruntime_linux_SemProcessSupport_Object__*)oa) + i;
    }

    if (ti_sdo_xdcruntime_linux_SemProcessSupport_Object__count__C == 0) {
        return NULL;
    }

    return ((ti_sdo_xdcruntime_linux_SemProcessSupport_Object__*)ti_sdo_xdcruntime_linux_SemProcessSupport_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sdo_xdcruntime_linux_SemProcessSupport_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sdo_xdcruntime_linux_SemProcessSupport_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sdo_xdcruntime_linux_SemProcessSupport_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sdo_xdcruntime_linux_SemProcessSupport_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sdo_xdcruntime_linux_SemProcessSupport_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sdo_xdcruntime_linux_SemProcessSupport_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const ti_sdo_xdcruntime_linux_SemProcessSupport___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    ti_sdo_xdcruntime_linux_SemProcessSupport_Params prms;
    ti_sdo_xdcruntime_linux_SemProcessSupport_Object* obj;
    int iStat;

    ti_sdo_xdcruntime_linux_SemProcessSupport_Args__create* args = aa;
    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sdo_xdcruntime_linux_SemProcessSupport_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sdo_xdcruntime_linux_SemProcessSupport_Instance_init__F(obj, args->count, args->key, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_SemProcessSupport_Object__DESC__C, obj, (xdc_Fxn)ti_sdo_xdcruntime_linux_SemProcessSupport_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void ti_sdo_xdcruntime_linux_SemProcessSupport_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_SemProcessSupport_Object__DESC__C, obj, (xdc_Fxn)ti_sdo_xdcruntime_linux_SemProcessSupport_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sdo_xdcruntime_linux_SemProcessSupport_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((ti_sdo_xdcruntime_linux_SemProcessSupport_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_SemProcessSupport_Object__DESC__C, *((ti_sdo_xdcruntime_linux_SemProcessSupport_Object**)instp), (xdc_Fxn)ti_sdo_xdcruntime_linux_SemProcessSupport_Instance_finalize__F, 0, FALSE);
    *((ti_sdo_xdcruntime_linux_SemProcessSupport_Handle*)instp) = NULL;
}


/*
 * ======== ti.sdo.xdcruntime.linux.SemThreadSupport SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_SemThreadSupport_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_SemThreadSupport_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_SemThreadSupport_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_SemThreadSupport_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_SemThreadSupport_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_SemThreadSupport_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_SemThreadSupport_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_SemThreadSupport_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_SemThreadSupport_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_SemThreadSupport_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sdo_xdcruntime_linux_SemThreadSupport_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_SemThreadSupport_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label* ti_sdo_xdcruntime_linux_SemThreadSupport_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32806;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sdo_xdcruntime_linux_SemThreadSupport_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &ti_sdo_xdcruntime_linux_SemThreadSupport_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sdo_xdcruntime_linux_SemThreadSupport_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((ti_sdo_xdcruntime_linux_SemThreadSupport_Object__*)oa) + i;
    }

    if (ti_sdo_xdcruntime_linux_SemThreadSupport_Object__count__C == 0) {
        return NULL;
    }

    return ((ti_sdo_xdcruntime_linux_SemThreadSupport_Object__*)ti_sdo_xdcruntime_linux_SemThreadSupport_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sdo_xdcruntime_linux_SemThreadSupport_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sdo_xdcruntime_linux_SemThreadSupport_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sdo_xdcruntime_linux_SemThreadSupport_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sdo_xdcruntime_linux_SemThreadSupport_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sdo_xdcruntime_linux_SemThreadSupport_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sdo_xdcruntime_linux_SemThreadSupport_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const ti_sdo_xdcruntime_linux_SemThreadSupport___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    ti_sdo_xdcruntime_linux_SemThreadSupport_Params prms;
    ti_sdo_xdcruntime_linux_SemThreadSupport_Object* obj;
    int iStat;

    ti_sdo_xdcruntime_linux_SemThreadSupport_Args__create* args = aa;
    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sdo_xdcruntime_linux_SemThreadSupport_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sdo_xdcruntime_linux_SemThreadSupport_Instance_init__F(obj, args->count, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_SemThreadSupport_Object__DESC__C, obj, (xdc_Fxn)ti_sdo_xdcruntime_linux_SemThreadSupport_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void ti_sdo_xdcruntime_linux_SemThreadSupport_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_SemThreadSupport_Object__DESC__C, obj, (xdc_Fxn)ti_sdo_xdcruntime_linux_SemThreadSupport_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sdo_xdcruntime_linux_SemThreadSupport_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((ti_sdo_xdcruntime_linux_SemThreadSupport_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_SemThreadSupport_Object__DESC__C, *((ti_sdo_xdcruntime_linux_SemThreadSupport_Object**)instp), (xdc_Fxn)ti_sdo_xdcruntime_linux_SemThreadSupport_Instance_finalize__F, 0, FALSE);
    *((ti_sdo_xdcruntime_linux_SemThreadSupport_Handle*)instp) = NULL;
}


/*
 * ======== ti.sdo.xdcruntime.linux.ThreadSupport SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_sdo_xdcruntime_linux_ThreadSupport_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_sdo_xdcruntime_linux_ThreadSupport_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_sdo_xdcruntime_linux_ThreadSupport_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_sdo_xdcruntime_linux_ThreadSupport_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_sdo_xdcruntime_linux_ThreadSupport_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_sdo_xdcruntime_linux_ThreadSupport_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sdo_xdcruntime_linux_ThreadSupport_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sdo_xdcruntime_linux_ThreadSupport_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sdo_xdcruntime_linux_ThreadSupport_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sdo_xdcruntime_linux_ThreadSupport_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool ti_sdo_xdcruntime_linux_ThreadSupport_Module__startupDone__S( void ) 
{
    return ti_sdo_xdcruntime_linux_ThreadSupport_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label* ti_sdo_xdcruntime_linux_ThreadSupport_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32808;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void ti_sdo_xdcruntime_linux_ThreadSupport_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &ti_sdo_xdcruntime_linux_ThreadSupport_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr ti_sdo_xdcruntime_linux_ThreadSupport_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((ti_sdo_xdcruntime_linux_ThreadSupport_Object__*)oa) + i;
    }

    if (ti_sdo_xdcruntime_linux_ThreadSupport_Object__count__C == 0) {
        return NULL;
    }

    return ((ti_sdo_xdcruntime_linux_ThreadSupport_Object__*)ti_sdo_xdcruntime_linux_ThreadSupport_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr ti_sdo_xdcruntime_linux_ThreadSupport_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)ti_sdo_xdcruntime_linux_ThreadSupport_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&ti_sdo_xdcruntime_linux_ThreadSupport_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr ti_sdo_xdcruntime_linux_ThreadSupport_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&ti_sdo_xdcruntime_linux_ThreadSupport_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr ti_sdo_xdcruntime_linux_ThreadSupport_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const ti_sdo_xdcruntime_linux_ThreadSupport___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    ti_sdo_xdcruntime_linux_ThreadSupport_Params prms;
    ti_sdo_xdcruntime_linux_ThreadSupport_Object* obj;
    int iStat;

    ti_sdo_xdcruntime_linux_ThreadSupport_Args__create* args = aa;
    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&ti_sdo_xdcruntime_linux_ThreadSupport_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = ti_sdo_xdcruntime_linux_ThreadSupport_Instance_init__F(obj, args->fxn, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_ThreadSupport_Object__DESC__C, obj, (xdc_Fxn)ti_sdo_xdcruntime_linux_ThreadSupport_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void ti_sdo_xdcruntime_linux_ThreadSupport_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_ThreadSupport_Object__DESC__C, obj, (xdc_Fxn)ti_sdo_xdcruntime_linux_ThreadSupport_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void ti_sdo_xdcruntime_linux_ThreadSupport_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((ti_sdo_xdcruntime_linux_ThreadSupport_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&ti_sdo_xdcruntime_linux_ThreadSupport_Object__DESC__C, *((ti_sdo_xdcruntime_linux_ThreadSupport_Object**)instp), (xdc_Fxn)ti_sdo_xdcruntime_linux_ThreadSupport_Instance_finalize__F, 0, FALSE);
    *((ti_sdo_xdcruntime_linux_ThreadSupport_Handle*)instp) = NULL;
}


/*
 * ======== ti.sdo.xdcruntime.linux.TimestampPosix SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool ti_sdo_xdcruntime_linux_TimestampPosix_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Assert SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Assert_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Core SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Core_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Defaults SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Defaults_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Diags SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Diags_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Error SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Error_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Gate SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Gate_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.GateNull SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_GateNull_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_GateNull_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_GateNull_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_GateNull_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_GateNull_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_GateNull_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_GateNull_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_GateNull_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_GateNull_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_GateNull_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_GateNull_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_GateNull_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_GateNull_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_GateNull_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_GateNull_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_GateNull_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_GateNull_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_GateNull_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_GateNull_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_GateNull_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32776;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_GateNull_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_GateNull_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_GateNull_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_GateNull_Object__*)oa) + i;
    }

    if (xdc_runtime_GateNull_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_GateNull_Object__*)xdc_runtime_GateNull_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_GateNull_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_GateNull_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_GateNull_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_GateNull_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_GateNull_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_GateNull_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_GateNull___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_GateNull_Params prms;
    xdc_runtime_GateNull_Object* obj;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_GateNull_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_GateNull_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_GateNull_Object__DESC__C, obj, NULL, -1, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_GateNull_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_GateNull_Object__DESC__C, *((xdc_runtime_GateNull_Object**)instp), NULL, -1, FALSE);
    *((xdc_runtime_GateNull_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.HeapStd SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_HeapStd_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_HeapStd_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_HeapStd_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_HeapStd_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_HeapStd_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_HeapStd_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_HeapStd_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_HeapStd_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_HeapStd_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_HeapStd_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_HeapStd_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_HeapStd_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_HeapStd_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_HeapStd_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_HeapStd_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_HeapStd_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_HeapStd_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_HeapStd_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_HeapStd_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_HeapStd_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32781;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_HeapStd_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_HeapStd_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_HeapStd_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_HeapStd_Object__*)oa) + i;
    }

    if (xdc_runtime_HeapStd_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_HeapStd_Object__*)xdc_runtime_HeapStd_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_HeapStd_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_HeapStd_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_HeapStd_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_HeapStd_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_HeapStd_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_HeapStd_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_HeapStd___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_HeapStd_Params prms;
    xdc_runtime_HeapStd_Object* obj;
    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_HeapStd_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = xdc_runtime_HeapStd_Instance_init__F(obj, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&xdc_runtime_HeapStd_Object__DESC__C, obj, NULL, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_HeapStd_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_HeapStd_Object__DESC__C, obj, NULL, 0, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_HeapStd_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_HeapStd_Object__DESC__C, *((xdc_runtime_HeapStd_Object**)instp), NULL, 0, FALSE);
    *((xdc_runtime_HeapStd_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.Log SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Log_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.LoggerSys SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_LoggerSys_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_LoggerSys_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_LoggerSys_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_LoggerSys_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_LoggerSys_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_LoggerSys_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_LoggerSys_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_LoggerSys_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_LoggerSys_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_LoggerSys_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_LoggerSys_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_LoggerSys_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_LoggerSys_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_LoggerSys_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_LoggerSys_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_LoggerSys_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_LoggerSys_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_LoggerSys_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_LoggerSys_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_LoggerSys_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32778;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_LoggerSys_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_LoggerSys_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_LoggerSys_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_LoggerSys_Object__*)oa) + i;
    }

    if (xdc_runtime_LoggerSys_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_LoggerSys_Object__*)xdc_runtime_LoggerSys_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_LoggerSys_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_LoggerSys_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_LoggerSys_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_LoggerSys_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_LoggerSys_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_LoggerSys_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_LoggerSys___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_LoggerSys_Params prms;
    xdc_runtime_LoggerSys_Object* obj;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_LoggerSys_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    xdc_runtime_LoggerSys_Instance_init__F(obj, &prms);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_LoggerSys_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_LoggerSys_Object__DESC__C, obj, NULL, -1, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_LoggerSys_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_LoggerSys_Object__DESC__C, *((xdc_runtime_LoggerSys_Object**)instp), NULL, -1, FALSE);
    *((xdc_runtime_LoggerSys_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.LoggerSys_TimestampProxy SYSTEM FUNCTIONS ========
 */

xdc_Bool xdc_runtime_LoggerSys_TimestampProxy_Proxy__abstract__S( void )
{
    return 0;
}
xdc_Ptr xdc_runtime_LoggerSys_TimestampProxy_Proxy__delegate__S( void )
{
    return (xdc_Ptr)&ti_sdo_xdcruntime_linux_TimestampPosix_Module__FXNS__C;
}


/*
 * ======== xdc.runtime.Main SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Main_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Main_Module_GateProxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_Main_Module_GateProxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_Main_Module_GateProxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_Main_Module_GateProxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_Main_Module_GateProxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_Main_Module_GateProxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_Main_Module_GateProxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_Main_Module_GateProxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_Main_Module_GateProxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_Main_Module_GateProxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_Main_Module_GateProxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_Main_Module_GateProxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_Main_Module_GateProxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_Main_Module_GateProxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_Main_Module_GateProxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_Main_Module_GateProxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_Main_Module_GateProxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_Main_Module_GateProxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_Main_Module_GateProxy_Module_GateProxy_query
xdc_Bool xdc_runtime_Main_Module_GateProxy_Proxy__abstract__S( void )
{
    return 0;
}
xdc_Ptr xdc_runtime_Main_Module_GateProxy_Proxy__delegate__S( void )
{
    return 0;
}



/*
 * ======== xdc.runtime.Memory SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Memory_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Memory_HeapProxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_Memory_HeapProxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_Memory_HeapProxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_Memory_HeapProxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_Memory_HeapProxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_Memory_HeapProxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_Memory_HeapProxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_Memory_HeapProxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_Memory_HeapProxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_Memory_HeapProxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_Memory_HeapProxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_Memory_HeapProxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_Memory_HeapProxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_Memory_HeapProxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_Memory_HeapProxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_Memory_HeapProxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_Memory_HeapProxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_Memory_HeapProxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_Memory_HeapProxy_Module_GateProxy_query
xdc_Bool xdc_runtime_Memory_HeapProxy_Proxy__abstract__S( void )
{
    return 1;
}
xdc_Ptr xdc_runtime_Memory_HeapProxy_Proxy__delegate__S( void )
{
    return (xdc_Ptr)&xdcruntime_heaps_HeapStdAlign_Module__FXNS__C;
}



/*
 * ======== xdc.runtime.Registry SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Registry_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Rta SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Rta_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Startup SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Startup_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.SysStd SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_SysStd_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.System SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_System_Module__startupDone__S( void ) 
{
    return xdc_runtime_System_Module__startupDone__F();
}



/*
 * ======== xdc.runtime.System_Module_GateProxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_System_Module_GateProxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_System_Module_GateProxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_System_Module_GateProxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_System_Module_GateProxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_System_Module_GateProxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_System_Module_GateProxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_System_Module_GateProxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_System_Module_GateProxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_System_Module_GateProxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_System_Module_GateProxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_System_Module_GateProxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_System_Module_GateProxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_System_Module_GateProxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_System_Module_GateProxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_System_Module_GateProxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_System_Module_GateProxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_System_Module_GateProxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_System_Module_GateProxy_Module_GateProxy_query
xdc_Bool xdc_runtime_System_Module_GateProxy_Proxy__abstract__S( void )
{
    return 0;
}
xdc_Ptr xdc_runtime_System_Module_GateProxy_Proxy__delegate__S( void )
{
    return 0;
}



/*
 * ======== xdc.runtime.System_SupportProxy SYSTEM FUNCTIONS ========
 */

xdc_Bool xdc_runtime_System_SupportProxy_Proxy__abstract__S( void )
{
    return 0;
}
xdc_Ptr xdc_runtime_System_SupportProxy_Proxy__delegate__S( void )
{
    return (xdc_Ptr)&xdc_runtime_SysStd_Module__FXNS__C;
}


/*
 * ======== xdc.runtime.Text SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Text_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Timestamp SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_Timestamp_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.Timestamp_SupportProxy SYSTEM FUNCTIONS ========
 */

xdc_Bool xdc_runtime_Timestamp_SupportProxy_Proxy__abstract__S( void )
{
    return 0;
}
xdc_Ptr xdc_runtime_Timestamp_SupportProxy_Proxy__delegate__S( void )
{
    return (xdc_Ptr)&ti_sdo_xdcruntime_linux_TimestampPosix_Module__FXNS__C;
}


/*
 * ======== xdc.runtime.knl.GateH SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_GateH_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.knl.GateH_Proxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_GateH_Proxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_GateH_Proxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_GateH_Proxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_GateH_Proxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_GateH_Proxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_GateH_Proxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_GateH_Proxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_GateH_Proxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_GateH_Proxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_GateH_Proxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_GateH_Proxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_GateH_Proxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_GateH_Proxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_GateH_Proxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_GateH_Proxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_GateH_Proxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_GateH_Proxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_GateH_Proxy_Module_GateProxy_query
xdc_Bool xdc_runtime_knl_GateH_Proxy_Proxy__abstract__S( void )
{
    return 1;
}
xdc_Ptr xdc_runtime_knl_GateH_Proxy_Proxy__delegate__S( void )
{
    return (xdc_Ptr)&xdc_runtime_knl_GateThread_Module__FXNS__C;
}



/*
 * ======== xdc.runtime.knl.GateProcess SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_GateProcess_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_GateProcess_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_GateProcess_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_GateProcess_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_GateProcess_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_GateProcess_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_GateProcess_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_GateProcess_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_GateProcess_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_GateProcess_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_GateProcess_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_GateProcess_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_GateProcess_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_GateProcess_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_GateProcess_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_GateProcess_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_GateProcess_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_GateProcess_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_GateProcess_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_GateProcess_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32795;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_knl_GateProcess_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_knl_GateProcess_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_knl_GateProcess_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_knl_GateProcess_Object__*)oa) + i;
    }

    if (xdc_runtime_knl_GateProcess_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_knl_GateProcess_Object__*)xdc_runtime_knl_GateProcess_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_knl_GateProcess_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_knl_GateProcess_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_knl_GateProcess_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_knl_GateProcess_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_knl_GateProcess_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_GateProcess_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_GateProcess___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_knl_GateProcess_Params prms;
    xdc_runtime_knl_GateProcess_Object* obj;
    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_knl_GateProcess_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = xdc_runtime_knl_GateProcess_Instance_init__F(obj, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_GateProcess_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_GateProcess_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_knl_GateProcess_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_GateProcess_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_GateProcess_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_knl_GateProcess_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((xdc_runtime_knl_GateProcess_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_GateProcess_Object__DESC__C, *((xdc_runtime_knl_GateProcess_Object**)instp), (xdc_Fxn)xdc_runtime_knl_GateProcess_Instance_finalize__F, 0, FALSE);
    *((xdc_runtime_knl_GateProcess_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.knl.GateProcess_Proxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_GateProcess_Proxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_GateProcess_Proxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_GateProcess_Proxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_GateProcess_Proxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_GateProcess_Proxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_GateProcess_Proxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_GateProcess_Proxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_GateProcess_Proxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_GateProcess_Proxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_GateProcess_Proxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_GateProcess_Proxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_GateProcess_Proxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_GateProcess_Proxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_GateProcess_Proxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_GateProcess_Proxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_GateProcess_Proxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_GateProcess_Proxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_GateProcess_Proxy_Module_GateProxy_query
xdc_Bool xdc_runtime_knl_GateProcess_Proxy_Proxy__abstract__S( void )
{
    return 0;
}
xdc_Ptr xdc_runtime_knl_GateProcess_Proxy_Proxy__delegate__S( void )
{
    return 0;
}



/*
 * ======== xdc.runtime.knl.GateThread SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_GateThread_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_GateThread_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_GateThread_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_GateThread_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_GateThread_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_GateThread_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_GateThread_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_GateThread_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_GateThread_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_GateThread_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_GateThread_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_GateThread_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_GateThread_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_GateThread_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_GateThread_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_GateThread_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_GateThread_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_GateThread_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_GateThread_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_GateThread_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32794;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_knl_GateThread_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_knl_GateThread_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_knl_GateThread_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_knl_GateThread_Object__*)oa) + i;
    }

    if (xdc_runtime_knl_GateThread_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_knl_GateThread_Object__*)xdc_runtime_knl_GateThread_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_knl_GateThread_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_knl_GateThread_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_knl_GateThread_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_knl_GateThread_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_knl_GateThread_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_GateThread_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_GateThread___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_knl_GateThread_Params prms;
    xdc_runtime_knl_GateThread_Object* obj;
    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_knl_GateThread_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = xdc_runtime_knl_GateThread_Instance_init__F(obj, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_GateThread_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_GateThread_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_knl_GateThread_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_GateThread_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_GateThread_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_knl_GateThread_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((xdc_runtime_knl_GateThread_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_GateThread_Object__DESC__C, *((xdc_runtime_knl_GateThread_Object**)instp), (xdc_Fxn)xdc_runtime_knl_GateThread_Instance_finalize__F, 0, FALSE);
    *((xdc_runtime_knl_GateThread_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.knl.GateThread_Proxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_GateThread_Proxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_GateThread_Proxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_GateThread_Proxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_GateThread_Proxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_GateThread_Proxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_GateThread_Proxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_GateThread_Proxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_GateThread_Proxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_GateThread_Proxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_GateThread_Proxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_GateThread_Proxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_GateThread_Proxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_GateThread_Proxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_GateThread_Proxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_GateThread_Proxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_GateThread_Proxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_GateThread_Proxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_GateThread_Proxy_Module_GateProxy_query
xdc_Bool xdc_runtime_knl_GateThread_Proxy_Proxy__abstract__S( void )
{
    return 0;
}
xdc_Ptr xdc_runtime_knl_GateThread_Proxy_Proxy__delegate__S( void )
{
    return 0;
}



/*
 * ======== xdc.runtime.knl.SemProcess SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SemProcess_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SemProcess_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SemProcess_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SemProcess_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SemProcess_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SemProcess_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SemProcess_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SemProcess_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SemProcess_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SemProcess_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SemProcess_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SemProcess_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SemProcess_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SemProcess_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SemProcess_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SemProcess_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SemProcess_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SemProcess_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_SemProcess_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_SemProcess_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32797;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_knl_SemProcess_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_knl_SemProcess_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_knl_SemProcess_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_knl_SemProcess_Object__*)oa) + i;
    }

    if (xdc_runtime_knl_SemProcess_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_knl_SemProcess_Object__*)xdc_runtime_knl_SemProcess_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_knl_SemProcess_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_knl_SemProcess_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_knl_SemProcess_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_knl_SemProcess_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_knl_SemProcess_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_SemProcess_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_SemProcess___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_knl_SemProcess_Params prms;
    xdc_runtime_knl_SemProcess_Object* obj;
    int iStat;

    xdc_runtime_knl_SemProcess_Args__create* args = aa;
    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_knl_SemProcess_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = xdc_runtime_knl_SemProcess_Instance_init__F(obj, args->count, args->key, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SemProcess_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_SemProcess_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_knl_SemProcess_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SemProcess_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_SemProcess_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_knl_SemProcess_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((xdc_runtime_knl_SemProcess_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SemProcess_Object__DESC__C, *((xdc_runtime_knl_SemProcess_Object**)instp), (xdc_Fxn)xdc_runtime_knl_SemProcess_Instance_finalize__F, 0, FALSE);
    *((xdc_runtime_knl_SemProcess_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.knl.SemProcess_Proxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SemProcess_Proxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SemProcess_Proxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SemProcess_Proxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SemProcess_Proxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SemProcess_Proxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SemProcess_Proxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SemProcess_Proxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SemProcess_Proxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SemProcess_Proxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SemProcess_Proxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SemProcess_Proxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SemProcess_Proxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SemProcess_Proxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SemProcess_Proxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SemProcess_Proxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SemProcess_Proxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SemProcess_Proxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SemProcess_Proxy_Module_GateProxy_query
xdc_Bool xdc_runtime_knl_SemProcess_Proxy_Proxy__abstract__S( void )
{
    return 0;
}
xdc_Ptr xdc_runtime_knl_SemProcess_Proxy_Proxy__delegate__S( void )
{
    return 0;
}



/*
 * ======== xdc.runtime.knl.SemThread SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SemThread_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SemThread_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SemThread_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SemThread_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SemThread_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SemThread_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SemThread_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SemThread_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SemThread_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SemThread_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SemThread_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SemThread_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SemThread_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SemThread_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SemThread_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SemThread_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SemThread_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SemThread_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_SemThread_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_SemThread_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32796;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_knl_SemThread_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_knl_SemThread_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_knl_SemThread_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_knl_SemThread_Object__*)oa) + i;
    }

    if (xdc_runtime_knl_SemThread_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_knl_SemThread_Object__*)xdc_runtime_knl_SemThread_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_knl_SemThread_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_knl_SemThread_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_knl_SemThread_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_knl_SemThread_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_knl_SemThread_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_SemThread_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_SemThread___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_knl_SemThread_Params prms;
    xdc_runtime_knl_SemThread_Object* obj;
    int iStat;

    xdc_runtime_knl_SemThread_Args__create* args = aa;
    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_knl_SemThread_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = xdc_runtime_knl_SemThread_Instance_init__F(obj, args->count, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SemThread_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_SemThread_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_knl_SemThread_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SemThread_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_SemThread_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_knl_SemThread_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((xdc_runtime_knl_SemThread_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SemThread_Object__DESC__C, *((xdc_runtime_knl_SemThread_Object**)instp), (xdc_Fxn)xdc_runtime_knl_SemThread_Instance_finalize__F, 0, FALSE);
    *((xdc_runtime_knl_SemThread_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.knl.SemThread_Proxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SemThread_Proxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SemThread_Proxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SemThread_Proxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SemThread_Proxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SemThread_Proxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SemThread_Proxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SemThread_Proxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SemThread_Proxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SemThread_Proxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SemThread_Proxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SemThread_Proxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SemThread_Proxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SemThread_Proxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SemThread_Proxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SemThread_Proxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SemThread_Proxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SemThread_Proxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SemThread_Proxy_Module_GateProxy_query
xdc_Bool xdc_runtime_knl_SemThread_Proxy_Proxy__abstract__S( void )
{
    return 0;
}
xdc_Ptr xdc_runtime_knl_SemThread_Proxy_Proxy__delegate__S( void )
{
    return 0;
}



/*
 * ======== xdc.runtime.knl.Semaphore SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_Semaphore_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.knl.Semaphore_Proxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_Semaphore_Proxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_Semaphore_Proxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_Semaphore_Proxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_Semaphore_Proxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_Semaphore_Proxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_Semaphore_Proxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_Semaphore_Proxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_Semaphore_Proxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_Semaphore_Proxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_Semaphore_Proxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_Semaphore_Proxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_Semaphore_Proxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_Semaphore_Proxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_Semaphore_Proxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_Semaphore_Proxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_Semaphore_Proxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_Semaphore_Proxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_Semaphore_Proxy_Module_GateProxy_query
xdc_Bool xdc_runtime_knl_Semaphore_Proxy_Proxy__abstract__S( void )
{
    return 1;
}
xdc_Ptr xdc_runtime_knl_Semaphore_Proxy_Proxy__delegate__S( void )
{
    return (xdc_Ptr)&xdc_runtime_knl_SemThread_Module__FXNS__C;
}



/*
 * ======== xdc.runtime.knl.Sync SYSTEM FUNCTIONS ========
 */

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_Sync_Module__startupDone__S( void ) 
{
    return 1;
}



/*
 * ======== xdc.runtime.knl.SyncGeneric SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SyncGeneric_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SyncGeneric_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SyncGeneric_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SyncGeneric_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SyncGeneric_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SyncGeneric_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SyncGeneric_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SyncGeneric_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SyncGeneric_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SyncGeneric_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SyncGeneric_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SyncGeneric_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SyncGeneric_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SyncGeneric_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SyncGeneric_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SyncGeneric_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SyncGeneric_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SyncGeneric_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_SyncGeneric_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_SyncGeneric_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32798;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_knl_SyncGeneric_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_knl_SyncGeneric_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_knl_SyncGeneric_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_knl_SyncGeneric_Object__*)oa) + i;
    }

    if (xdc_runtime_knl_SyncGeneric_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_knl_SyncGeneric_Object__*)xdc_runtime_knl_SyncGeneric_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_knl_SyncGeneric_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_knl_SyncGeneric_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_knl_SyncGeneric_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_knl_SyncGeneric_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_knl_SyncGeneric_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_SyncGeneric_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_SyncGeneric___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_knl_SyncGeneric_Params prms;
    xdc_runtime_knl_SyncGeneric_Object* obj;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_knl_SyncGeneric_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    xdc_runtime_knl_SyncGeneric_Instance_init__F(obj, &prms);
    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_knl_SyncGeneric_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SyncGeneric_Object__DESC__C, obj, NULL, -1, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_knl_SyncGeneric_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((xdc_runtime_knl_SyncGeneric_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SyncGeneric_Object__DESC__C, *((xdc_runtime_knl_SyncGeneric_Object**)instp), NULL, -1, FALSE);
    *((xdc_runtime_knl_SyncGeneric_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.knl.SyncNull SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SyncNull_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SyncNull_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SyncNull_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SyncNull_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SyncNull_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SyncNull_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SyncNull_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SyncNull_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SyncNull_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SyncNull_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SyncNull_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SyncNull_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SyncNull_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SyncNull_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SyncNull_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SyncNull_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SyncNull_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SyncNull_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_SyncNull_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_SyncNull_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32799;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_knl_SyncNull_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_knl_SyncNull_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_knl_SyncNull_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_knl_SyncNull_Object__*)oa) + i;
    }

    if (xdc_runtime_knl_SyncNull_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_knl_SyncNull_Object__*)xdc_runtime_knl_SyncNull_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_knl_SyncNull_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_knl_SyncNull_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_knl_SyncNull_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_knl_SyncNull_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_knl_SyncNull_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_SyncNull_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_SyncNull___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_knl_SyncNull_Params prms;
    xdc_runtime_knl_SyncNull_Object* obj;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_knl_SyncNull_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_knl_SyncNull_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SyncNull_Object__DESC__C, obj, NULL, -1, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_knl_SyncNull_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((xdc_runtime_knl_SyncNull_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SyncNull_Object__DESC__C, *((xdc_runtime_knl_SyncNull_Object**)instp), NULL, -1, FALSE);
    *((xdc_runtime_knl_SyncNull_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.knl.SyncSemThread SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_SyncSemThread_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_SyncSemThread_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_SyncSemThread_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_SyncSemThread_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_SyncSemThread_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_SyncSemThread_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_SyncSemThread_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_SyncSemThread_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_SyncSemThread_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_SyncSemThread_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_SyncSemThread_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_SyncSemThread_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_SyncSemThread_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_SyncSemThread_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_SyncSemThread_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_SyncSemThread_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_SyncSemThread_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_SyncSemThread_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_SyncSemThread_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_SyncSemThread_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32800;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_knl_SyncSemThread_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_knl_SyncSemThread_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_knl_SyncSemThread_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_knl_SyncSemThread_Object__*)oa) + i;
    }

    if (xdc_runtime_knl_SyncSemThread_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_knl_SyncSemThread_Object__*)xdc_runtime_knl_SyncSemThread_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_knl_SyncSemThread_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_knl_SyncSemThread_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_knl_SyncSemThread_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_knl_SyncSemThread_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_knl_SyncSemThread_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_SyncSemThread_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_SyncSemThread___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_knl_SyncSemThread_Params prms;
    xdc_runtime_knl_SyncSemThread_Object* obj;
    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_knl_SyncSemThread_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = xdc_runtime_knl_SyncSemThread_Instance_init__F(obj, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SyncSemThread_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_SyncSemThread_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_knl_SyncSemThread_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SyncSemThread_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_SyncSemThread_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_knl_SyncSemThread_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((xdc_runtime_knl_SyncSemThread_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_SyncSemThread_Object__DESC__C, *((xdc_runtime_knl_SyncSemThread_Object**)instp), (xdc_Fxn)xdc_runtime_knl_SyncSemThread_Instance_finalize__F, 0, FALSE);
    *((xdc_runtime_knl_SyncSemThread_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.knl.Sync_Proxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_Sync_Proxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_Sync_Proxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_Sync_Proxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_Sync_Proxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_Sync_Proxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_Sync_Proxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_Sync_Proxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_Sync_Proxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_Sync_Proxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_Sync_Proxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_Sync_Proxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_Sync_Proxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_Sync_Proxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_Sync_Proxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_Sync_Proxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_Sync_Proxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_Sync_Proxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_Sync_Proxy_Module_GateProxy_query
xdc_Bool xdc_runtime_knl_Sync_Proxy_Proxy__abstract__S( void )
{
    return 1;
}
xdc_Ptr xdc_runtime_knl_Sync_Proxy_Proxy__delegate__S( void )
{
    return (xdc_Ptr)&xdc_runtime_knl_SyncNull_Module__FXNS__C;
}



/*
 * ======== xdc.runtime.knl.Thread SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_Thread_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_Thread_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_Thread_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_Thread_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_Thread_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_Thread_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_Thread_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_Thread_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_Thread_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_Thread_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_Thread_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_Thread_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_Thread_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_Thread_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_Thread_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_Thread_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_Thread_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_Thread_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdc_runtime_knl_Thread_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdc_runtime_knl_Thread_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32792;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdc_runtime_knl_Thread_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdc_runtime_knl_Thread_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdc_runtime_knl_Thread_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdc_runtime_knl_Thread_Object__*)oa) + i;
    }

    if (xdc_runtime_knl_Thread_Object__count__C == 0) {
        return NULL;
    }

    return ((xdc_runtime_knl_Thread_Object__*)xdc_runtime_knl_Thread_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdc_runtime_knl_Thread_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdc_runtime_knl_Thread_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdc_runtime_knl_Thread_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdc_runtime_knl_Thread_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdc_runtime_knl_Thread_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdc_runtime_knl_Thread_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdc_runtime_knl_Thread___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdc_runtime_knl_Thread_Params prms;
    xdc_runtime_knl_Thread_Object* obj;
    int iStat;

    xdc_runtime_knl_Thread_Args__create* args = aa;
    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdc_runtime_knl_Thread_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = xdc_runtime_knl_Thread_Instance_init__F(obj, args->fxn, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_Thread_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_Thread_Instance_finalize__F, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdc_runtime_knl_Thread_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_Thread_Object__DESC__C, obj, (xdc_Fxn)xdc_runtime_knl_Thread_Instance_finalize__F, 0, TRUE);
}

/* Object__delete__S */
xdc_Void xdc_runtime_knl_Thread_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((xdc_runtime_knl_Thread_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&xdc_runtime_knl_Thread_Object__DESC__C, *((xdc_runtime_knl_Thread_Object**)instp), (xdc_Fxn)xdc_runtime_knl_Thread_Instance_finalize__F, 0, FALSE);
    *((xdc_runtime_knl_Thread_Handle*)instp) = NULL;
}


/*
 * ======== xdc.runtime.knl.Thread_Proxy SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdc_runtime_knl_Thread_Proxy_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdc_runtime_knl_Thread_Proxy_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdc_runtime_knl_Thread_Proxy_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdc_runtime_knl_Thread_Proxy_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdc_runtime_knl_Thread_Proxy_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdc_runtime_knl_Thread_Proxy_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdc_runtime_knl_Thread_Proxy_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdc_runtime_knl_Thread_Proxy_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdc_runtime_knl_Thread_Proxy_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdc_runtime_knl_Thread_Proxy_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdc_runtime_knl_Thread_Proxy_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdc_runtime_knl_Thread_Proxy_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdc_runtime_knl_Thread_Proxy_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdc_runtime_knl_Thread_Proxy_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdc_runtime_knl_Thread_Proxy_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdc_runtime_knl_Thread_Proxy_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdc_runtime_knl_Thread_Proxy_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdc_runtime_knl_Thread_Proxy_Module_GateProxy_query
xdc_Bool xdc_runtime_knl_Thread_Proxy_Proxy__abstract__S( void )
{
    return 0;
}
xdc_Ptr xdc_runtime_knl_Thread_Proxy_Proxy__delegate__S( void )
{
    return 0;
}



/*
 * ======== xdcruntime.cstubs.LoggerFlex SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdcruntime_cstubs_LoggerFlex_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdcruntime_cstubs_LoggerFlex_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdcruntime_cstubs_LoggerFlex_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdcruntime_cstubs_LoggerFlex_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdcruntime_cstubs_LoggerFlex_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdcruntime_cstubs_LoggerFlex_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdcruntime_cstubs_LoggerFlex_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdcruntime_cstubs_LoggerFlex_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdcruntime_cstubs_LoggerFlex_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdcruntime_cstubs_LoggerFlex_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdcruntime_cstubs_LoggerFlex_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdcruntime_cstubs_LoggerFlex_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdcruntime_cstubs_LoggerFlex_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdcruntime_cstubs_LoggerFlex_Module__startupDone__S( void ) 
{
    return xdcruntime_cstubs_LoggerFlex_Module__startupDone__F();
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdcruntime_cstubs_LoggerFlex_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32813;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdcruntime_cstubs_LoggerFlex_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdcruntime_cstubs_LoggerFlex_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdcruntime_cstubs_LoggerFlex_Object__*)oa) + i;
    }

    if (xdcruntime_cstubs_LoggerFlex_Object__count__C == 0) {
        return NULL;
    }

    return ((xdcruntime_cstubs_LoggerFlex_Object__*)xdcruntime_cstubs_LoggerFlex_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdcruntime_cstubs_LoggerFlex_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdcruntime_cstubs_LoggerFlex_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdcruntime_cstubs_LoggerFlex_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdcruntime_cstubs_LoggerFlex___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdcruntime_cstubs_LoggerFlex_Params prms;
    xdcruntime_cstubs_LoggerFlex_Object* obj;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdcruntime_cstubs_LoggerFlex_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    xdcruntime_cstubs_LoggerFlex_Instance_init__F(obj, &prms);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdcruntime_cstubs_LoggerFlex_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Core_deleteObject__I(&xdcruntime_cstubs_LoggerFlex_Object__DESC__C, obj, NULL, -1, TRUE);
}

/* Object__delete__S */
xdc_Void xdcruntime_cstubs_LoggerFlex_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Core_deleteObject__I(&xdcruntime_cstubs_LoggerFlex_Object__DESC__C, *((xdcruntime_cstubs_LoggerFlex_Object**)instp), NULL, -1, FALSE);
    *((xdcruntime_cstubs_LoggerFlex_Handle*)instp) = NULL;
}


/*
 * ======== xdcruntime.heaps.HeapStdAlign SYSTEM FUNCTIONS ========
 */

/* per-module runtime symbols */
#undef Module__MID
#define Module__MID xdcruntime_heaps_HeapStdAlign_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL xdcruntime_heaps_HeapStdAlign_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB xdcruntime_heaps_HeapStdAlign_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK xdcruntime_heaps_HeapStdAlign_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF xdcruntime_heaps_HeapStdAlign_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ xdcruntime_heaps_HeapStdAlign_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ xdcruntime_heaps_HeapStdAlign_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS xdcruntime_heaps_HeapStdAlign_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create xdcruntime_heaps_HeapStdAlign_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete xdcruntime_heaps_HeapStdAlign_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter xdcruntime_heaps_HeapStdAlign_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave xdcruntime_heaps_HeapStdAlign_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query xdcruntime_heaps_HeapStdAlign_Module_GateProxy_query

/* Module__startupDone__S */
xdc_Bool xdcruntime_heaps_HeapStdAlign_Module__startupDone__S( void ) 
{
    return 1;
}

/* Handle__label__S */
xdc_runtime_Types_Label* xdcruntime_heaps_HeapStdAlign_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab ) 
{
    lab->handle = obj;
    lab->modId = 32811;
    xdc_runtime_Core_assignLabel(lab, 0, 0);

    return lab;
}

/* Params__init__S */
xdc_Void xdcruntime_heaps_HeapStdAlign_Params__init__S( xdc_Ptr prms, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz ) 
{
    xdc_runtime_Core_assignParams__I(prms, (xdc_Ptr)(src ? src : &xdcruntime_heaps_HeapStdAlign_Object__PARAMS__C), psz, isz);
}

/* Object__get__S */
xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__get__S( xdc_Ptr oa, xdc_Int i ) 
{
    if (oa) {
        return ((xdcruntime_heaps_HeapStdAlign_Object__*)oa) + i;
    }

    if (xdcruntime_heaps_HeapStdAlign_Object__count__C == 0) {
        return NULL;
    }

    return ((xdcruntime_heaps_HeapStdAlign_Object__*)xdcruntime_heaps_HeapStdAlign_Object__table__C) + i;
}

/* Object__first__S */
xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__first__S( void ) 
{
    xdc_runtime_Types_InstHdr *iHdr = (xdc_runtime_Types_InstHdr *)xdcruntime_heaps_HeapStdAlign_Module__root__V.link.next;

    if (iHdr != (xdc_runtime_Types_InstHdr *)&xdcruntime_heaps_HeapStdAlign_Module__root__V.link) {
        return iHdr + 1;
    }
    else {
        return NULL;
    }
}

/* Object__next__S */
xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__next__S( xdc_Ptr obj ) 
{
    xdc_runtime_Types_InstHdr *iHdr = ((xdc_runtime_Types_InstHdr *)obj) - 1;

    if (iHdr->link.next != (xdc_runtime_Types_Link *)&xdcruntime_heaps_HeapStdAlign_Module__root__V.link) {
        return (xdc_runtime_Types_InstHdr *)(iHdr->link.next) + 1;
    }
    else {
        return NULL;
    }
}

/* Object__create__S */
xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__create__S (
    xdc_Ptr oa,
    xdc_SizeT osz,
    const xdc_Ptr aa,
    const xdcruntime_heaps_HeapStdAlign___ParamsPtr pa,
    xdc_SizeT psz,
    xdc_runtime_Error_Block* eb )
{
    xdcruntime_heaps_HeapStdAlign_Params prms;
    xdcruntime_heaps_HeapStdAlign_Object* obj;
    int iStat;

    /* common instance initialization */
    obj = xdc_runtime_Core_createObject__I(&xdcruntime_heaps_HeapStdAlign_Object__DESC__C, oa, osz, &prms, (xdc_Ptr)pa, psz, eb);
    if (obj == NULL) {
        return NULL;
    }

    /* module-specific initialization */
    iStat = xdcruntime_heaps_HeapStdAlign_Instance_init__F(obj, &prms, eb);
    if (xdc_runtime_Error_check(eb)) {
        xdc_runtime_Core_deleteObject__I(&xdcruntime_heaps_HeapStdAlign_Object__DESC__C, obj, NULL, iStat, (xdc_Bool)(oa != NULL));
        return NULL;
    }

    xdc_runtime_Log_write2(oa ? xdc_runtime_Log_L_construct : xdc_runtime_Log_L_create, (xdc_IArg)obj, 0);
    return obj;
}

/* Object__destruct__S */
xdc_Void xdcruntime_heaps_HeapStdAlign_Object__destruct__S( xdc_Ptr obj ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_destruct, (xdc_IArg)obj);
    xdc_runtime_Core_deleteObject__I(&xdcruntime_heaps_HeapStdAlign_Object__DESC__C, obj, NULL, 0, TRUE);
}

/* Object__delete__S */
xdc_Void xdcruntime_heaps_HeapStdAlign_Object__delete__S( xdc_Ptr instp ) 
{
    xdc_runtime_Log_write1(xdc_runtime_Log_L_delete, (xdc_IArg)(*((xdcruntime_heaps_HeapStdAlign_Object**)instp)));
    xdc_runtime_Core_deleteObject__I(&xdcruntime_heaps_HeapStdAlign_Object__DESC__C, *((xdcruntime_heaps_HeapStdAlign_Object**)instp), NULL, 0, FALSE);
    *((xdcruntime_heaps_HeapStdAlign_Handle*)instp) = NULL;
}


/*
 * ======== ti.sdo.xdcruntime.linux.GateThreadSupport PRAGMAS ========
 */

const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsEnabled ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsIncluded ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsMask ti_sdo_xdcruntime_linux_GateThreadSupport_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gateObj ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gateObj__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gatePrms ti_sdo_xdcruntime_linux_GateThreadSupport_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__id ti_sdo_xdcruntime_linux_GateThreadSupport_Module__id__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerDefined ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerObj ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn0 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn1 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn2 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn4 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn8 ti_sdo_xdcruntime_linux_GateThreadSupport_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDoneFxn ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Object__count ti_sdo_xdcruntime_linux_GateThreadSupport_Object__count__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Object__heap ti_sdo_xdcruntime_linux_GateThreadSupport_Object__heap__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Object__sizeof ti_sdo_xdcruntime_linux_GateThreadSupport_Object__sizeof__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_Object__table ti_sdo_xdcruntime_linux_GateThreadSupport_Object__table__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_A_POSIX_Error ti_sdo_xdcruntime_linux_GateThreadSupport_A_POSIX_Error__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_GateThreadSupport_E_POSIX_Error ti_sdo_xdcruntime_linux_GateThreadSupport_E_POSIX_Error__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* ti_sdo_xdcruntime_linux_GateThreadSupport_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr ti_sdo_xdcruntime_linux_GateThreadSupport_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void ti_sdo_xdcruntime_linux_GateThreadSupport_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void ti_sdo_xdcruntime_linux_GateThreadSupport_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr ti_sdo_xdcruntime_linux_GateThreadSupport_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr ti_sdo_xdcruntime_linux_GateThreadSupport_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr ti_sdo_xdcruntime_linux_GateThreadSupport_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void ti_sdo_xdcruntime_linux_GateThreadSupport_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Bool ti_sdo_xdcruntime_linux_GateThreadSupport_query__E(xdc_Int qual) __attribute__ ((externally_visible));
xdc_IArg ti_sdo_xdcruntime_linux_GateThreadSupport_enter__E(ti_sdo_xdcruntime_linux_GateThreadSupport_Handle) __attribute__ ((externally_visible));
xdc_Void ti_sdo_xdcruntime_linux_GateThreadSupport_leave__E(ti_sdo_xdcruntime_linux_GateThreadSupport_Handle, xdc_IArg key) __attribute__ ((externally_visible));

/*
 * ======== ti.sdo.xdcruntime.linux.TimestampPosix PRAGMAS ========
 */

const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsEnabled ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsIncluded ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsMask ti_sdo_xdcruntime_linux_TimestampPosix_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__gateObj ti_sdo_xdcruntime_linux_TimestampPosix_Module__gateObj__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__gatePrms ti_sdo_xdcruntime_linux_TimestampPosix_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__id ti_sdo_xdcruntime_linux_TimestampPosix_Module__id__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerDefined ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerObj ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn0 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn1 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn2 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn4 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn8 ti_sdo_xdcruntime_linux_TimestampPosix_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Module__startupDoneFxn ti_sdo_xdcruntime_linux_TimestampPosix_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Object__count ti_sdo_xdcruntime_linux_TimestampPosix_Object__count__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Object__heap ti_sdo_xdcruntime_linux_TimestampPosix_Object__heap__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Object__sizeof ti_sdo_xdcruntime_linux_TimestampPosix_Object__sizeof__C __attribute__ ((externally_visible));
const CT__ti_sdo_xdcruntime_linux_TimestampPosix_Object__table ti_sdo_xdcruntime_linux_TimestampPosix_Object__table__C __attribute__ ((externally_visible));
xdc_Bool ti_sdo_xdcruntime_linux_TimestampPosix_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Bits32 ti_sdo_xdcruntime_linux_TimestampPosix_get32__E(void) __attribute__ ((externally_visible));
xdc_Void ti_sdo_xdcruntime_linux_TimestampPosix_get64__E(xdc_runtime_Types_Timestamp64* result) __attribute__ ((externally_visible));
xdc_Void ti_sdo_xdcruntime_linux_TimestampPosix_getFreq__E(xdc_runtime_Types_FreqHz* freq) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Assert PRAGMAS ========
 */

const CT__xdc_runtime_Assert_Module__diagsEnabled xdc_runtime_Assert_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__diagsIncluded xdc_runtime_Assert_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__diagsMask xdc_runtime_Assert_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__gateObj xdc_runtime_Assert_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__gatePrms xdc_runtime_Assert_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__id xdc_runtime_Assert_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__loggerDefined xdc_runtime_Assert_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__loggerObj xdc_runtime_Assert_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__loggerFxn0 xdc_runtime_Assert_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__loggerFxn1 xdc_runtime_Assert_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__loggerFxn2 xdc_runtime_Assert_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__loggerFxn4 xdc_runtime_Assert_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__loggerFxn8 xdc_runtime_Assert_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Module__startupDoneFxn xdc_runtime_Assert_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Object__count xdc_runtime_Assert_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Object__heap xdc_runtime_Assert_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Object__sizeof xdc_runtime_Assert_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_Object__table xdc_runtime_Assert_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Assert_E_assertFailed xdc_runtime_Assert_E_assertFailed__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Assert_Module__startupDone__S(void) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Defaults PRAGMAS ========
 */

const CT__xdc_runtime_Defaults_Module__diagsEnabled xdc_runtime_Defaults_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__diagsIncluded xdc_runtime_Defaults_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__diagsMask xdc_runtime_Defaults_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__gateObj xdc_runtime_Defaults_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__gatePrms xdc_runtime_Defaults_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__id xdc_runtime_Defaults_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__loggerDefined xdc_runtime_Defaults_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__loggerObj xdc_runtime_Defaults_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__loggerFxn0 xdc_runtime_Defaults_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__loggerFxn1 xdc_runtime_Defaults_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__loggerFxn2 xdc_runtime_Defaults_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__loggerFxn4 xdc_runtime_Defaults_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__loggerFxn8 xdc_runtime_Defaults_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Module__startupDoneFxn xdc_runtime_Defaults_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Object__count xdc_runtime_Defaults_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Object__heap xdc_runtime_Defaults_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Object__sizeof xdc_runtime_Defaults_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Defaults_Object__table xdc_runtime_Defaults_Object__table__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Defaults_Module__startupDone__S(void) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Diags PRAGMAS ========
 */

const CT__xdc_runtime_Diags_Module__diagsEnabled xdc_runtime_Diags_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__diagsIncluded xdc_runtime_Diags_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__diagsMask xdc_runtime_Diags_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__gateObj xdc_runtime_Diags_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__gatePrms xdc_runtime_Diags_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__id xdc_runtime_Diags_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__loggerDefined xdc_runtime_Diags_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__loggerObj xdc_runtime_Diags_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__loggerFxn0 xdc_runtime_Diags_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__loggerFxn1 xdc_runtime_Diags_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__loggerFxn2 xdc_runtime_Diags_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__loggerFxn4 xdc_runtime_Diags_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__loggerFxn8 xdc_runtime_Diags_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Module__startupDoneFxn xdc_runtime_Diags_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Object__count xdc_runtime_Diags_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Object__heap xdc_runtime_Diags_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Object__sizeof xdc_runtime_Diags_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_Object__table xdc_runtime_Diags_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_setMaskEnabled xdc_runtime_Diags_setMaskEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Diags_dictBase xdc_runtime_Diags_dictBase__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Diags_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Diags_setMask__E(xdc_String control) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Error PRAGMAS ========
 */

const CT__xdc_runtime_Error_Module__diagsEnabled xdc_runtime_Error_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__diagsIncluded xdc_runtime_Error_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__diagsMask xdc_runtime_Error_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__gateObj xdc_runtime_Error_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__gatePrms xdc_runtime_Error_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__id xdc_runtime_Error_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__loggerDefined xdc_runtime_Error_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__loggerObj xdc_runtime_Error_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__loggerFxn0 xdc_runtime_Error_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__loggerFxn1 xdc_runtime_Error_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__loggerFxn2 xdc_runtime_Error_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__loggerFxn4 xdc_runtime_Error_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__loggerFxn8 xdc_runtime_Error_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Module__startupDoneFxn xdc_runtime_Error_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Object__count xdc_runtime_Error_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Object__heap xdc_runtime_Error_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Object__sizeof xdc_runtime_Error_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_Object__table xdc_runtime_Error_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_E_generic xdc_runtime_Error_E_generic__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_E_memory xdc_runtime_Error_E_memory__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_E_msgCode xdc_runtime_Error_E_msgCode__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_policy xdc_runtime_Error_policy__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_raiseHook xdc_runtime_Error_raiseHook__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Error_maxDepth xdc_runtime_Error_maxDepth__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Error_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Error_check__E(xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_runtime_Error_Data* xdc_runtime_Error_getData__E(xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_UInt16 xdc_runtime_Error_getCode__E(xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_runtime_Error_Id xdc_runtime_Error_getId__E(xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_String xdc_runtime_Error_getMsg__E(xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_runtime_Types_Site* xdc_runtime_Error_getSite__E(xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Error_init__E(xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Error_print__E(xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Error_raiseX__E(xdc_runtime_Error_Block* eb, xdc_runtime_Types_ModuleId mod, xdc_String file, xdc_Int line, xdc_runtime_Error_Id id, xdc_IArg arg1, xdc_IArg arg2) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.GateNull PRAGMAS ========
 */

const CT__xdc_runtime_GateNull_Module__diagsEnabled xdc_runtime_GateNull_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__diagsIncluded xdc_runtime_GateNull_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__diagsMask xdc_runtime_GateNull_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__gateObj xdc_runtime_GateNull_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__gatePrms xdc_runtime_GateNull_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__id xdc_runtime_GateNull_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__loggerDefined xdc_runtime_GateNull_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__loggerObj xdc_runtime_GateNull_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__loggerFxn0 xdc_runtime_GateNull_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__loggerFxn1 xdc_runtime_GateNull_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__loggerFxn2 xdc_runtime_GateNull_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__loggerFxn4 xdc_runtime_GateNull_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__loggerFxn8 xdc_runtime_GateNull_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Module__startupDoneFxn xdc_runtime_GateNull_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Object__count xdc_runtime_GateNull_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Object__heap xdc_runtime_GateNull_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Object__sizeof xdc_runtime_GateNull_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_GateNull_Object__table xdc_runtime_GateNull_Object__table__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_GateNull_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_GateNull_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_GateNull_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_GateNull_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_GateNull_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_GateNull_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_GateNull_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_GateNull_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_GateNull_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_GateNull_query__E(xdc_Int qual) __attribute__ ((externally_visible));
xdc_IArg xdc_runtime_GateNull_enter__E(xdc_runtime_GateNull_Handle) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_GateNull_leave__E(xdc_runtime_GateNull_Handle, xdc_IArg key) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.HeapStd PRAGMAS ========
 */

const CT__xdc_runtime_HeapStd_Module__diagsEnabled xdc_runtime_HeapStd_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__diagsIncluded xdc_runtime_HeapStd_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__diagsMask xdc_runtime_HeapStd_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__gateObj xdc_runtime_HeapStd_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__gatePrms xdc_runtime_HeapStd_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__id xdc_runtime_HeapStd_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__loggerDefined xdc_runtime_HeapStd_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__loggerObj xdc_runtime_HeapStd_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__loggerFxn0 xdc_runtime_HeapStd_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__loggerFxn1 xdc_runtime_HeapStd_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__loggerFxn2 xdc_runtime_HeapStd_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__loggerFxn4 xdc_runtime_HeapStd_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__loggerFxn8 xdc_runtime_HeapStd_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Module__startupDoneFxn xdc_runtime_HeapStd_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Object__count xdc_runtime_HeapStd_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Object__heap xdc_runtime_HeapStd_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Object__sizeof xdc_runtime_HeapStd_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_Object__table xdc_runtime_HeapStd_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_E_noRTSMemory xdc_runtime_HeapStd_E_noRTSMemory__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_A_zeroSize xdc_runtime_HeapStd_A_zeroSize__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_A_invalidTotalFreeSize xdc_runtime_HeapStd_A_invalidTotalFreeSize__C __attribute__ ((externally_visible));
const CT__xdc_runtime_HeapStd_A_invalidAlignment xdc_runtime_HeapStd_A_invalidAlignment__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_HeapStd_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_HeapStd_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_HeapStd_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_HeapStd_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_HeapStd_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_HeapStd_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_HeapStd_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_HeapStd_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_HeapStd_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_HeapStd_free__E(xdc_runtime_HeapStd_Handle, xdc_Ptr block, xdc_SizeT size) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_HeapStd_getStats__E(xdc_runtime_HeapStd_Handle, xdc_runtime_Memory_Stats* stats) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_HeapStd_alloc__E(xdc_runtime_HeapStd_Handle, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_HeapStd_isBlocking__E(xdc_runtime_HeapStd_Handle) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Log PRAGMAS ========
 */

const CT__xdc_runtime_Log_Module__diagsEnabled xdc_runtime_Log_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__diagsIncluded xdc_runtime_Log_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__diagsMask xdc_runtime_Log_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__gateObj xdc_runtime_Log_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__gatePrms xdc_runtime_Log_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__id xdc_runtime_Log_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__loggerDefined xdc_runtime_Log_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__loggerObj xdc_runtime_Log_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__loggerFxn0 xdc_runtime_Log_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__loggerFxn1 xdc_runtime_Log_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__loggerFxn2 xdc_runtime_Log_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__loggerFxn4 xdc_runtime_Log_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__loggerFxn8 xdc_runtime_Log_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Module__startupDoneFxn xdc_runtime_Log_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Object__count xdc_runtime_Log_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Object__heap xdc_runtime_Log_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Object__sizeof xdc_runtime_Log_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_Object__table xdc_runtime_Log_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_construct xdc_runtime_Log_L_construct__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_create xdc_runtime_Log_L_create__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_destruct xdc_runtime_Log_L_destruct__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_delete xdc_runtime_Log_L_delete__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_error xdc_runtime_Log_L_error__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_warning xdc_runtime_Log_L_warning__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_info xdc_runtime_Log_L_info__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_start xdc_runtime_Log_L_start__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_stop xdc_runtime_Log_L_stop__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_startInstance xdc_runtime_Log_L_startInstance__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Log_L_stopInstance xdc_runtime_Log_L_stopInstance__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Log_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Log_doPrint__E(xdc_runtime_Log_EventRec* evRec) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.LoggerSys PRAGMAS ========
 */

const CT__xdc_runtime_LoggerSys_Module__diagsEnabled xdc_runtime_LoggerSys_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__diagsIncluded xdc_runtime_LoggerSys_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__diagsMask xdc_runtime_LoggerSys_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__gateObj xdc_runtime_LoggerSys_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__gatePrms xdc_runtime_LoggerSys_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__id xdc_runtime_LoggerSys_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__loggerDefined xdc_runtime_LoggerSys_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__loggerObj xdc_runtime_LoggerSys_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__loggerFxn0 xdc_runtime_LoggerSys_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__loggerFxn1 xdc_runtime_LoggerSys_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__loggerFxn2 xdc_runtime_LoggerSys_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__loggerFxn4 xdc_runtime_LoggerSys_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__loggerFxn8 xdc_runtime_LoggerSys_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Module__startupDoneFxn xdc_runtime_LoggerSys_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Object__count xdc_runtime_LoggerSys_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Object__heap xdc_runtime_LoggerSys_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Object__sizeof xdc_runtime_LoggerSys_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_LoggerSys_Object__table xdc_runtime_LoggerSys_Object__table__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_LoggerSys_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_LoggerSys_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_LoggerSys_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_LoggerSys_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_LoggerSys_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_LoggerSys_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_LoggerSys_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_LoggerSys_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_LoggerSys_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_LoggerSys_enable__E(xdc_runtime_LoggerSys_Handle) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_LoggerSys_disable__E(xdc_runtime_LoggerSys_Handle) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_LoggerSys_write0__E(xdc_runtime_LoggerSys_Handle, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_LoggerSys_write1__E(xdc_runtime_LoggerSys_Handle, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_LoggerSys_write2__E(xdc_runtime_LoggerSys_Handle, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_LoggerSys_write4__E(xdc_runtime_LoggerSys_Handle, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_LoggerSys_write8__E(xdc_runtime_LoggerSys_Handle, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Main PRAGMAS ========
 */

const CT__xdc_runtime_Main_Module__diagsEnabled xdc_runtime_Main_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__diagsIncluded xdc_runtime_Main_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__diagsMask xdc_runtime_Main_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__gateObj xdc_runtime_Main_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__gatePrms xdc_runtime_Main_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__id xdc_runtime_Main_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__loggerDefined xdc_runtime_Main_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__loggerObj xdc_runtime_Main_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__loggerFxn0 xdc_runtime_Main_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__loggerFxn1 xdc_runtime_Main_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__loggerFxn2 xdc_runtime_Main_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__loggerFxn4 xdc_runtime_Main_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__loggerFxn8 xdc_runtime_Main_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Module__startupDoneFxn xdc_runtime_Main_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Object__count xdc_runtime_Main_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Object__heap xdc_runtime_Main_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Object__sizeof xdc_runtime_Main_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Main_Object__table xdc_runtime_Main_Object__table__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Main_Module__startupDone__S(void) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Memory PRAGMAS ========
 */

const CT__xdc_runtime_Memory_Module__diagsEnabled xdc_runtime_Memory_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__diagsIncluded xdc_runtime_Memory_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__diagsMask xdc_runtime_Memory_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__gateObj xdc_runtime_Memory_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__gatePrms xdc_runtime_Memory_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__id xdc_runtime_Memory_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__loggerDefined xdc_runtime_Memory_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__loggerObj xdc_runtime_Memory_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__loggerFxn0 xdc_runtime_Memory_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__loggerFxn1 xdc_runtime_Memory_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__loggerFxn2 xdc_runtime_Memory_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__loggerFxn4 xdc_runtime_Memory_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__loggerFxn8 xdc_runtime_Memory_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Module__startupDoneFxn xdc_runtime_Memory_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Object__count xdc_runtime_Memory_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Object__heap xdc_runtime_Memory_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Object__sizeof xdc_runtime_Memory_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_Object__table xdc_runtime_Memory_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Memory_defaultHeapInstance xdc_runtime_Memory_defaultHeapInstance__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Memory_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_Memory_alloc__E(xdc_runtime_IHeap_Handle heap, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_Memory_calloc__E(xdc_runtime_IHeap_Handle heap, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Memory_free__E(xdc_runtime_IHeap_Handle heap, xdc_Ptr block, xdc_SizeT size) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Memory_getStats__E(xdc_runtime_IHeap_Handle heap, xdc_runtime_Memory_Stats* stats) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Memory_query__E(xdc_runtime_IHeap_Handle heap, xdc_Int qual) __attribute__ ((externally_visible));
xdc_SizeT xdc_runtime_Memory_getMaxDefaultTypeAlign__E(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_Memory_valloc__E(xdc_runtime_IHeap_Handle heap, xdc_SizeT size, xdc_SizeT align, xdc_Char value, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Registry PRAGMAS ========
 */

const CT__xdc_runtime_Registry_Module__diagsEnabled xdc_runtime_Registry_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__diagsIncluded xdc_runtime_Registry_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__diagsMask xdc_runtime_Registry_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__gateObj xdc_runtime_Registry_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__gatePrms xdc_runtime_Registry_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__id xdc_runtime_Registry_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__loggerDefined xdc_runtime_Registry_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__loggerObj xdc_runtime_Registry_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__loggerFxn0 xdc_runtime_Registry_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__loggerFxn1 xdc_runtime_Registry_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__loggerFxn2 xdc_runtime_Registry_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__loggerFxn4 xdc_runtime_Registry_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__loggerFxn8 xdc_runtime_Registry_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Module__startupDoneFxn xdc_runtime_Registry_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Object__count xdc_runtime_Registry_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Object__heap xdc_runtime_Registry_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Object__sizeof xdc_runtime_Registry_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Registry_Object__table xdc_runtime_Registry_Object__table__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Registry_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_runtime_Registry_Result xdc_runtime_Registry_addModule__E(xdc_runtime_Registry_Desc* desc, xdc_String modName) __attribute__ ((externally_visible));
xdc_runtime_Types_RegDesc* xdc_runtime_Registry_findByName__E(xdc_String modName) __attribute__ ((externally_visible));
xdc_runtime_Types_RegDesc* xdc_runtime_Registry_findByNamePattern__E(xdc_String namePat, xdc_Int len, xdc_runtime_Registry_Desc* prev) __attribute__ ((externally_visible));
xdc_runtime_Types_RegDesc* xdc_runtime_Registry_findById__E(xdc_runtime_Types_ModuleId mid) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Registry_getMask__E(xdc_String name, xdc_runtime_Types_DiagsMask* mask) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Registry_isMember__E(xdc_runtime_Types_ModuleId mid) __attribute__ ((externally_visible));
xdc_runtime_Types_RegDesc* xdc_runtime_Registry_getNextModule__E(xdc_runtime_Registry_Desc* desc) __attribute__ ((externally_visible));
xdc_String xdc_runtime_Registry_getModuleName__E(xdc_runtime_Registry_Desc* desc) __attribute__ ((externally_visible));
xdc_Bits16 xdc_runtime_Registry_getModuleId__E(xdc_runtime_Registry_Desc* desc) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Rta PRAGMAS ========
 */

const CT__xdc_runtime_Rta_Module__diagsEnabled xdc_runtime_Rta_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__diagsIncluded xdc_runtime_Rta_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__diagsMask xdc_runtime_Rta_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__gateObj xdc_runtime_Rta_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__gatePrms xdc_runtime_Rta_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__id xdc_runtime_Rta_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__loggerDefined xdc_runtime_Rta_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__loggerObj xdc_runtime_Rta_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__loggerFxn0 xdc_runtime_Rta_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__loggerFxn1 xdc_runtime_Rta_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__loggerFxn2 xdc_runtime_Rta_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__loggerFxn4 xdc_runtime_Rta_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__loggerFxn8 xdc_runtime_Rta_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Module__startupDoneFxn xdc_runtime_Rta_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Object__count xdc_runtime_Rta_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Object__heap xdc_runtime_Rta_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Object__sizeof xdc_runtime_Rta_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_Object__table xdc_runtime_Rta_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_LD_cmdRcvd xdc_runtime_Rta_LD_cmdRcvd__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_LD_writeMask xdc_runtime_Rta_LD_writeMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_A_invalidLogger xdc_runtime_Rta_A_invalidLogger__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_E_badCommand xdc_runtime_Rta_E_badCommand__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_dataTransportClassName xdc_runtime_Rta_dataTransportClassName__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Rta_controlTransportClassName xdc_runtime_Rta_controlTransportClassName__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Rta_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Rta_processCommand__E(xdc_runtime_Rta_CommandPacket* cmd, xdc_runtime_Rta_ResponsePacket* resp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Rta_acknowledgeCmd__E(xdc_runtime_Rta_ResponsePacket* resp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Rta_readMask__E(xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg addr) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Rta_writeMask__E(xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg addr, xdc_UArg val) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Rta_enableLog__E(xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg log) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Rta_disableLog__E(xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg log) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Rta_getCpuSpeed__E(xdc_runtime_Rta_ResponsePacket* resp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Rta_resetLog__E(xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg log) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Rta_changePeriod__E(xdc_runtime_Rta_ResponsePacket* resp, xdc_UArg period) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Startup PRAGMAS ========
 */

const CT__xdc_runtime_Startup_Module__diagsEnabled xdc_runtime_Startup_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__diagsIncluded xdc_runtime_Startup_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__diagsMask xdc_runtime_Startup_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__gateObj xdc_runtime_Startup_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__gatePrms xdc_runtime_Startup_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__id xdc_runtime_Startup_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__loggerDefined xdc_runtime_Startup_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__loggerObj xdc_runtime_Startup_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__loggerFxn0 xdc_runtime_Startup_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__loggerFxn1 xdc_runtime_Startup_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__loggerFxn2 xdc_runtime_Startup_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__loggerFxn4 xdc_runtime_Startup_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__loggerFxn8 xdc_runtime_Startup_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Module__startupDoneFxn xdc_runtime_Startup_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Object__count xdc_runtime_Startup_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Object__heap xdc_runtime_Startup_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Object__sizeof xdc_runtime_Startup_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_Object__table xdc_runtime_Startup_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_maxPasses xdc_runtime_Startup_maxPasses__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_firstFxns xdc_runtime_Startup_firstFxns__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_lastFxns xdc_runtime_Startup_lastFxns__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_startModsFxn xdc_runtime_Startup_startModsFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_execImpl xdc_runtime_Startup_execImpl__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_sfxnTab xdc_runtime_Startup_sfxnTab__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Startup_sfxnRts xdc_runtime_Startup_sfxnRts__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Startup_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Startup_exec__E(void) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Startup_rtsDone__E(void) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.SysStd PRAGMAS ========
 */

const CT__xdc_runtime_SysStd_Module__diagsEnabled xdc_runtime_SysStd_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__diagsIncluded xdc_runtime_SysStd_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__diagsMask xdc_runtime_SysStd_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__gateObj xdc_runtime_SysStd_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__gatePrms xdc_runtime_SysStd_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__id xdc_runtime_SysStd_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__loggerDefined xdc_runtime_SysStd_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__loggerObj xdc_runtime_SysStd_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__loggerFxn0 xdc_runtime_SysStd_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__loggerFxn1 xdc_runtime_SysStd_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__loggerFxn2 xdc_runtime_SysStd_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__loggerFxn4 xdc_runtime_SysStd_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__loggerFxn8 xdc_runtime_SysStd_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Module__startupDoneFxn xdc_runtime_SysStd_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Object__count xdc_runtime_SysStd_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Object__heap xdc_runtime_SysStd_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Object__sizeof xdc_runtime_SysStd_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_SysStd_Object__table xdc_runtime_SysStd_Object__table__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_SysStd_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_SysStd_abort__E(xdc_String str) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_SysStd_exit__E(xdc_Int stat) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_SysStd_flush__E(void) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_SysStd_putch__E(xdc_Char ch) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_SysStd_ready__E(void) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.System PRAGMAS ========
 */

const CT__xdc_runtime_System_Module__diagsEnabled xdc_runtime_System_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__diagsIncluded xdc_runtime_System_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__diagsMask xdc_runtime_System_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__gateObj xdc_runtime_System_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__gatePrms xdc_runtime_System_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__id xdc_runtime_System_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__loggerDefined xdc_runtime_System_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__loggerObj xdc_runtime_System_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__loggerFxn0 xdc_runtime_System_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__loggerFxn1 xdc_runtime_System_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__loggerFxn2 xdc_runtime_System_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__loggerFxn4 xdc_runtime_System_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__loggerFxn8 xdc_runtime_System_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Module__startupDoneFxn xdc_runtime_System_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Object__count xdc_runtime_System_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Object__heap xdc_runtime_System_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Object__sizeof xdc_runtime_System_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_Object__table xdc_runtime_System_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_A_cannotFitIntoArg xdc_runtime_System_A_cannotFitIntoArg__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_maxAtexitHandlers xdc_runtime_System_maxAtexitHandlers__C __attribute__ ((externally_visible));
const CT__xdc_runtime_System_extendFxn xdc_runtime_System_extendFxn__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_System_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_System_abort__E(xdc_String str) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_System_atexit__E(xdc_runtime_System_AtexitHandler handler) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_System_exit__E(xdc_Int stat) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_System_putch__E(xdc_Char ch) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_System_flush__E(void) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_System_printf__E(xdc_String fmt, ...) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_System_aprintf__E(xdc_String fmt, ...) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_System_sprintf__E(xdc_Char buf[], xdc_String fmt, ...) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_System_asprintf__E(xdc_Char buf[], xdc_String fmt, ...) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_System_vprintf__E(xdc_String fmt, xdc_VaList va) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_System_avprintf__E(xdc_String fmt, xdc_VaList va) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_System_vsprintf__E(xdc_Char buf[], xdc_String fmt, xdc_VaList va) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_System_avsprintf__E(xdc_Char buf[], xdc_String fmt, xdc_VaList va) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Text PRAGMAS ========
 */

const CT__xdc_runtime_Text_Module__diagsEnabled xdc_runtime_Text_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__diagsIncluded xdc_runtime_Text_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__diagsMask xdc_runtime_Text_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__gateObj xdc_runtime_Text_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__gatePrms xdc_runtime_Text_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__id xdc_runtime_Text_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__loggerDefined xdc_runtime_Text_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__loggerObj xdc_runtime_Text_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__loggerFxn0 xdc_runtime_Text_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__loggerFxn1 xdc_runtime_Text_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__loggerFxn2 xdc_runtime_Text_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__loggerFxn4 xdc_runtime_Text_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__loggerFxn8 xdc_runtime_Text_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Module__startupDoneFxn xdc_runtime_Text_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Object__count xdc_runtime_Text_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Object__heap xdc_runtime_Text_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Object__sizeof xdc_runtime_Text_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_Object__table xdc_runtime_Text_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_nameUnknown xdc_runtime_Text_nameUnknown__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_nameEmpty xdc_runtime_Text_nameEmpty__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_nameStatic xdc_runtime_Text_nameStatic__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_isLoaded xdc_runtime_Text_isLoaded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_charTab xdc_runtime_Text_charTab__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_nodeTab xdc_runtime_Text_nodeTab__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_charCnt xdc_runtime_Text_charCnt__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_nodeCnt xdc_runtime_Text_nodeCnt__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_unnamedModsLastId xdc_runtime_Text_unnamedModsLastId__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_registryModsLastId xdc_runtime_Text_registryModsLastId__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_visitRopeFxn xdc_runtime_Text_visitRopeFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Text_visitRopeFxn2 xdc_runtime_Text_visitRopeFxn2__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Text_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_String xdc_runtime_Text_cordText__E(xdc_runtime_Text_CordAddr cord) __attribute__ ((externally_visible));
xdc_String xdc_runtime_Text_ropeText__E(xdc_runtime_Text_RopeId rope) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_Text_matchRope__E(xdc_runtime_Text_RopeId rope, xdc_String pat, xdc_Int* lenp) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_Text_putLab__E(xdc_runtime_Types_Label* lab, xdc_Char** bufp, xdc_Int len) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_Text_putMod__E(xdc_runtime_Types_ModuleId mid, xdc_Char** bufp, xdc_Int len) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_Text_putSite__E(xdc_runtime_Types_Site* site, xdc_Char** bufp, xdc_Int len) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.Timestamp PRAGMAS ========
 */

const CT__xdc_runtime_Timestamp_Module__diagsEnabled xdc_runtime_Timestamp_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__diagsIncluded xdc_runtime_Timestamp_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__diagsMask xdc_runtime_Timestamp_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__gateObj xdc_runtime_Timestamp_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__gatePrms xdc_runtime_Timestamp_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__id xdc_runtime_Timestamp_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__loggerDefined xdc_runtime_Timestamp_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__loggerObj xdc_runtime_Timestamp_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__loggerFxn0 xdc_runtime_Timestamp_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__loggerFxn1 xdc_runtime_Timestamp_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__loggerFxn2 xdc_runtime_Timestamp_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__loggerFxn4 xdc_runtime_Timestamp_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__loggerFxn8 xdc_runtime_Timestamp_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Module__startupDoneFxn xdc_runtime_Timestamp_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Object__count xdc_runtime_Timestamp_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Object__heap xdc_runtime_Timestamp_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Object__sizeof xdc_runtime_Timestamp_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_Timestamp_Object__table xdc_runtime_Timestamp_Object__table__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_Timestamp_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Bits32 xdc_runtime_Timestamp_get32__E(void) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Timestamp_get64__E(xdc_runtime_Types_Timestamp64* result) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_Timestamp_getFreq__E(xdc_runtime_Types_FreqHz* freq) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.GateH PRAGMAS ========
 */

const CT__xdc_runtime_knl_GateH_Module__diagsEnabled xdc_runtime_knl_GateH_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__diagsIncluded xdc_runtime_knl_GateH_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__diagsMask xdc_runtime_knl_GateH_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__gateObj xdc_runtime_knl_GateH_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__gatePrms xdc_runtime_knl_GateH_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__id xdc_runtime_knl_GateH_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__loggerDefined xdc_runtime_knl_GateH_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__loggerObj xdc_runtime_knl_GateH_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__loggerFxn0 xdc_runtime_knl_GateH_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__loggerFxn1 xdc_runtime_knl_GateH_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__loggerFxn2 xdc_runtime_knl_GateH_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__loggerFxn4 xdc_runtime_knl_GateH_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__loggerFxn8 xdc_runtime_knl_GateH_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Module__startupDoneFxn xdc_runtime_knl_GateH_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Object__count xdc_runtime_knl_GateH_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Object__heap xdc_runtime_knl_GateH_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Object__sizeof xdc_runtime_knl_GateH_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateH_Object__table xdc_runtime_knl_GateH_Object__table__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_GateH_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_IArg xdc_runtime_knl_GateH_enter__E(xdc_runtime_IGateProvider_Handle hdl) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_GateH_leave__E(xdc_runtime_IGateProvider_Handle hdl, xdc_IArg key) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.GateProcess PRAGMAS ========
 */

const CT__xdc_runtime_knl_GateProcess_Module__diagsEnabled xdc_runtime_knl_GateProcess_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__diagsIncluded xdc_runtime_knl_GateProcess_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__diagsMask xdc_runtime_knl_GateProcess_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__gateObj xdc_runtime_knl_GateProcess_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__gatePrms xdc_runtime_knl_GateProcess_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__id xdc_runtime_knl_GateProcess_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__loggerDefined xdc_runtime_knl_GateProcess_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__loggerObj xdc_runtime_knl_GateProcess_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__loggerFxn0 xdc_runtime_knl_GateProcess_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__loggerFxn1 xdc_runtime_knl_GateProcess_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__loggerFxn2 xdc_runtime_knl_GateProcess_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__loggerFxn4 xdc_runtime_knl_GateProcess_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__loggerFxn8 xdc_runtime_knl_GateProcess_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Module__startupDoneFxn xdc_runtime_knl_GateProcess_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Object__count xdc_runtime_knl_GateProcess_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Object__heap xdc_runtime_knl_GateProcess_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Object__sizeof xdc_runtime_knl_GateProcess_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_Object__table xdc_runtime_knl_GateProcess_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateProcess_A_invalidKey xdc_runtime_knl_GateProcess_A_invalidKey__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_knl_GateProcess_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_GateProcess_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_GateProcess_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_GateProcess_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_GateProcess_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_GateProcess_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_GateProcess_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_GateProcess_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_GateProcess_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_GateProcess_query__E(xdc_Int qual) __attribute__ ((externally_visible));
xdc_IArg xdc_runtime_knl_GateProcess_enter__E(xdc_runtime_knl_GateProcess_Handle) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_GateProcess_leave__E(xdc_runtime_knl_GateProcess_Handle, xdc_IArg key) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_knl_GateProcess_getReferenceCount__E(xdc_runtime_knl_GateProcess_Handle, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.GateThread PRAGMAS ========
 */

const CT__xdc_runtime_knl_GateThread_Module__diagsEnabled xdc_runtime_knl_GateThread_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__diagsIncluded xdc_runtime_knl_GateThread_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__diagsMask xdc_runtime_knl_GateThread_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__gateObj xdc_runtime_knl_GateThread_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__gatePrms xdc_runtime_knl_GateThread_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__id xdc_runtime_knl_GateThread_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__loggerDefined xdc_runtime_knl_GateThread_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__loggerObj xdc_runtime_knl_GateThread_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__loggerFxn0 xdc_runtime_knl_GateThread_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__loggerFxn1 xdc_runtime_knl_GateThread_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__loggerFxn2 xdc_runtime_knl_GateThread_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__loggerFxn4 xdc_runtime_knl_GateThread_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__loggerFxn8 xdc_runtime_knl_GateThread_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Module__startupDoneFxn xdc_runtime_knl_GateThread_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Object__count xdc_runtime_knl_GateThread_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Object__heap xdc_runtime_knl_GateThread_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Object__sizeof xdc_runtime_knl_GateThread_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_GateThread_Object__table xdc_runtime_knl_GateThread_Object__table__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_knl_GateThread_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_GateThread_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_GateThread_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_GateThread_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_GateThread_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_GateThread_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_GateThread_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_GateThread_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_GateThread_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_GateThread_query__E(xdc_Int qual) __attribute__ ((externally_visible));
xdc_IArg xdc_runtime_knl_GateThread_enter__E(xdc_runtime_knl_GateThread_Handle) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_GateThread_leave__E(xdc_runtime_knl_GateThread_Handle, xdc_IArg key) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.SemProcess PRAGMAS ========
 */

const CT__xdc_runtime_knl_SemProcess_Module__diagsEnabled xdc_runtime_knl_SemProcess_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__diagsIncluded xdc_runtime_knl_SemProcess_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__diagsMask xdc_runtime_knl_SemProcess_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__gateObj xdc_runtime_knl_SemProcess_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__gatePrms xdc_runtime_knl_SemProcess_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__id xdc_runtime_knl_SemProcess_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__loggerDefined xdc_runtime_knl_SemProcess_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__loggerObj xdc_runtime_knl_SemProcess_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__loggerFxn0 xdc_runtime_knl_SemProcess_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__loggerFxn1 xdc_runtime_knl_SemProcess_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__loggerFxn2 xdc_runtime_knl_SemProcess_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__loggerFxn4 xdc_runtime_knl_SemProcess_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__loggerFxn8 xdc_runtime_knl_SemProcess_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Module__startupDoneFxn xdc_runtime_knl_SemProcess_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Object__count xdc_runtime_knl_SemProcess_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Object__heap xdc_runtime_knl_SemProcess_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Object__sizeof xdc_runtime_knl_SemProcess_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemProcess_Object__table xdc_runtime_knl_SemProcess_Object__table__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_knl_SemProcess_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_SemProcess_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SemProcess_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SemProcess_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SemProcess_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SemProcess_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SemProcess_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SemProcess_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SemProcess_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_knl_SemProcess_pend__E(xdc_runtime_knl_SemProcess_Handle, xdc_UInt timeout, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_SemProcess_post__E(xdc_runtime_knl_SemProcess_Handle, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.SemThread PRAGMAS ========
 */

const CT__xdc_runtime_knl_SemThread_Module__diagsEnabled xdc_runtime_knl_SemThread_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__diagsIncluded xdc_runtime_knl_SemThread_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__diagsMask xdc_runtime_knl_SemThread_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__gateObj xdc_runtime_knl_SemThread_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__gatePrms xdc_runtime_knl_SemThread_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__id xdc_runtime_knl_SemThread_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__loggerDefined xdc_runtime_knl_SemThread_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__loggerObj xdc_runtime_knl_SemThread_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__loggerFxn0 xdc_runtime_knl_SemThread_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__loggerFxn1 xdc_runtime_knl_SemThread_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__loggerFxn2 xdc_runtime_knl_SemThread_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__loggerFxn4 xdc_runtime_knl_SemThread_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__loggerFxn8 xdc_runtime_knl_SemThread_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Module__startupDoneFxn xdc_runtime_knl_SemThread_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Object__count xdc_runtime_knl_SemThread_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Object__heap xdc_runtime_knl_SemThread_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Object__sizeof xdc_runtime_knl_SemThread_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SemThread_Object__table xdc_runtime_knl_SemThread_Object__table__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_knl_SemThread_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_SemThread_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SemThread_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SemThread_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SemThread_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SemThread_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SemThread_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SemThread_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SemThread_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_knl_SemThread_pend__E(xdc_runtime_knl_SemThread_Handle, xdc_UInt timeout, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_SemThread_post__E(xdc_runtime_knl_SemThread_Handle, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.Semaphore PRAGMAS ========
 */

const CT__xdc_runtime_knl_Semaphore_Module__diagsEnabled xdc_runtime_knl_Semaphore_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__diagsIncluded xdc_runtime_knl_Semaphore_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__diagsMask xdc_runtime_knl_Semaphore_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__gateObj xdc_runtime_knl_Semaphore_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__gatePrms xdc_runtime_knl_Semaphore_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__id xdc_runtime_knl_Semaphore_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__loggerDefined xdc_runtime_knl_Semaphore_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__loggerObj xdc_runtime_knl_Semaphore_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__loggerFxn0 xdc_runtime_knl_Semaphore_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__loggerFxn1 xdc_runtime_knl_Semaphore_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__loggerFxn2 xdc_runtime_knl_Semaphore_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__loggerFxn4 xdc_runtime_knl_Semaphore_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__loggerFxn8 xdc_runtime_knl_Semaphore_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Module__startupDoneFxn xdc_runtime_knl_Semaphore_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Object__count xdc_runtime_knl_Semaphore_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Object__heap xdc_runtime_knl_Semaphore_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Object__sizeof xdc_runtime_knl_Semaphore_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Semaphore_Object__table xdc_runtime_knl_Semaphore_Object__table__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Semaphore_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_knl_Semaphore_pend__E(xdc_runtime_knl_ISemaphore_Handle sem, xdc_UInt timeout, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Semaphore_post__E(xdc_runtime_knl_ISemaphore_Handle sem, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.Sync PRAGMAS ========
 */

const CT__xdc_runtime_knl_Sync_Module__diagsEnabled xdc_runtime_knl_Sync_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__diagsIncluded xdc_runtime_knl_Sync_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__diagsMask xdc_runtime_knl_Sync_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__gateObj xdc_runtime_knl_Sync_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__gatePrms xdc_runtime_knl_Sync_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__id xdc_runtime_knl_Sync_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__loggerDefined xdc_runtime_knl_Sync_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__loggerObj xdc_runtime_knl_Sync_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__loggerFxn0 xdc_runtime_knl_Sync_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__loggerFxn1 xdc_runtime_knl_Sync_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__loggerFxn2 xdc_runtime_knl_Sync_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__loggerFxn4 xdc_runtime_knl_Sync_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__loggerFxn8 xdc_runtime_knl_Sync_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Module__startupDoneFxn xdc_runtime_knl_Sync_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Object__count xdc_runtime_knl_Sync_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Object__heap xdc_runtime_knl_Sync_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Object__sizeof xdc_runtime_knl_Sync_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Sync_Object__table xdc_runtime_knl_Sync_Object__table__C __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Sync_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Sync_query__E(xdc_runtime_knl_ISync_Handle sync, xdc_Int qual) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_Sync_signal__E(xdc_runtime_knl_ISync_Handle sync) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_knl_Sync_wait__E(xdc_runtime_knl_ISync_Handle sync, xdc_UInt timeout, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.SyncGeneric PRAGMAS ========
 */

const CT__xdc_runtime_knl_SyncGeneric_Module__diagsEnabled xdc_runtime_knl_SyncGeneric_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__diagsIncluded xdc_runtime_knl_SyncGeneric_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__diagsMask xdc_runtime_knl_SyncGeneric_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__gateObj xdc_runtime_knl_SyncGeneric_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__gatePrms xdc_runtime_knl_SyncGeneric_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__id xdc_runtime_knl_SyncGeneric_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__loggerDefined xdc_runtime_knl_SyncGeneric_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__loggerObj xdc_runtime_knl_SyncGeneric_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn0 xdc_runtime_knl_SyncGeneric_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn1 xdc_runtime_knl_SyncGeneric_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn2 xdc_runtime_knl_SyncGeneric_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn4 xdc_runtime_knl_SyncGeneric_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__loggerFxn8 xdc_runtime_knl_SyncGeneric_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Module__startupDoneFxn xdc_runtime_knl_SyncGeneric_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Object__count xdc_runtime_knl_SyncGeneric_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Object__heap xdc_runtime_knl_SyncGeneric_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Object__sizeof xdc_runtime_knl_SyncGeneric_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncGeneric_Object__table xdc_runtime_knl_SyncGeneric_Object__table__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_knl_SyncGeneric_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_SyncGeneric_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncGeneric_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncGeneric_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncGeneric_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncGeneric_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncGeneric_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncGeneric_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncGeneric_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_SyncGeneric_query__E(xdc_runtime_knl_SyncGeneric_Handle, xdc_Int qual) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncGeneric_signal__E(xdc_runtime_knl_SyncGeneric_Handle) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_knl_SyncGeneric_wait__E(xdc_runtime_knl_SyncGeneric_Handle, xdc_UInt timeout, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.SyncNull PRAGMAS ========
 */

const CT__xdc_runtime_knl_SyncNull_Module__diagsEnabled xdc_runtime_knl_SyncNull_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__diagsIncluded xdc_runtime_knl_SyncNull_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__diagsMask xdc_runtime_knl_SyncNull_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__gateObj xdc_runtime_knl_SyncNull_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__gatePrms xdc_runtime_knl_SyncNull_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__id xdc_runtime_knl_SyncNull_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__loggerDefined xdc_runtime_knl_SyncNull_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__loggerObj xdc_runtime_knl_SyncNull_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__loggerFxn0 xdc_runtime_knl_SyncNull_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__loggerFxn1 xdc_runtime_knl_SyncNull_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__loggerFxn2 xdc_runtime_knl_SyncNull_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__loggerFxn4 xdc_runtime_knl_SyncNull_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__loggerFxn8 xdc_runtime_knl_SyncNull_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Module__startupDoneFxn xdc_runtime_knl_SyncNull_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Object__count xdc_runtime_knl_SyncNull_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Object__heap xdc_runtime_knl_SyncNull_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Object__sizeof xdc_runtime_knl_SyncNull_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncNull_Object__table xdc_runtime_knl_SyncNull_Object__table__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_knl_SyncNull_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_SyncNull_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncNull_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncNull_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncNull_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncNull_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncNull_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncNull_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncNull_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_SyncNull_query__E(xdc_runtime_knl_SyncNull_Handle, xdc_Int qual) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncNull_signal__E(xdc_runtime_knl_SyncNull_Handle) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_knl_SyncNull_wait__E(xdc_runtime_knl_SyncNull_Handle, xdc_UInt timeout, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.SyncSemThread PRAGMAS ========
 */

const CT__xdc_runtime_knl_SyncSemThread_Module__diagsEnabled xdc_runtime_knl_SyncSemThread_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__diagsIncluded xdc_runtime_knl_SyncSemThread_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__diagsMask xdc_runtime_knl_SyncSemThread_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__gateObj xdc_runtime_knl_SyncSemThread_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__gatePrms xdc_runtime_knl_SyncSemThread_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__id xdc_runtime_knl_SyncSemThread_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__loggerDefined xdc_runtime_knl_SyncSemThread_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__loggerObj xdc_runtime_knl_SyncSemThread_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn0 xdc_runtime_knl_SyncSemThread_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn1 xdc_runtime_knl_SyncSemThread_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn2 xdc_runtime_knl_SyncSemThread_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn4 xdc_runtime_knl_SyncSemThread_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__loggerFxn8 xdc_runtime_knl_SyncSemThread_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Module__startupDoneFxn xdc_runtime_knl_SyncSemThread_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Object__count xdc_runtime_knl_SyncSemThread_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Object__heap xdc_runtime_knl_SyncSemThread_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Object__sizeof xdc_runtime_knl_SyncSemThread_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_SyncSemThread_Object__table xdc_runtime_knl_SyncSemThread_Object__table__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_knl_SyncSemThread_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_SyncSemThread_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncSemThread_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncSemThread_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncSemThread_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncSemThread_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncSemThread_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_SyncSemThread_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncSemThread_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_SyncSemThread_query__E(xdc_runtime_knl_SyncSemThread_Handle, xdc_Int qual) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_SyncSemThread_signal__E(xdc_runtime_knl_SyncSemThread_Handle) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_knl_SyncSemThread_wait__E(xdc_runtime_knl_SyncSemThread_Handle, xdc_UInt timeout, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));

/*
 * ======== xdc.runtime.knl.Thread PRAGMAS ========
 */

const CT__xdc_runtime_knl_Thread_Module__diagsEnabled xdc_runtime_knl_Thread_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__diagsIncluded xdc_runtime_knl_Thread_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__diagsMask xdc_runtime_knl_Thread_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__gateObj xdc_runtime_knl_Thread_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__gatePrms xdc_runtime_knl_Thread_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__id xdc_runtime_knl_Thread_Module__id__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__loggerDefined xdc_runtime_knl_Thread_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__loggerObj xdc_runtime_knl_Thread_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__loggerFxn0 xdc_runtime_knl_Thread_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__loggerFxn1 xdc_runtime_knl_Thread_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__loggerFxn2 xdc_runtime_knl_Thread_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__loggerFxn4 xdc_runtime_knl_Thread_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__loggerFxn8 xdc_runtime_knl_Thread_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Module__startupDoneFxn xdc_runtime_knl_Thread_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Object__count xdc_runtime_knl_Thread_Object__count__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Object__heap xdc_runtime_knl_Thread_Object__heap__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Object__sizeof xdc_runtime_knl_Thread_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_Object__table xdc_runtime_knl_Thread_Object__table__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_A_zeroTimeout xdc_runtime_knl_Thread_A_zeroTimeout__C __attribute__ ((externally_visible));
const CT__xdc_runtime_knl_Thread_defaultStackSize xdc_runtime_knl_Thread_defaultStackSize__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdc_runtime_knl_Thread_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Thread_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_Thread_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_Thread_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_Thread_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_Thread_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_Thread_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_Thread_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_Thread_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Thread_start__E(xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Thread_yield__E(xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_knl_Thread_compareOsPriorities__E(xdc_Int p1, xdc_Int p2, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Thread_sleep__E(xdc_UInt timeout, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Thread_join__E(xdc_runtime_knl_Thread_Handle, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_runtime_knl_Thread_Priority xdc_runtime_knl_Thread_getPriority__E(xdc_runtime_knl_Thread_Handle, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Thread_setPriority__E(xdc_runtime_knl_Thread_Handle, xdc_runtime_knl_Thread_Priority newPri, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Int xdc_runtime_knl_Thread_getOsPriority__E(xdc_runtime_knl_Thread_Handle, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Thread_setOsPriority__E(xdc_runtime_knl_Thread_Handle, xdc_Int newPri, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_Thread_getOsHandle__E(xdc_runtime_knl_Thread_Handle) __attribute__ ((externally_visible));
xdc_Ptr xdc_runtime_knl_Thread_getTls__E(xdc_runtime_knl_Thread_Handle) __attribute__ ((externally_visible));
xdc_Void xdc_runtime_knl_Thread_setTls__E(xdc_runtime_knl_Thread_Handle, xdc_Ptr tls) __attribute__ ((externally_visible));
xdc_Bool xdc_runtime_knl_Thread_stat__E(xdc_runtime_knl_Thread_Handle, xdc_runtime_knl_Thread_Stat* buf, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));

/*
 * ======== xdcruntime.cstubs.LoggerFlex PRAGMAS ========
 */

const CT__xdcruntime_cstubs_LoggerFlex_Module__diagsEnabled xdcruntime_cstubs_LoggerFlex_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__diagsIncluded xdcruntime_cstubs_LoggerFlex_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__diagsMask xdcruntime_cstubs_LoggerFlex_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__gateObj xdcruntime_cstubs_LoggerFlex_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__gatePrms xdcruntime_cstubs_LoggerFlex_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__id xdcruntime_cstubs_LoggerFlex_Module__id__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerDefined xdcruntime_cstubs_LoggerFlex_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerObj xdcruntime_cstubs_LoggerFlex_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn0 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn1 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn2 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn4 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn8 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Module__startupDoneFxn xdcruntime_cstubs_LoggerFlex_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Object__count xdcruntime_cstubs_LoggerFlex_Object__count__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Object__heap xdcruntime_cstubs_LoggerFlex_Object__heap__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Object__sizeof xdcruntime_cstubs_LoggerFlex_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_Object__table xdcruntime_cstubs_LoggerFlex_Object__table__C __attribute__ ((externally_visible));
const CT__xdcruntime_cstubs_LoggerFlex_E_atexit xdcruntime_cstubs_LoggerFlex_E_atexit__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdcruntime_cstubs_LoggerFlex_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdcruntime_cstubs_LoggerFlex_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdcruntime_cstubs_LoggerFlex_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdcruntime_cstubs_LoggerFlex_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdcruntime_cstubs_LoggerFlex_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Bool xdcruntime_cstubs_LoggerFlex_enable__E(xdcruntime_cstubs_LoggerFlex_Handle) __attribute__ ((externally_visible));
xdc_Bool xdcruntime_cstubs_LoggerFlex_disable__E(xdcruntime_cstubs_LoggerFlex_Handle) __attribute__ ((externally_visible));
xdc_Void xdcruntime_cstubs_LoggerFlex_write0__E(xdcruntime_cstubs_LoggerFlex_Handle, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid) __attribute__ ((externally_visible));
xdc_Void xdcruntime_cstubs_LoggerFlex_write1__E(xdcruntime_cstubs_LoggerFlex_Handle, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1) __attribute__ ((externally_visible));
xdc_Void xdcruntime_cstubs_LoggerFlex_write2__E(xdcruntime_cstubs_LoggerFlex_Handle, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2) __attribute__ ((externally_visible));
xdc_Void xdcruntime_cstubs_LoggerFlex_write4__E(xdcruntime_cstubs_LoggerFlex_Handle, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4) __attribute__ ((externally_visible));
xdc_Void xdcruntime_cstubs_LoggerFlex_write8__E(xdcruntime_cstubs_LoggerFlex_Handle, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8) __attribute__ ((externally_visible));
xdc_Void xdcruntime_cstubs_LoggerFlex_shutdown__E(xdc_Int status) __attribute__ ((externally_visible));

/*
 * ======== xdcruntime.heaps.HeapStdAlign PRAGMAS ========
 */

const CT__xdcruntime_heaps_HeapStdAlign_Module__diagsEnabled xdcruntime_heaps_HeapStdAlign_Module__diagsEnabled__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__diagsIncluded xdcruntime_heaps_HeapStdAlign_Module__diagsIncluded__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__diagsMask xdcruntime_heaps_HeapStdAlign_Module__diagsMask__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__gateObj xdcruntime_heaps_HeapStdAlign_Module__gateObj__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__gatePrms xdcruntime_heaps_HeapStdAlign_Module__gatePrms__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__id xdcruntime_heaps_HeapStdAlign_Module__id__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerDefined xdcruntime_heaps_HeapStdAlign_Module__loggerDefined__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerObj xdcruntime_heaps_HeapStdAlign_Module__loggerObj__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn0 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn0__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn1 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn1__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn2 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn2__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn4 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn4__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn8 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn8__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Module__startupDoneFxn xdcruntime_heaps_HeapStdAlign_Module__startupDoneFxn__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Object__count xdcruntime_heaps_HeapStdAlign_Object__count__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Object__heap xdcruntime_heaps_HeapStdAlign_Object__heap__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Object__sizeof xdcruntime_heaps_HeapStdAlign_Object__sizeof__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_Object__table xdcruntime_heaps_HeapStdAlign_Object__table__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_E_noRTSMemory xdcruntime_heaps_HeapStdAlign_E_noRTSMemory__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_A_zeroSize xdcruntime_heaps_HeapStdAlign_A_zeroSize__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_A_align xdcruntime_heaps_HeapStdAlign_A_align__C __attribute__ ((externally_visible));
const CT__xdcruntime_heaps_HeapStdAlign_A_invalidTotalFreeSize xdcruntime_heaps_HeapStdAlign_A_invalidTotalFreeSize__C __attribute__ ((externally_visible));
xdc_runtime_Types_Label* xdcruntime_heaps_HeapStdAlign_Handle__label__S(xdc_Ptr obj, xdc_runtime_Types_Label* lab) __attribute__ ((externally_visible));
xdc_Bool xdcruntime_heaps_HeapStdAlign_Module__startupDone__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__create__S(xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb) __attribute__ ((externally_visible));
xdc_Void xdcruntime_heaps_HeapStdAlign_Object__delete__S(xdc_Ptr instp) __attribute__ ((externally_visible));
xdc_Void xdcruntime_heaps_HeapStdAlign_Object__destruct__S(xdc_Ptr objp) __attribute__ ((externally_visible));
xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__get__S(xdc_Ptr oarr, xdc_Int i) __attribute__ ((externally_visible));
xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__first__S(void) __attribute__ ((externally_visible));
xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__next__S(xdc_Ptr obj) __attribute__ ((externally_visible));
xdc_Void xdcruntime_heaps_HeapStdAlign_Params__init__S(xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz) __attribute__ ((externally_visible));
xdc_Void xdcruntime_heaps_HeapStdAlign_free__E(xdcruntime_heaps_HeapStdAlign_Handle, xdc_Ptr block, xdc_SizeT size) __attribute__ ((externally_visible));
xdc_Void xdcruntime_heaps_HeapStdAlign_getStats__E(xdcruntime_heaps_HeapStdAlign_Handle, xdc_runtime_Memory_Stats* stats) __attribute__ ((externally_visible));
xdc_Ptr xdcruntime_heaps_HeapStdAlign_alloc__E(xdcruntime_heaps_HeapStdAlign_Handle, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb) __attribute__ ((externally_visible));
xdc_Bool xdcruntime_heaps_HeapStdAlign_isBlocking__E(xdcruntime_heaps_HeapStdAlign_Handle) __attribute__ ((externally_visible));

/*
 * ======== INITIALIZATION ENTRY POINT ========
 */

extern int __xdc__init(void);
#ifdef __GNUC__
#if __GNUC__ >= 4
    __attribute__ ((externally_visible))
#endif
#endif
__FAR__ int (* volatile __xdc__init__addr)(void) = &__xdc__init;


/*
 * ======== PROGRAM GLOBALS ========
 */

/*
 *  @(#) linuxdist; 1, 0, 0,1; 2-15-2012 07:46:11; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

