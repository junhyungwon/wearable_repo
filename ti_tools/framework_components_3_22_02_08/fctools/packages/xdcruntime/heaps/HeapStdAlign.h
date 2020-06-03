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

/*
 * ======== GENERATED SECTIONS ========
 *     
 *     PROLOGUE
 *     INCLUDES
 *     
 *     INTERNAL DEFINITIONS
 *     MODULE-WIDE CONFIGS
 *     PER-INSTANCE TYPES
 *     VIRTUAL FUNCTIONS
 *     FUNCTION DECLARATIONS
 *     FUNCTION SELECTORS
 *     CONVERTORS
 *     SYSTEM FUNCTIONS
 *     
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef xdcruntime_heaps_HeapStdAlign__include
#define xdcruntime_heaps_HeapStdAlign__include

#ifndef __nested__
#define __nested__
#define xdcruntime_heaps_HeapStdAlign__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define xdcruntime_heaps_HeapStdAlign___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/IInstance.h>
#include <xdcruntime/heaps/package/package.defs.h>

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Error.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__xdcruntime_heaps_HeapStdAlign_Module__diagsEnabled;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__diagsEnabled xdcruntime_heaps_HeapStdAlign_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__xdcruntime_heaps_HeapStdAlign_Module__diagsIncluded;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__diagsIncluded xdcruntime_heaps_HeapStdAlign_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__xdcruntime_heaps_HeapStdAlign_Module__diagsMask;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__diagsMask xdcruntime_heaps_HeapStdAlign_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__xdcruntime_heaps_HeapStdAlign_Module__gateObj;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__gateObj xdcruntime_heaps_HeapStdAlign_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__xdcruntime_heaps_HeapStdAlign_Module__gatePrms;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__gatePrms xdcruntime_heaps_HeapStdAlign_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__xdcruntime_heaps_HeapStdAlign_Module__id;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__id xdcruntime_heaps_HeapStdAlign_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__xdcruntime_heaps_HeapStdAlign_Module__loggerDefined;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerDefined xdcruntime_heaps_HeapStdAlign_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__xdcruntime_heaps_HeapStdAlign_Module__loggerObj;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerObj xdcruntime_heaps_HeapStdAlign_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn0;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn0 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn1;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn1 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn2;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn2 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn4;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn4 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn8;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__loggerFxn8 xdcruntime_heaps_HeapStdAlign_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__xdcruntime_heaps_HeapStdAlign_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Module__startupDoneFxn xdcruntime_heaps_HeapStdAlign_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__xdcruntime_heaps_HeapStdAlign_Object__count;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Object__count xdcruntime_heaps_HeapStdAlign_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__xdcruntime_heaps_HeapStdAlign_Object__heap;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Object__heap xdcruntime_heaps_HeapStdAlign_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__xdcruntime_heaps_HeapStdAlign_Object__sizeof;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Object__sizeof xdcruntime_heaps_HeapStdAlign_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__xdcruntime_heaps_HeapStdAlign_Object__table;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_Object__table xdcruntime_heaps_HeapStdAlign_Object__table__C;

/* E_noRTSMemory */
#define xdcruntime_heaps_HeapStdAlign_E_noRTSMemory (xdcruntime_heaps_HeapStdAlign_E_noRTSMemory__C)
typedef xdc_runtime_Error_Id CT__xdcruntime_heaps_HeapStdAlign_E_noRTSMemory;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_E_noRTSMemory xdcruntime_heaps_HeapStdAlign_E_noRTSMemory__C;

/* A_zeroSize */
#define xdcruntime_heaps_HeapStdAlign_A_zeroSize (xdcruntime_heaps_HeapStdAlign_A_zeroSize__C)
typedef xdc_runtime_Assert_Id CT__xdcruntime_heaps_HeapStdAlign_A_zeroSize;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_A_zeroSize xdcruntime_heaps_HeapStdAlign_A_zeroSize__C;

/* A_align */
#define xdcruntime_heaps_HeapStdAlign_A_align (xdcruntime_heaps_HeapStdAlign_A_align__C)
typedef xdc_runtime_Assert_Id CT__xdcruntime_heaps_HeapStdAlign_A_align;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_A_align xdcruntime_heaps_HeapStdAlign_A_align__C;

