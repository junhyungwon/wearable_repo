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
 *     PER-INSTANCE TYPES
 *     VIRTUAL FUNCTIONS
 *     FUNCTION STUBS
 *     FUNCTION SELECTORS
 *     CONVERTORS
 *     
 *     EPILOGUE
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_sdo_utils_loggers_ILoggerMFP__include
#define ti_sdo_utils_loggers_ILoggerMFP__include

#ifndef __nested__
#define __nested__
#define ti_sdo_utils_loggers_ILoggerMFP__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sdo_utils_loggers_ILoggerMFP___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/IInstance.h>
#include <ti/sdo/utils/loggers/package/package.defs.h>

#include <xdc/runtime/ILogger.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* Cmd */
enum ti_sdo_utils_loggers_ILoggerMFP_Cmd {
    ti_sdo_utils_loggers_ILoggerMFP_SETTRACE = 0
};
typedef enum ti_sdo_utils_loggers_ILoggerMFP_Cmd ti_sdo_utils_loggers_ILoggerMFP_Cmd;


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct ti_sdo_utils_loggers_ILoggerMFP_Params {
    size_t __size;
    const void* __self;
    void* __fxns;
    xdc_runtime_IInstance_Params* instance;
};


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_sdo_utils_loggers_ILoggerMFP_Fxns__ {
    xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_Bool (*enable)(void*);
    xdc_Bool (*disable)(void*);
    xdc_Void (*write0)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId);
    xdc_Void (*write1)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg);
    xdc_Void (*write2)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg);
    xdc_Void (*write4)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
    xdc_Void (*write8)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
    xdc_Void (*control)(xdc_Int, xdc_UArg);
    xdc_runtime_Types_SysFxns2 __sfxns;
};

/* Interface__BASE__C */
__extern const xdc_runtime_Types_Base ti_sdo_utils_loggers_ILoggerMFP_Interface__BASE__C;


/*
 * ======== FUNCTION STUBS ========
 */

/* delete */
static inline void ti_sdo_utils_loggers_ILoggerMFP_delete( ti_sdo_utils_loggers_ILoggerMFP_Handle* instp )
{
    (*instp)->__fxns->__sysp->__delete(instp);
}

/* Handle_to_Module */
static inline ti_sdo_utils_loggers_ILoggerMFP_Module ti_sdo_utils_loggers_ILoggerMFP_Handle_to_Module( ti_sdo_utils_loggers_ILoggerMFP_Handle inst )
{
    return inst->__fxns;
}

/* Handle_label */
static inline xdc_runtime_Types_Label* ti_sdo_utils_loggers_ILoggerMFP_Handle_label( ti_sdo_utils_loggers_ILoggerMFP_Handle inst, xdc_runtime_Types_Label* lab )
{
    return inst->__fxns->__sysp->__label(inst, lab);
}

/* Module_id */
static inline xdc_runtime_Types_ModuleId ti_sdo_utils_loggers_ILoggerMFP_Module_id( ti_sdo_utils_loggers_ILoggerMFP_Module mod )
{
    return mod->__sysp->__mid;
}

/* enable */
static inline xdc_Bool ti_sdo_utils_loggers_ILoggerMFP_enable( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst )
{
    return __inst->__fxns->enable((void*)__inst);
}

/* disable */
static inline xdc_Bool ti_sdo_utils_loggers_ILoggerMFP_disable( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst )
{
    return __inst->__fxns->disable((void*)__inst);
}

/* write0 */
static inline xdc_Void ti_sdo_utils_loggers_ILoggerMFP_write0( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid )
{
    __inst->__fxns->write0((void*)__inst, evt, mid);
}

/* write1 */
static inline xdc_Void ti_sdo_utils_loggers_ILoggerMFP_write1( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 )
{
    __inst->__fxns->write1((void*)__inst, evt, mid, a1);
}

/* write2 */
static inline xdc_Void ti_sdo_utils_loggers_ILoggerMFP_write2( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 )
{
    __inst->__fxns->write2((void*)__inst, evt, mid, a1, a2);
}

