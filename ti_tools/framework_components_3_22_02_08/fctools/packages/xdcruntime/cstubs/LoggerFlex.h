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

#ifndef xdcruntime_cstubs_LoggerFlex__include
#define xdcruntime_cstubs_LoggerFlex__include

#ifndef __nested__
#define __nested__
#define xdcruntime_cstubs_LoggerFlex__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define xdcruntime_cstubs_LoggerFlex___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/IInstance.h>
#include <xdcruntime/cstubs/package/package.defs.h>

#include <xdc/runtime/ILogger.h>
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
typedef xdc_Bits32 CT__xdcruntime_cstubs_LoggerFlex_Module__diagsEnabled;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__diagsEnabled xdcruntime_cstubs_LoggerFlex_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__xdcruntime_cstubs_LoggerFlex_Module__diagsIncluded;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__diagsIncluded xdcruntime_cstubs_LoggerFlex_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__xdcruntime_cstubs_LoggerFlex_Module__diagsMask;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__diagsMask xdcruntime_cstubs_LoggerFlex_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__xdcruntime_cstubs_LoggerFlex_Module__gateObj;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__gateObj xdcruntime_cstubs_LoggerFlex_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__xdcruntime_cstubs_LoggerFlex_Module__gatePrms;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__gatePrms xdcruntime_cstubs_LoggerFlex_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__xdcruntime_cstubs_LoggerFlex_Module__id;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__id xdcruntime_cstubs_LoggerFlex_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__xdcruntime_cstubs_LoggerFlex_Module__loggerDefined;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerDefined xdcruntime_cstubs_LoggerFlex_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__xdcruntime_cstubs_LoggerFlex_Module__loggerObj;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerObj xdcruntime_cstubs_LoggerFlex_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn0;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn0 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn1;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn1 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn2;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn2 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn4;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn4 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn8;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__loggerFxn8 xdcruntime_cstubs_LoggerFlex_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__xdcruntime_cstubs_LoggerFlex_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Module__startupDoneFxn xdcruntime_cstubs_LoggerFlex_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__xdcruntime_cstubs_LoggerFlex_Object__count;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Object__count xdcruntime_cstubs_LoggerFlex_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__xdcruntime_cstubs_LoggerFlex_Object__heap;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Object__heap xdcruntime_cstubs_LoggerFlex_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__xdcruntime_cstubs_LoggerFlex_Object__sizeof;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Object__sizeof xdcruntime_cstubs_LoggerFlex_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__xdcruntime_cstubs_LoggerFlex_Object__table;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_Object__table xdcruntime_cstubs_LoggerFlex_Object__table__C;

/* E_atexit */
#define xdcruntime_cstubs_LoggerFlex_E_atexit (xdcruntime_cstubs_LoggerFlex_E_atexit__C)
typedef xdc_runtime_Error_Id CT__xdcruntime_cstubs_LoggerFlex_E_atexit;
__extern __FAR__ const CT__xdcruntime_cstubs_LoggerFlex_E_atexit xdcruntime_cstubs_LoggerFlex_E_atexit__C;


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct xdcruntime_cstubs_LoggerFlex_Params {
    size_t __size;
    const void* __self;
    void* __fxns;
    xdc_runtime_IInstance_Params* instance;
    xdc_runtime_IInstance_Params __iprms;
};

/* Struct */
struct xdcruntime_cstubs_LoggerFlex_Struct {
    const xdcruntime_cstubs_LoggerFlex_Fxns__* __fxns;
    xdc_runtime_Types_CordAddr __name;
};


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct xdcruntime_cstubs_LoggerFlex_Fxns__ {
    xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_Bool (*enable)(xdcruntime_cstubs_LoggerFlex_Handle);
    xdc_Bool (*disable)(xdcruntime_cstubs_LoggerFlex_Handle);
    xdc_Void (*write0)(xdcruntime_cstubs_LoggerFlex_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId);
    xdc_Void (*write1)(xdcruntime_cstubs_LoggerFlex_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg);
    xdc_Void (*write2)(xdcruntime_cstubs_LoggerFlex_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg);
    xdc_Void (*write4)(xdcruntime_cstubs_LoggerFlex_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
    xdc_Void (*write8)(xdcruntime_cstubs_LoggerFlex_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
    xdc_runtime_Types_SysFxns2 __sfxns;
};

/* Module__FXNS__C */
__extern const xdcruntime_cstubs_LoggerFlex_Fxns__ xdcruntime_cstubs_LoggerFlex_Module__FXNS__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define xdcruntime_cstubs_LoggerFlex_Module_startup xdcruntime_cstubs_LoggerFlex_Module_startup__E
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Module_startup__E, "xdcruntime_cstubs_LoggerFlex_Module_startup")
__extern xdc_Int xdcruntime_cstubs_LoggerFlex_Module_startup__E( xdc_Int state );
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Module_startup__F, "xdcruntime_cstubs_LoggerFlex_Module_startup")
__extern xdc_Int xdcruntime_cstubs_LoggerFlex_Module_startup__F( xdc_Int state );
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Module_startup__R, "xdcruntime_cstubs_LoggerFlex_Module_startup")
__extern xdc_Int xdcruntime_cstubs_LoggerFlex_Module_startup__R( xdc_Int state );

