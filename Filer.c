/* Filer.c

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

#include "ccres.h"
#include "Error.h"
#include "Filer.h"
#include "Utils.h"

static const char achScrapFile[] = "<Wimp$Scrap>";

// user has dragged something to us from another app...
void message_data_save(DATA *data)
{
	wimp_message *msg;

	msg = &data->poll.wb.message;
	msg->your_ref = msg->my_ref;
	msg->action = message_DATA_SAVE_ACK;
	msg->data.data_xfer.est_size = -1;
	strcpy(msg->data.data_xfer.file_name, achScrapFile);
	msg->size = (offsetof(wimp_message, data.data_xfer.file_name) + sizeof(achScrapFile) + 3) & ~3;
	wimp_send_message(wimp_USER_MESSAGE, msg, msg->sender);
	data->fUnsafeLoad = true;
}


// user has dragged something to us from a filer window...
void message_data_load(DATA *data)
{
	wimp_message *msg;
	bits file_type;

	msg = &data->poll.wb.message;
	if ((file_type = msg->data.data_xfer.file_type) == osfile_TYPE_TEXT
	    || file_type == osfile_TYPE_RESOURCE
	    || file_type == osfile_TYPE_TEMPLATE) {
		if (load_file(data, msg->data.data_xfer.file_name, file_type)) {
			saveas_set_file_type(0, data->idSaveAs, data->nFiletypeOut);
			saveas_set_file_size(0, data->idSaveAs, -1);
			toolbox_show_object(0, data->idSaveAs, toolbox_POSITION_AT_POINTER, NULL, data->idBaricon, toolbox_NULL_COMPONENT);
		}
	} else {
		error(data, "Filetype must be Text (fff), Resource (fae) or Template (fec)");
	}
	if (data->fUnsafeLoad) {
		remove(achScrapFile);
		data->fUnsafeLoad = false;
	}
	msg = &data->poll.wb.message;
	msg->your_ref = msg->my_ref;
	msg->action = message_DATA_LOAD_ACK;
	wimp_send_message(wimp_USER_MESSAGE, msg, msg->sender);
}
