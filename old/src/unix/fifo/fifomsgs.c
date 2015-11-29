/* routines for sending/receiving msgs using fifos */

/* include unix headers */
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* include local headers */
#include "fifomsgs.h"

/* fifo name */
static char fifo[FIFONAMESZ];

/* array of opened fifos */
static struct {
    long key;
    int fd;
    int time;
} fifos[MAXOPEN] = { 0 };

/* clock */
static int myclock;

/* error handling */
static void fifoerrors(msg)
char *msg;
{
    extern int errno, sys_nerr;
    extern char *sys_errlist[];

    fprintf(stderr, "ERROR: %s (%d", msg, errno);
    if (errno > 0 && errno < sys_nerr)
    {
	fprintf(stderr, "; %s)\n", sys_errlist[errno]);
    }
    else
    {
	fprintf(stderr, ")\n");
    }
    exit(1);
}

/* create a fifo */
int mymkfifo(path)
char *path;
{
    return(mknod(path, (S_IFIFO | 0666), 0));
}

/* construct fifo name from key */
static char *fifoname(key)
long key;
{
    (void) sprintf(fifo, "/tmp/fifo%ld", key);
    return(fifo);
}

/* open a fifo for read/write */
static int openfifo(key, flags)
long key;
int flags;
{
    int i, avail, oldest, fd, tries, saveerrno;
    char *fifo;

    /* increment clock */
    myclock++;
    /* find the fifo for the given key */
    for (avail = -1, i = 0; i < MAXOPEN; i++)
    {
	if (fifos[i].key == key)
	{
	    /* store time of last use */
	    fifos[i].time = myclock;
	    /* return with fifo */
	    return(fifos[i].fd);
	}
	if (fifos[i].key == 0 && avail == -1)
	{
	    /* save available fd but keep searching for match */
	    avail = i;
	}
    }
    /* check if no opened fifo was found */
    if (avail == -1)
    {
	oldest = -1;
	for (i = 0; i < MAXOPEN; i++)
	{
	    if (oldest == -1 || fifos[i].time < oldest)
	    {
		oldest = fifos[i].time;
		avail = i;
	    }
	}
	if (close(fifos[avail].fd) == -1) return(-1);
    }
    /* get fifo name */
    fifo = fifoname(key);
    if (mymkfifo(fifo) == -1 && errno != EEXIST) return(-1);
    /* try to open, retry upon failure */
    for (tries = 1; tries <= MAXTRIES; tries++)
    {
	if ((fd = open(fifo, (flags | O_NDELAY))) != -1) break;
	if (errno != ENXIO) return(-1);
	sleep(NAPTIME);
    }
    /* check if fifo was opened */
    if (fd == -1)
    {
	errno = ENXIO;
	return(-1);
    }
    /* clear O_NDELAY flag */
    if (fcntl(fd, F_SETFL, flags) == -1)
    {
	saveerrno = errno;
	close(fd);
	errno = saveerrno;
	return(-1);
    }
    /* save data for fifo */
    fifos[avail].key = key;
    fifos[avail].fd = fd;
    fifos[avail].time = myclock;
    /* all done */
    return(fd);
}

/* send a msg */
int send(dstkey, buf, nbytes)
long dstkey;
char *buf;
int nbytes;
{
    int fd;

    /* open fifo for write */
    if ((fd = openfifo(dstkey, O_WRONLY)) == -1) return(-1);
    /* write msg */
    return(write(fd, buf, nbytes));
}

/* read a msg */
int receive(srckey, buf, nbytes)
long srckey;
char *buf;
int nbytes;
{
    int fd, nread;

    /* open fifo for read */
    if ((fd = openfifo(srckey, O_RDONLY)) == -1) return(-1);
    /* read msg */
    while ((nread = read(fd, buf, nbytes)) == 0)
    {
	sleep(NAPTIME);
    }
    /* return bytes read and buffer */
    return(nread);
}

/* remove a fifo */
void rmqueue(key)
long key;
{
   if (unlink(fifoname(key)) == -1 && errno != ENOENT)
   {
	syserr("unlink");
   }
   return;
}
