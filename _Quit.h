/* _Quit.h

   Copyright (c) 2005-2006 John Tytgat

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

#ifndef QUIT_HEADER_INCLUDED
#define QUIT_HEADER_INCLUDED

#include <stdio.h>

#include "ccres.h"

int quit_t2g(DATA *data, char *pszIn, toolbox_relocatable_object_base * object);
void quit_g2t(DATA *data, FILE * hf, toolbox_resource_file_object_base * object, char *pszStringTable, char *pszMessageTable);

#endif
