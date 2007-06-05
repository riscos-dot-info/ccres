/* _Object.c

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

#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>

#include <oslib/ddeutils.h>

#include "ccres.h"
#include "Error.h"
#include "Eval.h"
#include "Library.h"
#include "Utils.h"
#include "_Object.h"

static const FLAGS ObjectHeaderFlags[] = {
	{toolbox_OBJECT_CREATE_ON_LOAD, "toolbox_OBJECT_CREATE_ON_LOAD"},
	{toolbox_OBJECT_SHOW_ON_CREATE, "toolbox_OBJECT_SHOW_ON_CREATE"},
	{toolbox_OBJECT_SHARED        , "toolbox_OBJECT_SHARED"        },
	{toolbox_OBJECT_ANCESTOR      , "toolbox_OBJECT_ANCESTOR"      }
};

static const OBJECTLIST ObjectHeaderList[] = {
	{iol_FLAGS, "header_flags:", offsetof(toolbox_resource_file_object_base, flags),   ObjectHeaderFlags, ELEMENTS(ObjectHeaderFlags)},
	{iol_INT,   "version:",      offsetof(toolbox_resource_file_object_base, version), NULL,              0                          },
	{iol_CHARPTR,  "object_name:",  offsetof(toolbox_resource_file_object_base, name),    NULL,              0                          }
};

// Icon flags to be masked with 0xFFF + 0xE00000 :
static const FLAGS WimpIconFlags[] = {
	{wimp_ICON_TEXT                                                , "wimp_ICON_TEXT"               },
	{wimp_ICON_SPRITE                                              , "wimp_ICON_SPRITE"             },
	{wimp_ICON_BORDER                                              , "wimp_ICON_BORDER"             },
	{wimp_ICON_HCENTRED                                            , "wimp_ICON_HCENTRED"           },
	{wimp_ICON_VCENTRED                                            , "wimp_ICON_VCENTRED"           },
	{wimp_ICON_FILLED                                              , "wimp_ICON_FILLED"             },
	{wimp_ICON_ANTI_ALIASED                                        , "wimp_ICON_ANTI_ALIASED"       },
	{wimp_ICON_NEEDS_HELP                                          , "wimp_ICON_NEEDS_HELP"         },
	{wimp_ICON_INDIRECTED                                          , "wimp_ICON_INDIRECTED"         },
	{wimp_ICON_RJUSTIFIED                                          , "wimp_ICON_RJUSTIFIED"         },
	{wimp_ICON_ALLOW_ADJUST                                        , "wimp_ICON_ALLOW_ADJUST"       },
	{wimp_ICON_HALF_SIZE                                           , "wimp_ICON_HALF_SIZE"          },

	{wimp_ICON_SELECTED                                            , "wimp_ICON_SELECTED"           },
	{wimp_ICON_SHADED                                              , "wimp_ICON_SHADED"             },
	{wimp_ICON_DELETED                                             , "wimp_ICON_DELETED"            }
};

// Icon flags to be masked with wimp_ICON_BUTTON_TYPE
static const FLAGS WimpIconType[] = {
	{(wimp_BUTTON_ALWAYS            << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_ALWAYS"           },
	{(wimp_BUTTON_REPEAT            << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_REPEAT"           },
	{(wimp_BUTTON_CLICK             << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_CLICK"            },
	{(wimp_BUTTON_RELEASE           << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_RELEASE"          },
	{(wimp_BUTTON_DOUBLE_CLICK      << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_DOUBLE_CLICK"     },
	{(wimp_BUTTON_CLICK_DRAG        << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_CLICK_DRAG"       },
	{(wimp_BUTTON_RELEASE_DRAG      << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_RELEASE_DRAG"     },
	{(wimp_BUTTON_DOUBLE_DRAG       << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_DOUBLE_DRAG"      },
	{(wimp_BUTTON_MENU_ICON         << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_MENU_ICON"        },
	{(wimp_BUTTON_DOUBLE_CLICK_DRAG << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_DOUBLE_CLICK_DRAG"},
	{(wimp_BUTTON_RADIO             << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_RADIO"            },
	{(wimp_BUTTON_WRITE_CLICK_DRAG  << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_WRITE_CLICK_DRAG" },
	{(wimp_BUTTON_WRITABLE          << wimp_ICON_BUTTON_TYPE_SHIFT), "wimp_BUTTON_WRITABLE"         }
};

static const FLAGS OsColours[] = {
	{os_COLOUR_TRANSPARENT    , "os_COLOUR_TRANSPARENT"    },
	{os_COLOUR_RED            , "os_COLOUR_RED"            },
	{os_COLOUR_GREEN          , "os_COLOUR_GREEN"          },
	{os_COLOUR_BLUE           , "os_COLOUR_BLUE"           },
	{os_COLOUR_CYAN           , "os_COLOUR_CYAN"           },
	{os_COLOUR_MAGENTA        , "os_COLOUR_MAGENTA"        },
	{os_COLOUR_YELLOW         , "os_COLOUR_YELLOW"         },
	{os_COLOUR_WHITE          , "os_COLOUR_WHITE"          },
	{os_COLOUR_VERY_LIGHT_GREY, "os_COLOUR_VERY_LIGHT_GREY"},
	{os_COLOUR_LIGHT_GREY     , "os_COLOUR_LIGHT_GREY"     },
	{os_COLOUR_MID_LIGHT_GREY , "os_COLOUR_MID_LIGHT_GREY" },
	{os_COLOUR_MID_DARK_GREY  , "os_COLOUR_MID_DARK_GREY"  },
	{os_COLOUR_DARK_GREY      , "os_COLOUR_DARK_GREY"      },
	{os_COLOUR_VERY_DARK_GREY , "os_COLOUR_VERY_DARK_GREY" },
	{os_COLOUR_BLACK          , "os_COLOUR_BLACK"          },
	{os_COLOUR_DARK_BLUE      , "os_COLOUR_DARK_BLUE"      },
	{os_COLOUR_LIGHT_YELLOW   , "os_COLOUR_LIGHT_YELLOW"   },
	{os_COLOUR_LIGHT_GREEN    , "os_COLOUR_LIGHT_GREEN"    },
	{os_COLOUR_LIGHT_RED      , "os_COLOUR_LIGHT_RED"      },
	{os_COLOUR_CREAM          , "os_COLOUR_CREAM"          },
	{os_COLOUR_DARK_GREEN     , "os_COLOUR_DARK_GREEN"     },
	{os_COLOUR_ORANGE         , "os_COLOUR_ORANGE"         },
	{os_COLOUR_LIGHT_BLUE     , "os_COLOUR_LIGHT_BLUE"     }
};

static const FLAGS WimpColour[] = {
	{wimp_COLOUR_WHITE          , "wimp_COLOUR_WHITE"          },
	{wimp_COLOUR_VERY_LIGHT_GREY, "wimp_COLOUR_VERY_LIGHT_GREY"},
	{wimp_COLOUR_LIGHT_GREY     , "wimp_COLOUR_LIGHT_GREY"     },
	{wimp_COLOUR_MID_LIGHT_GREY , "wimp_COLOUR_MID_LIGHT_GREY" },
	{wimp_COLOUR_MID_DARK_GREY  , "wimp_COLOUR_MID_DARK_GREY"  },
	{wimp_COLOUR_DARK_GREY      , "wimp_COLOUR_DARK_GREY"      },
	{wimp_COLOUR_VERY_DARK_GREY , "wimp_COLOUR_VERY_DARK_GREY" },
	{wimp_COLOUR_BLACK          , "wimp_COLOUR_BLACK"          },
	{wimp_COLOUR_DARK_BLUE      , "wimp_COLOUR_DARK_BLUE"      },
	{wimp_COLOUR_YELLOW         , "wimp_COLOUR_YELLOW"         },
	{wimp_COLOUR_LIGHT_GREEN    , "wimp_COLOUR_LIGHT_GREEN"    },
	{wimp_COLOUR_RED            , "wimp_COLOUR_RED"            },
	{wimp_COLOUR_CREAM          , "wimp_COLOUR_CREAM"          },
	{wimp_COLOUR_DARK_GREEN     , "wimp_COLOUR_DARK_GREEN"     },
	{wimp_COLOUR_ORANGE         , "wimp_COLOUR_ORANGE"         },
	{wimp_COLOUR_LIGHT_BLUE     , "wimp_COLOUR_LIGHT_BLUE"     },
	{wimp_COLOUR_TRANSPARENT    , "wimp_COLOUR_TRANSPARENT"    }
};

static const FLAGS CmpFlags[] = {
	{toolbox_NULL_COMPONENT, "toolbox_NULL_COMPONENT"},
	{toolbox_WINDOW_FOCUS  , "toolbox_WINDOW_FOCUS"  }
};


static char *parse(DATA *data, char *pszIn, const char *pszEntry)
{
	char *p, *pszEnd;
	int cb;
	char ch, ch0;

	ch0 = *pszEntry++;		// it's faster to check first char before calling strncasecmp
	cb = strlen(pszEntry);
	p = pszIn;
	pszEnd = &data->pszIn[data->cbIn] - cb;
	while ((ch = *p++) != '}' && p < pszEnd) {
		if (ch == ':' || ch == '#') {  // skip values and comments
			while (*p++ >= ' ' && p < pszEnd)
				/* */;
		} else if (ch == ch0 && strncasecmp(p, pszEntry, cb) == 0) {
			return p + cb;
		}
	}
	report(data, pszIn, "Missing entry '%s'", --pszEntry);
	return NULL;
}


