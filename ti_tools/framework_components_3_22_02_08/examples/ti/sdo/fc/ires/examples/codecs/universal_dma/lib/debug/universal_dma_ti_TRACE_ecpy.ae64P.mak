#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.elf.C64P{1,0,7.2,0
#
ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.oe64P.dep
endif

package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.oe64P: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.oe64P: package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.c lib/debug/universal_dma_ti_TRACE_ecpy.ae64P.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64P $< ...
	$(ti.targets.elf.C64P.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package -s oe64P $< -C   -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.oe64P: export C_DIR=
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.oe64P: PATH:=$(ti.targets.elf.C64P.rootDir)/bin/:$(PATH)

package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.se64P: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.se64P: package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.c lib/debug/universal_dma_ti_TRACE_ecpy.ae64P.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64P -n $< ...
	$(ti.targets.elf.C64P.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package -s oe64P $< -C  -n -s --symdebug:none -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.se64P: export C_DIR=
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.se64P: PATH:=$(ti.targets.elf.C64P.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.oe64P.dep
endif

package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.oe64P: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.oe64P: universal_dma_ti.c lib/debug/universal_dma_ti_TRACE_ecpy.ae64P.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64P $< ...
	$(ti.targets.elf.C64P.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -s oe64P $< -C   -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.oe64P: export C_DIR=
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.oe64P: PATH:=$(ti.targets.elf.C64P.rootDir)/bin/:$(PATH)

package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.se64P: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.se64P: universal_dma_ti.c lib/debug/universal_dma_ti_TRACE_ecpy.ae64P.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64P -n $< ...
	$(ti.targets.elf.C64P.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -s oe64P $< -C  -n -s --symdebug:none -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.se64P: export C_DIR=
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.se64P: PATH:=$(ti.targets.elf.C64P.rootDir)/bin/:$(PATH)

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.oe64P.dep
endif

package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.oe64P: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.oe64P: universal_dma_ti_ires.c lib/debug/universal_dma_ti_TRACE_ecpy.ae64P.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64P $< ...
	$(ti.targets.elf.C64P.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -s oe64P $< -C   -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.oe64P: export C_DIR=
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.oe64P: PATH:=$(ti.targets.elf.C64P.rootDir)/bin/:$(PATH)

package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.se64P: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.se64P: universal_dma_ti_ires.c lib/debug/universal_dma_ti_TRACE_ecpy.ae64P.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle64P -n $< ...
	$(ti.targets.elf.C64P.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -s oe64P $< -C  -n -s --symdebug:none -qq -pdsw225 -mv64p --abi=eabi -eo.oe64P -ea.se64P  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK  -Dxdc_target_name__=C64P -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_2_0 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.elf.C64P.rootDir)/include -fs=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy -fr=./package/lib/lib/debug/universal_dma_ti_TRACE_ecpy
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.se64P: export C_DIR=
package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.se64P: PATH:=$(ti.targets.elf.C64P.rootDir)/bin/:$(PATH)

clean,e64P ::
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.oe64P
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.oe64P
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.oe64P
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.se64P
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.se64P
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.se64P

lib/debug/universal_dma_ti_TRACE_ecpy.ae64P: package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.oe64P package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti.oe64P package/lib/lib/debug/universal_dma_ti_TRACE_ecpy/universal_dma_ti_ires.oe64P lib/debug/universal_dma_ti_TRACE_ecpy.ae64P.mak

clean::
	-$(RM) lib/debug/universal_dma_ti_TRACE_ecpy.ae64P.mak
