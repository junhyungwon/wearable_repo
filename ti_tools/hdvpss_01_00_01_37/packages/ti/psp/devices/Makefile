
MODULE_NAME = hdvpss_devices

# changed by jhhan
SRCDIR = src ph3100k/src nvp2440h/src
INCDIR = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXERNAL_INTERFACES = bios xdc hdvpss

MODULE_SRC_BASE_PATH := $(hdvpss_PATH)/packages/ti/psp/devices

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON =
CFLAGS_LOCAL_COMMON = $(HDVPSS_CFLAGS)

include $(MODULE_SRC_BASE_PATH)/SRC_FILES.MK
#include $(MODULE_SRC_BASE_PATH)/tvp5158/SRC_FILES.MK
#include $(MODULE_SRC_BASE_PATH)/ph3100k/SRC_FILES.MK
# added by jhhan
include $(MODULE_SRC_BASE_PATH)/nvp2440h/SRC_FILES.MK

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
include $(ROOTDIR)/makerules/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
