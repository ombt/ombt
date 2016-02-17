static char Csccs[] = "@(#)ctrmkrmt.c	2.7, last mod on 1/28/88";
/*
 *	K e r m i t  File Transfer Utility
 *
 *	For use with CTRM 1.9 or later.
 *
 *	UNIX Kermit, Columbia University, 1981, 1982, 1983
 *	Bill Catchings, Bob Cattani, Chris Maio, Frank da Cruz
 *
 *	Modified for use with CTRM by Ted Roycraft, ATT Bell Labs.
 *
 *	usage: ctrmkrmt [-s or -r or -S or -R][-i][-d] [f1 f2 ...]
 *
 *	where s=send [files], r=receive, d=debug,
 *	For "host" mode Kermit, format is either "kermit -r" to
 *	receive files, or "kermit -s f1 f2 ..." to send f1 .. fn.
 *
 *	Supported by CTRM 1.9 ...
 *	-S  --> send the named files to the pc and initiate the
 *		Kermit transfer option on the pc from the Unix end.	7/8/85
 *	-R  --> receive the named files from the pc and initiate the
 *		Kermit transfer option on the pc from the Unix end.	7/8/85
 *
 *
 *	Fixed up again for Unix, Jim Guyton 7/13/83 (Rand Corp)
 *
 *	Modified for use with 'CTRM' by Ted Roycraft (BTL), 7/84.
 *	(For use with CTRM 1.9 or later.)
 *
 *		If the file '.ctrmrc' is present either in the current
 *		or HOME directory, it will be opened and read for
 *		initialization commands.  So far, the only valid
 *		commands are:
 *			log transactions <name>
 *		and
 *			set debug
 *		and
 *			set blockcheck crc
 *		or
 *			set blockcheck lrc
 *		and
 *			set packet <size>
 *
 *		<name> is the name of a file to which transaction logging
 *		information will be appended.  This information includes
 *		when uploading and downloading was done, what files were
 *		sent or received and whether the transfer was successful.
 *
 *		The 'set debug' command turns on debugging to two files.
 *
 *		Normally ctrmkrmt will attempt to tell CTRM that
 *		it will use LRC block checking (which all Kermits
 *		can use).  The line 'set blockcheck crc' in .ctrmrc forces
 *		ctrmkrmt to use CRC-CCITT which will cause problems for older
 *		versions of CTRM.  The line 'set blockcheck lrc' forces
 *		LRC block checking (the default) which all Kermits can use.
 *
 *		'set packet <size>' tells ctrmkrmt what size packets
 *		to offer the remote Kermit for
 *		downloads.  The packet size can range from 94 bytes 
 *		to 8192 bytes. (The 'other' Kermit may not support up to
 *		8192 bytes but this is settled in the negotiation that
 *		goes on between the two Kermits.)
 *
 *		In .ctrmrc, lines beginning with '#' are treated as comments
 *		and invalid commands are silently ignored.
 *
 *	Compile options:
 *
 *		For ATT UNIX System V,
 *
 *			cc -o ctrmkrmt ctrmkrmt.c
 *
 *		For AT&T UNIX 9'th Edition,
 *
 *			cc -O -DV9 -o ctrmkrmt ctrmkrmt.c
 *
 *		For Berkeley 4.x UNIX,
 *
 *			cc -o ctrmkrmt -DUCB4X ctrmkrmt.c
 */
#ifdef V9
#define	index	strchr
#define	UCB4X	1
#endif
#ifndef UCB4X
#define USG	1
#endif

#include <stdio.h>		/* Standard UNIX definitions */
#include <fcntl.h>
#ifdef UCB4X
#include <sgtty.h>
#endif
#ifdef USG
#include <termio.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <signal.h>
#include <setjmp.h>

extern int settty();
char chk10();
unsigned int chk3();

/* Symbol Definitions */

#define MAXPACK 	94	/* Maximum packet size */
#define LGPKT		8192	/* long packet size */
#define SOH		1	/* Start of header */
#define SP		32	/* ASCII space */
#define NL		'\n'
#define CR		015	/* ASCII Carriage Return */
#define DEL		127	/* Delete (rubout) */
#define CTRLD		4

#define MAXTRY		5	/* Times to retry a packet */
#define MYQUOTE 	'#'     /* Quote character I will use */
#define MYPAD		0	/* Number of padding characters I will need */
#define MYPCHAR 	0	/* Padding character I need */
#define MYEOL		'\r'    /* End-Of-Line character I need */
#define MYTIME		7	/* Seconds after which I should be timed out */
#define MAXTIM		20	/* Maximum timeout interval */
#define MINTIM		2	/* Minumum timeout interval */
#define CTIMEOUT	127	/* # of tenths of seconds for getchar timeout */
#define MYQBIN		'Y'     /* I will do 8-bit quoting */
#define MYCHKT		'1'     /* lrc */
#define MYREPT		'`'     /* prefix for repeat characters */
#define MAXREPT 	94	/* maximum times to repeat a char */
#define TRUE		1	/* Boolean constants */
#define FALSE		0
#define LRC		1
#define CCITT		3
#define LONGPACKETS	0x02	/* capabilities flag */

#define equal(a,b)	(strcmp((a),(b)) == 0)
#define rptenabled()	(rhdr.rept != ' ')
#define min(a,b)	(((a) < (b)) ? (a) : (b))

/* tochar converts a control character to a printable one by adding a space */
#define tochar(ch)	((ch) + ' ')

/* unchar undoes tochar */
#define unchar(ch)	((ch) - ' ')

/*
 * ctl turns a control character into a printable character by toggling the
 * control bit (ie. ^A becomes A and A becomes ^A).
 */
#define ctl(ch) 	((ch) ^ 0100)
#define bump(n) 	((n+1) & 077)
#define cs8(term)	((((term).c_cflag) & CSIZE) == CS8)

/* Global Variables */

#ifdef USG
struct stat statbuf;
#endif

struct hdr {
	char maxl;		/* maximum length packet */
	char time;		/* timeout for a packet */
	char npad;		/* # of pad characters */
	char padc;		/* control char for padding */
	char eol;		/* packet terminator char */
	char qctl;		/* quote for ctl characters */
	char qbin;		/* 8-bit quote chars */
	char chkt;		/* bcc type; 1-lrc, 2- 2char checksum, 3- 3char checksum */
	char rept;		/* prefix for repeat characters */
	char capasl;		/* capability byte relevant for long packets */
	char windo;		/* space for window option */
	char maxl1;		/* maximum size of large buffer (high order) */
	char maxl2;		/* maximum size of large buffer (low order ) */
};

struct hdr shdr = {
	MAXPACK,	MYTIME, 	MYPAD,		MYPCHAR,
	MYEOL,		MYQUOTE,	MYQBIN, 	MYCHKT,
	MYREPT, 	0,		0,		0,	0
};

struct hdr rhdr = {
	MAXPACK,	MYTIME, 	MYPAD,		MYPCHAR,
	MYEOL,		MYQUOTE,	MYQBIN, 	MYCHKT,
	' ',            0,              0,              0,	0
};


long atol();
static int rpsize = LGPKT;
static int spsize;
int lpcapu = 0;			/* long packet capability used */
int Lpcapr = 1;			/* long packet capability requested */
int Incount = 0;		/* characters in the receive buffer */

static char pwd[80] = {'\0'};
static char *nullstr = "";
static int savech;
static int holdcount = 0;
static char esc = '\033';       /* escape */
static char delete = '\177';   /* delete character */
static char cantsend8;		/* set when trying to send 8-bit chars in text mode */

