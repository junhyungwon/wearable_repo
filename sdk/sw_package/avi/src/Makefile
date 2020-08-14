#INCLUDES = -I .

#COMPILE_OPTS =		$(INCLUDES) -I. -O3 -DSOCKLEN_T=socklen_t -DNO_STRSTREAM=1 -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64
COMPILE_OPTS =		-I. -O3 -DSOCKLEN_T=socklen_t -DNO_STRSTREAM=1 -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64
C =			c
C_COMPILER =		$(CROSS_COMPILE)gcc
C_FLAGS =		$(COMPILE_OPTS)
CPP =			cpp
CPLUSPLUS_COMPILER =	$(CROSS_COMPILE)c++
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall -DBSD=1
OBJ =			o
LINK =			$(CROSS_COMPILE)c++ -o
LINK_OPTS =		-L.
CONSOLE_LINK_OPTS =	$(LINK_OPTS)
LIBRARY_LINK =		$(CROSS_COMPILE)ld -o
LIBRARY_LINK_OPTS =	$(LINK_OPTS) -r -Bstatic
LIB_SUFFIX =			a
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
EXE =


ALL = libwriter.$(LIB_SUFFIX)

all: $(ALL)

.$(C).$(OBJ):
	$(C_COMPILER) -c $(C_FLAGS) $<

.$(CPP).$(OBJ):
	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $<

REL_OBJS = blockbuffer.$(OBJ) aviencoder.$(OBJ) avirecover.$(OBJ) aviwriter.$(OBJ)


blockbuffer.$(CPP):	blockbuffer.hh
aviencoder.$(CPP):	aviencoder.hh
aviencoder.hh:	mmsystem.h blockbuffer.hh
mmsystem.h:	syscommon.h
aviwriter.$(CPP):	mmsystem.h aviwriter.hh
aviwriter.hh:	blockbuffer.hh aviencoder.hh syscommon.h
avirecover.$(CPP): avirecover.hh
avirecover.hh: aviencoder.hh mmsystem.h

libwriter.$(LIB_SUFFIX): $(REL_OBJS) \
    $(PLATFORM_SPECIFIC_LIB_OBJS)
	$(LIBRARY_LINK)$@ $(LIBRARY_LINK_OPTS) $(REL_OBJS)
	mv $(ALL) ../
	rm -f $(REL_OBJS)

clean:
	rm -f $(ALL)
	rm -f $(REL_OBJS)
