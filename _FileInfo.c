/* _FileInfo.c
   $Id: _FileInfo.c,v 1.4 2005/01/30 14:49:29 joty Exp $

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

#include <OSLib/fileinfo.h>

#include "ccres.h"
#include "_Object.h"
#include "_FileInfo.h"

static const FLAGS FileInfoFlags[] = {
	{fileinfo_GENERATE_ABOUT_TO_BE_SHOWN , "fileinfo_GENERATE_ABOUT_TO_BE_SHOWN" },
	{fileinfo_GENERATE_DIALOGUE_COMPLETED, "fileinfo_GENERATE_DIALOGUE_COMPLETED"}
};

static const OBJECTLIST FileInfoObjectList[] = {
	{iol_FLAGS,  "fileinfo_flags:",          offsetof(fileinfo_object, flags),                   FileInfoFlags,  ELEMENTS(FileInfoFlags)               },
	{iol_MSG,    "title:",                   offsetof(fileinfo_object, title),                   "title_limit:", offsetof(fileinfo_object, title_limit)},
	{iol_BITS,   "file_type:",               offsetof(fileinfo_object, file_type),               NULL,           0                                     },
	{iol_MSG,    "file_name:",               offsetof(fileinfo_object, file_name),               NULL,           0                                     },
	{iol_STRING, "alternative_window_name:", offsetof(fileinfo_object, alternative_window_name), NULL,           0                                     }
};


        int fileinfo_t2g(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object)
//      ==================================================================================
{
put_objects(data, pszIn, 0, (PSTR) (object + 1), FileInfoObjectList, ELEMENTS(FileInfoObjectList));

return sizeof(fileinfo_object);
}


        void fileinfo_g2t(FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable)
//      ===================================================================================================================
{
get_objects(hf, pszStringTable, pszMessageTable, (PSTR) (object + 1), FileInfoObjectList, ELEMENTS(FileInfoObjectList), 1);
}
