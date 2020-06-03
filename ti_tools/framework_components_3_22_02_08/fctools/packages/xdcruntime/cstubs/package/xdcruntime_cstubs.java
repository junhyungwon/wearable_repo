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

public class xdcruntime_cstubs
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
        pkgP = (Proto.Obj)om.bind("xdcruntime.cstubs.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("xdcruntime.cstubs", new Value.Obj("xdcruntime.cstubs", pkgP));
    }

    void LoggerFlex$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("xdcruntime.cstubs.LoggerFlex.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("xdcruntime.cstubs.LoggerFlex", new Value.Obj("xdcruntime.cstubs.LoggerFlex", po));
        pkgV.bind("LoggerFlex", vo);
        // decls 
        spo = (Proto.Obj)om.bind("xdcruntime.cstubs.LoggerFlex$$Instance_State", new Proto.Obj());
        om.bind("xdcruntime.cstubs.LoggerFlex.Instance_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("xdcruntime.cstubs.LoggerFlex.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("xdcruntime.cstubs.LoggerFlex$$Object", new Proto.Obj());
        Object objP = om.bind("xdcruntime.cstubs.LoggerFlex.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("xdcruntime.cstubs.LoggerFlex$$Params", new Proto.Obj());
        om.bind("xdcruntime.cstubs.LoggerFlex.Params", new Proto.Str(po, false));
        om.bind("xdcruntime.cstubs.LoggerFlex.Handle", insP);
        if (isROV) {
            om.bind("xdcruntime.cstubs.LoggerFlex.Object", om.findStrict("xdcruntime.cstubs.LoggerFlex.Instance_State", "xdcruntime.cstubs"));
        }//isROV
    }

    void LoggerFlex$$CONSTS()
    {
        // module LoggerFlex
        om.bind("xdcruntime.cstubs.LoggerFlex.shutdown", new Extern("xdcruntime_cstubs_LoggerFlex_shutdown__E", "xdc_Void(*)(xdc_Int)", true, false));
    }

    void LoggerFlex$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("xdcruntime$cstubs$LoggerFlex$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function xdcruntime.cstubs.LoggerFlex.create() called before xdc.useModule('xdcruntime.cstubs.LoggerFlex')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("xdcruntime.cstubs.LoggerFlex$$create", new Proto.Fxn(om.findStrict("xdcruntime.cstubs.LoggerFlex.Module", "xdcruntime.cstubs"), om.findStrict("xdcruntime.cstubs.LoggerFlex.Instance", "xdcruntime.cstubs"), 1, 0, false));
                        fxn.addArg(0, "__params", (Proto)om.findStrict("xdcruntime.cstubs.LoggerFlex.Params", "xdcruntime.cstubs"), Global.newObject());
            sb = new StringBuilder();
            sb.append("xdcruntime$cstubs$LoggerFlex$$create = function( __params ) {\n");
                sb.append("var __mod = xdc.om['xdcruntime.cstubs.LoggerFlex'];\n");
                sb.append("var __inst = xdc.om['xdcruntime.cstubs.LoggerFlex.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['xdcruntime.cstubs']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['xdcruntime.cstubs.LoggerFlex'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['xdcruntime.cstubs.LoggerFlex'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("xdcruntime.cstubs.LoggerFlex$$construct", new Proto.Fxn(om.findStrict("xdcruntime.cstubs.LoggerFlex.Module", "xdcruntime.cstubs"), null, 2, 0, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("xdcruntime.cstubs.LoggerFlex$$Object", "xdcruntime.cstubs"), null);
                        fxn.addArg(1, "__params", (Proto)om.findStrict("xdcruntime.cstubs.LoggerFlex.Params", "xdcruntime.cstubs"), Global.newObject());
            sb = new StringBuilder();
            sb.append("xdcruntime$cstubs$LoggerFlex$$construct = function( __obj, __params ) {\n");
                sb.append("var __mod = xdc.om['xdcruntime.cstubs.LoggerFlex'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['xdcruntime.cstubs.LoggerFlex'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['xdcruntime.cstubs.LoggerFlex'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void LoggerFlex$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LoggerFlex$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("xdcruntime.cstubs.LoggerFlex.Instance_State", "xdcruntime.cstubs");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['xdcruntime.cstubs.LoggerFlex.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['xdcruntime.cstubs.LoggerFlex.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['xdcruntime.cstubs.LoggerFlex.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void LoggerFlex$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "xdcruntime/cstubs/LoggerFlex.xs");
        om.bind("xdcruntime.cstubs.LoggerFlex$$capsule", cap);
        po = (Proto.Obj)om.findStrict("xdcruntime.cstubs.LoggerFlex.Module", "xdcruntime.cstubs");
        po.init("xdcruntime.cstubs.LoggerFlex.Module", om.findStrict("xdc.runtime.ILogger.Module", "xdcruntime.cstubs"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("E_atexit", (Proto)om.findStrict("xdc.runtime.Error$$Id", "xdcruntime.cstubs"), Global.newObject("msg", "E_atexit: Could not plug in atexit function for shutdown"), "w");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("xdcruntime.cstubs.LoggerFlex$$create", "xdcruntime.cstubs"), Global.get("xdcruntime$cstubs$LoggerFlex$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("xdcruntime.cstubs.LoggerFlex$$construct", "xdcruntime.cstubs"), Global.get("xdcruntime$cstubs$LoggerFlex$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("xdcruntime.cstubs.LoggerFlex$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("xdcruntime.cstubs.LoggerFlex$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("xdcruntime.cstubs.LoggerFlex$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("xdcruntime.cstubs.LoggerFlex$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("xdcruntime.cstubs.LoggerFlex$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("xdcruntime.cstubs.LoggerFlex$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
                fxn = Global.get(cap, "getMetaArgs");
                if (fxn != null) po.addFxn("getMetaArgs", (Proto.Fxn)om.findStrict("xdc.runtime.ILogger$$getMetaArgs", "xdcruntime.cstubs"), fxn);
        po = (Proto.Obj)om.findStrict("xdcruntime.cstubs.LoggerFlex.Instance", "xdcruntime.cstubs");
        po.init("xdcruntime.cstubs.LoggerFlex.Instance", om.findStrict("xdc.runtime.ILogger.Instance", "xdcruntime.cstubs"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "xdcruntime.cstubs"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("xdcruntime.cstubs.LoggerFlex$$Params", "xdcruntime.cstubs");
        po.init("xdcruntime.cstubs.LoggerFlex.Params", om.findStrict("xdc.runtime.ILogger$$Params", "xdcruntime.cstubs"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "xdcruntime.cstubs"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("xdcruntime.cstubs.LoggerFlex$$Object", "xdcruntime.cstubs");
        po.init("xdcruntime.cstubs.LoggerFlex.Object", om.findStrict("xdcruntime.cstubs.LoggerFlex.Instance", "xdcruntime.cstubs"));
        // struct LoggerFlex.Instance_State
        po = (Proto.Obj)om.findStrict("xdcruntime.cstubs.LoggerFlex$$Instance_State", "xdcruntime.cstubs");
        po.init("xdcruntime.cstubs.LoggerFlex.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
    }

    void LoggerFlex$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("xdcruntime.cstubs.LoggerFlex", "xdcruntime.cstubs");
        po = (Proto.Obj)om.findStrict("xdcruntime.cstubs.LoggerFlex$$Instance_State", "xdcruntime.cstubs");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "xdcruntime.cstubs.LoggerFlex.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("xdcruntime.cstubs.LoggerFlex$$Instance_State", "xdcruntime.cstubs");
    }

    void $$SINGLETONS()
    {
        pkgP.init("xdcruntime.cstubs.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "xdcruntime.cstubs"));
        pkgP.bind("$capsule", $$UNDEF);
        pkgV.init2(pkgP, "xdcruntime.cstubs", Value.DEFAULT, false);
        pkgV.bind("$name", "xdcruntime.cstubs");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "xdcruntime.cstubs.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("xdc.runtime.knl", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['xdcruntime.cstubs'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/xdcruntime.cstubs.a86U',\n");
            sb.append("'lib/xdcruntime.cstubs.av5T',\n");
            sb.append("'lib/xdcruntime.cstubs.abnc',\n");
            sb.append("'lib/xdcruntime.cstubs.a470uC',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/xdcruntime.cstubs.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/xdcruntime.cstubs.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/xdcruntime.cstubs.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/xdcruntime.cstubs.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void LoggerFlex$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("xdcruntime.cstubs.LoggerFlex", "xdcruntime.cstubs");
        po = (Proto.Obj)om.findStrict("xdcruntime.cstubs.LoggerFlex.Module", "xdcruntime.cstubs");
        vo.init2(po, "xdcruntime.cstubs.LoggerFlex", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("xdcruntime.cstubs.LoggerFlex$$capsule", "xdcruntime.cstubs"));
        vo.bind("Instance", om.findStrict("xdcruntime.cstubs.LoggerFlex.Instance", "xdcruntime.cstubs"));
        vo.bind("Params", om.findStrict("xdcruntime.cstubs.LoggerFlex.Params", "xdcruntime.cstubs"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("xdcruntime.cstubs.LoggerFlex.Params", "xdcruntime.cstubs")).newInstance());
        vo.bind("Handle", om.findStrict("xdcruntime.cstubs.LoggerFlex.Handle", "xdcruntime.cstubs"));
        vo.bind("$package", om.findStrict("xdcruntime.cstubs", "xdcruntime.cstubs"));
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
        mcfgs.add("E_atexit");
        vo.bind("Instance_State", om.findStrict("xdcruntime.cstubs.LoggerFlex.Instance_State", "xdcruntime.cstubs"));
        tdefs.add(om.findStrict("xdcruntime.cstubs.LoggerFlex.Instance_State", "xdcruntime.cstubs"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "xdcruntime.cstubs")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("xdcruntime.cstubs.LoggerFlex$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("xdcruntime$cstubs$LoggerFlex$$__initObject"));
        }//isCFG
        vo.bind("shutdown", om.findStrict("xdcruntime.cstubs.LoggerFlex.shutdown", "xdcruntime.cstubs"));
        vo.bind("$$fxntab", Global.newArray("xdcruntime_cstubs_LoggerFlex_Handle__label__E", "xdcruntime_cstubs_LoggerFlex_Module__startupDone__E", "xdcruntime_cstubs_LoggerFlex_Object__create__E", "xdcruntime_cstubs_LoggerFlex_Object__delete__E", "xdcruntime_cstubs_LoggerFlex_Object__destruct__E", "xdcruntime_cstubs_LoggerFlex_Object__get__E", "xdcruntime_cstubs_LoggerFlex_Object__first__E", "xdcruntime_cstubs_LoggerFlex_Object__next__E", "xdcruntime_cstubs_LoggerFlex_Params__init__E", "xdcruntime_cstubs_LoggerFlex_enable__E", "xdcruntime_cstubs_LoggerFlex_disable__E", "xdcruntime_cstubs_LoggerFlex_write0__E", "xdcruntime_cstubs_LoggerFlex_write1__E", "xdcruntime_cstubs_LoggerFlex_write2__E", "xdcruntime_cstubs_LoggerFlex_write4__E", "xdcruntime_cstubs_LoggerFlex_write8__E", "xdcruntime_cstubs_LoggerFlex_shutdown__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray("E_atexit"));
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("xdcruntime.cstubs.LoggerFlex.Object", "xdcruntime.cstubs"));
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
        loggables.add(Global.newObject("name", "shutdown", "entry", "%d", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("LoggerFlex", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LoggerFlex");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
            Object srcP = ((XScriptO)om.findStrict("xdc.runtime.IInstance", "xdcruntime.cstubs")).findStrict("PARAMS", "xdcruntime.cstubs");
            Scriptable dstP;

            dstP = (Scriptable)((XScriptO)om.findStrict("xdcruntime.cstubs.LoggerFlex", "xdcruntime.cstubs")).findStrict("PARAMS", "xdcruntime.cstubs");
            Global.put(dstP, "instance", srcP);
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("xdcruntime.cstubs.LoggerFlex", "xdcruntime.cstubs"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("xdcruntime.cstubs.LoggerFlex")).bless();
        ((Value.Arr)om.findStrict("$packages", "xdcruntime.cstubs")).add(pkgV);
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
        LoggerFlex$$OBJECTS();
        LoggerFlex$$CONSTS();
        LoggerFlex$$CREATES();
        LoggerFlex$$FUNCTIONS();
        LoggerFlex$$SIZES();
        LoggerFlex$$TYPES();
        if (isROV) {
            LoggerFlex$$ROV();
        }//isROV
        $$SINGLETONS();
        LoggerFlex$$SINGLETONS();
        $$INITIALIZATION();
    }
}
