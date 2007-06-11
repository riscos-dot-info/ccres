/* CCres_Scale.c

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

#include <oslib/scale.h>

#include "CCres_Internal.h"
#include "CCres_Object.h"
#include "CCres_Scale.h"

static const FLAGS ScaleFlags[] =
  {
    {scale_GENERATE_ABOUT_TO_BE_SHOWN , "scale_GENERATE_ABOUT_TO_BE_SHOWN" },
    {scale_GENERATE_DIALOGUE_COMPLETED, "scale_GENERATE_DIALOGUE_COMPLETED"},
    {scale_INCLUDE_SCALE_TO_FIT       , "scale_INCLUDE_SCALE_TO_FIT"       }
  };

static const OBJECTLIST ScaleObjectList[] =
  {
    {iol_FLAGS,   "scale_flags:",             offsetof(scale_object, flags),                   ScaleFlags,     ELEMENTS(ScaleFlags)    },
    {iol_INT,     "lower_bound:",             offsetof(scale_object, lower_bound),             NULL,           0                                  },
    {iol_INT,     "upper_bound:",             offsetof(scale_object, upper_bound),             NULL,           0                                  },
    {iol_INT,     "step_size:",               offsetof(scale_object, step_size),               NULL,           0                                  },
    {iol_MSG,     "title:",                   offsetof(scale_object, title),                   "title_limit:", offsetof(scale_object, title_limit)},
    {iol_STRING,  "alternative_window_name:", offsetof(scale_object, alternative_window_name), NULL,           0                                  },
    {iol_BOX,     "std_value:",               offsetof(scale_object, std_value),               NULL,           0                                  }
  };

int scale_t2g(DATA *data, const char *pszIn, toolbox_relocatable_object_base *object)
{
  put_objects(data, pszIn, 0, (char *)(object + 1), ScaleObjectList, ELEMENTS(ScaleObjectList));

  return sizeof(scale_object);
}


void scale_g2t(DATA *data, FILE *hf, const toolbox_resource_file_object_base *object, const TOOLBOXSMTABLE *strMsgTableP)
{
  get_objects(data, hf, strMsgTableP, (const char *)(object + 1), ScaleObjectList, ELEMENTS(ScaleObjectList), 1);
}
