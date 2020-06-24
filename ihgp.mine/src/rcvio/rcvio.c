/*
 * program to communicate with a 5ess switch via a rc/v or
 * a unix port.
 */

/* system headers */
#include <stdio.h>
#include <string.h>
#include <termio.h>
#include <signal.h>
#include <fcntl.h>
#include <malloc.h>

/* externs */
extern int errno, sys_nerr;
extern char *sys_errlist[];
extern char *optarg;
extern int optind, opterr;

/* global variables */
struct termio ostdinbuf;	/* original state of stdin */
struct termio ottyfdbuf;	/* original state of port */
int Reset;		/* reset port state flag */
int gttyfd;		/* port file descriptor */
int timeout;		/* timeout flag for trap */
int ackflag;		/* ack flag for trap */
int write2out;		/* write to stdout flag, used for throttling */

/* local definitions */
#define USAGE "usage: rcvio [-?] [-D] [-s speed] [-m characters] [-t timeout] device"
#define RCVTRUE 1
#define RCVFALSE 0
#define RCVOK 0
#define RCVNOTOK -1
#define RCVNOTTTY -2
#define RCVTMOUT -3
#define RCVNOACK -4
#define RCVWRITE 0
#define RCVNOWRITE 1
#define RCVFLUSH 2
#define TIMEOUT 10
#define RCVCNTRLY 031
#define RCVESCAPE 033
#define RCVDELETE 0177
#define RCVNL '\n'
#define NULLCHR '\0'
#define NULLPTR 0
#define FIVESEC ((unsigned) 5)

/* error handling */
void syserr(errmsg, prtusage)
char *errmsg;
int prtusage;
{
	if (*errmsg != '\0') fprintf(stderr, "%s\n", errmsg);
	if (errno > 0 && errno < sys_nerr)
	{
		fprintf(stderr, "error %d : %s\n", errno, sys_errlist[errno]);
	}
	if (prtusage) fprintf(stderr, "%s\n", USAGE);
	return;
}

/* trap function for acknowledgements */
#ifdef VAX
void ack_trap()
#else
int ack_trap()
#endif
{
	/* ack has been received */
	ackflag = RCVTRUE;
	return;
}

/* trap function for timeouts */
#ifdef VAX
void tmout_trap()
#else
int tmout_trap()
#endif
{
	/* time out has occurred, set flag */
	timeout = RCVTRUE;
	return;
}

/* trap function for user signals */
#ifdef VAX
void user_trap()
#else
int user_trap()
#endif
{
	int kill(), getppid();

	/* reset trap */
	(void) signal(SIGUSR1, user_trap);

	/* toggle flag for writing to stdout */
	switch (write2out)
	{
	case RCVWRITE:
		write2out = RCVNOWRITE;
		break;
	case RCVNOWRITE:
		write2out = RCVFLUSH;
		break;
	default:
		exit(2);
	}

	/* send back acknowledgement */
	(void) kill(getppid(), SIGUSR1);

	/* return */
	return;
}

/* trap function for death of child */
#ifdef VAX
void child_trap()
#else
int child_trap()
#endif
{
	/* print error msg */
	fprintf(stderr, "child process has died.\n");

	/* reset terminals */
	(void) ioctl(0, TCSETAW, &ostdinbuf);
	if (Reset) (void) ioctl(gttyfd, TCSETAW, &ottyfdbuf);

	/* close tty */
	(void) close(gttyfd);

	/* exit */
	exit(2);
}

/* verify if a character string is numeric */
int isnumeric(pstr)
char *pstr;
{
	for ( ; ('0' <= *pstr) && (*pstr <= '9'); pstr++)
	{
		/* do nothing */
	}
	if (*pstr != '\0') return(RCVFALSE);
	return(RCVTRUE);
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
	timeout = RCVFALSE;

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
	if (timeout == RCVTRUE) return(RCVTMOUT);

	/* other failures ? */
	if (ttyfd < 0) return(RCVNOTOK);

	/* is devicename a tty ? */
	if ( ! isatty(ttyfd))
	{
		/* not a tty */
		(void) close(ttyfd);
		return(RCVNOTTTY);
	}

	/* everything is ok, return fd */
	return(ttyfd);
}

