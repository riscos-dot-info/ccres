/* _Object.c
   $Id: _Object.c,v 1.4 2004/12/01 23:18:38 joty Exp $

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

#include <string.h>

#include <OSLib/ddeutils.h>

static FLAGS ObjectHeaderFlags[] = {
	{toolbox_OBJECT_CREATE_ON_LOAD, "toolbox_OBJECT_CREATE_ON_LOAD"},
	{toolbox_OBJECT_SHOW_ON_CREATE, "toolbox_OBJECT_SHOW_ON_CREATE"},
	{toolbox_OBJECT_SHARED        , "toolbox_OBJECT_SHARED"        },
	{toolbox_OBJECT_ANCESTOR      , "toolbox_OBJECT_ANCESTOR"      }
};

static OBJECTLIST ObjectHeaderList[] = {
	{iol_FLAGS, "header_flags:", offsetof(toolbox_resource_file_object_base, flags),   ObjectHeaderFlags, ELEMENTS(ObjectHeaderFlags)},
	{iol_INT,   "version:",      offsetof(toolbox_resource_file_object_base, version), NULL,              0                          },
	{iol_PSTR,  "object_name:",  offsetof(toolbox_resource_file_object_base, name),    NULL,              0                          }
};

static FLAGS WimpIconFlags[] = {
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

static FLAGS WimpIconType[] = {
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

static FLAGS OsColours[] = {
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

static FLAGS WimpColour[] = {
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

static FLAGS CmpFlags[] = {
	{toolbox_NULL_COMPONENT, "toolbox_NULL_COMPONENT"},
	{toolbox_WINDOW_FOCUS  , "toolbox_WINDOW_FOCUS"  }
};


void report(PDATA data, PSTR ptr, PSTR pszFmt, ...)
{
	va_list list;
	PSTR p;
	int nRow;
	char achError[1024];

	va_start(list, pszFmt);
	vsprintf(achError, pszFmt, list);
	va_end(list);
	for (nRow = 1, p = data->pszIn; p < ptr; p++) {
		if (*p < ' ') {
			nRow++;
		}
	}
	if (!data->fThrowback) {
		ddeutils_throwback_start();
		data->fThrowback = TRUE;
	}
	ddeutils_throwback_send(ddeutils_THROWBACK_INFO_DETAILS, data->achTextFile, nRow, ddeutils_SEVERITY_ERROR, achError);
}


static PSTR parse(PDATA data, PSTR pszIn, PSTR pszEntry)
{
	PSTR p, pszEnd;
	int cb;
	char ch, ch0;

	ch0 = *pszEntry++;		// it's faster to check first char before calling __strnicmp
	cb = strlen(pszEntry);
	p = pszIn;
	pszEnd = &data->pszIn[data->cbIn] - cb;
	while ((ch = *p++) != '}' && p < pszEnd) {
		if (ch == ':' || ch == '#') {  // skip values and comments
			while (*p++ >= ' ' && p < pszEnd) ;
		} else if (ch == ch0 && __strnicmp(p, pszEntry, cb) == 0) {
			return(p + cb);
		}
	}
	report(data, pszIn, "Missing entry '%s'", --pszEntry);
	return(NULL);
}


static BOOL add_to_reloc_table(PRELOCTABLE pRelocTable, int nEntry, int nTable)
{
	PRELOC pReloc;
	int nReloc;

	nReloc = pRelocTable->ref++;
	pRelocTable->pReloc[nReloc].offset = nEntry;
	pRelocTable->pReloc[nReloc].type = nTable;

	if (pRelocTable->ref >= (pRelocTable->max - sizeof(RELOC))) {
		nReloc = pRelocTable->max * 3 / 2;
		if ((pReloc = MyAlloc(nReloc * sizeof(RELOC))) == NULL) {
			return(FALSE);
		}
		memcpy((PSTR) pReloc, (PSTR) pRelocTable->pReloc, pRelocTable->max * sizeof(RELOC));
		MyFree(pRelocTable->pReloc);
		pRelocTable->pReloc = pReloc;
		pRelocTable->max = nReloc;
	}
	return(TRUE);
}


// may be trailing spaces after closing quote
// may be no quotes at all
// report imbalance
static PSTR remove_quotes(PDATA data, PSTR pszEntry)
{
	int cb, cbTerm;
	char ch;

	cbTerm = strlen(pszEntry);
	if (pszEntry[0] == '\"') {
		for (cb = cbTerm - 1; cb > 0; cb--) {
			if ((ch = pszEntry[cb]) == '\"') {
				pszEntry[cbTerm] = ' ';		// keep line numbering correct for report()
				pszEntry[cb] = '\0';
				return(pszEntry + 1);
			} else if (ch != ' ') {
				break;						// unmatched quotes
			}
		}
	} else if (pszEntry[cbTerm - 1] != '\"') {
// if there are no quotes, how do we know if there are supposed to be trailing spaces or not?
// on balance probably best to leave them in?
		return(pszEntry);
	}
	report(data, pszEntry, "Unmatched quotes '%s'", pszEntry);
	return(pszEntry);
}


static BOOL put_string(PDATA data, PSTR pszIn, int nOffset, PSTR object, PSTRINGLIST StringList)
{
	PSTRINGTABLE pTable;
	PINT pInt;
	PSTR pszEntry, pszLimit, pstr;
	int cb, cbEntry, cbLimit, nTable;

	if (StringList->nTable == iol_STRING) {
		pTable = &data->StringTable;
		nTable = toolbox_RELOCATE_STRING_REFERENCE;
	} else if (StringList->nTable == iol_MSG) {
		pTable = &data->MessageTable;
		nTable = toolbox_RELOCATE_MSG_REFERENCE;
	} else {
		error("Unexpected string table type (%d)", StringList->nTable);
		return(FALSE);
	}
	cbEntry = 0;
LOG(("\nLookup: %s", StringList->pszEntry));
	if ((pszEntry = parse(data, pszIn, StringList->pszEntry)) != NULL) {
		cbEntry = my_strcpy(&pTable->pstr[pTable->ref], remove_quotes(data, pszEntry));
	}
LOG(("'%s' (%d)", &pTable->pstr[pTable->ref], cbEntry));
	cbLimit = 0;
	if (StringList->pszLimit != NULL && (pszLimit = parse(data, pszIn, StringList->pszLimit)) != NULL) {
		if (pszLimit[0] == '*') {
			cbLimit = (cbEntry == 0) ? 0 : (cbEntry + 1);
		} else {
			cbLimit = __atoi(&pszLimit);
			cbLimit = (cbLimit > 0 && cbLimit <= cbEntry) ? (cbEntry + 1) : cbLimit;
		}
	}
	pInt = (PINT) &object[StringList->nEntry];
	*pInt = (cbEntry > 0 || cbLimit > 0) ? pTable->ref : -1;
	add_to_reloc_table(&data->RelocTable, StringList->nEntry + nOffset, nTable);
	if (StringList->pszLimit != NULL) {
		pInt = (PINT) &object[StringList->nLimit];
		*pInt = cbLimit;
	}
//	if (cbEntry > 0) {
	if (cbEntry > 0 || cbLimit > 0) {
		pTable->ref += cbEntry + 1;
		if (pTable->ref > (pTable->max - 256)) {
			cb = pTable->max * 3 / 2;
			if ((pstr = MyAlloc(cb)) == NULL) {
				return(FALSE);
			}
			memcpy(pstr, pTable->pstr, pTable->max);
			MyFree(pTable->pstr);
			pTable->pstr = pstr;
			pTable->max = cb;
		}
	}
	return(TRUE);
}


static BOOL put_tstring(PDATA data, PSTR pszIn, int nOffset, PSTR object, PSTRINGLIST StringList)
{
	PSTRINGTABLE pTable;
	PINT pInt;
	PSTR pszEntry, pszLimit, pstr;
	int cb, cbEntry, cbLimit, nTable;

	pTable = &data->StringTable;
	nTable = toolbox_RELOCATE_STRING_REFERENCE;

	cbEntry = 0;
	if ((pszEntry = parse(data, pszIn, StringList->pszEntry)) != NULL) {
		pstr = &pTable->pstr[pTable->ref];
		cbEntry = my_strcpy(pstr, remove_quotes(data, pszEntry));
		pstr[cbEntry] = '\r';
	}
	cbLimit = 0;
	if (StringList->pszLimit != NULL && (pszLimit = parse(data, pszIn, StringList->pszLimit)) != NULL) {
		if (pszLimit[0] == '*') {
			cbLimit = (cbEntry == 0) ? 0 : (cbEntry + 1);
		} else {
			cbLimit = __atoi(&pszLimit);
			cbLimit = (cbLimit > 0 && cbLimit <= cbEntry) ? (cbEntry + 1) : cbLimit;
		}
	}
	pInt = (PINT) &object[StringList->nEntry];
	*pInt = (cbEntry > 0 || cbLimit > 0) ? (pTable->ref - nOffset) : -1;
	if (StringList->pszLimit != NULL) {
		pInt = (PINT) &object[StringList->nLimit];
		*pInt = cbLimit;
	}
	if (cbEntry > 0) {
		pTable->ref += cbEntry + 1;
		if (pTable->ref > (pTable->max - 256)) {
			cb = pTable->max * 3 / 2;
			if ((pstr = MyAlloc(cb)) == NULL) {
				return(FALSE);
			}
			memcpy(pstr, pTable->pstr, pTable->max);
			MyFree(pTable->pstr);
			pTable->pstr = pstr;
			pTable->max = cb;
		}
	}
	return(TRUE);
}


static PSTR string_from_table(PSTR pszTable, int ref)
{
	static PSTR pszNull = "";
	PSTR pstr, p;
	char ch;

	if (pszTable == NULL || ref < 0) {
		return(pszNull);
	}
// modified to handle Template \n terminated strings
//	return(&pszTable[ref]);
	pstr = p = &pszTable[ref];
	while ((ch = *p++) != '\0' && ch != '\n' && ch != '\r') ;
	*(p - 1) = '\0';
	return(pstr);
}


static void get_string(FILE * hf, PSTR pszStringTable, PSTR pszMessageTable, PSTR object, PSTRINGLIST StringList, PSTR pszIndent)
{
	PINT pInt;
	PSTR pstr, qstr, string_table;
	int cbLimit, ref;

	if (StringList->nTable == iol_STRING) {
		string_table = pszStringTable;
	} else if (StringList->nTable == iol_MSG) {
		string_table = pszMessageTable;
	} else {
		error("Unexpected string table type (%d)", StringList->nTable);
		return;
	}
	pInt = (PINT) &object[StringList->nEntry];
LOG(("get_string(object[%x]) = %x", StringList->nEntry, *pInt));
	pstr = "";
	if ((ref = *pInt) >= 0 && (qstr = string_from_table(string_table, ref)) != NULL) {
		pstr = qstr;
	}
LOG(("entry=%s\"%s\"\n", StringList->pszEntry, pstr));
	fprintf(hf, "%s\"%s\"\n", StringList->pszEntry, pstr);
	if (StringList->pszLimit != NULL) {
		pInt = (PINT) &object[StringList->nLimit];
		cbLimit = *pInt;
		fputs(pszIndent, hf);
		if (cbLimit > (strlen(pstr) + 1)) {
			fprintf(hf, "%s%d\n", StringList->pszLimit, cbLimit);
		} else {
			fprintf(hf, "%s*\n", StringList->pszLimit);
		}
	}
}


static void get_tstring(FILE * hf, PSTR pszStringTable, PSTR object, PSTRINGLIST StringList, PSTR pszIndent)
{
	PINT pInt;
	PSTR pstr, qstr;
	int cbLimit, ref;

	pInt = (PINT) &object[StringList->nEntry];
LOG(("get_tstring(object[%x]) = %x", StringList->nEntry, *pInt));
	pstr = "";
	if ((ref = *pInt) >= 0 && (qstr = string_from_table(pszStringTable, ref)) != NULL) {
		pstr = qstr;
	}
LOG(("entry=%s\"%s\"\n", StringList->pszEntry, pstr));
	fprintf(hf, "%s\"%s\"\n", StringList->pszEntry, pstr);
	if (StringList->pszLimit != NULL) {
		pInt = (PINT) &object[StringList->nLimit];
		cbLimit = *pInt;
		fputs(pszIndent, hf);
		if (cbLimit > (strlen(pstr) + 1)) {
			fprintf(hf, "%s%d\n", StringList->pszLimit, cbLimit);
		} else if (cbLimit > 0) {
			fprintf(hf, "%s*\n", StringList->pszLimit);
		} else {
			fprintf(hf, "%s\n", StringList->pszLimit);
		}
	}
}


static bits put_flags(PDATA data, PSTR pstrFlags, PFLAGS pFlags, int nFlags)
{
	PSTR p;
	bits f;
	int n, cb;
	char ch;

	f = 0;
	p = pstrFlags;
	while (*p != '\0') {
		while ((ch = *p) > ' ' && ch != '|') p++;
		cb = p - pstrFlags;
		for (n = 0; n < nFlags; n++) {
			if (__strnicmp(pstrFlags, pFlags[n].pstr, cb) == 0 && strlen(pFlags[n].pstr) == cb) {
				f |= pFlags[n].flag;
				goto put_flags_next_flag;
			}
		}
		report(data, pstrFlags, "Unknown flag '%.*s'", cb, pstrFlags);

put_flags_next_flag:

		while ((ch = *p) == ' ' || ch == '|') p++;
		pstrFlags = p;
	}
	return(f);
}


static void get_flags(FILE * hf, PSTR pszFlags, bits fFlags, PFLAGS pFlags, int nFlags)
{
	PSTR pszOr;
	int cb, n;
	char achBuff[1120];		// =32 x 32 character flags

	cb = sprintf(achBuff, "%s", pszFlags);
	pszOr = "";
	for (n = 0; n < nFlags && fFlags != 0; n++) {
		if ((fFlags & pFlags[n].flag) == pFlags[n].flag) {
			cb += sprintf(&achBuff[cb], "%s%s", pszOr, pFlags[n].pstr);
			pszOr = " | ";
			fFlags ^= pFlags[n].flag;
		}
	}
	/* Louzy error message but I don't know yet how I can improve it. */
	if (fFlags != 0)
		error("Not all bits are covered");
	achBuff[cb++] = '\n';
	fwrite(achBuff, cb, 1, hf);
}


