#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = gnu.targets.arm.GCArmv5T{1,0,4.2,0
#
ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/debug/universal_dma_ti_TRACE/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.ov5T.dep
endif

package/lib/lib/debug/universal_dma_ti_TRACE/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.ov5T: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.ov5T: package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.c lib/debug/universal_dma_ti_TRACE.av5T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clv5T $< ...
	$(gnu.targets.arm.GCArmv5T.rootDir)/bin/arm_v5t_le-gcc -c -MD -MF $@.dep -x c  -fPIC -Wunused -Wall -fno-strict-aliasing  -march=armv5t -Dfar=  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK -DCUSTOM_EDMAXFERUTIL  -Dxdc_target_name__=GCArmv5T -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_2_0 -g  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.ov5T: export LD_LIBRARY_PATH=

package/lib/lib/debug/universal_dma_ti_TRACE/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.sv5T: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.sv5T: package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.c lib/debug/universal_dma_ti_TRACE.av5T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clv5T -S $< ...
	$(gnu.targets.arm.GCArmv5T.rootDir)/bin/arm_v5t_le-gcc -c -MD -MF $@.dep -x c -S -fPIC -Wunused -Wall -fno-strict-aliasing  -march=armv5t -Dfar=  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK -DCUSTOM_EDMAXFERUTIL  -Dxdc_target_name__=GCArmv5T -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_2_0 -g  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.sv5T: export LD_LIBRARY_PATH=

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti.ov5T.dep
endif

package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti.ov5T: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti.ov5T: universal_dma_ti.c lib/debug/universal_dma_ti_TRACE.av5T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clv5T $< ...
	$(gnu.targets.arm.GCArmv5T.rootDir)/bin/arm_v5t_le-gcc -c -MD -MF $@.dep -x c  -fPIC -Wunused -Wall -fno-strict-aliasing  -march=armv5t -Dfar=  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK -DCUSTOM_EDMAXFERUTIL  -Dxdc_target_name__=GCArmv5T -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_2_0 -g  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti.ov5T: export LD_LIBRARY_PATH=

package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti.sv5T: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti.sv5T: universal_dma_ti.c lib/debug/universal_dma_ti_TRACE.av5T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clv5T -S $< ...
	$(gnu.targets.arm.GCArmv5T.rootDir)/bin/arm_v5t_le-gcc -c -MD -MF $@.dep -x c -S -fPIC -Wunused -Wall -fno-strict-aliasing  -march=armv5t -Dfar=  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK -DCUSTOM_EDMAXFERUTIL  -Dxdc_target_name__=GCArmv5T -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_2_0 -g  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti.sv5T: export LD_LIBRARY_PATH=

ifeq (,$(MK_NOGENDEPS))
-include package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti_ires.ov5T.dep
endif

package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti_ires.ov5T: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti_ires.ov5T: universal_dma_ti_ires.c lib/debug/universal_dma_ti_TRACE.av5T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clv5T $< ...
	$(gnu.targets.arm.GCArmv5T.rootDir)/bin/arm_v5t_le-gcc -c -MD -MF $@.dep -x c  -fPIC -Wunused -Wall -fno-strict-aliasing  -march=armv5t -Dfar=  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK -DCUSTOM_EDMAXFERUTIL  -Dxdc_target_name__=GCArmv5T -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_2_0 -g  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti_ires.ov5T: export LD_LIBRARY_PATH=

package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti_ires.sv5T: | .interfaces
package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti_ires.sv5T: universal_dma_ti_ires.c lib/debug/universal_dma_ti_TRACE.av5T.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) clv5T -S $< ...
	$(gnu.targets.arm.GCArmv5T.rootDir)/bin/arm_v5t_le-gcc -c -MD -MF $@.dep -x c -S -fPIC -Wunused -Wall -fno-strict-aliasing  -march=armv5t -Dfar=  -D_DEBUG_=1 -DDBC_ASSERTS=1  -DXDAIS_TRACE_ASSERT=1 -DXDAIS_TRACE_OUT=1 -DDMACHECK -DCUSTOM_EDMAXFERUTIL  -Dxdc_target_name__=GCArmv5T -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_bld__profile_debug -Dxdc_bld__vers_1_0_4_2_0 -g  $(XDCINCS)  -o $@ $<
	-@$(FIXDEP) $@.dep $@.dep
	
package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti_ires.sv5T: export LD_LIBRARY_PATH=

clean,v5T ::
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.ov5T
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti.ov5T
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti_ires.ov5T
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.sv5T
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti.sv5T
	-$(RM) package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti_ires.sv5T

lib/debug/universal_dma_ti_TRACE.av5T: package/lib/lib/debug/universal_dma_ti_TRACE/package/package_ti.sdo.fc.ires.examples.codecs.universal_dma.ov5T package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti.ov5T package/lib/lib/debug/universal_dma_ti_TRACE/universal_dma_ti_ires.ov5T lib/debug/universal_dma_ti_TRACE.av5T.mak

clean::
	-$(RM) lib/debug/universal_dma_ti_TRACE.av5T.mak
