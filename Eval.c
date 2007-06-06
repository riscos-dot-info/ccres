/* Eval.c

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

#include "ccres.h"
#include "Error.h"
#include "Eval.h"
#include "Library.h"
#include "Utils.h"

// hi nybble = precedence, added to lo nybble to make unique id
enum {LBRACKET=0x11,ADD=0x21,SUB=0x22,MUL=0x31,DIV=0x32};
#define Precedence(op) ((op)&0xf0)

static bool Eval2(int *sop, int *sn, int *piop, int *pin)
{
	int lnum, op, rnum;
	int iop, in, res;

	iop = *piop;
	in = *pin;

	rnum = sn[--in];
	lnum = sn[--in];
	op = sop[--iop];
	if (op == MUL) {
		res = lnum *rnum;
	} else if (op ==  DIV) {
		if (rnum == 0) {
			return false;
		} else {
			res = lnum / rnum;
		}
	} else if (op ==  ADD) {
		res = lnum + rnum;
	} else if (op ==  SUB) {
		res = lnum - rnum;
	} else {
		return false;
	}
	sn[in++] = res;

	*piop = iop;
	*pin = in;
	return true;
}


int Eval(DATA *data, const char **ppstr)
{
	const char *pstr;
	int sop[16], sn[16];
	int iop, in, op;
	bool fOp;
	char ch;

	pstr = *ppstr;
	iop = in = 0;
	fOp = true;
	for (;;) {
		if ((ch = *pstr++) == ' ' || ch == '\t' ) {
			// do nothing
		} else if (ch == '(') {
			if (iop >= ELEMENTS(sop)) {
				goto Eval_SyntaxError;
			}
			sop[iop++] = LBRACKET;
			fOp = true;
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
			fOp = false;
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
			fOp = true;
		} else if (isdigit(ch) || (ch == '-' && fOp) || ch == '&') {
			if (in >= ELEMENTS(sn)) {
				goto Eval_SyntaxError;
			}
			pstr--;
			sn[in++] = my_atoi(&pstr);
			fOp = false;
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
		return sn[0];
	}

Eval_SyntaxError:

	report(data, *ppstr, "Expression syntax error");
	while (*pstr != '\n' && pstr >= data->pszIn && (pstr - data->pszIn) < data->cbIn)
	  ++pstr;
	*ppstr = pstr;
	return 0;
}
