# Makefile for av_rec
CC = $(BUILD_TOOL_PREFIX)gcc
LD = $(BUILD_TOOL_PREFIX)ld
STRIP := $(BUILD_TOOL_PREFIX)strip
TARGET = av_rec.out

INCLUDE = -I. -I$(APP_INC_DIR)/inc -I$(CMEM_INC_DIR) -I$(APP_DIR)/lib/include
C_FLAGS += -Wall -Warray-bounds -O3 $(INCLUDE)
LDFLAGS += -lpthread -lstdc++
LIBS = $(CMEM_LIB_DIR)/cmem.a470MV $(APP_DIR)/lib/libmulti_avi.a

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

#install :
#	@echo "# -------- Header file Install -------\n"
#	rm -Rf $(INSTALL_DIR)/$(TARGET_HEADER)
#	cp $(TARGET_HEADER) $(INSTALL_DIR)/.
