/*
 * this function creates a tree of all the functions
 * called by a given function. it can handle MC680XX
 * transfer vectors, 3B20 transfer vectors, and 3B20
 * UNIX products.
 */

/* unix headers */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <memory.h>
#include <sys/types.h>
#if 0
#include <sys/dir.h>
#else
#include <dirent.h>
#endif
#include <sys/stat.h>

/* COFF headers */
#include "filehdr.h"
#include "aouthdr.h"
#include "scnhdr.h"
#include "reloc.h"
#include "linenum.h"
#include "syms.h"

/* other headers */
#include "debug.h"

/* externs for getopt */
extern char *optarg;
extern int optind, opterr;
extern int errno, sys_nerr;
extern char *sys_errlist[];

/* local definitions */
#define USAGE "\n\
usage: ftrc [-?ertauA] \n\
            [-m 3b|3btv|mctv] \n\
            [-l maxlevel] \n\
            [-o object,...] \n\
            [-v | [-p path,...]] functions\n\
\n\
    -e = create entry breakpoint.\n\
    -r = create return breakpoint.\n\
    -t = print function transfer vector address.\n\
    -a = print function entry address.\n\
    -u = print only the list of functions called, not the sequence of calls.\n\
    -A = scan all files in present directory, ignores -o, -p, -v.\n\
    -m 3b = 3B UNIX magic (NO transfer vectors).\n\
    -m 3btv = 3B OSDS transfer vector magic.\n\
    -m mctv = 680X0 transfer vector magic.\n\
    -l maxlevel = maximum number of levels to trace.\n\
    -o object,... = comma or colon separated list of products to scan.\n\
    -v = generate path for objects using VPATH, same as vuls.\n\
    -p object,... = comma or colon separated list of paths to scan.\n\
    functions = list of functions to trace (no commas or colons).\n\
\n\
Options -v and -p cannot be used simultaneously."

#define NOTOK 0
#define OK 1
#define NOTFOUND 2
#define DUPLICATE 3
#define DELIMITERS ":,"
#define NUMPATHS 100
#define PATHSZ 128
#define ONEITEM 1
#define TVSECTION ".tv"
#define MCTVJSR1 0x4e
#define MCTVJSR2 0xb9
#define N3BTVCALL 0x76
#define N3BCALL1 0xb9
#define N3BCALL2 0x8
#define FNAMESZ 16
#define DIVISOR 201
#define NBINS 210
#define UNIQUE 1
#define TVADDRESS 2
#define ADDRESS 4
#define WEPRINT 8
#define WRPRINT 16
#define ALLFILES 32
#define N3B "3b"
#define N3BTV "3btv"
#define MCTV "mctv"

/* macro for printing line numbers */
#define PRINTLNNO(fd) fprintf(((fd != NULL) ? fd : stderr), "%s'%d: ", __FILE__, __LINE__)

/* statistics */
#ifdef COUNT
int new, old;
#endif

/* structure for function tree */
struct TREE {
    unsigned long tvaddress;
    unsigned long address;
    char function[FNAMESZ];
    struct TREE *sibling;
    struct TREE *child;
} ;

/* structure for transfer vector table */
struct MCTVENTRY {
    unsigned short opcode;
    unsigned short address[2];
} ;

struct N3BTVENTRY {
    unsigned short address[2];
} ;

struct TVTBL {
    unsigned long startaddr;
    unsigned long endaddr;
    char *tvtbl;
} ;

/* definition for hash table */
struct HASHTBL {
    struct TREE *node;
    struct HASHTBL *next;
} ;

/* hash table for functions */
struct HASHTBL *hashtbl[NBINS];

/* table for storing unique entries */
struct UNIQUETBL {
    unsigned long tvaddress;
    struct UNIQUETBL *next;
} ;
    
/* error handling */
void usage(errmsg, prtusage)
char *errmsg;
int prtusage;
{
TRACE();
    if (*errmsg != NULL) fprintf(stderr, "%s\n", errmsg);
    if (errno > 0 && errno < sys_nerr)
    {
        fprintf(stderr, "error %d : %s\n", errno, sys_errlist[errno]);
    }
    if (prtusage == OK) fprintf(stderr, "%s\n", USAGE);
    return;
}

/* malloc and NULL out buffer */
char *Zmalloc(size)
unsigned size;
{
    char *p;

TRACE();
    p = malloc(size);
    if (p != NULL) (void) memset(p, 0, (int) size);
    return(p);
}

/* function to read in blocks of data and allocate a data buffer */
int readdata(infd, offset, datasize, pdata)
FILE *infd;
long offset;
int datasize;
char **pdata;
{
    /* check arguments */
TRACE();
    if (infd == NULL || pdata == NULL || offset < 0 || datasize <= 0)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "bad parameters were passed.\n");
	return(NOTOK);
    }
    /* allocate a buffer for reading in data */
    if (*pdata == NULL)
    {
	*pdata = Zmalloc(datasize);
	if (*pdata == NULL)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to malloc a buffer for data block.\n");
	    return(NOTOK);
        }
    }
    else
    {
	errno = 0;
	*pdata = realloc(*pdata, datasize);
	if (*pdata == NULL)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to realloc a buffer for data block.\n");
	    fprintf(stderr, "requested data size is %d, pointer is 0x%lx, errno is %d.\n", datasize, *pdata, errno);
	    return(NOTOK);
        }
    }
    /* seek to file location and read in data */
    if (fseek(infd, offset, 0) != 0)
    {
	usage("", NOTOK);
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to seek to data block.\n");
	return(NOTOK);
    }
    if (fread(*pdata, datasize, ONEITEM, infd) != ONEITEM)
    {
	usage("", NOTOK);
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to read in data block.\n");
	return(NOTOK);
    }
    /* all done */
    return(OK);
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
TRACE();
    if ((clist == (char *) NULL) || (dlist == (char *) NULL)) return((char **)NULL);
    /* determine how many list elements, count commas */
    for (num = 0, pstr = clist; *pstr != NULL; pstr++)
    {
	for (pdel = dlist; (*pstr != *pdel) && (*pdel != NULL); pdel++)
	{
	    /* do nothing */
	}
	if (*pdel != NULL) num++;
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
	*pstr = NULL;
    }
    /* null out the last pointer to indicate the end of the list */
    plist[num] = (char *) NULL;
    /* return pointer to list */
    return(plist);
}

/* function to read in transfer vector tables from object files */
int gettv(oplist, tv)
char **oplist;
struct TVTBL **tv;
{
    int status, sec, maxtvidx;
    char secname[16];
    FILE *infd;
    struct filehdr *fhdr;
    struct scnhdr *shdr;

    /* initialize */
TRACE();
    maxtvidx = 0;
    *tv = (struct TVTBL *) NULL;
    fhdr = (struct filehdr *) NULL;
    shdr = (struct scnhdr *) NULL;
    /* check input parameters */
    if (oplist == NULL || tv == NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "bad arguments passed.\n");
	return(NOTOK);
    }
    /* cycle thru all files in oplist, and generate a tv table */
    for (maxtvidx = 0; *oplist != NULL; oplist++)
    {
	/* open file for read */
	if ((infd = fopen(*oplist, "r")) == (FILE *) NULL)
	{
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to open file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* read in file header */
	status = readdata(infd, 0L, FILHSZ, (char *)&fhdr);
	if (status != OK)
	{
	    PRINTLNNO(stderr);
	    (void) fclose(infd);
	    fprintf(stderr, "unable to read file header for file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* read in section headers */
	status = readdata(infd, 
			 (long) (FILHSZ + fhdr->f_opthdr), 
			  fhdr->f_nscns*SCNHSZ, 
			 (char *)&shdr);
	if (status != OK)
	{
	    PRINTLNNO(stderr);
	    (void) fclose(infd);
	    fprintf(stderr, 
		   "unable to read section headers for file %s.\n", 
		   *oplist);
	    return(NOTOK);
	}
	/* search for the transfer vector table */
	for (sec = 0; sec < fhdr->f_nscns; sec++)
	{
	    /* get section name */
	    (void) memset(secname, 0L, 8L);
	    (void) strncpy(secname, shdr[sec].s_name, 8);
	    secname[8] = NULL;
	    /* check if this section contains transfer vectors */
	    if (strcmp(secname, TVSECTION) == 0)
	    {
		/* allocate a buffer to read in tv section */
		if (maxtvidx <= 0)
		{
		    /* first tv section to be read in */
		    *tv = (struct TVTBL *) Zmalloc(sizeof(struct TVTBL));
		    if (*tv == (struct TVTBL *) NULL)
		    {
			usage("", NOTOK);
			PRINTLNNO(stderr);
			(void) fclose(infd);
			fprintf(stderr, 
			       "unable to malloc a tv section for file %s.\n", 
			       *oplist);
			return(NOTOK);
		    }
		    /* set index to point to 1st tv section */
		    maxtvidx = 1;
		}
		else
		{
		    /* reallocate an additional buffer */
		    maxtvidx++;
		    *tv = (struct TVTBL *) realloc((char *) *tv, 
			  (unsigned) (maxtvidx*sizeof(struct TVTBL)));
		    if (*tv == (struct TVTBL *) NULL)
		    {
			PRINTLNNO(stderr);
			(void) fclose(infd);
			fprintf(stderr, 
			       "unable to realloc a tv section for file %s.\n", 
			       *oplist);
			return(NOTOK);
		    }
		}
		/* store address range for tv vectors */
		(*tv)[maxtvidx-1].startaddr = shdr[sec].s_paddr;
		(*tv)[maxtvidx-1].endaddr = shdr[sec].s_paddr + shdr[sec].s_size;
		/* read in tv data */
		(*tv)[maxtvidx-1].tvtbl = NULL;
		status = readdata(infd, 
				  shdr[sec].s_scnptr, 
				  shdr[sec].s_size, 
				 (char *) &((*tv)[maxtvidx-1].tvtbl));
		if (status != OK)
		{
		    PRINTLNNO(stderr);
		    (void) fclose(infd);
		    fprintf(stderr, 
			   "unable to read tv data for file %s.\n", 
			   *oplist);
		    return(NOTOK);
		}
	    }
	}
	/* all done with this object file */
	(void) fclose(infd);
	infd = (FILE *) NULL;
    }
    if (infd != NULL) (void) fclose(infd);
    /* check if tv tables were found */
    if (maxtvidx <= 0)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "no transfer vector tables were found.\n");
	return(NOTOK);
    }
    /* reallocate to add null terminator */
    maxtvidx++;
    *tv = (struct TVTBL *) realloc((char *) *tv, 
				   (unsigned) (maxtvidx*sizeof(struct TVTBL)));
    if (*tv == (struct TVTBL *) NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to realloc for NULL terminato.r\n");
	return(NOTOK);
    }
    (*tv)[maxtvidx-1].tvtbl = NULL;
    (*tv)[maxtvidx-1].startaddr = (unsigned long) -1;
    (*tv)[maxtvidx-1].endaddr = (unsigned long) -1;
    /* all done */
    return(OK);
}

