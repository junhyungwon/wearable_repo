#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.elf.C64T{1,0,7.2,0
#
ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.oe64T.dep
endif

package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.oe64T: | .interfaces
package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.oe64T: package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.c lib/release/edmacodec1_ti.ae64T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64T $< ...
	$(ti.targets.elf.C64T.rootDir)/bin/cl6x -c  -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti/package -fr=./package/lib/lib/release/edmacodec1_ti/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/edmacodec1_ti/package -s oe64T $< -C   -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti/package -fr=./package/lib/lib/release/edmacodec1_ti/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.oe64T: export C_DIR=
package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.oe64T: PATH:=$(ti.targets.elf.C64T.rootDir)/bin/:$(PATH)

package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.se64T: | .interfaces
package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.se64T: package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.c lib/release/edmacodec1_ti.ae64T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64T -n $< ...
	$(ti.targets.elf.C64T.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti/package -fr=./package/lib/lib/release/edmacodec1_ti/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/edmacodec1_ti/package -s oe64T $< -C  -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti/package -fr=./package/lib/lib/release/edmacodec1_ti/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.se64T: export C_DIR=
package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.se64T: PATH:=$(ti.targets.elf.C64T.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.oe64T.dep
endif

package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.oe64T: | .interfaces
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.oe64T: edmacodec1_ti.c lib/release/edmacodec1_ti.ae64T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64T $< ...
	$(ti.targets.elf.C64T.rootDir)/bin/cl6x -c  -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti -fr=./package/lib/lib/release/edmacodec1_ti -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/edmacodec1_ti -s oe64T $< -C   -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti -fr=./package/lib/lib/release/edmacodec1_ti
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.oe64T: export C_DIR=
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.oe64T: PATH:=$(ti.targets.elf.C64T.rootDir)/bin/:$(PATH)

package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.se64T: | .interfaces
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.se64T: edmacodec1_ti.c lib/release/edmacodec1_ti.ae64T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64T -n $< ...
	$(ti.targets.elf.C64T.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti -fr=./package/lib/lib/release/edmacodec1_ti -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/edmacodec1_ti -s oe64T $< -C  -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti -fr=./package/lib/lib/release/edmacodec1_ti
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.se64T: export C_DIR=
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.se64T: PATH:=$(ti.targets.elf.C64T.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.oe64T.dep
endif

package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.oe64T: | .interfaces
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.oe64T: edmacodec1_ti_ires.c lib/release/edmacodec1_ti.ae64T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64T $< ...
	$(ti.targets.elf.C64T.rootDir)/bin/cl6x -c  -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti -fr=./package/lib/lib/release/edmacodec1_ti -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/edmacodec1_ti -s oe64T $< -C   -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti -fr=./package/lib/lib/release/edmacodec1_ti
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.oe64T: export C_DIR=
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.oe64T: PATH:=$(ti.targets.elf.C64T.rootDir)/bin/:$(PATH)

package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.se64T: | .interfaces
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.se64T: edmacodec1_ti_ires.c lib/release/edmacodec1_ti.ae64T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64T -n $< ...
	$(ti.targets.elf.C64T.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti -fr=./package/lib/lib/release/edmacodec1_ti -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/release/edmacodec1_ti -s oe64T $< -C  -n -s --symdebug:none -qq -pdsw225 -pdr -pden -pds=880 -pds=238 -pds=452 -pds=195  -mv=tesla --abi=eabi -eo.oe64T -ea.se64T  -D_DEBUG_=1 -DDBC_ASSERTS=1 -Dxdc_target_name__=C64T -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64T.rootDir)/include -fs=./package/lib/lib/release/edmacodec1_ti -fr=./package/lib/lib/release/edmacodec1_ti
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.se64T: export C_DIR=
package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.se64T: PATH:=$(ti.targets.elf.C64T.rootDir)/bin/:$(PATH)

clean,e64T ::
	-$(RM) package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.oe64T
	-$(RM) package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.oe64T
	-$(RM) package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.oe64T
	-$(RM) package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.se64T
	-$(RM) package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.se64T
	-$(RM) package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.se64T

lib/release/edmacodec1_ti.ae64T: package/lib/lib/release/edmacodec1_ti/package/package_ti.sdo.fc.ires.examples.codecs.edmacodec1.oe64T package/lib/lib/release/edmacodec1_ti/edmacodec1_ti.oe64T package/lib/lib/release/edmacodec1_ti/edmacodec1_ti_ires.oe64T lib/release/edmacodec1_ti.ae64T.mak

clean::
	-$(RM) lib/release/edmacodec1_ti.ae64T.mak
