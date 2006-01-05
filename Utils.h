/* Utils.h
   $Id: Utils.h,v 1.1 2005/01/30 16:09:43 joty Exp $

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

#ifndef UTILS_HEADER_INCLUDED
#define UTILS_HEADER_INCLUDED

#include <OSLib/types.h>

#include "ccres.h"

// memset is (almost certainly) not required... so macro's will do
#define reset_string_table(pTable) (pTable)->ref=0
#define reset_reloc_table(pTable) (pTable)->ref=0

void HexToUInt(PDATA data, const char *strP, unsigned int len, unsigned int *resultP);

bits get_filetype(PDATA sessionP, const char *filenameP);
BOOL load_file(PDATA data, PSTR pszPath, bits nFiletype);

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
void log_on(PDATA data);
void log_it(PDATA data, PSTR pszFmt, ...);
#define LOG(p) log_it##p
#else
#define log_on(d)
#define LOG(p)
#endif

#endif
