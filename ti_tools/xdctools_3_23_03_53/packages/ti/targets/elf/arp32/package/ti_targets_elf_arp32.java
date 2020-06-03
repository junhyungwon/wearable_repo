/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-y23
 */
import java.util.*;
import org.mozilla.javascript.*;
import xdc.services.intern.xsr.*;
import xdc.services.spec.Session;

public class ti_targets_elf_arp32
{
    static final String VERS = "@(#) xdc-y23\n";

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
        Global.callFxn("loadPackage", xdcO, "xdc.bld");
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
        Global.callFxn("loadPackage", xdcO, "ti.targets");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.targets.elf.arp32.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.targets.elf.arp32", new Value.Obj("ti.targets.elf.arp32", pkgP));
    }

    void ARP32$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.targets.elf.arp32.ARP32.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.targets.elf.arp32.ARP32", new Value.Obj("ti.targets.elf.arp32.ARP32", po));
        pkgV.bind("ARP32", vo);
        // decls 
        om.bind("ti.targets.elf.arp32.ARP32.Model", om.findStrict("xdc.bld.ITarget.Model", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.DebugGen", om.findStrict("xdc.bld.ITarget.DebugGen", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.Extension", om.findStrict("xdc.bld.ITarget.Extension", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.CompileOptions", om.findStrict("xdc.bld.ITarget.CompileOptions", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.OptionSet", om.findStrict("xdc.bld.ITarget.OptionSet", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.CompileGoal", om.findStrict("xdc.bld.ITarget.CompileGoal", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.LinkGoal", om.findStrict("xdc.bld.ITarget.LinkGoal", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.ArchiveGoal", om.findStrict("xdc.bld.ITarget.ArchiveGoal", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.CommandSet", om.findStrict("xdc.bld.ITarget.CommandSet", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.TypeInfo", om.findStrict("xdc.bld.ITarget.TypeInfo", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.StdTypes", om.findStrict("xdc.bld.ITarget.StdTypes", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.Command", om.findStrict("xdc.bld.ITarget2.Command", "ti.targets.elf.arp32"));
        om.bind("ti.targets.elf.arp32.ARP32.Options", om.findStrict("xdc.bld.ITarget2.Options", "ti.targets.elf.arp32"));
    }

    void ARP32$$CONSTS()
    {
        // module ARP32
    }

    void ARP32$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void ARP32$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void ARP32$$SIZES()
    {
    }

    void ARP32$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/targets/elf/arp32/ARP32.xs");
        om.bind("ti.targets.elf.arp32.ARP32$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.targets.elf.arp32.ARP32.Module", "ti.targets.elf.arp32");
        po.init("ti.targets.elf.arp32.ARP32.Module", om.findStrict("ti.targets.ITarget.Module", "ti.targets.elf.arp32"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("name", $$T_Str, "ARP32", "rh");
        po.addFld("suffix", $$T_Str, "earp32", "rh");
        po.addFld("isa", $$T_Str, "arp32", "rh");
        po.addFld("model", (Proto)om.findStrict("xdc.bld.ITarget.Model", "ti.targets.elf.arp32"), Global.newObject("endian", "little", "shortEnums", true), "rh");
        po.addFld("rts", $$T_Str, "ti.targets.arp32.rts", "rh");
        po.addFld("platform", $$T_Str, "ti.platforms.simARP32", "wh");
        po.addFld("ar", (Proto)om.findStrict("xdc.bld.ITarget2.Command", "ti.targets.elf.arp32"), Global.newObject("cmd", "ar-arp32", "opts", "rq"), "rh");
        po.addFld("cc", (Proto)om.findStrict("xdc.bld.ITarget2.Command", "ti.targets.elf.arp32"), Global.newObject("cmd", "cl-arp32 -c", "opts", "--silicon_version=v210"), "rh");
        po.addFld("asm", (Proto)om.findStrict("xdc.bld.ITarget2.Command", "ti.targets.elf.arp32"), Global.newObject("cmd", "cl-arp32 -c", "opts", "--silicon_version=v210"), "rh");
        po.addFld("lnk", (Proto)om.findStrict("xdc.bld.ITarget2.Command", "ti.targets.elf.arp32"), Global.newObject("cmd", "lnk-arp32", "opts", ""), "rh");
        po.addFld("vers", (Proto)om.findStrict("xdc.bld.ITarget2.Command", "ti.targets.elf.arp32"), Global.newObject("cmd", "cl-arp32", "opts", "--compiler_revision"), "rh");
        po.addFld("asmOpts", (Proto)om.findStrict("xdc.bld.ITarget2.Options", "ti.targets.elf.arp32"), Global.newObject("prefix", "-qq", "suffix", ""), "wh");
        po.addFld("ccOpts", (Proto)om.findStrict("xdc.bld.ITarget2.Options", "ti.targets.elf.arp32"), Global.newObject("prefix", "-qq -pdsw225", "suffix", ""), "wh");
        po.addFld("ccConfigOpts", (Proto)om.findStrict("xdc.bld.ITarget2.Options", "ti.targets.elf.arp32"), Global.newObject("prefix", "$(ccOpts.prefix)", "suffix", "$(ccOpts.suffix)"), "wh");
        po.addFld("lnkOpts", (Proto)om.findStrict("xdc.bld.ITarget2.Options", "ti.targets.elf.arp32"), Global.newObject("prefix", "-w -q -u _c_int00", "suffix", "-c -m $(XDCCFGDIR)/$@.map -l $(rootDir)/lib/rtsarp32_v200.lib"), "wh");
        po.addFld("includeOpts", $$T_Str, "-I$(rootDir)/include", "wh");
        po.addFld("sectMap", new Proto.Map($$T_Str), Global.newArray(new Object[]{Global.newArray(new Object[]{".text", "code"}), Global.newArray(new Object[]{".stack", "stack"}), Global.newArray(new Object[]{".bss", "data"}), Global.newArray(new Object[]{".cinit", "data"}), Global.newArray(new Object[]{".init_array", "data"}), Global.newArray(new Object[]{".const", "data"}), Global.newArray(new Object[]{".data", "data"}), Global.newArray(new Object[]{".switch", "data"}), Global.newArray(new Object[]{".sysmem", "data"}), Global.newArray(new Object[]{".far", "data"}), Global.newArray(new Object[]{".args", "data"}), Global.newArray(new Object[]{".cio", "data"}), Global.newArray(new Object[]{".fardata", "data"}), Global.newArray(new Object[]{".rodata", "data"})}), "rh");
        po.addFld("bitsPerChar", Proto.Elm.newCNum("(xdc_Int)"), 8L, "rh");
        po.addFld("profiles", new Proto.Map((Proto)om.findStrict("xdc.bld.ITarget.OptionSet", "ti.targets.elf.arp32")), Global.newArray(new Object[]{Global.newArray(new Object[]{"debug", Global.newObject("compileOpts", Global.newObject("copts", "--symdebug:dwarf", "defs", "-D_DEBUG_=1"))}), Global.newArray(new Object[]{"release", Global.newObject("compileOpts", Global.newObject("copts", "-O2"))}), Global.newArray(new Object[]{"profile", Global.newObject("compileOpts", Global.newObject("copts", "--symdebug:dwarf"))}), Global.newArray(new Object[]{"coverage", Global.newObject("compileOpts", Global.newObject("copts", "--symdebug:dwarf"))})}), "wh");
        po.addFld("stdTypes", (Proto)om.findStrict("xdc.bld.ITarget.StdTypes", "ti.targets.elf.arp32"), Global.newObject("t_IArg", Global.newObject("size", 4L, "align", 4L), "t_Char", Global.newObject("size", 1L, "align", 1L), "t_Double", Global.newObject("size", 8L, "align", 4L), "t_Float", Global.newObject("size", 4L, "align", 4L), "t_Fxn", Global.newObject("size", 4L, "align", 4L), "t_Int", Global.newObject("size", 4L, "align", 4L), "t_Int8", Global.newObject("size", 1L, "align", 1L), "t_Int16", Global.newObject("size", 2L, "align", 2L), "t_Int32", Global.newObject("size", 4L, "align", 4L), "t_Int64", Global.newObject("size", 8L, "align", 4L), "t_Long", Global.newObject("size", 4L, "align", 4L), "t_LDouble", Global.newObject("size", 8L, "align", 4L), "t_LLong", Global.newObject("size", 8L, "align", 4L), "t_Ptr", Global.newObject("size", 4L, "align", 4L), "t_Short", Global.newObject("size", 2L, "align", 2L), "t_Size", Global.newObject("size", 4L, "align", 4L)), "rh");
        po.addFld("stdInclude", $$T_Str, "ti/targets/elf/arp32/std.h", "wh");
        po.addFld("binaryParser", $$T_Str, "ti.targets.omf.elf.Elf32", "wh");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.targets.elf.arp32.ARP32$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.targets.elf.arp32.ARP32$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.targets.elf.arp32.ARP32$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "archive");
                if (fxn != null) po.addFxn("archive", (Proto.Fxn)om.findStrict("xdc.bld.ITarget$$archive", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "compile");
                if (fxn != null) po.addFxn("compile", (Proto.Fxn)om.findStrict("xdc.bld.ITarget$$compile", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "scompile");
                if (fxn != null) po.addFxn("scompile", (Proto.Fxn)om.findStrict("xdc.bld.ITarget$$scompile", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "link");
                if (fxn != null) po.addFxn("link", (Proto.Fxn)om.findStrict("xdc.bld.ITarget$$link", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "getVersion");
                if (fxn != null) po.addFxn("getVersion", (Proto.Fxn)om.findStrict("xdc.bld.ITarget$$getVersion", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "getISAChain");
                if (fxn != null) po.addFxn("getISAChain", (Proto.Fxn)om.findStrict("xdc.bld.ITarget$$getISAChain", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "findSuffix");
                if (fxn != null) po.addFxn("findSuffix", (Proto.Fxn)om.findStrict("xdc.bld.ITarget$$findSuffix", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "selectSuffix");
                if (fxn != null) po.addFxn("selectSuffix", (Proto.Fxn)om.findStrict("xdc.bld.ITarget$$selectSuffix", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "genConstCustom");
                if (fxn != null) po.addFxn("genConstCustom", (Proto.Fxn)om.findStrict("xdc.bld.ITarget2$$genConstCustom", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "genVisibleData");
                if (fxn != null) po.addFxn("genVisibleData", (Proto.Fxn)om.findStrict("xdc.bld.ITarget2$$genVisibleData", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "genVisibleFxns");
                if (fxn != null) po.addFxn("genVisibleFxns", (Proto.Fxn)om.findStrict("xdc.bld.ITarget2$$genVisibleFxns", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "genVisibleLibFxns");
                if (fxn != null) po.addFxn("genVisibleLibFxns", (Proto.Fxn)om.findStrict("xdc.bld.ITarget2$$genVisibleLibFxns", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "asmName");
                if (fxn != null) po.addFxn("asmName", (Proto.Fxn)om.findStrict("ti.targets.ITarget$$asmName", "ti.targets.elf.arp32"), fxn);
    }

    void ARP32$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.targets.elf.arp32.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.targets.elf.arp32"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/targets/elf/arp32/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.targets.elf.arp32"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.targets.elf.arp32"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.targets.elf.arp32", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.targets.elf.arp32");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.targets.elf.arp32.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("xdc.bld", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.targets.elf.arp32'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void ARP32$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.targets.elf.arp32.ARP32", "ti.targets.elf.arp32");
        po = (Proto.Obj)om.findStrict("ti.targets.elf.arp32.ARP32.Module", "ti.targets.elf.arp32");
        vo.init2(po, "ti.targets.elf.arp32.ARP32", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.targets.elf.arp32.ARP32$$capsule", "ti.targets.elf.arp32"));
        vo.bind("$package", om.findStrict("ti.targets.elf.arp32", "ti.targets.elf.arp32"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("Model", om.findStrict("xdc.bld.ITarget.Model", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.Model", "ti.targets.elf.arp32"));
        vo.bind("DebugGen", om.findStrict("xdc.bld.ITarget.DebugGen", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.DebugGen", "ti.targets.elf.arp32"));
        vo.bind("Extension", om.findStrict("xdc.bld.ITarget.Extension", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.Extension", "ti.targets.elf.arp32"));
        vo.bind("CompileOptions", om.findStrict("xdc.bld.ITarget.CompileOptions", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.CompileOptions", "ti.targets.elf.arp32"));
        vo.bind("OptionSet", om.findStrict("xdc.bld.ITarget.OptionSet", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.OptionSet", "ti.targets.elf.arp32"));
        vo.bind("CompileGoal", om.findStrict("xdc.bld.ITarget.CompileGoal", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.CompileGoal", "ti.targets.elf.arp32"));
        vo.bind("LinkGoal", om.findStrict("xdc.bld.ITarget.LinkGoal", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.LinkGoal", "ti.targets.elf.arp32"));
        vo.bind("ArchiveGoal", om.findStrict("xdc.bld.ITarget.ArchiveGoal", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.ArchiveGoal", "ti.targets.elf.arp32"));
        vo.bind("CommandSet", om.findStrict("xdc.bld.ITarget.CommandSet", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.CommandSet", "ti.targets.elf.arp32"));
        vo.bind("StringArray", om.findStrict("xdc.bld.ITarget.StringArray", "ti.targets.elf.arp32"));
        vo.bind("TypeInfo", om.findStrict("xdc.bld.ITarget.TypeInfo", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.TypeInfo", "ti.targets.elf.arp32"));
        vo.bind("StdTypes", om.findStrict("xdc.bld.ITarget.StdTypes", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget.StdTypes", "ti.targets.elf.arp32"));
        vo.bind("Command", om.findStrict("xdc.bld.ITarget2.Command", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget2.Command", "ti.targets.elf.arp32"));
        vo.bind("Options", om.findStrict("xdc.bld.ITarget2.Options", "ti.targets.elf.arp32"));
        tdefs.add(om.findStrict("xdc.bld.ITarget2.Options", "ti.targets.elf.arp32"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.targets");
        inherits.add("xdc.bld");
        inherits.add("xdc.bld");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.targets.elf.arp32")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.targets.elf.arp32.ARP32$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", "xdc/bld/stddefs.xdt");
        atmap.seal("length");
        pkgV.bind("ARP32", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ARP32");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.targets.elf.arp32.ARP32", "ti.targets.elf.arp32"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.targets.elf.arp32.ARP32")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.targets.elf.arp32")).add(pkgV);
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
        ARP32$$OBJECTS();
        ARP32$$CONSTS();
        ARP32$$CREATES();
        ARP32$$FUNCTIONS();
        ARP32$$SIZES();
        ARP32$$TYPES();
        if (isROV) {
            ARP32$$ROV();
        }//isROV
        $$SINGLETONS();
        ARP32$$SINGLETONS();
        $$INITIALIZATION();
    }
}
