/*
 * ropscan scans a rop file for spps, audits, asserts and rc/v scripts.
 * it prints out a summary msg for all asserts, etc. that it finds.
 * the original version of this program was an awk script called
 * roplog.
 */

/* system headers */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <values.h>

/* externs for regcmp and regex */
extern char *regcmp();
extern char *regex();
extern *_loc1;
extern char *optarg;
extern int optind, opterr;

/* exit macros */
#define EXIT(status) { \
	if (fdout != (FILE *) NULLPTR) (void) fclose(fdout); \
	exit(status); \
}

/* error macro */
#define USAGE(string) usage(string, __FILE__, (int) __LINE__)

/* AM assert REs */
#define AMASRT1 "INIT +AM +LVL *= *([A-Z]+)$0 +([A-Za-z0-9_-]+)$1 +([A-Za-z0-9_= -]+)$2 +EVENT *= *([0-9]+)$3 +([A-Za-z0-9_-]+)$4"
#define AMASRT2 "SW-ERR +FAIL-ADDR *= *(H'[A-Fa-f0-9]+)$0"
#define DEFCHKFAIL "DEF-CHK-FAIL *= *([0-9]+)$0"

/* SM assert REs */
#define SMASRT1 "INIT +SM *= *([0-9]+)$0,[01] +LVL *= *([A-Z,]+)$1 +OSDS.*EVENT *= *([0-9]+)$2"
#define SMASRT2 "([A-Za-z0-9_=-]+)$0"
#define SMASRT3 "([A-Z]+)$0-ERR +FAILING-ADDR *= *(H'[A-fa-f0-9]+)$1"

/* CMP assert REs */
#define CMPASRT1 "INIT +CMP *= *[01]-[01] +([A-Z]+)$0 +LVL *= *([A-Z,]+)$1 +EVENT *= *([0-9]+)$2"
#define CMPASRT2 "([A-Za-z0-9_=-]+)$0"
#define CMPASRT3 "FAILING-ADDR *= *(H'[A-fa-f0-9]+)$0"

/* AM craft assert REs */
#define AMCRAFT1 "REPT +MANUAL +ACTION +ASSERT *= *([0-9]+)$0[, ]+AM[, ]+ENV *= *OKP[, ]+EVENT *= *([0-9]+)$1"
#define AMCRAFT2 "([A-Za-z0-9_\.-]+)$0 +AT +LINE +([0-9]+)$1"

/* SM craft assert REs */
#define SMCRAFT1 "REPT +MANUAL +ACTION +ASSERT *= *([0-9]+)$0[, ]+SM *= *([0-9]+)$1[, ]+EVENT *= *([0-9]+)$2"
#define SMCRAFT2 "([A-Za-z0-9_\.-]+)$0 +AT +LINE +([0-9]+)$1"

/* SM audit RE */
#define SMAUDIT1 "AUD +SM *= *([0-9]+)$0 +([A-Za-z0-9_-]+)$1 +ERROR-CODE *= *([A-Za-z0-9_-]+)$2 +EVENT *= *([0-9]+)$3"

/* date RE */
#define ROPDATE "([0-9][0-9])$0-([01][0-9])$1-([0-3][0-9])$2 ([0-2][0-9])$3:([0-5][0-9])$4:([0-5][0-9])$5"

/* RCV msgs REs */
#define RCVSTART "RC/V +starting +([\.A-Za-z0-9_-]+)$0 +at"
#define RCVEND "RC/V +ending +([\.A-Za-z0-9_-]+)$0 +at"
#define RCVSTOP "RC/V +stopping +([\.A-Za-z0-9_-]+)$0 +at"
#define RCVMSG "RC/V +msg +(.+)$0"

/* CMP audit RE */
#define CMPAUDIT1 "AUD +CMP *= *[01]-[01] +([A-Z]+)$0 +([A-Za-z0-9_-]+)$1 +ERROR-CODE *= *([A-Za-z0-9_-]+)$2 +EVENT *= *([0-9]+)$3"

/* CMP craft assert REs */
#define CMPCRAFT1 "REPT +MANUAL +ACTION +ASSERT *= *([0-9]+)$0 +CMP *= *[01]-[01] +([A-Z]+)$1 +EVENT *= *([0-9]+)$2"
#define CMPCRAFT2 "([A-Za-z0-9_\.-]+)$0 +AT +LINE +([0-9]+)$1"

/* AM SPP RE */
#define AMSPPRE1 "INIT +AM +LVL *= *SPP +SUMMARY +ENV *= *([A-Za-z0-9_-]+)$0 +EVENT *= *([0-9]+)$1"
#define AMSPPRE2 "PROCESS *= *([A-Za-z0-9',_-]+)$0"

/* SM SPP RE */
#define SMSPPRE1 "INIT +SM *= *([0-9]+)$0 +LVL *= *SPP +SUMMARY +ENV *= *([A-Za-z0-9_-]+)$1 +EVENT *= *([0-9]+)$2"
#define SMSPPRE2 "PROCESS *= *([A-Za-z0-9',_-]+)$0"

/* CMP SPP RE */
#define CMPSPPRE1 "REPT +CMP *= *[01]-[01] +([A-Z]+)$0 +ENV *= *([A-Za-z0-9_-]+)$1 +LVL *= *SPP +EVENT *= *([0-9]+)$2"
#define CMPSPPRE2 "FAILING-ADDR *= *(H'[A-fa-f0-9]+)$0"
#define CMPSPPRE3 "PROCESS: *BG *= *([A-Za-z0-9',_-]+)$0"
#define CMPSPPRE4 "ERR *SRC *= *([A-Za-z0-9_-]+)$0"

/* pointers to compiles REs */
char *amasrt1, *amasrt2;
char *smasrt1, *smasrt2, *smasrt3;
char *amcraft1, *amcraft2;
char *smcraft1, *smcraft2;
char *smaudit1;
char *ropdate;
char *defchkfail;
char *rcvstart, *rcvend, *rcvstop, *rcvmsg;
char *cmpaudit1;
char *cmpcraft1, *cmpcraft2;
char *cmpasrt1, *cmpasrt2, *cmpasrt3;
char *amsppre1, *amsppre2;
char *smsppre1, *smsppre2;
char *cmpsppre1, *cmpsppre2, *cmpsppre3, *cmpsppre4;

