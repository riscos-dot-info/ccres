/* _FontDBox.c
   $Id: _FontDbox.c,v 1.1 2003/12/09 00:00:05 joty Exp $

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

#include <OSLib/fontdbox.h>

static FLAGS FontDboxFlags[] = {
	{fontdbox_GENERATE_ABOUT_TO_BE_SHOWN , "fontdbox_GENERATE_ABOUT_TO_BE_SHOWN" },
	{fontdbox_GENERATE_DIALOGUE_COMPLETED, "fontdbox_GENERATE_DIALOGUE_COMPLETED"}
};

static OBJECTLIST FontDboxObjectList[] = {
	{iol_FLAGS,  "fontdbox_flags:",          offsetof(fontdbox_object, flags),                   FontDboxFlags,  ELEMENTS(FontDboxFlags)               },
	{iol_MSG,    "title:",                   offsetof(fontdbox_object, title),                   "title_limit:", offsetof(fontdbox_object, title_limit)},
	{iol_STRING, "initial_font:",            offsetof(fontdbox_object, initial.font),            NULL,           0                                     },
	{iol_INT,    "initial_height:",          offsetof(fontdbox_object, initial.height),          NULL,           0                                     },
	{iol_INT,    "initial_aspect:",          offsetof(fontdbox_object, initial.aspect),          NULL,           0                                     },
	{iol_MSG,    "try_string:",              offsetof(fontdbox_object, try_string),              NULL,           0                                     },
	{iol_STRING, "alternative_window_name:", offsetof(fontdbox_object, alternative_window_name), NULL,           0                                     }
};


int _fontdbox(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object)
{
	put_objects(data, pszIn, 0, (PSTR) (object + 1), FontDboxObjectList, ELEMENTS(FontDboxObjectList));

	return(sizeof(fontdbox_object));
}


void fontdbox(FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable)
{
	get_objects(hf, pszStringTable, pszMessageTable, (PSTR) (object + 1), FontDboxObjectList, ELEMENTS(FontDboxObjectList), 1);
}
