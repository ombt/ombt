/* test for simple dbms */
#include <stdio.h>
#include <errno.h>
#include "dbms.h"

/* print record */
void rcdprint(r)
RCD *r;
{
    fprintf(stdout, "Name\t%s\n", r->name);
    fprintf(stdout, "Street\t%s\n", r->street);
    fprintf(stdout, "City\t%s\n", r->city);
    fprintf(stdout, "State\t%s\n", r->state);
    fprintf(stdout, "ZIP\t%s\n", r->zip);
    fprintf(stdout, "Tel\t%s\n", r->tel);
    return;
}

/* get info from user */
void prompt(msg, result, max, required)
char *msg, *result;
int max, required;
{
    char s[200];
    int len;

    while (1)
    {
	fprintf(stdout, "\n%s? ", msg);
	if (gets(s) == NULL) exit(0);
	if ((len = strlen(s)) >= max)
	{
	    fprintf(stdout, "Response is too long.\n");
	    continue;
	}
	if (len == 0 && required)
	{
	    fprintf(stdout, "Value is required.\n");
	    continue;
	}
	strcpy(result, s);
	return;
    }
}

/* main entry point */
main()
{
    char cmd[5], file[50], name[30];
    RCD rcd;

    while (1)
    {
	prompt("Command (? for help)", cmd, sizeof(cmd), TRUE);
	if (strlen(cmd) != 1)
	{
	    fprintf(stdout, "One letter only.\n");
	    continue;
	}
	switch (cmd[0])
	{
	case '?':
	    fprintf(stdout, "o - open database\n");
	    fprintf(stdout, "c - create database\n");
	    fprintf(stdout, "p - put record\n");
	    fprintf(stdout, "d - delete record\n");
	    fprintf(stdout, "g - getrecord by key\n");
	    fprintf(stdout, "n - get next record\n");
	    fprintf(stdout, "t - rewind to top of database\n");
	    fprintf(stdout, "q - quit\n");
	    continue;
	case 'o':
	    prompt("File to open", file, sizeof(file), TRUE);
	    if (Dopen(file) == OK)
	    {
		fprintf(stdout, "OK\n");
	    }
	    else
	    {
		fprintf(stdout, "FAILED: errno = %d\n", errno);
	    }
	    continue;
	case 'c':
	    prompt("File to create", file, sizeof(file), TRUE);
	    if (Dcreate(file) == OK)
	    {
		fprintf(stdout, "OK\n");
	    }
	    else
	    {
		fprintf(stdout, "FAILED: errno = %d\n", errno);
	    }
	    continue;
	case 'q':
	    if (Dclose() == OK)
	    {
		fprintf(stdout, "OK\n");
	    }
	    else
	    {
		fprintf(stdout, "FAILED: errno = %d\n", errno);
	    }
	    exit(0);
	case 'g':
	    prompt("Name", name, sizeof(name), TRUE);
	    switch (Dget(name, &rcd))
	    {
	    case OK:
		rcdprint(&rcd);
		continue;
	    case NOTFOUND:
		fprintf(stdout, "NOT FOUND\n");
		continue;
	    case ERROR:
		fprintf(stdout, "FAILED: errno = %d\n", errno);
		continue;
	    }
	case 'n':
	    switch (Dgetnext(&rcd))
	    {
	    case OK:
		rcdprint(&rcd);
		continue;
	    case NOTFOUND:
		fprintf(stdout, "NOT FOUND\n");
		continue;
	    case ERROR:
		fprintf(stdout, "FAILED: errno = %d\n", errno);
		continue;
	    }
	case 'p':
	    prompt("Name", rcd.name, sizeof(rcd.name), TRUE);
	    prompt("Street", rcd.street, sizeof(rcd.street), FALSE);
	    prompt("City", rcd.city, sizeof(rcd.city), FALSE);
	    prompt("State", rcd.state, sizeof(rcd.state), FALSE);
	    prompt("ZIP", rcd.zip, sizeof(rcd.zip), FALSE);
	    prompt("Tel", rcd.tel, sizeof(rcd.tel), FALSE);
	    if (Dput(&rcd) == OK)
	    {
		fprintf(stdout, "OK\n");
	    }
	    else
	    {
		fprintf(stdout, "FAILED: errno = %d\n", errno);
	    }
	    continue;
	case 'd':
	    prompt("Name", name, sizeof(name), TRUE);
	    if (Ddelete(name) == OK)
	    {
		fprintf(stdout, "OK\n");
	    }
	    else
	    {
		fprintf(stdout, "FAILED: errno = %d\n", errno);
	    }
	    continue;
	case 't':
	    if (Dtop() == OK)
	    {
		fprintf(stdout, "OK\n");
	    }
	    else
	    {
		fprintf(stdout, "FAILED: errno = %d\n", errno);
	    }
	    continue;
	default:
	    fprintf(stdout, "Unknown command -use ? for help\n");
	    continue;
	}
    }
}