static bool add_to_reloc_table(RELOCTABLE *pRelocTable, int nEntry, int nTable)
{
	RELOC *pReloc;
	int nReloc;

	nReloc = pRelocTable->ref++;
	pRelocTable->pReloc[nReloc].offset = nEntry;
	pRelocTable->pReloc[nReloc].type = nTable;

	if (pRelocTable->ref >= (pRelocTable->max - sizeof(RELOC))) {
		nReloc = pRelocTable->max * 3 / 2;
		if ((pReloc = MyAlloc(nReloc * sizeof(RELOC))) == NULL) {
			return false;
		}
		memcpy(pReloc, pRelocTable->pReloc, pRelocTable->max * sizeof(RELOC));
		MyFree(pRelocTable->pReloc);
		pRelocTable->pReloc = pReloc;
		pRelocTable->max = nReloc;
	}
	return true;
}


// Copies string at inP terminated by any control character up to maxSize characters.
// When maxSize < 0, it is considered to be INT_MAX.
// If maxSize != 0, the result is always terminated.
// Input string maybe quoted and this will be checked for consistency.
// Returns the number of characters copied *including* termination character.
static int copy_quoted_string(DATA *data, char *outP, const char *inP, int maxSize)
{
const char * const fixOutP = outP;
const char * const fixInP = inP;
char c;
const bool isQuoted = (*inP == '"');
if (isQuoted)
  ++inP;
if (maxSize < 0)
  maxSize = INT_MAX;
const int fixMaxSize = maxSize;
while ((c = *inP++) >= ' '
       && (!isQuoted || c != '"' || *inP >= ' ')
       && maxSize > 1)
  {
    *outP++ = c;
    --maxSize;
  }
if (c >= ' ' && c != '"')
  report(data, inP, "Can only store first %d characters of string '%s'", fixMaxSize, fixInP);
else if (isQuoted)
  {
    if (c != '"')
      report(data, inP, "Incorrect quoted string '%s'", fixInP);
  }
if (maxSize >= 1)
  *outP++ = '\0';
return outP - fixOutP;
}


