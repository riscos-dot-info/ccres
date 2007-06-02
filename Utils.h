/* Utils.h

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

#ifndef UTILS_HEADER_INCLUDED
#define UTILS_HEADER_INCLUDED

#include <oslib/types.h>

#include "ccres.h"

// memset is (almost certainly) not required... so macro's will do
#define reset_string_table(pTable) (pTable)->ref=0
#define reset_reloc_table(pTable) (pTable)->ref=0

void HexToUInt(PDATA data, const char *strP, unsigned int len, unsigned int *resultP);

bits get_filetype(PDATA sessionP, const char *filenameP);
BOOL load_file(PDATA data, PSTR pszPath, bits nFiletype);

void * My_Alloc(int cb, PSTR pszFile, int nLine);
#define MyAlloc(v) My_Alloc(v, __FILE__, __LINE__)
#define MyFree(v) free(v)

#endif