/* write4 */
static inline xdc_Void ti_sdo_utils_loggers_ILoggerMFP_write4( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 )
{
    __inst->__fxns->write4((void*)__inst, evt, mid, a1, a2, a3, a4);
}

/* write8 */
static inline xdc_Void ti_sdo_utils_loggers_ILoggerMFP_write8( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 )
{
    __inst->__fxns->write8((void*)__inst, evt, mid, a1, a2, a3, a4, a5, a6, a7, a8);
}

/* control */
static inline xdc_Void ti_sdo_utils_loggers_ILoggerMFP_control( ti_sdo_utils_loggers_ILoggerMFP_Module __inst, xdc_Int cmd, xdc_UArg cmdArgs )
{
    __inst->control(cmd, cmdArgs);
}


/*
 * ======== FUNCTION SELECTORS ========
 */

/* enable_{FxnT,fxnP} */
typedef xdc_Bool (*ti_sdo_utils_loggers_ILoggerMFP_enable_FxnT)(void*);
static inline ti_sdo_utils_loggers_ILoggerMFP_enable_FxnT ti_sdo_utils_loggers_ILoggerMFP_enable_fxnP( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst )
{
    return (ti_sdo_utils_loggers_ILoggerMFP_enable_FxnT)__inst->__fxns->enable;
}

/* disable_{FxnT,fxnP} */
typedef xdc_Bool (*ti_sdo_utils_loggers_ILoggerMFP_disable_FxnT)(void*);
static inline ti_sdo_utils_loggers_ILoggerMFP_disable_FxnT ti_sdo_utils_loggers_ILoggerMFP_disable_fxnP( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst )
{
    return (ti_sdo_utils_loggers_ILoggerMFP_disable_FxnT)__inst->__fxns->disable;
}

/* write0_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_ILoggerMFP_write0_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId);
static inline ti_sdo_utils_loggers_ILoggerMFP_write0_FxnT ti_sdo_utils_loggers_ILoggerMFP_write0_fxnP( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst )
{
    return (ti_sdo_utils_loggers_ILoggerMFP_write0_FxnT)__inst->__fxns->write0;
}

/* write1_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_ILoggerMFP_write1_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg);
static inline ti_sdo_utils_loggers_ILoggerMFP_write1_FxnT ti_sdo_utils_loggers_ILoggerMFP_write1_fxnP( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst )
{
    return (ti_sdo_utils_loggers_ILoggerMFP_write1_FxnT)__inst->__fxns->write1;
}

/* write2_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_ILoggerMFP_write2_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg);
static inline ti_sdo_utils_loggers_ILoggerMFP_write2_FxnT ti_sdo_utils_loggers_ILoggerMFP_write2_fxnP( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst )
{
    return (ti_sdo_utils_loggers_ILoggerMFP_write2_FxnT)__inst->__fxns->write2;
}

/* write4_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_ILoggerMFP_write4_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
static inline ti_sdo_utils_loggers_ILoggerMFP_write4_FxnT ti_sdo_utils_loggers_ILoggerMFP_write4_fxnP( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst )
{
    return (ti_sdo_utils_loggers_ILoggerMFP_write4_FxnT)__inst->__fxns->write4;
}

/* write8_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_ILoggerMFP_write8_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
static inline ti_sdo_utils_loggers_ILoggerMFP_write8_FxnT ti_sdo_utils_loggers_ILoggerMFP_write8_fxnP( ti_sdo_utils_loggers_ILoggerMFP_Handle __inst )
{
    return (ti_sdo_utils_loggers_ILoggerMFP_write8_FxnT)__inst->__fxns->write8;
}

/* control_{FxnT,fxnP} */
typedef xdc_Void (*ti_sdo_utils_loggers_ILoggerMFP_control_FxnT)(xdc_Int, xdc_UArg);
static inline ti_sdo_utils_loggers_ILoggerMFP_control_FxnT ti_sdo_utils_loggers_ILoggerMFP_control_fxnP( ti_sdo_utils_loggers_ILoggerMFP_Module __inst )
{
    return (ti_sdo_utils_loggers_ILoggerMFP_control_FxnT)__inst->control;
}


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline xdc_runtime_ILogger_Module ti_sdo_utils_loggers_ILoggerMFP_Module_upCast( ti_sdo_utils_loggers_ILoggerMFP_Module m )
{
    return(xdc_runtime_ILogger_Module)m;
}

