#
# EDMA3 LLD
#

ifeq ($(ROOTDIR),)
  $(error Error: Environment variable ROOTDIR not set)
endif

# Begin block - Build options
#  By the end of this block, we'd have either xdc or make or both in BUILD_OPTIONS
#  using which, we'd build either XDC packages (+run configuro) 
#  or make based components (+link) or both.
BUILD_OPTIONS = xdc make
VALID_BUILD_OPTIONS = full $(BUILD_OPTIONS)
ifeq ($(filter $(BUILD),$(VALID_BUILD_OPTIONS)),)
 ifeq ($(BUILD),)
  BUILD = full
 else
  $(error Error: Invalid value specified for BUILD. Valid values are - $(VALID_BUILD_OPTIONS))
 endif
endif
ifneq ($(BUILD),full)
 BUILD_OPTIONS = $(BUILD)
endif
# End block - Build options

include $(ROOTDIR)/makerules/build_config.mk
include $(ROOTDIR)/makerules/env.mk
include component.mk

# Listing common targets
edma3_lld_COMP_LIST_ALL = $(edma3_lld_COMP_LIST_c6xdsp)
edma3_lld_PKG_LIST_ALL = $(foreach PKG,$(edma3_lld_COMP_LIST_ALL),$($(PKG)_PKG_LIST))
CLEANALL_TARGETS = $(addsuffix _clean, $(edma3_lld_PKG_LIST_ALL))
CLEANALL_EXAMPLES = $(addsuffix _clean, $(edma3_lld_EXAMPLES_LIST))

.PHONY : $(edma3_lld_LIBS_ALL) $(edma3_lld_EXAMPLES_LIST)

all: libs examples

libs: $(edma3_lld_LIBS_ALL)

examples: $(edma3_lld_EXAMPLES_LIST)

clean: $(CLEANALL_TARGETS)

cleanall: $(CLEANALL_TARGETS) $(CLEANALL_EXAMPLES)

#
# Rule to build all modules
#

edma3_lld_drv:
	$(ECHO) \# Making c64p:debug:edma3_lld_drv
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Making c64p:release:edma3_lld_drv
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm PROFILE_c6xdsp=release
	$(ECHO) \# Making c64p:debug:edma3_lld_drv -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making c64p:release:edma3_lld_drv -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm PROFILE_c6xdsp=release ENDIAN=big
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making c66:debug:edma3_lld_drv
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Making c66:release:edma3_lld_drv
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm PROFILE_c6xdsp=release
	$(ECHO) \# Making c66:debug:edma3_lld_drv -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making c66:release:edma3_lld_drv -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm PROFILE_c6xdsp=release ENDIAN=big
endif
	$(ECHO) \# Making c674:debug:edma3_lld_drv 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Making c674:release:edma3_lld_drv 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm PROFILE_c6xdsp=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making a8:debug:edma3_lld_drv 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=debug
	$(ECHO) \# Making a8:release:edma3_lld_drv 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=release
	$(ECHO) \# Making arm9:debug:edma3_lld_drv 
	$(MAKE) -C $($@_PATH) PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Making arm9:release:edma3_lld_drv 
	$(MAKE) -C $($@_PATH) PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=release
	$(ECHO) \# Making m3:debug:edma3_lld_drv 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Making m3:release:edma3_lld_drv 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release
	$(ECHO) \# Making m3:debug:edma3_lld_drv 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug
	$(ECHO) \# Making m3:release:edma3_lld_drv 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release	
endif
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making c64t:debug:edma3_lld_drv
	$(MAKE) -C $($@_PATH) PLATFORM=omap4-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Making c64t:release:edma3_lld_drv
	$(MAKE) -C $($@_PATH) PLATFORM=omap4-evm PROFILE_c6xdsp=release
endif

