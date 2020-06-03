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

SYSLINK_REPO = $(SYSLINK_INSTALL_DIR)/packages
XDC = $(XDC_INSTALL_DIR)/xdc

#
#  Determine host OS (Windows or UNIX)
#
ifeq (,$(HOSTOS))
    ifeq (,$(findstring :,$(WINDIR)$(windir)$(COMSPEC)$(comspec)))
       HOSTOS := Linux
    else
       HOSTOS := Windows
    endif
endif

ifeq (Windows,$(HOSTOS))
    SHELL = $(XDC_INSTALL_DIR)/bin/sh.exe
else
    # assume Linux
endif

XDC_PATH := $(IPC_INSTALL_DIR)/packages
XDC_PATH := $(XDC_PATH);$(BIOS_INSTALL_DIR)/packages;$(SYSLINK_REPO)

#if FC_INSTALL_DIR is set, add it to XDCPATH
ifneq ("$(FC_INSTALL_DIR)", "")
    XDC_PATH := $(XDC_PATH);$(FC_INSTALL_DIR)/packages
endif

ifeq ("$(SDK)", "NONE")
    SYSLINK_SDK =
else ifeq ("$(SDK)", "EZSDK")
    SYSLINK_SDK = $(SDK)
endif

LOADER_OPT =
ifneq ("$(LOADER)", "")
    LOADER_OPT = SYSLINK_LOADER=$(LOADER)
endif

ifeq ("$(DEVICE)","OMAP3530")
    SYSLINK_DEVICE=$(DEVICE)
    DEVICE_VARIANT=$(DEVICE)
    ifeq ("$(LOADER)","ELF")
        EXECUTABLE_SUFFIX=xe64P
    else
        EXECUTABLE_SUFFIX=x64P
    endif
    XDCARGS = \
        DEVICE=$(DEVICE) \
        CGT_C64P_DIR=$(CGT_C64P_INSTALL_DIR) \
        CGT_C64P_ELF_DIR=$(CGT_C64P_ELF_INSTALL_DIR) \
        DEVICE_VARIANT=$(DEVICE)
else ifeq ("$(DEVICE)","TI816X")
    SYSLINK_DEVICE=TI81XX
    DEVICE_VARIANT=TI816X
    EXECUTABLE_SUFFIX=xe674
    XDCARGS = \
        DEVICE=$(DEVICE) \
        CGT_C674_ELF_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
        CGT_M3_ELF_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
        DEVICE_VARIANT=$(DEVICE_VARIANT)
ifneq ("$(TI81XXDSP_DMTIMER_FREQ)","")
    XDCARGS := $(XDCARGS) TI81XXDSP_DMTIMER_FREQ=$(TI81XXDSP_DMTIMER_FREQ)
endif
else ifeq ("$(DEVICE)","TI814X")
    SYSLINK_DEVICE=TI81XX
    DEVICE_VARIANT=TI814X
    device=ti814x
    EXECUTABLE_SUFFIX=xe674
    XDCARGS = \
        DEVICE=$(DEVICE) \
        CGT_C674_ELF_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
        CGT_M3_ELF_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
        CGT_A8_ELF_DIR=$(CGT_A8_ELF_INSTALL_DIR) \
        DEVICE_VARIANT=$(DEVICE_VARIANT)
ifneq ("$(TI81XXDSP_DMTIMER_FREQ)","")
    XDCARGS := $(XDCARGS) TI81XXDSP_DMTIMER_FREQ=$(TI81XXDSP_DMTIMER_FREQ)
endif
else ifeq ("$(DEVICE)","TI813X")
    SYSLINK_DEVICE=TI81XX
    DEVICE_VARIANT=TI813X
    device=ti813x
    EXECUTABLE_SUFFIX=xem3
    XDCARGS = \
        DEVICE=$(DEVICE) \
        CGT_M3_ELF_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
        DEVICE_VARIANT=$(DEVICE_VARIANT)
else ifeq ("$(DEVICE)","TI811X")
    SYSLINK_DEVICE=TI81XX
    DEVICE_VARIANT=TI811X
    device=ti811x
    EXECUTABLE_SUFFIX=xe674
    XDCARGS = \
        DEVICE=$(DEVICE) \
        CGT_C674_ELF_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
        CGT_M3_ELF_DIR=$(CGT_M3_ELF_INSTALL_DIR) \
        DEVICE_VARIANT=$(DEVICE_VARIANT)
ifneq ("$(TI81XXDSP_DMTIMER_FREQ)","")
    XDCARGS := $(XDCARGS) TI81XXDSP_DMTIMER_FREQ=$(TI81XXDSP_DMTIMER_FREQ)
endif
else ifeq ("$(DEVICE)","OMAPL1XX")
    SYSLINK_DEVICE=$(DEVICE)
    DEVICE_VARIANT=$(DEVICE)
    ifeq ("$(LOADER)","ELF")
        EXECUTABLE_SUFFIX=xe674
    else
        EXECUTABLE_SUFFIX=x674
    endif
    XDCARGS = \
        DEVICE=$(DEVICE) \
        CGT_C674_DIR=$(CGT_C674_INSTALL_DIR) \
        CGT_C674_ELF_DIR=$(CGT_C674_ELF_INSTALL_DIR) \
        DEVICE_VARIANT=$(DEVICE)
endif

