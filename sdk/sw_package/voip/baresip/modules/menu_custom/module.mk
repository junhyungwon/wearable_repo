#
# module.mk
#
# Copyright (C) 2010 Creytiv.com
#

MOD		:= menu_custom
$(MOD)_SRCS	+= menu_custom.c msg.c
$(MOD)_CFLAGS += -D_GNU_SOURCE -I$(KERNELDIR)/usr/include -I$(APP_DIR)/lib/include

include mk/mod.mk
