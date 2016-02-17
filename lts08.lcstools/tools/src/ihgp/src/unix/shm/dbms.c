/* database example */

/* headers */
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include "dbms.h"

/* globals */
static int fd = -1;

/* open database */
int Dopen(file)
char *file;
{
	fprintf(stderr, "Service: opening file %s ...\n", file);
	if ((fd = open(file, O_RDWR, 0)) == -1)
		return(ERROR);
	return(OK);
}

/* create a database */
int Dcreate(file)
char *file;
{
	fprintf(stderr, "Service: creating file %s ...\n", file);
	if ((fd = open(file, (O_RDWR|O_CREAT|O_TRUNC), 0666)) == -1)
		return(ERROR);
	return(OK);
}

/* close database */
int Dclose()
{
	fprintf(stderr, "Service: closing ...\n");
	if (close(fd) == -1)
		return(ERROR);
	return(OK);
}

/* go to top of db */
int Dtop()
{
	fprintf(stderr, "Service: seeking to top ...\n");
	if (lseek(fd, 0L, 0) == -1)
		return(ERROR);
	return(OK);
}

/* retrieve a record */
int Dget(name, r)
char *name;
RCD *r;
{
	int nread;

	fprintf(stderr, "Service: getting record %s ... \n", name);

	/* reset to start of file */
	if (Dtop() != OK)
		return(ERROR);

	/* search for name */
	while ((nread = read(fd, r, sizeof(RCD))) == sizeof(RCD))
	{
		if (strcmp(r->name, name) == 0)
		{
			if (lseek(fd, -(long)sizeof(RCD), 1) == -1)
				return(ERROR);
			else
				return(OK);
		}
	}

	/* did we find it? */
	switch (nread)
	{
	case 0:
		return(NOTFOUND);
	case -1:
		return(ERROR);
	default:
		errno = 0;
		return(ERROR);
	}
}

/* once a top, get next record */
int Dgetnext(r)
RCD *r;
{
	fprintf(stderr, "Service: getting next record ... \n");

	/* get the next record in the file */
	while (1)
	{
		switch (read(fd, r, sizeof(RCD)))
		{
		case sizeof(RCD):
			if (r->name[0] == '\0') continue;
			return(OK);
		case 0:
			return(NOTFOUND);
		case -1:
			return(ERROR);
		default:
			errno = 0;
			return(ERROR);
		}
	}
}

/* put a new record in database */
int Dput(r)
RCD *r;
{
	RCD rcd;

	fprintf(stderr, "Service: putting record %s ... \n", r->name);

	/* get a existing record */
	switch (Dget(r->name, &rcd))
	{
	case NOTFOUND:
		if (lseek(fd, 0L, 2) == -1)
			return(-1);
		break;
	case ERROR:
		return(ERROR);
	}

	/* write new record */
	switch (write(fd, r, sizeof(RCD)))
	{
	case sizeof(RCD):
		return(OK);
	case -1:
		return(ERROR);
	default:
		errno = 0;
		return(ERROR);
	}
}

/* delete a record from database */
int Ddelete(name)
char *name;
{
	RCD rcd;

	fprintf(stderr, "Service: deleting record %s ... \n", name);

	/* delete a record, first get it */
	switch (Dget(name, &rcd))
	{
	case NOTFOUND:
		return(OK);
		break;
	case ERROR:
		return(ERROR);
	}

	/* over write record with a NULL name */
	rcd.name[0] = '\0';
	switch (write(fd, &rcd, sizeof(RCD)))
	{
	case sizeof(RCD):
		return(OK);
	case -1:
		return(ERROR);
	default:
		errno = 0;
		return(ERROR);
	}
}
