/*
 * function to get a line number given a hex address.
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
#define LNNOFALSE 0
#define LNNOTRUE 1
#define LNNONOTOK 0
#define LNNOOK 1
#define LNNOFOUND 2
#define LNNONOTFOUND 3
#define LNNONOLINE 4
#define NULLCHR '\0'
#define ONEITEM 1
#define USAGE "Usage: lnno [-?] [-d] [-o object,...] [-p path,...] address ..."
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
    if (prtusage == LNNOOK) fprintf(stderr, "%s\n", USAGE);
    return;
}

/* function to value from symbol table */
int symndx2nv(infd, fhdr, symndx, nvalue)
FILE *infd;
struct filehdr *fhdr;
long symndx, *nvalue;
{
    long offset;
    struct syment symhdr;

    /* seek to location of symbol entry */
    offset = fhdr->f_symptr + symndx*SYMESZ;
    if (fseek(infd, offset, 0) != 0) return(LNNONOTOK);
    /* read in symbol table entry */
    if (fread((char *) &symhdr, SYMESZ, ONEITEM, infd) != ONEITEM) return(LNNONOTOK);
    /* return nvalue */
    *nvalue = symhdr.n_value;
    return(LNNOOK);
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
    if (fseek(infd, offset, 0) != 0) return(LNNONOTOK);
    /* read in symbol table entry */
    if (fread((char *) &symhdr, SYMESZ, ONEITEM, infd) != ONEITEM) return(LNNONOTOK);
    /* check if name is in the string table */
    if (symhdr._n._n_n._n_zeroes == 0L)
    {
	/* read string table for symbol name */
	offset = fhdr->f_symptr + fhdr->f_nsyms*SYMESZ + symhdr._n._n_n._n_offset;
	if (fseek(infd, offset, 0) != 0) return(LNNONOTOK);
	/* since the string is NULL terminated */
	for (pstr = string; ((*pstr = getc(infd)) != NULLCHR); pstr++)
	{
	    /* do nothing */
	}
	*pstr = NULLCHR;
	/* done, return symbol name */
	return(LNNOOK);
    }
    /* name is in symhdr structure */
    strncpy(string, symhdr._n._n_name, SYMNMLEN);
    string[SYMNMLEN] = NULLCHR;
    /* return ok */
    return(LNNOOK);
}

/* find section containing an address */
int getaddrsec(addr, fhdr, infd, addrsec, shdr)
unsigned long addr;
struct filehdr *fhdr;
FILE *infd;
unsigned short *addrsec;
struct scnhdr *shdr;
{
    unsigned short sec;

    /* seek to the 1st section header */
    if (fseek(infd, (long) (FILHSZ + fhdr->f_opthdr), 0) != 0) return(LNNONOTOK);
    /* search for section containing the address */
    for (sec = 1; sec <= fhdr->f_nscns; sec++)
    {
	/* read in section header */
	if (fread((char *) shdr, SCNHSZ, ONEITEM, infd) != ONEITEM) return(LNNONOTOK);
	/* check if section contains the address */
	if ((shdr->s_paddr <= addr) && (addr <= shdr->s_paddr + shdr->s_size))
	{
	    /* found a match */
	    *addrsec = sec;
	    return(LNNOFOUND);
	}
    }
    /* address was not found in any section */
    return(LNNONOTFOUND);
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

    /* search backwards from the present location for file name */
    for ( ; symndx > 0; symndx--)
    {
	/* seek to location of symbol entry */
	offset = fhdr->f_symptr + symndx*SYMESZ;
	if (fseek(infd, offset, 0) != 0) return(LNNONOTOK);
	/* read in symbol table entry */
	if (fread((char *) &symhdr, 
		  SYMESZ, ONEITEM, infd) != ONEITEM) return(LNNONOTOK);
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
			      AUXESZ, ONEITEM, infd) != ONEITEM) return(LNNONOTOK);
		    strncpy(file, auxhdr.x_file.x_fname, AUXESZ);
		    /* return with success */
		    file[AUXESZ] = NULLCHR;
		    return(LNNOOK);
		}
		/* file name was not found */
		*file = NULLCHR;
		return(LNNONOTOK);
	    }
	}
    }
    /* file name was not found */
    *file = NULLCHR;
    return(LNNONOTOK);
}

