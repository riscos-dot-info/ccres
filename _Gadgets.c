/* _Gadgets.c

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

#include <stdio.h>

#include <oslib/actionbutton.h>
#include <oslib/adjuster.h>
#include <oslib/displayfield.h>
#include <oslib/draggable.h>
#include <oslib/label.h>
#include <oslib/labelledbox.h>
#include <oslib/numberrange.h>
#include <oslib/optionbutton.h>
#include <oslib/popup.h>
#include <oslib/radiobutton.h>
#include <oslib/scale.h>
#include <oslib/scrolllist.h>
#include <oslib/slider.h>
#include <oslib/stringset.h>
#include <oslib/textarea.h>
#include <oslib/textgadgets.h>
#include <oslib/toolaction.h>
#include <oslib/toolbox.h>
#include <oslib/writablefield.h>

#include "ccres.h"
#include "tabs_treeview.h"
#include "_Object.h"
#include "_Gadgets.h"

// ActionButton gadget

static const FLAGS ActionButtonFlags[] = {
	{actionbutton_IS_DEFAULT     , "actionbutton_IS_DEFAULT"     },
	{actionbutton_IS_CANCEL      , "actionbutton_IS_CANCEL"      },
	{actionbutton_IS_LOCAL       , "actionbutton_IS_LOCAL"       },
	{actionbutton_IS_MENU        , "actionbutton_IS_MENU"        },
	{actionbutton_SHOW_AS_MENU   , "actionbutton_SHOW_AS_MENU"   },
	{actionbutton_SHOW_AT_CENTRE , "actionbutton_SHOW_AT_CENTRE" },
	{actionbutton_SHOW_AT_POINTER, "actionbutton_SHOW_AT_POINTER"},
	{gadget_FADED                , "gadget_FADED"                },
	{gadget_AT_BACK              , "gadget_AT_BACK"              }
};

static const OBJECTLIST ActionbuttonObjectList[] = {
	{iol_FLAGS,  "flags:",      offsetof(actionbutton_object, flags),      ActionButtonFlags, ELEMENTS(ActionButtonFlags)              },
	{iol_MSG,    "text:",       offsetof(actionbutton_object, text),       "text_limit:",     offsetof(actionbutton_object, text_limit)},
	{iol_STRING, "click_show:", offsetof(actionbutton_object, click_show), NULL,              0                                        },
	{iol_BITS,   "action:",     offsetof(actionbutton_object, action),     NULL,              bits_ACTION                              }
};

        int actionbutton_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ======================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, ActionbuttonObjectList, ELEMENTS(ActionbuttonObjectList));

return sizeof(actionbutton_object);
}

        void actionbutton_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ====================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, ActionbuttonObjectList, ELEMENTS(ActionbuttonObjectList), 2);
}


// Adjuster gadget

static const FLAGS AdjusterFlags[] = {
	{adjuster_INCREMENT, "adjuster_INCREMENT"},
	{adjuster_VERTICAL , "adjuster_VERTICAL" },
	{gadget_FADED      , "gadget_FADED"      },
	{gadget_AT_BACK    , "gadget_AT_BACK"    }
};

static const OBJECTLIST AdjusterObjectList[] = {
	{iol_FLAGS,  "flags:",    offsetof(adjuster_object, flags),    AdjusterFlags, ELEMENTS(AdjusterFlags)},
	{iol_INT,    "reserved:", offsetof(adjuster_object, reserved), NULL,          0                      }
};


        int adjuster_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ==================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, AdjusterObjectList, ELEMENTS(AdjusterObjectList));

return sizeof(adjuster_object);
}

        void adjuster_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, AdjusterObjectList, ELEMENTS(AdjusterObjectList), 2);
}


// DisplayField gadget

static const FLAGS DisplayFieldFlags[] = {
	{displayfield_RJUSTIFIED     , "displayfield_RJUSTIFIED"     },
	{displayfield_HCENTRED       , "displayfield_HCENTRED"       },
	{gadget_FADED                , "gadget_FADED"                },
	{gadget_AT_BACK              , "gadget_AT_BACK"              }
};

static const OBJECTLIST DisplayFieldObjectList[] = {
	{iol_FLAGS,  "flags:", offsetof(displayfield_object, flags), DisplayFieldFlags, ELEMENTS(DisplayFieldFlags)},
	{iol_MSG,    "text:",  offsetof(displayfield_object, text),  "text_limit:",     offsetof(displayfield_object, text_limit)}
};

        int displayfield_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ======================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, DisplayFieldObjectList, ELEMENTS(DisplayFieldObjectList));

return sizeof(displayfield_object);
}

        void displayfield_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ====================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, DisplayFieldObjectList, ELEMENTS(DisplayFieldObjectList), 2);
}


// Draggable gadget

static const FLAGS DraggableFlags[] = {
	{draggable_GENERATE_DRAG_STARTED                           , "draggable_GENERATE_DRAG_STARTED" },
	{draggable_SPRITE                                          , "draggable_SPRITE"                },
	{draggable_TEXT                                            , "draggable_TEXT"                  },
	{(draggable_TYPE_CLICK_DRAG        << draggable_TYPE_SHIFT), "draggable_TYPE_CLICK_DRAG"       },
	{(draggable_TYPE_CLICK_DOUBLE_DRAG << draggable_TYPE_SHIFT), "draggable_TYPE_CLICK_DOUBLE_DRAG"},
	{draggable_USE_TOOLBOX_IDS                                 , "draggable_USE_TOOLBOX_IDS"       },
	{draggable_DROP_SHADOW                                     , "draggable_DROP_SHADOW"           },
	{draggable_NO_DITHER                                       , "draggable_NO_DITHER"             },
	{gadget_FADED                                              , "gadget_FADED"                    },
	{gadget_AT_BACK                                            , "gadget_AT_BACK"                  }
};

static const OBJECTLIST DraggableObjectList[] = {
	{iol_FLAGS,  "flags:",       offsetof(draggable_object, flags),       DraggableFlags,  ELEMENTS(DraggableFlags)},
	{iol_MSG,    "text:",        offsetof(draggable_object, text),        "text_limit:",   offsetof(draggable_object, text_limit)},
	{iol_STRING, "sprite_name:", offsetof(draggable_object, sprite_name), "sprite_limit:", offsetof(draggable_object, sprite_limit)},
};

        int draggable_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ===================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, DraggableObjectList, ELEMENTS(DraggableObjectList));

return sizeof(draggable_object);
}

        void draggable_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      =================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, DraggableObjectList, ELEMENTS(DraggableObjectList), 2);
}


// Label gadget

static const FLAGS LabelFlags[] = {
	{label_NO_BOX    , "label_NO_BOX"    },
	{label_RJUSTIFIED, "label_RJUSTIFIED"},
	{label_HCENTRED  , "label_HCENTRED"  },
	{gadget_FADED    , "gadget_FADED"    },
	{gadget_AT_BACK  , "gadget_AT_BACK"  }
};

static const OBJECTLIST LabelObjectList[] = {
	{iol_FLAGS, "flags:", offsetof(label_object, flags), LabelFlags, ELEMENTS(LabelFlags)},
	{iol_MSG,   "label:", offsetof(label_object, label), NULL,       0                   }
};

        int label_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ===============================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, LabelObjectList, ELEMENTS(LabelObjectList));

return sizeof(label_object);
}

        void label_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      =============================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, LabelObjectList, ELEMENTS(LabelObjectList), 2);
}


// LabelledBox gadget

static const FLAGS LabelledBoxFlags[] = {
	{labelledbox_SPRITE          , "labelledbox_SPRITE"          },
	{labelledbox_SPRITE_IS_FILLED, "labelledbox_SPRITE_IS_FILLED"},
	{gadget_FADED                , "gadget_FADED"                },
	{gadget_AT_BACK              , "gadget_AT_BACK"              }
};

static const OBJECTLIST LabelledBoxObjectList[] = {
	{iol_FLAGS, "flags:", offsetof(labelledbox_object, flags), LabelledBoxFlags, ELEMENTS(LabelledBoxFlags)},
	{iol_MSG,   "label:", offsetof(labelledbox_object, label), NULL,             0                         }
};

        int labelledbox_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      =====================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, LabelledBoxObjectList, ELEMENTS(LabelledBoxObjectList));

return sizeof(labelledbox_object);
}

        void labelledbox_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ===================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, LabelledBoxObjectList, ELEMENTS(LabelledBoxObjectList), 2);
}


// NumberRange gadget

#define numberrange_ABS_SLIDER_TYPE_RIGHT (numberrange_SLIDER_TYPE_RIGHT << numberrange_SLIDER_TYPE_SHIFT)
#define numberrange_ABS_SLIDER_TYPE_LEFT  (numberrange_SLIDER_TYPE_LEFT << numberrange_SLIDER_TYPE_SHIFT)
#define numberrange_KNOB_COLOUR_VERY_LIGHT_GREY    (wimp_COLOUR_VERY_LIGHT_GREY << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_LIGHT_GREY         (wimp_COLOUR_LIGHT_GREY      << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_MID_LIGHT_GREY     (wimp_COLOUR_MID_LIGHT_GREY  << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_MID_DARK_GREY      (wimp_COLOUR_MID_DARK_GREY   << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_DARK_GREY          (wimp_COLOUR_DARK_GREY       << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_VERY_DARK_GREY     (wimp_COLOUR_VERY_DARK_GREY  << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_BLACK              (wimp_COLOUR_BLACK           << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_DARK_BLUE          (wimp_COLOUR_DARK_BLUE       << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_YELLOW             (wimp_COLOUR_YELLOW          << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_LIGHT_GREEN        (wimp_COLOUR_LIGHT_GREEN     << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_RED                (wimp_COLOUR_RED             << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_CREAM              (wimp_COLOUR_CREAM           << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_DARK_GREEN         (wimp_COLOUR_DARK_GREEN      << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_ORANGE             (wimp_COLOUR_ORANGE          << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_KNOB_COLOUR_LIGHT_BLUE         (wimp_COLOUR_LIGHT_BLUE      << numberrange_KNOB_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_VERY_LIGHT_GREY    (wimp_COLOUR_VERY_LIGHT_GREY << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_LIGHT_GREY         (wimp_COLOUR_LIGHT_GREY      << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_MID_LIGHT_GREY     (wimp_COLOUR_MID_LIGHT_GREY  << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_MID_DARK_GREY      (wimp_COLOUR_MID_DARK_GREY   << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_DARK_GREY          (wimp_COLOUR_DARK_GREY       << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_VERY_DARK_GREY     (wimp_COLOUR_VERY_DARK_GREY  << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_BLACK              (wimp_COLOUR_BLACK           << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_DARK_BLUE          (wimp_COLOUR_DARK_BLUE       << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_YELLOW             (wimp_COLOUR_YELLOW          << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_LIGHT_GREEN        (wimp_COLOUR_LIGHT_GREEN     << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_RED                (wimp_COLOUR_RED             << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_CREAM              (wimp_COLOUR_CREAM           << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_DARK_GREEN         (wimp_COLOUR_DARK_GREEN      << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_ORANGE             (wimp_COLOUR_ORANGE          << numberrange_WELL_COLOUR_SHIFT)
#define numberrange_WELL_COLOUR_LIGHT_BLUE         (wimp_COLOUR_LIGHT_BLUE      << numberrange_WELL_COLOUR_SHIFT)

static const FLAGS NumberRangeFlags[] = {
	{numberrange_GENERATE_VALUE_CHANGED     , "numberrange_GENERATE_VALUE_CHANGED"     },
	{numberrange_WRITABLE                   , "numberrange_WRITABLE"                   },
	{numberrange_HAS_NUMERICAL_DISPLAY      , "numberrange_HAS_NUMERICAL_DISPLAY"      },
	{numberrange_ADJUSTERS                  , "numberrange_ADJUSTERS"                  },
	{numberrange_ABS_SLIDER_TYPE_RIGHT      , "numberrange_SLIDER_TYPE_RIGHT"          },
	{numberrange_ABS_SLIDER_TYPE_LEFT       , "numberrange_SLIDER_TYPE_LEFT"           },
	{numberrange_RJUSTIFIED                 , "numberrange_RJUSTIFIED"                 },
	{numberrange_HCENTRED                   , "numberrange_HCENTRED"                   },
	{numberrange_KNOB_COLOUR_VERY_LIGHT_GREY, "numberrange_KNOB_COLOUR_VERY_LIGHT_GREY"},
	{numberrange_KNOB_COLOUR_LIGHT_GREY     , "numberrange_KNOB_COLOUR_LIGHT_GREY"     },
	{numberrange_KNOB_COLOUR_MID_LIGHT_GREY , "numberrange_KNOB_COLOUR_MID_LIGHT_GREY" },
	{numberrange_KNOB_COLOUR_MID_DARK_GREY  , "numberrange_KNOB_COLOUR_MID_DARK_GREY"  },
	{numberrange_KNOB_COLOUR_DARK_GREY      , "numberrange_KNOB_COLOUR_DARK_GREY"      },
	{numberrange_KNOB_COLOUR_VERY_DARK_GREY , "numberrange_KNOB_COLOUR_VERY_DARK_GREY" },
	{numberrange_KNOB_COLOUR_BLACK          , "numberrange_KNOB_COLOUR_BLACK"          },
	{numberrange_KNOB_COLOUR_DARK_BLUE      , "numberrange_KNOB_COLOUR_DARK_BLUE"      },
	{numberrange_KNOB_COLOUR_YELLOW         , "numberrange_KNOB_COLOUR_YELLOW"         },
	{numberrange_KNOB_COLOUR_LIGHT_GREEN    , "numberrange_KNOB_COLOUR_LIGHT_GREEN"    },
	{numberrange_KNOB_COLOUR_RED            , "numberrange_KNOB_COLOUR_RED"            },
	{numberrange_KNOB_COLOUR_CREAM          , "numberrange_KNOB_COLOUR_CREAM"          },
	{numberrange_KNOB_COLOUR_DARK_GREEN     , "numberrange_KNOB_COLOUR_DARK_GREEN"     },
	{numberrange_KNOB_COLOUR_ORANGE         , "numberrange_KNOB_COLOUR_ORANGE"         },
	{numberrange_KNOB_COLOUR_LIGHT_BLUE     , "numberrange_KNOB_COLOUR_LIGHT_BLUE"     },
	{numberrange_WELL_COLOUR_VERY_LIGHT_GREY, "numberrange_WELL_COLOUR_VERY_LIGHT_GREY"},
	{numberrange_WELL_COLOUR_LIGHT_GREY     , "numberrange_WELL_COLOUR_LIGHT_GREY"     },
	{numberrange_WELL_COLOUR_MID_LIGHT_GREY , "numberrange_WELL_COLOUR_MID_LIGHT_GREY" },
	{numberrange_WELL_COLOUR_MID_DARK_GREY  , "numberrange_WELL_COLOUR_MID_DARK_GREY"  },
	{numberrange_WELL_COLOUR_DARK_GREY      , "numberrange_WELL_COLOUR_DARK_GREY"      },
	{numberrange_WELL_COLOUR_VERY_DARK_GREY , "numberrange_WELL_COLOUR_VERY_DARK_GREY" },
	{numberrange_WELL_COLOUR_BLACK          , "numberrange_WELL_COLOUR_BLACK"          },
	{numberrange_WELL_COLOUR_DARK_BLUE      , "numberrange_WELL_COLOUR_DARK_BLUE"      },
	{numberrange_WELL_COLOUR_YELLOW         , "numberrange_WELL_COLOUR_YELLOW"         },
	{numberrange_WELL_COLOUR_LIGHT_GREEN    , "numberrange_WELL_COLOUR_LIGHT_GREEN"    },
	{numberrange_WELL_COLOUR_RED            , "numberrange_WELL_COLOUR_RED"            },
	{numberrange_WELL_COLOUR_CREAM          , "numberrange_WELL_COLOUR_CREAM"          },
	{numberrange_WELL_COLOUR_DARK_GREEN     , "numberrange_WELL_COLOUR_DARK_GREEN"     },
	{numberrange_WELL_COLOUR_ORANGE         , "numberrange_WELL_COLOUR_ORANGE"         },
	{numberrange_WELL_COLOUR_LIGHT_BLUE     , "numberrange_WELL_COLOUR_LIGHT_BLUE"     },
	{gadget_FADED                           , "gadget_FADED"                           },
	{gadget_AT_BACK                         , "gadget_AT_BACK"                         }
};

static const OBJECTLIST NumberRangeObjectList[] = {
	{iol_FLAGS,  "flags:",          offsetof(numberrange_object, flags),          NumberRangeFlags, ELEMENTS(NumberRangeFlags)},
	{iol_INT,    "lower_bound:",    offsetof(numberrange_object, lower_bound),    NULL,             0                         },
	{iol_INT,    "upper_bound:",    offsetof(numberrange_object, upper_bound),    NULL,             0                         },
	{iol_INT,    "step_size:",      offsetof(numberrange_object, step_size),      NULL,             0                         },
	{iol_INT,    "initial_value:",  offsetof(numberrange_object, initial_value),  NULL,             0                         },
	{iol_INT,    "precision:",      offsetof(numberrange_object, precision),      NULL,             0                         },
	{iol_CMP,    "prev:",           offsetof(numberrange_object, prev),           NULL,             0                         },
	{iol_CMP,    "next:",           offsetof(numberrange_object, next),           NULL,             0                         },
	{iol_INT,    "display_length:", offsetof(numberrange_object, display_length), NULL,             0                         }
};

        int numberrange_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      =====================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, NumberRangeObjectList, ELEMENTS(NumberRangeObjectList));

return sizeof(numberrange_object);
}

        void numberrange_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ===================================================================================================================
{
	get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, NumberRangeObjectList, ELEMENTS(NumberRangeObjectList), 2);
}


// Optionbutton gadget

static const FLAGS OptionButtonFlags[] = {
	{optionbutton_GENERATE_STATE_CHANGED, "optionbutton_GENERATE_STATE_CHANGED"},
	{optionbutton_ON                    , "optionbutton_ON"                    },
	{gadget_FADED                       , "gadget_FADED"                       },
	{gadget_AT_BACK                     , "gadget_AT_BACK"                     }
};

static const OBJECTLIST OptionButtonObjectList[] = {
	{iol_FLAGS,  "flags:",  offsetof(optionbutton_object, flags),  OptionButtonFlags,  ELEMENTS(OptionButtonFlags)               },
	{iol_MSG,    "label:",  offsetof(optionbutton_object, label),  "label_limit:",     offsetof(optionbutton_object, label_limit)},
	{iol_BITS,   "action:", offsetof(optionbutton_object, action), NULL,               bits_ACTION                                }
};

        int optionbutton_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ======================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, OptionButtonObjectList, ELEMENTS(OptionButtonObjectList));

return sizeof(optionbutton_object);
}

        void optionbutton_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ====================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, OptionButtonObjectList, ELEMENTS(OptionButtonObjectList), 2);
}


// Popup gadget

static const FLAGS PopupFlags[] = {
	{popup_GENERATE_ABOUT_TO_BE_SHOWN, "popup_GENERATE_ABOUT_TO_BE_SHOWN"},
	{gadget_FADED                    , "gadget_FADED"                    },
	{gadget_AT_BACK                  , "gadget_AT_BACK"                  }
};

static const OBJECTLIST PopupObjectList[] = {
	{iol_FLAGS,  "flags:",     offsetof(popup_object, flags),      PopupFlags, ELEMENTS(PopupFlags)},
	{iol_STRING, "menu_name:", offsetof(popup_object, menu_name),  NULL,       0                   }
};

        int popup_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ===============================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, PopupObjectList, ELEMENTS(PopupObjectList));

return sizeof(popup_object);
}

        void popup_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      =============================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, PopupObjectList, ELEMENTS(PopupObjectList), 2);
}


// RadioButton gadget

static const FLAGS RadioButtonFlags[] = {
	{radiobutton_GENERATE_STATE_CHANGED    , "radiobutton_GENERATE_STATE_CHANGED"    },
	{radiobutton_GENERATE_SET_STATE_CHANGED, "radiobutton_GENERATE_SET_STATE_CHANGED"},
	{radiobutton_ON                        , "radiobutton_ON"                        },
	{gadget_FADED                          , "gadget_FADED"                          },
	{gadget_AT_BACK                        , "gadget_AT_BACK"                        }
};

static const OBJECTLIST RadioButtonObjectList[] = {
	{iol_FLAGS, "flags:",  offsetof(radiobutton_object, flags),  RadioButtonFlags, ELEMENTS(RadioButtonFlags)               },
	{iol_INT,   "group:",  offsetof(radiobutton_object, group),  NULL,             0                                        },
	{iol_MSG,   "label:",  offsetof(radiobutton_object, label),  "label_limit:",   offsetof(radiobutton_object, label_limit)},
	{iol_BITS,  "action:", offsetof(radiobutton_object, action), NULL,             bits_ACTION                               }
};

        int radiobutton_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      =====================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, RadioButtonObjectList, ELEMENTS(RadioButtonObjectList));

return sizeof(radiobutton_object);
}

        void radiobutton_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ===================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, RadioButtonObjectList, ELEMENTS(RadioButtonObjectList), 2);
}


// ScrollList gadget

static const FLAGS ScrolllistFlags[] = {
	{scrolllist_STATE_ALLOW_MULTIPLE_SELECTIONS, "scrolllist_STATE_ALLOW_MULTIPLE_SELECTIONS"},
	{gadget_FADED                              , "gadget_FADED"                              },
	{gadget_AT_BACK                            , "gadget_AT_BACK"                            }
};

static const OBJECTLIST ScrolllistObjectList[] = {
	{iol_FLAGS, "flags:",      offsetof(scrolllist_object, flags),      ScrolllistFlags, ELEMENTS(ScrolllistFlags)},
	{iol_INT,   "event:",      offsetof(scrolllist_object, event),      NULL,            0                        },
	{iol_OSCOL, "foreground:", offsetof(scrolllist_object, foreground), NULL,            0                        },
	{iol_OSCOL, "background:", offsetof(scrolllist_object, background), NULL,            0                        }
};

        int scrolllist_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ====================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, ScrolllistObjectList, ELEMENTS(ScrolllistObjectList));

return sizeof(scrolllist_object);
}

        void scrolllist_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ==================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, ScrolllistObjectList, ELEMENTS(ScrolllistObjectList), 2);
}

#define slider_KNOB_COLOUR_VERY_LIGHT_GREY    (wimp_COLOUR_VERY_LIGHT_GREY << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_LIGHT_GREY         (wimp_COLOUR_LIGHT_GREY      << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_MID_LIGHT_GREY     (wimp_COLOUR_MID_LIGHT_GREY  << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_MID_DARK_GREY      (wimp_COLOUR_MID_DARK_GREY   << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_DARK_GREY          (wimp_COLOUR_DARK_GREY       << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_VERY_DARK_GREY     (wimp_COLOUR_VERY_DARK_GREY  << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_BLACK              (wimp_COLOUR_BLACK           << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_DARK_BLUE          (wimp_COLOUR_DARK_BLUE       << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_YELLOW             (wimp_COLOUR_YELLOW          << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_LIGHT_GREEN        (wimp_COLOUR_LIGHT_GREEN     << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_RED                (wimp_COLOUR_RED             << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_CREAM              (wimp_COLOUR_CREAM           << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_DARK_GREEN         (wimp_COLOUR_DARK_GREEN      << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_ORANGE             (wimp_COLOUR_ORANGE          << slider_KNOB_COLOUR_SHIFT)
#define slider_KNOB_COLOUR_LIGHT_BLUE         (wimp_COLOUR_LIGHT_BLUE      << slider_KNOB_COLOUR_SHIFT)
#define slider_WELL_COLOUR_VERY_LIGHT_GREY    (wimp_COLOUR_VERY_LIGHT_GREY << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_LIGHT_GREY         (wimp_COLOUR_LIGHT_GREY      << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_MID_LIGHT_GREY     (wimp_COLOUR_MID_LIGHT_GREY  << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_MID_DARK_GREY      (wimp_COLOUR_MID_DARK_GREY   << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_DARK_GREY          (wimp_COLOUR_DARK_GREY       << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_VERY_DARK_GREY     (wimp_COLOUR_VERY_DARK_GREY  << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_BLACK              (wimp_COLOUR_BLACK           << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_DARK_BLUE          (wimp_COLOUR_DARK_BLUE       << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_YELLOW             (wimp_COLOUR_YELLOW          << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_LIGHT_GREEN        (wimp_COLOUR_LIGHT_GREEN     << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_RED                (wimp_COLOUR_RED             << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_CREAM              (wimp_COLOUR_CREAM           << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_DARK_GREEN         (wimp_COLOUR_DARK_GREEN      << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_ORANGE             (wimp_COLOUR_ORANGE          << slider_WELL_COLOUR_SHIFT)
#define slider_WELL_COLOUR_LIGHT_BLUE         (wimp_COLOUR_LIGHT_BLUE      << slider_WELL_COLOUR_SHIFT)

static const FLAGS SliderFlags[] = {
	{slider_GENERATE_VALUE_CHANGED            , "slider_GENERATE_VALUE_CHANGED"            },
	{slider_GENERATE_VALUE_CHANGED_BY_DRAGGING, "slider_GENERATE_VALUE_CHANGED_BY_DRAGGING"},
	{slider_GENERATE_SET_VALUE_CHANGED        , "slider_GENERATE_SET_VALUE_CHANGED"        },
	{slider_VERTICAL                          , "slider_VERTICAL"                          },
	{slider_DRAGGABLE                         , "slider_DRAGGABLE"                         },
	{slider_KNOB_COLOUR_VERY_LIGHT_GREY       , "slider_KNOB_COLOUR_VERY_LIGHT_GREY"       },
	{slider_KNOB_COLOUR_LIGHT_GREY            , "slider_KNOB_COLOUR_LIGHT_GREY"            },
	{slider_KNOB_COLOUR_MID_LIGHT_GREY        , "slider_KNOB_COLOUR_MID_LIGHT_GREY"        },
	{slider_KNOB_COLOUR_MID_DARK_GREY         , "slider_KNOB_COLOUR_MID_DARK_GREY"         },
	{slider_KNOB_COLOUR_DARK_GREY             , "slider_KNOB_COLOUR_DARK_GREY"             },
	{slider_KNOB_COLOUR_VERY_DARK_GREY        , "slider_KNOB_COLOUR_VERY_DARK_GREY"        },
	{slider_KNOB_COLOUR_BLACK                 , "slider_KNOB_COLOUR_BLACK"                 },
	{slider_KNOB_COLOUR_DARK_BLUE             , "slider_KNOB_COLOUR_DARK_BLUE"             },
	{slider_KNOB_COLOUR_YELLOW                , "slider_KNOB_COLOUR_YELLOW"                },
	{slider_KNOB_COLOUR_LIGHT_GREEN           , "slider_KNOB_COLOUR_LIGHT_GREEN"           },
	{slider_KNOB_COLOUR_RED                   , "slider_KNOB_COLOUR_RED"                   },
	{slider_KNOB_COLOUR_CREAM                 , "slider_KNOB_COLOUR_CREAM"                 },
	{slider_KNOB_COLOUR_DARK_GREEN            , "slider_KNOB_COLOUR_DARK_GREEN"            },
	{slider_KNOB_COLOUR_ORANGE                , "slider_KNOB_COLOUR_ORANGE"                },
	{slider_KNOB_COLOUR_LIGHT_BLUE            , "slider_KNOB_COLOUR_LIGHT_BLUE"            },
	{slider_WELL_COLOUR_VERY_LIGHT_GREY       , "slider_WELL_COLOUR_VERY_LIGHT_GREY"       },
	{slider_WELL_COLOUR_LIGHT_GREY            , "slider_WELL_COLOUR_LIGHT_GREY"            },
	{slider_WELL_COLOUR_MID_LIGHT_GREY        , "slider_WELL_COLOUR_MID_LIGHT_GREY"        },
	{slider_WELL_COLOUR_MID_DARK_GREY         , "slider_WELL_COLOUR_MID_DARK_GREY"         },
	{slider_WELL_COLOUR_DARK_GREY             , "slider_WELL_COLOUR_DARK_GREY"             },
	{slider_WELL_COLOUR_VERY_DARK_GREY        , "slider_WELL_COLOUR_VERY_DARK_GREY"        },
	{slider_WELL_COLOUR_BLACK                 , "slider_WELL_COLOUR_BLACK"                 },
	{slider_WELL_COLOUR_DARK_BLUE             , "slider_WELL_COLOUR_DARK_BLUE"             },
	{slider_WELL_COLOUR_YELLOW                , "slider_WELL_COLOUR_YELLOW"                },
	{slider_WELL_COLOUR_LIGHT_GREEN           , "slider_WELL_COLOUR_LIGHT_GREEN"           },
	{slider_WELL_COLOUR_RED                   , "slider_WELL_COLOUR_RED"                   },
	{slider_WELL_COLOUR_CREAM                 , "slider_WELL_COLOUR_CREAM"                 },
	{slider_WELL_COLOUR_DARK_GREEN            , "slider_WELL_COLOUR_DARK_GREEN"            },
	{slider_WELL_COLOUR_ORANGE                , "slider_WELL_COLOUR_ORANGE"                },
	{slider_WELL_COLOUR_LIGHT_BLUE            , "slider_WELL_COLOUR_LIGHT_BLUE"            },
	{gadget_FADED                             , "gadget_FADED"                             },
	{gadget_AT_BACK                           , "gadget_AT_BACK"                           }
};

static const OBJECTLIST SliderObjectList[] = {
	{iol_FLAGS, "flags:",         offsetof(slider_object, flags),         SliderFlags, ELEMENTS(SliderFlags)},
	{iol_INT,   "lower_bound:",   offsetof(slider_object, lower_bound),   NULL,        0                    },
	{iol_INT,   "upper_bound:",   offsetof(slider_object, upper_bound),   NULL,        0                    },
	{iol_INT,   "step_size:",     offsetof(slider_object, step_size),     NULL,        0                    },
	{iol_INT,   "initial_value:", offsetof(slider_object, initial_value), NULL,        0                    }
};

        int slider_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, SliderObjectList, ELEMENTS(SliderObjectList));

return sizeof(slider_object);
}

        void slider_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ==============================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, SliderObjectList, ELEMENTS(SliderObjectList), 2);
}


// StringSet gadget

#define stringset_ABS_JUSTIFICATION_RIGHT   (stringset_JUSTIFICATION_RIGHT   << stringset_JUSTIFICATION_SHIFT)
#define stringset_ABS_JUSTIFICATION_CENTRED (stringset_JUSTIFICATION_CENTRED << stringset_JUSTIFICATION_SHIFT)

static const FLAGS StringsetFlags[] = {
	{stringset_GENERATE_USER_VALUE_CHANGED, "stringset_GENERATE_USER_VALUE_CHANGED"},
	{stringset_GENERATE_SET_VALUE_CHANGED , "stringset_GENERATE_SET_VALUE_CHANGED" },
	{stringset_WRITABLE                   , "stringset_WRITABLE"                   },
	{stringset_GENERATE_ABOUT_TO_BE_SHOWN , "stringset_GENERATE_ABOUT_TO_BE_SHOWN" },
	{stringset_NO_DISPLAY                 , "stringset_NO_DISPLAY"                 },
	{stringset_ABS_JUSTIFICATION_RIGHT    , "stringset_JUSTIFICATION_RIGHT"        },
	{stringset_ABS_JUSTIFICATION_CENTRED  , "stringset_JUSTIFICATION_CENTRED"      },
	{gadget_FADED                         , "gadget_FADED"                         },
	{gadget_AT_BACK                       , "gadget_AT_BACK"                       }
};

static const OBJECTLIST StringsetObjectList[] = {
	{iol_FLAGS, "flags:",                   offsetof(stringset_object, flags),                   StringsetFlags,           ELEMENTS(StringsetFlags)                         },
	{iol_MSG,   "string_set:",              offsetof(stringset_object, string_set),              NULL,                     0                                                },
	{iol_MSG,   "title:",                   offsetof(stringset_object, title),                   NULL,                     0                                                },
	{iol_MSG,   "initial_selected_string:", offsetof(stringset_object, initial_selected_string), "selected_string_limit:", offsetof(stringset_object, selected_string_limit)},
	{iol_MSG,   "allowable:",               offsetof(stringset_object, allowable),               "allowable_limit:",       offsetof(stringset_object, allowable_limit)      },
	{iol_CMP,   "prev:",                    offsetof(stringset_object, prev),                    NULL,                     0                                                },
	{iol_CMP,   "next:",                    offsetof(stringset_object, next),                    NULL,                     0                                                }
};

        int stringset_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ===================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, StringsetObjectList, ELEMENTS(StringsetObjectList));

return sizeof(stringset_object);
}

        void stringset_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      =================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, StringsetObjectList, ELEMENTS(StringsetObjectList), 2);
}

// TextArea gadget

static const FLAGS TextAreaFlags[] = {
	{textarea_STATE_HAS_VERTICAL_SCROLLBAR  , "textarea_STATE_HAS_VERTICAL_SCROLLBAR"  },
	{textarea_STATE_HAS_HORIZONTAL_SCROLLBAR, "textarea_STATE_HAS_HORIZONTAL_SCROLLBAR"},
	{textarea_STATE_WORDWRAPPING_ENABLED    , "textarea_STATE_WORDWRAPPING_ENABLED"    },
	{textarea_ALLOW_SELECTION               , "textarea_ALLOW_SELECTION"               },
	{textarea_ALLOW_SAVE                    , "textarea_ALLOW_SAVE"                    },
	{textarea_ALLOW_LOAD                    , "textarea_ALLOW_LOAD"                    },
	{textarea_CLIPBOARD                     , "textarea_CLIPBOARD"                     },
	{textarea_AUTO_SCROLL                   , "textarea_AUTO_SCROLL"                   },
	{gadget_FADED                           , "gadget_FADED"                           },
	{gadget_AT_BACK                         , "gadget_AT_BACK"                         }
};

static const OBJECTLIST TextAreaObjectList[] = {
	{iol_FLAGS,  "flags:",      offsetof(textarea_object, flags),      TextAreaFlags, ELEMENTS(TextAreaFlags)},
	{iol_INT,    "type:",       offsetof(textarea_object, type),       NULL,          0                      },
	{iol_INT,    "event:",      offsetof(textarea_object, event),      NULL,          0                      },
	{iol_MSG,    "text:",       offsetof(textarea_object, text),       NULL,          0                      },
	{iol_OSCOL,  "foreground:", offsetof(textarea_object, foreground), NULL,          0                      },
	{iol_OSCOL,  "background:", offsetof(textarea_object, background), NULL,          0                      }
};

        int textarea_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ==================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, TextAreaObjectList, ELEMENTS(TextAreaObjectList));

return sizeof(textarea_object);
}

        void textarea_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, TextAreaObjectList, ELEMENTS(TextAreaObjectList), 2);
}


// ToolAction gadget

static const FLAGS ToolActionFlags[] = {
	{toolaction_GENERATE_SELECT  , "toolaction_GENERATE_SELECT"  },
	{toolaction_IS_TEXT          , "toolaction_IS_TEXT"          },
	{toolaction_ON               , "toolaction_ON"               },
	{toolaction_AUTO_TOGGLE      , "toolaction_AUTO_TOGGLE"      },
	{toolaction_NO_PRESSED_SPRITE, "toolaction_NO_PRESSED_SPRITE"},
	{toolaction_AUTO_REPEAT      , "toolaction_AUTO_REPEAT"      },
	{toolaction_SHOW_TRANSIENT   , "toolaction_SHOW_TRANSIENT"   },
	{toolaction_SHOW_AS_POP_UP   , "toolaction_SHOW_AS_POP_UP"   },
	{toolaction_HAS_FADE_SPRITE  , "toolaction_HAS_FADE_SPRITE"  },
	{gadget_FADED                , "gadget_FADED"                },
	{gadget_AT_BACK              , "gadget_AT_BACK"              }
};

static const OBJECTLIST ToolActionObjectList[] = {
	{iol_FLAGS,  "flags:",         offsetof(toolaction_object, flags),          ToolActionFlags,    ELEMENTS(ToolActionFlags)                   },
	{iol_STRING, "off_text:",      offsetof(toolaction_object, off_text),       "off_text_limit:",  offsetof(toolaction_object, off_text_limit) },
	{iol_STRING, "on_text:",       offsetof(toolaction_object, on_text),        "on_text_limit:",   offsetof(toolaction_object, on_text_limit)  },
	{iol_BITS,   "click_action:",  offsetof(toolaction_object, click_action),   NULL,               bits_ACTION                                 },
	{iol_STRING, "select_show:",   offsetof(toolaction_object, select_show),    NULL,               0                                           },
	{iol_BITS,   "adjust_action:", offsetof(toolaction_object, adjust_action),  NULL,               bits_ACTION                                 },
	{iol_STRING, "adjust_show:",   offsetof(toolaction_object, adjust_show),    NULL,               0                                           },
	{iol_STRING, "fade_text:",     offsetof(toolaction_object, fade_text),      "fade_text_limit:", offsetof(toolaction_object, fade_text_limit)}
};

        int toolaction_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ====================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, ToolActionObjectList, ELEMENTS(ToolActionObjectList));

return sizeof(toolaction_object);
}

        void toolaction_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ==================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, ToolActionObjectList, ELEMENTS(ToolActionObjectList), 2);
}


// Writablefield gadget

static const FLAGS WritableFieldFlags[] = {
	{writablefield_GENERATE_USER_VALUE_CHANGED, "writablefield_GENERATE_USER_VALUE_CHANGED"},
	{writablefield_GENERATE_SET_VALUE_CHANGED , "writablefield_GENERATE_SET_VALUE_CHANGED" },
	{writablefield_RJUSTIFIED                 , "writablefield_RJUSTIFIED"                 },
	{writablefield_HCENTRED                   , "writablefield_HCENTRED"                   },
	{writablefield_CONCEAL_TEXT               , "writablefield_CONCEAL_TEXT"               },
	{gadget_FADED                             , "gadget_FADED"                             },
	{gadget_AT_BACK                           , "gadget_AT_BACK"                           }
};

static const OBJECTLIST WritableFieldObjectList[] = {
	{iol_FLAGS, "flags:",     offsetof(writablefield_object, flags),     WritableFieldFlags, ELEMENTS(WritableFieldFlags)                   },
	{iol_MSG,   "text:",      offsetof(writablefield_object, text),      "text_limit:",      offsetof(writablefield_object, text_limit)     },
	{iol_MSG,   "allowable:", offsetof(writablefield_object, allowable), "allowable_limit:", offsetof(writablefield_object, allowable_limit)},
	{iol_CMP,   "prev:",      offsetof(writablefield_object, prev),      NULL,               0                                              },
	{iol_CMP,   "next:",      offsetof(writablefield_object, next),      NULL,               0                                              }
};

        int writablefield_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      =======================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, WritableFieldObjectList, ELEMENTS(WritableFieldObjectList));

return sizeof(writablefield_object);
}

        void writablefield_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      =====================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, WritableFieldObjectList, ELEMENTS(WritableFieldObjectList), 2);
}


// Tabs gadget

static const FLAGS TabsFlags[] = {
	{tabs_ALLOW_SCROLL_ARROWS, "tabs_ALLOW_SCROLL_ARROWS"},
	{gadget_FADED            , "gadget_FADED"            },
	{gadget_AT_BACK          , "gadget_AT_BACK"          }
};

static const OBJECTLIST TabsObjectList[] = {
	{iol_FLAGS, "flags:",      offsetof(tabs_object, flags),      TabsFlags, ELEMENTS(TabsFlags) },
	{iol_WCOL,  "foreground:", offsetof(tabs_object, foreground), NULL,      0                   },
	{iol_WCOL,  "background:", offsetof(tabs_object, background), NULL,      0                   },
	{iol_MSG,   "text:",       offsetof(tabs_object, text),       NULL,      0                   }
};

        int tabs_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ==============================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, TabsObjectList, ELEMENTS(TabsObjectList));

return sizeof(tabs_object);
}

        void tabs_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ============================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, TabsObjectList, ELEMENTS(TabsObjectList), 2);
}


// Treeview gadget

static const FLAGS TreeviewFlags[] = {
	{treeview_ALLOW_VSCROLL    , "treeview_ALLOW_VSCROLL"    },
	{treeview_ALLOW_HSCROLL    , "treeview_ALLOW_HSCROLL"    },
	{treeview_AUTO_UPDATE      , "treeview_AUTO_UPDATE"      },
	{treeview_SHOW_CURRENT_NODE, "treeview_SHOW_CURRENT_NODE"},
	{treeview_PLOT_LINES       , "treeview_PLOT_LINES"       },
	{treeview_ALLOW_SELECTION  , "treeview_ALLOW_SELECTION"  },
	{treeview_NOTIFY_SELECTION , "treeview_NOTIFY_SELECTION" },
	{treeview_SELECT_CHILDREN  , "treeview_SELECT_CHILDREN"  },
	{treeview_ALLOW_EXPAND     , "treeview_ALLOW_EXPAND"     },
	{treeview_NOTIFY_EXPANSION , "treeview_NOTIFY_EXPANSION" },
	{treeview_EXPAND_BY_DEFAULT, "treeview_EXPAND_BY_DEFAULT"},
	{treeview_ALLOW_RENAME     , "treeview_ALLOW_RENAME"     },
	{treeview_NOTIFY_RENAME    , "treeview_NOTIFY_RENAME"    },
	{treeview_ALLOW_DRAGS      , "treeview_ALLOW_DRAGS"      },
	{treeview_NOTIFY_DRAGEND   , "treeview_NOTIFY_DRAGEND"   },
	{treeview_ALL_EVENTS       , "treeview_ALL_EVENTS"       },
	{gadget_FADED              , "gadget_FADED"              },
	{gadget_AT_BACK            , "gadget_AT_BACK"            }
};

static const OBJECTLIST TreeviewObjectList[] = {
	{iol_FLAGS, "flags:",      offsetof(treeview_object, flags),      TreeviewFlags, ELEMENTS(TreeviewFlags) },
	{iol_WCOL,  "foreground:", offsetof(treeview_object, foreground), NULL,          0                   },
	{iol_WCOL,  "background:", offsetof(treeview_object, background), NULL,          0                   },
};

        int treeview_t2g(PDATA data, PSTR pszIn, int nOffset, gadget_object_base * gadget)
//      ==================================================================================
{
put_objects(data, pszIn, nOffset, (PSTR) gadget, TreeviewObjectList, ELEMENTS(TreeviewObjectList));

return sizeof(treeview_object);
}

        void treeview_g2t(PDATA data, FILE * hf, gadget_object_base * gadget, PSTR pszStringTable, PSTR pszMessageTable)
//      ================================================================================================================
{
get_objects(data, hf, pszStringTable, pszMessageTable, (PSTR) gadget, TreeviewObjectList, ELEMENTS(TreeviewObjectList), 2);
}