static bool put_string(DATA *data, char *pszIn, int nOffset, char *object, const STRINGLIST *StringList)
{
	STRINGTABLE *pTable;
	char *pszEntry, *pszLimit;
	int cbEntry, cbLimit, nTable;

	if (StringList->nTable == iol_STRING) {
		pTable = &data->StringTable;
		nTable = toolbox_RELOCATE_STRING_REFERENCE;
	} else if (StringList->nTable == iol_MSG) {
		pTable = &data->MessageTable;
		nTable = toolbox_RELOCATE_MSG_REFERENCE;
	} else {
		error(data, "Unexpected string table type (%d)", StringList->nTable);
		return false;
	}
	// cbEntry includes terminating character.
	if ((pszEntry = parse(data, pszIn, StringList->pszEntry)) != NULL)
		cbEntry = copy_quoted_string(data, &pTable->pstr[pTable->ref], pszEntry, -1);
	else
		cbEntry = 0;

	if (StringList->pszLimit != NULL && (pszLimit = parse(data, pszIn, StringList->pszLimit)) != NULL) {
		if (pszLimit[0] == '*') {
			cbLimit = cbEntry;
		} else {
			cbLimit = my_atoi(&pszLimit);
			if (cbLimit == 0 && cbEntry <= 1)
				cbEntry = 0;
			else if (cbLimit < cbEntry) {
				error(data, "Warning: increased size value for %s from %d to %d to fully include string <%.*s>", StringList->pszEntry, cbLimit, cbEntry, cbEntry ? cbEntry - 1 : cbEntry, &pTable->pstr[pTable->ref]);
				cbLimit = cbEntry;
			}
		}
	}
	else
		cbLimit = cbEntry;

	write_le_int32(&object[StringList->nEntry], (cbEntry > 1) ? pTable->ref : -1);
	add_to_reloc_table(&data->RelocTable, StringList->nEntry + nOffset, nTable);
	if (StringList->pszLimit != NULL)
		write_le_int32(&object[StringList->nLimit], cbLimit);
	if (cbEntry > 1) {
		pTable->ref += cbEntry;
		if (pTable->ref > (pTable->max - 256)) {
			char *pstr;
			int cb = pTable->max * 3 / 2;
			if ((pstr = MyAlloc(cb)) == NULL) {
				return false;
			}
			memcpy(pstr, pTable->pstr, pTable->max);
			MyFree(pTable->pstr);
			pTable->pstr = pstr;
			pTable->max = cb;
		}
	}
	return true;
}


static bool put_tstring(DATA *data, char *pszIn, int nOffset, char *object, const STRINGLIST *StringList)
{
	STRINGTABLE *pTable;
	char *pszEntry, *pszLimit;
	int cbEntry, cbLimit, nTable;

	pTable = &data->StringTable;
	nTable = toolbox_RELOCATE_STRING_REFERENCE;

	/* A template string is terminated with "\r". */
	if ((pszEntry = parse(data, pszIn, StringList->pszEntry)) != NULL) {
		cbEntry = copy_quoted_string(data, &pTable->pstr[pTable->ref], pszEntry, -1);
		// Convert terminating \0 char into \r:
		pTable->pstr[pTable->ref + cbEntry - 1] = '\r';
	}
	else
		cbEntry = 0;

	/* If the limit/size is 0, then we won't allocate any space for Entry,
           even not its terminating character "\r".  */
	if (StringList->pszLimit != NULL && (pszLimit = parse(data, pszIn, StringList->pszLimit)) != NULL) {
		if (pszLimit[0] == '*') {
			/* The limit is as long as the cbEntry is atm.  */
			cbLimit = cbEntry;
		} else {
			cbLimit = my_atoi(&pszLimit);
			if (cbLimit == 0 && cbEntry == 1)
				cbEntry = 0;
			else if (cbLimit < cbEntry) {
				error(data, "Warning: increased size value for %s from %d to %d to fully include string <%.*s>", StringList->pszEntry, cbLimit, cbEntry, cbEntry ? cbEntry - 1 : cbEntry, &pTable->pstr[pTable->ref]);
				cbLimit = cbEntry;
			}
		}
	}
	else
		cbLimit = cbEntry; /* No limit/size, we do NOT optimize string "\r" to a 0 offset.  */

	write_le_int32(&object[StringList->nEntry], (cbLimit > 0) ? (pTable->ref - nOffset) : 0);
	if (StringList->pszLimit != NULL)
		write_le_int32(&object[StringList->nLimit], cbLimit);
	if (cbEntry > 0) {
		pTable->ref += cbLimit;
		if (pTable->ref > pTable->max - 256) {
			char *pstr;
			int cb = pTable->max * 3 / 2;
			if ((pstr = MyAlloc(cb)) == NULL) {
				return false;
			}
			memcpy(pstr, pTable->pstr, pTable->max);
			MyFree(pTable->pstr);
			pTable->pstr = pstr;
			pTable->max = cb;
		}
	}
	return true;
}


