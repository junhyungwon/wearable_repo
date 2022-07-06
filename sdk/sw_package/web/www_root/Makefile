##### Change the following for your environment######################################
TOOL_PREFIX   = $(BUILD_TOOL_PREFIX)
WWW_DEV_DIR   = $(EXTERNAL_PACKAGE)/web
CROSS_COMPILE = $(TOOL_PREFIX)
TARGET_PATH   = $(TARGET_FS)
UID=$(shell id -u)
WID=www-data
################################################################################

# web root directory 생성
WWW_ROOT_DIR = $(TARGET_PATH)/var/www
HAVE_CGI_BIN_DIR = $(shell if [ -e $(WWW_ROOT_DIR) ]; then echo 1; else echo 0; fi;)
ifeq ($(HAVE_CGI_BIN_DIR),0)
$(shell sudo mkdir -p $(WWW_ROOT_DIR))
else
#$(info *********** INFO $(WWW_ROOT_DIR) exist.)
endif

VAR_LOG_DIR = $(TARGET_PATH)/var/log
HAVE_VAR_LOG_DIR = $(shell if [ -e $(VAR_LOG_DIR) ]; then echo 1; else echo 0; fi;)
ifeq ($(HAVE_VAR_LOG_DIR),0)
$(shell sudo mkdir -p $(VAR_LOG_DIR))
endif
VAR_RUN_DIR = $(TARGET_PATH)/var/run
HAVE_VAR_RUN_DIR = $(shell if [ -e $(VAR_RUN_DIR) ]; then echo 1; else echo 0; fi;)
ifeq ($(HAVE_VAR_RUN_DIR),0)
$(shell sudo mkdir -p $(VAR_RUN_DIR))
endif

# file system의 /opt/fit/bin directory 생성, 여기서 할 일은 아닌것 같은데...
OPT_BIN_DIR = $(TARGET_PATH)/opt/fit/bin
HAVE_OPT_BIN_DIR = $(shell if [ -e $(OPT_BIN_DIR) ]; then echo 1; else echo 0; fi;)
ifeq ($(HAVE_OPT_BIN_DIR),0)
$(shell sudo mkdir -p $(OPT_BIN_DIR))
else
#$(info *********** INFO $(OPT_BIN_DIR) exist.)
endif

#모델명 첵크
all:
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/web/www_root/srclib
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/web/www_root/default
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/web/www_root/src
	make install
	echo $(MAKE)

clean:
	-cd srclib; make clean;
	-cd default; make clean;
	-cd src; make clean;

install:
	@echo ""
#	sudo chown $(WID).$(WID) $(TARGET_PATH)/var/www
#	sudo chown -R $(UID).$(UID) $(TARGET_PATH)/var/www/*
	@echo "--- copying lighttpd configurations and tools..."
	sudo cp -a $(WWW_DEV_DIR)/server/bin/* $(TARGET_PATH)/opt/fit/bin/
	sudo cp -a $(WWW_DEV_DIR)/server/lighttpd/* $(TARGET_PATH)/etc/lighttpd/
	sudo ln -sf /tmp/lighttpd.user $(TARGET_PATH)/etc/lighttpd/lighttpd.user

	@echo ""
	@echo "---copying html, cgi, script and remove old *.js files."
	sudo cp -a $(WWW_DEV_DIR)/www_root/html      $(WWW_DEV_DIR)/www_root/nexx360-settings/
	sudo cp -a $(WWW_DEV_DIR)/www_root/srclib/libLFCGI.so $(TARGET_PATH)/usr/lib/
	sudo cp -a $(WWW_DEV_DIR)/www_root/src/cgi-bin      $(WWW_DEV_DIR)/www_root/nexx360-settings/
	sudo cp -a $(WWW_DEV_DIR)/www_root/default/cgi-bin  $(WWW_DEV_DIR)/www_root/nexx360-settings/default/
	sudo rm -f $(TARGET_PATH)/var/www/default/_nuxt/*
	sudo rm -f $(TARGET_PATH)/var/www/_nuxt/*
	sudo cp -a $(WWW_DEV_DIR)/www_root/nexx360-settings/* $(TARGET_PATH)/var/www/
	@echo ""

