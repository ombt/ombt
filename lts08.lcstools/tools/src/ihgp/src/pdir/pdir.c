/* look up a path for a file */

/* unix headers */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

/* other defs */
#define ALL 1L
#define FINFO 2L
#define DEFAULTPATH "PATH"
#define OK 0
#define NOTOK -1

/* definitions for file types */
#define F_NONE 0
#define C_NONE '\0'
#define C_DIR 'd'
#define C_CHR 'c'
#define C_BLK 'b'
#define C_REG 'r'
#define C_LNK 'l'
#define C_SOCK 's'
#define C_FIFO 'f'
#define SLASH '/'

/* structure containing allowed file types */
struct ALWDFTYP {
	char copt;
	unsigned long ftype;
} alwdftyp[] = {
	{ C_DIR, S_IFDIR },
	{ C_CHR, S_IFCHR },
	{ C_BLK, S_IFBLK },
	{ C_REG, S_IFREG },
	{ C_LNK, S_IFLNK },
	{ C_SOCK, S_IFSOCK },
	{ C_FIFO, S_IFIFO },
	{ C_NONE, F_NONE },
};

/* globals */
extern char *optarg;
extern int optind, opterr;
extern int errno, sys_nerr;
extern char *sys_errlist[];

/* print usage msgs */
void
usage()
{
	fprintf(stderr, "usage: pdir [-?a] [-rwxfl] [-p pathname] file ...\n");
	return;
}

/* print error msgs */
void assert(myerrno, errmsg, s1, s2, s3, s4, s5)
int myerrno;
char *errmsg;
unsigned long s1, s2, s3, s4, s5;
{
	fprintf(stderr, "errno %d: ", myerrno);
	if (myerrno > 0 && myerrno < sys_nerr)
	{
		fprintf(stderr, "%s\n", sys_errlist[myerrno]);
	}
	else
	{
		fprintf(stderr, "unknown error number.\n");
	}
	if (errmsg != 0) fprintf(stderr, errmsg, s1, s2, s3, s4, s5);
	return;
}


/* check if file matches required file types */
int chkftype(ftype)
unsigned long ftype;
{
	int i;

	/* check if ftype is in list */
	for (i = 0; alwdftyp[i].ftype != F_NONE; i++)
	{
		/* determine type of file */
		if ((ftype & S_IFMT) == alwdftyp[i].ftype)
		{
			/* we have a match */
			return(i);
		}
	}
	/* not found */
	return(NOTOK);
}

/* print file size */
void printsize(statbuf)
struct stat *statbuf;
{
	fprintf(stdout, "%8ld ", (long) (statbuf->st_size));
	return;
}

/* print file permissions */
void printperm(statbuf)
struct stat *statbuf;
{
	fprintf(stdout, "%4lo ", (unsigned long) (statbuf->st_mode & 07777));
	return;
}

/* print file group */
void printgrp(statbuf)
struct stat *statbuf;
{
	struct group *pgrp, *getgrgid();

	if ((pgrp = getgrgid(statbuf->st_gid)) == (struct group *) NULL)
	{
		return;
	}
	fprintf(stdout, "%s ", pgrp->gr_name);
	return;
}

/* print file owner */
void printown(statbuf)
struct stat *statbuf;
{
	struct passwd *pusr, *getpwuid();

	if ((pusr = getpwuid(statbuf->st_uid)) == (struct passwd *) NULL)
	{
		return;
	}
	fprintf(stdout, "%s ", pusr->pw_name);
	return;
}

/* print file modification date  */
void printmod(statbuf)
struct stat *statbuf;
{
	struct tm *ltime;
	struct tm *localtime();

	ltime = localtime(&(statbuf->st_mtime));
	fprintf(stdout, "%02d/%02d/%02d %02d:%02d ",
		++ltime->tm_mon, ltime->tm_mday, ltime->tm_year,
		ltime->tm_hour, ltime->tm_min);
	return;
}

/* print file type */
void printtype(statbuf)
struct stat *statbuf;
{
	fprintf(stdout, "%c ", 
		alwdftyp[chkftype((unsigned long)(statbuf->st_mode))].copt);
	return;
}

/* print info for file */
void finfo(fname)
char *fname;
{
	struct stat statbuf;

	/* get info on file */
	if (stat(fname, &statbuf) != OK)
	{
		assert(errno, "unable to stat file, %s.\n", fname);
		return;
	}

	/* print out data */
	printsize(&statbuf);
	printtype(&statbuf);
	printperm(&statbuf);
	printmod(&statbuf);
	printown(&statbuf);
	printgrp(&statbuf);

	/* all done */
	return;
}

/* trace file thru a path */
int trace(options, pname, fname, databits)
unsigned long options;
char *pname, *fname;
int databits;
{
	char *pt, *ppd;
	char pbuf[BUFSIZ], pd[BUFSIZ];

	/* save path */
	strcpy(pd, pname);

	for (ppd = pd; (pt = strtok(ppd, ":")) != 0; ppd = 0)
	{
		sprintf(pbuf, "%s/%s", pt, fname);
		if (access(pbuf, databits) == 0)
		{
			if ((options & FINFO) == FINFO) finfo(pbuf);
			fprintf(stdout, "%s\n", pbuf);
			if ((options & ALL) != ALL) break;
		}
	}

	/* not found */
	return(OK);
}

/* main entry */
main(argc, argv)
int argc;
char **argv;
{
	int c, databits;
	char *path, *pathname;
	unsigned long options;

	/* set defaults */
	databits = 0;
	pathname = DEFAULTPATH;

	/* get cmd line options */
	for (options = 0; (c = getopt(argc, argv, "?larwxfp:")) != EOF; )
	{
		switch (c)
		{
		case 'a':
			/* list all files in path */
			options |= ALL;
			break;
		case 'l':
			/* print stat data for file */
			options |= FINFO;
			break;
		case 'r':
			/* check for read permissions */
			databits |= R_OK;
			break;
		case 'w':
			/* check for write permissions */
			databits |= W_OK;
			break;
		case 'x':
			/* check for execute permissions */
			databits |= X_OK;
			break;
		case 'f':
			/* check for file existence */
			databits |= F_OK;
			break;
		case 'p':
			/* name of path variable */
			pathname = optarg;
			break;
		case '?':
			/* print usage msg */
			usage();
			exit(0);
		default:
			assert(EINVAL, "bad option in command line.\n");
			exit(2);
		}
	}

	/* search path for given file */
	if (optind >= argc)
	{
		assert(EINVAL, "missing file name in command line.\n");
		usage();
		exit(2);
	}

	/* check options */
	if (databits == 0) databits = F_OK;

	/* get path from environment */
	if ((path = getenv(pathname)) == (char *)0)
	{
		assert(EINVAL, "path variable %s not found.\n", pathname);
		exit(2);
	}

	/* search for files */
	for ( ; optind < argc; optind++)
	{
		/* trace file up path */
		if (trace(options, path, argv[optind], databits) != OK)
		{
			assert(errno, "trace failed for file %s and path %s.\n",
			       argv[optind], path);
		}
	}

	/* all done */
	exit(0);
}
