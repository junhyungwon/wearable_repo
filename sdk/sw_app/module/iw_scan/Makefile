# Makefile for iw_scan

INSTALL_DIR :=$(APP_INSTALL_DIR)
CC = $(CROSS_COMPILE)gcc

INCLUDE = -I .
C_FLAGS += -Wall -Warray-bounds -O3 -lpthread $(INCLUDE)

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

install :
	@echo "# -------- Header file Install -------\n"
#	rm -Rf $(INSTALL_DIR)/$(TARGET_HEADER)
#	cp $(TARGET_HEADER) $(INSTALL_DIR)/.
		