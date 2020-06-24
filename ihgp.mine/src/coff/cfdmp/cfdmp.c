/*
 * this program dumps the contents of COFF files. the options
 * are as follows.
 *
 * level                    option    
 * ------------------------------------------------------------
 * section                  -c [1,2,... | section_name,... | all]
 *     raw data             -d
 *     relocation           -r 
 *     line #               -l
 *     symbols              -s
 *     strings              -t
 * 
 * section                  -c [1,2,... | section_name,... | all]
 *     filename             -F [filename1,... | all]
 *         function         -f [func1,... | all]
 *             raw data     -d
 *             relocation   -r
 *             line #       -l
 *             symbols      -s
 *             strings      -t
 * 
 * file                     -h
 * optional                 -o
 * section                  -C
 * raw data                 -d
 * relocation               -r
 * line #                   -l
 * symbols                  -s
 * strings                  -t
 * global                   -g [name1,... | all]
 * unresolved               -u
 * extern                   -e
 */

/* COFF headers */
#include "filehdr.h"
#include "aouthdr.h"
#include "scnhdr.h"
#include "reloc.h"
#include "linenum.h"
#include "syms.h"

/* other UNIX headers */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <values.h>
#include <sys/types.h>
#include <sys/stat.h>

/* externs for getopt() */
extern char *optarg;
extern int optind, opterr;

/* macros to access data in auxiliary records */
#define AUXTAGNDX(paux) paux->x_sym.x_tagndx
#define AUXLNNO(paux) paux->x_sym.x_misc.x_lnsz.x_lnno
#define AUXSIZE(paux) paux->x_sym.x_misc.x_lnsz.x_size
#define AUXFSIZE(paux) paux->x_sym.x_misc.x_fsize
#define AUXLNNOPTR(paux) paux->x_sym.x_fcnary.x_fcn.x_lnnoptr
#define AUXENDNDX(paux) paux->x_sym.x_fcnary.x_fcn.x_endndx
#define AUXDIMEN(paux, dimen) paux->x_sym.x_fcnary.x_ary.x_dimen[dimen]
#define AUXTVNDX(paux) paux->x_sym.x_tvndx
#define AUXFNAME(paux) paux->x_file.x_fname
#define AUXSCNLEN(paux) paux->x_scn.x_scnlen
#define AUXNRELOC(paux) paux->x_scn.x_nreloc
#define AUXNLINNO(paux) paux->x_scn.x_nlinno
#define AUXTVFIL(paux) paux->x_tv.x_tvfill
#define AUXTVLEN(paux) paux->x_tv.x_tvlen
#define AUXTVRAN(paux, dimen) paux->x_tv.x_tvran[dimen]

/* errors to ignore */
unsigned long Ignore;
#define IGNNULL 00
#define IGNMAGIC 01
#define IGNLNNO 02
#define IGNRELFLG 04
#define IGNSCLASS 010

/* global option flag */
unsigned long optflag;
#define CFNULL 00
#define CFFILEHDR 01
#define CFOPTHDR 02
#define CFASECHDR 04
#define CFRAWDATA 010
#define CFRELOC 020
#define CFLINE 040
#define CFSYMBOL 0100
#define CFSTRING 0200
#define CFFILENAME 0400
#define CFFUNCTION 01000
#define CFGLOBAL 02000
#define CFUNRESOLVED 04000
#define CFEXTERN 010000
#define CFNSECHDR 020000
#define CFSSECHDR 040000
#define CFSECHDR2 0100000

/* relocation types and names */
#define RFLAGNULL 0xffff
struct RFLAGS {
    unsigned short flag;
    char *rflagname;
} ;
struct RFLAGS rflaglist[] = {
    { R_ABS, "R_ABS" },
    { R_OFF8, "R_OFF8" },
    { R_OFF16, "R_OFF16" },
    { R_SEG12, "R_SEG12" },
    { R_AUX, "R_AUX" },
    { R_DIR16, "R_DIR16" },
    { R_REL16, "R_REL16" },
    { R_IND16, "R_IND16" },
    { R_DIR24, "R_DIR24" },
    { R_REL24, "R_REL24" },
    { R_OPT16, "R_OPT16" },
    { R_IND24, "R_IND24" },
    { R_IND32, "R_IND32" },
#if 0
    { R_DIR10, "R_DIR10" },
    { R_REL10, "R_REL10" },
    { R_REL32, "R_REL32" },
#endif
    { R_DIR32, "R_DIR32" },
    { R_DIR32S, "R_DIR32S" },
    { R_RELBYTE, "R_RELBYTE" },
    { R_RELWORD, "R_RELWORD" },
    { R_RELLONG, "R_RELLONG" },
    { R_PCRBYTE, "R_PCRBYTE" },
    { R_PCRWORD, "R_PCRWORD" },
    { R_PCRLONG, "R_PCRLONG" },
    { RFLAGNULL, "RFLAGNULL" }
} ;

/* flag types and names */
#define NULLFLAG 0xffff
struct FLAGS {
    unsigned short flag;
    char *flagname;
} ;
struct FLAGS flaglist[] = {
    { F_RELFLG, "F_RELFLG" }, /* 00001 */
    { F_EXEC, "F_EXEC" }, /* 00002 */
    { F_LNNO, "F_LNNO" }, /* 00004 */
    { F_LSYMS, "F_LSYMS" }, /* 00010 */
    { F_MINMAL, "F_MINMAL" }, /* 00020 */
    { F_UPDATE, "F_UPDATE" }, /* 00040 */
    { F_SWABD, "F_SWABD" }, /* 00100 */
    { F_AR16WR, "F_AR16WR" }, /* 00200 */
    { F_AR32WR, "F_AR32WR" }, /* 00400 */
    { F_AR32W, "F_AR32W" }, /* 01000 */
    { F_PATCH, "F_PATCH" }, /* 02000 */
    { NULLFLAG, "NULLFLAG" } /* 00000 */
};

/* section flags types and names */
#define SFLAGNULL 0xffffffff
struct SFLAGS {
    unsigned long flag;
    char *sflagname;
} ;
struct SFLAGS sflaglist[] = {
    { STYP_REG, "STYP_REG" },
    { STYP_DSECT, "STYP_DSECT" },
    { STYP_NOLOAD, "STYP_NOLOAD" },
    { STYP_GROUP, "STYP_GROUP" },
    { STYP_PAD, "STYP_PAD" },
    { STYP_COPY, "STYP_COPY" },
    { STYP_TEXT, "STYP_TEXT" },
    { STYP_DATA, "STYP_DATA" },
    { STYP_BSS, "STYP_BSS" },
    { SFLAGNULL, "SFLAGNULL" },
};
/* machine types and character names */
#define NULLMAGIC 0xffff
struct MACHINE {
   unsigned short machine;
   char *machname;
} ;

struct MACHINE machlist[] = {
 { B16MAGIC, "B16MAGIC" }, /* 0502 */
 { BTVMAGIC, "BTVMAGIC" }, /* 0503 */
 { X86MAGIC, "X86MAGIC" }, /* 0510 */
 { XTVMAGIC, "XTVMAGIC" }, /* 0511 */
 { N3BMAGIC, "N3BMAGIC" }, /* 0550 */
 { NTVMAGIC, "NTVMAGIC" }, /* 0551 */
 { XLMAGIC, "XLMAGIC " }, /* 0540 */
 { FBOMAGIC, "FBOMAGIC" }, /* 0560 */
 { RBOMAGIC, "RBOMAGIC" }, /* 0562 */
 { MTVMAGIC, "MTVMAGIC" }, /* 0561 */
 { VAXWRMAGIC, "VAXWRMAGIC" }, /* 0570 */
 { VAXROMAGIC, "VAXROMAGIC" }, /* 0575 */
 { MC68MAGIC, "MC68MAGIC" }, /* 0520 */
 { MC68TVMAGIC, "MC68TVMAGIC" }, /* 0521 */
 { M68MAGIC, "M68MAGIC" }, /* 0210 */
 { M68TVMAGIC, "M68TVMAGIC" }, /* 0211 */
 { U370ROMAGIC, "U370ROMAGIC" }, /* 0530 */
 { U370WRMAGIC, "U370WRMAGIC" }, /* 0535 */
 { NULLMAGIC, "NULLMAGIC" }, /* 00, always last entry */
};

