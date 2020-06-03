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

public class xdcruntime_heaps
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
        pkgP = (Proto.Obj)om.bind("xdcruntime.heaps.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("xdcruntime.heaps", new Value.Obj("xdcruntime.heaps", pkgP));
    }

    void HeapStdAlign$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("xdcruntime.heaps.HeapStdAlign.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("xdcruntime.heaps.HeapStdAlign", new Value.Obj("xdcruntime.heaps.HeapStdAlign", po));
        pkgV.bind("HeapStdAlign", vo);
        // decls 
        spo = (Proto.Obj)om.bind("xdcruntime.heaps.HeapStdAlign$$Instance_View", new Proto.Obj());
        om.bind("xdcruntime.heaps.HeapStdAlign.Instance_View", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("xdcruntime.heaps.HeapStdAlign$$Module_State", new Proto.Obj());
        om.bind("xdcruntime.heaps.HeapStdAlign.Module_State", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("xdcruntime.heaps.HeapStdAlign$$Instance_State", new Proto.Obj());
        om.bind("xdcruntime.heaps.HeapStdAlign.Instance_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("xdcruntime.heaps.HeapStdAlign.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("xdcruntime.heaps.HeapStdAlign$$Object", new Proto.Obj());
        Object objP = om.bind("xdcruntime.heaps.HeapStdAlign.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("xdcruntime.heaps.HeapStdAlign$$Params", new Proto.Obj());
        om.bind("xdcruntime.heaps.HeapStdAlign.Params", new Proto.Str(po, false));
        om.bind("xdcruntime.heaps.HeapStdAlign.Handle", insP);
        if (isROV) {
            om.bind("xdcruntime.heaps.HeapStdAlign.Object", om.findStrict("xdcruntime.heaps.HeapStdAlign.Instance_State", "xdcruntime.heaps"));
        }//isROV
    }

    void HeapStdAlign$$CONSTS()
    {
        // module HeapStdAlign
    }

    void HeapStdAlign$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("xdcruntime$heaps$HeapStdAlign$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function xdcruntime.heaps.HeapStdAlign.create() called before xdc.useModule('xdcruntime.heaps.HeapStdAlign')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("xdcruntime.heaps.HeapStdAlign$$create", new Proto.Fxn(om.findStrict("xdcruntime.heaps.HeapStdAlign.Module", "xdcruntime.heaps"), om.findStrict("xdcruntime.heaps.HeapStdAlign.Instance", "xdcruntime.heaps"), 1, 0, false));
                        fxn.addArg(0, "__params", (Proto)om.findStrict("xdcruntime.heaps.HeapStdAlign.Params", "xdcruntime.heaps"), Global.newObject());
            sb = new StringBuilder();
            sb.append("xdcruntime$heaps$HeapStdAlign$$create = function( __params ) {\n");
                sb.append("var __mod = xdc.om['xdcruntime.heaps.HeapStdAlign'];\n");
                sb.append("var __inst = xdc.om['xdcruntime.heaps.HeapStdAlign.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['xdcruntime.heaps']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['xdcruntime.heaps.HeapStdAlign'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['xdcruntime.heaps.HeapStdAlign'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("xdcruntime.heaps.HeapStdAlign$$construct", new Proto.Fxn(om.findStrict("xdcruntime.heaps.HeapStdAlign.Module", "xdcruntime.heaps"), null, 2, 0, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("xdcruntime.heaps.HeapStdAlign$$Object", "xdcruntime.heaps"), null);
                        fxn.addArg(1, "__params", (Proto)om.findStrict("xdcruntime.heaps.HeapStdAlign.Params", "xdcruntime.heaps"), Global.newObject());
            sb = new StringBuilder();
            sb.append("xdcruntime$heaps$HeapStdAlign$$construct = function( __obj, __params ) {\n");
                sb.append("var __mod = xdc.om['xdcruntime.heaps.HeapStdAlign'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['xdcruntime.heaps.HeapStdAlign'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['xdcruntime.heaps.HeapStdAlign'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void HeapStdAlign$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void HeapStdAlign$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("xdcruntime.heaps.HeapStdAlign.Module_State", "xdcruntime.heaps");
        sizes.clear();
        sizes.add(Global.newArray("remainRTSSize", "UIArg"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['xdcruntime.heaps.HeapStdAlign.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['xdcruntime.heaps.HeapStdAlign.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['xdcruntime.heaps.HeapStdAlign.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("xdcruntime.heaps.HeapStdAlign.Instance_State", "xdcruntime.heaps");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("remainSize", "UIArg"));
        sizes.add(Global.newArray("startSize", "UIArg"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['xdcruntime.heaps.HeapStdAlign.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['xdcruntime.heaps.HeapStdAlign.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['xdcruntime.heaps.HeapStdAlign.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void HeapStdAlign$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "xdcruntime/heaps/HeapStdAlign.xs");
        om.bind("xdcruntime.heaps.HeapStdAlign$$capsule", cap);
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign.Module", "xdcruntime.heaps");
        po.init("xdcruntime.heaps.HeapStdAlign.Module", om.findStrict("xdc.runtime.IHeap.Module", "xdcruntime.heaps"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("E_noRTSMemory", (Proto)om.findStrict("xdc.runtime.Error$$Id", "xdcruntime.heaps"), Global.newObject("msg", "The RTS heap is used up. Examine Program.heap."), "w");
            po.addFld("A_zeroSize", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "xdcruntime.heaps"), Global.newObject("msg", "HeapStdAlign_create cannot have a zero size value"), "w");
            po.addFld("A_align", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "xdcruntime.heaps"), Global.newObject("msg", "A_align: Requested align is not a power of 2"), "w");
            po.addFld("A_invalidTotalFreeSize", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "xdcruntime.heaps"), Global.newObject("msg", "HeapStdAlign instance totalFreeSize > than starting size"), "w");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("xdcruntime.heaps.HeapStdAlign$$create", "xdcruntime.heaps"), Global.get("xdcruntime$heaps$HeapStdAlign$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("xdcruntime.heaps.HeapStdAlign$$construct", "xdcruntime.heaps"), Global.get("xdcruntime$heaps$HeapStdAlign$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("xdcruntime.heaps.HeapStdAlign$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("xdcruntime.heaps.HeapStdAlign$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("xdcruntime.heaps.HeapStdAlign$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("xdcruntime.heaps.HeapStdAlign$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("xdcruntime.heaps.HeapStdAlign$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("xdcruntime.heaps.HeapStdAlign$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign.Instance", "xdcruntime.heaps");
        po.init("xdcruntime.heaps.HeapStdAlign.Instance", om.findStrict("xdc.runtime.IHeap.Instance", "xdcruntime.heaps"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("size", new Proto.Adr("xdc_UArg", "Pv"), 0L, "w");
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "xdcruntime.heaps"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign$$Params", "xdcruntime.heaps");
        po.init("xdcruntime.heaps.HeapStdAlign.Params", om.findStrict("xdc.runtime.IHeap$$Params", "xdcruntime.heaps"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("size", new Proto.Adr("xdc_UArg", "Pv"), 0L, "w");
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "xdcruntime.heaps"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign$$Object", "xdcruntime.heaps");
        po.init("xdcruntime.heaps.HeapStdAlign.Object", om.findStrict("xdcruntime.heaps.HeapStdAlign.Instance", "xdcruntime.heaps"));
        // struct HeapStdAlign.Instance_View
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign$$Instance_View", "xdcruntime.heaps");
        po.init("xdcruntime.heaps.HeapStdAlign.Instance_View", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("address", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("label", $$T_Str, $$UNDEF, "w");
                po.addFld("remainSize", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("startSize", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFxn("$xml", $$T_Met, Global.get("$$xml"));
        // struct HeapStdAlign.Module_State
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign$$Module_State", "xdcruntime.heaps");
        po.init("xdcruntime.heaps.HeapStdAlign.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("remainRTSSize", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
        // struct HeapStdAlign.Instance_State
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign$$Instance_State", "xdcruntime.heaps");
        po.init("xdcruntime.heaps.HeapStdAlign.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("remainSize", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("startSize", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
    }

    void HeapStdAlign$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign", "xdcruntime.heaps");
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign$$Instance_State", "xdcruntime.heaps");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "xdcruntime.heaps.HeapStdAlign.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign$$Module_State", "xdcruntime.heaps");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "xdcruntime.heaps.HeapStdAlign.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign$$Instance_State", "xdcruntime.heaps");
    }

    void $$SINGLETONS()
    {
        pkgP.init("xdcruntime.heaps.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "xdcruntime.heaps"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "xdcruntime/heaps/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "xdcruntime.heaps"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "xdcruntime.heaps"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "xdcruntime.heaps"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "xdcruntime.heaps"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "xdcruntime.heaps"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "xdcruntime.heaps"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "xdcruntime.heaps", Value.DEFAULT, false);
        pkgV.bind("$name", "xdcruntime.heaps");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "xdcruntime.heaps.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['xdcruntime.heaps'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/release/heaps.a64P',\n");
            sb.append("'lib/debug/heaps.a64P',\n");
            sb.append("'lib/release/heaps.ae64P',\n");
            sb.append("'lib/debug/heaps.ae64P',\n");
            sb.append("'lib/release/heaps.a674',\n");
            sb.append("'lib/debug/heaps.a674',\n");
            sb.append("'lib/release/heaps.ae674',\n");
            sb.append("'lib/debug/heaps.ae674',\n");
            sb.append("'lib/release/heaps.ae64T',\n");
            sb.append("'lib/debug/heaps.ae64T',\n");
            sb.append("'lib/release/heaps.aem3',\n");
            sb.append("'lib/debug/heaps.aem3',\n");
            sb.append("'lib/release/heaps.a86U',\n");
            sb.append("'lib/debug/heaps.a86U',\n");
            sb.append("'lib/release/heaps.av5T',\n");
            sb.append("'lib/debug/heaps.av5T',\n");
            sb.append("'lib/release/heaps.aqv7',\n");
            sb.append("'lib/debug/heaps.aqv7',\n");
            sb.append("'lib/release/heaps.abnc',\n");
            sb.append("'lib/debug/heaps.abnc',\n");
            sb.append("'lib/release/heaps.a470uC',\n");
            sb.append("'lib/debug/heaps.a470uC',\n");
            sb.append("'lib/release/heaps.aea8f',\n");
            sb.append("'lib/debug/heaps.aea8f',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/release/heaps.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/debug/heaps.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/release/heaps.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/debug/heaps.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/release/heaps.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/debug/heaps.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/release/heaps.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/debug/heaps.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/release/heaps.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/debug/heaps.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/release/heaps.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/debug/heaps.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/release/heaps.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/debug/heaps.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/release/heaps.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/debug/heaps.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/release/heaps.aqv7', {target: 'qnx.targets.arm.Qnx', suffix: 'qv7'}],\n");
            sb.append("['lib/debug/heaps.aqv7', {target: 'qnx.targets.arm.Qnx', suffix: 'qv7'}],\n");
            sb.append("['lib/release/heaps.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/debug/heaps.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/release/heaps.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/debug/heaps.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/release/heaps.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
            sb.append("['lib/debug/heaps.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void HeapStdAlign$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign", "xdcruntime.heaps");
        po = (Proto.Obj)om.findStrict("xdcruntime.heaps.HeapStdAlign.Module", "xdcruntime.heaps");
        vo.init2(po, "xdcruntime.heaps.HeapStdAlign", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("xdcruntime.heaps.HeapStdAlign$$capsule", "xdcruntime.heaps"));
        vo.bind("Instance", om.findStrict("xdcruntime.heaps.HeapStdAlign.Instance", "xdcruntime.heaps"));
        vo.bind("Params", om.findStrict("xdcruntime.heaps.HeapStdAlign.Params", "xdcruntime.heaps"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("xdcruntime.heaps.HeapStdAlign.Params", "xdcruntime.heaps")).newInstance());
        vo.bind("Handle", om.findStrict("xdcruntime.heaps.HeapStdAlign.Handle", "xdcruntime.heaps"));
        vo.bind("$package", om.findStrict("xdcruntime.heaps", "xdcruntime.heaps"));
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
        vo.bind("Instance_View", om.findStrict("xdcruntime.heaps.HeapStdAlign.Instance_View", "xdcruntime.heaps"));
        tdefs.add(om.findStrict("xdcruntime.heaps.HeapStdAlign.Instance_View", "xdcruntime.heaps"));
        mcfgs.add("E_noRTSMemory");
        mcfgs.add("A_zeroSize");
        mcfgs.add("A_align");
        mcfgs.add("A_invalidTotalFreeSize");
        vo.bind("Module_State", om.findStrict("xdcruntime.heaps.HeapStdAlign.Module_State", "xdcruntime.heaps"));
        tdefs.add(om.findStrict("xdcruntime.heaps.HeapStdAlign.Module_State", "xdcruntime.heaps"));
        vo.bind("Instance_State", om.findStrict("xdcruntime.heaps.HeapStdAlign.Instance_State", "xdcruntime.heaps"));
        tdefs.add(om.findStrict("xdcruntime.heaps.HeapStdAlign.Instance_State", "xdcruntime.heaps"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "xdcruntime.heaps")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("xdcruntime.heaps.HeapStdAlign$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("xdcruntime$heaps$HeapStdAlign$$__initObject"));
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("xdcruntime_heaps_HeapStdAlign_Handle__label__E", "xdcruntime_heaps_HeapStdAlign_Module__startupDone__E", "xdcruntime_heaps_HeapStdAlign_Object__create__E", "xdcruntime_heaps_HeapStdAlign_Object__delete__E", "xdcruntime_heaps_HeapStdAlign_Object__destruct__E", "xdcruntime_heaps_HeapStdAlign_Object__get__E", "xdcruntime_heaps_HeapStdAlign_Object__first__E", "xdcruntime_heaps_HeapStdAlign_Object__next__E", "xdcruntime_heaps_HeapStdAlign_Params__init__E", "xdcruntime_heaps_HeapStdAlign_free__E", "xdcruntime_heaps_HeapStdAlign_getStats__E", "xdcruntime_heaps_HeapStdAlign_alloc__E", "xdcruntime_heaps_HeapStdAlign_isBlocking__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray("E_noRTSMemory"));
        vo.bind("$$assertDescCfgs", Global.newArray("A_zeroSize", "A_align", "A_invalidTotalFreeSize"));
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("xdcruntime.heaps.HeapStdAlign.Object", "xdcruntime.heaps"));
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "free", "entry", "%p, %p, 0x%x", "exit", ""));
        loggables.add(Global.newObject("name", "getStats", "entry", "%p, %p", "exit", ""));
        loggables.add(Global.newObject("name", "alloc", "entry", "%p, 0x%x, 0x%x, %p", "exit", "%p"));
        loggables.add(Global.newObject("name", "isBlocking", "entry", "%p", "exit", "%d"));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("HeapStdAlign", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("HeapStdAlign");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
            Object srcP = ((XScriptO)om.findStrict("xdc.runtime.IInstance", "xdcruntime.heaps")).findStrict("PARAMS", "xdcruntime.heaps");
            Scriptable dstP;

            dstP = (Scriptable)((XScriptO)om.findStrict("xdcruntime.heaps.HeapStdAlign", "xdcruntime.heaps")).findStrict("PARAMS", "xdcruntime.heaps");
            Global.put(dstP, "instance", srcP);
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("xdcruntime.heaps.HeapStdAlign", "xdcruntime.heaps"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("xdcruntime.heaps.HeapStdAlign")).bless();
        ((Value.Arr)om.findStrict("$packages", "xdcruntime.heaps")).add(pkgV);
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
        HeapStdAlign$$OBJECTS();
        HeapStdAlign$$CONSTS();
        HeapStdAlign$$CREATES();
        HeapStdAlign$$FUNCTIONS();
        HeapStdAlign$$SIZES();
        HeapStdAlign$$TYPES();
        if (isROV) {
            HeapStdAlign$$ROV();
        }//isROV
        $$SINGLETONS();
        HeapStdAlign$$SINGLETONS();
        $$INITIALIZATION();
    }
}
