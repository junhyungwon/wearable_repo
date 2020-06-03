#   
#   @file   Makefile.inc
#
#   @brief  Base makefile for building the kernel-side of SysLink.
#           This uses Kernel build (KBuild) system.
#
#
#   ============================================================================
#
#   Copyright (c) 2008-2012, Texas Instruments Incorporated
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#   
#   *  Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#   
#   *  Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#   
#   *  Neither the name of Texas Instruments Incorporated nor the names of
#      its contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#   
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#   OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#   Contact information for paper mail:
#   Texas Instruments
#   Post Office Box 655303
#   Dallas, Texas 75265
#   Contact information: 
#   http://www-k.ext.ti.com/sc/technical-support/product-information-centers.htm?
#   DCMP=TIHomeTracking&HQS=Other+OT+home_d_contact
#   ============================================================================
#   
# ---------------------------------------------------------------------------- #
# Defines: App build can override.                                             #
# ---------------------------------------------------------------------------- #
# Platform defines
ifeq ("$(SYSLINK_PLATFORM)", "")
# Default value
SYSLINK_PLATFORM := TI81XX
endif # ifeq ("$(SYSLINK_PLATFORM)", "")
ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")
COMPILE_FLAGS += -DSYSLINK_PLATFORM_OMAP3530
endif # ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")
ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")
COMPILE_FLAGS += -DSYSLINK_PLATFORM_TI81XX

#If being built for EZSDK, this define needs to be provided to the build
#system.
ifeq ("$(SYSLINK_SDK)", "EZSDK")
COMPILE_FLAGS += -DSYSLINK_SDK_EZSDK
endif # ifeq ("$(SYSLINK_SDK)", "EZSDK")
ifeq ("$(SYSLINK_VARIANT)", "")
#Default value
SYSLINK_VARIANT := TI816X
endif # ifeq ("$(SYSLINK_VARIANT)", "")
ifeq ("$(SYSLINK_VARIANT)", "TI816X")
COMPILE_FLAGS += -DSYSLINK_VARIANT_TI816X
endif # ifeq ("$(SYSLINK_VARIANT)", "TI816X")
ifeq ("$(SYSLINK_VARIANT)", "TI814X")
COMPILE_FLAGS += -DSYSLINK_VARIANT_TI814X
endif # ifeq ("$(SYSLINK_VARIANT)", "TI814X")
ifeq ("$(SYSLINK_VARIANT)", "TI813X")
COMPILE_FLAGS += -DSYSLINK_VARIANT_TI813X
endif # ifeq ("$(SYSLINK_VARIANT)", "TI813X")
ifeq ("$(SYSLINK_VARIANT)", "TI811X")
COMPILE_FLAGS += -DSYSLINK_VARIANT_TI811X
endif # ifeq ("$(SYSLINK_VARIANT)", "TI811X")
endif # ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")

ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")
COMPILE_FLAGS += -DSYSLINK_PLATFORM_OMAPL1XX
endif # ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")
ifeq ("$(SYSLINK_PLATFORM)", "C6474")
LOADER_DISABLE := 1
COMPILE_FLAGS += -DSYSLINK_PLATFORM_C6474
endif # ifeq ("$(SYSLINK_PLATFORM)", "C6474")
ifeq ("$(SYSLINK_PLATFORM)", "C6472")
LOADER_DISABLE := 1
COMPILE_FLAGS += -DSYSLINK_PLATFORM_C6472
endif # ifeq ("$(SYSLINK_PLATFORM)", "C6472")

# ---------------------------------------------------------------------------- #
# Export                                                                       #
# ---------------------------------------------------------------------------- #
MKDIR := mkdir -p
ifdef SYSLINK_VARIANT
SYSLINK_EXPORT := $(SYSLINK_ROOT)/ti/syslink/bin/$(SYSLINK_VARIANT)
SYSLINK_SAMPLE_EXPORT := $(SYSLINK_ROOT)/ti/syslink/bin/$(SYSLINK_VARIANT)/samples
else # ifdef SYSLINK_VARIANT
SYSLINK_EXPORT := $(SYSLINK_ROOT)/ti/syslink/bin/$(SYSLINK_PLATFORM)
SYSLINK_SAMPLE_EXPORT := $(SYSLINK_ROOT)/ti/syslink/bin/$(SYSLINK_PLATFORM)/samples
endif # ifdef SYSLINK_VARIANT

