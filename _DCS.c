/* _DCS.c
   $Id: _DCS.c,v 1.5 2005/01/30 16:04:56 joty Exp $

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

#include <OSLib/dcs.h>

#include "ccres.h"
#include "_DCS.h"
#include "_Object.h"

static const FLAGS DCSFlags[] = {
	{dcs_GENERATE_ABOUT_TO_BE_SHOWN , "dcs_GENERATE_ABOUT_TO_BE_SHOWN" },
	{dcs_GENERATE_DIALOGUE_COMPLETED, "dcs_GENERATE_DIALOGUE_COMPLETED"}
};

static const OBJECTLIST DCSObjectList[] = {
	{iol_FLAGS,  "dcs_flags:",               offsetof(dcs_object, flags),                   DCSFlags,         ELEMENTS(DCSFlags)                 },
	{iol_MSG,    "title:",                   offsetof(dcs_object, title),                   "title_limit:",   offsetof(dcs_object, title_limit)  },
	{iol_MSG,    "message:",                 offsetof(dcs_object, message),                 "message_limit:", offsetof(dcs_object, message_limit)},
	{iol_STRING, "alternative_window_name:", offsetof(dcs_object, alternative_window_name), NULL,             0                                  }
};


        int dcs_t2g(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object)
//      =============================================================================
{
put_objects(data, pszIn, 0, (PSTR) (object + 1), DCSObjectList, ELEMENTS(DCSObjectList));

return sizeof(dcs_object);
}


        void dcs_g2t(PDATA data, FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable)
//      ==========================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) (object + 1), DCSObjectList, ELEMENTS(DCSObjectList), 1);
}
