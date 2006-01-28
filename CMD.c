/* CMD.c

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

/* Standalone version of ccres */

/* Std C headers :
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccres.h"
#include "Convert.h"
#include "Error.h"
#include "Utils.h"

        int main(int argc, char *argv[])
//      ================================
{
if (argc != 3)
  {
  fprintf(stderr, "CCres " VERSION "\n"
                  "Convertor between RISC OS Toolbox Resource (filetype &FAE) & Wimp Template (filetype &FEC) files to and from text format.\n"
                  "Syntax: CCres <infile> <outfile>\n"
                  "  <infile>  : either Template, Resource or Text file\n"
                  "  <outfile> : output Text file (case <infile> is a Template or Resource file) or output Template/Resource file (case <infile> is a Text file)\n");
  return EXIT_FAILURE;
  }

DATA data;
if (!ccres_initialise(&data))
  return EXIT_FAILURE;

bits nFileType = get_filetype(&data, argv[1]);

switch (nFileType)
  {
  case osfile_TYPE_TEXT:
  case osfile_TYPE_RESOURCE:
  case osfile_TYPE_TEMPLATE:
    if (load_file(&data, argv[1], nFileType))
      ccres_convert(&data, argv[2]);
    break;
  default:
    error(&data, "Unsupported input filetype\n");
    break;
  }

if (data.pszIn != NULL)
  MyFree(data.pszIn);

(void)ccres_finish();

return data.returnStatus;
}
