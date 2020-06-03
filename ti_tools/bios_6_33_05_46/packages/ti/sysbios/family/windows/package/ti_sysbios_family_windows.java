/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-y25
 */
import java.util.*;
import org.mozilla.javascript.*;
import xdc.services.intern.xsr.*;
import xdc.services.spec.Session;

public class ti_sysbios_family_windows
{
    static final String VERS = "@(#) xdc-y25\n";

    static final Proto.Elm $$T_Bool = Proto.Elm.newBool();
    static final Proto.Elm $$T_Num = Proto.Elm.newNum();
    static final Proto.Elm $$T_Str = Proto.Elm.newStr();
    static final Proto.Elm $$T_Obj = Proto.Elm.newObj();

    static final Proto.Fxn $$T_Met = new Proto.Fxn(null, null, 0, -1, false);
    static final Proto.Map $$T_Map = new Proto.Map($$T_Obj);
    static final Proto.Arr $$T_Vec = new Proto.Arr($$T_Obj);

    static final XScriptO $$DEFAULT = Value.DEFAULT;
    static final Object $$UNDEF = Undefined.instance;

    static final Proto.Obj $$Package = (Proto.Obj)Global.get("$$Package");
    static final Proto.Obj $$Module = (Proto.Obj)Global.get("$$Module");
    static final Proto.Obj $$Instance = (Proto.Obj)Global.get("$$Instance");
    static final Proto.Obj $$Params = (Proto.Obj)Global.get("$$Params");

    static final Object $$objFldGet = Global.get("$$objFldGet");
    static final Object $$objFldSet = Global.get("$$objFldSet");
    static final Object $$proxyGet = Global.get("$$proxyGet");
    static final Object $$proxySet = Global.get("$$proxySet");
    static final Object $$delegGet = Global.get("$$delegGet");
    static final Object $$delegSet = Global.get("$$delegSet");

    Scriptable xdcO;
    Session ses;
    Value.Obj om;

    boolean isROV;
    boolean isCFG;

    Proto.Obj pkgP;
    Value.Obj pkgV;

    ArrayList<Object> imports = new ArrayList<Object>();
    ArrayList<Object> loggables = new ArrayList<Object>();
    ArrayList<Object> mcfgs = new ArrayList<Object>();
    ArrayList<Object> icfgs = new ArrayList<Object>();
    ArrayList<String> inherits = new ArrayList<String>();
    ArrayList<Object> proxies = new ArrayList<Object>();
    ArrayList<Object> sizes = new ArrayList<Object>();
    ArrayList<Object> tdefs = new ArrayList<Object>();

    void $$IMPORTS()
    {
        Global.callFxn("loadPackage", xdcO, "ti.sysbios.interfaces");
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
        Global.callFxn("loadPackage", xdcO, "xdc.runtime");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.sysbios.family.windows.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sysbios.family.windows", new Value.Obj("ti.sysbios.family.windows", pkgP));
    }