// Given strings (with either \0, \n or \r delimitered) and an offset, return
// string at offset 'ref' and its length in *outSizeP.
static  const char *string_from_table(DATA *data, const char *pszTable, size_t tableSize, int ref, int maxSize, int *outSizeP)
//      ========================================================================================
{
const char *pstr, *p;
char ch;

// ref == -1 is valid Toolbox reference meaning the empty string.
if (pszTable == NULL || ref == -1)
  {
    *outSizeP = 0;
    return "";
  }
else if (ref < 0 || ref >= tableSize)
  {
    error(data, "Error: string index %d is out-of-bounds (string table size is %d)", ref, tableSize);
    *outSizeP = 0;
    return "";
  }

if (maxSize < 0)
  maxSize = INT_MAX;
pstr = p = &pszTable[ref];
while ((ch = *p++) != '\0' && ch != '\n' && ch != '\r' && maxSize-- >= 0)
  /* */;
*outSizeP = p - 1 - pstr;
return pstr;
}


static  void get_string(DATA *data, FILE *hf, const TOOLBOXSMTABLE *strMsgTableP, const char *objectP, const STRINGLIST *StringList, const char *pszIndent)
//      ==============================================================================================================================================
{
const char *pstr, *string_table;
size_t string_table_size;
int ref, strSize;

switch (StringList->nTable)
  {
  case iol_STRING:
    string_table = strMsgTableP->stringTableP;
    string_table_size = strMsgTableP->stringTableSize;
    break;
  case iol_MSG:
    string_table = strMsgTableP->messageTableP;
    string_table_size = strMsgTableP->messageTableSize;
    break;
  default:
    error(data, "Unexpected string table type (%d)", StringList->nTable);
    return;
  }

ref = read_le_int32(&objectP[StringList->nEntry]);
pstr = string_from_table(data, string_table, string_table_size, ref, -1, &strSize);

fprintf(hf, "%s\"%.*s\"\n", StringList->pszEntry, strSize, pstr);
if (StringList->pszLimit != NULL)
  {
  int cbLimit;
  fputs(pszIndent, hf);
  cbLimit = read_le_int32(&objectP[StringList->nLimit]);
  // cbLimit = 0 for an empty string is ok (different than get_tstring() !)
  if (cbLimit == 0 && cbLimit < strSize
      || cbLimit != 0 && cbLimit < strSize + 1)
    error(data, "Warning: string <%.*s> is longer than its given limit value %d", strSize, pstr, cbLimit);
  if (cbLimit > strSize + 1)
    fprintf(hf, "%s%d\n", StringList->pszLimit, cbLimit);
  else
    fprintf(hf, "%s*\n", StringList->pszLimit);
  }
}


static  void get_tstring(DATA *data, FILE *hf, const TOOLBOXSMTABLE *strMsgTableP, const char *objectP, const STRINGLIST *StringList, const char *pszIndent)
//      =============================================================================================================
{
const char *pstr;
int ref, strSize;

ref = read_le_int32(&objectP[StringList->nEntry]);
pstr = string_from_table(data, strMsgTableP->stringTableP, strMsgTableP->stringTableSize, ref, -1, &strSize);

fprintf(hf, "%s\"%.*s\"\n", StringList->pszEntry, strSize, pstr);
if (StringList->pszLimit != NULL)
  {
  int cbLimit;
  fputs(pszIndent, hf);
  cbLimit = read_le_int32(&objectP[StringList->nLimit]);
  if (cbLimit < strSize + 1)
    error(data, "Warning: string <%.*s> is longer than its given limit value %d", strSize, pstr, cbLimit);
  if (cbLimit > strSize + 1)
    fprintf(hf, "%s%d\n", StringList->pszLimit, cbLimit);
  else
    fprintf(hf, "%s*\n", StringList->pszLimit);
  }
}


static  bits put_flags(DATA *data, char *pstrFlags, const FLAGS *pFlags, int nFlags)
//      ============================================================================
{
char *p;
bits f;

f = 0;
for (p = pstrFlags; *p != '\0'; /* */)
  {
  int n, cb;
  char ch;

  while ((ch = *p) > ' ' && ch != '|')
    p++;
  cb = p - pstrFlags;
  for (n = 0; n < nFlags; n++)
    {
    if (strncasecmp(pstrFlags, pFlags[n].pstr, cb) == 0 && strlen(pFlags[n].pstr) == cb)
      {
      f |= pFlags[n].flag;
      break;
      }
    }
  if (n == nFlags)
    {
    if (pstrFlags[0] == '0' && pstrFlags[1] == 'x')
      {
      unsigned int result;
      (void)HexToUInt(data, pstrFlags, cb, &result);
      f |= result;
      }
    else
      report(data, pstrFlags, "Unknown flag '%.*s'", cb, pstrFlags);
    }

  while ((ch = *p) == ' ' || ch == '|')
    p++;
  pstrFlags = p;
  }

return f;
}


static  void get_flags(DATA *data, FILE *hf, const char *pszFlags, bits fFlags, const FLAGS *pFlags, int nFlags)
//      =========================================================================================================
{
const char *pszOr;
int cb, n;
char achBuff[1120];		// =32 x 32 character flags

cb = sprintf(achBuff, "%s", pszFlags);
pszOr = "";
for (n = 0; n < nFlags && fFlags != 0; n++)
  {
  if ((fFlags & pFlags[n].flag) == pFlags[n].flag)
    {
    cb += sprintf(&achBuff[cb], "%s%s", pszOr, pFlags[n].pstr);
    pszOr = " | ";
    fFlags &= ~pFlags[n].flag;
    }
  }
if (fFlags != 0)
  cb += sprintf(&achBuff[cb], "%s0x%x", pszOr, fFlags);
achBuff[cb++] = '\n';
fwrite(achBuff, cb, 1, hf);
}