/* other definitions */
#define USAGE1 "usage: ropscan [-B] [-o outfile] [-racup] "
#define USAGE2 "               [[-s MMDDhhmm] [-e MMDDhhm] | [-S byte] [-E byte]]"
#define USAGE3 "               [-h string1[,string2,...] [ropfile ...]"
#define ROPOK 1L
#define ROPNOTOK 0L
#define NONE 0L
#define TIME 1L
#define SIZE 2L
#define AM "193"
#define CMP "194"
#define NULLPTR 0L
#define NULLCHR ((char) 0L)
#define NULLSTR ""
#define STRSZ 256
#define NRECS 512
#define CNTRLY ((char) 031)
#define CR ((char) 012)
#define SPACE ((char) 040)
#define COMMA ((char) 054)
#define DELIMITER ","
#define DINGDONG ""
#define BHLGHT "*** HIGHLIGHT ***\n"
#define EHLGHT "\n*** HIGHLIGHT ***"
#define ROPPRINT 0L
#define ROPNOPRINT 1L
#define ROPDONE 2L
#define BADTSTAMP -1L

/* declarations for hashing table and high light list*/
#define DIVISOR 203L
#define NBINS 250
#define FOLDNUM MAXSHORT
enum LISTTYPE { NOLIST, HIGHLIGHT, CRAFT, ASSERT, AUDIT, INIT, SPP } ;
struct LIST {
    char *str1;
    char *str2;
    short howmany;
    enum LISTTYPE ltype;
    struct LIST *next;
} *table[NBINS];
struct HLIST {
    short howmany;
    char *list;
};
struct HIGHLIGHT {
    short hnum;
    struct HLIST *hlist;
} hlist;

/* declarations for tracking totals */
long Bflag, hflag, rflag, aflag, cflag, uflag, pflag;
long asrtcnt = 0L;
long cftcnt = 0L;
long audcnt = 0L;
long ascfcnt = 0L;
long sppcnt = 0L;
long misccnt = 0L;

/* get high light list */
long gethlist(chlist)
char *chlist;
{
    char *pstr;
    short slen, h;

    /* determine how many list elements, count commas */
    for (hlist.hnum = 0, pstr = chlist; *pstr != NULLCHR; pstr++)
    {
	if (*pstr == COMMA)
	{
	    hlist.hnum++;
	}
    }
    /* number of elements is one more than number of commas */
    hlist.hnum++;
    /* allocate array of pointers */
    hlist.hlist = (struct HLIST *) malloc((unsigned) hlist.hnum*sizeof(struct HLIST));
    if (hlist.hlist == (struct HLIST *) NULLPTR)
    {
	return(ROPNOTOK);
    }
    /* get list elements and store */
    for (pstr = chlist, slen = 0, h = 0; h < hlist.hnum; h++, pstr++)
    {
	/* find length of string between commas */
	if ((slen = strcspn(pstr, DELIMITER)) == 0) return(ROPNOTOK);
	/* store pointer to string */
	hlist.hlist[h].list = pstr;
	/* set counter to zero */
	hlist.hlist[h].howmany = 0;
	/* reset string pointer */
	pstr += slen;
	/* replace comma with a null */
	*pstr = NULLCHR;
    }
    /* done, return */
    return(ROPOK);
}

/* function to check for highlights */
void highlight(pchkstr, phstr)
char *pchkstr, *phstr;
{
    long storeit();
    char buf[BUFSIZ];
    short h;

    /* check if any highlighting is requested */
    if (! hflag) return;
    /* compare with list */
    *buf = NULLCHR;
    for (h = 0; h < hlist.hnum; h++)
    {
	/* check for match */
	if (strcmp(pchkstr, hlist.hlist[h].list) == 0)
	{
	    /* high light the string */
	    if (Bflag)
	    {
		sprintf(buf, "%s%s%s%s%s", DINGDONG, BHLGHT, phstr, EHLGHT, DINGDONG);
	    }
	    else
	    {
		sprintf(buf, "%s%s%s", BHLGHT, phstr, EHLGHT);
	    }
	    strcpy(phstr, buf);
	    /* store in list */
	    (void) storeit(pchkstr, NULLSTR, HIGHLIGHT);
	    /* increment the counter */
	    hlist.hlist[h].howmany++;
	    /* highlight is finished, return */
	    return;
	}
    }
    return;
}

/* function to count asserts, etc. */
void track(ltype)
enum LISTTYPE ltype;
{
    /* increment totals depending on type */
    switch(ltype)
    {
    case AUDIT:
	audcnt++;
	break;
    case ASSERT:
	asrtcnt++;
	break;
    case CRAFT:
	cftcnt++;
	break;
    case INIT:
	ascfcnt++;
	break;
    case SPP:
	sppcnt++;
	break;
    default:
	misccnt++;
	break;
    }
    return;
}

/* comparison function for sorting */
int compar(plist1, plist2)
struct LIST *plist1, *plist2;
{
    return((int) (plist2->howmany - plist1->howmany));
}