edma3_lld_rm: 
	$(ECHO) \# Making ti814x-evm :debug:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Making ti816x-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Making ti816x-sim:debug:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Making ti814x-evm :release:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm PROFILE_c6xdsp=release
	$(ECHO) \# Making ti816x-evm:release:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm PROFILE_c6xdsp=release
	$(ECHO) \# Making ti816x-sim:release:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-sim PROFILE_c6xdsp=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making ti816x-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Making ti816x-evm:rel:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release
	$(ECHO) \# Making ti816x-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug
	$(ECHO) \# Making ti816x-evm:rel:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release
	$(ECHO) \# Making ti814x-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Making ti814x-evm:rel:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=m3vpss PROFILE_m3vpss=release
	$(ECHO) \# Making ti814x-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=m3video PROFILE_m3video=debug
	$(ECHO) \# Making ti814x-evm:rel:edma3_lld_rm 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=m3video PROFILE_m3video=release
endif

# Generic platform
edma3_lld_rm_generic:

	$(ECHO) \# Making generic/c674:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=674 PROFILE_674=debug
	$(ECHO) \# Making generic/c674:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=674 PROFILE_674=release
	$(ECHO) \# Making generic/c64p:debug:edma3_lld_rm: -for big_endian
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=64p PROFILE_64p=debug ENDIAN=big
	$(ECHO) \# Making generic/c64p:release:edma3_lld_rm: -for big_endian
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=64p PROFILE_64p=release ENDIAN=big
	$(ECHO) \# Making generic/c64p:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=64p PROFILE_64p=debug ENDIAN=little
	$(ECHO) \# Making generic/c64p:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=64p PROFILE_64p=release ENDIAN=little
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making generic/c66:debug:edma3_lld_rm: -for big_endian
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=66 PROFILE_66=debug ENDIAN=big
	$(ECHO) \# Making generic/c66:release:edma3_lld_rm: -for big_endian
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=66 PROFILE_66=release ENDIAN=big
	$(ECHO) \# Making generic/c66:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=66 PROFILE_66=debug ENDIAN=little
	$(ECHO) \# Making generic/c66:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=66 PROFILE_66=release ENDIAN=little
endif
	$(ECHO) \# Making generic/m3:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=m3 PROFILE_m3=debug
	$(ECHO) \# Making generic/m3:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=m3 PROFILE_m3=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making generic/a8:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=a8 PROFILE_a8=debug
	$(ECHO) \# Making generic/a8:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=a8 PROFILE_a8=release
	$(ECHO) \# Making generic/arm9:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Making generic/arm9:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic,,$@)_PATH) PLATFORM=generic CORE=arm9 PROFILE_arm9=release
endif
 
edma3_lld_rm_sample:
	$(ECHO) \# Making c6472-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making tci6486-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=debug
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making tci6608-sim:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making tci6616-sim:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making c6670-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making c6678-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=debug
endif
	$(ECHO) \# Making c6472-evm:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making tci6486-evm:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making tci6608-sim:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making tci6616-sim:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making c6670-evm:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making c6678-evm:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
endif
	$(ECHO) \# Making omapl138-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=omapl138-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making c6748-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6748-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making da830-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=da830-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making ti814x-evm :debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making ti816x-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making ti816x-sim:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-sim CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making c6472-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making tci6486-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making tci6608-sim:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making tci6616-sim:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making c6670-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making c6678-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=release
endif
	$(ECHO) \# Making c6472-evm:release:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making tci6486-evm:release:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making tci6608-sim:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making tci6616-sim:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making c6670-evm:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making c6678-evm:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
endif
	$(ECHO) \# Making omapl138-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=omapl138-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making c6748-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6748-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making da830-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=da830-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making ti814x-evm :release:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making ti816x-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making ti816x-sim:release:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-sim CORE=c6xdsp PROFILE_c6xdsp=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making ti814x-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=debug
	$(ECHO) \# Making ti814x-evm:rel:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=release
	$(ECHO) \# Making omapl138-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Making omapl138-evm:rel:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=release
	$(ECHO) \# Making ti816x-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug
	$(ECHO) \# Making ti816x-evm:rel:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release	
	$(ECHO) \# Making ti816x-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Making ti816x-evm:rel:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release
