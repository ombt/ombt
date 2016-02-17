/*
 * program to get a devices attributes.
 */

/* system headers */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <termio.h>
#include <fcntl.h>
#include <values.h>

/* externs */
extern int errno, sys_nerr;
extern char *sys_errlist[];

/* local definitions */
#define USAGE "usage: ttyattr device ..."
#define TIMEOUT 10
#define ATTRFALSE 0
#define ATTRTRUE 1
#define ATTROK 0
#define ATTRNOTOK -1
#define ATTRTMOUT -2
#define ATTRNOTTTY -3

/* translation tables */
struct TRTABLE {
    unsigned long value;
    char *name;
} ;
#define ENDOFTABLE MAXLONG

/* ascii table */
struct TRTABLE asciitable[] = {
    { 000, "nul" },
    { 001, "soh" },
    { 002, "stx" },
    { 003, "etx" },
    { 004, "eot" },
    { 005, "enq" },
    { 006, "ack" },
    { 007, "bel" },
    { 010, "bs" },
    { 011, "ht" },
    { 012, "nl" },
    { 013, "vt" },
    { 014, "np" },
    { 015, "cr" },
    { 016, "so" },
    { 017, "si" },
    { 020, "dle" },
    { 021, "dc1" },
    { 022, "dc2" },
    { 023, "dc3" },
    { 024, "dc4" },
    { 025, "nak" },
    { 026, "syn" },
    { 027, "etb" },
    { 030, "can" },
    { 031, "em" },
    { 032, "sub" },
    { 033, "esc" },
    { 034, "fs" },
    { 035, "gs" },
    { 036, "rs" },
    { 037, "us" },
    { 040, "sp" },
    { 041, "!" },
    { 042, "\"" },
    { 043, "#" },
    { 044, "$" },
    { 045, "%" },
    { 046, "&" },
    { 047, "'" },
    { 050, "(" },
    { 051, ")" },
    { 052, "*" },
    { 053, "+" },
    { 054, "," },
    { 055, "-" },
    { 056, "." },
    { 057, "/" },
    { 060, "0" },
    { 061, "1" },
    { 062, "2" },
    { 063, "3" },
    { 064, "4" },
    { 065, "5" },
    { 066, "6" },
    { 067, "7" },
    { 070, "8" },
    { 071, "9" },
    { 072, ":" },
    { 073, ";" },
    { 074, "<" },
    { 075, "=" },
    { 076, ">" },
    { 077, "?" },
    { 0100, "@" },
    { 0101, "A" },
    { 0102, "B" },
    { 0103, "C" },
    { 0104, "D" },
    { 0105, "E" },
    { 0106, "F" },
    { 0107, "G" },
    { 0110, "H" },
    { 0111, "I" },
    { 0112, "J" },
    { 0113, "K" },
    { 0114, "L" },
    { 0115, "M" },
    { 0116, "N" },
    { 0117, "O" },
    { 0120, "P" },
    { 0121, "Q" },
    { 0122, "R" },
    { 0123, "S" },
    { 0124, "T" },
    { 0125, "U" },
    { 0126, "V" },
    { 0127, "W" },
    { 0130, "X" },
    { 0131, "Y" },
    { 0132, "Z" },
    { 0133, "[" },
    { 0134, "\\" },
    { 0135, "]" },
    { 0136, "^" },
    { 0137, "_" },
    { 0140, "`" },
    { 0141, "a" },
    { 0142, "b" },
    { 0143, "c" },
    { 0144, "d" },
    { 0145, "e" },
    { 0146, "f" },
    { 0147, "g" },
    { 0150, "h" },
    { 0151, "i" },
    { 0152, "j" },
    { 0153, "k" },
    { 0154, "l" },
    { 0155, "m" },
    { 0156, "n" },
    { 0157, "o" },
    { 0160, "p" },
    { 0161, "q" },
    { 0162, "r" },
    { 0163, "s" },
    { 0164, "t" },
    { 0165, "u" },
    { 0166, "v" },
    { 0167, "w" },
    { 0170, "x" },
    { 0171, "y" },
    { 0172, "z" },
    { 0173, "{" },
    { 0174, "" },
    { 0175, "}" },
    { 0176, "~" },
    { 0177, "del" },
    { ENDOFTABLE, ""}
};
/* baud rates */
struct TRTABLE baudrate[] = {
	{ B0, "0" },
	{ B50, "50" },
	{ B75, "75" },
	{ B110, "110" },
	{ B134, "134" },
	{ B150, "150" },
	{ B200, "200" },
	{ B300, "300" },
	{ B600, "600" },
	{ B1200, "1200" },
	{ B1800, "1800" },
	{ B2400, "2400" },
	{ B4800, "4800" },
	{ B9600, "9600" },
	{ B19200, "19200" },
	{ B38400, "38400" },
	{ ENDOFTABLE, ""}
} ;