/* table of functions for different storage classes */
extern long sc_efcn();
extern long sc_null();
extern long sc_auto();
extern long sc_ext();
extern long sc_stat();
extern long sc_reg();
extern long sc_extdef();
extern long sc_label();
extern long sc_ulabel();
extern long sc_mos();
extern long sc_arg();
extern long sc_strtag();
extern long sc_mou();
extern long sc_untag();
extern long sc_tpdef();
extern long sc_ustatic();
extern long sc_entag();
extern long sc_moe();
extern long sc_regparm();
extern long sc_field();
extern long sc_block();
extern long sc_fcn();
extern long sc_eos();
extern long sc_file();
extern long sc_line();
extern long sc_alias();
extern long sc_hidden();
#ifndef VAX
extern long sc_endinit();
#endif

#define NULLSCFCN ((long (*)()) 0L)
enum ALLOWED { NOCARE, YES, NO } ;
struct SCLASSFCN {
    char sclass;
    char *sc_name;
    enum ALLOWED scn_debug;
    enum ALLOWED scn_abs;
    enum ALLOWED scn_undef;
    enum ALLOWED scn_scnum;
    long (*fn)();
};
struct SCLASSFCN scfnlist[] = {
    { C_EFCN, "C_EFCN", NO, NO, NO, NO, sc_efcn },
    { C_NULL, "C_NULL", NO, NO, NO, NO, sc_null },
    { C_AUTO, "C_AUTO", NO, YES, NO, NO, sc_auto },
    { C_EXT, "C_EXT", NO, YES, YES, YES, sc_ext },
    { C_STAT, "C_STAT", NO, NO, NO, YES, sc_stat },
    { C_REG, "C_REG", NO, YES, NO, NO, sc_reg },
    { C_EXTDEF, "C_EXTDEF", NO, NO, NO, NO, sc_extdef },
    { C_LABEL, "C_LABEL", NO, NO, YES, YES, sc_label },
    { C_ULABEL, "C_ULABEL", NO, NO, NO, NO, sc_ulabel },
    { C_MOS, "C_MOS", NO, YES, NO, NO, sc_mos },
    { C_ARG, "C_ARG", NO, YES, NO, NO, sc_arg },
    { C_STRTAG, "C_STRTAG", YES, NO, NO, NO, sc_strtag },
    { C_MOU, "C_MOU", NO, YES, NO, YES, sc_mou },
    { C_UNTAG, "C_UNTAG", YES, NO, NO, NO, sc_untag },
    { C_TPDEF, "C_TPDEF", YES, NO, NO, NO, sc_tpdef },
    { C_USTATIC, "C_USTATIC", NO, NO, NO, NO, sc_ustatic },
    { C_ENTAG, "C_ENTAG", YES, NO, NO, NO, sc_entag },
    { C_MOE, "C_MOE", NO, YES, NO, NO, sc_moe },
    { C_REGPARM, "C_REGPARM", NO, YES, NO, NO, sc_regparm },
    { C_FIELD, "C_FIELD", NO, YES, NO, NO, sc_field },
    { C_BLOCK, "C_BLOCK", NO, NO, NO, YES, sc_block },
    { C_FCN, "C_FCN", NO, NO, NO, YES, sc_fcn },
    { C_EOS, "C_EOS", NO, YES, NO, NO, sc_eos },
    { C_FILE, "C_FILE", YES, NO, NO, NO, sc_file },
    { C_LINE, "C_LINE", NO, NO, NO, NO, sc_line },
    { C_ALIAS, "C_ALIAS", YES, NO, NO, NO, sc_alias },
    { C_HIDDEN, "C_HIDDEN", NO, NO, NO, YES, sc_hidden },
#if 0
#ifndef VAX
    { C_ENDINIT, "C_ENDINIT", NO, NO, NO, NO, sc_endinit },
#endif
#endif
    { C_NULL, "C_NULL", NOCARE, NOCARE, NOCARE, NOCARE, NULLSCFCN },
};

/* fundamental/derived bits */
enum TYPEBITS { NOBITS, TYP, D1, D2, D3, D4, D5, D6 } ;

/* symbol type table */
char *symdtyptbl[] = {
    "no derived type", "*", "()", "[]"
} ;
char *symtyptbl[] = {
    "no type", "argument", "char", "short",
    "int", "long", "float", "double",
    "struct", "union", "enum", "member of enum",
    "uchar", "ushort", "uint", "ulong"
} ;
/* global parameters */
unsigned short machine;		/* machine type, i.e., magic */
unsigned short numsec;		/* number of sections */
unsigned short opthdrsz;	/* optional header size */
unsigned short flags;		/* flags describing the file */
long coffdate;			/* date of COFF file */
long symstart;		/* file offset to start of symbol table */
long numsyms;		/* number of symbols in table */
long secstart;		/* start of section headers */
char *machname;		/* pointer machine type string */
char **clist;		/* list section numbers or names */
char **Flist;		/* list files names */
char **flist;		/* list function names */
char **glist;		/* list global names */

/* local definitions */
#define NULLCHR ((char) 0L)
#define NULLSTR ""
#define NULLPTR 0L
#define DELIMITER ","
#define SPACE " "
#define CFNUMBERS "0123456789"
#define CFNOTOK 0L
#define CFOK 1L
#define CFBADMAGIC 2L
#define CFUNKSCL 3L
#define COMMA ((char) 054)
#define FHDROFFSET 0L
#define ONEITEM 1
#define CFALL "all"

/* usage messages */
char *usagemsgs[] = {
"usage: cfdmp [-hodrlstueC] ",
"       [-c [1,2,...|all|section_name,...]]",
"       [[-F[filename1,...|all]]|[-f[function1,...|all]]] COFF_file ...",
" "
"level                    option",
"------------------------------------------------------------",
"section                  -c [1,2,... | section_name,... | all]",
"    raw data             -d",
"    relocation           -r ",
"    line #               -l",
"    symbols              -s",
"    strings              -t",
" ",
"section                  -c [1,2,... | section_name,... | all]",
"    filename             -F [filename1,... | all]",
"        function         -f [func1,... | all]",
"            raw data     -d",
"            relocation   -r",
"            line #       -l",
"            symbols      -s",
"            strings      -t",
" ",
"file                     -h",
"optional                 -o",
"section                  -C",
"raw data                 -d",
"relocation               -r",
"line #                   -l",
"symbols                  -s",
"strings                  -t",
"global                   -g [name1,... | all]",
"unresolved               -u",
"extern                   -e",
NULL
};

/* function to get bits from an unsigned long */
unsigned long getbits(highbit, lowbit, value)
unsigned long highbit;
unsigned long lowbit;
unsigned long value;
{
    unsigned long numbits;

    numbits = highbit - lowbit + 1;
    return ((value >> (highbit + 1 - numbits)) & ~(~0 << numbits));
}

/* function to get bit form a given bit flag in type */
unsigned long getdtbits(symhdr, dtbits)
struct syment *symhdr;
enum TYPEBITS dtbits;
{
    unsigned long typ, value;

    /* get type */
    value = symhdr->n_type;
    /* switch according to which bits are requested */
    switch (dtbits)
    {
    case TYP:
	typ = getbits(3L, 0L, value);
	break;
    case D1:
	typ = getbits(5L, 4L, value);
	break;
    case D2:
	typ = getbits(7L, 6L, value);
	break;
    case D3:
	typ = getbits(9L, 8L, value);
	break;
    case D4:
	typ = getbits(11L, 10L, value);
	break;
    case D5:
	typ = getbits(13L, 12L, value);
	break;
    case D6:
	typ = getbits(15L, 14L, value);
	break;
    case NOBITS:
    default:
	return(MAXLONG);
    }
    /* finished */
    return(typ);
}