endif
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making omap4-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=omap4-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making omap4-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=omap4-evm CORE=c6xdsp PROFILE_c6xdsp=release
endif

edma3_lld_drv_sample:
	$(ECHO) \# Making c6472-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making tci6486-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=debug
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making tci6608-sim:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=debug 
	$(ECHO) \# Making tci6616-sim:debug:edma3_lld_drv_sample
	$(MAKE) -C $($@_PATH) PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=debug 
	$(ECHO) \# Making c6670-evm:debug:edma3_lld_drv_sample
	$(MAKE) -C $($@_PATH) PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=debug 
	$(ECHO) \# Making c6678-evm:debug:edma3_lld_drv_sample
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=debug 
endif
	$(ECHO) \# Making c6472-evm:debug:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making tci6486-evm:debug:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making tci6608-sim:debug:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making tci6616-sim:debug:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making c6670-evm:debug:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making c6678-evm:debug:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
endif
	$(ECHO) \# Making omapl138-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=omapl138-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making c6748-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6748-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making da830-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=da830-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making ti814x-evm :debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making ti816x-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making ti816x-sim:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-sim CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making c6472-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making tci6486-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making tci6608-sim:release:edma3_lld_drv_sample
	$(MAKE) -C $($@_PATH) PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=release 
	$(ECHO) \# Making tci6616-sim:release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=release 
	$(ECHO) \# Making c6670-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=release 
	$(ECHO) \# Making c6678-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=release 
endif
	$(ECHO) \# Making c6472-evm:release:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making tci6486-evm:release:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making tci6608-sim:release:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making tci6616-sim:release:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making c6670-evm:release:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making c6678-evm:release:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($@_PATH) PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
endif
	$(ECHO) \# Making omapl138-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=omapl138-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making c6748-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=c6748-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making da830-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=da830-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making ti814x-evm :release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making ti816x-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making ti816x-sim:release:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-sim CORE=c6xdsp PROFILE_c6xdsp=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making ti814x-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=debug
	$(ECHO) \# Making ti814x-evm:rel:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=release
	$(ECHO) \# Making omapl138-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Making omapl138-evm:rel:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=release
	$(ECHO) \# Making ti816x-evm:debug:edma3_lld_drv_sample
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Making ti816x-evm:rel:edma3_lld_drv_sample 	
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release
	$(ECHO) \# Making ti816x-evm:rel:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release
	$(ECHO) \# Making ti816x-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($@_PATH) PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug

endif
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Making omap4-evm:debug:edma3_lld_drv_sample
	$(MAKE) -C $($@_PATH) PLATFORM=omap4-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making omap4-evm:release:edma3_lld_drv_sample
	$(MAKE) -C $($@_PATH) PLATFORM=omap4-evm CORE=c6xdsp PROFILE_c6xdsp=release
endif

#
# Rule to build all examples
#

edma3_drv_c6472_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=release

edma3_drv_c6472be_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6472-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
 
edma3_drv_c6748_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6748-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6748-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6748-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6748-evm CORE=c6xdsp PROFILE_c6xdsp=release
 
edma3_drv_da830_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=da830-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=da830-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=da830-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=da830-evm CORE=c6xdsp PROFILE_c6xdsp=release
 
edma3_drv_omapl138_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=omapl138-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=omapl138-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=omapl138-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=omapl138-evm CORE=c6xdsp PROFILE_c6xdsp=release

edma3_drv_omap4_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=omap4-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=omap4-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=omap4-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=omap4-evm CORE=c6xdsp PROFILE_c6xdsp=release

edma3_drv_tci6486_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=release
 
edma3_drv_tci6486be_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6486-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
 
edma3_drv_ti816x_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti816x-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti816x-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti816x-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti816x-evm CORE=c6xdsp PROFILE_c6xdsp=release

edma3_drv_ti814x_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti814x-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti814x-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti814x-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti814x-evm CORE=c6xdsp PROFILE_c6xdsp=release