// fInt - value may be a #defined variable or a nunber (hex or decimal)
static int put_enum(DATA *data, char *pstrFlags, const FLAGS *pFlags, int nFlags, bool fInt)
{
	int n;

	if (*pstrFlags > ' ') {
		for (n = 0; n < nFlags; n++) {
			if (strcmp(pstrFlags, pFlags[n].pstr) == 0) {
				return pFlags[n].flag;
			}
		}
		if (fInt) {
			return my_atoi(&pstrFlags);
		}
		report(data, pstrFlags, "Unknown variable '%s'", pstrFlags);
	}

	return 0;
}


// fInt - value may be a #defined variable or a nunber (hex or decimal)
static void get_enum(FILE *hf, const char *pszFlags, int fFlags, const FLAGS *pFlags, int nFlags, bool fInt)
{
	int cb, n;
	char achBuff[64];		// = 32 + 32 character flags

	cb = my_strcpy(achBuff, pszFlags);
	for (n = 0; n < nFlags; n++) {
		if (fFlags == pFlags[n].flag) {
			cb += my_strcpy(&achBuff[cb], pFlags[n].pstr);
			goto get_enum_found;
		}
	}
	if (fInt) {
		cb += sprintf(&achBuff[cb], "&%x", fFlags);
	}

get_enum_found:

	achBuff[cb++] = '\n';
	fwrite(achBuff, cb, 1, hf);
}


static bits put_iflags(DATA *data, char *pstrFlags)
{
char *p;
bits f;

f = 0;
p = pstrFlags;
while (*p != '\0')
  {
  const FLAGS *pFlags;
  int n, cb, nFlags;
  char ch;

  while ((ch = *p) > ' ' && ch != '|')
    p++;
  cb = p - pstrFlags;

  pFlags = WimpIconFlags;
  nFlags = ELEMENTS(WimpIconFlags);
  for (n = 0; n < nFlags; n++)
    {
    if (strncasecmp(pstrFlags, pFlags[n].pstr, cb) == 0 && strlen(pFlags[n].pstr) == cb)
      {
      f |= pFlags[n].flag;
      goto put_iflags_next_flag;
      }
    }

  pFlags = WimpIconType;
  nFlags = ELEMENTS(WimpIconType);
  for (n = 0; n < nFlags; n++)
    {
    if (strcmp(pstrFlags, pFlags[n].pstr) == 0)
      {
      f |= pFlags[n].flag;
      goto put_iflags_next_flag;
      }
    }
  if (pstrFlags[0] == '0' && pstrFlags[1] == 'x')
    {
    unsigned int result;
    (void)HexToUInt(data, pstrFlags, cb, &result);
    f |= result;
    }
  else
    report(data, pstrFlags, "Unknown flag '%.*s'", cb, pstrFlags);

put_iflags_next_flag:

  while ((ch = *p) == ' ' || ch == '|')
    p++;
  pstrFlags = p;
}

return f;
}


// Output the bits 0xFFF | 0xE00000 and wimp_ICON_BUTTON_TYPE
static  void get_iflags(DATA *data, FILE *hf, const char *pszFlags, bits fFlags)
//      =========================================================================
{
const FLAGS *pFlags;
const char *pszOr;
int cb, n, nFlags;
bits fFlagsCur;
char achBuff[1120];		// =32 x 32 character flags

pFlags = WimpIconFlags;
nFlags = ELEMENTS(WimpIconFlags);
cb = sprintf(achBuff, "%s", pszFlags);
pszOr = "";
fFlagsCur = fFlags & (0xFFF | 0xE00000);
for (n = 0; n < nFlags; n++)
  {
  if ((fFlagsCur & pFlags[n].flag) == pFlags[n].flag)
    {
    cb += sprintf(&achBuff[cb], "%s%s", pszOr, pFlags[n].pstr);
    pszOr = " | ";
    fFlagsCur &= ~pFlags[n].flag;
    }
  }
if (fFlagsCur!= 0)
  cb += sprintf(&achBuff[cb], "%s0x%x", pszOr, fFlagsCur & ~wimp_ICON_BUTTON_TYPE);

pFlags = WimpIconType;
nFlags = ELEMENTS(WimpIconType);
fFlagsCur = fFlags & wimp_ICON_BUTTON_TYPE;
for (n = 0; n < nFlags; n++)
  {
  if (fFlagsCur == pFlags[n].flag)
    {
    cb += sprintf(&achBuff[cb], "%s%s", pszOr, pFlags[n].pstr);
    fFlagsCur = 0;
    break;
    }
  }
if (fFlagsCur != 0)
  cb += sprintf(&achBuff[cb], "%s0x%x", pszOr, fFlagsCur);
achBuff[cb++] = '\n';
fwrite(achBuff, cb, 1, hf);
}


static void put_box(DATA *data, char *pstr, os_box *box)
{
	int *pi;
	int n;

	pi = (int *) box;
	for (n = 0; n < 4; n++) {
		pi[n] = Eval(data, &pstr);
		pstr++;
	}
}


static  void get_box(FILE *hf, const char *pszBox, const os_box *bbox)
//      ================================================================
{
fprintf(hf, "%s%d,%d,%d,%d\n", pszBox, bbox->x0, bbox->y0, bbox->x1, bbox->y1);
}


static void put_coord(DATA *data, char *pstr, os_coord *coord)
{
	int *pi;
	int n;

	pi = (int *) coord;
	for (n = 0; n < 2; n++) {
		pi[n] = Eval(data, &pstr);
		pstr++;
	}
}


static  void get_coord(FILE *hf, const char *pszCoord, const os_coord *coord)
//      =======================================================================
{
fprintf(hf, "%s%d,%d\n", pszCoord, coord->x, coord->y);
}


static  void put_pstr(DATA *data, char *pstr, const char *pszEntry, int nChars)
//      =======================================================================
{
if (nChars == 0)
  nChars = -1;
int cb = copy_quoted_string(data, pstr, pszEntry, nChars);
if (data->nFiletypeOut == osfile_TYPE_TEMPLATE)
  pstr[cb - 1] = '\r';
}


