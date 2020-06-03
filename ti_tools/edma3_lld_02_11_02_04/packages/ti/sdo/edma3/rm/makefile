# Makefile for EDMA3 LLD RM

MODULE_NAME = edma3_lld_rm

SRCDIR = src src/configs
INCDIR = . ../../../.. src

# List all the external components/interfaces, whose interface header files 
#  need to be included for this component
INCLUDE_EXERNAL_INTERFACES = 

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON = edma3resmgr.c edma3_rm_gbl_data.c
CFLAGS_LOCAL_COMMON = -mi10

# Core/SoC/platform specific source files and CFLAGS
# Example: 
#   SRCS_<core/SoC/platform-name> = 
#   CFLAGS_LOCAL_<core/SoC/platform-name> =
SRCS_c6472-evm = edma3_c6472_cfg.c
SRCS_tci6486-evm = edma3_tci6486_cfg.c
SRCS_tci6608-sim = edma3_tci6608_cfg.c
SRCS_tci6616-sim = edma3_tci6616_cfg.c
SRCS_c6670-evm = edma3_c6670_cfg.c
SRCS_c6678-evm = edma3_c6678_cfg.c
SRCS_omapl138-evm = edma3_omapl138_cfg.c
SRCS_c6748-evm = edma3_c6748_cfg.c
SRCS_da830-evm = edma3_da830_cfg.c
SRCS_omap4-evm = edma3_omap4_cfg.c
SRCS_ti814x-evm = edma3_ti814x_cfg.c
SRCS_ti816x-evm = edma3_ti816x_cfg.c
SRCS_ti816x-sim = edma3_ti816x_cfg.c
CFLAGS_LOCAL_generic = -DGENERIC

# Include common make files
include $(ROOTDIR)/makerules/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk 
#     and need not be explicitly specified here

# Nothing beyond this point