int	size,			/* Size of present data */
	n,			/* Message number */
	numtry, 		/* Times this packet retried */
	oldtry, 		/* Times previous packet retried */
	image,			/* -1 means 8-bit mode */
	RptProc,		/* true if we can do repeat char processing */
	sentabort,		/* set true when we send an 'E' packet */
	exitcode,		/* exit code to use */
	ws8,			/* true if char size is 8 */
	ebq,			/* character for 8-bit quoting */
	ebqflg, 		/* true if 8-bit quoting on */
	fdtty,			/* file descriptor for /dev/tty */
	debug;			/* -1 means debugging */

FILE	*fp,
	*fptranlog;

char	state,			/* Present state of the automaton */
	ptype,			/* for logging unexpected packet types */
	alarmcalled,		/* for alarm processing */
	bctu = LRC,		/* block check type used; 1-LRC, 3-CRC-CCITT */
	bctr = LRC,		/* block check type requested */
	**filelist,		/* List of files to be sent */
	*filnam,		/* Current file name */
	logbuf[80],		/* temporary buffer for logging */
	recpkt[LGPKT+200],	/* Receive packet buffer */
	*recptr,
	*reason = "?",		/* reason for abort */
	packet[LGPKT+200];	/* Packet buffer */

char *getenv(), *ctrmtime(), *ctrmdate(), *ctime();
char *ctrmrc = ".ctrmrc";
extern int optind;
extern char *optarg;
#ifdef USG
struct termio ssave;
struct termio snew;
#endif
#ifdef UCB4X
struct sgttyb ssave;
struct sgttyb snew;
struct tchars tchsave;
struct tchars tchnew;
#endif
int	rflg, sflg, Rflg, Sflg;
FILE	*errors;
FILE	*fpdebug;

jmp_buf env;			/* Environment ptr for timeout longjump */

/*
    Kermit Packet Layout

	+-----+-----+-----+------+------------+-------+
	| SOH | LEN | SEQ | TYPE |    DATA    | CHECK |
	+-----+-----+-----+------+------------+-------+

    Long packet extensions

+-----+-----+-----+------+-------+-------+--------+---------------+-------+
| SOH |     | SEQ | TYPE | LENX1 | LENX2 | HCHECK |   DATA ...	  | CHECK |
+-----+-----+-----+------+-------+-------+--------+---------------+-------+

_______________________________________________________________________________
    Send-Init Data Field Layout

+------+------+------+------+-----+------+------+------+------+-------	 ------+
| MAXL | TIME | NPAD | PADC | EOL | QCTL | QBIN | CHKT | REPT | CAPA0 ... CAPA4
+------+------+------+------+-----+------+------+------+------+-------	 ------+

    Long packet extensions
			      CAPAS+1  CAPAS+2  CAPAS+3
	-------+-------+ ... +--------+--------+--------+
     ... CAPA4 | CAPA5 |     | WINDOW | MAXLX1 | MAXLX2 |
	-------+-------+ ... +--------+--------+--------+
*/
/*
 *	m a i n
 *
 *	Main routine - parse command and options, set up the
 *	tty lines, and dispatch to the appropriate routine.
 */

main(argc, argv)
int	argc;			/* Character pointer for */
char	**argv; 		/*  command line arguments */
{
	char	*susage;
	int	c;
	int	i;
	void	alarmfn();
	void	cleanup();

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
	initfile();			/* initialize from .ctrmrc if present */
	fdtty = open("/dev/tty", O_RDWR);
	tranlog("Transactions started:\t%s", ctrmdate());
#ifdef USG
	ioctl(fdtty, TCGETA, &ssave);
	ioctl(fdtty, TCGETA, &snew);
	ws8 = cs8(snew);		/* see whether we need 8-bit quoting */
#endif
#ifdef UCB4X
	ioctl(fdtty, TIOCGETP, &ssave);
	ioctl(fdtty, TIOCGETP, &snew);
	ioctl(fdtty, TIOCGETC, &tchsave);
	ioctl(fdtty, TIOCGETC, &tchnew);
	ws8 = TRUE;
#endif
	susage = argv[0];

	if (argc < 2)
		usage(susage);		/* Make sure there's a command line. */

	sflg = rflg = 0;		/* Turn off all parse flags */
	image = FALSE;			/* Default to 7-bit mode */

	while((c = getopt(argc, argv, "sridRS")) != EOF)
		switch (c) {
		case 's':
			sflg++;
			break;		/* S = SEND command */
		case 'r':
			rflg++;
			break;		/* R = RECEIVE command */
		case 'i':
			image = TRUE;
			break;		/* Image (8-bit) mode */
		case 'd':
			debug = TRUE;
			break;		/* Debug mode */
		case 'S':
			Sflg++;
			break;		/* initiate kermit transfer to pc */
		case 'R':
			Rflg++;
			break;		/* initiate kermit transfer from pc */
		case '?':
			usage(susage);
		}

	argc -= optind;
	argv = &argv[optind];
	if (debug) {
		tranlog(" Debugging output sent to files 'xk.err' and 'CTRM.DEB'\n");
		errors = fopen("xk.err", "w");
		setbuf(errors, NULL);	/* make it unbuffered */
		fpdebug = fopen("CTRM.DEB", "w");
		setbuf(fpdebug, NULL);	/* make it unbuffered */
	}

	/* Done parsing */

	if ((sflg + rflg + Sflg + Rflg) != 1)
		usage(susage);		/* Only one command allowed */

	signal(SIGQUIT, SIG_IGN);
/*	signal(SIGQUIT, cleanup);	*/
	signal(SIGALRM, alarmfn);
	signal(SIGINT, cleanup);
	signal(SIGHUP, cleanup);

	/* Put the tty(s) into the correct modes */

	settty();
#ifdef USG
	if(debug) {
		fputs("\nSTRUCT TERMIO SNEW <", fpdebug);
		fwrite(&snew, sizeof(struct termio), 1, fpdebug);
		fputs("> END OF STRUCT TERMIO SNEW\n",fpdebug);
	}
#endif
	exitcode = 0;

	/* All set up, now execute the command that was given. */

	if(sflg || Sflg) {
		if(Sflg) {	/* send <esc>&X[ia]R - tell CTRM to receive */
			write(fdtty, &esc, 1);
			write(fdtty, "&X", 2);
			write(fdtty, (image == TRUE) ? "iR" : "aR", 2);
			drain();
			sleep(2);
		}
		if (argc--)
			filnam = *argv++;	/* Get file to send */
		else
			usage(susage);
		filelist = argv;
		if (sendsw() == FALSE) {	/* Send the file(s) */
			exitcode = 1;
			if(!sentabort)
				sendepkt("Remote requested abort");
			tranlog("  Send aborted.  File may be truncated.  ");
			tranlog(reason);
			tranlog("\n");
			if (debug)
				fprintf(errors, "Send failed.\n");      /* Report failure */
		} else {					/*  or */
			if (debug)
				fprintf(errors, "OK\n");                        /* success */
		}
		finish(exitcode);
	}

	if (rflg || Rflg) {		/* RECEIVE command */
		if(Rflg) {
			if(argc > 0) {
				write(fdtty, &esc, 1);
				write(fdtty, "&X", 2);
				write(fdtty, (image == TRUE) ? "iS" : "aS", 2);
			} else {
				finish(1);
			}
			while(argc--) {
				write(fdtty, *argv, strlen(*argv));
				if(argc)
					write(fdtty, " ", 1);
				++argv;
			}
			write(fdtty, "", 1);        /* terminate with a null */
			drain();
			sleep(2);
		}
		if (recsw() == FALSE) { /* receive the file */
			exitcode = 1;
			if (debug)
				fprintf(errors, "Receive failed.\n");   /* Report failure */
			tranlog("  Receive aborted.  File may be truncated.  ");
			tranlog(reason);
			tranlog("\n");
		} else {	/*  or */
			if (debug)
				fprintf(errors, "OK\n");        /* success */
		}
		finish(exitcode);
	}
}


