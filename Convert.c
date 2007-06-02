/* Convert.c

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

#include <oslib/colourdbox.h>
#include <oslib/colourmenu.h>
#include <oslib/dcs.h>
#include <oslib/fileinfo.h>
#include <oslib/fontdbox.h>
#include <oslib/fontmenu.h>
#include <oslib/iconbar.h>
#include <oslib/menu.h>
#include <oslib/printdbox.h>
#include <oslib/proginfo.h>
#include <oslib/quit.h>
#include <oslib/saveas.h>
#include <oslib/scale.h>
#include <oslib/window.h>

#include "ccres.h"
#include "Convert.h"
#include "Error.h"
#include "Utils.h"

#include "_ColourDbox.h"
#include "_ColourMenu.h"
#include "_DCS.h"
#include "_FileInfo.h"
#include "_FontDbox.h"
#include "_FontMenu.h"
#include "_Iconbar.h"
#include "_Menu.h"
#include "_Object.h"
#include "_PrintDbox.h"
#include "_ProgInfo.h"
#include "_Quit.h"
#include "_SaveAs.h"
#include "_Scale.h"
#include "_Window.h"

static const CLASSES Classes[] = {
	{ColourDbox_ClassSWI, colourdbox_g2t, colourdbox_t2g, "colourdbox_object"},
	{ColourMenu_ClassSWI, colourmenu_g2t, colourmenu_t2g, "colourmenu_object"},
	{DCS_ClassSWI       , dcs_g2t       , dcs_t2g       , "dcs_object"       },
	{FileInfo_ClassSWI  , fileinfo_g2t  , fileinfo_t2g  , "fileinfo_object"  },
	{FontDbox_ClassSWI  , fontdbox_g2t  , fontdbox_t2g  , "fontdbox_object"  },
	{FontMenu_ClassSWI  , fontmenu_g2t  , fontmenu_t2g  , "fontmenu_object"  },
	{Iconbar_ClassSWI   , iconbar_g2t   , iconbar_t2g   , "iconbar_object"   },
	{Menu_ClassSWI      , menu_g2t      , menu_t2g      , "menu_object"      },
	{PrintDbox_ClassSWI , printdbox_g2t , printdbox_t2g , "printdbox_object" },
	{ProgInfo_ClassSWI  , proginfo_g2t  , proginfo_t2g  , "proginfo_object"  },
	{Quit_ClassSWI      , quit_g2t      , quit_t2g      , "quit_object"      },
	{SaveAs_ClassSWI    , saveas_g2t    , saveas_t2g    , "saveas_object"    },
	{Scale_ClassSWI     , scale_g2t     , scale_t2g     , "scale_object"     },
	{Window_ClassSWI    , window_g2t    , window_t2g    , "window_object"    }
};


static BOOL alloc_string_table(PSTRINGTABLE pTable)
{
	int cb;

	pTable->ref = pTable->max = 0;
	cb = 2 * 256 * 256;		// 256 gadgets, 256 chars each, 2 strings per gadget
	if ((pTable->pstr = MyAlloc(cb)) == NULL) {
		return FALSE;
	}
	pTable->max = cb;
	return TRUE;
}


static BOOL alloc_reloc_table(PRELOCTABLE pTable)
{
	int nReloc;

	pTable->ref = pTable->max = 0;
	nReloc = 2 * 256;		// 256 gadgets, 2 strings per gadget
	if ((pTable->pReloc = MyAlloc(nReloc * sizeof(RELOC))) == NULL) {
		return FALSE;
	}
	pTable->max = nReloc;
	return TRUE;
}


static void free_string_table(PSTRINGTABLE pTable)
{
	if (pTable->pstr != NULL) {
		MyFree(pTable->pstr);
	}
	pTable->pstr = NULL;
	pTable->ref = pTable->max = 0;
}


static void free_reloc_table(PRELOCTABLE pTable)
{
	if (pTable->pReloc != NULL) {
		MyFree(pTable->pReloc);
	}
	pTable->pReloc = NULL;
	pTable->ref = pTable->max = 0;
}


static BOOL text2res(PDATA data, PSTR pszOutFile)
{
	toolbox_resource_file_base Hdr;
	FILE * hf;
	PSTR pszIn, pszOut, pszEnd, pszObject;
	int m;
	BOOL fHeader;

	pszIn = data->pszIn;
	pszEnd = data->pszIn + data->cbIn;
	if (memcmp(pszIn, "RESF:1.01", 8) != 0) {
		error(data, "File is not RESF v1.01");
		return FALSE;
	}
	if ((pszOut = MyAlloc(296 * 268 * 3)) == NULL ||		// approx = window with 256 gadgets, 3 strings each
				!alloc_string_table(&data->StringTable) ||
				!alloc_string_table(&data->MessageTable) ||
				!alloc_reloc_table(&data->RelocTable)) {
		error(data, "Unable to allocate necessary memory");
		return FALSE;
	} else if ((hf = fopen(pszOutFile, "wb")) == NULL) {
		error(data, "Unable to create output file '%s'", pszOutFile);
		return FALSE;
	} else {
		data->fThrowback = FALSE;
		Hdr.file_id = RESF;
		Hdr.version = 101;
		Hdr.header_size = -1;
		fHeader = FALSE;
		while ((pszObject = next_object(&pszIn, pszEnd)) != NULL) {
			for (m = 0; m < ELEMENTS(Classes); m++) {
				if (strcasecmp(Classes[m].name, pszObject) == 0 && Classes[m].t2o != NULL) {
					if (!fHeader) {
						Hdr.header_size = sizeof(Hdr);
						fwrite(&Hdr, sizeof(Hdr), 1, hf);
						fHeader = TRUE;
					}
					reset_string_table(&data->StringTable);
					reset_string_table(&data->MessageTable);
					reset_reloc_table(&data->RelocTable);
					object_text2resource(data, hf, pszIn, pszOut, &Classes[m]);
					goto text2res_added;
				}
			}
			error(data, "Don't know how to handle object '%s'", pszObject);

text2res_added:

			if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL) {
				break;
			}
		}
		if (!fHeader) {
			fwrite(&Hdr, sizeof(Hdr), 1, hf);
		}
		fclose(hf);
#ifdef __riscos__
		osfile_set_type(pszOutFile, osfile_TYPE_RESOURCE);
#endif
		if (data->fThrowback)
		  {
		  report_end(data);
		  data->fThrowback = FALSE;
		  }
		free_string_table(&data->StringTable);
		free_string_table(&data->MessageTable);
		free_reloc_table(&data->RelocTable);
		MyFree(pszOut);
	}
	return TRUE;
}


static  BOOL res2text(PDATA data, PSTR pszOutFile)
//      ==========================================
{
PINT relocation_table;
toolbox_resource_file_base * file_hdr;
FILE * hf;
int cb, m;
BOOL fConverted;

file_hdr = (toolbox_resource_file_base *) data->pszIn;
if (file_hdr->file_id != RESF)
  {
  error(data, "Invalid Resource File Header (%x)", file_hdr->file_id);
  return FALSE;
  }
if (file_hdr->version != 101)
  {
  error(data, "Unknown Resource File Version (%x)", file_hdr->version);
  return FALSE;
  }

if ((hf = fopen(pszOutFile, "wb")) == NULL)
  {
  error(data, "Unable to create output file '%s'", pszOutFile);
  return FALSE;
  }

fConverted = FALSE;
fprintf(hf, "RESF:%d.%02d\n", file_hdr->version / 100, file_hdr->version % 100);
if (file_hdr->header_size > 0)
  {
  toolbox_relocatable_object_base * obj, * end;
  obj = (toolbox_relocatable_object_base *) (file_hdr + 1);
  end = (toolbox_relocatable_object_base *) &data->pszIn[data->cbIn];
  do
    {
    fputs("\n", hf);
    for (m = 0; m < ELEMENTS(Classes); m++)
      {
      if (Classes[m].class_no == obj->rf_obj.class_no)
        {
        fprintf(hf, "%s {\n", Classes[m].name);
        object_resource2text(data, hf, obj, Classes[m].o2t);
        fputs("}\n", hf);
        break;
        }
      }
    if (m == ELEMENTS(Classes))
      error(data, "Unknown class '%s' (%x)", obj->rf_obj.name, obj->rf_obj.class_no);

    cb = sizeof(toolbox_relocatable_object_base) - sizeof(obj->rf_obj) + obj->rf_obj.size;
    if (obj->relocation_table_offset != -1)
      {
      relocation_table = (PINT) (((PSTR) obj) + obj->relocation_table_offset);
      cb += sizeof(int) * (1 + 2 * relocation_table[0]);
      }
    obj = (toolbox_relocatable_object_base *) (((PSTR) obj) + cb);
    } while (cb != 0 && obj < end);
  }
fclose(hf);
#ifdef __riscos__
osfile_set_type(pszOutFile, osfile_TYPE_TEXT);
#endif
fConverted = TRUE;

return fConverted;
}

#define template_NO_FONTS 	((bits)-1)
#define template_TYPE_WINDOW	((bits)0x1u)

typedef struct {
	bits font_offset; /* Can be template_NO_FONTS too */
	bits reserved[3];
} template_header;