# Operating System
ifeq ("$(SYSLINK_BUILDOS)", "Linux")
SYSLINK_BUILDOS_LINUX := 1
endif # ifeq ("$(SYSLINK_BUILDOS_LINUX)", "")
ifeq ("$(SYSLINK_BUILDOS)", "Android")
SYSLINK_BUILDOS_LINUX := 1
endif # ifeq ("$(SYSLINK_BUILDOS_LINUX)", "")
ifeq ("$(SYSLINK_BUILDOS_LINUX)", "1")
COMPILE_FLAGS += -DSYSLINK_BUILDOS_LINUX
endif # ifeq ("$(SYSLINK_BUILDOS)", "Linux")
ifeq ("$(SYSLINK_BUILDOS)", "Qnx")
COMPILE_FLAGS += -DSYSLINK_BUILDOS_QNX
endif # ifeq ("$(SYSLINK_BUILDOS)", "Qnx")

# Optimize build flag
ifeq ("$(SYSLINK_BUILD_OPTIMIZE)", "")
SYSLINK_BUILD_OPTIMIZE :=
endif # ifeq ("$(SYSLINK_BUILD_OPTIMIZE)", "")
ifeq ("$(SYSLINK_BUILD_OPTIMIZE)", "1")
COMPILE_FLAGS += -DSYSLINK_BUILD_OPTIMIZE
endif # ifeq ("$(SYSLINK_BUILD_OPTIMIZE)", "1")

# Debug build
ifeq ("$(SYSLINK_BUILD_DEBUG)", "")
SYSLINK_BUILD_DEBUG := 1
endif # ifeq ("$(SYSLINK_BUILD_DEBUG)", "")
ifeq ("$(SYSLINK_BUILD_DEBUG)", "1")
COMPILE_FLAGS += -DSYSLINK_BUILD_DEBUG
endif # ifeq ("$(SYSLINK_BUILD_DEBUG)", "1")

# Trace enable
ifeq ("$(SYSLINK_TRACE_ENABLE)", "")
SYSLINK_TRACE_ENABLE := 1
endif # ifeq ("$(SYSLINK_TRACE_ENABLE)", "")
ifeq ("$(SYSLINK_TRACE_ENABLE)", "1")
COMPILE_FLAGS += -DSYSLINK_TRACE_ENABLE
endif # ifeq ("$(SYSLINK_TRACE_ENABLE)", "1")

# Loader Defines:  ELF or COFF

ifeq ("$(SYSLINK_LOADER)", "")
ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")
SYSLINK_LOADER := ELF
else # ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")
# Default value
SYSLINK_LOADER := COFF
endif
endif

ifeq ("$(SYSLINK_LOADER)", "ELF")
COMPILE_FLAGS += -DSYSLINK_LOADER_ELF
endif
ifeq ("$(SYSLINK_LOADER)", "COFF")
COMPILE_FLAGS += -DSYSLINK_LOADER_COFF
endif


# For DLOAD loader:
ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")
COMPILE_FLAGS += -DARM_TARGET  # Enable when ARM M3 supported
COMPILE_FLAGS += -DC60_TARGET  # Enable when ARM M3 supported
endif # ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")
ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")
COMPILE_FLAGS += -DC60_TARGET
endif # ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")
ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")
COMPILE_FLAGS += -DC60_TARGET
endif # ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")
ifeq ("$(SYSLINK_PLATFORM)", "C6474")
COMPILE_FLAGS += -DC60_TARGET
endif # ifeq ("$(SYSLINK_PLATFORM)", "C6474")
ifeq ("$(SYSLINK_PLATFORM)", "C6472")
COMPILE_FLAGS += -DC60_TARGET
-include $(KDIR)/source/arch/$(ARCH)/Makefile.lib
endif # ifeq ("$(SYSLINK_PLATFORM)", "C6472")
COMPILE_FLAGS += -DLOADER_DEBUG=1  # Enable debugging_on flag in ELF DLOAD
COMPILE_FLAGS += -DSYSLINK_BUILD_HLOS

# use syslink's own notify implementation for all non-Linux builds since support in knl
# is not there.  Linux will auto-detect.

ifeq ("$(USE_SYSLINK_NOTIFY)", "")
ifneq ("$(SYSLINK_BUILDOS_LINUX)", "1")
COMPILE_FLAGS += -DUSE_SYSLINK_NOTIFY
endif # ifneq ("$(SYSLINK_BUILDOS_LINUX)", "1")
endif # ifeq ("$(USE_SYSLINK_NOTIFY)", "")

