#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.C64P_big_endian{1,0,7.0,4
#
ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.o64Pe.dep
endif

package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.o64Pe: | .interfaces
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.o64Pe: src/edma3_drv_basic.c lib/64p/debug/ti.sdo.edma3.drv.a64Pe.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl64Pe $< ...
	$(ti.targets.C64P_big_endian.rootDir)/bin/cl6x -c  -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -s o64Pe $< -C   -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.o64Pe:C_DIR=
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.o64Pe: PATH:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.o64Pe: Path:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)

package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.s64Pe: | .interfaces
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.s64Pe: src/edma3_drv_basic.c lib/64p/debug/ti.sdo.edma3.drv.a64Pe.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl64Pe -n $< ...
	$(ti.targets.C64P_big_endian.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -s o64Pe $< -C  -n -s --symdebug:none -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.s64Pe:C_DIR=
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.s64Pe: PATH:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.s64Pe: Path:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)

ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.o64Pe.dep
endif

package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.o64Pe: | .interfaces
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.o64Pe: src/edma3_drv_init.c lib/64p/debug/ti.sdo.edma3.drv.a64Pe.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl64Pe $< ...
	$(ti.targets.C64P_big_endian.rootDir)/bin/cl6x -c  -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -s o64Pe $< -C   -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.o64Pe:C_DIR=
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.o64Pe: PATH:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.o64Pe: Path:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)

package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.s64Pe: | .interfaces
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.s64Pe: src/edma3_drv_init.c lib/64p/debug/ti.sdo.edma3.drv.a64Pe.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl64Pe -n $< ...
	$(ti.targets.C64P_big_endian.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -s o64Pe $< -C  -n -s --symdebug:none -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.s64Pe:C_DIR=
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.s64Pe: PATH:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.s64Pe: Path:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)

ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.o64Pe.dep
endif

package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.o64Pe: | .interfaces
package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.o64Pe: package/package_ti.sdo.edma3.drv.c lib/64p/debug/ti.sdo.edma3.drv.a64Pe.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl64Pe $< ...
	$(ti.targets.C64P_big_endian.rootDir)/bin/cl6x -c  -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/package -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/64p/debug/ti.sdo.edma3.drv/package -s o64Pe $< -C   -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/package -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.o64Pe:C_DIR=
package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.o64Pe: PATH:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)
package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.o64Pe: Path:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)

package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.s64Pe: | .interfaces
package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.s64Pe: package/package_ti.sdo.edma3.drv.c lib/64p/debug/ti.sdo.edma3.drv.a64Pe.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl64Pe -n $< ...
	$(ti.targets.C64P_big_endian.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/package -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/64p/debug/ti.sdo.edma3.drv/package -s o64Pe $< -C  -n -s --symdebug:none -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/package -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.s64Pe:C_DIR=
package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.s64Pe: PATH:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)
package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.s64Pe: Path:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)

ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.o64Pe.dep
endif

package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.o64Pe: | .interfaces
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.o64Pe: src/edma3_drv_adv.c lib/64p/debug/ti.sdo.edma3.drv.a64Pe.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl64Pe $< ...
	$(ti.targets.C64P_big_endian.rootDir)/bin/cl6x -c  -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -s o64Pe $< -C   -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.o64Pe:C_DIR=
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.o64Pe: PATH:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.o64Pe: Path:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)

package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.s64Pe: | .interfaces
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.s64Pe: src/edma3_drv_adv.c lib/64p/debug/ti.sdo.edma3.drv.a64Pe.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl64Pe -n $< ...
	$(ti.targets.C64P_big_endian.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -s o64Pe $< -C  -n -s --symdebug:none -qq -pdsw225 -me -mv64p -eo.o64Pe -ea.s64Pe  -mi10 -mo -me  -D_DEBUG_=1 -DBIG_ENDIAN_MODE -Dxdc_target_name__=C64P_big_endian -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_7_0_4 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.C64P_big_endian.rootDir)/include -fs=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src -fr=./package/lib/lib/64p/debug/ti.sdo.edma3.drv/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.s64Pe:C_DIR=
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.s64Pe: PATH:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)
package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.s64Pe: Path:=$(ti.targets.C64P_big_endian.rootDir)/bin/;$(PATH)

clean,64Pe ::
	-$(RM) package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.o64Pe
	-$(RM) package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.o64Pe
	-$(RM) package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.o64Pe
	-$(RM) package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.o64Pe
	-$(RM) package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.s64Pe
	-$(RM) package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.s64Pe
	-$(RM) package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.s64Pe
	-$(RM) package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.s64Pe

lib/64p/debug/ti.sdo.edma3.drv.a64Pe: package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_basic.o64Pe package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_init.o64Pe package/lib/lib/64p/debug/ti.sdo.edma3.drv/package/package_ti.sdo.edma3.drv.o64Pe package/lib/lib/64p/debug/ti.sdo.edma3.drv/src/edma3_drv_adv.o64Pe lib/64p/debug/ti.sdo.edma3.drv.a64Pe.mak

clean::
	-$(RM) lib/64p/debug/ti.sdo.edma3.drv.a64Pe.mak