/* function to get the data for a line number */
int lnno2data(infd, fhdr, shdr, lnno, filename, func, line, lineaddr)
FILE *infd;
struct filehdr *fhdr;
struct scnhdr *shdr;
unsigned short lnno;
char *filename;
char *func;
unsigned short *line;
unsigned long *lineaddr;
{
    struct lineno lnhdr;

    /* check range of line number */
    if (lnno < 1 || lnno > shdr->s_nlnno) return(LNNONOTOK);
    /* seek to location of line number data */
    if (fseek(infd, shdr->s_lnnoptr + (lnno - 1)*LINESZ, 0) != 0) return(LNNONOTOK);
    /* read line number data */
    if (fread((char *) &lnhdr, LINESZ, ONEITEM, infd) != ONEITEM) return(LNNONOTOK);
    /* check if this is a new function */
    if (lnhdr.l_lnno == 0)
    {
	/* get function name */
	if (symndx2nm(infd, fhdr, 
		      lnhdr.l_addr.l_symndx, func) != LNNOOK) return(LNNONOTOK);
	/* get address */
	if (symndx2nv(infd, fhdr, 
		      lnhdr.l_addr.l_symndx, lineaddr) != LNNOOK) return(LNNONOTOK);
	/* set line number */
	*line = 1;
	/* get file name */
	if (func2file(infd, fhdr,
		      lnhdr.l_addr.l_symndx, filename) != LNNOOK) return(LNNONOTOK);
    }
    else
    {
	/* not a new function, save address and line number */
	*lineaddr = lnhdr.l_addr.l_paddr;
	*line = lnhdr.l_lnno;
	/* get function name */
	for ( ; lnhdr.l_lnno != 0 && lnno > 0; lnno--)
	{
	    /* seek to location of line number data */
	    if (fseek(infd, 
		      shdr->s_lnnoptr + (lnno - 1)*LINESZ, 0) != 0) return(LNNONOTOK);
	    /* read line number data */
	    if (fread((char *) &lnhdr, LINESZ, 
	 	      ONEITEM, infd) != ONEITEM) return(LNNONOTOK);
	}
	/* was function name found */
	if (lnhdr.l_lnno != 0) return(LNNONOTOK);
	/* get function name */
	if (symndx2nm(infd, fhdr, 
		      lnhdr.l_addr.l_symndx, func) != LNNOOK) return(LNNONOTOK);
	/* get file name */
	if (func2file(infd, fhdr,
		      lnhdr.l_addr.l_symndx, filename) != LNNOOK) return(LNNONOTOK);
    }
    /* return with data */
    return(LNNOOK);
}

/* function to get the address for a line number */
int lnno2addr(infd, fhdr, shdr, lnno, lineaddr)
FILE *infd;
struct filehdr *fhdr;
struct scnhdr *shdr;
unsigned short lnno;
unsigned long *lineaddr;
{
    struct lineno lnhdr;

    /* check range of line number */
    if (lnno < 1 || lnno > shdr->s_nlnno) return(LNNONOTOK);
    /* seek to location of line number data */
    if (fseek(infd, shdr->s_lnnoptr + (lnno - 1)*LINESZ, 0) != 0) return(LNNONOTOK);
    /* read line number data */
    if (fread((char *) &lnhdr, LINESZ, ONEITEM, infd) != ONEITEM) return(LNNONOTOK);
    /* check if this is a new function */
    if (lnhdr.l_lnno == 0)
    {
	/* new function, get address */
	if (symndx2nv(infd, fhdr, lnhdr.l_addr.l_symndx, lineaddr) != LNNOOK)
	{
	    return(LNNONOTOK);
	}
    }
    else
    {
	/* get line number address */
	*lineaddr = lnhdr.l_addr.l_paddr;
    }
    /* return with address */
    return(LNNOOK);
}

