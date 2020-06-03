INCLUDES = -I . -I src/
CC := $(BUILD_TOOL_PREFIX)gcc
CPLUSPLUS = $(BUILD_TOOL_PREFIX)g++
STRIP := $(BUILD_TOOL_PREFIX)strip

AR_FLAGS += -r
AR = $(BUILD_TOOL_PREFIX)ar

TARGET_DIR = .
LIB_PATH := ./lib
LDFLAGS := -L$(LIB_PATH) $(LDFLAGS)
CFLAGS = $(INCLUDES) -D_LINUX -g -Wall

LIBS = libwriter.a

REL_LIB = libmulti_avi.a
REL_OBJS = udavi.o

all: $(REL_LIB)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.cpp.o:
	$(CPLUSPLUS) -c $(CFLAGS) $< -o $@

udavi.cpp:	udavi.h

$(REL_LIB): $(REL_OBJS) $(LIBS)
	$(AR) $(AR_FLAGS) $(REL_LIB) $(LIBS) $(REL_OBJS)
	mv $(REL_LIB) ../../lib/
	rm -f $(TARGET_DIR)/$(REL_OBJS)

clean:
	rm -f $(TARGET_DIR)/$(REL_LIB)
	rm -f $(TARGET_DIR)/$(REL_OBJS)
