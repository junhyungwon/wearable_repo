ROOTDIR = ../../../../
#include $(ROOTDIR)/Rules.make
include $(ROOTDIR)/sw_mcfw/makerules/common_header_a8.mk

CODEGEN_PATH_A8  := ../../../../../ti_tools
CODEGEN_PREFIX   := $(CODEGEN_PATH_A8)/linux_devkit/bin/arm-arago-linux-gnueabi-

INCLUDES = -I . \
        -I../netra_interface \
        $(CMEM_INC)

C_FLAGS += -Wall -g -D$(PRODUCT_NAME)
AR_FLAGS += -rcus

CC = $(CODEGEN_PREFIX)gcc $(INCLUDES) -c
AR = $(CODEGEN_PREFIX)ar

REL_LIB1 = Netra_interface.a

REL_OBJ1 = NetraDrvMsg.o Netra_interface.o sem_util.o msg_util.o

all:  $(REL_LIB1)

$(REL_LIB1): $(REL_OBJ1)
	$(AR) $(AR_FLAGS) $(REL_LIB1) $(REL_OBJ1)

msg_util.o: msg_util.c Msg_Def.h
	$(CC) $(C_FLAGS) -o $@ $<

NetraDrvMsg.o: NetraDrvMsg.c NetraDrvMsg.h
	$(CC) $(C_FLAGS) -o $@ $<

Netra_interface.o: Netra_interface.c Netra_interface.h
	$(CC) $(C_FLAGS) -o $@ $<

sem_util.o: sem_util.c sem_util.h
	$(CC) $(C_FLAGS) -o $@ $<

clean:
	-$(RM) -f *.o
	-$(RM) -f *.a