edma3_drv_ti816x_sim_sample:
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti816x-sim CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti816x-sim CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti816x-sim CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti816x-sim CORE=c6xdsp PROFILE_c6xdsp=release
 
edma3_drv_tci6608_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6608-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6608-sim PROFILE_c6xdsp=release
endif
 
edma3_drv_tci6608be_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6608-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6608-sim CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6608-sim PROFILE_c6xdsp=release ENDIAN=big
endif

edma3_drv_tci6616_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6616-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6616-sim PROFILE_c6xdsp=release
endif
 
edma3_drv_tci6616be_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6616-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=tci6616-sim CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=tci6616-sim PROFILE_c6xdsp=release ENDIAN=big
endif

edma3_drv_c6670_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6670-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6670-evm PROFILE_c6xdsp=release
endif

edma3_drv_c6670be_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6670-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6670-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6670-evm PROFILE_c6xdsp=release ENDIAN=big
endif
 

edma3_drv_c6678_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6678-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6678-evm PROFILE_c6xdsp=release
endif
 
edma3_drv_c6678be_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6678-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Configuring XDC packages for $@:c6xdsp:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=c6678-evm CORE=c6xdsp PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=c6678-evm PROFILE_c6xdsp=release ENDIAN=big
endif
 

edma3_drv_arm_omapl138_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:arm9:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Configuring XDC packages for $@:arm9:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=release
endif

edma3_drv_arm_ti814x_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:a8:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=debug
	$(ECHO) \# Configuring XDC packages for $@:a8:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=release
endif

edma3_drv_arm_ti816x_sample:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Configuring XDC packages for $@:m3:debug 
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug	

	$(ECHO) \# Configuring XDC packages for $@:m3:release 
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release
	
	$(ECHO) \# Configuring XDC packages for $@:m3:debug 
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Making example $@:debug
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug

	$(ECHO) \# Configuring XDC packages for $@:m3:release
	$(MAKE) -C $($@_EXAMPLES_PATH) xdc_configuro PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release	
	$(ECHO) \# Making example $@:release
	$(MAKE) -C $($@_EXAMPLES_PATH) PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release
endif

# Clean targets

edma3_lld_drv_clean:
	$(ECHO) \# Cleaning c64t:debug:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c64t:release:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c64p:debug:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c64p:release:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c64p:debug:edma3_lld_drv -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c64p:release:edma3_lld_drv -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=release ENDIAN=big

	$(ECHO) \# Cleaning c66:debug:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c66:release:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c66:debug:edma3_lld_drv -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c66:release:edma3_lld_drv -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning c66:debug:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c66:release:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c66:debug:edma3_lld_drv -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c66:release:edma3_lld_drv -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning c66:debug:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c66:release:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c66:debug:edma3_lld_drv -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c66:release:edma3_lld_drv -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning c66:debug:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c66:release:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c64t:debug:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c64t:release:edma3_lld_drv
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c66:debug:edma3_lld_drv -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c66:release:edma3_lld_drv -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning c674:debug:edma3_lld_drv 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c674:release:edma3_lld_drv 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm PROFILE_c6xdsp=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Cleaning a8:debug:edma3_lld_drv 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=debug
	$(ECHO) \# Cleaning a8:release:edma3_lld_drv 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=release
	$(ECHO) \# Cleaning arm9:debug:edma3_lld_drv 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Cleaning arm9:release:edma3_lld_drv 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=release
	$(ECHO) \# Cleaning m3:debug:edma3_lld_drv 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Cleaning m3:release:edma3_lld_drv 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release
	$(ECHO) \# Cleaning m3:debug:edma3_lld_drv 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug
	$(ECHO) \# Cleaning m3:release:edma3_lld_drv 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release
endif

