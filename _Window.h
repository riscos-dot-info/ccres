/* _Window.h
   $Id: _Window.h,v 1.1 2005/01/30 15:59:17 joty Exp $

   Copyright (c) 2005 John Tytgat

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

#ifndef WINDOW_HEADER_INCLUDED
#define WINDOW_HEADER_INCLUDED

#include <stdio.h>

#include "ccres.h"

int window_t2g(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object);
void window_g2t(PDATA data, FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable);
int window_text2template(PDATA data, PSTR pszIn, int nOffset, wimp_window_base * object);
void window_template2text(PDATA data, FILE * hf, PSTR pszBuff);

#endif
