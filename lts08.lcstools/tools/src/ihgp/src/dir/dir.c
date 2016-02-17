/*
 * print directory info.
 */

/* include unix headers */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

/* regular expression junk */
#ifdef SYSTEM5
extern char *regex();
extern char *regcmp();
#else
extern char *re_comp();
extern int re_exec();
#endif

/* for mips ... */
#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif

/* local definitions */
#define USAGE "usage: dir [-?arstpmogbT] [-S pattern] [-l level] [-f dcbrlsf] [filename]"
#define NOTOK -1
#define OK 0

/* command line options */
#define RECURSIVE 1
#define FILETYPE 2
#define PATTERN 4
#define SIZE 8
#define TYPE 16
#define PERMISSIONS 32
#define MODIFICATION 64
#define ALL 128
#define OWNER 256
#define GROUP 512
#define BLOCKS 1024
#define TOTAL 2048
#define IS_SET(x, y) (((x) & (y)) == (y))

/* definitions for file types */
#define F_NONE 0
#define F_DIR 1
#define F_CHR 2
#define F_BLK 4
#define F_REG 8
#define F_LNK 16
#define F_SOCK 32
#define F_FIFO 64
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
	unsigned long fbit;
	unsigned long ftype;
} alwdftyp[] = {
	{ C_DIR, F_DIR, S_IFDIR },
	{ C_CHR, F_CHR, S_IFCHR },
	{ C_BLK, F_BLK, S_IFBLK },
	{ C_REG, F_REG, S_IFREG },
	{ C_LNK, F_LNK, S_IFLNK },
	{ C_SOCK, F_SOCK, S_IFSOCK },
	{ C_FIFO, F_FIFO, S_IFIFO },
	{ C_NONE, F_NONE, F_NONE },
};

/* globals for controlling operations */
int level;
unsigned long total = 0;
unsigned long options = 0;
unsigned long filetype = F_NONE;
int maxlevel = 100;
char *pattern = (char *) NULL;

/* externs */
extern char *optarg;
extern int optind, errno;

/* check if file name fits pattern */
int chkpattern(fname)
char *fname;
{
	char *plastslash;

	/* compare file name to regular expression */
	plastslash = strrchr(fname, SLASH);
	if (plastslash == (char *) NULL)
	{
		plastslash = fname;
	}
	else
	{
		plastslash++;
	}
#ifdef SYSTEM5
	if (regex(pattern, plastslash) != NULL) return(OK);
#else
	if (re_exec(plastslash) == 1) return(OK);
#endif
	return(NOTOK);
}

/* check if file matches required file types */
int chkftype(ftype, reqftype)
unsigned long ftype;
unsigned long reqftype;
{
	int i;

	/* check if ftype is in list */
	for (i = 0; alwdftyp[i].fbit != F_NONE; i++)
	{
		/* determine type of file */
		if (((ftype & S_IFMT) == alwdftyp[i].ftype) &&
			((reqftype & alwdftyp[i].fbit) == alwdftyp[i].fbit))
		{
			/* we have a match */
			return(i);
		}
	}

	/* not found */
	return(NOTOK);
}

/* print file size */
int printsize(statbuf)
struct stat *statbuf;
{
	unsigned long blocks;
	if (IS_SET(options, BLOCKS))
	{
		fprintf(stdout, "%8ld ", 
		 (((blocks = statbuf->st_size/BUFSIZ) > 0) ? blocks : 1L));
	}
	else
	{
		fprintf(stdout, "%8ld ", (long) (statbuf->st_size));
	}
	return(OK);
}

/* print total file size */
int printtotal(statbuf)
struct stat *statbuf;
{
	total += statbuf->st_size;
	return(OK);
}

/* print file permissions */
int printperm(statbuf)
struct stat *statbuf;
{
	fprintf(stdout, "%4lo ", (unsigned long) (statbuf->st_mode & 07777));
	return(OK);
}

/* print file group */
int printgrp(statbuf)
struct stat *statbuf;
{
	struct group *pgrp, *getgrgid();

