/*
 * program to log all input from a terminal.
 */

/* system headers */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

/* local definitions */
#define USAGE "usage: log log=logfile prog=program"
#define LOGFILE "log="
#define PROGRAM "prog="
#define ONEITEM 1

/* globals for error numbers */
extern int errno, sys_nerr;
extern char *sys_errlist[];

/* file descriptors */
int tochild, logfd;

/* signal handler */
void logsignal(sig)
int sig;
{
	sig = sig;
	/* close pipe to send eof to child */
	close(tochild);
	/* close log file */
	close(logfd);
	/* exit with success */
	exit(0);
}

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

/* main function */
main(argc, argv)
int argc;
char **argv;
{
	register unsigned char *pstr;
	unsigned char clogtime[124], logdate[124], buf[1025], logfile[124], program[124];
	int nrtotal, nr, nw, pid, pipefd[2];
	long logtime, time();

	/* initialize some junk */
	pid = -1;

	/* check for log file and program name */
	if (argc != 3)
	{
		syserr("", 1);
		exit(1);
	}
	else if ((strncmp(LOGFILE, argv[1], strlen(LOGFILE)) == 0) &&
	         (strncmp(PROGRAM, argv[2], strlen(PROGRAM)) == 0))
	{
		strcpy(logfile, &argv[1][strlen(LOGFILE)]);
		strcpy(program, &argv[2][strlen(PROGRAM)]);
	}
	else if ((strncmp(LOGFILE, argv[2], strlen(LOGFILE)) == 0) &&
	         (strncmp(PROGRAM, argv[1], strlen(PROGRAM)) == 0))
	{
		strcpy(logfile, &argv[2][strlen(LOGFILE)]);
		strcpy(program, &argv[1][strlen(PROGRAM)]);
	}
	else
	{
		syserr("", 1);
		exit(1);
	}

	/* open log file for all input */
	if ((logfd = open(logfile, 1)) == -1)
	{
		/* check if file does not exist (errno = 2) */
		if (errno != 2)
		{
			/* some other error, exit */
			syserr("error in opening log file.\n", 0);
			exit(1);
		}
		/* create log file for all input */
		if ((logfd = creat(logfile, 0644)) == -1)
		{
			/* exit */
			syserr("error in creating log file.\n", 0);
			exit(1);
		}
	}

	/* seek to end of file */
	if ((lseek(logfd, 0L, 2)) == -1)
	{
		/* close pipe and logfile, and exit */
		syserr("error in seeking to end of log file.\n", 0);
		close(logfd);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(1);
	}

	/* get pipe */
	if (pipe(pipefd) == -1)
	{
		syserr("pipe call failed.\n", 0);
		close(logfd);
		exit(1);
	}

	/* fork to create two processes */
	if ((pid = fork()) < 0)
	{
		fprintf(stderr,"fork failed.\n");
		close(logfd);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(1);
	}
	else if (pid == 0)
	{
		/* child process, connect stdin to pipe */
		close(0);
		dup(pipefd[0]);
		close(pipefd[0]);
		close(pipefd[1]);

		/* exec to new process */
		execlp(program, (char *) 0);

		/* child shouldn't be here */
		syserr("child: exec failed.\n", 0);
		exit(1);
	}

	/* set up signal handler in parent */
	signal(SIGHUP, logsignal);
	signal(SIGINT, logsignal);
	signal(SIGQUIT, logsignal);
	signal(SIGPIPE, logsignal);
	signal(SIGCLD, logsignal);

	/* close read part of pipe, copy write part */
	close(pipefd[0]);
	tochild = pipefd[1];

	/* read stdin and write to baby */
	for (nrtotal = 0; (nr = read(0, buf, 1024)) > 0; )
	{
		/* write to child via pipe */
		if ((nw = write(tochild, buf, nr)) != nr) break;

		/* convert returns to newlines and remove parity */
		for (pstr = buf; pstr < (buf + nr); pstr++)
		{
			*pstr = *pstr & 0177;
			if (*pstr == '\r') *pstr = '\n';
		}

		/* write to log file */
		(void) write(logfd, buf, nr);

		/* write time every 30 characters */
		nrtotal += nr;
		if (nrtotal > 30)
		{
			/* reset counter */
			nrtotal = 0;
			/* get present time */
			logtime = time((long *) NULL);
			/* write time to logfile */
			sprintf(clogtime, "\ntime is %ld.\n", logtime);
			(void) write(logfd, clogtime, strlen(clogtime));
		}
	}

	/* ignore death of child signal */
	signal(SIGCLD, SIG_IGN);

	/* close pipe to send eof to child */
	close(tochild);

	/* close log file */
	close(logfd);

	/* exit with success */
	exit(0);
}