/* find the line number for an address */
int getaddrfn(addr, fhdr, shdr, filename, func, line, lineaddr, infd)
unsigned long addr;
struct filehdr *fhdr;
struct scnhdr *shdr;
char *filename;
char *func;
unsigned short *line;
unsigned long *lineaddr;
FILE *infd;
{
    int found;
    unsigned short lnno;
    unsigned short lowlnno, highlnno;
    unsigned short olowlnno, ohighlnno;
    struct lineno lnhdr;
    unsigned long lnnoaddr, lowaddr, highaddr;

    /* check if section has any line numbers */
    if (shdr->s_lnnoptr == 0 || shdr->s_nlnno == 0) return(LNNONOLINE);
    /* set up indices for binary search */
    found = LNNOFALSE;
    lowlnno = 1;
    olowlnno = 0;
    highlnno = shdr->s_nlnno;
    ohighlnno = shdr->s_nlnno + 1;
    lnno = (lowlnno + highlnno)/2;
    /* get address of middle line number */
    if (lnno2addr(infd, fhdr, shdr, lnno, &lnnoaddr) != LNNOOK) return(LNNONOTOK);
    /* start a binary search to find address */
    while (((lowlnno > olowlnno) || (highlnno < ohighlnno)) && 
	   (highlnno >= lowlnno) && (! found))
    {
	/* do comparisons for search */
	if (lnnoaddr == addr)
	{
	    /* found an exact match */
	    found = LNNOTRUE;
	}
	else if (lnnoaddr > addr)
	{
	    /* lower upper bound */
	    highlnno = lnno - 1;
	}
	else
	{
	    /* raise lower bound */
	    lowlnno = lnno + 1;
	}
	/* new middle address */
	if (! found)
	{
	    lnno = (lowlnno + highlnno)/2;
	    if (lnno2addr(infd, fhdr, shdr, lnno, &lnnoaddr) != LNNOOK) return(LNNONOTOK);
	}
    }
    /*
     * at this point, we either have an exact match, or
     * we have bounded the address from the top and the bottom.
     */
    if ( ! found)
    {
	/* an exact match was not found, find the line number */
	if (lnno2addr(infd, fhdr, shdr, lnno, &lnnoaddr) != LNNOOK) return(LNNONOTOK);
	/* check if lower or greater */
	if (lnnoaddr > addr)
	{	
	    do {
		/* get an upper lower bound */
		lnno = lnno - 1;
		/* an exact match was not found, find the line number */
		if (lnno2addr(infd, fhdr, 
		 	      shdr, lnno, &lnnoaddr) != LNNOOK) return(LNNONOTOK);
	    } while (lnnoaddr > addr && lnno > 0);
	    /* check if a line number was found */
	    if (lnno <= 0) return(LNNONOTFOUND);
	}
	else if (lnnoaddr < addr)
	{
	    do {
		/* get an upper lower bound */
		lnno = lnno + 1;
		/* an exact match was not found, find the line number */
		if (lnno2addr(infd, fhdr, 
		 	      shdr, lnno, &lnnoaddr) != LNNOOK) return(LNNONOTOK);
	    } while (lnnoaddr < addr && lnno <= shdr->s_nlnno);
	    /* always one higher, so decrement */
	    lnno = lnno - 1;
	    /* check if a line number was found */
	    if (lnno >= shdr->s_nlnno) return(LNNONOTFOUND);
	}
    }
    else if (lnno < 1 || lnno > shdr->s_nlnno)
    {
	/* no line number was found */
	return(LNNONOTFOUND);
    }
    /* get data for line number */
    if (lnno2data(infd, fhdr, shdr, lnno, filename, func, 
		  line, lineaddr) != LNNOOK) return(LNNONOTOK);
    /* return with any data */
    return(LNNOFOUND);
}