/* function to read in transfer vector tables from object files */
/*ARGSUSED*/
int n3bgettv(oplist, tv)
char **oplist;
struct TVTBL **tv;
{
    /* no tv section for unix products */
TRACE();
    *tv = NULL;
    /* all done */
    return(OK);
}

/* function to read in transfer vector tables from object files */
int n3btvgettv(oplist, tv)
char **oplist;
struct TVTBL **tv;
{
    /* all done */
TRACE();
    return(gettv(oplist, tv));
}

/* function to read in transfer vector tables from object files */
int mctvgettv(oplist, tv)
char **oplist;
struct TVTBL **tv;
{
    /* all done */
TRACE();
    return(gettv(oplist, tv));
}

/* function to read in transfer vector tables from object files */
int Gettv(magic, oplist, tv)
unsigned short magic;
char **oplist;
struct TVTBL **tv;
{
    /* call correct function depending on magic */
TRACE();
    switch (magic)
    {
    case N3BMAGIC:
	/* 3b unix products */
	return(n3bgettv(oplist, tv));
    case NTVMAGIC:
	/* 3b kernels products */
	return(n3btvgettv(oplist, tv));
    case MC68TVMAGIC:
	/* cmp and sm products */
	return(mctvgettv(oplist, tv));
    default:
	/* garbage */
	PRINTLNNO(stderr);
	fprintf(stderr, "unexpected magic value.\n");
	break;
    }
    /* oops */
    return(NOTOK);
}

/* function to read value from symbol table */
int symndx2nv(infd, fhdr, symndx, nvalue)
FILE *infd;
struct filehdr *fhdr;
long symndx, *nvalue;
{
    long offset;
    struct syment symhdr;

    /* seek to location of symbol entry */
TRACE();
    offset = fhdr->f_symptr + symndx*SYMESZ;
    if (fseek(infd, offset, 0) != 0) return(NOTOK);
    /* read in symbol table entry */
    if (fread((char *) &symhdr, SYMESZ, ONEITEM, infd) != ONEITEM) return(NOTOK);
    /* return nvalue */
    *nvalue = symhdr.n_value;
    return(OK);
}

/* function to get a symbol name given an index */
int symndx2nm(infd, fhdr, symndx, symhdr, auxhdr, string)
FILE *infd;
struct filehdr *fhdr;
long symndx;
struct syment *symhdr;
union auxent *auxhdr;
char *string;
{
    char *pstr;
    long offset;
#if 0
    int fseek(), fread();
#endif

    /* seek to location of symbol entry */
TRACE();
    offset = fhdr->f_symptr + symndx*SYMESZ;
    if (fseek(infd, offset, 0) != 0) return(NOTOK);
    /* read in symbol table entry */
    if (fread((char *) symhdr, SYMESZ, ONEITEM, infd) != ONEITEM) return(NOTOK);
    /* read in auxiliary table entry */
    if (fread((char *) auxhdr, AUXESZ, ONEITEM, infd) != ONEITEM) return(NOTOK);
    /* check if name is in the string table */
    if (symhdr->_n._n_n._n_zeroes == 0L)
    {
	/* read string table for symbol name */
	offset = fhdr->f_symptr + fhdr->f_nsyms*SYMESZ + symhdr->_n._n_n._n_offset;
	if (fseek(infd, offset, 0) != 0) return(NOTOK);
	/* since the string is NULL terminated */
	for (pstr = string; ((*pstr = getc(infd)) != NULL); pstr++)
	{
	    /* do nothing */
	}
	*pstr = NULL;
	/* done, return symbol name */
	return(OK);
    }
    /* name is in symhdr structure */
    strncpy(string, symhdr->_n._n_name, SYMNMLEN);
    string[SYMNMLEN] = NULL;
    /* return ok */
    return(OK);
}

/* function to get the data for a line number */
int lnno2data(infd, fhdr, shdr, lnno, func, symhdr, auxhdr)
FILE *infd;
struct filehdr *fhdr;
struct scnhdr *shdr;
unsigned short lnno;
char *func;
struct syment *symhdr;
union auxent *auxhdr;
{
    struct lineno lnhdr;

    /* check range of line number */
TRACE();
    if (lnno < 1 || lnno > shdr->s_nlnno) return(NOTOK);
    /* seek to location of line number data */
    if (fseek(infd, shdr->s_lnnoptr + (lnno - 1)*LINESZ, 0) != 0) return(NOTOK);
    /* read line number data */
    if (fread((char *) &lnhdr, LINESZ, ONEITEM, infd) != ONEITEM) return(NOTOK);
    /* check if this is a new function */
    if (lnhdr.l_lnno == 0)
    {
	/* get function name */
	if (symndx2nm(infd, fhdr, 
		      lnhdr.l_addr.l_symndx, symhdr, auxhdr, func) != OK) return(NOTOK);
    }
    else
    {
	/* get function name */
	for ( ; lnhdr.l_lnno != 0 && lnno > 0; lnno--)
	{
	    /* seek to location of line number data */
	    if (fseek(infd, 
		      shdr->s_lnnoptr + (lnno - 1)*LINESZ, 0) != 0) return(NOTOK);
	    /* read line number data */
	    if (fread((char *) &lnhdr, LINESZ, 
	 	      ONEITEM, infd) != ONEITEM) return(NOTOK);
	}
	/* was function name found */
	if (lnhdr.l_lnno != 0) return(NOTOK);
	/* get function name */
	if (symndx2nm(infd, fhdr, 
		      lnhdr.l_addr.l_symndx, symhdr, auxhdr, func) != OK) return(NOTOK);
    }
    /* return with data */
    return(OK);
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
TRACE();
    if (lnno < 1 || lnno > shdr->s_nlnno) return(NOTOK);
    /* seek to location of line number data */
    if (fseek(infd, shdr->s_lnnoptr + (lnno - 1)*LINESZ, 0) != 0) return(NOTOK);
    /* read line number data */
    if (fread((char *) &lnhdr, LINESZ, ONEITEM, infd) != ONEITEM) return(NOTOK);
    /* check if this is a new function */
    if (lnhdr.l_lnno == 0)
    {
	/* new function, get address */
	if (symndx2nv(infd, fhdr, lnhdr.l_addr.l_symndx, lineaddr) != OK)
	{
	    return(NOTOK);
	}
    }
    else
    {
	/* get line number address */
	*lineaddr = lnhdr.l_addr.l_paddr;
    }
    /* return with address */
    return(OK);
}

/* find the line number for an address */
int getaddrfn(addr, fhdr, shdr, symhdr, auxhdr, func, infd)
unsigned long addr;
struct filehdr *fhdr;
struct scnhdr *shdr;
struct syment *symhdr;
union auxent *auxhdr;
char *func;
FILE *infd;
{
    int found;
    unsigned short lnno;
    unsigned short lowlnno, highlnno;
    unsigned short olowlnno, ohighlnno;
    unsigned long lnnoaddr;

    /* check if section has any line numbers */
TRACE();
    if (shdr->s_lnnoptr == 0 || shdr->s_nlnno == 0) return(NOTFOUND);
    /* set up indices for binary search */
    found = NOTOK;
    lowlnno = 1;
    olowlnno = 0;
    highlnno = shdr->s_nlnno;
    ohighlnno = shdr->s_nlnno + 1;
    lnno = (lowlnno + highlnno)/2;
    /* get address of middle line number */
    if (lnno2addr(infd, fhdr, shdr, lnno, &lnnoaddr) != OK) return(NOTOK);
    /* start a binary search to find address */
    while (((lowlnno > olowlnno) || (highlnno < ohighlnno)) && 
	   (highlnno >= lowlnno) && (! found))
    {
	/* do comparisons for search */
	if (lnnoaddr == addr)
	{
	    /* found an exact match */
	    found = OK;
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
	    if (lnno2addr(infd, fhdr, shdr, lnno, &lnnoaddr) != OK) return(NOTOK);
	}
    }
    /*
     * at this point, we either have an exact match, or
     * we have bounded the address from the top and the bottom.
     */
    if ( ! found)
    {
	/* an exact match was not found, find the line number */
	if (lnno2addr(infd, fhdr, shdr, lnno, &lnnoaddr) != OK) return(NOTOK);
	/* check if lower or greater */
	if (lnnoaddr > addr)
	{	
	    do {
		/* get an upper lower bound */
		lnno = lnno - 1;
		/* an exact match was not found, find the line number */
		if (lnno2addr(infd, fhdr, 
		 	      shdr, lnno, &lnnoaddr) != OK) return(NOTOK);
	    } while (lnnoaddr > addr && lnno > 0);
	    /* check if a line number was found */
	    if (lnno <= 0) return(NOTFOUND);
	}
	else if (lnnoaddr < addr)
	{
	    do {
		/* get an upper lower bound */
		lnno = lnno + 1;
		/* an exact match was not found, find the line number */
		if (lnno2addr(infd, fhdr, 
		 	      shdr, lnno, &lnnoaddr) != OK) return(NOTOK);
	    } while (lnnoaddr < addr && lnno <= shdr->s_nlnno);
	    /* always one higher, so decrement */
	    lnno = lnno - 1;
	    /* check if a line number was found */
	    if (lnno >= shdr->s_nlnno) return(NOTFOUND);
	}
    }
    else if (lnno < 1 || lnno > shdr->s_nlnno)
    {
	/* no line number was found */
	return(NOTFOUND);
    }
    /* get data for line number */
    if (lnno2data(infd, fhdr, shdr, lnno, func, symhdr, auxhdr) != OK) return(NOTOK);
    /* return with any data */
    return(OK);
}

/* function to convert a tv address to function address */
int n3btv2fn(tvaddress, fname, address, text, textsize, oplist, tvtbl)
unsigned long tvaddress;
char *fname;
unsigned long *address;
char **text;
long *textsize;
char **oplist;
struct TVTBL *tvtbl;
{
    FILE *infd;
    int status, sec, offset;
    struct filehdr *fhdr;
    struct scnhdr *shdr;
    struct syment symhdr;
    union auxent auxhdr;
    union {
	unsigned short dummy[2];
	unsigned long address;
    } uaddress;

