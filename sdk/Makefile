# Copyright (c) Texas Instruments Inc 2012
DIR=default
FILE=default
CPU_NUM=4

include Rules.make
include $(ROOTDIR)/makerules/build_config.mk
include $(ROOTDIR)/makerules/env.mk
include $(ROOTDIR)/makerules/platform.mk
include $(ROOTDIR)/component.mk

##### APP BUILD #####

app:
	$(MAKE) libuv
	$(MAKE) librtmp
	$(MAKE) -fMAKEFILE.MK -C$(MCFW_ROOT_PATH)/mcfw/src_linux
	$(MAKE) -fMAKEFILE.MK -C$(APP_DIR) app
	@echo -e "\n--- make app done! (`date +'%H:%M:%S'`)---\n"

app_clean:
	$(MAKE) -fMAKEFILE.MK -C$(MCFW_ROOT_PATH)/mcfw/src_linux clean
	$(MAKE) -fMAKEFILE.MK -C$(APP_DIR) clean

rfs_unzip:
	sudo tar zxvf $(TARGET_ROOT)/rfs.tar.gz -C $(TARGET_ROOT)/

rfs_clean:
	sudo rm -rf $(TARGET_ROOT)/rfs

bin_clean:
	rm -rf $(TFTP_HOME)/MLO
	rm -rf $(TFTP_HOME)/boot.scr
	rm -rf $(TFTP_HOME)/rfs_fit.ubifs
	sudo rm -rf $(TFTP_HOME)/u-boot_fit.bin
	sudo rm -rf $(TFTP_HOME)/u-boot_fit-min.nand
	rm -rf $(TFTP_HOME)/uImage_fit
	rm -rf $(TARGET_MCFW_DIR)/bin/*

bigfile_unzip:
ifneq (exist, $(shell [ -e $(linuxdevkit_PATH)/usr/lib/libc.a ] && echo exist))
	echo "extracting libc.tar.gz..."
	tar zxvf $(linuxdevkit_PATH)/usr/lib/libc.tar.gz -C $(linuxdevkit_PATH)/usr/lib/
endif

app_all: app_clean app

app_test:
	$(MAKE) -fMAKEFILE.MK -C$(APP_DIR) hw_test
	$(MAKE) -fMAKEFILE.MK -C$(APP_DIR) hw_diag
	@echo -e "\n--- make app done! (`date +'%H:%M:%S'`)---\n"

app_test_clean:
	$(MAKE) -fMAKEFILE.MK -C$(APP_DIR) clean_test
	$(MAKE) -fMAKEFILE.MK -C$(APP_DIR) clean_diag

hw_test: app_test fsupdate
hw_test_all: app_test_clean hw_test

##### www component build #####
app_web:
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/web/www_root all

app_web_clean:
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/web/www_root clean

voip:
ifeq ($(USE_VOIP),YES)
	$(MAKE) -fMakefile CC=$(BUILD_TOOL_PREFIX)gcc -C $(EXTERNAL_PACKAGE)/voip/re all
	$(MAKE) -fMakefile CC=$(BUILD_TOOL_PREFIX)gcc -C $(EXTERNAL_PACKAGE)/voip/rem all
	$(MAKE) -fMakefile CC=$(BUILD_TOOL_PREFIX)gcc -C $(EXTERNAL_PACKAGE)/voip/baresip all
endif
voip_clean:
ifeq ($(USE_VOIP),YES)
	$(MAKE) -fMakefile CC=$(BUILD_TOOL_PREFIX)gcc -C $(EXTERNAL_PACKAGE)/voip/re clean
	$(MAKE) -fMakefile CC=$(BUILD_TOOL_PREFIX)gcc -C $(EXTERNAL_PACKAGE)/voip/rem clean
	$(MAKE) -fMakefile CC=$(BUILD_TOOL_PREFIX)gcc -C $(EXTERNAL_PACKAGE)/voip/baresip clean
endif
av_rec:
	$(MAKE) -fMakefile CC=$(BUILD_TOOL_PREFIX)gcc -C $(EXTERNAL_PACKAGE)/av_rec all

av_rec_clean:
	$(MAKE) -fMakefile CC=$(BUILD_TOOL_PREFIX)gcc -C $(EXTERNAL_PACKAGE)/av_rec clean

gnss:
	$(MAKE) -fMakefile CC=$(BUILD_TOOL_PREFIX)gcc -C $(EXTERNAL_PACKAGE)/gnss all

gnss_clean:
	$(MAKE) -fMakefile CC=$(BUILD_TOOL_PREFIX)gcc -C $(EXTERNAL_PACKAGE)/gnss clean

ubxapp: app app_web netdev voip av_rec gnss app_test fsupdate
ubxapp_clean: app_clean app_web_clean netdev_clean voip_clean av_rec_clean gnss_clean app_test_clean
ubxapp_all: ubxapp_clean ubxapp

##### EXTERNAL OPENSOURCE PACKAGE BUILD #####

libavi:
	$(MAKE) CROSS_COMPILE=$(BUILD_TOOL_PREFIX) -C $(EXTERNAL_PACKAGE)/avi/src all
	$(MAKE) CROSS_COMPILE=$(BUILD_TOOL_PREFIX) -C $(EXTERNAL_PACKAGE)/avi all

libavi_clean:
	$(MAKE) CROSS_COMPILE=$(BUILD_TOOL_PREFIX) -C $(EXTERNAL_PACKAGE)/avi/src clean
	$(MAKE) CROSS_COMPILE=$(BUILD_TOOL_PREFIX) -C $(EXTERNAL_PACKAGE)/avi clean

netdev:
	$(MAKE) -fMakefile -C$(EXTERNAL_PACKAGE)/network_manager all

netdev_clean:
	$(MAKE) -fMakefile -C$(EXTERNAL_PACKAGE)/network_manager clean

live555:
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/BasicUsageEnvironment
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/UsageEnvironment
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/groupsock
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/liveMedia
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/BackChannel
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/netra_streamer/netra_interface
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/netra_streamer

live555_clean:
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/BasicUsageEnvironment clean
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/liveMedia clean
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/UsageEnvironment clean
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/groupsock clean
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/BackChannel clean
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/netra_streamer/netra_interface clean
	$(MAKE) -fMakefile -C $(EXTERNAL_PACKAGE)/live555/netra_streamer clean

live555_all: live555_clean live555

ntpclient:
	$(MAKE) platform=ti -fMakefile -C $(EXTERNAL_PACKAGE)/ntpclient

ntpclient_install:
	$(MAKE) platform=ti -fMakefile -C $(EXTERNAL_PACKAGE)/ntpclient install

ntpclient_clean:
	$(MAKE) platform=ti -fMakefile -C $(EXTERNAL_PACKAGE)/ntpclient clean

ntpclient_all: ntpclient_clean ntpclient ntpclient_install

process_check:
	$(MAKE) platform=ti -fMakefile -C $(EXTERNAL_PACKAGE)/proc_check

process_check_install:
	$(MAKE) platform=ti -fMakefile -C $(EXTERNAL_PACKAGE)/proc_check install

process_check_clean:
	$(MAKE) platform=ti -fMakefile -C $(EXTERNAL_PACKAGE)/proc_check clean

process_check_all: process_check_clean process_check process_check_install

p2p:
	$(MAKE) platform=ti -fMakefile -C $(EXTERNAL_PACKAGE)/p2p/samples/Linux

p2p_install:
	$(MAKE) platform=ti -fMakefile -C $(EXTERNAL_PACKAGE)/p2p/samples/Linux install

p2p_clean:
	$(MAKE) platform=ti -fMakefile -C $(EXTERNAL_PACKAGE)/p2p/samples/Linux clean

p2p_all: p2p_clean p2p p2p_install

libuv:
	@cd $(THIRD_PARTY)/libuv \
	&& CFLAGS=-O3 CC=$(BUILD_TOOL_PREFIX)gcc cmake . \
	&& $(MAKE) -j$(CPU_NUM)

libuv_clean:
	@cd $(THIRD_PARTY)/libuv \
	&& $(MAKE) clean \
	&& rm -rf CMakeFiles CMakeCache.txt

librtmp:
	@cd $(THIRD_PARTY)/srs-librtmp/src/srs/ \
	&& $(BUILD_TOOL_PREFIX)gcc -O3 -c srs_librtmp.cpp

librtmp_clean:
	rm -rf $(THIRD_PARTY)/srs-librtmp/src/srs/srs_librtmp.o

app_pkg: libavi live555

app_pkg_clean: libavi_clean live555_clean

##### MCFW BUILD #####
mcfw: hdvpss m3 app
mcfw_all: hdvpss_all m3_all app_all

#### M3 BUILD #####

m3vpss:
	$(MAKE) -fMAKEFILE.MK -C $(MCFW_ROOT_PATH)/mcfw/src_bios6 $@
	$(STRIP470) $(TARGET_MCFW_DIR)/firmware/sdk_fw_m3vpss.xem3
	@echo -e "\n--- make m3vpss done! (`date +'%H:%M:%S'`)---\n"

m3video:
	$(MAKE) -fMAKEFILE.MK -C $(MCFW_ROOT_PATH)/mcfw/src_bios6 $@
	$(STRIP470) $(TARGET_MCFW_DIR)/firmware/sdk_fw_m3video.xem3
	@echo -e "\n--- make m3video done! (`date +'%H:%M:%S'`)---\n"

m3vpss_clean:
	$(MAKE) -fMAKEFILE.MK -C $(MCFW_ROOT_PATH)/mcfw/src_bios6 $@
	rm -rf $(TARGET_MCFW_DIR)/firmware/sdk_fw_m3vpss.xem3

m3video_clean:
	$(MAKE) -fMAKEFILE.MK -C $(MCFW_ROOT_PATH)/mcfw/src_bios6 $@
	rm -rf $(TARGET_MCFW_DIR)/firmware/sdk_fw_m3video.xem3

m3: m3vpss m3video
m3_clean: m3vpss_clean m3video_clean
m3_all: m3_clean m3

##### ISS BUILD #####

iss:
	$(MAKE) -C $(iss_PATH)/packages/ti/psp/iss $(TARGET) CORE=m3vpss
	@echo -e "\n--- make iss done! (`date +'%H:%M:%S'`)---\n"

iss_clean:
	$(MAKE) iss TARGET=clean

iss_all: iss_clean iss

##### HDVPSS BUILD #####

hdvpss:
	$(MAKE) -C $(hdvpss_PATH)/packages/ti/psp/vps $(TARGET) CORE=m3vpss
	$(MAKE) -C $(hdvpss_PATH)/packages/ti/psp/i2c $(TARGET) CORE=m3vpss
	$(MAKE) -C $(hdvpss_PATH)/packages/ti/psp/devices $(TARGET) CORE=m3vpss
	$(MAKE) -C $(hdvpss_PATH)/packages/ti/psp/platforms $(TARGET) CORE=m3vpss
	$(MAKE) -C $(hdvpss_PATH)/packages/ti/psp/proxyServer $(TARGET) CORE=m3vpss
	@echo -e "\n--- make hdvpss done! (`date +'%H:%M:%S'`)---\n"

hdvpss_clean:
	$(MAKE) hdvpss TARGET=clean

hdvpss_all: hdvpss_clean hdvpss

##### U-BOOT BUILD #####

ubootbuild:
	$(MAKE) -j$(CPU_NUM) -C$(UBOOTDIR) ARCH=arm CROSS_COMPILE=$(BUILD_TOOL_PREFIX) $(MAKE_TARGET)

ubootminsd:
	$(MAKE) uboot_clean
	$(MAKE) ubootbuild MAKE_TARGET=ti8107_$(SYSTEM_CFG)_min_sd
	$(MAKE) ubootbuild MAKE_TARGET=u-boot.ti
	mv $(UBOOTDIR)/u-boot.min.sd $(TFTP_HOME)/MLO

ubootminnand:
	$(MAKE) uboot_clean
	$(MAKE) ubootbuild MAKE_TARGET=ti8107_$(SYSTEM_CFG)_min_nand
	$(MAKE) ubootbuild MAKE_TARGET=u-boot.ti
	mv $(UBOOTDIR)/u-boot.min.nand $(TFTP_HOME)/u-boot_$(SYSTEM_CFG).min.nand

ubootbin:
	$(MAKE) uboot_clean
	$(MAKE) ubootbuild MAKE_TARGET=ti8107_$(SYSTEM_CFG)_config_nand
	$(MAKE) ubootbuild MAKE_TARGET=u-boot.ti
	mv $(UBOOTDIR)/u-boot.bin $(TFTP_HOME)/u-boot_$(SYSTEM_CFG).bin

uboot_clean:
	$(MAKE) ubootbuild MAKE_TARGET=distclean

uboot:
	-mkdir -p $(TFTP_HOME)
	sudo chmod 755 $(TFTP_HOME)
	$(MAKE) ubootminsd
	$(MAKE) ubootminnand
	$(MAKE) ubootbin
	$(MAKE) ubootscr
	@echo -e "\n--- make uboot done! (`date +'%H:%M:%S'`)---\n"

ubootscr:
ifeq ($(SYSTEM_PLATFORM),NEXX360W_CCTV_SA)
	mkimage -A arm -O linux -T script -C none -a 0 -e 0 -n 'Execute Boot Script' -d $(UBOOTDIR)/bootscript_sa.sh $(UBOOTDIR)/boot.scr
else
ifeq ($(SYSTEM_PLATFORM),NEXXONE_CCTV_SA)
	mkimage -A arm -O linux -T script -C none -a 0 -e 0 -n 'Execute Boot Script' -d $(UBOOTDIR)/bootscript_sa.sh $(UBOOTDIR)/boot.scr
else
ifeq ($(USE_CCTV),YES)
	mkimage -A arm -O linux -T script -C none -a 0 -e 0 -n 'Execute Boot Script' -d $(UBOOTDIR)/bootscript_cctv.sh $(UBOOTDIR)/boot.scr
else
ifeq ($(EXTERNAL_BATTERY_ONLY),YES)
	mkimage -A arm -O linux -T script -C none -a 0 -e 0 -n 'Execute Boot Script' -d $(UBOOTDIR)/bootscript_extb.sh $(UBOOTDIR)/boot.scr
else
	mkimage -A arm -O linux -T script -C none -a 0 -e 0 -n 'Execute Boot Script' -d $(UBOOTDIR)/bootscript_fit.sh $(UBOOTDIR)/boot.scr
endif
endif
endif
endif
	mv $(UBOOTDIR)/boot.scr $(TFTP_HOME)/boot.scr

uboot_all: uboot_clean uboot ubootscr

##### LSP BUILD #####

lspbuild:
	$(MAKE) -j$(CPU_NUM) -C$(KERNELDIR) ARCH=arm CROSS_COMPILE=$(BUILD_TOOL_PREFIX) $(MAKE_TARGET)

lspmenu:
	$(MAKE) lspcfg
	$(MAKE) lspbuild MAKE_TARGET=menuconfig

lspcfg:
	$(MAKE) lspbuild MAKE_TARGET=ti8107_$(SYSTEM_CFG)_defconfig

lspmodule:
	$(MAKE) lspbuild MAKE_TARGET=modules
	cp $(KERNELDIR)/drivers/video/ti81xx/vpss/vpss.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/drivers/video/ti81xx/ti81xxhdmi/ti81xxhdmi.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/drivers/video/ti81xx/ti81xxfb/ti81xxfb.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/drivers/net/usb/rndis_host.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/drivers/net/usb/ax88179_178a.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/sound/drivers/snd-aloop.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/drivers/net/tun.ko $(TARGET_MCFW_DIR)/kermod/.

lsp:
	$(MAKE) lspcfg
	$(MAKE) lspbuild MAKE_TARGET=uImage
	$(MAKE) lspbuild MAKE_TARGET=modules
	$(MAKE) lspbuild MAKE_TARGET=headers_install
	mv $(KERNELDIR)/arch/arm/boot/uImage $(TFTP_HOME)/uImage_$(SYSTEM_CFG)
	-mkdir -p $(TARGET_MCFW_DIR)/kermod
	cp $(KERNELDIR)/drivers/video/ti81xx/vpss/vpss.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/drivers/video/ti81xx/ti81xxhdmi/ti81xxhdmi.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/drivers/video/ti81xx/ti81xxfb/ti81xxfb.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/drivers/net/usb/rndis_host.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/drivers/net/usb/ax88179_178a.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/sound/drivers/snd-aloop.ko $(TARGET_MCFW_DIR)/kermod/.
	cp $(KERNELDIR)/drivers/net/tun.ko $(TARGET_MCFW_DIR)/kermod/.
	@echo -e "\n--- make lsp done! (`date +'%H:%M:%S'`)---\n"

lsp_clean:
	$(MAKE) lspbuild MAKE_TARGET=distclean
	-rm -f $(TARGET_MCFW_DIR)/kermod/*

lsp_all: lsp_clean lsp

##### Wi-Fi Driver BUILD #####
wlan:
ifeq ($(USE_WIFI),YES)
	$(MAKE) -j$(CPU_NUM) ARCH=arm CROSS_COMPILE=$(BUILD_TOOL_PREFIX) KSRC=$(KERNELDIR) -C$(KMODDIR)/wlan/88x1au_driver modules
	$(MAKE) -j$(CPU_NUM) ARCH=arm CROSS_COMPILE=$(BUILD_TOOL_PREFIX) KSRC=$(KERNELDIR) -C$(KMODDIR)/wlan/88x2bu_driver modules
endif

wlan_clean:
ifeq ($(USE_WIFI), YES)
	-rm -f $(TARGET_FS)/lib/modules/8811au.ko
	make ARCH=arm CROSS_COMPILE=$(BUILD_TOOL_PREFIX) KSRC=$(KERNELDIR) -C$(KMODDIR)/wlan/88x1au_driver clean
	-rm -f $(TARGET_FS)/lib/modules/88x2bu.ko
	make ARCH=arm CROSS_COMPILE=$(BUILD_TOOL_PREFIX) KSRC=$(KERNELDIR) -C$(KMODDIR)/wlan/88x2bu_driver clean
endif

wlan_all: wlan_clean wlan

##### Wi-Fi UTILS BUILD #####
wlan_tools:
ifeq ($(USE_WIFI),YES)
	$(MAKE) -j$(CPU_NUM) CC=$(BUILD_TOOL_PREFIX)gcc -C$(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/hostapd all
	$(MAKE) -j$(CPU_NUM) CC=$(BUILD_TOOL_PREFIX)gcc -C$(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/hostapd clean
	$(MAKE) -j$(CPU_NUM) CC=$(BUILD_TOOL_PREFIX)gcc -C$(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/wpa_supplicant all
	$(MAKE) -j$(CPU_NUM) CC=$(BUILD_TOOL_PREFIX)gcc -C$(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/wpa_supplicant clean
endif

wlan_tools_clean:
ifeq ($(USE_WIFI), YES)
	$(MAKE) -j$(CPU_NUM) CC=$(BUILD_TOOL_PREFIX)gcc -C$(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/hostapd clean
	$(MAKE) -j$(CPU_NUM) CC=$(BUILD_TOOL_PREFIX)gcc -C$(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/wpa_supplicant clean
endif

wlan_tools_all: wlan_tools_clean wlan_tools

##### PSP (uboot, kernel, wlan) BUILD #####
psp: uboot lsp wlan

psp_clean: uboot_clean lsp_clean wlan_clean

psp_all: psp_clean psp

##### SYSLINK BUILD #####

SYSLINK_BUILD_OPT = USE_SYSLINK_NOTIFY=0 SYSLINK_PLATFORM=TI81XX SYSLINK_VARIANT=TI814X

syslinkbuild:
	cp $(MCFW_ROOT_PATH)/makerules/syslink_products.mak $(syslink_PATH)/products.mak
	$(MAKE) -j$(CPU_NUM) -C$(syslink_PATH) $(SYSLINK_BUILD_OPT) $(TARGET)

syslink:
	$(MAKE) syslinkbuild DEVICE=TI814X TARGET=syslink
	-mkdir -p $(TARGET_MCFW_DIR)/kermod
	cp $(syslink_OUT_DIR)/syslink.ko $(TARGET_MCFW_DIR)/kermod/.
	@echo -e "\n--- make syslink done! (`date +'%H:%M:%S'`)---\n"

syslink_clean:
	$(MAKE) syslinkbuild DEVICE=TI814X TARGET=clean

syslink_all: syslink_clean syslink

##### CMEM MAKE #####

cmem:
	$(MAKE) -j$(CPU_NUM) -C $(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/src/interface \
	LINUXKERNEL_INSTALL_DIR=$(KERNELDIR) MVTOOL_PREFIX=$(BUILD_TOOL_PREFIX)
	$(MAKE) -j$(CPU_NUM) -C $(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/src/module \
	LINUXKERNEL_INSTALL_DIR=$(KERNELDIR) MVTOOL_PREFIX=$(BUILD_TOOL_PREFIX)
	cp $(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/src/module/cmemk.ko \
	$(TARGET_MCFW_DIR)/kermod/.
	@echo -e "\n--- make cmem done! (`date +'%H:%M:%S'`)---\n"

cmem_clean:
	$(MAKE) -C$(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/src/module clean

cmemall: cmem_clean cmem

##### FILESYS MAKE #####

ubifs:
	sudo $(TARGET_ROOT)/mkfs.ubifs -r $(TARGET_FS) -F -o ubifs.img -m 2KiB -e 124KiB -c 512
	sudo $(TARGET_ROOT)/ubinize -o $(TFTP_HOME)/rfs_$(SYSTEM_CFG).ubifs -m 2048 -p 128KiB -s 512 -O 2048 $(TARGET_ROOT)/ubinize.cfg
	sudo rm -rf ubifs.img
	@echo -e "\n--- make ubifs_image done! (`date +'%H:%M:%S'`)---\n"

fsupdate:
	-sudo /bin/mkdir -p $(EXEC_DIR)
	-sudo /bin/mkdir -p $(EXEC_DIR)/bin
	sudo /bin/cp $(APP_DIR)/lib/libfont_db.so $(TARGET_FS)/usr/lib/.
	sudo /bin/cp -Rp $(TARGET_MCFW_DIR)/* $(EXEC_DIR)/.
	sudo /bin/cp -f $(TFTP_HOME)/wis-streamer $(EXEC_DIR)/bin/.
	sudo /bin/cp -f $(TFTP_HOME)/ntpclient $(EXEC_DIR)/bin/.
	sudo /bin/cp -f $(TFTP_HOME)/tz_set $(EXEC_DIR)/bin/.
	sudo /bin/cp -f $(TFTP_HOME)/P2PTunnelServer_ti $(EXEC_DIR)/bin/.
	sudo /bin/cp -f $(TFTP_HOME)/process_check $(EXEC_DIR)/bin/.
	sudo /bin/cp -f $(EXTERNAL_PACKAGE)/network_manager/net_mgr.out $(EXEC_DIR)/bin/.
	sudo /bin/cp -f $(EXTERNAL_PACKAGE)/av_rec/av_rec.out $(EXEC_DIR)/bin/.
	sudo /bin/cp -f $(EXTERNAL_PACKAGE)/gnss/app_gnss.out $(EXEC_DIR)/bin/.
ifeq ($(USE_VOIP),YES)
	sudo /bin/cp $(EXTERNAL_PACKAGE)/voip/re/libre.so $(TARGET_FS)/usr/lib/.
	sudo /bin/cp $(EXTERNAL_PACKAGE)/voip/rem/librem.so $(TARGET_FS)/usr/lib/.
	sudo /bin/cp -f $(EXTERNAL_PACKAGE)/voip/baresip/baresip.out $(EXEC_DIR)/bin/.
	sudo /bin/cp -f $(EXTERNAL_PACKAGE)/voip/baresip/*.so $(TARGET_FS)/usr/baresip/modules/.
endif
ifeq ($(USE_WIFI),YES)
	sudo /bin/cp -f $(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/hostapd/hostapd $(TARGET_FS)/usr/sbin/.
	sudo /bin/cp -f $(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/hostapd/hostapd_cli $(TARGET_FS)/usr/sbin/.
	sudo /bin/cp -f $(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/wpa_supplicant/wpa_cli $(TARGET_FS)/usr/sbin/.
	sudo /bin/cp -f $(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/wpa_supplicant/wpa_passphrase $(TARGET_FS)/usr/sbin/.
	sudo /bin/cp -f $(EXTERNAL_PACKAGE)/wlan_tools/wpa_supplicant_hostapd-0.8_rtw/wpa_supplicant/wpa_supplicant $(TARGET_FS)/usr/sbin/.
endif
	sudo /bin/cp -f $(TFTP_HOME)/onvifserver $(EXEC_DIR)/bin/
	sudo /bin/cp -f $(TFTP_HOME)/onvifconfig.xml $(EXEC_DIR)/bin/
	sudo /bin/cp -f $(TFTP_HOME)/onvifsnapshot.jpg $(EXEC_DIR)/bin/
	sudo /bin/cp -f $(TFTP_HOME)/server.crt $(EXEC_DIR)/
	sudo /bin/cp -f $(TFTP_HOME)/server.key $(EXEC_DIR)/
	sudo /bin/cp -f $(TFTP_HOME)/server.pem $(EXEC_DIR)/
	sudo /bin/chmod -Rf 755 $(EXEC_DIR)/bin
	sudo /bin/chmod 755 $(EXEC_DIR)/*.sh
	sudo /bin/chmod 755 $(EXEC_DIR)/scripts/*.sh

ifeq ($(USE_WIFI),YES)
	sudo /bin/cp $(KMODDIR)/wlan/88x1au_driver/8811au.ko $(TARGET_FS)/lib/modules/.
	sudo /bin/cp $(KMODDIR)/wlan/88x2bu_driver/88x2bu.ko $(TARGET_FS)/lib/modules/.
endif

ifeq ($(USE_KCMVP), YES)
	sudo /bin/cp -f $(THIRD_PARTY)/kcmvp/linux_dll/2.6_armv7l_32_44_post_limit/libMagicCrypto* $(TARGET_FS)/usr/lib/.
endif
	@echo -e "\n--- RFS update for $(RDK_BOARD_TYPE) (`date +'%H:%M:%S'`)---\n"

update:
	@cd $(TFTP_HOME) && ./make_update.sh $(sw)

##### SYSTEM BUILD #####

sys: psp syslink cmem iss hdvpss m3 wlan_tools ubxapp

sys_clean: bin_clean rfs_clean psp_clean syslink_clean cmem_clean hdvpss_clean m3_clean wlan_tools_clean ubxapp_clean
	rm -rf $(MCFW_ROOT_PATH)/build
	@echo -e "\n--- make sysclean done! (`date +'%H:%M:%S'`)---\n"

sys_all: sys_clean rfs_unzip bigfile_unzip sys fsupdate
	@echo "--------------------------------------------------------------"
	@echo "******** Build completed for following configuration *********"
	@echo "--------------------------------------------------------------"
	@echo "Hardware Platform  : $(RDK_BOARD_TYPE)"
	@echo "Build Mode         : $(APP_BUILD_CFG)"
	@echo "Filesystem Mode    : $(FS_CFG)"
	@echo "Memory Config      : $(MEMORY_CONFIG)"
	@echo "--------------------------------------------------------------"
	@echo -e "\n--- make sys_all done! (`date +'%H:%M:%S'`)---\n"

fw: ubxapp ubifs
	@echo "----------------------------------------------------------------"
	@echo "***** Build Firmware (involved make ubxapp && make ubifs) ******"
	@echo "----------------------------------------------------------------"
	@cd $(TFTP_HOME) \
		&& rm -f rfs_fit.ubifs.md5 fitt_firmware_full_N.dat \
		&& touch fw_version.txt \
		&& md5sum rfs_fit.ubifs > rfs_fit.ubifs.md5 \
		&& tar cvf fitt_firmware_full_N.dat \
			boot.scr MLO fw_version.txt rfs_fit.ubifs rfs_fit.ubifs.md5
	@echo -e "\n--- make fw done! (`date +'%H:%M:%S'`)---\n"

