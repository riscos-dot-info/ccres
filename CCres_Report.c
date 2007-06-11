/* CCres_Report.c

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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <oslib/ddeutils.h>

#include "CCres_Internal.h"
#include "CCres_Report.h"

void report_varg_stderr(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, va_list list)
{
  char achError[1024];
  vsnprintf(achError, sizeof(achError), pszFmt, list);

  const char *severityStrP;
  switch (level)
    {
    case report_info:
      severityStrP = "info";
      break;
    case report_warning:
      severityStrP = "warning";
      break;
    case report_error:
    default:
      severityStrP = "error";
      break;
    }
  fprintf(stderr, "%s:%d:%s: %s\n", sessionP->achFileIn, linenr, severityStrP, achError);
}

void report_end_stderr(DATA *sessionP)
{
// Nothing to do.
}

#ifdef __riscos__
void report_varg_throwback(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, va_list list)
{
  char achError[1024];
  vsnprintf(achError, sizeof(achError), pszFmt, list);

  if (!sessionP->fThrowback && xddeutils_throwback_start() == NULL)
    {
      sessionP->fThrowback = true;
      xddeutils_throwback_send(ddeutils_THROWBACK_PROCESSING, sessionP->achFileIn, 0, 0, NULL);
    }
  int reason;
  int severity;
  const char *severityStrP;
  switch (level)
    {
    case report_info:
      reason = ddeutils_THROWBACK_INFO_DETAILS;
      severity = 0;
      severityStrP = "info";
      break;
    case report_warning:
      reason = ddeutils_THROWBACK_ERROR_DETAILS;
      severity = ddeutils_SEVERITY_WARNING;
      severityStrP = "warning";
      break;
    case report_error:
    default:
      reason = ddeutils_THROWBACK_ERROR_DETAILS;
      severity = ddeutils_SEVERITY_ERROR;
      severityStrP = "error";
      break;
    }

  fprintf(stderr, "%s:%d:%s: %s\n", sessionP->achFileIn, linenr, severityStrP, achError);
  if (sessionP->fThrowback)
    xddeutils_throwback_send(reason, sessionP->achFileIn, linenr, severity, achError);
}

void report_end_throwback(DATA *sessionP)
{
  if (sessionP->fThrowback)
    {
      xddeutils_throwback_end();
      sessionP->fThrowback = false;
    }
}
#endif
