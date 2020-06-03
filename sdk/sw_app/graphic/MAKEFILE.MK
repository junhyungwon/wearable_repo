include $(MCFW_ROOT_PATH)/makerules/common_header_a8.mk

INCLUDE+= -I$(KERNELDIR)/usr/include -I$(KERNELDIR)/arch/arm/include/
CC_OPTS= -c -O2 -mfpu=neon -mfloat-abi=softfp $(INCLUDE_DIR) -Wall
CC_OPTS+=-DCONFIG_TI81XX

include $(MCFW_ROOT_PATH)/makerules/common_footer_a8.mk
