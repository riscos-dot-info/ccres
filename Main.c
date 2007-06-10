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

// Std C headers :
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// OSLib headers :
#include <oslib/proginfo.h>
#include <oslib/saveas.h>
#include <oslib/taskwindow.h>
#include <oslib/wimpreadsysinfo.h>

// Project headers :
#include "Convert.h"
#include "Error.h"
#include "Filer.h"
#include "Main.h"
#include "Menu.h"
#include "Utils.h"
#include "SaveAs.h"

#define APPNAME	"CCres"
#define APPDIR	"<"APPNAME"$Dir>"

#define action_MENU_QUIT	0x01

typedef void (*action_handler)(APPDATA *data);

static bool ccres_appl_initialise(APPDATA *data);
static void ccres_appl_pollloop(APPDATA *data);
static void toolbox_error(APPDATA *data);
static int question(const char *pszKeys, bits nErr, const char *pszFmt, ...);

static void report_wimp(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, ...);
static void report_end_wimp(DATA *sessionP);

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

static  bool ccres_appl_initialise(APPDATA *data)
//      ======================================
{
data->idBaricon = toolbox_create_object(0, (const toolbox_id) "Iconbar");
data->idSaveAs  = toolbox_create_object(0, (const toolbox_id) "SaveAs");
proginfo_set_version(0, toolbox_create_object(0, (const toolbox_id) "ProgInfo"), VERSION);

return true;
}


static  void ccres_appl_pollloop(APPDATA *data)
//      ====================================
{
do {
  wimp_event_no e;
  if ((e = wimp_poll(wimp_MASK_NULL | wimp_MASK_LEAVING | wimp_MASK_ENTERING, &data->poll.wb, 0)) == toolbox_EVENT)
    {
    // is it a toolbox event?
    // if so, look-up the action number in the Action list, then call the associated handler
    bits nAction = data->poll.ta.action_no;
    for (unsigned int a = 0; a < ELEMENTS(Handler); a++)
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
    bits nAction;
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
  APPDATA data;

  memset(&data, 0, sizeof(APPDATA)); // FIXME: perhaps too crude ?
  if ((data.sessionP = ccres_initialise()) == NULL)
    {
      report_wimp(NULL, report_error, 0, "%s", "Failed to initialise ccres\n");
      return EXIT_FAILURE;
    }

  ccres_install_report_routine(data.sessionP, report_wimp, report_end_wimp);

  int nVersion;
  messagetrans_control_block cb;
  os_error *perr;
  if ((perr = xtoolbox_initialise(0, 310, Message, Action, APPDIR, &cb, &data.tb, &nVersion, &data.task, &data.pSprites)) != NULL)
    data.sessionP->report(data.sessionP, report_error, 0, "%s", perr->errmess);
  else
    {
    data.fRunning = ccres_appl_initialise(&data);
    ccres_appl_pollloop(&data);
    wimp_close_down(data.task);
    }

  (void)ccres_finish(data.sessionP);

  return EXIT_SUCCESS;
}


static  void toolbox_error(APPDATA *data)
//      ==============================
{
if (question("Continue,Quit", data->poll.ta.data.error.errnum, data->poll.ta.data.error.errmess) == 1)
  data->fRunning = false;
}

// return value is zero based index of keys passed in pszKeys
static  int question(const char *pszKeys, bits nErr, const char *pszFmt, ...)
//      =======================================================
{
os_error err;
va_list list;

err.errnum = nErr;
va_start(list, pszFmt);
vsnprintf(err.errmess, sizeof(err.errmess), pszFmt, list);
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

static void report_wimp(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, ...)
{
  va_list list;
  va_start(list, pszFmt);
  if (level == report_error)
    {
      os_error err;
      err.errnum = 0;
      vsnprintf(err.errmess, sizeof(err.errmess), pszFmt, list);
      wimp_report_error(&err, wimp_ERROR_BOX_OK_ICON | wimp_ERROR_BOX_NO_PROMPT, APPNAME);
    }

  if (sessionP != NULL)
    report_varg_throwback(sessionP, level, linenr, pszFmt, list);
  va_end(list);
}

static void report_end_wimp(DATA *sessionP)
{
  report_end_throwback(sessionP);
}