static  void get_pstr(DATA *data, FILE *hf, const char *pszEntry, const char *objectP, int nEntry, int nChars)
//      ======================================================================================================
{
int strSize;
const char *strP = string_from_table(data, objectP, INT_MAX, nEntry, nChars == 0 ? -1 : nChars, &strSize);
fprintf(hf, "%s\"%.*s\"\n", pszEntry, strSize, strP);
}


void put_objects(DATA *data, char *pszIn, int nOffset, char *Object, const OBJECTLIST *ObjectList, int nObjects)
{
bits *pBits;
uint16_t *pShort;
uint8_t *pByte;
char *pszEntry;
int n;

for (n = 0; n < nObjects; n++, ObjectList++)
  {
  switch (ObjectList->nTable)
    {
    case iol_MSG:
    case iol_STRING:
      put_string(data, pszIn, nOffset, Object, (const STRINGLIST *)ObjectList);
      break;
    case iol_TSTRING:
      put_tstring(data, pszIn, nOffset, Object, (const STRINGLIST *)ObjectList);
      break;
    case iol_OBJECT:
      add_to_reloc_table(&data->RelocTable, ObjectList->nEntry, toolbox_RELOCATE_OBJECT_OFFSET);
      break;
    default:
      if ((pszEntry = parse(data, pszIn, ObjectList->pszEntry)) != NULL)
        {
        switch (ObjectList->nTable)
          {
          case iol_FLAGS:
            pBits = (bits *) &Object[ObjectList->nEntry];
            *pBits = put_flags(data, pszEntry, (const FLAGS *) ObjectList->pData, ObjectList->nData);
            break;
          case iol_IFLAGS:
            pBits = (bits *) &Object[ObjectList->nEntry];
            *pBits = put_iflags(data, pszEntry);
            break;
          case iol_BFLAGS:
            pByte = (uint8_t *) &Object[ObjectList->nEntry];
            *pByte = (uint8_t) put_flags(data, pszEntry, (const FLAGS *) ObjectList->pData, ObjectList->nData);
            break;
          case iol_ENUM:
            write_le_int32(&Object[ObjectList->nEntry], put_enum(data, pszEntry, (const FLAGS *) ObjectList->pData, ObjectList->nData, false));
            break;
          case iol_CMP:
            write_le_int32(&Object[ObjectList->nEntry], put_enum(data, pszEntry, CmpFlags, ELEMENTS(CmpFlags), true));
            break;
          case iol_OSCOL:
            write_le_int32(&Object[ObjectList->nEntry], put_enum(data, pszEntry, OsColours, ELEMENTS(OsColours), false));
            break;
          case iol_BOX:
            put_box(data, pszEntry, (os_box *) &Object[ObjectList->nEntry]);
            break;
          case iol_BITS:
            pBits = (bits *) &Object[ObjectList->nEntry];
            if (*pszEntry < ' ')
              *pBits = (ObjectList->nData == bits_ACTION) ? ~0 : 0;
            else
              *pBits = (ObjectList->nData == bits_EVAL) ? Eval(data, &pszEntry) : my_atoi(&pszEntry);
            break;
          case iol_INT:
            write_le_int32(&Object[ObjectList->nEntry], my_atoi(&pszEntry));
            break;
          case iol_SHORT:
            pShort = (uint16_t *) &Object[ObjectList->nEntry];
            *pShort = (uint16_t) my_atoi(&pszEntry);
            break;
          case iol_BYTE:
            pByte = (uint8_t *) &Object[ObjectList->nEntry];
            *pByte = (uint8_t) my_atoi(&pszEntry);
            break;
          case iol_COORD:
            put_coord(data, pszEntry, (os_coord *) &Object[ObjectList->nEntry]);
            break;
          case iol_SPRITE:
            pBits = (bits *) &Object[ObjectList->nEntry];
            *pBits = my_atoi(&pszEntry);
            add_to_reloc_table(&data->RelocTable, ObjectList->nEntry, toolbox_RELOCATE_SPRITE_AREA_REFERENCE);
            break;
          case iol_CHARPTR:
            put_pstr(data, &Object[ObjectList->nEntry], pszEntry, ObjectList->nData);
            break;
          case iol_ESG:
            pBits = (bits *) &Object[ObjectList->nEntry];
            *pBits |= (my_atoi(&pszEntry) << wimp_ICON_ESG_SHIFT);
            break;
          case iol_WCOL:
            pByte = (uint8_t *) &Object[ObjectList->nEntry];
            *pByte = (uint8_t) put_enum(data, pszEntry, WimpColour, ELEMENTS(WimpColour), false);
            break;
          case iol_BCOLS:
            pBits = (bits *) &Object[ObjectList->nEntry];
            *pBits |= put_enum(data, pszEntry, WimpColour, ELEMENTS(WimpColour), false) << wimp_ICON_FG_COLOUR_SHIFT;
            if ((pszEntry = parse(data, pszIn, ObjectList->pData)) != NULL)
              *pBits |= put_enum(data, pszEntry, WimpColour, ELEMENTS(WimpColour), false) << wimp_ICON_BG_COLOUR_SHIFT;
            break;
          default:
            error(data, "Unknown iol_ value (%d)", ObjectList->nTable);
            break;
          }
        }
      break;
    }
  }
}


