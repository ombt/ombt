/*
 * file contains the series of simple functions for setting up a
 * queue in a directory.
 */

/* unix headers */
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>

/* extern for errno */
extern int errno;
int tmperrno;

/* global for checking if a directory exists */
int DirExists = 0;

/* function to queue up a job */
int Enqueue(pdir)
char *pdir;
{
    struct stat dirbuffer;
    char myjobname[BUFSIZ];
    int qjobfd;
#ifdef EES
    char mkdircmd[BUFSIZ];
#endif

    /* check flag which indicates the queue directory exists */
    if ( ! DirExists)
    {
	/* verify the directory's existence */
	if (stat(pdir, &dirbuffer) == -1)
	{
	    /* check if errno is because the file does not exist */
	    if (errno != ENOENT) return(-1);
	    /* create the directory */
#ifdef EES
	    (void) sprintf(mkdircmd, "/bin/mkdir %s", pdir);
	    (void) system(mkdircmd);
#else
	    if (mknod(pdir, 0040000) == -1) return(-1);
#endif
	    /* set the permissions */
	    if (chmod(pdir, 0777) == -1) return(-1);
	}
	/* set flag to indicate a directory exists */
	DirExists = 1;
    }
    /* get the file name for the job entry */
    (void) sprintf(myjobname,"%s/%d", pdir, getpid());
    /* create the job entry in the directory */
    if ((qjobfd = creat(myjobname, 0777)) == -1) return(-1);
    /* the job is created, close the file */
    if (close(qjobfd) == -1) return(-1);
    /* return success */
    return(0);
}

/* function to remove a job from the queue */
int Dequeue(pdir)
char *pdir;
{
    char myjobname[BUFSIZ];
    struct stat dirbuffer;
#ifdef EES
    char mkdircmd[BUFSIZ];
#endif

    /* check if the queue directory exists */
    if ( ! DirExists)
    {
	/* verify the directory's existence */
	if (stat(pdir, &dirbuffer) == -1)
	{
	    /* check if errno is because the file does not exist */
	    if (errno != ENOENT) return(-1);
	    /* create the directory */
#ifdef EES
	    (void) sprintf(mkdircmd, "/bin/mkdir %s", pdir);
	    (void) system(mkdircmd);
#else
	    if (mknod(pdir, 0040000) == -1) return(-1);
#endif
	    /* set the permissions */
	    if (chmod(pdir, 0777) == -1) return(-1);
	}
	/* set flag to indicate a directory exists */
	DirExists = 1;
	/* no job to remove so return success */
	return(0);
    }
    /* get the file name for the job entry */
    (void) sprintf(myjobname,"%s/%d", pdir, getpid());
    /* delete the queue entry */
    (void) unlink(myjobname);
    /* return success */
    return(0);
}

