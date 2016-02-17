/* main for dbms */
#include <stdio.h>
#include <errno.h>
#include "dbms.h"

/* main entry point */
main(argc, argv)
int argc;
char **argv;
{
    MESSAGE m;
    char *dbmsval, name[30], *getenv();
    long dbmskey, atol();
    void syserr();

    if ((dbmsval = getenv("DBMSKEY")) == NULL)
    {
	fprintf(stderr, "missing DBMSKEY in environment.\n");
	exit(2);
    }
    dbmskey = atol(dbmsval);
    while (receive(dbmskey, &m, sizeof(m)) > 0)
    {
	switch (m.cmd)
	{
	case 'o':
	    m.status = Dopen(m.file);
	    break;
	case 'c':
	    m.status = Dcreate(m.file);
	    break;
	case 'q':
	    m.status = Dclose();
	    rmqueue(dbmskey);
	    break;
	case 'g':
	    strcpy(name, m.rcd.name);
	    m.status = Dget(name, &m.rcd);
	    break;
	case 'n':
	    m.status = Dgetnext(&m.rcd);
	    break;
	case 'p':
	    m.status = Dput(&m.rcd);
	    break;
	case 'd':
	    m.status = Ddelete(m.rcd.name);
	    break;
	case 't':
	    m.status = Dtop();
	    break;
	default:
	    errno = EINVAL;
	    m.status = ERROR;
	    break;
	}
	m.errno = errno;
	if (send(m.clientkey, &m, sizeof(m)) < 0)
	{
	    fprintf(stderr, "can not send to %ld; errno = %d\n",
		    m.clientkey, errno);
	}
	if (m.cmd == 'q') exit(0);

    }
    syserr("dbmsmain");
}
