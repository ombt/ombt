/* functions called by a user */

#include <stdio.h>
#include <errno.h>
#include "dbms.h"

static MESSAGE m;

/* call dbms */
int dbmscall(r)
RCD *r;
{
    char *dbmsval, *getenv();
    static long dbmskey = 0L;
    long atol();
    int getpid();

    if (dbmskey == 0)
    {
	if ((dbmsval = getenv("DBMSKEY")) == NULL)
	{
	    fprintf(stderr, "missing DBMSKEY environment variable.\n");
	    exit(1);
	}
	dbmskey = atol(dbmsval);
    }
    if (m.clientkey == 0) m.clientkey = getpid();
    if (send(dbmskey, &m, sizeof(m)) < 0) return(ERROR);
    if (receive(m.clientkey, &m, sizeof(m)) <= 0) return(ERROR);
    if (r != NULL) *r = m.rcd;
    if (m.status == ERROR)
    {
	errno = m.errno;
    }
    else
    {
	errno = 0;
    }
    return(m.status);
}

/* open database */
int Dopen(file)
char *file;
{
    m.cmd = 'o';
    strcpy(m.file, file);
    return(dbmscall(NULL));
}

/* create database */
int Dcreate(file)
char *file;
{
    m.cmd = 'c';
    strcpy(m.file, file);
    return(dbmscall(NULL));
}

/* close database */
int Dclose()
{
    int status;

    m.cmd = 'q';
    status = dbmscall(NULL);
    rmqueue(m.clientkey);
    return(status);
}

/* goto top of database */
int Dtop()
{
    m.cmd = 't';
    return(dbmscall(NULL));
}

/* get record name */
int Dget(name, r)
char *name;
RCD *r;
{
    m.cmd = 'g';
    strcpy(m.rcd.name, name);
    return(dbmscall(r));
}

/* get next record */
int Dgetnext(r)
RCD *r;
{
    m.cmd = 'n';
    return(dbmscall(r));
}

/* put a record */
int Dput(r)
RCD *r;
{
    m.cmd = 'p';
    m.rcd = *r;
    return(dbmscall(NULL));
}

/* delete a record */
int Ddelete(name)
char *name;
{
    m.cmd = 'd';
    strcpy(m.rcd.name, name);
    return(dbmscall(NULL));
}
