/* _Window.c
   $Id: _Window.c,v 1.4 2005/01/30 14:55:57 joty Exp $

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

#include <stdio.h>
#include <string.h>

#include <OSLib/toolbox.h>
#include <OSLib/window.h>

#include <OSLib/actionbutton.h>
#include <OSLib/adjuster.h>
#include <OSLib/button.h>
#include <OSLib/displayfield.h>
#include <OSLib/draggable.h>
#include <OSLib/keyboardshortcut.h>
#include <OSLib/label.h>
#include <OSLib/labelledbox.h>
#include <OSLib/numberrange.h>
#include <OSLib/optionbutton.h>
#include <OSLib/popup.h>
#include <OSLib/radiobutton.h>
#include <OSLib/scrolllist.h>
#include <OSLib/slider.h>
#include <OSLib/stringset.h>
#include <OSLib/textarea.h>
#include <OSLib/textgadgets.h>
#include <OSLib/toolaction.h>
#include <OSLib/writablefield.h>

#include "ccres.h"
#include "Error.h"
#include "tabs_treeview.h"
#include "_Gadgets.h"
#include "_Icon.h"
#include "_Object.h"
#include "_Window.h"

static const GADGETS Gadgets[] = {
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

static const FLAGS WindowFlags[] = {
	{window_GENERATE_ABOUT_TO_BE_SHOWN , "window_GENERATE_ABOUT_TO_BE_SHOWN" },
	{window_AUTO_OPEN                  , "window_AUTO_OPEN"                  },
	{window_AUTO_CLOSE                 , "window_AUTO_CLOSE"                 },
	{window_GENERATE_DIALOGUE_COMPLETED, "window_GENERATE_DIALOGUE_COMPLETED"},
	{window_IS_TOOL_BAR                , "window_IS_TOOL_BAR"                }
};

static const FLAGS WindowWindowFlags[] = {
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

static const FLAGS WindowExtraFlags[] = {
	{wimp_WINDOW_USE_TITLE_VALIDATION_COLOURS, "wimp_WINDOW_USE_TITLE_VALIDATION_COLOURS"},
	{wimp_WINDOW_USE_TITLE_VALIDATION_STRING , "wimp_WINDOW_USE_TITLE_VALIDATION_STRING" },
	{wimp_WINDOW_USE_EXTENDED_SCROLL_REQUEST , "wimp_WINDOW_USE_EXTENDED_SCROLL_REQUEST" },
	{wimp_WINDOW_NEVER3D                     , "wimp_WINDOW_NEVER3D"                     },
	{wimp_WINDOW_ALWAYS3D                    , "wimp_WINDOW_ALWAYS3D"                    }
};

static const FLAGS WindowTitleFlags[] = {
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

static const FLAGS WindowWorkFlags[] = {
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

static const FLAGS WindowNextFlags[] = {
	{(int) wimp_TOP   , "wimp_TOP"   },
	{(int) wimp_BOTTOM, "wimp_BOTTOM"},
	{(int) wimp_HIDDEN, "wimp_HIDDEN"}
};

static const OBJECTLIST WindowObjectList[] = {
	{iol_FLAGS,  "window_flags:",            offsetof(window_object_base, flags),               WindowFlags,       ELEMENTS(WindowFlags)                   },
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

static const OBJECTLIST GadgetHeaderList[] = {
	{iol_BITS, "cmp:",          offsetof(gadget_object, cmp),          NULL,          bits_EVAL                          },
	{iol_BOX,  "bbox:",         offsetof(gadget_object, bbox),         NULL,          0                                  },
	{iol_MSG,  "help_message:", offsetof(gadget_object, help_message), "help_limit:", offsetof(gadget_object, help_limit)}
};

static const PSTR pszShortcutObject = "keyboardshortcut_object";
static const FLAGS ShortcutFlags[] = {
	{keyboardshortcut_SHOW_AS_MENU   , "keyboardshortcut_SHOW_AS_MENU"   },
	{keyboardshortcut_SHOW_AT_CENTRE , "keyboardshortcut_SHOW_AT_CENTRE" },
	{keyboardshortcut_SHOW_AT_POINTER, "keyboardshortcut_SHOW_AT_POINTER"}
};

static const OBJECTLIST ShortcutList[] = {
	{iol_FLAGS,  "flags:",     offsetof(keyboardshortcut_object, flags),  ShortcutFlags, ELEMENTS(ShortcutFlags)},
	{iol_BITS,   "key_code:",  offsetof(keyboardshortcut_object, c),      NULL,          0                      },
	{iol_BITS,   "key_event:", offsetof(keyboardshortcut_object, action), NULL,          0                      },
	{iol_STRING, "key_show:",  offsetof(keyboardshortcut_object, name),   NULL,          0                      }
};


int window_t2g(PDATA data, PSTR pszIn, toolbox_relocatable_object_base * object)
{
	window_object_base * window_object;
	keyboardshortcut_object * shortcut;
	gadget_object_base * gadget;
	PSTR pszObject, pszShortcut, pszEnd;
	int shortcut_count, g, gadget_count, nOffset, nSize;

	window_object = (window_object_base *) (object + 1);
	put_objects(data, pszIn, 0, (PSTR) window_object, WindowObjectList, ELEMENTS(WindowObjectList));

	pszEnd = data->pszIn + data->cbIn;
	pszShortcut = pszIn;
	shortcut = (keyboardshortcut_object *) ((PSTR) window_object + sizeof(window_object_base));
	shortcut_count = 0;
	while ((pszObject = next_object(&pszShortcut, pszEnd)) != NULL) {
		if (__stricmp(pszShortcutObject, pszObject) == 0) {
			nOffset = (int) ((PSTR) shortcut - (PSTR) window_object);
			put_objects(data, pszShortcut, nOffset, (PSTR) shortcut, ShortcutList, ELEMENTS(ShortcutList));
			shortcut++;
			shortcut_count++;
		}
		if ((pszShortcut = object_end(data, pszShortcut, pszEnd)) == NULL) {
			break;
		}
	}
	if (shortcut_count > 0) {
		window_object->shortcut_count = shortcut_count;
		window_object->shortcuts = (toolbox_object_offset) sizeof(window_object_base);
	} else {
		window_object->shortcuts = (toolbox_object_offset) -1;
	}

	gadget = (gadget_object_base *) shortcut;
	gadget_count = 0;
	while ((pszObject = next_object(&pszIn, pszEnd)) != NULL) {
LOG(("_window pszObject=%s @ %p", pszObject, gadget));
		for (g = 0; g < ELEMENTS(Gadgets); g++) {
			if (__stricmp(Gadgets[g].name, pszObject) == 0) {
				nOffset = (int) ((PSTR) gadget - (PSTR) window_object);
				put_objects(data, pszIn, nOffset, (PSTR) gadget, GadgetHeaderList, ELEMENTS(GadgetHeaderList));
				nSize = Gadgets[g].t2g(data, pszIn, nOffset, gadget);
				if (nSize & 0x3)
					error("Gadget class '%s' has non aligned size %d", pszObject, nSize);
				gadget->class_no_and_size = (nSize << 16) | Gadgets[g].class_no;
				gadget = (gadget_object_base *) ((PSTR) gadget + nSize);
				gadget_count++;
LOG(("size=%d next object @ %p", nSize, gadget));
				goto _window_gadget_added;
			}
		}
		if (__stricmp(pszShortcutObject, pszObject) != 0) {
			error("Unhandled gadget class '%s'", pszObject);
		}

_window_gadget_added:

		if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL) {
			break;
		}
	}
	if (gadget_count > 0) {
		window_object->gadget_count = gadget_count;
		window_object->gadgets = (toolbox_object_offset) ((PSTR) sizeof(window_object_base) + (shortcut_count * sizeof(keyboardshortcut_object)));
	} else {
		window_object->gadgets = (toolbox_object_offset) -1;
	}
	return (int) ((PSTR) gadget - (PSTR) window_object);
}


void window_g2t(FILE * hf, toolbox_resource_file_object_base * object, PSTR pszStringTable, PSTR pszMessageTable)
{
	window_object_base * window_object;
	gadget_object_base * gadget;
	keyboardshortcut_object * shortcut;
	int n, g, nSize, nClass;

	window_object = (window_object_base *) (object + 1);
	get_objects(hf, pszStringTable, pszMessageTable, (const char *)window_object, WindowObjectList, ELEMENTS(WindowObjectList), 1);

	shortcut = (keyboardshortcut_object *) ((PSTR) window_object + (int) window_object->shortcuts);
	for (n = 0; n < window_object->shortcut_count; n++, shortcut++) {
		fprintf(hf, "  %s {\n", pszShortcutObject);
		get_objects(hf, pszStringTable, pszMessageTable, (const char *)shortcut, ShortcutList, ELEMENTS(ShortcutList), 2);
		fputs("  }\n", hf);
	}

	gadget = (gadget_object_base *) ((PSTR) window_object + (int) window_object->gadgets);
	for (n = 0; n < window_object->gadget_count; n++) {
		nSize = (gadget->class_no_and_size >> 16);
		nClass = (gadget->class_no_and_size & 0xffff);
		for (g = 0; g < ELEMENTS(Gadgets); g++) {
			if (Gadgets[g].class_no == nClass) {
LOG((Gadgets[g].name));
				fprintf(hf, "  %s {\n", Gadgets[g].name);
				get_objects(hf, pszStringTable, pszMessageTable, (const char *)gadget, GadgetHeaderList, ELEMENTS(GadgetHeaderList), 2);
				Gadgets[g].g2t(hf, gadget, pszStringTable, pszMessageTable);
				fputs("  }\n", hf);
LOG((""));
				goto window_gadget_added;
			}
		}
		error("Unhandled gadget class &%x size %d", nClass, nSize);

window_gadget_added:

		gadget = (gadget_object_base *) ((char *) gadget + nSize);
	}
}

//=================================== Template window handlers ======================================
// a separate source file would have been nice, but sharing common data tricky without duplication...


static const OBJECTLIST WimpWindowObjectList[] = {
	{iol_BOX,    "visible:",      offsetof(wimp_window_base, visible),      NULL,              0                          },
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


        int window_text2template(PDATA data, PSTR pszIn, int nOffset, wimp_window_base * window)
//      ========================================================================================
{
wimp_icon * icon;
PSTR pszObject, pszEnd, strings;
int ref;

put_objects(data, pszIn, 0, (PSTR) window, WimpWindowObjectList, ELEMENTS(WimpWindowObjectList));
put_icon_data(data, pszIn, nOffset, (wimp_icon_data *) &window->title_data, window->title_flags);

pszEnd = data->pszIn + data->cbIn;
icon = (wimp_icon *) (window + 1);
while ((pszObject = next_object(&pszIn, pszEnd)) != NULL)
  {
LOG(("_window pszObject=%s offset=%d (%x)", pszObject, -nOffset, -nOffset));
  icon_text2template(data, pszIn, nOffset, icon);
  icon++;
  if ((pszIn = object_end(data, pszIn, pszEnd)) == NULL)
    break;
  }
window->icon_count = (int) ((PSTR) icon - (PSTR) (window + 1)) / sizeof(wimp_icon);

strings = (PSTR) icon;
if ((ref = data->StringTable.ref) > 0)
  {
  memcpy(strings, data->StringTable.pstr, ref);
  strings += ref;
  reset_string_table(&data->StringTable);
  }
return (int) (strings - (PSTR) window);
}


        void window_template2text(FILE * hf, PSTR pszBuff)
//      ==================================================
{
wimp_window_base * window;
wimp_icon * i;
int n;

window = (wimp_window_base *) pszBuff;
get_objects(hf, pszBuff, NULL, (const char *)window, WimpWindowObjectList, ELEMENTS(WimpWindowObjectList), 1);
get_icon_data(hf, pszBuff, (wimp_icon_data *) &window->title_data, window->title_flags, 1);

i = (wimp_icon *) (window + 1);
for (n = 0; n < window->icon_count; n++, i++)
  {
  fputs("  wimp_icon {\n", hf);
  icon_template2text(hf, pszBuff, i);
  fputs("  }\n", hf);
  }
}