/* sort and print hash table results */
long printtable(fdout)
FILE *fdout;
{
    short iinit, i, initcnt;
    struct LIST *initlist, *plist;
    void qsort();
    int compar();

    /* allocate lists for sorting */
    initcnt = misccnt + sppcnt + audcnt + asrtcnt + cftcnt + ascfcnt;
    if (initcnt == 0)
    {
	fprintf(fdout, "No asserts, audits, spps, etc. were found\n");
	return(ROPOK);
    }
    initlist = (struct LIST *) malloc((unsigned) initcnt*sizeof(struct LIST));
    if (initlist == (struct LIST *) NULLPTR) return(ROPNOTOK);

    /* transfer results from the hash table to the lists */
    for (iinit = 0, i = 0; i < NBINS; i++)
    {
	/* check if table entry is empty */
	if (table[i] == (struct LIST *) NULLPTR) continue;
	/* store table entries in list */
	for (plist = table[i]; 
  	     plist != (struct LIST *) NULLPTR; plist = plist->next)
	{
	    initlist[iinit++] = *plist;
	}
    }

    /* sort above lists */
    qsort((char *)initlist, (unsigned) initcnt, sizeof(struct LIST), compar);

    /* print heading */
    fprintf(fdout, "\ntotal asserts and audits are ...\n");

    /* print all asserts results */
    fprintf(fdout, "\nall assert totals ...\n\n");
    for (iinit = 0; iinit < initcnt; iinit++)
    {
	if (initlist[iinit].ltype == INIT)
	{
	    fprintf(fdout, "\t%d %s %s\n", initlist[iinit].howmany, 
		    initlist[iinit].str1, initlist[iinit].str2);
	}
    }

    /* print asserts results */
    fprintf(fdout, "\nassert subtotals ...\n\n");
    for (iinit = 0; iinit < initcnt; iinit++)
    {
	if (initlist[iinit].ltype == ASSERT)
	{
	    fprintf(fdout, "\t%d %s %s\n", initlist[iinit].howmany, 
		    initlist[iinit].str1, initlist[iinit].str2);
	}
    }

    /* print craft asserts results */
    fprintf(fdout, "\ncraft assert subtotals ...\n\n");
    for (iinit = 0; iinit < initcnt; iinit++)
    {
	if (initlist[iinit].ltype == CRAFT)
	{
	    fprintf(fdout, "\t%d %s %s\n", initlist[iinit].howmany, 
		    initlist[iinit].str1, initlist[iinit].str2);
	}
    }

    /* print audit results */
    fprintf(fdout, "\naudit totals ...\n\n");
    for (iinit = 0; iinit < initcnt; iinit++)
    {
	if (initlist[iinit].ltype == AUDIT)
	{
	    fprintf(fdout, "\t%d %s %s\n", initlist[iinit].howmany, 
		    initlist[iinit].str1, initlist[iinit].str2);
	}
    }

    /* print spp results */
    fprintf(fdout, "\nspp totals ...\n\n");
    for (iinit = 0; iinit < initcnt; iinit++)
    {
	if (initlist[iinit].ltype == SPP)
	{
	    fprintf(fdout, "\t%d mod: %s proc: %s\n", initlist[iinit].howmany, 
		    initlist[iinit].str1, initlist[iinit].str2);
	}
    }

    /* print highlight results */
    if (hflag)
    {
	fprintf(fdout, "\nhighlight totals ...\n\n");
	for (iinit = 0; iinit < initcnt; iinit++)
	{
	    if (initlist[iinit].ltype == HIGHLIGHT)
	    {
		fprintf(fdout, "\t%d %s\n", initlist[iinit].howmany, 
					    initlist[iinit].str1);
	    }
	}
    }

    /* summary */
    fprintf(fdout, "\nsummary of asserts, etc.\n\n");
    fprintf(fdout, "\tasserts = %5d\n", asrtcnt);
    fprintf(fdout, "\tcraft   = %5d\n", cftcnt);
    fprintf(fdout, "\taudits  = %5d\n", audcnt);
    fprintf(fdout, "\tspps    = %5d\n", sppcnt);
    fprintf(fdout, "\t----------------\n");
    fprintf(fdout, "\ttotals  = %5d\n", sppcnt + asrtcnt + cftcnt + audcnt);
    fprintf(fdout, "\n");

    /* finished */
    return(ROPOK);
}

/* init hash table */
void inittable()
{
    short i;

    for (i = 0; i < NBINS; i++)
    {
	table[i] = (struct LIST *) NULLPTR;
    }
    return;
}

/* insert list element into hash table */
long storeit(string1, string2, ltype)
char *string1;
char *string2;
enum LISTTYPE ltype;
{
    void track();
    short i, key, klen;
    long sum;
    char ckey[STRSZ];
    struct LIST *plist, *oplist;

    /* concatenate the keys together */
    strcpy(ckey, string1);
    strcat(ckey, string2);

    /* check key length, want an even length */
    if ((klen = strlen(ckey)) == 0) return(ROPOK);
    if ((klen % 2) == 1)
    {
	ckey[klen++] = SPACE;
 	ckey[klen] = NULLCHR;
    }

    /* check size of key, again */
    if (klen < 2) return(ROPOK);

    /* fold and sum key */
    for (i = 0, sum = 0; i < klen; i += 2)
    {
	sum = (sum + ckey[i]*100L + ckey[i+1]) % FOLDNUM;
    }

    /* hash key to get table index */
    key = sum % DIVISOR;

    /* check if this is the 1st table entry for this key */
    if (table[key] == (struct LIST *) NULLPTR)
    {
	/* get 1st entry in table for this key */
	table[key] = (struct LIST *) malloc(sizeof(struct LIST));
	if (table[key] == (struct LIST *) NULLPTR) return(ROPNOTOK);
	/* populate the entry */
	table[key]->str1 = malloc(strlen(string1) + 1);
	if (table[key]->str1 == (char *) NULLPTR) return(ROPNOTOK);
	strcpy(table[key]->str1, string1);
	table[key]->str2 = malloc(strlen(string2) + 1);
	if (table[key]->str2 == (char *) NULLPTR) return(ROPNOTOK);
	strcpy(table[key]->str2, string2);
	table[key]->howmany = 1;
	table[key]->ltype = ltype;
	table[key]->next = (struct LIST *) NULLPTR;
	/* increment counters */
	track(ltype);
	/* return */
#ifdef UNIQUE
	fprintf(stderr, "unique, bin is %d\n", key);
#endif
#ifdef HASH
	fprintf(stderr, "bin is %d\n", key);
#endif
	return(ROPOK);
    }

    /* an entry exists already */
    for (oplist = table[key], plist = table[key];
	 plist != (struct LIST *) NULLPTR;
	 oplist = plist, plist = plist->next)
    {
	if ((strcmp(plist->str1, string1) == 0) &&
	    (strcmp(plist->str2, string2) == 0) &&
	    (plist->ltype == ltype))
	{
	    /* found a match, increment counts */
	    plist->howmany++;
	    /* increment counters */
	    track(ltype);
	    /* return */
#ifdef NOTUNIQUE
	fprintf(stderr, "not unique, bin is %d\n", key);
#endif
#ifdef HASH
	    fprintf(stderr, "bin is %d\n", key);
#endif
	    return(ROPOK);
	}
    }
    
