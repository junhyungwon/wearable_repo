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

#ifndef ti_sdo_utils_loggers_LoggerMFPNull__include
#define ti_sdo_utils_loggers_LoggerMFPNull__include

#ifndef __nested__
#define __nested__
#define ti_sdo_utils_loggers_LoggerMFPNull__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sdo_utils_loggers_LoggerMFPNull___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/IInstance.h>
#include <ti/sdo/utils/loggers/package/package.defs.h>

#include <ti/sdo/utils/loggers/ILoggerMFP.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* Cmd */
typedef ti_sdo_utils_loggers_ILoggerMFP_Cmd ti_sdo_utils_loggers_LoggerMFPNull_Cmd;

/* SETTRACE */
#define ti_sdo_utils_loggers_LoggerMFPNull_SETTRACE ti_sdo_utils_loggers_ILoggerMFP_SETTRACE


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsEnabled ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsIncluded ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsMask;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsMask ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__gateObj;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__gateObj ti_sdo_utils_loggers_LoggerMFPNull_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__gatePrms;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__gatePrms ti_sdo_utils_loggers_LoggerMFPNull_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__id;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__id ti_sdo_utils_loggers_LoggerMFPNull_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerDefined;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerDefined ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerObj;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerObj ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn0 ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn1 ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn2 ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn4 ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn8 ti_sdo_utils_loggers_LoggerMFPNull_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__startupDoneFxn ti_sdo_utils_loggers_LoggerMFPNull_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sdo_utils_loggers_LoggerMFPNull_Object__count;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Object__count ti_sdo_utils_loggers_LoggerMFPNull_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sdo_utils_loggers_LoggerMFPNull_Object__heap;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Object__heap ti_sdo_utils_loggers_LoggerMFPNull_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sdo_utils_loggers_LoggerMFPNull_Object__sizeof;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Object__sizeof ti_sdo_utils_loggers_LoggerMFPNull_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sdo_utils_loggers_LoggerMFPNull_Object__table;
__extern __FAR__ const CT__ti_sdo_utils_loggers_LoggerMFPNull_Object__table ti_sdo_utils_loggers_LoggerMFPNull_Object__table__C;


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct ti_sdo_utils_loggers_LoggerMFPNull_Params {
    size_t __size;
    const void* __self;
    void* __fxns;
    xdc_runtime_IInstance_Params* instance;
    xdc_runtime_IInstance_Params __iprms;
};

/* Struct */
struct ti_sdo_utils_loggers_LoggerMFPNull_Struct {
    const ti_sdo_utils_loggers_LoggerMFPNull_Fxns__* __fxns;
    xdc_Bool __f0;
    xdc_runtime_Types_CordAddr __name;
};


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_sdo_utils_loggers_LoggerMFPNull_Fxns__ {
    xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_Bool (*enable)(ti_sdo_utils_loggers_LoggerMFPNull_Handle);
    xdc_Bool (*disable)(ti_sdo_utils_loggers_LoggerMFPNull_Handle);
    xdc_Void (*write0)(ti_sdo_utils_loggers_LoggerMFPNull_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId);
    xdc_Void (*write1)(ti_sdo_utils_loggers_LoggerMFPNull_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg);
    xdc_Void (*write2)(ti_sdo_utils_loggers_LoggerMFPNull_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg);
    xdc_Void (*write4)(ti_sdo_utils_loggers_LoggerMFPNull_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
    xdc_Void (*write8)(ti_sdo_utils_loggers_LoggerMFPNull_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
    xdc_Void (*control)(xdc_Int, xdc_UArg);
    xdc_runtime_Types_SysFxns2 __sfxns;
};

/* Module__FXNS__C */
__extern const ti_sdo_utils_loggers_LoggerMFPNull_Fxns__ ti_sdo_utils_loggers_LoggerMFPNull_Module__FXNS__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sdo_utils_loggers_LoggerMFPNull_Module_startup( state ) -1

/* Instance_init__F */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Instance_init__F, "ti_sdo_utils_loggers_LoggerMFPNull_Instance_init")
__extern void ti_sdo_utils_loggers_LoggerMFPNull_Instance_init__F( ti_sdo_utils_loggers_LoggerMFPNull_Object*, const ti_sdo_utils_loggers_LoggerMFPNull_Params* );

/* Instance_init__R */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Instance_init__R, "ti_sdo_utils_loggers_LoggerMFPNull_Instance_init")
__extern void ti_sdo_utils_loggers_LoggerMFPNull_Instance_init__R( ti_sdo_utils_loggers_LoggerMFPNull_Object*, const ti_sdo_utils_loggers_LoggerMFPNull_Params* );