edma3_lld_rm_clean: edma3_lld_rm_generic_clean
	$(ECHO) \# Cleaning c6472-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning omap4-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning omap4-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning tci6486-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning tci6608-sim:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning tci6616-sim:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6670-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6678-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=debug

	$(ECHO) \# Cleaning c6472-evm:debug:edma3_lld_rm -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning tci6486-evm:debug:edma3_lld_rm -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning tci6608-sim:debug:edma3_lld_rm  -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning tci6616-sim:debug:edma3_lld_rm   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c6670-evm:debug:edma3_lld_rm   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c6678-evm:debug:edma3_lld_rm   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning omapl138-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6748-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6748-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning da830-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=da830-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning ti814x-evm :debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning ti816x-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning ti816x-sim:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6472-evm:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning tci6486-evm:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning tci6608-sim:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning tci6616-sim:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c6670-evm:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c6678-evm:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=release

	$(ECHO) \# Cleaning c6472-evm:release:edma3_lld_rm -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning tci6486-evm:release:edma3_lld_rm -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning tci6608-sim:release:edma3_lld_rm  -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning tci6616-sim:release:edma3_lld_rm   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning c6670-evm:release:edma3_lld_rm   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning c6678-evm:release:edma3_lld_rm   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=release ENDIAN=big

	$(ECHO) \# Cleaning omapl138-evm:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c6748-evm:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6748-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning da830-evm:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=da830-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning ti814x-evm :release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning ti816x-evm:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning ti816x-sim:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-sim PROFILE_c6xdsp=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Cleaning ti814x-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=debug
	$(ECHO) \# Cleaning ti814x-evm:rel:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=release
	$(ECHO) \# Cleaning omapl138-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Cleaning omapl138-evm:rel:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=release
	$(ECHO) \# Cleaning ti816x-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Cleaning ti816x-evm:rel:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release
	$(ECHO) \# Cleaning ti816x-evm:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug
	$(ECHO) \# Cleaning ti816x-evm:rel:edma3_lld_rm 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release
endif

# Generic platform
edma3_lld_rm_generic_clean:

	$(ECHO) \# Cleaning generic/c674:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=674 PROFILE_674=debug
	$(ECHO) \# Cleaning generic/c674:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=674 PROFILE_674=release
	$(ECHO) \# Cleaning generic/c64p:debug:edma3_lld_rm: -for big_endian
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=64p PROFILE_64p=debug ENDIAN=big
	$(ECHO) \# Cleaning generic/c64p:release:edma3_lld_rm: -for big_endian
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=64p PROFILE_64p=release ENDIAN=big
	$(ECHO) \# Cleaning generic/c64p:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=64p PROFILE_64p=debug ENDIAN=little
	$(ECHO) \# Cleaning generic/c64p:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=64p PROFILE_64p=release ENDIAN=little
	$(ECHO) \# Cleaning generic/c66:debug:edma3_lld_rm: -for big_endian
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=66 PROFILE_66=debug ENDIAN=big
	$(ECHO) \# Cleaning generic/c66:release:edma3_lld_rm: -for big_endian
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=66 PROFILE_66=release ENDIAN=big
	$(ECHO) \# Cleaning generic/c66:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=66 PROFILE_66=debug ENDIAN=little
	$(ECHO) \# Cleaning generic/c66:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=66 PROFILE_66=release ENDIAN=little
	$(ECHO) \# Cleaning generic/m3:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=m3 PROFILE_m3=debug
	$(ECHO) \# Cleaning generic/m3:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=m3 PROFILE_m3=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Cleaning generic/a8:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=a8 PROFILE_a8=debug
	$(ECHO) \# Cleaning generic/a8:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=a8 PROFILE_a8=release
	$(ECHO) \# Cleaning generic/arm9:debug:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Cleaning generic/arm9:release:edma3_lld_rm 
	$(MAKE) -C $($(subst _generic_clean,,$@)_PATH) clean PLATFORM=generic CORE=arm9 PROFILE_arm9=release
endif
   