    /* new entry is not in the list, so add it at the end */
    oplist->next = (struct LIST *) malloc(sizeof(struct LIST));
    plist = oplist->next;
    if (plist == (struct LIST *) NULLPTR) return(ROPNOTOK);
    /* populate the entry */
    plist->str1 = malloc(strlen(string1) + 1);
    if (plist->str1 == (char *) NULLPTR) return(ROPNOTOK);
    strcpy(plist->str1, string1);
    plist->str2 = malloc(strlen(string2) + 1);
    if (plist->str2 == (char *) NULLPTR) return(ROPNOTOK);
    strcpy(plist->str2, string2);
    plist->howmany = 1;
    plist->ltype = ltype;
    plist->next = (struct LIST *) NULLPTR;
    /* increment counters */
    track(ltype);
    /* return */
#ifdef UNIQUE
    fprintf(stderr, "unique, bin is %d\n", key);
#endif
#ifdef HASH
    fprintf(stderr, "bin is %d\n", key);
#endif
    return(ROPOK);
}

/* function for printing out usage */
void usage(errmsg, filename, linenumber)
char *errmsg, *filename;
int linenumber;
{
    if (*errmsg != NULLCHR) fprintf(stderr, "%s (%s at %d)\n", 
				    errmsg, filename, linenumber);
    fprintf(stderr, "%s\n", USAGE1);
    fprintf(stderr, "%s\n", USAGE2);
    fprintf(stderr, "%s\n", USAGE3);
    return;
}

/*
 * check if record has a cntrl-y in it.
 */
long is_cntrly(instr)
char *instr;
{
    for ( ; *instr != NULLCHR; instr++)
    {
	if (*instr == CNTRLY) return(ROPOK);
    }
    return(ROPNOTOK);
}

/* read rop time in block */
long getroptime(croptime, buffer, nrecs)
char *croptime;
char buffer[NRECS][STRSZ];
short nrecs;
{
    char *pstr, cyear[6], cmonth[6], cday[6];
    char cdate[15], chour[6], cminute[6], csecond[6];
    long date, atol();

    nrecs = nrecs;

    /* get time, if any */
    pstr = regex(ropdate, buffer[0], cyear, cmonth, cday, 
		 chour, cminute, csecond);
    if (pstr == (char *) NULLPTR) return(BADTSTAMP);

    /* time and date */
    sprintf(croptime, "%s-%s-%s %s:%s:%s", cyear, cmonth, cday,
	    chour, cminute, csecond);

    /* convert time */
    strcpy(cdate, cmonth);
    strcat(cdate, cday);
    strcat(cdate, chour);
    strcat(cdate, cminute);
    date = atol(cdate);

    /* return rop date */
    return(date);
}

/* scan for rcv msgs */
long xrcv(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, rcvscript[50];

    /* check for rcv msgs */
    if ((pstr = regex(rcvstart, buffer[1], rcvscript)) != (char *) NULLPTR)
    {
	/* found a start msg */
	sprintf(instr, "%s, rc/v script, starting %s",
		croptime, rcvscript);
	/* check for highlights */
	highlight(rcvscript, instr);
	return(ROPPRINT);
    }
    if ((pstr = regex(rcvend, buffer[1], rcvscript)) != (char *) NULLPTR)
    {
	/* found a start msg */
	sprintf(instr, "%s, rc/v script, ending %s",
		croptime, rcvscript);
	/* check for highlights */
	highlight(rcvscript, instr);
	return(ROPPRINT);
    }
    if ((pstr = regex(rcvstop, buffer[1], rcvscript)) != (char *) NULLPTR)
    {
	/* found a stop msg */
	sprintf(instr, "%s, rc/v script, stopping %s",
		croptime, rcvscript);
	/* check for highlights */
	highlight(rcvscript, instr);
	return(ROPPRINT);
    }
    if ((pstr = regex(rcvmsg, buffer[1], rcvscript)) != (char *) NULLPTR)
    {
	/* found a rcv msg */
	sprintf(instr, "%s, rc/v msg: %s", croptime, rcvscript);
	return(ROPPRINT);
    }

    return(ROPNOPRINT);
}

/* scan for cmp audits */
long xcmpaud(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, cmp[50], audit[50], errorcode[50], event[50];

    /* check for audits */
    if ((pstr = regex(cmpaudit1, buffer[1], cmp, audit, errorcode, event)) != (char *) NULLPTR)
    {
	/* found an audit */
	sprintf(instr, "%s, cmp: %s, aud: %s, err: %s, evnt: %s",
		croptime, cmp, audit, errorcode, event);
	/* store in hash table */
	if (storeit(audit, errorcode, AUDIT) == ROPNOTOK) return(ROPNOTOK);
	/* check for highlights */
	highlight(audit, instr);
	return(ROPPRINT);
    }
    return(ROPNOPRINT);
}

/* scan for sm audits */
long xsmaud(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, sm[50], audit[50], errorcode[50], event[50];

    /* check for audits */
    if ((pstr = regex(smaudit1, buffer[1], sm, audit, errorcode, event)) != (char *) NULLPTR)
    {
	/* found an audit */
	sprintf(instr, "%s, sm: %s, aud: %s, err: %s, evnt: %s",
		croptime, sm, audit, errorcode, event);
	/* store in hash table */
	if (storeit(audit, errorcode, AUDIT) == ROPNOTOK) return(ROPNOTOK);
	/* check for highlights */
	highlight(audit, instr);
	return(ROPPRINT);
    }
    return(ROPNOPRINT);
}

/* scan for cmp craft asserts */
long xcmpcft(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, assert[50], cmp[50], event[50];
    char fname[50], fline[50];

    /* check for craft asserts */
    if ((pstr = regex(cmpcraft1, buffer[1], assert, cmp, event)) != (char *) NULLPTR)
    {
	/* found a craft assert, get file and line */
	if ((pstr = regex(cmpcraft2, buffer[2], fname, fline)) != (char *) NULLPTR)
	{
	    sprintf(instr, "%s, cmp: %s, craft: %s, evnt: %s, file: %s, line: %s",
		    croptime, cmp, assert, event, fname, fline);
	}
	else
	{
	    /* no file/line info was read */
	    sprintf(instr, "%s, cmp: %s, craft: %s, evnt: %s",
		    croptime, cmp, assert, event);
	}
	/* store in hash table */
	if (storeit(assert, fname, CRAFT) == ROPNOTOK) return(ROPNOTOK);
	if (storeit(assert, NULLSTR, INIT) == ROPNOTOK) return(ROPNOTOK);
	/* check for highlights */
	highlight(assert, instr);
	return(ROPPRINT);
    }
    return(ROPNOPRINT);
}

