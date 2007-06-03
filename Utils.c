/* Utils.c

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

#include <string.h>

#include "Error.h"
#include "Utils.h"

#define MAX_FILE	48

        void HexToUInt(DATA *data, const char *strP, unsigned int len, unsigned int *resultP)
//      =====================================================================================
{
unsigned int result;
const char *cpStrP;

if (len < 2 || strP[0] != '0' || strP[1] != 'x')
  {
  report(data, strP, "Unknown hex construction '%.*s'", len, strP);
  *resultP = 0;
  return;
  }
result = 0;
for (cpStrP = strP + 2, len -= 2; len-- > 0; ++cpStrP)
  {
  unsigned int nextResult;
  if (*cpStrP < '0'
      || *cpStrP > '9' && *cpStrP < 'A'
      || *cpStrP > 'F' && *cpStrP < 'a'
      || *cpStrP > 'f')
    {
    report(data, strP, "Unknown hex construction '%.*s'", len, strP);
    *resultP = 0;
    return;
    }
  nextResult = result << 4;
  if (*cpStrP <= '9')
    nextResult += *cpStrP - '0';
  else if (*cpStrP <= 'F')
    nextResult += *cpStrP - 'A' + 10;
  else
    nextResult += *cpStrP - 'a' + 10;
  if (nextResult < result)
    {
    report(data, strP, "Hex overflow '%.*s'", len, strP);
    *resultP = 0;
    return;
    }
  result = nextResult;
  }
*resultP = result;
}


        bits get_filetype(DATA *sessionP, const char *filenameP)
//      ========================================================
{
FILE *fhandle;
if ((fhandle = fopen(filenameP, "rb")) == NULL)
  {
  error(sessionP, "Can not open file <%s> for input", filenameP);
  return 0;
  }
char buffer[16];
if (fread(buffer, sizeof(buffer), 1, fhandle) != 1)
  {
  error(sessionP, "Can't read the file contents <%s>", filenameP);
  return 0;
  }
fclose(fhandle);

// Binary resource file ?
const toolbox_resource_file_base *resFileHdrP = (const toolbox_resource_file_base *)buffer;
if (resFileHdrP->file_id == RESF && resFileHdrP->version == 101)
  return osfile_TYPE_RESOURCE;

// Binary template file ?
const int *temFileHdrP = (const int *)buffer;
if (temFileHdrP[1] == 0 && temFileHdrP[2] == 0 && temFileHdrP[3] == 0)
  return osfile_TYPE_TEMPLATE;

// Text file (check for control chars) ?
int i;
for (i = 0; i < sizeof(buffer); ++i)
  if (buffer[i] < 32 && buffer[i] != 10)
    return 0;

return osfile_TYPE_TEXT;
}


// Returns 'false' in case of an error.
        bool load_file(DATA *sessionP, char *pszPath, bits nFiletype)
//      ============================================================
{
sessionP->nFiletypeIn = nFiletype;
if (sessionP->pszIn != NULL)
  MyFree(sessionP->pszIn);

FILE *fhandle;
if ((fhandle = fopen(pszPath, "rb")) == NULL)
  {
  error(sessionP, "Can not open file <%s> for input", pszPath);
  return false;
  }
fseek(fhandle, 0, SEEK_END);
int cbIn = (int)ftell(fhandle);
fseek(fhandle, 0, SEEK_SET);

char *pszIn;
if ((pszIn = (char *) MyAlloc(cbIn)) == NULL)
  return false;

if (fread(pszIn, cbIn, 1, fhandle) != 1)
  {
  error(sessionP, "Can not read file <%s>", pszPath);
  MyFree(pszIn);
  return false;
  }
fclose(fhandle); fhandle = NULL;

sessionP->pszIn = pszIn;
sessionP->cbIn = cbIn;
if (nFiletype == osfile_TYPE_TEXT)
  {
  strcpy(sessionP->achTextFile, pszPath);	// for throwback
  while (--cbIn >= 0)
    {
    if (pszIn[cbIn] == '\n')		// replace newlines with NULLs
      pszIn[cbIn] = '\0';
    }

  if (!memcmp(pszIn, "RESF:", sizeof("RESF:")-1))
    sessionP->nFiletypeOut = osfile_TYPE_RESOURCE;
  else if (!memcmp(pszIn, "Template:", sizeof("Template:")-1))
    sessionP->nFiletypeOut = osfile_TYPE_TEMPLATE;
  else
    {
    error(sessionP, "Unrecognized input file type for %s", pszPath);
    return false;
    }
  }
else
  sessionP->nFiletypeOut = osfile_TYPE_TEXT;

return true;
}


void * My_Alloc(int cb, char *pszFile, int nLine)
{
char *p;

if ((p = (char *) calloc(1, cb)) == NULL)
  fprintf(stderr, "Unable to allocate memory: %d bytes in file '%s' at line '%d'", cb, pszFile, nLine);
return p;
}
