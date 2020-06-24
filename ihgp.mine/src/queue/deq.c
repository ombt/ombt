#include <stdio.h>

extern int errno;

main(argc,argv)
int argc;
char **argv;
{
	if (argc <= 1)
	{
		fprintf(stderr, "usage: deq dirname\n");
		exit(2);
	}
	if (Enqueue(argv[1]) != 0)
	{
		fprintf(stderr, "error in dequeue, errno = %d\n", errno);
		exit(2);
	}
	fprintf(stdout, "job was queued. sleep time !!!\n");
#ifndef DEBUG
	sleep(10);
#endif
	fprintf(stdout, "i'm awake again !!!\n");
	if (Dequeue(argv[1]) != 0)
	{
		fprintf(stderr, "error in dequeue, errno = %d\n", errno);
		exit(2);
	}
	exit(0);
}