/* send a signal to a process and wait for an acknowledgement */
int throttle(pid, waittime, sig)
int pid, waittime, sig;
{
	int status;
	unsigned sleep();
#ifdef VAX
	void (*oldsigfn)();
#else
	int (*oldsigfn)();
#endif

	/* assume success, be optimistic */
	status = RCVOK;

	/* set flag for ack as not true */
	ackflag = RCVFALSE;

	/* save trap for given signal */
	oldsigfn = signal(sig, ack_trap);

	/* send signal and sleep */
	if (kill(pid, sig) < 0) return(RCVNOTOK);
	(void) sleep((unsigned) waittime);

	/* check if response was received */
	if (ackflag == RCVFALSE) status = RCVNOACK;

	/* reset previous trap */
	(void) signal(sig, oldsigfn);

	/* return status */
	return(status);
}

/* exec a command to a shell */
int callshell(ttyfd, outpid)
int ttyfd, outpid;
{
	struct termio stdinbuf;
	char cmdbuf[BUFSIZ + 1];
	register char *pstr;
	int throttle();
#ifdef VAX
	void (*oldsigfn)();
#else
	int (*oldsigfn)();
#endif
#ifdef DEBUG
	FILE *dbgfd;
#endif

#ifdef DEBUG
	dbgfd = fopen("/tmp/prcvio.dbg", "a");
#endif
	/* remove death-of-child trap */
	oldsigfn = signal(SIGCLD, SIG_DFL);

	/* send signal to throttle output to stdout */
	if (throttle(outpid, FIVESEC, SIGUSR1) < 0) return(RCVNOTOK);

	/* halt all output to stdout */
	(void) ioctl(1, TCXONC, 0);

	/* discard (flush) anything in input queue */
	(void) ioctl(0, TCFLSH, 0);

	/* discard (flush) anything in output queue */
	(void) ioctl(1, TCFLSH, 1);

	/* restart output to stdout */
	(void) ioctl(1, TCXONC, 1);

	/* save stdin raw mode state */
	if (ioctl(0, TCGETA, &stdinbuf) < 0) return(RCVNOTOK);

	/* reset stdin to original state */
	if (ioctl(0, TCSETA, &ostdinbuf) < 0) return(RCVNOTOK);

	/* read in a line from stdin and exec */
	(void) fprintf(stdout,"\n!");
	(void) fflush(stdout);
	cmdbuf[0] = NULLCHR;
	(void) fgets(cmdbuf, BUFSIZ, stdin);
	cmdbuf[BUFSIZ] = NULLCHR;

	/* exec the input msg */
#ifdef DEBUG
	fprintf(dbgfd, "callshell: system cmd is %s.\n", cmdbuf);
	fflush(dbgfd);
#endif
	(void) system(cmdbuf);
	(void) fprintf(stdout,"\n");
	(void) fflush(stdout);
#ifdef DEBUG
	fprintf(dbgfd, "callshell: after system call.\n");
	fflush(dbgfd);
#endif

	/* discard (flush) anything in input queue */
	(void) ioctl(0, TCFLSH, 0);

	/* reset death-of-child trap */
	(void) signal(SIGCLD, oldsigfn);

	/* reset stdin to raw mode */
	if (ioctl(0, TCSETAW, &stdinbuf) < 0) return(RCVNOTOK);

	/* send signal to restart output to stdout */
	if (throttle(outpid, FIVESEC, SIGUSR1) < 0) return(RCVNOTOK);
#ifdef DEBUG
	(void) fclose(dbgfd);
#endif

	/* return */
	return(RCVOK);
}

/* function to read stdin and write to external port */
int in2tty(ttyfd, outpid)
int ttyfd, outpid;
{
	unsigned char c;

	for ( ; ; )
	{
		if (read(0, &c, 1) < 0) return(RCVNOTOK);
		if (c == RCVESCAPE)
		{
			/* escape mode, input for rcvio */
			if (read(0, &c, 1) < 0) return(RCVNOTOK);

			/* determine the type of input */
			switch (c)
			{
			case RCVCNTRLY:
			case RCVDELETE:
				/* exit with these keys when in escape mode */
				goto done;

			case RCVESCAPE:
				/* send the 2nd escape to ttyfd */
				break;

			case '!':
				/* escape to shell and execute a command.  */
				if (callshell(ttyfd, outpid) != RCVOK) return(RCVNOTOK);
				continue;

			default:
				 /* ignore all other input */
				continue;
			}
		}
		if (write(ttyfd, &c, 1) < 0) return(RCVNOTOK);
	}
done:
	return(RCVOK);
}