/* scan for am or sm spps */
long xamsmspp(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, sm[50], env[50], event[50], process[50];

    /* check for am or sm spps */
    if ((pstr = regex(smsppre1, buffer[1], sm, env, event)) != (char *) NULLPTR)
    {
	/* found a sm spp, get process */
	if ((pstr = regex(smsppre2, buffer[2], process)) != (char *) NULLPTR)
	{
	    sprintf(instr, "%s, sm: %s spp, evnt: %s, proc: %s",
		    croptime, sm, event, process);
	    /* store in hash table */
	    if (storeit(sm, process, SPP) == ROPNOTOK) return(ROPNOTOK);
	    return(ROPPRINT);
	}
    }
    else if ((pstr = regex(amsppre1, buffer[1], env, event)) != (char *) NULLPTR)
    {
	/* found a am spp, get process */
	if ((pstr = regex(amsppre2, buffer[2], process)) != (char *) NULLPTR)
	{
	    sprintf(instr, "%s, am spp, evnt: %s, proc: %s",
		    croptime, event, process);
	    /* store in hash table */
	    if (storeit(AM, process, SPP) == ROPNOTOK) return(ROPNOTOK);
	    return(ROPPRINT);
	}
    }
    return(ROPNOPRINT);
}

/* scan for cmp spps */
long xcmpspp(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, cmp[50], env[50], event[50], process[50];

    /* check for cmp spps */
    if ((pstr = regex(cmpsppre1, buffer[1], cmp, env, event)) == (char *) NULLPTR)
    {
	/* no CMP SPP */
	return(ROPNOPRINT);
    }
    /* found a cmp spp, get process */
    if ((pstr = regex(cmpsppre3, buffer[3], process)) == (char *) NULLPTR)
    {
	/* found no process */
	return(ROPNOPRINT);
    }
    /* store info in string */
    sprintf(instr, "%s, cmp: %s spp, evnt: %s, proc: %s", 
	    croptime, cmp, event, process);
    /* store in hash table */
    if (storeit(CMP, process, SPP) == ROPNOTOK) return(ROPNOTOK);
    return(ROPPRINT);
}

/* scan for sm craft asserts */
long xsmcft(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, assert[50], mod[50], event[50];
    char fname[50], fline[50];

    /* check for craft asserts */
    if ((pstr = regex(smcraft1, buffer[1], assert, mod, event)) != (char *) NULLPTR)
    {
	/* found a craft assert, get file and line */
	if ((pstr = regex(smcraft2, buffer[2], fname, fline)) != (char *) NULLPTR)
	{
	    sprintf(instr, "%s, sm: %s, craft: %s, evnt: %s, file: %s, line: %s",
		    croptime, mod, assert, event, fname, fline);
	}
	else
	{
	    /* no file/line info was read */
	    sprintf(instr, "%s, sm: %s, craft: %s, evnt: %s",
		    croptime, mod, assert, event);
	}
	/* store in hash table */
	if (storeit(assert, fname, CRAFT) == ROPNOTOK) return(ROPNOTOK);
	if (storeit(assert, NULLSTR, INIT) == ROPNOTOK) return(ROPNOTOK);
	/* check for highlights */
	highlight(assert, instr);
	return(ROPPRINT);
    }
    return(ROPNOPRINT);
}

/* scan for am craft asserts */
long xamcft(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, assert[50], event[50];
    char fname[50], fline[50];

    /* check for craft asserts */
    if ((pstr = regex(amcraft1, buffer[1], assert, event)) != (char *) NULLPTR)
    {
	/* found a craft assert, get file and line */
	if ((pstr = regex(amcraft2, buffer[2], fname, fline)) != (char *) NULLPTR)
	{
	    sprintf(instr, "%s, am craft: %s, evnt: %s, file: %s, line: %s",
		    croptime, assert, event, fname, fline);
	}
	else
	{
	    /* no file/line info was read */
	    sprintf(instr, "%s, am craft: %s, evnt: %s", croptime, assert, event);
	}
	/* store in hash table */
	if (storeit(assert, fname, CRAFT) == ROPNOTOK) return(ROPNOTOK);
	if (storeit(assert, NULLSTR, INIT) == ROPNOTOK) return(ROPNOTOK);
	/* check for highlights */
	highlight(assert, instr);
	return(ROPPRINT);
    }
    return(ROPNOPRINT);
}

/* scan for am asserts */
long xamasrt(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, lvl[50], env[50], eventtype[50];
    char event[50], report[50], assert[50], failaddr[50];

    /* check for AM asserts */
    if ((pstr = regex(amasrt1, buffer[1], lvl, env, eventtype, event, report)) != (char *) NULLPTR)
    {
	/* check for asserts */
	if ((pstr = regex(defchkfail, eventtype, assert)) != (char *) NULLPTR)
	{
	    /* AM assert, get fail addr */
	    if ((pstr = regex(amasrt2, buffer[2], failaddr)) != (char *) NULLPTR)
	    {
		sprintf(instr, "%s, am asrt: %s, lvl: %s, env: %s, evnt: %s, addr: %s",
		        croptime, assert, lvl, env, event, failaddr);
	    }
	    else
	    {
		/* no fail addr */
		sprintf(instr, "%s, am asrt: %s, lvl: %s, env: %s, evnt: %s",
		        croptime, assert, lvl, env, event);
	    }
	    /* store in hash table */
	    if (storeit(assert, failaddr, ASSERT) == ROPNOTOK) return(ROPNOTOK);
	    if (storeit(assert, NULLSTR, INIT) == ROPNOTOK) return(ROPNOTOK);
	    /* check for highlights */
	    highlight(assert, instr);
	}
	else
	{
	    /* AM init, get fail addr */
	    if ((pstr = regex(amasrt2, buffer[2], failaddr)) != (char *) NULLPTR)
	    {
		sprintf(instr, "%s, am init, lvl: %s, env: %s, type: %s, event: %s, addr: %s",
		        croptime, lvl, env, eventtype, event, failaddr);
	    }
	    else
	    {
		/* no fail addr */
		sprintf(instr, "%s, am init, lvl: %s, env: %s, type: %s, evnt: %s",
		        croptime, lvl, env, eventtype, event);
	    }
	}
	return(ROPPRINT);
    }
    return(ROPNOPRINT);
}

