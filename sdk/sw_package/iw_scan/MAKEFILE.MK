# Makefile for iw_scan

INSTALL_DIR :=$(APP_INSTALL_DIR)
CC = $(BUILD_TOOL_PREFIX)gcc

INCLUDE = -I .
C_FLAGS += -Wall -Warray-bounds -O3 -pthread $(INCLUDE)

SOURCES  = $(wildcard *.c)
OBJFILES = $(SOURCES:%.c=%.o)

TARGET = iw_scan.out
TARGET_HEADER=ipc_common_def.h

all: $(TARGET) install

$(TARGET): $(OBJFILES)
	$(CC) $(C_FLAGS) -o $@ $(OBJFILES) $(LIBS)

%.o:%.c
	$(CC) $(C_FLAGS) -c $^
		
clean:
	rm -rf $(OBJFILES) $(TARGET) core
	rm -f $(TARGET_MCFW_DIR)/bin/$(TARGET)

install :
	@echo "# -------- Header file Install -------\n"
#	rm -Rf $(INSTALL_DIR)/$(TARGET_HEADER)
#	mv $(TARGET) $(TARGET_MCFW_DIR)/bin/.
		