void get_objects(DATA *data, FILE *hf, const TOOLBOXSMTABLE *strMsgTableP, const char *objectP, const OBJECTLIST *ObjectList, int nObjects, int nIndent)
{
const char *const pszIndent = (nIndent == 1) ? "  " : (nIndent == 2) ? "    " : "";
int i, n;

for (n = 0; n < nObjects; n++, ++ObjectList)
  {
  if (ObjectList->nTable != iol_OBJECT)
    { // do nothing for res2text
    fputs(pszIndent, hf);
    switch (ObjectList->nTable)
      {
      case iol_MSG:
      case iol_STRING:
        get_string(data, hf, strMsgTableP, objectP, (const STRINGLIST *)ObjectList, pszIndent);
        break;
      case iol_TSTRING:
        get_tstring(data, hf, strMsgTableP, objectP, (const STRINGLIST *)ObjectList, pszIndent);
        break;
      case iol_FLAGS:
        {
        const bits *pBits = (const bits *)&objectP[ObjectList->nEntry];
        get_flags(data, hf, ObjectList->pszEntry, *pBits, (const FLAGS *) ObjectList->pData, ObjectList->nData);
        break;
        }
      case iol_IFLAGS:
        {
        const bits *pBits = (const bits *)&objectP[ObjectList->nEntry];
        get_iflags(data, hf, ObjectList->pszEntry, *pBits);
        break;
        }
      case iol_BFLAGS:
        {
        const unsigned char *pByte = (const unsigned char *)&objectP[ObjectList->nEntry];
        get_flags(data, hf, ObjectList->pszEntry, *pByte, (const FLAGS *) ObjectList->pData, ObjectList->nData);
        break;
        }
      case iol_ENUM:
        get_enum(hf, ObjectList->pszEntry, read_le_int32(&objectP[ObjectList->nEntry]), (const FLAGS *) ObjectList->pData, ObjectList->nData, false);
        break;
      case iol_CMP:
        get_enum(hf, ObjectList->pszEntry, read_le_int32(&objectP[ObjectList->nEntry]), CmpFlags, ELEMENTS(CmpFlags), true);
        break;
      case iol_OSCOL:
        get_enum(hf, ObjectList->pszEntry, read_le_int32(&objectP[ObjectList->nEntry]), OsColours, ELEMENTS(OsColours), false);
        break;
      case iol_BOX:
        get_box(hf, ObjectList->pszEntry, (const os_box *) &objectP[ObjectList->nEntry]);
        break;
      case iol_BITS:
        {
        const bits *pBits = (const bits *)&objectP[ObjectList->nEntry];
        if (*pBits == ~0 && ObjectList->nData == bits_ACTION)
          fprintf(hf, "%s\n", ObjectList->pszEntry);
        else
          fprintf(hf, "%s&%x\n", ObjectList->pszEntry, *pBits);
        break;
        }
      case iol_INT:
        fprintf(hf, "%s%d\n", ObjectList->pszEntry, read_le_int32(&objectP[ObjectList->nEntry]));
        break;
      case iol_SHORT:
        fprintf(hf, "%s%d\n", ObjectList->pszEntry, read_le_int16(&objectP[ObjectList->nEntry]));
        break;
      case iol_BYTE:
        {
        const unsigned char *pByte = (const unsigned char *)&objectP[ObjectList->nEntry];
        fprintf(hf, "%s%d\n", ObjectList->pszEntry, (bits) *pByte);
        break;
        }
      case iol_COORD:
        get_coord(hf, ObjectList->pszEntry, (const os_coord *)&objectP[ObjectList->nEntry]);
        break;
      case iol_SPRITE:
        fprintf(hf, "%s&%x\n", ObjectList->pszEntry, read_le_int32(&objectP[ObjectList->nEntry]));
        break;
      case iol_CHARPTR:
        get_pstr(data, hf, ObjectList->pszEntry, objectP, ObjectList->nEntry, ObjectList->nData);
        break;
      case iol_ESG:
        {
        const bits *pBits = (const bits *)&objectP[ObjectList->nEntry];
        fprintf(hf, "%s%d\n", ObjectList->pszEntry, (*pBits & wimp_ICON_ESG) >> wimp_ICON_ESG_SHIFT);
        break;
        }
      case iol_WCOL:
        {
        const unsigned char *pByte = (const unsigned char *)&objectP[ObjectList->nEntry];
        get_enum(hf, ObjectList->pszEntry, *pByte, WimpColour, ELEMENTS(WimpColour), false);
        break;
        }
      case iol_BCOLS:
        {
        const bits *pBits = (const bits *)&objectP[ObjectList->nEntry];
        i = (*pBits & wimp_ICON_FG_COLOUR) >> wimp_ICON_FG_COLOUR_SHIFT;
        get_enum(hf, ObjectList->pszEntry, i, WimpColour, ELEMENTS(WimpColour), false);
        fputs(pszIndent, hf);
        i = (*pBits & wimp_ICON_BG_COLOUR) >> wimp_ICON_BG_COLOUR_SHIFT;
        get_enum(hf, ObjectList->pData, i, WimpColour, ELEMENTS(WimpColour), false);
        break;
        }
      default:
        error(data, "Unknown iol_ value (%d)", ObjectList->nTable);
        break;
      }
    }
  }
}


#define min(a,b) ((a)<(b)?(a):(b))
// Iconbar {
// returns "Iconbar\0" and sets pszIn to byte after '{'
char *next_object(char ** pszIn, char *pszEnd)
{
	char *p, *q;
	int cb;
	char ch;
	static char achObject[48];

	p = *pszIn;
	while ((ch = *p++) != '{') {
		if (ch == '}' || p >= pszEnd) {
			return NULL;
		}
	}
// p points to char *after* the brace
	*pszIn = p--;
	while (*(p - 1) <= ' ')
		p--;	// skip trailing spaces
	*p = '\0';
	q = p;
	while (*(q - 1) > ' ')
		q--;		// find start of name to return
	cb = min((int) (p - q), sizeof(achObject) - 1);
	memcpy(achObject, q, cb);
	achObject[cb] = '\0';
	*p = ' ';
	return &achObject[0];
}