/* Handle__label__S */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Handle__label__S, "ti_sdo_utils_loggers_LoggerMFPNull_Handle__label")
__extern xdc_runtime_Types_Label* ti_sdo_utils_loggers_LoggerMFPNull_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab );

/* Module__startupDone__S */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Module__startupDone__S, "ti_sdo_utils_loggers_LoggerMFPNull_Module__startupDone")
__extern xdc_Bool ti_sdo_utils_loggers_LoggerMFPNull_Module__startupDone__S( void );

/* Object__create__S */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Object__create__S, "ti_sdo_utils_loggers_LoggerMFPNull_Object__create")
__extern xdc_Ptr ti_sdo_utils_loggers_LoggerMFPNull_Object__create__S( xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb );

/* Object__delete__S */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Object__delete__S, "ti_sdo_utils_loggers_LoggerMFPNull_Object__delete")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_Object__delete__S( xdc_Ptr instp );

/* Object__destruct__S */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Object__destruct__S, "ti_sdo_utils_loggers_LoggerMFPNull_Object__destruct")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_Object__destruct__S( xdc_Ptr objp );

/* Object__get__S */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Object__get__S, "ti_sdo_utils_loggers_LoggerMFPNull_Object__get")
__extern xdc_Ptr ti_sdo_utils_loggers_LoggerMFPNull_Object__get__S( xdc_Ptr oarr, xdc_Int i );

/* Object__first__S */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Object__first__S, "ti_sdo_utils_loggers_LoggerMFPNull_Object__first")
__extern xdc_Ptr ti_sdo_utils_loggers_LoggerMFPNull_Object__first__S( void );

/* Object__next__S */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Object__next__S, "ti_sdo_utils_loggers_LoggerMFPNull_Object__next")
__extern xdc_Ptr ti_sdo_utils_loggers_LoggerMFPNull_Object__next__S( xdc_Ptr obj );

/* Params__init__S */
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_Params__init__S, "ti_sdo_utils_loggers_LoggerMFPNull_Params__init")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_Params__init__S( xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz );

/* enable__E */
#define ti_sdo_utils_loggers_LoggerMFPNull_enable ti_sdo_utils_loggers_LoggerMFPNull_enable__E
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_enable__E, "ti_sdo_utils_loggers_LoggerMFPNull_enable")
__extern xdc_Bool ti_sdo_utils_loggers_LoggerMFPNull_enable__E( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst );
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_enable__F, "ti_sdo_utils_loggers_LoggerMFPNull_enable")
__extern xdc_Bool ti_sdo_utils_loggers_LoggerMFPNull_enable__F( ti_sdo_utils_loggers_LoggerMFPNull_Object* __inst );
__extern xdc_Bool ti_sdo_utils_loggers_LoggerMFPNull_enable__R( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst );

/* disable__E */
#define ti_sdo_utils_loggers_LoggerMFPNull_disable ti_sdo_utils_loggers_LoggerMFPNull_disable__E
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_disable__E, "ti_sdo_utils_loggers_LoggerMFPNull_disable")
__extern xdc_Bool ti_sdo_utils_loggers_LoggerMFPNull_disable__E( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst );
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_disable__F, "ti_sdo_utils_loggers_LoggerMFPNull_disable")
__extern xdc_Bool ti_sdo_utils_loggers_LoggerMFPNull_disable__F( ti_sdo_utils_loggers_LoggerMFPNull_Object* __inst );
__extern xdc_Bool ti_sdo_utils_loggers_LoggerMFPNull_disable__R( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst );

/* write0__E */
#define ti_sdo_utils_loggers_LoggerMFPNull_write0 ti_sdo_utils_loggers_LoggerMFPNull_write0__E
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_write0__E, "ti_sdo_utils_loggers_LoggerMFPNull_write0")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write0__E( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid );
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_write0__F, "ti_sdo_utils_loggers_LoggerMFPNull_write0")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write0__F( ti_sdo_utils_loggers_LoggerMFPNull_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid );
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write0__R( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid );

