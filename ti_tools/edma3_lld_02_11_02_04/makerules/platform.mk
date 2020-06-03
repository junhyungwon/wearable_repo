# Filename: platforms.mk
#
# Platforms make file - Platform/SoC/targets are defined/derived in this file
# 
# This file needs to change when:
#     1. a new platform/SoC is added, which also might have its own cores/ISAs
#

#
# Derive SOC from PLATFORM
#

# ti816x (Netra) catalog EVM
ifeq ($(PLATFORM),ti816x-evm)
 SOC = ti816x
 ifeq ($(CORE),c6xdsp)
  PLATFORM_XDC = "ti.platforms.evmDM8168"
 endif
endif

# ti816x (Netra) Simulator
ifeq ($(PLATFORM),ti816x-sim)
 SOC = ti816x
 ifeq ($(CORE),c6xdsp)
  PLATFORM_XDC = "ti.platforms.simDM8168"
 endif
endif

# ti814x (Centaurus) Catalog EVM
ifeq ($(PLATFORM),ti814x-evm)
 SOC = ti814x
 ifeq ($(CORE),c6xdsp)
  PLATFORM_XDC = "ti.platforms.evmDM8148"
 endif
 ifeq ($(CORE),a8host)
  PLATFORM_XDC = "ti.platforms.evmDM8148"
 endif
endif

# c6472 (Tomahawk) EVM
ifeq ($(PLATFORM),c6472-evm)
 SOC = c6472
 PLATFORM_XDC = "ti.platforms.evm6472"
endif

# tci6486 (Tomahawk) EVM
ifeq ($(PLATFORM),tci6486-evm)
 SOC = c6472
 PLATFORM_XDC = "ti.platforms.evmTCI6486"
endif

# tci6608 (Shannon) Simulator 
ifeq ($(PLATFORM),tci6608-sim)
 SOC = c6608
 PLATFORM_XDC = "ti.platforms.simTCI6608"
endif

# tci6616 (Nyquist) Simulator 
ifeq ($(PLATFORM),tci6616-sim)
 SOC = c6616
 PLATFORM_XDC = "ti.platforms.simTCI6616"
endif

# c6670 (Nyquist) EVM
ifeq ($(PLATFORM),c6670-evm)
 SOC = c6670
 PLATFORM_XDC = "ti.platforms.evm6670"
endif

# c6678 (Shannon) EVM
ifeq ($(PLATFORM),c6678-evm)
 SOC = c6678
 PLATFORM_XDC = "ti.platforms.evm6678"
endif


# omapl138 (Freon) EVM
ifeq ($(PLATFORM),omapl138-evm)
 SOC = omapl138
 PLATFORM_XDC = "ti.platforms.evmOMAPL138"
endif

# c6748 (Freon) EVM
ifeq ($(PLATFORM),c6748-evm)
 SOC = omapl138
 PLATFORM_XDC = "ti.platforms.evm6748"
endif

# da830 (Primus) EVM
ifeq ($(PLATFORM),da830-evm)
 SOC = omapl137
 PLATFORM_XDC = "ti.platforms.evmDA830"
endif

# omap4 EVM
ifeq ($(PLATFORM),omap4-evm)
 SOC = omap4
 PLATFORM_XDC = "ti.platforms.sdp4430"
endif

# Generic platform
ifeq ($(PLATFORM),generic)
 SOC = generic
 PLATFORM_XDC = 
endif

# Derive Target/ISA from CORE

# m3vpss
ifeq ($(CORE),m3vpss)
 ISA = m3
endif

# m3video
ifeq ($(CORE),m3video)
 ISA = m3
endif

# a8host
ifeq ($(CORE),a8host)
 ISA = a8
endif

# arm9
ifeq ($(CORE),arm9)
 ISA = arm9
endif

# c6xdsp
ifeq ($(CORE),c6xdsp)
 ifeq ($(SOC),ti816x)
  ISA = 674
 endif
 ifeq ($(SOC),ti814x)
  ISA = 674
 endif
 ifeq ($(SOC),omapl137)
  ISA = 674
 endif
 ifeq ($(SOC),omapl138)
  ISA = 674
 endif
 ifeq ($(SOC),c6472)
  ISA = 64p
 endif
 ifeq ($(SOC),c6608)
  ISA = 66
 endif
 ifeq ($(SOC),c6616)
  ISA = 66
 endif
 ifeq ($(SOC),c6670)
  ISA = 66
 endif
 ifeq ($(SOC),c6678)
  ISA = 66
 endif
 ifeq ($(SOC),omap4)
  ISA = 64t
 endif
endif

# Platform and SOC is generic; use the core name same as ISA (eg: 674)
ifeq ($(SOC),generic)
  ISA = $(CORE)
endif

#
# Derive XDC/ISA specific settings 
#

ifeq ($(ISA),m3)
  ifeq ($(FORMAT),ELF)
    TARGET_XDC = ti.targets.arm.elf.M3
    FORMAT_EXT = e
  else
    TARGET_XDC = ti.targets.arm.M3
  endif
  ifeq ($(CORE),m3video)
    PLATFORM_XDC = "ti.platforms.evmDM8168"
  else
    PLATFORM_XDC = "ti.platforms.evmDM8168"
  endif
  
  # If ENDIAN is set to "big", set ENDIAN_EXT to "e", that would be used in
  #    in the filename extension of object/library/executable files
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
  LIBEXT = a$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
endif

