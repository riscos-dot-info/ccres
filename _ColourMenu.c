/* _ColourMenu.c
   $Id: _ColourMenu.c,v 1.5 2005/01/30 16:04:57 joty Exp $

   Copyright (c) 2003-2005 Dave Appleby / John Tytgat

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

#include <stdio.h>
#include <string.h>

#include <OSLib/colourmenu.h>

#include "ccres.h"
#include "_ColourMenu.h"
#include "_Object.h"

static const FLAGS ColourMenuFlags[] = {
	{colourmenu_GENERATE_ABOUT_TO_BE_SHOWN , "colourmenu_GENERATE_ABOUT_TO_BE_SHOWN" },
	{colourmenu_GENERATE_DIALOGUE_COMPLETED, "colourmenu_GENERATE_DIALOGUE_COMPLETED"},
	{colourmenu_GENERATE_HAS_BEEN_HIDDEN   , "colourmenu_GENERATE_HAS_BEEN_HIDDEN"   },
	{colourmenu_NONE_ENTRY                 , "colourmenu_NONE_ENTRY"                 }
};

static const FLAGS ColourMenuColours[] = {
	{colourmenu_TRANSPARENT, "colourmenu_TRANSPARENT"},
	{colourmenu_NONE       , "colourmenu_NONE"       }
};

static const OBJECTLIST ColourMenuObjectList[] = {
	{iol_FLAGS, "colourmenu_flags:", offsetof(colourmenu_object, flags),  ColourMenuFlags,   ELEMENTS(ColourMenuFlags)               },
	{iol_MSG,   "title:",            offsetof(colourmenu_object, title),  "title_limit:",    offsetof(colourmenu_object, title_limit)},
	{iol_ENUM,  "colour:",           offsetof(colourmenu_object, colour), ColourMenuColours, ELEMENTS(ColourMenuColours)             }
};


        int colourmenu_t2g(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object)
//      ====================================================================================
{
put_objects(data, pszIn, 0, (PSTR) (object + 1), ColourMenuObjectList, ELEMENTS(ColourMenuObjectList));

return sizeof(colourmenu_object);
}


        void colourmenu_g2t(PDATA data, FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable)
//      =================================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) (object + 1), ColourMenuObjectList, ELEMENTS(ColourMenuObjectList), 1);
}