// fInt - value may be a #defined variable or a nunber (hex or decimal)
static int put_enum(PDATA data, PSTR pstrFlags, PFLAGS pFlags, int nFlags, BOOL fInt)
{
	int n;

	if (*pstrFlags > ' ') {
		for (n = 0; n < nFlags; n++) {
			if (strcmp(pstrFlags, pFlags[n].pstr) == 0) {
				return(pFlags[n].flag);
			}
		}
		if (fInt) {
			return(__atoi(&pstrFlags));
		}
		report(data, pstrFlags, "Unknown variable '%s'", pstrFlags);
	}

	return(0);
}


// fInt - value may be a #defined variable or a nunber (hex or decimal)
static void get_enum(FILE * hf, PSTR pszFlags, int fFlags,  PFLAGS pFlags, int nFlags, BOOL fInt)
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


static bits put_iflags(PDATA data, PSTR pstrFlags)
{
	PFLAGS pFlags;
	PSTR p;
	bits f;
	int n, cb, nFlags;
	char ch;

	f = 0;
	p = pstrFlags;
	while (*p != '\0') {
		while ((ch = *p) > ' ' && ch != '|') p++;
		cb = p - pstrFlags;
		pFlags = WimpIconFlags;
		nFlags = ELEMENTS(WimpIconFlags);
		for (n = 0; n < nFlags; n++) {
			if (__strnicmp(pstrFlags, pFlags[n].pstr, cb) == 0 && strlen(pFlags[n].pstr) == cb) {
				f |= pFlags[n].flag;
				goto put_iflags_next_flag;
			}
		}
		pFlags = WimpIconType;
		nFlags = ELEMENTS(WimpIconType);
		for (n = 0; n < nFlags; n++) {
			if (strcmp(pstrFlags, pFlags[n].pstr) == 0) {
				f |= pFlags[n].flag;
				goto put_iflags_next_flag;
			}
		}
		report(data, pstrFlags, "Unknown flag '%.*s'", cb, pstrFlags);

put_iflags_next_flag:

		while ((ch = *p) == ' ' || ch == '|') p++;
		pstrFlags = p;
	}

	return(f);
}