    /* check arguments */
TRACE();
    if (fname == (char *) NULL || address == (unsigned long *) NULL || 
	text == (char **) NULL || oplist == (char **) NULL || 
	tvtbl == (struct TVTBL *) NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr,"bad arguments were passed.\n");
	return(NOTOK);
    }
    fhdr = NULL;
    shdr = NULL;
    *text = NULL;
    /* try convert the tv to a function */
    for ( ; tvtbl->tvtbl != NULL; tvtbl++)
    {
	/* check ranges in table */
	if (tvtbl->startaddr <= tvaddress && tvaddress <= tvtbl->endaddr)
	{
	    /* found the right range */
	    break;
	}
    }
    /* check if the tv range was found */
    if (tvtbl->tvtbl == NULL)
    {
	/* the tv was not found, return a null address */
	*address = 0;
	return(NOTFOUND);
    }
    /* we found a match */
    offset = (tvaddress - tvtbl->startaddr)/sizeof(struct N3BTVENTRY);
    uaddress.dummy[0] = ((struct N3BTVENTRY *) (tvtbl->tvtbl))[offset].address[0];
    uaddress.dummy[1] = ((struct N3BTVENTRY *) (tvtbl->tvtbl))[offset].address[1];
    *address = uaddress.address;
    /* now get name of function for address */
    for ( ; *oplist != NULL; oplist++)
    {
	/* open file for read */
	if ((infd = fopen(*oplist, "r")) == (FILE *) NULL)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to open file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* read in file header */
	status = readdata(infd, 0L, FILHSZ, (char *)&fhdr);
	if (status != OK)
	{
	    PRINTLNNO(stderr);
	    (void) fclose(infd);
	    fprintf(stderr, "unable to read file header for file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* read in section headers */
	status = readdata(infd, (long) (FILHSZ + fhdr->f_opthdr), 
			  fhdr->f_nscns*SCNHSZ, (char *)&shdr);
	if (status != OK)
	{
	    PRINTLNNO(stderr);
	    (void) fclose(infd);
	    fprintf(stderr, "unable to read section headers for file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* search for section which contains the address */
	for (sec = 0; sec < fhdr->f_nscns; sec++)
	{
	    /* check if the section contains the address */
	    if ((shdr[sec].s_paddr <= *address) && 
		(*address <= (shdr[sec].s_paddr + shdr[sec].s_size)))
	    {
		/* found the section which contains address */
		switch (getaddrfn(*address, fhdr, &shdr[sec], 
				  &symhdr, &auxhdr, fname, infd))
		{
		case OK:
		    /* a function name was found, get text and textsize */
		    offset = shdr[symhdr.n_scnum-1].s_scnptr +
			     symhdr.n_value - shdr[symhdr.n_scnum-1].s_paddr;
		    status = readdata(infd, offset, auxhdr.x_sym.x_misc.x_fsize, text);
		    if (status != OK)
		    {
			PRINTLNNO(stderr);
			(void) fclose(infd);
			fprintf(stderr, 
			       "unable to read in text data in file %s.\n", 
			       *oplist);
			return(NOTOK);
		    }
		    *textsize = auxhdr.x_sym.x_misc.x_fsize;
		    /* release resources */
		    (void) fclose(infd);
		    free(fhdr);
		    free(shdr);
		    /* return with info */
		    return(OK);
		case NOTFOUND:
		    /* function name was not found */
		    continue;
		default:
		    PRINTLNNO(stderr);
		    (void) fclose(infd);
		    free(fhdr);
		    free(shdr);
		    fprintf(stderr, "unable to convert an address to a function.\n");
		    return(NOTFOUND);
		}
	    }
	}
	/* all done with this object file */
	(void) fclose(infd);
    }
    /* release all memory */
    free(fhdr);
    free(shdr);
    /* not found */
    return(NOTFOUND);
}

/* function to convert a tv address to function address */
int mctv2fn(tvaddress, fname, address, text, textsize, oplist, tvtbl)
unsigned long tvaddress;
char *fname;
unsigned long *address;
char **text;
long *textsize;
char **oplist;
struct TVTBL *tvtbl;
{
    FILE *infd;
    int status, sec, offset;
    struct filehdr *fhdr;
    struct scnhdr *shdr;
    struct syment symhdr;
    union auxent auxhdr;
    union {
	unsigned short dummy[2];
	unsigned long address;
    } uaddress;

    /* check arguments */
TRACE();
    if (fname == (char *) NULL || address == (unsigned long *) NULL || 
	text == (char **) NULL || oplist == (char **) NULL || 
	tvtbl == (struct TVTBL *) NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr,"bad arguments were passed.\n");
	return(NOTOK);
    }
    fhdr = NULL;
    shdr = NULL;
    *text = NULL;
    /* try convert the tv to a function */
    for ( ; tvtbl->tvtbl != NULL; tvtbl++)
    {
	/* check ranges in table */
	if (tvtbl->startaddr <= tvaddress && tvaddress <= tvtbl->endaddr)
	{
	    /* found the right range */
	    break;
	}
    }
    /* check if the tv range was found */
    if (tvtbl->tvtbl == NULL)
    {
	/* the tv was not found, return a null address */
	*address = 0;
	return(NOTFOUND);
    }
    /* we found a match */
    offset = (tvaddress - tvtbl->startaddr)/sizeof(struct MCTVENTRY);
    uaddress.dummy[0] = ((struct MCTVENTRY *) (tvtbl->tvtbl))[offset].address[0];
    uaddress.dummy[1] = ((struct MCTVENTRY *) (tvtbl->tvtbl))[offset].address[1];
    *address = uaddress.address;
    /* now get name of function for address */
    for ( ; *oplist != NULL; oplist++)
    {
	/* open file for read */
	if ((infd = fopen(*oplist, "r")) == (FILE *) NULL)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to open file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* read in file header */
	status = readdata(infd, 0L, FILHSZ, (char *)&fhdr);
	if (status != OK)
	{
	    PRINTLNNO(stderr);
	    (void) fclose(infd);
	    fprintf(stderr, "unable to read file header for file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* read in section headers */
	status = readdata(infd, (long) (FILHSZ + fhdr->f_opthdr), 
			  fhdr->f_nscns*SCNHSZ, (char *)&shdr);
	if (status != OK)
	{
	    PRINTLNNO(stderr);
	    (void) fclose(infd);
	    fprintf(stderr, "unable to read section headers for file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* search for section which contains the address */
	for (sec = 0; sec < fhdr->f_nscns; sec++)
	{
	    /* check if the section contains the address */
	    if ((shdr[sec].s_paddr <= *address) && 
		(*address <= (shdr[sec].s_paddr + shdr[sec].s_size)))
	    {
		/* found the section which contains address */
		switch (getaddrfn(*address, fhdr, &shdr[sec], 
				  &symhdr, &auxhdr, fname, infd))
		{
		case OK:
		    /* a function name was found, get text and textsize */
		    offset = shdr[symhdr.n_scnum-1].s_scnptr +
			     symhdr.n_value - shdr[symhdr.n_scnum-1].s_paddr;
		    status = readdata(infd, offset, auxhdr.x_sym.x_misc.x_fsize, text);
		    if (status != OK)
		    {
			PRINTLNNO(stderr);
			(void) fclose(infd);
			fprintf(stderr, 
			       "unable to read in text data in file %s.\n", 
			       *oplist);
			return(NOTOK);
		    }
		    *textsize = auxhdr.x_sym.x_misc.x_fsize;
		    /* release resources */
		    (void) fclose(infd);
		    free(fhdr);
		    free(shdr);
		    /* return with info */
		    return(OK);
		case NOTFOUND:
		    /* function name was not found */
		    continue;
		default:
		    PRINTLNNO(stderr);
		    (void) fclose(infd);
		    free(fhdr);
		    free(shdr);
		    fprintf(stderr, "unable to convert an address to a function.\n");
		    return(NOTFOUND);
		}
	    }
	}
	/* all done with this object file */
	(void) fclose(infd);
    }
    /* release all memory */
    free(fhdr);
    free(shdr);
    /* not found */
    return(NOTFOUND);
}

/* read hash table for a given tv */
int readtbl(tvaddress, node)
unsigned long tvaddress;
struct TREE **node;
{
    int bin;
    struct HASHTBL *next;

    /* get the bin */
TRACE();
    bin = tvaddress%DIVISOR;
    /* check if entry exists */
    for (next = hashtbl[bin]; next != NULL; next = next->next)
    {
	/* check for match */
	if (next->node->tvaddress == tvaddress)
	{
	    /* return, we found the node */
	    *node = next->node;
	    return(OK);
	}
    }
    /* the node was not found */
    return(NOTFOUND);
}

/* insert a new function into hash table */
int inserttbl(node)
struct TREE *node;
{
    int bin;
    struct HASHTBL *next;

    /* get new bin */
TRACE();
    bin = node->tvaddress%DIVISOR;
    /* check if entry already exists */
    for (next = hashtbl[bin]; next != NULL; next = next->next)
    {
	/* check for match */
	if (next->node->tvaddress == node->tvaddress)
	{
	    /* return, we found the node */
	    return(DUPLICATE);
	}
    }
    /* bin is either empty or no match was found, store data */
    next = (struct HASHTBL *)Zmalloc(sizeof(struct HASHTBL));
    if (next == NULL)
    {
	usage("", NOTOK);
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to malloc.\n");
	return(NOTOK);
    }
    next->node = node;
    next->next = hashtbl[bin];
    hashtbl[bin] = next;
    /* all done */
    return(OK);
}

/* function to convert an address to function address */
/*ARGSUSED*/
int n3baddr2fn(tvaddress, fname, address, text, textsize, oplist, tvtbl)
unsigned long tvaddress;
char *fname;
unsigned long *address;
char **text;
long *textsize;
char **oplist;
struct TVTBL *tvtbl;
{
    FILE *infd;
    int status, sec, offset;
    struct filehdr *fhdr;
    struct scnhdr *shdr;
    struct syment symhdr;
    union auxent auxhdr;

    /* check arguments */
TRACE();
    if (fname == (char *) NULL || address == (unsigned long *) NULL || 
	text == (char **) NULL || oplist == (char **) NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr,"bad arguments were passed.\n");
	return(NOTOK);
    }
    fhdr = NULL;
    shdr = NULL;
    *text = NULL;
    /* copy tvaddress to address since unix has no tv */
    *address = tvaddress;
    /* now get name of function for address */
    for ( ; *oplist != NULL; oplist++)
    {
	/* open file for read */
	if ((infd = fopen(*oplist, "r")) == (FILE *) NULL)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to open file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* read in file header */
	status = readdata(infd, 0L, FILHSZ, (char *)&fhdr);
	if (status != OK)
	{
	    PRINTLNNO(stderr);
	    (void) fclose(infd);
	    fprintf(stderr, "unable to read file header for file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* read in section headers */
	status = readdata(infd, (long) (FILHSZ + fhdr->f_opthdr), 
			  fhdr->f_nscns*SCNHSZ, (char *)&shdr);
	if (status != OK)
	{
	    PRINTLNNO(stderr);
	    (void) fclose(infd);
	    fprintf(stderr, "unable to read section headers for file %s.\n", *oplist);
	    return(NOTOK);
	}
	/* search for section which contains the address */
	for (sec = 0; sec < fhdr->f_nscns; sec++)
	{
	    /* check if the section contains the address */
	    if ((shdr[sec].s_paddr <= *address) && 
		(*address <= (shdr[sec].s_paddr + shdr[sec].s_size)))
	    {
		/* found the section which contains address */
		switch (getaddrfn(*address, fhdr, &shdr[sec], 
				  &symhdr, &auxhdr, fname, infd))
		{
		case OK:
		    /* a function name was found, get text and textsize */
		    offset = shdr[symhdr.n_scnum-1].s_scnptr +
			     symhdr.n_value - shdr[symhdr.n_scnum-1].s_paddr;
		    status = readdata(infd, offset, auxhdr.x_sym.x_misc.x_fsize, text);
		    if (status != OK)
		    {
			PRINTLNNO(stderr);
			(void) fclose(infd);
			fprintf(stderr, 
			       "unable to read in text data in file %s.\n", 
			       *oplist);
			return(NOTOK);
		    }
		    *textsize = auxhdr.x_sym.x_misc.x_fsize;
		    /* release resources */
		    (void) fclose(infd);
		    free(fhdr);
		    free(shdr);
		    /* return with info */
		    return(OK);
		case NOTFOUND:
		    /* function name was not found */
		    continue;
		default:
		    PRINTLNNO(stderr);
		    (void) fclose(infd);
		    free(fhdr);
		    free(shdr);
		    fprintf(stderr, "unable to convert an address to a function.\n");
		    return(NOTFOUND);
		}
	    }
	}
	/* all done with this object file */
	(void) fclose(infd);
    }
    /* release all memory */
    free(fhdr);
    free(shdr);
    /* not found */
    return(NOTFOUND);
}

/* trace a function call */
int n3bftrc(tvaddress, oplist, tvtbl, root, maxlevel, level)
unsigned long tvaddress;
char **oplist;
struct TVTBL *tvtbl;
struct TREE **root;
int maxlevel, level;
{
    char *text, fname[FNAMESZ];
    int pc;
    unsigned long tvaddr, address;
    struct TREE **sibling, *child;
    long textsize;

    /* check arguments */
TRACE();
    if (root == NULL || oplist == NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "bad arguments were passed.\n");
	return(NOTOK);
    }
    text = NULL;
    /* check if this is the end */
    if (level > maxlevel)
    {
	/* null out the pointer, and return */
	*root = NULL;
	return(OK);
    }
    /* check if the function has already been traced */
    switch (readtbl(tvaddress, &child))
    {
    case NOTFOUND:
	/* insert new entry, and trace */
#ifdef COUNT
	new++;
#endif
	break;
    case OK:
	/* entry is old, use existing data */
#ifdef COUNT
	old++;
#endif
	(*root) = (struct TREE *) Zmalloc(sizeof(struct TREE));
	if (*root == NULL)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to malloc a new node for function %s.\n", 
		    child->function);
	    return(NOTOK);
	}
	(*root)->sibling = (*root)->child = NULL;
	(*root)->tvaddress = child->tvaddress;
	(*root)->address = child->address;
	(void) strcpy((*root)->function, child->function);
	/* loop thru existing data, and trace */
	for (sibling = &(*root)->child, child = child->child;
	     child != NULL; 
	     child = child->sibling)
	{
	    switch (n3bftrc(child->tvaddress, oplist, 
			     tvtbl, sibling, maxlevel, level + 1))
	    {
	    case NOTFOUND:
		continue;
	    case OK:
		/* go on to next case */
		sibling = &(*sibling)->sibling;
		continue;
	    default:
		PRINTLNNO(stderr);
		fprintf(stderr, 
		       "failure during reading of existing data for function %s.\n", 
			(*root)->function);
		free((char *) (*root));
		*root = NULL;
		return(NOTOK);
	    }
	}
	/* all done */
	return(OK);
    default:
	PRINTLNNO(stderr);
	fprintf(stderr, "failure reading hash table.\n");
	return(NOTOK);
    }
    /* get function information */
    switch (n3baddr2fn(tvaddress, fname, &address, &text, &textsize, oplist, tvtbl))
    {
    case OK:
	break;
    case NOTFOUND:
	*root = NULL;
	return(NOTFOUND);
    default:
	PRINTLNNO(stderr);
	fprintf(stderr, "converting tv to function failed.\n");
	return(NOTOK);
    }
    /* allocate a new node and store data */
    *root = (struct TREE *) Zmalloc(sizeof(struct TREE));
    if (*root == NULL)
    {
	usage("", NOTOK);
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to malloc a new node for function %s.\n", fname);
	return(NOTOK);
    }
    (*root)->sibling = (*root)->child = NULL;
    strcpy((*root)->function, fname);
    (*root)->tvaddress = tvaddress;
    (*root)->address = address;
    /* scan thru text looking for other functions */
    for (sibling = &(*root)->child, pc = 0; pc < textsize; )
    {
	if ((text[pc++] == N3BCALL1) && ((text[pc++] & 0xf) == N3BCALL2))
	{
	    /* get function address from text */
	    tvaddr = ((unsigned long) text[pc++]) << 24;
	    tvaddr += ((unsigned long) text[pc++]) << 16;
	    tvaddr += ((unsigned long) text[pc++]) << 8;
	    tvaddr += ((unsigned long) text[pc++]);
	    /* remove lowest order nibble, it's garbage */
	    tvaddr = (tvaddr >> 4);
#ifdef DEBUG
	    fprintf(stderr, "%s'%d: tvaddr = 0x%lx.\n", __FILE__, __LINE__, tvaddr);
#endif
	    switch (n3bftrc(tvaddr, oplist, tvtbl, sibling, maxlevel, level + 1))
	    {
	    case NOTFOUND:
		continue;
	    case OK:
		/* go on to next case */
#ifdef DEBUG
	    	fprintf(stderr, "%s'%d: function = %s.\n", 
		 	__FILE__, __LINE__, (*sibling)->function);
#endif
		sibling = &(*sibling)->sibling;
		continue;
	    default:
		PRINTLNNO(stderr);
		free((char *) (*root));
		*root = NULL;
		fprintf(stderr, 
		       "failure during screening of text for function %s.\n", 
			fname);
		return(NOTOK);
	    }
	}
    }
    /* insert new entry into hash table, only if a trace was done */
    if ((level + 1) <= maxlevel)
    {
	switch (inserttbl(*root))
	{
	/*
	 * if a function calls itself recurrsively, either directly or
	 * indirectly, then it is possible for the readtbl() to return
	 * a NOTFOUND, then for the inserttbl() to fail with DUPLICATE
	 * since the recurrsive call could have entered the entry into
	 * the hash table.
	 */
	case OK:
	case DUPLICATE:
	    break;
	default:
	    PRINTLNNO(stderr);
	    fprintf(stderr, "failure in inserting function data into hash table.\n");
	    return(NOTOK);
	}
    }
    /* release text */
    free(text);
    /* all done */
    return(OK);
}

/* trace a function call */
int n3btvftrc(tvaddress, oplist, tvtbl, root, maxlevel, level)
unsigned long tvaddress;
char **oplist;
struct TVTBL *tvtbl;
struct TREE **root;
int maxlevel, level;
{
    char *text, fname[FNAMESZ];
    int pc;
    unsigned long tvaddr, address;
    struct TREE **sibling, *child;
    long textsize;

    /* check arguments */
TRACE();
    if (tvtbl == NULL || root == NULL || oplist == NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "bad arguments were passed.\n");
	return(NOTOK);
    }
    text = NULL;
    /* check if this is the end */
    if (level > maxlevel)
    {
	/* null out the pointer, and return */
	*root = NULL;
	return(OK);
    }
    /* check if the function has already been traced */
    switch (readtbl(tvaddress, &child))
    {
    case NOTFOUND:
	/* insert new entry, and trace */
#ifdef COUNT
	new++;
#endif
	break;
    case OK:
	/* entry is old, use existing data */
#ifdef COUNT
	old++;
#endif
	(*root) = (struct TREE *) Zmalloc(sizeof(struct TREE));
	if (*root == NULL)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to malloc a new node for function %s.\n", 
		    child->function);
	    return(NOTOK);
	}
	(*root)->sibling = (*root)->child = NULL;
	(*root)->tvaddress = child->tvaddress;
	(*root)->address = child->address;
	(void) strcpy((*root)->function, child->function);
	/* loop thru existing data, and trace */
	for (sibling = &(*root)->child, child = child->child;
	     child != NULL; 
	     child = child->sibling)
	{
	    switch (n3btvftrc(child->tvaddress, oplist, 
			     tvtbl, sibling, maxlevel, level + 1))
	    {
	    case NOTFOUND:
		continue;
	    case OK:
		/* go on to next case */
		sibling = &(*sibling)->sibling;
		continue;
	    default:
		PRINTLNNO(stderr);
		fprintf(stderr, 
		       "failure during reading of existing data for function %s.\n", 
			(*root)->function);
		free((char *) (*root));
		*root = NULL;
		return(NOTOK);
	    }
	}
	/* all done */
	return(OK);
    default:
	PRINTLNNO(stderr);
	fprintf(stderr, "failure reading hash table.\n");
	return(NOTOK);
    }
    /* get function information */
    switch (n3btv2fn(tvaddress, fname, &address, &text, &textsize, oplist, tvtbl))
    {
    case OK:
	break;
    case NOTFOUND:
	*root = NULL;
	return(NOTFOUND);
    default:
	PRINTLNNO(stderr);
	fprintf(stderr, "converting tv to function failed.\n");
	return(NOTOK);
    }
    /* allocate a new node and store data */
    *root = (struct TREE *) Zmalloc(sizeof(struct TREE));
    if (*root == NULL)
    {
	usage("", NOTOK);
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to malloc a new node for function %s.\n", fname);
	return(NOTOK);
    }
    (*root)->sibling = (*root)->child = NULL;
    strcpy((*root)->function, fname);
    (*root)->tvaddress = tvaddress;
    (*root)->address = address;
    /* scan thru text looking for other functions */
    for (sibling = &(*root)->child, pc = 0; pc < textsize; )
    {
	if (text[pc++] == N3BTVCALL)
	{
	    /* get tv vector from text */
	    tvaddr = ((unsigned long) N3BTVCALL) << 16;
	    tvaddr += ((unsigned long) text[pc++]) << 8;
	    tvaddr += ((unsigned long) text[pc++]);
	    switch (n3btvftrc(tvaddr, oplist, tvtbl, sibling, maxlevel, level + 1))
	    {
	    case NOTFOUND:
		continue;
	    case OK:
		/* go on to next case */
		sibling = &(*sibling)->sibling;
		continue;
	    default:
		PRINTLNNO(stderr);
		free((char *) (*root));
		*root = NULL;
		fprintf(stderr, 
		       "failure during screening of text for function %s.\n", 
			fname);
		return(NOTOK);
	    }
	}
    }
    /* insert new entry into hash table, only if a trace was done */
    if ((level + 1) <= maxlevel)
    {
	switch (inserttbl(*root))
	{
	/*
	 * if a function calls itself recurrsively, either directly or
	 * indirectly, then it is possible for the readtbl() to return
	 * a NOTFOUND, then for the inserttbl() to fail with DUPLICATE
	 * since the recurrsive call could have entered the entry into
	 * the hash table.
	 */
	case OK:
	case DUPLICATE:
	    break;
	default:
	    PRINTLNNO(stderr);
	    fprintf(stderr, "failure in inserting function data into hash table.\n");
	    return(NOTOK);
	}
    }
    /* release text */
    free(text);
    /* all done */
    return(OK);
}

/* trace a function call */
int mctvftrc(tvaddress, oplist, tvtbl, root, maxlevel, level)
unsigned long tvaddress;
char **oplist;
struct TVTBL *tvtbl;
struct TREE **root;
int maxlevel, level;
{
    char *text, fname[FNAMESZ];
    int pc;
    unsigned long tvaddr, address;
    struct TREE **sibling, *child;
    long textsize;

    /* check arguments */
TRACE();
    if (tvtbl == NULL || root == NULL || oplist == NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "bad arguments were passed.\n");
	return(NOTOK);
    }
    text = NULL;
    /* check if this is the end */
    if (level > maxlevel)
    {
	/* null out the pointer, and return */
	*root = NULL;
	return(OK);
    }
    /* check if the function has already been traced */
    switch (readtbl(tvaddress, &child))
    {
    case NOTFOUND:
	/* insert new entry, and trace */
#ifdef COUNT
	new++;
#endif
	break;
    case OK:
	/* entry is old, use existing data */
#ifdef COUNT
	old++;
#endif
	(*root) = (struct TREE *) Zmalloc(sizeof(struct TREE));
	if (*root == NULL)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to malloc a new node for function %s.\n",
	 	    child->function);
	    return(NOTOK);
	}
	(*root)->sibling = (*root)->child = NULL;
	(*root)->tvaddress = child->tvaddress;
	(*root)->address = child->address;
	(void) strcpy((*root)->function, child->function);
	/* loop thru existing data, and trace */
	for (sibling = &(*root)->child, child = child->child;
	     child != NULL; 
	     child = child->sibling)
	{
	    switch (mctvftrc(child->tvaddress, oplist, 
			     tvtbl, sibling, maxlevel, level + 1))
	    {
	    case NOTFOUND:
		continue;
	    case OK:
		/* go on to next case */
		sibling = &(*sibling)->sibling;
		continue;
	    default:
		PRINTLNNO(stderr);
		fprintf(stderr, 
		       "failure during reading of existing data for function %s.\n", 
			(*root)->function);
		free((char *) (*root));
		*root = NULL;
		return(NOTOK);
	    }
	}
	/* all done */
	return(OK);
    default:
	PRINTLNNO(stderr);
	fprintf(stderr, "failure reading hash table.\n");
	return(NOTOK);
    }
    /* get function information */
    switch (mctv2fn(tvaddress, fname, &address, &text, &textsize, oplist, tvtbl))
    {
    case OK:
	break;
    case NOTFOUND:
	*root = NULL;
	return(NOTFOUND);
    default:
	PRINTLNNO(stderr);
	fprintf(stderr, "converting tv to function.\n");
	return(NOTOK);
    }
    /* allocate a new node and store data */
    *root = (struct TREE *) Zmalloc(sizeof(struct TREE));
    if (*root == NULL)
    {
	usage("", NOTOK);
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to malloc a new node for function %s.\n", fname);
	return(NOTOK);
    }
    (*root)->sibling = (*root)->child = NULL;
    strcpy((*root)->function, fname);
    (*root)->tvaddress = tvaddress;
    (*root)->address = address;
    /* scan thru text looking for other functions */
    for (sibling = &(*root)->child, pc = 0; pc < textsize; )
    {
	if (text[pc++] == MCTVJSR1 && text[pc++] == MCTVJSR2)
	{
	    /* get tv vector from text */
	    tvaddr = ((unsigned long) text[pc++]) << 24;
	    tvaddr += ((unsigned long) text[pc++]) << 16;
	    tvaddr += ((unsigned long) text[pc++]) << 8;
	    tvaddr += ((unsigned long) text[pc++]);
	    switch (mctvftrc(tvaddr, oplist, tvtbl, sibling, maxlevel, level + 1))
	    {
	    case NOTFOUND:
		continue;
	    case OK:
		/* go on to next case */
		sibling = &(*sibling)->sibling;
		continue;
	    default:
		PRINTLNNO(stderr);
		free((char *) (*root));
		*root = NULL;
		fprintf(stderr, 
		       "failure during screening of text for function %s.\n", 
			fname);
		return(NOTOK);
	    }
	}
    }
    /* insert new entry into hash table, only if a trace was done */
    if ((level + 1) <= maxlevel)
    {
	switch (inserttbl(*root))
	{
	/*
	 * if a function calls itself recurrsively, either directly or
	 * indirectly, then it is possible for the readtbl() to return
	 * a NOTFOUND, then for the inserttbl() to fail with DUPLICATE
	 * since the recurrsive call could have entered the entry into
	 * the hash table.
	 */
	case OK:
	case DUPLICATE:
	    break;
	default:
	    PRINTLNNO(stderr);
	    fprintf(stderr, "failure in inserting function data into hash table.\n");
	    return(NOTOK);
	}
    }
    /* release text */
    free(text);
    /* all done */
    return(OK);
}

