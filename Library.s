; Library.s
; $Id: Library.s,v 1.1 2003/12/09 00:00:00 joty Exp $
;
; Copyright (c) 2003-2004 Dave Appleby / John Tytgat
;
; This file is part of CCres.
;
; CCres is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; CCres is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with CCres; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

; need to include all recursively called headers here
; until assembler (as) can do conditional includes:-
; temporarily comment out GET's from OSLib headers...

	GET OSLib:OsLib.Hdr.Types
	GET OSLib:OsLib.Hdr.OS
	GET OSLib:OsLib.Hdr.FileSwitch
	GET OSLib:OsLib.Hdr.OSArgs
	GET OSLib:OsLib.Hdr.OSFile
	GET OSLib:OsLib.Hdr.OSFind
	GET OSLib:OsLib.Hdr.OSFSControl
	GET OSLib:OsLib.Hdr.Territory
	GET OSLib:OsLib.Hdr.Wimp

	EXPORT	my_strcpy
	EXPORT	my_strcpy0d
	EXPORT	my_strncpy0d
	EXPORT	__stricmp
	EXPORT	__strnicmp
	EXPORT	__atoi
	EXPORT	my_osfile_delete
	EXPORT	my_osfile_exists
	EXPORT	my_osfile_filesize
	EXPORT	my_osfile_filetype
	EXPORT	my_osfile_load
	EXPORT	my_osfscontrol_count_objects

	IMPORT	achProgName

