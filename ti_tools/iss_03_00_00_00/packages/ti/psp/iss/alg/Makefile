
MODULE_NAME = iss_alg

SRCDIR = . src

INCDIR = .  inc $(CSL_CRED_DIR) $(CSL_CRAM_DIR) $(iss_alg_PATH) $(iss_framework_PATH) $(iss_drivers_PATH) $(timmosal_PATH) $(omx_PATH) $(bios_PATH)/packages $(syslink_PATH)/packages $(ipc_PATH)/packages $(fc_PATH)/packages $(OMX_INTERFACES_PATH) $(iss_framework_PATH)/fw3a/include

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXERNAL_INTERFACES = bios xdc iss 

MODULE_SRC_BASE_PATH := $(iss_PATH)/packages/ti/psp/iss/alg

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON = 
SRCS_ti816x = 
SRCS_ti814x =
CFLAGS_LOCAL_COMMON = $(ISS_CFLAGS)

include $(MODULE_SRC_BASE_PATH)/msp_3A/SRC_FILES.MK
include $(MODULE_SRC_BASE_PATH)/ip_run/SRC_FILES.MK

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
include $(ROOTDIR)/makerules/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