/* function to get nvalue from symbol table given an index */
long symndx2nv(infd, symndx)
FILE *infd;
long symndx;
{
    long offset;
    struct syment symhdr;
#if 0
    int fseek(), fread();
#endif

    /* if an error occurs, return the maximum long possible */

    /* seek to location of symbol entry */
    offset = symstart + symndx*SYMESZ;
    if (fseek(infd, offset, 0) != 0) return(MAXLONG);
    /* read in symbol table entry */
    if (fread((char *) &symhdr, SYMESZ, ONEITEM, infd) != ONEITEM) return(MAXLONG);
    /* return nvalue */
    return(symhdr.n_value);
}

/* function to get symbol type */
char *getsymtype(symhdr, string)
struct syment *symhdr;
char *string;
{
    unsigned long typ, d, value, hbit, lbit;

    /* blank the string */
    *string = NULLCHR;
    /* get fundamental type */
    value = symhdr->n_type;
    typ = getbits(3L, 0L, value);
    strcpy(string, symtyptbl[(int)typ]);
    if (typ == T_NULL) return(string);
    /* get derived types */
    strcat(string, SPACE);
    for (hbit = 15L; hbit > 3L; hbit -= 2L)
    {
	lbit = hbit - 1;
	d = getbits(hbit, lbit, value);
	if (d != DT_NON) strcat(string, symdtyptbl[(short)d]);
    }
    /* finished */
    return(string);
}

/* function to get section type and name */
char *getsecnm(infd, symhdr, string)
FILE *infd;
struct syment *symhdr;
char *string;
{
    struct scnhdr shdr;

    /* check value of section number */
    if (symhdr->n_scnum == N_UNDEF)
    {
	/* undefined section */
	strcpy(string, "N_UNDEF");
    }
    else if (symhdr->n_scnum == N_ABS)
    {
	/* absolute value */
	strcpy(string, "N_ABS");
    }
    else if (symhdr->n_scnum == N_DEBUG)
    {
	/* debug info */
	strcpy(string, "N_DEBUG");
    }
    else if (symhdr->n_scnum == N_TV)
    {
	/* preload transfer vector */
	strcpy(string, "N_TV");
    }
    else if (symhdr->n_scnum == P_TV)
    {
	/* postload transfer vector */
	strcpy(string, "P_TV");
    }
    else if (symhdr->n_scnum > N_UNDEF)
    {
	/* section number was given, seek to header */
	if (fseek(infd, (secstart + (symhdr->n_scnum - 1)*SCNHSZ), 0) != 0)
	{
	    return((char *) NULLPTR);
	}
	/* read section header */
	if (fread((char *) &shdr, SCNHSZ, ONEITEM, infd) != ONEITEM)
	{
	    return((char *) NULLPTR);
	}
	/* section name */
	strcpy(string, "N_SCNUM, ");
	strncat(string, shdr.s_name, 8);
	string[9 + 8] = NULLCHR;
    }
    else
    {
	/* wrong section number */
	return((char *) NULLPTR);
    }
    /* return with section type and name, if any */
    return(string);
}

/* function to get symbol name */
char *getsymnm(infd, symhdr, string)
FILE *infd;
struct syment *symhdr;
char *string;
{
    char *pstr;
    long offset;
#if 0
    int fseek(), fread();
#endif

    /* check if name is in the string table */
    if (symhdr->_n._n_n._n_zeroes == 0L)
    {
	/* read string table for symbol name */
	offset = symstart + numsyms*SYMESZ + symhdr->_n._n_n._n_offset;
	if (fseek(infd, offset, 0) != 0) return((char *) NULLPTR);
	/* since the string is NULL terminated */
	for (pstr = string; ((*pstr = getc(infd)) != NULLCHR); pstr++)
	{
	    /* do nothing */
	}
	*pstr = NULLCHR;
	/* done, return symbol name */
	return(string);
    }
    /* name is in symhdr structure */
    strncpy(string, symhdr->_n._n_name, SYMNMLEN);
    string[SYMNMLEN] = NULLCHR;
    /* return name of string */
    return(string);
}

/* function to get a symbol name given an index */
char *symndx2nm(infd, symndx, string)
FILE *infd;
long symndx;
char *string;
{
    char *pstr;
    long offset;
    struct syment symhdr;
#if 0
    int fseek(), fread();
#endif

    /* seek to location of symbol entry */
    offset = symstart + symndx*SYMESZ;
    if (fseek(infd, offset, 0) != 0) return((char *) NULLPTR);
    /* read in symbol table entry */
    if (fread((char *) &symhdr, 
	      SYMESZ, ONEITEM, infd) != ONEITEM) return((char *) NULLPTR);
    /* check if name is in the string table */
    if (symhdr._n._n_n._n_zeroes == 0L)
    {
	/* read string table for symbol name */
	offset = symstart + numsyms*SYMESZ + symhdr._n._n_n._n_offset;
	if (fseek(infd, offset, 0) != 0) return((char *) NULLPTR);
	/* since the string is NULL terminated */
	for (pstr = string; ((*pstr = getc(infd)) != NULLCHR); pstr++)
	{
	    /* do nothing */
	}
	*pstr = NULLCHR;
	/* done, return symbol name */
	return(string);
    }
    /* name is in symhdr structure */
    strncpy(string, symhdr._n._n_name, SYMNMLEN);
    string[SYMNMLEN] = NULLCHR;
    /* return name of string */
    return(string);
}

/* dump raw data at given location */
long hexdump(infd, fileoffset, nbytes, startbyte)
FILE *infd;
long fileoffset, nbytes, startbyte;
{
    long offset, nbuflong, nremlong;
    long nr, nb, remainder, nreads, *pdata;
#if 0
    int fseek(), fread();
    void free();
    char *malloc();
#endif

    /* check arguments */
    if ((nbytes < 0) || (fileoffset < 0)) return(CFNOTOK);
    if (nbytes == 0) return(CFOK);

    /* determine how many longs in BUFSIZ buffer */
    nbuflong = BUFSIZ/sizeof(long);

    /* allocate buffer */
    pdata = (long *) malloc((unsigned) nbuflong*sizeof(long));
    if (pdata == (long *) NULLPTR) return(CFNOTOK);

    /* seek to raw data section */
    if (fseek(infd, fileoffset, 0) != 0) 
    {
	/* free memory */
	free((char *)pdata);
	return(CFNOTOK);
    }

    /* determine how many reads are needed to read in raw data */
    nreads = nbytes/(nbuflong*sizeof(long));
    remainder = nbytes % (nbuflong*sizeof(long));

    /* read in raw data and print */
    for (offset = 0, nr = 1; nr <= nreads; nr++)
    {
	/* read in a buffer */
	if (fread((char *) pdata, nbuflong*sizeof(long), ONEITEM, infd) != ONEITEM)
	{
	    /* free memory */
	    free((char *)pdata);
	    return(CFNOTOK);
	}

	/* print buffer */
	for (nb = 0; nb < nbuflong; nb++, 
	     startbyte += sizeof(long), offset += sizeof(long))
	{
	    if ((offset % 32) == 0) fprintf(stdout, "%l08x: ", startbyte);
	    fprintf(stdout, "%l08x ", pdata[nb]);
	    if ((offset % 32) == 28) fprintf(stdout, "\n");
	}
    }

    /* do final read of data */
    if (remainder > 0)
    {
	/* read in final buffer */
	if (fread((char *) pdata, remainder, ONEITEM, infd) != ONEITEM)
	{
	    /* free memory */
	    free((char *)pdata);
	    return(CFNOTOK);
	}

	/* print buffer */
	nremlong = remainder/sizeof(long);
	if (remainder > nremlong*sizeof(long)) nremlong++;
	for (nb = 0; nb < nremlong; 
	     nb++, startbyte += sizeof(long), offset += sizeof(long))
	{
	    if ((offset % 32) == 0) fprintf(stdout, "%l08x: ", startbyte);
	    fprintf(stdout, "%l08x ", pdata[nb]);
	    if ((offset % 32) == 28) fprintf(stdout, "\n");
	}
    }
    if ((offset % 32) != 0) fprintf(stdout, "\n");

    /* free memory */
    free((char *) pdata);

    /* return */
    return(CFOK);
}

