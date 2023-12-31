#   
#   @file   Makefile
#
#   @brief  This is the user side build system
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

# Override definitions in base Makefile if required
TOOLCHAIN_PREFIX :=

MKDIR            := mkdir -p

# ---------------------------------------------------------------------------- #
# Enviornment flags                                                            #
# ---------------------------------------------------------------------------- #
# Include all common enviroment flags
-include $(SYSLINK_ROOT)/ti/syslink/buildutils/hlos/usr/environment.mk

SAMPLE           := notifyapp
APP_DEP_LIB      := $(SAMPLES_DIR)/slaveloader.$(OBJSUFFIX)
APP_DEP_LIB      += $(SAMPLES_DIR)/syslinksamples.$(OBJSUFFIX)

ifeq ("$(SYSLINK_PLATFORM)", "C6472")
APP_DEP_LIB      :=
endif
ifeq ("$(SYSLINK_PLATFORM)", "C6474")
APP_DEP_LIB      :=
endif

# ---------------------------------------------------------------------------- #
# Defines                                                                      #
# ---------------------------------------------------------------------------- #
# Override definitions in base Makefile if required
SYSLINK_BUILD_OPTIMIZE :=
SYSLINK_PLATFORM :=
SYSLINK_BUILDOS_LINUX :=
SYSLINK_BUILD_DEBUG :=
SYSLINK_TRACE_ENABLE :=

SAMPLE_CSRCS := $(SYSLINK_ROOT)/ti/syslink/samples/hlos/notify/NotifyApp.c
SAMPLE_CSRCS += $(SYSLINK_ROOT)/ti/syslink/samples/hlos/notify/usr/Linux/NotifyAppOS.c
ifeq ("$(SYSLINK_PLATFORM)", "C6472")
SAMPLE_CSRCS += $(SYSLINK_ROOT)/ti/syslink/samples/hlos/common/SysLinkSamples.c
SAMPLE_CSRCS += $(SYSLINK_ROOT)/ti/syslink/samples/hlos/common/usr/Linux/SysLinkSamplesAppOS.c
SAMPLE_CSRCS += $(SYSLINK_ROOT)/ti/syslink/samples/hlos/slaveLoader/SlaveLoader.c
endif
ifeq ("$(SYSLINK_PLATFORM)", "C6474")
SAMPLE_CSRCS += $(SYSLINK_ROOT)/ti/syslink/samples/hlos/common/SysLinkSamples.c
SAMPLE_CSRCS += $(SYSLINK_ROOT)/ti/syslink/samples/hlos/common/usr/Linux/SysLinkSamplesAppOS.c
SAMPLE_CSRCS += $(SYSLINK_ROOT)/ti/syslink/samples/hlos/slaveLoader/SlaveLoader.c
endif
CHDRS        :=

.PHONY : standard debug release build cleanall clean cleandebug cleanrelease depend

standard:       WAY=debug, release
debug:          WAY=debug
release:        WAY=release

standard:       build debug release
debug:          build $(SAMPLES_DIR)/$(SAMPLE).exe_debug    move_debug
release:        build $(SAMPLES_DIR)/$(SAMPLE).exe_release  move_release

build:
	@echo Building $(SAMPLES_DIR)/$(SAMPLE) "("$(WAY)")"
	$(MKDIR) $(LIB_DIR)
	$(MKDIR) $(OBJS_DIR)
	$(MKDIR) $(SAMPLES_DIR)
	$(MKDIR) $(SAMPLES_DIR)/$(SAMPLE)
	$(MKDIR) $(SAMPLES_DIR)/$(SAMPLE)/debug
	$(MKDIR) $(SAMPLES_DIR)/$(SAMPLE)/release
	$(MKDIR) $(SAMPLES_EXES_DIR)

cleanall: clean
	@rm -rf $(SAMPLES_DIR)/$(SAMPLE)
	@rm -rf $(SAMPLES_EXES_DIR)/$(SAMPLE)
	@rm -rf $(SAMPLES_EXES_DIR)/$(SAMPLE)_debug
	@rm -rf $(SAMPLES_EXES_DIR)/$(SAMPLE)_release

clean: cleandebug cleanrelease

cleandebug:
	@rm -rf $(SAMPLES_DIR)/$(SAMPLE)/debug

cleanrelease:
	@rm -rf $(SAMPLES_DIR)/$(SAMPLE)/release


-include $(SYSLINK_ROOT)/ti/syslink/buildutils/hlos/usr/Makefile.inc