/* global variables */
long timeout;

/* find a match in a table */
long findtabent(table, value)
register struct TRTABLE *table;
register unsigned long value;
{
    register struct TRTABLE *ptab;
    for (ptab = table; ptab->value != ENDOFTABLE; ptab++)
    {
	if (ptab->value == value) return((long)(ptab - table));
    }
    return(ENDOFTABLE);
}

/* error handling */
void syserr(errmsg)
char *errmsg;
{
    if (*errmsg != '\0') fprintf(stderr, "%s\n", errmsg);
    if (errno > 0 && errno < sys_nerr)
    {
	fprintf(stderr, "error %d : %s\n", errno, sys_errlist[errno]);
    }
    return;
}

/* trap function for timeouts */
#ifdef VAX
void tmout_trap(int sig)
#else
int tmout_trap()
#endif
{
    /* time out has occurred, set flag */
    sig;
    timeout = ATTRTRUE;
    return;
}

/* function to open a terminal */
int ttyopen(devicename, flags)
char *devicename;
int flags;
{
    int ttyfd;
    unsigned alarm();
#ifdef VAX
    void (*oldsigfn)(), tmout_trap();
#else
    int (*oldsigfn)(), tmout_trap();
#endif

    /* set timeout flag to false */
    timeout = ATTRFALSE;
    /* set alarm trap */
    oldsigfn = signal(SIGALRM, tmout_trap);
    /* start alarm */
    (void) alarm((unsigned) TIMEOUT);
    /* open terminal */
    ttyfd = open(devicename, flags);
    /* device was opened or it failed with timeout, etc. */
    (void) alarm(0);
    (void) signal(SIGALRM, oldsigfn);
    /* did time out occur ? */
    if (timeout == ATTRTRUE) return(ATTRTMOUT);
    /* other failures ? */
    if (ttyfd < 0) return(ttyfd);
    /* is devicename a tty ? */
    if ( ! isatty(ttyfd))
    {
	/* not a tty */
	(void) close(ttyfd);
	return(ATTRNOTTTY);
    }
    /* everything is ok, return fd */
    return(ttyfd);
}

/* get ioctl attributes */
int getioctl(ttyfd, ioctlbuf)
int ttyfd;
struct termio *ioctlbuf;
{
    int ionctl();

    /* check if ttyfd points to a terminal */
    if ( ! isatty(ttyfd))
    {
	/* not a tty */
	return(ATTRNOTTTY);
    }
    /* get tty info */
    if (ioctl(ttyfd, TCGETA, ioctlbuf) == -1)
    {
	/* error getting ioctl info */
	return(ATTRNOTOK);
    }
    /* success in getting the ioctl info */
    return(ATTROK);
}

/* print ioctl data */
void printioctl(ioctlbuf)
struct termio *ioctlbuf;
{
    unsigned long i;

