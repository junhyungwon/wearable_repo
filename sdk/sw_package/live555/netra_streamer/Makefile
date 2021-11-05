# Make sure your own directory
ROOTDIR = ../../..

#include $(ROOTDIR)/Rules.make
include $(ROOTDIR)/sw_mcfw/makerules/common_header_a8.mk

dvr_rdk_BASE := $(shell pwd)
EXEC_DIR = ../../

fc_PREFIX = $(fc_PATH)

CODEGEN_PATH_A8  := ../../../../ti_tools
CODEGEN_PREFIX   := $(CODEGEN_PATH_A8)/linux_devkit/bin/arm-arago-linux-gnueabi-

LIVE_DIR = ..
#linuxutils_PATH = $(EXTERNAL_SW_ROOT)/linuxutils_3_21_00_02
EXEC = wis-streamer

all:	$(EXEC)

install:	$(EXEC)
	install $(EXEC) $(EXEC_DIR)

CC = $(CODEGEN_PREFIX)gcc
CPLUSPLUS = $(CODEGEN_PREFIX)g++

INCLUDES = -I . \
	-I$(LIVE_DIR)/BasicUsageEnvironment/include \
	-I$(LIVE_DIR)/UsageEnvironment/include \
	-I$(LIVE_DIR)/groupsock/include \
	-I$(LIVE_DIR)/liveMedia/include \
	-I$(LIVE_DIR)/BackChannel/include \
	-I$(LIVE_DIR)/netra_streamer/netra_interface \
        $(CMEM_INC)

CFLAGS = $(INCLUDES) -D_LINUX -g -Wall
LDFLAGS += -s

LIBS = $(CMEM_LIB)
LIBS += -L$(LIVE_DIR)/BasicUsageEnvironment -lBasicUsageEnvironment \
	-L$(LIVE_DIR)/UsageEnvironment -lUsageEnvironment \
	-L$(LIVE_DIR)/groupsock -lgroupsock \
	-L$(LIVE_DIR)/BackChannel -lbackChannel -lpthread\
	-L$(LIVE_DIR)/liveMedia -lliveMedia \
	$(LIVE_DIR)/BasicUsageEnvironment/libBasicUsageEnvironment.a \
	$(LIVE_DIR)/UsageEnvironment/libUsageEnvironment.a \
	$(LIVE_DIR)/groupsock/libgroupsock.a \
	$(LIVE_DIR)/BackChannel/libbackChannel.a \
	$(LIVE_DIR)/liveMedia/libliveMedia.a

OBJS = wis-streamer.o Err.o NETRAInput.o \
	WISServerMediaSubsession.o \
	WISJPEGStreamSource.o \
	WISJPEGVideoServerMediaSubsession.o \
	WISH264VideoServerMediaSubsession.o \
	WISPCMAudioServerMediaSubsession.o \
	WW_H264VideoSource.o WW_H264VideoServerMediaSubsession.o \
	$(LIVE_DIR)/netra_streamer/netra_interface/Netra_interface.a

$(EXEC): $(OBJS)
	$(CPLUSPLUS) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $(EXEC)
	cp -rf $(EXEC) $(EXEC_DIR)
	cp -rf $(EXEC) $(ROOTDIR)/bin

wis-streamer.cpp:			Err.hh

Err.cpp:				Err.hh

NETRAInput.cpp:				NETRAInput.hh Err.hh

WISServerMediaSubsession.cpp:		WISServerMediaSubsession.hh

WISServerMediaSubsession.hh:		NETRAInput.hh
WISMPEG4VideoServerMediaSubsession.hh:	WISServerMediaSubsession.hh

WISMPEG4VideoServerMediaSubsession.cpp:	WISMPEG4VideoServerMediaSubsession.hh

WISH264VideoServerMediaSubsession.hh:	WISServerMediaSubsession.hh

WISH264VideoServerMediaSubsession.cpp:	WISH264VideoServerMediaSubsession.hh

WISJPEGStreamSource.cpp:		WISJPEGStreamSource.hh
WISPCMAudioServerMediaSubsession.cpp:	WISPCMAudioServerMediaSubsession.hh


.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.cpp.o:
	$(CPLUSPLUS) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~
	rm -f $(EXEC)
