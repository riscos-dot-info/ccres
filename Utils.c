/* Utils.c
   $Id: Utils.c,v 1.1 2005/01/30 16:09:42 joty Exp $

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

#include <string.h>

#include "Error.h"
#include "Utils.h"

#define MAX_FILE	48

        void HexToUInt(PDATA data, const char *strP, unsigned int len, unsigned int *resultP)
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


        bits get_filetype(PDATA sessionP, const char *filenameP)
//      ========================================================
{
char buffer[16];
FILE *fhandle;
const toolbox_resource_file_base *resFileHdrP;
const int *temFileHdrP;
int i;

if ((fhandle = fopen(filenameP, "rb")) == NULL)
  {
  error(sessionP, "Can not open file <%s> for input", filenameP);
  return 0;
  }
if (fread(buffer, sizeof(buffer), 1, fhandle) != 1)
  {
  error(sessionP, "Can't read the file contents <%s>", filenameP);
  return 0;
  }
fclose(fhandle);

// Binary resource file ?
resFileHdrP = (const toolbox_resource_file_base *)buffer;
if (resFileHdrP->file_id == RESF && resFileHdrP->version == 101)
  return osfile_TYPE_RESOURCE;

// Binary template file ?
temFileHdrP = (const int *)buffer;
if (temFileHdrP[1] == 0 && temFileHdrP[2] == 0 && temFileHdrP[3] == 0)
  return osfile_TYPE_TEMPLATE;

// Text file ?
for (i = 0; i < sizeof(buffer); ++i)
  if (buffer[i] < 32)
    return 0;

return osfile_TYPE_TEXT;
}


BOOL load_file(PDATA data, PSTR pszPath, bits nFiletype)
{
PSTR pszIn;
int cbIn;

data->nFiletypeIn = nFiletype;
if (data->pszIn != NULL)
  MyFree(data->pszIn);

if ((cbIn = my_osfile_filesize(pszPath)) > 0
    && (pszIn = (PSTR) MyAlloc(cbIn)) != NULL)
  {
  if (my_osfile_load(pszPath, pszIn, cbIn) != cbIn)
    {
    MyFree(pszIn);
    return FALSE;
    }

  data->pszIn = pszIn;
  data->cbIn = cbIn;
  if (nFiletype == osfile_TYPE_TEXT)
    {
    strcpy(data->achTextFile, pszPath);	// for throwback
    while (--cbIn >= 0)
      {
      if (pszIn[cbIn] == '\n')		// replace newlines with NULLs
        pszIn[cbIn] = '\0';
      }

    if (memcmp(pszIn, "RESF:", 5) == 0)
      data->nFiletypeOut = osfile_TYPE_RESOURCE;
    else if (memcmp(pszIn, "Template:", 9) == 0)
      data->nFiletypeOut = osfile_TYPE_TEMPLATE;
    }
  else
    data->nFiletypeOut = osfile_TYPE_TEXT;

  return TRUE;
}

return FALSE;
}


#ifdef DEBUG
// use these for bounds checking and garbage collection during develop phase

static struct {
	void * p;
	int cb, nLine;
	char achFile[MAX_FILE];
} aAllocs[64];		// arbitary maximum no. of blocks

void MyAlloc_Init(void)
{
	memset(aAllocs, 0, sizeof(aAllocs));
}

// check all memory blocks have been deleted OK, and report the creator of any remaining
void MyAlloc_Report(void)
{
int i;

for (i = 0; i < ELEMENTS(aAllocs); i++)
  {
  if (aAllocs[i].p != NULL)
    LOG((data, "Memory leak: cb=%d File:%s Line:%d", aAllocs[i].cb, aAllocs[i].achFile, aAllocs[i].nLine));
  }
}


// allocate 12 extra bytes -
// 1st word set to required block size (cb)
// 2nd and last words set to 0x50515253
void * My_Alloc(int cb, PSTR pszFile, int nLine)
{
PSTR p;
int i;
char achBuff[128];

if ((p = (PSTR) calloc(1, cb + 12)) == NULL)
	LOG((data, "Unable to allocate memory: %d bytes in file '%s' at line '%d'", cb, pszFile, nLine))
else {
	*((int *) p) = cb;
	for (i = 0; i < 4; i++) {
		p[i + 4] = p[i + cb + 8] = (0x50 | i);
	}
	for (i = 0; i < ELEMENTS(aAllocs); i++) {
		if (aAllocs[i].p == NULL) {
			aAllocs[i].p = p;
			aAllocs[i].cb = cb;
			aAllocs[i].nLine = nLine;
			strcpy(aAllocs[i].achFile, pszFile);
			break;
		}
	}
	return (void *) (p + 8);
}
return NULL;
}

// check for overwritten memory & report if changed
void My_Free(void * v, PSTR pszFile, int nLine)
{
	PSTR p;
	int cb, cbErr, i;
	char achBuff[128];

	p = (PSTR) v;
	p -= 8;
	cb = *((int *) p);

	cbErr = 0;
	for (i = 0; i < 4; i++) {
		if (p[i + 4] != (0x50 | i)) {
			if (cbErr == 0) {
				cbErr += sprintf(&achBuff[cbErr], "%s (%d)", pszFile, nLine);
			}
			cbErr += sprintf(&achBuff[cbErr], "lo[%d = %d]", i, p[i + 4]);
		}
		if (p[i + cb + 8] != (0x50 | i)) {
			if (cbErr == 0) {
				cbErr += sprintf(&achBuff[cbErr], "%s (%d)", pszFile, nLine);
			}
			cbErr += sprintf(&achBuff[cbErr], "hi[%d = %d]", i, p[i + cb + 8]);
		}
	}
	if (cbErr > 0)
		LOG((data, "Memory overwritten: %s", achBuff));
	free(p);
	for (i = 0; i < ELEMENTS(aAllocs); i++) {
		if (aAllocs[i].p == p) {
			aAllocs[i].p = NULL;
			break;
		}
	}
}

#else

void * My_Alloc(int cb, PSTR pszFile, int nLine)
{
PSTR p;

if ((p = (PSTR) calloc(1, cb)) == NULL)
  LOG((data, "Unable to allocate memory: %d bytes in file '%s' at line '%d'", cb, pszFile, nLine));
return p;
}

#endif

#ifdef DEBUG

#include <stdarg.h>

static BOOL fLog = FALSE;
static char achLog[] = "RAM::0.$.ccres_log";
void log_on(PDATA data)
{
	FILE * hLog;

	if ((hLog = fopen(achLog, "w")) != NULL) {
		fclose(hLog);
		fLog = TRUE;
	} else {
		error(data, "Unable to create log file");
	}
}

void log_it(PDATA data, PSTR szFmt, ...)
{
	FILE * hLog;
	va_list list;

	if (fLog && (hLog = fopen(achLog, "a")) != NULL) {
		va_start(list, szFmt);
		vfprintf(hLog, szFmt, list);
		va_end(list);
		fputc('\n', hLog);
		fclose(hLog);
	}
}

#endif	// DEBUG
