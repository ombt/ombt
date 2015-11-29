#include <stdio.h>
#include <signal.h>
#include "message.h"

void byebye(sig)
int sig;
{
    setbuf(stderr, NULL);
    fprintf(stderr, "receive: signal caught was %d.\n", sig);
    (void) rmqueue(1000L);
    exit(1);
}

main()
{
    int ret;
    MESSAGE m;
    void syserr();

    signal(SIGHUP, byebye);
    signal(SIGINT, byebye);
    signal(SIGQUIT, byebye);
    signal(SIGTERM, byebye);
    setbuf(stdout, NULL);
    while ((ret = receive(1000L, &m, sizeof(m))) > 0)
    {
	printf("Received %d from %d\n", m.number, m.pid);
    }
    rmqueue(1000L);
    if (ret == -1) syserr("receive");
    exit(0);
}
