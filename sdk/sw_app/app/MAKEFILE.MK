
include $(MCFW_ROOT_PATH)/makerules/common_header_a8.mk

INCLUDE += -I./inc $(MCFW_INC) $(OSA_INC) $(XDAIS_INC) $(SYSLINK_INC) $(CODEC_INC) $(LINUXDEVKIT_INC) $(DEVICES_INC) -I$(KERNELDIR)/usr/include $(CMEM_INC)
INCLUDE += -I$(APP_DIR)/device/inc -I$(APP_DIR)/rtsptx -I$(APP_DIR)/control_sock -I$(APP_DIR)/lib/include
INCLUDE += -I$(EXTERNAL_PACKAGE)/iw_scan #for wifi client
INCLUDE += -I$(EXTERNAL_PACKAGE)/web/www_root/include
INCLUDE += -I$(EXTERNAL_PACKAGE)/av_rec/
INCLUDE += -I$(APP_DIR)/settings
INCLUDE += -I$(THIRD_PARTY)/libuv/include/
INCLUDE += -I$(THIRD_PARTY)/libuv/src/
INCLUDE += -I$(THIRD_PARTY)/kcmvp/include/
INCLUDE += -I$(THIRD_PARTY)/srs-librtmp/src/srs/

LIBS= $(LIB_DIR)/app_fitt.a $(MCFW_LIBS) $(AUDIO_LIBS) $(CMEM_LIB) $(BLKID_LIBS)

# app addition
LIBS += -L$(SHLIB_DIR) -lfont_db -ljson-c

ifeq ($(USE_KCMVP), YES)
LIBS += -L$(THIRD_PARTY)/kcmvp/linux_dll/2.6_armv7l_32_44_post_limit/ -lMagicCrypto
endif

LIBS += $(THIRD_PARTY)/libuv/libuv_a.a
LIBS += -lpthread
LIBS += $(THIRD_PARTY)/srs-librtmp/src/srs/srs_librtmp.o

LIBS += -L$(TLS_LIBS) -lssl -lcrypto

include $(MCFW_ROOT_PATH)/makerules/common_footer_a8.mk
