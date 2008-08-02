/* CCres_Convert.c

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

#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>

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

#include "CCres_Internal.h"
#include "CCres_Convert.h"
#include "CCres_Report.h"
#include "CCres_Utils.h"

#include "CCres_ColourDbox.h"
#include "CCres_ColourMenu.h"
#include "CCres_DCS.h"
#include "CCres_FileInfo.h"
#include "CCres_FontDbox.h"
#include "CCres_FontMenu.h"
#include "CCres_Iconbar.h"
#include "CCres_Menu.h"
#include "CCres_Object.h"
#include "CCres_PrintDbox.h"
#include "CCres_ProgInfo.h"
#include "CCres_Quit.h"
#include "CCres_SaveAs.h"
#include "CCres_Scale.h"
#include "CCres_Window.h"

static const CLASSES Classes[] =
  {
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


static bool text2res(DATA *data, const char *pszOutFile)
{
  toolbox_resource_file_base Hdr;
  FILE *hf;
  char *pszOut;
  bool fHeader;

  const char *pszIn = data->pszIn;
  const char * const pszEnd = data->pszIn + data->cbIn;
  if (memcmp(pszIn, "RESF:1.01", sizeof("RESF:1.01")-1) != 0)
    {
      ccres_report(data, report_error, 0, "File is not RESF v1.01");
      return false;
    }
  // approx = window with 256 gadgets, 3 strings each
  if ((pszOut = MyAlloc(296 * 268 * 3)) == NULL
      || !alloc_string_table(&data->StringTable)
      || !alloc_string_table(&data->MessageTable)
      || !alloc_reloc_table(&data->RelocTable))
    {
      ccres_report(data, report_error, 0, "Unable to allocate necessary memory");
      return false;
    }

  if (pszOutFile != NULL && pszOutFile[0] == '-' && pszOutFile[1] == '\0')
      hf = NULL;
  else if ((hf = fopen(pszOutFile, "wb")) == NULL)
    {
      ccres_report(data, report_error, 0, "Unable to create output file '%s'", pszOutFile);
      return false;
    }

  Hdr.file_id = RESF;
  Hdr.version = 101;
  Hdr.header_size = -1;
  fHeader = false;
  const char *pszObject;
  while ((pszObject = next_object(&pszIn, pszEnd)) != NULL)
    {
      for (unsigned int m = 0; m < ELEMENTS(Classes); m++)
        {
          if (strcasecmp(Classes[m].name, pszObject) == 0 && Classes[m].t2o != NULL)
            {
              if (!fHeader)
                {
                  Hdr.header_size = sizeof(Hdr);
                  fwrite(&Hdr, sizeof(Hdr), 1, hf != NULL ? hf : stdout);
                  fHeader = true;
                }
              reset_string_table(&data->StringTable);
              reset_string_table(&data->MessageTable);
              reset_reloc_table(&data->RelocTable);
              object_text2resource(data, hf != NULL ? hf : stdout, pszIn, pszOut, &Classes[m]);
              goto text2res_added;
            }
        }
      ccres_report(data, report_error, getlinenr(data, pszIn), "Don't know how to handle object '%s'", pszObject);

text2res_added:
      if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL)
        break;
    }
  if (!fHeader)
    fwrite(&Hdr, sizeof(Hdr), 1, hf != NULL ? hf : stdout);
  if (hf != NULL)
    fclose(hf);
#ifdef __riscos__
  osfile_set_type(pszOutFile, osfile_TYPE_RESOURCE);
#endif
  free_string_table(&data->StringTable);
  free_string_table(&data->MessageTable);
  free_reloc_table(&data->RelocTable);
  MyFree(pszOut);

  return true;
}


static bool res2text(DATA *data, const char *pszOutFile)
{
  const int *relocation_table;
  FILE *hf;
  int cb;
  bool fConverted;

  const toolbox_resource_file_base *file_hdr = (const toolbox_resource_file_base *)data->pszIn;
  if (file_hdr->file_id != RESF)
    {
      ccres_report(data, report_error, 0, "Invalid Resource File Header (%x)", file_hdr->file_id);
      return false;
    }
  if (file_hdr->version != 101)
    {
      ccres_report(data, report_error, 0, "Unknown Resource File Version (%x)", file_hdr->version);
      return false;
    }

  if (pszOutFile != NULL && pszOutFile[0] == '-' && pszOutFile[1] == '\0')
      hf = NULL;
  else if ((hf = fopen(pszOutFile, "wb")) == NULL)
    {
      ccres_report(data, report_error, 0, "Unable to create output file '%s'", pszOutFile);
      return false;
    }

  fConverted = false;
  fprintf(hf != NULL ? hf : stdout, "RESF:%d.%02d\n", file_hdr->version / 100, file_hdr->version % 100);
  if (file_hdr->header_size > 0)
    {
      const toolbox_relocatable_object_base *obj = (const toolbox_relocatable_object_base *)(file_hdr + 1);
      const toolbox_relocatable_object_base *end = (const toolbox_relocatable_object_base *)&data->pszIn[data->cbIn];
      do
        {
          fputs("\n", hf != NULL ? hf : stdout);
          unsigned int m;
          for (m = 0; m < ELEMENTS(Classes); m++)
            {
              if (Classes[m].class_no == obj->rf_obj.class_no)
                {
                  fprintf(hf != NULL ? hf : stdout, "%s {\n", Classes[m].name);
                  object_resource2text(data, hf != NULL ? hf : stdout, obj, Classes[m].o2t);
                  fputs("}\n", hf != NULL ? hf : stdout);
                  break;
                }
            }
          if (m == ELEMENTS(Classes))
            ccres_report(data, report_error, 0, "Unknown class '%s' (%x)", obj->rf_obj.name, obj->rf_obj.class_no);

          cb = sizeof(toolbox_relocatable_object_base) - sizeof(obj->rf_obj) + obj->rf_obj.size;
          if (obj->relocation_table_offset != -1)
            {
              relocation_table = (const int *)((const char *)obj + obj->relocation_table_offset);
              cb += sizeof(int) * (1 + 2 * relocation_table[0]);
            }
          obj = (const toolbox_relocatable_object_base *)((const char *)obj + cb);
        }
      while (cb != 0 && obj < end);
    }
  if (hf != NULL)
    fclose(hf);
#ifdef __riscos__
  osfile_set_type(pszOutFile, osfile_TYPE_TEXT);
#endif
  fConverted = true;

  return fConverted;
}

#define template_NO_FONTS 	((bits)-1)
#define template_TYPE_WINDOW	((bits)0x1u)

typedef struct
  {
    bits font_offset; /* Can be template_NO_FONTS too */
    bits reserved[3];
  }
