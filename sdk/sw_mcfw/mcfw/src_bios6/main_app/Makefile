
APP_NAME = ipnc_rdk

SRCDIR = src
INCDIR = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXERNAL_INTERFACES = bios xdc iss hdvpss ipc syslink ipnc_rdk fc xdais h264enc h264dec swosd edma3lld mpeg4enc

# List all the components required by the application
COMP_LIST_m3vpss  = hdvpss iss ipnc_rdk ipc syslink
COMP_LIST_m3video = ipnc_rdk ipc syslink
COMP_LIST_c6xdsp  = ipnc_rdk ipc syslink

# XDC CFG File
XDC_CFG_FILE_m3vpss  = MAIN_APP_m3vpss.cfg
XDC_CFG_FILE_m3video = MAIN_APP_m3video.cfg
XDC_CFG_FILE_c6xdsp  = MAIN_APP_c6xdsp.cfg

# Common source files and CFLAGS across all platforms and cores
SRCS_m3vpss  = main_m3vpss.c  xdc2c_cfg_m3vpss.c   edma3OsFunctions.c
#SRCS_m3vpss += vicp_edma3_M3_cfg.c
SRCS_m3video = main_m3video.c
SRCS_c6xdsp  = main_c6xdsp.c

CFLAGS_LOCAL_m3vpss  = $(IPNC_RDK_CFLAGS)
CFLAGS_LOCAL_m3video = $(IPNC_RDK_CFLAGS)
CFLAGS_LOCAL_c6xdsp = $(IPNC_RDK_CFLAGS)

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
include $(MCFW_ROOT_PATH)/makerules/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
