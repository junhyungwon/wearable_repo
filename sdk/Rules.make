# ============================================================================
# Copyright (c) Texas Instruments Inc 2012
#
# Use of this software is controlled by the terms and conditions found in the
# license agreement under which this software has been supplied or provided.
# ============================================================================

# Default build environment, windows or linux
ifeq ($(OS), )
  OS := Linux
endif

#####################################################################################
# CONFIGURATION
#####################################################################################
#--- Select SYSTEM PLATFORM -------------------------------
#SYSTEM_PLATFORM := FITT360_BASIC
#SYSTEM_PLATFORM := FITT360
#SYSTEM_PLATFORM := NEXX360B
#SYSTEM_PLATFORM := NEXX360W
SYSTEM_PLATFORM := NEXXONE_VOIP
######################################################################################

#--- Build Type (debug/release)
APP_BUILD_CFG 	 := debug

#----- select board type -------------------------
RDK_BOARD_TYPE := DM8107_UBX
#-------------------------------------------------

#--- UBX Target
UBX_TARGET := UBX_CAR

#--- Default platform
PLATFORM := ti810x-evm

#--- PSP Target
SYSTEM_CFG := fit

#--- Filesystem Mode
FS_CFG := ubifs

#--- Memory foot print (256M/512M)
MEMORY_CONFIG := 512M

ifeq ($(MEMORY_CONFIG), 256M)
    MEMORY_256MB := YES
else
    MEMORY_256MB := NO
endif

#--- ISP Feature (Fixed)
USE_ISP := NO

#--- Image Sensor (IMGS_SONY_IMX122/IMGS_MICRON_AR0331)
IMGS_ID := IMGS_SONY_IMX122

#--- SWOSD Operation Configuration (IMX_BASED/NON_IMX_BASED)
OSD_MODE := NON_IMX_BASED

#---- TILER Memory enable (YES/NO)
TILER_ENABLE := NO

#--- Teardown method (YES/NO)
TEARDOWN_LOAD_UNLOAD := YES

ifeq ($(SYSTEM_PLATFORM), FITT360_BASIC)
	#--- Select Wi-Fi method (YES/NO)
	USE_WIFI = NO
	#--- Select VOIP method (YES/NO)
	USE_VOIP := NO
	#---- IMAGE Manufacturer (AFO/PARTRON)
	CAM_MANUFACTURER := AFO
endif
ifeq ($(SYSTEM_PLATFORM), FITT360)
	#--- Select Wi-Fi method (YES/NO)
	USE_WIFI = YES
	#--- Select VOIP method (YES/NO)
	USE_VOIP := NO
	#---- IMAGE Manufacturer (AFO/PARTRON)
	CAM_MANUFACTURER := AFO
endif
ifeq ($(SYSTEM_PLATFORM), NEXX360B)
	#--- Select Wi-Fi method (YES/NO)
	USE_WIFI = YES
	#--- Select VOIP method (YES/NO)
	USE_VOIP := NO
	#---- IMAGE Manufacturer (AFO/PARTRON)
	CAM_MANUFACTURER := AFO
endif
ifeq ($(SYSTEM_PLATFORM), NEXX360W)
	#--- Select Wi-Fi method (YES/NO)
	USE_WIFI = YES
	#--- Select VOIP method (YES/NO)
	USE_VOIP = YES
	#---- IMAGE Manufacturer (AFO/PARTRON)
	CAM_MANUFACTURER  = AFO
endif
ifeq ($(SYSTEM_PLATFORM), NEXXONE_VOIP)
	#--- Select Wi-Fi method (YES/NO)
	USE_WIFI = YES
	#--- Select VOIP method (YES/NO)
	USE_VOIP = YES
	#---- IMAGE Manufacturer (AFO/PARTRON)
	CAM_MANUFACTURER  = PARTRON
endif

################################################################################
################################################################################
ifeq (exist, $(shell [ -e myRules.make ] && echo exist))
include myRules.make
BASE_INSTALL_DIR  := $(BUILD_ROOT)
else
BASE_INSTALL_DIR  := $(shell pwd)/..
endif

# Defining the install base directory for RDK
RDK_INSTALL_DIR   := $(BASE_INSTALL_DIR)/sdk
TOOLS_PRIVATE_DIR := $(BASE_INSTALL_DIR)/ti_tools