edma3_lld_rm_sample_clean:
	$(ECHO) \# Cleaning omap4-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning omap4-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c6472-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning tci6486-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning tci6608-sim:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning tci6616-sim:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6670-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6678-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning omap4-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6472-evm:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning tci6486-evm:debug:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning tci6608-sim:release:edma3_lld_rm_sample  -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning tci6616-sim:debug:edma3_lld_rm_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c6670-evm:debug:edma3_lld_rm_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c6678-evm:debug:edma3_lld_rm_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=debug ENDIAN=big

	$(ECHO) \# Cleaning omapl138-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6748-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6748-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning da830-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=da830-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning ti814x-evm :debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning ti816x-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning ti816x-sim:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6472-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning tci6486-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning tci6608-sim:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning tci6616-sim:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c6670-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c6678-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning omap4-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c6472-evm:release:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning tci6486-evm:release:edma3_lld_rm_sample -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning tci6608-sim:release:edma3_lld_rm_sample  -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning tci6616-sim:debug:edma3_lld_rm_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning c6670-evm:debug:edma3_lld_rm_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning c6678-evm:debug:edma3_lld_rm_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning omapl138-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c6748-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6748-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning da830-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=da830-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning ti814x-evm :release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning ti816x-evm:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning ti816x-sim:release:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-sim PROFILE_c6xdsp=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Cleaning ti814x-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=debug
	$(ECHO) \# Cleaning ti814x-evm:rel:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=release
	$(ECHO) \# Cleaning omapl138-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Cleaning omapl138-evm:rel:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=release
	$(ECHO) \# Cleaning ti816x-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug
	$(ECHO) \# Cleaning ti816x-evm:rel:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release
	$(ECHO) \# Cleaning ti816x-evm:debug:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Cleaning ti816x-evm:rel:edma3_lld_rm_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release
endif
 
edma3_lld_drv_sample_clean:
	$(ECHO) \# Cleaning omap4-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning omap4-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c6472-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning tci6486-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning tci6608-sim:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning tci6616-sim:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6670-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6678-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning omap4-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6472-evm:debug:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning tci6486-evm:debug:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning tci6608-sim:debug:edma3_lld_drv_sample  -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning tci6616-sim:debug:edma3_lld_drv_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c6670-evm:debug:edma3_lld_drv_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning c6678-evm:debug:edma3_lld_drv_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning omapl138-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6748-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6748-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning da830-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=da830-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning ti814x-evm :debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning ti816x-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning ti816x-sim:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning c6472-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning tci6486-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning tci6608-sim:release:edma3_lld_drv_sample  
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=release 
	$(ECHO) \# Cleaning tci6616-sim:release:edma3_lld_drv_sample  
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=release 
	$(ECHO) \# Cleaning c6670-evm:release:edma3_lld_drv_sample   
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=release 
	$(ECHO) \# Cleaning c6678-evm:release:edma3_lld_drv_sample   
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=release 
	$(ECHO) \# Cleaning omap4-evm:release:edma3_lld_drv_sample   
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omap4-evm PROFILE_c6xdsp=release 

	$(ECHO) \# Cleaning c6472-evm:release:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6472-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning tci6486-evm:release:edma3_lld_drv_sample -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6486-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning tci6608-sim:release:edma3_lld_drv_sample  -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6608-sim PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning tci6616-sim:release:edma3_lld_drv_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=tci6616-sim PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning c6670-evm:release:edma3_lld_drv_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6670-evm PROFILE_c6xdsp=release ENDIAN=big
	$(ECHO) \# Cleaning c6678-evm:release:edma3_lld_drv_sample   -for big_endian
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6678-evm PROFILE_c6xdsp=release ENDIAN=big

	$(ECHO) \# Cleaning omapl138-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning c6748-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=c6748-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning da830-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=da830-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning ti814x-evm :release:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning ti816x-evm:release:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm PROFILE_c6xdsp=release
	$(ECHO) \# Cleaning ti816x-sim:release:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-sim PROFILE_c6xdsp=release
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Cleaning ti814x-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=debug
	$(ECHO) \# Cleaning ti814x-evm:rel:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=release
	$(ECHO) \# Cleaning omapl138-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Cleaning omapl138-evm:rel:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=release
	$(ECHO) \# Cleaning ti816x-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug
	$(ECHO) \# Cleaning ti816x-evm:rel:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release
	$(ECHO) \# Cleaning ti816x-evm:debug:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Cleaning ti816x-evm:rel:edma3_lld_drv_sample 
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release
endif

