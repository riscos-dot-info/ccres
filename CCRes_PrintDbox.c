/* CCRes_PrintDbox.c

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

#include <oslib/printdbox.h>

#include "CCRes_Internal.h"
#include "CCRes_Object.h"
#include "CCRes_PrintDbox.h"

static const FLAGS PrintDboxFlags[] =
  {
    {printdbox_GENERATE_ABOUT_TO_BE_SHOWN , "printdbox_GENERATE_ABOUT_TO_BE_SHOWN" },
    {printdbox_GENERATE_DIALOGUE_COMPLETED, "printdbox_GENERATE_DIALOGUE_COMPLETED"},
    {printdbox_GENERATE_SHOW_SETUP_ACTION , "printdbox_GENERATE_SHOW_SETUP_ACTION" },
    {printdbox_INCLUDE_ALL_FROM_TO        , "printdbox_INCLUDE_ALL_FROM_TO"        },
    {printdbox_INCLUDE_COPIES             , "printdbox_INCLUDE_COPIES"             },
    {printdbox_INCLUDE_SCALE              , "printdbox_INCLUDE_SCALE"              },
    {printdbox_INCLUDE_ORIENTATION        , "printdbox_INCLUDE_ORIENTATION"        },
    {printdbox_INCLUDE_SAVE               , "printdbox_INCLUDE_SAVE"               },
    {printdbox_INCLUDE_SET_UP             , "printdbox_INCLUDE_SET_UP"             },
    {printdbox_INCLUDE_DRAFT              , "printdbox_INCLUDE_DRAFT"              },
    {printdbox_SELECT_FROM_TO             , "printdbox_SELECT_FROM_TO"             },
    {printdbox_SELECT_SIDEWAYS            , "printdbox_SELECT_SIDEWAYS"            },
    {printdbox_SELECT_DRAFT               , "printdbox_SELECT_DRAFT"               }
  };

static const OBJECTLIST PrintDboxObjectList[] =
  {
    {iol_FLAGS,   "printdbox_flags:",         offsetof(printdbox_object, flags),                   PrintDboxFlags, ELEMENTS(PrintDboxFlags)    },
    {iol_INT,     "from:",                    offsetof(printdbox_object, from),                    NULL,           0                       },
    {iol_INT,     "to:",                      offsetof(printdbox_object, to),                      NULL,           0                       },
    {iol_INT,     "copies:",                  offsetof(printdbox_object, copies),                  NULL,           0                       },
    {iol_INT,     "scale:",                   offsetof(printdbox_object, scale),                   NULL,           0                       },
    {iol_STRING,  "further_options:",         offsetof(printdbox_object, further_options),         NULL,           0                       },
    {iol_STRING,  "alternative_window_name:", offsetof(printdbox_object, alternative_window_name), NULL,           0                       }
  };

int printdbox_t2g(DATA *data, const char *pszIn, toolbox_relocatable_object_base *object)
{
  put_objects(data, pszIn, 0, (char *)(object + 1), PrintDboxObjectList, ELEMENTS(PrintDboxObjectList));

  return sizeof(printdbox_object);
}


void printdbox_g2t(DATA *data, FILE *hf, const toolbox_resource_file_object_base *object, const TOOLBOXSMTABLE *strMsgTableP)
{
  get_objects(data, hf, strMsgTableP, (const char *)(object + 1), PrintDboxObjectList, ELEMENTS(PrintDboxObjectList), 1);
}
