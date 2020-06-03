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

public class ti_sdo_tools_build
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
        Global.callFxn("loadPackage", xdcO, "xdc.bld");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.sdo.tools.build.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sdo.tools.build", new Value.Obj("ti.sdo.tools.build", pkgP));
    }

    void PackageBuildHelp$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sdo.tools.build.PackageBuildHelp.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sdo.tools.build.PackageBuildHelp", new Value.Obj("ti.sdo.tools.build.PackageBuildHelp", po));
        pkgV.bind("PackageBuildHelp", vo);
        // decls 
        spo = (Proto.Obj)om.bind("ti.sdo.tools.build.PackageBuildHelp$$LibAttrs", new Proto.Obj());
        om.bind("ti.sdo.tools.build.PackageBuildHelp.LibAttrs", new Proto.Str(spo, true));
        spo = (Proto.Obj)om.bind("ti.sdo.tools.build.PackageBuildHelp$$ProgAttrs", new Proto.Obj());
        om.bind("ti.sdo.tools.build.PackageBuildHelp.ProgAttrs", new Proto.Str(spo, true));
    }

    void PackageBuildHelp$$CONSTS()
    {
        // module PackageBuildHelp
    }

    void PackageBuildHelp$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void PackageBuildHelp$$FUNCTIONS()
    {
        Proto.Fxn fxn;

        // fxn PackageBuildHelp.makeExecutables
        fxn = (Proto.Fxn)om.bind("ti.sdo.tools.build.PackageBuildHelp$$makeExecutables", new Proto.Fxn(om.findStrict("ti.sdo.tools.build.PackageBuildHelp.Module", "ti.sdo.tools.build"), null, 3, 2, false));
                fxn.addArg(0, "progAry", new Proto.Arr((Proto)om.findStrict("ti.sdo.tools.build.PackageBuildHelp.ProgAttrs", "ti.sdo.tools.build"), false), $$DEFAULT);
                fxn.addArg(1, "arguments", $$T_Obj, $$UNDEF);
                fxn.addArg(2, "profiles", $$T_Obj, $$UNDEF);
        // fxn PackageBuildHelp.makeLibraries
        fxn = (Proto.Fxn)om.bind("ti.sdo.tools.build.PackageBuildHelp$$makeLibraries", new Proto.Fxn(om.findStrict("ti.sdo.tools.build.PackageBuildHelp.Module", "ti.sdo.tools.build"), null, 3, 2, false));
                fxn.addArg(0, "libAry", new Proto.Arr((Proto)om.findStrict("ti.sdo.tools.build.PackageBuildHelp.LibAttrs", "ti.sdo.tools.build"), false), $$DEFAULT);
                fxn.addArg(1, "arguments", $$T_Obj, $$UNDEF);
                fxn.addArg(2, "profiles", $$T_Obj, $$UNDEF);
    }

    void PackageBuildHelp$$SIZES()
    {
    }

    void PackageBuildHelp$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sdo/tools/build/PackageBuildHelp.xs");
        om.bind("ti.sdo.tools.build.PackageBuildHelp$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sdo.tools.build.PackageBuildHelp.Module", "ti.sdo.tools.build");
        po.init("ti.sdo.tools.build.PackageBuildHelp.Module", $$Module);
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("fileArray", $$T_Obj, $$UNDEF, "wh");
        po.addFld("skipExec", $$T_Obj, $$UNDEF, "wh");
        po.addFld("skipLib", $$T_Obj, $$UNDEF, "wh");
        po.addFld("usePlatformInstanceName", $$T_Bool, false, "wh");
        po.addFld("skipPlatform", $$T_Obj, $$UNDEF, "wh");
        po.addFld("skipTarget", $$T_Obj, $$UNDEF, "wh");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sdo.tools.build.PackageBuildHelp$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sdo.tools.build.PackageBuildHelp$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sdo.tools.build.PackageBuildHelp$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                po.addFxn("makeExecutables", (Proto.Fxn)om.findStrict("ti.sdo.tools.build.PackageBuildHelp$$makeExecutables", "ti.sdo.tools.build"), Global.get(cap, "makeExecutables"));
                po.addFxn("makeLibraries", (Proto.Fxn)om.findStrict("ti.sdo.tools.build.PackageBuildHelp$$makeLibraries", "ti.sdo.tools.build"), Global.get(cap, "makeLibraries"));
        // struct PackageBuildHelp.LibAttrs
        po = (Proto.Obj)om.findStrict("ti.sdo.tools.build.PackageBuildHelp$$LibAttrs", "ti.sdo.tools.build");
        po.init("ti.sdo.tools.build.PackageBuildHelp.LibAttrs", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("name", $$T_Str, $$UNDEF, "w");
                po.addFld("sources", new Proto.Arr($$T_Str, false), $$DEFAULT, "w");
                po.addFld("icw", new Proto.Arr($$T_Str, false), $$DEFAULT, "w");
                po.addFld("libAttrs", (Proto)om.findStrict("xdc.bld.Library.Attrs", "ti.sdo.tools.build"), $$DEFAULT, "w");
        // struct PackageBuildHelp.ProgAttrs
        po = (Proto.Obj)om.findStrict("ti.sdo.tools.build.PackageBuildHelp$$ProgAttrs", "ti.sdo.tools.build");
        po.init("ti.sdo.tools.build.PackageBuildHelp.ProgAttrs", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("name", $$T_Str, $$UNDEF, "w");
                po.addFld("sources", new Proto.Arr($$T_Str, false), $$DEFAULT, "w");
                po.addFld("platforms", new Proto.Arr($$T_Str, false), $$DEFAULT, "w");
                po.addFld("icw", new Proto.Arr($$T_Str, false), $$DEFAULT, "w");
                po.addFld("isas", new Proto.Arr($$T_Str, false), $$DEFAULT, "w");
                po.addFld("execAttrs", (Proto)om.findStrict("xdc.bld.Executable.Attrs", "ti.sdo.tools.build"), $$DEFAULT, "w");
    }

    void PackageBuildHelp$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sdo.tools.build.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sdo.tools.build"));
        pkgP.bind("$capsule", $$UNDEF);
        pkgV.init2(pkgP, "ti.sdo.tools.build", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sdo.tools.build");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sdo.tools.build.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sdo.tools.build'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void PackageBuildHelp$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sdo.tools.build.PackageBuildHelp", "ti.sdo.tools.build");
        po = (Proto.Obj)om.findStrict("ti.sdo.tools.build.PackageBuildHelp.Module", "ti.sdo.tools.build");
        vo.init2(po, "ti.sdo.tools.build.PackageBuildHelp", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sdo.tools.build.PackageBuildHelp$$capsule", "ti.sdo.tools.build"));
        vo.bind("$package", om.findStrict("ti.sdo.tools.build", "ti.sdo.tools.build"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("LibAttrs", om.findStrict("ti.sdo.tools.build.PackageBuildHelp.LibAttrs", "ti.sdo.tools.build"));
        tdefs.add(om.findStrict("ti.sdo.tools.build.PackageBuildHelp.LibAttrs", "ti.sdo.tools.build"));
        vo.bind("ProgAttrs", om.findStrict("ti.sdo.tools.build.PackageBuildHelp.ProgAttrs", "ti.sdo.tools.build"));
        tdefs.add(om.findStrict("ti.sdo.tools.build.PackageBuildHelp.ProgAttrs", "ti.sdo.tools.build"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sdo.tools.build")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.sdo.tools.build.PackageBuildHelp$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        pkgV.bind("PackageBuildHelp", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("PackageBuildHelp");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sdo.tools.build.PackageBuildHelp", "ti.sdo.tools.build"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sdo.tools.build.PackageBuildHelp")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sdo.tools.build")).add(pkgV);
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
        PackageBuildHelp$$OBJECTS();
        PackageBuildHelp$$CONSTS();
        PackageBuildHelp$$CREATES();
        PackageBuildHelp$$FUNCTIONS();
        PackageBuildHelp$$SIZES();
        PackageBuildHelp$$TYPES();
        if (isROV) {
            PackageBuildHelp$$ROV();
        }//isROV
        $$SINGLETONS();
        PackageBuildHelp$$SINGLETONS();
        $$INITIALIZATION();
    }
}
