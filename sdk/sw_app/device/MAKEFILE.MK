include $(MCFW_ROOT_PATH)/makerules/common_header_a8.mk

INCLUDE+= -I./inc $(LINUXDEVKIT_INC) $(DEVICES_INC) $(OSA_INC) -I$(KERNELDIR)/usr/include

include $(MCFW_ROOT_PATH)/makerules/common_footer_a8.mk