/* trace a function call */
int Ftrc(magic, tvaddress, oplist, tvtbl, root, maxlevel, level)
unsigned short magic;
unsigned long tvaddress;
char **oplist;
struct TVTBL *tvtbl;
struct TREE **root;
int maxlevel, level;
{
    /* switch on magic */
TRACE();
    switch (magic)
    {
    case N3BMAGIC:
	/* 3b unix products */
	return(n3bftrc(tvaddress, oplist, tvtbl, root, maxlevel, level));
    case NTVMAGIC:
	/* 3b kernel products */
	return(n3btvftrc(tvaddress, oplist, tvtbl, root, maxlevel, level));
    case MC68TVMAGIC:
	/* cmp and sm products */
	return(mctvftrc(tvaddress, oplist, tvtbl, root, maxlevel, level));
    default:
	/* garbage */
	PRINTLNNO(stderr);
	fprintf(stderr, "unexpected magic.\n");
	break;
    }
    /* oops */
    return(NOTOK);
}

/* insert an entry into uniqueness table */
int insuniq(utbl, tvaddress)
struct UNIQUETBL **utbl;
unsigned long tvaddress;
{
    int bin;
    struct UNIQUETBL *next;

    /* get new bin */
TRACE();
    bin = tvaddress%DIVISOR;
    /* check if entry already exists */
    for (next = utbl[bin]; next != NULL; next = next->next)
    {
	if (next->tvaddress == tvaddress) return(DUPLICATE);
    }
    /* bin is either empty or no match was found, store data */
    next = (struct UNIQUETBL *)Zmalloc(sizeof(struct UNIQUETBL));
    if (next == NULL)
    {
	usage("", NOTOK);
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to malloc.\n");
	return(NOTOK);
    }
    next->tvaddress = tvaddress;
    next->next = utbl[bin];
    utbl[bin] = next;
    /* all done */
    return(OK);
}