/* function to read external port and write to stdout */
void tty2out(ttyfd, ttymin)
int ttyfd, ttymin;
{
	register unsigned char *pbuf;
	register bytesread;
	char *malloc();
#ifdef VAX
	void user_trap();
#else
	int user_trap();
#endif

	/* malloc the buffer */
	if (ttymin > 0)
	{
		pbuf = (unsigned char *) malloc((unsigned) ttymin);
		if (pbuf == (unsigned char *) NULL) exit(2);
	}
	else
	{
		exit(2);
	}

	/* set flag for writing to stdout */
	write2out = RCVWRITE;

	/* 
	 * set trap for catching user signals, used 
	 * for throttling output to stdout. the flag
	 * write2out is toggled.
	 */
	(void) signal(SIGUSR1, user_trap);

	/* read from tty and write to stdout */
	for ( ; ; )
	{
		bytesread = read(ttyfd, pbuf, ttymin);
		switch (write2out)
		{
		case RCVWRITE:
			write(1, pbuf, bytesread);
			break;
		case RCVNOWRITE:
			break;
		case RCVFLUSH:
			write2out = RCVWRITE;
			(void) ioctl(ttyfd, TCFLSH, 1);
			break;
		default:
			exit(2);
		}
	}

	/* NEVER RETURNS */
	exit(2);
}

/* function to communicate with external port */
int rcvio(ttyfd, devname, ttyspeed, ttymin, ttytime)
int ttyfd;
char *devname;
int ttyspeed, ttymin, ttytime;
{
#ifdef VAX
	void (*osigintr)();
	void (*osigquit)();
	void (*osigcld)();
	void child_trap();
#else
	int (*osigintr)();
	int (*osigquit)();
	int (*osigcld)();
	int child_trap();
#endif
	int status, in2tty(), pid;
	void tty2out();

	/* ignore standard interrupt signals */
	osigintr = signal(SIGINT, SIG_IGN);
	osigquit = signal(SIGQUIT, SIG_IGN);

	/* 
	 * save current terminal and device states
	 * in global variables so that trap functions can use
	 * them to reset the terminals.
	 */
	if (ioctl(0, TCGETA, &ostdinbuf) < 0) return(RCVNOTOK);
	if (ioctl(ttyfd, TCGETA, &ottyfdbuf) < 0) return(RCVNOTOK);

	/* starting connection */
	fprintf(stdout, "connecting to %s ...\n", devname);

	/* set stdin and ttydev to raw mode, keep xon/xoff active */
	if ((set_raw(0, 0, 1, 0) < 0) || 
		(set_raw(ttyfd, ttyspeed, ttymin, ttytime) < 0))
	{
		/* reset old states */
		 (void) ioctl(0, TCSETAW, &ostdinbuf);
		 if (Reset) (void) ioctl(ttyfd, TCSETAW, &ottyfdbuf);
		/* restore signal handling */
		(void) signal(SIGINT, osigintr);
		(void) signal(SIGQUIT, osigquit);
		/* return */
		return(RCVNOTOK);
	}

	/* 
	 * fork to create two processes. the parent process
	 * reads stdin and writes to ttyfd. the child 
	 * process reads ttyfd and writes to stdout.
	 */
	if ((pid = fork()) < 0) return(RCVNOTOK);

	/* depending on which process, call a different function */
	if (pid > 0)
	{
		/* set function to catch death-of-child signal */
		osigcld = signal(SIGCLD, child_trap);
		/* parent process, read stdin, write ttyfd */
		status = in2tty(ttyfd, pid);
		/* reset old death-of-child handling */
		(void) signal(SIGCLD, osigcld);
	}
	else
	{
		/* child process, read ttyfd, write stdout */
		(void) tty2out(ttyfd, ttymin); /* NEVER RETURNS */
	}

	/* kill child process */
	kill(pid, SIGTERM);
	wait((int *) 0);

	/* reset old states */
	(void) ioctl(0, TCSETAW, &ostdinbuf);
	if (Reset) (void) ioctl(ttyfd, TCSETAW, &ottyfdbuf);

	/* restore signal handling */
	(void) signal(SIGINT, osigintr);
	(void) signal(SIGQUIT, osigquit);

	/* return status */
	return(status);
}

