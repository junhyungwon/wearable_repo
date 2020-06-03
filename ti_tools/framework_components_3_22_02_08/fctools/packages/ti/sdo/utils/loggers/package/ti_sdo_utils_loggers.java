/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-y21
 */
import java.util.*;
import org.mozilla.javascript.*;
import xdc.services.intern.xsr.*;
import xdc.services.spec.Session;

public class ti_sdo_utils_loggers
{
    static final String VERS = "@(#) xdc-y21\n";

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
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
        Global.callFxn("loadPackage", xdcO, "xdc.runtime");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.sdo.utils.loggers.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sdo.utils.loggers", new Value.Obj("ti.sdo.utils.loggers", pkgP));
    }

    void ILoggerMFP$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.ILoggerMFP.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.utils.loggers.ILoggerMFP", new Value.Obj("ti.sdo.utils.loggers.ILoggerMFP", po));
        pkgV.bind("ILoggerMFP", vo);
        // decls 
        om.bind("ti.sdo.utils.loggers.ILoggerMFP.Cmd", new Proto.Enm("ti.sdo.utils.loggers.ILoggerMFP.Cmd"));
        // insts 
        Object insP = om.bind("ti.sdo.utils.loggers.ILoggerMFP.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.ILoggerMFP$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.utils.loggers.ILoggerMFP.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.ILoggerMFP$$Params", new Proto.Obj());
        om.bind("ti.sdo.utils.loggers.ILoggerMFP.Params", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.ILoggerMFP$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.utils.loggers.ILoggerMFP.Instance_State", new Proto.Str(po, false));
        om.bind("ti.sdo.utils.loggers.ILoggerMFP.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.utils.loggers.ILoggerMFP.Object", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Instance_State", "ti.sdo.utils.loggers"));
        }//isROV
    }

    void LoggerSysTID$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID", new Value.Obj("ti.sdo.utils.loggers.LoggerSysTID", po));
        pkgV.bind("LoggerSysTID", vo);
        // decls 
        om.bind("ti.sdo.utils.loggers.LoggerSysTID.Cmd", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Cmd", "ti.sdo.utils.loggers"));
        om.bind("ti.sdo.utils.loggers.LoggerSysTID.TSMode", new Proto.Enm("ti.sdo.utils.loggers.LoggerSysTID.TSMode"));
        spo = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.utils.loggers.LoggerSysTID.Instance_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sdo.utils.loggers.LoggerSysTID.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.utils.loggers.LoggerSysTID.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID$$Params", new Proto.Obj());
        om.bind("ti.sdo.utils.loggers.LoggerSysTID.Params", new Proto.Str(po, false));
        om.bind("ti.sdo.utils.loggers.LoggerSysTID.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.utils.loggers.LoggerSysTID.Object", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Instance_State", "ti.sdo.utils.loggers"));
        }//isROV
    }

    void LoggerMFPNull$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerMFPNull.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.utils.loggers.LoggerMFPNull", new Value.Obj("ti.sdo.utils.loggers.LoggerMFPNull", po));
        pkgV.bind("LoggerMFPNull", vo);
        // decls 
        om.bind("ti.sdo.utils.loggers.LoggerMFPNull.Cmd", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Cmd", "ti.sdo.utils.loggers"));
        spo = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerMFPNull$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.utils.loggers.LoggerMFPNull.Instance_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sdo.utils.loggers.LoggerMFPNull.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerMFPNull$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.utils.loggers.LoggerMFPNull.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerMFPNull$$Params", new Proto.Obj());
        om.bind("ti.sdo.utils.loggers.LoggerMFPNull.Params", new Proto.Str(po, false));
        om.bind("ti.sdo.utils.loggers.LoggerMFPNull.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.utils.loggers.LoggerMFPNull.Object", om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Instance_State", "ti.sdo.utils.loggers"));
        }//isROV
    }

    void LogMFP$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LogMFP.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.utils.loggers.LogMFP", new Value.Obj("ti.sdo.utils.loggers.LogMFP", po));
        pkgV.bind("LogMFP", vo);
        // decls 
    }

    void LoggerSysTID_TimestampProxy$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy", new Value.Obj("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy", po));
        pkgV.bind("LoggerSysTID_TimestampProxy", vo);
        // decls 
    }

    void LoggerSysTID_Module_GateProxy$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy", new Value.Obj("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy", po));
        pkgV.bind("LoggerSysTID_Module_GateProxy", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy$$Params", new Proto.Obj());
        om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Params", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance_State", new Proto.Str(po, false));
        om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Object", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance_State", "ti.sdo.utils.loggers"));
        }//isROV
    }

    void LogMFP_Proxy$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LogMFP_Proxy.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.utils.loggers.LogMFP_Proxy", new Value.Obj("ti.sdo.utils.loggers.LogMFP_Proxy", po));
        pkgV.bind("LogMFP_Proxy", vo);
        // decls 
        om.bind("ti.sdo.utils.loggers.LogMFP_Proxy.Cmd", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Cmd", "ti.sdo.utils.loggers"));
        // insts 
        Object insP = om.bind("ti.sdo.utils.loggers.LogMFP_Proxy.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LogMFP_Proxy$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.utils.loggers.LogMFP_Proxy.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LogMFP_Proxy$$Params", new Proto.Obj());
        om.bind("ti.sdo.utils.loggers.LogMFP_Proxy.Params", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.utils.loggers.LogMFP_Proxy$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.utils.loggers.LogMFP_Proxy.Instance_State", new Proto.Str(po, false));
        om.bind("ti.sdo.utils.loggers.LogMFP_Proxy.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.utils.loggers.LogMFP_Proxy.Object", om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Instance_State", "ti.sdo.utils.loggers"));
        }//isROV
    }

    void ILoggerMFP$$CONSTS()
    {
        // interface ILoggerMFP
        om.bind("ti.sdo.utils.loggers.ILoggerMFP.SETTRACE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Cmd", "ti.sdo.utils.loggers"), "ti.sdo.utils.loggers.ILoggerMFP.SETTRACE", xdc.services.intern.xsr.Enum.intValue(0L)+0));
    }

    void LoggerSysTID$$CONSTS()
    {
        // module LoggerSysTID
        om.bind("ti.sdo.utils.loggers.LoggerSysTID.TSMode_USEC", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.TSMode", "ti.sdo.utils.loggers"), "ti.sdo.utils.loggers.LoggerSysTID.TSMode_USEC", 0));
        om.bind("ti.sdo.utils.loggers.LoggerSysTID.TSMode_SEC", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.TSMode", "ti.sdo.utils.loggers"), "ti.sdo.utils.loggers.LoggerSysTID.TSMode_SEC", 1));
        om.bind("ti.sdo.utils.loggers.LoggerSysTID.TSMode_DELTAUSEC", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.TSMode", "ti.sdo.utils.loggers"), "ti.sdo.utils.loggers.LoggerSysTID.TSMode_DELTAUSEC", 2));
        om.bind("ti.sdo.utils.loggers.LoggerSysTID.TSMode_TICKS", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.TSMode", "ti.sdo.utils.loggers"), "ti.sdo.utils.loggers.LoggerSysTID.TSMode_TICKS", 3));
        om.bind("ti.sdo.utils.loggers.LoggerSysTID.control", new Extern("ti_sdo_utils_loggers_LoggerSysTID_control__E", "xdc_Void(*)(xdc_Int,xdc_UArg)", true, false));
    }

    void LoggerMFPNull$$CONSTS()
    {
        // module LoggerMFPNull
        om.bind("ti.sdo.utils.loggers.LoggerMFPNull.control", new Extern("ti_sdo_utils_loggers_LoggerMFPNull_control__E", "xdc_Void(*)(xdc_Int,xdc_UArg)", true, false));
    }

    void LogMFP$$CONSTS()
    {
        // module LogMFP
        om.bind("ti.sdo.utils.loggers.LogMFP.control", new Extern("ti_sdo_utils_loggers_LogMFP_control__E", "xdc_Void(*)(xdc_Int,xdc_UArg)", true, false));
    }

    void LoggerSysTID_TimestampProxy$$CONSTS()
    {
        // module LoggerSysTID_TimestampProxy
        om.bind("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy.get32", new Extern("ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_get32__E", "xdc_Bits32(*)(xdc_Void)", true, false));
        om.bind("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy.get64", new Extern("ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_get64__E", "xdc_Void(*)(xdc_runtime_Types_Timestamp64*)", true, false));
        om.bind("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy.getFreq", new Extern("ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_getFreq__E", "xdc_Void(*)(xdc_runtime_Types_FreqHz*)", true, false));
    }

    void LoggerSysTID_Module_GateProxy$$CONSTS()
    {
        // module LoggerSysTID_Module_GateProxy
        om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.query", new Extern("ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_query__E", "xdc_Bool(*)(xdc_Int)", true, false));
    }

    void LogMFP_Proxy$$CONSTS()
    {
        // module LogMFP_Proxy
        om.bind("ti.sdo.utils.loggers.LogMFP_Proxy.control", new Extern("ti_sdo_utils_loggers_LogMFP_Proxy_control__E", "xdc_Void(*)(xdc_Int,xdc_UArg)", true, false));
    }

    void ILoggerMFP$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void LoggerSysTID$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sdo$utils$loggers$LoggerSysTID$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sdo.utils.loggers.LoggerSysTID.create() called before xdc.useModule('ti.sdo.utils.loggers.LoggerSysTID')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sdo.utils.loggers.LoggerSysTID$$create", new Proto.Fxn(om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Module", "ti.sdo.utils.loggers"), om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Instance", "ti.sdo.utils.loggers"), 1, 0, false));
                        fxn.addArg(0, "__params", (Proto)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Params", "ti.sdo.utils.loggers"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$utils$loggers$LoggerSysTID$$create = function( __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.utils.loggers.LoggerSysTID'];\n");
                sb.append("var __inst = xdc.om['ti.sdo.utils.loggers.LoggerSysTID.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sdo.utils.loggers']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sdo.utils.loggers.LoggerSysTID'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.utils.loggers.LoggerSysTID'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sdo.utils.loggers.LoggerSysTID$$construct", new Proto.Fxn(om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Module", "ti.sdo.utils.loggers"), null, 2, 0, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID$$Object", "ti.sdo.utils.loggers"), null);
                        fxn.addArg(1, "__params", (Proto)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Params", "ti.sdo.utils.loggers"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$utils$loggers$LoggerSysTID$$construct = function( __obj, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.utils.loggers.LoggerSysTID'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sdo.utils.loggers.LoggerSysTID'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.utils.loggers.LoggerSysTID'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void LoggerMFPNull$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sdo$utils$loggers$LoggerMFPNull$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sdo.utils.loggers.LoggerMFPNull.create() called before xdc.useModule('ti.sdo.utils.loggers.LoggerMFPNull')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sdo.utils.loggers.LoggerMFPNull$$create", new Proto.Fxn(om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Module", "ti.sdo.utils.loggers"), om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Instance", "ti.sdo.utils.loggers"), 1, 0, false));
                        fxn.addArg(0, "__params", (Proto)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Params", "ti.sdo.utils.loggers"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$utils$loggers$LoggerMFPNull$$create = function( __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.utils.loggers.LoggerMFPNull'];\n");
                sb.append("var __inst = xdc.om['ti.sdo.utils.loggers.LoggerMFPNull.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sdo.utils.loggers']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sdo.utils.loggers.LoggerMFPNull'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.utils.loggers.LoggerMFPNull'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sdo.utils.loggers.LoggerMFPNull$$construct", new Proto.Fxn(om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Module", "ti.sdo.utils.loggers"), null, 2, 0, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull$$Object", "ti.sdo.utils.loggers"), null);
                        fxn.addArg(1, "__params", (Proto)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Params", "ti.sdo.utils.loggers"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$utils$loggers$LoggerMFPNull$$construct = function( __obj, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.utils.loggers.LoggerMFPNull'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sdo.utils.loggers.LoggerMFPNull'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.utils.loggers.LoggerMFPNull'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void LogMFP$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void LoggerSysTID_TimestampProxy$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void LoggerSysTID_Module_GateProxy$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sdo$utils$loggers$LoggerSysTID_Module_GateProxy$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.create() called before xdc.useModule('ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy')\");\n");
                sb.append("}\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy$$create", new Proto.Fxn(om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Module", "ti.sdo.utils.loggers"), om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance", "ti.sdo.utils.loggers"), 1, 0, false));
                        fxn.addArg(0, "__params", (Proto)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Params", "ti.sdo.utils.loggers"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$utils$loggers$LoggerSysTID_Module_GateProxy$$create = function( __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy'];\n");
                sb.append("var __inst = xdc.om['ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sdo.utils.loggers']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy'].Instance_State);\n");
                sb.append("if (!__mod.delegate$) {\n");
                    sb.append("throw new Error(\"Unbound proxy module: ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy\");\n");
                sb.append("}\n");
                sb.append("var __dmod = __mod.delegate$.$orig;\n");
                sb.append("var __dinst = __dmod.create(__params);\n");
                sb.append("__inst.$$bind('delegate$', __dinst);\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void LogMFP_Proxy$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sdo$utils$loggers$LogMFP_Proxy$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sdo.utils.loggers.LogMFP_Proxy.create() called before xdc.useModule('ti.sdo.utils.loggers.LogMFP_Proxy')\");\n");
                sb.append("}\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sdo.utils.loggers.LogMFP_Proxy$$create", new Proto.Fxn(om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Module", "ti.sdo.utils.loggers"), om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Instance", "ti.sdo.utils.loggers"), 1, 0, false));
                        fxn.addArg(0, "__params", (Proto)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Params", "ti.sdo.utils.loggers"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$utils$loggers$LogMFP_Proxy$$create = function( __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.utils.loggers.LogMFP_Proxy'];\n");
                sb.append("var __inst = xdc.om['ti.sdo.utils.loggers.LogMFP_Proxy.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sdo.utils.loggers']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sdo.utils.loggers.LogMFP_Proxy'].Instance_State);\n");
                sb.append("if (!__mod.delegate$) {\n");
                    sb.append("throw new Error(\"Unbound proxy module: ti.sdo.utils.loggers.LogMFP_Proxy\");\n");
                sb.append("}\n");
                sb.append("var __dmod = __mod.delegate$.$orig;\n");
                sb.append("var __dinst = __dmod.create(__params);\n");
                sb.append("__inst.$$bind('delegate$', __dinst);\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.utils.loggers.LogMFP_Proxy'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void ILoggerMFP$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LoggerSysTID$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LoggerMFPNull$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LogMFP$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LoggerSysTID_TimestampProxy$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LoggerSysTID_Module_GateProxy$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LogMFP_Proxy$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void ILoggerMFP$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void LoggerSysTID$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Instance_State", "ti.sdo.utils.loggers");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("enabled", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sdo.utils.loggers.LoggerSysTID.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sdo.utils.loggers.LoggerSysTID.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sdo.utils.loggers.LoggerSysTID.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void LoggerMFPNull$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Instance_State", "ti.sdo.utils.loggers");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("enabled", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sdo.utils.loggers.LoggerMFPNull.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sdo.utils.loggers.LoggerMFPNull.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sdo.utils.loggers.LoggerMFPNull.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void LogMFP$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void LoggerSysTID_TimestampProxy$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void LoggerSysTID_Module_GateProxy$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void LogMFP_Proxy$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void ILoggerMFP$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Module", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.ILoggerMFP.Module", om.findStrict("xdc.runtime.ILogger.Module", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Instance", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.ILoggerMFP.Instance", om.findStrict("xdc.runtime.ILogger.Instance", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP$$Params", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.ILoggerMFP.Params", om.findStrict("xdc.runtime.ILogger$$Params", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
    }

    void LoggerSysTID$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/utils/loggers/LoggerSysTID.xs");
        om.bind("ti.sdo.utils.loggers.LoggerSysTID$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Module", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID.Module", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Module", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFldV("TimestampProxy", (Proto)om.findStrict("xdc.runtime.ITimestampClient.Module", "ti.sdo.utils.loggers"), null, "wh", $$delegGet, $$delegSet);
            po.addFld("gate", (Proto)om.findStrict("xdc.runtime.IGateProvider.Handle", "ti.sdo.utils.loggers"), $$UNDEF, "w");
            po.addFldV("Module_GateProxy", (Proto)om.findStrict("xdc.runtime.IGateProvider.Module", "ti.sdo.utils.loggers"), null, "wh", $$delegGet, $$delegSet);
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID$$create", "ti.sdo.utils.loggers"), Global.get("ti$sdo$utils$loggers$LoggerSysTID$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID$$construct", "ti.sdo.utils.loggers"), Global.get("ti$sdo$utils$loggers$LoggerSysTID$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sdo.utils.loggers.LoggerSysTID$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sdo.utils.loggers.LoggerSysTID$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sdo.utils.loggers.LoggerSysTID$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sdo.utils.loggers.LoggerSysTID$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sdo.utils.loggers.LoggerSysTID$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sdo.utils.loggers.LoggerSysTID$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
                fxn = Global.get(cap, "getMetaArgs");
                if (fxn != null) po.addFxn("getMetaArgs", (Proto.Fxn)om.findStrict("xdc.runtime.ILogger$$getMetaArgs", "ti.sdo.utils.loggers"), fxn);
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Instance", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID.Instance", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Instance", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.utils.loggers"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID$$Params", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID.Params", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP$$Params", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.utils.loggers"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID$$Object", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID.Object", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Instance", "ti.sdo.utils.loggers"));
        // struct LoggerSysTID.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID$$Instance_State", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("enabled", $$T_Bool, $$UNDEF, "w");
    }

    void LoggerMFPNull$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Module", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerMFPNull.Module", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Module", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull$$create", "ti.sdo.utils.loggers"), Global.get("ti$sdo$utils$loggers$LoggerMFPNull$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull$$construct", "ti.sdo.utils.loggers"), Global.get("ti$sdo$utils$loggers$LoggerMFPNull$$construct"));
        }//isCFG
                po.addFxn("getMetaArgs", (Proto.Fxn)om.findStrict("xdc.runtime.ILogger$$getMetaArgs", "ti.sdo.utils.loggers"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Instance", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerMFPNull.Instance", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Instance", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.utils.loggers"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull$$Params", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerMFPNull.Params", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP$$Params", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.utils.loggers"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull$$Object", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerMFPNull.Object", om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Instance", "ti.sdo.utils.loggers"));
        // struct LoggerMFPNull.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull$$Instance_State", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerMFPNull.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("enabled", $$T_Bool, $$UNDEF, "w");
    }

    void LogMFP$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/utils/loggers/LogMFP.xs");
        om.bind("ti.sdo.utils.loggers.LogMFP$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP.Module", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LogMFP.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFldV("Proxy", (Proto)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Module", "ti.sdo.utils.loggers"), null, "wh", $$delegGet, $$delegSet);
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sdo.utils.loggers.LogMFP$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sdo.utils.loggers.LogMFP$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sdo.utils.loggers.LogMFP$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sdo.utils.loggers.LogMFP$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void LoggerSysTID_TimestampProxy$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy.Module", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy.Module", om.findStrict("xdc.runtime.ITimestampClient.Module", "ti.sdo.utils.loggers"));
                po.addFld("delegate$", (Proto)om.findStrict("xdc.runtime.ITimestampClient.Module", "ti.sdo.utils.loggers"), null, "wh");
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
    }

    void LoggerSysTID_Module_GateProxy$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Module", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Module", om.findStrict("xdc.runtime.IGateProvider.Module", "ti.sdo.utils.loggers"));
                po.addFld("delegate$", (Proto)om.findStrict("xdc.runtime.IGateProvider.Module", "ti.sdo.utils.loggers"), null, "wh");
                po.addFld("abstractInstances$", $$T_Bool, false, "wh");
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
        if (isCFG) {
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy$$create", "ti.sdo.utils.loggers"), Global.get("ti$sdo$utils$loggers$LoggerSysTID_Module_GateProxy$$create"));
        }//isCFG
                po.addFxn("queryMeta", (Proto.Fxn)om.findStrict("xdc.runtime.IGateProvider$$queryMeta", "ti.sdo.utils.loggers"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance", om.findStrict("xdc.runtime.IGateProvider.Instance", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.utils.loggers"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy$$Params", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Params", om.findStrict("xdc.runtime.IGateProvider$$Params", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.utils.loggers"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy$$Object", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Object", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance", "ti.sdo.utils.loggers"));
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy$$Instance_State", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance_State", null);
        po.addFld("$hostonly", $$T_Num, 0, "r");
    }

    void LogMFP_Proxy$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Module", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LogMFP_Proxy.Module", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Module", "ti.sdo.utils.loggers"));
                po.addFld("delegate$", (Proto)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Module", "ti.sdo.utils.loggers"), null, "wh");
                po.addFld("abstractInstances$", $$T_Bool, false, "wh");
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy$$create", "ti.sdo.utils.loggers"), Global.get("ti$sdo$utils$loggers$LogMFP_Proxy$$create"));
        }//isCFG
                po.addFxn("getMetaArgs", (Proto.Fxn)om.findStrict("xdc.runtime.ILogger$$getMetaArgs", "ti.sdo.utils.loggers"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Instance", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LogMFP_Proxy.Instance", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Instance", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.utils.loggers"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy$$Params", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LogMFP_Proxy.Params", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP$$Params", "ti.sdo.utils.loggers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.utils.loggers"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy$$Object", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LogMFP_Proxy.Object", om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Instance", "ti.sdo.utils.loggers"));
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy$$Instance_State", "ti.sdo.utils.loggers");
        po.init("ti.sdo.utils.loggers.LogMFP_Proxy.Instance_State", null);
        po.addFld("$hostonly", $$T_Num, 0, "r");
    }

    void ILoggerMFP$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP$$Instance_State", "ti.sdo.utils.loggers");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
    }

    void LoggerSysTID$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID$$Instance_State", "ti.sdo.utils.loggers");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sdo.utils.loggers.LoggerSysTID.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID$$Instance_State", "ti.sdo.utils.loggers");
    }

    void LoggerMFPNull$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull$$Instance_State", "ti.sdo.utils.loggers");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sdo.utils.loggers.LoggerMFPNull.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull$$Instance_State", "ti.sdo.utils.loggers");
    }

    void LogMFP$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP", "ti.sdo.utils.loggers");
    }

    void LoggerSysTID_TimestampProxy$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy", "ti.sdo.utils.loggers");
    }

    void LoggerSysTID_Module_GateProxy$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy$$Instance_State", "ti.sdo.utils.loggers");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
    }

    void LogMFP_Proxy$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy$$Instance_State", "ti.sdo.utils.loggers");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sdo.utils.loggers.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sdo.utils.loggers"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/utils/loggers/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sdo.utils.loggers"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sdo.utils.loggers"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sdo.utils.loggers"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sdo.utils.loggers"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sdo.utils.loggers"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sdo.utils.loggers"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sdo.utils.loggers", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sdo.utils.loggers");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sdo.utils.loggers.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sdo.utils.loggers'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/debug/loggers.a64P',\n");
            sb.append("'lib/release/loggers.a64P',\n");
            sb.append("'lib/whole_program/loggers.a64P',\n");
            sb.append("'lib/whole_program_debug/loggers.a64P',\n");
            sb.append("'lib/debug/loggers.ae64P',\n");
            sb.append("'lib/release/loggers.ae64P',\n");
            sb.append("'lib/whole_program/loggers.ae64P',\n");
            sb.append("'lib/whole_program_debug/loggers.ae64P',\n");
            sb.append("'lib/debug/loggers.a674',\n");
            sb.append("'lib/release/loggers.a674',\n");
            sb.append("'lib/whole_program/loggers.a674',\n");
            sb.append("'lib/whole_program_debug/loggers.a674',\n");
            sb.append("'lib/debug/loggers.ae674',\n");
            sb.append("'lib/release/loggers.ae674',\n");
            sb.append("'lib/whole_program/loggers.ae674',\n");
            sb.append("'lib/whole_program_debug/loggers.ae674',\n");
            sb.append("'lib/debug/loggers.ae64T',\n");
            sb.append("'lib/release/loggers.ae64T',\n");
            sb.append("'lib/whole_program/loggers.ae64T',\n");
            sb.append("'lib/whole_program_debug/loggers.ae64T',\n");
            sb.append("'lib/debug/loggers.aem3',\n");
            sb.append("'lib/release/loggers.aem3',\n");
            sb.append("'lib/whole_program/loggers.aem3',\n");
            sb.append("'lib/whole_program_debug/loggers.aem3',\n");
            sb.append("'lib/debug/loggers.a86U',\n");
            sb.append("'lib/release/loggers.a86U',\n");
            sb.append("'lib/debug/loggers.av5T',\n");
            sb.append("'lib/release/loggers.av5T',\n");
            sb.append("'lib/release/loggers.aqv7',\n");
            sb.append("'lib/debug/loggers.aqv7',\n");
            sb.append("'lib/debug/loggers.abnc',\n");
            sb.append("'lib/release/loggers.abnc',\n");
            sb.append("'lib/debug/loggers.a470uC',\n");
            sb.append("'lib/release/loggers.a470uC',\n");
            sb.append("'lib/debug/loggers.aea8f',\n");
            sb.append("'lib/release/loggers.aea8f',\n");
            sb.append("'lib/whole_program/loggers.aea8f',\n");
            sb.append("'lib/whole_program_debug/loggers.aea8f',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/debug/loggers.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/release/loggers.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/whole_program/loggers.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/whole_program_debug/loggers.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/debug/loggers.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/release/loggers.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/whole_program/loggers.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/whole_program_debug/loggers.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/debug/loggers.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/release/loggers.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/whole_program/loggers.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/whole_program_debug/loggers.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/debug/loggers.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/release/loggers.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/whole_program/loggers.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/whole_program_debug/loggers.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/debug/loggers.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/release/loggers.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/whole_program/loggers.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/whole_program_debug/loggers.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/debug/loggers.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/release/loggers.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/whole_program/loggers.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/whole_program_debug/loggers.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/debug/loggers.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/release/loggers.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/debug/loggers.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/release/loggers.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/release/loggers.aqv7', {target: 'qnx.targets.arm.Qnx', suffix: 'qv7'}],\n");
            sb.append("['lib/debug/loggers.aqv7', {target: 'qnx.targets.arm.Qnx', suffix: 'qv7'}],\n");
            sb.append("['lib/debug/loggers.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/release/loggers.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/debug/loggers.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/release/loggers.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/debug/loggers.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
            sb.append("['lib/release/loggers.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
            sb.append("['lib/whole_program/loggers.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
            sb.append("['lib/whole_program_debug/loggers.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void ILoggerMFP$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Module", "ti.sdo.utils.loggers");
        vo.init2(po, "ti.sdo.utils.loggers.ILoggerMFP", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Interface");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Instance", "ti.sdo.utils.loggers"));
        vo.bind("Params", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Params", "ti.sdo.utils.loggers"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Params", "ti.sdo.utils.loggers")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Handle", "ti.sdo.utils.loggers"));
        vo.bind("$package", om.findStrict("ti.sdo.utils.loggers", "ti.sdo.utils.loggers"));
        tdefs.clear();
        proxies.clear();
        inherits.clear();
        vo.bind("Cmd", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Cmd", "ti.sdo.utils.loggers"));
        vo.bind("SETTRACE", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.SETTRACE", "ti.sdo.utils.loggers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$interfaces")).add(vo);
        pkgV.bind("ILoggerMFP", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ILoggerMFP");
        vo.seal(null);
    }

    void LoggerSysTID$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Module", "ti.sdo.utils.loggers");
        vo.init2(po, "ti.sdo.utils.loggers.LoggerSysTID", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID$$capsule", "ti.sdo.utils.loggers"));
        vo.bind("Instance", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Instance", "ti.sdo.utils.loggers"));
        vo.bind("Params", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Params", "ti.sdo.utils.loggers"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Params", "ti.sdo.utils.loggers")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Handle", "ti.sdo.utils.loggers"));
        vo.bind("$package", om.findStrict("ti.sdo.utils.loggers", "ti.sdo.utils.loggers"));
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
        vo.bind("Cmd", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Cmd", "ti.sdo.utils.loggers"));
        vo.bind("TSMode", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.TSMode", "ti.sdo.utils.loggers"));
        vo.bind("TimestampProxy$proxy", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy", "ti.sdo.utils.loggers"));
        proxies.add("TimestampProxy");
        mcfgs.add("gate");
        vo.bind("Instance_State", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Instance_State", "ti.sdo.utils.loggers"));
        tdefs.add(om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Instance_State", "ti.sdo.utils.loggers"));
        vo.bind("Module_GateProxy$proxy", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy", "ti.sdo.utils.loggers"));
        proxies.add("Module_GateProxy");
        vo.bind("SETTRACE", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.SETTRACE", "ti.sdo.utils.loggers"));
        vo.bind("TSMode_USEC", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.TSMode_USEC", "ti.sdo.utils.loggers"));
        vo.bind("TSMode_SEC", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.TSMode_SEC", "ti.sdo.utils.loggers"));
        vo.bind("TSMode_DELTAUSEC", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.TSMode_DELTAUSEC", "ti.sdo.utils.loggers"));
        vo.bind("TSMode_TICKS", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.TSMode_TICKS", "ti.sdo.utils.loggers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sdo.utils.loggers");
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.utils.loggers")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("ti.sdo.utils.loggers.LoggerSysTID$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sdo$utils$loggers$LoggerSysTID$$__initObject"));
        }//isCFG
        vo.bind("control", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.control", "ti.sdo.utils.loggers"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_utils_loggers_LoggerSysTID_Handle__label__E", "ti_sdo_utils_loggers_LoggerSysTID_Module__startupDone__E", "ti_sdo_utils_loggers_LoggerSysTID_Object__create__E", "ti_sdo_utils_loggers_LoggerSysTID_Object__delete__E", "ti_sdo_utils_loggers_LoggerSysTID_Object__destruct__E", "ti_sdo_utils_loggers_LoggerSysTID_Object__get__E", "ti_sdo_utils_loggers_LoggerSysTID_Object__first__E", "ti_sdo_utils_loggers_LoggerSysTID_Object__next__E", "ti_sdo_utils_loggers_LoggerSysTID_Params__init__E", "ti_sdo_utils_loggers_LoggerSysTID_enable__E", "ti_sdo_utils_loggers_LoggerSysTID_disable__E", "ti_sdo_utils_loggers_LoggerSysTID_write0__E", "ti_sdo_utils_loggers_LoggerSysTID_write1__E", "ti_sdo_utils_loggers_LoggerSysTID_write2__E", "ti_sdo_utils_loggers_LoggerSysTID_write4__E", "ti_sdo_utils_loggers_LoggerSysTID_write8__E", "ti_sdo_utils_loggers_LoggerSysTID_control__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID.Object", "ti.sdo.utils.loggers"));
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "enable", "entry", "%p", "exit", "%d"));
        loggables.add(Global.newObject("name", "disable", "entry", "%p", "exit", "%d"));
        loggables.add(Global.newObject("name", "write0", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write1", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write2", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write4", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write8", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "control", "entry", "%d, %p", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("LoggerSysTID", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LoggerSysTID");
    }

    void LoggerMFPNull$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Module", "ti.sdo.utils.loggers");
        vo.init2(po, "ti.sdo.utils.loggers.LoggerMFPNull", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Instance", "ti.sdo.utils.loggers"));
        vo.bind("Params", om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Params", "ti.sdo.utils.loggers"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Params", "ti.sdo.utils.loggers")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Handle", "ti.sdo.utils.loggers"));
        vo.bind("$package", om.findStrict("ti.sdo.utils.loggers", "ti.sdo.utils.loggers"));
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
        vo.bind("Cmd", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Cmd", "ti.sdo.utils.loggers"));
        vo.bind("Instance_State", om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Instance_State", "ti.sdo.utils.loggers"));
        tdefs.add(om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Instance_State", "ti.sdo.utils.loggers"));
        vo.bind("SETTRACE", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.SETTRACE", "ti.sdo.utils.loggers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sdo.utils.loggers");
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.utils.loggers")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("ti.sdo.utils.loggers.LoggerMFPNull$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sdo$utils$loggers$LoggerMFPNull$$__initObject"));
        }//isCFG
        vo.bind("control", om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.control", "ti.sdo.utils.loggers"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_utils_loggers_LoggerMFPNull_Handle__label__E", "ti_sdo_utils_loggers_LoggerMFPNull_Module__startupDone__E", "ti_sdo_utils_loggers_LoggerMFPNull_Object__create__E", "ti_sdo_utils_loggers_LoggerMFPNull_Object__delete__E", "ti_sdo_utils_loggers_LoggerMFPNull_Object__destruct__E", "ti_sdo_utils_loggers_LoggerMFPNull_Object__get__E", "ti_sdo_utils_loggers_LoggerMFPNull_Object__first__E", "ti_sdo_utils_loggers_LoggerMFPNull_Object__next__E", "ti_sdo_utils_loggers_LoggerMFPNull_Params__init__E", "ti_sdo_utils_loggers_LoggerMFPNull_enable__E", "ti_sdo_utils_loggers_LoggerMFPNull_disable__E", "ti_sdo_utils_loggers_LoggerMFPNull_write0__E", "ti_sdo_utils_loggers_LoggerMFPNull_write1__E", "ti_sdo_utils_loggers_LoggerMFPNull_write2__E", "ti_sdo_utils_loggers_LoggerMFPNull_write4__E", "ti_sdo_utils_loggers_LoggerMFPNull_write8__E", "ti_sdo_utils_loggers_LoggerMFPNull_control__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull.Object", "ti.sdo.utils.loggers"));
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "enable", "entry", "%p", "exit", "%d"));
        loggables.add(Global.newObject("name", "disable", "entry", "%p", "exit", "%d"));
        loggables.add(Global.newObject("name", "write0", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write1", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write2", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write4", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write8", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "control", "entry", "%d, %p", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("LoggerMFPNull", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LoggerMFPNull");
    }

    void LogMFP$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP.Module", "ti.sdo.utils.loggers");
        vo.init2(po, "ti.sdo.utils.loggers.LogMFP", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sdo.utils.loggers.LogMFP$$capsule", "ti.sdo.utils.loggers"));
        vo.bind("$package", om.findStrict("ti.sdo.utils.loggers", "ti.sdo.utils.loggers"));
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
        vo.bind("Proxy$proxy", om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy", "ti.sdo.utils.loggers"));
        proxies.add("Proxy");
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.utils.loggers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("control", om.findStrict("ti.sdo.utils.loggers.LogMFP.control", "ti.sdo.utils.loggers"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_utils_loggers_LogMFP_Module__startupDone__E", "ti_sdo_utils_loggers_LogMFP_control__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "control", "entry", "%d, %p", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("LogMFP", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LogMFP");
    }

    void LoggerSysTID_TimestampProxy$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy.Module", "ti.sdo.utils.loggers");
        vo.init2(po, "ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("$package", om.findStrict("ti.sdo.utils.loggers", "ti.sdo.utils.loggers"));
        tdefs.clear();
        proxies.clear();
        proxies.add("delegate$");
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.utils.loggers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 0);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("get32", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy.get32", "ti.sdo.utils.loggers"));
        vo.bind("get64", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy.get64", "ti.sdo.utils.loggers"));
        vo.bind("getFreq", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy.getFreq", "ti.sdo.utils.loggers"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Handle__label", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Module__startupDone", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Object__create", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Object__delete", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Object__destruct", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Object__get", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Object__first", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Object__next", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Params__init", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Proxy__abstract", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__Proxy__delegate", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__get32", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__get64", "ti_sdo_utils_loggers_LoggerSysTID_TimestampProxy_DELEGATE__getFreq"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 1);
        loggables.clear();
        loggables.add(Global.newObject("name", "get32", "entry", "", "exit", "0x%x"));
        loggables.add(Global.newObject("name", "get64", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "getFreq", "entry", "%p", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("LoggerSysTID_TimestampProxy", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LoggerSysTID_TimestampProxy");
    }

    void LoggerSysTID_Module_GateProxy$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Module", "ti.sdo.utils.loggers");
        vo.init2(po, "ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance", "ti.sdo.utils.loggers"));
        vo.bind("Params", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Params", "ti.sdo.utils.loggers"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Params", "ti.sdo.utils.loggers")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Handle", "ti.sdo.utils.loggers"));
        vo.bind("$package", om.findStrict("ti.sdo.utils.loggers", "ti.sdo.utils.loggers"));
        tdefs.clear();
        proxies.clear();
        proxies.add("delegate$");
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.utils.loggers")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 0);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
            vo.bind("__initObject", Global.get("ti$sdo$utils$loggers$LoggerSysTID_Module_GateProxy$$__initObject"));
        }//isCFG
        vo.bind("query", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.query", "ti.sdo.utils.loggers"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Handle__label", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Module__startupDone", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Object__create", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Object__delete", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Object__destruct", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Object__get", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Object__first", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Object__next", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Params__init", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Proxy__abstract", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__Proxy__delegate", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__queryMeta", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__query", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__enter", "ti_sdo_utils_loggers_LoggerSysTID_Module_GateProxy_DELEGATE__leave"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Object", "ti.sdo.utils.loggers"));
        vo.bind("Instance_State", om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy.Instance_State", "ti.sdo.utils.loggers"));
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 1);
        loggables.clear();
        loggables.add(Global.newObject("name", "query", "entry", "%d", "exit", "%d"));
        loggables.add(Global.newObject("name", "enter", "entry", "%p", "exit", "%p"));
        loggables.add(Global.newObject("name", "leave", "entry", "%p, %p", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("LoggerSysTID_Module_GateProxy", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LoggerSysTID_Module_GateProxy");
    }

    void LogMFP_Proxy$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy", "ti.sdo.utils.loggers");
        po = (Proto.Obj)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Module", "ti.sdo.utils.loggers");
        vo.init2(po, "ti.sdo.utils.loggers.LogMFP_Proxy", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Instance", "ti.sdo.utils.loggers"));
        vo.bind("Params", om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Params", "ti.sdo.utils.loggers"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Params", "ti.sdo.utils.loggers")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Handle", "ti.sdo.utils.loggers"));
        vo.bind("$package", om.findStrict("ti.sdo.utils.loggers", "ti.sdo.utils.loggers"));
        tdefs.clear();
        proxies.clear();
        proxies.add("delegate$");
        inherits.clear();
        vo.bind("Cmd", om.findStrict("ti.sdo.utils.loggers.ILoggerMFP.Cmd", "ti.sdo.utils.loggers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("ti.sdo.utils.loggers");
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.utils.loggers")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 0);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
            vo.bind("__initObject", Global.get("ti$sdo$utils$loggers$LogMFP_Proxy$$__initObject"));
        }//isCFG
        vo.bind("control", om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.control", "ti.sdo.utils.loggers"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Handle__label", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Module__startupDone", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Object__create", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Object__delete", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Object__destruct", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Object__get", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Object__first", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Object__next", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Params__init", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Proxy__abstract", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__Proxy__delegate", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__getMetaArgs", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__enable", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__disable", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__write0", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__write1", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__write2", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__write4", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__write8", "ti_sdo_utils_loggers_LogMFP_Proxy_DELEGATE__control"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Object", "ti.sdo.utils.loggers"));
        vo.bind("Instance_State", om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy.Instance_State", "ti.sdo.utils.loggers"));
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 1);
        loggables.clear();
        loggables.add(Global.newObject("name", "enable", "entry", "%p", "exit", "%d"));
        loggables.add(Global.newObject("name", "disable", "entry", "%p", "exit", "%d"));
        loggables.add(Global.newObject("name", "write0", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write1", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write2", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write4", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "write8", "entry", "%p", "exit", ""));
        loggables.add(Global.newObject("name", "control", "entry", "%d, %p", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("LogMFP_Proxy", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LogMFP_Proxy");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
            Object srcP = ((XScriptO)om.findStrict("xdc.runtime.IInstance", "ti.sdo.utils.loggers")).findStrict("PARAMS", "ti.sdo.utils.loggers");
            Scriptable dstP;

            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID", "ti.sdo.utils.loggers")).findStrict("PARAMS", "ti.sdo.utils.loggers");
            Global.put(dstP, "instance", srcP);
            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull", "ti.sdo.utils.loggers")).findStrict("PARAMS", "ti.sdo.utils.loggers");
            Global.put(dstP, "instance", srcP);
            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy", "ti.sdo.utils.loggers")).findStrict("PARAMS", "ti.sdo.utils.loggers");
            Global.put(dstP, "instance", srcP);
            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy", "ti.sdo.utils.loggers")).findStrict("PARAMS", "ti.sdo.utils.loggers");
            Global.put(dstP, "instance", srcP);
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID", "ti.sdo.utils.loggers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.utils.loggers.LoggerMFPNull", "ti.sdo.utils.loggers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.utils.loggers.LogMFP", "ti.sdo.utils.loggers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy", "ti.sdo.utils.loggers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy", "ti.sdo.utils.loggers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.utils.loggers.LogMFP_Proxy", "ti.sdo.utils.loggers"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sdo.utils.loggers.ILoggerMFP")).bless();
        ((Value.Obj)om.getv("ti.sdo.utils.loggers.LoggerSysTID")).bless();
        ((Value.Obj)om.getv("ti.sdo.utils.loggers.LoggerMFPNull")).bless();
        ((Value.Obj)om.getv("ti.sdo.utils.loggers.LogMFP")).bless();
        ((Value.Obj)om.getv("ti.sdo.utils.loggers.LoggerSysTID_TimestampProxy")).bless();
        ((Value.Obj)om.getv("ti.sdo.utils.loggers.LoggerSysTID_Module_GateProxy")).bless();
        ((Value.Obj)om.getv("ti.sdo.utils.loggers.LogMFP_Proxy")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sdo.utils.loggers")).add(pkgV);
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
        ILoggerMFP$$OBJECTS();
        LoggerSysTID$$OBJECTS();
        LoggerMFPNull$$OBJECTS();
        LogMFP$$OBJECTS();
        LoggerSysTID_TimestampProxy$$OBJECTS();
        LoggerSysTID_Module_GateProxy$$OBJECTS();
        LogMFP_Proxy$$OBJECTS();
        ILoggerMFP$$CONSTS();
        LoggerSysTID$$CONSTS();
        LoggerMFPNull$$CONSTS();
        LogMFP$$CONSTS();
        LoggerSysTID_TimestampProxy$$CONSTS();
        LoggerSysTID_Module_GateProxy$$CONSTS();
        LogMFP_Proxy$$CONSTS();
        ILoggerMFP$$CREATES();
        LoggerSysTID$$CREATES();
        LoggerMFPNull$$CREATES();
        LogMFP$$CREATES();
        LoggerSysTID_TimestampProxy$$CREATES();
        LoggerSysTID_Module_GateProxy$$CREATES();
        LogMFP_Proxy$$CREATES();
        ILoggerMFP$$FUNCTIONS();
        LoggerSysTID$$FUNCTIONS();
        LoggerMFPNull$$FUNCTIONS();
        LogMFP$$FUNCTIONS();
        LoggerSysTID_TimestampProxy$$FUNCTIONS();
        LoggerSysTID_Module_GateProxy$$FUNCTIONS();
        LogMFP_Proxy$$FUNCTIONS();
        ILoggerMFP$$SIZES();
        LoggerSysTID$$SIZES();
        LoggerMFPNull$$SIZES();
        LogMFP$$SIZES();
        LoggerSysTID_TimestampProxy$$SIZES();
        LoggerSysTID_Module_GateProxy$$SIZES();
        LogMFP_Proxy$$SIZES();
        ILoggerMFP$$TYPES();
        LoggerSysTID$$TYPES();
        LoggerMFPNull$$TYPES();
        LogMFP$$TYPES();
        LoggerSysTID_TimestampProxy$$TYPES();
        LoggerSysTID_Module_GateProxy$$TYPES();
        LogMFP_Proxy$$TYPES();
        if (isROV) {
            ILoggerMFP$$ROV();
            LoggerSysTID$$ROV();
            LoggerMFPNull$$ROV();
            LogMFP$$ROV();
            LoggerSysTID_TimestampProxy$$ROV();
            LoggerSysTID_Module_GateProxy$$ROV();
            LogMFP_Proxy$$ROV();
        }//isROV
        $$SINGLETONS();
        ILoggerMFP$$SINGLETONS();
        LoggerSysTID$$SINGLETONS();
        LoggerMFPNull$$SINGLETONS();
        LogMFP$$SINGLETONS();
        LoggerSysTID_TimestampProxy$$SINGLETONS();
        LoggerSysTID_Module_GateProxy$$SINGLETONS();
        LogMFP_Proxy$$SINGLETONS();
        $$INITIALIZATION();
    }
}
