BUILD_DATE=$(shell date +"%y/%m/%d %H:%M")
#CXX=gcc
#CXX=/opt/Sourcery/bin/arm-none-linux-gnueabi-g++
#CXX=/opt/arm-hisiv300-linux/bin/arm-hisiv300-linux-uclibcgnueabi-g++

#ROOTDIR = ../../../../../../
#include $(ROOTDIR)/sdk/sw_mcfw/makerules/common_header_a8.mk

#dvr_rdk_BASE := $(shell pwd)
EXEC_DIR = $(ROOTDIR)/sdk/bin

INSTALL_DIR :=$(APP_INSTALL_DIR)
CC	:= $(BUILD_TOOL_PREFIX)gcc
STRIP 	:= $(BUILD_TOOL_PREFIX)strip

ifeq ("$(platform)","ti")
CROSS_COMPILE := $(ROOTDIR)/ti_tools/linux_devkit/bin/arm-arago-linux-gnueabi-
endif

PPPLIB = '../../sdk/Linux/$(platform)'/libPPPP_API.a

ifeq ("$(platform)","")
        platform=x86
endif

ifeq ("$(platform)","x86")
        CXX=gcc
	CFLAGS += -m32
endif

ifeq ("$(platform)","hi300")
        CXX=/opt/arm-hisiv300-linux/bin/arm-hisiv300-linux-uclibcgnueabi-g++
endif

ifeq ("$(platform)","hi200")
        CXX=/opt/arm-hisiv200-linux/bin/arm-hisiv200-linux-gnueabi-g++
endif

ifeq ("$(platform)","hi400")
        CXX=/opt/arm-hisiv400-linux/bin/arm-hisiv400-linux-gnueabi-g++
endif

ifeq ("$(platform)","hi100")
        CXX=/opt/arm-hisiv100-linux/bin/arm-hisiv100-linux-uclibcgnueabi-g++
endif

ifeq ("$(platform)","ti")
#       CXX=/opt/ti-tools/linux_devkit/bin/arm-arago-linux-gnueabi-gcc
#        CXX=$(CROSS_COMPILE)gcc
        CXX=$(BUILD_TOOL_PREFIX)gcc
endif

all:
	@echo === Target Platform [$(platform)] ===
	$(CXX) -s $(CFLAGS) -O2 -Wall P2PTunnelServer.c -I./ -I../include/PPPP -I../../Includes -I../../include $(PPPLIB) -lpthread -lrt -o P2PTunnelServer
	$(CXX) -s $(CFLAGS) -O2 -Wall P2PTunnelAgent.c -I./ -I../include/PPPP -I../../Includes -I../../include $(PPPLIB) -lpthread -lrt -o P2PTunnelAgent
#	$(CXX) -s $(CFLAGS) -O2 -Wall P2PTunnelServer.c -I./ -I../include/PPPP -I../../Includes -I../../include $(PPPLIB) -lpthread -lrt -o P2PTunnelServer_$(platform)
#	$(CXX) -s $(CFLAGS) -O2 -Wall P2PTunnelAgent.c -I./ -I../include/PPPP -I../../Includes -I../../include $(PPPLIB) -lpthread -lrt -o P2PTunnelAgent_$(platform)


clean:
	rm -rf *.o *~ *.bak P2PTunnelServer P2PTunnelServer_* P2PTunnelAgent P2PTunnelAgent_*

install:
	cp P2PTunnelServer_$(platform) $(EXEC_DIR)