ifeq ("$(USE_SYSLINK_NOTIFY)", "1")
COMPILE_FLAGS += -DUSE_SYSLINK_NOTIFY
endif # ifeq ("$(USE_SYSLINK_NOTIFY)", "1")

# Choose the Notify Driver to be used.
ifeq ("$(SYSLINK_NOTIFYDRIVER)", "NOTIFYDRIVERCIRC")
COMPILE_FLAGS += -DSYSLINK_NOTIFYDRIVER_CIRC -DUSE_SYSLINK_NOTIFY
endif # ifeq ("$(SYSLINK_NOTIFYDRIVER)", "NOTIFYDRIVERCIRC")

# Choose the MessageQ Transport to be used.
ifeq ("$(SYSLINK_TRANSPORT)", "TRANSPORTSHMNOTIFY")
COMPILE_FLAGS += -DSYSLINK_TRANSPORT_SHMNOTIFY
else # ifeq ("$(SYSLINK_TRANSPORT)", "TRANSPORTSHMNOTIFY")
ifeq ("$(SYSLINK_TRANSPORT)", "TRANSPORTSHMCIRC")
COMPILE_FLAGS += -DSYSLINK_TRANSPORT_SHMCIRC
endif # ifeq ("$(SYSLINK_TRANSPORT)", "TRANSPORTSHMCIRC")
endif # ifeq ("$(SYSLINK_TRANSPORT)", "TRANSPORTSHMNOTIFY")


# ---------------------------------------------------------------------------- #
# Kernel dir: App build can override.                                          #
# ---------------------------------------------------------------------------- #
ifeq ("$(SYSLINK_BUILDOS)", "Linux")

ifeq ("$(KDIR)", "")
ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")
KDIR := /opt/DSPLINK/OMAP3530/AM35x-OMAP35x-PSP-SDK-03.00.00.03/src/kernel/linux-03.00.00.03
endif # ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")

ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")
ifeq ("$(SYSLINK_VARIANT)", "TI814X")
KDIR := /opt/DSPLINK/TI814X-LINUX-PSP-04.01.00.03/src/kernel/linux-04.01.00.03
endif
ifeq ("$(SYSLINK_VARIANT)", "TI816X")
KDIR := /opt/DSPLINK/TI816X-LINUX-PSP-04.00.00.10/src/kernel/linux-04.00.00.10
endif
ifeq ("$(SYSLINK_VARIANT)", "TI813X")
KDIR := /opt/DSPLINK/TI813X-LINUX-PSP-04.04.00.01/src/kernel/linux-04.04.00.01
endif
ifeq ("$(SYSLINK_VARIANT)", "TI811X")
KDIR := /opt/DSPLINK/TI811X-LINUX-PSP-04.04.00.01/src/kernel/linux-04.04.00.01
endif
endif # ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")

ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")
KDIR := /opt/DSPLINK/OMAPL1XX/DaVinci-PSP-SDK-03.20.00.11/src/kernel/linux-03.20.00.11
endif # ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")

ifeq ("$(SYSLINK_PLATFORM)", "C6472")
KDIR := /opt/DSPLINK/ecn_linux_c6x/Build/kobjs/evmc6472.el
endif # ifeq ("$(SYSLINK_PLATFORM)", "C6472")

ifeq ("$(SYSLINK_PLATFORM)", "C6474")
KDIR := /opt/DSPLINK/ecn_linux_c6x/Build/kobjs/evmc6474.el
endif # ifeq ("$(SYSLINK_PLATFORM)", "C6474")
endif # ifeq ("$(KDIR)", "")

endif # ifeq ("$(SYSLINK_BUILDOS)", "Linux")

# ---------------------------------------------------------------------------- #
# Include paths                                                                #
# ---------------------------------------------------------------------------- #
# the IPC product include required the same directory structure for minimal or no code changes
ifeq ("$(SYSLINK_BUILDOS)", "Linux")
COMPILE_FLAGS += -I$(KDIR)

#  This is needed for Android (should already be defined for Linux)
COMPILE_FLAGS += -D__linux
endif # ifeq ("$(SYSLINK_BUILDOS)", "Linux")

ifeq ("$(SYSLINK_BUILDOS)", "Qnx")
endif # ifeq ("$(SYSLINK_BUILDOS)", "Qnx")

COMPILE_FLAGS += -I$(SYSLINK_ROOT)
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/inc
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/inc/knl
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/inc/knl/$(SYSLINK_BUILDOS)
COMPILE_FLAGS += $(addprefix -I,$(subst ;, ,$(SYSLINK_PKGPATH)))