	if ((pgrp = getgrgid(statbuf->st_gid)) == (struct group *) NULL)
	{
		return(OK);
	}
	fprintf(stdout, "%s ", pgrp->gr_name);
	return(OK);
}

/* print file owner */
int printown(statbuf)
struct stat *statbuf;
{
	struct passwd *pusr, *getpwuid();

	if ((pusr = getpwuid(statbuf->st_uid)) == (struct passwd *) NULL)
	{
		return(OK);
	}
	fprintf(stdout, "%s ", pusr->pw_name);
	return(OK);
}

/* print file modification date  */
int printmod(statbuf)
struct stat *statbuf;
{
	struct tm *ltime;
	struct tm *localtime();

	ltime = localtime(&(statbuf->st_mtime));
	/* fix Y2K bug */
	if (ltime->tm_year >= 100)
		ltime->tm_year -= 100;
	fprintf(stdout, "%02d/%02d/%02d %02d:%02d ",
		  ++ltime->tm_mon, ltime->tm_mday, ltime->tm_year,
			ltime->tm_hour, ltime->tm_min);
	return(OK);
}

/* print file type */
int printtype(statbuf)
struct stat *statbuf;
{
	fprintf(stdout, "%c ", alwdftyp[chkftype((unsigned long)(statbuf->st_mode),
											 (unsigned long) (~0))].copt);
	return(OK);
}

/* print out file data */
int prtfdata(fname, statbuf)
char *fname;
struct stat *statbuf;
{
	long result;

	/* scan for file type, if required */
	if (IS_SET(options, FILETYPE))
	{
		if (chkftype((unsigned long) (statbuf->st_mode), 
					  filetype) == NOTOK) return(OK);
	}

	/* scan for regular expression, if required */
	if (IS_SET(options, PATTERN))
	{
		if (chkpattern(fname) == NOTOK) return(OK);
	}

	/* check for totals only */
	if (IS_SET(options, TOTAL))
	{
		return(printtotal(statbuf));
	}

	/* print file data */
	if (IS_SET(options, SIZE))
	{
		if (printsize(statbuf) != OK) return(NOTOK);
	}
	if (IS_SET(options, TYPE))
	{
		if (printtype(statbuf) != OK) return(NOTOK);
	}
	if (IS_SET(options, PERMISSIONS))
	{
		if (printperm(statbuf) != OK) return(NOTOK);
	}
	if (IS_SET(options, MODIFICATION))
	{
		if (printmod(statbuf) != OK) return(NOTOK);
	}
	if (IS_SET(options, OWNER))
	{
		if (printown(statbuf) != OK) return(NOTOK);
	}
	if (IS_SET(options, GROUP))
	{
		if (printgrp(statbuf) != OK) return(NOTOK);
	}

	/* print file name */
	fprintf(stdout, "%s\n", (strncmp(fname, "./", 2) == 0) ? fname+2 : fname);

	/* all done */
	return(OK);
}

/* process a directory and print out data */
int dproc(dname)
char *dname;
{
	int dnmlen;
	DIR *dirp;
	struct dirent *dp;

	/* check level if directory */
	if (level > maxlevel) return(OK);

	/* save file name length */
	dnmlen = strlen(dname);

	/* cycle thru entries in directory */
	if ((dirp = opendir(dname)) == NULL) return(NOTOK);

	for (dp = readdir(dirp); dp != NULL; dp = readdir(dirp))
	{
		/* skip this directory and previous level */
		if ((strcmp(dp->d_name, ".") == 0) || 
			(strcmp(dp->d_name, "..") == 0)) continue;
		/* process file name */
		strcpy(dname+dnmlen, "/");
		strcat(dname+dnmlen, dp->d_name);
		(void) fproc(dname);
	}

	/* close directory */
	(void) closedir(dirp);

	/* reset end of string */
	dname[dnmlen] = '\0';

	/* all done */
	return(OK);
}

