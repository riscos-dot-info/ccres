/* Utils.c
   $Id: $

   Copyright (c) 2004-2005 John Tytgat

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

#include "Error.h"
#include "Utils.h"

        void HexToUInt(PDATA data, const char *strP, unsigned int len, unsigned int *resultP)
//      =====================================================================================
{
unsigned int result;
const char *cpStrP;

if (len < 2 || strP[0] != '0' || strP[1] != 'x')
  {
  report(data, strP, "Unknown hex construction '%.*s'", len, strP);
  *resultP = 0;
  return;
  }
result = 0;
for (cpStrP = strP + 2, len -= 2; len-- > 0; ++cpStrP)
  {
  unsigned int nextResult;
  if (*cpStrP < '0'
      || *cpStrP > '9' && *cpStrP < 'A'
      || *cpStrP > 'F' && *cpStrP < 'a'
      || *cpStrP > 'f')
    {
    report(data, strP, "Unknown hex construction '%.*s'", len, strP);
    *resultP = 0;
    return;
    }
  nextResult = result << 4;
  if (*cpStrP <= '9')
    nextResult += *cpStrP - '0';
  else if (*cpStrP <= 'F')
    nextResult += *cpStrP - 'A' + 10;
  else
    nextResult += *cpStrP - 'a' + 10;
  if (nextResult < result)
    {
    report(data, strP, "Hex overflow '%.*s'", len, strP);
    *resultP = 0;
    return;
    }
  result = nextResult;
  }
*resultP = result;
}
