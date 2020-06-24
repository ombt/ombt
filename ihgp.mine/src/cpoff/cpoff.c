/*
 * copy a file from a start byte to an end byte.
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define USAGE "usage: cpoff [-s start] [-e end] from_file to_file"

long cpoff(from, to, start, end, perm)

char *from;		/* original file */
char *to;		/* target file */
long start;		/* byte offset to start copy */
long end;		/* byte offset to end copy */
unsigned short perm;	/* permissions */
{
    char buf[BUFSIZ], *tmpnam();
    int unlink(), creat(), open(), read(), write(), close();
    void rid();
    int nr, nw, fromfd, tofd;

    /* check files were given */
    if (strlen(from) == 0 || strlen(to) == 0) return(0);

    /* create to file */
    if ((tofd = creat(to, 0666)) == -1)
    {
	(void) unlink(to);
	return (0);
    }

    /* set permissions */
    (void) chmod(to, perm);

    /* open from file */
    if ((fromfd = open(from, 0)) == -1)
    {
	(void) close(tofd);
	(void) unlink(to);
	return (0);
    }

    /* seek to byte offset */
    if (lseek(fromfd, start, 0) == -1)
    {
	rid(fromfd, tofd, to);
	return (0);
    }

    /* starting from offset, copy files */
    nr = 0;
    nw = 0;

    for ( ; start + BUFSIZ <= end; start += BUFSIZ)
    {
	nr = read(fromfd, buf, BUFSIZ);
	if (nr < 0)
	{
	    rid(fromfd, tofd, to);
	    return (0);
	}
	nw = write(tofd, buf, nr);
	if (nw < 0 || nw != nr)
	{
	    rid(fromfd, tofd, to);
	    return (0);
	}
    }

    if (start < end)
    {
	nr = read(fromfd, buf, (end - start + 1));
	if (nr < 0)
	{
	    rid(fromfd, tofd, to);
	    return (0);
	}
	nw = write(tofd, buf, nr);
	if (nw < 0 || nw != nr)
	{
	    rid(fromfd, tofd, to);
	    return (0);
	}
    }

    (void) close(fromfd);
    (void) close(tofd);

    return (1);
}

void rid(fromfd, tofd, to)
int fromfd, tofd;
char *to;
{
    int unlink(), close();

    /* close files, and unlink target file */
    (void) close(fromfd);
    (void) close(tofd);
    (void) unlink(to);

    return;
}

main(argc, argv)
int argc;
char **argv;
{
    struct stat finfoto, finfofrom;
    long start, end, atol();
    unsigned short perm;
    int c;
    extern char *optarg;
    extern int optind;
    char *pstr, fname[100], from[100], to[100], *strcpy();
    char *strcpy(), *strrchr();

    /*
     * get start and end bytes, if given.
     */
    start = -1;
    end = -1;

    while ((c = getopt(argc, argv, "?s:e:")) != EOF)
    {
	switch (c)
	{
	case 's':
	    start = atol(optarg);
	    break;
	case 'e':
	    end = atol(optarg);
	    break;
	default:
	    fprintf(stderr, "%s\n", USAGE);
	    exit(2);
	}
    }

    /* check how many files were given. */
    if ((argc - optind) != 2)
    {
	fprintf(stderr, "%s\n", USAGE);
	exit(2);
    }

    /* copy file names */
    strcpy(from, argv[optind++]);
    strcpy(to, argv[optind]);

    /* from file should be a regular file */
    if (stat(from, &finfofrom) == -1)
    {
	fprintf(stderr, "cpoff: unable to read file %s\n", from);
	exit(2);
    }

    if ((finfofrom.st_mode & S_IFMT) != S_IFREG)
    {
	fprintf(stderr, "cpoff: from file %s is not a regular file.\n", from);
	exit(2);
    }

    /* save permissions */
    perm = finfofrom.st_mode & 0777;

    /* set defaults, if required */
    if (start == -1) start = 0;
    if (end == -1) end = finfofrom.st_size;

    /* check start and end bytes for copy of from file */
    if (start < 0 || end < 0 || start > end || 
        start > finfofrom.st_size || end > finfofrom.st_size)
    {
	fprintf(stderr, "cpoff: bad start or end bytes.\n");
	exit(2);
    }

    /* to file can be a directory, or non-existant */
    if (stat(to, &finfoto) != -1)
    {
	/* file exists, check if a directory */
	if ((finfoto.st_mode & S_IFMT) == S_IFDIR)
	{
	    pstr = strrchr(from, '/');
	    if (pstr != NULL)
	    {
		pstr++;
	    }
	    else
	    {
		pstr = from;
	    }
	    strcat(to, "/");
	    strcat(to, pstr);
	}
    }

    /* check files are not the same */
    if (finfofrom.st_dev == finfoto.st_dev &&
        finfofrom.st_ino == finfoto.st_ino)
    {
	fprintf(stderr,"cpoff: %s and %s are identical.\n", from, to);
	exit(2);
    }

    /* copy the files */
    if (! cpoff(from , to, start, end, perm))
    {
	fprintf(stderr, "cpoff: unable to copy %s to %s.\n", from, to);
	exit(2);
    }

    /* exit with success */
    return (0);
}