template_header;

typedef struct
  {
    bits offset;
    bits size;
    int type;          /* See template_TYPE_* */
    char name[12];
  }
template_index;

typedef struct
  {
    bits x_point_size;
    bits y_point_size;
    char font_name[40];
  }
template_font_data;

// Returns the number of windows and the maximum number of icons used in
// those windows (not the total number of icons !).
static int window_count(const char *pszIn, const char *pszEnd, int *pi)
{
  int d, w, i, maxi;
  char ch, ch0;

  d = w = i = maxi = 0;
  ch0 = '\0';
  while (pszIn < pszEnd)
    {
      if ((ch = *pszIn++) == ':' || ch == '\0')
        ch0 = ch;
      else if (ch0 == '\0')
        {
          if (ch == '{')
            {
              if (d == 0)
                {
                  w++;
                  i = 0;
                }
              else
                {
                  i++;
                  if (i > maxi)
                    maxi = i;
                }
              d++;
            }
          else if (ch == '}')
            d--;
         }
    }
  *pi = maxi;
  return w;
}

static int icon_count(const char *pszIn, const char *pszEnd)
{
  int d, i;
  char ch, ch0;

  d = 1;
  i = 0;
  ch0 = '\0';
  while (pszIn < pszEnd && d > 0)
    {
      if ((ch = *pszIn++) == ':' || ch == '\0')
        ch0 = ch;
      else if (ch0 == '\0')
        {
          if (ch == '{')
            {
              i++;
              d++;
            }
          else if (ch == '}')
            d--;
        }
    }
  return i;
}


static const OBJECTLIST TemplateFontDataList[] =
  {
    {iol_BITS,    "x_point_size:", offsetof(template_font_data, x_point_size), NULL,  0},
    {iol_BITS,    "y_point_size:", offsetof(template_font_data, y_point_size), NULL,  0},
    {iol_CHARPTR, "font_name:",    offsetof(template_font_data, font_name),    NULL, 40}
  };


