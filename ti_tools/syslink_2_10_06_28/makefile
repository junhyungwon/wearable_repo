#   
#   @file   makefile
#
#   @brief  Base makefile
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

# check for make-3.81 or higher
need := 3.81
ok := $(filter $(need),$(firstword $(sort $(MAKE_VERSION) $(need))))
ifeq ($(ok), )
    $(error GNU Make version is "$(MAKE_VERSION)". Version $(need) or greater is required)
endif

include products.mak

#if FC_INSTALL_DIR is set, add it to TI81XX builds (for tiler support)
ifneq ("$(FC_INSTALL_DIR)", "")
FC_IFPRESENT = "FC_INSTALL_DIR=\"$(FC_INSTALL_DIR)\""
endif

#  ======== all ========
.PHONY: syslink syslink-driver syslink-hlos syslink-rtos
.PHONY: samples samples-hlos samples-rtos examples
.PHONY: install install-driver install-samples
.PHONY: all clean help

all: syslink

ifeq ("$(GPPOS)","Bios")
syslink: syslink-rtos

samples: samples-rtos
else
syslink: syslink-driver
syslink: syslink-hlos
syslink: syslink-rtos

samples: samples-hlos samples-rtos
endif

install: install-driver
install: install-samples

syslink-driver:
	@echo "#"
	@echo "# Making $@..."
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            CGT_ARM_PREFIX=$(CGT_ARM_PREFIX) \
            LINUXKERNEL=$(LINUXKERNEL) \
            QNX_INSTALL_DIR=$(QNX_INSTALL_DIR) \
            SYSLINK_BUILD_DEBUG=$(SYSLINK_BUILD_DEBUG) \
            SYSLINK_BUILD_OPTIMIZE=$(SYSLINK_BUILD_OPTIMIZE) \
            SYSLINK_TRACE_ENABLE=$(SYSLINK_TRACE_ENABLE) \
            $@

syslink-hlos:
	@echo "#"
	@echo "# Making $@..."
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            CGT_ARM_PREFIX=$(CGT_ARM_PREFIX) \
            QNX_INSTALL_DIR=$(QNX_INSTALL_DIR) \
            ANDROID_DIR=$(ANDROID_DIR) \
            BIONIC=$(BIONIC) \
            $@

syslink-rtos:
	@echo "#"
	@echo "# Making $@..."
ifeq ("$(DEVICE)","OMAP3530")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            CGT_C64P_INSTALL_DIR=$(CGT_C64P_INSTALL_DIR) \
            CGT_C64P_ELF_INSTALL_DIR=$(CGT_C64P_ELF_INSTALL_DIR) \
            $@
