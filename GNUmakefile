# Project name: CCRes
# Written by John Tytgat / BASS

GCCSDK_INSTALL_CROSSBIN ?= /home/riscos/cross/bin
GCCSDK_INSTALL_ENV ?= /home/riscos/env

# Using the native host compiler:
HOST_CC = /usr/bin/gcc
HOST_CCFLAGS = -I${GCCSDK_INSTALL_ENV}/include -DHAVE_STRCASECMP -DHAVE_STRNCASECMP
# Using the GCCSDK cross-compiler:
CROSS_CC = ${GCCSDK_INSTALL_CROSSBIN}/gcc
CROSS_CCFLAGS = -I${GCCSDK_INSTALL_ENV}/include -mpoke-function-name
# -mlibscl

ifeq ($(CROSS_COMPILE),)
CC=$(HOST_CC)
CCFLAGS=$(HOST_CCFLAGS)
else
CC = $(CROSS_CC)
CCFLAGS = $(CROSS_CCFLAGS)
endif

#CCFLAGS += -O3
CCFLAGS += -g -O0
CCFLAGS += -std=c99 -W -Wall -Wundef -Wpointer-arith -Wcast-qual \
        -Wcast-align -Wwrite-strings -Wstrict-prototypes \
        -Wmissing-prototypes -Wmissing-declarations \
        -Wnested-externs -Winline -Wno-unused-parameter

COPY = cp
COPYFLAGS = 

MKDIR = mkdir
MKDIRFLAGS = -p

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

CCRES_LIBOBJS = Eval.o Convert.o Library.o Error.o \
	_ColourDbox.o _ColourMenu.o _DCS.o \
	_FileInfo.o _FontDbox.o _FontMenu.o _Gadgets.o \
	_Icon.o _Iconbar.o _Menu.o _Object.o _PrintDbox.o \
	_ProgInfo.o _Quit.o _SaveAs.o _Scale.o _Window.o

ifeq ($(CROSS_COMPILE),)
CCRES_APPDATA = 
APPEXT =
else
CCRES_APPDATA = Release/\!CCres/Res,fae \
	Release/\!CCres/\!Run,feb \
	Release/\!CCres/\!Sprites,ff9 \
	Release/\!CCres/\!Sprites22,ff9 \
	Release/\!CCres/COPYING \
	Release/\!CCres/\!Help \
	Release/\!CCres/Messages \
	Release/\!CCres/\!RunImage$(APPEXT) \
	Release/\!CCres/History
APPEXT = ,ff8
endif

DOBJS = Release/ccres$(APPEXT) $(CCRES_APPDATA)
	

.PHONY: all clean

.INIT:
	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres

# Target:
all: $(DOBJS)

clean:
	-$(RM) $(RMFLAGS) $(CCRES_APPOBJS) $(CCRES_CMDOBJS) $(CCRES_LIBOBJS) $(DOBJS)

Release/\!CCres/Res,fae: Data/Res,fae
	$(COPY) Data/Res,fae $@ $(COPYFLAGS)

Release/\!CCres/\!Run,feb: Data/Run,feb
	$(COPY) Data/Run,feb $@ $(COPYFLAGS)

Release/\!CCres/\!Sprites,ff9: Data/Sprites,ff9
	$(COPY) Data/Sprites,ff9 $@ $(COPYFLAGS)

Release/\!CCres/\!Sprites22,ff9: Data/Sprites22,ff9
	$(COPY) Data/Sprites22,ff9 $@ $(COPYFLAGS)

Release/\!CCres/COPYING: Data/COPYING
	$(COPY) Data/COPYING $@ $(COPYFLAGS)

Release/\!CCres/\!Help: Data/Help
	$(COPY) Data/Help $@ $(COPYFLAGS)

Release/\!CCres/Messages: Data/Messages
	$(COPY) Data/Messages $@ $(COPYFLAGS)

Release/\!CCres/History: Doc/History
	$(COPY) Doc/History $@ $(COPYFLAGS)

Release/\!CCres/\!RunImage$(APPEXT): $(CCRES_APPOBJS) $(CCRES_LIBOBJS)
	$(CC) $(CCFLAGS) -o $@ $(CCRES_APPOBJS) $(CCRES_LIBOBJS) $(USRLIBS)

ifeq ($(COMPILE_AT_ONCE),)
Release/ccres$(APPEXT): $(CCRES_CMDOBJS) $(CCRES_LIBOBJS)
	$(CC) $(CCFLAGS) -o $@ $(CCRES_CMDOBJS) $(CCRES_LIBOBJS) $(USRLIBS)
else
Release/ccres$(APPEXT): $(CCRES_CMDOBJS:.o=.c) $(CCRES_LIBOBJS:.o=.c)
	$(CC) $(CCFLAGS) -o $@ $(CCRES_CMDOBJS:.o=.c) $(CCRES_LIBOBJS:.o=.c) $(USRLIBS)
endif

.SUFFIXES: .o .c
.c.o:;  	$(CC) $(CCFLAGS) -o $@ -c $<

# Dynamic dependencies:
