# Project name: CCRes
# Written by John Tytgat / BASS

CC = cc
#CCFLAGS = -depend !Depend -throwback -DDEBUG=1 -Apcs 3/32/fpe2/swst/fp/nofpr -Wp -fahn -gflv -IOSLib: -c
CCFLAGS = -depend !Depend -throwback -Apcs 3/32/fpe2/swst/fp/nofpr -Wp -fafh -IOSLib: -c

LINK = link
LINKFLAGS = 
#LINKFLAGS = -debug 

LIBFILE = libfile
LIBFILEFLAGS = -c -o $@

OBJASM = objasm
OBJASMFLAGS = -depend !Depend -throwback -I <OSLib$Dir> -Apcs 3/32/swst/nofpr -Stamp -quit -CloseExec

COPY = copy
COPYFLAGS = A ~C ~D F ~L ~N ~P Q ~R ~S ~T V

MKDIR = cdir
MKDIRFLAGS =

# Dependencies

# System libraries
LDLIBS = C:o.StubsG

# User libraries
USRLIBS = OSLib:o.OSLib32

# Object files
CCRES_APPOBJS = o.Filer o.Main o.Menu o.Misc o.SaveAs

CCRES_CMDOBJS = o.CMD

CCRES_LIBOBJS = o.Eval o.Convert o.Utils o.Library o.Error \
	o._ColourDbox o._ColourMenu o._DCS \
	o._FileInfo o._FontDbox o._FontMenu o._Gadgets \
	o._Icon o._Iconbar o._Menu o._Object o._PrintDbox \
	o._ProgInfo o._Quit o._SaveAs o._Scale o._Window

CCRES_LIB = o.CCResLib

DOBJS = Release.!CCres.Res \
	Release.!CCres.!Run \
	Release.!CCres.!Sprites \
	Release.!CCres.!Sprites22 \
	Release.!CCres.COPYING \
	Release.!CCres.!Help \
	Release.!CCres.Messages \
	Release.!CCres.!RunImage \
	Release.!CCres.History \
	Release.ccres

# Target:
All: $(DOBJS)

Release.!CCres.Res: Data.Res
	$(COPY) Data.Res $@ $(COPYFLAGS)

Release.!CCres.!Run: Data.Run
	$(COPY) Data.Run $@ $(COPYFLAGS)

Release.!CCres.!Sprites: Data.Sprites
	$(COPY) Data.Sprites $@ $(COPYFLAGS)

Release.!CCres.!Sprites22: Data.Sprites22
	$(COPY) Data.Sprites22 $@ $(COPYFLAGS)

Release.!CCres.COPYING: Data.COPYING
	$(COPY) Data.COPYING $@ $(COPYFLAGS)

Release.!CCres.!Help: Data.Help
	$(COPY) Data.Help $@ $(COPYFLAGS)

Release.!CCres.Messages: Data.Messages
	$(COPY) Data.Messages $@ $(COPYFLAGS)

Release.!CCres.History: History
	$(COPY) History $@ $(COPYFLAGS)

Release.!CCres.!RunImage: $(CCRES_APPOBJS) $(CCRES_LIB) $(LDLIBS) $(USRLIBS)
	$(LINK) $(LINKFLAGS) -o $@ $(CCRES_APPOBJS) $(CCRES_LIB) $(LDLIBS) $(USRLIBS)

o.CCResLib: $(CCRES_LIBOBJS)
	$(LIBFILE) $(LIBFILEFLAGS) $(CCRES_LIBOBJS)

Release.ccres: $(CCRES_CMDOBJS) $(CCRES_LIB) $(LDLIBS)
	$(LINK) $(LINKFLAGS) -o $@ $(CCRES_CMDOBJS) $(CCRES_LIB) $(LDLIBS)

.SUFFIXES: .o .c .s
.c.o:;  	$(CC) $(CCFLAGS) -o $@ $<
.s.o:;  	$(OBJASM) $(OBJASMFLAGS) $< $@

.INIT:
	@$(MKDIR) $(MKDIRFLAGS) o
	@$(MKDIR) $(MKDIRFLAGS) Release
	@$(MKDIR) $(MKDIRFLAGS) Release.!CCres

# Dynamic dependencies:
