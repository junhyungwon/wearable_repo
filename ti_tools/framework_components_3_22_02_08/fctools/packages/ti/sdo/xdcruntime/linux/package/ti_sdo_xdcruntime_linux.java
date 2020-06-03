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

public class ti_sdo_xdcruntime_linux
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
        Global.callFxn("loadPackage", xdcO, "xdc.runtime.knl");
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
        Global.callFxn("loadPackage", xdcO, "xdc.runtime");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sdo.xdcruntime.linux", new Value.Obj("ti.sdo.xdcruntime.linux", pkgP));
    }

    void GateThreadSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport", new Value.Obj("ti.sdo.xdcruntime.linux.GateThreadSupport", po));
        pkgV.bind("GateThreadSupport", vo);
        // decls 
        spo = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$Params", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.Params", new Proto.Str(po, false));
        om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.Object", om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        }//isROV
    }

    void GateProcessSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport", new Value.Obj("ti.sdo.xdcruntime.linux.GateProcessSupport", po));
        pkgV.bind("GateProcessSupport", vo);
        // decls 
        spo = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$Params", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport.Params", new Proto.Str(po, false));
        om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport.Object", om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        }//isROV
    }

    void SemThreadSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport", new Value.Obj("ti.sdo.xdcruntime.linux.SemThreadSupport", po));
        pkgV.bind("SemThreadSupport", vo);
        // decls 
        om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.PendStatus", om.findStrict("xdc.runtime.knl.ISemaphore.PendStatus", "ti.sdo.xdcruntime.linux"));
        om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.Mode", om.findStrict("xdc.runtime.knl.ISemaphore.Mode", "ti.sdo.xdcruntime.linux"));
        spo = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance_State", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$Module_State", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.Module_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$Params", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.Params", new Proto.Str(po, false));
        om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.Object", om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        }//isROV
    }

    void SemProcessSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport", new Value.Obj("ti.sdo.xdcruntime.linux.SemProcessSupport", po));
        pkgV.bind("SemProcessSupport", vo);
        // decls 
        om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport.PendStatus", om.findStrict("xdc.runtime.knl.ISemaphore.PendStatus", "ti.sdo.xdcruntime.linux"));
        om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport.Mode", om.findStrict("xdc.runtime.knl.ISemaphore.Mode", "ti.sdo.xdcruntime.linux"));
        spo = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$Params", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport.Params", new Proto.Str(po, false));
        om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport.Object", om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        }//isROV
    }

    void ThreadSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.xdcruntime.linux.ThreadSupport", new Value.Obj("ti.sdo.xdcruntime.linux.ThreadSupport", po));
        pkgV.bind("ThreadSupport", vo);
        // decls 
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.Priority", om.findStrict("xdc.runtime.knl.IThreadSupport.Priority", "ti.sdo.xdcruntime.linux"));
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.CompStatus", om.findStrict("xdc.runtime.knl.IThreadSupport.CompStatus", "ti.sdo.xdcruntime.linux"));
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.Stat", om.findStrict("xdc.runtime.knl.IThreadSupport.Stat", "ti.sdo.xdcruntime.linux"));
        spo = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.Instance_State", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$Module_State", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.Module_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$Params", new Proto.Obj());
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.Params", new Proto.Str(po, false));
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.Object", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        }//isROV
    }

    void TimestampPosix$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.TimestampPosix.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.xdcruntime.linux.TimestampPosix", new Value.Obj("ti.sdo.xdcruntime.linux.TimestampPosix", po));
        pkgV.bind("TimestampPosix", vo);
        // decls 
    }

    void Settings$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.xdcruntime.linux.Settings.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.xdcruntime.linux.Settings", new Value.Obj("ti.sdo.xdcruntime.linux.Settings", po));
        pkgV.bind("Settings", vo);
        // decls 
    }

    void GateThreadSupport$$CONSTS()
    {
        // module GateThreadSupport
        om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.SETUP_FAILURE", 3L);
        om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.ALLOC_FAILURE", 2L);
        om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.POSIX_FAILURE", 1L);
        om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport.query", new Extern("ti_sdo_xdcruntime_linux_GateThreadSupport_query__E", "xdc_Bool(*)(xdc_Int)", true, false));
    }

    void GateProcessSupport$$CONSTS()
    {
        // module GateProcessSupport
        om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport.POSIX_FAILURE", 1L);
        om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport.query", new Extern("ti_sdo_xdcruntime_linux_GateProcessSupport_query__E", "xdc_Bool(*)(xdc_Int)", true, false));
    }

    void SemThreadSupport$$CONSTS()
    {
        // module SemThreadSupport
        om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.ALLOC_FAILURE", 2L);
        om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport.POSIX_FAILURE", 1L);
    }

    void SemProcessSupport$$CONSTS()
    {
        // module SemProcessSupport
        om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport.POSIX_FAILURE", 1L);
    }

    void ThreadSupport$$CONSTS()
    {
        // module ThreadSupport
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.PRI_FAILURE", 4L);
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.SETUP_FAILURE", 3L);
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.ALLOC_FAILURE", 2L);
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.POSIX_FAILURE", 1L);
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.self", new Extern("ti_sdo_xdcruntime_linux_ThreadSupport_self__E", "xdc_runtime_knl_IThreadSupport_Handle(*)(xdc_runtime_Error_Block*)", true, false));
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.start", new Extern("ti_sdo_xdcruntime_linux_ThreadSupport_start__E", "xdc_Bool(*)(xdc_runtime_Error_Block*)", true, false));
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.yield", new Extern("ti_sdo_xdcruntime_linux_ThreadSupport_yield__E", "xdc_Bool(*)(xdc_runtime_Error_Block*)", true, false));
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.compareOsPriorities", new Extern("ti_sdo_xdcruntime_linux_ThreadSupport_compareOsPriorities__E", "xdc_Int(*)(xdc_Int,xdc_Int,xdc_runtime_Error_Block*)", true, false));
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport.sleep", new Extern("ti_sdo_xdcruntime_linux_ThreadSupport_sleep__E", "xdc_Bool(*)(xdc_UInt,xdc_runtime_Error_Block*)", true, false));
    }

    void TimestampPosix$$CONSTS()
    {
        // module TimestampPosix
        om.bind("ti.sdo.xdcruntime.linux.TimestampPosix.get32", new Extern("ti_sdo_xdcruntime_linux_TimestampPosix_get32__E", "xdc_Bits32(*)(xdc_Void)", true, false));
        om.bind("ti.sdo.xdcruntime.linux.TimestampPosix.get64", new Extern("ti_sdo_xdcruntime_linux_TimestampPosix_get64__E", "xdc_Void(*)(xdc_runtime_Types_Timestamp64*)", true, false));
        om.bind("ti.sdo.xdcruntime.linux.TimestampPosix.getFreq", new Extern("ti_sdo_xdcruntime_linux_TimestampPosix_getFreq__E", "xdc_Void(*)(xdc_runtime_Types_FreqHz*)", true, false));
    }

    void Settings$$CONSTS()
    {
        // module Settings
    }

    void GateThreadSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$GateThreadSupport$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sdo.xdcruntime.linux.GateThreadSupport.create() called before xdc.useModule('ti.sdo.xdcruntime.linux.GateThreadSupport')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$create", new Proto.Fxn(om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Module", "ti.sdo.xdcruntime.linux"), om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance", "ti.sdo.xdcruntime.linux"), 1, 0, false));
                        fxn.addArg(0, "__params", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Params", "ti.sdo.xdcruntime.linux"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$GateThreadSupport$$create = function( __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.xdcruntime.linux.GateThreadSupport'];\n");
                sb.append("var __inst = xdc.om['ti.sdo.xdcruntime.linux.GateThreadSupport.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sdo.xdcruntime.linux']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sdo.xdcruntime.linux.GateThreadSupport'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.xdcruntime.linux.GateThreadSupport'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$construct", new Proto.Fxn(om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Module", "ti.sdo.xdcruntime.linux"), null, 2, 0, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport$$Object", "ti.sdo.xdcruntime.linux"), null);
                        fxn.addArg(1, "__params", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Params", "ti.sdo.xdcruntime.linux"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$GateThreadSupport$$construct = function( __obj, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.xdcruntime.linux.GateThreadSupport'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sdo.xdcruntime.linux.GateThreadSupport'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.xdcruntime.linux.GateThreadSupport'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void GateProcessSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$GateProcessSupport$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sdo.xdcruntime.linux.GateProcessSupport.create() called before xdc.useModule('ti.sdo.xdcruntime.linux.GateProcessSupport')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$create", new Proto.Fxn(om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Module", "ti.sdo.xdcruntime.linux"), om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance", "ti.sdo.xdcruntime.linux"), 1, 0, false));
                        fxn.addArg(0, "__params", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Params", "ti.sdo.xdcruntime.linux"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$GateProcessSupport$$create = function( __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.xdcruntime.linux.GateProcessSupport'];\n");
                sb.append("var __inst = xdc.om['ti.sdo.xdcruntime.linux.GateProcessSupport.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sdo.xdcruntime.linux']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sdo.xdcruntime.linux.GateProcessSupport'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.xdcruntime.linux.GateProcessSupport'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$construct", new Proto.Fxn(om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Module", "ti.sdo.xdcruntime.linux"), null, 2, 0, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport$$Object", "ti.sdo.xdcruntime.linux"), null);
                        fxn.addArg(1, "__params", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Params", "ti.sdo.xdcruntime.linux"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$GateProcessSupport$$construct = function( __obj, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.xdcruntime.linux.GateProcessSupport'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sdo.xdcruntime.linux.GateProcessSupport'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.xdcruntime.linux.GateProcessSupport'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void SemThreadSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$SemThreadSupport$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sdo.xdcruntime.linux.SemThreadSupport.create() called before xdc.useModule('ti.sdo.xdcruntime.linux.SemThreadSupport')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst.$args.count, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$create", new Proto.Fxn(om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Module", "ti.sdo.xdcruntime.linux"), om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance", "ti.sdo.xdcruntime.linux"), 2, 1, false));
                        fxn.addArg(0, "count", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(1, "__params", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Params", "ti.sdo.xdcruntime.linux"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$SemThreadSupport$$create = function( count, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport'];\n");
                sb.append("var __inst = xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sdo.xdcruntime.linux']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {count:count});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [count]);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$construct", new Proto.Fxn(om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Module", "ti.sdo.xdcruntime.linux"), null, 3, 1, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$Object", "ti.sdo.xdcruntime.linux"), null);
                        fxn.addArg(1, "count", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(2, "__params", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Params", "ti.sdo.xdcruntime.linux"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$SemThreadSupport$$construct = function( __obj, count, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {count:count});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void SemProcessSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$SemProcessSupport$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sdo.xdcruntime.linux.SemProcessSupport.create() called before xdc.useModule('ti.sdo.xdcruntime.linux.SemProcessSupport')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst.$args.count, inst.$args.key, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$create", new Proto.Fxn(om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Module", "ti.sdo.xdcruntime.linux"), om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance", "ti.sdo.xdcruntime.linux"), 3, 2, false));
                        fxn.addArg(0, "count", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(1, "key", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(2, "__params", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Params", "ti.sdo.xdcruntime.linux"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$SemProcessSupport$$create = function( count, key, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.xdcruntime.linux.SemProcessSupport'];\n");
                sb.append("var __inst = xdc.om['ti.sdo.xdcruntime.linux.SemProcessSupport.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sdo.xdcruntime.linux']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {count:count, key:key});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sdo.xdcruntime.linux.SemProcessSupport'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [count, key]);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.xdcruntime.linux.SemProcessSupport'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$construct", new Proto.Fxn(om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Module", "ti.sdo.xdcruntime.linux"), null, 4, 2, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport$$Object", "ti.sdo.xdcruntime.linux"), null);
                        fxn.addArg(1, "count", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(2, "key", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(3, "__params", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Params", "ti.sdo.xdcruntime.linux"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$SemProcessSupport$$construct = function( __obj, count, key, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.xdcruntime.linux.SemProcessSupport'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {count:count, key:key});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sdo.xdcruntime.linux.SemProcessSupport'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.xdcruntime.linux.SemProcessSupport'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void ThreadSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$ThreadSupport$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sdo.xdcruntime.linux.ThreadSupport.create() called before xdc.useModule('ti.sdo.xdcruntime.linux.ThreadSupport')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst.$args.fxn, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$create", new Proto.Fxn(om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Module", "ti.sdo.xdcruntime.linux"), om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Instance", "ti.sdo.xdcruntime.linux"), 2, 1, false));
                        fxn.addArg(0, "fxn", new Proto.Adr("xdc_Void(*)(xdc_IArg)", "PFv"), $$UNDEF);
                        fxn.addArg(1, "__params", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Params", "ti.sdo.xdcruntime.linux"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$ThreadSupport$$create = function( fxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport'];\n");
                sb.append("var __inst = xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sdo.xdcruntime.linux']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {fxn:fxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [fxn]);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$construct", new Proto.Fxn(om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Module", "ti.sdo.xdcruntime.linux"), null, 3, 1, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$Object", "ti.sdo.xdcruntime.linux"), null);
                        fxn.addArg(1, "fxn", new Proto.Adr("xdc_Void(*)(xdc_IArg)", "PFv"), $$UNDEF);
                        fxn.addArg(2, "__params", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Params", "ti.sdo.xdcruntime.linux"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$xdcruntime$linux$ThreadSupport$$construct = function( __obj, fxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {fxn:fxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void TimestampPosix$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Settings$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void GateThreadSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void GateProcessSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void SemThreadSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void SemProcessSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void ThreadSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TimestampPosix$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Settings$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void GateThreadSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("gate", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sdo.xdcruntime.linux.GateThreadSupport.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sdo.xdcruntime.linux.GateThreadSupport.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sdo.xdcruntime.linux.GateThreadSupport.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void GateProcessSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance_State", "ti.sdo.xdcruntime.linux");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("id", "TInt"));
        sizes.add(Global.newArray("count", "TInt"));
        sizes.add(Global.newArray("owner", "TInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sdo.xdcruntime.linux.GateProcessSupport.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sdo.xdcruntime.linux.GateProcessSupport.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sdo.xdcruntime.linux.GateProcessSupport.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void SemThreadSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("sem", "UPtr"));
        sizes.add(Global.newArray("mode", "Nxdc.runtime.knl.ISemaphore.Mode;;;"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Module_State", "ti.sdo.xdcruntime.linux");
        sizes.clear();
        sizes.add(Global.newArray("mutex", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sdo.xdcruntime.linux.SemThreadSupport.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void SemProcessSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance_State", "ti.sdo.xdcruntime.linux");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("id", "TInt"));
        sizes.add(Global.newArray("key", "TInt"));
        sizes.add(Global.newArray("count", "TInt"));
        sizes.add(Global.newArray("mode", "Nxdc.runtime.knl.ISemaphore.Mode;;;"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sdo.xdcruntime.linux.SemProcessSupport.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sdo.xdcruntime.linux.SemProcessSupport.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sdo.xdcruntime.linux.SemProcessSupport.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void ThreadSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("tls", "UPtr"));
        sizes.add(Global.newArray("startFxn", "UFxn"));
        sizes.add(Global.newArray("startFxnArg", "TIArg"));
        sizes.add(Global.newArray("stackSize", "USize"));
        sizes.add(Global.newArray("tid", "UPtr"));
        sizes.add(Global.newArray("stackBase", "UPtr"));
        sizes.add(Global.newArray("staticOsPri", "TInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Module_State", "ti.sdo.xdcruntime.linux");
        sizes.clear();
        sizes.add(Global.newArray("lowestPriority", "TInt"));
        sizes.add(Global.newArray("belowNormalPriority", "TInt"));
        sizes.add(Global.newArray("normalPriority", "TInt"));
        sizes.add(Global.newArray("aboveNormalPriority", "TInt"));
        sizes.add(Global.newArray("highestPriority", "TInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sdo.xdcruntime.linux.ThreadSupport.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void TimestampPosix$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void Settings$$SIZES()
    {
    }

    void GateThreadSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/xdcruntime/linux/GateThreadSupport.xs");
        om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Module", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.GateThreadSupport.Module", om.findStrict("xdc.runtime.knl.IGateThreadSupport.Module", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("SETUP_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 3L, "rh");
                po.addFld("ALLOC_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
            po.addFld("A_POSIX_Error", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "A_POSIX_Error: a POSIX function returned failure"), "w");
            po.addFld("E_POSIX_Error", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "E_POSIX_Error: a POSIX function returned failure, errno is %d"), "w");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport$$create", "ti.sdo.xdcruntime.linux"), Global.get("ti$sdo$xdcruntime$linux$GateThreadSupport$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport$$construct", "ti.sdo.xdcruntime.linux"), Global.get("ti$sdo$xdcruntime$linux$GateThreadSupport$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateThreadSupport$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
                fxn = Global.get(cap, "queryMeta");
                if (fxn != null) po.addFxn("queryMeta", (Proto.Fxn)om.findStrict("xdc.runtime.IGateProvider$$queryMeta", "ti.sdo.xdcruntime.linux"), fxn);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance", om.findStrict("xdc.runtime.knl.IGateThreadSupport.Instance", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("SETUP_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 3L, "rh");
                po.addFld("ALLOC_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport$$Params", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.GateThreadSupport.Params", om.findStrict("xdc.runtime.knl.IGateThreadSupport$$Params", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("SETUP_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 3L, "rh");
                po.addFld("ALLOC_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport$$Object", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.GateThreadSupport.Object", om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance", "ti.sdo.xdcruntime.linux"));
        // struct GateThreadSupport.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("gate", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
    }

    void GateProcessSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/xdcruntime/linux/GateProcessSupport.xs");
        om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Module", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.GateProcessSupport.Module", om.findStrict("xdc.runtime.knl.IGateProcessSupport.Module", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("GETREFCOUNT_FAILED", Proto.Elm.newCNum("(xdc_Int)"), Global.eval("-1"), "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
            po.addFld("A_invalidKey", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "A_invalidKey: params key has not been set"), "w");
            po.addFld("A_POSIX_Error", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "A_POSIX_Error: a POSIX function returned failure"), "w");
            po.addFld("E_POSIX_Error", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "E_POSIX_Error: a POSIX function returned failure, errno is %d"), "w");
            po.addFld("L_extant", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sdo.xdcruntime.linux"), Global.newObject("mask", 0x0004L, "msg", "--- GateProcess(%d) already exists, incrementing reference count"), "w");
            po.addFld("L_create", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sdo.xdcruntime.linux"), Global.newObject("mask", 0x0004L, "msg", "--- GateProcess(%d) DNE, creating"), "w");
            po.addFld("perms", Proto.Elm.newCNum("(xdc_Int)"), 0666L, "w");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport$$create", "ti.sdo.xdcruntime.linux"), Global.get("ti$sdo$xdcruntime$linux$GateProcessSupport$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport$$construct", "ti.sdo.xdcruntime.linux"), Global.get("ti$sdo$xdcruntime$linux$GateProcessSupport$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.GateProcessSupport$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
                fxn = Global.get(cap, "queryMeta");
                if (fxn != null) po.addFxn("queryMeta", (Proto.Fxn)om.findStrict("xdc.runtime.IGateProvider$$queryMeta", "ti.sdo.xdcruntime.linux"), fxn);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance", om.findStrict("xdc.runtime.knl.IGateProcessSupport.Instance", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("GETREFCOUNT_FAILED", Proto.Elm.newCNum("(xdc_Int)"), Global.eval("-1"), "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport$$Params", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.GateProcessSupport.Params", om.findStrict("xdc.runtime.knl.IGateProcessSupport$$Params", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("GETREFCOUNT_FAILED", Proto.Elm.newCNum("(xdc_Int)"), Global.eval("-1"), "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport$$Object", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.GateProcessSupport.Object", om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance", "ti.sdo.xdcruntime.linux"));
        // struct GateProcessSupport.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("id", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("count", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("owner", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
    }

    void SemThreadSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/xdcruntime/linux/SemThreadSupport.xs");
        om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Module", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemThreadSupport.Module", om.findStrict("xdc.runtime.knl.ISemThreadSupport.Module", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("FOREVER", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~(0)"), "rh");
                po.addFld("ALLOC_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
            po.addFld("A_POSIX_Error", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "A_POSIX_Error: a POSIX function returned failure"), "w");
            po.addFld("E_POSIX_Error", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "E_POSIX_Error: a POSIX function returned failure, errno is %d"), "w");
            po.addFld("E_resetError", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "E_resetError: %s returned failure"), "w");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$create", "ti.sdo.xdcruntime.linux"), Global.get("ti$sdo$xdcruntime$linux$SemThreadSupport$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$construct", "ti.sdo.xdcruntime.linux"), Global.get("ti$sdo$xdcruntime$linux$SemThreadSupport$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemThreadSupport$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance", om.findStrict("xdc.runtime.knl.ISemThreadSupport.Instance", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("FOREVER", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~(0)"), "rh");
                po.addFld("ALLOC_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$Params", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemThreadSupport.Params", om.findStrict("xdc.runtime.knl.ISemThreadSupport$$Params", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("FOREVER", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~(0)"), "rh");
                po.addFld("ALLOC_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$Object", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemThreadSupport.Object", om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance", "ti.sdo.xdcruntime.linux"));
        // struct SemThreadSupport.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("sem", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("mode", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Mode", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        // struct SemThreadSupport.Module_State
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$Module_State", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemThreadSupport.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("mutex", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
    }

    void SemProcessSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/xdcruntime/linux/SemProcessSupport.xs");
        om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Module", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemProcessSupport.Module", om.findStrict("xdc.runtime.knl.ISemProcessSupport.Module", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("FOREVER", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~(0)"), "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
            po.addFld("A_unsupportedTimeout", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "A_unsupportedTimeout: Only SemProcess_FOREVER is supported"), "w");
            po.addFld("A_invalidKey", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "A_invalidKey: params key has not been set"), "w");
            po.addFld("A_POSIX_Error", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "A_POSIX_Error: a POSIX function returned failure"), "w");
            po.addFld("E_POSIX_Error", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "E_POSIX_Error: a POSIX function returned failure, errno is %d"), "w");
            po.addFld("L_extant", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sdo.xdcruntime.linux"), Global.newObject("mask", 0x0004L, "msg", "--- SemProcess(%d) already exists, incrementing reference count"), "w");
            po.addFld("L_create", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sdo.xdcruntime.linux"), Global.newObject("mask", 0x0004L, "msg", "--- SemProcess(%d) DNE, creating"), "w");
            po.addFld("perms", Proto.Elm.newCNum("(xdc_Int)"), 0666L, "w");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport$$create", "ti.sdo.xdcruntime.linux"), Global.get("ti$sdo$xdcruntime$linux$SemProcessSupport$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport$$construct", "ti.sdo.xdcruntime.linux"), Global.get("ti$sdo$xdcruntime$linux$SemProcessSupport$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.SemProcessSupport$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance", om.findStrict("xdc.runtime.knl.ISemProcessSupport.Instance", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("FOREVER", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~(0)"), "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport$$Params", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemProcessSupport.Params", om.findStrict("xdc.runtime.knl.ISemProcessSupport$$Params", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("FOREVER", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~(0)"), "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport$$Object", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemProcessSupport.Object", om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance", "ti.sdo.xdcruntime.linux"));
        // struct SemProcessSupport.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("id", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("key", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("count", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("mode", (Proto)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Mode", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
    }

    void ThreadSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/xdcruntime/linux/ThreadSupport.xs");
        om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Module", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.ThreadSupport.Module", om.findStrict("xdc.runtime.knl.IThreadSupport.Module", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("INVALID_OS_PRIORITY", Proto.Elm.newCNum("(xdc_Int)"), 0L, "rh");
                po.addFld("GETPRI_FAILED", Proto.Elm.newCNum("(xdc_Int)"), Global.eval("-2"), "rh");
                po.addFld("PRI_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 4L, "rh");
                po.addFld("SETUP_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 3L, "rh");
                po.addFld("ALLOC_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
            po.addFld("E_priority", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "E_priority: Thread priority is invalid %d"), "w");
            po.addFld("A_POSIX_Error", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "A_POSIX_Error: a POSIX function returned failure"), "w");
            po.addFld("E_POSIX_Error", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sdo.xdcruntime.linux"), Global.newObject("msg", "E_POSIX_Error: a POSIX function returned failure, errno is %d"), "w");
            po.addFld("L_start", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sdo.xdcruntime.linux"), Global.newObject("mask", 0x0004L, "msg", "<-- start: (%p)"), "w");
            po.addFld("L_finish", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sdo.xdcruntime.linux"), Global.newObject("mask", 0x0004L, "msg", "--> finish: (%p)"), "w");
            po.addFld("L_join", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sdo.xdcruntime.linux"), Global.newObject("mask", 0x0004L, "msg", "--> join: (%p)"), "w");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$create", "ti.sdo.xdcruntime.linux"), Global.get("ti$sdo$xdcruntime$linux$ThreadSupport$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$construct", "ti.sdo.xdcruntime.linux"), Global.get("ti$sdo$xdcruntime$linux$ThreadSupport$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.ThreadSupport$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Instance", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.ThreadSupport.Instance", om.findStrict("xdc.runtime.knl.IThreadSupport.Instance", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("INVALID_OS_PRIORITY", Proto.Elm.newCNum("(xdc_Int)"), 0L, "rh");
                po.addFld("GETPRI_FAILED", Proto.Elm.newCNum("(xdc_Int)"), Global.eval("-2"), "rh");
                po.addFld("PRI_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 4L, "rh");
                po.addFld("SETUP_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 3L, "rh");
                po.addFld("ALLOC_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$Params", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.ThreadSupport.Params", om.findStrict("xdc.runtime.knl.IThreadSupport$$Params", "ti.sdo.xdcruntime.linux"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("INVALID_OS_PRIORITY", Proto.Elm.newCNum("(xdc_Int)"), 0L, "rh");
                po.addFld("GETPRI_FAILED", Proto.Elm.newCNum("(xdc_Int)"), Global.eval("-2"), "rh");
                po.addFld("PRI_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 4L, "rh");
                po.addFld("SETUP_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 3L, "rh");
                po.addFld("ALLOC_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
                po.addFld("POSIX_FAILURE", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.xdcruntime.linux"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$Object", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.ThreadSupport.Object", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Instance", "ti.sdo.xdcruntime.linux"));
        // struct ThreadSupport.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.ThreadSupport.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("tls", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("startFxn", new Proto.Adr("xdc_Void(*)(xdc_IArg)", "PFv"), $$UNDEF, "w");
                po.addFld("startFxnArg", new Proto.Adr("xdc_IArg", "Pv"), $$UNDEF, "w");
                po.addFld("stackSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("tid", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("stackBase", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("staticOsPri", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
        // struct ThreadSupport.Module_State
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$Module_State", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.ThreadSupport.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("lowestPriority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("belowNormalPriority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("normalPriority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("aboveNormalPriority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("highestPriority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
    }

    void TimestampPosix$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.TimestampPosix.Module", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.TimestampPosix.Module", om.findStrict("xdc.runtime.ITimestampProvider.Module", "ti.sdo.xdcruntime.linux"));
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

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/xdcruntime/linux/Settings.xs");
        om.bind("ti.sdo.xdcruntime.linux.Settings$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.Settings.Module", "ti.sdo.xdcruntime.linux");
        po.init("ti.sdo.xdcruntime.linux.Settings.Module", $$Module);
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.Settings$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.Settings$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sdo.xdcruntime.linux.Settings$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void GateThreadSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sdo.xdcruntime.linux.GateThreadSupport.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
    }

    void GateProcessSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sdo.xdcruntime.linux.GateProcessSupport.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
    }

    void SemThreadSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sdo.xdcruntime.linux.SemThreadSupport.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sdo.xdcruntime.linux.SemThreadSupport.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$Module_State", "ti.sdo.xdcruntime.linux");
    }

    void SemProcessSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sdo.xdcruntime.linux.SemProcessSupport.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
    }

    void ThreadSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sdo.xdcruntime.linux.ThreadSupport.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$Instance_State", "ti.sdo.xdcruntime.linux");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sdo.xdcruntime.linux.ThreadSupport.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$Module_State", "ti.sdo.xdcruntime.linux");
    }

    void TimestampPosix$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.TimestampPosix", "ti.sdo.xdcruntime.linux");
    }

    void Settings$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sdo.xdcruntime.linux.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sdo.xdcruntime.linux"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/xdcruntime/linux/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sdo.xdcruntime.linux"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sdo.xdcruntime.linux"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sdo.xdcruntime.linux"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sdo.xdcruntime.linux"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sdo.xdcruntime.linux"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sdo.xdcruntime.linux"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sdo.xdcruntime.linux", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sdo.xdcruntime.linux");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sdo.xdcruntime.linux.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("xdc.runtime.knl", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sdo.xdcruntime.linux'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/debug/osal_linux_86U.a86U',\n");
            sb.append("'lib/release/osal_linux_86U.a86U',\n");
            sb.append("'lib/debug/osal_linux_470.av5T',\n");
            sb.append("'lib/release/osal_linux_470.av5T',\n");
            sb.append("'lib/debug/osal_linux_470.aqv7',\n");
            sb.append("'lib/release/osal_linux_470.aqv7',\n");
            sb.append("'lib/debug/osal_linux_470.abnc',\n");
            sb.append("'lib/release/osal_linux_470.abnc',\n");
            sb.append("'lib/debug/osal_linux_470.a470uC',\n");
            sb.append("'lib/release/osal_linux_470.a470uC',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/debug/osal_linux_86U.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/release/osal_linux_86U.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/debug/osal_linux_470.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/release/osal_linux_470.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/debug/osal_linux_470.aqv7', {target: 'qnx.targets.arm.Qnx', suffix: 'qv7'}],\n");
            sb.append("['lib/release/osal_linux_470.aqv7', {target: 'qnx.targets.arm.Qnx', suffix: 'qv7'}],\n");
            sb.append("['lib/debug/osal_linux_470.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/release/osal_linux_470.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/debug/osal_linux_470.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/release/osal_linux_470.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void GateThreadSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Module", "ti.sdo.xdcruntime.linux");
        vo.init2(po, "ti.sdo.xdcruntime.linux.GateThreadSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport$$capsule", "ti.sdo.xdcruntime.linux"));
        vo.bind("Instance", om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance", "ti.sdo.xdcruntime.linux"));
        vo.bind("Params", om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Params", "ti.sdo.xdcruntime.linux"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Params", "ti.sdo.xdcruntime.linux")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Handle", "ti.sdo.xdcruntime.linux"));
        vo.bind("$package", om.findStrict("ti.sdo.xdcruntime.linux", "ti.sdo.xdcruntime.linux"));
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
        mcfgs.add("A_POSIX_Error");
        mcfgs.add("E_POSIX_Error");
        vo.bind("Instance_State", om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        tdefs.add(om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime.knl");
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.xdcruntime.linux")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("ti.sdo.xdcruntime.linux.GateThreadSupport$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sdo$xdcruntime$linux$GateThreadSupport$$__initObject"));
        }//isCFG
        vo.bind("query", om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.query", "ti.sdo.xdcruntime.linux"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_xdcruntime_linux_GateThreadSupport_Handle__label__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_Module__startupDone__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_Object__create__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_Object__delete__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_Object__destruct__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_Object__get__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_Object__first__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_Object__next__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_Params__init__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_query__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_enter__E", "ti_sdo_xdcruntime_linux_GateThreadSupport_leave__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray("E_POSIX_Error"));
        vo.bind("$$assertDescCfgs", Global.newArray("A_POSIX_Error"));
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport.Object", "ti.sdo.xdcruntime.linux"));
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "query", "entry", "%d", "exit", "%d"));
        loggables.add(Global.newObject("name", "enter", "entry", "%p", "exit", "%p"));
        loggables.add(Global.newObject("name", "leave", "entry", "%p, %p", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("GateThreadSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("GateThreadSupport");
    }

    void GateProcessSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Module", "ti.sdo.xdcruntime.linux");
        vo.init2(po, "ti.sdo.xdcruntime.linux.GateProcessSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport$$capsule", "ti.sdo.xdcruntime.linux"));
        vo.bind("Instance", om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance", "ti.sdo.xdcruntime.linux"));
        vo.bind("Params", om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Params", "ti.sdo.xdcruntime.linux"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Params", "ti.sdo.xdcruntime.linux")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Handle", "ti.sdo.xdcruntime.linux"));
        vo.bind("$package", om.findStrict("ti.sdo.xdcruntime.linux", "ti.sdo.xdcruntime.linux"));
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
        mcfgs.add("A_invalidKey");
        mcfgs.add("A_POSIX_Error");
        mcfgs.add("E_POSIX_Error");
        mcfgs.add("L_extant");
        mcfgs.add("L_create");
        mcfgs.add("perms");
        vo.bind("Instance_State", om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        tdefs.add(om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime.knl");
        inherits.add("xdc.runtime.knl");
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.xdcruntime.linux")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("ti.sdo.xdcruntime.linux.GateProcessSupport$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sdo$xdcruntime$linux$GateProcessSupport$$__initObject"));
        }//isCFG
        vo.bind("query", om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.query", "ti.sdo.xdcruntime.linux"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_xdcruntime_linux_GateProcessSupport_Handle__label__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_Module__startupDone__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_Object__create__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_Object__delete__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_Object__destruct__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_Object__get__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_Object__first__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_Object__next__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_Params__init__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_query__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_enter__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_leave__E", "ti_sdo_xdcruntime_linux_GateProcessSupport_getReferenceCount__E"));
        vo.bind("$$logEvtCfgs", Global.newArray("L_extant", "L_create"));
        vo.bind("$$errorDescCfgs", Global.newArray("E_POSIX_Error"));
        vo.bind("$$assertDescCfgs", Global.newArray("A_invalidKey", "A_POSIX_Error"));
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport.Object", "ti.sdo.xdcruntime.linux"));
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "query", "entry", "%d", "exit", "%d"));
        loggables.add(Global.newObject("name", "enter", "entry", "%p", "exit", "%p"));
        loggables.add(Global.newObject("name", "leave", "entry", "%p, %p", "exit", ""));
        loggables.add(Global.newObject("name", "getReferenceCount", "entry", "%p, %p", "exit", "%d"));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("GateProcessSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("GateProcessSupport");
    }

    void SemThreadSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Module", "ti.sdo.xdcruntime.linux");
        vo.init2(po, "ti.sdo.xdcruntime.linux.SemThreadSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport$$capsule", "ti.sdo.xdcruntime.linux"));
        vo.bind("Instance", om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance", "ti.sdo.xdcruntime.linux"));
        vo.bind("Params", om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Params", "ti.sdo.xdcruntime.linux"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Params", "ti.sdo.xdcruntime.linux")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Handle", "ti.sdo.xdcruntime.linux"));
        vo.bind("$package", om.findStrict("ti.sdo.xdcruntime.linux", "ti.sdo.xdcruntime.linux"));
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
        vo.bind("PendStatus", om.findStrict("xdc.runtime.knl.ISemaphore.PendStatus", "ti.sdo.xdcruntime.linux"));
        vo.bind("Mode", om.findStrict("xdc.runtime.knl.ISemaphore.Mode", "ti.sdo.xdcruntime.linux"));
        mcfgs.add("A_POSIX_Error");
        mcfgs.add("E_POSIX_Error");
        mcfgs.add("E_resetError");
        vo.bind("Instance_State", om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        tdefs.add(om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        vo.bind("Module_State", om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Module_State", "ti.sdo.xdcruntime.linux"));
        tdefs.add(om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Module_State", "ti.sdo.xdcruntime.linux"));
        vo.bind("PendStatus_ERROR", om.findStrict("xdc.runtime.knl.ISemaphore.PendStatus_ERROR", "ti.sdo.xdcruntime.linux"));
        vo.bind("PendStatus_TIMEOUT", om.findStrict("xdc.runtime.knl.ISemaphore.PendStatus_TIMEOUT", "ti.sdo.xdcruntime.linux"));
        vo.bind("PendStatus_SUCCESS", om.findStrict("xdc.runtime.knl.ISemaphore.PendStatus_SUCCESS", "ti.sdo.xdcruntime.linux"));
        vo.bind("Mode_COUNTING", om.findStrict("xdc.runtime.knl.ISemaphore.Mode_COUNTING", "ti.sdo.xdcruntime.linux"));
        vo.bind("Mode_BINARY", om.findStrict("xdc.runtime.knl.ISemaphore.Mode_BINARY", "ti.sdo.xdcruntime.linux"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime.knl");
        inherits.add("xdc.runtime.knl");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.xdcruntime.linux")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("ti.sdo.xdcruntime.linux.SemThreadSupport$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sdo$xdcruntime$linux$SemThreadSupport$$__initObject"));
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_sdo_xdcruntime_linux_SemThreadSupport_Handle__label__E", "ti_sdo_xdcruntime_linux_SemThreadSupport_Module__startupDone__E", "ti_sdo_xdcruntime_linux_SemThreadSupport_Object__create__E", "ti_sdo_xdcruntime_linux_SemThreadSupport_Object__delete__E", "ti_sdo_xdcruntime_linux_SemThreadSupport_Object__destruct__E", "ti_sdo_xdcruntime_linux_SemThreadSupport_Object__get__E", "ti_sdo_xdcruntime_linux_SemThreadSupport_Object__first__E", "ti_sdo_xdcruntime_linux_SemThreadSupport_Object__next__E", "ti_sdo_xdcruntime_linux_SemThreadSupport_Params__init__E", "ti_sdo_xdcruntime_linux_SemThreadSupport_pend__E", "ti_sdo_xdcruntime_linux_SemThreadSupport_post__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray("E_POSIX_Error", "E_resetError"));
        vo.bind("$$assertDescCfgs", Global.newArray("A_POSIX_Error"));
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport.Object", "ti.sdo.xdcruntime.linux"));
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "pend", "entry", "%p, 0x%x, %p", "exit", "%d"));
        loggables.add(Global.newObject("name", "post", "entry", "%p, %p", "exit", "%d"));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("SemThreadSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("SemThreadSupport");
    }

    void SemProcessSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Module", "ti.sdo.xdcruntime.linux");
        vo.init2(po, "ti.sdo.xdcruntime.linux.SemProcessSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport$$capsule", "ti.sdo.xdcruntime.linux"));
        vo.bind("Instance", om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance", "ti.sdo.xdcruntime.linux"));
        vo.bind("Params", om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Params", "ti.sdo.xdcruntime.linux"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Params", "ti.sdo.xdcruntime.linux")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Handle", "ti.sdo.xdcruntime.linux"));
        vo.bind("$package", om.findStrict("ti.sdo.xdcruntime.linux", "ti.sdo.xdcruntime.linux"));
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
        vo.bind("PendStatus", om.findStrict("xdc.runtime.knl.ISemaphore.PendStatus", "ti.sdo.xdcruntime.linux"));
        vo.bind("Mode", om.findStrict("xdc.runtime.knl.ISemaphore.Mode", "ti.sdo.xdcruntime.linux"));
        mcfgs.add("A_unsupportedTimeout");
        mcfgs.add("A_invalidKey");
        mcfgs.add("A_POSIX_Error");
        mcfgs.add("E_POSIX_Error");
        mcfgs.add("L_extant");
        mcfgs.add("L_create");
        mcfgs.add("perms");
        vo.bind("Instance_State", om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        tdefs.add(om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        vo.bind("PendStatus_ERROR", om.findStrict("xdc.runtime.knl.ISemaphore.PendStatus_ERROR", "ti.sdo.xdcruntime.linux"));
        vo.bind("PendStatus_TIMEOUT", om.findStrict("xdc.runtime.knl.ISemaphore.PendStatus_TIMEOUT", "ti.sdo.xdcruntime.linux"));
        vo.bind("PendStatus_SUCCESS", om.findStrict("xdc.runtime.knl.ISemaphore.PendStatus_SUCCESS", "ti.sdo.xdcruntime.linux"));
        vo.bind("Mode_COUNTING", om.findStrict("xdc.runtime.knl.ISemaphore.Mode_COUNTING", "ti.sdo.xdcruntime.linux"));
        vo.bind("Mode_BINARY", om.findStrict("xdc.runtime.knl.ISemaphore.Mode_BINARY", "ti.sdo.xdcruntime.linux"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime.knl");
        inherits.add("xdc.runtime.knl");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.xdcruntime.linux")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("ti.sdo.xdcruntime.linux.SemProcessSupport$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sdo$xdcruntime$linux$SemProcessSupport$$__initObject"));
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_sdo_xdcruntime_linux_SemProcessSupport_Handle__label__E", "ti_sdo_xdcruntime_linux_SemProcessSupport_Module__startupDone__E", "ti_sdo_xdcruntime_linux_SemProcessSupport_Object__create__E", "ti_sdo_xdcruntime_linux_SemProcessSupport_Object__delete__E", "ti_sdo_xdcruntime_linux_SemProcessSupport_Object__destruct__E", "ti_sdo_xdcruntime_linux_SemProcessSupport_Object__get__E", "ti_sdo_xdcruntime_linux_SemProcessSupport_Object__first__E", "ti_sdo_xdcruntime_linux_SemProcessSupport_Object__next__E", "ti_sdo_xdcruntime_linux_SemProcessSupport_Params__init__E", "ti_sdo_xdcruntime_linux_SemProcessSupport_pend__E", "ti_sdo_xdcruntime_linux_SemProcessSupport_post__E"));
        vo.bind("$$logEvtCfgs", Global.newArray("L_extant", "L_create"));
        vo.bind("$$errorDescCfgs", Global.newArray("E_POSIX_Error"));
        vo.bind("$$assertDescCfgs", Global.newArray("A_unsupportedTimeout", "A_invalidKey", "A_POSIX_Error"));
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport.Object", "ti.sdo.xdcruntime.linux"));
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "pend", "entry", "%p, 0x%x, %p", "exit", "%d"));
        loggables.add(Global.newObject("name", "post", "entry", "%p, %p", "exit", "%d"));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("SemProcessSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("SemProcessSupport");
    }

    void ThreadSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Module", "ti.sdo.xdcruntime.linux");
        vo.init2(po, "ti.sdo.xdcruntime.linux.ThreadSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport$$capsule", "ti.sdo.xdcruntime.linux"));
        vo.bind("Instance", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Instance", "ti.sdo.xdcruntime.linux"));
        vo.bind("Params", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Params", "ti.sdo.xdcruntime.linux"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Params", "ti.sdo.xdcruntime.linux")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Handle", "ti.sdo.xdcruntime.linux"));
        vo.bind("$package", om.findStrict("ti.sdo.xdcruntime.linux", "ti.sdo.xdcruntime.linux"));
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
        vo.bind("Priority", om.findStrict("xdc.runtime.knl.IThreadSupport.Priority", "ti.sdo.xdcruntime.linux"));
        vo.bind("CompStatus", om.findStrict("xdc.runtime.knl.IThreadSupport.CompStatus", "ti.sdo.xdcruntime.linux"));
        vo.bind("RunFxn", om.findStrict("xdc.runtime.knl.IThreadSupport.RunFxn", "ti.sdo.xdcruntime.linux"));
        vo.bind("Stat", om.findStrict("xdc.runtime.knl.IThreadSupport.Stat", "ti.sdo.xdcruntime.linux"));
        tdefs.add(om.findStrict("xdc.runtime.knl.IThreadSupport.Stat", "ti.sdo.xdcruntime.linux"));
        mcfgs.add("E_priority");
        mcfgs.add("A_POSIX_Error");
        mcfgs.add("E_POSIX_Error");
        mcfgs.add("L_start");
        mcfgs.add("L_finish");
        mcfgs.add("L_join");
        vo.bind("Instance_State", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        tdefs.add(om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Instance_State", "ti.sdo.xdcruntime.linux"));
        vo.bind("Module_State", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Module_State", "ti.sdo.xdcruntime.linux"));
        tdefs.add(om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Module_State", "ti.sdo.xdcruntime.linux"));
        vo.bind("Priority_INVALID", om.findStrict("xdc.runtime.knl.IThreadSupport.Priority_INVALID", "ti.sdo.xdcruntime.linux"));
        vo.bind("Priority_LOWEST", om.findStrict("xdc.runtime.knl.IThreadSupport.Priority_LOWEST", "ti.sdo.xdcruntime.linux"));
        vo.bind("Priority_BELOW_NORMAL", om.findStrict("xdc.runtime.knl.IThreadSupport.Priority_BELOW_NORMAL", "ti.sdo.xdcruntime.linux"));
        vo.bind("Priority_NORMAL", om.findStrict("xdc.runtime.knl.IThreadSupport.Priority_NORMAL", "ti.sdo.xdcruntime.linux"));
        vo.bind("Priority_ABOVE_NORMAL", om.findStrict("xdc.runtime.knl.IThreadSupport.Priority_ABOVE_NORMAL", "ti.sdo.xdcruntime.linux"));
        vo.bind("Priority_HIGHEST", om.findStrict("xdc.runtime.knl.IThreadSupport.Priority_HIGHEST", "ti.sdo.xdcruntime.linux"));
        vo.bind("CompStatus_ERROR", om.findStrict("xdc.runtime.knl.IThreadSupport.CompStatus_ERROR", "ti.sdo.xdcruntime.linux"));
        vo.bind("CompStatus_LOWER", om.findStrict("xdc.runtime.knl.IThreadSupport.CompStatus_LOWER", "ti.sdo.xdcruntime.linux"));
        vo.bind("CompStatus_EQUAL", om.findStrict("xdc.runtime.knl.IThreadSupport.CompStatus_EQUAL", "ti.sdo.xdcruntime.linux"));
        vo.bind("CompStatus_HIGHER", om.findStrict("xdc.runtime.knl.IThreadSupport.CompStatus_HIGHER", "ti.sdo.xdcruntime.linux"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime.knl");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.xdcruntime.linux")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("ti.sdo.xdcruntime.linux.ThreadSupport$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sdo$xdcruntime$linux$ThreadSupport$$__initObject"));
        }//isCFG
        vo.bind("self", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.self", "ti.sdo.xdcruntime.linux"));
        vo.bind("start", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.start", "ti.sdo.xdcruntime.linux"));
        vo.bind("yield", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.yield", "ti.sdo.xdcruntime.linux"));
        vo.bind("compareOsPriorities", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.compareOsPriorities", "ti.sdo.xdcruntime.linux"));
        vo.bind("sleep", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.sleep", "ti.sdo.xdcruntime.linux"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_xdcruntime_linux_ThreadSupport_Handle__label__E", "ti_sdo_xdcruntime_linux_ThreadSupport_Module__startupDone__E", "ti_sdo_xdcruntime_linux_ThreadSupport_Object__create__E", "ti_sdo_xdcruntime_linux_ThreadSupport_Object__delete__E", "ti_sdo_xdcruntime_linux_ThreadSupport_Object__destruct__E", "ti_sdo_xdcruntime_linux_ThreadSupport_Object__get__E", "ti_sdo_xdcruntime_linux_ThreadSupport_Object__first__E", "ti_sdo_xdcruntime_linux_ThreadSupport_Object__next__E", "ti_sdo_xdcruntime_linux_ThreadSupport_Params__init__E", "ti_sdo_xdcruntime_linux_ThreadSupport_self__E", "ti_sdo_xdcruntime_linux_ThreadSupport_start__E", "ti_sdo_xdcruntime_linux_ThreadSupport_yield__E", "ti_sdo_xdcruntime_linux_ThreadSupport_compareOsPriorities__E", "ti_sdo_xdcruntime_linux_ThreadSupport_sleep__E", "ti_sdo_xdcruntime_linux_ThreadSupport_join__E", "ti_sdo_xdcruntime_linux_ThreadSupport_getPriority__E", "ti_sdo_xdcruntime_linux_ThreadSupport_setPriority__E", "ti_sdo_xdcruntime_linux_ThreadSupport_getOsPriority__E", "ti_sdo_xdcruntime_linux_ThreadSupport_setOsPriority__E", "ti_sdo_xdcruntime_linux_ThreadSupport_getOsHandle__E", "ti_sdo_xdcruntime_linux_ThreadSupport_getTls__E", "ti_sdo_xdcruntime_linux_ThreadSupport_setTls__E", "ti_sdo_xdcruntime_linux_ThreadSupport_stat__E"));
        vo.bind("$$logEvtCfgs", Global.newArray("L_start", "L_finish", "L_join"));
        vo.bind("$$errorDescCfgs", Global.newArray("E_priority", "E_POSIX_Error"));
        vo.bind("$$assertDescCfgs", Global.newArray("A_POSIX_Error"));
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport.Object", "ti.sdo.xdcruntime.linux"));
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "self", "entry", "%p", "exit", "%p"));
        loggables.add(Global.newObject("name", "start", "entry", "%p", "exit", "%d"));
        loggables.add(Global.newObject("name", "yield", "entry", "%p", "exit", "%d"));
        loggables.add(Global.newObject("name", "compareOsPriorities", "entry", "%d, %d, %p", "exit", "%d"));
        loggables.add(Global.newObject("name", "sleep", "entry", "0x%x, %p", "exit", "%d"));
        loggables.add(Global.newObject("name", "join", "entry", "%p, %p", "exit", "%d"));
        loggables.add(Global.newObject("name", "getPriority", "entry", "%p, %p", "exit", "%d"));
        loggables.add(Global.newObject("name", "setPriority", "entry", "%p, %d, %p", "exit", "%d"));
        loggables.add(Global.newObject("name", "getOsPriority", "entry", "%p, %p", "exit", "%d"));
        loggables.add(Global.newObject("name", "setOsPriority", "entry", "%p, %d, %p", "exit", "%d"));
        loggables.add(Global.newObject("name", "getOsHandle", "entry", "%p", "exit", "%p"));
        loggables.add(Global.newObject("name", "getTls", "entry", "%p", "exit", "%p"));
        loggables.add(Global.newObject("name", "setTls", "entry", "%p, %p", "exit", ""));
        loggables.add(Global.newObject("name", "stat", "entry", "%p, %p, %p", "exit", "%d"));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("ThreadSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ThreadSupport");
    }

    void TimestampPosix$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.TimestampPosix", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.TimestampPosix.Module", "ti.sdo.xdcruntime.linux");
        vo.init2(po, "ti.sdo.xdcruntime.linux.TimestampPosix", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("$package", om.findStrict("ti.sdo.xdcruntime.linux", "ti.sdo.xdcruntime.linux"));
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
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.xdcruntime.linux")).add(vo);
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
        vo.bind("get32", om.findStrict("ti.sdo.xdcruntime.linux.TimestampPosix.get32", "ti.sdo.xdcruntime.linux"));
        vo.bind("get64", om.findStrict("ti.sdo.xdcruntime.linux.TimestampPosix.get64", "ti.sdo.xdcruntime.linux"));
        vo.bind("getFreq", om.findStrict("ti.sdo.xdcruntime.linux.TimestampPosix.getFreq", "ti.sdo.xdcruntime.linux"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_xdcruntime_linux_TimestampPosix_Module__startupDone__E", "ti_sdo_xdcruntime_linux_TimestampPosix_get32__E", "ti_sdo_xdcruntime_linux_TimestampPosix_get64__E", "ti_sdo_xdcruntime_linux_TimestampPosix_getFreq__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
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
        pkgV.bind("TimestampPosix", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TimestampPosix");
    }

    void Settings$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.xdcruntime.linux.Settings", "ti.sdo.xdcruntime.linux");
        po = (Proto.Obj)om.findStrict("ti.sdo.xdcruntime.linux.Settings.Module", "ti.sdo.xdcruntime.linux");
        vo.init2(po, "ti.sdo.xdcruntime.linux.Settings", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sdo.xdcruntime.linux.Settings$$capsule", "ti.sdo.xdcruntime.linux"));
        vo.bind("$package", om.findStrict("ti.sdo.xdcruntime.linux", "ti.sdo.xdcruntime.linux"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.xdcruntime.linux")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.sdo.xdcruntime.linux.Settings$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        pkgV.bind("Settings", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Settings");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
            Object srcP = ((XScriptO)om.findStrict("xdc.runtime.IInstance", "ti.sdo.xdcruntime.linux")).findStrict("PARAMS", "ti.sdo.xdcruntime.linux");
            Scriptable dstP;

            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport", "ti.sdo.xdcruntime.linux")).findStrict("PARAMS", "ti.sdo.xdcruntime.linux");
            Global.put(dstP, "instance", srcP);
            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport", "ti.sdo.xdcruntime.linux")).findStrict("PARAMS", "ti.sdo.xdcruntime.linux");
            Global.put(dstP, "instance", srcP);
            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport", "ti.sdo.xdcruntime.linux")).findStrict("PARAMS", "ti.sdo.xdcruntime.linux");
            Global.put(dstP, "instance", srcP);
            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport", "ti.sdo.xdcruntime.linux")).findStrict("PARAMS", "ti.sdo.xdcruntime.linux");
            Global.put(dstP, "instance", srcP);
            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport", "ti.sdo.xdcruntime.linux")).findStrict("PARAMS", "ti.sdo.xdcruntime.linux");
            Global.put(dstP, "instance", srcP);
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.xdcruntime.linux.GateThreadSupport", "ti.sdo.xdcruntime.linux"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.xdcruntime.linux.GateProcessSupport", "ti.sdo.xdcruntime.linux"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.xdcruntime.linux.SemThreadSupport", "ti.sdo.xdcruntime.linux"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.xdcruntime.linux.SemProcessSupport", "ti.sdo.xdcruntime.linux"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.xdcruntime.linux.ThreadSupport", "ti.sdo.xdcruntime.linux"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.xdcruntime.linux.TimestampPosix", "ti.sdo.xdcruntime.linux"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.xdcruntime.linux.Settings", "ti.sdo.xdcruntime.linux"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sdo.xdcruntime.linux.GateThreadSupport")).bless();
        ((Value.Obj)om.getv("ti.sdo.xdcruntime.linux.GateProcessSupport")).bless();
        ((Value.Obj)om.getv("ti.sdo.xdcruntime.linux.SemThreadSupport")).bless();
        ((Value.Obj)om.getv("ti.sdo.xdcruntime.linux.SemProcessSupport")).bless();
        ((Value.Obj)om.getv("ti.sdo.xdcruntime.linux.ThreadSupport")).bless();
        ((Value.Obj)om.getv("ti.sdo.xdcruntime.linux.TimestampPosix")).bless();
        ((Value.Obj)om.getv("ti.sdo.xdcruntime.linux.Settings")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sdo.xdcruntime.linux")).add(pkgV);
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
        GateThreadSupport$$OBJECTS();
        GateProcessSupport$$OBJECTS();
        SemThreadSupport$$OBJECTS();
        SemProcessSupport$$OBJECTS();
        ThreadSupport$$OBJECTS();
        TimestampPosix$$OBJECTS();
        Settings$$OBJECTS();
        GateThreadSupport$$CONSTS();
        GateProcessSupport$$CONSTS();
        SemThreadSupport$$CONSTS();
        SemProcessSupport$$CONSTS();
        ThreadSupport$$CONSTS();
        TimestampPosix$$CONSTS();
        Settings$$CONSTS();
        GateThreadSupport$$CREATES();
        GateProcessSupport$$CREATES();
        SemThreadSupport$$CREATES();
        SemProcessSupport$$CREATES();
        ThreadSupport$$CREATES();
        TimestampPosix$$CREATES();
        Settings$$CREATES();
        GateThreadSupport$$FUNCTIONS();
        GateProcessSupport$$FUNCTIONS();
        SemThreadSupport$$FUNCTIONS();
        SemProcessSupport$$FUNCTIONS();
        ThreadSupport$$FUNCTIONS();
        TimestampPosix$$FUNCTIONS();
        Settings$$FUNCTIONS();
        GateThreadSupport$$SIZES();
        GateProcessSupport$$SIZES();
        SemThreadSupport$$SIZES();
        SemProcessSupport$$SIZES();
        ThreadSupport$$SIZES();
        TimestampPosix$$SIZES();
        Settings$$SIZES();
        GateThreadSupport$$TYPES();
        GateProcessSupport$$TYPES();
        SemThreadSupport$$TYPES();
        SemProcessSupport$$TYPES();
        ThreadSupport$$TYPES();
        TimestampPosix$$TYPES();
        Settings$$TYPES();
        if (isROV) {
            GateThreadSupport$$ROV();
            GateProcessSupport$$ROV();
            SemThreadSupport$$ROV();
            SemProcessSupport$$ROV();
            ThreadSupport$$ROV();
            TimestampPosix$$ROV();
            Settings$$ROV();
        }//isROV
        $$SINGLETONS();
        GateThreadSupport$$SINGLETONS();
        GateProcessSupport$$SINGLETONS();
        SemThreadSupport$$SINGLETONS();
        SemProcessSupport$$SINGLETONS();
        ThreadSupport$$SINGLETONS();
        TimestampPosix$$SINGLETONS();
        Settings$$SINGLETONS();
        $$INITIALIZATION();
    }
}
