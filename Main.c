/* Main.c

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

/* Std C headers :
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* OSLib headers :
 */
#include <oslib/proginfo.h>
#include <oslib/saveas.h>
#include <oslib/taskwindow.h>
#include <oslib/wimpreadsysinfo.h>

/* Project headers :
 */
#include "ccres.h"
#include "Convert.h"
#include "Error.h"
#include "Utils.h"

#define APPDIR	"<"APPNAME"$Dir>"

#define action_MENU_QUIT	0x01

static bool ccres_appl_initialise(DATA *data);
static void ccres_appl_pollloop(DATA *data);
static void toolbox_error(DATA *data);
static int question(char *pszKeys, bits nErr, char *pszFmt, ...);


// Toolbox action codes returned by WimpPoll - each action has an associated handler
static const toolbox_action_list Action[] =
  {
  {{action_MENU_QUIT}},
  {{action_SAVE_AS_SAVE_TO_FILE}},
  {{action_SAVE_AS_SAVE_COMPLETED}},
  {{action_ERROR}},
  {{0}}
  };

// List of handler functions for the Toolbox actions - each handler corresponds to the action in the list above
static const action_handler Handler[] =
  {
  menu_quit,
  action_save_to_file,
  action_save_completed,
  toolbox_error
  };

// Wimp messages (except message_QUIT) that we want to receive
static const wimp_message_list Message[] =
  {
  {{message_DATA_SAVE}},
  {{message_DATA_LOAD}},
  {{0}}
  };

static  bool ccres_appl_initialise(DATA *data)
//      ======================================
{
data->idBaricon = toolbox_create_object(0, (toolbox_id) "Iconbar");
data->idSaveAs  = toolbox_create_object(0, (toolbox_id) "SaveAs");
proginfo_set_version(0, toolbox_create_object(0, (toolbox_id) "ProgInfo"), VERSION);

return true;
}


static  void ccres_appl_pollloop(DATA *data)
//      ====================================
{
  wimp_event_no e;
  int a;
  bits nAction;

do {
  if ((e = wimp_poll(wimp_MASK_NULL | wimp_MASK_LEAVING | wimp_MASK_ENTERING, &data->poll.wb, 0)) == toolbox_EVENT)
    {
    // is it a toolbox event?
    // if so, look-up the action number in the Action list, then call the associated handler
    nAction = data->poll.ta.action_no;
    for (a = 0; a < ELEMENTS(Handler); a++)
      {
      if (nAction == Action[a].action_nos[0])
        {
        Handler[a](data);
        break;
        }
      }
    }
  else if (e == wimp_USER_MESSAGE || e == wimp_USER_MESSAGE_RECORDED)
    {
    if ((nAction = data->poll.wb.message.action) == message_QUIT || nAction == message_SHUTDOWN)
      data->fRunning = false;
    else if (nAction == message_DATA_SAVE)
      message_data_save(data);
    else if (nAction == message_DATA_LOAD)
      message_data_load(data);
    }
  } while (data->fRunning);
}


        int main(int argc, char *argv[])
//      ===============================
{
DATA data;
if (ccres_initialise(&data))
  {
  int nVersion;
  messagetrans_control_block cb;
  os_error * perr;
  if ((perr = xtoolbox_initialise(0, 310, Message, Action, APPDIR, &cb, &data.tb, &nVersion, &data.task, &data.pSprites)) != NULL)
    error(&data, "%s", perr->errmess);
  else
    {
    data.fRunning = ccres_appl_initialise(&data);
    ccres_appl_pollloop(&data);
    wimp_close_down(data.task);
    }

  if (data.pszIn != NULL)
    MyFree(data.pszIn);
  (void)ccres_finish();
  }

return data.returnStatus;
}


static  void toolbox_error(DATA *data)
//      ==============================
{
if (question("Continue,Quit", data->poll.ta.data.error.errnum, data->poll.ta.data.error.errmess) == 1)
  {
  data->fRunning = false;
  data->returnStatus = EXIT_FAILURE;
  }
}

// return value is zero based index of keys passed in pszKeys
static  int question(char *pszKeys, bits nErr, char *pszFmt, ...)
//      =======================================================
{
os_error err;
va_list list;

err.errnum = nErr;
va_start(list, pszFmt);
vsprintf(err.errmess, pszFmt, list);
va_end(list);
return wimp_report_error_by_category(
	&err,
	wimp_ERROR_BOX_NO_PROMPT |
	wimp_ERROR_BOX_SHORT_TITLE |
	wimp_ERROR_BOX_GIVEN_CATEGORY |
	(wimp_ERROR_BOX_CATEGORY_QUESTION << wimp_ERROR_BOX_CATEGORY_SHIFT),
	APPNAME,
	"!"APPNAME,
	(osspriteop_area *) 1,		// wimp pool
	pszKeys) - 3;			// ignore standard buttons
}