typedef struct {
	bits offset;
	bits size;
	int type;          /* See template_TYPE_* */
	char name[12];
} template_index;

typedef struct {
	bits x_point_size;
	bits y_point_size;
	char font_name[40];
} template_font_data;

// Returns the number of windows and the maximum number of icons used in
// those windows (not the total number of icons !).
static int window_count(PSTR pszIn, PSTR pszEnd, PINT pi)
{
	int d, w, i, maxi;
	char ch, ch0;

	d = w = i = maxi = 0;
	ch0 = '\0';
	while(pszIn < pszEnd) {
		if ((ch = *pszIn++) == ':' || ch == '\0') {
			ch0 = ch;
		} else if (ch0 == '\0') {
			if (ch == '{') {
				if (d == 0) {
					w++;
					if (i > maxi)
					  maxi = i;
					i = 0;
				} else {
					i++;
				}
				d++;
			} else if (ch == '}') {
				d--;
			}
		}
	}
	*pi = maxi;
	return w;
}

static int icon_count(PSTR pszIn, PSTR pszEnd)
{
	int d, i;
	char ch, ch0;

	d = 1;
	i = 0;
	ch0 = '\0';
	while(pszIn < pszEnd && d > 0) {
		if ((ch = *pszIn++) == ':' || ch == '\0') {
			ch0 = ch;
		} else if (ch0 == '\0') {
			if (ch == '{') {
				i++;
				d++;
			} else if (ch == '}') {
				d--;
			}
		}
	}
	return i;
}