/* function to return if we're next in the queue */
int NextInQueue(pdir, MaxTimeInQ)
char *pdir;
long MaxTimeInQ;
{
    char myjobname[BUFSIZ], jobname[BUFSIZ];
    struct stat myjobstat, jobstat;
    time_t myjobqtime, jobqtime, rightnow;
    int myjobpid, jobpid; 
    long time();

    struct direct dirdata;
    int dirfd, bytesread;

    char cjobpid[BUFSIZ];

    /* get the time */
    rightnow = time((long *)0);
    /* check if the queue directory exists */
    if ( ! DirExists)
    {
	/* no directory exists, queue the job */
	if (Enqueue(pdir) == -1) return(-1);
	/* return for them to wait */
	return(0);
    }
    /* get the file name for the job entry */
    (void) sprintf(myjobname,"%s/%d", pdir, getpid());
    /* check if the job exists */
    if (stat(myjobname, &myjobstat) == -1)
    {
	/* check if errno is because the file does not exist */
	if (errno != ENOENT) return(-1);
	/* job does not exists, queue the job */
	DirExists = 0;
	if (Enqueue(pdir) == -1) return(-1);
	/* return for them to wait */
	return(0);
    }
    /* get the queue time for my job */
    myjobqtime = myjobstat.st_mtime;
    /* assume my job is next */
    myjobpid = getpid();
    /* open the queue directory for read */
    if ((dirfd = open(pdir, 0)) == -1) return(-1);
    /* read all the entries in the directory */
    while ((bytesread = read(dirfd, &dirdata, sizeof(struct direct))) == 
	     sizeof(struct direct))
    {
	/* check if the inode is not in use */
	if (dirdata.d_ino == 0) continue;
	/* skip present and parent directories */
	if ((strcmp(dirdata.d_name, ".") == 0) || 
	    (strcmp(dirdata.d_name, "..") == 0)) continue;
	/* get the job name and pid */
	(void) strncpy(cjobpid, dirdata.d_name, DIRSIZ);
	cjobpid[DIRSIZ] = '\0';
	(void) sprintf(jobname,"%s/%s", pdir, cjobpid);
	jobpid = atoi(cjobpid);
	/* check file name, should only be numbers */
	if (spnlen(cjobpid, "0123456789") != strlen(cjobpid))
	{
	    /* file name has non-numeric characters, blast it and continue */
	    (void) unlink(jobname);
	    continue;
	}
	/* get the queue time for this job entry */
	if (stat(jobname, &jobstat) == -1)
	{
	    tmperrno = errno;
	    (void) close(dirfd);
	    errno = tmperrno;
	    return(-1);
	}
	/* compare times */
	if (((jobstat.st_mtime == myjobqtime) && (jobpid < myjobpid)) ||
	    (jobstat.st_mtime < myjobqtime))
	{
	    /* check if this process was in the queue for too long */
	    if (MaxTimeInQ > 0 && ((rightnow - jobstat.st_mtime) > MaxTimeInQ))
	    {
#ifdef DEBUG
		fprintf(stderr, "Unlinking %s ...\n", jobname);
#endif
		/* we have a possible dead process */
		(void) unlink(jobname);
		/* go on to next entry */
		continue;
	    }
	    /* we found a job before us, we still have to wait */
	    (void) close(dirfd);
	    return(0);
	}
    }
    /* close the directory */
    (void) close(dirfd);
    /* check the read */
    if (bytesread != 0) return(-1);
    /* we're next */
    return(1);
}

/* get length of a string composed of given characters */
int spnlen(string, set)
char *string;
char *set;
{
    register char *pstring, *pset;

    /* 
     * scan string until a character not in the given set is found,
     * then return the length of the string which is composed only
     * of set characters.
     */
    for (pstring = string; *pstring != '\0'; pstring++)
    {
	for (pset = set; *pset != '\0' && *pset != *pstring; pset++)
	{
	    /* do nothing */
	}
	if (*pset == '\0') break;
    }
    /* return length of string */
    return(pstring - string);
}

/* function to wait for queued job */
int QAndWait(pdir, SleepTime, MaxTimeInQ)
char *pdir;
int SleepTime;
long MaxTimeInQ;
{
    int qstatus;
    int Iter, MaxIterations;

    /* check arguments, assume an invalid argument error */
    errno = EINVAL;
    if (SleepTime <= 0 || MaxTimeInQ <= 0) return(-1);
    if (SleepTime >= MaxTimeInQ) return(-1);
    if (pdir == (char *) 0) return(-1);
    /* clear errno for now */
    errno = 0;
    /* create a queue job */
    if (Enqueue(pdir) != 0) return(-1);
    /* we don't want to be purged, choose 80 percent */
    MaxIterations = ((MaxTimeInQ*8)/SleepTime)/10;
    if (MaxIterations <= 0) MaxIterations = 1;
    /* now wait for turn */
    for (Iter = 0; 
	(Iter < MaxIterations) && 
	((qstatus = NextInQueue(pdir, MaxTimeInQ)) == 0); 
	 Iter++)
    {
	/* sleep for a while */
	sleep(SleepTime);
#ifdef DEBUG
	fprintf(stderr, "sleep time, total time is %d.\n", Iter*SleepTime);
#endif
    }
    /* check status for errors */
    if (qstatus < 0) return(-1);
    /* check for timeout */
    if (Iter >= MaxIterations) return(0);
    /* this job is next */
    return(1);
}

