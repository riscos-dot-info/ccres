/* _Object.h

   Copyright (c) 2005-2007 John Tytgat

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

#ifndef OBJECT_HEADER_INCLUDED
#define OBJECT_HEADER_INCLUDED

#include <stdio.h>

#include "ccres.h"

void put_objects(DATA *data, const char *pszIn, int nOffset, char *object, const OBJECTLIST *Objectlist, int nObjects);
void get_objects(DATA *data, FILE *hf, const TOOLBOXSMTABLE *strMsgTableP, const char *objectP, const OBJECTLIST *ObjectList, int nObjects, int nIndent);
const char *next_object(const char **pszIn, const char *pszEnd);
const char *object_end(DATA *data, const char *pszIn, const char *pszEnd);
void object_text2resource(DATA *data, FILE *hf, const char *pszIn, char *pszOut, const CLASSES *pClass);
void object_resource2text(DATA *data, FILE *hf, toolbox_relocatable_object_base *object, object2text o2t);

bool alloc_string_table(STRINGTABLE *pTable);
bool add_to_reloc_table(RELOCTABLE *pRelocTable, int nEntry, int nTable);
void free_string_table(STRINGTABLE *pTable);
bool alloc_reloc_table(RELOCTABLE *pTable);
void free_reloc_table(RELOCTABLE *pTable);

#endif
