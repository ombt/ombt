/*
 * function get all the line numbers for a given function name.
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

/* externs for getopt */
extern char *optarg;
extern int optind, opterr;
extern int errno, sys_nerr;
extern char *sys_errlist[];

/* structure for function line number data */
struct LNNO {
    unsigned short lnno;
    unsigned long addr;
} ;

struct FUNCLNNO {
    char *fname;
    char file[AUXESZ + 1];
    unsigned short nlines;
    unsigned short maxnlines;
    struct LNNO *lndata;
} ;

/* initial expected number of lines before realloc */
#define NUMLINES 200

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

/* local definitions */
#define F2LNSFALSE 0
#define F2LNSTRUE 1
#define F2LNSNOTOK 0
#define F2LNSOK 1
#define F2LNSNOTFOUND 0
#define F2LNSFOUND 1
#define NULLCHR '\0'
#define ONEITEM 1
#define USAGE "Usage: f2lns [-?] [-sFA] [-o object,...] [-p path,...] [-f file.c] function ..."
#define DELIMITERS ":,"

/* error handling */
void usage(errmsg, prtusage)
char *errmsg;
int prtusage;
{
    if (*errmsg != NULLCHR) fprintf(stderr, "%s\n", errmsg);
    if (errno > 0 && errno < sys_nerr)
    {
        fprintf(stderr, "error %d : %s\n", errno, sys_errlist[errno]);
    }
    if (prtusage == F2LNSOK) fprintf(stderr, "%s\n", USAGE);
    return;
}

/* find the file which contains a function */
int func2file(infd, fhdr, symndx, file)
FILE *infd;
struct filehdr *fhdr;
long symndx;
char *file;
{
    long offset;
    struct syment symhdr;
    union auxent auxhdr;

    /* seach backwards from the present location for file name */
    for ( ; symndx > 0; symndx--)
    {
	/* seek to location of symbol entry */
	offset = fhdr->f_symptr + symndx*SYMESZ;
	if (fseek(infd, offset, 0) != 0) return(F2LNSNOTOK);
	/* read in symbol table entry */
	if (fread((char *) &symhdr, 
		  SYMESZ, ONEITEM, infd) != ONEITEM) return(F2LNSNOTOK);
	/* check if the symbol is a file name */
	if (symhdr.n_sclass == C_FILE)
	{
	    /* check if symbol name is .file */
	    if (strncmp(symhdr._n._n_name, ".file", 5) == 0)
	    {
		/* check if an auxiliary entry is present */
		if (symhdr.n_numaux > 0)
		{
		    /* read auxiliary header for file name */
		    if (fread((char *)&auxhdr, 
			      AUXESZ, ONEITEM, infd) != ONEITEM) return(F2LNSNOTOK);
		    strncpy(file, auxhdr.x_file.x_fname, AUXESZ);
		    /* return with success */
		    file[AUXESZ] = NULLCHR;
		    return(F2LNSOK);
		}
		/* file name was not found */
		*file = NULLCHR;
		return(F2LNSNOTOK);
	    }
	}
    }
    /* file name was not found */
    *file = NULLCHR;
    return(F2LNSNOTOK);
}

/* function to get value from symbol table */
int symndx2nv(infd, fhdr, symndx, nvalue)
FILE *infd;
struct filehdr *fhdr;
long symndx, *nvalue;
{
    long offset;
    struct syment symhdr;

    /* seek to location of symbol entry */
    offset = fhdr->f_symptr + symndx*SYMESZ;
    if (fseek(infd, offset, 0) != 0) return(F2LNSNOTOK);
    /* read in symbol table entry */
    if (fread((char *) &symhdr, SYMESZ, ONEITEM, infd) != ONEITEM) return(F2LNSNOTOK);
    /* return nvalue */
    *nvalue = symhdr.n_value;
    return(F2LNSOK);
}