/* write1__E */
#define ti_sdo_utils_loggers_LoggerMFPNull_write1 ti_sdo_utils_loggers_LoggerMFPNull_write1__E
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_write1__E, "ti_sdo_utils_loggers_LoggerMFPNull_write1")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write1__E( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 );
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_write1__F, "ti_sdo_utils_loggers_LoggerMFPNull_write1")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write1__F( ti_sdo_utils_loggers_LoggerMFPNull_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 );
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write1__R( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 );

/* write2__E */
#define ti_sdo_utils_loggers_LoggerMFPNull_write2 ti_sdo_utils_loggers_LoggerMFPNull_write2__E
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_write2__E, "ti_sdo_utils_loggers_LoggerMFPNull_write2")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write2__E( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 );
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_write2__F, "ti_sdo_utils_loggers_LoggerMFPNull_write2")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write2__F( ti_sdo_utils_loggers_LoggerMFPNull_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 );
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write2__R( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 );

/* write4__E */
#define ti_sdo_utils_loggers_LoggerMFPNull_write4 ti_sdo_utils_loggers_LoggerMFPNull_write4__E
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_write4__E, "ti_sdo_utils_loggers_LoggerMFPNull_write4")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write4__E( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 );
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_write4__F, "ti_sdo_utils_loggers_LoggerMFPNull_write4")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write4__F( ti_sdo_utils_loggers_LoggerMFPNull_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 );
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write4__R( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 );

/* write8__E */
#define ti_sdo_utils_loggers_LoggerMFPNull_write8 ti_sdo_utils_loggers_LoggerMFPNull_write8__E
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_write8__E, "ti_sdo_utils_loggers_LoggerMFPNull_write8")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write8__E( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 );
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_write8__F, "ti_sdo_utils_loggers_LoggerMFPNull_write8")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write8__F( ti_sdo_utils_loggers_LoggerMFPNull_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 );
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_write8__R( ti_sdo_utils_loggers_LoggerMFPNull_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 );

/* control__E */
#define ti_sdo_utils_loggers_LoggerMFPNull_control ti_sdo_utils_loggers_LoggerMFPNull_control__E
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_control__E, "ti_sdo_utils_loggers_LoggerMFPNull_control")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_control__E( xdc_Int cmd, xdc_UArg cmdArgs );
xdc__CODESECT(ti_sdo_utils_loggers_LoggerMFPNull_control__F, "ti_sdo_utils_loggers_LoggerMFPNull_control")
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_control__F( xdc_Int cmd, xdc_UArg cmdArgs );
__extern xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_control__R( xdc_Int cmd, xdc_UArg cmdArgs );


/*
 * ======== FUNCTION SELECTORS ========
 */

/* enable_{FxnT,fxnP} */
typedef xdc_Bool (*ti_sdo_utils_loggers_LoggerMFPNull_enable_FxnT)(void*);
static inline ti_sdo_utils_loggers_LoggerMFPNull_enable_FxnT ti_sdo_utils_loggers_LoggerMFPNull_enable_fxnP( void )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_enable_FxnT)ti_sdo_utils_loggers_LoggerMFPNull_enable; 
}

/* disable_{FxnT,fxnP} */
typedef xdc_Bool (*ti_sdo_utils_loggers_LoggerMFPNull_disable_FxnT)(void*);
static inline ti_sdo_utils_loggers_LoggerMFPNull_disable_FxnT ti_sdo_utils_loggers_LoggerMFPNull_disable_fxnP( void )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_disable_FxnT)ti_sdo_utils_loggers_LoggerMFPNull_disable; 
}