static void get_iflags(FILE * hf, PSTR pszFlags, bits fFlags)
{
	PFLAGS pFlags;
	PSTR pszOr;
	int cb, n, nFlags;
	char achBuff[1120];		// =32 x 32 character flags

	pFlags = WimpIconFlags;
	nFlags = ELEMENTS(WimpIconFlags);
	cb = sprintf(achBuff, "%s", pszFlags);
	pszOr = "";
	for (n = 0; n < nFlags; n++) {
		if ((fFlags & pFlags[n].flag) == pFlags[n].flag) {
			cb += sprintf(&achBuff[cb], "%s%s", pszOr, pFlags[n].pstr);
			pszOr = " | ";
		}
	}
	pFlags = WimpIconType;
	nFlags = ELEMENTS(WimpIconType);
	fFlags &= wimp_ICON_BUTTON_TYPE;
	for (n = 0; n < nFlags; n++) {
		if (fFlags == pFlags[n].flag) {
			cb += sprintf(&achBuff[cb], "%s%s", pszOr, pFlags[n].pstr);
			break;
		}
	}
	achBuff[cb++] = '\n';
	fwrite(achBuff, cb, 1, hf);
}


static void put_box(PDATA data, PSTR pstr, os_box * box)
{
	PINT pi;
	int n;

LOG(("put_box %s", pstr));
	pi = (PINT) box;
	for (n = 0; n < 4; n++) {
		pi[n] = Eval(data, &pstr);
		pstr++;
	}
LOG(("(%d, %d, %d, %d)", pi[0], pi[1], pi[2], pi[3]));
}


