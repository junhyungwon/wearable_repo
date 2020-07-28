# Makefile for av_rec
CC = $(BUILD_TOOL_PREFIX)gcc
LD = $(BUILD_TOOL_PREFIX)ld
STRIP := $(BUILD_TOOL_PREFIX)strip
TARGET = gpsd.out

INCLUDE = -I. -I$(APP_INC_DIR)/inc -I$(APP_DIR)/lib/include
C_FLAGS += -Wall -Warray-bounds -std=gnu99 -O3 $(INCLUDE)
LDFLAGS += -lpthread -lstdc++ -lrt

SOURCES  = $(wildcard *.c)
OBJFILES = $(SOURCES:%.c=%.o)

#all: $(TARGET) install
all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(C_FLAGS) $(LDFLAGS) -o $@ $(OBJFILES) $(LIBS)
	$(STRIP) $@

%.o:%.c
	$(CC) $(C_FLAGS) $(LDFLAGS) -c $^

clean:
	rm -rf $(OBJFILES) $(TARGET) core

