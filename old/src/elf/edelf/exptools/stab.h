/*
 * Copyright 10/24/97 Sun Microsystems, Inc. All Rights Reserved
 *
 * stab.h 4.33 97/10/24
 */

/*
 */

/*
 * THIS FILE IS NOW THE MASTER stab.h FILE, A DUPLICATE OF stab.h MAY BE
 * MAINTAINED BY SPARCompilers
 *
 * KEEP IN SYNC 
 *
 */

/* FILE NOT I18N */

/*
 * This file gives definitions supplementing <a.out.h>
 * for debugging symbol table entries.
 * These entries must have one of the N_STAB bits on,
 * and are subject to relocation according to the masks in <a.out.h>
 * on 4.x (stabs not relocated on SVR4).
 */

#ifndef _STAB_H
#define _STAB_H

#if (!defined(_a_out_h) && !defined(_A_OUT_H)) || defined(HP_UX)
/* this file also contains fragments of a.out.h relevant to
 * support of stab processing within ELF files
 * (when a.out.h is not available)
 */
struct  stab {
        unsigned	n_strx;		/* index into file string table */
        unsigned char   n_type;		/* type flag (N_TEXT,..)  */
        char		n_other;	/* used by N_SLINE stab */
        short		n_desc;		/* see stabs documentation */
        unsigned	n_value;	/* value of symbol (or sdb offset) */ 
};
 
/* patchtypes for N_PATCH stab (n_desc field) */

#define P_BITFIELD 0x1
#define P_SPILL 0x2
#define P_SCOPY 0x3

/*
 * Simple values for n_type.
 */
#define N_UNDF  0x0             /* undefined */
#define N_ABS   0x2             /* absolute */
#define N_TEXT  0x4             /* text */
#define N_DATA  0x6             /* data */
#define N_BSS   0x8             /* bss */
#define N_COMM  0x12            /* common (internal to ld) */
#define N_FN    0x1f            /* file name symbol */
 
#define N_EXT   01              /* external bit, or'ed in */
#define N_TYPE  0x1e            /* mask for all the type bits */

#endif

/* 
 * maximum length of stab string before using continuation stab.
 *   (this is just a suggested limit), assembler has no limit.
 */

#define MAX_STAB_STR_LEN 250

/*
 * for symbolic debuggers:
 */
#define	N_GSYM	0x20		/* global symbol: name,,0,type,0 */
#define	N_FNAME	0x22		/* procedure name (f77 kludge): name,,0 */
#define	N_FUN	0x24		/* procedure: name,,0,linenumber,0 */
#define	N_STSYM	0x26		/* static symbol: name,,0,type,0 or section relative */
#define	N_LCSYM	0x28		/* .lcomm symbol: name,,0,type,0 or section relative */
#define N_MAIN  0x2a            /* name of main routine : name,,0,0,0 */
#define N_ROSYM 0x2c		/* ro_data: name,,0,type,0 or section relative */
#define	N_PC	0x30		/* global pascal symbol: name,,0,subtype,line */
#define N_CMDLINE 0x34		/* command line info */
#define N_OBJ	0x38		/* object file path or name */
#define N_OPT	0x3c		/* compiler options */
#define	N_RSYM	0x40		/* register sym: name,,0,type,register */
#define	N_SLINE	0x44		/* src line: 0,,0,linenumber,function relative */
#define	N_XLINE	0x45		/* h.o. src line: 0,,0,linenumber>>16,0 */
#define	N_ILDPAD 0x4c		/* now used as ild pad stab value=strtab delta
				   was designed for "function start.end" */
#define	N_SSYM	0x60		/* structure elt: name,,0,type,struct_offset */
#define N_ENDM	0x62		/* last stab emitted for module */
#define	N_SO	0x64		/* source file name: name,,0,0,0 */
#define N_ALIAS 0x6c		/* alias name: name,,0,0,0 */
#define	N_LSYM	0x80		/* local sym: name,,0,type,offset */
#define	N_BINCL 0x82		/* header file: name,,0,0,0 */
#define	N_SOL	0x84		/* #included file name: name,,0,0,0 */
#define	N_PSYM	0xa0		/* parameter: name,,0,type,offset */
#define N_EINCL 0xa2		/* end of include file */
#define	N_ENTRY	0xa4		/* alternate entry: name,linenumber,0 */
#define N_SINCL 0xa6            /* shared include file */
#define	N_LBRAC	0xc0		/* left bracket: 0,,0,nesting level,function relative */
#define	N_EXCL	0xc2		/* excluded include file */
#define	N_USING	0xc4		/* C++ using command */
#define N_ISYM  0xc6            /* position independent type symbol, internal */
#define N_ESYM  0xc8            /* position independent type symbol, external */
#define N_PATCH 0xd0            /* Instruction to be ignored by run-time checking. */
#define N_CONSTRUCT 0xd2        /* C++ constructor call. */
#define N_DESTRUCT 0xd4         /* C++ destructor call. */
#define	N_RBRAC	0xe0		/* right bracket: 0,,0,nesting level,function relative */
#define	N_BCOMM	0xe2		/* begin common: name,, */
#define	N_TCOMM	0xe3		/* begin task common: name,, */
#define	N_ECOMM	0xe4		/* end task_common/common: name,, */
#define N_XCOMM	0xe6		/* excluded common block */
#define	N_ECOML	0xe8		/* end common (local name): ,,address */
#define	N_WITH	0xea		/* pascal with statement: type,,0,0,offset */
#define	N_LENG	0xfe		/* second stab entry with length information */

/*
 * for code browser only
 */
#define N_BROWS	0x48		/* path to associated .cb file */

/*
 * Optional language designations for N_SO (n_desc field)
 */
#define N_SO_AS         1       /* Assembler    */
#define N_SO_C          2       /* C            */
#define N_SO_ANSI_C     3       /* ANSI C       */
#define N_SO_CC         4       /* C++          */
#define N_SO_FORTRAN    5       /* Fortran 77   */
#define N_SO_PASCAL     6       /* Pascal       */
#define N_SO_FORTRAN90  7       /* Fortran 90   */
#define N_SO_JAVA  	8       /* Java 	*/

/*
 * Floating point type values (encoded in "R" type specification string)
 */
#define NF_NONE		0	/* Undefined type 	*/
#define NF_SINGLE	1	/* IEEE 32 bit float	*/
#define NF_DOUBLE	2	/* IEEE 64 bit float	*/
#define NF_COMPLEX	3	/* Fortran complex 	*/
#define NF_COMPLEX16	4	/* Fortran double complex */
#define NF_COMPLEX32	5	/* Fortran complex*16	*/
#define NF_LDOUBLE 	6	/* Long double		*/
#define NF_INTERARITH   7       /* Fortran interval arithmatic  */
#define NF_DINTERARITH  8       /* Fortran double interval arithmatic */
#define NF_QINTERARITH  9       /* Fortran quad interval arithmatic     */

#endif


