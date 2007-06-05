/* _Menu.c

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

#include <stdio.h>

#include <oslib/menu.h>

#include "ccres.h"
#include "_Object.h"
#include "_Menu.h"

static const FLAGS MenuFlags[] = {
	{menu_GENERATE_ABOUT_TO_BE_SHOWN , "menu_GENERATE_ABOUT_TO_BE_SHOWN"  },
	{menu_GENERATE_DIALOGUE_COMPLETED, "menu_GENERATE_DIALOGUE_COMPLETED" },
	{menu_GENERATE_HAS_BEEN_HIDDEN,    "menu_GENERATE_HAS_BEEN_HIDDEN"    }
};

static const OBJECTLIST MenuObjectList[] = {
	{iol_FLAGS, "menu_flags:",  offsetof(menu_object, flags),       MenuFlags,       ELEMENTS(MenuFlags)               },
	{iol_MSG,   "title:",       offsetof(menu_object, title),       "title_limit:",  offsetof(menu_object, title_limit)},
	{iol_MSG,   "help:",        offsetof(menu_object, help),        "help_limit:",   offsetof(menu_object, help_limit) },
	{iol_BITS,  "show_action:", offsetof(menu_object, show_action), NULL,            bits_ACTION                       },
	{iol_BITS,  "hide_action:", offsetof(menu_object, hide_action), NULL,            bits_ACTION                       }
};

static const FLAGS MenuEntryFlags[] = {
	{menu_ENTRY_TICKED                  , "menu_ENTRY_TICKED"                  },
	{menu_ENTRY_SEPARATE                , "menu_ENTRY_SEPARATE"                },
	{menu_ENTRY_FADED                   , "menu_ENTRY_FADED"                   },
	{menu_ENTRY_IS_SPRITE               , "menu_ENTRY_IS_SPRITE"               },
	{menu_ENTRY_SUB_MENU                , "menu_ENTRY_SUB_MENU"                },
	{menu_ENTRY_GENERATE_SUB_MENU_ACTION, "menu_ENTRY_GENERATE_SUB_MENU_ACTION"},
	{menu_ENTRY_IS_MENU                 , "menu_ENTRY_IS_MENU"                 }
};

// MenuEntry must be split into two to cope with the text entry, which is STRING for text and MSG for sprites-
// So when converting text to res, we read the flags, set iol_STRING or iol_MSG as appropriate, then do the rest...
static const OBJECTLIST MenuEntryObjectListFlags[] = {
	{iol_FLAGS, "flags:",                offsetof(menu_entry_object, flags),                MenuEntryFlags, ELEMENTS(MenuEntryFlags)               },
	{iol_BITS,  "cmp:",                  offsetof(menu_entry_object, cmp),                  NULL,           bits_EVAL                              }
};
// This is *not* a const object !
static OBJECTLIST MenuEntryObjectList[] = {
	{iol_MSG,   "text:",                 offsetof(menu_entry_object, text),                 "text_limit:",  offsetof(menu_entry_object, text_limit)},
	{iol_STRING,"click_object_name:",    offsetof(menu_entry_object, click_object_name),    NULL,           0                                      },
	{iol_STRING,"sub_menu_object_name:", offsetof(menu_entry_object, sub_menu_object_name), NULL,           0                                      },
	{iol_BITS,  "sub_menu_action:",      offsetof(menu_entry_object, sub_menu_action),      NULL,           bits_ACTION                            },
	{iol_BITS,  "click_action:",         offsetof(menu_entry_object, click_action),         NULL,           bits_ACTION                            },
	{iol_MSG,   "help:",                 offsetof(menu_entry_object, help),                 "help_limit:",  offsetof(menu_entry_object, help_limit)}
};


int menu_t2g(DATA *data, char *pszIn, toolbox_relocatable_object_base *object)
{
	menu_object_base * menu;
	menu_entry_object * entry;
	char *pszEnd;
	int entry_count;

	menu = (menu_object_base *) (object + 1);
	put_objects(data, pszIn, 0, (char *) menu, MenuObjectList, ELEMENTS(MenuObjectList));

	pszEnd = data->pszIn + data->cbIn;
	entry = (menu_entry_object *) (menu + 1);
	entry_count = 0;
	while (next_object(&pszIn, pszEnd) != NULL) {
		put_objects(data, pszIn, (int) ((char *) entry - (char *) menu), (char *) entry, MenuEntryObjectListFlags, ELEMENTS(MenuEntryObjectListFlags));
		MenuEntryObjectList[0].nTable = (entry->flags & menu_ENTRY_IS_SPRITE) ? iol_STRING : iol_MSG;		// text or sprite?
		put_objects(data, pszIn, (int) ((char *) entry - (char *) menu), (char *) entry, MenuEntryObjectList, ELEMENTS(MenuEntryObjectList));
		entry++;
		entry_count++;
		if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL) {
			break;
		}
	}
	menu->entry_count = entry_count;
	return (int) ((char *) entry - (char *) menu);
}


        void menu_g2t(DATA *data, FILE *hf, toolbox_resource_file_object_base *object, const TOOLBOXSMTABLE *strMsgTableP)
//      ===========================================================================================================================
{
menu_object_base * menu;
menu_entry_object * entry;
int n;

menu = (menu_object_base *) (object + 1);
get_objects(data, hf, strMsgTableP, (const char *)menu, MenuObjectList, ELEMENTS(MenuObjectList), 1);

for (n = 0, entry = (menu_entry_object *) (menu + 1); n < menu->entry_count; n++, entry++)
  {
  fprintf(hf, "  Entry {\n    cmp:%d\n", (int) entry->cmp);
  MenuEntryObjectList[0].nTable = (entry->flags & menu_ENTRY_IS_SPRITE) ? iol_STRING : iol_MSG;		// text or sprite?
  get_objects(data, hf, strMsgTableP, (const char *)entry, MenuEntryObjectListFlags, ELEMENTS(MenuEntryObjectListFlags), 2);
  get_objects(data, hf, strMsgTableP, (const char *)entry, MenuEntryObjectList, ELEMENTS(MenuEntryObjectList), 2);
  fputs("  }\n", hf);
  }
}