static void get_box(FILE * hf, PSTR pszBox, os_box * bbox)
{
	fprintf(hf, "%s%d,%d,%d,%d\n", pszBox, bbox->x0, bbox->y0, bbox->x1, bbox->y1);
}


static void put_coord(PDATA data, PSTR pstr, os_coord * coord)
{
	PINT pi;
	int n;

LOG(("put_coord %s", pstr));
	pi = (PINT) coord;
	for (n = 0; n < 2; n++) {
		pi[n] = Eval(data, &pstr);
//LOG(("New pstr = '%s'", pstr));
		pstr++;
	}
LOG(("(%d, %d)", pi[0], pi[1]));
}


static void get_coord(FILE * hf, PSTR pszCoord, os_coord * coord)
{
	fprintf(hf, "%s%d,%d\n", pszCoord, coord->x, coord->y);
}


static void put_pstr(PDATA data, PSTR pstr, PSTR pszEntry, int nChars)
{
	int cb;

	pszEntry = remove_quotes(data, pszEntry);
	if (nChars != 0 && strlen(pszEntry) == nChars) {
		memcpy(pstr, pszEntry, nChars);
	} else {
		if (data->nFiletypeOut == osfile_TYPE_TEMPLATE) {
			cb = my_strcpy0d(pstr, pszEntry);
			pstr[cb] = '\r';
		} else {
			strcpy(pstr, pszEntry);
		}
	}
}


