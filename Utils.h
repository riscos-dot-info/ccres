/* Utils.h
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

#ifndef UTILS_HEADER_INCLUDED
#define UTILS_HEADER_INCLUDED

#include <OSLib/types.h>

#include "ccres.h"

void HexToUInt(PDATA data, const char *strP, unsigned int len, unsigned int *resultP);

#endif
