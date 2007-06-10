/* CCRes_Convert.h

   Copyright (c) 2005-2007 John Tytgat

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

#ifndef CCRES_CONVERT_HEADER_INCLUDED
#define CCRES_CONVERT_HEADER_INCLUDED

#include <stdarg.h>
#include <stdbool.h>

#include <oslib/osfile.h>

#define VERSION "1.50 (xx-xxx-2007) - development"

typedef struct session_s DATA;
typedef enum {
  report_info,
  report_warning,
  report_error
} report_level;
typedef void (*report_cb)(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, va_list list);
typedef void (*report_end_cb)(DATA *sessionP);

// Returns NULL in case of failure.
DATA *ccres_initialise(void);

// Returns false for error, true otherwise.
bool ccres_finish(DATA *sessionP);

// Returns false for error, true otherwise.
bool ccres_convert(DATA *sessionP, const char *pszOutFile);

// The routine 'report_routine' gets called in case of info, warning or error reporting.
// The routine 'report_end' will be called when all the reporting has been done.
void ccres_install_report_routine(DATA *sessionP, report_cb report_routine, report_end_cb report_end_routine);

// Calls the report routine registered with ccres_install_report_routine().
void ccres_report(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, ...);

bool ccres_load_file(DATA *data, const char *pszPath, bits nFiletype);

bits ccres_get_filetype_in(DATA *sessionP, const char *filenameP);
bits ccres_get_filetype_out(DATA *sessionP);

#endif