/* print a function trace */
int prtftrc(unique, node, level)
int unique;
struct TREE *node;
int level;
{
    int dummy;
    struct TREE *next;
    struct UNIQUETBL **utbl;

    /* initialize locals */
    next = NULL;
    utbl = NULL;

    /* check if end of this branch */
TRACE();
    if (node == NULL) return(OK);

    /* print name or bps */
    if (((unique & WEPRINT) != WEPRINT) && ((unique & WRPRINT) != WRPRINT))
    {
	/* print name only */
	for (dummy = 1; dummy < level; dummy++)
	{
	    fprintf(stdout, "    ");
	}
	fprintf(stdout, "%s", node->function);
	if ((unique & TVADDRESS) == TVADDRESS)
	{
	    fprintf(stdout," tvaddr=%lx", node->tvaddress);
	}
	if ((unique & ADDRESS) == ADDRESS)
	{
	    fprintf(stdout," addr=%lx", node->address);
	}
	fprintf(stdout, "\n");
    }
    else
    {
	/* print entry and return breakpoints */
	if ((unique & WEPRINT) == WEPRINT)
	{
	    fprintf(stdout, "wep(%s)\n", node->function);
	}
	if ((unique & WRPRINT) == WRPRINT)
	{
	    fprintf(stdout, "wrp(%s)\n", node->function);
	}
    }
    /* allocate uniqueness table, if requested */
    if ((unique & UNIQUE) == UNIQUE)
    {
	utbl = (struct UNIQUETBL **)Zmalloc(NBINS*sizeof(struct UNIQUETBL *));
    }

    /* cycle thru the children and print data */
    for (next = node->child; next != NULL; next = next->sibling)
    {
	if (((unique & UNIQUE) == UNIQUE) && (utbl != NULL))
	{
	    switch (insuniq(utbl, next->tvaddress))
	    {
	    case OK:
		/* new entry, print it */
		break;
	    case DUPLICATE:
		/* skip this entry, it has already been printed */
		continue;
	    default:
		/* error inserting entry */
		if (utbl != NULL) free((char *)utbl);
		PRINTLNNO(stderr);
		fprintf(stderr, "error when inserting a node into unique table.\n");
		return(NOTOK);
	    }
	}

	/* print function name */
	if (prtftrc(unique, next, level + 1) != OK)
	{
	    if (utbl != NULL) free((char *)utbl);
	    PRINTLNNO(stderr);
	    fprintf(stderr, 
		   "unable to print tree for function %s.\n", 
		    node->function);
	    return(NOTOK);
	}
    }
    if (utbl != NULL) free((char *)utbl);

    /* all done */
    return(OK);
}