#
# Rule to clean all examples
#

edma3_drv_c6472_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6472-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6472-evm PROFILE_c6xdsp=release

edma3_drv_c6472be_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6472-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6472-evm PROFILE_c6xdsp=release ENDIAN=big
 
edma3_drv_c6748_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6748-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6748-evm PROFILE_c6xdsp=release
 
edma3_drv_da830_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=da830-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=da830-evm PROFILE_c6xdsp=release
 
edma3_drv_omapl138_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=omapl138-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=omapl138-evm PROFILE_c6xdsp=release

edma3_drv_arm_omapl138_sample_clean:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=omapl138-evm CORE=arm9 PROFILE_arm9=release
endif

edma3_drv_omap4_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=omap4-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=omap4-evm PROFILE_c6xdsp=release
  
edma3_drv_tci6486_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6486-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6486-evm PROFILE_c6xdsp=release
 
edma3_drv_tci6486be_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6486-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6486-evm PROFILE_c6xdsp=release ENDIAN=big
 
edma3_drv_ti816x_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti816x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti816x-evm PROFILE_c6xdsp=release

edma3_drv_ti814x_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti814x-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti814x-evm PROFILE_c6xdsp=release

edma3_drv_arm_ti814x_sample_clean:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti814x-evm CORE=a8host PROFILE_a8host=release
endif

edma3_drv_arm_ti816x_sample_clean:
ifeq ($(FORMAT),ELF)
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti816x-evm CORE=m3vpss PROFILE_m3vpss=release
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti816x-evm CORE=m3video PROFILE_m3video=release
endif

edma3_drv_ti816x_sim_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti816x-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=ti816x-sim PROFILE_c6xdsp=release
 
edma3_drv_tci6608_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6608-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6608-sim PROFILE_c6xdsp=release
 
edma3_drv_tci6608be_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6608-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6608-sim PROFILE_c6xdsp=release ENDIAN=big

edma3_drv_tci6616_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6616-sim PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6616-sim PROFILE_c6xdsp=release
 
edma3_drv_tci6616be_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6616-sim PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=tci6616-sim PROFILE_c6xdsp=release ENDIAN=big

edma3_drv_c6670_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6670-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6670-evm PROFILE_c6xdsp=release
 
edma3_drv_c6670be_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6670-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6670-evm PROFILE_c6xdsp=release ENDIAN=big

edma3_drv_c6678_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6678-evm PROFILE_c6xdsp=debug
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6678-evm PROFILE_c6xdsp=release
 
edma3_drv_c6678be_sample_clean:
	$(ECHO) \# Cleaning example $@:debug
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6678-evm PROFILE_c6xdsp=debug ENDIAN=big
	$(ECHO) \# Cleaning example $@:release
	$(MAKE) -C $($(subst _clean,,$@)_EXAMPLES_PATH) clean  PLATFORM=c6678-evm PROFILE_c6xdsp=release ENDIAN=big

      
# Help
help:
	$(ECHO) EDMA3 LLD Driver Help page
	$(ECHO) To build libraries and examples:
	$(ECHO) "Command line: make [all\|libs\|examples] [FORMAT=\{ELF\|COFF\}]"
	$(ECHO) "	Default target is "all", which builds both libraries and examples"
	$(ECHO) "	Default FORMAT is specified in makerules/build_config.mk. This can be"
	$(ECHO) "	 overridden by specifying a different FORMAT here."
	$(ECHO) To clean the build files:
	$(ECHO) "Command line: make \{clean\|cleanall\}"


# Nothing beyond this point