/* Instance_init__F */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Instance_init__F, "xdcruntime_cstubs_LoggerFlex_Instance_init")
__extern void xdcruntime_cstubs_LoggerFlex_Instance_init__F( xdcruntime_cstubs_LoggerFlex_Object*, const xdcruntime_cstubs_LoggerFlex_Params* );

/* Instance_init__R */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Instance_init__R, "xdcruntime_cstubs_LoggerFlex_Instance_init")
__extern void xdcruntime_cstubs_LoggerFlex_Instance_init__R( xdcruntime_cstubs_LoggerFlex_Object*, const xdcruntime_cstubs_LoggerFlex_Params* );

/* Handle__label__S */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Handle__label__S, "xdcruntime_cstubs_LoggerFlex_Handle__label")
__extern xdc_runtime_Types_Label* xdcruntime_cstubs_LoggerFlex_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab );

/* Module__startupDone__S */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Module__startupDone__S, "xdcruntime_cstubs_LoggerFlex_Module__startupDone")
__extern xdc_Bool xdcruntime_cstubs_LoggerFlex_Module__startupDone__S( void );

/* Object__create__S */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Object__create__S, "xdcruntime_cstubs_LoggerFlex_Object__create")
__extern xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__create__S( xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb );

/* Object__delete__S */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Object__delete__S, "xdcruntime_cstubs_LoggerFlex_Object__delete")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_Object__delete__S( xdc_Ptr instp );

/* Object__destruct__S */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Object__destruct__S, "xdcruntime_cstubs_LoggerFlex_Object__destruct")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_Object__destruct__S( xdc_Ptr objp );

/* Object__get__S */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Object__get__S, "xdcruntime_cstubs_LoggerFlex_Object__get")
__extern xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__get__S( xdc_Ptr oarr, xdc_Int i );

/* Object__first__S */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Object__first__S, "xdcruntime_cstubs_LoggerFlex_Object__first")
__extern xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__first__S( void );

/* Object__next__S */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Object__next__S, "xdcruntime_cstubs_LoggerFlex_Object__next")
__extern xdc_Ptr xdcruntime_cstubs_LoggerFlex_Object__next__S( xdc_Ptr obj );

/* Params__init__S */
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_Params__init__S, "xdcruntime_cstubs_LoggerFlex_Params__init")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_Params__init__S( xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz );

/* enable__E */
#define xdcruntime_cstubs_LoggerFlex_enable xdcruntime_cstubs_LoggerFlex_enable__E
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_enable__E, "xdcruntime_cstubs_LoggerFlex_enable")
__extern xdc_Bool xdcruntime_cstubs_LoggerFlex_enable__E( xdcruntime_cstubs_LoggerFlex_Handle __inst );
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_enable__F, "xdcruntime_cstubs_LoggerFlex_enable")
__extern xdc_Bool xdcruntime_cstubs_LoggerFlex_enable__F( xdcruntime_cstubs_LoggerFlex_Object* __inst );
__extern xdc_Bool xdcruntime_cstubs_LoggerFlex_enable__R( xdcruntime_cstubs_LoggerFlex_Handle __inst );

/* disable__E */
#define xdcruntime_cstubs_LoggerFlex_disable xdcruntime_cstubs_LoggerFlex_disable__E
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_disable__E, "xdcruntime_cstubs_LoggerFlex_disable")
__extern xdc_Bool xdcruntime_cstubs_LoggerFlex_disable__E( xdcruntime_cstubs_LoggerFlex_Handle __inst );
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_disable__F, "xdcruntime_cstubs_LoggerFlex_disable")
__extern xdc_Bool xdcruntime_cstubs_LoggerFlex_disable__F( xdcruntime_cstubs_LoggerFlex_Object* __inst );
__extern xdc_Bool xdcruntime_cstubs_LoggerFlex_disable__R( xdcruntime_cstubs_LoggerFlex_Handle __inst );