/* A_invalidTotalFreeSize */
#define xdcruntime_heaps_HeapStdAlign_A_invalidTotalFreeSize (xdcruntime_heaps_HeapStdAlign_A_invalidTotalFreeSize__C)
typedef xdc_runtime_Assert_Id CT__xdcruntime_heaps_HeapStdAlign_A_invalidTotalFreeSize;
__extern __FAR__ const CT__xdcruntime_heaps_HeapStdAlign_A_invalidTotalFreeSize xdcruntime_heaps_HeapStdAlign_A_invalidTotalFreeSize__C;


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct xdcruntime_heaps_HeapStdAlign_Params {
    size_t __size;
    const void* __self;
    void* __fxns;
    xdc_runtime_IInstance_Params* instance;
    xdc_runtime_Memory_Size size;
    xdc_runtime_IInstance_Params __iprms;
};

/* Struct */
struct xdcruntime_heaps_HeapStdAlign_Struct {
    const xdcruntime_heaps_HeapStdAlign_Fxns__* __fxns;
    xdc_runtime_Memory_Size __f0;
    xdc_runtime_Memory_Size __f1;
    xdc_runtime_Types_CordAddr __name;
};


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct xdcruntime_heaps_HeapStdAlign_Fxns__ {
    xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_Ptr (*alloc)(xdcruntime_heaps_HeapStdAlign_Handle, xdc_SizeT, xdc_SizeT, xdc_runtime_Error_Block*);
    xdc_Void (*free)(xdcruntime_heaps_HeapStdAlign_Handle, xdc_Ptr, xdc_SizeT);
    xdc_Bool (*isBlocking)(xdcruntime_heaps_HeapStdAlign_Handle);
    xdc_Void (*getStats)(xdcruntime_heaps_HeapStdAlign_Handle, xdc_runtime_Memory_Stats*);
    xdc_runtime_Types_SysFxns2 __sfxns;
};

/* Module__FXNS__C */
__extern const xdcruntime_heaps_HeapStdAlign_Fxns__ xdcruntime_heaps_HeapStdAlign_Module__FXNS__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define xdcruntime_heaps_HeapStdAlign_Module_startup( state ) -1

/* Instance_init__F */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Instance_init__F, "xdcruntime_heaps_HeapStdAlign_Instance_init")
__extern int xdcruntime_heaps_HeapStdAlign_Instance_init__F( xdcruntime_heaps_HeapStdAlign_Object*, const xdcruntime_heaps_HeapStdAlign_Params*, xdc_runtime_Error_Block* );

/* Instance_init__R */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Instance_init__R, "xdcruntime_heaps_HeapStdAlign_Instance_init")
__extern int xdcruntime_heaps_HeapStdAlign_Instance_init__R( xdcruntime_heaps_HeapStdAlign_Object*, const xdcruntime_heaps_HeapStdAlign_Params*, xdc_runtime_Error_Block* );

/* Handle__label__S */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Handle__label__S, "xdcruntime_heaps_HeapStdAlign_Handle__label")
__extern xdc_runtime_Types_Label* xdcruntime_heaps_HeapStdAlign_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab );

/* Module__startupDone__S */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Module__startupDone__S, "xdcruntime_heaps_HeapStdAlign_Module__startupDone")
__extern xdc_Bool xdcruntime_heaps_HeapStdAlign_Module__startupDone__S( void );

/* Object__create__S */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Object__create__S, "xdcruntime_heaps_HeapStdAlign_Object__create")
__extern xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__create__S( xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb );

/* Object__delete__S */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Object__delete__S, "xdcruntime_heaps_HeapStdAlign_Object__delete")
__extern xdc_Void xdcruntime_heaps_HeapStdAlign_Object__delete__S( xdc_Ptr instp );

/* Object__destruct__S */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Object__destruct__S, "xdcruntime_heaps_HeapStdAlign_Object__destruct")
__extern xdc_Void xdcruntime_heaps_HeapStdAlign_Object__destruct__S( xdc_Ptr objp );

/* Object__get__S */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Object__get__S, "xdcruntime_heaps_HeapStdAlign_Object__get")
__extern xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__get__S( xdc_Ptr oarr, xdc_Int i );

/* Object__first__S */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Object__first__S, "xdcruntime_heaps_HeapStdAlign_Object__first")
__extern xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__first__S( void );

/* Object__next__S */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Object__next__S, "xdcruntime_heaps_HeapStdAlign_Object__next")
__extern xdc_Ptr xdcruntime_heaps_HeapStdAlign_Object__next__S( xdc_Ptr obj );

