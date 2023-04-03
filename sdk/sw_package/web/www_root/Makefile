##### Change the following for your environment######################################
TOOL_PREFIX   = $(BUILD_TOOL_PREFIX)
WWW_DEV_DIR   = $(EXTERNAL_PACKAGE)/web
CROSS_COMPILE = $(TOOL_PREFIX)
TARGET_PATH   = $(TARGET_FS)
UID=$(shell id -u)
WID=www-data
################################################################################

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
#	chown $(WID).$(WID) $(TARGET_PATH)/var/www
#	chown -R $(UID).$(UID) $(TARGET_PATH)/var/www/*
	@echo "--- copying lighttpd configurations and tools..."
	cp -a $(WWW_DEV_DIR)/server/bin/* $(TARGET_PATH)/opt/fit/bin/
	cp -a $(WWW_DEV_DIR)/server/lighttpd/* $(TARGET_PATH)/etc/lighttpd/
	ln -sf /tmp/lighttpd.user $(TARGET_PATH)/etc/lighttpd/lighttpd.user

	@echo ""
	@echo "---copying html, cgi, script and remove old *.js files."
	cp -a $(WWW_DEV_DIR)/www_root/html      $(WWW_DEV_DIR)/www_root/nexx360-settings/
	cp -a $(WWW_DEV_DIR)/www_root/srclib/libLFCGI.so $(TARGET_PATH)/usr/lib/
	cp -a $(WWW_DEV_DIR)/www_root/src/cgi-bin      $(WWW_DEV_DIR)/www_root/nexx360-settings/
	cp -a $(WWW_DEV_DIR)/www_root/default/cgi-bin  $(WWW_DEV_DIR)/www_root/nexx360-settings/default/
	rm -f $(TARGET_PATH)/var/www/default/_nuxt/*
	rm -f $(TARGET_PATH)/var/www/_nuxt/*
	cp -a $(WWW_DEV_DIR)/www_root/nexx360-settings/* $(TARGET_PATH)/var/www/
	@echo ""