ifeq ("$(GPPOS)","Linux")
    SYSLINK_DRIVER = syslink-driver-linux
    SYSLINK_HLOS = syslink-hlos-linux
    KERNEL_VERSION = $(shell cat $(LINUXKERNEL)/include/config/kernel.release)
    INSTALL_DRIVER = install-driver-linux
    INSTALL_SAMPLES = install-samples-linux
else ifeq ("$(GPPOS)","Qnx")
    SYSLINK_DRIVER = syslink-driver-qnx
    SYSLINK_HLOS = syslink-hlos-qnx
    INSTALL_DRIVER = install-driver-qnx
    INSTALL_SAMPLES = install-samples-qnx
ifneq ($(wildcard $(XDC_INSTALL_DIR)/bin/echo.exe),)
    INSTALL_D = $(XDC_INSTALL_DIR)/bin/mkdir.exe -p
    INSTALL_M = $(XDC_INSTALL_DIR)/bin/cp.exe
    INSTALL_  = $(XDC_INSTALL_DIR)/bin/cp.exe
else
    INSTALL_D = install -d
    INSTALL_M = install -m 0755
    INSTALL_  = install
endif
endif #ifeq ("$(GPPOS)","Linux")

ifeq ("$(GPPOS)","Android")
    SYSLINK_DRIVER = syslink-driver-bionic
    SYSLINK_HLOS = syslink-hlos-bionic
    KERNEL_VERSION = $(shell cat $(LINUXKERNEL)/include/config/kernel.release)
    INSTALL_DRIVER = install-driver-bionic
    INSTALL_SAMPLES = install-samples-bionic
endif

ifneq ($(wildcard $(QNX_INSTALL_DIR)),)
    QNX_PATH := $(QNX_INSTALL_DIR)/host/linux/x86/usr/bin
    QNX_PATH := $(QNX_PATH):$(QNX_INSTALL_DIR)/host/linux/x86/bin
    QNX_PATH := $(QNX_PATH):$(QNX_INSTALL_DIR)/host/linux/x86/sbin
    QNX_PATH := $(QNX_PATH):$(QNX_INSTALL_DIR)/host/linux/x86/usr/sbin
    QNX_PATH := $(QNX_PATH):$(QNX_INSTALL_DIR)/linux/x86/usr/photon/appbuilder

    export QNX_CONFIGURATION?=/apps/qnx/6.5.0/etc/qnx
    export QNX_HOST?=$(QNX_INSTALL_DIR)/host/linux/x86
    export QNX_TARGET?=$(QNX_INSTALL_DIR)/target/qnx6
    export MAKEFLAGS=-I$(QNX_INSTALL_DIR)/target/qnx6/usr/include
    export LD_LIBRARY_PATH=$(QNX_INSTALL_DIR)/host/linux/x86/usr/lib
endif

syslink-driver: $(SYSLINK_DRIVER)
syslink-hlos: $(SYSLINK_HLOS)

install-driver: $(INSTALL_DRIVER)
install-samples: $(INSTALL_SAMPLES)


#
#  ======== syslink-driver ========
#
syslink-driver-linux:
	@echo "#"
	@echo "# Making $@ in ti/syslink/utils/hlos/knl/Linux"
	$(MAKE) -C ti/syslink/utils/hlos/knl/Linux \
            ARCH=arm CROSS_COMPILE=$(CGT_ARM_PREFIX) \
            KDIR=$(LINUXKERNEL) \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_PKGPATH="$(XDC_PATH)" \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) $(LOADER_OPT) \
            SYSLINK_BUILD_DEBUG=$(SYSLINK_BUILD_DEBUG) \
            SYSLINK_BUILD_OPTIMIZE=$(SYSLINK_BUILD_OPTIMIZE) \
            SYSLINK_TRACE_ENABLE=$(SYSLINK_TRACE_ENABLE)\
            SYSLINK_BUILDOS=$(GPPOS)

syslink-driver-bionic:
	@echo "#"
	@echo "# Making $@ in ti/syslink/utils/hlos/knl/Linux"
	$(MAKE) -C ti/syslink/utils/hlos/knl/Linux \
            ARCH=arm CROSS_COMPILE=$(CGT_ARM_PREFIX) \
            KDIR=$(LINUXKERNEL) \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_PKGPATH="$(XDC_PATH)" \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) $(LOADER_OPT) \
            SYSLINK_BUILD_DEBUG=$(SYSLINK_BUILD_DEBUG) \
            SYSLINK_BUILD_OPTIMIZE=$(SYSLINK_BUILD_OPTIMIZE) \
            SYSLINK_TRACE_ENABLE=$(SYSLINK_TRACE_ENABLE)\
            SYSLINK_BUILDOS=Linux


syslink-driver-qnx: PATH:=$(QNX_PATH):$(PATH)
syslink-driver-qnx:
	@echo "#"
	@echo "# Making $@ in ti/syslink/utils/hlos/knl/Qnx"
	$(MAKE) -C ti/syslink/utils/hlos/knl/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) $(LOADER_OPT) \
            SYSLINK_BUILD_DEBUG=$(SYSLINK_BUILD_DEBUG) \
            SYSLINK_BUILD_OPTIMIZE=$(SYSLINK_BUILD_OPTIMIZE) \
            SYSLINK_TRACE_ENABLE=$(SYSLINK_TRACE_ENABLE)\
            SYSLINK_BUILDOS=$(GPPOS) \
            install

