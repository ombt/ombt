/*
 * program to test using signals and fifos.
 */

/* system headers */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <termio.h>
#include <fcntl.h>
#include <errno.h>

/* externs */
extern int errno, sys_nerr;
extern char *sys_errlist[];
int gstate;

/* local definitions */
#define USAGE "usage: fifo fifo_name"
#define FIFOOK 1
#define FIFONOTOK -1
#define FIFOTRUE 1
#define FIFOFALSE 0
#define SLEEPTIME 1
#define MAXTRIES 10
#define NULLCHR '\0'
#define NULLPTR 0
#define COUNT 1
#define READFIFO 2

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

/* make a fifo */
int mkfifo(fifoname)
char *fifoname;
{
    return(mknod(fifoname, (010000 | 0666), 0));
}

/* function to open a fifo for write */
int openfifo(fifoname, flags, sleeptime, maxtries)
char *fifoname;
int flags, sleeptime, maxtries;
{
    int fd, tries;

    /* try opening fifo */
    for (tries = 1; tries <= maxtries; tries++)
    {
	/* try opening with no delay, retry for ENXIO */
	if ((fd = open(fifoname, (flags | O_NDELAY))) != -1) break;
	if (errno != ENXIO) return(FIFONOTOK);
	sleep(sleeptime);
    }
    /* check for failure in the last case */
    if (fd == -1)
    {
	/* reset errno, sleep would have changed it */
	errno = ENXIO;
	return(FIFONOTOK);
    }
    /* clear no delay bit */
    if (fcntl(fd, F_SETFL, flags) != 0) return(FIFONOTOK);
    /* return with file descriptor */
    return(fd);
}

/* trap for catching interrupts */
#ifdef VAX
void user1_trap()
#else
int user1_trap()
#endif
{
    /* ignore any more SIGUSR1 */
    (void) signal(SIGUSR1, SIG_IGN);
    /* change state to read fifo */
    gstate = READFIFO;
#ifdef VAX
    return;
#else
    return(1);
#endif
}

/* child process to read fifo */
void childproc(fifoname, inbuf)
char *fifoname, *inbuf;
{
    long count;
    int fd, nread;
#ifdef VAX
    void user1_trap();
#else
    int user1_trap();
#endif

    /* open fifo for read */
    if ((fd = openfifo(fifoname, O_RDONLY, 1, 1)) < 0) 
    {
	syserr("childproc can not open fifo.", FIFOFALSE);
	exit(2);
    }
    /* original state is counting */
    gstate = COUNT;
    count = 0;
    /* reset interrupt handler */
    (void) signal(SIGUSR1, user1_trap);
    while (1)
    {
	switch (gstate)
	{
	case READFIFO:
	    /* read fifo */
	    nread = read(fd, inbuf, BUFSIZ);
	    inbuf[nread] = NULLCHR;
	    /* write buffer */
	    if (nread > 0) fprintf(stdout, "childproc: %s\n", inbuf);
	    /* reset interrupt handler */
	    (void) signal(SIGUSR1, user1_trap);
	    /* continue counting */
	    gstate = COUNT;
	    break;
	case COUNT:
	default:
	    sleep(1);
	    fprintf(stdout, "childproc: counting ... %d\n", count++);
	    break;
	}
    }
    /*NOTREACHED*/
    return;
}

/* main program */
main(argc, argv)
int argc;
char *argv[];
{
    char *fifoname, inbuf[BUFSIZ + 1];
    int fifofd, pid, unlink();

    /* check for fifo name */
    if (argc <= 1)
    {
	syserr("no fifo name was given.", FIFOTRUE);
	exit(2);
    }
    fifoname = argv[1];
    /* create fifo */
    if (mkfifo(fifoname) < 0)
    {
	syserr("making a fifo failed.", FIFOFALSE);
	exit(2);
    }
    /* fork to create child process which will read the fifo */
    if ((pid = fork()) < 0)
    {
	syserr("failed to create child process.", FIFOFALSE);
	(void) unlink(fifoname);
	exit(2);
    }
    else if (pid == 0)
    {
	/* child process */
	childproc(fifoname, inbuf);
    }
    /* open fifo for write */
    if ((fifofd = openfifo(fifoname, O_WRONLY, SLEEPTIME, MAXTRIES)) < 0)
    {
	syserr("failed to open fifo for write.", FIFOFALSE);
	kill(pid, SIGINT);
	(void) unlink(fifoname);
	exit(2);
    }
    /* loop, read from tty, write to fifo */
    while ((fgets(inbuf, BUFSIZ, stdin) != (char *) NULLPTR) && ( ! feof(stdin)))
    {
	/* NULL terminate, just in case */
	inbuf[BUFSIZ] = NULLCHR;
	/* interrupt child to tell it to read fifo */
	(void) kill(pid, SIGUSR1);
	/* send msg to child proc */
	if (write(fifofd, inbuf, strlen(inbuf)) < 0)
	{
	    syserr("error writing to fifo.", FIFOFALSE);
	    kill(pid, SIGINT);
	    (void) unlink(fifoname);
	    exit(2);
	}
    }
    /* clean up and exit */
    kill(pid, SIGINT);
    (void) unlink(fifoname);
    exit(0);
}