static void get_pstr(FILE * hf, PSTR pszEntry, PSTR pszObject, int nEntry, int nChars)
{
	PSTR pszBuff;

LOG(("get_pstr(%s, %s)", pszEntry, &pszObject[nEntry]));
	pszBuff = NULL;
	if (nChars == 0) {
		fprintf(hf, "%s\"%s\"\n", pszEntry, string_from_table(pszObject, nEntry));
	} else {
		if ((pszBuff = MyAlloc(nChars + 1)) == NULL) {
			error("Not enough memory to get %s", pszEntry);
		} else {
			my_strncpy0d(pszBuff, &pszObject[nEntry], nChars);
LOG(("b(%s, %s)", pszBuff, pszBuff));
			fprintf(hf, "%s\"%s\"\n", pszEntry, pszBuff);
			MyFree(pszBuff);
		}
	}
}


void put_objects(PDATA data, PSTR pszIn, int nOffset, PSTR Object, POBJECTLIST ObjectList, int nObjects)
{
PBITS pBits;
PINT pInt;
PSHORT pShort;
PBYTE pByte;
PSTR pszEntry;
int n;

LOG(("put_objects(offset:%x)", nOffset));
for (n = 0; n < nObjects; n++, ObjectList++)
  {
LOG(("Item:%s\tOffset:%d\tType:%d", ObjectList->pszEntry, ObjectList->nEntry, ObjectList->nTable));
  switch (ObjectList->nTable)
    {
    case iol_MSG:
    case iol_STRING:
      put_string(data, pszIn, nOffset, Object, (PSTRINGLIST) ObjectList);
      break;
    case iol_TSTRING:
      put_tstring(data, pszIn, nOffset, Object, (PSTRINGLIST) ObjectList);
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
            pBits = (PBITS) &Object[ObjectList->nEntry];
            *pBits = put_flags(data, pszEntry, (PFLAGS) ObjectList->pData, ObjectList->nData);
            break;
          case iol_IFLAGS:
            pBits = (PBITS) &Object[ObjectList->nEntry];
            *pBits = put_iflags(data, pszEntry);
            break;
          case iol_BFLAGS:
            pByte = (PBYTE) &Object[ObjectList->nEntry];
            *pByte = (byte) put_flags(data, pszEntry, (PFLAGS) ObjectList->pData, ObjectList->nData);
            break;
          case iol_ENUM:
            pInt = (PINT) &Object[ObjectList->nEntry];
            *pInt = put_enum(data, pszEntry, (PFLAGS) ObjectList->pData, ObjectList->nData, FALSE);
            break;
          case iol_CMP:
            pInt = (PINT) &Object[ObjectList->nEntry];
            *pInt = put_enum(data, pszEntry, CmpFlags, ELEMENTS(CmpFlags), TRUE);
            break;
          case iol_OSCOL:
            pInt = (PINT) &Object[ObjectList->nEntry];
            *pInt = put_enum(data, pszEntry, OsColours, ELEMENTS(OsColours), FALSE);
            break;
          case iol_BOX:
            put_box(data, pszEntry, (os_box *) &Object[ObjectList->nEntry]);
            break;
          case iol_BITS:
            pBits = (PBITS) &Object[ObjectList->nEntry];
            if (*pszEntry < ' ')
              *pBits = (ObjectList->nData == bits_ACTION) ? ~0 : 0;
            else
              *pBits = (ObjectList->nData == bits_EVAL) ? Eval(data, &pszEntry) : __atoi(&pszEntry);
            break;
          case iol_INT:
            pInt = (PINT) &Object[ObjectList->nEntry];
            *pInt = __atoi(&pszEntry);
            break;
          case iol_SHORT:
            pShort = (PSHORT) &Object[ObjectList->nEntry];
            *pShort = (short) __atoi(&pszEntry);
            break;
          case iol_BYTE:
            pByte = (PBYTE) &Object[ObjectList->nEntry];
            *pByte = (byte) __atoi(&pszEntry);
            break;
          case iol_COORD:
            put_coord(data, pszEntry, (os_coord *) &Object[ObjectList->nEntry]);
            break;
          case iol_SPRITE:
            pBits = (PBITS) &Object[ObjectList->nEntry];
            *pBits = __atoi(&pszEntry);
            add_to_reloc_table(&data->RelocTable, ObjectList->nEntry, toolbox_RELOCATE_SPRITE_AREA_REFERENCE);
            break;
          case iol_PSTR:
            put_pstr(data, &Object[ObjectList->nEntry], pszEntry, ObjectList->nData);
            break;
          case iol_ESG:
            pBits = (PBITS) &Object[ObjectList->nEntry];
            *pBits |= (__atoi(&pszEntry) << wimp_ICON_ESG_SHIFT);
            break;
          case iol_WCOL:
            pByte = (PBYTE) &Object[ObjectList->nEntry];
            *pByte = (byte) put_enum(data, pszEntry, WimpColour, ELEMENTS(WimpColour), FALSE);
            break;
          case iol_BCOLS:
            pBits = (PBITS) &Object[ObjectList->nEntry];
            *pBits |= put_enum(data, pszEntry, WimpColour, ELEMENTS(WimpColour), FALSE) << wimp_ICON_FG_COLOUR_SHIFT;
            if ((pszEntry = parse(data, pszIn, ObjectList->pData)) != NULL)
              *pBits |= put_enum(data, pszEntry, WimpColour, ELEMENTS(WimpColour), FALSE) << wimp_ICON_BG_COLOUR_SHIFT;
            break;
          default:
            error("Unknown iol_ value (%d)", ObjectList->nTable);
            break;
          }
        }
      break;
    }
  }
