#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.C674{1,0,7.0,4
#
ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.o674.dep
endif

package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.o674: | .interfaces
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.o674: src/edma3resmgr.c lib/generic/674/release/ti.sdo.edma3.rm.a674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl674 $< ...
	$(ti.targets.C674.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -s o674 $< -C   -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.o674:C_DIR=
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.o674: PATH:=$(ti.targets.C674.rootDir)/bin/;$(PATH)
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.o674: Path:=$(ti.targets.C674.rootDir)/bin/;$(PATH)

package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.s674: | .interfaces
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.s674: src/edma3resmgr.c lib/generic/674/release/ti.sdo.edma3.rm.a674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl674 -n $< ...
	$(ti.targets.C674.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -s o674 $< -C  -n -s --symdebug:none -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.s674:C_DIR=
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.s674: PATH:=$(ti.targets.C674.rootDir)/bin/;$(PATH)
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.s674: Path:=$(ti.targets.C674.rootDir)/bin/;$(PATH)

ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.o674.dep
endif

package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.o674: | .interfaces
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.o674: src/edma3_rm_gbl_data.c lib/generic/674/release/ti.sdo.edma3.rm.a674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl674 $< ...
	$(ti.targets.C674.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -s o674 $< -C   -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.o674:C_DIR=
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.o674: PATH:=$(ti.targets.C674.rootDir)/bin/;$(PATH)
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.o674: Path:=$(ti.targets.C674.rootDir)/bin/;$(PATH)

package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.s674: | .interfaces
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.s674: src/edma3_rm_gbl_data.c lib/generic/674/release/ti.sdo.edma3.rm.a674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl674 -n $< ...
	$(ti.targets.C674.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -s o674 $< -C  -n -s --symdebug:none -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.s674:C_DIR=
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.s674: PATH:=$(ti.targets.C674.rootDir)/bin/;$(PATH)
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.s674: Path:=$(ti.targets.C674.rootDir)/bin/;$(PATH)

ifneq (clean,$(MAKECMDGOALS))
-include package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.o674.dep
endif

package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.o674: | .interfaces
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.o674: package/package_ti.sdo.edma3.rm.c lib/generic/674/release/ti.sdo.edma3.rm.a674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl674 $< ...
	$(ti.targets.C674.rootDir)/bin/cl6x -c  -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package -s o674 $< -C   -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.o674:C_DIR=
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.o674: PATH:=$(ti.targets.C674.rootDir)/bin/;$(PATH)
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.o674: Path:=$(ti.targets.C674.rootDir)/bin/;$(PATH)

package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.s674: | .interfaces
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.s674: package/package_ti.sdo.edma3.rm.c lib/generic/674/release/ti.sdo.edma3.rm.a674.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl674 -n $< ...
	$(ti.targets.C674.rootDir)/bin/cl6x -c -n -s --symdebug:none -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package -fc $<
	$(MKDEP) -a $@.dep -p package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package -s o674 $< -C  -n -s --symdebug:none -qq -pdsw225 -mv6740 -eo.o674 -ea.s674  -mi10 -mo   -DGENERIC  -Dxdc_target_name__=C674 -Dxdc_target_types__=ti/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_7_0_4 -O2  $(XDCINCS) -I$(ti.targets.C674.rootDir)/include -fs=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package -fr=./package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.s674:C_DIR=
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.s674: PATH:=$(ti.targets.C674.rootDir)/bin/;$(PATH)
package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.s674: Path:=$(ti.targets.C674.rootDir)/bin/;$(PATH)

clean,674 ::
	-$(RM) package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.o674
	-$(RM) package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.o674
	-$(RM) package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.o674
	-$(RM) package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.s674
	-$(RM) package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.s674
	-$(RM) package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.s674

lib/generic/674/release/ti.sdo.edma3.rm.a674: package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3resmgr.o674 package/lib/lib/generic/674/release/ti.sdo.edma3.rm/src/edma3_rm_gbl_data.o674 package/lib/lib/generic/674/release/ti.sdo.edma3.rm/package/package_ti.sdo.edma3.rm.o674 lib/generic/674/release/ti.sdo.edma3.rm.a674.mak

clean::
	-$(RM) lib/generic/674/release/ti.sdo.edma3.rm.a674.mak
