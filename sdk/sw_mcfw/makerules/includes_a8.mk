# (c) Texas Instruments

ifndef $(INCLUDES_MK)
INCLUDES_MK = 1

ISS_INC=-I$(iss_PATH)

#ALG_INC=-I$(MCFW_ROOT_PATH)/mcfw/src_linux/links/alg/

#GLBCE_INC=-I$(MCFW_ROOT_PATH)/mcfw/src_linux/links/alg/glbce/inc

#GLBCE_LIBS=$(MCFW_ROOT_PATH)/mcfw/src_linux/links/alg/glbce/lib/libGlbceSupport_ti.a

OSA_INC=-I$(MCFW_ROOT_PATH)/mcfw/src_linux/osa/inc

DEVICES_INC=-I$(MCFW_ROOT_PATH)/mcfw/src_linux/devices/inc

OSA_KERMOD_INC=-I$(MCFW_ROOT_PATH)/mcfw/src_linux/osa_kermod/inc

SYSLINK_INC=-I$(syslink_PATH)/packages -I$(ipc_PATH)/packages

CODEC_INC=-I$(xdais_PATH)/packages -I$(h264dec_PATH)/Inc -I$(h264enc_PATH)/Inc

LINUXDEVKIT_INC=-I$(linuxdevkit_PATH)/usr/include

MCFW_INC=-I$(MCFW_ROOT_PATH)/mcfw/interfaces -I$(MCFW_ROOT_PATH)/mcfw/interfaces/common_def

XDAIS_INC=-I$(xdais_PATH)/packages

COMMON_INC=-I. -I../inc -I../priv -I$(MCFW_ROOT_PATH) -I$(APP_DIR) -I$(APP_INC_DIR) -I$(APP_DIR)/device/inc -I$(APP_DIR)/graphic -I$(APP_DIR)/font -I$(APP_DIR)/module/rtsptx -I$(APP_DIR)/control_sock -I$(APP_DIR)/app/$(APP_DIR_NAME)/inc
MODULE_INC=-I$(APP_DIR)/module/iw_scan

CMEM_INC=-I$(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/include

SYSLINK_LIBS=$(syslink_PATH)/packages/ti/syslink/lib/syslink.a_release

AUDIO_LIBS=$(linuxdevkit_PATH)/usr/lib/libasound.so.2

MCFW_LIBS=$(LIB_DIR)/ipnc_rdk_mcfw_api.a $(LIB_DIR)/ipnc_rdk_link_api.a $(LIB_DIR)/ipnc_rdk_osa.a $(SYSLINK_LIBS) $(GLBCE_LIBS)

SHLIB_DIR=$(APP_DIR)/lib

CMEM_LIB=$(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/lib/cmem.a470MV

BLKID_LIBS=$(linuxdevkit_PATH)/usr/lib/libblkid.so.1.1.0
endif # ifndef $(INCLUDES_MK)

