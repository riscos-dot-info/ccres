/* CCres_Window.c

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

#include <stddef.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include <oslib/toolbox.h>
#include <oslib/window.h>

#include <oslib/actionbutton.h>
#include <oslib/adjuster.h>
#include <oslib/button.h>
#include <oslib/displayfield.h>
#include <oslib/draggable.h>
#include <oslib/keyboardshortcut.h>
#include <oslib/label.h>
#include <oslib/labelledbox.h>
#include <oslib/numberrange.h>
#include <oslib/optionbutton.h>
#include <oslib/popup.h>
#include <oslib/radiobutton.h>
#include <oslib/scrolllist.h>
#include <oslib/slider.h>
#include <oslib/stringset.h>
#include <oslib/textarea.h>
#include <oslib/textgadgets.h>
#include <oslib/toolaction.h>
#include <oslib/writablefield.h>

#include "CCres_Internal.h"
#include "CCres_Report.h"
#include "CCres_Utils.h"
#include "CCres_Gadgets_TabsTreeview.h"
#include "CCres_Gadgets.h"
#include "CCres_Icon.h"
#include "CCres_Object.h"
#include "CCres_Window.h"

static const GADGETS Gadgets[] =
  {
    {class_ACTION_BUTTON,  actionbutton_g2t,  actionbutton_t2g,  "actionbutton_object" },
    {class_ADJUSTER,       adjuster_g2t,      adjuster_t2g,      "adjuster_object"     },
    {class_BUTTON,         button_g2t,        button_t2g,        "button_object"       },
    {class_DISPLAY_FIELD,  displayfield_g2t,  displayfield_t2g,  "displayfield_object" },
    {class_DRAGGABLE,      draggable_g2t,     draggable_t2g,     "draggable_object"    },
    {class_LABEL,          label_g2t,         label_t2g,         "label_object"        },
    {class_LABELLED_BOX,   labelledbox_g2t,   labelledbox_t2g,   "labelledbox_object"  },
    {class_NUMBER_RANGE,   numberrange_g2t,   numberrange_t2g,   "numberrange_object"  },
    {class_OPTION_BUTTON,  optionbutton_g2t,  optionbutton_t2g,  "optionbutton_object" },
    {class_POP_UP,         popup_g2t,         popup_t2g,         "popup_object"        },
    {class_RADIO_BUTTON,   radiobutton_g2t,   radiobutton_t2g,   "radiobutton_object"  },
    {class_SCROLL_LIST,    scrolllist_g2t,    scrolllist_t2g,    "scrolllist_object"   },
    {class_SLIDER,         slider_g2t,        slider_t2g,        "slider_object"       },
    {class_STRING_SET,     stringset_g2t,     stringset_t2g,     "stringset_object"    },
    {class_TABS,           tabs_g2t,          tabs_t2g,          "tabs_object"         },
    {class_TEXT_AREA,      textarea_g2t,      textarea_t2g,      "textarea_object"     },
    {class_TREE_VIEW,      treeview_g2t,      treeview_t2g,      "treeview_object",    },
    {class_TOOL_ACTION,    toolaction_g2t,    toolaction_t2g,    "toolaction_object"   },
    {class_WRITABLE_FIELD, writablefield_g2t, writablefield_t2g, "writablefield_object"}
  };

static const FLAGS WindowFlags[] =
  {
    {window_GENERATE_ABOUT_TO_BE_SHOWN , "window_GENERATE_ABOUT_TO_BE_SHOWN" },
    {window_AUTO_OPEN                  , "window_AUTO_OPEN"                  },
    {window_AUTO_CLOSE                 , "window_AUTO_CLOSE"                 },
    {window_GENERATE_DIALOGUE_COMPLETED, "window_GENERATE_DIALOGUE_COMPLETED"},
    {window_IS_TOOL_BAR                , "window_IS_TOOL_BAR"                }
  };

static const FLAGS WindowWindowFlags[] =
  {
    {wimp_WINDOW_MOVEABLE         , "wimp_WINDOW_MOVEABLE"         },
    {wimp_WINDOW_AUTO_REDRAW      , "wimp_WINDOW_AUTO_REDRAW"      },
    {wimp_WINDOW_PANE             , "wimp_WINDOW_PANE"             },
    {wimp_WINDOW_NO_BOUNDS        , "wimp_WINDOW_NO_BOUNDS"        },
    {wimp_WINDOW_SCROLL_REPEAT    , "wimp_WINDOW_SCROLL_REPEAT"    },
    {wimp_WINDOW_SCROLL           , "wimp_WINDOW_SCROLL"           },
    {wimp_WINDOW_REAL_COLOURS     , "wimp_WINDOW_REAL_COLOURS"     },
    {wimp_WINDOW_BACK             , "wimp_WINDOW_BACK"             },
    {wimp_WINDOW_HOT_KEYS         , "wimp_WINDOW_HOT_KEYS"         },
    {wimp_WINDOW_BOUNDED          , "wimp_WINDOW_BOUNDED"          },
    {wimp_WINDOW_IGNORE_XEXTENT   , "wimp_WINDOW_IGNORE_XEXTENT"   },
    {wimp_WINDOW_IGNORE_YEXTENT   , "wimp_WINDOW_IGNORE_YEXTENT"   },
    {wimp_WINDOW_OPEN             , "wimp_WINDOW_OPEN"             },
    {wimp_WINDOW_NOT_COVERED      , "wimp_WINDOW_NOT_COVERED"      },
    {wimp_WINDOW_FULL_SIZE        , "wimp_WINDOW_FULL_SIZE"        },
    {wimp_WINDOW_TOGGLED          , "wimp_WINDOW_TOGGLED"          },
    {wimp_WINDOW_HAS_FOCUS        , "wimp_WINDOW_HAS_FOCUS"        },
    {wimp_WINDOW_BOUNDED_ONCE     , "wimp_WINDOW_BOUNDED_ONCE"     },
    {wimp_WINDOW_PARTIAL_SIZE     , "wimp_WINDOW_PARTIAL_SIZE"     },
    {wimp_WINDOW_FURNITURE_WINDOW , "wimp_WINDOW_FURNITURE_WINDOW" },
    {wimp_WINDOW_FOREGROUND_WINDOW, "wimp_WINDOW_FOREGROUND_WINDOW"},
    {wimp_WINDOW_BACK_ICON        , "wimp_WINDOW_BACK_ICON"        },
    {wimp_WINDOW_CLOSE_ICON       , "wimp_WINDOW_CLOSE_ICON"       },
    {wimp_WINDOW_TITLE_ICON       , "wimp_WINDOW_TITLE_ICON"       },
    {wimp_WINDOW_TOGGLE_ICON      , "wimp_WINDOW_TOGGLE_ICON"      },
    {wimp_WINDOW_VSCROLL          , "wimp_WINDOW_VSCROLL"          },
    {wimp_WINDOW_SIZE_ICON        , "wimp_WINDOW_SIZE_ICON"        },
    {wimp_WINDOW_HSCROLL          , "wimp_WINDOW_HSCROLL"          },
    {wimp_WINDOW_NEW_FORMAT       , "wimp_WINDOW_NEW_FORMAT"       }
  };

static const FLAGS WindowExtraFlags[] =
  {
    {wimp_WINDOW_USE_TITLE_VALIDATION_COLOURS, "wimp_WINDOW_USE_TITLE_VALIDATION_COLOURS"},
    {wimp_WINDOW_USE_TITLE_VALIDATION_STRING , "wimp_WINDOW_USE_TITLE_VALIDATION_STRING" },
    {wimp_WINDOW_USE_EXTENDED_SCROLL_REQUEST , "wimp_WINDOW_USE_EXTENDED_SCROLL_REQUEST" },
    {wimp_WINDOW_NEVER3D                     , "wimp_WINDOW_NEVER3D"                     },
    {wimp_WINDOW_ALWAYS3D                    , "wimp_WINDOW_ALWAYS3D"                    }
  };

static const FLAGS WindowTitleFlags[] =
  {
    {wimp_ICON_TEXT               , "wimp_ICON_TEXT"               },
    {wimp_ICON_SPRITE             , "wimp_ICON_SPRITE"             },
    {wimp_ICON_BORDER             , "wimp_ICON_BORDER"             },
    {wimp_ICON_HCENTRED           , "wimp_ICON_HCENTRED"           },
    {wimp_ICON_VCENTRED           , "wimp_ICON_VCENTRED"           },
    {wimp_ICON_FILLED             , "wimp_ICON_FILLED"             },
    {wimp_ICON_ANTI_ALIASED       , "wimp_ICON_ANTI_ALIASED"       },

    {wimp_ICON_INDIRECTED         , "wimp_ICON_INDIRECTED"         },
    {wimp_ICON_RJUSTIFIED         , "wimp_ICON_RJUSTIFIED"         },
    {wimp_ICON_ALLOW_ADJUST       , "wimp_ICON_ALLOW_ADJUST"       },
    {wimp_ICON_HALF_SIZE          , "wimp_ICON_HALF_SIZE"          },

    {wimp_ICON_SELECTED           , "wimp_ICON_SELECTED"           },
    {wimp_ICON_SHADED             , "wimp_ICON_SHADED"             }
  };

static const FLAGS WindowWorkFlags[] =
  {
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

static const FLAGS WindowNextFlags[] =
  {
    {(int) wimp_TOP   , "wimp_TOP"   },
    {(int) wimp_BOTTOM, "wimp_BOTTOM"},
    {(int) wimp_HIDDEN, "wimp_HIDDEN"}
  };

static const OBJECTLIST WindowObjectList[] =
  {
    {iol_FLAGS,  "window_flags:",            offsetof(window_object_base, flags),               WindowFlags,       ELEMENTS(WindowFlags)    },
    {iol_MSG,    "help_message:",            offsetof(window_object_base, help_message),        "help_limit:",     offsetof(window_object_base, help_limit)},
    {iol_STRING, "sprite_name:",             offsetof(window_object_base, sprite_name),         NULL,              0                                       },
    {iol_INT,    "pointer_limit:",           offsetof(window_object_base, pointer_limit),       NULL,              0                                       },
    {iol_COORD,  "hotspot:",                 offsetof(window_object_base, hotspot),             NULL,              0                                       },
    {iol_STRING, "menu_name:",               offsetof(window_object_base, menu_name),           NULL,              0                                       },
    {iol_OBJECT, "shortcuts:",               offsetof(window_object_base, shortcuts),           NULL,              0                                       },
    {iol_OBJECT, "gadgets:",                 offsetof(window_object_base, gadgets),             NULL,              0                                       },
    {iol_CMP,    "default_focus:",           offsetof(window_object_base, default_focus),       NULL,              0                                       },
    {iol_BITS,   "show_action:",             offsetof(window_object_base, show_action),         NULL,              bits_ACTION                             },
    {iol_BITS,   "hide_action:",             offsetof(window_object_base, hide_action),         NULL,              bits_ACTION                             },
    {iol_STRING, "toolbar_ibl:",             offsetof(window_object_base, toolbar_ibl),         NULL,              0                                       },
    {iol_STRING, "toolbar_itl:",             offsetof(window_object_base, toolbar_itl),         NULL,              0                                       },
    {iol_STRING, "toolbar_ebl:",             offsetof(window_object_base, toolbar_ebl),         NULL,              0                                       },
    {iol_STRING, "toolbar_etl:",             offsetof(window_object_base, toolbar_etl),         NULL,              0                                       },
    {iol_BOX,    "window.visible:",          offsetof(window_object_base, window.visible),      NULL,              0                                       },
    {iol_INT,    "window.xscroll:",          offsetof(window_object_base, window.xscroll),      NULL,              0                                       },
    {iol_INT,    "window.yscroll:",          offsetof(window_object_base, window.yscroll),      NULL,              0                                       },
    {iol_ENUM,   "window.next:",             offsetof(window_object_base, window.next),         WindowNextFlags,   ELEMENTS(WindowNextFlags)               },
    {iol_FLAGS,  "window.flags:",            offsetof(window_object_base, window.flags),        WindowWindowFlags, ELEMENTS(WindowWindowFlags)             },
    {iol_WCOL,   "window.title_fg:",         offsetof(window_object_base, window.title_fg),     NULL,              0                                       },
    {iol_WCOL,   "window.title_bg:",         offsetof(window_object_base, window.title_bg),     NULL,              0                                       },
    {iol_WCOL,   "window.work_fg:",          offsetof(window_object_base, window.work_fg),      NULL,              0                                       },
    {iol_WCOL,   "window.work_bg:",          offsetof(window_object_base, window.work_bg),      NULL,              0                                       },
    {iol_WCOL,   "window.scroll_outer:",     offsetof(window_object_base, window.scroll_outer), NULL,              0                                       },
    {iol_WCOL,   "window.scroll_inner:",     offsetof(window_object_base, window.scroll_inner), NULL,              0                                       },
    {iol_WCOL,   "window.highlight_bg:",     offsetof(window_object_base, window.highlight_bg), NULL,              0                                       },
    {iol_BFLAGS, "window.extra_flags:",      offsetof(window_object_base, window.extra_flags),  WindowExtraFlags,  ELEMENTS(WindowExtraFlags)              },
    {iol_BOX,    "window.extent:",           offsetof(window_object_base, window.extent),       NULL,              0                                       },
    {iol_FLAGS,  "window.title_flags:",      offsetof(window_object_base, window.title_flags),  WindowTitleFlags,  ELEMENTS(WindowTitleFlags)              },
    {iol_FLAGS,  "window.work_flags:",       offsetof(window_object_base, window.work_flags),   WindowWorkFlags,   ELEMENTS(WindowWorkFlags)               },
    {iol_SPRITE, "window.sprite_area:",      offsetof(window_object_base, window.sprite_area),  NULL,              0                                       },
    {iol_SHORT,  "window.xmin:",             offsetof(window_object_base, window.xmin),         NULL,              0                                       },
    {iol_SHORT,  "window.ymin:",             offsetof(window_object_base, window.ymin),         NULL,              0                                       },
    {iol_MSG,    "window.title_text:",       offsetof(window_object_base, window.title_data.indirected_text.text),
     "window.title_text_limit:", offsetof(window_object_base, window.title_data.indirected_text.size)                                          },
    {iol_STRING, "window.title_valid:",      offsetof(window_object_base, window.title_data.indirected_text.validation), NULL, 0                           }
  };

static const OBJECTLIST GadgetHeaderList[] =
  {
    {iol_BITS, "cmp:",          offsetof(gadget_object, cmp),          NULL,          bits_EVAL    },
    {iol_BOX,  "bbox:",         offsetof(gadget_object, bbox),         NULL,          0                                  },
    {iol_MSG,  "help_message:", offsetof(gadget_object, help_message), "help_limit:", offsetof(gadget_object, help_limit)}
  };

static const char *pszShortcutObject = "keyboardshortcut_object";
static const FLAGS ShortcutFlags[] =
  {
    {keyboardshortcut_SHOW_AS_MENU   , "keyboardshortcut_SHOW_AS_MENU"   },
    {keyboardshortcut_SHOW_AT_CENTRE , "keyboardshortcut_SHOW_AT_CENTRE" },
    {keyboardshortcut_SHOW_AT_POINTER, "keyboardshortcut_SHOW_AT_POINTER"}
  };

static const OBJECTLIST ShortcutList[] =
  {
    {iol_FLAGS,  "flags:",     offsetof(keyboardshortcut_object, flags),  ShortcutFlags, ELEMENTS(ShortcutFlags)},
    {iol_BITS,   "key_code:",  offsetof(keyboardshortcut_object, c),      NULL,          0                      },
    {iol_BITS,   "key_event:", offsetof(keyboardshortcut_object, action), NULL,          0                      },
    {iol_STRING, "key_show:",  offsetof(keyboardshortcut_object, name),   NULL,          0                      }
  };


int window_t2g(DATA *data, const char *pszIn, toolbox_relocatable_object_base *object)
{
  window_object_base *window_object = (window_object_base *)(object + 1);
  put_objects(data, pszIn, 0, (char *) window_object, WindowObjectList, ELEMENTS(WindowObjectList));

  const char * const pszEnd = data->pszIn + data->cbIn;
  const char *pszShortcut = pszIn;
  keyboardshortcut_object *shortcut = (keyboardshortcut_object *)((char *)window_object + sizeof(window_object_base));
  int shortcut_count = 0;
  const char *pszObject;
  while ((pszObject = next_object(&pszShortcut, pszEnd)) != NULL)
    {
      if (strcasecmp(pszShortcutObject, pszObject) == 0)
        {
          int nOffset = (int) ((char *) shortcut - (char *) window_object);
          put_objects(data, pszShortcut, nOffset, (char *) shortcut, ShortcutList, ELEMENTS(ShortcutList));
          shortcut++;
          shortcut_count++;
        }
      if ((pszShortcut = object_end(data, pszShortcut, pszEnd)) == NULL)
        break;
    }
  if (shortcut_count > 0)
    {
      window_object->shortcut_count = shortcut_count;
      window_object->shortcuts = (toolbox_object_offset) sizeof(window_object_base);
    }
  else
    window_object->shortcuts = (toolbox_object_offset) -1;

  gadget_object_base *gadget = (gadget_object_base *) shortcut;
  int gadget_count = 0;
  while ((pszObject = next_object(&pszIn, pszEnd)) != NULL)
    {
      for (unsigned int g = 0; g < ELEMENTS(Gadgets); g++)
        {
          if (strcasecmp(Gadgets[g].name, pszObject) == 0)
            {
              int nOffset = (int) ((char *)gadget - (char *) window_object);
              put_objects(data, pszIn, nOffset, (char *)gadget, GadgetHeaderList, ELEMENTS(GadgetHeaderList));
              int nSize = Gadgets[g].t2g(data, pszIn, nOffset, gadget);
              if (nSize & 0x3)
                ccres_report(data, report_error, getlinenr(data, pszIn), "Gadget class '%s' has non aligned size %d", pszObject, nSize);
              gadget->class_no_and_size = (nSize << 16) | Gadgets[g].class_no;
              gadget = (gadget_object_base *) ((char *)gadget + nSize);
              gadget_count++;
              goto _window_gadget_added;
            }
        }
      if (strcasecmp(pszShortcutObject, pszObject) != 0)
        ccres_report(data, report_error, getlinenr(data, pszIn), "Unhandled gadget class '%s'", pszObject);

_window_gadget_added:

      if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL)
        {
          break;
        }
    }
  if (gadget_count > 0)
    {
      window_object->gadget_count = gadget_count;
      window_object->gadgets = (toolbox_object_offset) (sizeof(window_object_base) + (shortcut_count * sizeof(keyboardshortcut_object)));
    }
  else
    window_object->gadgets = (toolbox_object_offset) -1;

  return (int) ((char *)gadget - (char *)window_object);
}


void window_g2t(DATA *data, FILE * hf, const toolbox_resource_file_object_base *object, const TOOLBOXSMTABLE *strMsgTableP)
{
  const window_object_base *window_object = (const window_object_base *)(object + 1);
  get_objects(data, hf, strMsgTableP, (const char *)window_object, WindowObjectList, ELEMENTS(WindowObjectList), 1);

  const keyboardshortcut_object *shortcut = (const keyboardshortcut_object *)((const char *)window_object + (int) window_object->shortcuts);
  for (int n = 0; n < window_object->shortcut_count; n++, shortcut++)
    {
      fprintf(hf, "  %s {\n", pszShortcutObject);
      get_objects(data, hf, strMsgTableP, (const char *)shortcut, ShortcutList, ELEMENTS(ShortcutList), 2);
      fputs("  }\n", hf);
    }

  const gadget_object_base *gadget = (const gadget_object_base *)((const char *)window_object + (int) window_object->gadgets);
  for (int n = 0; n < window_object->gadget_count; n++)
    {
      unsigned int nSize = (gadget->class_no_and_size >> 16);
      unsigned int nClass = (gadget->class_no_and_size & 0xffff);
      for (unsigned int g = 0; g < ELEMENTS(Gadgets); g++)
        {
          if (Gadgets[g].class_no == nClass)
            {
              fprintf(hf, "  %s {\n", Gadgets[g].name);
              get_objects(data, hf, strMsgTableP, (const char *)gadget, GadgetHeaderList, ELEMENTS(GadgetHeaderList), 2);
              Gadgets[g].g2t(data, hf, gadget, strMsgTableP);
              fputs("  }\n", hf);
              goto window_gadget_added;
            }
        }
      ccres_report(data, report_error, 0, "Unhandled gadget class &%x size %d", nClass, nSize);

window_gadget_added:

      gadget = (const gadget_object_base *) ((const char *) gadget + nSize);
    }
}

//=================================== Template window handlers ======================================
// a separate source file would have been nice, but sharing common data tricky without duplication...


static const OBJECTLIST WimpWindowObjectList[] =
  {
    {iol_BOX,    "visible:",      offsetof(wimp_window_base, visible),      NULL,              0    },
    {iol_INT,    "xscroll:",      offsetof(wimp_window_base, xscroll),      NULL,              0                          },
    {iol_INT,    "yscroll:",      offsetof(wimp_window_base, yscroll),      NULL,              0                          },
    {iol_ENUM,   "next:",         offsetof(wimp_window_base, next),         WindowNextFlags,   ELEMENTS(WindowNextFlags)  },
    {iol_FLAGS,  "window_flags:", offsetof(wimp_window_base, flags),        WindowWindowFlags, ELEMENTS(WindowWindowFlags)},
    {iol_WCOL,   "title_fg:",     offsetof(wimp_window_base, title_fg),     NULL,              0                          },
    {iol_WCOL,   "title_bg:",     offsetof(wimp_window_base, title_bg),     NULL,              0                          },
    {iol_WCOL,   "work_fg:",      offsetof(wimp_window_base, work_fg),      NULL,              0                          },
    {iol_WCOL,   "work_bg:",      offsetof(wimp_window_base, work_bg),      NULL,              0                          },
    {iol_WCOL,   "scroll_outer:", offsetof(wimp_window_base, scroll_outer), NULL,              0                          },
    {iol_WCOL,   "scroll_inner:", offsetof(wimp_window_base, scroll_inner), NULL,              0                          },
    {iol_WCOL,   "highlight_bg:", offsetof(wimp_window_base, highlight_bg), NULL,              0                          },
    {iol_BFLAGS, "extra_flags:",  offsetof(wimp_window_base, extra_flags),  WindowExtraFlags,  ELEMENTS(WindowExtraFlags) },
    {iol_BOX,    "extent:",       offsetof(wimp_window_base, extent),       NULL,              0                          },
    {iol_FLAGS,  "title_flags:",  offsetof(wimp_window_base, title_flags),  WindowTitleFlags,  ELEMENTS(WindowTitleFlags) },
    {iol_ENUM,   "work_flags:",   offsetof(wimp_window_base, work_flags),   WindowWorkFlags,   ELEMENTS(WindowWorkFlags)  },
    {iol_BITS,   "sprite_area:",  offsetof(wimp_window_base, sprite_area),  NULL,              0                          },
    {iol_SHORT,  "xmin:",         offsetof(wimp_window_base, xmin),         NULL,              0                          },
    {iol_SHORT,  "ymin:",         offsetof(wimp_window_base, ymin),         NULL,              0                          }
  };


int window_text2template(DATA *data, const char *pszIn, int nOffset, wimp_window_base *window)
{
  wimp_icon *icon;
  const char *pszObject, *pszEnd;
  char *strings;
  int ref;

  put_objects(data, pszIn, 0, (char *) window, WimpWindowObjectList, ELEMENTS(WimpWindowObjectList));
  put_icon_data(data, pszIn, nOffset, (wimp_icon_data *) &window->title_data, window->title_flags);

  pszEnd = data->pszIn + data->cbIn;
  icon = (wimp_icon *) (window + 1);
  while ((pszObject = next_object(&pszIn, pszEnd)) != NULL)
    {
      icon_text2template(data, pszIn, nOffset, icon);
      icon++;
      if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL)
        break;
    }
  window->icon_count = (int) ((char *) icon - (char *) (window + 1)) / sizeof(wimp_icon);

  strings = (char *) icon;
  if ((ref = data->StringTable.ref) > 0)
    {
      memcpy(strings, data->StringTable.pstr, ref);
      strings += ref;
      reset_string_table(&data->StringTable);
    }
  return (int) (strings - (char *) window);
}


void window_template2text(DATA *data, FILE *hf, const char *pszBuff, int size)
{
  const wimp_window_base *window;
  const wimp_icon *i;

  TOOLBOXSMTABLE strMsgTable;
  strMsgTable.stringTableP = pszBuff;
  strMsgTable.stringTableSize = size;
  strMsgTable.messageTableP = NULL;
  strMsgTable.messageTableSize = 0;

  window = (const wimp_window_base *) pszBuff;
  get_objects(data, hf, &strMsgTable, (const char *)window, WimpWindowObjectList, ELEMENTS(WimpWindowObjectList), 1);
  get_icon_data(data, hf, &strMsgTable, (const wimp_icon_data *)&window->title_data, window->title_flags, 1);

  i = (const wimp_icon *) (window + 1);
  for (int n = 0; n < window->icon_count; n++, i++)
    {
      fputs("  wimp_icon {\n", hf);
      icon_template2text(data, hf, &strMsgTable, i);
      fputs("  }\n", hf);
    }
}
