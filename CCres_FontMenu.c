/* CCres_FontMenu.c

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
#include <string.h>

#include <oslib/fontmenu.h>

#include "CCres_Internal.h"
#include "CCres_Object.h"
#include "CCres_FontMenu.h"

static const FLAGS FontMenuFlags[] =
  {
    {fontmenu_GENERATE_ABOUT_TO_BE_SHOWN , "fontmenu_GENERATE_ABOUT_TO_BE_SHOWN" },
    {fontmenu_GENERATE_DIALOGUE_COMPLETED, "fontmenu_GENERATE_DIALOGUE_COMPLETED"},
    {fontmenu_GENERATE_HAS_BEEN_HIDDEN   , "fontmenu_GENERATE_HAS_BEEN_HIDDEN"   },
    {fontmenu_INCLUDE_SYSTEM_FONT        , "fontmenu_INCLUDE_SYSTEM_FONT"        }
  };

static const OBJECTLIST FontMenuObjectList[] =
  {
    {iol_FLAGS,  "fontmenu_flags:", offsetof(fontmenu_object, flags),       FontMenuFlags, ELEMENTS(FontMenuFlags)},
    {iol_STRING, "ticked_font:",    offsetof(fontmenu_object, ticked_font), NULL,          0                      }
  };


int fontmenu_t2g(DATA *data, const char *pszIn, toolbox_relocatable_object_base *object)
{
  put_objects(data, pszIn, 0, (char *)(object + 1), FontMenuObjectList, ELEMENTS(FontMenuObjectList));

  return sizeof(fontmenu_object);
}


void fontmenu_g2t(DATA *data, FILE *hf, const toolbox_resource_file_object_base *object, const TOOLBOXSMTABLE *strMsgTableP)
{
  get_objects(data, hf, strMsgTableP, (const char *)(object + 1), FontMenuObjectList, ELEMENTS(FontMenuObjectList), 1);
}