/* function for printing out usage */
void USAGE(errmsg)
char *errmsg;
{
    int iu;
    if (*errmsg != NULLCHR) fprintf(stderr, "%s\n", errmsg);
    for (iu = 0; usagemsgs[iu] != NULL; iu++)
    {
	fprintf(stderr, "%s\n", usagemsgs[iu]);
    }
    return;
}

/* get file header info and store info in globals */
long getfhdr(infd)
FILE *infd;
{
    long offset;
    struct MACHINE *pmach;
    struct filehdr fhdr;
#if 0
    int fseek(), fread();
#endif

    /* seek to file hdr */
    offset = FHDROFFSET;
    if (fseek(infd, offset, 0) != 0) return(CFNOTOK);
    /* read in file hdr */
    if (fread((char *) &fhdr, FILHSZ, ONEITEM, infd) != ONEITEM) return(CFNOTOK);
    /* check magic */
    for (machine = NULLMAGIC, pmach = machlist; pmach->machine != NULLMAGIC; pmach++)
    {
	/* search table for a match */
	if (pmach->machine == fhdr.f_magic)
	{
	    /* found a match */
	    machine = pmach->machine;
	    machname = pmach->machname;
	    break;
	}
    }
    /* check if magic was found */
    if ((machine == NULLMAGIC) && ((Ignore & IGNMAGIC) != IGNMAGIC)) return(CFBADMAGIC);
    /* number of sections in file */
    numsec = fhdr.f_nscns;
    /* size of optional header */
    opthdrsz = fhdr.f_opthdr;
    /* file hdr flags */
    flags = fhdr.f_flags;
    /* file pointer to start of symbol table */
    symstart = fhdr.f_symptr;
    /* number of symbol entries in table */
    numsyms = fhdr.f_nsyms;
    /* file pointer to start of section headers */
    secstart = FILHSZ + opthdrsz;
    /* date for COFF file */
    coffdate = fhdr.f_timdat;
    /* return with info */
    return(CFOK);
}

/* print file hdr info */
long dumpfhdr(fname)
{
#if 0
    char *ptmstr, *asctime();
    struct tm *ptmbuf, *localtime();
#else
    char *ptmstr;
    struct tm *ptmbuf;
#endif
    struct FLAGS *pflag;

    /* start on new line, and print date */
    fprintf(stdout, "\nfile header info\n");
    ptmbuf = localtime(&coffdate);
    ptmstr = asctime(ptmbuf);
    ptmstr[strlen(ptmstr) - 1] = NULLCHR;
    fprintf(stdout, "date: <0x%lx,%s>\n", coffdate, ptmstr);
    /* print file name */
    fprintf(stdout, "file: <%s> ", fname);
    /* print magic string */
    fprintf(stdout, "magic: <%s,%04o> ", machname, machine);
    /* print optional header size */
    fprintf(stdout, "optstart: <0x%x> ", FILHSZ);
    fprintf(stdout, "opthdrsz: <0x%x>\n", opthdrsz);
    /* print sections info */
    fprintf(stdout, "secstart: <0x%lx> ", secstart);
    fprintf(stdout, "numsec: <%ld> ", numsec);
    /* print symbol info */
    fprintf(stdout, "symstart: <0x%lx> ", symstart);
    fprintf(stdout, "numsyms: <%ld>\n", numsyms);
    /* print flags */
    fprintf(stdout, "flags: <%07o> ", flags);
    for (pflag = flaglist; pflag->flag != NULLFLAG; pflag++)
    {
	/* search table for a match */
	if ((pflag->flag & flags) == pflag->flag)
	{
	    /* found a match */
	    fprintf(stdout, "%s ", pflag->flagname);
	}
    }
    fprintf(stdout, "\n");
    /* return */
    return(CFOK);
}

/* dump optional header info */
long dumpohdr(infd)
FILE *infd;
{
    long status, hexdump();

    /* print heading */
    fprintf(stdout, "\noptional hdr info\n"); 
    /* check if an optional header is present */
    if (opthdrsz <= 0)
    {
	fprintf(stdout, "file has no optional header.\n");
	return(CFOK);
    }
    /* dump optional hdr */
    status = hexdump(infd, (long) FILHSZ, opthdrsz, (long) FILHSZ);
    if (status != CFOK) return(status);
    /* return */
    return(CFOK);
}

/* dump all section headers */
long dumpsec(infd)
FILE *infd;
{
    short sec;
    long sflags;
    char secname[16];
    struct scnhdr shdr;
    struct SFLAGS *psflag;
#if 0
    int fseek(), fread();
#endif

    /* print heading */
    fprintf(stdout, "\nsection hdr info\n"); 
    /* check if any sections are present */
    if (numsec <= 0)
    {
	fprintf(stdout, "file has no section headers.\n");
	return(CFNOTOK);
    }
    /* seek to 1st hdr */
    if (fseek(infd, secstart, 0) != 0) return(CFNOTOK);
    /* loop over all section headers and dump data */
    for (sec = 1; sec <= numsec; sec++)
    {
	/* read in file hdr */
	if (fread((char *) &shdr, SCNHSZ, ONEITEM, infd) != ONEITEM) return(CFNOTOK);
	/* dump section name and address for section */
	strncpy(secname, shdr.s_name, 8);
	secname[8] = NULLCHR;
	fprintf(stdout, "section: <%d> secname: <%s>  paddr,vaddr: <0x%lx,0x%lx,%ld>\n",
		sec, secname, shdr.s_paddr, shdr.s_vaddr, 
		((long)shdr.s_vaddr)/(128L*1024L));
	/* dump file pointer to data section and size */
	fprintf(stdout, "datastart: <0x%lx> datasize: <0x%lx>\n",
		shdr.s_scnptr, shdr.s_size);
	/* dump file pointer to reloc info and number of entries */
	fprintf(stdout, "relocstart: <0x%lx> numreloc: <%d>\n",
		shdr.s_relptr, shdr.s_nreloc);
	/* dump file pointer to line number info and how many */
	fprintf(stdout, "linestart: <0x%lx> numline: <%d>\n",
		shdr.s_lnnoptr, shdr.s_nlnno);
	/* dump flags, mask out bits 4-31, keep bits 0-3 */
	sflags = (shdr.s_flags & (unsigned long) 0xf);
	fprintf(stdout, "flags: <%l011o> ", sflags);
	for (psflag = sflaglist; psflag->flag != SFLAGNULL; psflag++)
	{
	    /* search table for a match */
	    if (sflags == psflag->flag)
	    {
		/* found a match */
		fprintf(stdout, "%s ", psflag->sflagname);
	    }
	}
	fprintf(stdout, "\n");
    }
    /* return */
    return(CFOK);
}

/* dump all data */
long dumpdata(infd)
FILE *infd;
{
    short sec;
    char secname[16];
    struct scnhdr shdr;
#if 0
    int fseek(), fread();
#endif
    long status, hexdump();

    /* print heading */
    fprintf(stdout, "\nraw data info\n"); 
    /* check if any sections are present */
    if (numsec <= 0)
    {
	fprintf(stdout, "file has no section headers.\n");
	return(CFNOTOK);
    }
    /* loop over all section headers and dump raw data */
    for (sec = 1; sec <= numsec; sec++)
    {
	/* seek to hdr */
	if (fseek(infd, (secstart + (sec - 1)*SCNHSZ), 0) != 0) return(CFNOTOK);
	/* read in file hdr */
	if (fread((char *) &shdr, SCNHSZ, ONEITEM, infd) != ONEITEM) return(CFNOTOK);
	/* dump section name and address for section */
	strncpy(secname, shdr.s_name, 8);
	secname[8] = NULLCHR;
	fprintf(stdout, "section: <%d,%s> raw data:\n", sec, secname);
	/* verify that the section has any raw data */
	if (shdr.s_size == 0 || shdr.s_scnptr == 0)
	{
	    fprintf(stdout, "no raw data for this section.\n");
	    continue;
	}
	/* dump raw data section */
	status = hexdump(infd, shdr.s_scnptr, shdr.s_size, shdr.s_vaddr);
	if (status != CFOK) return(status);
    }
    /* return */
    return(CFOK);
}

