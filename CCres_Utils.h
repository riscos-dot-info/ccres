/* CCres_Utils.h

   Copyright (c) 2007 John Tytgat

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

#ifndef CCRES_UTILS_HEADER_INCLUDED
#define CCRES_UTILS_HEADER_INCLUDED

#include <stdint.h>
#include <stdlib.h>

#include "CCres_Internal.h"

int my_strcpy(char *to, const char *from);
int my_strcpy0d(char *to, const char *from);
void my_strncpy0d(char *to, const char *from, int max);
int my_atoi(const char **inPP);
#ifndef HAVE_STRCASECMP
int strcasecmp(const char *s1, const char *s2);
#endif
#ifndef HAVE_STRNCASECMP
int strncasecmp(const char *s1, const char *s2, size_t n);
#endif

unsigned int HexToUInt(DATA *data, const char *strP, unsigned int len);

unsigned int getlinenr(const DATA *sessionP, const char *ptrP);

void *My_Alloc(int cb, const char *pszFile, int nLine);
void *My_Realloc(void *b, int cb, const char *pszFile, int nLine);
#define MyAlloc(v) My_Alloc(v, __FILE__, __LINE__)
#define MyRealloc(v, c) My_Realloc(v, c, __FILE__, __LINE__)
#define MyFree(v) free(v)

/* Write integer value 'value' to memory at 'memP' as little endian 4 byte
   integer.  */
void write_le_int32(void *memP, int32_t value);
/* Read little endian integer value at memory 'memP'.  */
int32_t read_le_int32(const void *memP);

void write_le_uint32(void *memP, uint32_t value);
uint32_t read_le_uint32(const void *memP);

int16_t read_le_int16(const void *memP);

void write_le_uint16(void *memP, uint16_t value);

void write_le_uint8(void *memP, uint8_t value);

#endif