usage(s)				/* Give message if user makes */
char	*s;
{					/* a mistake in the command */
	sendepkt("ctrmupdn usage error");
#ifdef UCB4X
	ioctl(fdtty, TIOCSETP, &ssave);
	ioctl(fdtty, TIOCSETC, &tchsave);
#endif
#ifdef USG
	ioctl(fdtty, TCSETAW, &ssave);
#endif
	if (debug) {
		fclose(errors);
	}
	fprintf(stderr,
	    "\nusage: %s [-s or -r or -S or -R][-i][-d][-B] [f1 f2 ...]\n",s);
	exit(1);
}


/*
 *	s e n d s w
 *
 *	Sendsw is the state table switcher for sending
 *	files.	It loops until either it finishes, or
 *	an error is encountered.  The routines called by
 *	sendsw are responsible for changing the state.
 *
 */

sendsw()
{
	char	sinit(), sfile(), seof(), sdata(), sbreak();

	state = 'S';                    /* Send initiate is the start state */
	n = 0;				/* Initialize message number */
	numtry = 0;			/* Say no tries yet */
	while (TRUE) {			/* Do this as long as necessary */
		switch (state) {
		case 'D':
			state = sdata();
			break; /* Data-Send state */
		case 'F':
			state = sfile();
			break; /* File-Send */
		case 'Z':
			state = seof();
			break; /* End-of-File */
		case 'S':
			state = sinit();
			break; /* Send-Init */
		case 'B':
			state = sbreak();
			break; /* Break-Send */
		case 'C':
			return (TRUE);		 /* Complete */
		case 'A':
			return (FALSE); 	 /* "Abort" */
		default:
			return (FALSE); 	 /* Unknown, fail */
		}
	}
}


/*
 *	s i n i t
 *
 *	Send Initiate: Send my parameters, get other side's back.
 */

char
sinit()
{
	int	num, len;			/* Packet number, length */
	char *s;

	if (debug)
		fprintf(errors, "sinit\n");
	if (numtry++ > MAXTRY) {
		reason = "too many tries";
		return('A');    /* If too many tries, give up */
	}
	spar(packet);				/* Fill up with init info */
	if (debug)
		fprintf(errors, "n = %d\n", n);

	Flushinput();				/* Flush pending input */

	spack('S', n, 13, packet);               /* Send an S packet */
	recptr = recpkt;
	switch (ptype = rpack(&len, &num, &recptr)) {	/* What was the reply? */
	case 'N':
		return(state);			/* NAK */

	case 'Y':                               /* ACK */
		if (n != num)
			return(state);	/* If wrong ACK, stay in S state */
		rpar(recptr,len);	/* Get other side's init info */
		switch(bctr) {
		case LRC:
		case CCITT:
			bctu = bctr;
			break;
		default:
			sendepkt("Can't support this error checking.");
			finish(1);
		}
		switch(bctu) {
		case LRC:
			s = "LRC";
			break;
		case CCITT:
			s = "CRC-CCITT";
			break;
		default:
			s = "???";
		}
		tranlog("8-bit quoting is %s; block checking is %s; packet size: %d\n", ebqflg ? "on":"off", s, spsize);
		RptProc = rptenabled(); /* is repeat char processing enabled? */
		if (rhdr.eol == 0)
			rhdr.eol = '\r';/* Check and set defaults */
		numtry = 0;		/* Reset try counter */
		n = bump(n);		/* Bump packet count */
		if (debug)
			fprintf(errors, "Opening %s\n", filnam);
#ifdef USG
try1:
		stat(filnam, &statbuf);
		if(statbuf.st_mode & 070000) {
			if (gnxtfl() == FALSE)		/* No more files go? */
				return('B');            /* if not, break, EOT, all done */
			else
				goto try1;
		}
#endif
		fp = fopen(filnam, "r");/* open the file to be sent */
		if (fp == NULL) {
			sendepkt("Can't open <%s>",filnam);
			reason = "can't open file";
			return('A');    /* if bad file descriptor, give up */
		}
		tranlog(" Sending %s%s\t%s\n", (*filnam == '/')?nullstr:pwd, filnam, ctrmtime());
		return('F');            /* OK, switch state to F */

	case FALSE:
		return(state);		/* Receive failure, stay in S state */

	default:
		sprintf(logbuf,"unexpected packet type, <%c>",ptype);
		reason = logbuf;
		return('A');            /* Some other packet type, "abort" */
	}
}


/*
 *	s f i l e
 *
 *	Send File Header.
 */

char
sfile()
{
	int	num, len;		/* Packet number, length */
	char	*s;

	if (debug)
		fprintf(errors, "sfile\n");

	if (numtry++ > MAXTRY) {
		reason = "too many tries";
		return('A');            /* If too many tries, give up */
	}
	for (len = 0, s = filnam; filnam[len] != '\0'; len++)
		if (filnam[len] == '/')
			s = &filnam[len] + 1;

	len = strlen(s);
	spack('F', n, len, s);                  /* Send an F packet */
	recptr = recpkt;
	switch (ptype = rpack(&len, &num, &recptr))	/* What was the reply? */ {
	case 'N':                               /* NAK, just stay in this state, */
		if (n != (num = (--num < 0) ? 63 : num))	/*  unless NAK for next packet, */
			return(state);		/*  which is just like an ACK */
		/*  for this packet, fall thru to... */
	case 'Y':                               /* ACK */
		if (n != num)
			return(state);		/* If wrong ACK, stay in F state */
		numtry = 0;			/* Reset try counter */
		n = bump(n);			/* Bump packet count */
		size = bufill(packet);		/* Get first data from file */
		return('D');                    /* Switch state to D */

	case FALSE:
		return(state);			/* Receive failure, stay in F state */

	default:
		sprintf(logbuf,"unexpected packet type, <%c>",ptype);
		reason = logbuf;
		return('A');            /* Some other packet type, "abort" */
	}
}


/*
 *	s d a t a
 *
 *	Send File Data
 */

char
sdata()
{
	int	num, len;			/* Packet number, length */

	if (debug)
		fprintf(errors, "sdata\n");
	if(cantsend8) {
		fclose(fp);
		sendepkt("Can't send binary file in text mode.");
		reason = "can't send binary file in text mode";
		return('A');
	}
	if (numtry++ > MAXTRY) {
		reason = "too many tries";
		return('A');                    /* If too many tries, give up */
	}
	spack('D', n, size, packet);            /* Send a D packet */

sdataagn:
	recptr = recpkt;
	switch (ptype = rpack(&len, &num, &recptr))	/* What was the reply? */ {
	case 'N':                               /* NAK, just stay in this state, */
		if (n != (num = (--num < 0) ? 63 : num))	/*  unless NAK for next packet, */
			return(state);		/*  which is just like an ACK */
		/*  for this packet, fall thru to... */
	case 'Y':                               /* ACK */
		if (n != num)
			goto sdataagn;		/* hope for the best */
		numtry = 0;			/* Reset try counter */
		n = bump(n);			/* Bump packet count */
		if ((size = bufill(packet)) == EOF) /* Get data from file */
			return('Z');            /* If EOF set state to that */
		return('D');                    /* Got data, stay in state D */

	case FALSE:
		return(state);			/* Receive failure, stay in D */

	default:
		sprintf(logbuf,"unexpected packet type, <%c>",ptype);
		reason = logbuf;
		return('A');            /* Some other packet type, "abort" */
	}
}


/*
 *	s e o f
 *
 *	Send End-Of-File.
 */

