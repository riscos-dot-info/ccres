/* _Icon.c
   $Id: _Icon.c,v 1.6 2005/01/30 14:51:32 joty Exp $

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

#include <OSLib/button.h>

#include "ccres.h"
#include "_Object.h"
#include "_Icon.h"

// Res file button gadget

static const FLAGS ButtonFlags[] = {
  {button_TASK_SPRITE_AREA , "button_TASK_SPRITE_AREA" },
  {button_ALLOW_MENU_CLICKS, "button_ALLOW_MENU_CLICKS"},
  {gadget_FADED            , "gadget_FADED"            },
  {gadget_AT_BACK          , "gadget_AT_BACK"          }
};


static const OBJECTLIST ButtonObjectList[] = {
  {iol_FLAGS,  "flags:",        offsetof(button_object, flags),        ButtonFlags,         ELEMENTS(ButtonFlags)                    },
  {iol_IFLAGS, "button_flags:", offsetof(button_object, button_flags), NULL,                0                                        },
  {iol_ESG,    "button_esg:",   offsetof(button_object, button_flags), NULL,                0                                        },
  {iol_BCOLS,  "button_fg:",    offsetof(button_object, button_flags), "button_bg:",        offsetof(button_object, button_flags)    },
  {iol_MSG,    "value:",        offsetof(button_object, value),        "value_limit:",      offsetof(button_object, value_limit)     },
  {iol_STRING, "validation:",   offsetof(button_object, validation),   "validation_limit:", offsetof(button_object, validation_limit)}
  };

        int button_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, ButtonObjectList, ELEMENTS(ButtonObjectList));

return sizeof(button_object);
}


        void button_g2t(FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ==================================================================================================
{
get_objects(hf, pszStringTable, pszMessageTable, (PSTR) gadget, ButtonObjectList, ELEMENTS(ButtonObjectList), 2);
}


// Template file icon


static const OBJECTLIST IconObjectList[] = {
  {iol_BOX,    "extent:",      offsetof(wimp_icon, extent), NULL,          0                         },
  {iol_IFLAGS, "icon_flags:",  offsetof(wimp_icon, flags),  NULL,          0                         },
  {iol_ESG,    "icon_esg:",    offsetof(wimp_icon, flags),  NULL,          0                         }
  };

static const OBJECTLIST IconColorsObjectList[] = {
  {iol_BCOLS,  "icon_fg:",     offsetof(wimp_icon, flags),  "icon_bg:",    offsetof(wimp_icon, flags)}
  };

static const OBJECTLIST IconFontHandleObjectList[] = {
  {iol_BYTE,  "font_handle:", offsetof(wimp_icon, flags) + (wimp_ICON_FONT_HANDLE_SHIFT / 8),  "icon_bg:",    offsetof(wimp_icon, flags)}
  };

static const OBJECTLIST IconTextObjectList[] = {
  {iol_PSTR, "text_only:", offsetof(wimp_icon_data, text), NULL, sizeof(wimp_icon_data)}
  };

static const OBJECTLIST IconSpriteObjectList[] = {
  {iol_PSTR, "sprite_only:", offsetof(wimp_icon_data, sprite), NULL, sizeof(wimp_icon_data)}
  };

static const OBJECTLIST IconTextAndSpriteObjectList[] = {
  {iol_PSTR, "text_and_sprite:", offsetof(wimp_icon_data, text_and_sprite), NULL, sizeof(wimp_icon_data)}
  };

static const OBJECTLIST IconIndirectTextObjectList[] = {
  {iol_TSTRING, "text.text:",       offsetof(wimp_icon_data, indirected_text.text),       NULL, 0},
  {iol_TSTRING, "text.validation:", offsetof(wimp_icon_data, indirected_text.validation), NULL, 0},
  {iol_BITS,    "text.size:",       offsetof(wimp_icon_data, indirected_text.size),       NULL, 0}
  };

static const OBJECTLIST IconIndirectSpriteObjectList[] = {
  {iol_TSTRING, "sprite.id:",   offsetof(wimp_icon_data, indirected_sprite.id),   NULL, 0},
  {iol_BITS,    "sprite.area:", offsetof(wimp_icon_data, indirected_sprite.area), NULL, 0},
  {iol_BITS,    "sprite.size:", offsetof(wimp_icon_data, indirected_sprite.size), NULL, 0}
  };

static const OBJECTLIST IconIndirectTextAndSpriteObjectList[] = {
  {iol_TSTRING, "text_and_sprite.text",        offsetof(wimp_icon_data, indirected_text_and_sprite.text),       NULL, 0},
  {iol_TSTRING, "text_and_sprite.validation:", offsetof(wimp_icon_data, indirected_text_and_sprite.validation), NULL, 0},
  {iol_BITS,    "text_and_sprite.size:",       offsetof(wimp_icon_data, indirected_text_and_sprite.size),       NULL, 0}
  };


        void put_icon_data(PDATA data, PSTR pszIn, int nOffset, wimp_icon_data * icon_data, bits flags)
//      ===============================================================================================
{
switch (flags & (wimp_ICON_INDIRECTED | wimp_ICON_TEXT | wimp_ICON_SPRITE))
  {
  case 0:
  case wimp_ICON_INDIRECTED:
    break;

  case wimp_ICON_TEXT:
    put_objects(data, pszIn, nOffset, (PSTR) icon_data, IconTextObjectList, ELEMENTS(IconTextObjectList));
    break;
  case wimp_ICON_SPRITE:
    put_objects(data, pszIn, nOffset, (PSTR) icon_data, IconSpriteObjectList, ELEMENTS(IconSpriteObjectList));
    break;
  case (wimp_ICON_TEXT | wimp_ICON_SPRITE):
    put_objects(data, pszIn, nOffset, (PSTR) icon_data, IconTextAndSpriteObjectList, ELEMENTS(IconTextAndSpriteObjectList));
    break;

  case (wimp_ICON_INDIRECTED | wimp_ICON_TEXT):
    put_objects(data, pszIn, nOffset, (PSTR) icon_data, IconIndirectTextObjectList, ELEMENTS(IconIndirectTextObjectList));
    break;
  case (wimp_ICON_INDIRECTED | wimp_ICON_SPRITE):
    put_objects(data, pszIn, nOffset, (PSTR) icon_data, IconIndirectSpriteObjectList, ELEMENTS(IconIndirectSpriteObjectList));
    break;
  case (wimp_ICON_INDIRECTED | wimp_ICON_TEXT | wimp_ICON_SPRITE):
    put_objects(data, pszIn, nOffset, (PSTR) icon_data, IconIndirectTextAndSpriteObjectList, ELEMENTS(IconIndirectTextAndSpriteObjectList));
    break;
  }
}


        void get_icon_data(FILE * hf, PSTR pszStringTable, wimp_icon_data * icon_data, bits flags, int nIndent)
//      =======================================================================================================
{
switch (flags & (wimp_ICON_INDIRECTED | wimp_ICON_TEXT | wimp_ICON_SPRITE))
  {
  case 0:
  case wimp_ICON_INDIRECTED:
    break;

  case wimp_ICON_TEXT:
    get_objects(hf, pszStringTable, NULL, (PSTR) icon_data, IconTextObjectList, ELEMENTS(IconTextObjectList), nIndent);
    break;
  case wimp_ICON_SPRITE:
    get_objects(hf, pszStringTable, NULL, (PSTR) icon_data, IconSpriteObjectList, ELEMENTS(IconSpriteObjectList), nIndent);
    break;
  case (wimp_ICON_TEXT | wimp_ICON_SPRITE):
    get_objects(hf, pszStringTable, NULL, (PSTR) icon_data, IconTextAndSpriteObjectList, ELEMENTS(IconTextAndSpriteObjectList), nIndent);
    break;

  case (wimp_ICON_INDIRECTED | wimp_ICON_TEXT):
    get_objects(hf, pszStringTable, NULL, (PSTR) icon_data, IconIndirectTextObjectList, ELEMENTS(IconIndirectTextObjectList), nIndent);
    break;
  case (wimp_ICON_INDIRECTED | wimp_ICON_SPRITE):
    get_objects(hf, pszStringTable, NULL, (PSTR) icon_data, IconIndirectSpriteObjectList, ELEMENTS(IconIndirectSpriteObjectList), nIndent);
    break;
  case (wimp_ICON_INDIRECTED | wimp_ICON_TEXT | wimp_ICON_SPRITE):
    get_objects(hf, pszStringTable, NULL, (PSTR) icon_data, IconIndirectTextAndSpriteObjectList, ELEMENTS(IconIndirectTextAndSpriteObjectList), nIndent);
    break;
  }
}


        void icon_text2template(PDATA data, PSTR pszIn, int nOffset, wimp_icon * icon)
//      ==============================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) icon, IconObjectList, ELEMENTS(IconObjectList));
if (icon->flags & wimp_ICON_ANTI_ALIASED)
  put_objects(data, pszIn, nOffset, (PSTR) icon, IconFontHandleObjectList, ELEMENTS(IconFontHandleObjectList));
else
  put_objects(data, pszIn, nOffset, (PSTR) icon, IconColorsObjectList, ELEMENTS(IconColorsObjectList));

put_icon_data(data, pszIn, nOffset, (wimp_icon_data *) &icon->data, icon->flags);
}


        void icon_template2text(FILE * hf, PSTR pszStringTable, wimp_icon * icon)
//      =========================================================================
{
get_objects(hf, pszStringTable, NULL, (PSTR) icon, IconObjectList, ELEMENTS(IconObjectList), 2);
if (icon->flags & wimp_ICON_ANTI_ALIASED)
  get_objects(hf, pszStringTable, NULL, (PSTR) icon, IconFontHandleObjectList, ELEMENTS(IconFontHandleObjectList), 2);
else
  get_objects(hf, pszStringTable, NULL, (PSTR) icon, IconColorsObjectList, ELEMENTS(IconColorsObjectList), 2);

get_icon_data(hf, pszStringTable, (wimp_icon_data *) &icon->data, icon->flags, 2);
}