/* function to get a symbol name given an index */
int symndx2nm(infd, fhdr, symndx, string)
FILE *infd;
struct filehdr *fhdr;
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
    offset = fhdr->f_symptr + symndx*SYMESZ;
    if (fseek(infd, offset, 0) != 0) return(F2LNSNOTOK);
    /* read in symbol table entry */
    if (fread((char *) &symhdr, SYMESZ, ONEITEM, infd) != ONEITEM) return(F2LNSNOTOK);
    /* check if name is in the string table */
    if (symhdr._n._n_n._n_zeroes == 0L)
    {
	/* read string table for symbol name */
	offset = fhdr->f_symptr + fhdr->f_nsyms*SYMESZ + symhdr._n._n_n._n_offset;
	if (fseek(infd, offset, 0) != 0) return(F2LNSNOTOK);
	/* since the string is NULL terminated */
	for (pstr = string; ((*pstr = getc(infd)) != NULLCHR); pstr++)
	{
	    /* do nothing */
	}
	*pstr = NULLCHR;
	/* done, return symbol name */
	return(F2LNSOK);
    }
    /* name is in symhdr structure */
    strncpy(string, symhdr._n._n_name, SYMNMLEN);
    string[SYMNMLEN] = NULLCHR;
    /* return ok */
    return(F2LNSOK);
}

/* compare routine for unix qsort routine */
int lnnocmp(p1, p2)
register char *p1, *p2;
{
    return((int) (((struct LNNO *)p1)->lnno - ((struct LNNO *)p2)->lnno));
}

/* store line number data */
int storedata(pdata, lnno, lineaddr)
struct FUNCLNNO *pdata;
unsigned short lnno;
unsigned long lineaddr;
{
    /* check if data must be reallocated */
    if ((pdata->nlines + 1) >= pdata->maxnlines)
    {
	/* out of space, reallocate */
	pdata->maxnlines += NUMLINES;
	pdata->lndata = (struct LNNO *) realloc((char *)pdata->lndata, 
		(unsigned) pdata->maxnlines*sizeof(struct LNNO));
	if (pdata->lndata == (struct LNNO *) NULL) return(F2LNSNOTOK);
    }
    /* store data */
    pdata->nlines++;
    pdata->lndata[pdata->nlines - 1].lnno = lnno;
    pdata->lndata[pdata->nlines - 1].addr = lineaddr;
    /* return success */
    return(F2LNSOK);
}

