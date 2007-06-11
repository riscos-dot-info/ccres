# Project name: CCRes
# Written by John Tytgat / BASS

# Only to be used for cross-compilation with GCCSDK 3.4.6 or GCCSDK 4
# or native compile on an Unix alike host.  OSLib headers need to be
# available at $GCCSDK_INSTALL_ENV/include/oslib.
# Variables needed:
#   - cross-compilation (CROSS_COMPILE is non-empty):
#     GCCSDK_INSTALL_CROSS_BIN & GCCSDK_INSTALL_ENV
#   - native build (CROSS_COMPILE is empty/not defined):
#     GCCSDK_INSTALL_ENV
#   Both: COMPILE_AT_ONCE : when non-empty all C code will be compiled as one unit.

# Using the native host compiler:
HOST_CC = /usr/bin/gcc
# It is very important to have first "-I." and then the rest of the unfiltered OSLib
# headers.
HOST_CCFLAGS = -I. -I${GCCSDK_INSTALL_ENV}/include -DHAVE_STRCASECMP -DHAVE_STRNCASECMP
# Using the GCCSDK cross-compiler:
CROSS_CC = ${GCCSDK_INSTALL_CROSSBIN}/gcc
CROSS_CCFLAGS = -I${GCCSDK_INSTALL_ENV}/include -mpoke-function-name -mlibscl

ifeq ($(CROSS_COMPILE),)
CC=$(HOST_CC)
CCFLAGS=$(HOST_CCFLAGS)
else
CC = $(CROSS_CC)
CCFLAGS = $(CROSS_CCFLAGS)
endif

CCFLAGS += -O3
#CCFLAGS += -g -O0
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

# User libraries
ifeq ($(CROSS_COMPILE),)
USRLIBS =
else
USRLIBS = -L${GCCSDK_INSTALL_ENV}/lib -lOSLib32
endif

# Object files
CCRES_CMDOBJS = CMD.o

CCRES_LIBOBJS = CCRes_Eval.o CCRes_Convert.o CCRes_Utils.o CCRes_Report.o \
	CCRes_ColourDbox.o CCRes_ColourMenu.o CCRes_DCS.o \
	CCRes_FileInfo.o CCRes_FontDbox.o CCRes_FontMenu.o CCRes_Gadgets.o \
	CCRes_Icon.o CCRes_Iconbar.o CCRes_Menu.o CCRes_Object.o CCRes_PrintDbox.o \
	CCRes_ProgInfo.o CCRes_Quit.o CCRes_SaveAs.o CCRes_Scale.o CCRes_Window.o

ifeq ($(CROSS_COMPILE),)
# Only the OSLib headers which are used to read & write Resource and Template files
# and need their pointers replaced by 32-bit int type.
# We include also ospriteop.h and wimp.h for filtering the wimp_w and osspriteop_id
# pointer types.
OSLIB_HEADERS_TOFILTER = oslib/actionbutton.h oslib/adjuster.h oslib/button.h \
	oslib/colourdbox.h oslib/colourmenu.h oslib/dcs.h \
	oslib/displayfield.h oslib/draggable.h oslib/fileinfo.h \
	oslib/fontdbox.h oslib/fontmenu.h oslib/gadget.h \
	oslib/iconbar.h oslib/keyboardshortcut.h oslib/label.h \
	oslib/labelledbox.h oslib/menu.h oslib/numberrange.h \
	oslib/optionbutton.h oslib/popup.h oslib/printdbox.h \
	oslib/proginfo.h oslib/quit.h oslib/radiobutton.h \
	oslib/saveas.h oslib/scale.h oslib/scrolllist.h \
	oslib/slider.h oslib/stringset.h oslib/textarea.h \
	oslib/textgadgets.h oslib/toolaction.h oslib/toolbox.h \
	oslib/window.h oslib/writablefield.h \
	\
	oslib/osspriteop.h oslib/wimp.h
CCRES_APPOBJS =
CCRES_APPDATA = 
APPEXT =
else
OSLIB_HEADERS_TOFILTER =
CCRES_APPOBJS = Filer.o Main.o Menu.o SaveAs.o
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

# Target:
all: $(DOBJS)

clean:
	-$(RM) $(RMFLAGS) $(CCRES_APPOBJS) $(CCRES_CMDOBJS) $(CCRES_LIBOBJS) $(DOBJS) $(OSLIB_HEADERS_TOFILTER)

Release/\!CCres/Res,fae: Release/ccres Data/Res
	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
	Release/ccres Data/Res $@ $(COPYFLAGS)

Release/\!CCres/\!Run,feb: Data/!Run,feb
	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
	$(COPY) $< $@ $(COPYFLAGS)

Release/\!CCres/\!Sprites,ff9: Data/!Sprites,ff9
	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
	$(COPY) $< $@ $(COPYFLAGS)

Release/\!CCres/\!Sprites22,ff9: Data/!Sprites22,ff9
	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
	$(COPY) $< $@ $(COPYFLAGS)

Release/\!CCres/COPYING: Doc/COPYING
	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
	$(COPY) $< $@ $(COPYFLAGS)

Release/\!CCres/\!Help: Doc/!Help
	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
	$(COPY) $< $@ $(COPYFLAGS)

Release/\!CCres/Messages: Data/Messages
	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
	$(COPY) $< $@ $(COPYFLAGS)

Release/\!CCres/History: Doc/History
	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
	$(COPY) $< $@ $(COPYFLAGS)

Release/\!CCres/\!RunImage$(APPEXT): $(CCRES_APPOBJS) $(CCRES_LIBOBJS)
	$(MKDIR) $(MKDIRFLAGS) Release/\!CCres
	$(CC) $(CCFLAGS) -o $@ $(CCRES_APPOBJS) $(CCRES_LIBOBJS) $(USRLIBS)

ifeq ($(COMPILE_AT_ONCE),)
Release/ccres$(APPEXT): $(CCRES_CMDOBJS) $(CCRES_LIBOBJS)
	$(MKDIR) $(MKDIRFLAGS) Release/
	$(CC) $(CCFLAGS) -o $@ $(CCRES_CMDOBJS) $(CCRES_LIBOBJS) $(USRLIBS)
else
Release/ccres$(APPEXT): $(CCRES_CMDOBJS:.o=.c) $(CCRES_LIBOBJS:.o=.c)
	$(MKDIR) $(MKDIRFLAGS) Release/
	$(CC) $(CCFLAGS) -o $@ $(CCRES_CMDOBJS:.o=.c) $(CCRES_LIBOBJS:.o=.c) $(USRLIBS)
endif

$(CCRES_LIBOBJS) :  $(OSLIB_HEADERS_TOFILTER)
# To filter the pointer type usage in OSLib headers and replace it by a 32-bit int type.
# This is a very rough and crude hack but seems to work.
oslib/%.h: ${GCCSDK_INSTALL_ENV}/include/oslib/%.h
	$(MKDIR) $(MKDIRFLAGS) oslib
	cat $< | sed -r -e 's,(struct\s+)?\w+\s*(const\s+)?\*+([^/]),int \3,g' > $@

.SUFFIXES: .o .c
.c.o:
	$(CC) $(CCFLAGS) -o $@ -c $<
