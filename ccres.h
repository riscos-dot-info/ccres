/* ccres.h

   Copyright (c) 2003-2003 Dave Appleby
   Copyright (c) 2003-2007 John Tytgat

   This file is part of CCres.

   CCres is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   CCres is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with CCres; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef CCRES_HEADER_INCLUDED
#define CCRES_HEADER_INCLUDED

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <oslib/fileswitch.h>
#include <oslib/gadget.h>
#include <oslib/osfile.h>
#include <oslib/saveas.h>
#include <oslib/toolbox.h>
#include <oslib/wimp.h>

#define VERSION "1.13 (xx-xxx-2007) - development"
#define APPNAME	"CCres"

#define RESF	0x46534552

#define MAX_PATH	1024

#define ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ALIGN(n) (((n)+3)&~3)

//***************************************************************************

enum {
	iol_MSG = 1,	// toolbox_RELOCATE_MSG_REFERENCE
	iol_STRING,		// toolbox_RELOCATE_STRING_REFERENCE
	iol_SPRITE,		// toolbox_RELOCATE_SPRITE_AREA_REFERENCE
	iol_OBJECT,		// toolbox_RELOCATE_OBJECT_OFFSET, but without entry in text file
	iol_CHARPTR,		// bodge for iol_OBJECT also needing string
	iol_BITS,		// unsigned int
	iol_INT,		// signed int
	iol_SHORT,		// 16 bit int
	iol_BYTE,		// unsigned char
	iol_FLAGS,		// zero or more int symbols OR'd together
	iol_BFLAGS,		// zero or more byte symbols OR'd together
	iol_ENUM,		// single int symbol
	iol_BOX,		// four consecutive ints
	iol_COORD,		// two consecutive ints
	iol_OSCOL,		// os_colour (int) value
	iol_CMP,		// component (mix of ENUM & INT)
	iol_ESG,		// ESG selection group OR'd with wimp_icon_flags (bits 16-20)
	iol_WCOL,		// wimp_colours - similar to BENUM, but extracted to share data with iol_BCOLS
	iol_BCOLS,		// button fg/bg wimp_colours OR'd with wimp_icon_flags (bits 24-27, 28-31)
	iol_TSTRING,	// Template string (0d terminated)
	iol_IFLAGS		// Icon flags (mix of flags & button type)
};

// modifiers for iol_BITS
enum {
	bits_ACTION = 1,	// use value of -1 if unset
	bits_EVAL		// use Eval rather than atoi()
};

// The following should in fact be defined in OSLib but aren't in 6.50
// release :
typedef struct toolbox_relocatable_object_base toolbox_relocatable_object_base;
struct toolbox_relocatable_object_base
   {  int string_table_offset;
      int message_table_offset;
      int relocation_table_offset;
      toolbox_resource_file_object_base rf_obj;
   };

typedef struct {
	char *pstr;
	int ref, max;
} STRINGTABLE;

typedef struct {
	int offset, type;
} RELOC;

typedef struct {
	RELOC *pReloc;
	int ref, max;
} RELOCTABLE;

typedef struct {
	int nTable;
	char *pszEntry;
	int nEntry;
	char *pszLimit;
	int nLimit;
} STRINGLIST;

typedef struct {
	int nTable;
	char *pszEntry;
	int nEntry;
	const void *pData;
	int nData;
} OBJECTLIST;

typedef struct {
	bool fRunning;			// see check_quit()
	bool fThrowback;
	bool fUnsafeLoad;
	int returnStatus;
	wimp_t task;
	char *pszIn;
        char *pszOut;
	int cbIn, cbOut;
	bits nFiletypeIn, nFiletypeOut;
	STRINGTABLE StringTable, MessageTable;
	RELOCTABLE RelocTable;
	toolbox_o idBaricon, idSaveAs;		// toolbox objects created from res file
	osspriteop_area * pSprites;		// ...and sprite area
	toolbox_block tb;			// ...easy-access toolbox block
	union {
		saveas_action_save_to_file_block sa;
		toolbox_action ta;		// ...and action data
		wimp_block wb;
	} poll;
	char achTextFile[MAX_PATH];
} DATA;

typedef void (* action_handler)(DATA *data);
typedef int  (* text2object)(DATA *data, char *pszIn, toolbox_relocatable_object_base *object);
typedef void (* object2text)(DATA *data, FILE * hf, toolbox_resource_file_object_base *object, char *pszStringTable, char *pszMessageTable);
typedef int  (* text2gadget)(DATA *data, char *pszIn, int nOffset, gadget_object_base *gadget);
typedef void (* gadget2text)(DATA *data, FILE *hf, gadget_object_base * gadget, char *pszStringTable, char *pszMessageTable);

typedef struct {
	toolbox_class class_no;
	object2text o2t;
	text2object t2o;
	char *name;
} CLASSES;

typedef struct {
	toolbox_class class_no;
	gadget2text g2t;
	text2gadget t2g;
	char *name;
} GADGETS;

typedef struct {
	bits flag;
	char *pstr;
} FLAGS;


// library.c

int my_strcpy(char *to, const char *from);
int my_strcpy0d(char *to, const char *from);
void my_strncpy0d(char *to, const char *from, int max);
int my_atoi(char **inPP);
#ifndef HAVE_STRCASECMP
int strcasecmp(const char *s1, const char *s2);
#endif
#ifndef HAVE_STRNCASECMP
int strncasecmp(const char *s1, const char *s2, size_t n);
#endif

// filer.c

void message_data_save(DATA *data);
void message_data_load(DATA *data);

// menu.c

void menu_quit(DATA *data);


// saveas.c

void action_save_to_file(DATA *data);
void action_save_completed(DATA *data);

#endif
