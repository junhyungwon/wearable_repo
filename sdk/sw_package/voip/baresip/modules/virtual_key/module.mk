#
# module.mk
#
# Copyright (C) 2010 Creytiv.com
#

MOD		:= virtual_key
$(MOD)_SRCS	+= virtual_key.c msg.c
$(MOD)_CFLAGS += -D_GNU_SOURCE -I$(KERNELDIR)/usr/include -I$(APP_DIR)/lib/include

include mk/mod.mk
