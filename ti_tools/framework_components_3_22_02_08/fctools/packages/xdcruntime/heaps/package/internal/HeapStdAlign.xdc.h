/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-y21
 */

#ifndef xdcruntime_heaps_HeapStdAlign__INTERNAL__
#define xdcruntime_heaps_HeapStdAlign__INTERNAL__

#ifndef xdcruntime_heaps_HeapStdAlign__internalaccess
#define xdcruntime_heaps_HeapStdAlign__internalaccess
#endif

#include <xdcruntime/heaps/HeapStdAlign.h>

#undef xdc_FILE__
#ifndef xdc_FILE
#define xdc_FILE__ NULL
#else
#define xdc_FILE__ xdc_FILE
#endif

/* free */
#undef xdcruntime_heaps_HeapStdAlign_free
#define xdcruntime_heaps_HeapStdAlign_free xdcruntime_heaps_HeapStdAlign_free__F

/* getStats */
#undef xdcruntime_heaps_HeapStdAlign_getStats
#define xdcruntime_heaps_HeapStdAlign_getStats xdcruntime_heaps_HeapStdAlign_getStats__F

/* alloc */
#undef xdcruntime_heaps_HeapStdAlign_alloc
#define xdcruntime_heaps_HeapStdAlign_alloc xdcruntime_heaps_HeapStdAlign_alloc__F

/* isBlocking */
#undef xdcruntime_heaps_HeapStdAlign_isBlocking
#define xdcruntime_heaps_HeapStdAlign_isBlocking xdcruntime_heaps_HeapStdAlign_isBlocking__F

/* Module_startup */
#undef xdcruntime_heaps_HeapStdAlign_Module_startup
#define xdcruntime_heaps_HeapStdAlign_Module_startup xdcruntime_heaps_HeapStdAlign_Module_startup__F

/* Instance_init */
#undef xdcruntime_heaps_HeapStdAlign_Instance_init
#define xdcruntime_heaps_HeapStdAlign_Instance_init xdcruntime_heaps_HeapStdAlign_Instance_init__F

/* Instance_finalize */
#undef xdcruntime_heaps_HeapStdAlign_Instance_finalize
#define xdcruntime_heaps_HeapStdAlign_Instance_finalize xdcruntime_heaps_HeapStdAlign_Instance_finalize__F

/* module */
#define HeapStdAlign_module ((xdcruntime_heaps_HeapStdAlign_Module_State *)(xdc__MODOBJADDR__(xdcruntime_heaps_HeapStdAlign_Module__state__V)))
#if !defined(__cplusplus) || !defined(xdcruntime_heaps_HeapStdAlign__cplusplus)
#define module ((xdcruntime_heaps_HeapStdAlign_Module_State *)(xdc__MODOBJADDR__(xdcruntime_heaps_HeapStdAlign_Module__state__V)))
#endif
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

/* Object__sizingError */
#line 1 "Error_inconsistent_object_size_in_xdcruntime.heaps.HeapStdAlign"
typedef char xdcruntime_heaps_HeapStdAlign_Object__sizingError[sizeof(xdcruntime_heaps_HeapStdAlign_Object) > sizeof(xdcruntime_heaps_HeapStdAlign_Struct) ? -1 : 1];


#endif /* xdcruntime_heaps_HeapStdAlign__INTERNAL____ */