/* process a file and print out data */
int fproc(fname)
char *fname;
{
	struct stat statbuf;

	/* get info on file */
	if (stat(fname, &statbuf) != OK)
	{
		fprintf(stderr, "unable to stat file %s.\n", fname);
		return(NOTOK);
	}

	/* print out data */
	(void) prtfdata(fname, &statbuf);

	/* check if file is a directory */
	if (S_ISDIR(statbuf.st_mode) && (IS_SET(options, RECURSIVE) || level == 0))
	{
		/* process directory */
		level++;
		(void) dproc(fname);
		level--;
	}

	/* all done */
	return(OK);
}

/* main entry point */
main(argc, argv)
int argc;
char **argv;
{
	char *pc;
	int c, i;
	char fname[BUFSIZ];

	/* get cmd line options */
	for (options = 0; (c = getopt(argc, argv, "?S:l:f:rstpmaogbT")) != EOF; )
	{
		switch (c)
		{
		case 'S':
			/* regular exprssion for file names */
			options |= PATTERN;
#ifdef SYSTEM5
			if ((pattern = regcmp(optarg, (char *) 0L)) == NULL)
#else
			if ((pattern = re_comp(optarg)) != NULL)
#endif
			{
				fprintf(stderr, "bad regular expression.\n");
				exit(2);
			}
			break;

		case 'f':
			/* search for a given file type */
			options |= FILETYPE;
			for (pc = optarg; *pc != '\0'; pc++)
			{
				for (i = 0; alwdftyp[i].fbit != F_NONE; i++)
				{
					if (*pc == alwdftyp[i].copt)
					{
						filetype |= alwdftyp[i].fbit;
						break;
					}
				}
				/* check for bad file types */
				if (alwdftyp[i].fbit == F_NONE)
				{
					fprintf(stderr, "bad file type, %c.\n%s\n", *pc, USAGE);
					exit(2);
				}
			}
			break;

		case 'l':
			/* maximum level */
			maxlevel = atoi(optarg);
			options |= RECURSIVE;
			break;

		case 'r':
			/* recursive descent */
			options |= RECURSIVE;
			break;

		case 'g':
			/* print group name */
			options |= GROUP;
			break;

		case 'o':
			/* print owner name */
			options |= OWNER;
			break;

		case 'a':
			/* print all file data */
			options |= GROUP | OWNER | SIZE | TYPE | PERMISSIONS | MODIFICATION;
			break;

		case 's':
			/* print file size */
			options |= SIZE;
			break;

		case 'T':
			/* print total file size */
			options |= TOTAL;
			break;

		case 'b':
			/* print file size in blocks */
			options |= BLOCKS;
			break;

		case 't':
			/* print file type */
			options |= TYPE;
			break;

		case 'p':
			/* print file permissions */
			options |= PERMISSIONS;
			break;
		case 'm':
			/* print last file modification date */
			options |= MODIFICATION;
			break;

		case '?':
			fprintf(stderr, "%s\n", USAGE);
			exit(0);

		default:
			/* bad option */
			fprintf(stderr, "bad option.\n%s\n", USAGE);
			exit(2);
		}
	}

	/* check if any file names were given */
	if (optind >= argc)
	{
		/* default to present directory */
		total = level = 0;
		strcpy(fname, ".");
		(void) fproc(fname);
		if (IS_SET(options, TOTAL))
		{
			if (IS_SET(options, BLOCKS))
			{
				total = (((total/BUFSIZ) > 0) ? total/BUFSIZ : 1);
			}
			fprintf(stdout, "total for %s = %ld\n", fname, total);
		}
	}
	else
	{
		/* process each file */
		for ( ; optind < argc; optind++)
		{
			total = level = 0;
			strcpy(fname, argv[optind]);
			(void) fproc(fname);
			if (IS_SET(options, TOTAL))
			{
				if (IS_SET(options, BLOCKS))
				{
					total = (((total/BUFSIZ) > 0) ? total/BUFSIZ : 1);
				}
				fprintf(stdout, "total for %s = %ld\n", fname, total);
			}
		}
	}

	/* all done */
	exit(0);
}