/* given a function name, return all info about that function */
int getfinfo(fname, op, oplist, fhdr, shdr, symhdr, auxhdr)
char *fname;
char **op;
char **oplist;
struct filehdr *fhdr;
struct scnhdr *shdr;
struct syment *symhdr;
union auxent *auxhdr;
{
    FILE *infd;
    struct scnhdr *shdrlist;
    struct lineno lnhdr;
    int lnno, sec, status;
    long offset, fileloc;
    char *pstr, string[BUFSIZ];

    /* search path list for a function name */
TRACE();
    for (op = oplist; *op != NULL; op++)
    {
	/* open object file */
	if ((infd = fopen(*op, "r")) == NULL)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to open file %s.\n", *op);
	    return(NOTOK);
	}
	/* read in file and section headers */
	if (fseek(infd, 0, 0) != 0)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to seek to file header for file %s.\n", *op);
	    (void) fclose(infd);
	    return(NOTOK);
	}
	if (fread((char *)fhdr, FILHSZ, ONEITEM, infd) != ONEITEM)
	{
	    usage("", NOTOK);
	    PRINTLNNO(stderr);
	    fprintf(stderr, "unable to read file header for file %s.\n", *op);
	    (void) fclose(infd);
	    return(NOTOK);
	}
	/* read in file and section headers */
	shdrlist = NULL;
	status = readdata(infd, 
			 (long) (FILHSZ + fhdr->f_opthdr),
			 fhdr->f_nscns*SCNHSZ,
			 (char *)&shdrlist);
	if (status != OK)
	{
	    PRINTLNNO(stderr);
	    (void) fclose(infd);
	    fprintf(stderr, "unable to read section headers for file %s.\n", *op);
	    return(NOTOK);
	}
	/* search section headers for a match */
	for (sec = 0; sec < fhdr->f_nscns; sec++)
	{
	    /* check if section has any line number data */
	    if (shdrlist[sec].s_lnnoptr == 0 || 
		shdrlist[sec].s_nlnno == 0) continue;
	    /* seek to line number data */
	    if (fseek(infd, shdrlist[sec].s_lnnoptr, 0) != 0)
	    {
		PRINTLNNO(stderr);
		(void) fclose(infd);
		fprintf(stderr, 
		       "unable to seek to line number data in file %s.\n", 
			*op);
		return(NOTOK);
	    }
	    /* locate the function name */
	    for (lnno = 1; lnno <= shdrlist[sec].s_nlnno; lnno++)
	    {
		/* read in line number data */
		if (fread((char *)&lnhdr, LINESZ, ONEITEM, infd) != ONEITEM)
		{
		    PRINTLNNO(stderr);
		    (void) fclose(infd);
		    fprintf(stderr, 
		           "unable to read line number data in file %s.\n", 
			    *op);
		    return(NOTOK);
		}
		/* check if a new function was found */
		if (lnhdr.l_lnno == 0)
		{
		    /* get symbol information and symbol name */
		    fileloc = ftell(infd);
		    offset = fhdr->f_symptr + lnhdr.l_addr.l_symndx*SYMESZ,
		    status = fseek(infd, offset, 0);
		    if (status != 0)
		    {
			PRINTLNNO(stderr);
			(void) fclose(infd);
			fprintf(stderr, 
			       "unable to seek to a symbol entry in file %s.\n", 
			       *op);
			return(NOTOK);
		    }
		    if (fread((char *)symhdr, SYMESZ, ONEITEM, infd) != ONEITEM)
		    {
			PRINTLNNO(stderr);
			(void) fclose(infd);
			fprintf(stderr, 
			       "unable to read a symbol entry in file %s.\n", 
			       *op);
			return(NOTOK);
		    }
		    if (symhdr->_n._n_n._n_zeroes == 0L)
		    {
			/* read in name from string table */
			offset = fhdr->f_symptr + 
				 fhdr->f_nsyms*SYMESZ + 
				 symhdr->_n._n_n._n_offset;
			if (fseek(infd, offset, 0) != 0)
			{
			    PRINTLNNO(stderr);
			    (void) fclose(infd);
			    fprintf(stderr, 
				   "unable to seek to a symbol entry in file %s.\n", 
				   *op);
			    return(NOTOK);
			}
			for (pstr = string; ((*pstr = getc(infd)) != NULL); pstr++)
			{
			    /* do nothing */
			}
			*pstr = NULL;
			/* return to auxiliary entry file location */
			offset = fhdr->f_symptr + (lnhdr.l_addr.l_symndx+1)*SYMESZ,
			status = fseek(infd, offset, 0);
			if (status != 0)
			{
			    PRINTLNNO(stderr);
			    (void) fclose(infd);
			    fprintf(stderr, 
				   "unable to seek to a auxiliary entry in file %s.\n", 
				   *op);
			    return(NOTOK);
			}
		    }
		    else
		    {
			/* get name from symbol table */
			strncpy(string, symhdr->_n._n_name, SYMNMLEN);
			string[SYMNMLEN] = NULL;
		    }
		    /* check if the name matches */
		    if (strcmp(fname, string) == 0)
		    {
			/* found a match, save data */
			*shdr = shdrlist[sec];
			free(shdrlist);
			/* a function was found, read auxiliary entry */
			if (fread((char *)auxhdr, AUXESZ, ONEITEM, infd) != ONEITEM)
			{
			    PRINTLNNO(stderr);
			    (void) fclose(infd);
			    fprintf(stderr, 
				   "unable to read auxiliary entry in file %s.\n", 
				   *op);
			    return(NOTOK);
    			}
			/* all done, return with match */
			(void) fclose(infd);
			return(OK);
		    }
		    /* return to previous location*/
		    if (fseek(infd, fileloc, 0) != 0)
		    {
			PRINTLNNO(stderr);
			(void) fclose(infd);
			fprintf(stderr, 
			       "unable to seek to a line number entry in file %s.\n", 
			       *op);
			return(NOTOK);
		    }
		}
	    }
	}
	/* close file */
	(void) fclose(infd);
    }
    /* function was not found */
    free(shdrlist);
    return(NOTFOUND);
}

/* convert a function address to its tv */
int n3btvaddr2tv(addr, tvaddr, tvtbl)
unsigned long addr;
unsigned long *tvaddr;
struct TVTBL *tvtbl;
{
    struct N3BTVENTRY *tvtblend, *tventry;
    union {
	unsigned short dummy[2];
	unsigned long address;
    } address;

    /*  check arguments */
TRACE();
    if (tvaddr == NULL || tvtbl == NULL || addr == 0)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "bad arguments were passed.\n");
	return(NOTOK);
    }
    /* convert address to tv */
    for ( ; tvtbl->tvtbl != NULL; tvtbl++)
    {
	tvtblend = ((struct N3BTVENTRY *) tvtbl->tvtbl) + 
		  (tvtbl->endaddr - tvtbl->startaddr)/sizeof(struct N3BTVENTRY);
	for (tventry = (struct N3BTVENTRY *) tvtbl->tvtbl; 
	     tventry < tvtblend; 
	     tventry++)
	{
	    address.dummy[0] = tventry->address[0];
	    address.dummy[1] = tventry->address[1];
	    if (addr == address.address)
	    {
		*tvaddr = tvtbl->startaddr + 
			 (tventry - (struct N3BTVENTRY *)tvtbl->tvtbl)*sizeof(struct N3BTVENTRY);
		return(OK);
	    }
	}
    }
    return(NOTFOUND);
}

/* convert a function address to its tv */
int mctvaddr2tv(addr, tvaddr, tvtbl)
unsigned long addr;
unsigned long *tvaddr;
struct TVTBL *tvtbl;
{
    struct MCTVENTRY *tvtblend, *tventry;
    union {
	unsigned short dummy[2];
	unsigned long address;
    } address;

    /*  check arguments */
TRACE();
    if (tvaddr == NULL || tvtbl == NULL || addr == 0)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "bad arguments were passed.\n");
	return(NOTOK);
    }
    /* convert address to tv */
    for ( ; tvtbl->tvtbl != NULL; tvtbl++)
    {
	tvtblend = ((struct MCTVENTRY *) tvtbl->tvtbl) + 
		  (tvtbl->endaddr - tvtbl->startaddr)/sizeof(struct MCTVENTRY);
	for (tventry = (struct MCTVENTRY *) tvtbl->tvtbl; 
	     tventry < tvtblend; 
	     tventry++)
	{
	    address.dummy[0] = tventry->address[0];
	    address.dummy[1] = tventry->address[1];
	    if (addr == address.address)
	    {
		*tvaddr = tvtbl->startaddr + 
			 (tventry - (struct MCTVENTRY *)tvtbl->tvtbl)*sizeof(struct MCTVENTRY);
		return(OK);
	    }
	}
    }
    return(NOTFOUND);
}

/* convert a function name to a tv */
/*ARGSUSED*/
int n3bfn2tv(fname, tvaddr, addr, oplist, tvtbl)
char *fname;
unsigned long *tvaddr;
unsigned long *addr;
char **oplist;
struct TVTBL *tvtbl;
{
    struct filehdr fhdr;
    struct scnhdr shdr;
    struct syment symhdr;
    union auxent auxhdr;
    char *op;
    int status;

    /* get function data */
TRACE();
    status = getfinfo(fname, &op, oplist, &fhdr, &shdr, &symhdr, &auxhdr);
    switch (status)
    {
    case OK:
	break;
    case NOTFOUND:
	return(NOTFOUND);
    default:
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to get info for function %s.\n", fname);
	return(NOTOK);
    }
    /* get addresses, remember unix has no tv addresses */
    *addr = symhdr.n_value;
    *tvaddr = symhdr.n_value;
    /* all done */
    return(OK);
}