/* dump all relocation info */
long dumpreloc(infd)
FILE *infd;
{
    short sec;
    char *pstr, symname[64], secname[16];
    struct scnhdr shdr;
#if 0
    int fseek(), fread();
#endif
    long status, hexdump();
    unsigned short nreloc;
    struct reloc rhdr;
    struct RFLAGS *prflag;

    /* print heading */
    fprintf(stdout, "\nrelocation info\n"); 
    /* is relocation info available ? */
    if (((flags & F_RELFLG) == F_RELFLG) && ((Ignore & IGNRELFLG) != IGNRELFLG))
    {
	/* no relocation info */
	fprintf(stdout, "no relocation info in this file.\n");
	return(CFOK);
    }
    /* check if any sections are present */
    if (numsec <= 0)
    {
	fprintf(stdout, "file has no section headers.\n");
	return(CFNOTOK);
    }
    /* loop over all section headers and dump relocation info */
    for (sec = 1; sec <= numsec; sec++)
    {
	/* seek to hdr */
	if (fseek(infd, (secstart + (sec - 1)*SCNHSZ), 0) != 0) return(CFNOTOK);
	/* read in file hdr */
	if (fread((char *) &shdr, SCNHSZ, ONEITEM, infd) != ONEITEM) return(CFNOTOK);
	/* dump section name and address for section */
	strncpy(secname, shdr.s_name, 8);
	secname[8] = NULLCHR;
	fprintf(stdout, "section: <%d,%s> relocation:\n", sec, secname);
	/* verify that the section has any relocation data */
	if (shdr.s_relptr == 0 || shdr.s_nreloc == 0)
	{
	    fprintf(stdout, "no relocation info for this section.\n");
	    continue;
	}
	/* print all relocation entries for this section */
	for (nreloc = 1; nreloc <= shdr.s_nreloc; nreloc++)
	{
	    /* seek to relocation entry */
	    if (fseek(infd, shdr.s_relptr + (nreloc - 1)*RELSZ, 0) != 0) return(CFNOTOK);
	    /* read a relocation entry */
	    if (fread((char *) &rhdr, RELSZ, ONEITEM, infd) != ONEITEM) return(CFNOTOK);
	    /* get symbol name */
	    if (symndx2nm(infd, rhdr.r_symndx, symname) != symname) return(CFNOTOK);
	    /* print results */
	    fprintf(stdout, "r_addr: <0x%lx> r_symndx: <%d,%s> r_type: <%04o,",
		    shdr.s_vaddr + rhdr.r_vaddr, rhdr.r_symndx, symname, rhdr.r_type);
	    /* find name for type */
	    for (prflag = rflaglist; prflag->flag != RFLAGNULL; prflag++)
	    {
		if (rhdr.r_type == prflag->flag)
		{
		    fprintf(stdout, "%s", prflag->rflagname);
		}
	    }
	    fprintf(stdout, ">\n");
	}
    }
    /* return */
    return(CFOK);
}

/* dump all line number info */
long dumpline(infd)
FILE *infd;
{
    short sec;
    char *pstr, symname[64], secname[16];
    struct scnhdr shdr;
#if 0
    int fseek(), fread();
#endif
    unsigned short nlnno;
    struct lineno lnhdr;
    long vaddr;

    /* print heading */
    fprintf(stdout, "\nline number info\n"); 
    /* is line number info available ? */
    if (((flags & F_LNNO) == F_LNNO) && ((Ignore & IGNLNNO) != IGNLNNO))
    {
	/* no line number info */
	fprintf(stdout, "no line number info in this file.\n");
	return(CFOK);
    }
    /* check if any sections are present */
    if (numsec <= 0)
    {
	fprintf(stdout, "file has no section headers.\n");
	return(CFNOTOK);
    }
    /* loop over all section headers and dump line number info */
    for (sec = 1; sec <= numsec; sec++)
    {
	/* seek to hdr */
	if (fseek(infd, (secstart + (sec - 1)*SCNHSZ), 0) != 0) return(CFNOTOK);
	/* read in file hdr */
	if (fread((char *) &shdr, SCNHSZ, ONEITEM, infd) != ONEITEM) return(CFNOTOK);
	/* dump section name and address for section */
	strncpy(secname, shdr.s_name, 8);
	secname[8] = NULLCHR;
	fprintf(stdout, "section: <%d,%s> line number:\n", sec, secname);
	/* verify that the section has any line number data */
	if (shdr.s_lnnoptr == 0 || shdr.s_nlnno == 0)
	{
	    fprintf(stdout, "no line number info for this section.\n");
	    continue;
	}
	/* print all line number entries for this section */
	for (nlnno = 1; nlnno <= shdr.s_nlnno; nlnno++)
	{
	    /* seek to line number entry */
	    if (fseek(infd, shdr.s_lnnoptr + (nlnno - 1)*LINESZ, 0) != 0) return(CFNOTOK);
	    /* read a line number entry */
	    if (fread((char *) &lnhdr, LINESZ, ONEITEM, infd) != ONEITEM) return(CFNOTOK);
	    /* check if this is a new function */
	    if (lnhdr.l_lnno == 0)
	    {
		/* new function, get symbol name */
		if (symndx2nm(infd, lnhdr.l_addr.l_symndx, 
			      symname) != symname) return(CFNOTOK);
		/* get address of function */
		vaddr = symndx2nv(infd, lnhdr.l_addr.l_symndx);
		if (vaddr == MAXLONG) return(CFNOTOK);
		/* print new function name */
		fprintf(stdout, "function: <%s> vaddr: <0x%x> entry point\n",
			symname, vaddr);
	    }
	    else
	    {
		/* print line number info */
		fprintf(stdout, "function: <%s> vaddr: <0x%x> lineno: <%d>\n",
			symname, lnhdr.l_addr.l_paddr, lnhdr.l_lnno);
	    }
	}
    }
    /* return */
    return(CFOK);
}

