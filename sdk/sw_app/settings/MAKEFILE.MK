include $(MCFW_ROOT_PATH)/makerules/common_header_a8.mk

INCLUDE += $(MCFW_INC) $(OSA_INC) -I$(KERNELDIR)/usr/include
INCLUDE += -I../app/inc -I../device/inc

include $(MCFW_ROOT_PATH)/makerules/common_footer_a8.mk