/* convert a function name to a tv */
int n3btvfn2tv(fname, tvaddr, addr, oplist, tvtbl)
char *fname;
unsigned long *tvaddr;
unsigned long *addr;
char **oplist;
struct TVTBL *tvtbl;
{
    struct filehdr fhdr;
    struct scnhdr shdr;
    struct syment symhdr;
    union auxent auxhdr;
    char *op;
    int status;

    /* get function data */
TRACE();
    status = getfinfo(fname, &op, oplist, &fhdr, &shdr, &symhdr, &auxhdr);
    switch (status)
    {
    case OK:
	break;
    case NOTFOUND:
	return(NOTFOUND);
    default:
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to get info for function %s.\n", fname);
	return(NOTOK);
    }
    /* get addresses */
    *addr = symhdr.n_value;
    if (n3btvaddr2tv(*addr, tvaddr, tvtbl) != OK)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to get tv for function %s.\n", fname);
	return(NOTOK);
    }
    /* all done */
    return(OK);
}

/* convert a function name to a tv */
int mctvfn2tv(fname, tvaddr, addr, oplist, tvtbl)
char *fname;
unsigned long *tvaddr;
unsigned long *addr;
char **oplist;
struct TVTBL *tvtbl;
{
    struct filehdr fhdr;
    struct scnhdr shdr;
    struct syment symhdr;
    union auxent auxhdr;
    char *op;
    int status;

    /* get function data */
TRACE();
    status = getfinfo(fname, &op, oplist, &fhdr, &shdr, &symhdr, &auxhdr);
    switch (status)
    {
    case OK:
	break;
    case NOTFOUND:
	return(NOTFOUND);
    default:
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to get info for function %s.\n", fname);
	return(NOTOK);
    }
    /* get addresses */
    *addr = symhdr.n_value;
    if (mctvaddr2tv(*addr, tvaddr, tvtbl) != OK)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to get tv for function %s.\n", fname);
	return(NOTOK);
    }
    /* all done */
    return(OK);
}

/* convert a function name to a tv */
int Fn2tv(magic, fname, tvaddr, addr, oplist, tvtbl)
unsigned short magic;
char *fname;
unsigned long *tvaddr;
unsigned long *addr;
char **oplist;
struct TVTBL *tvtbl;
{
    /* switch on magic */
TRACE();
    switch (magic)
    {
    case N3BMAGIC:
	/* 3b unix products */
	return(n3bfn2tv(fname, tvaddr, addr, oplist, tvtbl));
    case NTVMAGIC:
	/* 3b kernel products */
	return(n3btvfn2tv(fname, tvaddr, addr, oplist, tvtbl));
    case MC68TVMAGIC:
	/* cmp and sm products */
	return(mctvfn2tv(fname, tvaddr, addr, oplist, tvtbl));
    default:
	/* garbage */
	PRINTLNNO(stderr);
	fprintf(stderr, "unexpected magic.\n");
	break;
    }
    /* oops */
    return(NOTOK);
}

/* combine object and path lists into one list */
/*ARGSUSED*/
int objectlist(magic, argc, argv, olist, plist, oplist)
unsigned short magic;
int argc;
char **argv;
char **olist;
char **plist;
char ***oplist;
{
    FILE *infd;
    char **o, **p, **op;
    char buf[BUFSIZ];
    char errbuf[BUFSIZ];
    struct stat statbuf;
    struct filehdr fhdr;

    /* allocate initial list */
TRACE();
    *oplist = (char **) Zmalloc(NUMPATHS*sizeof(char *));
    if (*oplist == (char **) NULL)
    {
	PRINTLNNO(stderr);
	usage("unable to Zmalloc memory for object/path array.", NOTOK);
	errno = 0;
	return(NOTOK);
    }
    /* generate a list of object paths */
    for (op = *oplist, p = plist; *p != (char *) NULL; p++)
    {
	for (o = olist; *o != (char *) NULL; o++)
	{
	    /* combine path and object name */
	    if (strlen(*p) > 0)
	    {
		sprintf(buf, "%s/%s", *p, *o);
	    }
	    else
	    {
		sprintf(buf, "%s", *o);
	    }
	    /* check if file exists */
	    if (stat(buf, &statbuf) != 0)
	    {
		switch (errno)
		{
		case ENOTDIR:
		case ENOENT:
		case EACCES:
		    continue;
		default:
		    PRINTLNNO(stderr);
		    (void) sprintf(errbuf, "unable to stat file %s.", buf);
		    usage(errbuf, NOTOK);
		    errno = 0;
	 	    return(NOTOK);
		}
	    }
	    /* open COFF file and read file header */
	    if ((infd = fopen(buf, "r")) == (FILE *) NULL)
	    {
		/* unable to open file, exit */
		PRINTLNNO(stderr);
		(void) sprintf(errbuf, "unable to open file %s.", buf);
		usage(errbuf, NOTOK);
		errno = 0;
		return(NOTOK);
	    }
	    if (fread((char *) &fhdr, FILHSZ, ONEITEM, infd) != ONEITEM)
	    {
		/* unable to read header, exit */
		PRINTLNNO(stderr);
		(void) fclose(infd);
		(void) sprintf(errbuf, "unable to read COFF header for file %s.", buf);
		usage(errbuf, NOTOK);
		errno = 0;
		return(NOTOK);
	    }
	    /* check magic with transfer vectors */
	    if (fhdr.f_magic != magic)
	    {
		/* bad magic, skip file and continue */
		(void) fprintf(stderr, 
			      "file %s does not have the correct magic. skipping it.\n", 
			       buf);
		(void) fclose(infd);
		continue;
	    }
	    /* check if file is stripped */
	    if ((fhdr.f_flags & F_LSYMS) == F_LSYMS)
	    {
		/* file is stripped, close file and continue */
		(void) fprintf(stderr, 
			      "file %s is stripped of symbol information. skipping it.\n", buf);
		(void) fclose(infd);
		continue;
	    }
	    /* check for object files, only bound products are allowed */
	    if ((fhdr.f_flags & F_EXEC) != F_EXEC)
	    {
		/* file has unresolved symbols */
		(void) fprintf(stderr, 
			      "file %s has unresolved symbols. skipping it.\n", buf);
		(void) fclose(infd);
		continue;
	    }
	    
	    /* close file, all done for now */
	    (void) fclose(infd);
	    /* store file name */
	    *op = (char *) Zmalloc((strlen(buf) + 1)*sizeof(char));
	    if (*op == (char *) NULL)
	    {
		PRINTLNNO(stderr);
		(void) sprintf(errbuf, "unable to malloc memory for %s.", buf);
		usage(errbuf, NOTOK);
		errno = 0;
		return(NOTOK);
	    }
	    /* store path in object/path array */
	    strcpy(*op++, buf);
	}
    }
    /* check if any object file were found */
    if (*oplist == op)
    {
	errno = 0;
	PRINTLNNO(stderr);
	usage("no products (not object files) were found.", NOTOK);
	return(NOTOK);
    }
    /* indicate the end of the list */
    *op = (char *) NULL;
    /* all done */
    return(OK);
}

/* generate a list of path using viewpath */
char **getvpath(vlist)
char **vlist;
{
    char **newlist, *tmp, *getcwd(), **v, *pwd;

    /* check arguments */
TRACE();
    if (vlist == NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "bad arguments.\n");
	return(NULL);
    }
    /* get present working directory */
    if ((pwd = getcwd((char *)NULL, PATHSZ)) == (char *) NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to get present working directory.\n");
	return(NULL);
    }
    /* find relative directory */
    for (v = vlist; *v != (char *) NULL; v++)
    {
	/* check for a match */
	if (strncmp(*v, pwd, strlen(*v)) == 0)
	{
	    if (pwd[strlen(*v)] != NULL)
	    {
		/* skip a slash */
		pwd += strlen(*v) + 1;
	    }
	    else
	    {
		/* return a NULL length string */
		pwd += strlen(*v);
	    }
	    break;
	}
    }
    /* check if a relative path was found */
    if (*v == NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to generate a relative directory.\n");
	return(NULL);
    }
    /* check for a null length relative directory */
    if (strlen(*v) == 0) return(vlist);
    /* now combine viewpath with relative path */
    if ((tmp = malloc(BUFSIZ*sizeof(char))) == NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to malloc a relative directory.\n");
	return(NULL);
    }
    for (*tmp = NULL, v = vlist; *v != (char *) NULL; v++)
    {
	strcat(tmp, *v);
	strcat(tmp, "/");
	strcat(tmp, pwd);
	strcat(tmp, ":");
#ifdef DEBUG
	fprintf(stderr,"list = %s, path = %s\n", tmp, *v);
#endif
    }
    tmp[strlen(tmp) - 1] = NULL;
    /* generate an argument vector */
    if ((newlist = getlist(tmp, DELIMITERS)) == (char **) NULL)
    {
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to get viewpath list.\n");
	return(NULL);
    }
    /* all done */
    return(newlist);
}

