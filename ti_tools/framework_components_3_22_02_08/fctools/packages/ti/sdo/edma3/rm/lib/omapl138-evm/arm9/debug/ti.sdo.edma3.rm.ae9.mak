#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.arm.elf.Arm9{1,0,4.6,3
#
ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.oe9.dep
endif

package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.oe9: | .interfaces
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.oe9: package/package_ti.sdo.edma3.rm.c lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle9 $< ...
	$(ti.targets.arm.elf.Arm9.rootDir)/bin/cl470 -c  -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package -s oe9 $< -C   -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.oe9:C_DIR=
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.oe9: PATH:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.oe9: Path:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)

package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.se9: | .interfaces
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.se9: package/package_ti.sdo.edma3.rm.c lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle9 -n $< ...
	$(ti.targets.arm.elf.Arm9.rootDir)/bin/cl470 -c -n -s --symdebug:none -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package -s oe9 $< -C  -n -s --symdebug:none -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.se9:C_DIR=
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.se9: PATH:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.se9: Path:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)

ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.oe9.dep
endif

package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.oe9: | .interfaces
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.oe9: src/edma3resmgr.c lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle9 $< ...
	$(ti.targets.arm.elf.Arm9.rootDir)/bin/cl470 -c  -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -s oe9 $< -C   -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.oe9:C_DIR=
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.oe9: PATH:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.oe9: Path:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)

package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.se9: | .interfaces
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.se9: src/edma3resmgr.c lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle9 -n $< ...
	$(ti.targets.arm.elf.Arm9.rootDir)/bin/cl470 -c -n -s --symdebug:none -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -s oe9 $< -C  -n -s --symdebug:none -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.se9:C_DIR=
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.se9: PATH:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.se9: Path:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)

ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.oe9.dep
endif

package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.oe9: | .interfaces
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.oe9: src/configs/edma3_omapl138_cfg.c lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle9 $< ...
	$(ti.targets.arm.elf.Arm9.rootDir)/bin/cl470 -c  -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs -s oe9 $< -C   -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.oe9:C_DIR=
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.oe9: PATH:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.oe9: Path:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)

package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.se9: | .interfaces
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.se9: src/configs/edma3_omapl138_cfg.c lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle9 -n $< ...
	$(ti.targets.arm.elf.Arm9.rootDir)/bin/cl470 -c -n -s --symdebug:none -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs -s oe9 $< -C  -n -s --symdebug:none -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.se9:C_DIR=
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.se9: PATH:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.se9: Path:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)

ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.oe9.dep
endif

package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.oe9: | .interfaces
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.oe9: src/edma3_rm_gbl_data.c lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle9 $< ...
	$(ti.targets.arm.elf.Arm9.rootDir)/bin/cl470 -c  -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -s oe9 $< -C   -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.oe9:C_DIR=
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.oe9: PATH:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.oe9: Path:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)

package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.se9: | .interfaces
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.se9: src/edma3_rm_gbl_data.c lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle9 -n $< ...
	$(ti.targets.arm.elf.Arm9.rootDir)/bin/cl470 -c -n -s --symdebug:none -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -s oe9 $< -C  -n -s --symdebug:none -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -D_DEBUG_=1  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_6_3 --symdebug:dwarf  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include/rts -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src -fr=./package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.se9:C_DIR=
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.se9: PATH:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)
package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.se9: Path:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)

clean,e9 ::
	-$(RM) package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.oe9
	-$(RM) package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.oe9
	-$(RM) package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.oe9
	-$(RM) package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.oe9
	-$(RM) package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.se9
	-$(RM) package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.se9
	-$(RM) package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.se9
	-$(RM) package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.se9

lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9: package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.oe9 package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3resmgr.oe9 package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/configs/edma3_omapl138_cfg.oe9 package/lib/lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.oe9 lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9.mak

clean::
	-$(RM) lib/omapl138-evm/arm9/debug/ti.sdo.edma3.rm.ae9.mak