    /* octal dumps of ioctl structure */
    fprintf(stdout, "- octal dumps -\n");
    fprintf(stdout, "input modes   : %07o\n", ioctlbuf->c_iflag);
    fprintf(stdout, "output modes  : %07o\n", ioctlbuf->c_oflag);
    fprintf(stdout, "control modes : %07o\n", ioctlbuf->c_cflag);
    fprintf(stdout, "line discipline modes : %07o\n", ioctlbuf->c_lflag);
    fprintf(stdout, "line discipline : %07o\n", ioctlbuf->c_line);
    fprintf(stdout, "control char    : ");
    for (i = 0; i < NCC; i++)
    {
	fprintf(stdout, "%04o ", ioctlbuf->c_cc[i]);
    }
    fprintf(stdout, "\n\n");
    /* print baud rate */
    i = findtabent(baudrate, ((unsigned long) ioctlbuf->c_cflag & CBAUD));
    fprintf(stdout, "baud rate : %s\n", 
	   (i == ENDOFTABLE) ? "unknown" : baudrate[i].name);
    /* print control characters */
    i = findtabent(asciitable, (unsigned long) ioctlbuf->c_cc[VINTR]);
    fprintf(stdout, "interrupt   : %s\n", 
	   (i == ENDOFTABLE) ? "unknown" : asciitable[i].name);
    i = findtabent(asciitable, (unsigned long) ioctlbuf->c_cc[VQUIT]);
    fprintf(stdout, "quit        : %s\n", 
	   (i == ENDOFTABLE) ? "unknown" : asciitable[i].name);
    i = findtabent(asciitable, (unsigned long) ioctlbuf->c_cc[VERASE]);
    fprintf(stdout, "erase       : %s\n", 
	   (i == ENDOFTABLE) ? "unknown" : asciitable[i].name);
    i = findtabent(asciitable, (unsigned long) ioctlbuf->c_cc[VKILL]);
    fprintf(stdout, "kill        : %s\n", 
	   (i == ENDOFTABLE) ? "unknown" : asciitable[i].name);
    i = findtabent(asciitable, (unsigned long) ioctlbuf->c_cc[VEOF]);
    fprintf(stdout, "end-of-file : %s\n", 
	   (i == ENDOFTABLE) ? "unknown" : asciitable[i].name);
    i = findtabent(asciitable, (unsigned long) ioctlbuf->c_cc[VEOL]);
    fprintf(stdout, "end-of-line : %s\n", 
	   (i == ENDOFTABLE) ? "unknown" : asciitable[i].name);
    /* finished */
    return;
}

/* function to check if a character string is numeric */
int isnumeric(pstr)
char *pstr;
{
    for ( ; ('0' <= *pstr) && (*pstr <= '9'); pstr++)
    {
	/* do nothing */
    }
    if (*pstr != '\0') return(ATTRFALSE);
    return(ATTRTRUE);
}

/* main program */
main(argc, argv)
int argc;
char *argv[];
{
    char devname[BUFSIZ];
    int status, iarg, ttyfd;
    struct termio ioctlbuf;
    void printioctl(), syserr();
    int getioctl();

    /* check that a device name was given */
    if (argc <= 1)
    {
	fprintf(stderr, "%s\n", USAGE);
	exit(2);
    }
    /* loop over all devices */
    for (iarg = 1; iarg < argc; iarg++)
    {
	/* device name must be in /dev partition */
	devname[0] = 0;
	if (strncmp("/dev/", argv[iarg], 5) != 0)
	{
	    if (isnumeric(argv[iarg]) == ATTRTRUE)
	    {
		/* append /dev/tty to devname */
		strcpy(devname, "/dev/tty");
	    }
	    else
	    {
		/* append /dev to devname */
		strcpy(devname, "/dev/");
	    }
	}
	strcat(devname, argv[iarg]);
	/* print device name */
	fprintf(stdout, "\ndevice <%s> characteristics are ...\n", devname);
	/* open terminal */
	ttyfd = ttyopen(devname, O_RDONLY);
	if (ttyfd < 0)
	{
	    /* check for errors */
	    switch (ttyfd)
	    {
	    case ATTRNOTTTY:
		fprintf(stderr, "device %s is not a tty.\n", devname);
		break;
	    case ATTRTMOUT:
		fprintf(stderr, "timeout opening device %s.\n", devname);
		break;
	    default:
		syserr("ttyopen failed for device.");
		break;
	    }
	    /* goto next device */
	    continue;
	}
	/* get ioctl data */
	switch (getioctl(ttyfd, &ioctlbuf))
	{
	case ATTROK:
	    break;
	case ATTRNOTTTY:
	    fprintf(stderr, "device %s is not a tty.\n", devname);
	    (void) close(ttyfd);
	    continue;
	default:
	    syserr("getioctl failed on device.");
	    (void) close(ttyfd);
	    continue;
	}
	/* print ioctl data */
	printioctl(&ioctlbuf);
	/* close tty */
	(void) close(ttyfd);
    }
    /* done,exit */
    exit(0);
}