char
seof()
{
	int	num, len;			/* Packet number, length */
	if (debug)
		fprintf(errors, "seof\n");
	if (numtry++ > MAXTRY) {
		reason = "too many tries";
		return('A');                    /* If too many tries, "abort" */
	}
	spack('Z', n, 0, packet);               /* Send a 'Z' packet */
	if (debug)
		fprintf(errors, "seof1 ");
	recptr = recpkt;
	switch (ptype = rpack(&len, &num, &recptr))	/* What was the reply? */ {
	case 'N':                               /* NAK, fail */
		if (n != (num = (--num < 0) ? 63 : num))	/* ...unless for previous packet, */
			return(state);		/*  in which case, fall thru to ... */
	case 'Y':                               /* ACK */
		if (debug)
			fprintf(errors, "seof2 ");
		if (n != num)
			return(state);		/* If wrong ACK, hold out */
		numtry = 0;			/* Reset try counter */
		n = bump(n);			/* and bump packet count */
		if (debug)
			fprintf(errors, "closing %s, ", filnam);
		fclose(fp);			/* Close the input file */
		if (debug)
			fprintf(errors, "ok, getting next file\n");
		if (gnxtfl() == FALSE)		/* No more files go? */
			return('B');            /* if not, break, EOT, all done */
		if (debug)
			fprintf(errors, "new file is %s\n", filnam);
#ifdef USG
try2:
		stat(filnam, &statbuf);
		if(statbuf.st_mode & 070000) {
			if (gnxtfl() == FALSE)		/* No more files go? */
				return('B');            /* if not, break, EOT, all done */
			else
				goto try2;
		}
#endif
		fp = fopen(filnam, "r");        /* open the file to be sent */
		if (fp == NULL) {
			sendepkt("Can't open <%s>",filnam);
			reason = "can't open file";
			return('A');            /* if bad file descriptor, give up */
		}
		tranlog(" Sending %s%s\t%s\n", (*filnam == '/')?nullstr:pwd, filnam, ctrmtime());
		return('F');                    /* More files, switch state to F */

	case FALSE:
		return(state);			/* Receive failure, stay in state Z */

	default:
		sprintf(logbuf,"unexpected packet type, <%c>",ptype);
		reason = logbuf;
		return('A');            /* Some other packet type, "abort" */
	}
}


/*
 *	s b r e a k
 *
 *	Send Break (EOT)
 */

char
sbreak()
{
	int	num, len;			/* Packet number, length */
	if (debug)
		fprintf(errors, "sbreak\n");
	if (numtry++ > MAXTRY) {
		reason = "too many tries";
		return('A');                    /* If too many tries "abort" */
	}
	spack('B', n, 0, packet);               /* Send a B packet */
	recptr = recpkt;
	switch (ptype = rpack(&len, &num, &recptr))	/* What was the reply? */ {
	case 'N':                               /* NAK, fail */
		if (n != (num = (--num < 0) ? 63 : num))	/* ...unless for previous packet, */
			return(state);		/*  in which case, fall thru to ... */

	case 'Y':                               /* ACK */
		if (n != num)
			return(state);		/* If wrong ACK, fail */

		numtry = 0;			/* Reset try counter */
		n = bump(n);			/* and bump packet count */
		return('C');                    /* switch state to Complete */

	case FALSE:
		return(state);			/* Receive failure, stay in state B */

	default:
		sprintf(logbuf,"unexpected packet type, <%c>",ptype);
		reason = logbuf;
		return('A');            /* Some other packet type, "abort" */
	}
}


/*
 *	r e c s w
 *
 *	This is the state table switcher for receiving files.
 */

recsw()
{
	char	rinit(), rdata(), rfile();	/* Use these procedures */

	state = 'R';                            /* Receive is the start state */
	n = 0;					/* Initialize message number */
	numtry = 0;				/* Say no tries yet */

	while (TRUE)
		switch (state)			/* Do until done */ {
		case 'D':
			state = rdata();
			break;			/* Data receive state */
		case 'F':
			state = rfile();
			break;			/* File receive state */
		case 'R':
			state = rinit();
			break;			/* Send initiate state */
		case 'C':
			return(TRUE);		/* Complete state */
		case 'A':
			return(FALSE);		/* "Abort" state */
		}
}



/*
 *	r i n i t
 *
 *	Receive Initialization
 */

char
rinit()
{
	int	len, num;			/* Packet length, number */
	char *s;

	if (numtry++ > MAXTRY) {
		reason = "too many tries";
		return('A');                    /* If too many tries, "abort" */
	}
	recptr = packet;
	switch (ptype = rpack(&len, &num, &recptr))	 /* Get a packet */ {
	case 'S':                               /* Send-Init */
		if (debug)
			fprintf(errors, "received init packet\n");
		rpar(recptr,len);		/* Get the other side's init data */
		if(bctr != LRC && bctr != CCITT)
			bctr = LRC;
		spar(recptr);			/* Fill up packet with my init info */
		RptProc = rptenabled(); 	/* is repeat char processing enabled? */
		spack('Y', n, 13, recptr);       /* ACK with my parameters */
		switch(bctr) {
		case LRC:
		case CCITT:
			bctu = bctr;
			break;
		default:
			bctu = bctr = LRC;
		}
		switch(bctu) {
		case LRC:
			s = "LRC";
			break;
		case CCITT:
			s = "CRC-CCITT";
			break;
		default:
			s = "???";
		}
		tranlog("8-bit quoting is %s; block checking is %s; packet size: %d\n", ebqflg ? "on":"off", s, spsize);
		oldtry = numtry;		/* Save old try count */
		numtry = 0;			/* Start a new counter */
		n = bump(n);			/* Bump packet number, mod 64 */
		return('F');                    /* Enter File-Send state */

	case FALSE:
		Flushinput();
		spack('N', n, 0, 0);            /* send NAK */
		return (state); 		/* Didn't get a packet, keep waiting */

	default:
		sprintf(logbuf,"unexpected packet type, <%c>",ptype);
		reason = logbuf;
		return('A');            /* Some other packet type, "abort" */
	}
}


/*
 *	r f i l e
 *
 *	Receive File Header
 */

char
rfile()
{
	int	num, len;		/* Packet number, length */

	if (numtry++ > MAXTRY) {
		reason = "too many tries";
		return('A');    /* "abort" if too many tries */
	}
	recptr = packet;
	switch (ptype = rpack(&len, &num, &recptr))   /* Get a packet */ {
	case 'S':                       /* Send-Init, maybe our ACK lost */
		if (oldtry++ > MAXTRY) {
			reason = "too many tries";
			return('A');    /* If too many tries, "abort" */
		}
		if (num == ((n == 0) ? 63 : n - 1)) {	/* Previous packet, mod 64? */
			spar(recptr);	/* with our Send-Init parameters */
			spack('Y', num, 13, recptr);     /*  ... */
			numtry = 0;	/* Reset try counter */
			return(state);	/* Stay in this state */
		} else {
			reason = "out of synchronization";
			return('A');    /* Not previous packet, "abort" */
		}

	case 'Z':                       /* End-Of-File */
		if (oldtry++ > MAXTRY) {
			reason = "too many tries";
			return('A');    /* If too many tries, give up */
		}
		if (num == ((n == 0) ? 63 : n - 1)) {	/* Else check packet number */
			spack('Y', num, 13, recptr);     /* Yes, re-ACK it */
			numtry = 0;	/* Reset try counter */
			return(state);	/* Stay in D, don't write out data! */
		} else {
			reason = "out of synchronization";
			return('A');    /* sorry wrong number */
		}

	case 'F':                       /* File Header, */
		if (num != n) {
			reason = "out of synchronization";
			return('A');    /* which is what we really want */
		}
		/* The packet number must be right */
		if (!getfil(recptr)) {	/* Try to open a new file */
			if (debug)
				fprintf(errors, "Could not create %s\n", recptr);       /* Give up if can't */
			sendepkt("Can't open <%s>",recptr);
			reason = "can't open file";
			return('A');
		}
		tranlog(" Receiving %s%s\t%s\n", (*recptr == '/')?nullstr:pwd, recptr, ctrmtime());
		spack('Y', n, 0, 0);    /* Acknowledge the file header */
		oldtry = numtry;	/* Reset try counters */
		numtry = 0;
		n = bump(n);		/* Bump packet number, mod 64 */
		return('D');            /* Switch to Data state */

	case 'B':                       /* Break transmission (EOT) */
		/****if (num != n) return ('A');****/   /* Need right packet number here */
		spack('Y', n, 0, 0);    /* Say OK */
		return('C');            /* Go to complete state */

	case FALSE:
		Flushinput();
		spack('N', n, 0, 0);    /* send NAK */
		return(state);		/* Couldn't get packet, keep trying */
	default:
		sprintf(logbuf,"unexpected packet type, <%c>",ptype);
		reason = logbuf;
		return ('A');           /* Some other packet, "abort" */
	}
}


