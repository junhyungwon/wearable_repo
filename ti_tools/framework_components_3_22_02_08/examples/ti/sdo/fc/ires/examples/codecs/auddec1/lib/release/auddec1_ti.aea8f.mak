#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.arm.elf.A8F{1,0,4.9,0
#
ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.oea8f.dep
endif

package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.oea8f: | .interfaces
package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.oea8f: package/package_ti.sdo.fc.ires.examples.codecs.auddec1.c lib/release/auddec1_ti.aea8f.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clea8f $< ...
	$(ti.targets.arm.elf.A8F.rootDir)/bin/cl470 -c  -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti/package -fr=./package/lib/lib/release/auddec1_ti/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/auddec1_ti/package -s oea8f $< -C   -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti/package -fr=./package/lib/lib/release/auddec1_ti/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.oea8f: export C_DIR=
package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.oea8f: PATH:=$(ti.targets.arm.elf.A8F.rootDir)/bin/:$(PATH)

package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.sea8f: | .interfaces
package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.sea8f: package/package_ti.sdo.fc.ires.examples.codecs.auddec1.c lib/release/auddec1_ti.aea8f.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clea8f -n $< ...
	$(ti.targets.arm.elf.A8F.rootDir)/bin/cl470 -c -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti/package -fr=./package/lib/lib/release/auddec1_ti/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/auddec1_ti/package -s oea8f $< -C  -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti/package -fr=./package/lib/lib/release/auddec1_ti/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.sea8f: export C_DIR=
package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.sea8f: PATH:=$(ti.targets.arm.elf.A8F.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/auddec1_ti/auddec1_ti.oea8f.dep
endif

package/lib/lib/release/auddec1_ti/auddec1_ti.oea8f: | .interfaces
package/lib/lib/release/auddec1_ti/auddec1_ti.oea8f: auddec1_ti.c lib/release/auddec1_ti.aea8f.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clea8f $< ...
	$(ti.targets.arm.elf.A8F.rootDir)/bin/cl470 -c  -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti -fr=./package/lib/lib/release/auddec1_ti -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/auddec1_ti -s oea8f $< -C   -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti -fr=./package/lib/lib/release/auddec1_ti
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/auddec1_ti/auddec1_ti.oea8f: export C_DIR=
package/lib/lib/release/auddec1_ti/auddec1_ti.oea8f: PATH:=$(ti.targets.arm.elf.A8F.rootDir)/bin/:$(PATH)

package/lib/lib/release/auddec1_ti/auddec1_ti.sea8f: | .interfaces
package/lib/lib/release/auddec1_ti/auddec1_ti.sea8f: auddec1_ti.c lib/release/auddec1_ti.aea8f.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clea8f -n $< ...
	$(ti.targets.arm.elf.A8F.rootDir)/bin/cl470 -c -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti -fr=./package/lib/lib/release/auddec1_ti -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/auddec1_ti -s oea8f $< -C  -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti -fr=./package/lib/lib/release/auddec1_ti
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/auddec1_ti/auddec1_ti.sea8f: export C_DIR=
package/lib/lib/release/auddec1_ti/auddec1_ti.sea8f: PATH:=$(ti.targets.arm.elf.A8F.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/auddec1_ti/auddec1_ti_ires.oea8f.dep
endif

package/lib/lib/release/auddec1_ti/auddec1_ti_ires.oea8f: | .interfaces
package/lib/lib/release/auddec1_ti/auddec1_ti_ires.oea8f: auddec1_ti_ires.c lib/release/auddec1_ti.aea8f.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clea8f $< ...
	$(ti.targets.arm.elf.A8F.rootDir)/bin/cl470 -c  -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti -fr=./package/lib/lib/release/auddec1_ti -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/auddec1_ti -s oea8f $< -C   -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti -fr=./package/lib/lib/release/auddec1_ti
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/auddec1_ti/auddec1_ti_ires.oea8f: export C_DIR=
package/lib/lib/release/auddec1_ti/auddec1_ti_ires.oea8f: PATH:=$(ti.targets.arm.elf.A8F.rootDir)/bin/:$(PATH)

package/lib/lib/release/auddec1_ti/auddec1_ti_ires.sea8f: | .interfaces
package/lib/lib/release/auddec1_ti/auddec1_ti_ires.sea8f: auddec1_ti_ires.c lib/release/auddec1_ti.aea8f.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clea8f -n $< ...
	$(ti.targets.arm.elf.A8F.rootDir)/bin/cl470 -c -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti -fr=./package/lib/lib/release/auddec1_ti -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/auddec1_ti -s oea8f $< -C  -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -ms --neon --endian=little -mv7A8 --abi=eabi -eo.oea8f -ea.sea8f  -Dxdc_target_name__=A8F -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_9_0 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.A8F.rootDir)/include/rts -I$(ti.targets.arm.elf.A8F.rootDir)/include  -fs=./package/lib/lib/release/auddec1_ti -fr=./package/lib/lib/release/auddec1_ti
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/auddec1_ti/auddec1_ti_ires.sea8f: export C_DIR=
package/lib/lib/release/auddec1_ti/auddec1_ti_ires.sea8f: PATH:=$(ti.targets.arm.elf.A8F.rootDir)/bin/:$(PATH)

clean,ea8f ::
	-$(RM) package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.oea8f
	-$(RM) package/lib/lib/release/auddec1_ti/auddec1_ti.oea8f
	-$(RM) package/lib/lib/release/auddec1_ti/auddec1_ti_ires.oea8f
	-$(RM) package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.sea8f
	-$(RM) package/lib/lib/release/auddec1_ti/auddec1_ti.sea8f
	-$(RM) package/lib/lib/release/auddec1_ti/auddec1_ti_ires.sea8f

lib/release/auddec1_ti.aea8f: package/lib/lib/release/auddec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.auddec1.oea8f package/lib/lib/release/auddec1_ti/auddec1_ti.oea8f package/lib/lib/release/auddec1_ti/auddec1_ti_ires.oea8f lib/release/auddec1_ti.aea8f.mak

clean::
	-$(RM) lib/release/auddec1_ti.aea8f.mak