/* find the function name in an object file */
int f2lns(fhdr, pdata, infd, sflag, All, pfile)
struct filehdr *fhdr;
struct FUNCLNNO *pdata;
FILE *infd;
int sflag, All;
char *pfile;
{
    int done, found;
    unsigned short sec, flnno, lnno;
    long fileloc;
    unsigned long lineaddr;
    struct scnhdr shdr;
    struct lineno lnhdr;
    char func2[BUFSIZ];
    enum { FFSEARCH, FFSTORE, FFDONE } Fstate;

    /* some initializations */
    done = found = F2LNSFALSE;
    /* search for section containing the function name */
    for (sec = 1; ! done && sec <= fhdr->f_nscns; sec++)
    {
	/* seek to the 1st section header */
	if (fseek(infd, (long) (FILHSZ + fhdr->f_opthdr + 
		 (sec - 1)*SCNHSZ), 0) != 0) return(F2LNSNOTOK);
	/* read in section header */
	if (fread((char *) &shdr, SCNHSZ, ONEITEM, infd) != ONEITEM) return(F2LNSNOTOK);
	/* if section has no line numbers, skip it */
	if (shdr.s_lnnoptr == 0 || shdr.s_nlnno == 0) continue;
	/* seek to start of line number data */
	if (fseek(infd, shdr.s_lnnoptr, 0) != 0) return(F2LNSNOTOK);
	/* cycle thru all the lnno data looking for the function name */
	for (Fstate = FFSEARCH, lnno = 1; ! done && lnno <= shdr.s_nlnno; lnno++)
	{
	    /* read line number data */
	    if (fread((char *) &lnhdr, LINESZ, 
		      ONEITEM, infd) != ONEITEM) return(F2LNSNOTOK);
	    /* switch on state */
	    switch (Fstate)
	    {
	    case FFSEARCH:
		/* check if a new function name was found */
		if (lnhdr.l_lnno == 0)
		{
		    /* new function was found, save file location */
		    fileloc = ftell(infd);
		    /* get the function name */
		    if (symndx2nm(infd, fhdr, lnhdr.l_addr.l_symndx, 
				  func2) != F2LNSOK) return(F2LNSNOTOK);
		    /* check if function names match */
		    if (strcmp(pdata->fname, func2) == 0)
		    {
			/* found a match, get address */
			flnno = 1;
			if (symndx2nv(infd, fhdr, lnhdr.l_addr.l_symndx, 
				      &lineaddr) != F2LNSOK) return(F2LNSNOTOK);
			/* store data */
			if (storedata(pdata, flnno, lineaddr) != F2LNSOK) 
			{
			    return(F2LNSNOTOK);
			}
			/* get file which contains the function */
			if (func2file(infd, fhdr, 
				      lnhdr.l_addr.l_symndx, pdata->file) != F2LNSOK)
			{
			    /* unable to read file name */
			    pdata->file[0] = NULLCHR;
			}
			/* check if a file name must match */
			if (pfile == (char *)NULL)
			{
			    /* set state to store remaining data */
			    Fstate = FFSTORE;
			    found = F2LNSTRUE;
			}
			else if (strcmp(pdata->file, pfile) == 0)
			{
			    /* files match, set state to store remaining data */
			    Fstate = FFSTORE;
			    found = F2LNSTRUE;
			}
			else
			{
			    pdata->nlines = 0;
			}
		    }
		    /* reset file location */
		    if (fseek(infd, fileloc, 0) != 0) return(F2LNSNOTOK);
		}
		break;
	    case FFSTORE:
		/* store line number data until a new function is found */
		if (lnhdr.l_lnno != 0)
		{
		    /* store data */
		    if (storedata(pdata, lnhdr.l_lnno, 
				  lnhdr.l_addr.l_paddr) != F2LNSOK) return(F2LNSNOTOK);
		    /* continue reading data */
		    continue;
		}
		/* done getting data, sort line number data */
		if (sflag) qsort((char *)pdata->lndata, (unsigned) pdata->nlines,
			         sizeof(struct LNNO), lnnocmp);
		/* now print the data */
		fprintf(stdout, "\n<<< function: %s >>>\n", pdata->fname);
		for (lnno = 0; lnno < pdata->nlines; lnno++)
		{
		    if (pdata->file[0] != NULLCHR)
		    {
			fprintf(stdout, "[%s]%s'%d, addr: 0x%lx\n",
				pdata->file, pdata->fname, pdata->lndata[lnno].lnno, 
				pdata->lndata[lnno].addr);
		    }
		    else
		    {
			fprintf(stdout, "%s'%d, addr: 0x%lx\n",
				pdata->fname, pdata->lndata[lnno].lnno, 
				pdata->lndata[lnno].addr);
		    }
		}
		/* data is printed, reset number of lines to zero */
		pdata->nlines = 0;
		/* do we get ALL occurences of the function */
		if (All == F2LNSTRUE)
		{
		    /* reset state to search, and backup the file */
		    Fstate = FFSEARCH;
		    fileloc = ftell(infd) - LINESZ;
		    if (fseek(infd, fileloc, 0) != 0) return(F2LNSNOTOK);
		}
		else
		{
		    /* all done, exit */
		    done = F2LNSTRUE;
		}
		break;
	    default:
		/* error of some type */
		return(F2LNSNOTOK);
	    }
	}
    }
    /* any line data left to sort and print */
    if (pdata->nlines > 0)
    {
	/* sort line number data */
	if (sflag) qsort((char *)pdata->lndata, (unsigned) pdata->nlines,
				  sizeof(struct LNNO), lnnocmp);
	/* now print the data */
	fprintf(stdout, "\n<<< function: %s >>>\n", pdata->fname);
	for (lnno = 0; lnno < pdata->nlines; lnno++)
	{
	    if (pdata->file[0] != NULLCHR)
	    {
		fprintf(stdout, "[%s]%s'%d, addr: 0x%lx\n",
			pdata->file, pdata->fname, pdata->lndata[lnno].lnno, 
			pdata->lndata[lnno].addr);
	    }
	    else
	    {
		fprintf(stdout, "%s'%d, addr: 0x%lx\n",
			pdata->fname, pdata->lndata[lnno].lnno, 
			pdata->lndata[lnno].addr);
	    }
	}
    }
    /* check if line number data found */
    if (found)
    {
	/* line number data were found */
	return(F2LNSFOUND);
    }
    /* no line number data were found */
    return(F2LNSNOTFOUND);
}

