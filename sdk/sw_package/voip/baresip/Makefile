#
# Makefile
#
# Copyright (C) 2010 Alfred E. Heggestad
#
#
# Internal features:
#
#   USE_TLS           Enable SIP over TLS transport
#

PROJECT	  := baresip
VERSION   := 1.1.0
DESCR     := "Baresip is a modular SIP User-Agent with audio and video support"
# added by LF_Rupy
BIN		  := baresip.out
# Verbose and silent build modes
ifeq ($(V),)
HIDE=@
endif

ifndef LIBRE_MK
LIBRE_MK  := $(shell [ -f ../re/mk/re.mk ] && \
	echo "../re/mk/re.mk")
ifeq ($(LIBRE_MK),)
LIBRE_MK  := $(shell [ -f ../re-$(VERSION)/mk/re.mk ] && \
	echo "../re-$(VERSION)/mk/re.mk")
endif
ifeq ($(LIBRE_MK),)
LIBRE_MK  := $(shell [ -f /usr/share/re/re.mk ] && \
	echo "/usr/share/re/re.mk")
endif
ifeq ($(LIBRE_MK),)
LIBRE_MK  := $(shell [ -f /usr/local/share/re/re.mk ] && \
	echo "/usr/local/share/re/re.mk")
endif
endif

include $(LIBRE_MK)
include mk/modules.mk

ifeq ($(SYSROOT_LOCAL),)
ifeq  ($(SYSROOT),/usr)
SYSROOT_LOCAL := $(shell [ -d /usr/local/include ] && echo "/usr/local")
endif
endif

ifndef LIBREM_PATH
LIBREM_PATH	:= $(shell [ -d ../rem ] && echo "../rem")
endif

ifeq ($(LIBREM_PATH),)
ifneq ($(SYSROOT_LOCAL),)
LIBREM_PATH	:= $(shell [ -f $(SYSROOT_LOCAL)/include/rem/rem.h ] && \
	echo "$(SYSROOT_LOCAL)")
endif
endif

ifeq ($(LIBREM_PATH),)
LIBREM_PATH	:= $(shell [ -f $(SYSROOT)/include/rem/rem.h ] && \
	echo "$(SYSROOT)")
endif

# Include path
LIBREM_INC := $(shell [ -f $(LIBREM_PATH)/include/rem.h ] && \
	echo "$(LIBREM_PATH)/include")
ifeq ($(LIBREM_INC),)
LIBREM_INC := $(shell [ -f $(LIBREM_PATH)/include/rem/rem.h ] && \
	echo "$(LIBREM_PATH)/include/rem")
endif
ifeq ($(LIBREM_INC),)
LIBREM_INC := $(shell [ -f /usr/local/include/rem/rem.h ] && \
	echo "/usr/local/include/rem")
endif
ifeq ($(LIBREM_INC),)
LIBREM_INC := $(shell [ -f /usr/include/rem/rem.h ] && echo "/usr/include/rem")
endif

# Library path
ifeq ($(LIBREM_SO),)
LIBREM_SO  := $(shell [ -f $(LIBREM_PATH)/librem.a ] && \
	echo "$(LIBREM_PATH)")
endif
ifeq ($(LIBREM_SO),)
LIBREM_SO :=$(shell [ -f $(LIBREM_PATH)/librem$(LIB_SUFFIX) ] && \
	echo "$(LIBREM_PATH)")
endif
ifeq ($(LIBREM_SO),)
LIBREM_SO := $(shell [ -f $(LIBREM_PATH)/lib/librem$(LIB_SUFFIX) ] && \
	echo "$(LIBREM_PATH)/lib")
endif
ifeq ($(LIBREM_SO),)
LIBREM_SO  := $(shell [ -f /usr/local/lib/librem$(LIB_SUFFIX) ] \
	&& echo "/usr/local/lib")
endif
ifeq ($(LIBREM_SO),)
LIBREM_SO  := $(shell [ -f /usr/lib/librem$(LIB_SUFFIX) ] && \
	echo "/usr/lib")
endif
ifeq ($(LIBREM_SO),)
LIBREM_SO  := $(shell [ -f /usr/lib64/librem$(LIB_SUFFIX) ] && \
	echo "/usr/lib64")
endif

CFLAGS    += -I. -Iinclude -I$(LIBRE_INC)
CFLAGS    += -I$(LIBREM_INC)

CXXFLAGS  += -I. -Iinclude -I$(LIBRE_INC)
CXXFLAGS  += -I$(LIBREM_INC)
CXXFLAGS  += $(EXTRA_CXXFLAGS)

# XXX: common for C/C++
CPPFLAGS += -DHAVE_INTTYPES_H

CLANG_OPTIONS  += -I$(LIBREM_INC)

ifeq ($(OS),win32)
STATIC    := yes
endif

ifneq ($(SYSROOT),)
ifeq ($(OS),freebsd)
CFLAGS += -I$(SYSROOT)/local/include
endif
ifeq ($(OS),openbsd)
CFLAGS += -isystem $(SYSROOT)/local/include
endif
endif

# Optional dependencies
ifneq ($(STATIC),)
CFLAGS    += -DSTATIC=1
CXXFLAGS  += -DSTATIC=1
endif

