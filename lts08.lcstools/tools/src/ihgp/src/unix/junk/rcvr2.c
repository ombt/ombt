#include <stdio.h>
#include <signal.h>
#include "addmsg.h"

void byebye(sig)
int sig;
{
    setbuf(stderr, NULL);
    fprintf(stderr, "receive: signal caught was %d.\n", sig);
    (void) rmqueue(ADDERKEY);
    exit(1);
}

main()
{
    extern int errno;
    int ret;
    MESSAGE m;
    void syserr();

    signal(SIGHUP, byebye);
    signal(SIGINT, byebye);
    signal(SIGQUIT, byebye);
    signal(SIGTERM, byebye);
    setbuf(stdout, NULL);
    while ((ret = receive(ADDERKEY, &m, sizeof(m))) > 0)
    {
	m.sum = m.x + m.y;
	if (send(m.clientkey, &m, sizeof(m)) == -1)
	{
	    printf("can't send to %ld; errno = %d\n", m.clientkey, errno);
	}
    }
    rmqueue(1000L);
    if (ret == -1) syserr("receive");
    exit(0);
}
