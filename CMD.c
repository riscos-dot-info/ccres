/* CMD.c

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

/* Standalone command line version of ccres */

// Std C headers :
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccres.h"
#include "Convert.h"
#include "Error.h"
#include "Utils.h"

static void give_help(void);
static void report_cmd(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, ...);
static void report_end_cmd(DATA *sessionP);

static int ReturnStatus = EXIT_SUCCESS;
#ifdef __riscos__
static bool OptionThrowback = false;
#endif

static void give_help(void)
{
  fprintf(stderr, "CCres " VERSION "\n"
          "Convertor between RISC OS Toolbox Resource (filetype &FAE) & Wimp Template (filetype &FEC) files to and from text format.\n"
          "Syntax: CCres [options] <infile> <outfile>\n"
          "Options:\n"
#ifdef __riscos__
          "  -mthrowback : use RISC OS throwback as well for reporting\n"
#endif
          "  -help : gives this help message\n"
          "Input/Output file:\n"
          "  <infile>  : either Template, Resource or Text file\n"
          "  <outfile> : output Text file (case <infile> is a Template or Resource file) or output Template/Resource file (case <infile> is a Text file)\n");
}

int main(int argc, char *argv[])
{
  int carg;
  bool givehelp = false;

  // Parse the option:
  for (carg = 1; carg < argc && argv[carg][0] == '-'; ++carg)
    {
      if (!strcmp(argv[carg], "-help"))
        givehelp = true;
#ifdef __riscos__
      else if (!strcmp(argv[carg], "-mthrowback"))
        OptionThrowback = true;
#endif
      else
        {
          fprintf(stderr, "Unknown option %s\n", argv[carg]);
          return EXIT_FAILURE;
        }
    }
  if (givehelp)
    {
      give_help();
      return EXIT_SUCCESS;
    }
  // Exactly two arguments need to be left over:
  if (argc - carg != 2)
    {
      fprintf(stderr, "Wrong number of arguments\n");
      return EXIT_FAILURE;
    }

  DATA *sessionP;
  if ((sessionP = ccres_initialise()) == NULL)
    {
      fprintf(stderr, "Failed to initialise ccres\n");
      return EXIT_FAILURE;
    }

  ccres_install_report_routine(sessionP, report_cmd, report_end_cmd);

  bits nFileType = get_filetype(sessionP, argv[carg]);

  switch (nFileType)
    {
    case osfile_TYPE_TEXT:
    case osfile_TYPE_RESOURCE:
    case osfile_TYPE_TEMPLATE:
      if (load_file(sessionP, argv[carg], nFileType))
        ccres_convert(sessionP, argv[carg + 1]);
      break;
    default:
      report_cmd(sessionP, report_error, 0, "Unsupported input filetype");
      break;
    }

  (void)ccres_finish(sessionP);

  return ReturnStatus;
}

static void report_cmd(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, ...)
{
  if (level == report_error)
    ReturnStatus = EXIT_FAILURE;

  va_list list;
  va_start(list, pszFmt);
#ifdef __riscos__
  if (OptionThrowback)
    report_varg_throwback(sessionP, level, linenr, pszFmt, list);
  else
#endif
    report_varg_stderr(sessionP, level, linenr, pszFmt, list);
  va_end(list);
}

static void report_end_cmd(DATA *sessionP)
{
#ifdef __riscos__
  if (OptionThrowback)
    report_end_throwback(sessionP);
  else
#endif
    report_end_stderr(sessionP);
}
