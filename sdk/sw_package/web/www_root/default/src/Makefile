################################################################################
TOOL_PREFIX   = $(BUILD_TOOL_PREFIX)
CROSS_COMPILE = $(TOOL_PREFIX)
WWW_DEV_DIR   = $(EXTERNAL_PACKAGE)/web
TARGET_PATH   = $(TARGET_FS)
UID=$(shell id -u)
WID=www-data
################################################################################

INC += -I. -I../../include -I../../srclib

COMPILE_OPTS =		-g -c
COMPILE_OPTS +=		$(INC) -O2 -DSOCKLEN_T=socklen_t -DNO_SSTREAM=1 -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -D$(SYSTEM_PLATFORM)
C =			c
CC =			$(CROSS_COMPILE)gcc
CPP =			$(CROSS_COMPILE)cpp
GPP =			$(CROSS_COMPILE)g++
GPPFLAGS =		$(COMPILE_OPTS)
CFLAGS =		$(COMPILE_OPTS)
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall
LINK =			$(CROSS_COMPILE)g++
LINK_OPTS =	
CONSOLE_LINK_OPTS =	$(LINK_OPTS)
LIBRARY_LINK =		$(CROSS_COMPILE)ar cr 
LIBRARY_LINK_OPTS =	$(LINK_OPTS)
LIB_SUFFIX =			a
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
EXE =
LIBS += -L../../srclib/ -lLFCGI -ljson-c
LIBS += ../../srclib/cgi.o
##### End of variables to change

# dev default/cgi-bin directory 생성
DEFAULT_CGI_BIN_DIR = $(WWW_DEV_DIR)/www_root/default/cgi-bin
HAVE_DEFAULT_CGI_BIN_DIR = $(shell if [ -e $(DEFAULT_CGI_BIN_DIR) ]; then echo 1; else echo 0; fi;)
ifeq ($(HAVE_DEFAULT_CGI_BIN_DIR),0)
$(shell mkdir -p $(DEFAULT_CGI_BIN_DIR))
else
#$(info *********** INFO $(DEFAULT_CGI_BIN_DIR) exist.)
endif

##### Files
.SUFFIXES: .o .c .cpp
HEADERS = $(wildcard *.h)
SRCGPP = $(wildcard *.cpp)
OBJGPP = $(SRCGPP:%.cpp=%.o)
SRCC = $(wildcard *.c)
OBJC = $(SRCC:%.c=%.o)


.cpp.o:
	$(GPP) -c $(GPPFLAGS) $(INC) -o "$@" "$<"
.c.o:
	$(CC) -c $(CFLAGS) $(INC) -o "$@" "$<"

all: clean $(OBJC) $(OBJGPP)
	cp *.cgi $(DEFAULT_CGI_BIN_DIR)/

$(OBJC):%.o: %.c $(HEADERS)
	@echo Linking $@ from $^
	$(CC) -o $*.cgi $(CFLAGS) $(INC) $(LDFLAGS) $(LIBS) $^ 
#	$(CC) -o $@ $(INC) $(LDFLAGS) $(LIBS) $<

$(OBJGPP):%.o: %.cpp $(HEADERS)
	@echo Compiling $@ from $<
	$(GPP) -std=c++0x -Wall -c $(GPPFLAGS) $(INC) -o $@ $<
	@echo Linking $@ from ...
	$(GPP) -o $*.cgi $(LDFLAGS) $(LIBS) $@

clean:
	$(RM) ./*.o
	$(RM) ./*.cgi

# 상위 Makefile에서 실시함.
install: all

	
.PHONY : all clean