/*
 *	r d a t a
 *
 *	Receive Data
 */

char
rdata()
{
	int	num, len;		/* Packet number, length */

	if (numtry++ > MAXTRY) {
		reason = "too many tries";
		return('A');    /* "abort" if too many tries */
	}
	recptr = packet;
	switch (ptype = rpack(&len, &num, &recptr))	/* Get packet */ {
	case 'D':                               /* Got Data packet */
		if (num != n) { 		/* Right packet? */
			if (oldtry++ > MAXTRY) {
				reason = "too many tries";
				return('A');    /* If too many tries, give up */
			}
			if (num == ((n == 0) ? 63 : n - 1))	/* Else check packet number */ {				/* Previous packet again? */
				spack('Y', num, 6, recptr);       /* Yes, re-ACK it */
				numtry = 0;	/* Reset try counter */
				return(state);	/* Stay in D, don't write out data! */
			} else {
				reason = "out of synchronization";
				return('A');    /* sorry wrong number */
			}
		}
		/* Got data with right packet number */
		bufemp(recptr, fp, len);	/* Write the data to the file */
		spack('Y', n, 0, 0);            /* Acknowledge the packet */
		oldtry = numtry;		/* Reset the try counters */
		numtry = 0;
		n = bump(n);			/* Bump packet number, mod 64 */
		return('D');                    /* Remain in data state */

	case 'F':                               /* Got a File Header */
		if (oldtry++ > MAXTRY) {
			reason = "too many tries";
			return('A');            /* If too many tries, give up */
		}
		if (num == ((n == 0) ? 63 : n - 1))	/* Else check packet number */ {				/* Previous packet again? */
			spack('Y', num, 6, recptr);       /* Yes, re-ACK it */
			numtry = 0;		/* Reset try counter */
			return(state);		/* Stay in D, don't write out data! */
		} else {
			reason = "out of synchronization";
			return('A');            /* sorry wrong number */
		}

	case 'Z':                               /* End-Of-File */
		if (num != n) {
			reason = "out of synchronization";
			return('A');            /* Must have right packet number */
		}
		spack('Y', n, 0, 0);            /* OK, ACK it. */
		fclose(fp);			/* Close the file */
		n = bump(n);			/* Bump packet number */
		return('F');                    /* Go back to Receive File state */

	case FALSE:
		Flushinput();
		spack('N', n, 0, 0);            /* send NAK */
		return(state);		/* No packet came or bad packet came */
	default:
		sprintf(logbuf,"unexpected packet type, <%c>",ptype);
		reason = logbuf;
		return('A');            /* Some other packet, "abort" */
	}
}


/*
 *	KERMIT utilities.
 */

clkint()				/* Timer interrupt handler */
{
	longjmp(env, TRUE);		/* Tell rpack to give up */
}


/*
 *	s p a c k
 *
 *	Send a Packet
 */

spack(type, num, len, data)
char	type, *data;
int	num, len;
{
	int	i, j;			/* Character loop counter */
	char	buffer[20];		/* Checksum, packet buffer */
	register char	*bufp;		/* Buffer pointer */
	char	chksum, *p, *q;
	extern int	errno;

	if (debug) {
		fprintf(errors, "spack: type <%c>, num <%d>, len <%d>\n", type, num, len);
		fputs("\nWRITE PADDING <", fpdebug);
	}
	for (i = 1; i <= rhdr.npad; i++) {
		write(fdtty, &rhdr.padc, 1); /* Issue any padding */
		if(debug)
			fputc(rhdr.padc, fpdebug);
	}
	if(debug)
		fputs("> END PADDING\n", fpdebug);

	bufp = &buffer[0];		/* Set up buffer pointer */
	*bufp++ = SOH;			/* Packet marker, ASCII 1 (SOH) */
	q = bufp++;			/* save this spot */
	*bufp++ = tochar(num);		/* Packet number */
	*bufp++ = type; 		/* Packet type */
	j = len + bctu;			/* true length */
	if(j > rhdr.maxl - 2) {
		*q = tochar(0);		/* set LEN to 0 */
		*bufp++ = tochar(j/95);	/* high part of length */
		*bufp++ = tochar(j%95);	/* low part of length */
		*bufp++ = tochar(chk1(q,5,0));
	} else
		*q = tochar(j+2);	/* regular packet length */

	p = bufp;
	write(fdtty,buffer, bufp - &buffer[0]);
	write(fdtty, data, len);

	switch(bctu) {			/* compute checksum */
	case LRC:
		chksum = chk10(&buffer[1], bufp - &buffer[1]);
		*bufp++ = tochar(chk1(data,len,chksum));
		break;
	case CCITT:
		j = chk3(data,len,chk3(&buffer[1],bufp - &buffer[1],0));
		*bufp++ = tochar(((unsigned)(j & 0170000)) >> 12);
		*bufp++ = tochar((j & 07700) >> 6);
		*bufp++ = tochar(j & 077);
		break;
	}
	*bufp++ = rhdr.eol;		/* Extra-packet line terminator */
	if(debug) {
		fprintf(fpdebug,"len:%d, bctu:%d, num:%d\n",len,bctu,num);
		fputs("\nSEND BUFFER <", fpdebug);
		fwrite(buffer, p - &buffer[0], 1, fpdebug);
		fwrite(data, len, 1, fpdebug);
		fwrite(p, bufp - p, 1, fpdebug);
		fputs("> END SEND BUFFER\n", fpdebug);
		fflush(fpdebug);
	}
	if (write(fdtty, p, bufp - p) <= 0)   /* Send the packet */
		if (debug)
			fprintf(errors, "spack: write error, errno <%d>\n", errno);
	if (debug) {
		fprintf(errors, "spack: return\n");
		fflush(errors);
	}
}


/*
 *  r p a c k
 *
 *  Read a Packet
 *
 */

