# Makefile for EDMA3 LLD RM Sample

MODULE_NAME = edma3_lld_rm_sample

SRCDIR = src src/platforms
INCDIR = . ../../../../.. src

# List all the external components/interfaces, whose interface header files 
#  need to be included for this component
INCLUDE_EXERNAL_INTERFACES = bios xdc

# Common source files and CFLAGS across all platforms and cores
ifeq ($(CORE),c6xdsp)
SRCS_COMMON = sample_cs.c sample_init.c
else
SRCS_COMMON = sample_arm_cs.c sample_arm_init.c
endif
CFLAGS_LOCAL_COMMON = -mi10

# Core/SoC/platform specific source files and CFLAGS
# Example: 
#   SRCS_<core/SoC/platform-name> = 
#   CFLAGS_LOCAL_<core/SoC/platform-name> =
ifeq ($(CORE),c6xdsp)
SRCS_c6472-evm = sample_c6472_cfg.c sample_c6472_int_reg.c
SRCS_tci6486-evm = sample_tci6486_cfg.c sample_tci6486_int_reg.c
SRCS_tci6608-sim = sample_tci6608_cfg.c sample_tci6608_int_reg.c
SRCS_tci6616-sim = sample_tci6616_cfg.c sample_tci6616_int_reg.c
SRCS_c6670-evm   = sample_c6670_cfg.c sample_c6670_int_reg.c
SRCS_c6678-evm   = sample_c6678_cfg.c sample_c6678_int_reg.c
SRCS_omapl138-evm = sample_omapl138_cfg.c sample_omapl138_int_reg.c
SRCS_ti814x-evm = sample_ti814x_cfg.c sample_ti814x_int_reg.c
SRCS_ti816x-evm = sample_ti816x_cfg.c sample_ti816x_int_reg.c
SRCS_ti816x-sim = sample_ti816x_cfg.c sample_ti816x_int_reg.c
else
SRCS_omapl138-evm = sample_omapl138_arm_cfg.c sample_omapl138_arm_int_reg.c
SRCS_ti814x-evm = sample_ti814x_arm_cfg.c sample_ti814x_arm_int_reg.c
endif
ifeq ($(CORE),m3video)
SRCS_ti816x-evm = sample_ti816x_m3video_cfg.c sample_ti816x_m3video_int_reg.c
SRCS_ti816x-sim = sample_ti816x_m3video_cfg.c sample_ti816x_m3video_int_reg.c
endif
ifeq ($(CORE),m3vpss)
SRCS_ti816x-evm = sample_ti816x_m3vpss_cfg.c sample_ti816x_m3vpss_int_reg.c
SRCS_ti816x-sim = sample_ti816x_m3vpss_cfg.c sample_ti816x_m3vpss_int_reg.c
endif
SRCS_c6748-evm = sample_c6748_cfg.c sample_c6748_int_reg.c
SRCS_da830-evm = sample_da830_cfg.c sample_da830_int_reg.c
CFLAGS_LOCAL_ti814x-evm = -DCHIP_TI814X

# Include common make files
include $(ROOTDIR)/makerules/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk 
#     and need not be explicitly specified here

# Nothing beyond this point