LOG(("-----"));
}


void get_objects(FILE * hf, PSTR pszStringTable, PSTR pszMessageTable, PSTR Object, POBJECTLIST ObjectList, int nObjects, int nIndent)
{
PSTR pszIndent;
PBITS pBits;
PINT pInt;
PSHORT pShort;
PBYTE pByte;
int i, n;

pszIndent = (nIndent == 1) ? "  " : (nIndent == 2) ? "    " : "";
LOG(("get_objects"));
for (n = 0; n < nObjects; n++, ObjectList++)
  {
LOG(("Item:%s\tOffset:%d\tType:%d", ObjectList->pszEntry, ObjectList->nEntry, ObjectList->nTable));
  if (ObjectList->nTable != iol_OBJECT)
    { // do nothing for res2text
    fputs(pszIndent, hf);
    switch (ObjectList->nTable)
      {
      case iol_MSG:
      case iol_STRING:
        get_string(hf, pszStringTable, pszMessageTable, Object, (PSTRINGLIST) ObjectList, pszIndent);
        break;
      case iol_TSTRING:
        get_tstring(hf, pszStringTable, Object, (PSTRINGLIST) ObjectList, pszIndent);
        break;
      case iol_FLAGS:
        pBits = (PBITS) &Object[ObjectList->nEntry];
        get_flags(hf, ObjectList->pszEntry, *pBits, (PFLAGS) ObjectList->pData, ObjectList->nData);
        break;
      case iol_IFLAGS:
        pBits = (PBITS) &Object[ObjectList->nEntry];
        get_iflags(hf, ObjectList->pszEntry, *pBits);
        break;
      case iol_BFLAGS:
        pByte = (PBYTE) &Object[ObjectList->nEntry];
        get_flags(hf, ObjectList->pszEntry, *pByte, (PFLAGS) ObjectList->pData, ObjectList->nData);
        break;
      case iol_ENUM:
        pInt = (int *) &Object[ObjectList->nEntry];
        get_enum(hf, ObjectList->pszEntry, *pInt, (PFLAGS) ObjectList->pData, ObjectList->nData, FALSE);
        break;
      case iol_CMP:
        pInt = (int *) &Object[ObjectList->nEntry];
        get_enum(hf, ObjectList->pszEntry, *pInt, CmpFlags, ELEMENTS(CmpFlags), TRUE);
        break;
      case iol_OSCOL:
        pInt = (int *) &Object[ObjectList->nEntry];
        get_enum(hf, ObjectList->pszEntry, *pInt, OsColours, ELEMENTS(OsColours), FALSE);
        break;
      case iol_BOX:
        get_box(hf, ObjectList->pszEntry, (os_box *) &Object[ObjectList->nEntry]);
        break;
      case iol_BITS:
        pBits = (PBITS) &Object[ObjectList->nEntry];
        if (*pBits == ~0 && ObjectList->nData == bits_ACTION)
          fprintf(hf, "%s\n", ObjectList->pszEntry);
        else
          fprintf(hf, "%s&%x\n", ObjectList->pszEntry, *pBits);
        break;
      case iol_INT:
        pInt = (PINT) &Object[ObjectList->nEntry];
        fprintf(hf, "%s%d\n", ObjectList->pszEntry, *pInt);
        break;
      case iol_SHORT:
        pShort = (PSHORT) &Object[ObjectList->nEntry];
        fprintf(hf, "%s%d\n", ObjectList->pszEntry, (int) *pShort);
        break;
      case iol_BYTE:
        pByte = (PBYTE) &Object[ObjectList->nEntry];
        fprintf(hf, "%s%d\n", ObjectList->pszEntry, (bits) *pByte);
        break;
      case iol_COORD:
        get_coord(hf, ObjectList->pszEntry, (os_coord *) &Object[ObjectList->nEntry]);
        break;
      case iol_SPRITE:
        pBits = (PBITS) &Object[ObjectList->nEntry];
        fprintf(hf, "%s&%x\n", ObjectList->pszEntry, *pBits);
        break;
      case iol_PSTR:
        get_pstr(hf, ObjectList->pszEntry, Object, ObjectList->nEntry, ObjectList->nData);
        break;
      case iol_ESG:
        pBits = (PBITS) &Object[ObjectList->nEntry];
        fprintf(hf, "%s%d\n", ObjectList->pszEntry, (*pBits & wimp_ICON_ESG) >> wimp_ICON_ESG_SHIFT);
        break;
      case iol_WCOL:
        pByte = (PBYTE) &Object[ObjectList->nEntry];
        get_enum(hf, ObjectList->pszEntry, *pByte, WimpColour, ELEMENTS(WimpColour), FALSE);
        break;
      case iol_BCOLS:
        pBits = (PBITS) &Object[ObjectList->nEntry];
        i = (*pBits & wimp_ICON_FG_COLOUR) >> wimp_ICON_FG_COLOUR_SHIFT;
        get_enum(hf, ObjectList->pszEntry, i, WimpColour, ELEMENTS(WimpColour), FALSE);
        fputs(pszIndent, hf);
        i = (*pBits & wimp_ICON_BG_COLOUR) >> wimp_ICON_BG_COLOUR_SHIFT;
        get_enum(hf, ObjectList->pData, i, WimpColour, ELEMENTS(WimpColour), FALSE);
        break;
      default:
        error("Unknown iol_ value (%d)", ObjectList->nTable);
        break;
      }
    }
  }
LOG(("-----"));
}


