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

void put_objects(DATA *data, char *pszIn, int nOffset, char *object, const OBJECTLIST *Objectlist, int nObjects);
void get_objects(DATA *data, FILE * hf, char *pszStringTable, char *pszMessageTable, const char *objectP, const OBJECTLIST *ObjectList, int nObjects, int nIndent);
char *next_object(char ** pszIn, char *pszEnd);
char *object_end(DATA *data, char *pszIn, char *pszEnd);
void object_text2resource(DATA *data, FILE * hf, char *pszIn, char *pszOut, const CLASSES *pClass);
void object_resource2text(DATA *data, FILE * hf, toolbox_relocatable_object_base * object, object2text o2t);

#endif
