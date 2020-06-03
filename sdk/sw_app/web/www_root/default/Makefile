################################################################################
include ../../webRules.make
WWW_DEV_DIR   = $(BUILD_ROOT)/sdk/sw_app/web
################################################################################

all: clean
	cd src; make

clean:
	$(RM) $(WWW_DEV_DIR)/www_root/default/cgi-bin/*
	cd src ; make clean

# real copying cgi files to tgarget rfs는 상위 Makefile에서 합니다.
install: clean
	cd src;make install
