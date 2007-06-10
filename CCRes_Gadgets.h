/* CCRes_Gadgets.h

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

#ifndef CCRES_GADGETS_HEADER_INCLUDED
#define CCRES_GADGETS_HEADER_INCLUDED

#include <stdio.h>

#include "CCRes_Internal.h"

int actionbutton_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void actionbutton_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int adjuster_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void adjuster_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int displayfield_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void displayfield_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int draggable_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void draggable_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int labelledbox_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void labelledbox_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int label_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void label_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int numberrange_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void numberrange_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int optionbutton_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void optionbutton_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int popup_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void popup_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int radiobutton_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void radiobutton_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int scrolllist_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void scrolllist_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int slider_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void slider_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int stringset_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void stringset_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int textarea_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void textarea_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int toolaction_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void toolaction_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int writablefield_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void writablefield_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int tabs_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void tabs_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);
int treeview_t2g(DATA *data, const char *pszIn, int nOffset, gadget_object_base *gadget);
void treeview_g2t(DATA *data, FILE *hf, const gadget_object_base *gadget, const TOOLBOXSMTABLE *strMsgTableP);

#endif