rpack(len, num, data)
int	*len, *num;				/* Packet length, number */
char	**data; 				/* Packet data */
{
	int	i, j, done;			/* Data character number, loop exit */
	char	t, u, type;		/* Checksum, current char, pkt type */
	char *odata, *tmp, *tptr, pbl;
	char xchk[3], chk[3];
	char badsum;
	char longpkt;

	odata = *data;
	drain();				/* let output drain */
	if (setjmp(env)) {
		sleep(2);			/* give things time to clear */
		return FALSE;			/* Timed out, fail */
	}
	do {					/* wait for packet header */
		t = lget_x_char(rhdr.time) & 0177;
	} while (t != SOH);

	done = FALSE;				/* Got SOH, init loop */
	badsum = 0;
	while (!done) { 			/* Loop to get a packet */
		tmp = tptr = odata;
		t = lget_x_char(rhdr.time) & 0177;
		if (t == SOH)
			continue;		/* Resynchronize if SOH */

		*tptr++ = t;
		j = *len = unchar(t);		/* Character count */
		if (debug)
			fprintf(errors, "have len <%d>\n", *len);

		t = lget_x_char(rhdr.time) & 0177;
		if (t == SOH)
			continue;		/* Resynchronize if SOH */
		*tptr++ = t;
		*num = unchar(t);		/* Packet number */
		if (debug)
			fprintf(errors, "have num <%d>\n", *num);

		t = lget_x_char(rhdr.time) & 0177;
		if (t == SOH)
			continue;		/* Resynchronize if SOH */
		*tptr++ = t;
		type = t;			/* Packet type */
		if (debug)
			fprintf(errors, "have type <%c>\n", type);
		longpkt = 0;
		if(j == 0) {			/* long packets? */
			t = lget_x_char(rhdr.time) & 0x7f;
			*tptr++ = t;
			*len = unchar(t) * (int)95;
			t = lget_x_char(rhdr.time) & 0x7f;
			*tptr++ = t;
			*len += unchar(t) - (int)bctu;
			t = lget_x_char(rhdr.time) & 0x7f;
			if(unchar(t) != chk1(odata, tptr - odata, 0)) {
				badsum = 1;
				break;
			}
			*tptr++ = t;
			longpkt = 1;
		}
		if((type == 'S') || (type == 'I'))
			pbl = LRC;
		else if(type == 'N')
			pbl = *len - 2;
		else
			pbl = bctu;

		if(!longpkt)
			*len -= pbl + 2;
		if (debug) {
			fprintf(errors, "error checking <%d>\n",pbl);
			fprintf(errors, "length of data <%d>\n", *len);
		}
		*data = tptr;
		for (i = 0; i < *len; i++) {	/* The data itself, if any */
			t = lget_x_char(rhdr.time);
			if (!image)
				t &= 0177;	/* Handle parity */
			*tptr++ = t;
		}
		*tptr = 0;	/* Mark the end of the data */
		if (debug)
			fprintf(errors, "have data\n");

		for(j = 0; j < pbl;) {	/* get the checksum that was sent */
			t = lget_x_char(rhdr.time) & 0177;
			if(t == SOH)
				continue;
			chk[j++] = t;
		}
		if (debug)
			fprintf(errors, "have chksum\n");
		lget_x_char(rhdr.time); 	/* get eol char */
		if (debug)
			fprintf(errors, "have eof\n");
		done = TRUE;			/* Got checksum, done */
	}
	alarm(0);				/* Disable the timer interrupt */
	switch(pbl) {
	case LRC:
		xchk[0] = tochar(chk1(odata, tptr - odata, 0));
		if(chk[0] != xchk[0])
			badsum = 1;
		break;
	case CCITT:
		j = chk3(odata, tptr - odata, 0);
		xchk[0] = tochar(((unsigned)(j & 0170000)) >> 12);
		xchk[1] = tochar((j & 07700) >> 6);
		xchk[2] = tochar(j & 077);
		if(xchk[0] != chk[0] || xchk[1] != chk[1] || xchk[2] != chk[2])
			badsum = 1;
		break;
	default:
		badsum = 1;
	}

	if (badsum) {
		if (debug) {
			fprintf(errors, "bad checksum:\n");
			for(j = 0; j < pbl; j++)
				fprintf(errors, "\txchk:%x\tchk:%x\n",xchk[j], chk[j]);
		}
		return(FALSE); /* Check the checksum, fail if bad */
	}

	if (debug)
		fprintf(errors, "rpack: returned good packet, type <%c>\n", type);
	return(type);				/* All OK, return packet type */
}



/*
 *	b u f i l l
 *
 *	Get a bufferful of data from the file that's being sent.
 *	Control-quoting, repeat count quoting, and 8-bit quoting are done;
 *	8-bit prefixes are not handled.
 */

int
bufill(buffer)
register char	*buffer;			/* Buffer */
{
	char	count;
	int	lasti;
	int i;
	register char	t7, t8;
	char t, tsave;

	cantsend8 = i = 0;					/* Init data buffer pointer */
	while(count = rdchars(&t, (RptProc ? MAXREPT : 1))) {	/* Get the next character */
		tsave = t;
		lasti = i;
		if(count > 1) {
			buffer[i++] = shdr.rept;/* quote char for rept count */
			buffer[i++] = tochar(count);
		}
		t7 = t & 0177;
		if((t8 = t & 0200) && !image)
			cantsend8 = TRUE;
		if(ebqflg && t8) {
			buffer[i++] = ebq;
			t = t7;
		}
		if(!image && t == NL) {
			buffer[i++] = shdr.qctl;
			buffer[i++] = ctl(CR);
		}
		if(t7 < SP || t7 == DEL) {
			buffer[i++] = shdr.qctl;
			t = ctl(t);
		}
		if(t7 == shdr.qctl)
			buffer[i++] = shdr.qctl;
		if(RptProc && t7 == shdr.rept)
			buffer[i++] = shdr.qctl;
		if(ebqflg && t7 == ebq)
			buffer[i++] = shdr.qctl;
		buffer[i++] = t;	/* Deposit the character itself */
		if (i >= spsize - 7 - (bctu - 1)) {
			if(i > spsize - 7 - (bctu - 1)) {
				i = lasti;
				unrdchars(tsave, count);
			}
			return((int)i); /* Check length */
		}
	}
	return((i == 0) ? EOF : i);
}


/*
 *	b u f e m p
 *
 *	Get data from an incoming packet into a file.
 */

bufemp(buffer, fp, len)
register char	*buffer;		/* Buffer */
int	len;				/* File pointer, length */
FILE *fp;
{
	char count;
	register int	i;		/* Counter */
	register char	t;		/* Character holder */
	char t7, t8;

	for (i = 0; i < len; i++)	/* Loop thru the data field */ {
		count = 1;
		t = buffer[i];		/* Get character */
		if(t == rhdr.rept && RptProc) { /* repeated char? */
			count = unchar(buffer[++i]);
			t = buffer[++i];
		}
		t8 = 0;
		if(ebqflg) {
			if(t == ebq) {
				t8 = 0200;
				t = buffer[++i];
			}
		}
		if (t == rhdr.qctl) {	/* Control quote? */
			t = buffer[++i];/* Get the quoted character */
			t7 = t & 0177;
			if((t7 >= 0100 && t7 <= 0137) || t7 == '?')
				t = ctl(t);
		}
		t |= t8;
		if (image || (t != CR)) /* Don't pass CR in text mode */
			do {
				putc(t, fp);
			} while(--count);
	}
}


/*
 *	r d c h a r s
 *
 *	Read multiple characters.
 */
int
rdchars(ch, maxcount)
char *ch, maxcount;
{
	int firstch, nextch;
	char count;

	if(holdcount) {
		*ch = savech;
		count = min(holdcount, maxcount);
		holdcount -= count;
		return(count);
	}
	count = 0;
	if((firstch = getc(fp)) != EOF) {
		*ch = firstch;
		if(maxcount == 1 || (firstch == NL && !image))
			return(1);
		++count;
		while(count < maxcount && ((nextch = getc(fp)) == firstch))
			++count;
		if(nextch != firstch && nextch != EOF)
			ungetc(nextch,fp);
	}
	if(count == 2) {	/* don't pass back counts of 2 */
		holdcount = count - 1;
		savech = firstch;
		return(1);
	}
	return(count);
}

/*
 *  u n r d c h a r s
 *
 *  Put back one character and read it again with the next rdchars().
 */
