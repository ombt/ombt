/* io routines with a sanity timer */

/* headers */
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

/* local headers */
#include "returns.h"
#include "debug.h"
#include "tio.h"

/* time out value */
static int maxtime = DEFAULTMAXTIME;
static int fired = 0;

/* signal handler */
#ifdef M3B
int
#else
void
#endif
timeout(sig)
int sig;
{
	/* set time out flag to true */
	sig;
	ERROR("time out during i/o operation.", errno);
	fired = 1;
#ifdef M3B
	return(OK);
#else
	return;
#endif
}

/* function declarations */
int
topen(path, oflag)
char *path;
int oflag;
{
	int ret;
#ifdef M3B
	int (*oldfn)();
#else
	void (*oldfn)();
#endif

	/* clear time out flag */
	fired = 0;

	/* set up handler */
	oldfn = signal(SIGALRM, timeout);
	alarm(maxtime);

	/* execute the open */
	ret = open(path, oflag);

	/* stop timer */
	if ( ! fired)
	{
		alarm(0);
		signal(SIGALRM, oldfn);
	}

	/* return with results */
	return(ret);
}

int
tclose(fd)
int fd;
{
	int ret;
#ifdef M3B
	int (*oldfn)();
#else
	void (*oldfn)();
#endif

	/* clear time out flag */
	fired = 0;

	/* set up handler */
	oldfn = signal(SIGALRM, timeout);
	alarm(maxtime);

	/* execute the close */
	ret = close(fd);

	/* stop timer */
	if ( ! fired)
	{
		alarm(0);
		signal(SIGALRM, oldfn);
	}

	/* return with results */
	return(ret);
}

int
tread(fd, buf, nbyte)
int fd;
char *buf;
unsigned nbyte;
{
	int ret;
#ifdef M3B
	int (*oldfn)();
#else
	void (*oldfn)();
#endif

	/* clear time out flag */
	fired = 0;

	/* set up handler */
	oldfn = signal(SIGALRM, timeout);
	alarm(maxtime);

	/* execute the read */
	ret = read(fd, buf, nbyte);

	/* stop timer */
	if ( ! fired)
	{
		alarm(0);
		signal(SIGALRM, oldfn);
	}

	/* return with results */
	return(ret);
}

int
twrite(fd, buf, nbyte)
int fd;
char *buf;
unsigned nbyte;
{
	int ret;
#ifdef M3B
	int (*oldfn)();
#else
	void (*oldfn)();
#endif

	/* clear time out flag */
	fired = 0;

	/* set up handler */
	oldfn = signal(SIGALRM, timeout);
	alarm(maxtime);

	/* execute the write */
	ret = write(fd, buf, nbyte);

	/* stop timer */
	if ( ! fired)
	{
		alarm(0);
		signal(SIGALRM, oldfn);
	}

	/* return with results */
	return(ret);
}

int
tsettime(tval)
int tval;
{
	if (tval < 0)
		return(NOTOK);
	maxtime = tval;
	return(OK);
}

int
tgettime()
{
	return(maxtime);
}
