/* Eval.c
   $Id: $

   Copyright (c) 2003 Dave Appleby / John Tytgat

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

#include <ctype.h>

// hi nybble = precedence, added to lo nybble to make unique id
enum {LBRACKET=0x11,ADD=0x21,SUB=0x22,MUL=0x31,DIV=0x32};
#define Precedence(op) ((op)&0xf0)

static BOOL Eval2(PINT sop, PINT sn, PINT piop, PINT pin)
{
	int lnum, op, rnum;
	int iop, in, res;

	iop = *piop;
	in = *pin;

	rnum = sn[--in];
	lnum = sn[--in];
	op = sop[--iop];
	if (op == MUL) {
		res = lnum * rnum;
	} else if (op ==  DIV) {
		if (rnum == 0) {
			return(FALSE);
		} else {
			res = lnum / rnum;
		}
	} else if (op ==  ADD) {
		res = lnum + rnum;
	} else if (op ==  SUB) {
		res = lnum - rnum;
	} else {
		return(FALSE);
	}
	sn[in++] = res;

	*piop = iop;
	*pin = in;
	return(TRUE);
}


int Eval(PDATA data, PSTR * ppstr)
{
	PSTR pstr;
	int sop[16], sn[16];
	int iop, in, op;
	BOOL fOp;
	char ch;

	pstr = *ppstr;
	iop = in = 0;
	fOp = TRUE;
	for (;;) {
		if ((ch = *pstr++) == ' ' || ch == '\t' ) {
			// do nothing
		} else if (ch == '(') {
			if (iop >= ELEMENTS(sop)) {
				goto Eval_SyntaxError;
			}
			sop[iop++] = LBRACKET;
			fOp = TRUE;
		} else if (ch == ')') {
			if (iop == 0) {
				goto Eval_SyntaxError;
			} else {
				while (sop[iop - 1] != LBRACKET) {
					if (!Eval2(sop, sn, &iop, &in)) {
						goto Eval_SyntaxError;
					}
				}
				iop--;		// pop LBRACKET
			}
			fOp = FALSE;
		} else if ((ch == '-' && !fOp) || ch == '+' || ch == '/' || ch == '*') {
			op = (ch == '+') ? ADD :
				 (ch == '-') ? SUB :
				 (ch == '*') ? MUL : DIV;
			while (iop > 0 && Precedence(sop[iop - 1]) >= Precedence(op)) {
				if (!Eval2(sop, sn, &iop, &in)) {
					goto Eval_SyntaxError;
				}
			}
			if (iop >= ELEMENTS(sop)) {
				goto Eval_SyntaxError;
			}
			sop[iop++] = op;
			fOp = TRUE;
		} else if (isdigit(ch) || (ch == '-' && fOp) || ch == '&') {
			if (in >= ELEMENTS(sn)) {
				goto Eval_SyntaxError;
			}
			pstr--;
//LOG(("__atoi(%s)", pstr));
			sn[in++] = __atoi(&pstr);
//LOG(("val=%d New pstr = %s", sn[in - 1], pstr));
			fOp = FALSE;
		} else {
			pstr--;
			break;
		}
	}
	while (iop > 0) {
		if (!Eval2(sop, sn, &iop, &in)) {
			goto Eval_SyntaxError;
		}
	}
	if (in == 1) {
		*ppstr = pstr;
		return(sn[0]);
	}

Eval_SyntaxError:

LOG(("Expression syntax error"));
	report(data, *ppstr, "Expression syntax error");
	*ppstr = pstr;
	return(0);
}
