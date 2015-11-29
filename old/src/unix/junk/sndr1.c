#include <stdio.h>
#include "message.h"

main()
{
    MESSAGE m;
    void syserr();

    setbuf(stdout, NULL);
    m.pid = getpid();
    for (m.number = 1; m.number <= 4; m.number++)
    {
	sleep(1);
	printf("sender %d is sending ...\n", getpid());
	if (send(1000L, &m, sizeof(m)) == -1) syserr("send");
	printf("sender %d just sent ...\n", getpid());
    }
    exit(0);
}