/* write0_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_LoggerMFPNull_write0_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId);
static inline ti_sdo_utils_loggers_LoggerMFPNull_write0_FxnT ti_sdo_utils_loggers_LoggerMFPNull_write0_fxnP( void )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_write0_FxnT)ti_sdo_utils_loggers_LoggerMFPNull_write0; 
}

/* write1_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_LoggerMFPNull_write1_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg);
static inline ti_sdo_utils_loggers_LoggerMFPNull_write1_FxnT ti_sdo_utils_loggers_LoggerMFPNull_write1_fxnP( void )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_write1_FxnT)ti_sdo_utils_loggers_LoggerMFPNull_write1; 
}

/* write2_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_LoggerMFPNull_write2_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg);
static inline ti_sdo_utils_loggers_LoggerMFPNull_write2_FxnT ti_sdo_utils_loggers_LoggerMFPNull_write2_fxnP( void )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_write2_FxnT)ti_sdo_utils_loggers_LoggerMFPNull_write2; 
}

/* write4_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_LoggerMFPNull_write4_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
static inline ti_sdo_utils_loggers_LoggerMFPNull_write4_FxnT ti_sdo_utils_loggers_LoggerMFPNull_write4_fxnP( void )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_write4_FxnT)ti_sdo_utils_loggers_LoggerMFPNull_write4; 
}

/* write8_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_LoggerMFPNull_write8_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
static inline ti_sdo_utils_loggers_LoggerMFPNull_write8_FxnT ti_sdo_utils_loggers_LoggerMFPNull_write8_fxnP( void )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_write8_FxnT)ti_sdo_utils_loggers_LoggerMFPNull_write8; 
}


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline ti_sdo_utils_loggers_ILoggerMFP_Module ti_sdo_utils_loggers_LoggerMFPNull_Module_upCast( void )
{
    return (ti_sdo_utils_loggers_ILoggerMFP_Module)&ti_sdo_utils_loggers_LoggerMFPNull_Module__FXNS__C;
}

/* Module_to_ti_sdo_utils_loggers_ILoggerMFP */
#define ti_sdo_utils_loggers_LoggerMFPNull_Module_to_ti_sdo_utils_loggers_ILoggerMFP ti_sdo_utils_loggers_LoggerMFPNull_Module_upCast

/* Handle_upCast */
static inline ti_sdo_utils_loggers_ILoggerMFP_Handle ti_sdo_utils_loggers_LoggerMFPNull_Handle_upCast( ti_sdo_utils_loggers_LoggerMFPNull_Handle i )
{
    return (ti_sdo_utils_loggers_ILoggerMFP_Handle)i;
}

/* Handle_to_ti_sdo_utils_loggers_ILoggerMFP */
#define ti_sdo_utils_loggers_LoggerMFPNull_Handle_to_ti_sdo_utils_loggers_ILoggerMFP ti_sdo_utils_loggers_LoggerMFPNull_Handle_upCast

/* Handle_downCast */
static inline ti_sdo_utils_loggers_LoggerMFPNull_Handle ti_sdo_utils_loggers_LoggerMFPNull_Handle_downCast( ti_sdo_utils_loggers_ILoggerMFP_Handle i )
{
    ti_sdo_utils_loggers_ILoggerMFP_Handle i2 = (ti_sdo_utils_loggers_ILoggerMFP_Handle)i;
    return (void*)i2->__fxns == (void*)&ti_sdo_utils_loggers_LoggerMFPNull_Module__FXNS__C ? (ti_sdo_utils_loggers_LoggerMFPNull_Handle)i : 0;
}

/* Handle_from_ti_sdo_utils_loggers_ILoggerMFP */
#define ti_sdo_utils_loggers_LoggerMFPNull_Handle_from_ti_sdo_utils_loggers_ILoggerMFP ti_sdo_utils_loggers_LoggerMFPNull_Handle_downCast

/* Module_upCast2 */
static inline xdc_runtime_ILogger_Module ti_sdo_utils_loggers_LoggerMFPNull_Module_upCast2( void )
{
    return (xdc_runtime_ILogger_Module)&ti_sdo_utils_loggers_LoggerMFPNull_Module__FXNS__C;
}

/* Module_to_xdc_runtime_ILogger */
#define ti_sdo_utils_loggers_LoggerMFPNull_Module_to_xdc_runtime_ILogger ti_sdo_utils_loggers_LoggerMFPNull_Module_upCast2

/* Handle_upCast2 */
static inline xdc_runtime_ILogger_Handle ti_sdo_utils_loggers_LoggerMFPNull_Handle_upCast2( ti_sdo_utils_loggers_LoggerMFPNull_Handle i )
{
    return (xdc_runtime_ILogger_Handle)i;
}

/* Handle_to_xdc_runtime_ILogger */
#define ti_sdo_utils_loggers_LoggerMFPNull_Handle_to_xdc_runtime_ILogger ti_sdo_utils_loggers_LoggerMFPNull_Handle_upCast2