/* Params__init__S */
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_Params__init__S, "xdcruntime_heaps_HeapStdAlign_Params__init")
__extern xdc_Void xdcruntime_heaps_HeapStdAlign_Params__init__S( xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz );

/* free__E */
#define xdcruntime_heaps_HeapStdAlign_free xdcruntime_heaps_HeapStdAlign_free__E
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_free__E, "xdcruntime_heaps_HeapStdAlign_free")
__extern xdc_Void xdcruntime_heaps_HeapStdAlign_free__E( xdcruntime_heaps_HeapStdAlign_Handle __inst, xdc_Ptr block, xdc_SizeT size );
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_free__F, "xdcruntime_heaps_HeapStdAlign_free")
__extern xdc_Void xdcruntime_heaps_HeapStdAlign_free__F( xdcruntime_heaps_HeapStdAlign_Object* __inst, xdc_Ptr block, xdc_SizeT size );
__extern xdc_Void xdcruntime_heaps_HeapStdAlign_free__R( xdcruntime_heaps_HeapStdAlign_Handle __inst, xdc_Ptr block, xdc_SizeT size );

/* getStats__E */
#define xdcruntime_heaps_HeapStdAlign_getStats xdcruntime_heaps_HeapStdAlign_getStats__E
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_getStats__E, "xdcruntime_heaps_HeapStdAlign_getStats")
__extern xdc_Void xdcruntime_heaps_HeapStdAlign_getStats__E( xdcruntime_heaps_HeapStdAlign_Handle __inst, xdc_runtime_Memory_Stats* stats );
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_getStats__F, "xdcruntime_heaps_HeapStdAlign_getStats")
__extern xdc_Void xdcruntime_heaps_HeapStdAlign_getStats__F( xdcruntime_heaps_HeapStdAlign_Object* __inst, xdc_runtime_Memory_Stats* stats );
__extern xdc_Void xdcruntime_heaps_HeapStdAlign_getStats__R( xdcruntime_heaps_HeapStdAlign_Handle __inst, xdc_runtime_Memory_Stats* stats );

/* alloc__E */
#define xdcruntime_heaps_HeapStdAlign_alloc xdcruntime_heaps_HeapStdAlign_alloc__E
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_alloc__E, "xdcruntime_heaps_HeapStdAlign_alloc")
__extern xdc_Ptr xdcruntime_heaps_HeapStdAlign_alloc__E( xdcruntime_heaps_HeapStdAlign_Handle __inst, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb );
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_alloc__F, "xdcruntime_heaps_HeapStdAlign_alloc")
__extern xdc_Ptr xdcruntime_heaps_HeapStdAlign_alloc__F( xdcruntime_heaps_HeapStdAlign_Object* __inst, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb );
__extern xdc_Ptr xdcruntime_heaps_HeapStdAlign_alloc__R( xdcruntime_heaps_HeapStdAlign_Handle __inst, xdc_SizeT size, xdc_SizeT align, xdc_runtime_Error_Block* eb );

/* isBlocking__E */
#define xdcruntime_heaps_HeapStdAlign_isBlocking xdcruntime_heaps_HeapStdAlign_isBlocking__E
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_isBlocking__E, "xdcruntime_heaps_HeapStdAlign_isBlocking")
__extern xdc_Bool xdcruntime_heaps_HeapStdAlign_isBlocking__E( xdcruntime_heaps_HeapStdAlign_Handle __inst );
xdc__CODESECT(xdcruntime_heaps_HeapStdAlign_isBlocking__F, "xdcruntime_heaps_HeapStdAlign_isBlocking")
__extern xdc_Bool xdcruntime_heaps_HeapStdAlign_isBlocking__F( xdcruntime_heaps_HeapStdAlign_Object* __inst );
__extern xdc_Bool xdcruntime_heaps_HeapStdAlign_isBlocking__R( xdcruntime_heaps_HeapStdAlign_Handle __inst );


/*
 * ======== FUNCTION SELECTORS ========
 */

/* free_{FxnT,fxnP} */
typedef xdc_Void (*xdcruntime_heaps_HeapStdAlign_free_FxnT)(void*, xdc_Ptr, xdc_SizeT);
static inline xdcruntime_heaps_HeapStdAlign_free_FxnT xdcruntime_heaps_HeapStdAlign_free_fxnP( void )
{
    return (xdcruntime_heaps_HeapStdAlign_free_FxnT)xdcruntime_heaps_HeapStdAlign_free; 
}