/* write0__E */
#define xdcruntime_cstubs_LoggerFlex_write0 xdcruntime_cstubs_LoggerFlex_write0__E
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_write0__E, "xdcruntime_cstubs_LoggerFlex_write0")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write0__E( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid );
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_write0__F, "xdcruntime_cstubs_LoggerFlex_write0")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write0__F( xdcruntime_cstubs_LoggerFlex_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid );
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write0__R( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid );

/* write1__E */
#define xdcruntime_cstubs_LoggerFlex_write1 xdcruntime_cstubs_LoggerFlex_write1__E
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_write1__E, "xdcruntime_cstubs_LoggerFlex_write1")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write1__E( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 );
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_write1__F, "xdcruntime_cstubs_LoggerFlex_write1")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write1__F( xdcruntime_cstubs_LoggerFlex_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 );
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write1__R( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 );

/* write2__E */
#define xdcruntime_cstubs_LoggerFlex_write2 xdcruntime_cstubs_LoggerFlex_write2__E
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_write2__E, "xdcruntime_cstubs_LoggerFlex_write2")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write2__E( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 );
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_write2__F, "xdcruntime_cstubs_LoggerFlex_write2")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write2__F( xdcruntime_cstubs_LoggerFlex_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 );
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write2__R( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 );

/* write4__E */
#define xdcruntime_cstubs_LoggerFlex_write4 xdcruntime_cstubs_LoggerFlex_write4__E
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_write4__E, "xdcruntime_cstubs_LoggerFlex_write4")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write4__E( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 );
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_write4__F, "xdcruntime_cstubs_LoggerFlex_write4")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write4__F( xdcruntime_cstubs_LoggerFlex_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 );
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write4__R( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 );

/* write8__E */
#define xdcruntime_cstubs_LoggerFlex_write8 xdcruntime_cstubs_LoggerFlex_write8__E
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_write8__E, "xdcruntime_cstubs_LoggerFlex_write8")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write8__E( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 );
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_write8__F, "xdcruntime_cstubs_LoggerFlex_write8")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write8__F( xdcruntime_cstubs_LoggerFlex_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 );
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_write8__R( xdcruntime_cstubs_LoggerFlex_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 );

/* shutdown__E */
#define xdcruntime_cstubs_LoggerFlex_shutdown xdcruntime_cstubs_LoggerFlex_shutdown__E
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_shutdown__E, "xdcruntime_cstubs_LoggerFlex_shutdown")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_shutdown__E( xdc_Int status );
xdc__CODESECT(xdcruntime_cstubs_LoggerFlex_shutdown__F, "xdcruntime_cstubs_LoggerFlex_shutdown")
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_shutdown__F( xdc_Int status );
__extern xdc_Void xdcruntime_cstubs_LoggerFlex_shutdown__R( xdc_Int status );


/*
 * ======== FUNCTION SELECTORS ========
 */

/* enable_{FxnT,fxnP} */
typedef xdc_Bool (*xdcruntime_cstubs_LoggerFlex_enable_FxnT)(void*);
static inline xdcruntime_cstubs_LoggerFlex_enable_FxnT xdcruntime_cstubs_LoggerFlex_enable_fxnP( void )
{
    return (xdcruntime_cstubs_LoggerFlex_enable_FxnT)xdcruntime_cstubs_LoggerFlex_enable; 
}

/* disable_{FxnT,fxnP} */
typedef xdc_Bool (*xdcruntime_cstubs_LoggerFlex_disable_FxnT)(void*);
static inline xdcruntime_cstubs_LoggerFlex_disable_FxnT xdcruntime_cstubs_LoggerFlex_disable_fxnP( void )
{
    return (xdcruntime_cstubs_LoggerFlex_disable_FxnT)xdcruntime_cstubs_LoggerFlex_disable; 
}

/* write0_{FxnT,fxnP} */
typedef xdc_Void (*xdcruntime_cstubs_LoggerFlex_write0_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId);
static inline xdcruntime_cstubs_LoggerFlex_write0_FxnT xdcruntime_cstubs_LoggerFlex_write0_fxnP( void )
{
    return (xdcruntime_cstubs_LoggerFlex_write0_FxnT)xdcruntime_cstubs_LoggerFlex_write0; 
}