/* Handle_downCast2 */
static inline ti_sdo_utils_loggers_LoggerMFPNull_Handle ti_sdo_utils_loggers_LoggerMFPNull_Handle_downCast2( xdc_runtime_ILogger_Handle i )
{
    xdc_runtime_ILogger_Handle i2 = (xdc_runtime_ILogger_Handle)i;
    return (void*)i2->__fxns == (void*)&ti_sdo_utils_loggers_LoggerMFPNull_Module__FXNS__C ? (ti_sdo_utils_loggers_LoggerMFPNull_Handle)i : 0;
}

/* Handle_from_xdc_runtime_ILogger */
#define ti_sdo_utils_loggers_LoggerMFPNull_Handle_from_xdc_runtime_ILogger ti_sdo_utils_loggers_LoggerMFPNull_Handle_downCast2


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sdo_utils_loggers_LoggerMFPNull_Module_startupDone() ti_sdo_utils_loggers_LoggerMFPNull_Module__startupDone__S()

/* Object_heap */
#define ti_sdo_utils_loggers_LoggerMFPNull_Object_heap() ti_sdo_utils_loggers_LoggerMFPNull_Object__heap__C

/* Module_heap */
#define ti_sdo_utils_loggers_LoggerMFPNull_Module_heap() ti_sdo_utils_loggers_LoggerMFPNull_Object__heap__C

/* Module_id */
static inline CT__ti_sdo_utils_loggers_LoggerMFPNull_Module__id ti_sdo_utils_loggers_LoggerMFPNull_Module_id( void ) 
{
    return ti_sdo_utils_loggers_LoggerMFPNull_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sdo_utils_loggers_LoggerMFPNull_Module_hasMask( void ) 
{
    return ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sdo_utils_loggers_LoggerMFPNull_Module_getMask( void ) 
{
    return ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsMask__C != NULL ? *ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sdo_utils_loggers_LoggerMFPNull_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsMask__C != NULL) *ti_sdo_utils_loggers_LoggerMFPNull_Module__diagsMask__C = mask;
}

/* Params_init */
static inline void ti_sdo_utils_loggers_LoggerMFPNull_Params_init( ti_sdo_utils_loggers_LoggerMFPNull_Params* prms ) 
{
    if (prms) {
        ti_sdo_utils_loggers_LoggerMFPNull_Params__init__S(prms, 0, sizeof(ti_sdo_utils_loggers_LoggerMFPNull_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Params_copy */
static inline void ti_sdo_utils_loggers_LoggerMFPNull_Params_copy( ti_sdo_utils_loggers_LoggerMFPNull_Params* dst, const ti_sdo_utils_loggers_LoggerMFPNull_Params* src ) 
{
    if (dst) {
        ti_sdo_utils_loggers_LoggerMFPNull_Params__init__S(dst, (xdc_Ptr)src, sizeof(ti_sdo_utils_loggers_LoggerMFPNull_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Object_count */
#define ti_sdo_utils_loggers_LoggerMFPNull_Object_count() ti_sdo_utils_loggers_LoggerMFPNull_Object__count__C

/* Object_sizeof */
#define ti_sdo_utils_loggers_LoggerMFPNull_Object_sizeof() ti_sdo_utils_loggers_LoggerMFPNull_Object__sizeof__C

/* Object_get */
static inline ti_sdo_utils_loggers_LoggerMFPNull_Handle ti_sdo_utils_loggers_LoggerMFPNull_Object_get( ti_sdo_utils_loggers_LoggerMFPNull_Instance_State* oarr, int i ) 
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_Handle)ti_sdo_utils_loggers_LoggerMFPNull_Object__get__S(oarr, i);
}

/* Object_first */
static inline ti_sdo_utils_loggers_LoggerMFPNull_Handle ti_sdo_utils_loggers_LoggerMFPNull_Object_first( void )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_Handle)ti_sdo_utils_loggers_LoggerMFPNull_Object__first__S();
}

/* Object_next */
static inline ti_sdo_utils_loggers_LoggerMFPNull_Handle ti_sdo_utils_loggers_LoggerMFPNull_Object_next( ti_sdo_utils_loggers_LoggerMFPNull_Object* obj )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_Handle)ti_sdo_utils_loggers_LoggerMFPNull_Object__next__S(obj);
}

/* Handle_label */
static inline xdc_runtime_Types_Label* ti_sdo_utils_loggers_LoggerMFPNull_Handle_label( ti_sdo_utils_loggers_LoggerMFPNull_Handle inst, xdc_runtime_Types_Label* lab )
{
    return ti_sdo_utils_loggers_LoggerMFPNull_Handle__label__S(inst, lab);
}

/* Handle_name */
static inline String ti_sdo_utils_loggers_LoggerMFPNull_Handle_name( ti_sdo_utils_loggers_LoggerMFPNull_Handle inst )
{
    xdc_runtime_Types_Label lab;
    return ti_sdo_utils_loggers_LoggerMFPNull_Handle__label__S(inst, &lab)->iname;
}

/* create */
static inline ti_sdo_utils_loggers_LoggerMFPNull_Handle ti_sdo_utils_loggers_LoggerMFPNull_create( const ti_sdo_utils_loggers_LoggerMFPNull_Params* __prms, xdc_runtime_Error_Block* __eb )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_Handle)ti_sdo_utils_loggers_LoggerMFPNull_Object__create__S(0, 0, 0, (const xdc_UChar*)__prms, sizeof(ti_sdo_utils_loggers_LoggerMFPNull_Params), __eb);
}

/* construct */
static inline void ti_sdo_utils_loggers_LoggerMFPNull_construct( ti_sdo_utils_loggers_LoggerMFPNull_Struct* __obj, const ti_sdo_utils_loggers_LoggerMFPNull_Params* __prms )
{
    ti_sdo_utils_loggers_LoggerMFPNull_Object__create__S(__obj, sizeof (ti_sdo_utils_loggers_LoggerMFPNull_Struct), 0, (const xdc_UChar*)__prms, sizeof(ti_sdo_utils_loggers_LoggerMFPNull_Params), NULL);
}

/* delete */
static inline void ti_sdo_utils_loggers_LoggerMFPNull_delete( ti_sdo_utils_loggers_LoggerMFPNull_Handle* instp )
{
    ti_sdo_utils_loggers_LoggerMFPNull_Object__delete__S(instp);
}

/* destruct */
static inline void ti_sdo_utils_loggers_LoggerMFPNull_destruct( ti_sdo_utils_loggers_LoggerMFPNull_Struct* obj )
{
    ti_sdo_utils_loggers_LoggerMFPNull_Object__destruct__S(obj);
}

/* handle */
static inline ti_sdo_utils_loggers_LoggerMFPNull_Handle ti_sdo_utils_loggers_LoggerMFPNull_handle( ti_sdo_utils_loggers_LoggerMFPNull_Struct* str )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_Handle)str;
}