#
#  ======== syslink-hlos ========
#
syslink-hlos-linux:
	@echo "#"
	@echo "# Making $@ ti/syslink/utils/hlos/usr/Linux"
	$(MAKE) -C ti/syslink/utils/hlos/usr/Linux \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            TOOLCHAIN_PREFIX=$(CGT_ARM_PREFIX) \
            SYSLINK_PKGPATH="$(XDC_PATH)" $(LOADER_OPT) \
            SYSLINK_BUILDOS=$(GPPOS)

syslink-hlos-bionic:
	@echo "#"
	@echo "# Making $@ ti/syslink/utils/hlos/usr/Linux"
	$(MAKE) -C ti/syslink/utils/hlos/usr/Linux \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            TOOLCHAIN_PREFIX=$(CGT_ARM_PREFIX) \
            SYSLINK_PKGPATH="$(XDC_PATH)" $(LOADER_OPT) \
            SYSLINK_BUILDOS=$(GPPOS)

syslink-hlos-qnx: PATH:=$(QNX_PATH):$(PATH)
syslink-hlos-qnx:
	@echo "#"
	@echo "# Making $@ ti/syslink/utils/hlos/usr/Qnx"
	$(MAKE) -C ti/syslink/utils/hlos/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_PKGPATH=$(IPC_INSTALL_DIR)/packages \
	    SYSLINK_BUILD_OPTIMIZE=$(SYSLINK_BUILD_OPTIMIZE) \
            SYSLINK_BUILDOS=$(GPPOS) \
            install

#
#  ======== syslink-rtos ========
#
syslink-rtos:
	@echo "#"
	@echo "# Making $@ in ti/syslink"
	$(XDC) XDCBUILDCFG=$(CURDIR)/_config.bld XDCPATH="$(XDC_PATH)" \
            XDCARGS="$(XDCARGS)" .libraries -Pr $(CURDIR)


ifeq ("$(GPPOS)","Linux")
GPPOS_LINUX_OR_ANDROID	:= "Linux"
else ifeq ("$(GPPOS)","Android")
GPPOS_LINUX_OR_ANDROID	:= "Android"
else
GPPOS_LINUX_OR_ANDROID	:=
endif

#
#  ======== syslink-samples ========
#
ifneq ("$(GPPOS_LINUX_OR_ANDROID)","")

SAMPLE_VARS = SYSLINK_ROOT=$(SYSLINK_REPO) TOOLCHAIN_PREFIX=$(CGT_ARM_PREFIX) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) SYSLINK_PKGPATH="$(XDC_PATH)" \
            SYSLINK_SDK=$(SYSLINK_SDK) SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            SYSLINK_BUILDOS=$(GPPOS)

Common:
	@echo "#"
	@echo "# Making $@ in ti/syslink/samples/hlos/common/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/common/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: SlaveLoader
SlaveLoader: Common
	@echo "#"
	@echo "# Making $@ in ti/syslink/samples/hlos/slaveLoader/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/slaveLoader/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: Notify
Notify: Common
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/notify/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/notify/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: MessageQ
MessageQ: Common
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/messageQ/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/messageQ/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: ListMP
ListMP: Common
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/listMP/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/listMP/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: HeapBufMP
HeapBufMP: Common
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/heapBufMP/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/heapBufMP/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: HeapMemMP
HeapMemMP: Common
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/heapMemMP/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/heapMemMP/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: FrameQ
FrameQ: Common
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/frameq/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/frameq/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: RingIO
RingIO: Common
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/ringIO/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/ringIO/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: RingIO_GPP
RingIO_GPP: Common
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/ringIO_gpp/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/ringIO_gpp/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: SharedRegion
SharedRegion: Common
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/sharedRegion/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/sharedRegion/usr/Linux $(SAMPLE_VARS)

syslink-samples-hlos: GateMP
GateMP: Common
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/gateMP/usr/Linux"
	$(MAKE) -C ti/syslink/samples/hlos/gateMP/usr/Linux $(SAMPLE_VARS)

else ifeq ("$(GPPOS)","Qnx")

Common: PATH:=$(QNX_PATH):$(PATH)
Common:
	@echo "#"
	@echo "# Making $@ in ti/syslink/samples/hlos/common/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/common/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

SlaveLoader: Common
SlaveLoader: PATH:=$(QNX_PATH):$(PATH)
SlaveLoader:
	@echo "#"
	@echo "# Making $@ in ti/syslink/samples/hlos/slaveLoaderlib/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/slaveLoaderlib/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

syslink-samples-hlos: Notify
Notify: Common SlaveLoader
Notify: PATH:=$(QNX_PATH):$(PATH)
Notify:
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/notify/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/notify/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

syslink-samples-hlos: MessageQ
MessageQ: Common SlaveLoader
MessageQ: PATH:=$(QNX_PATH):$(PATH)
MessageQ:
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/messageQ/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/messageQ/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

syslink-samples-hlos: ListMP
ListMP: Common SlaveLoader
ListMP: PATH:=$(QNX_PATH):$(PATH)
ListMP:
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/listMP/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/listMP/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

syslink-samples-hlos: HeapBufMP
HeapBufMP: Common SlaveLoader
HeapBufMP: PATH:=$(QNX_PATH):$(PATH)
HeapBufMP:
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/heapBufMP/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/heapBufMP/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

