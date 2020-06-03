#Disable XDC package build by default
ENABLE_XDC_PKG_BUILD ?= false
XDC_BUILD_PKG_LIST_COMMON += $(fc_PATH)/packages/ti.sdo.fc.dskt2
XDC_BUILD_PKG_LIST_M3      = $(fc_PATH)/packages/ti.sdo.fc.ires.hdvicp
XDC_BUILD_PKG_LIST_M3      += $(XDC_BUILD_PKG_LIST_COMMON)

FIRMWARE_OUT_PATH=$(TARGET_MCFW_DIR)/firmware/
XEM3_IN_PATH=$(DEST_ROOT)/ipnc_rdk/bin/$(PLATFORM)/

CPU_NUM=8

#---------------------------------------------------------------------
m3vpss:
	$(MAKE) -j$(CPU_NUM) -C $(MCFW_ROOT_PATH)/mcfw/src_bios6 CORE=m3vpss
	$(MAKE) -j$(CPU_NUM) -C $(MCFW_ROOT_PATH)/mcfw/src_bios6/main_app xdc_configuro CORE=m3vpss
	$(MAKE) -C $(MCFW_ROOT_PATH)/mcfw/src_bios6/main_app CORE=m3vpss
	-mkdir -p $(FIRMWARE_OUT_PATH)
	cp $(XEM3_IN_PATH)/ipnc_rdk_m3vpss_$(PROFILE_m3vpss).xem3 $(FIRMWARE_OUT_PATH)/sdk_fw_m3vpss.xem3

m3vpss_clean:
	$(MAKE) -C $(MCFW_ROOT_PATH)/mcfw/src_bios6 clean CORE=m3vpss
	$(MAKE) -C $(MCFW_ROOT_PATH)/mcfw/src_bios6/main_app clean CORE=m3vpss

#---------------------------------------------------------------------
m3video:
	$(MAKE) -C $(MCFW_ROOT_PATH)/mcfw/src_bios6 CORE=m3video
	$(MAKE) -C $(MCFW_ROOT_PATH)/mcfw/src_bios6/main_app xdc_configuro CORE=m3video
	$(MAKE) -C $(MCFW_ROOT_PATH)/mcfw/src_bios6/main_app all CORE=m3video
	-mkdir -p $(FIRMWARE_OUT_PATH)
	cp $(XEM3_IN_PATH)/ipnc_rdk_m3video_$(PROFILE_m3video).xem3 $(FIRMWARE_OUT_PATH)/sdk_fw_m3video.xem3

m3video_clean:
	$(MAKE) -C $(MCFW_ROOT_PATH)/mcfw/src_bios6 clean CORE=m3video
	$(MAKE) -C $(MCFW_ROOT_PATH)/mcfw/src_bios6/main_app clean CORE=m3video

#---------------------------------------------------------------------
apps: m3vpss m3video
clean: m3vpss_clean m3video_clean

all: clean apps