/* Module_to_xdc_runtime_ILogger */
#define ti_sdo_utils_loggers_ILoggerMFP_Module_to_xdc_runtime_ILogger ti_sdo_utils_loggers_ILoggerMFP_Module_upCast

/* Module_downCast */
static inline ti_sdo_utils_loggers_ILoggerMFP_Module ti_sdo_utils_loggers_ILoggerMFP_Module_downCast( xdc_runtime_ILogger_Module m )
{
    xdc_runtime_Types_Base* b; for (b = m->__base; b; b = b->base) {
        if (b == &ti_sdo_utils_loggers_ILoggerMFP_Interface__BASE__C) return (ti_sdo_utils_loggers_ILoggerMFP_Module)m;
    } return 0;
}

/* Module_from_xdc_runtime_ILogger */
#define ti_sdo_utils_loggers_ILoggerMFP_Module_from_xdc_runtime_ILogger ti_sdo_utils_loggers_ILoggerMFP_Module_downCast

/* Handle_upCast */
static inline xdc_runtime_ILogger_Handle ti_sdo_utils_loggers_ILoggerMFP_Handle_upCast( ti_sdo_utils_loggers_ILoggerMFP_Handle i )
{
    return (xdc_runtime_ILogger_Handle)i;
}

/* Handle_to_xdc_runtime_ILogger */
#define ti_sdo_utils_loggers_ILoggerMFP_Handle_to_xdc_runtime_ILogger ti_sdo_utils_loggers_ILoggerMFP_Handle_upCast

/* Handle_downCast */
static inline ti_sdo_utils_loggers_ILoggerMFP_Handle ti_sdo_utils_loggers_ILoggerMFP_Handle_downCast( xdc_runtime_ILogger_Handle i )
{
    xdc_runtime_ILogger_Handle i2 = (xdc_runtime_ILogger_Handle)i;
    xdc_runtime_Types_Base* b; for (b = i2->__fxns->__base; b; b = b->base) {
        if (b == &ti_sdo_utils_loggers_ILoggerMFP_Interface__BASE__C) return (ti_sdo_utils_loggers_ILoggerMFP_Handle)i;
    } return 0;
}

/* Handle_from_xdc_runtime_ILogger */
#define ti_sdo_utils_loggers_ILoggerMFP_Handle_from_xdc_runtime_ILogger ti_sdo_utils_loggers_ILoggerMFP_Handle_downCast


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sdo_utils_loggers_ILoggerMFP__top__
#undef __nested__
#endif

#endif /* ti_sdo_utils_loggers_ILoggerMFP__include */


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sdo_utils_loggers_ILoggerMFP__nolocalnames)

#ifndef ti_sdo_utils_loggers_ILoggerMFP__localnames__done
#define ti_sdo_utils_loggers_ILoggerMFP__localnames__done