    void Hwi$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.windows.Hwi.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.windows.Hwi", new Value.Obj("ti.sysbios.family.windows.Hwi", po));
        pkgV.bind("Hwi", vo);
        // decls 
        om.bind("ti.sysbios.family.windows.Hwi.HookSet", om.findStrict("ti.sysbios.interfaces.IHwi.HookSet", "ti.sysbios.family.windows"));
        om.bind("ti.sysbios.family.windows.Hwi.MaskingOption", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption", "ti.sysbios.family.windows"));
        om.bind("ti.sysbios.family.windows.Hwi.StackInfo", om.findStrict("ti.sysbios.interfaces.IHwi.StackInfo", "ti.sysbios.family.windows"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.windows.Hwi$$NonDispatchedInterrupt", new Proto.Obj());
        om.bind("ti.sysbios.family.windows.Hwi.NonDispatchedInterrupt", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.windows.Hwi$$InterruptObj", new Proto.Obj());
        om.bind("ti.sysbios.family.windows.Hwi.InterruptObj", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.windows.Hwi$$Instance_State", new Proto.Obj());
        om.bind("ti.sysbios.family.windows.Hwi.Instance_State", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.windows.Hwi$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.windows.Hwi.Module_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sysbios.family.windows.Hwi.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sysbios.family.windows.Hwi$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sysbios.family.windows.Hwi.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sysbios.family.windows.Hwi$$Params", new Proto.Obj());
        om.bind("ti.sysbios.family.windows.Hwi.Params", new Proto.Str(po, false));
        om.bind("ti.sysbios.family.windows.Hwi.Handle", insP);
        if (isROV) {
            om.bind("ti.sysbios.family.windows.Hwi.Object", om.findStrict("ti.sysbios.family.windows.Hwi.Instance_State", "ti.sysbios.family.windows"));
        }//isROV
    }

    void IntrinsicsSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.windows.IntrinsicsSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.windows.IntrinsicsSupport", new Value.Obj("ti.sysbios.family.windows.IntrinsicsSupport", po));
        pkgV.bind("IntrinsicsSupport", vo);
        // decls 
    }

    void Settings$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.windows.Settings.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.windows.Settings", new Value.Obj("ti.sysbios.family.windows.Settings", po));
        pkgV.bind("Settings", vo);
        // decls 
    }

    void TaskSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.windows.TaskSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.windows.TaskSupport", new Value.Obj("ti.sysbios.family.windows.TaskSupport", po));
        pkgV.bind("TaskSupport", vo);
        // decls 
        om.bind("ti.sysbios.family.windows.TaskSupport.State", new Proto.Enm("ti.sysbios.family.windows.TaskSupport.State"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.windows.TaskSupport$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.windows.TaskSupport.Module_State", new Proto.Str(spo, false));
    }

    void Timer$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.windows.Timer.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.windows.Timer", new Value.Obj("ti.sysbios.family.windows.Timer", po));
        pkgV.bind("Timer", vo);
        // decls 
        om.bind("ti.sysbios.family.windows.Timer.StartMode", om.findStrict("ti.sysbios.interfaces.ITimer.StartMode", "ti.sysbios.family.windows"));
        om.bind("ti.sysbios.family.windows.Timer.RunMode", om.findStrict("ti.sysbios.interfaces.ITimer.RunMode", "ti.sysbios.family.windows"));
        om.bind("ti.sysbios.family.windows.Timer.Status", om.findStrict("ti.sysbios.interfaces.ITimer.Status", "ti.sysbios.family.windows"));
        om.bind("ti.sysbios.family.windows.Timer.PeriodType", om.findStrict("ti.sysbios.interfaces.ITimer.PeriodType", "ti.sysbios.family.windows"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.windows.Timer$$Instance_State", new Proto.Obj());
        om.bind("ti.sysbios.family.windows.Timer.Instance_State", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.windows.Timer$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.windows.Timer.Module_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sysbios.family.windows.Timer.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sysbios.family.windows.Timer$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sysbios.family.windows.Timer.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sysbios.family.windows.Timer$$Params", new Proto.Obj());
        om.bind("ti.sysbios.family.windows.Timer.Params", new Proto.Str(po, false));
        om.bind("ti.sysbios.family.windows.Timer.Handle", insP);
        if (isROV) {
            om.bind("ti.sysbios.family.windows.Timer.Object", om.findStrict("ti.sysbios.family.windows.Timer.Instance_State", "ti.sysbios.family.windows"));
        }//isROV
    }

    void TimestampProvider$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.windows.TimestampProvider.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.windows.TimestampProvider", new Value.Obj("ti.sysbios.family.windows.TimestampProvider", po));
        pkgV.bind("TimestampProvider", vo);
        // decls 
    }

    void Hwi$$CONSTS()
    {
        // module Hwi
        om.bind("ti.sysbios.family.windows.Hwi.NUM_INTERRUPTS", 16L);
        om.bind("ti.sysbios.family.windows.Hwi.getStackInfo", new Extern("ti_sysbios_family_windows_Hwi_getStackInfo__E", "xdc_Bool(*)(ti_sysbios_interfaces_IHwi_StackInfo*,xdc_Bool)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.startup", new Extern("ti_sysbios_family_windows_Hwi_startup__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.disable", new Extern("ti_sysbios_family_windows_Hwi_disable__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.enable", new Extern("ti_sysbios_family_windows_Hwi_enable__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.restore", new Extern("ti_sysbios_family_windows_Hwi_restore__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.switchFromBootStack", new Extern("ti_sysbios_family_windows_Hwi_switchFromBootStack__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.post", new Extern("ti_sysbios_family_windows_Hwi_post__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.getTaskSP", new Extern("ti_sysbios_family_windows_Hwi_getTaskSP__E", "xdc_Char*(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.disableInterrupt", new Extern("ti_sysbios_family_windows_Hwi_disableInterrupt__E", "xdc_UInt(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.enableInterrupt", new Extern("ti_sysbios_family_windows_Hwi_enableInterrupt__E", "xdc_UInt(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.restoreInterrupt", new Extern("ti_sysbios_family_windows_Hwi_restoreInterrupt__E", "xdc_Void(*)(xdc_UInt,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.clearInterrupt", new Extern("ti_sysbios_family_windows_Hwi_clearInterrupt__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.disableIER", new Extern("ti_sysbios_family_windows_Hwi_disableIER__E", "xdc_Bits16(*)(xdc_Bits16)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.enableIER", new Extern("ti_sysbios_family_windows_Hwi_enableIER__E", "xdc_Bits16(*)(xdc_Bits16)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.restoreIER", new Extern("ti_sysbios_family_windows_Hwi_restoreIER__E", "xdc_Bits16(*)(xdc_Bits16)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.getHandle", new Extern("ti_sysbios_family_windows_Hwi_getHandle__E", "ti_sysbios_family_windows_Hwi_Handle(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.plug", new Extern("ti_sysbios_family_windows_Hwi_plug__E", "xdc_Void(*)(xdc_UInt,xdc_Void(*)(xdc_UInt))", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.dispatch", new Extern("ti_sysbios_family_windows_Hwi_dispatch__I", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.init", new Extern("ti_sysbios_family_windows_Hwi_init__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.Hwi.unPluggedInterrupt", new Extern("ti_sysbios_family_windows_Hwi_unPluggedInterrupt__I", "xdc_Void(*)(xdc_UInt)", true, false));
    }

    void IntrinsicsSupport$$CONSTS()
    {
        // module IntrinsicsSupport
        om.bind("ti.sysbios.family.windows.IntrinsicsSupport.maxbit", new Extern("ti_sysbios_family_windows_IntrinsicsSupport_maxbit__E", "xdc_UInt(*)(xdc_UInt)", true, false));
    }

    void Settings$$CONSTS()
    {
        // module Settings
    }

    void TaskSupport$$CONSTS()
    {
        // module TaskSupport
        om.bind("ti.sysbios.family.windows.TaskSupport.State_PREEMPTED", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.windows.TaskSupport.State", "ti.sysbios.family.windows"), "ti.sysbios.family.windows.TaskSupport.State_PREEMPTED", 0));
        om.bind("ti.sysbios.family.windows.TaskSupport.State_RUNNING", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.windows.TaskSupport.State", "ti.sysbios.family.windows"), "ti.sysbios.family.windows.TaskSupport.State_RUNNING", 1));
        om.bind("ti.sysbios.family.windows.TaskSupport.State_TERMINATING", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.windows.TaskSupport.State", "ti.sysbios.family.windows"), "ti.sysbios.family.windows.TaskSupport.State_TERMINATING", 2));
        om.bind("ti.sysbios.family.windows.TaskSupport.State_YIELDED", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.windows.TaskSupport.State", "ti.sysbios.family.windows"), "ti.sysbios.family.windows.TaskSupport.State_YIELDED", 3));
        om.bind("ti.sysbios.family.windows.TaskSupport.start", new Extern("ti_sysbios_family_windows_TaskSupport_start__E", "xdc_Ptr(*)(xdc_Ptr,xdc_Void(*)(xdc_Void),xdc_Void(*)(xdc_Void),xdc_runtime_Error_Block*)", true, false));
        om.bind("ti.sysbios.family.windows.TaskSupport.swap", new Extern("ti_sysbios_family_windows_TaskSupport_swap__E", "xdc_Void(*)(xdc_Ptr*,xdc_Ptr*)", true, false));
        om.bind("ti.sysbios.family.windows.TaskSupport.checkStack", new Extern("ti_sysbios_family_windows_TaskSupport_checkStack__E", "xdc_Bool(*)(xdc_Char*,xdc_SizeT)", true, false));
        om.bind("ti.sysbios.family.windows.TaskSupport.stackUsed", new Extern("ti_sysbios_family_windows_TaskSupport_stackUsed__E", "xdc_SizeT(*)(xdc_Char*,xdc_SizeT)", true, false));
        om.bind("ti.sysbios.family.windows.TaskSupport.getStackAlignment", new Extern("ti_sysbios_family_windows_TaskSupport_getStackAlignment__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.TaskSupport.getDefaultStackSize", new Extern("ti_sysbios_family_windows_TaskSupport_getDefaultStackSize__E", "xdc_SizeT(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.TaskSupport.enterCS", new Extern("ti_sysbios_family_windows_TaskSupport_enterCS__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.TaskSupport.leaveCS", new Extern("ti_sysbios_family_windows_TaskSupport_leaveCS__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.TaskSupport.processInterrupt", new Extern("ti_sysbios_family_windows_TaskSupport_processInterrupt__I", "xdc_Void(*)(xdc_Void(*)(xdc_UInt),xdc_UInt)", true, false));
    }

    void Timer$$CONSTS()
    {
        // module Timer
        om.bind("ti.sysbios.family.windows.Timer.NUM_TIMER_DEVICES", 2L);
        om.bind("ti.sysbios.family.windows.Timer.MAX_PERIOD", 0xffffffffL);
        om.bind("ti.sysbios.family.windows.Timer.getNumTimers", new Extern("ti_sysbios_family_windows_Timer_getNumTimers__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.Timer.getStatus", new Extern("ti_sysbios_family_windows_Timer_getStatus__E", "ti_sysbios_interfaces_ITimer_Status(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.windows.Timer.startup", new Extern("ti_sysbios_family_windows_Timer_startup__E", "xdc_Void(*)(xdc_Void)", true, false));
    }

    void TimestampProvider$$CONSTS()
    {
        // module TimestampProvider
        om.bind("ti.sysbios.family.windows.TimestampProvider.get32", new Extern("ti_sysbios_family_windows_TimestampProvider_get32__E", "xdc_Bits32(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.windows.TimestampProvider.get64", new Extern("ti_sysbios_family_windows_TimestampProvider_get64__E", "xdc_Void(*)(xdc_runtime_Types_Timestamp64*)", true, false));
        om.bind("ti.sysbios.family.windows.TimestampProvider.getFreq", new Extern("ti_sysbios_family_windows_TimestampProvider_getFreq__E", "xdc_Void(*)(xdc_runtime_Types_FreqHz*)", true, false));
    }

    void Hwi$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$windows$Hwi$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sysbios.family.windows.Hwi.create() called before xdc.useModule('ti.sysbios.family.windows.Hwi')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst.$args.intNum, inst.$args.hwiFxn, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sysbios.family.windows.Hwi$$create", new Proto.Fxn(om.findStrict("ti.sysbios.family.windows.Hwi.Module", "ti.sysbios.family.windows"), om.findStrict("ti.sysbios.family.windows.Hwi.Instance", "ti.sysbios.family.windows"), 3, 2, false));
                        fxn.addArg(0, "intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(1, "hwiFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF);
                        fxn.addArg(2, "__params", (Proto)om.findStrict("ti.sysbios.family.windows.Hwi.Params", "ti.sysbios.family.windows"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$windows$Hwi$$create = function( intNum, hwiFxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sysbios.family.windows.Hwi'];\n");
                sb.append("var __inst = xdc.om['ti.sysbios.family.windows.Hwi.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sysbios.family.windows']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {intNum:intNum, hwiFxn:hwiFxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sysbios.family.windows.Hwi'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [intNum, hwiFxn]);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sysbios.family.windows.Hwi'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sysbios.family.windows.Hwi$$construct", new Proto.Fxn(om.findStrict("ti.sysbios.family.windows.Hwi.Module", "ti.sysbios.family.windows"), null, 4, 2, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sysbios.family.windows.Hwi$$Object", "ti.sysbios.family.windows"), null);
                        fxn.addArg(1, "intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(2, "hwiFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF);
                        fxn.addArg(3, "__params", (Proto)om.findStrict("ti.sysbios.family.windows.Hwi.Params", "ti.sysbios.family.windows"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$windows$Hwi$$construct = function( __obj, intNum, hwiFxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sysbios.family.windows.Hwi'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {intNum:intNum, hwiFxn:hwiFxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sysbios.family.windows.Hwi'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sysbios.family.windows.Hwi'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void IntrinsicsSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Settings$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void TaskSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Timer$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$windows$Timer$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sysbios.family.windows.Timer.create() called before xdc.useModule('ti.sysbios.family.windows.Timer')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst.$args.id, inst.$args.tickFxn, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sysbios.family.windows.Timer$$create", new Proto.Fxn(om.findStrict("ti.sysbios.family.windows.Timer.Module", "ti.sysbios.family.windows"), om.findStrict("ti.sysbios.family.windows.Timer.Instance", "ti.sysbios.family.windows"), 3, 2, false));
                        fxn.addArg(0, "id", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(1, "tickFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF);
                        fxn.addArg(2, "__params", (Proto)om.findStrict("ti.sysbios.family.windows.Timer.Params", "ti.sysbios.family.windows"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$windows$Timer$$create = function( id, tickFxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sysbios.family.windows.Timer'];\n");
                sb.append("var __inst = xdc.om['ti.sysbios.family.windows.Timer.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sysbios.family.windows']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {id:id, tickFxn:tickFxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sysbios.family.windows.Timer'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [id, tickFxn]);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sysbios.family.windows.Timer'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sysbios.family.windows.Timer$$construct", new Proto.Fxn(om.findStrict("ti.sysbios.family.windows.Timer.Module", "ti.sysbios.family.windows"), null, 4, 2, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sysbios.family.windows.Timer$$Object", "ti.sysbios.family.windows"), null);
                        fxn.addArg(1, "id", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(2, "tickFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF);
                        fxn.addArg(3, "__params", (Proto)om.findStrict("ti.sysbios.family.windows.Timer.Params", "ti.sysbios.family.windows"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$windows$Timer$$construct = function( __obj, id, tickFxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sysbios.family.windows.Timer'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {id:id, tickFxn:tickFxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sysbios.family.windows.Timer'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sysbios.family.windows.Timer'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void TimestampProvider$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Hwi$$FUNCTIONS()
    {
        Proto.Fxn fxn;

        // fxn Hwi.inUseMeta
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.windows.Hwi$$inUseMeta", new Proto.Fxn(om.findStrict("ti.sysbios.family.windows.Hwi.Module", "ti.sysbios.family.windows"), $$T_Bool, 1, 1, false));
                fxn.addArg(0, "intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF);
        // fxn Hwi.plugMeta
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.windows.Hwi$$plugMeta", new Proto.Fxn(om.findStrict("ti.sysbios.family.windows.Hwi.Module", "ti.sysbios.family.windows"), null, 2, 2, false));
                fxn.addArg(0, "intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF);
                fxn.addArg(1, "fxn", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"), $$UNDEF);
    }

    void IntrinsicsSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Settings$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TaskSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Timer$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TimestampProvider$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Hwi$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.windows.Hwi.Instance_State", "ti.sysbios.family.windows");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("disableMask", "UInt16"));
        sizes.add(Global.newArray("restoreMask", "UInt16"));
        sizes.add(Global.newArray("arg", "UIArg"));
        sizes.add(Global.newArray("fxn", "UFxn"));
        sizes.add(Global.newArray("irp", "UIArg"));
        sizes.add(Global.newArray("hookEnv", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.windows.Hwi.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.windows.Hwi.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.windows.Hwi.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.windows.Hwi.Module_State", "ti.sysbios.family.windows");
        sizes.clear();
        sizes.add(Global.newArray("ierMask", "UInt16"));
        sizes.add(Global.newArray("dispatchTable", "A16;UPtr"));
        sizes.add(Global.newArray("GIE", "UShort"));
        sizes.add(Global.newArray("IER", "UInt16"));
        sizes.add(Global.newArray("IFR", "UInt16"));
        sizes.add(Global.newArray("intrNum", "UInt"));
        sizes.add(Global.newArray("IST", "A16;UFxn"));
        sizes.add(Global.newArray("privateData", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.windows.Hwi.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.windows.Hwi.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.windows.Hwi.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void IntrinsicsSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void Settings$$SIZES()
    {
    }

    void TaskSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.windows.TaskSupport.Module_State", "ti.sysbios.family.windows");
        sizes.clear();
        sizes.add(Global.newArray("currentTask", "UPtr"));
        sizes.add(Global.newArray("privateData", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.windows.TaskSupport.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.windows.TaskSupport.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.windows.TaskSupport.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void Timer$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.windows.Timer.Instance_State", "ti.sysbios.family.windows");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("id", "TInt"));
        sizes.add(Global.newArray("runMode", "Nti.sysbios.interfaces.ITimer.RunMode;;;;"));
        sizes.add(Global.newArray("startMode", "Nti.sysbios.interfaces.ITimer.StartMode;;;"));
        sizes.add(Global.newArray("period", "UInt"));
        sizes.add(Global.newArray("periodType", "Nti.sysbios.interfaces.ITimer.PeriodType;;;"));
        sizes.add(Global.newArray("extFreq", "Sxdc.runtime.Types;FreqHz"));
        sizes.add(Global.newArray("intNum", "UInt"));
        sizes.add(Global.newArray("tickFxn", "UFxn"));
        sizes.add(Global.newArray("tickFxnArg", "UIArg"));
        sizes.add(Global.newArray("hwi", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.windows.Timer.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.windows.Timer.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.windows.Timer.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.windows.Timer.Module_State", "ti.sysbios.family.windows");
        sizes.clear();
        sizes.add(Global.newArray("availMask", "TChar"));
        sizes.add(Global.newArray("handles", "A2;UPtr"));
        sizes.add(Global.newArray("privateData", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.windows.Timer.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.windows.Timer.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.windows.Timer.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void TimestampProvider$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void Hwi$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/windows/Hwi.xs");
        om.bind("ti.sysbios.family.windows.Hwi$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi.Module", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Hwi.Module", om.findStrict("ti.sysbios.interfaces.IHwi.Module", "ti.sysbios.family.windows"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("NUM_INTERRUPTS", Proto.Elm.newCNum("(xdc_Int)"), 16L, "rh");
        if (isCFG) {
            po.addFld("A_interNum", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "A_interNum: interrupt number must be less then NUM_INTERRUPTS"), "w");
            po.addFld("A_noPendingIntr", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "A_noPendingIntr: there must be a pending interrupt"), "w");
            po.addFld("A_pluggedIntr", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "interrupt must be plugged"), "w");
            po.addFld("E_calloc", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "calloc failed"), "w");
            po.addFld("E_event", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "CreateEvent failed"), "w");
            po.addFld("E_alreadyDefined", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "E_alreadyDefined: Hwi already defined: intr# %d"), "w");
            po.addFld("LM_begin", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sysbios.family.windows"), Global.newObject("mask", Global.eval("0x0100 | 0x0200"), "msg", "LM_begin: hwi: 0x%x, func: 0x%x, preThread: %d, intNum: %d"), "w");
            po.addFld("LD_end", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sysbios.family.windows"), Global.newObject("mask", 0x0200L, "msg", "LD_end: hwi: 0x%x"), "w");
            po.addFld("LW_missedIntr", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sysbios.family.windows"), Global.newObject("mask", 0x0400L, "msg", "Warning: LW_missedIntr: missed interrupt detected: %d"), "w");
            po.addFld("nonDispatchedInterrupts", new Proto.Map((Proto)om.findStrict("ti.sysbios.family.windows.Hwi.NonDispatchedInterrupt", "ti.sysbios.family.windows")), $$DEFAULT, "wh");
            po.addFld("swiDisable", new Proto.Adr("xdc_UInt(*)(xdc_Void)", "PFn"), $$UNDEF, "w");
            po.addFld("swiRestoreHwi", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"), $$UNDEF, "w");
            po.addFld("taskDisable", new Proto.Adr("xdc_UInt(*)(xdc_Void)", "PFn"), $$UNDEF, "w");
            po.addFld("taskRestoreHwi", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"), $$UNDEF, "w");
            po.addFld("hooks", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.windows.Hwi.HookSet", "ti.sysbios.family.windows"), true), Global.newArray(new Object[]{}), "w");
            po.addFld("interrupt", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.windows.Hwi.InterruptObj", "ti.sysbios.family.windows"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "wh");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sysbios.family.windows.Hwi$$create", "ti.sysbios.family.windows"), Global.get("ti$sysbios$family$windows$Hwi$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sysbios.family.windows.Hwi$$construct", "ti.sysbios.family.windows"), Global.get("ti$sysbios$family$windows$Hwi$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Hwi$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Hwi$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Hwi$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Hwi$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Hwi$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Hwi$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
                fxn = Global.get(cap, "addHookSet");
                if (fxn != null) po.addFxn("addHookSet", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.IHwi$$addHookSet", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "viewGetStackInfo");
                if (fxn != null) po.addFxn("viewGetStackInfo", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.IHwi$$viewGetStackInfo", "ti.sysbios.family.windows"), fxn);
                po.addFxn("inUseMeta", (Proto.Fxn)om.findStrict("ti.sysbios.family.windows.Hwi$$inUseMeta", "ti.sysbios.family.windows"), Global.get(cap, "inUseMeta"));
                po.addFxn("plugMeta", (Proto.Fxn)om.findStrict("ti.sysbios.family.windows.Hwi$$plugMeta", "ti.sysbios.family.windows"), Global.get(cap, "plugMeta"));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi.Instance", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Hwi.Instance", om.findStrict("ti.sysbios.interfaces.IHwi.Instance", "ti.sysbios.family.windows"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("NUM_INTERRUPTS", Proto.Elm.newCNum("(xdc_Int)"), 16L, "rh");
        if (isCFG) {
            po.addFld("disableMask", Proto.Elm.newCNum("(xdc_Bits16)"), 0L, "w");
            po.addFld("restoreMask", Proto.Elm.newCNum("(xdc_Bits16)"), 0L, "w");
            po.addFld("priority", Proto.Elm.newCNum("(xdc_Int)"), 0L, "w");
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sysbios.family.windows"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi$$Params", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Hwi.Params", om.findStrict("ti.sysbios.interfaces.IHwi$$Params", "ti.sysbios.family.windows"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("NUM_INTERRUPTS", Proto.Elm.newCNum("(xdc_Int)"), 16L, "rh");
        if (isCFG) {
            po.addFld("disableMask", Proto.Elm.newCNum("(xdc_Bits16)"), 0L, "w");
            po.addFld("restoreMask", Proto.Elm.newCNum("(xdc_Bits16)"), 0L, "w");
            po.addFld("priority", Proto.Elm.newCNum("(xdc_Int)"), 0L, "w");
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sysbios.family.windows"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi$$Object", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Hwi.Object", om.findStrict("ti.sysbios.family.windows.Hwi.Instance", "ti.sysbios.family.windows"));
        // typedef Hwi.PlugFuncPtr
        om.bind("ti.sysbios.family.windows.Hwi.PlugFuncPtr", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"));
        // typedef Hwi.IsrFxn
        om.bind("ti.sysbios.family.windows.Hwi.IsrFxn", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"));
        // struct Hwi.NonDispatchedInterrupt
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi$$NonDispatchedInterrupt", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Hwi.NonDispatchedInterrupt", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("fxn", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"), $$UNDEF, "w");
                po.addFld("enableInt", $$T_Bool, $$UNDEF, "w");
                po.addFld("eventId", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
        // struct Hwi.InterruptObj
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi$$InterruptObj", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Hwi.InterruptObj", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("name", $$T_Str, $$UNDEF, "w");
                po.addFld("used", $$T_Bool, $$UNDEF, "w");
                po.addFld("useDispatcher", $$T_Bool, $$UNDEF, "w");
                po.addFld("fxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF, "w");
                po.addFld("pfxn", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"), $$UNDEF, "w");
        // struct Hwi.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi$$Instance_State", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Hwi.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("disableMask", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("restoreMask", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("arg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("fxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF, "w");
                po.addFld("irp", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("hookEnv", new Proto.Arr(new Proto.Adr("xdc_Ptr", "Pv"), false), $$DEFAULT, "w");
        // struct Hwi.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi$$Module_State", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Hwi.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("ierMask", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("dispatchTable", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.windows.Hwi.Handle", "ti.sysbios.family.windows"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("GIE", $$T_Bool, $$UNDEF, "w");
                po.addFld("IER", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("IFR", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("intrNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("IST", new Proto.Arr(new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("privateData", new Proto.Adr("ti_sysbios_family_windows_Hwi_PrivateData*", "PE"), $$UNDEF, "w");
    }

    void IntrinsicsSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.IntrinsicsSupport.Module", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.IntrinsicsSupport.Module", om.findStrict("ti.sysbios.interfaces.IIntrinsicsSupport.Module", "ti.sysbios.family.windows"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
    }

    void Settings$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/windows/Settings.xs");
        om.bind("ti.sysbios.family.windows.Settings$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Settings.Module", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Settings.Module", om.findStrict("ti.sysbios.interfaces.ISettings.Module", "ti.sysbios.family.windows"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Settings$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Settings$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Settings$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "getDefaultHwiDelegate");
                if (fxn != null) po.addFxn("getDefaultHwiDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultHwiDelegate", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getDefaultTimerDelegate");
                if (fxn != null) po.addFxn("getDefaultTimerDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTimerDelegate", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getDefaultTimerSupportDelegate");
                if (fxn != null) po.addFxn("getDefaultTimerSupportDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTimerSupportDelegate", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getDefaultTimestampDelegate");
                if (fxn != null) po.addFxn("getDefaultTimestampDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTimestampDelegate", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getDefaultTaskSupportDelegate");
                if (fxn != null) po.addFxn("getDefaultTaskSupportDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTaskSupportDelegate", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getDefaultIntrinsicsSupportDelegate");
                if (fxn != null) po.addFxn("getDefaultIntrinsicsSupportDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultIntrinsicsSupportDelegate", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getDefaultCacheDelegate");
                if (fxn != null) po.addFxn("getDefaultCacheDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultCacheDelegate", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getDefaultBootModule");
                if (fxn != null) po.addFxn("getDefaultBootModule", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultBootModule", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getDefaultMmuModule");
                if (fxn != null) po.addFxn("getDefaultMmuModule", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultMmuModule", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getFamilySettingsXml");
                if (fxn != null) po.addFxn("getFamilySettingsXml", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getFamilySettingsXml", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getDefaultClockTickPeriod");
                if (fxn != null) po.addFxn("getDefaultClockTickPeriod", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultClockTickPeriod", "ti.sysbios.family.windows"), fxn);
    }

    void TaskSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/windows/TaskSupport.xs");
        om.bind("ti.sysbios.family.windows.TaskSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.TaskSupport.Module", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.TaskSupport.Module", om.findStrict("ti.sysbios.interfaces.ITaskSupport.Module", "ti.sysbios.family.windows"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("E_calloc", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "calloc failed"), "w");
            po.addFld("E_event", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "CreateEvent failed"), "w");
            po.addFld("E_taskState", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "invalid incoming task state"), "w");
            po.addFld("E_thread", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "CreateThread failed"), "w");
            po.addFld("E_wait", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "WaitForSingleObject failed"), "w");
            po.addFld("defaultStackSize", Proto.Elm.newCNum("(xdc_SizeT)"), 128L, "r");
            po.addFld("stackAlignment", Proto.Elm.newCNum("(xdc_UInt)"), 0L, "r");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.windows.TaskSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.TaskSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.TaskSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.windows.TaskSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "stackUsed$view");
                if (fxn != null) po.addFxn("stackUsed$view", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITaskSupport$$stackUsed$view", "ti.sysbios.family.windows"), fxn);
        // typedef TaskSupport.GlueFxn
        om.bind("ti.sysbios.family.windows.TaskSupport.GlueFxn", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"));
        // typedef TaskSupport.TaskFxn
        om.bind("ti.sysbios.family.windows.TaskSupport.TaskFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg,xdc_UArg)", "PFv"));
        // typedef TaskSupport.IsrFxn
        om.bind("ti.sysbios.family.windows.TaskSupport.IsrFxn", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"));
        // struct TaskSupport.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.TaskSupport$$Module_State", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.TaskSupport.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("currentTask", new Proto.Adr("ti_sysbios_family_windows_TaskSupport_Task*", "PE"), $$UNDEF, "w");
                po.addFld("privateData", new Proto.Adr("ti_sysbios_family_windows_TaskSupport_PrivateData*", "PE"), $$UNDEF, "w");
    }

    void Timer$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/windows/Timer.xs");
        om.bind("ti.sysbios.family.windows.Timer$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Timer.Module", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Timer.Module", om.findStrict("ti.sysbios.interfaces.ITimer.Module", "ti.sysbios.family.windows"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("ANY", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~0"), "rh");
                po.addFld("NUM_TIMER_DEVICES", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("MAX_PERIOD", Proto.Elm.newCNum("(xdc_UInt)"), 0xffffffffL, "rh");
        if (isCFG) {
            po.addFld("E_calloc", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "calloc failed"), "w");
            po.addFld("E_cannotSupport", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "Timer cannot support requested period %d"), "w");
            po.addFld("E_event", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "CreateEvent failed"), "w");
            po.addFld("E_invalidTimer", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "E_invalidTimer: Invalid Timer Id %d"), "w");
            po.addFld("E_noneAvailable", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "no available timer"), "w");
            po.addFld("E_notSupported", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "API not supported on Windows"), "w");
            po.addFld("E_thread", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "CreateThread failed"), "w");
            po.addFld("E_notAvailable", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "E_notAvailable: Timer not available %d"), "w");
            po.addFld("E_wait", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "WaitForSingleObject failed"), "w");
            po.addFld("anyMask", Proto.Elm.newCNum("(xdc_UInt)"), 0x3L, "w");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sysbios.family.windows.Timer$$create", "ti.sysbios.family.windows"), Global.get("ti$sysbios$family$windows$Timer$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sysbios.family.windows.Timer$$construct", "ti.sysbios.family.windows"), Global.get("ti$sysbios$family$windows$Timer$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Timer$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Timer$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Timer$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Timer$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Timer$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.Timer$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Timer.Instance", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Timer.Instance", om.findStrict("ti.sysbios.interfaces.ITimer.Instance", "ti.sysbios.family.windows"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("ANY", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~0"), "rh");
                po.addFld("NUM_TIMER_DEVICES", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("MAX_PERIOD", Proto.Elm.newCNum("(xdc_UInt)"), 0xffffffffL, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sysbios.family.windows"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Timer$$Params", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Timer.Params", om.findStrict("ti.sysbios.interfaces.ITimer$$Params", "ti.sysbios.family.windows"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("ANY", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~0"), "rh");
                po.addFld("NUM_TIMER_DEVICES", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("MAX_PERIOD", Proto.Elm.newCNum("(xdc_UInt)"), 0xffffffffL, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sysbios.family.windows"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Timer$$Object", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Timer.Object", om.findStrict("ti.sysbios.family.windows.Timer.Instance", "ti.sysbios.family.windows"));
        // struct Timer.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Timer$$Instance_State", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Timer.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("id", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("runMode", (Proto)om.findStrict("ti.sysbios.interfaces.ITimer.RunMode", "ti.sysbios.family.windows"), $$UNDEF, "w");
                po.addFld("startMode", (Proto)om.findStrict("ti.sysbios.interfaces.ITimer.StartMode", "ti.sysbios.family.windows"), $$UNDEF, "w");
                po.addFld("period", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("periodType", (Proto)om.findStrict("ti.sysbios.interfaces.ITimer.PeriodType", "ti.sysbios.family.windows"), $$UNDEF, "w");
                po.addFld("extFreq", (Proto)om.findStrict("xdc.runtime.Types.FreqHz", "ti.sysbios.family.windows"), $$DEFAULT, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("tickFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF, "w");
                po.addFld("tickFxnArg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("hwi", (Proto)om.findStrict("ti.sysbios.family.windows.Hwi.Handle", "ti.sysbios.family.windows"), $$UNDEF, "w");
        // struct Timer.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Timer$$Module_State", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.Timer.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("availMask", Proto.Elm.newCNum("(xdc_Char)"), $$UNDEF, "w");
                po.addFld("handles", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.windows.Timer.Handle", "ti.sysbios.family.windows"), false, xdc.services.intern.xsr.Enum.intValue(2L)), $$DEFAULT, "w");
                po.addFld("privateData", new Proto.Adr("ti_sysbios_family_windows_Timer_PrivateData*", "PE"), $$UNDEF, "w");
    }

    void TimestampProvider$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/windows/TimestampProvider.xs");
        om.bind("ti.sysbios.family.windows.TimestampProvider$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.TimestampProvider.Module", "ti.sysbios.family.windows");
        po.init("ti.sysbios.family.windows.TimestampProvider.Module", om.findStrict("xdc.runtime.ITimestampProvider.Module", "ti.sysbios.family.windows"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("E_processTimes", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.windows"), Global.newObject("msg", "GetProcessTimes failed: %d"), "w");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.windows.TimestampProvider$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.TimestampProvider$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.windows.TimestampProvider$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.windows.TimestampProvider$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void Hwi$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.Hwi", "ti.sysbios.family.windows");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi$$Instance_State", "ti.sysbios.family.windows");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.windows.Hwi.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi$$Instance_State", "ti.sysbios.family.windows");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.windows.Hwi.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi$$Module_State", "ti.sysbios.family.windows");
    }

    void IntrinsicsSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.IntrinsicsSupport", "ti.sysbios.family.windows");
    }

    void Settings$$ROV()
    {
    }

    void TaskSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.TaskSupport", "ti.sysbios.family.windows");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.windows.TaskSupport.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.TaskSupport$$Module_State", "ti.sysbios.family.windows");
    }

    void Timer$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.Timer", "ti.sysbios.family.windows");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Timer$$Instance_State", "ti.sysbios.family.windows");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.windows.Timer.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Timer$$Instance_State", "ti.sysbios.family.windows");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.windows.Timer.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Timer$$Module_State", "ti.sysbios.family.windows");
    }

    void TimestampProvider$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.TimestampProvider", "ti.sysbios.family.windows");
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sysbios.family.windows.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sysbios.family.windows"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/windows/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sysbios.family.windows"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sysbios.family.windows"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sysbios.family.windows", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sysbios.family.windows");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sysbios.family.windows.");
        pkgV.bind("$vers", Global.newArray("2, 0, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("ti.sysbios.interfaces", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sysbios.family.windows'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/debug/ti.sysbios.family.windows.a86W',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/debug/ti.sysbios.family.windows.a86W', {target: 'microsoft.targets.Win32', suffix: '86W'}],\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void Hwi$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.Hwi", "ti.sysbios.family.windows");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Hwi.Module", "ti.sysbios.family.windows");
        vo.init2(po, "ti.sysbios.family.windows.Hwi", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.windows.Hwi$$capsule", "ti.sysbios.family.windows"));
        vo.bind("Instance", om.findStrict("ti.sysbios.family.windows.Hwi.Instance", "ti.sysbios.family.windows"));
        vo.bind("Params", om.findStrict("ti.sysbios.family.windows.Hwi.Params", "ti.sysbios.family.windows"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sysbios.family.windows.Hwi.Params", "ti.sysbios.family.windows")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sysbios.family.windows.Hwi.Handle", "ti.sysbios.family.windows"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.windows", "ti.sysbios.family.windows"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        mcfgs.add("Module__diagsEnabled");
        icfgs.add("Module__diagsEnabled");
        mcfgs.add("Module__diagsIncluded");
        icfgs.add("Module__diagsIncluded");
        mcfgs.add("Module__diagsMask");
        icfgs.add("Module__diagsMask");
        mcfgs.add("Module__gateObj");
        icfgs.add("Module__gateObj");
        mcfgs.add("Module__gatePrms");
        icfgs.add("Module__gatePrms");
        mcfgs.add("Module__id");
        icfgs.add("Module__id");
        mcfgs.add("Module__loggerDefined");
        icfgs.add("Module__loggerDefined");
        mcfgs.add("Module__loggerObj");
        icfgs.add("Module__loggerObj");
        mcfgs.add("Module__loggerFxn0");
        icfgs.add("Module__loggerFxn0");
        mcfgs.add("Module__loggerFxn1");
        icfgs.add("Module__loggerFxn1");
        mcfgs.add("Module__loggerFxn2");
        icfgs.add("Module__loggerFxn2");
        mcfgs.add("Module__loggerFxn4");
        icfgs.add("Module__loggerFxn4");
        mcfgs.add("Module__loggerFxn8");
        icfgs.add("Module__loggerFxn8");
        mcfgs.add("Module__startupDoneFxn");
        icfgs.add("Module__startupDoneFxn");
        mcfgs.add("Object__count");
        icfgs.add("Object__count");
        mcfgs.add("Object__heap");
        icfgs.add("Object__heap");
        mcfgs.add("Object__sizeof");
        icfgs.add("Object__sizeof");
        mcfgs.add("Object__table");
        icfgs.add("Object__table");
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.interfaces.IHwi.FuncPtr", "ti.sysbios.family.windows"));
        vo.bind("Irp", om.findStrict("ti.sysbios.interfaces.IHwi.Irp", "ti.sysbios.family.windows"));
        vo.bind("HookSet", om.findStrict("ti.sysbios.interfaces.IHwi.HookSet", "ti.sysbios.family.windows"));
        tdefs.add(om.findStrict("ti.sysbios.interfaces.IHwi.HookSet", "ti.sysbios.family.windows"));
        vo.bind("MaskingOption", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption", "ti.sysbios.family.windows"));
        vo.bind("StackInfo", om.findStrict("ti.sysbios.interfaces.IHwi.StackInfo", "ti.sysbios.family.windows"));
        tdefs.add(om.findStrict("ti.sysbios.interfaces.IHwi.StackInfo", "ti.sysbios.family.windows"));
        mcfgs.add("dispatcherAutoNestingSupport");
        mcfgs.add("dispatcherSwiSupport");
        mcfgs.add("dispatcherTaskSupport");
        mcfgs.add("dispatcherIrpTrackingSupport");
        vo.bind("PlugFuncPtr", om.findStrict("ti.sysbios.family.windows.Hwi.PlugFuncPtr", "ti.sysbios.family.windows"));
        vo.bind("IsrFxn", om.findStrict("ti.sysbios.family.windows.Hwi.IsrFxn", "ti.sysbios.family.windows"));
        mcfgs.add("A_interNum");
        mcfgs.add("A_noPendingIntr");
        mcfgs.add("A_pluggedIntr");
        mcfgs.add("E_calloc");
        mcfgs.add("E_event");
        mcfgs.add("E_alreadyDefined");
        mcfgs.add("LM_begin");
        mcfgs.add("LD_end");
        mcfgs.add("LW_missedIntr");
        vo.bind("NonDispatchedInterrupt", om.findStrict("ti.sysbios.family.windows.Hwi.NonDispatchedInterrupt", "ti.sysbios.family.windows"));
        tdefs.add(om.findStrict("ti.sysbios.family.windows.Hwi.NonDispatchedInterrupt", "ti.sysbios.family.windows"));
        mcfgs.add("swiDisable");
        icfgs.add("swiDisable");
        mcfgs.add("swiRestoreHwi");
        icfgs.add("swiRestoreHwi");
        mcfgs.add("taskDisable");
        icfgs.add("taskDisable");
        mcfgs.add("taskRestoreHwi");
        icfgs.add("taskRestoreHwi");
        mcfgs.add("hooks");
        icfgs.add("hooks");
        vo.bind("InterruptObj", om.findStrict("ti.sysbios.family.windows.Hwi.InterruptObj", "ti.sysbios.family.windows"));
        tdefs.add(om.findStrict("ti.sysbios.family.windows.Hwi.InterruptObj", "ti.sysbios.family.windows"));
        icfgs.add("interrupt");
        vo.bind("Instance_State", om.findStrict("ti.sysbios.family.windows.Hwi.Instance_State", "ti.sysbios.family.windows"));
        tdefs.add(om.findStrict("ti.sysbios.family.windows.Hwi.Instance_State", "ti.sysbios.family.windows"));
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.windows.Hwi.Module_State", "ti.sysbios.family.windows"));
        tdefs.add(om.findStrict("ti.sysbios.family.windows.Hwi.Module_State", "ti.sysbios.family.windows"));
        vo.bind("MaskingOption_NONE", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_NONE", "ti.sysbios.family.windows"));
        vo.bind("MaskingOption_ALL", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_ALL", "ti.sysbios.family.windows"));
        vo.bind("MaskingOption_SELF", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_SELF", "ti.sysbios.family.windows"));
        vo.bind("MaskingOption_BITMASK", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_BITMASK", "ti.sysbios.family.windows"));
        vo.bind("MaskingOption_LOWER", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_LOWER", "ti.sysbios.family.windows"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.windows")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", om.has("ti.sysbios.family.windows.Hwi$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sysbios$family$windows$Hwi$$__initObject"));
        }//isCFG
        vo.bind("getStackInfo", om.findStrict("ti.sysbios.family.windows.Hwi.getStackInfo", "ti.sysbios.family.windows"));
        vo.bind("startup", om.findStrict("ti.sysbios.family.windows.Hwi.startup", "ti.sysbios.family.windows"));
        vo.bind("disable", om.findStrict("ti.sysbios.family.windows.Hwi.disable", "ti.sysbios.family.windows"));
        vo.bind("enable", om.findStrict("ti.sysbios.family.windows.Hwi.enable", "ti.sysbios.family.windows"));
        vo.bind("restore", om.findStrict("ti.sysbios.family.windows.Hwi.restore", "ti.sysbios.family.windows"));
        vo.bind("switchFromBootStack", om.findStrict("ti.sysbios.family.windows.Hwi.switchFromBootStack", "ti.sysbios.family.windows"));
        vo.bind("post", om.findStrict("ti.sysbios.family.windows.Hwi.post", "ti.sysbios.family.windows"));
        vo.bind("getTaskSP", om.findStrict("ti.sysbios.family.windows.Hwi.getTaskSP", "ti.sysbios.family.windows"));
        vo.bind("disableInterrupt", om.findStrict("ti.sysbios.family.windows.Hwi.disableInterrupt", "ti.sysbios.family.windows"));
        vo.bind("enableInterrupt", om.findStrict("ti.sysbios.family.windows.Hwi.enableInterrupt", "ti.sysbios.family.windows"));
        vo.bind("restoreInterrupt", om.findStrict("ti.sysbios.family.windows.Hwi.restoreInterrupt", "ti.sysbios.family.windows"));
        vo.bind("clearInterrupt", om.findStrict("ti.sysbios.family.windows.Hwi.clearInterrupt", "ti.sysbios.family.windows"));
        vo.bind("disableIER", om.findStrict("ti.sysbios.family.windows.Hwi.disableIER", "ti.sysbios.family.windows"));
        vo.bind("enableIER", om.findStrict("ti.sysbios.family.windows.Hwi.enableIER", "ti.sysbios.family.windows"));
        vo.bind("restoreIER", om.findStrict("ti.sysbios.family.windows.Hwi.restoreIER", "ti.sysbios.family.windows"));
        vo.bind("getHandle", om.findStrict("ti.sysbios.family.windows.Hwi.getHandle", "ti.sysbios.family.windows"));
        vo.bind("plug", om.findStrict("ti.sysbios.family.windows.Hwi.plug", "ti.sysbios.family.windows"));
        vo.bind("dispatch", om.findStrict("ti.sysbios.family.windows.Hwi.dispatch", "ti.sysbios.family.windows"));
        vo.bind("init", om.findStrict("ti.sysbios.family.windows.Hwi.init", "ti.sysbios.family.windows"));
        vo.bind("unPluggedInterrupt", om.findStrict("ti.sysbios.family.windows.Hwi.unPluggedInterrupt", "ti.sysbios.family.windows"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_windows_Hwi_Handle__label__E", "ti_sysbios_family_windows_Hwi_Module__startupDone__E", "ti_sysbios_family_windows_Hwi_Object__create__E", "ti_sysbios_family_windows_Hwi_Object__delete__E", "ti_sysbios_family_windows_Hwi_Object__destruct__E", "ti_sysbios_family_windows_Hwi_Object__get__E", "ti_sysbios_family_windows_Hwi_Object__first__E", "ti_sysbios_family_windows_Hwi_Object__next__E", "ti_sysbios_family_windows_Hwi_Params__init__E", "ti_sysbios_family_windows_Hwi_getStackInfo__E", "ti_sysbios_family_windows_Hwi_startup__E", "ti_sysbios_family_windows_Hwi_disable__E", "ti_sysbios_family_windows_Hwi_enable__E", "ti_sysbios_family_windows_Hwi_restore__E", "ti_sysbios_family_windows_Hwi_switchFromBootStack__E", "ti_sysbios_family_windows_Hwi_post__E", "ti_sysbios_family_windows_Hwi_getTaskSP__E", "ti_sysbios_family_windows_Hwi_disableInterrupt__E", "ti_sysbios_family_windows_Hwi_enableInterrupt__E", "ti_sysbios_family_windows_Hwi_restoreInterrupt__E", "ti_sysbios_family_windows_Hwi_clearInterrupt__E", "ti_sysbios_family_windows_Hwi_getFunc__E", "ti_sysbios_family_windows_Hwi_setFunc__E", "ti_sysbios_family_windows_Hwi_getHookContext__E", "ti_sysbios_family_windows_Hwi_setHookContext__E", "ti_sysbios_family_windows_Hwi_getIrp__E", "ti_sysbios_family_windows_Hwi_disableIER__E", "ti_sysbios_family_windows_Hwi_enableIER__E", "ti_sysbios_family_windows_Hwi_restoreIER__E", "ti_sysbios_family_windows_Hwi_getHandle__E", "ti_sysbios_family_windows_Hwi_plug__E", "ti_sysbios_family_windows_Hwi_reconfig__E"));
        vo.bind("$$logEvtCfgs", Global.newArray("LM_begin", "LD_end", "LW_missedIntr"));
        vo.bind("$$errorDescCfgs", Global.newArray("E_calloc", "E_event", "E_alreadyDefined"));
        vo.bind("$$assertDescCfgs", Global.newArray("A_interNum", "A_noPendingIntr", "A_pluggedIntr"));
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", "./Hwi.xdt");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sysbios.family.windows.Hwi.Object", "ti.sysbios.family.windows"));
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "getStackInfo", "entry", "%p, %d", "exit", "%d"));
        loggables.add(Global.newObject("name", "disable", "entry", "", "exit", "0x%x"));
        loggables.add(Global.newObject("name", "enable", "entry", "", "exit", "0x%x"));
        loggables.add(Global.newObject("name", "restore", "entry", "0x%x", "exit", ""));
        loggables.add(Global.newObject("name", "disableIER", "entry", "0x%x", "exit", "0x%x"));
        loggables.add(Global.newObject("name", "enableIER", "entry", "0x%x", "exit", "0x%x"));
        loggables.add(Global.newObject("name", "restoreIER", "entry", "0x%x", "exit", "0x%x"));
        loggables.add(Global.newObject("name", "getHandle", "entry", "0x%x", "exit", "%p"));
        loggables.add(Global.newObject("name", "plug", "entry", "0x%x, %p", "exit", ""));
        loggables.add(Global.newObject("name", "reconfig", "entry", "%p, %p, %p", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        vo.bind("TEMPLATE$", "./Hwi.xdt");
        pkgV.bind("Hwi", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Hwi");
    }

    void IntrinsicsSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.IntrinsicsSupport", "ti.sysbios.family.windows");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.IntrinsicsSupport.Module", "ti.sysbios.family.windows");
        vo.init2(po, "ti.sysbios.family.windows.IntrinsicsSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("$package", om.findStrict("ti.sysbios.family.windows", "ti.sysbios.family.windows"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        mcfgs.add("Module__diagsEnabled");
        icfgs.add("Module__diagsEnabled");
        mcfgs.add("Module__diagsIncluded");
        icfgs.add("Module__diagsIncluded");
        mcfgs.add("Module__diagsMask");
        icfgs.add("Module__diagsMask");
        mcfgs.add("Module__gateObj");
        icfgs.add("Module__gateObj");
        mcfgs.add("Module__gatePrms");
        icfgs.add("Module__gatePrms");
        mcfgs.add("Module__id");
        icfgs.add("Module__id");
        mcfgs.add("Module__loggerDefined");
        icfgs.add("Module__loggerDefined");
        mcfgs.add("Module__loggerObj");
        icfgs.add("Module__loggerObj");
        mcfgs.add("Module__loggerFxn0");
        icfgs.add("Module__loggerFxn0");
        mcfgs.add("Module__loggerFxn1");
        icfgs.add("Module__loggerFxn1");
        mcfgs.add("Module__loggerFxn2");
        icfgs.add("Module__loggerFxn2");
        mcfgs.add("Module__loggerFxn4");
        icfgs.add("Module__loggerFxn4");
        mcfgs.add("Module__loggerFxn8");
        icfgs.add("Module__loggerFxn8");
        mcfgs.add("Module__startupDoneFxn");
        icfgs.add("Module__startupDoneFxn");
        mcfgs.add("Object__count");
        icfgs.add("Object__count");
        mcfgs.add("Object__heap");
        icfgs.add("Object__heap");
        mcfgs.add("Object__sizeof");
        icfgs.add("Object__sizeof");
        mcfgs.add("Object__table");
        icfgs.add("Object__table");
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.windows")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("maxbit", om.findStrict("ti.sysbios.family.windows.IntrinsicsSupport.maxbit", "ti.sysbios.family.windows"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_windows_IntrinsicsSupport_Module__startupDone__E", "ti_sysbios_family_windows_IntrinsicsSupport_maxbit__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("IntrinsicsSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("IntrinsicsSupport");
    }

    void Settings$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.Settings", "ti.sysbios.family.windows");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Settings.Module", "ti.sysbios.family.windows");
        vo.init2(po, "ti.sysbios.family.windows.Settings", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.windows.Settings$$capsule", "ti.sysbios.family.windows"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.windows", "ti.sysbios.family.windows"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.windows")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.sysbios.family.windows.Settings$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        pkgV.bind("Settings", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Settings");
    }

    void TaskSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.TaskSupport", "ti.sysbios.family.windows");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.TaskSupport.Module", "ti.sysbios.family.windows");
        vo.init2(po, "ti.sysbios.family.windows.TaskSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.windows.TaskSupport$$capsule", "ti.sysbios.family.windows"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.windows", "ti.sysbios.family.windows"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        mcfgs.add("Module__diagsEnabled");
        icfgs.add("Module__diagsEnabled");
        mcfgs.add("Module__diagsIncluded");
        icfgs.add("Module__diagsIncluded");
        mcfgs.add("Module__diagsMask");
        icfgs.add("Module__diagsMask");
        mcfgs.add("Module__gateObj");
        icfgs.add("Module__gateObj");
        mcfgs.add("Module__gatePrms");
        icfgs.add("Module__gatePrms");
        mcfgs.add("Module__id");
        icfgs.add("Module__id");
        mcfgs.add("Module__loggerDefined");
        icfgs.add("Module__loggerDefined");
        mcfgs.add("Module__loggerObj");
        icfgs.add("Module__loggerObj");
        mcfgs.add("Module__loggerFxn0");
        icfgs.add("Module__loggerFxn0");
        mcfgs.add("Module__loggerFxn1");
        icfgs.add("Module__loggerFxn1");
        mcfgs.add("Module__loggerFxn2");
        icfgs.add("Module__loggerFxn2");
        mcfgs.add("Module__loggerFxn4");
        icfgs.add("Module__loggerFxn4");
        mcfgs.add("Module__loggerFxn8");
        icfgs.add("Module__loggerFxn8");
        mcfgs.add("Module__startupDoneFxn");
        icfgs.add("Module__startupDoneFxn");
        mcfgs.add("Object__count");
        icfgs.add("Object__count");
        mcfgs.add("Object__heap");
        icfgs.add("Object__heap");
        mcfgs.add("Object__sizeof");
        icfgs.add("Object__sizeof");
        mcfgs.add("Object__table");
        icfgs.add("Object__table");
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.interfaces.ITaskSupport.FuncPtr", "ti.sysbios.family.windows"));
        mcfgs.add("defaultStackSize");
        mcfgs.add("stackAlignment");
        vo.bind("GlueFxn", om.findStrict("ti.sysbios.family.windows.TaskSupport.GlueFxn", "ti.sysbios.family.windows"));
        vo.bind("TaskFxn", om.findStrict("ti.sysbios.family.windows.TaskSupport.TaskFxn", "ti.sysbios.family.windows"));
        vo.bind("IsrFxn", om.findStrict("ti.sysbios.family.windows.TaskSupport.IsrFxn", "ti.sysbios.family.windows"));
        mcfgs.add("E_calloc");
        mcfgs.add("E_event");
        mcfgs.add("E_taskState");
        mcfgs.add("E_thread");
        mcfgs.add("E_wait");
        vo.bind("State", om.findStrict("ti.sysbios.family.windows.TaskSupport.State", "ti.sysbios.family.windows"));
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.windows.TaskSupport.Module_State", "ti.sysbios.family.windows"));
        tdefs.add(om.findStrict("ti.sysbios.family.windows.TaskSupport.Module_State", "ti.sysbios.family.windows"));
        vo.bind("State_PREEMPTED", om.findStrict("ti.sysbios.family.windows.TaskSupport.State_PREEMPTED", "ti.sysbios.family.windows"));
        vo.bind("State_RUNNING", om.findStrict("ti.sysbios.family.windows.TaskSupport.State_RUNNING", "ti.sysbios.family.windows"));
        vo.bind("State_TERMINATING", om.findStrict("ti.sysbios.family.windows.TaskSupport.State_TERMINATING", "ti.sysbios.family.windows"));
        vo.bind("State_YIELDED", om.findStrict("ti.sysbios.family.windows.TaskSupport.State_YIELDED", "ti.sysbios.family.windows"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.windows")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("start", om.findStrict("ti.sysbios.family.windows.TaskSupport.start", "ti.sysbios.family.windows"));
        vo.bind("swap", om.findStrict("ti.sysbios.family.windows.TaskSupport.swap", "ti.sysbios.family.windows"));
        vo.bind("checkStack", om.findStrict("ti.sysbios.family.windows.TaskSupport.checkStack", "ti.sysbios.family.windows"));
        vo.bind("stackUsed", om.findStrict("ti.sysbios.family.windows.TaskSupport.stackUsed", "ti.sysbios.family.windows"));
        vo.bind("getStackAlignment", om.findStrict("ti.sysbios.family.windows.TaskSupport.getStackAlignment", "ti.sysbios.family.windows"));
        vo.bind("getDefaultStackSize", om.findStrict("ti.sysbios.family.windows.TaskSupport.getDefaultStackSize", "ti.sysbios.family.windows"));
        vo.bind("enterCS", om.findStrict("ti.sysbios.family.windows.TaskSupport.enterCS", "ti.sysbios.family.windows"));
        vo.bind("leaveCS", om.findStrict("ti.sysbios.family.windows.TaskSupport.leaveCS", "ti.sysbios.family.windows"));
        vo.bind("processInterrupt", om.findStrict("ti.sysbios.family.windows.TaskSupport.processInterrupt", "ti.sysbios.family.windows"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_windows_TaskSupport_Module__startupDone__E", "ti_sysbios_family_windows_TaskSupport_start__E", "ti_sysbios_family_windows_TaskSupport_swap__E", "ti_sysbios_family_windows_TaskSupport_checkStack__E", "ti_sysbios_family_windows_TaskSupport_stackUsed__E", "ti_sysbios_family_windows_TaskSupport_getStackAlignment__E", "ti_sysbios_family_windows_TaskSupport_getDefaultStackSize__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray("E_calloc", "E_event", "E_taskState", "E_thread", "E_wait"));
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("TaskSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TaskSupport");
    }

    void Timer$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.Timer", "ti.sysbios.family.windows");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.Timer.Module", "ti.sysbios.family.windows");
        vo.init2(po, "ti.sysbios.family.windows.Timer", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.windows.Timer$$capsule", "ti.sysbios.family.windows"));
        vo.bind("Instance", om.findStrict("ti.sysbios.family.windows.Timer.Instance", "ti.sysbios.family.windows"));
        vo.bind("Params", om.findStrict("ti.sysbios.family.windows.Timer.Params", "ti.sysbios.family.windows"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sysbios.family.windows.Timer.Params", "ti.sysbios.family.windows")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sysbios.family.windows.Timer.Handle", "ti.sysbios.family.windows"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.windows", "ti.sysbios.family.windows"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        mcfgs.add("Module__diagsEnabled");
        icfgs.add("Module__diagsEnabled");
        mcfgs.add("Module__diagsIncluded");
        icfgs.add("Module__diagsIncluded");
        mcfgs.add("Module__diagsMask");
        icfgs.add("Module__diagsMask");
        mcfgs.add("Module__gateObj");
        icfgs.add("Module__gateObj");
        mcfgs.add("Module__gatePrms");
        icfgs.add("Module__gatePrms");
        mcfgs.add("Module__id");
        icfgs.add("Module__id");
        mcfgs.add("Module__loggerDefined");
        icfgs.add("Module__loggerDefined");
        mcfgs.add("Module__loggerObj");
        icfgs.add("Module__loggerObj");
        mcfgs.add("Module__loggerFxn0");
        icfgs.add("Module__loggerFxn0");
        mcfgs.add("Module__loggerFxn1");
        icfgs.add("Module__loggerFxn1");
        mcfgs.add("Module__loggerFxn2");
        icfgs.add("Module__loggerFxn2");
        mcfgs.add("Module__loggerFxn4");
        icfgs.add("Module__loggerFxn4");
        mcfgs.add("Module__loggerFxn8");
        icfgs.add("Module__loggerFxn8");
        mcfgs.add("Module__startupDoneFxn");
        icfgs.add("Module__startupDoneFxn");
        mcfgs.add("Object__count");
        icfgs.add("Object__count");
        mcfgs.add("Object__heap");
        icfgs.add("Object__heap");
        mcfgs.add("Object__sizeof");
        icfgs.add("Object__sizeof");
        mcfgs.add("Object__table");
        icfgs.add("Object__table");
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.interfaces.ITimer.FuncPtr", "ti.sysbios.family.windows"));
        vo.bind("StartMode", om.findStrict("ti.sysbios.interfaces.ITimer.StartMode", "ti.sysbios.family.windows"));
        vo.bind("RunMode", om.findStrict("ti.sysbios.interfaces.ITimer.RunMode", "ti.sysbios.family.windows"));
        vo.bind("Status", om.findStrict("ti.sysbios.interfaces.ITimer.Status", "ti.sysbios.family.windows"));
        vo.bind("PeriodType", om.findStrict("ti.sysbios.interfaces.ITimer.PeriodType", "ti.sysbios.family.windows"));
        mcfgs.add("E_calloc");
        mcfgs.add("E_cannotSupport");
        mcfgs.add("E_event");
        mcfgs.add("E_invalidTimer");
        mcfgs.add("E_noneAvailable");
        mcfgs.add("E_notSupported");
        mcfgs.add("E_thread");
        mcfgs.add("E_notAvailable");
        mcfgs.add("E_wait");
        mcfgs.add("anyMask");
        vo.bind("Instance_State", om.findStrict("ti.sysbios.family.windows.Timer.Instance_State", "ti.sysbios.family.windows"));
        tdefs.add(om.findStrict("ti.sysbios.family.windows.Timer.Instance_State", "ti.sysbios.family.windows"));
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.windows.Timer.Module_State", "ti.sysbios.family.windows"));
        tdefs.add(om.findStrict("ti.sysbios.family.windows.Timer.Module_State", "ti.sysbios.family.windows"));
        vo.bind("StartMode_AUTO", om.findStrict("ti.sysbios.interfaces.ITimer.StartMode_AUTO", "ti.sysbios.family.windows"));
        vo.bind("StartMode_USER", om.findStrict("ti.sysbios.interfaces.ITimer.StartMode_USER", "ti.sysbios.family.windows"));
        vo.bind("RunMode_CONTINUOUS", om.findStrict("ti.sysbios.interfaces.ITimer.RunMode_CONTINUOUS", "ti.sysbios.family.windows"));
        vo.bind("RunMode_ONESHOT", om.findStrict("ti.sysbios.interfaces.ITimer.RunMode_ONESHOT", "ti.sysbios.family.windows"));
        vo.bind("RunMode_DYNAMIC", om.findStrict("ti.sysbios.interfaces.ITimer.RunMode_DYNAMIC", "ti.sysbios.family.windows"));
        vo.bind("Status_INUSE", om.findStrict("ti.sysbios.interfaces.ITimer.Status_INUSE", "ti.sysbios.family.windows"));
        vo.bind("Status_FREE", om.findStrict("ti.sysbios.interfaces.ITimer.Status_FREE", "ti.sysbios.family.windows"));
        vo.bind("PeriodType_MICROSECS", om.findStrict("ti.sysbios.interfaces.ITimer.PeriodType_MICROSECS", "ti.sysbios.family.windows"));
        vo.bind("PeriodType_COUNTS", om.findStrict("ti.sysbios.interfaces.ITimer.PeriodType_COUNTS", "ti.sysbios.family.windows"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.windows")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", om.has("ti.sysbios.family.windows.Timer$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sysbios$family$windows$Timer$$__initObject"));
        }//isCFG
        vo.bind("getNumTimers", om.findStrict("ti.sysbios.family.windows.Timer.getNumTimers", "ti.sysbios.family.windows"));
        vo.bind("getStatus", om.findStrict("ti.sysbios.family.windows.Timer.getStatus", "ti.sysbios.family.windows"));
        vo.bind("startup", om.findStrict("ti.sysbios.family.windows.Timer.startup", "ti.sysbios.family.windows"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_windows_Timer_Handle__label__E", "ti_sysbios_family_windows_Timer_Module__startupDone__E", "ti_sysbios_family_windows_Timer_Object__create__E", "ti_sysbios_family_windows_Timer_Object__delete__E", "ti_sysbios_family_windows_Timer_Object__destruct__E", "ti_sysbios_family_windows_Timer_Object__get__E", "ti_sysbios_family_windows_Timer_Object__first__E", "ti_sysbios_family_windows_Timer_Object__next__E", "ti_sysbios_family_windows_Timer_Params__init__E", "ti_sysbios_family_windows_Timer_getNumTimers__E", "ti_sysbios_family_windows_Timer_getStatus__E", "ti_sysbios_family_windows_Timer_startup__E", "ti_sysbios_family_windows_Timer_getMaxTicks__E", "ti_sysbios_family_windows_Timer_setNextTick__E", "ti_sysbios_family_windows_Timer_start__E", "ti_sysbios_family_windows_Timer_stop__E", "ti_sysbios_family_windows_Timer_setPeriod__E", "ti_sysbios_family_windows_Timer_setPeriodMicroSecs__E", "ti_sysbios_family_windows_Timer_getPeriod__E", "ti_sysbios_family_windows_Timer_getCount__E", "ti_sysbios_family_windows_Timer_getFreq__E", "ti_sysbios_family_windows_Timer_getFunc__E", "ti_sysbios_family_windows_Timer_setFunc__E", "ti_sysbios_family_windows_Timer_trigger__E", "ti_sysbios_family_windows_Timer_getExpiredCounts__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray("E_calloc", "E_cannotSupport", "E_event", "E_invalidTimer", "E_noneAvailable", "E_notSupported", "E_thread", "E_notAvailable", "E_wait"));
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sysbios.family.windows.Timer.Object", "ti.sysbios.family.windows"));
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("Timer", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Timer");
    }

    void TimestampProvider$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.windows.TimestampProvider", "ti.sysbios.family.windows");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.windows.TimestampProvider.Module", "ti.sysbios.family.windows");
        vo.init2(po, "ti.sysbios.family.windows.TimestampProvider", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.windows.TimestampProvider$$capsule", "ti.sysbios.family.windows"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.windows", "ti.sysbios.family.windows"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        mcfgs.add("Module__diagsEnabled");
        icfgs.add("Module__diagsEnabled");
        mcfgs.add("Module__diagsIncluded");
        icfgs.add("Module__diagsIncluded");
        mcfgs.add("Module__diagsMask");
        icfgs.add("Module__diagsMask");
        mcfgs.add("Module__gateObj");
        icfgs.add("Module__gateObj");
        mcfgs.add("Module__gatePrms");
        icfgs.add("Module__gatePrms");
        mcfgs.add("Module__id");
        icfgs.add("Module__id");
        mcfgs.add("Module__loggerDefined");
        icfgs.add("Module__loggerDefined");
        mcfgs.add("Module__loggerObj");
        icfgs.add("Module__loggerObj");
        mcfgs.add("Module__loggerFxn0");
        icfgs.add("Module__loggerFxn0");
        mcfgs.add("Module__loggerFxn1");
        icfgs.add("Module__loggerFxn1");
        mcfgs.add("Module__loggerFxn2");
        icfgs.add("Module__loggerFxn2");
        mcfgs.add("Module__loggerFxn4");
        icfgs.add("Module__loggerFxn4");
        mcfgs.add("Module__loggerFxn8");
        icfgs.add("Module__loggerFxn8");
        mcfgs.add("Module__startupDoneFxn");
        icfgs.add("Module__startupDoneFxn");
        mcfgs.add("Object__count");
        icfgs.add("Object__count");
        mcfgs.add("Object__heap");
        icfgs.add("Object__heap");
        mcfgs.add("Object__sizeof");
        icfgs.add("Object__sizeof");
        mcfgs.add("Object__table");
        icfgs.add("Object__table");
        mcfgs.add("E_processTimes");
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.windows")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("get32", om.findStrict("ti.sysbios.family.windows.TimestampProvider.get32", "ti.sysbios.family.windows"));
        vo.bind("get64", om.findStrict("ti.sysbios.family.windows.TimestampProvider.get64", "ti.sysbios.family.windows"));
        vo.bind("getFreq", om.findStrict("ti.sysbios.family.windows.TimestampProvider.getFreq", "ti.sysbios.family.windows"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_windows_TimestampProvider_Module__startupDone__E", "ti_sysbios_family_windows_TimestampProvider_get32__E", "ti_sysbios_family_windows_TimestampProvider_get64__E", "ti_sysbios_family_windows_TimestampProvider_getFreq__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray("E_processTimes"));
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "get32", "entry", "", "exit", "0x%x"));
        loggables.add(Global.newObject("name", "get64", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "getFreq", "entry", "%p", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("TimestampProvider", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TimestampProvider");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
            Object srcP = ((XScriptO)om.findStrict("xdc.runtime.IInstance", "ti.sysbios.family.windows")).findStrict("PARAMS", "ti.sysbios.family.windows");
            Scriptable dstP;

            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sysbios.family.windows.Hwi", "ti.sysbios.family.windows")).findStrict("PARAMS", "ti.sysbios.family.windows");
            Global.put(dstP, "instance", srcP);
            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sysbios.family.windows.Timer", "ti.sysbios.family.windows")).findStrict("PARAMS", "ti.sysbios.family.windows");
            Global.put(dstP, "instance", srcP);
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.windows.Hwi", "ti.sysbios.family.windows"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.windows.IntrinsicsSupport", "ti.sysbios.family.windows"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.windows.Settings", "ti.sysbios.family.windows"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.windows.TaskSupport", "ti.sysbios.family.windows"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.windows.Timer", "ti.sysbios.family.windows"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.windows.TimestampProvider", "ti.sysbios.family.windows"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sysbios.family.windows.Hwi")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.windows.IntrinsicsSupport")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.windows.Settings")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.windows.TaskSupport")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.windows.Timer")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.windows.TimestampProvider")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sysbios.family.windows")).add(pkgV);
    }

    public void exec( Scriptable xdcO, Session ses )
    {
        this.xdcO = xdcO;
        this.ses = ses;
        om = (Value.Obj)xdcO.get("om", null);

        Object o = om.geto("$name");
        String s = o instanceof String ? (String)o : null;
        isCFG = s != null && s.equals("cfg");
        isROV = s != null && s.equals("rov");

        $$IMPORTS();
        $$OBJECTS();
        Hwi$$OBJECTS();
        IntrinsicsSupport$$OBJECTS();
        Settings$$OBJECTS();
        TaskSupport$$OBJECTS();
        Timer$$OBJECTS();
        TimestampProvider$$OBJECTS();
        Hwi$$CONSTS();
        IntrinsicsSupport$$CONSTS();
        Settings$$CONSTS();
        TaskSupport$$CONSTS();
        Timer$$CONSTS();
        TimestampProvider$$CONSTS();
        Hwi$$CREATES();
        IntrinsicsSupport$$CREATES();
        Settings$$CREATES();
        TaskSupport$$CREATES();
        Timer$$CREATES();
        TimestampProvider$$CREATES();
        Hwi$$FUNCTIONS();
        IntrinsicsSupport$$FUNCTIONS();
        Settings$$FUNCTIONS();
        TaskSupport$$FUNCTIONS();
        Timer$$FUNCTIONS();
        TimestampProvider$$FUNCTIONS();
        Hwi$$SIZES();
        IntrinsicsSupport$$SIZES();
        Settings$$SIZES();
        TaskSupport$$SIZES();
        Timer$$SIZES();
        TimestampProvider$$SIZES();
        Hwi$$TYPES();
        IntrinsicsSupport$$TYPES();
        Settings$$TYPES();
        TaskSupport$$TYPES();
        Timer$$TYPES();
        TimestampProvider$$TYPES();
        if (isROV) {
            Hwi$$ROV();
            IntrinsicsSupport$$ROV();
            Settings$$ROV();
            TaskSupport$$ROV();
            Timer$$ROV();
            TimestampProvider$$ROV();
        }//isROV
        $$SINGLETONS();
        Hwi$$SINGLETONS();
        IntrinsicsSupport$$SINGLETONS();
        Settings$$SINGLETONS();
        TaskSupport$$SINGLETONS();
        Timer$$SINGLETONS();
        TimestampProvider$$SINGLETONS();
        $$INITIALIZATION();
    }
}