char *object_end(DATA *data, char *pszIn, char *pszEnd)
{
	char *p;
	int nDepth;
	char ch;

	nDepth = 1;
	p = pszIn;
	while (nDepth > 0) {
		if ((ch = *p++) == ':' || ch == '#') {  // skip values and comments
			while (*p++ >= ' ') {
				if (p >= pszEnd) {
					report(data, pszIn, "Missing brace }");
					return NULL;
				}
			}
		} else {
			if (ch == '{') {
				nDepth++;
			} else if (ch == '}') {
				nDepth--;
			}
			if (p >= pszEnd) {
				report(data, pszIn, "Missing brace }");
				return NULL;
			}
		}
	}
	return p;
}


void object_text2resource(DATA *data, FILE *hf, char *pszIn, char *pszOut, const CLASSES *pClass)
{
	toolbox_relocatable_object_base *object;
	int *pReloc;
	char *strings;
	int cb, ref;

	object = (toolbox_relocatable_object_base *) pszOut;
	object->rf_obj.class_no = pClass->class_no;
	put_objects(data, pszIn, 0, (char *) &object->rf_obj, ObjectHeaderList, ELEMENTS(ObjectHeaderList));
	object->rf_obj.body_size = pClass->t2o(data, pszIn, object);

	strings = (char *) (object + 1) + object->rf_obj.body_size;
	if ((ref = data->StringTable.ref) == 0) {
		object->string_table_offset = -1;
	} else {
		object->string_table_offset = (int) ((char *) strings - (char *) object);
		memcpy(strings, data->StringTable.pstr, ref);
		strings += ALIGN(ref);
	}
	if ((ref = data->MessageTable.ref) == 0) {
		object->message_table_offset = -1;
	} else {
		object->message_table_offset = (int) ((char *) strings - (char *) object);
		memcpy(strings, data->MessageTable.pstr, ref);
		strings += ALIGN(ref);
	}
	if (data->RelocTable.ref == 0) {
		object->relocation_table_offset = -1;
	} else {
		object->relocation_table_offset = (int) ((char *) strings - (char *) object);
		pReloc = (int *) strings;
		*pReloc++ = data->RelocTable.ref;
		cb = data->RelocTable.ref *sizeof(RELOC);
		memcpy(pReloc, data->RelocTable.pReloc, cb);
		strings = (char *) pReloc;
		strings += cb;
	}

	object->rf_obj.size = object->relocation_table_offset - (sizeof(toolbox_relocatable_object_base) - sizeof(object->rf_obj));
	object->rf_obj.header_size = sizeof(object->rf_obj);
	cb = (int) ((char *) strings - (char *) object);
	fwrite(pszOut, cb, 1, hf);
	memset(pszOut, 0, cb);
}


void object_resource2text(DATA *data, FILE *hf, toolbox_relocatable_object_base *object, object2text o2t)
{
  // Check string, message and relocation table are in this order:
  int nextMinOffset = offsetof(toolbox_relocatable_object_base, rf_obj)
                        + object->rf_obj.header_size
                        + object->rf_obj.body_size;
  if (object->string_table_offset != -1)
    {
      if (object->string_table_offset != nextMinOffset)
        {
          error(data, "Unexpected string table offset (is 0x%x, expected 0x%x)", object->string_table_offset, nextMinOffset);
          return;
        }
    }
  if (object->message_table_offset != -1)
    {
      if (object->message_table_offset < nextMinOffset)
        {
          error(data, "Unexpected message table offset (is 0x%x, expected to be bigger or equal than 0x%x)", object->message_table_offset, nextMinOffset);
          return;
        }
      nextMinOffset = object->message_table_offset;
    }
  if (object->relocation_table_offset != -1)
    {
      if (object->relocation_table_offset < nextMinOffset)
        {
          error(data, "Unexpected relocation table offset (is 0x%x, expected to be bigger or equal than 0x%x)", object->relocation_table_offset, nextMinOffset);
          return;
        }
      if (object->relocation_table_offset != offsetof(toolbox_relocatable_object_base, rf_obj)
                                               + object->rf_obj.size)
        {
          error(data, "Unexpected relocation table offset (is 0x%x, expected to be 0x%x)", object->relocation_table_offset, offsetof(toolbox_relocatable_object_base, rf_obj) + object->rf_obj.size);
          return;
        }
    }
  nextMinOffset = offsetof(toolbox_relocatable_object_base, rf_obj) + object->rf_obj.size;

  TOOLBOXSMTABLE stringMessageTable;
  if (object->string_table_offset == -1)
    {
      stringMessageTable.stringTableP = NULL;
      stringMessageTable.stringTableSize = 0;
    }
  else
    {
      stringMessageTable.stringTableP = (const char *)object + object->string_table_offset;
      stringMessageTable.stringTableSize = ((object->message_table_offset == -1) ? nextMinOffset : object->message_table_offset) - object->string_table_offset;
    }
  if (object->message_table_offset == -1)
    {
      stringMessageTable.messageTableP = NULL;
      stringMessageTable.messageTableSize = 0;
    }
  else
    {
      stringMessageTable.messageTableP = (const char *)object + object->message_table_offset;
      stringMessageTable.messageTableSize = nextMinOffset - object->message_table_offset;
    }

  get_objects(data, hf, &stringMessageTable, (const char *)&object->rf_obj, ObjectHeaderList, ELEMENTS(ObjectHeaderList), 1);
  o2t(data, hf, &object->rf_obj, &stringMessageTable);
}