/* write1_{FxnT,fxnP} */
typedef xdc_Void (*xdcruntime_cstubs_LoggerFlex_write1_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg);
static inline xdcruntime_cstubs_LoggerFlex_write1_FxnT xdcruntime_cstubs_LoggerFlex_write1_fxnP( void )
{
    return (xdcruntime_cstubs_LoggerFlex_write1_FxnT)xdcruntime_cstubs_LoggerFlex_write1; 
}

/* write2_{FxnT,fxnP} */
typedef xdc_Void (*xdcruntime_cstubs_LoggerFlex_write2_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg);
static inline xdcruntime_cstubs_LoggerFlex_write2_FxnT xdcruntime_cstubs_LoggerFlex_write2_fxnP( void )
{
    return (xdcruntime_cstubs_LoggerFlex_write2_FxnT)xdcruntime_cstubs_LoggerFlex_write2; 
}

/* write4_{FxnT,fxnP} */
typedef xdc_Void (*xdcruntime_cstubs_LoggerFlex_write4_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
static inline xdcruntime_cstubs_LoggerFlex_write4_FxnT xdcruntime_cstubs_LoggerFlex_write4_fxnP( void )
{
    return (xdcruntime_cstubs_LoggerFlex_write4_FxnT)xdcruntime_cstubs_LoggerFlex_write4; 
}

/* write8_{FxnT,fxnP} */
typedef xdc_Void (*xdcruntime_cstubs_LoggerFlex_write8_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
static inline xdcruntime_cstubs_LoggerFlex_write8_FxnT xdcruntime_cstubs_LoggerFlex_write8_fxnP( void )
{
    return (xdcruntime_cstubs_LoggerFlex_write8_FxnT)xdcruntime_cstubs_LoggerFlex_write8; 
}


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline xdc_runtime_ILogger_Module xdcruntime_cstubs_LoggerFlex_Module_upCast( void )
{
    return (xdc_runtime_ILogger_Module)&xdcruntime_cstubs_LoggerFlex_Module__FXNS__C;
}

/* Module_to_xdc_runtime_ILogger */
#define xdcruntime_cstubs_LoggerFlex_Module_to_xdc_runtime_ILogger xdcruntime_cstubs_LoggerFlex_Module_upCast

/* Handle_upCast */
static inline xdc_runtime_ILogger_Handle xdcruntime_cstubs_LoggerFlex_Handle_upCast( xdcruntime_cstubs_LoggerFlex_Handle i )
{
    return (xdc_runtime_ILogger_Handle)i;
}

/* Handle_to_xdc_runtime_ILogger */
#define xdcruntime_cstubs_LoggerFlex_Handle_to_xdc_runtime_ILogger xdcruntime_cstubs_LoggerFlex_Handle_upCast

/* Handle_downCast */
static inline xdcruntime_cstubs_LoggerFlex_Handle xdcruntime_cstubs_LoggerFlex_Handle_downCast( xdc_runtime_ILogger_Handle i )
{
    xdc_runtime_ILogger_Handle i2 = (xdc_runtime_ILogger_Handle)i;
    return (void*)i2->__fxns == (void*)&xdcruntime_cstubs_LoggerFlex_Module__FXNS__C ? (xdcruntime_cstubs_LoggerFlex_Handle)i : 0;
}

/* Handle_from_xdc_runtime_ILogger */
#define xdcruntime_cstubs_LoggerFlex_Handle_from_xdc_runtime_ILogger xdcruntime_cstubs_LoggerFlex_Handle_downCast


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define xdcruntime_cstubs_LoggerFlex_Module_startupDone() xdcruntime_cstubs_LoggerFlex_Module__startupDone__S()

/* Object_heap */
#define xdcruntime_cstubs_LoggerFlex_Object_heap() xdcruntime_cstubs_LoggerFlex_Object__heap__C

/* Module_heap */
#define xdcruntime_cstubs_LoggerFlex_Module_heap() xdcruntime_cstubs_LoggerFlex_Object__heap__C

/* Module_id */
static inline CT__xdcruntime_cstubs_LoggerFlex_Module__id xdcruntime_cstubs_LoggerFlex_Module_id( void ) 
{
    return xdcruntime_cstubs_LoggerFlex_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool xdcruntime_cstubs_LoggerFlex_Module_hasMask( void ) 
{
    return xdcruntime_cstubs_LoggerFlex_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 xdcruntime_cstubs_LoggerFlex_Module_getMask( void ) 
{
    return xdcruntime_cstubs_LoggerFlex_Module__diagsMask__C != NULL ? *xdcruntime_cstubs_LoggerFlex_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void xdcruntime_cstubs_LoggerFlex_Module_setMask( xdc_Bits16 mask ) 
{
    if (xdcruntime_cstubs_LoggerFlex_Module__diagsMask__C != NULL) *xdcruntime_cstubs_LoggerFlex_Module__diagsMask__C = mask;
}

/* Params_init */
static inline void xdcruntime_cstubs_LoggerFlex_Params_init( xdcruntime_cstubs_LoggerFlex_Params* prms ) 
{
    if (prms) {
        xdcruntime_cstubs_LoggerFlex_Params__init__S(prms, 0, sizeof(xdcruntime_cstubs_LoggerFlex_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Params_copy */
static inline void xdcruntime_cstubs_LoggerFlex_Params_copy( xdcruntime_cstubs_LoggerFlex_Params* dst, const xdcruntime_cstubs_LoggerFlex_Params* src ) 
{
    if (dst) {
        xdcruntime_cstubs_LoggerFlex_Params__init__S(dst, (xdc_Ptr)src, sizeof(xdcruntime_cstubs_LoggerFlex_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Object_count */
#define xdcruntime_cstubs_LoggerFlex_Object_count() xdcruntime_cstubs_LoggerFlex_Object__count__C

/* Object_sizeof */
#define xdcruntime_cstubs_LoggerFlex_Object_sizeof() xdcruntime_cstubs_LoggerFlex_Object__sizeof__C

/* Object_get */
static inline xdcruntime_cstubs_LoggerFlex_Handle xdcruntime_cstubs_LoggerFlex_Object_get( xdcruntime_cstubs_LoggerFlex_Instance_State* oarr, int i ) 
{
    return (xdcruntime_cstubs_LoggerFlex_Handle)xdcruntime_cstubs_LoggerFlex_Object__get__S(oarr, i);
}

/* Object_first */
static inline xdcruntime_cstubs_LoggerFlex_Handle xdcruntime_cstubs_LoggerFlex_Object_first( void )
{
    return (xdcruntime_cstubs_LoggerFlex_Handle)xdcruntime_cstubs_LoggerFlex_Object__first__S();
}

/* Object_next */
static inline xdcruntime_cstubs_LoggerFlex_Handle xdcruntime_cstubs_LoggerFlex_Object_next( xdcruntime_cstubs_LoggerFlex_Object* obj )
{
    return (xdcruntime_cstubs_LoggerFlex_Handle)xdcruntime_cstubs_LoggerFlex_Object__next__S(obj);
}

/* Handle_label */
static inline xdc_runtime_Types_Label* xdcruntime_cstubs_LoggerFlex_Handle_label( xdcruntime_cstubs_LoggerFlex_Handle inst, xdc_runtime_Types_Label* lab )
{
    return xdcruntime_cstubs_LoggerFlex_Handle__label__S(inst, lab);
}

/* Handle_name */
static inline String xdcruntime_cstubs_LoggerFlex_Handle_name( xdcruntime_cstubs_LoggerFlex_Handle inst )
{
    xdc_runtime_Types_Label lab;
    return xdcruntime_cstubs_LoggerFlex_Handle__label__S(inst, &lab)->iname;
}

/* create */
static inline xdcruntime_cstubs_LoggerFlex_Handle xdcruntime_cstubs_LoggerFlex_create( const xdcruntime_cstubs_LoggerFlex_Params* __prms, xdc_runtime_Error_Block* __eb )
{
    return (xdcruntime_cstubs_LoggerFlex_Handle)xdcruntime_cstubs_LoggerFlex_Object__create__S(0, 0, 0, (const xdc_UChar*)__prms, sizeof(xdcruntime_cstubs_LoggerFlex_Params), __eb);
}

/* construct */
static inline void xdcruntime_cstubs_LoggerFlex_construct( xdcruntime_cstubs_LoggerFlex_Struct* __obj, const xdcruntime_cstubs_LoggerFlex_Params* __prms )
{
    xdcruntime_cstubs_LoggerFlex_Object__create__S(__obj, sizeof (xdcruntime_cstubs_LoggerFlex_Struct), 0, (const xdc_UChar*)__prms, sizeof(xdcruntime_cstubs_LoggerFlex_Params), NULL);
}

/* delete */
static inline void xdcruntime_cstubs_LoggerFlex_delete( xdcruntime_cstubs_LoggerFlex_Handle* instp )
{
    xdcruntime_cstubs_LoggerFlex_Object__delete__S(instp);
}

/* destruct */
static inline void xdcruntime_cstubs_LoggerFlex_destruct( xdcruntime_cstubs_LoggerFlex_Struct* obj )
{
    xdcruntime_cstubs_LoggerFlex_Object__destruct__S(obj);
}

/* handle */
static inline xdcruntime_cstubs_LoggerFlex_Handle xdcruntime_cstubs_LoggerFlex_handle( xdcruntime_cstubs_LoggerFlex_Struct* str )
{
    return (xdcruntime_cstubs_LoggerFlex_Handle)str;
}

/* struct */
static inline xdcruntime_cstubs_LoggerFlex_Struct* xdcruntime_cstubs_LoggerFlex_struct( xdcruntime_cstubs_LoggerFlex_Handle inst )
{
    return (xdcruntime_cstubs_LoggerFlex_Struct*)inst;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef xdcruntime_cstubs_LoggerFlex__top__
#undef __nested__
#endif

#endif /* xdcruntime_cstubs_LoggerFlex__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(xdcruntime_cstubs_LoggerFlex__internalaccess))

#ifndef xdcruntime_cstubs_LoggerFlex__include_state
#define xdcruntime_cstubs_LoggerFlex__include_state

/* Object */
struct xdcruntime_cstubs_LoggerFlex_Object {
    const xdcruntime_cstubs_LoggerFlex_Fxns__* __fxns;
};

#endif /* xdcruntime_cstubs_LoggerFlex__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(xdcruntime_cstubs_LoggerFlex__nolocalnames)

#ifndef xdcruntime_cstubs_LoggerFlex__localnames__done
#define xdcruntime_cstubs_LoggerFlex__localnames__done

/* module prefix */
#define LoggerFlex_Instance xdcruntime_cstubs_LoggerFlex_Instance
#define LoggerFlex_Handle xdcruntime_cstubs_LoggerFlex_Handle
#define LoggerFlex_Module xdcruntime_cstubs_LoggerFlex_Module
#define LoggerFlex_Object xdcruntime_cstubs_LoggerFlex_Object
#define LoggerFlex_Struct xdcruntime_cstubs_LoggerFlex_Struct
#define LoggerFlex_Instance_State xdcruntime_cstubs_LoggerFlex_Instance_State
#define LoggerFlex_E_atexit xdcruntime_cstubs_LoggerFlex_E_atexit
#define LoggerFlex_Params xdcruntime_cstubs_LoggerFlex_Params
#define LoggerFlex_enable xdcruntime_cstubs_LoggerFlex_enable
#define LoggerFlex_enable_fxnP xdcruntime_cstubs_LoggerFlex_enable_fxnP
#define LoggerFlex_enable_FxnT xdcruntime_cstubs_LoggerFlex_enable_FxnT
#define LoggerFlex_disable xdcruntime_cstubs_LoggerFlex_disable
#define LoggerFlex_disable_fxnP xdcruntime_cstubs_LoggerFlex_disable_fxnP
#define LoggerFlex_disable_FxnT xdcruntime_cstubs_LoggerFlex_disable_FxnT
#define LoggerFlex_write0 xdcruntime_cstubs_LoggerFlex_write0
#define LoggerFlex_write0_fxnP xdcruntime_cstubs_LoggerFlex_write0_fxnP
#define LoggerFlex_write0_FxnT xdcruntime_cstubs_LoggerFlex_write0_FxnT
#define LoggerFlex_write1 xdcruntime_cstubs_LoggerFlex_write1
#define LoggerFlex_write1_fxnP xdcruntime_cstubs_LoggerFlex_write1_fxnP
#define LoggerFlex_write1_FxnT xdcruntime_cstubs_LoggerFlex_write1_FxnT
#define LoggerFlex_write2 xdcruntime_cstubs_LoggerFlex_write2
#define LoggerFlex_write2_fxnP xdcruntime_cstubs_LoggerFlex_write2_fxnP
#define LoggerFlex_write2_FxnT xdcruntime_cstubs_LoggerFlex_write2_FxnT
#define LoggerFlex_write4 xdcruntime_cstubs_LoggerFlex_write4
#define LoggerFlex_write4_fxnP xdcruntime_cstubs_LoggerFlex_write4_fxnP
#define LoggerFlex_write4_FxnT xdcruntime_cstubs_LoggerFlex_write4_FxnT
#define LoggerFlex_write8 xdcruntime_cstubs_LoggerFlex_write8
#define LoggerFlex_write8_fxnP xdcruntime_cstubs_LoggerFlex_write8_fxnP
#define LoggerFlex_write8_FxnT xdcruntime_cstubs_LoggerFlex_write8_FxnT
#define LoggerFlex_shutdown xdcruntime_cstubs_LoggerFlex_shutdown
#define LoggerFlex_Module_name xdcruntime_cstubs_LoggerFlex_Module_name
#define LoggerFlex_Module_id xdcruntime_cstubs_LoggerFlex_Module_id
#define LoggerFlex_Module_startup xdcruntime_cstubs_LoggerFlex_Module_startup
#define LoggerFlex_Module_startupDone xdcruntime_cstubs_LoggerFlex_Module_startupDone
#define LoggerFlex_Module_hasMask xdcruntime_cstubs_LoggerFlex_Module_hasMask
#define LoggerFlex_Module_getMask xdcruntime_cstubs_LoggerFlex_Module_getMask
#define LoggerFlex_Module_setMask xdcruntime_cstubs_LoggerFlex_Module_setMask
#define LoggerFlex_Object_heap xdcruntime_cstubs_LoggerFlex_Object_heap
#define LoggerFlex_Module_heap xdcruntime_cstubs_LoggerFlex_Module_heap
#define LoggerFlex_construct xdcruntime_cstubs_LoggerFlex_construct
#define LoggerFlex_create xdcruntime_cstubs_LoggerFlex_create
#define LoggerFlex_handle xdcruntime_cstubs_LoggerFlex_handle
#define LoggerFlex_struct xdcruntime_cstubs_LoggerFlex_struct
#define LoggerFlex_Handle_label xdcruntime_cstubs_LoggerFlex_Handle_label
#define LoggerFlex_Handle_name xdcruntime_cstubs_LoggerFlex_Handle_name
#define LoggerFlex_Instance_init xdcruntime_cstubs_LoggerFlex_Instance_init
#define LoggerFlex_Object_count xdcruntime_cstubs_LoggerFlex_Object_count
#define LoggerFlex_Object_get xdcruntime_cstubs_LoggerFlex_Object_get
#define LoggerFlex_Object_first xdcruntime_cstubs_LoggerFlex_Object_first
#define LoggerFlex_Object_next xdcruntime_cstubs_LoggerFlex_Object_next
#define LoggerFlex_Object_sizeof xdcruntime_cstubs_LoggerFlex_Object_sizeof
#define LoggerFlex_Params_copy xdcruntime_cstubs_LoggerFlex_Params_copy
#define LoggerFlex_Params_init xdcruntime_cstubs_LoggerFlex_Params_init
#define LoggerFlex_delete xdcruntime_cstubs_LoggerFlex_delete
#define LoggerFlex_destruct xdcruntime_cstubs_LoggerFlex_destruct
#define LoggerFlex_Module_upCast xdcruntime_cstubs_LoggerFlex_Module_upCast
#define LoggerFlex_Module_to_xdc_runtime_ILogger xdcruntime_cstubs_LoggerFlex_Module_to_xdc_runtime_ILogger
#define LoggerFlex_Handle_upCast xdcruntime_cstubs_LoggerFlex_Handle_upCast
#define LoggerFlex_Handle_to_xdc_runtime_ILogger xdcruntime_cstubs_LoggerFlex_Handle_to_xdc_runtime_ILogger
#define LoggerFlex_Handle_downCast xdcruntime_cstubs_LoggerFlex_Handle_downCast
#define LoggerFlex_Handle_from_xdc_runtime_ILogger xdcruntime_cstubs_LoggerFlex_Handle_from_xdc_runtime_ILogger

#endif /* xdcruntime_cstubs_LoggerFlex__localnames__done */
#endif
/*
 *  @(#) xdcruntime.cstubs; 1, 0, 0,1; 2-15-2012 07:45:05; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

