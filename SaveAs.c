/* _SaveAs.c

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
#include <string.h>

#include <oslib/saveas.h>

#include "Convert.h"
#include "SaveAs.h"

void action_save_to_file(APPDATA *data)
{
  bits fSaved;

  fSaved = (ccres_convert(data->sessionP, data->poll.sa.file_name)) ? saveas_SAVE_SUCCESSFUL : 0;
  saveas_file_save_completed(fSaved, data->idSaveAs, data->poll.sa.file_name);
  if (fSaved == saveas_SAVE_SUCCESSFUL)
    toolbox_hide_object(0, data->idSaveAs);
}


void action_save_completed(APPDATA *data)
{
  if (data->poll.sa.flags & saveas_SAVE_SUCCESSFUL)
    remove(data->poll.sa.file_name);
}