# Internal headers for DLOAD library. Keeping them here to ensure that the entire dload package
# can be drop-in.
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_DYN
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_REL
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/TMS470_DLOAD_DYN
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/TMS470_DLOAD_REL
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/DLOAD
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/DLOAD_API
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/DLOAD_SYM
COMPILE_FLAGS += -I$(SYSLINK_ROOT)/ti/syslink/procMgr/common/loaders/Elf/dlw_client

# ---------------------------------------------------------------------------- #
# Defines                                                                      #
# ---------------------------------------------------------------------------- #
EXTRA_CFLAGS += $(COMPILE_FLAGS)

# ---------------------------------------------------------------------------- #
# Objects                                                                      #
# Change the relative path enviorment according to sample directory location   #
# ---------------------------------------------------------------------------- #
# For SysLink build, include all SysLink makefiles.
ifeq ("$(SYSLINK_BUILD)", "1")

#ClockOps
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/common/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/common/,$(OBJECTS))

# ProcMgr
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/hlos/knl/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/hlos/knl/Linux/,$(OBJECTS))

# loaders
ifneq ("$(LOADER_DISABLE)", "1")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Linux/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Coff/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Coff/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/DLOAD/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/DLOAD/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/DLOAD_SYM/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/DLOAD_SYM/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/dlw_client/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/dlw_client/,$(OBJECTS))
endif # ifeq ("$(LOADER_DISABLE)", "1")

# OMAP3530 Slave

ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_DYN/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_DYN/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_REL/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_REL/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omap3530/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omap3530/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omap3530/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omap3530/Linux/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omap3530/omap3530dsp/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omap3530/omap3530dsp/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omap3530/omap3530dsp/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omap3530/omap3530dsp/Linux/,$(OBJECTS))
endif # ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")

# TI81XX Slave

ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/Linux/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/TMS470_DLOAD_DYN/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/TMS470_DLOAD_DYN/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/TMS470_DLOAD_REL/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/TMS470_DLOAD_REL/,$(OBJECTS))

-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_DYN/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_DYN/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_REL/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_REL/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/,$(OBJECTS))

ifneq ("$(SYSLINK_VARIANT)", "TI813X")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxdsp/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxdsp/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/common/ti81xx/ti81xxdsp/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/common/ti81xx/ti81xxdsp/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxdsp/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxdsp/Linux/,$(OBJECTS))
endif #ifneq ("$(SYSLINK_VARIANT)", "TI813X")

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/common/ti81xx/ti81xxducati/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/common/ti81xx/ti81xxducati/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/Linux/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/ti81xxvideom3/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/ti81xxvideom3/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/common/ti81xx/ti81xxducati/ti81xxvideom3/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/common/ti81xx/ti81xxducati/ti81xxvideom3/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/ti81xxvideom3/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/ti81xxvideom3/Linux/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/ti81xxvpssm3/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/ti81xxvpssm3/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/common/ti81xx/ti81xxducati/ti81xxvpssm3/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/common/ti81xx/ti81xxducati/ti81xxvpssm3/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/ti81xxvpssm3/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/ti81xxducati/ti81xxvpssm3/Linux/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/ti81xx/Linux/,$(OBJECTS))

endif # ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")

# OMAPL1XX Slave

ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_DYN/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_DYN/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_REL/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/procMgr/common/loaders/Elf/DLOAD/C60_DLOAD_REL/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omapl1xx/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omapl1xx/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omapl1xx/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omapl1xx/Linux/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omapl1xx/omapl1xxdsp/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omapl1xx/omapl1xxdsp/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omapl1xx/omapl1xxdsp/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/omapl1xx/omapl1xxdsp/Linux/,$(OBJECTS))
endif # ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")

# C6474 Slave

ifeq ("$(SYSLINK_PLATFORM)", "C6474")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6474/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6474/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6474/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6474/Linux/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6474/core0/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6474/core0/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6474/core1/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6474/core1/,$(OBJECTS))
endif # ifeq ("$(SYSLINK_PLATFORM)", "C6474")

# C6472 Slave

ifeq ("$(SYSLINK_PLATFORM)", "C6472")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/Linux/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/core0/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/core0/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/core1/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/core1/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/core2/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/core2/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/core3/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/core3/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/core4/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/family/hlos/knl/c6472/core4/,$(OBJECTS))
endif # ifeq ("$(SYSLINK_PLATFORM)", "C6472")

