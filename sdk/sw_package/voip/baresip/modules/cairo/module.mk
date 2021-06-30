#
# module.mk
#
# Copyright (C) 2010 Alfred E. Heggestad
#

MOD		:= cairo
$(MOD)_SRCS	+= cairo.c
$(MOD)_LFLAGS	+= $(shell pkg-config --libs cairo)
$(MOD)_CFLAGS	+= $(shell pkg-config --cflags cairo)

include mk/mod.mk
