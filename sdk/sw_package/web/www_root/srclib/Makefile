################################################################################
TOOL_PREFIX   = $(BUILD_TOOL_PREFIX)
CROSS_COMPILE = $(TOOL_PREFIX)
TARGET_PATH   = $(TARGET_FS)
UID=$(shell id -u)
WID=www-data
################################################################################

INC += -I. -I../include
COMPILE_OPTS =		-g -c
COMPILE_OPTS +=		$(INC) -O2 -DSOCKLEN_T=socklen_t  -D$(SYSTEM_PLATFORM)
#COMPILE_OPTS +=		$(INC) -O2 -DSOCKLEN_T=socklen_t -DNO_SSTREAM=1 -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64
C =			c
CC =			$(CROSS_COMPILE)gcc
CPP =			$(CROSS_COMPILE)cpp
GPP =			$(CROSS_COMPILE)g++
CFLAGS =		$(COMPILE_OPTS)
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall -DBSD=1 -std=c++0x
LINK =			$(CROSS_COMPILE)g++
LINK_OPTS =
CONSOLE_LINK_OPTS =	$(LINK_OPTS)
#LIBRARY_LINK =		$(CROSS_COMPILE)ar rscv
LIBRARY_LINK =		$(CROSS_COMPILE)ar rc
LIBRARY_LINK_OPTS =	$(LINK_OPTS)
LIB_SUFFIX =			a
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =

LDFLAGS += -lstdc++
##### End of variables to change

##### Files
.SUFFIXES: .o .c .cpp
HEADERS = $(wildcard *.h)
SRCCPP = $(wildcard *.cpp)
OBJCPP = $(SRCCPP:%.cpp=%.o)
SRCC = $(wildcard *.c)
OBJC = $(SRCC:%.c=%.o)

TARGET = libLFCGI.so

all: clean $(TARGET)

$(OBJCPP):%.o: %.cpp $(HEADERS)
	@echo Compiling CPP $@ from $<...
#	$(GPP) -std=c99 -fPIC $(CFLAGS) $(INCPATH) -o $@ $<
	$(GPP) $(CPLUSPLUS_FLAGS) $(INCPATH) $<

$(OBJC):%.o: %.c $(HEADERS)
	@echo Compiling C $@ from $<...
#	$(CC) -std=c99 -fPIC $(CFLAGS) $(INCPATH) -o $@ $<
	$(CC) $(CFLAGS) $(INCPATH) $<

$(TARGET): $(OBJC) $(OBJCPP)
	@echo Linking $@ from ...
#	$(LIBRARY_LINK) $(LDFLAGS) $(LIBS) $@ $^
	$(CC) -shared -o $(TARGET) -W1,-soname,libLFCGI.so $(LDFLAGS) $(LIBS) $^

clean:
	$(RM) *.o
	$(RM) $(TARGET)

install: all
	sudo cp $(TARGET) $(TARGET_PATH)/usr/lib/


