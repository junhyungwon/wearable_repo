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

public class ti_sdo_fc_rman_examples
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
        pkgP = (Proto.Obj)om.bind("ti.sdo.fc.rman.examples.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sdo.fc.rman.examples", new Value.Obj("ti.sdo.fc.rman.examples", pkgP));
    }

    void EXAMPLE$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.fc.rman.examples.EXAMPLE.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.fc.rman.examples.EXAMPLE", new Value.Obj("ti.sdo.fc.rman.examples.EXAMPLE", po));
        pkgV.bind("EXAMPLE", vo);
        // decls 
    }

    void EXAMPLE_Module_GateProxy$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy", new Value.Obj("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy", po));
        pkgV.bind("EXAMPLE_Module_GateProxy", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy$$Object", new Proto.Obj());
        Object objP = om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy$$Params", new Proto.Obj());
        om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Params", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy$$Instance_State", new Proto.Obj());
        om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance_State", new Proto.Str(po, false));
        om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Handle", insP);
        if (isROV) {
            om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Object", om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance_State", "ti.sdo.fc.rman.examples"));
        }//isROV
    }

    void EXAMPLE$$CONSTS()
    {
        // module EXAMPLE
    }

    void EXAMPLE_Module_GateProxy$$CONSTS()
    {
        // module EXAMPLE_Module_GateProxy
        om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.query", new Extern("ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_query__E", "xdc_Bool(*)(xdc_Int)", true, false));
    }

    void EXAMPLE$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void EXAMPLE_Module_GateProxy$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sdo$fc$rman$examples$EXAMPLE_Module_GateProxy$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.create() called before xdc.useModule('ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy')\");\n");
                sb.append("}\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy$$create", new Proto.Fxn(om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Module", "ti.sdo.fc.rman.examples"), om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance", "ti.sdo.fc.rman.examples"), 1, 0, false));
                        fxn.addArg(0, "__params", (Proto)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Params", "ti.sdo.fc.rman.examples"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sdo$fc$rman$examples$EXAMPLE_Module_GateProxy$$create = function( __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy'];\n");
                sb.append("var __inst = xdc.om['ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sdo.fc.rman.examples']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy'].Instance_State);\n");
                sb.append("if (!__mod.delegate$) {\n");
                    sb.append("throw new Error(\"Unbound proxy module: ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy\");\n");
                sb.append("}\n");
                sb.append("var __dmod = __mod.delegate$.$orig;\n");
                sb.append("var __dinst = __dmod.create(__params);\n");
                sb.append("__inst.$$bind('delegate$', __dinst);\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void EXAMPLE$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void EXAMPLE_Module_GateProxy$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void EXAMPLE$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void EXAMPLE_Module_GateProxy$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void EXAMPLE$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/fc/rman/examples/EXAMPLE.xs");
        om.bind("ti.sdo.fc.rman.examples.EXAMPLE$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE.Module", "ti.sdo.fc.rman.examples");
        po.init("ti.sdo.fc.rman.examples.EXAMPLE.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.sdo.fc.rman.examples"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("gate", (Proto)om.findStrict("xdc.runtime.IGateProvider.Handle", "ti.sdo.fc.rman.examples"), $$UNDEF, "w");
            po.addFldV("Module_GateProxy", (Proto)om.findStrict("xdc.runtime.IGateProvider.Module", "ti.sdo.fc.rman.examples"), null, "wh", $$delegGet, $$delegSet);
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sdo.fc.rman.examples.EXAMPLE$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sdo.fc.rman.examples.EXAMPLE$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sdo.fc.rman.examples.EXAMPLE$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sdo.fc.rman.examples.EXAMPLE$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void EXAMPLE_Module_GateProxy$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Module", "ti.sdo.fc.rman.examples");
        po.init("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Module", om.findStrict("xdc.runtime.IGateProvider.Module", "ti.sdo.fc.rman.examples"));
                po.addFld("delegate$", (Proto)om.findStrict("xdc.runtime.IGateProvider.Module", "ti.sdo.fc.rman.examples"), null, "wh");
                po.addFld("abstractInstances$", $$T_Bool, false, "wh");
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
        if (isCFG) {
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy$$create", "ti.sdo.fc.rman.examples"), Global.get("ti$sdo$fc$rman$examples$EXAMPLE_Module_GateProxy$$create"));
        }//isCFG
                po.addFxn("queryMeta", (Proto.Fxn)om.findStrict("xdc.runtime.IGateProvider$$queryMeta", "ti.sdo.fc.rman.examples"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance", "ti.sdo.fc.rman.examples");
        po.init("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance", om.findStrict("xdc.runtime.IGateProvider.Instance", "ti.sdo.fc.rman.examples"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.fc.rman.examples"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy$$Params", "ti.sdo.fc.rman.examples");
        po.init("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Params", om.findStrict("xdc.runtime.IGateProvider$$Params", "ti.sdo.fc.rman.examples"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("Q_BLOCKING", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("Q_PREEMPTING", Proto.Elm.newCNum("(xdc_Int)"), 2L, "rh");
        if (isCFG) {
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sdo.fc.rman.examples"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy$$Object", "ti.sdo.fc.rman.examples");
        po.init("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Object", om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance", "ti.sdo.fc.rman.examples"));
        po = (Proto.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy$$Instance_State", "ti.sdo.fc.rman.examples");
        po.init("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance_State", null);
        po.addFld("$hostonly", $$T_Num, 0, "r");
    }

    void EXAMPLE$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE", "ti.sdo.fc.rman.examples");
    }

    void EXAMPLE_Module_GateProxy$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy", "ti.sdo.fc.rman.examples");
        po = (Proto.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy$$Instance_State", "ti.sdo.fc.rman.examples");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sdo.fc.rman.examples.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sdo.fc.rman.examples"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/fc/rman/examples/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sdo.fc.rman.examples"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sdo.fc.rman.examples"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sdo.fc.rman.examples"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sdo.fc.rman.examples"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sdo.fc.rman.examples"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sdo.fc.rman.examples"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sdo.fc.rman.examples", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sdo.fc.rman.examples");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sdo.fc.rman.examples.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sdo.fc.rman.examples'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/release/rmanExample.a64P',\n");
            sb.append("'lib/debug/rmanExample.a64P',\n");
            sb.append("'lib/release/rmanExample.a674',\n");
            sb.append("'lib/debug/rmanExample.a674',\n");
            sb.append("'lib/release/rmanExample.ae64P',\n");
            sb.append("'lib/debug/rmanExample.ae64P',\n");
            sb.append("'lib/release/rmanExample.ae64T',\n");
            sb.append("'lib/debug/rmanExample.ae64T',\n");
            sb.append("'lib/release/rmanExample.ae674',\n");
            sb.append("'lib/debug/rmanExample.ae674',\n");
            sb.append("'lib/release/rmanExample.aearp32',\n");
            sb.append("'lib/debug/rmanExample.aearp32',\n");
            sb.append("'lib/release/rmanExample.a86U',\n");
            sb.append("'lib/debug/rmanExample.a86U',\n");
            sb.append("'lib/release/rmanExample.av5T',\n");
            sb.append("'lib/debug/rmanExample.av5T',\n");
            sb.append("'lib/release/rmanExample.abnc',\n");
            sb.append("'lib/debug/rmanExample.abnc',\n");
            sb.append("'lib/release/rmanExample.a470uC',\n");
            sb.append("'lib/debug/rmanExample.a470uC',\n");
            sb.append("'lib/release/rmanExample.aem3',\n");
            sb.append("'lib/debug/rmanExample.aem3',\n");
            sb.append("'lib/release/rmanExample.aea8f',\n");
            sb.append("'lib/debug/rmanExample.aea8f',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/release/rmanExample.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/debug/rmanExample.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/release/rmanExample.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/debug/rmanExample.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/release/rmanExample.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/debug/rmanExample.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/release/rmanExample.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/debug/rmanExample.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/release/rmanExample.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/debug/rmanExample.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/release/rmanExample.aearp32', {target: 'ti.targets.elf.arp32.ARP32', suffix: 'earp32'}],\n");
            sb.append("['lib/debug/rmanExample.aearp32', {target: 'ti.targets.elf.arp32.ARP32', suffix: 'earp32'}],\n");
            sb.append("['lib/release/rmanExample.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/debug/rmanExample.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/release/rmanExample.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/debug/rmanExample.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/release/rmanExample.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/debug/rmanExample.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/release/rmanExample.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/debug/rmanExample.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/release/rmanExample.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/debug/rmanExample.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/release/rmanExample.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
            sb.append("['lib/debug/rmanExample.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void EXAMPLE$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE", "ti.sdo.fc.rman.examples");
        po = (Proto.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE.Module", "ti.sdo.fc.rman.examples");
        vo.init2(po, "ti.sdo.fc.rman.examples.EXAMPLE", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE$$capsule", "ti.sdo.fc.rman.examples"));
        vo.bind("$package", om.findStrict("ti.sdo.fc.rman.examples", "ti.sdo.fc.rman.examples"));
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
        mcfgs.add("gate");
        vo.bind("Module_GateProxy$proxy", om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy", "ti.sdo.fc.rman.examples"));
        proxies.add("Module_GateProxy");
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.fc.rman.examples")).add(vo);
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
        vo.bind("$$fxntab", Global.newArray("ti_sdo_fc_rman_examples_EXAMPLE_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "./EXAMPLE.xdt");
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        vo.bind("TEMPLATE$", "./EXAMPLE.xdt");
        pkgV.bind("EXAMPLE", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("EXAMPLE");
    }

    void EXAMPLE_Module_GateProxy$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy", "ti.sdo.fc.rman.examples");
        po = (Proto.Obj)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Module", "ti.sdo.fc.rman.examples");
        vo.init2(po, "ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance", "ti.sdo.fc.rman.examples"));
        vo.bind("Params", om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Params", "ti.sdo.fc.rman.examples"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Params", "ti.sdo.fc.rman.examples")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Handle", "ti.sdo.fc.rman.examples"));
        vo.bind("$package", om.findStrict("ti.sdo.fc.rman.examples", "ti.sdo.fc.rman.examples"));
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
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.fc.rman.examples")).add(vo);
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
            vo.bind("__initObject", Global.get("ti$sdo$fc$rman$examples$EXAMPLE_Module_GateProxy$$__initObject"));
        }//isCFG
        vo.bind("query", om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.query", "ti.sdo.fc.rman.examples"));
        vo.bind("$$fxntab", Global.newArray("ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Handle__label", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Module__startupDone", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Object__create", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Object__delete", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Object__destruct", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Object__get", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Object__first", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Object__next", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Params__init", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Proxy__abstract", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__Proxy__delegate", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__queryMeta", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__query", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__enter", "ti_sdo_fc_rman_examples_EXAMPLE_Module_GateProxy_DELEGATE__leave"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Object", "ti.sdo.fc.rman.examples"));
        vo.bind("Instance_State", om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy.Instance_State", "ti.sdo.fc.rman.examples"));
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 1);
        loggables.clear();
        loggables.add(Global.newObject("name", "query", "entry", "%d", "exit", "%d"));
        loggables.add(Global.newObject("name", "enter", "entry", "%p", "exit", "%p"));
        loggables.add(Global.newObject("name", "leave", "entry", "%p, %p", "exit", ""));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("EXAMPLE_Module_GateProxy", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("EXAMPLE_Module_GateProxy");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
            Object srcP = ((XScriptO)om.findStrict("xdc.runtime.IInstance", "ti.sdo.fc.rman.examples")).findStrict("PARAMS", "ti.sdo.fc.rman.examples");
            Scriptable dstP;

            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy", "ti.sdo.fc.rman.examples")).findStrict("PARAMS", "ti.sdo.fc.rman.examples");
            Global.put(dstP, "instance", srcP);
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE", "ti.sdo.fc.rman.examples"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy", "ti.sdo.fc.rman.examples"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sdo.fc.rman.examples.EXAMPLE")).bless();
        ((Value.Obj)om.getv("ti.sdo.fc.rman.examples.EXAMPLE_Module_GateProxy")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sdo.fc.rman.examples")).add(pkgV);
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
        EXAMPLE$$OBJECTS();
        EXAMPLE_Module_GateProxy$$OBJECTS();
        EXAMPLE$$CONSTS();
        EXAMPLE_Module_GateProxy$$CONSTS();
        EXAMPLE$$CREATES();
        EXAMPLE_Module_GateProxy$$CREATES();
        EXAMPLE$$FUNCTIONS();
        EXAMPLE_Module_GateProxy$$FUNCTIONS();
        EXAMPLE$$SIZES();
        EXAMPLE_Module_GateProxy$$SIZES();
        EXAMPLE$$TYPES();
        EXAMPLE_Module_GateProxy$$TYPES();
        if (isROV) {
            EXAMPLE$$ROV();
            EXAMPLE_Module_GateProxy$$ROV();
        }//isROV
        $$SINGLETONS();
        EXAMPLE$$SINGLETONS();
        EXAMPLE_Module_GateProxy$$SINGLETONS();
        $$INITIALIZATION();
    }
}
