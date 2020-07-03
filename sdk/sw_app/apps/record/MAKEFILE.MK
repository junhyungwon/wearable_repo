
include $(MCFW_ROOT_PATH)/makerules/common_header_a8.mk

INCLUDE += -I./inc $(MCFW_INC) $(OSA_INC) $(XDAIS_INC) $(SYSLINK_INC) $(CODEC_INC) $(LINUXDEVKIT_INC) $(DEVICES_INC) -I$(KERNELDIR)/usr/include $(CMEM_INC)

LIBS= $(LIB_DIR)/app_rec.a $(MCFW_LIBS) $(AUDIO_LIBS) $(CMEM_LIB) $(BLKID_LIBS)

#LIBS += -L$(SHLIB_DIR) -lmulti_avi -lfont_db

include $(MCFW_ROOT_PATH)/makerules/common_footer_a8.mk
