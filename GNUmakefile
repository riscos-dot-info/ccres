# Project name: CCRes
# Written by John Tytgat / BASS

GCCSDK_INSTALL_CROSSBIN ?= /home/riscos/cross/bin
GCCSDK_INSTALL_ENV ?= /home/riscos/env

HOST_CC = /usr/bin/gcc
CROSS_CC = ${GCCSDK_INSTALL_CROSSBIN}/gcc
HOST_CCFLAGS = -I${GCCSDK_INSTALL_ENV}/include -g -DHAVE_STRCASECMP -DHAVE_STRNCASECMP
CROSS_CCFLAGS = -I${GCCSDK_INSTALL_ENV}/include -O3 -mlibscl

ifeq ($(CROSS_COMPILE),)
CC=$(HOST_CC)
CCFLAGS=$(HOST_CCFLAGS)
else
CC = $(CROSS_CC)
CCFLAGS = $(CROSS_CCFLAGS)
endif

CCFLAGS += -std=c99

COPY = copy
COPYFLAGS = A ~C ~D F ~L ~N ~P Q ~R ~S ~T V

MKDIR = mkdir
MKDIRFLAGS =

RM = rm
RMFLAGS =

# Dependencies

# User libraries
ifeq ($(CROSS_COMPILE),)
USRLIBS = 
else
USRLIBS = -L${GCCSDK_INSTALL_ENV}/lib -lOSLib32
endif

# Object files
CCRES_APPOBJS = Filer.o Main.o Menu.o SaveAs.o

CCRES_CMDOBJS = CMD.o

CCRES_LIBOBJS = Eval.o Convert.o Utils.o Library.o Error.o \
	_ColourDbox.o _ColourMenu.o _DCS.o \
	_FileInfo.o _FontDbox.o _FontMenu.o _Gadgets.o \
	_Icon.o _Iconbar.o _Menu.o _Object.o _PrintDbox.o \
	_ProgInfo.o _Quit.o _SaveAs.o _Scale.o _Window.o

DOBJS = Release/ccres

#Release.!CCres.Res \
#	Release.!CCres.!Run \
#	Release.!CCres.!Sprites \
#	Release.!CCres.!Sprites22 \
#	Release.!CCres.COPYING \
#	Release.!CCres.!Help \
#	Release.!CCres.Messages \
#	Release.!CCres.!RunImage \
#	Release.!CCres.History \
	

.PHONY: all clean

.INIT:
	$(MKDIR) $(MKDIRFLAGS) Release

# Target:
all: $(DOBJS)

clean:
	-$(RM) $(RMFLAGS) $(CCRES_LIBOBJS) $(CCRES_CMDOBJS) Release/ccres Release/ccres,ff8

##Release.!CCres.Res: Data.Res
##	$(COPY) Data.Res $@ $(COPYFLAGS)
##
##Release.!CCres.!Run: Data.Run
##	$(COPY) Data.Run $@ $(COPYFLAGS)
##
##Release.!CCres.!Sprites: Data.Sprites
##	$(COPY) Data.Sprites $@ $(COPYFLAGS)
##
##Release.!CCres.!Sprites22: Data.Sprites22
##	$(COPY) Data.Sprites22 $@ $(COPYFLAGS)
##
##Release.!CCres.COPYING: Data.COPYING
##	$(COPY) Data.COPYING $@ $(COPYFLAGS)
##
##Release.!CCres.!Help: Data.Help
##	$(COPY) Data.Help $@ $(COPYFLAGS)
##
##Release.!CCres.Messages: Data.Messages
##	$(COPY) Data.Messages $@ $(COPYFLAGS)
##
##Release.!CCres.History: History
##	$(COPY) History $@ $(COPYFLAGS)
##
##Release.!CCres.!RunImage: $(CCRES_APPOBJS) $(CCRES_LIB) $(LDLIBS) $(USRLIBS)
##	$(LINK) $(LINKFLAGS) -o $@ $(CCRES_APPOBJS) $(CCRES_LIB) $(LDLIBS) $(USRLIBS)
##
##o.CCResLib: $(CCRES_LIBOBJS)
##	$(LIBFILE) $(LIBFILEFLAGS) $(CCRES_LIBOBJS)

ifeq ($(COMPILE_AT_ONCE),)
Release/ccres: $(CCRES_CMDOBJS) $(CCRES_LIBOBJS)
	$(CC) $(CCFLAGS) -o $@ $(CCRES_CMDOBJS) $(CCRES_LIBOBJS) $(USRLIBS)
else
Release/ccres: $(CCRES_CMDOBJS:.o=.c) $(CCRES_LIBOBJS:.o=.c)
	$(CC) $(CCFLAGS) -o $@ $(CCRES_CMDOBJS:.o=.c) $(CCRES_LIBOBJS:.o=.c) $(USRLIBS)
endif

.SUFFIXES: .o .c
.c.o:;  	$(CC) $(CCFLAGS) -o $@ -c $<

# Dynamic dependencies:
