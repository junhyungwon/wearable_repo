ROOTDIR = ../../..
include $(ROOTDIR)/Rules.make

CFLAGS += -Wall -static

ifeq ($(POWER_OPT_DSP_OFF), YES)
CFLAGS += -DDSP_OFF
endif
ifeq ($(POWER_OPT_DSS_OFF), YES)
CFLAGS += -DDSS_OFF
endif

TARGETS = linux_prcm_ipcam
CC = $(BUILD_TOOL_PREFIX)gcc
all:$(TARGETS)
		
%.o:%.c
	$(CC) $(CFLAGS) -g -D__DEBUG -c $^

install:
	cp -f $(TARGETS) $(EXEC_DIR)
	
linux_prcm_ipcam: TI812x_ES_2x_evm_A8_ddr3.o
	$(CC) $(LDFLAGS) -o $@ $^

	
clean:
	rm -f *.o *~
	rm -f $(TARGETS)