/* function to set a device to raw mode */
int set_raw(fd, speed, min, time)
int fd, speed, min, time;
{
	struct termio tbuff;

	/* check that fd points to a terminal */
	if (! isatty(fd)) return(RCVNOTTTY);

	/* get current state of terminal */
	if (ioctl(fd, TCGETA, &tbuff) < 0) return(RCVNOTOK);

	/* change characteristics to raw mode with xon/xoff */
	tbuff.c_iflag = (IXON|IXOFF);
	tbuff.c_oflag = 0;
	tbuff.c_lflag = 0;

	/* set baud rate */
	if (speed > 0)
	{
		tbuff.c_cflag &= ~CBAUD;
		tbuff.c_cflag |= speed;
	}

	/* set minimum number of characters and timeout value */
	tbuff.c_cc[VMIN] = min;
	tbuff.c_cc[VTIME] = time;

	/* reset terminal */
	return(ioctl(fd, TCSETAW, &tbuff));
}

/* convert speed to enum for baud rate */
int getspeed(pbaud)
char *pbaud;
{
	int speed;

	switch(atoi(pbaud))
	{
	case 300:
		speed = B300;
		break;
	case 1200:
		speed = B1200;
		break;
	case 2400:
		speed = B2400;
		break;
	case 4800:
		speed = B4800;
		break;
	case 9600:
		speed = B9600;
		break;
	default:
		speed = RCVNOTOK;
		break;
	}
	return(speed);
}

/* main program */
main(argc, argv)
int argc;
char *argv[];
{
	int c, status, ttyfd, getopt(), getspeed();
	int ttyspeed, ttymin, ttytime;
	char devname[BUFSIZ];

	/* default values for reading tty */
	ttyspeed = 0;		/* no change in baud rate */
	ttymin = 1;				/* 1 character at a time */
	ttytime = 0;		/* return immediate, no wait for input */
	Reset = RCVTRUE;

	/* check that a port was given */
	if (argc <= 1)
	{
		syserr("no device name was given.", RCVTRUE);
		exit(2);
	}

	/* get options, like baud rate */
	while ((c = getopt(argc, argv, "?Ds:m:t:")) != EOF)
	{
		/* switch on option */
		switch (c)
		{
		case 'D':
			/* do not reset tty upon exit, leave the given state */
			Reset = RCVFALSE;
			break;

		 case 'm':
			if ( ! isnumeric(optarg))
			{
				syserr("minimum characters to read must be numeric.", RCVTRUE);
				exit(2);
			}
			ttymin = atoi(optarg);
			break;

		 case 't':
			if ( ! isnumeric(optarg))
			{
				syserr("maxtime for read timeouts must be numeric.", RCVTRUE);
				exit(2);
			}
			ttytime = atoi(optarg);
			break;

		case 's':
			/* get baud rate */
			if ((ttyspeed = getspeed(optarg)) <= 0)
			{
				syserr("speed <= 0, or non-numeric.", RCVTRUE);
				exit(2);
			}
			break;

		case '?':
			/* print help msg */
			syserr("", RCVTRUE);
			exit(0);

		default:
			/* bad option in command line */
			syserr("command line option failure.", RCVTRUE);
			exit(2);
		}
	}

	/* device name must be in /dev partition */
	if (strncmp("/dev/", argv[optind], 5) != 0)
	{
		if (isnumeric(argv[optind]) == RCVTRUE)
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
	strcat(devname, argv[optind]);

	/* open external port connected to 5ess for read and write */
	ttyfd = ttyopen(devname, O_RDWR);
	if (ttyfd < 0)
	{
		/* check for errors */
		switch (ttyfd)
		{
		case RCVNOTTTY:
			fprintf(stderr, "device %s is not a tty.\n", devname);
			break;

		case RCVTMOUT:
			fprintf(stderr, "timeout opening device %s.\n", devname);
			break;

		}
		syserr("ttyopen failed for device.", RCVFALSE);

		/* close tty and exit */
		(void) close(ttyfd);
		exit(2);
	}

	/* save ttyfd in global ttyfd for trap on death-of-child signal */
	gttyfd = ttyfd;

	/* verify that stdin/stdout are terminals */
	if (!isatty(0) || !isatty(1))
	{
		syserr("stdin or stdout is not a terminal.", RCVFALSE);
		(void) close(ttyfd);
		exit(2);
	}

	/* clear screen */
	(void) system("tput clear");

	/* call function to communicate with 5ess via the io port */
	if ((status = rcvio(ttyfd, devname, ttyspeed, ttymin, ttytime)) != RCVOK)
	{
		fprintf(stderr, "i/o failure via port %s\n", devname);
		syserr("rcvio() failed.", RCVFALSE);
		(void) close(ttyfd);
		exit(2);
	}

	/* clear screen */
	(void) system("tput clear");

	/* finished, close external port */
	fprintf(stdout, "\nrcvio() completed.\n");
	(void) close(ttyfd);
	exit(0);
}
