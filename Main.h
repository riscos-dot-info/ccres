/* Main.h

   Copyright (c) 2007-2007 John Tytgat

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

#ifndef MAIN_HEADER_INCLUDED
#define MAIN_HEADER_INCLUDED

#include <oslib/saveas.h>
#include "ccres.h"

typedef struct {
  DATA *sessionP;

  bool fRunning;			// see check_quit()
  bool fUnsafeLoad;
  wimp_t task;
  toolbox_o idBaricon, idSaveAs;	// toolbox objects created from res file
  osspriteop_area *pSprites;		// ...and sprite area
  toolbox_block tb;			// ...easy-access toolbox block
  union {
    saveas_action_save_to_file_block sa;
    toolbox_action ta;			// ...and action data
    wimp_block wb;
  } poll;
} APPDATA;

#endif
