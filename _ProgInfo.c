/* _ProgInfo.c
   $Id: _ProgInfo.c,v 1.2 2004/03/20 22:13:58 joty Exp $

   Copyright (c) 2003-2004 Dave Appleby / John Tytgat

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

#include <OSLib/proginfo.h>

#include "ccres.h"

static FLAGS ProgInfoLicenceType[] = {
	{~0                             , "None"},
	{proginfo_LICENCE_PUBLIC_DOMAIN , "proginfo_LICENCE_PUBLIC_DOMAIN" },
	{proginfo_LICENCE_SINGLE_USER   , "proginfo_LICENCE_SINGLE_USER"   },
	{proginfo_LICENCE_SINGLE_MACHINE, "proginfo_LICENCE_SINGLE_MACHINE"},
	{proginfo_LICENCE_SITE          , "proginfo_LICENCE_SITE"          },
	{proginfo_LICENCE_NETWORK       , "proginfo_LICENCE_NETWORK"       },
	{proginfo_LICENCE_AUTHORITY     , "proginfo_LICENCE_AUTHORITY"     }
};

static FLAGS ProgInfoFlags[] = {
	{proginfo_GENERATE_ABOUT_TO_BE_SHOWN , "proginfo_GENERATE_ABOUT_TO_BE_SHOWN" },
	{proginfo_GENERATE_DIALOGUE_COMPLETED, "proginfo_GENERATE_DIALOGUE_COMPLETED"},
	{proginfo_INCLUDE_LICENCE_TYPE       , "proginfo_INCLUDE_LICENCE_TYPE"       },
	{proginfo_INCLUDE_WEB_PAGE_BUTTON    , "proginfo_INCLUDE_WEB_PAGE_BUTTON"    },
	{proginfo_GENERATE_WEB_PAGE_CLICKED  , "proginfo_GENERATE_WEB_PAGE_CLICKED"  }
};

static OBJECTLIST ProgInfoObjectList[] = {
	{iol_FLAGS,   "proginfo_flags:",          offsetof(proginfo_object, flags),                   ProgInfoFlags,       ELEMENTS(ProgInfoFlags)                      },
	{iol_MSG,     "title:",                   offsetof(proginfo_object, title),                   "title_limit:",      offsetof(proginfo_object, title_limit)       },
	{iol_MSG,     "purpose:",                 offsetof(proginfo_object, purpose),                 NULL,                0                                            },
	{iol_MSG,     "author:",                  offsetof(proginfo_object, author),                  NULL,                0                                            },
	{iol_ENUM,    "licence_type:",            offsetof(proginfo_object, licence_type),            ProgInfoLicenceType, ELEMENTS(ProgInfoLicenceType)                },
	{iol_MSG,     "prog_version:",            offsetof(proginfo_object, version),                 NULL,                0                                            },
	{iol_STRING,  "alternative_window_name:", offsetof(proginfo_object, alternative_window_name), NULL,                0                                            }
};

// FIXME: official OSLib names don't match CCres names
static OBJECTLIST ProgInfoObjectList101[] = {
	{iol_MSG,     "visit_url:",               offsetof(proginfo_object, uri),            NULL,                0},
	{iol_BITS,    "visit_url_event:",         offsetof(proginfo_object, uri_action),      NULL,                0}
};


int proginfo_t2g(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object)
{
	put_objects(data, pszIn, 0, (PSTR) (object + 1), ProgInfoObjectList, ELEMENTS(ProgInfoObjectList));
	if (object->rf_obj.version == 101) {
		put_objects(data, pszIn, 0, (PSTR) (object + 1), ProgInfoObjectList101, ELEMENTS(ProgInfoObjectList101));
		return sizeof(proginfo_object);
	}
	return offsetof(proginfo_object, uri);
}


void proginfo_g2t(FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable)
{
	get_objects(hf, pszStringTable, pszMessageTable, (PSTR) (object + 1), ProgInfoObjectList, ELEMENTS(ProgInfoObjectList), 1);
	if (object->version == 101) {
		get_objects(hf, pszStringTable, pszMessageTable, (PSTR) (object + 1), ProgInfoObjectList101, ELEMENTS(ProgInfoObjectList101), 1);
	}
}