BINDIR	:= $(PREFIX)/bin
INCDIR  := $(PREFIX)/include
SHARED  := lib$(PROJECT)$(LIB_SUFFIX)
STATICLIB  := libbaresip.a
ifeq ($(STATIC),)
MOD_BINS:= $(patsubst %,%$(MOD_SUFFIX),$(MODULES))
endif
APP_MK	:= src/srcs.mk
MOD_MK	:= $(patsubst %,modules/%/module.mk,$(MODULES))
MOD_BLD	:= $(patsubst %,$(BUILD)/modules/%,$(MODULES))
LIBDIR     := $(PREFIX)/lib
MOD_PATH   := $(LIBDIR)/$(PROJECT)/modules
# comment out by LF_Rupy...(cfg 파일 위치등과 연관됨.)
#SHARE_PATH := $(PREFIX)/share/$(PROJECT)
SHARE_PATH := /usr/baresip
CFLAGS     += -DPREFIX=\"$(PREFIX)\"
CFLAGS    += -DMOD_PATH=\"$(MOD_PATH)\"
CFLAGS    += -DSHARE_PATH=\"$(SHARE_PATH)\"

all: $(MOD_BINS) $(BIN)

.PHONY: modules
modules:	$(MOD_BINS)

include $(APP_MK)
include $(MOD_MK)

OBJS      := $(patsubst %.c,$(BUILD)/src/%.o,$(filter %.c,$(SRCS)))
OBJS      += $(patsubst %.m,$(BUILD)/src/%.o,$(filter %.m,$(SRCS)))
OBJS      += $(patsubst %.S,$(BUILD)/src/%.o,$(filter %.S,$(SRCS)))

APP_OBJS  := $(OBJS) $(patsubst %.c,$(BUILD)/src/%.o,$(APP_SRCS)) $(MOD_OBJS)

LIB_OBJS  := $(OBJS) $(MOD_OBJS)

TEST_OBJS := $(patsubst %.c,$(BUILD)/test/%.o,$(filter %.c,$(TEST_SRCS)))
TEST_OBJS += $(patsubst %.cpp,$(BUILD)/test/%.o,$(filter %.cpp,$(TEST_SRCS)))

LIBS	+= -L$(LIBREM_SO)

# Static build: include module linker-flags in binary
ifneq ($(STATIC),)
LIBS      += $(MOD_LFLAGS)
else

ifneq ($(SYSROOT_LOCAL),)
LIBS      += -L$(SYSROOT_LOCAL)/lib
MOD_LFLAGS += -L$(SYSROOT_LOCAL)/lib
endif

endif

LIBS      += -lrem -lm
#LIBS      += -L$(SYSROOT)/lib

ifeq ($(OS),win32)
TEST_LIBS += -static-libgcc
endif

-include $(APP_OBJS:.o=.d)

Makefile:	mk/*.mk $(MOD_MK) $(LIBRE_MK)

$(SHARED): $(LIB_OBJS)
	@echo "  LD      $@"
	$(HIDE)$(LD) $(LFLAGS) $(SH_LFLAGS) $^ -L$(LIBRE_SO) -lre $(LIBS) -o $@

$(STATICLIB): $(LIB_OBJS)
	@echo "  AR      $@"
	@rm -f $@; $(AR) $(AFLAGS) $@ $^
ifneq ($(RANLIB),)
	@echo "  RANLIB  $@"
	$(HIDE)$(RANLIB) $@
endif

$(BIN):	$(APP_OBJS)
	@echo "  LD      $@"
	$(HIDE)$(LD) $(LFLAGS) $(APP_LFLAGS) $^ \
		-L$(LIBRE_SO) -lre $(LIBS) -o $@

#
# List of modules used by selftest
#
ifneq ($(STATIC),)
TEST_MODULES :=
else
TEST_MODULES := g711.so
endif

$(BUILD)/%.o: %.c $(BUILD) Makefile $(APP_MK)
	@echo "  CC      $@"
	$(HIDE)$(CC) $(CFLAGS) -c $< -o $@ $(DFLAGS)

$(BUILD)/%.o: %.cpp $(BUILD) Makefile $(APP_MK)
	@echo "  CXX     $@"
	$(HIDE)$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ $(DFLAGS)

$(BUILD)/%.o: %.m $(BUILD) Makefile $(APP_MK)
	@echo "  OC      $@"
	$(HIDE)$(CC) $(CFLAGS) $(OBJCFLAGS) -c $< -o $@ $(DFLAGS)

$(BUILD)/%.o: %.S $(BUILD) Makefile $(APP_MK)
	@echo "  AS      $@"
	$(HIDE)$(CC) $(CFLAGS) -c $< -o $@ $(DFLAGS)

$(BUILD): Makefile
	@mkdir -p $(BUILD)/src $(MOD_BLD) $(BUILD)/test/mock $(BUILD)/test/sip
	@touch $@

install: $(BIN) $(MOD_BINS)
	@mkdir -p $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(BIN) $(DESTDIR)$(BINDIR)
ifeq ($(STATIC),)
	@mkdir -p $(DESTDIR)$(MOD_PATH)
	$(INSTALL) -m 0644 $(MOD_BINS) $(DESTDIR)$(MOD_PATH)
endif
	@mkdir -p $(DESTDIR)$(SHARE_PATH)
	$(INSTALL) -m 0644 share/* $(DESTDIR)$(SHARE_PATH)

.PHONY: clean
clean:
	@rm -rf $(BIN) $(MOD_BINS) $(SHARED) $(BUILD) \
		$(STATICLIB)
	@rm -f *stamp \
	`find . -name "*.[od]"` \
	`find . -name "*~"` \
	`find . -name "\.\#*"`

bareinfo: info
	@echo "  LIBREM_PATH:   $(LIBREM_PATH)"
	@echo "  LIBREM_INC:    $(LIBREM_INC)"
	@echo "  LIBREM_SO:     $(LIBREM_SO)"