static void get_template_fonts(DATA *data, FILE *hf, const template_font_data *font_data, const template_font_data *end)
{
  template_font_data temp;

  while (font_data < end)
    {
      memcpy(&temp, font_data, sizeof(temp));
      fputs("\ntemplate_font_data {\n", hf);
      get_objects(data, hf, NULL, (const char *)&temp, TemplateFontDataList, ELEMENTS(TemplateFontDataList), 1);
      fputs("}\n", hf);
      font_data++;
    }
}


static const OBJECTLIST TemplateHeaderList[] =
  {
    {iol_CHARPTR,  "template_name:",  offsetof(template_index, name), NULL, 12 }
  };

static bool text2template(DATA *data, const char *pszOutFile)
{
  template_font_data font_data;
  template_header *header;
  template_index *index, *i;
  FILE *hf;
  int *pTerm;
  const char *pszIn, *pszEnd, *pszObject;
  char *pszBuff, *pszOut, *pszTemplate;
  int cb, cbBuff, nWindows, nIcons;
  bool fConverted;

  pszIn = data->pszIn;
  pszEnd = data->pszIn + data->cbIn;
  fConverted = false;
  if ((nWindows = window_count(pszIn, pszEnd, &nIcons)) > 0)
    {
      if ((pszTemplate = MyAlloc(sizeof(template_header) + sizeof(int) +
                                 nWindows * (sizeof(template_index) + sizeof(wimp_window) + (nIcons * (256 + sizeof(wimp_icon)))))) == NULL ||
          !alloc_string_table(&data->StringTable))
        {      // this is only required so that we can share put_string with Res files
          ccres_report(data, report_error, getlinenr(data, pszIn), "Unable to allocate necessary memory");
        }
      else
        {
	  hf = NULL;
	  if ((pszOutFile != NULL && pszOutFile[0] == '-' && pszOutFile[1] == '\0') || (hf = fopen(pszOutFile, "wb")) != NULL)
            {
              header = (template_header *) pszTemplate;
              header->font_offset = template_NO_FONTS;
              index = (template_index *) (header + 1);
              pTerm = (int *) (index + nWindows);
              pszOut = (char *) (pTerm + 1);
              pszBuff = NULL;
              cbBuff = 0;
              i = index;
              while ((pszObject = next_object(&pszIn, pszEnd)) != NULL && strcasecmp(pszObject, "wimp_window") == 0)
                {
                  nIcons = icon_count(pszIn, pszEnd);
                  cb = sizeof(wimp_window) + (nIcons * (256 + sizeof(wimp_icon)));
                  if (cbBuff < cb)
                    {
                      if (pszBuff != NULL)
                        {
                          MyFree(pszBuff);
                        }
                      if ((pszBuff = MyAlloc(cb)) == NULL)
                        {
                          ccres_report(data, report_error, getlinenr(data, pszIn), "Unable to allocate %d bytes", cb);
                          break;
                        }
                      cbBuff = cb;
                    }
                  cb = window_text2template(data, pszIn, -(sizeof(wimp_window_base) + nIcons *sizeof(wimp_icon)), (wimp_window_base *) pszBuff);
                  memcpy(pszOut, pszBuff, cb);
                  i->size = cb;
                  i->offset = (int) (pszOut - pszTemplate);
                  pszOut += cb;
                  i->type = template_TYPE_WINDOW;
                  put_objects(data, pszIn, 0, (char *)i, TemplateHeaderList, ELEMENTS(TemplateHeaderList));
// template name is different to icon name because it always needs a terminator, which restricts it to 11 chars
// to avoid having to write separate code, use the iol_char *data type then apply this bodge...
                  if ((unsigned char)i->name[11] >= (unsigned char)' ')
                    {
                      ccres_report(data, report_warning, getlinenr(data, pszObject), "Template name truncated to '%.*s'", 11, i->name);
                      i->name[11] = '\r';
                    }
                  i++;
                  if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL)
                    break;
                }
              pszIn = data->pszIn;
              while ((pszObject = next_object(&pszIn, pszEnd)) != NULL)
                {
                  if (strcasecmp(pszObject, "template_font_data") == 0)
                    {
                      if (header->font_offset == template_NO_FONTS)
                        header->font_offset = (bits) (pszOut - pszTemplate);
// make sure we're writing to a word-aligned structure...
                      memset(&font_data, 0, sizeof(font_data));
                      put_objects(data, pszIn, 0, (char *)&font_data, TemplateFontDataList, ELEMENTS(TemplateFontDataList));
                      memcpy(pszOut, &font_data, sizeof(font_data));
                      pszOut += sizeof(template_font_data);
                    }
                  if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL)
                    break;

                }
              fwrite(pszTemplate, (int) (pszOut - pszTemplate), 1, hf != NULL ? hf : stdout);
	      if (hf != NULL)
                fclose(hf);
#ifdef __riscos__
              osfile_set_type(pszOutFile, osfile_TYPE_TEMPLATE);
#endif
              MyFree(pszBuff);
              fConverted = true;
            }
          else
            ccres_report(data, report_error, 0, "Unable to create output file '%s'", pszOutFile);

          MyFree(pszTemplate);
          free_string_table(&data->StringTable);
        }
    }
  return fConverted;
}