BUILD_TI_TOOL_DIR := $(BASE_INSTALL_DIR)/ti_tools
BUILD_TOOL_DIR    := $(BUILD_TI_TOOL_DIR)/linux_devkit
BUILD_TOOL_PREFIX := $(BUILD_TOOL_DIR)/bin/arm-arago-linux-gnueabi-

# Defining all the tools that are required for RDK
CODEGEN_PATH_A8   := $(BUILD_TOOL_DIR)
CODEGEN_PATH_DSP  := $(BUILD_TI_TOOL_DIR)/cgt6x_7_3_5
CODEGEN_PATH_M3   := $(BUILD_TI_TOOL_DIR)/cgt470_4_9_5
edma3lld_PATH     := $(BUILD_TI_TOOL_DIR)/edma3_lld_02_11_02_04
fc_PATH           := $(BUILD_TI_TOOL_DIR)/framework_components_3_22_02_08
ipc_PATH          := $(BUILD_TI_TOOL_DIR)/ipc_1_24_03_32
hdvicplib_PATH    := $(BUILD_TI_TOOL_DIR)/ivahd-hdvicp20api_01_00_00_19
linuxdevkit_PATH  := $(BUILD_TI_TOOL_DIR)/linux_devkit/arm-arago-linux-gnueabi
bios_PATH         := $(BUILD_TI_TOOL_DIR)/bios_6_33_05_46
xdais_PATH        := $(BUILD_TI_TOOL_DIR)/xdais_7_22_00_03
xdc_PATH          := $(BUILD_TI_TOOL_DIR)/xdctools_3_23_03_53

# Defining all the build components that are required for RDK
codecs_DIR        := $(TOOLS_PRIVATE_DIR)/codecs-dm814x
syslink_PATH      := $(TOOLS_PRIVATE_DIR)/syslink_2_10_06_28
linuxutils_PATH   := $(TOOLS_PRIVATE_DIR)/linuxutils_3_22_00_02
iss_PATH       	  := $(TOOLS_PRIVATE_DIR)/iss_03_00_00_00
hdvpss_PATH       := $(TOOLS_PRIVATE_DIR)/hdvpss_01_00_01_37

h264enc_DIR       := $(codecs_DIR)/REL.500.V.H264AVC.E.IVAHD.02.00.04.01
h264dec_DIR       := $(codecs_DIR)/REL.500.V.H264AVC.D.HP.IVAHD.02.00.08.00
jpegenc_DIR       := $(codecs_DIR)/REL.500.V.MJPEG.E.IVAHD.01.00.04.00
jpegdec_DIR       := $(codecs_DIR)/REL.500.V.MJPEG.D.IVAHD.01.00.06.00
h264enc_PATH      := $(h264enc_DIR)/500.V.H264AVC.E.IVAHD.02.00/IVAHD_001
h264dec_PATH      := $(h264dec_DIR)/500.V.H264AVC.D.HP.IVAHD.02.00/IVAHD_001
jpegenc_PATH      := $(jpegenc_DIR)/500.V.MJPEG.E.IVAHD.01.00/IVAHD_001
jpegdec_PATH      := $(jpegdec_DIR)/500.V.MJPEG.D.IVAHD.01.00/IVAHD_001

# The directory that points to the Linux Support Package
lsp_PATH          := $(RDK_INSTALL_DIR)/psp
KERNELDIR         := $(lsp_PATH)/kernel
UBOOTDIR          := $(lsp_PATH)/u-boot
KMODDIR		  	  := $(lsp_PATH)/kernel_modules

# The directory that points to where filesystem is mounted
TARGET_ROOT       := $(BASE_INSTALL_DIR)/target
TARGET_FS         := $(TARGET_ROOT)/rfs
TARGET_FS_DIR     := $(TARGET_FS)/opt/bin

# The directory that points RDK source code
APP_DIR           := $(RDK_INSTALL_DIR)/sw_app
MCFW_ROOT_PATH    := $(RDK_INSTALL_DIR)/sw_mcfw
TFTP_HOME     	  := $(RDK_INSTALL_DIR)/bin
APP_INC_DIR       := $(APP_DIR)/app

# The directory that points RDK external package
EXTERNAL_PACKAGE  := $(RDK_INSTALL_DIR)/sw_package

