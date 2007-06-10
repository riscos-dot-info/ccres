/* _ColourMenu.c

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

#include <oslib/colourmenu.h>

#include "ccres.h"
#include "_ColourMenu.h"
#include "_Object.h"

static const FLAGS ColourMenuFlags[] =
  {
    {colourmenu_GENERATE_ABOUT_TO_BE_SHOWN , "colourmenu_GENERATE_ABOUT_TO_BE_SHOWN" },
    {colourmenu_GENERATE_DIALOGUE_COMPLETED, "colourmenu_GENERATE_DIALOGUE_COMPLETED"},
    {colourmenu_GENERATE_HAS_BEEN_HIDDEN   , "colourmenu_GENERATE_HAS_BEEN_HIDDEN"   },
    {colourmenu_NONE_ENTRY                 , "colourmenu_NONE_ENTRY"                 }
  };

static const FLAGS ColourMenuColours[] =
  {
    {colourmenu_TRANSPARENT, "colourmenu_TRANSPARENT"},
    {colourmenu_NONE       , "colourmenu_NONE"       }
  };

static const OBJECTLIST ColourMenuObjectList[] =
  {
    {iol_FLAGS, "colourmenu_flags:", offsetof(colourmenu_object, flags),  ColourMenuFlags,   ELEMENTS(ColourMenuFlags)    },
    {iol_MSG,   "title:",            offsetof(colourmenu_object, title),  "title_limit:",    offsetof(colourmenu_object, title_limit)},
    {iol_ENUM,  "colour:",           offsetof(colourmenu_object, colour), ColourMenuColours, ELEMENTS(ColourMenuColours)             }
  };


int colourmenu_t2g(DATA *data, const char *pszIn, toolbox_relocatable_object_base *object)
{
  put_objects(data, pszIn, 0, (char *)(object + 1), ColourMenuObjectList, ELEMENTS(ColourMenuObjectList));

  return sizeof(colourmenu_object);
}


void colourmenu_g2t(DATA *data, FILE *hf, const toolbox_resource_file_object_base *object, const TOOLBOXSMTABLE *strMsgTableP)
{
  get_objects(data, hf, strMsgTableP, (const char *)(object + 1), ColourMenuObjectList, ELEMENTS(ColourMenuObjectList), 1);
}
