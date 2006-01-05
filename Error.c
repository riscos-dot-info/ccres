/* Error.c
   $Id: Error.c,v 1.3 2005/01/30 14:47:08 joty Exp $

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

#include <OSLib/ddeutils.h>

#include "ccres.h"
#include "Error.h"

void report(PDATA data, const char *ptrP, PSTR pszFmt, ...)
{
	va_list list;
	const char *pP;
	int nRow;
	char achError[1024];

	va_start(list, pszFmt);
	vsprintf(achError, pszFmt, list);
	va_end(list);
	for (nRow = 1, pP = data->pszIn; pP < ptrP; pP++) {
		if (*pP < ' ') {
			nRow++;
		}
	}
	if (!data->fThrowback) {
		ddeutils_throwback_start();
		data->fThrowback = TRUE;
	}
	ddeutils_throwback_send(ddeutils_THROWBACK_INFO_DETAILS, data->achTextFile, nRow, ddeutils_SEVERITY_ERROR, achError);
}


void report_end(PDATA data)
{
ddeutils_throwback_end();
}


void error(DATA *sessionP, PSTR pszFmt, ...)
{
os_error err;
va_list list;

err.errnum = 0;
va_start(list, pszFmt);
vsprintf(err.errmess, pszFmt, list);
va_end(list);
wimp_report_error(&err, wimp_ERROR_BOX_OK_ICON | wimp_ERROR_BOX_NO_PROMPT, APPNAME);
sessionP->returnStatus = EXIT_FAILURE;
}