; to assemble for 32bit code with ObjAsm use -Apcs 3/32bit (sets {CONFIG}=32)
; these MACRO's sort out flags by assuming the caller saves flags in 32bit
; (restoring flags isn't easy in 32bit mode when condition flags are specified!)

; sregs is used to pass register list to function entry/exit macro's
; eg
; FNENTRY/FNEXIT                                    use lr for return
; sregs SETS lr    FNENTRY $sregs FNEXIT  $sregs    push lr
; sregs SETS r0,r1 FNENTRY $sregs FNEXIT $sregs     push r0,r1,lr

	GBLS	sregs

  MACRO
  FNENTRY $regs
  [ "$regs"<>""
    [ "$regs"="lr"
	STMFD	sp!, {lr}
    |
	STMFD	sp!, {$regs, lr}
    ]
  ]
  MEND

  MACRO
  FNEXIT $flags, $regs
  [ {CONFIG}=32
    [ "$regs"=""
	MOV$flags	pc, lr
    |
      [ "$regs"="lr"
	LDM$flags.FD	sp!, {pc}
      |
	LDM$flags.FD	sp!, {$regs, pc}
      ]
    ]
  |
    [ "$regs"=""
	MOV$flags.S	pc, lr
    |
      [ "$regs"="lr"
	LDM$flags.FD	sp!, {pc}^
      |
	LDM$flags.FD	sp!, {$regs, pc}^
      ]
    ]
  ]
  MEND


		GBLA	ReportOK
ReportOK	SETA	Wimp_ErrorBoxShortTitle + Wimp_ErrorBoxNoPrompt + Wimp_ErrorBoxOKIcon


	AREA |C$$Code|,CODE,READONLY

; int my_strcpy(PSTR to, PSTR from);
; same as string.h, but returns strlen
; entry		exit
; r0=to		r0=strlen (excluding term)
; r1=from

my_strcpy

	FNENTRY
	ADD	r3, r1, #1

my_strcpy_loop

	LDRB	r2, [r1], #1
	STRB	r2, [r0], #1
	CMPS	r2, #0x00
	BNE	my_strcpy_loop

	SUB	r0, r1, r3
	FNEXIT

; int my_strcpy0d(PSTR to, PSTR from);
; same as string.h, but handles 0x0d terminator and returns strlen
; entry		exit
; r0=to		r0=strlen (excluding term)
; r1=from

my_strcpy0d

	FNENTRY
	ADD	r3, r1, #1

my_strcpy0d_loop

	LDRB	r2, [r1], #1
	STRB	r2, [r0], #1
	CMPS	r2, #0x00
	CMPNES	r2, #0x0d
	BNE	my_strcpy0d_loop

	MOV	r2, #0x00		; change possible 0x0d term to 0x00
	STRB	r2, [r0, #-1]
	SUB	r0, r1, r3
	FNEXIT


; void my_strncpy0d(PSTR to, PSTR from, int max);
; same as string.h, but handles 0x0d terminator and void return
; needs an extra register to return a useful value
; entry		exit
; r0=to
; r1=from
; r2=max

my_strncpy0d

	FNENTRY

my_strncpy0d_loop

	LDRB	r3, [r1], #1
	STRB	r3, [r0], #1
	SUBS	r2, r2, #1
	CMPNES	r3, #0x00
	CMPNES	r3, #0x0d
	BNE	my_strncpy0d_loop

	MOV	r2, #0x00
	CMPS	r3, #' '
	STRLOB	r2, [r0, #-1]	; change possible 0x0d term to 0x00
	STRHSB	r2, [r0, #0]	; add terminator to fixed length string
	FNEXIT


; int __stricmp(PSTR p, PSTR q);
; on entry	on exit
; r0=string 1	r0=0 if equal
; r1=string 2

__stricmp

	FNENTRY
	MOV	r3, #Territory_IgnoreCase
	MOV	r2, r1
	MOV	r1, r0
	MOV	r0, #Territory_Current
	SWI	Territory_Collate
	FNEXIT


; int __strnicmp(PSTR p, PSTR q, int n);
; assumes n is always >= 1
; on entry	on exit
; r0=string 1	r0=0 if equal
; r1=string 2
; r2=n

__strnicmp
sregs	SETS	"r4"

	FNENTRY	$sregs
	MOV	r3, r0
	MOV	r0, #-1
	SWI	Territory_UpperCaseTable
	MOV	lr, r0

__strnicmp_loop

	LDRB	r0, [r1], #1
	LDRB	r4, [r3], #1
	LDRB	r0, [lr, r0]
	LDRB	r4, [lr, r4]
	SUBS	r0, r4, r0
	FNEXIT	NE, $sregs
	SUBS	r2, r2, #1
	CMPNES	r4, #0
	BNE	__strnicmp_loop
	FNEXIT	, $sregs


; unsigned int __atoi(PSTR * pszNumber);
; returns term pointer in *pszNumber
; OS_ReadUnsigned is far better than atoi library function!!
; on entry	on exit
; r0=&str	r0=number

__atoi
sregs	SETS	"lr"

	FNENTRY	$sregs
	MOV	lr, r0
	LDR	r1, [lr]
	LDRB	r3, [r1]		; check for -ve
	CMPS	r3, #'-'
	ADDEQ	r1, r1, #1

	MOV	r0, #10
	SWI	XOS_ReadUnsigned
	MOVVS	r0, #0
	MOVVC	r0, r2
	STRVC	r1, [lr]		; address of terminator

	CMPS	r3, #'-'		; adjust -ve
	RSBEQ	r0, r0, #0

	FNEXIT	,$sregs


; BOOL my_osfile_delete(PSTR pszFile);
; on entry	on exit
; r0=filepath	r0=True if deleted

my_osfile_delete
sregs	SETS	"r4-r5"

	FNENTRY	$sregs
	MOV	r1, r0
	MOV	r0, #6
	SWI	XOS_File
	MOVVS	r0, #False
	MOVVC	r0, #True
	FNEXIT	, $sregs


; fileswitch_object_type my_osfile_exists(PSTR pszFile);
; on entry	on exit
; r0=filepath	r0=True if exists

my_osfile_exists
sregs	SETS	"r4-r5"

	FNENTRY	$sregs
	MOV	r1, r0
	MOV	r0, #17
	SWI	XOS_File
	FNEXIT	, $sregs


; int my_osfile_filesize(PSTR pszFile);
; on entry	on exit
; r0=file	r0=filesize

my_osfile_filesize
sregs	SETS	"r4-r5"

	FNENTRY	$sregs
	MOV	r1, r0
	MOV	r0, #17
	SWI	XOS_File
	MOVVS	r4, #0
	CMPS	r0, #OSFile_IsFile
	MOVNE	r0, #0
	MOVEQ	r0, r4
	FNEXIT	, $sregs


; bits my_osfile_filetype(PSTR pszFile);
; on entry	on exit
; r0=file	r0=type

my_osfile_filetype
sregs	SETS	"r4-r6"

	FNENTRY	$sregs
	MOV	r1, r0
	MOV	r0, #23
	SWI	XOS_File
	CMPS	r0, #0
	MVNEQ	r0, #0
	MOVNE	r0, r6
	FNEXIT	, $sregs


; int my_osfile_load(PSTR pszFile, PSTR pszBuff, int cbBuff);
; on entry	on exit
; r0=file	r0=bytes read
; r1=buff
; r2=cbBuff

my_osfile_load
sregs	SETS	"r4-r5"
pszBuff	RN	ip
cbBuff	RN	lr

	FNENTRY	$sregs
	MOV	pszBuff, r1
	MOV	cbBuff, r2

	MOV	r1, r0
	MOV	r0, #17
	SWI	XOS_File
	FNEXIT	VS, $sregs

	CMPS	r0, #OSFile_IsFile
	FNEXIT	NE, $sregs

	CMPS	r4, cbBuff
	ADRGT	r0, osfile_load_smallbuff
	BGT	osfile_load_error

	MOV	r0, #16
; r1 is file path
	MOV	r2, pszBuff
	MOV	r3, #0
	SWI	XOS_File
	MOVVC	r0, r4
	FNEXIT	VC, $sregs

osfile_load_error

	MOV	r1, #ReportOK
	LDR	r2, pachProgName
	SWI	Wimp_ReportError
	FNEXIT	, $sregs

pachProgName		DCD	achProgName
osfile_load_smallbuff	DCD	0
			DCB	"osfile_load buffer too small",0
			ALIGN

; int osfscontrol_count_objects(PSTR pszDir);
; on entry	on exit
; r0=dir	r0=Object count

my_osfscontrol_count_objects

	FNENTRY
	MOV	r1, r0
	MOV	r0, #28
	MOV	r2, #0
	MOV	r3, #0
	SWI	XOS_FSControl
	MOVVS	r0, #-1
	MOVVC	r0, r3
	FNEXIT


	END
