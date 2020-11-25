
include $(MCFW_ROOT_PATH)/makerules/common_header_a8.mk

INCLUDE += -I./inc $(MCFW_INC) $(OSA_INC) $(XDAIS_INC) $(SYSLINK_INC) $(CODEC_INC) $(LINUXDEVKIT_INC) $(DEVICES_INC) $(CMEM_INC) -I$(KERNELDIR)/usr/include
INCLUDE += -I$(APP_DIR)/device/inc
INCLUDE += -I$(APP_DIR)/app
INCLUDE += -I$(APP_DIR)/app/inc

LIBS= $(LIB_DIR)/hw_test.a $(MCFW_LIBS) $(AUDIO_LIBS) $(CMEM_LIB) $(BLKID_LIBS) -L$(SHLIB_DIR) -lfont_db

include $(MCFW_ROOT_PATH)/makerules/common_footer_a8.mk
