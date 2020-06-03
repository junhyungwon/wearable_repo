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

public class ti_catalog_msp430_peripherals_clock
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
        Global.callFxn("loadPackage", xdcO, "ti.catalog.msp430.peripherals.special_function");
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
        Global.callFxn("loadPackage", xdcO, "xdc.platform");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.catalog.msp430.peripherals.clock", new Value.Obj("ti.catalog.msp430.peripherals.clock", pkgP));
    }

    void IClock$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.IClock.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.msp430.peripherals.clock.IClock", new Value.Obj("ti.catalog.msp430.peripherals.clock.IClock", po));
        pkgV.bind("IClock", vo);
        // decls 
        spo = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.IClock$$ForceSetDefaultRegister_t", new Proto.Obj());
        om.bind("ti.catalog.msp430.peripherals.clock.IClock.ForceSetDefaultRegister_t", new Proto.Str(spo, true));
        spo = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.IClock$$AvailableClockVariations_t", new Proto.Obj());
        om.bind("ti.catalog.msp430.peripherals.clock.IClock.AvailableClockVariations_t", new Proto.Str(spo, true));
        // insts 
        Object insP = om.bind("ti.catalog.msp430.peripherals.clock.IClock.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.IClock$$Object", new Proto.Obj());
        Object objP = om.bind("ti.catalog.msp430.peripherals.clock.IClock.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.IClock$$Params", new Proto.Obj());
        om.bind("ti.catalog.msp430.peripherals.clock.IClock.Params", new Proto.Str(po, true));
    }

    void BCSplus$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.msp430.peripherals.clock.BCSplus", new Value.Obj("ti.catalog.msp430.peripherals.clock.BCSplus", po));
        pkgV.bind("BCSplus", vo);
        // decls 
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.ForceSetDefaultRegister_t", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.AvailableClockVariations_t", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.AvailableClockVariations_t", "ti.catalog.msp430.peripherals.clock"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XTS_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.XTS_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.SELS_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.SELS_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF_t"));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF_t", new Proto.Enm("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF_t"));
        spo = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$DCOCTL_t", new Proto.Obj());
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCOCTL_t", new Proto.Str(spo, true));
        spo = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$BCSCTL1_t", new Proto.Obj());
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL1_t", new Proto.Str(spo, true));
        spo = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$BCSCTL2_t", new Proto.Obj());
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL2_t", new Proto.Str(spo, true));
        spo = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$BCSCTL3_t", new Proto.Obj());
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL3_t", new Proto.Str(spo, true));
        // insts 
        Object insP = om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$Object", new Proto.Obj());
        Object objP = om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$Params", new Proto.Obj());
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.Params", new Proto.Str(po, true));
    }

    void UCS$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.UCS.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.msp430.peripherals.clock.UCS", new Value.Obj("ti.catalog.msp430.peripherals.clock.UCS", po));
        pkgV.bind("UCS", vo);
        // decls 
        om.bind("ti.catalog.msp430.peripherals.clock.UCS.ForceSetDefaultRegister_t", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock"));
        om.bind("ti.catalog.msp430.peripherals.clock.UCS.AvailableClockVariations_t", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.AvailableClockVariations_t", "ti.catalog.msp430.peripherals.clock"));
        // insts 
        Object insP = om.bind("ti.catalog.msp430.peripherals.clock.UCS.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.UCS$$Object", new Proto.Obj());
        Object objP = om.bind("ti.catalog.msp430.peripherals.clock.UCS.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.msp430.peripherals.clock.UCS$$Params", new Proto.Obj());
        om.bind("ti.catalog.msp430.peripherals.clock.UCS.Params", new Proto.Str(po, true));
    }

    void IClock$$CONSTS()
    {
        // interface IClock
    }

    void BCSplus$$CONSTS()
    {
        // module BCSplus
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.MOD0", xdc.services.intern.xsr.Enum.intValue(0x01L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.MOD1", xdc.services.intern.xsr.Enum.intValue(0x02L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.MOD2", xdc.services.intern.xsr.Enum.intValue(0x04L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.MOD3", xdc.services.intern.xsr.Enum.intValue(0x08L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.MOD4", xdc.services.intern.xsr.Enum.intValue(0x10L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DCO0_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DCO0", xdc.services.intern.xsr.Enum.intValue(0x20L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DCO1_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DCO1", xdc.services.intern.xsr.Enum.intValue(0x40L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DCO2", xdc.services.intern.xsr.Enum.intValue(0x80L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF", xdc.services.intern.xsr.Enum.intValue(0x80L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XTS_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XTS_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XTS_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XTS", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XTS_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XTS", xdc.services.intern.xsr.Enum.intValue(0x40L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0", xdc.services.intern.xsr.Enum.intValue(0x01L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1", xdc.services.intern.xsr.Enum.intValue(0x02L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2", xdc.services.intern.xsr.Enum.intValue(0x04L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3", xdc.services.intern.xsr.Enum.intValue(0x08L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_0", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.SELM_0", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_2", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.SELM_2", xdc.services.intern.xsr.Enum.intValue(0x80L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_3", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.SELM_3", xdc.services.intern.xsr.Enum.intValue(0xC0L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_0", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_0", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_1", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_1", xdc.services.intern.xsr.Enum.intValue(0x10L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_2", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_2", xdc.services.intern.xsr.Enum.intValue(0x20L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_3", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_3", xdc.services.intern.xsr.Enum.intValue(0x30L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_0", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_0", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_1", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_1", xdc.services.intern.xsr.Enum.intValue(0x10L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_2", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_2", xdc.services.intern.xsr.Enum.intValue(0x20L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_3", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_3", xdc.services.intern.xsr.Enum.intValue(0x30L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_0", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_0", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_1", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_1", xdc.services.intern.xsr.Enum.intValue(0x02L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_2", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_2", xdc.services.intern.xsr.Enum.intValue(0x04L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_3", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_3", xdc.services.intern.xsr.Enum.intValue(0x06L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.SELS_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELS_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.SELS_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.SELS", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELS_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.SELS", xdc.services.intern.xsr.Enum.intValue(0x08L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.DCOR", xdc.services.intern.xsr.Enum.intValue(0x01L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_0", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_0", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_1", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_1", xdc.services.intern.xsr.Enum.intValue(0x40L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_2", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_2", xdc.services.intern.xsr.Enum.intValue(0x80L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_3", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_3", xdc.services.intern.xsr.Enum.intValue(0xC0L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_0", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_0", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_1", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_1", xdc.services.intern.xsr.Enum.intValue(0x10L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_2", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_2", xdc.services.intern.xsr.Enum.intValue(0x20L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_3", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_3", xdc.services.intern.xsr.Enum.intValue(0x30L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_0", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_0", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_1", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_1", xdc.services.intern.xsr.Enum.intValue(0x04L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_2", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_2", xdc.services.intern.xsr.Enum.intValue(0x08L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_3", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_3", xdc.services.intern.xsr.Enum.intValue(0x0CL)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF", xdc.services.intern.xsr.Enum.intValue(0x02L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF_OFF", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF_t", "ti.catalog.msp430.peripherals.clock"), "ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF", xdc.services.intern.xsr.Enum.intValue(0x01L)+0));
    }

    void UCS$$CONSTS()
    {
        // module UCS
    }

    void IClock$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void BCSplus$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$create", new Proto.Fxn(om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Module", "ti.catalog.msp430.peripherals.clock"), om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Instance", "ti.catalog.msp430.peripherals.clock"), 1, 0, false));
                fxn.addArg(0, "__params", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Params", "ti.catalog.msp430.peripherals.clock"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$msp430$peripherals$clock$BCSplus$$create = function( __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.msp430.peripherals.clock.BCSplus'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.msp430.peripherals.clock.BCSplus.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.msp430.peripherals.clock']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.name = __mod.PARAMS.name;\n");
            sb.append("__inst.owner = __mod.PARAMS.owner;\n");
            sb.append("__inst.baseAddr = __mod.PARAMS.baseAddr;\n");
            sb.append("__inst.ACLKHz = __mod.PARAMS.ACLKHz;\n");
            sb.append("__inst.MCLKHz = __mod.PARAMS.MCLKHz;\n");
            sb.append("__inst.SMCLKHz = __mod.PARAMS.SMCLKHz;\n");
            sb.append("__inst.preCalibratedValuesItems = __mod.PARAMS.preCalibratedValuesItems;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$construct", new Proto.Fxn(om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Module", "ti.catalog.msp430.peripherals.clock"), null, 2, 0, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus$$Object", "ti.catalog.msp430.peripherals.clock"), null);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Params", "ti.catalog.msp430.peripherals.clock"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$msp430$peripherals$clock$BCSplus$$construct = function( __obj, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.msp430.peripherals.clock.BCSplus'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.name = __mod.PARAMS.name;\n");
            sb.append("__inst.owner = __mod.PARAMS.owner;\n");
            sb.append("__inst.baseAddr = __mod.PARAMS.baseAddr;\n");
            sb.append("__inst.ACLKHz = __mod.PARAMS.ACLKHz;\n");
            sb.append("__inst.MCLKHz = __mod.PARAMS.MCLKHz;\n");
            sb.append("__inst.SMCLKHz = __mod.PARAMS.SMCLKHz;\n");
            sb.append("__inst.preCalibratedValuesItems = __mod.PARAMS.preCalibratedValuesItems;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void UCS$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.msp430.peripherals.clock.UCS$$create", new Proto.Fxn(om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Module", "ti.catalog.msp430.peripherals.clock"), om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Instance", "ti.catalog.msp430.peripherals.clock"), 1, 0, false));
                fxn.addArg(0, "__params", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Params", "ti.catalog.msp430.peripherals.clock"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$msp430$peripherals$clock$UCS$$create = function( __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.msp430.peripherals.clock.UCS'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.msp430.peripherals.clock.UCS.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.msp430.peripherals.clock']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.name = __mod.PARAMS.name;\n");
            sb.append("__inst.owner = __mod.PARAMS.owner;\n");
            sb.append("__inst.baseAddr = __mod.PARAMS.baseAddr;\n");
            sb.append("__inst.ACLKHz = __mod.PARAMS.ACLKHz;\n");
            sb.append("__inst.MCLKHz = __mod.PARAMS.MCLKHz;\n");
            sb.append("__inst.SMCLKHz = __mod.PARAMS.SMCLKHz;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.msp430.peripherals.clock.UCS$$construct", new Proto.Fxn(om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Module", "ti.catalog.msp430.peripherals.clock"), null, 2, 0, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS$$Object", "ti.catalog.msp430.peripherals.clock"), null);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Params", "ti.catalog.msp430.peripherals.clock"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$msp430$peripherals$clock$UCS$$construct = function( __obj, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.msp430.peripherals.clock.UCS'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.name = __mod.PARAMS.name;\n");
            sb.append("__inst.owner = __mod.PARAMS.owner;\n");
            sb.append("__inst.baseAddr = __mod.PARAMS.baseAddr;\n");
            sb.append("__inst.ACLKHz = __mod.PARAMS.ACLKHz;\n");
            sb.append("__inst.MCLKHz = __mod.PARAMS.MCLKHz;\n");
            sb.append("__inst.SMCLKHz = __mod.PARAMS.SMCLKHz;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void IClock$$FUNCTIONS()
    {
        Proto.Fxn fxn;

        // fxn IClock.computeDCOCLKHz
        fxn = (Proto.Fxn)om.bind("ti.catalog.msp430.peripherals.clock.IClock$$computeDCOCLKHz", new Proto.Fxn(om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Instance", "ti.catalog.msp430.peripherals.clock"), null, 1, 1, false));
                fxn.addArg(0, "DCOCLKHz", Proto.Elm.newCNum("(xdc_Float)"), $$UNDEF);
    }

    void BCSplus$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void UCS$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void IClock$$SIZES()
    {
    }

    void BCSplus$$SIZES()
    {
    }

    void UCS$$SIZES()
    {
    }

    void IClock$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Module", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.IClock.Module", om.findStrict("xdc.platform.IPeripheral.Module", "ti.catalog.msp430.peripherals.clock"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Instance", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.IClock.Instance", om.findStrict("xdc.platform.IPeripheral.Instance", "ti.catalog.msp430.peripherals.clock"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "wh");
        po.addFld("hasHFXT1", $$T_Bool, false, "wh");
        po.addFld("hasXT2", $$T_Bool, false, "wh");
        po.addFld("hasRosc", $$T_Bool, false, "wh");
        po.addFld("hasVLO", $$T_Bool, false, "wh");
        po.addFld("hasAllCal", $$T_Bool, false, "wh");
        po.addFld("maxCpuFrequency", Proto.Elm.newCNum("(xdc_Float)"), 0L, "wh");
        po.addFld("DCOCLKHz", Proto.Elm.newCNum("(xdc_Float)"), 1000000L, "rh");
        po.addFld("ACLKHz", Proto.Elm.newCNum("(xdc_Float)"), $$UNDEF, "rh");
        po.addFld("MCLKHz", Proto.Elm.newCNum("(xdc_Float)"), $$UNDEF, "rh");
        po.addFld("SMCLKHz", Proto.Elm.newCNum("(xdc_Float)"), $$UNDEF, "rh");
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock$$Params", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.IClock.Params", om.findStrict("xdc.platform.IPeripheral$$Params", "ti.catalog.msp430.peripherals.clock"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "wh");
        po.addFld("hasHFXT1", $$T_Bool, false, "wh");
        po.addFld("hasXT2", $$T_Bool, false, "wh");
        po.addFld("hasRosc", $$T_Bool, false, "wh");
        po.addFld("hasVLO", $$T_Bool, false, "wh");
        po.addFld("hasAllCal", $$T_Bool, false, "wh");
        po.addFld("maxCpuFrequency", Proto.Elm.newCNum("(xdc_Float)"), 0L, "wh");
        po.addFld("DCOCLKHz", Proto.Elm.newCNum("(xdc_Float)"), 1000000L, "rh");
        po.addFld("ACLKHz", Proto.Elm.newCNum("(xdc_Float)"), $$UNDEF, "rh");
        po.addFld("MCLKHz", Proto.Elm.newCNum("(xdc_Float)"), $$UNDEF, "rh");
        po.addFld("SMCLKHz", Proto.Elm.newCNum("(xdc_Float)"), $$UNDEF, "rh");
        // struct IClock.ForceSetDefaultRegister_t
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock$$ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.IClock.ForceSetDefaultRegister_t", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("register", $$T_Str, $$UNDEF, "w");
                po.addFld("regForceSet", $$T_Bool, $$UNDEF, "w");
        // struct IClock.AvailableClockVariations_t
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock$$AvailableClockVariations_t", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.IClock.AvailableClockVariations_t", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("clockType", $$T_Str, $$UNDEF, "w");
                po.addFld("hasClock", $$T_Bool, $$UNDEF, "w");
    }

    void BCSplus$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/msp430/peripherals/clock/BCSplus.xs");
        om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Module", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.BCSplus.Module", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Module", "ti.catalog.msp430.peripherals.clock"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus$$create", "ti.catalog.msp430.peripherals.clock"), Global.get("ti$catalog$msp430$peripherals$clock$BCSplus$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus$$construct", "ti.catalog.msp430.peripherals.clock"), Global.get("ti$catalog$msp430$peripherals$clock$BCSplus$$construct"));
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.msp430.peripherals.clock.BCSplus$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "addPeripheralsMap");
                if (fxn != null) po.addFxn("addPeripheralsMap", (Proto.Fxn)om.findStrict("xdc.platform.IPeripheral$$addPeripheralsMap", "ti.catalog.msp430.peripherals.clock"), fxn);
                fxn = Global.get(cap, "getAll");
                if (fxn != null) po.addFxn("getAll", (Proto.Fxn)om.findStrict("xdc.platform.IPeripheral$$getAll", "ti.catalog.msp430.peripherals.clock"), fxn);
                fxn = Global.get(cap, "getRegisters");
                if (fxn != null) po.addFxn("getRegisters", (Proto.Fxn)om.findStrict("xdc.platform.IPeripheral$$getRegisters", "ti.catalog.msp430.peripherals.clock"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Instance", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.BCSplus.Instance", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Instance", "ti.catalog.msp430.peripherals.clock"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("DCOCTL", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCOCTL_t", "ti.catalog.msp430.peripherals.clock"), Global.newObject("MOD0", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_OFF"), "MOD1", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_OFF"), "MOD2", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_OFF"), "MOD3", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_OFF"), "MOD4", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_OFF"), "DCO0", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0"), "DCO1", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1"), "DCO2", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_OFF")), "wh");
        po.addFld("BCSCTL1", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL1_t", "ti.catalog.msp430.peripherals.clock"), Global.newObject("XT2OFF", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF"), "XTS", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.XTS_OFF"), "DIVA", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_0"), "RSEL0", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0"), "RSEL1", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1"), "RSEL2", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2"), "RSEL3", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_OFF")), "wh");
        po.addFld("BCSCTL2", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL2_t", "ti.catalog.msp430.peripherals.clock"), Global.newObject("SELM", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_0"), "DIVM", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_0"), "SELS", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.SELS_OFF"), "DIVS", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_0"), "DCOR", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_OFF")), "wh");
        po.addFld("BCSCTL3", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL3_t", "ti.catalog.msp430.peripherals.clock"), Global.newObject("XT2S", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_0"), "LFXT1S", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_0"), "XCAP", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_1")), "wh");
        po.addFld("forceSetDefaultRegister", new Proto.Arr((Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock"), false), Global.newArray(new Object[]{Global.newObject("register", "DCOCTL", "regForceSet", false), Global.newObject("register", "BCSCTL1", "regForceSet", false), Global.newObject("register", "BCSCTL2", "regForceSet", false), Global.newObject("register", "BCSCTL3", "regForceSet", false)}), "rh");
        po.addFld("preCalibratedValues", Proto.Elm.newCNum("(xdc_UInt)"), 0L, "wh");
        po.addFld("preCalibratedValuesItems", new Proto.Arr($$T_Str, true), $$DEFAULT, "wh");
        po.addFld("VLOCLKHz", Proto.Elm.newCNum("(xdc_Float)"), 12000L, "wh");
        po.addFld("WATCHCRYSTALCLKHz", Proto.Elm.newCNum("(xdc_Float)"), 32768L, "wh");
        po.addFld("LFXT1CLKHz", Proto.Elm.newCNum("(xdc_Float)"), 0L, "wh");
        po.addFld("XT2CLKHz", Proto.Elm.newCNum("(xdc_Float)"), 0L, "wh");
        po.addFld("enableXT2", $$T_Bool, false, "wh");
                fxn = Global.get(cap, "computeDCOCLKHz");
                if (fxn != null) po.addFxn("computeDCOCLKHz", (Proto.Fxn)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock$$computeDCOCLKHz", "ti.catalog.msp430.peripherals.clock"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus$$Params", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.BCSplus.Params", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock$$Params", "ti.catalog.msp430.peripherals.clock"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("DCOCTL", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCOCTL_t", "ti.catalog.msp430.peripherals.clock"), Global.newObject("MOD0", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_OFF"), "MOD1", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_OFF"), "MOD2", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_OFF"), "MOD3", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_OFF"), "MOD4", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_OFF"), "DCO0", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0"), "DCO1", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1"), "DCO2", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_OFF")), "wh");
        po.addFld("BCSCTL1", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL1_t", "ti.catalog.msp430.peripherals.clock"), Global.newObject("XT2OFF", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF"), "XTS", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.XTS_OFF"), "DIVA", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_0"), "RSEL0", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0"), "RSEL1", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1"), "RSEL2", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2"), "RSEL3", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_OFF")), "wh");
        po.addFld("BCSCTL2", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL2_t", "ti.catalog.msp430.peripherals.clock"), Global.newObject("SELM", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_0"), "DIVM", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_0"), "SELS", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.SELS_OFF"), "DIVS", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_0"), "DCOR", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_OFF")), "wh");
        po.addFld("BCSCTL3", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL3_t", "ti.catalog.msp430.peripherals.clock"), Global.newObject("XT2S", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_0"), "LFXT1S", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_0"), "XCAP", om.find("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_1")), "wh");
        po.addFld("forceSetDefaultRegister", new Proto.Arr((Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock"), false), Global.newArray(new Object[]{Global.newObject("register", "DCOCTL", "regForceSet", false), Global.newObject("register", "BCSCTL1", "regForceSet", false), Global.newObject("register", "BCSCTL2", "regForceSet", false), Global.newObject("register", "BCSCTL3", "regForceSet", false)}), "rh");
        po.addFld("preCalibratedValues", Proto.Elm.newCNum("(xdc_UInt)"), 0L, "wh");
        po.addFld("preCalibratedValuesItems", new Proto.Arr($$T_Str, true), $$DEFAULT, "wh");
        po.addFld("VLOCLKHz", Proto.Elm.newCNum("(xdc_Float)"), 12000L, "wh");
        po.addFld("WATCHCRYSTALCLKHz", Proto.Elm.newCNum("(xdc_Float)"), 32768L, "wh");
        po.addFld("LFXT1CLKHz", Proto.Elm.newCNum("(xdc_Float)"), 0L, "wh");
        po.addFld("XT2CLKHz", Proto.Elm.newCNum("(xdc_Float)"), 0L, "wh");
        po.addFld("enableXT2", $$T_Bool, false, "wh");
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus$$Object", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.BCSplus.Object", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Instance", "ti.catalog.msp430.peripherals.clock"));
                fxn = Global.get(cap, "computeDCOCLKHz");
                if (fxn != null) po.addFxn("computeDCOCLKHz", (Proto.Fxn)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock$$computeDCOCLKHz", "ti.catalog.msp430.peripherals.clock"), fxn);
        // struct BCSplus.DCOCTL_t
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus$$DCOCTL_t", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.BCSplus.DCOCTL_t", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("MOD0", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("MOD1", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("MOD2", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("MOD3", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("MOD4", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("DCO0", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("DCO1", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("DCO2", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
        // struct BCSplus.BCSCTL1_t
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus$$BCSCTL1_t", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL1_t", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("XT2OFF", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("XTS", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XTS_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("DIVA", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("RSEL0", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("RSEL1", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("RSEL2", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("RSEL3", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
        // struct BCSplus.BCSCTL2_t
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus$$BCSCTL2_t", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL2_t", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("SELM", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("DIVM", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("SELS", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELS_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("DIVS", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("DCOR", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
        // struct BCSplus.BCSCTL3_t
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus$$BCSCTL3_t", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL3_t", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("XT2S", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("LFXT1S", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("XCAP", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("XT2OF", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
                po.addFld("LFXT1OF", (Proto)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF_t", "ti.catalog.msp430.peripherals.clock"), $$UNDEF, "w");
    }

    void UCS$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/msp430/peripherals/clock/UCS.xs");
        om.bind("ti.catalog.msp430.peripherals.clock.UCS$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Module", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.UCS.Module", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Module", "ti.catalog.msp430.peripherals.clock"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS$$create", "ti.catalog.msp430.peripherals.clock"), Global.get("ti$catalog$msp430$peripherals$clock$UCS$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS$$construct", "ti.catalog.msp430.peripherals.clock"), Global.get("ti$catalog$msp430$peripherals$clock$UCS$$construct"));
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.msp430.peripherals.clock.UCS$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.msp430.peripherals.clock.UCS$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.msp430.peripherals.clock.UCS$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.msp430.peripherals.clock.UCS$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "addPeripheralsMap");
                if (fxn != null) po.addFxn("addPeripheralsMap", (Proto.Fxn)om.findStrict("xdc.platform.IPeripheral$$addPeripheralsMap", "ti.catalog.msp430.peripherals.clock"), fxn);
                fxn = Global.get(cap, "getAll");
                if (fxn != null) po.addFxn("getAll", (Proto.Fxn)om.findStrict("xdc.platform.IPeripheral$$getAll", "ti.catalog.msp430.peripherals.clock"), fxn);
                fxn = Global.get(cap, "getRegisters");
                if (fxn != null) po.addFxn("getRegisters", (Proto.Fxn)om.findStrict("xdc.platform.IPeripheral$$getRegisters", "ti.catalog.msp430.peripherals.clock"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Instance", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.UCS.Instance", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Instance", "ti.catalog.msp430.peripherals.clock"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                fxn = Global.get(cap, "computeDCOCLKHz");
                if (fxn != null) po.addFxn("computeDCOCLKHz", (Proto.Fxn)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock$$computeDCOCLKHz", "ti.catalog.msp430.peripherals.clock"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS$$Params", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.UCS.Params", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock$$Params", "ti.catalog.msp430.peripherals.clock"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS$$Object", "ti.catalog.msp430.peripherals.clock");
        po.init("ti.catalog.msp430.peripherals.clock.UCS.Object", om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Instance", "ti.catalog.msp430.peripherals.clock"));
                fxn = Global.get(cap, "computeDCOCLKHz");
                if (fxn != null) po.addFxn("computeDCOCLKHz", (Proto.Fxn)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock$$computeDCOCLKHz", "ti.catalog.msp430.peripherals.clock"), fxn);
    }

    void IClock$$ROV()
    {
    }

    void BCSplus$$ROV()
    {
    }

    void UCS$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.catalog.msp430.peripherals.clock.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.catalog.msp430.peripherals.clock"));
        pkgP.bind("$capsule", $$UNDEF);
        pkgV.init2(pkgP, "ti.catalog.msp430.peripherals.clock", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.catalog.msp430.peripherals.clock");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.catalog.msp430.peripherals.clock.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("ti.catalog.msp430.peripherals.special_function", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.catalog.msp430.peripherals.clock'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void IClock$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock", "ti.catalog.msp430.peripherals.clock");
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Module", "ti.catalog.msp430.peripherals.clock");
        vo.init2(po, "ti.catalog.msp430.peripherals.clock.IClock", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Interface");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Instance", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("Params", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Params", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.Params", "ti.catalog.msp430.peripherals.clock")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.msp430.peripherals.clock", "ti.catalog.msp430.peripherals.clock"));
        tdefs.clear();
        proxies.clear();
        inherits.clear();
        vo.bind("IPeripheralArray", om.findStrict("xdc.platform.IPeripheral.IPeripheralArray", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("StringArray", om.findStrict("xdc.platform.IPeripheral.StringArray", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("ForceSetDefaultRegister_t", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock"));
        tdefs.add(om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("AvailableClockVariations_t", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.AvailableClockVariations_t", "ti.catalog.msp430.peripherals.clock"));
        tdefs.add(om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.AvailableClockVariations_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$interfaces")).add(vo);
        pkgV.bind("IClock", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("IClock");
        vo.seal(null);
    }

    void BCSplus$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus", "ti.catalog.msp430.peripherals.clock");
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Module", "ti.catalog.msp430.peripherals.clock");
        vo.init2(po, "ti.catalog.msp430.peripherals.clock.BCSplus", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus$$capsule", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("Instance", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Instance", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("Params", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Params", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Params", "ti.catalog.msp430.peripherals.clock")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.msp430.peripherals.clock", "ti.catalog.msp430.peripherals.clock"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("IPeripheralArray", om.findStrict("xdc.platform.IPeripheral.IPeripheralArray", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("StringArray", om.findStrict("xdc.platform.IPeripheral.StringArray", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("ForceSetDefaultRegister_t", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock"));
        tdefs.add(om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("AvailableClockVariations_t", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.AvailableClockVariations_t", "ti.catalog.msp430.peripherals.clock"));
        tdefs.add(om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.AvailableClockVariations_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD0_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD1_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD2_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD3_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD4_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCO0_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCO1_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCO2_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2OFF_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XTS_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XTS_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL0_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL1_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL2_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL3_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("SELM_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVA_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVM_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVS_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("SELS_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELS_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCOR_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2S_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("LFXT1S_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XCAP_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2OF_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("LFXT1OF_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCOCTL_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCOCTL_t", "ti.catalog.msp430.peripherals.clock"));
        tdefs.add(om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCOCTL_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("BCSCTL1_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL1_t", "ti.catalog.msp430.peripherals.clock"));
        tdefs.add(om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL1_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("BCSCTL2_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL2_t", "ti.catalog.msp430.peripherals.clock"));
        tdefs.add(om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL2_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("BCSCTL3_t", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL3_t", "ti.catalog.msp430.peripherals.clock"));
        tdefs.add(om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.BCSCTL3_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD0_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD0", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD0", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD1_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD1", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD1", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD2_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD2", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD2", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD3_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD3", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD3", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD4_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("MOD4", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.MOD4", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCO0_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCO0", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO0", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCO1_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCO1", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO1", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCO2_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCO2", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCO2", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2OFF_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XTS_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XTS_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XTS", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XTS", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL0_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL0", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL0", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL1_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL1", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL1", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL2_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL2", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL2", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL3_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("RSEL3", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.RSEL3", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("SELM_0", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_0", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("SELM_2", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_2", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("SELM_3", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELM_3", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVA_0", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_0", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVA_1", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_1", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVA_2", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_2", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVA_3", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVA_3", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVM_0", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_0", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVM_1", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_1", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVM_2", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_2", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVM_3", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVM_3", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVS_0", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_0", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVS_1", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_1", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVS_2", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_2", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DIVS_3", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DIVS_3", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("SELS_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELS_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("SELS", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.SELS", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCOR_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("DCOR", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.DCOR", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2S_0", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_0", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2S_1", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_1", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2S_2", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_2", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2S_3", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2S_3", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("LFXT1S_0", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_0", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("LFXT1S_1", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_1", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("LFXT1S_2", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_2", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("LFXT1S_3", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1S_3", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XCAP_0", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_0", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XCAP_1", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_1", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XCAP_2", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_2", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XCAP_3", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XCAP_3", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2OF_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("XT2OF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.XT2OF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("LFXT1OF_OFF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF_OFF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("LFXT1OF", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.LFXT1OF", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.msp430.peripherals.clock");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.msp430.peripherals.clock")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.msp430.peripherals.clock.BCSplus$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus.Object", "ti.catalog.msp430.peripherals.clock"));
        pkgV.bind("BCSplus", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("BCSplus");
    }

    void UCS$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS", "ti.catalog.msp430.peripherals.clock");
        po = (Proto.Obj)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Module", "ti.catalog.msp430.peripherals.clock");
        vo.init2(po, "ti.catalog.msp430.peripherals.clock.UCS", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.catalog.msp430.peripherals.clock.UCS$$capsule", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("Instance", om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Instance", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("Params", om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Params", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Params", "ti.catalog.msp430.peripherals.clock")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.msp430.peripherals.clock", "ti.catalog.msp430.peripherals.clock"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("IPeripheralArray", om.findStrict("xdc.platform.IPeripheral.IPeripheralArray", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("StringArray", om.findStrict("xdc.platform.IPeripheral.StringArray", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("ForceSetDefaultRegister_t", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock"));
        tdefs.add(om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.ForceSetDefaultRegister_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("AvailableClockVariations_t", om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.AvailableClockVariations_t", "ti.catalog.msp430.peripherals.clock"));
        tdefs.add(om.findStrict("ti.catalog.msp430.peripherals.clock.IClock.AvailableClockVariations_t", "ti.catalog.msp430.peripherals.clock"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.msp430.peripherals.clock");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.msp430.peripherals.clock")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.msp430.peripherals.clock.UCS$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.msp430.peripherals.clock.UCS.Object", "ti.catalog.msp430.peripherals.clock"));
        pkgV.bind("UCS", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("UCS");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.msp430.peripherals.clock.BCSplus", "ti.catalog.msp430.peripherals.clock"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.msp430.peripherals.clock.UCS", "ti.catalog.msp430.peripherals.clock"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.catalog.msp430.peripherals.clock.IClock")).bless();
        ((Value.Obj)om.getv("ti.catalog.msp430.peripherals.clock.BCSplus")).bless();
        ((Value.Obj)om.getv("ti.catalog.msp430.peripherals.clock.UCS")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.catalog.msp430.peripherals.clock")).add(pkgV);
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
        IClock$$OBJECTS();
        BCSplus$$OBJECTS();
        UCS$$OBJECTS();
        IClock$$CONSTS();
        BCSplus$$CONSTS();
        UCS$$CONSTS();
        IClock$$CREATES();
        BCSplus$$CREATES();
        UCS$$CREATES();
        IClock$$FUNCTIONS();
        BCSplus$$FUNCTIONS();
        UCS$$FUNCTIONS();
        IClock$$SIZES();
        BCSplus$$SIZES();
        UCS$$SIZES();
        IClock$$TYPES();
        BCSplus$$TYPES();
        UCS$$TYPES();
        if (isROV) {
            IClock$$ROV();
            BCSplus$$ROV();
            UCS$$ROV();
        }//isROV
        $$SINGLETONS();
        IClock$$SINGLETONS();
        BCSplus$$SINGLETONS();
        UCS$$SINGLETONS();
        $$INITIALIZATION();
    }
}
