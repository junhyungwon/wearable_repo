
include $(MCFW_ROOT_PATH)/makerules/common_header_a8.mk

DEFINE += -DMODEL_NAME='$(MODEL_NAME)'
DEFINE += -DMODEL_TYPE='$(MODEL_TYPE)'

INCLUDE += -I./inc $(MCFW_INC) $(OSA_INC) $(XDAIS_INC) $(SYSLINK_INC) $(CODEC_INC) $(LINUXDEVKIT_INC) $(DEVICES_INC) -I$(KERNELDIR)/usr/include $(CMEM_INC)
INCLUDE += -I$(APP_DIR)/device/inc -I$(APP_DIR)/rtsptx -I$(APP_DIR)/control_sock -I$(APP_DIR)/lib/include
INCLUDE += -I$(EXTERNAL_PACKAGE)/iw_scan #for wifi client
INCLUDE += -I$(EXTERNAL_PACKAGE)/web/www_root/include

LIBS= $(LIB_DIR)/app_fitt.a $(MCFW_LIBS) $(AUDIO_LIBS) $(CMEM_LIB) $(BLKID_LIBS)

# app addition
LIBS += -L$(SHLIB_DIR) -lfont_db

include $(MCFW_ROOT_PATH)/makerules/common_footer_a8.mk
