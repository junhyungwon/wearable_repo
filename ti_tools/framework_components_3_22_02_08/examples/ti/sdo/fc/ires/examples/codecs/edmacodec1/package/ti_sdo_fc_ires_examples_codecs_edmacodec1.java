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

public class ti_sdo_fc_ires_examples_codecs_edmacodec1
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
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.sdo.fc.ires.examples.codecs.edmacodec1.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sdo.fc.ires.examples.codecs.edmacodec1", new Value.Obj("ti.sdo.fc.ires.examples.codecs.edmacodec1", pkgP));
    }

    void EDMACODEC1$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1", new Value.Obj("ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1", po));
        pkgV.bind("EDMACODEC1", vo);
        // decls 
    }

    void EDMACODEC1$$CONSTS()
    {
        // module EDMACODEC1
    }

    void EDMACODEC1$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void EDMACODEC1$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void EDMACODEC1$$SIZES()
    {
    }

    void EDMACODEC1$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1.Module", "ti.sdo.fc.ires.examples.codecs.edmacodec1");
        po.init("ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1.Module", $$Module);
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("debug", $$T_Bool, false, "wh");
    }

    void EDMACODEC1$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sdo.fc.ires.examples.codecs.edmacodec1.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sdo.fc.ires.examples.codecs.edmacodec1"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/fc/ires/examples/codecs/edmacodec1/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sdo.fc.ires.examples.codecs.edmacodec1"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sdo.fc.ires.examples.codecs.edmacodec1"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sdo.fc.ires.examples.codecs.edmacodec1"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sdo.fc.ires.examples.codecs.edmacodec1"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sdo.fc.ires.examples.codecs.edmacodec1"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sdo.fc.ires.examples.codecs.edmacodec1"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sdo.fc.ires.examples.codecs.edmacodec1", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sdo.fc.ires.examples.codecs.edmacodec1");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sdo.fc.ires.examples.codecs.edmacodec1.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sdo.fc.ires.examples.codecs.edmacodec1'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/debug/edmacodec1_ti.a64P',\n");
            sb.append("'lib/release/edmacodec1_ti.a64P',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.a64P',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.a64P',\n");
            sb.append("'lib/debug/edmacodec1_ti.a674',\n");
            sb.append("'lib/release/edmacodec1_ti.a674',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.a674',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.a674',\n");
            sb.append("'lib/debug/edmacodec1_ti.ae64P',\n");
            sb.append("'lib/release/edmacodec1_ti.ae64P',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.ae64P',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.ae64P',\n");
            sb.append("'lib/debug/edmacodec1_ti.ae64T',\n");
            sb.append("'lib/release/edmacodec1_ti.ae64T',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.ae64T',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.ae64T',\n");
            sb.append("'lib/debug/edmacodec1_ti.ae674',\n");
            sb.append("'lib/release/edmacodec1_ti.ae674',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.ae674',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.ae674',\n");
            sb.append("'lib/debug/edmacodec1_ti.aearp32',\n");
            sb.append("'lib/release/edmacodec1_ti.aearp32',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.aearp32',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.aearp32',\n");
            sb.append("'lib/debug/edmacodec1_ti.a86U',\n");
            sb.append("'lib/release/edmacodec1_ti.a86U',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.a86U',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.a86U',\n");
            sb.append("'lib/debug/edmacodec1_ti.av5T',\n");
            sb.append("'lib/release/edmacodec1_ti.av5T',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.av5T',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.av5T',\n");
            sb.append("'lib/debug/edmacodec1_ti.abnc',\n");
            sb.append("'lib/release/edmacodec1_ti.abnc',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.abnc',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.abnc',\n");
            sb.append("'lib/debug/edmacodec1_ti.a470uC',\n");
            sb.append("'lib/release/edmacodec1_ti.a470uC',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.a470uC',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.a470uC',\n");
            sb.append("'lib/debug/edmacodec1_ti.aem3',\n");
            sb.append("'lib/release/edmacodec1_ti.aem3',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.aem3',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.aem3',\n");
            sb.append("'lib/debug/edmacodec1_ti.aea8f',\n");
            sb.append("'lib/release/edmacodec1_ti.aea8f',\n");
            sb.append("'lib/release/edmacodec1_ti_TRACE.aea8f',\n");
            sb.append("'lib/debug/edmacodec1_ti_TRACE.aea8f',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/debug/edmacodec1_ti.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/release/edmacodec1_ti.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.a64P', {target: 'ti.targets.C64P', suffix: '64P'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/release/edmacodec1_ti.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.a674', {target: 'ti.targets.C674', suffix: '674'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/release/edmacodec1_ti.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.ae64P', {target: 'ti.targets.elf.C64P', suffix: 'e64P'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/release/edmacodec1_ti.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.ae64T', {target: 'ti.targets.elf.C64T', suffix: 'e64T'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/release/edmacodec1_ti.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.ae674', {target: 'ti.targets.elf.C674', suffix: 'e674'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.aearp32', {target: 'ti.targets.elf.arp32.ARP32', suffix: 'earp32'}],\n");
            sb.append("['lib/release/edmacodec1_ti.aearp32', {target: 'ti.targets.elf.arp32.ARP32', suffix: 'earp32'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.aearp32', {target: 'ti.targets.elf.arp32.ARP32', suffix: 'earp32'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.aearp32', {target: 'ti.targets.elf.arp32.ARP32', suffix: 'earp32'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/release/edmacodec1_ti.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.a86U', {target: 'gnu.targets.Linux86', suffix: '86U'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/release/edmacodec1_ti.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.av5T', {target: 'gnu.targets.arm.GCArmv5T', suffix: 'v5T'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/release/edmacodec1_ti.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.abnc', {target: 'google.targets.arm.Bionic', suffix: 'bnc'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/release/edmacodec1_ti.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.a470uC', {target: 'gnu.targets.arm.UCArm9', suffix: '470uC'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/release/edmacodec1_ti.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/debug/edmacodec1_ti.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
            sb.append("['lib/release/edmacodec1_ti.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
            sb.append("['lib/release/edmacodec1_ti_TRACE.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
            sb.append("['lib/debug/edmacodec1_ti_TRACE.aea8f', {target: 'ti.targets.arm.elf.A8F', suffix: 'ea8f'}],\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void EDMACODEC1$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1", "ti.sdo.fc.ires.examples.codecs.edmacodec1");
        po = (Proto.Obj)om.findStrict("ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1.Module", "ti.sdo.fc.ires.examples.codecs.edmacodec1");
        vo.init2(po, "ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("$package", om.findStrict("ti.sdo.fc.ires.examples.codecs.edmacodec1", "ti.sdo.fc.ires.examples.codecs.edmacodec1"));
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
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.fc.ires.examples.codecs.edmacodec1")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        pkgV.bind("EDMACODEC1", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("EDMACODEC1");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1", "ti.sdo.fc.ires.examples.codecs.edmacodec1"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sdo.fc.ires.examples.codecs.edmacodec1.EDMACODEC1")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sdo.fc.ires.examples.codecs.edmacodec1")).add(pkgV);
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
        EDMACODEC1$$OBJECTS();
        EDMACODEC1$$CONSTS();
        EDMACODEC1$$CREATES();
        EDMACODEC1$$FUNCTIONS();
        EDMACODEC1$$SIZES();
        EDMACODEC1$$TYPES();
        if (isROV) {
            EDMACODEC1$$ROV();
        }//isROV
        $$SINGLETONS();
        EDMACODEC1$$SINGLETONS();
        $$INITIALIZATION();
    }
}