/* getStats_{FxnT,fxnP} */
typedef xdc_Void (*xdcruntime_heaps_HeapStdAlign_getStats_FxnT)(void*, xdc_runtime_Memory_Stats*);
static inline xdcruntime_heaps_HeapStdAlign_getStats_FxnT xdcruntime_heaps_HeapStdAlign_getStats_fxnP( void )
{
    return (xdcruntime_heaps_HeapStdAlign_getStats_FxnT)xdcruntime_heaps_HeapStdAlign_getStats; 
}

/* alloc_{FxnT,fxnP} */
typedef xdc_Ptr (*xdcruntime_heaps_HeapStdAlign_alloc_FxnT)(void*, xdc_SizeT, xdc_SizeT, xdc_runtime_Error_Block*);
static inline xdcruntime_heaps_HeapStdAlign_alloc_FxnT xdcruntime_heaps_HeapStdAlign_alloc_fxnP( void )
{
    return (xdcruntime_heaps_HeapStdAlign_alloc_FxnT)xdcruntime_heaps_HeapStdAlign_alloc; 
}

/* isBlocking_{FxnT,fxnP} */
typedef xdc_Bool (*xdcruntime_heaps_HeapStdAlign_isBlocking_FxnT)(void*);
static inline xdcruntime_heaps_HeapStdAlign_isBlocking_FxnT xdcruntime_heaps_HeapStdAlign_isBlocking_fxnP( void )
{
    return (xdcruntime_heaps_HeapStdAlign_isBlocking_FxnT)xdcruntime_heaps_HeapStdAlign_isBlocking; 
}


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline xdc_runtime_IHeap_Module xdcruntime_heaps_HeapStdAlign_Module_upCast( void )
{
    return (xdc_runtime_IHeap_Module)&xdcruntime_heaps_HeapStdAlign_Module__FXNS__C;
}

/* Module_to_xdc_runtime_IHeap */
#define xdcruntime_heaps_HeapStdAlign_Module_to_xdc_runtime_IHeap xdcruntime_heaps_HeapStdAlign_Module_upCast

/* Handle_upCast */
static inline xdc_runtime_IHeap_Handle xdcruntime_heaps_HeapStdAlign_Handle_upCast( xdcruntime_heaps_HeapStdAlign_Handle i )
{
    return (xdc_runtime_IHeap_Handle)i;
}

/* Handle_to_xdc_runtime_IHeap */
#define xdcruntime_heaps_HeapStdAlign_Handle_to_xdc_runtime_IHeap xdcruntime_heaps_HeapStdAlign_Handle_upCast

/* Handle_downCast */
static inline xdcruntime_heaps_HeapStdAlign_Handle xdcruntime_heaps_HeapStdAlign_Handle_downCast( xdc_runtime_IHeap_Handle i )
{
    xdc_runtime_IHeap_Handle i2 = (xdc_runtime_IHeap_Handle)i;
    return (void*)i2->__fxns == (void*)&xdcruntime_heaps_HeapStdAlign_Module__FXNS__C ? (xdcruntime_heaps_HeapStdAlign_Handle)i : 0;
}

/* Handle_from_xdc_runtime_IHeap */
#define xdcruntime_heaps_HeapStdAlign_Handle_from_xdc_runtime_IHeap xdcruntime_heaps_HeapStdAlign_Handle_downCast


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define xdcruntime_heaps_HeapStdAlign_Module_startupDone() xdcruntime_heaps_HeapStdAlign_Module__startupDone__S()

/* Object_heap */
#define xdcruntime_heaps_HeapStdAlign_Object_heap() xdcruntime_heaps_HeapStdAlign_Object__heap__C

/* Module_heap */
#define xdcruntime_heaps_HeapStdAlign_Module_heap() xdcruntime_heaps_HeapStdAlign_Object__heap__C

