################################################################################
TOOL_PREFIX   = $(BUILD_TOOL_PREFIX)
CROSS_COMPILE = $(TOOL_PREFIX)
TARGET_PATH   = $(TARGET_FS)
################################################################################

INC += -I. -I../include
COMPILE_OPTS =		-g -c
COMPILE_OPTS +=		$(INC) -O2 -DSOCKLEN_T=socklen_t  -D$(SYSTEM_PLATFORM)
C =			c
CC =			$(CROSS_COMPILE)gcc
CPP =			$(CROSS_COMPILE)cpp
GPP =			$(CROSS_COMPILE)g++
CFLAGS =		$(COMPILE_OPTS)
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall -DBSD=1 -std=c++0x

LDFLAGS += -lstdc++ -lcrypto
##### End of variables to change

##### Files
.SUFFIXES: .o .c .cpp
HEADERS = $(wildcard *.h)
SRCCPP = $(wildcard *.cpp)
OBJCPP = $(SRCCPP:%.cpp=%.o)
SRCC = $(wildcard *.c)
OBJC = $(SRCC:%.c=%.o)

TARGET = libLF.so

all: clean $(TARGET) install

$(OBJCPP):%.o: %.cpp $(HEADERS)
	@echo Compiling CPP $@ from $<...
	$(GPP) $(CPLUSPLUS_FLAGS) $(INCPATH) $<

$(OBJC):%.o: %.c $(HEADERS)
	@echo Compiling C $@ from $<...
	$(CC) $(CFLAGS) $(INCPATH) $<

$(TARGET): $(OBJC) $(OBJCPP)
	@echo Linking $@ from ...
	$(CC) -shared -o $(TARGET) -W1,-soname,libLF.so $(LDFLAGS) $(LIBS) $^

clean:
	$(RM) *.o
	$(RM) $(TARGET)

install: all
	sudo cp $(TARGET) $(TARGET_PATH)/usr/lib/