/* function to get list from command line */
char **getlist(clist,dlist)
char *clist;
char *dlist;
{
#if 0
    char *malloc();
#endif
    void free();
    char **plist;
    char *pstr, *pdel;
    short slen, h;
    short num;

    /* check arguments */
    if ((clist == (char *) NULL) || (dlist == (char *) NULL)) return((char **)NULL);
    /* determine how many list elements, count commas */
    for (num = 0, pstr = clist; *pstr != NULLCHR; pstr++)
    {
	for (pdel = dlist; (*pstr != *pdel) && (*pdel != NULLCHR); pdel++)
	{
	    /* do nothing */
	}
	if (*pdel != NULLCHR) num++;
    }
    /* number of elements is one more than number of commas */
    num++;
    /* allocate array of pointers for list */
    plist = (char **) malloc((unsigned) ((num + 1)*sizeof(char *)));
    if (plist == (char **) NULL) return((char **) NULL);
    /* get list elements and store */
    for (pstr = clist, slen = 0, h = 0; h < num; h++, pstr++)
    {
	/* find length of string between delimiters */
	if ((slen = strcspn(pstr, dlist)) == 0)
	{
	    /* null length string, free memory and return */
	    (void) free((char *) plist);
	    return((char **) NULL);
	}
	/* store pointer to string */
	plist[h] = pstr;
	/* reset string pointer */
	pstr += slen;
	/* replace delimiter with a null */
	*pstr = NULLCHR;
    }
    /* null out the last pointer to indicate the end of the list */
    plist[num] = (char *) NULL;
    /* return pointer to list */
    return(plist);
}