/* scan for sm asserts */
long xsmasrt(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, sm[50], lvl[50], event[50], eventtype[50];
    char assert[50], stimtype[50], failaddr[50];

    /* check for SM asserts */
    if ((pstr = regex(smasrt1, buffer[1], sm, lvl, event)) != (char *) NULLPTR)
    {
	/* check for asserts */
	if ((pstr = regex(smasrt2, buffer[2], eventtype)) == (char *) NULLPTR)
	{
	    return (ROPNOPRINT);
	}
	/* check for asserts */
	if ((pstr = regex(defchkfail, eventtype, assert)) != (char *) NULLPTR)
	{
	    /* SM assert, get fail addr */
	    if ((pstr = regex(smasrt3, buffer[3], stimtype, failaddr)) != (char *) NULLPTR)
	    {
		/* found a fail addr */
		sprintf(instr, "%s, sm: %s, asrt: %s, lvl: %s, evnt: %s, addr: %s",
			croptime, sm, assert, lvl, event, failaddr);
	    }
	    else
	    {
		/* no fail addr */
		sprintf(instr, "%s, sm: %s, asrt: %s, lvl: %s, evnt: %s",
			croptime, sm, assert, lvl, event);
	    }
	    /* store in hash table */
	    if (storeit(assert, failaddr, ASSERT) == ROPNOTOK) return(ROPNOTOK);
	    if (storeit(assert, NULLSTR, INIT) == ROPNOTOK) return(ROPNOTOK);
	    /* check for highlights */
	    highlight(assert, instr);
	}
	else
	{
	    /* SM init, get fail addr */
	    if ((pstr = regex(smasrt3, buffer[3], stimtype, failaddr)) != (char *) NULLPTR)
	    {
		/* found a fail addr */
		sprintf(instr, "%s, sm: %s, lvl: %s, env: %s, evnt: %s, addr: %s",
			croptime, sm, lvl, eventtype, event, failaddr);
	    }
	    else
	    {
		/* no fail addr */
		sprintf(instr, "%s, sm: %s, lvl: %s, env: %s, evnt: %s",
			croptime, sm, lvl, eventtype, event);
	    }
	}
	return(ROPPRINT);
    }

    return(ROPNOPRINT);
}

/* scan for cmp asserts */
long xcmpasrt(instr, buffer, nrecs, croptime)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
char *croptime;
{
    char *pstr, cmp[50], lvl[50], event[50], eventtype[50];
    char assert[50], failaddr[50];

    /* check for CMP asserts */
    if ((pstr = regex(cmpasrt1, buffer[1], cmp, lvl, event)) != (char *) NULLPTR)
    {
	/* check for asserts */
	if ((pstr = regex(cmpasrt2, buffer[2], eventtype)) == (char *) NULLPTR)
	{
	    return (ROPNOPRINT);
	}
	/* check for asserts */
	if ((pstr = regex(defchkfail, eventtype, assert)) != (char *) NULLPTR)
	{
	    /* SM assert, get fail addr */
	    if ((pstr = regex(cmpasrt3, buffer[3], failaddr)) != (char *) NULLPTR)
	    {
		/* found a fail addr */
		sprintf(instr, "%s, cmp: %s, asrt: %s, lvl: %s, evnt: %s, addr: %s",
			croptime, cmp, assert, lvl, event, failaddr);
	    }
	    else
	    {
		/* no fail addr */
		sprintf(instr, "%s, cmp: %s, asrt: %s, lvl: %s, evnt: %s",
			croptime, cmp, assert, lvl, event);
	    }
	    /* store in hash table */
	    if (storeit(assert, failaddr, ASSERT) == ROPNOTOK) return(ROPNOTOK);
	    if (storeit(assert, NULLSTR, INIT) == ROPNOTOK) return(ROPNOTOK);
	    /* check for highlights */
	    highlight(assert, instr);
	}
	else
	{
	    /* CMP init, get fail addr */
	    if ((pstr = regex(cmpasrt3, buffer[3], failaddr)) != (char *) NULLPTR)
	    {
		/* found a fail addr */
		sprintf(instr, "%s, cmp: %s, lvl: %s, env: %s, evnt: %s, addr: %s",
			croptime, cmp, lvl, eventtype, event, failaddr);
	    }
	    else
	    {
		/* no fail addr */
		sprintf(instr, "%s, cmp: %s, lvl: %s, env: %s, evnt: %s",
			croptime, cmp, lvl, eventtype, event);
	    }
	}
	return(ROPPRINT);
    }

    return(ROPNOPRINT);
}
/* function to scan rop file buffer */
long scan(instr, buffer, nrecs, 
          stime, etime, sbyte, ebyte, 
	  cuttype, ctlystart, ctlyend)
