################################################################################
TOOL_PREFIX   = $(BUILD_TOOL_PREFIX)
CROSS_COMPILE = $(TOOL_PREFIX)
TARGET_PATH   = $(TARGET_FS)
UID=$(shell id -u)
WID=www-data
################################################################################

INC += -I. -I../srclib
COMPILE_OPTS =		-g -c
COMPILE_OPTS +=		$(INC) -O2 -DSOCKLEN_T=socklen_t -DNO_SSTREAM=1 -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64
COMPILE_OPTS +=		
C =			c
CC =			$(CROSS_COMPILE)gcc
CPP =			$(CROSS_COMPILE)cpp
GPP =			$(CROSS_COMPILE)g++
CFLAGS =		$(COMPILE_OPTS)
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall -DBSD=1
LINK =			$(CROSS_COMPILE)g++
LINK_OPTS =	
CONSOLE_LINK_OPTS =	$(LINK_OPTS)
LIBRARY_LINK =		$(CROSS_COMPILE)ar cr 
LIBRARY_LINK_OPTS =	$(LINK_OPTS)
LIB_SUFFIX =			a
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
EXE =
#LIBS += -L../lib/ -llfuds
LIBS += -L../lib/ -ljson-c
LIBS += -L../srclib/ -lLFCGI
##### End of variables to change

##### Files
.SUFFIXES: .o .c .cpp .cc .cxx .C

HEADERS = $(wildcard *.h)
SRCC = $(wildcard *.c)
OBJC = $(SRCC:%.c=%.o)

SRCGPP = $(wildcard *.cpp)
OBJGPP = $(SRCGPP:%.cpp=%.o)

ifeq ($(PRODUCT_NAME),)
$(info *********** PRODUCT_NAME is NULL. please check Rules.make)
exit 0
endif

# mkdir cgi-bin 
HAVE_CGI_DIR = $(shell if [ -e ./cgi-bin ]; then echo 1; else echo 0; fi;)
ifeq ($(HAVE_CGI_DIR),0)
$(info *********** INFO ./cgi-bin directory not found. mkdir cgi-bin)
$(shell mkdir cgi-bin)
else
$(info *********** INFO ./cgi-bin directory exist.)
endif

.c.o:
	$(CC) -c $(CFLAGS) $(INC) -o "$@" "$<"

.cpp.o:
	$(GPP) -c $(CFLAGS) $(INC) -o "$@" "$<"

all: clean $(OBJC) $(OBJGPP)

$(OBJC):%.o: %.c $(HEADER)
	@echo Linking $@ from ...
	$(CC) -o cgi-bin/$*.cgi $(INC) -DMODEL_NAME='$(MODEL_NAME)' -D$(PRODUCT_NAME) $(LDFLAGS) $(LIBS) $^ 
#	$(CC) -o $@ $(INC) $(LDFLAGS) $(LIBS) $<

$(OBJGPP):%.o: %.cpp $(HEADER)
	@echo Linking $@ from ...
	$(GPP) -o cgi-bin/$*.cgi $(INC) -DMODEL_NAME='$(MODEL_NAME)' -D$(PRODUCT_NAME) $(LDFLAGS) $(LIBS) $^ 

clean:
#	$(RM) $(OBJC)
	$(RM) cgi-bin/*.cgi


install:all

	
.PHONY:all