/* function to get list from command line */
char **getlist(clist,dlist)
char *clist;
char *dlist;
{
#if 0
    char *malloc();
    void free();
#endif
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
    char fname[BUFSIZ], filename[BUFSIZ];
    struct filehdr fhdr;
#if 0
    int getopt();
#endif
    int give_details, badmagic, found, status, arg, c;
    unsigned short addrsec, line;
    unsigned long addr, lineaddr;
    struct scnhdr shdr;
    char func[BUFSIZ];
    char **plist, **olist, **p, **o;
    char *nullplist[2];
    unsigned short machine;
    struct MACHINE *pmach;

    /* data initialization */
    plist = olist = (char **) NULL;
    give_details = LNNOFALSE;
    /* get command line arguments */
    while ((c = getopt(argc, argv, "?do:p:")) != EOF)
    {
	/* check options */
	switch (c)
	{
	case 'd':
	    /* print details */
	    give_details = LNNOTRUE;
	    break;
	case 'o':
	    /* get names of COFF file */
	    if ((olist = getlist(optarg, DELIMITERS)) == (char **) NULL)
	    {
		usage("Unable to get object list.", LNNOOK);
		exit(2);
	    }
	    break;
	case 'p':
	    /* get path for objs, if any */
	    if ((plist = getlist(optarg, DELIMITERS)) == (char **) NULL)
	    {
		usage("Unable to get path list.", LNNOOK);
		exit(2);
	    }
	    break;
	case '?':
	    /* print usage msg */
	    usage("", LNNOOK);
	    exit(0);
	default:
	    /* bad option in command line */
	    usage("Bad option in command line.", LNNOOK);
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
	    usage("No object files were given.", LNNOOK);
	    exit(2);
	}
    }
    /* check if an address was given */
    if (optind >= argc)
    {
	usage("No addresses were given.", LNNOOK);
	exit(2);
    }
    /* cycle thru the addresses and find the line number */
    for (arg = optind; arg < argc; arg++)
    {
	/* convert address to hex */
	(void) sscanf(argv[arg], "%lx", &addr);
	/* cycle thru all the paths */
	for (found = LNNOFALSE, p = plist; ! found && *p != (char *) NULL; p++)
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
		for (badmagic = LNNOTRUE, machine = NULLMAGIC, pmach = machlist; 
		     pmach->machine != NULLMAGIC; pmach++)
		{
		    /* search table for a match */
		    if (pmach->machine == fhdr.f_magic)
		    {
			/* found a match */
			badmagic = LNNOFALSE;
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
	 	/* search for section which contains the address */
		status = getaddrsec(addr, &fhdr, infd, &addrsec, &shdr);
		switch (status)
		{
		case LNNOFOUND:
		    /* a section was found, now find the function */
		    found = LNNOTRUE;
		    status = getaddrfn(addr, &fhdr, &shdr, 
				       filename, func, &line, &lineaddr, infd);
		    switch (status)
		    {
		    case LNNOFOUND:
			if (give_details)
			{
			    char secname[9];
			    strncpy(secname, shdr.s_name, 8);
			    secname[8] = 0;
			    fprintf(stdout, 
				   "obj: %s, sec: <%d, %s> [%s]%s'%d, line addr: 0x%lx, addr: 0x%lx\n",
				    fname, addrsec, secname, filename, func, line, lineaddr, addr);
			}
			else
			{
			    fprintf(stdout, 
				   "[%s]%s'%d, line addr: 0x%lx, addr: 0x%lx\n",
				    filename, func, line, lineaddr, addr);
			}
			break;
		    case LNNONOTFOUND:
		    case LNNONOLINE:
		    {
			/* found the section which contains the address */
			char secname[9];
			strncpy(secname, shdr.s_name, 8);
			secname[8] = 0;
			fprintf(stdout, 
			"obj: %s, sec: <%d, %s>, start, end: <0x%lx, 0x%lx>, addr: 0x%lx.\n", 
			fname, addrsec, secname, shdr.s_paddr, 
			shdr.s_paddr + shdr.s_size, addr);
			break;
		    }
		    default:
			(void) fclose(infd);
			usage("Error getting line number.", LNNONOTOK);
			/*
			exit(2);
			 */
			break;
		    }
		    break;
		case LNNONOTFOUND:
		    /* address was not found, continue to next file */
		    break;
		default:
		    /* some error */
		    (void) fclose(infd);
		    usage("Error looking for section which contains the address.", 
			  LNNONOTOK);
		    exit(2);
		}
		/* done with COFF file */
		(void) fclose(infd);
	    }
	}
	/* check if address was found */
	if ( ! found)
	{
	    fprintf(stdout, "Address 0x%lx not found in any object file.\n", addr);
	}
    }
    /* all done */
    exit(0);
}
