/* _Iconbar.c
   $Id: $

   Copyright (c) 2003 Dave Appleby / John Tytgat

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

#include <OSLib/iconbar.h>


static FLAGS IconbarFlags[] = {
	{iconbar_GENERATE_SELECT_ABOUT_TO_BE_SHOWN, "iconbar_GENERATE_SELECT_ABOUT_TO_BE_SHOWN"},
	{iconbar_GENERATE_ADJUST_ABOUT_TO_BE_SHOWN, "iconbar_GENERATE_ADJUST_ABOUT_TO_BE_SHOWN"},
	{iconbar_SELECT_IS_MENU                   , "iconbar_SELECT_IS_MENU"                   },
	{iconbar_ADJUST_IS_MENU                   , "iconbar_ADJUST_IS_MENU"                   },
	{iconbar_HAS_TEXT                         , "iconbar_HAS_TEXT"                         },
	{iconbar_GENERATE_SELECT_CLICKED          , "iconbar_GENERATE_SELECT_CLICKED"          },
	{iconbar_GENERATE_ADJUST_CLICKED          , "iconbar_GENERATE_ADJUST_CLICKED"          }
};

static FLAGS IconbarPosition[] = {
	{(int) wimp_ICON_BAR_RIGHT              , "wimp_ICON_BAR_RIGHT"              },
	{(int) wimp_ICON_BAR_LEFT               , "wimp_ICON_BAR_LEFT"               },
	{(int) wimp_ICON_BAR_LEFT_RELATIVE      , "wimp_ICON_BAR_LEFT_RELATIVE"      },
	{(int) wimp_ICON_BAR_RIGHT_RELATIVE     , "wimp_ICON_BAR_RIGHT_RELATIVE"     },
	{(int) wimp_ICON_BAR_LEFT_HIGH_PRIORITY , "wimp_ICON_BAR_LEFT_HIGH_PRIORITY" },
	{(int) wimp_ICON_BAR_LEFT_LOW_PRIORITY  , "wimp_ICON_BAR_LEFT_LOW_PRIORITY"  },
	{(int) wimp_ICON_BAR_RIGHT_LOW_PRIORITY , "wimp_ICON_BAR_RIGHT_LOW_PRIORITY" },
	{(int) wimp_ICON_BAR_RIGHT_HIGH_PRIORITY, "wimp_ICON_BAR_RIGHT_HIGH_PRIORITY"}
};


static OBJECTLIST IconbarObjectList[] = {
	{iol_FLAGS,  "iconbar_flags:", offsetof(iconbar_object, flags),         IconbarFlags,    ELEMENTS(IconbarFlags)                },
	{iol_ENUM,   "position:",      offsetof(iconbar_object, position),      IconbarPosition, ELEMENTS(IconbarPosition)             },
	{iol_INT,    "priority:",      offsetof(iconbar_object, priority),      NULL,            0                                     },
	{iol_STRING, "sprite_name:",   offsetof(iconbar_object, sprite_name),   "sprite_limit:", offsetof(iconbar_object, sprite_limit)},
	{iol_MSG,    "text:",          offsetof(iconbar_object, text),          "text_limit:",   offsetof(iconbar_object, text_limit)  },
	{iol_STRING, "menu:",          offsetof(iconbar_object, menu),          NULL,            0                                     },
	{iol_BITS,   "select_action:", offsetof(iconbar_object, select_action), NULL,            bits_ACTION                           },
	{iol_BITS,   "adjust_action:", offsetof(iconbar_object, adjust_action), NULL,            bits_ACTION                           },
	{iol_STRING, "select_show:",   offsetof(iconbar_object, select_show),   NULL,            0                                     },
	{iol_STRING, "adjust_show:",   offsetof(iconbar_object, adjust_show),   NULL,            0                                     },
	{iol_MSG,    "help_message:",  offsetof(iconbar_object, help_message),  "help_limit:",   offsetof(iconbar_object, help_limit)  }
};


int _iconbar(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object)
{
	put_objects(data, pszIn, 0, (PSTR) (object + 1), IconbarObjectList, ELEMENTS(IconbarObjectList));

	return(sizeof(iconbar_object));
}


void iconbar(FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable)
{
	get_objects(hf, pszStringTable, pszMessageTable, (PSTR) (object + 1), IconbarObjectList, ELEMENTS(IconbarObjectList), 1);
}
