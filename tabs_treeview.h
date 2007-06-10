/* Tabs / TreeView

   Copyright (c) 2004-2007 John Tytgat

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

#ifndef TABS_TREEVIEW_HEADER_INCLUDED
#define TABS_TREEVIEW_HEADER_INCLUDED

/* Based on Tabs.h and TreeView.h headers written by Rik Griffin
   dated Nov 2004, release 0.08. */

#include <stdint.h>
#include <oslib/gadget.h>

/* Tabs */

#define class_TABS				((toolbox_class)0x402c)

#define tabs_ALLOW_SCROLL_ARROWS		((gadget_flags)0x01)

typedef struct tabs_object tabs_object;
struct tabs_object
  {
    gadget_OBJECT_MEMBERS
    wimp_colour foreground;
    uint8_t unused1[3];
    wimp_colour background;
    uint8_t unused2[3];
    toolbox_msg_reference text;
  };

/* TreeView */

#define class_TREE_VIEW				((toolbox_class)0x4028)

#define treeview_ALLOW_VSCROLL			((gadget_flags)(1u<<0))
#define treeview_ALLOW_HSCROLL			((gadget_flags)(1u<<1))
#define treeview_AUTO_UPDATE			((gadget_flags)(1u<<2))
#define treeview_SHOW_CURRENT_NODE		((gadget_flags)(1u<<3))
#define treeview_PLOT_LINES			((gadget_flags)(1u<<4))

#define treeview_ALLOW_SELECTION		((gadget_flags)(1u<<5))
#define treeview_NOTIFY_SELECTION		((gadget_flags)(1u<<6))
#define treeview_SELECT_CHILDREN		((gadget_flags)(1u<<7))

#define treeview_ALLOW_EXPAND			((gadget_flags)(1u<<8))
#define treeview_NOTIFY_EXPANSION		((gadget_flags)(1u<<9))
#define treeview_EXPAND_BY_DEFAULT		((gadget_flags)(1u<<10))

#define treeview_ALLOW_RENAME			((gadget_flags)(1u<<11))
#define treeview_NOTIFY_RENAME			((gadget_flags)(1u<<12))

#define treeview_ALLOW_DRAGS			((gadget_flags)(1u<<13))
#define treeview_NOTIFY_DRAGEND			((gadget_flags)(1u<<14))

#define treeview_ALL_EVENTS			((gadget_flags)(1u<<15))

typedef struct treeview_object treeview_object;
struct treeview_object
  {
    gadget_OBJECT_MEMBERS
    wimp_colour foreground;
    uint8_t unused1[3];
    wimp_colour background;
    uint8_t unused2[3];
  };

#endif