# Utils

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/utils/common/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/utils/common/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/utils/hlos/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/utils/hlos/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/utils/hlos/knl/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/utils/hlos/knl/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/utils/hlos/knl/osal/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/utils/hlos/knl/osal/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/utils/hlos/knl/osal/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/utils/hlos/knl/osal/Linux/,$(OBJECTS))

# SysMemMgr
#OBJECTS :=
#-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/sysMemMgr/hlos/knl/Makefile.inc
#$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/sysMemMgr/hlos/knl/,$(OBJECTS))
#OBJECTS :=
#-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/sysMemMgr/hlos/knl/Linux/Makefile.inc
#$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/sysMemMgr/hlos/knl/Linux/,$(OBJECTS))

# IPC

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/Linux/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/transports/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/transports/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/shMem/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/shMem/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/cfg/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/cfg/,$(OBJECTS))


ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/omap3530/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/omap3530/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/omap3530/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/omap3530/Linux/,$(OBJECTS))
endif #ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")

ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/ti81xx/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/ti81xx/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/ti81xx/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/ti81xx/Linux/,$(OBJECTS))
endif #ifeq ("$(SYSLINK_PLATFORM)", "TI81XX")

ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/omapl1xx/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/omapl1xx/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/omapl1xx/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/omapl1xx/Linux/,$(OBJECTS))
endif #ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")

ifeq ("$(SYSLINK_PLATFORM)", "C6474")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/c6474/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/c6474/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/c6474/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/c6474/Linux/,$(OBJECTS))
endif #ifeq ("$(SYSLINK_PLATFORM)", "C6474")

ifeq ("$(SYSLINK_PLATFORM)", "C6472")
OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/c6472/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/c6472/,$(OBJECTS))

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/c6472/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/notifyDrivers/arch/c6472/Linux/,$(OBJECTS))
endif #ifeq ("$(SYSLINK_PLATFORM)", "C6472")

