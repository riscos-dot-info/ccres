/* _ColourDbox.c
   $Id: _ColourDbox.c,v 1.1 2003/12/09 00:00:03 joty Exp $

   Copyright (c) 2003-2004 Dave Appleby / John Tytgat

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

#include "ccres.h"

#include <stdio.h>
#include <string.h>

#include <OSLib/colourdbox.h>

static FLAGS ColourDboxFlags[] = {
	{colourdbox_GENERATE_ABOUT_TO_BE_SHOWN , "colourdbox_GENERATE_ABOUT_TO_BE_SHOWN" },
	{colourdbox_GENERATE_DIALOGUE_COMPLETED, "colourdbox_GENERATE_DIALOGUE_COMPLETED"},
	{colourdbox_INCLUDE_NONE_BUTTON        , "colourdbox_INCLUDE_NONE_BUTTON"        },
	{colourdbox_SELECT_NONE_BUTTON         , "colourdbox_SELECT_NONE_BUTTON"         }
};

static OBJECTLIST ColourDboxObjectList[] = {
	{iol_FLAGS, "colourdbox_flags:", offsetof(colourdbox_object, flags),  ColourDboxFlags, ELEMENTS(ColourDboxFlags)               },
	{iol_MSG,   "title:",            offsetof(colourdbox_object, title),  "title_limit:",  offsetof(colourdbox_object, title_limit)},
	{iol_OSCOL, "colour:",           offsetof(colourdbox_object, colour), NULL,            0                                       }
};


int _colourdbox(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object)
{
	put_objects(data, pszIn, 0, (PSTR) (object + 1), ColourDboxObjectList, ELEMENTS(ColourDboxObjectList));

	return(sizeof(colourdbox_object));
}


void colourdbox(FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable)
{
	get_objects(hf, pszStringTable, pszMessageTable, (PSTR) (object + 1), ColourDboxObjectList, ELEMENTS(ColourDboxObjectList), 1);
}
