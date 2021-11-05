INCLUDES = -Iinclude -I../UsageEnvironment/include -I../groupsock/include -I../liveMedia/include
PREFIX = /usr/local
LIBDIR = $(PREFIX)/lib
##### Change the following for your environment:
CODEGEN_PATH_A8     = ../../../../ti_tools
BUILD_TOOL_PREFIX   = $(CODEGEN_PATH_A8)/linux_devkit/bin/arm-arago-linux-gnueabi-
CROSS_COMPILE?=		$(BUILD_TOOL_PREFIX)
COMPILE_OPTS =		$(INCLUDES) -I. -O2 -DSOCKLEN_T=socklen_t -DNO_SSTREAM=1 -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64
C =			c
C_COMPILER =		$(CROSS_COMPILE)gcc
C_FLAGS =		$(COMPILE_OPTS)
CPP =			cpp
CPLUSPLUS_COMPILER =	$(CROSS_COMPILE)g++
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall -DBSD=1 -std=c++0x -lpthread
OBJ =			o
LINK =			$(CROSS_COMPILE)g++ -o  
LINK_OPTS =	
CONSOLE_LINK_OPTS =	$(LINK_OPTS) -lpthread
LIBRARY_LINK =		$(CROSS_COMPILE)ar cr 
LIBRARY_LINK_OPTS =	$(LINK_OPTS) 
LIB_SUFFIX =			a
LIBS_FOR_CONSOLE_APPLICATION = -lssl -lcrypto
LIBS_FOR_GUI_APPLICATION =
EXE =
##### End of variables to change

NAME = libbackChannel
ALL = $(NAME).$(LIB_SUFFIX)
all:	$(ALL)

.$(C).$(OBJ):
	$(C_COMPILER) -c $(C_FLAGS) $<
.$(CPP).$(OBJ):
	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $<

BACKCHANNEL_LIB_OBJS = BCAWaveSink.$(OBJ) \
						AudioUtility.$(OBJ) \
						../liveMedia/*.$(OBJ) \
						WaveBackChannelAudioFileServerMediaSubsession.$(OBJ) \
						WaveBackChannelAudioServerMediaSubsession.$(OBJ)

BCAWaveSink.$(CPP):	include/BCAWaveSink.hh
WaveBackChannelAudioServerMediaSubsession.$(CPP):	include/WaveBackChannelAudioServerMediaSubsession.hh 
AudioUtility.$(C):		include/AudioUtility.h

libbackChannel.$(LIB_SUFFIX): $(BACKCHANNEL_LIB_OBJS) \
    $(PLATFORM_SPECIFIC_LIB_OBJS)
	$(LIBRARY_LINK)$@ $(LIBRARY_LINK_OPTS) \
		$(BACKCHANNEL_LIB_OBJS)

clean:
	-rm -rf *.$(OBJ) $(ALL) core *.core *~ include/*~

install: install1 $(INSTALL2)
install1: libbackChannel.$(LIB_SUFFIX)
	  install -d $(DESTDIR)$(PREFIX)/include/backChannel $(DESTDIR)$(LIBDIR)
	  install -m 644 include/*.hh include/*.h $(DESTDIR)$(PREFIX)/include/backChannel
	  install -m 644 libbackChannel.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)
install_shared_libraries: libbackChannel.$(LIB_SUFFIX)
	  ln -s libbackChannel.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/libbackChannel.$(SHORT_LIB_SUFFIX)
	  ln -s libbackChannel.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/libbackChannel.so

##### Any additional, platform-specific rules come here:
