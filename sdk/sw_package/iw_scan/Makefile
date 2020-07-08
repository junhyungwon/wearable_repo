# Makefile for iw_scan
INSTALL_DIR :=$(APP_INSTALL_DIR)
CC 		:= $(BUILD_TOOL_PREFIX)gcc
STRIP 	:= $(BUILD_TOOL_PREFIX)strip
TARGET = iw_scan.out
#TARGET_HEADER=ipc_common_def.h

INCLUDE = -I .
C_FLAGS += -Wall -Warray-bounds -O3 $(INCLUDE)
LDFLAGS += -lpthread -lzmq

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

install :
	@echo "# -------- Header file Install -------\n"
#	rm -Rf $(INSTALL_DIR)/$(TARGET_HEADER)
#	cp $(TARGET_HEADER) $(INSTALL_DIR)/.
