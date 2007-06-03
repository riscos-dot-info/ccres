/* _Icon.h

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

#ifndef ICON_HEADER_INCLUDED
#define ICON_HEADER_INCLUDED

#include <stdio.h>

#include "ccres.h"

int button_t2g(DATA *data, char *pszIn, int nOffset, gadget_object_base * gadget);
void button_g2t(DATA *data, FILE * hf, gadget_object_base * gadget, char *pszStringTable, char *pszMessageTable);
void put_icon_data(DATA *data, char *pszIn, int nOffset, wimp_icon_data * icon_data, bits flags);
void get_icon_data(DATA *data, FILE * hf, char *pszStringTable, wimp_icon_data * icon_data, bits flags, int nIndent);
void icon_text2template(DATA *data, char *pszIn, int nOffset, wimp_icon * icon);
void icon_template2text(DATA *data, FILE * hf, char *pszStringTable, wimp_icon * icon);

#endif
