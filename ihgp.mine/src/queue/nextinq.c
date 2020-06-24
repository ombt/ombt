#include <stdio.h>

extern int errno;

main(argc,argv)
int argc;
char **argv;
{
	int maxqtime, qstatus;

	if (argc <= 2)
	{
		fprintf(stderr, "usage: nextinq dirname maxqtime\n");
		exit(2);
	}
	maxqtime = atoi(argv[2]);
	fprintf(stdout, "max q time is %d\n", maxqtime);
	if (Enqueue(argv[1]) != 0)
	{
		fprintf(stderr, "error in dequeue, errno = %d\n", errno);
		exit(2);
	}
	fprintf(stdout, "job was queued !!!\n");
	while ((qstatus = NextInQueue(argv[1], maxqtime)) == 0)
	{
		fprintf(stderr, "golly !!! i gotta wait !!!\n");
		sleep(1);
	}
	if (qstatus == 1)
	{
		fprintf(stderr, "my turn !!!\n");
	}
	else
	{
		fprintf(stderr, "error in nextinqueue, errno = %d\n", errno);
		(void) Dequeue(argv[1]);
		exit(2);
	}
	if (Dequeue(argv[1]) != 0)
	{
		fprintf(stderr, "error in dequeue, errno = %d\n", errno);
		exit(2);
	}
	exit(0);
}
