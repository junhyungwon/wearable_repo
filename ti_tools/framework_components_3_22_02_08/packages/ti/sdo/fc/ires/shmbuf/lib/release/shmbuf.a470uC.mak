#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = gnu.targets.arm.UCArm9{1,0,4.2,1
#
ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/shmbuf/package/package_ti.sdo.fc.ires.shmbuf.o470uC.dep
endif

package/lib/lib/release/shmbuf/package/package_ti.sdo.fc.ires.shmbuf.o470uC: | .interfaces
package/lib/lib/release/shmbuf/package/package_ti.sdo.fc.ires.shmbuf.o470uC: package/package_ti.sdo.fc.ires.shmbuf.c lib/release/shmbuf.a470uC.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl470uC $< ...
	$(gnu.targets.arm.UCArm9.rootDir)//bin/arm-linux-gcc -c -MD -MF $@.dep -x c  -Wunused -D_REENTRANT   -Dfar=   -DDBC_ASSERTS=0 -Dxdc_runtime_Assert_DISABLE_ALL=1  -Dxdc_target_name__=UCArm9 -Dxdc_target_types__=gnu/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_2_1 -O2 -ffunction-sections -fdata-sections  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/shmbuf/package/package_ti.sdo.fc.ires.shmbuf.o470uC: export LD_LIBRARY_PATH=

package/lib/lib/release/shmbuf/package/package_ti.sdo.fc.ires.shmbuf.s470uC: | .interfaces
package/lib/lib/release/shmbuf/package/package_ti.sdo.fc.ires.shmbuf.s470uC: package/package_ti.sdo.fc.ires.shmbuf.c lib/release/shmbuf.a470uC.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl470uC -S $< ...
	$(gnu.targets.arm.UCArm9.rootDir)//bin/arm-linux-gcc -c -MD -MF $@.dep -x c -S -Wunused -D_REENTRANT   -Dfar=   -DDBC_ASSERTS=0 -Dxdc_runtime_Assert_DISABLE_ALL=1  -Dxdc_target_name__=UCArm9 -Dxdc_target_types__=gnu/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_2_1 -O2 -ffunction-sections -fdata-sections  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/shmbuf/package/package_ti.sdo.fc.ires.shmbuf.s470uC: export LD_LIBRARY_PATH=

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/shmbuf/shmbuf.o470uC.dep
endif

package/lib/lib/release/shmbuf/shmbuf.o470uC: | .interfaces
package/lib/lib/release/shmbuf/shmbuf.o470uC: shmbuf.c lib/release/shmbuf.a470uC.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl470uC $< ...
	$(gnu.targets.arm.UCArm9.rootDir)//bin/arm-linux-gcc -c -MD -MF $@.dep -x c  -Wunused -D_REENTRANT   -Dfar=   -DDBC_ASSERTS=0 -Dxdc_runtime_Assert_DISABLE_ALL=1  -Dxdc_target_name__=UCArm9 -Dxdc_target_types__=gnu/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_2_1 -O2 -ffunction-sections -fdata-sections  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/shmbuf/shmbuf.o470uC: export LD_LIBRARY_PATH=

package/lib/lib/release/shmbuf/shmbuf.s470uC: | .interfaces
package/lib/lib/release/shmbuf/shmbuf.s470uC: shmbuf.c lib/release/shmbuf.a470uC.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl470uC -S $< ...
	$(gnu.targets.arm.UCArm9.rootDir)//bin/arm-linux-gcc -c -MD -MF $@.dep -x c -S -Wunused -D_REENTRANT   -Dfar=   -DDBC_ASSERTS=0 -Dxdc_runtime_Assert_DISABLE_ALL=1  -Dxdc_target_name__=UCArm9 -Dxdc_target_types__=gnu/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_2_1 -O2 -ffunction-sections -fdata-sections  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/shmbuf/shmbuf.s470uC: export LD_LIBRARY_PATH=

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/release/shmbuf/shmbuf_params.o470uC.dep
endif

package/lib/lib/release/shmbuf/shmbuf_params.o470uC: | .interfaces
package/lib/lib/release/shmbuf/shmbuf_params.o470uC: shmbuf_params.c lib/release/shmbuf.a470uC.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl470uC $< ...
	$(gnu.targets.arm.UCArm9.rootDir)//bin/arm-linux-gcc -c -MD -MF $@.dep -x c  -Wunused -D_REENTRANT   -Dfar=   -DDBC_ASSERTS=0 -Dxdc_runtime_Assert_DISABLE_ALL=1  -Dxdc_target_name__=UCArm9 -Dxdc_target_types__=gnu/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_2_1 -O2 -ffunction-sections -fdata-sections  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/shmbuf/shmbuf_params.o470uC: export LD_LIBRARY_PATH=

package/lib/lib/release/shmbuf/shmbuf_params.s470uC: | .interfaces
package/lib/lib/release/shmbuf/shmbuf_params.s470uC: shmbuf_params.c lib/release/shmbuf.a470uC.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl470uC -S $< ...
	$(gnu.targets.arm.UCArm9.rootDir)//bin/arm-linux-gcc -c -MD -MF $@.dep -x c -S -Wunused -D_REENTRANT   -Dfar=   -DDBC_ASSERTS=0 -Dxdc_runtime_Assert_DISABLE_ALL=1  -Dxdc_target_name__=UCArm9 -Dxdc_target_types__=gnu/targets/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_4_2_1 -O2 -ffunction-sections -fdata-sections  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/release/shmbuf/shmbuf_params.s470uC: export LD_LIBRARY_PATH=

clean,470uC ::
	-$(RM) package/lib/lib/release/shmbuf/package/package_ti.sdo.fc.ires.shmbuf.o470uC
	-$(RM) package/lib/lib/release/shmbuf/shmbuf.o470uC
	-$(RM) package/lib/lib/release/shmbuf/shmbuf_params.o470uC
	-$(RM) package/lib/lib/release/shmbuf/package/package_ti.sdo.fc.ires.shmbuf.s470uC
	-$(RM) package/lib/lib/release/shmbuf/shmbuf.s470uC
	-$(RM) package/lib/lib/release/shmbuf/shmbuf_params.s470uC

lib/release/shmbuf.a470uC: package/lib/lib/release/shmbuf/package/package_ti.sdo.fc.ires.shmbuf.o470uC package/lib/lib/release/shmbuf/shmbuf.o470uC package/lib/lib/release/shmbuf/shmbuf_params.o470uC lib/release/shmbuf.a470uC.mak

clean::
	-$(RM) lib/release/shmbuf.a470uC.mak
