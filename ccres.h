/* ccres.h
   $Id: ccres.h,v 1.6 2005/01/30 14:56:15 joty Exp $

   Copyright (c) 2003-2005 Dave Appleby / John Tytgat

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

#include <OSLib/fileswitch.h>
#include <OSLib/gadget.h>
#include <OSLib/osfile.h>
#include <OSLib/saveas.h>
#include <OSLib/toolbox.h>
#include <OSLib/wimp.h>

#define VERSION "1.12 (XX-XXX-2005)"
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
	iol_PSTR,		// bodge for iol_OBJECT also needing string
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
	bits_EVAL			// use Eval rather than __atoi
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

typedef int BOOL;
typedef int * PINT;
typedef bits * PBITS;
typedef short * PSHORT;
typedef unsigned char * PBYTE;
typedef char * PSTR;
typedef void * PVOID;

typedef struct {
	PSTR pstr;
	int ref, max;
} STRINGTABLE;
typedef STRINGTABLE * PSTRINGTABLE;

typedef struct {
	int offset, type;
} RELOC;
typedef RELOC * PRELOC;

typedef struct {
	PRELOC pReloc;
	int ref, max;
} RELOCTABLE;
typedef RELOCTABLE * PRELOCTABLE;

typedef struct {
	int nTable;
	PSTR pszEntry;
	int nEntry;
	PSTR pszLimit;
	int nLimit;
} STRINGLIST;
typedef STRINGLIST * PSTRINGLIST;

typedef struct {
	int nTable;
	PSTR pszEntry;
	int nEntry;
	const void *pData;
	int nData;
} OBJECTLIST;
typedef OBJECTLIST * POBJECTLIST;

typedef struct {
	BOOL fRunning;			// see check_quit()
	BOOL fThrowback;
	BOOL fUnsafeLoad;
	wimp_t task;
	PSTR pszIn, pszOut;
	int cbIn, cbOut;
	bits nFiletypeIn, nFiletypeOut;
	STRINGTABLE StringTable, MessageTable;
	RELOCTABLE RelocTable;
	toolbox_o idBaricon, idSaveAs;			// toolbox objects created from res file
	osspriteop_area * pSprites;				// ...and sprite area
	toolbox_block tb;						// ...easy-access toolbox block
	union {
		saveas_action_save_to_file_block sa;
		toolbox_action ta;					// ...and action data
		wimp_block wb;
	} poll;
	char achTextFile[MAX_PATH];
} DATA;
typedef DATA * PDATA;

typedef void (* action_handler)(PDATA data);
typedef int  (* text2object)(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object);
typedef void (* object2text)(FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable);
typedef int  (* text2gadget)(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
typedef void (* gadget2text)(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);

typedef struct {
	toolbox_class class_no;
	object2text o2t;
	text2object t2o;
	PSTR name;
} CLASSES;

typedef struct {
	toolbox_class class_no;
	gadget2text g2t;
	text2gadget t2g;
	PSTR name;
} GADGETS;
typedef GADGETS * PGADEGET;

typedef struct {
	bits flag;
	PSTR pstr;
} FLAGS;
typedef FLAGS * PFLAGS;


// library.s

int my_strcpy(char *to, const char *from);
int my_strcpy0d(char *to, const char *from);
void my_strncpy0d(char *to, const char *from, int max);
int __stricmp(const char *p, const char *q);
int __strnicmp(const char *p, const char *q, int n);
unsigned int __atoi(PSTR * pszNumber);
fileswitch_object_type my_osfile_exists(PSTR pszFile);
int my_osfile_filesize(PSTR pszFile);
bits my_osfile_filetype(PSTR pszFile);
int my_osfile_load(PSTR pszFile, PSTR pszBuff, int cbBuff);
int my_osfscontrol_count_objects(PSTR pszDir);


// convert.c


// memset is (almost certainly) not required... so macro's will do
#define reset_string_table(pTable) (pTable)->ref=0
#define reset_reloc_table(pTable) (pTable)->ref=0

BOOL convert(PDATA data, PSTR pszOutFile);


// eval.c

int Eval(PDATA data, PSTR * pstr);


// filer.c

void message_data_save(PDATA data);
void message_data_load(PDATA data);
BOOL load_file(PDATA data, PSTR pszPath, bits nFiletype);


// main.c

extern const char achProgName[];
extern int returnStatus;


// menu.c

void menu_quit(PDATA data);


// misc.c

wimp_t is_running(void);

void * My_Alloc(int cb, PSTR pszFile, int nLine);
#define MyAlloc(v) My_Alloc(v, __FILE__, __LINE__)
#ifdef DEBUG
void MyAlloc_Init(void);
void MyAlloc_Report(void);
void My_Free(void * v, PSTR pszFile, int nLine);
#define MyFree(v) My_Free(v, __FILE__, __LINE__)
#else
#define MyAlloc_Init();
#define MyAlloc_Report();
#define MyFree(v) free(v)
#endif

#ifdef DEBUG
void log_on(void);
void log_it(PSTR pszFmt, ...);
#define LOG(p) log_it##p
#else
#define log_on()
#define LOG(p)
#endif


// saveas.c

void action_save_to_file(PDATA data);
void action_save_completed(PDATA data);

#endif
