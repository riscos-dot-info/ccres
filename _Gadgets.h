/* _Gadgets.h
   $Id: $

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

#ifndef GADGETS_HEADER_INCLUDED
#define GADGETS_HEADER_INCLUDED

#include <stdio.h>

#include "ccres.h"

int actionbutton_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void actionbutton_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int adjuster_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void adjuster_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int displayfield_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void displayfield_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int draggable_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void draggable_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int labelledbox_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void labelledbox_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int label_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void label_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int numberrange_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void numberrange_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int optionbutton_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void optionbutton_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int popup_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void popup_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int radiobutton_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void radiobutton_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int scrolllist_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void scrolllist_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int slider_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void slider_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int stringset_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void stringset_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int textarea_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void textarea_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int toolaction_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void toolaction_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int writablefield_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void writablefield_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int tabs_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void tabs_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);
int treeview_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget);
void treeview_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable);

#endif