/* module prefix */
#define ILoggerMFP_Instance ti_sdo_utils_loggers_ILoggerMFP_Instance
#define ILoggerMFP_Handle ti_sdo_utils_loggers_ILoggerMFP_Handle
#define ILoggerMFP_Module ti_sdo_utils_loggers_ILoggerMFP_Module
#define ILoggerMFP_Cmd ti_sdo_utils_loggers_ILoggerMFP_Cmd
#define ILoggerMFP_SETTRACE ti_sdo_utils_loggers_ILoggerMFP_SETTRACE
#define ILoggerMFP_Params ti_sdo_utils_loggers_ILoggerMFP_Params
#define ILoggerMFP_enable ti_sdo_utils_loggers_ILoggerMFP_enable
#define ILoggerMFP_enable_fxnP ti_sdo_utils_loggers_ILoggerMFP_enable_fxnP
#define ILoggerMFP_enable_FxnT ti_sdo_utils_loggers_ILoggerMFP_enable_FxnT
#define ILoggerMFP_disable ti_sdo_utils_loggers_ILoggerMFP_disable
#define ILoggerMFP_disable_fxnP ti_sdo_utils_loggers_ILoggerMFP_disable_fxnP
#define ILoggerMFP_disable_FxnT ti_sdo_utils_loggers_ILoggerMFP_disable_FxnT
#define ILoggerMFP_write0 ti_sdo_utils_loggers_ILoggerMFP_write0
#define ILoggerMFP_write0_fxnP ti_sdo_utils_loggers_ILoggerMFP_write0_fxnP
#define ILoggerMFP_write0_FxnT ti_sdo_utils_loggers_ILoggerMFP_write0_FxnT
#define ILoggerMFP_write1 ti_sdo_utils_loggers_ILoggerMFP_write1
#define ILoggerMFP_write1_fxnP ti_sdo_utils_loggers_ILoggerMFP_write1_fxnP
#define ILoggerMFP_write1_FxnT ti_sdo_utils_loggers_ILoggerMFP_write1_FxnT
#define ILoggerMFP_write2 ti_sdo_utils_loggers_ILoggerMFP_write2
#define ILoggerMFP_write2_fxnP ti_sdo_utils_loggers_ILoggerMFP_write2_fxnP
#define ILoggerMFP_write2_FxnT ti_sdo_utils_loggers_ILoggerMFP_write2_FxnT
#define ILoggerMFP_write4 ti_sdo_utils_loggers_ILoggerMFP_write4
#define ILoggerMFP_write4_fxnP ti_sdo_utils_loggers_ILoggerMFP_write4_fxnP
#define ILoggerMFP_write4_FxnT ti_sdo_utils_loggers_ILoggerMFP_write4_FxnT
#define ILoggerMFP_write8 ti_sdo_utils_loggers_ILoggerMFP_write8
#define ILoggerMFP_write8_fxnP ti_sdo_utils_loggers_ILoggerMFP_write8_fxnP
#define ILoggerMFP_write8_FxnT ti_sdo_utils_loggers_ILoggerMFP_write8_FxnT
#define ILoggerMFP_control ti_sdo_utils_loggers_ILoggerMFP_control
#define ILoggerMFP_control_fxnP ti_sdo_utils_loggers_ILoggerMFP_control_fxnP
#define ILoggerMFP_control_FxnT ti_sdo_utils_loggers_ILoggerMFP_control_FxnT
#define ILoggerMFP_Module_name ti_sdo_utils_loggers_ILoggerMFP_Module_name
#define ILoggerMFP_delete ti_sdo_utils_loggers_ILoggerMFP_delete
#define ILoggerMFP_Handle_label ti_sdo_utils_loggers_ILoggerMFP_Handle_label
#define ILoggerMFP_Handle_to_Module ti_sdo_utils_loggers_ILoggerMFP_Handle_to_Module
#define ILoggerMFP_Module_upCast ti_sdo_utils_loggers_ILoggerMFP_Module_upCast
#define ILoggerMFP_Module_to_xdc_runtime_ILogger ti_sdo_utils_loggers_ILoggerMFP_Module_to_xdc_runtime_ILogger
#define ILoggerMFP_Module_downCast ti_sdo_utils_loggers_ILoggerMFP_Module_downCast
#define ILoggerMFP_Module_from_xdc_runtime_ILogger ti_sdo_utils_loggers_ILoggerMFP_Module_from_xdc_runtime_ILogger
#define ILoggerMFP_Handle_upCast ti_sdo_utils_loggers_ILoggerMFP_Handle_upCast
#define ILoggerMFP_Handle_to_xdc_runtime_ILogger ti_sdo_utils_loggers_ILoggerMFP_Handle_to_xdc_runtime_ILogger
#define ILoggerMFP_Handle_downCast ti_sdo_utils_loggers_ILoggerMFP_Handle_downCast
#define ILoggerMFP_Handle_from_xdc_runtime_ILogger ti_sdo_utils_loggers_ILoggerMFP_Handle_from_xdc_runtime_ILogger

#endif /* ti_sdo_utils_loggers_ILoggerMFP__localnames__done */
#endif
/*
 *  @(#) ti.sdo.utils.loggers; 1, 0, 0,1; 2-15-2012 07:44:55; /db/atree/library/trees/osal/osal-e09/src/ xlibrary

 */

