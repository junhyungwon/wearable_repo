LOCAL_PATH:= $(call my-dir)
include $(LOCAL_PATH)/../config/mm_config.mk

###############################################################################
# SYSLINK.A
###############################################################################
include $(CLEAR_VARS)

LOCAL_MODULE                := timm_syslink
LOCAL_MODULE_SUFFIX         := .a
LOCAL_MODULE_CLASS          := STATIC_LIBRARIES
LOCAL_MODULE_TAGS           := optional

# Module-specific settings
timm_SYSLINK_PKGPATH  := $(timm_IPC_INSTALL_DIR)/packages;$(timm_SYSLINK_INSTALL_DIR)/packages
timm_ARCH             := arm
timm_SYSLINK_PLATFORM := TI81XX
#timm_SYSLINK_VARIANT  := TI816X
timm_SYSLINK_VARIANT  := TI814X

LOCAL_SRC_FILES := android_binaries/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)

# Specify the header files to copy for use by other applications
CUSTOM_COPY_HEADERS :=  $(wildcard $(LOCAL_PATH)/packages/ti/syslink/*.h) 
CUSTOM_COPY_HEADERS :=  $(patsubst $(LOCAL_PATH)/%,%,$(CUSTOM_COPY_HEADERS))

# The built-in mechanism for copying header files doesn't support the creation
# of sub-directories for header files (like xdc/runtime/knl).  We must
# customize the copying rule to achieve this.  The following is based on
# build/core/copy_headers.mk
$(foreach header,$(CUSTOM_COPY_HEADERS), \
  $(eval _chFrom := $(LOCAL_PATH)/$(header)) \
  $(eval _chTo := \
      $(TARGET_OUT_HEADERS)/$(LOCAL_MODULE)/$(header)) \
  $(eval $(call copy-one-header,$(_chFrom),$(_chTo))) \
  $(eval all_copied_headers: $(_chTo)) \
 )

# Specify variables to be exported to the sub-build
define $(LOCAL_MODULE)_set_config_vars
    $(call timm_set_config_var1,SYSLINK_ROOT) \
    $(call timm_set_config_var1,SYSLINK_PKGPATH) \
    $(call timm_set_config_var1,ARCH) \
    $(call timm_set_config_var2,CROSS_COMPILE,TOOLCHAIN_PREFIX) \
    $(call timm_set_config_var1,TOOLCHAIN_PREFIX) \
    $(call timm_set_config_var1,SYSLINK_PLATFORM) \
    $(call timm_set_config_var1,SYSLINK_VARIANT) \
    $(call timm_set_config_var2,KDIR,LINUXKERNEL_INSTALL_DIR) \
    $(call timm_set_config_var2,IPC_DIR,IPC_INSTALL_DIR) \
    $(call timm_set_config_var2,ANDROID_ROOT,ANDROID_INSTALL_DIR) \
    $(call timm_set_config_var2,ANDROID_CFLAGS,CFLAGS) \
    $(call timm_set_config_var2,ANDROID_LDFLAGS,LDFLAGS)
endef

# Build against the Android stack
define $(LOCAL_MODULE)_rebuild
$(LOCAL_PATH)/$(LOCAL_SRC_FILES) : all_copied_headers
	$(timm_XDC_INSTALL_DIR)/xdc clean -C $(LOCAL_PATH)/packages/ti/syslink
	$(MAKE) -C $(LOCAL_PATH)/packages/ti/syslink/utils/hlos/knl/Linux \
	$(call $(LOCAL_MODULE)_set_config_vars) \
	clean
	$(MAKE) -C $(LOCAL_PATH)/packages/ti/syslink/utils/hlos/usr/Linux \
	$(call $(LOCAL_MODULE)_set_config_vars) \
	clean
	$(MAKE) -C $(LOCAL_PATH)/packages/ti/syslink/utils/hlos/knl/Linux \
	SYSLINK_BUILDOS="Linux" \
	$(call $(LOCAL_MODULE)_set_config_vars)
	$(MAKE) -C $(LOCAL_PATH)/packages/ti/syslink/utils/hlos/usr/Linux \
	$(call $(LOCAL_MODULE)_set_config_vars) SYSLINK_BUILDOS="Android"
	$(timm_XDC_INSTALL_DIR)/xdc -C $(LOCAL_PATH)/packages/ti/syslink .interfaces
	mkdir -p $$(dir $$@)
	cp -fp $(LOCAL_PATH)/packages/ti/syslink/lib/syslink.a_release $$@
endef
$(eval $(call $(LOCAL_MODULE)_rebuild))

include $(BUILD_PREBUILT)

# Cleanup
timm_SYSLINK_PKGPATH            :=
timm_ARCH                       :=
timm_SYSLINK_PLATFORM           :=
$(LOCAL_MODULE)_set_config_vars :=
$(LOCAL_MODULE)_rebuild         :=
_chFrom                         :=
_chTo                           :=
CUSTOM_COPY_HEADERS_TO          :=
CUSTOM_COPY_HEADERS             :=

###############################################################################
# SYSLINK.KO
###############################################################################
include $(CLEAR_VARS)

LOCAL_MODULE                := timm_syslink_kernel
LOCAL_MODULE_SUFFIX         := .ko
LOCAL_MODULE_CLASS          := SHARED_LIBRARIES
LOCAL_MODULE_TAGS           := eng

LOCAL_SRC_FILES := android_binaries/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)

define $(LOCAL_MODULE)_rebuild
$(LOCAL_PATH)/$(LOCAL_SRC_FILES) : $(LOCAL_PATH)/android_binaries/timm_syslink.a
	cp -fp $(LOCAL_PATH)/packages/ti/syslink/bin/$(timm_SYSLINK_VARIANT)/syslink.ko $$@
endef
$(eval $(call $(LOCAL_MODULE)_rebuild))

include $(BUILD_PREBUILT)

include $(LOCAL_PATH)/../config/mm_config_cleanup.mk

