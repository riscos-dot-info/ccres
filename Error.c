/* Error.c

   Copyright (c) 2003-2006 Dave Appleby / John Tytgat

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

#include <oslib/ddeutils.h>

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
#ifdef __riscos__
//FIXME:		ddeutils_throwback_start();
#endif
		data->fThrowback = TRUE;
	}
#ifdef __riscos__
//FIXME:	ddeutils_throwback_send(ddeutils_THROWBACK_INFO_DETAILS, data->achTextFile, nRow, ddeutils_SEVERITY_ERROR, achError);
	fprintf(stderr, "Line %d: %s\n", nRow, achError);
#else
	fprintf(stderr, "Line %d: %s\n", nRow, achError);
#endif
}


void report_end(PDATA data)
{
#ifdef __riscos__
//FIXME:ddeutils_throwback_end();
#endif
}


void error(DATA *sessionP, PSTR pszFmt, ...)
{
//FIXME:#ifdef __riscos__
#if 0
os_error err;
va_list list;
err.errnum = 0;
va_start(list, pszFmt);
vsprintf(err.errmess, pszFmt, list);
va_end(list);
wimp_report_error(&err, wimp_ERROR_BOX_OK_ICON | wimp_ERROR_BOX_NO_PROMPT, APPNAME);
#else
va_list list;
va_start(list, pszFmt);
vfprintf(stderr, pszFmt, list);
va_end(list);
fputc('\n', stderr);
#endif
sessionP->returnStatus = EXIT_FAILURE;
}