/* get all files in present directory */
int allfiles(magic, oplist)
unsigned short magic;
char ***oplist;
{
    static char *cwd = ".";
    DIR *dirp;
    struct dirent *dinfo;
    struct stat statbuf;
    struct filehdr fhdr;
    char fname[BUFSIZ+1], **list;
    FILE *ffd;
    int howMany;

    /* open present directory for read */
TRACE();
    if ((dirp = opendir(cwd)) == NULL)
    {
	usage("", NOTOK);
	PRINTLNNO(stderr);
	fprintf(stderr, "unable to open current working directory.\n");
	return(NOTOK);
    }

    /* scan present directory for files with the correct magic */
    howMany = 0;
    *oplist = list = NULL;
    while ((dinfo = readdir(dirp)) != NULL)
    {
	/* ignore present and parent directories */
        if ((strcmp(dinfo->d_name, ".") == 0) ||
            (strcmp(dinfo->d_name, "..") == 0))
	{
	    continue;
	}

	/* get name of file */
	strcpy(fname, dinfo->d_name);

	/* make sure the file is a regular file */
	if (stat(fname, &statbuf) != 0) continue;
	if ((statbuf.st_mode & S_IFMT) != S_IFREG) continue;

	/* check minimum file size, remember .text, .data, .bss sections */
	if (statbuf.st_size < (FILHSZ + 3*SCNHSZ)) continue;

	/* we have a possible candidate, read file header */
	if ((ffd = fopen(fname, "r")) == NULL) continue;
	if (fread((char *) &fhdr, FILHSZ, ONEITEM, ffd) != ONEITEM)
	{
	    /* close file and skip it */
	    fclose(ffd);
	    continue;
	}

	/* close file */
	(void) fclose(ffd);

	/* checking file */
	fprintf(stdout, "checking file ... %s.\n", fname);

	/* check file contents */
	if ((fhdr.f_magic != magic) ||
	   ((fhdr.f_flags & F_LSYMS) == F_LSYMS) ||
	   ((fhdr.f_flags & F_EXEC) != F_EXEC))
	{
	    /* file is either stripped, wrong magic, or not bound */
	    continue;
	}

	/* new file found */
	fprintf(stdout, "found object file ... %s.\n", fname);

	/* we have a good file, save file name in list */
	howMany++;
	if (howMany == 1)
	{
	    /* first file to save */
	    list = (char **) malloc(sizeof(char *));
	    if (list == NULL)
	    {
		PRINTLNNO(stderr);
		usage("unable to malloc space.", NOTOK);
		errno = 0;
		closedir(dirp);
		return(NOTOK);
	    }
	}
	else
	{
	    /* new file name */
	    list = (char **) realloc(list, howMany*sizeof(char *));
	    if (list == NULL)
	    {
		PRINTLNNO(stderr);
		usage("unable to realloc space.", NOTOK);
		errno = 0;
		closedir(dirp);
		return(NOTOK);
	    }
	}

	/* save file name */
	list[howMany-1] = (char *) malloc(strlen(fname)+1);
	if (list[howMany-1] == NULL)
	{
	    PRINTLNNO(stderr);
	    usage("unable to realloc space.", NOTOK);
	    errno = 0;
    	    closedir(dirp);
	    return(NOTOK);
	}

	/* save file name in list */
	strcpy(list[howMany-1], fname);
    }

    /* close directory */
    closedir(dirp);

    /* check if any files were found */
    if (howMany == 0)
    {
	PRINTLNNO(stderr);
	usage("no files were found in present directory.", NOTOK);
	return(NOTOK);
    }

    /* add null terminator */
    howMany++;
    list = (char **) realloc(list, howMany*sizeof(char *));
    if (list == NULL)
    {
	PRINTLNNO(stderr);
	usage("unable to realloc space.", NOTOK);
	return(NOTOK);
    }
    list[howMany-1] = '\0';

    *oplist = list;
    /* all done */
    return(OK);
}

/* main entry point */
main(argc, argv)
int argc;
char **argv;
{
    char *pc, **vlist, **olist, **plist, **oplist, *nullplist[2], *getenv();
    int arg, level, maxlevel, c, getopt(), atoi();
    struct TREE *root;
    struct TVTBL *tvtbl;
    struct HASHTBL *treetbl, *tmpnode;
    unsigned long tvaddr, addr;
    int options;
    unsigned short magic;

    /* initialize some variables */
TRACE();
    vlist = oplist = plist = olist = (char **) NULL;
    tvtbl = (struct TVTBL *) NULL;
    treetbl = tmpnode = (struct HASHTBL *) NULL;
    root = (struct TREE *) NULL;
    maxlevel = 2;
    level = 1;
    options = 0;
    magic = MC68TVMAGIC;
    memset((char *)hashtbl, 0, NBINS*sizeof(struct HASHTBL *));
#ifdef COUNT
    new = old = 0;
#endif
    /* get command line arguments */
    while ((c = getopt(argc, argv, "?vertauAl:o:p:m:")) != EOF)
    {
	/* check options */
	switch (c)
	{
	case 'l':
	    /* get maximum level to trace */
	    for (pc = optarg; *pc != NULL; pc++)
	    {
		if ( ! ('0' <= *pc && *pc <= '9'))
		{
		    PRINTLNNO(stderr);
		    usage("maxlevel must be an integer >= 2.", OK);
		    exit(2);
		}
	    }
	    maxlevel = atoi(optarg);
	    if (maxlevel <= 1)
	    {
		PRINTLNNO(stderr);
		usage("maxlevel must be >= 2.", OK);
	 	exit(2);
	    }
	    else if (maxlevel >= 10)
	    {
		fprintf(stdout, "Maxlevel >= 10 ??? Ok, but it\'s probably going to bomb !!!\n");
	    }
	    break;
	case 'o':
	    /* get names of COFF file */
	    if ((olist = getlist(optarg, DELIMITERS)) == (char **) NULL)
	    {
		PRINTLNNO(stderr);
		usage("unable to get object list.", OK);
		exit(2);
	    }
	    break;
	case 'm':
	    /* given a magic */
	    if (strcmp(optarg, N3B) == 0)
	    {
		magic = N3BMAGIC;
	    }
	    else if (strcmp(optarg, N3BTV) == 0)
	    {
		magic = NTVMAGIC;
	    }
	    else if (strcmp(optarg, MCTV) == 0)
	    {
		magic = MC68TVMAGIC;
	    }
	    else
	    {
		PRINTLNNO(stderr);
		usage("unsupported magic value.", OK);
		exit(2);
	    }
	    break;
	case 'p':
	    /* get path for objs, if any */
	    if (vlist != NULL)
	    {
		PRINTLNNO(stderr);
		usage("use either -v or -p, but not both.", OK);
		exit(2);
	    }
	    if ((plist = getlist(optarg, DELIMITERS)) == (char **) NULL)
	    {
		PRINTLNNO(stderr);
		usage("unable to get path list.", OK);
		exit(2);
	    }
	    break;
	case 'e':
	    /* print when enter print breakpoints */
	    options |= WEPRINT;
	    break;
	case 'r':
	    /* print when return print breakpoints */
	    options |= WRPRINT;
	    break;
	case 't':
	    /* print tvs */
	    options |= TVADDRESS;
	    break;
	case 'a':
	    /* print addresses */
	    options |= ADDRESS;
	    break;
	case 'u':
	    /* print only unique functions */
	    options |= UNIQUE;
	    break;
	case 'A':
	    /* scan all files in present directory */
	    options |= ALLFILES;
	    break;
	case 'v':
	    /* get viewpath from environment */
	    if (plist != NULL)
	    {
		PRINTLNNO(stderr);
		usage("use either -v or -p, but not both.", OK);
		exit(2);
	    }
	    if ((vlist = getlist(getenv("VPATH"), DELIMITERS)) == (char **)NULL)
	    {
		PRINTLNNO(stderr);
		usage("unable to get viewpath, VPATH.", OK);
		exit(2);
	    }
	    break;
	case '?':
	    /* print usage msg */
	    usage("", OK);
	    exit(0);
	default:
	    /* bad option in command line */
	    PRINTLNNO(stderr);
	    usage("bad option in command line.", OK);
	    exit(2);
	}
    }
    /* see if any paths were given */
    if ((options & ALLFILES) != ALLFILES)
    {
	/* check if a path list was given */
	if (vlist != (char **) NULL)
	{
	    if ((plist = getvpath(vlist)) == (char **) NULL)
	    {
		PRINTLNNO(stderr);
		usage("unable to generate a path list from viewpath.", OK);
		exit(2);
	    }
	}
	else if (plist == (char **) NULL)
	{
	    /* no path was given, check variable LNNOPATH */
	    if ((plist = getlist(getenv("LNNOPATH"), 
					DELIMITERS)) == (char **)NULL)
	    {
		/* default to no path */
		plist = nullplist;
		nullplist[0] = "";
		nullplist[1] = (char *) NULL;
	    }
	}
	/* check if an object list was given */
	if (olist == (char **) NULL)
	{
	    /* no objects were given, check variable LNNOOBJS */
	    if ((olist = getlist(getenv("LNNOOBJS"), 
					DELIMITERS)) == (char **)NULL)
	    {
		PRINTLNNO(stderr);
		usage("no object files were given.", OK);
		exit(2);
	    }
	}
	/* combine object list and path list into one */
	if (objectlist(magic, argc, argv, olist, plist, &oplist) != OK)
	{
	    PRINTLNNO(stderr);
	    errno = 0;
	    usage("unable to combine object and path lists.", OK);
	    exit(2);
	}
    }
    else if (allfiles(magic, &oplist) != OK)
    {
	/* generate list from the files in the present directory */
	PRINTLNNO(stderr);
	errno = 0;
	usage("unable to generate a list of files to scan.", OK);
	exit(2);
    }
    /* check if any functions were given */
    if (optind >= argc)
    {
	PRINTLNNO(stderr);
	errno = 0;
	usage("no functions were given.", OK);
	exit(2);
    }
    /* read in transfer vector tables */
    if (Gettv(magic, oplist, &tvtbl) != OK)
    {
	PRINTLNNO(stderr);
	usage("unable to read in transfer vector table.", NOTOK);
	exit(2);
    }
    /* cycle thru all function names and find the tree structure */
    for (arg = optind; arg < argc; arg++)
    {
	/* convert function name to a tv to start off trace */
	switch (Fn2tv(magic, argv[arg], &tvaddr, &addr, oplist, tvtbl))
	{
	case OK:
	    break;
	case NOTFOUND:
	    fprintf(stdout, "function %s was not found.\n", argv[arg]);
	    continue;
	default:
	    PRINTLNNO(stderr);
	    fprintf(stderr, "error in converting function %s to a tv.\n", argv[arg]);
	    exit(2);
	}
	/* trace function tree */
	switch (Ftrc(magic, tvaddr, oplist, tvtbl, &root, maxlevel, level))
	{
	case OK:
	    /* save function trace */
	    tmpnode = (struct HASHTBL *) Zmalloc(sizeof(struct HASHTBL));
	    if (tmpnode == NULL)
	    {
		usage("", NOTOK);
		PRINTLNNO(stderr);
		fprintf(stderr, 
		       "unable to store function trace for function %s.\n", 
			argv[arg]);
		exit(2);
	    }
	    tmpnode->node = root;
	    tmpnode->next = treetbl;
	    treetbl = tmpnode;
	    break;
	case NOTFOUND:
	    fprintf(stdout, "function %s was not found.\n", argv[arg]);
	    continue;
	default:
	    PRINTLNNO(stderr);
	    fprintf(stderr, "error in tracing the tree for function %s.\n", argv[arg]);
	    exit(2);
	}
	/* print function tree */
	if (prtftrc(options, root, level) != OK)
	{
	    PRINTLNNO(stderr);
	    fprintf(stderr, "error in printing the tree for function %s.\n", argv[arg]);
	    exit(2);
	}
    }
#ifdef COUNT
    fprintf(stdout, "total counts = %d, new = %d, old = %d\n", new+old, new, old);
    fprintf(stdout, "percent new = %f, percent old = %f\n",
	    100.0*(float)new/((float)(new+old)), 100.0*(float)old/((float)(new+old)));
#endif
    /* all done */
    exit(0);
}
