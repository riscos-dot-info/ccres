/* _Quit.c
   $Id: _Quit.c,v 1.1 2003/12/09 00:00:10 joty Exp $

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

#include <OSLib/quit.h>


static FLAGS QuitFlags[] = {
	{quit_GENERATE_ABOUT_TO_BE_SHOWN , "quit_GENERATE_ABOUT_TO_BE_SHOWN" },
	{quit_GENERATE_DIALOGUE_COMPLETED, "quit_GENERATE_DIALOGUE_COMPLETED"}
};

static OBJECTLIST QuitObjectList[] = {
	{iol_FLAGS,   "quit_flags:",              offsetof(quit_object, flags),                   QuitFlags,        ELEMENTS(QuitFlags)                 },
	{iol_MSG,     "title:",                   offsetof(quit_object, title),                   "title_limit:",   offsetof(quit_object, title_limit)  },
	{iol_MSG,     "message:",                 offsetof(quit_object, message),                 "message_limit:", offsetof(quit_object, message_limit)},
	{iol_STRING,  "alternative_window_name:", offsetof(quit_object, alternative_window_name), NULL,             0                                   }
};

int _quit(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object)
{
	put_objects(data, pszIn, 0, (PSTR) (object + 1), QuitObjectList, ELEMENTS(QuitObjectList));

	return(sizeof(quit_object));
}


void quit(FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable)
{
	get_objects(hf, pszStringTable, pszMessageTable, (PSTR) (object + 1), QuitObjectList, ELEMENTS(QuitObjectList), 1);
}