static bool template2text(DATA *data, const char *pszOutFile)
{
  char *pszBuff;
  FILE *hf;
  unsigned int cbBuff;

  if (pszOutFile != NULL && pszOutFile[0] == '-' && pszOutFile[1] == '\0')
      hf = NULL;
  else if ((hf = fopen(pszOutFile, "wb")) == NULL)
    {
      ccres_report(data, report_error, 0, "Unable to create output file '%s'", pszOutFile);
      return false;
    }
  fputs("Template:\n", hf != NULL ? hf : stdout);

  pszBuff = NULL;
  cbBuff = 0;
  const template_header *template_hdr = (const template_header *) data->pszIn;
  const template_index *obj;
  for (obj = (const template_index *)(template_hdr + 1); obj->offset != 0; ++obj)
    {
      if (obj->type != template_TYPE_WINDOW)
        {
          ccres_report(data, report_error, 0, "Template file contains a non window entry (type %d)", obj->type);
          continue;
        }
      if (cbBuff < obj->size)
        {
          if (pszBuff != NULL)
            MyFree(pszBuff);
          if ((pszBuff = MyAlloc(obj->size)) == NULL)
            {
              ccres_report(data, report_error, 0, "Unable to allocate %d bytes", obj->size);
              break;
            }
          cbBuff = obj->size;
        }
      memcpy(pszBuff, data->pszIn + obj->offset, obj->size);
      fputs("\nwimp_window {\n", hf != NULL ? hf : stdout);
      get_objects(data, hf != NULL ? hf : stdout, NULL, (const char *)obj, TemplateHeaderList, ELEMENTS(TemplateHeaderList), 1);
      window_template2text(data, hf != NULL ? hf : stdout, pszBuff, obj->size);
      fputs("}\n", hf != NULL ? hf : stdout);
    }
  if (template_hdr->font_offset != template_NO_FONTS)
    {
      const template_index *end = (const template_index *)&data->pszIn[data->cbIn];
      get_template_fonts(data, hf != NULL ? hf : stdout, (const template_font_data *)(data->pszIn + template_hdr->font_offset), (const template_font_data *) end);
    }
  if (hf != NULL)
    fclose(hf);
#ifdef __riscos__
  osfile_set_type(pszOutFile, osfile_TYPE_TEXT);
#endif

  if (pszBuff != NULL)
    MyFree(pszBuff);

  return true;
}


DATA *ccres_initialise(void)
{
  DATA *sessionP;
  if ((sessionP = MyAlloc(sizeof(DATA))) == NULL)
    return NULL;

  memset(sessionP, 0, sizeof(DATA));

  // Setup default stderr reporting:
  ccres_install_report_routine(sessionP, report_varg_stderr, report_end_stderr);

  return sessionP;
}


bool ccres_finish(DATA *sessionP)
{
  MyFree((void *)sessionP->pszIn);
  MyFree((void *)sessionP);

  return true;
}


bool ccres_convert(DATA *data, const char *pszOutFile)
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

  data->report_end(data);

  return false;
}

void ccres_install_report_routine(DATA *sessionP, report_cb report_routine, report_end_cb report_end_routine)
{
  sessionP->report = report_routine;
  sessionP->report_end = report_end_routine;
}

void ccres_report(DATA *sessionP, report_level level, unsigned int linenr, const char *pszFmt, ...)
{
  va_list list;
  va_start(list, pszFmt);
  sessionP->report(sessionP, level, linenr, pszFmt, list);
  va_end(list);
}

