#include <stdio.h>

extern int errno;

main(argc,argv)
int argc;
char **argv;
{
	if (argc <= 1)
	{
		fprintf(stderr, "usage: test dirname\n");
		exit(2);
	}
	if(Enqueue(argv[1]) != 0)
	{
		fprintf(stderr, "error in enqueue, errno = %d\n", errno);
		exit(2);
	}
	exit(0);
}
