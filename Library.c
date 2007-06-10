/* Library.c

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

#if !defined(HAVE_STRCASECMP) || !defined(HAVE_STRNCASECMP)
#  include <ctype.h>
#endif
#include <stdbool.h>
#include <stdint.h>

#include "Library.h"

// DESC. | Same as strcpy() @ string.h, but returns strlen()
// EXIT  | strlen (excluding term)
int my_strcpy(char *to, const char *from)
{
  const char *d = from++;
  while ((*to++ = *d++) != '\0')
    /* */;

  return d - from;
}


// DESC. | Same as strcpy() @ string.h, but handles 0x0d terminator and
//         returns strlen()
// EXIT  | strlen (excluding term)
// COMM  | A bit stronger, any cntrl char will terminate string.
int my_strcpy0d(char *to, const char *from)
{
  const char *d = from++;
  while ((*to++ = *d++) >= ' ')
    /* */;

  return d - from;
}


// DESC. | Same as strncpy() @ string.h, but handles 0x0d terminator and
//         returns strlen()
// EXIT  | strlen (excluding term)
// COMM  | A bit stronger, any cntrl char will terminate string.
//         Always terminate with '\0' char
void my_strncpy0d(char *to, const char *from, int max)
{
  if (max == 0)
    return;

  while (--max && *from >= ' ')
    *to++ = *from++;
  *to = '\0';
}


int my_atoi(const char **inPP)
{
  const char *inP = *inPP;
// Negative ?
  bool neg;
  if (*inP == '-')
    {
      neg = true;
      inP += 1;
    }
  else
    neg = false;
// Determine 'base':
  int base;
  if (*inP == '&')
    {
      base = 16;
      inP +=1;
    }
  else if (inP[0] == '0' && inP[1] == 'x')
    {
      base = 16;
      inP += 2;
    }
  else if (inP[0] == '1' && inP[1] == '6' && inP[2] == '_')
    {
      base = 16;
      inP += 3;
    }
  else if (inP[0] == '2' && inP[1] == '_')
    {
      base = 2;
      inP += 2;
    }
  else if (inP[0] == '1' && inP[1] == '0' && inP[2] == '_')
    {
      base = 10;
      inP += 3;
    }
  else
    base = 10;

  int result = 0;
  switch (base)
    {
    case 2:
      while (*inP >= '0' && *inP <= '1')
        {
          result = result*2 + *inP - '0';
          ++inP;
        }
      break;
    case 10:
      while (*inP >= '0' && *inP <= '9')
        {
          result = result*10 + *inP - '0';
          ++inP;
        }
      break;
    case 16:
      do
        {
          if (*inP >= '0' && *inP <= '9')
            result = result*16 + *inP - '0';
          else if (*inP >= 'A' && *inP <= 'F')
            result = result*16 + *inP - 'A' + 10;
          else if (*inP >= 'a' && *inP <= 'f')
            result = result*16 + *inP - 'a' + 10;
          else
            break;
          ++inP;
        }
      while (1);
      break;
    }

  if (neg)
    result = -result;

  *inPP = inP;
  return result;
}


#ifndef HAVE_STRCASECMP
int strcasecmp(const char *s1, const char *s2)
{
  while (*s1 && toupper(*s1) == toupper(*s2))
    {
      ++s2;
      ++s1;
    }
  return toupper(*s1) - toupper(*s2);
}
#endif


#ifndef HAVE_STRNCASECMP
int strncasecmp(const char *s1, const char *s2, size_t n)
{
  while (n != 0 && *s1 && toupper(*s1) == toupper(*s2))
    {
      ++s2;
      ++s1;
      --n;
    }

  return n && toupper(*s1) - toupper(*s2);
}
#endif


void write_le_int32(void *memP, int32_t value)
{
  uint8_t *imemP = (uint8_t *)memP;
  imemP[0] = value & 0xFF;
  imemP[1] = (value >> 8) & 0xFF;
  imemP[2] = (value >> 16) & 0xFF;
  imemP[3] = (value >> 24) & 0xFF;
}


int32_t read_le_int32(const void *memP)
{
  const uint8_t *imemP = (const uint8_t *)memP;
  return imemP[0] | (imemP[1]<<8) | (imemP[2]<<16) | (imemP[3]<<24);
}


void write_le_uint32(void *memP, uint32_t value)
{
  uint8_t *imemP = (uint8_t *)memP;
  imemP[0] = value & 0xFF;
  imemP[1] = (value >> 8) & 0xFF;
  imemP[2] = (value >> 16) & 0xFF;
  imemP[3] = (value >> 24) & 0xFF;
}


uint32_t read_le_uint32(const void *memP)
{
  const uint8_t *imemP = (const uint8_t *)memP;
  return imemP[0] | (imemP[1]<<8) | (imemP[2]<<16) | (imemP[3]<<24);
}


int16_t read_le_int16(const void *memP)
{
  const uint8_t *imemP = (const uint8_t *)memP;
  return imemP[0] | (imemP[1]<<8);
}


void write_le_uint16(void *memP, uint16_t value)
{
  uint8_t *imemP = (uint8_t *)memP;
  imemP[0] = value & 0xFF;
  imemP[1] = (value >> 8) & 0xFF;
}


void write_le_uint8(void *memP, uint8_t value)
{
  *(uint8_t *)memP = value;
}
