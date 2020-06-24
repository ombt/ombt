#include <stdio.h>

extern int errno;

main(argc,argv)
int argc;
char **argv;
{
	int sleeptime, maxqtime, qstatus;

	if (argc <= 3)
	{
		fprintf(stderr, "usage: qandwait dirname sleeptime maxqtime\n");
		exit(2);
	}
	sleeptime = atoi(argv[2]);
	maxqtime = atoi(argv[3]);
	if ((qstatus = QAndWait(argv[1], sleeptime, maxqtime)) == 1)
	{
		fprintf(stderr, "my turn !!!\n");
	}
	else if (qstatus < 0)
	{
		fprintf(stderr, "error in QAndWait, errno = %d\n", errno);
		(void) Dequeue(argv[1]);
		exit(2);
	}
	else
	{
		fprintf(stderr, "TIME OUT TURKEY\n");
	}
	if (Dequeue(argv[1]) != 0)
	{
		fprintf(stderr, "error in dequeue, errno = %d\n", errno);
		exit(2);
	}
	exit(0);
}
