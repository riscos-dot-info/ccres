/* Error.c
   $Id: Error.c,v 1.1 2003/12/08 23:59:58 joty Exp $

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

#include "ccres.h"

#include <stdio.h>
#include <string.h>


// return value is zero based index of keys passed in pszKeys
int question(PSTR pszKeys, bits nErr, PSTR pszFmt, ...)
{
	os_error err;
	va_list list;

	err.errnum = nErr;
	va_start(list, pszFmt);
	vsprintf(err.errmess, pszFmt, list);
	va_end(list);
	return(wimp_report_error_by_category(
				&err,
				wimp_ERROR_BOX_NO_PROMPT |
				wimp_ERROR_BOX_SHORT_TITLE |
				wimp_ERROR_BOX_GIVEN_CATEGORY |
				(wimp_ERROR_BOX_CATEGORY_QUESTION << wimp_ERROR_BOX_CATEGORY_SHIFT),
				achProgName,
				"!"APPNAME,
				(osspriteop_area *) 1,		// wimp pool
				pszKeys) - 3);				// ignore standard buttons
}

void toolbox_error(PDATA data)
{
	if (question("Continue,Quit", data->poll.ta.data.error.errnum,  data->poll.ta.data.error.errmess) == 1) {
		data->fRunning = FALSE;
	}
}


void oserr(os_error * err)
{
	wimp_report_error(err, wimp_ERROR_BOX_OK_ICON | wimp_ERROR_BOX_NO_PROMPT, achProgName);
}


void error(PSTR pszFmt, ...)
{
	os_error err;
	va_list list;

	err.errnum = 0;
	va_start(list, pszFmt);
	vsprintf(err.errmess, pszFmt, list);
	va_end(list);
	oserr(&err);
}


void errtitle(PSTR pszTitle, PSTR pszError)
{
	os_error err;

	err.errnum = 0;
	strncpy(err.errmess, pszError, sizeof(err.errmess) - 2);
	wimp_report_error(&err, (wimp_ERROR_BOX_OK_ICON | wimp_ERROR_BOX_NO_PROMPT | wimp_ERROR_BOX_SHORT_TITLE), pszTitle);
}


#ifdef DEBUG
void errnum(PSTR pszError, int num)
{
	os_error err;

	err.errnum = 0;
	sprintf(err.errmess, "%.32s (%d)", pszError, num);
	oserr(&err);
}
#endif
