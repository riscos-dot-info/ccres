/* CCres_Quit.c

   Copyright (c) 2003-2003 Dave Appleby
   Copyright (c) 2003-2007 John Tytgat

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

#include <stddef.h>
#include <stdio.h>

#include <oslib/quit.h>

#include "CCres_Internal.h"
#include "CCres_Object.h"
#include "CCres_Quit.h"

static const FLAGS QuitFlags[] =
  {
    {quit_GENERATE_ABOUT_TO_BE_SHOWN , "quit_GENERATE_ABOUT_TO_BE_SHOWN" },
    {quit_GENERATE_DIALOGUE_COMPLETED, "quit_GENERATE_DIALOGUE_COMPLETED"}
  };

static const OBJECTLIST QuitObjectList[] =
  {
    {iol_FLAGS,   "quit_flags:",              offsetof(quit_object, flags),                   QuitFlags,        ELEMENTS(QuitFlags)    },
    {iol_MSG,     "title:",                   offsetof(quit_object, title),                   "title_limit:",   offsetof(quit_object, title_limit)  },
    {iol_MSG,     "message:",                 offsetof(quit_object, message),                 "message_limit:", offsetof(quit_object, message_limit)},
    {iol_STRING,  "alternative_window_name:", offsetof(quit_object, alternative_window_name), NULL,             0                                   }
  };

int quit_t2g(DATA *data, const char *pszIn, toolbox_relocatable_object_base *object)
{
  put_objects(data, pszIn, 0, (char *)(object + 1), QuitObjectList, ELEMENTS(QuitObjectList));

  return sizeof(quit_object);
}


void quit_g2t(DATA *data, FILE *hf, const toolbox_resource_file_object_base *object, const TOOLBOXSMTABLE *strMsgTableP)
{
  get_objects(data, hf, strMsgTableP, (const char *)(object + 1), QuitObjectList, ELEMENTS(QuitObjectList), 1);
}
