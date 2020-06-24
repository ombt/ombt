#include <stdio.h>
#include "addmsg.h"

main()
{
    MESSAGE m;
    int x, y;
    void syserr();

    m.clientkey = getpid();
    for (x = 1; x <= 5; x++)
    {
	for (y = 1; y <= 5; y++)
	{
	    m.x = x;
	    m.y = y;
	    if (send(ADDERKEY, &m, sizeof(m)) == -1) syserr("send");
	    if (receive(m.clientkey, &m, sizeof(m)) == -1) syserr("receive");
	    if (x + y != m.sum)
	    {
		fprintf(stderr, "addition error in client %ld\n",
			m.clientkey);
		rmqueue(m.clientkey);
		exit(1);
	    }
	}
    }
    rmqueue((long)getpid());
    fprintf(stderr, "work ok client %ld\n", m.clientkey);
    exit(0);
}