#define min(a,b) ((a)<(b)?(a):(b))
// Iconbar {
// returns "Iconbar\0" and sets pszIn to byte after '{'
PSTR next_object(PSTR * pszIn, PSTR pszEnd)
{
	PSTR p, q;
	int cb;
	char ch;
	static char achObject[48];

	p = *pszIn;
	while ((ch = *p++) != '{') {
		if (ch == '}' || p >= pszEnd) {
			return(NULL);
		}
	}
// p points to char *after* the brace
	*pszIn = p--;
	while (*(p - 1) <= ' ') p--;	// skip trailing spaces
	*p = '\0';
	q = p;
	while (*(q - 1) > ' ') q--;		// find start of name to return
	cb = min((int) (p - q), sizeof(achObject) - 1);
	memcpy(achObject, q, cb);
	achObject[cb] = '\0';
	*p = ' ';
	return(&achObject[0]);
}


PSTR object_end(PDATA data, PSTR pszIn, PSTR pszEnd)
{
	PSTR p;
	int nDepth;
	char ch;

	nDepth = 1;
	p = pszIn;
	while (nDepth > 0) {
		if ((ch = *p++) == ':' || ch == '#') {  // skip values and comments
			while (*p++ >= ' ') {
				if (p >= pszEnd) {
					report(data, pszIn, "Missing brace }");
					return(NULL);
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
				return(NULL);
			}
		}
	}
	return(p);
}


void _object(FILE * hf, PDATA data, PSTR pszIn, PSTR pszOut, PCLASSES pClass)
{
	toolbox_relocatable_object_base * object;
	PINT pReloc;
	PSTR strings;
	int cb, ref;

	object = (toolbox_relocatable_object_base *) pszOut;
	object->rf_obj.class_no = pClass->class_no;
	put_objects(data, pszIn, 0, (PSTR) &object->rf_obj, ObjectHeaderList, ELEMENTS(ObjectHeaderList));
	object->rf_obj.body_size = pClass->t2o(data, pszIn, object);

	strings = (PSTR) (object + 1) + object->rf_obj.body_size;
	if ((ref = data->StringTable.ref) == 0) {
		object->string_table_offset = -1;
	} else {
		object->string_table_offset = (int) ((PSTR) strings - (PSTR) object);
		memcpy(strings, data->StringTable.pstr, ref);
		strings += ALIGN(ref);
	}
	if ((ref = data->MessageTable.ref) == 0) {
		object->message_table_offset = -1;
	} else {
		object->message_table_offset = (int) ((PSTR) strings - (PSTR) object);
		memcpy(strings, data->MessageTable.pstr, ref);
		strings += ALIGN(ref);
	}
	if (data->RelocTable.ref == 0) {
		object->relocation_table_offset = -1;
	} else {
		object->relocation_table_offset = (int) ((PSTR) strings - (PSTR) object);
		pReloc = (PINT) strings;
		*pReloc++ = data->RelocTable.ref;
		cb = data->RelocTable.ref * sizeof(RELOC);
		memcpy((PSTR) pReloc, (PSTR) data->RelocTable.pReloc, cb);
		strings = (PSTR) pReloc;
		strings += cb;
	}

	object->rf_obj.size = object->relocation_table_offset - (sizeof(toolbox_relocatable_object_base) - sizeof(object->rf_obj));
	object->rf_obj.header_size = sizeof(object->rf_obj);
	cb = (int) ((PSTR) strings - (PSTR) object);
	fwrite(pszOut, cb, 1, hf);
	memset(pszOut, 0, cb);
}


void object(FILE * hf, toolbox_relocatable_object_base * object, object2text o2t)
{
	PSTR pszStringTable, pszMessageTable;

	pszStringTable = pszMessageTable = NULL;
	if (object->string_table_offset != -1) {
		pszStringTable = ((PSTR) object) + object->string_table_offset;
	}
	if (object->message_table_offset != -1) {
		pszMessageTable = ((PSTR) object) + object->message_table_offset;
	}

LOG(("StringTable:%d", object->string_table_offset));
LOG(("MessageTable:%d", object->message_table_offset));

//LOG(("size:%d", template->rf_header.size));
//LOG(("header_size:%d", template->rf_header.header_size));
//LOG(("body_size:%d", template->rf_header.body_size));

	get_objects(hf, pszStringTable, pszMessageTable, (PSTR) &object->rf_obj, ObjectHeaderList, ELEMENTS(ObjectHeaderList), 1);
	o2t(hf, (toolbox_resource_file_object_base *) &object->rf_obj, pszStringTable, pszMessageTable);
}