/* physical end of function class */
long sc_efcn(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown efcn auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* null class */
long sc_null(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown null auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* automatic variable class */
long sc_auto(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_NON:
	case DT_PTR:
	    switch (getdtbits(symhdr, TYP))
	    {
	    case T_STRUCT:
	    case T_UNION:
	    case T_ENUM:
		fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, size: <%ld>\n",
			++sym, AUXTAGNDX(auxhdr), AUXSIZE(auxhdr));
		break;
	    default:
		fprintf(stdout, 
			"auxno: <%ld> unknown auto auxiliary record format.\n", ++sym);
		break;
	    }
	    break;
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown auto auxiliary record format.\n", ++sym);
	    break;
	case DT_ARY:
	    fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, lnno: <%ld> arraysize: <%ld>\n",
		    ++sym, AUXTAGNDX(auxhdr), AUXLNNO(auxhdr), AUXSIZE(auxhdr));
	    fprintf(stdout, "arraydim: <%ld,%ld,%ld,%ld)\n",
		    AUXDIMEN(auxhdr, 0), AUXDIMEN(auxhdr, 1),
		    AUXDIMEN(auxhdr, 2), AUXDIMEN(auxhdr, 3));
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* external symbol class */
long sc_ext(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown ext auxiliary record format.\n", 
		    ++sym);
	    break;
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, fsize: <%ld>\n", 
		    ++sym, AUXTAGNDX(auxhdr), AUXFSIZE(auxhdr));
	    fprintf(stdout, "lnnoptr: <%ld>, endndx: <%ld>, tvndx: <%ld>\n", 
		    AUXLNNOPTR(auxhdr), AUXENDNDX(auxhdr), AUXTVNDX(auxhdr));
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* static class */
long sc_stat(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	    fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, lnno: <%ld> arraysize: <%ld>\n",
		    ++sym, AUXTAGNDX(auxhdr), AUXLNNO(auxhdr), AUXSIZE(auxhdr));
	    fprintf(stdout, "arraydim: <%ld,%ld,%ld,%ld)\n",
		    AUXDIMEN(auxhdr, 0), AUXDIMEN(auxhdr, 1),
		    AUXDIMEN(auxhdr, 2), AUXDIMEN(auxhdr, 3));
	    break;
	case DT_PTR:
	    switch (getdtbits(symhdr, TYP))
	    {
	    case T_STRUCT:
	    case T_UNION:
	    case T_ENUM:
		fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, size: <%ld>\n",
			++sym, AUXTAGNDX(auxhdr), AUXSIZE(auxhdr));
		break;
	    default:
		fprintf(stdout, 
			"auxno: <%ld> unknown stat auxiliary record format.\n", ++sym);
		break;
	    }
	    break;
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, fsize: <%ld>\n", 
		    ++sym, AUXTAGNDX(auxhdr), AUXFSIZE(auxhdr));
	    fprintf(stdout, "lnnoptr: <%ld>, endndx: <%ld>, tvndx: <%ld>\n", 
		    AUXLNNOPTR(auxhdr), AUXENDNDX(auxhdr), AUXTVNDX(auxhdr));
	    break;
	case DT_NON:
	    switch (getdtbits(symhdr, TYP))
	    {
	    case T_STRUCT:
	    case T_UNION:
	    case T_ENUM:
		fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, size: <%ld>\n",
			++sym, AUXTAGNDX(auxhdr), AUXSIZE(auxhdr));
		break;
	    default:
		if ((strncmp(string, ".text", 5) == 0) ||
		    (strncmp(string, ".data", 5) == 0) ||
		    (strncmp(string, ".bss", 4) == 0))
		{
		    fprintf(stdout, 
			    "auxno: <%ld>, scnlen: <%ld>, nreloc: <%ld>, nlinno: <%ld>\n", 
			    ++sym, AUXSCNLEN(auxhdr), AUXNRELOC(auxhdr), 
			    AUXNLINNO(auxhdr));
		}
		else
		{
		    fprintf(stdout, 
			    "auxno: <%ld> unknown stat auxiliary record format.\n", ++sym);
		}
	    }
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* register variable class */
long sc_reg(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown reg auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* external definition class */
long sc_extdef(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown extdef auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* label class */
long sc_label(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown label auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* undefined label class */
long sc_ulabel(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown ulabel auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* member of structure class */
long sc_mos(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	    fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, lnno: <%ld> arraysize: <%ld>\n",
		    ++sym, AUXTAGNDX(auxhdr), AUXLNNO(auxhdr), AUXSIZE(auxhdr));
	    fprintf(stdout, "arraydim: <%ld,%ld,%ld,%ld)\n",
		    AUXDIMEN(auxhdr, 0), AUXDIMEN(auxhdr, 1),
		    AUXDIMEN(auxhdr, 2), AUXDIMEN(auxhdr, 3));
	    break;
	case DT_PTR:
	case DT_NON:
	    switch (getdtbits(symhdr, TYP))
	    {
	    case T_STRUCT:
	    case T_UNION:
	    case T_ENUM:
		fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, size: <%ld>\n",
			++sym, AUXTAGNDX(auxhdr), AUXSIZE(auxhdr));
		break;
	    default:
		fprintf(stdout, 
			"auxno: <%ld> unknown mos auxiliary record format.\n", ++sym);
		break;
	    }
	    break;
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown mos auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* function argument class */
long sc_arg(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown arg auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* structure tag class */
long sc_strtag(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print sym hdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown strtag auxiliary record format.\n", 
		    ++sym);
	    break;
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld>, size: <%ld>, endndx: <%ld>\n", 
		    ++sym, AUXSIZE(auxhdr), AUXENDNDX(auxhdr));
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* member of union class */
long sc_mou(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	    fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, lnno: <%ld> arraysize: <%ld>\n",
		    ++sym, AUXTAGNDX(auxhdr), AUXLNNO(auxhdr), AUXSIZE(auxhdr));
	    fprintf(stdout, "arraydim: <%ld,%ld,%ld,%ld)\n",
		    AUXDIMEN(auxhdr, 0), AUXDIMEN(auxhdr, 1),
		    AUXDIMEN(auxhdr, 2), AUXDIMEN(auxhdr, 3));
	    break;
	case DT_PTR:
	case DT_NON:
	    switch (getdtbits(symhdr, TYP))
	    {
	    case T_STRUCT:
	    case T_UNION:
	    case T_ENUM:
		fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, size: <%ld>\n",
			++sym, AUXTAGNDX(auxhdr), AUXSIZE(auxhdr));
		break;
	    default:
		fprintf(stdout, 
			"auxno: <%ld> unknown mou auxiliary record format.\n", ++sym);
		break;
	    }
	    break;
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown mou auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* union tag class */
long sc_untag(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown untag auxiliary record format.\n", 
		    ++sym);
	    break;
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld>, size: <%ld>, endndx: <%ld>\n", 
		    ++sym, AUXSIZE(auxhdr), AUXENDNDX(auxhdr));
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* type definition tag class */
long sc_tpdef(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	    fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, lnno: <%ld> arraysize: <%ld>\n",
		    ++sym, AUXTAGNDX(auxhdr), AUXLNNO(auxhdr), AUXSIZE(auxhdr));
	    fprintf(stdout, "arraydim: <%ld,%ld,%ld,%ld)\n",
		    AUXDIMEN(auxhdr, 0), AUXDIMEN(auxhdr, 1),
		    AUXDIMEN(auxhdr, 2), AUXDIMEN(auxhdr, 3));
	    break;
	case DT_PTR:
	case DT_NON:
	    switch (getdtbits(symhdr, TYP))
	    {
	    case T_STRUCT:
	    case T_UNION:
	    case T_ENUM:
		fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, size: <%ld>\n",
			++sym, AUXTAGNDX(auxhdr), AUXSIZE(auxhdr));
		break;
	    default:
		fprintf(stdout, 
			"auxno: <%ld> unknown tpdef auxiliary record format.\n", ++sym);
		break;
	    }
	    break;
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown tpdef auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* undefined static class */
long sc_ustatic(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown ustatic auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* enumeration tag class */
long sc_entag(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown entag auxiliary record format.\n", 
		    ++sym);
	    break;
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld>, size: <%ld>, endndx: <%ld>\n", 
		    ++sym, AUXSIZE(auxhdr), AUXENDNDX(auxhdr));
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* member of enumeration class */
long sc_moe(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown moe auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* register parameter class */
long sc_regparm(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown regparm auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* bit field class */
long sc_field(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown field auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* block class */
long sc_block(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown block auxiliary record format.\n", 
		    ++sym);
	    break;
	case DT_NON:
	    if ((strncmp(string, ".bb", 3) == 0) || (strncmp(string, ".eb", 3) == 0))
	    {
		fprintf(stdout, "auxno: <%ld>, lnno: <%ld>\n", ++sym, AUXLNNO(auxhdr));
	    }
	    else
	    {
		fprintf(stdout, 
			"auxno: <%ld> unknown block auxiliary record format.\n", 
			++sym);
	    }
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* function class */
long sc_fcn(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown fcn auxiliary record format.\n", 
		    ++sym);
	    break;
	case DT_NON:
	    if ((strncmp(string, ".bf", 3) == 0) || (strncmp(string, ".ef", 3) == 0))
	    {
		fprintf(stdout, "auxno: <%ld>, lnno: <%ld>\n", ++sym, AUXLNNO(auxhdr));
	    }
	    else
	    {
		fprintf(stdout, 
			"auxno: <%ld> unknown fcn auxiliary record format.\n", 
			++sym);
	    }
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* end of structure class */
long sc_eos(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown eos auxiliary record format.\n", 
		    ++sym);
	    break;
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld>, tagndx: <%ld>, size: <%ld>\n", 
		    ++sym, AUXTAGNDX(auxhdr), AUXSIZE(auxhdr));
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* file name class */
long sc_file(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	    fprintf(stdout, "auxno: <%ld> unknown file auxiliary record format.\n", 
		    ++sym);
	    break;
	case DT_NON:
	    if (strncmp(string, ".file", 5) == 0)
	    {
		fprintf(stdout, "auxno: <%ld>, filename: <%s>\n", 
			++sym, AUXFNAME(auxhdr));
	    }
	    else
	    {
		fprintf(stdout, 
			"auxno: <%ld> unknown auto auxiliary record format.\n", ++sym);
	    }
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* line class */
long sc_line(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown line auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* duplicate tag class */
long sc_alias(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown alias auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

/* external symbols for DMERT libraries class */
long sc_hidden(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown hidden auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}

#ifndef VAX
/* sdb info class */
long sc_endinit(infd, symhdr, auxhdr, sym, sc, string)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym, sc;
char *string;
{
    /* print symhdr info */
    fprintf(stdout, "symvalue: <0x%x> symnumaux: <%d>\n", 
	    symhdr->n_value, symhdr->n_numaux);
    /* print auxiliary hdr info */
    if (symhdr->n_numaux > 0)
    {
	/* print aux hdr info */
	switch (getdtbits(symhdr, D1))
	{
	case DT_ARY:
	case DT_PTR:
	case DT_FCN:
	case DT_NON:
	    fprintf(stdout, "auxno: <%ld> unknown endinit auxiliary record format.\n", 
		    ++sym);
	    break;
	}
    }
    /* finished, return */
    return(CFOK);
}
#endif

/* dump symbol structure */
long dumpsclass(infd, symhdr, auxhdr, sym)
FILE *infd;
struct syment *symhdr;
union auxent *auxhdr;
long sym;
{
    char string[64], symtype[128], secname[64];
    long status, sc;

    /* assume the class is unknown */
    status = CFUNKSCL;
    /* search for the appropriate storage class function */
    for (sc = 0; scfnlist[sc].fn != NULLSCFCN; sc++)
    {
	if (scfnlist[sc].sclass == symhdr->n_sclass)
	{
	    /* get symbol name */
	    if (getsymnm(infd, symhdr, string) != string) return(CFNOTOK);
	    /* get symbol type */
	    if (getsymtype(symhdr, symtype) != symtype) return(CFNOTOK);
	    /* get section name */
	    if (getsecnm(infd, symhdr, secname) != secname) return(CFNOTOK);
	    /* write data */
	    fprintf(stdout, "symno: <%ld> symname: <%s> symsclass: <0x%x, %s>\n",
		    sym, string, symhdr->n_sclass, scfnlist[sc].sc_name);
	    fprintf(stdout, "symscnum: <%d, %s> ", symhdr->n_scnum, secname);
	    fprintf(stdout, "symtype: <0x%x, %s> ", symhdr->n_type, symtype);
	    /* dump section number, value, type, etc. data */
	    status = (*scfnlist[sc].fn)(infd, symhdr, auxhdr, sym, sc, string);
	    fprintf(stdout, "\n");
	    break;
	}
    }
    /* done, return */
    return(status);
}

/* dump symbol table */
long dumpsym(infd)
FILE *infd;
{
    long offset, sym, deltasym;
    struct syment symhdr;
    union auxent auxhdr;
    long status, dumpsclass();
#if 0
    int fseek(), fread();
#endif

    /* print heading */
    fprintf(stdout, "\nsymbol table info\n"); 
    /* is symbol table info available ? */
    if ((flags & F_LSYMS) == F_LSYMS)
    {
	/* no local symbol info */
	fprintf(stdout, "WARNING: no local symbol info in this file.\n");
    }
    /* check if any symbols are present */
    if (numsyms <= 0)
    {
	fprintf(stdout, "file has no symbols defined.\n");
	return(CFOK);
    }
    /* loop over all symbols, and dump info */
    for (deltasym = 1, sym = 0; sym < numsyms; sym += deltasym)
    {
	offset = symstart + sym*SYMESZ;
	/* seek to next symbol in symbol table */
	if (fseek(infd, offset, 0) != 0) return(CFNOTOK);
	/* read in sym hdr */
	if (fread((char *) &symhdr, SYMESZ, ONEITEM, infd) != ONEITEM) return(CFNOTOK);
	/* read in auxiliary entry, if any */
	if (symhdr.n_numaux > 0)
	{
	    /* read in auxiliary hdr */
	    if (fread((char *) &auxhdr, 
		      AUXESZ, ONEITEM, infd) != ONEITEM) return(CFNOTOK);
	}
	/* calculate delta for loop increment */
	deltasym = symhdr.n_numaux + 1;
	/* dump out data depending on storage class */
	status = dumpsclass(infd, &symhdr, &auxhdr, sym);
	switch (status)
	{
	case CFOK:
	    /* ok, continue */
	    break;
	case CFUNKSCL:
	    /* return on bad class, unless flag to ignore is set */
	    if ((Ignore & IGNSCLASS) != IGNSCLASS) return(status);
	    break;
	default:
	    /* bad class info */
	    return(status);
	}
    }
    /* return */
    return(CFOK);
}

/* dump string table */
long dumpstr(infd)
FILE *infd;
{
    char *pstr, *pbuf;
    long str, nstr, offset, stringsize;
    struct stat coffstat;
#if 0
    void free();
    char *malloc();
    int oinfd, fstat(), fread(), fseek();
#else
    int oinfd;
#endif

    /* print heading */
    fprintf(stdout, "\nstring table info\n"); 
    /* calculate the offset to the beginning of the string table */
    offset = symstart + numsyms*SYMESZ;
    /* check that this file has a string table, check sizes */
    oinfd = fileno(infd);
    if (fstat(oinfd, &coffstat) != 0) return(CFNOTOK);
    /*
     * smallest string table is: a long for the size,
     * and a 9 character null terminated string. if the
     * offset plus the smallest string table size is 
     * greater than the file size, then there is no
     * string table.
     */
    if ((offset + sizeof(long) + 9) > (coffstat.st_size))
    {
	fprintf(stdout, "file has no long strings defined.\n");
	return(CFOK);
    }
    /* seek to the beginning of the string table */
    if (fseek(infd, offset, 0) != 0) return(CFNOTOK);
    /* read size of string table */
    if (fread((char *) &stringsize, 
	      sizeof(long), ONEITEM, infd) != ONEITEM) return(CFNOTOK);
    /* are there any strings ? */
    if (stringsize == 0)
    {
	/* no long strings */
	fprintf(stdout, "no string table info in this file.\n");
	return(CFOK);
    }
    /* print the number of bytes in string table */
    fprintf(stdout, "stringsize: <0x%lx>\n", stringsize);
    /* get a buffer to read strings */
    pbuf = malloc((unsigned) BUFSIZ*sizeof(char));
    if (pbuf == (char *) NULLPTR) return(CFNOTOK);
    strcpy(pbuf, CFALL);
    /* read the string table and print strings */
    for (str = 1, nstr = sizeof(long); 
        (nstr < stringsize) && (strlen(pbuf) > 1); str++, nstr += strlen(pbuf) + 1)
    {
	/* seek to next string */
	if (fseek(infd, (symstart + numsyms*SYMESZ + nstr), 0) != 0)
	{
	    /* free memory */
	    free((char *) pbuf);
	    return(CFNOTOK);
	}
	/* read in null terminated string */
	for (pstr = pbuf; ((*pstr = getc(infd)) != NULLCHR); pstr++)
	{
	    /* do nothing */
	}
	*pstr = NULLCHR;
	/* print string */
	fprintf(stdout, "stringno: <%ld> offset: <0x%lx> string: <%s>\n", 
		str, nstr, pbuf);
    }
    /* free memory */
    free(pbuf);
    /* finished */
    return(CFOK);
}

/* dump sections in coff file, no tracing */
long dumpall(infd)
FILE *infd;
{
    long status;
    long dumpsec();
    long dumpdata();
    long dumpreloc();
    long dumpline();
    long dumpsym();
    long dumpstr();

    /* dump section hdr info */
    if ((optflag & CFSECHDR2) == CFSECHDR2)
    {
	if ((status = dumpsec(infd)) != CFOK) return(status);
    }
    /* dump data info */
    if ((optflag & CFRAWDATA) == CFRAWDATA)
    {
	if ((status = dumpdata(infd)) != CFOK) return(status);
    }
    /* dump reloc info */
    if ((optflag & CFRELOC) == CFRELOC)
    {
	if ((status = dumpreloc(infd)) != CFOK) return(status);
    }
    /* dump line info */
    if ((optflag & CFLINE) == CFLINE)
    {
	if ((status = dumpline(infd)) != CFOK) return(status);
    }
    /* dump symbol table info */
    if ((optflag & CFSYMBOL) == CFSYMBOL)
    {
	if ((status = dumpsym(infd)) != CFOK) return(status);
    }
    /* dump string table info */
    if ((optflag & CFSTRING) == CFSTRING)
    {
	if ((status = dumpstr(infd)) != CFOK) return(status);
    }
    /* return */
    return(CFOK);
}

/* function to dump out COFF info */
long coffdump(infd, fname)
FILE *infd;
char *fname;
{
    long status;

    /* dump file header info */
    if ((optflag & CFFILEHDR) == CFFILEHDR)
    {
	if ((status = dumpfhdr(fname)) != CFOK) return(status);
    }
    /* dump optional header info */
    if ((optflag & CFOPTHDR) == CFOPTHDR)
    {
	if ((status = dumpohdr(infd)) != CFOK) return(status);
    }
    /* check if any file, function, section qualifier is present */
    if ((optflag & (CFASECHDR|CFNSECHDR|CFSSECHDR|CFFILENAME|CFFUNCTION)) == 0L)
    {
	/* no qualifier, just dumps of sections */
	if ((status = dumpall(infd)) != CFOK) return(status);
    }
    /* done, return */
    return(CFOK);
}

/* function to get list from command line */
char **getlist(clist)
char *clist;
{
#if 0
    char *malloc();
#endif
    register char **plist;
    register char *pstr;
    register short slen, h;
    short num;

    /* determine how many list elements, count commas */
    for (num = 0, pstr = clist; *pstr != NULLCHR; pstr++)
    {
	if (*pstr == COMMA) num++;
    }
    /* number of elements is one more than number of commas */
    num++;
    /* allocate array of pointers for list */
    plist = (char **) malloc((unsigned) ((num + 1)*sizeof(char *)));
    if (plist == (char **) NULLPTR) return((char **) NULLPTR);
    /* get list elements and store */
    for (pstr = clist, slen = 0, h = 0; h < num; h++, pstr++)
    {
	/* find length of string between commas */
	if ((slen = strcspn(pstr, DELIMITER)) == 0) return((char **) NULLPTR);
	/* store pointer to string */
	plist[h] = pstr;
	/* reset string pointer */
	pstr += slen;
	/* replace comma with a null */
	*pstr = NULLCHR;
    }
    /* null out the last pointer to indicate the end of the list */
    plist[num] = (char *) NULLPTR;
    /* return pointer to list */
    return(plist);
}

/* start of main program */
main(argc, argv)
int argc;
char **argv;
{
    FILE *infd;
    int coff, c;
    long status;
#if 0
    int getopt();
#endif
    register char **plist;
    char **getlist();
    long getfhdr(), coffdump();

    /* initialize some variables */
    optflag = CFNULL;
    Ignore = IGNNULL;

    /* get options from command line */
    while ((c = getopt(argc,argv, "?hoc:CdrlstF:f:g:ueI")) != EOF)
    {
	switch (c)
	{
	case 'I':
	    /* ignore certain errors or file header flags */
	    Ignore |= IGNSCLASS|IGNMAGIC|IGNRELFLG|IGNLNNO;
	    break;
	case 'h':
	    /* print file header */
	    optflag |= CFFILEHDR;
	    break;
	case 'o':
	    /* print optional file header */
	    optflag |= CFOPTHDR;
	    break;
	case 'c':
	    /* print section headers */
	    if ((clist = getlist(optarg)) == (char **) NULLPTR)
	    {
		USAGE("-c option, unable to get list of section numbers.");
		exit(2);
	    }
	    /* verify the list numbers, section names, or the word "all" */
	    if (strcmp(*clist, CFALL) != 0)
	    {
		for (plist = clist; *plist != (char *) NULLPTR; plist++)
		{
		    if (strspn(*plist, CFNUMBERS) == strlen(*plist))
		    {
			/* only numbers, a number list was given */
			optflag |= CFNSECHDR;
		    }
		    else
		    {
			/* a section name list was given */
			optflag |= CFSSECHDR;
		    }
		}
		if ((optflag & (CFNSECHDR | CFSSECHDR)) == (CFSSECHDR | CFNSECHDR))
		{
		    USAGE("-c option, cannot mix numbers and section names.\n");
		    exit(2);
		}
	    }
	    else
	    {
		/* "all" was given */
		optflag |= CFASECHDR;
	    }
	    break;
	case 'C':
	    /* print section headers */
	    optflag |= CFSECHDR2;
	    break;
	case 'd':
	    /* print raw data */
	    optflag |= CFRAWDATA;
	    break;
	case 'r':
	    /* print relocation data */
	    optflag |= CFRELOC;
	    break;
	case 'l':
	    /* print line number data */
	    optflag |= CFLINE;
	    break;
	case 's':
	    /* print symbol data */
	    optflag |= CFSYMBOL;
	    break;
	case 't':
	    /* print string table data */
	    optflag |= CFSTRING;
	    break;
	case 'F':
	    /* print data for given file */
	    if ((Flist = getlist(optarg)) == (char **) NULLPTR)
	    {
		USAGE("-F option, unable to get list of file names.");
		exit(2);
	    }
	    optflag |= CFFILENAME;
	    break;
	case 'f':
	    /* print data for given function */
	    if ((flist = getlist(optarg)) == (char **) NULLPTR)
	    {
		USAGE("-f option, unable to get list of function names.");
		exit(2);
	    }
	    optflag |= CFFUNCTION;
	    break;
	case 'g':
	    /* print global variables */
	    if ((glist = getlist(optarg)) == (char **) NULLPTR)
	    {
		USAGE("-g option, unable to get list of global names.");
		exit(2);
	    }
	    optflag |= CFGLOBAL;
	    break;
	case 'u':
	    /* print unresolved references */
	    optflag |= CFUNRESOLVED;
	    break;
	case 'e':
	    /* print external symbols */
	    optflag |= CFEXTERN;
	    break;
	case '?':
	    /* help text */
	    USAGE("");
	    exit(2);
	default:
	    /* bad option in command line */
	    USAGE("bad option in command line.");
	    exit(2);
	}
    }

    /* verify a file was given */
    if (optind >= argc)
    {
	USAGE("no COFF file was given.");
	exit(2);
    }

    /* start looping over all COFF files */
    for (coff = optind; coff < argc; coff++)
    {
	/* open coff file for read */
	if ((infd = fopen(argv[coff], "r")) == (FILE *) NULLPTR)
	{
	    fprintf(stderr, "unable to open for read COFF file - %s.\n", argv[coff]);
	    exit(2);
	}
	/* get file header info */
	status = getfhdr(infd);
	switch (status)
	{
	case CFOK:
	    /* read header info ok */
	    break;
	case CFBADMAGIC:
	    /* bad magic, skip this file */
	    fprintf(stderr, "bad magic in COFF file - %s.\n", argv[coff]);
	    (void) fclose(infd);
	    continue;
	default:
	    /* bad failure, abort */
	    fprintf(stderr, "reading file header failed for COFF file - %s.\n", 
		   argv[coff]);
	    (void) fclose(infd);
	    exit(2);
	}
	/* print request info */
	if ((status = coffdump(infd, argv[coff])) != CFOK)
	{
	    fprintf(stderr, "COFF dump failed for file - %s.\n", argv[coff]);
	    (void) fclose(infd);
	    exit(2);
	}
	/* close COFF file */
	(void) fclose(infd);
    }
    
    /* finished, exit */
    exit(0);
}