OBJECTS :=
-include $(obj)/$(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/Linux/Makefile.inc
$(MODULE)-objs += $(addprefix $(SYSLINK_RELATIVE_PATH)/ti/syslink/ipc/hlos/knl/Linux/,$(OBJECTS))

# List all base paths to clean intermediate files

SYSLINK_BUILD_PATH := $(SYSLINK_ROOT)/ti/syslink/cfg     \
                      $(SYSLINK_ROOT)/ti/syslink/family  \
                      $(SYSLINK_ROOT)/ti/syslink/ipc     \
                      $(SYSLINK_ROOT)/ti/syslink/procMgr \
                      $(SYSLINK_ROOT)/ti/syslink/utils

else #Not SysLink build

ifneq ($(wildcard $(SYSLINK_ROOT)/ti/syslink/ipc/hlos/knl/Linux/Module.symvers),)
EXTRA_SYMBOLS = $(SYSLINK_ROOT)/ti/syslink/ipc/hlos/knl/Linux/Module.symvers
endif
ifneq ($(wildcard $(SYSLINK_ROOT)/ti/syslink/ipc/hlos/knl/notifyDrivers/Linux/Module.symvers),)
EXTRA_SYMBOLS = $(SYSLINK_ROOT)/ti/syslink/ipc/hlos/knl/notifyDrivers/Linux/Module.symvers
endif
ifneq ($(wildcard $(SYSLINK_ROOT)/ti/syslink/utils/hlos/knl/Linux/Module.symvers),)
EXTRA_SYMBOLS = $(SYSLINK_ROOT)/ti/syslink/utils/hlos/knl/Linux/Module.symvers
endif
ifneq ($(wildcard $(SYSLINK_ROOT)/ti/syslink/procMgr/hlos/knl/Linux/Module.symvers),)
EXTRA_SYMBOLS = $(SYSLINK_ROOT)/ti/syslink/procMgr/hlos/knl/Linux/Module.symvers
endif
ifneq ($(wildcard $(SYSLINK_ROOT)/ti/syslink/procMgr/common/loaders/Linux/Module.symvers),)
EXTRA_SYMBOLS = $(SYSLINK_ROOT)/ti/syslink/procMgr/common/loaders/Linux/Module.symvers
endif
ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")
ifneq ($(wildcard $(SYSLINK_ROOT)/ti/syslink/procMgr/hlos/knl/omap3530dsp/Linux/Module.symvers),)
EXTRA_SYMBOLS = $(SYSLINK_ROOT)/ti/syslink/procMgr/hlos/knl/omap3530dsp/Linux/Module.symvers
endif
endif # ifeq ("$(SYSLINK_PLATFORM)", "OMAP3530")
ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")
ifneq ($(wildcard $(SYSLINK_ROOT)/ti/syslink/procMgr/hlos/knl/omapl1xxdsp/Linux/Module.symvers),)
EXTRA_SYMBOLS = $(SYSLINK_ROOT)/ti/syslink/procMgr/hlos/knl/omapl1xxdsp/Linux/Module.symvers
endif
endif # ifeq ("$(SYSLINK_PLATFORM)", "OMAPL1XX")

endif #SysLink build


# ---------------------------------------------------------------------------- #
# Targets                                                                      #
# ---------------------------------------------------------------------------- #
ifneq ($(KERNELRELEASE),)
obj-m := $(MODULE).o
else
PWD  := $(shell pwd)
default:
ifneq ($(EXTRA_SYMBOLS),)
	rm -f Module.symvers
	cat $(foreach file, $(EXTRA_SYMBOLS), $(file)) >> Module.symvers
endif
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	@echo "-------------------------------------------------------------------------"
	@echo "Exporting the $(MODULE).ko:"
ifeq ("$(SYSLINK_BUILD)", "1")
	@echo "$(SYSLINK_EXPORT)"
	@$(MKDIR) $(SYSLINK_EXPORT)
	@cp $(MODULE).ko $(SYSLINK_EXPORT)/
else
	@echo "$(SYSLINK_SAMPLE_EXPORT)"
	@$(MKDIR) $(SYSLINK_SAMPLE_EXPORT)
	@cp $(MODULE).ko $(SYSLINK_SAMPLE_EXPORT)/
endif

endif

cleanall: clean
ifeq ("$(SYSLINK_BUILD)", "1")
	@echo Cleaning... $(SYSLINK_EXPORT)
	find $(SYSLINK_EXPORT) -name "$(MODULE).ko" -exec rm {} \;
else #ifeq ("$(SYSLINK_BUILD)", "1")
	@echo Cleaning... $(SYSLINK_SAMPLE_EXPORT)
	find $(SYSLINK_SAMPLE_EXPORT) -name "$(MODULE).ko" -exec rm {} \;
endif #ifeq ("$(SYSLINK_BUILD)", "1")

clean:
ifeq ("$(SYSLINK_BUILD)", "1")
	@echo Cleaning... only @ $(SYSLINK_ROOT)/ti/syslink
	@rm -rf $(SYSLINK_ROOT)/ti/syslink/*.o
	@rm -rf $(SYSLINK_ROOT)/ti/syslink/.*.o.cmd
	@echo    # To create a new line
	@echo Cleaning... @ $(SYSLINK_BUILD_PATH)
	@find $(SYSLINK_BUILD_PATH) -name "*.o" -exec rm {} \;
	@find $(SYSLINK_BUILD_PATH) -name "*.ko" -exec rm {} \;
	@find $(SYSLINK_BUILD_PATH) -name ".*.o.cmd" -exec rm {} \;
	@find $(SYSLINK_BUILD_PATH) -name ".*.ko.cmd" -exec rm {} \;
	@find $(SYSLINK_BUILD_PATH) -name "*.mod.c" -exec rm {} \;
	@find $(SYSLINK_BUILD_PATH) -name "Module.symvers" -exec rm {} \;
	@find $(SYSLINK_BUILD_PATH) -name "modules.order" -exec rm {} \;
	@find $(SYSLINK_BUILD_PATH) -name ".tmp_versions" -prune -exec rm -rf {} \;
	@echo    # To create a new line
else #ifeq ("$(SYSLINK_BUILD)", "1")
	@echo Cleaning... @ $(SAMPLE_KNL_PATH)
	@find $(SAMPLE_KNL_PATH) -name "*.o" -exec rm {} \;
	@find $(SAMPLE_KNL_PATH) -name "*.ko" -exec rm {} \;
	@find $(SAMPLE_KNL_PATH) -name ".*.o.cmd" -exec rm {} \;
	@find $(SAMPLE_KNL_PATH) -name ".*.ko.cmd" -exec rm {} \;
	@find $(SAMPLE_KNL_PATH) -name "*.mod.c" -exec rm {} \;
	@find $(SAMPLE_KNL_PATH) -name "Module.symvers" -exec rm {} \;
	@find $(SAMPLE_KNL_PATH) -name "modules.order" -exec rm {} \;
	@find $(SAMPLE_KNL_PATH) -name ".tmp_versions" -prune -exec rm -rf {} \;
	@echo    # To create a new line
endif #ifeq ("$(SYSLINK_BUILD)", "1")