syslink-samples-hlos: HeapMemMP
HeapMemMP: Common SlaveLoader
HeapMemMP: PATH:=$(QNX_PATH):$(PATH)
HeapMemMP:
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/heapMemMP/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/heapMemMP/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

syslink-samples-hlos: FrameQ
FrameQ: Common SlaveLoader
FrameQ: PATH:=$(QNX_PATH):$(PATH)
FrameQ:
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/frameq/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/frameq/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

syslink-samples-hlos: RingIO
RingIO: Common SlaveLoader
RingIO: PATH:=$(QNX_PATH):$(PATH)
RingIO:
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/ringIO/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/ringIO/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

syslink-samples-hlos: SharedRegion
SharedRegion: Common SlaveLoader
SharedRegion: PATH:=$(QNX_PATH):$(PATH)
SharedRegion:
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/sharedRegion/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/sharedRegion/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

syslink-samples-hlos: slaveLoader
slaveLoader: Common SlaveLoader
slaveLoader: PATH:=$(QNX_PATH):$(PATH)
slaveLoader:
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/slaveLoader/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/slaveLoader/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

syslink-samples-hlos: GateMP
GateMP: Common SlaveLoader
GateMP: PATH:=$(QNX_PATH):$(PATH)
GateMP:
	@echo "#"
	@echo "# Making $@ ti/syslink/samples/hlos/gateMP/usr/Qnx"
	$(MAKE) -C ti/syslink/samples/hlos/gateMP/usr/Qnx \
            SYSLINK_ROOT=$(SYSLINK_REPO) \
            SYSLINK_PKGPATH=$(subst ;,+,$(XDC_PATH)) \
            SYSLINK_BUILDOS=$(GPPOS) \
            SYSLINK_PLATFORM=$(SYSLINK_DEVICE) \
            SYSLINK_VARIANT=$(DEVICE_VARIANT) \
            install

endif # ifeq ("$(GPPOS)","Qnx")

syslink-samples-rtos: RtosSamples
RtosSamples:
ifeq ("$(GPPOS)","Linux")
	@echo "#"
	@echo "# Making $@ in ti/syslink"
	$(XDC) XDCBUILDCFG=$(CURDIR)/_config.bld XDCPATH="$(XDC_PATH)" \
            XDCARGS="$(XDCARGS)" all -Pr $(CURDIR)
else ifeq ("$(GPPOS)","Qnx")
	@echo "#"
	@echo "# Making $@ in ti/syslink"
	$(XDC) XDCBUILDCFG=$(CURDIR)/_config.bld XDCPATH="$(XDC_PATH)" \
            XDCARGS="$(XDCARGS)" all -Pr $(CURDIR)
else ifeq ("$(GPPOS)","Android")
	@echo "#"
	@echo "# Making $@ in ti/syslink"
	$(XDC) XDCBUILDCFG=$(CURDIR)/_config.bld XDCPATH="$(XDC_PATH)" \
            XDCARGS="$(XDCARGS)" all -Pr $(CURDIR)
else ifeq ("$(GPPOS)","Bios")
	@echo "#"
	@echo "# Making $@ in ti/syslink/samples/rtos/ti81xx"

	@echo "# Building frameQ slave samples"
	$(XDC) XDCBUILDCFG=$(CURDIR)/_config.bld XDCPATH="$(XDC_PATH)" \
            XDCARGS="$(XDCARGS)" all -P $(CURDIR)/ti/syslink/samples/rtos/ti81xx/frameq_ti814x/slaveBuild

	@echo "# Building ringIO slave samples"
	$(XDC) XDCBUILDCFG=$(CURDIR)/_config.bld XDCPATH="$(XDC_PATH)" \
            XDCARGS="$(XDCARGS)" all -P $(CURDIR)/ti/syslink/samples/rtos/ti81xx/ringIO_ti814x/slaveBuild

ifeq (Linux,$(HOSTOS))
	@echo "# Building RAM disk for FrameQ sample"
	cd $(CURDIR)/ti/syslink/samples/rtos/ti81xx/frameq_ti814x; PATH=$(BIOS_INSTALL_DIR)/packages/ti/sysbios/fatfs/utils/bin ./makeRamDisk.sh

	@echo "# Building RAM disk for RingIO sample"
	cd $(CURDIR)/ti/syslink/samples/rtos/ti81xx/ringIO_ti814x; PATH=$(BIOS_INSTALL_DIR)/packages/ti/sysbios/fatfs/utils/bin ./makeRamDisk.sh

else
# assume Windows
	@echo "# Building RAM disk for FrameQ sample"
	$(CURDIR)/ti/syslink/samples/rtos/ti81xx/frameq_ti814x/makeRamDisk.bat $(BIOS_INSTALL_DIR)/packages/ti/sysbios/fatfs/utils/bin  $(CURDIR)/ti/syslink/samples/rtos/ti81xx/frameq_ti814x

	@echo "# Building RAM disk for RingIO sample"
	$(CURDIR)/ti/syslink/samples/rtos/ti81xx/ringIO_ti814x/makeRamDisk.bat $(BIOS_INSTALL_DIR)/packages/ti/sysbios/fatfs/utils/bin  $(CURDIR)/ti/syslink/samples/rtos/ti81xx/ringIO_ti814x