/* struct */
static inline ti_sdo_utils_loggers_LoggerMFPNull_Struct* ti_sdo_utils_loggers_LoggerMFPNull_struct( ti_sdo_utils_loggers_LoggerMFPNull_Handle inst )
{
    return (ti_sdo_utils_loggers_LoggerMFPNull_Struct*)inst;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sdo_utils_loggers_LoggerMFPNull__top__
#undef __nested__
#endif

#endif /* ti_sdo_utils_loggers_LoggerMFPNull__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sdo_utils_loggers_LoggerMFPNull__internalaccess))

#ifndef ti_sdo_utils_loggers_LoggerMFPNull__include_state
#define ti_sdo_utils_loggers_LoggerMFPNull__include_state

/* Object */
struct ti_sdo_utils_loggers_LoggerMFPNull_Object {
    const ti_sdo_utils_loggers_LoggerMFPNull_Fxns__* __fxns;
    xdc_Bool enabled;
};

#endif /* ti_sdo_utils_loggers_LoggerMFPNull__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sdo_utils_loggers_LoggerMFPNull__nolocalnames)

#ifndef ti_sdo_utils_loggers_LoggerMFPNull__localnames__done
#define ti_sdo_utils_loggers_LoggerMFPNull__localnames__done

/* module prefix */
#define LoggerMFPNull_Instance ti_sdo_utils_loggers_LoggerMFPNull_Instance
#define LoggerMFPNull_Handle ti_sdo_utils_loggers_LoggerMFPNull_Handle
#define LoggerMFPNull_Module ti_sdo_utils_loggers_LoggerMFPNull_Module
#define LoggerMFPNull_Object ti_sdo_utils_loggers_LoggerMFPNull_Object
#define LoggerMFPNull_Struct ti_sdo_utils_loggers_LoggerMFPNull_Struct
#define LoggerMFPNull_Cmd ti_sdo_utils_loggers_LoggerMFPNull_Cmd
#define LoggerMFPNull_Instance_State ti_sdo_utils_loggers_LoggerMFPNull_Instance_State
#define LoggerMFPNull_SETTRACE ti_sdo_utils_loggers_LoggerMFPNull_SETTRACE
#define LoggerMFPNull_Params ti_sdo_utils_loggers_LoggerMFPNull_Params
#define LoggerMFPNull_enable ti_sdo_utils_loggers_LoggerMFPNull_enable
#define LoggerMFPNull_enable_fxnP ti_sdo_utils_loggers_LoggerMFPNull_enable_fxnP
#define LoggerMFPNull_enable_FxnT ti_sdo_utils_loggers_LoggerMFPNull_enable_FxnT
#define LoggerMFPNull_disable ti_sdo_utils_loggers_LoggerMFPNull_disable
#define LoggerMFPNull_disable_fxnP ti_sdo_utils_loggers_LoggerMFPNull_disable_fxnP
#define LoggerMFPNull_disable_FxnT ti_sdo_utils_loggers_LoggerMFPNull_disable_FxnT
#define LoggerMFPNull_write0 ti_sdo_utils_loggers_LoggerMFPNull_write0
#define LoggerMFPNull_write0_fxnP ti_sdo_utils_loggers_LoggerMFPNull_write0_fxnP
#define LoggerMFPNull_write0_FxnT ti_sdo_utils_loggers_LoggerMFPNull_write0_FxnT
#define LoggerMFPNull_write1 ti_sdo_utils_loggers_LoggerMFPNull_write1
#define LoggerMFPNull_write1_fxnP ti_sdo_utils_loggers_LoggerMFPNull_write1_fxnP
#define LoggerMFPNull_write1_FxnT ti_sdo_utils_loggers_LoggerMFPNull_write1_FxnT
#define LoggerMFPNull_write2 ti_sdo_utils_loggers_LoggerMFPNull_write2
#define LoggerMFPNull_write2_fxnP ti_sdo_utils_loggers_LoggerMFPNull_write2_fxnP
#define LoggerMFPNull_write2_FxnT ti_sdo_utils_loggers_LoggerMFPNull_write2_FxnT
#define LoggerMFPNull_write4 ti_sdo_utils_loggers_LoggerMFPNull_write4
#define LoggerMFPNull_write4_fxnP ti_sdo_utils_loggers_LoggerMFPNull_write4_fxnP
#define LoggerMFPNull_write4_FxnT ti_sdo_utils_loggers_LoggerMFPNull_write4_FxnT
#define LoggerMFPNull_write8 ti_sdo_utils_loggers_LoggerMFPNull_write8
#define LoggerMFPNull_write8_fxnP ti_sdo_utils_loggers_LoggerMFPNull_write8_fxnP
#define LoggerMFPNull_write8_FxnT ti_sdo_utils_loggers_LoggerMFPNull_write8_FxnT
#define LoggerMFPNull_control ti_sdo_utils_loggers_LoggerMFPNull_control
#define LoggerMFPNull_Module_name ti_sdo_utils_loggers_LoggerMFPNull_Module_name
#define LoggerMFPNull_Module_id ti_sdo_utils_loggers_LoggerMFPNull_Module_id
#define LoggerMFPNull_Module_startup ti_sdo_utils_loggers_LoggerMFPNull_Module_startup
#define LoggerMFPNull_Module_startupDone ti_sdo_utils_loggers_LoggerMFPNull_Module_startupDone
#define LoggerMFPNull_Module_hasMask ti_sdo_utils_loggers_LoggerMFPNull_Module_hasMask
#define LoggerMFPNull_Module_getMask ti_sdo_utils_loggers_LoggerMFPNull_Module_getMask
#define LoggerMFPNull_Module_setMask ti_sdo_utils_loggers_LoggerMFPNull_Module_setMask
#define LoggerMFPNull_Object_heap ti_sdo_utils_loggers_LoggerMFPNull_Object_heap
#define LoggerMFPNull_Module_heap ti_sdo_utils_loggers_LoggerMFPNull_Module_heap
#define LoggerMFPNull_construct ti_sdo_utils_loggers_LoggerMFPNull_construct
#define LoggerMFPNull_create ti_sdo_utils_loggers_LoggerMFPNull_create
#define LoggerMFPNull_handle ti_sdo_utils_loggers_LoggerMFPNull_handle
#define LoggerMFPNull_struct ti_sdo_utils_loggers_LoggerMFPNull_struct
#define LoggerMFPNull_Handle_label ti_sdo_utils_loggers_LoggerMFPNull_Handle_label
#define LoggerMFPNull_Handle_name ti_sdo_utils_loggers_LoggerMFPNull_Handle_name
#define LoggerMFPNull_Instance_init ti_sdo_utils_loggers_LoggerMFPNull_Instance_init
#define LoggerMFPNull_Object_count ti_sdo_utils_loggers_LoggerMFPNull_Object_count
#define LoggerMFPNull_Object_get ti_sdo_utils_loggers_LoggerMFPNull_Object_get
#define LoggerMFPNull_Object_first ti_sdo_utils_loggers_LoggerMFPNull_Object_first
#define LoggerMFPNull_Object_next ti_sdo_utils_loggers_LoggerMFPNull_Object_next
#define LoggerMFPNull_Object_sizeof ti_sdo_utils_loggers_LoggerMFPNull_Object_sizeof
#define LoggerMFPNull_Params_copy ti_sdo_utils_loggers_LoggerMFPNull_Params_copy
#define LoggerMFPNull_Params_init ti_sdo_utils_loggers_LoggerMFPNull_Params_init
#define LoggerMFPNull_delete ti_sdo_utils_loggers_LoggerMFPNull_delete
#define LoggerMFPNull_destruct ti_sdo_utils_loggers_LoggerMFPNull_destruct
#define LoggerMFPNull_Module_upCast ti_sdo_utils_loggers_LoggerMFPNull_Module_upCast
#define LoggerMFPNull_Module_to_ti_sdo_utils_loggers_ILoggerMFP ti_sdo_utils_loggers_LoggerMFPNull_Module_to_ti_sdo_utils_loggers_ILoggerMFP
#define LoggerMFPNull_Handle_upCast ti_sdo_utils_loggers_LoggerMFPNull_Handle_upCast
#define LoggerMFPNull_Handle_to_ti_sdo_utils_loggers_ILoggerMFP ti_sdo_utils_loggers_LoggerMFPNull_Handle_to_ti_sdo_utils_loggers_ILoggerMFP
#define LoggerMFPNull_Handle_downCast ti_sdo_utils_loggers_LoggerMFPNull_Handle_downCast
#define LoggerMFPNull_Handle_from_ti_sdo_utils_loggers_ILoggerMFP ti_sdo_utils_loggers_LoggerMFPNull_Handle_from_ti_sdo_utils_loggers_ILoggerMFP
#define LoggerMFPNull_Module_upCast2 ti_sdo_utils_loggers_LoggerMFPNull_Module_upCast2
#define LoggerMFPNull_Module_to_xdc_runtime_ILogger ti_sdo_utils_loggers_LoggerMFPNull_Module_to_xdc_runtime_ILogger
#define LoggerMFPNull_Handle_upCast2 ti_sdo_utils_loggers_LoggerMFPNull_Handle_upCast2
#define LoggerMFPNull_Handle_to_xdc_runtime_ILogger ti_sdo_utils_loggers_LoggerMFPNull_Handle_to_xdc_runtime_ILogger
#define LoggerMFPNull_Handle_downCast2 ti_sdo_utils_loggers_LoggerMFPNull_Handle_downCast2
#define LoggerMFPNull_Handle_from_xdc_runtime_ILogger ti_sdo_utils_loggers_LoggerMFPNull_Handle_from_xdc_runtime_ILogger

#endif /* ti_sdo_utils_loggers_LoggerMFPNull__localnames__done */
#endif
/*
 *  @(#) ti.sdo.utils.loggers; 1, 0, 0,1; 2-15-2012 07:44:55; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