char *instr;
char buffer[NRECS][STRSZ];
short nrecs;
long stime, etime;
long sbyte, ebyte;
long cuttype;
long ctlystart, ctlyend;
{
    char *pstr;
    long xrcv(), xcmpaud(), xamcft(), xcmpcft(), xcmpasrt();
    long xsmaud(), xsmcft(), xamasrt();
    long xamsmspp(), xcmpspp();
    long xsmasrt(), getroptime();
    long roptime;
    char croptime[50];

    /* store string in local pointer */
    pstr = instr;

    /* get rop time */
    roptime = getroptime(croptime, buffer, nrecs);
    if (roptime == BADTSTAMP) return(ROPNOPRINT);
#ifdef DEBUG
    fprintf(stderr, "rop time is %s\n", croptime);
#endif

    /* check if a start/end byte or times were given */
    switch (cuttype)
    {
    case TIME:
	/* check time offsets */
	if (etime < stime) return(ROPNOTOK);
	/* check if record is within the given ranges */
	if (roptime < stime || roptime > etime) return(ROPNOPRINT);
	break;
    case SIZE:
	/* check byte offsets */
	if (ebyte < sbyte) return(ROPNOTOK);
	/* check if record is within the given ranges */
	if (ctlyend < sbyte) return(ROPNOPRINT);
	if (ctlystart > ebyte) return(ROPDONE);
	break;
    default:
	/* neither time or byte tests were given */
	break;
    }

    /* check number of records */
    if (nrecs < 2) return (ROPNOPRINT);

    /* check for rcv msgs */
    if ((rflag == ROPOK) && (xrcv(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found an rcv msg */
	return(ROPPRINT);
    }

    /* check for CMP audits */
    if ((uflag == ROPOK) && (xcmpaud(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found an audit */
	return(ROPPRINT);
    }

    /* check for SM audits */
    if ((uflag == ROPOK) && (xsmaud(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found an audit */
	return(ROPPRINT);
    }

    /* check number of records */
    if (nrecs < 3) return (ROPNOPRINT);

    /* check for cmp craft asserts */
    if ((cflag == ROPOK) && (xcmpcft(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found a craft assert */
	return(ROPPRINT);
    }

    /* check for sm craft asserts */
    if ((cflag == ROPOK) && (xsmcft(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found a craft assert */
	return(ROPPRINT);
    }

    /* check for am craft asserts */
    if ((cflag == ROPOK) && (xamcft(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found a craft assert */
	return(ROPPRINT);
    }

    /* check for AM or SM SPPs */
    if ((pflag == ROPOK) && (xamsmspp(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found an SPP */
	return(ROPPRINT);
    }

    /* check for AM asserts */
    if ((aflag == ROPOK) && (xamasrt(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found an AM assert */
	return(ROPPRINT);
    }

    /* check number of records */
    if (nrecs < 4) return (ROPNOPRINT);

    /* check for CMP asserts */
    if ((aflag == ROPOK) && (xcmpasrt(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found a CMP assert */
	return(ROPPRINT);
    }

    /* check for SM asserts */
    if ((aflag == ROPOK) && (xsmasrt(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found an SM assert */
	return(ROPPRINT);
    }

    /* check number of records */
    if (nrecs < 5) return (ROPNOPRINT);

    /* check for CMP SPPs */
    if ((pflag == ROPOK) && (xcmpspp(pstr, buffer, nrecs, croptime) == ROPPRINT))
    {
	/* found a CMP SPP */
	return(ROPPRINT);
    }

    /* done, no printing */
    return (ROPNOPRINT);
}

/* function to scan a rop file */
long ropscan(fdrop, fdout, sbyte, ebyte, stime, etime, cuttype)
FILE *fdrop;
FILE *fdout;
long sbyte, ebyte;
long stime, etime;
long cuttype;
{
    char buffer[NRECS][STRSZ];
    char *pstr, instr[STRSZ];
    short nrecs;
    long toolarge, Print, ctlystart, ctlyend;
    long is_cntrly(), scan();

    /* initialize some variables */
    ctlystart = ctlyend = 0L;
    toolarge = ROPNOTOK;
    nrecs = 0;

    /* read records from rop file or stdin */
    while ((fgets(instr, STRSZ, fdrop) != (char *) NULLPTR) && (! feof(fdrop)))
    {
	/* replace CR with a blank */
	for (pstr = instr; *pstr != NULLCHR; pstr++)
	{
	    if (*pstr == CR) *pstr = SPACE;
	}
	/* check for a control-y */
	if (is_cntrly(instr) == ROPOK)
	{
	    /* get present location, if required */
	    if (cuttype == SIZE)
	    {
		ctlyend = ftell(fdrop) - 1L;
	    }
	    /* scan for audits, etc. */
	    Print = scan(instr, buffer, nrecs, 
			 stime, etime, sbyte, ebyte,
			 cuttype, ctlystart, ctlyend);
	    switch (Print)
	    {
	    case ROPPRINT:
		fprintf(fdout, "%s\n", instr);
		if (fdout == stdout) fflush(fdout);
	    case ROPNOPRINT:
		ctlystart = ctlyend + 1;
		nrecs = 0;
		toolarge = ROPNOTOK;
		continue;
	    case ROPDONE:
		return (ROPOK);
	    default:
		return (ROPNOTOK);
	    }
	}
	/* store record in buffer */
	if (nrecs < NRECS)
	{
	    strcpy(buffer[nrecs], instr);
	    nrecs++;
	}
	else if (toolarge == ROPNOTOK)
	{
	    toolarge = ROPOK;
	}
    }
    /* done */
    return (ROPOK);
}

/* main program */
main(argc, argv)
int argc;
char **argv;
{
    long stime, etime;
    long sbyte, ebyte;
    long cuttype;
    FILE *fdrop;
    FILE *fdout;
    char *outfile;
    int rop, c, getopt();
    long status, atol();
    void inittable();
    long printtable();

    /* initialize some variables */
    hflag = rflag = aflag = cflag = Bflag = uflag = pflag = ROPNOTOK;
    stime = sbyte = 0L;
    etime = ebyte = MAXLONG;
    outfile = (char *) NULLPTR;
    fdout = (FILE *) NULLPTR;

    /* initialize the hash table */
    inittable();

    /* get options from commnad line */
    while ((c = getopt(argc, argv, "?Bracups:e:S:E:o:h:")) != EOF)
    {
	switch (c)
	{
	case 'h':
	    hflag = ROPOK;
	    if (gethlist(optarg) == ROPNOTOK)
	    {
		USAGE("failure getting highlight list.");
		exit(2);	
	    }
	    break;
	case 'B':
	    /* ring stupid bell for highlights */
	    Bflag = ROPOK;
	    break;
	case 'r':
	    /* track rcv */
	    rflag = ROPOK;
	    break;
	case 'a':
	    /* track asserts */
	    aflag = ROPOK;
	    break;
	case 'c':
	    /* track craft asserts */
	    cflag = ROPOK;
	    break;
	case 'u':
	    /* track audits */
	    uflag = ROPOK;
	    break;
	case 'p':
	    /* track spps */
	    pflag = ROPOK;
	    break;
	case 'o':
	    /* output file name */
	    outfile = optarg;
	    break;
	case 's':
	    /* start time for scan */
	    stime = atol(optarg);
	    cuttype = TIME;
	    break;
	case 'e':
	    /* end time for scan */
	    etime = atol(optarg);
	    cuttype = TIME;
	    break;
	case 'S':
	    /* start byte for scan */
	    sbyte = atol(optarg);
	    cuttype = SIZE;
	    break;
	case 'E':
	    /* end byte for scan */
	    ebyte = atol(optarg);
	    cuttype = SIZE;
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

    /* check options */
    if ((cuttype == TIME) && (sbyte != 0L || ebyte != MAXLONG))
    {
	USAGE("cannot combine -S, -E options with -s, -e options.");
	exit(2);
    }
    if ((cuttype == SIZE) && (stime != 0L || etime != MAXLONG))
    {
	USAGE("cannot combine -s, -e options with -S, -E options.");
	exit(2);
    }

    /* compile all REs */
    if ((amasrt1 = regcmp(AMASRT1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for AMASRT1.");
	exit(2);
    }
    if ((amasrt2 = regcmp(AMASRT2, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for AMASRT2.");
	exit(2);
    }
    if ((smasrt1 = regcmp(SMASRT1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for SMASRT1.");
	exit(2);
    }
    if ((smasrt2 = regcmp(SMASRT2, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for SMASRT2.");
	exit(2);
    }
    if ((smasrt3 = regcmp(SMASRT3, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for SMASRT3.");
	exit(2);
    }
    if ((amcraft1 = regcmp(AMCRAFT1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for AMCRAFT1.");
	exit(2);
    }
    if ((amcraft2 = regcmp(AMCRAFT2, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for AMCRAFT2.");
	exit(2);
    }
    if ((smcraft1 = regcmp(SMCRAFT1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for SMCRAFT1.");
	exit(2);
    }
    if ((smcraft2 = regcmp(SMCRAFT2, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for SMCRAFT2.");
	exit(2);
    }
    if ((smaudit1 = regcmp(SMAUDIT1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for SMAUDIT1.");
	exit(2);
    }
    if ((ropdate = regcmp(ROPDATE, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for ROPDATE.");
	exit(2);
    }
    if ((defchkfail = regcmp(DEFCHKFAIL, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for DEFCHKFAIL.");
	exit(2);
    }
    if ((rcvstart = regcmp(RCVSTART, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for RCVSTART.");
	exit(2);
    }
    if ((rcvend = regcmp(RCVEND, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for RCVEND.");
	exit(2);
    }
    if ((rcvstop = regcmp(RCVSTOP, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for RCVSTOP.");
	exit(2);
    }
    if ((rcvmsg = regcmp(RCVMSG, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for RCVMSG.");
	exit(2);
    }
    if ((cmpaudit1 = regcmp(CMPAUDIT1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for CMPAUDIT1.");
	exit(2);
    }
    if ((cmpcraft1 = regcmp(CMPCRAFT1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for CMPCRAFT1.");
	exit(2);
    }
    if ((cmpcraft2 = regcmp(CMPCRAFT2, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for CMPCRAFT2.");
	exit(2);
    }
    if ((cmpasrt1 = regcmp(CMPASRT1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for CMPASRT1.");
	exit(2);
    }
    if ((cmpasrt2 = regcmp(CMPASRT2, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for CMPASRT2.");
	exit(2);
    }
    if ((cmpasrt3 = regcmp(CMPASRT3, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for CMPASRT3.");
	exit(2);
    }
    if ((amsppre1 = regcmp(AMSPPRE1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for AMSPPRE1.");
	exit(2);
    }
    if ((amsppre2 = regcmp(AMSPPRE2, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for AMSPPRE2.");
	exit(2);
    }
    if ((smsppre1 = regcmp(SMSPPRE1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for SMSPPRE1.");
	exit(2);
    }
    if ((smsppre2 = regcmp(SMSPPRE2, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for SMSPPRE2.");
	exit(2);
    }
    if ((cmpsppre1 = regcmp(CMPSPPRE1, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for CMPSPPRE1.");
	exit(2);
    }
    if ((cmpsppre2 = regcmp(CMPSPPRE2, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for CMPSPPRE2.");
	exit(2);
    }
    if ((cmpsppre3 = regcmp(CMPSPPRE3, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for CMPSPPRE3.");
	exit(2);
    }
    if ((cmpsppre4 = regcmp(CMPSPPRE4, (char *) NULLPTR)) == (char *) NULLPTR)
    {
	USAGE("bad RE for CMPSPPRE4.");
	exit(2);
    }

    /* check that at least one type of init is tracked */
    if ((rflag + aflag + cflag + uflag + pflag) == ROPNOTOK)
    {
	/* track everything */
	rflag = aflag = cflag = uflag = pflag = ROPOK;
    }

    /* check for output file name */
    if (outfile == (char *) NULLPTR)
    {
	fdout = stdout;
    }
    else if ((fdout = fopen(outfile, "w")) == (FILE *) NULLPTR)
    {
	USAGE("Unable to open output file.");
	EXIT(2);
    }
    /* check if a rop file was given */
    if (optind >= argc)
    {
	/* no rop file, get rop from stdin */
	if (cuttype == SIZE)
	{
	    /* size option can not be used when reading stdin */
	    USAGE("-S and -E options can only be used if a rop file is given.");
	    EXIT(2);
	}
#ifdef DEBUG
	fprintf(stderr, "ropfile is %s.\n", argv[rop]);
#endif
	/* scan rop file */
	status = ropscan(stdin, fdout, sbyte, ebyte, stime, etime, cuttype);
	if (status != ROPOK)
	{
	    USAGE("scan thru rop read from stdin failed.");
	    EXIT(2);
	}
    }
    else
    {
	/* rop file was given, scan each rop file */
	for (rop = optind; rop < argc; rop++)
	{
	    /* open rop file for read */
	    if ((fdrop = fopen(argv[rop], "r")) == (FILE *)NULLPTR)
	    {
		USAGE("unable to open rop file.");
		EXIT(2);
	    }
#ifdef DEBUG
	    fprintf(stderr, "ropfile is %s.\n", argv[rop]);
#endif
	    /* scan rop file */
	    status = ropscan(fdrop, fdout, sbyte, ebyte, stime, etime, cuttype);
	    if (status != ROPOK)
	    {
		USAGE("scan thru rop file failed.");
		EXIT(2);
	    }
	    /* close rop file */
	    (void) fclose(fdrop);
	}
    }

    /* print table results */
    if (printtable(fdout) != ROPOK)
    {
	USAGE("print table failed.");
	EXIT(2);
    }

    /* finished */
    EXIT(0);
}