/* main program */
main(argc, argv)
int argc;
char *argv[];
{
    FILE *infd;
    char fname[BUFSIZ];
    char *pfile;
    struct filehdr fhdr;
    int Flexnames, badmagic;
    int found, status, arg, c, getopt();
    int All, sflag;
    struct scnhdr shdr;
    char func[BUFSIZ];
    char **plist, **olist, **p, **o;
    char *nullplist[2];
    unsigned short machine;
    struct MACHINE *pmach;
    struct FUNCLNNO *pdata;

    /* data initialization */
    pfile = (char *) NULL;
    plist = olist = (char **) NULL;
    All = sflag = Flexnames = F2LNSFALSE;
    /* get command line arguments */
    while ((c = getopt(argc, argv, "?AFso:p:f:")) != EOF)
    {
	/* check options */
	switch (c)
	{
	case 's':
	    /* sort the line numbers */
	    sflag = F2LNSTRUE;
	    break;
	case 'A':
	    /* print all functions with the given name */
	    All = F2LNSTRUE;
	    break;
	case 'f':
	    /* search for the given file name */
	    pfile = optarg;
	    break;
	case 'F':
	    /* use Flex names */
	    Flexnames = F2LNSTRUE;
	    break;
	case 'o':
	    /* get names of COFF file */
	    if ((olist = getlist(optarg, DELIMITERS)) == (char **) NULL)
	    {
		usage("Unable to get object list.", F2LNSOK);
		exit(2);
	    }
	    break;
	case 'p':
	    /* get path for objs, if any */
	    if ((plist = getlist(optarg, DELIMITERS)) == (char **) NULL)
	    {
		usage("Unable to get path list.", F2LNSOK);
		exit(2);
	    }
	    break;
	case '?':
	    /* print usage msg */
	    usage("", F2LNSOK);
	    exit(0);
	default:
	    /* bad option in command line */
	    usage("Bad option in command line.", F2LNSOK);
	    exit(2);
	}
    }
    /* check if a path list was given */
    if (plist == (char **) NULL)
    {
	/* no path was given, check variable LNNOPATH */
	if ((plist = getlist(getenv("LNNOPATH"), DELIMITERS)) == (char **)NULL)
	{
	    /* default to no path */
	    plist = nullplist;
	    nullplist[0] = "";
	    nullplist[1] = (char *) NULL;
	}
    }
    /* check if a object list was given */
    if (olist == (char **) NULL)
    {
	/* no objects were given, check variable LNNOOBJS */
	if ((olist = getlist(getenv("LNNOOBJS"), DELIMITERS)) == (char **)NULL)
	{
	    usage("No object files were given.", F2LNSOK);
	    exit(2);
	}
    }
    /* check if an address was given */
    if (optind >= argc)
    {
	usage("No function names were given.", F2LNSOK);
	exit(2);
    }
#ifdef FLEXWARN
    /* give Flexname warning, if required */
    if ( ! Flexnames)
    {
	fprintf(stderr, 
		"Symbol names will be truncated. To get FLEXNAMES, use -F option.\n");
    }
#endif
    /* allocate an initial block of space for line number data */
    pdata = (struct FUNCLNNO *) malloc((unsigned) sizeof(struct FUNCLNNO));
    if (pdata == (struct FUNCLNNO *)NULL)
    {
	usage("Unable to malloc a function block.", F2LNSNOTOK);
	exit(2);
    }
    /* allocate an initial block of line number data */
    pdata->lndata = (struct LNNO *) malloc((unsigned) NUMLINES*sizeof(struct LNNO));
    if (pdata->lndata == (struct LNNO *)NULL)
    {
	free((char *) pdata);
	usage("Unable to malloc a line number block.", F2LNSNOTOK);
	exit(2);
    }
    /* store the present maximum number of allowed line number data entries */
    pdata->maxnlines = NUMLINES;
    /* cycle thru the fucntion names and find the line number */
    for (arg = optind; arg < argc; arg++)
    {
	/* truncate to 8 characters, if FLEXNAMES is not in effect */
	if (Flexnames == F2LNSFALSE)
	{
	    if (strlen(argv[arg]) > sizeof(shdr.s_name))
	    {
		argv[arg][sizeof(shdr.s_name)] = NULLCHR;
	    }
	}
	/* store function name */
	pdata->fname = argv[arg];
	pdata->nlines = 0;
	/* cycle thru all the paths */
	for (found = F2LNSFALSE, p = plist; ! found && *p != (char *) NULL; p++)
	{
	    /* cycle thru all the objects */
	    for (o = olist; ! found && *o != (char *) NULL; o++)
	    {
		/* get path to object */
		if (strlen(*p) > 0)
		{
		    sprintf(fname, "%s/%s", *p, *o);
		}
		else
		{
		    sprintf(fname, "%s", *o);
		}
		/* open COFF file and read file header */
		if ((infd = fopen(fname, "r")) == (FILE *) NULL)
		{
		    /* unable to open file, go on to next one */
		    continue;
		}
		if (fread((char *) &fhdr, FILHSZ, ONEITEM, infd) != ONEITEM)
		{
		    /* unable to read header, close file and continue */
		    (void) fclose(infd);
		    continue;
		}
		/* check magic */
		for (badmagic = F2LNSTRUE, machine = NULLMAGIC, pmach = machlist; 
		     pmach->machine != NULLMAGIC; pmach++)
		{
		    /* search table for a match */
		    if (pmach->machine == fhdr.f_magic)
		    {
			/* found a match */
			badmagic = F2LNSFALSE;
			break;
		    }
		}
		if (badmagic)
		{
		    /* bad magic for this file */
		    fprintf(stderr, "File %s has unknown magic value.\n", fname);
		    (void) fclose(infd);
		    continue;
		}
		/* check if file is stripped */
		if ((fhdr.f_flags & F_LSYMS) == F_LSYMS)
		{
		    /* file is stripped, close file and continue */
		    fprintf(stderr, "File %s is stripped of symbols.\n", fname);
		    (void) fclose(infd);
		    continue;
		}
		if ((fhdr.f_flags & F_LNNO) == F_LNNO)
		{
		    /* file is stripped, close file and continue */
		    fprintf(stderr, "File %s is stripped of line number data.\n", fname);
		    (void) fclose(infd);
		    continue;
		}
	 	/* search for the function name */
		status = f2lns(&fhdr, pdata, infd, sflag, All, pfile);
		switch (status)
		{
		case F2LNSFOUND:
		    /* function was found, finished */
		    found = F2LNSTRUE;
		    break;
		case F2LNSNOTFOUND:
		    /* function was not found, continue to next file */
		    break;
		default:
		    /* some error */
		    (void) fclose(infd);
		    usage("Error looking for function name.", F2LNSNOTOK);
		    exit(2);
		}
		/* done with COFF file */
		(void) fclose(infd);
	    }
	}
	/* check if address was found */
	if ( ! found)
	{
	    fprintf(stdout, "Function %s not found in any object file.\n", argv[arg]);
	}
    }
    /* all done */
    exit(0);
}
