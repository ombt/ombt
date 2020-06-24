/* make a directory and all intervening directories in path */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifdef M3B
#include <fastring.h>
#endif
#include "returns.h"
#include "debug.h"

#ifdef M3B
/* strtok code swiped from maxi */

#undef NULL
#define	NULL	(char *) 0

char *
strpbrk(string, brkset)
register char *string, *brkset;
{
	register char *p;

	do {
		for(p=brkset; *p != '\0' && *p != *string; ++p)
			;
		if(*p != '\0')
			return(string);
	}
	while(*string++);
	return(NULL);
}

int
strspn(string, charset)
char	*string;
register char	*charset;
{
	register char *p, *q;

	for(q=string; *q != '\0'; ++q) {
		for(p=charset; *p != '\0' && *p != *q; ++p)
			;
		if(*p == '\0')
			break;
	}
	return(q-string);
}

char *
strtok(string, sepset)
char	*string, *sepset;
{
	register char	*p, *q, *r;
	static char	*savept;

	/*first or subsequent call*/
	p = (string == NULL)? savept: string;

	if(p == 0)		/* return if no tokens remaining */
		return(NULL);

	q = p + strspn(p, sepset);	/* skip leading separators */

	if(*q == '\0')		/* return if no tokens remaining */
		return(NULL);

	if((r = strpbrk(q, sepset)) == NULL)	/* move past token */
		savept = 0;	/* indicate this is last token */
	else {
		*r = '\0';
		savept = ++r;
	}
	return(q);
}

/*
 * make a directory, if running as root. the following code
 * was swiped from the DMERT version of the mkdir cmd.
 */
int
mkdir(d)
char *d;
{
	char pname[128], dname[128];
	register i, slash = 0;

	/* initialize */
	pname[0] = '\0';

	/* find last slash in path */
	for(i = 0; d[i]; ++i)
	{
		if(d[i] == '/')
		{
			slash = i + 1;
		}
	}

	/* found at least one slash */
	if (slash)
	{
		strncpy(pname, d, slash);
	}
	strcpy(pname+slash, ".");

	/* check if directory exits */
	if (access(pname, 02))
	{
		fprintf(stderr,"mkdir: cannot access %s\n", pname);
		return(NOTOK);
	}

	/* make new directory and set owner */
	if ((mknod(d, 040777, 0, 0, 0)) < 0)
	{
		fprintf(stderr,"mkdir: cannot make directory %s\n", d);
		return(NOTOK);
	}
#ifndef	dmert
	chown(d, getuid(), getgid());
#else
	if(chown(d, getuid(), getgid()) < 0)
	{
		fprintf(stderr, "mkdir: cannot change owner of %s\n", d);
		return(NOTOK);
	}
#endif

	/* create links to previous directory and current directory */
	strcpy(dname, d);
	strcat(dname, "/.");
	if ((link(d, dname)) < 0)
	{
		fprintf(stderr, "mkdir: cannot link %s\n", dname);
		unlink(d);
		return(NOTOK);
	}
	strcat(dname, ".");
	if ((link(pname, dname)) < 0)
	{
		fprintf(stderr, "mkdir: cannot link %s\n",dname);
		dname[strlen(dname)] = '\0';
		unlink(dname);
		unlink(d);
		return(NOTOK);
	}

	/* all done */
	return(OK);
}
#endif

/* make a directory */
int
makeDir(path, mode)
char *path;
int mode;
{
	int mkdir();
	char *pdir, path1[BUFSIZ], path2[BUFSIZ];

	/* check parameters */
	if (path == (char *)0 || *path == 0)
	{
		errno = EINVAL;
		ERROR("invalid path given.", errno);
		return(NOTOK);
	}

	/* copy path for getting tokens */
	strcpy(path1, path);
	*path2 = 0;

	/* start looping thru path and making any missing directories */
	for (pdir = strtok(path1, "/"); pdir != (char *)0; 
	     pdir = strtok((char *)0, "/"))
	{
		/* add directory to path */
		strcat(path2, "/");
		strcat(path2, pdir);

		/* check if path exists */
		if (access(path2, 02) == OK)
		{
			/* directory exists, continue to next case */
			continue;
		}

		/* make directory */
#ifdef M3B
		if (mkdir(path2) != OK)
#else
		if (mkdir(path2, mode) != OK)
#endif
		{
			ERRORS("unable to make directory.", path2, errno);
			return(NOTOK);
		}
	}

	/* all done */
	return(OK);
}