ifeq ($(ISA),674)
  ifeq ($(FORMAT),ELF)
    TARGET_XDC = ti.targets.elf.C674
    FORMAT_EXT = e
  else
    TARGET_XDC = ti.targets.C674
  endif
  
  # If ENDIAN is set to "big", set ENDIAN_EXT to "e", that would be used in
  #    in the filename extension of object/library/executable files
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
  LIBEXT = a$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
endif

ifeq ($(ISA),64p)
  ifeq ($(FORMAT),ELF)
   ifeq ($(ENDIAN),big)
    TARGET_XDC = ti.targets.elf.C64P_big_endian
   else
    TARGET_XDC = ti.targets.elf.C64P
   endif
    FORMAT_EXT = e
  else
   ifeq ($(ENDIAN),big)
    TARGET_XDC = ti.targets.C64P_big_endian
   else
    TARGET_XDC = ti.targets.C64P
   endif
  endif
  
  # If ENDIAN is set to "big", set ENDIAN_EXT to "e", that would be used in
  #    in the filename extension of object/library/executable files
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)64P$(ENDIAN_EXT)
  LIBEXT = a$(FORMAT_EXT)64P$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)64P$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)64P$(ENDIAN_EXT)
endif

ifeq ($(ISA),64t)
  ifeq ($(FORMAT),ELF)
   ifeq ($(ENDIAN),big)
    TARGET_XDC = ti.targets.elf.C64T_big_endian
   else
    TARGET_XDC = ti.targets.elf.C64T
   endif
    FORMAT_EXT = e
  else
   ifeq ($(ENDIAN),big)
    TARGET_XDC = ti.targets.C64T_big_endian
   else
    TARGET_XDC = ti.targets.C64T
   endif
  endif
  
  # If ENDIAN is set to "big", set ENDIAN_EXT to "e", that would be used in
  #    in the filename extension of object/library/executable files
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)64T$(ENDIAN_EXT)
  LIBEXT = a$(FORMAT_EXT)64T$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)64T$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)64T$(ENDIAN_EXT)
endif

ifeq ($(ISA),66)
  ifeq ($(FORMAT),ELF)
   ifeq ($(ENDIAN),big)
    TARGET_XDC = ti.targets.elf.C66_big_endian
   else
    TARGET_XDC = ti.targets.elf.C66
   endif
    FORMAT_EXT = e
  else
   ifeq ($(ENDIAN),big)
    TARGET_XDC = ti.targets.C66_big_endian
   else
    TARGET_XDC = ti.targets.C66
   endif
  endif
  
  # If ENDIAN is set to "big", set ENDIAN_EXT to "e", that would be used in
  #    in the filename extension of object/library/executable files
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
  LIBEXT = a$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)$(ISA)$(ENDIAN_EXT)

endif

ifeq ($(ISA),a8)
  ifeq ($(TOOLCHAIN_a8),TI)
  TARGET_XDC = ti.targets.arm.elf.A8F
  else
  TARGET_XDC = gnu.targets.arm.GCArmv5T
  PLATFORM_XDC = "host.platforms.arm"
  endif
  ifeq ($(FORMAT),ELF)
    FORMAT_EXT = e
  endif
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)a8f$(ENDIAN_EXT)
  LIBEXT = a$(FORMAT_EXT)a8f$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)a8f$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)a8f$(ENDIAN_EXT)
endif

ifeq ($(ISA),arm9)
  TARGET_XDC = ti.targets.arm.elf.Arm9
  ifeq ($(FORMAT),ELF)
    FORMAT_EXT = e
  endif
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)9$(ENDIAN_EXT)
  LIBEXT = a$(FORMAT_EXT)9$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)9$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)9$(ENDIAN_EXT)
endif

ifeq (sim,$(findstring sim,$(PLATFORM)))
  OMX_PLATFORM = sim
endif

ifeq (ti816x-evm,$(findstring ti816x-evm,$(PLATFORM)))
 # Don't know why platform define for XDC is separate for A8. TO DO.
 ifeq ($(CORE),a8host)
  OMX_PLATFORM = evmSi
 else
  OMX_PLATFORM = evmSi816X
 endif
endif

ifeq ($(CORE),m3vpss)
  CFGARGS_XDC = \"{mode: \\\"$(IPC_MODE)\\\", coreName: \\\"VPSS-M3\\\", tiler: \\\"DucatiTilerMemMgr\\\", platformMem: \\\"$(PLATFORM_MEM)\\\", maxResolution: \\\"$(MAX_RESOLUTION)\\\", omxBuildPlatform: \\\"$(OMX_PLATFORM)\\\"}\"
endif
ifeq ($(CORE),m3video)
  CFGARGS_XDC = \"{mode: \\\"$(IPC_MODE)\\\", coreName:\\\"VIDEO-M3\\\", tiler: \\\"DucatiTilerMemMgr\\\", platformMem: \\\"$(PLATFORM_MEM)\\\", maxResolution: \\\"$(MAX_RESOLUTION)\\\", omxBuildPlatform: \\\"$(OMX_PLATFORM)\\\"}\"
endif
ifeq ($(CORE),c6xdsp)
  CFGARGS_XDC = 
endif
ifeq ($(CORE),a8host)
  CFGARGS_XDC = \"{mode: \\\"$(IPC_MODE)\\\", coreName:\\\"HOST\\\", tiler: \\\"NullTilerMemMgr\\\", platformMem: \\\"$(PLATFORM_MEM)\\\", maxResolution: \\\"$(MAX_RESOLUTION)\\\", omxBuildPlatform: \\\"$(OMX_PLATFORM)\\\"}\"
endif

# Nothing beyond this point
