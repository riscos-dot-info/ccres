/* Misc.c
   $Id: Misc.c,v 1.2 2004/03/20 22:12:20 joty Exp $

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

#include <stdio.h>		// sprintf
#include <string.h>		// strncmp

#include <OSLib/taskmanager.h>


// @@should read appname from _Messages file...
wimp_t is_running(void)
{
	taskmanager_task task;
	PSTR pszEnd;
	int i;

	i = 0;
	while ((i = taskmanager_enumerate_tasks(i, &task, sizeof(task), &pszEnd)) > 0) {
		if (__strnicmp(task.name, APPNAME, sizeof(APPNAME) - 1) == 0 && task.name[sizeof(APPNAME) - 1] < ' ') {
			return task.task;
		}
	}

	return NULL;
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
	char achBuff[128];

	for (i = 0; i < ELEMENTS(aAllocs); i++) {
		if (aAllocs[i].p != NULL) {
			sprintf(achBuff, "cb=%d File:%s Line:%d", aAllocs[i].cb, aAllocs[i].achFile, aAllocs[i].nLine);
			errtitle(APPNAME": Memory leak", achBuff);
		}
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

	if ((p = (PSTR) calloc(1, cb + 12)) == NULL) {
		sprintf(achBuff, "%d bytes in file '%s' at line '%d'", cb, pszFile, nLine);
		errtitle(APPNAME": Unable to allocate memory", achBuff);
	} else {
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
	if (cbErr > 0) {
		errtitle(APPNAME": Memory overwritten", achBuff);
	}
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
	char achBuff[128];

	if ((p = (PSTR) calloc(1, cb)) == NULL) {
		sprintf(achBuff, "%d bytes in file '%s' at line '%d'", cb, pszFile, nLine);
		errtitle(APPNAME": Unable to allocate memory", achBuff);
	}
	return p;
}

#endif

#ifdef DEBUG

#include <stdarg.h>

static BOOL fLog = FALSE;
// static char achLog[] = APPDIR".log";
static char achLog[] = "RAM::0.$.ccres_log";
void log_on(void)
{
	FILE * hLog;

	if ((hLog = fopen(achLog, "w")) != NULL) {
		fclose(hLog);
		fLog = TRUE;
	} else {
		error("Unable to create log file");
	}
}

void log_it(PSTR szFmt, ...)
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
