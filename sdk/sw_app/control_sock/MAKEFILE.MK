
include $(MCFW_ROOT_PATH)/makerules/common_header_a8.mk

INCLUDE += -I. $(MCFW_INC) $(OSA_INC) -I$(KERNELDIR)/usr/include
INCLUDE += -I$(APP_INC_DIR)/inc -I$(APP_DIR)/device/inc

include $(MCFW_ROOT_PATH)/makerules/common_footer_a8.mk