unrdchars(c,count)
char c;
char count;
{
	holdcount += count;
	savech = c;
}


/*
 *	g e t f i l
 *
 *	Open a new file
 */

getfil(filenm)				/* Bizarre logic, as this is called */
char	*filenm;			/* only with packet as the arg! */
{
	if (filenm[0] == '\0')
		fp = fopen(packet, "w");/* If filename known, use it */
	else
		fp = fopen(filenm, "w");/* else use sourcefile name */
	return (fp != NULL);		/* Return false if file won't open */
}


/*
 *	g n x t f l
 *
 *	Get next file in a file group
 */

gnxtfl()
{
	if (debug)
		fprintf(errors, "gnxtfl\n");
	filnam = *(filelist++);
	if (filnam == 0)
		return FALSE;	/* If no more, fail */
	else
		return TRUE;	/* else succeed */
}


/*
 *	s p a r
 *
 *	Fill the data array with my send-init parameters
 *
 */

spar(data)
register char	*data;
{
	data[0] = tochar(shdr.maxl);	      /* Biggest packet I can receive */
	data[1] = tochar(shdr.time);	      /* When I want to be timed out */
	data[2] = tochar(shdr.npad);	      /* How much padding I need */
	data[3] = ctl(shdr.padc);	      /* Padding character I want */
	data[4] = tochar(shdr.eol);	      /* End-Of-Line character I want */
	data[5] = shdr.qctl;		      /* Control-Quote character I send */
	data[6] = 'Y';                          /* I'm willing to do 8-bit quoting */
	data[7] = shdr.chkt = bctr + '0';       /* bcc */
	data[8] = shdr.rept;			/* repeat prefix char */
	data[9] = tochar(Lpcapr?LONGPACKETS:0);	/* long packet capability */
	data[10] = tochar(0);			/* no window capability */
	data[11] = tochar(rpsize/95);		/* long packet sizes */
	data[12] = tochar(rpsize%95);
}


/*	r p a r
 *
 *	Get the other host's send-init parameters
 *
 */

rpar(data,len)
register char	*data;
register int	len;
{
	int x;
	int i;

	spsize = rhdr.maxl = unchar(data[0]);	      /* Maximum send packet size */
	rhdr.time = unchar(data[1]);	      /* When I should time out */
	if(rhdr.time > MAXTIM || rhdr.time < MINTIM)
		rhdr.time = MYTIME;
	rhdr.npad = unchar(data[2]);	      /* Number of pads to send */
	rhdr.padc = ctl(data[3]);	      /* Padding character to send */
	rhdr.eol = unchar(data[4]);	      /* EOL character I must send */
	rhdr.qctl = data[5];		      /* Incoming data quote character */
	if(len>=7) {
		ebq = rhdr.qbin = data[6];	/* 8-bit quote char */
		if((ebq > 040 && ebq < 0100) || (ebq > 0140 && ebq < 0177)) {
			ebqflg = 1;
		} else
			ebqflg = 0;
	}
	if(len>=8) {
		rhdr.chkt = data[7];		/* bcc */
		bctr = rhdr.chkt - '0';
	} else
		bctr = LRC;
	if(len>=9)
		rhdr.rept = data[8];		/* prefix for repeat char */
	if(len >= 10) {
		x = unchar(data[9]);		/* capabilities byte */
		lpcapu = (x & LONGPACKETS);
		for(i = 9; (unchar(data[i]) & 1) && (len > i); ++i)
			;
		if(lpcapu) {
			x = unchar(data[i+2]) * 95 + unchar(data[i+3]);
			spsize = (x > rpsize)?rpsize:x;
			Lpcapr = 1;
		}
	}
	shdr.rept = rhdr.rept;
	if(!ebqflg && image)
		set8bits();
}


Flushinput()
{
#ifdef UCB4X
	/* 4.2 BSD */
	long count; /* Number of bytes ready to read. */
	long i; /* Number of bytes to read in loop. */
#endif

	if (debug) {
		fprintf(errors, "Flushinput()\n");
	}
#ifdef USG
	ioctl(fdtty, TCFLSH, 0);
#endif	/* USG */
#ifdef UCB4X
	ioctl(fdtty, FIONREAD, &count);
	if (!count) return;
	while(count) {	 /* Loop till all are flushed. */
		i = (count < sizeof(recpkt)) ?	 /* Read min of count and size of */
			count : sizeof(recpkt);      /* the read buffer. */
		read(fdtty, recpkt, i);  /* Read a bunch. */
		count -= i;
	}
#endif
	Incount = 0;
}


get_x_char(timeout)
int	timeout;
{
	static char	buf[1024];
	static char	*bufptr;
	extern int	errno;
	int	ret;

	while(1) {
		if(Incount-- > 0) {
			if(debug) {
				fputc(*bufptr, fpdebug);
			}
			return((int)(*bufptr++ & 0xff));
		}
		alarm(timeout);
		alarmcalled = 0;
		if ((Incount = read(fdtty, buf, sizeof(buf))) <= 0 || alarmcalled != 0) {
			if (debug) {
				fprintf(errors, "get_x_char returning read error\n");
				fprintf(errors, "errno == %d\n", errno);
				fprintf(errors, "timeout == %d\n", timeout);
				fprintf(errors, "Incount == %d\n", Incount);
				fprintf(fpdebug,"\nREAD TIMEOUT, errno:%d>\n",errno);
			}
			return(-1);
		}
		bufptr = &buf[0];
		alarm(0);
	}
}


int
lget_x_char(timeout)
int	timeout;
{
	int	t;

	if ((t = get_x_char(timeout)) < 0)
		longjmp(env, TRUE);
	if(!ws8 || ebqflg)
		t &= 0177;
	return(t);
}


void
cleanup(x)
int	x;
{
#ifdef UCB4X
	ioctl(fdtty, TIOCSETP, &ssave);
	ioctl(fdtty, TIOCSETC, &tchsave);
#endif
#ifdef USG
	ioctl(fdtty, TCSETAW, &ssave);
#endif
	if (debug) {
		fprintf(errors, "caught signal %d\n", x);
		fclose(errors);
		fprintf(fpdebug, "\ncaught signal %d\n", x);
		fclose(fpdebug);
	}
	if(fptranlog != NULL) {
		tranlog("Transactions aborted:\t%s", ctrmdate());
		tranlog("	caught signal <%d>\n",x);
		fclose(fptranlog);
	}
	exit(1);
}


finish(x)
int	x;
{
#ifdef UCB4X
	ioctl(fdtty, TIOCSETP, &ssave);
	ioctl(fdtty, TIOCSETC, &tchsave);
#endif
#ifdef USG
	ioctl(fdtty, TCSETAW, &ssave);
#endif
	if (debug) {
		fprintf(errors, "exit %d\n", x);
		fclose(errors);
		fprintf(fpdebug, "\nexit %d\n", x);
		fclose(fpdebug);
	}
	if(fptranlog != NULL) {
		tranlog("Transactions ended:\t%s", ctrmdate());
		fclose(fptranlog);
	}
	exit(x);
}

void
alarmfn()
{
	void alarmfn();

	signal(SIGALRM, alarmfn);
	alarmcalled = 1;
}

/*
 *	sendepkt()
 *
 *	send an error packet
 */
sendepkt(s,s1,s2,s3,s4,s5,s6)
char *s,*s1,*s2,*s3,*s4,*s5,*s6;
{
	int n;
	char buf[100];

	sprintf(buf,s,s1,s2,s3,s4,s5,s6);
	n = strlen(buf);
	spack('E', 0, n, buf);          /* Send an E packet */
	sentabort = TRUE;
}