# IPC
ipc_INCLUDE 	  := $(ipc_PATH)/packages

# SYSLINK
syslink_INCLUDE   := $(syslink_PATH)/packages
syslink_LIB_DIR   := $(syslink_INCLUDE)/ti/syslink/lib
syslink_OUT_DIR   := $(syslink_PATH)/packages/ti/syslink/bin/TI814X/

# The directory to MCFW binaries
TARGET_MCFW_DIR:= $(TFTP_HOME)/$(SYSTEM_CFG)

# The directory to CMEM library
CMEM_LIB_DIR:=$(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/lib
CMEM_INC_DIR:=$(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/include

# The directory to root file system
ROOT_FILE_SYS:= $(TARGET_FS)

# Where to copy the resulting executables and data to (when executing 'make
# install') in a proper file structure. This EXEC_DIR should either be visible
# from the target, or you will have to copy this (whole) directory onto the
# target filesystem.
EXEC_DIR:=$(TARGET_FS)/opt/$(SYSTEM_CFG)

# Values are "NO" and "YES"
POWER_OPT_DSP_OFF := YES
POWER_OPT_DSS_OFF := NO

USE_SYSLINK_NOTIFY=0

# Default profile
ifeq ($(PROFILE_m3video), )
  PROFILE_m3video := $(APP_BUILD_CFG)
endif

ifeq ($(PROFILE_m3vpss), )
  PROFILE_m3vpss := $(APP_BUILD_CFG)
endif

# Default klockwork build flag
ifeq ($(KW_BUILD), )
  KW_BUILD := no
endif

XDCPATH = $(bios_PATH)/packages;$(xdc_PATH)/packages;$(ipc_PATH)/packages;$(hdvpss_PATH)/packages;$(fc_PATH)/packages;$(MCFW_ROOT_PATH);$(syslink_PATH)/packages;$(xdais_PATH)/packages;$(edma3lld_PATH)/packages

ifeq (, $(findstring $(UBOOTDIR)/tools, $(PATH)))
export PATH:=$(UBOOTDIR)/tools:$(PATH)
endif

TREAT_WARNINGS_AS_ERROR=no

ROOTDIR := $(MCFW_ROOT_PATH)

export OS
export PLATFORM
export CORE
export SYSTEM_CFG
export FS_CFG
export APP_BUILD_CFG
export PROFILE_m3vpss
export PROFILE_m3video
export CODEGEN_PATH_M3
export CODEGEN_PATH_A8
export CODEGEN_PATH_DSP
export ROOTDIR
export bios_PATH
export xdc_PATH
export iss_PATH
export hdvpss_PATH
export ipc_PATH
export fc_PATH
export xdais_PATH
export h264dec_PATH
export h264enc_PATH
export jpegenc_PATH
export jpegdec_PATH
export hdvicplib_PATH
export linuxdevkit_PATH
export edma3lld_PATH
export XDCPATH
export KW_BUILD
export syslink_PATH
export KERNELDIR
export KMODDIR
export TARGET_ROOT
export TARGET_FS
export TARGET_MCFW_DIR
export ROOT_FILE_SYS
export MCFW_ROOT_PATH
export UBOOTDIR
export RDK_BOARD_TYPE
export USE_SYSLINK_NOTIFY
export DEST_ROOT
export RDK_INSTALL_DIR
export TFTP_HOME
export BUILD_TOOL_DIR
export BUILD_TOOL_PREFIX
export EXEC_DIR
export syslink_INCLUDE
export syslink_LIB_DIR
export ipc_INCLUDE
export linuxutils_PATH
export POWER_OPT_DSP_OFF
export POWER_OPT_DSS_OFF
export IMGS_ID
export NOISE_FILTER_MODE
export OSD_MODE
export TREAT_WARNINGS_AS_ERROR
export CMEM_LIB_DIR
export CMEM_INC_DIR
export FRAMES_TO_A8
export MEMORY_256MB
export TILER_ENABLE
export TARGET_FS_DIR
export TEARDOWN_LOAD_UNLOAD
export APP_DIR
export APP_INC_DIR
export UBX_TARGET
export SYSTEM_PLATFORM
export USE_ISP
export USE_WIFI
export USE_VOIP
export CAM_MANUFACTURER
export EXTERNAL_PACKAGE
export SYSROOT
