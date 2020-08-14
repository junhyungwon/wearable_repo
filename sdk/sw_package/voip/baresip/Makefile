#
# Makefile
#
# Copyright (C) 2010 Creytiv.com
#
#
# Internal features:
#
#   USE_TLS           Enable SIP over TLS transport
#

VERSION   := 0.6.6
DESCR     := "Baresip is a modular SIP User-Agent with audio and video support"
BIN	:= baresip.out

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

ifeq ($(SYSROOT_LOCAL),)
SYSROOT_LOCAL := $(shell [ -d /usr/local/include ] && echo "/usr/local")
endif

include $(LIBRE_MK)
include mk/modules.mk

ifndef LIBREM_PATH
LIBREM_PATH	:= $(shell [ -d ../rem ] && echo "../rem")
endif

CFLAGS    += -I. -Iinclude -I$(LIBRE_INC)
ifneq ($(LIBREM_PATH),)
CFLAGS    += -I$(LIBREM_PATH)/include
endif
CFLAGS    += -I$(SYSROOT)/local/include/rem -I$(SYSROOT)/include/rem
ifneq ($(SYSROOT_LOCAL),)
CFLAGS    += -I$(SYSROOT_LOCAL)/include/rem
endif

CXXFLAGS  += -I. -Iinclude -I$(LIBRE_INC)
ifneq ($(LIBREM_PATH),)
CXXFLAGS  += -I$(LIBREM_PATH)/include
endif
CXXFLAGS  += -I$(SYSROOT)/local/include/rem -I$(SYSROOT)/include/rem
ifneq ($(SYSROOT_LOCAL),)
CXXFLAGS  += -I$(SYSROOT_LOCAL)/include/rem
endif
CXXFLAGS  += $(EXTRA_CXXFLAGS)

# XXX: common for C/C++
CPPFLAGS += -DHAVE_INTTYPES_H

ifneq ($(LIBREM_PATH),)
CLANG_OPTIONS  += -I$(LIBREM_PATH)/include
endif

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

#PREFIX  := /usr
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
#SHARE_PATH := $(PREFIX)/share/$(PROJECT)
# /usr/baresip? ??
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

ifneq ($(LIBREM_PATH),)
LIBS	+= -L$(LIBREM_PATH)
endif

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

.PHONY: clean
clean:
	@rm -rf $(BIN) $(MOD_BINS) $(SHARED) $(BUILD) \
		$(STATICLIB) libbaresip.pc
	@rm -f *stamp \
	`find . -name "*.[od]"` \
	`find . -name "*~"` \
	`find . -name "\.\#*"`