/*
 *	initfile
 *
 *	Valid commands in a .ctrmrc file are:
 *
 *		#	- a '#' in column 1 causes the rest of line to be
 *			  ignored
 *
 *		log transactions <filename>
 *			-appends logging information to the file you specify
 *
 *		set debug
 *			- creates two files on the UNIX end with debugging
 *			  data, CTRM.DEB and xk.err
 *
 *		set blockcheck lrc
 *			- normally ctrmkrmt will attempt to tell CTRM that
 *			  it is willing to use CRC-CCITT block checking.  Older
 *			  CTRMs (prior to 1.10) abort when this happens.
 *			  This line in .ctrmrc forces ctrmkrmt to use LRC
 *			  block checking (which all Kermits can use).
 *
 *		set packet <size>
 *			- the default packet size to use on downloads
 *			  is the extended (up to 8K bytes) packet size.
 *			  If this option is set, ctrmkrmt will try to
 *			  use the requested size packets for transfers.
 *
 */
initfile()
{
	char buf[80], s1[50], s2[50], s3[50], *home;
	int i;
	FILE *fp, *popen();

	if((fp = fopen(ctrmrc,"r")) == NULL) {  /* look in current directory */
		if((home = getenv("HOME")) != NULL) {
			strcpy(buf, home);
			strcat(buf, "/");
			strcat(buf, ctrmrc);
			fp = fopen(buf, "r");
		}
	}
	if(fp != NULL) {
		while(fgets(buf, sizeof(buf), fp) != NULL) {
			if(buf[0] == '#')
				continue;
			buf[strlen(buf) - 1] = '\0';    /* get rid of the \n */
			i = sscanf(buf,"%s %s %s",s1,s2,s3);
			switch(i) {
			case 3: /* look for 'log transactions <name>' */
				if(equal(s1,"log")) {
					if(equal(s2,"transactions"))
						fptranlog = fopen(s3,"a");
				/* look for 'set blockcheck lrc or crc' */
				} else if(equal(s1, "set")) {
					if(equal(s2,"blockcheck") && equal(s3,"crc"))
						bctr = CCITT;
					else if(equal(s2,"blockcheck") && equal(s3,"lrc")) {
						bctr = LRC;
					} else if(equal(s2, "packet")) {
						rpsize = atol(s3);
					}
				}
				break;
			case 2: 	/* look for 'set debug' */
				if(equal(s1,"set"))
					if(equal(s2,"debug"))
						debug = 1;
				break;
			default:	/* ignore anything else for now */
				break;
			}
		}
		fclose(fp);
		if(fptranlog != NULL) {
			if((fp = popen("pwd", "r")) != NULL) {
				fgets(pwd, sizeof(pwd), fp);
				pwd[strlen(pwd) - 1] = '/';
				pclose(fp);
			}
		}
	}
}


tranlog(s,s1,s2,s3,s4)
char *s;
unsigned long s1, s2, s3, s4;
{
	if(fptranlog == NULL)
		return;
	fprintf(fptranlog,s,s1,s2,s3,s4);
}


char *
ctrmdate()
{
	long t, time();

	t = time((long *)0);
	return(ctime(&t));
}

char *
ctrmtime()
{
	static char time[9];

	sscanf(ctrmdate(),"%*s %*s %*s %s",time);
	return(time);
}

set8bits()
{
#ifdef USG
	snew.c_cflag &= ~PARENB;
	snew.c_cflag |= CS8;
	snew.c_cflag &= ~CSTOPB;
	snew.c_iflag &= ~ISTRIP;
	ioctl(fdtty, TCSETAW, &snew);
	ws8 = cs8(snew);		/* see whether we need 8-bit quoting */
#endif
#ifdef UCB4X
	ws8 = TRUE;
#endif
}


#ifdef UCB4X
#define ERR(s, c)	if(opterr){\
	fputs(argv[0], stderr);\
	fputs(s, stderr);\
	fputc(c, stderr);\
	fputc('\n', stderr);}

extern int strcmp();
extern char *index();

int	opterr = 1;
int	optind = 1;
int	optopt;
char	*optarg;

int
getopt(argc, argv, opts)
int	argc;
char	**argv, *opts;
{
	static int sp = 1;
	register int c;
	register char *cp;

	if(sp == 1)
		if(optind >= argc ||
		   argv[optind][0] != '-' || argv[optind][1] == '\0')
			return(EOF);
		else if(strcmp(argv[optind], "--") == NULL) {
			optind++;
			return(EOF);
		}
	optopt = c = argv[optind][sp];
	if(c == ':' || (cp=index(opts, c)) == NULL) {
		ERR(": illegal option -- ", c);
		if(argv[optind][++sp] == '\0') {
			optind++;
			sp = 1;
		}
		return('?');
	}
	if(*++cp == ':') {
		if(argv[optind][sp+1] != '\0')
			optarg = &argv[optind++][sp+1];
		else if(++optind >= argc) {
			ERR(": option requires an argument -- ", c);
			sp = 1;
			return('?');
		} else
			optarg = argv[optind++];
		sp = 1;
	} else {
		if(argv[optind][++sp] == '\0') {
			sp = 1;
			optind++;
		}
		optarg = NULL;
	}
	return(c);
}
#endif	/* UCB4X */


/* Set up port */
int
settty() {
#ifdef UCB4X
	snew.sg_flags |= (RAW|TANDEM);
	snew.sg_flags &= ~(ECHO|CRMOD);
	ioctl(fdtty, TIOCSETP, &snew);
	tchnew.t_startc = '\021';
	tchnew.t_stopc = '\023';
	ioctl(fdtty, TIOCSETC, &tchnew);
#endif
#ifdef USG
	snew.c_iflag = 0;
	snew.c_oflag = 0;
	snew.c_iflag |= ISTRIP | BRKINT | IGNPAR | IXOFF | IXON;
	snew.c_lflag &= ~(ICANON | ECHO);
	snew.c_cc[0] = 5;		/* VINTR	*/
	snew.c_cc[1] = 5;		/* VQUIT	*/
	snew.c_cc[7] = 6;		/* SWTCH	*/
	snew.c_cc[4] = 1;
	snew.c_cc[5] = CTIMEOUT;
	ioctl(fdtty, TCSETAW, &snew);
#endif
}


drain()
{
#ifdef USG
	ioctl(fdtty, TCSBRK, 1);	/* let output drain */
#endif
#ifdef UCB4X
	ioctl(fdtty, TIOCSETP, &snew);
#endif
}

char
chk10(line,n)
char *line;
int n;
{
	char sum;
	int m;

	m = n;
	sum = 0;
	while(n--) {
		sum += *line;
		++line;
	}
	return(sum);
}

chk1(line,n,chksum)
char *line, chksum;
int n;
{
	while(n--) {
		chksum += *line;
		++line;
	}
	chksum = ((chksum + (chksum & 0300) / 64) & 077);  /* Compute final checksum */
	if (debug)
		fprintf(errors, "LRC: 0x%x\n",chksum);
	return((int)chksum);
}

static int ccitt[] = {
	0x0000, 0x1081, 0x2102, 0x3183, 0x4204, 0x5285, 0x6306, 0x7387,
	0x8408, 0x9489, 0xA50A, 0xB58B, 0xC60C, 0xD68D, 0xE70E, 0xF78F
};

unsigned int
chk3(line,n,crc)
register char *line ;
int n;
register unsigned int crc;
{
    unsigned int q;
    register char c;

    while(n--) {
	c = *line++;
	q = (crc ^ c) & 0x0f;
	crc = (crc >> 4) ^ ccitt[q];
	q = (crc ^ (c >> 4)) & 0x0f;
	crc = (crc >> 4) ^ ccitt[q];
    }
    if (debug)
	fprintf(errors, "CRC-CCITT: 0x%x\n",crc);
    return(crc) ;
}