/* Module_id */
static inline CT__xdcruntime_heaps_HeapStdAlign_Module__id xdcruntime_heaps_HeapStdAlign_Module_id( void ) 
{
    return xdcruntime_heaps_HeapStdAlign_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool xdcruntime_heaps_HeapStdAlign_Module_hasMask( void ) 
{
    return xdcruntime_heaps_HeapStdAlign_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 xdcruntime_heaps_HeapStdAlign_Module_getMask( void ) 
{
    return xdcruntime_heaps_HeapStdAlign_Module__diagsMask__C != NULL ? *xdcruntime_heaps_HeapStdAlign_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void xdcruntime_heaps_HeapStdAlign_Module_setMask( xdc_Bits16 mask ) 
{
    if (xdcruntime_heaps_HeapStdAlign_Module__diagsMask__C != NULL) *xdcruntime_heaps_HeapStdAlign_Module__diagsMask__C = mask;
}

/* Params_init */
static inline void xdcruntime_heaps_HeapStdAlign_Params_init( xdcruntime_heaps_HeapStdAlign_Params* prms ) 
{
    if (prms) {
        xdcruntime_heaps_HeapStdAlign_Params__init__S(prms, 0, sizeof(xdcruntime_heaps_HeapStdAlign_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Params_copy */
static inline void xdcruntime_heaps_HeapStdAlign_Params_copy( xdcruntime_heaps_HeapStdAlign_Params* dst, const xdcruntime_heaps_HeapStdAlign_Params* src ) 
{
    if (dst) {
        xdcruntime_heaps_HeapStdAlign_Params__init__S(dst, (xdc_Ptr)src, sizeof(xdcruntime_heaps_HeapStdAlign_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Object_count */
#define xdcruntime_heaps_HeapStdAlign_Object_count() xdcruntime_heaps_HeapStdAlign_Object__count__C

/* Object_sizeof */
#define xdcruntime_heaps_HeapStdAlign_Object_sizeof() xdcruntime_heaps_HeapStdAlign_Object__sizeof__C

/* Object_get */
static inline xdcruntime_heaps_HeapStdAlign_Handle xdcruntime_heaps_HeapStdAlign_Object_get( xdcruntime_heaps_HeapStdAlign_Instance_State* oarr, int i ) 
{
    return (xdcruntime_heaps_HeapStdAlign_Handle)xdcruntime_heaps_HeapStdAlign_Object__get__S(oarr, i);
}

/* Object_first */
static inline xdcruntime_heaps_HeapStdAlign_Handle xdcruntime_heaps_HeapStdAlign_Object_first( void )
{
    return (xdcruntime_heaps_HeapStdAlign_Handle)xdcruntime_heaps_HeapStdAlign_Object__first__S();
}

/* Object_next */
static inline xdcruntime_heaps_HeapStdAlign_Handle xdcruntime_heaps_HeapStdAlign_Object_next( xdcruntime_heaps_HeapStdAlign_Object* obj )
{
    return (xdcruntime_heaps_HeapStdAlign_Handle)xdcruntime_heaps_HeapStdAlign_Object__next__S(obj);
}

/* Handle_label */
static inline xdc_runtime_Types_Label* xdcruntime_heaps_HeapStdAlign_Handle_label( xdcruntime_heaps_HeapStdAlign_Handle inst, xdc_runtime_Types_Label* lab )
{
    return xdcruntime_heaps_HeapStdAlign_Handle__label__S(inst, lab);
}

/* Handle_name */
static inline String xdcruntime_heaps_HeapStdAlign_Handle_name( xdcruntime_heaps_HeapStdAlign_Handle inst )
{
    xdc_runtime_Types_Label lab;
    return xdcruntime_heaps_HeapStdAlign_Handle__label__S(inst, &lab)->iname;
}

/* create */
static inline xdcruntime_heaps_HeapStdAlign_Handle xdcruntime_heaps_HeapStdAlign_create( const xdcruntime_heaps_HeapStdAlign_Params* __prms, xdc_runtime_Error_Block* __eb )
{
    return (xdcruntime_heaps_HeapStdAlign_Handle)xdcruntime_heaps_HeapStdAlign_Object__create__S(0, 0, 0, (const xdc_UChar*)__prms, sizeof(xdcruntime_heaps_HeapStdAlign_Params), __eb);
}

/* construct */
static inline void xdcruntime_heaps_HeapStdAlign_construct( xdcruntime_heaps_HeapStdAlign_Struct* __obj, const xdcruntime_heaps_HeapStdAlign_Params* __prms, xdc_runtime_Error_Block* __eb )
{
    xdcruntime_heaps_HeapStdAlign_Object__create__S(__obj, sizeof (xdcruntime_heaps_HeapStdAlign_Struct), 0, (const xdc_UChar*)__prms, sizeof(xdcruntime_heaps_HeapStdAlign_Params), __eb);
}

/* delete */
static inline void xdcruntime_heaps_HeapStdAlign_delete( xdcruntime_heaps_HeapStdAlign_Handle* instp )
{
    xdcruntime_heaps_HeapStdAlign_Object__delete__S(instp);
}

/* destruct */
static inline void xdcruntime_heaps_HeapStdAlign_destruct( xdcruntime_heaps_HeapStdAlign_Struct* obj )
{
    xdcruntime_heaps_HeapStdAlign_Object__destruct__S(obj);
}

/* handle */
static inline xdcruntime_heaps_HeapStdAlign_Handle xdcruntime_heaps_HeapStdAlign_handle( xdcruntime_heaps_HeapStdAlign_Struct* str )
{
    return (xdcruntime_heaps_HeapStdAlign_Handle)str;
}

/* struct */
static inline xdcruntime_heaps_HeapStdAlign_Struct* xdcruntime_heaps_HeapStdAlign_struct( xdcruntime_heaps_HeapStdAlign_Handle inst )
{
    return (xdcruntime_heaps_HeapStdAlign_Struct*)inst;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef xdcruntime_heaps_HeapStdAlign__top__
#undef __nested__
#endif

#endif /* xdcruntime_heaps_HeapStdAlign__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(xdcruntime_heaps_HeapStdAlign__internalaccess))

#ifndef xdcruntime_heaps_HeapStdAlign__include_state
#define xdcruntime_heaps_HeapStdAlign__include_state

/* Module_State */
struct xdcruntime_heaps_HeapStdAlign_Module_State {
    xdc_runtime_Memory_Size remainRTSSize;
};

/* Module__state__V */
extern struct xdcruntime_heaps_HeapStdAlign_Module_State__ xdcruntime_heaps_HeapStdAlign_Module__state__V;

/* Object */
struct xdcruntime_heaps_HeapStdAlign_Object {
    const xdcruntime_heaps_HeapStdAlign_Fxns__* __fxns;
    xdc_runtime_Memory_Size remainSize;
    xdc_runtime_Memory_Size startSize;
};

#endif /* xdcruntime_heaps_HeapStdAlign__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(xdcruntime_heaps_HeapStdAlign__nolocalnames)

#ifndef xdcruntime_heaps_HeapStdAlign__localnames__done
#define xdcruntime_heaps_HeapStdAlign__localnames__done

/* module prefix */
#define HeapStdAlign_Instance xdcruntime_heaps_HeapStdAlign_Instance
#define HeapStdAlign_Handle xdcruntime_heaps_HeapStdAlign_Handle
#define HeapStdAlign_Module xdcruntime_heaps_HeapStdAlign_Module
#define HeapStdAlign_Object xdcruntime_heaps_HeapStdAlign_Object
#define HeapStdAlign_Struct xdcruntime_heaps_HeapStdAlign_Struct
#define HeapStdAlign_Module_State xdcruntime_heaps_HeapStdAlign_Module_State
#define HeapStdAlign_Instance_State xdcruntime_heaps_HeapStdAlign_Instance_State
#define HeapStdAlign_E_noRTSMemory xdcruntime_heaps_HeapStdAlign_E_noRTSMemory
#define HeapStdAlign_A_zeroSize xdcruntime_heaps_HeapStdAlign_A_zeroSize
#define HeapStdAlign_A_align xdcruntime_heaps_HeapStdAlign_A_align
#define HeapStdAlign_A_invalidTotalFreeSize xdcruntime_heaps_HeapStdAlign_A_invalidTotalFreeSize
#define HeapStdAlign_Params xdcruntime_heaps_HeapStdAlign_Params
#define HeapStdAlign_free xdcruntime_heaps_HeapStdAlign_free
#define HeapStdAlign_free_fxnP xdcruntime_heaps_HeapStdAlign_free_fxnP
#define HeapStdAlign_free_FxnT xdcruntime_heaps_HeapStdAlign_free_FxnT
#define HeapStdAlign_getStats xdcruntime_heaps_HeapStdAlign_getStats
#define HeapStdAlign_getStats_fxnP xdcruntime_heaps_HeapStdAlign_getStats_fxnP
#define HeapStdAlign_getStats_FxnT xdcruntime_heaps_HeapStdAlign_getStats_FxnT
#define HeapStdAlign_alloc xdcruntime_heaps_HeapStdAlign_alloc
#define HeapStdAlign_alloc_fxnP xdcruntime_heaps_HeapStdAlign_alloc_fxnP
#define HeapStdAlign_alloc_FxnT xdcruntime_heaps_HeapStdAlign_alloc_FxnT
#define HeapStdAlign_isBlocking xdcruntime_heaps_HeapStdAlign_isBlocking
#define HeapStdAlign_isBlocking_fxnP xdcruntime_heaps_HeapStdAlign_isBlocking_fxnP
#define HeapStdAlign_isBlocking_FxnT xdcruntime_heaps_HeapStdAlign_isBlocking_FxnT
#define HeapStdAlign_Module_name xdcruntime_heaps_HeapStdAlign_Module_name
#define HeapStdAlign_Module_id xdcruntime_heaps_HeapStdAlign_Module_id
#define HeapStdAlign_Module_startup xdcruntime_heaps_HeapStdAlign_Module_startup
#define HeapStdAlign_Module_startupDone xdcruntime_heaps_HeapStdAlign_Module_startupDone
#define HeapStdAlign_Module_hasMask xdcruntime_heaps_HeapStdAlign_Module_hasMask
#define HeapStdAlign_Module_getMask xdcruntime_heaps_HeapStdAlign_Module_getMask
#define HeapStdAlign_Module_setMask xdcruntime_heaps_HeapStdAlign_Module_setMask
#define HeapStdAlign_Object_heap xdcruntime_heaps_HeapStdAlign_Object_heap
#define HeapStdAlign_Module_heap xdcruntime_heaps_HeapStdAlign_Module_heap
#define HeapStdAlign_construct xdcruntime_heaps_HeapStdAlign_construct
#define HeapStdAlign_create xdcruntime_heaps_HeapStdAlign_create
#define HeapStdAlign_handle xdcruntime_heaps_HeapStdAlign_handle
#define HeapStdAlign_struct xdcruntime_heaps_HeapStdAlign_struct
#define HeapStdAlign_Handle_label xdcruntime_heaps_HeapStdAlign_Handle_label
#define HeapStdAlign_Handle_name xdcruntime_heaps_HeapStdAlign_Handle_name
#define HeapStdAlign_Instance_init xdcruntime_heaps_HeapStdAlign_Instance_init
#define HeapStdAlign_Object_count xdcruntime_heaps_HeapStdAlign_Object_count
#define HeapStdAlign_Object_get xdcruntime_heaps_HeapStdAlign_Object_get
#define HeapStdAlign_Object_first xdcruntime_heaps_HeapStdAlign_Object_first
#define HeapStdAlign_Object_next xdcruntime_heaps_HeapStdAlign_Object_next
#define HeapStdAlign_Object_sizeof xdcruntime_heaps_HeapStdAlign_Object_sizeof
#define HeapStdAlign_Params_copy xdcruntime_heaps_HeapStdAlign_Params_copy
#define HeapStdAlign_Params_init xdcruntime_heaps_HeapStdAlign_Params_init
#define HeapStdAlign_delete xdcruntime_heaps_HeapStdAlign_delete
#define HeapStdAlign_destruct xdcruntime_heaps_HeapStdAlign_destruct
#define HeapStdAlign_Module_upCast xdcruntime_heaps_HeapStdAlign_Module_upCast
#define HeapStdAlign_Module_to_xdc_runtime_IHeap xdcruntime_heaps_HeapStdAlign_Module_to_xdc_runtime_IHeap
#define HeapStdAlign_Handle_upCast xdcruntime_heaps_HeapStdAlign_Handle_upCast
#define HeapStdAlign_Handle_to_xdc_runtime_IHeap xdcruntime_heaps_HeapStdAlign_Handle_to_xdc_runtime_IHeap
#define HeapStdAlign_Handle_downCast xdcruntime_heaps_HeapStdAlign_Handle_downCast
#define HeapStdAlign_Handle_from_xdc_runtime_IHeap xdcruntime_heaps_HeapStdAlign_Handle_from_xdc_runtime_IHeap

#endif /* xdcruntime_heaps_HeapStdAlign__localnames__done */
#endif
/*
 *  @(#) xdcruntime.heaps; 1, 0, 0,1; 2-15-2012 07:45:06; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