static const OBJECTLIST TemplateFontDataList[] = {
	{iol_BITS,  "x_point_size:",  offsetof(template_font_data, x_point_size), NULL,  0},
	{iol_BITS,  "y_point_size:",  offsetof(template_font_data, y_point_size), NULL,  0},
	{iol_PSTR,  "font_name:",     offsetof(template_font_data, font_name),    NULL, 40}
};


static void get_template_fonts(PDATA data, FILE * hf, template_font_data * font_data, template_font_data * end)
{
	template_font_data temp;
	PSTR pszFrom;

	while (font_data < end) {
		pszFrom = (PSTR) font_data;			// bug in memcpy? without this it does a word-aligned copy
		memcpy(&temp, pszFrom, sizeof(temp));
		fputs("\ntemplate_font_data {\n", hf);
		get_objects(data, hf, NULL, NULL, (const char *) &temp, TemplateFontDataList, ELEMENTS(TemplateFontDataList), 1);
		fputs("}\n", hf);
		font_data++;
	}
}


static const OBJECTLIST TemplateHeaderList[] = {
	{iol_PSTR,  "template_name:",  offsetof(template_index, name), NULL, 12}
};

static BOOL text2template(PDATA data, PSTR pszOutFile)
{
	template_font_data font_data;
	template_header * header;
	template_index * index, * i;
	FILE * hf;
	PINT pTerm;
	PSTR pszIn, pszTemplate, pszOut, pszEnd, pszObject, pszBuff;
	int cb, cbBuff, nWindows, nIcons;
	BOOL fConverted;

	pszIn = data->pszIn;
	pszEnd = data->pszIn + data->cbIn;
	fConverted = FALSE;
	if ((nWindows = window_count(pszIn, pszEnd, &nIcons)) > 0) {
		if ((pszTemplate = MyAlloc(sizeof(template_header) + sizeof(int) +
							   nWindows * (sizeof(template_index) + sizeof(wimp_window) + (nIcons * (256 + sizeof(wimp_icon)))))) == NULL ||
			!alloc_string_table(&data->StringTable)) {      // this is only required so that we can share put_string with Res files
			error(data, "Unable to allocate necessary memory");
		} else {
			if ((hf = fopen(pszOutFile, "wb")) == NULL) {
				error(data, "Unable to create output file '%s'", pszOutFile);
			} else {
				data->fThrowback = FALSE;
				header = (template_header *) pszTemplate;
				header->font_offset = template_NO_FONTS;
				index = (template_index *) (header + 1);
				pTerm = (PINT) (index + nWindows);
				pszOut = (PSTR) (pTerm + 1);
				pszBuff = NULL;
				cbBuff = 0;
				i = index;
				while ((pszObject = next_object(&pszIn, pszEnd)) != NULL && strcasecmp(pszObject, "wimp_window") == 0) {
					nIcons = icon_count(pszIn, pszEnd);
					cb = sizeof(wimp_window) + (nIcons * (256 + sizeof(wimp_icon)));
					if (cbBuff < cb) {
						if (pszBuff != NULL) {
							MyFree(pszBuff);
						}
						if ((pszBuff = MyAlloc(cb)) == NULL) {
							error(data, "Unable to allocate %d bytes", cb);
							break;
						}
						cbBuff = cb;
					}
					cb = window_text2template(data, pszIn, -(sizeof(wimp_window_base) + nIcons * sizeof(wimp_icon)), (wimp_window_base *) pszBuff);
					memcpy(pszOut, pszBuff, cb);
					i->size = cb;
					i->offset = (int) (pszOut - pszTemplate);
					pszOut += cb;
					i->type = template_TYPE_WINDOW;
					put_objects(data, pszIn, 0, (PSTR) i, TemplateHeaderList, ELEMENTS(TemplateHeaderList));
// template name is different to icon name because it always needs a terminator, which restricts it to 11 chars
// to avoid having to write separate code, use the iol_PSTR data type then apply this bodge...
					if (i->name[11] >= ' ') {
						i->name[11] = '\0';
						report(data, pszObject, "Template name truncated to '%s'", i->name);
						i->name[11] = '\r';
					}
					i++;
					if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL) {
						break;
					}
				}
				pszIn = data->pszIn;
				while ((pszObject = next_object(&pszIn, pszEnd)) != NULL) {
					if (strcasecmp(pszObject, "template_font_data") == 0) {
						if (header->font_offset == template_NO_FONTS) {
							header->font_offset = (bits) (pszOut - pszTemplate);
						}
// make sure we're writing to a word-aligned structure...
						memset(&font_data, 0, sizeof(font_data));
						put_objects(data, pszIn, 0, (PSTR) &font_data, TemplateFontDataList, ELEMENTS(TemplateFontDataList));
						memcpy(pszOut, &font_data, sizeof(font_data));
						pszOut += sizeof(template_font_data);
					}
					if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL) {
						break;
					}

				}
				fwrite(pszTemplate, (int) (pszOut - pszTemplate), 1, hf);
				fclose(hf);
#ifdef __riscos__
				osfile_set_type(pszOutFile, osfile_TYPE_TEMPLATE);
#endif
				if (data->fThrowback)
				  {
				  report_end(data);
				  data->fThrowback = FALSE;
				  }
				if (pszBuff != NULL) {
					MyFree(pszBuff);
				}
				fConverted = TRUE;
			}
			MyFree(pszTemplate);
			free_string_table(&data->StringTable);
		}
	}
	return fConverted;
}