endif

	@echo "# Building frameQ master sample"
	$(XDC) XDCBUILDCFG=$(CURDIR)/_config.bld XDCPATH="$(XDC_PATH)" \
            XDCARGS="$(XDCARGS)" all -P $(CURDIR)/ti/syslink/samples/rtos/ti81xx/frameq_ti814x

	@echo "# Building ringIO master sample"
	$(XDC) XDCBUILDCFG=$(CURDIR)/_config.bld XDCPATH="$(XDC_PATH)" \
            XDCARGS="$(XDCARGS)" all -P $(CURDIR)/ti/syslink/samples/rtos/ti81xx/ringIO_ti814x

endif #ifeq ("$(GPPOS)","Bios")

install-driver-bionic:
	@echo "#"
	@echo "# Installing SysLink kernel driver in $(EXEC_DIR)/lib/modules/$(KERNEL_VERSION)/kernel/drivers/dsp"
	install -d $(EXEC_DIR)/lib/modules/$(KERNEL_VERSION)/kernel/drivers/dsp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/syslink.ko $(EXEC_DIR)/lib/modules/$(KERNEL_VERSION)/kernel/drivers/dsp

install-samples-bionic: install-common-bionic
install-common-bionic:
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/runsamples_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)

install-samples-bionic: install-frameq-bionic
install-frameq-bionic:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/frameqapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/frameq/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_frameqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
    else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/frameq/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_frameqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
    endif
endif

install-samples-bionic: install-gatemp-bionic
install-gatemp-bionic:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/gatempapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/gateMP/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_gatempapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
    else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/gateMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_gatempapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
    endif
endif

install-samples-bionic: install-heapbufmp-bionic
install-heapbufmp-bionic:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/heapbufmpapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapBufMP/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_heapbufmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
    else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapBufMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_heapbufmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
    endif
endif

install-samples-bionic: install-heapmemmp-bionic
install-heapmemmp-bionic:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/heapmemmpapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapMemMP/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_heapmemmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
    else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapMemMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_heapmemmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
    endif
endif

install-samples-bionic: install-listmp-bionic
install-listmp-bionic:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/listmpapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/listMP/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_listmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/listMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_listmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
    endif
endif

install-samples-bionic: install-messageq-bionic
install-messageq-bionic:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/messageqapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/messageQ/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_messageqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
    else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/messageQ/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_messageqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
    endif
endif

install-samples-bionic: install-notify-bionic
install-notify-bionic:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/notifyapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/notify/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_notifyapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
    else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/notify/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_notifyapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
    endif
endif

install-samples-bionic: install-ringio-bionic
install-ringio-bionic:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/ringioapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/ringIO/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_ringio_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
    else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/ringIO/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_ringio_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
    endif
endif

# Sample not install(ed) since Ipc_stop not called on RTOS-side
#install-samples-bionic: install-ringiogpp-bionic
install-ringiogpp-bionic:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/ringiogppapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/ringIO_gpp/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_ringiogpp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
    else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/ringIO_gpp/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_ringiogpp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
    endif
endif

# Sample not install(ed) since Ipc_stop not called on RTOS-side
#install-samples-bionic: install-sharedregion-bionic
install-sharedregion-bionic:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/bionic/$(DEVICE)/samples/sharedregionapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/sharedRegion/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_sharedregionapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
    else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/sharedRegion/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_sharedregionapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
    endif
endif


install-driver-linux:
	@echo "#"
	@echo "# Installing SysLink kernel driver in $(EXEC_DIR)/lib/modules/$(KERNEL_VERSION)/kernel/drivers/dsp"
	install -d $(EXEC_DIR)/lib/modules/$(KERNEL_VERSION)/kernel/drivers/dsp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/syslink.ko $(EXEC_DIR)/lib/modules/$(KERNEL_VERSION)/kernel/drivers/dsp

install-samples-linux: install-common-linux
install-common-linux:
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/runsamples_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/runsamples_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/runsamples_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)
endif