// Returns 'false' in case of an error.
bool ccres_load_file(DATA *sessionP, const char *pszPath, bits nFiletype)
{
  sessionP->nFiletypeIn = nFiletype;
  if (sessionP->pszIn != NULL)
    MyFree((void *)sessionP->pszIn);

  FILE *fhandle;
  char *pszIn;
  int cbIn;
  if (pszPath != NULL && pszPath[0] == '-' && pszPath[1] == '\0')
    {
#define CHUNK_SIZE (4096)
      if ((pszIn = (char *) MyAlloc(CHUNK_SIZE)) == NULL)
        return false;

      memcpy(pszIn, sessionP->ftypeBuf, sizeof(sessionP->ftypeBuf));
      cbIn = sizeof(sessionP->ftypeBuf);

      int nToRead = CHUNK_SIZE - cbIn;
      do
	{
	  int read = fread(pszIn + cbIn, 1, nToRead, stdin);
	  if (read == nToRead)
	    {
	      char *pTemp;
	      if ((pTemp = (char *) MyRealloc(pszIn, cbIn + nToRead + CHUNK_SIZE)) == NULL)
		{
		  MyFree(pszIn);
	          return false;
		}
	      pszIn = pTemp;
	      nToRead = CHUNK_SIZE;
	    }
	  else
	    nToRead -= read;
	  cbIn += read;
	}
      while(feof(stdin) == 0);
#undef CHUNK_SIZE
    }
  else
    {
      if ((fhandle = fopen(pszPath, "rb")) == NULL)
        {
          ccres_report(sessionP, report_error, 0, "Can not open file <%s> for input", pszPath);
          return false;
        }
      fseek(fhandle, 0, SEEK_END);
      cbIn = (int)ftell(fhandle);
      fseek(fhandle, 0, SEEK_SET);

      if ((pszIn = (char *) MyAlloc(cbIn)) == NULL)
        return false;

      if (fread(pszIn, cbIn, 1, fhandle) != 1)
        {
          ccres_report(sessionP, report_error, 0, "Can not read file <%s>", pszPath);
          MyFree(pszIn);
          return false;
        }
      fclose(fhandle);
      fhandle = NULL;
    }

  sessionP->pszIn = pszIn;
  sessionP->cbIn = cbIn;
  strcpy(sessionP->achFileIn, pszPath);	// for throwback
  if (nFiletype == osfile_TYPE_TEXT)
    {
      int nSpaces = 0;
      while (nSpaces < cbIn)
	{
	  if (!isspace(pszIn[nSpaces])) // Skip any leading whitespace
            break;
	  nSpaces++;
	}
      if (nSpaces > 0)
	{
	  memmove(pszIn, pszIn + nSpaces, cbIn - nSpaces);
	  sessionP->cbIn = cbIn - nSpaces;
	}

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
          ccres_report(sessionP, report_error, 0, "Unrecognized input file type for %s", pszPath);
          return false;
        }
    }
  else
    sessionP->nFiletypeOut = osfile_TYPE_TEXT;

  return true;
}

bits ccres_get_filetype_in(DATA *sessionP, const char *filenameP)
{
  FILE *fhandle;
  if (filenameP != NULL && filenameP[0] == '-' && filenameP[1] == '\0')
    fhandle = NULL;
  else if ((fhandle = fopen(filenameP, "rb")) == NULL)
    {
      ccres_report(sessionP, report_error, 0, "Can not open file <%s> for input", filenameP);
      return 0;
    }
  if (fread(sessionP->ftypeBuf, sizeof(sessionP->ftypeBuf), 1, fhandle != NULL ? fhandle : stdin) != 1)
    {
      ccres_report(sessionP, report_error, 0, "Can't read the file contents <%s>", filenameP);
      return 0;
    }
  if (fhandle != NULL)
    fclose(fhandle);

// Binary resource file ?
  const toolbox_resource_file_base *resFileHdrP = (const toolbox_resource_file_base *)sessionP->ftypeBuf;
  if (resFileHdrP->file_id == RESF && resFileHdrP->version == 101)
    return osfile_TYPE_RESOURCE;

// Binary template file ?
  const int *temFileHdrP = (const int *)sessionP->ftypeBuf;
  if (temFileHdrP[1] == 0 && temFileHdrP[2] == 0 && temFileHdrP[3] == 0)
    return osfile_TYPE_TEMPLATE;

// Text file (check for control chars) ?
  for (unsigned int i = 0; i < sizeof(sessionP->ftypeBuf); ++i)
    if (sessionP->ftypeBuf[i] < 32 && sessionP->ftypeBuf[i] != 10)
      return 0;

  return osfile_TYPE_TEXT;
}

bits ccres_get_filetype_out(DATA *sessionP)
{
  return sessionP->nFiletypeOut;
}
