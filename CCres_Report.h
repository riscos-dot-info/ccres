/* CCres_Report.h

   Copyright (c) 2004-2007 John Tytgat

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

#ifndef CCRES_REPORT_HEADER_INCLUDED
#define CCRES_REPORT_HEADER_INCLUDED

#include <stdarg.h>
#include "CCres_Convert.h"

void report_varg_stderr(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, va_list list);
void report_end_stderr(DATA *sessionP);
#ifdef __riscos__
void report_varg_throwback(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, va_list list);
void report_end_throwback(DATA *sessionP);
#endif

#endif
