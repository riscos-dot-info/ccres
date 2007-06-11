/* CCres_SaveAs.c

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

#include <oslib/saveas.h>

#include "CCres_Internal.h"
#include "CCres_Object.h"
#include "CCres_SaveAs.h"

static const FLAGS SaveAsFlags[] =
  {
    {saveas_GENERATE_ABOUT_TO_BE_SHOWN  , "saveas_GENERATE_ABOUT_TO_BE_SHOWN"  },
    {saveas_GENERATE_DIALOGUE_COMPLETED , "saveas_GENERATE_DIALOGUE_COMPLETED" },
    {saveas_NO_SELECTION_BUTTON         , "saveas_NO_SELECTION_BUTTON"         },
    {saveas_GIVEN_DATA                  , "saveas_GIVEN_DATA"                  },
    {saveas_CLIENT_SUPPORTS_RAM_TRANSFER, "saveas_CLIENT_SUPPORTS_RAM_TRANSFER"}
  };

static const OBJECTLIST SaveAsObjectList[] =
  {
    {iol_FLAGS,  "saveas_flags:",            offsetof(saveas_object, flags),                   SaveAsFlags,    ELEMENTS(SaveAsFlags)    },
    {iol_MSG,    "file_name:",               offsetof(saveas_object, file_name),               NULL,           0                                  },
    {iol_BITS,   "file_type:",               offsetof(saveas_object, file_type),               NULL,           0                                  },
    {iol_MSG,    "title:",                   offsetof(saveas_object, title),                   "title_limit:", offsetof(saveas_object,title_limit)},
    {iol_STRING, "alternative_window_name:", offsetof(saveas_object, alternative_window_name), NULL,           0                                  }
  };

int saveas_t2g(DATA *data, const char *pszIn, toolbox_relocatable_object_base *object)
{
  put_objects(data, pszIn, 0, (char *)(object + 1), SaveAsObjectList, ELEMENTS(SaveAsObjectList));

  return sizeof(saveas_object);
}


void saveas_g2t(DATA *data, FILE *hf, const toolbox_resource_file_object_base *object, const TOOLBOXSMTABLE *strMsgTableP)
{
  get_objects(data, hf, strMsgTableP, (const char *)(object + 1), SaveAsObjectList, ELEMENTS(SaveAsObjectList), 1);
}
