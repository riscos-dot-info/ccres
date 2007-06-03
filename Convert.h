/* Convert.h

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

#ifndef CONVERT_HEADER_INCLUDED
#define CONVERT_HEADER_INCLUDED

#include "ccres.h"

// Returns 0 for error, 1 otherwise.
int ccres_initialise(DATA *sessionP);
// Returns 0 for error, 1 otherwise.
int ccres_finish(void);
// Returns 0 for error, 1 otherwise.
bool ccres_convert(DATA *data, char *pszOutFile);

#endif