else ifeq ("$(DEVICE)","TI816X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            TI81XXDSP_DMTIMER_FREQ=$(TI81XXDSP_DMTIMER_FREQ) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
	    $(FC_IFPRESENT) \
            CGT_C674_ELF_INSTALL_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
            CGT_M3_ELF_INSTALL_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
            $@
else ifeq ("$(DEVICE)","TI814X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            TI81XXDSP_DMTIMER_FREQ=$(TI81XXDSP_DMTIMER_FREQ) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
	    $(FC_IFPRESENT) \
            CGT_C674_ELF_INSTALL_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
            CGT_M3_ELF_INSTALL_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
            CGT_A8_ELF_INSTALL_DIR=$(CGT_A8_ELF_INSTALL_DIR) \
            $@
else ifeq ("$(DEVICE)","TI813X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
	    $(FC_IFPRESENT) \
            CGT_M3_ELF_INSTALL_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
            $@
else ifeq ("$(DEVICE)","TI811X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            TI81XXDSP_DMTIMER_FREQ=$(TI81XXDSP_DMTIMER_FREQ) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
	    $(FC_IFPRESENT) \
            CGT_C674_ELF_INSTALL_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
            CGT_M3_ELF_INSTALL_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
            $@
else ifeq ("$(DEVICE)","OMAPL1XX")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            CGT_C674_INSTALL_DIR=$(CGT_C674_INSTALL_DIR) \
            CGT_C674_ELF_INSTALL_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
            $@
endif

samples-hlos:
	@echo "#"
	@echo "# Making $@..."
ifeq ("$(DEVICE)","OMAP3530")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            CGT_ARM_PREFIX=$(CGT_ARM_PREFIX) \
            syslink-samples-hlos
else ifeq ("$(DEVICE)","TI816X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            CGT_ARM_PREFIX=$(CGT_ARM_PREFIX) \
            syslink-samples-hlos
else ifeq ("$(DEVICE)","TI814X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            CGT_ARM_PREFIX=$(CGT_ARM_PREFIX) \
            QNX_INSTALL_DIR=$(QNX_INSTALL_DIR) \
	    ANDROID_DIR=$(ANDROID_DIR) \
	    BIONIC=$(BIONIC) \
	    BIONIC_TARGETOBJS=$(BIONIC_TARGETOBJS) \
	    BIONICTOOL_DIR=$(BIONICTOOL_DIR) \
	    BIONIC_MISC=$(BIONIC_MISC) \
            syslink-samples-hlos
else ifeq ("$(DEVICE)","TI813X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            CGT_ARM_PREFIX=$(CGT_ARM_PREFIX) \
            syslink-samples-hlos
else ifeq ("$(DEVICE)","TI811X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            CGT_ARM_PREFIX=$(CGT_ARM_PREFIX) \
            QNX_INSTALL_DIR=$(QNX_INSTALL_DIR) \
            syslink-samples-hlos
else ifeq ("$(DEVICE)","OMAPL1XX")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            CGT_ARM_PREFIX=$(CGT_ARM_PREFIX) \
            syslink-samples-hlos
endif

samples-rtos:
	@echo "#"
	@echo "# Making $@..."
ifeq ("$(DEVICE)","OMAP3530")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            CGT_C64P_ELF_INSTALL_DIR=$(CGT_C64P_ELF_INSTALL_DIR) \
            CGT_C64P_INSTALL_DIR=$(CGT_C64P_INSTALL_DIR) \
            syslink-samples-rtos
else ifeq ("$(DEVICE)","TI816X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            TI81XXDSP_DMTIMER_FREQ=$(TI81XXDSP_DMTIMER_FREQ) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            CGT_C674_ELF_INSTALL_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
            CGT_M3_ELF_INSTALL_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
            syslink-samples-rtos
else ifeq ("$(DEVICE)","TI814X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            TI81XXDSP_DMTIMER_FREQ=$(TI81XXDSP_DMTIMER_FREQ) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            CGT_C674_ELF_INSTALL_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
            CGT_M3_ELF_INSTALL_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
            CGT_A8_ELF_INSTALL_DIR=$(CGT_A8_ELF_INSTALL_DIR) \
            syslink-samples-rtos
else ifeq ("$(DEVICE)","TI813X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            CGT_M3_ELF_INSTALL_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
            syslink-samples-rtos
else ifeq ("$(DEVICE)","TI811X")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SDK=$(SDK) \
            TI81XXDSP_DMTIMER_FREQ=$(TI81XXDSP_DMTIMER_FREQ) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            CGT_C674_ELF_INSTALL_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
            CGT_M3_ELF_INSTALL_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
            syslink-samples-rtos
else ifeq ("$(DEVICE)","OMAPL1XX")
	$(MAKE) -C packages  \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            LOADER=$(LOADER) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            IPC_INSTALL_DIR=$(IPC_INSTALL_DIR) \
            BIOS_INSTALL_DIR=$(BIOS_INSTALL_DIR) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            CGT_C674_INSTALL_DIR=$(CGT_C674_INSTALL_DIR) \
            CGT_C674_ELF_INSTALL_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
            syslink-samples-rtos
endif

examples:
	@echo "#"
	@echo "# Making $@..."
	$(MAKE) -C examples all

install-driver:
	@echo "#"
	@echo "# Installing files in $(EXEC_DIR)"
	$(MAKE) -C packages \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            LINUXKERNEL=$(LINUXKERNEL) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            EXEC_DIR=$(EXEC_DIR) \
            SDK=$(SDK) \
            LOADER=$(LOADER) \
            $@

install-samples:
	@echo "#"
	@echo "# Installing files in $(EXEC_DIR)"
	$(MAKE) -C packages \
            DEVICE=$(DEVICE) \
            GPPOS=$(GPPOS) \
            SYSLINK_REPO=$(SYSLINK_INSTALL_DIR)/packages \
            LINUXKERNEL=$(LINUXKERNEL) \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            EXEC_DIR=$(EXEC_DIR) \
            SDK=$(SDK) \
            LOADER=$(LOADER) \
            $@

clean::
	@echo "#"
	@echo "# Cleaning packages..."
	$(MAKE) -C packages \
            XDC_INSTALL_DIR=$(XDC_INSTALL_DIR) \
            QNX_INSTALL_DIR=$(QNX_INSTALL_DIR) \
            clean

help:
	@echo
	@echo "Available build targets are:"
	@echo
	@echo "  all (default)       : Builds GPP driver, GPP user libraries and slave"
	@echo "                        libraries"
	@echo
	@echo "  syslink             : Builds GPP driver, GPP user libraries and slave"
	@echo "                        libraries"
	@echo "  syslink-driver      : Builds GPP driver"
	@echo "  syslink-hlos        : Builds GPP (HLOS) user libraries"
	@echo "  syslink-rtos        : Builds slave (RTOS) libraries"
	@echo
	@echo "  samples             : Builds GPP and slave sample applications"
	@echo "  samples-hlos        : Builds GPP (HLOS) sample applications"
	@echo "  samples-rtos        : Builds slave (RTOS) sample applications"
	@echo
	@echo "  install             : Install GPP driver and samples in"
	@echo "                        $(EXEC_DIR)"
	@echo "  install-driver      : Install GPP driver"
	@echo "  install-samples     : Install all GPP and slave sample applications"
	@echo
	@echo "  .show-products      : Prints all component and option settings"
	@echo
	@echo "  clean               : Remove all generated files and sample applications"
	@echo

################################################################################
# Set required components and options flags to check for specific target goals
###############################################################################
ifeq ($(MAKECMDGOALS), )
REQUIRE_GPPOS :=1
REQUIRE_SYSLINK_BUILD_FLAGS:=1
REQUIRE_KERNEL :=1
REQUIRE_GPP_CGT :=1
REQUIRE_CGT_DSP :=1
REQUIRE_IPC := 1
REQUIRE_BIOS := 1
REQUIRE_XDC := 1
endif

ifeq ($(MAKECMDGOALS), syslink)
REQUIRE_GPPOS :=1
REQUIRE_SYSLINK_BUILD_FLAGS:=1
REQUIRE_KERNEL :=1
REQUIRE_GPP_CGT :=1
REQUIRE_CGT_DSP :=1
REQUIRE_IPC := 1
REQUIRE_BIOS := 1
REQUIRE_XDC := 1
endif

ifeq ($(MAKECMDGOALS), all)
REQUIRE_GPPOS :=1
REQUIRE_SYSLINK_BUILD_FLAGS:=1
REQUIRE_KERNEL :=1
REQUIRE_GPP_CGT :=1
REQUIRE_CGT_DSP :=1
REQUIRE_IPC := 1
REQUIRE_BIOS := 1
REQUIRE_XDC := 1
endif

ifeq ($(MAKECMDGOALS), syslink-driver)
REQUIRE_GPPOS :=1
REQUIRE_SYSLINK_BUILD_FLAGS:=1
REQUIRE_KERNEL :=1
REQUIRE_GPP_CGT :=1
REQUIRE_IPC := 1
endif

ifeq ($(MAKECMDGOALS), syslink-hlos)
REQUIRE_GPPOS :=1
REQUIRE_GPP_CGT :=1
REQUIRE_IPC := 1
endif

ifeq ($(MAKECMDGOALS), syslink-rtos)
REQUIRE_GPPOS :=1
REQUIRE_CGT_DSP :=1
REQUIRE_IPC := 1
REQUIRE_BIOS := 1
REQUIRE_XDC := 1
endif

ifeq ($(MAKECMDGOALS), samples)
REQUIRE_GPPOS :=1
REQUIRE_GPP_CGT :=1
REQUIRE_CGT_DSP :=1
REQUIRE_IPC := 1
REQUIRE_BIOS := 1
REQUIRE_XDC := 1
endif

ifeq ($(MAKECMDGOALS), samples-hlos)
REQUIRE_GPPOS :=1
REQUIRE_GPP_CGT :=1
REQUIRE_IPC := 1
endif

ifeq ($(MAKECMDGOALS), samples-rtos)
REQUIRE_GPPOS :=1
REQUIRE_CGT_DSP :=1
REQUIRE_IPC := 1
REQUIRE_BIOS := 1
REQUIRE_XDC := 1
endif

#
# Set error message strings
#
ERRMSG = Set this in <SysLink Install>/products.mak. Refer to the SysLink Install Guide for more information.
ERRMSG1 = which is invalid (could not find file "$(TEST_FILE)"). $(ERRMSG)

#
# Perform component and option check if required by goals
#
ifeq ($(REQUIRE_GPPOS), 1)
    ifeq ("$(GPPOS)", "Linux")
    else ifeq ("$(GPPOS)", "Qnx")
    else ifeq ("$(GPPOS)", "Bios")
    else ifeq ("$(GPPOS)", "Android")
    else
        $(error GPPOS is set to "$(GPPOS)", which is invalid. $(ERRMSG))
    endif
endif

ifeq ($(REQUIRE_SYSLINK_BUILD_FLAGS), 1)
    ifeq ("$(SYSLINK_BUILD_DEBUG)", "0")
    else ifeq ("$(SYSLINK_BUILD_DEBUG)", "1")
    else
        $(error SYSLINK_BUILD_DEBUG is set to "$(SYSLINK_BUILD_DEBUG)", which is invalid. $(ERRMSG))
    endif

    ifeq ("$(SYSLINK_BUILD_OPTIMIZE)", "0")
    else ifeq ("$(SYSLINK_BUILD_OPTIMIZE)", "1")
    else
        $(error SYSLINK_BUILD_OPTIMIZE is set to "$(SYSLINK_BUILD_OPTIMIZE)", which is invalid. $(ERRMSG))
    endif

    ifeq ("$(SYSLINK_TRACE_ENABLE)", "0")
    else ifeq ("$(SYSLINK_TRACE_ENABLE)", "1")
    else
        $(error SYSLINK_TRACE_ENABLE is set to "$(SYSLINK_TRACE_ENABLE)", which is invalid. $(ERRMSG))
    endif
endif

ifeq ($(REQUIRE_KERNEL), 1)
    ifeq ("$(GPPOS)", "Linux")
        TEST_FILE := $(LINUXKERNEL)/include/config/kernel.release
        ifeq ($(wildcard $(TEST_FILE)),)
            $(error LINUXKERNEL is set to "$(LINUXKERNEL)", $(ERRMSG1))
        endif
    endif
    ifeq ("$(GPPOS)", "Qnx")
        ifeq ($(wildcard $(QNX_INSTALL_DIR)),)
            $(error QNX_INSTALL_DIR is set to "$(QNX_INSTALL_DIR)" which is invalid. $(ERRMSG))
        endif
    endif
endif

ifeq ($(REQUIRE_GPP_CGT), 1)
    ifeq ("$(GPPOS)", "Linux")
        TEST_FILE := $(CGT_ARM_PREFIX)gcc
        ifeq ($(wildcard $(TEST_FILE)),)
            $(error CGT_ARM_PREFIX is set to "$(CGT_ARM_PREFIX)", $(ERRMSG1))
        endif
    endif
endif

ifeq ($(REQUIRE_IPC), 1)
    TEST_FILE := $(IPC_INSTALL_DIR)/packages/ti/sdo/ipc/package.xdc
    ifeq ($(wildcard $(TEST_FILE)),)
        $(error IPC_INSTALL_DIR is set to "$(IPC_INSTALL_DIR)", $(ERRMSG1))
    endif
endif

ifeq ($(REQUIRE_BIOS), 1)
    TEST_FILE := $(BIOS_INSTALL_DIR)/packages/ti/bios/package.xdc
    ifeq ($(wildcard $(TEST_FILE)),)
        $(error BIOS_INSTALL_DIR is set to "$(BIOS_INSTALL_DIR)", $(ERRMSG1))
    endif
endif

ifeq ($(REQUIRE_XDC), 1)
    TEST_FILE := $(XDC_INSTALL_DIR)/packages/xdc/package.xdc
    ifeq ($(wildcard $(TEST_FILE)),)
        $(error XDC_INSTALL_DIR is set to "$(XDC_INSTALL_DIR)", $(ERRMSG1))
    endif
endif

#
# Check device specific dependecies and options for all goals except for
# ".show-products", "clean", "help", or "install*"
#
ifeq ($(MAKECMDGOALS), .show-products)
else ifeq ($(MAKECMDGOALS), clean)
else ifeq ($(MAKECMDGOALS), help)
else ifeq ($(MAKECMDGOALS), install)
else ifeq ($(MAKECMDGOALS), install-driver)
else ifeq ($(MAKECMDGOALS), install-samples)

#
# Check for OMAP3530 specific components and options
#
else ifeq ("$(DEVICE)","OMAP3530")
    ifneq ("$(GPPOS)", "Linux")
        $(error "$(GPPOS)" is not a supported GPPOS on "$(DEVICE)")
    endif
    ifeq ("$(LOADER)", "")
        # default OMAP3530 loader is [currently] COFF
        LOADER=COFF
    endif
    ifeq ("$(LOADER)", "COFF")
        ifeq ($(REQUIRE_CGT_DSP), 1)
            TEST_FILE := $(CGT_C64P_INSTALL_DIR)/bin/cl6x*
            ifeq ($(wildcard $(TEST_FILE)),)
                $(error CGT_C64P_INSTALL_DIR is set to "$(CGT_C64P_INSTALL_DIR)", $(ERRMSG1))
            endif
        endif
    else ifeq ("$(LOADER)", "ELF")
        ifeq ($(REQUIRE_CGT_DSP), 1)
            TEST_FILE := $(CGT_C64P_ELF_INSTALL_DIR)/bin/cl6x*
            ifeq ($(wildcard $(TEST_FILE)),)
                $(error CGT_C64P_ELF_INSTALL_DIR is set to "$(CGT_C64P_ELF_INSTALL_DIR)", $(ERRMSG1))
            endif
        endif
    else
        $(error LOADER is set to "$(LOADER)", which is invalid. $(ERRMSG))
    endif

#
# Check for TI816X specific components and options
#
else ifeq ("$(DEVICE)","TI816X")
    ifeq ("$(GPPOS)", "Bios")
        $(error "$(GPPOS)" is not a supported GPPOS on "$(DEVICE)")
    endif
    ifeq ("$(LOADER)", "")
    else ifneq ("$(LOADER)", "ELF")
        $(error LOADER is set to "$(LOADER)", which is invalid. $(ERRMSG))
    endif
    ifeq ($(REQUIRE_CGT_DSP), 1)
        TEST_FILE := $(CGT_C674_ELF_INSTALL_DIR)/bin/cl6x*
        ifeq ($(wildcard $(TEST_FILE)),)
            $(error CGT_C674_ELF_INSTALL_DIR is set to "$(CGT_C674_ELF_INSTALL_DIR)", $(ERRMSG1))
        endif
    endif
    ifeq ("$(SDK)","NONE")
        ifeq ($(REQUIRE_CGT_DSP), 1)
            TEST_FILE := $(CGT_M3_ELF_INSTALL_DIR)/bin/cl470*
            ifeq ($(wildcard $(TEST_FILE)),)
                $(error CGT_M3_ELF_INSTALL_DIR is set to "$(CGT_M3_ELF_INSTALL_DIR)", $(ERRMSG1))
            endif
        endif
    else ifneq ("$(SDK)", "EZSDK")
         $(error SDK is set to "$(SDK)", which is invalid. $(ERRMSG))
    endif

#
# Check for TI814X specific components and options
#
else ifeq ("$(DEVICE)","TI814X")
    ifeq ("$(LOADER)", "")
    else ifneq ("$(LOADER)", "ELF")
        $(error LOADER is set to "$(LOADER)", which is invalid. $(ERRMSG))
    endif
    ifeq ($(REQUIRE_CGT_DSP), 1)
        TEST_FILE := $(CGT_C674_ELF_INSTALL_DIR)/bin/cl6x*
        ifeq ($(wildcard $(TEST_FILE)),)
            $(error CGT_C674_ELF_INSTALL_DIR is set to "$(CGT_C674_ELF_INSTALL_DIR)", $(ERRMSG1))
        endif
    endif
    ifeq ("$(SDK)","NONE")
        ifeq ("$(GPPOS)", "Bios")
            TEST_FILE := $(CGT_A8_ELF_INSTALL_DIR)/bin/cl470*
            ifeq ($(wildcard $(TEST_FILE)),)
                $(error CGT_A8_ELF_INSTALL_DIR is set to "$(CGT_A8_ELF_INSTALL_DIR)", $(ERRMSG1))
            endif
        endif
        ifeq ($(REQUIRE_CGT_DSP), 1)
            TEST_FILE := $(CGT_M3_ELF_INSTALL_DIR)/bin/cl470*
            ifeq ($(wildcard $(TEST_FILE)),)
                $(error CGT_M3_ELF_INSTALL_DIR is set to "$(CGT_M3_ELF_INSTALL_DIR)", $(ERRMSG1))
            endif
        endif
    else ifeq ("$(SDK)", "EZSDK")
        ifeq ("$(GPPOS)", "Bios")
             $(error SDK is set to "$(SDK)", which is invalid when setting GPPOS to "$(GPPOS)". $(ERRMSG))
        endif
    else
         $(error SDK is set to "$(SDK)", which is invalid. $(ERRMSG))
    endif

#
# Check for TI813X specific components and options
#
else ifeq ("$(DEVICE)","TI813X")
    ifeq ("$(GPPOS)", "Bios")
        $(error "$(GPPOS)" is not a supported GPPOS on "$(DEVICE)")
    endif
    ifeq ("$(LOADER)", "")
    else ifneq ("$(LOADER)", "ELF")
        $(error LOADER is set to "$(LOADER)", which is invalid. $(ERRMSG))
    endif
    ifeq ("$(SDK)","NONE")
        ifeq ($(REQUIRE_CGT_DSP), 1)
            TEST_FILE := $(CGT_M3_ELF_INSTALL_DIR)/bin/cl470*
            ifeq ($(wildcard $(TEST_FILE)),)
                $(error CGT_M3_ELF_INSTALL_DIR is set to "$(CGT_M3_ELF_INSTALL_DIR)", $(ERRMSG1))
            endif
        endif
    else ifeq ("$(SDK)", "EZSDK")
             ifeq ($(MAKECMDGOALS), samples)
             $(error For SDK set to "$(SDK)", building samples are not support on "$(DEVICE)")
             endif
    else
         $(error SDK is set to "$(SDK)", which is invalid. $(ERRMSG))
    endif

#
# Check for TI811X specific components and options
#
else ifeq ("$(DEVICE)","TI811X")
    ifeq ("$(GPPOS)", "Bios")
        $(error "$(GPPOS)" is not a supported GPPOS on "$(DEVICE)")
    endif
    ifeq ("$(LOADER)", "")
    else ifneq ("$(LOADER)", "ELF")
        $(error LOADER is set to "$(LOADER)", which is invalid. $(ERRMSG))
    endif
    ifeq ($(REQUIRE_CGT_DSP), 1)
        TEST_FILE := $(CGT_C674_ELF_INSTALL_DIR)/bin/cl6x*
        ifeq ($(wildcard $(TEST_FILE)),)
            $(error CGT_C674_ELF_INSTALL_DIR is set to "$(CGT_C674_ELF_INSTALL_DIR)", $(ERRMSG1))
        endif
    endif
    ifeq ("$(SDK)","NONE")
        ifeq ($(REQUIRE_CGT_DSP), 1)
            TEST_FILE := $(CGT_M3_ELF_INSTALL_DIR)/bin/cl470*
            ifeq ($(wildcard $(TEST_FILE)),)
                $(error CGT_M3_ELF_INSTALL_DIR is set to "$(CGT_M3_ELF_INSTALL_DIR)", $(ERRMSG1))
            endif
        endif
    else ifeq ("$(SDK)", "EZSDK")
             ifeq ($(MAKECMDGOALS), samples)
             $(error For SDK set to "$(SDK)", building samples are not support on "$(DEVICE)")
             endif
    else
         $(error SDK is set to "$(SDK)", which is invalid. $(ERRMSG))
    endif

#
# Check for OMAPLXX specific components and options
#
else ifeq ("$(DEVICE)","OMAPL1XX")
    ifeq ("$(GPPOS)", "Bios")
        $(error "$(GPPOS)" is not a supported GPPOS on "$(DEVICE)")
    endif
    ifeq ("$(LOADER)", "")
        ifeq ($(REQUIRE_CGT_DSP), 1)
            TEST_FILE := $(CGT_C674_INSTALL_DIR)/bin/cl6x*
            ifeq ($(wildcard $(TEST_FILE)),)
                $(error CGT_C674_INSTALL_DIR is set to "$(CGT_C674_INSTALL_DIR)", $(ERRMSG1))
            endif
        endif
    else ifeq ("$(LOADER)", "COFF")
        ifeq ($(REQUIRE_CGT_DSP), 1)
            TEST_FILE := $(CGT_C674_INSTALL_DIR)/bin/cl6x*
            ifeq ($(wildcard $(TEST_FILE)),)
                $(error CGT_C674_INSTALL_DIR is set to "$(CGT_C674_INSTALL_DIR)", $(ERRMSG1))
            endif
        endif
    else ifeq ("$(LOADER)", "ELF")
        ifeq ($(REQUIRE_CGT_DSP), 1)
            TEST_FILE := $(CGT_C674_ELF_INSTALL_DIR)/bin/cl6x*
            ifeq ($(wildcard $(TEST_FILE)),)
                $(error CGT_C674_ELF_INSTALL_DIR is set to "$(CGT_C674_ELF_INSTALL_DIR)", $(ERRMSG1))
            endif
        endif
    else
        $(error LOADER is set to "$(LOADER)", which is invalid. $(ERRMSG))
    endif

endif # Device specific checks