install-samples-linux: install-frameq-linux
install-frameq-linux:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/frameqapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/frameq/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/elf/run_frameqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/run_frameqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
    endif
	install $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoader/omap3530_memmap.txt $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/elf/run_frameqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/run_frameqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
    endif
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(DEVICE_VARIANT)", "TI813X")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti813x/run_frameqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
    else
        ifeq ("$(SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_frameqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
        else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/frameq/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_frameqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/frameq
        endif
    endif
endif

install-samples-linux: install-gatemp-linux
install-gatemp-linux:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/gatempapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/gateMP/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/elf/run_gatempapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/run_gatempapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
    endif
	install $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoader/omap3530_memmap.txt $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/elf/run_gatempapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/run_gatempapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
    endif
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(DEVICE_VARIANT)", "TI813X")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti813x/run_gatempapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
    else
        ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_gatempapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
        else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/gateMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_gatempapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/gatemp
        endif
    endif
endif

install-samples-linux: install-heapbufmp-linux
install-heapbufmp-linux:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/heapbufmpapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapBufMP/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/elf/run_heapbufmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/run_heapbufmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
    endif
	install $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoader/omap3530_memmap.txt $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/elf/run_heapbufmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/run_heapbufmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
    endif
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(DEVICE_VARIANT)", "TI813X")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti813x/run_heapbufmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
    else
        ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_heapbufmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
        else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapBufMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_heapbufmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapbufmp
        endif
    endif
endif

install-samples-linux: install-heapmemmp-linux
install-heapmemmp-linux:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/heapmemmpapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapMemMP/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/elf/run_heapmemmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/run_heapmemmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
    endif
	install $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoader/omap3530_memmap.txt $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/elf/run_heapmemmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/run_heapmemmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
    endif
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(DEVICE_VARIANT)", "TI813X")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti813x/run_heapmemmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
    else
        ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_heapmemmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
        else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapMemMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_heapmemmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/heapmemmp
        endif
    endif
endif

install-samples-linux: install-listmp-linux
install-listmp-linux:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/listmpapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/listMP/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/elf/run_listmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/run_listmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
    endif
	install $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoader/omap3530_memmap.txt $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/elf/run_listmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/run_listmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
    endif
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(DEVICE_VARIANT)", "TI813X")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti813x/run_listmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
    else
        ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_listmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
        else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/listMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_listmpapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/listmp
        endif
    endif
endif

install-samples-linux: install-messageq-linux
install-messageq-linux:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/messageqapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/messageQ/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/elf/run_messageqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/run_messageqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
    endif
	install $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoader/omap3530_memmap.txt $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/elf/run_messageqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/run_messageqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
    endif
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(DEVICE_VARIANT)", "TI813X")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti813x/run_messageqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
    else
        ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_messageqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
        else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/messageQ/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_messageqapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/messageq
        endif
    endif
endif

install-samples-linux: install-notify-linux
install-notify-linux:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/notifyapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/notify/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/elf/run_notifyapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/run_notifyapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
    endif
	install $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoader/omap3530_memmap.txt $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/elf/run_notifyapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/run_notifyapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
    endif
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(DEVICE_VARIANT)", "TI813X")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti813x/run_notifyapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
    else
        ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_notifyapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
        else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/notify/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_notifyapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/notify
        endif
    endif
endif

install-samples-linux: install-ringio-linux
install-ringio-linux:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/ringioapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/ringIO/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/elf/run_ringio_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/run_ringio_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
    endif
	install $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoader/omap3530_memmap.txt $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/elf/run_ringio_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/run_ringio_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
    endif
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(DEVICE_VARIANT)", "TI813X")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti813x/run_ringio_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
    else
        ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_ringio_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
        else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/ringIO/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_ringio_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringio
        endif
    endif
endif

# Sample not install(ed) since Ipc_stop not called on RTOS-side
#install-samples-linux: install-ringiogpp-linux
install-ringiogpp-linux:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/ringiogppapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/ringIO_gpp/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/elf/run_ringiogpp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/run_ringiogpp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
    endif
	install $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoader/omap3530_memmap.txt $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/elf/run_ringiogpp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/run_ringiogpp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
    endif
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(DEVICE_VARIANT)", "TI813X")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti813x/run_ringiogpp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
    else
        ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_ringiogpp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
        else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/ringIO_gpp/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_ringiogpp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/ringiogpp
        endif
    endif
endif

# Sample not install(ed) since Ipc_stop not called on RTOS-side
#install-samples-linux: install-sharedregion-linux
install-sharedregion-linux:
	@echo "#"
	@echo "# Installing $@ sample applications in $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)"
	install -d $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/slaveloader_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
	install -m 0755 $(SYSLINK_REPO)/ti/syslink/bin/$(DEVICE)/samples/sharedregionapp_* $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/sharedRegion/*/*/*.$(EXECUTABLE_SUFFIX) $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
ifeq ("$(SYSLINK_DEVICE)", "OMAP3530")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/elf/run_sharedregionapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omap3530/run_sharedregionapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
    endif
	install $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoader/omap3530_memmap.txt $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
else ifeq ("$(SYSLINK_DEVICE)", "OMAPL1XX")
    ifeq ("$(LOADER)", "ELF")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/elf/run_sharedregionapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
    else
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/omapl1xx/run_sharedregionapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
    endif
else ifeq ("$(SYSLINK_DEVICE)", "TI81XX")
    ifeq ("$(DEVICE_VARIANT)", "TI813X")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti813x/run_sharedregionapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
    else
        ifeq ("$(SYSLINK_SDK)", "EZSDK")
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/ezsdk/run_sharedregionapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
        else # M3 build enabled
	install $(SYSLINK_REPO)/ti/syslink/samples/rtos/sharedRegion/*/*/*.xem3 $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
	install $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/run_sharedregionapp_*.sh $(EXEC_DIR)/ti/syslink-examples/$(DEVICE)/sharedregion
        endif
    endif
endif

install-driver-qnx:
	@echo "#"
	@echo "# Installing SysLink Device Manager in $(EXEC_DIR)/ti/syslink/driver"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/driver/debug
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/$(device)/debug/syslink_drv $(EXEC_DIR)/ti/syslink/driver/debug
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/utils/hlos/knl/Qnx/resMgr/run_syslink_drv.sh $(EXEC_DIR)/ti/syslink/driver/debug
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/driver/release
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/$(device)/release/syslink_drv $(EXEC_DIR)/ti/syslink/driver/release
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/utils/hlos/knl/Qnx/resMgr/run_syslink_drv.sh $(EXEC_DIR)/ti/syslink/driver/release
	@echo "#"
	@echo "# Installing SysLink Shared Library in $(EXEC_DIR)/ti/syslink/lib"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/lib
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/lib/qnx/libsyslink_g.so.1 $(EXEC_DIR)/ti/syslink/lib
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/lib/qnx/libsyslink.so.1 $(EXEC_DIR)/ti/syslink/lib

install-samples-qnx: install-samplelibs-qnx
install-samplelibs-qnx:
	@echo "#"
	@echo "# Installing Sample Shared Libraries in $(EXEC_DIR)/ti/syslink/lib"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/lib
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/samples/hlos/common/usr/Qnx/arm/so.g.le.v7/libsyslink_common_g.so $(EXEC_DIR)/ti/syslink/lib/libsyslink_common_g.so.1
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/samples/hlos/common/usr/Qnx/arm/so.le.v7/libsyslink_common.so $(EXEC_DIR)/ti/syslink/lib/libsyslink_common.so.1
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoaderlib/usr/Qnx/arm/so.g.le.v7/libsyslink_slave_loader_g.so $(EXEC_DIR)/ti/syslink/lib/libsyslink_slave_loader_g.so.1
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/samples/hlos/slaveLoaderlib/usr/Qnx/arm/so.le.v7/libsyslink_slave_loader.so $(EXEC_DIR)/ti/syslink/lib/libsyslink_slave_loader.so.1

install-samples-qnx: install-frameq-qnx
install-frameq-qnx:
	@echo "#"
	@echo "# Installing FrameQ in $(EXEC_DIR)/ti/syslink/samples"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/samples/frameq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader $(EXEC_DIR)/ti/syslink/samples/frameq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader_g $(EXEC_DIR)/ti/syslink/samples/frameq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/frameq_g $(EXEC_DIR)/ti/syslink/samples/frameq
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/frameq/*/*/*.xe674 $(EXEC_DIR)/ti/syslink/samples/frameq
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/frameq/*/*/*.xem3 $(EXEC_DIR)/ti/syslink/samples/frameq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/qnx/run_frameq.sh $(EXEC_DIR)/ti/syslink/samples/frameq

install-samples-qnx: install-gatemp-qnx
install-gatemp-qnx:
	@echo "#"
	@echo "# Installing GateMP in $(EXEC_DIR)/ti/syslink/samples"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/samples/gatemp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader $(EXEC_DIR)/ti/syslink/samples/gatemp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader_g $(EXEC_DIR)/ti/syslink/samples/gatemp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/gateMP_g $(EXEC_DIR)/ti/syslink/samples/gatemp
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/gateMP/*/*/*.xe674 $(EXEC_DIR)/ti/syslink/samples/gatemp
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/gateMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink/samples/gatemp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/qnx/run_gatempapp.sh $(EXEC_DIR)/ti/syslink/samples/gatemp

install-samples-qnx: install-heapbufmp-qnx
install-heapbufmp-qnx:
	@echo "#"
	@echo "# Installing HeapBufMP in $(EXEC_DIR)/ti/syslink/samples"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/samples/heapbufmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader $(EXEC_DIR)/ti/syslink/samples/heapbufmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader_g $(EXEC_DIR)/ti/syslink/samples/heapbufmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/heapBufMP_g $(EXEC_DIR)/ti/syslink/samples/heapbufmp
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapBufMP/*/*/*.xe674 $(EXEC_DIR)/ti/syslink/samples/heapbufmp
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapBufMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink/samples/heapbufmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/qnx/run_heapbufmpapp.sh $(EXEC_DIR)/ti/syslink/samples/heapbufmp

install-samples-qnx: install-listmp-qnx
install-listmp-qnx:
	@echo "#"
	@echo "# Installing ListMP in $(EXEC_DIR)/ti/syslink/samples"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/samples/listmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader $(EXEC_DIR)/ti/syslink/samples/listmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader_g $(EXEC_DIR)/ti/syslink/samples/listmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/listMP_g $(EXEC_DIR)/ti/syslink/samples/listmp
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/listMP/*/*/*.xe674 $(EXEC_DIR)/ti/syslink/samples/listmp
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/listMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink/samples/listmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/qnx/run_listmpapp.sh $(EXEC_DIR)/ti/syslink/samples/listmp

install-samples-qnx: install-heapmemmp-qnx
install-heapmemmp-qnx:
	@echo "#"
	@echo "# Installing HeapMemMP in $(EXEC_DIR)/ti/syslink/samples"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/samples/heapmemmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader $(EXEC_DIR)/ti/syslink/samples/heapmemmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader_g $(EXEC_DIR)/ti/syslink/samples/heapmemmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/heapMemMP_g $(EXEC_DIR)/ti/syslink/samples/heapmemmp
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapMemMP/*/*/*.xe674 $(EXEC_DIR)/ti/syslink/samples/heapmemmp
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/heapMemMP/*/*/*.xem3 $(EXEC_DIR)/ti/syslink/samples/heapmemmp
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/qnx/run_heapmemmpapp.sh $(EXEC_DIR)/ti/syslink/samples/heapmemmp

install-samples-qnx: install-messageq-qnx
install-messageq-qnx:
	@echo "#"
	@echo "# Installing MessageQ in $(EXEC_DIR)/ti/syslink/samples"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/samples/messageq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader $(EXEC_DIR)/ti/syslink/samples/messageq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader_g $(EXEC_DIR)/ti/syslink/samples/messageq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/messageQ $(EXEC_DIR)/ti/syslink/samples/messageq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/messageQ_g $(EXEC_DIR)/ti/syslink/samples/messageq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/messageQ-dyn_g $(EXEC_DIR)/ti/syslink/samples/messageq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/messageQ-dyn $(EXEC_DIR)/ti/syslink/samples/messageq
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/messageQ/*/*/*.xe674 $(EXEC_DIR)/ti/syslink/samples/messageq
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/messageQ/*/*/*.xem3 $(EXEC_DIR)/ti/syslink/samples/messageq
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/qnx/run_messageqapp.sh $(EXEC_DIR)/ti/syslink/samples/messageq

install-samples-qnx: install-notify-qnx
install-notify-qnx:
	@echo "#"
	@echo "# Installing Notify in $(EXEC_DIR)/ti/syslink/samples"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/samples/notify
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader $(EXEC_DIR)/ti/syslink/samples/notify
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader_g $(EXEC_DIR)/ti/syslink/samples/notify
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/notify $(EXEC_DIR)/ti/syslink/samples/notify
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/notify_g $(EXEC_DIR)/ti/syslink/samples/notify
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/notify-dyn_g $(EXEC_DIR)/ti/syslink/samples/notify
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/notify/*/*/*.xe674 $(EXEC_DIR)/ti/syslink/samples/notify
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/notify/*/*/*.xem3 $(EXEC_DIR)/ti/syslink/samples/notify
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/qnx/run_notifyapp.sh $(EXEC_DIR)/ti/syslink/samples/notify

install-samples-qnx: install-ringio-qnx
install-ringio-qnx:
	@echo "#"
	@echo "# Installing RingIO in $(EXEC_DIR)/ti/syslink/samples"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/samples/ringio
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader $(EXEC_DIR)/ti/syslink/samples/ringio
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader_g $(EXEC_DIR)/ti/syslink/samples/ringio
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/ringIO_g $(EXEC_DIR)/ti/syslink/samples/ringio
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/ringIO/*/*/*.xe674 $(EXEC_DIR)/ti/syslink/samples/ringio
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/ringIO/*/*/*.xem3 $(EXEC_DIR)/ti/syslink/samples/ringio
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/qnx/run_ringio.sh $(EXEC_DIR)/ti/syslink/samples/ringio

install-samples-qnx: install-sharedregion-qnx
install-sharedregion-qnx:
	@echo "#"
	@echo "# Installing SharedRegion in $(EXEC_DIR)/ti/syslink/samples"
	$(INSTALL_D) $(EXEC_DIR)/ti/syslink/samples/sharedregion
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader $(EXEC_DIR)/ti/syslink/samples/sharedregion
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/slaveloader_g $(EXEC_DIR)/ti/syslink/samples/sharedregion
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/bin/qnx/samples/sharedRegion_g $(EXEC_DIR)/ti/syslink/samples/sharedregion
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/sharedRegion/*/*/*.xe674 $(EXEC_DIR)/ti/syslink/samples/sharedregion
	$(INSTALL_) $(SYSLINK_REPO)/ti/syslink/samples/rtos/sharedRegion/*/*/*.xem3 $(EXEC_DIR)/ti/syslink/samples/sharedregion
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/qnx/run_sharedregionapp.sh $(EXEC_DIR)/ti/syslink/samples/sharedregion

install-samples-qnx:
	$(INSTALL_M) $(SYSLINK_REPO)/ti/syslink/tools/scripts/ti81xx/qnx/run_all.sh $(EXEC_DIR)/ti/syslink/samples

#
#  ======== clean ========
#
clean::
	@echo "#"
	@echo "# Cleaning packages..."
ifeq (Linux,$(HOSTOS))
	$(MAKE) -C ti/syslink/utils/hlos/knl/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/utils/hlos/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/common/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/slaveLoader/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/notify/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/messageQ/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/listMP/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/heapBufMP/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/heapMemMP/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/frameq/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/ringIO/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/ringIO_gpp/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/sharedRegion/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/gateMP/usr/Linux \
            SYSLINK_ROOT=$(CURDIR) clean
endif
	rm -rf $(CURDIR)/ti/syslink/obj
	rm -rf $(CURDIR)/ti/syslink/lib
	rm -rf $(CURDIR)/ti/syslink/bin

ifneq ($(wildcard $(QNX_INSTALL_DIR)),)
	$(MAKE) -C ti/syslink/utils/hlos/knl/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/utils/hlos/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/common/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/slaveLoaderlib/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/slaveLoader/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/notify/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/messageQ/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/listMP/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/heapBufMP/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/heapMemMP/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/frameq/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/ringIO/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/sharedRegion/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	$(MAKE) -C ti/syslink/samples/hlos/gateMP/usr/Qnx \
            SYSLINK_ROOT=$(CURDIR) clean
	rm -rf $(CURDIR)/ti/install
endif

	$(XDC) XDCBUILDCFG=$(CURDIR)/_config.bld XDCPATH="$(XDC_PATH)" \
            clean -PR $(CURDIR)

ifeq (Linux,$(HOSTOS))
	find $(CURDIR)/ti/syslink -name ".xdcenv.mak" -exec rm {} \;
else
	cmd /C del /S /A:H .xdcenv.mak
endif