static BOOL template2text(PDATA data, PSTR pszOutFile)
{
template_header * template_hdr;
template_index * obj;
PSTR pszBuff;
FILE * hf;
int cbBuff;

if ((hf = fopen(pszOutFile, "wb")) == NULL)
  {
  error(data, "Unable to create output file '%s'", pszOutFile);
  return FALSE;
  }
fputs("Template:\n", hf);

pszBuff = NULL;
cbBuff = 0;
template_hdr = (template_header *) data->pszIn;
for (obj = (template_index *) (template_hdr + 1); obj->offset != 0; ++obj)
  {
  if (cbBuff < obj->size)
    {
    if (pszBuff != NULL)
      MyFree(pszBuff);
    if ((pszBuff = MyAlloc(obj->size)) == NULL)
      {
      error(data, "Unable to allocate %d bytes", obj->size);
      break;
      }
    }
  memcpy(pszBuff, data->pszIn + obj->offset, obj->size);
  fputs("\nwimp_window {\n", hf);
  get_objects(data, hf, NULL, NULL, (const char *) obj, TemplateHeaderList, ELEMENTS(TemplateHeaderList), 1);
  window_template2text(data, hf, pszBuff);
  fputs("}\n", hf);
  }
if (template_hdr->font_offset != template_NO_FONTS)
  {
  template_index *end = (template_index *) &data->pszIn[data->cbIn];
  get_template_fonts(data, hf, (template_font_data *) (data->pszIn + template_hdr->font_offset), (template_font_data *) end);
  }
fclose(hf);
#ifdef __riscos__
osfile_set_type(pszOutFile, osfile_TYPE_TEXT);
#endif

if (pszBuff != NULL)
  MyFree(pszBuff);

return TRUE;
}


        int ccres_initialise(DATA *sessionP)
//      ====================================
{
memset(sessionP, 0, sizeof(DATA));
sessionP->returnStatus = EXIT_SUCCESS;

return TRUE;
}


        int ccres_finish(void)
//      ======================
{

return TRUE;
}


        BOOL ccres_convert(PDATA data, PSTR pszOutFile)
//      ===============================================
{
switch (data->nFiletypeIn)
  {
  case osfile_TYPE_TEXT:
    switch (data->nFiletypeOut)
      {
      case osfile_TYPE_RESOURCE:
        return text2res(data, pszOutFile);
      case osfile_TYPE_TEMPLATE:
        return text2template(data, pszOutFile);
      }
    break;
  case osfile_TYPE_RESOURCE:
    return res2text(data, pszOutFile);
  case osfile_TYPE_TEMPLATE:
    return template2text(data, pszOutFile);
  }

return FALSE;
}
